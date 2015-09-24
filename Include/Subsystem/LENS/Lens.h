/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       Lens.h
    @ingroup    mISYSLens

    @brief      Header file of Lens control functions
                Lens control functions header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2011/07/13
*/

#ifndef _LENS_H
#define _LENS_H

#include "Type.h"

/*
    1. Add TOTOL_NUM in enum type.
    2. Use upper case in enum type.
    3. Use "zoom_gotoSection" instead of "zoomgotosection" in function table.
*/

//#NT#2012/07/23#KS Hung -begin
//#NT#expand the status
// lens calibration information
typedef enum
{
    LENS_CAL_STATUS,               // lens calibration status
    LENS_CAL_FAR_FOCUS,            // lens far focus step
    LENS_CAL_NEAR_FOCUS,           // lens near focus step
    LENS_CAL_ZOOM_BL,              // lens zoom backlash
    LENS_CAL_FAR_ADJUST_STEP,  // lens far focus compensation or not
    LENS_CAL_NEAR_ADJUST_STEP, // lens near focus compensation or not
    ENUM_DUMMY4WORD(LENS_CAL_INFO)
} LENS_CAL_INFO;
//#NT#2012/07/23#KS Hung -end

typedef INT32 (*FPLENSZOOMCAF)(void);
typedef INT32 (*FP_LENS_CAL_CB)(LENS_CAL_INFO CalInfo, INT32 iParam1, INT32 iParam2);

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for overall Lens Module related definition.
//--------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    LENS_IO_MODE_1,
    LENS_IO_MODE_2,
    LENS_IO_MODE_3,
    LENS_IO_IN_0,
    LENS_IO_IN_1,
    LENS_IO_IN_2,
    LENS_IO_IN_3,
    LENS_IO_IN_4,
    LENS_IO_IN_5,
    LENS_IO_PWR,
    LENS_IO_ZOOM_PI,
    LENS_IO_ZOOM_PR,
    LENS_IO_FOCUS_PI,
    LENS_IO_MOT_EXT1,
    LENS_IO_MOT_EXT2,
    LENS_IO_MOT_MOB1,
    LENS_IO_MOT_MOB2,
    //#NT#2011/10/03#Hideo Lin -begin
    //#NT#Marked, since it may use ADC key
    //LENS_IO_WIDE_KEY,
    //LENS_IO_TELE_KEY,
    //#NT#2011/10/03#Hideo Lin -end
    LENS_IO_PWR_INVERSE,
    LENS_IO_MOTOR_INVERSE,
    LENS_IO_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_IO_MAP)
} LENS_MODULE_IO_MAP;

typedef enum
{
    LENS_PAD_ZOOM_PI,
    LENS_PAD_ZOOM_PR,
    LENS_PAD_FOCUS_PI,
    LENS_PAD_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_PAD_MAP)
} LENS_MODULE_PAD_MAP;

typedef enum
{
    LENS_MOTOR_PARAM_IRIS_VOLTAGE,
    LENS_MOTOR_PARAM_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_MOTOR_PARAM_MAP)
} LENS_MODULE_MOTOR_PARAM_MAP;

typedef enum
{
    LENS_AFSTOP = 0,
    LENS_AFMOVE,
    ENUM_DUMMY4WORD(LENS_AFSTATUS)
}LENS_AFSTATUS;

typedef enum
{
    LENS_NONE,
    LENS_INITOK,
    LENS_IDLE,
    LENS_ERROR,
    LENS_ZOOM_INITPART1,
    LENS_ZOOMMOVING,
    LENS_FOCUSMOVING,
    LENS_IRISMOVING,
    LENS_SHUTTERMOVING,
    LENS_RETRACTING,
    ENUM_DUMMY4WORD(LENS_MODULE_STATE)
} LENS_MODULE_STATE;

typedef enum
{
    LENS_SIF_CONTROLLER,
    LENS_SIF_GPIOSIM,
    ENUM_DUMMY4WORD(LENS_SIF_TYPE)
} LENS_SIF_TYPE;

