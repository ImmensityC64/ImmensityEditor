
#include "mainwindow.h"

/****    M E N U   E D I T
 ******************************************************************************/

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

/****    M E N U   B R O W S E
 ******************************************************************************/

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
        b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        central_layout->addWidget(b, 0, Qt::AlignTop);
        QHBoxLayout *v_browsers_layout = new QHBoxLayout();
        v_browsers_layout->setContentsMargins(0,0,0,0);
        w = new QWidget();
        w->setLayout(v_browsers_layout);
        central_layout->addWidget(w);

        /* Sprites */
        b = new Browser(&gvScenerySprites);
        b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        v_browsers_layout->addWidget(b);

        /* C&F Tiles */
        b = new Browser(&gvSceneryCnfTiles);
        b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        v_browsers_layout->addWidget(b);

        /* Bg Tiles */
        b = new Browser(&gvSceneryBgTiles);
        b->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        v_browsers_layout->addWidget(b);

        /* CharSet */
        QVBoxLayout *info_layout = new QVBoxLayout();
        info_layout->setContentsMargins(0,0,0,0);
        w = new QWidget();
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        w->setLayout(info_layout);
        v_browsers_layout->addWidget(w, 0, Qt::AlignTop);
        charSet = new CharSet(props.map2imgCharSet(map_index));
        charSet->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QFrame *line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sceneryInfo = new QPlainTextEdit("Scenery Info");
        sceneryInfo->setReadOnly(true);
        sceneryInfo->setFrameShape(QFrame::NoFrame);
        sceneryInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        info_layout->addWidget(charSet,     0, Qt::AlignTop);
        info_layout->addWidget(line,        0, Qt::AlignBottom);
        info_layout->addWidget(sceneryInfo, 0, Qt::AlignBottom);

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
    else browseSketches->activateWindow();
}

void MainWindow::closeBrowseSketches() { browseSketches = nullptr; }

/****    S E C T O R   E D I T O R S
 ******************************************************************************/

void MainWindow::openSectorCEditor()
{
    if(nullptr == sectorCEditor)
    {
        shared_ptr<GfxData> data(scrDatas.at((int)ScrPart::CeilingFgC));
        sectorCEditor = new CnfTileEditor(data, indexEditSectorCeiling);
        sectorCEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sectorCEditor->show();
        connect(sectorCEditor, SIGNAL(destroyed()),   this, SLOT(closeSectorCEditor()));
        connect(data.get(),    SIGNAL(dataChanged()), this, SLOT(sectorCChanged()));
    }
    /* ... or activate the already existing one */
    else sectorCEditor->activateWindow();
}

void MainWindow::openSectorBEditor()
{
    if(nullptr == sectorBEditor)
    {
        shared_ptr<GfxData> data(scrDatas.at((int)ScrPart::BackgroundC));
        sectorBEditor = new BgTileEditor(data, indexEditSectorBackground);
        sectorBEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sectorBEditor->show();
        connect(sectorBEditor, SIGNAL(destroyed()),   this, SLOT(closeSectorBEditor()));
        connect(data.get(),    SIGNAL(dataChanged()), this, SLOT(sectorBChanged()));
    }
    /* ... or activate the already existing one */
    else sectorBEditor->activateWindow();
}

void MainWindow::openSectorFEditor()
{
    if(nullptr == sectorFEditor)
    {
        shared_ptr<GfxData> data(scrDatas.at((int)ScrPart::FloorFgC));
        sectorFEditor = new CnfTileEditor(data, indexEditSectorFloor);
        sectorFEditor->setAttribute(Qt::WA_DeleteOnClose, true);
        sectorFEditor->show();
        connect(sectorFEditor, SIGNAL(destroyed()),   this, SLOT(closeSectorFEditor()));
        connect(data.get(),    SIGNAL(dataChanged()), this, SLOT(sectorFChanged()));
    }
    /* ... or activate the already existing one */
    else sectorFEditor->activateWindow();
}

void MainWindow::closeSectorCEditor() { sectorCEditor = nullptr; }
void MainWindow::closeSectorBEditor() { sectorBEditor = nullptr; }
void MainWindow::closeSectorFEditor() { sectorFEditor = nullptr; }
