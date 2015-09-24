/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_A300.c
    @ingroup    mISYSLens

    @brief      A300 lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/09/24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interrupt.h"
#include "Lens_int.h"
#include "LensDrv_A300.h"
#include "LensDrv_Dummy.h"
#include "GPIO.h"
#include "debug.h"
#include "TOP.h"
#include "Timer.h"
#include "Type.h"
#include "Utility.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for function declaration.
//--------------------------------------------------------------------------------------------------------------------------

//lens
static void     lensA300_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensA300_isFFLens(void);

//zoom
static INT32    lensA300_zoom_initPart1(void);
static INT32    lensA300_zoom_initPart2(void);
static INT32    lensA300_zoom_ctrlRelease(void); // return current zoom section
static INT32    lensA300_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN); // 1: zoom in, 0:zoom out
static INT16    lensA300_zoom_getCurrentPosition(void);
static UINT8    lensA300_zoom_getCurrentSection(void);
static UINT8    lensA300_zoom_getMaxSection(void);
static INT32    lensA300_zoom_retract(UINT32 param);
static void     lensA300_zoom_gotoSection(UINT8 section);
static void     lensA300_zoom_gotoSection2(UINT8 section, BOOL bWaitFinish);
static void     lensA300_zoom_setAutoBoundaryCheck(BOOL en);
static INT32    lensA300_zoom_gotoState(LENS_ZOOM_STATE newState);
static void     lensA300_zoom_changeState(void);
static void     lensA300_zoom_enablePWM(BOOL enable);
#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
static void     lensA300_zoom_hdlSpeed(void);
#endif
static UINT32   lensA300_zoom_getBL(void);
static void     lensA300_zoom_setBL(UINT32 BLstep);
static void     lensA300_zoom_doBL(void);

//zoom ISR
static void     lensA300_zoomIsr(void);
static void     lensA300_zoomIsr_doNormal(void);
static void     lensA300_zoomIsr_enable(BOOL enable);

//focus ISR
static void     lensA300_focusIsr(void);

//focus
static void     lensA300_focus_initPhase(void);
static void     lensA300_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position);
static INT32    lensA300_focus_gotoHome(void);
static INT32    lensA300_focus_gotoGarage(void);
static void     lensA300_focus_runStep(UINT8 dir, UINT16 step);
static INT16    lensA300_focus_getCurrentPosition(void);
static INT16    lensA300_focus_getMaxPosition(void);
static INT16    lensA300_focus_getMinPosition(void);
static UINT32   lensA300_focus_getFL(UINT32 zoomSection);
static INT16    lensA300_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index);
static UINT16   lensA300_focus_getZoomTblMaxIdx(void);
static UINT32   lensA300_focus_getBL(void);

//aperture
static INT32    lensA300_aperture_gotoState(LENS_APERTURE_ACT position);
static UINT32   lensA300_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);
static LENS_APERTURE_ACT lensA300_aperture_getState(void);

//shutter
static INT32    lensA300_shutter_gotoState(LENS_SHUTTER_ACT newState);

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------

static ZOOM_Struct g_pZoomPara={0};
static FOCUS_Struct g_pFocusPara={0};
static IRIS_Struct g_pIrisPara={0};
static SHUTTER_Struct g_pShutterPara={0};
BOOL bShowFlag = FALSE;

static UINT16 g_pZoomSectionPulse[(ZOOM_SECTION_TELE+1)]=
{
    ZOOM_POS_GARAGE,
    ZOOM_POS_WIDE,
    ZOOM_POS_HOME_PR + 133,
    ZOOM_POS_HOME_PR + 180,
    ZOOM_POS_HOME_PR + 228,
    ZOOM_POS_HOME_PR + 276,
    ZOOM_POS_HOME_PR + 318,
    ZOOM_POS_HOME_PR + 361,
    ZOOM_POS_HOME_PR + 403,
    ZOOM_POS_HOME_PR + 446,
    ZOOM_POS_HOME_PR + 494,
    ZOOM_POS_TELE
};

static UINT16 g_pFocusZoomTblDefault[ZOOM_SECTION_TELE][FOCUS_ZOOM_TABLE_COLUMN]={
//  Inf , 0.5m, 0.1m
{   196 , 505 , 402   },
{   302-100 , 323 ,   0   },
{   254-100 , 287 ,   0   },
{   198-100 , 251 ,   0   },
{   155-100 , 235 ,   0   },
{   139-100 , 247 ,   0   },
{   203-100 , 317 ,   0   },
{   198-100 , 251 ,   0   },
{   155-100 , 235 ,   0   },
{   139-100 , 247 ,   0   },
{   203-100 , 317 ,   0   }
};

/*
static UINT16 g_pFocusZoomTblDist[FOCUS_ZOOM_TABLE_COLUMN]=
{ 0xffff, 50 , 10   };
*/

//The value is (fno)*10.
static UINT16 g_pApertureZoomTbl[(ZOOM_SECTION_TELE+1)][2] = {
    { 0,  0},
    {30, 45},
    {33, 47},
    {36, 51},
    {38, 55},
    {41, 59},
    {43, 62},
    {45, 66},
    {48, 69},
    {51, 72},
    {53, 75},
    {56, 80}
};

//The value is FL*10.
static UINT16 g_pFocusLengthTbl[(ZOOM_SECTION_TELE+1)] =
{0, 62, 74, 86, 99, 111, 124, 136, 148, 160, 172, 186};

#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
static UINT g_uiZoomTimerID = LENS_NULL_TIMER;
#endif

/* This table is 1-2 phase. */
static UINT8 g_pFocusStepperPhase[FOCUS_TOTAL_PHASE+1] =
{
                                            //Terminal(2,1,4,3)
    STEPPER_PHASE_M4_INVERSE(0,1,0,1),      //0101 = 5
    STEPPER_PHASE_M4_INVERSE(0,1,0,0),      //0100 = 4
    STEPPER_PHASE_M4_INVERSE(0,1,1,0),      //0110 = 6
    STEPPER_PHASE_M4_INVERSE(0,0,1,0),      //0010 = 2
    STEPPER_PHASE_M4_INVERSE(1,0,1,0),      //1010 = a
    STEPPER_PHASE_M4_INVERSE(1,0,0,0),      //1000 = 8
    STEPPER_PHASE_M4_INVERSE(1,0,0,1),      //1001 = 9
    STEPPER_PHASE_M4_INVERSE(0,0,0,1),      //0001 = 1
    STEPPER_PHASE_M4_INVERSE(0,0,0,0)       //off
};

