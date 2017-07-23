#ifndef GFX_H
#define GFX_H

#include "map_general.h"

 /*================================================================================*\
( *     G F X   I M A G E
 \*================================================================================*/

/* GfxImage displays an image described by the data stored by a given GfxData.
 * GfxImage supports different display modes, see GfxDisplayMode!
 * GfxImage provides an interface towards GfxData and hides the differencies
 * of data handling between different display modes.
 * GfxImage is set as a brush for GfxRectItemImage.
 */

class GfxImage : public QObject,  public QImage
{
    Q_OBJECT
public:
    enum Mode { /* Display mode */
        Nothing,
        Expanded,
        Ceiling,
        Floor,
        Wall,
        Enemy, /* can be used for multicolor sprites too */
        Player,
        Cyber,
        Sketch,
        CnfSketch,
        HiRes,
    };
protected:
    Mode              M; /* Mode */
    weak_ptr<GfxData> D; /* Data */
    GfxData::Type     T; /* Type */

    quint32 cnt=0; /* Last seen modification counter of GfxData. */

    /* Scene Mask
    *     If it is set to 'true', then image pixels outside
    *     the territory of sprite will be set to the same
    *     color as the scene backtround. It has an effect
    *     in case of Expanded, Ceiling and Floor sprites only.
    */
    bool SM = false;

    /* Placement
    *     Only Ceiling and Floor sprites use this value.
    *     -1 = sprite is on the left
    *      0 = sprite is in the center
    *      1 = sprite is on the right
    */
    int P = 0;

public:
    void init(shared_ptr<GfxData> src);
    explicit GfxImage(shared_ptr<GfxData> src);
    explicit GfxImage(shared_ptr<GfxData> src, int tP);
    virtual ~GfxImage();

    Mode    mode(void);
    Mode setMode(Mode tM);
    bool    mask(void);
    bool setMask(bool tSM);
    void setData(shared_ptr<GfxData> tD);

    void refresh(void);

    QPoint img2dat(QPoint i);
    QPoint dat2img(QPoint d);

    void bitDraw(QPoint i, int m);
    void bitRect(QPoint i1, QPoint i2, int m);
    void clrDraw(QPoint i, int m);
    void clrRect(QPoint i1, QPoint i2, int m);
    void invRect(QPoint i1, QPoint i2);
    void selSrcDraw(QPoint i1, QPoint i2);

    bool isSelectionDragged(QPoint start);
    QByteArray exportDataFromSelection(QPoint start);
    void importDataToImage(QByteArray &src, QPoint p);

private:
    QPoint selSrcA, selSrcC;
    quint8 get_clr_val(shared_ptr<GfxData> d, int m);
    int xE2C(int x, int y);
    int xC2E(int x, int y);
    int xE2F(int x, int y);
    int xF2E(int x, int y);
    int hor_offset(int y);
    //                         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
    //                                |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |
    const int offset_L[21] = { 0, 0, 0, 0, 0, 2, 2, 5, 5, 7, 7, 9, 9,12,12,14,14,16,16,16,16};
    const int offset_C[21] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4};
    const int offset_R[21] = {16,16,16,16,16,14,14,11,11, 9, 9, 7, 7, 4, 4, 2, 2, 0, 0, 0, 0};

signals:
    void refreshHappened(void);
    void selectSrc(int x, int y, int w, int h);
    void selectTgt(int x, int y, int w, int h);
};

 /*================================================================================*\
( *     G F X   V E C T O R
 \*================================================================================*/

class GfxVector : public QObject {
    Q_OBJECT

public:
    enum Scope {
        Sketches,
        Scenery,
    };

protected:
    struct Element {
        shared_ptr<GfxData> data;
        Editor *editor;
        bool read_only;
    };
    QVector<Element> pVector;
    Browser *pBrowser;
    QString pTitle;
    Scope pScope;
    Qt::Orientation pOri;
    GfxImage::Mode pMode;
    GfxData::Type pType;

    void initElement(Element &e);

public:
    explicit GfxVector(int tSize = 0);
    virtual ~GfxVector();

