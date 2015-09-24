/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       LensDrv_DZ5311.c
    @ingroup    mISYSLens

    @brief      A-optronics DZ5311 lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2011/07/13
*/

#include <stdio.h>
#include <stdlib.h>
#include "interrupt.h"
#include "Lens_int.h"
#include "LensDrv_DZ5311.h"
#include "LensDrv_DUMMY.h"
#include "gpio.h"
#include "debug.h"
#include "top.h"
#include "timer.h"
#include "Type.h"
#include "Utility.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for function declaration.
//--------------------------------------------------------------------------------------------------------------------------

//lens
static void     lensDZ5311_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensDZ5311_isFFLens(void);

//zoom
static INT32    lensDZ5311_zoom_initPart1(void);
static INT32    lensDZ5311_zoom_initPart2(void);
static INT32    lensDZ5311_zoom_ctrlRelease(void); // return current zoom section
static INT32    lensDZ5311_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN); // 1: zoom in, 0:zoom out
static INT32    lensDZ5311_zoom_ctrlPress_inner(LENS_ZOOM_ACT Zoom_IN);
static INT16    lensDZ5311_zoom_getCurrentPosition(void);
static UINT8    lensDZ5311_zoom_getCurrentSection(void);
static UINT8    lensDZ5311_zoom_getMaxSection(void);
static INT32    lensDZ5311_zoom_retract(UINT32 param);
static void     lensDZ5311_zoom_gotoSection(UINT8 section);
static INT32    lensDZ5311_zoom_gotoState(LENS_ZOOM_STATE newState);
static void     lensDZ5311_zoom_changeState(void);

//zoom ISR
static void     lensDZ5311_zoomIsr_enable(BOOL enable);
static void     lensDZ5311_zoomIsr(void);

//focus
static void     lensDZ5311_focusIsr(void);
static void     lensDZ5311_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position);
static INT32    lensDZ5311_focus_gotoHome(void);
static INT32    lensDZ5311_focus_gotoGarage(void);
static void     lensDZ5311_focus_runStep(UINT8 dir, UINT16 step);
static INT16    lensDZ5311_focus_getCurrentPosition(void);
static INT16    lensDZ5311_focus_getMaxPosition(void);
static INT16    lensDZ5311_focus_getMinPosition(void);
static UINT32   lensDZ5311_focus_getFL(UINT32 zoomSection);
static INT16    lensDZ5311_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index);
static UINT16   lensDZ5311_focus_getZoomTblMaxIdx(void);
static UINT16   lensDZ5311_focus_getCurrentDist(INT32 basicStep);
static void     lensDZ5311_focus_updateFocusDistIdx(void);
static UINT16   lensDZ5311_focus_getDiffStepByDist(UINT32 dist);
static INT16    lensDZ5311_focus_getBestPosition(UINT32 section, BOOL bUpdateBestTbl);
static void     lensDZ5311_focus_doBL(void);
static void     lensDZ5311_focus_setBasisStep(UINT32 section, INT32 farStep);

//aperture
static UINT32   lensDZ5311_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);

//shutter

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------

static ZOOM_Struct g_pZoomPara={0};
static FOCUS_Struct g_pFocusPara={0};

static UINT16 g_pZoomSectionPulse[(ZOOM_SECTION_TELE+1)]=
{
#if (ZOOM_SECTION_TELE == 25)
    ZOOM_POS_GARAGE,
    ZOOM_POS_WIDE,
    ZOOM_POS_WIDE + 18,  // 1.2
    ZOOM_POS_WIDE + 36,  // 1.4
    ZOOM_POS_WIDE + 54,  // 1.6
    ZOOM_POS_WIDE + 72,  // 1.8
    ZOOM_POS_WIDE + 90,
    ZOOM_POS_WIDE + 114, // 2.25
    ZOOM_POS_WIDE + 138, // 2.5
    ZOOM_POS_WIDE + 162, // 2.75
    ZOOM_POS_WIDE + 180,
    ZOOM_POS_WIDE + 210, // 3.33
    ZOOM_POS_WIDE + 240, // 3.66
    ZOOM_POS_WIDE + 270,
    ZOOM_POS_WIDE + 315, // 4.5
    ZOOM_POS_WIDE + 360,
    ZOOM_POS_WIDE + 405, // 5.5
    ZOOM_POS_WIDE + 451,
    ZOOM_POS_WIDE + 496, // 6.5
    ZOOM_POS_WIDE + 541,
    ZOOM_POS_WIDE + 586, // 7.5
    ZOOM_POS_WIDE + 631,
    ZOOM_POS_WIDE + 676, // 8.5
    ZOOM_POS_WIDE + 721,
    ZOOM_POS_WIDE + 766, // 9.5
    ZOOM_POS_TELE
#else
    ZOOM_POS_GARAGE,
    ZOOM_POS_WIDE,
    ZOOM_POS_WIDE + 90,
    ZOOM_POS_WIDE + 180,
    ZOOM_POS_WIDE + 270,
    ZOOM_POS_WIDE + 360,
    ZOOM_POS_WIDE + 451,
    ZOOM_POS_WIDE + 541,
    ZOOM_POS_WIDE + 631,
    ZOOM_POS_WIDE + 721,
    ZOOM_POS_TELE
#endif
};

