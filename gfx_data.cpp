
#include "gfx.h"

 /*================================================================================*\
( *     G F X   D A T A   -   U S E F U L
 \*================================================================================*/

GfxData::Type GfxData::type(void) { return T; }
GfxData::Type GfxData::setType(Type tT)
{
    T = tT;
    CM = get_mode();
    return T;
}
GfxData::PixelMode GfxData::colorMode(void) { return CM; }

int GfxData::clrSize(int s) { return (s+7)/8; } /* convert to colormap size */
int GfxData::bitW(void) { return W; }           /* bitmap width  */
int GfxData::bitH(void) { return H; }           /* bitmap height */
int GfxData::clrW(void) { return clrSize(W); }  /* colormap width  */
int GfxData::clrH(void) { return clrSize(H); }  /* colormap height */

int GfxData::imgW(void) /* image width (usually the double of bitmap width) */
{
    /* Image for Ceiling or Floor sprite requires
     * 4 pixels more space because of the
     * perspectivic distortion.
     */
    switch(T)
    {
    case Type::Sprite: return 52; /* 24*2 + 4 */
    case Type::Wall: return W*2;
    default: return W;
    }
}
int GfxData::imgH(void) { return H; } /* image height */

int GfxData::bit2clr(int coord) { return coord/8; } /* convert coordinate */
int GfxData::clr2bit(int coord) { return coord*8; } /* convert coordinate */
QPoint GfxData::bit2clr(QPoint p) { return QPoint(bit2clr(p.x()),bit2clr(p.y())); }
QPoint GfxData::clr2bit(QPoint p) { return QPoint(clr2bit(p.x()),clr2bit(p.y())); }

bool GfxData::validColor(int i) { return (i<0||i>=(int)ColorIndex::Size) ? false : true; }
bool GfxData::validBitCoord(int x, int y) { return (x<0||y<0||y>=H||x>=W) ? false : true; }
bool GfxData::validClrCoord(int x, int y) { return (x<0||y<0||y>=clrH()||x>=clrW()) ? false : true; }
void GfxData::validateSelection(int &x, int &y)
{
    if(x>=W) x=W-1; else if(x<0) x=0;
    if(y>=H) y=H-1; else if(y<0) y=0;
}
void GfxData::validateSelection(int &ax, int &ay, int &cx, int &cy)
{
    int tmp;
    if(ax>cx) { tmp=ax; ax=cx; cx=tmp; }
    if(ay>cy) { tmp=ay; ay=cy; cy=tmp; }
    if(Type::Sketch==T || Type::CnfSketch==T)
    {
        ax&=-8; ay&=-8; cx|=7; cy|=7;
    }
    validateSelection(ax,ay);
    validateSelection(cx,cy);
}
void GfxData::validateSelectionStart(int &ax, int &ay)
{
    if(Type::Sketch==T || Type::CnfSketch==T)
    {
        ax&=-8; ay&=-8;
    }
}

 /*================================================================================*\
( *     G F X   D A T A   -   I N I T
 \*================================================================================*/

void GfxData::init(Type tType, int tWidth, int tHeight)
{
    setType(tType);

    if(tWidth && tHeight)
    {
        W = tWidth;
        H = tHeight;
    }
    else /* Size is not specified => let's find it out from type! */
    {
        switch(T)
        {
        case Type::Sprite:
        {
            W = 1 * C64::SpriteWidth;
            H = 1 * C64::SpriteHeight;
            break;
        }
        case Type::Wall:
        {
            W = 1 * C64::SpriteWidth;
            H = 5 * C64::SpriteHeight;
            break;
        }
        case Type::Enemy:
        {
            W = 3 * C64::SpriteWidth;
            H = 3 * C64::SpriteHeight;
            break;
        }
        case Type::Player:
        case Type::Cyber:
        {
            W = 1 * C64::SpriteWidth;
            H = 2 * C64::SpriteHeight;
            break;
        }
        case Type::Sketch:
        {
            W = 40 * 8;
            H = 24 * 8;
            break;
        }
        case Type::CnfSketch:
        {
            W = 36 * 8;
            H = 24 * 8;
            break;
        }
        case Type::CharSet:
        {
            W = 8 * C64::CharSetCols;
            H = 8 * C64::CharSetRows;
            break;
        }
        case Type::Nothing:
        default:
        {
            /* Let's use what we have got,
             * perhaps not both of them are zero!
             */
            W = tWidth;
            H = tHeight;
            break;
        }
        } /* switch(T) */
    }

    for(quint8 i=0; i<(int)ColorIndex::Size; i++) { colors[i] = i&0xF; }
    bitmap = new QVector<QBitArray*>(H);            /* destructor will delete it */
    clrmap = new QVector<QVector<quint8>*>(clrH()); /* destructor will delete it */
    for(int i=0; i<bitmap->size(); i++) { bitmap->data()[i] = new QBitArray(W); }
    for(int i=0; i<clrmap->size(); i++) { clrmap->data()[i] = new QVector<quint8>(clrW()); }

} /* GfxData::init */

 /*================================================================================*\
( *     G F X   D A T A   -   R E S I Z E
 \*================================================================================*/