static LENS_TAB g_pA300Obj =
{
    //Lens
    lensA300_init,
    lensA300_isFFLens,
    //Zoom
    lensA300_zoom_initPart1,
    lensA300_zoom_initPart2,
    lensA300_zoom_ctrlRelease,
    lensA300_zoom_ctrlPress,
    lensDUMMY_zoom_ctrlPress_inner,
    lensA300_zoom_getCurrentPosition,
    lensA300_zoom_getCurrentSection,
    lensA300_zoom_getMaxSection,
    lensDUMMY_zoom_getRatio,
    lensA300_zoom_retract,
    lensA300_zoom_gotoSection,
    lensA300_zoom_setAutoBoundaryCheck,
    lensA300_zoomIsr_enable,
    lensA300_zoom_getBL,
    lensA300_zoom_setBL,
    lensA300_zoom_doBL,
    //Focus
    lensA300_focus_gotoPosition,
    lensA300_focus_gotoHome,
    lensA300_focus_gotoGarage,
    lensA300_focus_getCurrentPosition,
    lensA300_focus_getMaxPosition,
    lensA300_focus_getMinPosition,
    lensA300_focus_getFL,
    lensA300_focus_getZoomTblValue,
    lensA300_focus_getZoomTblMaxIdx,
    lensDUMMY_focus_getCurrentDist,
    lensDUMMY_focus_getDiffStepByDist,
    lensA300_focus_getBL,
    lensDUMMY_focus_setBL,
    lensDUMMY_focus_setFarStep,
    //Aperture
    lensA300_aperture_gotoState,
    lensA300_aperture_getState,
    lensA300_aperture_getFNO,
    //Shutter
    lensA300_shutter_gotoState,
    lensDUMMY_shutter_getPulsePeriod,
    //Others
    NULL
};

static MOTOR_INFO_TAB g_pA300INFOTab =
{
    //pwm
    FALSE,
    _ZOOM_PSEUDO_PWM_PERIOD_,
    _ZOOM_PSEUDO_PWM_ONTIME_,
    //zoom
    #if 0
    MOTOR_ZOOM_PHASE_2_2,
    MOTOR_ZOOM_VOLTAGE_46V
    #endif
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensA300_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    g_pA300Obj.pMotor = pMotorTab;

    //Zoom parameter
    g_pZoomPara.bZoomBoundaryAutoCheck=TRUE;
    g_pZoomPara.ZoomOverRun=ZOOM_OVERRUN_COUNT;// 3

    //Zoom ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID, GPIO_INTTYPE_EDGE, GPIO_INTPOL_POSHIGH);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID, lensA300_zoomIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID);

    //Focus ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID2, GPIO_INTTYPE_EDGE, GPIO_INTPOL_NEGLOW);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID2, lensA300_focusIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID2);

    //Focus parameter
    g_pFocusPara.uiFocusPhase = 0;
    g_pFocusPara.iFocusTestPos = 200;
}

/**
    Get A300 lens object

    @return A300 lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    return &g_pA300Obj;
}

BOOL lensA300_isFFLens(void)
{
    return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with LensTask.
//--------------------------------------------------------------------------------------------------------------------------

#if 0
static void lensA300_task_doZoomOff(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMOFF);
}
#endif

static void lensA300_task_doZoomBrake(void)
{
#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
    if(g_uiZoomTimerID != LENS_NULL_TIMER)
    {
        timer_close(g_uiZoomTimerID);
        g_uiZoomTimerID = LENS_NULL_TIMER;
    }
#endif

    set_flg(FLG_ID_LENS, FLGLENS_ZOOMBRK);

    LENS_MSG(("LENS_M: %s, zoom step=%d\r\n", __func__, g_pZoomPara.ZoomStep));
}

static void lensA300_task_doZoomForward(void)
{
    g_pZoomPara.Direction = DIRECTION_FWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD);

#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
    if(g_pA300INFOTab.bPwmCtrl && g_uiZoomTimerID == LENS_NULL_TIMER)
    {
        if (timer_open(&g_uiZoomTimerID, lensA300_zoom_hdlSpeed) == 0)
        {
            g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;
            timer_set_us(g_uiZoomTimerID,
                         (_ZOOM_PSEUDO_PWM_SPEED_ * _ZOOM_PSEUDO_PWM_CHECKCOUNT_),
                         _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE,
                         _TIMER_PLAY);
        }
        else
        {
            LENS_ERR(("LENS_E: timer_open fail (speed check)\r\n"));
        }
    }
#endif
}

static void lensA300_task_doZoomReverse(void)
{
    g_pZoomPara.Direction = DIRECTION_BWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMREV);

#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
    if(g_pA300INFOTab.bPwmCtrl && g_uiZoomTimerID == LENS_NULL_TIMER)
    {
        if (timer_open(&g_uiZoomTimerID, lensA300_zoom_hdlSpeed) == 0)
        {
            g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;
            timer_set_us(g_uiZoomTimerID,
                         (_ZOOM_PSEUDO_PWM_SPEED_ * _ZOOM_PSEUDO_PWM_CHECKCOUNT_),
                         _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE,
                         _TIMER_PLAY);
        }
        else
        {
            LENS_ERR(("LENS_E: timer_open fail (speed check)\r\n"));
        }
    }
#endif
}

static void lensA300_task_doZoomCB(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMCHANGE);
}

static void lensA300_task_setApertureState(UINT32 state)
{
    LENSCTRL_PARAMOBJ lensCtrlObj;

    memset(&lensCtrlObj, 0, sizeof(LENSCTRL_PARAMOBJ));
    lensCtrlObj.uiApertureState = state;
    LensCtrl_SetParam(&lensCtrlObj);

    set_flg(FLG_ID_LENS, FLGLENS_APERTUREMOVE);
}

static void lensA300_task_setShutterState(UINT32 state)
{
    LENSCTRL_PARAMOBJ lensCtrlObj;

    memset(&lensCtrlObj, 0, sizeof(LENSCTRL_PARAMOBJ));
    lensCtrlObj.uiShutterState = state;
    LensCtrl_SetParam(&lensCtrlObj);

    set_flg(FLG_ID_LENS, FLGLENS_SHUTTERMOVE);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom ISR.
//--------------------------------------------------------------------------------------------------------------------------

static void lensA300_zoomIsr_enable(BOOL enable)
{
    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensA300: pMotor is null\r\n"));
        return;
    }

    g_pZoomPara.bZoomIsr = enable;
    g_pA300Obj.pMotor->zoom_setPIInt(enable);
}

static void lensA300_zoomIsr(void)
{
    if(!g_pZoomPara.bZoomIsr)
        return;

    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensA300: pMotor is null\r\n"));
        return;
    }

    if(g_pA300Obj.pMotor->zoom_checkPIInt())
    {
        g_pA300Obj.pMotor->zoom_clearPIInt();

        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            g_pZoomPara.ZoomStep++;

        }
        else
        {
            g_pZoomPara.ZoomStep--;
        }

        switch(g_pZoomPara.uiCurrentState)
        {
        case ZOOM_STATE_NULL:
        case ZOOM_STATE_INSIDEINIT:
        case ZOOM_STATE_OUTSIDEINIT:
        case ZOOM_STATE_INITPART1:
            lensA300_zoom_changeState();
            break;
        case ZOOM_STATE_INITPART2:
            //do nothing
            break;
        case ZOOM_STATE_IDLE:
        case ZOOM_STATE_RUNNING:
            lensA300_zoomIsr_doNormal();
            break;
        case ZOOM_STATE_CLOSING:
            //do nothing
            break;
        case ZOOM_STATE_CLOSING_HOME:
            if(g_pZoomPara.ZoomStep<=(g_pZoomSectionPulse[0] + g_pZoomPara.ZoomOverRun))
            {
                lensA300_task_doZoomBrake();

                /* Enter ZOOM_STATE_CLOSED */
                lensA300_zoom_changeState();
            }
            break;
        case ZOOM_STATE_CLOSED:
            //do nothing
            break;
        case ZOOM_STATE_FAIL:
            LENS_ERR(("LENS_E: lensA300: zoom isr fail state\r\n"));
            break;
        case ZOOM_STATE_GETBL1:
            lensA300_zoom_changeState();
            break;
        case ZOOM_STATE_GETBL2:
        case ZOOM_STATE_GETBL3:
            g_pZoomPara.ZoomBLStep++;
            //debug_err(("g_pZoomPara.ZoomBLStep = %d\r\n", g_pZoomPara.ZoomBLStep));
            lensA300_zoom_changeState();
            break;
        case ZOOM_STATE_GETBL4:
            if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
            {
                //debug_err(("g_pZoomPara.ZoomBLStep2 = %d\r\n", g_pZoomPara.ZoomBLStep2));
                g_pZoomPara.ZoomBLStep2++;
            }
            else
            {
                lensA300_zoom_changeState();
                lensA300_task_doZoomBrake();
            }
            break;
        default:
            LENS_ERR(("LENS_E: lensA300: zoom isr wrong state = %d\r\n", g_pZoomPara.uiCurrentState));
        }
    }
    else
        LENS_ERR(("LENS_E: %s: No PI interupt !!\r\n", __func__));
}

