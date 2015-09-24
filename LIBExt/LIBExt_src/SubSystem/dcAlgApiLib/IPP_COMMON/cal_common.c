#include <stdio.h>
#include <string.h>
#include "FilesysTsk.h"
#include "IPPCtrl.h"
#include "Alg_IPLMode.h"
#include "Calibration.h"
#include "Lens.h"
#include "Sensor.h"
#include "sen_common.h"
#include "sen_isr.h"
#include "debug.h"
#include "ErrorNo.h"
#include "dzoom.h"
#include "SensorSys.h"
#include "aaa_lib.h"
#include "fd_lib.h"
#include "sd_lib.h"
#include "SensorFlowFunc.h"

const UINT8 CAL_AE_Win_Spot[8][8] =
{
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

void CalSetMode(UINT32 mode,UINT32 uiSenMode);
void CalLock3A(UINT32 isLock);
void CalGetRawSize(UINT32 Type, CAL_RAW_INFO* RawInfo);
void CalGetPrvAddr(UINT32* Addr);
void CalPreviewOne(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath);
void CalCaptureOne(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath, UINT32 MshtTime);
void CalSaveRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV);
void CalGetStatistics(UINT32 CAType, UINT32 Addr, CAL_RAW_INFO RawInfo, UINT16 *pStatistics);
UINT32 CalAEProcess(CAL_AE_INFO *AeInfo);

CALIBRATION_API g_CalApi =
{
    CalSetMode,
    CalLock3A,

    CalGetRawSize,
    CalGetPrvAddr,
    CalPreviewOne,
    CalCaptureOne,
    CalSaveRAW,
    CalGetStatistics,
    CalAEProcess
};

PCALIBRATION_API Cal_GetCalApi(void)
{
    return &g_CalApi;
}
/**
  Change Ipp mode and sensor mode in calibration flow
*/
void CalSetMode(UINT32 mode,UINT32 uiSenMode)
{
    ISR_CB Isr_cb = {NULL};
    IPPCTRL_OBJ ippctrl_obj = {0};
    PImageAlgInfo pimginfo;
    static IPL_MODE cur_mode;
    static BOOL bInit = FALSE;

    if(bInit!= TRUE)
    {
        cur_mode = IPL_GetMode();
        bInit = TRUE;
    }
    //debug_msg("^G %s: %d %d\r\n",__func__,cur_mode,mode);
    if(cur_mode == mode)
    {
        return;
    }

    pimginfo = UI_GetParameter();

    switch(mode)
    {
        case IPL_MODE_PREVIEW:
            //config sensor
            //Sensor_WakeUp(0);
            Sensor_ChgMode(uiSenMode);
            //hook ipp isrcb func
            IsrCBReg(&Isr_cb);
            //config Image pipeline
            if(cur_mode!= IPL_MODE_CAP)
            {
                ippctrl_obj.opmode = IPP_OPEN;
                IPPCTRL_SetCmd(ippctrl_obj);
            }
            ippctrl_obj.fpIppFlowApi = Set2Preview;
            ippctrl_obj.fpStrongFlow = NULL;
            ippctrl_obj.IPPctrl_info.uiDzoomStep = 10;//dzoom default min idx
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = DispSizeInfo.uiDisplayFBWidth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = DispSizeInfo.uiDisplayFBHeight;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = DispSizeInfo.uiDisplayFBWidth;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = DispSizeInfo.uiDisplayFBWidth>>1;
            ippctrl_obj.IPPctrl_info.uiImgFmt = IMEOUT2_FMT422_2;
            ippctrl_obj.IPPctrl_info.ImePath2Enable = DISABLE;
            ippctrl_obj.opmode = IPP_START;
            IPPCTRL_SetCmd(ippctrl_obj);
            break;
        case IPL_MODE_OFF:
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.opmode = IPP_CLOSE;
            IPPCTRL_SetCmd(ippctrl_obj);

            //config sensor into sleep/pause mode
            //Sensor_Sleep(0);
            break;
        case IPL_MODE_CAP:
            switch(cur_mode)
            {
                case IPL_MODE_OFF:
                    ippctrl_obj.opmode = IPP_OPEN;
                    IPPCTRL_SetCmd(ippctrl_obj);
                    break;
                case IPL_MODE_PREVIEW:
                    ippctrl_obj.opmode = IPP_STOP;
                    IPPCTRL_SetCmd(ippctrl_obj);
                    break;
                default:
                    break;
            }
            sie_waitVD(1);//for signal sync???,according to HDCF KS's test
            Sensor_ChgMode(uiSenMode);
            sie_waitVD(1);
            break;
        default:
            debug_err(("%s:only support preview/off\r\n",__func__));
            break;
    }
    cur_mode = mode;
}

