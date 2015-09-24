/**
    calibration API


    @file       cal_api.c
    @ingroup    calibration
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include <String.h>
#include "Calibration.h"
#include "dpc_alg.h"
#include "Alg_IPLMode.h"
#include "Debug.h"
#include "ImageAlgInfor.h"
#include "aaa_lib.h"
#include "fd_lib.h"
#include "sd_lib.h"
#include "sensor.h"
#include "OSCommon.h"
#include "Sen_Common.h"

typedef enum
{
    CAL_STATE_NONE,
    CAL_STATE_IDLE,
    CAL_STATE_INIT,

    CAL_STATE_CHGMODE,
    CAL_STATE_LOCK3A,
    CAL_STATE_GETRAWSIZE,
    CAL_STATE_GETRAW,
    CAL_STATE_SAVERAW,
    CAL_STATE_GETSTATISTICS,

    CAL_STATE_DPC,
    CAL_STATE_ISO,
    CAL_STATE_AWB,
    CAL_STATE_MSHT,
    CAL_STATE_ECS,
    CAL_STATE_OB,   // [NTK#RandyHo]<2012/04/28>

    ENUM_DUMMY4WORD(CAL_STATE)
} CAL_STATE;

static CAL_APPOBJ gCalCtrlObj;
static CAL_STATE uiCalibrationState = CAL_STATE_NONE;
static UINT32 Cal_semid = SEMID_CALIBRATION;
static CAL_DATA gCalStructData = {0};

static void Cal_Module_SetState(CAL_STATE state)
{
    wai_sem(Cal_semid);

    uiCalibrationState = state;

    sig_sem(Cal_semid);
}

static CAL_STATE Cal_Module_GetState(void)
{
    return uiCalibrationState;
}

void Cal_Init(PCAL_APPOBJ pCalAppObj)
{
    if(pCalAppObj->uiMemoryAddr == NULL || pCalAppObj->uiMemorySize == NULL)
    {
        debug_err(("%s: Memory Not Error!!\r\n", __func__));
        return;
    }

    if(pCalAppObj->pCalApi == NULL)
    {
        debug_err(("%s: Calibration not initial!!\r\n", __func__));
        return;
    }

    Cal_Module_SetState(CAL_STATE_INIT);

    // Hook calibration info
    memcpy(&gCalCtrlObj, pCalAppObj, sizeof(CAL_APPOBJ));

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_SetMode(UINT32 IppMode, UINT32 SenMode)
{
    Cal_Module_SetState(CAL_STATE_CHGMODE);

    gCalCtrlObj.pCalApi->fpSetMode(IppMode, SenMode);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_Lock3A(UINT32 isLock)
{
    Cal_Module_SetState(CAL_STATE_LOCK3A);

    gCalCtrlObj.pCalApi->fpLock3A(isLock);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_GetRAWSize(UINT32 Type, CAL_RAW_INFO* RawInfo)
{
    Cal_Module_SetState(CAL_STATE_GETRAWSIZE);

    gCalCtrlObj.pCalApi->fpGetRAWSize(Type, RawInfo);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_GetPrvAddr(UINT32* Addr)
{
    Cal_Module_SetState(CAL_STATE_GETRAWSIZE);

    gCalCtrlObj.pCalApi->fpGetPrvAddr(Addr);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_GetPrvRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath)
{
    Cal_Module_SetState(CAL_STATE_GETRAW);

    gCalCtrlObj.pCalApi->fpGetPrvRAW(RawAddr, RawSizeH, RawSizeV, ObOfs, SiePath);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_GetCapRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath, UINT32 ShutterTimeUs)
{
    Cal_Module_SetState(CAL_STATE_GETRAW);

    gCalCtrlObj.pCalApi->fpGetCapRAW(RawAddr, RawSizeH, RawSizeV, ObOfs, SiePath, ShutterTimeUs);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_SaveRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV)
{
    Cal_Module_SetState(CAL_STATE_SAVERAW);

    gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawSizeH, RawSizeV);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_GetStatistics(UINT32 CAType, UINT32 Addr, CAL_RAW_INFO RawInfo, UINT16 *pStatistics)
{
    Cal_Module_SetState(CAL_STATE_GETSTATISTICS);

    gCalCtrlObj.pCalApi->fpGetStatistics(CAType, Addr, RawInfo, pStatistics);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_WaitAeStable(CAL_AE_INFO *AeInfo)
{
    //CAL_AE_INFO CurAeInfo;
    //UINT16 Y=0, R=0, G=0, B=0;
    UINT16 RGB[4];
    //UINT32 i;
    UINT32 status = CAL_AE_RUNNING;
    CAL_RAW_INFO PrvRawInfo;
    UINT32 PrvAddr;

    // initial ae setting
    //memcpy(&CurAeInfo, &AeInfo, sizeof(CAL_AE_INFO));

    gCalCtrlObj.pCalApi->fpGetPrvAddr(&PrvAddr);

    gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_PREVIEW, &PrvRawInfo);

    do
    {
        Sensor_SetExpTime(AeInfo->uiExpT);
        sie_waitVD(1);
        Sensor_SetISO(AeInfo->uiISO);
        sie_waitVD(3);
        if(AeInfo->uiAEType == AE_FIX_TIMEnGAIN)
            break;

        if(AeInfo->uiCAType != CAL_CASOURCE_SUBOUT)
        {
            debug_err(("^R Calibration Preview ae raw type is supportted only by (CAL_CASOURCE_SUBOUT)\r\n"));
            AeInfo->uiCAType = CAL_CASOURCE_SUBOUT;
        }
        gCalCtrlObj.pCalApi->fpGetStatistics(AeInfo->uiCAType, PrvAddr, PrvRawInfo, &RGB[0]);

        AeInfo->uiLum = RGB[0]; // Y

        debug_msg(">>Yavg=%d, target=%d, ExpoT=%d, ISO=%d\r\n", AeInfo->uiLum, AeInfo->uiTarget,AeInfo->uiExpT,AeInfo->uiISO);
        //infor which show on panel
        //sprintf(message, "Y=%ld", AeInfo->Y);
        //Cal_AlgShowString(message,Cal_PanelMsgXPos(15),Cal_PanelMsgYPos(16));

        if (AeInfo->uiLum == 0)
            AeInfo->uiLum = 1;

        status = gCalCtrlObj.pCalApi->fpAEProcess(AeInfo);
    }while(status == CAL_AE_RUNNING);
}

void Cal_DPCAdjust(CAL_DPC_PARAM* pDPCParam)
{
    UINT32 RawAddr = gCalCtrlObj.uiMemoryAddr;
    CAL_RAW_INFO RawInfo;
    DPC_ALG_INFO DPCSetting;
    UINT32 DPNum;
    SENSOR_STATUS sen_status;

    if (Cal_Module_GetState() != CAL_STATE_IDLE)
    {
        debug_err(("Cal_DPCAdjust Fail!\r\n"));
        return;
    }

    Cal_Module_SetState(CAL_STATE_DPC);


    //ethan_20110729_inorder to get right cur sensor mode ob
    Sensor_GetStatus(&sen_status);
    pDPCParam->uiOBofs = sen_status.pSyncInfo->OB_Offset;

    memcpy(&DPCSetting, &pDPCParam->DPCInfo, sizeof(DPC_ALG_INFO));

    if (DPCSetting.Type == DPC_TYPE_BRIGHT_PREVIEW ||\
        DPCSetting.Type == DPC_TYPE_DARK_PREVIEW)
    {
        debug_msg("~~~~~~Start Preview dp~~~~~~~~~~\r\n");
        /**************** preview DP *********************/
        gCalCtrlObj.pCalApi->fpGetPrvAddr(&RawAddr);

        gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_OFF, 0);

        gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_PREVIEW, &RawInfo);

        gCalCtrlObj.pCalApi->fpGetPrvRAW(RawAddr,
                                        RawInfo.uiTotalWidth,
                                        RawInfo.uiTotalHeight,
                                        pDPCParam->uiOBofs,
                                        PATH_POST_GAMMA);

        if (pDPCParam->bSaveRaw == TRUE)
            gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight);

        DPCSetting.BayAddr = RawAddr;
        DPC_SetSearchInfo(DPCSetting);

        DPNum = DPC_DataSearch(DPC_SEARCH_METHOD_BAYER);
    }
    else
    {
        debug_msg("~~~~~~Start Capture dp~~~~~~~~~~\r\n");
        /**************** Capture DP *********************/
        //chg sensor/ipp 2 capture mode
        gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_CAP, SENSOR_MODE_FULL);
        //ethan_20110729_inorder to get right cur sensor mode ob
        Sensor_GetStatus(&sen_status);
        pDPCParam->uiOBofs = sen_status.pSyncInfo->OB_Offset;

        Sensor_SetExpTime(pDPCParam->uiTime);
        sie_waitVD(1);
        Sensor_SetISO(pDPCParam->uiISO);
        sie_waitVD(3);

        gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_STILL, &RawInfo);

        gCalCtrlObj.pCalApi->fpGetCapRAW(RawAddr,
                                        RawInfo.uiTotalWidth,
                                        RawInfo.uiTotalHeight,
                                        pDPCParam->uiOBofs,
                                        PATH_POST_GAMMA,
                                        pDPCParam->uiMsht);

        if (pDPCParam->bSaveRaw == TRUE)
            gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight);

        DPCSetting.BayAddr = RawAddr;
        DPC_SetSearchInfo(DPCSetting);

        DPNum = DPC_DataSearch(DPC_SEARCH_METHOD_BAYER);
    }
    // report result
    pDPCParam->uiDPNum = DPNum;

    // sorting
    DPC_SortDPdata((UW*)DPCSetting.DPAddr, DPNum);
    (*(unsigned long *)(DPCSetting.DPAddr + (DPNum << 2))) = 0xFFFFFFFF;

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_ISOAdjust(CAL_ISO_PARAM* pISOParam)
{
    UINT16 RGB[4];
    CAL_RAW_INFO PrvRawInfo;
    UINT32 PrvAddr, Status=0,LastStatus=0, Counter=0,CalStatus;

    if (Cal_Module_GetState() != CAL_STATE_IDLE)
    {
        debug_err(("Cal_ISOAdjust Fail!\r\n"));
        return;
    }

    Cal_Module_SetState(CAL_STATE_ISO);

    // Set IPP to preview
    gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_PREVIEW, pISOParam->uiPrvMode);

    gCalCtrlObj.pCalApi->fpGetPrvAddr(&PrvAddr);

    gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_PREVIEW, &PrvRawInfo);

    CalStatus = _ISO_Status;
    Cal_SetCalData(CAL_DATANUM_STATUS_ISO,  (UINT*)&CalStatus);

    while(1)
    {
        Cal_SetCalData(CAL_DATANUM_ISO,  (UINT*)&pISOParam->uiGain);
        Sensor_SetExpTime(pISOParam->uiTime);
        sie_waitVD(2);
        Sensor_SetISO(pISOParam->uiISOBase);
        sie_waitVD(3);

        gCalCtrlObj.pCalApi->fpGetStatistics(CAL_CASOURCE_SUBOUT, PrvAddr, PrvRawInfo, &RGB[0]);

        debug_err(("%d %d %d\r\n", RGB[0], pISOParam->uiTime, pISOParam->uiGain));

        if (RGB[0] > pISOParam->uiTarget)
        {
            pISOParam->uiGain -= pISOParam->uiAdjStep;
            Status = 0;
        }
        else if(RGB[0] < pISOParam->uiTarget)
        {
            pISOParam->uiGain += pISOParam->uiAdjStep;
            Status = 1;
        }
        else
        {
            break;
        }

        if(pISOParam->uiAdjStep == 1)
        {
            break;
        }
        if(Status != LastStatus)
            pISOParam->uiAdjStep = (pISOParam->uiAdjStep + 1) / 2;
        LastStatus = Status;

        if(Counter++ > 200)
            break;
    }

    CalStatus = 0;
    Cal_SetCalData(CAL_DATANUM_STATUS_ISO,  (UINT*)&CalStatus);

    // report result
    pISOParam->uiISOBaseGain = pISOParam->uiGain;

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_AWBAdjust(CAL_AWB_PARAM* pAWBParam)
{
    CAL_RAW_INFO RawInfo = {0};
    UINT32 RawAddr = gCalCtrlObj.uiMemoryAddr;

    if (Cal_Module_GetState() != CAL_STATE_IDLE)
    {
        debug_err(("Cal_AWBAdjust Fail!\r\n"));
        return;
    }

    Cal_Module_SetState(CAL_STATE_AWB);

    if (pAWBParam->uiAWBType == CAL_AWBTYPE_PREVIEW)
    {
        gCalCtrlObj.pCalApi->fpGetPrvAddr(&RawAddr);

        gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_OFF, 0);

        // Get scaled down raw size
        gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_PREVIEW, &RawInfo);

        gCalCtrlObj.pCalApi->fpGetPrvRAW(RawAddr,
                                        RawInfo.uiTotalWidth,
                                        RawInfo.uiTotalHeight,
                                        pAWBParam->uiOBofs,
                                        PATH_PRE_COLORGAIN);

        if (pAWBParam->bSaveRaw == TRUE)
            gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight);

        gCalCtrlObj.pCalApi->fpGetStatistics(CAL_CASOURCE_RAW, RawAddr, RawInfo, &pAWBParam->uiRGB[0]);
    }
    else if (pAWBParam->uiAWBType == CAL_AWBTYPE_CAPTURE)
    {
         gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_CAP, SENSOR_MODE_FULL);

        Sensor_SetExpTime(pAWBParam->uiExpT);
        sie_waitVD(1);
        Sensor_SetISO(pAWBParam->uiISO);
        sie_waitVD(3);

        gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_STILL, &RawInfo);

        gCalCtrlObj.pCalApi->fpGetCapRAW(RawAddr,
                                        RawInfo.uiTotalWidth,
                                        RawInfo.uiTotalHeight,
                                        pAWBParam->uiOBofs,
                                        PATH_PRE_COLORGAIN,
                                        pAWBParam->uiMsht);

        if (pAWBParam->bSaveRaw == TRUE)
            gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight);

        gCalCtrlObj.pCalApi->fpGetStatistics(CAL_CASOURCE_RAW, RawAddr, RawInfo, &pAWBParam->uiRGB[0]);
    }

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_MshtAdjust(CAL_MSHT_PARAM* pMSHTParam)
{
    CAL_RAW_INFO RawInfo = {0};
    UINT32 RawAddr = gCalCtrlObj.uiMemoryAddr;
    UINT32 PreY = 65;

    if (Cal_Module_GetState() != CAL_STATE_IDLE)
    {
        debug_err(("Cal_AWBAdjust Fail!\r\n"));
        return;
    }

    Cal_Module_SetState(CAL_STATE_MSHT);

    while(1)
    {
        // Lock 3A, FD and SD
        Cal_Lock3A(1);
        sie_waitVD(6);

        Sensor_SetCalMshutterDelayTime(pMSHTParam->uiMsht);

        gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_CAP, SENSOR_MODE_FULL);

        Sensor_SetExpTime(pMSHTParam->uiExpT);
        sie_waitVD(1);
        Sensor_SetISO(pMSHTParam->uiISO);
        sie_waitVD(3);

        gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_STILL, &RawInfo);

        gCalCtrlObj.pCalApi->fpGetCapRAW(RawAddr,
                                        RawInfo.uiTotalWidth,
                                        RawInfo.uiTotalHeight,
                                        pMSHTParam->uiOBofs,
                                        PATH_PRE_COLORGAIN,//PATH_PRE_GAMMA,
                                        pMSHTParam->uiMsht);

        Lens_Shutter_Move(LENS_SHUTTER_OPEN, TRUE);

        if (pMSHTParam->bSaveRaw == TRUE)
            gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight);

        //Sensor_Cap2Prv();
        gCalCtrlObj.pCalApi->fpGetStatistics(CAL_CASOURCE_RAW, RawAddr, RawInfo, &pMSHTParam->uiRGB[0]);

        debug_err(("Y=%d Mshutter=%d\r\n", pMSHTParam->uiRGB[0], pMSHTParam->uiMsht));

        // Set IPP to preview
        gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_PREVIEW, pMSHTParam->uiPrvMode);

        if((pMSHTParam->uiRGB[0] <= PreY*105/100) && \
            (pMSHTParam->uiRGB[0] > PreY*100/105))
        {
            break;
        }
        else if (pMSHTParam->uiRGB[0] > PreY*115/100)
        {
            pMSHTParam->uiMsht += 100;
            if(pMSHTParam->uiMsht > pMSHTParam->uiMshtMax)
            {
                //pMSHTParam->uiMsht = Default_Mshutter;
                break;
            }
        }
        else if (pMSHTParam->uiRGB[0] < PreY*115/100)
        {
            pMSHTParam->uiMsht -= 100;
            if(pMSHTParam->uiMsht < pMSHTParam->uiMshtMin)
            {
                //pMSHTParam->uiMsht = Default_Mshutter;
                break;
            }
        }
        else if (pMSHTParam->uiRGB[0] > PreY*107/100)
        {
            pMSHTParam->uiMsht +=50;
            if(pMSHTParam->uiMsht > pMSHTParam->uiMshtMax)
            {
                //pMSHTParam->uiMsht = Default_Mshutter;
                break;
            }
        }
        else if (pMSHTParam->uiRGB[0] < PreY*107/100)
        {
            pMSHTParam->uiMsht -=50;
            if(pMSHTParam->uiMsht < pMSHTParam->uiMshtMin)
            {
                //pMSHTParam->uiMsht = Default_Mshutter;
                break;
            }
        }
        else if (pMSHTParam->uiRGB[0] > PreY*105/100)
        {
            pMSHTParam->uiMsht +=20;
            if(pMSHTParam->uiMsht > pMSHTParam->uiMshtMax)
            {
                //pMSHTParam->uiMsht = Default_Mshutter;
                break;
            }
        }
        else// if (pMSHTParam->uiRGB[0] < PreY*105)
        {
            pMSHTParam->uiMsht -=20;
            if(pMSHTParam->uiMsht < pMSHTParam->uiMshtMin)
            {
                //pMSHTParam->uiMsht = Default_Mshutter;
                break;
            }
        }
    }

    Cal_Module_SetState(CAL_STATE_IDLE);
}

