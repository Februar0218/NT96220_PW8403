//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIFlowWndWrnMsgRes.c"
#include "UIFlow.h"
#include "PrimaryTsk.h"
//-----------------------------------------------------------------------------
// UIFlowWndWrnMsg User Variable & Function Declarations
//-----------------------------------------------------------------------------
static UINT32 g_uiWrnMsgIssue       = 0;
static UINT32 g_uiWrnMsgExpTime     = 0;
static UINT32 g_uiExpTimerID        = NULL_TIMER;
static UINT32 g_uiWrnMsgPseudoTimerID = NULL_TIMER;

//---------------------UIFlowWndWrnMsgCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndWrnMsg)
CTRL_LIST_ITEM(UIFlowWndWrnMsg_StatusTXT_Msg)
CTRL_LIST_END

//----------------------UIFlowWndWrnMsgCtrl Event---------------------------
INT32 UIFlowWndWrnMsg_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWrnMsg_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWrnMsg_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWrnMsg_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWrnMsg_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWrnMsg_OnKeyPlayback(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWrnMsg_OnTimer(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWrnMsg_OnKeyDisplay(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndWrnMsg)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndWrnMsg_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndWrnMsg_OnClose)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIFlowWndWrnMsg_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_MODE,UIFlowWndWrnMsg_OnKeyMode)
EVENT_ITEM(NVTEVT_KEY_MENU,UIFlowWndWrnMsg_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_PLAYBACK,UIFlowWndWrnMsg_OnKeyPlayback)
EVENT_ITEM(NVTEVT_KEY_DISPLAY,UIFlowWndWrnMsg_OnKeyDisplay)
EVENT_ITEM(NVTEVT_TIMER,UIFlowWndWrnMsg_OnTimer)
EVENT_END

INT32 UIFlowWndWrnMsg_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    debug_ind(("UIFlowWndWrnMsg: OnOpen\r\n"));

    if(paramNum > 0)
    {
        g_uiWrnMsgIssue = paramArray[0];
        debug_ind(("UIFlowWndWrnMsg: Issue case = %d\r\n", g_uiWrnMsgIssue));
        if(paramNum > 1)
        {
            g_uiWrnMsgExpTime = paramArray[1];
            debug_err(("UIFlowWndWrnMsg: Exp time = %d\r\n", g_uiWrnMsgExpTime));
        }
    }

    debug_err(("UIFlowWndWrnMsg_OnOpen g_uiWrnMsgIssue = %d\n\r",g_uiWrnMsgIssue));

    UxState_SetData(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl,STATE_CURITEM,g_uiWrnMsgIssue);
    UxCtrl_SetShow(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl,TRUE);

    /* Start window expiration timer */
    if(g_uiWrnMsgExpTime)
    {
        if(g_uiExpTimerID != NULL_TIMER)
        {
            KeyScan_stopTimer(&g_uiExpTimerID);
        }
        if(g_uiWrnMsgPseudoTimerID != NULL_TIMER)
        {
            KeyScan_stopTimer(&g_uiWrnMsgPseudoTimerID);
        }
        g_uiExpTimerID = KeyScan_startTimer((TIMER_ONE_SEC * g_uiWrnMsgExpTime), NVTEVT_WRNMSG_WND_EXP_TIMER, TIMER_TYPE_ONE_SHOT);
        g_uiWrnMsgPseudoTimerID = KeyScan_startTimer(500, NVTEVT_05SEC_TIMER, TIMER_TYPE_CONTINUE);
    }

    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWrnMsg_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    debug_err(("UIFlowWndWrnMsg: OnClose\r\n"));
    /* Stop window expiration timer if exists */
    if(g_uiExpTimerID != NULL_TIMER)
    {
        KeyScan_stopTimer(&g_uiExpTimerID);
    }
    if(g_uiWrnMsgPseudoTimerID != NULL_TIMER)
    {
        KeyScan_stopTimer(&g_uiWrnMsgPseudoTimerID);
    }

    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWrnMsg_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWrnMsg_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_CHANGE_DSCMODE,1,DSCMODE_CHGTO_NEXT);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWrnMsg_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(pCtrl,1,NVTRET_ENTER_MENU);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWrnMsg_OnKeyPlayback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (Primary_GetCurrentMode()!=PRIMARY_MODE_PLAYBACK)
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_PLAYBACK_MODE,0);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWrnMsg_OnKeyDisplay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiKeyAct;

    uiKeyAct = paramArray[0];

    switch(uiKeyAct)
    {
    case NVTEVT_KEY_PRESS:
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DISPLAY,1,DISPLAY_NORMAL);
    break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWrnMsg_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    NVTEVT event;

    event = paramArray[0];
    debug_ind(("UIFlowWndWrnMsg OnTimer event = 0x%x\r\n", event));

    switch(event)
    {
        case NVTEVT_WRNMSG_WND_EXP_TIMER:
            /**************************************************************************************
            **
            ** Warning Timeout Action
            **
            **************************************************************************************/
            if(g_uiExpTimerID != NULL_TIMER)
            {
                KeyScan_stopTimer(&g_uiExpTimerID);
            }
            if(g_uiWrnMsgPseudoTimerID != NULL_TIMER)
            {
                KeyScan_stopTimer(&g_uiWrnMsgPseudoTimerID);
            }
            Ux_CloseWindow(pCtrl,0);
            if (g_uiWrnMsgIssue == UIFlowWndWrnMsg_StatusTXT_Msg_STRID_BATTERY_LOW)
            {
                KeyScan_PoweroffFlow();
            }
            break;
        case NVTEVT_05SEC_TIMER:
            switch (g_uiWrnMsgIssue)
            {
//            case UIFlowWndWrnMsg_StatusTXT_Msg_STRID_FORMAT_HAS_COMPLETED:
//            case UIFlowWndWrnMsg_StatusTXT_Msg_STRID_SETTING_HAVE_BEEN_RESET:
            case UIFlowWndWrnMsg_StatusTXT_Msg_STRID_BATTERY_LOW:
                /* if (UxCtrl_IsShow(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl) == TRUE)
                 {
                     UxCtrl_SetShow(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl, FALSE);
                 }
                 else
                 {
                     UxCtrl_SetShow(&UIFlowWndWrnMsg_StatusTXT_MsgCtrl, TRUE);
                 }*/ //henry mark 20141225
                break;
            }
            break;

        default:
            debug_err(("UIFlowWndWrnMsg OnTimer: Unknown event 0x%x\r\n", event));
            break;
    }
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndWrnMsg_StatusTXT_MsgCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndWrnMsg_StatusTXT_Msg)
EVENT_END
