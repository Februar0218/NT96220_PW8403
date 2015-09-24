/**
    Auto Exposure flow.

    ae flow.

    @file       ae_sample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "type.h"
#include "debug.h"
#include "Ae_alg.h"
#include "Ae_MI5140_220_int.h"
#include "Ae_Api.h"

#include "ImageAlgInfor.h"
#include "aaa_lib.h"
#include "Sensor.h"
#include "sie_lib.h"
#include "Lens.h"
#include "i2c.h"
#include "utility.h"
#define AE_DEBUG_MODE DISABLE
#if (AE_DEBUG_MODE == ENABLE)
#define AE_IND(...)      debug_msg("AE: "__VA_ARGS__)
#define AE_ERR(...)      debug_msg("^RAE: "__VA_ARGS__)
#else
#define AE_IND(...)
#define AE_ERR(...)      debug_msg("^RAE: "__VA_ARGS__)
#endif

UINT32 GetCenterOverExpoRatio(UINT32 HistMaxNum, UINT32 *Hist);

UINT32 AEUartMsg = DISABLE;
static BOOL bRestFlg = FALSE;
static UINT8 (*AeWin)[AE_WIN_X] = 0;
static UINT32 AeFlag = 0;
static AE_INFO AeInfo = {0};
static AE_EVENT_TAB AEEventTAB = {0};
static AEALG_ARG PrvOldAeArg, PrvNewAeArg, CapNewAeArg;
static UINT32 GAcc[AE_WIN_X * AE_WIN_Y] = {0};
static UINT32 EVTable[(Max_EvValue + 1)] = {13,16,18,20,25,31,35,40,50,63,71,79,100,
                                        126,141,159,200,252,283,317,400,504,566,
                                        635,800};

static AEALG_EVENT_TAB AeAlgEventTab =
{
    GetOverExpoRatio,
    NULL,
    GetFnoRatio,
    NULL
};

UINT32 PreviousExpoTime;

static BOOL ChkRest(void);

UINT32 MI5140VALUE;
static void AEMI5140_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulReg2, ulData, ulData1, ulData2;

    i2c_lock();
    TimerDelayUs(100);//add for adpk downgrade wafer,ensure i2c stop and next start have enough time gap
    ulWriteAddr =0x78;
    ulReadAddr  =0x79;
    ulReg1      =(uiAddr>>8);
    ulReg2      =((uiAddr)&(0x000000ff));
    ulData      =0;
    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);
        if (erReturn != 0)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);
        if (erReturn != 0)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);

        if (erReturn != 0)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);
        if (erReturn !=  0)
        {
            debug_err(("Error transmit addr before receive!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData1, 0, 0);
        if (erReturn != 0)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData2, 1, 1);
        if (erReturn != 0)
        {
            debug_err(("Error receive data2!!\r\n"));
            break;
        }

    } while(erReturn != 0);
    ulData=((ulData1<<8)|(ulData2));

    *uiValue = ulData;
    i2c_unlock();
}


void AE_LoadEventTAB(AE_EVENT_TAB *EventTab)
{
    AEEventTAB = *EventTab;
}

void AE_Init(void)
{
    ImageAlgInfo *ImgInfo;

    //initial ae algorithm
    AEAlg_LoadEventTAB(&AeAlgEventTab);
    AE_getInfo(&AeInfo);

    //initial ae algorithm parameters
    aaa_SetAeRate(AE_RATE);
    AEAlg_SetPrvEVValue(AE_INIT_EV);

    //get ae table
    ImgInfo = UI_GetParameter();
    AE_getPrvTab(AE_getIsoType(ImgInfo->ISO), AE_getFlickerType(ImgInfo->Frequency), &AeInfo);

    //get & save iso, exposure time, iris
    AEAlg_Allot(AE_INIT_EV, AeInfo.PrvCurve, &PrvNewAeArg);
    AE_copyArg(&PrvNewAeArg, &PrvOldAeArg);
    AE_setFlag((AE_FLAG_SET_IRIS|AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT));
    AE_setExpT(PrvNewAeArg.ExpoTime);
    AE_setGain(PrvNewAeArg.ISOGain);
    AE_setIris(PrvNewAeArg.Iris);
    PreviousExpoTime = PrvNewAeArg.ExpoTime;    
}

void AE_Process (void)
{
    //UINT32 value;
    UINT32 Flag,uiImeY;
    ImageAlgInfo *ImgInfo;
    AE_RESET AeRst = AE_RESET_FALSE;
    ACTFBADDR_PRAM _FB_Act;
    EXPOSURE_SETTING expoinfo;
    ImgInfo = UI_GetParameter();
    //Get ime Y,only used by print msg
    GetActImgFBAddr_Path1(&_FB_Act);
    uiImeY = aaa_EstimateY2(_FB_Act.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));

    //set ae window
    AeRst |= AE_setWin(AE_getWinType(ImgInfo->AEMode));
    //get preview ae curve
    AeRst |= AE_getPrvTab(AE_getIsoType(ImgInfo->ISO), AE_getFlickerType(ImgInfo->Frequency), &AeInfo);
    //chk ae_reset is true or flase
    AeRst |= ChkRest();
    AeInfo.Reset = AeRst;

    //get capture ae curve
    AE_getCapTab(AE_getIsoType(ImgInfo->ISO), AE_getFlickerType(ImgInfo->Frequency), AE_getSceneType(ImgInfo->SceneMode), AE_getAntiShakeLv(ImgInfo->AntiShakeLevel), &AeInfo);

    // get statistic informaiton
    AeInfo.LumY = AE_getStatisticInfo(AEALG_HIST_NUM, AeInfo.Hist);

    /**** do something at here start****/
    //we can change ae information
    AeInfo.ExpectY = (AE_EXPECT_Y * EVTable[ImgInfo->AEEVValue]) / AE_EV_RATIO;
    AeInfo.PrvBiningMode = AE_getPrvBiningMode();
    /**** do something at here end  ****/
    //AE_ERR(" binning mode = %d\r\n",AE_getPrvBiningMode());
    //calculate ae
    AEAlg_Converge(&AeInfo, &PrvNewAeArg, &CapNewAeArg);

    AE_judgePrvSetting(&PrvNewAeArg, &PrvOldAeArg, &Flag);
    AE_copyArg(&PrvNewAeArg, &PrvOldAeArg);
    AE_setFlag(Flag);


    AE_setExpT(PrvNewAeArg.ExpoTime);
    //FF,iris marked
    //sie_waitVD(1);
    //AE_setIris(PrvNewAeArg.Iris);
   /* if((PreviousExpoTime != PrvNewAeArg.ExpoTime) && (PrvNewAeArg.ExpoTime>=10000))//&&(PrvNewAeArg.ISOGain=50))
    {

    }
    else if((PreviousExpoTime <= 10000) && (PrvNewAeArg.ExpoTime>=16666))
    {
        //sie_waitVD(2);
    }
    else*/
        sie_waitVD(1);
    Sensor_GetExpoSetting(&expoinfo);
 /*   if(expoinfo.uiExpoRatio!= 100)
    {
        AE_setFlag(AE_getFlag()|AE_FLAG_SET_GAIN);
    }
*/
//if (PrvNewAeArg.ISOGain > 300) PrvNewAeArg.ISOGain = 300;

    AE_setGain(PrvNewAeArg.ISOGain*expoinfo.uiExpoRatio/100);


    if (AEEventTAB.AeProcessEnd != NULL)
    {
        AEEventTAB.AeProcessEnd(PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime);
    }
    PreviousExpoTime = PrvNewAeArg.ExpoTime;    
 /*   AEMI5140_ReadReg(0x31FE, &MI5140VALUE);

    AEMI5140_ReadReg(0x305e,&value);
    AE_ERR("0x305E = 0x%4x\r\n", value);
    AEMI5140_ReadReg(0x3012,&value);
    AE_ERR("0x3012 = 0x%4x\r\n", value);
    AEMI5140_ReadReg(0x0340,&value);
    AE_ERR("0x3012 = 0x%4x\r\n", value);
*/
    //AE_ERR("%3d %3d %8d\r\n", AEAlg_GetPrvLV(), PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime);
  /*  if (1)
    {
        char            message[160];
        SenFP_ClearOSD();
        sprintf(message,"%d %d %d",ImgInfo->Frequency, PrvNewAeArg.ISOGain, MI5140VALUE);
        SenFP_ShowOSDString(message,50,60,4);
    }*/

