/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       Lens_int.c
    @ingroup    mISYSLens

    @brief      Internal header file of Lens control functions

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/10/14
*/

#ifndef _LEN_INT_H
#define _LEN_INT_H

#include "Debug.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "Utility.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for overall Lens Module related definition.
//--------------------------------------------------------------------------------------------------------------------------

#define LENS_NULL_TIMER         0xFFFFFFFF

#define LENS_MAX_WAIT_TIME      30 //unit: ms

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for debug message.
//--------------------------------------------------------------------------------------------------------------------------

#define LENS_DEBUG_MSG   0
#define LENS_DEBUG_ERR   1

#if (LENS_DEBUG_MSG&DEBUG_LEVEL)
#define LENS_MSG(msg)    debug_msg msg
#else
#define LENS_MSG(msg)
#endif

#if (LENS_DEBUG_ERR&DEBUG_LEVEL)
#define LENS_ERR(msg)    debug_msg msg
#else
#define LENS_ERR(msg)
#endif

#endif

