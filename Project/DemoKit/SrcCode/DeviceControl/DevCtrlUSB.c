/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlUSB.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for USB mode
                Control LCD/LCD backlight on/off, CCD power on/off.
                Lens...

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "DeviceCtrl.h"
#include "GPIOMapping.h"
#include "ide.h"
#include "Pll.h"
#include "SystemClock.h"
#include "PhotoTsk.h"
#include "ide.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "Utility.h"
#include "Alg_IPLMode.h"

/**
  Control the devices for USB MSDC/PCC mode

  Control the devices after leaving USB MSDC/PCC mode.

  @param void
  @return void
*/
void DevCtrl_ModeUSBRemove(void)
{
    // Enable IDE clock
    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    // Enable IDE
    ide_set_en(TRUE);
}

/**
  Control the devices for USB MSDC mode

  Control the devices before entering USB MSDC mode.

  @param void
  @return void
*/
void DevCtrl_ModeUSBMSDC(void)
{
    IPL_OBJ IplObj;

    UI_ClearOSD(_OSD_INDEX_BLACK);
    TimerDelayMs(10);

    KeyScan_TurnOffLED(KEYSCAN_LED_FCS);

    //#NT#2011/07/19#KS Hung -begin
    // Turn on LCD backlight to display MSDC/PCC window
#if 0
    // Turn off LCD backlight
    if (GPIOMap_IsLCDBacklightOn() == TRUE)
    {
        GPIOMap_TurnOffLCDBacklight();
    }
#endif
    //#NT#2011/07/19#KS Hung -end

    if (IPL_GetMode() != IPL_MODE_OFF)
    {
        // Set to off mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }

    // Turn off Sensor power
    if (GPIOMap_IsSensorPowerOn() == TRUE)
    {
        GPIOMap_TurnOffSensorPower();
    }

    //#NT#2011/07/19#KS Hung -begin
    // Turn on LCD backlight to display MSDC/PCC window
#if 0
    // Disable IDE
    ide_wait_frame_end();
    ide_set_en(FALSE);


    // Disable IDE Clock
    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
#endif
    //#NT#2011/07/19#KS Hung -end

    //pll_disableClock(UART_CLK);   // must open before remove USB
    pll_disableClock(IPE_CLK);
    pll_disableClock(IME_CLK);

    SysClk_SetClk(SPEED_ID_USBMSDC);
}

/**
  Control the devices for USB SICD mode

  Control the devices before entering USB SICD mode.

  @param void
  @return void
*/
void DevCtrl_ModeUSBSICD(void)
{
    IPL_OBJ IplObj;

    // Turn on LCD backlight if TV is un plugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE  && KeyScan_IsTVPlugIn() == FALSE)
    {
        GPIOMap_TurnOnLCDBacklight();
    }

    if (IPL_GetMode() != IPL_MODE_OFF)
    {
        // Set to off mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }

    // Disable IDE video1 output
    ide_disable_video(IDE_VIDEOID_1);

    // Disable IDE video2 output
    ide_disable_video(IDE_VIDEOID_2);

    // Turn off CCD power
    if (GPIOMap_IsSensorPowerOn() == TRUE)
    {
        GPIOMap_TurnOffSensorPower();
    }

    SysClk_SetClk(SPEED_ID_USBSICD);
}

/**
  Control the devices for USB PCC mode

  Control the devices before entering USB PCC mode.

  @param void
  @return void
*/
void DevCtrl_ModeUSBPCC(void)
{
    IPL_OBJ IplObj;

    UI_ClearOSD(_OSD_INDEX_BLACK);
    TimerDelayMs(10);
    KeyScan_TurnOffLED(KEYSCAN_LED_FCS);

    //#NT#2011/07/19#KS Hung -begin
    // Turn on LCD backlight to display MSDC/PCC window
#if 0
    // Turn off LCD backlight
    if (GPIOMap_IsLCDBacklightOn() == TRUE)
    {
        GPIOMap_TurnOffLCDBacklight();
    }

#endif
    //#NT#2011/07/19#KS Hung -end

    if (IPL_GetMode() != IPL_MODE_OFF)
    {
        // Set to off mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
        }

    // Turn on CCD power
    if (GPIOMap_IsSensorPowerOn() == FALSE)
    {
        GPIOMap_TurnOnSensorPower();
    }

    //#NT#2011/07/19#KS Hung -begin
    // Turn on LCD backlight to display MSDC/PCC window
#if 0
    // Disable IDE
    ide_wait_frame_end();
    ide_set_en(FALSE);

    // Disable IDE Clock
    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
#endif
    //#NT#2011/07/19#KS Hung -end

    // Set system clock to USB PCC mode
    SysClk_SetClk(SPEED_ID_USBPCC);
}

//@}
