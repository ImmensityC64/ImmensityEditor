
#include "toolbox.h"

 /*================================================================================*\
( *     I C O N   R E C T
 \*================================================================================*/

/* IconRect is a QGraphicsRectItem displaying the icon */
IconRect::IconRect(int x, int y, int id, Icon::Enum icon) :
    QGraphicsRectItem( x*Icon::size, y*Icon::size,  /* x, y */
                         Icon::size,   Icon::size ) /* width, height */
{
    this->id = id;
    setPen(QPen(Qt::NoPen));
    setBrush( QBrush(QPixmap(Icon::ins().xpm[icon])) );
}
IconRect::~IconRect() {}

/* tell the toolbox if the icon is being clicked */
void IconRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    emit clicked(id);
}

 /*================================================================================*\
( *     T O O L
 \*================================================================================*/

Tool::Tool(QObject *parent) :
     QObject(parent),
     I(Icon::Empty) {}
Tool::~Tool() {}

Icon::Enum Tool::icon(void)            { return I; }
Icon::Enum Tool::setIcon(Icon::Enum i) { return I = i; }

 /*================================================================================*\
( *     T O O L B O X
 \*================================================================================*/

Toolbox::Toolbox(QWidget *parent) :
    QGraphicsView(parent),
    selection(0,0,Icon::size,Icon::size)
{
    selection.setPen(QPen(Qt::NoPen));
    selection.setBrush( QBrush(QColor(QRgb( Cfg::ins().val(Cfg::IconSelectColor) ))) );
    selection.setZValue(-1);

    setScene(&scene);
    scene.setBackgroundBrush(QColor(QRgb( Cfg::ins().val(Cfg::ToolboxSceneColor) )));
    scene.addItem(&selection);
    show();

    current = -1;
}
Toolbox::~Toolbox() {}

/* Register tools. */
void Toolbox::addTool(Tool *tool)
{
    int n = v.size();
    int x = n%width;
    int y = n/width;
    v.resize(n+1);
    v.last().tool = tool;
    v.last().icon = new IconRect(x,y,n,tool->icon());
    scene.addItem(v.last().icon);
    setMinimumHeight((y+1)*Icon::size+1);
    setMaximumHeight((y+1)*Icon::size+1);
    setSceneRect(0, 0, width*Icon::size, (y+1)*Icon::size);
    connect(v.last().icon, SIGNAL(clicked(int)), this, SLOT(changeTool(int)));
}

 /*================================================================================*\
( *     G F X   E D I T O R
 \*================================================================================*/

GfxEditorToolbox::GfxEditorToolbox(QWidget *parent) : Toolbox(parent)
{
    view = 0;
}
GfxEditorToolbox::~GfxEditorToolbox() {}

/* Connect editor view to toolbox.
 * Toolbox will forward signals of the
 * editor view towards the registered tools.
 */
void GfxEditorToolbox::setView(GfxEditorView *view) { this->view = view; }

void GfxEditorToolbox::changeTool(int id)
{
    if(!view) return;

    if(0 <= current)
    {
        /* disconnect previous tool from mouse events of editor */
        disconnect( view, SIGNAL(gfxEditorViewPressEvent(QPoint,int)),
                    v.at(current).tool,   SLOT(gfxEditorToolPressEvent(QPoint,int)) );
        disconnect( view, SIGNAL(gfxEditorViewReleaseEvent(QPoint)),
                    v.at(current).tool,   SLOT(gfxEditorToolReleaseEvent(QPoint)) );
        disconnect( view, SIGNAL(gfxEditorViewMoveEvent(QPoint)),
                    v.at(current).tool,   SLOT(gfxEditorToolMoveEvent(QPoint)) );
        ((GfxEditorTool*)v.at(current).tool)->close();
    }

    current = id;
    selection.setX( id%width*Icon::size );
    selection.setY( id/width*Icon::size );

    /* connect the new tool to mouse events of editor */
    connect( view, SIGNAL(gfxEditorViewPressEvent(QPoint,int)),
             v.at(current).tool,   SLOT(gfxEditorToolPressEvent(QPoint,int)) );
    connect( view, SIGNAL(gfxEditorViewReleaseEvent(QPoint)),
             v.at(current).tool,   SLOT(gfxEditorToolReleaseEvent(QPoint)) );
    connect( view, SIGNAL(gfxEditorViewMoveEvent(QPoint)),
             v.at(current).tool,   SLOT(gfxEditorToolMoveEvent(QPoint)) );
    ((GfxEditorTool*)v.at(current).tool)->open();
}

 /*================================================================================*\
( *     G F X   E D I T O R   T O O L
 \*================================================================================*/

GfxEditorTool::GfxEditorTool(GfxImage *img, QObject *editor) :
    Tool(editor), img(img), mod(0), start(0,0)
{
    connect( this, SIGNAL(command(Command)),
             editor, SLOT(command(Command)) );
}
GfxEditorTool::~GfxEditorTool() {}
void GfxEditorTool::gfxEditorToolPressEvent  (QPoint /* unused */, int /* unused */) {}
void GfxEditorTool::gfxEditorToolReleaseEvent(QPoint /* unused */) {}
void GfxEditorTool::gfxEditorToolMoveEvent   (QPoint /* unused */) {}
void GfxEditorTool::open(void) {}
void GfxEditorTool::close(void) {}
