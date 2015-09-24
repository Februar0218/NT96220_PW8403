/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanUSB.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect USB is plugging in or unplugged
                Detect USB is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "USB.h"
#include "KeyScanInt.h"
#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "FilesysTsk.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "MsdcNvtManager.h"
#include "SystemClock.h"
#include "UIFlow.h"

#define  USB_SKIP_POWEROFF_COUNT        10 // 1000 ms (9 KeyScan_DetPoweroff time + 1 debounce )

UINT32   bUSBDetectedSkipPowerCount     = 0;
volatile BOOL   g_bKeyScanUSBDetEn = TRUE;
static volatile BOOL    g_bUSBPlug =  FALSE;    //#NT#2007/08/21#JaniceHuang::when usb dectect skip low battery check,for HW printer bug
static volatile BOOL    g_bUSBPower=  FALSE;
BOOL KeyScan_IsUSBPower(void);
void KeyScan_SetUSBPower(BOOL bEn);
//#NT#2011/07/19#KS Hung -begin
//#NT#Only for MSDC vendor command
BOOL bRunningMsdcTool = FALSE;
//#NT#2011/07/19#KS Hung -end

/**
  Detect USB is plugging in or unplugged

  Detect USB is plugging in or unplugged and change corresponding mode
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetUSB(void)
{
    static BOOL bLastUSBDet     = FALSE;
    static BOOL bLastUSBStatus  = FALSE;
    BOOL        bCurUSBDet, bCurUSBStatus;
    VControl    *pCurrnetWnd;
    UINT32      uiUsbPlugStatus;
    static UINT32 uiUsbLastPlugStatus=0;

    static BOOL gbRecordingUSBDet=TRUE;
    //2009/03/16 Janice::USB would not redear ,need to wait UIframework
    if (!Ux_IsInitMessage())
    {
        return ;
    }

#if 0
    bCurUSBDet = gpio_getIntStatus(GPIO_INT_USBPLUGIN);
#else
    // for USB debounce
    /*
    UINT32 usb_getPlugState(void);
    Get USB plug state

    @return
        - @b 0: VBUS is removed
        - @b 1: VBUS is HIGH, but DP/DM is NOT contacted
        - @b 2: VBUS is HIGH, and DP/DM is contacted
    */
    uiUsbPlugStatus = usb_getPlugState() & uiUsbLastPlugStatus;
    uiUsbLastPlugStatus = usb_getPlugState();
    if (uiUsbPlugStatus==0)
        bCurUSBDet = FALSE;
    else
        bCurUSBDet = TRUE;
