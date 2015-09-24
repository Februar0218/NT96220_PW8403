/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanTsk.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, I/O task
                Scan key, I/O task

    @note       Nothing.

    @date       2005/11/18
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "stdio.h"
#include "Kernel.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "KeySCanInt.h"
#include "Timer.h"
#include "RTC.h"
#include "PrimaryTsk.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "debug.h"
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "SystemInit.h"
#include "FileSysTsk.h"
#include "Ramdisk.h"
#include "NAND.h"
#include "UIFlow.h"

#if (_CALIBRATION_MODE_ == ENABLE)
#include "CalibrationTsk.h"
#endif

#include "Gesture_Public.h"
#include "TouchPanel.h"

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "FwVersion.h"
#include "UIConfig.h"
#include "Utility.h"
#include "PhotoTsk.h"

// KeyScan timer interval 20 ms
#define KEYSCAN_TIMER_INTERVAL      20
#define DSC_FW_STR          "Format and Default"

TIMER_INFO  KeyScan_SysTimer[KEYSCAN_SYSTIMER_AMOUNT];

// ---------------------------------------------------------------------
// Internal Global variables
// ---------------------------------------------------------------------
// Declare all global variables used in KeyScan task here.
// Don't reference these variables in your code.
volatile UINT32     g_uiKeyScanTimerID;
volatile UINT32     g_uiKeyScanAutoPoweroffCnt  = 0;
volatile UINT32     g_uiKeyScanAutoLcdoffCnt  = 0;
volatile BOOL       g_bKeyScanAutoPoweroffEn    = TRUE;
volatile BOOL       g_bKeyScanKeyDetEn          = TRUE;
volatile BOOL       g_bKeyScanMDDetEn           = TRUE;
         UINT32     g_uiKeyScanFlags;
volatile BOOL       g_bKeyScanTP = TRUE;

UINT32	g_uiKeyScanDelayShutdownCnt	= 0;
BOOL    g_bKeyScanDelayShutdownEn    = FALSE;
volatile BOOL   g_formatcard  = FALSE;
static void     KeyScan_initTimer(void);
static ER       KeyScan_lockTimer(void);
static void     KeyScan_countTimer(void);
static ER       KeyScan_unlockTimer(void);
static UINT32   KeyScan_getFreeTimer(void);

void KeyScan_PostNvtTPEvent(GESTURE_GESTURE_STRUCT* pGesture);


