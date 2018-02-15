
#include "map_general.h"

Map::Map():
    ceiling_idxs(SCENERY_MAP_SECTORS), wall_idxs(SCENERY_MAP_SECTORS), floor_idxs(SCENERY_MAP_SECTORS),
    ceiling_clrs(SCENERY_MAP_SECTORS), wall_clrs(SCENERY_MAP_SECTORS), floor_clrs(SCENERY_MAP_SECTORS),
    block_c_idxs(SCENERY_MAP_BLOCKS), block_f_idxs(SCENERY_MAP_BLOCKS),
    block_0_idxs(SCENERY_MAP_BLOCKS), block_1_idxs(SCENERY_MAP_BLOCKS),
    block_2_idxs(SCENERY_MAP_BLOCKS), block_3_idxs(SCENERY_MAP_BLOCKS),
    scenery_index(-1), theme_index(-1), human_index(-1)
{
    loadDefaultValues();
}

Map::~Map(){}

void Map::loadDefaultValues()
{
    VE::loadDefaultValues();
    name = "Default Map";

    for(int i=0; i<SCENERY_MAP_SECTORS; ceiling_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    wall_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   floor_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS; ceiling_clrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    wall_clrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   floor_clrs[i]=0, i++);

    for(int i=0; i<SCENERY_MAP_BLOCKS; block_c_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_f_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_0_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_1_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_2_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_3_idxs[i]=0, i++);

    scenery_index = 0;
    theme_index = 0;
    human_index = 0;
    cyber_index = 0;
    music_index = 0;

    human_comm0_index = 0;
    human_comm1_index = 1;
    cyber_comm0_index = 2;
    cyber_comm1_index = 3;
}

Map &Map::operator=(const Map& other)
{
    VE::operator =((const VE&)other);

    for(int i=0; i<SCENERY_MAP_SECTORS; ceiling_idxs[i]=other.ceiling_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    wall_idxs[i]=other.wall_idxs[i],    i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   floor_idxs[i]=other.floor_idxs[i],   i++);
    for(int i=0; i<SCENERY_MAP_SECTORS; ceiling_clrs[i]=other.ceiling_clrs[i], i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    wall_clrs[i]=other.wall_clrs[i],    i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   floor_clrs[i]=other.floor_clrs[i],   i++);

    for(int i=0; i<SCENERY_MAP_BLOCKS; block_c_idxs[i]=other.block_c_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_f_idxs[i]=other.block_f_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_0_idxs[i]=other.block_0_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_1_idxs[i]=other.block_1_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_2_idxs[i]=other.block_2_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_3_idxs[i]=other.block_3_idxs[i], i++);

    ceiling_idxs  = other.ceiling_idxs;
    wall_idxs     = other.wall_idxs;
    floor_idxs    = other.floor_idxs;
    ceiling_clrs  = other.ceiling_clrs;
    wall_clrs     = other.wall_clrs;
    floor_clrs    = other.floor_clrs;
    block_c_idxs  = other.block_c_idxs;
    block_f_idxs  = other.block_f_idxs;
    block_0_idxs  = other.block_0_idxs;
    block_1_idxs  = other.block_1_idxs;
    block_2_idxs  = other.block_2_idxs;
    block_3_idxs  = other.block_3_idxs;

    scenery_index = other.scenery_index;
    theme_index   = other.theme_index;
    human_index   = other.human_index;
    cyber_index   = other.cyber_index;
    music_index   = other.music_index;

    human_comm0_index = other.human_comm0_index;
    human_comm1_index = other.human_comm1_index;
    cyber_comm0_index = other.cyber_comm0_index;
    cyber_comm1_index = other.cyber_comm1_index;

    return *this;
}

Map *Map::copy()
{
    Map *ret = new Map();
    *ret = *this;
    return ret;
}
