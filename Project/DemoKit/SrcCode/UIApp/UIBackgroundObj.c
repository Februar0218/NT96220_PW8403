#include <stdio.h>
#include <string.h>
#include "PlaybackTsk.h"
#include "UIFlow.h"
#include "NVTUserCommand.h"
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "PrimaryTsk.h"
#include "FileSysTsk.h"
#include "Utility.h"

#define NVT_PTR_RESPOSE_TIME            15  //15 seconds
#define NVT_PC_RESPOSE_TIME             10  //10 seconds

static UINT32 BackgroundDelAll(void);
static UINT32 BackgroundCopy2Card(void);
static UINT32 BackgroundFormat(void);
static UINT32 BackgroundFormatCard(void);
static UINT32 BackgroundFormatNand(void);
static UINT32 BackgroundProtectAll(void);
static UINT32 BackgroundUnProtectAll(void);
static UINT32 BackgroundWaitPrintInit(void);
static UINT32 BackgroundBurnInTest(void);

BACKGROUND_JOB_ENTRY gBackgroundExtFuncTable[] =
{
    {NVTEVT_BKW_DELALL,             BackgroundDelAll},
    {NVTEVT_BKW_COPY2CARD,          BackgroundCopy2Card},
    {NVTEVT_BKW_FORMAT,             BackgroundFormat},
    {NVTEVT_BKW_FORMATCARD,         BackgroundFormatCard},
    {NVTEVT_BKW_FORMATNAND,         BackgroundFormatNand},
    {NVTEVT_BKW_PROTECTALL,         BackgroundProtectAll},
    {NVTEVT_BKW_UNPROTECTALL,       BackgroundUnProtectAll},
    {NVTEVT_BKW_WAIT_PTR_INIT,      BackgroundWaitPrintInit},
    {NVTEVT_BKW_BURNIN_TEST,        BackgroundBurnInTest},
    {0,                             0},
};

static UINT32 BackgroundDelAll(void)
{
    INT32 ret;

    PB_PlayDeleteMode(PB_DELETE_ALL);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
}

static UINT32 BackgroundCopy2Card(void)
{
    INT32  ret;
    UINT32 uiDirId, uiFileId, uiFileType, uiCopyFileNum;
    char   *pBuf;
    //PFILE_TSK_DCF_PARAM pFileParam;

    //Save gResrInfo of file system
    uiDirId = PB_GetCurrFileSysInfo(PLAYFILEINFO_DIRID);
    uiFileId = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILEID);
    uiFileType = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILEFORMAT);
    FilesysSaveReservedInfo(uiDirId, uiFileId, uiFileType);

    #if 0
    //Change to NAND and get total file number
    Primary_ChangeStorage(PRIMARY_STGTYPE_NAND);
    Primary_Wait4ChangeStorageDone();
//#NT#2011/05/31#Scottie -begin
//#NT#CANNOT call the internal API of PB, use FileSystem API to replace it
//    uiTotalFileNum = PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILENUMS);
    pFileParam = FilesysGetFSTParam();
    uiTotalFileNum = pFileParam->TolFileNum;
//#NT#2011/05/31#Scottie -end
    #endif

    //Do copy
    get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, pBuf);

    ret = FilesysCopyTo(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND),
                        Primary_GetStorageObj(PRIMARY_STGTYPE_CARD),
                        pBuf,
                        POOL_SIZE_CAPTURE - POOL_SIZE_RAM_DISK,
                        FST_COPYTO_APPEND,
                        &uiCopyFileNum,
                        NULL,
                        FALSE);


    #if 0
    debug_err(("###chris: total file num=%d, copy file num=%d\r\n", uiTotalFileNum, uiCopyFileNum));

    //Change back to Card
    Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);
    Primary_Wait4ChangeStorageDone();
    #endif

    //Restore gResrInfo of file system
    FilesysRestoreReservedInfo();

    #if 0
    if(uiTotalFileNum == uiCopyFileNum)
        return UIBKW_STA_OK;
    else
        return UIBKW_STA_CARDFULL;
    #else
    return UIBKW_STA_OK; // always return OK temporary!!!
#endif
}

static UINT32 BackgroundFormat(void)
{
    int ret;
    PSTRG_TAB   pWhichObj;

    FilesysGetStrgObj(&pWhichObj);
    ret = FilesysFormat(pWhichObj, FALSE);

    #if (_MP3_MODE_ == ENABLE)
    Set_MP3FtbRenewIndex(MP3FTB_RENEW_ON);
    #endif

    return (UINT32)ret;
}

