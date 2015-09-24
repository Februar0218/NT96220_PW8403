/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       PlaySoundTsk.h
    @ingroup    mIPRJAPKey

    @brief      Header file of Play Sound Task
                Header file of task handles the sound playback of startup, keypad ...

    @note       Nothing.

    @date       2006/01/23
*/

#ifndef _PLAYSOUNDTSK_H
#define _PLAYSOUNDTSK_H

/** \addtogroup mIPRJAPKey */
//@{

#include "Type.h"
#include "Audio.h"

#define PLAYSOUND_SOUND_STARTUP         0
#define PLAYSOUND_SOUND_SHUTDOWN        1
#define PLAYSOUND_SOUND_KEY_SHUTTER2    2
#define PLAYSOUND_SOUND_KEY_OTHERS      3

typedef struct
{
    UINT32          uiMemoryAddr;
    UINT32          uiMemorySize;
} PLAYSOUND_APPOBJ, *PPLAYSOUND_APPOBJ;

extern ER       PlaySound_Open(PPLAYSOUND_APPOBJ pPlaySoundObj);
extern ER       PlaySound_Close(void);

extern void     PlaySound_Play(UINT32 uiSound);
extern void     PlaySound_Stop(void);
//#NT#2009/06/22#Chris Chung -begin
//#NT#Add new PlaySound API to wait stop succesfully.
extern void     PlaySound_WaitStopped(void);
//#NT#2009/06/22#Chris Chung -end

//@}

#endif
