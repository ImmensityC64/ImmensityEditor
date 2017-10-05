
#include "char_set.h"

CharSet::CharSet(shared_ptr<GfxData> init, QWidget *parent) :
    QWidget(parent),
    gfx_data(init),
    ui(new Ui::CharSet)
{
    ui->setupUi(this);

    /* Scene */

    rect = new GfxRectItemImage(init);      /* 'view->scene' will delete it */
    img  = rect->getImg();                  /*        'rect' will delete it */
    img->setMask(true);
    bg   = new GfxRectItemBackground(init); /* 'view->scene' will delete it */
    bg->setZValue(-1);

    ui->view->scene->addItem(rect);
    ui->view->scene->addItem(bg);

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

    img->refresh();

    /* Grids */

    grid = new Grid(ui->view, img->width(), img->height());
    createGrids();
    grid->fineVisible(ui->checkFineGrid->checkState());
    grid->mainVisible(ui->checkMainGrid->checkState());

    connect(ui->checkFineGrid, SIGNAL(stateChanged(int)), grid, SLOT(fineVisible(int)));
    connect(ui->checkMainGrid, SIGNAL(stateChanged(int)), grid, SLOT(mainVisible(int)));
    connect(ui->view, SIGNAL(gfxEditorViewScaleEvent()), grid, SLOT(vrfyVisibility()));
}

CharSet::~CharSet()
{
    delete grid;
}

void CharSet::refresh(shared_ptr<GfxData> data)
{
    gfx_data->load(data);
    img->refresh();
}

/****    G R I D S
 ******************************************************************************/

void CharSet::createGrids(void)
{
    grid->createSeries(Grid::Type::Fine,8,8);
    grid->createMainLine(0,16,256,16);
}
