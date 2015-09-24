/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       MOVLib.h
    @ingroup    mIAVMOV

    @brief      MOV file reader/writer utility functions


    @note       Nothing.

*/

#ifndef _MOVLIB_H
#define _MOVLIB_H

#include "dcAVCodec.h"
#include "FileDB.h"
#include "MOVUtilities.h"


#define MOV_WITHMPEG4       1

//#NT#2008/11/24#Chris Hsu -begin
//#NT#FilesysWaitCmdFinish(FST_TIME_INFINITE) always return FST_STA_OK now.
//#NT#We have to call FilesysStatus() to get command status.
#define MOVWriteWaitPreCmdFinish()  {\
                                        FilesysWaitCmdFinish(FST_TIME_INFINITE);\
                                        if (FilesysStatus() != FST_STA_OK)\
                                        {\
                                            debug_err(("MOV write file error at Pos: 0x%X\r\n", gMOVFilePos));\
                                            FilesysCloseActFile();\
                                            return MOV_STATUS_WRITE_ERR;\
                                        }\
                                    }
//#NT#2008/11/24#Chris Hsu -end

#define MOV_PLAYDEBUG  0
#define MOV_RECDEBUG   0

#define MOV_STATUS_OK               0
#define MOV_STATUS_INVALID_PARAM    1
#define MOV_STATUS_WRITE_ERR        2
#define MOV_STATUS_READ_ERR         3
#define MOV_STATUS_ENTRYADDRERR     4

#define MOV_DEC_FIRSTFRAME_ERR_MOVFORMAT 0x01

#define MOV_MDAT_UNUSED_BLOCK       2//2007/05/22 the first two block is unused

#define MOV_MOOVERROR_MDATSIZEERROR  0x00000001
#define MOV_MOOVERROR_MDATNOFOUND    0x00000002
#define MOV_MOOVERROR_MOOVNOFOUND    0x00000004
#define MOV_MOOVERROR_IMAGESIZEERROR 0x00000008
#define MOV_MOOVERROR_MDATNOTMP4S    0x00000010

#define MOV_MDAT_HEADER   8
#define MOV_MDAT_TEMP     0xFF8
#define MOV_FIRST_AUDIOPREROLL  0//2007/06/01 0x5800


#define MOV_SINGLE_PAYLOAD          1
#define MOV_MULTI_PAYLOAD           2
#define WTMOV_StreamNumVideo        0x0001
#define WTMOV_StreamNumAudio        0x0002

#define MOV_PACKET_ERR_HEADER       0x04
#define MOVSeekFile(Pos)                                {\
                                                            FilesysWaitCmdFinish(FST_TIME_INFINITE);\
                                                            gMOVFilePos = (Pos);\
                                                        }

#define MOV_MDAT_BASETIME  0xBC191380;//base 2004 01 01 0:0:0

typedef struct _MOVINDEXENTRY
{
    UINT32 PacketNumber;
    UINT32 PacketCount;
    UINT32 PacketSize;

} MOVINDEXENTRY;

typedef __packed struct {
   UINT64      ObjSize;                    //8
   UINT64      IndexEntryTimeInterval;     //8
   UINT32      MaxPacketCount;             //4
   UINT32      IndexEntriesCount;          //4
   //struct size = (at least 56)
   //
   //INT8      IndexEntries;               // varies
   //
}MOVINDEXOBJ, *PMOVINDEXOBJ;


