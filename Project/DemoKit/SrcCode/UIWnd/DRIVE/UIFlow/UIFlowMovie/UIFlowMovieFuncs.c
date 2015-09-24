#include "UIFlow.h"
#include "type.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "PrimaryTsk.h"
#include "Adc.h"

MOV_TASK_DATA   gMovData = { 0 };

static UINT32  g_MovRecMaxTime = 0;
static UINT32  g_MovRecCurrTime = 0;
static UINT32  g_MovRecSelfTimerSec = 0;
static UINT32  g_MovRecSelfTimerID = NULL_TIMER;
extern BOOL g_bSpeLockFun;

//#NT#2012/9/25#Philex Lin - begin
// Handling Slow card that cause to do re-recording
// Frame level : 24/20 frame rate
#if (_MOVIE_SLOW_CARD_ == ENABLE)
static UINT32  g_MovRecLevel = MOV_QUALITY_LEVEL_1;
static BOOL    g_MovRecNeedHandleSlowCard = TRUE;

BOOL  FlowMovie_NeedHandleSlowCard(void)
{
    return g_MovRecNeedHandleSlowCard;
}
void  FlowMovie_SlowCardHandling(void)
{

    switch (g_MovRecLevel)
    {
      case MOV_QUALITY_LEVEL_1:
         // Frame rate
         switch(SysGetFlag(FL_MOVIE_SIZE))
         {
            case MOVIE_SIZE_1080FHD:
                 SetMovieFrameRate(20);   //20 frame rate
                break;
            case MOVIE_SIZE_1080P:
            case MOVIE_SIZE_720P:
                 SetMovieFrameRate(24);   //24 frame rate
                break;
         }
         g_MovRecNeedHandleSlowCard = TRUE;
         g_MovRecLevel = MOV_QUALITY_LEVEL_2;
        break;
      case MOV_QUALITY_LEVEL_2:
         // Frame rate
         SetMovieFrameRate(20);   //20 frame rate
         g_MovRecNeedHandleSlowCard = FALSE;
        break;
    }
}
#endif
//#NT#2012/9/25#Philex Lin - end

UINT8 FlowMovie_GetMovDataState(void)
{
    return gMovData.State;
}

void FlowMovie_StartRec(void)
{
    debug_err(("FlowMovie_StartRec... \r\n"));

    if (g_MovRecSelfTimerSec == 0)
    {
        gMovData.State= MOV_ST_REC;
        g_MovRecCurrTime = 0;
        //FlowMovie_IconDrawRec(&UIFlowWndMovie_StaticICN_RecordCtrl);
        //FlowMovie_IconDrawRecTime(&UIFlowWndMovie_StaticTXT_FreeTimeCtrl);
        //Ux_Redraw();
        //UxCtrl_SetShow(&UIFlowWndMovie_StaticTXT_FreeTimeCtrl, FALSE);
        //Ux_Redraw();
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_START_RECAVI,NULL);
        KeyScan_EnableAutoPoweroff(FALSE); //disable auto poweroff,while recording
        KeyScan_EnableUSBDet(FALSE);       //disable USB detect,while recording
    }
    else
    {
        debug_err(("FlowMovie_StartRec: Not yet,g_MovSelfTimerSec=%d\r\n", g_MovRecSelfTimerSec));
    }
}

void FlowMovie_StopRec(void)
{
    debug_err(("FlowMovie_StopRec... \r\n"));
    gMovData.State= MOV_ST_VIEW;
    UxCtrl_SetShow(&UIFlowWndMovie_PanelCtrl,FALSE);
    #if(PRJ == APC3)
    if(g_bSpeLockFun == TRUE)
    {
        g_bSpeLockFun = FALSE;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_STOP_RECAVI,1,1);
    }
    else
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_STOP_RECAVI,NULL);
    #else
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_STOP_RECAVI,NULL);
    #endif

    KeyScan_EnableAutoPoweroff(TRUE);
    KeyScan_EnableUSBDet(TRUE);
    KeyScan_TurnOffLED(KEYSCAN_LED_FCS);
    ide_enable_video(IDE_VIDEOID_1);
}

void FlowMovie_StopRecSelfTimer(void)
{
    if (g_MovRecSelfTimerID != NULL_TIMER)
    {
        KeyScan_stopTimer(&g_MovRecSelfTimerID);
        g_MovRecSelfTimerID = NULL_TIMER;
    }
    g_MovRecSelfTimerSec = 0;
}

UINT8 FlowMovie_GetDataState(void)
{
    return gMovData.State;
}

void FlowMovie_SetRecMaxTime(UINT32 RecMaxTime)
{
    g_MovRecMaxTime = RecMaxTime;
}

UINT32 FlowMovie_GetRecMaxTime(void)
{
    return g_MovRecMaxTime;
}

void FlowMovie_SetRecCurrTime(UINT32 RecCurrTime)
{
    g_MovRecCurrTime = RecCurrTime;
}

UINT32 FlowMovie_GetRecCurrTime(void)
{
    return g_MovRecCurrTime;
}