static UINT32 BackgroundFormatCard(void)
{
    int ret;

    if (Primary_GetCurrentMode() == PRIMARY_MODE_PLAYBACK)
    {
        PB_SetFormatStorage(PLAY_FORMAT_CARD);
        ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    else
    {
        ret = FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_CARD), FALSE);
    }

    if (Primary_GetStorage() == PRIMARY_STGTYPE_CARD)
    {
        SysSetFlag(FL_DCF_DIR_ID, MIN_DCF_DIR_NUM);
        SysSetFlag(FL_DCF_FILE_ID, MIN_DCF_FILE_NUM);
        FilesysSetWriteNextFileId(MIN_DCF_DIR_NUM,MIN_DCF_FILE_NUM,FALSE);
    }

    #if (_MP3_MODE_ == ENABLE)
    Set_MP3FtbRenewIndex(MP3FTB_RENEW_ON);
    #endif

    return (UINT32)ret;
}

static UINT32 BackgroundFormatNand(void)
{
    int ret;

    if (Primary_GetCurrentMode() == PRIMARY_MODE_PLAYBACK)
    {
        PB_SetFormatStorage(PLAY_FORMAT_NAND);
        ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    else
    {
        ret = FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), FALSE);
    }

    if (Primary_GetStorage() == PRIMARY_STGTYPE_NAND)
    {
        SysSetFlag(FL_DCF_DIR_ID, MIN_DCF_DIR_NUM);
        SysSetFlag(FL_DCF_FILE_ID, MIN_DCF_FILE_NUM);
        FilesysSetWriteNextFileId(MIN_DCF_DIR_NUM,MIN_DCF_FILE_NUM,FALSE);
    }

    #if (_MP3_MODE_ == ENABLE)
    Set_MP3FtbRenewIndex(MP3FTB_RENEW_ON);
    #endif

    return (UINT32)ret;
}

static UINT32 BackgroundProtectAll(void)
{
    INT32 ret;

    PB_SetProtectFile(PLAY_PROTECT_ALL);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
}

static UINT32 BackgroundUnProtectAll(void)
{
    INT32 ret;

    PB_SetProtectFile(PLAY_UNPROTECT_ALL);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
}

static UINT32 BackgroundWaitPrintInit(void)
{
#if 0   //#NewUx porting: Temporarily marked for compile error
    UINT    uiUSBFlag;
    UINT8 notFirst = 1;
    UINT32 notResponseTime =0;

    debug_msg("BackgroundWaitPrintInit\r\n ");
    clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL);
    clr_flg(FLG_DPS, FLGDPS_1SECOND|FLGDPS_FIRSTPTPCMD);

    while(notFirst)
    {
        wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_FIRSTPTPCMD|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);

        if ((uiUSBFlag & FLGDPS_FIRSTPTPCMD))
        {
            notFirst = 0;
        }
        else
        {
            notResponseTime++;
            if (notResponseTime > NVT_PTR_RESPOSE_TIME )
            {
                debug_err(("Wait FLGDPS_FIRSTPTPCMD failed!!!, notResponseTime > %d sec\r\n", NVT_PTR_RESPOSE_TIME));
                return FLGDPS_INITFAIL;
            }
        }
    }

    wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);
    notResponseTime = 0;

    while((notResponseTime < NVT_PC_RESPOSE_TIME) && (uiUSBFlag & FLGDPS_1SECOND))
    {
        wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);
        notResponseTime ++;
    }

    if (notResponseTime == NVT_PC_RESPOSE_TIME)
    {
        debug_err(("Wait init failed!!!, notResponseTime > %d sec\r\n", NVT_PC_RESPOSE_TIME));
        return FLGDPS_INITFAIL;
    }

    if (uiUSBFlag & FLGDPS_INITFAIL)
    {
        notResponseTime= 0;
        clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_1SECOND);

        while (notResponseTime < NVT_PTR_RESPOSE_TIME)
        {
            wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_INITOK|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);
            debug_err(("Init Fail, wait flag again:uiUSBFlag=%x, notResponseTime=%x\r\n", uiUSBFlag, notResponseTime));
            if (uiUSBFlag & FLGDPS_INITOK)
            {
                break;
            }
            else
            {
                notResponseTime++;
            }
        }

        if (notResponseTime == NVT_PTR_RESPOSE_TIME)
        {
            return FLGDPS_INITFAIL;
        }
    }

    if (uiUSBFlag & FLGDPS_INITOK)
    {
        debug_msg("Wait PrintInit ok !!!\r\n ");
        return FLGDPS_INITOK;

    }
    else
    {
        return FLGDPS_INITFAIL;
    }
#else
    return 0;
#endif
}

