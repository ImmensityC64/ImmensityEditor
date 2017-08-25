#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QRect>
#include <QStatusBar>
#include <QString>
#include <Qt>
#include "gfx.h"
#include "editor.h"

class Browser : public QMainWindow
{
    Q_OBJECT

private:
    GfxVector *gv;
    QStatusBar *status_bar;
    QWidget *central;
    QHBoxLayout *central_layout;
    QScrollArea *scroll_area;
    QWidget *scroll_widget;
    QBoxLayout *layout;
    int no_of_elements;

    void addGfxTile(int index);
    void addNewTile(int index);
public:
    explicit Browser(GfxVector *gv, QWidget *parent = 0);
    virtual ~Browser();
public slots:
    void refresh(void);
    void refreshTile(int index);
    void createGfx(int index);
    void openEditor(int index);
};

class BrowserTile : public QFrame
{
    Q_OBJECT
public:
    enum class Type {
        Gfx,
        New,
    };
protected:
    int  I; /* index of the displayed graphical object in Browser::DataVector */
    Type T;
    QBoxLayout *layout;
public:
    explicit BrowserTile(int index, Qt::Orientation ori, QWidget *parent = 0);
    virtual ~BrowserTile();
    int    index(void);
    int setIndex(int tI);
    Type    type(void);
signals:
    void clicked(int index);
};

/* BrowserGfxTile displays a graphical object in read-only mode. */
class BrowserGfxTile : public BrowserTile
{
    Q_OBJECT

private:
    QPoint mouse_start;
    int mouse_mode;

public:
    GfxRectItemImage *rect;     /* Rect item for image */
    GfxImage *img;              /* Sprite image */
    GfxRectItemBackground *bg;  /* Background for image */
    explicit BrowserGfxTile(int index,
                            Qt::Orientation ori,
                            shared_ptr<GfxData> src,
                            /* use default image display mode if it is not given */
                            GfxImage::Mode mode = GfxImage::Mode::Nothing,
                            QWidget *parent = 0);
    virtual ~BrowserGfxTile();

public slots:
    void browserMousePressEvent  (QPoint p, int m);
    void browserMouseMoveEvent   (QPoint p);
    void browserMouseReleaseEvent(QPoint p);
};

/* BrowserNewTile is functioning as a 'new' button to create and edit new graphical objects */
class BrowserNewTile : public BrowserTile
{
    Q_OBJECT
private:
    void mousePressEvent(QMouseEvent *event);
public:
    explicit BrowserNewTile(int index, Qt::Orientation ori, QWidget *parent = 0);
    virtual ~BrowserNewTile();
};

#endif // BROWSER_H
