/**
    Header file of Media Recorder Task

    Exported header file of media recorder task.

    @file       MediaRecAPI.h
    @ingroup    mIAPPMEDIAREC
    @note       add gMediaRecPwrOffPT
    @version    V1.02.008
    @date       2011/10/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MEDIARECAPI_H
#define _MEDIARECAPI_H

#include "MediaWriteLib.h"
#include "MediaReadLib.h"
#include "VideoEncode.h"
#include "ImageAlgInfor.h"//2010/09/06 Meg Lin

/**
    @addtogroup mIAPPMEDIAREC
*/
//@{

/**
    Stop type for MediaRec_Stop()
*/
typedef enum
{
    MEDIAREC_NO_WAIT  = 0x0,  ///< stop recording and no wait
    MEDIAREC_WAIT_END = 0x1,   ///< stop recording and wait this action finished
    ENUM_DUMMY4WORD(MEDIAREC_STOP_TYPE)
} MEDIAREC_STOP_TYPE;

//#NT#2010/05/17#Meg Lin -begin
//#NT#add restriction type
/**
    check type for MediaRec_SetDisk2SecRestriction()
*/
typedef enum
{
    MEDIAREC_RESTR_INDEXBUF = 0x0,  ///< check index buffer to calculate max recording time
    MEDIAREC_RESTR_FSLIMIT  = 0x1,  ///< check fs limitation, 4GB now
    ENUM_DUMMY4WORD(MEDIAREC_RESTR_TYPE)
} MEDIAREC_RESTR_TYPE;
//#NT#2010/05/17#Meg Lin -end

/**
    event type for CallbackOpenDISFunc( )
*/
typedef enum
{
    MEDIAREC_IPLMODE_PREVIEW  = 0x0,   // preview mode , only path1 works
    MEDIAREC_IPLMODE_VIDEO    = 0x1    // recording mode, path1/path2 work
} MEDIAREC_IPLMODE;

/**
    @name media record EVENT ID

    callback ID of media recorder.
*/
//@{
#define     MEDIAREC_EVENT_RESULT_NORMAL        1    ///< Finish normally
#define     MEDIAREC_EVENT_RESULT_HW_ERR        2    ///< Hardware error
#define     MEDIAREC_EVENT_RESULT_FULL          3    ///< Card full
#define     MEDIAREC_EVENT_RESULT_SLOW          4    ///< Write card too slowly
#define     MEDIAREC_EVENT_RESULT_NOTREADY      5    ///< Not ready
#define     MEDIAREC_EVENT_THRESHOLD            6    ///< threshold meets
#define     MEDIAREC_EVENT_RESULT_OVERTIME      7    ///< Overtime
#define     MEDIAREC_EVENT_RESULT_WRITE_ERR    10    ///< filesystem write fails
#define     MEDIAREC_EVENT_STOPPED             11    ///< callback when stops totally
#define     MEDIAREC_EVENT_CUTFILE_END         12    ///< end of one cut file//2011/01/24 Meg Lin
#define     MEDIAREC_EVENT_FILECOMPLETE        13    ///< write file completely//2011/01/27 Meg Lin
#define     MEDIAREC_EVENT_DELETE_DID          14    ///< delete directory ID//2011/02/08 Meg Lin
#define     MEDIAREC_EVENT_DELETE_FID          15    ///< delete file ID//2011/02/08 Meg Lin
#define     MEDIAREC_EVENT_RESULT_CUT_FAIL     19    ///< cut, overlap fail (write slowly)
#define     MEDIAREC_EVENT_STOPTEST            20
#define     MEDIAREC_EVENT_ONEFRAME            21    ///< encode one frame, //2011/07/28 Meg Lin
#define     MEDIAREC_EVENT_START_VID           22
//@}


