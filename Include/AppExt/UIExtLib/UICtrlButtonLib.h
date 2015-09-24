#ifndef UICTRLBUTTONLIB_H
#define UICTRLBUTTONLIB_H
#include "UIFrameworkExt.h"

#define BUTTON_ONE      0   //button only one item,always get first one


#define BTN_DRAW_MASK    0xF00000
#define BTN_DRAW_IMAGE       0x000000
#define BTN_DRAW_IMAGE_LIST     0x100000 // 



typedef struct _CTRL_BUTTON_ITEM_DATA
{
   UINT32 stringID;
   UINT32 iconID;
   UINT32 StatusFlag;  
   UINT32 pExeEvent;
}CTRL_BUTTON_ITEM_DATA;

typedef struct _CTRL_BUTTON_DATA
{
    //UINT32  currentItem;            // Current selected data
    //UINT32  totalItem;              // Total data count
    UINT32  style; 
    CTRL_BUTTON_ITEM_DATA **item;
    //UINT32 totalChangeStatus;
}CTRL_BUTTON_DATA;


typedef enum
{
    BTNITM_STRID =  (CTRL_BUTTON<<8),
    BTNITM_ICONID,
    BTNITM_STATUS, 
    BTNITM_EVENT,
    BTNITM_MAX
}BTNITM_DATA_SET;

typedef enum
{
    BTN_STYLE =BTNITM_MAX +1,           
    BTN_MAX
}BTN_DATA_SET;
extern void UxButton_SetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute,UINT32 value);
extern UINT32 UxButton_GetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute);
extern void UxButton_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxButton_GetData(VControl *pCtrl,UINT32 attribute);
#endif
