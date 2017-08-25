
#include "map_general.h"

bool Props::img2mapCeiling(int sector, shared_ptr<GfxData> img)
{
    return img2mapCnf(sector, img, &editor_block_c_idxs);
}

bool Props::img2mapFloor(int sector, shared_ptr<GfxData> img)
{
    return img2mapCnf(sector, img, &editor_block_f_idxs);
}

bool Props::img2mapCnf(int sector, shared_ptr<GfxData> img, QVector<quint8> *block_idxs)
{
    bool ret = true;

    /* Temporary scenery
     * All modificiations will be done in the temporary scenery below.
     * It will be dropped in case of failure (lack of resources to store new data).
     * It will be replaced with scenery got in paramaterers in case of success.
     */
    Scenery *s = editor_scenery.copy();

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
    for(int b=0; b<=2; b++)
    {
        /* Note that freeCnfTile() frees up unused characters automatically! */
        s->freeCnfTile( block_idxs->at(b) );
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
            tile_v[b].char_idxs[row][col] = chr_ind;
            identified_chars_v[b].char_idxs[row][col] = 1;
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
        if(!identified_chars_v[b].char_idxs[row][col])
        {
            /* It is a new character, let's create it! */
            quint64 chr = new_chr_v.takeFirst();

            /* new_chr_v may contain the same character several times.
             * Verify wether the current character has already been created
             * in this very loop.
             */
            qint16 chr_ind = s->findChar(chr); /* index of character */

            if(chr_ind < 0)
            {
                /* Character must be created */
                chr_ind = s->createChar(chr);
                s->reserveChar(chr_ind);
            }

            if(0 <= chr_ind)
            {
                tile_v[b].char_idxs[row][col] = chr_ind;
            }
            else
            {
                /* out of resources */
                ret = false;
                goto IMG2MAP_CNF_EXIT;
            }
        }

        /* Store color value */
        tile_v[b].colors[row][col] = img->clrVal(b*SCENERY_CNF_TILE_COLS+col,row);
    } /* foreach b & row & col */

    /* Identify existing tiles */
    /* Some tiles may had been used only in this very sector, and now they
     * are unused because they have just been freed up at the beginning of
     * this function. However, they may going to be used again here. Let's
     * reserve them to prevent the next loop to override them during the
     * creation of new tiles. Note, that the database will be flawless
     * without this reservation, as the next loop would recreate these
     * overridden tiles anyway. Therefore this reservation may seem
     * superfluous. Note, that tiles, whose usage is equal to one, would
     * jump up and down in the browser window without this reservation!
     */
    for(int b=0; b<=2; b++)
    {
        qint16 tile_ind = s->findCnfTile(tile_v[b]);
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
        if(0 <= identified_tiles_v[b])
            continue;

        /* Verify wether the current tile has already been created
         * in this very loop.
         */
        qint16 tile_ind = s->findCnfTile(tile_v[b]);

        if(tile_ind < 0)
        {
            /* Tile must be created */
            tile_ind = s->createCnfTile(tile_v[b]);
            s->reserveCnfTile(tile_ind);
        }

        if(0 <= tile_ind)
        {
            identified_tiles_v[b] = tile_ind;
        }
        else
        {
            /* out of resources */
            ret = false;
            goto IMG2MAP_CNF_EXIT;
        }
    }

    /* Use new tiles */
    for(int b=0; b<=2; b++)
    {
        if( ! s->useCnfTile(identified_tiles_v[b]) )
        {
            /* out of resources */
            ret = false;
            goto IMG2MAP_CNF_EXIT;
        }
    }

    /* Everything was successful, we can save the changes */
    s->clearReservations();
    editor_scenery = *s;
    for(int b=0; b<=2; b++)
        block_idxs->data()[b] = (quint8)identified_tiles_v[b];

IMG2MAP_CNF_EXIT:
    delete s;
    return ret;
}

