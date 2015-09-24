/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AVILib.h
    @ingroup    mIAVAVI

    @brief      AVI utility functions


    @version    V1.01.003
    @date       2010/05/05
    @note       Nothing.

*/

#ifndef _AVILIB_H
#define _AVILIB_H

#include "MediaReadLib.h"
#include "MediaWriteLib.h"
#include "audio.h"

#define AVI_PLAYDEBUG       1//show debug msg

#define AVI_H264_GOPFN     15//H264 GOP frame number = i+14p = 15
#define AVI_H264_GOPFN20   20//H264 GOP frame number = i+19p = 15//2011/11/21 Meg

#define AVI_PARSEOK             0
#define AVI_ERRHEADER           (-1)
#define AVI_ERRDECODE           (-2)
#define AVI_ERRREAD             (-3)
#define AVI_ERRBIGFILE          (-4)

#define AVIWRITELIB_CHECKID  0x72696666//riff

#define AVI_RIFF   0x66666972
#define AVIREADLIB_SETINFO_FILESIZE         1//filesize, param1= size
#define AVI_Probe_RIFF  0x46464952

// AVI file 'four CC' literals

#define FOURCC_RIFF 0x46464952  // 'RIFF'
#define FOURCC_LIST 0x5453494c  // 'LIST'
#define FOURCC_AVI  0x20495641  // 'AVI '
#define FOURCC_HDRL 0x6c726468  // 'hdrl'
#define FOURCC_AVIH 0x68697661  // 'avih'
#define FOURCC_STRL 0x6c727473  // 'strl'
#define FOURCC_STRH 0x68727473  // 'strh'
#define FOURCC_STRF 0x66727473  // 'strf"
#define FOURCC_INFO 0x4f464e49  // 'INFO'
#define FOURCC_ISFT 0x54465349  // 'ISFT'
#define FOURCC_MOVI 0x69766f6d  // 'movi'
#define FOURCC_JUNK 0x4b4e554a  // 'JUNK'
#define FOURCC_IDX1 0x31786469  // 'idx1'
#define FOURCC_VIDS 0x73646976  // 'vids'
#define FOURCC_AUDS 0x73647561  // 'auds'
#define FOURCC_mjpg 0x67706a6d  // 'mjpg'
#define FOURCC_MJPG 0x47504a4d  // 'MJPG'
#define FOURCC_00DC 0x63643030  // '00dc'
#define FOURCC_01WB 0x62773130  // '01wb'
#define FOURCC_WB01 0x31306277  // 'wb01'
#define FOURCC_01WB 0x62773130  // '01wb'
#define FOURCC_WAVE 0x45564157  // 'MOVE'
#define FOURCC_DATA 0x61746164  // 'data'
#define FOURCC_WAVE 0x45564157  // 'WAVE'
#define FOURCC_FMT  0x20746d66  // 'fmt '
#define FOURCC_IDIT 0x54494449  // 'IDIT'       //DateTimeOriginal
#define FOURCC_H264 0x34363248  // 'H264'
#define FOURCC_NIDX 0x7864696e  // 'nidx'       //2011/02/18 Meg Lin
#define FOURCC_GPS  0x20535047  // 'GPS '       //2011/06/23 Meg Lin

typedef struct 
{
    UINT32 pos;
    UINT32 size;
    

} AVI_Entry;

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




//
// here define some default value of record avi file
//
#define     AVIDEFAULT_FORMAT           (AVIREC_AUD_VID_BOTH)   // (AVIREC_VIDEO_ONLY)  // (AVIREC_AUDIO_ONLY)
#define     AVIDEFAULT_STOP_CONDITION   AVIREC_STOP_BY_CMD
#define     AVIDEFAULT_VIDEO_FR         30// 15 fps
#define     AVIDEFAULT_MICRO_SEC_FRAME  (1000000 / AVIDEFAULT_VIDEO_FR)
#define     AVIDEFAULT_QUALITY          75
#define     AVIDEFAULT_WIDTH            320
#define     AVIDEFAULT_HEIGHT           240
#define     AVIDEFAULT_AUDIO_RATE       AUDIO_SR_8000    // 8000 //8064 //11025     // AUDIO Simple RATE
#define     AVIDEFAULT_AUDIO_CHANNEL    1
#define     AVIDEFAULT_AUDIO_SIZE       16000    // AVIDEFAULT_AUDIO_CHANNEL * (AVIDEFAULT_AUDIO_BITS / 8) * AVIDEFAULT_AUDIO_RATE
#define     AVIDEFAULT_AUDIO_SAMP_BYTE  2        // 2 bytes per sample
#define     AVIDEFAULT_AUDIO_SAMP_BIT   16       // 16 bits per sample

