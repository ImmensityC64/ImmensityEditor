
#include "gfx.h"

 /*================================================================================*\
( *     G F X   I M A G E
 \*================================================================================*/

/* GfxImage displays an image described by the data stored by a given GfxData.
 * GfxImage supports different display modes, see Mode!
 * GfxImage provides an interface towards GfxData and hides the differencies
 * of data handling between different display modes.
 * GfxImage is set as a brush for GfxRectItemImage.
 */

GfxImage::GfxImage(shared_ptr<GfxData> src) :
    QImage(src->imgW(), src->imgH(), QImage::Format_Indexed8)
{ init(src); }

   /* Special constructor for sprites placed on the edge of the screen */
GfxImage::GfxImage(shared_ptr<GfxData> src, int tP) :
    QImage(64 /* 24*2 + 16 */, src->imgH(), QImage::Format_Indexed8)
{ init(src); P = tP; }

GfxImage::~GfxImage() { }

void GfxImage::init(shared_ptr<GfxData> src)
{
    setColorTable(C64::ins().color_table());
    fill(C64::IndexAlpha);
    T = src->type();

    switch(T) /* find the proper display mode */
    {
    case GfxData::Type::Wall:      M = Mode::Wall;      break;
    case GfxData::Type::Enemy:     M = Mode::Enemy;     break;
    case GfxData::Type::Player:    M = Mode::Player;    break;
    case GfxData::Type::Cyber:     M = Mode::Cyber;     break;
    case GfxData::Type::Sketch:    M = Mode::Sketch;    break;
    case GfxData::Type::CnfSketch: M = Mode::CnfSketch; break;
    case GfxData::Type::CharSet:   M = Mode::HiRes;     break;
    case GfxData::Type::Nothing:   M = Mode::Nothing;   break;
    default:
    case GfxData::Type::Sprite:    M = Mode::Expanded;  break;
    }

    setData(src);
    selSrcA = QPoint(0,0);
    selSrcC = QPoint(0,0);
}

 /*================================================================================*\
( *     S E T T I N G S
 \*================================================================================*/

GfxImage::Mode GfxImage::mode(void) { return M; }
GfxImage::Mode GfxImage::setMode(Mode tM)
{
        /* Display mode can be changed in case of sprites only. */
    if( GfxData::Type::Sprite == T &&
        /* Expanded, Ceiling and Floor are the valid display modes for a sprite. */
        ( Mode::Expanded == tM || Mode::Ceiling == tM || Mode::Floor == tM ) &&
        /* Is it really changed? */
        tM != M )
    {
        M = tM;
        refresh();
    }
    return M;
}

bool GfxImage::mask(void) { return SM; }
bool GfxImage::setMask(bool tSM) { return SM = tSM; }

void GfxImage::setData(shared_ptr<GfxData> tD)
{
    if(tD->type() != T) D.reset();
    else D = tD; /* Accept data with same type only */
    refresh();
}

 /*================================================================================*\
( *     D I S P L A Y
 \*================================================================================*/

