/**
    Header file of media fileformat writing library

    Exported header file of media fileformat writing library.

    @file       MediaWriteLib.h
    @ingroup    mIMEDIAWRITE
    @note       Nothing.
    @version    V1.01.006
    @date       2011/06/23

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MEDIAWRITELIB_H
#define _MEDIAWRITELIB_H

#include "type.h"

/**
    @addtogroup mIMEDIAWRITE
*/
//@{



/**
    @name recoding type

     Record a video-only/video-audio media file. Used for ucRecFormat in MediaRec_Open().
*/
//@{
#define MEDIAREC_VID_ONLY           1
#define MEDIAREC_AUD_VID_BOTH       2
#define MEDIAREC_FAST_FWD           3 //video only
#define MEDIAREC_GOLFSHOT           4 //rec 60/120 fps, play 30 fps, no audio

#define MEDIAREC_FLASH_RECORD_AV    0x40 //flash record audio/video
#define MEDIAREC_FLASH_RECORD_VO    0x41 //flash record video only
//@}

/**
    @name video codec type for mov writer

    video codec type for mov writer
*/
//@{
#define MEDIAVIDENC_MJPG 1    ///< motion jpeg
#define MEDIAVIDENC_H264 2    ///< h.264
//@}

/**
    @name audio codec type for mov writer

    audio codec type for mov writer
*/
//@{
#define MOVAUDENC_PCM  1    ///< PCM
#define MOVAUDENC_AAC  2    ///< AAC
//@}


/**
    Get information from media fileformat writing library.
    format: ER (*GetInfo)(MEDIAWRITE_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3); 
*/
typedef enum
{
    MEDIAWRITE_GETINFO_HEADERSIZE = 0x1,   ///< param1: headersize per second(out)
    MEDIAWRITE_GETINFO_VALID_VFNUM = 0x2,   ///< param1: valid video frame numbers(out)//2010/04/20 Meg Lin
    MEDIAWRITE_GETINFO_VIDEOENTRY_ADDR = 0x81,  ///< param1: video frame number(in), param2: video entry addr(out)
    MEDIAWRITE_GETINFO_AUDIOENTRY_ADDR = 0x82,  ///< param1: audio frame number(in), param2: audio pos(out), param3: audio size(out)
    ENUM_DUMMY4WORD(MEDIAWRITE_GETINFO_TYPE)
} MEDIAWRITE_GETINFO_TYPE;


