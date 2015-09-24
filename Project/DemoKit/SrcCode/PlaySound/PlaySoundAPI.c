/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       PlaySoundAPI.c
    @ingroup    mIPRJAPKey

    @brief      Play Sound task API
                This file includes all APIs of play sound task

    @note       Nothing.

    @date       2006/01/23
*/

/** \addtogroup mIPRJAPKey */
//@{

#include <string.h>
#include "Kernel.h"
#include "SysCfg.h"
#include "PlaySoundTsk.h"
#include "PlaySoundInt.h"
#include "dcAVCodec.h"

static volatile BOOL            bPlaySoundOpened = FALSE;

/**
  Open Play Sound task

  Open Play Sound task.
  The parameter is not using now, just for compliance

  Return value is listed below:
  E_SYS: Task is already opened
  E_OK: No error

  @param PPLAYSOUND_APPOBJ pWavObj: Play Sound application object
  @return ER
*/
ER PlaySound_Open(PPLAYSOUND_APPOBJ pPlaySoundObj)
{
    if (bPlaySoundOpened == TRUE)
    {
        return E_SYS;
    }

    clr_flg(FLG_ID_SOUND, FLGSOUND_ALL);

    bPlaySoundOpened    = TRUE;
    g_uiPlaySoundStatus = PLAYSOUND_STS_STOPPED;
    set_flg(FLG_ID_SOUND, FLGSOUND_STOPPED);

    sta_tsk(PLAYSOUNDTSK_ID);

    return E_OK;
}

/**
  Close Play Sound task

  Close Play Sound task.

  Return value is listed below:
  E_SYS: Task is already closed
  E_OK: No error

  @param void
  @return ER
*/
ER PlaySound_Close(void)
{
    FLGPTN uiFlag;

    if (bPlaySoundOpened == FALSE)
    {
        return E_SYS;
    }

    if (g_uiPlaySoundStatus == PLAYSOUND_STS_PLAYING)
    {
        wai_flg(&uiFlag, FLG_ID_SOUND, FLGSOUND_STOPPED, TWF_ORW | TWF_CLR);
    }

    bPlaySoundOpened = FALSE;

    ter_tsk(PLAYSOUNDTSK_ID);
    return E_OK;
}

/**
  Play Sound

  Play Sound

  @param UINT32 uiSound: Sound to play (Please refer to PlaySoundTsk.h)
  @return void
*/
void PlaySound_Play(UINT32 uiSound)
{
    // Case 1: Don't stop previous playing sound
    /*
    // Don't play sound if previous sound is not finished yet
    if (g_uiPlaySoundStatus == PLAYSOUND_STS_PLAYING)
    {
        return;
    }

    g_uiPlaySoundSound = uiSound;
    set_flg(FLG_ID_SOUND, FLGSOUND_PLAY);
    */

    // Case 2: Stop previous playing sound first
    g_uiPlaySoundSound = uiSound;

    if (g_uiPlaySoundStatus == PLAYSOUND_STS_PLAYING)
    {
        set_flg(FLG_ID_SOUND, FLGSOUND_PLAY | FLGSOUND_STOP);
    }
    else
    {
        //#NT#2009/06/22#Chris Chung -begin
        //#NT#Add new PlaySound API to wait stop succesfully.
        clr_flg(FLG_ID_SOUND, FLGSOUND_STOPPED);
        //#NT#2009/06/22#Chris Chung -end
        set_flg(FLG_ID_SOUND, FLGSOUND_PLAY);
    }
}

/**
  Stop playing Sound

  Stop playing Sound

  @param void
  @return void
*/
void PlaySound_Stop(void)
{
    if (g_uiPlaySoundStatus == PLAYSOUND_STS_PLAYING)
    {
        set_flg(FLG_ID_SOUND, FLGSOUND_STOP);
    }
}

//#NT#2009/06/22#Chris Chung -begin
//#NT#Add new PlaySound API to wait stop succesfully.
/**
  Wait playing Sound stopped

  Wait playing Sound stopped

  @param void
  @return void
*/
void PlaySound_WaitStopped(void)
{
    FLGPTN uiFlag;

    wai_flg(&uiFlag, FLG_ID_SOUND, FLGSOUND_STOPPED, TWF_ORW);
}
//#NT#2009/06/22#Chris Chung -end

