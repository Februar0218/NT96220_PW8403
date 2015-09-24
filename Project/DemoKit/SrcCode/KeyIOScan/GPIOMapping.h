/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMapphing.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of GPIO mapping
                This file is the header file of GPIO mapping

    @note       Nothing.
    @date       2005/12/05
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _GPIOMAPPING_H
#define _GPIOMAPPING_H

#include "pad.h"
#include "GPIO.h"
#include "SysCfg.h"
#include "LCDTV.h"
#include "TouchPanel.h"
#include "Lens.h"

#if (PRJ == NVT_EVB)
#include "NT96220_EVB/GPIOMappingPcb.h"
#elif (PRJ == APC3)
#include "APC3/GPIOMappingPcb.h"
#elif (PRJ == JXW)
#include "JXW/GPIOMappingPcb.h"
#elif (PRJ == V010N)
#include "V010N/GPIOMappingPcb.h"
#elif (PRJ == HA60)
#include "HA60/GPIOMappingPcb.h"
#elif (PRJ == HDV5160)
#include "HDV5160/GPIOMappingPcb.h"
#elif (PRJ == ADPK_DSC_DEMO)
#include "ADPK_DSC_DEMO/GPIOMappingPcb.h"
#elif (PRJ == P5000)
#include "P5000/GPIOMappingPcb.h"
#else
#include "NT96220_EVB/GPIOMappingPcb.h"
#endif

#define _GSENSOR_FUNC_ ENABLE //DISABLE
//--------------------------------------------------------------------
// Definitions for APIs
//--------------------------------------------------------------------
// Definitions for GPIOMap_EnableLED() and GPIOMap_IsLEDOn
#define GPIOMAP_LED_GREEN           0
#define GPIOMAP_LED_RED             1
#define GPIOMAP_LED_FCS             2

// Definitions for Mode Dial
#define GPIOMAP_MD_PLAYBACK         0x00000006      // Modedial PLAYBACK
#define GPIOMAP_MD_MP3              0x00000007      // Modedial MP3
#define GPIOMAP_MD_VOICE            0x00000003      // Modedial VOICE
#define GPIOMAP_MD_USB              0x0000000B      // Modedial USB
#define GPIOMAP_MD_SETUP            0x00000009      // Modedial SETUP
#define GPIOMAP_MD_MOVIE            0x0000000D      // Modedial MOVIE
#define GPIOMAP_MD_PHOTO            0x0000000F      // Modedial PHOTO
#define GPIOMAP_MD_BROWSER          0x0000000E      // Modedial BROWSER
#define GPIOMAP_MD_MASK             0x0000000F      // Modedial mask

typedef struct
{
    UINT32  SEN;
    UINT32  SCK;
    UINT32  SDA;
}GPIOMap_LCD_SIF_STRUCT;

extern void     GPIOMap_PinmuxInit(void);
extern void     GPIOMap_Init(void);

extern UINT32   GPIOMap_DetKey(void);
extern UINT32   GPIOMap_DetModeDial(void);

extern BOOL     GPIOMap_DetStrgCard(void);
extern BOOL     GPIOMap_DetStrgCardWP(void);

extern BOOL     GPIOMap_DetSDIO2Card(void);
extern BOOL     GPIOMap_DetSDIO2CardWP(void);

extern BOOL     GPIOMap_DetTV(void);
extern BOOL    GPIOMap_DetBack(void);
extern void GPIOMap_SetTV(BOOL En);
extern BOOL     GPIOMap_DetHDMI(void);
extern BOOL     GPIOMap_DetACPower(void);
extern BOOL     GPIOMap_DetAudio(void);
extern BOOL     GPIOMap_DetPoweroff(void);
extern BOOL     GPIOMap_DetMacro(void);

extern void     GPIOMap_TurnOnLCDBacklight(void);
extern void     GPIOMap_TurnOffLCDBacklight(void);
extern BOOL     GPIOMap_IsLCDBacklightOn(void);

extern void     GPIOMap_TurnOnLCDPower(void);
extern void     GPIOMap_TurnOffLCDPower(void);
extern BOOL     GPIOMap_IsLCDPowerOn(void);

extern void     GPIOMap_SetLCDSifPin(LCD_SIFPIN_DEF* pSif);

extern BOOL     GPIOMap_DetLCDRotate(void);
extern BOOL     GPIOMap_DetPower(void);

extern void     GPIOMap_TurnOnSensorPower(void);
extern void     GPIOMap_TurnOffSensorPower(void);
extern void     GPIOMap_ResetSensor(void);
extern void     GPIOMap_ResumeSensor(void);
extern BOOL     GPIOMap_IsSensorPowerOn(void);

extern void     GPIOMap_TurnOnLED(UINT32 uiLED);
extern void     GPIOMap_TurnOffLED(UINT32 uiLED);
extern BOOL     GPIOMap_IsLEDOn(UINT32 uiLED);

extern void     GPIOMap_ChargeFlash(BOOL bCharge, BOOL bHighCurrent);
extern BOOL     GPIOMap_IsFlashCharging(void);
extern BOOL     GPIOMap_IsFlashReady(void);

extern void     GPIOMap_SetPoweroffCtrlHigh(void);
extern void     GPIOMap_SetPoweroffCtrlLow(void);

extern void     GPIOMap_SetNANDWp(void);

extern void     GPIOMap_MShutOpen(void);
extern void     GPIOMap_MShutClose(void);
extern void     GPIOMap_MShutOff(void);

extern BOOL     GPIOMap_CheckEngModeGpio(void);

extern void     GPIOMap_SetTPPin(TP_INIT_STRUCT* pInit);
extern void     GPIOMap_SetLensGpio(LENS_DEVICE_OBJ* pLensDevice);

extern BOOL     GPIOMap_IsFloodLightOn(void);
extern void     GPIOMap_TurnOnFloodLight(void);
extern void     GPIOMap_TurnOffFloodLight(void);

#endif

//@}
