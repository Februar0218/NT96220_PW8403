/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       SysCfg.h
    @ingroup

    @brief      Header file of SysCfg.c
                Config the task, event, semaphore, memory pool, memory layout, mailbox

    @note       Nothing.
*/

#ifndef _SYSCFG_H
#define _SYSCFG_H

#include "Type.h"
#include "Kernel.h"
#include "StdCMac.h"
#include "Chipset.h"
#include "KerCfg.h"
#include "OSCommon.h"
#include "PStore.h"

#define _PROJECT_TUOBU_        DISABLE//ÍØ²½set enable else set disable

//===========================================================================
// Module functions selection
// The following symbols may depends on these module functions selection.
// You must take care the library during linking time too.
//===========================================================================
//---------------------------------------------------------------------------
//For IQ Modules
//---------------------------------------------------------------------------
#define _IQ_MODE_               ENABLE          // Enable/Disable IQ App functions
#define _IQ_SIM_                DISABLE         // Enable/Disable IQ Sim functions
                                                //  NOTE: need set MSDCVENDOR = MSDCVENDOR_NVT in MakeConfig.txt
//---------------------------------------------------------------------------
//For PLAYBACK Module
//---------------------------------------------------------------------------
#define _PLAYBACK_MODE_         ENABLE          // Enable/Disable Playback App functions

//---------------------------------------------------------------------------
//For Movie Mode Modules
//---------------------------------------------------------------------------
// Movie format
#define _MOVIE_ASFMPEG4_        0               // ASF MPEG-4
#define _MOVIE_AVIMJPEG_        1               // AVI MJPEG
#define _MOVIE_AVIMPEG4_        2               // AVI MPEG-4 (Not support)
#define _MOVIE_MP4MPEG4_        3               // MP4 MPEG-4 (Not support)
#define _MOVIE_MPEG1_           4
#define _MOVIE_MOVMJPG_         5
#define _MOVIE_MEDIA_           6
#define _MOVIE_MODE_            _MOVIE_AVIMJPEG_

// Movie max resolution by user setting
#define _MOVIE_MAX_RES_VGA_     0
#define _MOVIE_MAX_RES_720P_    1
#define _MOVIE_MAX_RES_1080P_   2
#define _MOVIE_MAX_RES_         _MOVIE_MAX_RES_720P_//_MOVIE_MAX_RES_1080P_

#define _MOVIE_AVI_USERDATA_    ENABLE      // Enable/Disable AVI MJPEG with embeded user data
#define _MOVIE_PIM_MODE_        DISABLE          // Enable/Disable Picture In Movie function (RawEnc)

#if (UI_STYLE == UI_STYLE_DRIVE)
#define _MOVIE_DATE_STAMP_      ENABLE     // Enable/Disable movie date stamp function
#define _MOVIE_REC_SEAMLESS_    ENABLE     // Enable/Disable movie seamless recording function
#define _MOVIE_SLOW_CARD_       DISABLE     // Enable/Disable handling slow card in movie mode
#else
#define _MOVIE_DATE_STAMP_      DISABLE      // Enable/Disable movie date stamp function
#define _MOVIE_REC_SEAMLESS_    DISABLE     // Enable/Disable movie seamless recording function
#define _MOVIE_SLOW_CARD_       DISABLE     // Enable/Disable handling slow card in movie mode
#endif

//---------------------------------------------------------------------------
//For USB Modules
//---------------------------------------------------------------------------
#define _USBMSDC_MODE_          ENABLE      // Enable/Disable USB MSDC function
#define _USBSICD_MODE_          ENABLE      // Enable/Disable USB SICD function
#define _USBPCC_MODE_           ENABLE      // Enable/Disable USB PCC function

// MSDC modes
#define _MSDC_SINGLE_           0           // Only the current active storage device can be accessed through USB MSDC, i.e. card "OR" nand.
#define _MSDC_ALL_              1           // All the storage devices can be accessed concurrently through USB MSDC, i.e. card "AND" nand.
#define _MSDC_SINGLE_NAND2_     2           // Only the active storage device plus the NAND2 can be accessed concurrently through USB MSDC.
#define _MSDC_ALL_NAND2_        3           // All the storage devices plus the NAND2 can be accessed concurrently through USB MSDC.
#define _MSDC_SINGLE_NAND3_     4           // Only the active storage device plus the NAND3 can be accessed concurrently through USB MSDC.
#define _MSDC_ALL_NAND3_        5           // All the storage devices plus the NAND3 can be accessed concurrently through USB MSDC.
#define _MSDC_MODE_             _MSDC_SINGLE_

