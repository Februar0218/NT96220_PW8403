/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       AF_Lib.c
    @ingroup    mISYSLens

    @brief      Auto focus library

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/12
*/
#include <stdio.h>
#include "debug.h"
#include "OSCommon.h"
#include "ImageAlgInfor.h"
#include "Lens.h"
#include "AF_Lib.h"
#include "Utility.h"
#include "ipe_lib.h"
//#NT#2010/08/26#KS Hung -begin
//#NT#Remove warning message
#include "string.h"
//#NT#2010/08/26#KS Hung -end
#include "sie_lib.h"
#include "Ipe_lib.h"
#include "fd_lib.h"
#include <stdlib.h>
#include "afdrv_common.h"
#include "af_api.h"
#include "type.h"
#include "aaa_lib.h"
#include "Ae_alg.h"
#include "Ae_api.h"

ER User_VABegin(AF_IPEINFO *AfIpeInfo);
ER User_VAEnd(AF_IPEINFO *AfIpeInfo);

//#NT#2012/08/06#KS Hung -begin
//#NT#Add the callback function of Detecting GPIO
static FPAF_DETGPIOKEY g_bfpAfDetShutter2 = NULL;
//#NT#2012/08/06#KS Hung -end
static FPCALLBACK_GETVA g_fpGetVariation = NULL;
static UINT32 g_AFInfoIdx = 0;
static UINT32 g_AFGuessInfoIdx = 0;
static UINT32 g_AFMaxIdx = 0;

AF_INFO g_AFInfo = {0};
AF_GUESS_INFO g_AFGuessInfo = {0};
BOOL g_AFfocusforAllWin[AF_FOCUS_TOTAL_WIN] = {0};

#define AFDRV_DEBUG   ENABLE

#if(AFDRV_DEBUG)
#define AFDRV_MSG(...)       debug_msg("AFDRV MSG:"__VA_ARGS__)
#else
#define AFDRV_MSG(...)
#endif
#define AFDRV_ERR(...)      debug_msg("AFDRV ERR:"__VA_ARGS__)

static AFDRV_MANUAL af_manual = {0};

AF_IPEINFO gAFIPEInfo={0};
INT32 Focus_TempComp_Near[] = {58,59,63,58,59,61,63,64,63,62,58,59,63};
INT32 Focus_TempComp_Far[] = {58,59,63,58,59,61,63,64,63,62,58,59,63};
static BOOL g_bAFChangeMode = TRUE;


AF_AETRANSFERMODE AFdrv_GetAFModel(void)
{
    return Model_Normal;
}

void AFdrv_SetAFModeChange(BOOL EN)
{
    if(EN)
        g_bAFChangeMode = TRUE;
    else
        g_bAFChangeMode = FALSE;
}

BOOL AFdrv_CheckAFModeChange(void)
{
    return g_bAFChangeMode;
}

void AFdrv_AFPrepare(BOOL bOP, UINT32 step)
{
#if 0
    if(g_bAFChangeMode == TRUE)
    {
        if(bOP)
        {
            AF_PrvModeSel(SENSOR_AF_MODE, SENSOR_OUT_PROGRESSIVE, step);
        }
        else
        {
            AF_PrvModeSel(SENSOR_PRV_MODE, SENSOR_OUT_PROGRESSIVE, step);
        }
    }
#endif
}

void AFdrv_AASwitch(BOOL bOP)
{
    if(bOP)
    {
        aaa_AeLock(LOCK);
        aaa_AwbLock(LOCK);
    }
    else
    {
        aaa_AeLock(UNLOCK);
        aaa_AwbLock(UNLOCK);
    }
}

void AFdrv_AEPrepare(BOOL bOP, UINT32 step)
{
#if 0
    if(bOP)
    {
        AeSetPrv2AF(step);
    }
    else
    {
        AeSetAF2Prv();
    }
#endif
}

