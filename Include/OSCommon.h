/**
    Header file of OSCommon.c

                Common Define, to sync the defination bewteen Application/Middle layer/Driver,
                we create this header file to put our common define that should be the same
                value in all files.

    @file       OSCommon.h
    @ingroup
    @note       Don't Modify the file.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _OSCOMMON_H
#define _OSCOMMON_H

#include "Type.h"
#include "Kernel.h"
#include "Debug.h"

/**
    @addtogroup
*/
//@{

/**
    @name   Task Configuration Error code

    Definition for API: void OSCommon_CfgModuleTask(PTASKINFO pTaskInfo)
*/
//@{
#define TASKCFG_OK              0X00000000      ///< T_CTSK Task: configure OK.
#define TASKCFG_ERRSTACK        0XFFFFFFFF      ///< T_CTSK Task: stack error.
#define TASKCFG_ERROCCUPIED     0XFFFFFFFE      ///< T_CTSK Task: task id error(duplicate).
#define TASKCFG_ERRFUNEMPTY     0XFFFFFFFD      ///< T_CTSK Task: task body is empty.
#define TASKCFG_ERRID           0XFFFFFFFC      ///< T_CTSK Task: task id overflow.
#define TASKCFG_ERRPRI          0XFFFFFFFB      ///< T_CTSK Task: task priority error
#define TASKCFG_ERRPARA         0XFFFFFFFA      ///< T_CTSK Task: parameter is NULL.
//@}

/**
    Task Configuration Infomation
*/
typedef struct _TaskInfo_
{
    T_CTSK Task;            ///< Task Configuration settings.
    CHAR*  TaskName;        ///< Task name.(for Debuging)

}TASKINFO,*PTASKINFO;

extern void OSCommon_CfgModuleTask(PTASKINFO pTaskInfo);
extern void OSCommon_CfgUserTask(PTASKINFO pTaskInfo);


//For Debuging(Internal using, don't reference it)
// Statck checking tag. If you modify this value, please re-build Debug module
#define STACK_CHECK_TAG         0xDEADBEEF

//#NT#2009/09/29#Elvis Chuang -begin
extern CHAR* tasknametable[];
extern CHAR* flagnametable[];
extern CHAR* semaphorenametable[];
extern CHAR* mailboxnametable[];
extern CHAR* mempoolnametable[];

extern UINT32 guiMaxSTDTaskCount;
extern UINT32 guiMaxSTDFlagCount;
extern UINT32 guiMaxSTDSemaphoreCount;
extern UINT32 guiMaxSTDMailboxCount;
extern UINT32 guiMaxSTDMempoolCount;

extern UINT32 guiMaxTaskCount;
extern UINT32 guiMaxFlagCount;
extern UINT32 guiMaxSemaphoreCount;
extern UINT32 guiMaxMailboxCount;
extern UINT32 guiMaxMempoolCount;
//#NT#2009/09/29#Elvis Chuang -end
//===========================================================================
//
//===========================================================================

#define INT_VEC_NUM             64              // Total number of hardware interrupt sources

