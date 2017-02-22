
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    scrBgs((int)ScrBg::Size),
    scrRects((int)ScrPart::Size),
    scrImgs((int)ScrPart::Size),
    scrDatas((int)ScrPart::Size),
    map_index(0),
    sector(0),
    modifiedBgTiles(SCENERY_BG_TILE_NUM),
    modifiedCnfTiles(SCENERY_CNF_TILE_NUM),
    charSetData(new GfxData(GfxData::Type::Nothing, 256, 64)),
    ui(new Ui::MainWindow)
{
    /***   W I N D O W
     ******************************************************************************/
    ui->setupUi(this);
    setWindowTitle("Immensity Editor");
    setAttribute(Qt::WA_DeleteOnClose, true);

    /***   G F X   V E C T O R
     ******************************************************************************/
    initGv(gvSketchesCeilings, QString("Ceiling"),  GfxVector::Scope::Sketches, Qt::Horizontal, GfxImage::Mode::Ceiling,   GfxData::Type::Sprite   );
    initGv(gvSketchesFloors,   QString("Floor"),    GfxVector::Scope::Sketches, Qt::Horizontal, GfxImage::Mode::Floor,     GfxData::Type::Sprite   );
    initGv(gvSketchesWalls,    QString("Wall"),     GfxVector::Scope::Sketches, Qt::Vertical,   GfxImage::Mode::Wall,      GfxData::Type::Wall     );
    initGv(gvSketchesBgTiles,  QString("Tile"),     GfxVector::Scope::Sketches, Qt::Horizontal, GfxImage::Mode::Sketch,    GfxData::Type::Sketch   );
    initGv(gvSketchesCnfTiles, QString("CnF Tile"), GfxVector::Scope::Sketches, Qt::Horizontal, GfxImage::Mode::CnfSketch, GfxData::Type::CnfSketch);

    initGv(gvSceneryBgTiles,   QString("Tile"),     GfxVector::Scope::Scenery,  Qt::Horizontal, GfxImage::Mode::Sketch,    GfxData::Type::Sketch   );
    initGv(gvSceneryCnfTiles,  QString("CnF Tile"), GfxVector::Scope::Scenery,  Qt::Horizontal, GfxImage::Mode::CnfSketch, GfxData::Type::CnfSketch);

    /* Initialize gfx vectors of scenery */
    for(int v=0; v<SCENERY_BG_TILE_NUM; v++)
    {
        shared_ptr<GfxData> data(new GfxData(gvSceneryBgTiles.type(), 8*SCENERY_BG_TILE_COLS, 8*SCENERY_BG_TILE_ROWS));
        gvSceneryBgTiles.setDataAt((int)GfxData::Id::Append, data);
    }
    for(int v=0; v<SCENERY_CNF_TILE_NUM; v++)
    {
        shared_ptr<GfxData> data(new GfxData(gvSceneryCnfTiles.type(), 8*SCENERY_CNF_TILE_COLS, 8*SCENERY_CNF_TILE_ROWS));
        gvSceneryCnfTiles.setDataAt((int)GfxData::Id::Append, data);
    }

    charSetWindow = nullptr;

    /***   M E N U
     ******************************************************************************/
    connect(ui->actionEditSceneries,    SIGNAL(triggered()), this, SLOT(openEditSceneries())    );
    connect(ui->actionEditThemes,       SIGNAL(triggered()), this, SLOT(openEditThemes())       );

    connect(ui->actionSketchesCelinigs, SIGNAL(triggered()), this, SLOT(openSketchesCeilings()) );
    connect(ui->actionSketchesFloors,   SIGNAL(triggered()), this, SLOT(openSketchesFloors())   );
    connect(ui->actionSketchesWalls,    SIGNAL(triggered()), this, SLOT(openSketchesWalls())    );
    connect(ui->actionSketchesBgTiles,  SIGNAL(triggered()), this, SLOT(openSketchesBgTiles())  );
    connect(ui->actionSketchesCnfTiles, SIGNAL(triggered()), this, SLOT(openSketchesCnfTiles()) );
    connect(ui->actionSketchesOpenAll,  SIGNAL(triggered()), this, SLOT(openSketchesAll())      );
    connect(ui->actionSketchesCloseAll, SIGNAL(triggered()), this, SLOT(closeSketchesAll())     );

    connect(ui->actionSceneryCharSet,  SIGNAL(triggered()), this, SLOT(openSceneryCharSet())    );
    connect(ui->actionSceneryBgTiles,  SIGNAL(triggered()), this, SLOT(openSceneryBgTiles())    );
    connect(ui->actionSceneryCnfTiles, SIGNAL(triggered()), this, SLOT(openSceneryCnfTiles())   );

    connect(ui->actionMapSettings,     SIGNAL(triggered()), this, SLOT(settingsClicked())       );

    /***   F I L E
     ******************************************************************************/
    projPath = qApp->applicationDirPath()+"/project/";
    projExt = ".imm";
    projName = "test";
    projFile = new QFile(projPath+projName+projExt);

    loadGfxData();

    /***   M A P
     ******************************************************************************/
    for(int i=0; i<props.maps.size(); i++)
        ui->mapBox->addItem(props.maps.at(i)->name);

    connect(ui->newMap, SIGNAL(clicked()), this, SLOT(createNew()));
    connect(ui->checkDelete, SIGNAL(clicked(bool)), this, SLOT(enableDeletBtn(bool)));
    connect(ui->deleteMap, SIGNAL(clicked()), this, SLOT(deleteMap()));
    connect(ui->mapBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectMap(int)));


    /***   S C E N E
     ******************************************************************************/
    initScrPart(ScrPart::CeilingFg,  GfxData::Type::CnfSketch, 144,  24, GfxImage::Mode::Nothing);
    initScrPart(ScrPart::Ceiling,    GfxData::Type::Sprite,     24,  21, GfxImage::Mode::Ceiling);
    initScrPart(ScrPart::Background, GfxData::Type::Sketch,    120,  64, GfxImage::Mode::Nothing);
    initScrPart(ScrPart::Wall,       GfxData::Type::Sprite,     24, 105, GfxImage::Mode::Expanded);
    initScrPart(ScrPart::Floor,      GfxData::Type::Sprite,     24,  21, GfxImage::Mode::Floor);
    initScrPart(ScrPart::FloorFg,    GfxData::Type::CnfSketch, 144,  24, GfxImage::Mode::Nothing);

    scrHisC = new GfxHistory(scrDatas.at((int)ScrPart::CeilingFgC));
    scrHisB = new GfxHistory(scrDatas.at((int)ScrPart::BackgroundC));
    scrHisF = new GfxHistory(scrDatas.at((int)ScrPart::FloorFgC));

    backgroundModified = false;

    initScrRects();
    initScrBgs();

    scrBgs[(int)ScrBg::BorderL]->setBrush(QBrush(QColor(QRgb( Cfg::ins().val(Cfg::ViewSceneColor) ))));
    scrBgs[(int)ScrBg::BorderR]->setBrush(QBrush(QColor(QRgb( Cfg::ins().val(Cfg::ViewSceneColor) ))));

    for(int i=0; i<(int)ScrBg::Size; i++)
    {
        scrBgs[i]->setPen(QPen(Qt::NoPen));
        scrBgs[i]->setZValue(-2);
        ui->view->scene->addItem(scrBgs.at(i));
    }

    scrBgs[(int)ScrBg::BorderL]->setZValue(9);
    scrBgs[(int)ScrBg::BorderR]->setZValue(9);

    selTgt = new GfxRectItemSelection(GfxRectItemSelection::Type::Target);
    selTgt->setZValue(22);
    ui->view->scene->addItem(selTgt);

    connect(ui->view, SIGNAL(gfxEditorViewEnterEvent(QByteArray&,QPoint)), this, SLOT(dndEnter(QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewDrawEvent (QByteArray&,QPoint)), this, SLOT(dndDraw (QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewDropEvent (QByteArray&,QPoint)), this, SLOT(dndDrop (QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewLeaveEvent()), this, SLOT(dndLeave()));

    /***   G R I D
     ******************************************************************************/
    grid = new Grid(ui->view);
    createGrids();
    grid->fineVisible(ui->checkFineGrid->checkState());
    grid->mainVisible(ui->checkMainGrid->checkState());

    wallState   = ui->checkWall->checkState();
    playerState = ui->checkPlayer->checkState();
    wallGrp     = ui->view->scene->createItemGroup(wallItems);
    playerGrp   = ui->view->scene->createItemGroup(wallItems);
    playerGrp  ->setZValue(13);
    wallGrp    ->setZValue(14);

    connect(ui->checkFineGrid, SIGNAL(stateChanged(int)), grid, SLOT(fineVisible(int)));
    connect(ui->checkMainGrid, SIGNAL(stateChanged(int)), grid, SLOT(mainVisible(int)));
    connect(ui->checkWall,     SIGNAL(stateChanged(int)), this, SLOT(wallVisible(int)));
    connect(ui->checkPlayer,   SIGNAL(stateChanged(int)), this, SLOT(playerVisible(int)));

    /***   M I S C
     ******************************************************************************/

    themeEditor   = nullptr;
    sceneryEditor = nullptr;

    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)  modifiedBgTiles.append(i);
    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++) modifiedCnfTiles.append(i);

    scenery = new Scenery();
    refreshEditor();
}

