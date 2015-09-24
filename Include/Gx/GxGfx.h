
//    Graphics

/*
    DC : Drawing Canvas object, a bitmap use as a drawing target/source

    BRUSH: Draw brush, a property-set to help DC drawing

    VAR: Draw variable, a variable array to help DC drawing custom items
*/

#ifndef _GX_GRAPHICS_H_
#define _GX_GRAPHICS_H_

//Text buffer config:
#define STRING_BUFFER_SIZE      256
#define PRINTF_BUFFER_SIZE      32

#include "GxCommon.h"
#include "GxRect.h"
#include "GxDC.h"
#include "GxData.h"
#include "GxColor.h"

#ifdef __cplusplus
extern "C" {
#endif


//--------------------------------------------------------------------------------------
//  enum - brush
//--------------------------------------------------------------------------------------
//shape
#define BR_SHAPELINESTYLE       0x00  //shape line style: line style, brush weight, brush type //(TODO) line style
#define BR_SHAPEFILLSTYLE       0x01  //shape rect style: draw line, fill inner
#define BR_SHAPEFORECOLOR       0x02  //shape fore color (line color)
#define BR_SHAPEBACKCOLOR       0x03  //shape back color (fill color)
#define BR_SHAPEMAPPING         0x04  //shape color mapping table
#define BR_SHAPELAYOUT          0x05  //shape layout style (TODO)
#define BR_SHAPEALIGN           0x06  //shape alignment control (TODO)
#define BR_SHAPETABLE           0x07  //shape table (TODO)
//image
#define BR_IMAGEROP             0x08  //image ROP
#define BR_IMAGEPARAM           0x09  //image parameters for ROP
#define BR_IMAGEPALETTE         0x0A  //image palette = color LUT for mapping index to color
#define BR_IMAGEMAPPING         0x0B  //image mapping = index LUT for mapping index to index
#define BR_IMAGELAYOUT          0x0C  //image layout (TODO)
#define BR_IMAGEALIGN           0x0D  //image align
#define BR_IMAGETABLE           0x0E  //image table
//text
#define BR_TEXTFONT             0x0F  //text font table
#define BR_TEXTFONTSTYLE        0x10  //text font style: font style, font effect //(TODO) font style
#define BR_TEXTFONTSIZE         0x11  //text font size (TODO)
#define BR_TEXTFORECOLOR1       0x12  //text font face color
#define BR_TEXTFORECOLOR2       0x13  //text font shadow color
#define BR_TEXTFORECOLOR3       0x14  //text font line color (TODO)
#define BR_TEXTLAYOUT           0x15  //text layout: Line-break, Use-Margin
#define BR_TEXTALIGN            0x16  //text align: line, vertical, horizontal
#define BR_TEXTLINEHEIGHT       0x17  //text line height
#define BR_TEXTLETTERSPACE      0x18  //text letter space
#define BR_TEXTINDENTSPACE      0x19  //text indent space (TODO)
#define BR_TEXTSTRINGTABLE      0x1A  //text string table

//--------------------------------------------------------------------------------------
//  enum - brush value
//--------------------------------------------------------------------------------------
//line style
#define LINESTYLE_LINE          0  //line style : ----------------
#if 0
#define LINESTYLE_DOT           1  //line style : . . . . . . . .
#define LINESTYLE_DASH          2  //line style : -- -- -- -- --
#define LINESTYLE_DOTDASH       3  //line style : . -- . -- . -- .
#define LINESTYLE_DOTDOTDASH    4  //line style : . . -- . . -- .
#define LINESTYLE_DOTDASHDASH   5  //line style : . -- -- . -- --
#define LINESTYLE_SPARSEDOT     6  //line style : .   .   .   .
#define LINESTYLE_SPARSEDASH    7  //line style : --  --  --  --
#endif
#define DSF_LS(ls)              ((UINT32)(ls) & 0x07)
#define DSF_LS_MASK             0x00000007
#define GET_LINESTYLE(ls)       ((ls) & DSF_LS_MASK)

//line weight: 0~15, weight=1+lw
#define DSF_LW(lw)              (((UINT32)(lw) & 0xff)<<8)
#define DSF_LW_MASK             0x0000ff00
#define LINEWEIGHT(lw)          DSF_LW(lw)
#define LINEWEIGHT_MAX          31
#define GET_LINEWEIGHT(lw)      (((lw) & DSF_LW_MASK) >> 8)

//line brush
#define DSF_LB(lt)              (((UINT32)(lt) & 0x0f)<<4)
#define DSF_LB_MASK             0x000000f0
#define LINEBRUSH_CIRCLE        0x00000000
#define LINEBRUSH_SQUARE        0x00000010
#define GET_LINEBRUSH(lb)       (((lb) & DSF_LB_MASK) >> 4)

//fill style
#define FILLSTYLE_FILL          0x00000000  //enable fill color at rect background
#define FILLSTYLE_BORDER        0x00000000  //enable draw frame at rect conner
#define FILLSTYLE_EMPTY         0x00000001  //disable fill color
#define FILLSTYLE_NOBORDER      0x00000002  //disable draw frame
//#NT#2009/7/15#JeahYen -begin
#define FILLSTYLE_FILLALPHA     0x00000010  //fill alpha channel
#define FILLSTYLE_FILLRED       0x00000020  //fill color.r channel
#define FILLSTYLE_FILLGREEN     0x00000040  //fill color.g channel
#define FILLSTYLE_FILLBLUE      0x00000080  //fill color.b channel
#define FILLSTYLE_FILLCOLOR     (FILLSTYLE_FILLRED|FILLSTYLE_FILLGREEN|FILLSTYLE_FILLBLUE)
//#NT#2009/7/15#JeahYen -end

//scale
#define SCALE_1X                0     //scale=1.00
#if 0 //TODO
#define SCALE_2X                100   //scale=2.00
#define SCALE_3X                200   //scale=3.00
#define SCALE_4X                300   //scale=4.00
#endif //TODO

//image rop
#define ROP_COPY                0x00000000 //(Support INDEX,YUV format)
#define ROP_NOT                 0x01000000 //(Support INDEX,YUV format)
#define ROP_AND                 0x02000000 //(Support INDEX,YUV format)
#define ROP_OR                  0x03000000 //(Support INDEX,YUV format)
#define ROP_XOR                 0x04000000 //(Support INDEX,YUV format)
#define ROP_KEY                 0x05000000 //(Support INDEX,YUV format) param: color = 0x0~0xfffff
#define ROP_DESTNOT             0x06000000 //(Support INDEX,YUV format)
#define ROP_DESTKEY             0x07000000 //(Support INDEX,YUV format) param: color = 0x0~0xfffff
#define ROP_DESTFILL            0x08000000 //(Support INDEX,YUV format) param: color = 0x0~0xfffff
#define ROP_ADD                 0x10000000 //(Support YUV format only)  sub-type: SRC_DIV***
#define ROP_SUB                 0x11000000 //(Support YUV format only)  sub-type: SRC_DIV***
#define ROP_ABSSUB              0x12000000 //(Support YUV format only)  sub-type: SRC_DIV***
#define ROP_MUL                 0x13000000 //(Support YUV format only)  sub-type: OUT_DIV***
#define ROP_BLEND               0x14000000 //(Support YUV format only)  sub-type: SRC_ALPHA***
#define ROP_FONT                0x20000000 //(Support INDEX format only) [always rop=ROP_KEY and colorkey =0]
#define ROP_ROTATE              0x40000000 //(Support INDEX,YUV format) sub-type: SRC_RCW***,SRC_RCCW***,SRC_MRR***
#define ROP_MASK                0xFF000000

//#NT#2009/10/30#JeahYen -begin
//image rop - subtype for ROP_COPY, ROP_KEY with ARGB image
#define SRC_ALPHA(a)            (0x00ff0000 | ((a)&0x000000ff))
//#NT#2009/10/30#JeahYen -end
//image rop - subtype for ROP_ADD, ROP_SUB, ROP_ABSSUB
#define SRC_DIV_1               0x00000000 //B = SRC
#define SRC_DIV_2               0x00001000 //B = SRC / 2
#define SRC_DIV_4               0x00002000 //B = SRC / 4
#define SRC_DIV_8               0x00003000 //B = SRC / 8
#define SRC_DIV_MASK            0x00003000
#define SRC_DIV_SHIFT           12
//image rop - subtype for ROP_BLEND
#define SRC_ALPHA_012           0x00000000 //OUT= 0.88 x A + 0.12 x B
#define SRC_ALPHA_025           0x00010000 //OUT= 0.75 x A + 0.25 x B
#define SRC_ALPHA_038           0x00020000 //OUT= 0.62 x A + 0.38 x B
#define SRC_ALPHA_050           0x00030000 //OUT= 0.50 x A + 0.50 x B
#define SRC_ALPHA_062           0x00040000 //OUT= 0.38 x A + 0.62 x B
#define SRC_ALPHA_075           0x00050000 //OUT= 0.25 x A + 0.75 x B
#define SRC_ALPHA_088           0x00060000 //OUT= 0.50 x A + 0.88 x B
#define SRC_ALPHA_100           0x00070000 //OUT= B
#define SRC_ALPHA_MASK          0x00070000
#define SRC_ALPHA_SHIFT         16
//image rop - subtype for ROP_MUL
#define OUT_DIV_1               0x00000000 //A' = OUT
#define OUT_DIV_2               0x00080000 //A' = OUT / 2
#define OUT_DIV_4               0x00100000 //A' = OUT / 4
#define OUT_DIV_8               0x00180000 //A' = OUT / 8
#define OUT_DIV_16              0x00200000 //A' = OUT / 16
#define OUT_DIV_32              0x00280000 //A' = OUT / 32
#define OUT_DIV_64              0x00400000 //A' = OUT / 64
#define OUT_DIV_128             0x00480000 //A' = OUT / 128
#define OUT_DIV_MASK            0x00480000
#define OUT_DIV_SHIFT           19
//image rop - subtype for ROP_ROTATE
#define SRC_RCW_90              0x00000000 //rotate CW 90 degree
#define SRC_RCW_180             0x01000000 //rotate CW 180 degree
#define SRC_RCW_270             0x02000000 //rotate CW 270 degree
#define SRC_MRR_X               0x04000000 //mirror in x (horizontal flip)
#define SRC_MRR_Y               0x08000000 //mirror in y (vertical flip)
#define SRC_RCCW_90             SRC_RCW_270 //rotate CCW 270 degree
#define SRC_RCCW_180            SRC_RCW_180 //rotate CCW 180 degree
#define SRC_RCCW_270            SRC_RCW_90 //rotate CCW 90 degree
//image rop - subtype for scaling
#define FILTER_NONE             0x00000000
#define FILTER_PIXEL            0x00400000 //(TODO)
#define FILTER_LINEAR           0x00800000
#define FILTER_CUBIC            0x00C00000 //(TODO)
#define FILTER_MASK             0x00C00000
#define FILTER_SHIFT            22

//layout
#define LAYOUT_NORMAL           0x00000000
#define LAYOUT_LINEBREAK        0x00000100
#define LAYOUT_LINEWRAP         0x00000200
#if 0 //TODO
#define LAYOUT_ROTATE_L         0x00000001
#define LAYOUT_ROTATE_R         0x00000002
#define LAYOUT_MIRROR_X         0x00000004
#define LAYOUT_MIRROR_Y         0x00000008
#define LAYOUT_REPEAT_X         0x00000010
#define LAYOUT_REPEAT_Y         0x00000020
#define LAYOUT_MARGIN           0x00000400
#endif //TODO
//#NT#2010/02/26#Janice --begin
#define LAYOUT_CLIPPING         0x00000800
//#NT#2010/02/26#Janice --end

//alignment
#define ALIGN_LEFT              0x00000000  //horizontal alignment : left
#define ALIGN_CENTER            0x00000001  //horizontal alignment : center
#define ALIGN_RIGHT             0x00000002  //horizontal alignment : right
#define ALIGN_JUSTIFY           0x00000003  //horizontal alignment : justify (distributed)
#define ALIGN_H_MASK            0x00000003
#define ALIGN_TOP               0x00000000  //vertical alignment : top
#define ALIGN_MIDDLE            0x00000010  //vertical alignment : center
#define ALIGN_BOTTOM            0x00000020  //vertical alignment : bottom
#define ALIGN_MASK              0x00000030
#define ALIGN_V_MASK            0x00000030

//font style
#define FONTSTYLE_NORMAL        0x00000000  //font style : normal (use color index 1)
#if 0 //TODO
#define FONTSTYLE_ITALIC        0x00000001  //font style : italic (use color index 1)
#define FONTSTYLE_BOLD          0x00000002  //font style : bold  (use color index 1)
#define FONTSTYLE_UNDERLINE     0x00000004  //font style : underline (use color index 3)
#define FONTSTYLE_STRIKEOUT     0x00000008  //font style : strikeout (use color index 3)
#define DSF_FS(fs)              (((UINT32)(fs) & 0x000000ff))
#define DSF_FS_MASK             0x000000ff
#endif //TODO

//font effect
#define FONTEFFECT_NONE         0x00000000  //
#define FONTEFFECT_SHADOW       0x00000100  //shadow (use color index 2)
#define FONTEFFECT_SHADOW2      0x00000200  //shadow2 (use color index 2)
#define FONTEFFECT_HIGHLIGHT    0x00000300  //highlight (use color index 2)
#define FONTEFFECT_HIGHLIGHT2   0x00000400  //highlight2 (use color index 2)
#define FONTEFFECT_OUTLINE      0x00000500  //outline (use color index 2)
#define FONTEFFECT_OUTLINE2     0x00000600  //outline2 (use color index 2)
#define F_NOEXTEND              0x00008000  //no auto extend
#define DSF_FE(fs)              ((((UINT32)(fs) & 0x00007f00))>>8)
#define DSF_FE_MASK             0x0000ff00

//text alignment
#define LETTERALIGN_TOP         0x00000000  //letter alignment : top
#define LETTERALIGN_BASELINE    0x00000100  //letter alignment : baseline = (bottom-2)
#define LETTERALIGN_BOTTOM      0x00000200  //letter alignment : bottom
#define LETTERALIGN_MASK        0x00000300

//--------------------------------------------------------------------------------------
//  enum - brush default value
//--------------------------------------------------------------------------------------
//shape
#define SHAPELINESTYLE_DEFAULT  (LINESTYLE_LINE|LINEBRUSH_CIRCLE|LINEWEIGHT(0)) //(TODO) line style
#define SHAPEFILLSTYLE_DEFAULT  (FILLSTYLE_FILL|FILLSTYLE_BORDER)
#define SHAPEFORECOLOR_DEFAULT  0
#define SHAPEBACKCOLOR_DEFAULT  0
#define SHAPEMAPPING_DEFAULT    0
#define SHAPELAYOUT_DEFAULT     0  //(TODO)
#define SHAPEALIGN_DEFAULT      0  //(TODO)
#define SHAPETABLE_DEFAULT      0  //(TODO) //default shape table?
//image
#define IMAGEROP_DEFAULT        (ROP_COPY|FILTER_NONE)
#define IMAGEPARAM_DEFAULT      0
#define IMAGEPALETTE_DEFAULT    0
#define IMAGEMAPPING_DEFAULT    0
#define IMAGELAYOUT_DEFAULT     (LAYOUT_NORMAL)  //(TODO)
#define IMAGEALIGN_DEFAULT      (ALIGN_LEFT|ALIGN_TOP)
#define IMAGETABLE_DEFAULT      0 //default image table? (only 1 image : [NULL])
//text
#define TEXTFONT_DEFAULT        0 //default font?
#define TEXTFONTSTYLE_DEFAULT   (FONTSTYLE_NORMAL|FONTEFFECT_NONE) //(TODO) font style
#define TEXTFONTSIZE_DEFAULT    SCALE_1X  //(TODO)
#define TEXTFORECOLOR1_DEFAULT  0
#define TEXTFORECOLOR2_DEFAULT  0
#define TEXTFORECOLOR3_DEFAULT  0 //(TODO)
#define TEXTLAYOUT_DEFAULT      (LAYOUT_NORMAL)
#define TEXTALIGN_DEFAULT       (LETTERALIGN_TOP|ALIGN_LEFT|ALIGN_TOP)
#define TEXTLINEHEIGHT_DEFAULT  SCALE_1X
#define TEXTLETTERSPACE_DEFAULT 0
#define TEXTINDENTSPACE_DEFAULT 0 //(TODO)
#define TEXTSTRINGTABLE_DEFAULT 0 //default string table? (only 1 string : [NULL])

//--------------------------------------------------------------------------------------
//  enum - shape type
//--------------------------------------------------------------------------------------
//shape type
#define SHAPE_NONE              0x00
#define SHAPE_LINE_TOP          0x01
#define SHAPE_LINE_BOTTOM       0x02
#define SHAPE_LINE_LEFT         0x03
#define SHAPE_LINE_RIGHT        0x04
#define SHAPE_LINE_BACKSLASH    0x05 // '\'
#define SHAPE_LINE_SLASH        0x06 // '/'
#define SHAPE_LINE              SHAPE_LINE_BACKSLASH
#define SHAPE_FILLRECT          0x10
#define SHAPE_FRAMERECT         0x11
#define SHAPE_INVERTRECT        0x12
#define SHAPE_RECTANGLE         0x13
#define SHAPE_ELLIPSE           0x20
#define SHAPE_ROUNDRECT_1       0x30    //rw = w
#define SHAPE_ROUNDRECT_2       0x31    //rw = 1/2 w
#define SHAPE_ROUNDRECT_3       0x32    //rw = 1/3 w
#define SHAPE_ROUNDRECT_4       0x33    //rw = 1/4 w
#define SHAPE_ROUNDRECT_5       0x34    //rw = 1/5 w
#define SHAPE_ROUNDRECT_6       0x35    //rw = 1/6 w
#define SHAPE_ROUNDRECT_7       0x36    //rw = 1/7 w
#define SHAPE_ROUNDRECT_8       0x37    //rw = 1/8 w
#define SHAPE_ROUNDRECT         SHAPE_ROUNDRECT_1

//--------------------------------------------------------------------------------------
//  type - brush
//--------------------------------------------------------------------------------------
//<type>

typedef UINT32  BVALUE; //bursh property value

#define BR_STATE_NUM            0x20

typedef struct _BRUSH
{
    BVALUE uiState[BR_STATE_NUM];
}
BRUSH;

#define FLOAT_TO_BVALUE(fv)     (*(BVALUE*)&(fv))       //convert float to BVALUE

//--------------------------------------------------------------------------------------
//  function - init/exit
//--------------------------------------------------------------------------------------
//init & exit
void GxGfx_Init                 (UINT32* pWorkBuf, UINT32 nWorkBufSize); //NOTE: pWorkBuf is need to word alignment
void GxGfx_Exit                 (void);
//work stack
void GxGfx_ResetStack           (void);
UINT32* GxGfx_PushStack         (UINT32 uiSize);
void GxGfx_PopStack             (void);
//dc work stack
RESULT GxGfx_PushStackDC        (DC* pDC, UINT16 uiType, UINT16 uiPxlfmt, UINT32 uiWidth, UINT32 uiHeight);
void GxGfx_PopStackDC           (DC* pDC);

//--------------------------------------------------------------------------------------
//  function - set/get brush state and variable
//--------------------------------------------------------------------------------------
//control brush
void GxGfx_GetAll               (BRUSH* pBrush);
void GxGfx_SetAll               (const BRUSH* pBrush);//if pBrush == 0, using default brush
void GxGfx_SetAllDefault        (void);
/*
//inline functions:
void   GxGfx_Set                (UINT8 uiProperty, BVALUE uiValue);
BVALUE GxGfx_Get                (UINT8 uiProperty);
*/
void BR_SetP                    (UINT8 uiProperty, BVALUE uiValue);
#define GxGfx_Set(n, p)         BR_SetP(n, (BVALUE)(p))
BVALUE BR_GetP                  (UINT8 uiProperty);
#define GxGfx_Get(n)            BR_GetP(n)
//var
/*
//inline functions:
void GxGfx_SetVar               (UINT8 nVarId, BVALUE nValue);
BVALUE GxGfx_GetVar             (UINT8 nVarId);
*/
void VAR_SetV                   (UINT8 nVarId, BVALUE nValue);
#define GxGfx_SetVar(n, v)      VAR_SetV(n, (BVALUE)(v))
BVALUE VAR_GetV                 (UINT8 nVarId);
#define GxGfx_GetVar(n)         VAR_GetV(n)

//--------------------------------------------------------------------------------------
//  function - set dc unit/origin/window/pos
//--------------------------------------------------------------------------------------
//dc origin point - for support xform (shift)
RESULT GxGfx_SetOrigin          (DC* pDC, LVALUE x, LVALUE y);
POINT_2D GxGfx_GetOrigin        (const DC* pDC);
//dc coordinate rect - for support xform (shift+scale)
RESULT GxGfx_SetCoord           (DC* pDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RECT_2D GxGfx_GetCoord          (const DC* pDC);
//dc clipping window
RESULT GxGfx_SetWindow          (DC* pDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RECT_2D GxGfx_GetWindow         (const DC* pDC);
//dc move pen
RESULT GxGfx_MoveTo             (DC* pDestDC, LVALUE x, LVALUE y);
RESULT GxGfx_MoveBack           (DC* pDestDC);
POINT_2D GxGfx_GetPos           (const DC* pDestDC);
SIZE_2D GxGfx_GetLastMove       (const DC* pDestDC);

//--------------------------------------------------------------------------------------
//  function - clear/copy/convert dc
//--------------------------------------------------------------------------------------
//dc clear/copy/convert
RESULT GxGfx_Clear              (DC* pDestDC);
RESULT GxGfx_Copy               (DC* pDestDC, const DC* pSrcDC);
RESULT GxGfx_CopyEx             (DC* pDestDC, const DC* pSrcDC, UINT32 uiRop, UINT32 uiParam);
RESULT GxGfx_Convert            (DC* pDestDC, const DC* pSrcDC, const void* pConvertTable);

//--------------------------------------------------------------------------------------
//  function - draw shape on dc
//--------------------------------------------------------------------------------------
//control shape brush
void GxGfx_SetShapeStroke       (BVALUE uiLineStyle, BVALUE uiFillStyle);
void GxGfx_SetShapeColor        (BVALUE uiForeColor, BVALUE uiBackColor, const MAPPING_ITEM* pTable);
void GxGfx_SetShapeLayout       (BVALUE uiLayout, BVALUE uiAlignment);
//dc draw shape
RESULT GxGfx_Point              (DC* pDestDC, LVALUE x, LVALUE y);
RESULT GxGfx_LineTo             (DC* pDestDC, LVALUE x, LVALUE y);
RESULT GxGfx_Line               (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RESULT GxGfx_FillRect           (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RESULT GxGfx_FrameRect          (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RESULT GxGfx_InvertRect         (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RESULT GxGfx_Rectangle          (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RESULT GxGfx_RectTo             (DC* pDestDC, LVALUE x, LVALUE y); //like GxGfx_Rectangle()
RESULT GxGfx_Ellipse            (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
RESULT GxGfx_RoundRect          (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, LVALUE rw, LVALUE rh);
//dc draw shape in rect
RESULT GxGfx_ShapeInRect        (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, UINT8 uiShapeType);

//--------------------------------------------------------------------------------------
//  function - draw image on dc
//--------------------------------------------------------------------------------------
//control image brush
void GxGfx_SetImageStroke       (BVALUE uiRop, BVALUE uiParam);
void GxGfx_SetImageColor        (const PALETTE_ITEM* pPalette, const MAPPING_ITEM* pTable);
void GxGfx_SetImageLayout       (BVALUE uiLayout, BVALUE uiAlignment);
void GxGfx_SetImageTable        (const IMAGE_TABLE* pTable); //(set current IMAGE_TABLE)
//get image content
RESULT GxGfx_GetImageIDSize     (IVALUE img_id, SIZE_2D* pImageSize);//(refe to current IMAGE_TABLE)
const IMAGE* GxGfx_GetImageID   (IVALUE img_id);//(image of current IMAGE_TABLE)
const IMAGE* GxGfx_GetImageIDEx (IVALUE img_id, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2); //(sub-image of current IMAGE_TABLE)
//dc draw image
RESULT GxGfx_Image              (DC* pDestDC, LVALUE x, LVALUE y, const IMAGE* pImage);
RESULT GxGfx_ImageScale         (DC* pDestDC, LVALUE x, LVALUE y, LVALUE w, LVALUE h, const IMAGE* pImage);
//dc draw image in rect
RESULT GxGfx_ImageInRect        (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const IMAGE* pImage);
RESULT GxGfx_ImageScaleInRect   (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, LVALUE w, LVALUE h, const IMAGE* pImage);

//--------------------------------------------------------------------------------------
//  function - draw text on dc
//--------------------------------------------------------------------------------------
//control text brush
void GxGfx_SetTextStroke        (const FONT* pFont, BVALUE uiFontStyle, BVALUE uiScale); //(set current FONT)
void GxGfx_SetTextColor         (BVALUE uiForeColor1, BVALUE uiForeColor2, BVALUE uiForeColor3);
void GxGfx_SetTextLayout        (BVALUE uiLayout, BVALUE uiAlignment, BVALUE lh, BVALUE cs, BVALUE is);
void GxGfx_SetStringTable       (const STRING_TABLE* pTable); //(set current STRING_TABLE)
//get text content
RESULT GxGfx_GetFontSize        (SIZE_2D* pFontSize);//(refer to current FONT)
RESULT GxGfx_GetFontCharSize    (IVALUE chr_id, SIZE_2D* pCharSize);//(refer to current FONT)
const TCHAR* GxGfx_GetStringID  (IVALUE str_id);//(string of current STRING_TABLE)
//dc draw text
RESULT GxGfx_Char               (DC* pDestDC, LVALUE x, LVALUE y, IVALUE id);
RESULT GxGfx_Text               (DC* pDestDC, LVALUE x, LVALUE y, const TCHAR* pszSrc); //no ESC command (fast)
RESULT GxGfx_TextPrint          (DC* pDestDC, LVALUE x, LVALUE y, const TCHAR* pszSrc, ...); //no ESC command (fast)
//dc draw text in rect
RESULT GxGfx_TextInRect         (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const TCHAR* pszSrc); //support ESC command
RESULT GxGfx_TextPrintInRect    (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const TCHAR* pszSrc, ...); //support ESC command
//get text size
POINT_2D GxGfx_GetTextLastLoc   (void);
SIZE_2D GxGfx_GetTextLastSize   (void);

//--------------------------------------------------------------------------------------
//  function - others <for compatible to old version>
//--------------------------------------------------------------------------------------
#define BR_TEXTSCALE            BR_TEXTFONTSIZE
#define TEXTSCALE_DEFAULT       TEXTFONTSIZE_DEFAULT
#define BRUSHWEIGHT(lw)         LINEWEIGHT(lw)
#define GET_BRUSHWEIGHT(lw)     GET_LINEWEIGHT(lw)
#define BRUSHTYPE_CIRCLE        LINEBRUSH_CIRCLE
#define BRUSHTYPE_SQUARE        LINEBRUSH_SQUARE
#define GET_BRUSHTYPE(lb)       GET_LINEBRUSH(lb)
#define LINEHEIGHT_1X           SCALE_1X
#define BR_IMAGEKEYCOLOR        BR_IMAGEPALETTE
#define IMAGEKEYCOLOR_DEFAULT   IMAGEPALETTE_DEFAULT
#define GxGfx_TextRect          GxGfx_TextInRect
#define GxGfx_TextPrintRect     GxGfx_TextPrintInRect
#define GxGfx_GetImageIDRect    GxGfx_GetImageIDEx
#define GxGfx_GetCharSize       GxGfx_GetFontCharSize
#define GxGfx_GetEscImageSize   GxGfx_GetImageIDSize
#define GxGfx_TextChar          GxGfx_Char
#define GxGfx_ImageRect         GxGfx_ImageInRect
#define GxGfx_ImageScaleRect(pDC, x1, y1, x2, y2, pImg) \
                                GxGfx_ImageScale(pDC, (x1), (y1), (x2)-(x1)+1, (y2)-(y1)+1, pImg)
#define LAYOUT_LINEWARP         LAYOUT_LINEWRAP

//#NT#2011/07/11#Elvis Chuang -begin
//#NT# Add version info and build date functions
extern CHAR *Gx_GetVerInfo_GxGfx(void);
extern CHAR *Gx_GetBuildDate_GxGfx(void);
//#NT#2011/07/11#Elvis Chuang -end

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GX_GRAPHICS_H_
