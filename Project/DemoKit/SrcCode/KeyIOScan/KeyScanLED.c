/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanLED.c
    @ingroup    mIPRJAPKeyIO

    @brief      Control LED on/off
                Control LED on/off/toggle

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "KeyScanTsk.h"

static volatile BOOL    bGreenLEDToggleEn   = FALSE;
static volatile BOOL    bRedLEDToggleEn     = FALSE;
static volatile BOOL    bFCSLEDToggleEn     = FALSE;

static volatile BOOL    bGreenLEDOn         = FALSE;
static volatile BOOL    bRedLEDOn           = FALSE;
static volatile BOOL    bFCSLEDOn           = FALSE;

static volatile BOOL    bGreenLEDOriStatus  = FALSE;
static volatile BOOL    bRedLEDOriStatus    = FALSE;
static volatile BOOL    bFCSLEDOriStatus    = FALSE;

static volatile UINT32  uiGreenLEDCnt, uiRedLEDCnt, uiFCSLEDCnt;

/**
  Toggle LED

  Toggle LED
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_ToggleLED(void)
{
    static UINT32 uiGLedCnt   = 0;
    static UINT32 uiRLedCnt   = 0;
    static UINT32 uiFCSLedCnt = 0;

    // Green LED toggle enabled
    if (bGreenLEDToggleEn == TRUE)
    {
        uiGLedCnt++;

        if (uiGLedCnt >= uiGreenLEDCnt)
        {
            bGreenLEDOn = !bGreenLEDOn;

            // Toggle LED
            if (bGreenLEDOn == TRUE)
            {
                GPIOMap_TurnOnLED(GPIOMAP_LED_GREEN);
            }
            else
            {
                GPIOMap_TurnOffLED(GPIOMAP_LED_GREEN);
            }

            uiGLedCnt = 0;
        }
    }

    // Red LED toggle enabled
    if (bRedLEDToggleEn == TRUE)
    {
        uiRLedCnt++;

        if (uiRLedCnt >= uiRedLEDCnt)
        {
            bRedLEDOn = !bRedLEDOn;

            // Toggle LED
            if (bRedLEDOn == TRUE)
            {
                GPIOMap_TurnOnLED(GPIOMAP_LED_RED);
            }
            else
            {
                GPIOMap_TurnOffLED(GPIOMAP_LED_RED);
            }

            uiRLedCnt = 0;
        }
    }

    // FCS LED toggle enabled
    if (bFCSLEDToggleEn == TRUE)
    {
        uiFCSLedCnt++;

        if (uiFCSLedCnt >= uiFCSLEDCnt)
        {
            bFCSLEDOn = !bFCSLEDOn;

            // Toggle LED
            if (bFCSLEDOn == TRUE)
            {
                GPIOMap_TurnOnLED(GPIOMAP_LED_FCS);
            }
            else
            {
                GPIOMap_TurnOffLED(GPIOMAP_LED_FCS);
            }

            uiFCSLedCnt = 0;
        }
    }
}

/**
  Turn on LED

  Turn on LED

  @param UINT32 uiLED: Which LED, could be the following
    KEYSCAN_LED_GREEN
    KEYSCAN_LED_RED
    KEYSCAN_LED_FCS
  @return void
*/
void KeyScan_TurnOnLED(UINT32 uiLED)
{
    switch (uiLED)
    {
    case KEYSCAN_LED_GREEN:
        bGreenLEDOn         =
        bGreenLEDOriStatus  = TRUE;
        break;

    case KEYSCAN_LED_RED:
        bRedLEDOn           =
        bRedLEDOriStatus    = TRUE;
        break;

    case KEYSCAN_LED_FCS:
    default:
        bFCSLEDOn           =
        bFCSLEDOriStatus    = TRUE;
        break;
    }

    GPIOMap_TurnOnLED(uiLED);
}

/**
  Turn off LED

  Turn off LED

  @param UINT32 uiLED: Which LED, could be the following
    KEYSCAN_LED_GREEN
    KEYSCAN_LED_RED
    KEYSCAN_LED_FCS
  @return void
*/
void KeyScan_TurnOffLED(UINT32 uiLED)
{
    switch (uiLED)
    {
    case KEYSCAN_LED_GREEN:
        bGreenLEDOn         =
        bGreenLEDOriStatus  = FALSE;
        break;

    case KEYSCAN_LED_RED:
        bRedLEDOn           =
        bRedLEDOriStatus    = FALSE;
        break;

    case KEYSCAN_LED_FCS:
    default:
        bFCSLEDOn           =
        bFCSLEDOriStatus    = FALSE;
        break;
    }

    GPIOMap_TurnOffLED(uiLED);
}

