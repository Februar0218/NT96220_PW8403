/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensAPI.c
    @ingroup    mISYSLens

    @brief      Lens API functions

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

/*
    Todo: Justin
    1. Define device object.
    2. Use the functions in lens function table instead of original hard coding functions.
    3. Implement power on/off and mode enable/disable here.
*/

#include <string.h>
#include "OSCommon.h"
#include "GPIO.h"
#include "Lens_int.h"
#include "LensAPI.h"
#include "Timer.h"

static LENS_MODULE_OBJ gLensModuleObj =
{
    LENS_NONE,                      ///< The current state of lens module
    NULL,                           ///< The function table of motor driver
    NULL                            ///< The function table of lens driver
};

//#NT#2011/11/23#KS Hung -begin
//Use lens calibration callback instead of UI calibration function
static FP_LENS_CAL_CB   g_fpLensCalCB = NULL;
//#NT#2011/11/23#KS Hung -end

//#NT#2012/05/29#KS Hung -begin
static UINT32 g_ui32CurrentVA=0;
//#NT#2012/05/29#KS Hung -end

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

UINT32 Lens_Module_GetState(void)
{
    return gLensModuleObj.uiLensModuleState;
}

static void Lens_Module_SetState(LENS_MODULE_STATE state)
{
    LENS_MSG(("LENS_M: %s state=%d\r\n", __func__, state));
    loc_cpu();  // enter critical section
    gLensModuleObj.uiLensModuleState = state;
    unl_cpu();  // exit critical section
}

void Lens_Module_Init(PLENS_DEVICE_OBJ pLensDev, PLENS_TAB pLensTab, PMOTOR_TAB pMotorTab)
{
    LENSCTRL_APPOBJ LensCtrlObj;
    LENS_ERR(("LENS_M: Lens_Module_Init\n\r"));

    if(pLensDev == NULL||
       pLensTab == NULL||
       pMotorTab == NULL)
    {
        LENS_ERR(("LENS_E: %s param error!!\r\n", __func__));
        return;
    }

    //init module object
    Lens_Module_SetState(LENS_INITOK);
    gLensModuleObj.pLensObj = pLensTab;
    gLensModuleObj.pMotorObj = pMotorTab;

    //init motor driver
    gLensModuleObj.pMotorObj->init(pLensDev);

    //init lens driver
    gLensModuleObj.pLensObj->init(pLensDev, gLensModuleObj.pMotorObj);

    //#NT#2011/11/23#KS Hung -begin
    //Calibration information callback
    if (pLensDev->fpLensCalCB)
    {
         g_fpLensCalCB = pLensDev->fpLensCalCB;
    }
    //#NT#2011/11/23#KS Hung -end

    if(gLensModuleObj.pLensObj->isFFLens())
    {
        Lens_Module_SetState(LENS_IDLE);
    }

    //init lens control task
    LensCtrl_Close();
    memset(&LensCtrlObj, 0, sizeof(LENSCTRL_APPOBJ));
    LensCtrlObj.pLens = pLensTab;
    LensCtrlObj.pMotor = pMotorTab;
    //#NT#2011/10/03#Hideo Lin -begin
    //#NT#Marked, since it may use ADC key
    //LensCtrlObj.uiWideKey = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_WIDE_KEY];
    //LensCtrlObj.uiTeleKey = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_TELE_KEY];
    //#NT#2011/10/03#Hideo Lin -end
    LensCtrl_Open(&LensCtrlObj);

    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);

    return;
}

void Lens_OnOff(MOTOR_POWER ON_OFF, UINT32 param1)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return;

    gLensModuleObj.pMotorObj->power(ON_OFF, param1);
}

