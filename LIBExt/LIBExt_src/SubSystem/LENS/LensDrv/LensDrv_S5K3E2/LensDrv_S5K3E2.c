/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_FF.c
    @ingroup    mISYSLens

    @brief      FF lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/02/20
*/

#include <stdio.h>
#include <stdlib.h>
#include "Lens_int.h"
#include "LensDrv_S5K3E2.h"
#include "LensDrv_DUMMY.h"
#include "debug.h"
#include "Timer.h"
#include "Type.h"
#include "Utility.h"
#include "Sensor.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for function declaration.
//--------------------------------------------------------------------------------------------------------------------------

//lens
static void     lensS5K3E2_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensS5K3E2_isFFLens(void);

//zoom
static INT32    lensS5K3E2_zoom_initPart1(void);
static INT32    lensS5K3E2_zoom_initPart2(void);

//focus
//static void     lensFF_focus_initPulse(void);
static void     lensS5K3E2_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position);
static INT32    lensS5K3E2_focus_gotoHome(void);
static INT32    lensS5K3E2_focus_gotoGarage(void);
static INT16    lensS5K3E2_focus_getCurrentPosition(void);
static INT16    lensS5K3E2_focus_getMaxPosition(void);
static INT16    lensS5K3E2_focus_getMinPosition(void);
static UINT32   lensS5K3E2_focus_getFL(UINT32 zoomSection);

//aperture
static void     lensS5K3E2_aperture_setMotorDriver(LENS_IRIS_POSITION phase);
static INT32    lensS5K3E2_aperture_gotoState(LENS_APERTURE_ACT position);
static UINT32   lensS5K3E2_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);
static LENS_APERTURE_ACT lensS5K3E2_aperture_getState(void);

//shutter
static INT32    lensS5K3E2_shutter_gotoState(LENS_SHUTTER_ACT newState);
static UINT32   lensS5K3E2_shutter_getPulsePeriod(void);


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------

static FOCUS_Struct g_pFocusPara={0};
static IRIS_Struct g_pIrisPara={0};
static SHUTTER_Struct g_pShutterPara={0};

/*static UINT8 g_pFocusStepperPhase[FOCUS_TOTAL_PHASE+1] =
{
    //Input=(IN1, IN2, IN3, IN4)    //Output=(O2,O3,O6,O7)=(/B,B,/A,A)
    STEPPER_PHASE_M4(1,0,0,1),      //0x1001 = 9
    STEPPER_PHASE_M4(0,0,0,1),      //0x1000 = 8
    STEPPER_PHASE_M4(0,1,0,1),      //0x1010 = a
    STEPPER_PHASE_M4(0,1,0,0),      //0x0010 = 2
    STEPPER_PHASE_M4(0,1,1,0),      //0x0110 = 6
    STEPPER_PHASE_M4(0,0,1,0),      //0x0100 = 4
    STEPPER_PHASE_M4(1,0,1,0),      //0x0101 = 5
    STEPPER_PHASE_M4(1,0,0,0),      //0x0001 = 1
    STEPPER_PHASE_M4(0,0,0,0)       //off

};*/

UINT8 g_pIrisPhase[IRIS_POS_TOTAL_NUM] =
{
    STEPPER_PHASE_M2(0,1),
    STEPPER_PHASE_M2(1,0)
};

UINT8 g_pShutterPhase[LENS_SHUTTER_TOTAL_NUM] =
{
    STEPPER_PHASE_M2(0,1),
    STEPPER_PHASE_M2(1,0)
};

static LENS_TAB g_pFFObj =
{
    //Lens
    lensS5K3E2_init,
    lensS5K3E2_isFFLens,
    //Zoom
    lensS5K3E2_zoom_initPart1,
    lensS5K3E2_zoom_initPart2,
    lensDUMMY_zoom_ctrlRelease,
    lensDUMMY_zoom_ctrlPress,
    lensDUMMY_zoom_ctrlPress_inner,
    lensDUMMY_zoom_getCurrentPosition,
    lensDUMMY_zoom_getCurrentSection,
    lensDUMMY_zoom_getMaxSection,
    lensDUMMY_zoom_getRatio,
    lensDUMMY_zoom_retract,
    lensDUMMY_zoom_gotoSection,
    lensDUMMY_zoom_setAutoBoundaryCheck,
    lensDUMMY_zoomIsr_enable,
    lensDUMMY_zoom_getBL,
    lensDUMMY_zoom_setBL,
    lensDUMMY_zoom_doBL,
    //Focus
    lensS5K3E2_focus_gotoPosition,
    lensS5K3E2_focus_gotoHome,
    lensS5K3E2_focus_gotoGarage,
    lensS5K3E2_focus_getCurrentPosition,
    lensS5K3E2_focus_getMaxPosition,
    lensS5K3E2_focus_getMinPosition,
    lensS5K3E2_focus_getFL,
    lensDUMMY_focus_getZoomTblValue,
    lensDUMMY_focus_getZoomTblMaxIdx,
    lensDUMMY_focus_getCurrentDist,
    lensDUMMY_focus_getDiffStepByDist,
    lensDUMMY_focus_getBL,
    lensDUMMY_focus_setBL,
    lensDUMMY_focus_setFarStep,
    //Aperture
    lensS5K3E2_aperture_gotoState,
    lensS5K3E2_aperture_getState,
    lensS5K3E2_aperture_getFNO,
    //Shutter
    lensS5K3E2_shutter_gotoState,
    lensS5K3E2_shutter_getPulsePeriod,
    //Others
    NULL
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensS5K3E2_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    g_pFFObj.pMotor = pMotorTab;

       debug_msg("%s\r\n",__func__);
    if(g_pFFObj.pMotor == NULL)
    {
        debug_err(("lensFF: pMotor is null\r\n"));
        return;
    }
    //Focus parameter
    g_pFocusPara.uiFocusPhase = 0;
}

