#ifndef UICTRLSTATICLIB_H
#define UICTRLSTATICLIB_H

#include "UIFrameworkExt.h"

typedef struct _CTRL_STATIC_DATA
{
   UINT32 value;
}CTRL_STATIC_DATA;


typedef enum
{
    STATIC_VALUE =( CTRL_STATIC <<8 ),         
    STATIC_MAX
}STATIC_DATA_SET;
extern void UxStatic_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxStatic_GetData(VControl *pCtrl,UINT32 attribute);
#endif
