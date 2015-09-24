/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_ST8005.c
    @ingroup    mISYSLens

    @brief      ST8005 (with Macro) lens driver

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
#include "LensDrv_ST8005.h"
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
static void     lensST8005_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensST8005_isFFLens(void);

//zoom
static INT32    lensST8005_zoom_initPart1(void);
static INT32    lensST8005_zoom_initPart2(void);
static INT32    lensST8005_zoom_ctrlRelease(void);
static INT32    lensST8005_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN);
static UINT8    lensST8005_zoom_getCurrentSection(void);
static UINT8    lensST8005_zoom_getMaxSection(void);
static INT32    lensST8005_zoom_retract(UINT32 param);
static void     lensST8005_zoom_doBL(void);
static void     lensST8005_zoom_gotoSection(UINT8 section);
static void     lensST8005_zoom_gotoSection2(UINT8 section, BOOL bWaitFinish);
static void     lensST8005_zoom_setAutoBoundaryCheck(BOOL en);
static void     lensST8005_zoomIsr_enable(BOOL enable);
static UINT32   lensST8005_zoom_getBL(void);
static void     lensST8005_zoom_setBL(UINT32 BLstep);
static void     lensST8005_zoom_changeState(void);
static INT32    lensST8005_zoom_gotoState(LENS_ZOOM_STATE newState);

//zoom ISR
static void     lensST8005_zoomIsr(void);
static void     lensST8005_zoomIsr_doNormal(void);

//focus
static void     lensST8005_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position);
//static void     lensST8005_focus_runStep(UINT8 dir, UINT16 step);
static INT16    lensST8005_focus_getCurrentPosition(void);
static INT16    lensST8005_focus_getMaxPosition(void);
static UINT32   lensST8005_focus_getFL(UINT32 zoomSection);
static INT16    lensST8005_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index);

//aperture
static UINT32   lensST8005_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);
static LENS_APERTURE_ACT lensST8005_aperture_getState(void);

//shutter
static INT32    lensST8005_shutter_gotoState(LENS_SHUTTER_ACT newState);

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------

static ZOOM_Struct      g_pZoomPara = {0};
static FOCUS_Struct     g_pFocusPara = {0};
static IRIS_Struct      g_pIrisPara = {0};
static SHUTTER_Struct   g_pShutterPara = {0};
static FP_LENS_CAL_CB   g_fpLensCalCB = NULL;

static UINT16 g_pZoomSectionPulse[(ZOOM_SECTION_TELE+1)]=
{
    ZOOM_POS_GARAGE,
    ZOOM_POS_WIDE,
    ZOOM_POS_HOME_PR + 520,
    ZOOM_POS_TELE
};

static UINT16 g_pFocusZoomTblDefault[ZOOM_SECTION_TELE][FOCUS_ZOOM_TABLE_COLUMN]={
//  Inf                               , 0.5m
//#NT#2011/03/02#Chris Chung -begin
{ ZOOM_POS_HOME_PR+370 , ZOOM_POS_HOME_PR+240 },
{ ZOOM_POS_HOME_PR+608 , ZOOM_POS_HOME_PR+478 },
{ ZOOM_POS_HOME_PR+845 , ZOOM_POS_HOME_PR+700 }
//#NT#2011/03/02#Chris Chung -end
};

/**
static UINT16 g_pFocusZoomTblDist[FOCUS_ZOOM_TABLE_COLUMN]=
{ 0xffff, 50 };
**/

static UINT16 g_pApertureZoomTbl[(ZOOM_SECTION_TELE+1)][1] = {
    { 0},
    {30},
    {34},
    {38}
};

static UINT16 g_pFocusLengthTbl[(ZOOM_SECTION_TELE+1)] =
{0, 62, 93, 124};//#NTK#2011/02/22#Robin Chen

static LENS_TAB g_pST8005Obj =
{
    //Lens
    lensST8005_init,
    lensST8005_isFFLens,
    //Zoom
    lensST8005_zoom_initPart1,
    lensST8005_zoom_initPart2,
    lensST8005_zoom_ctrlRelease,
    lensST8005_zoom_ctrlPress,
    lensDUMMY_zoom_ctrlPress_inner,
    lensDUMMY_zoom_getCurrentPosition,
    lensST8005_zoom_getCurrentSection,
    lensST8005_zoom_getMaxSection,
    lensDUMMY_zoom_getRatio,
    lensST8005_zoom_retract,
    lensST8005_zoom_gotoSection,
    lensST8005_zoom_setAutoBoundaryCheck,
    lensST8005_zoomIsr_enable,
    lensST8005_zoom_getBL,
    lensST8005_zoom_setBL,
    lensST8005_zoom_doBL,
    //Focus
    lensST8005_focus_gotoPosition,
    lensDUMMY_focus_gotoHome,
    lensDUMMY_focus_gotoGarage,
    lensST8005_focus_getCurrentPosition,
    lensST8005_focus_getMaxPosition,
    lensDUMMY_focus_getMinPosition,
    lensST8005_focus_getFL,
    lensST8005_focus_getZoomTblValue,
    lensDUMMY_focus_getZoomTblMaxIdx,
    lensDUMMY_focus_getCurrentDist,
    lensDUMMY_focus_getDiffStepByDist,
    lensDUMMY_focus_getBL,
    lensDUMMY_focus_setBL,
    lensDUMMY_focus_setFarStep,
    //Aperture
    lensDUMMY_aperture_gotoState,
    lensST8005_aperture_getState,
    lensST8005_aperture_getFNO,
    //Shutter
    lensST8005_shutter_gotoState,
    lensDUMMY_shutter_getPulsePeriod,
    //Others
    NULL
};