/**
    @name media record parameter

    Media recording changable parameters.
*/
//@{
#define MEDIAREC_RECPARAM_WIDTH      0x1 ///< width, p1 valid
#define MEDIAREC_RECPARAM_HEIGHT     0x2 ///< height, p1 valid
#define MEDIAREC_RECPARAM_FRAMERATE  0x3 ///< frameRate, p1 valid
#define MEDIAREC_RECPARAM_QUALITY    0x4 ///< quality, p1 valid
#define MEDIAREC_RECPARAM_TARGETRATE 0x5 ///< target rate (bytes per second), p1 valid
//#NT#2009/11/03#Meg Lin -begin
//#NT#Add type MEDIAREC_RECPARAM_CBTHRESHOLD, fixbug 07599
#define MEDIAREC_RECPARAM_CBTHRESHOLD 0x6 ///< callback threshold, p1 valid
//#NT#2009/11/03#Meg Lin -end
#define MEDIAREC_RECPARAM_CUTSECOND  0x7
#define MEDIAREC_RECPARAM_GOLFSHOT_ON 0x8 ///< change recFormat to golfshot, p1 0 or 1
#define MEDIAREC_RECPARAM_CUTSEC     0x9  ///< seamless cut sec  //2010/12/22 Meg Lin
#define MEDIAREC_RECPARAM_CUTSIZE    0xA  ///< seamless cut size //2010/12/23 Meg Lin
#define MEDIAREC_RECPARAM_ENDTYPE    0xB  ///< end type,  //2011/01/24 Meg Lin
#define MEDIAREC_RECPARAM_FLASH_ON   0xC  ///< flash recording on/off, p1 0 or 1 //2011/01/24 Meg Lin
#define MEDIAREC_RECPARAM_PWROFFPT   0xD  ///< enable/disable poweroff_protection, default: on//2011/02/18 Meg Lin
#define MEDIAREC_RECPARAM_GPSON      0xE  ///< enable/disable gps info on, default: off, 2011/06/23

#define MEDIAREC_RECPARAM_FILETYPE   0x87 ///< file type, MEDIAREC_MOV or MEDIAREC_AVI
#define MEDIAREC_RECPARAM_CHG_NOAUD  0x88 ///< rec type, MEDIAREC_VID_ONLY or MEDIAREC_AUD_VID_BOTH//2010/05/25 Meg Lin
//@}

/**
    @name valid video frame rate

    Valid video frame rate.
*/
//@{
#define MEDIAREC_VRATE_2            2  ///< 2 fps, only for MEDIAREC_FAST_FWD
#define MEDIAREC_VRATE_10           10 ///< 10 fps
#define MEDIAREC_VRATE_15           15 ///< 15 fps
#define MEDIAREC_VRATE_20           20 ///< 20 fps
#define MEDIAREC_VRATE_24           24 ///< 24 fps
#define MEDIAREC_VRATE_25           25 ///< 25 fps 2011/12/28 Meg
#define MEDIAREC_VRATE_30           30 ///< 30 fps
#define MEDIAREC_VRATE_50           50 ///< 50 fps 2012/06/06 Meg
#define MEDIAREC_VRATE_60           60 ///< 60 fps, only for 640x480 or smaller
//@}

/**
    @name valid video quality

    Valid video quality.
*/
//@{
#define MEDIAREC_VQUALITY_BEST      1 ///< best
#define MEDIAREC_VQUALITY_NORMAL    2 ///< normal
#define MEDIAREC_VQUALITY_DRAFT     3 ///< draft
//@}


/**
    @name valid file type

    Valid file type.
*/
//@{
#define MEDIAREC_AVI    MEDIA_FILEFORMAT_AVI
#define MEDIAREC_MOV    MEDIA_FILEFORMAT_MOV   ///< .mov file
//#define MEDIAREC_MP4    3
//@}

/**
    @name valid video encode codec.

    Valid video encode codec.
*/
//@{
#define MEDIAREC_ENC_JPG  MEDIAVIDENC_MJPG   ///< motion JPG
#define MEDIAREC_ENC_H264 MEDIAVIDENC_H264   ///< H.264
//@}
/**
    @name valid audio encode codec.

    Valid audio encode codec.
*/
//@{
#define MEDIAREC_ENC_PCM  MOVAUDENC_PCM  ///< PCM
#define MEDIAREC_ENC_AAC  MOVAUDENC_AAC  ///< AAC
//@}

/**
    @name valid video input YCbCr format.

    Valid video input YCbCr format.
*/
//@{
#define MEDIAREC_JPEG_FORMAT_444    1  ///< Y:Cb:Cr = 1:1:1
#define MEDIAREC_JPEG_FORMAT_422    2  ///< Y:Cb:Cr = 2:1:1
#define MEDIAREC_JPEG_FORMAT_420    3  ///< Y:Cb:Cr = 4:1:1
//@}
//#NT#2010/01/25#Meg Lin -begin

