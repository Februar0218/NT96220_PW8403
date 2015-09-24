/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensAPI.c
    @ingroup    mISYSLens

    @brief      Lens API functions header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSAPI_H
#define _LENSAPI_H

#include "Lens.h"


typedef struct _LENS_MODULE_OBJ
{
    UINT32              uiLensModuleState;      ///< The current state of lens module
    PMOTOR_TAB          pMotorObj;              ///< The function table of motor driver
    PLENS_TAB           pLensObj;               ///< The function table of lens driver
} LENS_MODULE_OBJ, *PLENS_MODULE_OBJ;

#endif

