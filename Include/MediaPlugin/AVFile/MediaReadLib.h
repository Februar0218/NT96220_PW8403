/**
    Header file of media fileformat reading library

    Exported header file of media fileformat reading library.

    @file       MediaReadLib.h
    @ingroup    mIMEDIAWRITE
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MEDIAREADLIB_H
#define _MEDIAREADLIB_H

#include "type.h"
/**
    @addtogroup mIMEDIAREAD
*/
//@{


/**
    @name media file type

     Probe a media file to check file type.
*/
//@{
#define MEDIA_FILEFORMAT_AVI  0x1
#define MEDIA_FILEFORMAT_MOV  0x2
#define MEDIA_FILEFORMAT_ASF  0x4
//@}

#define MEDIA_PARSEINDEX_AVI 0x1

/**
    @name media video codec type

     Read mov header to check video codec type.
*/
//@{
#define MEDIAPLAY_VIDEO_MJPG  0x1  ///< motion-jpeg
#define MEDIAPLAY_VIDEO_H264  0x2  ///< H.264
//@}

#define MEDIA_FILFFORMAT_TOTAL  2+1 //two + default

/**
    @name filetype check ID

     Filetype check ID.
*/
//@{
#define MOVREADLIB_CHECKID  0xBC191380//mdat base time
#define AVIREADLIB_CHECKID  0x72696666//riff
//@}

/**
    @name video codec type check ID

     Video codec type check ID.
*/
//@{
#define MJPG_DECODEID  0x01020304 //1234
#define H264_DECODEID  0x48323634 //H264
//@}

/**
    @name decode 1st frame mode

     Decode 1st frame mode.
*/
//@{
#define MEMORY_NORMAL   0x0 //after decoding, using IME to IDE
#define MEMORY_TINY     0x1 //for 8M_720p, using jpg(h.264) output buffer to IDE
//@}




/**
    @name set info to file reading library

     Setting information type to file reading library.
*/
//@{
#define MEDIAREADLIB_SETINFO_FILESIZE    1 ///< param1:total filesize(in)
#define MEDIAREADLIB_SETINFO_ENTRYBUF    2 ///< param1:address(in), param2:size(in)

/**
    @name get info from file reading library

     Getting information type from file reading library.
*/
//@{
#define MEDIAREADLIB_GETINFO_GETVIDEOPOSSIZE    0x01///< p1:video frame num(in), p2:pos(out), p3:size(out)
#define MEDIAREADLIB_GETINFO_GETAUDIOPOSSIZE    0x02///< p1:audio frame num(in), p2:pos(out), p3:size(out)
#define MEDIAREADLIB_GETINFO_VFRAMENUM          0x03///< p1:total video frame num(out)
#define MEDIAREADLIB_GETINFO_VIDEOTYPE          0x04///< p1:video type, MEDIAPLAY_VIDEO_MJPG or other(out)
#define MEDIAREADLIB_GETINFO_VIDEOFR            0x05///< p1:video frame rate(out)
#define MEDIAREADLIB_GETINFO_USERDATA           0x06///< p1:user data pos(out), p2:size(out)
#define MEDIAREADLIB_GETINFO_FRE1DATA           0x07///< p1:fre1 data pos(out), p2:size(out)
#define MEDIAREADLIB_GETINFO_H264DESC           0x08///< p1:h264 description addr(out), p2:len(out)
#define MEDIAREADLIB_GETINFO_VIDEO_W_H          0x09///< p1:video width(out), p2:video height(out)
#define MEDIAREADLIB_GETINFO_NEXTIFRAMENUM      0x0A///< p1:now video frame num(in), p2:next i framenumber(out)
#define MEDIAREADLIB_GETINFO_PREVIFRAMENUM        0x0B///< p1:now video frame num(in), p2:prev i framenumber(out)
#define MOVREADLIB_GETINFO_MAXIFRAMESIZE        0x81///< p1:max iframe size(out)
#define MOVREADLIB_GETINFO_IFRAMETOTALCOUNT     0x82///< p1:iframe total count(out)
#define MOVREADLIB_GETINFO_SECONDS2FILEPOS      0x83///< p1:sec(in), p2:pos(out)
#define MOVREADLIB_GETINFO_SHOWALLSIZE          0x84///< no parameters, show size and pos of all frames 
#define MOVREADLIB_GETINFO_GETVIDEOENTRY        0x85///< p1:video frame num(in), p2:(MOV_Ientry *)(out)
#define MOVREADLIB_GETINFO_GETAUDIOENTRY        0x86///< p1:audio frame num(in), p2:(MOV_Ientry *)(out)
#define MOVREADLIB_GETINFO_TOTALDURATION        0x87///< p1:total sec(out)
#define MOVREADLIB_GETINFO_NEXTPFRAME           0x8C///< p1:now video frame num(in), p2:next i or p framenumber(out)
#define MOVREADLIB_GETINFO_H264_IPB             0x8D///< p1:has B frames or not(out)
#define MOVREADLIB_GETINFO_VID_ENTRY_ADDR       0x8E///< p1:video entry starting address, gp_movReadEntry(out)
#define MOVREADLIB_GETINFO_AUD_ENTRY_ADDR       0x8F///< p1:audio entry starting address, gp_movReadAudioEntry(out)