INT32 Lens_Init(LENS_INIT_CATEGORY category)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    switch(category)
    {
    case LENS_INIT_ZOOM_PART1:

        if(gLensModuleObj.uiLensModuleState == LENS_ZOOM_INITPART1)
            return ERR_OK;

        //#NT#2011/09/23#Hideo Lin -begin
        //#NT#For internal power control (on)
        Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
        //#NT#2011/09/23#Hideo Lin -end

        //1. Change Lens State
        Lens_Module_SetState(LENS_ZOOM_INITPART1);

        //2. Set Zoom Mode
        gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FIND_HOME);

        //3. Init Zoom Part1
        gLensModuleObj.pLensObj->zoom_initPart1();

        return ERR_OK;

    case LENS_INIT_ZOOM_PART2:

        if(gLensModuleObj.pLensObj->zoom_initPart2() == ERR_OK)
        {
            //2.1.1 Set Aperture Mode
            //gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_APERTURE);

            //2.1.2 Set Big Aperture
            //gLensModuleObj.pLensObj->aperture_gotoState(LENS_APERTURE_BIG);
            //TimerDelayMs(30);
            //gLensModuleObj.pLensObj->aperture_gotoState(LENS_APERTURE_OFF);

            //2.1.3 Set Shutter Mode
            gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_SHUTTER);

            //2.1.4 Open Shutter
            gLensModuleObj.pLensObj->shutter_gotoState(LENS_SHUTTER_OPEN);
            TimerDelayMs(30);
            gLensModuleObj.pLensObj->shutter_gotoState(LENS_SHUTTER_OFF);

            //2.1.5 Set Focus Mode
            gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);

            //2.1.6 Focus to Home
            gLensModuleObj.pLensObj ->focus_gotoHome();

            //#NT#2011/09/23#Hideo Lin -begin
            //#NT#Should go back to idle state!!!
            Lens_Module_SetState(LENS_IDLE);
            //#NT#2011/09/23#Hideo Lin -end

            //#NT#2011/09/23#Hideo Lin -begin
            //#NT#For internal power control (off)
            Lens_OnOff(MOTOR_POWER_OFF, 0);
            //#NT#2011/09/23#Hideo Lin -end

            return ERR_OK;
        }
        else
        {
            LENS_ERR(("LENS_E: %s init part2 error!!\r\n", __func__));

            Lens_Module_SetState(LENS_ERROR);

            //#NT#2011/09/23#Hideo Lin -begin
            //#NT#For internal power control (off)
            Lens_OnOff(MOTOR_POWER_OFF, 0);
            //#NT#2011/09/23#Hideo Lin -end

            return ERR_OPERATION_FAIL;
        }
        break;

    case LENS_INIT_FOCUS:

        //#NT#2011/09/23#Hideo Lin -begin
        //#NT#For internal power control (on)
        //Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NO_PI);
        Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
        //#NT#2011/09/23#Hideo Lin -end

        // Focus to Home
        gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FIND_HOME);
        gLensModuleObj.pLensObj->focus_gotoHome();
        Lens_Module_SetState(LENS_IDLE);

        //#NT#2011/09/23#Hideo Lin -begin
        //#NT#For internal power control (off)
        //Lens_OnOff(MOTOR_POWER_OFF, 0);
        //#NT#2011/09/23#Hideo Lin -end
        break;

    default:
        LENS_ERR(("LENS_E: %s: init wrong state = %d\r\n", __func__, category));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

//#NT#2011/11/23#KS Hung -begin
//Use lens calibration callback instead of UI calibration function
FP_LENS_CAL_CB Lens_GetCalInfo(void)
{
    return g_fpLensCalCB;
}
//#NT#2011/11/23#KS Hung -end

INT32 Lens_Retract(UINT32 param)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    if(gLensModuleObj.pLensObj->isFFLens())
        return ERR_OK;

    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_RETRACTING:
        return ERR_OK;
    default:
        break;
    }

    LENS_ERR(("LENS_M: Lens Retracting...\r\n"));

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_RETRACTING);

    //1. Retract Zoom
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FIND_HOME);
    gLensModuleObj.pLensObj->zoom_retract(param);

    Lens_Module_SetState(LENS_INITOK);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return ERR_OK;
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

INT16 Lens_Zoom_GetCurrentPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->zoom_getCurrentPosition();
}

INT32 Lens_Zoom_GetRatio(UINT32 section)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->zoom_getRatio(section);
}

