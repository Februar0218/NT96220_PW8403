/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanFlashLight.c
    @ingroup    mIPRJAPKeyIO

    @brief      Flash light control
                Flash light control

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "KeyScanTsk.h"
#include "VoltageDet.h"
#include "PrimaryTsk.h"
#include "KeyScanInt.h"

#define FLASH_CHARGE_TIMEOUT      (20000 / 200)//  (20000 / 250)       // 20 sec (based on 250 ms timer)

static volatile BOOL    bFlashChargingEn    = FALSE;
static volatile BOOL    bFlashCharging      = FALSE;
static UINT32   uiFlashChargeCount = 0;

/**
  Detect flash voltage

  Detect flash voltage and determine charge flash or stop charging
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetFlash(void)
{
    if (bFlashChargingEn == TRUE && GPIOMap_IsFlashReady() == FALSE)
    {
        if (!bFlashCharging)
        {
            bFlashCharging = TRUE;
            uiFlashChargeCount = 0;
            GPIOMap_ChargeFlash(TRUE, TRUE);
            debug_ind(("Charge flash\r\n"));
        }
    }
    else  // Stop charging
    {
        if (bFlashCharging)
        {
            bFlashCharging = FALSE;
            GPIOMap_ChargeFlash(FALSE, TRUE);
            debug_ind(("Stop charging flash\r\n"));
        }
    }

    // flash charge time-out protection
    if (bFlashCharging)
    {
        uiFlashChargeCount++;
        if (uiFlashChargeCount > FLASH_CHARGE_TIMEOUT)
        {
            uiFlashChargeCount = 0;
            KeyScan_EnableFlashCharging(FALSE);
            debug_err(("Flash charge timeout!\r\n"));
        }
    }
}

/**
  Enable/Disable flash charging

  Enable/Disable flash charging.
  If flash is charging now and you want to disable flash charging,
  this function call will stop flash charging immediately.

  @param BOOL bEn: TRUE -> Enable flash charging, FALSE -> Disable flash charging
  @return void
*/
void KeyScan_EnableFlashCharging(BOOL bEn)
{
    bFlashChargingEn = bEn;
    debug_ind(("KeyScan_EnableFlashCharging: bFlashChargingEn = %d\r\n", bFlashChargingEn));

    //start charge
    if (bEn == TRUE )
    {
        if (!bFlashCharging)
        {
            bFlashCharging = TRUE;
            uiFlashChargeCount = 0;
            GPIOMap_ChargeFlash(TRUE, TRUE);
            debug_ind(("Charge flash\r\n"));
        }
    }
    else    //stop charge
    {
        if (bFlashCharging)
        {
            bFlashCharging = FALSE;
            GPIOMap_ChargeFlash(FALSE, TRUE);
            debug_ind(("disable Stop charging flash\r\n"));
        }
    }
}

/**
  Check whether flash is charging or not

  Check whether flash is charging or not

  @param void
  @return BOOL: TRUE -> Flash is charging, FALSE -> Flash isn't charging
  @return void
*/
BOOL KeyScan_IsFlashCharging(void)
{
    return bFlashCharging;
}



//@}
