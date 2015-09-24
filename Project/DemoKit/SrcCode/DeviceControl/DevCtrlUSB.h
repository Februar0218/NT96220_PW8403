/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlUSB.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of DevCtrlUSB.c
                Header file of DevCtrlUSB.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _DEVCTRLUSB_H
#define _DEVCTRLUSB_H

#include "Type.h"

extern void     DevCtrl_ModeUSBMSDC(void);
extern void     DevCtrl_ModeUSBSICD(void);
extern void     DevCtrl_ModeUSBPCC(void);

extern void     DevCtrl_ModeUSBRemove(void);

#endif

//@}
