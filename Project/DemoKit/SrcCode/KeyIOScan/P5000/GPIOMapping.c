/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMapphing.c
    @ingroup    mIPRJAPKeyIO

    @brief      GPIO mapping
                This file do GPIO initialization and GPIO detection

    @note       Nothing.
    @date       2005/12/05
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "SIF.h"
#include "GPIOMapping.h"
#include "KeyScanTsk.h"
#include "KeyADC.h"
#include "Utility.h"
#include "Pll.h"
#include "Top.h"
#include "PWM.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "IrRx.h"
#include "hdmitx.h"
#include "dma.h"
#include "UIFlow.h"

BOOL gStarSOSDet = FALSE;
static BOOL gBTVPlugInEn=FALSE;
//#NT#2011/06/23#HH Chuang -begin
//#NT#add sample code for new TOP API
static const PIN_GROUP_CONFIG vTopConfig[] =
{
    // Module name              pinmux config

    //
    // Storage configuration
    //
    {PIN_FUNC_SDIO,             PIN_SDIO_CFG_4BITS},
    {PIN_FUNC_SPI,              PIN_SPI_CFG_NORMAL},
    {PIN_FUNC_NAND,             PIN_NAND_CFG_NONE},

    //
    // Sensor/Scanner configuration
    //
#if 1
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_10BITS |
                                PIN_SENSOR_CFG_MCLK  | PIN_SENSOR_CFG_FLCTR},
#else
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_8BITS |
                                PIN_SENSOR_CFG_MCLK |
                                PIN_SENSOR_CFG_MES1 | PIN_SENSOR_CFG_FLCTR},
#endif                                
    {PIN_FUNC_SCANNER,          PIN_SCANNER_CFG_NONE},

    //
    // Serial interface configuration
    //
    {PIN_FUNC_I2C,              PIN_I2C_CFG_1CH},
    {PIN_FUNC_SIF,              PIN_SIF_CFG_NONE},
    {PIN_FUNC_UART,             PIN_UART_CFG_1CH},

    //
    // PWM configuration
    //
    {PIN_FUNC_PWM,              PIN_PWM_CFG_NONE},

    //
    // Display configuration
    //
#if   (_LCDTYPE_ == _LCDTYPE_TXDT240C_||_LCDTYPE_ == _LCDTYPE_LIL9341_)
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_MI_FMT3},
#elif (_LCDTYPE_ == _LCDTYPE_ALT020DSLN_J2_)
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL},
#elif(_LCDTYPE_ == _LCDTYPE_TXDT200LER_31V2_ )   
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGBD360},
#elif(_LCDTYPE_ == _LCDTYPE_HX8238_ )   
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL},    
#elif (_LCDTYPE_ == _LCDTYPE_AUCN01_)
    { PIN_FUNC_LCD, PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_CCIR656},   
#else
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGBD320},
#endif
    {PIN_FUNC_LCD2,             PINMUX_DISPMUX_SEL_NONE},
    {PIN_FUNC_TV,               PINMUX_TV_HDMI_CFG_NORMAL},
    {PIN_FUNC_HDMI,             PINMUX_TV_HDMI_CFG_NORMAL | PINMUX_HDMI_CFG_DDC | PINMUX_HDMIMODE_1280X720P60}
};
//#NT#2011/06/23#HH Chuang -end


/**
  Do GPIO pinmux initialization

  Initialize I/O pinmux

  @param void
  @return void
*/
void GPIOMap_PinmuxInit(void)
{
    // Initialize pinmux setting
    pinmux_init((PIN_GROUP_CONFIG *)vTopConfig);
}


