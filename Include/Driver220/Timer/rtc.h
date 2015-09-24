/**
    Header file for RTC module driver.

    This file is the driver header of RTC module

    @file       rtc.h
    @ingroup    mIHALRTC
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/


#ifndef _RTC_H
#define _RTC_H

#include "Type.h"

/**
    @addtogroup mIHALRTC
*/
//@{

/**
    RTC Date
*/
typedef union
{
    // Format conformed to RTC register
    __packed struct
    {
        UINT    day:5;      ///< Day
        UINT    month:4;    ///< Month
        UINT    year:12;    ///< Year
    } s;
    UINT32      value;      ///< Date value all together Y:M:D
} RTC_DATE;

/**
    RTC Time
*/
typedef union
{
    // Format conformed to RTC register
    __packed struct
    {
        UINT    second:6;   ///< Second
        UINT    minute:6;   ///< Minute
        UINT    hour:5;     ///< Hour
    } s;
    UINT32      value;      ///< Time value all together H:M:S
} RTC_TIME;

/**
    RTC power on source

    @note For rtc_getPwrOnSource()
*/
typedef enum
{
    RTC_PWRON_SRC_PWR_SW,       ///< Power on from power switch
    RTC_PWRON_SRC_PWR_SW2,      ///< Power on from power switch 2 (NT96220 doesn't supported)
    RTC_PWRON_SRC_VBAT,         ///< Power on from VBAT
    RTC_PWRON_SRC_VBUS,         ///< Power on from USB VBUS
    RTC_PWRON_SRC_PWR_ALM,      ///< Power on from PWR ALARM
    RTC_PWRON_SRC_POR,          ///< Power on from POR
    ENUM_DUMMY4WORD(RTC_PWRON_SRC)
} RTC_PWRON_SRC;

/**
    RTC power fail level

    @note for rtc_setPwrFailVoltage()
*/
typedef enum
{
    RTC_PWRFAIL_LEVEL_0,        ///< RTC report power lost at about 1.1V (default power fail level)
    RTC_PWRFAIL_LEVEL_1,        ///< RTC report power lost at about 0.7V (Clock off detection)
    ENUM_DUMMY4WORD(RTC_PWRFAIL_LEVEL)
} RTC_PWRFAIL_LEVEL;

/**
    @name RTC status definition

    @note for rtc_enableInt(), rtc_disableInt(), rtc_getIntEnabled(), rtc_getIntStatus() and rtc_clearInt()
*/
//@{
#define RTC_ALARM           0x00000001      ///< RTC alarm
#define RTC_CSET_DONE       0x00000100      ///< RTC CSET done
//@}

#define RTC_KEY             0xA             ///< RTC KEY. POR will be set to 0x1
#define RTC_KEY_MAX         0xF             ///< RTC max KEY value

// external function declaration

// RTC driver APIs
extern ER               rtc_open(void);
extern ER               rtc_close(void);
extern void             rtc_enableInt(UINT32 ens);
extern void             rtc_disableInt(UINT32 dis);
extern UINT32           rtc_getIntEnabled(void);
extern UINT32           rtc_getIntStatus(void);
extern void             rtc_clearInt(UINT32 clrSts);
extern void             rtc_reset(void);
extern BOOL             rtc_isRegReady(void);
extern void             rtc_waitCSETDone(void);
extern void             rtc_triggerCSET(void);
extern BOOL             rtc_isPowerLost(void);
extern void             rtc_setPwrFailVoltage(RTC_PWRFAIL_LEVEL voltageLevel);

// RTC key control APIs
extern UINT32           rtc_getKey(void);
extern void             rtc_setKey(UINT32 key);

// RTC time control APIs
extern void             rtc_setBaseDate(UINT32 Year, UINT32 Month, UINT32 Day);
extern RTC_DATE         rtc_getBaseDate(void);
extern void             rtc_setMaxDate(UINT32 Year, UINT32 Month, UINT32 Day);
extern RTC_DATE         rtc_getMaxDate(void);
extern UINT32           rtc_setDate(UINT32 Year, UINT32 Month, UINT32 Day);
extern RTC_DATE         rtc_getDate(void);
extern UINT32           rtc_setTime(UINT32 Hour, UINT32 Minute, UINT32 Second);
extern RTC_TIME         rtc_getTime(void);
extern UINT32           rtc_convertDate2Days(RTC_DATE date);
extern RTC_DATE         rtc_convertDays2Date(UINT32 days);
extern BOOL             rtc_isLeapYear(UINT32 year);

// RTC alarm control APIs
extern UINT32           rtc_setAlarm(UINT32 Hour, UINT32 Minute, UINT32 Second);
extern RTC_TIME         rtc_getAlarm(void);
extern BOOL             rtc_isAlarmTime(void);

// RTC power alarm APIs
extern UINT32           rtc_setPWRAlarm(UINT32 uiHour, UINT32 uiMinute); // for backward compatible
extern UINT32           rtc_setPWRAlarmDateTime(RTC_DATE rtcDate, RTC_TIME rtcTime);
extern RTC_TIME         rtc_getPWRAlarm(void);
extern RTC_DATE         rtc_getPWRAlarmDate(void);

extern void             rtc_disablePWRAlarm(void);
extern void             rtc_enablePWRAlarm(void);
extern BOOL             rtc_isPWRAlarmEnabled(void);
extern BOOL             rtc_isPWRAlarm(void);

extern void             rtc_coldReboot(UINT32 uiDelay);

// RTC power button control APIs
extern void             rtc_resetShutdownTimer(void);
extern void             rtc_poweroffPWR(void);
extern BOOL             rtc_getPWRStatus(void);
extern RTC_PWRON_SRC    rtc_getPwrOnSource(void);

//@}

// RTC obsoleted APIs
#define rtc_getPWR2Status()                 debug_err(("NT96220 RTC doesn't support PWR_SW2\r\n"))
#define rtc_setPwrOffTmrTarget(uiTarget)    debug_err(("NT96220 RTC doesn't support PWR OFF timer\r\n"))
#define rtc_getPwrOffCntr()                 debug_err(("NT96220 RTC doesn't support PWR OFF timer\r\n"))
#define rtc_setPwrOffTmrEn(bEn)             debug_err(("NT96220 RTC doesn't support PWR OFF timer\r\n"))
#define rtc_getPwrOffTmrEn()                debug_err(("NT96220 RTC doesn't support PWR OFF timer\r\n"))
#define rtc_setData0(data0)                 debug_err(("NT96220 RTC doesn't support user data\r\n"))
#define rtc_setData1(data1)                 debug_err(("NT96220 RTC doesn't support user data\r\n"))
#define rtc_getData0()                      debug_err(("NT96220 RTC doesn't support user data\r\n"))
#define rtc_getData1()                      debug_err(("NT96220 RTC doesn't support user data\r\n"))
#define rtc_enable()                        debug_err(("NT96220 RTC doesn't support rtc_enable()\r\n"));
#define rtc_disable()                       debug_err(("NT96220 RTC doesn't support rtc_disable()\r\n"));
#define rtc_clearPWRAlarmStatus()           debug_err(("NT96220 RTC doesn't support rtc_clearPWRAlarmStatus()\r\n"));

#endif
