
//    Display
/*
    LAYER : Display layer, display a DC to specified window on screen

    DISPLAY : Display device, contain many display layers
*/

#ifndef _GX_DISPLAY_H_
#define _GX_DISPLAY_H_

#include "GxCommon.h"
#include "GxRect.h"
#include "GxDC.h"
#include "GxColor.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------------------
//  type - display
//--------------------------------------------------------------------------------------
typedef struct _LAYER_INIT
{
    //LAYER_STATE_FLAG
    UINT32 uiFlag;

//-input buffer
    //LAYER_STATE_TYPEFMT
    UINT16 uiType;
    UINT16 uiPxlfmt;

    //LAYER_STATE_BUFWIDTH
    UINT32 uiWidth;

    //LAYER_STATE_BUFHEIGHT
    UINT32 uiHeight;

    //LAYER_STATE_BUFSIZE
    UINT32 uiBufSize;

    //LAYER_STATE_BUFADDR0/1/2
    UINT32 pBufAddr[3];     //memory address

    //LAYER_STATE_BUFATTR
    UINT16 uiSwapEffect;    // 1 = DISCARD, 2 = COPY, 3 = FLIP;
    UINT16 uiBufCount;      // 0 = one buffer, 1 = double buffer, 2 = triple buffer...

//-output window
    //LAYER_STATE_WINX
    //LAYER_STATE_WINY
    //LAYER_STATE_WINW
    //LAYER_STATE_WINH
    RECT_2D win;

    //LAYER_STATE_WINATTR
    UINT32  uiWinAttr;  // 0x01 = MIRROR X, 0x02 = MIRROR Y
}
LAYER_INIT;


//--------------------------------------------------------------------------------------
//  enum - display
//--------------------------------------------------------------------------------------
//DISPLAY id
#define LAYER_OSD1                  0
#define LAYER_OSD2                  1
#define LAYER_VDO1                  2
#define LAYER_VDO2                  3
#define LAYER_NUM                   4       //osd1,osd2,vdo1,vdo2
#define LAYER_OUTPUT                4       //output = mix ( osd1, osd2, vdo1, vdo2 )
#define LAYER_MAX                   5       //osd1,osd2,vdo1,vdo2 + output

//DISPLAY state
#define LAYER_STATE_FLAG            0x00    //bit0:valid, bit1:draw (RO)
//-input buffer
#define LAYER_STATE_TYPEFMT         0x01    //uiType and uiPxlfmt (RO)  (init)
#define LAYER_STATE_BUFWIDTH        0x02    //uiWidth (RO)  (init)
#define LAYER_STATE_BUFHEIGHT       0x03    //uiHeight (RO)  (init)
#define LAYER_STATE_BUFSIZE         0x04    //uiBufSize (RO)  (init)
#define LAYER_STATE_BUFADDR0        0x05    //pBufAddr[0] (RO)  (init)
#define LAYER_STATE_BUFADDR1        0x06    //pBufAddr[1] (RO)  (init)
#define LAYER_STATE_BUFADDR2        0x07    //pBufAddr[2] (RO)  (init)
#define LAYER_STATE_BUFATTR         0x08    //uiBufCount and uiSwapEffect (RO)  (init)
//-output window
#define LAYER_STATE_WINX            0x09    //window x (R/W)  (init)
#define LAYER_STATE_WINY            0x0A    //window y (R/W)  (init)
#define LAYER_STATE_WINW            0x0B    //window w (R/W)  (init)
#define LAYER_STATE_WINH            0x0C    //window h (R/W)  (init)
#define LAYER_STATE_WINATTR         0x0D    //uiMirrorEffect (R/W)  (init)
//-status
#define LAYER_STATE_ENABLE          0x0E    //enable/disable
#define LAYER_STATE_INFO            0x0F    //current show/current draw/palette color count (RO)
#define LAYER_STATE_NUM             0x10
//#NT#2009/7/15#JeahYen -begin remove
//#NT#2009/7/15#JeahYen -end remove

