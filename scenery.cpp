
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

qint16 Scenery::createChar(quint64 chr)
{
    for(qint16 i=0; i<SCENERY_CHR_NUM; i++)
    {
        if(!chr_vector.at(i).usage)
        {
            /* We have just found an unused character.
             * Let's overwrite it with the new data.
             * It is the caller's responsibility to register usage of the character!
             * Caller must check that return value is non-negative (means success)
             * and call useBgChar() or useCnfChar()!
             */
            modifyChar(i,chr);
            return i; /* return index of created character */
        }
    }
    return -1; /* failure, all characters are in use */
}

qint16 Scenery::findChar(quint64 chr)
{
    for(qint16 i=0; i<SCENERY_CHR_NUM; i++)
    {
        if(!chr_vector.at(i).usage)
        {
             /* Unused characters are not allowed to be returned even if they match!
              * It would break functions of props_img2map.cpp!
              */
            continue;
        }
        if(chr_vector.at(i).chr == chr)
        {
            return i; /* found a match */
        }
    }
    return -1; /* no match was found */
}

void Scenery::modifyChar(quint8 ind, quint64 chr)
{
    chr_vector[ind].chr = chr;
}

void Scenery::useBgChar(quint8 ind)
{
    quint32 usage = chr_vector.at(ind).usage;
    if(!usage) chr_counter++; /* overall character usage in scenery */
    usage++;
    chr_vector[ind].usage = usage;
}

void Scenery::freeBgChar(quint8 ind)
{
    quint32 usage = chr_vector.at(ind).usage;
    usage--;
    if(!usage) chr_counter--; /* overall character usage in scenery */
    chr_vector[ind].usage = usage;
}

bool Scenery::useCnfChar(quint8 ind)
{
    quint32 cnf_usage = chr_vector.at(ind).cnf_usage;
    if(!cnf_usage)
    {
        /* check if there is room for a new ECM character */
        if(cnf_chr_counter == SCENERY_CNF_CHR_NUM-1) return false;
        cnf_chr_counter++; /* overall ECM character usage in scenery */
    }
    cnf_usage++;
    chr_vector[ind].cnf_usage = cnf_usage;
    useBgChar(ind);
    return true;
}

void Scenery::freeCnfChar(quint8 ind)
{
    quint32 cnf_usage = chr_vector.at(ind).cnf_usage;
    cnf_usage--;
    if(!cnf_usage) cnf_chr_counter--; /* overall ECM character usage in scenery */
    chr_vector[ind].cnf_usage = cnf_usage;
    freeBgChar(ind);
}

/*******************************************************************************
****
****    T I L E
****
*******************************************************************************/

qint16 Scenery::createBgTile(BgTile tile)
{
    for(qint16 i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        if(!bg_tile_vector.at(i).usage && !bg_tile_vector.at(i).keep)
        {
            /* We have just found an unused tile.
             * Let's overwrite it with the new data.
             * It is the caller's responsibility to register usage of the tile!
             * Caller must check that return value is non-negative (means success)
             * and call useBgTile()!
             */
            modifyBgTile(i,tile);
            return i; /* return index of created tile */
        }
    }
    return -1; /* failure, all tiles are in use */
}

qint16 Scenery::findBgTile(BgTile tile)
{
    for(qint16 i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        if(!bg_tile_vector.at(i).usage && !bg_tile_vector.at(i).keep)
        {
             /* Unused but not protected tiles are not allowed to be returned even if they match!
              * It would break functions of props_img2map.cpp!
              */
            continue;
        }
        if(bg_tile_vector.at(i).tile == tile)
        {
            return i; /* found a match */
        }
    }
    return -1; /* no match was found */
}

void Scenery::modifyBgTile(quint8 ind, BgTile tile)
{
    bg_tile_vector[ind].tile = tile;
}

void Scenery::useBgTile(quint8 ind)
{
    quint32 usage = bg_tile_vector.at(ind).usage;
    if(!usage) bg_tile_counter++; /* overall tile usage in scenery */
    usage++;
    bg_tile_vector[ind].usage = usage;
}

void Scenery::freeBgTile(quint8 ind)
{
    quint32 usage = bg_tile_vector.at(ind).usage;
    usage--;
    if(!usage) bg_tile_counter--; /* overall tile usage in scenery */
    bg_tile_vector[ind].usage = usage;
}

qint16 Scenery::createCnfTile(CnfTile tile)
{
    for(qint16 i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(!cnf_tile_vector.at(i).usage && !cnf_tile_vector.at(i).keep)
        {
            /* We have just found an unused tile.
             * Let's overwrite it with the new data.
             * It is the caller's responsibility to register usage of the tile!
             * Caller must check that return value is non-negative (means success)
             * and call useCnfTile()!
             */
            modifyCnfTile(i,tile);
            return i; /* return index of created tile */
        }
    }
    return -1; /* failure, all tiles are in use */
}