/**
    Set information to media fileformat writing library for making file header.
    format: ER (*SetInfo)(MEDIAWRITE_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAWRITE_SETINFO_WID_HEI        = 0x1,  ///< param1:width(in), param2: height(in) 
    MEDIAWRITE_SETINFO_VID_FRAME      = 0x2,  ///< param1:total video frame (in)
    MEDIAWRITE_SETINFO_AUD_FRAME      = 0x3,  ///< param1:total audio frame (in)
    MEDIAWRITE_SETINFO_REC_DURATION   = 0x4,  ///< param1:total duration microsecond(in)
    MEDIAWRITE_SETINFO_FRAMEINFOBUF   = 0x5,  ///< param1:addr(in), param2:size(in)
    MEDIAWRITE_SETINFO_VID_FR         = 0x6,  ///< param1:video frame rate(in)
    MEDIAWRITE_SETINFO_AUD_SAMPLERATE = 0x7,  ///< param1:audio sample rate(in)
    MEDIAWRITE_SETINFO_USERDATA       = 0x8,  ///< param1:addr(in), param2:size(in)
    MEDIAWRITE_SETINFO_AUD_CHS        = 0x9,  ///< param1:channels (in)//2009/12/21 Meg
    MEDIAWRITE_SETINFO_AUD_BITRATE    = 0xA,  ///< param1:audio bit rate (in), for AAC
    MEDIAWRITE_SETINFO_FRE1DATA       = 0xB,  ///< param1:addr(in), param2:size(in)//2010/01/29 Meg Lin
    MEDIAWRITE_SETINFO_RESET          = 0xC,  ///< no parameters, reset variables
    MEDIAWRITE_SETINFO_VIDEOTYPE      = 0xD,  ///< param1:MEDIAVIDENC_MJPG
    MEDIAWRITE_SETINFO_CHECKHDR       = 0xE,  ///< param2: outlen (out)
    MEDIAWRITE_SETINFO_CLUSTERSIZE    = 0xF,  ///< param1: cluster size (in)
    MEDIAWRITE_SETINFO_UPDATEHDR      = 0x10, ///< no parameters
    MEDIAWRITE_SETINFO_CUTENTRY       = 0x11, ///< param1:(MEDIAREC_FLASHINFO *)(in)
    MEDIAWRITE_SETINFO_SAVE_VF        = 0x12, ///< param1: save video frame number
    MEDIAWRITE_SETINFO_MOVVIDEOENTRY  = 0x81, ///< mov: param1:num(in), param2:(MOV_Ientry *)(in)
    MEDIAWRITE_SETINFO_MOVAUDIOENTRY  = 0x82, ///< mov: param1:num(in), param2:(MOV_Ientry *)(in)
    MEDIAWRITE_SETINFO_MOVMDATSIZE    = 0x83, ///< mov: param1:mdatsize(in)
    MEDIAWRITE_SETINFO_TEMPMOOVADDR   = 0x84, ///< mov: param1:(UINT8 *)(in)
    MEDIAWRITE_SETINFO_AUDIOSIZE      = 0x85, ///< mov: param1:audiodatasize(in)
    MEDIAWRITE_SETINTO_MOV_264DESC    = 0x87, ///< mov: param1:264 video description addr(in), param2: len (in)
    MEDIAWRITE_SETINFO_AUDIOTYPE      = 0x89, ///< mov: param1:MOVAUDENC_PCM 
    MEDIAWRITE_SETINFO_UPDATEVIDPOS   = 0x8A, ///< mov: param1:num(in), param2:(MOV_Ientry *)(in)
    MEDIAWRITE_SETINFO_UPDATEAUDPOS   = 0x8B, ///< mov: param1:num(in), param2:(MOV_Ientry *)(in)
    MEDIAWRITE_SETINFO_AVIVIDPOS      = 0x91, ///< avi: param1:num(in), param2:pos(in), param3:size(in)
    MEDIAWRITE_SETINFO_AVIAUDPOS      = 0x92, ///< avi: param1:num(in), param2:pos(in), param3:size(in)
    MEDIAWRITE_SETINFO_TEMPIDX1ADDR   = 0x93, ///< avi: param1:(UINT8 *)(in)
    MEDIAWRITE_SETINFO_NOWPOS         = 0x94, ///< avi: param1:now fileposition(in)
    MEDIAWRITE_SETINFO_H264DESC       = 0x95, ///< avi: param1:h264desc(in), param2: h264descSize(in)
    MEDIAWRITE_SETINFO_PADAVISIZE     = 0x96, ///< avi: param1:num(in), param2:x, param3:size(in)
    MEDIAWRITE_SETINFO_NIDXDATA       = 0x97, ///< avi: param1:nidx info (MEDIAREC_NIDXINFO *)(in)
    MEDIAWRITE_SETINFO_MAKEIDX1DATA   = 0x98, ///< avi: param1:(MEDIAREC_MAKEAVIIDX1 *)(in)
    MEDIAWRITE_SETINFO_GPSDATA        = 0x99, ///< avi: param1:gps info (MEDIAREC_GPSINFO *)(in)
    MEDIAWRITE_SETINFO_MP4VIDEOENTRY  = 0xA1, ///< mp4: param1:num(in), param2:(MOV_Ientry *)(in)
    MEDIAWRITE_SETINFO_MP4AUDIOENTRY  = 0xA2, ///< mp4: param1:num(in), param2:(MOV_Ientry *)(in)
    MEDIAWRITE_SETINFO_MP4MDATSIZE    = 0xA3, ///< mp4: param1:mdatsize(in)
    MEDIAWRITE_SETINFO_MP4TEMPMOOVADDR= 0xA4, ///< mp4: param1:(UINT8 *)(in)
    MEDIAWRITE_SETINFO_MP4AUDIOSIZE   = 0xA5, ///< mp4: param1:audiodatasize(in)
    ENUM_DUMMY4WORD(MEDIAWRITE_SETINFO_TYPE)
} MEDIAWRITE_SETINFO_TYPE;



/** 
    Callback function type for media recording. Event types are defined in MediaRecAPI.h.
*/

