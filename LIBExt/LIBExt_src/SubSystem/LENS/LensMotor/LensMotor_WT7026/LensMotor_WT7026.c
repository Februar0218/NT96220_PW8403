/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       LensMotor_WT7026.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "Weltrend WT7026"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2011/07/14
*/
#include "OSCommon.h"
#include "pwm.h"
#include "sif.h"
#include "gpio.h"
#include "pad.h"
#include "top.h"
#include "timer.h"
#include "Lens_int.h"
#include "LensMotor_WT7026.h"
#include "LensMotor_Dummy.h"
#include "Utility.h"

//static var declare
static MOTOR_TAB        gWT7026Obj;//extern object for others
static MOTOR_PVT_OBJ    gWT7026PvtObj;//private object for internal usage

static BOOL             g_bMotorPowerOn = FALSE;
static BOOL             g_bMotorResetOn = FALSE;

//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_select_mes((sel))
#define MOTOR_PINMUX_SELECT_PWM(sel) pinmux_select_pwm((sel))

//new architecture
#define MOTOR_PINDEF_MOTO_POWER          (gWT7026PvtObj.uiPinPower)
#define MOTOR_PINDEF_MOTO_RESET          (gWT7026PvtObj.uiPinReset)
#define MOTOR_PINDEF_MOTO_SLD            (gWT7026PvtObj.uiPinSLD)
#define MOTOR_PINDEF_MOTO_SIFCLK         (gWT7026PvtObj.uiPinSIFCLK)
#define MOTOR_PINDEF_MOTO_IN1            (gWT7026PvtObj.uiPinIN4A)
#define MOTOR_PINDEF_MOTO_IN2            (gWT7026PvtObj.uiPinIN4B)
#define MOTOR_PINDEF_MOTO_IN3            (gWT7026PvtObj.uiPinIN5A)
#define MOTOR_PINDEF_MOTO_IN4            (gWT7026PvtObj.uiPinIN5B)

#define MOTOR_PINDEF_IRIS_PIN0           MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_IRIS_PIN1           MOTOR_PINDEF_MOTO_IN2
#define MOTOR_PINDEF_FOCUS_PIN0          MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_FOCUS_PIN1          MOTOR_PINDEF_MOTO_IN2
#define MOTOR_PINDEF_FOCUS_PIN2          MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_FOCUS_PIN3          MOTOR_PINDEF_MOTO_IN4
#define MOTOR_PINDEF_ZOOM_PIN0           MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_ZOOM_PIN1           MOTOR_PINDEF_MOTO_IN4
#define MOTOR_PINDEF_ZOOM_PR             (gWT7026PvtObj.uiPinZoomPR)
#define MOTOR_PINDEF_ZOOM_PI             (gWT7026PvtObj.uiPinZoomPI)
#define MOTOR_PINDEF_FOCUS_PI            (gWT7026PvtObj.uiPinFocusPI)
#define MOTOR_ITR_ZOOM_PI                (gWT7026PvtObj.uiZoomPIINT)//dgpio[0] is for ZOOM PI
#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN4

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorWT7026_init(PLENS_DEVICE_OBJ pLensDev);
static void motorWT7026_power(MOTOR_POWER state, UINT32 param1);
static void motorWT7026_changeMode(MOTOR_MODE state);
static void motorWT7026_resetInputPin(void);
static void motorWT7026_resetMotor(void);
static void motorWT7026_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1);
static void motorWT7026_zoom_setPIInt(BOOL state);
static void motorWT7026_zoom_clearPIInt(void);
static BOOL motorWT7026_zoom_checkPIInt(void);
static BOOL motorWT7026_zoom_checkPR(void);
static BOOL motorWT7026_zoom_checkPI(void);
static BOOL motorWT7026_focus_checkPI(void);
static void motorWT7026_focus_setState(UINT32 state, UINT32 param1);
static void motorWT7026_aperture_setState(UCHAR state);
static void motorWT7026_shutter_setState(UCHAR state);
static void motorWT7026_sif_init(void);
static void motorWT7026_sif_WrReg(UINT32 addr,UINT32 val);
static void motorWT7026_gpio_WrReg(UINT32 val);

static void motorWT7026_gpio_powerOn(void);
static void motorWT7026_gpio_powerOff(void);
static BOOL motorWT7026_gpio_isPowerOn(void);
static void motorWT7026_gpio_resetOn(void);
static void motorWT7026_gpio_resetOff(void);
static BOOL motorWT7026_gpio_isResetOn(void);

