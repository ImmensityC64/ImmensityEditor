
#include "browser.h"

Browser::Browser(GfxVector *gv, QWidget *parent) :
    QMainWindow(parent)
{
    no_of_elements = 0;
    this->gv = gv;
    connect(gv, SIGNAL(vectorChanged()), this, SLOT(refresh()));

    QString title_str = GfxVector::Scope::Sketches == gv->scope() ? gv->title()+" Sketches" : gv->title()+" Scenery";
    setWindowTitle(title_str);

    status_bar = new QStatusBar(); /* QMainWindow will delete it */
    setStatusBar(status_bar);

    /* Widget and layout arrangement of the Browser window:
     * @ central widget
     * `-@ central layout
     *   `-@ scroll area
     *     `-@ scroll widget
     *       `-@ layout
     *         +-@ tile
     *         +-@ tile
     *         +-@ tile
     *         +-@ ...
     */
    central = new QWidget();
    setCentralWidget(central);
    central_layout = new QHBoxLayout();
    central->setLayout(central_layout);

    scroll_widget = new QWidget(this);
    scroll_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    scroll_area = new QScrollArea();
    scroll_area->setWidget(scroll_widget);
    central_layout->addWidget(scroll_area);

    int bar_width = qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent) + BrowserTileFrameWidth;

    /* 'ori' holds the orientation of the tiles, not the orientation of the browser! */
    if(Qt::Horizontal == gv->ori())
    {
        scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll_area->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        scroll_area->setMinimumWidth(BrowserTileSizeLong + bar_width);
        scroll_widget->setMinimumWidth(BrowserTileSizeLong);
        layout = new QVBoxLayout(scroll_widget);
    }
    else /* Qt::Vertical */
    {
        scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_area->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        scroll_area->setMinimumHeight(BrowserTileSizeLong + bar_width);
        scroll_widget->setMinimumHeight(BrowserTileSizeLong);
        layout = new QHBoxLayout(scroll_widget);
    }

    central_layout->setMargin(0);
    central_layout->setSpacing(0);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignLeft|Qt::AlignTop);

    if(GfxVector::Scope::Scenery == gv->scope())
        for(int i=0; i<gv->size(); i++)
            addGfxTile(i);

    refresh();
}
Browser::~Browser() {}

/* Refresh content of Browser window.
 * This function tries to refresh those elements only which have been changed.
 */
void Browser::refresh(void)
{
    shared_ptr<GfxData> data;
    BrowserTile *tile;
    int v_size = gv->size(); /* vector size */

    if(GfxVector::Scope::Sketches == gv->scope())
    {
        int l=0; /* count layout items */
        for(int v=0; v<v_size; v++) /* Iterate through on GfxVector */
        {
            data = gv->dataAt(v);

            /* Skip those elements which does not have data */
            if(data)
            {
                BrowserTile *tile = nullptr;
                bool found = false;

                /* Let's find the tile which holds that data! */
                while(!found && l < layout->count())
                {
                    /* Give my tile back! */
                    QLayoutItem *li = layout->itemAt(l);
                    tile = (BrowserTile *)li->widget();

                    if(tile->type() == BrowserTile::Type::Gfx && tile->index() == v)
                    {
                        /* We have just found the corresponding tile. Let's refresh it! */
                        found = true;
                        ((BrowserGfxTile *)tile)->img->refresh();
                        l++;
                    }
                    else
                    {
                        /* There are several options now:
                         * 1. It is the BrowserNewTile at the bottom of the layout.
                         * 2. The corresponding data was deleted for that tile. (tile->index() < v)
                         * 3. Something is really messed up.
                         * Let's delete this tile!
                         * Note that 'l' should not be increased here as layout count is decreased!
                         */
                        layout->takeAt(l);
                        delete tile;
                    }
                } /* while */

                /* No tile for that data, let's create one! */
                if(!found) { addGfxTile(v); l++; }

            } /* if(data) */
        } /* for */

        /* At this point, all vector element which has data has a corresponding tile too.
         * But it can happen that there was no such an element which has data at all.
         * So, let's clean up the rest of the layout items!
         * If we find a BrowserNewTile let's keep the first one to be at the bottom of the layout!
         */
        bool found = false;
        while(l < layout->count())
        {
            tile = (BrowserTile *)layout->itemAt(l);
            if(!found &&
               tile->type()  == BrowserTile::Type::New &&
               tile->index() == (int)GfxData::Id::Append)
            {
                found = true;
                l++;
            }
            else
            {
                layout->takeAt(l);
                delete tile;
            }
        }

        /* Add a BrowserNewTile at the end of global browser which is meant to be a new button.
         * DataVector will now that it must append a new element because of the GfxVectorIndexAppend index value.
         */
        if(!found) addNewTile((int)GfxData::Id::Append);
    }
    else /* GfxVector::Scope::Scenery */
    {
        /* Scenery tiles always have data and contains only GfxTiles.
         * It is enough to iterate through them and refresh all.
         */
        for(int v=0; v<v_size; v++) /* Iterate through on GfxVector */
        {
            QLayoutItem *li = layout->itemAt(v);
            tile = (BrowserTile *)li->widget();
            ((BrowserGfxTile *)tile)->img->refresh();
        } /* for */
    }

    /* Resize scroll_widget if number of elements has been changed */
    int layout_cnt = layout->count();
    if(layout_cnt != no_of_elements)
    {
        if(Qt::Horizontal == gv->ori()) scroll_widget->setMinimumHeight(layout_cnt*BrowserTileSizeShort);
        else /* Qt::Vertical */         scroll_widget->setMinimumWidth (layout_cnt*BrowserTileSizeShort);
        no_of_elements = layout_cnt;
    }
}

