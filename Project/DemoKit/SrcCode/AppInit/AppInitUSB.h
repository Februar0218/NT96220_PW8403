/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitUSB.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of AppInitUSB.c
                Header file of AppInitUSB.c

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _APPINITUSB_H
#define _APPINITUSB_H

#include "Type.h"
#include "UMSDC.h"
#include "Usicd.h"
#include "USBPcc.h"

extern void     AppInit_ModeUSBMSDC(void);
extern void     AppInit_ModeUSBSICD(void);
extern void     AppInit_ModeUSBPCC(void);
//#NT#2009/07/13#Chris Chung -begin
//#NT#add to control PCC vendor function
extern void     AppInit_ModeUSBPCCEnableVendorFunc(BOOL en);
extern BOOL  AppInit_ModeUSBPCCGetVendorFunc(void);
//#NT#2009/07/13#Chris Chung -end

#endif

//@}
