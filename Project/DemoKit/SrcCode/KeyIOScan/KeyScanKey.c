/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanKey.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, modedial
                Scan key, modedial

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "KeyScanInt.h"
#include "KeyScanTsk.h"
#include "GPIOMapping.h"
#include "PrimaryTsk.h"
#include "PlaySoundTsk.h"
#include "FileSysTsk.h"
#include "Fs_file_op.h"
#include "SysCfg.h"
#include "Utility.h"
#include "UIFlow.h"
#include "Interface.h"

#define KEY_HOLD_COUNT          10
#define KEY_HOLD_CONTINUE       1//2
#define KEY_LONG_HOLD_COUNT     18//9

// -------------------------------------------------------------------
// Static variables
// -------------------------------------------------------------------
static UINT32       uiLastKeyStatus         = 0;
static UINT32       uiLastMDStatus          = 0;
static UINT32       uiStableKeyStatus       = 0;
//static UINT32       uiStableMDStatus        = 0;
static UINT32       uiLastStableKeyStatus   = 0;
static UINT32       uiKeyMaskPress          = FLGKEY_KEY_MASK_DEFAULT;
static UINT32       uiKeyMaskRelease        = FLGKEY_KEY_REL_MASK_DEFAULT;
static UINT32       uiKeyMaskContinue       = FLGKEY_KEY_CONT_MASK_DEFAULT;
static UINT32       uiKeyMaskLong           = 0;
static UINT32       uiKeyModeChgMaskPress   = FLGKEY_KEY_MASK_DEFAULT;

static UINT32       uiKeyHoldCount          = 0;
static UINT32       uiKeyLongHoldCount      = 0;
static BOOL         bKeyHoldFlag            = FALSE;

