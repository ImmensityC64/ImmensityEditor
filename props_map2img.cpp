
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
    int s = maps.at(map_index)->scenery_index; /* scenery_index */

    /* total number of characters in the image */
    int chars = 3*SCENERY_CNF_TILE_ROWS*SCENERY_CNF_TILE_COLS;
    int cnt = 0; /* count copied characters */

    /* Creation GfxData of a character has high cost.
     * Supposing that some characters are used more times in a tile,
     * I try to avoid multiple creation of the same character.
     * Thus, I loop through all the caracters and then check
     * whether the character is used in the tile.
     * If it is used, then I create and loop through the rest of the
     * tile to find more instances of the same character.
     */
    for(unsigned int c=0; c<SCENERY_CHR_NUM; c++)
    {
        quint8 cnf_c = (quint8)c & 0x3F;
        chr_container *chr = &(sceneries.at(s)->chr_vector.data()[cnf_c]);
        if(!chr->cnf_usage) { continue; } /* check if the char is used in cnf tiles at all */

        shared_ptr<GfxData> chrImg;
        for(int b=0; b<=2; b++)
        {
            quint8 tile_ptr = maps.at(map_index)->block_c_idxs.at(blockL+b);
            cnf_tile_container *tile = &(sceneries.at(s)->cnf_tile_vector.data()[tile_ptr]);

            for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
            {
                int px = b*8*SCENERY_CNF_TILE_COLS + col*8;
                int py = row*8;

                if(tile->tile.char_idxs.at(row).at(col) == (quint8)c)
                {
                    if(!chrImg)
                    {
                        /* the first occurence of the character
                         * has just been found, let's create GfxData
                         */
                        shared_ptr<GfxData> tChrImg(new GfxData(chr->chr, GfxData::Type::CnfSketch));
                        chrImg=tChrImg;

                        /* set ECM background color */
                        chrImg->setClrVal(0,0,cnf_c & 0xC0);
                    }

                    ret->paste(chrImg, px, py);

                    /* break out from all the loops if all characters have been copied */
                    cnt++;
                    if(chars <= cnt) goto BREAK_genImgCeiling_MAIN_LOOP;
                }
            } /* foreach row & col */
        }
    }
BREAK_genImgCeiling_MAIN_LOOP:

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
    ret->setColor((int)GfxData::ColorIndex::ECM0, themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM0));
    ret->setColor((int)GfxData::ColorIndex::ECM1, themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM1));
    ret->setColor((int)GfxData::ColorIndex::ECM2, themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM2));
    ret->setColor((int)GfxData::ColorIndex::ECM3, themes.at(t)->colors.at((int)Theme::Enum::FloorFgECM3));

    int blockL = sector2blockL(sector); /* left block of sector */
    int s = maps.at(map_index)->scenery_index; /* scenery_index */

    /* total number of characters in the image */
    int chars = 3*SCENERY_CNF_TILE_ROWS*SCENERY_CNF_TILE_COLS;
    int cnt = 0; /* count copied characters */

    /* Creation GfxData of a character has high cost.
     * Supposing that some characters are used more times in a tile,
     * I try to avoid multiple creation of the same character.
     * Thus, I loop through all the caracters and then check
     * whether the character is used in the tile.
     * If it is used, then I create and loop through the rest of the
     * tile to find more instances of the same character.
     */
    for(unsigned int c=0; c<SCENERY_CHR_NUM; c++)
    {
        quint8 cnf_c = (quint8)c & 0x3F;
        chr_container *chr = &(sceneries.at(s)->chr_vector.data()[cnf_c]);
        if(!chr->cnf_usage) { continue; } /* check if the char is used in cnf tiles at all */

        shared_ptr<GfxData> chrImg;
        for(int b=0; b<=2; b++)
        {
            quint8 tile_ptr = maps.at(map_index)->block_f_idxs.at(blockL+b);
            cnf_tile_container *tile = &(sceneries.at(s)->cnf_tile_vector.data()[tile_ptr]);

            for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
            {
                int px = b*8*SCENERY_CNF_TILE_COLS + col*8;
                int py = row*8;

                if(tile->tile.char_idxs.at(row).at(col) == (quint8)c)
                {
                    if(!chrImg)
                    {
                        /* the first occurence of the character
                         * has just been found, let's create GfxData
                         */
                        shared_ptr<GfxData> tChrImg(new GfxData(chr->chr, GfxData::Type::CnfSketch));
                        chrImg=tChrImg;

                        /* set ECM background color */
                        chrImg->setClrVal(0,0,cnf_c & 0xC0);
                    }

                    ret->paste(chrImg, px, py);

                    /* break out from all the loops if all characters have been copied */
                    cnt++;
                    if(chars <= cnt) goto BREAK_genImgFloor_MAIN_LOOP;
                }
            } /* foreach row & col */
        }
    }