void GfxData::resize(int tW, int tH) /* width, height */
{
    if(H < tH)
    {
        /* enlarge vector */
        bitmap->resize(tH);
        clrmap->resize(clrSize(tH));
        /* create new elements with new size */
        for(int i=H;      i<tH;          i++) { bitmap->data()[i] = new QBitArray(tW); }
        for(int i=clrH(); i<clrSize(tH); i++) { clrmap->data()[i] = new QVector<quint8>(clrSize(tW)); }
        /* resize old elements */
        if(W != tW)
        {
            for(int i=0; i<H;      i++) { bitmap->at(i)->resize(tW); }
            for(int i=0; i<clrH(); i++) { clrmap->at(i)->resize(clrSize(tW)); }
        }
    }
    else /* tHeight <= height */
    {
        if(tH < H)
        {
            /* delete superfluous elements */
            for(int i=tH;          i<bitmap->size(); i++) { delete bitmap->at(i); }
            for(int i=clrSize(tH); i<clrmap->size(); i++) { delete clrmap->at(i); }
            /* shrink vector */
            bitmap->resize(tH);
            clrmap->resize(clrSize(tH));
        }
        /* resize elements */
        if(W != tW)
        {
            for(int i=0; i<bitmap->size(); i++) { bitmap->at(i)->resize(tW); }
            for(int i=0; i<clrmap->size(); i++) { clrmap->at(i)->resize(clrSize(tW)); }
        }
    }

    /* store new size */
    W = tW;
    H = tH;

} /* GfxData::resize */

 /*================================================================================*\
( *     G F X   D A T A   -   L O A D
 \*================================================================================*/

void GfxData::load(shared_ptr<GfxData> src)
{
    resize(0,0); /* clear old data */

    /* load type, id and size */
    setType(src->T);
    W = src->W;
    H = src->H;

    /* load colors */
    for(int i=0; i<(int)ColorIndex::Size; i++) { colors[i] = src->colors[i]; }

    /* load bitmap */
    bitmap->resize(H);
    for(int i=0; i<H; ++i)
        bitmap->data()[i] = new QBitArray( *( src->bitmap->at(i) ) );

    /* load clrmap */
    clrmap->resize(clrH());
    for(int i=0; i<clrH(); ++i)
        clrmap->data()[i] = new QVector<quint8>( *( src->clrmap->at(i) ) );
} /* GfxData::load */

void GfxData::paste(shared_ptr<GfxData> src, QPoint p)
{
    paste(src,p.x(),p.y());
}

void GfxData::paste(shared_ptr<GfxData> src, int px, int py)
{
    /* Do not load foreign color settings! */

    /* load bitmap */
    int h=src->H; int w=src->W;
    for(int y=0; y<h; y++)
    {
        int ty = y+py;
        for(int x=0; x<w; x++)
        {
            int tx = x+px;
            if(validBitCoord(tx,ty))
            {
                bitmap->at(ty)->setBit(tx,src->bitmap->at(y)->at(x));
            }
        }
    }

    /* Do not load clrmap of a stranger type! */
    if(src->T == T)
    {
        /* load clrmap */
        w=clrSize(w); h=clrSize(h);
        px=bit2clr(px); py=bit2clr(py);
        for(int y=0; y<h; y++)
        {
            int ty = y+py;
            for(int x=0; x<w; x++)
            {
                int tx = x+px;
                if(validClrCoord(tx,ty))
                {
                    clrmap->at(ty)->data()[tx] = src->clrmap->at(y)->at(x);
                }
            }
        }
    }
}

 /*================================================================================*\
( *     G F X   D A T A   -   C O N S T R U C T O R S
 \*================================================================================*/