static INT16 g_pFocusZoomTblDefault[ZOOM_SECTION_TELE][FOCUS_ZOOM_TABLE_COLUMN]={
#if (ZOOM_SECTION_TELE == 25)
//  Inf, 8.0m, 3.8m, 2.5m, 1.7m, 1.0m, 0.7m, 0.6m, 0.5m, 0.2m
{    12,   9 ,   6 ,   4 ,   0 ,  -8 , -17 , -24 , -32 , -110 },
{    12,   9 ,   6 ,   4 ,   0 ,  -8 , -17 , -24 , -32 , -110 },  // 1.2
{    12,   9 ,   6 ,   4 ,   0 ,  -8 , -17 , -24 , -32 , -110 },  // 1.4
{    12,   9 ,   6 ,   4 ,   0 ,  -8 , -17 , -24 , -32 , -110 },  // 1.6
{    12,   9 ,   6 ,   4 ,   0 ,  -8 , -17 , -24 , -32 , -110 },  // 1.8
{   190, 188 , 185 , 182 , 178 , 170 , 162 , 154 , 146 ,   96 },
{   190, 188 , 185 , 182 , 178 , 170 , 162 , 154 , 146 ,   96 },  // 2.25
{   190, 188 , 185 , 182 , 178 , 170 , 162 , 154 , 146 ,   96 },  // 2.5
{   190, 188 , 185 , 182 , 178 , 170 , 162 , 154 , 146 ,   96 },  // 2.75
{   297, 295 , 292 , 289 , 285 , 277 , 269 , 261 , 253 ,  202 },
{   297, 295 , 292 , 289 , 285 , 277 , 269 , 261 , 253 ,  202 },  // 3.33
{   297, 295 , 292 , 289 , 285 , 277 , 269 , 261 , 253 ,  202 },  // 3.66
{   358, 355 , 352 , 350 , 346 , 338 , 329 , 322 , 314 ,  263 },
{   358, 355 , 352 , 350 , 346 , 338 , 329 , 322 , 314 ,  263 },  // 4.5
{   387, 384 , 381 , 378 , 375 , 366 , 358 , 350 , 343 ,  292 },
{   387, 384 , 381 , 378 , 375 , 366 , 358 , 350 , 343 ,  292 },  // 5.5
{   393, 390 , 387 , 384 , 381 , 372 , 364 , 356 , 349 ,  298 },
{   393, 390 , 387 , 384 , 381 , 372 , 364 , 356 , 349 ,  298 },  // 6.5
{   382, 379 , 376 , 374 , 370 , 362 , 353 , 346 , 338 ,  287 },
{   382, 379 , 376 , 374 , 370 , 362 , 353 , 346 , 338 ,  287 },  // 7.5
{   358, 355 , 353 , 350 , 346 , 338 , 329 , 322 , 314 ,  263 },
{   358, 355 , 353 , 350 , 346 , 338 , 329 , 322 , 314 ,  263 },  // 8.5
{   324, 322 , 319 , 316 , 312 , 304 , 296 , 288 , 280 ,  229 },
{   324, 322 , 319 , 316 , 312 , 304 , 296 , 288 , 280 ,  229 },  // 9.5
{   283, 280 , 278 , 275 , 271 , 263 , 254 , 247 , 239 ,  188 }
#else
//  Inf, 8.0m, 3.8m, 2.5m, 1.7m, 1.0m, 0.7m, 0.6m, 0.5m, 0.2m
{    12,   9 ,   6 ,   4 ,   0 ,  -8 , -17 , -24 , -32 , -110 },
{   190, 188 , 185 , 182 , 178 , 170 , 162 , 154 , 146 ,   96 },
{   297, 295 , 292 , 289 , 285 , 277 , 269 , 261 , 253 ,  202 },
{   358, 355 , 352 , 350 , 346 , 338 , 329 , 322 , 314 ,  263 },
{   387, 384 , 381 , 378 , 375 , 366 , 358 , 350 , 343 ,  292 },
{   393, 390 , 387 , 384 , 381 , 372 , 364 , 356 , 349 ,  298 },
{   382, 379 , 376 , 374 , 370 , 362 , 353 , 346 , 338 ,  287 },
{   358, 355 , 353 , 350 , 346 , 338 , 329 , 322 , 314 ,  263 },
{   324, 322 , 319 , 316 , 312 , 304 , 296 , 288 , 280 ,  229 },
{   283, 280 , 278 , 275 , 271 , 263 , 254 , 247 , 239 ,  188 }
#endif
};

static INT16 g_pFocusZoomTblBasisStep[ZOOM_SECTION_TELE] = {0};
static INT16 g_pFocusZoomTblBest[ZOOM_SECTION_TELE] = {0};

static UINT16 g_pFocusZoomTblDist[FOCUS_ZOOM_TABLE_COLUMN]=
{ 0xffff, 80, 38, 25, 17, 10, 7, 6, 5, 2 };

static UINT16 g_pApertureZoomTbl[(ZOOM_SECTION_TELE+1)] = {
#if (ZOOM_SECTION_TELE == 25)
    0,
    36,
    37,  // 1.2
    37,  // 1.4
    38,  // 1.6
    39,  // 1.8
    39,
    40,  // 2.25
    41,  // 2.5
    42,  // 2.75
    43,
    44,  // 3.33
    45,  // 3.66
    46,
    47,  // 4.5
    49,
    50,  // 5.5
    52,
    53,  // 6.5
    55,
    56,  // 7.5
    58,
    59,  // 8.5
    61,
    62,  // 9.5
    64
#else
    0,
    36,
    39,
    43,
    46,
    49,
    52,
    55,
    58,
    61,
    64
#endif
};

static UINT16 g_pFocusLengthTbl[(ZOOM_SECTION_TELE+1)] = {
#if (ZOOM_SECTION_TELE == 25)
    0,
    75,
    78,  // 1.2
    81,  // 1.4
    85,  // 1.6
    88,  // 1.8
    89,
    93,  // 2.25
    96,  // 2.5
    99,  // 2.75
    103,
    108, // 3.33
    113, // 3.66
    117,
    125, // 4.5
    131,
    138, // 5.5
    145,
    152, // 6.5
    159,
    166, // 7.5
    173,
    180, // 8.5
    187,
    194, // 9.5
    201
#else
    0,
    75,
    89,
    103,
    117,
    131,
    145,
    159,
    173,
    187,
    201
#endif
};

static LENS_TAB g_pDZ5311Obj =
{
    //Lens
    lensDZ5311_init,
    lensDZ5311_isFFLens,
    //Zoom
    lensDZ5311_zoom_initPart1,
    lensDZ5311_zoom_initPart2,
    lensDZ5311_zoom_ctrlRelease,
    lensDZ5311_zoom_ctrlPress,
    lensDZ5311_zoom_ctrlPress_inner,
    lensDZ5311_zoom_getCurrentPosition,
    lensDZ5311_zoom_getCurrentSection,
    lensDZ5311_zoom_getMaxSection,
    lensDUMMY_zoom_getRatio,
    lensDZ5311_zoom_retract,
    lensDZ5311_zoom_gotoSection,
    lensDUMMY_zoom_setAutoBoundaryCheck,
    lensDZ5311_zoomIsr_enable,
    lensDUMMY_zoom_getBL,
    lensDUMMY_zoom_setBL,
    lensDUMMY_zoom_doBL,
    //Focus
    lensDZ5311_focus_gotoPosition,
    lensDZ5311_focus_gotoHome,
    lensDZ5311_focus_gotoGarage,
    lensDZ5311_focus_getCurrentPosition,
    lensDZ5311_focus_getMaxPosition,
    lensDZ5311_focus_getMinPosition,
    lensDZ5311_focus_getFL,
    lensDZ5311_focus_getZoomTblValue,
    lensDZ5311_focus_getZoomTblMaxIdx,
    lensDZ5311_focus_getCurrentDist,
    lensDZ5311_focus_getDiffStepByDist,
    lensDUMMY_focus_getBL,
    lensDUMMY_focus_setBL,
    lensDZ5311_focus_setBasisStep,
    //Aperture
    lensDUMMY_aperture_gotoState,
    lensDUMMY_aperture_getState,
    lensDZ5311_aperture_getFNO,
    //Shutter
    lensDUMMY_shutter_gotoState,
    lensDUMMY_shutter_getPulsePeriod,
    //Others
    NULL
};

