/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       PlaySoundInt.h
    @ingroup    mIPRJAPKey

    @brief      Internal header file of Play Sound Task
                Internal Header file of task handles the sound playback of startup, keypad ...

    @note       Nothing.

    @date       2006/01/23
*/

/** \addtogroup mIPRJAPKey */
//@{

#ifndef _PLAYSOUNDINT_H
#define _PLAYSOUNDINT_H

#include "Type.h"
#include "WAVLib.h"

#define FLGSOUND_STOP               0x00000001
#define FLGSOUND_PLAY               0x00000002
#define FLGSOUND_STOPPED            0x00000004
#define FLGSOUND_ALL                0xFFFFFFFF

#define PLAYSOUND_STS_STOPPED       0
#define PLAYSOUND_STS_PLAYING       1

#define PLAYSOUND_WAVHEADER_BUF_SIZE    0x00000030  //48 bytes (PCM Wave Header: 44 bytes, must larger than this!)

extern volatile UINT32              g_uiPlaySoundSound;
extern volatile UINT32              g_uiPlaySoundStatus;

extern void PlaySound_AudioHdl(UINT32 uiEventID);
extern BOOL PlaySound_ParseWAVHeader(UINT32 uiSoundDataAddr, PWAV_FILEINFO pRtnWAVFileInfo);


#endif

//@}
