#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "pll.h"
#include "AlbumShot.h"
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ife_lib.h"
#include "ime_lib.h"
#include "IDE.h"
#include "JpgEnc.h"
#include "JpgDec.h"
#include "JpgParseHeader.h"
#include "Jpeg.h"
#include "Grph.h"
#include "filesystsk.h"
#include "CoupleShot.h"
#include "PreShot.h"
#include "ImageEffect.h"
#include "IPPCtrl.h"
#include "IPPManager.h"
#include "sen_common.h"
#include "Alg_IPLMode.h"
#include "SensorFlowFunc.h"
#include "SensorSys.h"
#include "Sensor.h"
#include "dzoom.h"
#include "datastamp.h"
#include "ipp_debug.h"
//static IME_FBADDR_PRAM IMEJPGAddrP1,IMEJPGAddrP2;
//#NT#2009/04/15#Harry Tsai -begin
//#NT#Used for GEO + 90 rotation
//UINT8 CfaMapping_Rot90[4] = {IPE_PATGR,IPE_PATB,IPE_PATR,IPE_PATGB};
//#NT#2009/04/15#Harry Tsai -end

//#NT#2009/05/26#YC Peng -begin
//#NT#Modify capture flip flow
//UINT8 CfaMapping_HorFlip[4] = {IPE_PATGR,IPE_PATR,IPE_PATB,IPE_PATGB};
//#NT#2009/05/26#YC Peng -end
//UINT (*ChromaPara)[28];
static void Sen_SetJpgQinfo(UINT32 Q);
#define CAP_DEBUG   ENABLE
#if (CAP_DEBUG)
#define CAP_MSG(...)    debug_msg ("CAP:"__VA_ARGS__)
#else
#define CAP_MSG(...)
#endif

void Exec_SIE_D2D(SIE_PRE_PARAM *SiePreInfo)
{
    IPPMgrReq.SIE_PRE_D2D = SiePreInfo;

    SIE_BeginIPP(&IPPMgrReq, SIE_D2D);
    SenFlowFunc(SIE_D2D_SETTING);
    SIE_AllocatResourceIPP(&IPPMgrReq, SIE_D2D);
    SIE_ExecIPP(&IPPMgrReq, SIE_D2D);

    if(gImageAlgInfo.FileFmt & _FileFmt_Raw)
    {
        CAP_MSG("Addr: 0X%x size:%d X %d \r\n",SiePreInfo->SieOutAdd0,SiePreInfo->SieOutLineOfs,SiePreInfo->SieCropSizeV);
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)SiePreInfo->SieOutAdd0,
                        SiePreInfo->SieCropSizeV * SiePreInfo->SieOutLineOfs,
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);
    }
}

void Set2SieCap_Normal(SIE_PRE_PARAM *SiePreInfo, UINT32 BufIdx)
{
    IMG_FILTER_PARAM ImgFilterInfo;
    //IPPBUF_ENG2DRAMINFO draminfo = {0};
    PImageAlgInfo pimginfo;
    pIFE_FILTER     pife_para = NULL;
    UINT32 ifeRepeat = 0;
    //#NTK#2012/06/13#Robin Chen begin 
    //support multi zoom section ECS Correction
    UINT32 zoomSection;
    zoomSection = Lens_Zoom_GetSection();
    //#NTK#2012/06/13#Robin Chen end
    pimginfo = UI_GetParameter();

#if 0
    //initial ImgEffect capture info
    if ( IE_GetCapStatus() == TRUE )
    {
        SetIPPFuncInfor(FQV_EN, DISABLE);
        IE_InitCaptureARF();
    }
#endif
    //SiePreInfo->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_SIERAW_1);
    //SiePreInfo->SieOutAdd1 = ippbuf_GetBuf(IPPBUF_CAP_SIERAW_2);
    if((BufIdx%3)==0)
    {
        SiePreInfo->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1 + BufIdx);
    }
    else if((BufIdx%3)==1)
    {
        SiePreInfo->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1 + BufIdx);
    }
    else //if((BufIdx%3)==2)
        SiePreInfo->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1 + BufIdx);

    //IFE raw overlap, BufAddr must shift left 1 word,lineoft add 1 word.
    if(GetIPPFuncInfor(IFE_RAW_EN))
    {
        UINT32 i=0;
        pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_RAW_TABLE_ADDR);
        for(i=0; pife_para[i].repeat != 0xFF; i++)
        {
            ifeRepeat += pife_para[i].repeat;
        }
        SiePreInfo->SieOutAdd0 += (IFE_OVERLAP_H * ifeRepeat);
    }

    SiePreInfo->SiePreAddr2 = ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME);
    //#NTK#2012/06/13#Robin Chen begin 
    //support multi zoom section ECS Correction
    SiePreInfo->SiePreAddr1 = ippbuf_GetBuf(IPPBUF_ECS)+(zoomSection-1)*ECS_TblSize;
    //#NTK#2012/06/13#Robin Chen end
    CaptureOne(SiePreInfo);
    //#NT#2010/04/23#Linkin Huang -begin
    //#NT# ipp debug
    #if IPP_DEBUG
    {
       char filename[64];

       sprintf(filename, "A:\\SIE.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_SIE);

       sprintf(filename, "A:\\PRE.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_PRE);
    }
    #endif
    //#NT#2010/04/23#Linkin Huang -end

    //#NT#2011/12/12#Ethan Lau -begin
    //#NT#move into SIED2D_EN
    /*
    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING);
    */
    //#NT#2011/12/12#Ethan Lau -end
    if (GetIPPFuncInfor(SIED2D_EN))
    {
        SIE_OPENOBJ SIEObjCB;
        extern void sie_isrCB(UINT32 IntStatus);
        SIEObjCB.FP_SIEISR_CB = sie_isrCB;
        sie_close();
        //#NT#2011/12/12#Ethan Lau -begin
        //CAP AWB parameter
        SenFlowFunc(AWB_CAP_SETTING);
        //#NT#2011/12/12#Ethan Lau -end
        memcpy(&SIE_PRE_Para_d2d,SiePreInfo,sizeof(SIE_PRE_PARAM));
        SIE_PRE_Para_d2d.SieObOfs = 0x0;//ethan_20110809
        SIE_PRE_Para_d2d.SiePreAddr0 = SiePreInfo->SieOutAdd0;
        SIE_PRE_Para_d2d.SieInBitDepth = SiePreInfo->SieBitDepth;
        SIE_PRE_Para_d2d.SieDramInLineOfs = SiePreInfo->SieOutLineOfs;
        SIE_PRE_Para_d2d.SieDramInSizeH = SiePreInfo->SieOutSizeH;
        SIE_PRE_Para_d2d.SieDramInSizeV = SiePreInfo->SieCropSizeV;
        //ethan_20110808_add sie output when input bit is 10bit
        SIE_PRE_Para_d2d.SieBitDepth = SIE_OUT_PACK_8BIT;
        SIE_PRE_Para_d2d.SieOutLineOfs = (SiePreInfo->SieOutLineOfs - ifeRepeat*IFE_OVERLAP_H)
                                            * _SieBitDepth_8/ pimginfo->SieCapBitDepth;
        SIE_PRE_Para_d2d.SieOutLineOfs += (ifeRepeat*IFE_OVERLAP_H);
        //#NT#2011/12/05#Ethan Lau -begin
        //#NT#mark,cause this will make 10bit d2d burst flow error
        //SetIPPSizeInfor(_SieOutLineOffset,SIE_PRE_Para_d2d.SieOutLineOfs);
        //SiePreInfo->SieBitDepth = SIE_OUT_PACK_8BIT;
        //SiePreInfo->SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);
        //SIE_PRE_Para_d2d.SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);
        //#NT#2011/12/05#Ethan Lau -end
        SIE_PRE_Para_d2d.SiePathSel = PATH_POST_GAMMA;
        SIE_PRE_Para_d2d.SieFuncEn = SIE_CG_EN|SIE_GAMMA_EN;
        SIE_PRE_Para_d2d.SieDRAMInEn = ENABLE;
        Exec_SIE_D2D(&SIE_PRE_Para_d2d);
        CAP_MSG("Sie D2D END\r\n");
        sie_open(&SIEObjCB);
        Sensor_Cap2Prv();
    }

    if (GetIPPFuncInfor(IFE_RAW_EN))
    {
        IFE_OPENOBJ     IFEOpenObj;
        //SenFlowFunc(IFE_RAW_SETTING);//already get

        ImgFilterInfo.Mode = IFE_RAW;
        ImgFilterInfo.ImgAddr = SiePreInfo->SieOutAdd0;
        ImgFilterInfo.HSize = SiePreInfo->SieCropSizeH;
        ImgFilterInfo.VSize = SiePreInfo->SieCropSizeV;
        //#NT#2011/12/12#Ethan Lau -begin
        //#NT#support 10bit input burst mode
        if(SiePreInfo->SieBitDepth == SIE_OUT_PACK_10BIT)
        {
            UINT32 tmp;
            tmp = (SiePreInfo->SieOutLineOfs - ifeRepeat*IFE_OVERLAP_H)
                                            * _SieBitDepth_8/ pimginfo->SieCapBitDepth;
            tmp += (ifeRepeat*IFE_OVERLAP_H);
            ImgFilterInfo.InLineOfs = tmp;
            ImgFilterInfo.OutLineOfs = tmp;
        }
        else
        {
            ImgFilterInfo.InLineOfs = SiePreInfo->SieOutLineOfs;
            ImgFilterInfo.OutLineOfs = SiePreInfo->SieOutLineOfs;
        }
        ImgFilterInfo.up_addr = 0;
        ImgFilterInfo.down_addr = 0;
        ImgFilterInfo.WaitEnd = ENABLE;
        //IFE d2d overlap, outaddr must shift left 1 word,lineoft add 1 word.
        ImgFilterInfo.BufAddr = ImgFilterInfo.ImgAddr - ifeRepeat*IFE_OVERLAP_H;
        ImgFilterInfo.pCB = NULL;
        IFEOpenObj.FP_IFEISR_CB = NULL;
        ife_open(&IFEOpenObj);
        ImageFilter(&ImgFilterInfo);
        ife_close();
        CAP_MSG("Ife RAW END\r\n");
        //IFE raw overlap, outaddr must shift left 1 word,lineoft add 1 word.
        SiePreInfo->SieOutAdd0 = ImgFilterInfo.BufAddr;
        if (gImageAlgInfo.FileFmt & _FileFmt_Raw)
        {
            CAP_MSG("Addr: 0X%x size:%d X %d \r\n",SiePreInfo->SieOutAdd0,SiePreInfo->SieOutLineOfs,SiePreInfo->SieCropSizeV);
            FilesysWaitCmdFinish(FST_TIME_INFINITE);
            FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                            (UB*)SiePreInfo->SieOutAdd0,
                            SiePreInfo->SieCropSizeV * SiePreInfo->SieOutLineOfs,
                            0,
                            1,
                            FST_FMT_RAW,
                            FST_TIME_INFINITE);
        }
    }
}


void Set2SieCap_AlbumShot(SIE_PRE_PARAM *SiePreInfo)
{
#if 0
    //CAP AE parameter
    SenFlowFunc(AE_CAP_SETTING);

    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_ALBUMSHOT);
    SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    SiePreInfo->SiePreAddr2 = ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME);
    CaptureOne(SiePreInfo);

    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING);
    if (ASGetCapCounter() == 0)
    {
        ASCapParamInit();
    }
    ASImgProcess(ASGetCapCounter());
#endif
}


void Set2SieCap_CoupleShot(SIE_PRE_PARAM *SiePreInfo)
{
#if 0
    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_COUPLESHOT);
    if(CSGetCapCounter()==COUPLESHOT_0)
    {
        CSParamInit();
        CSCopyPrvYCC();
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    }
    else if(CSGetCapCounter()==COUPLESHOT_1)
    {
        ide_disable_video(IDE_VIDEOID_2);
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_2);
    }

    //CAP AE parameter
    SenFlowFunc(AE_CAP_SETTING);

    //Capture RAW
    CaptureOne(SiePreInfo);

    //merge couple shot img1 to img0
    if(CSGetCapCounter()==COUPLESHOT_1)
    {
        CSMergeCapRAW();
    }

    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING);
#endif
}
void Set2SieCap_PreShot(SIE_PRE_PARAM *SiePreInfo)
{
#if 0
    //assign preshot buffer address
    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_PRESHOT);
    if(PSGetCapCounter()==PRESHOT_0)
    {
        PSParamInit();
        PSCopyPrvYCC();
    }
    else if(PSGetCapCounter()==PRESHOT_1)
    {
        ide_disable_video(IDE_VIDEOID_2);
    }
    SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);

    //CAP AE parameter
    SenFlowFunc(AE_CAP_SETTING);

    //Capture RAW
    CaptureOne(SiePreInfo);

    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING);
#endif
}

void Set2SieCap_Init(void)
{
    Set_SenInfo(IPPImg_BeforCap);
    SenFlowFunc(AAFD_Switch);

    if(g_fpAlgExport)
    {
        g_fpAlgExport((ALGMSG_SUSKEY), &gAlgMsg);
        g_fpAlgExport((ALGMSG_SUSFLOWPRV), &gAlgMsg);
        g_fpAlgExport((ALGMSG_SUSPHOTO), &gAlgMsg);
        //#NT#20090929#Bowen Li -begin
        //#NT#add new function for change clock
        //g_fpAlgExport(ALGMSG_FIRSTRAW, &gAlgMsg);
        //#NT#20090929#Bowen Li -end
    }
}

