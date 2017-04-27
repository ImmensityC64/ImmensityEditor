
#include "gfx.h"

 /*================================================================================*\
( *     G F X   V I E W
 \*================================================================================*/

GfxView::GfxView(QWidget *parent) :
    QGraphicsView(parent)
{
    background = new QGraphicsRectItem();
    background->setPen(QPen(Qt::NoPen));
    background->setBrush( C64::ins().brush_alpha( C64::AlphaWhite ) );
    background->setZValue(-2);

    scene = new QGraphicsScene();
    setScene(scene);
    scene->setBackgroundBrush(QColor(QRgb( Cfg::ins().val(Cfg::ViewSceneColor) )));
    scene->addItem(background);
    show();
}
GfxView::~GfxView()
{
    delete scene;
}

 /*================================================================================*\
( *     G F X   B R O W S E R   V I E W
 \*================================================================================*/

GfxBrowserView::GfxBrowserView(QWidget *parent) :
    GfxView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
}
GfxBrowserView::~GfxBrowserView() {}

 /*================================================================================*\
( *     G F X   E D I T O R   V I E W
 \*================================================================================*/

GfxEditorView::GfxEditorView(QWidget *parent) :
    GfxView(parent)
{
    setAcceptDrops(true);
    setResizeAnchor(ViewportAnchor::AnchorViewCenter);
    setTransformationAnchor(ViewportAnchor::AnchorViewCenter);
}
GfxEditorView::~GfxEditorView() {}

void GfxEditorView::wheelEvent(QWheelEvent *event)
{
    /* TODO: CAD-like scroll */
    qreal factor = (event->delta() > 0) ? scaleFactor : 1.0/scaleFactor;
    scale(factor, factor);
    emit gfxEditorViewScaleEvent(); // Tell Editor to verify grids!
    event->accept();
}

void GfxEditorView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(mimeTypeGfxData))
    {
        QByteArray ba(event->mimeData()->data(mimeTypeGfxData));
        emit gfxEditorViewEnterEvent(ba, QPoint(
             (int)mapToScene(event->pos()).x(),
             (int)mapToScene(event->pos()).y() ));
        event->acceptProposedAction();
    }
}

void GfxEditorView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(mimeTypeGfxData))
    {
        QByteArray ba(event->mimeData()->data(mimeTypeGfxData));
        emit gfxEditorViewDrawEvent(ba, QPoint(
             (int)mapToScene(event->pos()).x(),
             (int)mapToScene(event->pos()).y() ));
        event->acceptProposedAction();
    }
}

void GfxEditorView::dragLeaveEvent(QDragLeaveEvent * /* unused */)
{
    emit gfxEditorViewLeaveEvent();
}

void GfxEditorView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(mimeTypeGfxData))
    {
        QByteArray ba(event->mimeData()->data(mimeTypeGfxData));
        emit gfxEditorViewDropEvent(ba, QPoint(
             (int)mapToScene(event->pos()).x(),
             (int)mapToScene(event->pos()).y() ));
        event->acceptProposedAction();
    }
}

/*  Interface towards Tools  */

void GfxEditorView::mousePressEvent(QMouseEvent *event)
{
    /* Handle left & right buttons only! */
    if(! (event->button() & (Qt::LeftButton|Qt::RightButton)) ) return;
    event->accept();

    if( (event->buttons() & Qt::LeftButton) &&
            (event->buttons() & Qt::RightButton) )
    {
        /* Both buttons are pressed.
         * It is an invalid state for the tools.
         * Let's pretend that none of the buttons are pressed anymore!
         */
        emit gfxEditorViewReleaseEvent(
                    QPoint( (int) mapToScene(event->pos()).x(),
                            (int) mapToScene(event->pos()).y() ) );
    }
    else
    {
        /* Ordinary button press event. */

        /* Calculate GfxEdTool::Modifiers value which will tell the tool
         * which color should be used.
         *     Right     = 0b00,
         *     Left      = 0b01,
         *     CtrlRight = 0b10,
         *     CtrlLeft  = 0b11,
         */
        int mod = (event->modifiers() & Qt::ControlModifier) ? 2:0;
        if(event->button() & Qt::LeftButton) mod++;

        emit gfxEditorViewPressEvent(
                    QPoint( (int) mapToScene(event->pos()).x(),
                            (int) mapToScene(event->pos()).y() ), mod );
    }
}

void GfxEditorView::mouseReleaseEvent(QMouseEvent *event)
{
    /* Handle left & right buttons only! */
    if(! (event->button() & (Qt::LeftButton|Qt::RightButton)) ) return;
    event->accept();

    if(event->buttons() & (Qt::LeftButton|Qt::RightButton))
    {
        /* A button is still being pressed.
         * It means that both buttons were pressed which was an invalid state.
         * Let's pretend that the remaining button has just been pressed!
         */

        /* Calculate GfxEdTool::Modifiers value which will tell the tool
         * which color should be used.
         *     Right     = 0b00,
         *     Left      = 0b01,
         *     CtrlRight = 0b10,
         *     CtrlLeft  = 0b11,
         */
        int mod = (event->modifiers() & Qt::ControlModifier) ? 2:0;
        if(event->buttons() & Qt::LeftButton) mod++;

        emit gfxEditorViewPressEvent(
                    QPoint( (int) mapToScene(event->pos()).x(),
                            (int) mapToScene(event->pos()).y() ), mod );
    }
    else
    {
        /* Ordinary button release event. */
        emit gfxEditorViewReleaseEvent(
                    QPoint( (int) mapToScene(event->pos()).x(),
                            (int) mapToScene(event->pos()).y() ) );
    }
}

void GfxEditorView::mouseMoveEvent(QMouseEvent *event)
{
    /* Do not forward the event
     * either if none of the buttons are pressed
     * or both buttons are pressed!
     */
    int state = event->buttons() & (Qt::LeftButton|Qt::RightButton);
    if(!state) return;
    event->accept();

    if((state&Qt::LeftButton) && (state&Qt::RightButton)) return;

    emit gfxEditorViewMoveEvent(
                QPoint( (int) mapToScene(event->pos()).x(),
                        (int) mapToScene(event->pos()).y() ) );
}
