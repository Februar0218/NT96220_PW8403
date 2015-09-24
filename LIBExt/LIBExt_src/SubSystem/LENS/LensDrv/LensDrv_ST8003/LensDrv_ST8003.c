/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       LensDrv_ST8003.c
    @ingroup    mISYSLens

    @brief      ST8003 (ST8003 w/o pseudo 5x) lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2011/07/13
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interrupt.h"
#include "Lens_int.h"
#include "LensDrv_ST8003.h"
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
static void     lensST8003_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensST8003_isFFLens(void);

//zoom
static INT32    lensST8003_zoom_initPart1(void);
static INT32    lensST8003_zoom_initPart2(void);
static INT32    lensST8003_zoom_ctrlRelease(void); // return current zoom section
static INT32    lensST8003_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN); // 1: zoom in, 0:zoom out
static UINT8    lensST8003_zoom_getCurrentSection(void);
static UINT8    lensST8003_zoom_getMaxSection(void);
static INT32    lensST8003_zoom_retract(UINT32 param);
static void     lensST8003_zoom_gotoSection(UINT8 section);
static void     lensST8003_zoom_gotoSection2(UINT8 section, BOOL bWaitFinish);
static void     lensST8003_zoom_setAutoBoundaryCheck(BOOL en);
static INT32    lensST8003_zoom_gotoState(LENS_ZOOM_STATE newState);
static void     lensST8003_zoom_changeState(void);
static void     lensST8003_zoom_enablePWM(BOOL enable);
static UINT32   lensST8003_zoom_getBL(void);
static void     lensST8003_zoom_setBL(UINT32 BLstep);
static void     lensST8003_zoom_doBL(void);

//zoom ISR
static void     lensST8003_zoomIsr(void);
static void     lensST8003_zoomIsr_doNormal(void);
static void     lensST8003_zoomIsr_enable(BOOL enable);

//focus ISR
static void     lensST8003_focusIsr(void);

//focus
static void     lensST8003_focus_initPhase(void);
static void     lensST8003_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position);
static INT32    lensST8003_focus_gotoHome(void);
static INT32    lensST8003_focus_gotoGarage(void);
static void     lensST8003_focus_runStep(UINT8 dir, UINT16 step);
static INT16    lensST8003_focus_getCurrentPosition(void);
static INT16    lensST8003_focus_getMaxPosition(void);
static UINT32   lensST8003_focus_getFL(UINT32 zoomSection);
static INT16    lensST8003_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index);
static UINT32   lensST8003_focus_getBL(void);

//aperture
static INT32    lensST8003_aperture_gotoState(LENS_APERTURE_ACT position);
static UINT32   lensST8003_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);
static LENS_APERTURE_ACT lensST8003_aperture_getState(void);

//shutter
static INT32    lensST8003_shutter_gotoState(LENS_SHUTTER_ACT newState);

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------

static ZOOM_Struct      g_pZoomPara={0};
static FOCUS_Struct     g_pFocusPara={0};
static IRIS_Struct      g_pIrisPara={0};
static SHUTTER_Struct   g_pShutterPara={0};
static FP_LENS_CAL_CB   g_fpLensCalCB = NULL;

static UINT16 g_pZoomSectionPulse[(ZOOM_SECTION_TELE+1)]=
{
    ZOOM_POS_GARAGE,
    ZOOM_POS_WIDE,
    ZOOM_POS_HOME_PR + 520,
    ZOOM_POS_HOME_PR + 607,
    ZOOM_POS_HOME_PR + 693,
    ZOOM_POS_HOME_PR + 780,
    ZOOM_POS_HOME_PR + 867,
    ZOOM_POS_TELE
};

static UINT16 g_pFocusZoomTblDefault[ZOOM_SECTION_TELE][FOCUS_ZOOM_TABLE_COLUMN]={
//  Inf , 0.5m, 0.1m
{   344-100 , 357 , 402   },
{   302-100 , 323 ,   0   },
{   254-100 , 287 ,   0   },
{   198-100 , 251 ,   0   },
{   155-100 , 235 ,   0   },
{   139-100 , 247 ,   0   },
{   203-100 , 317 ,   0   }
};

//The value is (fno)*10.
static UINT16 g_pApertureZoomTbl[(ZOOM_SECTION_TELE+1)][2] = {
    { 0,  0},
    {30, 43},
    {34, 49},
    {38, 55},
    {43, 62},
    {48, 70},
    {53, 77},
    {55, 80},
};

