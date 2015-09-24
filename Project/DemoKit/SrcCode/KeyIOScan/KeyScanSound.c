/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanSound.c
    @ingroup    mIPRJAPKeyIO

    @brief      Play keypad sound
                Play keypad sound

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "KeyScanTsk.h"
#include "PlaySoundTsk.h"
#include "Timer.h"
#include "Audio.h"
#include "PWM.h"
#include "UIFlow.h"

static volatile BOOL        bKeyToneEn = TRUE;
static UINT8 ubIsPlaying = FALSE;

static BOOL     bHPStatus       = FALSE;
static BOOL     bLastHPDet      = FALSE;
static BOOL     bLastHPStatus   = FALSE;

/**
  Detect external audio (headphone...) is plugging in or unplugged

  Detect external audio (headphone...) is plugging in or unplugged.

  @param void
  @return void
*/
void KeyScan_DetAudio(void)
{
    BOOL            bCurHPDet, bCurHPStatus;
    AUDIO_OUTPUT    AudioOutput;

    bCurHPDet = GPIOMap_DetAudio();

    bCurHPStatus = (BOOL)(bCurHPDet & bLastHPDet);

    if (bCurHPStatus != bLastHPStatus)
    {
        //AUDIO_DEVICE_OBJ    AudioDevObj;

        //SysKickIdle();   //#NewUx porting: Temporarily marked for compile error

        // HP is plugging in
        if (bCurHPStatus == TRUE)
        {
            bHPStatus = TRUE;
            //debug_err(("aud_setOutput(AUDIO_OUTPUT_HP)\r\n"));
        }
        // HP is unplugged
        else
        {
            bHPStatus = FALSE;

            if (KeyScan_IsTVPlugIn())
            	{
                //AudioOutput = AUDIO_OUTPUT_LINE;
                AudioOutput = AUDIO_OUTPUT_SPK;//henry 20141225
		}
            else
            	{
                AudioOutput = AUDIO_OUTPUT_SPK;
            	}
        }
    }

    bLastHPDet     = bCurHPDet;
    bLastHPStatus  = bCurHPStatus;
}

/**
  Return headphone plug in or unplugged

  Return headphone plug in or unplugged.
  TRUE  -> headphone plug in.
  FALSE -> headphone is unplugged.

  @param void
  @return BOOL: TRUE -> headphone plug in, FALSE -> headphone is unplugged
*/
BOOL KeyScan_IsHPPlugIn(void)
{
    return bHPStatus;
}

/**
  Enable/Disable keypad tone

  Enable/Disable keypad tone

  @param BOOL bEn: TRUE -> Enable, FALSE -> Disable
  @return void
*/
void KeyScan_EnableKeyTone(BOOL bEn)
{
    bKeyToneEn = bEn;
}

/**
  Check whether keypad tone is enabled or not

  Check whether keypad tone is enabled or not

  @param void
  @return BOOL: TRUE -> Enable, FALSE -> Disable
*/
BOOL KeyScan_IsKeyToneEnabled(void)
{
    return bKeyToneEn;
}

/**
  Play sound

  Play startup, shutdown, keypad tone

  @param UINT32 uiSound: Which sound you want to play, please refer to KeyScanTsk.h
  @return void
*/
void KeyScan_PlaySound(UINT32 uiSound)
{
    // Only play keypad tone when keypad tone is enabled
    if (bKeyToneEn == FALSE)
    {
        return;
    }

    PlaySound_Play(uiSound);
}

void KeyScan_PlayToneStop()
{
    if(!ubIsPlaying)
        return;

    pwm_stop(PWMID_0);
    ubIsPlaying = FALSE;
}
void KeyScan_PlayTone(UINT8 bType)
{
        PWM_struct PWMInfo;
        UINT32 uiKeyToneTimerID;
        UINT32 uiKeyPlayTime = 100; //ms


        if((ubIsPlaying)||(!bKeyToneEn))
            return;

        PWMInfo.uiDiv       = 180;
        PWMInfo.uiPrd       = 20;
        PWMInfo.uiOnCycle   = 0;
        PWMInfo.uiInv       = 0;
        PWMInfo.uiRise      = 0;
        PWMInfo.uiFall      = 10;
        pwm_open(PWMID_0);
        pwm_set(PWMID_0, &PWMInfo);

        if(timer_openAutoClose((UINT *)&uiKeyToneTimerID, (FP)KeyScan_PlayToneStop) == E_OK)
        {
            pwm_en(PWMID_0);
            timer_set(uiKeyToneTimerID, uiKeyPlayTime, _TIMER_CTRL_ONE_SHOT|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

        }
        ubIsPlaying = TRUE;
}

//@}
