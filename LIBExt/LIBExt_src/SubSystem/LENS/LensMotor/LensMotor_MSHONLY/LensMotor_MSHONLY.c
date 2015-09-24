/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       LensMotor_MSHONLY.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "M-Shutter ONLY"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2011/07/14
*/

#include "OSCommon.h"
#include "PWM.h"
#include "SIF.h"
#include "GPIO.h"
#include "pad.h"
#include "top.h"
#include "Timer.h"
#include "Lens.h"
#include "Lens_int.h"
#include "LensMotor_DUMMY.h"
#include "LensMotor_MSHONLY.h"

//static var declare
static MOTOR_TAB        gMSHONLYObj;//extern object for others
static MOTOR_PVT_OBJ    gMSHONLYPvtObj;//private object for internal usage
//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_select_mes((sel))
//new architecture
#define MOTOR_PINDEF_MOTO_IN1            (gMSHONLYPvtObj.uiPinInput1)//SN_MES0 = AGPIO20
#define MOTOR_PINDEF_MOTO_IN2            (gMSHONLYPvtObj.uiPinInput2)//SN_MES1 = AGPIO21
//#define MOTOR_PINDEF_MOTO_IN3            (gMSHONLYPvtObj.uiPinInput3)
//#define MOTOR_PINDEF_MOTO_IN4            (gMSHONLYPvtObj.uiPinInput4)

//#define MOTOR_PINDEF_IRIS_PIN0           MOTOR_PINDEF_MOTO_IN3
//#define MOTOR_PINDEF_IRIS_PIN1           MOTOR_PINDEF_MOTO_IN4

#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN2

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorMSHONLY_init(PLENS_DEVICE_OBJ pLensDev);
static void motorMSHONLY_changeMode(MOTOR_MODE state);
static void motorMSHONLY_resetInputPin(void);
static void motorMSHONLY_shutter_setState(UCHAR state);


static MOTOR_TAB gMSHONLYObj =
{
    motorMSHONLY_init,
    motorDUMMY_power,
    motorMSHONLY_changeMode,
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
    motorMSHONLY_shutter_setState,
    NULL
};

static MOTOR_PVT_OBJ gMSHONLYPvtObj =
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

    Init MSHONLY pin assignment

    @param None.

    @return None.
*/
static void motorMSHONLY_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gMSHONLYPvtObj.bInitYet)
    {
        debug_err(("motorMSHONLY_init already\n\r"));
        return;
    }

    if(pLensDev->uiMotorCtrlMode == MOTOR_CTRL_SIF)
    {
    }
    else
    {
        //assign pin number from user,to be construct.
        gMSHONLYPvtObj.uiPinInput1  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        gMSHONLYPvtObj.uiPinInput2  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        //gMSHONLYPvtObj.uiPinInput3  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_2];
        //gMSHONLYPvtObj.uiPinInput4  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_3];
        //debug_err(("[MSHONLY]motorMSHONLY_init,%d %d %d %d\n\r",gMSHONLYPvtObj.uiPinInput1,gMSHONLYPvtObj.uiPinInput2,gMSHONLYPvtObj.uiPinInput3,gMSHONLYPvtObj.uiPinInput4));
    }

    //#NT#2011/07/14#Chris Chung -begin
    //#NT#Since 220 change the pinmux usage, we should mark pinmux code here.
    #if 0
    MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
    #endif
    //#NT#2011/07/14#Chris Chung -end
    gpio_setDir(MOTOR_PINDEF_MOTO_IN1, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2, GPIO_DIR_OUTPUT);
    //gpio_setDir(MOTOR_PINDEF_MOTO_IN3, GPIO_DIR_OUTPUT);
    //gpio_setDir(MOTOR_PINDEF_MOTO_IN4, GPIO_DIR_OUTPUT);

    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);

    gMSHONLYPvtObj.bInitYet = TRUE;
    return;
}

/**
    Get MSHONLY motor object

    @return MSHONLY motor object pointer

*/
PMOTOR_TAB motor_getObject(void)
{
    return &gMSHONLYObj;
}

/**
    motor mode setting

    Set MOTOR_MODE_STANDBY/MOTOR_MODE_ZOOM/MOTOR_MODE_FOCUS/MOTOR_MODE_APERTURE/MOTOR_MODE_SHUTTER to motor

    @param state - The mode state of motor.

    @return None.
*/
static void motorMSHONLY_changeMode(MOTOR_MODE state)
{
    if(!gMSHONLYPvtObj.bInitYet)
    {
        debug_err(("motorMSHONLY_init not ready\n\r"));
        return;
    }
    //debug_err(("[MSHONLY]motorMSHONLY_mode_change, state=%d\n\r", state));
    switch(state)
    {
        case MOTOR_MODE_STANDBY:
            motorMSHONLY_resetInputPin();
            break;
        case MOTOR_MODE_ZOOM:
            motorMSHONLY_resetInputPin();
            //MOTOR_PinMux_Select_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_FOCUS:
            motorMSHONLY_resetInputPin();
//            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_APERTURE:
            motorMSHONLY_resetInputPin();
//            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_SHUTTER:
            motorMSHONLY_resetInputPin();
            //#NT#2011/07/14#Chris Chung -begin
            //#NT#Since 220 change the pinmux usage, we should mark pinmux code here.
            #if 0
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            #endif
            //#NT#2011/07/14#Chris Chung -end
            break;
        default:
            debug_err(("[MSHONLY]motorMSHONLY_changeMode,UNKNOW\n\r"));
            break;
    }
    return;
}

/**
    Reset input pin of motor

    Reset input pin of motor.

    @param None

    @return None.
*/
static void motorMSHONLY_resetInputPin(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);
    return;
}

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

/**
    motor shutter mode action

    Set phase to motor in shutter mode

    @param state - The phase state of shutter mode.

    @return None.
*/
static void motorMSHONLY_shutter_setState(UCHAR state)
{
    if(!gMSHONLYPvtObj.bInitYet)
    {
        debug_err(("motorMSHONLY_init not ready\n\r"));
        return;
    }
    debug_ind(("[MSHONLY]motorMSHONLY_shutter_setState,state=0x%x\n\r",state));

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES0);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES0);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES1);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES1);

    return;
}

