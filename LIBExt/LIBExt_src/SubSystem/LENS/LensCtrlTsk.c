/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       LensCtrlTsk.c
    @ingroup    mISYSLens

    @brief      Lens control task functions
                This file is the control task functions of Lens module

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2011/07/13
*/

#include <string.h>
#include "OSCommon.h"
#include "Lens_int.h"
#include "ImageAlgInfor.h"

static BOOL                 bLensCtrlOpened = FALSE;
static LENSCTRL_APPOBJ      gLensCtrlObj = {0};
static LENSCTRL_PARAMOBJ    gLensCtrlParamObj = {0};
static MOTOR_INFO_TAB       gLensCtrlMotorInfoTab = {0};

/**
  Open Lens Control task

  Open Lens Control task.
  The parameter is not using now, just for compliance

  Return value is listed below:
  E_SYS: Task is already opened
  E_OK: No error

  @param LENSCTRL_APPOBJ pLensCtrlObj: Lens Control application object
  @return ER
*/
ER LensCtrl_Open(PLENSCTRL_APPOBJ pLensCtrlObj)
{
    if (bLensCtrlOpened == TRUE)
    {
        return E_SYS;
    }

    memcpy(&gLensCtrlObj, pLensCtrlObj, sizeof(LENSCTRL_APPOBJ));
    memset(&gLensCtrlParamObj, 0, sizeof(LENSCTRL_PARAMOBJ));

    clr_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD |
                         FLGLENS_ZOOMREV |
                         FLGLENS_ZOOMBRK |
                         FLGLENS_ZOOMOFF |
                         FLGLENS_ZOOMCHANGE |
                         FLGLENS_FOCUSFWD   |
                         FLGLENS_FOCUSREV   |
                         FLGLENS_APERTUREMOVE |
                         FLGLENS_SHUTTERMOVE  |
                         FLGLENS_SHUTTEROFF   |
                         FLGLENS_ZOOM_FOCUS_MOVE_IN     |
                         FLGLENS_ZOOM_FOCUS_MOVE_OUT    |
                         FLGLENS_IDLE);

    bLensCtrlOpened = TRUE;

    sta_tsk(LENSCTRLTSK_ID);

    return E_OK;
}

/**
  Close Play Sound task

  Close Play Sound task.

  Return value is listed below:
  E_SYS: Task is already closed
  E_OK: No error

  @param void
  @return ER
*/
ER LensCtrl_Close(void)
{
    if (bLensCtrlOpened == FALSE)
    {
        return E_SYS;
    }

    bLensCtrlOpened = FALSE;

    ter_tsk(LENSCTRLTSK_ID);

    return E_OK;
}

ER LensCtrl_RegCB(FPLENSCALLBACK cb)
{
    gLensCtrlObj.fpLensCB = cb;
    return E_OK;
}

ER LensCtrl_RegKeyCB(FP_LENS_KEY_CB cb)
{
    gLensCtrlObj.fpLensKeyCB = cb;
    return E_OK;
}

PLENSCTRL_APPOBJ LensCtrl_GetObject(void)
{
    if (bLensCtrlOpened == FALSE)
    {
        return NULL;
    }
    return &gLensCtrlObj;
}

void LensCtrl_GetParam(PLENSCTRL_PARAMOBJ pLensCtrlParamObj)
{
    memcpy(pLensCtrlParamObj, &gLensCtrlParamObj, sizeof(LENSCTRL_PARAMOBJ));
}

ER LensCtrl_SetParam(PLENSCTRL_PARAMOBJ pLensCtrlParamObj)
{
    memcpy(&gLensCtrlParamObj, pLensCtrlParamObj, sizeof(LENSCTRL_PARAMOBJ));

    if(gLensCtrlObj.pMotor != NULL)
    {
        gLensCtrlMotorInfoTab.bWaitZoomFinish = gLensCtrlParamObj.bWaitZoomFinish;
        gLensCtrlMotorInfoTab.bOpenZoomPR     = gLensCtrlParamObj.bOpenZoomPR;
        gLensCtrlMotorInfoTab.uiZoomPPS       = gLensCtrlParamObj.uiZoomPPS;
        gLensCtrlMotorInfoTab.uiFocusPPS      = gLensCtrlParamObj.uiFocusPPS;

        gLensCtrlObj.pMotor->setMotorInfo(&gLensCtrlMotorInfoTab);
    }

    return E_OK;
}

