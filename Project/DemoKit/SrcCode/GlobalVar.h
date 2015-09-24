/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GlobalVar.h
    @ingroup    mIPRJAPCfg

    @brief      Gloabl variables
                Gloabl variables

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _GLOBALVAR_H
#define _GLOBALVAR_H

#include "Type.h"
#include "LCDTV.h"

//#include "LIS33DE.h"
#include "DMARD07.h"

#include "CK235.H"

// LCD display size
extern LCDSIZE          g_LCDSize;

// LCD control object
extern LCD_DEVICE_OBJ   g_LCDCtrlObj;

extern ENDE_DEVICE_OBJ g_ENDECtrlObj;

extern GSENSOR_DEVICE_OBJ GSensor_DeviceObj;

#endif

//@}