void AFdrv_VAPrepare(BOOL bface, BOOL bOP, UINT32 step)
{
    AEALG_ARG AeArg;

    AE_GetPrvAEArg(&AeArg.ISOGain, &AeArg.ExpoTime,&AeArg.Iris);

    if(bOP)
    {
        switch(step)
        {
            case 1:
                if (AeArg.ISOGain <= 100)
                {
                    gAFIPEInfo.vacc_gain = 0;
                    gAFIPEInfo.va_th = 8;
                    gAFIPEInfo.hnfsel = IPE_HNF_AUTO;
                    gAFIPEInfo.va_hnf = 0;
                }
                else if (AeArg.ISOGain <= 200)
                {
                    gAFIPEInfo.vacc_gain = 0;
                    gAFIPEInfo.va_th = 10;
                    gAFIPEInfo.hnfsel = IPE_HNF_AUTO;
                    gAFIPEInfo.va_hnf = 0;
                }
                else if (AeArg.ISOGain <= 400)
                {
                    gAFIPEInfo.vacc_gain = 0;
                    gAFIPEInfo.va_th = 12;
                    gAFIPEInfo.hnfsel = IPE_HNF_AUTO;
                    gAFIPEInfo.va_hnf = 0;
                }
                else
                {
                    gAFIPEInfo.vacc_gain = 0;
                    gAFIPEInfo.va_th = 16;
                    gAFIPEInfo.hnfsel = IPE_HNF_AUTO;
                    gAFIPEInfo.va_hnf = 0;
                }

                if(bface)
                {
                    gAFIPEInfo.vacc_gain = 0;
                    gAFIPEInfo.va_th = 0;
                    gAFIPEInfo.hnfsel = IPE_HNF_MANUAL;
                    gAFIPEInfo.va_hnf = 0;
                }
                break;
            case 2:
                    User_VABegin(&gAFIPEInfo);
                break;
            case 3:
                     User_VABegin(&gAFIPEInfo);
                break;
        }

    }
    else
    {
        User_VAEnd(&gAFIPEInfo);
    }
}

void AFdrv_SetManualRange(AFDRV_MANUAL ManualSet)
{
    af_manual = ManualSet;
}

void AFdrv_GetCurrentRange(PAFALG_INFO_IN af_Obj, _IMGAFMode_sel AFMode)
{
    UINT32 uiZoomSection = Lens_Zoom_GetSection();
    UINT32 AF_Step;
    //#NT#2011/11/23#KS Hung -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    FP_LENS_CAL_CB   fpLensCalCB = NULL;
    //#NT#2011/11/23#KS Hung -end


    if(Lens_Zoom_GetSection() >= 4)
        AF_Step=4;
    else
        AF_Step=4;

    //#NT#2012/04/25#KS Hung -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    fpLensCalCB = Lens_GetCalInfo();
    switch(AFMode)
    {
    case _IMGAFMode_Macro:
        af_Obj->iStartIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, FALSE)-32; // 0.5m
        af_Obj->iEndIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, FALSE) + (Lens_Focus_GetZoomTableValue(uiZoomSection,2) - Lens_Focus_GetZoomTableValue(uiZoomSection,1) + 20);
        af_Obj->iStep = AF_Step;
        af_Obj->iDefaultIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, FALSE); // default for NG
        af_Obj->iOffsetStep = 0; // for heat compensation
        break;

    case _IMGAFMode_Normal:
    default:
        af_Obj->iStartIdx = fpLensCalCB(LENS_CAL_FAR_FOCUS,uiZoomSection, TRUE); // inf
        af_Obj->iEndIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, FALSE)+4*AF_Step; // near // 2011-06-01 Chris
        af_Obj->iStep = AF_Step;
        af_Obj->iDefaultIdx = fpLensCalCB(LENS_CAL_FAR_FOCUS,uiZoomSection, FALSE); // default for NG
        af_Obj->iOffsetStep = 0; // for heat compensation
        break;
    }
    //#NT#2011/11/23#KS Hung -end

    AFDRV_MSG(" AF mode = %d; %d -> %d ,Step = %d\r\n", AFMode, af_Obj->iStartIdx, af_Obj->iEndIdx, af_Obj->iStep);
}

