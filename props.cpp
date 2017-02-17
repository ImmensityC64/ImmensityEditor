
#include "map_general.h"

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
