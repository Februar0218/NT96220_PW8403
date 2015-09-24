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
                if (AeArg.ISOGain <= 120)
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

    //#NT#2011/11/23#KS Hung -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    fpLensCalCB = Lens_GetCalInfo();
    switch(AFMode)
    {
    case _IMGAFMode_Macro:
        af_Obj->iStartIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, FALSE); // 2m
        af_Obj->iEndIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, TRUE);    //0.5m
        af_Obj->iStep = AF_Step;
        af_Obj->iDefaultIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, FALSE); // default for NG : 0.5m
        af_Obj->iOffsetStep = 0; // for heat compensation
        break;

    case _IMGAFMode_Normal:
    default:
        af_Obj->iStartIdx = fpLensCalCB(LENS_CAL_FAR_FOCUS,uiZoomSection, TRUE); // inf
        af_Obj->iEndIdx = fpLensCalCB(LENS_CAL_NEAR_FOCUS,uiZoomSection, FALSE); // near
        af_Obj->iStep = AF_Step;
        af_Obj->iDefaultIdx = fpLensCalCB(LENS_CAL_FAR_FOCUS,uiZoomSection, FALSE); // default for NG : 2m
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

    Addr += sprintf((char *)Addr,"AFTsk\t: OZoomIdx:%ld DZoomIdx:%d CurIdx:%ld Status:0x%ld\r\n",
        Lens_Zoom_GetSection(),
        gImageAlgInfo.DZoomIndex,
        Lens_Focus_GetCurrentPosition(),
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
    INT16 idx=0, maxIdx=0;
    UINT32 tempVA=0, maxVA=0;
    UINT32 contrastCnt;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    g_AFInfo.StartIdx = startIdx;
    g_AFInfo.EndIdx = endIdx;

    for(idx=startIdx; idx <= (endIdx+jumpStep-1); idx += jumpStep)
    {
        if (idx > endIdx)
        {
            Lens_Focus_Goto(endIdx);
        }
        else
        {
            Lens_Focus_Goto(idx);
        }

        //#NT#2012/05/11#KS Hung -begin
        //#NT#The contrast value is for current frame after delay 1 VD and wait ipe_getVACnt end
        if (idx == 0)
            sie_waitVD(3);
        else
            sie_waitVD(1);
        //#NT#2012/05/11#KS Hung -end

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = g_fpGetVariation(4);

        debug_err(("%s: step=%d, tempVA=%d, maxVA=%d\r\n", __func__, idx, tempVA, maxVA));

        if(g_AFInfoIdx<=AF_FOCUS_ARRAY_SIZE-1)
        {
            g_AFInfo.Step[g_AFInfoIdx] = idx;
            g_AFInfo.Value[g_AFInfoIdx] = tempVA;
            g_AFInfo.VACnt[g_AFInfoIdx] = ipe_getVACnt();
            g_AFInfoIdx++;
        }

        //#NT#2012/05/10#KS Hung -begin
        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
        else
        {
            if(tempVA < (maxVA * 0.8)) //0.8 is threshold.
            {
                debug_err(("find peak\r\n"));
                break;
            }
        }
        //#NT#2012/05/10#KS Hung -end
    }

    if (maxVA > 100)
    {
        gAlgMsg.AF_Status=AFStatus_OK;
    }
    else
    {
        gAlgMsg.AF_Status=AFStatus_FAIL;
    }

    g_AFInfo.MaxVA  = maxVA;
    g_AFInfo.MaxIdx = maxIdx;

    //Lens_Focus_Goto(maxIdx);
    if(pMaxVA != NULL)
    {
        *pMaxVA = maxVA;
    }
    debug_err(("%s: maxIdx=%d, maxVA=%d\r\n\r\n", __func__, maxIdx, maxVA));

    return maxIdx;
}

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

#define S5K3E2_VA_Treshold   1000
#define S5K3E2_VA_LowLimit   500

INT32 AF_SearchFullRange_S5K3E2_Single(INT32 startIdx, INT32 endIdx, UINT32 jumpStep, UINT32 *pMaxVA, UINT32 *pMinVA, UINT32 waitVDcount, BOOL bSkipRemainder)
{
    INT32   idx=0, maxIdx=0;
    UINT32  tempVA, maxVA=0, minVA=0xffffffff, j=0;
    UINT32  contrastCnt;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    g_AFInfo.StartIdx = startIdx;
    g_AFInfo.EndIdx = endIdx;

    for(idx=startIdx; idx <= (endIdx+jumpStep-1); idx += jumpStep)
    {
        if (idx > endIdx)
        {
            Lens_Focus_Goto(endIdx);
        }
        else
        {
            Lens_Focus_Goto(idx);
        }

        if (idx == 0)
            sie_waitVD(3);
        else
            sie_waitVD(1);

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = g_fpGetVariation(4);
        //debug_err(("step=%d, tempVA=%d, maxVA=%d\r\n",Lens_Focus_GetCurrentPosition(), tempVA, maxVA));
        g_AFInfo.Step[g_AFInfoIdx]  = idx;
        g_AFInfo.Value[g_AFInfoIdx] = tempVA;
        g_AFInfo.VACnt[g_AFInfoIdx] = ipe_getVACnt();
        g_AFInfoIdx++;

        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
            j=g_AFInfoIdx;
            g_AFMaxIdx = g_AFInfoIdx - 1;
        }
        else if(tempVA < minVA)
        {
            minVA = tempVA;
        }

        if ( ((idx - maxIdx) >= jumpStep) && (tempVA < maxVA*0.8) )  //20120527 by KS_Hung
        {
            debug_err(("find peak\r\n"));
            break;
        }
    }

    #if 0
    debug_msg("^Rg_AFInfoIdx  = %d\n\r",  g_AFInfoIdx);
    debug_msg("^Rg_AFMaxIdx  = %d\n\r",  g_AFMaxIdx);
    debug_msg("^Rg_AFInfo.Value[%d]  = %d\n\r",  g_AFMaxIdx-1, g_AFInfo.Value[g_AFMaxIdx-1]);
    debug_msg("^Rg_AFInfo.Value[%d]  = %d\n\r",  g_AFMaxIdx, g_AFInfo.Value[g_AFMaxIdx]);
    debug_msg("^Rg_AFInfo.Value[%d]  = %d\n\r",  g_AFMaxIdx+1, g_AFInfo.Value[g_AFMaxIdx+1]);
    #endif

    g_AFInfo.MaxVA=maxVA;
    g_AFInfo.MaxIdx = maxIdx;

    if ( (pMaxVA != NULL) && (pMinVA != NULL))
    {
        *pMaxVA = maxVA;
        *pMinVA = minVA;
    }

    return maxIdx;
}

