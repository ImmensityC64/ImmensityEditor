
#include "map_general.h"

/* generate 3 ceiling tiles of a sector from map data */
shared_ptr<GfxData> Props::map2imgCeiling(int map_index, int sector)
{
    shared_ptr<GfxData> ret(new GfxData(GfxData::Type::CnfSketch,
                                        3*8*SCENERY_CNF_TILE_COLS,
                                        1*8*SCENERY_CNF_TILE_ROWS));

    int t = maps.at(map_index)->theme_index;

    ret->setColor((int)GfxData::ColorIndex::Color, themes.at(t)->colors.at((int)Theme::Enum::CeilingFg));
    ret->setColor((int)GfxData::ColorIndex::ECM0,  themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM0));
    ret->setColor((int)GfxData::ColorIndex::ECM1,  themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM1));
    ret->setColor((int)GfxData::ColorIndex::ECM2,  themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM2));
    ret->setColor((int)GfxData::ColorIndex::ECM3,  themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM3));

    int blockL = sector2blockL(sector); /* left block of sector */

    ret->paste(map2imgCnfTile(map_index, maps.at(map_index)->block_c_idxs.at(blockL)),                           0, 0);
    ret->paste(map2imgCnfTile(map_index, maps.at(map_index)->block_c_idxs.at(blockL+1)),   8*SCENERY_CNF_TILE_COLS, 0);
    ret->paste(map2imgCnfTile(map_index, maps.at(map_index)->block_c_idxs.at(blockL+2)), 2*8*SCENERY_CNF_TILE_COLS, 0);

    return ret;
}

/* generate 3 floor tiles of a sector from map data */
shared_ptr<GfxData> Props::map2imgFloor(int map_index, int sector)
{
    shared_ptr<GfxData> ret(new GfxData(GfxData::Type::CnfSketch,
                                        3*8*SCENERY_CNF_TILE_COLS,
                                        1*8*SCENERY_CNF_TILE_ROWS));

    int t = maps.at(map_index)->theme_index;

    ret->setColor((int)GfxData::ColorIndex::Color, themes.at(t)->colors.at((int)Theme::Enum::FloorFg));
    ret->setColor((int)GfxData::ColorIndex::ECM0,  themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM0));
    ret->setColor((int)GfxData::ColorIndex::ECM1,  themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM1));
    ret->setColor((int)GfxData::ColorIndex::ECM2,  themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM2));
    ret->setColor((int)GfxData::ColorIndex::ECM3,  themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM3));

    int blockL = sector2blockL(sector); /* left block of sector */

    ret->paste(map2imgCnfTile(map_index, maps.at(map_index)->block_f_idxs.at(blockL)),                           0, 0);
    ret->paste(map2imgCnfTile(map_index, maps.at(map_index)->block_f_idxs.at(blockL+1)),   8*SCENERY_CNF_TILE_COLS, 0);
    ret->paste(map2imgCnfTile(map_index, maps.at(map_index)->block_f_idxs.at(blockL+2)), 2*8*SCENERY_CNF_TILE_COLS, 0);

    return ret;
}

/* generate 3*4 background tiles of a sector from map data */
shared_ptr<GfxData> Props::map2imgBackground(int map_index, int sector)
{
    shared_ptr<GfxData> ret(new GfxData(GfxData::Type::Sketch,
                                        3*8*SCENERY_BG_TILE_COLS,
                                        4*8*SCENERY_BG_TILE_ROWS));
    int t = maps.at(map_index)->theme_index;
    ret->setColor((int)GfxData::ColorIndex::Backg, themes.at(t)->colors.at((int)Theme::Enum::Background));
    ret->setColor((int)GfxData::ColorIndex::Comm0, themes.at(t)->colors.at((int)Theme::Enum::BackgroundComm0));
    ret->setColor((int)GfxData::ColorIndex::Comm1, themes.at(t)->colors.at((int)Theme::Enum::BackgroundComm1));

    int blockL = sector2blockL(sector); /* left block of sector */

    for(int b=0; b<=2; b++)
    {
        ret->paste(map2imgBgTile(map_index, maps.at(map_index)->block_0_idxs.at(blockL+b)),
                   b*8*SCENERY_BG_TILE_COLS,                        0);
        ret->paste(map2imgBgTile(map_index, maps.at(map_index)->block_1_idxs.at(blockL+b)),
                   b*8*SCENERY_BG_TILE_COLS,   8*SCENERY_BG_TILE_ROWS);
        ret->paste(map2imgBgTile(map_index, maps.at(map_index)->block_2_idxs.at(blockL+b)),
                   b*8*SCENERY_BG_TILE_COLS, 2*8*SCENERY_BG_TILE_ROWS);
        ret->paste(map2imgBgTile(map_index, maps.at(map_index)->block_3_idxs.at(blockL+b)),
                   b*8*SCENERY_BG_TILE_COLS, 3*8*SCENERY_BG_TILE_ROWS);
    }

    return ret;
}

