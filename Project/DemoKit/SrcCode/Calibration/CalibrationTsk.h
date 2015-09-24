/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.h
    @ingroup    mIPRJAPTest

    @brief      Header file of calibration task
                Header file of calibration task

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALIBRATIONTSK_H
#define _CALIBRATIONTSK_H

#include "Type.h"
#include "UIResource.h"

extern ER       Cal_Open(void);
extern void     Cal_ShowFocusResult(UINT32 *pFocusPoint);
extern BOOL     Cal_GetFocusParam(INT8 * pFileName);
extern void     Cal_ShowStringWithColor(char *pStr, UINT16 X, UINT16 Y,UINT32 color);
extern void     Cal_ShowRectStringWithColor(INT8 *pStr, PRECT pRect, BOOL bClear,UINT32 color);
extern void     Cal_ShowStringByColor(INT8 *pStr, PRECT pRect, UINT32 uiFrColor, UINT32 uiBgColor);
extern void     Cal_FillRect(PRECT pRect, UINT32 uiBgColor);
extern void     Cal_ShowString(INT8 *pStr, PRECT pRect, BOOL bClear);
extern UINT32   Cal_GetSerialNum(UINT32 uiIncrement);
extern void     Cal_ClearOSD(UINT32 uiColorIdx);
//extern UINT32   Cal_GetMemoryAddr(void);
//extern UINT32   Cal_GetMemorySize(void);

extern UINT32   Cal_GetMaxItemNum(void);

#endif

//@}
