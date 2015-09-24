
#ifndef UIDRAW_SHOWOBJ_H
#define UIDRAW_SHOWOBJ_H
#include "GxGfx.h"
typedef struct _DC_CMD
{
    UINT8       uiCmd;
    UINT16      x1;
    UINT16      y1;
    UINT16      x2;
    UINT16      y2;
    UINT32      param1;
    UINT32      param2;

}DC_CMD;


typedef struct _DC_STY
{
    UINT8       uiCmd;
    UINT32      param1;
    UINT32      param2;
    UINT32      param3;
    UINT32      param4;
    UINT32      param5;
    UINT32      param6;
}DC_STY;

#define DECLARE_BASE   \
  INT16      ItemType;\
  INT16      x1;      \
  INT16      y1;      \
  INT16      x2;      \
  INT16      y2;

typedef struct _ITEM_BASE{
    DECLARE_BASE
}ITEM_BASE;

#define DECLARE_SHAPE_BASE    \
  UINT32  uiLineStyle;  \
  UINT32  uiFillStyle;  \
  UINT32  uiForeColor;  \
  UINT32  uiBackColor;  \
  UINT32  uiLayout;     \
  UINT32  uiAlignment;  \
  void    *uiColorMapTable;

typedef struct _ITEM_RECTANGLE{
    DECLARE_BASE
    DECLARE_SHAPE_BASE
}ITEM_RECTANGLE;

typedef struct _ITEM_ROUNDRECT{
    DECLARE_BASE
    DECLARE_SHAPE_BASE
    UINT16   RoundX;
    UINT16   RoundY;
}ITEM_ROUNDRECT;

typedef struct _ITEM_ELLIPSE{
    DECLARE_BASE
    DECLARE_SHAPE_BASE
}ITEM_ELLIPSE;

typedef struct _ITEM_LINE{
    DECLARE_BASE
    UINT32  uiLineStyle;
    UINT32  uiFillStyle;
    UINT32  uiLineColor;
    void    *uiColorMapTable;
}ITEM_LINE;

typedef struct _ITEM_IMAGE{
    DECLARE_BASE
    UINT32  Content;
    UINT32  uiBltROP;
    UINT32  uiParamROP;
    UINT32  uiColorKey;
    void    *uiColorMapTable;
    UINT32  uiLayout;
    UINT32  uiCLipping;
    UINT32  uiAlignment;
    void    *pImageTable;
}ITEM_IMAGE;

typedef struct _ITEM_TEXT{
    DECLARE_BASE
    UINT32  Content;
    void    *uiFontTable;
    UINT32  uiFontStyle;
    UINT32  uiFillStyle;
    UINT32  uiFontSize;
    UINT32  uiTextColor;
    UINT32  uiShadowColor;
    UINT32  uiLineColor;
    UINT32  uiLayout;
    UINT32  uiCLipping;
    UINT32  uiAlignment;
    UINT16  uiLineHeight;
    UINT16  uiLetterSpace;
    UINT16  uiIndentSpace;
    void    *pStringTable;
}ITEM_TEXT;



typedef struct _ITEM_GROUP{
    DECLARE_BASE
    struct _ITEM_BASE** ShowTable;
    UINT32  uiBackColor;
}ITEM_GROUP;

#define CMD_DrawNull      0
#define CMD_Rectangle     1
#define CMD_RoundRect     2
#define CMD_Ellipse       3
#define CMD_Line          4
#define CMD_Image         5
#define CMD_Text          6
#define CMD_Group         7
#define CMD_SKIN          8
#define CMD_MAX           9

#define CMD_MASK         0x00FF
#define CMD_EXT_MASK  0xFF00

#define CMD_ITEM           0x0100
#define CMD_VALUE         0x0200

#define STY_Rectangle     1
#define STY_RoundRect     2
#define STY_Ellipse       3
#define STY_Line          4
#define STY_Image         5
#define STY_Text          6


#define TEXT_POINTER    0x10000000
#define Txt_Pointer(txt)       ((UINT32)txt|TEXT_POINTER)


#define STRID_NULL       0
#define ICONID_NULL     0xFFFFFFFF
typedef struct _SHOW_ELEMENT
{
   DC_CMD showCmd;
   DC_STY showSty;
} SHOW_ELEMENT;

typedef enum
{
  PRO_ICONID = 0x0,
  PRO_STRID,
  PRO_STRCOLOR,
  PRO_SHAPE_COLOR,
  SHOWOBJ_PROPERTY_TYPE_MAX
}SHOWOBJ_PROPERTY_TYPE;

typedef void (*SHOWOBJ_FUNC_PTR)(UIScreen ScreenObj,ITEM_BASE* pShowObj);

extern void Ux_SetOrigin(UIScreen ScreenObj,LVALUE x,LVALUE y);
extern POINT_2D Ux_GetOrigin(UIScreen ScreenObj);
extern void Ux_DrawShowObj(UIScreen ScreenObj,ITEM_BASE* pShowObj);
extern void Ux_setShowObjProperty(ITEM_BASE *pShowObj,UINT32 propertyIndex,UINT32 value);
extern void Ux_getShowObjPos(ITEM_BASE *pShowObj,INT16 * x1,INT16 * y1,INT16 * x2,INT16 * y2);
extern void Ux_setShowObjPos(ITEM_BASE *pShowObj,INT16 x1,INT16 y1,INT16 x2,INT16 y2);
extern INT32 Ux_DrawShowTable(UIScreen ScreenObj,ITEM_BASE** ShowTable);
extern void Ux_DrawItemByStatus(UIScreen ScreenObj,ITEM_BASE* pShowObj,UINT32 stringID,UINT32 iconID,UINT32 value);
extern void Ux_SetShowObjFunc(UINT32 cmd,SHOWOBJ_FUNC_PTR pFunc);


#endif
