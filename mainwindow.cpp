
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    props(Props::ins()),
    scrBgs((int)ScrBg::Size),
    scrRects((int)ScrPart::Size),
    scrImgs((int)ScrPart::Size),
    scrDatas((int)ScrPart::Size),
    map_index(0),
    sector(0), sectorSelectionL(0), sectorSelectionR(0),
    remapSectors(SCENERY_MAP_SECTORS),
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
    initGv(gvScenerySprites,   QString("Sprite"),   GfxVector::Scope::Scenery,  Qt::Horizontal, GfxImage::Mode::Expanded,  GfxData::Type::Sprite   );
    initGv(gvSceneryWalls,     QString("Wall"),     GfxVector::Scope::Scenery,  Qt::Vertical,   GfxImage::Mode::Wall,      GfxData::Type::Wall     );

    /* Initialize gfx vectors of scenery */
    for(int v=0; v<SCENERY_BG_TILE_NUM; v++)
    {
        shared_ptr<GfxData> data(new GfxData(gvSceneryBgTiles.type(), 8*SCENERY_BG_TILE_COLS, 8*SCENERY_BG_TILE_ROWS));
        gvSceneryBgTiles.setDataAt((int)GfxData::Id::Append, data);
        connect(data.get(), SIGNAL(dataChanged()), this, SLOT(tileChanged()));
    }
    for(int v=0; v<SCENERY_CNF_TILE_NUM; v++)
    {
        shared_ptr<GfxData> data(new GfxData(gvSceneryCnfTiles.type(), 8*SCENERY_CNF_TILE_COLS, 8*SCENERY_CNF_TILE_ROWS));
        gvSceneryCnfTiles.setDataAt((int)GfxData::Id::Append, data);
        connect(data.get(), SIGNAL(dataChanged()), this, SLOT(tileChanged()));
    }
    for(int v=0; v<SCENERY_SPRITE_NUM; v++)
    {
        shared_ptr<GfxData> data(new GfxData(gvScenerySprites.type()));
        gvScenerySprites.setDataAt((int)GfxData::Id::Append, data);
        connect(data.get(), SIGNAL(dataChanged()), this, SLOT(spriteChanged()));
    }
    for(int v=0; v<SCENERY_WALL_NUM; v++)
    {
        shared_ptr<GfxData> data(new GfxData(gvSceneryWalls.type()));
        gvSceneryWalls.setDataAt((int)GfxData::Id::Append, data);
        connect(data.get(), SIGNAL(dataChanged()), this, SLOT(wallChanged()));
    }

    charSet = nullptr;
    browseSketches = nullptr;
    browseScenery  = nullptr;

    /***   M E N U
     ******************************************************************************/
    connect(ui->actionFileOpen,   SIGNAL(triggered()), this, SLOT(openProject())  );
    connect(ui->actionFileSave,   SIGNAL(triggered()), this, SLOT(saveProject())  );
    connect(ui->actionFileSaveAs, SIGNAL(triggered()), this, SLOT(saveAsProject()));
    connect(ui->actionFileRevert, SIGNAL(triggered()), this, SLOT(loadProject())  );
    connect(ui->actionFileExit,   SIGNAL(triggered()), this, SLOT(close())        );

    connect(ui->actionEditSceneries, SIGNAL(triggered()), this, SLOT(openEditSceneries()));
    connect(ui->actionEditThemes,    SIGNAL(triggered()), this, SLOT(openEditThemes())   );

    connect(ui->actionBrowseSketches, SIGNAL(triggered()), this, SLOT(openBrowseSketches()));
    connect(ui->actionBrowseScenery,  SIGNAL(triggered()), this, SLOT(openBrowseScenery()) );

    connect(ui->actionMapSettings, SIGNAL(triggered()), this, SLOT(settingsClicked()));

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
    initScrPart(ScrPart::Wall,       GfxData::Type::Wall,       24, 105, GfxImage::Mode::Wall);
    initScrPart(ScrPart::Floor,      GfxData::Type::Sprite,     24,  21, GfxImage::Mode::Floor);
    initScrPart(ScrPart::FloorFg,    GfxData::Type::CnfSketch, 144,  24, GfxImage::Mode::Nothing);

    scrHisC = new GfxHistory(scrDatas.at((int)ScrPart::CeilingFgC));
    scrHisB = new GfxHistory(scrDatas.at((int)ScrPart::BackgroundC));
    scrHisF = new GfxHistory(scrDatas.at((int)ScrPart::FloorFgC));
    scrHisCSpr = new GfxHistory(scrDatas.at((int)ScrPart::CeilingC));
    scrHisWSpr = new GfxHistory(scrDatas.at((int)ScrPart::WallC));
    scrHisFSpr = new GfxHistory(scrDatas.at((int)ScrPart::FloorC));

    editor_img_c_modified = false;
    editor_img_b_modified = false;
    editor_img_f_modified = false;
    editor_img_s_modified = false;
    editor_img_t_modified = false;

    initScrRects();
    initScrBgs();

    scrRects[(int)ScrPart::CeilingL]->setZValue(10);
    scrRects[(int)ScrPart::CeilingC]->setZValue(10);
    scrRects[(int)ScrPart::CeilingR]->setZValue(10);
    scrRects[(int)ScrPart::FloorL]->setZValue(10);
    scrRects[(int)ScrPart::FloorC]->setZValue(10);
    scrRects[(int)ScrPart::FloorR]->setZValue(10);

    scrBgs[(int)ScrBg::BorderL]->setBrush(QBrush(QColor(QRgb( Cfg::ins().val(Cfg::ViewSceneColor) ))));
    scrBgs[(int)ScrBg::BorderR]->setBrush(QBrush(QColor(QRgb( Cfg::ins().val(Cfg::ViewSceneColor) ))));

    for(int i=0; i<(int)ScrBg::Size; i++)
    {
        scrBgs[i]->setPen(QPen(Qt::NoPen));
        scrBgs[i]->setZValue(-2);
        ui->view->scene->addItem(scrBgs.at(i));
    }

    scrBgs[(int)ScrBg::BorderL]->setZValue(20);
    scrBgs[(int)ScrBg::BorderR]->setZValue(20);

    selTgt = new GfxRectItemSelection(GfxRectItemSelection::Type::Target);
    selTgt->setZValue(22);
    ui->view->scene->addItem(selTgt);

    connect(ui->view, SIGNAL(gfxEditorViewEnterEvent(QByteArray&,QPoint)), this, SLOT(dndEnter(QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewDrawEvent (QByteArray&,QPoint)), this, SLOT(dndDraw (QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewDropEvent (QByteArray&,QPoint)), this, SLOT(dndDrop (QByteArray&,QPoint)));
    connect(ui->view, SIGNAL(gfxEditorViewLeaveEvent()), this, SLOT(dndLeave()));
    connect(ui->view, SIGNAL(gfxEditorViewPressEvent(QPoint,int)), this, SLOT(editorMousePressEvent(QPoint,int)));
    connect(ui->colorCeiling, SIGNAL(sendColor(quint8)), this, SLOT(receiveColorCeiling(quint8)));
    connect(ui->colorWall,    SIGNAL(sendColor(quint8)), this, SLOT(receiveColorWall(quint8)));
    connect(ui->colorFloor,   SIGNAL(sendColor(quint8)), this, SLOT(receiveColorFloor(quint8)));

    /***   G R I D
     ******************************************************************************/
    grid = new Grid(ui->view);
    playerState   = ui->checkPlayer->checkState();
    wallGrp       = ui->view->scene->createItemGroup(wallItems);
    wallGridGrp   = ui->view->scene->createItemGroup(wallItems);
    playerGrp     = ui->view->scene->createItemGroup(wallItems);
    wallGrp->addToGroup(scrRects[(int)ScrPart::WallL]);
    wallGrp->addToGroup(scrRects[(int)ScrPart::WallC]);
    wallGrp->addToGroup(scrRects[(int)ScrPart::WallR]);
    playerGrp  ->setZValue(13);
    wallGrp    ->setZValue(14);
    wallGridGrp->setZValue(15);
    createGrids();

    grid->fineVisible(ui->checkFineGrid->checkState());
    grid->mainVisible(ui->checkMainGrid->checkState());
    wallGridVisible(ui->checkWallGrid->checkState());
    wallVisible(ui->checkWall->checkState());
    playerVisible(ui->checkPlayer->checkState());

    connect(ui->checkFineGrid, SIGNAL(stateChanged(int)), grid, SLOT(fineVisible(int)));
    connect(ui->checkMainGrid, SIGNAL(stateChanged(int)), grid, SLOT(mainVisible(int)));
    connect(ui->checkWallGrid, SIGNAL(stateChanged(int)), this, SLOT(wallGridVisible(int)));
    connect(ui->checkWall,     SIGNAL(stateChanged(int)), this, SLOT(wallVisible(int)));
    connect(ui->checkPlayer,   SIGNAL(stateChanged(int)), this, SLOT(playerVisible(int)));

    /***   M I S C
     ******************************************************************************/

    themeEditor   = nullptr;
    sceneryEditor = nullptr;
    sectorCEditor = nullptr;
    sectorBEditor = nullptr;
    sectorFEditor = nullptr;

    QValidator *validator = new QIntValidator(0, 84, this);
    ui->sector->setValidator(validator);
    ui->sectorSlider->setMinimum(0);
    ui->sectorSlider->setMaximum(84);
    connect(ui->sector, SIGNAL(textEdited(QString)), SLOT(setSector(QString)));
    connect(ui->sectorSlider, SIGNAL(valueChanged(int)), this, SLOT(setSector(int)));
    connect(ui->sectorInc, SIGNAL(clicked()), this, SLOT(incSector()));
    connect(ui->sectorDec, SIGNAL(clicked()), this, SLOT(decSector()));
    connect(ui->btnSelectionL, SIGNAL(clicked()), this, SLOT(setSectorSelectionL()));
    connect(ui->btnSelectionR, SIGNAL(clicked()), this, SLOT(setSectorSelectionR()));
    connect(ui->btnMove, SIGNAL(clicked()), this, SLOT(remapSectorsForMove()));
    connect(ui->btnCopy, SIGNAL(clicked()), this, SLOT(remapSectorsForCopy()));

    connect(ui->btnReload, SIGNAL(clicked()), this, SLOT(refreshEditor()));
    connect(ui->btnApply,  SIGNAL(clicked()), this, SLOT(saveModifications()));

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

    gvSceneryBgTiles.close();
    gvSceneryCnfTiles.close();
    gvScenerySprites.close();
    gvSceneryWalls.close();

    if(themeEditor) themeEditor->close();
    if(sceneryEditor) sceneryEditor->close();

    if(browseSketches) browseSketches->close();
    if(browseScenery) browseScenery->close();

    if(sectorCEditor) sectorCEditor->close();
    if(sectorBEditor) sectorBEditor->close();
    if(sectorFEditor) sectorFEditor->close();

    delete projFile;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Exiting Immensity Editor");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Save:
          saveGfxData();
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

/****    I N I T
 ******************************************************************************/

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

void MainWindow::initScrPart(ScrPart part,
                             GfxData::Type type,
                             int width,
                             int height,
                             GfxImage::Mode mode)
{
    /* Initialize L, C and R */
    for(int i=0; i<=2; i++)
    {
        int index = (int)part + i;
        scrDatas[index] = shared_ptr<GfxData>(new GfxData(type, width, height));
        if(GfxImage::Mode::Ceiling == mode || GfxImage::Mode::Floor == mode)
        {
            scrRects[index] = new GfxRectItemImage(scrDatas.at(index), i-1);
            scrImgs[index] = scrRects.at(index)->getImg();
            scrImgs[index]->setMode(mode);
        }
        else
        {
            scrRects[index] = new GfxRectItemImage(scrDatas.at(index));
            scrImgs[index] = scrRects.at(index)->getImg();
        }
        ui->view->scene->addItem(scrRects.at(index));
        connect(scrImgs[index], SIGNAL(refreshHappened()), scrRects[index], SLOT(refresh()));
    }
}

void MainWindow::initScrRects()
{
    scrRects[(int)ScrPart::CeilingFgL] ->setRect(-216,  -72, 144,  24);
    scrRects[(int)ScrPart::CeilingFgC] ->setRect( -72,  -72, 144,  24);
    scrRects[(int)ScrPart::CeilingFgR] ->setRect(  72,  -72, 144,  24);
    scrRects[(int)ScrPart::CeilingL]   ->setRect(-166,  -51,  64,  21);
    scrRects[(int)ScrPart::CeilingC]   ->setRect( -32,  -51,  64,  21);
    scrRects[(int)ScrPart::CeilingR]   ->setRect( 102,  -51,  64,  21);
    scrRects[(int)ScrPart::BackgroundL]->setRect(-180,  -32, 120,  64);
    scrRects[(int)ScrPart::BackgroundC]->setRect( -60,  -32, 120,  64);
    scrRects[(int)ScrPart::BackgroundR]->setRect(  60,  -32, 120,  64);
    scrRects[(int)ScrPart::WallL]      ->setRect(-168,  -53,  48, 105);
    scrRects[(int)ScrPart::WallC]      ->setRect( -24,  -53,  48, 105);
    scrRects[(int)ScrPart::WallR]      ->setRect( 120,  -53,  48, 105);
    scrRects[(int)ScrPart::FloorL]     ->setRect(-166,   30,  64,  21);
    scrRects[(int)ScrPart::FloorC]     ->setRect( -32,   30,  64,  21);
    scrRects[(int)ScrPart::FloorR]     ->setRect( 102,   30,  64,  21);
    scrRects[(int)ScrPart::FloorFgL]   ->setRect(-216,   48, 144,  24);
    scrRects[(int)ScrPart::FloorFgC]   ->setRect( -72,   48, 144,  24);
    scrRects[(int)ScrPart::FloorFgR]   ->setRect(  72,   48, 144,  24);

    scrRects[(int)ScrPart::CeilingFgL] ->setOffset(QPoint(-216,  -72));
    scrRects[(int)ScrPart::CeilingFgC] ->setOffset(QPoint( -72,  -72));
    scrRects[(int)ScrPart::CeilingFgR] ->setOffset(QPoint(  72,  -72));
    scrRects[(int)ScrPart::CeilingL]   ->setOffset(QPoint(-166,  -51));
    scrRects[(int)ScrPart::CeilingC]   ->setOffset(QPoint( -24,  -51));
    scrRects[(int)ScrPart::CeilingR]   ->setOffset(QPoint( 102,  -51));
    scrRects[(int)ScrPart::BackgroundL]->setOffset(QPoint(-180,  -32));
    scrRects[(int)ScrPart::BackgroundC]->setOffset(QPoint( -60,  -32));
    scrRects[(int)ScrPart::BackgroundR]->setOffset(QPoint(  60,  -32));
    scrRects[(int)ScrPart::WallL]      ->setOffset(QPoint(-168,  -53));
    scrRects[(int)ScrPart::WallC]      ->setOffset(QPoint( -24,  -53));
    scrRects[(int)ScrPart::WallR]      ->setOffset(QPoint( 120,  -53));
    scrRects[(int)ScrPart::FloorL]     ->setOffset(QPoint(-166,   30));
    scrRects[(int)ScrPart::FloorC]     ->setOffset(QPoint( -24,   30));
    scrRects[(int)ScrPart::FloorR]     ->setOffset(QPoint( 102,   30));
    scrRects[(int)ScrPart::FloorFgL]   ->setOffset(QPoint(-216,   48));
    scrRects[(int)ScrPart::FloorFgC]   ->setOffset(QPoint( -72,   48));
    scrRects[(int)ScrPart::FloorFgR]   ->setOffset(QPoint(  72,   48));

    scrRects[(int)ScrPart::WallL]->setZValue(5);
    scrRects[(int)ScrPart::WallC]->setZValue(5);
    scrRects[(int)ScrPart::WallR]->setZValue(5);
}

void MainWindow::initScrBgs()
{
    scrBgs[(int)ScrBg::CeilingFg]  = new QGraphicsRectItem(-152, -72, 304,  24);
    scrBgs[(int)ScrBg::Background] = new QGraphicsRectItem(-152, -32, 304,  64);
    scrBgs[(int)ScrBg::FloorFg]    = new QGraphicsRectItem(-152,  48, 304,  24);

    scrBgs[(int)ScrBg::Ceiling0]   = new QGraphicsRectItem(-152, -48, 304,   3);
    scrBgs[(int)ScrBg::Ceiling1]   = new QGraphicsRectItem(-152, -45, 304,   2);
    scrBgs[(int)ScrBg::Ceiling2]   = new QGraphicsRectItem(-152, -43, 304,   2);
    scrBgs[(int)ScrBg::Ceiling3]   = new QGraphicsRectItem(-152, -41, 304,   2);
    scrBgs[(int)ScrBg::Ceiling4]   = new QGraphicsRectItem(-152, -39, 304,   2);
    scrBgs[(int)ScrBg::Ceiling5]   = new QGraphicsRectItem(-152, -37, 304,   2);
    scrBgs[(int)ScrBg::Ceiling6]   = new QGraphicsRectItem(-152, -35, 304,   3);

    scrBgs[(int)ScrBg::Floor6]     = new QGraphicsRectItem(-152,  32, 304,   3);
    scrBgs[(int)ScrBg::Floor5]     = new QGraphicsRectItem(-152,  35, 304,   2);
    scrBgs[(int)ScrBg::Floor4]     = new QGraphicsRectItem(-152,  37, 304,   2);
    scrBgs[(int)ScrBg::Floor3]     = new QGraphicsRectItem(-152,  39, 304,   2);
    scrBgs[(int)ScrBg::Floor2]     = new QGraphicsRectItem(-152,  41, 304,   2);
    scrBgs[(int)ScrBg::Floor1]     = new QGraphicsRectItem(-152,  43, 304,   2);
    scrBgs[(int)ScrBg::Floor0]     = new QGraphicsRectItem(-152,  45, 304,   3);

    scrBgs[(int)ScrBg::BorderL]    = new QGraphicsRectItem(-216, -72,  64, 144);
    scrBgs[(int)ScrBg::BorderR]    = new QGraphicsRectItem( 152, -72,  64, 144);
}

void MainWindow::initScrBgColors()
{
    int t = props.maps.at(map_index)->theme_index; /* current theme of current map */
    scrBgs[(int)ScrBg::CeilingFg]  ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::CeilingFg]));
    scrBgs[(int)ScrBg::Ceiling0]   ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Ceiling0]));
    scrBgs[(int)ScrBg::Ceiling1]   ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Ceiling1]));
    scrBgs[(int)ScrBg::Ceiling2]   ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Ceiling2]));
    scrBgs[(int)ScrBg::Ceiling3]   ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Ceiling3]));
    scrBgs[(int)ScrBg::Ceiling4]   ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Ceiling4]));
    scrBgs[(int)ScrBg::Ceiling5]   ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Ceiling5]));
    scrBgs[(int)ScrBg::Ceiling6]   ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Ceiling6]));
    scrBgs[(int)ScrBg::Background] ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Background]));
    scrBgs[(int)ScrBg::Floor6]     ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Floor6]));
    scrBgs[(int)ScrBg::Floor5]     ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Floor5]));
    scrBgs[(int)ScrBg::Floor4]     ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Floor4]));
    scrBgs[(int)ScrBg::Floor3]     ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Floor3]));
    scrBgs[(int)ScrBg::Floor2]     ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Floor2]));
    scrBgs[(int)ScrBg::Floor1]     ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Floor1]));
    scrBgs[(int)ScrBg::Floor0]     ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::Floor0]));
    scrBgs[(int)ScrBg::FloorFg]    ->setBrush(C64::ins().brush_hires(props.themes.at(t)->colors[Theme::Enum::FloorFg]));
}

