
#include "mainwindow.h"

void MainWindow::openEditSceneries()
{
    if(nullptr == sceneryEditor)
    {
        sceneryEditor = new SceneryEditor(0);
        sceneryEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sceneryEditor->show();
        connect(sceneryEditor, SIGNAL(destroyed()),
                this, SLOT(closeEditSceneries()));
        connect(sceneryEditor, SIGNAL(sceneryChanged(int)),
                this, SLOT(sceneryChanged(int)));
    }
    /* ... or activate the already existing one */
    else sceneryEditor->activateWindow();
}

void MainWindow::closeEditSceneries() { sceneryEditor = nullptr; }

void MainWindow::openEditThemes()
{
    if(nullptr == themeEditor)
    {
        themeEditor = new ThemeEditor(0);
        themeEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        themeEditor->show();
        connect(themeEditor, SIGNAL(destroyed()),
                this, SLOT(closeEditThemes()));
        connect(themeEditor, SIGNAL(themeChanged(int)),
                this, SLOT(themeChanged(int)));
    }
    /* ... or activate the already existing one */
    else themeEditor->activateWindow();
}

void MainWindow::closeEditThemes() { themeEditor = nullptr; }

void MainWindow::openBrowseScenery()
{
    if(nullptr == browseScenery)
    {
        QWidget *w;
        Browser *b;
        QMainWindow *mw = new QMainWindow();
        mw->setWindowTitle("Browse Scenery");
        mw->setAttribute(Qt::WA_DeleteOnClose, true);
        w = new QWidget();
        QVBoxLayout *central_layout = new QVBoxLayout();
        central_layout->setContentsMargins(7,7,7,7);
        w->setLayout(central_layout);
        mw->setCentralWidget(w);

        /* Walls */
        b = new Browser(&gvSceneryWalls);
        central_layout->addWidget(b);
        QHBoxLayout *v_browsers_layout = new QHBoxLayout();
        v_browsers_layout->setContentsMargins(0,0,0,0);
        w = new QWidget();
        w->setLayout(v_browsers_layout);
        central_layout->addWidget(w);

        /* Sprites */
        b = new Browser(&gvScenerySprites);
        v_browsers_layout->addWidget(b);

        /* C&F Tiles */
        b = new Browser(&gvSceneryCnfTiles);
        v_browsers_layout->addWidget(b);

        /* Bg Tiles */
        b = new Browser(&gvSceneryBgTiles);
        v_browsers_layout->addWidget(b);

        /* CharSet */
        QVBoxLayout *info_layout = new QVBoxLayout();
        info_layout->setContentsMargins(0,0,0,0);
        w = new QWidget();
        w->setLayout(info_layout);
        v_browsers_layout->addWidget(w);
        charSet = new CharSet(props.map2imgCharSet(map_index));
        info_layout->addWidget(charSet);

        /* Info */
        /* TODO */

        mw->show();
        browseScenery = mw;
        connect(browseScenery, SIGNAL(destroyed()), this, SLOT(closeBrowseScenery()));
    }
    /* ... or activate the already existing one */
    else browseScenery->activateWindow();
}

void MainWindow::closeBrowseScenery() { browseScenery = nullptr; }

void MainWindow::openBrowseSketches()
{
    if(nullptr == browseSketches)
    {
        QWidget *w;
        Browser *b;
        QMainWindow *mw = new QMainWindow();
        mw->setWindowTitle("Browse Sketches");
        mw->setAttribute(Qt::WA_DeleteOnClose, true);
        w = new QWidget();
        QVBoxLayout *central_layout = new QVBoxLayout();
        central_layout->setContentsMargins(7,7,7,7);
        w->setLayout(central_layout);
        mw->setCentralWidget(w);

        /* Walls */
        b = new Browser(&gvSketchesWalls);
        central_layout->addWidget(b);
        QHBoxLayout *v_browsers_layout = new QHBoxLayout();
        v_browsers_layout->setContentsMargins(0,0,0,0);
        w = new QWidget();
        w->setLayout(v_browsers_layout);
        central_layout->addWidget(w);

        /* Ceiling Sprites */
        b = new Browser(&gvSketchesCeilings);
        v_browsers_layout->addWidget(b);

        /* Floor Sprites */
        b = new Browser(&gvSketchesFloors);
        v_browsers_layout->addWidget(b);

        /* C&F Tiles */
        b = new Browser(&gvSketchesCnfTiles);
        v_browsers_layout->addWidget(b);

        /* Bg Tiles */
        b = new Browser(&gvSketchesBgTiles);
        v_browsers_layout->addWidget(b);

        /* Info */
        QVBoxLayout *info_layout = new QVBoxLayout();
        info_layout->setContentsMargins(0,0,0,0);
        w = new QWidget();
        w->setLayout(info_layout);
        v_browsers_layout->addWidget(w);
        /* TODO */

        mw->show();
        browseSketches = mw;
        connect(browseSketches, SIGNAL(destroyed()), this, SLOT(closeBrowseSketches()));
    }
    /* ... or activate the already existing one */
    else browseScenery->activateWindow();
}

void MainWindow::closeBrowseSketches() { browseSketches = nullptr; }