#define AVIREC_AVI_HEADER_SIZE      (512+12)        // avi header size include junk data    // 12 bytes for LIST-size-movi
/* flags for use in <dwFlags> in AVIFileHdr */
#define AVIF_HASINDEX       0x00000010  // Index at end of file
#define AVIF_MUSTUSEINDEX   0x00000020
#define AVIF_ISINTERLEAVED  0x00000100
#define AVIF_TRUSTCKTYPE    0x00000800  // Use CKType to find key frames
#define AVIF_WASCAPTUREFILE 0x00010000
#define AVIF_COPYRIGHTED    0x00020000
/*--------------------------------------*/
/*      AVI main header structure       */
/*--------------------------------------*/
typedef __packed struct
{
   int ID;
   int Type;
   int Size;
   int MicroSecPerFrame;
   int MaxBytePerSec;
   int Reverved1;
   int Flags;
   int TotalFrames;
   int InitialFrames;
   int Streams;
   int SuggestedBufferSize;
   int Width;
   int Height;
   int Scale;
   int Rate;
   int Start;
   int Length;
} MAINAVIHEADER;
/*--------------------------------------*/
/*      AVI stream header structure       */
/*--------------------------------------*/
typedef __packed struct
{
  int Type;                 //'vids' or 'auds'
  int Handler;              // 'mjpg/MPG4"
  int Flags;
  int Reserved1;
  int InitialFrames;
  int Scale;
  int Rate;
  int Start;
  int Length;
  int SuggestedBufferSize;
  int Quality;
  int SampleSize;
  int Rec_UpLeft;
  int Rec_DownRight;
} AVISTREAMHEADER;
/*--------------------------------------*/
/*      Video data header structure     */
/*--------------------------------------*/
typedef __packed struct
{
   int    Size;
   int    Width;
   int    Height;
   short  Planes;
   short  BitCount;
   int    Compression;
   int    SizeImage;
   int    XPelsPerMeter;
   int    YPelsPerMeter;
   int    ClrUsed;
   int    ClrImportant;
} DIBINFO;
/*--------------------------------------*/
/*      Audio data header structure     */
/*--------------------------------------*/
typedef __packed struct
{
   short  wFormatTag;
   short  nChannels;
   int    nSamplesPerSec;
   int    nAvgBytesPerSec;
   short  nBlockAlign;
   short  wBitsPerSample;
} WAVEINFO;
/*--------------------------------------*/
/*  AVI INFO structure    */
/*--------------------------------------*/
typedef  __packed struct
{
   UINT32 tag;
   UINT32 size;
   UINT8 timecode[26];
}AVIIDITTAG;
//Add for AVI header with embeded user data
/*--------------------------------------*/
/*  AVI IDIT structure    */
/*--------------------------------------*/
typedef __packed struct
{
   UINT32 ID;
   UINT32 tag;
   UINT32 size;
   UCHAR info[40];
}AVIINFOTAG;


typedef __packed struct{
    UINT  RIFF;
    UINT  riff_len;
    UINT  AVI;
        UINT  LIST0;
        UINT  LIST0_LEN;
        //UCHAR AVIMainHeader[sizeof(MAINAVIHEADER)];   //  68 bytes
            MAINAVIHEADER MainAviHeader;
            UINT  LIST1;
            UINT  LIST1_LEN;
            UINT  STRL1;
            UINT  STRH1;
            UINT  STRH1_LEN;
            //UCHAR VideoStreamHeader[sizeof(AVISTREAMHEADER)]; // 56 bytes
                AVISTREAMHEADER VidStrHeader;
            UINT  STRF1;
            UINT  STRF1_LEN;
            //UCHAR VideoStreamFmt[sizeof(DIBINFO)];    // 40 bytes
            DIBINFO     VidStrFmt;
            //#if __AUDIO_ENABLE
            UINT  LIST2;
            UINT  LIST2_LEN;
            UINT  STRL2;
            UINT  STRH2;
            UINT  STRH2_LEN;
                //UCHAR AudioStreamHeader[sizeof(AVISTREAMHEADER)];  // 56 bytes
                AVISTREAMHEADER AudStrHeader;
            UINT  STRF2;
            UINT  STRF2_LEN;
            //UCHAR AudioStreamFmt[sizeof(WAVEINFO)];  // 16 bytes
                WAVEINFO AudStrFmt;

//Add for AVI header with embeded user data
            AVIIDITTAG IDITtag;                        // 26 bytes
            UINT  LIST3;
            UINT  LIST3_LEN;
            AVIINFOTAG INFOTag;                        // 52 bytes

            //#endif // __AUDIO_ENABLE
}NOJUNKAVIHEADER, *PNOJUNKAVIHEADER;

/*--------------------------------------*/
/*  AVI file's header structure     */
/*--------------------------------------*/

