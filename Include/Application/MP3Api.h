/*++

Copyright (c) 2005  Novatek Microelectronics Corporation

Module Name:
    MP3Api.h

Abstract:
    MP3 Playback mode

Environment:
    For nt96610

Notes:
    Copyright (c) 2005 Novatek Microelectronics Corporation.  All Rights Reserved.

Revision History:
    2008/08/08: Created by Meg Lin
--*/

#ifndef _MP3API_H
#define _MP3API_H


#define MP3OPENERR_BLOCKSIZETOOSMALL  0x01

#define MP3FILEREADMODE_BYINDEX   0x01
#define MP3FILEREADMODE_SETORDER  0x02
#define MP3FILEREADMODE_SETINDEX  0x04

#define MP3FILEREADMODE_BYNAME    0x80



#define SEARCHTYPE_MP3      0x01
#define SEARCHTYPE_LRC      0x02
#define SEARCHTYPE_WAV      0x02
#define SEARCHTYPE_JPG      0x04
#define SEARCHTYPE_DIR      0x08
#define SEARCHTYPE_WMA      0x10
#define SEARCHTYPE_ASF      0x20
#define SEARCHTYPE_AVI      0x40
#define SEARCHTYPE_UNKNOWN  0x80

#define READFILE_NEXT       0x01
#define READFILE_PREV       0x02
#define READFILE_CURR       0x04

//#NT#2007/07/31#Meg Lin -begin
//add for drawing playing seconds

#define     MP3PLAY_SETTIME_NORMAL      0x0001
#define     MP3PLAY_SETTIME_FORWARD     0x0002
#define     MP3PLAY_SETTIME_BACKWARD    0x0004
#define     MP3PLAY_SETTIMG_FFBW_RESUME     0x0008
#define     MP3PLAY_SETTIME_REPEATA         0x0010
#define     MP3PLAY_SETTIME_REPEATB         0x0020
#define     MP3PLAY_SETTIME_REPEATCANCEL    0x0040
//#NT#2007/07/31#Meg Lin -end
//-------------------------------------------------------
// Status define
//-------------------------------------------------------
#define     MP3PLAY_START           4
#define     MP3PLAY_PLAYING         5
#define     MP3PLAY_STOP            6
#define     MP3PLAY_PAUSE           7
#define     MP3PLAY_DECODE_ERR      8
#define     MP3PLAY_DECODE_FINISH   9
#define     MP3PLAY_PLAYING_FINISH  10
#define     MP3PLAY_SLOWLY          11
#define     MP3PLAY_FORWARD         12
#define     MP3PLAY_BACKWARD        13
#define     MP3PLAY_FORWARDEND      14

#define     MP3PLAY_CURRTRACK        1
#define     MP3PLAY_NEXTTRACK        2
#define     MP3PLAY_PREVTRACK        3

#define     MP3PLAY_REPEAT_OFF      0
#define     MP3PLAY_REPEAT_ON       0x01
#define     MP3PLAY_REPEAT_A        0x02
#define     MP3PLAY_REPEAT_B        0x04

#define MP3PLAYFILE_MODE_SET    0x01
#define MP3PLAYFILE_MODE_CLR    0x02
#define MP3PLAYFILE_MODE_WAIT   0x04

#define MP3PLAYFILE_FLAG_INIT1STOK  0x10000000 //FLGMP3INIT_READFIRSTOK
#define MP3PLAYFILE_FLAG_NEXYLYC    0x08000000 //FLGMP3READ_NEXTLYC
#define MP3PLAYFILE_FLAG_FSIDLE     0x20000000 //FLGMP3_FILESYSTEM_IDLE


#define MP3READNEXTFILE     0
#define MP3READPREVFILE     1
#define MP3READCURRFILE     2

#define MP3CALLBACK_FINISH      1 //finish, no param
#define MP3CALLBACK_ONESECOND   2 //one second, param = second


//#NT#2007/09/11#Meg Lin -begin
//add for nand reserved mp3
typedef ER (*PREDMP3GETSIZE_CB)(UINT16 id, UINT32 *pSize);
typedef ER (*PREDMP3READDATA_CB)(UINT16 id, UINT8 *buff, UINT32 *pBufSize, UINT32 filepos);
//#NT#2007/09/11#Meg Lin -end
typedef void (Mp3CallbackType)(UINT32 event_id, UINT32 param);


