#ifndef UICTRLPROGRESSBARLIB_H
#define UICTRLPROGRESSBARLIB_H

#include "UIFrameworkExt.h"

#define PROBAR_TYPE_MASK       0x0F
#define PROBAR_VERTICAL        0x00
#define PROBAR_HORIZONTAL      0x01

#define PROBAR_DIR_MASK     0xF0
#define PROBAR_NORMAL       0x00
#define PROBAR_INVERSE      0x10

typedef struct _CTRL_PROBAR_DATA
{
    UINT32  progressBarType;   // Slider bar type : vertical / horizontal
    UINT32  currentStep;                // value , current value index
    UINT32  totalStep;                  // Max step index
}CTRL_PROBAR_DATA;

typedef enum
{
    PROBAR_TYPE= ( CTRL_PROGRESSBAR <<8 ),
    PROBAR_CURSTP,           
    PROBAR_TOTSTP,             
    PROBAR_MAX
}PROBAR_DATA_SET;
extern void UxProgressBar_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxProgressBar_GetData(VControl *pCtrl,UINT32 attribute);
#endif