/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void GPIOMap_Init(void)
{

    // Open GPIO driver
    gpio_open();

    // Storage Card detect
    gpio_setIntTypePol(GPIOINT_CARD_DETECT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_KEY_CARD_DETECT, TRUE);

    // Storage Card write protect
    gpio_setIntTypePol(GPIOINT_CARD_WP, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_KEY_CARD_WP, TRUE);

    // LCD
    //gpio_setDir(GPIO_LCD_BL_CTRL, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_LCD_BL_CTRL);

    //lcd rotate
    gpio_setIntTypePol(GPIOINT_LCD_ROTATE, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_LCD_ROTATE, TRUE);

	
    //Sensor rotate
    gpio_setIntTypePol(GPIOINT_SENSOR_ROTATE, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_SENSOR_ROTATE, TRUE);
#if 0
    // LCD reset
    gpio_setDir(GPIO_LCD_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LCD_RESET);
    TimerDelayMs(1);
    gpio_clearPin(GPIO_LCD_RESET);
    TimerDelayMs(10);
    gpio_setPin(GPIO_LCD_RESET);
    TimerDelayMs(120);
#endif
    //Back det
    gpio_setIntTypePol(GPIOINT_BACK_DET, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_KEY_BACK_DET, TRUE);

    // TV
     gpio_setIntTypePol(GPIOINT_TV_DETECT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    //gpio_setIntTypePol(GPIOINT_TV_DETECT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);
    dma_setKeyScanEN(DRAM_KEY_TV_DETECT, TRUE);

    // Sensor
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_RESET);

    // Key
    gpio_setIntTypePol(GPIOINT_KEY_REC, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_KEY_KEY_REC, TRUE);

    gpio_setIntTypePol(GPIO_KEY_PLAYBACK, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_KEY_KEY_PLAYBACK, TRUE);

    gpio_setIntTypePol(GPIOINT_KEY_MODE, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_KEY_KEY_MODE, TRUE);

    gpio_setIntTypePol(GPIOINT_KEY_MENU, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_KEY_KEY_MENU, TRUE);

    //LCD rotate
    gpio_setIntTypePol(GPIOINT_LCD_ROTATE, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_LCD_ROTATE, TRUE);

    gpio_setIntTypePol(GPIOINT_LCD_ROTATE2, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_LCD_ROTATE2, TRUE);

    // lcd close pin
    gpio_setIntTypePol(GPIOINT_LCD_CLOSE, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    dma_setKeyScanEN(DRAM_LCD_CLOSE, TRUE);


    //LDOx_OUT is used to GPIO
    pll_setLDO(PLL_LDO_ID_1, FALSE, FALSE, PLL_LDO_VOLTAGE_2_8V);
    pll_setLDOEn(PLL_LDO_ID_1, FALSE);

    //LDOx_OUT is used to GPIO
    pll_setLDO(PLL_LDO_ID_2, FALSE, FALSE, PLL_LDO_VOLTAGE_2_8V);
    pll_setLDOEn(PLL_LDO_ID_2, FALSE);

    // White LED
     gpio_setDir(VIDEO_FLOOD_LIGHT, GPIO_DIR_OUTPUT); //ERIC edit
     gpio_clearPin(VIDEO_FLOOD_LIGHT);  //ERIC edit
}

/**
  Detect Storage card is inserted or not

  Detect Storage card is inserted or not.
  Return TRUE if storage card is inserted, FALSE if storage card is removed.

  @param void
  @return BOOL: TRUE -> Storage card inserted, FALSE -> Storage card removed
*/
BOOL GPIOMap_DetStrgCard(void)
{
    return (gpio_getPin(GPIO_CARD_DETECT) == 0 ? TRUE : FALSE);
}

BOOL GPIOMap_IsFocusFar(void)
{
    return TRUE;
}

/**
  Detect Storage card is write protected or not

  Detect Storage card is write protected or not.
  Return TRUE if storage card is write protected, FALSE if storage card is not write protected.

  @param void
  @return BOOL: TRUE -> Storage card is write protected, FALSE -> Storage card is not write protected
*/
BOOL GPIOMap_DetStrgCardWP(void)
{
   return FALSE;
   // return (gpio_getPin(GPIO_CARD_WP) == 0 ? FALSE : TRUE);
}

/**
  Detect key is pressed.

  Detect key is pressed.
  Return key pressed status (refer to GPIOMapping.h)

  @param void
  @return UINT32
*/
UINT32 GPIOMap_DetKey(void)
{
  UINT32  uiKeyCode = 0;
#if 0//(_GSENSOR_FUNC_ == ENABLE)
   if (gpio_getPin(GPIO_KEY_MODE) == 1)
   {
       if(gStarSOSDet == TRUE)  
       	{
       //debug_ind(("GPIOMap_DetKey: GPIO_KEY_PLAYBACK\n\r"));
	debug_err(("----gsensor send DISPLAY Flag---\r\n"));	   
       uiKeyCode |= FLGKEY_DISPLAY;	   
       //InterFace_KeyScan_PlaySound(KEYSCAN_SOUND_KEY_OTHERS);       
       //Ux_PostEvent(NVTEVT_KEY_DISPLAY, 1, NVTEVT_KEY_PRESS);                           
       GSensorSensitivityReset();	
       	}
   }
#endif   
#if 0
   if (gpio_getPin(GPIO_KEY_REC) == 0)
   {
       debug_ind(("GPIOMap_DetKey: GPIO_KEY_REC\n\r"));
       uiKeyCode |= FLGKEY_SHUTTER2;
   }

   if (gpio_getPin(GPIO_KEY_PLAYBACK) == 0)
   {
       debug_ind(("GPIOMap_DetKey: GPIO_KEY_PLAYBACK\n\r"));
       uiKeyCode |= FLGKEY_PLAYBACK;
   }

   if (gpio_getPin(GPIO_KEY_MODE) == 0)
   {
       debug_ind(("GPIOMap_DetKey: GPIO_KEY_MODE\n\r"));
       uiKeyCode |= FLGKEY_MODE;
   }

   if (gpio_getPin(GPIO_KEY_MENU) == 0)
   {
       debug_ind(("GPIOMap_DetKey: GPIO_KEY_MENU\n\r"));
       uiKeyCode |= FLGKEY_MENU;
   }
#endif
   uiKeyCode |= KeyADC_GetKey();

   return uiKeyCode;
}

/**
  Detect mode dial

  Detect mode dial

  @param void
  @return UINT32
*/
UINT32 GPIOMap_DetModeDial(void)
{
    return FALSE;
}

/**
  Detect TV is plugging in or unplugged

  Detect TV is plugging in or unplugged.
  Return TRUE if TV is plugging in, FALSE if TV is unplugged.

  @param void
  @return BOOL: TRUE -> TV is plugging in, FALSE -> TV is unplugged
*/


BOOL GPIOMap_DetTV(void)
{
    //return (gpio_getPin(DRAM_KEY_TV_DETECT) ? TRUE :FALSE);
    //return (gpio_getPin(DRAM_KEY_TV_DETECT) ? FALSE : TRUE);
    return (gpio_getPin(GPIO_TV_DETECT) ? FALSE : TRUE);
    //return FALSE;
    //return gBTVPlugInEn;
    //return FALSE;
}
/**
  Detect Back is plugging in or unplugged

  Detect Back is plugging in or unplugged.
  Return TRUE if Back is plugging in, FALSE if Back is unplugged.

  @param void
  @return BOOL: TRUE -> Back is plugging in, FALSE -> Back is unplugged
*/
BOOL GPIOMap_DetBack(void)
{
    return (gpio_getPin(GPIO_BACK_DET) ==0 ? TRUE : FALSE);
}

void GPIOMap_SetTV(BOOL En)
{
     gBTVPlugInEn=En;
}

BOOL GPIOMap_GetTVStatus(void)
{
    return (gpio_getPin(GPIO_TV_DETECT) ? FALSE : TRUE);
     //return (gpio_getPin(DRAM_KEY_TV_DETECT) ? FALSE : TRUE);//eric edit
      //return (gpio_getPin(DRAM_KEY_TV_DETECT) ? TRUE : FALSE);//eric edit
     //return gBTVPlugInEn;
}

/**
  Detect HDMI is plugging in or unplugged

  Detect HDMI is plugging in or unplugged.
  Return TRUE if HDMI is plugging in, FALSE if HDMI is unplugged.

  @param void
  @return BOOL: TRUE -> HDMI is plugging in, FALSE -> HDMI is unplugged
*/
BOOL GPIOMap_DetHDMI(void)
{
    return hdmitx_checkHotplug();
}

void GPIOMap_SetNANDWp(void)
{
}

/**
  Detect AC power is plugging in or unplugged

  Detect AC power is plugging in or unplugged.
  Return TRUE if AC power is plugging in, FALSE if AC power is unplugged.

  @param void
  @return BOOL: TRUE -> AC power is plugging in, FALSE -> AC power is unplugged
*/
BOOL GPIOMap_DetACPower(void)
{
    return FALSE;
}

/**
  Detect Audio is plugging in or unplugged

  Detect Audio is plugging in or unplugged.
  Return TRUE if Audio is plugging in, FALSE if Audio is unplugged.

  @param void
  @return BOOL: TRUE -> Audio is plugging in, FALSE -> Audio is unplugged
*/
BOOL GPIOMap_DetAudio(void)
{
    return FALSE;
}

/**
  Detect power off key is pressed or not

  Detect power off key is pressed or not.
  Return TRUE if power off key is pressed, FALSE if power off key is released.

  @param void
  @return BOOL: TRUE -> power off key is pressed, FALSE -> power off key is released
*/
BOOL GPIOMap_DetPoweroff(void)
{
    return FALSE;
}

/**
  Detect macro

  Detect macro

  @param void
  @return BOOL: TRUE -> Is macro, FALSE -> Isn't macro
*/
BOOL GPIOMap_DetMacro(void)
{
    return FALSE;
}

#if 0
#pragma mark -
#endif

//#NT#2008/12/16#Philex Lin -begin
//#NT#Add key detection of shutter 1&2.
/**
  Detect Shutter1 is pressed or not

  Detect Shutter1 is pressed or not.

  @param void
  @return BOOL: TRUE -> Shutter1 is pressed, FALSE -> Shutter1 is released
*/
BOOL GPIOMap_DetShutter1(void)
{
    return FALSE;
}

/**
  Detect Shutter2 is pressed or not

  Detect Shutter2 is pressed or not.
  Specifically designed usage for continue capture process.

  @param void
  @return BOOL: TRUE -> Shutter2 is pressed, FALSE -> Shutter2 is released
*/
BOOL GPIOMap_DetShutter2(void)
{
    return FALSE;
}
//#NT#2008/12/16#Philex Lin -end

#if 0
#pragma mark -
#endif


/**
  Turn on LCD backlight

  Turn on LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOnLCDBacklight(void)
{
    //debug_msg("^R%s\r\n",__func__);
    //gpio_setPin(GPIO_LCD_BL_CTRL);
    pll_setLDOEn(PLL_LDO_ID_1, TRUE);
}

/**
  Turn off LCD backlight

  Turn off LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDBacklight(void)
{
    //debug_msg("^R%s\r\n",__func__); 
    //gpio_clearPin(GPIO_LCD_BL_CTRL);
     pll_setLDOEn(PLL_LDO_ID_1, FALSE); //eric edit
}

/**
  Check whether LCD backlight is on or not

  Check whether LCD backlight is on or not.
  Return TRUE if LCD backlight is on, return FALSE if LCD backlight is off.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsLCDBacklightOn(void)
{
    //return (BOOL)gpio_getPin(GPIO_LCD_BL_CTRL);
    return pll_getLDOEn(PLL_LDO_ID_1);
}

#if 0
#pragma mark -
#endif

/**
  Turn on LCD power

  Turn on LCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOnLCDPower(void)
{
 #if (_LCDTYPE_ == _LCDTYPE_TXDT240C_)
    gpio_setPin(GPIO_LCD_BL_PWR);
 #endif
}

/**
  Turn off LCD power

  Turn off LCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDPower(void)
{
 #if (_LCDTYPE_ == _LCDTYPE_TXDT240C_)
    gpio_clearPin(GPIO_LCD_BL_PWR);
 #endif
}

/**
  Check whether LCD power is on or not

  Check whether LCD power is on or not.
  Return TRUE if LCD power is on, return FALSE if LCD power is off.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsLCDPowerOn(void)
{
 #if (_LCDTYPE_ == _LCDTYPE_TXDT240C_)
    return (BOOL)gpio_getPin(GPIO_LCD_BL_PWR);
 #else
     return TRUE;
 #endif
}

void GPIOMap_SetLCDSifPin(LCD_SIFPIN_DEF* pSif)
{
    pSif->uigpio_sen = GPIO_LCD_SIF_SEN;
    pSif->uigpio_clk = GPIO_LCD_SIF_SCK;
    pSif->uigpio_data = GPIO_LCD_SIF_SDA;
}

#if 0
#pragma mark -
#endif

/**
  Turn on Motor power

  Turn on Motor power.

  @param void
  @return void
*/
void GPIOMap_TurnOnMotorPower(void)
{
}

/**
  Turn off Motor power

  Turn off Motor power.

  @param void
  @return void
*/
void GPIOMap_TurnOffMotorPower(void)
{
}

/**
  Check whether Motor power is on or not

  Check whether Motor power is on or not.
  Return TRUE if Motor power is on, return FALSE if Lens power is off.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsMotorPowerOn(void)
{
    return TRUE;
}

#if 0
#pragma mark -
#endif

/**
  Turn on CCD power

  Turn on CCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOnSensorPower(void)
{
}

/**
  Turn off CCD power

  Turn off CCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffSensorPower(void)
{
    // LDO setting
//    pll_setLDOEn(PLL_LDO_ID_1, FALSE);
//    pll_setLDOEn(PLL_LDO_ID_2, FALSE);
}

/**
  Check whether CCD power is on or not

  Check whether CCD power is on or not.
  Return TRUE if CCD power is on, return FALSE if CCD power is off.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsSensorPowerOn(void)
{
    return TRUE;
}

/**
  Sensor HW reset
*/
void GPIOMap_ResetSensor(void)
{
   //gpio_clearPin(GPIO_SENSOR_RESET);
    debug_err(("H22 Reset\r\n"));
    gpio_setPin(GPIO_SENSOR_RESET);//enable sensor power
    TimerDelayMs(10);
    gpio_setPin(GPIO_SENSOR_RESET);//enable sensor power   
}

/**
  Sensor HW resume
*/
void GPIOMap_ResumeSensor(void)
{
   gpio_setPin(GPIO_SENSOR_RESET);
   //gpio_setPin(GPIO_LENS_IO_IN_2);
}

#if 0
#pragma mark -
#endif

/**
  Turn on LED

  Turn on LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return void
*/
void GPIOMap_TurnOnLED(UINT32 uiLED)
{

    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
        pll_setLDOEn(PLL_LDO_ID_2, TRUE);
        break;
    case GPIOMAP_LED_RED:
        //pll_setLDOEn(PLL_LDO_ID_1, TRUE);
        break;
    case GPIOMAP_LED_FCS:
    default:
        break;
    }
}