void Cal_ColorShading(unsigned long RawAdd, unsigned long Width,unsigned long Height,unsigned char StartPixel, UINT32 TblAdd, UINT32 Ratio)
{
    UINT16 i,j;
    UINT32 Hori_Step,Vert_Step,Max_R=0,Max_G=0,Max_B=0,Data_R,Data_G,Data_B;
    UINT32 StartH,StartV,SizeH,SizeV,MaxTbl_H,MaxTbl_V;
    UINT32 tempR,tempG,tempB;
    UINT32 MaxValue;
    //unsigned long DataEnd;
    //unsigned char endl[2]={0X0D,0X0A};
    //unsigned char *buff;
    //DataEnd = RawAdd+Width*Height;
    if(Ratio > 100)
        Ratio = 100;

    MaxValue = 0x3ff;// limited by NT96220

    MaxTbl_H = 65;
    MaxTbl_V = 65;

    Hori_Step = Width/(MaxTbl_H);
    Vert_Step = Height/(MaxTbl_V);

    if(Hori_Step%2)
        Hori_Step--;
    if(Vert_Step%2)
        Vert_Step--;

    for(j=0;j<(MaxTbl_V);j++)
    {
        if(j==0)
        {
            SizeV = Vert_Step/2;
            StartV = 0;
        }
        else if(j==(MaxTbl_V-1))
        {
            SizeV = Vert_Step/2;
            StartV = Height-SizeV;
        }
        else
        {
            SizeV = Vert_Step;
            StartV = (Height*j/(MaxTbl_V-1))-(SizeV/2);//Vert_Step*j;//-offsetV;
        }
        if(StartV%2)
            StartV--;

        for(i=0;i<(MaxTbl_H);i++)
        {
            if(i==0)
            {
                SizeH = Hori_Step/2;
                StartH = 0;
            }
            else if(i == (MaxTbl_H-1))
            {
                SizeH = Hori_Step/2;
                StartH = Width-SizeH;
            }
            else
            {
                SizeH = Hori_Step;
                StartH = (Width*i/(MaxTbl_H-1))-(SizeH/2);//Hori_Step*i;//-offsetH;
            }

            if(StartH%2)
                StartH--;

            CalculateRawY(RawAdd, StartH, StartV, SizeH, SizeV, Width,1, StartPixel, &Data_R,&Data_G,&Data_B);

            if(Data_R>Max_R)
                Max_R = Data_R;
            if(Data_G>Max_G)
                Max_G = Data_G;
            if(Data_B>Max_B)
                Max_B = Data_B;
        }
    }
    for(j=0;j<(MaxTbl_V);j++)
    {
        if(j==0)
        {
            SizeV = Vert_Step/2;
            StartV = 0;
        }
        else if(j==(MaxTbl_V-1))
        {
            SizeV = Vert_Step/2;
            StartV = Height-SizeV;
        }
        else
        {
            SizeV = Vert_Step;
            StartV = (Height*j/(MaxTbl_V-1))-(SizeV/2);//Vert_Step*j;//-offsetV;
        }
        if(StartV%2)
            StartV--;

        for(i=0;i<(MaxTbl_H);i++)
        {
            if(i==0)
            {
                SizeH = Hori_Step/2;
                StartH = 0;
            }
            else if(i == (MaxTbl_H-1))
            {
                SizeH = Hori_Step/2;
                StartH = Width-SizeH;
            }
            else
            {
                SizeH = Hori_Step;
                StartH = (Width*i/(MaxTbl_H-1))-(SizeH/2);//Hori_Step*i;//-offsetH;
            }
            if(StartH%2)
                StartH--;
            CalculateRawY(RawAdd, StartH, StartV, SizeH, SizeV, Width,1, StartPixel, &Data_R,&Data_G,&Data_B);

            if(Data_R == 0)
                Data_R = 1;
            if(Data_G == 0)
                Data_G =1;
            if(Data_B == 0)
                Data_B = 1;

            //Ratio = 100
            //tempR = (Max_R<<8)/Data_R;
            //tempG = (Max_G<<8)/Data_G;
            //tempB = (Max_B<<8)/Data_B;

            //tempR = (((Max_R-Data_R)*Ratio/100+Data_R)<<8)/Data_R;
            //tempG = (((Max_G-Data_G)*Ratio/100+Data_G)<<8)/Data_G;
            //tempB = (((Max_B-Data_B)*Ratio/100+Data_B)<<8)/Data_B;
            // equal ==>
            tempR = ((Max_R*Ratio+(100-Ratio)*Data_R)<<8)/(Data_R*100);
            tempG = ((Max_G*Ratio+(100-Ratio)*Data_G)<<8)/(Data_G*100);
            tempB = ((Max_B*Ratio+(100-Ratio)*Data_B)<<8)/(Data_B*100);

            if(tempR>=MaxValue)
                tempR = MaxValue;

            if(tempG>=MaxValue)
                tempG = MaxValue;

            if(tempB>=MaxValue)
                tempB = MaxValue;

            *(UINT32 *)TblAdd = ((tempR << 20) | (tempG<<10) | tempB);
            TblAdd += 4;
        }
    }
    //SaveGainTable((unsigned long)&ColorShadingTbl[0][0][0],ColorShadingTbl_H*ColorShadingTbl_V*3,byte);

#if 0 // for debug
    buff = (UB*)DataEnd;
    buff += sprintf((char *)buff,"%s",endl);

    buff += sprintf((char *)buff,"MaxR=%d MaxG=%d MaxB=%d%s%s",Max_R,Max_G,Max_B,endl,endl);

/*
    buff += sprintf((char *)buff,"R data: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%d\t",Color_Tbl[0][j][i]);
        }
        buff += sprintf((char *)buff,"%s", endl);
    }
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"G data: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%d\t",Color_Tbl[1][j][i]);
        }
        buff += sprintf((char *)buff,"%s", endl);
    }
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"B data: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%d\t",Color_Tbl[2][j][i]);
        }
        buff += sprintf((char *)buff,"%s", endl);
    }
    buff += sprintf((char *)buff,"%s", endl);

    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                (UB*)DataEnd,// UB *pBuf,
                (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                0,//UW Pos,
                1,//BOOL CloseFile,
                FST_FMT_RAW,//UW FileFormat,
                FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
    buff = (UB*)DataEnd;
*/

    //buff += sprintf((char *)buff,"R gain: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%x",ColorShadingTbl[0][j][i]);
            buff += sprintf((char *)buff,"%s", endl);
        }
    }
    //buff += sprintf((char *)buff,"%s", endl);
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                (UB*)DataEnd,// UB *pBuf,
                (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                0,//UW Pos,
                1,//BOOL CloseFile,
                FST_FMT_RAW,//UW FileFormat,
                FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
    buff = (UB*)DataEnd;

    //buff += sprintf((char *)buff,"G gain: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%x",ColorShadingTbl[1][j][i]);
            buff += sprintf((char *)buff,"%s", endl);
        }
    }
    //buff += sprintf((char *)buff,"%s", endl);
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                (UB*)DataEnd,// UB *pBuf,
                (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                0,//UW Pos,
                1,//BOOL CloseFile,
                FST_FMT_RAW,//UW FileFormat,
                FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
    buff = (UB*)DataEnd;

    //buff += sprintf((char *)buff,"B gain: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%x",ColorShadingTbl[2][j][i]);
            buff += sprintf((char *)buff,"%s", endl);
        }
    }

    //buff += sprintf((char *)buff,"%s", endl);
    //buff += sprintf((char *)buff,"%s", endl);

    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)DataEnd,// UB *pBuf,
                    (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
#endif

}

