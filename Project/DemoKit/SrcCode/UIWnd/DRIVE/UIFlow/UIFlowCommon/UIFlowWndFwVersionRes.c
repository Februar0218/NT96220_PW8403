//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDFWVERSIONRES_H
#define UIFLOWWNDFWVERSIONRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_ITEM_RECT(CMD_Rectangle,Rect5,0,0,219,49,16,0,0,14,0,0,0)
SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_BODY(Rect5)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,219,49)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text7,0,0,219,49,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal2)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_BODY(Text7)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal2,50,92,269,141)

SHOW_MAP_BEGIN(UIFlowWndFwVersion_StaticText)
SHOW_MAP_BODY(Normal2)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndFwVersion_StaticText)
CREATE_STATIC_DATA(UIFlowWndFwVersion_StaticText,STRID_NULL)
CREATE_STATIC_CTRL(UIFlowWndFwVersion_StaticText,0,50,92,269,141)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Rect12,0,0,219,49,16,0,0,14,0,0,0)
SHOW_MAP_BEGIN(Skin10)
SHOW_MAP_BODY(Rect12)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin10,0,0,219,49)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text14,0,0,219,49,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal9)
SHOW_MAP_BODY(Skin10)
SHOW_MAP_BODY(Text14)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal9,50,140,269,189)

SHOW_MAP_BEGIN(UIFlowWndConfigFwVersion_StaticText)
SHOW_MAP_BODY(Normal9)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndConfigFwVersion_StaticText)
CREATE_STATIC_DATA(UIFlowWndConfigFwVersion_StaticText,STRID_NULL)
CREATE_STATIC_CTRL(UIFlowWndConfigFwVersion_StaticText,0,50,140,269,189)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndFwVersion)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndFwVersion)
DECLARE_CTRL_LIST(UIFlowWndFwVersion)
CREATE_CTRL(UIFlowWndFwVersion,UIFlowWndFwVersion,CTRL_WND,NULL,0 ,0,0,319,239)
#endif