UINT16 AFdrv_GetFocusDist(void)
{
#if 0
    UINT8 i;
    UINT32 uiDist;
    UINT8 uiZoomSecMinusOne;
    //#NT#2010/08/17#Photon Lin -begin
    //#Rearrange lens driver
    uiZoomSecMinusOne = Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1;
    //#NT#2010/08/17#Photon Lin -end
    for(i=0; i<11; i++)
    {
         if(Lens_Focus_GetPosition()<Lens_Focus_GetTable(uiZoomSecMinusOne,i))
            break;
    }
    //#NT#2010/08/05#Photon Lin -begin
    //#Rearrange lens driver
    if(i==0)
    {
        uiDist= Lens_Focus_GetDistTable(0);
    }
    else if(i==11)
    {
        uiDist= Lens_Focus_GetDistTable(10);
    }
    else
    {
        UINT32 uiAval, uiBval, uiAintvl, uiBintvl;

        uiAval = Lens_Focus_GetDistTable(i-1);
        uiBval = Lens_Focus_GetDistTable(i);
        uiAintvl = Lens_Focus_GetPosition() - Lens_Focus_GetTable(uiZoomSecMinusOne,i-1);
        uiBintvl = Lens_Focus_GetTable(uiZoomSecMinusOne,i)-Lens_Focus_GetPosition();
        uiDist = (uiAval*uiBintvl + uiBval*uiAintvl)/(uiAintvl+uiBintvl);
    }
    //#NT#2010/08/05#Photon Lin -end
    //debug_err(("- - uiDist %d - - \r\n", uiDist));
    return (UINT16)uiDist;
#else
    return 100;
#endif
}

UINT32 AF_WriteDebugInfo(UINT32 Addr)
{
    //#NT#2011/11/29#KS Hung -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    FP_LENS_CAL_CB   fpLensCalCB = NULL;
    //#NT#2011/11/29#KS Hung -end
    UH i;

    //#NT#2011/11/29#KS Hung -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    fpLensCalCB = Lens_GetCalInfo();
    for(i=1; i<=Lens_Zoom_GetMaxSection(); i++)
    {
        Addr += sprintf((char *)Addr,"Lens: S=%d, Near:%ld Far:%ld\r\n",
            i,
            fpLensCalCB(LENS_CAL_NEAR_FOCUS, i, FALSE),
            fpLensCalCB(LENS_CAL_FAR_FOCUS, i, TRUE)
            );
    }
    //#NT#2011/11/29#KS Hung -end

    Addr += sprintf((char *)Addr,"AFTsk\t: OZoomIdx:%ld DZoomIdx:%d CurIdx:%ld CurVA:%ld Status:0x%ld\r\n",
        Lens_Zoom_GetSection(),
        gImageAlgInfo.DZoomIndex,
        Lens_Focus_GetCurrentPosition(),
        Lens_Focus_GetCurrentVA(),
        gAlgMsg.AF_Status
        );

    for(i=0; i<AF_TOTAL_WIN; i++)
    {
        Addr += sprintf((char *)Addr,"AFTsk\t: win[%d]: %ld \r\n",
            i,
            gAlgMsg.AF_Window[i]
            );
    }

    {
        INT16 _idx;
        UINT32 _va;

        for(i=0; i<AF_TOTAL_STEP; i++)
        {
            AFALG_GetAFReaultBystep(4, i, &_idx, &_va);

            if(_va == 0)
                break;

            Addr += sprintf((char *)Addr,"AFTsk\t: Step: %d VA: %ld\r\n",
                _idx,
                _va
                );
        }

        Addr += sprintf((char *)Addr,"Non-AFTsk\t: StartIdx: %ld EndIdx: %ld MaxIdx: %ld MaxVA: %ld AF_GuessIdx: %ld AF_time: %ld AF_Window: %ld\r\n",
            g_AFInfo.StartIdx,
            g_AFInfo.EndIdx,
            g_AFInfo.MaxIdx,
            g_AFInfo.MaxVA,
            g_AFInfo.AF_GuessIdx,
            g_AFInfo.AF_time,
            g_AFInfo.AF_Window
            );

        for(i=0; i<AF_FOCUS_ARRAY_SIZE; i++)
        {
            if (g_AFInfo.Value[i] !=0)
            {
                Addr += sprintf((char *)Addr,"Non-AFTsk\t: Step=%ld, VA=%ld \r\n",
                g_AFInfo.Step[i],
                g_AFInfo.Value[i]
                );
            }
        }
    }

    return Addr;
}