/****    G R I D S
 ******************************************************************************/

void MainWindow::wallVisible    (int state) { wallState     = state; vrfyWallVisibility(); }
void MainWindow::wallGridVisible(int state) { wallGridState = state; vrfyWallVisibility(); }
void MainWindow::playerVisible  (int state) { playerState   = state; vrfyPlayerVisibility(); }

void MainWindow::vrfyWallVisibility()
{
    if (wallState)
    {
        wallGrp->show();
        if (wallGridState) wallGridGrp->show();
        else               wallGridGrp->hide();
    }
    else
    {
        wallGrp->hide();
        wallGridGrp->hide();
    }
}

void MainWindow::vrfyPlayerVisibility()
{

}

void MainWindow::createGrids()
{
    grid->createSeries(Grid::Type::Fine, 8, 8, -72, -72, 72, -48);
    grid->createSeries(Grid::Type::Fine, 8, 8, -60, -32, 60,  32);
    grid->createSeries(Grid::Type::Fine, 8, 8, -72,  48, 72,  72);

    grid->createSeries(Grid::Type::Main, 48, 24, -72, -72, 72, -48);
    grid->createSeries(Grid::Type::Main, 40, 16, -60, -32, 60,  32);
    grid->createSeries(Grid::Type::Main, 48, 24, -72,  48, 72,  72);

    grid->createMainLine( -24, -48,  -20, -32);
    grid->createMainLine(  24, -48,   20, -32);
    grid->createMainLine( -24,  48,  -20,  32);
    grid->createMainLine(  24,  48,   20,  32);

    createWallLine(Qt::SolidLine, -24, -53,  24, -53);
    createWallLine(Qt::SolidLine, -24,  52,  24,  52);
    createWallLine(Qt::SolidLine, -24, -53, -24,  52);
    createWallLine(Qt::SolidLine,  24, -53,  24,  52);
    createWallLine(Qt::DotLine, -24, -32,  24, -32);
    createWallLine(Qt::DotLine, -24, -11,  24, -11);
    createWallLine(Qt::DotLine, -24,  10,  24,  10);
    createWallLine(Qt::DotLine, -24,  31,  24,  31);
}

