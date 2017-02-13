#ifndef GRID_H
#define GRID_H

#include "gfx.h"
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QList>
#include <QPen>
#include <QBrush>
#include <QColor>

class Grid : public QObject {
    Q_OBJECT
private:
    GfxEditorView *view;
    int width;
    int height;
    bool dependsOnScale;

    QList<QGraphicsItem *> items;
    QGraphicsItemGroup *fineGrp;
    QGraphicsItemGroup *mainGrp;

    int fineState;
    int mainState;

    void createLine(int x0, int y0, int x1, int y1, int color, int style);
    void createRect(int x, int y, int w, int h, int color);

public:

    enum Type
    {
        Fine,
        Main,
    };

    explicit Grid(GfxEditorView *view, int width=0, int height=0);
    virtual ~Grid();
    void clear(void);

    bool setDependsOnScale(bool v) { return dependsOnScale=v; }

    void createFineLine(int x0, int y0, int x1, int y1);
    void createMainLine(int x0, int y0, int x1, int y1);
    void createMainRect(int x, int y, int w, int h);
    void createSeries(Type type=Type::Fine,
                      int stpX= 1, int stpY= 1,
                      int begX= 0, int begY= 0,
                      int endX=-1, int endY=-1);
public slots:
    void fineVisible(int state);
    void mainVisible(int state);
    void vrfyVisibility(void);
};

#endif // GRID_H

