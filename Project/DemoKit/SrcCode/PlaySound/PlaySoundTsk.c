/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       PlaySoundTsk.c
    @ingroup    mIPRJAPKey

    @brief      Play Startup, keypad tone...sound
                This task handles the sound playback of startup, keypad ...

    @note       Nothing.

    @date       2006/01/23
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "Kernel.h"
#include "SysCfg.h"
#include "SoundData.h"
#include "PlaySoundTsk.h"
#include "PlaySoundInt.h"

volatile UINT32     g_uiPlaySoundSound;
volatile UINT32     g_uiPlaySoundStatus;

void PlaySoundTsk(void)
{
    FLGPTN              uiFlag;
    AUDIO_DEVICE_OBJ    AudioDevice;
    AUDIO_BUF_QUEUE     AudioBufQueue;
    //#NT#2010/01/20#JustinLee -begin
    //#Add wav header parsing before playing sound data
    WAV_FILEINFO        WAVFileInfo;
    //#NT#2010/01/20#JustinLee -end

    while (1)
    {
        wai_flg(&uiFlag, FLG_ID_SOUND, FLGSOUND_STOP | FLGSOUND_PLAY, TWF_ORW | TWF_CLR);

        if (uiFlag & FLGSOUND_STOP)
        {
            if (g_uiPlaySoundStatus == PLAYSOUND_STS_PLAYING)
            {
                aud_stop();
                aud_close();
                g_uiPlaySoundStatus = PLAYSOUND_STS_STOPPED;

                set_flg(FLG_ID_SOUND, FLGSOUND_STOPPED);
            }
        }

        if (uiFlag & FLGSOUND_PLAY)
        {
            //#NT#2009/06/24#Chris Chung -begin
            //#NT#avoid re-entry to audio driver.
            if(aud_isBusy())
                continue;
            //#NT#2009/06/24#Chris Chung -end

            g_uiPlaySoundStatus = PLAYSOUND_STS_PLAYING;
            clr_flg(FLG_ID_SOUND, FLGSOUND_STOPPED);

            // Open audio driver
            aud_getDeviceObject(&AudioDevice);
            AudioDevice.pEventHandler = PlaySound_AudioHdl;
            aud_open(&AudioDevice);

            switch (g_uiPlaySoundSound)
            {
            case PLAYSOUND_SOUND_STARTUP:
            case PLAYSOUND_SOUND_SHUTDOWN:
                // Parse header before playing
                if(PlaySound_ParseWAVHeader((UINT32)g_PowerOn_SoundKey.puiData, &WAVFileInfo) == TRUE)
                {
                    // Set sampling rate and channel based on parameter
                    aud_setSamplingRate(WAVFileInfo.AudioSamplingRate);
                    aud_setChannel(WAVFileInfo.AudioChannel);

                    // Set audio buffer queue addr and size
                    AudioBufQueue.uiAddress = ((UINT32)g_PowerOn_SoundKey.puiData) + (sizeof(WAV_PCMHEADER));
                    AudioBufQueue.uiSize    = WAVFileInfo.uiDataSize;
                }
                else
                {
                    // Set sampling rate and channel manually
                    aud_setSamplingRate(AUDIO_SR_32000);
                    aud_setChannel(AUDIO_CH_LEFT);

                    // Set audio buffer queue addr and size
                    AudioBufQueue.uiAddress = (UINT32)g_PowerOn_SoundKey.puiData;
                    AudioBufQueue.uiSize    = g_PowerOn_SoundKey.uiSize;
                }
                break;
            case PLAYSOUND_SOUND_KEY_SHUTTER2:
                // Parse header before playing
                if(PlaySound_ParseWAVHeader((UINT32)g_Shutter_SoundKey.puiData, &WAVFileInfo) == TRUE)
                {
                    // Set sampling rate and channel based on parameter
                    aud_setSamplingRate(WAVFileInfo.AudioSamplingRate);
                    aud_setChannel(WAVFileInfo.AudioChannel);

                    // Set audio buffer queue addr and size
                    AudioBufQueue.uiAddress = ((UINT32)g_Shutter_SoundKey.puiData) + (sizeof(WAV_PCMHEADER));
                    AudioBufQueue.uiSize    = WAVFileInfo.uiDataSize;
                }
                else
                {
                    // Set sampling rate and channel manually
                    aud_setSamplingRate(AUDIO_SR_32000);
                    aud_setChannel(AUDIO_CH_LEFT);

                    // Set audio buffer queue addr and size
                    AudioBufQueue.uiAddress = (UINT32)g_Shutter_SoundKey.puiData;
                    AudioBufQueue.uiSize    = g_Shutter_SoundKey.uiSize;
                }
                break;

            case PLAYSOUND_SOUND_KEY_OTHERS:
                // Parse header before playing
                if(PlaySound_ParseWAVHeader((UINT32)g_SoundKey.puiData, &WAVFileInfo) == TRUE)
                {
                    // Set sampling rate and channel based on parameter
                    aud_setSamplingRate(WAVFileInfo.AudioSamplingRate);
                    aud_setChannel(WAVFileInfo.AudioChannel);

                    // Set audio buffer queue addr and size
                    AudioBufQueue.uiAddress = ((UINT32)g_SoundKey.puiData) + (sizeof(WAV_PCMHEADER));
                    AudioBufQueue.uiSize    = WAVFileInfo.uiDataSize;
                }
                else
                {
                    // Set sampling rate and channel manually
                    aud_setSamplingRate(AUDIO_SR_32000);
                    aud_setChannel(AUDIO_CH_LEFT);

                    // Set audio buffer queue addr and size
                    AudioBufQueue.uiAddress = (UINT32)g_SoundKey.puiData;
                    AudioBufQueue.uiSize    = g_SoundKey.uiSize;
                }
                break;

            default:
                aud_close();
                continue;
            }

            // Set time code offset and first time code trigger value
            aud_setTimecodeOffset(0);
            aud_setTimecodeTrigger(AudioBufQueue.uiSize >> 1);

            // Reset audio buffer queue and add buffer to queue
            aud_resetBufferQueue();
            aud_addBufferToQueue(&AudioBufQueue);

            // Start to play
            aud_playback(FALSE, TRUE);
        }
    }
}

//@}