//static BOOL gbOpenPerf = FALSE;

UINT32 g_lensST8005_finalStep = 0;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8005_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    LENS_ERR(("LENS_M: %s\n\r", __func__));

    g_pST8005Obj.pMotor = pMotorTab;

    //Zoom parameter
    g_pZoomPara.bZoomBoundaryAutoCheck = FALSE;
    g_pZoomPara.ZoomOverRun = ZOOM_OVERRUN_COUNT;
    //#NT#2012/05/25#KS Hung -begin
    //#NT#Match with CDPE_NT96226
    g_pZoomPara.uiGPIOIntPol = GPIO_INTPOL_POSHIGH;
    //#NT#2012/05/25#KS Hung -end

    //Zoom ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID, GPIO_INTTYPE_EDGE, g_pZoomPara.uiGPIOIntPol);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID, lensST8005_zoomIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID);

    //Focus parameter
    g_pFocusPara.uiFocusPhase = 0;

    //Calibration information callback
    if (pLensDev->fpLensCalCB)
    {
        g_fpLensCalCB = pLensDev->fpLensCalCB;
    }
}

/**
    Get ST8005 lens object

    @return ST8005 lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    return &g_pST8005Obj;
}

BOOL lensST8005_isFFLens(void)
{
    return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with LensTask.
//--------------------------------------------------------------------------------------------------------------------------
/**
static void lensST8005_task_doZoomOff(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMOFF);
}
**/

static void lensST8005_task_doZoomBrake(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMBRK);

    LENS_MSG(("LENS_M: %s, zoom step=%d\r\n", __func__, g_pZoomPara.ZoomStep));
}

static void lensST8005_task_doZoomForward(void)
{
    g_pZoomPara.Direction = DIRECTION_FWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD);
}

static void lensST8005_task_doZoomReverse(void)
{
    g_pZoomPara.Direction = DIRECTION_BWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMREV);
}

static void lensST8005_task_doZoomCB(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMCHANGE);
}

static void lensST8005_task_setShutterState(UINT32 state)
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

static void lensST8005_zoomIsr_enable(BOOL enable)
{
    if(g_pST8005Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8005: pMotor is null\r\n"));
        return;
    }

    g_pZoomPara.bZoomIsr = enable;
    g_pST8005Obj.pMotor->zoom_setPIInt(enable);
}

static void lensST8005_zoomIsr(void)
{
    static BOOL bFirst = TRUE;

    if(!g_pZoomPara.bZoomIsr)
        return;

    if(g_pST8005Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8005: pMotor is null\r\n"));
        return;
    }

    //#NT#2012/08/20#KS Hung -begin
    {
        if(g_pZoomPara.uiGPIOIntPol == GPIO_INTPOL_NEGLOW)
        {
            if(g_pST8005Obj.pMotor->zoom_checkPI())
            {
                debug_err(("#1# rush pulse !!!!!!!!!!!!!!!!!!!\r\n"));
                return;
            }
        }
        else
        {
            if(g_pST8005Obj.pMotor->zoom_checkPI() == 0)
            {
                debug_err(("#2# rush pulse !!!!!!!!!!!!!!!!!!!\r\n"));
                return;
            }
        }

        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            g_pZoomPara.uiZoomAFStep++;
            g_pZoomPara.ZoomStep++;
        }
        else
        {
            g_pZoomPara.uiZoomAFStep--;
            g_pZoomPara.ZoomStep--;
        }

        switch(g_pZoomPara.uiCurrentState)
        {
        case ZOOM_STATE_NULL:
        case ZOOM_STATE_INSIDEINIT:
        case ZOOM_STATE_OUTSIDEINIT:
        case ZOOM_STATE_INITPART1:
            bFirst = TRUE;
            lensST8005_zoom_changeState();
            break;
        case ZOOM_STATE_INITPART2:
            //do nothing
            break;
        case ZOOM_STATE_IDLE:
        case ZOOM_STATE_RUNNING:
            lensST8005_zoomIsr_doNormal();
            break;
        case ZOOM_STATE_CLOSING:
            //do nothing
            break;
        case ZOOM_STATE_CLOSING_HOME:
            if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
            {
                if(bFirst)
                {
                    g_lensST8005_finalStep = g_pZoomPara.uiZoomAFStep;
                    bFirst = FALSE;
                }
            }

            if(g_pZoomPara.ZoomStep<=(g_pZoomSectionPulse[0] + g_pZoomPara.ZoomOverRun))
            {
                lensST8005_task_doZoomBrake();

                /* Enter ZOOM_STATE_CLOSED */
                lensST8005_zoom_changeState();
            }
            break;
        case ZOOM_STATE_CLOSED:
            //do nothing
            break;
        case ZOOM_STATE_FAIL:
            LENS_ERR(("LENS_E: lensST8005: zoom isr fail state\r\n"));
            break;
        case ZOOM_STATE_GETBL1:
            lensST8005_zoom_changeState();
            break;
        case ZOOM_STATE_GETBL2:
        case ZOOM_STATE_GETBL3:
            g_pZoomPara.ZoomBLStep++;
            //debug_err(("g_pZoomPara.ZoomBLStep = %d\r\n", g_pZoomPara.ZoomBLStep));
            lensST8005_zoom_changeState();
            break;
        case ZOOM_STATE_GETBL4:
            if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
            {
                //debug_err(("g_pZoomPara.ZoomBLStep2 = %d\r\n", g_pZoomPara.ZoomBLStep2));
                g_pZoomPara.ZoomBLStep2++;
            }
            else
            {
                lensST8005_zoom_changeState();
                lensST8005_task_doZoomBrake();
            }
            break;
        case ZOOM_STATE_GETPROFFSET1:
            if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
            {
                g_pZoomPara.ZoomPR_realStep=0;
                lensST8005_zoom_changeState();
                lensST8005_task_doZoomBrake();
            }
            break;
        case ZOOM_STATE_GETPROFFSET2:
            if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
            {
                g_pZoomPara.ZoomPR_realStep++;
            }
            break;
        case ZOOM_STATE_FOCUSING:
            break;
        default:
            LENS_ERR(("LENS_E: lensST8005: zoom isr wrong state = %d\r\n", g_pZoomPara.uiCurrentState));
        }
    }
    //else
    //    LENS_ERR(("LENS_E: %s: No PI interupt !!\r\n", __func__));
    //#NT#2012/08/20#KS Hung -end
}