/**
  Check whether LED is on or off

  Check whether LED is on or off.
  Return TRUE if LED is on, otherwise return FALSE.

  @param UINT32 uiLED: Which LED, could be the following
    KEYSCAN_LED_GREEN
    KEYSCAN_LED_RED
    KEYSCAN_LED_FCS
  @return BOOL
*/
BOOL KeyScan_IsLEDOn(UINT32 uiLED)
{
    return GPIOMap_IsLEDOn(uiLED);
}

/**
  Set LED toggle speed

  Set LED toggle speed

  @param UINT32 uiLED: Which LED you want to set, could be the following
    KEYSCAN_LED_GREEN
    KEYSCAN_LED_RED
    KEYSCAN_LED_FCS
  @param UINT32 uiSpeed: LED toggle speed
  @return void
*/
void KeyScan_SetLEDToggleSpeed(UINT32 uiLED, UINT32 uiSpeed)
{
    UINT32 uiCounter;

    switch (uiSpeed)
    {
    case KEYSCAN_LED_TSPEED_SLOW:
        uiCounter = 8;
        break;

    case KEYSCAN_LED_TSPEED_NORMAL:
        uiCounter = 4;
        break;

    case KEYSCAN_LED_TSPEED_FAST:
    default:
        uiCounter = 1;
        break;
    }

    switch (uiLED)
    {
    case KEYSCAN_LED_GREEN:
        uiGreenLEDCnt   = uiCounter;
        break;

    case KEYSCAN_LED_RED:
        uiRedLEDCnt     = uiCounter;
        break;

    case KEYSCAN_LED_FCS:
    default:
        uiFCSLEDCnt  = uiCounter << 1;
        break;
    }
}

/**
  Enable/Disable LED toggle

  Enable/Disable LED toggle

  @param UINT32 uiLED: Which LED you want to enable/disable, could be the following
    KEYSCAN_LED_GREEN
    KEYSCAN_LED_RED
    KEYSCAN_LED_FCS
  @param BOOL bEn: TRUE -> Enable LED toggle, FALSE -> Disable LED toggle
  @return void
*/
void KeyScan_EnableLEDToggle(UINT32 uiLED, BOOL bEn)
{
    switch (uiLED)
    {
    case KEYSCAN_LED_GREEN:
        // Enable LED toggle
        if (bEn == TRUE)
        {
            bGreenLEDToggleEn = TRUE;
        }
        // Disable LED toggle
        else
        {
            if (bGreenLEDToggleEn == TRUE)
            {
                bGreenLEDToggleEn   = FALSE;
                bGreenLEDOn         = bGreenLEDOriStatus;

                if (bGreenLEDOn == TRUE)
                {
                    GPIOMap_TurnOnLED(GPIOMAP_LED_GREEN);
                }
                else
                {
                    GPIOMap_TurnOffLED(GPIOMAP_LED_GREEN);
                }
            }
        }
        break;

    case KEYSCAN_LED_RED:
        // Enable LED toggle
        if (bEn == TRUE)
        {
            bRedLEDToggleEn = TRUE;
        }
        // Disable LED toggle
        else
        {
            if (bRedLEDToggleEn == TRUE)
            {
                bRedLEDToggleEn     = FALSE;
                bRedLEDOn           = bRedLEDOriStatus;

                if (bRedLEDOn == TRUE)
                {
                    CHKPNT;
                    GPIOMap_TurnOnLED(GPIOMAP_LED_RED);
                }
                else
                {
                    CHKPNT;
                    GPIOMap_TurnOffLED(GPIOMAP_LED_RED);
                }
            }
        }
        break;

    case KEYSCAN_LED_FCS:
    default:
        // Enable LED toggle
        if (bEn == TRUE)
        {
            bFCSLEDToggleEn = TRUE;
        }
        // Disable LED toggle
        else
        {
            if (bFCSLEDToggleEn == TRUE)
            {
                bFCSLEDToggleEn     = FALSE;
                bFCSLEDOn           = bFCSLEDOriStatus;

                if (bFCSLEDOn == TRUE)
                {
                    GPIOMap_TurnOnLED(GPIOMAP_LED_FCS);
                }
                else
                {
                    GPIOMap_TurnOffLED(GPIOMAP_LED_FCS);
                }
            }
        }
        break;
    }
}

//@}