#if 0
    {
        extern int sprintf(char * /*s*/, const char * /*format*/, ...);
        extern void SenFP_ShowOSDString(CHAR *pString, UINT16 uiX, UINT16 uiY, UINT16 uiColor);

        static char msg1[30], msg2[30];

        sprintf(msg1,"ExpT %7ld ISO %3ld", (int) PrvNewAeArg.ExpoTime, (int)PrvNewAeArg.ISOGain);
        SenFP_ShowOSDString(msg1,30,25,3);

        sprintf(msg2,"LumY %3ld PrvLV %3ld", (int)AeInfo.LumY, (int)AEAlg_GetPrvLV());
        SenFP_ShowOSDString(msg2,30,45,3);

    }
#endif
 
    if (AEUartMsg == ENABLE)
    {
        AE_ERR("%3d %3d %8d %3d %7d, %3d, %d, %d %d %d\r\n", AeInfo.LumY, AEAlg_GetPrvLV(), AEAlg_GetPrvEVValue(),
                    PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime, AEAlg_GetAdjExpectY(), PrvNewAeArg.Iris,AEAlg_GetStbCnt(),uiImeY,expoinfo.uiExpoRatio);
        debug_err(("----------------------------\r\n"));
    }
}

void AE_GetPrvAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
    *ExpT = PrvNewAeArg.ExpoTime;
    *ISO = PrvNewAeArg.ISOGain;
    *Iris = PrvNewAeArg.Iris;
}

