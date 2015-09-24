/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensCmd.c
    @ingroup    mISYSLens

    @brief      Lens Test command functions header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSCMD_H
#define _LENSCMD_H

#include "Type.h"


/**
    Command lens driver model set
*/
typedef struct _LENS_CMD_LENSMODEL
{
    UINT32      uiLensModelID;
    CHAR        *pcLensModelStr;
} LENS_CMD_LENSMODEL, *PLENS_CMD_LENSMODEL;

/**
    Command motor driver model set
*/
typedef struct _LENS_CMD_MOTORMODEL
{
    UINT32      uiMotorModelID;
    CHAR        *pcMotorModelStr;
} LENS_CMD_MOTORMODEL, *PLENS_CMD_MOTORMODEL;






#endif