bool Props::img2mapBackground(int sector, shared_ptr<GfxData> img)
{
    bool ret = true;

    /* Temporary scenery
     * All modificiations will be done in the temporary scenery below.
     * It will be dropped in case of failure (lack of resources to store new data).
     * It will be replaced with scenery got in paramaterers in case of success.
     */
    Scenery *s = editor_scenery.copy();

    QVector<quint64> new_chr_v;

    /* tiles being identified */
    QVector<QVector<BgTile>> tile_v(3, QVector<BgTile>(4));

    /* Temporary storage for tile indexes
     * -1 = not identified
     */
    QVector<QVector<qint16>> identified_tiles_v(3, QVector<qint16>(4, -1));

    /* Use another tile to track which characters have been identified.
     * 0 = not identified (default value in constructor)
     * 1 = identified
     */
    QVector<QVector<BgTile>> identified_chars_v(3, QVector<BgTile>(4));

    /* Free up tiles of sector */
    /* Note that 'b' cannot overflow as we are in one sector */
    for(int b=0; b<=2; b++)
    {
        /* Note that freeBgTile() frees up unused characters automatically! */
        s->freeBgTile( editor_block_0_idxs[b] );
        s->freeBgTile( editor_block_1_idxs[b] );
        s->freeBgTile( editor_block_2_idxs[b] );
        s->freeBgTile( editor_block_3_idxs[b] );
    }

    /* Identify existing characters */
    for(int b=0; b<=2; b++)
    for(int t=0; t<=3; t++)
    for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
    {
         /* Get character data from 'img' and find matching character.
          * Note that functions of 'img' use different parameter order!
          * 'x' coord is 'col' and 'y' coord is 'row'!
          */
        quint64 chr = img->chr(b*SCENERY_BG_TILE_COLS + col, t*SCENERY_BG_TILE_ROWS + row);
        qint16 chr_ind = s->findChar(chr); /* index of character */

        if(0 <= chr_ind)
        {
            /* Matching character found
             * Store its index to the tile.
             * Reserve the character, to make sure that creation of
             * a new character will not override it until it will be
             * registered as used! Note that it may not be used at this moment!
             */
            tile_v[b][t].char_idxs[row][col] = chr_ind;
            identified_chars_v[b][t].char_idxs[row][col] = 1;
            s->reserveChar(chr_ind);
        }
        else
        {
            /* Found a new character, we will create it later */
            new_chr_v.append(chr);
        }
    } /* foreach b & t & row & col */

    /* Create new characters */
    for(int b=0; b<=2; b++)
    for(int t=0; t<=3; t++)
    for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
    {
        /* Skip if it has already been identified */
        if(!identified_chars_v[b][t].char_idxs[row][col])
        {
            /* It is a new character, let's create it! */
            quint64 chr = new_chr_v.takeFirst();

            /* new_chr_v may contain the same character several times.
             * Verify wether the current character has already been created
             * in this very loop.
             */
            qint16 chr_ind = s->findChar(chr); /* index of character */

            if(chr_ind < 0)
            {
                /* Character must be created */
                chr_ind = s->createChar(chr);
                s->reserveChar(chr_ind);
            }

            if(0 <= chr_ind)
            {
                tile_v[b][t].char_idxs[row][col] = chr_ind;
            }
            else
            {
                /* out of resources */
                ret = false;
                goto IMG2MAP_BG_EXIT;
            }
        }

        /* Store color value */
        tile_v[b][t].colors[row][col] = img->clrVal(b*SCENERY_BG_TILE_COLS+col,t*SCENERY_BG_TILE_ROWS+row);
    } /* foreach b & t & row & col */

    /* Identify existing tiles */
    /* Some tiles may had been used only in this very sector, and now they
     * are unused because they have just been freed up at the beginning of
     * this function. However, they may going to be used again here. Let's
     * reserve them to prevent the next loop to override them during the
     * creation of new tiles. Note, that the database will be flawless
     * without this reservation, as the next loop would recreate these
     * overridden tiles anyway. Therefore this reservation may seem
     * superfluous. Note, that tiles, whose usage is equal to one, would
     * jump up and down in the browser window without this reservation!
     */
    for(int b=0; b<=2; b++)
    for(int t=0; t<=3; t++)
    {
        qint16 tile_ind = s->findBgTile(tile_v[b][t]);
        if(0 <= tile_ind)
        {
            identified_tiles_v[b][t] = tile_ind;
            s->reserveBgTile(tile_ind);
        }
    }

    /* Create new tiles */
    for(int b=0; b<=2; b++)
    for(int t=0; t<=3; t++)
    {
        /* Skip if it has already been identified */
        if(0 <= identified_tiles_v[b][t])
            continue;

        /* Verify wether the current tile has already been created
         * in this very loop.
         */
        qint16 tile_ind = s->findBgTile(tile_v[b][t]);

        if(tile_ind < 0)
        {
            /* Tile must be created */
            tile_ind = s->createBgTile(tile_v[b][t]);
            s->reserveBgTile(tile_ind);
        }

        if(0 <= tile_ind)
        {
            identified_tiles_v[b][t] = tile_ind;
        }
        else
        {
            /* out of resources */
            ret = false;
            goto IMG2MAP_BG_EXIT;
        }
    }

    /* Use new tiles */
    for(int b=0; b<=2; b++)
    for(int t=0; t<=3; t++)
    {
        s->useBgTile(identified_tiles_v[b][t]);
    }

    /* Everything was successful, we can save the changes */
    s->clearReservations();
    editor_scenery = *s;
    for(int b=0; b<=2; b++)
    {
        editor_block_0_idxs[b] = (quint8)identified_tiles_v[b][0];
        editor_block_1_idxs[b] = (quint8)identified_tiles_v[b][1];
        editor_block_2_idxs[b] = (quint8)identified_tiles_v[b][2];
        editor_block_3_idxs[b] = (quint8)identified_tiles_v[b][3];
    }

IMG2MAP_BG_EXIT:
    delete s;
    return ret;
}