void AE_GetCapAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{

    //else
    {
        *ExpT = CapNewAeArg.ExpoTime*15/10;
        *ISO = CapNewAeArg.ISOGain;
        /*
        if (AEAlg_GetCapLV() <= 4)
        *ISO = CapNewAeArg.ISOGain;
        else
        *ISO = CapNewAeArg.ISOGain/2;
        */
    }
/*
    if (CapNewAeArg.ISOGain <=50)
    {
        *ExpT = CapNewAeArg.ExpoTime/2;
        *ISO = CapNewAeArg.ISOGain*2;
    }
    */
    *Iris = CapNewAeArg.Iris;
}

void AE_WaitStable(UINT32 count)
{
    UINT32 VD_count = 0;
    UINT32 orgNewWeight=0,orgPrvWeight=0,orgConH=0,orgConL=0,orgRatioH=0,orgRatioL=0;
    BOOL orgbOverExpt = FALSE;
    PImageAlgInfo pimginfo;
    UINT32(* orgPrvCurve)[ AEALG_INFO_MAX ];

    pimginfo = UI_GetParameter();
    if (AEAlg_GetStbCnt() >= 2)
    {
        return;
    }
    //enlarge coverge condition
    orgNewWeight = AeInfo.EVNewWeight;
    orgPrvWeight = AeInfo.EVPrvWeight;
    orgRatioH = AeInfo.AdjRatioH;
    orgRatioL = AeInfo.AdjRatioL;
    orgConH = AeInfo.CovRangeH;
    orgConL = AeInfo.CovRangeL;
    orgbOverExpt = AeInfo.ReduceOverExpoModeEn;
    AeInfo.EVNewWeight = 1;
    AeInfo.EVPrvWeight = 0;
    AeInfo.AdjRatioH = 300;
    AeInfo.AdjRatioL = 20;
    AeInfo.CovRangeH = 10;
    AeInfo.CovRangeL = 10;
    AeInfo.ReduceOverExpoModeEn = FALSE;
    orgPrvCurve = AeInfo.PrvCurve;
    //debug_msg("^G %s:",__func__);
    AE_Process();
    if(AE_getFlickerType(pimginfo->Frequency) == AE_FLICKER_50HZ)
        AeInfo.PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_50Hz;
    else
        AeInfo.PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_60Hz;

    while (AEAlg_GetStbCnt() < 2)
    {
        sie_waitVD(2);
        //debug_msg("^G %s:",__func__);
        AE_Process();
        if(AEAlg_GetStbCnt() >= 2)
            break;
        VD_count += 2;
        if((VD_count > count * aaa_GetAeRate()))
        {
            break;
        }
    }
    AeInfo.PrvCurve = orgPrvCurve;
    AE_reset();//inorder to use org ae curve
    AeInfo.EVNewWeight = orgNewWeight;
    AeInfo.EVPrvWeight = orgPrvWeight;
    AeInfo.AdjRatioH = orgRatioH;
    AeInfo.AdjRatioL = orgRatioL;
    AeInfo.CovRangeH = orgConH;
    AeInfo.CovRangeL = orgConL;
    AeInfo.ReduceOverExpoModeEn = orgbOverExpt;
}

