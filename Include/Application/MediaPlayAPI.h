/**
    Header file of Media Player Task

    Exported header file of media Player task.

    @file       MediaPlayAPI.h
    @ingroup    mIAPPMEDIAPLAY
    @note       004->005, modify videoRawBuffer for TINY mode
    @version    V1.0a.005
    @date       2011/03/31

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MEDIAPLAYAPI_H
#define _MEDIAPLAYAPI_H


#include "Audio.h"
#include "FileDB.h"
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"
#include "OSCommon.h"
#include "Interrupt.h"
#include "Clock.h"
#include "FilesysTsk.h"
#include "Debug.h"
#include "ide.h"
#include "AppCommon.h"
#include "VideoDecode.h"
#include "MediaReadLib.h"

/**
    @addtogroup mIAPPMEDIAPLAY
*/
//@{

/**
    @name status of media player

    status of media player
*/
//@{
#define MEDIAPLAY_STATUS_PAUSE        1
#define MEDIAPLAY_STATUS_OTHER        2
#define MEDIAPLAY_STATUS_PLAYING      3
#define MEDIAPLAY_STATUS_STOPPED      4
//@}

/**
    @name media playing EVENT ID

    callback ID of media player.
*/
//@{
#define MEDIAPLAY_EVENT_FINISH            1   ///< Finish
#define MEDIAPLAY_EVENT_ONE_SECOND        2   ///< One second arrives
#define MEDIAPLAY_EVENT_ONE_VIDEOFRAME    3   ///< Decode one video-frame
#define MEDIAPLAY_EVENT_START             4   ///< Start mjpgMov playback
#define MEDIAPLAY_EVENT_STOPATEND         5
//#NT#2009/12/01#Meg Lin# -begin
//#NT#fixbug: decode error, end
#define MEDIAPLAY_EVENT_VID_ERROR         6   ///< decode video frame error
//#NT#2009/12/01#Meg Lin# -end
//@}

/**
    @name type of ide display format

    type of ide display format
*/
//@{
#define MEDIAPLAY_VID_422               1
#define MEDIAPLAY_VID_420_UVPACK        2
#define MEDIAPLAY_VID_444               3
#define MEDIAPLAY_VID_420               4
//@}

#define MEDIAPLAY_DEF_MAXWIDTH4PANEL    1280//2011/08/08 Meg

/**
    Speed up type for MediaPlay_SpeedUp()
*/
typedef enum
{
    SPEED_NORMAL    = 0x0,      ///< normal speed
    SPEED_H264_15X  = 0x21,      ///< h264 i+14p, audio%2  trigger 15/1 = 7.5X //2010/05/25 Meg Lin
    SPEED_H264_7_5X = 0x22,      ///< h264 i+14p, audio%2  trigger 15/2 = 7.5X
    SPEED_H264_5X   = 0x23,      ///< h264 i+14p, audio%3  trigger 15/3 = 5X
    SPEED_H264_3_75X = 0x24,     ///< h264 i+14p, audio%4  trigger 15/4 = 3.75X
    SPEED_H264_3X   = 0x25,      ///< h264 i+14p, audio%5  trigger 15/5 = 3X
    SPEED_H264_2_5X = 0x26,      ///< h264 i+14p, audio%6  trigger 15/6 = 2.5X
    SPEED_H264_1_5X = 0x2a,      ///< h264 i+14p, audio%10 trigger 15/10= 1.5X
    SPEED_MJPG_30X  = 0x1E,      /// 0x80 + 30  //2011/07/20 Meg
    SPEED_MJPG_60X  = 0x3C,      /// 0x80 + 60
    SPEED_MJPG_90X  = 0x5A,      /// 0x80 + 90
    ENUM_DUMMY4WORD(MEDIAPLAY_SPEEDUP_TYPE)
} MEDIAPLAY_SPEEDUP_TYPE;

