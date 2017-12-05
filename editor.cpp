
#include "editor.h"

Editor::Editor(shared_ptr<GfxData> init, int index, QWidget *parent) :
    QMainWindow(parent),
    props(Props::ins()),
    data(new GfxData(init)), /* GfxData 'data' is a copy of 'init' */
    ui(new Ui::Editor)
{
    ui->setupUi(this);
    I = index;

    src = init;
    his = new GfxHistory(data);

    /* Scene */

    showSelTgt = true;
    rect = new GfxRectItemImage(data);      /* 'view->scene' will delete it */
    img  = rect->getImg();                  /*        'rect' will delete it */
    img->setMask(true);
    bg   = new GfxRectItemBackground(data); /* 'view->scene' will delete it */
    bg->setZValue(-1);
    selSrc = new GfxRectItemSelection(GfxRectItemSelection::Type::Source);
    selTgt = new GfxRectItemSelection(GfxRectItemSelection::Type::Target);
    selSrc->setZValue(21);
    selTgt->setZValue(22);

    ui->view->scene->addItem(rect);
    ui->view->scene->addItem(bg);
    ui->view->scene->addItem(selSrc);
    ui->view->scene->addItem(selTgt);

    int w=init->imgW(), h=init->imgH();
    //ui->view->setSceneRect(0,0,w,h);
    ui->view->centerOn(w/2,h/2);
    ui->view->background->setRect(0,0,w,h);

    qreal x_factor = EditorViewSize / w;
    qreal y_factor = EditorViewSize / h;
    qreal factor = x_factor<y_factor ? x_factor : y_factor;
    ui->view->scale(factor, factor);

    connect(img, SIGNAL(refreshHappened()), rect, SLOT(refresh()));
    connect(img, SIGNAL(refreshHappened()), bg,   SLOT(refresh()));
    connect(img, SIGNAL(selectSrc(int,int,int,int)), selSrc, SLOT(select(int,int,int,int)));
    connect(img, SIGNAL(selectTgt(int,int,int,int)), selTgt, SLOT(select(int,int,int,int)));
    connect(ui->view, SIGNAL(gfxEditorViewEnterEvent(QByteArray&,QPoint)), this, SLOT(dndEnter(QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewDrawEvent (QByteArray&,QPoint)), this, SLOT(dndDraw (QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewDropEvent (QByteArray&,QPoint)), this, SLOT(dndDrop (QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewLeaveEvent()), this, SLOT(dndLeave()));

    img->refresh();

    /* Toolbox */

    ui->toolbox->setView(ui->view);

    /* Buttons */

    scUndo = new QShortcut(QKeySequence("Ctrl+Z"), this);
    scRedo = new QShortcut(QKeySequence("Ctrl+Y"), this);
    connect(scUndo, SIGNAL(activated()), this, SLOT(undo()));
    connect(scRedo, SIGNAL(activated()), this, SLOT(redo()));
    connect(ui->buttonUndo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(ui->buttonRedo, SIGNAL(clicked()), this, SLOT(redo()));

    scApply  = new QShortcut(QKeySequence("Ctrl+S"), this);
    scRevert = new QShortcut(QKeySequence("Ctrl+R"), this);
    connect(scApply,  SIGNAL(activated()), this, SLOT(apply()));
    connect(scRevert, SIGNAL(activated()), this, SLOT(revert()));
    connect(ui->buttonSave,   SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->buttonReload, SIGNAL(clicked()), this, SLOT(revert()));

    /* Grids */

    grid = new Grid(ui->view, img->width(), img->height());
    if( img->mode()!=GfxImage::Mode::Sketch && img->mode()!=GfxImage::Mode::CnfSketch )
        grid->setDependsOnScale(true);
    createGrids();
    grid->fineVisible(ui->checkFineGrid->checkState());
    grid->mainVisible(ui->checkMainGrid->checkState());

    connect(ui->checkFineGrid, SIGNAL(stateChanged(int)), grid, SLOT(fineVisible(int)));
    connect(ui->checkMainGrid, SIGNAL(stateChanged(int)), grid, SLOT(mainVisible(int)));
    connect(ui->view, SIGNAL(gfxEditorViewScaleEvent()), grid, SLOT(vrfyVisibility()));
}

Editor::~Editor()
{
    emit destroyed(I);
    delete grid;
    delete scUndo;
    delete scRedo;
    delete scApply;
    delete scRevert;
    delete his;
}

void Editor::addSpecWidget(void)
{
    ui->controlLayout->insertWidget(2,spec);
}

void Editor::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Closing "+windowTitle());
    msgBox.setInformativeText("Do you want to apply your changes?");
    msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Apply);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Apply:
          apply();
          event->accept();
          break;
      case QMessageBox::Discard:
          event->accept();
          break;
      case QMessageBox::Cancel:
          event->ignore();
          break;
      default:
          // should never be reached
          break;
    }
}

/****    H I S T O R Y
 ******************************************************************************/

void Editor::load(void) { his->load(data); }
void Editor::save(void) { his->save(data); }
void Editor::undo(void)
{
    his->undo(data);
    img->refresh();
}
void Editor::redo(void)
{
    his->redo(data);
    img->refresh();
}
/* Apply changes, store 'data' into 'src'. */
void Editor::apply(void)
{
    shared_ptr<GfxData> d = src.lock();
    if(d)
    {
        d->load(data);
        d->dataChanged();
    }
    /* TODO else{ store deleted data again } */
}
/* Revert changes, reload 'src' into 'data'. */
void Editor::revert(void)
{
    shared_ptr<GfxData> d = src.lock();
    if(d)
    {
        data->load(d);
        save();
        img->refresh();
    }
    /* TODO notificiation pop-up, source does not exist */
}

/****    T O O L   C O M M A N D
 ******************************************************************************/

void Editor::command(Command cmd)
{
    switch(cmd)
    {
    case Command::ShowSelSrc: selSrc->show(); break;
    case Command::HideSelSrc: selSrc->hide(); break;
    case Command::ShowSelTgt: selTgt->show(); break;
    case Command::HideSelTgt: selTgt->hide(); break;
    case Command::LoadHistory: load(); break;
    case Command::SaveHistory: save(); break;
    default: /* do nothing */ break;
    }
}

/****    D R A G  &  D R O P
 ******************************************************************************/

void Editor::dndEnter(QByteArray &src, QPoint p)
{
    if(showSelTgt) { selTgt->show(); }
    load();
    img->importDataToImage(src,p);
    img->refresh();
}

void Editor::dndDraw(QByteArray &src, QPoint p)
{
    load();
    img->importDataToImage(src,p);
    img->refresh();
}

void Editor::dndDrop(QByteArray &src, QPoint p)
{
    load();
    img->importDataToImage(src,p);
    img->refresh();
    save();
    selTgt->hide();
}

void Editor::dndLeave(void)
{
    load();
    img->refresh();
    selTgt->hide();
}

/****    G R I D S
 ******************************************************************************/

void Editor::createGrids(void)
{
    switch (img->mode()) {
    case GfxImage::Mode::Ceiling:
    {
        grid->createSeries(Grid::Type::Fine,2,1, 0, 0,48, 5);
        grid->createSeries(Grid::Type::Fine,2,1, 1, 5,49, 9);
        grid->createSeries(Grid::Type::Fine,2,1, 2, 9,50,13);
        grid->createSeries(Grid::Type::Fine,2,1, 3,13,51,17);
        grid->createSeries(Grid::Type::Fine,2,1, 4,17,52,21);
        grid->createMainRect( 0, 0,48,3);
        grid->createMainRect(47, 5, 2,4);
        grid->createMainRect(46, 9, 4,4);
        grid->createMainRect(45,13, 6,4);
        grid->createMainRect(44,17, 8,2);
        grid->createMainRect( 4,19,48,2);
        break;
    }
    case GfxImage::Mode::Floor:
    {
        grid->createSeries(Grid::Type::Fine,2,1, 4, 0,52, 4);
        grid->createSeries(Grid::Type::Fine,2,1, 3, 4,51, 8);
        grid->createSeries(Grid::Type::Fine,2,1, 2, 8,50,12);
        grid->createSeries(Grid::Type::Fine,2,1, 1,12,49,16);
        grid->createSeries(Grid::Type::Fine,2,1, 0,16,48,21);
        grid->createMainRect( 4, 0,48,2);
        grid->createMainRect(44, 2, 8,2);
        grid->createMainRect(45, 4, 6,4);
        grid->createMainRect(46, 8, 4,4);
        grid->createMainRect(47,12, 2,4);
        grid->createMainRect( 0,18,48,3);
        break;
    }
    case GfxImage::Mode::Expanded:
    {
        grid->createSeries(Grid::Type::Fine,2,1,2,0,50,21);
        break;
    }
    case GfxImage::Mode::Wall:
    {
        grid->createSeries(Grid::Type::Fine,2,1);
        grid->createMainLine(0,21,48,21);
        grid->createMainLine(0,42,48,42);
        grid->createMainLine(0,63,48,63);
        grid->createMainLine(0,84,48,84);
        grid->createMainRect(0,  0,48,5);
        grid->createMainRect(0,101,48,4);
        break;
    }
    case GfxImage::Mode::Sketch:
    {
        grid->createSeries(Grid::Type::Fine,8,8);
        grid->createSeries(Grid::Type::Main,40,64);
        break;
    }
    case GfxImage::Mode::CnfSketch:
    {
        grid->createSeries(Grid::Type::Fine,8,8);
        grid->createSeries(Grid::Type::Main,48,24);
        break;
    }
    default:
        break;
    } /* switch */
}