static void lensA300_zoomIsr_doNormal(void)
{
    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            if(g_pZoomPara.ZoomStep > ( ZOOM_POS_TELE - g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensA300_task_doZoomBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensA300_zoom_changeState();
            }
        }
        else
        {
            if(g_pZoomPara.ZoomStep < ( ZOOM_POS_WIDE + g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensA300_task_doZoomBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensA300_zoom_changeState();
            }
        }
    }

    if(((++g_pZoomPara.StepCnt + g_pZoomPara.ZoomOverRun) >= g_pZoomPara.StepMove))
    {
        lensA300_task_doZoomBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
           lensA300_zoom_changeState();
    }

    if(g_pZoomPara.uiLastSection != lensA300_zoom_getCurrentSection())
    {
        g_pZoomPara.uiLastSection = lensA300_zoom_getCurrentSection();
        lensA300_task_doZoomCB();
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensA300_zoom_initPart1(void)
{
    LENS_ERR(("LENS_M: lensA300: zoom initPart1\r\n"));

    lensA300_zoom_enablePWM(FALSE); //set full speed

    lensA300_zoomIsr_enable(TRUE);

    g_pZoomPara.uiCurrentState = ZOOM_STATE_NULL;

    /* Change State from ZOOM_STATE_NULL to ZOOM_STATE_INITPART2 */
    lensA300_zoom_changeState();

    return ERR_OK;
}

static INT32 lensA300_zoom_initPart2(void)
{
    if(lensA300_zoom_gotoState(ZOOM_STATE_INITPART2) != ERR_OK)
        return ERR_OPERATION_FAIL;

    TimerDelayMs(100);  //wait 100ms for brake stop

    LENS_ERR(("LENS_M: lensA300: zoom initPart2 finish, cur ZoomStep=%d\r\n", g_pZoomPara.ZoomStep));

    /* Enter ZOOM_STATE_IDLE */
    lensA300_zoom_changeState();

    lensA300_zoom_enablePWM(TRUE); //set PWM speed

    g_pZoomPara.uiLastSection = lensA300_zoom_getCurrentSection();

    //bShowFlag = TRUE;

    //#PIC#2011/06/09#Creator -begin
    //after init part2,change zoom voltage to 4.0v.
    //g_pA300INFOTab.uiZoomVoltage = MOTOR_ZOOM_VOLTAGE_40V;
    g_pA300Obj.pMotor->setMotorInfo(&g_pA300INFOTab);
    //#PIC#2011/06/09#Creator -end

    return ERR_OK;
}

static INT32 lensA300_zoom_gotoState(LENS_ZOOM_STATE newState)
{
    UINT16 cnt=0;
    UINT16 ZoomTimeOut=(_ZOOM_PSEUDO_PWM_SPEED_/1000) * (g_pZoomSectionPulse[ZOOM_SECTION_TELE] + ZOOM_BL_STEP*2 + 100);   //100 is buffer.

    switch(newState)
    {
    case ZOOM_STATE_CLOSING:
    case ZOOM_STATE_GETBL1:
    case ZOOM_STATE_GETBL4:
        LENS_MSG(("LENS_M: /* Current State=%d --> New State=%d */\r\n",
                  g_pZoomPara.uiCurrentState, newState));

        g_pZoomPara.uiCurrentState = newState;
        break;
    default:

        while(g_pZoomPara.uiCurrentState != newState)
        {
            TimerDelayMs(1);
            if(cnt++>ZoomTimeOut)
            {
                LENS_ERR(("LENS_E: lensA300: Zoom Timeout, stop state=%d, cur step=%d\r\n", g_pZoomPara.uiCurrentState, g_pZoomPara.ZoomStep));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_FAIL;

                lensA300_task_doZoomBrake();

                return ERR_OPERATION_FAIL;
            }

            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_CLOSING)
            {
                if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
                {
                    /* Set ZOOM PR Step*/
                    g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;
                    lensA300_zoom_changeState();

                    return ERR_OK;
                }
            }
        }
    }

    return ERR_OK;
}

static void lensA300_zoom_changeState(void)
{
    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensA300: pMotor is null\r\n"));
        return;
    }

    switch(g_pZoomPara.uiCurrentState)
    {
    case ZOOM_STATE_NULL:

        if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

            LENS_ERR(("LENS_M: Zoom In\r\n"));
            lensA300_task_doZoomForward();
            TimerDelayMs(500);
            lensA300_task_doZoomBrake();
            LENS_ERR(("LENS_M: Focus retract\r\n"));
            lensA300_focus_gotoGarage();
            LENS_ERR(("LENS_M: Zoom Out\r\n"));
            lensA300_task_doZoomReverse();
        }
        else
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            LENS_ERR(("LENS_M: Zoom In, state=%d\r\n", g_pZoomPara.uiCurrentState));

            lensA300_task_doZoomForward();
        }

        break;

    case ZOOM_STATE_FAIL:
        break;

    case ZOOM_STATE_INSIDEINIT:

        if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_INSIDEINIT --> ZOOM_STATE_INITPART1 */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART1;

            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;

            lensA300_task_doZoomForward();
        }
        break;

    case ZOOM_STATE_OUTSIDEINIT:

        if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
        {
            //#PIC#2011/04/14#Creator -begin
            //FOR A300 SPEC.
            static int count = 0;
            count++;
            if (count < ZOOM_POS_HOME_PR-50) break;
            //back-->brake--delay50ms-->forword
            g_pA300Obj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);//lensA300_task_doZoomBrake();
            SoftTimerDelayUs(50000/3);//LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);
            //#PIC#2011/04/14#Creator -end
            LENS_MSG(("LENS_M: /* ZOOM_STATE_OUTSIDEINIT --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            lensA300_task_doZoomForward();
        }

        break;

    case ZOOM_STATE_INITPART1:

        /*
            Check after 5/10 counts again, make sure we passed PR
            Somehow the PR we detected before is incorrect
        */
        if(g_pZoomPara.ZoomStep == (ZOOM_POS_HOME_PR+5) ||
           g_pZoomPara.ZoomStep == (ZOOM_POS_HOME_PR+10))
        {
            if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART1 --> ZOOM_STATE_OUTSIDEINIT */\r\n"));
                g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;
            }
        }

        /*  Zoom to Wide */
        if(g_pZoomPara.ZoomStep>=(g_pZoomSectionPulse[1]-g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
        {
            lensA300_task_doZoomBrake();

            LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART1 --> ZOOM_STATE_INITPART2 */\r\n"));
            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART2;
        }

        break;
    case ZOOM_STATE_INITPART2:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART2 --> ZOOM_STATE_IDLE */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;

        break;
    case ZOOM_STATE_IDLE:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_IDLE --> ZOOM_STATE_RUNNING */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_RUNNING;

        break;
    case ZOOM_STATE_RUNNING:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_RUNNING --> ZOOM_STATE_IDLE */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;

        break;
    case ZOOM_STATE_CLOSING:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_CLOSING --> ZOOM_STATE_CLOSING_HOME */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSING_HOME;

        break;
    case ZOOM_STATE_CLOSING_HOME:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_CLOSING_HOME --> ZOOM_STATE_CLOSED */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSED;

        break;
    case ZOOM_STATE_CLOSED:
        //do nothing
        break;
    case ZOOM_STATE_GETBL1:
        if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_ERR(("LENS_M: /* ZOOM_STATE_GETBL1 --> ZOOM_STATE_GETBL2 */\r\n"));
            g_pZoomPara.uiCurrentState = ZOOM_STATE_GETBL2;

            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomBLStep = 0;
        }
        break;
    case ZOOM_STATE_GETBL2:
        if(g_pZoomPara.ZoomBLStep >= ( ZOOM_BL_CALIBRATION_STEP - g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
        {
            lensA300_task_doZoomBrake();

            LENS_ERR(("LENS_M: /* ZOOM_STATE_GETBL2 --> ZOOM_STATE_GETBL3 */\r\n"));
            g_pZoomPara.uiCurrentState = ZOOM_STATE_GETBL3;
        }
        break;
    case ZOOM_STATE_GETBL3:
        //do nothing
        break;
    case ZOOM_STATE_GETBL4:
        LENS_ERR(("LENS_M: /* ZOOM_STATE_GETBL4 --> ZOOM_STATE_IDLE */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;
        break;
    default:
        LENS_ERR(("LENS_E: lensA300: zoom change unknown state\r\n"));
    }
}

static void lensA300_zoom_runStep(unsigned char dir,unsigned short step)
{
    unsigned long StepCnt=0,count=0,Sts=0;

    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensA300: pMotor is null\r\n"));
        return;
    }

    TimerDelayMs(1);
    if(g_pZoomPara.Direction!= dir)
    {
        //[Chris Chung] What does it do?
        //Ans: There is 40~50% probability to stop on PI signal, it will cause step miss when running opposite direction.
        if(!g_pA300Obj.pMotor->zoom_checkPI())
        {
            //JJ: I guess the position(ZoomStep) presents the "position of the nearest PI", not the "position of current PI-gap"
            //JJ: Even though, it still doesn't make sense.
            //JJ: This mechanism must be done in other ways, cause Zoom-step-miss is invited here!
            #if 1//JJ: temporally mark it
            if(dir==DIRECTION_FWD)
            {
                //lensalg_err(("<-->"));
                g_pZoomPara.ZoomStep--;
            }
            else
            {
                //lensalg_err(("<++>"));
                g_pZoomPara.ZoomStep++;
            }
            #endif
        }
        else
        {
            //lensalg_err(("<==>"));
        }
    }

    g_pZoomPara.StepMove = step;
    g_pZoomPara.StepCnt = 0;

    /* Enter ZOOM_STATE_RUNNING */
    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
        lensA300_zoom_changeState();

    if(dir == DIRECTION_FWD)
        lensA300_task_doZoomForward();
    else
        lensA300_task_doZoomReverse();

    if(!g_pZoomPara.bZoomIsr)
    {
        while(1)
        {
            if(g_pA300Obj.pMotor->zoom_checkPIInt())
            {
                g_pA300Obj.pMotor->zoom_clearPIInt();

                StepCnt++;
                if(dir == DIRECTION_FWD)
                    g_pZoomPara.ZoomStep++;
                else
                    g_pZoomPara.ZoomStep--;
            }
            if((StepCnt+3) >= step && (Sts==0))
            {
                lensA300_task_doZoomBrake();
                Sts = 1;
            }
            if(Sts == 1)
            {
                count++;
            }
            if(count>50000)
            {
                Sts = 0;
                break;
            }
        }
    }
}

static void lensA300_zoom_doBL(void)
{
    UINT32 step;
    //Todo: remove AAA_Adjust global variable
#if 0
    g_pZoomPara.uiZoomBL = CalLens_GetZoomBL();

    if(g_pZoomPara.uiZoomBL > 0 && g_pZoomPara.uiZoomBL <= ZOOM_BL_STEP_LIMIT)
        step = g_pZoomPara.uiZoomBL;
    else
        step = ZOOM_BL_STEP;
#else
    if(g_pZoomPara.uiZoomBL > 0 && g_pZoomPara.uiZoomBL <= ZOOM_BL_STEP)
        step = g_pZoomPara.uiZoomBL;
    else
        step = ZOOM_BL_STEP;
#endif

    LENS_ERR(("LENS_M: %s: BL step = %d\r\n", __func__, step));

    lensA300_zoom_setAutoBoundaryCheck(FALSE);

    lensA300_zoom_runStep(DIRECTION_BWD, step);
    lensA300_zoom_gotoState(ZOOM_STATE_IDLE);
    TimerDelayMs(100);
    lensA300_zoom_runStep(DIRECTION_FWD, step);
    lensA300_zoom_gotoState(ZOOM_STATE_IDLE);
    TimerDelayMs(100);

    lensA300_zoom_setAutoBoundaryCheck(TRUE);
}

static void lensA300_zoom_gotoSection(UINT8 section)
{
    BOOL bBwd = FALSE;

    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
        bBwd = TRUE;

    lensA300_zoom_gotoSection2(section, TRUE);

    if(bBwd)
        lensA300_zoom_doBL();
}

static void lensA300_zoom_gotoSection2(UINT8 section, BOOL bWaitFinish)
{
    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
    {
        lensA300_zoom_runStep(DIRECTION_BWD, g_pZoomPara.ZoomStep - g_pZoomSectionPulse[section]);
        if(bWaitFinish)
            lensA300_zoom_gotoState(ZOOM_STATE_IDLE);
    }
    else if(g_pZoomPara.ZoomStep < g_pZoomSectionPulse[section])
    {
        lensA300_zoom_runStep(DIRECTION_FWD, g_pZoomSectionPulse[section] - g_pZoomPara.ZoomStep);
        if(bWaitFinish)
            lensA300_zoom_gotoState(ZOOM_STATE_IDLE);
    }
}

static UINT8 lensA300_zoom_getTargetSection(void)
{
    unsigned char i=ZOOM_SECTION_WIDE;

    if(g_pZoomPara.Direction == DIRECTION_FWD)
    {
        for(i=ZOOM_SECTION_WIDE; i<=ZOOM_SECTION_TELE; i++)
        {
            if(g_pZoomPara.ZoomStep <= (g_pZoomSectionPulse[i]-g_pZoomPara.ZoomOverRun-2/*2 steps before*/))
                break;
        }
    }
    else if(g_pZoomPara.Direction == DIRECTION_BWD)
    {
        for(i=ZOOM_SECTION_TELE; i>=ZOOM_SECTION_WIDE; i--)
        {
            if(g_pZoomPara.ZoomStep >= (g_pZoomSectionPulse[i]+g_pZoomPara.ZoomOverRun+2/*2 steps before*/))
                break;
        }
    }
    if(i>ZOOM_SECTION_TELE)
        i = ZOOM_SECTION_TELE;
    else if(i<ZOOM_SECTION_WIDE)
        i = ZOOM_SECTION_WIDE;

    return i;
}

static INT16 lensA300_zoom_getCurrentPosition(void)
{
    return g_pZoomPara.ZoomStep;
}

static UINT8 lensA300_zoom_getCurrentSection(void)
{
    unsigned char i;
    for(i=ZOOM_SECTION_WIDE; i<ZOOM_SECTION_TELE; i++)
    {
        if(g_pZoomPara.ZoomStep <= ((g_pZoomSectionPulse[i]+g_pZoomSectionPulse[i+1])/2))
            break;
    }
    return i;
}

static UINT8 lensA300_zoom_getMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}

static INT32 lensA300_zoom_ctrlRelease(void) // return current zoom section
{
    unsigned char TargetSection;

    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensA300: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    g_pZoomPara.bDoOptZoomDraw=FALSE;//AO835@JJ@20080115 OpticalZoom Callback to Draw

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
    {
        if(g_pZoomPara.Direction == DIRECTION_BWD)
        {
            lensA300_zoom_enablePWM(FALSE); //set full speed

            TimerDelayMs(100);//harry
            lensA300_zoom_doBL();

            lensA300_zoom_enablePWM(TRUE); //set PWM speed
        }

        LENS_MSG(("LENS_M: lensA300_zoom_ctrlRelease: already idle state\r\n"));
        return lensA300_zoom_getCurrentSection();
    }
    else if(g_pZoomPara.uiCurrentState != ZOOM_STATE_RUNNING)
    {
        LENS_ERR(("LENS_E: lensA300_zoom_ctrlRelease: not in running state\r\n"));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        TargetSection=lensA300_zoom_getTargetSection();

        lensA300_zoom_gotoSection2(TargetSection, FALSE);

        if(lensA300_zoom_gotoState(ZOOM_STATE_IDLE) != ERR_OK)
        {
            LENS_ERR(("LENS_E: lensA300_zoom_ctrlRelease: go to IDLE state fail\r\n"));
            return ERR_OPERATION_FAIL;
        }

        if(g_pZoomPara.Direction == DIRECTION_BWD)
        {
            lensA300_zoom_enablePWM(FALSE); //set full speed

            TimerDelayMs(100);//harry
            lensA300_zoom_doBL();

            lensA300_zoom_enablePWM(TRUE); //set PWM speed
        }

        if(lensA300_zoom_gotoState(ZOOM_STATE_IDLE) != ERR_OK)
        {
            LENS_ERR(("LENS_E: lensA300_zoom_ctrlRelease: go to IDLE state fail[2]\r\n"));
            return ERR_OPERATION_FAIL;
        }
    }
    else
    {
        lensA300_task_doZoomBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
            lensA300_zoom_changeState();
    }

    TimerDelayMs(200);

    //???? Zoom_OnOff(OFF);
    //???? Lens_OnOff(OFF); //motor power control. 0601

    //AO835@JJ@20080115 OpticalZoom Callback to Draw
    {
        lensA300_task_doZoomCB();
    }
    //~AO835@JJ@20080115 OpticalZoom Callback to Draw

    return lensA300_zoom_getCurrentSection();

}

static INT32 lensA300_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    UINT32 Step_Ori,Step_New;

    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
    {
        LENS_ERR(("LENS_E: %s, not in idle state\r\n", __func__));
        return ERR_OPERATION_FAIL;
    }

    g_pZoomPara.bDoOptZoomDraw=TRUE;//AO835@JJ@20080115 OpticalZoom Callback to Draw

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(Zoom_IN == LENS_ZOOM_IN &&
           lensA300_zoom_getCurrentSection()<ZOOM_SECTION_TELE)
        {
            lensA300_zoom_gotoSection2(ZOOM_SECTION_TELE, FALSE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensA300_zoom_changeState();
        }
        else if(Zoom_IN == LENS_ZOOM_OUT &&
                lensA300_zoom_getCurrentSection() >ZOOM_SECTION_WIDE)
        {
            lensA300_zoom_gotoSection2(ZOOM_SECTION_WIDE, FALSE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensA300_zoom_changeState();
        }
        else
            return ERR_OPERATION_FAIL;
    }
    else
    {
        if(Zoom_IN == LENS_ZOOM_IN)
        {
            lensA300_zoom_runStep(DIRECTION_FWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensA300_zoom_changeState();
        }
        else
        {
            lensA300_zoom_runStep(DIRECTION_BWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensA300_zoom_changeState();
        }
    }
    //[Chris Chung] Check if the lens is moving. If it isn't, then return fail.
    Step_Ori = g_pZoomPara.ZoomStep;
    lensA300_task_doZoomCB();
    TimerDelayMs(40);
    lensA300_task_doZoomCB();
    Step_New = g_pZoomPara.ZoomStep;
    //lensalg_err(("Step_Ori = %d, Step_New=%d\r\n",Step_Ori,Step_New));
    if((abs(Step_New - Step_Ori)<2) && (g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING))
        return ERR_OPERATION_FAIL;
    else
    {
        //Lens_ZoomDCSpeed(255);//0
        return ERR_OK;
    }
}


static void lensA300_zoom_setAutoBoundaryCheck(BOOL en)
{
    g_pZoomPara.bZoomBoundaryAutoCheck=en;
}

static INT32 lensA300_zoom_retract(UINT32 param)
{
    UINT32 count=0;

    switch(g_pZoomPara.uiCurrentState)
    {
    case ZOOM_STATE_RUNNING:
    case ZOOM_STATE_IDLE:
    case ZOOM_STATE_FAIL:
    case ZOOM_STATE_INITPART2:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, g_pZoomPara.uiCurrentState));
        return ERR_OPERATION_FAIL;
    }

    LENS_ERR(("LENS_M: lensA300: Zoom Retract\r\n"));

    lensA300_zoom_enablePWM(FALSE); //set full speed

    /* Enter ZOOM_STATE_CLOSING */
    lensA300_zoom_gotoState(ZOOM_STATE_CLOSING);

    if(g_pZoomPara.ZoomStep<=g_pZoomSectionPulse[0])
    {
        LENS_ERR(("LENS_M: Zoom Already Home!!\r\n"));

        /* Enter ZOOM_STATE_CLOSED */
        lensA300_zoom_changeState();

        return ERR_OK;
    }

    lensA300_task_doZoomReverse();
    lensA300_zoom_gotoState(ZOOM_STATE_CLOSING_HOME);

    /* Enter ZOOM_STATE_CLOSED */
    while(g_pZoomPara.uiCurrentState != ZOOM_STATE_CLOSED)
    {
        TimerDelayMs(100);
        count++;

        if(count > 3)
        {
            LENS_ERR(("LENS_E: %s: timeout, cur step = %d\r\n", __func__, g_pZoomPara.ZoomStep));
            return ERR_OPERATION_FAIL;
        }
    }

    return ERR_OK;
}

void lensA300_zoom_enablePWM(BOOL en)
{
#if (_ZOOM_PSEUDO_PWM_ == ENABLE)

    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: pMotor is null\r\n"));
        return;
    }

    g_pA300INFOTab.bPwmCtrl = en;
    g_pA300Obj.pMotor->setMotorInfo(&g_pA300INFOTab);

    if(en)
        g_pZoomPara.ZoomOverRun = _ZOOM_PSEUDO_PWM_OVERRUNCOUNT_;
    else
        g_pZoomPara.ZoomOverRun = ZOOM_OVERRUN_COUNT;
#else

    g_pZoomPara.ZoomOverRun = ZOOM_OVERRUN_COUNT;

#endif
}

