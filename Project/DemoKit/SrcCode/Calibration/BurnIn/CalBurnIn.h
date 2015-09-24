/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalBurnIn.h
    @ingroup    mIPRJAPTest

    @brief      Header file for BurnIn process
                This file is the header file for BurnIn process

    @note       Nothing.

    @date       2005/08/24
*/

/** \addtogroup mIPRJAPTest */
//@{

#ifndef _BURNIN_H
#define _BURNIN_H

#include "Type.h"
#include "SysCfg.h"

extern void     Cal_BurnInMenu(void);
extern void     Cal_BurnInBurnIn(void);
extern void     Cal_BurnInZoomAF(void);
extern void     Cal_BurnInFlash(void);
extern void     Cal_BurnIn168H(void);
extern void     Cal_BurnIn10min(void);

extern UINT32   Cal_BurnInGetIteration(void);

#endif

//@}