#endif
    bCurUSBStatus = (BOOL)(bCurUSBDet & bLastUSBDet);

    if (bCurUSBStatus != bLastUSBStatus)
    {
        // USB plugging in
        if (bCurUSBStatus == TRUE)
        {
//          PrimaryModeID CurMode = Primary_GetChangeMode();

           // debug_err(("^R USB plugging in\r\n"));
            bUSBDetectedSkipPowerCount =  USB_SKIP_POWEROFF_COUNT;
            //#NT#2007/08/08#JaniceHuang:: avoid HW bug,plug USB would power btn would detect,need to skip detect pwr off.
            //g_bKeyScanUSBDetEn move here after setting bUSBDetectedSkipPowerCount,
            // g_bKeyScanUSBDetEn had better in keyscantsk ,before KeyScan_DetUSB,but cannot now
            /*if(!g_bKeyScanUSBDetEn)
               // return;				
            	{  
            	   if(gbRecordingUSBDet == TRUE)
            	   {
			gbRecordingUSBDet = FALSE;
			KeyScan_SetUSBPower(TRUE);
		      bLastUSBDet     = bCurUSBDet;
		      bLastUSBStatus  = bCurUSBStatus;	
                return;
		   }
		   else
		   	{
                return;
		   	}
            	}*/
            g_bUSBPlug =  TRUE;

            KeyScan_EnableAutoPoweroff(FALSE);
	//KeyScan_EnableDelayShutdown(FALSE);
            //check filesys, if busy skip USB dectect ##can not wait inifinit,would lock keyscantsk
            //donot change bCurUSBStatus for dectect again,until FST not busy
            //#NT#2007/07/28@Steven Wang begin
            //Fix usb plug while wait moment window is executing
            if (FilesysWaitCmdFinish(FST_TIME_NO_WAIT) == FST_STA_BUSY || Ux_GetBackgroundTskBusy() == TRUE)
            {
                if(Ux_GetBackgroundTskExeuteCommand() == NVTEVT_BKW_WAIT_PTR_INIT && FilesysWaitCmdFinish(FST_TIME_NO_WAIT) != FST_STA_BUSY)
                {
                    debug_err(("UIBackgroundReset ... while print init\r\n"));
                    Ux_ResetBackgroundTsk();
                }
                else
                {
                    debug_err(("^R UIBackground busy\r\n"));
                    return;
                }
            }
            //#NT#2007/07/28@Steven Wang end
            //Fix power on and plug in USB would multi enter change mode
            if(Primary_CheckIdle(PRI_TIME_NO_WAIT) != PRI_STA_IDLE)
            {
                return; //return would not update bLastUSBDet ,next time would detect again
            }


            //#NT#2011/11/21@Philex Lin - begin
            // for USB timeout check
#if 0
            if (usb_StateChange() == FALSE)
            {
                debug_msg("USB is connected to charger\r\n");
                bLastUSBDet     = bCurUSBDet;
                bLastUSBStatus  = bCurUSBStatus;
                KeyScan_SetUSBPower(TRUE);

            }
#else
            if (usb_getPlugState()==1||(usb_getPlugState()==2&&usb_chkCharger(200)))
            {
                debug_msg("USB is connected to charger\r\n");
                bLastUSBDet     = bCurUSBDet;
                bLastUSBStatus  = bCurUSBStatus;
                KeyScan_SetUSBPower(TRUE);
                return;
            }
#endif
            KeyScan_SetUSBPower(FALSE);
            //#NT#2011/11/21@Philex Lin - end

            //janice for screen ¸õ°Ê and ´Ý¼v
            UI_Set_NotFlip();
            //#NT#2011/07/19#KS Hung -begin
            #if (_MSDCVENDOR_ == _MSDCVENDOR_NVT_)
            if (1)//((GPIOMap_DetKey() & FLGKEY_SHUTTER2) && (KeyScan_CheckEngModeTextFile() == TRUE))
            //#NT#2010/11/30#KS Hung -end
            {
                SysClk_SetClk(SPEED_ID_USBMSDCVDRCMD);
                MsdcNvt_Open();
                bRunningMsdcTool = TRUE;
                Primary_ChangeMode(Primary_GetCurrentMode());
                Primary_Wait4ChangeModeDone();
            }
            else
            {
                if ((GPIOMap_DetKey() & FLGKEY_SHUTTER2) && (KeyScan_CheckEngModeTextFile() == FALSE) && (Primary_GetCurrentMode() != PRIMARY_MODE_UNKNOWN))
                {
                    //#NT#2011/09/14#KS Hung -begin
                    switch(Primary_GetCurrentMode())
                    {
                    case PRIMARY_MODE_MOVIE:
                        switch (FlowMovie_GetMovDataState())
                        {
                        case MOV_ST_REC:
                        case MOV_ST_REC | MOV_ST_ZOOM:
                            FlowMovie_StopRec();
                            break;
                        }
                        break;
                    }
                    //#NT#2011/09/14#KS Hung -end
                     //Primary_ChangeMode(PRIMARY_MODE_DUMMY);
                     //Primary_Wait4ChangeModeDone();
                     Primary_ChangeMode(PRIMARY_MODE_USBPCC);
                     //Primary_Wait4ChangeModeDone();
                }
                else
                {
                    Ux_GetFocusedWindow(&pCurrnetWnd);
                    if (!strncmp(pCurrnetWnd->Name,"UIFlowWndPlayPictB",18))
                    {
                        Primary_ChangeMode(PRIMARY_MODE_USBSICD);
                        //Primary_Wait4ChangeModeDone();
                    }
                    else
                    {
                        Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
                        //Primary_Wait4ChangeModeDone();
                    }
                }
            }
            #else
            if (GPIOMap_DetKey() & FLGKEY_SHUTTER2)
            {
                // press shoot key to enter PCC
                Primary_ChangeMode(PRIMARY_MODE_USBPCC);
                //Primary_Wait4ChangeModeDone();
            }
            else
            {
                Ux_GetFocusedWindow(&pCurrnetWnd);
                if (!strncmp(pCurrnetWnd->Name,"UIFlowWndPlayPictB",18))
                {
                    Primary_ChangeMode(PRIMARY_MODE_USBSICD);
                    //Primary_Wait4ChangeModeDone();
                }
                else
                {
                    //Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
                    Primary_ChangeMode(PRIMARY_MODE_CUSTOM1);                    
                    //Primary_Wait4ChangeModeDone();
                }
            }
            #endif
            //#NT#2011/07/19#KS Hung -end
        }
        // USB unplugged
        else
        {
            debug_err(("^R USB unplugging\r\n"));
            g_bUSBPlug = FALSE;

            KeyScan_SetUSBPower(FALSE);
	     Ux_GetFocusedWindow(&pCurrnetWnd);
            if (!strncmp(pCurrnetWnd->Name,"UIMenuWndUSB",12))
            {
			KeyScan_PoweroffFlow();
            }
            if ((Primary_GetCurrentMode() == PRIMARY_MODE_USBMSDC) ||(Primary_GetCurrentMode() == PRIMARY_MODE_USBPCC))//henry 20141218
		{
			KeyScan_PoweroffFlow();
		}
            #if (KeyScan_AC_DET == ENABLE)
            //for Car driver
            KeyScan_DetAC();
            #endif

            #if (KeyScan_POWEROF_ACUNPLUG == ENABLE)
            KeyScan_DetACUnPlug();
            #endif

		//KeyScan_EnableDelayShutdown(TRUE);
            //#2007/08/17#JaniceHuang::do power off while unplug
            //if  (KeyScan_IsACUnPlugPowerOff()==FALSE)
                //KeyScan_PoweroffFlow();

            // Enable modedial detection
            KeyScan_EnableMDDet(TRUE);
            // Enable auto power off function
            KeyScan_EnableAutoPoweroff(TRUE);
            // Enable sleep mode
            //KeyScan_EnableSleep(TRUE);
            // Enable key detection
            KeyScan_EnableKeyDet(TRUE);

            // Detect modedial and force change mode
            // Unnecessary here
            // KeyScan_DetModedial(TRUE);
            // #NT#2007/07/26@StevenWang begin
            // When USB un plug, using Get previous mode to back to previous mode
            // uiPreModeID = Primary_GetPreviousMode();
            // #NT#2007/07/31@Janice begin -- always back to photo mode
            //Primary_ChangeMode(PRIMARY_MODE_PHOTO);
            // #NT#2007/07/26@StevenWang end
        }
    }

    bLastUSBDet     = bCurUSBDet;
    bLastUSBStatus  = bCurUSBStatus;
}

/**
  Enable/Disable USB detection

  Enable/Disable USB detection

  @param BOOL bEn: TRUE -> Enable USB detection, FALSE -> Disable USB detection
  @return void
*/
void KeyScan_EnableUSBDet(BOOL bEn)
{
    g_bKeyScanUSBDetEn = bEn;
}

BOOL KeyScan_IsUSBPlug(void)
{
    if (KeyScan_IsUSBPower())
    {
        return FALSE;
    }

    return g_bUSBPlug;
}

BOOL KeyScan_CheckUSBInSysInit(void)
{
    UINT32 uiUSBStatus = 0;
    uiUSBStatus = usb_getPlugState();
    if(uiUSBStatus == 0)
	return FALSE;
    else
      return TRUE;		
    //return (usb_StateChange());
}

//#NT#2011/11/21@Philex Lin - begin
void KeyScan_SetUSBPower(BOOL bEn)
{
    g_bUSBPower = bEn;
}

BOOL KeyScan_IsUSBPower(void)
{
    return g_bUSBPower;
}
//#NT#2011/11/21@Philex Lin - end

//@}