//#NT#2009/10/23#Bowen Li -begin
//#modify capture flow for new memory arrange
void Set2SieCap_SetParam(void)
{
    IPL_OBJ IplObj;
    UINT CapInfo;
    UINT32 OffsetValue, OffsetValue1;
    ImageAlgInfo *AlgInfo;
    UINT32 maxidx;
    SENSOR_STATUS SensorStatus;
    pIFE_FILTER     pife_para = NULL;
    //CAP AE parameter
    //ethan_20110805_chg_cap_flow,inorder to get ife repeat information first,than assign memory
    SenFlowFunc(AE_CAP_SETTING);
    SenFlowFunc(IFE_RAW_SETTING);
    SetIPPFuncInfor(PRIIMG_INPROCESS,ENABLE);
    SenFlowFunc(IFE_Y_SETTING);
    SenFlowFunc(IFE_YCC_SETTING);
    SetIPPFuncInfor(PRIIMG_INPROCESS,DISABLE);
    SenFlowFunc(IFE_FUNC_SETTING);
    SenFlowFunc(IPPFUNC_Switch);
    AlgInfo = UI_GetParameter();
    OffsetValue = 0;
    OffsetValue1 = 0;

    //#NT#2011/06/27#Chris Chung -begin
    //#NT#remove lens related code
    //Lens_OnOff(ON);
    //#NT#2011/06/27#Chris Chung -end

    //assign cap parameter
    SetIPPSizeInfor(_FQVSampleRate, FQV_SAMPLE_RATE);

    //SO far , NT96220 capture only support QV size == Screennail size
    //and not support QV_PRE (memory issue)
    SetIPPSizeInfor(_ThumbHsize, CAP_THUMB_H);
    SetIPPSizeInfor(_ThumbVsize, CAP_THUMB_V);
    SetIPPSizeInfor(_ThumbLineOffset, GetIPPSizeInfor(_ThumbHsize));
    SetIPPSizeInfor(_QVHsize, CAP_QV_H);
    SetIPPSizeInfor(_QVVsize, CAP_QV_V);
    SetIPPSizeInfor(_QVLineOffset, (GetIPPSizeInfor(_QVHsize) + (OffsetValue << 1)));
    SetIPPSizeInfor(_QVPreHsize, CAP_QV_PRE_H);
    SetIPPSizeInfor(_QVPreVsize, CAP_QV_PRE_V);
    SetIPPSizeInfor(_QVPreYLineOffset, (GetIPPSizeInfor(_QVPreHsize) + (OffsetValue << 1)));
    SetIPPSizeInfor(_QVPreUVLineOffset,GetIPPSizeInfor(_QVPreYLineOffset)/2);
    SetIPPSizeInfor(_ScreenHsize, CAP_SCREEN_H);
    SetIPPSizeInfor(_ScreenVsize, CAP_SCREEN_V);
    SetIPPSizeInfor(_ScreenLineOffset, GetIPPSizeInfor(_ScreenHsize));

    //#NT#2011/03/16#Ethan Lau -begin
    //#NT#move 2 here in order 2 get right dzoom tbl information, and right sensor info
    Get_CapInfo(Cap_SyncMode, &CapInfo);
    SetSensorMode(CapInfo);
    SIE_PRE_Para_cap.ModeSel = CapInfo;

//#NT#2011/08/01#Jarkko Chang -begin
    Sensor_GetStatus(&SensorStatus);
//    Set_DZoomInfor(SenMode2Tbl(CapInfo,&maxidx),_Ratio_4_3,AlgInfo->ImageRatio,AlgInfo->DispRatio);
    Set_DZoomInfor(SenMode2Tbl(CapInfo,&maxidx),SensorStatus.uiRatio,AlgInfo->ImageRatio,AlgInfo->DispRatio, AlgInfo->DsiplayFitWindow);
//#NT#2011/08/01#Jarkko Chang -end
    Set_DzoomMaxIndex(maxidx);

    Set_digitalzoom(Get_DzoomIndex(),AlgInfo->SieCapBitDepth);
    SetIPPSizeInfor(_IpeCapOutHsize,GetIPPSizeInfor(_IpeOutHsize));
    SetIPPSizeInfor(_IpeCapOutVsize,GetIPPSizeInfor(_IpeOutVsize));
    SetIPPSizeInfor(_SieCropCapHsize,GetIPPSizeInfor(_SieOutHsize));
    SetIPPSizeInfor(_SieCropCapVsize,GetIPPSizeInfor(_SieOutVsize));
    //#NT#2011/03/16#Ethan Lau -end
    SetIPPSizeInfor(_ImeCapOutHsize, GetIPPSizeInfor(_JpegCapHsize));
    SetIPPSizeInfor(_ImeCapOutVsize, GetIPPSizeInfor(_JpegCapVsize));
    SetIPPSizeInfor(_ImeCapOutYLineOffset, (GetIPPSizeInfor(_ImeCapOutHsize) + (OffsetValue << 1)));
    SetIPPSizeInfor(_ImeCapOutCCLineOffset, (GetIPPSizeInfor(_ImeCapOutYLineOffset)>>1));
    //IFE raw overlap, outaddr must shift left 1 word,lineoft add 1 word.
    if(GetIPPFuncInfor(IFE_RAW_EN))
    {
        UINT32 repeat=0,i=0;
        pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_RAW_TABLE_ADDR);
        for(i=0; pife_para[i].repeat != 0xFF; i++)
        {
            repeat += pife_para[i].repeat;
        }
        SetIPPSizeInfor(_SieOutLineOffset,(GetIPPSizeInfor(_SieOutLineOffset)+ repeat * IFE_OVERLAP_H) );
    }
    //frame mode IFE YCC buf overlap,so need to add 1 word to lineoft
    //slice mode IFE have it own tmp buf
    if(GetIPPFuncInfor(FRAME_MODE_EN))
    {
        if(GetIPPFuncInfor(IFE_Y_EN))
        {
            SetIPPSizeInfor(_ImeCapOutYLineOffset, GetIPPSizeInfor(_ImeCapOutYLineOffset) + IFE_OVERLAP_H);
        }
        if(GetIPPFuncInfor(IFE_YCC_EN))
        {
            SetIPPSizeInfor(_ImeCapOutCCLineOffset, GetIPPSizeInfor(_ImeCapOutCCLineOffset) + IFE_OVERLAP_H);
        }
    }
    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_CAP;
    IplObj.uiImeOutWidth1 = GetIPPSizeInfor(_ImeCapOutHsize);
    IplObj.uiImeOutHeight1 = GetIPPSizeInfor(_ImeCapOutVsize);
    IplObj.uiImeOutYoft1 = GetIPPSizeInfor(_ImeCapOutYLineOffset);
    IplObj.uiImeOutUVoft1 = GetIPPSizeInfor(_ImeCapOutCCLineOffset);
    if(!GetIPPFuncInfor(FQV_FRAMEMDOE_EN))
    {
        JPGLIB_DC_OUT_CFG  DCOutCfg = {0};

        DCOutCfg.FrameWidth = GetIPPSizeInfor(_ImeCapOutHsize);
        DCOutCfg.FrameHeight = GetIPPSizeInfor(_ImeCapOutVsize);
        DCOutCfg.PanelWidth = GetIPPSizeInfor(_QVHsize);
        DCOutCfg.PanelHeight = GetIPPSizeInfor(_QVVsize);
        JpegGetDCOutConfig(&DCOutCfg);
        SetIPPSizeInfor(_QVPreHsize, DCOutCfg.DCWidth);
        SetIPPSizeInfor(_QVPreVsize, DCOutCfg.DCHeight);
        if(DCOutCfg.DCULineOffset != DCOutCfg.DCVLineOffset)
        {
            debug_err(("%s: dcout U lineoft != V lineoft\r\n",__func__));
        }
        SetIPPSizeInfor(_QVPreYLineOffset,DCOutCfg.DCYLineOffset);
        SetIPPSizeInfor(_QVPreUVLineOffset,DCOutCfg.DCULineOffset);
    }
    IplObj.uiImeOutWidth2 = GetIPPSizeInfor(_QVHsize);
    IplObj.uiImeOutHeight2 = GetIPPSizeInfor(_QVVsize);
    IplObj.uiImeOutYoft2 = GetIPPSizeInfor(_QVLineOffset);
    IplObj.uiImeOutUVoft2 = (GetIPPSizeInfor(_QVLineOffset)>>1);
    //IplObj.uiImgFmt = NULL;//move to alg_ipl_xxx.c to choose fmt,in oreder to maintain common file consistancy
    IPL_SetCmd(&IplObj);

    SetIPPSizeInfor(_ImePath1Fmt,Get_IME_OutInfo(IPP_SliceMode_F2S,ImeOutInfo_FMT));
    SetIPPSizeInfor(_ImePath2Fmt,Get_IME_OutInfo(IPP_FrameMode_FW,ImeOutInfo_FMT));
    SetIPPSizeInfor(_IMESliceModeH, Get_IME_OutInfo(IPP_SliceMode_F2S,ImeOutInfo_SliceH));
    //SetIPPSizeInfor(_DceAddrOffset, OffsetValue);
    //SetIPPSizeInfor(_PreD2DOffsetLine, OffsetValue1);

    SetSiePreCapParam(&SIE_PRE_Para_cap);
    SenFlowFunc(PREFLASH_FLOW);
}
//#NT#2009/10/23#Bowen Li -end

void Sen_GetFQVRaw(SIE_PRE_PARAM *SiePreParam)
{
#if 0
    UINT32 FieldV;
    FieldV = ((SiePreParam->SieCropSizeV + GetIPPSizeInfor(_FQVSampleRate) - 1) / GetIPPSizeInfor(_FQVSampleRate));
    FieldV = FieldV / 4 * 4;
    grph_open();
    grph_enableDMA();
    grph_setImg1(SiePreParam->SieOutAdd0,
                    (SiePreParam->SieCropSizeH * GetIPPSizeInfor(_FQVSampleRate) * gImageAlgInfo.SieCapBitDepth >> 3),
                    FieldV, (SiePreParam->SieCropSizeH * gImageAlgInfo.SieCapBitDepth >> 3));
    grph_setImg2(ippbuf_GetBuf(IPPBUF_CAP_QV_BUF), (SiePreParam->SieCropSizeH * gImageAlgInfo.SieCapBitDepth >> 3));
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    grph_disableDMA();
    grph_swReset();
    grph_close();
#endif
}


void Set2SieCap(ImageAlgInfo *AlgInfo, UINT32 BufIdx)
{
    switch(AlgInfo->DriveMode)
    {
        //normal mode(single, burst, continue......)
        case _IMGDriveMode_BurstFlash:
        case _IMGDriveMode_Burst:
        case _IMGDriveMode_InfBurst:
        case _IMGDriveMode_SmallBurst16:
        case _IMGDriveMode_AEBreaketing:
        case _IMGDriveMode_AWBBreaketing:
        case _IMGDriveMode_Single:
            Set2SieCap_Normal(&SIE_PRE_Para_cap, BufIdx);
            break;

        case _IMGDriveMode_PreShot:
            SetIPPFuncInfor(FQV_EN, DISABLE);
            Set2SieCap_PreShot(&SIE_PRE_Para_cap);
            break;

        case _IMGDriveMode_CoupleShot:
            SetIPPFuncInfor(FQV_EN, DISABLE);
            Set2SieCap_CoupleShot(&SIE_PRE_Para_cap);
            break;

        case _IMGDriveMode_AlbumShot:
            SetIPPFuncInfor(FQV_EN, DISABLE);
            Set2SieCap_AlbumShot(&SIE_PRE_Para_cap);
            break;

        default:
            debug_err(("-E- AlgInfo->DriveMode = %d\r\n", AlgInfo->DriveMode));
            break;
    }
}

ER SliceMode_EventHandle(UINT32 uiJpgStatus, UINT32 uiImeStatus)
{
    if (uiJpgStatus & _JPEG_Buf_End)
    {
        debug_err(("%s: JPG BUFFER END\r\n",__func__));
        while((uiImeStatus & IME_INT_FRM) == 0)
        {
            ime_triggerSlice();
            uiImeStatus = ime_waitDone();
        }
        return E_BOVR;
    }
    return E_OK;
}

