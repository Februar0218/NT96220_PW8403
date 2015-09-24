#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "AviRecApi.h"
#include "UIFlow.h"
#include "MediaRecAPI.h"
#include "KeyScanTsk.h"
#include "GenDateImprintData.h"
#include "RawEncAPI.h"
#include "MovieInterface.h"

UINT32 g_uiRecSecs = 0;
extern BOOL gbMoviFileLock;

void FlowMovie_RecCB(UINT32 uiEventID, UINT32 uiSeconds)
{

    switch (uiEventID)
    {
    case MEDIAREC_EVENT_RESULT_OVERTIME:
        Ux_PostEvent(NVTEVT_CB_MOVIE_FULL, 0);
        debug_ind(("FlowMovie_RecCB() result overtime: %d\r\n", uiEventID));
        break;

    case MEDIAREC_EVENT_RESULT_FULL:
        Ux_PostEvent(NVTEVT_CB_MOVIE_FULL, 0);
        debug_ind(("FlowMovie_RecCB() result full: %d\r\n", uiEventID));
        break;

    case MEDIAREC_EVENT_RESULT_HW_ERR:
    case MEDIAREC_EVENT_RESULT_NOTREADY:
    case MEDIAREC_EVENT_RESULT_WRITE_ERR:
        Ux_PostEvent(NVTEVT_CB_MOVIE_WR_ERROR, 0);
        debug_ind(("FlowMovie_RecCB() result error: %d\r\n", uiEventID));
        break;

    case MEDIAREC_EVENT_THRESHOLD:
        debug_ind(("FlowMovie_RecCB() uiSeconds = %d\r\n", uiSeconds));
        g_uiRecSecs = uiSeconds;
        Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_SEC, 1,  uiSeconds);
        break;

    case MEDIAREC_EVENT_RESULT_NORMAL:
        debug_ind(("FlowMovie_RecCB() finish normal\r\n"));
        #if (_GPS_RECEIVER_LOG_ == ENABLE)
        GPSRec_LogGPSData();
        #endif		
        Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 0);
        break;

    case MEDIAREC_EVENT_CUTFILE_END:
        debug_ind(("^R%s,MEDIAREC_EVENT_CUTFILE_END\r\n",__func__));
		if (gbMoviFileLock)
		{
			gbMoviFileLock = FALSE;
			FilesysWaitInitFinish(FST_TIME_INFINITE);	 
			FilesysWaitCmdFinish(FST_TIME_INFINITE);           
			FilesysSetFileLock(SysGetFlag(FL_DCF_FILE_ID),SysGetFlag(FL_DCF_DIR_ID),  TRUE);
			FilesysWaitInitFinish(FST_TIME_INFINITE);	 
			FilesysWaitCmdFinish(FST_TIME_INFINITE);  
			UxCtrl_SetShow(&UIFlowWndMovie_Status_LOCKCtrl, FALSE);	
		}
        /* update DCF filename while capturing one photo and sync with RTC date/time */
        Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_UPDATE_DCF, 1, FALSE);
        #if (_GPS_RECEIVER_LOG_ == ENABLE)
        GPSRec_LogGPSData();
        #endif		
        break;

//#NT#2011/2/9#Philex - being
// get cyclic recording deleted file's DID/FID,
// and delete the corresponding gps log file
    case MEDIAREC_EVENT_DELETE_DID:
        break;

    case MEDIAREC_EVENT_DELETE_FID:
        break;
//#NT#2011/2/9#Philex - end

    case MEDIAREC_EVENT_RESULT_SLOW:
        //#NT#2011/5/25#Philex - begin
        // recording fisished because card write speed is to slow
        // re-start recording again if this condition comes true
        debug_err(("^MEDIAREC_EVENT_RESULT_SLOW\r\n"));
        // decide card is slow card if recording time is less than 100 sec
        //if(g_uiRecSecs <=100)
            //Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 4, NVTEVT_KEY_PRESS, FLGKEY_PRESSED,UIFlowWndMovie_Restart_Rec,UIFlowWndMovie_Restart_SlowCard);
        Ux_PostEvent(NVTEVT_KEY_CUSTOM2, 1,NVTEVT_KEY_PRESS);    
	  //TimerDelayMs(300);
        //Ux_PostEvent(NVTEVT_KEY_ENTER, 1,NVTEVT_KEY_PRESS);       
		
        //#NT#2011/5/25#Philex - end
        break;
    case MEDIAREC_EVENT_RESULT_CUT_FAIL:
        //#NT#2011/5/25#Philex - begin
        // cut, overlap fail (write slowly)
        debug_err(("^RMEDIAREC_EVENT_RESULT_CUT_FAIL\r\n"));
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 3, NVTEVT_KEY_PRESS, FLGKEY_PRESSED,UIFlowWndMovie_Restart_Rec);
        //#NT#2011/5/25#Philex - end
        break;

    case MEDIAREC_EVENT_ONEFRAME:
        GenMovieDID_ReFresh();
        break;

    case MEDIAREC_EVENT_START_VID://2011/08/02 Meg Lin
        GenMovieDID_ResetShowingTime();
        break;

    default:
        break;

    }
}

void FlowMovie_RawEncCB(UINT32 uiEventID)
{
#if (_MOVIE_PIM_MODE_ == ENABLE)
    debug_ind(("%s, ID %d\r\n", __func__, uiEventID));

    switch (uiEventID)
    {
    case RAWENC_EVENT_RESULT_OK:
        debug_ind(("RawEnc write file OK\r\n"));
        // Inform media recording task about picture file size
        MovRec_MinusFilesize(RawEnc_GetRawFilesize());
        break;
    }

    // Inform UI for picture in movie is done
    Ux_PostEvent(NVTEVT_CB_RAWENC_FINISH, 0);
#endif
}