void Browser::refreshTile(int index)
{
    /* Let's find the tile which holds that data! */
    BrowserTile *tile = nullptr;
    for(int l=0; l<layout->count(); l++)
    {
        /* Give my tile back! */
        QLayoutItem *li = layout->itemAt(l);
        tile = (BrowserTile *)li->widget();

        if(tile->index() == index)
        {
            if(tile->type() == BrowserTile::Type::Gfx)
                /* We have just found the corresponding tile. Let's refresh it! */
                ((BrowserGfxTile *)tile)->img->refresh();
            break;
        }
    }
}

void Browser::addGfxTile(int index)
{
    BrowserGfxTile *tile = new BrowserGfxTile(index, gv->ori(), gv->dataAt(index), gv->mode());
    layout->addWidget((QWidget *) tile);
    connect(tile, SIGNAL(pressed(int)), this, SLOT(openEditor(int)));
}

void Browser::addNewTile(int index)
{
    BrowserNewTile *tile = new BrowserNewTile(index, gv->ori());
    layout->addWidget((QWidget *) tile);
    connect(tile, SIGNAL(pressed(int)), this, SLOT(createGfx(int)));
}

/* It is called when a BrowserNewTile is clicked. */
void Browser::createGfx(int index)
{
    shared_ptr<GfxData> data(new GfxData(gv->type()));
    gv->setDataAt(index, data); /* it automatically orders a refresh */
}

/* It is called when a BrowserGfxTile is clicked. */
void Browser::openEditor(int index)
{
    Editor *e = gv->editorAt(index);
    if(e) e->activateWindow();
    else
    {
        switch (gv->type()) {
        case GfxData::Type::Sprite:
        case GfxData::Type::Wall:
        {
            SpriteEditor *se = new SpriteEditor(gv->dataAt(index), index);
            if     (gv->mode() == GfxImage::Mode::Ceiling) se->changeDisplayMode(1);
            else if(gv->mode() == GfxImage::Mode::Floor)   se->changeDisplayMode(2);
            e = se;
            break;
        }
        case GfxData::Type::Sketch:
        {
            if(GfxVector::Scope::Sketches == gv->scope())
            {
                e = (Editor *)new BgSketchEditor(gv->dataAt(index), index);
                connect(e, SIGNAL(changesApplied(int)), this, SLOT(refreshTile(int)));
            }
            else
            {
                e = (Editor *)new BgTileEditor(gv->dataAt(index), index);
                /* TODO Scenery tiles follow different method on apply - changesApplied(int,shared_ptr<GfxData>) */
                cout << "TODO bg tile editor apply" << endl;
            }
            break;
        }
        case GfxData::Type::CnfSketch:
        {
            if(GfxVector::Scope::Sketches == gv->scope())
            {
                e = (Editor *)new CnfSketchEditor(gv->dataAt(index), index);
                connect(e, SIGNAL(changesApplied(int)), this, SLOT(refreshTile(int)));
            }
            else
            {
                e = (Editor *)new CnfTileEditor(gv->dataAt(index), index);
                /* TODO Scenery tiles follow different method on apply - changesApplied(int,shared_ptr<GfxData>) */
                cout << "TODO cnf tile editor apply" << endl;
            }
            break;
        }
        default:
            return;
        }
        e->setAttribute(Qt::WA_DeleteOnClose, true);
        e->show();
        connect(e, SIGNAL(destroyed(int)), gv, SLOT(closeEditorAt(int)));
        gv->setEditorAt(index, e);
    }
}

 /*================================================================================*\
( *     T I L E
 \*================================================================================*/