UINT32 AE_WriteDebugInfo(UINT32 Addr)
{
    AEMI5140_ReadReg(0x31FE, &MI5140VALUE);
    Addr += sprintf((char *)Addr, "ExpectY = \t%ld\r\nAdd_Y = \t%ld\r\nLumY = \t%ldStableCnt = \t%ld\r\n",
                                            AEAlg_GetAdjExpectY(), AEAlg_GetAdjY(), AeInfo.LumY,AEAlg_GetStbCnt());

    Addr += sprintf((char *)Addr, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AEAlg_GetPrvEVValue(), AEAlg_GetPrvLV());

    Addr += sprintf((char *)Addr, "CapEV_Value = \t%ld\r\nCapLV = \t%ld 0x31FE = 0x%lx\r\n",
                                            AEAlg_GetCapEVValue(), AEAlg_GetCapLV(),MI5140VALUE);
    Addr += sprintf((char *)Addr, "CapEV_Value = \t%ld\r\nCapLV = \t%ld\r\n",
                                            AEAlg_GetCapEVValue(), AEAlg_GetCapLV());

    Addr += sprintf((char *)Addr, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvNewAeArg.ISOGain, PrvNewAeArg.ExpoTime, PrvNewAeArg.Iris);

    Addr += sprintf((char *)Addr, "Cap_ISOGain = \t%ld\r\nCap_ExpoTime = \t%ld\r\nCap_Iris = \t%ld\r\n",
                                            CapNewAeArg.ISOGain, CapNewAeArg.ExpoTime, CapNewAeArg.Iris);
    return Addr;
}

void AE_GetHisto(UINT32 *Hist)
{
    UINT32 i;
    for (i = 0; i < AEALG_HIST_NUM; i ++)
    {
        Hist[i] = AeInfo.Hist[i];
    }
}

void AE_Cap2Prv(void)
{
    EXPOSURE_SETTING expoinfo;
    AE_setFlag((AE_FLAG_SET_IRIS|AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT));
    AE_setExpT(PrvNewAeArg.ExpoTime);
    Sensor_GetExpoSetting(&expoinfo);
    AE_setGain(PrvNewAeArg.ISOGain*expoinfo.uiExpoRatio/100);
    AE_setIris(PrvNewAeArg.Iris);
}

UINT32 AE_GetPrvCurLum(void)
{
    return AeInfo.LumY;
}
#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_judgePrvSetting(AEALG_ARG *CurInfo, AEALG_ARG *PrvInfo, UINT32 *Flag)
{
    UINT32 JudgeFlag;

    JudgeFlag = FALSE;
    *Flag = 0;

    if (PrvInfo->ExpoTime != CurInfo->ExpoTime)
    {
        *Flag |= AE_FLAG_SET_EXPT;
        JudgeFlag = TRUE;
    }

    if (PrvInfo->ISOGain != CurInfo->ISOGain)
    {
        *Flag |= AE_FLAG_SET_GAIN;
        JudgeFlag = TRUE;
    }

    if (PrvInfo->Iris != CurInfo->Iris)
    {
        *Flag |= AE_FLAG_SET_IRIS;
        JudgeFlag = TRUE;
    }

    if (JudgeFlag == TRUE)
    {
        return 1;
    }
    return 0;
}

UINT32 AE_getStatisticInfo(UINT32 HistSize, UINT32 *Hist)
{
    UINT32 i, j;
    UINT32 RTotal, GTotal, BTotal, WTotal;//, Lum;
    UINT32 ImgAddr,ImgH,ImgLinoft,ImgV,Cfa;

    Sen_GetSubOutImgInfo(&ImgH,&ImgLinoft,&ImgV,&ImgAddr,&Cfa);
    //get histogram result
    aaa_EstBlkAverBayerG(ImgAddr,ImgH,ImgV,ImgLinoft,Cfa,GAcc,AE_WIN_X*AE_WIN_Y,Hist,HistSize,HistSize);

    //calculate ae weight
    RTotal = 0;
    GTotal = 0;
    BTotal = 0;
    WTotal = 0;
    for (j = 0; j < AE_WIN_Y; j ++)
    {
        for (i = 0; i < AE_WIN_X; i ++)
        {
            //RTotal += (AeWin[j][i] * RAcc[j * AE_WIN_X + i]);
            GTotal += (AeWin[j][i] * GAcc[j * AE_WIN_X + i]);
            //BTotal += (AeWin[j][i] * BAcc[j * AE_WIN_X + i]);
            WTotal += AeWin[j][i];
        }
    }

    return (GTotal/WTotal);
}


