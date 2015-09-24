/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       PlaySoundUtil.c
    @ingroup    mIPRJAPKey

    @brief      Play Sound task API
                Internal functions of play sound task

    @note       Nothing.

    @date       2006/01/23
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "Kernel.h"
#include "SysCfg.h"
#include "PlaySoundInt.h"
#include "Audio.h"

/**
  Audio event handler of play sound task

  Audio event handler of play sound task

  @param UINT32 uiEventID: Audio event ID
  @return void
*/
void PlaySound_AudioHdl(UINT32 uiEventID)
{
    if (uiEventID & AUDIO_EVENT_TCHIT)
    {
        set_flg(FLG_ID_SOUND, FLGSOUND_STOP);
    }
}

//@}