    void clear(void);
    void close(void);
    Browser    *browser(void);
    Browser *setBrowser(Browser *tBrowser);
    QString    title(void);
    QString setTitle(QString tTitle);
    Scope    scope(void);
    Scope setScope(Scope tScope);
    Qt::Orientation    ori(void);
    Qt::Orientation setOri(Qt::Orientation tOri);
    GfxImage::Mode    mode(void);
    GfxImage::Mode setMode(GfxImage::Mode tMode);
    GfxData::Type    type(void);
    GfxData::Type setType(GfxData::Type tType);

    int size(void);
    shared_ptr<GfxData> dataAt(int index);
    shared_ptr<GfxData> setDataAt(int index, shared_ptr<GfxData> tData);
    Editor *editorAt(int index);
    Editor *setEditorAt(int index, Editor *tEditor);

public slots:
    void closeEditorAt(int index);

signals:
    void vectorChanged(void);
};

 /*================================================================================*\
( *     G F X   R E C T   I T E M
 \*================================================================================*/

/* GfxRectItemBackground is used to display a background
 * behind transparent pixels of a graphical object.
 */
class GfxRectItemBackground : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
    /* Although the background is not part of the
     * graphical object itself, the default color
     * of the background is stored by the object.
     * For that reason, we nead a pointer to the
     * graphical object.
     */
    weak_ptr<GfxData> D; /* data */
public:
    explicit GfxRectItemBackground(shared_ptr<GfxData> init);
    virtual ~GfxRectItemBackground();

public slots:
    void refresh(void);
};

/* GfxRectItemImage is used to display graphical objects (e.g. sprites).
 * It creates a GfxImage and set it as background.
 * Pointer to the GfxData which describes the graphical object is not
 * stored here but in the GfxImage.
 */
class GfxRectItemImage : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
protected:
    GfxImage *img;
    QPoint O;
public:
    explicit GfxRectItemImage(shared_ptr<GfxData> init);
    explicit GfxRectItemImage(shared_ptr<GfxData> init, int pos);
    virtual ~GfxRectItemImage();

    GfxImage *getImg(void);

    GfxImage::Mode    mode(void);
    GfxImage::Mode setMode(GfxImage::Mode tM);
    QPoint offset(void);
    QPoint setOffset(QPoint tO);

public slots:
    void refresh(void);
};

/* GfxRectItemSelection is used to highlight selected
 * area of a graphical object.
 */
class GfxRectItemSelection : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum class Type {
        Source,
        Target,
    };

    explicit GfxRectItemSelection(Type type);
    virtual ~GfxRectItemSelection();

public slots:
    void select(int x, int y, int w, int h);
};

 /*================================================================================*\
( *     G F X   V I E W
 \*================================================================================*/

class GfxView : public QGraphicsView
{
   Q_OBJECT

public:
   QGraphicsScene *scene;
   QGraphicsRectItem *background;
   explicit GfxView(QWidget *parent = 0);
   virtual ~GfxView();
};

 /*================================================================================*\
( *     G F X   B R O W S E R   V I E W
 \*================================================================================*/

class GfxBrowserView : public GfxView
{
   Q_OBJECT

public:
   explicit GfxBrowserView(QWidget *parent = 0);
   virtual ~GfxBrowserView();
};

 /*================================================================================*\
( *     G F X   E D I T O R   V I E W
 \*================================================================================*/

class GfxEditorView : public GfxView
{
   Q_OBJECT

private:
   const double scaleFactor = 1.15;

   void wheelEvent(QWheelEvent *event);
   void dragEnterEvent(QDragEnterEvent *event);
   void dragMoveEvent (QDragMoveEvent  *event);
   void dragLeaveEvent(QDragLeaveEvent *);
   void dropEvent     (QDropEvent      *event);
   void mousePressEvent  (QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseMoveEvent   (QMouseEvent *event);
public:
   explicit GfxEditorView(QWidget *parent = 0);
   virtual ~GfxEditorView();

signals:
   void gfxEditorViewPressEvent  (QPoint p, int m);
   void gfxEditorViewReleaseEvent(QPoint p);
   void gfxEditorViewMoveEvent   (QPoint p);
   void gfxEditorViewScaleEvent  (void);
   void gfxEditorViewEnterEvent(QByteArray &data, QPoint p);
   void gfxEditorViewDrawEvent (QByteArray &data, QPoint p);
   void gfxEditorViewDropEvent (QByteArray &data, QPoint p);
   void gfxEditorViewLeaveEvent();
};

#endif // GFX_H