void MainWindow::createWallLine(Qt::PenStyle type, int x0, int y0, int x1, int y1)
{
    QGraphicsLineItem *line;
    QPen pen = QPen(QColor::fromRgba( Cfg::ins().val(Cfg::EditorWallGridColor) ));
    pen.setWidth(0);
    pen.setStyle(type);
    line = new QGraphicsLineItem(x0,y0,x1,y1);
    line->setPen(pen);
    wallItems.push_back(line);
    wallGridGrp->addToGroup(wallItems.back());
}

/****    S P R I T E   C O L O R S
 ******************************************************************************/

void MainWindow::receiveColorCeiling(quint8 i)
{
    props.editor_ceiling_clr = i;
    scrDatas.at((int)ScrPart::CeilingC)->setColor((int)GfxData::ColorIndex::Color, i);
    scrImgs.at((int)ScrPart::CeilingC)->refresh();
    editor_img_t_modified=true;
    ui->colorCeiling->buttonColor(i);
}

void MainWindow::receiveColorWall(quint8 i)
{
    props.editor_wall_clr = i;
    scrDatas.at((int)ScrPart::WallC)->setColor((int)GfxData::ColorIndex::Color, i);
    scrImgs.at((int)ScrPart::WallC)->refresh();
    editor_img_t_modified=true;
    ui->colorWall->buttonColor(i);
}