static BOOL bUpdateHomeDiff = FALSE;
UINT32 focusHomeDiff = 999;
UINT32 zoomHomeDiff = 999;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensDZ5311_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    LENS_MSG(("LENS_M: %s\r\n", __func__));

    g_pDZ5311Obj.pMotor = pMotorTab;

    //Zoom ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID, GPIO_INTTYPE_EDGE, GPIO_INTPOL_POSHIGH);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID, lensDZ5311_zoomIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID);
    g_pZoomPara.bZoomIsr = TRUE;

    //Focus ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID2, GPIO_INTTYPE_EDGE, GPIO_INTPOL_POSHIGH);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID2, lensDZ5311_focusIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID2);
    g_pFocusPara.bFocusIsr = TRUE;
}

/**
    Get DZ5311 lens object

    @return DZ5311 lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    return &g_pDZ5311Obj;
}

BOOL lensDZ5311_isFFLens(void)
{
    return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with LensTask.
//--------------------------------------------------------------------------------------------------------------------------
static void lensDZ5311_task_zoomForward(UINT32 step, BOOL bWaitZoomFinish, BOOL bOpenZoomPR)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);

    LensCtrl_GetParam(&lensCtrlParamObj);

    lensCtrlParamObj.uiZoomStep      = step;
    lensCtrlParamObj.bWaitZoomFinish = bWaitZoomFinish;
    lensCtrlParamObj.bOpenZoomPR     = bOpenZoomPR;

    LensCtrl_SetParam(&lensCtrlParamObj);

    /* Set target step that will be checked in zoom ISR. */
    g_pZoomPara.ZoomTargetStep = g_pZoomPara.ZoomStep + step;
    g_pZoomPara.Direction = DIRECTION_FWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD);

    if(bWaitZoomFinish)
        LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);
}

static void lensDZ5311_task_zoomReverse(UINT32 step, BOOL bWaitZoomFinish, BOOL bOpenZoomPR)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);

    LensCtrl_GetParam(&lensCtrlParamObj);

    lensCtrlParamObj.uiZoomStep      = step;
    lensCtrlParamObj.bWaitZoomFinish = bWaitZoomFinish;
    lensCtrlParamObj.bOpenZoomPR     = bOpenZoomPR;

    LensCtrl_SetParam(&lensCtrlParamObj);

    /* Set target step that will be checked in zoom ISR. */
    g_pZoomPara.ZoomTargetStep = g_pZoomPara.ZoomStep - step;
    g_pZoomPara.Direction = DIRECTION_BWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMREV);

    if(bWaitZoomFinish)
        LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);
}

static void lensDZ5311_task_zoomBrake(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMBRK);
}

static void lensDZ5311_task_doCallBack(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMCHANGE);
}

static void lensDZ5311_task_focusForward(UINT32 step)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);

    LensCtrl_GetParam(&lensCtrlParamObj);

    lensCtrlParamObj.uiFocusStep = step;

    LensCtrl_SetParam(&lensCtrlParamObj);

    /* Set target step that will be checked in focus ISR. */
    g_pFocusPara.iTargetStep = g_pFocusPara.iFocusPosition + step;
    g_pFocusPara.Direction = DIRECTION_FWD;
    set_flg(FLG_ID_LENS, FLGLENS_FOCUSFWD);
    LensCtrl_WaitCmdTypeFinish(LENSCTRL_WAIT_INFINITE, FLGLENS_FOCUS_CMD_FIN2);
}

static void lensDZ5311_task_focusReverse(UINT32 step)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);

    LensCtrl_GetParam(&lensCtrlParamObj);

    lensCtrlParamObj.uiFocusStep = step;

    LensCtrl_SetParam(&lensCtrlParamObj);

    /* Set target step that will be checked in focus ISR. */
    g_pFocusPara.iTargetStep = g_pFocusPara.iFocusPosition - step;
    g_pFocusPara.Direction = DIRECTION_BWD;
    set_flg(FLG_ID_LENS, FLGLENS_FOCUSREV);
    LensCtrl_WaitCmdTypeFinish(LENSCTRL_WAIT_INFINITE, FLGLENS_FOCUS_CMD_FIN2);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom ISR.
//--------------------------------------------------------------------------------------------------------------------------
static void lensDZ5311_zoomIsr_enable(BOOL enable)
{
    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensDZ5311: pMotor is null\r\n"));
        return;
    }

    g_pZoomPara.bZoomIsr = enable;
    g_pDZ5311Obj.pMotor->zoom_setPIInt(enable);
}

