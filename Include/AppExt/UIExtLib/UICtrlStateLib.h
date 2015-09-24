#ifndef UICTRLSTATELIB_H
#define UICTRLSTATELIB_H

#include "UIFrameworkExt.h"

typedef struct _CTRL_STATE_ITEM_DATA
{
   UINT32 stringID;
   UINT32 iconID;
   UINT32 pExeEvent;
}CTRL_STATE_ITEM_DATA;

typedef struct _CTRL_STATE_DATA
{
    UINT32  currentItem;            // Current selected data    
    UINT32  totalItem;              // Total data count   
    CTRL_STATE_ITEM_DATA **item;
}CTRL_STATE_DATA;


typedef enum
{
    STATE_ITEM_STRID=( CTRL_STATE <<8 ),
    STATE_ITEM_ICONID,
    STATE_ITEM_EVENT,
    STATE_ITEM_MAX
}STATE_ITEM_DATA_SET;

typedef enum
{
    STATE_CURITEM= STATE_ITEM_MAX +1 ,          
    STATE_TOTITEM,             
    STATE_MAX
}STATE_DATA_SET;
extern void UxState_SetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute,UINT32 value);
extern UINT32 UxState_GetItemData(VControl *pCtrl,UINT32 index,UINT32 attribute);
extern void UxState_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxState_GetData(VControl *pCtrl,UINT32 attribute);

#endif
