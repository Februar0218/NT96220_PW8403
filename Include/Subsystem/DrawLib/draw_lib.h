/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       draw_lib.h
    @ingroup    mISYSDraw

    @brief      OSD Draw library

    @note       Nothing.
*/

#ifndef _DRAW_LIB_H
#define _DRAW_LIB_H

#include "Type.h"

/// @name OSD Alignment definition
//@{
#define KH_RIGHT                    0x01    // Horizontal alignment right
#define KH_LEFT                     0x02    // Horizontal alignment left
#define KH_CENTER                   0x04    // Horizontal alignment center
#define KV_TOP                      0x10    // Vertical alignment top
#define KV_BOTTOM                   0x20    // Vertical alignment bottom
#define KV_CENTER                   0x40    // Vertical alignment center
//@}

/// @name Special color mapping index
//@{
#define KOSD_DONT_DRAW              0xFFFF  // Don't draw the pixel
//@}

/// @name The maximum string length
//@{
#define KMAX_STR                    1024    // define maximum string length drawlib can draw
//@}

/// @name The ESC icon defintion
//@{
#define ESC_notify                  0
#define ESC_start                   1
#define ESC_next                    2
#define ESC_end                     3
#define ESC_icon                    4
#define ESC_value                   5
//@}

//#NT#2007/04/16#Ryan Ho -begin
//#The code is moved to draw_lib.c
/*
//#NT#2007/04/03#Ryan Ho -begin
//new function: Macro definition needed by Multi-Line string function
#define min(a, b)  (a<b?a:b)
//#NT#2007/04/03#Ryan Ho -end
*/
//#NT#2007/04/16#Ryan Ho -end

//#NT#2007/04/16#Ryan Ho -begin
//This macro definition is removed
//#define pWideStr(a)  ((pStr[(a<<1)+3]<<8)+(pStr[(a<<1)+2]))
//#NT#2007/04/16#Ryan Ho -end


/// @name OSD mode to draw definition
//@
typedef enum {
    //OSDMODE_1BIT = 0x00,              ///< OSD MODE: 1 pixel = 1-bit
    //OSDMODE_2BIT = 0x01,              ///< OSD MODE: 1 pixel = 2-bit
    OSDMODE_4BIT = 0x02,                ///< OSD MODE: 1 pixel = 4-bit
    OSDMODE_8BIT = 0x03                 ///< OSD MODE: 1 pixel = 8-bit
}OSD_MODE;
//@}

/// @name OSD draw operation
//@
typedef enum {
    OSD_DRAW_FRAME            = 0x00,   ///< OSD draw operation: only draw frame
    OSD_DRAW_FILL             = 0x01,   ///< OSD draw operation: draw solid
    //#NT#2007/04/13#Ryan Ho -begin
    //New item added to support Solid Rectangle but with border
    OSD_DRAW_FILL_WITH_BORDER = 0x02    ///< OSD draw operation: draw solid with border
    //#NT#2007/04/13#Ryan Ho -end
}OSD_DRAW_OP;
//@}

/// @name The paramter defintion of YCbCr Color
//@{
typedef struct {
    UINT8       uiY;                    ///< Y color component
    UINT8       uiCB;                   ///< Cb color component
    UINT8       uiCR;                   ///< Cr color component
    UINT8       uireserved;             ///< padding to word alignment
}YCBCRColor;
//@}

/// @name The paramter defintion of circle
//@{
typedef struct _CIRCLE
{
    UINT16      uiCenX;                 ///< circle center X
    UINT16      uiCenY;                 ///< circle center Y
    UINT16      uiRadius;               ///< circle radius
}CIRCLE, *PCIRCLE;

/// @name The paramter defintion of circle to draw
//@{
typedef struct _T_DRAW_CIRCLE
{
    UINT8       *pOSDBuf;               ///< start address of osd buffer
    UINT8       uiColorIndex;           ///< which color to fill
    UINT16      uiBufWidth;             ///< the buffer width
    UINT16      uiBufHeight;            ///< the buffer height
    CIRCLE      *pCircle;               ///< the effect circle
    UINT8       uiBorder;
}T_DRAW_CIRCLE, *PT_DRAW_CIRCLE;
//@}

/// @name The paramter defintion of line
//@{
typedef struct _LINE
{
    UINT32      uiColorIndex;           ///< Which color to draw the line
    UINT32      uiDot;                  ///< How many dots of this line
    UINT32      uiStartX;               ///< Start point X position
    UINT32      uiStartY;               ///< Start point Y position
    UINT32      uiEndX;                 ///< End point X position
    UINT32      uiEndY;                 ///< End point Y position
}LINE, *PLINE;
//@}