//===========================================================================
// Standard Task IDs
//===========================================================================
typedef enum
{
    TASK_ID_00 = 0x00000000,
    TASK_ID_01,
    TASK_ID_02,
    TASK_ID_03,
    TASK_ID_04,
    TASK_ID_05,
    TASK_ID_06,
    TASK_ID_07,
    TASK_ID_08,
    TASK_ID_09,
    TASK_ID_10,
    TASK_ID_11,
    TASK_ID_12,
    TASK_ID_13,
    TASK_ID_14,
    TASK_ID_15,
    TASK_ID_16,
    TASK_ID_17,
    TASK_ID_18,
    TASK_ID_19,
    TASK_ID_20,
    TASK_ID_21,
    TASK_ID_22,
    TASK_ID_23,
    TASK_ID_24,
    TASK_ID_25,
    TASK_ID_26,
    TASK_ID_27,
    TASK_ID_28,
    TASK_ID_29,
    TASK_ID_30,
    TASK_ID_31,
    TASK_ID_32,
    TASK_ID_33,
    TASK_ID_34,
    TASK_ID_35,
    TASK_ID_36,
    TASK_ID_37,
    TASK_ID_38,
    TASK_ID_39,
//#NT#2010/05/14#Janice Huang    --begin
//#NT#Add fs background task
    TASK_ID_40,
//#NT#2010/05/14#Janice Huang    --end
//#NT#2011/02/16#JustinLee -begin
//#NT#Add lens control task
    TASK_ID_41,
//#NT#2011/02/16#JustinLee -end
//#NT#2011/02/09#ethanlau -begin
//#NT#add phototsk0 for IPL autoslice mode
    TASK_ID_42,
//#NT#2011/02/09#ethanlau -end
    TASK_ID_43,
//#NT#2011/10/18#Jarkko Chang -begin 
//#NT#add for AF task(sync. 450)
    TASK_ID_44,
    TASK_ID_45,
//#NT#2011/10/18#Jarkko Chang -end
//#NT#2011/11/9#Philex Lin -begin
// for FW rotation task
    TASK_ID_46,
//#NT#2011/11/9#Philex Lin -end
    STD_TSK_NUM,                                // Total number of Standard Task
    ENUM_DUMMY4WORD(STD_TASK)
} STD_TASK;

// Mandatory Tasks
#define INITSK_ID               TASK_ID_01      // Initialize System
#define FILESYSTSK_ID           TASK_ID_02      // File system
#define PRIMARYTSK_ID           TASK_ID_03      // Main flow control

// Playback
#define PLAYBAKTSK_ID           TASK_ID_04      // Playback

// IQ
#define PHOTO1STTSK_ID          TASK_ID_05      // Photo mode
#define PHOTO2NDTSK_ID          TASK_ID_06
#define PHOTO3RDTSK_ID          TASK_ID_07
#define IMGSLICETSK_ID          TASK_ID_08      // IPE to JPG slice
#define SIECAPTSK_ID            TASK_ID_09      // SIE raw image capture

// USB
#define USBCTRLTSK_ID           TASK_ID_10      // usb control start
#define USBSETUPTSK_ID          TASK_ID_11      // usb setup handle
#define USBSTRGTSK_ID           TASK_ID_12      // USB MSDC mode
#define USBPCCVIDEOTSK_ID       TASK_ID_13      // usb PC camera video task
#define USBSICDTSK_ID           TASK_ID_14      // for USB Still Image Class.
#define USBSICDEP3TSK_ID        TASK_ID_15

// AVI
#define AVIRECMANAGERTSK_ID     TASK_ID_16      // AVI and WAV record/playback
//#define AVIPLAYMANAGERTSK_ID    TASK_ID_17
//#define PLAYAUDIOTSK_ID         TASK_ID_18
//#define PLAYVIDEOTSK_ID         TASK_ID_19
//#NT#2009/11/02#Photon Lin -begin
//#Add Display task for play usage
//#define PLAYVIDEOFRMTSK_ID      TASK_ID_20
#define RECVIDEOTSK_ID          TASK_ID_21

// MP3
#define MP3PLAYTSK_ID           TASK_ID_22
#define MP3PLAYFILETSK_ID       TASK_ID_23      // MP3 play file read task

// WAV
#define WAVSTUDIOTSK_ID         TASK_ID_24      // WAV record/playback task

// Media Framework
#define MEDIAPLAYTSK_ID         TASK_ID_26      // MJPG MOV Dec mode
#define MEDIAVIDEOPLAYTSK_ID    TASK_ID_28      // MJPG Video decode task
#define MEDIAAUDIOPLAYTSK_ID    TASK_ID_29      // MJPG Audio decode task
#define MEDIARECTSK_ID          TASK_ID_25
#define MEDIAVIDEORECTSK_ID     TASK_ID_27
#define MEDIARECFSTSK_ID        TASK_ID_17
#define MEDIARECCMDTSK_ID       TASK_ID_18
#define MEDIAAUDIOENCTSK_ID     TASK_ID_19
// MJPG MOV
#define MOVMJPGRECTSK_ID        TASK_ID_30      // MJPG MOV Enc mode
#define MOVMJPGPLAYTSK_ID       TASK_ID_31      // MJPG MOV Dec mode
#define MOVMJPGVIDEORECTSK_ID   TASK_ID_32      // MJPG Video encode task
#define MOVMJPGVIDEOPLAYTSK_ID  TASK_ID_33      // MJPG Video decode task
#define MOVMJPGAUDIOPLAYTSK_ID  TASK_ID_34      // MJPG Audio decode task