/**
  KeyScan task

  KeyScan task

  @param void
  @return void
*/
void KeyScanTsk(void)
{
    UINT32 uiKeyScanTimerCnt;
    UINT32 uiKeyScanQSecCnt;
    BOOL   uiKey;
    //UINT32 uiFlag;

    #if (_TouchPanel_ != DISABLE)
    GESTURE_GESTURE_STRUCT  TempGesture;
    #endif

    //#NT#2007/08/15#Stan Lin -begin
    // Initial Message Queue
    // KeyScan may send commands in the boot stage before UIFrameworkTsk starts.
    // MessageQueue has to be initialized before KeyScan running.
    Ux_InitMessage(MBXID_SYS_EVENT, MBXID_BKG_EVENT);
    //#NT#2007/08/15#Stan Lin -end

    // Init the callback function of Primary task for file system initialization
    Primary_SetFSInitCB(KeyScan_StorageCB);
    KeyScan_DetUSB();

    // Set LED toggle speed
    KeyScan_SetLEDToggleSpeed(KEYSCAN_LED_GREEN,  KEYSCAN_LED_TSPEED_SLOW);
    KeyScan_SetLEDToggleSpeed(KEYSCAN_LED_RED,    KEYSCAN_LED_TSPEED_FAST);
    KeyScan_SetLEDToggleSpeed(KEYSCAN_LED_FCS,    KEYSCAN_LED_TSPEED_NORMAL);

    KeyScan_initTimer();

    if (timer_open((UINT *)&g_uiKeyScanTimerID, NULL) != E_OK)
    {
        debug_err(("KEYSCAN ERR: Error open keyscan timer\r\n"));
        while (1)
        {
            ;
        }
    }

    // Turn on green LED and turn off red LED
    KeyScan_TurnOnLED(KEYSCAN_LED_GREEN);
    KeyScan_TurnOffLED(KEYSCAN_LED_RED);
    KeyScan_TurnOffLED(KEYSCAN_LED_FCS);

    //Already clr_flg during timer_open procedure
    //clr_flg(FLG_ID_INT2, FLGINT2_TIMER0 << g_uiKeyScanTimerID);
    timer_set(g_uiKeyScanTimerID, KEYSCAN_TIMER_INTERVAL, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

    // Init variables
    uiKeyScanTimerCnt   = 0;
    uiKeyScanQSecCnt    = 0;

    // Init Card and File System
    KeyScan_DetStrgCard();
    KeyScan_DetStrgCard();

    #if (UI_STYLE == UI_STYLE_DRIVE)
    KeyScan_DetSensorRotate();
    KeyScan_DetSensorRotate();
    #endif

    #if (KEYSCAN_BATTERY_DET == ENABLE)
    if (!KeyScan_CheckUSBInSysInit())
        KeyScan_DetBatteryInSysInit();
    #endif

    // Detect mode dial
    #if (KEYSCAN_KEY_DET == ENABLE)
    //KeyQueue_Init();
    KeyScan_DetKey();
    KeyScan_DetKey();//debounce
    //#NT#2011/08/31#KS Hung -begin
    //#if (UI_STYLE != UI_STYLE_DRIVE)
    if (KeyScan_CheckShowFwVersion())
    {
        /*char ldVerStr[30] = {0};
        PBLINFO_OBJ BLInfoObj;

        BLInfoObj = (PBLINFO_OBJ)Get_BLInfo();
        ldVerStr[0] = 0x4C; //L
        ldVerStr[1] = 0x6F; //o
        ldVerStr[2] = 0x61; //a
        ldVerStr[3] = 0x64; //d
        ldVerStr[4] = 0x65; //e
        ldVerStr[5] = 0x72; //r
        ldVerStr[6] = 0x3A; //:
        ldVerStr[7] = 0x20; //space
        sprintf(&ldVerStr[8], "%s", BLInfoObj->strBLVer);

        UI_DrawRect(_OSD_INDEX_BLUE, 30, 38, 256, 50);
        UI_DrawRect(_OSD_INDEX_BLUE, 30, 90, 256, 50);
        UI_DrawRect(_OSD_INDEX_BLUE, 30, 142, 256, 50);
        UI_DrawOsdString(ldVerStr, 40, 51, _OSD_INDEX_WHITE);
        UI_DrawOsdString(DSC_FW_VERSION_STR, 40, 103, _OSD_INDEX_WHITE);
        UI_DrawOsdString(GetUIConfigSoftwareVer(), 40, 155, _OSD_INDEX_WHITE);
        TimerDelayMs(2000);
        UI_ClearOSD(_OSD_INDEX_TRANSPART);*/
        UI_DrawOsdString(DSC_FW_STR, 40, 43, _OSD_INDEX_RED);
	 g_formatcard = TRUE;
        //UI_DrawOsdString(GetVersionString(), 40, 155, _OSD_INDEX_GREEN);
	 Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_SYSRESET, 0);
        TimerDelayMs(1000);
        UI_ClearOSD(_OSD_INDEX_TRANSPART);
    }
	
    //#endif
	/*if (TRUE == KeyScan_CheckUpdateCfgTextFile())
	{
		//debug_err(("henry----------------11\r\n"));
		if (TRUE == KeyScan_CheckUpdateLogoTextFile())
		{
		//debug_err(("henry----------------22\r\n"));
			UI_UpdateLogoFile();
		}
		if (TRUE == KeyScan_CheckUpdateVersionTextFile())
		{
		//debug_err(("henry----------------33\r\n"));
			UI_UpdateCfgFile();
		}
	}*/
	
        if (TRUE == KeyScan_CheckUpdateLogoTextFile())
        {
        //debug_err(("henry----------------22\r\n"));
           UI_UpdateLogoFile();
        }
        if (TRUE == KeyScan_CheckUpdateVersionTextFile())
        {
        //debug_err(("henry----------------33\r\n"));
            UI_UpdateCfgFile();
        }
	//GPIOMap_TurnOffFloodLight();
	//GPIOMap_TurnOnFloodLight();
	 //uiKey=GPIOMap_IsFloodLightOn();
	 debug_msg("uiKey============%d",GPIOMap_IsFloodLightOn());
	
        //else
        {
        //#NT#2011/08/31#KS Hung -end
        //#NT#2011/08/07#KS Hung -begin
        //if (!KeyScan_CheckEngMode() && !KeyScan_CheckUSBInSysInit())
        //if (!KeyScan_CheckEngMode())
        //{
                //Primary_ChangeMode(PRIMARY_MODE_PHOTO);
                //Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
                Primary_ChangeMode(PRIMARY_MODE_MOVIE);
        //}
            }
	//while(1);

    //#NT#2011/08/07#KS Hung -end
    #endif
	// Init UI USB detection
    KeyScan_DetUSB();
    KeyScan_DetUSB();//eric edit

    //avoid UI battery is unknown level
    KeyScan_DetBattery();
    KeyScan_DetBattery();//eric edit
#if 0//(_GSENSOR_FUNC_ == ENABLE)
    GSensorSensitivity(SysGetFlag(FL_GSENSOR));
#endif
    //---------------------------------------------------------------------------------------------------
    // KeyScan Main Loop
    //---------------------------------------------------------------------------------------------------
    while (1)
    {
        //wai_flg((UINT *)&uiFlag, FLG_ID_INT2, FLGINT2_TIMER0 << g_uiKeyScanTimerID, TWF_ORW | TWF_CLR);
        timer_waitTimeup(g_uiKeyScanTimerID);

        uiKeyScanTimerCnt++;
        g_uiKeyScanFlags = 0;

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 20ms
        //-----------------------------------------------------------------------------------------------
        // Detect key
        #if (KEYSCAN_KEY_DET == ENABLE)
        if (g_bKeyScanKeyDetEn == TRUE)
        {
            KeyScan_DetKey();
        }
        #endif

        // Detect IR Remote key
        #if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
        // Check IR data expiration, frequency value must be smaller than 100ms
        KeyScan_CheckIrRxExp();
        // Detect IR remote control receiver detected key, frequency value must be smaller than 50ms
        KeyScan_DetIrRxKey();
        #endif

        if(Ux_GetBackgroundTskBusy() == TRUE)
        {
            // Detect storage card,prevent card unplug
            KeyScan_DetStrgCard();
        }

        KeyScan_countTimer();

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 60ms
        //-----------------------------------------------------------------------------------------------
        if ((uiKeyScanTimerCnt & 3) == 0)
        {
            // Detect touch panel
            #if (_TouchPanel_ != DISABLE)
            if(g_bKeyScanTP)
            {
                Gesture_DetGesture(&TempGesture);
                if(TempGesture.Event != GESTURE_EVENT_IDLE)
                {
                    KeyScan_PostNvtTPEvent(&TempGesture);
                }
            }
            #endif

            // Toggle LED
            KeyScan_ToggleLED();
            // Detect flash light
            KeyScan_DetFlash();
        }

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 100ms
        //-----------------------------------------------------------------------------------------------
        if ((uiKeyScanTimerCnt % 5) == 0)
        {
            #if (KEYSCAN_POWEROFF_DET == ENABLE)
            // Detect power off key
            KeyScan_DetPoweroff();
            #endif

            //-------------------------------------------------------------------------------------------
            // Do the following task every 1s
            //-------------------------------------------------------------------------------------------
            if ((uiKeyScanTimerCnt % 50) == 0)
            {
                #if (KEYSCAN_AUTOPOWEROFF_DET == ENABLE)
                // Detect auto poweroff
                if ((KeyScan_GetAutoPoweroffTime() != KEYSCAN_AUTOPOWEROFF_DISABLED) && (g_bKeyScanAutoPoweroffEn == TRUE)&&
					(SysGetFlag(FL_MOVIE_MOTION_DET)==MOVIE_MOTIONDET_OFF))
                {
                    KeyScan_DetAutoPoweroff();
                }

                if ((KeyScan_GetAutoLCDoffTime() != 0))
                {
                    KeyScan_DetAutoLCDoff();
                }
                #endif

		if (/*(KeyScan_GetDelayShutdownTime() != 0) && */(g_bKeyScanDelayShutdownEn== TRUE))
               {
			KeyScan_DetDelayShusdown();
                }
        
                KeyScan_DelayShutterKey();
            }
        }

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 250ms
        //-----------------------------------------------------------------------------------------------
        if ((uiKeyScanTimerCnt % 12) == 0)
        {
            uiKeyScanQSecCnt++;

            #if (KEYSCAN_KEY_DET == ENABLE)
            // Detect modedial
            //if (g_bKeyScanMDDetEn == TRUE)
            //{
            //    KeyScan_DetModedial(FALSE);
            //}
            #endif

            // Detect flash light
            //KeyScan_DetFlash();

            #if (KeyScan_AC_DET == ENABLE)
            //for Car driver
            KeyScan_DetAC();
            #endif

            #if (KeyScan_POWEROF_ACUNPLUG == ENABLE)
            KeyScan_DetACUnPlug();
            #endif

            // Detect USB
            KeyScan_DetUSB();

            //Detect macro mode
            KeyScan_DetMacro();

            //-------------------------------------------------------------------------------------------
            // Do the following task every 500ms
            //-------------------------------------------------------------------------------------------
            if (uiKeyScanQSecCnt & 1)
            {
                #if (KEYSCAN_CARDDET_INT == DISABLE)
                // Detect storage card
                KeyScan_DetStrgCard();
                #endif

                // Detect HDMI
                KeyScan_DetHDMI();

                // Detect TV
                 KeyScan_DetTV();
                
                //Detect back
                 KeyScan_DetBack();

                 #if 0//(UI_STYLE == UI_STYLE_DRIVE)//henry 20141209
                 // detect sensor rotate
                 KeyScan_DetSensorRotate();

                 // detect lcd close
                 KeyScan_DetLCDClose();
                 #endif

                // Detect External Audio (headphone...)
                // KeyScan_DetAudio();

                // Detect LCD rotate
                //KeyScan_DetLCDRotate();//henry 20141209

                //debug_err(("GPIOMap_DetPowerOff():%d\r\n",GPIOMap_DetPower()));
                //if (GPIOMap_DetPoweroff())
                //{
                //    KeyScan_PoweroffFlow();
                //}

                // Detect system is busy or not
                KeyScan_DetBusy();
            }

            //-------------------------------------------------------------------------------------------
            // Do the following task every 1s
            //-------------------------------------------------------------------------------------------
            if ((uiKeyScanQSecCnt % 4) == 0)
            {
                #if (KEYSCAN_BATTERY_DET == ENABLE)
                if (!KeyScan_IsUSBPlug())
                {
                    KeyScan_DetBattery();
                }
                #endif

#if 0   //#NewUx porting: Temporarily marked for compile error
                //#NT#2007/05/16#Lily Kao -begin
                if(Primary_GetCurrentMode() == PRIMARY_MODE_USBSICD)
                {
                    set_flg(FLG_DPS, FLGDPS_1SECOND);
                }
                //#NT#2007/05/16#Lily Kao -end
#endif
            }

	      if ((uiKeyScanQSecCnt % 8) == 0)
            {
		    //KeyScan_MovieRecorddingCheck();
	      }            
        }

        //-----------------------------------------------------------------------------------------------
        // End of KeyScanTimer divider
        //-----------------------------------------------------------------------------------------------
/*
#if (_CALIBRATION_MODE_ == ENABLE)
    if (GPIOMap_DetKey() == (FLGKEY_SHUTTER2|FLGKEY_LEFT))
    {
        if(Primary_GetCurrentMode() == PRIMARY_MODE_PHOTO)
        {
            CAL_APPOBJ  CalAppObj;
            UINT32      uiPoolAddr;

            while (GPIOMap_DetKey() & (GPIO_KEY_SHUTTER|GPIO_KEY_LEFT))  ;

            // Get memory for calibration task
            get_blk((VP *)&uiPoolAddr, POOL_ID_CAPTURE);
            rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

            CalAppObj.uiMemoryAddr = uiPoolAddr;
            CalAppObj.uiMemorySize = POOL_SIZE_CAPTURE;
            Cal_Open(&CalAppObj);
        }
    }
#endif
*/

        //-----------------------------------------------------------------------------------------------
        // Start to set key flag
        //-----------------------------------------------------------------------------------------------
        if (g_uiKeyScanFlags != 0)
        {
#if (_CALIBRATION_MODE_ == ENABLE)
            set_flg(FLG_ID_KEY, g_uiKeyScanFlags);
#endif
            switch(g_uiKeyScanFlags & FLGKEY_ACT_MASK)
            {
            case FLGKEY_PRESSED:
                Ux_PostEvent(Get_NVTCMD(g_uiKeyScanFlags), 1, NVTEVT_KEY_PRESS);
                break;

            case FLGKEY_RELEASED:
                Ux_PostEvent(Get_NVTCMD(g_uiKeyScanFlags), 1, NVTEVT_KEY_RELEASE);
                break;

            case FLGKEY_HOLD:
                Ux_PostEvent(Get_NVTCMD(g_uiKeyScanFlags), 1, FLGKEY_HOLD);
                break;

            case FLGKEY_LONGHOLD:
                //Do nothing
                break;

            default:
                debug_err(("KeyScanTsk: Unknown key action (0x%.8X)\r\n", (g_uiKeyScanFlags & FLGKEY_ACT_MASK)));
            break;
            }
            g_uiKeyScanFlags = 0;
        }
    }
}