void AE_getInfo(AE_INFO *AeInfo)
{
    AeInfo->EVAdjEn = ENABLE;
    AeInfo->ReduceOverExpoModeEn = AE_REDUCE_OVER_EXPO_MODE;
    AeInfo->CovRangeH = AE_COVER_RANGE_H;
    AeInfo->CovRangeL = AE_COVER_RANGE_L;
    AeInfo->PrvBiningMode = AE_getPrvBiningMode();
    AeInfo->CapBiningMode = AE_getCapBiningMode();
    AeInfo->AdjRatioH = AE_ADJUST_RATIO_H;
    AeInfo->AdjRatioL = AE_ADJUST_RATIO_L;
    AeInfo->EVPrvWeight = AE_SMOOTH_PRV_W;
    AeInfo->EVNewWeight = AE_SMOOTH_NEW_W;
    AeInfo->ExpectY = AE_EXPECT_Y;
    AeInfo->Reset = TRUE;
    AeInfo->MinEV = AE_MIN_EV;
    AeInfo->MaxEV = AE_MAX_EV;
    AeInfo->MinISOValue = AE_MIN_ISO;
    AeInfo->MaxISOValue = AE_MAX_ISO;
    AeInfo->OverExpoInfo.AdjYStep = AE_ADJ_Y_STEP;
    AeInfo->OverExpoInfo.OutdoorEV = AE_ADJ_OUTDOOR_EV;
    AeInfo->OverExpoInfo.IndoorEV = AE_ADJ_INDOOR_EV;
    AeInfo->OverExpoInfo.NightEV = AE_ADJ_NIGHT_EV;
    AeInfo->OverExpoInfo.OutdoorMaxAdjY = AE_ADJ_OUTDOOR_MAX_Y;
    AeInfo->OverExpoInfo.IndoorMaxAdjY = AE_ADJ_INDOOR_MAX_Y;
    AeInfo->OverExpoInfo.NightMaxAdjY = AE_ADJ_NIGHT_MAX_Y;
    AeInfo->OverExpoInfo.OverExpoTh = AE_ADJ_OVER_EXPO_TH;
    AeInfo->OverExpoInfo.UnderExpoTh = AE_ADJ_UNDER_EXPO_TH;
    AeInfo->OverExpoInfo.CenterOverExpoTh = AE_ADJ_CENTER_OVER_EXPO_TH;
}

AE_RESET AE_setWin(AE_WIN mode)
{
    static AE_WIN prvmode = 0xFF;
    if (mode == prvmode)
    {
        return AE_RESET_FALSE;
    }
    switch(mode)
    {
        case AE_WIN_CENTERWEIGHT:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_CenterWeighted;
            break;

        case AE_WIN_SPOT:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_Spot;
            break;

        case AE_WIN_MARTIX:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_Multi;
            break;

        default:
        case _IMGAEMode_Evaluative:
            AeWin = (UINT8(*)[AE_WIN_X])AE_Win_Evaluative;
            break;
    }
    prvmode = mode;
    return AE_RESET_TRUE;
//    AE_reset();
}

UINT32 AE_getFlag(void)
{
    return AeFlag;
}

void AE_setFlag(UINT32 flag)
{
    AeFlag |= flag;
}

void AE_clrFlag(UINT32 flag)
{
    AeFlag &= ~flag;
}

void AE_copyArg(AEALG_ARG *src, AEALG_ARG *dest)
{
    *dest = *src;
}

void AE_reset (void)
{
    AeInfo.Reset = TRUE;
    bRestFlg = TRUE;
}

static BOOL ChkRest(void)
{
    UINT32 flg = bRestFlg;
    bRestFlg = FALSE;
    return flg;
}