//#NT#2010/05/17#Meg Lin -begin
//#NT#add for getting fileinfo
/**
    fileinfo type for MediaPlay_GetFileInfo()
*/
typedef enum
{
    MEDIA_INFO_WIDTH = 0x0,     ///< video width
    MEDIA_INFO_HEIGHT = 0x1,    ///< video height
    MEDIA_INFO_FILETYPE = 0x2,  ///< container type, MEDIA_FILEFORMAT_MOV or others
    MEDIA_INFO_DURATION = 0x3,  ///< playing time
    MEDIA_INFO_AUDSR = 0x4,     ///< audio sample rate
    MEDIA_INFO_VIDTYPE = 0x5,   ///< video codec type, MEDIAPLAY_VIDEO_H264 or others
    MEDIA_INFO_AUDTYPE = 0x6,   ///< audio codec type, MEDIAAUDIO_CODEC_SOWT or others
    MEDIA_INFO_VIDFR = 0x7,     ///< video frame rate
    MEDIA_INFO_AUDCHS = 0x8,    ///< audio channels
    MEDIA_INFO_FRE1_POS = 0x9,  ///< free data 1 position //2010/09/17 Meg Lin
    MEDIA_INFO_FRE1_SIZE = 0xA, ///< free data 1 size  //2010/09/17 Meg Lin
    ENUM_DUMMY4WORD(MEDIAPLAY_FILEINFO)
} MEDIAPLAY_FILEINFO;
//#NT#2010/05/17#Meg Lin -end

typedef struct _MEDIAPLAY_PANELINFO
{
    UINT32 y1Addr;// display buffer
    UINT32 cb1Addr;//
    UINT32 cr1Addr;//
    UINT32 y2Addr;// temp display buffer
    UINT32 cb2Addr;//
    UINT32 cr2Addr;//

} MEDIAPLAY_PANELINFO;

/**
    Struture of video encoder
*/
typedef struct _MEDIAPLAY_OBJ {
    UINT32      uiMemAddr;                          ///< Data buffer starting address
    UINT32      uiMemSize;                          ///< Data buffer size
    void        (*CallBackFunc)(UINT32 event_id);   ///< Callback function
    MEDIAPLAY_PANELINFO     panelInfo;
    UINT32      iAudioType;                         ///< AUDIO_OUTPUT_HP or other
    UINT32      u32PanelW;                          ///< in: panel width
    UINT32      u32PanelH;                          ///< in: panel height
    UINT32      u32decodeMode;                      ///< in: decode mode, MEMORY_TINY or MEMORY_NORMAL
    UINT16      u16OutWidth;                        ///< display width, MediaPlay_Play() can change this
    UINT16      u16OutHeight;                       ///< display height, MediaPlay_Play() can change this
    UINT16      u16OutWratio;                       ///< width ratio, MediaPlay_Play() can change this
    UINT16      u16OutHratio;                       ///< height ratio, MediaPlay_Play() can change this
    UINT16      u16MaxIDEWidth;                     ///< max IDE window width
    UINT16      u16MaxIDEHeight;                    ///< max IDE window height
    UINT16      u16IDEFormat;                       ///< IDE format, MEDIAPLAY_IDE_422
    UINT16      rev;
    UINT16      u16MaxRawWidth;                     ///< max video resolution width
    UINT16      u16MaxRawHeight;                    ///< max video resolution height
    UINT32      u32MaxHDMIWidth;                    ///< max hdmi width for VideoPlay, dram8M is 640, 2011/06/07 Meg
    UINT32      u32MaxHDMIHeight;                   ///< max hdmi height for VideoPlay, dram8M is 720, 2011/06/07 Meg
    UINT32      u32MaxWidth4Panel;                  ///< max raw width for panel (no jpg decode down), default: 1280, 2011/08/08 Meg
    BOOL        bHWAudio;                           ///< Hardware audio support : TRUE or FALSE
    BOOL        bPlayH264;                          ///< if true(H.264 supported), need more memory

} MEDIAPLAY_OBJ;







