/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       BurnIn.c
    @ingroup    mIPRJAPTest

    @brief      BurnIn process
                This file is the BurnIn process

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <string.h>
#include <stdio.h>
#include "Kernel.h"
#include "CalBurnIn.h"
#include "GlobalVar.h"
#include "Timer.h"
#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "VoltageDet.h"
#include "CalibrationTsk.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTUserCommand.h"
#include "Filesystsk.h"
#include "Primarytsk.h"
#include "CalibrationInt.h"
#include "ImageAlgInfor.h"
#include "UIPlayObj.h"
#include "UIGraphics.h"


extern     UINT32 Get_NVTCMD(UINT32 keys);
extern     int i2c_getLockStatus(void);
#if (0)//(_CALIBRATION_MODE_ == ENABLE)
//#NT#2007/05/04#Steven Wang -begin
//#Wait to modify
#define TIMEROpen                       timer_open
#define TIMERClose                      timer_close
#define TIMERSet                        timer_set
#define TIMERSet_us                     timer_set_us
#define TIMERPausePlay                  timer_pausePlay
#define TIMERClrINTStatus               timer_clrINTStatus
#define TIMERGetINTStatus               timer_getINTStatus
#define TIMERGetCurrentCount            timer_getCurrentCount
#define TIMERGetTargetCount             timer_getTargetCount
//#NT#2007/05/04#Steven Wang -end

typedef struct _CAPTURE_IMAGE_INFO
{
    UINT16 ubFlashIndex;
    UINT16 ubSizeIndex;
    UINT16 ubQualityIndex;
    UINT16 ubSceneIndex;
    UINT16 ubISOIndex;
    UINT16 ubColorIndex;
    UINT16 ubSaturationIndex;
    UINT16 ubSharpnessIndex;
    UINT16 ubPreviewIndex;
    UINT16 ubDzoomIndex;
    UINT16 ubSelftimerIndex;
 }_CAPTURE_IMAGE_INFO;

typedef __packed struct
{
    UINT32      uiKeyFlagID;
    UINT32      uiKeyEvent;
    UINT32      uiInterval;
    UINT8       uiRepeatCnt;
    UINT8       uiKeyStatus;
} SIM_EVENT, *PSIM_EVENT;

#define TIMER_INTERVAL              50
#define FLASH_TIMEOUT               60000*30       // 30min
#define WAITING_TIMEOUT             5000       // 5 Sec
#define SLIDESHOW_TIMEOUT  60000*9       // 9min

#define SIM_END                     0xFFFFFFFF

#define KEY_STATUS_PRESS            0
#define KEY_STATUS_RELEASE          1

#define BURNIN_OP_BURNIN            0
#define BURNIN_OP_ZOOMAF            1
#define BURNIN_OP_FLASH             2
#define BURNIN_OP_168H              3
#define BURNIN_OP_10MIN             4


#define FLG_ID_BURNIN               FLG_ID_MP3
#define FLGBURNIN_TIMER             0x00000001
#define FLGBURNIN_WAITING           0x00000002
#define FLGBURNIN_TIMEOUT           0x00000004
#define FLGBURNIN_MODE_PHOTO        0x00000008
#define FLGBURNIN_MODE_ASF          0x00000010
#define FLGBURNIN_MODE_PLAYBACK     0x00000020
#define FLGBURNIN_MODE_SETUP        0x00000040
#define FLGBURNIN_ALL               0x0000007F

#define BURNIN_LOG               DISABLE

static INT8                     LogStringBuf[64];
static RECT                     BurnInStringRect = { 64, 192, 180, 24 };
static UINT32                   uiIteration;
static volatile UINT32          uiTimeoutValue, uiTimerCnt;
_CAPTURE_IMAGE_INFO *ImageInfoTable;
INT32 Cal_CheckStatus(void);
void Cal_DPinBurnIn(BOOL checkStatus);