GfxData::GfxData()                                    { init(); }
GfxData::GfxData(Type tType)                          { init(tType); }
GfxData::GfxData(Type tType, int tWidth, int tHeight) { init(tType, tWidth, tHeight); }

GfxData::GfxData(shared_ptr<GfxData> src) { init(); load(src); }

/* Create GfxData for a character */
GfxData::GfxData(quint64 chr, Type tType)
{
    init(tType, 8, 8);
    quint64 mask = 0x8000000000000000;
    for(int x=0; x<8; x++)
    for(int y=0; y<8; y++)
    {
        if(chr & mask) bitmap->at(y)->setBit(x,true);
        mask >>= 1;
    }
}

/* Get character data from image */
quint64 GfxData::chr(int cx, int cy)
{
    int px = clr2bit(cx);
    int py = clr2bit(cy);
    quint64 ret = 0;
    quint64 mask = 0x8000000000000000;
    for(int x=px; x<px+8; x++)
        for(int y=py; y<py+8; y++)
        {
            if(bitmap->at(y)->at(x)) ret |= mask;
            mask >>= 1;
        }
    return ret;
}

shared_ptr<GfxData> GfxData::copy(void)
{
    shared_ptr<GfxData> ret( new GfxData(shared_from_this()) );
    return ret;
}

shared_ptr<GfxData> GfxData::copy(QPoint a, QPoint c)
{
    return copy(a.x(),a.y(),c.x(),c.y());
}

shared_ptr<GfxData> GfxData::copy(int ax, int ay, int cx, int cy)
{
    validateSelection(ax,ay,cx,cy);
    int w=cx+1-ax; int h=cy+1-ay;
    shared_ptr<GfxData> data(new GfxData(T,w,h));

    /* load colors */
    for(int i=0; i<(int)ColorIndex::Size; i++) { data->colors[i] = colors[i]; }

    /* load bitmap */
    for(int y=0; y<h; y++)
    {
        int sy=ay+y; // source Y
        for(int x=0; x<w; x++)
            data->bitmap->at(y)->setBit(x,bitmap->at(sy)->at(ax+x));
    }

    /* load clrmap */
    w=clrSize(w); h=clrSize(h);
    ax=bit2clr(ax); ay=bit2clr(ay);
    for(int y=0; y<h; y++)
    {
        int sy=ay+y; // source Y
        for(int x=0; x<w; x++)
            data->clrmap->at(y)->data()[x] = clrmap->at(sy)->at(ax+x);
    }

    return data;
}

GfxData::~GfxData()
{
    for(int i=0; i<bitmap->size(); i++) { delete bitmap->at(i); }
    for(int i=0; i<clrmap->size(); i++) { delete clrmap->at(i); }
    delete bitmap; /* was created in GfxData::init */
    delete clrmap; /* was created in GfxData::init */
}

 /*================================================================================*\
( *     G F X   D A T A   -   I N T E R F A C E
 \*================================================================================*/

quint8 GfxData::color(int i)
{
    return validColor(i) ? colors[i] : 0;
}
quint8 GfxData::setColor(int i, quint8 v)
{
    if(!validColor(i)) return v;
    colors[i] = v;
    return v;
}