static UINT16 g_pFocusLengthTbl[(ZOOM_SECTION_TELE+1)] =
{0, 60, 80, 97, 113, 140, 162, 180};

/* This table is 2-2 phase. */
static UINT8 g_pFocusStepperPhase[FOCUS_TOTAL_PHASE+1] =
{
                                    //(B,/B,A,/A)
    STEPPER_PHASE_M4(0,1,0,1),      //1010 = a
    STEPPER_PHASE_M4(0,1,1,0),      //0110 = 6
    STEPPER_PHASE_M4(1,0,1,0),      //0101 = 5
    STEPPER_PHASE_M4(1,0,0,1),      //1001 = 9
    STEPPER_PHASE_M4(0,0,0,0)       //off
};

static LENS_TAB g_pST8003Obj =
{
    //Lens
    lensST8003_init,
    lensST8003_isFFLens,
    //Zoom
    lensST8003_zoom_initPart1,
    lensST8003_zoom_initPart2,
    lensST8003_zoom_ctrlRelease,
    lensST8003_zoom_ctrlPress,
    lensDUMMY_zoom_ctrlPress_inner,
    lensDUMMY_zoom_getCurrentPosition,
    lensST8003_zoom_getCurrentSection,
    lensST8003_zoom_getMaxSection,
    lensDUMMY_zoom_getRatio,
    lensST8003_zoom_retract,
    lensST8003_zoom_gotoSection,
    lensST8003_zoom_setAutoBoundaryCheck,
    lensST8003_zoomIsr_enable,
    lensST8003_zoom_getBL,
    lensST8003_zoom_setBL,
    lensST8003_zoom_doBL,
    //Focus
    lensST8003_focus_gotoPosition,
    lensST8003_focus_gotoHome,
    lensST8003_focus_gotoGarage,
    lensST8003_focus_getCurrentPosition,
    lensST8003_focus_getMaxPosition,
    lensDUMMY_focus_getMinPosition,
    lensST8003_focus_getFL,
    lensST8003_focus_getZoomTblValue,
    lensDUMMY_focus_getZoomTblMaxIdx,
    lensDUMMY_focus_getCurrentDist,
    lensDUMMY_focus_getDiffStepByDist,
    lensST8003_focus_getBL,
    lensDUMMY_focus_setBL,
    lensDUMMY_focus_setFarStep,
    //Aperture
    lensST8003_aperture_gotoState,
    lensST8003_aperture_getState,
    lensST8003_aperture_getFNO,
    //Shutter
    lensST8003_shutter_gotoState,
    lensDUMMY_shutter_getPulsePeriod,
    //Others
    NULL
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8003_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    g_pST8003Obj.pMotor = pMotorTab;

    //Zoom parameter
    g_pZoomPara.bZoomBoundaryAutoCheck = TRUE;
    g_pZoomPara.ZoomOverRun = ZOOM_OVERRUN_COUNT;
    g_pZoomPara.uiGPIOIntPol = GPIO_INTPOL_POSHIGH;

    //Zoom ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID, GPIO_INTTYPE_EDGE, g_pZoomPara.uiGPIOIntPol);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID, lensST8003_zoomIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID);

    //Focus ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID2, GPIO_INTTYPE_EDGE, GPIO_INTPOL_NEGLOW);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID2, lensST8003_focusIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID2);

    //Focus parameter
    g_pFocusPara.uiFocusPhase = 0;
    g_pFocusPara.iFocusTestPos = 200;

    //Calibration information callback
    if (pLensDev->fpLensCalCB)
    {
        g_fpLensCalCB = pLensDev->fpLensCalCB;
    }
}

