/**
    Header file of movie interface

    movie interface

    @file       MovieInterface.h
    @ingroup    mIMEDIAWRITE
    @note       Nothing.
    @version    V1.01.001
    @date       2011/04/11

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MOVIEINTERFACE_H
#define _MOVIEINTERFACE_H

#include "type.h"

#define MOVIEREC_AVIREC     0
#define MOVIEREC_MEDIAREC   1

#define MOVIEREC_JPGFMT_444  1
#define MOVIEREC_JPGFMT_422  2
#define MOVIEREC_JPGFMT_420  3

#define MOVIEREC_RECFORMAT_VIDEO_ONLY       1
#define MOVIEREC_RECFORMAT_AUD_VID_BOTH     2

#define MOVIEREC_1STQP_BEST     1280
#define MOVIEREC_1STQP_NORMAL   2560
#define MOVIEREC_1STQP_DRAFT    6400

#define MOVIEREC_ENDTYPE_CUT    1
#define MOVIEREC_ENDTYPE_NORMAL 2

typedef void (MovieRecCallback)(UINT32 event_id, UINT32 e_value);


#define MOVIEREC_CHGTYPE_WIDTH      1 ///< p1[in]: width
#define MOVIEREC_CHGTYPE_HEIGHT     2 ///< p1[in]: height
#define MOVIEREC_CHGTYPE_VFR        3 ///< p1[in]: video frame rate
#define MOVIEREC_CHGTYPE_1ST_FR_Q   4 ///< p1[in]: 1st video frame QP
#define MOVIEREC_CHGTYPE_TBR        5 ///< p1[in]: target bit rate, size of one frame
#define MOVIEREC_CHGTYPE_JPGFMT     6 ///< p1[in]: jpeg format, MOVIEREC_JPGFMT_420 or others
#define MOVIEREC_CHGTYPE_ENDTYPE    7 ///< p1[in]: endtype, MOVIEREC_ENDTYPE_CUT or others


#define MOVIEREC_EVENT_RESULT_NORMAL        1    ///< Finish normally
#define MOVIEREC_EVENT_RESULT_HW_ERR        2    ///< Hardware error
#define MOVIEREC_EVENT_RESULT_FULL          3    ///< Card full
#define MOVIEREC_EVENT_RESULT_SLOW          4    ///< Write card too slowly
#define MOVIEREC_EVENT_RESULT_NOTREADY      5    ///< Not ready
#define MOVIEREC_EVENT_ONE_SECOND           6    ///< One second arrives
#define MOVIEREC_EVENT_RESULT_OVERTIME      7    ///< Overtime
#define MOVIEREC_EVENT_RESULT_OVERWRITE     8    ///< Buffer overwrite
#define MOVIEREC_EVENT_RESULT_INVALIDFILE   9    ///< SAVE INVALIDFILE
#define MOVIEREC_EVENT_RESULT_WRITE_ERR    10
#define MOVIEREC_EVENT_ONE_FRAME           21    ///< encode one frame, //2011/07/28 Meg Lin
#define MOVIEREC_EVENT_START_VID           22

typedef struct
{
    UINT32              uiMemAddr;          ///< starting address for recording
    UINT32              uiMemSize;          ///< size for recording
    MovieRecCallback    *CallBackFunc;      ///< event inform callback
    UINT32              uiRecFormat;        ///< recording type, MEDIAREC_AUD_VID_BOTH
    UINT32              uiVidFrameRate;     ///< recording video frame rate, MEDIAREC_VRATE_30
    UINT32              ui1stFrameQp;       ///< recording video quality
    UINT32              uiCBthreshold;      ///< video frames for callback, usually equals to frame rate
    UINT32              uiMaxWidth;         ///< recording max video width, 1440x1088 or 1280x720
    UINT32              uiMaxHeight;        ///< recording max video height
    UINT32              uiWidth;            ///< now recording video width, 1440x1088/1280x720/640x480/320x240
    UINT32              uiHeight;           ///< now recording video height
    UINT32              uiJpegFormat;
    UINT32              uiAudSampleRate;    ///< recording audio sample rate, AUDIO_SR_8000
    UINT32              uiAudBits;          ///< audio setting, bits per sample, 8 or 16
    UINT32              uiAudChannels;      ///< audio setting, channels, 1 or 2
    UINT32              audioCh;            ///< audio recording channel, AUDIO_CH_LEFT or others//2009/12/22 Meg
    UINT16              ucSeamlessSec;      ///< seamless cut second,
    UINT8               ucEngType;          ///< engine, MOVIEREC_AVIREC, MOVIEREC_MEDIAREC
    UINT8               ucEndType;          ///< end type, CUTOVERLAP, NORMAL
    UINT32              uiNoiseFilterLevel;       // 0: close noise filter
    UINT32              uiNoiseFilterNoiseDecay;  //0: noise*(1/2), 1: noise*(1/4), 2: noise*(1/8)
    UINT8               ucSaveFrameNum; //if memory enough, can be frame number. else, 3,5,6,10,15,30
    UINT8               rev;
    UINT32              uiUserDataAddr;
    UINT32              uiUserDataSize;
    UINT32              uiTargetFramesize;
    UINT32              uiAudioBufferAddr;
    UINT32              uiAudioBufferSize;
    UINT32              uiMaxRecSecs;
    UINT32              uiPanelWidth;       ///< input: panel size, 2011/06/10 Meg
    UINT32              uiPanelHeight;      ///< input: panel size, 2011/06/10 Meg

} MOVIEREC_OBJ;




extern ER MovRec_Open(MOVIEREC_OBJ *pObj);
extern ER MovRec_Close(void);
extern ER MovRec_ChangeParam(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);
extern ER MovRec_Record(void);
extern ER MovRec_Stop(UINT8 ucWait);
extern ER MovRec_ResetMemoryValue(UINT32 addr, UINT32 size);//2011/06/15 Meg Lin

extern UINT32 MovRec_Disk2Second(void);
extern UINT32 MovRec_CheckUnsavedFileBlocks(void);
extern void MovRec_MinusFilesize(UINT32 size);








#endif