bool Props::img2cnfTile(quint8 index, shared_ptr<GfxData> img)
{
    bool ret = true;

    /* Temporary scenery
     * All modificiations will be done in the temporary scenery below.
     * It will be dropped in case of failure (lack of resources to store new data).
     * It will be replaced with scenery got in paramaterers in case of success.
     */
    Scenery *s = editor_scenery.copy();

    QVector<quint64> new_chr_v;

    CnfTile &tile = s->cnf_tile_vector[index].tile;

    /* Use another tile to track which characters have been identified.
     * 0 = not identified (default value in constructor)
     * 1 = identified
     */
    CnfTile identified_chars;

    /* Free characters of tile being modified */
    for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
        s->freeCnfChar(tile.char_idxs.at(row).at(col));

    /* Identify existing characters */
    for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
    {
         /* Get character data from 'img' and find matching character.
          * Note that functions of 'img' use different parameter order!
          * 'x' coord is 'col' and 'y' coord is 'row'!
          */
        quint64 chr = img->chr(col, row);
        qint16 chr_ind = s->findChar(chr); /* index of character */

        if(0 <= chr_ind)
        {
            /* Matching character found
             * Store its index to the tile.
             * Reserve the character, to make sure that creation of
             * a new character will not override it until it will be
             * registered as used! Note that it may not be used at this moment!
             */
            tile.char_idxs[row][col] = chr_ind;
            identified_chars.char_idxs[row][col] = 1;
            s->reserveChar(chr_ind);
        }
        else
        {
            /* Found a new character, we will create it later */
            new_chr_v.append(chr);
        }
    } /* foreach row & col */

    /* Create new characters */
    for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
    {
        /* Skip if it has already been identified */
        if(!identified_chars.char_idxs[row][col])
        {
            /* It is a new character, let's create it! */
            quint64 chr = new_chr_v.takeFirst();

            /* new_chr_v may contain the same character several times.
             * Verify wether the current character has already been created
             * in this very loop.
             */
            qint16 chr_ind = s->findChar(chr); /* index of character */

            if(chr_ind < 0)
            {
                /* Character must be created */
                chr_ind = s->createChar(chr);
                s->reserveChar(chr_ind);
            }

            if(0 <= chr_ind)
            {
                tile.char_idxs[row][col] = chr_ind;
            }
            else
            {
                /* out of resources */
                ret = false;
                goto IMG2MAP_CNF_TILE_EXIT;
            }
        }

        /* Store color value */
        tile.colors[row][col] = img->clrVal(col,row);
    } /* foreach row & col */

    /* Registere character usage of tile got from img */
    for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
        s->useCnfChar(tile.char_idxs.at(row).at(col));

    /* Everything was successful, we can save the changes */
    s->clearReservations();
    editor_scenery = *s;

IMG2MAP_CNF_TILE_EXIT:
    delete s;
    return ret;
}