static MOTOR_TAB gWT7026Obj =
{
    //common related cb
    motorWT7026_init,
    motorWT7026_power,
    motorWT7026_changeMode,
    motorDUMMY_setMotorInfo,

    //zoom related cb
    motorWT7026_zoom_setState,
    motorWT7026_zoom_setPIInt,
    motorWT7026_zoom_clearPIInt,
    motorWT7026_zoom_checkPIInt,
    motorWT7026_zoom_checkPR,
    motorWT7026_zoom_checkPI,

    //focus related cb
    motorWT7026_focus_checkPI,
    motorWT7026_focus_setState,

    //aperture related cb
    motorWT7026_aperture_setState,

    //shutter related cb
    motorWT7026_shutter_setState
};

static MOTOR_PVT_OBJ gWT7026PvtObj =
{
    //GPIO related var
    FALSE,//BOOL          bInitYet;
    0,//BOOL          bPowerInverse;
    0,//BOOL          bMotorInverse
    0,//UINT32        uiPinPower;
    0,//UINT32        uiPinReset;
    0,//UINT32        uiPinSLD;
    0,//UINT32        uiPinSIFCLK;
    0,//UINT32        uiPinIN4A;
    0,//UINT32        uiPinIN4B;
    0,//UINT32        uiPinIN5A;
    0,//UINT32        uiPinIN5B;

    0,//UINT32        uiPinZoomPI;
    0,//UINT32        uiPinZoomPR;
    0,//UINT32        uiPinFocusPI;
    0,//UINT32        uiZoomPIINT;

    //SIF,WT7026 usage
    LENS_SIF_CONTROLLER,//UINT32        uiSIFType;
    SIF_CH0,//SIF_CH        uiSIFChannel;
    0,//UINT32        uiGPIOSen;                  ///< GPIO pin number for SEN, valid when uimode == LCD_CTRL_MODE_GPIO
    0,//UINT32        uiGPIOClk;                  ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    0,//UINT32        uiGPIOData;                 ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO

    //pwm control related var
    FALSE,//BOOL          bPwmCtrl;
    LENS_NULL_TIMER,//UINT          uiPwmTimerID;
    10000,//UINT32        uiPwmonTime;
    30000,//UINT32        uiPwmoffTime;
    MOTOR_ZOOM_OFF,//MOTOR_ZOOM_ACT        pwmprevdirection;

    MOTOR_IRIS_VOLTAGE_30V//MOTOR_IRIS_VOLTAGE uiIrisVoltage;
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with common related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor init

    Init WT7026 pin assignment

    @param None.

    @return None.
*/
static void motorWT7026_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init already\n\r"));
        return;
    }
    LENS_ERR(("LENS_M: motorWT7026_init\n\r"));

    if((pLensDev->uiMotorCtrlMode & MOTOR_CTRL_SIF) == MOTOR_CTRL_SIF)
    {
        gWT7026PvtObj.uiSIFType = pLensDev->LensSIFPinObj.uiSIFType;
        //Resv,init sif channel mode!!
        if(pLensDev->LensSIFPinObj.uiSIFType != LENS_SIF_GPIOSIM)
        {
            gWT7026PvtObj.uiSIFChannel = pLensDev->LensSIFPinObj.uiSIFChannel;
            motorWT7026_sif_init();
        }
        else
        {
            gWT7026PvtObj.uiGPIOSen = pLensDev->LensSIFPinObj.uiGPIOSen;
            gWT7026PvtObj.uiGPIOClk = pLensDev->LensSIFPinObj.uiGPIOClk;
            gWT7026PvtObj.uiGPIOData = pLensDev->LensSIFPinObj.uiGPIOData;

            LENS_ERR(("%d %d %d\n\r",pLensDev->LensSIFPinObj.uiGPIOSen,
                                     pLensDev->LensSIFPinObj.uiGPIOClk,
                                     pLensDev->LensSIFPinObj.uiGPIOData));

            gpio_setDir(gWT7026PvtObj.uiGPIOSen , GPIO_DIR_OUTPUT);
            gpio_setDir(gWT7026PvtObj.uiGPIOClk, GPIO_DIR_OUTPUT);
            gpio_setDir(gWT7026PvtObj.uiGPIOData, GPIO_DIR_OUTPUT);

            gpio_clearPin(gWT7026PvtObj.uiGPIOSen);
            gpio_clearPin(gWT7026PvtObj.uiGPIOData);
            gpio_setPin(gWT7026PvtObj.uiGPIOClk);
        }
    }

    if((pLensDev->uiMotorCtrlMode & MOTOR_CTRL_GPIO) == MOTOR_CTRL_GPIO)
    {
        gWT7026PvtObj.uiPinReset    = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MODE_1];
        gWT7026PvtObj.uiPinIN4A     = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        gWT7026PvtObj.uiPinIN4B     = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        gWT7026PvtObj.uiPinIN5A     = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_2];
        gWT7026PvtObj.uiPinIN5B     = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_3];
        gWT7026PvtObj.uiPinPower    = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_PWR];
        gWT7026PvtObj.bPowerInverse = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_PWR_INVERSE];
        gWT7026PvtObj.bMotorInverse = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MOTOR_INVERSE];


        gWT7026PvtObj.uiPinZoomPI   = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PI];
        gWT7026PvtObj.uiPinZoomPR   = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR];
        gWT7026PvtObj.uiPinFocusPI  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI];

        gWT7026PvtObj.uiZoomPIINT   = pLensDev->uiMotorCtrlGPIOIntrID;

        gWT7026PvtObj.uiIrisVoltage = pLensDev->puiMotorCtrlParamArray[LENS_MOTOR_PARAM_IRIS_VOLTAGE];
    }

    //#NT#2011/07/14#Chris Chung -begin
    //#NT#Since 220 change the pinmux usage, we should mark pinmux code here.
    #if 0
    MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
    MOTOR_PINMUX_SELECT_PWM(PINMUX_GPIO);
    #endif
    //#NT#2011/07/14#Chris Chung -end

    gpio_setDir(MOTOR_PINDEF_MOTO_POWER, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_RESET, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN1, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN3, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN4, GPIO_DIR_OUTPUT);

    //#NT#2011/10/05#Hideo Lin -begin
    //#NT#Use API instead of setting GPIO directly
    motorWT7026_gpio_powerOff();
    motorWT7026_gpio_resetOn();
    //#NT#2011/10/05#Hideo Lin -end

    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);

    gpio_setDir(MOTOR_PINDEF_ZOOM_PR, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_ZOOM_PI, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_FOCUS_PI, GPIO_DIR_INPUT);

    //[Chris Chung] What does it do?
    //Ans: Pull NONE to use foreign input voltage.
    //     The pad number should follow NT96211.
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI], PAD_NONE);

    gWT7026PvtObj.bInitYet = TRUE;
}

