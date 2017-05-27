
#include "general.h"



 /*================================================================================*\
( *     C F G
 \*================================================================================*/



Cfg& Cfg::ins()
{
    static Cfg instance;
    return instance;
}

int Cfg::   val(int i)        { return cfg[i];     }
int Cfg::setVal(int i, int v) { return cfg[i] = v; }

Cfg::Cfg() :
    cfg(QVector<int>(Size))
{
    cfg[ViewBrowserScale]  = 200; /* % */
    cfg[ViewSceneColor]         = 0xFFC8C8B0; /* #AARRGGBB */
    cfg[ViewNoDataColor]        = 0xFFA8A890; /* #AARRGGBB */
    cfg[EditorFineGridColor]    = 0xFF6080C0; /* #AARRGGBB */
    cfg[EditorMainGridColor]    = 0xFFA0C0FF; /* #AARRGGBB */
    cfg[EditorMainGridOutland]  = 0x80C04040; /* #AARRGGBB */
    cfg[EditorSelSrcPenColor]   = 0x80C0C000; /* #AARRGGBB */
    cfg[EditorSelSrcBrushColor] = 0x20C0C000; /* #AARRGGBB */
    cfg[EditorSelTgtPenColor]   = 0x8000FF80; /* #AARRGGBB */
    cfg[EditorSelTgtBrushColor] = 0x2000FF80; /* #AARRGGBB */
    cfg[ToolboxSceneColor]      = 0xFFF0F0F0; /* #AARRGGBB */
    cfg[IconSelectColor]        = 0xFFC0E070; /* #AARRGGBB */
    cfg[GfxEdHistoryDepth] = 64;
}



 /*================================================================================*\
( *     C 6 4
 \*================================================================================*/



C64& C64::ins()
{
    static C64 instance;
    return instance;
}

const QColor& C64::color(int i)        { return color_v.at(i&0x0F); }
const QColor& C64::shade(int i, int j) { return shade_v.at(i&0x0F).at(j&0x0F); }
const QBrush& C64::brush_hires(int i)  { return br_hi_v.at(i&0x0F); } /* Brush Hires */
const QBrush& C64::brush_multi(int i)  { return br_mu_v.at(i&0x0F); } /* Brush MultiColor */
const QBrush& C64::brush_alpha(int i)  { return br_al_v.at(i&0x1F); } /* Brush Alpha */

int       C64::color_size(void) { return color_v.size(); }
int C64::brush_hires_size(void) { return br_hi_v.size(); }
int C64::brush_multi_size(void) { return br_mu_v.size(); }
int C64::brush_alpha_size(void) { return br_al_v.size(); }
const QVector<QRgb>& C64::color_table(void) { return color_table_v; }

C64::C64()
{
    /****************************************\
   ( **  COLOR                             ** )
    \****************************************/

    color_v.append(QColor(0x00,0x00,0x00));
    color_v.append(QColor(0xFF,0xFF,0xFF));
    color_v.append(QColor(0x68,0x37,0x2B));
    color_v.append(QColor(0x70,0xA4,0xB2));
    color_v.append(QColor(0x6F,0x3D,0x86));
    color_v.append(QColor(0x58,0x8D,0x43));
    color_v.append(QColor(0x35,0x28,0x79));
    color_v.append(QColor(0xB8,0xC7,0x6F));
    color_v.append(QColor(0x6F,0x4F,0x25));
    color_v.append(QColor(0x43,0x39,0x00));
    color_v.append(QColor(0x9A,0x67,0x59));
    color_v.append(QColor(0x44,0x44,0x44));
    color_v.append(QColor(0x6C,0x6C,0x6C));
    color_v.append(QColor(0x9A,0xD2,0x84));
    color_v.append(QColor(0x6C,0x5E,0xB5));
    color_v.append(QColor(0x95,0x95,0x95));

    /****************************************\
   ( **  SHADE                             ** )
    \****************************************/

    /* Generate mixture of all colors (16x16) */
    for(int i=0; i<=0xF; i++)
    {
        QVector<QColor> line;
        int r0 = color_v.at(i).red();
        int g0 = color_v.at(i).green();
        int b0 = color_v.at(i).blue();
        for(int j=0; j<=0xF; j++)
        {
            int r1 = color_v.at(j).red();
            int g1 = color_v.at(j).green();
            int b1 = color_v.at(j).blue();
            line.append(QColor((r0+r1)/2, (g0+g1)/2, (b0+b1)/2));
        }
        shade_v.append(line);
    }

    /****************************************\
   ( **  PIXMAP                            ** )
    \****************************************/

    generate_xpm_templates_mu();
    generate_xpm_templates_al();

    /****************************************\
   ( **  BRUSH                             ** )
    \****************************************/

    for(int i=0x00; i<=0x0F; i++) { br_hi_v.append(QBrush(color_v.at(i))); }

    for(int i=0x00; i<=0x07; i++) { br_mu_v.append(QBrush(color_v.at(i))); }
    for(int i=0x08; i<=0x0F; i++) { br_mu_v.append(QBrush(QPixmap(xpm_mu_v[i-0x08]))); }

    for(int i=0x00; i<=0x0F; i++) { br_al_v.append(QBrush(color_v.at(i))); }
    for(int i=0x10; i<=0x1F; i++) { br_al_v.append(QBrush(QPixmap(xpm_al_v[i-0x10]))); }

    /****************************************\
   ( **  COLORTABLE                        ** )
    \****************************************/

    for(int i=0x00; i<=0x0F; i++) { color_table_v.append(color_v.at(i).rgba()); }
    color_table_v.append(QRgb(0)); /* [0x10] = alpha channel */
    color_table_v.append(QRgb( Cfg::ins().val(Cfg::ViewSceneColor)   )); /* [0x11] */
    color_table_v.append(QRgb( Cfg::ins().val(Cfg::ViewNoDataColor)  )); /* [0x12] */

    /****************************************\
   ( **  END                               ** )
    \****************************************/

} /* C64 */