_CAPTURE_IMAGE_INFO BurnInImageInfoTable[] =
{
         {FLASH_OFF, SIZE_5M,  QUALITY_FINE,    SCENE_AUTO,     ISO_AUTO,COLOR_STANDARD,SATURATION_HIGH,  SHARPNESS_HARD,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
         {FLASH_AUTO,SIZE_1_3M,  QUALITY_STANDARD,SCENE_SPORT,   ISO_100, COLOR_VIVID,   SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
         {FLASH_ON,  SIZE_5M,  QUALITY_ECONOMY, SCENE_NIGHT,    ISO_200, COLOR_SEPIA,   SATURATION_LOW,   SHARPNESS_SOFT,  PREVIEW_OFF,DZOOM_10X,SELFTIMER_OFF},
         {FLASH_OFF, SIZE_3M,  QUALITY_FINE,    SCENE_PORTRAIT, ISO_AUTO,COLOR_MONO,    SATURATION_HIGH,  SHARPNESS_HARD,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
         {FLASH_AUTO,SIZE_5M,QUALITY_STANDARD,SCENE_LANDSCAPE,ISO_100, COLOR_STANDARD,SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_OFF,DZOOM_10X,SELFTIMER_OFF},
         {FLASH_ON,  SIZE_VGA, QUALITY_ECONOMY, SCENE_BACKLIGHT,ISO_200, COLOR_VIVID,   SATURATION_LOW,   SHARPNESS_SOFT,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
         {FLASH_OFF, SIZE_3M,  QUALITY_FINE,    SCENE_AUTO,     ISO_AUTO,COLOR_SEPIA,   SATURATION_HIGH,  SHARPNESS_HARD,  PREVIEW_OFF,DZOOM_10X,SELFTIMER_OFF},
         {FLASH_ON, SIZE_1_3M,  QUALITY_STANDARD,SCENE_AUTO,      ISO_100, COLOR_MONO,    SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_ON, DZOOM_20X,SELFTIMER_OFF},
         {FLASH_OFF, SIZE_3M,  QUALITY_ECONOMY, SCENE_AUTO,     ISO_200, COLOR_STANDARD,SATURATION_LOW,   SHARPNESS_SOFT,  PREVIEW_OFF, DZOOM_10X,SELFTIMER_ON_2S},
};


const SIM_EVENT BurnInEvent[] =
{
    //3 min one loop
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 8000, 8, KEY_STATUS_PRESS},   // 00  capture 9 pictures
    {FLG_ID_KEY, FLGKEY_DOWN,     7000, 1, KEY_STATUS_PRESS},   // 01  movie mode
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 1000, 1, KEY_STATUS_PRESS},   // 02
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 6000, 1, KEY_STATUS_PRESS},   // 03  10 sec stop record
    {FLG_ID_KEY, FLGKEY_PLAYBACK, 4000, 1, KEY_STATUS_PRESS},   // 04  playback mode
    {FLG_ID_KEY, FLGKEY_UP,       1000, 1, KEY_STATUS_PRESS},   // 05  playback menu
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 1, KEY_STATUS_PRESS},   // 06
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 2, KEY_STATUS_PRESS},   // 07
    {FLG_ID_KEY, FLGKEY_ENTER,   33000, 1, KEY_STATUS_PRESS},   // 08  stop slide show
    {FLG_ID_KEY, FLGKEY_UP,       2000, 1, KEY_STATUS_PRESS},   // 09  playback menu
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 2, KEY_STATUS_PRESS},   // 10
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 1, KEY_STATUS_PRESS},   // 11
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 12  execute delete one
    {FLG_ID_KEY, FLGKEY_UP,       1500, 1, KEY_STATUS_PRESS},   // 13  playback menu
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 2, KEY_STATUS_PRESS},   // 14
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 15
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 1, KEY_STATUS_PRESS},   // 16
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 2, KEY_STATUS_PRESS},   // 17  execute protect all
    {FLG_ID_KEY, FLGKEY_UP,       3000, 1, KEY_STATUS_PRESS},   // 18  playback menu
    {FLG_ID_KEY, FLGKEY_RIGHT,     500, 1, KEY_STATUS_PRESS},   // 19  setup menu
    {FLG_ID_KEY, FLGKEY_UP,        500, 4, KEY_STATUS_PRESS},   // 19  format item
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 20
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 1, KEY_STATUS_PRESS},   // 21
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 22  execute format
    {FLG_ID_KEY, FLGKEY_DOWN,      2000, 1, KEY_STATUS_PRESS},   // 23
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 2, KEY_STATUS_PRESS},   // 23
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 25   enter sys reset
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 1, KEY_STATUS_PRESS},   // 23
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 25   exe sys reset
    {FLG_ID_KEY, FLGKEY_LEFT,      1000, 2, KEY_STATUS_PRESS},   // 26   change to photo mode
    {FLG_ID_KEY, FLGKEY_DOWN,      1000, 2, KEY_STATUS_PRESS},
    //{FLG_ID_KEY, FLGKEY_DOWN, 100, 3, KEY_STATUS_PRESS},

    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, WAITING_TIMEOUT, 1, 0},
    // End
    {SIM_END, 0, 0, 0, 0}
};