#if ((_MSDC_MODE_ == _MSDC_ALL_NAND3_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_) || (_MSDC_MODE_ == _MSDC_ALL_NAND2_))
#define NAND2_SIZE              0x800000    // In bytes,and this value must be divisible by the block size of the NAND
#endif
#if ((_MSDC_MODE_ == _MSDC_SINGLE_NAND3_) || (_MSDC_MODE_ == _MSDC_ALL_NAND3_))
#define NAND3_SIZE              0xA00000    // In bytes,and this value must be divisible by the block size of the NAND
#endif

//---------------------------------------------------------------------------
//For MP3 Module
//---------------------------------------------------------------------------
#define _MP3_MODE_              DISABLE     // Enable/Disable MP3 player function, 0 = Disable

//---------------------------------------------------------------------------
//For WAV Module
//---------------------------------------------------------------------------
#define _WAVSTUDIO_MODE_        DISABLE     // Enable/Disable WAV file record/playback function

//#NT#2011/09/14#Klins Chen -begin
//---------------------------------------------------------------------------
//For DISP ROTATE Function
//---------------------------------------------------------------------------
#define _DISPROTATE_MODE_       ENABLE          // Enable/Disable display rotation function
//#NT#2011/09/14#Klins Chen -end

// Voice memo type
#define _VM_TYPE_SINGLE_        0           // Single WAV file
#define _VM_TYPE_EMBEDDED_      1           // Embedded WAV into JPG file
#define _VM_TYPE_               _VM_TYPE_SINGLE_

// Voice memo format
#define _VM_FORMAT_PCM8_        0
#define _VM_FORMAT_PCM16_       1
#define _VM_FORMAT_ADPCM_       2
#define _VM_FORMAT_ASFPCM_      3
#define _VM_FORMAT_             _VM_FORMAT_PCM16_

#define _WAV_SINGLE_USERDATA_   DISABLE     // Enable/Disable WAV (Single file) user data in header

//---------------------------------------------------------------------------
//For MISC Module
//---------------------------------------------------------------------------
#define _COMMANDTSK_MODE_       ENABLE      // Enable/Disable Command task
#define _SYSINFO_MODE_          ENABLE      // Enable/Disable Get System Information function
#define _CALIBRATION_MODE_      ENABLE      // Enable/Disable Calibration
#define _USBWRITE_MODE_         DISABLE     // Enable/Disable Write data to Nand Flash via USB PCC mode function
#define _LOADFW_INMAIN_         DISABLE     // Enable/Disable load FW to run in main code
#define _TV_FUNCTION_           ENABLE      // Enable/Disable TV function
#define _OSD_SCREEN_DUMP_       DISABLE     // Enable/Disable OSD screen dump function
#define _PCC_720P_FUNC_         ENABLE      // Enable/Disable PCC 720P function support
#define _PCC_BULK_MODE_         DISABLE     // Enable/Disable PCC using BulkOut (Non-UVC) mode
#define _SD_CLASS4_REC_         ENABLE      // Enable/Disable to allow recording with Class4 sd card
#if (_LCDTYPE2_ != _LCDTYPE2_OFF_)
#define _DUAL_PANEL_            ENABLE      // Enable/Disable dual panel function support
#else
#define _DUAL_PANEL_            DISABLE      // Enable/Disable dual panel function support
#endif

// PStore section name ID
//#define PS_CAL_DATA             MAKE_PS_SECNAME_ID('C', 'A', 'L', 'D')//ethan:move to cal_ps_rw.c
#define PS_UNIFONT_DATA         MAKE_PS_SECNAME_ID('U', 'F', 'N', 'T')
#define PS_PHOTO_FRAME_HEADER   MAKE_PS_SECNAME_ID('P', 'F', 'H', 'E')
#define PS_PHOTO_FRAME_DATA     MAKE_PS_SECNAME_ID('P', 'F', 'D', 'A')
#define PS_SYS_PARAM            MAKE_PS_SECNAME_ID('S', 'Y', 'S', 'P')
#define PS_SYS_CALBR            MAKE_PS_SECNAME_ID('C', 'A', 'L', 'B')
#define PS_BG_CFG               MAKE_PS_SECNAME_ID('C', 'F', 'G', 'B')
#define PS_BG_LOGO              MAKE_PS_SECNAME_ID('L', 'O', 'G', 'O')
#define PS_BG_LOGO2             MAKE_PS_SECNAME_ID('L', 'O', 'G', 'B')