// GPS
#define GPSRECEIVE_ID           TASK_ID_35

// IQ
#define PHOTODISPLAYTSK_ID      TASK_ID_36      // Photo mode

//#NT#2011/08/16#Ben Wang -begin
//#NT#Remove Event View
// Event View
//#define EVENTVIEW_ID            TASK_ID_37      // Event view task
//#define EVENTVIEWSHOW_ID        TASK_ID_38      // Event view showing task
#define RAWENC_ID               TASK_ID_37      //2011/08/15 Meg

// DIS
#define PHOTODISTSK_ID          TASK_ID_38      // DIS mode
//#NT#2009/11/02#Photon Lin -end
//#NT#2010/05/14#Janice Huang    --begin
//#NT#Add fs background task
#define FSBACKGROUNDTSK_ID        TASK_ID_39
//#NT#2010/05/14#Janice Huang    --end

//#NT#2011/02/16#JustinLee -begin
//#NT#Add lens control task
// Lens Control
#define LENSCTRLTSK_ID          TASK_ID_40
//#NT#2011/02/16#JustinLee -end
#define AETSK_ID                TASK_ID_41
#define AWBTSK_ID               TASK_ID_42
//#NT#2011/08/16#Ben Wang -end

//#NT#2011/09/14#Klins Chen -begin
// Display Rotation
#define DISPROTATE_ID           TASK_ID_43
//#NT#2011/09/14#Klins Chen -end
//#NT#2011/10/18#Jarkko Chang -begin 
//#NT#add for AF task(sync. 450)
// AF
#define AFTSK_ID                TASK_ID_44
#define AFTSK2_ID               TASK_ID_45
//#NT#2011/10/18#Jarkko Chang -end

//#NT#2011/11/9#Philex Lin -begin
// add FW rotation task
#define FW_ROTATETSK_ID         TASK_ID_46
//#NT#2011/11/9#Philex Lin -end

//===========================================================================
// Standard Task Priority
// User defined tasks Priority are declared in SysCfg.h
// Lower value higher priority
//===========================================================================
// Mandatory Tasks
#define PRI_INIT                 2
#define PRI_FILESYS             10
#define PRI_PRIMARY              4

// Playback
#define PRI_PLAYBACK            12

// IQ
#define PRI_1STPHOTO             3
#define PRI_2NDPHOTO            17
#define PRI_3RDPHOTO            18
#define PRI_IMGSLICE             6
#define PRI_SIECAP               4
#define PRI_DISPLAYPHOTO         5//tmp for Disptsk un-realtime
#define PRI_DISPHOTO             8
#define PRI_AE                  15
#define PRI_AWB                 16
//#NT#2011/10/18#Jarkko Chang -begin 
//#NT#add for AF task
#define PRI_AFTSK                3
#define PRI_AFTSK2               3
//#NT#2011/10/18#Jarkko Chang -end
// USB
#define PRI_USBCTRL             12
#define PRI_USBSETUP            10
#define PRI_USBSTRG              8
#define PRI_USBPCCVIDEO         PRI_USBSETUP
#define PRI_USBSICD             10
#define PRI_USBSICDEP3           8

// AVI
//#NT#2012/07/05#KS Hung -begin
//#NT#It must be higher than UIFrameworkTsk, because drawing zoom bar will affect 1080P recording for HDV510_project.
#define PRI_AVIRECMANAGER        11
//#define PRI_AVIRECMANAGER        12
//#NT#2012/07/05#KS Hung -end
//#define PRI_AVIPLAYMANAGER      18
//#define PRI_PLAYAUDIO            6
//#define PRI_PLAYVIDEO            8
//#NT#2009/11/02#Photon Lin -begin
//#Add Display task for play usage
//#define PRI_PLAYVIDEOFRM         8
//#NT#2009/11/02#Photon Lin -end
#define PRI_RECVIDEO             8