void GfxImage::refresh(void)
{
    int iW = width();  /* image width  */
    int iH = height(); /* image height */

    shared_ptr<GfxData> tD = D.lock(); /* data */

    if(tD) /* data exists */
    {
        switch(M) /* display mode */
        {
        case Mode::Expanded:
        case Mode::Ceiling:
        case Mode::Floor:
        case Mode::Wall:
        {
            /* Data width is not proportional to image width
                * in case of perspective display of sprites.
                */
            int dW = tD->bitW();
            /* pixel color outside of sprite */
            int oc = SM ? (int)C64::IndexScene : (int)C64::IndexAlpha;
            /* sprite pixel color */
            int sc = (int)tD->color((int)GfxData::ColorIndex::Color);

            /* This ugly code below draws an expanded sprite on the QImage.
                * It considers perspective of Ceiling and Floor.
                */
            for(int y=0; y<iH; y++)
            {
                int offset=hor_offset(y); /* horizontal offset */
                int x=0, d=0, exp=0, c=(int)C64::IndexAlpha;
                for(; x<offset && x<iW; x++)
                    setPixel(x,y,oc); /* outside of sprite */
                for(; x<iW && d<dW; x++)
                { /* inside of sprite */
                    if(!exp) /* expanded pixels */
                    {
                        /* set pixel color */
                        c = tD->bitVal(d,y) ? sc : (int)C64::IndexAlpha;
                    }
                    else d++; /* step data ptr */
                    exp ^= 1; /* toggle 'exp' */
                    setPixel(x,y,c);
                }
                for(; x<iW; x++)
                    setPixel(x,y,oc); /* outside of sprite */
            }
            break; /* Expanded, Ceiling, Floor, Wall */
        }
        case Mode::Enemy:
        case Mode::Cyber:
        {
            break; /* Enemy, Cyber */
        }
        case Mode::Player:
        {
            break; /* Player */
        }
        case Mode::Sketch:
        {
            int alp = (int)C64::IndexAlpha;
            int cm0 = (int)tD->color((int)GfxData::ColorIndex::Comm0);
            int cm1 = (int)tD->color((int)GfxData::ColorIndex::Comm1);

            for(int cy=0; cy<iH; cy+=8)
            {
                for(int cx=0; cx<iW; cx+=8)
                {
                    int clr = tD->clrVal(tD->bit2clr(cx),tD->bit2clr(cy));
                    if(clr & 8) /* multicolor mode */
                    {
                        for(int py=cy; py<cy+8; py++)
                        {
                            int exp=0;
                            int c=alp;
                            for(int px=cx; px<cx+8; px++)
                            {
                                if(!exp)
                                {
                                    switch(tD->bitVal(px,py))
                                    {
                                    default:
                                    case 0: c = alp; break;
                                    case 1: c = cm0; break;
                                    case 2: c = cm1; break;
                                    case 3: c = clr&7; break;
                                    }
                                }
                                exp ^= 1; /* toggle 'exp' */
                                setPixel(px,py,c);
                            }
                        }
                    }
                    else /* hires mode */
                    {
                        for(int py=cy; py<cy+8; py++)
                            for(int px=cx; px<cx+8; px++)
                                setPixel(px,py,tD->bitVal(px,py) ? clr : alp);
                    }
                } /* for(cx) */
            } /* for(cy) */
            break; /* Sketch */
        }
        case Mode::CnfSketch:
        {
            /* I decided not to use alpha pixels in ECM.
             * Although color is read from the color memory,
             * the whole ceiling area has the same color
             * because there is not enough CPU time to refresh
             * the colors character by character.
             * 'Color' represents the color of the whole ceiling area.
             * 'ECM0-ECM3' represent the four background colors.
             * Although in C64 ECM0 is the standard background color,
             * I handle them separatly in the editor, thus
             * GfxData::ColorIndex::ECM0 is not equal to
             * GfxData::ColorIndex::Backg.
             */
            int clr = (int)tD->color((int)GfxData::ColorIndex::Color);
            int e0  = (int)tD->color((int)GfxData::ColorIndex::ECM0);
            int e1  = (int)tD->color((int)GfxData::ColorIndex::ECM1);
            int e2  = (int)tD->color((int)GfxData::ColorIndex::ECM2);
            int e3  = (int)tD->color((int)GfxData::ColorIndex::ECM3);

            for(int cy=0; cy<iH; cy+=8)
            {
                for(int cx=0; cx<iW; cx+=8)
                {
                    int bg;
                    switch(tD->clrVal(tD->bit2clr(cx),tD->bit2clr(cy)))
                    {
                    default:
                    case (quint8)GfxData::ColorValue::ECM0: bg=e0; break;
                    case (quint8)GfxData::ColorValue::ECM1: bg=e1; break;
                    case (quint8)GfxData::ColorValue::ECM2: bg=e2; break;
                    case (quint8)GfxData::ColorValue::ECM3: bg=e3; break;
                    }
                    for(int py=cy; py<cy+8; py++)
                        for(int px=cx; px<cx+8; px++)
                            setPixel(px,py,tD->bitVal(px,py) ? clr : bg);
                } /* for(cx) */
            } /* for(cy) */
            break; /* SketchCnF */
        }
        case Mode::HiRes:
        case Mode::Nothing:
        default:
        {
            int alp = (int)C64::IndexAlpha;
            for(int cy=0; cy<iH; cy+=8)
            for(int cx=0; cx<iW; cx+=8)
            {
                int clr = tD->clrVal(tD->bit2clr(cx),tD->bit2clr(cy));
                for(int py=cy; py<cy+8; py++)
                for(int px=cx; px<cx+8; px++)
                    setPixel(px,py,tD->bitVal(px,py) ? clr : alp);
            }
            break;
        } /* HiRes, Nothing, default */
        } /* switch(mode) */

        cnt = tD->imageEqualsCnt();

    } /* if(tD) */
    else /* data does not exist */
    {
        /* Set IndexScene and IndexNoData colors using checked pattern. */
        for(int y=0; y<iH; y++)
            for(int x=0; x<iW; x++)
                setPixel(x,y, (int)C64::IndexScene + ((x^y)&1) );
        cnt = 0;
    }

    /* Notify GfxRectItemImage and GfxRectItemBackground
     * about a refresh has just happened.
     */
    emit refreshHappened();

} /* GfxImage::refresh() */

