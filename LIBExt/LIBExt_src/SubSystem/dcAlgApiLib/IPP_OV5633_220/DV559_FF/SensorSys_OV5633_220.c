#include "filesystsk.h"
#include "Exif.h"
//#NT#2011/06/14#Hideo Lin -begin
//#NT#For using new EXIF
#include "ExifRW.h"
//#NT#2011/06/14#Hideo Lin -end
#include "sen_isr.h"
#include "fd_lib.h"
#include "sd_lib.h"
#include "IPPCtrl.h"
#include "ae_api.h"
#include "ae_alg.h"//for get ae lv
#include "awb_api.h"
#include "Flashlight_api.h"
//#include    "AlbumShot.h"
//#include    "ImageEffect.h"
//#include    "PanCap_lib.h"
//#NT#2011/11/29#KS Hung -begin
#include "Lens.h"
//#NT#2011/11/29#KS Hung -end
#define Get10BitRAW DISABLE
#define Get8BitRAW DISABLE

#define _CAF_FUNC_  DISABLE
#define _IMAGE_BORDER_FUNC_ DISABLE
#if (_CAF_FUNC_== ENABLE)
#include    "caf_lib.h"
#endif
#if(_IMAGE_BORDER_FUNC_ == ENABLE)
#include "ImgBorder.h"
#endif

#if OV5633_220
#include "IPP_OV5633_220_int.h"
#include "IQSetting_OV5633_220.h"
#define SENSORLIB_DEBUG  1

#if (SENSORLIB_DEBUG)
#define SENSORLIB_MSG(...)    debug_msg (" SENLIBDRV: "__VA_ARGS__)
#else
#define SENSORLIB_MSG(...)
#endif

const UINT32 CapBufAlloc[Max_BurstNum][3]=
{
    //{raw, jpeg, CapCnt},
    {0x01, 0x01, 0x01},  //_BurstNum_Dummy
    {0x01, 0x01, 0x01},  //_BurstNum_1
    {0x01, 0x02, 0x02},  //_BurstNum_2
    {0x02, 0x02, 0x03},  //_BurstNum_3
    {0x01, 0x02, 0x04},  //_BurstNum_4
    {0x01, 0x01, 0x05},  //_BurstNum_5
    {0x01, 0x02, 0xff},  //_BurstNum_Inf
};

void ECS_TableSel(void)
{
}

UINT32 Get_CapBufAlloc(UINT32 type)
{
    return CapBufAlloc[gImageAlgInfo.BurstDriveNum][type];
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void Set2Preview(PIPPCTRL_INFO info, PIPPCTRL_RST result)
{
    CAL_DATA CalInfo;
    PImageAlgInfo pimginfo;
    SENSOR_STATUS SensorStatus;
    AE_EVENT_TAB aeEventTab;
    static BOOL bFirst = TRUE;
    SENSORLIB_MSG("%s: start\r\n",__func__);
    pimginfo = UI_GetParameter();
    Cal_GetCalData(&CalInfo);
    Sensor_GetStatus(&SensorStatus);

    aeEventTab.AeProcessEnd = AeProcessEnd;
    AE_LoadEventTAB(&aeEventTab);

    gAlgMsg.Alg_Mode=MODE_Preview;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_PREVIEW, &gAlgMsg);
    }

    SIE_PRE_Para_prv.ModeSel = SensorStatus.uiSensorMode;
    SIE_PRE_Para_prv.VGAType = SensorStatus.uiReadoutMode;

    //IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType);
    Set_digitalzoom(Get_DzoomIndex(), pimginfo->SieBitDepth);

//    SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
//    PRV_SIE_BP2(SIE_PRE_Para_prv.ModeSel,&SIE_PRE_Para_prv.SieBp2);
    SIE_PRE_Para_prv.SieBp1 = 10; // PRV_SIE_BP1
    SIE_PRE_Para_prv.SieBp2 = SensorStatus.pSyncInfo->Act_CenterY;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieVDPhase= 0;
    SIE_PRE_Para_prv.SieHDPhase = 0;
    SIE_PRE_Para_prv.SieDataPhase = 0;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = FALSE;
    SIE_PRE_Para_prv.SiePathSel= PATH_POST_GAMMA;
    SIE_PRE_Para_prv.SieFuncEn = SIE_SUBOUT_EN|SIE_CG_EN|SIE_GAMMA_EN;
    SIE_PRE_Para_prv.SieVDInv = Invert;
    SIE_PRE_Para_prv.SieHDInv = Invert;
    SIE_PRE_Para_prv.SieOutModSel = result->SieMode;
    SIE_PRE_Para_prv.SieOutBufNum = result->siebufnum;
    SIE_PRE_Para_prv.SieDRAMInEn = FALSE;
    SIE_PRE_Para_prv.SieCfaPat = SensorStatus.uiBayer;
    if (SIE_PRE_Para_prv.AutoObEn == DISABLE)
    {
        SetIPPSizeInfor(_OBOffset, SensorStatus.pSyncInfo->OB_Offset);
    }
    else
    {
        SetIPPSizeInfor(_OBOffset, 0x0);
    }

    if ((CalInfo.CalStatus_PrvDP_B == _Prv_B_Status) && \
        (CalInfo.CalStatus_PrvDP_W == _Prv_W_Status))
    {
        SIE_PRE_Para_prv.SiePreAddr2 = CalInfo.Cal_DP_PreviewAddr;
        SIE_PRE_Para_prv.SieFuncEn |= SIE_BLM_EN;
    }
    if(CalInfo.CalStatus_ECS == _ECS_Status)
    {
        SIE_PRE_Para_prv.SieFuncEn |= SIE_ECS_EN;
        ippbuf_SetBuf(IPPBUF_ECS, CalInfo.Cal_ECS_Addr);
    }

    //initial ipe parameter
    IPE_Para.CfaPat = Sen_GetIPECfaPat(SensorStatus.uiBayer,pimginfo->ImageFlip);
    IPE_Para.IntEn = IPE_INTE_FRM;
    IPE_Para.StripeMode = result->IpeMode;

    //initial ime parameter
    SenFlowFunc(IME_IQ_Init);
    IME_D1_Para_ext.op_mode = result->ImeMode;
    IME_D1_Para_ext.Ime_BuffNum = result->imebufnum[ImePath1];
    switch(result->ImeFmt[ImePath1])
    {
        case FMT444:
            IME_D1_Para.out_format = IME_OUTPUT_YCC_444;
            break;
        case FMT422:
            IME_D1_Para.out_format = IME_OUTPUT_YCC_422_COS;
            break;
        case FMT420:
            IME_D1_Para.out_format = IME_OUTPUT_YCC_420_COS;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }

    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.pIpe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct.pIme_fbaddr = &IME_D1_Para;
    Ipp_Param.Ime_Struct.pIme_Ext = &IME_D1_Para_ext;
    Ipp_Param.Ife_Struct = &Ife_Param;

//#NT#2009/11/18#Bowen Li -begin
//#NT#fine tune cap-preview performance
    if (IPL_GetMode() == IPL_MODE_CAP)
    {
        Set2Prv_Common(&Ipp_Param, DISABLE, DISABLE);
    }
    else
    {
        Set2Prv_Common(&Ipp_Param, ENABLE, DISABLE);
    }

//#NT#2009/11/18#Bowen Li -end
    sie_waitVD(1);
    if(bFirst)
    {
        sie_waitVD(2);//inorder to make sure sensor output data is stable
        //ethan_20110808_add for ae awb quick converge
        AE_WaitStable(2);
        AWB_Process();
        //wait color gain latch
        sie_waitVD(1);
        ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
        bFirst= FALSE;
    }
    else
        ime_waitFlagFrameEnd(IME_CLEAR_FLAG);//wait ime out ready for ide to show
    Set_SenInfo(IPPImg_Preview);
    SenFlowFunc(AAFD_Switch);

    SENSORLIB_MSG("%s: end\r\n",__func__);
}

