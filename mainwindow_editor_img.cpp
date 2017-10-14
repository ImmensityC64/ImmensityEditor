
#include "mainwindow.h"

/****    I M G   L O A D / S A V E
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

    sector_modint sL(sector); sL--;
    sector_modint sR(sector); sR++;

    scrDatas.at((int)ScrPart::CeilingFgL)->load(props.map2imgCeiling(map_index, sL));
    scrDatas.at((int)ScrPart::CeilingFgC)->load(props.map2imgCeiling(map_index, sector));
    scrDatas.at((int)ScrPart::CeilingFgR)->load(props.map2imgCeiling(map_index, sR));

    int sprL    = props.maps.at(map_index)->ceiling_idxs.at(sL);
    int sprC    = props.maps.at(map_index)->ceiling_idxs.at(sector);
    int sprR    = props.maps.at(map_index)->ceiling_idxs.at(sR);
    int sprClrL = props.maps.at(map_index)->ceiling_clrs.at(sL);
    int sprClrC = props.maps.at(map_index)->ceiling_clrs.at(sector);
    int sprClrR = props.maps.at(map_index)->ceiling_clrs.at(sR);
    scrDatas.at((int)ScrPart::CeilingL)->load(props.map2imgSprite(map_index, sprL));
    scrDatas.at((int)ScrPart::CeilingC)->load(props.map2imgSprite(map_index, sprC));
    scrDatas.at((int)ScrPart::CeilingR)->load(props.map2imgSprite(map_index, sprR));
    scrDatas.at((int)ScrPart::CeilingL)->setColor((int)GfxData::ColorIndex::Color, sprClrL);
    scrDatas.at((int)ScrPart::CeilingC)->setColor((int)GfxData::ColorIndex::Color, sprClrC);
    scrDatas.at((int)ScrPart::CeilingR)->setColor((int)GfxData::ColorIndex::Color, sprClrR);
    ui->colorCeiling->buttonColor(sprClrC);

    scrDatas.at((int)ScrPart::BackgroundL)->load(props.map2imgBackground(map_index, sL));
    scrDatas.at((int)ScrPart::BackgroundC)->load(props.map2imgBackground(map_index, sector));
    scrDatas.at((int)ScrPart::BackgroundR)->load(props.map2imgBackground(map_index, sR));

    sprL    = props.maps.at(map_index)->wall_idxs.at(sL);
    sprC    = props.maps.at(map_index)->wall_idxs.at(sector);
    sprR    = props.maps.at(map_index)->wall_idxs.at(sR);
    sprClrL = props.maps.at(map_index)->wall_clrs.at(sL);
    sprClrC = props.maps.at(map_index)->wall_clrs.at(sector);
    sprClrR = props.maps.at(map_index)->wall_clrs.at(sR);
    scrDatas.at((int)ScrPart::WallL)->load(props.map2imgWall(map_index, sprL));
    scrDatas.at((int)ScrPart::WallC)->load(props.map2imgWall(map_index, sprC));
    scrDatas.at((int)ScrPart::WallR)->load(props.map2imgWall(map_index, sprR));
    scrDatas.at((int)ScrPart::WallL)->setColor((int)GfxData::ColorIndex::Color, sprClrL);
    scrDatas.at((int)ScrPart::WallC)->setColor((int)GfxData::ColorIndex::Color, sprClrC);
    scrDatas.at((int)ScrPart::WallR)->setColor((int)GfxData::ColorIndex::Color, sprClrR);
    ui->colorWall->buttonColor(sprClrC);

    sprL    = props.maps.at(map_index)->floor_idxs.at(sL);
    sprC    = props.maps.at(map_index)->floor_idxs.at(sector);
    sprR    = props.maps.at(map_index)->floor_idxs.at(sR);
    sprClrL = props.maps.at(map_index)->floor_clrs.at(sL);
    sprClrC = props.maps.at(map_index)->floor_clrs.at(sector);
    sprClrR = props.maps.at(map_index)->floor_clrs.at(sR);
    scrDatas.at((int)ScrPart::FloorL)->load(props.map2imgSprite(map_index, sprL));
    scrDatas.at((int)ScrPart::FloorC)->load(props.map2imgSprite(map_index, sprC));
    scrDatas.at((int)ScrPart::FloorR)->load(props.map2imgSprite(map_index, sprR));
    scrDatas.at((int)ScrPart::FloorL)->setColor((int)GfxData::ColorIndex::Color, sprClrL);
    scrDatas.at((int)ScrPart::FloorC)->setColor((int)GfxData::ColorIndex::Color, sprClrC);
    scrDatas.at((int)ScrPart::FloorR)->setColor((int)GfxData::ColorIndex::Color, sprClrR);
    ui->colorFloor->buttonColor(sprClrC);

    scrDatas.at((int)ScrPart::FloorFgL)->load(props.map2imgFloor(map_index, sL));
    scrDatas.at((int)ScrPart::FloorFgC)->load(props.map2imgFloor(map_index, sector));
    scrDatas.at((int)ScrPart::FloorFgR)->load(props.map2imgFloor(map_index, sR));

    scrImgs.at((int)ScrPart::CeilingFgL)->refresh();
    scrImgs.at((int)ScrPart::CeilingFgC)->refresh();
    scrImgs.at((int)ScrPart::CeilingFgR)->refresh();

    scrImgs.at((int)ScrPart::CeilingL)->refresh();
    scrImgs.at((int)ScrPart::CeilingC)->refresh();
    scrImgs.at((int)ScrPart::CeilingR)->refresh();

    scrImgs.at((int)ScrPart::BackgroundL)->refresh();
    scrImgs.at((int)ScrPart::BackgroundC)->refresh();
    scrImgs.at((int)ScrPart::BackgroundR)->refresh();

    scrImgs.at((int)ScrPart::WallL)->refresh();
    scrImgs.at((int)ScrPart::WallC)->refresh();
    scrImgs.at((int)ScrPart::WallR)->refresh();

    scrImgs.at((int)ScrPart::FloorL)->refresh();
    scrImgs.at((int)ScrPart::FloorC)->refresh();
    scrImgs.at((int)ScrPart::FloorR)->refresh();

    scrImgs.at((int)ScrPart::FloorFgL)->refresh();
    scrImgs.at((int)ScrPart::FloorFgC)->refresh();
    scrImgs.at((int)ScrPart::FloorFgR)->refresh();

    /* TODO: clear history somehow */
    scrHisC->save(scrDatas.at((int)ScrPart::CeilingFgC));
    scrHisB->save(scrDatas.at((int)ScrPart::BackgroundC));
    scrHisF->save(scrDatas.at((int)ScrPart::FloorFgC));
    scrHisCSpr->save(scrDatas.at((int)ScrPart::CeilingC));
    scrHisWSpr->save(scrDatas.at((int)ScrPart::WallC));
    scrHisFSpr->save(scrDatas.at((int)ScrPart::FloorC));

    /* Temporary scenery will store modifications until they are saved */
    props.editor_scenery = *(props.sceneries.at(props.maps.at(map_index)->scenery_index));
    props.editor_ceiling_idx = props.maps.at(map_index)->ceiling_idxs.at(sector);
    props.editor_wall_idx    = props.maps.at(map_index)->wall_idxs.at(sector);
    props.editor_floor_idx   = props.maps.at(map_index)->floor_idxs.at(sector);
    props.editor_ceiling_clr = props.maps.at(map_index)->ceiling_clrs.at(sector);
    props.editor_wall_clr    = props.maps.at(map_index)->wall_clrs.at(sector);
    props.editor_floor_clr   = props.maps.at(map_index)->floor_clrs.at(sector);
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
    editor_img_t_modified = false;
}