void GfxImage::refreshIfImageNotEquals(void)
{
    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD && tD->imageEquals(cnt)) return;
    refresh();
}


 /*================================================================================*\
( *     C O N V E R T E R S
 \*================================================================================*/


/* convert image coordinates to data coordinates */
QPoint GfxImage::img2dat(QPoint i)
{
    int iX = i.x();
    int iY = i.y();
    int dX = iX;
    int dY = iY;

    switch(M) /* display mode */
    {
    case Mode::Expanded:
    case Mode::Ceiling:
    case Mode::Floor:
    case Mode::Wall:
    {
        /* These are perspective display modes,
         * so the horizontal offset should be
         * subtracted first.
         * All of these modes use expanded sprites,
         * so we have to divide the coordinate with 2.
         */
        dX = (iX-hor_offset(iY))/2;
        break; /* Expanded, Ceiling, Floor, Wall */
    }
    case Mode::Enemy:
    case Mode::Cyber:
    {
        break; /* Enemy, Cyber */
    }
    case Mode::Player:
    {
        break; /* Player */
    }
    case Mode::Sketch:
    case Mode::CnfSketch:
    default:
    {
        /* No correction is needed */
        break; /* Sketch, SketchCnF, default */
    }
    } /* switch(mode) */

    return QPoint(dX,dY);

} /* GfxImage::img2dat() */

/* convert data coordinates to image coordinates */
QPoint GfxImage::dat2img(QPoint d)
{
    int dX = d.x();
    int dY = d.y();
    int iX = dX;
    int iY = dY;

    switch(M) /* display mode */
    {
    case Mode::Expanded:
    case Mode::Ceiling:
    case Mode::Floor:
    case Mode::Wall:
    {
        /* All of these modes use expanded sprites,
         * so we have to multiply the coordinate with 2.
         * These are perspective display modes,
         * so the horizontal offset should be added.
         */
        iX = dX*2+hor_offset(dY);
        break; /* Expanded, Ceiling, Floor, Wall */
    }
    case Mode::Enemy:
    case Mode::Cyber:
    {
        break; /* Enemy, Cyber */
    }
    case Mode::Player:
    {
        break; /* Player */
    }
    case Mode::Sketch:
    case Mode::CnfSketch:
    default:
    {
        /* No correction is needed */
        break; /* Sketch, SketchCnF, default */
    }
    } /* switch(mode) */

    return QPoint(iX,iY);

} /* GfxImage::dat2img() */

 /*================================================================================*\
( *     D R A W I N G   T O O L S
 \*================================================================================*/