#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
void lensA300_zoom_hdlSpeed(void)
{
    UINT32 diff1, max_diff1_zero_count;
    static UINT32 diff1_zero_count = 0;

    /* No allow diff zero count more than (_ZOOM_PSEUDO_PWM_FORCETOBRAKETIME_) ms */
    max_diff1_zero_count = _ZOOM_PSEUDO_PWM_FORCETOBRAKETIME_/(_ZOOM_PSEUDO_PWM_SPEED_*_ZOOM_PSEUDO_PWM_CHECKCOUNT_/1000);

    diff1 = abs((INT32)(g_pZoomPara.ZoomLastStep - g_pZoomPara.ZoomStep));

    if(diff1 > 0)
        diff1_zero_count = 0;

    LENS_MSG(("LENS_M: diff1=%d\r\n", diff1));

    if(diff1 > (_ZOOM_PSEUDO_PWM_CHECKCOUNT_+_ZOOM_PSEUDO_PWM_CHECKDIFFCOUNT_))
    {
        //slow down
        g_pA300INFOTab.uiPwmPeriodTime += _ZOOM_PSEUDO_PWM_PERIOD_ADD_;

        lensA300_zoom_enablePWM(TRUE); //set PWM speed
    }
    else if(diff1 < (_ZOOM_PSEUDO_PWM_CHECKCOUNT_-_ZOOM_PSEUDO_PWM_CHECKDIFFCOUNT_))
    {
        //speed up
        if(g_pA300INFOTab.uiPwmPeriodTime > (_ZOOM_PSEUDO_PWM_ONTIME_+_ZOOM_PSEUDO_PWM_PERIOD_ADD_))
        {
            g_pA300INFOTab.uiPwmPeriodTime -= _ZOOM_PSEUDO_PWM_PERIOD_ADD_;

            lensA300_zoom_enablePWM(TRUE); //set PWM speed
        }

        if(diff1 == 0)
        {
            diff1_zero_count++;

            if(diff1_zero_count == max_diff1_zero_count)
            {
                LENS_ERR(("LENS_M: %s: force to do brake, cur step=%d\r\n", __func__, g_pZoomPara.ZoomStep));
                lensA300_task_doZoomBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                   lensA300_zoom_changeState();
            }
        }
    }
    else
    {
        //do nothing
    }

    g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;
}
#endif