const SIM_EVENT ZoomAFEvent[] =
{
    // Wide
    {FLG_ID_KEY, FLGKEY_ZOOMOUT, 500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_ZOOMOUT, 4000, 1, KEY_STATUS_RELEASE},
    // Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},
    // Tele
    {FLG_ID_KEY, FLGKEY_ZOOMIN, 500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_ZOOMIN, 4000, 1, KEY_STATUS_RELEASE},
    // Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, WAITING_TIMEOUT, 1, 0},
    // End
    {SIM_END, 0, 0, 0, 0}
};


_CAPTURE_IMAGE_INFO FlashImageInfoTable[] =
{
     {FLASH_ON, SIZE_5M,  QUALITY_STANDARD,    SCENE_NIGHT,   ISO_AUTO, COLOR_STANDARD,SATURATION_NORNAL,  SHARPNESS_HARD,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_OFF,SIZE_5M,  QUALITY_STANDARD,    SCENE_AUTO,   ISO_AUTO, COLOR_STANDARD, SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_ON, SIZE_5M,  QUALITY_STANDARD,    SCENE_NIGHT,   ISO_AUTO, COLOR_STANDARD,SATURATION_NORNAL,  SHARPNESS_HARD,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_OFF,SIZE_5M,  QUALITY_STANDARD,    SCENE_AUTO,   ISO_AUTO, COLOR_STANDARD, SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_ON, SIZE_5M,  QUALITY_STANDARD,    SCENE_NIGHT,   ISO_AUTO, COLOR_STANDARD,SATURATION_NORNAL,  SHARPNESS_HARD,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_OFF,SIZE_5M,  QUALITY_STANDARD,    SCENE_AUTO,   ISO_AUTO, COLOR_STANDARD, SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_ON, SIZE_5M,  QUALITY_STANDARD,    SCENE_NIGHT,   ISO_AUTO, COLOR_STANDARD,SATURATION_NORNAL,  SHARPNESS_HARD,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_OFF,SIZE_5M,  QUALITY_STANDARD,    SCENE_AUTO,   ISO_AUTO, COLOR_STANDARD, SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_ON, SIZE_5M,  QUALITY_STANDARD,    SCENE_NIGHT,   ISO_AUTO, COLOR_STANDARD,SATURATION_NORNAL,  SHARPNESS_HARD,  PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},
     {FLASH_OFF,SIZE_5M,  QUALITY_STANDARD,    SCENE_AUTO,   ISO_AUTO, COLOR_STANDARD, SATURATION_NORNAL,SHARPNESS_NORMAL,PREVIEW_ON, DZOOM_10X,SELFTIMER_OFF},

};
const SIM_EVENT FlashEvent[] =
{
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 30000, 10, KEY_STATUS_PRESS},   // 00  capture 9 pictures
    {FLG_ID_KEY, FLGKEY_PLAYBACK, 10000, 1, KEY_STATUS_PRESS},   // 04  playback mode
    {FLG_ID_KEY, FLGKEY_UP,       1000, 1, KEY_STATUS_PRESS},   // 05  playback menu
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 1, KEY_STATUS_PRESS},   // 06
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 2, KEY_STATUS_PRESS},   // 07
    {FLG_ID_KEY, FLGKEY_ENTER,   (60000*8), 1, KEY_STATUS_PRESS},   // 08  stop slide show
    {FLG_ID_KEY, FLGKEY_UP,       3000, 1, KEY_STATUS_PRESS},   // 18  playback menu
    {FLG_ID_KEY, FLGKEY_RIGHT,     500, 1, KEY_STATUS_PRESS},   // 19  setup menu
    {FLG_ID_KEY, FLGKEY_UP,        500, 4, KEY_STATUS_PRESS},   // 19  format item
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 20
    {FLG_ID_KEY, FLGKEY_DOWN,      500, 1, KEY_STATUS_PRESS},   // 21
    {FLG_ID_KEY, FLGKEY_ENTER,     500, 1, KEY_STATUS_PRESS},   // 22  execute format
    {FLG_ID_KEY, FLGKEY_LEFT,      2000, 2, KEY_STATUS_PRESS},   // 26   change to photo mode
    {FLG_ID_KEY, FLGKEY_SHUTTER2,      60000, 1, KEY_STATUS_PRESS},   // 26   change to photo mode
    {SIM_END, 0, 0, 0, 0}

};