UINT32 Get_NVTCMD(UINT32 keys)
{
    if(keys & FLGKEY_MODE)
    {
        return NVTEVT_KEY_MODE;
    }
    if(keys & FLGKEY_MENU)
    {
        return NVTEVT_KEY_MENU;
    }
    if(keys & FLGKEY_LEFT)
    {
        return NVTEVT_KEY_LEFT;
    }
    if(keys & FLGKEY_RIGHT)
    {
        return NVTEVT_KEY_RIGHT;
    }
    if(keys & FLGKEY_ENTER)
    {
        return NVTEVT_KEY_ENTER;
    }
    if(keys & FLGKEY_UP)
    {
        return NVTEVT_KEY_UP;
    }
    if(keys & FLGKEY_DOWN)
    {
        return NVTEVT_KEY_DOWN;
    }
    if(keys & FLGKEY_ZOOMOUT)
    {
        return NVTEVT_KEY_ZOOMOUT;
    }
    if(keys & FLGKEY_ZOOMIN)
    {
        return NVTEVT_KEY_ZOOMIN;
    }
    if(keys & FLGKEY_DEL)
    {
        return NVTEVT_KEY_DEL;
    }
    if(keys & FLGKEY_PLAYBACK)
    {
        return NVTEVT_KEY_PLAYBACK;
    }
    if(keys & FLGKEY_SHUTTER1)
    {
        return NVTEVT_KEY_SHUTTER1;
    }
    if(keys & FLGKEY_SHUTTER2)
    {
        return NVTEVT_KEY_SHUTTER2;
    }
    if(keys & FLGKEY_NETWORK)
    {
        return NVTEVT_KEY_NETWORK;
    }

    if(keys & FLGKEY_DISPLAY)
    {
	//debug_err(("----Get_NVTCMD DISPLAY KEY---\r\n"));	      
        return NVTEVT_KEY_DISPLAY;
    }
    else
    {
        debug_err(("Get_NVTCMD: Unknown key (0x%.8X)\r\n", keys));
        return NVTEVT_NULL;
    }
}