//mask for accessing LAYER_STATE_TYPEFMT
#define TYPEFMT_TYPE                0x0000ffff
#define TYPEFMT_TYPE_SHIFT          0
#define TYPEFMT_FMT                 0xffff0000
#define TYPEFMT_FMT_SHIFT           16
#define TYPEFMT_GET_TYPE(v)         (((v) & TYPEFMT_TYPE) >> TYPEFMT_TYPE_SHIFT)
#define TYPEFMT_GET_FMT(v)          (((v) & TYPEFMT_FMT) >> TYPEFMT_FMT_SHIFT)
//make value of LAYER_STATE_TYPEFMT
#define TYPEFMT_MAKE(tp, fmt)       (((fmt) << TYPEFMT_FMT_SHIFT) | ((tp) << TYPEFMT_TYPE_SHIFT))

//flag for SWAPEFFECT
#define SWAPEFFECT_DISCARD          0x00    //Draw on front (Maybe flicker defect)
#define SWAPEFFECT_COPY             0x01    //Copy font->back, draw on back, swap back->front
#define SWAPEFFECT_FLIP             0x02    //Draw on back, swap back->front
//value for SWAPEFFECT
#define SWAPEFFECT_DEFAULT          SWAPEFFECT_DISCARD

//value for BUFCOUNT
#define BUFCOUNT_DEFAULT            ( 0 )

//mask for accessing LAYER_STATE_BUFATTR
#define BUFATTR_SWAPEFFECT          0x0000ffff
#define BUFATTR_SWAPEFFECT_SHIFT    0
#define BUFATTR_BUFCOUNT            0xffff0000
#define BUFATTR_BUFCOUNT_SHIFT      16
#define BUFATTR_GET_BUFCOUNT(v)     (((v) & BUFATTR_BUFCOUNT) >> BUFATTR_BUFCOUNT_SHIFT)
#define BUFATTR_GET_SWAPEFFECT(v)   (((v) & BUFATTR_SWAPEFFECT) >> BUFATTR_SWAPEFFECT_SHIFT)
//make value of LAYER_STATE_BUFATTR
#define BUFATTR_MAKE(swap, bufc)    (((bufc) << BUFATTR_BUFCOUNT_SHIFT) | ((swap) << BUFATTR_SWAPEFFECT_SHIFT))

//value for BUFATTR
#define BUFATTR_DEFAULT             BUFATTR_MAKE(BUFCOUNT_DEFAULT, SWAPEFFECT_DEFAULT)

//flag for MIRROREFFECT
#define MIRROR_DISCARD              0x00    //no mirror
#define MIRROR_X                    0x01    //mirror in x direction
#define MIRROR_Y                    0x02    //mirror in y direction
//value for MIRROREFFECT
#define MIRROR_DEFAULT              (MIRROR_DISCARD)

//mask for accessing LAYER_STATE_WINATTR
#define WINATTR_MIRROR              0x0000ffff
#define WINATTR_GET_MIRROR(v)       ((v) & WINATTR_MIRROR)
//make value of LAYER_STATE_WINATTR
#define WINATTR_MAKE(mirr)          ((mirr) & WINATTR_MIRROR)

//value for WINATTR
#define WINATTR_DEFAULT             WINATTR_MAKE(MIRROR_DEFAULT)

//#NT#2009/7/15#JeahYen -begin remove
//#NT#2009/7/15#JeahYen -end remove
/*
//inline function
UIN16 TYPEFMT_GET_TYPE(UINT32 v);
UIN16 TYPEFMT_GET_FMT(UINT32 v);
UIN16 BUFATTR_GET_BUFCOUNT(UINT32 v);
UIN16 BUFATTR_GET_SWAPEFFECT(UINT32 v);
UIN16 WINATTR_GET_MIRROR(UINT32 v);
void BUFATTR_MAKE(UINT32 bufc, UINT32 swap);
void WINATTR_MAKE(UINT32 mirr);
*/