void DataCopy(UINT32 addr,UINT32 lnoft,UINT32 v,UINT32 Fmt)
{
    static UINT32 addroffset= 0;
    static UINT32 Cnt=0;
    grph_open();
    grph_init(PLL_CLKSEL_GRAPHIC_160);
    grph_enableDMA();
    switch(Fmt)
    {
        case IFE_Y:
            grph_setImg1(   addr,
                            lnoft,
                            v,
                            lnoft);
            grph_setImg2(   ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_Y),
                            lnoft);
            grph_setAOP(    0,  GRPH_DST_2, GRPH_AOP_A_COPY,    0);
            break;
        case IFE_YCC://YCC sequence must be Cb Cr Cb Cr ....
            grph_setImg1(   addr,
                            lnoft,
                            v,
                            lnoft);
            grph_setImg2(   ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_CC) + addroffset,
                            lnoft);
            grph_setAOP(    0,  GRPH_DST_2, GRPH_AOP_A_COPY,    0);
            if(Cnt%2 == 0)
                addroffset += lnoft*v;
            else
                addroffset = 0;
            Cnt++;
            break;
         default:
            break;
    }
    grph_disableDMA();
    grph_swReset();
    grph_close();
}
//fill ime output buf addr and ime output format
static void GetIMEJPGAddr(UINT StartAddr,IME_FBADDR_PRAM* TmpAddr,UINT IME_OutHeight,UINT IME_OutYlineoft,UINT IME_OutCClineoft,UB OutFormat,UB OPPBnum)
{
    UINT Slc_YSize,Slc_CSize = 0;

    Slc_YSize = IME_OutYlineoft*IME_OutHeight;
    Slc_CSize = IME_OutCClineoft*IME_OutHeight;
    switch(OutFormat)
    {
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            Slc_CSize/=2;
            break;
        default:
            debug_err(("Cap ime out fmt error(%d)\r\n",OutFormat));
        case IME_OUTPUT_YCC_444:
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
            break;
    }
    switch(OPPBnum)
    {
        case IME_OUTPUT_PPB_1:
            TmpAddr->y0_addr= StartAddr;
            TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
            TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;
            break;
        case IME_OUTPUT_PPB_3:
            TmpAddr->y0_addr= StartAddr;
            TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
            TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;

            TmpAddr->y1_addr= TmpAddr->cr0_addr+Slc_CSize;
            TmpAddr->cb1_addr=TmpAddr->y1_addr+Slc_YSize;
            TmpAddr->cr1_addr=TmpAddr->cb1_addr+Slc_CSize;

            TmpAddr->y2_addr= TmpAddr->cr1_addr+Slc_CSize;
            TmpAddr->cb2_addr=TmpAddr->y2_addr+Slc_YSize;
            TmpAddr->cr2_addr=TmpAddr->cb2_addr+Slc_CSize;
            break;
        case IME_OUTPUT_PPB_4:
            TmpAddr->y0_addr= StartAddr;
            TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
            TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;

            TmpAddr->y1_addr= TmpAddr->cr0_addr+Slc_CSize;
            TmpAddr->cb1_addr=TmpAddr->y1_addr+Slc_YSize;
            TmpAddr->cr1_addr=TmpAddr->cb1_addr+Slc_CSize;

            TmpAddr->y2_addr= TmpAddr->cr1_addr+Slc_CSize;
            TmpAddr->cb2_addr=TmpAddr->y2_addr+Slc_YSize;
            TmpAddr->cr2_addr=TmpAddr->cb2_addr+Slc_CSize;

            TmpAddr->y3_addr= TmpAddr->cr2_addr+Slc_CSize;
            TmpAddr->cb3_addr=TmpAddr->y3_addr+Slc_YSize;
            TmpAddr->cr3_addr=TmpAddr->cb3_addr+Slc_CSize;
            break;
        case IME_OUTPUT_PPB_5:
            TmpAddr->y0_addr= StartAddr;
            TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
            TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;

            TmpAddr->y1_addr= TmpAddr->cr0_addr+Slc_CSize;
            TmpAddr->cb1_addr=TmpAddr->y1_addr+Slc_YSize;
            TmpAddr->cr1_addr=TmpAddr->cb1_addr+Slc_CSize;

            TmpAddr->y2_addr= TmpAddr->cr1_addr+Slc_CSize;
            TmpAddr->cb2_addr=TmpAddr->y2_addr+Slc_YSize;
            TmpAddr->cr2_addr=TmpAddr->cb2_addr+Slc_CSize;

            TmpAddr->y3_addr= TmpAddr->cr2_addr+Slc_CSize;
            TmpAddr->cb3_addr=TmpAddr->y3_addr+Slc_YSize;
            TmpAddr->cr3_addr=TmpAddr->cb3_addr+Slc_CSize;

            TmpAddr->y4_addr= TmpAddr->cr3_addr+Slc_CSize;
            TmpAddr->cb4_addr=TmpAddr->y4_addr+Slc_YSize;
            TmpAddr->cr4_addr=TmpAddr->cb4_addr+Slc_CSize;
            break;
        default:
            debug_err(("%s:ime not support this pingpong num so far!\r\n",__func__));
            break;
    }
    TmpAddr->out_format=OutFormat;
}

static __inline void CloseIPPEng()
{
    ime_pause();
    ipe_pause();
    ipe_reset(ON);
    ipe_reset(OFF);
    ime_setReset();
    ipe_close();
    ife_close();
    ime_close();
}