void MainWindow::editorImgSave(void)
{
    bool save_scenery=false;
    int blockL = props.sector2blockL(sector);
    Map *m = props.maps.at(map_index);

    if(editor_img_c_modified)
    {
        /* Mark tiles which need to be refreshed in browsers */
        for(int b=0; b<=2; b++)
        {
            props.editor_modified_cnf_tiles.setBit( props.editor_block_c_idxs.at(b) , true);
            props.editor_modified_cnf_tiles.setBit( m->block_c_idxs.at(blockL+b) , true);
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
            props.editor_modified_cnf_tiles.setBit( m->block_f_idxs.at(blockL+b) , true);
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
            props.editor_modified_bg_tiles.setBit( m->block_0_idxs.at(blockL+b) , true);
            props.editor_modified_bg_tiles.setBit( m->block_1_idxs.at(blockL+b) , true);
            props.editor_modified_bg_tiles.setBit( m->block_2_idxs.at(blockL+b) , true);
            props.editor_modified_bg_tiles.setBit( m->block_3_idxs.at(blockL+b) , true);
        }
        save_scenery=true;
        editor_img_b_modified=false;
    }

    if(editor_img_s_modified)
    {
        props.editor_modified_sprites.setBit( props.editor_ceiling_idx , true);
        props.editor_modified_sprites.setBit( props.editor_floor_idx   , true);
        props.editor_modified_walls.setBit  ( props.editor_wall_idx    , true);
        for(int r=0; r<SCENERY_WALL_ROWS; r++)
        {
            int s = props.editor_scenery.wall_vector.at(props.editor_wall_idx).wall.sprite_idxs.at(r);
            props.editor_modified_sprites.setBit(s, true);
        }

        props.editor_modified_sprites.setBit( m->ceiling_idxs[sector] , true);
        props.editor_modified_sprites.setBit( m->floor_idxs[sector]   , true);
        props.editor_modified_walls.setBit  ( m->wall_idxs[sector]    , true);
        for(int r=0; r<SCENERY_WALL_ROWS; r++)
        {
            int s = props.sceneries.at(m->scenery_index)->wall_vector.at(m->wall_idxs[sector]).wall.sprite_idxs.at(r);
            props.editor_modified_sprites.setBit(s, true);
        }

        save_scenery=true;
        editor_img_s_modified=false;
    }

    if(editor_img_t_modified)
    {
        save_scenery=true;
        editor_img_t_modified=false;
    }

    if(save_scenery)
    {
        /* Save modifications made on temporary scenery */
        *(props.sceneries.at(props.maps.at(map_index)->scenery_index)) = props.editor_scenery;
        props.maps.at(map_index)->ceiling_idxs[sector] = props.editor_ceiling_idx;
        props.maps.at(map_index)->wall_idxs[sector]    = props.editor_wall_idx;
        props.maps.at(map_index)->floor_idxs[sector]   = props.editor_floor_idx;
        props.maps.at(map_index)->ceiling_clrs[sector] = props.editor_ceiling_clr;
        props.maps.at(map_index)->wall_clrs[sector]    = props.editor_wall_clr;
        props.maps.at(map_index)->floor_clrs[sector]   = props.editor_floor_clr;
        int blockL = props.sector2blockL(sector);
        for(int b=0; b<=2; b++)
        {
            props.maps.at(map_index)->block_c_idxs[blockL+b] = props.editor_block_c_idxs.at(b);
            props.maps.at(map_index)->block_f_idxs[blockL+b] = props.editor_block_f_idxs.at(b);
            props.maps.at(map_index)->block_0_idxs[blockL+b] = props.editor_block_0_idxs.at(b);
            props.maps.at(map_index)->block_1_idxs[blockL+b] = props.editor_block_1_idxs.at(b);
            props.maps.at(map_index)->block_2_idxs[blockL+b] = props.editor_block_2_idxs.at(b);
            props.maps.at(map_index)->block_3_idxs[blockL+b] = props.editor_block_3_idxs.at(b);
        }
    }
}