void Cal_SetImageInfor_Value(UINT32 uiRepeatCnt)
{
    VControl *pSysCtrl = Ux_GetSysObj();
    debug_err(("Cal_SetImageInfor_Value  %x  flash %d\r\n",uiRepeatCnt ,ImageInfoTable[uiRepeatCnt].ubFlashIndex));

    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_FLASH,1,ImageInfoTable[uiRepeatCnt].ubFlashIndex);
    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_SIZE,1,ImageInfoTable[uiRepeatCnt].ubSizeIndex);
    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_QUALITY,1,ImageInfoTable[uiRepeatCnt].ubQualityIndex);
    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_SCENEMODE,1,ImageInfoTable[uiRepeatCnt].ubSceneIndex);
    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_ISO,1,ImageInfoTable[uiRepeatCnt].ubISOIndex);
    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_COLOR,1,ImageInfoTable[uiRepeatCnt].ubColorIndex);
    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_SATURATION,1,ImageInfoTable[uiRepeatCnt].ubSaturationIndex);
    Ux_SendEvent(pSysCtrl,NVTEVT_EXE_SHARPNESS,1,ImageInfoTable[uiRepeatCnt].ubSharpnessIndex);

}

/**
  Show BurnIn loop number

  Show BurnIn loop number
  [CalBurnIn internal API]

  @param UINT32 uiLoop: Loop number
  @param UINT32 uiItem: Item number
  @return void
*/
static void Cal_BurnInShowLoop(UINT32 uiLoop, UINT32 uiItem,UINT32 uiRepeatCnt)
{
    INT8 StringBuf[32];
    sprintf(StringBuf, "Loop=%ld,Item=%ld %ld", uiLoop, uiItem,uiRepeatCnt);

    Cal_ShowString(StringBuf, &BurnInStringRect, TRUE);

}

static void Cal_BurnInLog(UINT32 uiLoop, UINT32 uiItem,UINT32 uiRepeat)
{
    UINT32 Fsize = 64;

    sprintf(LogStringBuf, "Loop=%ld,Item=%ld,repeat=%ld \r\n", uiLoop, uiItem,uiRepeat);

#if (BURNIN_LOG == ENABLE)
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    //FilesysNANDWriteSysParamBlockByBytes((INT8 *)&BurnInStringRect,0,32 );
    FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, (char *)"A:\\BurnIn.log", (UB *)LogStringBuf,
                            &Fsize, 0, FST_TIME_INFINITE);
