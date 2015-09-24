#include "UIFlow.h"
#include "type.h"
#include "Audio.h"

PLB_TASK_DATA g_PlbData = {0};


UINT8 FlowPB_GetPlbDataState(void)
{
    return g_PlbData.State;
}

void FlowPB_PlayVideo(void)
{
    KeyScan_EnableAutoPoweroff(FALSE); //disable auto poweroff,while playing video
    KeyScan_EnableUSBDet(FALSE);       //disable USB detect,while playing video
    g_PlbData.State = PLB_ST_PLAY_MOV;
    g_PlbData.VolCount = AUDIO_VOL_63;  //AUDIO_VOL_63
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,g_PlbData.VolCount);
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVPLAY,1,UIFlowMoviePlayCBFunc);
}

void FlowPB_PauseVideoPlaying(void)
{
    Ux_FlushEvent();
    KeyScan_EnableAutoPoweroff(FALSE); //disable auto poweroff,while playing video
    KeyScan_EnableUSBDet(FALSE);       //disable USB detect,while playing video
    g_PlbData.State = PLB_ST_PAUSE_MOV;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVPAUSE,0);
#if 0
    FlowPB_IconDrawMovPause(&UIFlowWndPlayVideo_StaticICN_PauseCtrl);
    FlowPB_IconDrawMovPlay(&UIFlowWndPlayVideo_StaticICN_PlayCtrl);
    FlowPB_IconDrawMovPlayPause(&UIFlowWndPlayVideo_StaticTXT_PlayPauseTipsCtrl);
#endif
}

void FlowPB_ResumeVideoPlaying(void)
{
    Ux_FlushEvent();
    KeyScan_EnableAutoPoweroff(FALSE); //disable auto poweroff,while playing video
    KeyScan_EnableUSBDet(FALSE);       //disable USB detect,while playing video
    g_PlbData.State = PLB_ST_PLAY_MOV;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVRESUME,0);
}

void FlowPB_StopVideoPlaying(void)
{
    g_PlbData.State = PLB_ST_FULL;
    g_PlbData.VideoPBSpeed=PLB_FWD_MOV_1x;
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVSTOP,0);
    KeyScan_EnableAutoPoweroff(TRUE);
    KeyScan_EnableUSBDet(TRUE);
}