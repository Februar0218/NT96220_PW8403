/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_DUMMY.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "DUMMY"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
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

//static var declare

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with common related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor init

    Init DUMMY pin assignment

    @param None.

    @return None.
*/
void motorDUMMY_init(PLENS_DEVICE_OBJ pLensDev)
{
    return;
}

/**
    motor power setting

    Turn on/off motor power

    @param state - The power state of motor.

    @return None.
*/
void motorDUMMY_power(MOTOR_POWER state, UINT32 param1)
{
    return;
}

/**
    motor mode setting

    Set MOTOR_MODE_STANDBY/MOTOR_MODE_ZOOM/MOTOR_MODE_FOCUS/MOTOR_MODE_APERTURE/MOTOR_MODE_SHUTTER to motor

    @param state - The mode state of motor.

    @return None.
*/
void motorDUMMY_changeMode(MOTOR_MODE state)
{
    return;
}

/**
    motor info setting

    Set MOTOR_INFO_TAB info to motor

    @param pTab - The MOTOR_INFO_TAB pointer.

    @return None.
*/
void motorDUMMY_setMotorInfo(PMOTOR_INFO_TAB pTab)
{
    return;
}

/**
    Reset input pin of motor

    Reset input pin of motor.

    @param None

    @return None.
*/
void motorDUMMY_resetInputPin(void)
{
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with pwm related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    PWM control handler.

    DC motor will use this process handler to set the voltage output period.

    @param None

    @return None.
*/
void motorDUMMY_pwm_hdlPWM(void)
{
    return;
}

/**
    PWM action

    Create / Delete a timer to process PWM control.

    @param state - PWM control handler create or delete
    @param pwmprevdirection - The direction of the PWM.

    @return None.
*/
void motorDUMMY_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection)
{
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    Zoom PI check

    Check zoom PI status

    @param None.

    @return None.
*/
BOOL motorDUMMY_zoom_checkPI(void)
{
    return TRUE;
}

/**
    Zoom PI interrupt check

    Check zoom PI interrupt status

    @param None.

    @return BOOL - The zoom PI interrupt status.
*/
BOOL motorDUMMY_zoom_checkPIInt(void)
{
    return TRUE;
}


/**
    Zoom PR check

    Check zoom PR status

    @param None.

    @return BOOL - The zoom PR status.
*/
BOOL motorDUMMY_zoom_checkPR(void)
{
    return TRUE;
}

/**
    Zoom PI interrupt status clear

    clear zoom PI interrupt status

    @param None.

    @return None.
*/
void motorDUMMY_zoom_clearPIInt(void)
{
    return;
}

/**
    Zoom PI interrupt setting

    Set zoom PI interrupt to DSP

    @param state - The zoom PI interrupt state.

    @return None.
*/
void motorDUMMY_zoom_setPIInt(BOOL state)
{
    return;
}

/**
    motor zoom mode action

    Set MOTOR_ZOOM_BRAKE/MOTOR_ZOOM_FWD/MOTOR_ZOOM_BWD/MOTOR_ZOOM_OFF to motor in Zoom mode

    @param state - The action state of zoom mode.

    @return None.
*/
void motorDUMMY_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1)
{
    return;
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with focus related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    Focus PI check

    Check focus PI status

    @param None.

    @return BOOL - The focus PI status.
*/
BOOL motorDUMMY_focus_checkPI(void)
{
    return TRUE;
}

/**
    motor focus mode action

    Set phase to motor in focus mode

    @param state - The phase state of focus mode.

    @return None.
*/
void motorDUMMY_focus_setState(UINT32 state, UINT32 param1)
{
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with aperture related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    motor aperture mode action

    Set phase to motor in aperture mode

    @param state - The phase state of aperture mode.

    @return None.
*/
void motorDUMMY_aperture_setState(UCHAR state)
{
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with shutter related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor shutter mode action

    Set phase to motor in shutter mode

    @param state - The phase state of shutter mode.

    @return None.
*/
void motorDUMMY_shutter_setState(UCHAR state)
{
    return;
}