MainWindow::~MainWindow()
{
    for(int i=0; i<wallItems.size(); i++)
    {
        wallGrp->removeFromGroup(wallItems.at(i));
        playerGrp->removeFromGroup(wallItems.at(i));
        delete wallItems.at(i);
    }
    wallItems.clear();
    delete grid;

    gvSketchesCeilings.close();
    gvSketchesFloors.close();
    gvSketchesWalls.close();
    gvSketchesBgTiles.close();
    gvSketchesCnfTiles.close();

    if(charSetWindow) charSetWindow->close();
    gvSceneryBgTiles.close();
    gvSceneryCnfTiles.close();

    if(themeEditor) themeEditor->close();
    if(sceneryEditor) sceneryEditor->close();

    saveGfxData();

    delete projFile;
    delete ui;
}

void MainWindow::initGv(GfxVector &gv,
                        QString title,
                        GfxVector::Scope scope,
                        Qt::Orientation ori,
                        GfxImage::Mode mode,
                        GfxData::Type type)
{
    gv.setTitle(title);
    gv.setScope(scope);
    gv.setOri(ori); /* orientation of tiles (not of the browser) */
    gv.setMode(mode);
    gv.setType(type);
}

void MainWindow::selectMap(int index)
{
    /* Clearing combobox also triggers the signal which is connected to this slot,
     * thus we have to be sure that the combobox is not empty!
     */
    if(0 <= index)
    {
        map_index = index;
        refreshEditor();
    }
}

