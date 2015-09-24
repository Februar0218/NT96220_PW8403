
#ifndef UIFRAMEWORK_EXT_H
#define UIFRAMEWORK_EXT_H
#include "GxGfx.h"

#define ERR_TYPE_MSG   " type error,is"
#define ERR_ATTRIBUTE_MSG  "no this attribute"
#define ERR_NO_SYSOBJ_MSG  "no system obj"
#define ERR_OUT_RANGE_MSG  "index out of range\r\n"
#define ERR_VALUE_MSG      "value error\r\n"
#define ERR_TYPE            (-1)
#define ERR_ATTRIBUTE      (-2)
#define ERR_OUT_RANGE      (-3)

typedef NVTRET (* NVTMENU_FUNC_PTR)(UINT32);
#define TP_EVENT 0x10000000
typedef enum
{
    NVTEVT_UNFOCUS = 32,
    NVTEVT_FOCUS,
    NVTEVT_CHANGE_STATE,
    NVTEVT_PREVIOUS_ITEM,
    NVTEVT_NEXT_ITEM,
    NVTEVT_FIRST_ITEM,
    NVTEVT_LAST_ITEM,
    NVTEVT_PRESS_ITEM,
    NVTEVT_NEXT_STEP,
    NVTEVT_PREVIOUS_STEP,
    CONTROL_EVENT_MAX
}CONTROL_EVENT;

typedef enum
{
    NVTEVT_PRESS = CONTROL_EVENT_MAX + TP_EVENT,
    NVTEVT_RELEASE,
    NVTEVT_MOVE,
    NVTEVT_CLICK,
    NVTEVT_DOUBLECLICK,
    NVTEVT_SLIDE_LEFT,
    NVTEVT_SLIDE_RIGHT,
    //#NT#2009/08/28#Ben Wang -begin
    NVTEVT_SLIDE_UP,
    NVTEVT_SLIDE_DOWN,
    //#NT#2009/08/28#Ben Wang -end
    NVTEVT_COMPONENT_TYPE_MAX
}CONTROL_TP_VENT;

typedef enum
{
    CTRL_WND = 0x0,
    CTRL_PANEL,
    CTRL_STATIC,
    CTRL_STATE,
    CTRL_STATEGRAPH,
    CTRL_TAB,
    CTRL_BUTTON,
    CTRL_MENU,
    CTRL_LIST,
    CTRL_SLIDER,
    CTRL_SCROLLBAR,
    CTRL_PROGRESSBAR,
    CTRL_ZOOM,
    CTRL_SYS,
    CONTROL_TYPE_MAX
}CONTROL_TYPE;

typedef enum
{
    STATUS_NORMAL = 0x00,
    STATUS_FOCUS,
    STATUS_NORMAL_DISABLE,
    STATUS_FOCUS_DISABLE,
    STATUS_FOCUS_PRESS,
    STATUS_SETTIMG_MAX
}STATUS_SETTIMG;



#define STATUS_FOCUS_MASK 0x0F
#define STATUS_NORMAL_BIT    0x00
#define STATUS_FOCUS_BIT      0x01
#define STATUS_ENABLE_MASK 0xF0
#define STATUS_ENABLE   0x00
#define STATUS_DISABLE  0x10

#define STATUS_VALUE        0xF00
#define STATUS_TEXT          0x100
#define STATUS_ICON          0x200
#define STATUS_CONTROL   0x200

#define CURITEM_INDEX  0xFFFFFFFF
#define OUTRANGE_INDEX  0xFFFFFFFE
//////////////////////////////////////////////////
extern INT32 Ux_EventProcExt(VControl *pCtrlObj, NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);
extern void Ux_SetSysObj(VControl *pCtrlObj);
extern VControl *Ux_GetSysObj(void);
extern void Ux_SetSysDefCmd(EVENT_ENTRY *CmdMap);

extern void Ux_SetFontTable(FONT** pFont);
extern FONT* Ux_GetFontTable(UINT32 index);
extern void Ux_SetPaletteTable(MAPPING_ITEM **pPaletteTable);
extern PALETTE_ITEM* Ux_GetPaletteTable(UINT32 index);
extern void Ux_SetColorMapTable(MAPPING_ITEM **pColorMapTable);
extern MAPPING_ITEM *Ux_GetColorMapTable(UINT32 index);
#include "UICtrlButtonLib.h"
#include "UICtrlListLib.h"
#include "UICtrlMenuLib.h"
#include "UICtrlPanelLib.h"
#include "UICtrlProgressBarLib.h"
#include "UICtrlScrollBarLib.h"
#include "UICtrlSliderLib.h"
#include "UICtrlStateGraphLib.h"
#include "UICtrlStateLib.h"
#include "UICtrlStaticLib.h"
#include "UICtrlTabLib.h"
#include "UICtrlWndLib.h"
#include "UICtrlZoomLib.h"
#include "UIDrawShowObj.h"
#endif