BOOL KeyScan_CheckFileExit(char* filename)
{
   HNVT_FILE   *pFile;
   BOOL         bStatus;

    debug_err(("----->IN KeyScan_CheckEngModeTextFile3 \r\n"));
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        debug_err(("Card is not exist opening %s\r\n", filename));
        return FALSE;
    }
    FilesysWaitInitFinish(FST_TIME_INFINITE);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    TimerDelayMs(50);
    pFile = Filesys_fopen((char *)filename, "R");
    bStatus = (pFile == NULL) ? FALSE : TRUE;

    FilesysCloseActFile();

    if (!bStatus)
    {
        debug_err(("Error opening %s\r\n", filename));
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


BOOL KeyScan_CheckEngModeTextFile(void)
{
    return InterFace_CheckPrjFile();
}

BOOL KeyScan_CheckUpdateCfgTextFile(void)
{
    return KeyScan_CheckFileExit("A:\\NVTCFG.txt");
}

BOOL KeyScan_CheckUpdateLogoTextFile(void)
{
    return KeyScan_CheckFileExit("A:\\logo.jpg");
}

BOOL KeyScan_CheckUpdateVersionTextFile(void)
{
    return KeyScan_CheckFileExit("A:\\NTCONFIG.bin");
}

BOOL KeyScan_CheckEngMode(void)
{
    UINT32 i = 0;

    //while (gpio_getPin(GPIO_KEY_MODE) == 0)
    //while (KeyScan_CheckEngModeTextFile())
    while(GPIOMap_CheckEngModeGpio() && KeyScan_CheckEngModeTextFile())
    {
        debug_err(("----->input_while \r\n")); //Jarkko tmp
        if (i == 5)
        {
            Primary_ChangeMode(PRIMARY_MODE_ENGINEER);
            return TRUE;
        }
        TimerDelayMs(10);
        i++;
    }
    return FALSE;
}

/**
  Show key name by input key code

  Show key name by input key code
  [KeyScan internal API]

  @param UINT32 uiKeyCode: Input key code detected from GPIO mapping
  @return void
*/
void KeyScan_ShowKeyName(UINT32 uiKeyCode, CHAR *pStr)
{
    return;

    if(uiKeyCode & FLGKEY_UP)
        debug_err(("KEYSCAN %s KEY UP\r\n", pStr));
    if(uiKeyCode & FLGKEY_DOWN)
        debug_err(("KEYSCAN %s KEY DOWN\r\n", pStr));
    if(uiKeyCode & FLGKEY_LEFT)
        debug_err(("KEYSCAN %s KEY LEFT\r\n", pStr));
    if(uiKeyCode & FLGKEY_RIGHT)
        debug_err(("KEYSCAN %s KEY RIGHT\r\n", pStr));
    if(uiKeyCode & FLGKEY_ENTER)
        debug_err(("KEYSCAN %s KEY ENTER\r\n", pStr));
    if(uiKeyCode & FLGKEY_MENU)
        debug_err(("KEYSCAN %s KEY MENU\r\n", pStr));
    if(uiKeyCode & FLGKEY_MODE)
        debug_err(("KEYSCAN %s KEY MODE\r\n", pStr));
    if(uiKeyCode & FLGKEY_ZOOMOUT)
        debug_err(("KEYSCAN %s KEY ZOOMOUT\r\n", pStr));
    if(uiKeyCode & FLGKEY_ZOOMIN)
        debug_err(("KEYSCAN %s KEY ZOOMIN\r\n", pStr));
    if(uiKeyCode & FLGKEY_DEL)
        debug_err(("KEYSCAN %s KEY DEL\r\n", pStr));
    if(uiKeyCode & FLGKEY_PLAYBACK)
        debug_err(("KEYSCAN %s KEY PLAYBACK\r\n", pStr));
    if(uiKeyCode & FLGKEY_SHUTTER1)
        debug_err(("KEYSCAN %s KEY SHUTTER1\r\n", pStr));
    if(uiKeyCode & FLGKEY_SHUTTER2)
        debug_err(("KEYSCAN %s KEY SHUTTER2\r\n", pStr));
    if(uiKeyCode & FLGKEY_DISPLAY)
        debug_err(("KEYSCAN %s KEY DISPLAY\r\n", pStr));
}

//#NT#2011/07/18#KS Hung -begin
/**
  Show FW version

  Show FW version
  [KeyScan internal API]

  @param UINT32 uiKeyCode: Input key code detected from GPIO mapping
  @return void
*/
BOOL KeyScan_CheckShowFwVersion(void)
{
/*UINT32 rec,i;
//debug_err(("henry--------------11111111111111\r\n"));
//debug_err(("henry-----------%d\r\n",GPIOMap_DetKey()));
	for(i=0;i<2;i++)
    {
		rec = GPIOMap_DetKey();
		if(rec == FLGKEY_UP)
		{
			break;
		}
	}
    if (rec & FLGKEY_DOWN)
    {
    	//debug_err(("henry--------------33333\r\n"));
        return TRUE;
    }
    else
    {
        return FALSE;
    }*/
    if (GPIOMap_DetKey() & FLGKEY_UP)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//#NT#2011/07/18#KS Hung -end


/**
  Detect key is pressed or released

  Detect key is pressed or released and set flg (FLG_ID_KEY)
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetKey(void)
{
    UINT32          uiCurKeyStatus, uiTempKeyStatus;
    UINT32          uiKeyChanged;
    UINT32          uiKeyPressed, uiKeyReleased, uiKeyContinue;

    uiCurKeyStatus          =
    uiTempKeyStatus         = GPIOMap_DetKey();
    uiCurKeyStatus         &= (uiLastKeyStatus & (0x0000FFFF));//FLGKEY_KEY_MASK);

    uiKeyChanged            = uiCurKeyStatus ^ uiStableKeyStatus;

    uiKeyPressed            = uiKeyChanged & uiCurKeyStatus;
    uiKeyReleased           = uiKeyChanged & (~uiCurKeyStatus);

    uiKeyContinue           = uiCurKeyStatus & uiStableKeyStatus & uiLastStableKeyStatus;

    uiLastStableKeyStatus   = uiStableKeyStatus & ~(uiKeyContinue);
    uiStableKeyStatus       = uiCurKeyStatus;
    uiLastKeyStatus         = uiTempKeyStatus;

    if (uiKeyPressed)
    {
        //debug_err(("Key Pressed: 0x%.8X\r\n", uiKeyPressed));
        KeyScan_ShowKeyName(uiKeyPressed, "PRESSED");

        uiTempKeyStatus = uiKeyPressed & uiKeyMaskPress;

        if (uiTempKeyStatus)
        {
            // Play keypad tone
            //KeyScan_PlaySound(KEYSCAN_SOUND_KEY_OTHERS);
            InterFace_KeyScan_PlaySound(KEYSCAN_SOUND_KEY_OTHERS);

			//when LCD is turn off,press any key instead of Display key to turn on LCD.
	     InterFace_Get_WakeUp_BackLight_Key(&uiTempKeyStatus);

            // Key pressed event
            //debug_err(("Key Pressed : 0x%.8X\r\n", uiKeyPressed));
            g_uiKeyScanFlags |= (FLGKEY_PRESSED | uiTempKeyStatus);

            //#NT#2007/08/08@Janice Hunag::play tone if press masked key
            //if(g_uiKeyScanFlags)
                //KeyScan_PlayTone(0);  //play key tone when press
            //#NT#2007/08/08@Janice Hunag
        }
        #if 0
        if(!KeyScan_IsHDMIPlugIn()&&(!KeyScan_IsTVPlugIn()))
        {
        if((GPIOMap_DetKey() & !FLGKEY_UP)||(GPIOMap_DetKey() & !FLGKEY_DOWN))
            {
	        if(!GPIOMap_IsLCDBacklightOn())
	        {
			GPIOMap_TurnOnLCDBacklight();
			g_uiKeyScanFlags = 0;
		  }
            }
        }
        #endif
        // Reset auto power off counter
        g_uiKeyScanAutoPoweroffCnt = 0;
	g_uiKeyScanAutoLcdoffCnt = 0;		
    }

    if (uiKeyReleased)
    {
        //debug_err(("Key Released: 0x%.8X\r\n", uiKeyReleased));
        KeyScan_ShowKeyName(uiKeyReleased, "RELEASED");

        uiTempKeyStatus = uiKeyReleased & uiKeyMaskRelease;


        if (uiTempKeyStatus != 0)
        {
            // Key release event
            //debug_err(("Key Released: 0x%.8X\r\n", uiKeyReleased));
            if (bKeyHoldFlag == TRUE)
            {
                if (uiTempKeyStatus & (FLGKEY_MODE))
                   g_uiKeyScanFlags |= (FLGKEY_HOLD | FLGKEY_RELEASED | uiKeyReleased);
                else
                   g_uiKeyScanFlags |= (FLGKEY_RELEASED | uiKeyReleased);
            }
            else
            {
                g_uiKeyScanFlags |= (FLGKEY_RELEASED | uiKeyReleased);
            }
            bKeyHoldFlag = FALSE;
            uiKeyHoldCount = 0;
            uiKeyLongHoldCount = 0;
        }

        if (uiKeyReleased & uiKeyMaskContinue)
        {
            bKeyHoldFlag = FALSE;
            uiKeyHoldCount = 0;
            uiKeyLongHoldCount = 0;
        }

        // Reset auto power off counter
        g_uiKeyScanAutoPoweroffCnt = 0;
g_uiKeyScanAutoLcdoffCnt = 0;		
    }

    if (uiKeyContinue)
    {
        //debug_err(("Continue key: 0x%.8X\r\n", uiKeyContinue));
        //KeyScan_ShowKeyName(uiKeyContinue, "CONTINUE");

        uiTempKeyStatus = uiKeyContinue & (uiKeyMaskContinue & uiKeyMaskPress);   //If mask key press, would not send continue key

        if (uiTempKeyStatus)
        {
            uiKeyHoldCount++;
            if (bKeyHoldFlag == FALSE && uiKeyHoldCount > KEY_HOLD_COUNT ||
                bKeyHoldFlag == TRUE  && uiKeyHoldCount > KEY_HOLD_CONTINUE)
            {

                uiKeyHoldCount = 0;

                // Play keypad tone
                if (bKeyHoldFlag == FALSE)
                {
                    bKeyHoldFlag = TRUE;
                    //KeyScan_PlaySound(KEYSCAN_SOUND_KEY_OTHERS);
                }

                // Continue key pressed event
                //debug_err(("Continue key: 0x%.8X\r\n", uiKeyContinue));
                g_uiKeyScanFlags |= (FLGKEY_HOLD | uiTempKeyStatus);

                if (bKeyHoldFlag)
                {
                    uiKeyLongHoldCount++;
                    //debug_err(("uiKeyLongHoldCount = %d\r\n", uiKeyLongHoldCount));
                    if (uiKeyLongHoldCount > KEY_LONG_HOLD_COUNT)
                    {
                        //debug_err(("Long hold key: 0x%.8X\r\n", uiKeyContinue));
                        uiKeyLongHoldCount = 0;
                        g_uiKeyScanFlags &= ~FLGKEY_HOLD;
                        g_uiKeyScanFlags |= FLGKEY_LONGHOLD;
                    }
                }
            }
        }

        // Reset auto power off counter
        g_uiKeyScanAutoPoweroffCnt = 0;
g_uiKeyScanAutoLcdoffCnt = 0;		
    }
}

/**
  Scan Modedial status

  Scan Modedial status.
  [KeyScan internal API]

  @param BOOL bForceChangeMode: Force KeyScan task to change mode
  @return void
*/
void KeyScan_DetModedial(BOOL bForceChangeMode)
{
    UINT32          uiCurMDStatus, uiMDChanged, uiModedial;

    uiCurMDStatus       = GPIOMap_DetModeDial();

    uiMDChanged         = uiCurMDStatus ^ uiLastMDStatus;

    uiModedial          = uiCurMDStatus;

    if (uiMDChanged || bForceChangeMode)
    {
        switch (uiCurMDStatus)
        {
        case GPIOMAP_MD_PLAYBACK:
            Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
            break;

        case GPIOMAP_MD_MP3:
            Primary_ChangeMode(PRIMARY_MODE_MP3);
            break;

        case GPIOMAP_MD_VOICE:
            Primary_ChangeMode(PRIMARY_MODE_VOICE);
            break;

        case GPIOMAP_MD_USB:
            Primary_ChangeMode(PRIMARY_MODE_USB);
            break;

        case GPIOMAP_MD_SETUP:
            Primary_ChangeMode(PRIMARY_MODE_SETUP);
            break;

        case GPIOMAP_MD_MOVIE:
            Primary_ChangeMode(PRIMARY_MODE_MOVIE);
            break;

        case GPIOMAP_MD_PHOTO:
            Primary_ChangeMode(PRIMARY_MODE_PHOTO);
            break;

        case GPIOMAP_MD_BROWSER:
            Primary_ChangeMode(PRIMARY_MODE_BROWSER);
            break;

        default:
            uiModedial = uiLastMDStatus;
            break;
        }

        // Reset auto power off counter
        g_uiKeyScanAutoPoweroffCnt = 0;
    }

    uiLastMDStatus = uiModedial;

/*
    UINT32          uiCurMDStatus, uiTempMDStatus, uiMDChanged, uiModedial;
    UINT32          i, uiOneCounter;

    uiCurMDStatus       = uiLastKeyStatus & GPIOMAP_MD_MASK;

    uiTempMDStatus      = uiCurMDStatus;
    uiOneCounter        = 0;
    for (i=3; i>0; i--)
    {
        if (uiTempMDStatus & GPIOMAP_MD_PLAYBACK)
        {
            uiOneCounter++;
        }
        uiTempMDStatus >>= 1;
    }

    if (uiOneCounter > 1)
    {
        uiCurMDStatus   = uiLastMDStatus;
    }
    else
    {
        uiTempMDStatus  = uiLastKeyStatus & FLGKEY_KEY_MASK;
        uiOneCounter    = 0;
        for (i=12; i>0; i--)
        {
            if (uiTempMDStatus & FLGKEY_UP)
            {
                uiOneCounter++;
            }
            uiTempMDStatus >>= 1;
        }

        if (uiOneCounter > 1)
        {
            uiCurMDStatus   = uiLastMDStatus;
        }
    }
    uiTempMDStatus      = uiCurMDStatus;

    uiCurMDStatus      &= uiLastMDStatus;

    uiMDChanged         = uiCurMDStatus ^ uiStableMDStatus;
    uiModedial          = uiMDChanged & uiCurMDStatus;

    if (uiCurMDStatus != 0)
    {
        uiStableMDStatus= uiCurMDStatus;
    }
    uiLastMDStatus      = uiTempMDStatus;

    if (bForceChangeMode == TRUE)
    {
        // No stable status
        if (uiStableMDStatus == 0)
        {
            // Didn't detect valid status
            if (uiTempMDStatus == 0)
            {
                uiModedial = GPIOMAP_MD_PHOTO;
            }
            // Detect valid status
            else
            {
                uiModedial = uiTempMDStatus;
            }

            uiStableMDStatus = uiModedial;
        }
        // uiModedial = stable status
        else
        {
            uiModedial = uiStableMDStatus;
        }
    }

    if (uiModedial != 0)
    {
        switch (uiModedial)
        {
        case GPIOMAP_MD_MOVIE:
            Primary_ChangeMode(PRIMARY_MODE_MOVIE);
            break;

        case GPIOMAP_MD_PLAYBACK:
            Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
            break;

        case GPIOMAP_MD_PHOTO:
            Primary_ChangeMode(PRIMARY_MODE_PHOTO);
            break;

        default:
            break;
        }
    }
*/
}

/**
  Enable/Disable key detection

  Enable/Disable key detection

  @param BOOL bEn: TRUE -> Enable key detection, FALSE -> Disable key detection
  @return void
*/
void KeyScan_EnableKeyDet(BOOL bEn)
{
    g_bKeyScanKeyDetEn = bEn;
}

/**
  Enable/Disable modedial detection

  Enable/Disable modedial detection

  @param BOOL bEn: TRUE -> Enable modedial detection, FALSE -> Disable modedial detection
  @return void
*/
void KeyScan_EnableMDDet(BOOL bEn)
{
    g_bKeyScanMDDetEn = bEn;
}

/**
  KeyScan_SetPressKeyEventMask(UINT32 * evtTable)

  Mask / unmask key press event

  @param UINT32 * evtTable: Table of masked event
  @return void
*/
void KeyScan_SetPressKeyEventMask(UINT32 * evtTable)
{
    UINT32  uiKeyMask = 0;
    UINT32  uiMaskKeyCnt = 0;
    if(evtTable == NULL)
    {
        debug_ind(("KeyScan_SetPressKeyEventMask not put mask event default receive all\r\n"));
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK);
    }
    else
    {
        while(evtTable[uiMaskKeyCnt])
        {
            uiKeyMask |= Get_KeyEvt(evtTable[uiMaskKeyCnt]);
            uiMaskKeyCnt ++;
        }
        debug_ind(("KeyScan_SetPressKeyEventMask key: 0x%.8X\r\n", uiKeyMask));
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, (uiKeyMask & FLGKEY_KEY_MASK));
    }
}