static void lensA300_zoom_getBL_step1(void)
{
    //lensA300_zoom_retract();
    lensA300_zoom_retract(0);

    TimerDelayMs(500);

    lensA300_zoom_gotoState(ZOOM_STATE_GETBL1);

    lensA300_task_doZoomForward();
}

static void lensA300_zoom_getBL_step2(void)
{
    lensA300_zoom_gotoState(ZOOM_STATE_GETBL4);

    g_pZoomPara.ZoomBLStep2 = 0;

    lensA300_task_doZoomReverse();
}

static UINT32 lensA300_zoom_getBL(void)
{
    lensA300_zoom_getBL_step1();
    TimerDelayMs(1000);

    lensA300_zoom_getBL_step2();
    TimerDelayMs(1000);

    LENS_ERR(("LENS_M: %s: BLStep = %d, BLStep2 = %d\r\n",
              __func__, g_pZoomPara.ZoomBLStep, g_pZoomPara.ZoomBLStep2));

    if(g_pZoomPara.ZoomBLStep2 > g_pZoomPara.ZoomBLStep)
        return (g_pZoomPara.ZoomBLStep2 - g_pZoomPara.ZoomBLStep);
    else
        return (g_pZoomPara.ZoomBLStep - g_pZoomPara.ZoomBLStep2);
}

