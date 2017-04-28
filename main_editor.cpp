
#include "mainwindow.h"

/****    S C R E E N
 ******************************************************************************/

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
    scrRects[(int)ScrPart::CeilingL]   ->setRect(-170,  -51,  64,  21);
    scrRects[(int)ScrPart::CeilingC]   ->setRect( -32,  -51,  64,  21);
    scrRects[(int)ScrPart::CeilingR]   ->setRect( 102,  -51,  64,  21);
    scrRects[(int)ScrPart::BackgroundL]->setRect(-180,  -32, 120,  64);
    scrRects[(int)ScrPart::BackgroundC]->setRect( -60,  -32, 120,  64);
    scrRects[(int)ScrPart::BackgroundR]->setRect(  60,  -32, 120,  64);
    scrRects[(int)ScrPart::WallL]      ->setRect(-170,  -53,  52, 105);
    scrRects[(int)ScrPart::WallC]      ->setRect( -26,  -53,  52, 105);
    scrRects[(int)ScrPart::WallR]      ->setRect( 118,  -53,  52, 105);
    scrRects[(int)ScrPart::FloorL]     ->setRect(-170,   30,  64,  21);
    scrRects[(int)ScrPart::FloorC]     ->setRect( -32,   30,  64,  21);
    scrRects[(int)ScrPart::FloorR]     ->setRect( 102,   30,  64,  21);
    scrRects[(int)ScrPart::FloorFgL]   ->setRect(-216,   48, 144,  24);
    scrRects[(int)ScrPart::FloorFgC]   ->setRect( -72,   48, 144,  24);
    scrRects[(int)ScrPart::FloorFgR]   ->setRect(  72,   48, 144,  24);

    scrRects[(int)ScrPart::CeilingFgL] ->setOffset(QPoint(-216,  -72));
    scrRects[(int)ScrPart::CeilingFgC] ->setOffset(QPoint( -72,  -72));
    scrRects[(int)ScrPart::CeilingFgR] ->setOffset(QPoint(  72,  -72));
    scrRects[(int)ScrPart::CeilingL]   ->setOffset(QPoint(-170,  -51));
    scrRects[(int)ScrPart::CeilingC]   ->setOffset(QPoint( -32,  -51));
    scrRects[(int)ScrPart::CeilingR]   ->setOffset(QPoint( 102,  -51));
    scrRects[(int)ScrPart::BackgroundL]->setOffset(QPoint(-180,  -32));
    scrRects[(int)ScrPart::BackgroundC]->setOffset(QPoint( -60,  -32));
    scrRects[(int)ScrPart::BackgroundR]->setOffset(QPoint(  60,  -32));
    scrRects[(int)ScrPart::WallL]      ->setOffset(QPoint(-170,  -53));
    scrRects[(int)ScrPart::WallC]      ->setOffset(QPoint( -26,  -53));
    scrRects[(int)ScrPart::WallR]      ->setOffset(QPoint( 118,  -53));
    scrRects[(int)ScrPart::FloorL]     ->setOffset(QPoint(-170,   30));
    scrRects[(int)ScrPart::FloorC]     ->setOffset(QPoint( -32,   30));
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

void MainWindow::wallVisible    (int state) { wallState     = state; }
void MainWindow::playerVisible  (int state) { playerState   = state; }