void C64::generate_xpm_templates_mu(void)
{
    int i=0;
    xpm_template_mu[i++] = (char *)"20 10 2 1";
    xpm_template_mu[i++] = (char *)" 	c #808080";
    xpm_template_mu[i++] = (char *)"+	c #FFFFFF";
    xpm_template_mu[i++] = (char *)"++++++++++++++++++++";
    xpm_template_mu[i++] = (char *)"++++++++++++++++++++";
    xpm_template_mu[i++] = (char *)"++  +  +++    ++++++";
    xpm_template_mu[i++] = (char *)"++     ++  +++++++++";
    xpm_template_mu[i++] = (char *)"++     ++  +++++++++";
    xpm_template_mu[i++] = (char *)"++  +  ++  +++++++++";
    xpm_template_mu[i++] = (char *)"++  +  +++    ++++++";
    xpm_template_mu[i++] = (char *)"++++++++++++++++++++";
    xpm_template_mu[i++] = (char *)"++++++++++++++++++++";
    xpm_template_mu[i++] = (char *)"++++++++++++++++++++";
    if(xpm_template_mu_size!=i)
        cerr << __FILE__ << ":" << __LINE__ << " xpm_template_mu has wrong size!" << endl;

    xpm_mu_v       = new char ** [0x08];

    for(int i=0x00; i<0x08; i++)
    {
        xpm_mu_v[i] = new char * [xpm_template_mu_size];
        for(int l=0; l<xpm_template_mu_size; l++)
            xpm_mu_v[i][l] = xpm_template_mu[l];

        xpm_mu_v[i][2] = new char [12];
        snprintf(xpm_mu_v[i][2], 12, "+	c #%2.2X%2.2X%2.2X",
                color_v.at(i).red(), color_v.at(i).green(), color_v.at(i).blue());
    }
}

void C64::generate_xpm_templates_al(void)
{
    int i=0;
    xpm_template_al[i++] = (char *)"2 2 2 1";
    xpm_template_al[i++] = (char *)" 	c #000000";
    xpm_template_al[i++] = (char *)"+	c #FFFFFF";
    xpm_template_al[i++] = (char *)" +";
    xpm_template_al[i++] = (char *)"+ ";

    if(xpm_template_al_size!=i)
        cerr << __FILE__ << ":" << __LINE__ << " xpm_template_al has wrong size!" << endl;

    xpm_al_v       = new char ** [0x10];

    for(int i=0x00; i<0x10; i++)
    {
        xpm_al_v[i] = new char * [xpm_template_al_size];
        for(int l=0; l<xpm_template_al_size; l++)
            xpm_al_v[i][l] = xpm_template_al[l];

        xpm_al_v[i][1] = new char [12];
        snprintf(xpm_al_v[i][1], 12, " 	c #%2.2X%2.2X%2.2X",
                mix_colors(color_v.at(i).red(),  dark_component),
                mix_colors(color_v.at(i).green(),dark_component),
                mix_colors(color_v.at(i).blue(), dark_component));

        xpm_al_v[i][2] = new char [12];
        snprintf(xpm_al_v[i][2], 12, "+	c #%2.2X%2.2X%2.2X",
                mix_colors(color_v.at(i).red(),  light_component),
                mix_colors(color_v.at(i).green(),light_component),
                mix_colors(color_v.at(i).blue(), light_component));
    }
}

int C64::mix_colors(int a, int b)
{
    return (a*(int)(255-opacity) + b*(int)opacity)/255;
}



 /*================================================================================*\
( *     I C O N
 \*================================================================================*/



#include "images/pencil.xpm"
#include "images/pencil_rect.xpm"
#include "images/bucket.xpm"
#include "images/rect_empty.xpm"
#include "images/rect_filled.xpm"
#include "images/paint.xpm"
#include "images/paint_rect.xpm"
#include "images/select.xpm"
#include "images/invert_rect.xpm"
#include "images/empty.xpm"

Icon& Icon::ins()
{
    static Icon instance;
    return instance;
}

Icon::Icon()
{
    xpm[Pencil]     = icon_pencil_xpm;
    xpm[PencilRect] = icon_pencil_rect_xpm;
    xpm[Bucket]     = icon_bucket_xpm;
    xpm[RectEmpty]  = icon_rect_empty_xpm;
    xpm[RectFilled] = icon_rect_filled_xpm;
    xpm[Empty]      = icon_empty_xpm;
    xpm[Paint]      = icon_paint_xpm;
    xpm[PaintRect]  = icon_paint_rect_xpm;
    xpm[Select]     = icon_select_xpm;
    xpm[InvertRect] = icon_invert_rect_xpm;
}

