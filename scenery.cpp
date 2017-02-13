
#include "map_general.h"

Scenery::Scenery():
    chr_vector(SCENERY_CHR_NUM),           chr_counter(0), cnf_chr_counter(0),
    bg_tile_vector(SCENERY_BG_TILE_NUM),    bg_tile_counter(0),
    cnf_tile_vector(SCENERY_CNF_TILE_NUM),  cnf_tile_counter(0),
    sprite_vector(SCENERY_SPRITE_NUM),      sprite_counter(0),
    wall_vector(SCENERY_WALL_NUM),          wall_counter(0)
{
    loadDefaultValues();
}

Scenery::~Scenery()
{

}

void Scenery::loadDefaultValues()
{
    name = "Default Scenery";
}

Scenery *Scenery::copy()
{
    Scenery *ret = new Scenery();
    ret->chr_vector      = chr_vector;      ret->chr_counter      = chr_counter;
                                            ret->cnf_chr_counter  = cnf_chr_counter;
    ret->bg_tile_vector  = bg_tile_vector;  ret->bg_tile_counter  = bg_tile_counter;
    ret->cnf_tile_vector = cnf_tile_vector; ret->cnf_tile_counter = cnf_tile_counter;
    ret->sprite_vector   = sprite_vector;   ret->sprite_counter   = sprite_counter;
    ret->wall_vector     = wall_vector;     ret->wall_counter     = wall_counter;
    return ret;
}

/*******************************************************************************
****
****    C H A R
****
*******************************************************************************/

qint16 Scenery::findBgChar(quint64 chr)
{
    qint16 match = -1;
    for(qint16 i=0; i<SCENERY_CHR_NUM; i++)
    {
        if(chr_vector.at(i).chr == chr)
        {
            match = i;
            break;
        }
    }
    return match;
}

qint16 Scenery::findCnfChar(quint64 chr)
{
    qint16 match = findBgChar(chr);
    if(-1 < match && chr_vector.at(match).cnf_usage > 0)
    {
        return match;
    }
    return -1;
}

qint16 Scenery::bgCharInsert(quint64 chr)
{
    return charInsert(chr, false); /* BG char */
}

qint16 Scenery::cnfCharInsert(quint64 chr)
{
    return charInsert(chr, true); /* C&F char */
}

void Scenery::bgCharRemove(quint8 ind)
{
    quint32 usage = chr_vector.at(ind).usage;
    usage--;
    if(!usage) chr_counter--;
    chr_vector[ind].usage = usage;
}

void Scenery::cnfCharRemove(quint8 ind)
{
    quint32 cnf_usage = chr_vector.at(ind).cnf_usage;
    cnf_usage--;
    if(!cnf_usage) cnf_chr_counter--;
    chr_vector[ind].cnf_usage = cnf_usage;
    bgCharRemove(ind); /* also remove overall usage */
}

/* Looks for the given character in the character table.
 * Inserts the character into the table if it has not been found.
 * Returns the index of the character or -1 in case of failure.
 */
qint16 Scenery::charInsert(quint64 chr, bool cnf)
{
    /* Look for the first matching character and for the first unused index */
    qint16 match = -1;
    qint16 empty = -1;

    for(qint16 i=0; i<SCENERY_CHR_NUM; i++)
    {
        if(match < 0 && chr_vector.at(i).chr == chr) match = i;
        if(empty < 0 && chr_vector.at(i).usage == 0) empty = i;
    }

    if( 0 < match ) /* there is a matching character */
    {
        if(cnf)
        {
            /* Has this char already been used by any C&F tiles? */
            if(chr_vector.at(match).cnf_usage == 0)
            {
                /* No, it has not been used by any C&F tiles yet */

                /* It can happen that there is a matching char but it is not used
                 * by any C&F tiles and there is no more space for a new C&F char.
                 */
                if(cnf_chr_counter >= SCENERY_CNF_CHR_NUM) return -1;

                cnf_chr_counter++;
            }

            chr_vector[match].cnf_usage++;
        }

        /* Has this char already been used at all? */
        /* Do not forget that matching does not ensure that it is actually used! */
        if(chr_vector.at(match).usage == 0)
        {
            /* No, it has not been used by any C&F tiles yet */
            chr_counter++;
        }

        chr_vector[match].usage++;

        return match;
    }

    if( 0 < empty ) /* there was no matching character, but we have an unused index */
    {
        if(cnf)
        {
            /* It can happen that there is an unused index but we do not have
             * more space for a new C&F char.
             */
            if(cnf_chr_counter >= SCENERY_CNF_CHR_NUM) return -1;

            cnf_chr_counter++;
            chr_vector[empty].cnf_usage++;
        }

        chr_vector[empty].chr = chr; /* store the new char */
        chr_counter++;
        chr_vector[empty].usage++;

        return empty;
    }

    return -1; /* no matching char, no unused index :-( */
}