static void lensDZ5311_zoomIsr(void)
{
    static UINT32 uiMOB2 = 0;
    PLENSCTRL_APPOBJ pLensCtrlObj = LensCtrl_GetObject();

    if(!g_pZoomPara.bZoomIsr)
        return;

    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensDZ5311: pMotor is null\r\n"));
        return;
    }

    if(g_pDZ5311Obj.pMotor->zoom_checkPIInt())
    {
        g_pDZ5311Obj.pMotor->zoom_clearPIInt();
        uiMOB2 = (uiMOB2+1)%2;  // 2 MOB count = 1 zoom step

        if(uiMOB2 == 0)
        {
            if(g_pZoomPara.Direction == DIRECTION_FWD)
            {
                g_pZoomPara.ZoomStep++;
            }
            else
            {
                g_pZoomPara.ZoomStep--;
            }

            /* Check if target step and final step match and set Zoom Cmd finish. */
            if(g_pZoomPara.ZoomTargetStep == g_pZoomPara.ZoomStep)
                set_flg(FLG_ID_LENS, FLGLENS_ZOOM_CMD_FIN);

            switch(g_pZoomPara.uiCurrentState)
            {
            case ZOOM_STATE_NULL:
            case ZOOM_STATE_INSIDEINIT:
            case ZOOM_STATE_OUTSIDEINIT:
            case ZOOM_STATE_INITPART1:
                lensDZ5311_zoom_changeState();
                break;
            case ZOOM_STATE_INITPART2:
            case ZOOM_STATE_IDLE:
                //do nothing
                break;
            case ZOOM_STATE_RUNNING:
                if(g_pZoomPara.uiLastSection != lensDZ5311_zoom_getCurrentSection())
                {
                    g_pZoomPara.uiLastSection = lensDZ5311_zoom_getCurrentSection();
                    #if 0
                    lensDZ5311_task_doCallBack();
                    #else
                    if(pLensCtrlObj->fpLensCB != NULL)
                    {
                        pLensCtrlObj->fpLensCB(ALGMSG_CHGOZOOMSTEP, NULL);
                    }
                    #endif
                }

                #if 0
                if((g_pZoomPara.ZoomStep % 4) == 0)
                {
                    lensDZ5311_task_doZoomCAF();
                }
                #endif
                break;
            case ZOOM_STATE_CLOSING:
            case ZOOM_STATE_CLOSED:
                //do nothing
                break;
            case ZOOM_STATE_FAIL:
                LENS_ERR(("LENS_E: lensST8002: zoom isr fail state\r\n"));
                break;
            default:
                LENS_ERR(("LENS_E: lensST8002: zoom isr wrong state = %d\r\n", g_pZoomPara.uiCurrentState));
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensDZ5311_zoom_initPart1(void)
{
    LENS_ERR(("LENS_M: lensDZ5311: zoom initPart1\r\n"));

    g_pZoomPara.uiCurrentState = ZOOM_STATE_NULL;

    /* Change State from ZOOM_STATE_NULL to ZOOM_STATE_INITPART2 */
    lensDZ5311_zoom_changeState();

    return ERR_OK;
}

static INT32 lensDZ5311_zoom_initPart2(void)
{
    if(lensDZ5311_zoom_gotoState(ZOOM_STATE_INITPART2) != ERR_OK)
        return ERR_OPERATION_FAIL;

    if(g_pZoomPara.ZoomStep > ZOOM_POS_HOME_PR)
        lensDZ5311_task_zoomReverse(g_pZoomPara.ZoomStep - ZOOM_POS_HOME_PR, TRUE, FALSE);

    LENS_ERR(("LENS_M: lensDZ5311: zoom initPart2 finish, cur ZoomStep=%d\r\n", g_pZoomPara.ZoomStep));

    /* Enter ZOOM_STATE_IDLE */
    lensDZ5311_zoom_changeState();

    g_pZoomPara.uiLastSection = lensDZ5311_zoom_getCurrentSection();

    return ERR_OK;
}

/* Only zoom_gotoState and zoom_changeState can modify the value
       of g_pZoomPara.uiCurrentState. */
static INT32 lensDZ5311_zoom_gotoState(LENS_ZOOM_STATE newState)
{
    UINT16 cnt=0;
    UINT16 FocusTimeOut=FOCUS_SPEED_PER_STEP * (FOCUS_BWD_LIMIT_OUTSIDE_PI + FOCUS_BL_STEP*2 + 100);   //100 is buffer.
    UINT16 ZoomTimeOut=ZOOM_SPEED_PER_STEP * (g_pZoomSectionPulse[ZOOM_SECTION_TELE] + ZOOM_BL_STEP*2 + 100);   //100 is buffer.

    switch(newState)
    {
    case ZOOM_STATE_CLOSING:
        LENS_MSG(("LENS_M: /* Current State=%d --> ZOOM_STATE_CLOSING */\r\n", g_pZoomPara.uiCurrentState));

        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSING;
        break;

    case ZOOM_STATE_CLOSED:
        LENS_MSG(("LENS_M: /* Current State=%d --> ZOOM_STATE_CLOSED */\r\n", g_pZoomPara.uiCurrentState));

        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSED;
        break;

    default:

        while(g_pZoomPara.uiCurrentState != newState)
        {
            TimerDelayMs(1);
            if(cnt++>(FocusTimeOut+ZoomTimeOut))
            {
                LENS_ERR(("LENS_E: lensDZ5311: Zoom Init Timeout, stop state=%d, cur step=%d\r\n", g_pZoomPara.uiCurrentState, g_pZoomPara.ZoomStep));

                lensDZ5311_task_zoomBrake();
                g_pZoomPara.uiCurrentState = ZOOM_STATE_FAIL;
                return ERR_OPERATION_FAIL;
            }
        }
    }

    return ERR_OK;
}

static void lensDZ5311_zoom_changeState(void)
{
    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: %s: pMotor is null\r\n", __func__));
        return;
    }

    switch(g_pZoomPara.uiCurrentState)
    {
    case ZOOM_STATE_NULL:

        if(g_pZoomPara.bZoomIsr)
        {
            if(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

                LENS_ERR(("LENS_M: Focus retract\r\n"));
                g_pDZ5311Obj.pMotor->changeMode(MOTOR_MODE_FOCUS);
                lensDZ5311_focus_gotoHome();
                LENS_ERR(("LENS_M: Zoom Out\r\n"));
                g_pDZ5311Obj.pMotor->changeMode(MOTOR_MODE_ZOOM);
                lensDZ5311_task_zoomReverse(ZOOM_POS_OUTER_STOP, FALSE, TRUE);
            }
            else
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_INSIDEINIT */\r\n"));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

                LENS_ERR(("LENS_M: Zoom In, state=%d\r\n", g_pZoomPara.uiCurrentState));

                lensDZ5311_task_zoomForward(ZOOM_FWD_LIMIT_INSIDE_PI, FALSE, TRUE); // 2011-03-17 Chris
            }
        }
        else
        {
            LENS_ERR(("LENS_E: /* ZOOM_STATE_NULL --> ZOOM_STATE_FAIL */\r\n"));
            g_pZoomPara.uiCurrentState = ZOOM_STATE_FAIL;
        }
        break;

    case ZOOM_STATE_FAIL:
        break;

    case ZOOM_STATE_INSIDEINIT:

        if(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;

            lensDZ5311_task_zoomBrake();

            LENS_MSG(("LENS_M: /* ZOOM_STATE_INSIDEINIT --> ZOOM_STATE_INITPART2 */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART2;
        }
        break;

    case ZOOM_STATE_OUTSIDEINIT:

        if(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_OUTSIDEINIT --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            lensDZ5311_task_zoomForward(ZOOM_POS_OUTER_STOP, FALSE, TRUE);
        }
        break;

    case ZOOM_STATE_INITPART1:
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

        LENS_MSG(("LENS_M: /* ZOOM_STATE_CLOSING --> ZOOM_STATE_CLOSED */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSED;

        break;
    case ZOOM_STATE_CLOSED:
        //do nothing
        break;
    default:
        LENS_ERR(("LENS_E: lensDZ5311: zoom change unknown state\r\n"));
    }
}

static void lensDZ5311_zoom_doBL(void)
{
    lensDZ5311_task_zoomReverse(ZOOM_BL_STEP, TRUE, FALSE);
    lensDZ5311_task_zoomForward(ZOOM_BL_STEP, TRUE, FALSE);
}

static void lensDZ5311_zoom_gotoSection(UINT8 section)
{
    UINT32 step;

    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
    {
        step = g_pZoomPara.ZoomStep - g_pZoomSectionPulse[section];
        lensDZ5311_task_zoomReverse(step, TRUE, FALSE);
        lensDZ5311_zoom_doBL();

        if(g_pZoomPara.bZoomIsr == FALSE)
            g_pZoomPara.ZoomStep -= step;
    }
    else if(g_pZoomPara.ZoomStep < g_pZoomSectionPulse[section])
    {
        step = g_pZoomSectionPulse[section] - g_pZoomPara.ZoomStep;
        lensDZ5311_task_zoomForward(step, TRUE, FALSE);

        if(g_pZoomPara.bZoomIsr == FALSE)
            g_pZoomPara.ZoomStep += step;
    }

    lensDZ5311_task_doCallBack();
}

static INT16 lensDZ5311_zoom_getCurrentPosition(void)
{
    return g_pZoomPara.ZoomStep;
}

static UINT8 lensDZ5311_zoom_getCurrentSection(void)
{
    unsigned char i;
    for(i=ZOOM_SECTION_WIDE; i<ZOOM_SECTION_TELE; i++)
    {
        if(g_pZoomPara.ZoomStep <= ((g_pZoomSectionPulse[i]+g_pZoomSectionPulse[i+1])/2))
            break;
    }
    //debug_err(("lensDZ5311_zoom_getCurrentSection = %d[%d]\n\r",i,g_pZoomPara.ZoomStep));
    return i;
}

static UINT8 lensDZ5311_zoom_getMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}

static INT32 lensDZ5311_zoom_ctrlRelease(void) // return current section
{
    UINT32 curSection;

    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: %s: pMotor is null\r\n", __func__));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
    {
        LENS_ERR(("LENS_M: %s: zoom is already idle\r\n", __func__));
        return lensDZ5311_zoom_getCurrentSection();
    }

    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);

    if(g_pZoomPara.Direction == DIRECTION_BWD)
    {
        lensDZ5311_zoom_doBL();
    }

    if(g_pFocusPara.Direction == DIRECTION_FWD)
    {
        lensDZ5311_focus_doBL();
    }

    curSection = lensDZ5311_zoom_getCurrentSection();

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
        lensDZ5311_zoom_changeState();

    return curSection;
}

static INT32 lensDZ5311_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
    {
        LENS_MSG(("LENS_E: %s: not idle state!!, cur=%d\r\n", __func__, g_pZoomPara.uiCurrentState));
        return ERR_OPERATION_FAIL;
    }

    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);

    /* Update focus distance index and the g_pFocusZoomTblBest table before zoom in/out. */
    lensDZ5311_focus_updateFocusDistIdx();
    lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, lensDZ5311_focus_getBestPosition(lensDZ5311_zoom_getCurrentSection(), TRUE));

    if(Zoom_IN == LENS_ZOOM_IN)
        set_flg(FLG_ID_LENS, FLGLENS_ZOOM_FOCUS_MOVE_IN);
    else
        set_flg(FLG_ID_LENS, FLGLENS_ZOOM_FOCUS_MOVE_OUT);

    /* ZOOM_STATE_IDLE -> ZOOM_STATE_RUNNING */
    lensDZ5311_zoom_changeState();

    return ERR_OK;
}

