#ifndef UICTRLLISTLIB_H
#define UICTRLLISTLIB_H

#include "UIFrameworkExt.h"


typedef struct _CTRL_LIST_ITEM_DATA
{
   UINT32 stringID;
   UINT32 iconID;
   UINT32 StatusFlag; 
}CTRL_LIST_ITEM_DATA;
typedef struct _CTRL_LIST_DATA
{
    UINT32  currentItem;            // Current selected data
    UINT32  totalItem;              // Total data count
    UINT32  pageItem;
    UINT32  style;   // menu disable (normal/skip/hidden)  menu item style (cycling/stop)
    UINT32  action;
    CTRL_LIST_ITEM_DATA **item;
    UINT32 reserved;
    UINT32 pExeEvent;    
}CTRL_LIST_DATA;



#define LIST_LAYOUT_MASK         0xF
#define LIST_LAYOUT_VERTICAL     0x0
#define LIST_LAYOUT_HORIZONTAL   0x1
#define LIST_LAYOUT_ARRAY        0x2

#define LIST_SCROLL_END_MASK     0xF0
#define LIST_SCROLL_STOP         0x00
#define LIST_SCROLL_CYCLE        0x10

#define LIST_DISABLE_SCRL_MASK   0xF00
#define LIST_DISABLE_NORMAL      0x000
#define LIST_DISABLE_SKIP        0x100

#define LIST_DISABLE_MASK        0xF000
#define LIST_DISABLE_SHOW        0x0000
#define LIST_DISABLE_HIDE        (0x1000|LIST_DISABLE_SKIP) // if hide item,must skip item

#define LIST_SCROLL_MASK         0xF0000
#define LIST_SCROLL_NEXT_PAGE    0x00000
#define LIST_SCROLL_NEXT_ITEM    0x10000

#define LIST_DRAW_MASK           0xF00000
#define LIST_DRAW_IMAGE          0x000000
#define LIST_DRAW_IMAGE_LIST     0x100000

typedef enum
{
    LSTITM_STRID =  (CTRL_LIST<<8),
    LSTITM_ICONID,
    LSTITM_STATUS, 
    LSTITM_MAX
}LSTITM_DATA_SET;

typedef enum
{
    LST_CURITM =LSTITM_MAX +1,           
    LST_TOTITM,             
    LST_PAGEITEM,
    LST_STYLE,   
    LST_ACTION,
    LST_RESERVED,
    LST_EVENT,
    LST_MAX
}LST_DATA_SET;
extern void UxList_SetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute,UINT32 value);
extern UINT32 UxList_GetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute);
extern void UxList_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxList_GetData(VControl *pCtrl,UINT32 attribute);
extern void UxList_GetRange(VControl *pCtrl,Ux_RECT *pRect);    
extern UINT32 UxList_FindItem(VControl *pCtrl, UINT32 x,UINT32 y);    

#endif
