
#include "map_general.h"

bool Props::img2mapCeiling(int map_index, int sector, Scenery *scenery, shared_ptr<GfxData> img)
{
    Map *m = maps.at(map_index);
    return img2mapCnf(sector, scenery, img, &m->block_c_ptrs);
}

bool Props::img2mapFloor(int map_index, int sector, Scenery *scenery, shared_ptr<GfxData> img)
{
    Map *m = maps.at(map_index);
    return img2mapCnf(sector, scenery, img, &m->block_f_ptrs);
}

bool Props::img2mapCnf(int sector, Scenery *scenery, shared_ptr<GfxData> img, QVector<quint8> *block_cnf_ptrs)
{
    Scenery *s = scenery->copy(); /* temporary scenery */
    int blockL = sector2blockL(sector);  /* left block of sector */
    QVector<quint8> tile_indexes(3);

    /* Free tiles in sector */
    /* Note that 'b' cannot overflow as we are in one sector */
    for(int b=blockL; b<=blockL+2; b++)
    {
        /* Note that freeCnfTile() frees up unused characters automatically! */
        s->freeCnfTile( block_cnf_ptrs->at(b) );
    }

    /* Identify existing tiles and create new tiles */
    for(int b=0; b<=2; b++)
    {
        CnfTile tile;
        bool new_chr_found = false;
        bool new_tile = false;
        bool characters_are_used = false;

        /* At first, try to build up the tile from existing characters
         * and do nothing else. We have no information from the tile.
         * We will have different tasks in the following cases:
         * - Existing tile
         * - New tile with existing characters
         * - New tile with at least one new character
         */
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
        {
            quint64 chr = img->chr(col, row); /* get character data */
            quint16 chr_i; /* index of character */

            if(chr_i = s->findChar(chr) < 0) /* search for the character */
            {
                /* If this character is new, then it is in a new tile */
                new_chr_found = true;
                new_tile = true;
                goto IMG2MAP_C_EXIT_CHR_IDENTIFICATION;
            }

            tile.char_ptrs.data()[row][col] = chr_i;
        }
IMG2MAP_C_EXIT_CHR_IDENTIFICATION:

        if(new_chr_found)
        {
            /* A new character was found, so it is in a new tile.
             * We have to increase the usage counter of all characters,
             * because they are in a tile which is being created now.
             * useCnfTile() is not allowed to handle this because it
             * would break the logic which searches for unused characters
             * in that loop below.
             */
            for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
            {
                quint64 chr = img->chr(col, row); /* character data */
                quint16 chr_i; /* index of character */

                if(chr_i = s->findChar(chr) < 0) /* search for the character */
                {
                    /* it is a new character */
                    if(chr_i = s->createChar(chr) < 0) /* create it */
                        goto IMG2MAP_C_EXIT; /* out of resources */
                }

                /* Register new usage of every character */
                if( !s->useCnfChar(chr_i) )
                    goto IMG2MAP_C_EXIT; /* out of resources */

                tile.char_ptrs.data()[row][col] = chr_i;
            }
            characters_are_used = true;
        }

        /* Register new usage of tile */
        /* All characters of the tile are identified at this point */
        quint16 tile_i; /* index of tile */
        if(tile_i = s->findCnfTile(tile) < 0) /* search for the tile */
        {
            /* it is a new tile */
            if(tile_i = s->createCnfTile(tile) < 0)
                goto IMG2MAP_C_EXIT; /* out of resources */
            new_tile = true;

        }
        s->useCnfTile(tile_i);
        tile_indexes.data()[b] = (quint8)tile_i;

        /* Register new usage of every character if still needed */
        if(new_tile && !characters_are_used)
        {
            for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
            {
                /* Note that useCnfChar() cannot fail at this point!
                 * We cannot get here if new ECM characters had been found.
                 */
                s->useCnfChar(tile.char_ptrs.at(row).at(col));
            }
        }

    } /* for(int b=0; b<=2; b++) */

    /* Everything was successful, we can save the changes */
    delete scenery;
    scenery = s;
    for(int b=0; b<=2; b++)
        block_cnf_ptrs->data()[blockL+b] = tile_indexes.at(b);
    return true;

IMG2MAP_C_EXIT:
    delete s;
    return false;
}

bool Props::img2mapBackground(int map_index, int sector, Scenery *scenery, shared_ptr<GfxData> img)
{
    bool ret = true;
    Scenery *tScenery = scenery->copy(); /* temporary scenery */
    int blockL = sector2blockL(sector);  /* left block of sector */
    int s = maps.at(map_index)->scenery_index; /* scenery_index */

    delete tScenery;
    return ret;
}

#if 0
bool Props::img2mapTest(int map_index, int sector,
                        shared_ptr<GfxData> oldC, /* old ceiling    */
                        shared_ptr<GfxData> oldB, /* old background */
                        shared_ptr<GfxData> oldF, /* old floor      */
                        shared_ptr<GfxData> newC, /* new ceiling    */
                        shared_ptr<GfxData> newB, /* new background */
                        shared_ptr<GfxData> newF) /* new floor      */
{
    bool ret = true;

    newChars.clear();
    newBgTiles.clear();
    newCnfTiles.clear();

    unusedChars.clear();
    unusedBgTiles.clear();
    unusedCnfTiles.clear();

    int blockL = sector2blockL(sector); /* left block of sector */
    int s = maps.at(map_index)->scenery_index; /* scenery_index */

    /* Explode GfxData to chars & tiles
     * Find existing chars & tiles
     * Create a list of new chars & tiles
     */
    for(int b=0; b<=2; b++)
    {
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
        {
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
            {

            }
        }
    }

    /* Find unused chars & tiles */
    /* TODO: also check whether an element is protected against overwrite!
     * See TODO above struct chr_container{} in map_general.h for more info!
     */
    for(int i=0; i<SCENERY_CHR_NUM; i++)
    {
        if(sceneries.at(s)->chr_vector.at(i).usage == 0)
        {
            unusedChars.append((qint8)i);
        }
    }
    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        if(sceneries.at(s)->bg_tile_vector.at(i).usage == 0)
        {
            unusedBgTiles.append((qint8)i);
        }
    }
    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(sceneries.at(s)->cnf_tile_vector.at(i).usage == 0)
        {
            unusedCnfTiles.append((qint8)i);
        }
    }

    /* Are there enough unused chars & tiles? */

    return ret;
}
#endif