typedef enum
{
    MOTOR_CTRL_GPIO = 0x01,
    MOTOR_CTRL_SIF  = 0x02,
    MOTOR_CTRL_I2C  = 0x04,
    MOTOR_CTRL_RESV = 0x08,
    ENUM_DUMMY4WORD(MOTOR_CTRL)
} MOTOR_CTRL;

typedef struct _LENS_SIF_PINDEF
{
    LENS_SIF_TYPE   uiSIFType;                  ///< SIF use SIF controller or GPIO simulation
    UINT32          uiSIFChannel;               ///< SIF channel SIF_CH0 ~ 3, refer to SIF.h, valid in LCD_CTRL_MODE_SIF
    UINT32          uiGPIOSen;                  ///< GPIO pin number for SEN, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32          uiGPIOClk;                  ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32          uiGPIOData;                 ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO
} LENS_SIF_PINDEF, *PLENS_SIF_PINDEF;

typedef enum
{
    LENS_I2C_CONTROLLER,
    LENS_I2C_GPIOSIM,
    ENUM_DUMMY4WORD(LENS_I2C_TYPE)
} LENS_I2C_TYPE;

typedef struct _LENS_I2C_PINDEF
{
    LENS_I2C_TYPE   uiI2CType;                  ///< I2C use I2C controller or GPIO simulation
    UINT32          uiDeviceAddress;            ///< I2C device address
    UINT32          uiGPIOClk;                  ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32          uiGPIOData;                 ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO
} LENS_I2C_PINDEF, *PLENS_I2C_PINDEF;

/**
    Lens device object.
*/
typedef struct _LENS_DEVICE_OBJ
{
    MOTOR_CTRL          uiMotorCtrlMode;        ///< Motor control interface mode
    LENS_SIF_PINDEF     LensSIFPinObj;          ///< Lens SIF motor control pin definition data
    LENS_I2C_PINDEF     LensI2CPinObj;          ///< Lens I2C motor control pin definition data
    UINT32              uiMotorCtrlGPIOIntrID;  ///< GPIO interrupt ID for zoom PI
    UINT32              uiMotorCtrlGPIOIntrID2; ///< GPIO interrupt ID for focus PI
    UINT32              *puiMotorCtrlGPIOArray; ///< GPIO pin array for HW connection of DSP, motor driver and lens
    UINT32              *puiMotorCtrlPADArray;  ///< PAD pin array for voltage pull
    UINT32              *puiMotorCtrlParamArray;///< Motor parameter array for Motor info control
    UINT32              *puiLensCtrlParamArray; ///< Lens parameter array for Lens info control
    //#NT#2011/10/03#Hideo Lin -begin
    //#NT#Add lens calibration information callback
    FP_LENS_CAL_CB      fpLensCalCB;            ///< Lens calibration information
    //#NT#2011/10/03#Hideo Lin -end
} LENS_DEVICE_OBJ, *PLENS_DEVICE_OBJ;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Motor Driver related definition.
//--------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    MOTOR_POWER_OFF,
    MOTOR_POWER_ON,
    MOTOR_POWER_MAX,
    ENUM_DUMMY4WORD(MOTOR_POWER)
} MOTOR_POWER;

typedef enum
{
    MOTOR_POWERON_NORMAL,
    MOTOR_POWERON_PI1_ONLY,
    MOTOR_POWERON_PI2_ONLY,
    MOTOR_POWERON_NO_PI,
    MOTOR_POWERON_MAX,
    ENUM_DUMMY4WORD(MOTOR_POWERON_PARAM)
} MOTOR_POWERON_PARAM;

typedef enum
{
    MOTOR_MODE_STANDBY,
    MOTOR_MODE_ZOOM,
    MOTOR_MODE_FOCUS,
    MOTOR_MODE_APERTURE,
    MOTOR_MODE_SHUTTER,
    MOTOR_MODE_FIND_HOME,
    ENUM_DUMMY4WORD(MOTOR_MODE)
} MOTOR_MODE;

