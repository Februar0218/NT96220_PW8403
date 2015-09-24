#include "UIFlow.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "AppinitPlayback.h"
#include "KeyScanTsk.h"
#include "PrimaryTsk.h"

//#NT#2009/9/4#Philex - end
//-----------------------------------------------------------------------------
// Photo Mode Main Flow Window
//-----------------------------------------------------------------------------
ER FlowPhoto_Open(void)
{
    gPhotoData.State = PHOTO_ST_VIEW;
    Ux_SetSysObj(&UIPhotoObjCtrl);
    Ux_SetSysDefCmd(UISystemObjCtrlCmdMap);
    Ux_OpenWindow((VControl *)(&UIFlowWndPhotoCtrl), 0);
    debug_err(("FlowPhoto_Open()\r\n"));
    return E_OK;
}

ER FlowPhoto_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndPhotoCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Movie Mode Main Flow Window
//-----------------------------------------------------------------------------
ER FlowMovie_Open(void)
{
    gMovData.State = MOV_ST_VIEW;
    Ux_SetSysObj(&UIMovieObjCtrl);
    Ux_SetSysDefCmd(UISystemObjCtrlCmdMap);
    Ux_OpenWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
    debug_ind(("FlowMovie_Open()\r\n"));
    return E_OK;
}

ER FlowMovie_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndMovieCtrl), 0);

    return E_OK;
}

//-----------------------------------------------------------------------------
// Playback Mode Main Flow Window
//-----------------------------------------------------------------------------
ER FlowPB_Open(void)
{
    //init state in playback mode
    g_PlbData.State = PLB_ST_FULL;

    Ux_SetSysObj(&UIPlayObjCtrl);
    Ux_SetSysDefCmd(UISystemObjCtrlCmdMap);
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYINIT,0);

    Ux_OpenWindow((VControl *)(&UIFlowWndPlayCtrl), 0);
    debug_ind(("FlowPB_Open()\r\n"));
    return E_OK;
}
ER FlowPB_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndPlayCtrl), 0);

    return E_OK;
}

//-----------------------------------------------------------------------------
// USB Menu Window
//-----------------------------------------------------------------------------
ER MenuUSB_Open(void)
{
    #if (UI_STYLE == UI_STYLE_DRIVE)
    Ux_OpenWindow((VControl *)(&UIMenuWndUSBCtrl), 0);
    debug_ind(("MenuUSB_Open()\r\n"));
    #endif

    return E_OK;
}

ER MenuUSB_Close(void)
{
    #if (UI_STYLE == UI_STYLE_DRIVE)
    Ux_CloseWindowClear((VControl *)(&UIMenuWndUSBCtrl), 0);
    #endif
    return E_OK;
}

//-----------------------------------------------------------------------------
// Battery Empty Notice Window
//-----------------------------------------------------------------------------
ER FlowWrnBatteryEmpty_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIFlowWndWrnMsgCtrl), 2, FLOWWRNMSG_ISSUE_BATTERY_LOW, FLOWWRNMSG_TIMER_KEEP);
    debug_ind(("FlowWrnBatteryEmpty_Open()\r\n"));
   return E_OK;
}

ER FlowWrnBatteryEmpty_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndWrnMsgCtrl), 0);
   return E_OK;
}

//-----------------------------------------------------------------------------
// Storage Card Hot Plug/Unplug Notice Window
//-----------------------------------------------------------------------------
ER FlowWrnStrgHotPlug_Open(void)
{
    #if (UI_STYLE == UI_STYLE_DRIVE)
    Ux_OpenWindow((VControl *)(&UIFlowWndWrnStrgHotPlugCtrl), 0);
    debug_ind(("FlowWrnStrgHotPlug_Open()\r\n"));
    #endif

    return E_OK;
}

ER FlowWrnStrgHotPlug_Close(void)
{
    #if (UI_STYLE == UI_STYLE_DRIVE)
    Ux_CloseWindowClear((VControl *)(&UIFlowWndWrnStrgHotPlugCtrl), 0);
    #endif

    return E_OK;
}

