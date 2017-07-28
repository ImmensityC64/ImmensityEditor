#ifndef EDITOR_H
#define EDITOR_H

#include "ui_editor.h"
#include "ui_sprite_editor.h"
#include "ui_bg_sketch_editor.h"
#include "ui_cnf_sketch_editor.h"
#include "color_dialog.h"
#include "gfx.h"
#include "tools.h"
#include "grid.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QApplication>
#include <QShortcut>

using namespace std;

class Editor : public QMainWindow
{
    Q_OBJECT
public:
    explicit Editor(shared_ptr<GfxData> init, int index, QWidget *parent = 0);
    virtual ~Editor();

protected:
    Ui::Editor *ui;
    QWidget *spec;  /* Editor specific widget */
    void addSpecWidget(void);

    int I; /* index of GfxData in GfxVector */

    GfxRectItemImage *rect;     /* Rect item for image */
    GfxImage *img;              /* Sprite image */
    GfxRectItemBackground *bg;  /* Background for image */
    GfxRectItemSelection *selSrc; /* Selection source */
    GfxRectItemSelection *selTgt; /* Selection target */
    bool showSelTgt;

    Grid *grid;

    shared_ptr<GfxData> data;   /* Edited data */
    weak_ptr<GfxData> src;      /* Source (original) data */
    GfxHistory *his;        /* Data history */

    QShortcut *scUndo;
    QShortcut *scRedo;
    QShortcut *scApply;
    QShortcut *scRevert;

    void createGrids(void);

public slots:
    void load(void);
    void save(void);
    void undo(void);
    void redo(void);
    void apply(void);
    void revert(void);
    void command(Command cmd);
    void dndEnter(QByteArray &src, QPoint p);
    void dndDraw (QByteArray &src, QPoint p);
    void dndDrop (QByteArray &src, QPoint p);
    void dndLeave(void);

signals:
    void destroyed(int index);
};

class SpriteEditor : public Editor
{
    Q_OBJECT
protected:
    Ui::SpriteEditorWidget *wui;

public:
    explicit SpriteEditor(shared_ptr<GfxData> init, int index, QWidget *parent = 0);
    virtual ~SpriteEditor();

    void changeDisplayMode(int i);

public slots:
    void receiveColor(quint8 i);
    void receiveAlpha(quint8 i);
    void refreshColorButtons(void);
    void changeImgDisplayMode(int i);
};

class BgSketchEditor : public Editor
{
    Q_OBJECT
protected:
    Ui::BgSketchEditorWidget *wui;

public:
    explicit BgSketchEditor(shared_ptr<GfxData> init, int index, QWidget *parent = 0);
    virtual ~BgSketchEditor();

public slots:
    void receiveColor(quint8 i);
    void receiveAlpha(quint8 i);
    void receiveComm0(quint8 i);
    void receiveComm1(quint8 i);
    void refreshColorButtons(void);
};

class BgTileEditor : public BgSketchEditor
{
    Q_OBJECT
public:
    explicit BgTileEditor(shared_ptr<GfxData> init, int index = 0, QWidget *parent = 0);
    virtual ~BgTileEditor();
public slots:
    void apply(void);
    void revert(void);
};

class CnfSketchEditor : public Editor
{
    Q_OBJECT
protected:
    Ui::CnfSketchEditorWidget *wui;

public:
    explicit CnfSketchEditor(shared_ptr<GfxData> init, int index, QWidget *parent = 0);
    virtual ~CnfSketchEditor();

public slots:
    void receiveColor(quint8 i);
    void receiveECM0(quint8 i);
    void receiveECM1(quint8 i);
    void receiveECM2(quint8 i);
    void receiveECM3(quint8 i);
    void refreshColorButtons(void);
};

class CnfTileEditor : public CnfSketchEditor
{
    Q_OBJECT
public:
    explicit CnfTileEditor(shared_ptr<GfxData> init, int index = 0, QWidget *parent = 0);
    virtual ~CnfTileEditor();
public slots:
    void apply(void);
    void revert(void);
};

#endif // EDITOR_H
