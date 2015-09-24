/**
    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.

    @file       Cal_Lens.c
    @ingroup    mISYSLens

    @brief      Lens calibration library

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2010/02/01
*/

#include    <string.h>

#include    "SysCfg.h"
#include    "Lens.h"
#include    "CalLens.h"
#include    "UILensObj.h"
#include    "PhotoTsk.h"
#include    "KeyScanTsk.h"
#include    "IDE.h"
#include    "UIResource.h"

#define CALLENS_VA_NG                       10000
#define CALLENS_VA_WARNING                  50000

#if (_CALIBRATION_MODE_ == ENABLE)

INT16 gCalLens_FocusRangeTable[CALLENS_ZOOM_SECTION_NUM][2] = {
  #if(CALLENS_ZOOM_SECTION_NUM == 25)
    //start ,  end ,
    {  -116 ,   28 , },
    {  -116 ,  216 , }, // 1.2
    {  -116 ,  216 , }, // 1.4
    {  -116 ,  216 , }, // 1.6
    {  -116 ,  216 , }, // 1.8
    {    88 ,  216 , },
    {    88 ,  324 , }, // 2.25
    {    88 ,  324 , }, // 2.5
    {    88 ,  324 , }, // 2.75
    {   200 ,  324 , },
    {   200 ,  384 , }, // 3.33
    {   200 ,  384 , }, // 3.66
    {   260 ,  384 , },
    {   260 ,  384 , }, // 4.5
    {   272 ,  412 , },
    {   272 ,  412 , }, // 5.5
    {   304 ,  416 , },
    {   304 ,  416 , }, // 6.5
    {   292 ,  404 , },
    {   292 ,  404 , }, // 7.5
    {   272 ,  384 , },
    {   272 ,  384 , }, // 8.5
    {   240 ,  348 , },
    {   240 ,  348 , }, // 9.5
    {   200 ,  304 , }
  #else
    //start ,  end ,
    {  -116 ,   28 , },
    {    88 ,  216 , },
    {   200 ,  324 , },
    {   260 ,  384 , },
    {   272 ,  412 , },
    {   304 ,  416 , },
    {   292 ,  404 , },
    {   272 ,  384 , },
    {   240 ,  348 , },
    {   200 ,  304 , }
  #endif
};