//-----------------------------------------------------------------------------
// No File window open/close
//-----------------------------------------------------------------------------
ER FlowNoFile_Open(void)
{
   Ux_OpenWindow((VControl *)(&UIFlowWndPlayNoFileCtrl), 0);
   return E_OK;
}

ER FlowNoFile_Close(void)
{
   Ux_CloseWindow((VControl *)(&UIFlowWndPlayNoFileCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// loading notice Window
//-----------------------------------------------------------------------------
ER FlowWrnLoading_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1,UIFlowWndUSB_LOADING_MODE);
    return E_OK;
}

ER FlowWrnLoading_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndUSBCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// uploading Window
//-----------------------------------------------------------------------------
ER FlowWrnUploading_Open(void)
{
    return E_OK;
}

ER FlowWrnUploading_Close(void)
{
    return E_OK;
}
//-----------------------------------------------------------------------------
// social network window in playback mode
//-----------------------------------------------------------------------------
ER FlowPB_SocialNetwork_Open(void)
{
    return E_OK;
}

ER FlowPB_SocialNetwork_Close(void)
{
    return E_OK;
}

//-----------------------------------------------------------------------------
ER MenuMSDC_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1,UIFlowWndUSB_MSDC_MODE);
    debug_ind(("MenuMSDC_Open()\r\n"));
    return E_OK;
}

ER MenuMSDC_Close(void)
{

    if(KeyScan_IsUSBPlug() ==TRUE)
    {
         ide_disable_video(IDE_VIDEOID_1);
         ide_disable_video(IDE_VIDEOID_2);
    }
    Ux_CloseWindow((VControl *)(&UIFlowWndUSBCtrl), 0);
    return E_OK;
}
// PCC Menu Window
//-----------------------------------------------------------------------------
ER MenuPCC_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1,UIFlowWndUSB_PCC_MODE);
    debug_ind(("MenuPCC_Open()\r\n"));
    return E_OK;
}

ER MenuPCC_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndUSBCtrl), 0);
    return E_OK;
}
// Printer Menu Window
//-----------------------------------------------------------------------------
ER MenuPrint_Open(void)
{
    Ux_SetSysObj(&UIPrintObjCtrl);
    Ux_OpenWindow((VControl *)(&UIFlowWndPrintCtrl), 0);
    debug_ind(("MenuPrint_Open()\r\n"));
    return E_OK;
}

ER MenuPrint_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndPrintCtrl), 0);
//    Ux_CloseWindowClear(&UIFlowWndPlayPictBCtrl,1,2);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Setup Menu Window
//-----------------------------------------------------------------------------
ER MenuSetup_Open(void)
{
    return E_OK;
}

ER MenuSetup_Close(void)
{
    return E_OK;
}
//#PIC#2010/04/27#Creator -begin
//-----------------------------------------------------------------------------
// Init setup Mode Main Flow Window
//-----------------------------------------------------------------------------

ER FlowInitSetup_Open(void)
{
    return E_OK;
}

ER FlowInitSetup_Close(void)
{
    return E_OK;
}

//-----------------------------------------------------------------------------
// TV Mode Main Flow Window
//-----------------------------------------------------------------------------

ER FlowTvMode_Open(void)
{
    debug_err(("FlowTvMode_Open()\r\n"));
    return E_OK;
}

ER FlowTvMode_Close(void)
{
    debug_err(("FlowTvMode_Close()\r\n"));
    return E_OK;
}

//-----------------------------------------------------------------------------
// Calibration Menu Window
//-----------------------------------------------------------------------------
#if (_CALIBRATION_MODE_ == ENABLE)

ER MenuCalibration_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIMenuWndCalibrationCtrl), 0);
    //Ux_Redraw();
    debug_ind(("MenuCalibration_Open()\r\n"));
    return E_OK;
}

ER MenuCalibration_Close(void)
{
    if (UxCtrl_IsShow(&UIMenuWndCalibrationCtrl) == TRUE)
    {
        //set_flg(FLG_ID_KEY, FLGKEY_ENTER);
        Ux_CloseWindowClear((VControl *)(&UIMenuWndCalibrationCtrl), 0);
    }
    return E_OK;
}

#endif