void Scenery::charModify(quint8 ind, quint64 chr)
{
    chr_vector[ind].chr = chr;
}

/*******************************************************************************
****
****    T I L E
****
*******************************************************************************/

qint16 Scenery::findBgTile(BgTile tile)
{
    qint16 match = -1;
    for(qint16 i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        if(bg_tile_vector.at(i).bg_tile == tile)
        {
            match = i;
            break;
        }
    }
    return match;
}

qint16 Scenery::findCnfTile(CnfTile tile)
{
    qint16 match = -1;
    for(qint16 i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(cnf_tile_vector.at(i).cnf_tile == tile)
        {
            match = i;
            break;
        }
    }
    return match;
}

qint16 Scenery::bgTileInsert(BgTile tile)
{
    /* Look for the first match and for the first unused index */
    qint16 match = -1;
    qint16 empty = -1;

    for(qint16 i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        if(match < 0 && bg_tile_vector.at(i).bg_tile == tile) match = i;
        if(empty < 0 && bg_tile_vector.at(i).usage == 0) empty = i;
    }

    if( 0 < match ) /* there is a match */
    {
        if(bg_tile_vector.at(match).usage == 0) bg_tile_counter++;
        bg_tile_vector[match].usage++;
        return match;
    }

    if( 0 < empty ) /* there was no match, but we have an unused index */
    {
        bg_tile_vector[empty].bg_tile = tile; /* store the new char */
        bg_tile_counter++;
        bg_tile_vector[empty].usage++;
        return empty;
    }

    return -1; /* no match, no unused index :-( */
}

qint16 Scenery::cnfTileInsert(CnfTile tile)
{
    /* Look for the first match and for the first unused index */
    qint16 match = -1;
    qint16 empty = -1;

    for(qint16 i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(match < 0 && cnf_tile_vector.at(i).cnf_tile == tile) match = i;
        if(empty < 0 && cnf_tile_vector.at(i).usage == 0) empty = i;
    }

    if( 0 < match ) /* there is a match */
    {
        if(cnf_tile_vector.at(match).usage == 0) cnf_tile_counter++;
        cnf_tile_vector[match].usage++;
        return match;
    }

    if( 0 < empty ) /* there was no match, but we have an unused index */
    {
        cnf_tile_vector[empty].cnf_tile = tile; /* store the new char */
        cnf_tile_counter++;
        cnf_tile_vector[empty].usage++;
        return empty;
    }

    return -1; /* no match, no unused index :-( */
}

void Scenery::bgTileRemove(quint8 ind)
{
    quint32 usage = bg_tile_vector.at(ind).usage;
    usage--;
    if(!usage) bg_tile_counter--;
    bg_tile_vector[ind].usage = usage;
}

void Scenery::cnfTileRemove(quint8 ind)
{
    quint32 usage = cnf_tile_vector.at(ind).usage;
    usage--;
    if(!usage) cnf_tile_counter--;
    cnf_tile_vector[ind].usage = usage;
}

void Scenery::bgTileModify(quint8 ind, BgTile tile)
{
    bg_tile_vector[ind].bg_tile = tile;
}

void Scenery::cnfTileModify(quint8 ind, CnfTile tile)
{
    cnf_tile_vector[ind].cnf_tile = tile;
}

/*******************************************************************************
****
****    S P R I T E
****
*******************************************************************************/

