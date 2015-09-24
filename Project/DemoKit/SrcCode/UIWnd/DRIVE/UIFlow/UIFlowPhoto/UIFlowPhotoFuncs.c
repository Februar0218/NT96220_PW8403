#include <stdio.h>
#include "UIFramework.h"
#include "ImageAlgInfor.h"
#include "FileSysTsk.h"
#include "PrimaryTsk.h"
#include "UIFlow.h"
#include "SoundData.h"
#include "PlaySoundTsk.h"
#include "PhotoTsk.h"

PHOTO_TASK_DATA gPhotoData = { 0 };

static UINT32 g_uiFreePicNum        = 0;
static CHAR    g_cFreePicNumStr[8]     = {0};
static UINT16  g_SelftimerTime = 0;

UINT32 Get_FreePicNum(UINT32 uiSizeIndex, UINT32 uiQualityIndex)
{
    UINT32              fileNum;
    UINT64              uiFreeSpace;
    UINT32              uiCompressRate;
    UINT32              freeImgNum;
    UINT32              reserveSize = 0x100000;
    UINT32              MinCaptureSize;
    _IMGSize_sel        size;
    UINT8               CompressRateTable[QUALITY_ID_MAX]
                        ={
                           23,  // quality fine,        25%
                           14,  // quality standard,    15%
                           9    // quality economy,     10%
                         };

    if (GetCardStatus() == CARD_REMOVED)
    {
        if(FilesysGetFreeSpace() < (reserveSize/2))
        {
            uiFreeSpace = 0;
        }
        else
    {
            uiFreeSpace = FilesysGetFreeSpace();
        }
    }
    else
    {
        if(FilesysGetFreeSpace() < reserveSize)
        {
            uiFreeSpace = 0;
        }
        else
        {
            uiFreeSpace = FilesysGetFreeSpace() - reserveSize;
        }
    }

    debug_ind(("Free Space = %ld\r\n", uiFreeSpace));

    uiCompressRate = CompressRateTable[uiQualityIndex];

    size = Get_SizeValue();
    MinCaptureSize = (JPGIMG_SIZE[size][0] * JPGIMG_SIZE[size][1] * 3) * uiCompressRate / 200;
    debug_ind(("MinCaptureSize = %ld K \r\n", MinCaptureSize/1024));

    // Reserve double size for last file.
    if(uiFreeSpace < MinCaptureSize * 2)
    {
    if (GetCardStatus() == CARD_REMOVED)
    {
            FilesysGetDCFFileTypeNum(FST_FMT_JPG,&fileNum);

            if(fileNum == 0)
            {
                freeImgNum = 1;
    }
    else
    {
                freeImgNum = 0;
            }

            if(uiFreeSpace < (reserveSize/2))
            {
                freeImgNum = 0;
            }
        }
        else
        {
            freeImgNum = 0;
        }
    }
    else    // uiFreeSpace = (N-1)*MinCaptureSize + MinCaptureSize*2
    {
        if (GetCardStatus() == CARD_REMOVED)
        {
            FilesysGetDCFFileTypeNum(FST_FMT_JPG,&fileNum);

            if(fileNum == 0)
        {
                freeImgNum = 1;
        }
            else
        {
                freeImgNum = 0;
        }
        }
        else
        {
            freeImgNum = (uiFreeSpace - MinCaptureSize * 2) / MinCaptureSize + 1;
        }
    }

    debug_err((">> Free ImgNum = %ld\r\n", freeImgNum));

    return freeImgNum;
 }

CHAR *Get_FreePicNumString(UINT32 uiValue)
{
    sprintf(g_cFreePicNumStr, "%05ld", uiValue);
    return g_cFreePicNumStr;
}

void DCF_SetFileId(void)
{
    UINT16 uhFolderId, uhFileId;

    FilesysGetDCFNextID(&uhFolderId, &uhFileId);

    if (uhFolderId > SysGetFlag(FL_DCF_DIR_ID))
        SysSetFlag(FL_DCF_DIR_ID, uhFolderId);

    if (uhFileId > SysGetFlag(FL_DCF_FILE_ID))
        SysSetFlag(FL_DCF_FILE_ID, uhFileId);

    debug_ind(("DCF_SetFileId: Set new DirNumber = %d, FileNumber = %d\r\n", SysGetFlag(FL_DCF_DIR_ID), SysGetFlag(FL_DCF_FILE_ID)));

    FilesysSetWriteNextFileId(((UINT16)SysGetFlag(FL_DCF_DIR_ID)), ((UINT16)SysGetFlag(FL_DCF_FILE_ID)), FALSE);
}