// OSD color
#define _OSD_OFF_               0               // off
#define _OSD_COLOR_INDEX1_      1               // 2 colors
#define _OSD_COLOR_INDEX2_      2               // 4 colors
#define _OSD_COLOR_INDEX4_      3               // 16 colors
#define _OSD_COLOR_INDEX8_      4               // 256 colors
#define _OSD_COLOR_ARGB4565_    5               // ARGB4565
#define _OSD_COLOR_ARGB8565_    6               // ARGB8565

// VDO color
#define _VDO_OFF_               0               // off
#define _VDO_COLOR_ARGB4565_    5               // ARGB4565
#define _VDO_COLOR_ARGB8565_    6               // ARGB8565
#define _VDO_COLOR_YUV420_      7               // 420 colors
#define _VDO_COLOR_YUV422_      8               // 422 colors
#define _VDO_COLOR_YUV444_      9               // 444 colors

#define _OSD_COLOR_             _OSD_COLOR_INDEX8_
#define _VDO_COLOR_             _VDO_OFF_

// Sound/Beep play
#define _SUDPLAY_USE_NONE_      0
#define _SUDPLAY_USE_PWM_       1
#define _SUDPLAY_USE_PWM_IOSIM_ 2
#define _SUDPLAY_USE_TSK_       3
#define _SUDPLAY_USE_           _SUDPLAY_USE_NONE_

// MsdcVendor NVT using cache memory
#define _MSDCNVT_MEM_LOCAL_     0
#define _MSDCNVT_MEM_POOL_      1
#define _MSDCNVT_MEM_           _MSDCNVT_MEM_POOL_

// For Video Debug Info
//---------------------------------------------------------------------------
#define _MOVIE_IQLOG_          DISABLE     // Enable/Disable Video IQLOG
//#NT#2011/10/14#Spark Chou -end
//---------------------------------------------------------------------------
// For GPS Receiver Module
//---------------------------------------------------------------------------
#define _GPS_RECEIVER_LOG_      DISABLE//ENABLE     // Enable/Disable GPS Reciver log function

//===========================================================================
// Memory layout
//===========================================================================
// FW size in DRAM (Code + RO + RW + ZI)
// Now defined in MakeConfig.txt
//#define FRE_MEM                 0x00200000

#if ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00001000)
    #define END_MEM             (DRAM_END - 0x00001000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00002000)
    #define END_MEM             (DRAM_END - 0x00002000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00004000)
    #define END_MEM             (DRAM_END - 0x00004000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00008000)
    #define END_MEM             (DRAM_END - 0x00008000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00010000)
    #define END_MEM             (DRAM_END - 0x00010000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00020000)
    #define END_MEM             (DRAM_END - 0x00020000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00040000)
    #define END_MEM             (DRAM_END - 0x00040000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00080000)
    #define END_MEM             (DRAM_END - 0x00080000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00100000)
    #define END_MEM             (DRAM_END - 0x00100000)
#else
    #define END_MEM             (DRAM_END - 0x00200000)
#endif


//===========================================================================
// User defined Task IDs
// Standard tasks IDs are declared in OSCommon.h
//===========================================================================
typedef enum
{
    KEYSCANTSK_ID = STD_TSK_NUM,        // KeyScan task
    COMMANDTSK_ID,                      // UART command task
    PLAYSOUNDTSK_ID,                    // Play sound task
    UIFRAMEWORKTSK_ID,                  // UIFramework task
    UIBACKGROUNDTSK_ID,                 // UIBackground task

    //#NT#2009/07/30#Niven Cho -begin
    //#NT#Added., for MSDCExt with NVT
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    MSDC_DBGSYS_TSK_ID,
    #endif
    //#NT#2009/07/30#Niven Cho -end

    #if (_CALIBRATION_MODE_ == ENABLE)
    CALIBRATIONTSK_ID,                  // Calibration task
    #endif

    USER_TSK_MAX_NUM,                           // MAX number of User defined Task
    ENUM_DUMMY4WORD(USER_TSK)
} USER_TSK;

