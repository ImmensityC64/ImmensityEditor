#ifndef MAP_GENERAL_H
#define MAP_GENERAL_H

#include <gfx_data.h>
#include <modular_integer.h>
#include <Qt>
#include <QVector>
#include <QString>
#include <QMessageBox>

#define SCENERY_CHR_NUM 256
#define SCENERY_CNF_CHR_NUM 64
#define SCENERY_BG_TILE_NUM 128
#define SCENERY_CNF_TILE_NUM 42

#define SCENERY_BASIC_SPRITE_NUM 36
#define SCENERY_ENEMY_SPRITE_NUM 24*3 /* Top, Middle, Bottom */
#define SCENERY_SPRITE_NUM SCENERY_BASIC_SPRITE_NUM+SCENERY_ENEMY_SPRITE_NUM
#define SCENERY_WALL_NUM 51

#define SCENERY_MAP_SECTORS 85
#define SCENERY_MAP_BLOCKS SCENERY_MAP_SECTORS*3 /* Left, Center, Right */
#define SCENERY_MAP_BLOCK_LAST SCENERY_MAP_BLOCKS-1
#define SCENERY_MAP_SECTOR_LAST SCENERY_MAP_SECTORS-1

#define SCENERY_BG_TILE_ROWS 2
#define SCENERY_BG_TILE_COLS 5
#define SCENERY_CNF_TILE_ROWS 3
#define SCENERY_CNF_TILE_COLS 6
#define SCENERY_WALL_ROWS 5

#define indexEditSectorCeiling    -1
#define indexEditSectorBackground -2
#define indexEditSectorFloor      -3

typedef modint<quint8, SCENERY_MAP_SECTORS> sector_modint;
typedef modint<quint8, SCENERY_MAP_BLOCKS>  block_modint;

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
    QVector<QVector<quint8>> char_idxs;
    QVector<QVector<quint8>> colors;
    explicit BgTile():
        char_idxs(SCENERY_BG_TILE_ROWS,QVector<quint8>(SCENERY_BG_TILE_COLS, 0)),
        colors(SCENERY_BG_TILE_ROWS,QVector<quint8>(SCENERY_BG_TILE_COLS, 0)){}
    virtual ~BgTile(){}

    bool operator ==(BgTile& other) const
    {
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
        {
            if(char_idxs.at(row).at(col) != other.char_idxs.at(row).at(col)) return false;
            if(colors.at(row).at(col)    != other.colors.at(row).at(col))    return false;
        }
        return true;
    }
    bool operator !=(BgTile& other) const { return !(operator ==(other)); }
    BgTile& operator=(const BgTile& other)
    {
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
        {
            char_idxs[row][col] = other.char_idxs.at(row).at(col);
            colors[row][col]    = other.colors.at(row).at(col);
        }
        return *this;
    }

    friend QDataStream& operator <<(QDataStream& out, Scenery const &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
};

class CnfTile {
public:
    QVector<QVector<quint8>> char_idxs;
    QVector<QVector<quint8>> colors;
    explicit CnfTile():
        char_idxs(SCENERY_CNF_TILE_ROWS,QVector<quint8>(SCENERY_CNF_TILE_COLS, 0)),
        colors(SCENERY_CNF_TILE_ROWS,QVector<quint8>(SCENERY_CNF_TILE_COLS, 0)){}
    virtual ~CnfTile(){}

    bool operator ==(CnfTile& other) const
    {
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
        {
            if(char_idxs.at(row).at(col) != other.char_idxs.at(row).at(col)) return false;
            if(colors.at(row).at(col)    != other.colors.at(row).at(col))    return false;
        }
        return true;
    }
    bool operator !=(CnfTile& other) const { return !(operator ==(other)); }
    CnfTile& operator=(const CnfTile& other)
    {
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
        {
            char_idxs[row][col] = other.char_idxs.at(row).at(col);
            colors[row][col]    = other.colors.at(row).at(col);
        }
        return *this;
    }

