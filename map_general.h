#ifndef MAP_GENERAL_H
#define MAP_GENERAL_H

#include <gfx_data.h>
#include <Qt>
#include <QVector>
#include <QString>
#include <QMessageBox>

#define SCENERY_CHR_NUM 256
#define SCENERY_CNF_CHR_NUM 64
#define SCENERY_BG_TILE_NUM 256
#define SCENERY_CNF_TILE_NUM 42

#define SCENERY_BASIC_SPRITE_NUM 36
#define SCENERY_ENEMY_SPRITE_NUM 24*3 /* Top, Middle, Bottom */
#define SCENERY_SPRITE_NUM SCENERY_BASIC_SPRITE_NUM+SCENERY_ENEMY_SPRITE_NUM
#define SCENERY_WALL_NUM 51

#define SCENERY_MAP_BLOCKS 255
#define SCENERY_MAP_SECTORS 85
#define SCENERY_MAP_BLOCK_LAST SCENERY_MAP_BLOCKS-1
#define SCENERY_MAP_SECTOR_LAST SCENERY_MAP_SECTORS-1

#define SCENERY_BG_TILE_ROWS 2
#define SCENERY_BG_TILE_COLS 5
#define SCENERY_CNF_TILE_ROWS 3
#define SCENERY_CNF_TILE_COLS 6
#define SCENERY_WALL_ROWS 5

class VE { // Vector Element
public:
    enum Type
    {
        Map,
        Scenery,
        Theme,
        Human,
        Cyber,
        Enemy,
        Size
    };

    QString name = "";
    int mapping  = 0;
    explicit VE(){}
    virtual ~VE(){}
};

class Scenery;
class Map;
class MainWindow;

class BgTile {
public:
    QVector<QVector<quint8>> char_ptrs;
    QVector<QVector<quint8>> colors;
    explicit BgTile():
        char_ptrs(SCENERY_BG_TILE_ROWS,QVector<quint8>(SCENERY_BG_TILE_COLS)),
        colors(SCENERY_BG_TILE_ROWS,QVector<quint8>(SCENERY_BG_TILE_COLS)){}
    virtual ~BgTile(){}

    bool operator ==(BgTile &other) const
    {
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
            {
                if(char_ptrs.at(row).at(col) != other.char_ptrs.at(row).at(col)) return false;
                if(colors.at(row).at(col)    != other.colors.at(row).at(col))    return false;
            }
        return true;
    }
    BgTile& operator=(const BgTile& other)
    {
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
            {
                char_ptrs[row][col] = other.char_ptrs.at(row).at(col);
                colors[row][col]    = other.colors.at(row).at(col);
            }
        return *this;
    }

    friend QDataStream& operator <<(QDataStream& out, Scenery const &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
};

class CnfTile {
public:
    QVector<QVector<quint8>> char_ptrs;
    explicit CnfTile():
        char_ptrs(SCENERY_CNF_TILE_ROWS,QVector<quint8>(SCENERY_CNF_TILE_COLS)){}
    virtual ~CnfTile(){}

    bool operator ==(CnfTile &other) const
    {
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
                if(char_ptrs.at(row).at(col) != other.char_ptrs.at(row).at(col))
                    return false;
        return true;
    }
    CnfTile& operator=(const CnfTile& other)
    {
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
                char_ptrs[row][col] = other.char_ptrs.at(row).at(col);
        return *this;
    }

    friend QDataStream& operator <<(QDataStream& out, Scenery const &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
};

class Sprite {
public:
    QVector<QBitArray> sprite_bits;
    explicit Sprite():sprite_bits((int)C64::SpriteHeight,QBitArray((int)C64::SpriteWidth)){}
    virtual ~Sprite(){}
    bool operator ==(Sprite &other) const
    {
        for(int row=0; row<(int)C64::SpriteHeight; row++)
            for(int col=0; col<(int)C64::SpriteWidth; col++)
                if(sprite_bits.at(row).at(col) != other.sprite_bits.at(row).at(col))
                    return false;
        return true;
    }
    Sprite& operator=(const Sprite& other)
    {
        for(int row=0; row<(int)C64::SpriteHeight; row++)
            for(int col=0; col<(int)C64::SpriteWidth; col++)
                sprite_bits[row].setBit(col, other.sprite_bits.at(row).at(col));
        return *this;
    }

