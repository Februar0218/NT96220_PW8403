
//    Color
/*
    COLOR_ITEM; color value

    PALETTE_ITEM; Palette color value

    MAPPING_ITEM; Color index mapping value
*/

#ifndef _GX_COLOR_H_
#define _GX_COLOR_H_

#include "GxCommon.h"

#ifdef __cplusplus
extern "C" {
#endif


//--------------------------------------------------------------------------------------
//  color
//--------------------------------------------------------------------------------------

//<type>

typedef UINT8   CVALUE; //color value
typedef UINT32  COLOR_ITEM; //color

//<enum>

//color : pre-defined RGBD
//                                  0x00BBGGRR
#define COLOR_RGB_BLACK             0x00000000
#define COLOR_RGB_DARKRED           0x00000080
#define COLOR_RGB_DARKGREEN         0x00008000
#define COLOR_RGB_DARKYELLOW        0x00008080
#define COLOR_RGB_DARKBLUE          0x00800000
#define COLOR_RGB_DARKMAGENTA       0x00800080
#define COLOR_RGB_DARKCYAN          0x00808000
#define COLOR_RGB_LIGHTGRAY         0x00C0C0C0
#define COLOR_RGB_GRAY              0x00808080
#define COLOR_RGB_RED               0x000000FF
#define COLOR_RGB_GREEN             0x0000FF00
#define COLOR_RGB_YELLOW            0x0000FFFF
#define COLOR_RGB_BLUE              0x00FF0000
#define COLOR_RGB_MAGENTA           0x00FF00FF
#define COLOR_RGB_CYAN              0x00FFFF00
#define COLOR_RGB_WHITE             0x00FFFFFF

#define COLOR_RGB_PASTELGREEN       0x00C0DCC0
#define COLOR_RGB_PASTELBLUE        0x00F0CAA6
#define COLOR_RGB_SOFTWHITE         0x00F0FBFF
#define COLOR_RGB_MEDIUMGRAY        0x00A4A0A0

//color : pre-defined YUVD
//NOTE: this value is calc by Excel rgb2yuv matrix
//                                  0x00VVUUYY
#define COLOR_YUV_BLACK             0x00808000
#define COLOR_YUV_DARKRED           0x00C06A26
#define COLOR_YUV_DARKGREEN         0x004A564B
#define COLOR_YUV_DARKYELLOW        0x008A4071
#define COLOR_YUV_DARKBLUE          0x0076C00F
#define COLOR_YUV_DARKMAGENTA       0x00B6AA35
#define COLOR_YUV_DARKCYAN          0x0040965A
#define COLOR_YUV_LIGHGRAY          0x008080C0
#define COLOR_YUV_GRAY              0x00808080
#define COLOR_YUV_RED               0x00FF554C
#define COLOR_YUV_GREEN             0x00152C96
#define COLOR_YUV_YELLOW            0x009500E2
#define COLOR_YUV_BLUE              0x006BFF1D
#define COLOR_YUV_MAGENTA           0x00EBD469
#define COLOR_YUV_CYAN              0x0001ABB3
#define COLOR_YUV_WHITE             0x008080FF

#define COLOR_YUV_PASTELGREEN       0x008080FF
#define COLOR_YUV_PASTELBLUE        0x008080FF
#define COLOR_YUV_SOFTWHITE         0x008080FF
#define COLOR_YUV_MEDIUMGRAY        0x008080FF


//#ifndef _WIN32
#define COLOR_BLACK                 COLOR_YUV_BLACK
#define COLOR_DARKRED               COLOR_YUV_DARKRED
#define COLOR_DARKGREEN             COLOR_YUV_DARKGREEN
#define COLOR_DARKYELLOW            COLOR_YUV_DARKYELLOW
#define COLOR_DARKBLUE              COLOR_YUV_DARKBLUE
#define COLOR_DARKMAGENTA           COLOR_YUV_DARKMAGENTA
#define COLOR_DARKCYAN              COLOR_YUV_DARKCYAN
#define COLOR_LIGHGRAY              COLOR_YUV_LIGHGRAY
#define COLOR_GRAY                  COLOR_YUV_GRAY
#define COLOR_RED                   COLOR_YUV_RED
#define COLOR_GREEN                 COLOR_YUV_GREEN
#define COLOR_YELLOW                COLOR_YUV_YELLOW
#define COLOR_BLUE                  COLOR_YUV_BLUE
#define COLOR_MAGENTA               COLOR_YUV_MAGENTA
#define COLOR_CYAN                  COLOR_YUV_CYAN
#define COLOR_WHITE                 COLOR_YUV_WHITE
#define COLOR_PASTELGREEN           COLOR_YUV_PASTELGREEN
#define COLOR_PASTELBLUE            COLOR_YUV_PASTELBLUE
#define COLOR_SOFTWHITE             COLOR_YUV_SOFTWHITE
#define COLOR_MEDIUMGRAY            COLOR_YUV_MEDIUMGRAY
/*#else
#define COLOR_BLACK                 COLOR_RGB_BLACK
#define COLOR_DARKRED               COLOR_RGB_DARKRED
#define COLOR_DARKGREEN             COLOR_RGB_DARKGREEN
#define COLOR_DARKYELLOW            COLOR_RGB_DARKYELLOW
#define COLOR_DARKBLUE              COLOR_RGB_DARKBLUE
#define COLOR_DARKMAGENTA           COLOR_RGB_DARKMAGENTA
#define COLOR_DARKCYAN              COLOR_RGB_DARKCYAN
#define COLOR_LIGHGRAY              COLOR_RGB_LIGHGRAY
#define COLOR_GRAY                  COLOR_RGB_GRAY
#define COLOR_RED                   COLOR_RGB_RED
#define COLOR_GREEN                 COLOR_RGB_GREEN
#define COLOR_YELLOW                COLOR_RGB_YELLOW
#define COLOR_BLUE                  COLOR_RGB_BLUE
#define COLOR_MAGENTA               COLOR_RGB_MAGENTA
#define COLOR_CYAN                  COLOR_RGB_CYAN
#define COLOR_WHITE                 COLOR_RGB_WHITE
#define COLOR_PASTELGREEN           COLOR_RGB_PASTELGREEN
#define COLOR_PASTELBLUE            COLOR_RGB_PASTELBLUE
#define COLOR_SOFTWHITE             COLOR_RGB_SOFTWHITE
#define COLOR_MEDIUMGRAY            COLOR_RGB_MEDIUMGRAY
#endif*/

#define COLOR_TRANSPARENT           ((UINT32)(0xFFFFFFFF))
#define COLOR_DONTDRAW              COLOR_TRANSPARENT
#define COLOR_DONTKEY               COLOR_TRANSPARENT


//<function>
/*
//inline function
CVALUE COLOR_YUVD_GET_Y(COLOR_ITEM yuvd);
CVALUE COLOR_YUVD_GET_U(COLOR_ITEM yuvd);
CVALUE COLOR_YUVD_GET_V(COLOR_ITEM yuvd);
CVALUE COLOR_RGBD_GET_R(COLOR_ITEM rgbd);
CVALUE COLOR_RGBD_GET_G(COLOR_ITEM rgbd);
CVALUE COLOR_RGBD_GET_B(COLOR_ITEM rgbd);
COLOR_ITEM COLOR_MAKE_RGBD(CVALUE r, CVALUE g, CVALUE b);
COLOR_ITEM COLOR_MAKE_YUVD(CVALUE y, CVALUE u, CVALUE v);
*/
//color : get channel value
//#NT#2009/7/15#JeahYen -begin
#define COLOR_YUVD_GET_V(c)         (CVALUE)(((c) & 0x00ff0000) >> 16)
#define COLOR_YUVD_GET_U(c)         (CVALUE)(((c) & 0x0000ff00) >> 8)
#define COLOR_YUVD_GET_Y(c)         (CVALUE)(((c) & 0x000000ff) >> 0)

#define COLOR_YUVA_GET_A(c)         (CVALUE)(((c) & 0xff000000) >> 24)
#define COLOR_YUVA_GET_V(c)         (CVALUE)(((c) & 0x00ff0000) >> 16)
#define COLOR_YUVA_GET_U(c)         (CVALUE)(((c) & 0x0000ff00) >> 8)
#define COLOR_YUVA_GET_Y(c)         (CVALUE)(((c) & 0x000000ff) >> 0)

#define COLOR_RGBD_GET_B(c)         (CVALUE)(((c) & 0x00ff0000) >> 16)
#define COLOR_RGBD_GET_G(c)         (CVALUE)(((c) & 0x0000ff00) >> 8)
#define COLOR_RGBD_GET_R(c)         (CVALUE)(((c) & 0x000000ff) >> 0)

#define COLOR_RGBA_GET_A(c)         (CVALUE)(((c) & 0xff000000) >> 24)
#define COLOR_RGBA_GET_B(c)         (CVALUE)(((c) & 0x00ff0000) >> 16)
#define COLOR_RGBA_GET_G(c)         (CVALUE)(((c) & 0x0000ff00) >> 8)
#define COLOR_RGBA_GET_R(c)         (CVALUE)(((c) & 0x000000ff) >> 0)

#define COLOR_RGB565_GET_COLOR(c)   (COLOR_ITEM)(((c) & 0x0000ffff) >> 0)
#define COLOR_RGB565_GET_A(c)       (CVALUE)(((c) & 0x00ff0000) >> 16)
//#NT#2009/8/17#JeahYen -begin
#define COLOR_RGB565_GET_B(c)       ((CVALUE)(((c) & 0x0000001f) >> 0) << 3)
#define COLOR_RGB565_GET_G(c)       ((CVALUE)(((c) & 0x000007e0) >> 5) << 2)
#define COLOR_RGB565_GET_R(c)       ((CVALUE)(((c) & 0x0000f800) >> 11) << 3)
//#NT#2009/8/17#JeahYen -end

#define COLOR_MAKE_RGBA(r,g,b,a)    \
                                    (((COLOR_ITEM)(r)&0x000000ff)|\
                                    (((COLOR_ITEM)(g)&0x000000ff)<<8)|\
                                    (((COLOR_ITEM)(b)&0x000000ff)<<16)|\
                                    (((COLOR_ITEM)(a)&0x000000ff)<<24))
#define COLOR_MAKE_RGBD(r,g,b)      \
                                    (((COLOR_ITEM)(r)&0x000000ff)|\
                                    (((COLOR_ITEM)(g)&0x000000ff)<<8)|\
                                    (((COLOR_ITEM)(b)&0x000000ff)<<16))
#define COLOR_MAKE_YUVD(y,u,v)      \
                                    (((COLOR_ITEM)(y)&0x000000ff)|\
                                    (((COLOR_ITEM)(u)&0x000000ff)<<8)|\
                                    (((COLOR_ITEM)(v)&0x000000ff)<<16))
#define COLOR_MAKE_YUVA(y,u,v,a)    \
                                    (((COLOR_ITEM)(y)&0x000000ff)|\
                                    (((COLOR_ITEM)(u)&0x000000ff)<<8)|\
                                    (((COLOR_ITEM)(v)&0x000000ff)<<16)|\
                                    (((COLOR_ITEM)(a)&0x000000ff)<<24))
#define COLOR_MAKE_RGB565(r,g,b,a)  \
                                    (\
                                    (((COLOR_ITEM)(b)&0x000000f8)>>3)|\
                                    (((COLOR_ITEM)(g)&0x000000fc)<<3)|\
                                    (((COLOR_ITEM)(r)&0x000000f8)<<8)|\
                                    (((COLOR_ITEM)(a)&0x000000ff)<<16))
//#NT#2009/7/15#JeahYen -end

//color : convert rgb to yuv (aka:CST)
COLOR_ITEM COLOR_RGBD_2_YUVD(COLOR_ITEM rgbd);
//color : convert yuv to rgb (aka:ICST)
COLOR_ITEM COLOR_YUVD_2_RGBD(COLOR_ITEM yuvd);
//#NT#2009/7/15#JeahYen -begin
//color : convert rgbd to rgb565
COLOR_ITEM COLOR_RGBD_2_RGB565(COLOR_ITEM rgbd, UINT32 a);
//color : convert rgba to rgb565
COLOR_ITEM COLOR_RGBA_2_RGB565(COLOR_ITEM rgba);
//#NT#2009/7/15#JeahYen -end



//--------------------------------------------------------------------------------------
//  palette color
//--------------------------------------------------------------------------------------

//<type>

typedef COLOR_ITEM PALETTE_ITEM;

//<enum>

//blend op
#ifndef GX_BLEND_OP
#define GX_BLEND_OP
#define BLEND_OPAQUE_000            0x00    //Output = Under_Layer
#define BLEND_OPAQUE_012            0x01    //Output = This_Layer * 12% + Under_Layer * 88%
#define BLEND_OPAQUE_025            0x02    //Output = This_Layer * 25% + Under_Layer * 75%
#define BLEND_OPAQUE_050            0x03    //Output = This_Layer * 50% + Under_Layer * 50%
#define BLEND_OPAQUE_075            0x04    //Output = This_Layer * 75% + Under_Layer * 25%
#define BLEND_OPAQUE_088            0x05    //Output = This_Layer * 88% + Under_Layer * 12%
#define BLEND_OPAQUE_100            0x06    //Output = This_Layer
#endif //GX_BLEND_OP
#define BLEND_LOGIC_AND             0x07    //Output = This_Layer AND Under_Layer
#define BLEND_LOGIC_OR              0x08    //Output = This_Layer OR Under_Layer
#define BLEND_LOGIC_XOR             0x09    //Output = This_Layer XOR Under_Layer
#define BLEND_LOGIC_INVERT          0x0A    //Output = INVERT Under_Layer
#define BLEND_TRANSPARENT           BLEND_OPAQUE_000
#define BLEND_DEFAULT               BLEND_OPAQUE_100

//blink op
#define BLINK_OFF                   0x00    //No blink effect
#define BLINK_F1                    0x01    //Using blink with frequence 1
#define BLINK_F2                    0x10    //Using blink with frequence 2
#define BLINK_F3                    0x11    //Using blink with frequence 3
#define BLINK_ON                    BLINK_F1    //Using blink effect
#define BLINK_DEFAULT               BLINK_OFF

//<function>

#define PALETTE_COLOR_MASK          ((COLOR_ITEM)0x00ffffff)
#define PALETTE_BLENDOP_MASK        ((COLOR_ITEM)0x0f000000)
#define PALETTE_BLINKOP_MASK        ((COLOR_ITEM)0x30000000)
//#NT#2009/7/15#JeahYen -begin
#define PALETTE_ALPHA_MASK          ((COLOR_ITEM)0xff000000)
//#NT#2009/7/15#JeahYen -end
#define PALETTE_ALL_MASK            ((COLOR_ITEM)0xffffffff)

/*
//inline function
PALETTE_ITEM PALETTE_MAKE(UINT8 blend_op, UINT8 blink_op, COLOR_ITEM color);
COLOR_ITEM PALETTE_GET_COLOR(PALETTE_ITEM pal);
UINT8 PALETTE_GET_BLENDOP(PALETTE_ITEM pal);
UINT8 PALETTE_GET_BLINKOP(PALETTE_ITEM pal);
*/


//palette : set color and attr
#define PALETTE_MAKE(blend_op, blink_op, color)   \
        (\
        (((COLOR_ITEM)(color)) & 0x00ffffff) | \
        ((((COLOR_ITEM)(blend_op)) & 0x0f) << 24) | \
        (((COLOR_ITEM)(blink_op!=0)) << 28) \
        )

//#NT#2009/7/15#JeahYen -begin
//palette : set color and alpha
#define PALETTE_YUVA_MAKE(alpha, color)   \
        (\
        (((COLOR_ITEM)(color)) & 0x00ffffff) | \
        ((((COLOR_ITEM)(alpha)) & 0x000000ff)<<24) \
        )

//palette : set color and alpha
#define PALETTE_RGBA_MAKE(alpha, color)   \
        (\
        (((COLOR_ITEM)(color)) & 0x00ffffff) | \
        ((((COLOR_ITEM)(alpha)) & 0x000000ff)<<24) \
        )
//#NT#2009/7/15#JeahYen -end

//palette : get color, attr and alpha
#define PALETTE_GET_COLOR(dw)       (((COLOR_ITEM)(dw)) & PALETTE_COLOR_MASK)
#define PALETTE_GET_BLENDOP(dw)     ((((COLOR_ITEM)(dw)) & PALETTE_BLENDOP_MASK) >> 24)
#define PALETTE_GET_BLINKOP(dw)     ((((COLOR_ITEM)(dw)) & PALETTE_BLINKOP_MASK) >> 28)
//#NT#2009/7/15#JeahYen -begin
#define PALETTE_GET_ALPHA(dw)       ((((COLOR_ITEM)(dw)) & PALETTE_ALPHA_MASK) >> 24)
//#NT#2009/7/15#JeahYen -end

//palette: set to default (1bpp/2bpp/4bpp/8bpp) palette
RESULT PALETTE_SET_DEFAULT          (UINT32 uiPxlfmt, UINT8 bpp, PALETTE_ITEM* pPalette);

//palette: fill gradient colors to palette
RESULT PALETTE_FILL_GRADIENT        (PALETTE_ITEM* pPalette, COLOR_ITEM color_first, COLOR_ITEM color_last, int nColor);

//--------------------------------------------------------------------------------------
//  color index
//--------------------------------------------------------------------------------------

//<type>

typedef UINT32 MAPPING_ITEM;

//<enum>

//color index: pre-defined of default palette
//                                  index
#define COLOR_INDEX1_BLACK          0
#define COLOR_INDEX1_WHITE          1
//                                  index
#define COLOR_INDEX2_BLACK          0
#define COLOR_INDEX2_GRAY           1
#define COLOR_INDEX2_LIGHTGRAY      2
#define COLOR_INDEX2_WHITE          3
//                                  index
#define COLOR_INDEX4_BLACK          0
#define COLOR_INDEX4_DARKRED        1
#define COLOR_INDEX4_DARKGREEN      2
#define COLOR_INDEX4_DARKYELLOW     3
#define COLOR_INDEX4_DARKBLUE       4
#define COLOR_INDEX4_DARKMAGENTA    5
#define COLOR_INDEX4_DARKCYAN       6
#define COLOR_INDEX4_LIGHTGRAY      7
#define COLOR_INDEX4_GRAY           8
#define COLOR_INDEX4_RED            9
#define COLOR_INDEX4_GREEN          10
#define COLOR_INDEX4_YELLOW         11
#define COLOR_INDEX4_BLUE           12
#define COLOR_INDEX4_MAGENTA        13
#define COLOR_INDEX4_CYAN           14
#define COLOR_INDEX4_WHITE          15
//                                  index
#define COLOR_INDEX8_BLACK          0
#define COLOR_INDEX8_DARKRED        1
#define COLOR_INDEX8_DARKGREEN      2
#define COLOR_INDEX8_DARKYELLOW     3
#define COLOR_INDEX8_DARKBLUE       4
#define COLOR_INDEX8_DARKMAGENTA    5
#define COLOR_INDEX8_DARKCYAN       6
#define COLOR_INDEX8_LIGHTGRAY      7
#define COLOR_INDEX8_PASTELGREEN    8
#define COLOR_INDEX8_PASTELBLUE     9
#define COLOR_INDEX8_SOFTWHITE      246
#define COLOR_INDEX8_MEDIUMGRAY     247
#define COLOR_INDEX8_GRAY           248
#define COLOR_INDEX8_RED            249
#define COLOR_INDEX8_GREEN          250
#define COLOR_INDEX8_YELLOW         251
#define COLOR_INDEX8_BLUE           252
#define COLOR_INDEX8_MAGENTA        253
#define COLOR_INDEX8_CYAN           254
#define COLOR_INDEX8_WHITE          255

//color index: return color index of default 8bpp palette by grayscale
MAPPING_ITEM
        COLOR_INDEX8_GRAYLEVEL(CVALUE gray);
//color index: return color index of default 8bpp palette by red, green, blue
MAPPING_ITEM
        COLOR_INDEX8_COLORRGB(CVALUE r, CVALUE g, CVALUE b);



//--------------------------------------------------------------------------------------
//  decode color from color index
//--------------------------------------------------------------------------------------
COLOR_ITEM INDEX_2_COLOR(UINT8 index, MAPPING_ITEM* pMap, PALETTE_ITEM* pPal);


//--------------------------------------------------------------------------------------
//  color index of font
//--------------------------------------------------------------------------------------

//<enum>

//FONT's pixel index : predefined font color index
#define COLOR_INDEX_FONTBACK        0   //background-> always Transparent
#define COLOR_INDEX_FONTFACE        1   //face      -> Use Face Color
#define COLOR_INDEX_FONTSHADOW      2   //shadow    -> Use Shadow Color
#define COLOR_INDEX_FONTLINE        3   //line      -> Use Line Color

#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_COLOR_H_