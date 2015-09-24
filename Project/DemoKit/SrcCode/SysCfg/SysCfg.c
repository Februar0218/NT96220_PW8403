/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       SysCfg.c
    @ingroup

    @brief      Config the Task System Configuration
                Config the Task System Configuration.

    @note       Nothing.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "SysCfg.h"
#include "externs.h"
#include "UART.h"
#include "GPS.h"
#include "arc_autoredeye.h"

//===========================================================================
// Module functions selection
//===========================================================================
#if (_COMMANDTSK_MODE_ == DISABLE)
void CommandTsk() {}
#endif

#if (_MP3_MODE_ == DISABLE)
void MP3PlayTsk() {}
void MP3PlayFileReadTsk() {}
void MP3PlayFileSys_ResetGlobalTable(void) {}
#endif

// Don't link FD & SD library if it is disabled
#if (_FD_FUNCTION_ == DISABLE)
// FD
UINT32 fd_GetWorkingBufSize(void) { return 0; }
void fd_SetFaceSize(UINT32 Size) {}
void fd_SetInitFlag(UINT32 Flag) {}
void fd_Lock(UINT8 lock) {}
UINT8 fd_IsLock(void) { return 1; }
void fd_SetRate(UINT8 rate) {}
UINT8 fd_GetRate(void) { return 1; }
void fd_FaceDetectionProcess(void) {}
UINT8 GetFD_Num(void){ return 0; }
#endif

#if (_SD_FUNCTION_ == DISABLE)
// SD
void sd_setInitFlag(UINT32 Flag) {}
void sd_Lock(UINT8 lock) {}
UINT8 sd_IsLock(void) { return 1; }
void sd_SetRate(UINT8 rate) {}
UINT8 sd_GetRate(void) { return 1; }
UINT32 sd_GetResult(void) { return 1; }
void sd_SmileDetectionProcess(void) {}
#endif

#if (_REDEYE_FUNCTION_ == DISABLE)
MRESULT ARER_AutoRemove (
    LPARER_OFFSCREEN    pInputImg,
    MVoid           *pWorkMem,
    MLong           nWorkMemSize,
    ARER_FNPROGRESS fnCallback,
    MVoid           *pUserParam)
{
    return 0;
}
#endif


#if (_GPS_RECEIVER_ == DISABLE)
void GPSRecTsk(void) {}
BOOL GPSRec_CheckData(void) {return FALSE;}
RMCINFO RMCInfo;
GGAINFO GGAInfo;
#endif

//===========================================================================
// (1) User defined task initital table
//===========================================================================
__align(4) UCHAR pucStkKeyScan[STKSIZE_KEYSCAN];
__align(4) UCHAR pucStkCommand[STKSIZE_COMMAND];
__align(4) UCHAR pucStkPlaySound[STKSIZE_PLAYSOUND];

__align(4) UCHAR pucStkUIFramework[STKSIZE_UIFRAMEWORK];
__align(4) UCHAR pucStkUIBackgound[STKSIZE_UIBACKGROUND];

#if (_CALIBRATION_MODE_ == ENABLE)
__align(4) UCHAR pucStkCalbiration[STKSIZE_CALIBRATION];
#endif

//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
#if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
__align(4) UCHAR pucStkMsdcVendorNvtTsk[STKSIZE_MSDC_DBGSYS];
#endif
//#NT#2009/09/09#Niven Cho -end


//===========================================================================
// ID, Function pointer, Priority, Stack Size, Stack Address
//===========================================================================
extern void KeyScanTsk(void);
extern void CommandTsk(void);
extern void PlaySoundTsk(void);
extern void UIFrameworkTsk(void);
extern void UIBackgroundTsk(void);
extern void CalibrationTsk(void);

//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
extern void MsdcVendorNvtTsk(void);
//#NT#2009/09/09#Niven Cho -end




//===========================================================================
// User defined task initital table
// ID, Function pointer, Priority, Stack Size, Stack Address
//===========================================================================
__align(4) TASKINFO user_systasktabl[USER_TSK_NUM] =
{
    // Task ID,             Priority,           Stack size,             Func,               Stack
    {KEYSCANTSK_ID,         PRI_KEYSCAN,        STKSIZE_KEYSCAN,        KeyScanTsk,         pucStkKeyScan,          "KeyScanTsk()"},
    {COMMANDTSK_ID,         PRI_COMMAND,        STKSIZE_COMMAND,        CommandTsk,         pucStkCommand,          "CommandTsk()"},
    {PLAYSOUNDTSK_ID,       PRI_PLAYSOUND,      STKSIZE_PLAYSOUND,      PlaySoundTsk,       pucStkPlaySound,        "PlaySoundTsk()"},
    {UIFRAMEWORKTSK_ID,     PRI_UIFRAMEWORK,    STKSIZE_UIFRAMEWORK,    UIFrameworkTsk,     pucStkUIFramework,      "UIFrameworkTsk()"},
    {UIBACKGROUNDTSK_ID,    PRI_BACKWORKER,     STKSIZE_UIBACKGROUND,   UIBackgroundTsk,    pucStkUIBackgound,      "UIBackgroundTsk()"},
    #if (_CALIBRATION_MODE_ == ENABLE)
    {CALIBRATIONTSK_ID,     PRI_CALIBRATION,    STKSIZE_CALIBRATION,    CalibrationTsk,     pucStkCalbiration,      "CalibrationTsk()"},
    #endif

    //#NT#2009/09/09#Niven Cho -begin
    //#NT#Added., for USB Debug System
    #if(_MSDCVENDOR_ == _MSDCVENDOR_NVT_)
    {MSDC_DBGSYS_TSK_ID,    PRI_MSDC_DBGSYS,    STKSIZE_MSDC_DBGSYS,    MsdcVendorNvtTsk,   pucStkMsdcVendorNvtTsk, "MsdcVendorNvtTsk()"},
    #endif
    //#NT#2009/09/09#Niven Cho -end
};