typedef struct
{
    UINT8          FrameResolution;
    UINT8          VideoQuality;
    UINT16         rev;
    UINT32         TimeResolution;
    UINT32              BufHeaderLen;
    UINT32              BufOffsetLen;
    UINT32              BufLen;
    UINT32              BufOffsetCnt;
    UINT32          DataPacketSize;//4K

    VIDEOCODEC          decVideoCodec;
    AUDIOCODEC          decAudioCodec;

    VIDEO_CODEC         encVideoCodec;
    AUDIO_CODEC         encAudioCodec;
    UINT32              Preroll;
    UINT16              PacketPayloadType;
    UINT16              rev2;
    UINT32              TotalAudioSampleCount;
    UINT16              AudioSampleRate;
    UINT16              AudioChannel;
    UINT32              AudioBitsPerSample;
    UINT16              ImageWidth;             ///< [Out]     : mov Image Width
    UINT16              ImageHeight;            ///< [Out]     : mov Image Height
    UINT32              VideoPayloadSize;
    UINT32              AudioPayloadSize;
    UINT32              CreationDate;
    UINT32              NowFilePos;
    UINT64              IndexEntryTimeInterval; ///< [In/Out]  : Key frame Time Inerval
    UINT64              DataPacketsCount;
    UINT64              PlayDuration;
    UINT64              FileSize;
} MOV_FILE_INFO, *PMOV_FILE_INFO;
typedef struct
{
   UINT16      MediaType;              ///< [IN]      : Video or Audio
   UINT16      PacketCount;            ///< [OUT]     : Data packet Counts
   UINT32      VideoPresentationTime;  ///< [IN]      : VideoMedia Presentation Time
   UINT32      VOPType;                ///< [IN/OUT]  : I-vop(0) or P-VOP(1)
   UINT32      AudioPresentationTime;  ///< [IN]      : AudioMedia Presentation Time
   UINT32      PresentationTime;       ///< [IN]      : Media Presentation Time
   UINT32      ObjSize;                ///< [IN]      : Media Object Size
   UINT32      SendTime;               ///< [IN/OUT]  : [In]  Media Begin Send Time(system times) ,
                                       //               [Out] Media End Send Time(evalue).
   UINT32      ReadFilePos;
   UINT8       Sequence;               ///< [In/Out]  : IN: Begind Sequence Number, Out: End of Sequence
   UINT8       AudioMediaObjNum;       ///< [Out]     : Audio Media Object IndexNumber
   UINT8       VideoMediaObjNum;       ///< [Out]     : Video Media Object IndexNumber
   UINT8       Status;                 //

   UINT16      AudioTimeInterval;      ///< [IN]      :
   UINT16      VideoTimeInterval;      ///< [IN]      :
   INT8*       pDataBuf;               ///< [IN]      : Media Object data buffer address

} MOV_MEDIA_OBJ, *PMOV_MEDIA_OBJ;
typedef struct
{
    UINT8          FrameResolution;
    UINT8          VideoQuality;
    UINT16         rev;
    UINT32         TimeResolution;
    UINT32              BufHeaderLen;
    UINT32              BufOffsetLen;
    UINT32              BufLen;
    UINT32              BufOffsetCnt;
    UINT32          DataPacketSize;//4K

    VIDEOCODEC          decVideoCodec;
    AUDIOCODEC          decAudioCodec;

    VIDEO_CODEC         encVideoCodec;
    AUDIO_CODEC         encAudioCodec;
    UINT32              Preroll;
    UINT16              PacketPayloadType;
    UINT16              rev2;
    UINT32              TotalAudioSampleCount;
    UINT16              AudioSampleRate;
    UINT16              AudioChannel;
    UINT32              AudioBitsPerSample;
    UINT16              ImageWidth;             ///< [Out]     : mov Image Width
    UINT16              ImageHeight;            ///< [Out]     : mov Image Height
    UINT32              VideoPayloadSize;
    UINT32              AudioPayloadSize;
    UINT32              CreationDate;
    //UINT32              NowFilePos;
    UINT32              fileOffset;
    UINT32              audioFrameCount;
    UINT32              videoFrameCount;
    UINT32              totalMdatSize;
    UINT8               *ptempMoovBufAddr;
    UINT32              audioTotalSize;
    UINT64              IndexEntryTimeInterval; ///< [In/Out]  : Key frame Time Inerval
    UINT64              DataPacketsCount;
    UINT64              PlayDuration;

    //UINT64              FileSize;
} MOVMJPG_FILEINFO;


typedef struct _MOVMJPG_HEADINFO
{
//#NT#2007/04/04#KH Chang -begin
//#Add for keeping AVI file start address
    UINT32  uiFileStartAddr;        // AVI file start address
//#NT#2007/04/04#KH Chang -end
    UINT32  uiFirstVideoOffset;     // first video frame offset
    UINT32  uiFirstAudioOffset;     // first audio frame offset
    UINT32  uiIndexChunkOffset;     // index chunk offset
    UINT32  uiTotalFileSize;        // total file size
    UINT32  uiTotalFrames;          // total frames, video + audio
    UINT32  uiTotalSecs;           // total seconds
    UINT32  uiVidWidth;             // video width
    UINT32  uiVidHeight;            // video height
    UINT32  uiVidScale;             // video scale     (66666)
    UINT32  uiVidRate;              // video rate    (1000000)
    UINT32  uiAudSampleRate;     // audio sample rate
    UINT32  uiAudBytesPerSec;       // audio bytes per second
    UINT32  uiAudWavTotalSamples;   // audio all samples
    UINT8   ucAudBitsPerSample;     // audio bits per sample
    UINT8   ucAudChannels;          // audio channels
    BOOL    bAudioSupport;          // with audio data
    BOOL    bVideoSupport;          // with video data
} MOVMJPG_HEADINFO, *PMOVMJPG_HEADINFO;
//#NT#2007/03/28#Corey Ke -begin
//#Enable MOV to play all files without DCF