#endif
}
/**
  BurnIn timer ISR

  BurnIn timer ISR
  [CalBurnIn internal API]

  @param UINT32 uiLoop: Loop number
  @param UINT32 uiItem: Item number
  @return void
*/
static void Cal_BurnInTimerISR(void)
{
    UINT32 uiFlag;

    uiFlag = FLGBURNIN_TIMER;
    if (uiTimeoutValue != 0)
    {
        uiTimerCnt += TIMER_INTERVAL;

        if (uiTimerCnt >= uiTimeoutValue)
        {
            uiFlag |= FLGBURNIN_TIMEOUT;
        }
    }
    set_flg(FLG_ID_BURNIN, uiFlag);
}

/**
  BurnIn main loop

  BurnIn main loop
  [CalBurnIn internal API]

  @param UINT32 uiOperation: Which operation you want to run
    BURNIN_OP_BURNIN            (Burn In)
    BURNIN_OP_ZOOMAF            (ZoomAF Lasting)
    BURNIN_OP_FLASH             (Flash Lasting)
    BURNIN_OP_168H              (168H)
  @return void
*/
static void Cal_BurnIn(UINT32 uiOperation)
{
    UINT32      uiTimerID, uiFlag;
    UINT32      uiKeyFlagID, uiKeyEvent;
//    UINT32      uiOldPressConfig, uiOldReleaseConfig;
    UINT32      uiTotalIteration;
    UINT32      uiInterval;
    UINT32      uiEventCnt;
    PSIM_EVENT  pSimEvent, pStartEvent;
    UINT8       uiKeyStatus, uiRepeatCnt;

    if(i2c_getLockStatus())
        i2c_unlock();

    // Open timer
    if (TIMEROpen((UINT *)&uiTimerID, Cal_BurnInTimerISR) != E_OK)
    {
        /*
        set_scankey_press_config(uiOldPressConfig);
        set_scankey_release_config(uiOldReleaseConfig);
        */
        return;
    }

    switch (uiOperation)
    {
    case BURNIN_OP_BURNIN:
        //read cal status for check finish item
        ReadCalData();
        FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE);

        //if(Cal_CheckStatus()!= 0)
        //    return ;
        uiTotalIteration    = 40; //2hour    //10 ;   // 10*3 min = 30 mins
        pStartEvent         = (PSIM_EVENT)&BurnInEvent;
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        break;

    case BURNIN_OP_ZOOMAF:
        uiTotalIteration    = 1000;
        pStartEvent         = (PSIM_EVENT)&ZoomAFEvent;
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        break;

    case BURNIN_OP_FLASH:
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE);
        uiTotalIteration    = 8;  //15 min * 8 = 2 hr
        pStartEvent         = (PSIM_EVENT)&FlashEvent;
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        SysSetFlag(FL_FLASH, FLASH_ON);
        break;

    case BURNIN_OP_168H:
        //read cal status for check finish item
        ReadCalData();
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE);

        //if(Cal_CheckStatus()!= 0)
        //     return ;
        uiTotalIteration    = 0xFFFFFFFF;
        pStartEvent         = (PSIM_EVENT)&BurnInEvent;
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        break;

    case BURNIN_OP_10MIN:
        //read cal status for check finish item
        ReadCalData();
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE);

        //if(Cal_CheckStatus()!= 0)
        //     return ;
        uiTotalIteration    = 10; // 3 min * 10 =30 mins
        pStartEvent         = (PSIM_EVENT)&BurnInEvent;
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        break;

    }

    // Init variable
    uiTimeoutValue  = 0;

    // Enable timer and clear all flags
    clr_flg(FLG_ID_BURNIN, FLGBURNIN_ALL);
    TIMERSet(uiTimerID, TIMER_INTERVAL, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

    for (uiIteration=1; uiIteration<=uiTotalIteration; uiIteration++)
    {
        debug_ind(("\n\r ***Loop = %ld,Event= %ld ***\r\n", uiIteration, uiEventCnt));
        pSimEvent   = pStartEvent;
        uiRepeatCnt = 0;
        uiInterval  = 0;
        uiEventCnt  = 0;

        if(uiOperation == BURNIN_OP_FLASH)
        {
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_EXE_SLIDE,1,SLIDE_5SEC);
            ImageInfoTable = FlashImageInfoTable;
        }
        else
            ImageInfoTable = BurnInImageInfoTable;
        TimerDelayMs(2000);
        Cal_SetImageInfor_Value(uiRepeatCnt);
        while (1)
        {
            wai_flg((UINT *)&uiFlag, FLG_ID_BURNIN, FLGBURNIN_TIMER, TWF_ORW | TWF_CLR);
            uiInterval += TIMER_INTERVAL;

            //avoid display msg too much
            if(uiInterval%2000 == 0)
                Cal_BurnInShowLoop(uiIteration, uiEventCnt,uiRepeatCnt);

            //accumlate time until to interval;the post event
            if (uiInterval >= pSimEvent->uiInterval)
            {
                uiInterval = 0;
                #if 0
                if((uiOperation == BURNIN_OP_BURNIN || uiOperation == BURNIN_OP_10MIN ) && (uiIteration == 4))   //about 10 min,run dark dp
                {
                    Cal_BurnInLog(uiIteration,uiEventCnt,999);
                    if(uiOperation == BURNIN_OP_10MIN)
                        Cal_DPinBurnIn(FALSE);  //not check dp status
                    else
                        Cal_DPinBurnIn(TRUE);   // check dp status
                    break;
                }
                #endif

                // End of event
                if (pSimEvent->uiKeyFlagID == SIM_END)
                {
                    break;
                }
                // Wait for specific flag, or change mode
                else if (pSimEvent->uiKeyFlagID == FLG_ID_BURNIN)
                {
                    if (pSimEvent->uiKeyEvent == FLGBURNIN_WAITING)
                    {
                        // Enable timeout
                        uiTimeoutValue  = pSimEvent->uiInterval;
                        uiTimerCnt      = 0;
                        clr_flg(FLG_ID_BURNIN, FLGBURNIN_TIMEOUT);

                        Cal_BurnInLog(uiIteration,uiEventCnt,uiRepeatCnt);
                        wai_flg((UINT *)&uiFlag, FLG_ID_BURNIN, pSimEvent->uiKeyEvent | FLGBURNIN_TIMEOUT, TWF_ORW | TWF_CLR);

                        if (uiFlag & FLGBURNIN_TIMEOUT)
                        {
                            debug_err(("Wait for specific flag timeout  %d\r\n",pSimEvent->uiInterval));
                        }

                        // Disable timeout
                        uiTimeoutValue = 0;

                        // Clear timer flag
                        clr_flg(FLG_ID_BURNIN, FLGBURNIN_TIMER);

                        uiRepeatCnt = 0;
                        pSimEvent++;
                        uiEventCnt++;
                        debug_ind(("\n\rFLGBURNIN_WAITING  ***Loop = %ld,Event= %ld ***\r\n", uiIteration, uiEventCnt));
                        continue;
                    }


                }
                else // FLG_ID_KEY
                {
                    //log current event
                    Cal_BurnInLog(uiIteration,uiEventCnt,uiRepeatCnt);

                    uiKeyFlagID = pSimEvent->uiKeyFlagID;
                    uiKeyEvent  = pSimEvent->uiKeyEvent;
                    uiKeyStatus = pSimEvent->uiKeyStatus;

                    // Set key event
                    if (uiKeyFlagID != NULL && uiKeyEvent != NULL)
                    {
                        //change key event to post message
                        if(uiKeyFlagID == FLG_ID_KEY)
                        {
                        /*
                            NVTMSG msg;

                            if(uiKeyStatus == KEY_STATUS_PRESS)
                                SET_COMMAND(&msg, NVTEVT_KEY_PRESS);
                            else
                                SET_COMMAND(&msg, NVTEVT_KEY_RELEASE);

                            debug_ind(("uiKeyEvent %x nvtcmd %x \r\n",uiKeyEvent ,Get_NVTCMD(uiKeyEvent)));
                            SET_KEY_EVENT(&msg,Get_NVTCMD(uiKeyEvent));
                            PostMessage(&msg);
                        */
                            if(uiKeyEvent & (KeyScan_GetKeyMask(KEYSCAN_KEYMODE_PRESS)))
                            {
                                debug_err(("Ux_PostEvent %d \r\n",uiKeyEvent));
                                Ux_PostEvent(Get_NVTCMD(uiKeyEvent), 1, Get_NVTCMD(uiKeyEvent));
                            }

                        }
                        else
                        {
                            set_flg(uiKeyFlagID, uiKeyEvent);
                        }
                    }
                }

                uiRepeatCnt++;


                // Move to next event
                if (uiRepeatCnt == pSimEvent->uiRepeatCnt)
                {
                    uiRepeatCnt = 0;
                    pSimEvent++;
                    uiEventCnt++;
                    debug_ind(("*****next event*Loop = %ld,Event= %ld ***************\r\n", uiIteration, uiEventCnt));
                }

                if ((pSimEvent == pStartEvent)&&((uiOperation==BURNIN_OP_10MIN)||(uiOperation==BURNIN_OP_BURNIN) || (uiOperation==BURNIN_OP_168H) || (uiOperation==BURNIN_OP_FLASH)) )
                {
                     TimerDelayMs(2000);
                     //for set flash charg,so set capture index first
                     Cal_SetImageInfor_Value(uiRepeatCnt);

                }

            }

        }
    }

    TIMERClose(uiTimerID);

    //tmp write to AAA for read cal data
    AAA_Adjust.ISO_Status = _ISO_Status;
    WriteCalData();

    if(uiOperation == BURNIN_OP_FLASH)
        sprintf(LogStringBuf,"Flash O\r\n");
    else
        sprintf(LogStringBuf,"BurnIn O\r\n");
    Cal_ShowStringWithColor((INT8 *)LogStringBuf, 64,170, _OSD_INDEX_GREEN);

