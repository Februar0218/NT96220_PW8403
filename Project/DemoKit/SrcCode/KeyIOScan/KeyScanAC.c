/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanTV.c
    @ingroup    mIPRJAPKey

    @brief      Detect TV is plugging in or unplugged
                Detect TV is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "Alg_IPLMode.h"
#include "PlaybackTsk.h"
#include "AVIPlayAPI.h"
#include "MediaPlayAPI.h"
#include "UIFlow.h"
#include "IDE.h"
#include "Top.h"
#include "GlobalVar.h"
#include "Utility.h"
#include "Audio.h"
//#include "UIMenuSetupSettings.h"
#include "UIFlowWndMovie.h"

static UINT32   uiCurrentACMode;
static BOOL     bACStatus       = FALSE;
static BOOL     bLastACDet      = FALSE;
static BOOL     bLastACStatus   = FALSE;
BOOL bUSBUnplug = FALSE;
/**
  Detect AC is plugging in or unplugged

  Detect AC is plugging in or unplugged.

  @param void
  @return void
*/
void KeyScan_DetAC(void)
{
    #define AC_PLUS_COUNT 30
    BOOL            bCurACDet, bCurACStatus;
    PrimaryModeID   CurrMode;
    static UINT32   ACCount = 0;
    static BOOL     bACUnplus = FALSE;
    extern MOV_TASK_DATA   gMovData ;

    if ((KeyScan_IsHDMIPlugIn() == TRUE) || (KeyScan_IsTVPlugIn() == TRUE))
        return;

    //check acpower 15 seconds,if AC Power is unplused,call stop record and poweroff
    bCurACDet = KeyScan_IsUSBPower();

    if (bCurACDet)
    {
        ACCount = AC_PLUS_COUNT;
	 /*if(SysGetFlag(FL_POWEROFF_DELAY) == POWER_OFF_DELAY_DISABLE)
	 	ACCount = 1;
	 else if(SysGetFlag(FL_POWEROFF_DELAY) == POWER_OFF_DELAY_1MIN)
	 	ACCount = 240;
	 else if(SysGetFlag(FL_POWEROFF_DELAY) == POWER_OFF_DELAY_2MIN)
	 	ACCount = 480;	 	
	 else if(SysGetFlag(FL_POWEROFF_DELAY) == POWER_OFF_DELAY_3MIN)
	 	ACCount = 720;	 
	 else if(SysGetFlag(FL_POWEROFF_DELAY) == POWER_OFF_DELAY_5MIN)
	 	ACCount = 1200;	 
	 else if(SysGetFlag(FL_POWEROFF_DELAY) == POWER_OFF_DELAY_10MIN)
	 	ACCount = 2400;	 
	 else if(SysGetFlag(FL_POWEROFF_DELAY) == POWER_OFF_DELAY_15MIN)
	 	ACCount = 3600;	 */
    }
    else if (bACUnplus)
    {
        if (ACCount)
        {
            ACCount -- ;
        }
        if ((ACCount == 0) && ((gMovData.State == MOV_ST_REC)||(gMovData.State == (MOV_ST_REC|MOV_ST_ZOOM))))
        {
            bUSBUnplug = TRUE;
            //Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_STOP_RECAVI,NULL);
            //TimerDelayMs(1000);
            //KeyScan_Poweroff();
        }
    }

    bCurACStatus = (BOOL)(bCurACDet & bLastACDet);

    //debug_err(("AC is plugged, GPIOMap_DetAC = %d\r\n", bCurACDet));

    if (bCurACStatus != bLastACStatus)
    {
        // AC is plugging in
        if (bCurACStatus == TRUE)
        {
            VControl    *pCurrnetWnd;

            bACStatus = TRUE;

            bACUnplus = FALSE;

            debug_err(("AC is plugged, GPIOMap_DetAC = %d\r\n", bCurACDet));
            CurrMode = Primary_GetCurrentMode();

            /*if (CurrMode == PRIMARY_MODE_MOVIE)
            {
                Ux_GetFocusedWindow(&pCurrnetWnd);
                if (!strncmp(pCurrnetWnd->Name,"UIFlowWndMovie",14))
                {
                    if ((gMovData.State == MOV_ST_VIEW)||(gMovData.State == (MOV_ST_VIEW|MOV_ST_ZOOM)))
                    {
                        //g_uiKeyScanFlags |= (FLGKEY_PRESSED | FLGKEY_DEL);
                        Ux_FlushEvent();
                        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 2, NVTEVT_KEY_PRESS, FLGKEY_PRESSED);
//                        Ux_PostEvent(NVTEVT_KEY_ENTER, 3, NVTEVT_KEY_PRESS, FLGKEY_PRESSED,UIFlowWndMovie_Start_Rec);
                    }
                }
            }else
            {
                // Enter dummy mode first
                Primary_ChangeMode(PRIMARY_MODE_DUMMY);
                Primary_Wait4ChangeModeDone();

                Primary_ChangeMode(PRIMARY_MODE_MOVIE);
                Primary_Wait4ChangeModeDone();
                TimerDelayMs(500);
                //g_uiKeyScanFlags |= (FLGKEY_PRESSED | FLGKEY_DEL);
                Ux_FlushEvent();
                Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 2, NVTEVT_KEY_PRESS, FLGKEY_PRESSED);
                //Ux_PostEvent(NVTEVT_KEY_ENTER, 3, NVTEVT_KEY_PRESS, FLGKEY_PRESSED,UIFlowWndMovie_Start_Rec);
            }*/
        }
        // AC is unplugged
        else
        {
            debug_err(("AC is unplugged, GPIOMap_DetAC = %d\r\n", bCurACDet));
            bACStatus = FALSE;

            bACUnplus = TRUE;

            //KeyScan_PoweroffFlow();

            //#PIC#2011/06/01#Creator -end

            // Get current mode
            CurrMode = Primary_GetCurrentMode();

            if (CurrMode != PRIMARY_MODE_MOVIE)
            {
                return;
            }
        }
    }

    bLastACDet     = bCurACDet;
    bLastACStatus  = bCurACStatus;
}

/**
  Return AC plug in or unplugged

  Return AC plug in or unplugged.
  TRUE  -> AC plug in.
  FALSE -> AC is unplugged.

  @param void
  @return BOOL: TRUE -> AC plug in, FALSE -> AC is unplugged
*/
BOOL KeyScan_IsACIn(void)
{
    return bACStatus;
}

//@}
