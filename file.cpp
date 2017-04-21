
#include "mainwindow.h"

#define ENABLE_SAVE 1

void MainWindow::loadGfxData(void)
{
    if(projFile->open(QIODevice::ReadOnly))
    {
        QDataStream file(projFile);
        quint8 version;
        file >> version;

        switch (version)
        {
        case 1:
        default:
        {
            loadGfxVector(gvSketchesBgTiles,  file);
            loadGfxVector(gvSketchesCnfTiles, file);
            loadGfxVector(gvSketchesCeilings, file);
            loadGfxVector(gvSketchesFloors,   file);
            loadGfxVector(gvSketchesWalls,    file);
            loadSceneryVector(file); /* it must precede loadMapVector() */
            loadThemeVector(file);   /* it must precede loadMapVector() */
            loadMapVector(file);
            break;
        }
        } /* switch */

        projFile->close();
    }
    else
    {
        cerr << "Could not open data file: " << projFile->fileName().toUtf8().constData() << endl;
        cerr << projFile->errorString().toUtf8().constData() << endl;
        resetGfxData(); /* create an empty one with default values */
    }
}

void MainWindow::resetGfxData(void)
{
    gvSketchesBgTiles.clear();
    gvSketchesCnfTiles.clear();
    gvSketchesCeilings.clear();
    gvSketchesFloors.clear();
    gvSketchesWalls.clear();

    while(!props.sceneries.isEmpty()) delete props.sceneries.takeLast();
    props.sceneries.append(new Scenery());

    while(!props.themes.isEmpty()) delete props.themes.takeLast();
    props.themes.append(new Theme());

    while(!props.maps.isEmpty()) delete props.maps.takeLast();
    props.maps.append(new Map());
}

void MainWindow::saveGfxData(void)
{
#if ENABLE_SAVE
    if(projFile->open(QIODevice::WriteOnly))
    {
        QDataStream file(projFile);
        file << quint8(1); /* version */

        saveGfxVector(gvSketchesBgTiles,  file);
        saveGfxVector(gvSketchesCnfTiles, file);
        saveGfxVector(gvSketchesCeilings, file);
        saveGfxVector(gvSketchesFloors,   file);
        saveGfxVector(gvSketchesWalls,    file);
        saveSceneryVector(file); /* it must precede saveMapVector() */
        saveThemeVector(file);   /* it must precede saveMapVector() */
        saveMapVector(file);

        projFile->close();
    }
    else cerr << projFile->errorString().toUtf8().constData() << endl;
#else
    cerr << "Data saving is disabled!" << endl;
#endif
}

void MainWindow::loadGfxVector(GfxVector &gv, QDataStream &in)
{
    quint32 size;
    in >> size;
    gv.clear();
    for(quint32 i=0; i<size; i++)
    {
        shared_ptr<GfxData> data(new GfxData());
        in >> data;
        gv.setDataAt((int)GfxData::Id::Append, data);
    }
}

void MainWindow::saveGfxVector(GfxVector &gv, QDataStream &out)
{
    quint32 size = gv.size();
    out << size;
    for(quint32 i=0; i<size; i++)
    {
        out << gv.dataAt((int)i);
    }
}

void MainWindow::loadSceneryVector(QDataStream &in)
{
    for(int i=0; i<props.sceneries.size(); i++)
        delete props.sceneries.at(i);

    props.sceneries.clear();

    quint32 size;
    in >> size;

    for(int i=0; i<(int)size; i++)
    {
        Scenery *scenery = new Scenery();
        scenery->mapping = i;
        in >> *scenery;
        props.sceneries.append(scenery);
    }

    if(!size)
    {
        /* Create a default scenery if there is none */
        Scenery *scenery = new Scenery();
        props.sceneries.append(scenery);
    }
}

void MainWindow::saveSceneryVector(QDataStream &out)
{
    int size = props.sceneries.size();

    out << quint32(size);

    for(int i=0; i<size; i++)
        out << *props.sceneries.at(i);
}

void MainWindow::loadThemeVector(QDataStream &in)
{
    for(int i=0; i<props.themes.size(); i++)
        delete props.themes.at(i);

    props.themes.clear();

    quint32 size;
    in >> size;

    for(int i=0; i<(int)size; i++)
    {
        Theme *theme = new Theme();
        theme->mapping = i;
        in >> *theme;
        props.themes.append(theme);
    }

    if(!size)
    {
        /* Create a default theme if there is none */
        Theme *theme = new Theme();
        props.themes.append(theme);
    }
}

void MainWindow::saveThemeVector(QDataStream &out)
{
    int size = props.themes.size();

    out << quint32(size);

    for(int i=0; i<size; i++)
        out << *props.themes.at(i);
}

void MainWindow::loadMapVector(QDataStream &in)
{
    for(int i=0; i<props.maps.size(); i++)
        delete props.maps.at(i);

    props.maps.clear();

    quint32 size;
    in >> size;

    for(int i=0; i<(int)size; i++)
    {
        Map *map = new Map();
        in >> *map;
        props.maps.append(map);
        props.useSceneryInMap(map->scenery_index,i);
    }

    if(!size)
    {
        /* Create a default map if there is none */
        Map *map = new Map();
        props.maps.append(map);
        props.useSceneryInMap(0,0);
    }
}

void MainWindow::saveMapVector(QDataStream &out)
{
    int size = props.maps.size();

    out << quint32(size);

    for(int i=0; i<size; i++)
        out << *props.maps.at(i);
}