void AE_getCapTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_SCENE nScene, AE_ANTISHAKE nShakeLevel, AE_INFO *AeInfo)
{
    static UINT32 scene=0xff,iso=0xff,flicker=0xff,shakelevel=0xff;
    //ethan_20110807_rest ae when scene mode chg
    if((scene == nScene)&&(iso == nISO)&&(flicker == nFlicker)&&(shakelevel == nShakeLevel))
    {
        return;
    }
    if(nFlicker == AE_FLICKER_50HZ)
    {
        switch(nISO)
        {
            case AE_ISO_50:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO50_50Hz;
                break;
            case AE_ISO_100:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO100_50Hz;
                break;
            case AE_ISO_200:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_50Hz;
                break;
            case AE_ISO_400:
            case AE_ISO_800:
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_50Hz;
                break;

            case AE_ISO_AUTO:
            default:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Cap50;
                break;
        }
    }
    else//if(nFlicker==AE_FLICKER_60HZ)
    {
        switch(nISO)
        {
            case AE_ISO_50:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO50_60Hz;
                break;
            case AE_ISO_100:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO100_60Hz;
                break;
            case AE_ISO_200:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_60Hz;
                break;
            case AE_ISO_400:
            case AE_ISO_800:
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_60Hz;
                break;
            case AE_ISO_AUTO:
            default:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Cap60;
                break;
        }
    }
    if (nScene == AE_SCENE_SPORT)
    {
        AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Action;
    }
    else if (nScene == AE_SCENE_HIGHISO)
    {
        if(nFlicker == AE_FLICKER_50HZ)
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_50Hz;
        }
        else
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_60Hz;
        }
    }

    if (nShakeLevel != AE_ANTISHAKE_LV0)
    {
        if(nFlicker == AE_FLICKER_50HZ)
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Shake_50Hz;
        }
        else
        {
            AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Shake_60Hz;
        }
    }
    shakelevel = nShakeLevel;
    scene = nScene;
    iso = nISO;
    flicker = nFlicker;
    AE_reset();
}

AE_RESET AE_getPrvTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_INFO *AeInfo)
{
    static AE_FLICKER nPreFlicker = 0xFF;
    if (nPreFlicker == nFlicker)
    {
        return AE_RESET_FALSE;
    }

    if(nFlicker == AE_FLICKER_50HZ)
    {
        switch(nISO)
        {
            case AE_ISO_AUTO:
            case AE_ISO_25:
            case AE_ISO_50:
            case AE_ISO_80:
            case AE_ISO_100:
            case AE_ISO_200:
            case AE_ISO_400:
            case AE_ISO_800:
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
            default:
                AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker50;
                AeInfo->MaxEV = AE_MAX_EV;
                AeInfo->MinEV = AE_MIN_EV;
                AeInfo->MaxISOValue = AE_MAX_ISO;
                AeInfo->MinISOValue = AE_MIN_ISO;
                break;
        }
    }
    else //if(nFlicker==_POWERFreq_60Hz)
    {
        switch(nISO)
        {
            case AE_ISO_AUTO:
            case AE_ISO_25:
            case AE_ISO_50:
            case AE_ISO_80:
            case AE_ISO_100:
            case AE_ISO_200:
            case AE_ISO_400:
            case AE_ISO_800:
            case AE_ISO_1600:
            case AE_ISO_3200:
            case AE_ISO_6400:
            default:
                AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker60;
                AeInfo->MaxEV = AE_MAX_EV;
                AeInfo->MinEV = AE_MIN_EV;
                AeInfo->MaxISOValue = AE_MAX_ISO;
                AeInfo->MinISOValue = AE_MIN_ISO;
                break;
        }
    }
    nPreFlicker = nFlicker;


    //    AE_reset();

    return AE_RESET_TRUE;
}
#if 0
#endif
//------------------------------------------------------------------------------
UINT32 GetOverExpoRatio(UINT32 HistMaxNum, UINT32 *Hist)
{
    UINT32 Cnt;

    Cnt = (Hist[60] + Hist[61] + Hist[62] + Hist[63]) +
          (Hist[56] + Hist[57] + Hist[58] + Hist[59]) +
          (Hist[52] + Hist[53] + Hist[54] + Hist[55]) / 2; // >224  [224~239]+[240~225
    return Cnt;
}