//==============================================================================
// Global MOV File DB Info
//==============================================================================
typedef struct _MOV_FileDBInfo_{
    UINT8               uiPlayCommand;          ///< MOV_PLAY_CURR / MOV_PLAY_NEXT / MOV_PLAY_PREV
    BOOL                bPlayAllFiles;          ///< TRUE / FALSE
    FILEDB_HANDLE       MovFileHandle;          ///< File handle
} MOV_FILEDB_INFO,*PMOV_FILEDB_INFO;
//#NT#2007/03/28#Corey Ke -end
//#NT#2007/03/28#Corey Ke -begin
//#Enable ASF to play all files without DCF
extern void MOVInitFileDB(PMOV_FILEDB_INFO pMOVDBInfo);
//#NT#2007/03/28#Corey Ke -end

extern UINT32  guiAudioPacketIndex;
extern UINT32  guiPacketIndex;
extern MOV_Ientry                          *gp_movEntry;
extern MOV_Ientry                          *gp_movAudioEntry;
extern MOV_Context                         g_movCon;
extern MOV_Context                         g_movAudioCon;
extern UINT32                              g_movEntryMax;
extern UINT32                              g_movAudioEntryMax;
extern MOV_Track                           g_movTrack, g_movAudioTrack;


extern UINT32 MOVWriteHeader(PMOV_FILE_INFO pMOVInfo);
extern UINT32 MOVUpdateHeader(PMOV_FILE_INFO pMOVInfo);
extern UINT32 MOVWriteVideoMulti(PMOV_FILE_INFO pMOVInfo, PMOV_MEDIA_OBJ pMediaObj, UINT32 *puiPacketRemSize);
extern void MOV_Write_SetEntryAddr(UINT32 addr, UINT32 size);

extern UINT8 ReadMOVHeaderObjs(MOV_FILE_INFO *pMovInfo);
extern UINT32 MOVReader_FirstPacket(PMOV_FILE_INFO pMovInfo, PMOV_MEDIA_OBJ pMovMediaObj);
extern UINT32 MOVReaderM(PMOV_FILE_INFO pMOVInfo, PMOV_MEDIA_OBJ pMovMediaObj);
extern UINT32 MOVPreLoadRead(UINT8* BufAddr, UINT32 BufSize);
extern void MOVReadFile(UINT8* BufAddr, UINT32 BufSize);

extern volatile UINT32                     gMOVFilePos;
extern UINT32 gMOVTempPacketBufBase;
extern void MOV_Read_SetEntryAddr(UINT32 addr, UINT32 size);

extern UINT32 MOVWriteAudioMulti(PMOV_FILE_INFO pMovInfo, PMOV_MEDIA_OBJ pMediaObj, UINT32 *puiPacketRemSize);
extern UINT32 MOVWritePaddingMulti(UINT32 *puiPacketRemSize);

//#NT#2007/09/06#Meg Lin -begin
//fixbug: if video > audio, add pure audio frame
extern UINT32 MOVWriteAudioOnePacket(PMOV_FILE_INFO pMovInfo, PMOV_MEDIA_OBJ pMediaObj, UINT32 *puiPacketRemSize);
extern UINT32 MOVWritePaddingOnePacket(UINT32 *puiPacketRemSize, UINT32 *pthisAudiosize);//2007/09/07
extern UINT32 MOVReaderM_Audio(PMOV_FILE_INFO pMOVInfo, PMOV_MEDIA_OBJ pMovMediaObj);
//#NT#2007/09/06#Meg Lin -end

//extern UINT32 MOVUpdateAudioHeader(PMOV_FILE_INFO pMOVInfo);
extern UINT32 MOV_Write_GetMoovAtomSize(void);
extern void MOVDec2_Test(UINT32 addr, UINT32 count);
extern void MOVRead_GetMaxIFrameSize(UINT32 *psize);
extern UINT32 MOVRead_GetIFrameTotalPacketCnt(void);
extern UINT32 MOVRead_GetSecond2FilePos(UINT32 seconds);
extern UINT32 MOVReader_PacketPos2PacketSize(PMOV_FILE_INFO pMovInfo, PMOV_MEDIA_OBJ pMovMediaObj);

extern void MOV_SetIentry(UINT32 index, MOV_Ientry *ptr);
extern void MOV_SetAudioIentry(UINT32 index, MOV_Ientry *ptr);
extern UINT32 MOVMJPG_UpdateHeader(MOVMJPG_FILEINFO *pMOVInfo);
extern UINT32 MOVMJPG_GetMoovAtomSize(UINT32 sec);
extern void MOVRead_ParseHeader(UINT32 addr, MOVMJPG_HEADINFO *pHeader);
extern void MOVRead_GetVideoPosAndOffset(UINT32 findex, UINT32 *pos, UINT32 *size);
extern void MOVRead_GetAudioPosAndOffset(UINT32 findex, UINT32 *pos, UINT32 *size);

#endif