/**
    Get WT7026 motor object

    @return WT7026 motor object pointer

*/
PMOTOR_TAB motor_getObject(void)
{
    return &gWT7026Obj;
}

/**
    motor power setting

    Turn on/off motor power

    @param state - The power state of motor.

    @return None.
*/
static void motorWT7026_power(MOTOR_POWER state, UINT32 param1)
{
//#NT#2011/10/05#Hideo Lin -begin
//#NT#Modify power control
#if 0

    switch(state)
    {
    case MOTOR_POWER_OFF:
        if(gpio_getPin(MOTOR_PINDEF_MOTO_RESET)==1)
        {
            motorWT7026_zoom_setPIInt(FALSE);
            gpio_clearPin(MOTOR_PINDEF_MOTO_RESET);
        }
        break;
    case MOTOR_POWER_ON:
        break;
    default:
        LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
        break;
    }

#else

    switch(state)
    {
    case MOTOR_POWER_OFF:
        if(motorWT7026_gpio_isPowerOn())
        {
            motorWT7026_sif_WrReg(Address_Common_Ctrl, 0x02); // TEST: power off
            motorWT7026_resetInputPin(); // TEST: clear pin

            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_gpio_powerOff();
            motorWT7026_gpio_resetOn();
        }
        break;

    case MOTOR_POWER_ON:
        if((motorWT7026_gpio_isPowerOn() == FALSE) || motorWT7026_gpio_isResetOn())
        {
            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_gpio_powerOn();
            motorWT7026_resetMotor();

            switch(param1)
            {
            case MOTOR_POWERON_PI1_ONLY:
                LENS_ERR(("LENS_M: %s: power with PI1 only\r\n", __func__));
                motorWT7026_sif_WrReg(Address_Common_Ctrl, 0x1A);       //111-11010: Power ON, PI1 ON, STM
                break;
            //#NT#2012/05/25#KS Hung -begin
            case MOTOR_POWERON_PI2_ONLY:
                LENS_ERR(("LENS_M: %s: power with PI2 only\r\n", __func__));
                motorWT7026_sif_WrReg(Address_Common_Ctrl, 0x16);       //111-11010: Power ON, PI2 ON, STM
                break;
            //#NT#2012/05/25#KS Hung -end
            case MOTOR_POWERON_NO_PI:
                LENS_ERR(("LENS_M: %s: power w/o PI1 and PI2\r\n", __func__));
                motorWT7026_sif_WrReg(Address_Common_Ctrl, 0x12);       //111-10010: Power ON, STM
                break;
            default:
                LENS_ERR(("LENS_M: %s: power with PI1 and PI2\r\n", __func__));
                motorWT7026_sif_WrReg(Address_Common_Ctrl, 0x1E);       //111-11110: Power ON, PI1/PI2 ON, STM
            }

            motorWT7026_sif_WrReg(Address_3ch_DAC_Setup, 0x02);     //100-00010: CH3 DAC, 4.6v, CV
            motorWT7026_sif_WrReg(Address_4ch_DAC_Setup, 0x0e);     //101-01110: CH4 DAC, 3.4v, CV
            motorWT7026_sif_WrReg(Address_5ch_DAC_Setup, 0x10);     //110-10000: CH5 DAC, 220mv, CC
            motorWT7026_sif_WrReg(Address_45ch_CtrlMode, 0x0B);     //001-01011: CH4/5 ctrl, IN/IN, CV
            //#NT#2010/02/02#Chris Chung -begin
            //#NT#add to reduce big current
            TimerDelayMs(10);
            //#NT#2010/02/02#Chris Chung -end
            motorWT7026_zoom_setPIInt(TRUE);
        }
        break;

    default:
        LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
        break;
    }

#endif
//#NT#2011/10/05#Hideo Lin -end
}

