/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanInt.h
    @ingroup    mIPRJAPKeyIO

    @brief      Internal header file of KeyScan task
                Internal function/defintions...

    @note       Nothing.

    @date       2005/11/18
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _KEYSCANINT_H
#define _KEYSCANINT_H

#include "SysCfg.h"

//-----------------------------------------------------------------------------
// KeyScan internal definitions
//-----------------------------------------------------------------------------
// Use interrupt to detect SD card insert/remove or not
#define KEYSCAN_CARDDET_INT             DISABLE

// Use for demo board that don't have these pins
#define KEYSCAN_POWEROFF_DET            ENABLE
#if (_IQ_SIM_ == ENABLE)
#define KEYSCAN_AUTOPOWEROFF_DET        DISABLE
#else
#define KEYSCAN_AUTOPOWEROFF_DET        ENABLE
#endif
#define KEYSCAN_BATTERY_DET             ENABLE
#define KEYSCAN_KEY_DET                 ENABLE

#if (UI_STYLE == UI_STYLE_DRIVE)
#define KeyScan_AC_DET                  ENABLE
#define KeyScan_POWEROF_ACUNPLUG        ENABLE
#else
#define KeyScan_AC_DET                  DISABLE
#define KeyScan_POWEROF_ACUNPLUG        DISABLE
#endif

// If DISABLE, keyscan task will read SD controller to get the status.
// If ENABLE, keyscan task will read GPIO pin to get the status.
#define KEYSCAN_SDWRPTDET_GPIO          ENABLE

#define KEYSCAN_CARD_REMOVED            0
#define KEYSCAN_CARD_INSERTED           1
#define KEYSCAN_CARD_UNKNOWN            0xFFFFFFFF

// SD card hot plug/unplug f/w function support
#if (_PROJECT_TUOBU_ == ENABLE)
#define KEYSCAN_CARD_HOTPLUG_FUNC       ENABLE
#else
#define KEYSCAN_CARD_HOTPLUG_FUNC       DISABLE
#endif

//add for TDC
#define KEYSCAN_MACRO_OFF               0
#define KEYSCAN_MACRO_ON                1
#define KEYSCAN_MACRO_UNKNOWN           0xFFFFFFFF

#define KEYSCAN_PWROFF_INIT_STATE       0
#define KEYSCAN_PWROFF_RELEASE_STATE    1
#define KEYSCAN_PWROFF_PRESS_STATE      2

#define KEYSCAN_PWRKEY_UNKNOWN          0xFFFFFFFF
#define KEYSCAN_PWRKEY_RELEASED         0
#define KEYSCAN_PWRKEY_PRESSED          1

//#NT#2007/03/21#Chris Hsu -begin
//#For new GPIO interrupt definitions
// Define card detect interrupt
#define KEYSCAN_CD_GPIO_INT             GPIO_INT_00
//#NT#2007/03/21#Chris Hsu -end

//-----------------------------------------------------------------------------
// KeyScan internal variables
//-----------------------------------------------------------------------------
extern volatile UINT32  g_uiKeyScanTimerID;
extern volatile UINT32  g_uiKeyScanAutoPoweroffCnt;
extern volatile BOOL    g_bKeyScanAutoPoweroffEn;
extern volatile BOOL    g_bKeyScanKeyDetEn;
extern volatile BOOL    g_bKeyScanMDDetEn;
extern          UINT32  g_uiKeyScanFlags;
extern          BOOL    g_bFilesysAbortCmd;
extern volatile UINT32  g_uiKeyScanAutoLcdoffCnt;

//-----------------------------------------------------------------------------
// KeyScan internal APIs
//-----------------------------------------------------------------------------
// Flash light
extern void     KeyScan_DetFlash(void);

// Key
extern BOOL     KeyScan_CheckFileExit(char* filename);
extern BOOL     KeyScan_CheckShowFwVersion(void);
extern void     KeyScan_DetKey(void);
extern void     KeyScan_DetModedial(BOOL bForceChangeMode);
extern BOOL     KeyScan_CheckEngMode(void);
extern BOOL     KeyScan_CheckEngModeTextFile(void);
extern BOOL KeyScan_CheckUpdateCfgTextFile(void);
extern BOOL KeyScan_CheckUpdateLogoTextFile(void);
extern BOOL KeyScan_CheckUpdateVersionTextFile(void);
extern void SetGsensorPowerOn(BOOL gPwron);
extern BOOL GetGsensorPowerOn(void);
// LED
extern void     KeyScan_ToggleLED(void);
extern void     KeyScan_DetBusy(void);

// Power
extern void     KeyScan_DetACPower(void);
extern void     KeyScan_DetPoweroff(void);
extern void     KeyScan_DetBattery(void);
extern void     KeyScan_DetBatteryInSysInit(void);
extern void     KeyScan_DetAutoPoweroff(void);
extern void     KeyScan_PoweroffFlow(void);

// Storage card
extern void     KeyScan_DetStrgCard(void);
extern void     KeyScan_StorageCB(UINT32 uiStatus);

// Sound
extern void     KeyScan_DetAudio(void);

// TV
extern void     KeyScan_DetTV(void);

// HDMI
extern void     KeyScan_DetHDMI(void);

// USB
extern void     KeyScan_DetUSB(void);
extern BOOL     KeyScan_CheckUSBInSysInit(void);

// LCD rotate
extern void     KeyScan_DetLCDRotate(void);

//Macro
extern void     KeyScan_DetMacro(void);

// IR Remote
extern void     KeyScan_DetIrRxKey(void);
extern void     KeyScan_CheckIrRxExp(void);

extern void     KeyScan_DetAutoLCDoff(void);

extern void KeyScan_DetGSensor(void);

extern UINT32   g_uiKeyScanDelayShutdownCnt;
extern BOOL     g_bKeyScanDelayShutdownEn;

extern void 	KeyScan_SetDelayShutdownTime(UINT32 uiTime);
extern UINT32 	KeyScan_GetDelayShutdownTime(void);
extern void 	KeyScan_DetDelayShusdown(void);
extern void 	KeyScan_EnableDelayShutdown(BOOL bEn);

#endif

//@}