void Set2Video(PIPPCTRL_INFO info, PIPPCTRL_RST result)
{
    CAL_DATA CalInfo;
    PImageAlgInfo pimginfo;
    SENSOR_STATUS SensorStatus;
    SENSORLIB_MSG("%s: start\r\n",__func__);
    pimginfo = UI_GetParameter();
    Cal_GetCalData(&CalInfo);
    Sensor_GetStatus(&SensorStatus);

    gAlgMsg.Alg_Mode=MODE_Video_Recording;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_VIDEO_RECORDING, &gAlgMsg);
    }

    SetIPPSizeInfor(_ImeOut2Hsize, info->uiPath2OutsizeH);
    SetIPPSizeInfor(_ImeOut2Vsize, info->uiPath2OutsizeV);
    SetIPPSizeInfor(_ImeOut2LineOffset, info->uiPath2OutLineoffset);

    SIE_PRE_Para_prv.ModeSel = SensorStatus.uiSensorMode;
    SIE_PRE_Para_prv.VGAType = SensorStatus.uiReadoutMode;
    //IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType);
    Set_digitalzoom(Get_DzoomIndex(), pimginfo->SieBitDepth);

    SetIPPSizeInfor(_ImeOut1Hsize, info->uiPath1OutsizeH);
    SetIPPSizeInfor(_ImeOut1Vsize, info->uiPath1OutsizeV);
    SetIPPSizeInfor(_ImeOut1LineOffset, info->uiPath1OutLineoffset);
    //SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
    //PRV_SIE_BP2(SIE_PRE_Para_prv.ModeSel,&SIE_PRE_Para_prv.SieBp2);
    SIE_PRE_Para_prv.SieBp1 = 10; // PRV_SIE_BP1
    SIE_PRE_Para_prv.SieBp2 = SensorStatus.pSyncInfo->Act_CenterY;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_OVFL1|SIE_INTE_BP1;
    if(result->SieMode == SIE_SLICE_MODE)
    {
        SIE_PRE_Para_prv.SieIntEn |= SIE_INTE_SLICE_END;
    }
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieVDPhase= 0;
    SIE_PRE_Para_prv.SieHDPhase = 0;
    SIE_PRE_Para_prv.SieDataPhase = 0;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = FALSE;
    SIE_PRE_Para_prv.SiePathSel= PATH_POST_GAMMA;
    SIE_PRE_Para_prv.SieFuncEn = SIE_SUBOUT_EN|SIE_CG_EN|SIE_GAMMA_EN;
    SIE_PRE_Para_prv.SieVDInv = Notinvert;
    SIE_PRE_Para_prv.SieHDInv = Invert;
    SIE_PRE_Para_prv.SieDRAMInEn = FALSE;
    SIE_PRE_Para_prv.SieOutModSel = result->SieMode;
    SIE_PRE_Para_prv.SieOutBufNum = result->siebufnum;
    SIE_PRE_Para_prv.SieCfaPat = SensorStatus.uiBayer;
    if ((CalInfo.CalStatus_PrvDP_B == _Prv_B_Status) && (CalInfo.CalStatus_PrvDP_W == _Prv_W_Status))
    {
        SIE_PRE_Para_prv.SiePreAddr2 = CalInfo.Cal_DP_PreviewAddr;
        SIE_PRE_Para_prv.SieFuncEn |= SIE_BLM_EN;
    }
    if(CalInfo.CalStatus_ECS == _ECS_Status)
    {
        SIE_PRE_Para_prv.SieFuncEn |= SIE_ECS_EN;
        ippbuf_SetBuf(IPPBUF_ECS, CalInfo.Cal_ECS_Addr);
    }

    //initial ipe parameter
    IPE_Para.CfaPat = Sen_GetIPECfaPat(SensorStatus.uiBayer,pimginfo->ImageFlip);
    IPE_Para.IntEn = /*IPE_INTE_SLI|*/IPE_INTE_FRM;
    IPE_Para.StripeMode = result->IpeMode;

    //initial ime parameter
    SenFlowFunc(IME_IQ_Init);
    IME_D2_Para_ext.ImeInten = IME_INTE_FRM;
    if(result->ImeMode == IPE2IME_SLICE_MODE)
    {
        IME_D2_Para_ext.ImeInten |= IME_INTE_SLICE_DN1;
    }
    IME_D2_Para_ext.op_mode = result->ImeMode;

    IME_D2_Para_ext.Ime_BuffNum = result->imebufnum[ImePath2];
    switch(result->ImeFmt[ImePath2])
    {
        case FMT444:
            IME_D2_Para.out_format = IME_OUTPUT_YCC_444;
            break;
        case FMT422:
            IME_D2_Para.out_format = IME_OUTPUT_YCC_422_COS;
            break;
        case FMT420:
            IME_D2_Para.out_format = IME_OUTPUT_YCC_420_COS;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }
    IME_D2_Para_ext.p1_out_slice_height = result->uiImeSliceH;

    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.pIpe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct.pIme_fbaddr = &IME_D1_Para;
    Ipp_Param.Ime_Struct.pIme_Ext = &IME_D1_Para_ext;
    Ipp_Param.Ime_Struct2.pIme_fbaddr = &IME_D2_Para;
    Ipp_Param.Ime_Struct2.pIme_Ext = &IME_D2_Para_ext;
    Ipp_Param.Ife_Struct = &Ife_Param;

    Set2Prv_Common(&Ipp_Param, ENABLE, ENABLE);
    sie_waitVD(1);
    ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
    ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);//wait ime out ready for ide to show
    Set_SenInfo(IPPImg_Video);
    SenFlowFunc(AAFD_Switch);

    SENSORLIB_MSG("%s: end\r\n",__func__);
}

void SetDzoom2Video(unsigned int ZoomIndex)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    Set_SenInfo(IPPImg_DzoomEnter);
    SenFlowFunc(AAFD_Switch);
    Set_digitalzoom(ZoomIndex, pimginfo->SieBitDepth);

    SIE_PRE_Para_prv.SieCropSizeH = GetIPPSizeInfor(_SieCropHsize);
    SIE_PRE_Para_prv.SieCropSizeV = GetIPPSizeInfor(_SieCropVsize);
    SIE_PRE_Para_prv.SieOutSizeH = GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF;
    SIE_PRE_Para_prv.SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);

    //IPE_Para.InSizeH = SIE_PRE_Para_prv.SieOutSizeH;
    IPE_Para.InSizeH = GetIPPSizeInfor(_IpeOutHsize)+IPE_HSIZE_IODIFF;
    IPE_Para.InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    IPE_Para.InLineOfs = SIE_PRE_Para_prv.SieOutLineOfs;

    IME_D2_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D2_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D2_Para.in_lineoffset = IME_D2_Para.in_h;
    //ime input format alyway 444 under ipe-ime direct path
    IME_D2_Para.in_Cb_lineoffset = IME_D2_Para.in_lineoffset;
    IME_D2_Para.in_Cr_lineoffset = IME_D2_Para.in_lineoffset;

    IME_D2_Para.out_h=GetIPPSizeInfor(_ImeOut2Hsize);
    IME_D2_Para.out_v=GetIPPSizeInfor(_ImeOut2Vsize);
    IME_D2_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut2LineOffset);
    switch(IME_D2_Para.out_format)
    {
        case IME_OUTPUT_YCC_444:
            IME_D2_Para.out_Cb_lineoffset = IME_D2_Para.out_lineoffset;
            IME_D2_Para.out_Cr_lineoffset = IME_D2_Para.out_lineoffset;
            break;
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            IME_D2_Para.out_Cb_lineoffset = IME_D2_Para.out_lineoffset/2;
            IME_D2_Para.out_Cr_lineoffset = IME_D2_Para.out_lineoffset/2;
            break;
        default:
            debug_err(("ime out format error!\r\n"));
            break;
    }
    if(SIE_PRE_Para_prv.SieOutModSel == SIE_SLICE_MODE)
    {
        SIE_PRE_Para_prv.SieSliceHight = (SIE_PRE_Para_prv.SieCropSizeV+SIE_PRE_Para_prv.SieOutBufNum)/(SIE_PRE_Para_prv.SieOutBufNum+1);
    }
    else
    {
        SIE_PRE_Para_prv.SieSliceHight = SIE_PRE_Para_prv.SieCropSizeV;
    }
    //#NT#2011/02/08#ethanlau -begin
    //#NT#for ipe/ipe+ime slice mode NLM calculation
    IPL_SliceSize(&Ipp_Param,IMEALG_1PATH);//(&IPE_Para,&IME_D2_Para,&IME_D2_Para_ext);
    //#NT#2011/02/08#ethanlau -end

    Clr_DZoomFlag(DZOOM_IDLE);
    Set_DZoomFlag(DZOOM_SET_SIEPRE);
    while(!(Get_DZoomFlag() & DZOOM_IDLE));

    Set_SenInfo(IPPImg_DzoomLeave);
    SenFlowFunc(AAFD_Switch);
    gAlgMsg.DzoomIndex=ZoomIndex;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_CHGDZOOMSTEP, &gAlgMsg);
    }
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDzoom2Preview(unsigned int ZoomIndex)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();

    Set_SenInfo(IPPImg_DzoomEnter);
    SenFlowFunc(AAFD_Switch);
    Set_digitalzoom(ZoomIndex, pimginfo->SieBitDepth);

    SIE_PRE_Para_prv.SieCropSizeH = GetIPPSizeInfor(_SieCropHsize);
    SIE_PRE_Para_prv.SieCropSizeV = GetIPPSizeInfor(_SieCropVsize);
    SIE_PRE_Para_prv.SieOutSizeH = GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF;
    SIE_PRE_Para_prv.SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);

    //IPE_Para.InSizeH = SIE_PRE_Para_prv.SieOutSizeH;
    IPE_Para.InSizeH = GetIPPSizeInfor(_IpeOutHsize)+IPE_HSIZE_IODIFF;
    IPE_Para.InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    IPE_Para.InLineOfs = SIE_PRE_Para_prv.SieOutLineOfs;

    IME_D1_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D1_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D1_Para.in_lineoffset = IME_D1_Para.in_h;
    //ime input format alyway 444 under ipe-ime direct path
    IME_D1_Para.in_Cb_lineoffset = IME_D1_Para.in_lineoffset;
    IME_D1_Para.in_Cr_lineoffset = IME_D1_Para.in_lineoffset;

    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeOut1Vsize);

    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);
    switch(IME_D1_Para.out_format)
    {
        case IME_OUTPUT_YCC_444:
            IME_D1_Para.out_Cb_lineoffset = IME_D1_Para.out_lineoffset;
            IME_D1_Para.out_Cr_lineoffset = IME_D1_Para.out_lineoffset;
            break;
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            IME_D1_Para.out_Cb_lineoffset = IME_D1_Para.out_lineoffset/2;
            IME_D1_Para.out_Cr_lineoffset = IME_D1_Para.out_lineoffset/2;
            break;
        default:
            debug_err(("ime out format error!\r\n"));
            break;
    }
    //#NT#2011/02/08#ethanlau -begin
    //#NT#for ipe/ipe+ime slice mode NLM calculation
    SIE_PRE_Para_prv.SieSliceHight = (SIE_PRE_Para_prv.SieCropSizeV+SIE_PRE_Para_prv.SieOutBufNum)/(SIE_PRE_Para_prv.SieOutBufNum+1);
    IPL_SliceSize(&Ipp_Param,IMEALG_1PATH);
    //#NT#2011/02/08#ethanlau -end
    Clr_DZoomFlag(DZOOM_IDLE);
    Set_DZoomFlag(DZOOM_SET_SIEPRE);
    while(!(Get_DZoomFlag() & DZOOM_IDLE));

    Set_SenInfo(IPPImg_DzoomLeave);
    SenFlowFunc(AAFD_Switch);
    gAlgMsg.DzoomIndex=ZoomIndex;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_CHGDZOOMSTEP, &gAlgMsg);
    }
}