/**
  Get_KeyEvt

  Transfer NVTEVT to key event --> only physical key map

  @param NVTEVT
  @return Key event
*/
UINT32 Get_KeyEvt(NVTEVT evt)
{
    UINT32 result = 0;

    switch(evt)
    {
        case NVTEVT_KEY_UP:
            result = FLGKEY_UP;
            break;

        case NVTEVT_KEY_DOWN:
            result = FLGKEY_DOWN;
            break;

        case NVTEVT_KEY_LEFT:
            result = FLGKEY_LEFT;
            break;

        case NVTEVT_KEY_RIGHT:
            result = FLGKEY_RIGHT;
            break;

        case NVTEVT_KEY_ENTER:
            result = FLGKEY_ENTER;
            break;

        case NVTEVT_KEY_MENU:
            result = FLGKEY_MENU;
            break;

        case NVTEVT_KEY_MODE:
            result = FLGKEY_MODE;
            break;

        case NVTEVT_KEY_ZOOMOUT:
            result = FLGKEY_ZOOMOUT;
            break;

        case NVTEVT_KEY_ZOOMIN:
            result = FLGKEY_ZOOMIN;
            break;

        case NVTEVT_KEY_SHUTTER1:
            result = FLGKEY_SHUTTER1;
            break;

        case NVTEVT_KEY_SHUTTER2:
            result = FLGKEY_SHUTTER2;
            break;

        case NVTEVT_KEY_DEL:
            result = FLGKEY_DEL;
            break;

        case NVTEVT_KEY_PLAYBACK:
            result = FLGKEY_PLAYBACK;
            break;

        default:
            break;
    }
    return result;
}

