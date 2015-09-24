/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SensorFP.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of SensorFP.c
                Header file of SensorFP.c

    @note       Nothing.

    @date       2007/01/23
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _SENSORFP_H
#define _SENSORFP_H

#include "Type.h"

extern void     SenFP_ChargeFlash(BOOL bCharge, BOOL bHighCurrent);
extern BOOL     SenFP_IsFlashReady(void);
extern UINT32   SenFP_GetFlashADCValue(void);
extern void     SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor);
extern void     SenFP_ClearOSD(void);
extern void     SenFP_PowerOff(BOOL bLensRet, BOOL bWriteNand);

#endif

//@}