/**
    Get ST8003 lens object

    @return ST8003 lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    return &g_pST8003Obj;
}

BOOL lensST8003_isFFLens(void)
{
    return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with LensTask.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8003_task_doZoomBrake(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMBRK);

    LENS_MSG(("LENS_M: %s, zoom step=%d\r\n", __func__, g_pZoomPara.ZoomStep));
}

static void lensST8003_task_doZoomForward(void)
{
    g_pZoomPara.Direction = DIRECTION_FWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD);
}

static void lensST8003_task_doZoomReverse(void)
{
    g_pZoomPara.Direction = DIRECTION_BWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMREV);
}

static void lensST8003_task_doZoomCB(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMCHANGE);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom ISR.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8003_zoomIsr_enable(BOOL enable)
{
    if(g_pST8003Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8003: pMotor is null\r\n"));
        return;
    }

    g_pZoomPara.bZoomIsr = enable;
    g_pST8003Obj.pMotor->zoom_setPIInt(enable);
}

static void lensST8003_zoomIsr(void)
{
    if(!g_pZoomPara.bZoomIsr)
        return;

    if(g_pST8003Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8003: pMotor is null\r\n"));
        return;
    }

    if(g_pST8003Obj.pMotor->zoom_checkPIInt())
    {
        g_pST8003Obj.pMotor->zoom_clearPIInt();

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
            lensST8003_zoom_changeState();
            break;
        case ZOOM_STATE_INITPART2:
            //do nothing
            break;
        case ZOOM_STATE_IDLE:
        case ZOOM_STATE_RUNNING:
            lensST8003_zoomIsr_doNormal();
            break;
        case ZOOM_STATE_CLOSING:
            //do nothing
            break;
        case ZOOM_STATE_CLOSING_HOME:
            if(g_pZoomPara.ZoomStep<=(g_pZoomSectionPulse[0] + g_pZoomPara.ZoomOverRun))
            {
                lensST8003_task_doZoomBrake();

                /* Enter ZOOM_STATE_CLOSED */
                lensST8003_zoom_changeState();
            }
            break;
        case ZOOM_STATE_CLOSED:
            //do nothing
            break;
        case ZOOM_STATE_FAIL:
            LENS_ERR(("LENS_E: lensST8003: zoom isr fail state\r\n"));
            break;
        case ZOOM_STATE_GETBL1:
            lensST8003_zoom_changeState();
            break;
        case ZOOM_STATE_GETBL2:
        case ZOOM_STATE_GETBL3:
            g_pZoomPara.ZoomBLStep++;
            //debug_err(("g_pZoomPara.ZoomBLStep = %d\r\n", g_pZoomPara.ZoomBLStep));
            lensST8003_zoom_changeState();
            break;
        case ZOOM_STATE_GETBL4:
            if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
            {
                //debug_err(("g_pZoomPara.ZoomBLStep2 = %d\r\n", g_pZoomPara.ZoomBLStep2));
                g_pZoomPara.ZoomBLStep2++;
            }
            else
            {
                lensST8003_zoom_changeState();
                lensST8003_task_doZoomBrake();
            }
            break;
        default:
            LENS_ERR(("LENS_E: lensST8003: zoom isr wrong state = %d\r\n", g_pZoomPara.uiCurrentState));
        }
    }
    else
        LENS_ERR(("LENS_E: %s: No PI interupt !!\r\n", __func__));
}

static void lensST8003_zoomIsr_doNormal(void)
{
    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            if(g_pZoomPara.ZoomStep > ( ZOOM_POS_TELE - g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensST8003_task_doZoomBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensST8003_zoom_changeState();
            }
        }
        else
        {
            if(g_pZoomPara.ZoomStep < ( ZOOM_POS_WIDE + g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensST8003_task_doZoomBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensST8003_zoom_changeState();
            }
        }
    }

    if(((++g_pZoomPara.StepCnt + g_pZoomPara.ZoomOverRun) >= g_pZoomPara.StepMove))
    {
        lensST8003_task_doZoomBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
           lensST8003_zoom_changeState();
    }

    if(g_pZoomPara.uiLastSection != lensST8003_zoom_getCurrentSection())
    {
        g_pZoomPara.uiLastSection = lensST8003_zoom_getCurrentSection();
        lensST8003_task_doZoomCB();
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensST8003_zoom_initPart1(void)
{
    LENS_ERR(("LENS_M: lensST8003: zoom initPart1\r\n"));

    lensST8003_zoom_enablePWM(FALSE); //set full speed

    lensST8003_zoomIsr_enable(TRUE);

    g_pZoomPara.uiCurrentState = ZOOM_STATE_NULL;

    /* Change State from ZOOM_STATE_NULL to ZOOM_STATE_INITPART2 */
    lensST8003_zoom_changeState();

    return ERR_OK;
}

static INT32 lensST8003_zoom_initPart2(void)
{
    if(lensST8003_zoom_gotoState(ZOOM_STATE_INITPART2) != ERR_OK)
        return ERR_OPERATION_FAIL;

    TimerDelayMs(100);  //wait 100ms for brake stop

    LENS_ERR(("LENS_M: lensST8003: zoom initPart2 finish, cur ZoomStep=%d\r\n", g_pZoomPara.ZoomStep));

    /* Enter ZOOM_STATE_IDLE */
    lensST8003_zoom_changeState();

    lensST8003_zoom_enablePWM(TRUE); //set PWM speed

    g_pZoomPara.uiLastSection = lensST8003_zoom_getCurrentSection();

    return ERR_OK;
}

/* Only zoom_gotoState and zoom_changeState can modify the value
       of g_pZoomPara.uiCurrentState. */
static INT32 lensST8003_zoom_gotoState(LENS_ZOOM_STATE newState)
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
                LENS_ERR(("LENS_E: lensST8003: Zoom Timeout, stop state=%d, cur step=%d\r\n", g_pZoomPara.uiCurrentState, g_pZoomPara.ZoomStep));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_FAIL;

                lensST8003_task_doZoomBrake();

                return ERR_OPERATION_FAIL;
            }

            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_CLOSING)
            {
                if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
                {
                    /* Set ZOOM PR Step*/
                    g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;
                    lensST8003_zoom_changeState();

                    return ERR_OK;
                }
            }
        }
    }

    return ERR_OK;
}

