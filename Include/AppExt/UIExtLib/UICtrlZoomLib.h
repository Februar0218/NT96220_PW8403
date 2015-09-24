#ifndef UICTRLZOOMLIB_H
#define UICTRLZOOMLIB_H

#include "UIFrameworkExt.h"

typedef enum
{
    ZOOM_EXTERNAL_RECT =1,         
    ZOOM_INTERNAL_RECT
}ZOOM_POS_ATTRIBUTE_SET;

typedef struct _CTRL_ZOOM_DATA
{
   UINT32 value;
}CTRL_ZOOM_DATA;


typedef enum
{
    ZOOM_VALUE =( CTRL_ZOOM <<8 ),         
    ZOOM_MAX
}ZOOM_DATA_SET;
extern void UxZoom_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxZoom_GetData(VControl *pCtrl,UINT32 attribute);
extern void UxZoom_SetPosition(VControl *pCtrl,UINT32 attribute,Ux_RECT * pRect);

#endif