/**
  Set key mask

  Set key mask, there are 4 different modes of key mask now.
  (1) KEYSCAN_KEYMODE_PRESS     (Pressed)
  (2) KEYSCAN_KEYMODE_RELEASE   (Released)
  (3) KEYSCAN_KEYMODE_CONTINUE  (Continue)
  (4) KEYSCAN_KEYMODE_LONG      (Long pressed)
  Key Scan task will only issue the flag if the bit of key mask is 1 and
  the event (pressed, released...) is happened.
  Use KeyScan_GetKeyMask() API to get current key mask before you set the
  new key mask.

  @param UINT32 uiMode: Which mode do you want to set the mask
    KEYSCAN_KEYMODE_PRESS       (Pressed)
    KEYSCAN_KEYMODE_RELEASE     (Released)
    KEYSCAN_KEYMODE_CONTINUE    (Continue)
    KEYSCAN_KEYMODE_LONG        (Long pressed)
  @param UINT32 uiMask: The mask you want to set (FLGKEY_UP, FLGKEY_DOWN...)
  @return void
*/
void KeyScan_SetKeyMask(UINT32 uiMode, UINT32 uiMask)
{
    switch (uiMode)
    {
    case KEYSCAN_KEYMODE_PRESS:
        uiKeyMaskPress          = uiMask;
        break;

    case KEYSCAN_KEYMODE_RELEASE:
        uiKeyMaskRelease        = uiMask;
        break;

    case KEYSCAN_KEYMODE_CONTINUE:
        uiKeyMaskContinue       = uiMask;
        break;

    case KEYSCAN_KEYMODE_LONG:
        uiKeyMaskLong           = uiMask;
        break;

    case KEYSCAN_KEYMODE_CHGMOD_PRESS:
        uiKeyModeChgMaskPress   = uiMask;
        break;

    default:
        break;
    }
}