//#NT#2009/7/15#JeahYen -begin
#define CTRL_STATE_OSD_MIN          0x00
#define CTRL_STATE_OSD1COMPARE      0x00    //color compare func = COMPARE_KEY_NEVER/EQUAL
#define CTRL_STATE_OSD1COLORKEY     0x01    //OSD1 do color key to see VDO_mix: color key = [index]
#define CTRL_STATE_OSD1BLEND        0x02    //blend ctrl of OSD1
#define CTRL_STATE_OSD1ALPHA        0x03    //const alpha of OSD1
#define CTRL_STATE_OSD2COMPARE      0x04    //color compare func = COMPARE_KEY_NEVER/EQUAL
#define CTRL_STATE_OSD2COLORKEY     0x05    //OSD2 do color key to see OSD1: color key = [index]
#define CTRL_STATE_OSD2BLEND        0x06    //blend ctrl of OSD2
#define CTRL_STATE_OSD2ALPHA        0x07    //const alpha of OSD2
#define CTRL_STATE_OSD_MAX          0x07

#define CTRL_STATE_VDO_MIN          0x08
#define CTRL_STATE_VDO1COMPARE      0x08    //color compare func = COMPARE_KEY_NEVER/EQUAL/LESS/GREATER
#define CTRL_STATE_VDO1COLORKEY     0x09    //VDO2 do color key to see VDO1: color key = [color]
#define CTRL_STATE_VDO1BLEND        0x0a    //(reserved)
#define CTRL_STATE_VDO1ALPHA        0x0b    //(reserved)
#define CTRL_STATE_VDO2COMPARE      0x0c    //color compare func = COMPARE_KEY_NEVER/EQUAL/LESS/GREATER
#define CTRL_STATE_VDO2COLORKEY     0x0d    //VDO2 do color key to see VDO1: color key = [color]
#define CTRL_STATE_VDO2BLEND        0x0e    //blend ctrl of VDO2
#define CTRL_STATE_VDO2ALPHA        0x0f    //const alpha of VDO2
#define CTRL_STATE_VDO_MAX          0x0f

#define CTRL_STATE_BK_MIN           0x10
#define CTRL_STATE_BACKCOLOR        0x10    //Background: color
#define CTRL_STATE_ALLCTRL          0x11    //bit 0:Swap VDO1 and VDO2 (color key)
#define CTRL_STATE_BK_MAX           0x11

#define CTRL_STATE_NUM              0x12
//#NT#2009/7/15#JeahYen -end

//flag for CTRL_STATE_OSD_COMPARE/CTRL_STATE_VDO_COMPARE
#define COMPARE_KEY_NEVER           0       //never key
#define COMPARE_KEY_LESS            1       //key if pixel color is less then color key
#define COMPARE_KEY_EQUAL           2       //key if pixel color is equal to color key
#define COMPARE_KEY_GREATER         3       //key if pixel color is grater then color key

//flag for CTRL_STATE_OSD_BLEND/CTRL_STATE_VDO_BLEND
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

//#NT#2009/7/15#JeahYen -begin
//ctrl value for CTRL_STATE_xxxxBLEND
#define BLEND_DISABLE               0x00    //blending disable
#define BLEND_CONSTALPHA            0x01    //blending by layer const alpha
#define BLEND_PIXELALPHA            0x02    //blending by layer pixel alpha
#define BLEND_R1                    0x03    //(reserved)
//#NT#2009/7/15#JeahYen -end


//order mode for GxDisplay_SetOrder()
#define ORDER_NO_OSD                0x01
#define ORDER_OSD1                  0x02    //support OSD1 color-key
#define ORDER_OSD2_OSD1             0x03    //support OSD2/OSD1 color-key and OSD2 alpha-blend
#define ORDER_OSD2                  0x04    //support OSD2 color-key
#define ORDER_NO_VDO                0x10
#define ORDER_VDO1                  0x20
#define ORDER_VDO2_VDO1             0x30    //support VDO2 color-key and VDO2 alpha-blend
#define ORDER_VDO2                  0x40
#define ORDER_VDO1_VDO2             0x50    //support VDO1 color-key and VDO1 alpha-blend

//--------------------------------------------------------------------------------------
//  function - init/exit
//--------------------------------------------------------------------------------------
void GxDisplay_Init             (UINT32 uiDisplayObj, UINT32 uiDispWidth, UINT32 uiDispHeight);
void GxDisplay_Exit             (void);