qint16 Scenery::findCnfTile(CnfTile tile)
{
    for(qint16 i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(!cnf_tile_vector.at(i).usage && !cnf_tile_vector.at(i).keep)
        {
             /* Unused but not protected tiles are not allowed to be returned even if they match!
              * It would break functions of props_img2map.cpp!
              */
            continue;
        }
        if(cnf_tile_vector.at(i).tile == tile)
        {
            return i; /* found a match */
        }
    }
    return -1; /* no match was found */
}

void Scenery::modifyCnfTile(quint8 ind, CnfTile tile)
{
    cnf_tile_vector[ind].tile = tile;
}

void Scenery::useCnfTile(quint8 ind)
{
    quint32 usage = cnf_tile_vector.at(ind).usage;
    if(!usage) cnf_tile_counter++; /* overall tile usage in scenery */
    usage++;
    cnf_tile_vector[ind].usage = usage;
}

void Scenery::freeCnfTile(quint8 ind)
{
    quint32 usage = cnf_tile_vector.at(ind).usage;
    usage--;
    if(!usage) cnf_tile_counter--; /* overall tile usage in scenery */
    cnf_tile_vector[ind].usage = usage;
}

/*******************************************************************************
****
****    S P R I T E
****
*******************************************************************************/

qint16 Scenery::createSprite(Sprite sprite)
{
    for(qint16 i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        if(!sprite_vector.at(i).usage && !sprite_vector.at(i).keep)
        {
            /* We have just found an unused sprite.
             * Let's overwrite it with the new data.
             * It is the caller's responsibility to register usage of the sprite!
             * Caller must check that return value is non-negative (means success)
             * and call useSprite()!
             */
            modifySprite(i,sprite);
            return i; /* return index of created sprite */
        }
    }
    return -1; /* failure, all sprites are in use */
}

qint16 Scenery::findSprite(Sprite sprite)
{
    for(qint16 i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        if(!sprite_vector.at(i).usage && !sprite_vector.at(i).keep)
        {
             /* Unused but not protected sprites are not allowed to be returned even if they match!
              * It would break functions of props_img2map.cpp!
              */
            continue;
        }
        if(sprite_vector.at(i).sprite == sprite)
        {
            return i; /* found a match */
        }
    }
    return -1; /* no match was found */
}

void Scenery::modifySprite(quint8 ind, Sprite sprite)
{
    sprite_vector[ind].sprite = sprite;
}

void Scenery::useSprite(quint8 ind)
{
    quint32 usage = sprite_vector.at(ind).usage;
    if(!usage) sprite_counter++; /* overall sprite usage in scenery */
    usage++;
    sprite_vector[ind].usage = usage;
}

void Scenery::freeSprite(quint8 ind)
{
    quint32 usage = sprite_vector.at(ind).usage;
    usage--;
    if(!usage) sprite_counter--; /* overall sprite usage in scenery */
    sprite_vector[ind].usage = usage;
}

/*******************************************************************************
****
****    W A L L
****
*******************************************************************************/

qint16 Scenery::createWall(Wall wall)
{
    for(qint16 i=0; i<SCENERY_WALL_NUM; i++)
    {
        if(!wall_vector.at(i).usage && !wall_vector.at(i).keep)
        {
            /* We have just found an unused wall.
             * Let's overwrite it with the new data.
             * It is the caller's responsibility to register usage of the wall!
             * Caller must check that return value is non-negative (means success)
             * and call useWall()!
             */
            modifyWall(i,wall);
            return i; /* return index of created wall */
        }
    }
    return -1; /* failure, all walls are in use */
}

qint16 Scenery::findWall(Wall wall)
{
    for(qint16 i=0; i<SCENERY_WALL_NUM; i++)
    {
        if(!wall_vector.at(i).usage && !wall_vector.at(i).keep)
        {
             /* Unused but not protected walls are not allowed to be returned even if they match!
              * It would break functions of props_img2map.cpp!
              */
            continue;
        }
        if(wall_vector.at(i).wall == wall)
        {
            return i; /* found a match */
        }
    }
    return -1; /* no match was found */
}

void Scenery::modifyWall(quint8 ind, Wall wall)
{
    wall_vector[ind].wall = wall;
}

void Scenery::useWall(quint8 ind)
{
    quint32 usage = wall_vector.at(ind).usage;
    if(!usage) wall_counter++; /* overall wall usage in scenery */
    usage++;
    wall_vector[ind].usage = usage;
}

void Scenery::freeWall(quint8 ind)
{
    quint32 usage = wall_vector.at(ind).usage;
    usage--;
    if(!usage) wall_counter--; /* overall wall usage in scenery */
    wall_vector[ind].usage = usage;
}

/*******************************************************************************
****
****    R E M A P
****
*******************************************************************************/

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