typedef enum
{
    MOTOR_ZOOM_BRAKE,
    MOTOR_ZOOM_FWD,
    MOTOR_ZOOM_BWD,
    MOTOR_ZOOM_OFF,
    ENUM_DUMMY4WORD(MOTOR_ZOOM_ACT)
} MOTOR_ZOOM_ACT;


typedef enum
{
    MOTOR_IRIS_VOLTAGE_30V,
    MOTOR_IRIS_VOLTAGE_32V,
    MOTOR_IRIS_VOLTAGE_34V,
    MOTOR_IRIS_VOLTAGE_36V,
    MOTOR_IRIS_VOLTAGE_38V,
    MOTOR_IRIS_VOLTAGE_40V,
    MOTOR_IRIS_VOLTAGE_42V,
    MOTOR_IRIS_VOLTAGE_44V,
    MOTOR_IRIS_VOLTAGE_46V,
    MOTOR_IRIS_VOLTAGE_48V,
    ENUM_DUMMY4WORD(MOTOR_IRIS_VOLTAGE)
} MOTOR_IRIS_VOLTAGE;

typedef enum
{
    MOTOR_FOCUS_FWD,
    MOTOR_FOCUS_BWD,
    ENUM_DUMMY4WORD(MOTOR_FOCUS_ACT)
} MOTOR_FOCUS_ACT;

/**
    Motor Info object.
    Some motor information can be transfered via motor info object.
*/
typedef struct _MOTOR_INFO_TAB
{
    //PWM
    BOOL          bPwmCtrl;
    UINT32        uiPwmPeriodTime;
    UINT32        uiPwmOnTime;

    //Zoom
    BOOL          bWaitZoomFinish;
    BOOL          bOpenZoomPR;
    UINT32        uiZoomPPS;
    UINT32        uiZoomDirection;
    UINT32        uiZoomStep;

    //Focus
    UINT32        uiFocusPPS;
    UINT32        uiFocusDirection;
    UINT32        uiFocusStep;
} MOTOR_INFO_TAB, *PMOTOR_INFO_TAB;

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_TAB
{
    //common related cb
    void          (*init)(PLENS_DEVICE_OBJ);
    void          (*power)(MOTOR_POWER, UINT32);
    void          (*changeMode)(MOTOR_MODE);//mode type
    void          (*setMotorInfo)(PMOTOR_INFO_TAB);

    //zoom related cb
    void          (*zoom_setState)(MOTOR_ZOOM_ACT, UINT32);//zoom action
    void          (*zoom_setPIInt)(BOOL);//zoom PI int set
    void          (*zoom_clearPIInt)(void);//zoom PI int clear
    BOOL          (*zoom_checkPIInt)(void);//zoom PI int check
    BOOL          (*zoom_checkPR)(void);//zoom PR check
    BOOL          (*zoom_checkPI)(void);//zoom PR check

    //focus related cb
    BOOL          (*focus_checkPI)(void);//focus PI check
    void          (*focus_setState)(UINT32, UINT32);//focus action

    //aperture related cb
    void          (*aperture_setState)(UCHAR);//aperture action

    //shutter related cb
    void          (*shutter_setState)(UCHAR);//shutter action

    //other cb
    void          (*sif_doZoomFocusMove)(void);
} MOTOR_TAB, *PMOTOR_TAB;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Lens Driver related definition.
//--------------------------------------------------------------------------------------------------------------------------
/**
    Category of lens initiation
*/
typedef enum
{
    LENS_INIT_ZOOM_PART1,           ///< Category of zoom part1 initiation
    LENS_INIT_ZOOM_PART2,           ///< Category of zoom part2 initiation
    LENS_INIT_FOCUS,                ///< Category of focus initiation
    ENUM_DUMMY4WORD(LENS_INIT_CATEGORY)
} LENS_INIT_CATEGORY;

typedef enum
{
    LENS_ZOOM_IN,
    LENS_ZOOM_OUT,
    ENUM_DUMMY4WORD(LENS_ZOOM_ACT)
} LENS_ZOOM_ACT;