void AF_PrvModeSel(UINT32 Mode, UINT32 ModeType, UINT32 step)
{
    SENSOR_STATUS SensorStatus;
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    Sensor_GetStatus(&SensorStatus);

    switch(step)
    {
        case 1:
            SIE_PRE_Para_prv.ModeSel = Mode;
            SIE_PRE_Para_prv.VGAType = SensorStatus.uiReadoutMode;
            //IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType);
            Set_digitalzoom(Get_DzoomIndex(), pimginfo->SieBitDepth);

            //SIE_PRE_Para_prv.SieBp1 = AF_SIE_BP1;
            //SIE_PRE_Para_prv.SieBp2 = AF_SIE_BP2;
            SIE_PRE_Para_prv.SieBp1 = 10; // PRV_SIE_BP1
            SIE_PRE_Para_prv.SieBp2 = SensorStatus.pSyncInfo->Act_CenterY;

            Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
            Ipp_Param.pIpe_Struct = &IPE_Para;
            Ipp_Param.Ime_Struct.pIme_fbaddr = &IME_D1_Para;
            Ipp_Param.Ime_Struct.pIme_Ext = &IME_D1_Para_ext;
            Ipp_Param.Ife_Struct = &Ife_Param;
            Clr_ChgModeFlag(CHGMODE_IDLE);
            Set_ChgModeFlag(CHGMODE_SIE_SET);
            SetSensorMode(SIE_PRE_Para_prv.ModeSel);
            //SetPrvParams(&Ipp_Param);
        break;
        case 2:
            //sie_waitVD(1);
            SenFlowFunc(SENSOR_MODE_SETTING);
            while(!(Get_ChgModeFlag() & CHGMODE_IDLE));
            //#NT#2009/12/09#Bowen Li -begin
            //#NT#fix bug for image division issue
                //ipe_reset(ON);
                //ime_setReset();
            //#NT#2009/12/09#Bowen Li -end
            //SetSensorMode(SIE_PRE_Para_prv.ModeSel);
            //SetPrvParams(&Ipp_Param);
            ime_Set2Preview(Ipp_Param.Ime_Struct.pIme_fbaddr,Ipp_Param.Ime_Struct.pIme_Ext);
            //ipe stripe
            Sen_GetImeStripeInfo(&Ipp_Param.Ime_Struct.pIme_Ext->ImeStripe);
            //H stripe
            Ipp_Param.pIpe_Struct->HStripe.stripe.n = Ipp_Param.Ime_Struct.pIme_Ext->ImeStripe.Hstripe.n;
            Ipp_Param.pIpe_Struct->HStripe.stripe.l = Ipp_Param.Ime_Struct.pIme_Ext->ImeStripe.Hstripe.l;
            Ipp_Param.pIpe_Struct->HStripe.stripe.m = Ipp_Param.Ime_Struct.pIme_Ext->ImeStripe.Hstripe.m;
            //V stripe
            Ipp_Param.pIpe_Struct->VStripe.stripe.n = Ipp_Param.Ime_Struct.pIme_Ext->ImeStripe.Vstripe.n;
            Ipp_Param.pIpe_Struct->VStripe.stripe.l = Ipp_Param.Ime_Struct.pIme_Ext->ImeStripe.Vstripe.l;
            Ipp_Param.pIpe_Struct->VStripe.stripe.m = Ipp_Param.Ime_Struct.pIme_Ext->ImeStripe.Vstripe.m;
            //stripe overlap
            Ipp_Param.pIpe_Struct->HStripe.olap = IPE_OLAP_ON;
            Ipp_Param.pIpe_Struct->VStripe.olap = IPE_OLAP_ON;
            ipe_Set2Preview(Ipp_Param.pIpe_Struct);

            sie_Set2Preview(Ipp_Param.Sie_Struct);
            //Move to AFdrv_AEPrepare
            //SenFlowFunc(AE_PRV_SETTING);
            SenFlowFunc(AWB_PRV_SETTING);
        break;
        case 3:
            //sie_waitVD(1);

            sie_start();
            //#NT#2009/12/02#Bowen Li -begin
            //#NT#fix bug for image division issue
        break;
        case 4:
            ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
            //#NT#2009/12/02#Bowen Li -end
        break;
        default:
            debug_err(("ERROR!! NO this step for AF mode transfer(%d)!!",step));
        break;
    }
}

#if 0
#pragma mark -
#endif