// MP3
#define PRI_MP3PLAY             10
#define PRI_MP3PLAYFILE         12

// WAV
#define PRI_WAVSTUDIO            8

// Media Framework
//#define PRI_MEDIAREC            12
#define PRI_MEDIAPLAY           18
//#define PRI_MEDIAVIDEOREC        8
#define PRI_MEDIAVIDEOPLAY       8
#define PRI_MEDIAAUDIOPLAY       6

#define PRI_MEDIAREC            8 //12 //2010/12/15 Meg Lin
#define PRI_MEDIAVIDEOREC       6 //8  //2010/12/15 Meg Lin
#define PRI_MEDIAAUDIOENC       7 //9  //2010/12/15 Meg Lin
#define PRI_MEDIARECFS          9 //2010/12/15 Meg Lin
#define PRI_MEDIARECCMD         7 //2010/12/15 Meg Lin



// Mov MJPG
#define PRI_MOVMJPGREC          12
#define PRI_MOVMJPGPLAY         18
#define PRI_MOVMJPGAUDIOPLAY     6
#define PRI_MOVMJPGVIDEOPLAY     8
#define PRI_MOVMJPGVIDEOREC      8

// GPS
#define PRI_GPSRECEIVE           4//6

//#NT#2011/08/16#Ben Wang -begin
//#NT#Remove Event View
// Event View
//#define PRI_EVENTVIEW           12
//#define PRI_EVENTVIEWSHOW        6
//#NT#2011/08/16#Ben Wang -end
//#NT#2010/05/14#Janice Huang -begin
//#NT#Add fs background task
#define PRI_FSBACKGROUND            3
//#NT#2010/05/14#Janice Huang -end

//#NT#2011/02/16#JustinLee -begin
//#NT#Add lens control task
// Lens Control
#define PRI_LENSCTRL             1
//#NT#2011/02/16#JustinLee -end
#define PRI_RAWENC             18 //2011/08/15 Meg

//#NT#2011/09/14#Klins Chen -begin
// Display Rotation
#define PRI_DISPROTATE           4
//#NT#2011/09/14#Klins Chen -end

//#NT#2011/10/26#Philex  -begin
#define PRI_FW_ROTATE           4
//#NT#2011/10/26#Philex  -end


//===========================================================================
// Standard Task Stack size
//===========================================================================
#define STKSIZE_DUMMY              4

// Mandatory Task
#define STKSIZE_INIT            2048
#define STKSIZE_FILESYS         4096
#define STKSIZE_PRIMARY         4096

// Playback
#define STKSIZE_PLAYBACK        4096

// IQ
#define STKSIZE_1STPHOTO        1024
//#NT#2009/09/28#Connie Yu -begin
//#for FW Nvt Smear Reduction
#define STKSIZE_2NDPHOTO        2048 //1024
//#NT#2009/09/28#Connie Yu -end
#define STKSIZE_3RDPHOTO        4096
//#NT#2010/04/12#Niven Cho -begin
//#NT#PORTING_DATE_IMPRINT
#define STKSIZE_IMGSLICETSK     4096
//#NT#2010/04/12#Niven Cho -end
//#NT#2010/05/07#Niven Cho -begin
//#NT#2048->4096 for Custom Request
#define STKSIZE_SIECAPTSK       4096
//#NT#2010/05/07#Niven Cho -end
#define STKSIZE_DISPLAYPHOTO    2048
#define STKSIZE_DISPHOTO        1024
#define STKSIZE_AE              4096    //ethan-20110725_fix stack overflow when dzoom
#define STKSIZE_AWB             2048
//#NT#2011/10/18#Jarkko Chang -begin 
//#NT#add for AF task(sync. 450)
#define STKSIZE_AFTSK           1024
#define STKSIZE_AFTSK2          1024
//#NT#2011/10/18#Jarkko Chang -end
// USB
#define STKSIZE_USBCTRL         1024
#define STKSIZE_USBSETUP        2048
#define STKSIZE_USBSTRGVND      1024
#define STKSIZE_USBPCCAMVIDEO   2048
#define STKSIZE_USBSICDVND      2048
#define STKSIZE_USBSICDEP3      2048