typedef enum
{
    LENS_APERTURE_BIG,
    LENS_APERTURE_SMALL,
    LENS_APERTURE_OFF,
    ENUM_DUMMY4WORD(LENS_APERTURE_ACT)
} LENS_APERTURE_ACT;

typedef enum
{
    LENS_SHUTTER_CLOSE,
    LENS_SHUTTER_OPEN,
    LENS_SHUTTER_OFF,
    LENS_SHUTTER_TOTAL_NUM,
    ENUM_DUMMY4WORD(LENS_SHUTTER_ACT)
} LENS_SHUTTER_ACT;

typedef enum
{
    FOCUS_PREEXC   = 0x1,      //Focus with pre-excitation
    FOCUS_RUN      = 0x2,      //Focus with no-excitation
    FOCUS_POSTEXC  = 0x4,      //Focus with post-excitation
    FOCUS_FORWARD  = 0x8,      //add for debug
    FOCUS_BACKWORD = 0x10,
    ENUM_DUMMY4WORD(FOCUS_CATEGORY)
} FOCUS_CATEGORY;

typedef struct _LENS_TAB
{
    //Lens
    void                (*init)(PLENS_DEVICE_OBJ, PMOTOR_TAB);
    BOOL                (*isFFLens)(void);
    //Zoom
    INT32               (*zoom_initPart1)(void);
    INT32               (*zoom_initPart2)(void);
    INT32               (*zoom_ctrlRelease)(void);
    INT32               (*zoom_ctrlPress)(LENS_ZOOM_ACT);
    INT32               (*zoom_ctrlPress_inner)(LENS_ZOOM_ACT);
    INT16               (*zoom_getCurrentPosition)(void);
    UINT8               (*zoom_getCurrentSection)(void);
    UINT8               (*zoom_getMaxSection)(void);
    INT32               (*zoom_getRatio)(UINT32 section);
    INT32               (*zoom_retract)(UINT32);
    void                (*zoom_gotoSection)(UINT8);
    void                (*zoom_setAutoBoundaryCheck)(BOOL);
    void                (*zoomIsr_enable)(BOOL);
    UINT32              (*zoom_getBL)(void);
    void                (*zoom_setBL)(UINT32);
    void                (*zoom_doBL)(void);
    //Focus
    void                (*focus_gotoPosition)(FOCUS_CATEGORY, INT32);
    INT32               (*focus_gotoHome)(void);
    INT32               (*focus_gotoGarage)(void);
    INT16               (*focus_getCurrentPosition)(void);
    INT16               (*focus_getMaxPosition)(void);
    INT16               (*focus_getMinPosition)(void);
    UINT32              (*focus_getFL)(UINT32);
    INT16               (*focus_getZoomTblValue)(UINT32, UINT32);
    UINT16              (*focus_getZoomTblMaxIdx)(void);
    UINT16              (*focus_getCurrentDist)(INT32);
    UINT16              (*focus_getDiffStepByDist)(UINT32 dist);
    UINT32              (*focus_getBL)(void);
    void                (*focus_setBL)(UINT32);
    void                (*focus_setBasisStep)(UINT32, INT32);
    //Aperture
    INT32               (*aperture_gotoState)(LENS_APERTURE_ACT);
    LENS_APERTURE_ACT   (*aperture_getState)(void);
    UINT32              (*aperture_getFNO)(LENS_APERTURE_ACT, UINT32);
    //Shutter
    INT32               (*shutter_gotoState)(LENS_SHUTTER_ACT);
    UINT32              (*shutter_getPulsePeriod)(void);
    //Others
    PMOTOR_TAB          pMotor;
} LENS_TAB, *PLENS_TAB;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for other old definition.
//--------------------------------------------------------------------------------------------------------------------------
#define ERR_OK_TIMER_CLOSE_SH   1   //Operation is OK and indicate caller that let Timer do close shutter.
#define ERR_OK                  0
#define ERR_OPERATION_FAIL      (-4)
#define OPEN                    1
#define CLOSE                   0

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for function declaration.
//--------------------------------------------------------------------------------------------------------------------------
//Lens Command
extern void                 Lens_Cmd(CHAR *pcCmdStr);