static void lensST8005_zoomIsr_doNormal(void)
{
    /* The bZoomBoundaryAutoCheck restricts Zoom to the range between Wide and Tele. */
    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            if(g_pZoomPara.ZoomStep > ( g_pZoomSectionPulse[ZOOM_SECTION_TELE] - g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensST8005_task_doZoomBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensST8005_zoom_changeState();
            }
        }
        else
        {
            if(g_pZoomPara.ZoomStep < ( g_pZoomSectionPulse[ZOOM_SECTION_WIDE] + g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensST8005_task_doZoomBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensST8005_zoom_changeState();
            }
        }
    }

    if(((++g_pZoomPara.StepCnt + g_pZoomPara.ZoomOverRun) >= g_pZoomPara.StepMove))
    {
        lensST8005_task_doZoomBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
           lensST8005_zoom_changeState();
    }

    //debug_msg("lensST8005_zoomIsr_doNormal: LastSec=%d, CurrSec=%d\r\n", g_pZoomPara.uiLastSection, lensST8005_zoom_getCurrentSection());
    if(g_pZoomPara.uiLastSection != lensST8005_zoom_getCurrentSection())
    {
        g_pZoomPara.uiLastSection = lensST8005_zoom_getCurrentSection();
        lensST8005_task_doZoomCB();
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensST8005_zoom_initPart1(void)
{
    UINT32 i;

    LENS_ERR(("LENS_M: lensST8005: zoom initPart1\r\n"));

    //#NT#2011/10/03#Hideo Lin -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    if (g_fpLensCalCB != NULL)
    {
        //if(CalLens_IsAdjust())
        if (g_fpLensCalCB(LENS_CAL_STATUS, 0, 0))
        {
            for(i=1; i<=ZOOM_SECTION_TELE; i++)
            {
                /* Set the calibration value to g_pZoomSectionPulse if it is bigger than default. */
                //debug_msg(">>>>>> CalLens_GetFarFocusStep(%d) = %d\r\n", i, CalLens_GetFarFocusStep(i, FALSE));
                /*
                if (CalLens_GetFarFocusStep(i, FALSE) > g_pFocusZoomTblDefault[i-1][1] &&
                    CalLens_GetFarFocusStep(i, FALSE) < ZOOM_POS_OUTER_STOP)
                    g_pZoomSectionPulse[i] = CalLens_GetFarFocusStep(i, FALSE);
                else
                    g_pZoomSectionPulse[i] = g_pFocusZoomTblDefault[i-1][1];
                */
                if (g_fpLensCalCB(LENS_CAL_FAR_FOCUS, i, FALSE) > g_pFocusZoomTblDefault[i-1][1] &&
                    g_fpLensCalCB(LENS_CAL_FAR_FOCUS, i, FALSE) < ZOOM_POS_OUTER_STOP)
                    g_pZoomSectionPulse[i] = g_fpLensCalCB(LENS_CAL_FAR_FOCUS, i, FALSE);
                else
                    g_pZoomSectionPulse[i] = g_pFocusZoomTblDefault[i-1][1];
            }
        }
        else
        {
            debug_err(("Lens calibration is NG!\r\n"));
        }
    }
    else
    {
        debug_err(("Lens calibration CB is not registered!\r\n"));
    }

    lensST8005_zoomIsr_enable(TRUE);

    g_pZoomPara.uiCurrentState = ZOOM_STATE_NULL;

    /* Change State from ZOOM_STATE_NULL to ZOOM_STATE_INITPART2 */
    lensST8005_zoom_changeState();

    return ERR_OK;
}

static INT32 lensST8005_zoom_initPart2(void)
{
    if(lensST8005_zoom_gotoState(ZOOM_STATE_INITPART2) != ERR_OK)
        return ERR_OPERATION_FAIL;

    TimerDelayMs(100);  //wait 100ms for brake stop

    LENS_ERR(("LENS_M: lensST8005: zoom initPart2 finish, cur ZoomStep=%d\r\n", g_pZoomPara.ZoomStep));

    /* Enter ZOOM_STATE_IDLE */
    lensST8005_zoom_changeState();

    g_pZoomPara.uiLastSection = lensST8005_zoom_getCurrentSection();

    //#NT#2012/05/20#KS Hung -begin
    lensST8005_zoom_doBL();
    //#NT#2012/05/20#KS Hung -end

    return ERR_OK;
}

static INT32 lensST8005_zoom_gotoState(LENS_ZOOM_STATE newState)
{
    UINT16 cnt=0;
    UINT16 ZoomTimeOut=(_ZOOM_PSEUDO_PWM_SPEED_/1000) * (g_pZoomSectionPulse[ZOOM_SECTION_TELE] + ZOOM_BL_STEP*2 + 100);   //100 is buffer.

    //debug_msg("lensST8005_zoom_gotoState: uiCurrentState=%d, newState=%d\r\n", g_pZoomPara.uiCurrentState, newState);

    switch(newState)
    {
    case ZOOM_STATE_CLOSING:
    case ZOOM_STATE_GETBL1:
    case ZOOM_STATE_GETBL4:
    case ZOOM_STATE_GETPROFFSET1:
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
                LENS_ERR(("LENS_E: lensST8005: Zoom Timeout, stop state=%d, cur step=%d\r\n", g_pZoomPara.uiCurrentState, g_pZoomPara.ZoomStep));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_FAIL;

                lensST8005_task_doZoomBrake();

                return ERR_OPERATION_FAIL;
            }

            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_CLOSING)
            {
                if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
                {
                    /* Set ZOOM PR Step*/
                    g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;
                    lensST8005_zoom_changeState();

                    return ERR_OK;
                }
            }
        }
    }

    return ERR_OK;
}