void AF_RegGetVariationCB(FPCALLBACK_GETVA getVariation)
{
    g_fpGetVariation = getVariation;
}

/*
    @brief  Full Range search for calibration using

    @author Chris Chung
*/

INT32 AF_SearchFullRange(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA)
{
    INT16  maxIdx=0;
    UINT32 tempVA, maxVA=0, j=0;
    UINT32 contrastCnt;
    UINT32  loop = 0;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    g_AFInfo.StartIdx = startIdx;
    g_AFInfo.EndIdx = endIdx;

    //for(idx = endIdx; idx >= startIdx; idx -= jumpStep)
    while (1)
    {
        if(loop > ((endIdx - startIdx)/jumpStep + 3))
        {
            debug_err(("%s: cur = %d, loop = %d is over!!\r\n", __func__, Lens_Focus_GetCurrentPosition(), loop));
            debug_err(("@@@ endIdx=%d, startIdx=%d, jumpStep=%d\r\n", endIdx, startIdx, jumpStep));
            break;
        }

        loop++;
        tempVA=0;
        sie_waitVD(2);

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = g_fpGetVariation(4);

        debug_err(("%s: step=%d, tempVA=%d, maxVA=%d\r\n", __func__, Lens_Focus_GetCurrentPosition(), tempVA, maxVA));

        if(g_AFInfoIdx<=AF_FOCUS_ARRAY_SIZE-1)
        {
            g_AFInfo.Step[g_AFInfoIdx] = Lens_Focus_GetCurrentPosition();
            g_AFInfo.Value[g_AFInfoIdx] = tempVA;
            g_AFInfo.VACnt[g_AFInfoIdx] = ipe_getVACnt();
            g_AFInfoIdx++;
        }

        if (tempVA > maxVA)
        {
            maxIdx = Lens_Focus_GetCurrentPosition();
            maxVA = tempVA;
            j=g_AFInfoIdx;
            g_AFMaxIdx = g_AFInfoIdx - 1;
        }
        /*else
        {
            if(tempVA < (maxVA * 0.6)) //0.6 is threshold.
            {
                debug_err(("find peak\r\n"));
                break;
            }
        }*/

        if(Lens_Focus_GetCurrentPosition() <= startIdx)
        {
            if(g_AFInfoIdx != j)
            {
                // Peak is not the last one.
                break;
            }
        }

        Lens_Focus_Goto(Lens_Focus_GetCurrentPosition()-jumpStep);
    }

    //Lens_Focus_Goto(maxIdx);
    if(pMaxVA != NULL)
        *pMaxVA = maxVA;

    debug_err(("%s: maxIdx=%d, maxVA=%d\r\n", __func__, maxIdx, maxVA));

    return maxIdx;
}

/*
    for ST8005A lens using
*/

PAF_INFO AF_GetAFInfo(void)
{
    return &g_AFInfo;
}

void AF_ResetAFInfo(void)
{
    g_AFInfoIdx = 0;
    g_AFMaxIdx = 0;
    memset(&g_AFInfo, 0, sizeof(AF_INFO));
}

void AF_ResetAFGuessInfo(void)
{
    g_AFGuessInfoIdx = 0;
    memset((void *)&g_AFGuessInfo, 0, sizeof(AF_GUESS_INFO));
}

#define ST8005A_VA_Treshold   1000
#define ST8005A_VA_LowLimit   3500