static void lensA300_zoom_setBL(UINT32 BLstep)
{
    if(BLstep > 0 && BLstep <= ZOOM_BL_STEP_LIMIT)
        g_pZoomPara.uiZoomBL = BLstep;
    else
        g_pZoomPara.uiZoomBL = ZOOM_BL_STEP;

    if(g_pZoomPara.uiZoomBL < 9+ZOOM_OVERRUN_COUNT)
        g_pZoomPara.uiZoomBL = 9+ZOOM_OVERRUN_COUNT;

    LENS_ERR(("LENS_M: %s: BL step = %d\r\n", __func__, g_pZoomPara.uiZoomBL));
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with focus ISR.
//--------------------------------------------------------------------------------------------------------------------------

static void lensA300_focusIsr(void)
{
    char str1[32];

    if(!g_pFocusPara.bFocusIsr)
        return;

    if(bShowFlag)
    {
        //SenFP_ClearOSD();
        snprintf(str1, 32, "step=%d", g_pFocusPara.iFocusTestPos);
        //SenFP_ShowOSDString(str1,50,50,4);
    }

    //debug_err(("### chris: focus home = %d\r\n", g_pFocusPara.iFocusTestPos));
    g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;
    g_pFocusPara.iFocusTestPos=100;
    g_pFocusPara.bFocusIsr = FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensA300_focus_setMotorDriver(unsigned char phase)
{
    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensA300: pMotor is null\r\n"));
        return;
    }

    g_pA300Obj.pMotor->focus_setState(g_pFocusStepperPhase[phase], 0);
}

static void lensA300_focus_doBL(void)
{
    lensA300_focus_runStep(DIRECTION_BWD, FOCUS_BL_STEP);

    TimerDelayMs(FOCUS_INIT_DELAY);

    lensA300_focus_runStep(DIRECTION_FWD, FOCUS_BL_STEP);
}

static void lensA300_focus_initPhase(void)
{
    lensA300_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);   //restore original phase

    TimerDelayMs(FOCUS_INIT_DELAY);         //init pulse 10ms
}

