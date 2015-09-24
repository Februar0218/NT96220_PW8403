/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlDummy.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for dummy mode
                Control device for dummy mode

    @note       Nothing.

    @date       2006/02/07
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "DeviceCtrl.h"
#include "Alg_IPLMode.h"

/**
  Control the devices for dummy mode

  Control the devices before entering dummy mode.

  @param void
  @return void
*/
void DevCtrl_ModeDummy(void)
{
 IPL_OBJ IplObj;

    if (IPL_GetMode() != IPL_MODE_OFF)
    {
        // Set to off mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }
}

//@}