void MainWindow::createNew()
{
    QString name = "";
    int copy = -1;
    NewDialog dialog((QVector<VE *> *) &props.maps, &name, &copy, this);
    dialog.exec();

    if(!name.isEmpty())
    {
        Map *tMap;
        if(-1 == copy) tMap = new Map();
        else tMap = props.maps.at(copy)->copy();
        tMap->name = name;
        map_index = props.maps.size();
        props.maps.append(tMap);
        props.useSceneryInMap(tMap->scenery_index,props.maps.size()-1);
        int box_last_index = ui->mapBox->count();
        ui->mapBox->addItem(props.maps.at(map_index)->name, QVariant(map_index));
        ui->mapBox->setCurrentIndex(box_last_index);
        mapSettingsChanged();
    }
}

void MainWindow::enableDeletBtn(bool b) { ui->deleteMap->setEnabled(b); }

void MainWindow::deleteMap()
{
    if(props.deleteMap(map_index))
        ui->mapBox->removeItem(map_index); // this will trigger the currentIndexChanged signal
}

void MainWindow::mapSettingsChanged()
{
    int i = ui->mapBox->currentIndex();
    QString s = props.maps.at(map_index)->name;
    ui->mapBox->setItemText(i, s);
    refreshEditor();
}

void MainWindow::themeChanged(int i)
{
    if(props.maps.at(map_index)->theme_index == i)
        refreshEditor();
}

void MainWindow::sceneryChanged(int i)
{
    if(props.maps.at(map_index)->scenery_index == i)
        refreshEditor();
}

void MainWindow::refreshEditor()
{
    initScrBgColors();
    bgLoad();
    refreshSceneryBrowsers();
    ui->checkDelete->setChecked(false);
    enableDeletBtn(false);
}
