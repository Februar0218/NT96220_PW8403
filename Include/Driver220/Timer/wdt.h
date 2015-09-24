/**
    Public header file for WDT module.

    This file is the header file that define the API and data type for WDT
    module.

    @file       wdt.h
    @ingroup    mIHALWDT
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/


#ifndef _WDT_H
#define _WDT_H

#include "Type.h"

/**
    @addtogroup mIHALWDT
*/
//@{

// WDT Driver APIs
extern ER       wdt_open(void);
extern ER       wdt_close(void);
extern ER       wdt_waitTimeout(void);
extern ER       wdt_setTrig(void);
extern ER       wdt_setEnable(void);
extern ER       wdt_setDisable(void);
extern ER       wdt_getCnt(UINT32 *uiCnt);

// WDT Driver NT96220 New APIs
extern ER       wdt_setTimeout(BOOL bReset, UINT32 uiIntval);
extern ER       wdt_getEnSTS(BOOL *bEn);
extern void     wdt_getResetNums(UINT32 *pUiResetNum);
extern void     wdt_setData(UINT32 data);
extern UINT32   wdt_getData(void);

// WDT Driver Obsoleted APIs
#define         wdt_setIntEn(bEn)           debug_err(("NT96220 WDT Obsoleted API\r\n"))
#define         wdt_setNahbRstEn(bEn)       debug_err(("NT96220 WDT Obsoleted API\r\n"))
#define         wdt_setMS4B(val)            debug_err(("Please call wdt_setTimeout()\r\n"))
#define         wdt_setMS4B2(val)           debug_err(("Please call wdt_setTimeout()\r\n"))
#define         wdt_setRstEn(bEn)           debug_err(("Please call wdt_setTimeout()\r\n"))

//@}

#endif