void MainWindow::createGrids()
{
//    grid->createMainRect(-152, -72, 80, 24);
//    grid->createMainRect(  72, -72, 80, 24);
//    grid->createMainRect(-152,  48, 80, 24);
//    grid->createMainRect(  72,  48, 80, 24);

//    grid->createMainRect(-152, -48, 86, 16);
//    grid->createMainRect(  66, -48, 86, 16);
//    grid->createMainRect(-152,  32, 86, 16);
//    grid->createMainRect(  66,  32, 86, 16);

//    grid->createMainRect(-152, -32, 92, 64);
//    grid->createMainRect(  60, -32, 92, 64);

    grid->createSeries(Grid::Type::Fine, 8, 8, -72, -72, 72, -48);
    grid->createSeries(Grid::Type::Fine, 8, 8, -60, -32, 60,  32);
    grid->createSeries(Grid::Type::Fine, 8, 8, -72,  48, 72,  72);

    grid->createSeries(Grid::Type::Main, 48, 24, -72, -72, 72, -48);
    grid->createSeries(Grid::Type::Main, 40, 16, -60, -32, 60,  32);
    grid->createSeries(Grid::Type::Main, 48, 24, -72,  48, 72,  72);

//    grid->createSeries(Grid::Type::Main, 48, 24, -216, -72, 216, -48);
//    grid->createSeries(Grid::Type::Main, 40, 16, -180, -32, 180,  32);
//    grid->createSeries(Grid::Type::Main, 48, 24, -216,  48, 216,  72);

    grid->createMainLine( -24, -48,  -20, -32);
    grid->createMainLine(  24, -48,   20, -32);
    grid->createMainLine( -24,  48,  -20,  32);
    grid->createMainLine(  24,  48,   20,  32);

//    grid->createMainLine(-120, -48, -100, -32);
//    grid->createMainLine( 120, -48,  100, -32);
//    grid->createMainLine(-120,  48, -100,  32);
//    grid->createMainLine( 120,  48,  100,  32);

//    grid->createMainLine(-168, -48, -140, -32);
//    grid->createMainLine( 168, -48,  140, -32);
//    grid->createMainLine(-168,  48, -140,  32);
//    grid->createMainLine( 168,  48,  140,  32);
}

/****    D R A G  &  D R O P
 ******************************************************************************/

void MainWindow::dndEnter(QByteArray &src, QPoint p)
{
// TODO: display selection
//    selTgt->show();
    dndLoad();
    dndImport(src,p);
    dndRefresh();
}

void MainWindow::dndDraw(QByteArray &src, QPoint p)
{
    dndLoad();
    dndImport(src,p);
    dndRefresh();
}

void MainWindow::dndDrop(QByteArray &src, QPoint p)
{
    dndLoad();
    dndImport(src,p);
    if(dndTest(p))
        dndSave(p);
    else
        dndLoad();
    dndRefresh();
//    selTgt->hide();
}

void MainWindow::dndLeave(void)
{
    dndLoad();
    dndRefresh();
//    selTgt->hide();
}

void MainWindow::dndLoad()
{
    scrHisC->load(scrDatas.at((int)ScrPart::CeilingFgC));
    scrHisB->load(scrDatas.at((int)ScrPart::BackgroundC));
    scrHisF->load(scrDatas.at((int)ScrPart::FloorFgC));
}

void MainWindow::dndRefresh()
{
    scrImgs.at((int)ScrPart::CeilingFgC)->refresh();
    scrImgs.at((int)ScrPart::BackgroundC)->refresh();
    scrImgs.at((int)ScrPart::FloorFgC)->refresh();
}

void MainWindow::dndImport(QByteArray &src, QPoint p)
{
    int x = p.x();
    int y = p.y();
    if      ( y < -40 ) scrImgs.at((int)ScrPart::CeilingFgC)-> importDataToImage(src, QPoint(x+72,y+72));
    else if ( y <  40 ) scrImgs.at((int)ScrPart::BackgroundC)->importDataToImage(src, QPoint(x+60,y+32));
    else                scrImgs.at((int)ScrPart::FloorFgC)->   importDataToImage(src, QPoint(x+72,y-48));
}

bool MainWindow::dndTest(QPoint p)
{
    int y = p.y();
    if      ( y < -40 ) return props.img2mapCeiling(   sector, scrDatas.at((int)ScrPart::CeilingFgC));
    else if ( y <  40 ) return props.img2mapBackground(sector, scrDatas.at((int)ScrPart::BackgroundC));
    else                return props.img2mapFloor(     sector, scrDatas.at((int)ScrPart::FloorFgC));
}

void MainWindow::dndSave(QPoint p)
{
    int y = p.y();
    if      ( y < -40 ) { editor_img_c_modified=true; scrHisC->save(scrDatas.at((int)ScrPart::CeilingFgC));  }
    else if ( y <  40 ) { editor_img_b_modified=true; scrHisB->save(scrDatas.at((int)ScrPart::BackgroundC)); }
    else                { editor_img_f_modified=true; scrHisF->save(scrDatas.at((int)ScrPart::FloorFgC));    }
}

