
#include "map_general.h"

Map::Map():
    ceiling_ptrs(SCENERY_MAP_SECTORS), wall_ptrs(SCENERY_MAP_SECTORS), floor_ptrs(SCENERY_MAP_SECTORS),
    block_c_ptrs(SCENERY_MAP_BLOCKS), block_f_ptrs(SCENERY_MAP_BLOCKS),
    block_0_ptrs(SCENERY_MAP_BLOCKS), block_1_ptrs(SCENERY_MAP_BLOCKS),
    block_2_ptrs(SCENERY_MAP_BLOCKS), block_3_ptrs(SCENERY_MAP_BLOCKS),
    scenery_index(-1), theme_index(-1), human_index(-1)
{
    loadDefaultValues();
}

Map::~Map(){}

void Map::loadDefaultValues()
{
    name = "Default Map";

    for(int i=0; i<SCENERY_MAP_SECTORS; ceiling_ptrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    wall_ptrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   floor_ptrs[i]=0, i++);

    for(int i=0; i<SCENERY_MAP_BLOCKS; block_c_ptrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_f_ptrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_0_ptrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_1_ptrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_2_ptrs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; block_3_ptrs[i]=0, i++);

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

Map *Map::copy()
{
    Map *ret = new Map();

    for(int i=0; i<SCENERY_MAP_SECTORS; ret->ceiling_ptrs[i]=ceiling_ptrs[i], i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    ret->wall_ptrs[i]=wall_ptrs[i],    i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   ret->floor_ptrs[i]=floor_ptrs[i],   i++);

    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_c_ptrs[i]=block_c_ptrs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_f_ptrs[i]=block_f_ptrs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_0_ptrs[i]=block_0_ptrs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_1_ptrs[i]=block_1_ptrs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_2_ptrs[i]=block_2_ptrs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_3_ptrs[i]=block_3_ptrs[i], i++);

    ret->ceiling_ptrs  = ceiling_ptrs;
    ret->wall_ptrs     = wall_ptrs;
    ret->floor_ptrs    = floor_ptrs;
    ret->block_c_ptrs  = block_c_ptrs;
    ret->block_f_ptrs  = block_f_ptrs;
    ret->block_0_ptrs  = block_0_ptrs;
    ret->block_1_ptrs  = block_1_ptrs;
    ret->block_2_ptrs  = block_2_ptrs;
    ret->block_3_ptrs  = block_3_ptrs;

    ret->scenery_index = scenery_index;
    ret->theme_index   = theme_index;
    ret->human_index   = human_index;
    ret->cyber_index   = cyber_index;
    ret->music_index   = music_index;

    ret->human_comm0_index = human_comm0_index;
    ret->human_comm1_index = human_comm1_index;
    ret->cyber_comm0_index = cyber_comm0_index;
    ret->cyber_comm1_index = cyber_comm1_index;

    return ret;
}
