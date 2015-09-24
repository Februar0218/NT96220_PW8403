/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanMisc.c
    @ingroup    mIPRJAPKeyIO

    @brief      Miscellaneous KeyScan functions
                Miscellaneous KeyScan functions

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "KeyScanInt.h"
#include "KeyScanTsk.h"
#include "FileSysTsk.h"
#include "Timer.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UIFlow.h"
#include "PrimaryTsk.h"

static BOOL     bBusyLED = FALSE;

UINT32  KeyScanIdleTimetimeCount = 0;
BOOL    flagAutoBacklightOffPerformed = FALSE;
BOOL    flagAutoBacklightOffKeyAction = FALSE;
extern UINT32 g_uiRecSecs;

/**
  Detect system is busy or not

  Detect system is busy or not.
  The following contidions indicate that system is busy:
    (1) Flash is charging
    (2) FileSystem is reading/writing
  If system is busy then toggle red LED
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetBusy(void)
{
     if (KeyScan_IsFlashCharging() == TRUE)
    {
        Ux_PostEvent(NVTEVT_FLASHCHARGING, 1, NVTEVT_FLASHCHARGING_PROC);
        if (bBusyLED == FALSE)
        {
            bBusyLED = TRUE;
            KeyScan_EnableLEDToggle(KEYSCAN_LED_FCS, TRUE);
        }
    }
    else
    {
        if (bBusyLED == TRUE)
        {
        bBusyLED = FALSE;
        KeyScan_EnableLEDToggle(KEYSCAN_LED_FCS, FALSE);
            Ux_PostEvent(NVTEVT_FLASHCHARGING, 1, NVTEVT_FLASHCHARGING_END);
        }
    }
}

/**
  Suspend KeyScan task

  Suspend KeyScan task. The KeyScan timer interrupt will be disabled.
  All key event will be disabled.

  @param void
  @return void
*/
void KeyScan_Suspend(void)
{
    timer_pausePlay(g_uiKeyScanTimerID, _TIMER_PAUSE);

    //#NT#2008/06/11#Steven Wang -begin
    //clr_flg(FLG_ID_INT2, FLGINT2_TIMER0 << g_uiKeyScanTimerID);
    //#NT#2008/06/11#Steven Wang -end
}

/**
  Resume KeyScan task

  Resume KeyScan task.

  @param void
  @return void
*/
void KeyScan_Resume(void)
{
    timer_pausePlay(g_uiKeyScanTimerID, _TIMER_PLAY);
}

/**
  Detect Macro h/w switch is ON or OFF

  Detect Macro h/w switch is ON or OFF

  @param void
  @return void
*/
void KeyScan_DetMacro(void)
{
    static UINT32   uiMacroPrevSts = 0xFFFFFFFF;
    UINT32          uiMacroCurSts;

    if(GPIOMap_DetMacro())
    {
        uiMacroCurSts =  MACRO_ON;
    }
    else
    {
        uiMacroCurSts =  MACRO_OFF;
    }

    //debug_err(("uiMacroPrevSts %x uiMacroCurSts %x  \r\n", uiMacroPrevSts, uiMacroCurSts));

    if(uiMacroPrevSts != uiMacroCurSts)
    {
        SysSetFlag(FL_MACRO, uiMacroCurSts);

        if (Ux_IsInitMessage())
        {
            Ux_PostEvent(NVTEVT_MACRO_CHANGE, 0);
            uiMacroPrevSts = uiMacroCurSts;
        }
        else
        {
            //clear init stat for detect change
            uiMacroPrevSts = 0xFFFFFFFF;
        }
    }
}


/**
  Detect Panel close or not

  Detect Panel close or not

  @param void
  @return void
*/
void KeyScan_DetLCDClose(void)
{
    if (GPIOMap_DetLCDClose())
    {
        // power off
        KeyScan_PoweroffFlow();
    }
}

 /**


  Det idle time, to turn off backlight

  @param void
  @return void
*/
void KeyScan_DetIdleTime(void)
{
   static UINT32 idlePeriod = 0;

 //  static UINT32 timeCount = 0;

   if ((KeyScan_IsUSBPlug() == TRUE) || (KeyScan_IsHDMIPlugIn() == TRUE))
   {

         if(flagAutoBacklightOffPerformed)
         {
             flagAutoBacklightOffPerformed = FALSE;
         }
         KeyScanIdleTimetimeCount = 0;                                                      //if usb or hdmi is pluged, do not
         return;                                                            //perform auto backlight off process
   }

   idlePeriod = Get_LCDBacklightValue();
   if(idlePeriod == 0)
        return;
//   debug_err(("KeyScanIdleTimetimeCount = %d\r\n", KeyScanIdleTimetimeCount));
   if(KeyScanIdleTimetimeCount >= idlePeriod)
   {
       GPIOMap_TurnOffLCDBacklight();
       flagAutoBacklightOffPerformed = TRUE;
   }
   else
       KeyScanIdleTimetimeCount++;                                                          //perfomr every 1s


}

void KeyScan_DetAction(void)
{
    static UINT32 passCount = 0;
    if ((KeyScan_IsUSBPlug() == TRUE) || (KeyScan_IsHDMIPlugIn() == TRUE)
        || Primary_GetCurrentMode() == PRIMARY_MODE_PLAYBACK)
        return;                                                             //if usb or hdmi is pluged, do not
                                                                            //wake up the screens
        if(flagAutoBacklightOffKeyAction)
        {                                                                   //if there is any key action, recount
            KeyScanIdleTimetimeCount = 0;                                   //and wake up lcd
            flagAutoBacklightOffKeyAction = FALSE;
//          debug_err(("clear performed\r\n"));
            if(flagAutoBacklightOffPerformed)
            {
                GPIOMap_TurnOnLCDBacklight();
                passCount++;
//              debug_err(("passCount = %d\r\n",passCount));
                if(passCount > 10)
                {
                    passCount = 0;

                    flagAutoBacklightOffPerformed = FALSE;
                }
            }
        }
}

#define MOVE_RESTART   0XBB

void KeyScan_MovieRecorddingCheck(void)
{
      UINT32 MovieCurrSecond;
      static UINT32 MoviePreSecond=0xffff;
	static UINT32 counter=0;
       switch(Primary_GetCurrentMode())
        {
          case PRIMARY_MODE_MOVIE:
            switch (FlowMovie_GetMovDataState())
            {
		     case MOV_ST_REC:
			 //debug_err(("g_uiRecSecs : %d  MoviePreSecond : %d...\r\n",g_uiRecSecs,MoviePreSecond));
			 MovieCurrSecond = g_uiRecSecs;
			 if(MovieCurrSecond != MoviePreSecond)
			 {	
				MoviePreSecond = MovieCurrSecond;
			 }
			 else
			 {
				counter++;
				if(counter>5)
				{
                                MovieCurrSecond=0;
                                MoviePreSecond=0xffff,				
					counter=0;
                			Ux_PostEvent(NVTEVT_KEY_ENTER, 2, NVTEVT_KEY_PRESS,MOVE_RESTART);
				}
			 }
			break;
	      }
	   break;
	   default:
	   break;
       	}
}
//@}