/**
  Get key mask

  Get key mask, there are 4 different mode key mask now.
  (1) KEYSCAN_KEYMODE_PRESS     (Pressed)
  (2) KEYSCAN_KEYMODE_RELEASE   (Released)
  (3) KEYSCAN_KEYMODE_CONTINUE  (Continue)
  (4) KEYSCAN_KEYMODE_LONG      (Long pressed)
  Key Scan task will only issue the flag if the bit of key mask is 1 and
  the event (pressed, released...) is happened. This API will return the
  current key mask of the mode.

  @param UINT32 uiMode: Which mode do you want to get the mask
    KEYSCAN_KEYMODE_PRESS       (Pressed)
    KEYSCAN_KEYMODE_RELEASE     (Released)
    KEYSCAN_KEYMODE_CONTINUE    (Continue)
    KEYSCAN_KEYMODE_LONG        (Long pressed)
  @return UINT32: The mask (FLGKEY_UP, FLGKEY_DOWN...)
*/
UINT32 KeyScan_GetKeyMask(UINT32 uiMode)
{
    switch (uiMode)
    {
    case KEYSCAN_KEYMODE_PRESS:
        return uiKeyMaskPress & uiKeyModeChgMaskPress;

    case KEYSCAN_KEYMODE_RELEASE:
        return uiKeyMaskRelease;

    case KEYSCAN_KEYMODE_CONTINUE:
        return uiKeyMaskContinue;

    case KEYSCAN_KEYMODE_LONG:
        return uiKeyMaskLong;

    default:
        return 0;
    }
}

//@}