INT32 Lens_Zoom_GetSection(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->zoom_getCurrentSection();
}

INT32 Lens_Zoom_GetMaxSection(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->zoom_getMaxSection();
}

INT32 Lens_Zoom_Goto(UINT8 section)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_INITOK:
    case LENS_IDLE:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_Goto section=%d\r\n", section));

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_ZOOMMOVING);

    //1. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //2. Set Zoom In
    gLensModuleObj.pLensObj->zoom_gotoSection(section);

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    //Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return ERR_OK;
}

//#NT#2012/06/01#KS Hung -begin
INT32 Lens_Zoom_Goto2(MOTOR_POWER powerMode, UINT8 section)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_INITOK:
    case LENS_IDLE:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_Goto section=%d\r\n", section));

    Lens_OnOff(MOTOR_POWER_ON, powerMode);

    Lens_Module_SetState(LENS_ZOOMMOVING);

    //1. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //2. Set Zoom In
    gLensModuleObj.pLensObj->zoom_gotoSection(section);

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    //Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return ERR_OK;
}
//#NT#2012/06/01#KS Hung -end

INT32 Lens_Zoom_In(void)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
        break;
    case LENS_ZOOMMOVING:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_In\n\r"));

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_ZOOMMOVING);

    //1. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //2. Set Zoom In
    gLensModuleObj.pLensObj->zoom_ctrlPress(LENS_ZOOM_IN);

    return ERR_OK;
}

//#NT#2012/06/01#KS Hung -begin
INT32 Lens_Zoom_In2(MOTOR_POWER powerMode)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
        break;
    case LENS_ZOOMMOVING:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_In2\n\r"));

    Lens_OnOff(MOTOR_POWER_ON, powerMode);

    Lens_Module_SetState(LENS_ZOOMMOVING);

    //1. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //2. Set Zoom In
    gLensModuleObj.pLensObj->zoom_ctrlPress(LENS_ZOOM_IN);

    return ERR_OK;
}

//#NT#2012/06/01#KS Hung -end

INT32 Lens_Zoom_Out(void)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
        break;
    case LENS_ZOOMMOVING:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_Out\n\r"));

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_ZOOMMOVING);

    //1. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //2. Set Zoom Out
    gLensModuleObj.pLensObj->zoom_ctrlPress(LENS_ZOOM_OUT);

    return ERR_OK;
}

//#NT#2012/06/01#KS Hung -begin
INT32 Lens_Zoom_Out2(MOTOR_POWER powerMode)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
        break;
    case LENS_ZOOMMOVING:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_Out2\n\r"));

    Lens_OnOff(MOTOR_POWER_ON, powerMode);

    Lens_Module_SetState(LENS_ZOOMMOVING);

    //1. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //2. Set Zoom Out
    gLensModuleObj.pLensObj->zoom_ctrlPress(LENS_ZOOM_OUT);

    return ERR_OK;
}
//#NT#2012/06/01#KS Hung -end

INT32 Lens_Zoom_Stop(void)
{
    INT32 Status;

    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
        return ERR_OK;
    case LENS_ZOOMMOVING:
    case LENS_FOCUSMOVING:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_Stop\n\r"));

    Status = gLensModuleObj.pLensObj->zoom_ctrlRelease();

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    if((Status > gLensModuleObj.pLensObj->zoom_getMaxSection()) || Status <= 0)
    {
        return ERR_OPERATION_FAIL;
    }
    else
    {
        return Status;
    }
}

void Lens_Zoom_EnableIsr(BOOL enable)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return;

    gLensModuleObj.pLensObj->zoomIsr_enable(enable);
}

UINT32 Lens_Zoom_GetBL(void)
{
    UINT32 retValue;

    if(gLensModuleObj.uiLensModuleState == LENS_INITOK)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    //1. Focus to garage
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);
    gLensModuleObj.pLensObj ->focus_gotoGarage();

    //2. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //3. Get Zoom BL value
    retValue = gLensModuleObj.pLensObj->zoom_getBL();

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return retValue;
}