/**
    @name valid counting type.

    Valid counting type for MediaRec_Disk2SecondWithType.
*/
//@{
#define MEDIAREC_COUNTTYPE_FS    1  ///< get freespace in storage
#define MEDIAREC_COUNTTYPE_4G    2  ///< using 4G to calculate
#define MEDIAREC_COUNTTYPE_USER  3  ///< using given size to calculate
//@}
//#NT#2010/01/25#Meg Lin -end
/**
    @name valid save type.

    Valid save type. Save data every one second or half second.
*/
//@{
#define MEDIAREC_SAVETYPE_ONE_SEC   1//save data every 1 sec
#define MEDIAREC_SAVETYPE_HALF_SEC  2//save data every 0.5 sec
//@}
/**
    @name valid end type.

    Valid end type. Normal end, or cutOverlap
*/
//@{
#define MEDIAREC_ENDTYPE_NORMAL     1//end when ks2 pressed
#define MEDIAREC_ENDTYPE_CUTOVERLAP 2//cut automatically
#define MEDIAREC_ENDTYPE_CUT_TILLCARDFULL 3//2010/12/23 Meg Lin
#define MEDIAREC_ENDTYPE_MAX              3//2011/01/24 Meg Lin
//@}

/**
    Status type for MediaRec_GetStatus()
*/
typedef enum
{
    MEDIAREC_STATUS_NOT_OPENED        = 0x1,  ///< media recorder task is not open
    MEDIAREC_STATUS_OPENED_NOT_RECORD = 0x2,  ///< media recorder task is open, not recording
    MEDIAREC_STATUS_RECORDING         = 0x3,   ///< media recorder task is recording
    ENUM_DUMMY4WORD(MEDIAREC_STATUS_TYPE)
} MEDIAREC_STATUS_TYPE;


/*
    Public media recording functions
*/
//MediaRecTsk.c
#if 1
extern ER   MediaRec_Open(MEDIAREC_OBJ *param);
extern ER   MediaRec_Close(void);
extern ER   MediaRec_Record(void);
extern ER   MediaRec_Stop(MEDIAREC_STOP_TYPE waitType);
extern UINT32   MediaRec_Disk2Second(void);
extern MEDIAREC_STATUS_TYPE   MediaRec_GetStatus(void);
extern ER   MediaRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);
extern ER   MediaRec_CutOverlapContinue(void);
extern void MediaRec_SetUserData(UINT32 addr, UINT32 size);
extern UINT32 MediaRec_Disk2SecondWithType(UINT32 type, UINT32 size);//2010/01/25 Meg
extern void MediaRec_SetFre1Data(UINT32 addr, UINT32 size);
extern void MediaRec_SetGPSData(UINT32 addr, UINT32 size);//2011/06/23 Meg
extern void MediaRec_SetCrash(void);//2011/06/27 Meg



//MediaVideoRecTsk.c
extern void     MediaRec_SetVideoEncObj(VIDEO_ENCODER *ptr);
extern UINT32   MediaRec_GetNowVideoFrames(void);
extern void     MediaRec_GetActVideoAddr(ACTFBADDR_PRAM *ptr);//2010/09/06 Meg
//extern void     MediaRec_Show_AudVidCount(void);//2011/08/08 Meg


//#NT#2009/12/17#Meg Lin -begin
//MediaAudioRecTsk.c
extern void     MediaRec_Pause(void);
extern void     MediaRec_Resume(void);
//#NT#2009/12/17#Meg Lin -end

extern void     MediaRec_MinusFilesize(UINT32 size);//2010/01/07 Meg
extern UINT32   MediaRec_CheckUnsavedFileBlocks(void);//2010/01/07 Meg
extern void     MediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value);//2010/05/17 Meg Lin
extern ER       MediaRec_ReAllocBuf2VA(UINT32 addr, UINT32 size);//2011/06/20 Meg
extern void     MediaRec_SetTheSameCrashDid(UINT8 on);//2011/09/15 Meg
extern void     MediaRec_SetCrashFolderName(char *pChar);//2011/09/15 Meg
extern ER       MediaRec_StopAndReadOnly(void);//2012/02/17 Meg
extern void     MediaRec_ResetReadOnlyPath(void);//2012/03/01 Meg

#endif
#endif//_MEDIARECAPI_H

//@}