/****    R E F R E S H
 ******************************************************************************/

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
    /* Scenery Wall Browser must update all walls as some of them may contain the modified sprite */
    for(int v=0; v<SCENERY_WALL_NUM; v++)
        props.editor_modified_walls.setBit(v, true);
    tileChanged();
}

void MainWindow::wallChanged()
{
    /* Scenery Sprite Browser must update all sprites as some of them may contained by the modified wall */
    for(int v=0; v<SCENERY_SPRITE_NUM; v++)
        props.editor_modified_sprites.setBit(v, true);
    tileChanged();
}

void MainWindow::sectorCChanged()
{
    editor_img_c_modified = true;
    tileChanged();
}

void MainWindow::sectorBChanged()
{
    editor_img_b_modified = true;
    tileChanged();
}

void MainWindow::sectorFChanged()
{
    editor_img_f_modified = true;
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

void MainWindow::refreshSceneryBrowsers(void)
{
    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        if(props.editor_modified_bg_tiles.at(i))
        {
            gvSceneryBgTiles.setDataAt(i, props.map2imgBgTile(map_index, i));
            props.editor_modified_bg_tiles.clearBit(i);
            if(browseScenery) gvSceneryBgTiles.browser()->refreshTile(i);
        }
    }

    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(props.editor_modified_cnf_tiles.at(i))
        {
            gvSceneryCnfTiles.setDataAt(i, props.map2imgCnfTile(map_index, i));
            props.editor_modified_cnf_tiles.clearBit(i);
            if(browseScenery) gvSceneryCnfTiles.browser()->refreshTile(i);
        }
    }

    for(int i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        if(props.editor_modified_sprites.at(i))
        {
            gvScenerySprites.setDataAt(i, props.map2imgSprite(map_index, i));
            props.editor_modified_sprites.clearBit(i);
            if(browseScenery) gvScenerySprites.browser()->refreshTile(i);
        }
    }

    for(int i=0; i<SCENERY_WALL_NUM; i++)
    {
        if(props.editor_modified_walls.at(i))
        {
            gvSceneryWalls.setDataAt(i, props.map2imgWall(map_index, i));
            props.editor_modified_walls.clearBit(i);
            if(browseScenery) gvSceneryWalls.browser()->refreshTile(i);
        }
    }

    if(browseScenery) charSet->refresh(props.map2imgCharSet(map_index));
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
    if(dndTest(src,p))
        dndSave(src,p);
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
    scrHisCSpr->load(scrDatas.at((int)ScrPart::CeilingC));
    scrHisWSpr->load(scrDatas.at((int)ScrPart::WallC));
    scrHisFSpr->load(scrDatas.at((int)ScrPart::FloorC));
}