void Cal_ECSAdjust(CAL_ECS_PARAM* pECSParam)
{
    CAL_RAW_INFO RawInfo = {0};
    SENSOR_STATUS sen_status;
    UINT32 RawAddr = gCalCtrlObj.uiMemoryAddr;

    if (Cal_Module_GetState() != CAL_STATE_IDLE)
    {
        debug_err(("Cal_ECSAdjust Fail!\r\n"));
        return;
    }

    Cal_Module_SetState(CAL_STATE_ECS);
    gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_CAP, pECSParam->uiCalibMode);

    //ethan_20110729_inorder to get right cur sensor mode ob
    Sensor_GetStatus(&sen_status);
    pECSParam->uiOBofs = sen_status.pSyncInfo->OB_Offset;

    Sensor_SetExpTime(pECSParam->uiExpT);
    sie_waitVD(1);
    Sensor_SetISO(pECSParam->uiISO);
    sie_waitVD(3);

    gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_STILL, &RawInfo);

    gCalCtrlObj.pCalApi->fpGetCapRAW(RawAddr,
                                    RawInfo.uiTotalWidth,
                                    RawInfo.uiTotalHeight,
                                    pECSParam->uiOBofs,
                                    PATH_PRE_COLORGAIN,
                                    pECSParam->uiMsht);

    if (pECSParam->bSaveRaw == TRUE)
    {
        gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight);
    }

    Cal_ColorShading(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight, RawInfo.uiStartPixel, pECSParam->uiTableAddr, pECSParam->uiCmpRatio);

    Cal_Module_SetState(CAL_STATE_IDLE);
}