ER LensCtrl_WaitCmdFinish(UINT32 TimeOut)
{
    FLGPTN  nFlag;

    if (bLensCtrlOpened == FALSE)
    {
        return E_SYS;
    }

    if(TimeOut == LENSCTRL_WAIT_INFINITE)
    {
        wai_flg(&nFlag, FLG_ID_LENS, FLGLENS_IDLE, TWF_ORW);
    }

    return E_OK;
}

ER LensCtrl_WaitCmdTypeFinish(UINT32 TimeOut,UINT32 cmd_type)
{
    FLGPTN  nFlag;

    if (bLensCtrlOpened == FALSE)
    {
        return E_SYS;
    }

    if(TimeOut == LENSCTRL_WAIT_INFINITE)
    {
        wai_flg(&nFlag, FLG_ID_LENS, cmd_type, TWF_ANDW|TWF_CLR);
    }

    return E_OK;
}

/**
  Lens control task

  Lens control task to handle interrupt event.

  @param void
  @return void
*/
void LensCtrlTsk(void)
{
    FLGPTN  uiFlag;

    while (1)
    {
        set_flg(FLG_ID_LENS, FLGLENS_IDLE);
        wai_flg(&uiFlag, FLG_ID_LENS,
                         FLGLENS_ZOOMFWD |
                         FLGLENS_ZOOMREV |
                         FLGLENS_ZOOMBRK |
                         FLGLENS_ZOOMOFF |
                         FLGLENS_ZOOMCHANGE |
                         FLGLENS_FOCUSFWD   |
                         FLGLENS_FOCUSREV   |
                         FLGLENS_APERTUREMOVE |
                         FLGLENS_SHUTTERMOVE  |
                         FLGLENS_ZOOM_FOCUS_MOVE_IN     |
                         FLGLENS_ZOOM_FOCUS_MOVE_OUT    |
                         FLGLENS_SHUTTEROFF
                       , TWF_ORW | TWF_CLR);
        clr_flg(FLG_ID_LENS, FLGLENS_IDLE);

        if(gLensCtrlObj.pMotor == NULL)
        {
            LENS_ERR(("LENS_E: %s pMotor is null\r\n", __func__));
            continue;
        }

        if(uiFlag & FLGLENS_ZOOM_FOCUS_MOVE_IN)
        {
            //#NT#2011/10/03#Hideo Lin -begin
            //#NT#Use callback instead of GPIO since it may use ADC key
            //while (gpio_getPin(gLensCtrlObj.uiTeleKey)==0 && gLensCtrlObj.pLens->zoom_getCurrentSection() < gLensCtrlObj.pLens->zoom_getMaxSection())
            if (gLensCtrlObj.fpLensKeyCB != NULL)
            {
                while (gLensCtrlObj.fpLensKeyCB(LENS_KEY_TELE, LENS_KEY_PRESS) && \
                       gLensCtrlObj.pLens->zoom_getCurrentSection() < gLensCtrlObj.pLens->zoom_getMaxSection())
                {
                    gLensCtrlObj.pLens->zoom_ctrlPress_inner(LENS_ZOOM_IN);
                    LensCtrl_WaitCmdTypeFinish(LENSCTRL_WAIT_INFINITE, FLGLENS_ZOOM_CMD_FIN|FLGLENS_FOCUS_CMD_FIN);
                    TimerDelayMs(10);
                }
            }
            //#NT#2011/10/03#Hideo Lin -end
            continue;
        }

        if(uiFlag & FLGLENS_ZOOM_FOCUS_MOVE_OUT)
        {
            //#NT#2011/10/03#Hideo Lin -begin
            //#NT#Use callback instead of GPIO since it may use ADC key
            //while (gpio_getPin(gLensCtrlObj.uiWideKey)==0 && gLensCtrlObj.pLens->zoom_getCurrentSection() > 1)
            if (gLensCtrlObj.fpLensKeyCB != NULL)
            {
                while (gLensCtrlObj.fpLensKeyCB(LENS_KEY_WIDE, LENS_KEY_PRESS) && \
                       gLensCtrlObj.pLens->zoom_getCurrentSection() > 1)
                {
                    gLensCtrlObj.pLens->zoom_ctrlPress_inner(LENS_ZOOM_OUT);
                    LensCtrl_WaitCmdTypeFinish(LENSCTRL_WAIT_INFINITE, FLGLENS_ZOOM_CMD_FIN|FLGLENS_FOCUS_CMD_FIN);
                    TimerDelayMs(10);
                }
            }
            //#NT#2011/10/03#Hideo Lin -end
            continue;
        }

        if(uiFlag & FLGLENS_ZOOMBRK)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);
            continue;
        }

        if(uiFlag & FLGLENS_ZOOMFWD)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_FWD, gLensCtrlParamObj.uiZoomStep);
            continue;
        }

        if(uiFlag & FLGLENS_ZOOMREV)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, gLensCtrlParamObj.uiZoomStep);
            continue;
        }

        if(uiFlag & FLGLENS_ZOOMOFF)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_OFF, 0);
            continue;
        }

        if(uiFlag & FLGLENS_ZOOMCHANGE)
        {
            if(gLensCtrlObj.fpLensCB != NULL)
            {
                gLensCtrlObj.fpLensCB(ALGMSG_CHGOZOOMSTEP, NULL);
            }
            continue;
        }

        if(uiFlag & FLGLENS_FOCUSFWD)
        {
            gLensCtrlObj.pMotor->focus_setState(MOTOR_FOCUS_FWD, gLensCtrlParamObj.uiFocusStep);
            LensCtrl_WaitCmdTypeFinish(LENSCTRL_WAIT_INFINITE, FLGLENS_FOCUS_CMD_FIN);
            TimerDelayMs(2);
            set_flg(FLG_ID_LENS, FLGLENS_FOCUS_CMD_FIN2);
            continue;
        }

        if(uiFlag & FLGLENS_FOCUSREV)
        {
            gLensCtrlObj.pMotor->focus_setState(MOTOR_FOCUS_BWD, gLensCtrlParamObj.uiFocusStep);
            LensCtrl_WaitCmdTypeFinish(LENSCTRL_WAIT_INFINITE, FLGLENS_FOCUS_CMD_FIN);
            TimerDelayMs(2);
            set_flg(FLG_ID_LENS, FLGLENS_FOCUS_CMD_FIN2);
            continue;
        }

        if(uiFlag & FLGLENS_APERTUREMOVE)
        {
            gLensCtrlObj.pMotor->aperture_setState(gLensCtrlParamObj.uiApertureState);
            TimerDelayMs(gLensCtrlObj.pLens->shutter_getPulsePeriod());
            gLensCtrlObj.pMotor->aperture_setState(LENS_APERTURE_OFF);
            continue;
        }

        if(uiFlag & FLGLENS_SHUTTERMOVE)
        {
            gLensCtrlObj.pMotor->shutter_setState(gLensCtrlParamObj.uiShutterState);
            TimerDelayMs(gLensCtrlObj.pLens->shutter_getPulsePeriod());
            gLensCtrlObj.pMotor->shutter_setState(LENS_SHUTTER_OFF);
            continue;
        }

        if(uiFlag & FLGLENS_SHUTTEROFF)
        {
            TimerDelayMs(gLensCtrlObj.pLens->shutter_getPulsePeriod());

            gLensCtrlObj.pMotor->changeMode(MOTOR_MODE_SHUTTER);
            gLensCtrlObj.pMotor->shutter_setState(LENS_SHUTTER_OFF);

            gLensCtrlObj.pMotor->changeMode(MOTOR_MODE_STANDBY);
            gLensCtrlObj.pMotor->power(MOTOR_POWER_OFF, 0);
            continue;
        }

    }
}

