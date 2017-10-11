#ifndef CHAR_SET_H
#define CHAR_SET_H

#include "ui_char_set.h"
#include "gfx.h"
#include "grid.h"
#include <QWidget>
#include <QGraphicsScene>

using namespace std;

class CharSet : public QWidget
{
    Q_OBJECT
public:
    explicit CharSet(shared_ptr<GfxData> init, QWidget *parent = 0);
    virtual ~CharSet();

    void refresh(shared_ptr<GfxData> data);

protected:
    Ui::CharSet *ui;

    GfxRectItemImage *rect;     /* Rect item for image */
    GfxImage *img;              /* Sprite image */
    GfxRectItemBackground *bg;  /* Background for image */
    shared_ptr<GfxData> gfx_data;

    Grid *grid;
    void createGrids(void);
};

#endif // CHAR_SET_H