INT32 AF_SearchFullRange_ST8005A_Single(UINT16 startIdx, UINT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA, UINT32 *pMinVA, UINT32 waitVDcount, BOOL bSkipRemainder)
{
    UINT32  maxIdx=0, tempVA, maxVA=0, minVA=0x0fffffff, j=0;
    UINT32 contrastCnt, waitCnt;
//    char str1[32];
    UINT32 loop = 0;
    UINT32 VA_thr = 0;


    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    g_AFInfo.StartIdx = startIdx;
    g_AFInfo.EndIdx = endIdx;

    //for(idx = endIdx; idx >= startIdx; idx -= jumpStep)
    while (1)
    {
        if(loop > ((endIdx - startIdx)/jumpStep + 4))   // 2011-07-26 Chris
        {
            debug_err(("%s: cur = %d, loop = %d is over!!\r\n", __func__, Lens_Focus_GetCurrentPosition(), loop));
            debug_err(("@@@ endIdx=%d, startIdx=%d, jumpStep=%d\r\n", endIdx, startIdx, jumpStep));
            break;
        }

        loop++;
        tempVA=0;
        sie_waitVD(waitVDcount);

        contrastCnt = GetIpeVACnt();

        waitCnt = 0;
        while(contrastCnt == GetIpeVACnt())
        {
            waitCnt++;
            TimerDelayMs(1);
            if(waitCnt > 1000 && (waitCnt % 1000)==1)
            {
                debug_err(("%s: VA Cnt error!!\r\n", __func__));
            }
        }

        tempVA = g_fpGetVariation(4);

        g_AFInfo.Step[g_AFInfoIdx]  = Lens_Focus_GetCurrentPosition();
        g_AFInfo.Value[g_AFInfoIdx] = tempVA;
        g_AFInfo.VACnt[g_AFInfoIdx] = GetIpeVACnt();
        g_AFInfoIdx++;

        if(tempVA > maxVA)
        {
            maxIdx = Lens_Focus_GetCurrentPosition();
            maxVA = tempVA;
            j=g_AFInfoIdx;
            g_AFMaxIdx = g_AFInfoIdx - 1;
        }

        if(tempVA < minVA)
        {
            minVA = tempVA;
        }
        if ((fd_IsLock() == FALSE) && (gAlgMsg.FaceDetectNum != 0))
        {
             VA_thr = 100;
         }
        else if (Lens_Zoom_GetSection() > 2)
        {
            VA_thr = 1000;
        }
        else if (Lens_Zoom_GetSection() > 1)
        {
            VA_thr = 1500;
        }
        else
        {
            VA_thr = 2500;
        }

        if(bSkipRemainder && maxVA > VA_thr && tempVA < (maxVA * 0.8)) //0.8 is threshold.
        {
            debug_err(("find peak\r\n"));
            break;
        }

        if(Lens_Focus_GetCurrentPosition() <= startIdx)
        {
            if(g_AFInfoIdx != j)
            {
                // Peak is not the last one.
                break;
            }
        }

        Lens_Focus_Goto(Lens_Focus_GetCurrentPosition()-jumpStep);
    }

    g_AFInfo.MaxVA=maxVA;
    *pMaxVA = maxVA;
    *pMinVA = minVA;

    return maxIdx;
}