#define AVIREADLIB_GETINFO_GETVIDEOENTRY        0x91///< p1:video entry
#define AVIREADLIB_GETINFO_GETAUDIOENTRY        0x92///< p1:audio entry
//@}

//mov audio codec type
#define MEDIAAUDIO_CODEC_SOWT  1//pcm
#define MEDIAAUDIO_CODEC_MP4A  2//aac
#define MEDIAAUDIO_CODEC_RAW8  3
#define MEDIAAUDIO_CODEC_XX    4

#define AVIREAD_INDEXSIZE 0x0A0000 //minimum avi index size

typedef struct
{
    ER (*Probe)(UINT32 addr, UINT32 size);
    ER (*Initialize)(void);
    ER (*SetMemBuf)(UINT32 startAddr, UINT32 size);
    ER (*ParseHeader)(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);
    ER (*GetInfo)(UINT32 type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
    ER (*SetInfo)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
    ER (*CustomizeFunc)(UINT32 type, void *pobj);
    ER (*cbReadFile)(UINT8 *type, UINT32 pos, UINT32 size, UINT32 addr);
    void    (*cbShowErrMsg)(char *fmtstr, ...);
    UINT32 checkID;
} CONTAINERPARSER;

typedef struct
{
    UINT32  checkID;
    UINT32  uiFileStartAddr;        // AVI file start address
    UINT32  uiFirstVideoOffset;     // first video frame offset
    UINT32  uiFirstAudioOffset;     // first audio frame offset
    UINT32  uiIndexChunkOffset;     // index chunk offset
    UINT32  uiTotalFileSize;        // total file size
    UINT32  uiTotalVideoFrames;     // total frames, video
    UINT32  uiTotalAudioFrames;     // total frames, audio
    UINT32  uiTotalSecs;            // total seconds
    UINT32  uiVidWidth;             // video width
    UINT32  uiVidHeight;            // video height
    UINT32  uiVidScale;             // video scale     (66666)
    UINT32  uiVidRate;              // video rate    (1000000)
    UINT32  uiVidType;              // MEDIAPLAY_VIDEO_MJPG or other
    UINT32  uiAudSampleRate;        // audio sample rate
    UINT32  uiAudBytesPerSec;       // audio bytes per second
    UINT32  uiAudWavTotalSamples;   // audio all samples
    UINT32  uiAudType;              // audio type, MEDIAAUDIO_CODEC_SOWT or others
    UINT8   ucAudBitsPerSample;     // audio bits per sample
    UINT8   ucAudChannels;          // audio channels
    BOOL    bAudioSupport;          // with audio data
    BOOL    bVideoSupport;          // with video data
} MEDIA_HEADINFO, *PMEDIA_HEADINFO;

typedef struct
{
    MEDIA_HEADINFO headinfo; //header information
    UINT32 totalsize_high;
    UINT32 totalsize_low;    //small than 4G
    UINT32 nowpos_high;
    UINT32 nowpos_low;       //small than 4G
    UINT16 speed;            //1x, 2x...
    UINT16 direct;           //forward, backward
    UINT16 status;           //play, pause
    UINT16 rev;

} MEDIA_FILE_INFO;

/*
    File info structure for MediaPlay_Show1stVideo() and other APIs
*/
typedef struct 
{
    UINT32 pos;     ///< [out] first video frame position
    UINT32 size;    ///< [out] first video frame size
    UINT16 wid;     ///< [out] video width
    UINT16 hei;     ///< [out] video height
    UINT32 type;    ///< [out] container format after probed, MEDIA_FILEFORMAT_MOV or others
    UINT32 udta_pos;///< [out] filepos of user data
    UINT32 udta_size;///<[out] size of user data
    UINT32 dur;     ///< [out] total duration (in seconds)
    UINT32 audSR;   ///< [out] audioSampleRates
    UINT32 audChs;  ///< [out] audioChannels
    UINT32 vidFR;   ///< [out] videoframeRate
    UINT32 vidtype; ///< [out] videoframe type, MEDIAPLAY_VIDEO_H264 or others
    UINT32 audtype; ///< [out] audiotype, MEDIAAUDIO_CODEC_SOWT or others
    UINT32 fre1_pos;///< [out] filepos of user data
    UINT32 fre1_size;///<[out] size of user data
    UINT32 avgBPS;  ///< [out] average bytesPerSecond//2010/11/22 Meg Lin
} MEDIA_FIRST_INFO;



typedef struct
{
    UINT32  ParserIndex;
    CONTAINERPARSER *parser;
    INT8* ParserName;
} FORMATPARSER_OBJ;

typedef struct
{
    UINT32 fileStartAddr;//in: file address
    UINT32 freeSize;     //in: avaliable size
    UINT32 yAddr;        //in: y starting addr
    UINT32 lcdWidth;     //in: lcd width
    UINT32 lcdHeight;    //in: lcd height
    UINT32 decodeMode;   //in: decode mode, MEMORY_TINY, MEMORY_NORMAL
    UINT32 maxDecWidth;  //in: max raw width 4 panel//2011/08/08 Meg Lin
    UINT32 imageWidth;   //out: image width
    UINT32 imageHeight;  //out: image height
    UINT32 wid_lineoft;  //out: width line offset//2011/11/14 Meg Lin
    UINT32 wid_lineoftuv;  //out: width uv offset//2011/11/14 Meg Lin
    UINT32 cbAddr;       //out: cb addr
    UINT32 crAddr;       //out: cr addr
    UINT32 totalSec;     //out: totalsec              //2010/11/03 Meg Lin
    UINT32 userDataOffset;//out: user data offset     //2010/11/03 Meg Lin
    UINT32 jpgType;      //out: JPGFormatYUV420       //2011/05/20 Meg Lin
    UINT8  filetype;     //out: MEDIA_FILEFORMAT_MOV
    UINT8  videoCodec;   //out: MEDIAPLAY_VIDEO_MJPG
    UINT8  bAudSupport;  //out: with audio or not     //2010/11/03 Meg Lin
    UINT8  rev;                                       //2011/05/20 Meg Lin
} MEDIA_PLAY_1ST_INFO;


extern CONTAINERPARSER  gDefaultReadObj;
extern CONTAINERPARSER  gMovReadObj;
extern CONTAINERPARSER  gAviReadObj;
extern FORMATPARSER_OBJ gFormatParser[];

extern ER MovReadLib_Probe(UINT32 addr, UINT32 readsize);
extern ER MovReadLib_Initialize(void);
extern ER MovReadLib_SetMemBuf(UINT32 startAddr, UINT32 size);
extern ER MovReadLib_ParseHeader(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);
extern ER MovReadLib_GetInfo(UINT32 type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
extern ER MovReadLib_SetInfo(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
extern ER MovReadLib_Parse1stVideo(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);
extern ER MovReadLib_RegisterObjCB(void *pobj);//pContainer = (CONTAINERPARSER *)pobj; 

extern ER MP_AVIReadLib_Probe(UINT32 addr, UINT32 readsize);
extern ER MP_AVIReadLib_Initialize(void);
extern ER MP_AVIReadLib_SetMemBuf(UINT32 startAddr, UINT32 size);
extern ER MP_AVIReadLib_ParseHeader(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);
extern ER MP_AVIReadLib_GetInfo(UINT32 type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
extern ER MP_AVIReadLib_SetInfo(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
extern ER MP_AVIReadLib_Parse1stVideo(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);
extern ER MP_AVIReadLib_RegisterObjCB(void *pobj);//pContainer = (CONTAINERPARSER *)pobj; 


//@}
#endif //_MEDIAREADLIB_H