void CalLock3A(UINT32 isLock)
{
    // Lock 3A, FD and SD
    aaa_AeLock(isLock);
    aaa_AwbLock(isLock);
    fd_Lock(isLock);
    sd_Lock(isLock);
}

void CalGetRawSize(UINT32 Type, CAL_RAW_INFO* RawInfo)
{
    #define NUM2EVEN(num)   (((num) % 2 == 1) ? ((num) - 1): (num))

    SENSOR_STATUS SensorStatus;
    UINT32 maxidx=0;
    UINT32 dzoomtbl[8];
    Sensor_GetStatus(&SensorStatus);

    if (Type == RAW_TYPE_PREVIEW)
    {
        // Preview RAW info
        //RawInfo->Addr = ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1);
        RawInfo->uiTotalWidth = GetIPPSizeInfor(_SieOutLineOffset);
        RawInfo->uiTotalHeight = GetIPPSizeInfor(_SieOutVsize);
        //25% size of orignal img,incase of shading
        RawInfo->uiStartX = NUM2EVEN(RawInfo->uiTotalWidth >> 2);
        RawInfo->uiStartY = NUM2EVEN(RawInfo->uiTotalHeight >> 2);
        RawInfo->uiSizeX = NUM2EVEN(RawInfo->uiTotalWidth >> 1);
        RawInfo->uiSizeY = NUM2EVEN(RawInfo->uiTotalHeight >> 1);
        RawInfo->uiSample = 1;
        RawInfo->uiStartPixel = SensorStatus.uiBayer;
    }
    else
    {
        //set dzoom tbl according to sensor cap mode
        //sensor must be chged to cap mode first
        Set_DZoomInfor(SenMode2Tbl(SensorStatus.uiSensorMode,&maxidx),_Ratio_4_3,_Ratio_4_3,_Ratio_4_3, _WindowFitAuto);
        //get dzoom max crop size
        Get_DZoomMaxInfor(dzoomtbl);
        // Capture RAW info
        //RawInfo->Addr = Addr;
        RawInfo->uiTotalWidth = dzoomtbl[2];
        RawInfo->uiTotalHeight = dzoomtbl[3];
        //25% size of orignal img,incase of shading
        RawInfo->uiStartX = NUM2EVEN(RawInfo->uiTotalWidth >> 2);
        RawInfo->uiStartY = NUM2EVEN(RawInfo->uiTotalHeight >> 2);
        RawInfo->uiSizeX = NUM2EVEN(RawInfo->uiTotalWidth >> 1);
        RawInfo->uiSizeY = NUM2EVEN(RawInfo->uiTotalHeight >> 1);
        RawInfo->uiSample = 1;
        RawInfo->uiStartPixel = SensorStatus.uiBayer;
    }
}

void CalGetPrvAddr(UINT32* Addr)
{
    *Addr = ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1);
}