BrowserTile::BrowserTile(int index, Qt::Orientation ori, QWidget *parent) :
    QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    I = index;

    if(Qt::Horizontal == ori) setMinimumSize(BrowserTileSizeLong, BrowserTileSizeShort);
    else /* Qt::Vertical */   setMinimumSize(BrowserTileSizeShort, BrowserTileSizeLong);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
BrowserTile::~BrowserTile() {}

int BrowserTile::index(void) { return I; }
int BrowserTile::setIndex(int tI) { return I=tI; }
BrowserTile::Type BrowserTile::type(void) { return T; }

 /*================================================================================*\
( *     T I L E   -   G F X
 \*================================================================================*/

/* BrowserGfxTile displays a graphical object in read-only mode. */
BrowserGfxTile::BrowserGfxTile(int index,
                               Qt::Orientation ori,
                               shared_ptr<GfxData> src,
                               GfxImage::Mode mode,
                               QWidget *parent) :
    BrowserTile(index, ori, parent)
{
    T = Type::Gfx;

    GfxBrowserView *view = new GfxBrowserView(this);
    qreal w, h;

    if(Qt::Horizontal == ori)
    {
        layout = new QHBoxLayout();
        w = BrowserTileSizeLong;
        h = BrowserTileSizeShort;
    }
    else /* Qt::Vertical */
    {
        layout = new QVBoxLayout();
        w = BrowserTileSizeShort;
        h = BrowserTileSizeLong;
    }
    setLayout(layout); /* 'this' will delete 'layout' */
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(view);

    qreal x_factor = (w-BrowserTileFrameWidth) / src->imgW();
    qreal y_factor = (h-BrowserTileFrameWidth) / src->imgH();
    qreal factor = x_factor<y_factor ? x_factor : y_factor;
    view->scale(factor, factor);

    rect = new GfxRectItemImage(src);      /* 'view->scene' will delete it */
    img  = rect->getImg();                 /*        'rect' will delete it */
    img->setMask(true);
    img->setMode(mode);
    bg   = new GfxRectItemBackground(src); /* 'view->scene' will delete it */
    bg->setZValue(-1);

    view->scene->addItem(rect);
    view->scene->addItem(bg);
    view->setSceneRect(0,0,src->imgW(),src->imgH());
    view->background->setRect(0,0,src->imgW(),src->imgH());

    connect(img, SIGNAL(refreshHappened()), rect, SLOT(refresh()));
    connect(img, SIGNAL(refreshHappened()), bg,   SLOT(refresh()));

    img->refresh();
}
BrowserGfxTile::~BrowserGfxTile() {}

void BrowserGfxTile::mousePressEvent(QMouseEvent *event)
{
    emit pressed(I);
}

 /*================================================================================*\
( *     T I L E   -   N E W
 \*================================================================================*/

/* BrowserNewTile is functioning as a 'new' button to create and edit new graphical objects */
BrowserNewTile::BrowserNewTile(int index, Qt::Orientation ori, QWidget *parent) :
    BrowserTile(index, ori, parent)
{
    T = Type::New;
    layout = new QHBoxLayout();
    setLayout(layout); /* 'this' will delete 'layout' */

    /* TODO: display a 'new' icon in 'layout' */
}
BrowserNewTile::~BrowserNewTile() {}

void BrowserNewTile::mousePressEvent(QMouseEvent *event)
{
    emit pressed(I);
}
