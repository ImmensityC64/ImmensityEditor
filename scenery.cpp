
#include "map_general.h"

Scenery::Scenery():
    /* container vectors */
    chr_vector(SCENERY_CHR_NUM),
    bg_tile_vector(SCENERY_BG_TILE_NUM),
    cnf_tile_vector(SCENERY_CNF_TILE_NUM),
    sprite_vector(SCENERY_SPRITE_NUM),
    wall_vector(SCENERY_WALL_NUM)
{
    loadDefaultValues();
}

Scenery::~Scenery()
{

}

void Scenery::loadDefaultValues()
{
    name = "Default Scenery";
    clearUsage();
}

void Scenery::clearUsage()
{
    for(int i=0; i<SCENERY_CHR_NUM; i++)
    {
        chr_vector[i].usage     = 0;
        chr_vector[i].cnf_usage = 0;
    }

    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
        bg_tile_vector[i].usage = 0;

    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
        cnf_tile_vector[i].usage = 0;

    for(int i=0; i<SCENERY_SPRITE_NUM; i++)
        sprite_vector[i].usage = 0;

    for(int i=0; i<SCENERY_WALL_NUM; i++)
        wall_vector[i].usage = 0;

    /* overall usage counters */
    chr_counter      = 0;
    cnf_chr_counter  = 0;
    bg_tile_counter  = 0;
    cnf_tile_counter = 0;
    sprite_counter   = 0;
    wall_counter     = 0;
}


Scenery &Scenery::operator=(const Scenery& other)
{
    chr_vector      = other.chr_vector;
    bg_tile_vector  = other.bg_tile_vector;
    cnf_tile_vector = other.cnf_tile_vector;
    sprite_vector   = other.sprite_vector;
    wall_vector     = other.wall_vector;

    chr_counter      = other.chr_counter;
    cnf_chr_counter  = other.cnf_chr_counter;
    bg_tile_counter  = other.bg_tile_counter;
    cnf_tile_counter = other.cnf_tile_counter;
    sprite_counter   = other.sprite_counter;
    wall_counter     = other.wall_counter;

    return *this;
}

Scenery *Scenery::copy()
{
    Scenery *ret = new Scenery();

    *ret = *this;

    return ret;
}

void Scenery::clearReservations()
{
    for(int i=0; i<SCENERY_CHR_NUM; i++)
        chr_vector[i].reserved  = false;

    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
        bg_tile_vector[i].reserved = false;

    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
        cnf_tile_vector[i].reserved = false;

    for(int i=0; i<SCENERY_SPRITE_NUM; i++)
        sprite_vector[i].reserved = false;

    for(int i=0; i<SCENERY_WALL_NUM; i++)
        wall_vector[i].reserved = false;
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
        if(!chr_vector.at(i).usage &&
           !chr_vector.at(i).reserved)
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
        if(chr_vector.at(i).chr == chr)
            return i; /* found a match */
    return -1; /* no match was found */
}

void Scenery::reserveChar(quint8 ind)
{
    chr_vector[ind].reserved = true;
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

    /* Decrease element usage */
    if(!usage)
    {
        /* Report error, this function should have not been called if element is unused! */
        cerr << __func__ << "(" << ind << ") was called, but element is unused!" << endl;
    }
    else
    {
        /* It can be decreased, do it! */
        usage--;

        /* Has element just become unused? */
        if(!usage)
        {
            /* Decrease overall usage counter in scenery */
            if(!chr_counter)
            {
                /* Report error, this counter should not be zero! */
                cerr << __func__ << "(" << ind << ") tries to decrease overall counter but it is zero already!" << endl;
            }
            else
            {
                chr_counter--;
            }
        }
    }

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
    /* ECM characters are a subset of character set, so, they counted among bgChars too */
    useBgChar(ind);
    return true;
}

void Scenery::freeCnfChar(quint8 ind)
{
    quint32 cnf_usage = chr_vector.at(ind).cnf_usage;

    /* Decrease element usage */
    if(!cnf_usage)
    {
        /* Report error, this function should have not been called if element is unused! */
        cerr << __func__ << "(" << ind << ") was called, but element is unused!" << endl;
    }
    else
    {
        /* It can be decreased, do it! */
        cnf_usage--;

        /* Has element just become unused? */
        if(!cnf_usage)
        {
            /* Decrease overall usage counter in scenery */
            if(!cnf_chr_counter)
            {
                /* Report error, this counter should not be zero! */
                cerr << __func__ << "(" << ind << ") tries to decrease overall counter but it is zero already!" << endl;
            }
            else
            {
                cnf_chr_counter--;
            }
        }
    }

    chr_vector[ind].cnf_usage = cnf_usage;

    /* ECM characters are a subset of character set, so, they counted among bgChars too */
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
        if(!bg_tile_vector.at(i).usage &&
           !bg_tile_vector.at(i).keep  &&
           !bg_tile_vector.at(i).reserved)
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
        if(bg_tile_vector.at(i).tile == tile)
            return i; /* found a match */
    return -1; /* no match was found */
}

