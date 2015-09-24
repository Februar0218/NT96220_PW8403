/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       LensMotor_WT7026.h
    @ingroup    mISYSLens

    @brief      Weltrend WT7026 driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/10/28
*/

#ifndef _LENSMOTOR_WT7026_H
#define _LENSMOTOR_WT7026_H
#include "Type.h"

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_PVT_OBJ
{
    //GPIO related var
    BOOL          bInitYet;
    BOOL          bPowerInverse;
    BOOL          bMotorInverse;
    UINT32        uiPinPower;    
    UINT32        uiPinReset;
    UINT32        uiPinSLD;
    UINT32        uiPinSIFCLK;
    UINT32        uiPinIN4A;
    UINT32        uiPinIN4B;
    UINT32        uiPinIN5A;
    UINT32        uiPinIN5B;

    UINT32        uiPinZoomPI;
    UINT32        uiPinZoomPR;
    UINT32        uiPinFocusPI;
    UINT32        uiZoomPIINT;

    //SIF,WT7026 usage
    UINT32        uiSIFType;
    SIF_CH        uiSIFChannel;
    UINT32        uiGPIOSen;                  ///< GPIO pin number for SEN, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32        uiGPIOClk;                  ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32        uiGPIOData;                 ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO

    //pwm control related var
    BOOL          bPwmCtrl;
    UINT          uiPwmTimerID;
    //#NT#2009/12/17#Chris Chung -begin
    UINT32        uiPwmPeriodTime;
    UINT32        uiPwmOnTime;
    //#NT#2009/12/17#Chris Chung -end
    MOTOR_ZOOM_ACT        pwmprevdirection;

    MOTOR_IRIS_VOLTAGE uiIrisVoltage;
} MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;

#define _LENS_ZOOM_PSEUDO_PWM_          ENABLE
#define _LENS_ZOOM_PSEUDO_PWM_PERIOD_   40000   //unit: us
#define _LENS_ZOOM_PSEUDO_PWM_ONTIME_   10000   //This value is experimental.

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
// This zone is for Motor-driver Register Definition
//--------------------------------------------------------------------------------------------------------------------------
#define ADDRESS_SHIFT_OFFSET            5
#define VALUE_SHIFT_OFFSET              0

#define Address_123ch_CtrlMode          0x00    //000
#define Address_45ch_CtrlMode           0x01    //001
#define Address_12ch_STM_Enable         0x02    //010
#define Address_34ch_SerialCtrl         0x02    //010
#define Address_12ch_DAC_Setup          0x03    //011
#define Address_3ch_DAC_Setup           0x04    //100
#define Address_4ch_DAC_Setup           0x05    //101
#define Address_5ch_DAC_Setup           0x06    //110
#define Address_Common_Ctrl             0x07    //111

#define MOTOR_RESET_MIN_TIME            1       //unit: us

#endif

