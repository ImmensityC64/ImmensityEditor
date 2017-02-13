#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QPoint>
#include <QObject>
#include <QPen>
#include <QBrush>
#include "general.h"
#include "gfx.h"

 /*================================================================================*\
( *     I C O N   R E C T
 \*================================================================================*/

/* IconRect is a QGraphicsRectItem displaying the icon */

class IconRect : public QObject, public  QGraphicsRectItem
{
    Q_OBJECT

private:
    int id;

    /* tell the toolbox if the icon is being clicked */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    explicit IconRect(int x, int y, int id, Icon::Enum icon);
    virtual ~IconRect();

signals:
    void clicked(int id);
};

 /*================================================================================*\
( *     T O O L
 \*================================================================================*/

class Tool : public QObject
{
  Q_OBJECT

protected:
  Icon::Enum I; /* icon */

public:
  explicit Tool(QObject *parent = 0);
  virtual ~Tool();

  Icon::Enum icon(void);
  Icon::Enum setIcon(Icon::Enum i);
};

 /*================================================================================*\
( *     T O O L B O X
 \*================================================================================*/

class Toolbox : public QGraphicsView
{
    Q_OBJECT

protected:
    const int width = 4;
    struct s {
        Tool     *tool;
        IconRect *icon;
    };
    QVector<s> v;
    int current; /* current tool */
    QGraphicsScene scene;
    QGraphicsRectItem selection;

public:
    explicit Toolbox(QWidget *parent = 0);
    virtual ~Toolbox();

    /* Register tools */
    void addTool(Tool *tool);

public slots:
    virtual void changeTool(int /* unused */) = 0;
};

 /*================================================================================*\
( *     G F X   E D I T O R
 \*================================================================================*/

class GfxEditorToolbox : public Toolbox
{
    Q_OBJECT

protected:
    GfxEditorView *view;

public:
    explicit GfxEditorToolbox(QWidget *parent = 0);
    virtual ~GfxEditorToolbox();

    /* Connect editor's view to toolbox.
     * Toolbox will forward signals of the
     * editor's view towards the registered tools.
     */
    void setView(GfxEditorView *view);

public slots:
    void changeTool(int id) override;
};

 /*================================================================================*\
( *     G F X   E D I T O R   T O O L
 \*================================================================================*/

class GfxEditorTool : public Tool
{
   Q_OBJECT

protected:
    GfxImage *img;
    int mod; /* modifier */
    QPoint start;

public:
    explicit GfxEditorTool(GfxImage *img, QObject *editor);
    virtual ~GfxEditorTool();

public slots:
    virtual void gfxEditorToolPressEvent  (QPoint /* unused */, int /* unused */);
    virtual void gfxEditorToolReleaseEvent(QPoint /* unused */);
    virtual void gfxEditorToolMoveEvent   (QPoint /* unused */);
    virtual void open(void);
    virtual void close(void);

signals:
    void command(Command cmd);
};

#endif // TOOLBOX_H