bool Props::img2bgTile(quint8 index, shared_ptr<GfxData> img)
{
    bool ret = true;

    /* Temporary scenery
     * All modificiations will be done in the temporary scenery below.
     * It will be dropped in case of failure (lack of resources to store new data).
     * It will be replaced with scenery got in paramaterers in case of success.
     */
    Scenery *s = editor_scenery.copy();

    QVector<quint64> new_chr_v;

    BgTile &tile = s->bg_tile_vector[index].tile;

    /* Use another tile to track which characters have been identified.
     * 0 = not identified (default value in constructor)
     * 1 = identified
     */
    BgTile identified_chars;

    /* Free characters of tile being modified */
    for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
        s->freeBgChar(tile.char_idxs.at(row).at(col));

    /* Identify existing characters */
    for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
    {
         /* Get character data from 'img' and find matching character.
          * Note that functions of 'img' use different parameter order!
          * 'x' coord is 'col' and 'y' coord is 'row'!
          */
        quint64 chr = img->chr(col, row);
        qint16 chr_ind = s->findChar(chr); /* index of character */

        if(0 <= chr_ind)
        {
            /* Matching character found
             * Store its index to the tile.
             * Reserve the character, to make sure that creation of
             * a new character will not override it until it will be
             * registered as used! Note that it may not be used at this moment!
             */
            tile.char_idxs[row][col] = chr_ind;
            identified_chars.char_idxs[row][col] = 1;
            s->reserveChar(chr_ind);
        }
        else
        {
            /* Found a new character, we will create it later */
            new_chr_v.append(chr);
        }
    } /* foreach row & col */

    /* Create new characters */
    for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
    {
        /* Skip if it has already been identified */
        if(!identified_chars.char_idxs[row][col])
        {
            /* It is a new character, let's create it! */
            quint64 chr = new_chr_v.takeFirst();

            /* new_chr_v may contain the same character several times.
             * Verify wether the current character has already been created
             * in this very loop.
             */
            qint16 chr_ind = s->findChar(chr); /* index of character */

            if(chr_ind < 0)
            {
                /* Character must be created */
                chr_ind = s->createChar(chr);
                s->reserveChar(chr_ind);
            }

            if(0 <= chr_ind)
            {
                tile.char_idxs[row][col] = chr_ind;
            }
            else
            {
                /* out of resources */
                ret = false;
                goto IMG2MAP_BG_TILE_EXIT;
            }
        }

        /* Store color value */
        tile.colors[row][col] = img->clrVal(col,row);
    } /* foreach row & col */

    /* Registere character usage of tile got from img */
    for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
    for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
        s->useBgChar(tile.char_idxs.at(row).at(col));

    /* Everything was successful, we can save the changes */
    s->clearReservations();
    editor_scenery = *s;

IMG2MAP_BG_TILE_EXIT:
    delete s;
    return ret;
}

bool Props::img2mapSpriteCeiling(shared_ptr<GfxData> img)
{
    return img2mapSpriteCnf(img, editor_ceiling_idx, editor_ceiling_clr);
}

bool Props::img2mapSpriteFloor(shared_ptr<GfxData> img)
{
    return img2mapSpriteCnf(img, editor_floor_idx, editor_floor_clr);
}