// AVI
#define STKSIZE_AVIRECMANAGER   4096//2011/05/13 Meg
//#define STKSIZE_AVIPLAYMANAGER  1024
//#define STKSIZE_PLAYAUDIO       1024
//#define STKSIZE_PLAYVIDEO       1024
//#NT#2009/11/02#Photon Lin -begin
//#Add Display task for play usage
//#NT#2010/01/18#Niven Cho -begin
//#NT#Modified., Task use UtImageScaleByIme needs to more stack size
//#define STKSIZE_PLAYVIDEOFRM    2048
//#NT#2010/01/18#Niven Cho -end
//#NT#2009/11/02#Photon Lin -end
#define STKSIZE_RECAUDIO        1024
//#NT#2012/08/14#KS Hung -begin
//#NT#If it has zoom in/out action during recording for HDCK project, it will cover this stack area.
//#NT#sie_isr (sie_api.c) will occupy the stack area of current task and it will a series of function calling.
//#define STKSIZE_RECVIDEO        1024
#define STKSIZE_RECVIDEO        2048
//#NT#2012/08/14#KS Hung -end

// MP3
#define STKSIZE_MP3PLAY         8192
#define STKSIZE_MP3PLAYFILESYS  2048

// WAV
#define STKSIZE_WAVSTUDIO       1024

// Media Framework
//#define STKSIZE_MEDIAREC        2048
#define STKSIZE_MEDIAPLAY       2048
//#define STKSIZE_MEDIAVIDEOREC   1024
#define STKSIZE_MEDIAVIDEOPLAY  8192 //2011/04/06 Meg Lin
#define STKSIZE_MEDIAAUDIOPLAY  1024

#define STKSIZE_MEDIAREC            8196
#define STKSIZE_MEDIAVIDEOREC       2048//1024
#define STKSIZE_MEDIAAUDIOENC       8196
#define STKSIZE_MEDIARECFS          4096 //2010/12/15 Meg Lin
#define STKSIZE_MEDIARECCMD         2048 //2010/12/15 Meg Lin

// Mov MJPG
#define STKSIZE_MOVMJPGREC          2048
#define STKSIZE_MOVMJPGPLAY         2048
#define STKSIZE_MOVMJPGVIDEOREC     1024
#define STKSIZE_MOVMJPGAUDIOPLAY    1024
#define STKSIZE_MOVMJPGVIDEOPLAY    1024

// GPS
#define STKSIZE_GPSRECEIVE      2048//1024

//#NT#2011/08/16#Ben Wang -begin
//#NT#Remove Event View
// Event View
//#define STKSIZE_EVENTVIEW       2048
//#define STKSIZE_EVENTVIEWSHOW   2048
//#NT#2011/08/16#Ben Wang -end

//#NT#2010/05/14#Janice Huang -begin
//#NT#Add fs background task
#define STKSIZE_FSBACKGROUND     1024
//#NT#2010/05/14#Janice Huang -end

//#NT#2011/02/16#JustinLee -begin
//#NT#Add lens control task
// Lens Control
#define STKSIZE_LENSCTRL        1024
//#NT#2011/02/16#JustinLee -end
#define STKSIZE_RAWENC          4096 //2011/08/15 Meg

//#NT#2011/09/14#Klins Chen -begin
// Display Rotation
#define STKSIZE_DISPROTATE      1024
//#NT#2011/09/14#Klins Chen -end

//#NT#2011/10/26#Philex Lin -begin
#define STKSIZE_FW_ROTATE        1024
//#NT#2011/10/26#Philex Lin -end

