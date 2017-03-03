
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
quint32 GfxData::dataDiffersCrc(void) { return crc; }
quint32 GfxData::imageEqualsCnt(void) { return cnt; }

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

quint32 GfxData::static_cnt = 0;

void GfxData::init(Type tType, int tWidth, int tHeight)
{
    setType(tType);
    crc = 0;
    cnt = 0;

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

void GfxData::loadNewVersion(shared_ptr<GfxData> src)
{
    load(src);
    calcCrc();
}

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

quint32 GfxData::crc_tbl[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535,
    0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD,
    0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D,
    0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4,
    0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
    0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC,
    0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB,
    0xB6662D3D, 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
    0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB,
    0x086D3D2D, 0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA,
    0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, 0x4DB26158, 0x3AB551CE,
    0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A,
    0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409,
    0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
    0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739,
    0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344, 0x8708A3D2, 0x1E01F268,
    0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0,
    0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8,
    0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF,
    0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703,
    0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7,
    0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE,
    0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
    0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6,
    0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D,
    0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5,
    0x47B2CF7F, 0x30B5FFE9, 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605,
    0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

void GfxData::calcCrc(void)
{
    quint32 tCrc = 0xFFFFFFFF;
    quint32 byte;
    int cnt8;

    /* Calculate CRC of 'bitmap' */
    for(int y=0; y<bitmap->size(); y++)
    {
        cnt8 = 8;
        byte = 0;
        for(int b=0; b<bitmap->at(y)->count();++b)
        {
            byte |= (bitmap->at(y)->at(b)?1:0)<<(b%8);
            cnt8--;
            if(!cnt8)
            {
                tCrc = calc_crc_next_byte(tCrc, byte);
                cnt8 = 8;
                byte = 0;
            }
        }
    }

    /* Calculate CRC of 'clrmap' */
    for(int y=0; y<clrmap->size(); y++)
        for(int x=0; x<clrmap->at(y)->size(); x++)
            tCrc = calc_crc_next_byte(tCrc, (quint32)clrmap->at(y)->at(x));

    /* 'colors' are not part of CRC32 calculation.
     * CRC32 is used to fast recognize whether a sprite being
     * just imported to a scenery is identical to an
     * existing one. We do not care about individual color
     * values in a scenery hence the map overrides
     * them. So, we do not consider color values.
     */
#if 0
    /* Calculate CRC of 'colors' */
    for(int i=0; i<(int)GfxData::ColorIndex::Size; i++)
    {
        byte = (quint32)colors[i];
        crc = calc_crc_next_byte(crc, byte);
    }
#endif

    crc = tCrc;
    static_cnt++;
    cnt = static_cnt;
}

quint32 GfxData::calc_crc_next_byte(quint32 crc, quint32 byte)
{
    return (crc >> 8) ^ crc_tbl[(crc ^ byte) & 0xFF];
}

/* Uses 'crc' to decide if 'src' and 'this' are different.
 * Returns true if 'crc' values are different which means
 * that the two graphical objects are surely different.
 * However, false does not mean that they are equal!
 * Note that 'colors' is NOT considered!
 */
bool GfxData::dataDiffers(shared_ptr<GfxData> src) { return (crc != src->crc); }
bool GfxData::dataDiffers(quint32 tCrc)            { return (crc != tCrc);     }

/* Uses 'cnt' to decide if 'src' and 'this' are equal.
 * Returns true if 'cnt' values are equal which means
 * that the two graphical objects are surely the same.
 * However, false does not mean that they are different!
 * Note that 'colors' is considered!
 */
bool GfxData::imageEquals(shared_ptr<GfxData> src) { return (cnt == src->cnt); }
bool GfxData::imageEquals(quint32 tCnt)            { return (cnt == tCnt);     }

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