/**
  Turn off LED

  Turn off LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return void
*/
void GPIOMap_TurnOffLED(UINT32 uiLED)
{
    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
        pll_setLDOEn(PLL_LDO_ID_2, FALSE);
        break;
    case GPIOMAP_LED_RED:
        //pll_setLDOEn(PLL_LDO_ID_1, FALSE);
        break;
    case GPIOMAP_LED_FCS:
    default:
        break;
    }
}

/**
  Check whether LED is on or not

  Check whether LED is on or not.
  Return TRUE if LED is on, return FALSE if LED is off.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return BOOL
*/
BOOL GPIOMap_IsLEDOn(UINT32 uiLED)
{
    return FALSE;
}

/**
  Turn on Flood LED

  Turn on Flood LED.
  @return void
*/
void GPIOMap_TurnOnFloodLight(void)
{
        gpio_setPin(VIDEO_FLOOD_LIGHT);//ERIC edit
       //gpio_setPin(VIDEO_FLOOD_LIGHT);//ERIC edit
       //pll_setLDOEn(PLL_LDO_ID_1, TRUE);    
   
}

/**
  Turn off Flood LED

  Turn off Flood LED.
  @return void
*/
void GPIOMap_TurnOffFloodLight(void)
{
       gpio_clearPin(VIDEO_FLOOD_LIGHT);//ERIC edit
       //pll_setLDOEn(PLL_LDO_ID_1, FALSE);    
   
}