/**
    motor mode setting

    Set MOTOR_MODE_STANDBY/MOTOR_MODE_ZOOM/MOTOR_MODE_FOCUS/MOTOR_MODE_APERTURE/MOTOR_MODE_SHUTTER to motor

    @param state - The mode state of motor.

    @return None.
*/
static void motorWT7026_changeMode(MOTOR_MODE state)
{
//#NT#2011/10/05#Hideo Lin -begin
//#NT#Modify motor mode change control
#if 0

    static MOTOR_MODE lastState = MOTOR_MODE_STANDBY;

    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }

    LENS_MSG(("LENS_M: %s: state=%d\n\r", __func__, state));

    if(state != lastState)
    {
        if(gpio_getPin(MOTOR_PINDEF_MOTO_RESET)==1)
        {
            motorWT7026_zoom_setPIInt(FALSE);
            gpio_clearPin(MOTOR_PINDEF_MOTO_RESET);
        }
    }

    switch(state)
    {
    case MOTOR_MODE_STANDBY:
        motorWT7026_resetInputPin();
        break;
    case MOTOR_MODE_ZOOM:
    case MOTOR_MODE_FOCUS:
    case MOTOR_MODE_APERTURE:
    case MOTOR_MODE_SHUTTER:
        //#NT#2011/07/14#Chris Chung -begin
        //#NT#Since 220 change the pinmux usage, we should mark pinmux code here.
        #if 0
        if(state == MOTOR_MODE_SHUTTER)
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
        #endif
        //#NT#2011/07/14#Chris Chung -end

        if(gpio_getPin(MOTOR_PINDEF_MOTO_RESET)==0)
        {
            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_resetMotor();
            LENS_ERR(("LENS_M: %s: power w/o PI1 and PI2\r\n", __func__));
            //motorWT7026_sif_WrReg(Address_Common_Ctrl,   0x12);     //111-10010: Power ON, STM
            motorWT7026_sif_WrReg(Address_Common_Ctrl,   0x1E);     //111-11110: Power ON, PI1/PI2 ON, STM
            motorWT7026_sif_WrReg(Address_3ch_DAC_Setup, 0x02);     //100-00010: CH3 DAC, 4.6v, CV
            motorWT7026_sif_WrReg(Address_4ch_DAC_Setup, 0x12);     //101-10010: CH4 DAC, 3.0v, CV
            motorWT7026_sif_WrReg(Address_5ch_DAC_Setup, 0x10);     //110-10000: CH5 DAC, 220mv, CC
            motorWT7026_sif_WrReg(Address_45ch_CtrlMode, 0x0B);     //001-01011: CH4/5 ctrl, IN/IN, CV
            motorWT7026_zoom_setPIInt(TRUE);
        }
        break;
    case MOTOR_MODE_FIND_HOME:
        if(gpio_getPin(MOTOR_PINDEF_MOTO_RESET)==0)
        {
            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_resetMotor();
            LENS_ERR(("LENS_M: %s: power with PI1 and PI2\r\n", __func__));
            motorWT7026_sif_WrReg(Address_Common_Ctrl,   0x1E);     //111-11110: Power ON, PI1/PI2 ON, STM
            motorWT7026_sif_WrReg(Address_3ch_DAC_Setup, 0x02);     //100-00010: CH3 DAC, 4.6v, CV
            motorWT7026_sif_WrReg(Address_4ch_DAC_Setup, 0x12);     //101-10010: CH4 DAC, 3.0v, CV
            motorWT7026_sif_WrReg(Address_5ch_DAC_Setup, 0x10);     //110-10000: CH5 DAC, 220mv, CC
            motorWT7026_sif_WrReg(Address_45ch_CtrlMode, 0x0B);     //001-01011: CH4/5 ctrl, IN/IN, CV
            motorWT7026_zoom_setPIInt(TRUE);
        }
        break;
    default:
        LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
        break;
    }

    lastState = state;

