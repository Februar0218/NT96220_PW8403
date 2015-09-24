/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_PSEUDO.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "PSEUDO"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

/*
    Todo: Randy
    1. Re-write function name.
    2. Define motor funciton table in Lens.h, refer to AUDIO_CODEC_FUNC.
    3. Clear unused dummy function.
    4. Use device obj to set pin.
*/
/*
    Todo:
    1. motorPSEUDO_pinin_reset -> motorPSEUDO_resetInputPin
    2. motorPSEUDO_motorobject_get -> motorPSEUDO_getObject
    3. motorPSEUDO_zoomPR_check -> motorPSEUDO_zoom_checkPR
    4. motorPSEUDO_mode_change -> motorPSEUDO_changeMode
    5. motorPSEUDO_zoom_action -> motorPSEUDO_zoom_setState
    6. motorPSEUDO_pwm_process -> motorPSEUDO_pwm_hdlPWM (hdlSpeed)
`   7. MOTOR_PinMux_Select_MES -> upper case
*/

#include "OSCommon.h"
#include "PWM.h"
#include "SIF.h"
#include "GPIO.h"
#include "pad.h"
#include "top.h"
#include "Timer.h"
#include "Lens_int.h"
#include "LensMotor_DUMMY.h"
#include "LensMotor_PSEUDO.h"

//static var declare
static MOTOR_TAB        gPSEUDOObj;//extern object for others
static MOTOR_PVT_OBJ    gPSEUDOPvtObj;//private object for internal usage
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorPSEUDO_init(PLENS_DEVICE_OBJ pLensDev);

static MOTOR_TAB gPSEUDOObj =
{
    motorPSEUDO_init,
    motorDUMMY_power,
    motorDUMMY_changeMode,
    motorDUMMY_setMotorInfo,
    motorDUMMY_zoom_setState,
    motorDUMMY_zoom_setPIInt,
    motorDUMMY_zoom_clearPIInt,
    motorDUMMY_zoom_checkPIInt,
    motorDUMMY_zoom_checkPR,
    motorDUMMY_zoom_checkPI,
    motorDUMMY_focus_checkPI,
    motorDUMMY_focus_setState,
    motorDUMMY_aperture_setState,
    motorDUMMY_shutter_setState,
    NULL
};

static MOTOR_PVT_OBJ gPSEUDOPvtObj =
{
    FALSE,
    0,0,0,0,//pin assign

    FALSE,//pwm control
    LENS_NULL_TIMER,
    10000,//pwm on
    30000,//pwm off
    MOTOR_ZOOM_OFF
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with common related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor init

    Init PSEUDO pin assignment

    @param None.

    @return None.
*/
static void motorPSEUDO_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gPSEUDOPvtObj.bInitYet)
    {
        debug_err(("motorPSEUDO_init already\n\r"));
        return;
    }

    if(pLensDev->uiMotorCtrlMode == MOTOR_CTRL_SIF)
    {
    }
    else
    {
    }
    gPSEUDOPvtObj.bInitYet = TRUE;
    return;
}

/**
    Get PSEUDO motor object

    @return PSEUDO motor object pointer

*/
PMOTOR_TAB motor_getObject(void)
{
    return &gPSEUDOObj;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with pwm related API.
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom related API.
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with focus related API.
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with aperture related API.
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with shutter related API.
//--------------------------------------------------------------------------------------------------------------------------

