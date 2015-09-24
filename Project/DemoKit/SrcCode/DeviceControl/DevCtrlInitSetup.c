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
#include "Alg_IPLMode.h"

/**
  Control the devices for Setup mode

  Control the devices before entering Setup mode.

  @param void
  @return void
*/
void DevCtrl_ModeInitSetup(void)
{
    IPL_OBJ IplObj;

    if (IPL_GetMode() != IPL_MODE_OFF)
    {
        // Set to off mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }

    // Turn on LCD backlight if TV is unplugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE  && KeyScan_IsTVPlugIn() == FALSE
        && KeyScan_IsHDMIPlugIn() == FALSE)
    {
        GPIOMap_TurnOnLCDBacklight();
    }
}

//@}