/* bitDraw
 * 'i' should be given in image coordinates, GfxImage takes care of mapping it
 *     to data coordinates
 * 'm' is the value of key and mouse button modifiers and represents the
 *     data value should be stored
 */
void GfxImage::bitDraw(QPoint i, int m)
{
    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD) /* data exists */
    {
        tD->setBitVal(img2dat(i), (quint8)m);
        /* Do not change color in ECM mode */
        if(M != Mode::CnfSketch) tD->setClrVal(tD->bit2clr(i),get_clr_val(tD,m));
    }
} /* GfxImage::bitDraw() */

/* bitRect
 * 'i' should be given in image coordinates, GfxImage takes care of mapping it
 *     to data coordinates
 * 'm' is the value of key and mouse button modifiers and represents the
 *     data value should be stored
 */
void GfxImage::bitRect(QPoint i1, QPoint i2, int m)
{
    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD) /* data exists */
    {
        /*      A-----B      *\
        |*      |     |      *|
        \*      D-----C      */

        /* Get and reorder data coordinates */
        QPoint d1 = img2dat(i1);
        QPoint d2 = img2dat(i2);
        int x1 = d1.x(); int y1 = d1.y();
        int x2 = d2.x(); int y2 = d2.y();
        int Ax,Ay,Cx,Cy;

        if(M == Mode::Ceiling)
        {
            x1 = xC2E(x1,y1);
            x2 = xC2E(x2,y2);

            if     (x1<x2)   { Ax=x1; Cx=x2; }
            else /* x2<x1 */ { Ax=x2; Cx=x1; }
            if     (y1<y2)   { Ay=y1; Cy=y2; }
            else /* y2<y1 */ { Ay=y2; Cy=y1; }

            for(int y=Ay; y<=Cy; y++)
            for(int x=Ax; x<=Cx; x++)
                tD->setBitVal(xE2C(x,y),y,(quint8)m);

        } /* Ceiling */
        else if(M == Mode::Floor)
        {
            x1 = xF2E(x1,y1);
            x2 = xF2E(x2,y2);

            if     (x1<x2)   { Ax=x1; Cx=x2; }
            else /* x2<x1 */ { Ax=x2; Cx=x1; }
            if     (y1<y2)   { Ay=y1; Cy=y2; }
            else /* y2<y1 */ { Ay=y2; Cy=y1; }

            for(int y=Ay; y<=Cy; y++)
            for(int x=Ax; x<=Cx; x++)
                tD->setBitVal(xE2F(x,y),y,(quint8)m);

        } /* Floor */
        else /* ordinary rectangle */
        {
            if     (x1<x2)   { Ax=x1; Cx=x2; }
            else /* x2<x1 */ { Ax=x2; Cx=x1; }
            if     (y1<y2)   { Ay=y1; Cy=y2; }
            else /* y2<y1 */ { Ay=y2; Cy=y1; }

            for(int y=Ay; y<=Cy; y++)
            for(int x=Ax; x<=Cx; x++)
                tD->setBitVal(x,y,(quint8)m);

            /* Do not change color in ECM mode */
            if(M != Mode::CnfSketch)
            {
                Ax=tD->bit2clr(Ax); Ay=tD->bit2clr(Ay);
                Cx=tD->bit2clr(Cx); Cy=tD->bit2clr(Cy);

                for(int y=Ay; y<=Cy; y++)
                for(int x=Ax; x<=Cx; x++)
                    tD->setClrVal(x,y,get_clr_val(tD,m));
            }

        } /* ordinary rectangle */
    } /* if(tD) */
} /* GfxImage::bitRect() */

/* clrDraw
 * 'i' should be given in image coordinates, GfxImage takes care of mapping it
 *     to data coordinates
 */
