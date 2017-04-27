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

    void closeSceneryCharSet();
    void closeSceneryBgTiles();
    void closeSceneryCnfTiles();

    void openEditSceneries();
    void openEditThemes();

    void closeSceneryEditor();
    void closeThemeEditor();

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

    void wallVisible(int);
    void playerVisible(int);

    void dndEnter(QByteArray &src, QPoint p);
    void dndDraw (QByteArray &src, QPoint p);
    void dndDrop (QByteArray &src, QPoint p);
    void dndLeave(void);

private:
    Ui::MainWindow *ui;

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

    bool openBrowser(GfxVector *gv);

    void resetGfxData(void);
    void loadGfxData(void);
    void saveGfxData(void);

    GfxVector gvSketchesCeilings;
    GfxVector gvSketchesFloors;
    GfxVector gvSketchesWalls;
    GfxVector gvSketchesBgTiles;
    GfxVector gvSketchesCnfTiles;

    GfxVector gvSceneryBgTiles;
    GfxVector gvSceneryCnfTiles;

    CharSetW *charSetWindow;

    Props props;
    int map_index;
    int sector;
    bool editor_img_c_modified; /* ceiling */
    bool editor_img_b_modified; /* background */
    bool editor_img_f_modified; /* floor */
    bool editor_img_s_modified; /* sprites */

    /* Map Editor */
    QVector<QGraphicsRectItem *> scrBgs;
    QVector<GfxRectItemImage *> scrRects;
    QVector<GfxImage *> scrImgs;
    QVector<shared_ptr<GfxData>> scrDatas;
    GfxHistory * scrHisC;
    GfxHistory * scrHisB;
    GfxHistory * scrHisF;
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
    int playerState;
    QList<QGraphicsItem *> wallItems;
    QGraphicsItemGroup *wallGrp;
    QGraphicsItemGroup *playerGrp;
    void createGrids();

    /* Helpers */
    void dndLoad();
    void dndRefresh();
    void dndImport(QByteArray &src, QPoint p);
    bool dndTest(QPoint p);
    void dndSave(QPoint p);

    /* Other Editors */
    SceneryEditor* sceneryEditor;
    ThemeEditor* themeEditor;
};

#endif // MAINWINDOW_H
