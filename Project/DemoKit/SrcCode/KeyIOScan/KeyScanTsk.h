/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanTsk.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of scan key, I/O task
                Header file of scan key, I/O task

    @note       Nothing.

    @date       2005/11/18
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _KEYSCANTSK_H
#define _KEYSCANTSK_H

#include "SysCfg.h"
#include "GPIOMapping.h"

// Flag pattern
// Key pressed
#define FLGKEY_UP                       0x00000001              // Key UP pressed
#define FLGKEY_DOWN                     0x00000002              // Key DOWN pressed
#define FLGKEY_LEFT                     0x00000004              // Key LEFT pressed
#define FLGKEY_RIGHT                    0x00000008              // Key RIGHT pressed
#define FLGKEY_ENTER                    0x00000010              // Key ENTER (SET) pressed

#define FLGKEY_MENU                     0x00000020              // Key MENU pressed
#define FLGKEY_MODE                     0x00000040              // Key MODE
#define FLGKEY_DISPLAY                  0x00000080              // Key DISPLAY pressed

#define FLGKEY_ZOOMOUT                  0x00000100              // Key ZOOM OUT pressed
#define FLGKEY_ZOOMIN                   0x00000200              // Key ZOOM IN pressed

#define FLGKEY_SHUTTER1                 0x00000400              // Key SHUTTER 1 (SHUTTER_HALF) pressed
#define FLGKEY_SHUTTER2                 0x00000800              // Key SHUTTER 2 (SHUTTER_FULL) pressed

#define FLGKEY_DEL                      0x00001000              // Key DELETE (TRASH) pressed
#define FLGKEY_PLAYBACK                 0x00002000              // Key PLAYBACK pressed
#define FLGKEY_NETWORK                  0x00004000              // Key NETWORK pressed
#define FLGKEY_MOVIE                    0x00008000              // Key MOVIE pressed

#define FLGKEY_KEY_MASK                 0x0000FFFF              // Key flag mask

#define FLGKEY_KEY_MASK_DEFAULT         FLGKEY_KEY_MASK         // Default key mask
#define FLGKEY_KEY_MASK_NULL            0x00000000              // Key mask off all
#define FLGKEY_KEY_REL_MASK_DEFAULT     0x00000000              // Default key released mask
#define FLGKEY_KEY_CONT_MASK_DEFAULT    (FLGKEY_UP|FLGKEY_DOWN|FLGKEY_RIGHT|FLGKEY_LEFT)   // Default key continue mask

// Key released, long pressed, continue
#define FLGKEY_PRESSED                  0x00000000
#define FLGKEY_RELEASED                 0x00010000
#define FLGKEY_CONTINUE                 0x00020000
#define FLGKEY_HOLD                     0x00020000
#define FLGKEY_LONGPRESSED              0x00040000
#define FLGKEY_LONGHOLD                 0x00040000

#define FLGKEY_ACT_MASK                 0x000F0000              // Key action mask

// Storage
#define FLGKEY_STG_CHANGE               0x00100000              // Storage changed
#define FLGKEY_STG_INITFAIL             0x00200000              // File system init fail
#define FLGKEY_STG_DISKERR              0x00400000              // Disk error

#define FLGKEY_KEEP                     0x01000000
#define FLGKEY_CUSTOM1                  0x02000000
#define FLGKEY_CUSTOM2                  0x04000000
#define FLGKEY_CUSTOM3                  0x08000000
#define FLGKEY_TASK_CTRL                0x08000000              // Pseudo key for task control (test)

#define FLGKEY_TIMER01S                 0x10000000              // Pseudo 0.1 second timer
#define FLGKEY_TIMER05S                 0x20000000              // Pseudo 0.5 second timer
#define FLGKEY_TIMER1S                  0x40000000              // Pseudo 1 second timer

#define FLGKEY_REFRESHOSD               0x80000000              // Pseudo KEY Refresh OSD display

#define FLGKEY_ALL                      0xFFFFFFFF

// LED
// Please sync to GPIOMapping.h
#define KEYSCAN_LED_GREEN               0                       // Green LED
#define KEYSCAN_LED_RED                 1                       // Red LED
#define KEYSCAN_LED_FCS                 2                       // FCS LED

// LED toggle speed
#define KEYSCAN_LED_TSPEED_SLOW         0                       // Slow
#define KEYSCAN_LED_TSPEED_NORMAL       1                       // Normal
#define KEYSCAN_LED_TSPEED_FAST         2                       // Fast