void Lens_Zoom_SetBL(UINT32 BLstep)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return;

    gLensModuleObj.pLensObj->zoom_setBL(BLstep);
}

void Lens_Zoom_SetPPS(UINT32 pps)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    LensCtrl_GetParam(&lensCtrlParamObj);

    lensCtrlParamObj.uiZoomPPS = pps;

    LensCtrl_SetParam(&lensCtrlParamObj);
}

INT32 Lens_Zoom_DoBL(MOTOR_POWER powerMode)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: %s\r\n", __func__));

    Lens_Module_SetState(LENS_ZOOMMOVING);

    //1. Power ON
    Lens_OnOff(MOTOR_POWER_ON, powerMode);

    //2. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //3. Set Zoom In
    gLensModuleObj.pLensObj->zoom_doBL();

    //4. Power OFF
    Lens_OnOff(MOTOR_POWER_OFF, 0);

    Lens_Module_SetState(LENS_IDLE);

    return ERR_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 Lens_Focus_DoAction(FOCUS_CATEGORY category, INT32 position)
{
    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    if(position > gLensModuleObj.pLensObj->focus_getMaxPosition())
        return ERR_OPERATION_FAIL;

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    //Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NO_PI);
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_FOCUSMOVING);

    //1. Set Focus Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);

    //2. Set Focus Go2
    gLensModuleObj.pLensObj->focus_gotoPosition(category, position);

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    //Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return ERR_OK;
}

//#NT#2012/06/01#KS Hung -begin
INT32 Lens_Focus_DoAction2(MOTOR_POWER powerMode, FOCUS_CATEGORY category, INT32 position)
{
    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    if(position > gLensModuleObj.pLensObj->focus_getMaxPosition())
        return ERR_OPERATION_FAIL;

    Lens_OnOff(MOTOR_POWER_ON, powerMode);

    Lens_Module_SetState(LENS_FOCUSMOVING);

    //1. Set Focus Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);

    //2. Set Focus Go2
    gLensModuleObj.pLensObj->focus_gotoPosition(category, position);

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    //Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return ERR_OK;
}
//#NT#2012/06/01#KS Hung -end

INT32 Lens_Focus_Goto(INT32 position)
{
    return Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, position);
}

INT32 Lens_Focus_Goto2(MOTOR_POWER powerMode, INT16 position)
{
    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    if(position > gLensModuleObj.pLensObj->focus_getMaxPosition())
        return ERR_OPERATION_FAIL;

    Lens_Module_SetState(LENS_FOCUSMOVING);

    //1. Power ON
    Lens_OnOff(MOTOR_POWER_ON, powerMode);

    //2. Set Focus Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);

    //3. Set Focus Go2
    gLensModuleObj.pLensObj->focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC,position);

    Lens_Module_SetState(LENS_IDLE);

    return ERR_OK;
}

INT32 Lens_Focus_GotoHome(void)
{
    INT32 status;

    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    //Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NO_PI);
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_FOCUSMOVING);

    //1. Set Focus Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FIND_HOME);

    //2. Set Focus Home
    status = gLensModuleObj.pLensObj ->focus_gotoHome();

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    //Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return status;
}

INT32 Lens_Focus_GetCurrentPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getCurrentPosition();
}

INT32 Lens_Focus_GetMaxPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getMaxPosition();
}

INT32 Lens_Focus_GetMinPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getMinPosition();
}

INT32 Lens_Focus_GetFL(UINT32 zoomSection)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getFL(zoomSection);
}

INT16 Lens_Focus_GetZoomTableValue(UINT32 zoomSection, UINT32 index)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getZoomTblValue(zoomSection, index);
}

INT16 Lens_Focus_GetZoomTableMaxIdx(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getZoomTblMaxIdx();
}

INT32 Lens_Focus_GetCurrentDist(INT32 basicStep)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getCurrentDist(basicStep);
}

INT32 Lens_Focus_GetDiffStepByDist(UINT32 dist)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getDiffStepByDist(dist);
}

