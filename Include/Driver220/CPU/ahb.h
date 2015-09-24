/**
    AHB module driver.

    This file is the driver of AHB module.

    @file       ahb.h
    @ingroup    mIHALAHB
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _AHB_H
#define _AHB_H

#include "Type.h"

/**
    @addtogroup mIHALAHB
*/
//@{

extern void     ahb_enablePrefetch(BOOL bEn);
extern void     ahb_clearPrefetch(void);

//@}

#endif