INT32 AF_SearchFullRange_ST8005A_Multi(UINT16 startIdx, UINT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA, UINT32 *pMinVA, UINT32 waitVDcount, UINT32 *pWinIdx)
{
    UINT32 i, j, winIdx=0, maxIdx=0, maxVA=0, tmpIdx, tmpMaxVA, tmpMinVA, minVA=0x0fffffff;
    UINT32 contrastCnt;
    UINT32 loop = 0;
    UINT32 focusIdxforAllWin[AF_FOCUS_TOTAL_WIN] = {0};
    UINT32 focusMaxVAforAllWin[AF_FOCUS_TOTAL_WIN] = {0};
    UINT32 focusMinVAforAllWin[AF_FOCUS_TOTAL_WIN] = {0};

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    g_AFInfo.StartIdx = startIdx;
    g_AFInfo.EndIdx = endIdx;

    // Step1. Focus to every step and record all VA windows.
    while (1)
    {
        if(loop > ((endIdx - startIdx)/jumpStep + 3))
        {
            debug_err(("%s: cur = %d, loop = %d is over!!\r\n", __func__, Lens_Focus_GetCurrentPosition(), loop));
            debug_err(("@@@ endIdx=%d, startIdx=%d, jumpStep=%d\r\n", endIdx, startIdx, jumpStep));
            break;
        }

        loop++;
        sie_waitVD(waitVDcount);

        contrastCnt = GetIpeVACnt();

        while(contrastCnt == GetIpeVACnt())
            TimerDelayMs(1);

        g_AFInfo.Step[g_AFInfoIdx]  = Lens_Focus_GetCurrentPosition();
        for(i=0; i<AF_FOCUS_TOTAL_WIN; i++)
        {
            g_AFInfo.MultiValue[i][g_AFInfoIdx] = g_fpGetVariation(i);
        }
        g_AFInfo.VACnt[g_AFInfoIdx] = GetIpeVACnt();
        g_AFInfoIdx++;

        if(Lens_Focus_GetCurrentPosition() <= startIdx)
        {
            break;
        }

        Lens_Focus_Goto(Lens_Focus_GetCurrentPosition()-jumpStep);
    }

    // Step2. Analyze all VA windows and Choose the near-est one.
    for(i=0; i<AF_FOCUS_TOTAL_WIN; i++)
    {
        tmpIdx = 0;
        tmpMaxVA = 0;
        tmpMinVA = 0x0fffffff;
        for(j=0; j<AF_FOCUS_ARRAY_SIZE; j++)
        {
            if(g_AFInfo.MultiValue[i][j] == 0)
                break;

            if(tmpMaxVA < g_AFInfo.MultiValue[i][j])
            {
                tmpIdx = j;
                tmpMaxVA = g_AFInfo.MultiValue[i][j];
            }

            if(tmpMinVA > g_AFInfo.MultiValue[i][j])
                tmpMinVA = g_AFInfo.MultiValue[i][j];
        }

        focusIdxforAllWin[i] = tmpIdx;
        focusMaxVAforAllWin[i] = tmpMaxVA;
        focusMinVAforAllWin[i] = tmpMinVA;

        //debug_err(("@@@@ i = %d, tmpIdx = %d, maxVA = %d, minVA = %d\r\n", i, tmpIdx, tmpMaxVA, tmpMinVA));

        if(Lens_Zoom_GetSection() > 1)
        {
            if(tmpIdx > maxIdx && (tmpMaxVA-tmpMinVA)>=ST8005A_VA_Treshold)
            {
                maxIdx = tmpIdx;
                maxVA  = tmpMaxVA;
                winIdx = i;
            }
            else if(tmpIdx == maxIdx && (tmpMaxVA-tmpMinVA)>=ST8005A_VA_Treshold)
            {
                if(tmpMaxVA > maxVA)
                {
                    maxVA  = tmpMaxVA;
                    winIdx = i;
                }
            }
        }
        else
        {
        if(tmpIdx > maxIdx)
        {
            maxIdx = tmpIdx;
                maxVA  = tmpMaxVA;
            winIdx = i;
        }
        else if(tmpIdx == maxIdx)
        {
                if(tmpMaxVA > maxVA)
            {
                    maxVA  = tmpMaxVA;
                winIdx = i;
            }
        }
    }
    }

    // Step3. Copy g_AFInfo.MultiValue to g_AFInfo.Value.
    for(j=0; j<AF_FOCUS_ARRAY_SIZE; j++)
    {
        if(g_AFInfo.MultiValue[winIdx][j] == 0)
            break;

        if(minVA > g_AFInfo.MultiValue[winIdx][j])
            minVA = g_AFInfo.MultiValue[winIdx][j];

        g_AFInfo.Value[j] = g_AFInfo.MultiValue[winIdx][j];
        g_AFInfoIdx++;
    }

    // Step4. Set focus for all windows
    for(i=0; i<AF_FOCUS_TOTAL_WIN; i++)
    {
        if(Lens_Zoom_GetSection() > 1)
        {
            if(focusIdxforAllWin[i] == maxIdx && (focusMaxVAforAllWin[i]-focusMinVAforAllWin[i]) >= ST8005A_VA_Treshold)
                g_AFfocusforAllWin[i] = TRUE;
            else
                g_AFfocusforAllWin[i] = FALSE;
        }
        else
        {
            if(focusIdxforAllWin[i] == maxIdx)
                g_AFfocusforAllWin[i] = TRUE;
            else
                g_AFfocusforAllWin[i] = FALSE;
        }
    }

    //debug_err(("@@@@ winIdx = %d\r\n", winIdx));

    g_AFInfo.MaxVA=maxVA;
    *pMaxVA = maxVA;
    *pMinVA = minVA;
    *pWinIdx = winIdx;

    return g_AFInfo.Step[maxIdx];
}