#define USER_TSK_NUM    (USER_TSK_MAX_NUM-STD_TSK_NUM)

//===========================================================================
// User defined Task Priority
// Standard tasks priority are declared in OSCommon.h
// Lower value higher priority
//===========================================================================
#define PRI_KEYSCAN              6
#define PRI_COMMAND              4
#define PRI_PLAYSOUND            2

#define PRI_UIFRAMEWORK         10
#define PRI_CALIBRATION         16
#define PRI_BACKWORKER          11

//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
#define PRI_MSDC_DBGSYS         10
//#NT#2009/09/09#Niven Cho -end

//===========================================================================
// User defined task Stack size
//===========================================================================
//#NT#2011/09/28#Ks Hung -begin
//#NT#The 4096 bytes has the over flow of probability for HDCF project and
//#NT#the side effect is that the system flag will be covered.
#define STKSIZE_KEYSCAN         8192//4096
//#NT#2011/09/28#Ks Hung -end
#define STKSIZE_COMMAND         4096
#define STKSIZE_PLAYSOUND       2048
#define STKSIZE_UIFRAMEWORK     4096
#define STKSIZE_CALIBRATION     2048
#define STKSIZE_UIBACKGROUND    2048

//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
#define STKSIZE_MSDC_DBGSYS     4096
//#NT#2009/09/09#Niven Cho -end

//===========================================================================
// User defined flag IDs
//===========================================================================
typedef enum
{
    FLG_ID_KEY = STD_EVF_NUM,
    FLG_ID_KEY_REL,
    FLG_ID_KEY_CONT,
    FLG_ID_KEY_LONG,
    FLG_ID_SOUND,
    FLG_ID_FRAMEWORK,

    USER_EVF_MAX_NUM,                           // MAX number of User defined flag
    ENUM_DUMMY4WORD(USER_FLAG)
} USER_FLAG;

#define USER_EVF_NUM    (USER_EVF_MAX_NUM-STD_EVF_NUM)

//===========================================================================
//  User defined Semaphore IDs
//===========================================================================
typedef enum
{
    SEMID_FRAMEWORK_WINDOW = STD_SEM_NUM,
    SEMID_FRAMEWORK_DRAW,
    SEMID_UIGRAPHIC,

    //#NT#2009/07/30#Niven Cho -begin
    //#NT#Added., for MSDCExt with NVT
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    SEMID_MSDC_DBGYSYS,
    #endif
    //#NT#2009/07/30#Niven Cho -end

    USER_SEM_MAX_NUM,                           // MAX number of User defined Semaphore
    ENUM_DUMMY4WORD(USER_SEM)
} USER_SEM;

#define USER_SEM_NUM    (USER_SEM_MAX_NUM-STD_SEM_NUM)

//===========================================================================
//  User defined Mailbox IDs
//===========================================================================
typedef enum
{
    MBXID_SYS_EVENT = STD_MBX_NUM,
    MBXID_BKG_EVENT,

    USER_MBX_MAX_NUM,                           // MAX number of User defined Mailbox
    ENUM_DUMMY4WORD(USER_MBX)
} USER_MBX;

#define USER_MBX_NUM    (USER_MBX_MAX_NUM-STD_MBX_NUM)

//===========================================================================
//  Fixed size memory pool IDs, Sizes and Counts
//===========================================================================
/****** Fixed Size Memory Pool *******/
typedef enum
{
    POOL_ID_FTABLE = STD_FPL_NUM,
    POOL_ID_GX,
    POOL_ID_DEFECT_PIXEL,
    POOL_ID_ECS,
    POOL_ID_OSD1,
    POOL_ID_OSD_DUMP,
    POOL_ID_MSDCVENDOR,
    POOL_ID_FB,
    POOL_ID_CAPTURE,

    USER_FPL_MAX_NUM,                           // MAX number of Fixed size memory pool
    ENUM_DUMMY4WORD(USER_MPOOL)
} USER_MPOOL;

#define USER_FPL_NUM    (USER_FPL_MAX_NUM-STD_FPL_NUM)

#define FB_W                    640
#define FB_H                    480
#define OSD_W                   320
#define OSD_H                   240