void MainWindow::dndRefresh()
{
    scrImgs.at((int)ScrPart::CeilingFgC)->refresh();
    scrImgs.at((int)ScrPart::BackgroundC)->refresh();
    scrImgs.at((int)ScrPart::FloorFgC)->refresh();
    scrImgs.at((int)ScrPart::CeilingC)->refresh();
    scrImgs.at((int)ScrPart::WallC)->refresh();
    scrImgs.at((int)ScrPart::FloorC)->refresh();
}

void MainWindow::dndImport(QByteArray &src, QPoint p)
{
    int x = p.x();
    int y = p.y();
    if ((quint8)GfxData::Type::Sprite == src.at(0))
    {
        if (wallState)
        {
            int wy;
            if      ( y < -32 ) { wy =  0; }
            else if ( y < -11 ) { wy = 21; }
            else if ( y <  10 ) { wy = 42; }
            else if ( y <  31 ) { wy = 63; }
            else                { wy = 84; }

            scrImgs.at((int)ScrPart::WallC)->importSpriteDataToImage(src, QPoint(0, wy));
        }
        else
        {
            if ( y < 0 ) scrImgs.at((int)ScrPart::CeilingC)->importSpriteDataToImage(src, QPoint(0,0));
            /* x is 4 to compensate initial horizontal offset of floor sprite */
            else         scrImgs.at((int)ScrPart::FloorC)->  importSpriteDataToImage(src, QPoint(4,0));
        }
    }
    else if ((quint8)GfxData::Type::Wall == src.at(0))
    {
        if (wallState)
        {
            scrImgs.at((int)ScrPart::WallC)->importSpriteDataToImage(src, QPoint(0,0));
        }
    }
    else
    {
        if      ( y < -40 ) scrImgs.at((int)ScrPart::CeilingFgC)-> importDataToImage(src, QPoint(x+72,y+72));
        else if ( y <  40 ) scrImgs.at((int)ScrPart::BackgroundC)->importDataToImage(src, QPoint(x+60,y+32));
        else                scrImgs.at((int)ScrPart::FloorFgC)->   importDataToImage(src, QPoint(x+72,y-48));
    }
}

