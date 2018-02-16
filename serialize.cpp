
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

    return in;
}

QDataStream& operator <<(QDataStream& out, Map const &src)
{
    out << src.name;

    out << src.ceiling_idxs;
    out << src.wall_idxs;
    out << src.floor_idxs;

    out << src.ceiling_clrs;
    out << src.wall_clrs;
    out << src.floor_clrs;

    out << src.block_c_idxs;
    out << src.block_f_idxs;
    out << src.block_0_idxs;
    out << src.block_1_idxs;
    out << src.block_2_idxs;
    out << src.block_3_idxs;

    out << src.scenery_index;
    out << src.theme_index;
    out << src.human_index;
    out << src.cyber_index;
    out << src.music_index;

    out << src.human_comm0_index;
    out << src.human_comm1_index;
    out << src.cyber_comm0_index;
    out << src.cyber_comm1_index;

    return out;
}

QDataStream& operator >>(QDataStream& in, Map &dst)
{
    in >> dst.name;

    in >> dst.ceiling_idxs;
    in >> dst.wall_idxs;
    in >> dst.floor_idxs;

    in >> dst.ceiling_clrs;
    in >> dst.wall_clrs;
    in >> dst.floor_clrs;

    in >> dst.block_c_idxs;
    in >> dst.block_f_idxs;
    in >> dst.block_0_idxs;
    in >> dst.block_1_idxs;
    in >> dst.block_2_idxs;
    in >> dst.block_3_idxs;

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

QDataStream& operator <<(QDataStream& out, Scenery &src)
{
    out << src.name;
    src.serOutCharSets(out);
    src.serOutCnfTiles(out);
    src.serOutBgTiles(out);
    src.serOutSprites(out);
    src.serOutWalls(out);
    return out;
}

QDataStream& operator >>(QDataStream& in, Scenery &dst)
{
    in >> dst.name;
    dst.serInCharSets(in);
    dst.serInCnfTiles(in);
    dst.serInBgTiles(in);
    dst.serInSprites(in);
    dst.serInWalls(in);
    return in;
}

QDataStream& Scenery::serOutCharSets(QDataStream& out, SerializeFor mode)
{
    calculateRealCharIndexes();
    for(int mapped_index=0; mapped_index<SCENERY_CHR_NUM; mapped_index++)
    {
        quint64 chr_data = 0;
        /* loop through char mapping vector */
        for(int index=0; index<SCENERY_CHR_NUM; index++)
        {
            if(realCharIndex(index) == mapped_index)
            {
                chr_data = chr_vector.at(index).chr;
                break;
            }
        }
        out << chr_data;
    }
}

QDataStream& Scenery::serInCharSets(QDataStream &in)
{
    //resetRealCharIndexes(); /* I think it is not needed here... */
    for(int i=0; i<SCENERY_CHR_NUM; i++)
    {
        chr_container &chr_c = chr_vector[i];
        in >> chr_c.chr;
        chr_c.usage     = 0;
        chr_c.cnf_usage = 0;
        chr_c.mapping   = i;
        chr_c.reserved  = false;
    }
}

QDataStream& Scenery::serOutCnfTiles(QDataStream& out, SerializeFor mode)
{
    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        const CnfTile &tile = cnf_tile_vector.at(i).tile;
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
        {
            quint8 chr_idx = realCharIndex(tile.char_idxs.at(row).at(col));
            chr_idx |= tile.colors.at(row).at(col);
            out << chr_idx;
        }
        if (mode != Export) /* do not export 'keep' value */
            out << cnf_tile_vector.at(i).keep;
    }
}

QDataStream& Scenery::serInCnfTiles(QDataStream &in)
{
    for(int i=0; i<SCENERY_CNF_TILE_NUM; i++)
    {
        cnf_tile_container &tile_c = cnf_tile_vector[i];
        CnfTile &tile = tile_c.tile;
        for(int row=0; row<SCENERY_CNF_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_CNF_TILE_COLS; col++)
        {
            quint8 chr_idx;
            in >> chr_idx;
            tile.char_idxs[row][col] = chr_idx & 0x3F;
            tile.colors[row][col] = chr_idx & 0xC0;
        }
        in >> tile_c.keep;
        tile_c.usage = 0;
        tile_c.reserved = false;
    }
}

QDataStream& Scenery::serOutBgTiles(QDataStream& out, SerializeFor mode)
{
    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        const BgTile &tile = bg_tile_vector.at(i).tile;
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
            out << realCharIndex(tile.char_idxs.at(row).at(col));
        if (mode != Export) /* colors are stored separately in C64's memory */
        {
            for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                out << tile.colors.at(row).at(col);
            out << bg_tile_vector.at(i).keep;
        }
    }

    if (mode == Export) /* colors are stored separately in C64's memory */
    {
        for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
        {
            const BgTile &tile = bg_tile_vector.at(i).tile;
            for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
            for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
                out << tile.colors.at(row).at(col);
            /* do not export 'keep' value */
        }
    }
}

QDataStream& Scenery::serInBgTiles(QDataStream &in)
{
    for(int i=0; i<SCENERY_BG_TILE_NUM; i++)
    {
        bg_tile_container &tile_c = bg_tile_vector[i];
        BgTile &tile = tile_c.tile;
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
            in >> tile.char_idxs[row][col];
        for(int row=0; row<SCENERY_BG_TILE_ROWS; row++)
        for(int col=0; col<SCENERY_BG_TILE_COLS; col++)
            in >> tile.colors[row][col];
        in >> tile_c.keep;
        tile_c.usage = 0;
        tile_c.reserved = false;
    }
}

QDataStream& Scenery::serOutSprites(QDataStream& out, SerializeFor mode)
{
    for(int i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        const Sprite &sprite = sprite_vector.at(i).sprite;

        quint8 byte=0;
        for(int row=0; row<(int)C64::SpriteHeight; row++)
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
        if (mode == Export)
            out << quint8(0); /* 64th byte */
            /* do not export 'keep' value */
        else
            out << sprite_vector.at(i).keep;
    }
}

QDataStream& Scenery::serInSprites(QDataStream &in)
{
    for(int i=0; i<SCENERY_SPRITE_NUM; i++)
    {
        sprite_container &sprite_c = sprite_vector[i];
        Sprite &sprite = sprite_c.sprite;
        for(int row=0; row<(int)C64::SpriteHeight; row++)
        for(int col=0; col<(int)C64::SpriteWidth; col++)
        {
            quint8 byte;
            int bit = col%8;
            if( 0==bit ) in >> byte;
            sprite.sprite_bits[row].setBit(col, byte & (1<<bit) );
        }
        in >> sprite_c.keep;
        sprite_c.usage = 0;
        sprite_c.reserved = false;
    }
}

QDataStream& Scenery::serOutWalls(QDataStream& out, SerializeFor mode)
{
    for(int i=0; i<SCENERY_WALL_NUM; i++)
    {
        const Wall &wall = wall_vector.at(i).wall;
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            out << wall.sprite_idxs.at(row);
        if (mode != Export) /* do not export 'keep' value */
            out << wall_vector.at(i).keep;
    }
}

QDataStream& Scenery::serInWalls(QDataStream &in)
{
    for(int i=0; i<SCENERY_WALL_NUM; i++)
    {
        wall_container &wall_c = wall_vector[i];
        Wall &wall = wall_c.wall;
        for(int row=0; row<SCENERY_WALL_ROWS; row++)
            in >> wall.sprite_idxs[row];
        in >> wall_c.keep;
        wall_c.usage = 0;
        wall_c.reserved = false;
    }
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