typedef void (MediaRecCallbackType)(UINT32 event_id, UINT32 Seconds);
typedef void (MediaRecDrawStr)(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineoftY, UINT32 drawWidth);//2010/09/07 Meg Lin
typedef void (MediaRecOpenDIS)(UINT32 event_id, UINT32 value);//2010/10/29 Meg Lin

/**
    Structure definition for Media recording flash info.
    (for MEDIAREC_OBJ)
*/
typedef struct
{
    UINT32 fstVideoPos;     ///< first video offset
    UINT32 fstAudioPos;     ///< first audio offset
    UINT32 startSec;        ///< real start second
    UINT32 flashSec;        ///< flash second
    UINT32 frameRate;       ///< video frame rate
} MEDIAREC_FLASHINFO;

/**
    Structure definition for Media recording quick info.
    (for MEDIAREC_OBJ)
*/
typedef struct
{
    UINT32              uiQVMemAddr;      ///< [in]  quick view output addr
    UINT32              uiQVActualSize;   ///< [out] quick view picutre size
    UINT8               ucQVEnable;       ///< [in]  quick view enable or not
    UINT8               rev;
    UINT16              rev2;
} MEDIAREC_QVINFO;

/**
    Structure definition for Media recording user data.
    (for MEDIAREC_OBJ)
*/
typedef struct
{
    UINT16              uiDisplayWidth;   ///< display width
    UINT16              uiDisplayHeight;  ///< display height
    UINT16              uiThumbWidth;     ///< thumbnail width
    UINT16              uiThumbHeight;    ///< thumbnail height
    UINT32              uiThumbLen;       ///< [out] thumbnail length
    UINT32              uiUserDataBuf;    ///< user data buffer
    UINT32              uiUserDataSize;   ///< size of user data buffer
    UINT8               ucUserDataEmbed;  ///< encode userdata embeded or not
    UINT8               rev; 
    UINT16              rev2;
} MEDIAREC_USERDATA;

/**
    Structure definition for Media recording rate control.
    (for MEDIAREC_OBJ)
*/
typedef struct
{
    UINT32                  uiTargetBitrate;    ///< video data bytes per second
    UINT8                   ucEnableBRCtrl;     ///< enable rate control or not
    UINT8                   rev8;    
    UINT16                  rev16;
} MEDIAREC_BRCTRL;

/**
    Structure definition for Media recording file limit.
    (for MEDIAREC_OBJ)
*/
typedef struct
{
    UINT32                  uiMaxRecSecs;       ///< if not zero, stop when recording time is enough
    UINT32                  uiMaxRecFileSize;   ///< if not zero, stop when recording size is enough
} MEDIAREC_FILELIMIT;

/**
    Structure definition for Media recording file info.
    (for MEDIAREC_OBJ)
*/
typedef struct
{
    UINT32  filetype;   ///< [in] file format, only MEDIAREC_MOV
    UINT32  vFoffset;   ///< [out] offset between two video frames
    UINT32  aFoffset;   ///< [out] offset between two audio frames
} MEDIAREC_FILEINFO;