void MainWindow::editorMousePressEvent(QPoint p, int /* unused */)
{
    int y = p.y();
    if      ( y < -48 ) { openSectorCEditor(); }
    else if ( y < -32 ) { cout << "TODO open ceiling sprite editor" << endl; /* TODO */ }
    else if ( y <  32 ) { openSectorBEditor(); }
    else if ( y <  48 ) { cout << "TODO open floor sprite editor" << endl; /* TODO */ }
    else                { openSectorFEditor(); }
}

/****    E D I T O R   I M G
 ******************************************************************************/

/* Generate and load background images from map data. */
void MainWindow::editorImgLoad(void)
{
    /* TODO: Check if there is unsaved modification.
     * E.g. if current theme is changed, this function is called and changes in bg image will be lost.
     * So, we have to save changes first and then let the load function continue.
     * If background is reloaded to loose modifications intentionally, then the flag which shows
     * that there were modifications should be cleared by the reload button before this function is called.
     * Ooooh, dear Future David! I hope you will understand this comment later...
     */

    int sL = sector; props.sectorDec(sL);
    int sR = sector; props.sectorInc(sR);

    scrDatas.at((int)ScrPart::CeilingFgL)->load(props.map2imgCeiling(map_index, sL));
    scrDatas.at((int)ScrPart::CeilingFgC)->load(props.map2imgCeiling(map_index, sector));
    scrDatas.at((int)ScrPart::CeilingFgR)->load(props.map2imgCeiling(map_index, sR));

    scrDatas.at((int)ScrPart::BackgroundL)->load(props.map2imgBackground(map_index, sL));
    scrDatas.at((int)ScrPart::BackgroundC)->load(props.map2imgBackground(map_index, sector));
    scrDatas.at((int)ScrPart::BackgroundR)->load(props.map2imgBackground(map_index, sR));

    scrDatas.at((int)ScrPart::FloorFgL)->load(props.map2imgFloor(map_index, sL));
    scrDatas.at((int)ScrPart::FloorFgC)->load(props.map2imgFloor(map_index, sector));
    scrDatas.at((int)ScrPart::FloorFgR)->load(props.map2imgFloor(map_index, sR));

    scrImgs.at((int)ScrPart::CeilingFgL)->refresh();
    scrImgs.at((int)ScrPart::CeilingFgC)->refresh();
    scrImgs.at((int)ScrPart::CeilingFgR)->refresh();

    scrImgs.at((int)ScrPart::BackgroundL)->refresh();
    scrImgs.at((int)ScrPart::BackgroundC)->refresh();
    scrImgs.at((int)ScrPart::BackgroundR)->refresh();

    scrImgs.at((int)ScrPart::FloorFgL)->refresh();
    scrImgs.at((int)ScrPart::FloorFgC)->refresh();
    scrImgs.at((int)ScrPart::FloorFgR)->refresh();

    /* TODO: clear history somehow */
    scrHisC->save(scrDatas.at((int)ScrPart::CeilingFgC));
    scrHisB->save(scrDatas.at((int)ScrPart::BackgroundC));
    scrHisF->save(scrDatas.at((int)ScrPart::FloorFgC));

    /* Temporary scenery will store modifications until they are saved */
    props.editor_scenery = *(props.sceneries.at(props.maps.at(map_index)->scenery_index));
    props.editor_ceiling_idx = props.maps.at(map_index)->ceiling_idxs.at(sector);
    props.editor_wall_idx    = props.maps.at(map_index)->wall_idxs.at(sector);
    props.editor_floor_idx   = props.maps.at(map_index)->floor_idxs.at(sector);
    int blockL = props.sector2blockL(sector);
    for(int b=0; b<=2; b++)
    {
        props.editor_block_c_idxs[b]=props.maps.at(map_index)->block_c_idxs.at(blockL+b);
        props.editor_block_f_idxs[b]=props.maps.at(map_index)->block_f_idxs.at(blockL+b);
        props.editor_block_0_idxs[b]=props.maps.at(map_index)->block_0_idxs.at(blockL+b);
        props.editor_block_1_idxs[b]=props.maps.at(map_index)->block_1_idxs.at(blockL+b);
        props.editor_block_2_idxs[b]=props.maps.at(map_index)->block_2_idxs.at(blockL+b);
        props.editor_block_3_idxs[b]=props.maps.at(map_index)->block_3_idxs.at(blockL+b);
    }
    editor_img_c_modified = false;
    editor_img_b_modified = false;
    editor_img_f_modified = false;
    editor_img_s_modified = false;
}