//#NT#2011/01/21#ethanlau -begin
//#NT#ife postproc must trigger after ime have more than 1 slicedone
//(first ime slice done have no upside overlap,last slice done have no downside overlap)
#define IFE_POSTPROC_START 1
//#NT#2011/01/21#ethanlau -end
/**
Only support Slice mode sequential flow, IFE must wait frame end
*/
#define IFE_WAITFMD ENABLE
ER SliceMode_Process(UINT BayerAddr, UINT JpgDAddr, UINT8 JpgQuality, UINT *JpgSize)
{
    UINT JpgYAddr, JpgCbAddr, JpgCrAddr, JpgFmt = 0;
    UINT32 _IfeAddr,_IfeUAddr,_IfeDAddr;
    UINT SliceHeight, ProcHeight;
    UINT32 Ife_Cnt = 0,Ime_Counter,StampCnt,CounterMax,_IfeBufID=0;
    UINT32 uiImeStatus = 0 ,uiJpgStatus = 0;
    //UINT32  ChrFiltIdx;
    UINT32 DataStampVStartPxl,DataStampHStartPxl,DataStampVDoBlkStartPxl,DataStampVDoBlk, DataStampVRemain, DataStampVPartialSize;
    UINT32 DataStampXPos, DataStampYPos;
    DS_STAMP_INFOR dsStamp, dsStampTmp;
    DS_BACKGROUND_INFOR dsBackgroundTmp;
    //IMEStripeInfo ImeStripe;
    IMG_FILTER_PARAM ImgFilterInfo;
    //PRE_YCC_D2D_PARAM PreD2DInfo;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;
    IFE_OPENOBJ IFEOpenObj;
    UINT IPECFAPat = SIE_PRE_Para_cap.SieCfaPat;
    IPEStripeInfo IpeHStripe,IpeVStripe;
    BOOL bPostProc = FALSE;
    UINT32 _Shift,tmp = 0,ifeReT=0;

    pIFE_FILTER pife_para =  NULL;
    IPEOpenObj.FP_IPEISR_CB = NULL;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_96;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IFEOpenObj.FP_IFEISR_CB = NULL;
    ipe_open(&IPEOpenObj);
    ime_open(&IMEOpenObj);
    ife_open(&IFEOpenObj);
    //reset ime & jpg
    //calculate slice height & slice number
    Ime_Counter = 0;
    SliceHeight = GetIPPSizeInfor(_IMESliceModeH);

    //initial datastamp parameters
    datastamp_GetStampInfor(&dsStamp);
    datastamp_GetPosition(&DataStampXPos, &DataStampYPos);
    DataStampHStartPxl = (GetIPPSizeInfor(_ImeCapOutHsize)*DataStampXPos)>>StampStartRatioShift;
    DataStampHStartPxl &= 0xfffffff8;//needed to be word-aligned for Cb,Cr
    DataStampVStartPxl = (GetIPPSizeInfor(_ImeCapOutVsize)*DataStampYPos)>>StampStartRatioShift;
    DataStampVStartPxl &= 0xFFFFFFFE;//must be even
    DataStampVDoBlk = (DataStampVStartPxl)/SliceHeight;
    DataStampVRemain = dsStamp.Height;
    DataStampVDoBlkStartPxl = DataStampVStartPxl%SliceHeight;

//#NT#2011/01/10#ethanlau -end
    //initial slice parameters
    GetIMEExtInfo(&IME_D1_Para_ext,&IME_D2_Para_ext,CAPTURE_JOB);
    GetIMEJPGAddr(ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF),&IME_D1_Para,IME_D1_Para_ext.p1_out_slice_height,
        GetIPPSizeInfor(_ImeCapOutYLineOffset),GetIPPSizeInfor(_ImeCapOutCCLineOffset),GetIPPSizeInfor(_ImePath1Fmt),IME_D1_Para_ext.Ime_BuffNum);

    IME_D1_Para.in_h=GetIPPSizeInfor(_SieCropCapHsize)-16;
    IME_D1_Para.in_v=GetIPPSizeInfor(_SieCropCapVsize);
    IME_D1_Para.in_lineoffset = IME_D1_Para.in_h;
    IME_D1_Para.in_Cb_lineoffset = IME_D1_Para.in_lineoffset/2;
    IME_D1_Para.in_Cr_lineoffset = IME_D1_Para.in_lineoffset/2;
    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeCapOutHsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeCapOutVsize);

    IME_D1_Para.in_format=IMEIN_FMT444;//direct path ipe always out 444 fmt
    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeCapOutHsize);
    IME_D1_Para.out_Cb_lineoffset=GetIPPSizeInfor(_ImeCapOutHsize)/2;
    IME_D1_Para.out_Cr_lineoffset=GetIPPSizeInfor(_ImeCapOutHsize)/2;

    {
        SliceSizeInfo _OutInfo;
        SliceGenIn _InInfo;
        //generator slice in/out size of each engine
        _InInfo.ImeOutH = IME_D1_Para.out_h;
        _InInfo.ImeOutV = IME_D1_Para.out_v;
        _InInfo.IpeInH = GetIPPSizeInfor(_SieCropCapHsize);
        _InInfo.IpeInV = GetIPPSizeInfor(_SieCropCapVsize);
        _InInfo.ImeSliceH = GetIPPSizeInfor(_IMESliceModeH);
        _InInfo.Sie_SliceH_Default = GetIPPSizeInfor(_SieCropCapVsize)/3;
        _InInfo.IpeMode = IPE_AUTOSTP;
        _InInfo.ImeMode = IPE2IME_SLICE_MODE;
        SliceSize_Gen(&_OutInfo,_InInfo);
        IME_D1_Para_ext.ImeStripe.Ime_StripeMode = IME_STRIPE_USER_MODE;

        IpeHStripe.stripe.n = _OutInfo.Ipe_Hstripe.n;
        IpeHStripe.stripe.l = _OutInfo.Ipe_Hstripe.l;
        IpeHStripe.stripe.m = _OutInfo.Ipe_Hstripe.m;
        IpeHStripe.olap=IPE_OLAP_ON;
        IpeVStripe.stripe.n = _OutInfo.Ipe_Vstripe.n;
        IpeVStripe.stripe.l = _OutInfo.Ipe_Vstripe.l;
        IpeVStripe.stripe.m = _OutInfo.Ipe_Vstripe.m;
        IpeVStripe.olap=IPE_OLAP_ON;

        IME_D1_Para_ext.ImeStripe.Hstripe.n = _OutInfo.Ime_Hstripe.n;
        IME_D1_Para_ext.ImeStripe.Hstripe.l = _OutInfo.Ime_Hstripe.l;
        IME_D1_Para_ext.ImeStripe.Hstripe.m = _OutInfo.Ime_Hstripe.m;

        IME_D1_Para_ext.ImeStripe.Vstripe.n = _OutInfo.Ime_Vstripe.n;
        IME_D1_Para_ext.ImeStripe.Vstripe.l = _OutInfo.Ime_Vstripe.l;
        IME_D1_Para_ext.ImeStripe.Vstripe.m = _OutInfo.Ime_Vstripe.m;
    }

    ime_SliceCfg(&IME_D1_Para,&IME_D2_Para,&IME_D1_Para_ext,&IME_D2_Para_ext,CAPTURE_JOB);
    CounterMax = ime_getEngineInfo(IME_GET_OPERATED_SLICE_NUM);
    Get_CapInfo(Cap_CFAPat, &IPECFAPat);
    //#NT#2011/01/10#ethanlau -begin
    //#NT#remove, move to ime iq param
    //ChrFiltIdx = GetIPPFuncInfor(CAP_ISO);
    //ChromaPara = (UINT(*)[28])Get_ChromaInfo();
    //SetIPPFuncInfor(CHRFILT_NUM, ChromaPara[ChrFiltIdx][27]);
    //#NT#2011/01/10#ethanlau -end

    if(GetIPPFuncInfor(IFE_EMBEDDED_EN))
        ife_EmbeddedInIpeCfg(CAPTURE_JOB,IPECFAPat);
    ipe_SliceCfg(&IpeHStripe,&IpeVStripe,BayerAddr,IPECFAPat, CAPTURE_JOB);

    switch(IME_D1_Para.out_format)
    {
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
            _Shift = 2;
            JpgFmt = JPGFormatYUV422;
                break;
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            _Shift = 4;
            JpgFmt = JPGFormatYUV420;
                break;
        default:
            JpgFmt = JPGFormatYUV422;
            _Shift = 2;
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }

    //cfg stamp fmt if stamp fmt != img fmt
    if(datastamp_IsEnable())
        datastamp_ChkStampFmt(IME_D1_Para.out_format,&dsStamp,ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF),1);

    //JPG ENG Slice mode CFG
    {
        UINT        JPG_BSize;
        JPGCmpParam    JPGParam = {0};
        char        ER_Code;

        //Address will be setted in 1st jpeg trigger
        //JPGParam.RawImageAddr[0] = IME_D1_Para.y0_addr;            //Raw Image address Y
        //JPGParam.RawImageAddr[1] = IME_D1_Para.cb0_addr;           //Raw Image address CB
        //JPGParam.RawImageAddr[2] = IME_D1_Para.cr0_addr;           //Raw Image address CR
        JPGParam.JPGBSAddr = JpgDAddr;             //Compress Bitstream location,
        JPG_BSize = ippbuf_GetBuf(IPPBUF_CAP_JPG_BTSSIZE);
        JPGParam.JPGBSBufSize = &JPG_BSize;
        JPGParam.Width = GetIPPSizeInfor(_ImeCapOutHsize);
        JPGParam.Height = GetIPPSizeInfor(_ImeCapOutVsize);
        JPGParam.LineOffsetY = GetIPPSizeInfor(_ImeCapOutYLineOffset);
        JPGParam.LineOffsetU = GetIPPSizeInfor(_ImeCapOutCCLineOffset);
        JPGParam.LineOffsetV = GetIPPSizeInfor(_ImeCapOutCCLineOffset);
        //JPGParam.LineOffsetCbCr = GetIPPSizeInfor(_ImeCapOutCCLineOffset);
        JPGParam.Mode  = JpgFmt | JPG_Enc_SrcSliceMode;
        if(GetIPPFuncInfor(FQV_EN))
        {
            if(!GetIPPFuncInfor(FQV_FRAMEMDOE_EN))
            {
                //get dcout ratio and size
                JPGParam.DCOutCfg.FrameWidth = GetIPPSizeInfor(_ImeCapOutHsize);
                JPGParam.DCOutCfg.FrameHeight = GetIPPSizeInfor(_ImeCapOutVsize);
                JPGParam.DCOutCfg.PanelWidth = GetIPPSizeInfor(_QVPreHsize);
                JPGParam.DCOutCfg.PanelHeight = GetIPPSizeInfor(_QVPreVsize);
                JpegGetDCOutConfig(&JPGParam.DCOutCfg);

                JPGParam.DCOutCfg.DCEnable = TRUE;
                JPGParam.DCOutCfg.YUVFormat = JpgFmt;
                //SetIPPSizeInfor(_QVPreYLineOffset,JPGParam.DCOutCfg.DCYLineOffset);
                //SetIPPSizeInfor(_QVPreUVLineOffset,JPGParam.DCOutCfg.DCULineOffset);
                JPGParam.DCOutCfg.DCYAddr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
                JPGParam.DCOutCfg.DCUAddr = JPGParam.DCOutCfg.DCYAddr + GetIPPSizeInfor(_QVPreYLineOffset) * GetIPPSizeInfor(_QVPreVsize);
                JPGParam.DCOutCfg.DCVAddr = JPGParam.DCOutCfg.DCUAddr + GetIPPSizeInfor(_QVPreUVLineOffset) * GetIPPSizeInfor(_QVPreVsize)/(_Shift/2);
            }
        }
        Sen_SetJpgQinfo(JpgQuality);
        ER_Code = JpegEncOnePic(&JPGParam, JpgQuality);
        if(ER_Code != E_OK)
        {
            debug_err(("%s: JPG ENG CFG error\r\n",__func__));
        }
    }

    if(GetIPPFuncInfor(IFE_Y_EN) || GetIPPFuncInfor(IFE_YCC_EN))
    {
        bPostProc = TRUE;
        //let for loop run 1 more time for ife to proc. last ime slice data
        CounterMax++;

        if(IME_D1_Para_ext.Ime_BuffNum < IME_OUTPUT_PPB_4)
        {
            debug_err(("IFE post process under slice mode must have more than(equal to) 4 ime slice buffer"));
            SetIPPFuncInfor(IFE_Y_EN,DISABLE);
            SetIPPFuncInfor(IFE_YCC_EN,DISABLE);
            CounterMax --;
            bPostProc = FALSE;
        }
    }
    ime_setStart();
    ipe_start();
    #if IPP_DEBUG
    {
       char filename[64];

       sprintf(filename, "A:\\CAP_SLICE_IPE.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_IPE);

       sprintf(filename, "A:\\CAP_SLICE_IME.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_IME);
    }
    #endif
    for (Ime_Counter = 0; Ime_Counter < CounterMax; Ime_Counter ++)
    {
        /************* step 1 wait ime slice end *************/
        CAP_MSG(" %d",Ime_Counter);
        while(1)
        {
            if (uiImeStatus & IME_INT_FRM)//once ime have frame end,than no need to wait anything
            {
                break;
            }
            uiImeStatus = ime_waitDone();
            if (uiImeStatus & IME_INT_FRM)
            {
                break;
            }
            if (uiImeStatus & IME_INT_SLICE_DN1)
            {
                break;
            }
        }
        //Jpg Eng get ime output address
        switch(Ime_Counter % (IME_D1_Para_ext.Ime_BuffNum+1))
        {
            case 0: JpgYAddr=IME_D1_Para.y0_addr;JpgCbAddr=IME_D1_Para.cb0_addr;JpgCrAddr=IME_D1_Para.cr0_addr;
                break;
            case 1: JpgYAddr=IME_D1_Para.y1_addr;JpgCbAddr=IME_D1_Para.cb1_addr;JpgCrAddr=IME_D1_Para.cr1_addr;
                break;
            case 2: JpgYAddr=IME_D1_Para.y2_addr;JpgCbAddr=IME_D1_Para.cb2_addr;JpgCrAddr=IME_D1_Para.cr2_addr;
                break;
            case 3: JpgYAddr=IME_D1_Para.y3_addr;JpgCbAddr=IME_D1_Para.cb3_addr;JpgCrAddr=IME_D1_Para.cr3_addr;
                break;
            case 4: JpgYAddr=IME_D1_Para.y4_addr;JpgCbAddr=IME_D1_Para.cb4_addr;JpgCrAddr=IME_D1_Para.cr4_addr;
                break;
            default:    JpgYAddr = JpgCbAddr = JpgCrAddr = 0;
                break;
        }

        /************* step 2 post processer *************/
        //IFE filter trigger idx
        if( (Ime_Counter >= IFE_POSTPROC_START)&&((GetIPPFuncInfor(IFE_Y_EN))||(GetIPPFuncInfor(IFE_YCC_EN))))
        {
            CAP_MSG("_%d ",Ife_Cnt);
            ProcHeight = SliceHeight;
            //last ife proc.
            if(Ife_Cnt == (CounterMax-2))//deduct 2 is because cntmax have been ++ when postporc. is enabled
            {
                //last ime slice height(valid data height)
                if((((CounterMax-1) * SliceHeight) -  GetIPPSizeInfor(_ImeCapOutVsize)) > 0 )
                {
                    ProcHeight = GetIPPSizeInfor(_ImeCapOutVsize) - (Ife_Cnt * SliceHeight);
                }
            }

            //Y
            if(GetIPPFuncInfor(IFE_Y_EN))
            {
                pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_Y_TABLE_ADDR);
                //ife current buffid,get ime previous output addr
                tmp = Ime_Counter%(IME_D1_Para_ext.Ime_BuffNum+1);
                _IfeBufID = ((tmp>0)?(tmp-1):IME_D1_Para_ext.Ime_BuffNum);
                switch(_IfeBufID)
                {
                    case 0: _IfeAddr=IME_D1_Para.y0_addr;//current addr
                            _IfeUAddr=((IME_D1_Para_ext.Ime_BuffNum==IME_OUTPUT_PPB_5)?(IME_D1_Para.y4_addr):(IME_D1_Para.y3_addr));//up addr
                            _IfeDAddr=IME_D1_Para.y1_addr;//down addr
                        break;
                    case 1: _IfeAddr=IME_D1_Para.y1_addr;//current addr
                            _IfeUAddr=IME_D1_Para.y0_addr;//up addr
                            _IfeDAddr=IME_D1_Para.y2_addr;//down addr
                        break;
                    case 2: _IfeAddr=IME_D1_Para.y2_addr;//current addr
                            _IfeUAddr=IME_D1_Para.y1_addr;//up addr
                            _IfeDAddr=IME_D1_Para.y3_addr;//down addr
                        break;
                    case 3: _IfeAddr=IME_D1_Para.y3_addr;//current addr
                            _IfeUAddr=IME_D1_Para.y2_addr;//up addr
                            _IfeDAddr=((IME_D1_Para_ext.Ime_BuffNum==IME_OUTPUT_PPB_5)?(IME_D1_Para.y4_addr):(IME_D1_Para.y3_addr));//down addr
                        break;
                    case 4: _IfeAddr=IME_D1_Para.y4_addr;//current addr
                            _IfeUAddr=IME_D1_Para.y3_addr;//up addr
                            _IfeDAddr=IME_D1_Para.y0_addr;//down addr
                        break;
                    default: _IfeAddr = _IfeUAddr = _IfeDAddr = 0;
                        break;
                }
                ImgFilterInfo.Mode = IFE_Y;
                ImgFilterInfo.ImgAddr = _IfeAddr;
                //ImgFilterInfo.up_addr = _IfeUAddr;//data have been overwrite by previous ife rst
                ImgFilterInfo.up_addr = ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_Y) + //get clean data for upside boundry
                pife_para->repeat * IFE_OVERLAP_V*IME_D1_Para.out_lineoffset - IFE_OVERLAP_H;   //-4 is to get end addr.
                //ife handle ime buf0 at 2nd ime slice end
                if((Ife_Cnt == 0)&&(IFE_POSTPROC_START==1))
                {
                    ImgFilterInfo.up_addr = 0x0;
                    //copy cur src downside boundry to ife src buf
                    //for next time ife d2d upside boundry
                    DataCopy(
                        ImgFilterInfo.ImgAddr + (ProcHeight - pife_para->repeat * IFE_OVERLAP_V)*IME_D1_Para.out_lineoffset,
                        IME_D1_Para.out_lineoffset,
                        pife_para->repeat * IFE_OVERLAP_V,
                        ImgFilterInfo.Mode);
                }
                ImgFilterInfo.down_addr = _IfeDAddr;
                //ife handle ime last slice data
                if(Ife_Cnt == (CounterMax-1))
                {
                    ImgFilterInfo.down_addr = 0x0;
                }
                ImgFilterInfo.HSize = IME_D1_Para.out_h;
                ImgFilterInfo.VSize = ProcHeight;
                ImgFilterInfo.InLineOfs = IME_D1_Para.out_lineoffset;
                ImgFilterInfo.OutLineOfs = IME_D1_Para.out_lineoffset;
                ImgFilterInfo.WaitEnd = IFE_WAITFMD;
                ImgFilterInfo.BufAddr = ippbuf_GetBuf(IPPBUF_CAP_IFESLICE);
                ImgFilterInfo.pCB = DataCopy;
                ImageFilter(&ImgFilterInfo);
            }
            //#NT#only CbCr
            if(GetIPPFuncInfor(IFE_YCC_EN))
            {
                pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_YCC_TABLE_ADDR);
                ifeReT = 0;
                {
                    UINT32 i=0;
                    for(i=0; pife_para[i].repeat != 0xFF; i++)
                    {
                        ifeReT += pife_para[i].repeat;
                    }
                }
                //ife current buffid
                tmp = Ime_Counter%(IME_D1_Para_ext.Ime_BuffNum+1);
                _IfeBufID = ((tmp>0)?(tmp-1):IME_D1_Para_ext.Ime_BuffNum);

                //Cb
                switch(_IfeBufID)
                {
                    case 0: _IfeAddr=IME_D1_Para.cb0_addr;//current addr
                            _IfeUAddr=((IME_D1_Para_ext.Ime_BuffNum==IME_OUTPUT_PPB_5)?(IME_D1_Para.cb4_addr):(IME_D1_Para.cb3_addr));//up addr
                            _IfeDAddr=IME_D1_Para.cb1_addr;//down addr
                        break;
                    case 1: _IfeAddr=IME_D1_Para.cb1_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cb0_addr;//up addr
                            _IfeDAddr=IME_D1_Para.cb2_addr;//down addr
                        break;
                    case 2: _IfeAddr=IME_D1_Para.cb2_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cb1_addr;//up addr
                            _IfeDAddr=IME_D1_Para.cb3_addr;//down addr
                        break;
                    case 3: _IfeAddr=IME_D1_Para.cb3_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cb2_addr;//up addr
                            _IfeDAddr=((IME_D1_Para_ext.Ime_BuffNum==IME_OUTPUT_PPB_5)?(IME_D1_Para.cb4_addr):(IME_D1_Para.cb0_addr));//down addr
                        break;
                    case 4: _IfeAddr=IME_D1_Para.cb4_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cb3_addr;//up addr
                            _IfeDAddr=IME_D1_Para.cb0_addr;//down addr
                        break;
                    default: _IfeAddr = _IfeUAddr = _IfeDAddr = 0;
                        break;
                }
                ImgFilterInfo.Mode = IFE_YCC;
                ImgFilterInfo.ImgAddr = _IfeAddr;
                //ImgFilterInfo.up_addr = _IfeUAddr;//data have been overwrite by previous ife rst
                ImgFilterInfo.up_addr = ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_CC) +     //get clean data for upside boundry
                ifeReT * IFE_OVERLAP_V*IME_D1_Para.out_Cb_lineoffset - IFE_OVERLAP_H;     //-4 is to get end addr.
                //ife handle ime buf0 at 2nd ime slice end
                if((Ife_Cnt == 0)&&(IFE_POSTPROC_START==1))
                {
                    ImgFilterInfo.up_addr = 0x0;
                    //copy cur src downside boundry to ife src buf
                    //for next time ife d2d upside boundry
                    grph_open();
                    grph_init(PLL_CLKSEL_GRAPHIC_160);
                    grph_enableDMA();
                    grph_setImg1(   ImgFilterInfo.ImgAddr + (ProcHeight/(_Shift/2) - ifeReT * IFE_OVERLAP_V)*IME_D1_Para.out_Cb_lineoffset,
                                            IME_D1_Para.out_Cb_lineoffset,
                                            ifeReT * IFE_OVERLAP_V,
                                            IME_D1_Para.out_h/2);
                    grph_setImg2(   ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_CC),
                                    IME_D1_Para.out_Cb_lineoffset);
                    grph_setAOP(    0,  GRPH_DST_2, GRPH_AOP_A_COPY,    0);
                    grph_disableDMA();
                    grph_swReset();
                    grph_close();
                }
                ImgFilterInfo.down_addr = _IfeDAddr;
                //ife handle ime last slice data
                if(Ife_Cnt == (CounterMax-1))
                {
                    ImgFilterInfo.down_addr = 0x0;
                }
                ImgFilterInfo.HSize = IME_D1_Para.out_h/2;//422fmt
                ImgFilterInfo.VSize = ProcHeight/(_Shift/2);//420fmt
                ImgFilterInfo.InLineOfs = IME_D1_Para.out_Cb_lineoffset;
                ImgFilterInfo.OutLineOfs = IME_D1_Para.out_Cb_lineoffset;
                ImgFilterInfo.WaitEnd = IFE_WAITFMD;
                ImgFilterInfo.BufAddr = ippbuf_GetBuf(IPPBUF_CAP_IFESLICE);
                ImgFilterInfo.pCB = DataCopy;
                ImageFilter(&ImgFilterInfo);

                //Cr
                switch(_IfeBufID)
                {
                    case 0: _IfeAddr=IME_D1_Para.cr0_addr;//current addr
                            _IfeUAddr=((IME_D1_Para_ext.Ime_BuffNum==IME_OUTPUT_PPB_5)?(IME_D1_Para.cr4_addr):(IME_D1_Para.cr3_addr));//up addr
                            _IfeDAddr=IME_D1_Para.cr1_addr;//down addr
                        break;
                    case 1: _IfeAddr=IME_D1_Para.cr1_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cr0_addr;//up addr
                            _IfeDAddr=IME_D1_Para.cr2_addr;//down addr
                        break;
                    case 2: _IfeAddr=IME_D1_Para.cr2_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cr1_addr;//up addr
                            _IfeDAddr=IME_D1_Para.cr3_addr;//down addr
                        break;
                    case 3: _IfeAddr=IME_D1_Para.cr3_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cr2_addr;//up addr
                            _IfeDAddr=((IME_D1_Para_ext.Ime_BuffNum==IME_OUTPUT_PPB_5)?(IME_D1_Para.cr4_addr):(IME_D1_Para.cr0_addr));//down addr
                        break;
                    case 4: _IfeAddr=IME_D1_Para.cr4_addr;//current addr
                            _IfeUAddr=IME_D1_Para.cr3_addr;//up addr
                            _IfeDAddr=IME_D1_Para.cr0_addr;//down addr
                        break;
                    default: _IfeAddr = _IfeUAddr = _IfeDAddr = 0;
                        break;
                }
                ImgFilterInfo.ImgAddr = _IfeAddr;
                //ImgFilterInfo.up_addr = _IfeUAddr;//data have been overwrite by previous ife rst
                ImgFilterInfo.up_addr = ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_CC)
                    + ifeReT * IFE_OVERLAP_V*IME_D1_Para.out_Cb_lineoffset +       //get clean data for upside boundry
                    ifeReT * IFE_OVERLAP_V*IME_D1_Para.out_Cr_lineoffset - IFE_OVERLAP_H;   //-4 is to get end addr.
                //ife handle ime buf0 at 2nd ime slice end
                if((Ife_Cnt == 0)&&(IFE_POSTPROC_START==1))
                {
                    ImgFilterInfo.up_addr = 0x0;
                    grph_open();
                    grph_init(PLL_CLKSEL_GRAPHIC_160);
                    grph_enableDMA();
                    grph_setImg1(   ImgFilterInfo.ImgAddr + (ProcHeight/(_Shift/2) - ifeReT * IFE_OVERLAP_V)*IME_D1_Para.out_Cr_lineoffset,
                                            IME_D1_Para.out_Cr_lineoffset,
                                            ifeReT * IFE_OVERLAP_V,
                                            IME_D1_Para.out_h/2);
                    grph_setImg2(   ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_CC) + ifeReT * IFE_OVERLAP_V*IME_D1_Para.out_Cb_lineoffset,
                                    IME_D1_Para.out_Cr_lineoffset);
                    grph_setAOP(    0,  GRPH_DST_2, GRPH_AOP_A_COPY,    0);
                    grph_disableDMA();
                    grph_swReset();
                    grph_close();
                }
                ImgFilterInfo.down_addr = _IfeDAddr;
                //ife handle ime last slice data
                if(Ife_Cnt == (CounterMax-1))
                {
                    ImgFilterInfo.down_addr = 0x0;
                }
                ImgFilterInfo.InLineOfs = IME_D1_Para.out_Cr_lineoffset;
                ImgFilterInfo.OutLineOfs = IME_D1_Para.out_Cr_lineoffset;
                ImgFilterInfo.BufAddr = ippbuf_GetBuf(IPPBUF_CAP_IFESLICE);
                ImageFilter(&ImgFilterInfo);

            }

            Ife_Cnt ++;
            //Y curve
            if (GetIPPFuncInfor(YCURVE_EN))
            {
                debug_err(("Y curve not ready\r\n"));
            }
            //chroma curve
            if(GetIPPFuncInfor(CHRCURVE_EN))
            {
                debug_err(("Ycc curve not ready\r\n"));
            }
        }

        //if post proc. is enable,jpg must follow ife output.
        if((bPostProc)&&(Ime_Counter >= IFE_POSTPROC_START))
        {
            switch(_IfeBufID)
            {
                case 0:
                    JpgYAddr=IME_D1_Para.y0_addr;JpgCbAddr=IME_D1_Para.cb0_addr;JpgCrAddr=IME_D1_Para.cr0_addr;
                    break;
                case 1:
                    JpgYAddr=IME_D1_Para.y1_addr;JpgCbAddr=IME_D1_Para.cb1_addr;JpgCrAddr=IME_D1_Para.cr1_addr;
                    break;
                case 2:
                    JpgYAddr=IME_D1_Para.y2_addr;JpgCbAddr=IME_D1_Para.cb2_addr;JpgCrAddr=IME_D1_Para.cr2_addr;
                    break;
                case 3:
                    JpgYAddr=IME_D1_Para.y3_addr;JpgCbAddr=IME_D1_Para.cb3_addr;JpgCrAddr=IME_D1_Para.cr3_addr;
                    break;
                case 4:
                    JpgYAddr=IME_D1_Para.y4_addr;JpgCbAddr=IME_D1_Para.cb4_addr;JpgCrAddr=IME_D1_Para.cr4_addr;
                    break;
                default:
                    JpgYAddr = JpgCbAddr = JpgCrAddr = 0;
                    break;
            }
        }

        //datastamp
        //if post proc. is enable,stamp must follow ife eng.
        if((bPostProc)&&(Ime_Counter >= IFE_POSTPROC_START))
            StampCnt = Ife_Cnt-1;
        else
            StampCnt = Ime_Counter;
        if(StampCnt==DataStampVDoBlk && datastamp_IsEnable())
        {
            if (DataStampVRemain>(SliceHeight-DataStampVDoBlkStartPxl))
            {
                DataStampVPartialSize = (SliceHeight-DataStampVDoBlkStartPxl);
            }
            else
                DataStampVPartialSize = DataStampVRemain;

            dsBackgroundTmp.AddrY  = JpgYAddr+(GetIPPSizeInfor(_ImeCapOutHsize)*DataStampVDoBlkStartPxl+DataStampHStartPxl);
            dsBackgroundTmp.AddrCb = JpgCbAddr+(GetIPPSizeInfor(_ImeCapOutHsize)*DataStampVDoBlkStartPxl/_Shift+DataStampHStartPxl/2);
            dsBackgroundTmp.AddrCr = JpgCrAddr+(GetIPPSizeInfor(_ImeCapOutHsize)*DataStampVDoBlkStartPxl/_Shift+DataStampHStartPxl/2);
            dsBackgroundTmp.LineOffset = GetIPPSizeInfor(_ImeCapOutHsize);
            dsBackgroundTmp.Format = dsStamp.Format;
            dsStampTmp.AddrY    = dsStamp.AddrY+((dsStamp.Height-DataStampVRemain)*dsStamp.Width);
            dsStampTmp.AddrCb   = dsStamp.AddrCb+((dsStamp.Height-DataStampVRemain)*dsStamp.Width/_Shift);
            dsStampTmp.AddrCr   = dsStamp.AddrCr+((dsStamp.Height-DataStampVRemain)*dsStamp.Width/_Shift);
            dsStampTmp.LineOffset   = dsStamp.LineOffset;
            dsStampTmp.Height   = DataStampVPartialSize;
            dsStampTmp.Width    = dsStamp.Width;
            dsStampTmp.ColorKeyY    = dsStamp.ColorKeyY;
            dsStampTmp.ColorKeyCb   = dsStamp.ColorKeyCb;
            dsStampTmp.ColorKeyCr   = dsStamp.ColorKeyCr;
            dsStampTmp.Format = dsStamp.Format;

            datastamp_Open(PLL_CLKSEL_GRAPHIC_160);

            datastamp_ImprintYCC(   &dsBackgroundTmp,
                                    &dsStampTmp,
                                    ippbuf_GetBuf(IPPBUF_CAP_DATA_STAMP));
            datastamp_Close();

            DataStampVRemain -= DataStampVPartialSize;
            if(DataStampVRemain)
            {
                DataStampVDoBlkStartPxl = 0;
                DataStampVDoBlk ++;
            }
        }

        /************* step 3 encode to jpeg & trigger next slice *************/
        if(!bPostProc)//if postproc. is disable,jpg trigger at each ime slice end.
        {
            if (Ime_Counter == 0)//1st ime slicedone
            {
                //jpeg slice start
                jpeg_setSliceEncAddr(SliceHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
                jpeg_setStartEncode();
                ime_triggerSlice();
            }
            else if (Ime_Counter == (CounterMax - 1))//last slice
            {
                //jpeg slice end
                while(1)
                {
                    uiJpgStatus = jpeg_waitDone();
                    if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                    {
                        break;
                    }
                }
                if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
                {
                    CloseIPPEng();
                    jpeg_setEndEncode();
                    jpeg_close();
                    return E_BOVR;
                }
                //#NT#2011/01/20#ethanlau -begin
                //#NT#jpg driver have been modified last slice hight condition, can't use 0.
                jpeg_setSliceEncAddr(SliceHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
                //#NT#2011/01/20#ethanlau -end
                CAP_MSG(" \r\n");
            }
            else
            {
                //jpeg slice
                while(1)
                {
                    uiJpgStatus = jpeg_waitDone();
                    if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                    {
                        break;
                    }
                }
                if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
                {
                    CloseIPPEng();
                    jpeg_setEndEncode();
                    jpeg_close();
                    return E_BOVR;
                }
                jpeg_setSliceEncAddr(SliceHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
                ime_triggerSlice();
            }
        }
        else // jpg trigger at each ife frameend
        {
            if (Ife_Cnt == 1)//1st ife fmd
            {
                //jpeg slice start
                //CAP_MSG(" (ad:0x%x 0x%x 0x%x, (%d,%d)) ",JpgYAddr,JpgCbAddr,JpgCrAddr,Ife_Cnt,_IfeBufID);
                jpeg_setSliceEncAddr(SliceHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
                jpeg_setStartEncode();
                ime_triggerSlice();
            }
            else if (Ife_Cnt == (CounterMax-1))//last ife fmd,CounterMax have been add 1 when Postporc. enable
            {
                //jpeg slice end
                while(1)
                {
                    uiJpgStatus = jpeg_waitDone();
                    if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                    {
                        break;
                    }
                }
                if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
                {
                    CloseIPPEng();
                    jpeg_setEndEncode();
                    jpeg_close();
                    return E_BOVR;
                }
                //CAP_MSG(" (ad:0x%x 0x%x 0x%x, (%d,%d)) ",JpgYAddr,JpgCbAddr,JpgCrAddr,Ife_Cnt,_IfeBufID);
                //#NT#2011/01/20#ethanlau -begin
                //#NT#jpg driver have been modified last slice hight condition, can't use 0.
                jpeg_setSliceEncAddr(SliceHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
                //#NT#2011/01/20#ethanlau -end
                debug_msg(" \r\n");
            }
            else
            {
                if(Ife_Cnt != 0)//if Ife_Cnt == 0 , jpg do nothing
                {
                    //jpeg slice
                    while(1)
                    {
                        uiJpgStatus = jpeg_waitDone();
                        if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                        {
                            break;
                        }
                    }
                    if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
                    {
                        CloseIPPEng();
                        jpeg_setEndEncode();
                        jpeg_close();
                        return E_BOVR;
                    }

                    //CAP_MSG(" (ad:0x%x 0x%x 0x%x, (%d,%d)) ",JpgYAddr,JpgCbAddr,JpgCrAddr,Ife_Cnt,_IfeBufID);
                    jpeg_setSliceEncAddr(SliceHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
                }
                if (uiImeStatus & IME_INT_FRM)
                {
                    //do thing,ime already done
                }
                else
                    ime_triggerSlice();
            }
        }

    }
    CloseIPPEng();
    //wait jpeg encode finish
    while(1)
    {
        uiJpgStatus = jpeg_waitDone();
        if (uiJpgStatus & (_JPEG_Frame_End | _JPEG_Buf_End))
        {
            break;
        }
    }
    if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
    {
        jpeg_setEndEncode();
        jpeg_close();
        return E_BOVR;
    }
    jpeg_setEndEncode();
    jpeg_close();
    *JpgSize = jpeg_getComSize() +  JPGHeaderSize;
    return E_OK;
}

static UINT32 m_uiJpgToutCnt = 10;
static UINT32 m_uiJpgMinQ = 40;
ER FrameMode_Process(UINT BayerAddr, UINT JpgDAddr, UINT8 JpgQuality, UINT *JpgSize)
{
    UINT MainImgSize, retry=0;
    //UINT32 BlackV;
    //IME_OPENOBJ IMEOpenObj;
    JPGCmpParam JPGParam = {0};
    IPE_IME_FM_PARAM fmInfo;
    ER rtStatus, rtBRCStatus;
    //UINT32 uiTmpQVal;
    //UINT32 P1YAddr, P1CbAddr, P1CrAddr;
    //UINT32 P2YAddr, P2CbAddr, P2CrAddr;
    IMG_FILTER_PARAM ImgFilterInfo;
    UINT32 JpgFmt,_Shift;
    //STR_YCBCR_ADDR YCCInAddr, VAOutAddr;
    //#NT#2010/08/09/#Linkin Huang -begin
    //#NT# integrate function interface
    //ImgYCC_POST_PROCESS_INFO PostProcInfo;
    //#NT#2010/08/09/#Linkin Huang -end

    //source
    fmInfo.BayerAddr = BayerAddr;
    fmInfo.BayerHsize = GetIPPSizeInfor(_SieCropCapHsize);
    fmInfo.BayerVsize = GetIPPSizeInfor(_SieCropCapVsize);

    //dest
    fmInfo.ImeOutPath = IMEALG_1PATH;
    fmInfo.ImeHsize1 = GetIPPSizeInfor(_ImeCapOutHsize);
    fmInfo.ImeVsize1 = GetIPPSizeInfor(_ImeCapOutVsize);
    fmInfo.ImeFmt1 = GetIPPSizeInfor(_ImePath1Fmt);
    fmInfo.ImeLineoffset1 = GetIPPSizeInfor(_ImeCapOutYLineOffset);
    fmInfo.ImeLineCCoffset1 = GetIPPSizeInfor(_ImeCapOutCCLineOffset);
    //image output address
    fmInfo.OutputAddr1Y = ippbuf_GetBuf(IPPBUF_CAP_FRAME_Y);
    fmInfo.OutputAddr1Cb = ippbuf_GetBuf(IPPBUF_CAP_FRAME_CB);
    fmInfo.OutputAddr1Cr = ippbuf_GetBuf(IPPBUF_CAP_FRAME_CR);
    if(GetIPPFuncInfor(IFE_Y_EN))
    {
        fmInfo.OutputAddr1Y += IFE_OVERLAP_H;
    }
    if(GetIPPFuncInfor(IFE_YCC_EN))
    {
        fmInfo.OutputAddr1Cb += IFE_OVERLAP_H;
        fmInfo.OutputAddr1Cr += IFE_OVERLAP_H;
    }
    ipe_ime_Framemode(fmInfo);
    //#NT#2010/04/23#Linkin Huang -begin
    //#NT# ipp debug
    #if IPP_DEBUG
    {
       char filename[64];

       sprintf(filename, "A:\\RegBin\\IPE.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_IPE);

       sprintf(filename, "A:\\RegBin\\IME.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_IME);
    }
    #endif
    //#NT#2010/04/23#Linkin Huang -end

    if(GetIPPFuncInfor(IFE_Y_EN))
    {
        IFE_OPENOBJ     IFEOpenObj;
        SenFlowFunc(IFE_Y_SETTING);

        ImgFilterInfo.Mode = IFE_Y;
        ImgFilterInfo.ImgAddr = fmInfo.OutputAddr1Y;
        ImgFilterInfo.HSize = fmInfo.ImeHsize1;
        ImgFilterInfo.VSize = fmInfo.ImeVsize1;

        ImgFilterInfo.InLineOfs = fmInfo.ImeLineoffset1;
        ImgFilterInfo.OutLineOfs = fmInfo.ImeLineoffset1;
        ImgFilterInfo.up_addr = 0;
        ImgFilterInfo.down_addr = 0;
        ImgFilterInfo.WaitEnd = ENABLE;
        //IFE d2d overlap, outaddr must shift left 1 word,lineoft add 1 word.
        ImgFilterInfo.BufAddr = ImgFilterInfo.ImgAddr - IFE_OVERLAP_H;
        ImgFilterInfo.pCB = NULL;
        ife_open(&IFEOpenObj);
        ImageFilter(&ImgFilterInfo);
        ife_close();
        fmInfo.OutputAddr1Y -= IFE_OVERLAP_H;
    }
    if(GetIPPFuncInfor(IFE_YCC_EN))
    {
        IFE_OPENOBJ     IFEOpenObj;
        SenFlowFunc(IFE_YCC_SETTING);
        //Cb
        ImgFilterInfo.Mode = IFE_YCC;
        ImgFilterInfo.ImgAddr = fmInfo.OutputAddr1Cb;
        ImgFilterInfo.HSize = fmInfo.ImeHsize1/2;
        ImgFilterInfo.VSize = fmInfo.ImeVsize1;
        if(fmInfo.ImeFmt1 == IMEOUT2_FMT420_1)
        {
            ImgFilterInfo.VSize /= 2;
        }
        ImgFilterInfo.InLineOfs = fmInfo.ImeLineCCoffset1;
        ImgFilterInfo.OutLineOfs = fmInfo.ImeLineCCoffset1;
        ImgFilterInfo.up_addr = 0;
        ImgFilterInfo.down_addr = 0;
        ImgFilterInfo.WaitEnd = ENABLE;
        //IFE d2d overlap, outaddr must shift left 1 word,lineoft add 1 word.
        ImgFilterInfo.BufAddr = ImgFilterInfo.ImgAddr - IFE_OVERLAP_H;
        ImgFilterInfo.pCB = NULL;
        ife_open(&IFEOpenObj);
        ImageFilter(&ImgFilterInfo);
        ife_close();
        fmInfo.OutputAddr1Cb -= IFE_OVERLAP_H;
        //Cr
        ImgFilterInfo.Mode = IFE_YCC;
        ImgFilterInfo.ImgAddr = fmInfo.OutputAddr1Cr;
        //IFE d2d overlap, outaddr must shift left 1 word,lineoft add 1 word.
        ImgFilterInfo.BufAddr = ImgFilterInfo.ImgAddr - IFE_OVERLAP_H;
        ImageFilter(&ImgFilterInfo);
        fmInfo.OutputAddr1Cr -= IFE_OVERLAP_H;
    }

#if 0
    if ( IE_GetCapStatus() == TRUE )
        IE_ProcessCaptureARF();
#endif
//post proc. marked
#if 0
    //process primary image
    //#NT#2010/08/09/#Linkin Huang -begin
    //#NT# integrate function interface
    SetIPPFuncInfor(PRIIMG_INPROCESS, ENABLE);
    PostProcInfo.AddrY = fmInfo.OutputAddr1Y;
    PostProcInfo.AddrCb = fmInfo.OutputAddr1Cb;
    PostProcInfo.AddrCr = fmInfo.OutputAddr1Cr;
    PostProcInfo.WidthY = fmInfo.ImeHsize1;
    PostProcInfo.HeightY = fmInfo.ImeVsize1;
    PostProcInfo.LineoffsetY = fmInfo.ImeLineoffset1;
    PostProcInfo.WidthCx  = (PostProcInfo.WidthY >> 1);
    PostProcInfo.LineoffsetCx = (PostProcInfo.LineoffsetY >> 1);
    //PostProcInfo.IpeD2DEn = IpeD2DEn;
    //PostProcInfo.YCCPostProcCB = IPED2DCB;
    if((fmInfo.ImeFmt1 == IME_OUTPUT_YCC_422_COS) || (fmInfo.ImeFmt1 == IME_OUTPUT_YCC_422_CEN))
    {
        PostProcInfo.HeightCx = PostProcInfo.HeightY;
    }
    else if((fmInfo.ImeFmt1 == IME_OUTPUT_YCC_420_COS) || (fmInfo.ImeFmt1 == IME_OUTPUT_YCC_420_CEN ))
    {
        PostProcInfo.HeightCx = (PostProcInfo.HeightY >> 1);
    }
    else
    {
        debug_err(("-E- ImgProcessFrame unsupport format %d\r\n", fmInfo.ImeFmt1));
    }
    //ImgYCCPostProcess(fmInfo.OutputAddr1Y, fmInfo.OutputAddr1Cb, fmInfo.OutputAddr1Cr, fmInfo.ImeHsize1, fmInfo.ImeVsize1, fmInfo.ImeLineoffset1, AlgInfo);
    ImgYCCPostProcess(&PostProcInfo, AlgInfo);
    SetIPPFuncInfor(PRIIMG_INPROCESS, DISABLE);
    //#NT#2010/08/09/#Linkin Huang -end

    //process pre qv image
    //#NT#2010/08/09/#Linkin Huang -begin
    //#NT# integrate function interface
    SetIPPFuncInfor(THUMBIMG_INPROCESS, ENABLE);
    PostProcInfo.AddrY = fmInfo.OutputAddr2Y;
    PostProcInfo.AddrCb = fmInfo.OutputAddr2Cb;
    PostProcInfo.AddrCr = fmInfo.OutputAddr2Cr;
    PostProcInfo.WidthY = fmInfo.ImeHsize2;
    PostProcInfo.HeightY = fmInfo.ImeVsize2;
    PostProcInfo.LineoffsetY = fmInfo.ImeLineoffset2;
    PostProcInfo.WidthCx  = (PostProcInfo.WidthY >> 1);
    PostProcInfo.LineoffsetCx = (PostProcInfo.LineoffsetY >> 1);
    //PostProcInfo.IpeD2DEn = IpeD2DEn;
    //PostProcInfo.YCCPostProcCB = IPED2DCB;
    if((fmInfo.ImeFmt2 == IME_OUTPUT_YCC_422_COS) || (fmInfo.ImeFmt2 == IME_OUTPUT_YCC_422_CEN))
    {
        PostProcInfo.HeightCx = PostProcInfo.HeightY;
    }
    else if((fmInfo.ImeFmt2 == IME_OUTPUT_YCC_420_COS) || (fmInfo.ImeFmt2 == IME_OUTPUT_YCC_420_CEN))
    {
        PostProcInfo.HeightCx = (PostProcInfo.HeightY >> 1);
    }
    else
    {
        debug_err(("-E- ImgProcessFrame unsupport format %d\r\n", fmInfo.ImeFmt1));
    }
    //ImgYCCPostProcess(fmInfo.OutputAddr2Y, fmInfo.OutputAddr2Cb, fmInfo.OutputAddr2Cr, fmInfo.ImeHsize2, fmInfo.ImeVsize2, fmInfo.ImeLineoffset2, AlgInfo);
    ImgYCCPostProcess(&PostProcInfo, AlgInfo);
    SetIPPFuncInfor(THUMBIMG_INPROCESS, DISABLE);
    //#NT#2010/08/09/#Linkin Huang -end
#endif

#if 0//small img 16:9 black board
    //draw black frame
    if (AlgInfo->ImageRatio != _Ratio_4_3)
    {
        BlackV = (GetIPPSizeInfor(_QVPreVsize) - fmInfo.ImeVsize2) >> 1;
        //upper
        memset((void *)P2YAddr, 0x0, (fmInfo.ImeLineoffset2 * BlackV));
        memset((void *)P2CbAddr, 0x80, ((fmInfo.ImeLineoffset2 / 2) * BlackV));
        memset((void *)P2CrAddr, 0x80, ((fmInfo.ImeLineoffset2 / 2) * BlackV));
        //lower
        memset((void *)(P2YAddr + (fmInfo.ImeLineoffset2 * (BlackV + fmInfo.ImeVsize2))), 0x0, (fmInfo.ImeLineoffset2 * BlackV));
        memset((void *)(P2CbAddr + ((fmInfo.ImeLineoffset2 / 2) * (BlackV + fmInfo.ImeVsize2))), 0x80, ((fmInfo.ImeLineoffset2 / 2) * BlackV));
        memset((void *)(P2CrAddr + ((fmInfo.ImeLineoffset2 / 2) * (BlackV + fmInfo.ImeVsize2))), 0x80, ((fmInfo.ImeLineoffset2 / 2) * BlackV));
    }
#endif

//Jpg Bitstream
    JPGParam.RawImageAddr[0] = fmInfo.OutputAddr1Y;
    JPGParam.RawImageAddr[1] = fmInfo.OutputAddr1Cb;
    JPGParam.RawImageAddr[2] = fmInfo.OutputAddr1Cr;
    JPGParam.JPGBSAddr      = JpgDAddr;
    JPGParam.JPGBSBufSize   = &MainImgSize;
    JPGParam.Width          = fmInfo.ImeHsize1;
    JPGParam.Height         = fmInfo.ImeVsize1;
    JPGParam.LineOffsetY    = fmInfo.ImeLineoffset1;
    JPGParam.LineOffsetU = fmInfo.ImeLineCCoffset1;
    JPGParam.LineOffsetV = fmInfo.ImeLineCCoffset1;
    switch(fmInfo.ImeFmt1)
    {
        case IMEOUT2_FMT444:    JpgFmt = JPEG_FORMAT_444;   _Shift = 1;
            break;
        case IMEOUT2_FMT422_1:
        case IMEOUT2_FMT422_2:  JpgFmt = JPEG_FORMAT_422;   _Shift = 1;
            break;
        case IMEOUT2_FMT420_1:
        case IMEOUT2_FMT420_2:  JpgFmt = JPEG_FORMAT_420;   _Shift = 2;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            JpgFmt = JPEG_FORMAT_422;
            _Shift= 1;
            break;
    }
    JPGParam.Mode = JpgFmt;
    if(GetIPPFuncInfor(FQV_EN))
    {
        if(!GetIPPFuncInfor(FQV_FRAMEMDOE_EN))
        {
            //get dcout ratio and size
            JPGParam.DCOutCfg.FrameWidth = fmInfo.ImeHsize1;
            JPGParam.DCOutCfg.FrameHeight = fmInfo.ImeVsize1;
            JPGParam.DCOutCfg.PanelWidth = GetIPPSizeInfor(_QVPreHsize);
            JPGParam.DCOutCfg.PanelHeight = GetIPPSizeInfor(_QVPreVsize);
            JpegGetDCOutConfig(&JPGParam.DCOutCfg);

            JPGParam.DCOutCfg.DCEnable = TRUE;
            JPGParam.DCOutCfg.YUVFormat = JpgFmt;
            //SetIPPSizeInfor(_QVPreYLineOffset,JPGParam.DCOutCfg.DCYLineOffset);
            //SetIPPSizeInfor(_QVPreUVLineOffset,JPGParam.DCOutCfg.DCULineOffset);
            JPGParam.DCOutCfg.DCYAddr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
            JPGParam.DCOutCfg.DCUAddr = JPGParam.DCOutCfg.DCYAddr + GetIPPSizeInfor(_QVPreYLineOffset) * GetIPPSizeInfor(_QVPreVsize);
            JPGParam.DCOutCfg.DCVAddr = JPGParam.DCOutCfg.DCUAddr + GetIPPSizeInfor(_QVPreUVLineOffset) * GetIPPSizeInfor(_QVPreVsize)/(_Shift/2);
        }
    }
    rtBRCStatus = E_SYS;
    rtStatus = E_SYS;
    CAP_MSG("\r\n");
    while(1)
    {
        MainImgSize = ippbuf_GetBuf(IPPBUF_CAP_JPG_BTSSIZE);
        rtStatus = JpegEncOnePic(&JPGParam, JpgQuality);
        if(rtStatus == E_OK)
            break;
        JpgQuality -= 5;
        if(rtStatus == E_BOVR)
        {
            if((retry > m_uiJpgToutCnt)||(JpgQuality < m_uiJpgMinQ) )
            {
                debug_err(("^R jpg encode time out(bit stream size too large)\r\n"));
                break;
            }
        }
        retry++;
        CAP_MSG("Bitstream retry: %d,%d  ",retry,JpgQuality);
    }
    *JpgSize = MainImgSize;
    CAP_MSG("\r\n");
    CAP_MSG("Bitstream size: 0x%x\r\n",MainImgSize);
#if 0 //BRC
    if(AlgInfo->ReEncMechanism == _CaptureMode_FSizeCtrl)
    {
        JPGParam.Mode |= JPG_Enc_FileSZCtl;
        uiTmpQVal = ucJPEGQuality;
        if(uiTmpQVal < 50)
        {
            uiTmpQVal = (5000/uiTmpQVal);
        }
        else
        {
            uiTmpQVal = 200 - (uiTmpQVal << 1);
        }
        uiTmpQVal *= 32;
        JPEG_RCInit_Photo(g_JPEGBitsteamSize, uiTmpQVal);
        rtBRCStatus = E_SYS;
        rtStatus = E_SYS;
        retry = 0;
        while(retry < g_ReEncCnt)
        {
            JpegInitQValue4Photo(uiTmpQVal);
            MainImgSize = ippbuf_GetBuf(IPPBUF_CAP_JPG_BTSSIZE);
            rtStatus = JpegEncOnePic(&JPGParam, uiTmpQVal);
            //#NT#2009/09/22#Niven Cho -begin
            //#NT#Modified., Daniel Fixed SW Rate Control has no effect!
            if ((E_OK != rtStatus) && (E_BOVR!=rtStatus))
            {
                debug_err(("jpeg encode error!\r\n"));
                break;
            }
            debug_ind(("BS size [Q=%d] = %d\r\n", uiTmpQVal, jpeg_getComSize()));
            if ((jpeg_getComSize() > g_JPGFileSZ_TH1) || (jpeg_getComSize() < g_JPGFileSZ_TH2))
            {
                uiTmpQVal = JPEG_RCCal_Photo(jpeg_getComSize(), g_JPEGBSSZCtrlVal);
                if (uiTmpQVal < JPEG_QVAL_MINIMUM)
                {
                    debug_err(("jpeg bit rate control fail\r\n"));
                    break;
                }
            }
            else
            {
                rtBRCStatus = E_OK;
                break;
            }
            //#NT#2009/09/22#Niven Cho -end
            retry ++;
        }
    }
    //#NT#2009/10/28#Daniel Lu -begin
    //#NT#add BRC function
    else if(AlgInfo->ReEncMechanism == _CaptureMode_VAEstCtrl)
    {
//KH mark
#if 0
        ImgVA16x16to8x8(VAOutAddr.y_addr, VAParam.YVA);
        ImgVA16x16to8x8(VAOutAddr.cb_addr, VAParam.UVA);
        ImgVA16x16to8x8(VAOutAddr.cr_addr, VAParam.VVA);
#endif
        VAParam.TargetByte = g_JPEGBitsteamSize;
        uiTmpQVal = JpegVAtoRC(&JPGParam, &VAParam);
        JPEG_RCInit_Photo(g_JPEGBitsteamSize, uiTmpQVal);
        JPGParam.Mode |= JPG_Enc_VAEstCtl;
        //#NT#2011/06/16#Ben Wang -begin
        //#NT#refine UserJpeg API
        //JPGParam.pVAParam = &VAParam;
        //#NT#2011/06/16#Ben Wang -end
        rtBRCStatus = E_SYS;
        rtStatus = E_SYS;
        retry = 0;
        while(retry < g_ReEncCnt)
        {
            JpegInitQValue4Photo(uiTmpQVal);
            if (uiTmpQVal == 0)
            {
                debug_err(("JpegVAtoRC setting error!\r\n"));
                break;
            }
            MainImgSize = ippbuf_GetBuf(IPPBUF_CAP_JPG_BTSSIZE);
            rtStatus = JpegEncOnePic(&JPGParam, uiTmpQVal);
            if ((E_OK != rtStatus) && (E_BOVR!=rtStatus))
            {
                debug_err(("jpeg encode error!\r\n"));
                break;
            }
            debug_ind(("BS size [Q=%d] = %d\r\n", uiTmpQVal, jpeg_getComSize()));
            if ((jpeg_getComSize() > g_JPGFileSZ_TH1) || (jpeg_getComSize() < g_JPGFileSZ_TH2))
            {
                if(uiTmpQVal == 1)
                {
                    debug_err(("jpeg QP reach maximun value!\r\n"));
                    break;
                }
                uiTmpQVal = JPEG_RCCal_Photo(jpeg_getComSize(), g_JPEGBSSZCtrlVal);
                if ((uiTmpQVal < 1) && (uiTmpQVal > 1920))
                {
                    debug_err(("jpeg bit rate control fail\r\n"));
                    break;
                }
            }
            else
            {
                rtBRCStatus = E_OK;
                break;
            }
            retry ++;
        }
    }
    //#NT#2009/10/28#Daniel Lu -end

    //#NT#2009/12/11#Niven Cho -begin
    //#NT#Fix FileCtrl Capture DNP fail handler
    //if ((AlgInfo->ReEncMechanism == _CaptureMode_FixQ) || ((rtBRCStatus == E_SYS) && (rtStatus != E_OK)))
    if ((AlgInfo->ReEncMechanism == _CaptureMode_FixQ) || (rtBRCStatus != E_OK) || (rtStatus != E_OK))
    //#NT#2009/12/11#Niven Cho -end
    {
        rtStatus = E_SYS;
        retry = 0;
        while(retry < 19)
        {
            MainImgSize = ippbuf_GetBuf(IPPBUF_CAP_JPG_BTSSIZE);

            rtStatus = JpegEncOnePic(&JPGParam, (ucJPEGQuality - (retry * 5)));
            if (E_OK == rtStatus)
            {
                break;
            }
            retry ++;
        }
    }
#endif
    return rtStatus;
}

//#NT#2011/08/26#Ethan Lau -begin
//#NT#for writedebuginfo
UINT32 m_uiJpgQ = 0;
static void Sen_SetJpgQinfo(UINT32 Q)
{
    m_uiJpgQ = Q;
}

UINT32 Sen_GetJpgQinfo(void)
{
    return m_uiJpgQ;
}
//#NT#2011/08/26#Ethan Lau -end

UINT8 Sen_GetJpgQuality(void)
{
    return JPEG_Quality[gImageAlgInfo.Quality];
}

UINT32 Sen_GetBurstNum(void)
{
    return Get_CapBufAlloc(2);
}

UINT32 Sen_GetRawNum(void)
{
    return Get_CapBufAlloc(0);
}

UINT32 Sen_GetJpgNum(void)
{
    return Get_CapBufAlloc(1);
}

void ipe_ime_Framemode(IPE_IME_FM_PARAM fmInfo)
{
    UINT IPECFAPat = SIE_PRE_Para_cap.SieCfaPat;
    IPEStripeInfo IpeHStripe,IpeVStripe;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;
    IFE_OPENOBJ IFEOpenObj;
    SliceSizeInfo _OutInfo;
    SliceGenIn _InInfo;

    IPEOpenObj.FP_IPEISR_CB = NULL;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_96;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IFEOpenObj.FP_IFEISR_CB = NULL;
    ipe_open(&IPEOpenObj);
    ife_open(&IFEOpenObj);
    ime_open(&IMEOpenObj);

    //Get address and set IME for path1 and path2
    GetIMEExtInfo(&IME_D1_Para_ext,&IME_D2_Para_ext,IPE_IME_FRAME_1PATH_JOB);

    IME_D1_Para.y0_addr= fmInfo.OutputAddr1Y;
    IME_D1_Para.cb0_addr=fmInfo.OutputAddr1Cb;
    IME_D1_Para.cr0_addr=fmInfo.OutputAddr1Cr;
    IME_D1_Para.in_h=fmInfo.BayerHsize-16;
    IME_D1_Para.in_v=fmInfo.BayerVsize;
    //direct path ime input lineoffset don't care
    //IME_D1_Para.in_lineoffset = IME_D1_Para.in_h;
    //IME_D1_Para.in_Cb_lineoffset = IME_D1_Para.in_lineoffset/2;
    //IME_D1_Para.in_Cr_lineoffset = IME_D1_Para.in_lineoffset/2;
    IME_D1_Para.out_h=fmInfo.ImeHsize1;
    IME_D1_Para.out_v=fmInfo.ImeVsize1;
    IME_D1_Para.in_format=IMEIN_FMT444;    //ipe always 444 fmt
    IME_D1_Para.out_lineoffset= fmInfo.ImeLineoffset1;//fmInfo.ImeHsize1;
    IME_D1_Para.out_Cb_lineoffset = fmInfo.ImeLineCCoffset1;
    IME_D1_Para.out_Cr_lineoffset = IME_D1_Para.out_Cb_lineoffset;
    IME_D1_Para.out_format = fmInfo.ImeFmt1;


    //generator slice in/out size of each engine
    _InInfo.ImeOutH = IME_D1_Para.out_h;
    _InInfo.ImeOutV = IME_D1_Para.out_v;
    _InInfo.IpeInH = fmInfo.BayerHsize;
    _InInfo.IpeInV = fmInfo.BayerVsize;
    //_InInfo.ImeSliceH = IME_SLICE_H(_Cap);
    _InInfo.Sie_SliceH_Default = fmInfo.BayerVsize/3;
    _InInfo.IpeMode = IPE_AUTOSTP;
    _InInfo.ImeMode = IPE2IME_FRAME_MODE;
    SliceSize_Gen(&_OutInfo,_InInfo);
    IME_D1_Para_ext.ImeStripe.Ime_StripeMode = IME_STRIPE_USER_MODE;

    IpeHStripe.stripe.n = _OutInfo.Ipe_Hstripe.n;
    IpeHStripe.stripe.l = _OutInfo.Ipe_Hstripe.l;
    IpeHStripe.stripe.m = _OutInfo.Ipe_Hstripe.m;
    IpeHStripe.olap=IPE_OLAP_ON;
    IpeVStripe.stripe.n = _OutInfo.Ipe_Vstripe.n;
    IpeVStripe.stripe.l = _OutInfo.Ipe_Vstripe.l;
    IpeVStripe.stripe.m = _OutInfo.Ipe_Vstripe.m;
    IpeVStripe.olap=IPE_OLAP_ON;

    IME_D1_Para_ext.ImeStripe.Hstripe.n = _OutInfo.Ime_Hstripe.n;
    IME_D1_Para_ext.ImeStripe.Hstripe.l = _OutInfo.Ime_Hstripe.l;
    IME_D1_Para_ext.ImeStripe.Hstripe.m = _OutInfo.Ime_Hstripe.m;

    IME_D1_Para_ext.ImeStripe.Vstripe.n = _OutInfo.Ime_Vstripe.n;
    IME_D1_Para_ext.ImeStripe.Vstripe.l = _OutInfo.Ime_Vstripe.l;
    IME_D1_Para_ext.ImeStripe.Vstripe.m = _OutInfo.Ime_Vstripe.m;

    ime_SliceCfg(&IME_D1_Para,&IME_D2_Para,&IME_D1_Para_ext,&IME_D2_Para_ext,IPE_IME_FRAME_1PATH_JOB);
    Get_CapInfo(Cap_CFAPat, &IPECFAPat);

    if(GetIPPFuncInfor(IFE_EMBEDDED_EN))
        ife_EmbeddedInIpeCfg(IPE_IME_FRAME_1PATH_JOB,IPECFAPat);
    ipe_SliceCfg(&IpeHStripe,&IpeVStripe,fmInfo.BayerAddr,IPECFAPat, IPE_IME_FRAME_1PATH_JOB);

    #if IPP_DEBUG
    {
       char filename[64];

       sprintf(filename, "A:\\CAP_IPEIMEFrameMode_IPE.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_IPE);

       sprintf(filename, "A:\\CAP_IPEIMEFrameMode_IME.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_IME);
    }
    #endif

    ime_clearFlagFrameEnd();
    ime_setStart();
    ipe_start();
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
    ipe_pause();
    ime_pause();
    ipe_reset(ON);
    ipe_reset(OFF);
    ime_setReset();
    ipe_close();
    ife_close();
    ime_close();
}

UINT32 GetCurrentCapRawBuf(void)
{
    return SIE_PRE_Para_cap.SieOutAdd0;
}

//#NT#2010/12/31#ethanlau -begin
void ImgFQVProcess(ImageAlgInfo *AlgInfo,UINT32 SourceAddr)
{
    UINT32 YAddr, CbAddr, CrAddr;
    IPE_IME_FM_PARAM fmInfo;
    IME_PARAM OneIme;
    IME_FBADDR_PRAM _Ime_FB;
    IME_EXTEN _Ime_Ext;
    IME_OPENOBJ IMEOpenObj;
    UINT32 _Shift;

    //source
    fmInfo.BayerAddr = SourceAddr;
    fmInfo.BayerHsize = GetIPPSizeInfor(_SieCropCapHsize);
    fmInfo.BayerVsize = GetIPPSizeInfor(_SieCropCapVsize);
    fmInfo.BayerVsize = fmInfo.BayerVsize / 4 * 4;

    //dest
    fmInfo.ImeOutPath = IMEALG_1PATH;
    fmInfo.ImeHsize1 = GetIPPSizeInfor(_QVPreHsize);
    fmInfo.ImeVsize1 = GetIPPSizeInfor(_QVPreVsize);
    fmInfo.ImeFmt1 = GetIPPSizeInfor(_ImePath2Fmt);
    fmInfo.ImeLineoffset1 = GetIPPSizeInfor(_QVPreYLineOffset);
    fmInfo.ImeLineCCoffset1 = GetIPPSizeInfor(_QVPreUVLineOffset);

    switch(GetIPPSizeInfor(_ImePath2Fmt))
    {
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
            _Shift = 2;
            break;
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            _Shift = 4;
            break;
        default:
            _Shift = 1;
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }
    //image output address
    YAddr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    CbAddr = YAddr + (fmInfo.ImeLineoffset1 * (fmInfo.ImeVsize1 + GetIPPSizeInfor(_PreD2DOffsetLine)));
    CrAddr = CbAddr + fmInfo.ImeLineCCoffset1 * (fmInfo.ImeVsize1 + GetIPPSizeInfor(_PreD2DOffsetLine))*2/(_Shift);
    fmInfo.OutputAddr1Y = YAddr;
    fmInfo.OutputAddr1Cb = CbAddr;
    fmInfo.OutputAddr1Cr = CrAddr;

    //adjust image ratio
    if (AlgInfo->ImageRatio != _Ratio_4_3)
    {
        //get adjust ratio
        fmInfo.ImeVsize1 = Adj_VSize(fmInfo.ImeVsize1, Get_VRatio(AlgInfo->ImageRatio, _Ratio_4_3), 4);

        fmInfo.OutputAddr1Y += (GetIPPSizeInfor(_QVPreVsize)-fmInfo.ImeVsize1)*fmInfo.ImeLineoffset1/2;
        fmInfo.OutputAddr1Cb += (GetIPPSizeInfor(_QVPreVsize)-fmInfo.ImeVsize1)*fmInfo.ImeLineCCoffset1/(_Shift);
        fmInfo.OutputAddr1Cr += (GetIPPSizeInfor(_QVPreVsize)-fmInfo.ImeVsize1)*fmInfo.ImeLineCCoffset1/(_Shift);

        //up side
        memset((void *)YAddr, 0x0, fmInfo.OutputAddr1Y -YAddr);
        memset((void *)CbAddr, 0x80, fmInfo.OutputAddr1Cb-CbAddr);
        memset((void *)CrAddr, 0x80, fmInfo.OutputAddr1Cr -CrAddr);
        //lower
        memset((void *)(fmInfo.OutputAddr1Y + (fmInfo.ImeVsize1 * fmInfo.ImeLineoffset1)), 0x0, fmInfo.OutputAddr1Y -YAddr);
        memset((void *)(fmInfo.OutputAddr1Cb + (fmInfo.ImeVsize1 * fmInfo.ImeLineCCoffset1)*2/_Shift), 0x80, fmInfo.OutputAddr1Cb-CbAddr);
        memset((void *)(fmInfo.OutputAddr1Cr + (fmInfo.ImeVsize1 * fmInfo.ImeLineCCoffset1)*2/_Shift), 0x80, fmInfo.OutputAddr1Cr -CrAddr);
    }
    ipe_ime_Framemode(fmInfo);

//#NT#2010/12/31#ethanlau -begin
//mark post processing
#if 0
//#NT#2010/01/06#Bowen Li -begin
//#fix bug for QV image crop offset issue
    SetIPPFuncInfor(FQV_INPROCESS, ENABLE);

    OrgOVPreHSize = GetIPPSizeInfor(_QVPreHsize);
    OrgOVPreVSize = GetIPPSizeInfor(_QVPreVsize);
    SetIPPSizeInfor(_QVPreHsize, fmInfo.ImeHsize1);
    SetIPPSizeInfor(_QVPreVsize, fmInfo.ImeVsize1);

    //#NT#2010/08/09/#Linkin Huang -begin
    //#NT# integrate function interface
    PostProcInfo.AddrY = fmInfo.OutputAddr1Y;
    PostProcInfo.AddrCb = fmInfo.OutputAddr1Cb;
    PostProcInfo.AddrCr = fmInfo.OutputAddr1Cr;
    PostProcInfo.WidthY = fmInfo.ImeHsize1;
    PostProcInfo.HeightY = fmInfo.ImeVsize1;
    PostProcInfo.LineoffsetY = fmInfo.ImeLineoffset1;
    PostProcInfo.WidthCx  = (PostProcInfo.WidthY >> 1);
    PostProcInfo.LineoffsetCx = (PostProcInfo.LineoffsetY >> 1);
    //PostProcInfo.IpeD2DEn = IpeD2DEn;
    //PostProcInfo.YCCPostProcCB = IPED2DCB;
    if((fmInfo.ImeFmt1 == IME_OUTPUT_YCC_422_COS) || (fmInfo.ImeFmt1 == IME_OUTPUT_YCC_422_CEN))
    {
        PostProcInfo.HeightCx = PostProcInfo.HeightY;
    }
    else if((fmInfo.ImeFmt1 == IME_OUTPUT_YCC_420_COS) || (fmInfo.ImeFmt1 == IME_OUTPUT_YCC_420_CEN))
    {
        PostProcInfo.HeightCx = (PostProcInfo.HeightY >> 1);
    }
    else
    {
        debug_err(("-E- ImgFQVProcess unsupport format %d\r\n", fmInfo.ImeFmt1));
    }
    //ImgYCCPostProcess(fmInfo.OutputAddr1Y, fmInfo.OutputAddr1Cb, fmInfo.OutputAddr1Cr, fmInfo.ImeHsize1, fmInfo.ImeVsize1, fmInfo.ImeLineoffset1, AlgInfo);
    ImgYCCPostProcess(&PostProcInfo, AlgInfo);
    //#NT#2010/08/09/#Linkin Huang -end

    SetIPPSizeInfor(_QVPreHsize, OrgOVPreHSize);
    SetIPPSizeInfor(_QVPreVsize, OrgOVPreVSize);

    SetIPPFuncInfor(FQV_INPROCESS, DISABLE);
//#NT#2010/01/06#Bowen Li -end
#endif
//#NT#2010/12/31#ethanlau -end
    //scale to quickview(320x240 usually, not only scale,include data copy)
    _Ime_Ext.bil = IMEALG_BILINEAR;
    _Ime_FB.y0_addr = YAddr;
    _Ime_FB.cb0_addr = CbAddr;
    _Ime_FB.cr0_addr = CrAddr;
    _Ime_FB.in_h = GetIPPSizeInfor(_QVPreHsize);//fmInfo.ImeHsize1;
    _Ime_FB.in_v = GetIPPSizeInfor(_QVPreVsize);//fmInfo.ImeVsize1;
    _Ime_FB.in_lineoffset = fmInfo.ImeLineoffset1;
    _Ime_FB.in_Cb_lineoffset = fmInfo.ImeLineCCoffset1;
    _Ime_FB.in_Cr_lineoffset = fmInfo.ImeLineCCoffset1;
    switch(fmInfo.ImeFmt1)
    {
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
            _Ime_FB.in_format = IMEIN_FMT422;
            break;
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            _Ime_FB.in_format = IMEIN_FMT420;
            break;
        default:
            break;
    }


    //Target
    _Ime_FB.y1_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY);
    _Ime_FB.cb1_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY) + GetIPPSizeInfor(_QVLineOffset) * GetIPPSizeInfor(_QVVsize);
    _Ime_FB.cr1_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY) + GetIPPSizeInfor(_QVLineOffset) * GetIPPSizeInfor(_QVVsize)
        + GetIPPSizeInfor(_QVLineOffset) * GetIPPSizeInfor(_QVVsize)/_Shift;
    _Ime_FB.out_h = GetIPPSizeInfor(_QVHsize);
    _Ime_FB.out_v = GetIPPSizeInfor(_QVVsize);
    _Ime_FB.out_lineoffset = GetIPPSizeInfor(_QVLineOffset);
    _Ime_FB.out_Cb_lineoffset = GetIPPSizeInfor(_QVLineOffset)/2;
    _Ime_FB.out_Cr_lineoffset = GetIPPSizeInfor(_QVLineOffset)/2;
    _Ime_FB.out_format = GetIPPSizeInfor(_ImePath2Fmt);

    OneIme.pIme_Ext = &_Ime_Ext;
    OneIme.pIme_fbaddr = &_Ime_FB;
    IPPMgrReq.ImeParam1 = OneIme;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IMEOpenObj.FP_IMEISR_CB = NULL;

    ime_open(&IMEOpenObj);
    if ( IME_BeginIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_BeginIPP() error \n"));

    if ( IME_AllocatResourceIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_AllocatResourceIPP() error \n"));

    SenFlowFunc(IME_CAP_SETTING);
    if ( IME_ExecIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
    if ( IME_EndIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
    ime_close();
}
//#NT#2010/12/31#ethanlau -end

//#NT#2011/01/10#ethanlau -begin
//#NT#for decode dcout
UINT32 JPGScalarHandler(PJPGHEAD_DEC_CFG jdcfg_p, UINT32 *scalar)
{
    UINT32 ScalarLevel = 0;
    PImageAlgInfo _pimginfo;
    _pimginfo = UI_GetParameter();
    if (JPGIMG_SIZE[_pimginfo->IMGSize][0] / 8 >= GetIPPSizeInfor(_ScreenHsize))
    {
        ScalarLevel = 3;
    }
    else if (JPGIMG_SIZE[_pimginfo->IMGSize][0] / 4 >= GetIPPSizeInfor(_ScreenHsize))
    {
        ScalarLevel = 2;
    }
    else if (JPGIMG_SIZE[_pimginfo->IMGSize][0] / 2 >= GetIPPSizeInfor(_ScreenHsize))
    {
        ScalarLevel = 1;
    }
    *scalar = ScalarLevel;
    return 0;
}

//#NT# //only support 4:3 and 16:9
void DecodeDCout(UINT _SurAddr,UINT _TmpAddr,UINT _jpgsize,UINT OutAddr,UINT OutH,UINT OutV,UINT LineOffset)
{
    DECJPG_INFO DecJPGInfo={0};
    JPGHEAD_DEC_CFG DecJPGParam={0};
    IME_FBADDR_PRAM _Ime_FB;
    UINT _tmp = 0, _tmp2 = 0, _shift = 0;
    PImageAlgInfo _pimginfo;

    _pimginfo = UI_GetParameter();

    DecJPGInfo.pSrcAddr           = (UINT8*)_SurAddr;
    DecJPGInfo.pDstAddr           = (UINT8*)_TmpAddr;//(GetIPPBufInfor(_BUF_CAP_SLC_BUF)+JpgHeaderSize);//for jpgheader encode temp data
    DecJPGInfo.pJPGScalarHandler  = JPGScalarHandler;
    DecJPGInfo.jdcfg_p            = &DecJPGParam;
    DecJPGInfo.JPGFileSize        = _jpgsize;
    DecJPGInfo.IfDecodeThumb      = FALSE;
    switch(GetIPPSizeInfor(_ImePath1Fmt))
    {
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            DecJPGInfo.jdcfg_p->fileformat = JPGFormatYUV420;
            break;
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
        default:
            DecJPGInfo.jdcfg_p->fileformat = JPGFormatYUV422;
            break;
    }

    DecodeOneJPG(&DecJPGInfo);

    //scale from dcout to screennail
    //#NT#2012/06/06#Robin -begin
    //#NT#use ime to move data
    //if( (DecJPGInfo.jdcfg_p->imagewidth!=OutH)||(DecJPGInfo.jdcfg_p->imageheight!=OutV))
    {
        _Ime_FB.y0_addr = DecJPGInfo.uiOutAddrY;
        _Ime_FB.cb0_addr = DecJPGInfo.uiOutAddrCb;
        _Ime_FB.cr0_addr = DecJPGInfo.uiOutAddrCr;
        _Ime_FB.in_lineoffset = DecJPGInfo.jdcfg_p->lineoffsetY;
        _Ime_FB.in_Cb_lineoffset = DecJPGInfo.jdcfg_p->lineoffsetUV;
        _Ime_FB.in_Cr_lineoffset = DecJPGInfo.jdcfg_p->lineoffsetUV;

        switch(DecJPGInfo.jdcfg_p->fileformat)
        {
            case JPGFormatYUV420:
                _Ime_FB.in_format = IMEIN_FMT420;
                _shift=4;
                break;
            case JPGFormatYUV211:
            case JPGFormatYUV422:
            default:
                _shift=2;
                _Ime_FB.in_format = IMEIN_FMT422;
                break;
        }
        _Ime_FB.in_h=DecJPGInfo.jdcfg_p->imagewidth;
        _Ime_FB.in_v=DecJPGInfo.jdcfg_p->imageheight;

        _Ime_FB.out_format=GetIPPSizeInfor(_ImePath1Fmt);
        switch(_Ime_FB.out_format)
        {
            case IME_OUTPUT_YCC_420_COS:
            case IME_OUTPUT_YCC_420_CEN:
                _tmp = OutH*OutV/4;
                break;
            case IME_OUTPUT_YCC_422_COS:
            case IME_OUTPUT_YCC_422_CEN:
            default:
                _tmp = OutH*OutV/2;
                break;
        }
        _Ime_FB.y1_addr = OutAddr;
        _Ime_FB.cb1_addr = _Ime_FB.y1_addr + OutH*OutV;
        _Ime_FB.cr1_addr = _Ime_FB.cb1_addr + _tmp;
        _Ime_FB.out_h = OutH;
        _Ime_FB.out_v = OutV;
        if(_pimginfo->ImageRatio == _Ratio_16_9)
        {
            _tmp = _Ime_FB.out_h*9/16;
            _tmp = _tmp - _tmp%4;
            _Ime_FB.out_v = _tmp;
            _tmp2 = (OutV-_tmp)/2;
        }
        _Ime_FB.out_lineoffset = LineOffset;
        _Ime_FB.out_Cb_lineoffset = LineOffset/2;
        _Ime_FB.out_Cr_lineoffset = LineOffset/2;

        Exce_IME_D2D(&_Ime_FB);
    }
    //#NT#2012/06/06#Robin -end

    //add upper black and lower black,and need to copy data back to screennail&thumbnail,about 26ms
    if(_pimginfo->ImageRatio == _Ratio_16_9)
    {
        //copy from outaddr 2 tmpaddr
        //clean dram
        memset((void *)(_TmpAddr), 0x0, (OutV*OutH));//Y
        memset((void *)(_TmpAddr+(OutV*OutH)), 0x0, (2*OutV*OutH/_shift));//CbCr
        //copy 16:9 YCbCr
        memcpy((void *)(_TmpAddr+(_tmp2*OutH)),(void *)(_Ime_FB.y1_addr),(OutH*_tmp));
        memcpy((void *)(_TmpAddr+OutV*OutH+(_tmp2*OutH/_shift)), (void *)(_Ime_FB.cb1_addr),(OutH*_tmp/_shift));
        memcpy((void *)(_TmpAddr+OutV*OutH+OutV*OutH/_shift+(_tmp2*OutH/_shift)), (void *)(_Ime_FB.cr1_addr), (OutH*_tmp/_shift));

        //upper black
        memset((void *)(_TmpAddr), 0x0, (_tmp2*OutH));
        memset((void *)(_TmpAddr+OutV*OutH), 0x80, ((OutH/_shift)*_tmp2));
        memset((void *)(_TmpAddr+OutV*OutH+OutV*OutH/_shift), 0x80, ((OutH/_shift)*_tmp2));

        //lower black
        memset((void *)(_TmpAddr+(OutH*(_tmp2+_tmp))), 0x0, (_tmp2*OutH));
        memset((void *)(_TmpAddr+(OutH*(_tmp2+_tmp)/_shift)+OutV*OutH), 0x80, ((OutH/_shift)*_tmp2));
        memset((void *)(_TmpAddr+(OutH*(_tmp2+_tmp)/_shift)+OutV*OutH+OutV*OutH/_shift), 0x80, ((OutH/_shift)*_tmp2));

        //copy img from tmpaddr 2 outaddr
        memcpy((void*)(OutAddr),(void*)(_TmpAddr),(OutV*OutH+2*OutV*OutH/_shift));
    }

}
//#NT#2011/01/10#ethanlau -end
