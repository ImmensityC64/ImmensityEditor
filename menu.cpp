
#include "mainwindow.h"

/* Returns true if a new browser has just been created.
 * Caller needs that information to decide whether a
 * signal connection should be established.
 */
bool MainWindow::openBrowser(GfxVector *gv)
{
    bool ret = false;

    /* Open a new browser */
    if(nullptr == gv->browser())
    {
        Browser *browser = new Browser(gv);
        browser->setAttribute(Qt::WA_DeleteOnClose, true);
        browser->show();
        gv->setBrowser(browser);
        ret = true;
    }
    /* ... or activate the already existing one */
    else gv->browser()->activateWindow();

    return ret;
}

void MainWindow::openSketchesCeilings()
{
    if( openBrowser(&gvSketchesCeilings) )
    {
        /* Establish signal connection if a new browser has just been created. */
        connect(gvSketchesCeilings.browser(), SIGNAL(destroyed()),
                this, SLOT(closeSketchesCeilings()));
    }
}
void MainWindow::openSketchesFloors()
{
    if( openBrowser(&gvSketchesFloors) )
    {
        /* Establish signal connection if a new browser has just been created. */
        connect(gvSketchesFloors.browser(), SIGNAL(destroyed()),
                this, SLOT(closeSketchesFloors()));
    }
}
void MainWindow::openSketchesWalls()
{
    if( openBrowser(&gvSketchesWalls) )
    {
        /* Establish signal connection if a new browser has just been created. */
        connect(gvSketchesWalls.browser(), SIGNAL(destroyed()),
                this, SLOT(closeSketchesWalls()));
    }
}
void MainWindow::openSketchesBgTiles()
{
    if( openBrowser(&gvSketchesBgTiles) )
    {
        /* Establish signal connection if a new browser has just been created. */
        connect(gvSketchesBgTiles.browser(), SIGNAL(destroyed()),
                this, SLOT(closeSketchesBgTiles()));
    }
}
void MainWindow::openSketchesCnfTiles()
{
    if( openBrowser(&gvSketchesCnfTiles) )
    {
        /* Establish signal connection if a new browser has just been created. */
        connect(gvSketchesCnfTiles.browser(), SIGNAL(destroyed()),
                this, SLOT(closeSketchesCnfTiles()));
    }
}
void MainWindow::openSketchesAll()
{
    openSketchesCeilings();
    openSketchesFloors();
    openSketchesWalls();
    openSketchesBgTiles();
    openSketchesCnfTiles();
}

void MainWindow::closeSketchesCeilings() { gvSketchesCeilings.setBrowser(nullptr); }
void MainWindow::closeSketchesFloors()   {   gvSketchesFloors.setBrowser(nullptr); }
void MainWindow::closeSketchesWalls()    {    gvSketchesWalls.setBrowser(nullptr); }
void MainWindow::closeSketchesBgTiles()  {  gvSketchesBgTiles.setBrowser(nullptr); }
void MainWindow::closeSketchesCnfTiles() { gvSketchesCnfTiles.setBrowser(nullptr); }
void MainWindow::closeSketchesAll()
{
    Browser *b;
    b=gvSketchesCeilings.browser(); if(b) b->close();
    b=gvSketchesFloors.browser();   if(b) b->close();
    b=gvSketchesWalls.browser();    if(b) b->close();
    b=gvSketchesBgTiles.browser();  if(b) b->close();
    b=gvSketchesCnfTiles.browser(); if(b) b->close();
}

void MainWindow::openSceneryCharSet()
{
    if(nullptr == charSetWindow)
    {
        CharSetW *w = new CharSetW(charSetData);
        w->setAttribute(Qt::WA_DeleteOnClose, true);
        w->show();
        charSetWindow = w;
        connect(charSetWindow, SIGNAL(destroyed()),
                this, SLOT(closeSceneryCharSet()));
    }
    /* ... or activate the already existing one */
    else charSetWindow->activateWindow();
}
void MainWindow::openSceneryBgTiles()
{
    if( openBrowser(&gvSceneryBgTiles) )
    {
        /* Establish signal connection if a new browser has just been created. */
        connect(gvSceneryBgTiles.browser(), SIGNAL(destroyed()),
                this, SLOT(closeSceneryBgTiles()));
    }
}
void MainWindow::openSceneryCnfTiles()
{
    if( openBrowser(&gvSceneryCnfTiles) )
    {
        /* Establish signal connection if a new browser has just been created. */
        connect(gvSceneryCnfTiles.browser(), SIGNAL(destroyed()),
                this, SLOT(closeSceneryCnfTiles()));
    }
}

void MainWindow::closeSceneryCharSet()  { charSetWindow = nullptr; }
void MainWindow::closeSceneryBgTiles()  { gvSceneryBgTiles.setBrowser(nullptr);  }
void MainWindow::closeSceneryCnfTiles() { gvSceneryCnfTiles.setBrowser(nullptr); }

void MainWindow::openEditSceneries()
{
    if(nullptr == sceneryEditor)
    {
        sceneryEditor = new SceneryEditor(props, 0);
        sceneryEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sceneryEditor->show();
        connect(sceneryEditor, SIGNAL(destroyed()),
                this, SLOT(closeSceneryEditor()));
        connect(sceneryEditor, SIGNAL(sceneryChanged(int)),
                this, SLOT(sceneryChanged(int)));
    }
    /* ... or activate the already existing one */
    else sceneryEditor->activateWindow();
}

void MainWindow::closeSceneryEditor() { sceneryEditor = nullptr; }

void MainWindow::openEditThemes()
{
    if(nullptr == themeEditor)
    {
        themeEditor = new ThemeEditor(props, 0);
        themeEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        themeEditor->show();
        connect(themeEditor, SIGNAL(destroyed()),
                this, SLOT(closeThemeEditor()));
        connect(themeEditor, SIGNAL(themeChanged(int)),
                this, SLOT(themeChanged(int)));
    }
    /* ... or activate the already existing one */
    else themeEditor->activateWindow();
}

void MainWindow::closeThemeEditor() { themeEditor = nullptr; }