#if 0
    Menu_PowerOff(POWER_OFF);
#endif
    while(1)
        ;;


    /*
    set_scankey_press_config(uiOldPressConfig);
    set_scankey_release_config(uiOldReleaseConfig);
    */

    // Power off DSC after burn in done
    // Premier don't want to power off now (2005.10.16)
    /*
    if (uiOperation == BURNIN_OP_BURNIN)
    {
        PowerOffFlow(TRUE, FALSE);
    }
    */
}

/**
  Get BurnIn iteration (loop number)

  Get BurnIn iteration (loop number)

  @param void
  @return UINT32: Iteration
*/
UINT32 Cal_BurnInGetIteration(void)
{
    return uiIteration;
}
CAL_ITEM g_BurnInItem[]    =
{
    // Display Name             Function pointer
    {"Burnin 0.5H",                     Cal_BurnInBurnIn },
    {"Burnin 2H",                      Cal_BurnIn10min },
};
void Cal_BurnInMenu(void)
{

    UINT32 i,uiFlag =0;
    UINT32 uiSelectNum =0;
    UINT32 g_CalBurninItemNum =1;
    INT8   StringBuffer[32];

    while(1)
    {

        #if 0
        if(uiSelectNum == 0)
            sprintf(StringBuffer, "Burnin 0.5H");
        else
            sprintf(StringBuffer, "Burnin 2H");
        Cal_ShowStringWithColor(StringBuffer,20, 50+i*30,_OSD_INDEX_RED);
        #else //draw mulit line menu
        for(i=0;i<=g_CalBurninItemNum;i++)
        {
            if(i ==uiSelectNum)
                Cal_ShowStringWithColor(g_BurnInItem[i].pItemName,20, 50+i*30,_OSD_INDEX_RED);
            else
                Cal_ShowStringWithColor(g_BurnInItem[i].pItemName,20, 50+i*30,_OSD_INDEX_GRAY);
        }
        #endif
        wai_flg((UINT *)&uiFlag, FLG_ID_KEY, (FLGKEY_UP | FLGKEY_DOWN | FLGKEY_SHUTTER2 |FLGKEY_LEFT), TWF_ORW | TWF_CLR);

        if (uiFlag & FLGKEY_UP)
        {
            if(uiSelectNum == 0)
            {
                uiSelectNum = g_CalBurninItemNum;
            }
            else
            {
                uiSelectNum--;
            }
        }

        // Next item
        if (uiFlag & FLGKEY_DOWN)
        {
            if(uiSelectNum == g_CalBurninItemNum)
            {
                uiSelectNum = 0;
            }
            else
            {
                uiSelectNum++;
            }
        }

        if (uiFlag & FLGKEY_SHUTTER2)
        {
            if(uiSelectNum == 0)
                Cal_BurnIn10min();
            else
                Cal_BurnInBurnIn();
        }

        if (uiFlag & FLGKEY_LEFT)
        {
            break;
        }

    }

}

