/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitVoice.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for Voice Recorder mode
                Get memory and set parameters for Voice Recorder mode

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#include "UIFlow.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"

/**
  Initialize application for Voice Recorder mode

  Initialize application for Voice Recorder mode.

  @param void
  @return void
*/
void AppInit_ModeVoice(void)
{
    UINT32              uiPoolAddr;
    WAVSTUDIO_APPOBJ    WavObj;
    WAVSTUDIO_FMT       WavFormat;

    // Get memory for Capture task
    get_blk((VP *)&uiPoolAddr, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    // Setup parameter for WAV studio task
    WavObj.uiMemoryAddr         = uiPoolAddr + POOL_SIZE_CAPTURE - POOL_SIZE_WAV;
    WavObj.uiMemorySize         = POOL_SIZE_WAV;
    WavObj.WavStudioCB          = (WAVSTUDIO_CB)FlowVoice_WAVStudioCB;

    WavFormat.AudioChannel      = AUDIO_CH_RIGHT;
    //WavFormat.AudioChannel    = AUDIO_CH_LEFT;
    WavFormat.AudioSamplingRate = AUDIO_SR_11025;
    WavFormat.AudioCodec        = AUDIOCODEC_IMA_ADPCM; // OK, 但背景雜音頗重
    //WavFormat.AudioCodec      = AUDIOCODEC_PCM;       // 不知為何會有爆音
    WavStudio_SetFormat(&WavFormat);

    WavStudio_SetMaxRecTime(WAVSTUDIO_MAX_RECTIME_INFINITE);

    // Open Wav studio task
    if (WavStudio_Open(&WavObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for wav studio task\r\n"));
    }
}

//@}