    friend QDataStream& operator <<(QDataStream& out, Scenery const &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
};

class Sprite {
public:
    QVector<QBitArray> sprite_bits;
    explicit Sprite():
        sprite_bits((int)C64::SpriteHeight,QBitArray((int)C64::SpriteWidth)){}
    explicit Sprite(shared_ptr<GfxData> img, int wall_row = 0):
        sprite_bits((int)C64::SpriteHeight,QBitArray((int)C64::SpriteWidth))
    {
        load_gfx_data(img, wall_row);
    }
    virtual ~Sprite(){}
    void load_gfx_data(shared_ptr<GfxData> img, int wall_row = 0)
    {
        int wr = wall_row*(int)C64::SpriteHeight;
        for(int row=0; row<(int)C64::SpriteHeight; row++)
        for(int col=0; col<(int)C64::SpriteWidth; col++)
            sprite_bits[row].setBit(col, img->bitVal(col,wr+row));
    }
    bool is_equal_to_gfx_data(shared_ptr<GfxData> img, int wall_row = 0)
    {
        int wr = wall_row*(int)C64::SpriteHeight;
        for(int row=0; row<(int)C64::SpriteHeight; row++)
        for(int col=0; col<(int)C64::SpriteWidth; col++)
            if(sprite_bits.at(row).at(col) != img->bitVal(col,wr+row))
                return false;
        return true;
    }
    shared_ptr<GfxData> gfx_data()
    {
        shared_ptr<GfxData> ret(new GfxData(GfxData::Type::Sprite));
        for(int row=0; row<(int)C64::SpriteHeight; row++)
        for(int col=0; col<(int)C64::SpriteWidth; col++)
            ret->setBitVal(col, row, sprite_bits.at(row).at(col));
        return ret;
    }
    bool operator ==(Sprite& other) const
    {
        for(int row=0; row<(int)C64::SpriteHeight; row++)
        for(int col=0; col<(int)C64::SpriteWidth; col++)
            if(sprite_bits.at(row).at(col) != other.sprite_bits.at(row).at(col))
                return false;
        return true;
    }
    bool operator !=(Sprite& other) const { return !(operator ==(other)); }
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
    QVector<quint8> sprite_idxs;
    explicit Wall():sprite_idxs(SCENERY_WALL_ROWS){}
    virtual ~Wall(){}
    bool operator ==(Wall& other) const
    {
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            if(sprite_idxs.at(row) != other.sprite_idxs.at(row))
                return false;
        return true;
    }
    bool operator !=(Wall& other) const { return !(operator ==(other)); }
    Wall& operator=(const Wall& other)
    {
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            sprite_idxs[row] = other.sprite_idxs.at(row);
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
    quint64 chr       = 0; /* character data */
    quint32 usage     = 0; /* follow how many times a char is used by any type of tiles */
    quint32 cnf_usage = 0; /* follow how many times a char is used by C&F tiles */
    quint8 mapping    = 0; /* used to remap char indexes (see comment above!) */
    bool reserved = false; /* it is going to be used */
};

struct bg_tile_container {
    BgTile tile;        /* tile data */
    quint32 usage = 0;  /* follow how many times a BG  tile is used by maps */
    bool keep = false;  /* keep for later use */
    bool reserved = false; /* it is going to be used */
};

struct cnf_tile_container {
    CnfTile tile;       /* tile data */
    quint32 usage = 0;  /* follow how many times a C&F tile is used by maps */
    bool keep = false;  /* keep for later use */
    bool reserved = false; /* it is going to be used */
};

struct sprite_container {
    Sprite sprite;      /* sprites data */
    quint32 usage = 0;  /* follow how many times a sprite is used by sprite walls */
    bool keep = false;  /* keep for later use */
    bool reserved = false; /* it is going to be used */
};

struct wall_container {
    Wall wall;          /* sprite wall data */
    quint32 usage = 0;  /* follow how many times a sprite wall is used by maps */
    bool keep = false;  /* keep for later use */
    bool reserved = false; /* it is going to be used */
};

class Scenery : public VE {
public:
    QVector<chr_container> chr_vector;
    QVector<bg_tile_container> bg_tile_vector;
    QVector<cnf_tile_container> cnf_tile_vector;
    QVector<sprite_container> sprite_vector;
    QVector<wall_container> wall_vector;