// Note: Pool size must be 32 bytes alignment
#define POOL_SIZE_FTABLE        SIZE_ALIGN32(0x54600)       // MP3 file table for maximum ? files
//#define POOL_SIZE_DEFECT_PIXEL  SIZE_ALIGN32(0x2A000)       // 168KB
#define POOL_SIZE_DEFECT_PIXEL  SIZE_ALIGN32(0x17800)       // 94KB
#define POOL_SIZE_ECS           SIZE_ALIGN32(0x4210)        // 65*65*4 = 0x4204

#if (DRAM_END <= 0x00800000)
#define POOL_SIZE_SICD          SIZE_ALIGN32(0x80000)       // 512KB
#else
#define POOL_SIZE_SICD          SIZE_ALIGN32(0x100000)      // 1MB
#endif

#if (_OSD_COLOR_ == _OSD_COLOR_ARGB8565_)
#define POOL_SIZE_OSD           SIZE_ALIGN32(OSD_W*OSD_H*3) // OSD -> 320*240 pixel, RGB565 + A8
#elif (_OSD_COLOR_ == _OSD_COLOR_ARGB4565_)
#define POOL_SIZE_OSD           SIZE_ALIGN32(OSD_W*OSD_H*3) // OSD -> 320*240 pixel, RGB565 + A4
#elif (_OSD_COLOR_ == _OSD_COLOR_INDEX8_)
#define POOL_SIZE_OSD           SIZE_ALIGN32(OSD_W*OSD_H)   // OSD -> 320*240 pixel, 8 bits for one pixel
#elif (_OSD_COLOR_ == _OSD_COLOR_INDEX4_)
#define POOL_SIZE_OSD           SIZE_ALIGN32(OSD_W*OSD_H/2) // OSD -> 320*240 pixel, 4 bits for one pixel
#elif (_OSD_COLOR_ == _OSD_OFF_)
#define POOL_SIZE_OSD           SIZE_ALIGN32(0)             // not used
#else
Error! not support format of OSD1!
#endif

#define POOL_SIZE_OSD_DUMP      SIZE_ALIGN32(0x100000)      // 1MB is recommended for OSD Dump buffer

#define POOL_SIZE_MSDCVENDOR        SIZE_ALIGN32(0x24040)       // MsdcVendor NVT buffer

#define POOL_SIZE_GX            SIZE_ALIGN32(0x1000)        // at leat 4k for Gxlib

#if (DRAM_END <= 0x00800000)
#define POOL_SIZE_FB            SIZE_ALIGN32(FB_W*FB_H*3)
#else
#define POOL_SIZE_FB            SIZE_ALIGN32(FB_W*FB_H*14)
#endif


#if (_MP3_MODE_ == ENABLE)
#define POOL_CNT_FTABLE         1
#else
#define POOL_CNT_FTABLE         0
#endif
#define POOL_CNT_GX             1
#define POOL_CNT_DEFECT_PIXEL   1
#define POOL_CNT_ECS            0 // 1
#define POOL_CNT_OSD1           2 // for double buffer
#if (_OSD_SCREEN_DUMP_ == ENABLE)
#define POOL_CNT_OSD_DUMP       1
#else
#define POOL_CNT_OSD_DUMP       0
#endif
#if ((_MSDCVENDOR_ == _MSDCVENDOR_NVT_) && (_MSDCNVT_MEM_ == _MSDCNVT_MEM_POOL_))
#define POOL_CNT_MSDCVENDOR         1
#else
#define POOL_CNT_MSDCVENDOR         0
#endif
#define POOL_CNT_FB             1
#define POOL_CNT_CAPTURE        1

#define POOL_SIZE_CAPTURE       ((END_MEM - FRE_MEM - POOL_SIZE_COMMON) -\
                                 (POOL_SIZE_FTABLE * POOL_CNT_FTABLE) -\
                                 (POOL_SIZE_DEFECT_PIXEL * POOL_CNT_DEFECT_PIXEL) -\
                                 (POOL_SIZE_ECS * POOL_CNT_ECS) -\
                                 (POOL_SIZE_OSD * POOL_CNT_OSD1) -\
                                 (POOL_SIZE_OSD_DUMP * POOL_CNT_OSD_DUMP) -\
                                     (POOL_SIZE_MSDCVENDOR * POOL_CNT_MSDCVENDOR) -\
                                 (POOL_SIZE_GX * POOL_CNT_GX) -\
                                     (POOL_SIZE_FB * POOL_CNT_FB))

