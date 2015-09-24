/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SensorFP.c
    @ingroup    mIPRJAPCfg

    @brief      Implement some functions for sensor object.
                These functions are required (?) for sensor object when Open_Sensor()

    @note       Nothing.

    @date       2007/01/23
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#include "SensorFP.h"
#include "GPIOMapping.h"
#include "VoltageDet.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "KeyScanTsk.h"


static DC**     pDCList;

/**
  Function for sensor object

  Function for sensor object.
  Charge Flash.

  @param BOOL bCharge: Charge flash or not
  @param BOOL bHighCurrent: Charge flash with high current or not
  @return void
*/
void SenFP_ChargeFlash(BOOL bCharge, BOOL bHighCurrent)
{
    GPIOMap_ChargeFlash(bCharge, bHighCurrent);
}

/**
  Function for sensor object

  Function for sensor object.
  Check if flash is ready.

  @param void
  @return BOOL: TRUE if flahs is ready
*/
BOOL SenFP_IsFlashReady(void)
{
    if (VolDet_GetFlashLevel() == VOLDET_FLASH_LVL_FULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
  Function for sensor object

  Function for sensor object.
  Get Flash ADC value.

  @param void
  @return UINT32 Flash ADC value
*/
UINT32 SenFP_GetFlashADCValue(void)
{
    return VolDet_GetFlashADC();
}

/**
  Function for sensor object

  Function for sensor object.
  Draw a string on OSD.

  @param CHAR *pString: String to draw
  @param UINT16 uiX: Start X
  @param UINT16 uiY: Start Y
  @param UINT16 uiColor: Color to draw string
  @return void
*/
void SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor)
{
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetTextColor(uiColor, _OSD_INDEX_GRAY, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], uiX, uiY, pString);   
    UI_EndScreen((UINT32)pDCList);
}

/**
  Function for sensor object

  Function for sensor object.
  Clear OSD.

  @param void
  @return void
*/
void SenFP_ClearOSD(void)
{
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_Clear(pDCList[GxGfx_OSD]);
    UI_EndScreen((UINT32)pDCList);
}

/**
  Function for sensor object

  Function for sensor object.
  Power off.

  @param BOOL bLensRet: Retract Lens or not
  @param BOOL bWriteNand: Write data to NAND or not
  @return void
*/
void SenFP_PowerOff(BOOL bLensRet, BOOL bWriteNand)
{
    KeyScan_Poweroff();
}

//@}
