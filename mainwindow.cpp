
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
    initScrPart(ScrPart::Wall,       GfxData::Type::Sprite,     24, 105, GfxImage::Mode::Expanded);
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

void MainWindow::tileChanged()
{
    editor_img_t_modified = true;
    editorImgSave();
    refreshEditor();
}

void MainWindow::spriteChanged()
{
    /* Scenery Wall Browser must be update all walls as some of them may contain the modified sprite */
    for(int v=0; v<SCENERY_WALL_NUM; v++)
        props.editor_modified_walls.setBit(v, true);
    tileChanged();
}

void MainWindow::wallChanged()
{
    /* Scenery Sprite Browser must be update all sprites as some of them may contained by the modified wall */
    for(int v=0; v<SCENERY_SPRITE_NUM; v++)
        props.editor_modified_sprites.setBit(v, true);
    tileChanged();
}

void MainWindow::incSector()
{
    editorImgSave();
    sector++;
    refreshEditor();
}

void MainWindow::decSector()
{
    editorImgSave();
    sector--;
    refreshEditor();
}

void MainWindow::setSector(int s)
{
    editorImgSave();
    sector = s;
    refreshEditor();
}

void MainWindow::setSector(QString s)
{
    editorImgSave();
    sector = s.toInt();
    refreshEditor();
}

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

void MainWindow::saveModifications()
{
    editorImgSave();
    refreshEditor();
}

void MainWindow::refreshEditor()
{
    initScrBgColors();
    editorImgLoad();
    refreshSceneryBrowsers();
    ui->checkDelete->setChecked(false);
    enableDeletBtn(false);
    ui->sector->setText(QString::number(sector));
    ui->sectorSlider->setSliderPosition(sector);
    calculateSectorSelection();
}

void MainWindow::calculateSectorSelection()
{
    sectorMoveL = sector;
    sectorMoveR = sector;
    sectorMoveR++;

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
    Map *tmpM = m->copy();

    for(int dst=0; dst<SCENERY_MAP_SECTORS; dst++)
    {
        int src = remapSectors.at(dst);
        int srcL = props.sector2blockL(src);
        int srcC = srcL+1;
        int srcR = srcC+1;
        int dstL = props.sector2blockL(dst);
        int dstC = dstL+1;
        int dstR = dstC+1;

        m->ceiling_idxs[dst] = tmpM->ceiling_idxs.at(src);
        m->wall_idxs[dst]    = tmpM->wall_idxs.at(src);
        m->floor_idxs[dst]   = tmpM->floor_idxs.at(src);
        m->ceiling_clrs[dst] = tmpM->ceiling_clrs.at(src);
        m->wall_clrs[dst]    = tmpM->wall_clrs.at(src);
        m->floor_clrs[dst]   = tmpM->floor_clrs.at(src);

        m->block_c_idxs[dstL] = tmpM->block_c_idxs.at(srcL);
        m->block_c_idxs[dstC] = tmpM->block_c_idxs.at(srcC);
        m->block_c_idxs[dstR] = tmpM->block_c_idxs.at(srcR);
        m->block_f_idxs[dstL] = tmpM->block_f_idxs.at(srcL);
        m->block_f_idxs[dstC] = tmpM->block_f_idxs.at(srcC);
        m->block_f_idxs[dstR] = tmpM->block_f_idxs.at(srcR);
        m->block_0_idxs[dstL] = tmpM->block_0_idxs.at(srcL);
        m->block_0_idxs[dstC] = tmpM->block_0_idxs.at(srcC);
        m->block_0_idxs[dstR] = tmpM->block_0_idxs.at(srcR);
        m->block_1_idxs[dstL] = tmpM->block_1_idxs.at(srcL);
        m->block_1_idxs[dstC] = tmpM->block_1_idxs.at(srcC);
        m->block_1_idxs[dstR] = tmpM->block_1_idxs.at(srcR);
        m->block_2_idxs[dstL] = tmpM->block_2_idxs.at(srcL);
        m->block_2_idxs[dstC] = tmpM->block_2_idxs.at(srcC);
        m->block_2_idxs[dstR] = tmpM->block_2_idxs.at(srcR);
        m->block_3_idxs[dstL] = tmpM->block_3_idxs.at(srcL);
        m->block_3_idxs[dstC] = tmpM->block_3_idxs.at(srcC);
        m->block_3_idxs[dstR] = tmpM->block_3_idxs.at(srcR);
    }

    delete tmpM;
    refreshEditor();
}


void MainWindow::openSectorCEditor()
{
    if(nullptr == sectorCEditor)
    {
        sectorCEditor = new CnfTileEditor(scrDatas.at((int)ScrPart::CeilingFgC));
        sectorCEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sectorCEditor->show();
        connect(sectorCEditor, SIGNAL(destroyed()), this, SLOT(closeSectorCEditor()));
    }
    /* ... or activate the already existing one */
    else sectorCEditor->activateWindow();
}

void MainWindow::openSectorBEditor()
{
    if(nullptr == sectorBEditor)
    {
        sectorBEditor = new BgTileEditor(scrDatas.at((int)ScrPart::BackgroundC));
        sectorBEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sectorBEditor->show();
        connect(sectorBEditor, SIGNAL(destroyed()), this, SLOT(closeSectorBEditor()));
    }
    /* ... or activate the already existing one */
    else sectorBEditor->activateWindow();
}

void MainWindow::openSectorFEditor()
{
    if(nullptr == sectorFEditor)
    {
        sectorFEditor = new CnfTileEditor(scrDatas.at((int)ScrPart::FloorFgC));
        sectorFEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sectorFEditor->show();
        connect(sectorFEditor, SIGNAL(destroyed()), this, SLOT(closeSectorFEditor()));
    }
    /* ... or activate the already existing one */
    else sectorFEditor->activateWindow();
}

void MainWindow::closeSectorCEditor() { sectorCEditor = nullptr; }
void MainWindow::closeSectorBEditor() { sectorBEditor = nullptr; }
void MainWindow::closeSectorFEditor() { sectorFEditor = nullptr; }