/// @name The paramter defintion of dot to draw
//@{
typedef struct _T_DRAW_LINE
{
    UINT8       *pOSDBuf;               ///< start address of osd buffer
    UINT16      uiBufWidth;             ///< the buffer width
    UINT16      uiBufHeight;            ///< the buffer height
    PLINE       pline;                  ///< the start/end point of the line
}T_DRAW_LINE, *PT_DRAW_LINE;
//@}

/// @name The paramter defintion of rectangle to draw
//@{
typedef struct {
    UINT8       *pOSDBuf;               ///< start address of osd buffer
    UINT16      uiBufWidth;             ///< the buffer width
    UINT16      uiBufHeight;            ///< the buffer height
    RECT        *pDisplayRect;          ///< the effect rectangle
    UINT8       uiColorIndex;           ///< which color to fill
    //#NT#2007/04/13#Ryan Ho -begin
    //New attribute for RoundRectangle / Rectangle to draw a solid entity with different border color
    UINT8       uiBorderColorIndex;     ///< which color of border to draw
    //#NT#2007/04/13#Ryan Ho -end
    UINT8       uiBorder;               ///< the border size, un-used in fill rectangle
    UINT16      uiRadius;               ///< draw round rectangle use
}T_DRAW_RECT, *PT_DRAW_RECT;
//@}

/// @name The paramter defintion of Icon header
//@{
typedef struct {
    UINT32      uiOffset;              ///< offset in database.
    UINT16      uiWidth;               ///< icon width.
    UINT16      uiHeight;              ///< icon height.
}ICON_HEADER, *PICON_HEADER;
//@}

/// @name The paramter defintion of Icon database
//@{
typedef struct {
    ICON_HEADER const   *pIconHeader;       ///< point tp icon header
    UINT8       const   *pIconData;         ///< point to icon data
    UINT16              uiNumOfIcon;        ///< how many icon in this icon data base
    UINT8               uiBitPerPixel;      ///< bit perpixel of this icon DB, it can be 1, 2 or 4 bits per pixel
    UINT8               uiDrawStrOffset;    ///< Data base first item offset
}ICON_DB, *PICON_DB;
//@}

/// @name The paramter defintion of Icon to draw
//@{
typedef struct {
    ICON_DB const   *pIconDB;          ///< which icon database.
    UINT8           *pOSDBuf;          ///< start address of OSD buffer.
    UINT8           *pTmpOSDBuf;       ///< temp OSD Buffer
    RECT            *pDisplayRect;     ///< display rectangle, If this parameter is NULL, it will use whole OSD buffer width and height as display area. In this situation, the "Alignment" will determine the icon's display position.
    UINT16          *MappingArray;     ///< color mapping array, this should be 2,4,16 element long. While this field is NULL, the display color will one-to-one mapping to hardware setting.
    UINT16          uiIconID;          ///< which icon of database
    UINT8           uiAlignment;       ///< alignment for display rectangle,You can use logical or to combine two direction. In horizotal direction can be KH_RIGHT,KH_LEFT, KH_CENTER, vertical direction can be KV_TOP, KV_BOTTOM or KV_CENTER.The default alignment is Left-Top.
    UINT16          uiBufWidth;        ///< Buffer width
    UINT16          uiBufHeight;       ///< Buffer height
} ICON_PARAMETER, *PICON_PARAMETER;
//@}

// This palette structure will be removed next generation, don't use this structure any more
/// @name The paramter defintion of palette
//@{
typedef struct {
    UINT32      Entry;                 ///< OSD entry index
    YCBCRColor  OSDColor;              ///< OSD color data
    UINT32      Blend_OP;              ///< Blending operation
    UINT32      Blink;                 ///< Blinking attribute
    UINT32      OSDColorLong;          ///< OSD color data word value
}OSD_PALETTEL_ENTRY, *POSD_PALETTEL_ENTRY;
//@}

// This palette structure will be removed next generation, don't use this structure any more
/// @name The paramter defintion of palette
//@{
typedef struct {
    UINT32      Color;                 ///< color entry data
    UINT32      Index;                 ///< which color to fill
    UINT8       Opacity;               ///< blending attribute
}OSD_PALETTE, *POSD_PALETTE;
//@}


