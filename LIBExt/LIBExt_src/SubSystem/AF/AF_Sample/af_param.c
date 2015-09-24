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

    //#NT#2012/07/03#KS Hung -begin
    //#NT#Use lens calibration callback instead of UI calibration function
    fpLensCalCB = Lens_GetCalInfo();
    if (fpLensCalCB==NULL)
    {
        return Addr;
    }
    for(i=1; i<=Lens_Zoom_GetMaxSection(); i++)
    {
        Addr += sprintf((char *)Addr,"Lens: S=%d, Near:%ld Far:%ld\r\n",
            i,
            fpLensCalCB(LENS_CAL_NEAR_FOCUS, i, FALSE),
            fpLensCalCB(LENS_CAL_FAR_FOCUS, i, TRUE)
            );
    }
    //#NT#2012/07/03#KS Hung -end

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
    INT16 idx, maxIdx=0;
    UINT32 tempVA=0, maxVA=0;
    UINT32 contrastCnt;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    for(idx=startIdx; idx <= (endIdx+jumpStep-1); idx += jumpStep)
    {
        //#NT#2012/08/06#KS Hung -begin
        //#NT#If the shutter2 key is pressed, escape the AF flow.
        #if 0
        if ( (g_bfpAfDetShutter2 != NULL) && (g_bfpAfDetShutter2() == TRUE) )
        {
            break;
        }
        #endif
        //#NT#2012/08/06#KS Hung -end
        Lens_Focus_Goto(idx);
        sie_waitVD(3);  // 2011-04-27 Chris

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = g_fpGetVariation(4);

        //debug_err(("%s: step=%d, tempVA=%d, maxVA=%d\r\n", __func__, idx, tempVA, maxVA));

        if(g_AFInfoIdx<=AF_FOCUS_ARRAY_SIZE-1)
        {
            g_AFInfo.Step[g_AFInfoIdx] = idx;
            g_AFInfo.Value[g_AFInfoIdx] = tempVA;
            g_AFInfo.VACnt[g_AFInfoIdx] = ipe_getVACnt();
            g_AFInfoIdx++;
        }

        //#NT#2012/01/06#KS Hung -begin
        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
        else
        {
            if(tempVA < (maxVA * 0.6)) //0.6 is threshold.
            {
                debug_err(("find peak\r\n"));
                break;
            }
        }
        //#NT#2012/01/06#KS Hung -end
    }

    //Lens_Focus_Goto(maxIdx);
    if(pMaxVA != NULL)
        *pMaxVA = maxVA;

    //debug_err(("%s: maxIdx=%d, maxVA=%d\r\n", __func__, maxIdx, maxVA));

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
