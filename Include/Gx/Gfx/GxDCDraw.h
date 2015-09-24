
//    Graphics

/*
    DC : Drawing Canvas object, a bitmap use as a drawing target/source

    BRUSH: Draw brush, a property-set to help DC drawing

    VAR: Draw variable, a variable array to help DC drawing custom items
*/

#ifndef _GX_DCDRAW_H_
#define _GX_DCDRAW_H_

#include "GxCommon.h"
#include "GxRect.h"
#include "GxDC.h"
#include "GxColor.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------------------
//  DC
//--------------------------------------------------------------------------------------

//#NT#2009/8/05#JeahYen -begin
//<struct>

typedef struct _DC_CTRL
{
const RECT_2D* pClipRect;
UINT32 uiRop;
UINT32 uiParam;
const PALETTE_ITEM* pColorTable;
const MAPPING_ITEM* pIndexTable;
}
DC_CTRL;
//#NT#2009/8/05#JeahYen -end


//<function>

//dc (low level) - not support xform/scaling
void _GxGfx_SetClip             (const RECT_2D* pClipRect);
//#NT#2009/8/05#JeahYen -begin
void _GxGfx_SetImageROP         (UINT32 uiRop, UINT32 uiParam, const PALETTE_ITEM* pColorTable, const MAPPING_ITEM* pIndexTable);
//#NT#2009/8/05#JeahYen -end

RESULT _GxGfx_Pixel             (DC* pDestDC, const POINT_2D* pPt, COLOR_ITEM uiColor);
RESULT _GxGfx_Line              (DC* pDestDC, const POINT_2D* pStartPt, const POINT_2D* pEndPt, UINT32 uiLineStyle, COLOR_ITEM uiColor);
RESULT _GxGfx_Ellipse           (DC* pDestDC, const RECT_2D* pRect, const POINT_2D* pRadius, UINT32 uiLineStyle, COLOR_ITEM uiColor, BOOL bFill);
#define _GxGfx_FillEllipse(pDestDC, pRect, pRadius, uiColor)    \
                                _GxGfx_Ellipse(pDestDC, pRect, pRadius, 0, uiColor, TRUE)
#define _GxGfx_FrameEllipse(pDestDC, pRect, pRadius, uiLineStyle, uiColor)    \
                                _GxGfx_Ellipse(pDestDC, pRect, pRadius, uiLineStyle, uiColor, FALSE)
RESULT _GxGfx_Rect              (DC* pDestDC, const RECT_2D* pDestRect, UINT32 uiLineStyle, COLOR_ITEM uiColor, BOOL bFill);
#define _GxGfx_FillRect(pDestDC, pRect, uiColor)    \
                                _GxGfx_Rect(pDestDC, pRect, 0, uiColor, TRUE)
#define _GxGfx_FrameRect(pDestDC, pRect, uiLineStyle, uiColor)    \
                                _GxGfx_Rect(pDestDC, pRect, uiLineStyle, uiColor, FALSE)
RESULT _GxGfx_BitBlt            (DC* pDestDC, const POINT_2D* pDestPt, const DC* pSrcDC, const RECT_2D* pSrcRect);
RESULT _GxGfx_StretchBlt        (DC* pDestDC, const RECT_2D* pDestRect, const DC* pSrcDC, const RECT_2D* pSrcRect);

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GX_DCDRAW_H_
