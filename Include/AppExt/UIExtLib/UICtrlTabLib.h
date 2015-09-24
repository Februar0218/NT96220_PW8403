#ifndef UICTRLTABLIB_H
#define UICTRLTABLIB_H

typedef struct _CTRL_TAB_DATA
{
    UINT32 focus;
    UINT32 total;
}CTRL_TAB_DATA;

typedef enum
{
    TAB_FOCUS =( CTRL_TAB <<8 ), 
    TAB_TOTAL,    
    TAB_MAX
}TAB_DATA_SET;

extern void UxTab_SetData(VControl *pCtrl,UINT32 attribute,UINT32 value);
extern UINT32 UxTab_GetData(VControl *pCtrl,UINT32 attribute);



#endif