void GfxImage::clrDraw(QPoint i, int m)
{
    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD) /* data exists */
    {
        if(M == Mode::Sketch)
            /* Change color value but keep HiRes/MC mode of characters in MC mode */
            tD->setClrVal(tD->bit2clr(i), (0xF8 & tD->clrVal(tD->bit2clr(i))) | (0xF7 & get_clr_val(tD,m)) );
        else
            tD->setClrVal(tD->bit2clr(i),get_clr_val(tD,m));
    }
} /* GfxImage::clrDraw() */

/* clrRect
 * 'i' should be given in image coordinates, GfxImage takes care of mapping it
 *     to data coordinates
 */
void GfxImage::clrRect(QPoint i1, QPoint i2, int m)
{
    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD) /* data exists */
    {
        /*      A-----B      *\
        |*      |     |      *|
        \*      D-----C      */

        /* Get and reorder data coordinates */
        int x1 = tD->bit2clr(i1.x()); int y1 = tD->bit2clr(i1.y());
        int x2 = tD->bit2clr(i2.x()); int y2 = tD->bit2clr(i2.y());
        int Ax,Ay,Cx,Cy;
        if     (x1<x2)   { Ax=x1; Cx=x2; }
        else /* x2<x1 */ { Ax=x2; Cx=x1; }
        if     (y1<y2)   { Ay=y1; Cy=y2; }
        else /* y2<y1 */ { Ay=y2; Cy=y1; }

        for(int y=Ay; y<=Cy; y++)
        for(int x=Ax; x<=Cx; x++)
            if(M == Mode::Sketch)
                /* Change color value but keep HiRes/MC mode of characters in MC mode */
                tD->setClrVal(x,y, (0xF8 & tD->clrVal(x,y)) | (0xF7 & get_clr_val(tD,m)) );
            else
                tD->setClrVal(x,y,get_clr_val(tD,m));
    } /* if(tD) */
} /* GfxImage::clrRect() */

 /*================================================================================*\
( *     S E L E C T I O N   T O O L S
 \*================================================================================*/

void GfxImage::selSrcDraw(QPoint i1, QPoint i2)
{
    /*      A-----B      *\
    |*      |     |      *|
    \*      D-----C      */
    int Ax,Ay,Cx,Cy;
    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD) /* data exists */
    {
        Ax=i1.x(); Ay=i1.y(); Cx=i2.x(); Cy=i2.y();
        tD->validateSelection(Ax,Ay,Cx,Cy);
    }
    else /* no data */
    {
        Ax=0; Ay=0; Cx=-1; Cy=-1;
    }
    selSrcA = QPoint(Ax,Ay);
    selSrcC = QPoint(Cx,Cy);
    emit selectSrc(Ax,Ay,Cx+1-Ax,Cy+1-Ay);
}

 /*================================================================================*\
( *     D R A G  &  D R O P
 \*================================================================================*/

bool GfxImage::isSelectionDragged(QPoint start)
{
    int x = start.x();
    int y = start.y();

    /* Inside the selected area */
    if(selSrcA.x() < x && x <= selSrcC.x() &&
       selSrcA.y() < y && y <= selSrcC.y() )
        return true;

    if(GfxData::Type::Sketch==T || GfxData::Type::CnfSketch==T)
    {
        if(0 < x && x <= width() &&
           0 < y && y <= height() )
            return true;
    }

    return false;
}

QByteArray GfxImage::exportDataFromSelection(QPoint start)
{
    int x = start.x();
    int y = start.y();

    /* Inside the selected area */
    bool inside = (selSrcA.x() < x && x <= selSrcC.x() &&
                   selSrcA.y() < y && y <= selSrcC.y() );

    QPoint sA = inside ? selSrcA : start;
    QPoint sC = inside ? selSrcC : start;

    QByteArray ba;
    /* Create an empty GfxData with 0x0 size */
    shared_ptr<GfxData> d(new GfxData());

    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD) /* data exists */
    {
        /* Load a copy of the selected area */
        d->load(tD->copy(sA,sC));
    }

    /* Serialize data of the selected area.
     * If we could not load anything,
     * then the empty GfxData will be serialized with 0x0 size.
     */
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds << d;

    ba.append(d->bitW()); // size()-4
    ba.append(d->bitH()); // size()-3
    ba.append(x-sA.x());  // size()-2
    ba.append(y-sA.y());  // size()-1

    return ba;
}