    friend QDataStream& operator <<(QDataStream& out, Scenery const &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
};

class Wall { /* Sprite Wall */
public:
    QVector<quint8> sprite_ptrs;
    explicit Wall():sprite_ptrs(SCENERY_WALL_ROWS){}
    virtual ~Wall(){}
    bool operator ==(Wall &other) const
    {
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            if(sprite_ptrs.at(row) != other.sprite_ptrs.at(row))
                return false;
        return true;
    }
    Wall& operator=(const Wall& other)
    {
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            sprite_ptrs[row] = other.sprite_ptrs.at(row);
        return *this;
    }

    friend QDataStream& operator <<(QDataStream& out, Scenery const &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
};

/* Chars should be remapped during C64 output file is generated.
 * I do not want handle an ordered list in the editor.
 * So, the used characters stored in 'QVector<quint64> chars' are
 * not in the right order. C&F chars, which are displayed in EMC,
 * should be moved to the first 64 position of the character table.
 */
struct chr_container {
    quint64 chr;            /* characters of scenery */
    quint32 usage     = 0;  /* follow how many times a char is used by any type of tiles */
    quint32 cnf_usage = 0;  /* follow how many times a char is used by C&F tiles */
    quint8 mapping;         /* used to remap char indexes (see comment above!) */
};

struct bg_tile_container {
    BgTile bg_tile;     /* BG tiles of scenery */
    quint32 usage = 0;  /* follow how many times a BG  tile is used by maps */
};

struct cnf_tile_container {
    CnfTile cnf_tile;   /* C&F tiles of scenery */
    quint32 usage = 0;  /* follow how many times a C&F tile is used by maps */
};

struct sprite_container {
    Sprite sprite;      /* sprites of scenery */
    quint32 usage = 0;  /* follow how many times a sprite is used by sprite walls */
};

struct wall_container {
    Wall wall;          /* sprite walls of scenery */
    quint32 usage = 0;  /* follow how many times a sprite wall is used by maps */
};

class Scenery : public VE {
public:
    QVector<chr_container> chr_vector;
    QVector<bg_tile_container> bg_tile_vector;
    QVector<cnf_tile_container> cnf_tile_vector;
    QVector<sprite_container> sprite_vector;
    QVector<wall_container> wall_vector;

    quint32 chr_counter;     /* follow overall char usage (0..255) */
    quint32 cnf_chr_counter; /* follow   C&F   char usage (0..63)  */
    quint32 bg_tile_counter;
    quint32 cnf_tile_counter;
    quint32 sprite_counter;
    quint32 wall_counter;

    explicit Scenery();
    virtual ~Scenery();
    void loadDefaultValues(void);

    friend QDataStream& operator <<(QDataStream& out, Scenery const &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
    Scenery *copy();

private:
    qint16 charInsert(quint64 chr, bool cnf);
public:
    qint16 findBgChar(quint64 chr);
    qint16 findCnfChar(quint64 chr);
    qint16 bgCharInsert(quint64 chr);
    qint16 cnfCharInsert(quint64 chr);
    void bgCharRemove(quint8 ind);
    void cnfCharRemove(quint8 ind);
    void charModify(quint8 ind, quint64 chr);

    qint16 findBgTile(BgTile tile);
    qint16 findCnfTile(CnfTile tile);
    qint16 bgTileInsert(BgTile tile);
    qint16 cnfTileInsert(CnfTile tile);
    void bgTileRemove(quint8 ind);
    void cnfTileRemove(quint8 ind);
    void bgTileModify(quint8 ind, BgTile tile);
    void cnfTileModify(quint8 ind, CnfTile tile);

    qint16 findSprite(Sprite sprite);
    qint16 spriteInsert(Sprite sprite);
    void spriteRemove(quint8 ind);
    void spriteModify(quint8 ind, Sprite sprite);

    qint16 findWall(Wall wall);
    qint16 wallInsert(Wall wall);
    void wallRemove(quint8 ind);
    void wallModify(quint8 ind, Wall wall);

    void calculateRealCharIndexes(void);
    void resetRealCharIndexes(void);
    quint8 realCharIndex(quint8 const index) const;
}; /* Scenery */

class Theme : public VE {
public:
    enum Enum {
        CeilingFg,
        CeilingFgECM0,
        CeilingFgECM1,
        CeilingFgECM2,
        CeilingFgECM3,
        Ceiling0,
        Ceiling1,
        Ceiling2,
        Ceiling3,
        Ceiling4,
        Ceiling5,
        Ceiling6,
        Background,
        BackgroundComm0,
        BackgroundComm1,
        Floor6,
        Floor5,
        Floor4,
        Floor3,
        Floor2,
        Floor1,
        Floor0,
        FloorFg,
        FloorFgECM0,
        FloorFgECM1,
        FloorFgECM2,
        FloorFgECM3,
        Size
    };