#else

    static MOTOR_MODE lastState = MOTOR_MODE_STANDBY;

    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }

    LENS_MSG(("LENS_M: %s: state=%d\n\r", __func__, state));

    #if 0
    if(state != lastState)
    {
        if(motorWT7026_gpio_isResetOn() == FALSE)
        {
            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_gpio_resetOn();
        }
    }
    #endif

    switch(state)
    {
    case MOTOR_MODE_STANDBY:
        motorWT7026_resetInputPin();
        break;

    case MOTOR_MODE_ZOOM:
        #if 0
        //#NT#2011/09/23#Hideo Lin -begin
        //#NT#Should enable PI for zoom
        if(motorWT7026_gpio_isResetOn())
        {
            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_resetMotor();
            LENS_ERR(("LENS_M: %s: power with PI1 and PI2\r\n", __func__));
            motorWT7026_sif_WrReg(Address_Common_Ctrl,   0x1E);     //111-11110: Power ON, PI1/PI2 ON, STM
            motorWT7026_sif_WrReg(Address_3ch_DAC_Setup, 0x02);     //100-00010: CH3 DAC, 4.6v, CV
            motorWT7026_sif_WrReg(Address_4ch_DAC_Setup, 0x12);     //101-10010: CH4 DAC, 3.0v, CV
            motorWT7026_sif_WrReg(Address_5ch_DAC_Setup, 0x10);     //110-10000: CH5 DAC, 220mv, CC
            motorWT7026_sif_WrReg(Address_45ch_CtrlMode, 0x0B);     //001-01011: CH4/5 ctrl, IN/IN, CV
            motorWT7026_zoom_setPIInt(TRUE);
        }
        //#NT#2011/09/23#Hideo Lin -end
        #endif
        break;

    case MOTOR_MODE_FOCUS:
    case MOTOR_MODE_APERTURE:
    case MOTOR_MODE_SHUTTER:
        //#NT#2011/07/14#Chris Chung -begin
        //#NT#Since 220 change the pinmux usage, we should mark pinmux code here.
        #if 0
        if(state == MOTOR_MODE_SHUTTER)
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
        #endif
        //#NT#2011/07/14#Chris Chung -end

        #if 0
        if(motorWT7026_gpio_isResetOn())
        {
            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_resetMotor();
            LENS_ERR(("LENS_M: %s: power w/o PI1 and PI2\r\n", __func__));
            motorWT7026_sif_WrReg(Address_Common_Ctrl,   0x12);     //111-10010: Power ON, STM
            motorWT7026_sif_WrReg(Address_3ch_DAC_Setup, 0x02);     //100-00010: CH3 DAC, 4.6v, CV
            motorWT7026_sif_WrReg(Address_4ch_DAC_Setup, 0x12);     //101-10010: CH4 DAC, 3.0v, CV
            motorWT7026_sif_WrReg(Address_5ch_DAC_Setup, 0x10);     //110-10000: CH5 DAC, 220mv, CC
            motorWT7026_sif_WrReg(Address_45ch_CtrlMode, 0x0B);     //001-01011: CH4/5 ctrl, IN/IN, CV
            motorWT7026_zoom_setPIInt(TRUE);
        }
        #endif
        break;

    case MOTOR_MODE_FIND_HOME:
        #if 0
        if(motorWT7026_gpio_isResetOn())
        {
            motorWT7026_zoom_setPIInt(FALSE);
            motorWT7026_resetMotor();
            LENS_ERR(("LENS_M: %s: power with PI1 and PI2\r\n", __func__));
            motorWT7026_sif_WrReg(Address_Common_Ctrl,   0x1E);     //111-11110: Power ON, PI1/PI2 ON, STM
            motorWT7026_sif_WrReg(Address_3ch_DAC_Setup, 0x02);     //100-00010: CH3 DAC, 4.6v, CV
            motorWT7026_sif_WrReg(Address_4ch_DAC_Setup, 0x12);     //101-10010: CH4 DAC, 3.0v, CV
            motorWT7026_sif_WrReg(Address_5ch_DAC_Setup, 0x10);     //110-10000: CH5 DAC, 220mv, CC
            motorWT7026_sif_WrReg(Address_45ch_CtrlMode, 0x0B);     //001-01011: CH4/5 ctrl, IN/IN, CV
            motorWT7026_zoom_setPIInt(TRUE);
        }
        #endif
        break;

    default:
        LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
        break;
    }

    lastState = state;

#endif
//#NT#2011/10/05#Hideo Lin -end
}

/**
    Reset input pin of motor

    Reset input pin of motor.

    @param None

    @return None.
*/
static void motorWT7026_resetInputPin(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);
}

