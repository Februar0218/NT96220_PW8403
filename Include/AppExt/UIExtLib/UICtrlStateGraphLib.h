#ifndef UICTRLSTATEGRAPHLIB_H
#define UICTRLSTATEGRAPHLIB_H

#include "UIFrameworkExt.h"

typedef struct _CTRL_STATEGRAPH_DATA
{
    UINT32  StatusFlag;            
    UINT32 pExeEvent;
    UINT32 totalChangeStatus;
}CTRL_STATEGRAPH_DATA;

typedef enum
{
    STATEGRAPH_STATUS=( CTRL_STATEGRAPH<<8 ),
    STATEGRAPH_EVENT,           
    STATEGRAPH_TOTSTA,             
    STATEGRAPH_MAX
}STATEGRAPH_DATA_SET;

extern void UxStateGraph_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxStateGraph_GetData(VControl *pCtrl,UINT32 attribute);
#endif
