
#include "map_general.h"

void Props::useSceneryInMap(int scenery_index, int map_index)
{
    Scenery *s = sceneries.at(scenery_index);
    Map *m = maps.at(map_index);
    for(int i=0; i<SCENERY_MAP_BLOCKS; i++)
    {
        s->useCnfTile(m->block_c_ptrs.at(i));
        s->useCnfTile(m->block_f_ptrs.at(i));

        s->useBgTile(m->block_0_ptrs.at(i));
        s->useBgTile(m->block_1_ptrs.at(i));
        s->useBgTile(m->block_2_ptrs.at(i));
        s->useBgTile(m->block_3_ptrs.at(i));
    }
    for(int i=0; i<SCENERY_MAP_SECTORS; i++)
    {
        s->useSprite(m->ceiling_ptrs.at(i));
        s->useSprite(m->floor_ptrs.at(i));

        s->useWall(m->wall_ptrs.at(i));
    }
    /* TODO: also check scripts and animatied tiles */
}

void Props::freeSceneryInMap(int scenery_index, int map_index)
{
    Scenery *s = sceneries.at(scenery_index);
    Map *m = maps.at(map_index);
    for(int i=0; i<SCENERY_MAP_BLOCKS; i++)
    {
        s->freeCnfTile(m->block_c_ptrs.at(i));
        s->freeCnfTile(m->block_f_ptrs.at(i));

        s->freeBgTile(m->block_0_ptrs.at(i));
        s->freeBgTile(m->block_1_ptrs.at(i));
        s->freeBgTile(m->block_2_ptrs.at(i));
        s->freeBgTile(m->block_3_ptrs.at(i));
    }
    for(int i=0; i<SCENERY_MAP_SECTORS; i++)
    {
        s->freeSprite(m->ceiling_ptrs.at(i));
        s->freeSprite(m->floor_ptrs.at(i));

        s->freeWall(m->wall_ptrs.at(i));
    }
    /* TODO: also check scripts and animatied tiles */
}

bool Props::deleteScenery(int index)
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

bool Props::deleteTheme(int index)
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

void Props::deleteHuman(int index)
{

}

void Props::deleteCyber(int index)
{

}

void Props::deleteEnemy(int index)
{
    /* TODO: reindex enemies in scripts */
}

void Props::deleteMusic(int index)
{

}

bool Props::deleteMap(int index)
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