// add OB calibration *common part* [NTK#RandyHo]<2012/04/28>
void Cal_OBAdjust(CAL_OB_PARAM* pOBParam)
{
    CAL_RAW_INFO RawInfo = {0};
    SENSOR_STATUS sen_status;
    UINT32 RawAddr = gCalCtrlObj.uiMemoryAddr;

    if (Cal_Module_GetState() != CAL_STATE_IDLE)
    {
        debug_err(("Cal_OBAdjust Fail!\r\n"));

        return;
    }

    Cal_Module_SetState(CAL_STATE_OB);
    gCalCtrlObj.pCalApi->fpSetMode(IPL_MODE_CAP, pOBParam->uiCalibMode);

    //ethan_20110729_inorder to get right cur sensor mode ob
    Sensor_GetStatus(&sen_status);

    Sensor_SetExpTime(pOBParam->uiExpT);
    sie_waitVD(1);
    Sensor_SetISO(pOBParam->uiISO);
    sie_waitVD(3);

    gCalCtrlObj.pCalApi->fpGetRAWSize(RAW_TYPE_STILL, &RawInfo);

    gCalCtrlObj.pCalApi->fpGetCapRAW( RawAddr,
                                    RawInfo.uiTotalWidth,
                                    RawInfo.uiTotalHeight,
                                    0,
                                    PATH_PRE_COLORGAIN,
                                    pOBParam->uiMsht);

    if (pOBParam->bSaveRaw == TRUE)
    {
        gCalCtrlObj.pCalApi->fpSaveRAW(RawAddr, RawInfo.uiTotalWidth, RawInfo.uiTotalHeight);
    }

    pOBParam->Y =CalculateRawY(RawAddr, 0, 0, (RawInfo.uiTotalWidth*16)/16, (RawInfo.uiTotalHeight*16)/16, (RawInfo.uiTotalWidth*16)/16, 1, RawInfo.uiStartPixel, (UINT32 *)&pOBParam->Y_R,  (UINT32 *)&pOBParam->Y_G,  (UINT32 *)&pOBParam->Y_B);

    // results in shift right 4 bits(>>4) corresponding to "mean brightness value" in photoshop
    pOBParam->Y >>= 4;
    pOBParam->Y_R >>= 4;
    pOBParam->Y_G >>= 4;
    pOBParam->Y_B >>= 4;

    Cal_Module_SetState(CAL_STATE_IDLE);

    return;
}