typedef struct
{
    UINT32 dataoutAddr;         ///< [in] address to put nidx data
    UINT32 thisVideoStartPos;   ///< [in] this second video data starting fileoffset
    UINT32 thisVideoSize;       ///< [in] this second video data size
    UINT32 thisVideoStartFN;    ///< [in] this second video starting frame number
    UINT32 thisVideoFN;         ///< [in] this second video frame number
    UINT32 thisVideoFrameData;  ///< [in] this second 1st video frame entry addr
    UINT32 thisVideoFrmDataLen; ///< [in] this second video frame info len
    UINT32 thisAudioStartPos;   ///< [in] this second audio data starting fileoffset
    UINT32 thisAudioSize;       ///< [in] this second audio data size
    UINT32 lastNidxdataPos;     ///< [in] last nidx fileoffset

} MEDIAREC_NIDXINFO;

typedef struct
{
    UINT32 junkCC;      //oft: 0
    UINT32 junkSize;    //oft: 4
    UINT32 nidxCC;      //oft: 8
    UINT32 nidxSize;    //oft: c
    UINT32 audPos;      //oft: 10
    UINT32 audSize;     //oft: 14
    UINT32 lastjunkPos; //oft: 18
    UINT32 vidstartFN;  //oft: 1c
    UINT32 vidPosSize;//array of vidPos, vidSize
} MEDIAREC_NIDXAVI;

typedef struct
{
    UINT32 outsize;

} MEDIAREC_MAKEAVIIDX1;

//#NT#2011/06/23#Meg Lin -begin
typedef struct
{
    UINT32 bufAddr;             ///< [in] address to put gps data
    UINT32 clusterSize;         ///< [in] this tag size
    UINT32 gpsDataAddr;         ///< [in] gpsData addr
    UINT32 gpsDataSize;         ///< [in] gpsData size

} MEDIAREC_GPSINFO;

typedef struct
{
    UINT32 junkCC;      //oft: 0
    UINT32 junkSize;    //oft: 4
    UINT32 gpsCC;      //oft: 8
    UINT32 gpsSize;    //oft: c
    UINT32 gpsData;    //GPS data starting pointer
} MEDIAREC_GPSAVI;
//#NT#2011/06/23#Meg Lin -end