/**
    H/W reset motor.

    H/W reset motor.

    @param None

    @return None.
*/
static void motorWT7026_resetMotor(void)
{
    //#NT#2011/07/14#Chris Chung -begin
    //#NT#Since 220 change the pinmux usage, we should mark pinmux code here.
    #if 0
    MOTOR_PINMUX_SELECT_PWM(PINMUX_GPIO);
    #endif
    //#NT#2011/07/14#Chris Chung -end

    //#NT#2011/10/05#Hideo Lin -begin
    //#NT#Use API instead of setting GPIO directly
    //gpio_clearPin(MOTOR_PINDEF_MOTO_RESET);
    motorWT7026_gpio_resetOn();
    //#NT#2011/10/05#Hideo Lin -end

    TimerDelayUs(MOTOR_RESET_MIN_TIME);

    //#NT#2011/10/05#Hideo Lin -begin
    //#NT#Use API instead of setting GPIO directly
    //gpio_setPin(MOTOR_PINDEF_MOTO_RESET);
    motorWT7026_gpio_resetOff();
    //#NT#2011/10/05#Hideo Lin -end

    TimerDelayUs(MOTOR_RESET_MIN_TIME);
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
static BOOL motorWT7026_zoom_checkPI(void)
{
    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return FALSE;
    }
    LENS_MSG(("LENS_M: motorWT7026_zoomPI_check\n\r"));
    return gpio_getPin(MOTOR_PINDEF_ZOOM_PI);
}

/**
    Zoom PI interrupt check

    Check zoom PI interrupt status

    @param None.

    @return BOOL - The zoom PI interrupt status.
*/
static BOOL motorWT7026_zoom_checkPIInt(void)
{
    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return FALSE;
    }

    return gpio_getIntStatus(MOTOR_ITR_ZOOM_PI);
}


/**
    Zoom PR check

    Check zoom PR status

    @param None.

    @return BOOL - The zoom PR status.
*/
static BOOL motorWT7026_zoom_checkPR(void)
{
    BOOL ret, ret_tmp;

    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return FALSE;
    }
    //LENS_MSG(("LENS_M: motorWT7026_zoomPR_check\n\r"));
#if 0
    return gpio_getPin(MOTOR_PINDEF_ZOOM_PR);
#else
    ret = gpio_getPin(MOTOR_PINDEF_ZOOM_PR);

    /* check if PI is stable after 1 ms */
    do{
        SoftTimerDelayUs(1000);

        ret_tmp = gpio_getPin(MOTOR_PINDEF_ZOOM_PR);

        if(ret == ret_tmp)
            break;
        else
        {
            LENS_ERR(("LENS_E: %s: PR is not same as 1 ms before!!\r\n", __func__));
            ret = ret_tmp;
        }
    } while(1);

    return ret;
#endif
}

/**
    Zoom PI interrupt status clear

    clear zoom PI interrupt status

    @param None.

    @return None.
*/
static void motorWT7026_zoom_clearPIInt(void)
{
    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }

    gpio_clearIntStatus(MOTOR_ITR_ZOOM_PI);
}

/**
    Zoom PI interrupt setting

    Set zoom PI interrupt to DSP

    @param state - The zoom PI interrupt state.

    @return None.
*/
static void motorWT7026_zoom_setPIInt(BOOL state)
{
    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }
    LENS_MSG(("LENS_M: motorWT7026_zoomPIInt_set\n\r"));
    if(state)
    {
        gpio_clearIntStatus(MOTOR_ITR_ZOOM_PI);
        gpio_enableInt(MOTOR_ITR_ZOOM_PI);
    }
    else
    {
        gpio_disableInt(MOTOR_ITR_ZOOM_PI);
    }
}