void Cal_SetCalData(UINT CalItem, UINT  *pCalData)
{
    int i;

    switch(CalItem)
    {
        case CAL_DATANUM_STATUS_AWB:
            gCalStructData.CalStatus_AWB=pCalData[0];
            break;

        //ethan_20110803_add
        case CAL_DATANUM_STATIS_AWBGS:
            gCalStructData.CalStatus_AWBGS=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_PrvDP_W:
            gCalStructData.CalStatus_PrvDP_W=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_PrvDP_B:
            gCalStructData.CalStatus_PrvDP_B=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_CapDP_W:
            gCalStructData.CalStatus_CapDP_W=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_CapDP_B:
            gCalStructData.CalStatus_CapDP_B=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_ISO:
            gCalStructData.CalStatus_ISO=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_Mshutter:
            gCalStructData.CalStatus_Mshutter=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_LensShading:
            gCalStructData.CalStatus_LensShading=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_ColorShading:
            gCalStructData.CalStatus_ColorShading=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_FlashTriggerPointer:
            gCalStructData.CalStatus_FlashTriggerPointer=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_FlashShading:
            gCalStructData.CalStatus_FlashShading=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_FlashFire:
            gCalStructData.CalStatus_FlashFire=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_FlashAWB:
            gCalStructData.CalStatus_FlashAWB=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_FlashADC:
            gCalStructData.CalStatus_FlashADC=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_PrvDL:
            gCalStructData.CalStatus_DL_Prv = pCalData[0];
            break;

        case CAL_DATANUM_STATUS_CapDL:
            gCalStructData.CalStatus_DL_Cap = pCalData[0];
            break;

        case CAL_DATANUM_STATUS_ECS:
            gCalStructData.CalStatus_ECS = pCalData[0];
            break;

        case CAL_DATANUM_AWB_Statistic:
            gCalStructData.Cal_AWBStatistic.R=pCalData[0];
            gCalStructData.Cal_AWBStatistic.G=pCalData[1];
            gCalStructData.Cal_AWBStatistic.B=pCalData[2];
            break;

        //ethan_20110801_addawbgain
        case CAL_DATANUM_AWB_GAIN:
            gCalStructData.Cal_AWBGain.Rgain=pCalData[0];
            gCalStructData.Cal_AWBGain.Ggain=256;
            gCalStructData.Cal_AWBGain.Bgain=pCalData[1];
            break;

        //ethan_20110801_addawbgsgain
        case CAL_DATANUM_AWB_GAIN_GS:
            gCalStructData.Cal_AWBGSGain.Rgain=pCalData[0];
            gCalStructData.Cal_AWBGSGain.Ggain=256;
            gCalStructData.Cal_AWBGSGain.Bgain=pCalData[1];
            break;

        case CAL_DATANUM_DP_PreviewAddr:
            gCalStructData.Cal_DP_PreviewAddr=pCalData[0];
            break;

        case CAL_DATANUM_DP_VideoAddr:
            gCalStructData.Cal_DP_VideoAddr=pCalData[0];
            break;            

        case CAL_DATANUM_DP_CaptureAddr:
            gCalStructData.Cal_DP_CaptureAddr=pCalData[0];
            break;

        case CAL_DATANUM_Mshutter:
            gCalStructData.Cal_Mshutterr_Data[0]=pCalData[0];
            gCalStructData.Cal_Mshutterr_Data[1]=pCalData[1];
            gCalStructData.Cal_Mshutterr_Data[2]=pCalData[2];
            break;

        case CAL_DATANUM_ISO:
            gCalStructData.Cal_ISO_Data[0]=pCalData[0];
            gCalStructData.Cal_ISO_Data[1]=pCalData[1];
            gCalStructData.Cal_ISO_Data[2]=pCalData[2];
            break;

        case CAL_DATANUM_Flash_TriggerPoint:
            gCalStructData.Cal_Flash.FlashTriggerPoint=pCalData[0];
            break;

        case CAL_DATANUM_Flash_Fire:
            gCalStructData.Cal_Flash.FlashFire=pCalData[0];
            break;

        case CAL_DATANUM_Flash_AWB:
            gCalStructData.Cal_Flash.FlashR=pCalData[0];
            gCalStructData.Cal_Flash.FlashG=pCalData[1];
            gCalStructData.Cal_Flash.FlashB=pCalData[2];
            break;

        case CAL_DATANUM_DL_PrvPos:
            for(i = 0; i < 16; i++)
                gCalStructData.Cal_DL_PreviewPos[i] = pCalData[i];
            break;

        case CAL_DATANUM_DL_CapPos:
            for(i = 0; i < 16; i++)
                gCalStructData.Cal_DL_CapturePos[i] = pCalData[i];
            break;

        case CAL_DATANUM_ECS_Addr:
            gCalStructData.Cal_ECS_Addr=pCalData[0];
            break;

        case CAL_DATANUM_STATUS_OB: // [NTK#RandyHo]<2012/04/28>
            gCalStructData.CalStatus_OB = pCalData[0];
            break;

        case CAL_DATANUM_OB_OFFSET: // [NTK#RandyHo]<2012/04/28>
            gCalStructData.OBoffset = pCalData[0];
            break;
    }

//    return E_OK;
}

void Cal_GetCalData(CAL_DATA* pCalInfo)
{
    *pCalInfo = gCalStructData;
}

//@}
