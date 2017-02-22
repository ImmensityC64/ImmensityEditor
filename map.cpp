
#include "map_general.h"

Map::Map():
    ceiling_idxs(SCENERY_MAP_SECTORS), wall_idxs(SCENERY_MAP_SECTORS), floor_idxs(SCENERY_MAP_SECTORS),
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
    name = "Default Map";

    for(int i=0; i<SCENERY_MAP_SECTORS; ceiling_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    wall_idxs[i]=0, i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   floor_idxs[i]=0, i++);

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

Map *Map::copy()
{
    Map *ret = new Map();

    for(int i=0; i<SCENERY_MAP_SECTORS; ret->ceiling_idxs[i]=ceiling_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;    ret->wall_idxs[i]=wall_idxs[i],    i++);
    for(int i=0; i<SCENERY_MAP_SECTORS;   ret->floor_idxs[i]=floor_idxs[i],   i++);

    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_c_idxs[i]=block_c_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_f_idxs[i]=block_f_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_0_idxs[i]=block_0_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_1_idxs[i]=block_1_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_2_idxs[i]=block_2_idxs[i], i++);
    for(int i=0; i<SCENERY_MAP_BLOCKS; ret->block_3_idxs[i]=block_3_idxs[i], i++);

    ret->ceiling_idxs  = ceiling_idxs;
    ret->wall_idxs     = wall_idxs;
    ret->floor_idxs    = floor_idxs;
    ret->block_c_idxs  = block_c_idxs;
    ret->block_f_idxs  = block_f_idxs;
    ret->block_0_idxs  = block_0_idxs;
    ret->block_1_idxs  = block_1_idxs;
    ret->block_2_idxs  = block_2_idxs;
    ret->block_3_idxs  = block_3_idxs;

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
