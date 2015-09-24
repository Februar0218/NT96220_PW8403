/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       LensDrv_DZ5311.h
    @ingroup    mISYSLens

    @brief      A-optronics DZ5311 lens driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/09/30
*/

#ifndef _LENSDRV_DZ5311_H
#define _LENSDRV_DZ5311_H
//#include "constant.h"
#include "Type.h"
//#define LENS_NO_ISR
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Zoom related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct ZOOMPARA
{
    /* Used in all states */
    volatile UINT8          uiCurrentState; //Record current state in zoom state machine.
    volatile UINT8          uiZoomWideCnt;  //Count how many times the lens has zoom to wide.
    volatile UINT8          Direction;      //Record current zoom direction.
    volatile UINT8          ZoomOverRun;    //Limit zoom over-run step.
    volatile INT16          ZoomStep;       //Record current zoom step position.
    volatile INT16          ZoomLastStep;   //Record last zoom step position.
    volatile BOOL           bZoomIsr;       //Open Zoom ISR.
    volatile BOOL           bZoomBoundaryAutoCheck;   //Auto check zoom bundary.
    volatile BOOL           bDoOptZoomDraw;           //OpticalZoom Callback to Draw.
    volatile UINT8          uiLastSection;  //Record last zoom section.
    volatile UINT16         StepMove;       //Limit lens zoom steps in running state.
    volatile UINT16         StepCnt;        //Record how many steps the lens has zoom in running state.
    volatile INT16          ZoomTargetStep; //Record target zoom step position.
} ZOOM_Struct;

typedef enum _LENS_ZOOM_STATE {
    ZOOM_STATE_NULL = 0,        //unknow state
    ZOOM_STATE_FAIL,            //fail state
    ZOOM_STATE_INSIDEINIT,      //init from inside state
    ZOOM_STATE_OUTSIDEINIT,     //init from outside state
    ZOOM_STATE_INITPART1,       //InsideInit ok and enter part1 state
    ZOOM_STATE_INITPART2,       //InitPart1 ok and enter part2 state
    ZOOM_STATE_IDLE,            //idle state
    ZOOM_STATE_RUNNING,         //running state
    ZOOM_STATE_CLOSING,         //closing state
    ZOOM_STATE_CLOSED           //closed state
} LENS_ZOOM_STATE;

typedef enum _LENS_ZOOM_PR_LEVEL {
    ZOOMPR_INSDIE_LEVEL = 0,
    ZOOMPR_OUTSDIE_LEVEL
} LENS_ZOOM_PR_LEVEL;

#define ZOOM_POS_INNER_STOP     0
#define ZOOM_POS_GARAGE         12
#define ZOOM_POS_HOME_PR        20
#define ZOOM_POS_WIDE           (ZOOM_POS_HOME_PR+0)
#define ZOOM_POS_TELE           (ZOOM_POS_HOME_PR+800) // 2011-04-26 Chris
#define ZOOM_POS_OUTER_STOP     (ZOOM_POS_HOME_PR+900)

#define ZOOM_FWD_LIMIT_INSIDE_PI   40 //(a-optronics nick: 40)

#define ZOOM_SECTION_TELE       25
#define ZOOM_SECTION_WIDE       1
#define ZOOM_BL_STEP            16  // 2011-05-08 Chris

#define ZOOM_MIN_SPEED          40  //pps
#define ZOOM_MAX_SPEED          300 //pps
#define ZOOM_SPEED_PER_STEP     (1000/ZOOM_MAX_SPEED)   //unit: ms/step

/* The following valus is experimental. */
#define ZOOM_OVERRUN_COUNT      0

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Focus related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct FOCUSPARA
{
    volatile UINT8          uiFocusPhase;
    volatile UINT8          Direction;
    volatile INT16          iFocusPosition;
    volatile INT16          iTargetStep;            //Record target focus step position.
    volatile UINT16         uiFocusDistIdx;         //Record current index of focus distance.
    volatile UINT16         uiFocusOffsetRatio;     //Record current offset ratio of focus distance.
    volatile BOOL           bFocusLeftOffset;
    volatile BOOL           bFocusIsr;              //Open Focus ISR.
    volatile BOOL           bSetBasisStep;
} FOCUS_Struct;

typedef enum _LENS_FOCUS_PI_LEVEL {
    FOCUSPI_INSDIE_LEVEL = 0,
    FOCUSPI_OUTSDIE_LEVEL
} LENS_FOCUS_PI_LEVEL;

#define FOCUS_POS_HOME_DEF       0
#define FOCUS_POS_INNER_STOP     (FOCUS_POS_HOME_DEF-120)
#define FOCUS_POS_GARAGE         (FOCUS_POS_HOME_DEF-6)
#define FOCUS_POS_NEAR           (FOCUS_POS_HOME_DEF-12)
#define FOCUS_POS_HOME_PR        (FOCUS_POS_HOME_DEF-0)
#define FOCUS_POS_NORMAL_WIDE    (FOCUS_POS_HOME_DEF+12)
#define FOCUS_POS_FAR            (FOCUS_POS_HOME_DEF+450)
#define FOCUS_POS_OUTER_STOP     (FOCUS_POS_HOME_DEF+480)

#define FOCUS_BWD_LIMIT_OUTSIDE_PI  (480+10)
#define FOCUS_FWD_LIMIT_INSIDE_PI   (125+10)
#define FOCUS_BL_STEP               8      //(a-optronics nick: 4)
#define FOCUS_SPEED                 1000   //unit: us
#define FOCUS_INIT_DELAY            10     //unit: ms   //?

#define FOCUS_TOTAL_PHASE           8

#define FOCUS_ZOOM_TABLE_COLUMN     10
#define FOCUS_ZOOM_BASIS_COLUMN     (FOCUS_ZOOM_TABLE_COLUMN-2) // 0.5m

#define FOCUS_MIN_SPEED             40  //pps
#define FOCUS_MAX_SPEED             500 //pps
#define FOCUS_SPEED_PER_STEP        (1000/FOCUS_MAX_SPEED)   //unit: ms/step

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

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Shutter related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct SHUTTERPARA
{
    volatile UINT8          uiCurrentState; //Record current LENS_SHUTTER_STATE in SHUTTER state machine.
} SHUTTER_Struct;

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