typedef __packed struct {
    UINT  RIFF;
    UINT  riff_len;
    UINT  AVI;
        UINT  LIST0;
        UINT  LIST0_LEN;
        //UCHAR AVIMainHeader[sizeof(MAINAVIHEADER)];   //  68 bytes
        MAINAVIHEADER MainAviHeader;
            UINT  LIST1;
            UINT  LIST1_LEN;
            UINT  STRL1;
            UINT  STRH1;
            UINT  STRH1_LEN;
            //UCHAR VideoStreamHeader[sizeof(AVISTREAMHEADER)]; // 56 bytes
            AVISTREAMHEADER VidStrHeader;
            UINT  STRF1;
            UINT  STRF1_LEN;
            //UCHAR VideoStreamFmt[sizeof(DIBINFO)];    // 40 bytes
            DIBINFO     VidStrFmt;
//#if __AUDIO_ENABLE
            UINT  LIST2;
            UINT  LIST2_LEN;
            UINT  STRL2;
            UINT  STRH2;
            UINT  STRH2_LEN;
            //UCHAR AudioStreamHeader[sizeof(AVISTREAMHEADER)];  // 56 bytes
            AVISTREAMHEADER AudStrHeader;
            UINT  STRF2;
            UINT  STRF2_LEN;
            //UCHAR AudioStreamFmt[sizeof(WAVEINFO)];  // 16 bytes
            WAVEINFO AudStrFmt;
    UINT  JUNK;
    UINT  JUNK_LEN;
    UCHAR JUNK_CHAR[180];   // for 512 bytes align
//#else
//  UINT  JUNK;
//  UINT  JUNK_LEN;
//  UCHAR JUNK_CHAR[280];
//#endif
    UINT  LIST3;
    UINT  LIST3_LEN;
    UINT  MOVI;
} AVIHEADER;
typedef __packed struct{
    UINT  RIFF;
    UINT  riff_len;
    UINT  AVI;
        UINT  LIST0;
        UINT  LIST0_LEN;
        //UCHAR AVIMainHeader[sizeof(MAINAVIHEADER)];   //  68 bytes
            MAINAVIHEADER MainAviHeader;
            UINT  LIST1;
            UINT  LIST1_LEN;
            UINT  STRL1;
            UINT  STRH1;
            UINT  STRH1_LEN;
            //UCHAR VideoStreamHeader[sizeof(AVISTREAMHEADER)]; // 56 bytes
                AVISTREAMHEADER VidStrHeader;
            UINT  STRF1;
            UINT  STRF1_LEN;
            //UCHAR VideoStreamFmt[sizeof(DIBINFO)];    // 40 bytes
            DIBINFO     VidStrFmt;

//Add for AVI header with embeded user data
            AVIIDITTAG IDITtag;                        // 26 bytes
            UINT  LIST3;
            UINT  LIST3_LEN;
            AVIINFOTAG INFOTag;                        // 52 bytes

    UINT  JUNK;
    UINT  JUNK_LEN;
    UINT8 JUNK_DATA[280];
}AVI_VID_ONLY_HEADER, *PAVI_VID_ONLY_HEADER;

typedef struct{
    UINT32  riff;
    UINT32  fileSize;
    UINT32  wave;
    UINT32  fmt;
    UINT32  fmtLen;
    UINT16  rev1;
    UINT16  channel;
    UINT32  sampleRate;
    UINT32  bytesPerSec;
    UINT16  bytesPerSample;
    UINT16  bitsPerSample;
    UINT32  data;
    UINT32  dataSize;
}WAVE_HEADER, *PWAVE_HEADER;

typedef struct _AVIREC_FILEOBJ
{
    UINT32 videoFrameCount;//total video frame
    UINT32 audioFrameCount;//
    UINT32 nowPos;
    UINT32 sizeAlignCluster;
    UINT32 clusterSize;
    UINT32 lastIdx1Size;
    UINT32 lastPadSize;
    UINT16 saveVF;
    UINT16 rev;
} AVIREC_FILEOBJ;

typedef struct
{
    UINT32 padStartAddr;
    UINT32 oldSize;
    UINT32 thisPadSize;
    UINT32 oldVidFrame;
    UINT32 oldAudFrame;
} AVIREC_PADINFO;
//Functions
extern ER MP_AVIReadLib_RegisterObjCB(void *pobj);


extern ER MP_AviWriteLib_Initialize(void);
extern ER MP_AviWriteLib_SetMemBuf(UINT32 startAddr, UINT32 size);
extern UINT32 MP_AviWrite_MakeHeader(UINT8 *pAviHeader, MEDIAREC_OBJ *pRecParam);
extern ER MP_AviWriteLib_RegisterObjCB(void *pobj);
extern ER MP_AviWriteLib_MakeHeader(UINT32 hdrAddr, UINT32 *pSize, void *pobj);
extern ER MP_AviWriteLib_GetInfo(MEDIAWRITE_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
extern ER MP_AviWriteLib_SetInfo(MEDIAWRITE_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
extern ER MP_AviWriteLib_UpdateHeader(UINT32 type, void *pobj);
extern void MP_AVIUti_ShowAllSize(void);

#endif

