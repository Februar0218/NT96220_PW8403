/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlPlayback.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for Playback mode
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
#include "SystemClock.h"
#include "PLL.h"
#include "Alg_IPLMode.h"

/**
  Control the devices for Playback mode

  Control the devices before entering Playback mode.

  @param void
  @return void
*/
void DevCtrl_ModePlayback(void)
{
    IPL_OBJ IplObj;
    if (IPL_GetMode() != IPL_MODE_OFF)
    {
        // Set to off mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }
    else
    {
        // Turn off SIE MCLK
        pll_disableClock(SIE_MCLK);
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

    // Turn on LCD backlight if TV is un plugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE &&
        KeyScan_IsTVPlugIn() == FALSE &&
        KeyScan_IsHDMIPlugIn() == FALSE)
    {
        //GPIOMap_TurnOnLCDBacklight();
    }

    SysClk_SetClk(SPEED_ID_PLAYBACK);
}

//@}