void CaptureOne(SIE_PRE_PARAM *SiePreParam)
{
    CAL_DATA CalInfo;
    UINT32 Addr2;

    Cal_GetCalData(&CalInfo);
    if(CalInfo.CalStatus_ECS == _ECS_Status)
    {
        SiePreParam->SieFuncEn |= SIE_ECS_EN;
        ippbuf_SetBuf(IPPBUF_ECS, CalInfo.Cal_ECS_Addr);
    }

    if (GetIPPFuncInfor(DFS_EN))
    {
        SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
        //Addr1 = SiePreParam->SiePreAddr0;
        Addr2 = SiePreParam->SiePreAddr2;
        //SiePreParam->SiePreAddr0 = Addr2;

        if ((CalInfo.CalStatus_CapDP_B == _Cap_B_Status) && (CalInfo.CalStatus_CapDP_W == _Cap_W_Status))
        {
            SiePreParam->SiePreAddr2 = CalInfo.Cal_DP_CaptureAddr;
            SiePreParam->SieFuncEn |= SIE_BLM_EN;
            SiePreParam->SieFuncEn |= SIE_DF_EMBED_EN;
        }

        sie_Set2Capture(SiePreParam);
        Sensor_Cap2Prv();

        SetIPPFuncInfor(DFS_CNT,DFS_SECONDPIC);
        SiePreParam->SiePathSel = PATH_PRE_GAMMA;
        //SiePreParam->SiePreAddr0 = Addr1;
        SiePreParam->SiePreAddr2 = Addr2;
        sie_Set2Capture(SiePreParam);
        Sensor_Cap2Prv();
        SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
    }
    else
    {
        Addr2 = SiePreParam->SiePreAddr2;

        if ((CalInfo.CalStatus_CapDP_B == _Cap_B_Status) && \
            (CalInfo.CalStatus_CapDP_W == _Cap_W_Status))
        {
            SiePreParam->SiePreAddr2 = CalInfo.Cal_DP_CaptureAddr;
            SiePreParam->SieFuncEn |= SIE_BLM_EN;
        }
        if(GetIPPFuncInfor(SIED2D_EN))
        {
            UINT32 FuncEn,Path;
            FuncEn = SiePreParam->SieFuncEn;
            Path = SiePreParam->SiePathSel;
            SiePreParam->SieFuncEn &= ~(SIE_GAMMA_EN|SIE_CG_EN);
            SiePreParam->SiePathSel = PATH_PRE_COLORGAIN;
            sie_Set2Capture(SiePreParam);
            SiePreParam->SieFuncEn = FuncEn;
            SiePreParam->SiePathSel = Path;
        }
        else
        {
            sie_Set2Capture(SiePreParam);
            Sensor_Cap2Prv();
        }
        SENSORLIB_MSG("Sie Raw End\r\n");
        SiePreParam->SiePreAddr2 = Addr2;
    }
#if (Get10BitRAW|Get8BitRAW)
    if(1)
#else
    if (gImageAlgInfo.FileFmt & _FileFmt_Raw)
#endif
    {
        SENSORLIB_MSG("Addr: 0X%x size:%d X %d \r\n",SiePreParam->SieOutAdd0,SiePreParam->SieOutLineOfs,SiePreParam->SieCropSizeV);
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)SiePreParam->SieOutAdd0,
                        SiePreParam->SieCropSizeV * SiePreParam->SieOutLineOfs,
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);
    }
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif

void siepre_GetCapRaw(SIE_PRE_PARAM *SiePreParam)
{
    UINT32 Line,Pixel2;
    STR_SIE_MSHTR_PARAM _mshut_param;
    PImageAlgInfo pimginfo;
    SENSOR_STATUS SensorStatus;
    UINT32 capiso,capext,iris;
    FLASH_PARAM *FlashParam;

    Sensor_GetStatus(&SensorStatus);
    FlashParam = GetFlashParam();

    pimginfo = UI_GetParameter();

    //while (!(sie_getIntrStatus() & SIE_INT_FLDEND));
    //sie_ClrIntrStatus(SIE_INT_ALL);
    sie_enableInt(ENABLE, SiePreParam->SieIntEn);
    //ena_int(_INT_SIE);

    Sensor_ChgMode(SiePreParam->ModeSel);
    if(FlashParam->FakeFlash != 0)
    {
        flash_ConfigMainFlash(FlashParam->DelayLineMainFlash, FlashParam->WidthMainFlash,FALSE);
        flash_TriggerFlash();
    }
    sie_waitVD(1);
    AE_GetCapAEArg(&capiso,&capext,&iris);
    if(CheckFlashFireStatus() != FALSE)
    {
        if(FlashParam->FakeFlash == 0)
        {
            //ConfigMainFlash in flash.c
            Sensor_SetExpTime(FlashParam->ExpoTime_MainFlash);
            Sensor_SetISO(FlashParam->ISO_MainFlash);
            //flash_ConfigMainFlash(SensorStatus.pSyncInfo->VD_total-10, FlashParam->WidthMainFlash,FALSE);
            flash_ConfigMainFlash(FlashParam->DelayLineMainFlash, FlashParam->WidthMainFlash,FALSE);
            flash_TriggerFlash();
            debug_err(("Flash ExpoTime=%d, ISO=%d, VDTotal=%d\r\n",FlashParam->ExpoTime_MainFlash,FlashParam->ISO_MainFlash,SensorStatus.pSyncInfo->VD_total));
        }
        else
        {
            Sensor_SetExpTime(capext);
            Sensor_SetISO(capiso);
        }
    }
    else
    {
        Sensor_SetExpTime(capext);
        Sensor_SetISO(capiso);
    }

    sie_waitVD(1);
    if(pimginfo->Exposuremode_UI == _GRS)
    {
        Time2_Line_Pixel2(SiePreParam->MShutterTime, SensorStatus.uiPclk, GetIPPSizeInfor(_HDTotal), &Line, &Pixel2);
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

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void sie_PrvParameter(struct  PUBLIC_STR_JOBREQS *job_req)
{
    //CG and Ofs
    if(job_req->SIE_PRE_PRV->SieFuncEn & SIE_CG_EN)
    {
        UINT32 Rg,Gg,Bg;
        AWB_GetColorGian(&Rg,&Gg,&Bg);
        switch(job_req->SIE_PRE_PRV->SieCfaPat)
        {
            case IPE_PATR:
                job_req->sie_cg_info.Gain0 = Rg;
                job_req->sie_cg_info.Gain1 = Gg;
                job_req->sie_cg_info.Gain2 = Gg;
                job_req->sie_cg_info.Gain3 = Bg;
                break;
            case IPE_PATGR:
                job_req->sie_cg_info.Gain0 = Gg;
                job_req->sie_cg_info.Gain1 = Rg;
                job_req->sie_cg_info.Gain2 = Bg;
                job_req->sie_cg_info.Gain3 = Gg;
                break;
            case IPE_PATGB:
                job_req->sie_cg_info.Gain0 = Gg;
                job_req->sie_cg_info.Gain1 = Bg;
                job_req->sie_cg_info.Gain2 = Rg;
                job_req->sie_cg_info.Gain3 = Gg;
                break;
            case IPE_PATB:
                job_req->sie_cg_info.Gain0 = Bg;
                job_req->sie_cg_info.Gain1 = Gg;
                job_req->sie_cg_info.Gain2 = Gg;
                job_req->sie_cg_info.Gain3 = Rg;
                break;
        }
        job_req->sie_cofs_info.Ofs0 = 0;
        job_req->sie_cofs_info.Ofs1 = 0;
        job_req->sie_cofs_info.Ofs2 = 0;
        job_req->sie_cofs_info.Ofs3 = 0;
    }
    if (job_req->SIE_PRE_PRV->SieFuncEn & SIE_ECS_EN)
    {
        job_req->SIE_PRE_PRV->Sie_ECS.MapDthrEn = DISABLE;
        job_req->SIE_PRE_PRV->Sie_ECS.MapDthrRst = DISABLE;
        job_req->SIE_PRE_PRV->Sie_ECS.MapSizeSel = ECS_TblSel;
        job_req->SIE_PRE_PRV->Sie_ECS.MapDthrLevel = 0;
        job_req->SIE_PRE_PRV->Sie_ECS.MapShift = 8;
        job_req->SIE_PRE_PRV->Sie_ECS.CfaPat = job_req->SIE_PRE_PRV->SieCfaPat;
        job_req->SIE_PRE_PRV->SiePreAddr1= ippbuf_GetBuf(IPPBUF_ECS);
    }
    if (job_req->SIE_PRE_PRV->SieFuncEn & SIE_GAMMA_EN)
    {
        job_req->sra_gammaLUT = SRA_SIE_GammaLUT_Using;
    }
    if (job_req->SIE_PRE_PRV->SieFuncEn & SIE_LINE_BLM_EN)
    {
        job_req->SIE_PRE_PRV->SieDefNfact = DEFECT_NFACT_0;
        job_req->SIE_PRE_PRV->SieDefColumn = NULL;
    }
    if(job_req->SIE_PRE_PRV->SieFuncEn & SIE_DF_SUB_EN)
    {
        job_req->sie_dfs_setting.DfsOp = DFS_A_MINUS_B;
        job_req->sie_dfs_setting.DfsShiftNum = 0;
        job_req->sie_dfs_setting.DfsTh = 0;
        job_req->sie_dfs_setting.DfsThSrc = DFS_TH_DRAM;

//        job_req->SIE_PRE_PRV->SiePreAddr2 = NULL;
    }
//    else if(job_req->SIE_PRE_PRV->SieFuncEn & SIE_BLM_EN)
//    {
//        job_req->SIE_PRE_PRV->SiePreAddr2 = NULL;
//    }

    if(job_req->SIE_PRE_PRV->SieFuncEn & SIE_SUBOUT_EN)
    {
        UINT32 sizeH,sizeV;
        job_req->SIE_PRE_PRV->SieSubParam.Hsub = 0;
        job_req->SIE_PRE_PRV->SieSubParam.Vsub = 0;
        //#NT#2011/01/24#ethanlau -begin
        //#NT#chk sie sub out size.must small than 320x240
        while(1)
        {
            sizeH = (((job_req->SIE_PRE_PRV->SieOutSizeH - 1)/(1<<job_req->SIE_PRE_PRV->SieSubParam.Hsub))>>2)<<2;
            sizeV = ((job_req->SIE_PRE_PRV->SieCropSizeV - 1)/(1<<job_req->SIE_PRE_PRV->SieSubParam.Vsub));
            if( (sizeH <= SIE_SubOut_H)&&(sizeV <= SIE_SubOut_V) )
            {
                break;
            }
            if(sizeH > SIE_SubOut_H)
                job_req->SIE_PRE_PRV->SieSubParam.Hsub++;
            if(sizeV > SIE_SubOut_V)
                job_req->SIE_PRE_PRV->SieSubParam.Vsub++;
        }
        //#NT#2011/01/24#ethanlau -end
        job_req->SIE_PRE_PRV->SieSubParam.Hcnt = sizeH;
        job_req->SIE_PRE_PRV->SieSubParam.Vcnt = sizeV;
        job_req->SIE_PRE_PRV->SieSubParam.SubUpdateMode = FRAME_UPDATE;
        job_req->SIE_PRE_PRV->SieSubParam.SubOutSrc = PATH_PRE_COLORGAIN;
        job_req->SIE_PRE_PRV->SieSubAddr = ippbuf_GetBuf(IPPBUF_CA);
        SetIPPSizeInfor(_CAHsize, sizeH);
        SetIPPSizeInfor(_CAVsize, sizeV);
        SetIPPSizeInfor(_CALineOffset, sizeH);
    }

    job_req->SIE_PRE_PRV->SieHBinParam.PixAdd = 0;
    job_req->SIE_PRE_PRV->SieHBinParam.PixDiv = 0;
}

void sie_CapParameter(struct  PUBLIC_STR_JOBREQS *job_req)
{
    FLASH_PARAM *FlashParam;

    //CG and Ofs
    if(job_req->SIE_PRE_CAP->SieFuncEn & SIE_CG_EN)
    {
        UINT32 Rg,Gg,Bg;

        FlashParam = GetFlashParam();
        if((CheckFlashFireStatus() == TRUE) && (FlashParam->FakeFlash == 0))
            Flash_GetColorGian(&Rg,&Gg,&Bg);
        else
            AWB_GetColorGian(&Rg,&Gg,&Bg);

        switch(job_req->SIE_PRE_CAP->SieCfaPat)
        {
            case IPE_PATR:
                job_req->sie_cg_info.Gain0 = Rg;
                job_req->sie_cg_info.Gain1 = Gg;
                job_req->sie_cg_info.Gain2 = Gg;
                job_req->sie_cg_info.Gain3 = Bg;

                job_req->sie_cofs_info.Ofs0 = 2;
                job_req->sie_cofs_info.Ofs1 = 4;
                job_req->sie_cofs_info.Ofs2 = 4;
                job_req->sie_cofs_info.Ofs3 = 1;
                break;
            case IPE_PATGR:
                job_req->sie_cg_info.Gain0 = Gg;
                job_req->sie_cg_info.Gain1 = Rg;
                job_req->sie_cg_info.Gain2 = Bg;
                job_req->sie_cg_info.Gain3 = Gg;

                job_req->sie_cofs_info.Ofs0 = 4;
                job_req->sie_cofs_info.Ofs1 = 2;
                job_req->sie_cofs_info.Ofs2 = 1;
                job_req->sie_cofs_info.Ofs3 = 4;
                break;
            case IPE_PATGB:
                job_req->sie_cg_info.Gain0 = Gg;
                job_req->sie_cg_info.Gain1 = Bg;
                job_req->sie_cg_info.Gain2 = Rg;
                job_req->sie_cg_info.Gain3 = Gg;

                job_req->sie_cofs_info.Ofs0 = 4;
                job_req->sie_cofs_info.Ofs1 = 1;
                job_req->sie_cofs_info.Ofs2 = 2;
                job_req->sie_cofs_info.Ofs3 = 4;
                break;
            case IPE_PATB:
                job_req->sie_cg_info.Gain0 = Bg;
                job_req->sie_cg_info.Gain1 = Gg;
                job_req->sie_cg_info.Gain2 = Gg;
                job_req->sie_cg_info.Gain3 = Rg;

                job_req->sie_cofs_info.Ofs0 = 1;
                job_req->sie_cofs_info.Ofs1 = 4;
                job_req->sie_cofs_info.Ofs2 = 4;
                job_req->sie_cofs_info.Ofs3 = 2;
                break;
        }
    }
    if (job_req->SIE_PRE_CAP->SieFuncEn & SIE_ECS_EN)
    {
        job_req->SIE_PRE_CAP->Sie_ECS.MapDthrEn = DISABLE;
        job_req->SIE_PRE_CAP->Sie_ECS.MapDthrRst = DISABLE;
        job_req->SIE_PRE_CAP->Sie_ECS.MapSizeSel = ECS_TblSel;
        job_req->SIE_PRE_CAP->Sie_ECS.MapDthrLevel = 0;
        job_req->SIE_PRE_CAP->Sie_ECS.MapShift = 8;
        job_req->SIE_PRE_CAP->Sie_ECS.CfaPat = job_req->SIE_PRE_CAP->SieCfaPat;
        job_req->SIE_PRE_CAP->SiePreAddr1= ippbuf_GetBuf(IPPBUF_ECS);
    }
    if (job_req->SIE_PRE_CAP->SieFuncEn & SIE_GAMMA_EN)
    {
        job_req->sra_gammaLUT = SRA_SIE_GammaLUT_Using;
    }
    if (job_req->SIE_PRE_CAP->SieFuncEn & SIE_LINE_BLM_EN)
    {
        job_req->SIE_PRE_CAP->SieDefNfact = DEFECT_NFACT_0;
        job_req->SIE_PRE_CAP->SieDefColumn = NULL;
    }
    if(job_req->SIE_PRE_CAP->SieFuncEn & SIE_DF_SUB_EN)
    {
        job_req->sie_dfs_setting.DfsOp = DFS_A_MINUS_B;
        job_req->sie_dfs_setting.DfsShiftNum = 0;
        job_req->sie_dfs_setting.DfsTh = 0;
        job_req->sie_dfs_setting.DfsThSrc = DFS_TH_DRAM;

//        job_req->SIE_PRE_CAP->SiePreAddr2 = NULL;
    }
//    else if(job_req->SIE_PRE_CAP->SieFuncEn & SIE_BLM_EN)
//    {
//        job_req->SIE_PRE_CAP->SiePreAddr2 = NULL;
//    }

    if(job_req->SIE_PRE_CAP->SieFuncEn & SIE_SUBOUT_EN)
    {
        UINT32 sizeH,sizeV;
        if(ippbuf_GetBuf(IPPBUF_CA)==0)
        {
            job_req->SIE_PRE_CAP->SieFuncEn &= ~(SIE_SUBOUT_EN);
            debug_err(("no dram for ca out(cap),disable CA\r\n"));
        }
        job_req->SIE_PRE_CAP->SieSubParam.Hsub = 0;
        job_req->SIE_PRE_CAP->SieSubParam.Vsub = 0;
        //#NT#2011/01/24#ethanlau -begin
        //#NT#chk sie sub out size.must small than 320x240
        while(1)
        {
            sizeH = (((job_req->SIE_PRE_CAP->SieOutSizeH - 1)/(1<<job_req->SIE_PRE_PRV->SieSubParam.Hsub))>>2)<<2;
            sizeV = ((job_req->SIE_PRE_CAP->SieCropSizeV - 1)/(1<<job_req->SIE_PRE_PRV->SieSubParam.Vsub));
            if( (sizeH <= SIE_SubOut_H)&&(sizeV <= SIE_SubOut_V) )
            {
                break;
            }
            if(sizeH > SIE_SubOut_H)
                job_req->SIE_PRE_CAP->SieSubParam.Hsub++;
            if(sizeV > SIE_SubOut_V)
                job_req->SIE_PRE_CAP->SieSubParam.Vsub++;
        }
        //#NT#2011/01/24#ethanlau -end
        job_req->SIE_PRE_CAP->SieSubParam.Hcnt = sizeH;
        job_req->SIE_PRE_CAP->SieSubParam.Vcnt = sizeV;
        job_req->SIE_PRE_CAP->SieSubParam.SubUpdateMode = FRAME_UPDATE;
        job_req->SIE_PRE_CAP->SieSubParam.SubOutSrc = PATH_PRE_COLORGAIN;
        job_req->SIE_PRE_CAP->SieSubAddr = ippbuf_GetBuf(IPPBUF_CA);
        SetIPPSizeInfor(_CAHsize, sizeH);
        SetIPPSizeInfor(_CAVsize, sizeV);
        SetIPPSizeInfor(_CALineOffset, sizeH);
    }

    job_req->SIE_PRE_CAP->SieHBinParam.PixAdd = 0;
    job_req->SIE_PRE_CAP->SieHBinParam.PixDiv = 0;

}

void sie_D2DParameter(struct  PUBLIC_STR_JOBREQS *job_req)
{
    //CG and Ofs
    if(job_req->SIE_PRE_D2D->SieFuncEn & SIE_CG_EN)
    {
        UINT32 Rg,Gg,Bg;
        AWB_GetColorGian(&Rg,&Gg,&Bg);
        switch(job_req->SIE_PRE_D2D->SieCfaPat)
        {
            case IPE_PATR:
                job_req->sie_cg_info.Gain0 = Rg;
                job_req->sie_cg_info.Gain1 = Gg;
                job_req->sie_cg_info.Gain2 = Gg;
                job_req->sie_cg_info.Gain3 = Bg;
                break;
            case IPE_PATGR:
                job_req->sie_cg_info.Gain0 = Gg;
                job_req->sie_cg_info.Gain1 = Rg;
                job_req->sie_cg_info.Gain2 = Bg;
                job_req->sie_cg_info.Gain3 = Gg;
                break;
            case IPE_PATGB:
                job_req->sie_cg_info.Gain0 = Gg;
                job_req->sie_cg_info.Gain1 = Bg;
                job_req->sie_cg_info.Gain2 = Rg;
                job_req->sie_cg_info.Gain3 = Gg;
                break;
            case IPE_PATB:
                job_req->sie_cg_info.Gain0 = Bg;
                job_req->sie_cg_info.Gain1 = Gg;
                job_req->sie_cg_info.Gain2 = Gg;
                job_req->sie_cg_info.Gain3 = Rg;
                break;
        }
        job_req->sie_cofs_info.Ofs0 = 0;
        job_req->sie_cofs_info.Ofs1 = 0;
        job_req->sie_cofs_info.Ofs2 = 0;
        job_req->sie_cofs_info.Ofs3 = 0;
    }
    if (job_req->SIE_PRE_D2D->SieFuncEn & SIE_GAMMA_EN)
    {
        job_req->sra_gammaLUT = SRA_SIE_GammaLUT_Using;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void Sensor_Cap2Prv(void)
{
    SENSOR_STATUS SensorStatus;

    Sensor_GetStatus_Indicated(SIE_PRE_Para_prv.ModeSel, &SensorStatus);

    SetIPPSizeInfor(_SieXcenter, SensorStatus.pSyncInfo->Act_CenterX);
    SetIPPSizeInfor(_SieYcenter, SensorStatus.pSyncInfo->Act_CenterY);

    SenFlowFunc(SENSOR_MODE_SETTING);
    sie_Set2Preview(&SIE_PRE_Para_prv);
}

BOOL Check_FlashFire(void)
{
    if((gImageAlgInfo.FlashMode == _IMGFlashMode_ForceON) || (gImageAlgInfo.FlashMode == _IMGFlashMode_ForceONRedEye))
        return TRUE;
    else if(gImageAlgInfo.FlashMode == _IMGFlashMode_ForceOFF)
        return FALSE;
    else if((gImageAlgInfo.FlashMode == _IMGFlashMode_Auto) || (gImageAlgInfo.FlashMode == _IMGFlashMode_AutoRedEye))
    {
        return FALSE;
    }
    else
        return FALSE;
}

#if 0
#pragma mark -
#endif
static unsigned short RatioLog2( unsigned long Devider, unsigned long Devident )
{
    unsigned long Ratio;
    unsigned short idx,Ret;
    const UINT32 logtable[10] = {107,115,123,132,141,152,162,174,187,200};
    if(Devident==0)
        return 1;

    Ret = 0;
    Ratio = (Devider * 100) / Devident;
    while( Ratio >= 200 ) {
        Ratio /= 2;
        Ret += 10;
    }

    for( idx = 0; idx < 10; idx++ )
        if( Ratio < logtable[idx] )break;

    return Ret + idx;
}

void Write_AE_EXIF_Infor(UINT paddress)
{
    UINT32 iso,expt,iris;
    //static UINT32 fn[2] = {28,10};
    static UINT32 shutterspeed[2];
    static UINT32 exptime[2];
    static UINT32 focus[2] = {85,10};
    static INT32 expbias[2];
    UINT32 g_uiFNumber[2]      = {28, 10}; // F2.8
    UINT32 g_uiAperture[2]     = {30, 10}; // F2.8
    UINT32 g_uiMaxAperture[2]  = {30, 10}; // F2.8
    EXIF_MODIFYDATATAG  modifyDataTag;
    EXIF_MODIFYPTRTAG  modifyTag;
    AE_GetCapAEArg(&iso,&expt,&iris);
    if(gImageAlgInfo.ISO==_IMGISO_Auto)
    {
    }
    else if(gImageAlgInfo.ISO==_IMGISO_50)
    {
        expt=expt*iso/50;
        iso = 50;
    }
    else if(gImageAlgInfo.ISO==_IMGISO_100)
    {
        expt=expt*iso/100;
        iso = 100;
    }
    else if(gImageAlgInfo.ISO==_IMGISO_200)
    {
        expt=expt*iso/200;
        iso = 200;
    }
    else if(gImageAlgInfo.ISO==_IMGISO_400)
    {
        expt=expt*iso/400;
        iso = 400;
    }

    // ISO Speed Ratings (fixed temporary; need to modify)
    modifyDataTag.tagId = TagISOSpeedRatings;
    modifyDataTag.tagDataValue = (UINT32)iso;
    modifyDataTag.tagLen = TagISOSpeedLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    // Shutter Speed (fixed temporary; need to modify)
    if((expt) <= 1000000)
        shutterspeed[0]=RatioLog2(1000000,(expt));
    else
        shutterspeed[0]=-(RatioLog2((expt),1000000));
    shutterspeed[1] = 10;
    modifyTag.tagId = TagShutterSpeed;
    modifyTag.ptagDataAddr = (UINT32 *)shutterspeed;
    modifyTag.tagLen = TagShutterLen;
    ExifW_ModifyPtrTag(&modifyTag);

    // Exposure Time (fixed temporary; need to modify)
    if((expt) <= 100000)
    {
        exptime[1]=1000000/expt;
        exptime[0] = 1;
    }
    else
    {
        exptime[0]= expt/100000;
        exptime[1] = 10;
    }
    modifyTag.tagId = TagExposureTime;
    modifyTag.ptagDataAddr = (UINT32 *)exptime;
    modifyTag.tagLen = TagExpTimeLen;
    ExifW_ModifyPtrTag(&modifyTag);

    // F Number (fixed temporary; should get information from lens)
    modifyTag.tagId = TagFNumber;
    modifyTag.ptagDataAddr = (UINT32 *)g_uiFNumber;
    modifyTag.tagLen = TagFNumberLen;
    ExifW_ModifyPtrTag(&modifyTag);

    // Aperture (fixed temporary; should get information from lens)
    modifyTag.tagId = TagAperture;
    modifyTag.ptagDataAddr = (UINT32 *)g_uiAperture;
    modifyTag.tagLen = TagApertureLen;
    ExifW_ModifyPtrTag(&modifyTag);

    // Maximum Aperture (fixed temporary; should get information from lens)
    modifyTag.tagId = TagMaxAperture;
    modifyTag.ptagDataAddr = (UINT32 *)g_uiMaxAperture;
    modifyTag.tagLen = TagMaxApertureLen;
    ExifW_ModifyPtrTag(&modifyTag);

    // Focal Length (fixed temporary; should get information from lens)
    //g_uiFocalLength[0] = Lens_Focus_GetFL(iLensCurSection);
    //g_uiFocalLength[1] = 10;
    modifyTag.tagId = TagFocalLength;
    modifyTag.ptagDataAddr = (UINT32 *)focus;
    modifyTag.tagLen = TagFocalLenLen;
    ExifW_ModifyPtrTag(&modifyTag);

    // Exposure Bias,winston modify begin
    if(gImageAlgInfo.AEEVValue==_IMGEVValue_N2)
        expbias[0] = -6;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N5_3)
        expbias[0] = -5;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N4_3)
        expbias[0] = -4;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N1)
        expbias[0] = -3;
   else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N2_3)
        expbias[0] = -2;
   else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N1_3)
        expbias[0] = -1;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_00)
        expbias[0] = 0;
   else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P1_3)
        expbias[0] = 1;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P2_3)
        expbias[0] = 2;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P1)
        expbias[0] = 3;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P4_3)
        expbias[0] = 4;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P5_3)
        expbias[0] = 5;
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P2)
        expbias[0] = 6;

    expbias[1] = 3;
    // Exposure Bias,winston modify end
    //debug_msg("%d (%d,%d)\r\n",gImageAlgInfo.AEEVValue,expbias[0],expbias[1]);
    modifyTag.tagId = TagExposureBias;
    modifyTag.ptagDataAddr = (UINT32 *)expbias;
    modifyTag.tagLen = TagExpoBiasLen;
    ExifW_ModifyPtrTag(&modifyTag);
}