    QVector<quint8> colors;

    explicit Theme():colors((int)Enum::Size){loadDefaultValues();}
    virtual ~Theme(){}

    void loadDefaultValues(void)
    {
        name = "Default Theme";
        colors[(int)Enum::CeilingFg]       = (quint8)C64::IndexBlue;
        colors[(int)Enum::CeilingFgECM0]   = (quint8)C64::IndexLightGrey;
        colors[(int)Enum::CeilingFgECM1]   = (quint8)C64::IndexGrey;
        colors[(int)Enum::CeilingFgECM2]   = (quint8)C64::IndexLightBlue;
        colors[(int)Enum::CeilingFgECM3]   = (quint8)C64::IndexCyan;
        colors[(int)Enum::Ceiling0]        = (quint8)C64::IndexCyan;
        colors[(int)Enum::Ceiling1]        = (quint8)C64::IndexLightBlue;
        colors[(int)Enum::Ceiling2]        = (quint8)C64::IndexCyan;
        colors[(int)Enum::Ceiling3]        = (quint8)C64::IndexLightBlue;
        colors[(int)Enum::Ceiling4]        = (quint8)C64::IndexBlue;
        colors[(int)Enum::Ceiling5]        = (quint8)C64::IndexLightBlue;
        colors[(int)Enum::Ceiling6]        = (quint8)C64::IndexBlue;
        colors[(int)Enum::Background]      = (quint8)C64::IndexDarkGrey;
        colors[(int)Enum::BackgroundComm0] = (quint8)C64::IndexOrange;
        colors[(int)Enum::BackgroundComm1] = (quint8)C64::IndexLightBlue;
        colors[(int)Enum::Floor6]          = (quint8)C64::IndexBrown;
        colors[(int)Enum::Floor5]          = (quint8)C64::IndexRed;
        colors[(int)Enum::Floor4]          = (quint8)C64::IndexBrown;
        colors[(int)Enum::Floor3]          = (quint8)C64::IndexRed;
        colors[(int)Enum::Floor2]          = (quint8)C64::IndexOrange;
        colors[(int)Enum::Floor1]          = (quint8)C64::IndexRed;
        colors[(int)Enum::Floor0]          = (quint8)C64::IndexOrange;
        colors[(int)Enum::FloorFg]         = (quint8)C64::IndexBrown;
        colors[(int)Enum::FloorFgECM0]     = (quint8)C64::IndexGrey;
        colors[(int)Enum::FloorFgECM1]     = (quint8)C64::IndexDarkGrey;
        colors[(int)Enum::FloorFgECM2]     = (quint8)C64::IndexRed;
        colors[(int)Enum::FloorFgECM3]     = (quint8)C64::IndexOrange;
    }

    friend QDataStream& operator <<(QDataStream& out, Theme const &data);
    friend QDataStream& operator >>(QDataStream& in, Theme &data);

    Theme& operator=(const Theme& other)
    {
        for(int i=0; i<(int)Enum::Size; i++)
            colors[i] = other.colors.at(i);
        name = other.name;
        return *this;
    }
    Theme *copy()
    {
        Theme *ret = new Theme();
        *ret = *this;
        return ret;
    }
};

class Human : public VE {
public:
    explicit Human(){}
    virtual ~Human(){}
    Human *copy() { return this; } /* TODO */
};

class Cyber : public VE {
public:
    explicit Cyber(){}
    virtual ~Cyber(){}
    Cyber *copy() { return this; } /* TODO */
};

class Enemy : public VE {
public:
    explicit Enemy(){}
    virtual ~Enemy(){}
    Enemy *copy() { return this; } /* TODO */
};

class Music : public VE {
public:
    explicit Music(){}
    virtual ~Music(){}
};

class Map : public VE {
public:
    /* Sprite Pointers */
    QVector<quint8> ceiling_ptrs;
    QVector<quint8> wall_ptrs;
    QVector<quint8> floor_ptrs;

    /* Tile Pointers */
    QVector<quint8> block_c_ptrs;
    QVector<quint8> block_f_ptrs;
    QVector<quint8> block_0_ptrs;
    QVector<quint8> block_1_ptrs;
    QVector<quint8> block_2_ptrs;
    QVector<quint8> block_3_ptrs;

    int scenery_index;
    int   theme_index;
    int   human_index;
    int   cyber_index;
    int   music_index;

    quint8 human_comm0_index;
    quint8 human_comm1_index;
    quint8 cyber_comm0_index;
    quint8 cyber_comm1_index;