qint16 Scenery::findSprite(Sprite sprite)
{
    qint16 match = -1;
    for(qint16 i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        if(sprite_vector.at(i).sprite == sprite)
        {
            match = i;
            break;
        }
    }
    return match;
}

qint16 Scenery::spriteInsert(Sprite sprite)
{
    /* Look for the first match and for the first unused index */
    qint16 match = -1;
    qint16 empty = -1;

    for(qint16 i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        if(match < 0 && sprite_vector.at(i).sprite == sprite) match = i;
        if(empty < 0 && sprite_vector.at(i).usage == 0) empty = i;
    }

    if( 0 < match ) /* there is a match */
    {
        if(sprite_vector.at(match).usage == 0) sprite_counter++;
        sprite_vector[match].usage++;
        return match;
    }

    if( 0 < empty ) /* there was no match, but we have an unused index */
    {
        sprite_vector[empty].sprite = sprite; /* store the new char */
        sprite_counter++;
        sprite_vector[empty].usage++;
        return empty;
    }

    return -1; /* no match, no unused index :-( */
}

void Scenery::spriteRemove(quint8 ind)
{
    quint32 usage = sprite_vector.at(ind).usage;
    usage--;
    if(!usage) sprite_counter--;
    sprite_vector[ind].usage = usage;
}

void Scenery::spriteModify(quint8 ind, Sprite sprite)
{
    sprite_vector[ind].sprite = sprite;
}

/*******************************************************************************
****
****    S P R I T E   W A L L
****
*******************************************************************************/

qint16 Scenery::findWall(Wall wall)
{
    qint16 match = -1;
    for(qint16 i=0; i<SCENERY_WALL_NUM; i++)
    {
        if(wall_vector.at(i).wall == wall)
        {
            match = i;
            break;
        }
    }
    return match;
}

qint16 Scenery::wallInsert(Wall wall)
{
    /* Look for the first match and for the first unused index */
    qint16 match = -1;
    qint16 empty = -1;

    for(qint16 i=0; i<SCENERY_WALL_NUM; i++)
    {
        if(match < 0 && wall_vector.at(i).wall == wall) match = i;
        if(empty < 0 && wall_vector.at(i).usage == 0) empty = i;
    }

    if( 0 < match ) /* there is a match */
    {
        if(wall_vector.at(match).usage == 0) wall_counter++;
        wall_vector[match].usage++;
        return match;
    }

    if( 0 < empty ) /* there was no match, but we have an unused index */
    {
        wall_vector[empty].wall = wall; /* store the new char */
        wall_counter++;
        wall_vector[empty].usage++;
        return empty;
    }

    return -1; /* no match, no unused index :-( */
}

void Scenery::wallRemove(quint8 ind)
{
    quint32 usage = wall_vector.at(ind).usage;
    usage--;
    if(!usage) wall_counter--;
    wall_vector[ind].usage = usage;
}

void Scenery::wallModify(quint8 ind, Wall wall)
{
    wall_vector[ind].wall = wall;
}

/* Remap characters in order to move every EMC char
 * into the first quarter of character set (0-63 indexes).
 */
void Scenery::calculateRealCharIndexes(void)
{
    int cnf = 0; /* C&F - EMC char index */
    int  bg = SCENERY_CHR_NUM-1; /* BG - normal char index */

    /* search for C&F chars first */
    for(int i=0; i<SCENERY_CHR_NUM; i++)
    {
        if(chr_vector.at(i).cnf_usage > 0)
        {
            chr_vector[i].mapping = cnf;
            cnf++;
        }
        else
        {
            /* Note that unused chars are also stored!
             * It is intentional. Perhaps, they will be
             * used again later...
             */
            chr_vector[i].mapping = bg;
            bg--;
        }
    }
}

void Scenery::resetRealCharIndexes(void)
{
    int size = chr_vector.size();
    for(int i=0; i<size; i++)
        chr_vector[i].mapping = i;
}

/* Remap a character */
quint8 Scenery::realCharIndex(quint8 const ind) const
{
    return chr_vector.at(ind).mapping;
}