UINT32 WriteDebugData(unsigned char *Add)
{
    UH i;
    unsigned char *buff;
    UINT32 RstSize;
    char endl[2]={0X0D,0X0A};
    char endqty[2]={0XCC,0XBB};
    PImageAlgInfo pimginfo;
    CAL_DATA CalInfo;

    pimginfo = UI_GetParameter();
    buff = Add;
    *buff++= '\t';
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    //insert Qty tag
    buff += sprintf((char *)buff,"%s",endqty);

    buff += sprintf((char *)buff,"%s",endl);
    //version tag
    buff += sprintf((char *)buff,"%s\t IQ_Appendix_Date %s", endl, endl);
    buff += sprintf((char *)buff,"\t Version:     %s",endl);
    *buff++= '\t';
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);
    /**
        UI Setting
    */
    buff += sprintf((char *)buff,"UI CONFIG: %s",endl);
    buff += sprintf((char *)buff,"\t\tCommon:   %3d %3d %3d %3d %s",
        pimginfo->DriveMode,
        pimginfo->BurstDriveNum,
        pimginfo->Quality,
        pimginfo->SceneMode,
        endl
    );
    buff += sprintf((char *)buff,"\t\tAE:       %3d %3d %3d %3d %3d %s",
        pimginfo->AEEVValue,
        pimginfo->AEMode,
        pimginfo->AntiShakeLevel,
        pimginfo->Frequency,
        pimginfo->ISO,
        endl
    );
    buff += sprintf((char *)buff,"\t\tColor:    %3d %3d %3d %3d %s",
        pimginfo->WBMode,
        pimginfo->ImageEffect,
        pimginfo->Saturation,
        pimginfo->Sharpness,
        endl
    );
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);
    /**
    Calibration Msg
    */
    Cal_GetCalData(&CalInfo);
    buff += sprintf((char *)buff,"Calibration Info: %s",endl);
    buff += sprintf((char *)buff,"\t\t Status:   ISO:0x%x, AWB:0x%x, AWB_GS:0x%x, DP_PrvB:0x%x, DP_PrvW:0x%x, DP_CapB:0x%x, DP_CapW:0x%x, ECS:0x%x %s",
        CalInfo.CalStatus_ISO,
        CalInfo.CalStatus_AWB,
        CalInfo.CalStatus_AWBGS,
        CalInfo.CalStatus_PrvDP_B,
        CalInfo.CalStatus_PrvDP_W,
        CalInfo.CalStatus_CapDP_B,
        CalInfo.CalStatus_CapDP_W,
        CalInfo.CalStatus_ECS,
        endl
    );

    if(CalInfo.CalStatus_ISO == _ISO_Status)
    {
        buff += sprintf((char *)buff,"\t\t ISO:   %3d%s",
            CalInfo.Cal_ISO_Data[0],
            endl
        );
    }

    if(CalInfo.CalStatus_AWB == _AWB_Status)
    {
        buff += sprintf((char *)buff,"\t\t AWB:   %3d %3d%s",
            CalInfo.Cal_AWBGain.Rgain,
            CalInfo.Cal_AWBGain.Bgain,
            endl
        );
    }
    if(CalInfo.CalStatus_AWBGS == _AWB_Status)
    {
        buff += sprintf((char *)buff,"\t\t AWBGS:   %3d %3d%s",
            CalInfo.Cal_AWBGSGain.Rgain,
            CalInfo.Cal_AWBGSGain.Bgain,
            endl
        );
    }
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);
    /**
    ae msg
    */
    buff = (unsigned char *)AE_WriteDebugInfo((UINT32)buff);
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);
    /**
    awb msg
    */
    buff = (unsigned char *)AWB_WriteDebugInfo((UINT32)buff);
    {
        UINT32 Rg,Gg,Bg;
        AWB_GetColorGian(&Rg,&Gg,&Bg);
        buff += sprintf((char *)buff,"AWB Real Gain: %d %d%s",(UINT)Rg,(UINT)Bg, endl);
    }
    *buff++= '\t';
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"JPG Qvalue: %d%s",(UINT)Sen_GetJpgQinfo(), endl);

    buff += sprintf((char *)buff,"%s", endl);


    /**
    af msg
    */
    for(  i = 0; i < 80; i++ )
    {
        *buff++ = '-' ;
    }
    buff += sprintf((char *)buff,"%s", endl);

    buff = (unsigned char *)AF_WriteDebugInfo((UINT32)buff);

    buff += sprintf((char *)buff,"%s", endl);
    buff += sprintf((char *)buff,"%s", endl);
    RstSize = (UINT32)(buff-Add);
    if ( RstSize> CAP_DEBUGINFO_SIZE)
    {
        debug_err(("^R WriteDebugData() buffer overflow\r\n"));
    }
    return RstSize;
}