static UINT32 BackgroundBurnInTest(void)
{
    //-------------------------------------------------------------------------
    // Purpose: automatically capturing photos and recording movies
    //          for each image resolution
    //-------------------------------------------------------------------------

    UINT32 i, uiTimeCount1;
    UINT32 uiMovieSize[MOVIE_SIZE_ID_MAX] = {MOVIE_SIZE_1080FHD,MOVIE_SIZE_720P, MOVIE_SIZE_WVGA,MOVIE_SIZE_VGA};
#if (UI_STYLE == UI_STYLE_DRIVE)
    UINT32 uiPhotoSize[PHOTO_SIZE_ID_MAX] = {\
        PHOTO_SIZE_12M, PHOTO_SIZE_8M, PHOTO_SIZE_5M, PHOTO_SIZE_3M };
#else
#if (UPDATE_CFG == UPDATE_CFG_YES)
    UINT32 uiPhotoSize[PHOTO_SIZE_ID_MAX] = {\
        PHOTO_SIZE_15M, PHOTO_SIZE_14M, PHOTO_SIZE_12M, PHOTO_SIZE_10M, PHOTO_SIZE_8M, PHOTO_SIZE_7M, \
        PHOTO_SIZE_7MHD, PHOTO_SIZE_5M, PHOTO_SIZE_4M, PHOTO_SIZE_3M, PHOTO_SIZE_2M, PHOTO_SIZE_2MHD, \
        PHOTO_SIZE_VGA};
#else
    UINT32 uiPhotoSize[PHOTO_SIZE_ID_MAX] = {\
        PHOTO_SIZE_12M, PHOTO_SIZE_10M, PHOTO_SIZE_7MHD, PHOTO_SIZE_5M, PHOTO_SIZE_3M, \
        PHOTO_SIZE_2MHD, PHOTO_SIZE_VGA};
#endif
#endif

    Perf_Open();
    uiTimeCount1 = Perf_GetCurrent();

    // Disable auto power off
    SysSetFlag(FL_AUTO_POWER_OFF, AUTO_POWER_OFF_DISABLE);
    Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_POWEROFF, 1, AUTO_POWER_OFF_DISABLE);

    // Change to movie mode
    Primary_ChangeMode(PRIMARY_MODE_MOVIE);
    TimerDelayMs(3000);

    for (i = 0; i < MOVIE_SIZE_ID_MAX; i++)
    {
        SysSetFlag(FL_MOVIE_SIZE, uiMovieSize[i]);
        Ux_SendEvent(&UIMovieObjCtrl, NVTEVT_EXE_MOVIESIZE, 1, uiMovieSize[i]);
        TimerDelayMs(500);

        // Record movie for several seconds
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);   // start recording
        TimerDelayMs(2000);

        Ux_PostEvent(NVTEVT_KEY_ZOOMIN,   1, NVTEVT_KEY_PRESS);   // zoom in
        TimerDelayMs(8000);
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN,   1, NVTEVT_KEY_RELEASE); // zoom in release
        TimerDelayMs(200);

        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT,  1, NVTEVT_KEY_PRESS);   // zoom out
        TimerDelayMs(8000);
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT,  1, NVTEVT_KEY_RELEASE); // zoom out release
        TimerDelayMs(200);

        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);   // stop recording
        TimerDelayMs(5000);
    }

    // Change to movie mode
    Primary_ChangeMode(PRIMARY_MODE_PHOTO);
    TimerDelayMs(3000);

    for (i = 0; i < PHOTO_SIZE_ID_MAX; i++)
    {
        SysSetFlag(FL_PHOTO_SIZE, uiPhotoSize[i]);
        Ux_SendEvent(&UIPhotoObjCtrl, NVTEVT_EXE_SIZE, 1, uiPhotoSize[i]);
        TimerDelayMs(500);

        // Take a picture
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);   // take a picture
        TimerDelayMs(5000);

        Ux_PostEvent(NVTEVT_KEY_ZOOMIN,   1, NVTEVT_KEY_PRESS);   // zoom in
        TimerDelayMs(8000);
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN,   1, NVTEVT_KEY_RELEASE); // zoom in release
        TimerDelayMs(200);

        // Take a picture with zoom
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);   // take a picture
        TimerDelayMs(5000);

        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT,  1, NVTEVT_KEY_PRESS);   // zoom out
        TimerDelayMs(8000);
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT,  1, NVTEVT_KEY_RELEASE); // zoom out release
        TimerDelayMs(200);
    }

    // Change to playback mode
    Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);

    debug_msg("^BAuto test time = %d sec\r\n", (Perf_GetCurrent()-uiTimeCount1)/1000000);

    return 0;
}