//=====================================================================
//  (2)User defined event flag table
//=====================================================================
__align(4) SYSTEM_EVF_TABLE user_sysevftabl[USER_EVF_NUM] =
{
//   Flag ID,               Flag Name
    {FLG_ID_KEY,            "FLG_ID_KEY"},
    {FLG_ID_KEY_REL,        "FLG_ID_KEY_REL"},
    {FLG_ID_KEY_CONT,       "FLG_ID_KEY_CONT"},
    {FLG_ID_KEY_LONG,       "FLG_ID_KEY_LONG"},
    {FLG_ID_SOUND,          "FLG_ID_SOUND"},
    {FLG_ID_FRAMEWORK,      "FLG_ID_FRAMEWORK"},
};

//=====================================================================
//  (3)User defined semaphore table
//=====================================================================
__align(4) SYSTEM_SEM_TABLE user_syssemtabl[USER_SEM_NUM] =
{
//    Semaphore ID,             Attribute,  Counter,    Max counter,    SEM Name
//   Cnt > 0, Max >= Cnt, Attribute is useless now

    {SEMID_FRAMEWORK_WINDOW,    128,        1,          1,              "SEMID_FRAMEWORK_WINDOW"},
    {SEMID_FRAMEWORK_DRAW,      128,        1,          1,              "SEMID_FRAMEWORK_DRAW"},
    {SEMID_UIGRAPHIC,           128,        1,          1,              "SEMID_UIGRAPHIC"},
    //#NT#2009/09/09#Niven Cho -begin
    //#NT#Added., New USB Driver Support
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    {SEMID_MSDC_DBGYSYS,        128,        1,          1,              "SEMID_MSDC_DBGYSYS"},
    #endif
    //#NT#2009/09/09#Niven Cho -end
};


//=====================================================================
//  (4)User defined mailbox
//=====================================================================
__align(4) SYSTEM_MBX_TABLE user_sysmbxtabl[USER_MBX_NUM] =
{
//  Mailbox ID,         Mailbox Name
    {MBXID_SYS_EVENT,   "MBXID_SYS_EVENT"},
    {MBXID_BKG_EVENT,   "MBXID_BKG_EVENT"}
};

//=====================================================================
//  (5)User defined fixed memory pool
//=====================================================================
// The address of the pool will be always start at 32 bytes boundary
__align(4) SYSTEM_MEMPOOL_TABLE user_sysfpltabl[USER_FPL_NUM] = {
    // Pool ID,             Pool size,              Pool Counter
    {POOL_ID_FTABLE,        POOL_SIZE_FTABLE,       POOL_CNT_FTABLE,        "POOL_ID_FTABLE"},       // MP3 File Table
    {POOL_ID_GX,            POOL_SIZE_GX,           POOL_CNT_GX,            "POOL_ID_GX"},           // Gx buffer
    {POOL_ID_DEFECT_PIXEL,  POOL_SIZE_DEFECT_PIXEL, POOL_CNT_DEFECT_PIXEL,  "POOL_ID_DEFECT_PIXEL"}, // defect pixel buffer
    {POOL_ID_ECS,           POOL_SIZE_ECS,          POOL_CNT_ECS,           "POOL_ID_ECS"},          // ECS buffer
    {POOL_ID_OSD1,          POOL_SIZE_OSD,          POOL_CNT_OSD1,          "POOL_ID_OSD1"},         // OSD1 buffer
    {POOL_ID_OSD_DUMP,      POOL_SIZE_OSD_DUMP,     POOL_CNT_OSD_DUMP,      "POOL_ID_OSD_DUMP"},     // OSD dump buffer
    {POOL_ID_MSDCVENDOR,    POOL_SIZE_MSDCVENDOR,   POOL_CNT_MSDCVENDOR,    "POOL_ID_MSDCVENDOR"},   // MsdcVendor NVT buffer
    {POOL_ID_FB,            POOL_SIZE_FB,           POOL_CNT_FB,            "POOL_ID_FB"},           // Frame buffer
    {POOL_ID_CAPTURE,       POOL_SIZE_CAPTURE,      POOL_CNT_CAPTURE,       "POOL_ID_CAPTURE"},      // Capture buffer
};