UINT8 CalLens_FarFocusAdjust(void)
{
    INT32 maxIdx = 0;
    INT16 StartIdx,EndIdx;
    UINT8 i;
    UINT  AF_Step;
    FLGPTN uiFlag;
    char str1[32];
    UINT32 maxVA;
    BOOL bFocusNG = FALSE;

    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    //Cal_AlgClearScreen();
    //Cal_AlgShowString("1. Face ISO chart(far)",0,0);
    //Cal_AlgShowString("2. Press shutter key",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    //Cal_AlgClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    //Photo_LockCAF(1);
    KeyScan_Suspend();

    Lens_Zoom_SetPPS(300);
    Lens_Focus_SetPPS(500);
    Lens_Init(LENS_INIT_ZOOM_PART1);
    Lens_Init(LENS_INIT_ZOOM_PART2);
    Lens_Init(LENS_INIT_FOCUS);
    Lens_Zoom_EnableIsr(FALSE);

    UILens_AFPrepare(TRUE);

    AF_Step=4;
    for(i=0;i<Lens_Zoom_GetMaxSection();i++)
    {
        //Lens_Focus_GotoHome();
        Lens_Zoom_Goto(i+1);

        if(CalLens_IsNearAdjust())
            StartIdx = CalLens_GetNearFocusStep(i+1, FALSE);
        else
            StartIdx = gCalLens_FocusRangeTable[i][0];

        Lens_Focus_Goto(StartIdx);
        EndIdx = gCalLens_FocusRangeTable[i][1];

        AF_Step = abs(StartIdx-EndIdx)/20;

        if(AF_Step > 4)
        {
            maxIdx = UILens_SearchFullRange_Bwd(StartIdx, EndIdx, AF_Step);
            maxIdx = UILens_SearchFullRange_Bwd(maxIdx-AF_Step+4, maxIdx+AF_Step-4, 4);
            maxIdx = UILens_SearchFullRange_Bwd(maxIdx-3, maxIdx+3, 1);
        }
        else
        {
            maxIdx = UILens_SearchFullRange_Bwd(StartIdx, EndIdx, 4);
            maxIdx = UILens_SearchFullRange_Bwd(maxIdx-3, maxIdx+3, 1);
        }
        gCalLens_Info.Far_Focus[i] = maxIdx;

        //Cal_AlgClearScreen();
        snprintf(str1, 32, "S=%d,Far=%d,VA=%d", i+1, gCalLens_Info.Far_Focus[i], maxVA);

        if(maxVA > CALLENS_VA_WARNING)
        {
            //if(i % 2 == 0)
            //    Cal_AlgShowString(str1,0,10*(i/2));
            //else
            //    Cal_AlgShowString(str1,30,10*(i/2));
        }
        else
        {
            //if(i % 2 == 0)
            //    Cal_AlgShowString(str1,0,10*(i/2));
            //else
            //    Cal_AlgShowString(str1,30,10*(i/2));

            if(maxVA < CALLENS_VA_NG)
            {
                bFocusNG =TRUE;
                TimerDelayMs(2000);
                break;
            }
        }
    }

    for(i=1;i<=Lens_Zoom_GetMaxSection();i++)
    {
        if(CalLens_GetFarFocusStep(i, FALSE) <= CalLens_GetNearFocusStep(i, FALSE))
            bFocusNG = TRUE;
    }

    if(bFocusNG)
        gCalLens_Info.bFarAdjust = FALSE;
    else
        gCalLens_Info.bFarAdjust = TRUE;

    UILens_AFPrepare(FALSE);

    CalLens_WriteCalData();
    KeyScan_Resume();
    Lens_Zoom_EnableIsr(TRUE);
    Lens_Retract(0);

    /************************************************************/
    //Cal_AlgClearScreen();
    //if(bFocusNG)
    //    Cal_AlgShowString("1. Adjust NG (far)",0,0);
    //else
    //    Cal_AlgShowString("1. Adjust done (far)",0,0);
    //Cal_AlgShowString("2. Press enter key to leave",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_ENTER);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_ENTER, TWF_ORW|TWF_CLR);
    /*************************************************************/
    return CALLENS_STA_OK;
}

UINT8 CalLens_NearFocusAdjust(void)
{
    INT32 maxIdx = 0;
    INT16 StartIdx,EndIdx;
    UINT8 i;
    UINT  AF_Step;
    FLGPTN uiFlag;
    char str1[32];
    UINT32 maxVA;
    BOOL bFocusNG = FALSE;

    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    //Cal_AlgClearScreen();
    //Cal_AlgShowString("1. Face ISO chart (near)",0,0);
    //Cal_AlgShowString("2. Press shutter key",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    //Cal_AlgClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    //Photo_LockCAF(1);
    KeyScan_Suspend();

    Lens_Zoom_SetPPS(300);
    Lens_Focus_SetPPS(500);
    Lens_Init(LENS_INIT_ZOOM_PART1);
    Lens_Init(LENS_INIT_ZOOM_PART2);
    Lens_Init(LENS_INIT_FOCUS);
    Lens_Zoom_EnableIsr(FALSE);

    UILens_AFPrepare(TRUE);

    AF_Step=4;
    for(i=0;i<Lens_Zoom_GetMaxSection();i++)
    {
        //Lens_Focus_GotoHome();
        Lens_Zoom_Goto(i+1);
        StartIdx = gCalLens_FocusRangeTable[i][0];
        Lens_Focus_Goto(StartIdx);
        EndIdx = gCalLens_FocusRangeTable[i][1];

        AF_Step = abs(StartIdx-EndIdx)/20;

        if(AF_Step > 4)
        {
            maxIdx = UILens_SearchFullRange_Bwd(StartIdx, EndIdx, AF_Step);
            maxIdx = UILens_SearchFullRange_Bwd(maxIdx-AF_Step+4, maxIdx+AF_Step-4, 4);
            maxIdx = UILens_SearchFullRange_Bwd(maxIdx-3, maxIdx+3, 1);
        }
        else
        {
            maxIdx = UILens_SearchFullRange_Bwd(StartIdx, EndIdx, 4);
            maxIdx = UILens_SearchFullRange_Bwd(maxIdx-3, maxIdx+3, 1);
        }
        gCalLens_Info.Near_Focus[i] = maxIdx;

        //Cal_AlgClearScreen();
        snprintf(str1, 32, "S=%d,Near=%d,VA=%d", i+1, gCalLens_Info.Near_Focus[i], maxVA);

        if(maxVA > CALLENS_VA_WARNING)
        {
            //if(i % 2 == 0)
            //    Cal_AlgShowString(str1,0,10*(i/2));
            //else
            //    Cal_AlgShowString(str1,30,10*(i/2));
        }
        else
        {
            //if(i % 2 == 0)
            //    Cal_AlgShowString(str1,0,10*(i/2));
            //else
            //    Cal_AlgShowString(str1,30,10*(i/2));

            if(maxVA < CALLENS_VA_NG)
            {
                bFocusNG =TRUE;
                TimerDelayMs(2000);
                break;
            }
        }
    }

    if(bFocusNG)
        gCalLens_Info.bNearAdjust = FALSE;
    else
        gCalLens_Info.bNearAdjust = TRUE;

    UILens_AFPrepare(FALSE);

    CalLens_WriteCalData();
    KeyScan_Resume();
    Lens_Zoom_EnableIsr(TRUE);
    Lens_Retract(0);

    /************************************************************/
    //Cal_AlgClearScreen();
    //if(bFocusNG)
    //    Cal_AlgShowString("1. Adjust NG (near)",0,0);
    //else
    //    Cal_AlgShowString("1. Adjust done (near)",0,0);
    //Cal_AlgShowString("2. Press enter key to leave",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_ENTER);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_ENTER, TWF_ORW|TWF_CLR);
    /*************************************************************/

    return CALLENS_STA_OK;
}

