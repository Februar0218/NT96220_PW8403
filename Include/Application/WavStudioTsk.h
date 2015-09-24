/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       WavStudioTsk.h
    @ingroup    mIAPPWAV

    @brief      Header file of WavStudio task.

                APIs/structure/macro declaration of WavStudio task.

    @note       Nothing.

*/

/**
    @addtogroup mIAPPWAV
*/
//@{

#ifndef _WAVSTUDIOTSK_H
#define _WAVSTUDIOTSK_H

#include "OSCommon.h"
#include "dcAVCodec.h"
#include "Audio.h"
#include "WAVLib.h"

/// Recording time infinite
#define WAVSTUDIO_MAX_RECTIME_INFINITE      0xFFFFFFFF

//#NT#2008/04/28#Chris Hsu -begin
//#NT#Move to internal header file
/// Default recording time (30 Secs)
//#define WAVSTUDIO_MAX_RECTIME_DEFAULT       30
//#NT#2008/04/28#Chris Hsu -end

/// Create new WAV file automatically based on DCF naming rule
#define WAVSTUDIO_DIR_FILE_AUTO             WAV_DIR_FILE_AUTO

//#NT#2008/04/28#Chris Hsu -begin
//#NT#Move to internal header file
/// Minimum memory requirement to run WAV studio task
//#define WAVSTUDIO_MIN_MEMORY_REQ            0x00100000  // 1MB
//#NT#2008/04/28#Chris Hsu -end

/**
    @name Wave file type
*/
//@{
#define WAVSTUDIO_FILE_SINGLE               0   ///< WAV file type standalone in separate wave file
#define WAVSTUDIO_FILE_EMBEDDED             1   ///< WAV file type embedded in JPEG EXIF header
// Not consist with other definition, don't use any more
#define WAV_FILE_SINGLE                     WAVSTUDIO_FILE_SINGLE       ///< Obsoleted
#define WAV_FILE_EMBEDDED                   WAVSTUDIO_FILE_EMBEDDED     ///< Obsoleted
//@}

/**
    @name Wave studio event IDs
*/
//@{
#define WAVSTUDIO_EVENT_RECORDING           0x00000001      ///< Start to record
#define WAVSTUDIO_EVENT_PLAYING             0x00000002      ///< Start to play
#define WAVSTUDIO_EVENT_STOPPED             0x00000004      ///< Stopped. (If in recording mode all data is saved)
#define WAVSTUDIO_EVENT_PAUSED              0x00000008      ///< Paused
#define WAVSTUDIO_EVENT_RESUMED             0x00000010      ///< Resumed
#define WAVSTUDIO_EVENT_SAVING              0x00000020      ///< Stop recording and save data in RAM to card
#define WAVSTUDIO_EVENT_DURATION            0x00000040      ///< 1 second passed
#define WAVSTUDIO_EVENT_DISKFULL            0x00000080      ///< Disk is full
#define WAVSTUDIO_EVENT_BUFFULL             0x00000100      ///< Buffer is full
#define WAVSTUDIO_EVENT_INVALIDFILE         0x00000200      ///< Not a valid file when playing
#define WAVSTUDIO_EVENT_RWERROR             0x00000400      ///< Read/Write error
//@}

/**
    @name Wave studio task status
*/
//@{
#define WAVSTUDIO_STS_CLOSED                0x00000001      ///< Task is not opened
#define WAVSTUDIO_STS_STOPPED               0x00000002      ///< Stopped (No recording or playing)
#define WAVSTUDIO_STS_RECORDING             0x00000004      ///< Recording now
#define WAVSTUDIO_STS_PLAYING               0x00000008      ///< Playing now
#define WAVSTUDIO_STS_PAUSED                0x00000010      ///< Paused
#define WAVSTUDIO_STS_SAVING                0x00000020      ///< Saving remaining data in RAM to card
//@}

/**
    Wave studio event callback function prototype
*/
typedef void (*WAVSTUDIO_CB)(UINT32);