//Lens API: Lens
extern void                 Lens_Module_Init(PLENS_DEVICE_OBJ pLensDev, PLENS_TAB pLensTab, PMOTOR_TAB pMotorTab);
extern UINT32               Lens_Module_GetState(void);
extern void                 Lens_OnOff(MOTOR_POWER ON_OFF, UINT32 param1);
extern INT32                Lens_Init(LENS_INIT_CATEGORY category);
//#NT#2011/11/23#KS Hung -begin
//Use lens calibration callback instead of UI calibration function
extern FP_LENS_CAL_CB       Lens_GetCalInfo(void);
//#NT#2011/11/23#KS Hung -end
extern INT32                Lens_Retract(UINT32 param);

//Lens API: Zoom
extern INT16                Lens_Zoom_GetCurrentPosition(void);
extern INT32                Lens_Zoom_GetRatio(UINT32 section);
extern INT32                Lens_Zoom_GetSection(void);
extern INT32                Lens_Zoom_GetMaxSection(void);
extern INT32                Lens_Zoom_Goto(UINT8 section);
extern INT32                Lens_Zoom_Goto2(MOTOR_POWER powerMode, UINT8 section);
extern INT32                Lens_Zoom_In(void);
extern INT32                Lens_Zoom_In2(MOTOR_POWER powerMode);
extern INT32                Lens_Zoom_Out(void);
extern INT32                Lens_Zoom_Out2(MOTOR_POWER powerMode);
extern INT32                Lens_Zoom_Stop(void);

extern void                 Lens_Zoom_EnableIsr(BOOL enable);
extern UINT32               Lens_Zoom_GetBL(void);
extern void                 Lens_Zoom_SetBL(UINT32 BLstep);
extern INT32                Lens_Zoom_DoBL(MOTOR_POWER powerMode);
extern void                 Lens_Zoom_SetPPS(UINT32 pps);

//Lens API: Focus
extern INT32                Lens_Focus_DoAction(FOCUS_CATEGORY category, INT32 position);
extern INT32                Lens_Focus_DoAction2(MOTOR_POWER powerMode, FOCUS_CATEGORY category, INT32 position);
extern INT32                Lens_Focus_Goto(INT32 position);
extern INT32                Lens_Focus_Goto2(MOTOR_POWER powerMode, INT16 position);
extern INT32                Lens_Focus_GotoHome(void);
extern INT32                Lens_Focus_GetCurrentPosition(void);
extern INT32                Lens_Focus_GetMaxPosition(void);
extern INT32                Lens_Focus_GetMinPosition(void);
extern INT32                Lens_Focus_GetFL(UINT32 zoomSection);
extern INT16                Lens_Focus_GetZoomTableValue(UINT32 zoomSection, UINT32 index);
extern INT16                Lens_Focus_GetZoomTableMaxIdx(void);
extern INT32                Lens_Focus_GetCurrentDist(INT32 basicStep);
extern INT32                Lens_Focus_GetDiffStepByDist(UINT32 dist);
extern void                 Lens_Focus_SetPPS(UINT32 pps);
extern UINT32               Lens_Focus_GetBL(void);
extern void                 Lens_Focus_SetBL(UINT32 BLstep);
extern void                 Lens_Focus_SetBasisStep(UINT32 section, INT32 farStep);
extern void                 Lens_Focus_SetCurrentVA(UINT32 ui32currentVA);
extern UINT32               Lens_Focus_GetCurrentVA(void);

//Lens API: Aperture
extern INT32                Lens_Aperture_Move(LENS_APERTURE_ACT act);
extern INT32                Lens_Aperture_GetPosition(void);
extern INT32                Lens_Aperture_GetFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);

//Lens API: Shutter
extern INT32                Lens_Shutter_Move(LENS_SHUTTER_ACT act, BOOL bAutoClose);

//Driver: Motor
extern PMOTOR_TAB           motor_getObject(void);

//Driver: Lens
extern PLENS_TAB            lens_getObject(void);

#endif