typedef struct{
    UINT8 *indexTableAddr;
    char  *fullPath;
    PREDMP3GETSIZE_CB  fpPredMp3GetSize;
    PREDMP3READDATA_CB fpPredMp3ReadData;
    //ER      (*fpPredMp3GetSize)(UINT16 id, UINT32 *pSize);
    //ER      (*fpPredMp3ReadData)(UINT16 id, UINT8 *buff, UINT32 *pBufSize, UINT32 filepos);
    UINT8 FileReadMode;     //MP3FILEREADMODE_BYINDEX |MP3FILEREADMODE_SETORDER
    UINT8 NeedInitIndexTable;
    UINT8 NeedResetIndexTable;
    UINT8 rev;
    UINT16 nandRevFileNum;
    UINT16 readModeParam;//index or order

} MP3PLAYFILE_PARAM;
typedef struct{
    UINT32 Mp3PlayBufAddr;
    UINT32 Mp3PlayBufSize;  //minimum : 0xD4800
    Mp3CallbackType *CallbackFunc;

    //no need to set
    UINT32 AudioBufferSize;
    UINT32 FileBufferSize;
    UINT32 TwoFrameSize;
    UINT32 AddrIdxTableSize;
    UINT32 framePerBlock;
    
} MP3PLAYAPI_INITPARAM;
typedef struct{
    UINT8       ucMP3CurrStatus;        // (O)  MP3 player curr status
    UINT8       ucCurrentFileBufIndex;  // (O)  Curr using MP3 file buffer index
    UINT8       ucNextReadFileBufIndex; // (O)  Next using MP3 file buffer index
    UINT8       ucCurrentFileBufNums;   // (I/O)Readed file buffer nums
    UINT8       ucMP3PlayVolume;        // (I)  Volume setting
    UINT8       ucMP3PlayRepeatOn;      // (I)  Repeat A-B mode enable
    UINT8       ucMP3AudioEffect;       // (I)  Audio effect setting
    UINT8       ucReserved;

    UINT32      uiMP3PlayFileBuf[4];    // (O)  MP3 file buffer addr
    UINT32      uiMP3PlayLRCFileBuf;    // (O)  LRC file buffer addr
    UINT32      uiMP3PlayFileSize;      // (I)  Playing MP3 file size
    UINT32      uiMP3RemainFileSize;    // (I)  Remain file size
    UINT32      uiMP3PlayBitRate;       // (O)  MP3 file bit-rate
    UINT32      uiMP3PlaySampleRate;    // (O)  MP3 file sample-rate
    UINT32      uiMP3PlayTotalSecs;     // (O)  MP3 file total seconds
    //#NT#2007/12/27#Meg Lin -begin
    //modify for seconds showing
    UINT32      uiMP3FrameDataLength;   // (O)  MP3 frame data length 0x1200
    //#NT#2007/12/27#Meg Lin -end
    UINT32      uiMP3PlayXrBuf;         // (O)  MP3 frame spectrum//2007/10/31 Meg add

    UINT16      usMP3PlayCurrSecond;    // (I/O)  Playing MP3 curr second
    UINT16      usMP3PlayRptSecond_A;   // (I)  Repeat playing second A
    UINT16      usMP3PlayRptSecond_B;   // (I)  Repeat playing second B
    UINT16      usReserved;

} MP3PLAY_INFO, *PMP3PLAY_INFO;

extern void     MP3PlayOpen(MP3PLAYAPI_INITPARAM *pParam);
extern void     MP3PlayClose(void);

extern PMP3PLAY_INFO MP3PlayApi_GetStatusPtr(void);
extern void     MP3PlayApi_SetDefaultVolume(UINT32 Volume);
extern void     MP3PlayApi_SetPlayingSecond(UINT16 second, UINT16 type);


extern INT32    MP3PlayFileRead_Open(MP3PLAYFILE_PARAM *pParam);
extern void     MP3PlayFileRead_Close(void);
extern void     MP3PlayFileRead_Close_NoCloseFile(void);
extern void     MP3PlayFileApi_ReadFileByOrder(UINT32 order, UINT8 dir);
extern UINT32   MP3PlyaFileApi_ResetTableAndGetMp3Num(void);
extern UINT32   MP3PlayFileApi_GetLRCSize(void);
extern void     MP3PlayFileApi_ResetGlobalTable(void);
extern void     MP3PlayFileApi_FlagCmd(UINT8 mode, UINT32 flag);
extern UINT32   MP3PlayFileSys_SetNowOrder(UINT32 order);
extern UINT32   MP3PlayFileSys_GetFileIndex(UINT32 order);
extern UINT32   MP3PlayFileSys_GetNowOrder(void);
extern UINT32   MP3PlayFileSys_GetMp3TotalFiles(void);
extern void     MP3PlayFile_SetNowIndexToRead(UINT32 index);
extern UINT32   MP3PlayFileSys_GetType(UINT32 indexNow);
extern void     MP3PlayFile_ReadToNextFile(UINT8 mode);

extern void MP3PlayFileApi_GetShortName(UINT32 order, char *fullPath);
extern void MP3PlayFileApi_GetLongName(UINT32 order, UINT16 *longNameAddr);

//Mp3PlayTsk.c
extern UINT8    MP3Play_GetMP3CurrStatus(void);
extern void     MP3Play_StartOrResumePlaying(void);
extern void     MP3Play_StartPlaying(void);
extern UINT32   MP3Play_GetActualPlayingSeconds(UINT32 order, UINT32 addr, UINT32 size);
extern UINT32   MP3Play_Parse1stFrameToGetSecond(UINT32 order, UINT32 addr, UINT32 size);//2010/01/04 Meg Lin

#endif // _MP3API_H