/**
    motor zoom mode action

    Set MOTOR_ZOOM_BRAKE/MOTOR_ZOOM_FWD/MOTOR_ZOOM_BWD/MOTOR_ZOOM_OFF to motor in Zoom mode

    @param state - The action state of zoom mode.

    @return None.
*/
static void motorWT7026_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1)
{
    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }

    LENS_MSG(("LENS_M: %s, state=%d, param1=%d\n\r", __func__, state, param1));

    switch(state)
    {
    case MOTOR_ZOOM_BRAKE:
        LENS_MSG(("LENS_M: %s MOTOR_ZOOM_BRAKE\n\r", __func__));
        motorWT7026_sif_WrReg(Address_34ch_SerialCtrl, 0x0C);   //010-01100: CH3/4 serial ctrl, CH3 Brake
        break;

    case MOTOR_ZOOM_FWD:
        //#PIC#2010/11/17#Creator -begin
        LENS_MSG(("LENS_M: %s fwd=%d\n\r", __func__,gWT7026PvtObj.bMotorInverse));
        if (gWT7026PvtObj.bMotorInverse)
        {
            motorWT7026_sif_WrReg(Address_34ch_SerialCtrl, 0x08);//010-00100: CH3/4 serial ctrl, CH3 Forward
        }
        else
        {
            motorWT7026_sif_WrReg(Address_34ch_SerialCtrl, 0x04);//010-00100: CH3/4 serial ctrl, CH3 Forward
        }
        //#PIC#2010/11/17#Creator -end
        break;

    case MOTOR_ZOOM_BWD:
        //#PIC#2010/11/17#Creator -begin
        LENS_MSG(("LENS_M: %s bwd=%d\n\r", __func__,gWT7026PvtObj.bMotorInverse));
        if (gWT7026PvtObj.bMotorInverse)
        {
            motorWT7026_sif_WrReg(Address_34ch_SerialCtrl, 0x04);//010-01000: CH3/4 serial ctrl, CH3 Reverse
        }
        else
        {
            motorWT7026_sif_WrReg(Address_34ch_SerialCtrl, 0x08);//010-00100: CH3/4 serial ctrl, CH3 Forward
        }
        //#PIC#2010/11/17#Creator -end
        break;

    case MOTOR_ZOOM_OFF:
        LENS_MSG(("LENS_M: %s MOTOR_ZOOM_OFF\n\r", __func__));
        motorWT7026_sif_WrReg(Address_34ch_SerialCtrl, 0x00);   //010-00000: CH3/4 serial ctrl, CH3 OFF
        break;

    default:
        LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
        break;
    }
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
static BOOL motorWT7026_focus_checkPI(void)
{
    BOOL ret, ret_tmp;

    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return FALSE;
    }
    //LENS_MSG(("LENS_M: motorWT7026_focusPI_check\n\r"));

    ret = gpio_getPin(MOTOR_PINDEF_FOCUS_PI);

    /* check if PI is stable after 1 ms */
    do{
        TimerDelayMs(1);

        ret_tmp = gpio_getPin(MOTOR_PINDEF_FOCUS_PI);

        if(ret == ret_tmp)
            break;
        else
        {
            LENS_ERR(("LENS_E: %s: PI is not same as 1 ms before!!\r\n", __func__));
            ret = ret_tmp;
        }
    } while(1);

    return ret;
}

/**
    motor focus mode action

    Set phase to motor in focus mode

    @param state - The phase state of focus mode.

    @return None.
*/
static void motorWT7026_focus_setState(UINT32 state, UINT32 param1)
{
    UINT32 value = 0xFF;

    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }

    //LENS_MSG(("LENS_M: %s, state=%d param1=%d\n\r", __func__, state, param1));

    /* The "state" is as the output signal you want to make,
       then the "value" is the coresponding register setting of motor driver. */
#if 1
    //CH1/2 Stepping, B4-B3-B2, Normal
    switch(state)
    {
    case 0x09: //1001
        value = 0x10;   //100-00
        break;
    case 0x08: //1000
        value = 0x1C;   //111-00
        break;
    case 0x0A: //1010
        value = 0x18;   //110-00
        break;
    case 0x02: //0010
        value = 0x0C;   //011-00
        break;
    case 0x06: //0110
        value = 0x08;   //010-00
        break;
    case 0x04: //0100
        value = 0x04;   //001-00
        break;
    case 0x05: //0101
        value = 0x00;   //000-00
        break;
    case 0x01: //0001
        value = 0x14;   //101-00
        break;
    default:
        LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
    }

    /* CH1/2 Stepping motor setup: Enable */
    motorWT7026_sif_WrReg(Address_12ch_STM_Enable, 0x10);
#else
    //CH1/2 DC, B4-B3-B2-B1, Normal
    switch(state)
    {
    case 0x0A: //1010
    case 0x09: //1001
    case 0x05: //0101
    case 0x06: //0110
        value = state << 1;
        break;
    default:
        LENS_MSG(("LENS_E: %s,UNKNOW\n\r", __func__));
        return;
    }
#endif

    if(value != 0xFF)
        motorWT7026_sif_WrReg(Address_123ch_CtrlMode, value);
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
static void motorWT7026_aperture_setState(UCHAR state)
{
    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }

    LENS_MSG(("%s: state=%d\r\n", __func__, state));

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN0);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN0);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN1);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN1);
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
static void motorWT7026_shutter_setState(UCHAR state)
{
    if(!gWT7026PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorWT7026_init not ready\n\r"));
        return;
    }

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES0);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES0);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES1);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES1);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with SIF related API.
//--------------------------------------------------------------------------------------------------------------------------
static void motorWT7026_sif_init(void)
{
    SIF_CTRL    SIFCtrl;

    //#NT#2011/07/14#Chris Chung -begin
    //#NT#Since 220 change the pinmux usage, we should mark pinmux code here.
    #if 0
    switch(gWT7026PvtObj.uiSIFChannel)
    {
    case SIF_CH0:
        pinmux_enable_sif(PINMUX_SIF_CH0);
        break;
    case SIF_CH1:
        pinmux_enable_sif(PINMUX_SIF_CH1);
        break;
    case SIF_CH2:
        pinmux_enable_sif(PINMUX_SIF_CH2);
        break;
    case SIF_CH3:
        pinmux_enable_sif(PINMUX_SIF_CH3);
        break;
    default:
        pinmux_enable_sif(PINMUX_SIF_CH0);
        break;
    }
    #endif
    //#NT#2011/07/14#Chris Chung -end

    sif_open();
    // Setup SIF channel parameter
    SIFCtrl.uiMode          = SIF_MODE_0000;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = 0;                // Don't care
    SIFCtrl.uiSenDuration   = 0;                // Don't care
    SIFCtrl.uiLength        = 8;
    SIFCtrl.uiClkDiv        = 4;                // 48/((4+1)*2) = 4.8Mhz
    SIFCtrl.bMSB            = TRUE;
    sif_setControl(gWT7026PvtObj.uiSIFChannel, &SIFCtrl);
}

