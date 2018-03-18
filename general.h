#ifndef GENERAL_H
#define GENERAL_H

#include <QVector>
#include <QList>
#include <QString>
#include <QColor>
#include <QBrush>
#include <iostream>
#include <stdio.h>


#define BrowserTileSizeLong  150+20 /* gfxImage + info label */
#define BrowserTileSizeShort  60    /* gfxImage */
#define BrowserTileFrameWidth  4
#define EditorViewSize 480
#define NewDialogNoCopyString "- no copy - create a blank one -"


using namespace std;


enum class Command {
    LoadHistory,
    SaveHistory,
    ShowSelSrc,
    HideSelSrc,
    ShowSelTgt,
    HideSelTgt,
};

enum class ScrBg {
    CeilingFg,
    Ceiling0,
    Ceiling1,
    Ceiling2,
    Ceiling3,
    Ceiling4,
    Ceiling5,
    Ceiling6,
    Background,
    Floor6,
    Floor5,
    Floor4,
    Floor3,
    Floor2,
    Floor1,
    Floor0,
    FloorFg,
    BorderL,
    BorderR,
    Size
};

enum class ScrPart {
    CeilingFgL,
    CeilingFgC,
    CeilingFgR,

    CeilingL,
    CeilingC,
    CeilingR,

    BackgroundL,
    BackgroundC,
    BackgroundR,

    WallL,
    WallC,
    WallR,

    FloorL,
    FloorC,
    FloorR,

    FloorFgL,
    FloorFgC,
    FloorFgR,

    Size,

    CeilingFg   = ScrPart::CeilingFgL,
    Ceiling     = ScrPart::CeilingL,
    Background  = ScrPart::BackgroundL,
    Wall        = ScrPart::WallL,
    Floor       = ScrPart::FloorL,
    FloorFg     = ScrPart::FloorFgL,
};


class Cfg
{
public:
    enum  Enum {
        ViewBrowserScale,
        ViewSceneColor,
        ViewNoDataColor,
        EditorFineGridColor,
        EditorMainGridColor,
        EditorWallGridColor,
        EditorMainGridOutland,
        EditorSelSrcPenColor,
        EditorSelSrcBrushColor,
        EditorSelTgtPenColor,
        EditorSelTgtBrushColor,
        ToolboxSceneColor,
        IconSelectColor,
        GfxEdHistoryDepth,
        Size,
    };

    static Cfg& ins();
    int    val(int i);
    int setVal(int i, int v);

private:
    QVector<int> cfg;
    Cfg();
    Cfg(Cfg const&);            // Don't implement
    void operator=(Cfg const&); // Don't implement

}; /* class Cfg */


class C64
{
public:
    enum Enum {
        IndexBlack      = 0x00,
        IndexWhite      = 0x01,
        IndexRed        = 0x02,
        IndexCyan       = 0x03,
        IndexPurple     = 0x04,
        IndexGreen      = 0x05,
        IndexBlue       = 0x06,
        IndexYellow     = 0x07,
        IndexOrange     = 0x08,
        IndexBrown      = 0x09,
        IndexPink       = 0x0A,
        IndexDarkGrey   = 0x0B,
        IndexGrey       = 0x0C,
        IndexLightGreen = 0x0D,
        IndexLightBlue  = 0x0E,
        IndexLightGrey  = 0x0F,

        IndexSize       = 0x10,
        IndexAlpha      = 0x10,
        IndexScene      = 0x11,
        IndexNoData     = 0x12,

        MultiBlack      = 0x08,
        MultiWhite      = 0x09,
        MultiRed        = 0x0A,
        MultiCyan       = 0x0B,
        MultiPurple     = 0x0C,
        MultiGreen      = 0x0D,
        MultiBlue       = 0x0E,
        MultiYellow     = 0x0F,

        AlphaBlack      = 0x10,
        AlphaWhite      = 0x11,
        AlphaRed        = 0x12,
        AlphaCyan       = 0x13,
        AlphaPurple     = 0x14,
        AlphaGreen      = 0x15,
        AlphaBlue       = 0x16,
        AlphaYellow     = 0x17,
        AlphaOrange     = 0x18,
        AlphaBrown      = 0x19,
        AlphaPink       = 0x1A,
        AlphaDarkGrey   = 0x1B,
        AlphaGrey       = 0x1C,
        AlphaLightGreen = 0x1D,
        AlphaLightBlue  = 0x1E,
        AlphaLightGrey  = 0x1F,

        SpriteHeight    = 21,
        SpriteWidth     = 24,

        CharSetRows =  8,
        CharSetCols = 32,
    };

    static C64& ins();

    const QColor& color(int i);
    const QColor& shade(int i, int j);
    const QBrush& brush_hires(int i); /* Brush Hires */
    const QBrush& brush_multi(int i); /* Brush MultiColor */
    const QBrush& brush_alpha(int i); /* Brush Alpha */

    int       color_size(void);
    int brush_hires_size(void);
    int brush_multi_size(void);
    int brush_alpha_size(void);
    const QVector<QRgb>& color_table(void);

    private:
        QVector<QColor> color_v;
        QVector<QVector<QColor>> shade_v;
        QList<QBrush> br_hi_v;
        QList<QBrush> br_mu_v;
        QList<QBrush> br_al_v;
        QVector<QRgb> color_table_v;
        const static int xpm_template_mu_size = 13;
        const static int xpm_template_al_size =  5;
        const static unsigned char opacity = 128;
        const static unsigned char  dark_component = 104;
        const static unsigned char light_component = 152;
        char   * xpm_template_mu[xpm_template_mu_size];
        char   * xpm_template_al[xpm_template_al_size];
        char *** xpm_mu_v;
        char *** xpm_al_v;

        C64();
        C64(C64 const&);            // Don't implement
        void operator=(C64 const&); // Don't implement

        void generate_xpm_templates_mu(void);
        void generate_xpm_templates_al(void);
        int mix_colors(int a, int b);

}; /* class C64 */


class Icon
{

public:
    typedef enum Enum {
        Pencil,
        PencilRect,
        Bucket,
        RectEmpty,
        RectFilled,
        Empty,
        Paint,
        PaintRect,
        Select,
        InvertRect,
        Size
    } Enum;

    char const **xpm[Size];
    static const int size=48;

    static Icon& ins();

    Icon();
    Icon(Icon const&);           // Don't implement
    void operator=(Icon const&); // Don't implement

}; /* class Icon */

const QString mimeTypeGfxData="immGfxData";

#endif // GENERAL_H