static void lensST8005_zoom_changeState(void)
{
    if(g_pST8005Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8005: pMotor is null\r\n"));
        return;
    }

    switch(g_pZoomPara.uiCurrentState)
    {
    case ZOOM_STATE_NULL:

        if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

            LENS_ERR(("LENS_M: Zoom Out\r\n"));
            g_pST8005Obj.pMotor->changeMode(MOTOR_MODE_ZOOM);
            lensST8005_task_doZoomReverse();
        }
        else
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            LENS_ERR(("LENS_M: Zoom In, state=%d\r\n", g_pZoomPara.uiCurrentState));

            lensST8005_task_doZoomForward();
        }

        break;

    case ZOOM_STATE_FAIL:
        break;

    case ZOOM_STATE_INSIDEINIT:

        if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_INSIDEINIT --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

            lensST8005_task_doZoomReverse();
        }
        break;

    case ZOOM_STATE_OUTSIDEINIT:

        if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
        {
            /* Set uiZoomAFStep to 100 and check whether it equals to 100 or not after retraction. */
            g_pZoomPara.uiZoomAFStep = 100;

            LENS_MSG(("LENS_M: /* ZOOM_STATE_OUTSIDEINIT --> ZOOM_STATE_INITPART1 */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART1;

            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;

            lensST8005_task_doZoomForward();
        }

        break;

    case ZOOM_STATE_INITPART1:

        /*
            Check after 5/10 counts again, make sure we passed PR
            Somehow the PR we detected before is incorrect
        */
        if(g_pZoomPara.ZoomStep == (ZOOM_POS_HOME_PR-5) ||
           g_pZoomPara.ZoomStep == (ZOOM_POS_HOME_PR-10))
        {
            if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART1 --> ZOOMPR_OUTSDIE_LEVEL */\r\n"));
                g_pZoomPara.uiCurrentState = ZOOMPR_OUTSDIE_LEVEL;
            }
        }

        /*  Zoom to Wide */
        if(g_pZoomPara.ZoomStep>=(g_pZoomSectionPulse[1]-g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
        {
            lensST8005_task_doZoomBrake();

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
        if(g_pST8005Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
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
            lensST8005_task_doZoomBrake();

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

    case ZOOM_STATE_GETPROFFSET1:
        LENS_ERR(("LENS_M: /* ZOOM_STATE_GETPROFFSET1 --> ZOOM_STATE_GETPROFFSET2 */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_GETPROFFSET2;
        break;

    default:
        LENS_ERR(("LENS_E: lensST8005: zoom change unknown state\r\n"));
    }
}

static void lensST8005_zoom_runStep(unsigned char dir,unsigned short step)
{
    unsigned long StepCnt=0,count=0,Sts=0;

    if(g_pST8005Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8005: pMotor is null\r\n"));
        return;
    }

    TimerDelayMs(1);

    //debug_msg("lensST8005_zoom_runStep: dir=%d, step=%d\r\n", dir, step);

    if(g_pZoomPara.Direction!= dir)
    {
        //[Chris Chung] What does it do?
        //Ans: There is 40~50% probability to stop on PI signal, it will cause step miss when running opposite direction.
        if(g_pZoomPara.uiGPIOIntPol == GPIO_INTPOL_NEGLOW)
        {
            if(g_pST8005Obj.pMotor->zoom_checkPI() == 0)
            {
                if(dir==DIRECTION_BWD)
                {
                    //debug_err(("#1# @@@ chris -- \r\n"));
                    g_pZoomPara.ZoomStep--;
                    g_pZoomPara.uiZoomAFStep--;
                }
                else
                {
                    //debug_err(("#1# @@@ chris ++ \r\n"));
                    g_pZoomPara.ZoomStep++;
                    g_pZoomPara.uiZoomAFStep++;
                }
            }
        }
        else
        {
            if(g_pST8005Obj.pMotor->zoom_checkPI())
            {
                if(dir==DIRECTION_BWD)  // 2011-03-09 Chris
                {
                    //debug_err(("#2# @@@ chris -- \r\n"));
                    g_pZoomPara.ZoomStep--;
                    g_pZoomPara.uiZoomAFStep--;
                }
                else
                {
                    //debug_err(("#2# @@@ chris ++ \r\n"));
                    g_pZoomPara.ZoomStep++;
                    g_pZoomPara.uiZoomAFStep++;
                }
            }
        }
    }

    //debug_err(("cur = %d, dir = %d, step = %d\r\n", g_pZoomPara.ZoomStep, dir, step));

    g_pZoomPara.StepMove = step;
    g_pZoomPara.StepCnt = 0;

    /* Enter ZOOM_STATE_RUNNING */
    //debug_msg("lensST8005_zoom_runStep: uiCurrentState=%d\r\n", g_pZoomPara.uiCurrentState);
    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
        lensST8005_zoom_changeState();

    if(dir == DIRECTION_FWD)
        lensST8005_task_doZoomForward();
    else
        lensST8005_task_doZoomReverse();

    if(!g_pZoomPara.bZoomIsr)
    {
        debug_err(("### zoom ISR disabled!!\r\n"));
        while(1)
        {
            if(g_pST8005Obj.pMotor->zoom_checkPIInt())
            {
                g_pST8005Obj.pMotor->zoom_clearPIInt();

                StepCnt++;
                if(dir == DIRECTION_FWD)
                    g_pZoomPara.ZoomStep++;
                else
                    g_pZoomPara.ZoomStep--;
            }
            if((StepCnt+3) >= step && (Sts==0))
            {
                lensST8005_task_doZoomBrake();
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

    //debug_msg("lensST8005_zoom_runStep: exit\r\n");
}

static void lensST8005_zoom_doBL(void)
{
    UINT32 BLstep;

    //#NT#2011/10/03#Hideo Lin -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    if (g_fpLensCalCB != NULL)
    {
        g_pZoomPara.uiZoomBL = g_fpLensCalCB(LENS_CAL_ZOOM_BL, 0, 0);
    }
    else
    {
        g_pZoomPara.uiZoomBL = ZOOM_BL_STEP;
    }
    //#NT#2011/10/03#Hideo Lin -end

    if(g_pZoomPara.uiZoomBL > 0 && g_pZoomPara.uiZoomBL <= ZOOM_BL_STEP_LIMIT)
        BLstep = g_pZoomPara.uiZoomBL;
    else
        BLstep = ZOOM_BL_STEP;

    if(BLstep < 9+ZOOM_OVERRUN_COUNT)
        BLstep = 9+ZOOM_OVERRUN_COUNT;

    lensST8005_zoom_runStep(DIRECTION_FWD, BLstep);
    lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
    TimerDelayMs(100); //wait 100ms for brake stop
    lensST8005_zoom_runStep(DIRECTION_BWD, BLstep);
    lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
    TimerDelayMs(100); //wait 100ms for brake stop
}

static void lensST8005_zoom_gotoSection(UINT8 section)
{
    lensST8005_zoom_gotoSection2(section, TRUE);
}

static void lensST8005_zoom_gotoSection2(UINT8 section, BOOL bWaitFinish)
{
    UINT32 BLstep;

    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    BLstep = g_pZoomPara.uiZoomBL;

    //debug_msg("lensST8005_zoom_gotoSection2: BLstep=%d, ZoomStep=%d, Pulse=%d\r\n", BLstep, g_pZoomPara.ZoomStep, g_pZoomSectionPulse[section]);
    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
    {
        lensST8005_zoom_runStep(DIRECTION_BWD, g_pZoomPara.ZoomStep - g_pZoomSectionPulse[section]);
        lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
        TimerDelayMs(100);
        //debug_msg("lensST8005_zoom_gotoSection2: 1. cur step = %d\r\n", g_pZoomPara.ZoomStep);
    }
    else if(g_pZoomPara.ZoomStep < g_pZoomSectionPulse[section])
    {
        lensST8005_zoom_runStep(DIRECTION_FWD, g_pZoomSectionPulse[section] - g_pZoomPara.ZoomStep + BLstep);
        lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
        TimerDelayMs(100);
        //debug_msg("lensST8005_zoom_gotoSection2: 2. cur step = %d\r\n", g_pZoomPara.ZoomStep);
        lensST8005_zoom_runStep(DIRECTION_BWD, BLstep);
        lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
        TimerDelayMs(100);
        //debug_msg("lensST8005_zoom_gotoSection2: 3. cur step = %d\r\n", g_pZoomPara.ZoomStep);
    }
}

static UINT8 lensST8005_zoom_getTargetSection(void)
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

static UINT8 lensST8005_zoom_getCurrentSection(void)
{
    unsigned char i;
    for(i=ZOOM_SECTION_WIDE; i<ZOOM_SECTION_TELE; i++)
    {
        if(g_pZoomPara.ZoomStep <= ((g_pZoomSectionPulse[i]+g_pZoomSectionPulse[i+1])/2))
            break;
    }
    return i;
}

static UINT8 lensST8005_zoom_getMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}

/*
static UINT8 lensST8005_zoom_getNextSection(void)
{
    unsigned char i;

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
    {
        i = lensST8005_zoom_getTargetSection();
    }
    else
    {
        i = lensST8005_zoom_getCurrentSection();
    }
    if(i>ZOOM_SECTION_TELE)
        i=ZOOM_SECTION_TELE;
    else if (i<ZOOM_SECTION_WIDE)
        i=ZOOM_SECTION_WIDE;
    return i;
}
*/

static INT32 lensST8005_zoom_ctrlRelease(void) // return current zoom section
{
    unsigned char TargetSection;

    if(g_pST8005Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8005: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    g_pZoomPara.bDoOptZoomDraw=FALSE;//AO835@JJ@20080115 OpticalZoom Callback to Draw

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
    {
        if(g_pZoomPara.Direction == DIRECTION_BWD)
        {
            //lensST8005_zoom_enablePWM(FALSE); //set full speed

            TimerDelayMs(100);//harry
            lensST8005_zoom_doBL();

            //lensST8005_zoom_enablePWM(TRUE); //set PWM speed
        }

        LENS_MSG(("LENS_M: lensST8005_zoom_ctrlRelease: already idle state\r\n"));
        return lensST8005_zoom_getCurrentSection();
    }
    else if(g_pZoomPara.uiCurrentState != ZOOM_STATE_RUNNING)
    {
        LENS_ERR(("LENS_E: lensST8005_zoom_ctrlRelease: not in running state\r\n"));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        TargetSection=lensST8005_zoom_getTargetSection();

        lensST8005_zoom_gotoSection2(TargetSection, FALSE);

        if(lensST8005_zoom_gotoState(ZOOM_STATE_IDLE) != ERR_OK)
        {
            LENS_ERR(("LENS_E: lensST8005_zoom_ctrlRelease: go to IDLE state fail\r\n"));
            return ERR_OPERATION_FAIL;
        }
    }
    else
    {
        lensST8005_task_doZoomBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
            lensST8005_zoom_changeState();
    }

    TimerDelayMs(200);

    //AO835@JJ@20080115 OpticalZoom Callback to Draw
    {
        lensST8005_task_doZoomCB();
    }
    //~AO835@JJ@20080115 OpticalZoom Callback to Draw

    return lensST8005_zoom_getCurrentSection();

}

static INT32 lensST8005_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    UINT32 Step_Ori,Step_New;

    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
    {
        LENS_ERR(("LENS_E: %s, not in idle state\r\n", __func__));
        return ERR_OPERATION_FAIL;
    }
#if 0
    for(i=1; i<=ZOOM_SECTION_TELE; i++)
    {
        g_pZoomSectionPulse[i] = CalLens_GetFarFocusStep(i);
    }
#endif
    g_pZoomPara.bDoOptZoomDraw=TRUE;//AO835@JJ@20080115 OpticalZoom Callback to Draw

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(Zoom_IN == LENS_ZOOM_IN &&
           lensST8005_zoom_getCurrentSection()<ZOOM_SECTION_TELE)
        {
            lensST8005_zoom_gotoSection2(ZOOM_SECTION_TELE, FALSE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8005_zoom_changeState();
        }
        else if(Zoom_IN == LENS_ZOOM_OUT &&
                lensST8005_zoom_getCurrentSection() >ZOOM_SECTION_WIDE)
        {
            lensST8005_zoom_gotoSection2(ZOOM_SECTION_WIDE, FALSE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8005_zoom_changeState();
        }
        else
            return ERR_OPERATION_FAIL;
    }
    else
    {
        if(Zoom_IN == LENS_ZOOM_IN)
        {
            lensST8005_zoom_runStep(DIRECTION_FWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8005_zoom_changeState();
        }
        else
        {
            lensST8005_zoom_runStep(DIRECTION_BWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8005_zoom_changeState();
        }
    }
    //[Chris Chung] Check if the lens is moving. If it isn't, then return fail.
    Step_Ori = g_pZoomPara.ZoomStep;
    lensST8005_task_doZoomCB();
    TimerDelayMs(40);
    lensST8005_task_doZoomCB();
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

static void lensST8005_zoom_setAutoBoundaryCheck(BOOL en)
{
    g_pZoomPara.bZoomBoundaryAutoCheck=en;
}

static INT32 lensST8005_zoom_retract(UINT32 param)
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

    LENS_ERR(("LENS_M: lensST8005: Zoom Retract\r\n"));

    /* Enter ZOOM_STATE_CLOSING */
    lensST8005_zoom_gotoState(ZOOM_STATE_CLOSING);

    if(g_pZoomPara.ZoomStep<=g_pZoomSectionPulse[0])
    {
        LENS_ERR(("LENS_M: Zoom Already Home!!\r\n"));

        /* Enter ZOOM_STATE_CLOSED */
        lensST8005_zoom_changeState();

        return ERR_OK;
    }

    lensST8005_task_doZoomReverse();
    lensST8005_zoom_gotoState(ZOOM_STATE_CLOSING_HOME);

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

static void lensST8005_zoom_getBL_step1(void)
{
    lensST8005_zoom_retract(0);

    TimerDelayMs(500);

    lensST8005_zoom_gotoState(ZOOM_STATE_GETBL1);

    lensST8005_task_doZoomForward();
}

static void lensST8005_zoom_getBL_step2(void)
{
    lensST8005_zoom_gotoState(ZOOM_STATE_GETBL4);

    g_pZoomPara.ZoomBLStep2 = 0;

    lensST8005_task_doZoomReverse();
}

static UINT32 lensST8005_zoom_getBL(void)
{
    lensST8005_zoom_getBL_step1();
    TimerDelayMs(2000); //wait ZOOM_STATE_GETBL1 --> ZOOM_STATE_GETBL3

    lensST8005_zoom_getBL_step2();
    TimerDelayMs(2000); //wait ZOOM_STATE_GETBL4 --> ZOOM_STATE_IDLE

    LENS_ERR(("LENS_M: %s: BLStep = %d, BLStep2 = %d\r\n",
              __func__, g_pZoomPara.ZoomBLStep, g_pZoomPara.ZoomBLStep2));

    return abs(g_pZoomPara.ZoomBLStep - g_pZoomPara.ZoomBLStep2);
}

static void lensST8005_zoom_setBL(UINT32 BLstep)
{
    g_pZoomPara.uiZoomBL = BLstep;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

#if 0
static void lensST8005_focus_run03Step(LENS_DIRECTION dir)
{
    INT32 t=0;
    BOOL bAddMore = FALSE;
    static UINT32 count=0;
    volatile INT16 oldZoomStep;

    count++;
#if 0
    if((count % 30) == 0)
    {
        count = 0;
        g_pZoomPara.ZoomStep--;
        g_pZoomPara.uiZoomAFStep--;
    }
#endif
    if(g_pST8005Obj.pMotor->zoom_checkPI())
        bAddMore = TRUE;

    if(gbOpenPerf == FALSE)
    {
        Perf_Open();
        gbOpenPerf = TRUE;
    }

    //debug_err(("@@@ old step=%d\r\n", g_pZoomPara.ZoomStep));

    g_pZoomPara.Direction = dir;
    oldZoomStep = g_pZoomPara.ZoomStep;
    g_pZoomPara.uiCurrentState = ZOOM_STATE_FOCUSING;

    Perf_Mark();

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    while(abs(g_pZoomPara.ZoomStep - oldZoomStep) < 2)
    {
        if(Perf_GetDuration() > 100000)
        {
            debug_err(("%s break: g_pZoomPara.ZoomStep = %d\r\n", __func__, g_pZoomPara.ZoomStep));
            break;
        }
    }

    t = Perf_GetDuration();
#if 0
    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    TimerDelayUs(t/3);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_FWD, 0);

    if(bAddMore)
        TimerDelayUs(t/10);
    else
        TimerDelayUs(t/20);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);

    TimerDelayMs(10);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF, 0);

#else
    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_FWD, 0);

    SoftTimerDelayUs(t/9);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    if(bAddMore)
        SoftTimerDelayUs(t/30);
    else
        SoftTimerDelayUs(t/60);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);

    SoftTimerDelayUs(3300);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF, 0);
#endif

    debug_err(("@@@ t=%d, new step=%d, time=%d ms\r\n", t, g_pZoomPara.ZoomStep, Perf_GetDuration()/1000));

    g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;
}
#endif

static void lensST8005_focus_run05Step(LENS_DIRECTION dir)
{
    INT32 t=0;
    volatile INT16 oldZoomStep;

    //debug_err(("@@@ old step=%d\r\n", g_pZoomPara.ZoomStep));

    g_pZoomPara.Direction = dir;
    oldZoomStep = g_pZoomPara.ZoomStep;
    g_pZoomPara.uiCurrentState = ZOOM_STATE_FOCUSING;

    /*if(Perf_IsOpened() == FALSE)
    {
        Perf_Open();
    }*/

    Perf_Mark();

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    while(abs(g_pZoomPara.ZoomStep - oldZoomStep) < 2)
    {
        if(Perf_GetDuration() > 100000)
        {
            debug_err(("%s break: g_pZoomPara.ZoomStep = %d\r\n", __func__, g_pZoomPara.ZoomStep));
            break;
        }
    }

    t = Perf_GetDuration();

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);

    do
    {
        g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;

        TimerDelayMs(20);
    }
    while (g_pZoomPara.ZoomLastStep != g_pZoomPara.ZoomStep);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF, 0);

    //debug_err(("@1@ t=%d, new step=%d, time=%d ms\r\n", t, g_pZoomPara.ZoomStep, Perf_GetDuration()/1000));

    g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;
}

#if 0
static void lensST8005_focus_run07Step(LENS_DIRECTION dir)
{
    INT32 t=0;
    BOOL bAddMore = FALSE;
    static UINT32 count=0;
    volatile INT16 oldZoomStep;

    count++;
#if 0
    if((count % 30) == 0)
    {
        count = 0;
        g_pZoomPara.ZoomStep--;
        g_pZoomPara.uiZoomAFStep--;
    }
#endif
    if(g_pST8005Obj.pMotor->zoom_checkPI())
        bAddMore = TRUE;

    if(gbOpenPerf == FALSE)
    {
        Perf_Open();
        gbOpenPerf = TRUE;
    }

    //debug_err(("@@@ old step=%d\r\n", g_pZoomPara.ZoomStep));

    g_pZoomPara.Direction = dir;
    oldZoomStep = g_pZoomPara.ZoomStep;
    g_pZoomPara.uiCurrentState = ZOOM_STATE_FOCUSING;

    Perf_Mark();

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    while(abs(g_pZoomPara.ZoomStep - oldZoomStep) < 3)
    {
        if(Perf_GetDuration() > 100000)
        {
            debug_err(("%s break: g_pZoomPara.ZoomStep = %d\r\n", __func__, g_pZoomPara.ZoomStep));
            break;
        }
    }

    t = Perf_GetDuration();

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_FWD, 0);

    TimerDelayUs(t/2.2);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    if(bAddMore)
        TimerDelayUs(t/30);
    else
        TimerDelayUs(t/40);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);

    TimerDelayMs(20);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF, 0);

    //debug_err(("@@@ t=%d, new step=%d, time=%d ms\r\n", t, g_pZoomPara.ZoomStep, Perf_GetDuration()/1000));

    g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;
}
#else
static void lensST8005_focus_run06Step(LENS_DIRECTION dir)
{
    //INT32 t=0;
    volatile INT16 oldZoomStep;

    //debug_err(("@@@ old step=%d\r\n", g_pZoomPara.ZoomStep));

    g_pZoomPara.Direction = dir;
    oldZoomStep = g_pZoomPara.ZoomStep;
    g_pZoomPara.uiCurrentState = ZOOM_STATE_FOCUSING;

    /*if(gbOpenPerf == FALSE)
    {
        Perf_Open();
        gbOpenPerf = TRUE;
    }*/

    //Perf_Mark();

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    while(abs(g_pZoomPara.ZoomStep - oldZoomStep) < 3)
    {
        if(Perf_GetDuration() > 100000)
        {
            debug_err(("%s break: g_pZoomPara.ZoomStep = %d\r\n", __func__, g_pZoomPara.ZoomStep));
            break;
        }
    }

    //t = Perf_GetDuration();

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);

    do
    {
        g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;

        TimerDelayMs(20);
    }
    while (g_pZoomPara.ZoomLastStep != g_pZoomPara.ZoomStep);

    g_pST8005Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF, 0);

    //debug_err(("@2@ t=%d, new step=%d, time=%d ms\r\n", t, g_pZoomPara.ZoomStep, Perf_GetDuration()/1000));

    g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;
}
#endif