bool Props::img2mapSpriteCnf(shared_ptr<GfxData> img, quint8 &idx, quint8 &clr)
{
    bool ret = true;

    /* Temporary scenery
     * All modificiations will be done in the temporary scenery below.
     * It will be dropped in case of failure (lack of resources to store new data).
     * It will be replaced with scenery got in paramaterers in case of success.
     */
    Scenery *s = editor_scenery.copy();

    s->freeSprite(idx);

    Sprite sprite(img);

    qint16 sprite_ind = s->findSprite(sprite);
    if(sprite_ind < 0)
    {
        /* not found, let's create it */
        sprite_ind = s->createSprite(sprite);
        if(sprite_ind < 0)
        {
            /* out of resources */
            ret = false;
            goto IMG2MAP_SPRITE_EXIT;
        }
    }
    s->useSprite(sprite_ind);
    editor_scenery = *s;
    idx = sprite_ind;
    clr = img->color((int)GfxData::ColorIndex::Color);

IMG2MAP_SPRITE_EXIT:
    delete s;
    return ret;
}

bool Props::img2mapWall(shared_ptr<GfxData> img)
{
    bool ret = true;

    /* Temporary scenery
     * All modificiations will be done in the temporary scenery below.
     * It will be dropped in case of failure (lack of resources to store new data).
     * It will be replaced with scenery got in paramaterers in case of success.
     */
    Scenery *s = editor_scenery.copy();

    QVector<Sprite> new_sprite_v;

    Wall identified_sprites;
    Wall wall;
    qint16 wall_ind;

    s->freeWall(editor_wall_idx);

    /* Identify existing sprites */
    for(int row=0; row<SCENERY_WALL_ROWS; row++)
    {
        Sprite sprite(img, row);
        qint16 spr_ind = s->findSprite(sprite);
        if(0 <= spr_ind)
        {
            /* Matching sprite found, let's store its index!
             * Reserve the sprite, to make sure that creation of
             * a new sprite will not override it until it will be
             * registered as used! Note that it may not be used at this moment!
             */
            wall.sprite_idxs[row] = spr_ind;
            identified_sprites.sprite_idxs[row] = 1;
            s->reserveSprite(spr_ind);
        }
        else
        {
            /* Found a new sprite, we will create it later */
            new_sprite_v.append(sprite);
        }
    }

    /* Create new sprites */
    for(int row=0; row<SCENERY_WALL_ROWS; row++)
    {
        /* Skip if it has already been identified */
        if(!identified_sprites.sprite_idxs[row])
        {
            Sprite sprite = new_sprite_v.takeFirst();

            /* new_sprite_v may contain the same sprite several times.
             * Verify wether the current sprite has already been created
             * in this very loop.
             */
            qint16 spr_ind = s->findSprite(sprite);

            if(spr_ind < 0)
            {
                /* Sprite must be created */
                spr_ind = s->createSprite(sprite);
                s->reserveSprite(spr_ind);
            }

            if(0 <= spr_ind)
            {
                wall.sprite_idxs[row] = spr_ind;
            }
            else
            {
                /* out of resources */
                ret = false;
                goto IMG2MAP_WALL_EXIT;
            }
        }
    }

    wall_ind = s->findWall(wall);

    if(wall_ind < 0)
    {
        wall_ind = s->createWall(wall);
        if(wall_ind < 0)
        {
            /* out of resources */
            ret = false;
            goto IMG2MAP_WALL_EXIT;
        }
    }

    /* Everything was successful, we can save the changes */
    s->useWall(wall_ind);
    s->clearReservations();
    editor_scenery = *s;
    editor_wall_idx = wall_ind;
    editor_wall_clr = img->color((int)GfxData::ColorIndex::Color);

IMG2MAP_WALL_EXIT:
    delete s;
    return ret;
}

void Props::img2sprite(quint8 index, shared_ptr<GfxData> img)
{
    editor_scenery.sprite_vector[index].sprite.load_gfx_data(img);
}

void Props::img2wall(quint8 index, shared_ptr<GfxData> img)
{
    /* TODO img2wall() */
}
