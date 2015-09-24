/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanTV.c
    @ingroup    mIPRJAPKey

    @brief      Detect TV is plugging in or unplugged
                Detect TV is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "UIFlow.h"

static BOOL     bSensorStatus       = FALSE;
static BOOL     bLastSensorDet      = FALSE;
static BOOL     bLastSensorStatus   = FALSE;

/**
  Detect Sensor flag is roated or not

  Detect Sensor flag is roated or not.

  @param void
  @return void
*/
void KeyScan_DetSensorRotate(void)
{
    BOOL            bCurSensorDet, bCurSensorStatus;
    PrimaryModeID   CurrMode;

    if ((KeyScan_IsUSBPlug() == TRUE) || (KeyScan_IsHDMIPlugIn() == TRUE))
         return;

    #if (UI_STYLE == UI_STYLE_DRIVE)
    //#NT#2011/11/24#Philex Lin -begin
    // can't rotate sensor in playback mode
    if (Primary_GetCurrentMode()==PRIMARY_MODE_PLAYBACK)
        return;
    else
        bCurSensorDet = GPIOMap_DetSensorRotate();
    //#NT#2011/11/24#Philex Lin -end
    #else
    bCurSensorDet = FALSE;
    #endif
    bCurSensorStatus = (BOOL)(bCurSensorDet & bLastSensorDet);
    CurrMode = Primary_GetCurrentMode();

    if (bCurSensorStatus != bLastSensorStatus)
    {
        if (bCurSensorStatus==TRUE)
            bSensorStatus = TRUE;
        else
            bSensorStatus = FALSE;
        Primary_ChangeMode(PRIMARY_MODE_DUMMY);
        Primary_Wait4ChangeModeDone();

        if (CurrMode == PRIMARY_MODE_PHOTO)
        {
            Primary_ChangeMode(PRIMARY_MODE_PHOTO); // back to current mode
            Primary_Wait4ChangeModeDone();
        } else if (CurrMode == PRIMARY_MODE_MOVIE)
        {
            if (gMovData.State != MOV_ST_REC)
            {
                 Primary_ChangeMode(PRIMARY_MODE_MOVIE); // back to current mode
                 Primary_Wait4ChangeModeDone();
            }
        }
    }

    bLastSensorDet     = bCurSensorDet;
    bLastSensorStatus  = bCurSensorStatus;
}


/**
  Return Sensor Rotate status

  Return Sensor Rotate status
  TRUE  -> Sensor rorate.
  FALSE -> Sensor not rotate.

  @param void
  @return BOOL: TRUE -> Sensor rorate, FALSE -> Sensor not rotate
*/
BOOL KeyScan_IsSensorRotate(void)
{
    return bSensorStatus;
}



//@}
