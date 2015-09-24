#ifndef UICTRLSLIDERLIB_H
#define UICTRLSLIDERLIB_H

#include "UIFrameworkExt.h"

typedef enum
{
    SLIDER_VERTICAL = 0x00,         // horizontal slider
    SLIDER_HORIZONTAL              // vertical slider
}SLIDER_TYPE_SET;

typedef struct _CTRL_SLIDER_DATA
{
    SLIDER_TYPE_SET sliderType;         // Slider bar type : vertical / horizontal
    UINT32  currentStep;            // value , current value index
    UINT32  totalStep;              // Max step index
    //BOOL    bEnable;
    //BOOL    bShow;
}CTRL_SLIDER_DATA;



typedef enum
{
    SLIDER_BACKGROUND = 0x00,
    SLIDER_UP,
    SLIDER_DOWN,
    SLIDER_THUMBNAIL,

    SLIDER_TOTAL_GROUP
}SLIDER_GROUP;

typedef enum
{
    SLIDER_TYPE=( CTRL_SLIDER <<8 ),
    SLIDER_CURSTP,
    SLIDER_TOTSTP,
    SLIDER_MAX
}SLIDER_DATA_SET;
extern void UxSlider_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxSlider_GetData(VControl *pCtrl,UINT32 attribute);
#endif