/**
    Struture of memory setting to get header information
*/
typedef struct _MEDIA_1STV
{
    UINT32 pos;
    UINT32 size;
    UINT16 wid;
    UINT16 hei;
    UINT32 type;//after probed
    UINT32 vidDesc;//video description
    UINT32 lenDesc;//length of video description
    //#NT#2009/11/13#Meg Lin#[0007720] -begin
    //#NT#add for user data
    UINT32 udta_pos;//filepos of user data
    UINT32 udta_size;//size of user data
    //#NT#2009/11/13#Meg Lin#[0007720] -end
} MEDIA_1STV;//media 1st video info

typedef struct _MEDIA_GET1STV
{
    UINT32 addr;    ///<[in] file addr (at least 8 bytes)
    UINT32 size;    ///<[in] free size
    UINT32 validType;///<[in] MEDIA_FILEFORMAT_MOV or others
} MEDIA_GET1STV;


/**
    Struture of ide setting for MediaPlay_Play()
*/
typedef struct
{
    UINT32 uiNowWid;    ///< [in]  ide buffer width
    UINT32 uiNowHei;    ///< [in]  ide buffer height
    UINT32 uiNowFormat; ///< [out] ide format, MEDIAPLAY_VID_422 or others
    UINT32 uiIdeWid;    ///< [in]  ide window width
    UINT32 uiIdeHei;    ///< [in]  ide window height
} MEDIALAY_FBINFO;


extern ER       MediaPlay_WaitReady(void);
extern ER       MediaPlay_FWD(void);
extern ER       MediaPlay_BWD(void);
extern void     MediaPlay_Pause(void);
extern void     MediaPlay_Stop(void);
extern UINT32   MediaPlay_GetTotalSecs(void);
extern UINT32   MediaPlay_GetCurrPlaySecs(void);
extern void     MediaPlay_Show1stVideo(MEDIA_GET1STV *pinfo, FILEDB_HANDLE handle, MEDIA_FIRST_INFO *ptr);
extern ER       MediaPlay_SpeedUp(MEDIAPLAY_SPEEDUP_TYPE ucSpeedUpLevel);
extern ER       MediaPlay_WaitFFBW1XFinish(void);
extern ER       MediaPlay_WaitStepFinish(void);
extern UINT32   MediaPlay_GetTotalVideoFrame(void);

extern void     MediaPlay_FWDStep(UINT32 ucSpeedUpLevel);
extern void     MediaPlay_BWDStep(UINT32 ucSpeedUpLevel);
extern ER       MediaPlay_Open(MEDIAPLAY_OBJ    *pMediaPlayObj);
extern ER       MediaPlay_Close(void);

extern void     MediaPlay_Decode1stVideo(MEDIA_PLAY_1ST_INFO *pMediaInfo);
extern UINT32   MediaPlay_GetNowSecond(void);
extern ER       MediaPlay_SetDecode1stParam(MEDIA_PLAY_1ST_INFO *ptr);
extern ER       MediaPlay_GetDecode1stParam(MEDIA_PLAY_1ST_INFO *ptr);
extern void     MediaPlay_SetVideoDecObj(VIDEO_DECODER *ptr);
extern ER       MediaPlay_GetFileInfo(MEDIAPLAY_FILEINFO infotype, UINT32 *pValue);

extern ER MediaPlay_SetPlayFile(char *path);
extern ER MediaPlay_Play(MEDIALAY_FBINFO *pFbinfo);
extern ER MediaPlay_DecodeMediaInfo(char *pfullPath, UINT32 freeAddr, UINT32 freeSize, MEDIA_FIRST_INFO *ptr);//2009/12/16 Meg Lin
extern UINT32 MediaPlay_GetStatus(void);
extern void MediaPlay_SetIMEIDESize(PIMEIDE_ATTR  pIMEIDEAttr);

extern UINT8    MediaPlay_CheckIfAudio(void);//2011/08/24 Meg


//MediaPlayRecovery.c
extern ER       MediaPlay_FileRecovery(char *pAviFilePath, UINT32 memAddr, UINT32 size);//2011/02/18 Meg Lin

//@}

#endif//_MEDIAPLAYAPI_H