    /* follow overall gfx element usage in scenery */
    quint32 chr_counter;     /* (0..255) */
    quint32 cnf_chr_counter; /* ECM aka. C&F (0..63)  */
    quint32 bg_tile_counter;
    quint32 cnf_tile_counter;
    quint32 sprite_counter;
    quint32 wall_counter;

    explicit Scenery();
    virtual ~Scenery();
    void loadDefaultValues(void);
    void clearUsage(void);
    void clearReservations(void);

    friend QDataStream& operator <<(QDataStream& out, Scenery &data);
    friend QDataStream& operator >>(QDataStream& in, Scenery &data);
    Scenery& operator=(const Scenery& other);
    Scenery *copy();

    enum SerializeFor {
        Project,
        Export,
    };

    QDataStream& serOutSprites(QDataStream& out, SerializeFor ser = Project);
    QDataStream& serInSprites(QDataStream& in);

public:
    qint16 createChar(quint64 chr);
    qint16 findChar(quint64 chr);
    void reserveChar(quint8 ind);
    void modifyChar(quint8 ind, quint64 chr);
    void useBgChar(quint8 ind);
    void freeBgChar(quint8 ind);
    bool useCnfChar(quint8 ind);
    void freeCnfChar(quint8 ind);

    qint16 createBgTile(BgTile tile);
    qint16 findBgTile(BgTile tile);
    void reserveBgTile(quint8 ind);
    void modifyBgTile(quint8 ind, BgTile tile);
    void useBgTile(quint8 ind);
    void freeBgTile(quint8 ind);

    qint16 createCnfTile(CnfTile tile);
    qint16 findCnfTile(CnfTile tile);
    void reserveCnfTile(quint8 ind);
    void modifyCnfTile(quint8 ind, CnfTile tile);
    bool useCnfTile(quint8 ind);
    void freeCnfTile(quint8 ind);

    qint16 createSprite(Sprite sprite);
    qint16 findSprite(Sprite sprite);
    void reserveSprite(quint8 ind);
    void modifySprite(quint8 ind, Sprite sprite);
    void useSprite(quint8 ind);
    void freeSprite(quint8 ind);

    qint16 createWall(Wall wall);
    qint16 findWall(Wall wall);
    void reserveWall(quint8 ind);
    void modifyWall(quint8 ind, Wall wall);
    void useWall(quint8 ind);
    void freeWall(quint8 ind);

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
    QVector<quint8> ceiling_idxs;
    QVector<quint8> wall_idxs;
    QVector<quint8> floor_idxs;
    QVector<quint8> ceiling_clrs;
    QVector<quint8> wall_clrs;
    QVector<quint8> floor_clrs;

    /* Tile Pointers */
    QVector<quint8> block_c_idxs;
    QVector<quint8> block_f_idxs;
    QVector<quint8> block_0_idxs;
    QVector<quint8> block_1_idxs;
    QVector<quint8> block_2_idxs;
    QVector<quint8> block_3_idxs;

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
    /* Main editor's temporary scenery and map */
    Scenery editor_scenery;
    quint8 editor_ceiling_idx;
    quint8 editor_wall_idx;
    quint8 editor_floor_idx;
    quint8 editor_ceiling_clr;
    quint8 editor_wall_clr;
    quint8 editor_floor_clr;
    QVector<quint8> editor_block_c_idxs;
    QVector<quint8> editor_block_f_idxs;
    QVector<quint8> editor_block_0_idxs;
    QVector<quint8> editor_block_1_idxs;
    QVector<quint8> editor_block_2_idxs;
    QVector<quint8> editor_block_3_idxs;
    QBitArray editor_modified_bg_tiles;
    QBitArray editor_modified_cnf_tiles;
    QBitArray editor_modified_sprites;
    QBitArray editor_modified_walls;