void CalPreviewOne(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath)
{
    SIE_OPENOBJ SieObjCB;
    UINT32 SieFuncEn = DISABLE;
    SetIPPSizeInfor(_SieCropHsize, RawSizeH);
    SetIPPSizeInfor(_SieCropVsize, RawSizeV);
    SetIPPSizeInfor(_SieOutLineOffset, (RawSizeH * gImageAlgInfo.SieCapBitDepth / 8));

    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.SieObOfs = ObOfs;
    SIE_PRE_Para_prv.SieOutAdd0 = RawAddr;
    SIE_PRE_Para_prv.SiePathSel = SiePath;

    if(SiePath == PATH_PRE_GAMMA)
        SieFuncEn |= SIE_CG_EN;
    if(SiePath == PATH_POST_GAMMA)
        SieFuncEn |= SIE_GAMMA_EN;
    SIE_PRE_Para_prv.SieFuncEn = SieFuncEn;

    SIE_PRE_Para_prv.SieCropSizeH = GetIPPSizeInfor(_SieCropHsize);
    SIE_PRE_Para_prv.SieCropSizeV = GetIPPSizeInfor(_SieCropVsize);
    SIE_PRE_Para_prv.SieOutSizeH = GetIPPSizeInfor(_SieCropHsize);
    SIE_PRE_Para_prv.SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);

    if (gImageAlgInfo.SieBitDepth == _SieBitDepth_8)
    {
        SIE_PRE_Para_prv.SieBitDepth = SIE_OUT_PACK_8BIT;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_10)
    {
        SIE_PRE_Para_prv.SieBitDepth = SIE_OUT_PACK_10BIT;
    }
    else
    {
        debug_err(("%s:ERROR!!\r\n",__func__));
    }

    SieObjCB.FP_SIEISR_CB = NULL;
    sie_open(&SieObjCB);
    IPPMgrReq.SIE_PRE_PRV = &SIE_PRE_Para_prv;
    if (Sensor_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    if ( SIE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));

    sie_waitVD(1);
    sie_start();
//    sie_waitVD(1);
    sie_waitVD(2); // wait SIE output buffer update
    sie_pause();
    sie_close();
}