void Get_CapInfo(UINT InfoID, UINT *ReturnInfo)
{
    PImageAlgInfo _pimginfo;
    _pimginfo = UI_GetParameter();
     switch(InfoID)
    {
        case Cap_SyncMode:
            if(_pimginfo->DriveMode == _IMGDriveMode_Single)
            {
                *ReturnInfo = SENSOR_MODE_FULL;
            }
            else
            {
                *ReturnInfo = SENSOR_MODE_1280X960;
            }
            break;

        case Cap_ChromaParam:
            ReturnInfo = CHR_Param[0];
            break;

        case Cap_CFAPat:
            *ReturnInfo = Sen_GetIPECfaPat(*ReturnInfo, _pimginfo->ImageFlip);
            break;
        default:
            debug_err(("Err InfoID(%d) > MaxID(%d)\r\n", InfoID, Cap_MaxID));
        break;
     }
}

void AAFD_LockSwitch(UINT32 ImgMode)
{
    static BOOL bFdLock = TRUE;
#if (_CAF_FUNC_ == ENABLE)
    static BOOL bCafLock = TRUE;
#endif
    switch(ImgMode)
    {
        case IPPImg_Preview:
            aaa_AwbLock(UNLOCK);
            aaa_AeLock(UNLOCK);
            break;
        case IPPImg_Video:
        case IPPImg_PCC:
            aaa_AwbLock(UNLOCK);
            aaa_AeLock(UNLOCK);
            break;
        case IPPImg_AF:
            aaa_AwbLock(UNLOCK);
            aaa_AeLock(UNLOCK);
            fd_Lock(LOCK);
            break;
        case IPPImg_BeforCap:
            aaa_AwbLock(LOCK);
            aaa_AeLock(LOCK);
            fd_Lock(LOCK);
            sd_Lock(LOCK);
            break;

        case IPPImg_DzoomEnter:
#if (_CAF_FUNC_ == ENABLE)
            bCafLock = Is_CAFLock();
            aaa_CafLock(LOCK);
            if (bCafLock == UNLOCK)
            {
                aaa_CafLock(LOCK);
                caf_VAPprepare(FALSE);
                caf_setInitFlag(TRUE);
            }
#endif
            bFdLock = fd_IsLock();
            fd_Lock(LOCK);
            break;
        case IPPImg_DzoomLeave:
#if (_CAF_FUNC_ == ENABLE)
            aaa_CafLock(bCafLock);
#endif
            fd_Lock(bFdLock);
            break;
        default:
            debug_err(("-E- ImgMode(%d) > MaxID(%d)\r\n", ImgMode, IPPImg_MaxID));
        break;
    }

}

