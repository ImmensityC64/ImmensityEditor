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
#include "char_set.h"
#include <QApplication>
#include <QMainWindow>
#include <QList>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QPlainTextEdit>
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
    void openEditSceneries();
    void openEditThemes();

    void closeEditSceneries();
    void closeEditThemes();

    void openBrowseSketches();
    void openBrowseScenery();

    void closeBrowseSketches();
    void closeBrowseScenery();

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
    void setSectorSelectionL();
    void setSectorSelectionR();
    void remapSectorsForMove();
    void remapSectorsForCopy();
    void saveModifications();
    void refreshEditor();

    void editorMousePressEvent(QPoint p, int /* unused */);

    void receiveColorCeiling(quint8 i);
    void receiveColorWall(quint8 i);
    void receiveColorFloor(quint8 i);

    void radioWallToggled(bool checked);
    void radioPlayerToggled(bool checked);

public slots:
    void selectMap(int index);
    void mapSettingsChanged();
    void sceneryChanged(int);
    void themeChanged(int);
    void tileChanged();
    void spriteChanged();
    void wallChanged();
    void sectorCChanged();
    void sectorBChanged();
    void sectorFChanged();

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
    sector_modint sector;
    sector_modint sectorSelectionL;
    sector_modint sectorSelectionR;
    int sectorSelectionNum; /* intentionally integer, can be equal to mod */
    sector_modint sectorMoveL;
    sector_modint sectorMoveR;
    sector_modint sectorCopyL;
    sector_modint sectorCopyR;
    bool editor_img_c_modified; /* ceiling */
    bool editor_img_b_modified; /* background */
    bool editor_img_f_modified; /* floor */
    bool editor_img_s_modified; /* sprites */
    bool editor_img_t_modified; /* tile */
    void calculateSectorSelection();
    QVector<int> remapSectors;
    void sectorMoveOrCopy();

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
    QString sceneryInfoLine(char *name, quint32 used, quint32 total);

    QShortcut *scSectorInc;
    QShortcut *scSectorDec;

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

    QPlainTextEdit *sceneryInfo;
    CharSet *charSet;
    QMainWindow *browseSketches;
    QMainWindow *browseScenery;
};

#endif // MAINWINDOW_H
