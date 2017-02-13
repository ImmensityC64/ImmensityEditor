#include "grid.h"

Grid::Grid(GfxEditorView *view, int width, int height) : QObject(),
    view(view), width(width), height(height), dependsOnScale(false)
{
    fineGrp = view->scene->createItemGroup(items);
    mainGrp = view->scene->createItemGroup(items);
    fineGrp->setZValue(11);
    mainGrp->setZValue(12);
    vrfyVisibility();
}

Grid::~Grid()
{
    clear();
}

void Grid::clear()
{
    for(int i=0; i<items.size(); i++)
    {
        fineGrp->removeFromGroup(items.at(i));
        mainGrp->removeFromGroup(items.at(i));
        delete items.at(i);
    }
    items.clear();
}

void Grid::fineVisible(int state) { fineState = state; vrfyVisibility(); }
void Grid::mainVisible(int state) { mainState = state; vrfyVisibility(); }

void Grid::vrfyVisibility(void)
{
    if( fineState )
    {
        if( dependsOnScale )
        {
            qreal scale = view->transform().m11();
            if(scale >= 4) fineGrp->show();
            else           fineGrp->hide();
        }
        else
            fineGrp->show();
    }
    else
        fineGrp->hide();

    if( mainState )
        mainGrp->show();
    else
        mainGrp->hide();
}

void Grid::createSeries(Grid::Type type,
                        int stpX, int stpY,
                        int begX, int begY,
                        int endX, int endY)
{
    if(-1==endX) endX = width;
    if(-1==endY) endY = height;
    if(Type::Fine==type)
    {
        for(int y=begY; y<=endY; y+=stpY)
            createFineLine(begX,y,endX,y);
        for(int x=begX; x<=endX; x+=stpX)
            createFineLine(x,begY,x,endY);
    }
    else /* Type::Main */
    {
        for(int y=begY; y<=endY; y+=stpY)
            createMainLine(begX,y,endX,y);
        for(int x=begX; x<=endX; x+=stpX)
            createMainLine(x,begY,x,endY);
    }
}

void Grid::createFineLine(int x0, int y0, int x1, int y1)
{
    createLine(x0,y0,x1,y1, Cfg::ins().val(Cfg::EditorFineGridColor), Qt::DotLine );
    fineGrp->addToGroup(items.back());
}
void Grid::createMainLine(int x0, int y0, int x1, int y1)
{
    createLine(x0,y0,x1,y1, Cfg::ins().val(Cfg::EditorMainGridColor), Qt::SolidLine );
    mainGrp->addToGroup(items.back());
}
void Grid::createMainRect(int x, int y, int w, int h)
{
    createRect(x,y,w,h, Cfg::ins().val(Cfg::EditorMainGridOutland) );
    mainGrp->addToGroup(items.back());
}

void Grid::createLine(int x0, int y0, int x1, int y1, int color, int style)
{
    QGraphicsLineItem *line;
    QPen pen = QPen(QColor::fromRgba( color ));
    pen.setWidth(0);
    pen.setStyle( (Qt::PenStyle)style );
    line = new QGraphicsLineItem(x0,y0,x1,y1);
    line->setPen(pen);
    items.push_back(line);
}
void Grid::createRect(int x, int y, int w, int h, int color)
{
    QGraphicsRectItem *rect;
    rect = new QGraphicsRectItem(x,y,w,h);
    rect->setPen(QPen(Qt::NoPen));
    rect->setBrush( QBrush(QColor::fromRgba( color )) );
    items.push_back(rect);
}
