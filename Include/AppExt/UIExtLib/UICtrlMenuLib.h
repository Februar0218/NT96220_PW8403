#ifndef UICTRLMENULIB_H
#define UICTRLMENULIB_H

#include "UIFrameworkExt.h"

typedef struct _CTRL_MENU_ITEM_DATA
{
   UINT32 stringID;
   UINT32 iconID;
   UINT32 StatusFlag; 
   UINT32 pExeEvent;
   VControl *pBindCtrl;
   UINT32   value;
}CTRL_MENU_ITEM_DATA;
typedef struct _CTRL_MENU_DATA
{
    UINT32  currentItem;            // Current selected data
    UINT32  totalItem;              // Total data count
    UINT32  pageItem;
    UINT32  style;   // menu disable (normal/skip/hidden)  menu item style (cycling/stop)
    UINT32  action;
    CTRL_MENU_ITEM_DATA **item;
    UINT32 reserved;
}CTRL_MENU_DATA;

#define MENU_LAYOUT_MASK         0xF
#define MENU_LAYOUT_VERTICAL     0x0
#define MENU_LAYOUT_HORIZONTAL   0x1

#define MENU_SCROLL_END_MASK     0xF0
#define MENU_SCROLL_STOP         0x00
#define MENU_SCROLL_CYCLE        0x10

#define MENU_DISABLE_SCRL_MASK   0xF00
#define MENU_DISABLE_NORMAL      0x000
#define MENU_DISABLE_SKIP        0x100
#define MENU_DISABLE_MASK        0xF000
#define MENU_DISABLE_SHOW        0x0000
#define MENU_DISABLE_HIDE        (0x1000|MENU_DISABLE_SKIP) // if hide item,must skip item

#define MENU_SCROLL_MASK         0xF0000
#define MENU_SCROLL_NEXT_PAGE    0x00000
#define MENU_SCROLL_NEXT_ITEM    0x10000
#define MENU_SCROLL_CENTER       0x20000

#define MENU_DRAW_MASK           0xF00000
#define MENU_DRAW_IMAGE          0x000000
#define MENU_DRAW_IMAGE_LIST     0x100000
#define MENU_DRAW_IMAGE_TABLE    0x200000

typedef enum
{
    MNUITM_STRID,
    MNUITM_ICONID,
    MNUITM_STATUS, 
    MNUITM_EVENT,
    MNUITM_CTRL,
    MNUITM_VALUE,
    MNUITM_MAX
}MNUITM_DATA_SET;

typedef enum
{
    MNU_CURITM,           
    MNU_TOTITM,             
    MNU_PAGEITEM,
    MNU_STYLE,   
    MNU_ACTION,
    MNU_RESERVED,    //if MENU_SCROLL_NEXT_ITEM,reserved would keep scrollStartIndex
    MNU_MAX
}MNU_DATA_SET;
extern void UxMenu_SetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute,UINT32 value);
extern UINT32 UxMenu_GetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute);
extern void UxMenu_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxMenu_GetData(VControl *pCtrl,UINT32 attribute);
extern void UxMenu_GetRange(VControl *pCtrl,Ux_RECT *pRect);    
extern void UxMenu_GetItemPos(VControl *pCtrl,UINT32 index,Ux_RECT *rect);

#endif