INT32 CalLens_GetFarFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    if(zoomSection > CALLENS_ZOOM_SECTION_NUM)
        return 0;

    if(bCompensate)
        return (gCalLens_Info.Far_Focus[zoomSection-1]+12);
    else
        return (gCalLens_Info.Far_Focus[zoomSection-1]);
}

INT32 CalLens_GetNearFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    if(zoomSection > CALLENS_ZOOM_SECTION_NUM)
        return 0;

    if(bCompensate)
        return (gCalLens_Info.Near_Focus[zoomSection-1]-8);
    else
        return (gCalLens_Info.Near_Focus[zoomSection-1]);
}

INT32 CalLens_FocusStepMissTest(void)
{
    FLGPTN  uiFlag;
    UINT32  i;
    char str1[32];
    extern UINT32 focusHomeDiff;
    extern UINT32 zoomHomeDiff;

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    //Photo_LockCAF(1);
    KeyScan_Suspend();

    //Cal_AlgClearScreen();
    //Cal_AlgShowString("1. Please wait..",0,0);

    Lens_Zoom_SetPPS(300);
    Lens_Focus_SetPPS(500);

    Lens_Retract(0);
    TimerDelayMs(500);
    Lens_Init(LENS_INIT_ZOOM_PART1);
    Lens_Init(LENS_INIT_ZOOM_PART2);
    Lens_Init(LENS_INIT_FOCUS);

    for(i=0;i<2;i++)
    {
        lensDZ5311_test();

        UILens_WaitZoomFinish();

        TimerDelayMs(100);
    }

    Lens_Retract(1);
    TimerDelayMs(500);
    Lens_Init(LENS_INIT_ZOOM_PART1);
    Lens_Init(LENS_INIT_ZOOM_PART2);
    Lens_Init(LENS_INIT_FOCUS);

    gCalLens_Info.bFocusStepMissTest = TRUE;
    gCalLens_Info.uiZoomMissStep = zoomHomeDiff;
    gCalLens_Info.uiFocusMissStep = focusHomeDiff;
    CalLens_WriteCalData();
    KeyScan_Resume();

    snprintf(str1, 32, "2. Diff Zoom(%d),Focus(%d)", zoomHomeDiff, focusHomeDiff);
    //Cal_AlgShowString(str1,0,30);
    //Cal_AlgShowString("3. Press enter key to leave",0,60);
    clr_flg(FLG_ID_KEY, FLGKEY_ENTER);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_ENTER, TWF_ORW|TWF_CLR);

    return 0;
}

#endif
