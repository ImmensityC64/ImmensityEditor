#ifndef CHAR_SET_W_H
#define CHAR_SET_W_H

#include "ui_char_set_w.h"
#include "gfx.h"
#include "grid.h"
#include <QMainWindow>
#include <QGraphicsScene>

using namespace std;

class CharSetW : public QMainWindow
{
    Q_OBJECT
public:
    explicit CharSetW(shared_ptr<GfxData> init, QWidget *parent = 0);
    virtual ~CharSetW();

protected:
    Ui::CharSetW *ui;

    GfxRectItemImage *rect;     /* Rect item for image */
    GfxImage *img;              /* Sprite image */
    GfxRectItemBackground *bg;  /* Background for image */

    Grid *grid;
    void createGrids(void);
};

#endif // CHAR_SET_W_H