INT32 AFAlgo_GuessMaxIdx_S5K3E2(INT32 StartIdx, INT32 MaxIdx, UINT32 SearchStep, UINT32 *pContrastValue, UINT32 Cnt)
{
    INT32   GuessIdx;
    LONG    Temp;

    GuessIdx = MaxIdx;

    if(Cnt == 0)
    {
        return GuessIdx;    //  This VCM has 0~1023 position and the zero step is infinite.
    }

    #if 0
    debug_msg("^R%s: Cnt = %d\n\r", __func__, Cnt);
    debug_msg("^R%s: StartIdx = %d\n\r", __func__, StartIdx);
    debug_msg("^R%s: pContrastValue[Cnt-1]  = %d\n\r", __func__, pContrastValue[Cnt-1]);
    debug_msg("^R%s: pContrastValue[Cnt]  = %d\n\r", __func__, pContrastValue[Cnt]);
    debug_msg("^R%s: pContrastValue[Cnt+1]  = %d\n\r", __func__, pContrastValue[Cnt+1]);
    #endif

    if(pContrastValue[Cnt-1] != 0 &&
       pContrastValue[Cnt]   != 0 &&
       pContrastValue[Cnt+1] != 0 &&
       (2*pContrastValue[Cnt] - pContrastValue[Cnt-1] - pContrastValue[Cnt+1]) != 0)
    {
        Temp = (LONG)(SearchStep*(pContrastValue[Cnt+1] - pContrastValue[Cnt-1]))/
               (LONG)(2*(2*pContrastValue[Cnt] - pContrastValue[Cnt-1] - pContrastValue[Cnt+1]));

        GuessIdx = MaxIdx + Temp;

        if(GuessIdx < MaxIdx-(SearchStep+1)/2)
            GuessIdx = MaxIdx-(SearchStep+1)/2;
        else if(GuessIdx > MaxIdx+(SearchStep+1)/2)
            GuessIdx = MaxIdx+(SearchStep+1)/2;

        if(GuessIdx < StartIdx)
            GuessIdx = StartIdx;
    }

    return GuessIdx;
}


/*
    @brief  AF for VCM-8552A (S5K3E2 sensor control) lens

    @author KS Hung
*/
INT32 AF_Process_S5K3E2(UINT32 startIdx, UINT32 endIdx, UINT32 jumpStep, UINT32 *pMaxVA)
{
    UINT32 maxVA=0, minVA=0;
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

    maxIdx = AF_SearchFullRange_S5K3E2_Single(startIdx, endIdx, jumpStep, &maxVA, &minVA, 1, FALSE);
    g_AFInfo.AF_Window = 4;

    /*for(i=0; i<g_AFInfoIdx; i++)
    {
        g_AFGuessInfo.Value[i] = g_AFInfo.Value[i] - minVA + 1;
    }*/

    if (g_AFMaxIdx == 0)
    {
        g_AFInfo.AF_GuessIdx = maxIdx;
    }
    else
    {
        g_AFInfo.AF_GuessIdx = AFAlgo_GuessMaxIdx_S5K3E2(startIdx, maxIdx, jumpStep, g_AFInfo.Value, g_AFMaxIdx);
    }
    debug_msg("^G%s: g_AFInfo.AF_GuessIdx = %d\r\n", __func__, g_AFInfo.AF_GuessIdx );
    g_AFInfo.MaxIdx = maxIdx;
    gAlgMsg.Alg_Mode=MODE_Captrue_FocusEnd;

    if(g_AFInfo.AF_GuessIdx >= 0)
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
        debug_msg("^GmaxIdx=%d, startIdx=%d, minVA=%d, maxVA=%d\r\n", maxIdx, startIdx, minVA, maxVA);
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
            if (maxVA < S5K3E2_VA_LowLimit)
            {
                debug_msg("^RFocus NG, VA too low\r\n");
                bFocusNG = TRUE;
            }
        }
        //#NT#2011/08/04#Hideo Lin -end
    }
    else
        bFocusNG = TRUE;

    if(bFocusNG == FALSE)
    {
        debug_msg("^GFocus OK\r\n");
        Lens_Focus_Goto(g_AFInfo.AF_GuessIdx);

        gAlgMsg.AF_Status=AFStatus_OK;

        /*if(g_fpAlgExport)
        {
            g_fpAlgExport(ALGMSG_FOCUSEND, &gAlgMsg);
        }*/

        //return AF_STA_OK;
    }
    else
    {
        debug_msg("^RFocus NG\r\n");
        gAlgMsg.AF_Status=AFStatus_FAIL;

        /*if(g_fpAlgExport)
        {
            g_fpAlgExport(ALGMSG_FOCUSEND, &gAlgMsg);
        }*/

        //return AF_STA_FAIL;
    }

    return maxIdx;
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