INT32 AFAlgo_GuessMaxIdx_ST8005A(INT32 EndIdx, INT32 MaxIdx, UINT32 SearchStep, UINT32 *pContrastValue, UINT32 Cnt)
{
    INT32   GuessIdx;
    LONG    Temp;

    GuessIdx = MaxIdx;

    if(Cnt == 0)
    {
        Cnt = 1;    // Choose 0, 1, 2 three points
    }

    if(pContrastValue[Cnt-1] != 0 &&
       pContrastValue[Cnt]   != 0 &&
       pContrastValue[Cnt+1] != 0 &&
       (2*pContrastValue[Cnt] - pContrastValue[Cnt-1] - pContrastValue[Cnt+1]) != 0)
    {
        Temp = (LONG)(SearchStep*(pContrastValue[Cnt+1] - pContrastValue[Cnt-1]))/
               (LONG)(2*(2*pContrastValue[Cnt] - pContrastValue[Cnt-1] - pContrastValue[Cnt+1]));

        GuessIdx = MaxIdx - Temp;

        if(GuessIdx < MaxIdx-(SearchStep+1)/2)
            GuessIdx = MaxIdx-(SearchStep+1)/2;
        else if(GuessIdx > MaxIdx+(SearchStep+1)/2)
            GuessIdx = MaxIdx+(SearchStep+1)/2;

        if(GuessIdx > EndIdx)
            GuessIdx = EndIdx;
    }

    return GuessIdx;
}


/*
    @brief  AF for ST8005 2x lens

    @author Chris Chung
*/
INT32 AF_Process_ST8005(UINT32 startIdx, UINT32 endIdx)
{
    UINT32 jumpStep = 3, maxVA=0, minVA=0, i, winIdx;
    INT32 maxIdx;
    BOOL bFocusNG = FALSE;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return AF_STA_FAIL;
    }

    gAlgMsg.Alg_Mode=MODE_Captrue_Focusing;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FOCUS, &gAlgMsg);
    }

    AF_ResetAFInfo();
    g_AFInfo.StartIdx = startIdx;
    g_AFInfo.EndIdx   = endIdx;

    switch(Lens_Zoom_GetSection())
    {
    case 1:
        Lens_Focus_Goto(endIdx);
        jumpStep = 5;
        break;
    case 2:
        if(abs(Lens_Focus_GetCurrentPosition() - endIdx) >= 6)
        {
            Lens_Focus_Goto(endIdx);
        }
        jumpStep = 6;
        break;
    case 3:
        if(abs(Lens_Focus_GetCurrentPosition() - endIdx) >= 6)
        {
            Lens_Focus_Goto(endIdx);
        }
        jumpStep = 6;
        break;
    }

    if(gImageAlgInfo.AFWindow == _IMGAFWindow_Auto)
    {
        //multi AF
        maxIdx = AF_SearchFullRange_ST8005A_Multi(startIdx, endIdx, jumpStep, &maxVA, &minVA, 2, &winIdx); // 2011-05-17 Chris
        g_AFInfo.AF_Window = winIdx;

        bFocusNG = TRUE;
        for(i=0; i<AF_FOCUS_TOTAL_WIN; i++)
        {
            if(g_AFfocusforAllWin[i] == TRUE)
            {
                bFocusNG = FALSE;
                break;
            }
        }
    }
    else
    {
        //single AF
        maxIdx = AF_SearchFullRange_ST8005A_Single(startIdx, endIdx, jumpStep, &maxVA, &minVA, 2, FALSE); // 2011-05-17 Chris
        g_AFInfo.AF_Window = 4;
    }

    for(i=0; i<g_AFInfoIdx; i++)
    {
        g_AFGuessInfo.Value[i] = g_AFInfo.Value[i] - minVA + 1;
    }

    g_AFInfo.AF_GuessIdx = AFAlgo_GuessMaxIdx_ST8005A(g_AFInfo.Step[0], maxIdx, jumpStep, g_AFGuessInfo.Value, g_AFMaxIdx);
    g_AFInfo.MaxIdx = maxIdx;