/**
  Check whether LED is off or not

  Check whether LED is off or not.
  Return TRUE if LED is off, return FALSE if LED is off.

  @return BOOL
*/
BOOL GPIOMap_IsFloodLightOn(void)
{
	//return pll_getLDOEn(PLL_LDO_ID_1);
    return (BOOL)gpio_getPin(VIDEO_FLOOD_LIGHT);
}

#if 0
#pragma mark -
#endif

/**
  Charge flash or stop charging flash

  Charge flash or stop charging flash.

  @param BOOL bCharge: TRUE -> Charge flash, FALSE -> Stop charging flash
  @param BOOL bHighCurrent: TRUE -> Charging with high current, FALSE -> Charging with low current
  @return void
*/
void GPIOMap_ChargeFlash(BOOL bCharge, BOOL bHighCurrent)
{
    /*
    if (bCharge == TRUE)
    {
        gpio_setPin(GPIO_FLASH_CHARGE);
    }
    else
    {
        gpio_clearPin(GPIO_FLASH_CHARGE);
    }
    */
}

/**
  Check whether flash is charging or not

  Check whether flash is charging or not.
  Return TRUE if flash is charging, otherwise return FALSE.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsFlashCharging(void)
{
    /*
    return (BOOL)gpio_getPin(GPIO_FLASH_CHARGE);
    */
    return FALSE;
}