/* zoom_ctrlPress_inner is called in LensCtrlTsk for continous zoom */
static INT32 lensDZ5311_zoom_ctrlPress_inner(LENS_ZOOM_ACT Zoom_IN)
{
    UINT32 zoomDir, zoomPPS, zoomStep, curZoomSection;
    UINT32 focusDir, focusPPS, focusStep;
    MOTOR_INFO_TAB lensCtrlMotorInfoTab = {0};

    curZoomSection = lensDZ5311_zoom_getCurrentSection();

    /* Since zoom and focus will move together, we should calculate
           the step and speed of zoom and focus for every zoom section. */
    if(Zoom_IN == LENS_ZOOM_IN)
    {
        if(lensDZ5311_zoom_getCurrentSection()<ZOOM_SECTION_TELE)
        {
            zoomDir = MOTOR_ZOOM_FWD;
            zoomStep = g_pZoomSectionPulse[lensDZ5311_zoom_getCurrentSection()+1] - g_pZoomPara.ZoomStep;

            g_pZoomPara.ZoomTargetStep = g_pZoomPara.ZoomStep + zoomStep;
            g_pZoomPara.Direction = DIRECTION_FWD;

            if(lensDZ5311_focus_getBestPosition(curZoomSection+1, FALSE) >= lensDZ5311_focus_getBestPosition(curZoomSection, FALSE))
            {
                focusStep = lensDZ5311_focus_getBestPosition(curZoomSection+1, FALSE) - lensDZ5311_focus_getBestPosition(curZoomSection, FALSE);
                focusDir = MOTOR_FOCUS_FWD;

                g_pFocusPara.iTargetStep = g_pFocusPara.iFocusPosition + focusStep;
                g_pFocusPara.Direction = focusDir;
            }
            else
            {
                focusStep = lensDZ5311_focus_getBestPosition(curZoomSection, FALSE) - lensDZ5311_focus_getBestPosition(curZoomSection+1, FALSE);
                focusDir = MOTOR_FOCUS_BWD;

                g_pFocusPara.iTargetStep = g_pFocusPara.iFocusPosition - focusStep;
                g_pFocusPara.Direction = focusDir;
            }

            if(zoomStep < focusStep)
            {
                focusPPS = FOCUS_MAX_SPEED;

                zoomPPS = focusPPS*zoomStep/focusStep;

                if(zoomPPS > ZOOM_MAX_SPEED)
                    zoomPPS = ZOOM_MAX_SPEED;
                if(zoomPPS < ZOOM_MIN_SPEED)
                    zoomPPS = ZOOM_MIN_SPEED;

                if(zoomPPS == ZOOM_MAX_SPEED)
                {
                    focusPPS = zoomPPS*focusStep/zoomStep;

                    if(focusPPS < FOCUS_MIN_SPEED)
                        focusPPS = FOCUS_MIN_SPEED;
                }
            }
            else
            {
                zoomPPS = ZOOM_MAX_SPEED;

                focusPPS = zoomPPS*focusStep/zoomStep;

                if(focusPPS < FOCUS_MIN_SPEED)
                    focusPPS = FOCUS_MIN_SPEED;
            }

            if(focusStep <= 4)  // 2011-06-03 Chris
            {
                lensCtrlMotorInfoTab.bWaitZoomFinish = TRUE;
                lensCtrlMotorInfoTab.uiZoomPPS       = zoomPPS;
                g_pDZ5311Obj.pMotor->setMotorInfo(&lensCtrlMotorInfoTab);
                g_pDZ5311Obj.pMotor->zoom_setState(MOTOR_ZOOM_FWD, zoomStep);
                set_flg(FLG_ID_LENS, FLGLENS_ZOOM_CMD_FIN|FLGLENS_FOCUS_CMD_FIN);
            }
            else
            {
                lensCtrlMotorInfoTab.uiZoomDirection     = zoomDir;
                lensCtrlMotorInfoTab.uiZoomPPS           = zoomPPS;
                lensCtrlMotorInfoTab.uiZoomStep          = zoomStep;
                lensCtrlMotorInfoTab.uiFocusDirection    = focusDir;
                lensCtrlMotorInfoTab.uiFocusPPS          = focusPPS;
                lensCtrlMotorInfoTab.uiFocusStep         = focusStep;
                g_pDZ5311Obj.pMotor->setMotorInfo(&lensCtrlMotorInfoTab);
                g_pDZ5311Obj.pMotor->sif_doZoomFocusMove();
            }
        }
    }
    else
    {
        if(lensDZ5311_zoom_getCurrentSection()>ZOOM_SECTION_WIDE)
        {
            zoomDir = MOTOR_ZOOM_BWD;
            zoomStep = g_pZoomPara.ZoomStep - g_pZoomSectionPulse[lensDZ5311_zoom_getCurrentSection()-1];

            g_pZoomPara.ZoomTargetStep = g_pZoomPara.ZoomStep - zoomStep;
            g_pZoomPara.Direction = DIRECTION_BWD;

            if(lensDZ5311_focus_getBestPosition(curZoomSection-1, FALSE) >= lensDZ5311_focus_getBestPosition(curZoomSection, FALSE))
            {
                focusStep = lensDZ5311_focus_getBestPosition(curZoomSection-1, FALSE) - lensDZ5311_focus_getBestPosition(curZoomSection, FALSE);
                focusDir = MOTOR_FOCUS_FWD;

                g_pFocusPara.iTargetStep = g_pFocusPara.iFocusPosition + focusStep;
                g_pFocusPara.Direction = focusDir;
            }
            else
            {
                focusStep = lensDZ5311_focus_getBestPosition(curZoomSection, FALSE) - lensDZ5311_focus_getBestPosition(curZoomSection-1, FALSE);
                focusDir = MOTOR_FOCUS_BWD;

                g_pFocusPara.iTargetStep = g_pFocusPara.iFocusPosition - focusStep;
                g_pFocusPara.Direction = focusDir;
            }

            if(zoomStep < focusStep)
            {
                focusPPS = FOCUS_MAX_SPEED;

                zoomPPS = focusPPS*zoomStep/focusStep;

                if(zoomPPS > ZOOM_MAX_SPEED)
                    zoomPPS = ZOOM_MAX_SPEED;
                if(zoomPPS < ZOOM_MIN_SPEED)
                    zoomPPS = ZOOM_MIN_SPEED;

                if(zoomPPS == ZOOM_MAX_SPEED)
                {
                    focusPPS = zoomPPS*focusStep/zoomStep;

                    if(focusPPS < FOCUS_MIN_SPEED)
                        focusPPS = FOCUS_MIN_SPEED;
                }
            }
            else
            {
                zoomPPS = ZOOM_MAX_SPEED;

                focusPPS = zoomPPS*focusStep/zoomStep;

                if(focusPPS < FOCUS_MIN_SPEED)
                    focusPPS = FOCUS_MIN_SPEED;
            }

            if(focusStep <= 4)  // 2011-06-03 Chris
            {
                lensCtrlMotorInfoTab.bWaitZoomFinish = TRUE;
                lensCtrlMotorInfoTab.uiZoomPPS       = zoomPPS;
                g_pDZ5311Obj.pMotor->setMotorInfo(&lensCtrlMotorInfoTab);
                g_pDZ5311Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, zoomStep);
                set_flg(FLG_ID_LENS, FLGLENS_ZOOM_CMD_FIN|FLGLENS_FOCUS_CMD_FIN);
            }
            else
            {
                lensCtrlMotorInfoTab.uiZoomDirection     = zoomDir;
                lensCtrlMotorInfoTab.uiZoomPPS           = zoomPPS;
                lensCtrlMotorInfoTab.uiZoomStep          = zoomStep;
                lensCtrlMotorInfoTab.uiFocusDirection    = focusDir;
                lensCtrlMotorInfoTab.uiFocusPPS          = focusPPS;
                lensCtrlMotorInfoTab.uiFocusStep         = focusStep;
                g_pDZ5311Obj.pMotor->setMotorInfo(&lensCtrlMotorInfoTab);
                g_pDZ5311Obj.pMotor->sif_doZoomFocusMove();
            }
        }
    }

    return ERR_OK;
}