static void lensA300_focus_runStep2(UINT8 dir, UINT16 step, BOOL bFullSpeed)
{
    UINT32 Count = 0, i, uiSetPhaseCnt;

    /* A300 spec. define focus step for 2-2 phase. If we want to change
           1-2 phase and still use original 2-2 phase step, we need to
           set motor driver twice in every focus step. */
    uiSetPhaseCnt = 2;

    while(Count++ < step)
    {
        if(dir==DIRECTION_BWD)
        {
            for(i=0; i<uiSetPhaseCnt; i++)
            {
                if(g_pFocusPara.uiFocusPhase == 0)
                    g_pFocusPara.uiFocusPhase = (FOCUS_TOTAL_PHASE-1);
                else
                    g_pFocusPara.uiFocusPhase--;

                lensA300_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);

                if(bFullSpeed)
                    TimerDelayUs(FOCUS_SPEED);
                else
                    TimerDelayUs(FOCUS_SPEED_SLOW);
            }

            g_pFocusPara.iFocusPosition-=1;
            g_pFocusPara.iFocusTestPos-=1;
        }
        else
        {
            for(i=0; i<uiSetPhaseCnt; i++)
            {
                if(g_pFocusPara.uiFocusPhase >= (FOCUS_TOTAL_PHASE-1))
                    g_pFocusPara.uiFocusPhase = 0;
                else
                    g_pFocusPara.uiFocusPhase++;

                lensA300_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);

                if(bFullSpeed)
                    TimerDelayUs(FOCUS_SPEED);
                else
                    TimerDelayUs(FOCUS_SPEED_SLOW);
            }

            g_pFocusPara.iFocusPosition+=1;
            g_pFocusPara.iFocusTestPos+=1;
        }
    }
}