// APIs
extern void     DrawOSD_SetMode(OSD_MODE OSDMode);
extern OSD_MODE DrawOSD_GetMode(void);

extern void     DrawOSD_SetESCIcon(UINT16 ui, ICON_DB *pIconDB, UINT16 uiIconID, UINT8 *puiMappingArray);
extern BOOL     DrawOSD_ParseStrESC(const char *pStrESC, BOOL bisWideStr, UINT16 uiOldi, UINT16 *puiNewi, UINT16 *puiIconValue);

extern ER       DrawOSD_Clear(UINT8 *pOSDBuf, UINT16 uiWidth, UINT16 uiHeight);
extern ER       DrawOSD_DrawLine(T_DRAW_LINE *pParam);
extern ER       DrawOSD_DrawCircle(T_DRAW_CIRCLE *pDrawcircle, OSD_DRAW_OP drawosd_op);
extern ER       DrawOSD_DrawRoundRect(T_DRAW_RECT *pParam, OSD_DRAW_OP drawosd_op);
extern ER       DrawOSD_DrawRect(T_DRAW_RECT *pParam, OSD_DRAW_OP drawosd_op);
extern ER       DrawOSD_CopyRect(T_DRAW_RECT *pParam, RECT *pDesRect);
extern ER       DrawOSD_DrawIcon(ICON_PARAMETER *pIconParam);
extern ER       DrawOSD_DrawStr(const char *pStr, ICON_PARAMETER *pIconParam, UINT8 uiVStrAlignment, UINT32 *puitotal_width);
//#NT#2007/04/03#Ryan Ho -begin
//Parameters : UINT16 uiMax_chr_num, UINT32 *puitotal_width, UINT32 *puidraw_char are removed.
//extern ER       DrawOSD_DrawStrMultiLine(const char *pStr, ICON_PARAMETER *pIconParam, UINT8 uiVStrAlignment, UINT16 uiMax_chr_num, UINT32 *puitotal_width, UINT32 *puidraw_char);
extern ER       DrawOSD_DrawStrMultiLine(const char *pStr, ICON_PARAMETER *pIconParam, UINT8 uiVStrAlignment);
//#NT#2007/04/03#Ryan Ho -end

//#NT#2007/04/16#Ryan Ho -begin
//The Boolean parameter isWideStr is moved behind the pointer pStr
//#NT#2007/04/03#Ryan Ho -begin
//new function: BOOL parameter isWideStr is new requested.
//extern UINT16   DrawOSD_GetStrLen(const char *pStr);
extern UINT16   DrawOSD_GetStrLen(const char *pStr, BOOL isWideStr);
//#NT#2007/04/03#Ryan Ho -end
//#NT#2007/04/16#Ryan Ho -end

extern void     DrawOSD_GetStrSize(const char *pStr, ICON_PARAMETER *pIconParam, SIZE* pSize);
extern UINT16   DrawOSD_GetStrWidth(const char *pStr, ICON_PARAMETER *pIconParam);
extern UINT16   DrawOSD_GetStrHeight(const char *pStr, ICON_PARAMETER *pIconParam);
extern ER       DrawOSD_ScaleOSDIcon(UINT32 uiIconDstW, UINT32 uiIconDstH, ICON_PARAMETER *pParameter);
extern ER       DrawOSD_ScaleOSDStr(char *pChar, UINT32 uiStrSrcW, UINT32 uiStrSrcH, UINT32 uiStrDstW, UINT32 uiStrDstH, ICON_PARAMETER *pParameter);

//#NT#2007/04/16#Ryan Ho -begin
//#The code is moved to compatsysApi.h
/*
// These APIs are removed, don't call this APIs any more, leave these for old version compatible
#define         DrawOSD_DrawRectSub(a)                 DrawOSD_DrawRect(a, OSD_DRAW_FRAME)
#define         DrawOSD_FillRectSub(a)                 DrawOSD_DrawRect(a, OSD_DRAW_FILL)
#define         DrawOSD_PutIconSub(a)                  DrawOSD_DrawIcon(a)
#define         DrawOSD_PutStrSub(a, b, c, d)          DrawOSD_DrawStr(a, b, c, d)
#define         DrawOSD_PutLongStrSub(a, b, c, d, e)   DrawOSD_DrawStrMultiLine(a, b, c, d, e)
*/
//#NT#2007/04/16#Ryan Ho -end
#endif /* _DRAW_LIB_H */