bool MainWindow::dndTest(QByteArray &src, QPoint p)
{
    int y = p.y();
    if ((quint8)GfxData::Type::Sprite == src.at(0))
    {
        if (wallState)
        {
            return props.img2mapWall(scrDatas.at((int)ScrPart::WallC));
        }
        else
        {
            if ( y < 0 ) return props.img2mapSpriteCeiling(scrDatas.at((int)ScrPart::CeilingC));
            else         return props.img2mapSpriteFloor  (scrDatas.at((int)ScrPart::FloorC  ));
        }
    }
    else if ((quint8)GfxData::Type::Wall == src.at(0))
    {
        if (wallState)
        {
            return props.img2mapWall(scrDatas.at((int)ScrPart::WallC));
        }
    }
    else
    {
        if      ( y < -40 ) return props.img2mapCeiling(   scrDatas.at((int)ScrPart::CeilingFgC ));
        else if ( y <  40 ) return props.img2mapBackground(scrDatas.at((int)ScrPart::BackgroundC));
        else                return props.img2mapFloor(     scrDatas.at((int)ScrPart::FloorFgC   ));
    }
}

void MainWindow::dndSave(QByteArray &src, QPoint p)
{
    int y = p.y();
    if ((quint8)GfxData::Type::Sprite == src.at(0))
    {
        editor_img_s_modified=true;
        if (wallState)
        {
            scrHisWSpr->save(scrDatas.at((int)ScrPart::WallC));
            ui->colorWall->buttonColor(scrDatas.at((int)ScrPart::WallC)->color((int)GfxData::ColorIndex::Color));
        }
        else
        {
            if ( y < 0 )
            {
                scrHisCSpr->save(scrDatas.at((int)ScrPart::CeilingC));
                ui->colorCeiling->buttonColor(scrDatas.at((int)ScrPart::CeilingC)->color((int)GfxData::ColorIndex::Color));
            }
            else
            {
                scrHisFSpr->save(scrDatas.at((int)ScrPart::FloorC));
                ui->colorFloor->buttonColor(scrDatas.at((int)ScrPart::FloorC)->color((int)GfxData::ColorIndex::Color));
            }
        }
    }
    else if ((quint8)GfxData::Type::Wall == src.at(0))
    {
        if (wallState)
        {
            editor_img_s_modified=true;
            scrHisWSpr->save(scrDatas.at((int)ScrPart::WallC));
            ui->colorWall->buttonColor(scrDatas.at((int)ScrPart::WallC)->color((int)GfxData::ColorIndex::Color));
        }
    }
    else
    {
        if      ( y < -40 ) { editor_img_c_modified=true; scrHisC->save(scrDatas.at((int)ScrPart::CeilingFgC));  }
        else if ( y <  40 ) { editor_img_b_modified=true; scrHisB->save(scrDatas.at((int)ScrPart::BackgroundC)); }
        else                { editor_img_f_modified=true; scrHisF->save(scrDatas.at((int)ScrPart::FloorFgC));    }
    }
}

void MainWindow::editorMousePressEvent(QPoint p, int /* unused */)
{
    int x = p.x();
    int y = p.y();
    if (wallState && -24 < x && x < 24)
    {
        QMessageBox msgBox;
        msgBox.setText("TODO:\nOpen wall editor");
        msgBox.exec();
    }
    else if ( y < -48 ) { openSectorCEditor(); }
    else if ( y < -32 )
    {
        QMessageBox msgBox;
        msgBox.setText("TODO:\nOpen ceiling sprite editor");
        msgBox.exec();
    }
    else if ( y <  32 ) { openSectorBEditor(); }
    else if ( y <  48 )
    {
        QMessageBox msgBox;
        msgBox.setText("TODO:\nOpen floor sprite editor");
        msgBox.exec();
    }
    else                { openSectorFEditor(); }
}