//===========================================================================
// Standard Flag ID value symbols
// User defined flag IDs are declared in SysCfg.h
//===========================================================================
typedef enum
{
    FLG_ID_00 = 0x00000000,
    FLG_ID_01,
    FLG_ID_02,
    FLG_ID_03,
    FLG_ID_04,
    FLG_ID_05,
    FLG_ID_06,
    FLG_ID_07,
    FLG_ID_08,
    FLG_ID_09,
    FLG_ID_10,
    FLG_ID_11,
    FLG_ID_12,
    FLG_ID_13,
    FLG_ID_14,
    FLG_ID_15,
    FLG_ID_16,
    FLG_ID_17,
    FLG_ID_18,
    FLG_ID_19,
    FLG_ID_20,
    FLG_ID_21,
    FLG_ID_22,
    FLG_ID_23,
    FLG_ID_24,
    FLG_ID_25,
//#NT#2010/05/14#Janice Huang   --begin
//#NT#Add fs background task
    FLG_ID_26,
//#NT#2010/05/14#Janice Huang     --end
//#NT#2011/02/16#JustinLee -begin
//#NT#Add lens control task
    FLG_ID_27,
    //#NT#2011/09/14#Klins Chen -begin
    FLG_ID_28,
    //#NT#2011/09/14#Klins Chen -end
//#NT#2011/02/16#JustinLee -end
//#NT#2011/08/16#Ben Wang -begin
//#NT#Remove Event View
//    FLG_ID_28,
//#NT#2011/07/13#Chris Hsu -begin
//#NT#For system driver 1.00.006
//    FLG_ID_29,
//#NT#2011/07/13#Chris Hsu -end
//#NT#2011/08/16#Ben Wang -end
//#NT#2011/10/18#Jarkko Chang -begin 
//#NT#add for AF task(sync. 450)
    FLG_ID_29,
//#NT#2011/10/18#Jarkko Chang -end
//#NT#2011/11/9#Philex Lin -begin
//#NT#add for FW rotation flag
    FLG_ID_30,
//#NT#2011/11/9#Philex Lin -end
    STD_EVF_NUM,                                // Total number of Standard Flag
    ENUM_DUMMY4WORD(STD_FLAG)
} STD_FLAG;

//-----------------------------------------------------------------------------
// Standard flag IDs flags
//-----------------------------------------------------------------------------
#define FLG_ID_INT              FLG_ID_00       // All interrupts
#define FLG_ID_GPIO             FLG_ID_01       // GPIO Interupt
#define FLG_ID_FILESYS          FLG_ID_02       // File system task
#define FLG_ID_PSTORE           FLG_ID_03       // PStore
#define FLG_ID_STIMER           FLG_ID_04       // Soft timer
#define FLG_ID_PHOTO            FLG_ID_05       // For photo task
#define FLG_ID_SLICE            FLG_ID_06       // For slice mode
#define FLG_ID_PLAYBACK         FLG_ID_07       // Playback task
#define FLG_ID_AVI              FLG_ID_08       // AVI
#define FLG_ID_AVI_VID          FLG_ID_09       // AVI video task
//#define FLG_ID_AVI_AUD          FLG_ID_10       // AVI audio task //2011/08/15 Meg
#define FLG_ID_RAWENC           FLG_ID_10       // Raw encode task //2011/08/15 Meg
#define FLG_ID_MEDIA            FLG_ID_11       // Media Framework
#define FLG_ID_RECMEDIA         FLG_ID_12       // Media Framework
#define FLG_ID_USBTSK           FLG_ID_13       // USB INT
#define FLG_ID_DPS              FLG_ID_14       // For DPS task
#define FLG_ID_PRIMARY          FLG_ID_15       // Primary task
#define FLG_ID_WAV              FLG_ID_16       // Wave studio task
#define FLG_ID_MP3              FLG_ID_17       // MP3
#define FLG_ID_GPS              FLG_ID_18       // GPS
#define FLG_ID_INT2             FLG_ID_19       // Interrupt 2
#define FLG_ID_IPL              FLG_ID_20       // image pipeline driver
#define FLG_ID_USBCLASS         FLG_ID_21       // USB device class (SICD/MSDC/PCC)
#define FLG_ID_PHOTODISPLAY     FLG_ID_22       // For photo display task
//#NT#2011/08/16#Ben Wang -begin
//#NT#Remove Event View
//#define FLG_ID_EVENTVIEW        FLG_ID_23       // Event View
//#define FLG_ID_EVENTVIEWSHOW    FLG_ID_24       // Event View Show
#define FLG_ID_PHOTODIS         FLG_ID_23       // For photo digital image stabilizer task
//#NT#2010/05/14#Janice Huang   --begin
//#NT#Add fs background task
#define FLG_ID_FSBACKGROUND     FLG_ID_24
//#NT#2010/05/14#Janice Huang     --end
//#NT#2011/02/16#JustinLee -begin
//#NT#Add lens control task
#define FLG_ID_LENS             FLG_ID_25       // Lens Control task
//#NT#2011/02/16#JustinLee -end
#define FLG_ID_EP3TSK           FLG_ID_26       // For SICD
//#NT#2011/07/13#Chris Hsu -begin
//#NT#For system driver 1.00.006
#define FLG_ID_GPIO2            FLG_ID_27
//#NT#2011/07/13#Chris Hsu -end
//#NT#2011/08/16#Ben Wang -end
//#NT#2011/09/14#Klins Chen -begin
#define FLG_ID_DISPROTATE       FLG_ID_28       // Display Rotation
//#NT#2011/09/14#Klins Chen -end
//#NT#2011/10/18#Jarkko Chang -begin 
//#NT#add for AF task(sync. 450)
#define FLG_ID_AF               FLG_ID_29       // AF task
//#NT#2011/10/18#Jarkko Chang -end