static void lensST8005_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position)
{
    UINT32 step, BLstep;

    //debug_msg("%s: cur step = %d, position = %d\r\n", __func__, g_pZoomPara.ZoomStep, position);

    if(position>ZOOM_POS_OUTER_STOP || position<ZOOM_POS_INNER_STOP)
        return;

    //g_pZoomPara.ZoomOverRun=7;

    //#NT#2011/10/03#Hideo Lin -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    if (g_fpLensCalCB != NULL)
    {
        g_pZoomPara.uiZoomBL = g_fpLensCalCB(LENS_CAL_ZOOM_BL, 0, 0);
    }
    else
    {
        g_pZoomPara.uiZoomBL = ZOOM_BL_STEP;
    }
    //#NT#2011/10/03#Hideo Lin -end
    //debug_msg("%s: CalLens_GetZoomBL = %d\r\n", __func__, g_pZoomPara.uiZoomBL);

    if(g_pZoomPara.uiZoomBL > 0 && g_pZoomPara.uiZoomBL <= ZOOM_BL_STEP_LIMIT)
        BLstep = g_pZoomPara.uiZoomBL;
    else
        BLstep = ZOOM_BL_STEP;

    if(BLstep < 9+ZOOM_OVERRUN_COUNT)
        BLstep = 9+ZOOM_OVERRUN_COUNT;

    if(g_pZoomPara.ZoomStep < position)
    {
        step = abs(g_pZoomPara.ZoomStep - position);
        //debug_msg("%s: move step (FWD) = %d\r\n", __func__, step);

        if(step <= 3)
            return;

#if 1
        lensST8005_zoom_runStep(DIRECTION_FWD,step+BLstep);
        lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
        #if 1
        do
        {
            g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;

            TimerDelayMs(20);
        }
        while (g_pZoomPara.ZoomLastStep != g_pZoomPara.ZoomStep);
        #else
        TimerDelayMs(100);
        #endif
        step = abs(g_pZoomPara.ZoomStep - position);
        lensST8005_zoom_runStep(DIRECTION_BWD,step);
        lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
        #if 1
        do
        {
            g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;

            TimerDelayMs(20);
        }
        while (g_pZoomPara.ZoomLastStep != g_pZoomPara.ZoomStep);
        #else
        TimerDelayMs(100);
        #endif
#else
        while(g_pZoomPara.ZoomStep > position -  BLstep)
        {
            lensST8005_focus_runThreeStep(DIRECTION_BWD);
        }
        while(position > g_pZoomPara.ZoomStep)
        {
            lensST8005_focus_runThreeStep(DIRECTION_FWD);
        }
#endif
    }
    else if(g_pZoomPara.ZoomStep > position)
    {
        step = abs(position - g_pZoomPara.ZoomStep);
        //debug_msg("%s: move step (BWD) = %d\r\n", __func__, step);

        if(step <= 1)
            return;

        if(step >= 10+ZOOM_OVERRUN_COUNT)
        {
            lensST8005_zoom_runStep(DIRECTION_BWD, step);
            lensST8005_zoom_gotoState(ZOOM_STATE_IDLE);
            #if 1
            do
            {
                g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;

                TimerDelayMs(20);
            }
            while (g_pZoomPara.ZoomLastStep != g_pZoomPara.ZoomStep);
            #else
            TimerDelayMs(100);
            #endif
        }
        else
        {
            while(position < g_pZoomPara.ZoomStep)
            {
                if(abs(position - g_pZoomPara.ZoomStep) >= 6)
                    lensST8005_focus_run06Step(DIRECTION_BWD);
                else
                    //lensST8005_focus_run03Step(DIRECTION_BWD);
                    lensST8005_focus_run05Step(DIRECTION_BWD);

                if(abs(position - g_pZoomPara.ZoomStep) <= 1)
                    break;
            }
        }
    }

    //debug_err(("%s: @@@ after step = %d\r\n", __func__, g_pZoomPara.ZoomStep));
}

