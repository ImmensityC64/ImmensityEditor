
#include "gfx.h"

 /*================================================================================*\
( *     G F X   R E C T   I T E M
 \*================================================================================*/

/* GfxRectItemBackground is used to display a background
 * behind transparent pixels of a graphical object.
 */
GfxRectItemBackground::GfxRectItemBackground(shared_ptr<GfxData> init)
{
   D = init;
   setRect(0,0,init->imgW(),init->imgH());
   setPen(QPen(Qt::NoPen));
   refresh();
}
GfxRectItemBackground::~GfxRectItemBackground() {}

void GfxRectItemBackground::refresh(void)
{
    shared_ptr<GfxData> tD = D.lock();
    if(tD)
    {
        setBrush( C64::ins().brush_alpha( tD->color((int)GfxData::ColorIndex::Backg) ) );
    }
    else
    {
        setBrush( QBrush( Qt::NoBrush ) );
    }
}

/* GfxRectItemImage is used to display graphical objects (e.g. sprites).
 * It creates a GfxImage and set it as background.
 * Pointer to the GfxData which describes the graphical object is not
 * stored here but in the GfxImage.
 */
GfxRectItemImage::GfxRectItemImage(shared_ptr<GfxData> init) : O(0,0)
{
    img = new GfxImage(init);
    setRect(0,0,img->width(),img->height());
    setPen(QPen(Qt::NoPen));
}
GfxRectItemImage::GfxRectItemImage(shared_ptr<GfxData> init, int pos) : O(0,0)
{
    img = new GfxImage(init, pos);
    setRect(0,0,img->width(),img->height());
    setPen(QPen(Qt::NoPen));
}
GfxRectItemImage::~GfxRectItemImage()
{
    delete img;
}

GfxImage *GfxRectItemImage::getImg(void) { return img; }

GfxImage::Mode GfxRectItemImage::mode(void) { return img->mode(); }
GfxImage::Mode GfxRectItemImage::setMode(GfxImage::Mode tM) { return img->setMode(tM); }
QPoint GfxRectItemImage::offset(void) { return O; }
QPoint GfxRectItemImage::setOffset(QPoint tO) { return O = tO; }

void GfxRectItemImage::refresh(void)
{
    QBrush brush(*img);
    if(!O.isNull())
    {
        QTransform matrix;
        brush.setTransform(matrix.fromTranslate(O.x(),O.y()));
    }
    setBrush(brush);
}

/* GfxRectItemSelection is used to highlight selected
 * area of a graphical object.
 */
GfxRectItemSelection::GfxRectItemSelection(Type type)
{
    QColor penClr, brushClr;
    if(Type::Source == type)
    {
        penClr   = QColor::fromRgba( Cfg::ins().val(Cfg::EditorSelSrcPenColor) );
        brushClr = QColor::fromRgba( Cfg::ins().val(Cfg::EditorSelSrcBrushColor) );
    }
    else /* Type::Target */
    {
        penClr   = QColor::fromRgba( Cfg::ins().val(Cfg::EditorSelTgtPenColor) );
        brushClr = QColor::fromRgba( Cfg::ins().val(Cfg::EditorSelTgtBrushColor) );
    }

    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(0.5);
    pen.setColor(penClr);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(brushClr);

    setPen(pen);
    setBrush(brush);
    hide();
}
GfxRectItemSelection::~GfxRectItemSelection() {}

void GfxRectItemSelection::select(int x, int y, int w, int h)
{
    setRect(x,y,w,h);
}