//#NT#2011/10/26#Philex Lin -begin
#define FLG_ID_FWRT             FLG_ID_30     // LCD FW rotate
//#NT#2011/10/26#Philex Lin -end

//===========================================================================
//  Standard Semaphore IDs
//===========================================================================
typedef enum
{
    SEMID_JPEG = 0x00000000,                    // 0
    SEMID_AMBA_DMA,
    SEMID_SIF,
    SEMID_I2C,
    SEMID_SPI,
    SEMID_IDE,                                  // 5
    SEMID_HDMI,
    SEMID_DAI,
    SEMID_SDIO,
    SEMID_XDNAND,
    SEMID_USB,                                  // 10
    SEMID_UART,
    SEMID_UART2,
    SEMID_RTC,
    SEMID_TIMER,
    SEMID_WDT,                                  // 15
    SEMID_PWM_0,
    SEMID_PWM_1,
    SEMID_PWM_2,
    SEMID_PWM_CCNT,
    SEMID_ADC_0,                                // 20
    SEMID_ADC_1,
    SEMID_ADC_2,
    SEMID_GRAPHIC,
    SEMID_SIE,
    SEMID_IPE,                                  // 25
    SEMID_IME,
    SEMID_IFE,
    SEMID_KNLTIMER,
    SEMID_MI,
    SEMID_SCANTG,                               // 30
    SEMID_SRA,
    //#NT#2011/06/23#HH Chuang -begin
    //#NT#add new top architecture
    SEMID_TOP_STRG,
    //#NT#2011/06/23#HH Chuang -end
//#NT#2009/12/11#Bowen Li -begin
//#NT#add Semaphore for IPL
    SEMID_IPL,
//#NT#2009/12/11#Bowen Li -end
//#NT#2010/05/14#Janice Huang    --begin
//#NT#Add fs background task
    SEMID_FSBACKGROUND,
//#NT#2010/05/14#Janice Huang     --end
    SEMID_SICD,
    SEMID_SENSOR,
    SEMID_CALIBRATION,
    //#NT#2011/09/14#Klins Chen -begin
    SEMID_DISPROT,
    //#NT#2011/09/14#Klins Chen -end
    STD_SEM_NUM,                                // Total number of Standard Semaphore
    ENUM_DUMMY4WORD(STD_SEM)
} STD_SEM;


//Backward Compatible
#define SEMID_PWM_3    SEMID_PWM_2
#define SEMID_PWM_4    SEMID_PWM_2
#define SEMID_PWM_5    SEMID_PWM_2
#define SEMID_PWM_6    SEMID_PWM_2
#define SEMID_PWM_7    SEMID_PWM_2
#define SEMID_PWM_8    SEMID_PWM_2
#define SEMID_PWM_9    SEMID_PWM_2
#define SEMID_PWM_10   SEMID_PWM_2
#define SEMID_PWM_11   SEMID_PWM_2