void Lens_Focus_SetPPS(UINT32 pps)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    LensCtrl_GetParam(&lensCtrlParamObj);

    lensCtrlParamObj.uiFocusPPS = pps;

    LensCtrl_SetParam(&lensCtrlParamObj);
}

UINT32 Lens_Focus_GetBL(void)
{
    UINT32 retValue;

    if(gLensModuleObj.uiLensModuleState == LENS_INITOK)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    //Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NO_PI);
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_FOCUSMOVING);

    //1. Set Focus Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FIND_HOME);

    //2. Get Focus BL value
    retValue = gLensModuleObj.pLensObj->focus_getBL();

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return retValue;
}

void Lens_Focus_SetBL(UINT32 BLstep)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return;

    gLensModuleObj.pLensObj->focus_setBL(BLstep);
}

void Lens_Focus_SetBasisStep(UINT32 section, INT32 farStep)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return;

    gLensModuleObj.pLensObj->focus_setBasisStep(section, farStep);
}

//#NT#2012/05/29#KS Hung -begin
void Lens_Focus_SetCurrentVA(UINT32 ui32currentVA)
{
    g_ui32CurrentVA = ui32currentVA;
}

UINT32 Lens_Focus_GetCurrentVA(void)
{
    return g_ui32CurrentVA;
}
//#NT#2012/05/29#KS Hung -end

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 Lens_Aperture_Move(LENS_APERTURE_ACT act)
{
    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
        return ERR_OPERATION_FAIL;

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NO_PI);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_IRISMOVING);

    //1. Set Aperture Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_APERTURE);

    //2. Move Aperture
    gLensModuleObj.pLensObj->aperture_gotoState(act);
    TimerDelayMs(gLensModuleObj.pLensObj->shutter_getPulsePeriod());
    gLensModuleObj.pLensObj->aperture_gotoState(LENS_APERTURE_OFF);

    Lens_Module_SetState(LENS_IDLE);

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (off)
    Lens_OnOff(MOTOR_POWER_OFF, 0);
    //#NT#2011/09/23#Hideo Lin -end

    return ERR_OK;
}

INT32 Lens_Aperture_GetPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->aperture_getState();
}

INT32 Lens_Aperture_GetFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->aperture_getFNO(irisPos, zoomSection);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

static void Lens_Shutter_AutoClose_Hdl(void)
{
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_SHUTTER);
    gLensModuleObj.pLensObj->shutter_gotoState(LENS_SHUTTER_OFF);
}

static void Lens_Shutter_AutoClose(UINT32 waitMS)
{
    UINT TimerID;

    if( timer_openAutoClose(&TimerID, Lens_Shutter_AutoClose_Hdl) == E_OK)
    {
        //clr_flg(FLG_ID_INT, (FLGINT2_TIMER0<<TimerID));
        timer_set(TimerID, waitMS, _TIMER_CTRL_ONE_SHOT|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
    }
    else
    {
        set_flg(FLG_ID_LENS, FLGLENS_SHUTTEROFF);
    }
}

INT32 Lens_Shutter_Move(LENS_SHUTTER_ACT act, BOOL bAutoClose)
{
    INT32 status;

    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
        return ERR_OPERATION_FAIL;

    //#NT#2011/09/23#Hideo Lin -begin
    //#NT#For internal power control (on)
    Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NO_PI);
    //#NT#2011/09/23#Hideo Lin -end

    Lens_Module_SetState(LENS_SHUTTERMOVING);

    //1. Set Shutter Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_SHUTTER);

    //2. Open/Close Shutter
    status = gLensModuleObj.pLensObj->shutter_gotoState(act);

    //3. Use timer to set shutter off
    if(status != ERR_OK_TIMER_CLOSE_SH)
    {
        if(bAutoClose)
            Lens_Shutter_AutoClose(gLensModuleObj.pLensObj->shutter_getPulsePeriod());
        else
        {
            TimerDelayMs(gLensModuleObj.pLensObj->shutter_getPulsePeriod());
            gLensModuleObj.pLensObj->shutter_gotoState(LENS_SHUTTER_OFF);
        }
    }

    Lens_Module_SetState(LENS_IDLE);

    return ERR_OK;
}