/**
  KeyScan_initTimer

  Start a timer control array

  @param void
  @return void
*/
static void KeyScan_initTimer(void)
{
    UINT32   i;
    for(i = 0; i < KEYSCAN_SYSTIMER_AMOUNT; i++)
    {
        KeyScan_SysTimer[i].counter = 0;
        KeyScan_SysTimer[i].interval = 0;
        KeyScan_SysTimer[i].eventID = NVTEVT_NULL;
        KeyScan_SysTimer[i].isEnabled = FALSE;
        KeyScan_SysTimer[i].type = TIMER_TYPE_ONE_SHOT;
    }
}

/**
  KeyScan_getFreeTimer

  Get a free timer of timer control array

  @param void
  @return void
*/
static UINT32 KeyScan_getFreeTimer(void)
{
    UINT32   i;
    for(i = 0; i < KEYSCAN_SYSTIMER_AMOUNT; i++)
    {
        if(KeyScan_SysTimer[i].isEnabled == FALSE)
            return i;
    }
    return TIMER_NULL;
}

/**
  KeyScan_lockTimer

  Get a sem to lock the resource

  @param void
  @return void
*/
static ER KeyScan_lockTimer(void)
{
    ER erReturn;
    erReturn = wai_sem(SEMID_KNLTIMER);
    if (erReturn != E_OK)
    {
        return erReturn;
    }
    return E_OK;
}