/**
    Get FF lens object

    @return FF lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    return &g_pFFObj;
}

BOOL lensS5K3E2_isFFLens(void)
{
    return TRUE;
}

static INT32 lensS5K3E2_zoom_initPart1(void)
{
    g_pFocusPara.iFocusPosition = FOCUS_POS_HOME_DEF;
    //move to part2,because part1 not init iic
    //Sensor_WriteReg(0x3130,0x00);

    return ERR_OK;
}

static INT32 lensS5K3E2_zoom_initPart2(void)
{
    //move from part1,because  part1 not init iic
    Sensor_WriteReg(0x3130,0x00);

    return ERR_OK;
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

#if 0
static void lensFF_focus_setMotorDriver(unsigned char phase)
{
    if(g_pFFObj.pMotor == NULL)
    {
        debug_err(("lensFF: pMotor is null\r\n"));
        return;
    }

    g_pFFObj.pMotor->focus_setState(g_pFocusStepperPhase[phase], 0);
}

static void lensFF_focus_doBL( unsigned char Steps )
{
    lensFF_focus_initPulse();

    lensFF_focus_runStep(DIRECTION_BWD,Steps);

    TimerDelayMs(FOCUS_INIT_DELAY);

    lensFF_focus_runStep(DIRECTION_FWD,Steps);
}

static void lensFF_focus_initPulse(void)
{
   lensFF_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);   //restore original phase

    TimerDelayMs(FOCUS_INIT_DELAY);         //init pulse 10ms
}
#endif

static void lensS5K3E2_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position)
{
    unsigned short Step;

    g_pFocusPara.iFocusPosition = position;

    Step = position;
    //debug_msg("%s,%d,%d\r\n",__func__,position,Step);
    if(Step)
    {
        Sensor_WriteReg(0x3130,0x03);
        Sensor_WriteReg(0x3131,(Step&0xff00)>>8);
        Sensor_WriteReg(0x3132,Step& 0xff);
    }else{
        Sensor_WriteReg(0x3130,0x00);
    }
}

static INT32 lensS5K3E2_focus_gotoHome(void)
{
    g_pFocusPara.iFocusPosition = FOCUS_POS_HOME_DEF;
    Sensor_WriteReg(0x3130,0x00);

    return ERR_OK;
}

static INT32 lensS5K3E2_focus_gotoGarage(void)
{
        return ERR_OK;

}

static INT16 lensS5K3E2_focus_getCurrentPosition(void)
{
    return g_pFocusPara.iFocusPosition;
}

static INT16 lensS5K3E2_focus_getMaxPosition(void)
{
    return FOCUS_POS_OUTER_STOP;
}

static INT16 lensS5K3E2_focus_getMinPosition(void)
{
    return FOCUS_POS_INNER_STOP;
}

static UINT32 lensS5K3E2_focus_getFL(UINT32 zoomSection)
{
    return 57; // 2011-01-07 Chris
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensS5K3E2_aperture_setMotorDriver(LENS_IRIS_POSITION phase)
{
    if(g_pFFObj.pMotor == NULL)
    {
        debug_err(("lensFF: pMotor is null\r\n"));
        return;
    }

    g_pFFObj.pMotor->aperture_setState(g_pIrisPhase[phase]);
}

static INT32 lensS5K3E2_aperture_gotoState(LENS_APERTURE_ACT newState)
{
    switch(newState)
    {
    case LENS_APERTURE_SMALL:
        lensS5K3E2_aperture_setMotorDriver(IRIS_POS_SMALL);
        g_pIrisPara.uiCurrentState = newState;
        break;
    case LENS_APERTURE_BIG:
        lensS5K3E2_aperture_setMotorDriver(IRIS_POS_BIG);
        g_pIrisPara.uiCurrentState = newState;
        break;
    default:
        debug_err(("lensFF: aperture goto wrong state=%d\r\n", newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

static UINT32 lensS5K3E2_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    //return 28; // 2010-12-07 Chris
    return 30;//2011-07-20 ethan
}

static LENS_APERTURE_ACT lensS5K3E2_aperture_getState(void)
{
    return g_pIrisPara.uiCurrentState;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensS5K3E2_shutter_setMotorDriver(LENS_SHUTTER_ACT phase)
{
    if(g_pFFObj.pMotor == NULL)
    {
        debug_err(("lensFF: pMotor is null\r\n"));
        return;
    }

    g_pFFObj.pMotor->shutter_setState(g_pShutterPhase[phase]);
}

static INT32 lensS5K3E2_shutter_gotoState(LENS_SHUTTER_ACT newState)
{
    switch(newState)
    {
    case LENS_SHUTTER_CLOSE:
    case LENS_SHUTTER_OPEN:
        lensS5K3E2_shutter_setMotorDriver(newState);
        g_pShutterPara.uiCurrentState = newState;
        break;
    default:
        debug_err(("lensFF: shutter goto wrong state=%d\r\n", newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

static UINT32 lensS5K3E2_shutter_getPulsePeriod(void)
{
    return SHUTTER_PULSE_PERIOD;
}