static void motorWT7026_sif_WrReg(UINT32 addr,UINT32 val)
{
    UINT32 uiSIFData = 0;

    //MSB
    uiSIFData = ((val & 0x1f) << VALUE_SHIFT_OFFSET);       /* 5 bit value */
    uiSIFData |= ((addr & 0x7) << ADDRESS_SHIFT_OFFSET);    /* 3 bit addr  */
    uiSIFData = (uiSIFData << 24);

    if(gWT7026PvtObj.uiSIFType == LENS_SIF_GPIOSIM)
    {
        motorWT7026_gpio_WrReg(uiSIFData);
    }
    else
    {
        sif_sendMSB(gWT7026PvtObj.uiSIFChannel, uiSIFData);
    }
}

#define INREG32(x)      (*((volatile UINT32*)(x)))

static void Delay_1_5_Us(UINT32 count)
{
    UINT32  dlyclk = 0;
    UINT32  dummyRead = 0;

    for(dlyclk = 3*count; dlyclk > 0; dlyclk--)
        dummyRead = INREG32(0x98100000);
}

static void motorWT7026_gpio_WrReg(UINT32 val)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(gWT7026PvtObj.uiGPIOSen , GPIO_DIR_OUTPUT);
    gpio_setDir(gWT7026PvtObj.uiGPIOClk, GPIO_DIR_OUTPUT);
    gpio_setDir(gWT7026PvtObj.uiGPIOData, GPIO_DIR_OUTPUT);

    gpio_clearPin(gWT7026PvtObj.uiGPIOSen);
    gpio_clearPin(gWT7026PvtObj.uiGPIOData);
    gpio_setPin(gWT7026PvtObj.uiGPIOClk);

    data = (val>>24);//(UINT32)addr << 8 | (UINT32)value;

    gpio_setPin(gWT7026PvtObj.uiGPIOSen);
    Delay_1_5_Us(5*MOTOR_RESET_MIN_TIME);

    for(j = 8; j > 0; j--)  // 2010-10-19 Chris
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(gWT7026PvtObj.uiGPIOData);
        else
            gpio_clearPin(gWT7026PvtObj.uiGPIOData);

        Delay_1_5_Us(1);
        gpio_clearPin(gWT7026PvtObj.uiGPIOClk);
        Delay_1_5_Us(1);
        gpio_setPin(gWT7026PvtObj.uiGPIOClk);
        Delay_1_5_Us(1);
    }

    gpio_clearPin(gWT7026PvtObj.uiGPIOSen);
    Delay_1_5_Us(5*MOTOR_RESET_MIN_TIME);
}

static void motorWT7026_gpio_powerOn(void)
{
    if (gWT7026PvtObj.bPowerInverse)
    {
        gpio_clearPin(MOTOR_PINDEF_MOTO_POWER);
    }
    else
    {
        gpio_setPin(MOTOR_PINDEF_MOTO_POWER);
    }
    g_bMotorPowerOn = TRUE;
}

static void motorWT7026_gpio_powerOff(void)
{
    if (gWT7026PvtObj.bPowerInverse)
    {
        gpio_setPin(MOTOR_PINDEF_MOTO_POWER);
    }
    else
    {
        gpio_clearPin(MOTOR_PINDEF_MOTO_POWER);
    }
    g_bMotorPowerOn = FALSE;
}

static BOOL motorWT7026_gpio_isPowerOn(void)
{
    #if 0
    if (gWT7026PvtObj.bPowerInverse)
    {
        return !gpio_getPin(MOTOR_PINDEF_MOTO_POWER);
    }
    else
    {
        return gpio_getPin(MOTOR_PINDEF_MOTO_POWER);
    }
    #else
    return g_bMotorPowerOn;
    #endif
}

static void motorWT7026_gpio_resetOn(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_RESET);
    g_bMotorResetOn = TRUE;
}

static void motorWT7026_gpio_resetOff(void)
{
    gpio_setPin(MOTOR_PINDEF_MOTO_RESET);
    g_bMotorResetOn = FALSE;
}

static BOOL motorWT7026_gpio_isResetOn(void)
{
    #if 0
    return !gpio_getPin(MOTOR_PINDEF_MOTO_RESET);
    #else
    return g_bMotorResetOn;
    #endif
}