UINT32 aHist[AEALG_HIST_NUM];
UINT32 aaa_BlockAverageY2(UINT32 YAddr,UINT32 x_start,UINT32 y_start,UINT32 width,UINT32 height,UINT32 offset, UINT32 *Hist)
{
    UINT16    i,j;
    UINT32    Yall=0,YAvg=0,Ypixel=0,cnt=0,tmp=0;
    UINT32    binnum;

    for(j=0;j<height;j+=4)
    {
        for(i=0;i<width;i+=16)
        {
            tmp=LCD_Y(YAddr,offset,(x_start+i),(y_start+j));
            Ypixel=(tmp    )&(0xff);
            Yall+=Ypixel;
            binnum=(Ypixel*AEALG_HIST_NUM)>>8;
            //*(volatile UINT32 *)(Hist+binnum);
            //(*(Hist+binnum))++;
            aHist[binnum]++;
            Ypixel=(tmp>>8 )&(0xff);
            Yall+=Ypixel;
            binnum=(Ypixel*AEALG_HIST_NUM)>>8;
            //(*(Hist+binnum))++;
            aHist[binnum]++;
            Ypixel=(tmp>>16)&(0xff);
            Yall+=Ypixel;
            binnum=(Ypixel*AEALG_HIST_NUM)>>8;
            //(*(Hist+binnum))++;
            aHist[binnum]++;
            Ypixel=(tmp>>24)&(0xff);
            Yall+=Ypixel;
            binnum=(Ypixel*AEALG_HIST_NUM)>>8;
            //(*(Hist+binnum))++;
            aHist[binnum]++;

            cnt+=4;
        }
    }
    if (cnt == 0) cnt = 1;
    YAvg=Yall/cnt;
    return (YAvg);
}

UINT32 GetCenterOverExpoRatio(UINT32 HistMaxNum, UINT32 *Hist)
{
    UINT16 i,j;
    UINT32 hist_total,temp;
    UINT32 h_size, v_size, offset, YAddr;
    UINT32 Cnt;
    ACTFBADDR_PRAM _FB_Act;

    GetActImgFBAddr_Path1(&_FB_Act);
    YAddr = _FB_Act.y_addr;
    h_size = GetIPPSizeInfor(_ImeOut1Hsize);
    v_size = GetIPPSizeInfor(_ImeOut1Vsize);
    offset = GetIPPSizeInfor(_ImeOut1LineOffset);
    for(i=0;i<AEALG_HIST_NUM;i++)
    {
        aHist[i] = 0;
    }

    for(i=3;i<=5;i++)
    {
        for(j=2;j<=5;j++)
        {
            aaa_BlockAverageY2(YAddr,((j*h_size)/CA_WINDOW_X),((i*v_size)/CA_WINDOW_Y),(h_size/CA_WINDOW_X),(v_size/CA_WINDOW_Y),offset, aHist);
        }
    }
    hist_total =0;
    for(i=0;i<AEALG_HIST_NUM;i++)
    {
        hist_total+=aHist[i];
    }
    if (hist_total == 0) hist_total = 1;
    for(i=0;i<AEALG_HIST_NUM;i++)
    {
        temp=1024*aHist[i]/hist_total;
        *(aHist+i)=(UINT32)temp;
    }

    Cnt = (aHist[60] + aHist[61] + aHist[62] + aHist[63]) +
          (aHist[56] + aHist[57] + aHist[58] + aHist[59]) +
          (aHist[52] + aHist[53] + aHist[54] + aHist[55]) / 2; // >224  [224~239]+[240~225
    return Cnt;
}


UINT32 GetFnoRatio(AEALG_IRIS_IDX Idx)
{
    UINT32 FN;
    LENS_APERTURE_ACT iris;
    if(Idx == AEALG_IRIS_0)
        iris = LENS_APERTURE_BIG;
    else
        iris = LENS_APERTURE_SMALL;
    FN = 28;//Lens_Aperture_GetFNO(iris,Lens_Zoom_GetSection());//FF lens
    return ( FN * FN / 10);
}
#if 0
#endif
//------------------------------------------------------------------------------