void CalCaptureOne(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath, UINT32 MshtTime)
{
    UINT32 Line,Pixel2;
    STR_SIE_MSHTR_PARAM _mshut_param;
    SENSOR_STATUS SensorStatus;
    UINT32 SieFuncEn = DISABLE;

    CAL_DATA CalInfo;
    Cal_GetCalData(&CalInfo);

    if(gImageAlgInfo.Exposuremode_UI == _ERS){

    Sensor_GetStatus(&SensorStatus);

    SetSensorMode(SensorStatus.uiSensorMode);

    SetIPPSizeInfor(_SieCropCapHsize, RawSizeH);
    SetIPPSizeInfor(_SieCropCapVsize, RawSizeV);
    SetIPPSizeInfor(_SieOutLineOffset, (RawSizeH * gImageAlgInfo.SieCapBitDepth / 8));

    SetSiePreCapParam(&SIE_PRE_Para_cap);

    SIE_PRE_Para_cap.SieObOfs = ObOfs;
    SIE_PRE_Para_cap.SiePathSel = SiePath;

    SIE_PRE_Para_cap.MShutterTime = MshtTime;

    if(SiePath == PATH_PRE_GAMMA)
        SieFuncEn |= SIE_CG_EN;
    if(SiePath == PATH_POST_GAMMA)
        SieFuncEn |= SIE_GAMMA_EN;

    if(CalInfo.CalStatus_ECS == _ECS_Status)
    {
        SieFuncEn |= SIE_ECS_EN;
        ippbuf_SetBuf(IPPBUF_ECS, CalInfo.Cal_ECS_Addr);
    }
    
    SIE_PRE_Para_cap.SieFuncEn = SieFuncEn;

    SIE_PRE_Para_cap.SieOutAdd0 = RawAddr;

    IPPMgrReq.SIE_PRE_CAP = &SIE_PRE_Para_cap;
    if ( Sensor_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    SenFlowFunc(SIE_CAP_SETTING);
    
    if ( SIE_AllocatResourceIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_AllocatResourceIPP() error \n"));

    if(gImageAlgInfo.Exposuremode_UI == _GRS)
    {
        Time2_Line_Pixel2(SIE_PRE_Para_cap.MShutterTime, SensorStatus.uiPclk, GetIPPSizeInfor(_HDTotal), &Line, &Pixel2);
        _mshut_param.MshMode = MSHUTTER_PULSE_MODE;
        _mshut_param.FrameDelay = 0;
        _mshut_param.LineDelay = Line;
        _mshut_param.PX2Delay = Pixel2;
        _mshut_param.MshActSel = MSHUTTER_HIGH_ACTIVE;
        _mshut_param.MshWidth = 20;
        _mshut_param.PinSwap = FALSE;
        _mshut_param.CloseField = 0;
        sie_setMshtrParam(_mshut_param);
        sie_setMshtrStart();
    }

    if ( SIE_ExecIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));

    sie_start();
    sie_waitVD(1);
    sie_waitBP1(FALSE);
    sie_pause();
    sie_waitVD(1);

    }
    else{//only for MI9130 Sensor + Mshutter

        Sensor_GetStatus(&SensorStatus);

        SetSensorMode(SensorStatus.uiSensorMode);

        SetIPPSizeInfor(_SieCropCapHsize, RawSizeH);
        SetIPPSizeInfor(_SieCropCapVsize, RawSizeV);
        SetIPPSizeInfor(_SieOutLineOffset, (RawSizeH * gImageAlgInfo.SieCapBitDepth / 8));

        SetSiePreCapParam(&SIE_PRE_Para_cap);

        SIE_PRE_Para_cap.SieObOfs = ObOfs;
        SIE_PRE_Para_cap.SiePathSel = SiePath;

        SIE_PRE_Para_cap.MShutterTime = MshtTime;

        if(SiePath == PATH_PRE_GAMMA)
            SieFuncEn |= SIE_CG_EN;
        if(SiePath == PATH_POST_GAMMA)
            SieFuncEn |= SIE_GAMMA_EN;
        
        if(CalInfo.CalStatus_ECS == _ECS_Status)
        {
            SieFuncEn |= SIE_ECS_EN;
            ippbuf_SetBuf(IPPBUF_ECS, CalInfo.Cal_ECS_Addr);
        }
        
        SIE_PRE_Para_cap.SieFuncEn = SieFuncEn;

        SIE_PRE_Para_cap.SieOutAdd0 = RawAddr;

        IPPMgrReq.SIE_PRE_CAP = &SIE_PRE_Para_cap;
        if ( Sensor_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
            debug_err(("Sensor_BeginIPP() error \n"));

        if ( SIE_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
            debug_err(("SIE_BeginIPP() error \n"));

        if ( SIE_AllocatResourceIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
            debug_err(("SIE_AllocatResourceIPP() error \n"));

        SenFlowFunc(SIE_CAP_SETTING);

        Sensor_TriggerGlobalReset();
        if(Sensor_checkExposureLongTime()==TRUE)
        {
            //TimerDelayUs(pDPCParam->uiTime);
        }

        if ( SIE_ExecIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
            debug_err(("SIE_ExecIPP() error \n"));

        if(Sensor_checkExposureLongTime()==TRUE)
        {
            Lens_Shutter_Move(LENS_SHUTTER_OPEN,TRUE);
        }

        sie_start();
        sie_waitVD(1);
        Sensor_ClearGlobalReset();
        sie_waitBP1(FALSE);
        sie_pause();
        sie_waitVD(1);
    }
}

/**
  save raw data from dram
*/
void CalSaveRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV)
{
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)RawAddr,
                        (RawSizeH * RawSizeV),
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);
    debug_msg(">>Cal_SaveRAWsize: %d x %d(0x%x)\r\n",RawSizeH,RawSizeV,RawAddr);
}

void CalGetStatistics(UINT32 CAType, UINT32 Addr, CAL_RAW_INFO RawInfo, UINT16 *pStatistics)
{
    UINT SumR = 0, SumGr = 0, SumGb = 0, SumB = 0,SumTotal=0;
    UINT AvgR,AvgG,AvgB,Data[4];
    UINT i,j,count=0;
    unsigned char Pixel_R,Pixel_Gr,Pixel_Gb,Pixel_B;
    UINT32 r[64]={0},g[64]={0},b[64]={0};
    UINT32 tblsize = 64;

    if (CAType == CAL_CASOURCE_SUBOUT)
    {
        GetSubOutRGBinfo(r,g,b,tblsize);

        if(gImageAlgInfo.AEMode==_IMGAEMode_Spot)
        {
            //calculate ae weight
            SumR = 0;
            SumGr = 0;
            SumB = 0;
            SumTotal = 0;
            for (j = 0; j < 8; j ++)
            {
                for (i = 0; i < 8; i ++)
                {
                    SumR += (CAL_AE_Win_Spot[j][i] * r[j * 8 + i]);
                    SumGr += (CAL_AE_Win_Spot[j][i] * g[j * 8 + i]);
                    SumB += (CAL_AE_Win_Spot[j][i] * b[j * 8 + i]);
                    SumTotal += CAL_AE_Win_Spot[j][i];
                }
            }

            AvgR = SumR / SumTotal;
            AvgB = SumB / SumTotal;
            AvgG = SumGr / SumTotal;
        }
        else{
        for(i=0;i<tblsize;i++)
        {
            SumR += r[i];
            SumGr += g[i];
            SumB += b[i];
        }
        AvgR = SumR / tblsize;
        AvgB = SumB / tblsize;
        AvgG = SumGr / tblsize;
        }
        pStatistics[1] = AvgR;
        pStatistics[2] = AvgG;
        pStatistics[3] = AvgB;

        pStatistics[0] = (77*AvgR+150*AvgG+29*AvgB)>>8;
    }
    else
    {
        Data[0]=0;
        Data[1]=0;
        Data[2]=0;
        Data[3]=0;
        if(RawInfo.uiStartPixel == 0)// R
        {
            Pixel_R = 0;
            Pixel_Gr = 1;
            Pixel_Gb = 2;
            Pixel_B = 3;
        }
        else if (RawInfo.uiStartPixel == 1) // Gr
        {
            Pixel_Gr = 0;
            Pixel_R = 1;
            Pixel_B = 2;
            Pixel_Gb = 3;
        }
        else if (RawInfo.uiStartPixel == 2) // Gb
        {
            Pixel_Gb = 0;
            Pixel_B = 1;
            Pixel_R = 2;
            Pixel_Gr = 3;
        }
        else if (RawInfo.uiStartPixel == 3) // B
        {
            Pixel_B = 0;
            Pixel_Gb = 1;
            Pixel_Gr = 2;
            Pixel_R = 3;
        }
        else// default = R
        {
            Pixel_R = 0;
            Pixel_Gr = 1;
            Pixel_Gb = 2;
            Pixel_B = 3;
        }
        for(j=RawInfo.uiStartY;j<(RawInfo.uiStartY+RawInfo.uiSizeY);j+=(RawInfo.uiSample*2))
        {
            for(i=RawInfo.uiStartX;i<(RawInfo.uiStartX+RawInfo.uiSizeX);i+=(RawInfo.uiSample*2))
            {
                Data[Pixel_Gb]+=*((UB*)(Addr+i+j*RawInfo.uiTotalWidth));
                Data[Pixel_B]+=*((UB*)(Addr+i+1+j*RawInfo.uiTotalWidth));
                Data[Pixel_R]+=*((UB*)(Addr+i+(j+1)*RawInfo.uiTotalWidth));
                Data[Pixel_Gr]+=*((UB*)(Addr+i+1+(j+1)*RawInfo.uiTotalWidth));
                count++;
            }
        }
        if((RawInfo.uiStartX+RawInfo.uiStartY)%2==0)
        {
            if(RawInfo.uiStartX%2==0)
            {
                SumGb=Data[0];
                SumB=Data[1];
                SumR=Data[2];
                SumGr=Data[3];
            }
            else
            {
                SumGr=Data[0];
                SumR=Data[1];
                SumB=Data[2];
                SumGb=Data[3];
            }
        }
        else
        {
            if(RawInfo.uiStartX%2==0)
            {
                SumR=Data[0];
                SumGr=Data[1];
                SumGb=Data[2];
                SumB=Data[3];
            }
            else
            {
                SumB=Data[0];
                SumGb=Data[1];
                SumGr=Data[2];
                SumR=Data[3];
            }
        }
        AvgR=SumR/count;
        AvgB=SumB/count;
        AvgG=(SumGr+SumGb)/(count*2);
        pStatistics[1] = AvgR;
        pStatistics[2] = AvgG;
        pStatistics[3] = AvgB;

        pStatistics[0] = (77*AvgR+150*AvgG+29*AvgB)>>8;
    }
}

UINT32 CalAEProcess(CAL_AE_INFO *AeInfo)
{
    static UINT32 stableCnt;
    UINT32 time, gain;
    static UINT8 count;

    if (AeInfo->uiAEType == AE_FIX_GAIN) /*fix gain*/
    {
        // backup old value
        time = AeInfo->uiExpT;

        //1/4EV convergence
        if(((AeInfo->uiLum >= AeInfo->uiTarget*109/100) ||\
            (AeInfo->uiLum <= AeInfo->uiTarget*100/109)))
        {
            AeInfo->uiExpT = AeInfo->uiExpT * AeInfo->uiTarget / AeInfo->uiLum;
        }
        else if(AeInfo->uiLum >= AeInfo->uiTarget*105/100)
        {
            AeInfo->uiExpT -= AeInfo->uiAdjStep;
        }
        else if(AeInfo->uiLum <= AeInfo->uiTarget*100/105)
        {
            AeInfo->uiExpT += AeInfo->uiAdjStep;
        }

        AeInfo->uiResult = AeInfo->uiExpT;

        // check AE stable
        count = (time == AeInfo->uiExpT)? (count+1): 0;

        // check timeout count
        if (count > AeInfo->uiTimeout)
        {
            debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
            return CAL_AE_TIMEOUT;
        }
        // check ae stable
        if ((AeInfo->uiLum < AeInfo->uiTarget*105/100) && (AeInfo->uiLum > AeInfo->uiTarget*100/105))
        {
            stableCnt++;

            if (stableCnt > 3)
            {
                stableCnt = 0;
                return CAL_AE_STABLE;
            }
        }
    }
    else if (AeInfo->uiAEType == AE_FIX_TIME)
    {
        gain = AeInfo->uiISO;

        //1/4EV convergence
        if(((AeInfo->uiLum >= AeInfo->uiTarget*109/100) ||\
            (AeInfo->uiLum <= AeInfo->uiTarget*100/109)))
        {
            AeInfo->uiISO = AeInfo->uiISO * AeInfo->uiTarget / AeInfo->uiLum;
        }
        else if(AeInfo->uiLum >= AeInfo->uiTarget*105/100)
        {
            AeInfo->uiISO -= AeInfo->uiAdjStep;
        }
        else if(AeInfo->uiLum <= AeInfo->uiTarget*100/105)
        {
            AeInfo->uiISO += AeInfo->uiAdjStep;
        }
        else
        {
            stableCnt++;
            if (stableCnt > 3)
            {
                stableCnt = 0;
                return CAL_AE_STABLE;
            }
        }

        AeInfo->uiResult = AeInfo->uiISO;

        count = (gain == AeInfo->uiISO)? (count+1): 0;

        // check timeout count
        if (count > AeInfo->uiTimeout)
        {
            debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
            return CAL_AE_TIMEOUT;
        }
    }
    else if (AeInfo->uiAEType == AE_FIX_TIMEnGAIN)
    {
        return CAL_AE_STABLE;
    }
    else
    {
        AeInfo->uiResult = 0;
        debug_msg("CAL_AEProcess: Unknown type!!\r\n");
    }

    return CAL_AE_RUNNING;
}
// EOF
