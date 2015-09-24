/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       VoltageDet.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of voltage detection module
                This file is the header file of voltage detection module

    @note       Nothing.

    @date       2005/08/24
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _VOLTAGEDET_H
#define _VOLTAGEDET_H

#include "SysCfg.h"

#define VOLDET_BATTERY_LVL_CHARGE   5
#define VOLDET_BATTERY_LVL_EMPTY    4
#define VOLDET_BATTERY_LVL_0        3
#define VOLDET_BATTERY_LVL_1        2
#define VOLDET_BATTERY_LVL_2        1
#define VOLDET_BATTERY_LVL_3        0

#define VOLDET_FLASH_LVL_EMPTY      0
#define VOLDET_FLASH_LVL_HALF       1
#define VOLDET_FLASH_LVL_FULL       2

//#NT#2009/03/25#Hideo Lin -begin
//#NT#Detect battery level in different conditions
#define VOLDET_CONDITION_POWER_ON           0
#define VOLDET_CONDITION_BEFORE_CHARGE      1
#define VOLDET_CONDITION_BEFORE_CHARGE_QV   2
#define VOLDET_CONDITION_BEFORE_CAPTURE     3
#define VOLDET_CONDITION_BEFORE_CAPTURE_QV  4
//#NT#2009/03/25#Hideo Lin -end

extern void     VolDet_Init(void);

extern UINT32   VolDet_GetBatteryADC(void);
extern UINT32   VolDet_GetFlashADC(void);

extern UINT32   VolDet_GetBatteryLevel(void);
extern UINT32   VolDet_GetFlashLevel(void);
//#NT#2009/03/25#Hideo Lin -begin
//#NT#Detect battery low in different conditions
//extern BOOL     VolDet_CheckBatteryLow(UINT32 condition);
extern BOOL     VolDet_CheckBatteryLow(void);
//#NT#2009/03/25#Hideo Lin -end
#endif

//@}