static INT16 lensST8005_focus_getCurrentPosition(void)
{
    return g_pZoomPara.ZoomStep;
}

static INT16 lensST8005_focus_getMaxPosition(void)
{
    return ZOOM_POS_OUTER_STOP;
}

static UINT32 lensST8005_focus_getFL(UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pFocusLengthTbl[zoomSection];
}

static INT16 lensST8005_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    if(index > FOCUS_ZOOM_TABLE_COLUMN-1)
        return 0;

    return g_pFocusZoomTblDefault[zoomSection-1][index];
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

static UINT32 lensST8005_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    if(irisPos > LENS_APERTURE_BIG)
        return 0;

    return g_pApertureZoomTbl[zoomSection][irisPos];
}

static LENS_APERTURE_ACT lensST8005_aperture_getState(void)
{
    return g_pIrisPara.uiCurrentState;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensST8005_shutter_gotoState(LENS_SHUTTER_ACT newState)
{
    switch(newState)
    {
    case LENS_SHUTTER_CLOSE:
        lensST8005_task_setShutterState(0x2);
        g_pShutterPara.uiCurrentState = newState;
        break;
    case LENS_SHUTTER_OPEN:
        lensST8005_task_setShutterState(0x1);
        g_pShutterPara.uiCurrentState = newState;
        break;
    case LENS_SHUTTER_OFF:
        lensST8005_task_setShutterState(0x0);
        break;
    default:
        debug_err(("%s: shutter goto wrong state=%d\r\n", __func__, newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