void SetSiePreCapParam(SIE_PRE_PARAM *SiePreInfo)
{
    PImageAlgInfo _pimginfo;
    SENSOR_STATUS SensorStatus;

    _pimginfo = UI_GetParameter();

    Sensor_GetStatus(&SensorStatus);

    SiePreInfo->SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2;

    SiePreInfo->SieBp1 = 10; // PRV_SIE_BP1
    SiePreInfo->SieBp2 = SensorStatus.pSyncInfo->Act_CenterY;

    SiePreInfo->SieMaster = SlaveMode;
    SiePreInfo->SieHDPhase = 0;
    SiePreInfo->SieVDPhase = 0;
    SiePreInfo->SieDataPhase= 0;

    SiePreInfo->AutoObEn = DISABLE;
    if (SiePreInfo->AutoObEn == DISABLE)
    {
        SetIPPSizeInfor(_OBOffset, SensorStatus.pSyncInfo->OB_Offset);
        SiePreInfo->SieObOfs = GetIPPSizeInfor(_OBOffset);
    }
    else
    {
        SetIPPSizeInfor(_OBOffset, 0x0);
        SiePreInfo->SieObOfs = GetIPPSizeInfor(_OBOffset);
        SiePreInfo->SieObParam.X0 = GetIPPSizeInfor(_OBWinX0);
        SiePreInfo->SieObParam.X1 = GetIPPSizeInfor(_OBWinX1);
        SiePreInfo->SieObParam.Y0 = GetIPPSizeInfor(_OBWinY0);
        SiePreInfo->SieObParam.Y1 = GetIPPSizeInfor(_OBWinY1);
        SiePreInfo->SieObParam.SubSample = GetIPPSizeInfor(_OBSubSample);
        SiePreInfo->SieObParam.Thres = GetIPPSizeInfor(_OBThreshold);
    }

    SiePreInfo->SieVDInv = Notinvert;
    SiePreInfo->SieHDInv = Invert;
    SiePreInfo->SiePathSel = PATH_POST_GAMMA;//PATH_PRE_COLORGAIN;
#if Get10BitRAW
    SiePreInfo->SiePathSel = PATH_PRE_COLORGAIN;
#endif
    SiePreInfo->SieOutModSel = SIE_FRAME_MODE;

    SiePreInfo->HDActX = GetIPPSizeInfor(_HDActX);
    SiePreInfo->HDActY = GetIPPSizeInfor(_HDActY);
    SiePreInfo->HDActSizeX = GetIPPSizeInfor(_HDActSizeX);
    SiePreInfo->HDActSizeY = GetIPPSizeInfor(_HDActSizeY);
    SiePreInfo->VDTotal = GetIPPSizeInfor(_VDTotal);
    SiePreInfo->VDSync = GetIPPSizeInfor(_VDSync);
    SiePreInfo->HDTotal = GetIPPSizeInfor(_HDTotal);
    SiePreInfo->HDSync = GetIPPSizeInfor(_HDSync);
    SiePreInfo->RawCenterX = GetIPPSizeInfor(_SieXcenter);
    SiePreInfo->RawCenterY = GetIPPSizeInfor(_SieYcenter);
    SiePreInfo->SieCropSizeH = GetIPPSizeInfor(_SieCropCapHsize);
    SiePreInfo->SieCropSizeV = GetIPPSizeInfor(_SieCropCapVsize);
    SiePreInfo->SieOutSizeH = GetIPPSizeInfor(_SieCropCapHsize);
    SiePreInfo->SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);

    SiePreInfo->MShutterTime = DEFAULT_SHUTTER_TIME;
    SiePreInfo->SieFuncEn = SIE_CG_EN|SIE_GAMMA_EN;

    if(_pimginfo->SieCapInBitDepth==_SieBitDepth_10)
    {
        SiePreInfo->SieInBitDepth = SIE_IN_PACK_10BIT;//SIE in
    }
    else if(_pimginfo->SieCapInBitDepth==_SieBitDepth_8)
    {
        SiePreInfo->SieInBitDepth = SIE_IN_PACK_8BIT;//SIE in
    }

    if (_pimginfo->SieCapBitDepth== _SieBitDepth_8)
    {
        SiePreInfo->SieBitDepth = SIE_OUT_PACK_8BIT;//SIE out
    }
    else if (_pimginfo->SieCapBitDepth == _SieBitDepth_10)
    {
        SiePreInfo->SieBitDepth = SIE_OUT_PACK_10BIT;//SIE out
    }
    else
    {
        debug_err(("Cap SieBitDepth error\r\n"));
    }

    SiePreInfo->SieCfaPat = SensorStatus.uiBayer;
}

UINT32 AeProcessEnd(UINT32 ISO, UINT32 ExposureTime)
{
    SetAEInfo(ISO,ExposureTime);
    SenFlowFunc(IFE_PRV_AE_SETTING);
    SenFlowFunc(IPE_PRV_AE_SETTING);
    return E_OK;
}

void IPPFunc_OnOffSwitch(void)
{
    //Integrated IPP function setting point, ON/OFF switch and parameters setting
    //DFS
    SetIPPFuncInfor(DFS_EN,OFF);

    //#NT#2011/01/15#ethanlau -begin
    //#NT#no following opperation on 96220,wait to remove
    SetIPPFuncInfor(NLIGHT_CAP_EN,OFF);
    SetIPPFuncInfor(NLIGHT_CAP_SEL,NLCap_QVDataIn);
    SetIPPFuncInfor(GEO_EN,OFF);
    SetIPPFuncInfor(YCURVE_EN,OFF);
    SetIPPFuncInfor(BRC_EN,OFF);
    //SetIPPFuncInfor(YCURVE,(UINT32)GammaLUT_Y_128Tab);
    //SetIPPFuncInfor(CHRCBCURVE,(UINT32)ChrCurveLUT);
    //SetIPPFuncInfor(CHRCRCURVE,(UINT32)ChrCurveLUT);
    //SetIPPFuncInfor(CAPGAMMACURVE,(UINT32)GammaLUT_128Tab);
    //#NT#2011/01/15#ethanlau -end

    //Cap Sie d2d
#if Get10BitRAW
    SetIPPFuncInfor(SIED2D_EN,ON);
    UI_SetParameter(_SieCapBitDepth, _SieBitDepth_10);
#else
    SetIPPFuncInfor(SIED2D_EN,OFF);
#endif
    //Chroma filter
    SetIPPFuncInfor(CHRFILT_EN,OFF);
    //Chroma curve
    SetIPPFuncInfor(CHRCURVE_EN,OFF);

    SetIPPFuncInfor(IFE_RAW_EN,*(BOOL*)GetIPPFuncInfor(IFE_FUNC_TABLE_ADDR));
    SetIPPFuncInfor(IFE_YCC_EN,*(BOOL*)(GetIPPFuncInfor(IFE_FUNC_TABLE_ADDR)+4));
    SetIPPFuncInfor(IFE_Y_EN, *(BOOL*)(GetIPPFuncInfor(IFE_FUNC_TABLE_ADDR)+8));
    SetIPPFuncInfor(IFE_EMBEDDED_EN,*(BOOL*)(GetIPPFuncInfor(IFE_FUNC_TABLE_ADDR)+12));

    //Cap flow
    SetIPPFuncInfor(FRAME_MODE_EN, OFF);

    /**
    Cap under slice mode, IFE EMBEDDED and YCC and Y can't enable at the same time
    */
    if(!GetIPPFuncInfor(FRAME_MODE_EN))
    {
        if(GetIPPFuncInfor(IFE_EMBEDDED_EN) && (GetIPPFuncInfor(IFE_YCC_EN)||GetIPPFuncInfor(IFE_Y_EN)))
        {
            debug_err(("^R IFE only supprot one opperation at one time(IFE_EMBEDDED_EN or IFE_YCC or IFE_Y)"));
            SetIPPFuncInfor(IFE_YCC_EN,OFF);
            SetIPPFuncInfor(IFE_Y_EN, OFF);
        }
    }

    SetIPPFuncInfor(ECS_EN, OFF);
    SetIPPFuncInfor(FQV_EN, ENABLE);
    //FQV use framemode if enable,otherwise, use jpg encode dcout.
    //above two condition must enable FQV_EN first
    SetIPPFuncInfor(FQV_FRAMEMDOE_EN, ENABLE);
}
#endif