UINT32 FlowMovie_CheckMemoryForCyclicRec(UINT32 uiCyclicRecTime)
{
    UH      uhDirID = 1, uhFileID = 1;
    UINT32  err;

    debug_err(("uiCyclicRecTime : %d Movie Maxsecond : %d\r\n",uiCyclicRecTime,UIRecMovie_GetData(RECMOVIE_MAXSECOND)));

    while ((uiCyclicRecTime) > UIRecMovie_GetData(RECMOVIE_MAXSECOND))//5*
    {
        // always get oldest (1st) DCF file
        FilesysWaitInitFinish(FST_TIME_INFINITE);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        err = FilesysGetDCFIDBySeq(1, &uhDirID, &uhFileID);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);

        // check if there is no DCF file then stop
        if (uhDirID == 0 && uhFileID == 0)
        {
            debug_err(("[Cyclic Rec]: No DCF file\r\n"));
            break;
        }

        // check if file is protected; if yes, unlock it
        FilesysSetFileLock(uhFileID, uhDirID, FALSE);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);

        // erase file
        debug_err(("[Cyclic Rec]: Erase file: DirID = %d, FileID = %d\r\n", uhDirID, uhFileID));
        err = FilesysDeleteFile(uhDirID, uhFileID, FST_FMT_ANYFORMAT);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
    }

    //if (uiCyclicRecTime > 2*UIRecMovie_GetData(RECMOVIE_MAXSECOND)) // still no enough space for recording one period
    //{
    //    return FALSE;
    //}

    return TRUE;
}

BOOL FlowMovie_IsStorageErr(void)
    {
        BOOL    retV = FALSE;
    
        if(UI_Validate_Storage(STORAGE_CHECK_ERROR, NULL) == FALSE)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_ERR,FLOWWRNMSG_TIMER_KEEP);
            retV = TRUE;
            return retV;
        }
        if(UI_Validate_Storage(STORAGE_CHECK_FULL, &(g_MovRecMaxTime)) == FALSE)
        {
            if (GetCardStatus() == CARD_REMOVED)
            {
                Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD,FLOWWRNMSG_TIMER_2SEC);
            }
            else
            {
            if (Get_MovieCyclicRecTime() != 0)
                {
                        debug_err(("^R#Card Full Get_MovieCyclicRecTime : %d\r\n",Get_MovieCyclicRecTime()));                   
                     FlowMovie_CheckMemoryForCyclicRec(Get_MovieCyclicRecTime());
                return FALSE;
             }  
                   Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
            }
    
            retV = TRUE;
            return retV;
        }
        if(UI_Validate_Storage(STORAGE_CHECK_LOCKED, NULL) == FALSE)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_LOCKED,FLOWWRNMSG_TIMER_2SEC);
            retV = TRUE;
            return retV;
        }
        //#NT#2012/5/30#Philex Lin-begin
        // add check valided cluster class4/size(32KB)/class6 sd card
#if (_SD_CLASS4_REC_ == DISABLE)    // allow recording with Class4 sd card
        if (UI_Validate_Storage(STORAGE_CHECK_SDCLASS4, NULL) == FALSE)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_SD_CLASS4,FLOWWRNMSG_TIMER_2SEC);
            retV = TRUE;
            return retV;
        }
#endif    
        if (UI_Validate_Storage(STORAGE_CHECK_CLUSTER, NULL) == FALSE)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CLUSTER_WRONG,FLOWWRNMSG_TIMER_2SEC);
            retV = TRUE;
            return retV;
        }
        /*if (UI_Validate_Storage(STORAGE_CHECK_SDCLASS6, NULL) == FALSE)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_SD_CLASS6,FLOWWRNMSG_TIMER_2SEC);
            retV = TRUE;
            return retV;
        }*/ //henry mark 20141225
        //#NT#2012/5/30#Philex Lin-end
    
        retV = FALSE;
        return retV;
    }


UINT32 FlowMovie_GetSelfTimerID(void)
{
    return g_MovRecSelfTimerID;
}

BOOL FlowMovie_ChkDrawStoreFullFolderFull(void)
{
    if(UI_Validate_Storage(STORAGE_CHECK_FULL, &(g_MovRecMaxTime)) == FALSE)
    {
        g_MovRecMaxTime = UIRecMovie_GetData(RECMOVIE_MAXSECOND);
        if (GetCardStatus() == CARD_REMOVED)
        {
            DbgMsg_UIRecAvi(("@Intrnal Memory Full\r\n"));
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_FULL,FLOWWRNMSG_TIMER_2SEC);
            return TRUE;
        }
        else
        {
            DbgMsg_UIRecAvi(("@Card Full\r\n"));
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
            return TRUE;
        }
    }

    return FALSE;
}

void FlowMovie_GetPictureInMovie(void)
{
#if (_MOVIE_PIM_MODE_ == ENABLE)
    UINT32  sec, unsafe;

    switch (gMovData.State)
    {
    case MOV_ST_REC:
        sec = MovRec_Disk2Second();
        unsafe = MovRec_CheckUnsavedFileBlocks();

        if ((sec > 3)&&(unsafe == 0))
        {
            RawEnc_SetCaptureOne();
            //UxCtrl_SetShow(&UIFlowWndMovie_Static_pimCtrl, TRUE);
        }
        else
        {
            if (unsafe)
            {
                debug_err(("unsafe to take picture !\r\n"));
            }
            else
            {
                debug_err(("no space to encode RAW !\r\n"));
            }
        }
        break;
    }
#endif
}

void FlowMovie_OnTimer1SecIndex(void)
{
    if(Primary_CheckIdle(PRI_TIME_NO_WAIT) != PRI_STA_IDLE)
    {
        return; //return would not update bLastUSBDet ,next time would detect again
    }

    switch (gMovData.State)
    {
    case MOV_ST_VIEW:
    case MOV_ST_VIEW | MOV_ST_ZOOM:
    case MOV_ST_REC:
    case MOV_ST_REC | MOV_ST_ZOOM:
        gMovData.SysTimeCount++;
        if (UxCtrl_IsShow(&UIFlowWndMovie_YMD_StaticCtrl))
        {
            FlowMovie_IconDrawDateTime();
        }
        break;
    }
}