void MainWindow::editorImgSave(void)
{
    bool save_scenery=false;
    int blockL = props.sector2blockL(sector);

    if(editor_img_c_modified)
    {
        /* Mark tiles which need to be refreshed in browsers */
        for(int b=0; b<=2; b++)
        {
            props.editor_modified_cnf_tiles.setBit( props.editor_block_c_idxs.at(b) , true);
            props.editor_modified_cnf_tiles.setBit( props.maps.at(map_index)->block_c_idxs.at(blockL+b) , true);
        }
        save_scenery=true;
        editor_img_c_modified=false;
    }

    if(editor_img_f_modified)
    {
        /* Mark tiles which need to be refreshed in browsers */
        for(int b=0; b<=2; b++)
        {
            props.editor_modified_cnf_tiles.setBit( props.editor_block_f_idxs.at(b) , true);
            props.editor_modified_cnf_tiles.setBit( props.maps.at(map_index)->block_f_idxs.at(blockL+b) , true);
        }
        save_scenery=true;
        editor_img_f_modified=false;
    }

    if(editor_img_b_modified)
    {
        /* Mark tiles which need to be refreshed in browsers */
        for(int b=0; b<=2; b++)
        {
            props.editor_modified_bg_tiles.setBit( props.editor_block_0_idxs.at(b) , true);
            props.editor_modified_bg_tiles.setBit( props.editor_block_1_idxs.at(b) , true);
            props.editor_modified_bg_tiles.setBit( props.editor_block_2_idxs.at(b) , true);
            props.editor_modified_bg_tiles.setBit( props.editor_block_3_idxs.at(b) , true);
            props.editor_modified_bg_tiles.setBit( props.maps.at(map_index)->block_0_idxs.at(blockL+b) , true);
            props.editor_modified_bg_tiles.setBit( props.maps.at(map_index)->block_1_idxs.at(blockL+b) , true);
            props.editor_modified_bg_tiles.setBit( props.maps.at(map_index)->block_2_idxs.at(blockL+b) , true);
            props.editor_modified_bg_tiles.setBit( props.maps.at(map_index)->block_3_idxs.at(blockL+b) , true);
        }
        save_scenery=true;
        editor_img_b_modified=false;
    }

    if(editor_img_s_modified)
    {
        /* Save sprites */
        props.maps.at(map_index)->ceiling_idxs[sector] = props.editor_ceiling_idx;
        props.maps.at(map_index)->wall_idxs[sector]    = props.editor_wall_idx;
        props.maps.at(map_index)->floor_idxs[sector]   = props.editor_floor_idx;
        save_scenery=true;
        editor_img_s_modified=false;
    }

    if(save_scenery)
    {
        /* Save modifications made on temporary scenery */
        *(props.sceneries.at(props.maps.at(map_index)->scenery_index)) = props.editor_scenery;
    }
}

/****    S P R I T E S
 ******************************************************************************/

/****    W A L L S
 ******************************************************************************/

/****    S C E N E R Y
 ******************************************************************************/

void MainWindow::refreshSceneryBrowsers(void)
{
    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        if(props.editor_modified_bg_tiles.at(i))
        {
            gvSceneryBgTiles.setDataAt(i, props.map2imgBgTile(map_index, i));
            props.editor_modified_bg_tiles.clearBit(i);
        }
    }

    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(props.editor_modified_cnf_tiles.at(i))
        {
            gvSceneryCnfTiles.setDataAt(i, props.map2imgCnfTile(map_index, i));
            props.editor_modified_cnf_tiles.clearBit(i);
        }
    }

    if(charSetWindow) charSetWindow->refresh(props.map2imgCharSet(map_index));
}
