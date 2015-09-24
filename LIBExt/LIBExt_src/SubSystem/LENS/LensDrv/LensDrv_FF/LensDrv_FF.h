/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_FF.h
    @ingroup    mISYSLens

    @brief      FF lens driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSDRV_FF_H
#define _LENSDRV_FF_H
//#include "constant.h"
#include "Type.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Focus related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct FOCUSPARA
{
    volatile UINT8          uiFocusPhase;
    volatile INT16          iFocusPosition;
} FOCUS_Struct;

typedef enum _LENS_FOCUS_PI_LEVEL {
    FOCUSPI_OUTSDIE_LEVEL = 0,
    FOCUSPI_INSDIE_LEVEL
} LENS_FOCUS_PI_LEVEL;

#define FOCUS_POS_HOME_DEF       0
#define FOCUS_POS_INNER_STOP     (FOCUS_POS_HOME_DEF-104)
#define FOCUS_POS_GARAGE         (FOCUS_POS_HOME_DEF-60-10)//-10 to avoid BackLash hit the wall
#define FOCUS_POS_FAR            (FOCUS_POS_HOME_DEF-60)
#define FOCUS_POS_HOME_PR        (FOCUS_POS_HOME_DEF-0)
#define FOCUS_POS_NORMAL_WIDE    (FOCUS_POS_HOME_DEF+97)
#define FOCUS_POS_NEAR           (FOCUS_POS_HOME_DEF+440)
#define FOCUS_POS_OUTER_STOP     (FOCUS_POS_HOME_DEF+465)

#define FOCUS_BWD_LIMIT_OUTSIDE_PI  ((FOCUS_POS_OUTER_STOP-FOCUS_POS_HOME_PR)+20)   //limitation for  FPI search.
#define FOCUS_FWD_LIMIT_INSIDE_PI   ((FOCUS_POS_HOME_PR-FOCUS_POS_INNER_STOP)+20)   //limitation for  FPI search.
#define FOCUS_BL_STEP               6
#define FOCUS_SPEED                 1000   //unit: us
#define FOCUS_INIT_DELAY            10     //unit: ms

#define FOCUS_TOTAL_PHASE           8

#define FOCUS_ZOOM_TABLE_COLUMN     11

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for IRIS related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct IRISPARA
{
    volatile UINT8          uiCurrentState; //Record current LENS_IRIS_STATE in IRIS state machine.
} IRIS_Struct;

typedef enum _LENS_IRIS_POSITION {
    IRIS_POS_BIG = 0,
    IRIS_POS_SMALL,
    IRIS_POS_TOTAL_NUM      //Total Positon Number
} LENS_IRIS_POSITION;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Shutter related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct SHUTTERPARA //revise by 2006/05/08
{
    volatile UINT8          uiCurrentState; //Record current LENS_SHUTTER_STATE in SHUTTER state machine.
} SHUTTER_Struct;

#define SHUTTER_PULSE_PERIOD    20  //unit: ms

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for other definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef enum _LENS_DIRECTION {
    DIRECTION_FWD = 0,
    DIRECTION_BWD
} LENS_DIRECTION;

#define STEPPER_PHASE_M4(a,b,c,d)   ((a)|((b)<<1)|((c)<<2) | ((d)<<3))
#define STEPPER_PHASE_M2(ap,am)     ((ap)|((am)<<1))

#endif
