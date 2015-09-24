 /**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyADC.c
    @ingroup    mIPRJAPKeyIO

    @brief      Key detection by using ADC
                Key detection by using ADC

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "GPIOMapping.h"
#include "KeyScanTsk.h"
#include "KeyADC.h"
#include "ADC.h"

// ADC channel for key group 1 detection
#define KEY_ADC_CH_1            ADC_CHANNEL_1


// ADC level for key detection
#define KEY_ADC_LOW              0//0
#define KEY_ADC_LEVEL_1         240//171//115//246
#define KEY_ADC_LEVEL_2         510//344//270//480
#define KEY_ADC_LEVEL_3         840//510//410//677
#define KEY_ADC_LEVEL_4         950//637//536//819
#define KEY_ADC_LEVEL_5         955//790//700//883


// ADC error range
#define ADC_ERR                 40

static UINT32 gAdcRead = 4;

/**
  Judge key code by reading value from ADC

  Judge key code by reading value from ADC

  @param void
  @return void
*/

UINT32 KeyADC_GetKey(void)
{
    static UINT32 uiPreKey1 = 0, uiKeyCode = 0;
    UINT32 ad1 = 0, key1 = 0;
    static UINT32 AD2 = 0;
    ad1 = adc_readData(KEY_ADC_CH_1);
    //if(abs(AD2 - ad1) > 40)
        //debug_err(("^Gad1 = %d\n\r", ad1));
		
   // if (ad1!=1023)
        //debug_err(("^Gad1 = %d\n\r", ad1));

    //if ((ad1 >= KEY_ADC_LOW-25) && (ad1 < KEY_ADC_LOW + 25))
    if ((ad1 >= KEY_ADC_LOW) && (ad1 < KEY_ADC_LOW + ADC_ERR))
    {
        //debug_err(("KeyADC_GetKey: FLGKEY_UP\n\r"));
        key1 =  FLGKEY_MENU;//FLGKEY_UP;
    }
    else if ((ad1 > KEY_ADC_LEVEL_1 - ADC_ERR) && (ad1 < KEY_ADC_LEVEL_1 + ADC_ERR))
    {
        //debug_err(("KeyADC_GetKey: FLGKEY_DOWN\n\r"));
        key1 =  FLGKEY_MODE;//
    }
    else if ((ad1 > KEY_ADC_LEVEL_2 - ADC_ERR) && (ad1 < KEY_ADC_LEVEL_2 + ADC_ERR))
    {
        //debug_err(("KeyADC_GetKey: FLGKEY_ENTER\n\r"));
        key1 = FLGKEY_DOWN;//FLGKEY_ENTER;
    }
    else if ((ad1 > KEY_ADC_LEVEL_3 - ADC_ERR) && (ad1 < KEY_ADC_LEVEL_3 + ADC_ERR))
    {
        //debug_err(("KeyADC_GetKey: FLGKEY_MODE\n\r"));
        key1 = FLGKEY_ENTER; //
    }
    else if ((ad1 > KEY_ADC_LEVEL_4 - ADC_ERR) && (ad1 < KEY_ADC_LEVEL_4 + ADC_ERR))
    {
        //debug_err(("KeyADC_GetKey: FLGKEY_UP\n\r"));
        key1 = FLGKEY_UP;//FLGKEY_MENU;
    }

    if (key1 == uiPreKey1)
    {
        uiKeyCode = key1;
    }
    uiPreKey1 = key1;
    AD2 = ad1;
    return uiKeyCode;
}

/**
  Initialize key ADC detection

  Initialize key ADC detection

  @param void
  @return void
*/
void KeyADC_Init(void)
{
    debug_ind(("KeyADC_Init\r\n"));

    if (adc_open(KEY_ADC_CH_1) != E_OK)
    {
        debug_err(("Can't open ADC for key group 1\r\n"));
        return;
    }
    // Set sampling rate to 40KHz and sequential sampling mode
    // Each channel will sample once about 100 us
    adc_setControl(36, TRUE);
    // Set mode for key group 1 detection
    adc_setMode(KEY_ADC_CH_1, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);
    // Enable adc control logic
    adc_enable();
    //adc_triggerOneShot(ADC_CHANNEL_1);
}

void KeyADC_SetReadChannel(UINT32 ch)
{
    gAdcRead = ch;
}

void KeyADC_ReadChannel(void)
{
}

//@}