#if (_WAVSTUDIO_MODE_ == ENABLE)
#if (_VM_TYPE_ == _VM_TYPE_SINGLE_)
#define POOL_SIZE_WAV           SIZE_ALIGN32(0x00080000)    // 512 KB
#else
#define POOL_SIZE_WAV           SIZE_ALIGN32(0x00400000)    // 4 MB
#endif
#else
#define POOL_SIZE_WAV           SIZE_ALIGN32(0)             // no WAV function
#endif

#define POOL_SIZE_UNIFONT       SIZE_ALIGN32(0x00500000)    // Full Unicode font data buffer size
#define POOL_SIZE_PCC_VGA       SIZE_ALIGN32(0x00100000)    // 1MB buffer for VGA PCC usage
#define POOL_SIZE_PCC_HD        SIZE_ALIGN32(0x00200000)    // 2MB buffer for 720P PCC usage

#if (DRAM_END <= 0x00800000)
#define POOL_SIZE_DATE_STAMP    SIZE_ALIGN32(0x51AE0)       // max font size = 82x136, max character number = 10 (ex. 2011/01/01)
                                                            // YUV444, so buffer request = 82x136x3x10 = 334560 (must be 4 alignment)
#else
#define POOL_SIZE_DATE_STAMP    SIZE_ALIGN32(0x9B310)       // max font size = 82x136, max character number = 19 (ex. 2011/01/01 00:00 AM)
                                                            // YUV444, so buffer request = 82x136x3x19 = 602208 (must be 4 alignment)
#endif
#define POOL_SIZE_DATE_STAMP_THUMB  SIZE_ALIGN32(0x2580)    // 10x16x3x20 = 9600
//#define POOL_SIZE_MOVIE_DATE_STAMP  SIZE_ALIGN32(0x38F40)    // 36x60x3x18 x2(double buffer)= 233280//2011/07/28 Meg
//#define POOL_SIZE_MOVIE_DATE_STAMP  SIZE_ALIGN32(0x1C7A0)   //(0x4B00)    // 10x16x3x20 x2(double buffer)= 17280//2012/06/04 Meg
//#define POOL_SIZE_MOVIE_DATE_STAMP  SIZE_ALIGN32(0x155b8)    // 18x30x3x27 x2(double buffer)= 87480
#define POOL_SIZE_MOVIE_DATE_STAMP  SIZE_ALIGN32(0x97E0)    // 12x20x3x27 x2(double buffer)= 87480

#if (_RAM_DISK_ == ENABLE)
    #if (DRAM_END == 0x00800000)
    #define POOL_SIZE_RAM_DISK      SIZE_ALIGN32(512*1024)      // RAM disk buffer size (512KB)
    #else
    #define POOL_SIZE_RAM_DISK      SIZE_ALIGN32(1*1024*1024)   // RAM disk buffer size (1MB)
    #endif
#else
    #define POOL_SIZE_RAM_DISK      SIZE_ALIGN32(0)             // RAM disk buffer size
#endif

#if (_MOVIE_PIM_MODE_ == ENABLE)
#if (_MOVIE_MAX_RES_ == _MOVIE_MAX_RES_1080P_)
#define POOL_SIZE_PIM               SIZE_ALIGN32(400*1024)      // 400KB
#elif (_MOVIE_MAX_RES_ == _MOVIE_MAX_RES_720P_)
#define POOL_SIZE_PIM               SIZE_ALIGN32(200*1024)      // 200KB
#else
#define POOL_SIZE_PIM               SIZE_ALIGN32(100*1024)      // 100KB
#endif
#else
#define POOL_SIZE_PIM               SIZE_ALIGN32(0)
#endif

#if (_MP3_MODE_ == ENABLE)
#define POOL_SIZE_MP3           SIZE_ALIGN32(0x00480000)    // MP3 decode buffer size
#define POOL_SIZE_MP3FONT       SIZE_ALIGN32(0x00500000)    // MP3 unicode font data buffer size
#else
#define POOL_SIZE_MP3           SIZE_ALIGN32(0)             // MP3 decode buffer size
#define POOL_SIZE_MP3FONT       SIZE_ALIGN32(0)             // MP3 unicode font data buffer size
#endif

#define POOL_SIZE_PTP_OBJECT    SIZE_ALIGN32(0x00100000)    // PTP object list buffer (1MB)

#endif