void GfxImage::importDataToImage(QByteArray &src, QPoint p)
{
    shared_ptr<GfxData> tD = D.lock(); /* data */
    if(tD) /* data exists */
    {
        shared_ptr<GfxData> d(new GfxData());
        QDataStream ds(src);
        ds >> d;

        /* paste source into the image */
        int Ax,Ay,Cx,Cy;
        int s=src.size();
        Ax=p.x()-src.at(s-2);
        Ay=p.y()-src.at(s-1);
        tD->validateSelectionStart(Ax,Ay);
        tD->paste(d,Ax,Ay);

        /* display target selection */
        Cx=Ax+src.at(s-4)-1;
        Cy=Ay+src.at(s-3)-1;
        tD->validateSelection(Ax,Ay,Cx,Cy);
        refresh();
        emit selectTgt(Ax,Ay,Cx+1-Ax,Cy+1-Ay);
    }
}

 /*================================================================================*\
( *     H E L P E R S
 \*================================================================================*/

quint8 GfxImage::get_clr_val(shared_ptr<GfxData> d, int m)
{
    quint8 clr;
    if(M == Mode::CnfSketch)
    {
        /* Mouse Button        Color
         * Right     = 0b00 -> ECM1
         * Left      = 0b01 -> ECM0
         * CtrlRight = 0b10 -> ECM3
         * CtrlLeft  = 0b11 -> ECM2
         */
        switch(m)
        {
        case 0: clr=(quint8)GfxData::ColorValue::ECM1; break;
        default:
        case 1: clr=(quint8)GfxData::ColorValue::ECM0; break;
        case 2: clr=(quint8)GfxData::ColorValue::ECM3; break;
        case 3: clr=(quint8)GfxData::ColorValue::ECM2; break;
        }
    }
    else
    {
        clr = (quint8)d->color((int)GfxData::ColorIndex::Color);
    }
    return clr;
}

/* Return horizontal offset for line 'y' */
/* Needed for sprites only */
int GfxImage::hor_offset(int y)
{
    if(y<0) y=0;
    int offset = 0;

    if (Mode::Expanded == M)
    {
        offset = 2;
    }
    else if (Mode::Ceiling == M)
    {
        if(20<y) y=20;

        if      (P < 0) offset = offset_L[y]; /* left   */
        else if (0 < P) offset = offset_R[y]; /* right  */
        else            offset = offset_C[y]; /* center */
    }
    else if (Mode::Floor == M)
    {
        if(20<y) y=20;

        if      (P < 0) offset = offset_L[20-y]; /* left   */
        else if (0 < P) offset = offset_R[20-y]; /* right  */
        else            offset = offset_C[20-y]; /* center */
    }

    return offset;
}
/* Perspective transformations of X coord */
int GfxImage::xE2C(int x, int y) /* Extended to Ceiling */
{
    float prop = (24.0-float(y-2)/4.0)/24.0;
    return int(float(x)*prop+0.5);
}
int GfxImage::xC2E(int x, int y) /* Ceiling to Extended */
{
    float prop = (24.0-float(y-2)/4.0)/24.0;
    return int(float(x)/prop+0.5);
}
int GfxImage::xE2F(int x, int y) /* Extended to Floor */
{
    float prop = (24.0-float(18-y)/4.0)/24.0;
    return int(float(x)*prop+0.5);
}
int GfxImage::xF2E(int x, int y) /* Floor to Extended */
{
    float prop = (24.0-float(18-y)/4.0)/24.0;
    return int(float(x)/prop+0.5);
}