/**
  KeyScan_unlockTimer

  ulock the resource

  @param void
  @return void
*/
static ER KeyScan_unlockTimer(void)
{
    return sig_sem(SEMID_KNLTIMER);
}

/**
  KeyScan_countTimer

  Count the timer ...

  @param void
  @return void
*/
static void KeyScan_countTimer(void)
{
    UINT32      i;

    KeyScan_lockTimer();
    for(i = 0; i < KEYSCAN_SYSTIMER_AMOUNT; i++)
    {
        if(KeyScan_SysTimer[i].isEnabled == TRUE)
        {
            KeyScan_SysTimer[i].counter ++;
            if((KeyScan_SysTimer[i].counter * KEYSCAN_TIMER_INTERVAL) ==  KeyScan_SysTimer[i].interval)
            {
                Ux_PostEvent(NVTEVT_TIMER, 1, KeyScan_SysTimer[i].eventID);
                KeyScan_SysTimer[i].counter = 0;

                if(KeyScan_SysTimer[i].type == TIMER_TYPE_ONE_SHOT)
                {
                    KeyScan_SysTimer[i].eventID = NVTEVT_NULL;
                    //KeyScan_SysTimer[i].isBusy = FALSE;
                    KeyScan_SysTimer[i].isEnabled = FALSE;
                }
            }
        }
    }
    KeyScan_unlockTimer();
}
/**
  KNLstartTimer

  @param void
  @return timer ID
*/
UINT32  KeyScan_startTimer(UINT32 interval, UINT32 eventID, TIMER_TYPE_ENUM type)
{
    UINT32 freeTimer = TIMER_NULL;

    KeyScan_lockTimer();
    freeTimer = KeyScan_getFreeTimer();

    if(freeTimer != TIMER_NULL)
    {
        KeyScan_SysTimer[freeTimer].eventID = eventID;
        KeyScan_SysTimer[freeTimer].interval = interval;
        KeyScan_SysTimer[freeTimer].isEnabled = TRUE;
        KeyScan_SysTimer[freeTimer].type = type;
        KeyScan_SysTimer[freeTimer].counter = 0;
    }
    KeyScan_unlockTimer();

    return freeTimer;
}