void MainWindow::receiveColorFloor(quint8 i)
{
    props.editor_floor_clr = i;
    scrDatas.at((int)ScrPart::FloorC)->setColor((int)GfxData::ColorIndex::Color, i);
    scrImgs.at((int)ScrPart::FloorC)->refresh();
    editor_img_t_modified=true;
    ui->colorFloor->buttonColor(i);
}

/****    M A P
 ******************************************************************************/

void MainWindow::selectMap(int index)
{
    /* Clearing combobox also triggers the signal which is connected to this slot,
     * thus we have to be sure that the combobox is not empty!
     */
    if(0 <= index)
    {
        map_index = index;
        props.all_tiles_need_refresh();
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

/****    S E C T O R   M O V E / C O P Y
 ******************************************************************************/

void MainWindow::setSectorSelectionL()
{
    sectorSelectionL = sector;
    calculateSectorSelection();
}

void MainWindow::setSectorSelectionR()
{
    sectorSelectionR = sector;
    calculateSectorSelection();
}

void MainWindow::calculateSectorSelection()
{
    sectorMoveL = sector;
    sectorMoveR = sector;
    --sectorMoveL;

    sectorCopyL = sector;
    sectorCopyR = sector+sectorSelectionR-sectorSelectionL;

    sectorSelectionNum = int(sectorSelectionR-sectorSelectionL)+1;

    ui->lblSelectionL->setText(QString::number(sectorSelectionL));
    ui->lblSelectionR->setText(QString::number(sectorSelectionR));
    ui->lblSelectionNum->setText(QString::number(sectorSelectionNum));
    ui->lblMoveL->setText(QString::number(sectorMoveL));
    ui->lblMoveR->setText(QString::number(sectorMoveR));
    ui->lblCopyL->setText(QString::number(sectorCopyL));
    ui->lblCopyR->setText(QString::number(sectorCopyR));
}

void MainWindow::remapSectorsForMove()
{
    sector_modint src = sectorSelectionL;
    sector_modint dst = sectorMoveR;
    int num = sectorSelectionNum;
    QBitArray alreadyMoved(SCENERY_MAP_SECTORS);

    for (int i=0; i<SCENERY_MAP_SECTORS; ++i)
    {
        while (alreadyMoved.at(src))
        {
            /* do not move what has already been moved! */
            src++;
        }

        remapSectors[dst] = src;
        alreadyMoved.setBit(src);

        ++dst;
        ++src;
        --num;

        if (0 == num)
        {
            /* selection has just been moved,
             * let's move the remaining sectors! */
            src = sectorMoveR;
        }
    }

    sectorMoveOrCopy();
}

void MainWindow::remapSectorsForCopy()
{
    sector_modint src = sectorSelectionL;
    sector_modint dst = sectorCopyL;
    int num = sectorSelectionNum;

    for (int i=0; i<SCENERY_MAP_SECTORS; ++i)
    {
        if (num)
        {
            /* copy selection */
            remapSectors[dst] = src;
            ++src;
            --num;
        }
        else
        {
            /* leave the remaining sectors */
            remapSectors[dst] = dst;
        }
        ++dst;
    }

    sectorMoveOrCopy();
}

void MainWindow::sectorMoveOrCopy()
{
    editorImgSave();
    Map *m = props.maps.at(map_index);
    Scenery *s = props.sceneries.at(m->scenery_index);
    Map *tmpM = m->copy();

    for(int dst=0; dst<SCENERY_MAP_SECTORS; ++dst)
    {
        /* sector indexes */
        int src = remapSectors.at(dst);

        /* calculate index of left block */
        int srcL = props.sector2blockL(src);
        int dstL = props.sector2blockL(dst);

        quint8 *idx;

        idx = &(m->ceiling_idxs[dst]);
        s->freeSprite(*idx);
        *idx = tmpM->ceiling_idxs.at(src);
        s->useSprite(*idx);

        idx = &(m->floor_idxs[dst]);
        s->freeSprite(*idx);
        *idx = tmpM->floor_idxs.at(src);
        s->useSprite(*idx);

        idx = &(m->wall_idxs[dst]);
        s->freeWall(*idx);
        *idx = tmpM->wall_idxs.at(src);
        s->useWall(*idx);

        m->ceiling_clrs[dst] = tmpM->ceiling_clrs.at(src);
        m->floor_clrs[dst]   = tmpM->floor_clrs.at(src);
        m->wall_clrs[dst]    = tmpM->wall_clrs.at(src);

        for (int b=0; b<=2; ++b)
        {
            idx = &(m->block_c_idxs[dstL+b]);
            s->freeCnfTile(*idx);
            *idx = tmpM->block_c_idxs.at(srcL+b);
            s->useCnfTile(*idx);

            idx = &(m->block_f_idxs[dstL+b]);
            s->freeCnfTile(*idx);
            *idx = tmpM->block_f_idxs.at(srcL+b);
            s->useCnfTile(*idx);

            idx = &(m->block_0_idxs[dstL+b]);
            s->freeBgTile(*idx);
            *idx = tmpM->block_0_idxs.at(srcL+b);
            s->useBgTile(*idx);

            idx = &(m->block_1_idxs[dstL+b]);
            s->freeBgTile(*idx);
            *idx = tmpM->block_1_idxs.at(srcL+b);
            s->useBgTile(*idx);

            idx = &(m->block_2_idxs[dstL+b]);
            s->freeBgTile(*idx);
            *idx = tmpM->block_2_idxs.at(srcL+b);
            s->useBgTile(*idx);

            idx = &(m->block_3_idxs[dstL+b]);
            s->freeBgTile(*idx);
            *idx = tmpM->block_3_idxs.at(srcL+b);
            s->useBgTile(*idx);
        }
    }

    delete tmpM;
    refreshEditor();
}