BOOL GPIOMap_IsFlashReady(void)
{
    return TRUE;
}

void GPIOMap_TriggerFlash(BOOL bOn)
{
}

#if 0
#pragma mark -
#endif

/**
  Output poweroff control signal to high

  Output poweroff control signal to high.

  @param void
  @return void
*/
void GPIOMap_SetPoweroffCtrlHigh(void)
{
    /*
    gpio_setPin(GPIO_POWEROFF_CTRL);
    */
}

/**
  Output poweroff control signal to low

  Output poweroff control signal to low.

  @param void
  @return void
*/
void GPIOMap_SetPoweroffCtrlLow(void)
{
    /*
    gpio_clearPin(GPIO_POWEROFF_CTRL);
    */
}

BOOL GPIOMap_CheckEngModeGpio(void)
{

    if (GPIOMap_DetKey() & FLGKEY_SHUTTER2)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void GPIOMap_SetTPPin(TP_INIT_STRUCT* pInit)
{
}

void GPIOMap_SetLensGpio(LENS_DEVICE_OBJ* pLensDevice)
{
}

void GPIOMap_SetIrRxGpio(PIRRX_DEVICE_OBJ pIrRxDevice)
{
    pIrRxDevice->uiGPIOPin          = GPIO_IR_RECEIVER;
    pIrRxDevice->uiGPIOPad          = PAD_IR_RECEIVER;
    pIrRxDevice->uiGPIOIntr         = IR_RECEIVER_INTR_ID;
}

BOOL GPIOMap_DetSensorRotate(void)
{
#if 1
  return FALSE;
#else
    //return SysGetFlag(FL_SENSOR_ROTATE);
   // debug_err(("----sensor rotate --- %d--\r\n",gpio_getPin(GPIO_SENSOR_ROTATE)));
   if (gpio_getPin(GPIO_SENSOR_ROTATE))   
   {
       return TRUE;
   }    
   else
   {
	return FALSE;	
   }
#endif   
}

BOOL GPIOMap_DetLCDRotate(void)
{
   //if (gpio_getPin(GPIO_LCD_ROTATE)||gpio_getPin(GPIO_LCD_ROTATE2))
   if (gpio_getPin(GPIOINT_LCD_ROTATE)||gpio_getPin(GPIO_LCD_ROTATE2))   
   {
       return TRUE;
   } else {
       return FALSE;
   }
}

BOOL GPIOMap_DetLCDClose(void)
{
#if 1
   return FALSE;
#else
   if (gpio_getPin(GPIO_LCD_CLOSE))
   {
       return TRUE;
   } else {
       return FALSE;
   }
#endif   
}

//@}