/**
  KNLstopTimer

  @param timerID
  @return timer ID
*/
void KeyScan_stopTimer(UINT32 * timerID)
{
    if((!timerID)||((* timerID)>KEYSCAN_SYSTIMER_AMOUNT))
    {
        debug_err(("^R stopTimer id error\r\n"));
        return ;
    }
    KeyScan_lockTimer();

    KeyScan_SysTimer[*timerID].counter = 0;
    KeyScan_SysTimer[*timerID].interval = 0;
    KeyScan_SysTimer[*timerID].eventID = NVTEVT_NULL;
    KeyScan_SysTimer[*timerID].isEnabled = FALSE;
    KeyScan_SysTimer[*timerID].type = TIMER_TYPE_ONE_SHOT;
    KeyScan_unlockTimer();

    *timerID = TIMER_NULL;
}

void KeyScan_PostNvtTPEvent(GESTURE_GESTURE_STRUCT* pGesture)
{
    CONTROL_TP_VENT TempTPEvent;

    debug_msg("TP:Event=%d, X=%03d, Y=%03d\r\n",pGesture->Event,pGesture->PosX,pGesture->PosY);

    switch(pGesture->Event)
    {
        case GESTURE_EVENT_PRESS:
        {
            TempTPEvent = NVTEVT_PRESS;
            break;
        }

        case GESTURE_EVENT_RELEASE:
        {
            TempTPEvent = NVTEVT_RELEASE;
            break;
        }

        case GESTURE_EVENT_MOVE:
        {
            TempTPEvent = NVTEVT_MOVE;
            break;
        }

        case GESTURE_EVENT_CLICK:
        {
            TempTPEvent = NVTEVT_CLICK;
            break;
        }

        case GESTURE_EVENT_DOUBLE_CLICK:
        {
            TempTPEvent = NVTEVT_DOUBLECLICK;
            break;
        }

        case GESTURE_EVENT_SLIDE_UP:
        {
            TempTPEvent = NVTEVT_SLIDE_UP;
            break;
        }

        case GESTURE_EVENT_SLIDE_DOWN:
        {
            TempTPEvent = NVTEVT_SLIDE_DOWN;
            break;
        }

        case GESTURE_EVENT_SLIDE_LEFT:
        {
            TempTPEvent = NVTEVT_SLIDE_LEFT;
            break;
        }

        case GESTURE_EVENT_SLIDE_RIGHT:
        {
            TempTPEvent = NVTEVT_SLIDE_RIGHT;
            break;
        }

        default:
        {
            debug_err(("Gesture %d is not handled!",pGesture->Event));
            return;
        }
    }

    Ux_PostEvent(TempTPEvent, 2, pGesture->PosX, pGesture->PosY);
}

void KeyScan_SetDetTP(BOOL IsEnable)
{
    g_bKeyScanTP = IsEnable;
}

//@}