//#NT#2010/01/20#JustinLee -begin
//#Add wav header parsing before playing sound data
/**
  Parse information from WAV file header API

  Parse information from WAV file header.
  The WAV file information will be return to caller.
  (PlaySound only supports PCM audio)

  @param PWAV_FILEINFO pRtnWAVFileInfo: Returned WAV file information
      - @ uiDirID:       [in] Dir ID of WAV file you want to read
      - @ uiFileID:      [in] File ID of WAV file you want to read
      - @ uiDataSize:    [out] Total data (PCM or ADPCM) size of WAV file
      - @ uiSampleCount: [out] Total audio sample count
      - @ AudioCodec:    [out] What kind of codec
          - @ AUDIOCODEC_PCM
          - @ AUDIOCODEC_IMA_ADPCM
      - @ AudioSamplingRate: [out] Sampling rate
      - @ AudioChannel:  [out] Audio channel
          - @ AUDIO_CH_LEFT
          - @ AUDIO_CH_STEREO
      - @ uiBitsPerSample:   [out] bits per sample

  @return
      - @ TRUE: Read success
      - @ FALSE: Read error

*/
BOOL PlaySound_ParseWAVHeader(UINT32 uiSoundDataAddr, PWAV_FILEINFO pRtnWAVFileInfo)
{
    WAV_FILEINFO FileInfo;
    CHAR ReadBuff[PLAYSOUND_WAVHEADER_BUF_SIZE];

    if (!pRtnWAVFileInfo)
    {
        debug_err(("PlaySound_ParseWAVHeader: No wav info buffer for output!\r\n"));
        return FALSE;
    }
    memset((void *)&ReadBuff[0], 0, PLAYSOUND_WAVHEADER_BUF_SIZE);

    // Set buffer for header
    if (WAV_SetHeaderBuffer((UINT32)&ReadBuff[0], PLAYSOUND_WAVHEADER_BUF_SIZE) != WAV_STATUS_OK)
    {
        debug_err(("PlaySound_ParseWAVHeader: Set header buffer error!\r\n"));
        return FALSE;
    }

    // Read header and parse information
    if (WAVReader_ReadHeader(&FileInfo, uiSoundDataAddr) == WAV_STATUS_OK)
    {
        pRtnWAVFileInfo->uiDataSize = FileInfo.uiDataSize;
        pRtnWAVFileInfo->uiSampleCount = FileInfo.uiSampleCount;
        pRtnWAVFileInfo->uiBitsPerSample = FileInfo.uiBitsPerSample;
        pRtnWAVFileInfo->AudioCodec = FileInfo.AudioCodec;
        pRtnWAVFileInfo->AudioSamplingRate = FileInfo.AudioSamplingRate;
        pRtnWAVFileInfo->AudioChannel = FileInfo.AudioChannel;
        #if 0
        debug_err(("###: uiDataSize = %d\r\n", pRtnWAVFileInfo->uiDataSize));
        debug_err(("###: uiSampleCount = %d\r\n", pRtnWAVFileInfo->uiSampleCount));
        debug_err(("###: uiBitsPerSample = %d\r\n", pRtnWAVFileInfo->uiBitsPerSample));
        debug_err(("###: AudioCodec = %d\r\n", pRtnWAVFileInfo->AudioCodec));
        debug_err(("###: AudioSamplingRate = %d\r\n", pRtnWAVFileInfo->AudioSamplingRate));
        debug_err(("###: AudioChannel = %d\r\n", pRtnWAVFileInfo->AudioChannel));
        #endif

        if (pRtnWAVFileInfo->AudioCodec != AUDIOCODEC_PCM)
        {
            debug_err(("PlaySound_ParseWAVHeader: Audio is not PCM data!\r\n"));
            return FALSE;
        }
        if (pRtnWAVFileInfo->uiBitsPerSample != 16)
        {
            debug_err(("PlaySound_ParseWAVHeader: Audio is not 16bits PCM!\r\n"));
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        debug_err(("PlaySound_ParseWAVHeader: Read header error!\r\n"));
        return FALSE;
    }
}
//#NT#2010/01/20#JustinLee -end

//@}
