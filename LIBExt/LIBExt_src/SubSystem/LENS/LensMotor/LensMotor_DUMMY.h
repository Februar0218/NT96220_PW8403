/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_DUMMY.h
    @ingroup    mISYSLens

    @brief      DUMMY motor driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSMOTOR_DUMMY_H
#define _LENSMOTOR_DUMMY_H
#include "Type.h"

//This zone is extern func declare
//--------------------------------------------------------------------------------------------------------------------------
void    motorDUMMY_init(PLENS_DEVICE_OBJ pLensDev);
void    motorDUMMY_power(MOTOR_POWER state, UINT32 param1);
void    motorDUMMY_changeMode(MOTOR_MODE state);
void    motorDUMMY_setMotorInfo(PMOTOR_INFO_TAB pTab);
void    motorDUMMY_resetInputPin(void);
void    motorDUMMY_pwm_hdlPWM(void);
void    motorDUMMY_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection);
void    motorDUMMY_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1);
void    motorDUMMY_zoom_setPIInt(BOOL state);
void    motorDUMMY_zoom_clearPIInt(void);
BOOL    motorDUMMY_zoom_checkPIInt(void);
BOOL    motorDUMMY_zoom_checkPR(void);
BOOL     motorDUMMY_zoom_checkPI(void);
BOOL    motorDUMMY_focus_checkPI(void);
void    motorDUMMY_focus_setState(UINT32 state, UINT32 param1);
void    motorDUMMY_aperture_setState(UCHAR state);
void    motorDUMMY_shutter_setState(UCHAR state);
#endif