void Scenery::reserveBgTile(quint8 ind)
{
    bg_tile_vector[ind].reserved = true;
}

void Scenery::modifyBgTile(quint8 ind, BgTile tile)
{
    bg_tile_vector[ind].tile = tile;
}

void Scenery::useBgTile(quint8 ind)
{
    quint32 usage = bg_tile_vector.at(ind).usage;
    if(!usage)
    {
        /* it is being used the first time */
        bg_tile_counter++; /* overall tile usage in scenery */
        BgTile tile = bg_tile_vector.at(ind).tile;
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                useBgChar(tile.char_idxs.at(row).at(col));
    }
    usage++;
    bg_tile_vector[ind].usage = usage;
}

void Scenery::freeBgTile(quint8 ind)
{
    quint32 usage = bg_tile_vector.at(ind).usage;

    /* Decrease element usage */
    if(!usage)
    {
        /* Report error, this function should have not been called if element is unused! */
        cerr << __func__ << "(" << ind << ") was called, but element is unused!" << endl;
    }
    else
    {
        /* It can be decreased, do it! */
        usage--;

        /* Has element just become unused? */
        if(!usage)
        {
            /* Decrease overall usage counter in scenery */
            if(!bg_tile_counter)
            {
                /* Report error, this counter should not be zero! */
                cerr << __func__ << "(" << ind << ") tries to decrease overall counter but it is zero already!" << endl;
            }
            else
            {
                bg_tile_counter--;
            }

            BgTile tile = bg_tile_vector.at(ind).tile;
            for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
                for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                    freeBgChar(tile.char_idxs.at(row).at(col));
        }
    }

    bg_tile_vector[ind].usage = usage;
}

qint16 Scenery::createCnfTile(CnfTile tile)
{
    for(qint16 i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        if(!cnf_tile_vector.at(i).usage &&
           !cnf_tile_vector.at(i).keep  &&
           !cnf_tile_vector.at(i).reserved)
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
        if(cnf_tile_vector.at(i).tile == tile)
            return i; /* found a match */
    return -1; /* no match was found */
}

void Scenery::reserveCnfTile(quint8 ind)
{
    cnf_tile_vector[ind].reserved = true;
}

void Scenery::modifyCnfTile(quint8 ind, CnfTile tile)
{
    cnf_tile_vector[ind].tile = tile;
}

bool Scenery::useCnfTile(quint8 ind)
{
    quint32 usage = cnf_tile_vector.at(ind).usage;
    if(!usage)
    {
        /* it is being used the first time */
        cnf_tile_counter++; /* overall tile usage in scenery */
        CnfTile tile = cnf_tile_vector.at(ind).tile;
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
            {
                if(!useCnfChar(tile.char_idxs.at(row).at(col)))
                    return false;
            }
    }
    usage++;
    cnf_tile_vector[ind].usage = usage;
    return true;
}

void Scenery::freeCnfTile(quint8 ind)
{
    quint32 usage = cnf_tile_vector.at(ind).usage;

    /* Decrease element usage */
    if(!usage)
    {
        /* Report error, this function should have not been called if element is unused! */
        cerr << __func__ << "(" << ind << ") was called, but element is unused!" << endl;
    }
    else
    {
        /* It can be decreased, do it! */
        usage--;

        /* Has element just become unused? */
        if(!usage)
        {
            /* Decrease overall usage counter in scenery */
            if(!cnf_tile_counter)
            {
                /* Report error, this counter should not be zero! */
                cerr << __func__ << "(" << ind << ") tries to decrease overall counter but it is zero already!" << endl;
            }
            else
            {
                cnf_tile_counter--;
            }

            CnfTile tile = cnf_tile_vector.at(ind).tile;
            for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
                for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
                    freeCnfChar(tile.char_idxs.at(row).at(col));
        }
    }

    cnf_tile_vector[ind].usage = usage;
}

/*******************************************************************************
****
****    S P R I T E
****
*******************************************************************************/

qint16 Scenery::createSprite(Sprite sprite)
{
    return -1;
}

qint16 Scenery::findSprite(Sprite sprite)
{
    return -1;
}

void Scenery::reserveSprite(quint8 ind)
{
    sprite_vector[ind].reserved = true;
}

void Scenery::modifySprite(quint8 ind, Sprite sprite)
{
    sprite_vector[ind].sprite = sprite;
}

void Scenery::useSprite(quint8 ind)
{

}

void Scenery::freeSprite(quint8 ind)
{

}

/*******************************************************************************
****
****    W A L L
****
*******************************************************************************/

qint16 Scenery::createWall(Wall wall)
{
    return -1;
}

qint16 Scenery::findWall(Wall wall)
{
    return -1;
}

void Scenery::reserveWall(quint8 ind)
{
    wall_vector[ind].reserved = true;
}

void Scenery::modifyWall(quint8 ind, Wall wall)
{
    wall_vector[ind].wall = wall;
}

void Scenery::useWall(quint8 ind)
{

}

void Scenery::freeWall(quint8 ind)
{

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
