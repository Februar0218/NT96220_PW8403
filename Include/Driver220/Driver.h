/**
    Driver common part header file

    @file       Driver.h
    @ingroup    mIDriver
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _DRIVER_H
#define _DRIVER_H

/**
    @addtogroup mIDriver
*/
//@{

#include "Type.h"

extern void     drv_init(void);
extern void     drv_dumpInfo(void);
extern CHAR    *drv_getVerInfo(void);
extern CHAR    *drv_getBuildDate(void);
extern CHAR    *drv_getVerInfo_IPP(void);
extern CHAR    *drv_getBuildDate_IPP(void);
extern CHAR    *drv_getVerInfo_Codec(void);
extern CHAR    *drv_getBuildDate_Codec(void);

//@}

#endif