void FlowPhoto_DoCapture(void)
{
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_CAPTURE,0);
}

void FlowPhoto_SetFdSdProc(void)
{
    switch (SysGetFlag(FL_FD))
    {
    case FD_OFF:
        Ux_SendEvent(&UIPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_OFF);
        break;
    case FD_ON:
        Ux_SendEvent(&UIPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_ON);
        break;
    case FD_SMILE:
        Ux_SendEvent(&UIPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_SMILE);
        break;
    }
}

void FlowPhoto_ForceLockFdSd(void)
{
    Ux_SendEvent(&UIPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_OFF);
}

UINT32 FlowPhoto_GetFreePicNum(void)
{
    return g_uiFreePicNum;
}

BOOL FlowPhoto_IsStorageErr(void)
{
    BOOL    retV = FALSE;

    if(UI_Validate_Storage(STORAGE_CHECK_ERROR, NULL) == FALSE)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_ERR,FLOWWRNMSG_TIMER_KEEP);
        return TRUE;
    }
    if(UI_Validate_Storage(STORAGE_CHECK_FULL, &(g_uiFreePicNum)) == FALSE)
    {
        if (GetCardStatus() == CARD_REMOVED)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD,FLOWWRNMSG_TIMER_2SEC);
        }
        else
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
        }
        return TRUE;
    }
    if(UI_Validate_Storage(STORAGE_CHECK_LOCKED, NULL) == FALSE)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_LOCKED,FLOWWRNMSG_TIMER_2SEC);
        return TRUE;
    }

    return retV;
}

void FlowPhoto_OnTimer01SecIndex(void)
{

    //if(Primary_CheckIdle(PRI_TIME_NO_WAIT) != PRI_STA_IDLE)
    //{
    //    return; //return would not update bLastUSBDet ,next time would detect again
    //}
#if 0
    switch (gPhotoData.State)
    {
    case PHOTO_ST_SELFTIMER:

        if ((gPhotoData.SelfTimerCount % 10 == 0) && (gPhotoData.SelfTimerCount != 0))
        {
            FlowPhoto_IconDrawSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl, gPhotoData.SelfTimerCount/10);
            PlaySound_Play(PLAYSOUND_SOUND_KEY_OTHERS);
        }

        if (gPhotoData.SelfTimerCount == 0)
        {
            FlowPhoto_IconHideSelftimer(&UIFlowWndPhoto_StatusICN_SelftimerCtrl);
            FlowPhoto_IconHideSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl);
            Ux_Redraw();
            gPhotoData.State = PHOTO_ST_CAPTURE;
            FlowPhoto_DoCapture();
        }
        break;
    }
#endif
    if ((gPhotoData.SelfTimerCount % 10 == 0) && (gPhotoData.SelfTimerCount != 0))
        {
            FlowPhoto_IconDrawSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl, gPhotoData.SelfTimerCount/10);
            PlaySound_Play(PLAYSOUND_SOUND_KEY_OTHERS);
        }

        if (gPhotoData.SelfTimerCount == 0)
        {
            FlowPhoto_IconHideSelftimer(&UIFlowWndPhoto_StatusICN_SelftimerCtrl);
            FlowPhoto_IconHideSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl);
            Ux_Redraw();
            gPhotoData.State = PHOTO_ST_CAPTURE;
            FlowPhoto_DoCapture();
        }

}

void FlowPhoto_OnTimer1SecIndex(void)
{
    //if(Primary_CheckIdle(PRI_TIME_NO_WAIT) != PRI_STA_IDLE)
    //{
    //    return; //return would not update bLastUSBDet ,next time would detect again
    //}

    switch (gPhotoData.State)
    {
    case PHOTO_ST_VIEW:
    case PHOTO_ST_VIEW | PHOTO_ST_ZOOM:
    case PHOTO_ST_SELFTIMER:
        gPhotoData.SysTimeCount++;
        break;
    }
}