    explicit Map();
    virtual ~Map();
    void loadDefaultValues(void);

    friend QDataStream& operator <<(QDataStream& out, Map const &data);
    friend QDataStream& operator >>(QDataStream& in, Map &data);

    Map *copy();
};

class Props
{
public:
    explicit Props(){}
    ~Props(){}
    QVector<Scenery*> sceneries;
    QVector<Theme*>   themes;
    QVector<Human*>   humans;
    QVector<Cyber*>   cybers;
    QVector<Enemy*>   enemies;
    QVector<Music*>   musics;
    QVector<Map*>     maps;

    void sectorInc(int &sector)
    {
        if(SCENERY_MAP_SECTOR_LAST <= sector) sector = 0;
        else sector++;
    }

    void sectorDec(int &sector)
    {
        if(0 >= sector) sector = SCENERY_MAP_SECTOR_LAST;
        else sector--;
    }

    void blockInc(int &block)
    {
        if(SCENERY_MAP_BLOCK_LAST <= block) block = 0;
        else block++;
    }

    void blockDec(int &block)
    {
        if(0 >= block) block = SCENERY_MAP_BLOCK_LAST;
        else block--;
    }

    /* return index sector's center block */
    int sector2block(int sector) { return sector*3+1; }
    int sector2blockL(int sector) { return sector*3; }
    int sector2blockR(int sector) { return sector*3+2; }
    int block2sector(int block) { return block/3; }

    /* generate map editor's images from map data */
    shared_ptr<GfxData> genImgCeiling(int map_index, int sector);
    shared_ptr<GfxData> genImgFloor(int map_index, int sector);
    shared_ptr<GfxData> genImgBackground(int map_index, int sector);
    shared_ptr<GfxData> genImgBgTile(int map_index, int tile_index);
    shared_ptr<GfxData> genImgCnfTile(int map_index, int tile_index);

    /* generate map data from map editor's images */
    void genMapData(int map_index, int sector,
                    shared_ptr<GfxData> ceilingFg,
                    shared_ptr<GfxData> background,
                    shared_ptr<GfxData> floorFg);

    bool deleteScenery(int index)
    {
        bool ret=false;
        QString map_list = "";

        /* Verify that current scenery is not used by any map */
        for(int i=0; i<maps.size(); i++)
        {
            if(maps.at(i)->scenery_index == index)
                map_list += "- " + maps.at(i)->name + "\n";
        }

        if(map_list.isEmpty())
        {
            for(int i=0; i<maps.size(); i++)
            {
                if(index < maps.at(i)->scenery_index)
                    maps[i]->scenery_index--;
            }
            delete sceneries[index];
            sceneries.remove(index);
            ret=true;
        }
        else
        {
            /* Current scenery is used in some maps, it cannot be deleted */
            QMessageBox msgBox;
            msgBox.setText("Scenery is used by the following maps:\n"+map_list+"It cannot be deleted!");
            msgBox.exec();
        }

        return ret;
    }

    bool deleteTheme(int index)
    {
        bool ret=false;
        QString map_list = "";

        /* Verify that current theme is not used by any map */
        for(int i=0; i<maps.size(); i++)
        {
            if(maps.at(i)->theme_index == index)
                map_list += "- " + maps.at(i)->name + "\n";
            /* TODO: also verify scripts of the map */
        }

        if(map_list.isEmpty())
        {
            for(int i=0; i<maps.size(); i++)
            {
                if(index < maps.at(i)->theme_index)
                    maps[i]->theme_index--;
            }
            delete themes[index];
            themes.remove(index);
            ret=true;
        }
        else
        {
            /* Current theme is used in some maps, it cannot be deleted */
            QMessageBox msgBox;
            msgBox.setText("Theme is used by the following maps:\n"+map_list+"It cannot be deleted!");
            msgBox.exec();
        }

        return ret;
    }

    void deleteHuman(int index)
    {

    }

    void deleteCyber(int index)
    {

    }

    void deleteEnemy(int index)
    {
        /* TODO: reindex enemies in scripts */
    }

    void deleteMusic(int index)
    {

    }

    bool deleteMap(int index)
    {
        bool ret=false;

        /* TODO: verify if map is used in the project */

        if(maps.size() <= 1)
        {
            /* There is no other map, do not let the last one to be deleted! */
            maps[0]->loadDefaultValues();
        }
        else
        {
            /* TODO: reindex maps in project */
            delete maps[index];
            maps.remove(index);
            ret=true;
        }

        return ret;
    }

};

#endif // MAP_GENERAL_H