    static Props& ins();

private:
    Props() :
        editor_ceiling_idx(0),
        editor_wall_idx(0),
        editor_floor_idx(0),
        editor_ceiling_clr(0),
        editor_wall_clr(0),
        editor_floor_clr(0),
        editor_block_c_idxs(3,0),
        editor_block_f_idxs(3,0),
        editor_block_0_idxs(3,0),
        editor_block_1_idxs(3,0),
        editor_block_2_idxs(3,0),
        editor_block_3_idxs(3,0),
        editor_modified_bg_tiles(SCENERY_BG_TILE_NUM, true),
        editor_modified_cnf_tiles(SCENERY_CNF_TILE_NUM, true),
        editor_modified_sprites(SCENERY_SPRITE_NUM, true),
        editor_modified_walls(SCENERY_WALL_NUM, true)
    {}
    Props(Props const&);          // Don't implement
    void operator=(Props const&); // Don't implement
    ~Props(){}

public:
    QVector<Scenery*> sceneries;
    QVector<Theme*>   themes;
    QVector<Human*>   humans;
    QVector<Cyber*>   cybers;
    QVector<Enemy*>   enemies;
    QVector<Music*>   musics;
    QVector<Map*>     maps;

    void useSceneryInMap(int scenery_index, int map_index);
    void freeSceneryInMap(int scenery_index, int map_index);

    bool deleteScenery(int index);
    bool deleteTheme(int index);
    void deleteHuman(int index);
    void deleteCyber(int index);
    void deleteEnemy(int index);
    void deleteMusic(int index);
    bool deleteMap(int index);

    /* return index sector's center block */
    int sector2blockL(sector_modint sector) { return sector.get()*3; }
    int sector2block (sector_modint sector) { return sector.get()*3+1; }
    int sector2blockR(sector_modint sector) { return sector.get()*3+2; }
    sector_modint block2sector(int block) { return sector_modint((int)(block/3)); }

    void all_tiles_need_refresh()
    {
        for(int i=0; i<SCENERY_BG_TILE_NUM;  i++) editor_modified_bg_tiles.setBit(i, true);
        for(int i=0; i<SCENERY_CNF_TILE_NUM; i++) editor_modified_cnf_tiles.setBit(i, true);
        for(int i=0; i<SCENERY_SPRITE_NUM;   i++) editor_modified_sprites.setBit(i, true);
        for(int i=0; i<SCENERY_WALL_NUM;     i++) editor_modified_walls.setBit(i, true);
    }

    /* generate map editor's images from map data */
    shared_ptr<GfxData> map2imgCeiling   (int map_index, sector_modint sector);
    shared_ptr<GfxData> map2imgFloor     (int map_index, sector_modint sector);
    shared_ptr<GfxData> map2imgBackground(int map_index, sector_modint sector);
    shared_ptr<GfxData> map2imgBgTile (int map_index, int tile_index);
    shared_ptr<GfxData> map2imgCnfTile(int map_index, int tile_index);
    shared_ptr<GfxData> map2imgSprite (int map_index, int sprite_index);
    shared_ptr<GfxData> map2imgWall   (int map_index, int wall_index);
    shared_ptr<GfxData> map2imgCharSet(int map_index);

    /* generate map data from map editor's images */
    bool img2mapCeiling   (shared_ptr<GfxData> img);
    bool img2mapBackground(shared_ptr<GfxData> img);
    bool img2mapFloor     (shared_ptr<GfxData> img);
    bool img2cnfTile(quint8 index, shared_ptr<GfxData> img);
    bool img2bgTile (quint8 index, shared_ptr<GfxData> img);
    bool img2mapSpriteCeiling(shared_ptr<GfxData> img);
    bool img2mapSpriteFloor  (shared_ptr<GfxData> img);
    bool img2mapWall(shared_ptr<GfxData> img);
    void img2sprite(quint8 index, shared_ptr<GfxData> img);
    bool img2wall(quint8 index, shared_ptr<GfxData> img);
private:
    bool img2mapCnf(shared_ptr<GfxData> img, QVector<quint8> *block_idxs);
    bool img2mapSpriteCnf(shared_ptr<GfxData> img, quint8 &idx, quint8 &clr);

}; /* Props */

#endif // MAP_GENERAL_H