shared_ptr<GfxData> Props::map2imgBgTile(int map_index, int tile_index)
{
    shared_ptr<GfxData> ret(new GfxData(GfxData::Type::Sketch,
                                        8*SCENERY_BG_TILE_COLS,
                                        8*SCENERY_BG_TILE_ROWS));
    int t = maps.at(map_index)->theme_index;
    ret->setColor((int)GfxData::ColorIndex::Backg, themes.at(t)->colors.at((int)Theme::Enum::Background));
    ret->setColor((int)GfxData::ColorIndex::Comm0, themes.at(t)->colors.at((int)Theme::Enum::BackgroundComm0));
    ret->setColor((int)GfxData::ColorIndex::Comm1, themes.at(t)->colors.at((int)Theme::Enum::BackgroundComm1));

    int s = maps.at(map_index)->scenery_index;
    bg_tile_container *tile = &(sceneries.at(s)->bg_tile_vector.data()[tile_index]);

    for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
    {
        int c = tile->tile.char_idxs.at(row).at(col);
        chr_container *chr = &(sceneries.at(s)->chr_vector.data()[c]);
        shared_ptr<GfxData> chrImg(new GfxData(chr->chr, GfxData::Type::Sketch));
        chrImg->setClrVal(0,0,tile->tile.colors.at(row).at(col));
        ret->paste(chrImg, col*8, row*8);
    } /* foreach row & col */

    return ret;
}

shared_ptr<GfxData> Props::map2imgCnfTile(int map_index, int tile_index)
{
    shared_ptr<GfxData> ret(new GfxData(GfxData::Type::CnfSketch,
                                        8*SCENERY_CNF_TILE_COLS,
                                        8*SCENERY_CNF_TILE_ROWS));
    /* TODO: let it be possible to choose between ceiling and floor colors */
    int t = maps.at(map_index)->theme_index;
    ret->setColor((int)GfxData::ColorIndex::Color, themes.at(t)->colors.at((int)Theme::Enum::CeilingFg));
    ret->setColor((int)GfxData::ColorIndex::ECM0, themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM0));
    ret->setColor((int)GfxData::ColorIndex::ECM1, themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM1));
    ret->setColor((int)GfxData::ColorIndex::ECM2, themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM2));
    ret->setColor((int)GfxData::ColorIndex::ECM3, themes.at(t)->colors.at((int)Theme::Enum::CeilingFgECM3));

    int s = maps.at(map_index)->scenery_index;
    cnf_tile_container *tile = &(sceneries.at(s)->cnf_tile_vector.data()[tile_index]);

    for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
    {
        int c = tile->tile.char_idxs.at(row).at(col);
        chr_container *chr = &(sceneries.at(s)->chr_vector.data()[c]);
        shared_ptr<GfxData> chrImg(new GfxData(chr->chr, GfxData::Type::CnfSketch));
        chrImg->setClrVal(0,0,tile->tile.colors.at(row).at(col));
        ret->paste(chrImg, col*8, row*8);
    } /* foreach row & col */

    return ret;
}

shared_ptr<GfxData> Props::map2imgSprite(int map_index, int sprite_index)
{
    int s = maps.at(map_index)->scenery_index;
    return sceneries.at(s)->sprite_vector[sprite_index].sprite.gfx_data();
}

shared_ptr<GfxData> Props::map2imgWall(int map_index, int wall_index)
{
    /* This function does not set sprite color! */
    shared_ptr<GfxData> ret(new GfxData(GfxData::Type::Wall));
    int s = maps.at(map_index)->scenery_index;
    const Wall &wall = sceneries.at(s)->wall_vector.at(wall_index).wall;
    for(int wr=0; wr<SCENERY_WALL_ROWS; wr++)
    {
        int sprite_index = wall.sprite_idxs.at(wr);
        shared_ptr<GfxData> sprImg = map2imgSprite(map_index, sprite_index);
        ret->paste(sprImg, 0, wr*(int)C64::SpriteHeight);
    }
    return ret;
}

shared_ptr<GfxData> Props::map2imgCharSet(int map_index)
{
    shared_ptr<GfxData> ret(new GfxData(GfxData::Type::CharSet));
    ret->setColor((int)GfxData::ColorIndex::Backg, (quint8)C64::IndexWhite);
    int s = maps.at(map_index)->scenery_index;
    sceneries.at(s)->calculateRealCharIndexes();

    for(int row=0; row< 8; row++)
    for(int col=0; col<32; col++)
    {
        int set = row*32+col;
        quint64 chr_data = 0;
        /* loop through char MAPping vector */
        for(int map=0; map<SCENERY_CHR_NUM; map++)
        {
            if(sceneries.at(s)->realCharIndex(map) == set)
            {
                chr_data = sceneries.at(s)->chr_vector.at(map).chr;
                break;
            }
        }
        shared_ptr<GfxData> chrImg(new GfxData(chr_data));
        chrImg->setClrVal(0,0,C64::IndexBlack);
        ret->paste(chrImg, col*8, row*8);
    } /* foreach row & col */

    return ret;
}