//--------------------------------------------------------------------------------------
//  function - init layer
//--------------------------------------------------------------------------------------
//layer init function - need load by calling GxDisplay_Flush()
RESULT GxDisplay_InitLayer      (UINT8 LayerID, const LAYER_INIT* pInit, BOOL bClear);
//#NT#2010/3/24#JeahYen -begin
//layer exit function
void GxDisplay_ExitLayer        (UINT8 LayerID);
//#NT#2010/3/24#JeahYen -end
//check layer is already init?
BOOL GxDisplay_IsReady          (UINT8 LayerID);

//--------------------------------------------------------------------------------------
//  function - set layer state
//--------------------------------------------------------------------------------------
//state function - need load by calling GxDisplay_Flush()
void GxDisplay_Set              (UINT8 LayerID, UINT16 nState, UINT32 nValue);
UINT32 GxDisplay_Get            (UINT8 LayerID, UINT16 nState);
void GxDisplay_SetWindow        (UINT8 LayerID, LVALUE x, LVALUE y, LVALUE w, LVALUE h, UINT32 uiMirror);
void GxDisplay_SetSwapEffect    (UINT8 LayerID, UINT32 uiSwapEffect);
void GxDisplay_SetPalette       (UINT8 LayerID, UINT16 uiStartID, UINT16 uiCount, const PALETTE_ITEM* pTable);
void GxDisplay_Flush            (UINT8 LayerID); //this will auto wait VD
//#NT#2010/4/28#JeahYen -begin
void GxDisplay_FlushEx          (UINT8 LayerID, BOOL bWait);
//#NT#2010/4/28#JeahYen -end

void GxDisplay_GetPalette       (UINT8 LayerID, UINT16 uiStartID, UINT16 uiCount, PALETTE_ITEM* pTable);

//--------------------------------------------------------------------------------------
//  function - control layer order and blending
//--------------------------------------------------------------------------------------
//macro state function - need load by calling GxDisplay_FlushAll()
void GxDisplay_SetOrder         (UINT32 uiOrder, UINT32 uiAlphaBlend, UINT32 uiColorKey);
void GxDisplay_SetBack          (UINT32 uiColor);
void GxDisplay_FlushAll         (void); //this will auto wait VD
//#NT#2010/4/28#JeahYen -begin
void GxDisplay_FlushAllEx       (BOOL bWait);
//#NT#2010/4/28#JeahYen -end

//--------------------------------------------------------------------------------------
//  function - support dc draw and buffer flip
//--------------------------------------------------------------------------------------
//  DISPLAY : DC.uiType = TYPE_FB
DC*  GxDisplay_BeginDraw        (UINT8 LayerID);
void GxDisplay_EndDraw          (UINT8 LayerID, DC* pLayerDC);
void GxDisplay_Flip             (BOOL bWait); //update all layers, and show output to screen
void GxDisplay_WaitVD           (void);

//#NT#2009/7/15#JeahYen -begin

//--------------------------------------------------------------------------------------
//  macro - others <for compatible to old version>
//--------------------------------------------------------------------------------------
#define CTRL_STATE_OSDCOMPARE       CTRL_STATE_OSD2COMPARE
#define CTRL_STATE_OSDCOLORKEY      CTRL_STATE_OSD2COLORKEY
#define CTRL_STATE_OSDBLEND         CTRL_STATE_OSD2ALPHA
#define CTRL_STATE_VDOCOMPARE       CTRL_STATE_VDO2COMPARE
#define CTRL_STATE_VDOCOLORKEY      CTRL_STATE_VDO2COLORKEY
#define CTRL_STATE_VDOBLEND         CTRL_STATE_VDO2ALPHA
#define CTRL_STATE_VDOSWAP          CTRL_STATE_ALLCTRL

//#NT#2009/7/15#JeahYen -end

//#NT#2011/07/11#Elvis Chuang -begin
//#NT# Add version info and build date functions
extern CHAR *Gx_GetVerInfo_Display(void);
extern CHAR *Gx_GetBuildDate_Display(void);
//#NT#2011/07/11#Elvis Chuang -end

#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_DISPLAY_H_