/**
    Structure definition for media recording operation.
    (for MediaRec_Open() )
*/
typedef struct
{
    UINT32              uiMemAddr;          ///< starting address for recording
    UINT32              uiMemSize;          ///< size for recording
    UINT32              uiFrontHDRAddr;     ///< [out] address to put front HDR
    MediaRecCallbackType  *CallBackFunc;    ///< event inform callback
    MediaRecDrawStr     *CallbackDrawFunc;  ///< callback draw func//2010/07/30 Meg Lin
    MediaRecOpenDIS     *CallbackOpenDISFunc;///< callback open dis//2010/10/29 Meg Lin
    UINT8               ucRecFormat;        ///< recording type, MEDIAREC_AUD_VID_BOTH
    UINT8               ucVidCodec;         ///< recording video codec, MEDIAREC_ENC_JPG, MEDIAREC_ENC_H264
    UINT8               ucYCbCrFormat;      ///< [out] recording YCbCr type, MEDIAREC_JPEG_FORMAT_420
    UINT8               ucVidFrameRate;     ///< recording video frame rate, MEDIAREC_VRATE_30
    UINT32              uiRecQuality;       ///< recording video quality, MEDIAREC_VQUALITY_NORMAL
    UINT8               rev;
    UINT8               ucCBthreshold;      ///< video frames for callback, usually equals to frame rate 
    UINT8               ucAudCodec;         ///< recording audio codec, MEDIAREC_ENC_PCM, MEDIAREC_ENC_PCM
    UINT8               ucSyncN;            ///< [out] if half sec, half video rate
    UINT16              uiMaxWidth;         ///< recording max video width, 1440x1088 or 1280x720
    UINT16              uiMaxHeight;        ///< recording max video height
    UINT16              uiWidth;            ///< now recording video width, 1440x1088/1280x720/640x480/320x240
    UINT16              uiHeight;           ///< now recording video height
    UINT32              uiAudSampleRate;    ///< recording audio sample rate, AUDIO_SR_8000
    UINT16              uiAudBits;          ///< audio setting, bits per sample, 8 or 16
    UINT16              uiAudChannels;      ///< audio setting, channels, 1 or 2 
    UINT32              uiAudChunkSize;     ///< [out] chunksize bytes per second
    UINT32              uiAudAlignCluSize;  ///< [out] chunksize bytes align to cluster size
    UINT32              audioCh;            ///< audio recording channel, AUDIO_CH_LEFT or others//2009/12/22 Meg
    UINT32              uiHeadersize;       ///< [out] headersize after start recording
    UINT32              uiMaxTime;          ///< [out] max recording time by frame entries //2010/04/20 Meg Lin
    UINT32              uiMaxVF;            ///< [out] max recording video frame entries   //2010/04/20 Meg Lin
    UINT32              uiFrameEntryStart;  ///< [out] memory address for saving frame entries
    UINT32              uiPanelWidth;       ///< [in] panel width, set to IPL 2011/06/11 Meg Lin
    UINT32              uiPanelHeight;      ///< [in] panel height, set to IPL 2011/06/11 Meg Lin
    MEDIAREC_QVINFO     QVinfo;             ///< quickview information
    MEDIAREC_USERDATA   UserData;           ///< user data embedded
    MEDIAREC_BRCTRL     BRCtrl;             ///< rate control information
    MEDIAREC_FILELIMIT  FileLimit;          ///< file limit information, not used now 2010/01/25 Meg 
    MEDIAREC_FILEINFO   FileInfo;           ///< file type information
    UINT16              ucSeamlessSec;      ///< seamless cut second,
    UINT8               ucSaveType;         ///< save type, MEDIAREC_SAVETYPE_HALF_SEC
    UINT8               ucEndType;          ///< end type, CUTOVERLAP, NORMAL
} MEDIAREC_OBJ;

/**
    Structure definition for media fileformat writing container.
    (for MediaRecCreateContainerObj()) For example, we can get mov recording container by 
    mov_getContainerMaker(), and set to gMediaRecMaker.
*/
typedef struct
{
    ER (*Initialize)(void);                          ///< initializing fileformat container   
    ER (*SetMemBuf)(UINT32 startAddr, UINT32 size);  ///< set memory to fileformat container
    ER (*MakeHeader)(UINT32 hdrAddr, UINT32 *pSize, void *pobj); ///< make front-header using fileformat maker
    ER (*UpdateHeader)(UINT32 type, void *pobj);     ///< make end-header using fileformat maker
    ER (*GetInfo)(MEDIAWRITE_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3); ///< get information from media fileformat container.
    ER (*SetInfo)(MEDIAWRITE_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);       ///< set information to media fileformat container.
    ER (*CustomizeFunc)(UINT32 type, void *pobj);    ///< custimized function if neeeded
    ER (*cbWriteFile)(UINT8 *type, UINT32 pos, UINT32 size, UINT32 addr); ///< a file-writing function for fileformat maker
    void    (*cbShowErrMsg)(char *fmtstr, ...);      ///< a debug function for fileformat maker
    UINT32 checkID;                                  ///< checkID for fileformat maker to check valid type
} CONTAINERMAKER, *PCONTAINERMAKER;

/*
    Public funtions to use fileformat maker
*/
//@{
extern PCONTAINERMAKER mov_getContainerMaker(void);
extern ER MovWriteLib_RegisterObjCB(void *pobj);
extern PCONTAINERMAKER avi_getContainerMaker(void);
extern ER MP_AviWriteLib_RegisterObjCB(void *pobj);

//@}

#endif//_MEDIAWRITELIB_H

//@}

