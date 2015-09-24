#ifndef UICTRLSCROLLBARLIB_H
#define UICTRLSCROLLBARLIB_H

#include "UIFrameworkExt.h"

typedef enum
{
    SCRBAR_VERTICAL = 0x00,      // horizontal scroll bar
    SCRBAR_HORIZONTAL           // vertical scroll
}SCRLBAR_TYPE;


typedef enum
{
    SCRBAR_FIXSIZE = 0x00,       // scroll bar rectangle define by user
    SCRBAR_STEP_AVG             // scroll bar rectangle calculate by total step
}SCRLBAR_SHAPE;

typedef struct _CTRL_SCRBAR_DATA
{
    SCRLBAR_TYPE    scrlBarType;    // Scroll bar type : horizontal / vertical
    SCRLBAR_SHAPE   scrlBarShape;   // Fix size / calculate by average step rectangle
    UINT32          currentStep;    // value , current value index
    UINT32          totalStep;      // Max step index
    UINT32          pageStep;
}CTRL_SCRBAR_DATA;
typedef enum
{
    SCRBAR_BACKGROUND = 0x00,
    SCRBAR_UP,
    SCRBAR_DOWN,
    SCRBAR_THUMBNAIL,
    SCRBAR_GROUP_MAX
}SCRBAR_GROUP;

typedef enum
{
    SCRBAR_TYPE =( CTRL_SCROLLBAR <<8 ),
    SCRBAR_SHAPE,        
    SCRBAR_CURSTP,        
    SCRBAR_TOTSTP,   
    SCRBAR_PAGESTP,
    SCRBAR_RECT,
    SCRBAR_THUMB,
    SCRBAR_DATA_SET_MAX
}SCRBAR_DATA_SET;
extern void UxScrollBar_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxScrollBar_GetData(VControl *pCtrl,UINT32 attribute);
extern void UxScrollBar_GetRange(VControl *pCtrl,Ux_RECT *pRange);

#endif