AE_SCENE AE_getSceneType(UINT32 Idx)
{
    if (Idx == _SceneMode_Auto) return AE_SCENE_AUTO;
    if (Idx == _SceneMode_Landscape) return AE_SCENE_LANDSCAPE;
    if (Idx == _SceneMode_Potrait) return AE_SCENE_POTRAIT;
    if (Idx == _SceneMode_Sport) return AE_SCENE_SPORT;
    if (Idx == _SceneMode_NightScene) return AE_SCENE_NIGHTSCENE;
    if (Idx == _SceneMode_AV) return AE_SCENE_AV;
    if (Idx == _SceneMode_TV) return AE_SCENE_TV;
    if (Idx == _SceneMode_Manual) return AE_SCENE_MANUAL;
    if (Idx == _SceneMode_NightPotrait) return AE_SCENE_NIGHTPOTRAIT;
    if (Idx == _SceneMode_NightLandscape) return AE_SCENE_NIGHTLANDSCAPE;
    if (Idx == _SceneMode_Party) return AE_SCENE_PARTY;
    if (Idx == _SceneMode_SunSet) return AE_SCENE_SUNSET;
    if (Idx == _SceneMode_BackLight) return AE_SCENE_BACKLIGHT;
    if (Idx == _SceneMode_Document) return AE_SCENE_DOCUMENT;
    if (Idx == _SceneMode_Firework) return AE_SCENE_FIREWORK;
    if (Idx == _SceneMode_Beach) return AE_SCENE_BEACH;
    if (Idx == _SceneMode_BMode) return AE_SCENE_BMODE;
    if (Idx == _SceneMode_Program) return AE_SCENE_PROGRAM;
    if (Idx == _SceneMode_HighISO) return AE_SCENE_HIGHISO;
    return AE_SCENE_AUTO;
}

AE_FLICKER AE_getFlickerType(UINT32 Idx)
{
    if (Idx == _POWERFreq_50Hz) return AE_FLICKER_50HZ;
    return AE_FLICKER_60HZ;
}

AE_ANTISHAKE AE_getAntiShakeLv(UINT32 Idx)
{
    if (Idx == _Level0) return AE_ANTISHAKE_LV0;
    if (Idx == _Level1) return AE_ANTISHAKE_LV1;
    if (Idx == _Level2) return AE_ANTISHAKE_LV2;
    return AE_ANTISHAKE_LV0;
}

AE_ISO AE_getIsoType(UINT32 Idx)
{
    if (Idx == _IMGISO_Auto) return AE_ISO_AUTO;
    if (Idx == _IMGISO_50) return AE_ISO_50;
    if (Idx == _IMGISO_100) return AE_ISO_100;
    if (Idx == _IMGISO_200) return AE_ISO_200;
    if (Idx == _IMGISO_400) return AE_ISO_400;
    if (Idx == _IMGISO_800) return AE_ISO_800;
    return AE_ISO_AUTO;
}

AE_WIN AE_getWinType(UINT32 Idx)
{
    if (Idx == _IMGAEMode_CenterWeighted) return AE_WIN_CENTERWEIGHT;
    if (Idx == _IMGAEMode_Spot) return AE_WIN_SPOT;
    if (Idx == _IMGAEMode_Matrix) return AE_WIN_MARTIX;
    if (Idx == _IMGAEMode_Evaluative) return AE_WIN_EVALUATIVE;
    return AE_WIN_CENTERWEIGHT;
}

#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_getPrvBiningMode(void)
{
    //#NT#2011/11/24#Ethan Lau -begin
    //#NT#replace sensor driver api
    SENSOR_STATUS SensorStatus = {0};

    Sensor_GetStatus(&SensorStatus);
    return (SensorStatus.uiBinning * 100);
    //#NT#2011/11/24#Ethan Lau -end
}

UINT32 AE_getCapBiningMode(void)
{
    return 100;
}

void AE_setExpT(UINT32 ExpT)
{
    if ((AE_getFlag() & AE_FLAG_SET_EXPT) == AE_FLAG_SET_EXPT)
    {
        Sensor_SetExpTime(ExpT);
        AE_clrFlag(AE_FLAG_SET_EXPT);
    }
}

void AE_setGain(UINT32 ISO)
{
    if ((AE_getFlag() & AE_FLAG_SET_GAIN) == AE_FLAG_SET_GAIN)
    {
        Sensor_SetISO(ISO);
        AE_clrFlag(AE_FLAG_SET_GAIN);
    }
}

void AE_setIris(AEALG_IRIS_IDX Idx)
{
    if ((AE_getFlag() & AE_FLAG_SET_IRIS) == AE_FLAG_SET_IRIS)
    {
        /**** do something at here start****/
        /**** do something at here end  ****/
        AE_clrFlag(AE_FLAG_SET_IRIS);
    }
}
