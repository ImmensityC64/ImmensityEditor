
#include "tools.h"

 /*================================================================================*\
( *     D R A W
 \*================================================================================*/

GfxEditorToolBitDraw::GfxEditorToolBitDraw(GfxImage *img, QObject *editor) :
    GfxEditorTool(img, editor)
{
    I = Icon::Pencil;
}
GfxEditorToolBitDraw::~GfxEditorToolBitDraw() {}

void GfxEditorToolBitDraw::gfxEditorToolPressEvent(QPoint p, int m)
{
    mod = m;
    gfxEditorToolMoveEvent(p);
}
void GfxEditorToolBitDraw::gfxEditorToolMoveEvent(QPoint p)
{
    img->bitDraw(p,mod);
    img->clrDraw(p,mod);
    img->refresh();
}
void GfxEditorToolBitDraw::gfxEditorToolReleaseEvent(QPoint /* unused */)
{
    emit command(Command::SaveHistory);
}

 /*================================================================================*\
( *     D R A W - R E C T
 \*================================================================================*/

GfxEditorToolBitRect::GfxEditorToolBitRect(GfxImage *img, QObject *editor) :
    GfxEditorTool(img, editor)
{
    I = Icon::PencilRect;
}
GfxEditorToolBitRect::~GfxEditorToolBitRect(){}

void GfxEditorToolBitRect::gfxEditorToolPressEvent(QPoint p, int m)
{
    mod = m;
    start = p;
    gfxEditorToolMoveEvent(p);
}
void GfxEditorToolBitRect::gfxEditorToolMoveEvent(QPoint p)
{
    emit command(Command::LoadHistory);
    img->bitRect(start,p,mod);
    img->clrRect(start,p,mod);
    img->refresh();
}
void GfxEditorToolBitRect::gfxEditorToolReleaseEvent(QPoint /* unused */)
{
    emit command(Command::SaveHistory);
}

 /*================================================================================*\
( *    P A I N T
 \*================================================================================*/

GfxEditorToolClrDraw::GfxEditorToolClrDraw(GfxImage *img, QObject *editor) :
  GfxEditorTool(img, editor)
{
    I = Icon::Paint;
}
GfxEditorToolClrDraw::~GfxEditorToolClrDraw(){}

void GfxEditorToolClrDraw::gfxEditorToolPressEvent(QPoint p, int m)
{
    mod = m;
    gfxEditorToolMoveEvent(p);
}
void GfxEditorToolClrDraw::gfxEditorToolMoveEvent(QPoint p)
{
    img->clrDraw(p,mod);
    img->refresh();
}
void GfxEditorToolClrDraw::gfxEditorToolReleaseEvent(QPoint /* unused */)
{
    emit command(Command::SaveHistory);
}

 /*================================================================================*\
( *    P A I N T - R E C T
 \*================================================================================*/

GfxEditorToolClrRect::GfxEditorToolClrRect(GfxImage *img, QObject *editor) :
   GfxEditorTool(img, editor)
{
    I = Icon::PaintRect;
}
GfxEditorToolClrRect::~GfxEditorToolClrRect(){}

void GfxEditorToolClrRect::gfxEditorToolPressEvent(QPoint p, int m)
{
    mod = m;
    start = p;
    gfxEditorToolMoveEvent(p);
}
void GfxEditorToolClrRect::gfxEditorToolMoveEvent(QPoint p)
{
    emit command(Command::LoadHistory);
    img->clrRect(start,p,mod);
    img->refresh();
}
void GfxEditorToolClrRect::gfxEditorToolReleaseEvent(QPoint /* unused */)
{
    emit command(Command::SaveHistory);
}

 /*================================================================================*\
( *     S E L E C T
 \*================================================================================*/

GfxEditorToolSelect::GfxEditorToolSelect(GfxImage *img, QObject *editor) :
   GfxEditorTool(img, editor)
{
    I = Icon::Select;
}
GfxEditorToolSelect::~GfxEditorToolSelect(){}

void GfxEditorToolSelect::gfxEditorToolPressEvent(QPoint p, int m)
{
    mod = m&1; /* Do not care about Ctrl key */
    start = p;
    gfxEditorToolMoveEvent(p);
}
void GfxEditorToolSelect::gfxEditorToolMoveEvent(QPoint p)
{
    if(mod) /* Left click -> Drag & Drop */
    {
        if ((p-start).manhattanLength() < QApplication::startDragDistance())
            return;

        if(img->isSelectionDragged(start))
        {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            mimeData->setData(mimeTypeGfxData,img->exportDataFromSelection(start));
            drag->setMimeData(mimeData);
            /* TODO: Tell 'img' to generate pixmap from selection */
            //drag->setPixmap(iconPixmap);
            drag->exec(); /* We always copy, do not care the result. */
        }
    }
    else /* Right click -> Select */
    {
        img->selSrcDraw(start,p);
    }
}
void GfxEditorToolSelect::open(void)
{
    emit command(Command::ShowSelSrc);
}
void GfxEditorToolSelect::close(void)
{
    emit command(Command::HideSelSrc);
}