static void lensA300_focus_runStep(UINT8 dir, UINT16 step)
{
    lensA300_focus_runStep2(dir, step, TRUE);
}

static void lensA300_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position)
{
    unsigned short Step;
    unsigned char Dir;

    if(g_pFocusPara.iFocusPosition == position)
        return;

    if(g_pFocusPara.iFocusPosition > position)
    {
        Dir = DIRECTION_BWD;
        Step = g_pFocusPara.iFocusPosition - position;
    }
    else
    {
        Dir = DIRECTION_FWD;
        Step = position - g_pFocusPara.iFocusPosition;
    }

    if(category & FOCUS_PREEXC)
        lensA300_focus_initPhase();

    lensA300_focus_runStep(Dir,Step);

    if(Dir == DIRECTION_BWD)
        lensA300_focus_doBL();

    if(category & FOCUS_POSTEXC)
        TimerDelayMs(FOCUS_INIT_DELAY);
}

static INT32 lensA300_focus_gotoHome(void)
{
    unsigned short sBoundary;
    char str1[32];

    if(g_pA300Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensA300: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    if(g_pA300Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
    {
        LENS_ERR(("LENS_E: lensA300: ZOOM is inside, focus can't goto home.\r\n"));
        return ERR_OPERATION_FAIL;
    }

    lensA300_focus_initPhase();

    sBoundary = 0;
    while(g_pA300Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensA300_focus_runStep2(DIRECTION_BWD,10,TRUE);
        sBoundary += 10;
        if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            LENS_ERR(("LENS_E: B1: %d>%d",sBoundary, FOCUS_BWD_LIMIT_OUTSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    if(sBoundary > 0)   //check if do FWD.
        TimerDelayMs(FOCUS_INIT_DELAY);

    sBoundary = 0;
    g_pFocusPara.bFocusIsr = TRUE;
    while(g_pA300Obj.pMotor->focus_checkPI() == FOCUSPI_INSDIE_LEVEL)
    {
        lensA300_focus_runStep2(DIRECTION_FWD,1,FALSE);
        sBoundary += 1;
        if(sBoundary > FOCUS_FWD_LIMIT_INSIDE_PI)
        {
            LENS_ERR(("LENS_E: B2: %d>%d",sBoundary, FOCUS_FWD_LIMIT_INSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    if(g_pFocusPara.bFocusIsr)
    {
        if(bShowFlag)
        {
            //SenFP_ClearOSD();
            snprintf(str1, 32, "NG=%d", g_pFocusPara.iFocusTestPos);
            //SenFP_ShowOSDString(str1,50,50,4);
        }

        //debug_err(("############ chris: no ISR = %d\r\n", g_pFocusPara.iFocusTestPos));
        g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;
        g_pFocusPara.iFocusTestPos=100;
        g_pFocusPara.bFocusIsr = FALSE;
    }
    else
        g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;

    return ERR_OK;
}

static INT32 lensA300_focus_gotoGarage(void)
{
    if(lensA300_focus_gotoHome() == ERR_OK)
    {
        lensA300_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, FOCUS_POS_GARAGE);
        return ERR_OK;
    }
    else
    {
        return ERR_OPERATION_FAIL;
    }
}

static INT16 lensA300_focus_getCurrentPosition(void)
{
    return g_pFocusPara.iFocusPosition;
}

static INT16 lensA300_focus_getMaxPosition(void)
{
    return FOCUS_POS_OUTER_STOP;
}

static INT16 lensA300_focus_getMinPosition(void)
{
    return FOCUS_POS_INNER_STOP;
}

static UINT32 lensA300_focus_getFL(UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pFocusLengthTbl[zoomSection];
}

static INT16 lensA300_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    if(index > FOCUS_ZOOM_TABLE_COLUMN-1)
        return 0;

    return g_pFocusZoomTblDefault[zoomSection-1][index];
}

static UINT16 lensA300_focus_getZoomTblMaxIdx(void)
{
    return FOCUS_ZOOM_TABLE_COLUMN-1;
}

static UINT32 lensA300_focus_getBL(void)
{
    UINT32 count = 0;

    lensA300_zoom_gotoSection2(ZOOM_SECTION_TELE, TRUE);

    lensA300_focus_gotoHome();

    lensA300_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, FOCUS_BL_CALIBRATION_STEP);

    while(g_pA300Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensA300_focus_runStep(DIRECTION_BWD,1);
        count += 1;
        if(count > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            LENS_ERR(("LENS_E: %s: %d>%d", __func__, count, FOCUS_BWD_LIMIT_OUTSIDE_PI));
            return 0;
        }
    }

    LENS_ERR(("LENS_M: %s: count = %d\r\n", __func__, count));

    if(count > FOCUS_BL_CALIBRATION_STEP)
        return (count - FOCUS_BL_CALIBRATION_STEP);
    else
        return (FOCUS_BL_CALIBRATION_STEP - count);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensA300_aperture_gotoState(LENS_APERTURE_ACT newState)
{
    switch(newState)
    {
    case LENS_APERTURE_SMALL:
        lensA300_task_setApertureState(0x1);
        g_pIrisPara.uiCurrentState = newState;
        break;
    case LENS_APERTURE_BIG:
        lensA300_task_setApertureState(0x2);
        g_pIrisPara.uiCurrentState = newState;
        break;
    case LENS_APERTURE_OFF:
        lensA300_task_setApertureState(0x0);
        break;
    default:
        debug_err(("%s: aperture goto wrong state=%d\r\n", __func__, newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

static UINT32 lensA300_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pApertureZoomTbl[zoomSection][irisPos];
}

static LENS_APERTURE_ACT lensA300_aperture_getState(void)
{
    return g_pIrisPara.uiCurrentState;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensA300_shutter_gotoState(LENS_SHUTTER_ACT newState)
{
    switch(newState)
    {
    case LENS_SHUTTER_CLOSE:
        lensA300_task_setShutterState(0x1);
        g_pShutterPara.uiCurrentState = newState;
        break;
    case LENS_SHUTTER_OPEN:
        lensA300_task_setShutterState(0x2);
        g_pShutterPara.uiCurrentState = newState;
        break;
    case LENS_SHUTTER_OFF:
        lensA300_task_setShutterState(0x0);
        break;
    default:
        debug_err(("%s: shutter goto wrong state=%d\r\n", __func__, newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