static INT32 lensDZ5311_zoom_retract(UINT32 param)
{
    UINT32 step, sBoundary;

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

    LENS_ERR(("LENS_M: lensDZ5311: Zoom Retract\r\n"));

    /* Enter ZOOM_STATE_CLOSING */
    lensDZ5311_zoom_gotoState(ZOOM_STATE_CLOSING);

    if(g_pZoomPara.ZoomStep < ZOOM_POS_TELE)
        lensDZ5311_task_zoomForward(4, TRUE, TRUE);
    else
        lensDZ5311_task_zoomReverse(4, TRUE, TRUE);

    sBoundary = 0;
    step = 20;
    while(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
    {
        lensDZ5311_task_zoomReverse(step, TRUE, TRUE);
        sBoundary += step;

        if(sBoundary > ZOOM_POS_OUTER_STOP)
        {
            LENS_ERR(("LENS_E: zoom B1: %d>%d",sBoundary, ZOOM_POS_OUTER_STOP));
            break;
        }
    }

    if(param != 0)
    {
        /* The following action is for miss step testing. */
        bUpdateHomeDiff = TRUE;
        sBoundary = 0;
        step = 2;
        while(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
        {
            lensDZ5311_task_zoomForward(step, TRUE, TRUE);
            sBoundary += step;

            if(sBoundary > ZOOM_FWD_LIMIT_INSIDE_PI)
            {
                LENS_ERR(("LENS_E: zoom B2: %d>%d",sBoundary, ZOOM_FWD_LIMIT_INSIDE_PI));
                break;
            }
        }
        zoomHomeDiff = abs(g_pZoomPara.ZoomStep - ZOOM_POS_HOME_PR);
        debug_err(("@@@ zoomHomeDiff = %d\r\n", zoomHomeDiff));
    }

    g_pDZ5311Obj.pMotor->changeMode(MOTOR_MODE_FOCUS);
    lensDZ5311_focus_gotoGarage();

    lensDZ5311_zoom_gotoState(ZOOM_STATE_CLOSED);

    bUpdateHomeDiff = FALSE;

    return ERR_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensDZ5311_focusIsr(void)
{
    static UINT32 uiMOB1 = 0;

    if(!g_pFocusPara.bFocusIsr)
        return;

    uiMOB1 = (uiMOB1+1)%2;  // 1 MOB count = 1 focus step

    if(uiMOB1 == 0)
    {
        if(g_pFocusPara.Direction == DIRECTION_FWD)
        {
            g_pFocusPara.iFocusPosition++;
        }
        else
        {
            g_pFocusPara.iFocusPosition--;
        }

        /* Check if target step and final step match and set focus Cmd finish. */
        if(g_pFocusPara.iFocusPosition == g_pFocusPara.iTargetStep)
            set_flg(FLG_ID_LENS, FLGLENS_FOCUS_CMD_FIN);
    }
}

static void lensDZ5311_focus_runStep(UINT8 dir, UINT16 step)
{
    if(dir==DIRECTION_FWD)
    {
        lensDZ5311_task_focusForward(step);
    }
    else
    {
        lensDZ5311_task_focusReverse(step);
    }
}

static void lensDZ5311_focus_doBL(void)
{
    lensDZ5311_focus_runStep(DIRECTION_FWD, FOCUS_BL_STEP);
    lensDZ5311_focus_runStep(DIRECTION_BWD, FOCUS_BL_STEP);
}

static void lensDZ5311_focus_gotoPosition(FOCUS_CATEGORY category, INT32 position)
{
    unsigned short Step;
    unsigned char Dir;
    MOTOR_INFO_TAB lensCtrlMotorInfoTab = {0};

    if(position < FOCUS_POS_INNER_STOP || position > FOCUS_POS_OUTER_STOP)
    {
        LENS_ERR(("LENS_E: %s: position (%d) is out of range\r\n", __func__, position));
        return;
    }

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

    if(Step==0)
        return;

    if(Step > (FOCUS_POS_OUTER_STOP-FOCUS_POS_INNER_STOP))
    {
        LENS_ERR(("LENS_E: %s: Step (%d) is overflow\r\n", __func__, Step));
        LENS_ERR(("LENS_E: %s: Step cur = %d, pos = %d\r\n", __func__, g_pFocusPara.iFocusPosition, position));
        return;
    }

    lensCtrlMotorInfoTab.uiFocusPPS  = FOCUS_MAX_SPEED;
    g_pDZ5311Obj.pMotor->setMotorInfo(&lensCtrlMotorInfoTab);

    if(Dir == DIRECTION_BWD)
    {
        lensDZ5311_focus_runStep(Dir,Step);
        if(g_pFocusPara.bFocusIsr == FALSE)
            g_pFocusPara.iFocusPosition -= Step;
    }
    else
    {
        lensDZ5311_focus_runStep(Dir,Step+FOCUS_BL_STEP);
        lensDZ5311_focus_runStep(DIRECTION_BWD, FOCUS_BL_STEP);
        if(g_pFocusPara.bFocusIsr == FALSE)
            g_pFocusPara.iFocusPosition += Step;
    }
}

static INT32 lensDZ5311_focus_gotoHome(void)
{
    unsigned short sBoundary;
    UINT32 step;

    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: %s: pMotor is null\r\n", __func__));
        return ERR_OPERATION_FAIL;
    }

    //find PI
    sBoundary = 0;
    step = 20;
    while(g_pDZ5311Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensDZ5311_focus_runStep(DIRECTION_BWD,step);
        sBoundary += step;
        if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            LENS_ERR(("LENS_E: B1: %d>%d",sBoundary, FOCUS_BWD_LIMIT_OUTSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    //debug_err(("@@@ [1] focus step = %d\r\n", g_pFocusPara.iFocusPosition));
    //g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;

    sBoundary = 0;
    step = 10;
    while(g_pDZ5311Obj.pMotor->focus_checkPI() == FOCUSPI_INSDIE_LEVEL)
    {
        lensDZ5311_focus_runStep(DIRECTION_FWD,step);
        sBoundary += step;
        if(sBoundary > FOCUS_FWD_LIMIT_INSIDE_PI)
        {
            LENS_ERR(("LENS_E: B2: %d>%d",sBoundary, FOCUS_FWD_LIMIT_INSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    sBoundary = 0;
    step = 2;
    while(g_pDZ5311Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensDZ5311_focus_runStep(DIRECTION_BWD,step);
        sBoundary += step;
        if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            LENS_ERR(("LENS_E: B3: %d>%d",sBoundary, FOCUS_BWD_LIMIT_OUTSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    //debug_err(("@@@ [2] focus step = %d\r\n", g_pFocusPara.iFocusPosition));
    if(bUpdateHomeDiff)
    {
        focusHomeDiff = abs(g_pFocusPara.iFocusPosition - FOCUS_POS_HOME_PR);
        debug_err(("@@@ focusHomeDiff = %d\r\n", focusHomeDiff));
    }
    g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;

    return ERR_OK;
}

static INT32 lensDZ5311_focus_gotoGarage(void)
{
    if(lensDZ5311_focus_gotoHome() == ERR_OK)
    {
        lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, FOCUS_POS_GARAGE);
        return ERR_OK;
    }
    else
    {
        return ERR_OPERATION_FAIL;
    }
}

static INT16 lensDZ5311_focus_getCurrentPosition(void)
{
    return g_pFocusPara.iFocusPosition;
}

static INT16 lensDZ5311_focus_getMaxPosition(void)
{
    return FOCUS_POS_OUTER_STOP;
}

static INT16 lensDZ5311_focus_getMinPosition(void)
{
    return FOCUS_POS_INNER_STOP;
}

static UINT32 lensDZ5311_focus_getFL(UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pFocusLengthTbl[zoomSection];
}

static INT16 lensDZ5311_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    if(index > FOCUS_ZOOM_TABLE_COLUMN-1)
        return 0;

    return g_pFocusZoomTblDefault[zoomSection-1][index];
}

static UINT16 lensDZ5311_focus_getZoomTblMaxIdx(void)
{
    return FOCUS_ZOOM_TABLE_COLUMN-1;
}

static UINT16 lensDZ5311_focus_getCurrentDist(INT32 basicStep)
{
    INT16 iFocusZoomTblBestColumn[FOCUS_ZOOM_TABLE_COLUMN] = {0};
    UINT32 minDiff=0xffff, curSection;
    INT32 i, idx = 0;

    curSection = lensDZ5311_zoom_getCurrentSection();

    /* Refer to the basicStep and g_pFocusZoomTblDefault table,
           and then Fullfill the iFocusZoomTblBestColumn table. */
    for(i=0; i<=FOCUS_ZOOM_TABLE_COLUMN-1; i++)
    {
        if(i <= FOCUS_ZOOM_BASIS_COLUMN)
            iFocusZoomTblBestColumn[i] = basicStep + abs(g_pFocusZoomTblDefault[curSection-1][i] - g_pFocusZoomTblDefault[curSection-1][FOCUS_ZOOM_BASIS_COLUMN]);
        else
            iFocusZoomTblBestColumn[i] = basicStep - abs(g_pFocusZoomTblDefault[curSection-1][i] - g_pFocusZoomTblDefault[curSection-1][FOCUS_ZOOM_BASIS_COLUMN]);
    }

    /* Refer to the iFocusZoomTblBestColumn table, and then
           calculate the near-est index. */
    if(g_pFocusPara.iFocusPosition < iFocusZoomTblBestColumn[0])
    {
        for(i=0; i<=FOCUS_ZOOM_TABLE_COLUMN-1; i++)
        {
            if(abs(iFocusZoomTblBestColumn[i] - g_pFocusPara.iFocusPosition) < minDiff)
            {
                minDiff = abs(iFocusZoomTblBestColumn[i] - g_pFocusPara.iFocusPosition);
                idx = i;
            }
        }
    }

    /* Return the focus distance by the near-est index. */
    return g_pFocusZoomTblDist[idx];
}

static void lensDZ5311_focus_updateFocusDistIdx(void)
{
    INT16 iFocusZoomTblBestColumn[FOCUS_ZOOM_TABLE_COLUMN] = {0};
    UINT32 minDiff=0xffff, curSection;
    INT32 i, idx = 0, a1=1, b1=1;
    INT32 basicStep;

    curSection = lensDZ5311_zoom_getCurrentSection();

    /* Refer to the basicStep (in g_pFocusZoomTblBasisStep) and g_pFocusZoomTblDefault table,
           and then Fullfill the iFocusZoomTblBestColumn table. */
    basicStep = g_pFocusZoomTblBasisStep[curSection-1];

    for(i=0; i<=FOCUS_ZOOM_TABLE_COLUMN-1; i++)
    {
        if(i <= FOCUS_ZOOM_BASIS_COLUMN)
            iFocusZoomTblBestColumn[i] = basicStep + abs(g_pFocusZoomTblDefault[curSection-1][i] - g_pFocusZoomTblDefault[curSection-1][FOCUS_ZOOM_BASIS_COLUMN]);
        else
            iFocusZoomTblBestColumn[i] = basicStep - abs(g_pFocusZoomTblDefault[curSection-1][i] - g_pFocusZoomTblDefault[curSection-1][FOCUS_ZOOM_BASIS_COLUMN]);
    }

    /* Refer to the iFocusZoomTblBestColumn table, and then
           calculate the near-est index. */
    if(g_pFocusPara.iFocusPosition < iFocusZoomTblBestColumn[0])
    {
        for(i=0; i<=FOCUS_ZOOM_TABLE_COLUMN-1; i++)
        {
            if(abs(iFocusZoomTblBestColumn[i] - g_pFocusPara.iFocusPosition) < minDiff)
            {
                minDiff = abs(iFocusZoomTblBestColumn[i] - g_pFocusPara.iFocusPosition);
                idx = i;
            }
        }
    }

    g_pFocusPara.uiFocusDistIdx = idx;
    g_pFocusPara.uiFocusOffsetRatio = 0;
    g_pFocusPara.bFocusLeftOffset = FALSE;

    /* Calculate the offset ratio. */
    if(g_pFocusPara.iFocusPosition > iFocusZoomTblBestColumn[idx])
    {
        if(idx > 0)
        {
            a1 = abs(g_pFocusPara.iFocusPosition - iFocusZoomTblBestColumn[idx]);
            b1 = abs(iFocusZoomTblBestColumn[idx-1] - iFocusZoomTblBestColumn[idx]);
            g_pFocusPara.uiFocusOffsetRatio = a1*100/b1;
            g_pFocusPara.bFocusLeftOffset = TRUE;
        }
    }
    else if(g_pFocusPara.iFocusPosition < iFocusZoomTblBestColumn[idx])
    {
        if(idx < FOCUS_ZOOM_TABLE_COLUMN-1)
        {
            a1 = abs(g_pFocusPara.iFocusPosition - iFocusZoomTblBestColumn[idx]);
            b1 = abs(iFocusZoomTblBestColumn[idx+1] - iFocusZoomTblBestColumn[idx]);
            g_pFocusPara.uiFocusOffsetRatio = a1*100/b1;
        }
    }
}

static UINT16 lensDZ5311_focus_getDiffStepByDist(UINT32 dist)
{
    UINT32 curSection, i;
    BOOL bFind = FALSE;

    if(dist == g_pFocusZoomTblDist[FOCUS_ZOOM_BASIS_COLUMN])
        return 0;

    for(i=0; i<=FOCUS_ZOOM_TABLE_COLUMN-1; i++)
    {
        if(g_pFocusZoomTblDist[i] == dist)
        {
            bFind = TRUE;
            break;
        }
    }

    if(bFind)
    {
        curSection = lensDZ5311_zoom_getCurrentSection();

        return abs(g_pFocusZoomTblDefault[curSection-1][i] - g_pFocusZoomTblDefault[curSection-1][FOCUS_ZOOM_BASIS_COLUMN]);
    }
    else
        return 0;
}

static INT16 lensDZ5311_focus_getBestPosition(UINT32 section, BOOL bUpdateBestTbl)
{
    static BOOL bFirst = TRUE;
    UINT32 i;

    if(g_pFocusPara.bSetBasisStep)
    {
        if(bFirst || bUpdateBestTbl)
        {
            for(i=0; i<ZOOM_SECTION_TELE; i++)
            {
                /* Use the focus distance index and offset ratio to update the g_pFocusZoomTblBest table. */
                if(g_pFocusPara.uiFocusDistIdx <= FOCUS_ZOOM_BASIS_COLUMN)
                    g_pFocusZoomTblBest[i] = g_pFocusZoomTblBasisStep[i] + abs(g_pFocusZoomTblDefault[i][g_pFocusPara.uiFocusDistIdx] - g_pFocusZoomTblDefault[i][FOCUS_ZOOM_BASIS_COLUMN]);
                else
                    g_pFocusZoomTblBest[i] = g_pFocusZoomTblBasisStep[i] - abs(g_pFocusZoomTblDefault[i][g_pFocusPara.uiFocusDistIdx] - g_pFocusZoomTblDefault[i][FOCUS_ZOOM_BASIS_COLUMN]);

                if(g_pFocusPara.uiFocusOffsetRatio > 0)
                {
                    if(g_pFocusPara.bFocusLeftOffset)
                    {
                        g_pFocusZoomTblBest[i] += abs(g_pFocusZoomTblDefault[i][g_pFocusPara.uiFocusDistIdx-1] - g_pFocusZoomTblDefault[i][g_pFocusPara.uiFocusDistIdx])*g_pFocusPara.uiFocusOffsetRatio/100;
                    }
                    else
                    {
                        g_pFocusZoomTblBest[i] -= abs(g_pFocusZoomTblDefault[i][g_pFocusPara.uiFocusDistIdx+1] - g_pFocusZoomTblDefault[i][g_pFocusPara.uiFocusDistIdx])*g_pFocusPara.uiFocusOffsetRatio/100;
                    }
                }
            }

            bFirst = FALSE;
        }

        return g_pFocusZoomTblBest[section-1];
    }
    else
    {
        return FOCUS_POS_HOME_PR;
    }
}

/* This function is used to set the basis step of lens calibration into
       the g_pFocusZoomTblBasisStep table. */
static void lensDZ5311_focus_setBasisStep(UINT32 section, INT32 farStep)
{
    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    g_pFocusPara.bSetBasisStep = TRUE;

    g_pFocusZoomTblBasisStep[section-1] = farStep;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

static UINT32 lensDZ5311_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pApertureZoomTbl[zoomSection];
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens testing.
//--------------------------------------------------------------------------------------------------------------------------
/* This funciton is only for DZ5311 testing. */
void lensDZ5311_test(void)
{
    lensDZ5311_zoom_gotoSection(ZOOM_SECTION_TELE);

    TimerDelayMs(10);

    lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 304);

    TimerDelayMs(10);

    lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, -60);

    TimerDelayMs(10);

    lensDZ5311_task_zoomReverse(ZOOM_POS_TELE-g_pZoomSectionPulse[13], FALSE, FALSE);

    lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 216);

    TimerDelayMs(10);

    lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, -60);

    TimerDelayMs(10);

    lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 216);

    TimerDelayMs(10);

    lensDZ5311_focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, -60);
}