BREAK_genImgFloor_MAIN_LOOP:

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
    int s = maps.at(map_index)->scenery_index; /* scenery_index */

    /* total number of characters in the image */
    int chars = 3*4*SCENERY_BG_TILE_ROWS*SCENERY_BG_TILE_COLS;
    int cnt = 0; /* count copied characters */

    /* Creation GfxData of a character has high cost.
     * Supposing that some characters are used more times in a tile,
     * I try to avoid multiple creation of the same character.
     * Thus, I loop through all the caracters and then check
     * whether the character is used in the tile.
     * If it is used, then I create and loop through the rest of the
     * tile to find more instances of the same character.
     */
    for(unsigned int c=0; c<SCENERY_CHR_NUM; c++)
    {
        chr_container *chr = &(sceneries.at(s)->chr_vector.data()[c]);
        if(!chr->usage) { continue; } /* check if the char is used at all */

        shared_ptr<GfxData> chrImg;
        for(int b=0; b<=2; b++)
        {
            quint8 tile0_ind = maps.at(map_index)->block_0_idxs.at(blockL+b);
            quint8 tile1_ind = maps.at(map_index)->block_1_idxs.at(blockL+b);
            quint8 tile2_ind = maps.at(map_index)->block_2_idxs.at(blockL+b);
            quint8 tile3_ind = maps.at(map_index)->block_3_idxs.at(blockL+b);
            bg_tile_container *tile[4];
            tile[0] = &(sceneries.at(s)->bg_tile_vector.data()[tile0_ind]);
            tile[1] = &(sceneries.at(s)->bg_tile_vector.data()[tile1_ind]);
            tile[2] = &(sceneries.at(s)->bg_tile_vector.data()[tile2_ind]);
            tile[3] = &(sceneries.at(s)->bg_tile_vector.data()[tile3_ind]);

            for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
            {
                int px = b*8*SCENERY_BG_TILE_COLS + col*8;
                int py = row*8;

                for(int t=0; t<=3; t++)
                {
                    if(tile[t]->tile.char_idxs.at(row).at(col) == (quint8)c)
                    {
                        if(!chrImg)
                        {
                            /* The first occurence of the character has just been found, let's create GfxData */
                            shared_ptr<GfxData> tChrImg(new GfxData(chr->chr, GfxData::Type::Sketch));
                            chrImg=tChrImg;
                        }

                        /* Set character color */
                        chrImg->setClrVal(0,0, tile[t]->tile.colors.at(row).at(col) );

                        ret->paste(chrImg, px, py);

                        /* break out from all the loops if all characters have been copied */
                        cnt++;
                        if(chars <= cnt) goto BREAK_genImgBackground_MAIN_LOOP;
                    }
                }
            } /* foreach row & col */
        }
    }
BREAK_genImgBackground_MAIN_LOOP:

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
        ret->paste(chrImg, col*8, row*8);
    } /* foreach row & col */

    return ret;
}
