
#include "mainwindow.h"

/* serialize GfxData pointed by the given shared_ptr and send out to the stream */
QDataStream& operator <<(QDataStream& out, shared_ptr<GfxData> const src)
{
    /* save type, id and size */
    out <<  (quint8)src->T;
    out << (quint16)src->W;
    out << (quint16)src->H;

    /* save colors */
    for(int i=0; i<(int)GfxData::ColorIndex::Size; i++) { out << (quint8)src->colors[i]; }

    /* save bitmap */
    for(typename QVector<QBitArray*>::const_iterator
        it  = src->bitmap->begin();
        it != src->bitmap->end();
        ++it)
    { out << **it; }

    /* save clrmap */
    for(typename QVector<QVector<quint8>*>::const_iterator
        it  = src->clrmap->begin();
        it != src->clrmap->end();
        ++it)
    { out << **it; }

    return out;
}

/* read the stream and reconstruct the content of GfxData pointed by the given shared_ptr */
QDataStream& operator >>(QDataStream& in, shared_ptr<GfxData> dst)
{
    dst->resize(0,0); /* clear old data */

    /* load type, id and size */
    quint8  data_type;
    quint16 width, height;
    in >> data_type;
    in >> width;
    in >> height;
    dst->setType((GfxData::Type)data_type); /* also sets color mode */
    dst->W = (int) width;
    dst->H = (int) height;

    /* load colors */
    for(int i=0; i<(int)GfxData::ColorIndex::Size; i++) { in >> (quint8 &)dst->colors[i]; }

    /* load bitmap */
    dst->bitmap->resize(height);
    for(int i=0; i<height; ++i)
    {
        QBitArray *array = new QBitArray();
        in >> *array;
        dst->bitmap->data()[i] = array;
    }

    /* load clrmap */
    dst->clrmap->resize(dst->clrH());
    for(int i=0; i<dst->clrH(); ++i)
    {
        QVector<quint8> *array = new QVector<quint8>();
        in >> *array;
        dst->clrmap->data()[i] = array;
    }

    dst->calcCrc();

    return in;
}

QDataStream& operator <<(QDataStream& out, Map const &src)
{
    out << src.name;

    out << src.ceiling_ptrs;
    out << src.wall_ptrs;
    out << src.floor_ptrs;

    out << src.block_c_ptrs;
    out << src.block_f_ptrs;
    out << src.block_0_ptrs;
    out << src.block_1_ptrs;
    out << src.block_2_ptrs;
    out << src.block_3_ptrs;

    out << src.human_comm0_index;
    out << src.human_comm1_index;
    out << src.cyber_comm0_index;
    out << src.cyber_comm1_index;

    return out;
}

QDataStream& operator >>(QDataStream& in, Map &dst)
{
    in >> dst.name;

    in >> dst.ceiling_ptrs;
    in >> dst.wall_ptrs;
    in >> dst.floor_ptrs;

    in >> dst.block_c_ptrs;
    in >> dst.block_f_ptrs;
    in >> dst.block_0_ptrs;
    in >> dst.block_1_ptrs;
    in >> dst.block_2_ptrs;
    in >> dst.block_3_ptrs;

    in >> dst.scenery_index;
    in >> dst.theme_index;
    in >> dst.human_index;
    in >> dst.cyber_index;
    in >> dst.music_index;

    in >> dst.human_comm0_index;
    in >> dst.human_comm1_index;
    in >> dst.cyber_comm0_index;
    in >> dst.cyber_comm1_index;

    return in;
}

QDataStream& operator <<(QDataStream& out, Scenery const &src)
{
    out << src.name;

    /* Character Set */

    //src.calculateRealCharIndexes();

    /* loop through character SET */
    for(int set=0; set<SCENERY_CHR_NUM; set++)
    {
        quint64 chr_data = 0;
        /* loop through char MAPping vector */
        for(int map=0; map<SCENERY_CHR_NUM; map++)
        {
            if(src.realCharIndex(map) == set)
            {
                chr_data = src.chr_vector.at(map).chr;
                break;
            }
        }
        out << chr_data;
    }

    /* C&F Tiles */
    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        const CnfTile &tile = src.cnf_tile_vector.at(i).tile;
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
                out << src.realCharIndex(tile.char_ptrs.at(row).at(col));
        out << src.cnf_tile_vector.at(i).keep;
    }

    /* BG Tiles */
    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        const BgTile &tile = src.bg_tile_vector.at(i).tile;
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                out << src.realCharIndex(tile.char_ptrs.at(row).at(col));
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                out << tile.colors.at(row).at(col);
        out << src.bg_tile_vector.at(i).keep;
    }

    /* Sprites */
    for(int i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        const Sprite &sprite = src.sprite_vector.at(i).sprite;

        quint8 byte=0;
        for(int row=0; row<(int)C64::SpriteHeight; row++)
        {
            for(int col=0; col<(int)C64::SpriteWidth; col++)
            {
                int bit = col%8;
                byte |= sprite.sprite_bits.at(row).at(col) << bit ;
                if( 7==bit )
                {
                    out << byte;
                    byte = 0;
                }
            }
        }
        out << src.sprite_vector.at(i).keep;
    }

    /* Walls */
    for(int i=0; i<SCENERY_WALL_NUM; i++)
    {
        const Wall &wall = src.wall_vector.at(i).wall;
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            out << wall.sprite_ptrs.at(row);
        out << src.wall_vector.at(i).keep;
    }

    return out;
}

QDataStream& operator >>(QDataStream& in, Scenery &dst)
{
    in >> dst.name;

    /* Character Set */

    //dst.resetRealCharIndexes();

    /* loop through character SET */
    for(int set=0; set<SCENERY_CHR_NUM; set++)
    {
        in >> dst.chr_vector[set].chr;
    }

    /* C&F Tiles */
    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        CnfTile &tile = dst.cnf_tile_vector[i].tile;
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
                in >> tile.char_ptrs[row][col];
        in >> dst.cnf_tile_vector[i].keep;
    }

    /* BG Tiles */
    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        BgTile &tile = dst.bg_tile_vector[i].tile;
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                in >> tile.char_ptrs[row][col];
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                in >> tile.colors[row][col];
        in >> dst.bg_tile_vector[i].keep;
    }

    /* Sprites */
    for(int i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        Sprite &sprite = dst.sprite_vector[i].sprite;

        quint8 byte;
        for(int row=0; row<(int)C64::SpriteHeight; row++)
        {
            for(int col=0; col<(int)C64::SpriteWidth; col++)
            {
                int bit = col%8;
                if( 0==bit ) in >> byte;
                sprite.sprite_bits[row].setBit(col, byte & (1<<bit) );
            }
        }
        in >> dst.sprite_vector[i].keep;
    }

    /* Walls */
    for(int i=0; i<SCENERY_WALL_NUM; i++)
    {
        Wall &wall = dst.wall_vector[i].wall;
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            in >> wall.sprite_ptrs[row];
        in >> dst.wall_vector[i].keep;
    }

    return in;
}

QDataStream& operator <<(QDataStream& out, Theme const &src)
{
    out << src.name;

    for(int i=0; i<(int)Theme::Enum::Size; i++)
        out << src.colors.at(i);

    return out;
}

QDataStream& operator >>(QDataStream& in, Theme &dst)
{
    in >> dst.name;

    for(int i=0; i<(int)Theme::Enum::Size; i++)
        in >> dst.colors[i];

    return in;
}