//    gAlgMsg.AF_Window = g_AFInfo.AF_Window;
    gAlgMsg.Alg_Mode=MODE_Captrue_FocusEnd;

    if(g_AFInfo.AF_GuessIdx > 0)
    {
    /*
        if(Lens_Zoom_GetSection() > 1)
        {
            if((maxVA - minVA) < ST8005A_VA_Treshold)
                bFocusNG = TRUE;
        }
*/
        //#NT#2011/08/04#Hideo Lin -begin
        //#NT#Test for focus failed conditions
        debug_msg("^BmaxIdx=%d, startIdx=%d, minVA=%d, maxVA=%d\r\n", maxIdx, startIdx, minVA, maxVA);
        // VA too low
//        if (gAlgMsg.FaceDetectNum > 0)
        if ((fd_IsLock() == FALSE) && (gAlgMsg.FaceDetectNum != 0))
        {
            if (maxVA < 100)
            {
                debug_msg("^RFocus NG, VA too low in FD mode\r\n");
                bFocusNG = TRUE;
            }
        }
        else
        {
            if(Lens_Zoom_GetSection() > 1)
            {
                    if (maxVA < ST8005A_VA_Treshold)
                    {
                        debug_msg("^RFocus NG, VA too low\r\n");
                        bFocusNG = TRUE;
                    }
             }
            else
            {
                    if (maxVA < ST8005A_VA_LowLimit)
                    {
                        debug_msg("^RFocus NG, VA too low\r\n");
                        bFocusNG = TRUE;
                    }
            }
        }

        // Focus position too near
        if (maxIdx <= startIdx + jumpStep-3)
        {
            //#PIC#2011/08/16#david -begin
            //#change 92 --> 98
            if (((minVA*100)/maxVA > 98) ||                    // VA too close
            //#PIC#2011/08/16#david -end
                (maxVA < ST8005A_VA_Treshold))    // VA too low
            {
                debug_msg("^RFocus NG, too near, VA too close or too low\r\n");
                bFocusNG = TRUE;
            }
        }
        //#NT#2011/08/04#Hideo Lin -end
    }
    else
        bFocusNG = TRUE;

    if(bFocusNG == FALSE)
    {
        debug_msg("^BFocus OK\r\n");
        Lens_Focus_Goto(g_AFInfo.AF_GuessIdx);

        gAlgMsg.AF_Status=AFStatus_OK;

        if(g_fpAlgExport)
        {
            g_fpAlgExport(ALGMSG_FOCUSEND, &gAlgMsg);
        }

        return AF_STA_OK;
    }
    else
    {
        debug_msg("^RFocus NG\r\n");
        gAlgMsg.AF_Status=AFStatus_FAIL;

        if(g_fpAlgExport)
        {
            g_fpAlgExport(ALGMSG_FOCUSEND, &gAlgMsg);
        }

        return AF_STA_FAIL;
    }
}

//#NT#2012/08/06#KS Hung -begin
//#NT#Add the callback function of Detecting GPIO
/**
    Register FPAF_DETGPIOKEY function
    @param pafDetGPIOKey function
*/
void AF_RegisterFP_DetGPIOKey(FPAF_DETGPIOKEY pafDetGPIOKey)
{
    g_bfpAfDetShutter2 = pafDetGPIOKey;
}
//#NT#2012/08/06#KS Hung -end