static void lensST8003_zoom_changeState(void)
{
    if(g_pST8003Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8003: pMotor is null\r\n"));
        return;
    }

    switch(g_pZoomPara.uiCurrentState)
    {
    case ZOOM_STATE_NULL:

        if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

            /* At first, zoom in and focus retract to avoid two motors conflict. */
            LENS_ERR(("LENS_M: Zoom In\r\n"));
            lensST8003_task_doZoomForward();
            TimerDelayMs(500);
            lensST8003_task_doZoomBrake();
            LENS_ERR(("LENS_M: Focus retract\r\n"));
            lensST8003_focus_gotoGarage();
            LENS_ERR(("LENS_M: Zoom Out\r\n"));
            lensST8003_task_doZoomReverse();
        }
        else
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            LENS_ERR(("LENS_M: Zoom In, state=%d\r\n", g_pZoomPara.uiCurrentState));

            lensST8003_task_doZoomForward();
        }

        break;

    case ZOOM_STATE_FAIL:
        break;

    case ZOOM_STATE_INSIDEINIT:

        if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_INSIDEINIT --> ZOOM_STATE_INITPART1 */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART1;

            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;

            lensST8003_task_doZoomForward();
        }
        break;

    case ZOOM_STATE_OUTSIDEINIT:

        if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_OUTSIDEINIT --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            lensST8003_task_doZoomForward();
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
            if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART1 --> ZOOM_STATE_OUTSIDEINIT */\r\n"));
                g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;
            }
        }

        /*  Zoom to Wide */
        if(g_pZoomPara.ZoomStep>=(g_pZoomSectionPulse[1]-g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
        {
            lensST8003_task_doZoomBrake();

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
        if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
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
            lensST8003_task_doZoomBrake();

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
        LENS_ERR(("LENS_E: lensST8003: zoom change unknown state\r\n"));
    }
}

static void lensST8003_zoom_runStep(unsigned char dir,unsigned short step)
{
    unsigned long StepCnt=0,count=0,Sts=0;

    if(g_pST8003Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8003: pMotor is null\r\n"));
        return;
    }

    TimerDelayMs(1);
    if(g_pZoomPara.Direction!= dir)
    {
        //[Chris Chung] What does it do?
        //Ans: There is 40~50% probability to stop on PI signal, it will cause step miss when running opposite direction.
        if(g_pZoomPara.uiGPIOIntPol == GPIO_INTPOL_NEGLOW)
        {
            if(g_pST8003Obj.pMotor->zoom_checkPI() == 0)
            {
                if(dir==DIRECTION_BWD)
                {
                    //debug_err(("#1# @@@ chris -- \r\n"));
                    g_pZoomPara.ZoomStep--;
                }
                else
                {
                    //debug_err(("#1# @@@ chris ++ \r\n"));
                    g_pZoomPara.ZoomStep++;
                }
            }
        }
        else
        {
            if(g_pST8003Obj.pMotor->zoom_checkPI())
            {
                if(dir==DIRECTION_BWD)
                {
                    //debug_err(("#2# @@@ chris -- \r\n"));
                    g_pZoomPara.ZoomStep--;
                }
                else
                {
                    //debug_err(("#2# @@@ chris ++ \r\n"));
                    g_pZoomPara.ZoomStep++;
                }
            }
        }
    }

    g_pZoomPara.StepMove = step;
    g_pZoomPara.StepCnt = 0;

    /* Enter ZOOM_STATE_RUNNING */
    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
        lensST8003_zoom_changeState();

    if(dir == DIRECTION_FWD)
        lensST8003_task_doZoomForward();
    else
        lensST8003_task_doZoomReverse();

    if(!g_pZoomPara.bZoomIsr)
    {
        while(1)
        {
            if(g_pST8003Obj.pMotor->zoom_checkPIInt())
            {
                g_pST8003Obj.pMotor->zoom_clearPIInt();

                StepCnt++;
                if(dir == DIRECTION_FWD)
                    g_pZoomPara.ZoomStep++;
                else
                    g_pZoomPara.ZoomStep--;
            }
            if((StepCnt+3) >= step && (Sts==0))
            {
                lensST8003_task_doZoomBrake();
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

static void lensST8003_zoom_doBL(void)
{
    UINT32 step;

    step = g_pZoomPara.uiZoomBL;

    LENS_ERR(("LENS_M: %s: BL step = %d\r\n", __func__, step));

    /* Since zoom BL will run over boundary, we should disable auto boundary check at first. */
    lensST8003_zoom_setAutoBoundaryCheck(FALSE);

    lensST8003_zoom_runStep(DIRECTION_BWD, step);
    lensST8003_zoom_gotoState(ZOOM_STATE_IDLE);
    TimerDelayMs(100);
    lensST8003_zoom_runStep(DIRECTION_FWD, step);
    lensST8003_zoom_gotoState(ZOOM_STATE_IDLE);
    TimerDelayMs(100);

    lensST8003_zoom_setAutoBoundaryCheck(TRUE);
}

static void lensST8003_zoom_gotoSection(UINT8 section)
{
    BOOL bBwd = FALSE;

    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    if(abs(g_pZoomPara.ZoomStep - g_pZoomSectionPulse[section]) <= 1)
        return;

    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
        bBwd = TRUE;

    lensST8003_zoom_gotoSection2(section, TRUE);

    if(bBwd)
        lensST8003_zoom_doBL();
}

static void lensST8003_zoom_gotoSection2(UINT8 section, BOOL bWaitFinish)
{
    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
    {
        lensST8003_zoom_runStep(DIRECTION_BWD, g_pZoomPara.ZoomStep - g_pZoomSectionPulse[section]);
        if(bWaitFinish)
        {
            lensST8003_zoom_gotoState(ZOOM_STATE_IDLE);
            TimerDelayMs(100);
        }
    }
    else if(g_pZoomPara.ZoomStep < g_pZoomSectionPulse[section])
    {
        lensST8003_zoom_runStep(DIRECTION_FWD, g_pZoomSectionPulse[section] - g_pZoomPara.ZoomStep);
        if(bWaitFinish)
        {
            lensST8003_zoom_gotoState(ZOOM_STATE_IDLE);
            TimerDelayMs(100);
        }
    }
}

static UINT8 lensST8003_zoom_getTargetSection(void)
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

static UINT8 lensST8003_zoom_getCurrentSection(void)
{
    unsigned char i;
    for(i=ZOOM_SECTION_WIDE; i<ZOOM_SECTION_TELE; i++)
    {
        if(g_pZoomPara.ZoomStep <= ((g_pZoomSectionPulse[i]+g_pZoomSectionPulse[i+1])/2))
            break;
    }
    return i;
}

static UINT8 lensST8003_zoom_getMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}

static INT32 lensST8003_zoom_ctrlRelease(void) // return current zoom section
{
    unsigned char TargetSection;

    if(g_pST8003Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8003: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
    {
        if(g_pZoomPara.Direction == DIRECTION_BWD)
        {
            lensST8003_zoom_enablePWM(FALSE); //set full speed

            TimerDelayMs(100);//harry
            lensST8003_zoom_doBL();

            lensST8003_zoom_enablePWM(TRUE); //set PWM speed
        }

        LENS_MSG(("LENS_M: lensST8003_zoom_ctrlRelease: already idle state\r\n"));
        return lensST8003_zoom_getCurrentSection();
    }
    else if(g_pZoomPara.uiCurrentState != ZOOM_STATE_RUNNING)
    {
        LENS_ERR(("LENS_E: lensST8003_zoom_ctrlRelease: not in running state\r\n"));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        TargetSection=lensST8003_zoom_getTargetSection();

        lensST8003_zoom_gotoSection2(TargetSection, FALSE);

        if(lensST8003_zoom_gotoState(ZOOM_STATE_IDLE) != ERR_OK)
        {
            LENS_ERR(("LENS_E: lensST8003_zoom_ctrlRelease: go to IDLE state fail\r\n"));
            return ERR_OPERATION_FAIL;
        }

        if(g_pZoomPara.Direction == DIRECTION_BWD)
        {
            lensST8003_zoom_enablePWM(FALSE); //set full speed

            TimerDelayMs(100);//harry
            lensST8003_zoom_doBL();

            lensST8003_zoom_enablePWM(TRUE); //set PWM speed
        }

        if(lensST8003_zoom_gotoState(ZOOM_STATE_IDLE) != ERR_OK)
        {
            LENS_ERR(("LENS_E: lensST8003_zoom_ctrlRelease: go to IDLE state fail[2]\r\n"));
            return ERR_OPERATION_FAIL;
        }
    }
    else
    {
        lensST8003_task_doZoomBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
            lensST8003_zoom_changeState();
    }

    TimerDelayMs(200);

    //???? Zoom_OnOff(OFF);
    //???? Lens_OnOff(OFF); //motor power control. 0601

    //AO835@JJ@20080115 OpticalZoom Callback to Draw
    {
        lensST8003_task_doZoomCB();
    }
    //~AO835@JJ@20080115 OpticalZoom Callback to Draw

    return lensST8003_zoom_getCurrentSection();

}

static INT32 lensST8003_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    UINT32 Step_Ori,Step_New;

    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
    {
        LENS_ERR(("LENS_E: %s, not in idle state\r\n", __func__));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(Zoom_IN == LENS_ZOOM_IN &&
           lensST8003_zoom_getCurrentSection()<ZOOM_SECTION_TELE)
        {
            lensST8003_zoom_gotoSection2(ZOOM_SECTION_TELE, FALSE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8003_zoom_changeState();
        }
        else if(Zoom_IN == LENS_ZOOM_OUT &&
                lensST8003_zoom_getCurrentSection() >ZOOM_SECTION_WIDE)
        {
            lensST8003_zoom_gotoSection2(ZOOM_SECTION_WIDE, FALSE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8003_zoom_changeState();
        }
        else
            return ERR_OPERATION_FAIL;
    }
    else
    {
        if(Zoom_IN == LENS_ZOOM_IN)
        {
            lensST8003_zoom_runStep(DIRECTION_FWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8003_zoom_changeState();
        }
        else
        {
            lensST8003_zoom_runStep(DIRECTION_BWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8003_zoom_changeState();
        }
    }
    //[Chris Chung] Check if the lens is moving. If it isn't, then return fail.
    Step_Ori = g_pZoomPara.ZoomStep;
    lensST8003_task_doZoomCB();
    TimerDelayMs(40);
    lensST8003_task_doZoomCB();
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

static void lensST8003_zoom_setAutoBoundaryCheck(BOOL en)
{
    g_pZoomPara.bZoomBoundaryAutoCheck=en;
}

static INT32 lensST8003_zoom_retract(UINT32 param)
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

    LENS_ERR(("LENS_M: lensST8003: Zoom Retract\r\n"));

    lensST8003_zoom_enablePWM(FALSE); //set full speed

    /* Enter ZOOM_STATE_CLOSING */
    lensST8003_zoom_gotoState(ZOOM_STATE_CLOSING);

    lensST8003_focus_gotoGarage();

    if(g_pZoomPara.ZoomStep<=g_pZoomSectionPulse[0])
    {
        LENS_ERR(("LENS_M: Zoom Already Home!!\r\n"));

        /* Enter ZOOM_STATE_CLOSED */
        lensST8003_zoom_changeState();

        return ERR_OK;
    }

    lensST8003_task_doZoomReverse();
    lensST8003_zoom_gotoState(ZOOM_STATE_CLOSING_HOME);

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

void lensST8003_zoom_enablePWM(BOOL en)
{
}

static void lensST8003_zoom_getBL_step1(void)
{
    lensST8003_zoom_retract(0);

    TimerDelayMs(500);

    lensST8003_zoom_gotoState(ZOOM_STATE_GETBL1);

    lensST8003_task_doZoomForward();
}

static void lensST8003_zoom_getBL_step2(void)
{
    lensST8003_zoom_gotoState(ZOOM_STATE_GETBL4);

    g_pZoomPara.ZoomBLStep2 = 0;

    lensST8003_task_doZoomReverse();
}

static UINT32 lensST8003_zoom_getBL(void)
{
    lensST8003_zoom_getBL_step1();
    TimerDelayMs(1000);

    lensST8003_zoom_getBL_step2();
    TimerDelayMs(1000);

    LENS_ERR(("LENS_M: %s: BLStep = %d, BLStep2 = %d\r\n",
              __func__, g_pZoomPara.ZoomBLStep, g_pZoomPara.ZoomBLStep2));

    if(g_pZoomPara.ZoomBLStep2 > g_pZoomPara.ZoomBLStep)
        return (g_pZoomPara.ZoomBLStep2 - g_pZoomPara.ZoomBLStep);
    else
        return (g_pZoomPara.ZoomBLStep - g_pZoomPara.ZoomBLStep2);
}

static void lensST8003_zoom_setBL(UINT32 BLstep)
{
    if(BLstep > 0 && BLstep <= ZOOM_BL_STEP_LIMIT)
        g_pZoomPara.uiZoomBL = BLstep;
    else
        g_pZoomPara.uiZoomBL = ZOOM_BL_STEP;

    /* The following valus is experimental. */
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

static void lensST8003_focusIsr(void)
{
    if(!g_pFocusPara.bFocusIsr)
        return;

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

static void lensST8003_focus_setMotorDriver(unsigned char phase)
{
    if(g_pST8003Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8003: pMotor is null\r\n"));
        return;
    }

    g_pST8003Obj.pMotor->focus_setState(g_pFocusStepperPhase[phase], 0);
}

static void lensST8003_focus_doBL(void)
{
    lensST8003_focus_runStep(DIRECTION_BWD, FOCUS_BL_STEP);

    TimerDelayMs(FOCUS_INIT_DELAY);

    lensST8003_focus_runStep(DIRECTION_FWD, FOCUS_BL_STEP);
}

static void lensST8003_focus_initPhase(void)
{
    lensST8003_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);   //restore original phase

    TimerDelayMs(FOCUS_INIT_DELAY);         //init pulse 10ms
}

static void lensST8003_focus_runStep2(UINT8 dir, UINT16 step, BOOL bFullSpeed)
{
    unsigned long Count = 0;

    while(Count++ < step)
    {
        //if(dir==DIRECTION_FWD)
        if(dir==DIRECTION_BWD)//JJ1225
        {
            if(g_pFocusPara.uiFocusPhase >= (FOCUS_TOTAL_PHASE-1))
                g_pFocusPara.uiFocusPhase = 0;
            else
                g_pFocusPara.uiFocusPhase++;
            lensST8003_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);
            //g_pZoomPara.iFocusPosition+=1;
            g_pFocusPara.iFocusPosition-=1;//JJ1225
            g_pFocusPara.iFocusTestPos-=1;
        }
        else
        {
            if(g_pFocusPara.uiFocusPhase == 0)
                g_pFocusPara.uiFocusPhase = (FOCUS_TOTAL_PHASE-1);
            else
                g_pFocusPara.uiFocusPhase--;

            lensST8003_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);
            //g_pZoomPara.iFocusPosition-=1;
            g_pFocusPara.iFocusPosition+=1;//JJ1225
            g_pFocusPara.iFocusTestPos+=1;
        }

        if(bFullSpeed)
            TimerDelayUs(FOCUS_SPEED);
        else
            TimerDelayUs(FOCUS_SPEED_SLOW);
    }
}

static void lensST8003_focus_runStep(UINT8 dir, UINT16 step)
{
    lensST8003_focus_runStep2(dir, step, TRUE);
}

static void lensST8003_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position)
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
        lensST8003_focus_initPhase();

    lensST8003_focus_runStep(Dir,Step);

    if(Dir == DIRECTION_BWD)
        lensST8003_focus_doBL();

    if(category & FOCUS_POSTEXC)
        TimerDelayMs(FOCUS_INIT_DELAY);
}

static INT32 lensST8003_focus_gotoHome(void)
{
    unsigned short sBoundary;

    if(g_pST8003Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8003: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    if(g_pST8003Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
    {
        LENS_ERR(("LENS_E: lensST8003: ZOOM is inside, focus can't goto home.\r\n"));
        return ERR_OPERATION_FAIL;
    }

    lensST8003_focus_initPhase();

    sBoundary = 0;
    while(g_pST8003Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensST8003_focus_runStep2(DIRECTION_BWD,10,TRUE);
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
    while(g_pST8003Obj.pMotor->focus_checkPI() == FOCUSPI_INSDIE_LEVEL)
    {
        lensST8003_focus_runStep2(DIRECTION_FWD,1,FALSE);
        sBoundary += 1;
        if(sBoundary > FOCUS_FWD_LIMIT_INSIDE_PI)
        {
            LENS_ERR(("LENS_E: B2: %d>%d",sBoundary, FOCUS_FWD_LIMIT_INSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    if(g_pFocusPara.bFocusIsr)
    {
        //debug_err(("############ chris: no ISR = %d\r\n", g_pFocusPara.iFocusTestPos));
        g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;
        g_pFocusPara.iFocusTestPos=100;
        g_pFocusPara.bFocusIsr = FALSE;
    }
    else
    {
        g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;
        g_pFocusPara.iFocusTestPos=100;
    }

    TimerDelayMs(FOCUS_INIT_DELAY);

    return ERR_OK;
}

static INT32 lensST8003_focus_gotoGarage(void)
{
    if(lensST8003_focus_gotoHome() == ERR_OK)
    {
        lensST8003_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, FOCUS_POS_GARAGE);
        return ERR_OK;
    }
    else
    {
        return ERR_OPERATION_FAIL;
    }
}

static INT16 lensST8003_focus_getCurrentPosition(void)
{
    return g_pFocusPara.iFocusPosition;
}

static INT16 lensST8003_focus_getMaxPosition(void)
{
    return FOCUS_POS_OUTER_STOP;
}

static UINT32 lensST8003_focus_getFL(UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pFocusLengthTbl[zoomSection];
}

static INT16 lensST8003_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    if(index > FOCUS_ZOOM_TABLE_COLUMN-1)
        return 0;

    return g_pFocusZoomTblDefault[zoomSection-1][index];
}

static UINT32 lensST8003_focus_getBL(void)
{
    UINT32 count = 0;

    lensST8003_zoom_gotoSection2(ZOOM_SECTION_TELE, TRUE);

    lensST8003_focus_gotoHome();

    lensST8003_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, FOCUS_BL_CALIBRATION_STEP);

    while(g_pST8003Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensST8003_focus_runStep(DIRECTION_BWD,1);
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

static INT32 lensST8003_aperture_gotoState(LENS_APERTURE_ACT newState)
{
    switch(newState)
    {
    case LENS_APERTURE_SMALL:
        //lensST8003_task_setApertureState(0x2);
        g_pST8003Obj.pMotor->aperture_setState(0x2);    // Directly set motor driver I/O instead of task.
        g_pIrisPara.uiCurrentState = newState;
        break;
    case LENS_APERTURE_BIG:
        //lensST8003_task_setApertureState(0x1);
        g_pST8003Obj.pMotor->aperture_setState(0x1);
        g_pIrisPara.uiCurrentState = newState;
        break;
    case LENS_APERTURE_OFF:
        //lensST8003_task_setApertureState(0x0);
        g_pST8003Obj.pMotor->aperture_setState(0x0);
        break;
    default:
        debug_err(("%s: aperture goto wrong state=%d\r\n", __func__, newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

static UINT32 lensST8003_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pApertureZoomTbl[zoomSection][irisPos];
}

static LENS_APERTURE_ACT lensST8003_aperture_getState(void)
{
    return g_pIrisPara.uiCurrentState;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensST8003_shutter_gotoState(LENS_SHUTTER_ACT newState)
{
    switch(newState)
    {
    case LENS_SHUTTER_CLOSE:
        //lensST8003_task_setShutterState(0x2);
        g_pST8003Obj.pMotor->shutter_setState(0x2);
        g_pShutterPara.uiCurrentState = newState;
        break;
    case LENS_SHUTTER_OPEN:
        //lensST8003_task_setShutterState(0x1);
        g_pST8003Obj.pMotor->shutter_setState(0x1);
        g_pShutterPara.uiCurrentState = newState;
        break;
    case LENS_SHUTTER_OFF:
        g_pST8003Obj.pMotor->shutter_setState(0x0);
        //lensST8003_task_setShutterState(0x0);
        break;
    default:
        debug_err(("%s: shutter goto wrong state=%d\r\n", __func__, newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