// Power
#define KEYSCAN_AUTOPOWEROFF_DISABLED   0

// Sound
#define KEYSCAN_SOUND_STARTUP           0
#define KEYSCAN_SOUND_SHUTDOWN          1
#define KEYSCAN_SOUND_KEY_SHUTTER2      2
#define KEYSCAN_SOUND_KEY_OTHERS        3

// Key mode for turn on/off key mask
#define KEYSCAN_KEYMODE_PRESS           0
#define KEYSCAN_KEYMODE_RELEASE         1
#define KEYSCAN_KEYMODE_CONTINUE        2
#define KEYSCAN_KEYMODE_LONG            3
#define KEYSCAN_KEYMODE_CHGMOD_PRESS    4

#define KEYSCAN_SYSTIMER_AMOUNT         5

#define TIMER_NULL                      0xFFFFFFFF
#define NULL_TIMER                      TIMER_NULL // for compatible issue

typedef enum
{
    TIMER_TYPE_ONE_SHOT = 0x00,
    TIMER_TYPE_CONTINUE
}TIMER_TYPE_ENUM;

typedef struct
{
    //BOOL              isBusy;
    UINT32              interval;
    UINT32              eventID;
    UINT32              counter;
    TIMER_TYPE_ENUM     type;
    BOOL                isEnabled;
}TIMER_INFO;

// APIs
extern void     KeyScan_Suspend(void);
extern void     KeyScan_Resume(void);

// Flash light
extern void     KeyScan_EnableFlashCharging(BOOL bEn);
extern BOOL     KeyScan_IsFlashCharging(void);

// Key
extern void     KeyScan_EnableKeyDet(BOOL bEn);
extern void     KeyScan_EnableMDDet(BOOL bEn);
extern void     KeyScan_SetKeyMask(UINT32 uiMode, UINT32 uiMask);
extern UINT32   KeyScan_GetKeyMask(UINT32 uiMode);

// LCD
extern BOOL     KeyScan_IsLCDRotate(void);

// LED
extern void     KeyScan_TurnOnLED(UINT32 uiLED);
extern void     KeyScan_TurnOffLED(UINT32 uiLED);
extern BOOL     KeyScan_IsLEDOn(UINT32 uiLED);
extern void     KeyScan_SetLEDToggleSpeed(UINT32 uiLED, UINT32 uiSpeed);
extern void     KeyScan_EnableLEDToggle(UINT32 uiLED, BOOL bEn);

// Power
extern void     KeyScan_Poweroff(void);
extern void     KeyScan_EnableAutoPoweroff(BOOL bEn);
extern void     KeyScan_SetAutoPoweroffTime(UINT32 uiTime);
extern UINT32   KeyScan_GetAutoPoweroffTime(void);
extern void     KeyScan_EnableUSBDet(BOOL bEn);

// SD
extern BOOL     KeyScan_IsStrgCardLocked(void);

// Sound
extern void     KeyScan_EnableKeyTone(BOOL bEn);
extern BOOL     KeyScan_IsKeyToneEnabled(void);
extern void     KeyScan_PlaySound(UINT32 uiSound);
extern void     KeyScan_PlayTone(UINT8 bType);
extern BOOL     KeyScan_IsHPPlugIn(void);

// TV
extern BOOL     KeyScan_IsTVPlugIn(void);
extern void     KeyScan_ChangeTVMode(void);
extern UINT32   KeyScan_GetCurrentTVMode(void);

// HDMI
extern BOOL     KeyScan_IsHDMIPlugIn(void);

// USB

// Timer
extern UINT32   KeyScan_startTimer(UINT32 interval, UINT32 eventID, TIMER_TYPE_ENUM type);
extern void     KeyScan_stopTimer(UINT32 * timerID);
extern UINT32   Get_NVTCMD(UINT32 keys);
extern UINT32   Get_KeyEvt(UINT32 evt);
extern void     KeyScan_SetPressKeyEventMask(UINT32 * evtTable);
extern BOOL     KeyScan_IsUSBPlug(void);
extern BOOL     KeyScan_IsUSBPower(void);

extern void     KeyScan_SetDetTP(BOOL IsEnable);

extern UINT32   KeyScan_GetAutoLCDoffTime(void);

extern void     KeyScan_MovieRecorddingCheck(void);

#endif

//@}
