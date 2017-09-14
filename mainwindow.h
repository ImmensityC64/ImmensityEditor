#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "general.h"
#include "browser.h"
#include "map_general.h"
#include "gfx.h"
#include "grid.h"
#include "scenery_editor.h"
#include "theme_editor.h"
#include "new_dialog.h"
#include "char_set_w.h"
#include <QApplication>
#include <QMainWindow>
#include <QList>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <Qt>

class SceneryEditor;
class ThemeEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /* Browsers */
    void openSketchesCeilings();
    void openSketchesFloors();
    void openSketchesWalls();
    void openSketchesBgTiles();
    void openSketchesCnfTiles();
    void openSketchesAll();

    void closeSketchesCeilings();
    void closeSketchesFloors();
    void closeSketchesWalls();
    void closeSketchesBgTiles();
    void closeSketchesCnfTiles();
    void closeSketchesAll();

    void openSceneryCharSet();
    void openSceneryBgTiles();
    void openSceneryCnfTiles();
    void openScenerySprites();
    void openSceneryWalls();

    void closeSceneryCharSet();
    void closeSceneryBgTiles();
    void closeSceneryCnfTiles();
    void closeScenerySprites();
    void closeSceneryWalls();

    void openEditSceneries();
    void openEditThemes();

    void closeSceneryEditor();
    void closeThemeEditor();

    void openSectorCEditor(); /* edit ceiling tiles */
    void openSectorBEditor(); /* edit background tiles */
    void openSectorFEditor(); /* edit floor tiles */

    void closeSectorCEditor();
    void closeSectorBEditor();
    void closeSectorFEditor();

    void createNew();
    void enableDeletBtn(bool b);
    void deleteMap();
    void settingsClicked();

    void incSector();
    void decSector();
    void setSector(int s);
    void setSector(QString s);
    void saveModifications();
    void refreshEditor();

    void editorMousePressEvent(QPoint p, int /* unused */);

public slots:
    void selectMap(int index);
    void mapSettingsChanged();
    void sceneryChanged(int);
    void themeChanged(int);
    void tileChanged();
    void spriteChanged();
    void wallChanged();

    void wallVisible(int);
    void wallGridVisible(int);
    void playerVisible(int);

    void dndEnter(QByteArray &src, QPoint p);
    void dndDraw (QByteArray &src, QPoint p);
    void dndDrop (QByteArray &src, QPoint p);
    void dndLeave(void);

    void openProject(void);
    void saveProject(void);
    void saveAsProject(void);
    void loadProject(void);

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);

    /* Project */
    QString projPath;
    QString projExt;
    QString projName;
    QFile *projFile;

    void initGv(GfxVector &gv,
                QString title,
                GfxVector::Scope scope,
                Qt::Orientation ori,
                GfxImage::Mode mode,
                GfxData::Type type);
    void loadGfxVector(GfxVector &gv, QDataStream &in);
    void saveGfxVector(GfxVector &gv, QDataStream &out);
    void loadSceneryVector(QDataStream &in);
    void saveSceneryVector(QDataStream &out);
    void loadThemeVector(QDataStream &in);
    void saveThemeVector(QDataStream &out);
    void loadMapVector(QDataStream &in);
    void saveMapVector(QDataStream &out);

    void loadGfxData(void);
    void saveGfxData(void);
    void resetGfxData(void);

    bool openBrowser(GfxVector *gv);

    GfxVector gvSketchesCeilings;
    GfxVector gvSketchesFloors;
    GfxVector gvSketchesWalls;
    GfxVector gvSketchesBgTiles;
    GfxVector gvSketchesCnfTiles;

    GfxVector gvSceneryBgTiles;
    GfxVector gvSceneryCnfTiles;
    GfxVector gvScenerySprites;
    GfxVector gvSceneryWalls;

    Props &props;
    int map_index;
    int sector;
    bool editor_img_c_modified; /* ceiling */
    bool editor_img_b_modified; /* background */
    bool editor_img_f_modified; /* floor */
    bool editor_img_s_modified; /* sprites */
    bool editor_img_t_modified; /* tile */

    /* Map Editor */
    QVector<QGraphicsRectItem *> scrBgs;
    QVector<GfxRectItemImage *> scrRects;
    QVector<GfxImage *> scrImgs;
    QVector<shared_ptr<GfxData>> scrDatas;
    GfxHistory * scrHisC;
    GfxHistory * scrHisB;
    GfxHistory * scrHisF;
    GfxHistory * scrHisCSpr;
    GfxHistory * scrHisWSpr;
    GfxHistory * scrHisFSpr;
    GfxRectItemSelection *selTgt; /* Selection target */

    void initScrPart(ScrPart part, GfxData::Type type, int width, int height, GfxImage::Mode mode);
    void initScrRects();
    void initScrBgs();
    void initScrBgColors();

    void editorImgLoad(void);
    void editorImgSave(void);

    void refreshSceneryBrowsers(void);

    /* Grids */
    Grid *grid;
    int wallState;
    int wallGridState;
    int playerState;
    QList<QGraphicsItem *> wallItems;
    QGraphicsItemGroup *wallGrp;
    QGraphicsItemGroup *wallGridGrp;
    QGraphicsItemGroup *playerGrp;
    void createGrids();
    void createWallLine(Qt::PenStyle type, int x0, int y0, int x1, int y1);
    void vrfyWallVisibility();
    void vrfyPlayerVisibility();

    /* Helpers */
    void dndLoad();
    void dndRefresh();
    void dndImport(QByteArray &src, QPoint p);
    bool dndTest(QByteArray &src, QPoint p);
    void dndSave(QByteArray &src, QPoint p);

    /* Other Editors */
    CnfTileEditor* sectorCEditor; /* edit ceiling tiles */
    BgTileEditor*  sectorBEditor; /* edit background tiles */
    CnfTileEditor* sectorFEditor; /* edit floor tiles */

    SceneryEditor* sceneryEditor;
    ThemeEditor* themeEditor;

    CharSetW *charSetWindow;
};

#endif // MAINWINDOW_H
