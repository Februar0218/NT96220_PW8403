/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlSetup.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for Setup mode
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
#include "Alg_IPLMode.h"

/**
  Control the devices for Setup mode

  Control the devices before entering Setup mode.

  @param void
  @return void
*/
void DevCtrl_ModeSetup(void)
{
    IPL_OBJ IplObj;

    //if (DevCtrl_GetIPLMode() != DEVCTRL_IPLMODE_OFF)
    if (IPL_GetMode() != IPL_MODE_OFF)
    {
        // Set to off mode
        //DevCtrl_SetIPLMode(DEVCTRL_IPLMODE_OFF);
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

    // Turn on LCD backlight if TV is un plugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE &&
        KeyScan_IsTVPlugIn() == FALSE &&
        KeyScan_IsHDMIPlugIn() == FALSE)
    {
        GPIOMap_TurnOnLCDBacklight();
    }

    //SysClk_SetClk(SPEED_ID_PLAYBACK);
}

//@}