/* get/set bitmap value */
quint8 GfxData::bitVal(int x, int y)
{
    quint8 v=0;
    if(get_mc(x,y))
    {
        x &= -2;
        if(validBitCoord(x,y)) v = (quint8)bitmap->at(y)->at(x);
        v *= 2;
        x++;
        if(validBitCoord(x,y)) v |= (quint8)bitmap->at(y)->at(x);
    }
    else
    {
        if(validBitCoord(x,y)) v = (quint8)bitmap->at(y)->at(x);
    }
    return v;
}
quint8 GfxData::setBitVal(int x, int y, quint8 v)
{
    if(get_mc(x,y))
    {
        /* MC Char and MC Sprite pixel colors are represented on different ways.
         * MC Sprite is supported by default.
         * Let's rectify the value for MC Char!
         */
        if(PixelMode::ClrMap==CM && v&1) v^=2;

        x &= -2;
        if(validBitCoord(x,y)) bitmap->at(y)->setBit(x,(bool)(v&2));
        x++;
        if(validBitCoord(x,y)) bitmap->at(y)->setBit(x,(bool)(v&1));
    }
    else
    {
        if(validBitCoord(x,y)) bitmap->at(y)->setBit(x,(bool)(v&1));
    }
    return v;
}
quint8 GfxData::   bitVal(QPoint p)           { return    bitVal(p.x(), p.y()   ); }
quint8 GfxData::setBitVal(QPoint p, quint8 v) { return setBitVal(p.x(), p.y(), v); }

/* get/set clrmap value */
quint8 GfxData::clrVal(int x, int y)
{
    quint8 v=0;
    if(validClrCoord(x,y)) v = clrmap->at(y)->at(x);
    return v;
}
quint8 GfxData::setClrVal(int x, int y, quint8 v)
{
    if(validClrCoord(x,y)) clrmap->at(y)->data()[x] = v;
    return v;
}
quint8 GfxData::   clrVal(QPoint p)           { return    clrVal(p.x(),p.y()   ); }
quint8 GfxData::setClrVal(QPoint p, quint8 v) { return setClrVal(p.x(),p.y(), v); }

 /*================================================================================*\
( *     G F X   D A T A   -   O T H E R
 \*================================================================================*/

GfxData::PixelMode GfxData::get_mode(void)
{
    switch(T)
    {
    case Type::Enemy:
    case Type::Cyber:
        return PixelMode::Multi;
    case Type::Sketch:
        return PixelMode::ClrMap;
    case Type::Player:
        return PixelMode::HiRes;
    case Type::Sprite:
    case Type::Wall:
    case Type::CnfSketch:
    case Type::CharSet:
    case Type::Nothing:
    default:
        return PixelMode::HiRes;
    }
}

bool GfxData::get_mc(int x, int y)
{
    if(PixelMode::Multi  == CM) return true;
    if(PixelMode::ClrMap == CM
            && 7 < clrVal(x/8, y/8) )   return true;
    return false;
}

 /*================================================================================*\
( *     G F X   D A T A   -   H I S T O R Y
 \*================================================================================*/

void GfxHistory::inc(int &v) { v++; if(size()<=v) v=0; }
void GfxHistory::dec(int &v) { if(v<=0) v=size(); v--; }
void GfxHistory::clearData(int &v)
{
    data()[v].reset();
    inc(v);
}

GfxHistory::GfxHistory(shared_ptr<GfxData> init) :
    QVector( Cfg::ins().val(Cfg::GfxEdHistoryDepth), nullptr )
{
    o = 0;
    c = 0;
    data()[0]=init->copy(); /* we need at least one saved data */
}
GfxHistory::~GfxHistory()
{
    /* delete all elements starting from the oldest one
     * until we reach the first empty slot
     */
    while(at(o)) clearData(o);
}

void GfxHistory::load(shared_ptr<GfxData> dst)
{
    dst->load(at(c)); /* load current data to destination */
}

void GfxHistory::save(shared_ptr<GfxData> const src)
{
    inc(c);

    /* save source data into current element if it exists */
    if(at(c)) { at(c)->load(src); }
    /* create a new element and save a copy of source otherwise */
    else { data()[c]=src->copy(); }

    if(c==o)
    { /* we have just overriden the oldest element */
        inc(o);
    }
    else
    { /* delete following elements which contains obselete data */
        int t=c;
        inc(t);
        while(t!=o && at(t)) clearData(t);
    }
}

void GfxHistory::undo(shared_ptr<GfxData> dst)
{
    /* decrease 'c' only if we have not reached the oldest element yet */
    if(c!=o) dec(c);
    dst->load(at(c)); /* load current data to destination */
}

void GfxHistory::redo(shared_ptr<GfxData> dst)
{
    /* increase 'c' only if the next element is valid */
    int t=c;
    inc(t);
    if(t!=o && at(t)) c=t;
    dst->load(at(c)); /* load current data to destination */
}
