/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AVILib.h
    @ingroup    mIAVAVI

    @brief      AVI utility functions


    @note       Nothing.

*/

#ifndef _AVILIB_H
#define _AVILIB_H

#define AVI_PARSEOK             0
#define AVI_ERRHEADER           (-1)
#define AVI_ERRDECODE           (-2)
#define AVI_ERRREAD             (-3)
#define AVI_ERRBIGFILE          (-4)

//#NT#2009/03/25#Scottie -begin
// AVI file 'four CC' literals
#define FOURCC_RIFF             0x46464952  // 'RIFF'
#define FOURCC_LIST             0x5453494c  // 'LIST'
#define FOURCC_AVI              0x20495641  // 'AVI '
#define FOURCC_HDRL             0x6c726468  // 'hdrl'
#define FOURCC_AVIH             0x68697661  // 'avih'
#define FOURCC_STRL             0x6c727473  // 'strl'
#define FOURCC_STRH             0x68727473  // 'strh'
#define FOURCC_STRF             0x66727473  // 'strf"
#define FOURCC_INFO             0x4f464e49  // 'INFO'
#define FOURCC_ISFT             0x54465349  // 'ISFT'
#define FOURCC_MOVI             0x69766f6d  // 'movi'
#define FOURCC_JUNK             0x4b4e554a  // 'JUNK'
#define FOURCC_IDX1             0x31786469  // 'idx1'
#define FOURCC_VIDS             0x73646976  // 'vids'
#define FOURCC_AUDS             0x73647561  // 'auds'
#define FOURCC_mjpg             0x67706a6d  // 'mjpg'
#define FOURCC_MJPG             0x47504a4d  // 'MJPG'
#define FOURCC_00DC             0x63643030  // '00dc'
#define FOURCC_01WB             0x62773130  // '01wb'
#define FOURCC_WB01             0x31306277  // 'wb01'
#define FOURCC_WAVE             0x45564157  // 'MOVE'
#define FOURCC_DATA             0x61746164  // 'data'
#define FOURCC_WAVE             0x45564157  // 'WAVE'
#define FOURCC_FMT              0x20746d66  // 'fmt '
//#NT#2009/03/25#Scottie -end
//#NT#2009/03/31#Photon Lin -begin
#define FOURCC_IDIT             0x54494449  // 'IDIT'
//#NT#2009/03/31#Photon Lin -end
//#NT#2007/07/27#KH Chang -begin
#define PARSE_IDXCHUNK          1
//#NT#2007/07/27#KH Chang -end

typedef struct
{
    UINT32      uiWavGroupID;           // 'RIFF'
    UINT32      uiWavTotalSize;         // audio size + 44 - 8
    UINT32      uiWavRiffType;          // 'WAVE'
    UINT32      uiWavFmtChunkID;        // 'fmt '
    UINT32      uiWavFmtChunkSize;      // 16
    INT16       sWavFormatTag;
    UINT16      usWavChannels;

    UINT32      uiWavSamplesPerSec;
    UINT32      uiWavAvgBytesPerSec;

    UINT16      usWavBlockAlign;
    UINT16      usWavBitsPerSample;
    UINT32      uiWavDatChunkID;
    UINT32      uiWavDatChunkSize;
} WAVEHEAD_INFO, *PWAVEHEAD_INFO;

typedef struct _AVIHEAD_INFO
{
//#NT#2007/04/04#KH Chang -begin
//#Add for keeping AVI file start address
    UINT32  uiFileStartAddr;        // AVI file start address
//#NT#2007/04/04#KH Chang -end
    UINT32  uiFirstVideoOffset;     // AVI first video frame offset
    UINT32  uiFirstAudioOffset;     // AVI first audio frame offset
    UINT32  uiIndexChunkOffset;     // AVI index chunk offset
    UINT32  uiMovListOffset;        // AVI MOVI List offset
    UINT32  uiUserDataOffset;       // AVI JUNK data offset
    UINT32  uiTotalFileSize;        // total file size
    UINT32  uiTotalFrames;          // total frames, video + audio
    UINT32  uiToltalSecs;           // total seconds
    UINT32  uiVidWidth;             // video width
    UINT32  uiVidHeight;            // video height
    UINT32  uiVidScale;             // video scale     (66666)
    UINT32  uiVidRate;              // video rate    (1000000)
    UINT32  uiAVIAudSampleRate;     // audio sample rate
    UINT32  uiAudBytesPerSec;       // audio bytes per second
    UINT32  uiAudWavTotalSamples;   // audio all samples
    UINT8   ucAudBitsPerSample;     // audio bits per sample
    UINT8   ucAudChannels;          // audio channels
    BOOL    bAudioSupport;          // with audio data
    BOOL    bVideoSupport;          // with video data
    //#NT#2009/12/23#Photon Lin -begin
    //#Add header infomation
    UINT32  fccHandler;
    UINT32  FormatTag;
    UINT32  SamplesPerSec;
    UINT32  BlockAlign;
    UINT32  CBSize;
    BOOL    IDITexist;
    //#NT#2009/12/23#Photon Lin -end

    //#NT#2010/01/08#Lily Kao - begin
    UINT32  uiThumbWidth;           // AVI Thumbnail Width
    UINT32  uiThumbHeight;          // AVI Thumbnail Height
    UINT32  uiThumbSize;            // AVI Thumbnail Size
    UINT32  uiThumbOfs;             // thumbnail offset
    UINT32  videoOnlyByteSize;
    //#NT#2010/01/08#Lily Kao - end
    //#NT#2010/02/01#Photon Lin -begin
    UINT32  uiEV;
    //#NT#2010/02/01#Photon Lin -end
} AVIHEAD_INFO, *PAVIHEAD_INFO;

//Functions
//#NT#2009/03/19#Photon Lin -begin
extern INT32 AVIUti_ParseHeader(UINT32 AVIStartAddr, AVIHEAD_INFO *pAviParams, UINT32 ReadAVISize);
//Use AVIFileSZ, ReadAVISize can be '0' for use system default
extern INT32 AVIUti_ParseHeaderWithTargetSize(UINT32 AVIFileSZ, UINT32 AVIStartAddr,AVIHEAD_INFO *pAviParams, UINT32 ReadAVISize);
extern void AVIUti_SetThumbOffset(UINT32 offset);
extern void AVIUti_EnableThumbCheck(void);
//#NT#2010/03/01#Photon Lin -begin
//#Modify function name
extern void AVIUti_SetUserCustom(UINT32 custom, BOOL setting);
//#NT#2010/03/01#Photon Lin -end
//#NT#2009/03/19#Photon Lin -end
#endif

