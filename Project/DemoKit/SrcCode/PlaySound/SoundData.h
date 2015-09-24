/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SoundData.c
    @ingroup    mIPRJAPKey

    @brief      Keypad tone, Startup... sound data
                This file contain the PCM (ADPCM) data of keypad tone, Startup...

    @note       Nothing.

    @date       2005/01/23
*/

/** \addtogroup mIPRJAPKey */
//@{

#ifndef _SOUNDDATA_H
#define _SOUNDDATA_H

#include "Type.h"

typedef struct
{
    UINT8       *puiData;
    UINT32      uiSize;
} SOUND_DATA, *PSOUND_DATA;

extern SOUND_DATA g_PowerOn_SoundKey;
extern SOUND_DATA g_Shutter_SoundKey;
extern SOUND_DATA g_SoundKey;

#endif

//@}
