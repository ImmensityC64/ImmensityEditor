#ifndef GFX_DATA_H
#define GFX_DATA_H

#include <Qt>
#include <QObject>
#include <QVector>
#include <QBitArray>
#include <QByteArray>
#include <QString>
#include <QMainWindow>
#include <QDataStream>
#include <QPoint>
#include <QPen>
#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDrag>
#include <QMimeData>
#include <memory>
#include "general.h"

class Browser;
class Editor;
class GfxImage;

 /*================================================================================*\
( *     G F X   D A T A
 \*================================================================================*/

class GfxData : public enable_shared_from_this<GfxData> {

public:
    enum class Type {
        Nothing = 0,
        /* Sprite Group: Colors + BitMap */
        Sprite,     /* 1x1 sprites */
        /* Sprite is a common type for ceiling, floor and expanded sprites */
        Wall,       /* 1x5 sprites */
        Enemy,      /* 3x3 sprites */
        Player,     /* 1x2 sprites with special display */
        Cyber,      /* 1x2 sprites with special display */
        /* Sketch Group: Colors + BitMap + ClrMap */
        Sketch,     /* 80x60 chars by default */
        CnfSketch,  /* 80x60 chars by default */
        /* Blocks, tiles, animated tiles, C&F tiles, characters, C&F characters
         * are all generated from Scenery DB.
         * These entities do not have GfxData storage.
         */
    };

    enum class ColorIndex {
        Color,
        Backg,
        Comm0,
        Comm1,
        ECM0,
        ECM1,
        ECM2,
        ECM3,
        Size
    };

    enum class ColorValue {
        ECM0 = 0b00000000,
        ECM1 = 0b01000000,
        ECM2 = 0b10000000,
        ECM3 = 0b11000000,
    };

    enum class PixelMode {
        HiRes,
        Multi,
        ClrMap,
    };

    enum class Id { /* 0 and positive numbers are reserved for valid ID values */
        Nothing = -1,
        Append  = -2,
    };

private:
    Type T; /* type   */
    int W;  /* width  */
    int H;  /* height */
    PixelMode CM; /* color mode */
    quint8 colors[(int)ColorIndex::Size];
    QVector<QBitArray*> *bitmap;
    QVector<QVector<quint8>*> *clrmap;
    quint32 crc; /* CRC32 hash of 'bitmap' and 'clrmap'.
                  * If 'crc' values or two GfxData are different, then
                  * the two GfxData are surely different.
                  * It can be used to decide fast if data maps of
                  * a graphical object is different from another one.
                  * It is useful for example when new sprites are
                  * imported to a scenery.
                  * It is (re)calculated when data is read from file and
                  * if data is changed and saved in an editor.
                  * Note that it does NOT consider 'colors'!
                  */
    quint32 cnt; /* Modification counter.
                  * If 'cnt' values of two GfxData are identical, then
                  * the two GfxData are surely identical.
                  * It can be used to decide fast if GfxImage is not
                  * required to be refreshed in a BrowserGfxTile.
                  * It is (re)calculated when data is read from file and
                  * if data is changed and saved in an editor.
                  * Note that it includes changes of 'colors'!
                  */
    static quint32 static_cnt;

public:
    Type    type(void);
    Type setType(Type tT);
    PixelMode colorMode(void);
    quint32 dataDiffersCrc(void);
    quint32 imageEqualsCnt(void);
    bool dataDiffers(shared_ptr<GfxData> src);
    bool dataDiffers(quint32 tCrc);
    bool imageEquals(shared_ptr<GfxData> src);
    bool imageEquals(quint32 tCnt);

    int clrSize(int s); /* convert to colormap size */
    int bitW(void); /* bitmap width  */
    int bitH(void); /* bitmap height */
    int clrW(void); /* colormap width  */
    int clrH(void); /* colormap height */
    int imgW(void); /* image width (usually the double of bitmap width) */
    int imgH(void); /* image height */

    int bit2clr(int coord); /* convert coordinate */
    int clr2bit(int coord); /* convert coordinate */
    QPoint bit2clr(QPoint p);
    QPoint clr2bit(QPoint p);

    bool validColor(int i);
    bool validBitCoord(int x, int y);
    bool validClrCoord(int x, int y);
    void validateSelection(int &x, int &y);
    void validateSelection(int &ax, int &ay, int &cx, int &cy);
    void validateSelectionStart(int &ax, int &ay);

    void init(Type tType = Type::Nothing, int tWidth = 0, int tHeight = 0);
    void resize(int tW, int tH); /* width, height */
    void load(shared_ptr<GfxData> src);
    void loadNewVersion(shared_ptr<GfxData> src);

    explicit GfxData();
    explicit GfxData(Type tType);
    explicit GfxData(Type tType, int tWidth, int tHeight);
    explicit GfxData(shared_ptr<GfxData> src);
    shared_ptr<GfxData> copy(void);
    shared_ptr<GfxData> copy(QPoint a, QPoint c);
    shared_ptr<GfxData> copy(int ax, int ay, int cx, int cy);
    void paste(shared_ptr<GfxData> src, QPoint p);
    void paste(shared_ptr<GfxData> src, int px, int py);
    explicit GfxData(quint64 chr, Type tType=Type::Sketch);
    quint64 chr(int cx=0, int cy=0);
    virtual ~GfxData();

    quint8    color(int i);
    quint8 setColor(int i, quint8 v);

    /* get/set bitmap value */
    quint8    bitVal(int x, int y);
    quint8 setBitVal(int x, int y, quint8 v);
    quint8    bitVal(QPoint p);
    quint8 setBitVal(QPoint p, quint8 v);

    /* get/set clrmap value */
    quint8    clrVal(int x, int y);
    quint8 setClrVal(int x, int y, quint8 v);
    quint8    clrVal(QPoint p);
    quint8 setClrVal(QPoint p, quint8 v);

    friend QDataStream& operator <<(QDataStream& out, shared_ptr<GfxData> const data);
    friend QDataStream& operator >>(QDataStream& in, shared_ptr<GfxData> data);

private:
    PixelMode get_mode(void);
    bool get_mc(int x, int y);
    static quint32 crc_tbl[256];
    void calcCrc(void);
    inline quint32 calc_crc_next_byte(quint32 crc, quint32 byte);
};

/* serialize GfxData pointed by the given shared_ptr and send out to the stream */
QDataStream& operator <<(QDataStream& out, shared_ptr<GfxData> const src);

/* read the stream and reconstruct the content of GfxData pointed by the given shared_ptr */
QDataStream& operator >>(QDataStream& in, shared_ptr<GfxData> dst);

 /*================================================================================*\
( *     G F X   H I S T O R Y
 \*================================================================================*/

class GfxHistory : public QVector<shared_ptr<GfxData>> {
private:
    int o; /* Oldest  */
    int c; /* Current */
    void inc(int &v);
    void dec(int &v);
    void clearData(int &v);

public:
    explicit GfxHistory(shared_ptr<GfxData> init);
    virtual ~GfxHistory();

    void load(shared_ptr<GfxData> dst);
    void save(shared_ptr<GfxData> const src);
    void undo(shared_ptr<GfxData> dst);
    void redo(shared_ptr<GfxData> dst);
};

#endif // GFX_DATA_H
