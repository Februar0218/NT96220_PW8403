/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_ST8005.h
    @ingroup    mISYSLens

    @brief      ST8005 (with Macro) lens driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/09/24
*/

#ifndef _LENSDRV_ST8005_H
#define _LENSDRV_ST8005_H
//#include "constant.h"
#include "Type.h"

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
    volatile BOOL           bZoomIsr;       //Open Zoom ISR.
    volatile UINT8          uiZoomWideCnt;  //Count how many times the lens has zoom to wide.
    volatile UINT8          Direction;      //Record current zoom direction.
    volatile UINT8          ZoomOverRun;    //Limit zoom over-run step.
    volatile INT16          ZoomStep;       //Record current zoom step position.
    volatile INT16          ZoomLastStep;   //Record last zoom step position.
    volatile BOOL           bZoomBoundaryAutoCheck;   //Auto check zoom bundary.
    volatile BOOL           bDoOptZoomDraw;           //OpticalZoom Callback to Draw.
    volatile unsigned short StepMove;       //Limit lens zoom steps in running state.
    volatile unsigned short StepCnt;        //Record how many steps the lens has zoom in running state.
    volatile INT16          ZoomBLStep;     //Record current zoom step1 position.
    volatile INT16          ZoomBLStep2;    //Record current zoom step2 position.
    volatile UINT32         uiZoomBL;       //Zoom back lash step.
    volatile UINT32         uiZoomAFStep;   //This is used to test miss step.
    volatile UINT8          uiLastSection;  //Record last zoom section.
    volatile INT32          ZoomPR_realStep;//The real distance between inner stop and PR for zoom
    volatile UINT32         uiGPIOIntPol;   //Positive or negative trigger for Zoom PI
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
    ZOOM_STATE_GETBL1,          //geting BL state1
    ZOOM_STATE_GETBL2,          //geting BL state2
    ZOOM_STATE_GETBL3,          //geting BL state3
    ZOOM_STATE_GETBL4,          //geting BL state4
    ZOOM_STATE_GETPROFFSET1,    //geting PR offset state1
    ZOOM_STATE_GETPROFFSET2,    //geting PR offset state2
    ZOOM_STATE_CLOSING,         //closing state
    ZOOM_STATE_CLOSING_HOME,    //closing state got PR
    ZOOM_STATE_CLOSED,          //closed state
    ZOOM_STATE_FOCUSING         //focusing state
} LENS_ZOOM_STATE;

typedef enum _LENS_ZOOM_PR_LEVEL {
    ZOOMPR_INSDIE_LEVEL = 0,
    ZOOMPR_OUTSDIE_LEVEL
} LENS_ZOOM_PR_LEVEL;

#define ZOOM_POS_INNER_STOP     0
#define ZOOM_POS_GARAGE         65
#define ZOOM_POS_HOME_PR        130
#define ZOOM_POS_WIDE           (ZOOM_POS_HOME_PR + 271)
////#define ZOOM_POS_WIDE           (ZOOM_POS_HOME_PR + 317) // just a test!!!
#define ZOOM_POS_TELE           (ZOOM_POS_HOME_PR + 729)
#define ZOOM_POS_OUTER_STOP     (ZOOM_POS_HOME_PR + 851) // 2010-04-19

#define ZOOM_SECTION_TELE       3
#define ZOOM_SECTION_WIDE       1
#define ZOOM_BL_STEP            50
#define ZOOM_BL_STEP_LIMIT      60
#define ZOOM_BL_CALIBRATION_STEP    300

#define _ZOOM_PSEUDO_PWM_       DISABLE
/* The following valus is experimental. */
#define ZOOM_OVERRUN_COUNT                 9
#define _ZOOM_PSEUDO_PWM_PERIOD_           60000   //unit: us
#define _ZOOM_PSEUDO_PWM_PERIOD_ADD_       5000    //unit: us
#define _ZOOM_PSEUDO_PWM_ONTIME_           10000   //unit: us
#define _ZOOM_PSEUDO_PWM_ONTIME_ADD_       3000    //unit: us
#define _ZOOM_PSEUDO_PWM_SPEED_            2000    //unit: us/step
#define _ZOOM_PSEUDO_PWM_CHECKCOUNT_       5       //unit: step
#define _ZOOM_PSEUDO_PWM_CHECKDIFFCOUNT_   1       //unit: step (_LENS_ZOOM_PSEUDO_PWM_CHECKCOUNT_*20%)
#define _ZOOM_PSEUDO_PWM_OVERRUNCOUNT_     1       //unit: step
#define _ZOOM_PSEUDO_PWM_FORCETOBRAKETIME_ 200     //unit: ms

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

#define FOCUS_ZOOM_TABLE_COLUMN     2

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