/**
  Run burn in

  Run burn in

  @param void
  @return void
*/
void Cal_BurnInBurnIn(void)
{
    Cal_BurnIn(BURNIN_OP_BURNIN);
}

/**
  Run Zoom AF lasting

  Run Zoom AF lasting

  @param void
  @return void
*/
void Cal_BurnInZoomAF(void)
{
    Cal_BurnIn(BURNIN_OP_ZOOMAF);
}

/**
  Run Flash lasting

  Run Flash lasting

  @param void
  @return void
*/
void Cal_BurnInFlash(void)
{
    Cal_BurnIn(BURNIN_OP_FLASH);
}

/**
  Run 168H

  Run 168H

  @param void
  @return void
*/
void Cal_BurnIn168H(void)
{
    Cal_BurnIn(BURNIN_OP_168H);
}
void Cal_BurnIn10min(void)
{
    Cal_BurnIn(BURNIN_OP_10MIN);
}


INT32 Cal_CheckStatus(void)
{
    if( (AAA_Adjust.DP_Status.Prv_White!=_Prv_W_Status)||(AAA_Adjust.DP_Status.Cap_White!=_Cap_W_Status) )
    {
        char StringBuf[64];
        sprintf(StringBuf,"White defect pixel fail \r\n");
        debug_err(("%s  \r\n",StringBuf));
        Cal_ShowString(StringBuf, &BurnInStringRect, TRUE);

        TimerDelayMs(1000);
        return -1;
    }

#if 0
    //check if remove card
    while(1)
    {
        if(Primary_GetChangeStorage() == PRIMARY_STGTYPE_CARD)
        {
            char StringBuf[64];
            sprintf(StringBuf,"Please Remove Card \r\n");
            Cal_ShowString(StringBuf, &BurnInStringRect, TRUE);
        }
        else
        {
            break;
        }
        TimerDelayMs(1000);
    }

    Menu_SysReset(0);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE);
