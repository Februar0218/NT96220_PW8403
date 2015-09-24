/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       VoltageDet.c
    @ingroup    mIPRJAPKeyIO

    @brief      Voltage detection module
                This file is the voltage detection module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "VoltageDet.h"
#include "Adc.h"
#include "GlobalVar.h"

#define VOLDET_FLASH_ADC_CH         ADC_CHANNEL_2
#define VOLDET_BATTERY_ADC_CH       ADC_CHANNEL_0

/*#define VOLDET_BATTERY_ADC_LVL0     300//375// 3.7V
#define VOLDET_BATTERY_ADC_LVL1     385//396//3.8v
#define VOLDET_BATTERY_ADC_LVL2     396//403//413//3.9v
#define VOLDET_BATTERY_ADC_LVL3     400//413//427//4.0v
*/
#define VOLDET_BATTERY_ADC_LVL0     365// 3.6V
#define VOLDET_BATTERY_ADC_LVL1     377//3.7v
#define VOLDET_BATTERY_ADC_LVL2     388//3.8v
#define VOLDET_BATTERY_ADC_LVL3     400//3.9Vv
#define VOLDET_BATTERY_ADC_TH       3

#define VOLDET_FLASH_LVL_UNKNOWN    0xFFFFFFFF
#define VOLDET_BATTERY_LVL_UNKNOWN  0xFFFFFFFF


/**
  Get battery voltage ADC value

  Get battery voltage ADC value

  @param void
  @return UINT32 ADC value
*/
UINT32 VolDet_GetBatteryADC(void)
{
    return adc_readData(VOLDET_BATTERY_ADC_CH);
}

/**
  Get flash voltage ADC value

  Get flash voltage ADC value

  @param void
  @return UINT32 ADC value
*/
UINT32 VolDet_GetFlashADC(void)
{
    return adc_readData(VOLDET_FLASH_ADC_CH);
}

/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/
void VolDet_Init(void)
{
    if (adc_open(VOLDET_FLASH_ADC_CH) != E_OK)
    {
        debug_err(("VOLDET ERR: Can't open ADC channel for flash detection\r\n"));
        return;
    }

    if (adc_open(VOLDET_BATTERY_ADC_CH) != E_OK)
    {
        debug_err(("VOLDET ERR: Can't open ADC channel for battery detection\r\n"));
        return;
    }

    // Set mode for flash voltage detection
    adc_setMode(VOLDET_FLASH_ADC_CH, TRUE, FALSE, NULL);

    // Set mode for battery voltage detection
    adc_setMode(VOLDET_BATTERY_ADC_CH, TRUE, FALSE, NULL);

    // Enable adc control logic
    adc_enable();
}

/**
  Get battery voltage level

  Get battery voltage level.
  If battery voltage level is VOLDET_BATTERY_LVL_EMPTY, it means
  that you have to power off the system.

  @param void
  @return UINT32 Battery Level, refer to VoltageDet.h -> VOLDET_BATTERY_LVL_XXXX
*/
UINT32 VolDet_GetBatteryLevel(void)
{
    static UINT32   uiPreBatteryLvl     = VOLDET_BATTERY_LVL_UNKNOWN;
    static UINT32   uiPreBatteryADC     = 0;
    static UINT32   uiRetBatteryLvl;
    UINT32          uiCurBatteryADC, uiCurBatteryLvl;

    static UINT32   ucIndex=0xff;
    static UINT32   ucTemp[10];
    int i;

    uiCurBatteryADC = VolDet_GetBatteryADC();

     //debug_err(("uiCurBatteryADC:%d\r\n",uiCurBatteryADC));

    if(ucIndex==0xff)
    {
        for(i=0;i<10;i++)
        {
            ucTemp[i]=uiCurBatteryADC;

        }
        ucIndex = 0;
    }
    else
    {
        if(ucIndex>=10)
        {
            ucIndex=0;
        }
        ucTemp[ ucIndex++ ]=uiCurBatteryADC;
        for(i=0,uiCurBatteryADC=0;i<10;i++)
        {
            uiCurBatteryADC+=ucTemp[i] ;

        }
        uiCurBatteryADC/=10;
    }

    // Rising
    if (uiCurBatteryADC > uiPreBatteryADC)
    {
        if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL3 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_3;
        }
        else if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL2 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_2;
        }
        else if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL1 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_1;
        }
        else if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL0 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_0;
        }
        else
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_EMPTY;
        }
    }
    // Falling
    else
    {
        if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL3 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_3;
        }
        else if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL2 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_2;
        }
        else if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL1 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_1;
        }
        else if (uiCurBatteryADC > (VOLDET_BATTERY_ADC_LVL0 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_0;
        }
        else
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_EMPTY;
        }
    }

    // Debounce
    if ((uiCurBatteryLvl == uiPreBatteryLvl) ||
        (uiPreBatteryLvl == VOLDET_BATTERY_LVL_UNKNOWN))
    {
        uiRetBatteryLvl = uiCurBatteryLvl;
    }
    uiPreBatteryLvl = uiCurBatteryLvl;

    //return VOLDET_BATTERY_LVL_3;
    return uiRetBatteryLvl;
}

/**
  Get flash voltage level

  Get flash voltage level.
  If battery voltage level is VOLDET_FLASH_LVL_EMPTY, it means
  that you have to charge the flash.

  @param void
  @return UINT32 Flash Level, refer to VoltageDet.h -> VOLDET_FLASH_LVL_XXXX
*/
UINT32 VolDet_GetFlashLevel(void)
{
    return VOLDET_FLASH_LVL_EMPTY;
}

BOOL VolDet_CheckBatteryLow(void)
{
    return FALSE;
}

//@}