//===========================================================================
//  Standard Mailbox IDs
//===========================================================================
typedef enum
{
    MBXID_UART_RX = 0x00000000,
    MBXID_UART_TX,

    STD_MBX_NUM,                                // Total number of Standard Mailbox
    ENUM_DUMMY4WORD(STD_MBX)
} STD_MBX;

#define BUF_NUM                 255


//===========================================================================
//  Fixed size memory pool IDs, Sizes and Counts
//===========================================================================
typedef enum
{
    POOL_ID_FS_SECTOR = 0x00000000,
    POOL_ID_FS_FAT,
    POOL_ID_STORAGE_SMC,

    STD_FPL_NUM,                                // Total number of Fixed size memory pool
    ENUM_DUMMY4WORD(STD_MPOOL)
} STD_MPOOL;

//#NT#2009/11/04#Chris Hsu -begin
//#NT#Use macro to make sure pool size is 32 bytes alignment
// Note: Pool size must be 32 bytes alignment
//#NT#2012/09/18#Nestor Yang -begin
//#NT#Enlarge POOL_SIZE_FS_SECTOR size to support windows 64KB cluster format
#define POOL_SIZE_FS_SECTOR     SIZE_ALIGN32(0x10020)    // 65,568 bytes (64KB + 32 for 32 bytes align)
//#NT#2012/09/18#Nestor Yang -end
//#NT#2010/05/14#Janice Huang -begin
//for file sys int multi task
//#NT#2012/10/01#Lincy Lin -begin
//#NT#Suppot FAT32 BitMap
//#define POOL_SIZE_FS_FAT        SIZE_ALIGN32(0x80020)   // 131,104 bytes (131072+8+24, 24 for 32 bytes align)
#define POOL_SIZE_FS_FAT        SIZE_ALIGN32(0x60020)   // (256KB for FAT, 128KB for FAT bitMap)
//#NT#2012/10/01#Lincy Lin -end
//#NT#2010/05/14#Janice Huang -end
#define POOL_SIZE_STORAGE_SMC   SIZE_ALIGN32(0x4000)    // 16384 bytes
//#NT#2009/11/04#Chris Hsu -end

#define POOL_SIZE_COMMON        ((POOL_SIZE_FS_SECTOR * POOL_CNT_FS_SECTOR) +\
                                 (POOL_SIZE_FS_FAT * POOL_CNT_FS_FAT) +\
                                 (POOL_SIZE_STORAGE_SMC * POOL_CNT_STORAGE_SMC))

#define POOL_CNT_FS_SECTOR      2
#define POOL_CNT_FS_FAT         1
//#NT#2010/02/03#Brad Chen -begin
//#NT# set SMC Buffer from 1 -> 2 first is for nand driver,second is for Pstore
#define POOL_CNT_STORAGE_SMC    2
//#NT#2010/02/03#Brad Chen -end


//===========================================================================
// Modified uITRON RTOS data structure
//===========================================================================
//#NT#2009/09/29#Elvis Chuang -begin
typedef struct _SYSTEM_EVF_INFO_
{
    UB          evfid;                          ///< Event Flag ID
    //UINT        value;                          ///< Event Flag Value
    CHAR*       EventName;                      ///< Event name.(for Debuging)
} SYSTEM_EVF_TABLE;

typedef struct _SYSTEM_SEM_INFO_
{
    T_CSEM      Semphore;                       ///< Semphore information.
    CHAR*       SemphoreName;                   ///< Semphore name.(for Debuging)
}SYSTEM_SEM_TABLE;

typedef struct _SYSTEM_MBX_INFO_
{
    UB          mbxid;                          ///< Mail Box ID
    CHAR*       MailboxName;                    ///< Maibox name.(for Debuging)
} SYSTEM_MBX_TABLE;

typedef struct _SYSTEM_MEMPOOL_INFO_
{
    T_CMPF      MemPool;                        ///< Memory pool information
    CHAR*       MempoolName;                    ///< Memory pool name.(for Debuging)
} SYSTEM_MEMPOOL_TABLE;
//#NT#2009/09/29#Elvis Chuang -end

extern UCHAR pucStkDummy[];
extern void DummyTsk(void);

//@}

#endif