/**
    Wave studio data format
*/
typedef struct
{
    //#NT#2008/04/28#Chris Hsu -begin
    //#NT#Support 8 bits per sample PCM
    UINT32          uiBitsPerSample;    ///< BitsPerSample. PCM could be 8 or 16, ADPCM is always 4
    //#NT#2008/04/28#Chris Hsu -end
    AUDIO_CODEC     AudioCodec;         ///< Audio codec type
    AUDIO_SR        AudioSamplingRate;  ///< Audio sampling rate
    AUDIO_CH        AudioChannel;       ///< Audio Channel selection
    //#NT#2009/10/21#Cliff Lin -begin
    //#Add new parameters
    AUDIO_VOL       AudioVolume;
    AUDIO_GAIN      AudioGain;
    //#NT#2009/10/21#Cliff Lin -end

#if (WAVFILE_ASF_ENABLE == ENABLE)
    WAVFILETYPE     FileFormat;
#endif
} WAVSTUDIO_FMT, *PWAVSTUDIO_FMT;

/**
    Wave studio duration record
*/
typedef struct
{
    volatile UINT32 uiElapsed;      ///< Elapse time
    volatile UINT32 uiRemaining;    ///< Remain time
} WAVSTUDIO_DURATION, *PWAVSTUDIO_DURATION;

/**
    Wave studio memory allocation
*/
typedef struct
{
    UINT32          uiMemoryAddr;   ///< Memory address for wave record/playback
    UINT32          uiMemorySize;   ///< Memory size for wave record/playback
    WAVSTUDIO_CB    WavStudioCB;    ///< Wave studio record/playback callback
} WAVSTUDIO_APPOBJ, *PWAVSTUDIO_APPOBJ;

/**
    Wave studio file information of DCF format
*/
typedef struct
{
    UINT32          uiDirID;    ///< DCF directory ID
    UINT32          uiFileID;   ///< DCF file ID
    UINT32          uiType;     ///< Wave file type
                                ///< - @b WAVSTUDIO_FILE_SINGLE
                                ///< - @b WAVSTUDIO_FILE_EMBEDDED
} WAVSTUDIO_FILEINFO, *PWAVSTUDIO_FILEINFO;

// Operations (APIs)
// General Operations
extern ER       WavStudio_Open(PWAVSTUDIO_APPOBJ pWavObj);
extern ER       WavStudio_Close(void);
extern UINT32   WavStudio_GetStatus(void);

// Application Operations
extern BOOL     WavStudio_Record(void);
extern BOOL     WavStudio_Play(void);
extern BOOL     WavStudio_Stop(void);
extern BOOL     WavStudio_Pause(void);
extern BOOL     WavStudio_Resume(void);

extern void     WavStudio_GetDuration(PWAVSTUDIO_DURATION pWavDuration);
extern BOOL     WavStudio_SetFile(PWAVSTUDIO_FILEINFO pWavFileInfo);
//#NT#2008/09/01#Chris Hsu -begin
//#NT#Export delete file API
extern BOOL     WavStudio_DelFile(PWAVSTUDIO_FILEINFO pWavFileInfo);
//#NT#2008/09/01#Chris Hsu -end
extern BOOL     WavStudio_SetFormat(PWAVSTUDIO_FMT pWavFmt);
extern void     WavStudio_SetMaxRecTime(UINT32 uiSecond);
//#NT#2008/04/28#Chris Hsu -begin
//#NT#Support user data in header
extern void     WavStudio_SetUserData(UINT32 uiDataAddr, UINT32 uiDataSize);
//#NT#2008/04/28#Chris Hsu -end

//#NT#2009/10/21#Cliff Lin -begin
//#NT#Add new APIs for playback volume and record gain
extern void WavStudio_InitVol(AUDIO_VOL uiAudioVolume);
extern void WavStudio_InitGain(AUDIO_GAIN uiAudioGain);
//#NT#2009/10/21#Cliff Lin -end
//#NT#2009/12/02#Cliff Lin -begin
//#NT#Support to parse information from WAV file header
extern BOOL WavStudio_ParseHeader(PWAV_FILEINFO pWAVFileInfo);
//#NT#2009/12/02#Cliff Lin -end
#endif

//@}