#endif
    return 0;
}




void Cal_DPinBurnIn(BOOL checkStatus)
{
    ALG_CAL_PARAM CalParam;

    if( (checkStatus) && (AAA_Adjust.DP_Status.Prv_Black==_Prv_B_Status)&&(AAA_Adjust.DP_Status.Cap_Black==_Cap_B_Status) )
    {
        char StringBuf[64];
        sprintf(StringBuf,"dark pixel ok \r\n");
        debug_err(("%s  \r\n",StringBuf));
        Cal_ShowString(StringBuf, &BurnInStringRect, TRUE);

        debug_err(("warn pixel ok  \r\n"));
        return ;
    }
    else
    {
        char StringBuf[64];
        sprintf(StringBuf,"n warn dark defect pixel   \r\n");
        debug_err(("%s  \r\n",StringBuf));
        Cal_ShowString(StringBuf, &BurnInStringRect, TRUE);

        debug_err(("rn warn dark defect pixel   \r\n"));
    }

    // Close all task
    AppInit_Close();

    CalParam.FnShowString = Cal_ShowRectStringWithColor;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();

    Alg_Cal(CAL_DEFECTPIXEL_DARK, &CalParam);

    //change back to photo mode
    Primary_ChangeMode(PRIMARY_MODE_PHOTO);

}
#endif

//@}
