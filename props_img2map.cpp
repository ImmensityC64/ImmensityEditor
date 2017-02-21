
#include "map_general.h"

bool Props::img2mapCeiling(int map_index, int sector, Scenery **scenery, shared_ptr<GfxData> img)
{
    Map *m = maps.at(map_index);
    return img2mapCnf(sector, scenery, img, &m->block_c_ptrs);
}

bool Props::img2mapFloor(int map_index, int sector, Scenery **scenery, shared_ptr<GfxData> img)
{
    Map *m = maps.at(map_index);
    return img2mapCnf(sector, scenery, img, &m->block_f_ptrs);
}

bool Props::img2mapCnf(int sector, Scenery **scenery, shared_ptr<GfxData> img, QVector<quint8> *block_ptrs)
{
    /* Temporary scenery
     * All modificiations will be done in the temporary scenery below.
     * It will be dropped in case of failure (lack of resources to store new data).
     * It will be replaced with scenery got in paramaterers in case of success.
     */
    Scenery *s = (*scenery)->copy();

    int blockL = sector2blockL(sector);  /* index of left block of sector */
    QVector<CnfTile> tile_v(3);/* tiles being identified */
    QVector<quint64> new_chr_v;

    /* Temporary storage for tile indexes
     * -1 = not identified
     */
    QVector<qint16> identified_tiles_v(3,-1);

    /* Use another tile to track which characters have been identified.
     * 0 = not identified (default value in constructor)
     * 1 = identified
     */
    QVector<CnfTile> identified_chars_v(3);

    /* Free up tiles of sector */
    /* Note that 'b' cannot overflow as we are in one sector */
    for(int b=blockL; b<=blockL+2; b++)
    {
        /* Note that freeCnfTile() frees up unused characters automatically! */
        s->freeCnfTile( block_ptrs->at(b) );
    }

    /* Identify existing characters */
    for(int b=0; b<=2; b++)
    for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
    {
         /* Get character data from 'img' and find matching character.
          * Note that functions of 'img' use different parameter order!
          * 'x' coord is 'col' and 'y' coord is 'row'!
          */
        quint64 chr = img->chr(b*SCENERY_CNF_TILE_COLS + col, row);
        qint16 chr_ind = s->findChar(chr); /* index of character */

        if(0 <= chr_ind)
        {
            /* Matching character found
             * Store its index to the tile.
             * Reserve the character, to make sure that creation of
             * a new character will not override it until it will be
             * registered as used! Note that it may not be used at this moment!
             */
            tile_v[b].char_ptrs[row][col] = chr_ind;
            identified_chars_v[b].char_ptrs[row][col] = 1;
            s->reserveChar(chr_ind);
        }
        else
        {
            /* Found a new character, we will create it later */
            new_chr_v.append(chr);
        }
    } /* foreach b & row & col */

    /* Create new characters */
    for(int b=0; b<=2; b++)
    for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
    {
        /* Skip if it has already been identified */
        if(identified_chars_v.at(b).char_ptrs.at(row).at(col))
            continue;

        /* It is a new character, let's create it! */
        quint64 chr = new_chr_v.takeFirst();
        qint16 chr_ind = s->createChar(chr);

        if(0 <= chr_ind)
        {
            tile_v[b].char_ptrs[row][col] = chr_ind;
            s->reserveChar(chr_ind);
        }
        else
        {
            goto IMG2MAP_C_EXIT; /* out of resources */
        }
    } /* foreach b & row & col */

    /* Identify existing tiles */
    for(int b=0; b<=2; b++)
    {
        qint16 tile_ind = s->findCnfTile(tile_v.at(b));
        if(0 <= tile_ind)
        {
            identified_tiles_v[b] = tile_ind;
            s->reserveCnfTile(tile_ind);
        }
    }

    /* Create new tiles */
    for(int b=0; b<=2; b++)
    {
        /* Skip if it has already been identified */
        if(0 <= identified_tiles_v.at(b))
            continue;

        qint16 tile_ind = s->createCnfTile(tile_v.at(b));

        if(0 <= tile_ind)
        {
            identified_tiles_v[b] = tile_ind;
            s->reserveCnfTile(tile_ind);
        }
        else
        {
            goto IMG2MAP_C_EXIT; /* out of resources */
        }
    }

    /* Use new tiles */
    for(int b=0; b<=2; b++)
    {
        if( ! s->useCnfTile(identified_tiles_v.at(b)) )
        {
            goto IMG2MAP_C_EXIT; /* out of resources */
        }
    }

    /* Everything was successful, we can save the changes */
    s->clearReservations();
    delete *scenery;
    *scenery = s;
    for(int b=0; b<=2; b++)
        block_ptrs->data()[blockL+b] = (quint8)identified_tiles_v.at(b);
    return true;

IMG2MAP_C_EXIT:
    delete s;
    return false;
}

bool Props::img2mapBackground(int map_index, int sector, Scenery **scenery, shared_ptr<GfxData> img)
{
    bool ret = true;
    Scenery *tScenery = (*scenery)->copy(); /* temporary scenery */
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
