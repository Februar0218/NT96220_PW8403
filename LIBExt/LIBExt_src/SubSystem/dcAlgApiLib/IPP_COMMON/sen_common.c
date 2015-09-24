/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Sensor Common Module                                               *
* Description:                                                             *
* Author: Adrian                                                           *
****************************************************************************/

/** \addtogroup SEN_COMMON_Lib */
//@{
#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "OSCommon.h"
#include "interrupt.h"
#include "IDE.h"
#include "Timer.h"
#include "pll.h"
#include "JpgEnc.h"
#include "JPEG.h"
#include "Grph.h"
#include "Top.h"
#include "Filesystsk.h"
#include "utility.h"
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ife_lib.h"
#include "ime_lib.h"
#include "aaa_lib.h"
#include "ImageEffect.h"
#include "math.h"
#include "IPPCtrl.h"
#include "IPPManager.h"
#include "sen_common.h"
#include "IQSetting.h"
#include "prv_common.h"
#include "Alg_IPLMode.h"
#include "SensorFlowFunc.h"
#include "SensorSys.h"
#include "IPPCtrl.h"
#include "IPP_Debug.h"
#include "sensor.h"
//#NT#2009/10/20#Adrian Jiang, Rearrange the code start. -begin
//#NT#Rearrange the code
//#NT#2009/10/20 remove the unused code -Adrian
//#define    IME_PATH_DRAW_VA_WINDOW    0
//#NT#2009/10/20#Adrian Jiang  -begin

//#NT#2009/10/20#Adrian Jiang, Rearrange the code start. -begin
//#NT#Rearrange the code
//#NT#2009/10/20 remove the unused code -Adrian
//unsigned long     AWB_NormRatio = (1<<AWBScaleBit);
//unsigned long     AF_NormRatio = (1<<AFScaleBit);
//#NT#2009/10/20#Adrian Jiang -end

UB PCC2CAP=0;
BOOL DZoom_move = FALSE;

//#NT#2009/10/12#Bowen Li -begin
//#NT#modify pre va interface
//BOOL AfScaleEn = DISABLE;
//#NT#2009/10/12#Bowen Li -end

FPMPEG4PutSrcImgIdxCallBack g_pMP4Callback = NULL;
//#NT#2009/10/20#Adrian Jiang, Rearrange the code start. -begin
//#NT#Rearrange the code
//#NT#2009/10/20 marke the unused code -Adrian
//UINT32  IPE_Vacc_TAb1[8][8];
//UINT32  IPE_Vacc_TAb2[8][8];
//UINT32  IPE_Vacc_TAb3[16][16];
//UINT32  IPE_Vacc_TAb4[16][16];
//UINT32  PRE_Vacc_TAb[5];
//UINT32  PRE_Vacc_TAb[256];
//#NT#2009/10/20#Adrian Jiang -end

//#NT#2009/10/20#Adrian Jiang, Rearrange the code start. -begin
//#NT#Rearrange the code

unsigned int ImeVideoFmt=0xff;

//#NT#2010/04/12#Connie Yu  -begin
//#NT#Remove FW nvtSR
//UINT16 OBLine[1280];
//#NT#2010/04/12#Connie Yu  -end

RGB_IMAGE_TIFF_HEADER RGB_TIFF_Header =
{
//TIFF header for RGB planar image
{ {0x4D,0x4D}, {0x00,0x2A}, {0x00,0x00,0x00,0x08} },//ImageFileHeader
{  0x00,0x0E                                      },//IFD_Entry_Number
{ {0x00,0xFE}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_NewSubFileType
{ {0x01,0x00}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_ImageWidth       (to be set)
{ {0x01,0x01}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_ImageLength      (to be set)
{ {0x01,0x02}, {0x00,0x03}, {0x00,0x00,0x00,0x03}, {0x00,0x00,0x00,0xB6} },//IFD_Entry_BitsPerSample    (offset value 1)
{ {0x01,0x03}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x01,0x00,0x00} },//IFD_Entry_Compression
{ {0x01,0x06}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x02,0x00,0x00} },//IFD_Entry_PhotometricInterpretation
{ {0x01,0x11}, {0x00,0x04}, {0x00,0x00,0x00,0x03}, {0x00,0x00,0x00,0xBC} },//IFD_Entry_StripOffsets     (offset value 2)
{ {0x01,0x15}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x03,0x00,0x00} },//IFD_Entry_SamplesPerPixel
{ {0x01,0x16}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_RowsPerStrip     (to be set)
{ {0x01,0x17}, {0x00,0x04}, {0x00,0x00,0x00,0x03}, {0x00,0x00,0x00,0xC8} },//IFD_Entry_StripByteCounts  (offset value 3)
{ {0x01,0x1A}, {0x00,0x05}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0xD4} },//IFD_Entry_XResolution      (offset value 4)
{ {0x01,0x1B}, {0x00,0x05}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0xDC} },//IFD_Entry_YResolution      (offset value 5)
{ {0x01,0x1C}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x02,0x00,0x00} },//IFD_Entry_PlanarConfiguration
{ {0x01,0x28}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x02,0x00,0x00} },//IFD_Entry_ResolutionUnit
{  0x00,0x00,0x00,0x00 },                                                  //NextIDFOffset
{ {0x00,0x08}, {0x00,0x08}, {0x00,0x08} },                                 //BitsPerSampleValue
{ {0x00,0x00,0x00,0xE4}, {0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00} },   //StripOffsetsValue          (to be set)
{ {0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00} },   //StripByteCounts            (to be set)
{  0x00,0x00,0x01,0x2C,0x00,0x00,0x00,0x01 },                              //XResolutionValue
{  0x00,0x00,0x01,0x2C,0x00,0x00,0x00,0x01 }                               //YResolutionValue
};

const UINT32 VA_9PixTable[31][2]=
{
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 0},
{35, 7},
{34, 0},
{33, 0},
{32, 0},
{29, 5},
{28, 3},
{27, 5},
{26, 3},
{24, 7},
{23, 6},
{22, 5},
{22, 0},
{22, 0},
{20, 7},
{19, 7},
{19, 7},
{19, 0},
{19, 0},
{17, 7},
{17, 7}
};
const UINT32 VA_Mod3Table[31][2]=
{
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 0},
{34, 5},
{34, 0},
{33, 0},
{31, 0},
{29, 5},
{28, 3},
{27, 5},
{26, 3},
{24, 7},
{23, 6},
{22, 5},
{22, 5},
{22, 0},
{20, 7},
{20, 7},
{19, 7},
{19, 0},
{19, 0},
{17, 7},
{17, 7}
};

//PSENSOR_APPOBJ  pSensorInfor;
//SENSOR_APPOBJ   SensorInfor;
PCAPTURE_APPOBJ pCapInfor;
CAPTURE_APPOBJ  CapInfor;


UINT    gDP_Add_Prv=NULL;
UINT    gDP_Add_Cap=NULL;

IPP_FUNC_INFOR gIPPFuncInfor = {
    DISABLE,       // 0  - DfsEn;
    DFS_FIRSTPIC,  // 1  - DfsCnt;
    DISABLE,       // 2  - SIED2DEn;
    DISABLE,       // 3  - NLightCapEn;
    NLCap_QVDataIn,// 4  - NLightCapSel;
    ENABLE,        // 5  - FQVEn;
    0,             // 6  - CapISO;
    DISABLE,       // 7  - ChrFiltEn;
    0,             // 8  - ChrFiltNum;
    DISABLE,       // 9  - FQVInProcess;
    DISABLE,       // 10 - ThumbImgInProcess;
    DISABLE,       // 11 - MainImgInProcess;
    DISABLE,       // 12 - ChrCurveEn;
    0,             // 13 - *ChrCBCurve;
    0,             // 14 - *ChrCRCurve;
    0,             // 15 - *CapGamma;
    DISABLE,       // 16 - YCurveEn;
    0,             // 17 - *YCurve;
    DISABLE,       // 18 - GEOEn;
    DISABLE,       // 19 - BRCEn;
    DISABLE,       // 20 - FrameMode;
    ENABLE,        // 21 - IfeEmbeddedEn;
    DISABLE,       // 22 - IfeRawEn;
    DISABLE,       // 23 - IfeYccEn;
    0,             // 24 - IfeFuncTalbeAddr;
    0,             // 25 - IfeRawTableAddr;
    0,             // 26 - IfeYccTableAddr;
    DISABLE,       // 27 - IFE_Y_EN
    0,             // 28 - IfeYTableAddr;
    DISABLE,       // 29 - EcsEn
    DISABLE,       // 30 - VidDatestampEn
    DISABLE,       // 31 - FQV_FRAMEMODE_EN
    DISABLE,       // 32 - JPG_DCOUT_EN
    DISABLE,       // 33 - JPG_EN
};

void GetActImgFBAddr_Path1(ACTFBADDR_PRAM *pActFBAddr_para)
{
    switch (ime_getPath1CurrentBufferIndex())
    {
        case 0:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CR);
        break;
        case 1:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CR);
        break;
        case 2:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CR);
            break;
        case 3:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_3,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_3,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_3,IME_CHANNEL_CR);
            break;
        case 4:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_4,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_4,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_4,IME_CHANNEL_CR);
            break;
        default:
            break;
    }
}
void GetActImgFBAddr_Path2(ACTFBADDR_PRAM *pActFBAddr_para)
{
    //#NT#2011/01/11#ethanlau -begin
    //#NT#only 1-path under 220
#if 1
    GetActImgFBAddr_Path1(pActFBAddr_para);
#else
    if(gImageAlgInfo.DISMode == _DISMode_On)
    {
        //#NT#2009/12/23#Connie Yu -begin
        //#for seperating display & recording calculation
        dis_accumRecProcess();
        dis_updateRecAddr(pActFBAddr_para);
        //#NT#2009/12/23#Connie Yu -end
    }
    else
    {
        //#NT#2010/09/08#Yuppie Lin -begin
        //#NT#Add for Preview IFE
        if(Get_PrvIfeFlag() == ENABLE)
        {
            switch (ime_getPath2CurrentBufferIndex())
            {
                case 1:
                    pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_Y);
                    pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CB);
                    pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CR);
                    break;

                case 2:
                    pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_Y);
                    pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CB);
                    pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CR);
                    break;

                case 0:
                    pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_Y);
                    pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CB);
                    pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CR);
                break;
            }
        }
        else
        {
            switch (ime_getPath2CurrentBufferIndex())
            {
                case 0:
                    pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_Y);
                    pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CB);
                    pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CR);
                    break;

                case 1:
                    pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_Y);
                    pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CB);
                    pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CR);
                    break;

                case 2:
                    pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_Y);
                    pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CB);
                    pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CR);
                break;
            }
        }
        //#NT#2010/09/08#Yuppie Lin -end
    }
    //debug_err(("fb2 0x%x 0x%x 0x%x\r\n",pActFBAddr_para->y_addr,pActFBAddr_para->cb_addr,pActFBAddr_para->cr_addr));
#endif
    //#NT#2011/01/11#ethanlau -end
}

void SetImeVideoFormat(UINT ImeFmt)
{
    ImeVideoFmt=ImeFmt;
}
UINT GetImeVideoFormat(void)
{
    if(IME_D2_Para.out_format != ImeVideoFmt)
        debug_err(("ime output format error: AVI & SensorLib sync error!"));

    return ImeVideoFmt;
}

//#NT#2010/12/29#ethanlau -begin
static UINT32 m_ImeInfo[ImeOutInfo_Max][IPP_MaxFlowdefine];
//base on buffer chk result
void Set_IME_OutInfo(UINT32 mode,ImeOutInfo Info,UINT32 value)
{
    if(Info == ImeOutInfo_FMT)
    {
        switch((IPPBUF_FMT)value)
        {
            case FMT444: value = IMEOUT2_FMT444;
                break;
            case FMT422: value = IMEOUT2_FMT422_1;
                break;
            case FMT420: value = IMEOUT2_FMT420_1;
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
                break;
        }
    }
    else if(Info == ImeOutInfo_BufferNum)
    {
        value = (_IME_PPB_NUM_SEL)value;
        if(value > IME_OUTPUT_PPB_5)
            debug_err(("%d %s\r\n",__LINE__,__func__));
    }
    else if(Info == ImeOutInfo_SliceH)
    {}
    else
        debug_err(("%d %s\r\n",__LINE__,__func__));

    switch((IPPCTRL_FLOW)mode)
    {
        case IPP_FrameMode_HW:
        case IPP_FrameMode_FW:
        case IPP_SliceMode_S2F:
        case IPP_SliceMode_S2S:
        case IPP_SliceMode_F2S:
            m_ImeInfo[Info][mode] = value;
            break;
        default:
            debug_err(("%d %d %s\r\n",__LINE__,mode,__func__));
            break;
    }
}

UINT32 Get_IME_OutInfo(UINT32 mode,ImeOutInfo Info)
{
    switch((IPPCTRL_FLOW)mode)
    {
        case IPP_FrameMode_HW:
        case IPP_FrameMode_FW:
        case IPP_SliceMode_S2F:
        case IPP_SliceMode_S2S:
        case IPP_SliceMode_F2S:
            return m_ImeInfo[Info][mode];
        default:
            debug_err(("%d %d %s\r\n",__LINE__,mode,__func__));
            return 0;
    }
}
//#NT#2010/12/29#ethanlau -end

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void DzoomStop(void)
{
    DZoom_move = FALSE;
}

BOOL Get_DzoomFlag(void)
{
    return DZoom_move;
}
void Set_DzoomFlag(BOOL CDZoom)
{
     DZoom_move = CDZoom;
}

void SetPCC2Cap(UINT *Addr, UINT *JPGsize)
{
#if 0
    UINT uiFlag;

    PCC2CAP=1;
    sta_tsk(SIECAPTSK_ID);
    sta_tsk(IMGSLICETSK_ID);

    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
    ImgCapture_SetParameter(_ImgSize, _IMGSize_5M);
    ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
    ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
    ImgCapture_SetParameter(_SiePath,_PostGamma);
    ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
    ImgCapture_StartCapture();
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_JPGCOMPLETE,TWF_ORW);
    clr_flg(FLG_ID_SLICE, FLGSLICETSK1_JPGCOMPLETE);
    *Addr=ippbuf_GetBuf(IPPBUF_CAP_JPG_1);//JpgOutAddr[0];
    *JPGsize=JPG_Size[0];
#endif
}
void SetPCC2CapRawJpg(UINT *rawAddr, UINT *rawWidth, UINT *rawHeight, UINT *rawBitDepth, UINT *jpgAddr, UINT *jpgSize)
{
#if 0
    UINT uiFlag;
    extern unsigned long JPG_TempAdd[3];

    PCC2CAP=1;
    clr_flg(FLG_ID_SLICE, 0xffffffff);
    sta_tsk(SIECAPTSK_ID);
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_SIECAPIDLE,TWF_ORW);
    sta_tsk(IMGSLICETSK_ID);
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_IDLE,TWF_ORW);
    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
    ImgCapture_SetParameter(_ImgSize,  _IMGSize_10M);
    ImgCapture_SetParameter(_FileFmt,  _FileFmt_RawJpg);
    ImgCapture_SetParameter(_PreCapBitDepth, _PreBitDepth_8);
    ImgCapture_SetParameter(_BurstNum,  _BurstNum_1);
    ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
    ImgCapture_StartCapture();
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_JPGCOMPLETE,TWF_ORW);
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_IDLE,TWF_ORW);

    *rawAddr=ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    *rawWidth=GetIPPSizeInfor(_SieCropCapHsize);
    *rawHeight=GetIPPSizeInfor(_SieCropCapVsize);
    *rawBitDepth=_PreBitDepth_8;
    *jpgAddr=JPG_TempAdd[0]  ;// ippbuf_GetBuf(IPPBUF_CAP_JPG_1);
    *jpgSize=JPG_Size[0];
    ter_tsk(SIECAPTSK_ID);
    ter_tsk(IMGSLICETSK_ID);
#endif

}

#if 0
#pragma mark -
#endif
void sie_Set2Capture(SIE_PRE_PARAM *SiePreParam)
{
    STR_HDVD _Str_HDVD;
    //removed, all pimux-select are ctrled by proj layer
    //pinmux_select_mes(PINMUX_MES0|PINMUX_MES1);
    //incase multiple pause when burst mode
    if(sie_checkFunctionEnable(SIE_CROP_EN))
    {
        sie_pause();
    }
    _Str_HDVD.HD_sync   = SIE_PRE_Para_prv.HDSync;
    _Str_HDVD.HD_total  = SIE_PRE_Para_prv.HDTotal;
    _Str_HDVD.VD_sync   = SIE_PRE_Para_prv.VDSync;
    _Str_HDVD.VD_total  = SIE_PRE_Para_prv.VDTotal;
    _Str_HDVD.HD_inv    = SIE_PRE_Para_prv.SieHDInv;
    _Str_HDVD.VD_inv    = SIE_PRE_Para_prv.SieVDInv;
    sie_setSensorSync(_Str_HDVD);

    sie_clrIntrStatus(SIE_INT_ALL);
    sie_enableInt(DISABLE,SIE_INTE_ALL);
    sie_clrIntrStatus(SIE_INT_ALL);

//    if(SensorInfor.FP_GPIOMap_CloseShutter!=NULL)
//    {
//        SensorInfor.FP_GPIOMap_CloseShutter();
//    }

    IPPMgrReq.SIE_PRE_CAP = SiePreParam;
    sie_setBreakPoint(SiePreParam->SieBp1, SiePreParam->SieBp2);
    sie_load();

    if ( Sensor_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    SenFlowFunc(SIE_CAP_SETTING);

    if ( SIE_AllocatResourceIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_AllocatResourceIPP() error \n"));

    SenFlowFunc(SIE_PRE_GET_CAP_RAW);
//    if(SensorInfor.FP_GPIOMap_OpenShutter!=NULL)
//    {
//        SensorInfor.FP_GPIOMap_OpenShutter();
//    }

}

/**
    Stop the IPP DRAM access when "the IPP are running"(Important).
    If the IPP are not running,it should not be called or it will hang at sie_waitVD().

    @param void
    @return void
*/
void StopPreviewDramData(StopIplStage _stage)
{
/*
    UINT32 uiPixel[2] = {0},i,bCount = FALSE;
*/
    switch(_stage)
    {
        case _Start:
            debug_msg("%s:",__func__);
            //#NT#2011/12/13#Jarkko Chang -begin
            //#NT#only use SIE crop. sensor must have signal when pipleline is running
/*
            //SIE Pclk counter will be clamp in max value(0xfff<<2) when stop in blanking zone
            //sie pclk counter only refresh by H-sync
            uiPixel[0] = sie_getPixelCounter();
            TimerDelayUs(1);
            uiPixel[1] = sie_getPixelCounter();

            if( (uiPixel[0] == uiPixel[1]) && (uiPixel[0]== (0xfff<<2)) )
            {
                //ethan_20110803
                for(i=0;i<500;i++)
                {
                    TimerDelayMs(i%5);
                    if(i==0)
                    {
                        if(uiPixel[0] != sie_getPixelCounter())
                        {
                            bCount = TRUE;
                            break;
                        }
                        //uiPixel[i] = sie_getPixelCounter();
                    }
                    else
                    {
                        uiPixel[1] = sie_getPixelCounter();
                        if(uiPixel[0] != uiPixel[1])
                        {
                            bCount = TRUE;
                            break;
                        }
                    }
                }
            }
            else if(uiPixel[0] == uiPixel[1])//pclk cnt have no chg && cnt != max
            {
                bCount = FALSE;
            }
            else
            {
                bCount = TRUE;
            }
            if ((bCount == FALSE) ||
                (sie_checkFunctionEnable(SIE_CROP_EN) == 0))
*/
            //#NT#2011/12/13#Jarkko Chang -end
            if(sie_checkFunctionEnable(SIE_CROP_EN) == 0)
            {
                debug_msg("Sie already stop!\r\n");
                return ;
            }
            IPLTriggerStop();
            debug_msg("_Start ");
            break;
         case _IPEIME_PAUSE:
            ime_pause();
            ipe_pause();
            debug_msg("_IPEIME_P ");
            break;
         case _SIE_PAUSE:
            sie_pause();
            debug_msg("_SIE_P ");
            break;
         case _IPE_IME_RESET://reset need sie_pause() vd latch
            ipe_reset(ON);
            ipe_reset(OFF);
            ime_setReset();
            debug_msg("_IPEIME_R \r\n");
            break;
         default:
            //debug_err(("use old way to stop IPL\r\n"));
            debug_msg("%s: \r\n",__func__);
            //#NT#2011/12/13#Jarkko Chang -begin
            //#NT#only use SIE crop. sensor must have signal when pipleline is running
/*
            uiPixel[0] = sie_getPixelCounter();
            TimerDelayUs(1);
            uiPixel[1] = sie_getPixelCounter();
            if( (uiPixel[0] == uiPixel[1]) && (uiPixel[0]== (0xfff<<2)) )
            {
                for(i=0;i<100;i++)
                {
                    TimerDelayMs(5);
                    if(i==0)
                    {
                        if(uiPixel[0] != sie_getPixelCounter())
                        {
                            bCount = TRUE;
                            break;
                        }
                        uiPixel[i] = sie_getPixelCounter();
                    }
                    else
                    {
                        uiPixel[i] = sie_getPixelCounter();
                        if(uiPixel[i] != uiPixel[i-1])
                        {
                            bCount = TRUE;
                            break;
                        }
                    }
                }
            }
            else if(uiPixel[0] == uiPixel[1])//pclk cnt have no chg && cnt != max
            {
                bCount = FALSE;
            }
            else
            {
                bCount = TRUE;
            }

            if ((bCount == FALSE) ||
                (sie_checkFunctionEnable(SIE_CROP_EN) == 0))
*/
            //#NT#2011/12/13#Jarkko Chang -end
            if (sie_checkFunctionEnable(SIE_CROP_EN) == 0)
            {
                debug_msg("Sie already stop!\r\n");
                return ;
            }
            //sync
            ime_waitFlagFrameEnd(IME_CLEAR_FLAG);

            ipe_pause();
            ime_pause();
            ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
            //disable sensor data & pre
            sie_pause();
            sie_waitVD(1);

            ipe_reset(ON);
            ipe_reset(OFF);
            ime_setReset();
            break;
    }
}

/**
    Restart the IPP DRAM access when the IPP had been stop by calling StopPreviewDramData().
    The IPP register setting should be set properly before calling StartPreviewDramData().

    @param void
    @return void
*/
void StartPreviewDramData(void)
{
    if (sie_checkFunctionEnable(SIE_CROP_EN) == 0)
    {
        ime_setStart();
        ipe_start();
        sie_start();
        sie_waitVD(1);//sie start latch
        sie_waitVD(1);//sie first frame
        //ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
    }

}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void SetupMPEG4CallBackFun(FPMPEG4PutSrcImgIdxCallBack pmp4callback)
{
    g_pMP4Callback = pmp4callback;
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
//#NT#2011/01/10#ethanlau -begin
//#NT#get ime ext info
void GetIMEExtInfo(pIME_EXTEN _Ime1_ext,pIME_EXTEN _Ime2_ext,JobTypeEnum JobType)
{
    if(JobType == CAPTURE_JOB)
    {
        _Ime1_ext->bil = IMEALG_BILINEAR;
        _Ime1_ext->Bp1 = _Ime1_ext->Bp2 = 0;
        _Ime1_ext->ImeInten = IME_INTE_SLICE_DN1|IME_INTE_FRM;
        _Ime1_ext->ImeStripe.Ime_StripeMode = IME_STRIPE_USER_MODE;
        _Ime1_ext->Ime_BuffNum = Get_IME_OutInfo(IPP_SliceMode_F2S, ImeOutInfo_BufferNum);//SLICE_BUF_NUM(_Cap);
        _Ime1_ext->op_mode = IPE2IME_SLICE_MODE;
        _Ime1_ext->p1_out_slice_height = GetIPPSizeInfor(_IMESliceModeH);
        _Ime1_ext->path_num = IMEALG_1PATH;
    }
    //reason of useing _Ime1_ext is because of ippmanager
    else if(JobType == IPE_IME_FRAME_1PATH_JOB)
    {
        _Ime1_ext->bil = IMEALG_BILINEAR;
        _Ime1_ext->Bp1 = _Ime1_ext->Bp2 = 0;
        _Ime1_ext->ImeInten = IME_INTE_FRM;
        _Ime1_ext->ImeStripe.Ime_StripeMode = IME_STRIPE_USER_MODE;
        _Ime1_ext->Ime_BuffNum = Get_IME_OutInfo(IPP_FrameMode_FW, ImeOutInfo_BufferNum);
        _Ime1_ext->op_mode = IPE2IME_FRAME_MODE;
        //pIME_Para1->p1_out_slice_height = 0; don't care when ime in frame mode
        _Ime1_ext->path_num = IMEALG_1PATH;
    }
    else
    {
        debug_err(("unsupport flow\r\n"));
    }
}
//#NT#2011/01/10#ethanlau -end
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------

//#NT#2009/10/28#Yuppie Lin -begin
//#NT#use ime_triggerSlice() in IME driver
#if 0
void TrgImeSlice(void)
{
    ime_Slice_En(ON);
}
#endif
//#NT#2009/10/28#Yuppie Lin -end

void ipe_SliceCfg(IPEStripeInfo *Hstripe,IPEStripeInfo *Vstripe,UW InAddr0,UINT cfapat,JobTypeEnum Job)
{
    PImageAlgInfo _pimginfo;
    UINT32 ifeRepeat = 0;
    _pimginfo = UI_GetParameter();
    IPE_Para.HStripe = *Hstripe;
    IPE_Para.VStripe = *Vstripe;
    IPE_Para.Add0 = InAddr0;
    IPE_Para.CfaPat = cfapat;
    IPE_Para.StripeMode = IPE_AUTOSTP;

    IPE_Para.InSizeH = GetIPPSizeInfor(_IpeCapOutHsize)+IPE_HSIZE_IODIFF;
    IPE_Para.InSizeV = GetIPPSizeInfor(_IpeCapOutVsize);
    //#NT#2011/12/05#Ethan Lau -begin
    //#NT#patch sie 10bit-d2d burst mode flow
    if(GetIPPFuncInfor(IFE_RAW_EN))
    {
        UINT32 i=0;
        pIFE_FILTER     pife_para = NULL;
        pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_RAW_TABLE_ADDR);
        for(i=0; pife_para[i].repeat != 0xFF; i++)
        {
            ifeRepeat += pife_para[i].repeat;
        }
    }
    if(GetIPPFuncInfor(SIED2D_EN))
    {
        IPE_Para.InLineOfs = (GetIPPSizeInfor(_SieOutLineOffset) - ifeRepeat*IFE_OVERLAP_H)
                                            * _SieBitDepth_8/ _pimginfo->SieCapBitDepth;
        IPE_Para.InLineOfs += (ifeRepeat*IFE_OVERLAP_H);
        IPE_Para.BitDepth = IPE_PACK_08bit;
    }
    else
    {
        IPE_Para.InLineOfs = GetIPPSizeInfor(_SieOutLineOffset);
        switch(_pimginfo->SieCapBitDepth)
        {
            case _SieBitDepth_10 :
                IPE_Para.BitDepth = IPE_PACK_10bit;
                debug_msg("warm: capflow don't support 10bit ipe in so far\r\n");
                break;
            case _SieBitDepth_8 :
                IPE_Para.BitDepth = IPE_PACK_08bit;
                break;
            default:
                debug_err(("Cap: ipe bitdepth error!"));
                break;
        }
    }
    //#NT#2011/12/05#Ethan Lau -end
    IPPMgrReq.IpeParam = &IPE_Para;

    if ( IPE_BeginIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \n"));
    //#NT#2011/01/10#ethanlau -begin
    //#NT#add IFE condition
    if(GetIPPFuncInfor(IFE_EMBEDDED_EN))
    {
        IPE_Para.IFEembed_en = TRUE;
    }
    else
    {
        IPE_Para.IFEembed_en = FALSE;
    }

    //#NT#2011/01/10#ethanlau -end
    SenFlowFunc(IPE_CAP_SETTING);

    if ( IPE_ExecIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_ExecIPP() error \n"));
}

void ime_SliceCfg(IME_FBADDR_PRAM *pIME_Para1,IME_FBADDR_PRAM *pIME_Para2,pIME_EXTEN pIME_Ext1,pIME_EXTEN pIME_Ext2,JobTypeEnum Job)
{
    IPPMgrReq.ImeParam1.pIme_fbaddr = pIME_Para1;
    IPPMgrReq.ImeParam2.pIme_fbaddr = pIME_Para2;
    IPPMgrReq.ImeParam1.pIme_Ext = pIME_Ext1;
    IPPMgrReq.ImeParam2.pIme_Ext = pIME_Ext2;

    if (IME_BeginIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \n"));

    if (IME_AllocatResourceIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \n"));

    SenFlowFunc(IME_CAP_SETTING);
    if (IME_ExecIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
}

//#NT#2011/01/21#ethanlau -begin
/**
D2D api,need not include ife open and close
*/
ER ImageFilter(IMG_FILTER_PARAM *FilterParam)
{
    IFE_ENG_SETTING ife_param;
    pIFE_FILTER     pife_para;
    UINT32          i=0, array=0,allrepeat=0,cnt=1;

    ife_param.mode = IFE_MODE_D2D;
    //get ife filter quality related param.
    if (FilterParam->Mode == IFE_RAW)
    {
        ife_param.srcFmt = IFE_FMT_RAW;
        Get_CapInfo(Cap_CFAPat, (UINT*)&ife_param.cfaPat);
        SenFlowFunc(IFE_RAW_SETTING);
        pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_RAW_TABLE_ADDR);
        allrepeat=0;
        for(i=0; pife_para[i].repeat != 0xFF; i++)
        {
            allrepeat += pife_para[i].repeat;
        }
    }
    else if (FilterParam->Mode == IFE_YCC)
    {
        ife_param.srcFmt = IFE_FMT_NORMAL;
        SenFlowFunc(IFE_YCC_SETTING);
        pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_YCC_TABLE_ADDR);
        allrepeat=0;
        for(i=0; pife_para[i].repeat != 0xFF; i++)
        {
            allrepeat += pife_para[i].repeat;
        }
    }
    else if (FilterParam->Mode == IFE_Y)
    {
        ife_param.srcFmt = IFE_FMT_NORMAL;
        SenFlowFunc(IFE_Y_SETTING);
        pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_Y_TABLE_ADDR);
        allrepeat=0;
        for(i=0; pife_para[i].repeat != 0xFF; i++)
        {
            allrepeat += pife_para[i].repeat;
        }
    }
    else
    {
        debug_err(("^R IFE mode set error\r\n"));
        return E_PAR;
    }
    if(allrepeat < 1)
    {
        debug_err(("^R IFE repeat time must >=1 when IFE D2D enabled\r\n"));
        allrepeat = 1;
    }

    ife_param.up_addr = FilterParam->up_addr;
    ife_param.down_addr = FilterParam->down_addr;
    if(ife_param.up_addr==0x0)
        ife_param.upSrcEn = DISABLE;
    else
    {
        ife_param.upSrcEn = ENABLE;
        //cal. ife real start addr of upside
        //up_addr is the end addr of upside boundry
        ife_param.up_addr = ife_param.up_addr - allrepeat * IFE_OVERLAP_V * FilterParam->InLineOfs + 0x4;
    }
    if(ife_param.down_addr==0x0)
        ife_param.downSrcEn = DISABLE;
    else
    {
        ife_param.downSrcEn = ENABLE;
    }

    ife_param.in_addr = FilterParam->ImgAddr;
    ife_param.out_addr = FilterParam->BufAddr + (allrepeat-1)*IFE_OVERLAP_H;//cause ife output will shift left when in/out overlap
    ife_param.ofso = FilterParam->OutLineOfs;
    ife_param.height = FilterParam->VSize;
    ife_param.width = FilterParam->HSize;
    ife_param.ofsi = FilterParam->InLineOfs;

    //copy src data to IFE buf(slice data), ensure data to be continuous
    if((ife_param.upSrcEn==ENABLE) || (ife_param.downSrcEn==ENABLE))
    {
        //chg size
        ife_param.ofso += (allrepeat-1)*IFE_OVERLAP_H;

        grph_open();
        grph_init(PLL_CLKSEL_GRAPHIC_160);
        grph_enableDMA();
        //upside overlap line
        grph_setImg1(   ife_param.up_addr,
                        ife_param.ofsi,
                        allrepeat * IFE_OVERLAP_V,
                        ife_param.width);
        grph_setImg2(   ife_param.out_addr,
                        ife_param.ofso);
        grph_setAOP(    0,  GRPH_DST_2, GRPH_AOP_A_COPY,    0);

        //src img
        grph_setImg1(   ife_param.in_addr,
                        ife_param.ofsi,
                        ife_param.height,
                        ife_param.width);
        grph_setImg2(   ife_param.out_addr + (allrepeat * IFE_OVERLAP_V * ife_param.ofso),
                        ife_param.ofso);
        grph_setAOP(    0,  GRPH_DST_2, GRPH_AOP_A_COPY,    0);

        //downside overlap line
        grph_setImg1(   ife_param.down_addr,
                        ife_param.ofsi,
                        allrepeat * IFE_OVERLAP_V,
                        ife_param.width);
        grph_setImg2(   ife_param.out_addr + (allrepeat*IFE_OVERLAP_V + ife_param.height)* ife_param.ofso,
                        ife_param.ofso);
        grph_setAOP(    0,  GRPH_DST_2, GRPH_AOP_A_COPY,    0);

        grph_disableDMA();
        grph_swReset();
        grph_close();

        //chg size,cause data addr is chg. from src 2 ife buf. Condition is chged
        if((ife_param.upSrcEn==ENABLE) && (ife_param.downSrcEn==ENABLE))
            ife_param.height += (allrepeat-1)*2*IFE_OVERLAP_V;
        else
            ife_param.height += (allrepeat-1)*IFE_OVERLAP_V;
        ife_param.ofsi = ife_param.ofso;
        //chg input addr from src img to ife buf
        if(ife_param.upSrcEn==ENABLE)
        {
            ife_param.up_addr = ife_param.out_addr;
            //input img is not only src img, including boundrys if repeat time is more than 1
            ife_param.in_addr = ife_param.out_addr + (IFE_OVERLAP_V * ife_param.ofso);
            //last 2 line start addr
            if(ife_param.downSrcEn==ENABLE)
                ife_param.down_addr = ife_param.out_addr + (IFE_OVERLAP_V + ife_param.height )* ife_param.ofso;
        }
        else
        {
            ife_param.in_addr = ife_param.out_addr + (IFE_OVERLAP_V* allrepeat * ife_param.ofso);
            //last 2 line start addr
            ife_param.down_addr = ife_param.out_addr +
            (IFE_OVERLAP_V + ife_param.height + (allrepeat-1)*IFE_OVERLAP_V)* ife_param.ofso;//cause img height have no up-boundry
        }
        //shift output addr left 1 word,cause ife d2d hw design under buf overlap case
        ife_param.out_addr = ife_param.in_addr - IFE_OVERLAP_H;

    }
    ife_param.binn = IFE_BINNING_DISABLE;
    ife_param.intrEn = IFE_INTE_FRMEND;
    ife_param.bp1 = 0;

    array = 0;
    ife_param.IFE_filter = pife_para[array];
    IPPMgrReq.pIfeParam = &ife_param;
    IFE_BeginIPP(&IPPMgrReq, IFE_D2D);
    IFE_AllocatResourceIPP(&IPPMgrReq, IFE_D2D);

    for(i = 0;i < allrepeat;i++)
    {
        //chg ife filter param.
        if(cnt>ife_param.IFE_filter.repeat)
        {
            array++;
            cnt =1 ;
            ife_param.IFE_filter = pife_para[array];
            if(ife_param.IFE_filter.repeat == 0xFF)
                debug_err(("^R %s: ife filter param. error!",__func__));
        }
        if(i == (allrepeat - 1))
        {
            if(FilterParam->pCB != NULL)
            {
                //copy clean data to IPPBUF_CAP_IFESLICE_SRC_Y/IPPBUF_CAP_IFESLICE_SRC_CC
                FilterParam->pCB(
                    FilterParam->ImgAddr+ (FilterParam->VSize - allrepeat * IFE_OVERLAP_V)*FilterParam->InLineOfs,
                    FilterParam->InLineOfs,
                    allrepeat * IFE_OVERLAP_V,
                    FilterParam->Mode
                );
            }
            if((ife_param.upSrcEn==ENABLE) || (ife_param.downSrcEn==ENABLE))//src is slice data
                ife_param.out_addr = FilterParam->ImgAddr;//last repeat time,ife will output to src addr
            else//src is frame data
            {
                //do nothing
            }
            ife_param.ofso = FilterParam->OutLineOfs;
            IFE_BeginIPP(&IPPMgrReq, IFE_D2D);//cfg in/out addr
            IFE_AllocatResourceIPP(&IPPMgrReq, IFE_D2D);//chg ife filter param
        }
        IFE_ExecIPP(&IPPMgrReq, IFE_D2D);
        #if IPP_DEBUG
        {
            char filename[64];
            if (FilterParam->Mode == IFE_RAW)
                sprintf(filename, "A:\\IFE_RAW_%d.bin",(UINT)(i+1));
            else if (FilterParam->Mode == IFE_Y)
                sprintf(filename, "A:\\IFE_Y_%d.bin",(UINT)(i+1));
            else if (FilterParam->Mode == IFE_YCC)
                sprintf(filename, "A:\\IFE_YCC_%d.bin",(UINT)(i+1));
            else
                sprintf(filename, "A:\\none.bin");
            IppDebug_WriteEngineRegInfo(filename, IppDebug_IFE);
        }
        #endif
        if(i == (allrepeat - 1))
        {
            //do nothing
        }
        else
        {
            ImageFilterWaitEnd();

            //chg in/out addr
            //when src data is a frame
            ife_param.in_addr = ife_param.out_addr;
            ife_param.out_addr -= IFE_OVERLAP_H;
            //when src data is slice data
            if((ife_param.upSrcEn==ENABLE) || (ife_param.downSrcEn==ENABLE))
            {
                if(ife_param.upSrcEn==ENABLE)
                {
                    ife_param.out_addr += IFE_OVERLAP_V * ife_param.ofso;
                    ife_param.in_addr += IFE_OVERLAP_V * ife_param.ofso;
                    ife_param.up_addr -= IFE_OVERLAP_H;
                    ife_param.up_addr += IFE_OVERLAP_V * ife_param.ofso;
                }
                if(ife_param.downSrcEn==ENABLE)
                {
                    ife_param.down_addr -= IFE_OVERLAP_V * ife_param.ofso;
                    ife_param.down_addr -= IFE_OVERLAP_H;
                }
                if((ife_param.upSrcEn==ENABLE) && (ife_param.downSrcEn==ENABLE))
                    ife_param.height -= 2*IFE_OVERLAP_V;
                else
                    ife_param.height -= IFE_OVERLAP_V;
            }

            IFE_BeginIPP(&IPPMgrReq, IFE_D2D);//cfg in/out addr
            IFE_AllocatResourceIPP(&IPPMgrReq, IFE_D2D);//chg ife filter param
        }
        cnt++;
    }

    if (FilterParam->WaitEnd == ENABLE)
        ImageFilterWaitEnd();

    return E_OK;
}
//#NT#2011/01/21#ethanlau -end

ER ImageFilterWaitEnd(void)
{
    ife_waitFlagFrameEnd();
    //ife_close();
    return E_OK;
}

void ife_EmbeddedInIpeCfg(JobTypeEnum Job,_IFE_CFASEL Cfa)
{
    IFE_ENG_SETTING ife_param;
    ife_param.mode = IFE_MODE_EMBIPE;
    ife_param.cfaPat = Cfa;
    IPPMgrReq.pIfeParam = &ife_param;

    if ( IFE_BeginIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IFE_BeginIPP() error \r\n"));

    switch(Job)
    {
        case PREVIEW_JOB:
            SenFlowFunc(IFE_PRV_SETTING);
            break;
        case VIDEO_JOB:
            SenFlowFunc(IFE_VIDREC_SETTING);
            break;
        case CAPTURE_JOB:
        case IPE_IME_FRAME_1PATH_JOB:
            SenFlowFunc(IFE_CAP_SETTING);
            break;
        default:
            debug_err(("ife_embedded do not support this job!\r\n"));
            break;
    }

    #if IPP_DEBUG
    {
       char filename[64];

       sprintf(filename, "A:\\RegBin\\IFE_IPE_RAW.bin");
       IppDebug_WriteEngineRegInfo(filename, IppDebug_IFE);
    }
    #endif

}

//#NT#2011/02/08#ethanlau -begin
//#NT#Change ime in/out size
void Ime_ChgIOSize(pIME_FBADDR_PRAM pIme_fb,pIME_EXTEN pIme_ext)
{
    IME_HV_STRIPE_INFO _ImeStripeInfo;
    IME_IO_CHG_SIZE _ImeSize;

    _ImeSize.p1_out_addr.oPPBNums = pIme_ext->Ime_BuffNum;
    switch(_ImeSize.p1_out_addr.oPPBNums)
    {
        case IME_OUTPUT_PPB_5:
            _ImeSize.p1_out_addr.oDMA4.y_addr = pIme_fb->y4_addr;
            _ImeSize.p1_out_addr.oDMA4.cb_addr = pIme_fb->cb4_addr;
            _ImeSize.p1_out_addr.oDMA4.cr_addr = pIme_fb->cr4_addr;
        case IME_OUTPUT_PPB_4:
            _ImeSize.p1_out_addr.oDMA3.y_addr = pIme_fb->y3_addr;
            _ImeSize.p1_out_addr.oDMA3.cb_addr = pIme_fb->cb3_addr;
            _ImeSize.p1_out_addr.oDMA3.cr_addr = pIme_fb->cr3_addr;
        case IME_OUTPUT_PPB_3:
            _ImeSize.p1_out_addr.oDMA2.y_addr = pIme_fb->y2_addr;
            _ImeSize.p1_out_addr.oDMA2.cb_addr = pIme_fb->cb2_addr;
            _ImeSize.p1_out_addr.oDMA2.cr_addr = pIme_fb->cr2_addr;
        case IME_OUTPUT_PPB_2:
            _ImeSize.p1_out_addr.oDMA1.y_addr = pIme_fb->y1_addr;
            _ImeSize.p1_out_addr.oDMA1.cb_addr = pIme_fb->cb1_addr;
            _ImeSize.p1_out_addr.oDMA1.cr_addr = pIme_fb->cr1_addr;
        case IME_OUTPUT_PPB_1:
            _ImeSize.p1_out_addr.oDMA0.y_addr = pIme_fb->y0_addr;
            _ImeSize.p1_out_addr.oDMA0.cb_addr = pIme_fb->cb0_addr;
            _ImeSize.p1_out_addr.oDMA0.cr_addr = pIme_fb->cr0_addr;
            break;
    }
    _ImeSize.in_size.h_size = pIme_fb->in_h;
    _ImeSize.in_size.v_size = pIme_fb->in_v;
    _ImeSize.line_ofs.in_lineofs.LineOffsetY = pIme_fb->in_lineoffset;
    _ImeSize.line_ofs.in_lineofs.LineOffsetCb = pIme_fb->in_Cb_lineoffset;
    _ImeSize.line_ofs.in_lineofs.LineOffsetCr = pIme_fb->in_Cr_lineoffset;

    _ImeSize.p1_out_size.h_size = pIme_fb->out_h;
    _ImeSize.p1_out_size.v_size = pIme_fb->out_v;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetY = pIme_fb->out_lineoffset;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetCb = pIme_fb->out_Cb_lineoffset;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetCr = pIme_fb->out_Cr_lineoffset;

    //IME h stripe
    _ImeStripeInfo.uHN = pIme_ext->ImeStripe.Hstripe.n;
    _ImeStripeInfo.uHL = pIme_ext->ImeStripe.Hstripe.l;
    _ImeStripeInfo.uHM = pIme_ext->ImeStripe.Hstripe.m;
    //IME v stripe
    _ImeStripeInfo.uVN = pIme_ext->ImeStripe.Vstripe.n;
    _ImeStripeInfo.uVL = pIme_ext->ImeStripe.Vstripe.l;
    _ImeStripeInfo.uVM = pIme_ext->ImeStripe.Vstripe.m;
    //IME stripe mode
    _ImeStripeInfo.StripeCalMode = pIme_ext->ImeStripe.Ime_StripeMode;
    ime_setUserStripeInfo(&_ImeStripeInfo);

    ime_chgParam((void*)&_ImeSize,IME_CHG_IO_SIZE);
}

//#NT#Cal. NLM size when ipe/ipe+ime slice mode in-out size have been changed
void IPL_SliceSize(IPP_PARAM* pIPP_param,_IMEALGSETPATH Path)
{
    SliceSizeInfo _OutInfo;
    SliceGenIn _InInfo;
    pIPE_PARAM pIpe_param;
    pIME_FBADDR_PRAM pIme_fb;
    pIME_EXTEN pIme_exten;

    pIpe_param = pIPP_param->pIpe_Struct;
    //#NT#2011/02/15#ethanlau -begin
    //#NT#220 have no path2, only use vid or prv to distinguish
    if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
    {
        pIme_fb = pIPP_param->Ime_Struct2.pIme_fbaddr;
        pIme_exten = pIPP_param->Ime_Struct2.pIme_Ext;
    }
    else
    {
        pIme_fb = pIPP_param->Ime_Struct.pIme_fbaddr;
        pIme_exten = pIPP_param->Ime_Struct.pIme_Ext;
    }
    //#NT#2011/02/15#ethanlau -end
    _InInfo.ImeOutH = pIme_fb->out_h;
    _InInfo.ImeOutV = pIme_fb->out_v;
    _InInfo.IpeInH = pIpe_param->InSizeH;
    _InInfo.IpeInV = pIpe_param->InSizeV;
    _InInfo.ImeSliceH = pIme_exten->p1_out_slice_height;
    _InInfo.Sie_SliceH_Default = pIPP_param->Sie_Struct->SieSliceHight;
    _InInfo.IpeMode = pIpe_param->StripeMode;
    _InInfo.ImeMode = pIme_exten->op_mode;
    SliceSize_Gen(&_OutInfo, _InInfo);
    //IME H
    pIme_exten->ImeStripe.Hstripe.n = _OutInfo.Ime_Hstripe.n;
    pIme_exten->ImeStripe.Hstripe.l = _OutInfo.Ime_Hstripe.l;
    pIme_exten->ImeStripe.Hstripe.m = _OutInfo.Ime_Hstripe.m;
    //IME V
    pIme_exten->ImeStripe.Vstripe.n = _OutInfo.Ime_Vstripe.n;
    pIme_exten->ImeStripe.Vstripe.l = _OutInfo.Ime_Vstripe.l;
    pIme_exten->ImeStripe.Vstripe.m = _OutInfo.Ime_Vstripe.m;

    pIme_exten->ImeStripe.Ime_StripeMode = IME_STRIPE_USER_MODE;

    pIPP_param->Sie_Struct->SieSliceHight = _OutInfo.Sie_SliceH;
    pIpe_param->IpeSliceH = _OutInfo.Sie_SliceH;
    //IPE H
    pIpe_param->HStripe.stripe.n = _OutInfo.Ipe_Hstripe.n;
    pIpe_param->HStripe.stripe.l = _OutInfo.Ipe_Hstripe.l;
    pIpe_param->HStripe.stripe.m = _OutInfo.Ipe_Hstripe.m;
    //IPE V
    pIpe_param->VStripe.stripe.n = _OutInfo.Ipe_Vstripe.n;
    pIpe_param->VStripe.stripe.l = _OutInfo.Ipe_Vstripe.l;
    pIpe_param->VStripe.stripe.m = _OutInfo.Ipe_Vstripe.m;

    pIpe_param->HStripe.olap = IPE_OLAP_ON;
    pIpe_param->VStripe.olap = IPE_OLAP_ON;
}
//#NT#2011/02/08#ethanlau -end
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void Time2_Line_Pixel2(UINT32 TimeUs, UINT32 PixClkKHz, UINT32 HDTotal, UINT32 *Line, UINT32 *Pixel2)
{
    UINT32 Temp, LineTmp, Pixel2Tmp;

    Temp = (TimeUs * PixClkKHz) / 1000;
    LineTmp = Temp / HDTotal;
    Pixel2Tmp = (Temp - (LineTmp * HDTotal))/2;
    *(UINT32 *)Line = LineTmp;
    *(UINT32 *)Pixel2 = Pixel2Tmp;
}
//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------
UINT GetDPAdd_Prv(void)
{
    return gDP_Add_Prv;
}

UINT GetDPAdd_Cap(void)
{
    return gDP_Add_Cap;
}

void SetDPAdd(UINT DP_Prv, UINT DP_Cap)
{
    gDP_Add_Prv = DP_Prv;
    gDP_Add_Cap = DP_Cap;
}

//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------
//IPE TIFF configuration
void ipe_TIFFCfg(unsigned long InAddr0,
                unsigned long OutYAddr0,
                unsigned long InHsize,
                unsigned long InVsize,
                unsigned long cfapat,
                unsigned long ipe_omat,
                unsigned long ipe_dest)
{
#if 0
    unsigned long features=0,int_en=0;
    STR_IMG_SIZE InImgSz,OutImgSz;
    unsigned long OutYsize,OutCsize;

    ipe_Stop();
    ipe_ResetOn();
    ipe_ResetOff();

    //Input size setting
    InImgSz.hsize=InHsize;//2880;
    InImgSz.vsize=InVsize;//2148;

    //Config capture IPE registers
    //ipe_SetIOTarget(IPE_PARA_DMA,ipe_dest);//DMA->IPE->IME

    //JJ_UI_FileFmt
    if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_8)
        ipe_SetInPacking(IPE_PACK_08bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_10)
        ipe_SetInPacking(IPE_PACK_10bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_12)
        ipe_SetInPacking(IPE_PACK_12bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_16)
        ipe_SetInPacking(IPE_PACK_16bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else
        ipe_SetInPacking(IPE_PACK_08bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit

    ipe_SetOutFormat(ipe_omat,0);//ipe_omat: 0=444, 1=422, 2=420
    ipe_SetIOPPB(IPE_DISABLE,0,0);//parameters: (ppbi_en,ppbi_ini,ppbo_num);

    OutImgSz=ipe_SizeCfg(InImgSz,AUTO_FRAME_MODE,cfapat,IPE_LINEX9,IPE_OLAP_ON,IPE_OLAP_ON,IPE_ENABLE);    //Parameters:(InIMG,ipe_mode,cfapat,lbufnum,holap,volap)

    OutYsize=(OutImgSz.hsize)*(OutImgSz.vsize);
    switch(ipe_omat)
    {
        case IPE_YCC444:
            OutCsize=OutYsize;
            break;
        case IPE_YCC422:
            OutCsize=OutYsize/2;
            break;
        case IPE_YCC420:
            OutCsize=OutYsize/4;
            break;
        default:
            OutCsize=OutYsize;
            break;
    }

    //IO Dram configuration
    //JJ_UI_FileFmt
    ipe_SetDmaInAddr(InAddr0,0,InImgSz.hsize*gImageAlgInfo.PreCapBitDepth/8);
    //ipe_SetDmaOutAddr0(OutYAddr0,(OutYAddr0+OutYsize),(OutYAddr0+OutYsize+OutCsize),OutImgSz.hsize);

    features|=IPE_NR_EN;
    features|=IPE_RLPF_EN;
    features|=IPE_GLPF_EN;
    features|=IPE_BLPF_EN;
    features|=IPE_CCR_EN;
    features|=IPE_CTRL_EN;
    features|=IPE_HADJ_EN;
    features|=IPE_CADJ_EN;
    features|=IPE_CADJ_YENH_EN;
    features|=IPE_CADJ_YCON_EN;
    features|=IPE_CADJ_CCON_EN;
    features|=IPE_VACC_EN;
    features|=IPE_CGAIN_EN;//Adrian0623
    ipe_SetFeature(features);//Enable capture IPE blocks

    ipe_EnableInt(int_en);//Enable capture IPE interrupt

    //ipe_SetNRWeight(10,45,8,40);
    ipe_SetNRMap(0,0,0);
    ipe_SetColorInterpolation(4, 4, 0);
    ipe_SetColorCorrect(0,Normal_CC);
    ipe_SetYcolorAdj(120,128);
    //ipe_SetEdgeExtractHV(8,8,0,0,-1,-1);
    //ipe_SetEdgeExtractDiag(0,-1,-1,4);
    //ipe_SetEdgeMapping(8,1023,1023,1023,0);
    ipe_SetColorSup(32);

    //ipe_CapParameter(IPEISO100);

    //Clear int status
    ipe_ClearInt(IPE_INT_ALL);
#endif
}

//KH mark
#if 0
//IME TIFF configuration
void ime_TIFFCfg(STR_YCBCR_ADDR in_addr,
                STR_YCBCR_ADDR out_addr,
                unsigned int in_format,
                unsigned int out_format,
                STR_SCALE_SIZE_INFO io_size,
                BOOL src)
{
    #if 0
    ime_reset(ON);
    ime_reset(OFF);
    ime_start(OFF);
    ime_load();
    //ime_freeRun(0);

    ime_setSource(src);//src  0:IPE to IME, 1:DMA to IME
    ime_setPath(1);//path 0: both scale paths, 1: only one scale path, 2: only Bicubic scale path
    ime_Bilinear(0);//bil 0: bicubic, 1: bilinear
    ime_InputFormat(in_format);//format 0: 444, 1:422, 2:420
    ime_StripeMode(1);//sten 0: single stripe, 1: multi stripe
    ime_InputPingpong(0);//ippbe 0: ping-pong buffer disable, 1:ping-pong buffer enable
    ime_Path1OutputPingpong(0);//oppbe1 0:one output buffer, 1:two output buffer, 2:three output buffer
    ime_Path2OutputPingpong(0);//oppbe2 0:one output buffer, 1:two output buffer, 2:three output buffer
    ime_SetCST(103,88,183,198);//CST_PROM0,CST_PROM1,CST_PROM2,CST_PROM3
    ime_setICST(1,1);//icst_en 0:disable, 1:enable  //icst_opt 0:RGB transfer to YCBCR, 1:YCBCR transfer to RGB

    //Input
    ime_InputYcbCrAddr0(in_addr.y_addr, in_addr.cb_addr, in_addr.cr_addr);
    ime_InputYcbCrAddr1(in_addr.y_addr, in_addr.cb_addr, in_addr.cr_addr);
    ime_InputYcbCrOffset(io_size.in_h);

    set_ImeInputStripe();
    //Output path1
    ime_OutputPath1SizeScale(io_size.in_h, io_size.in_v, io_size.out_h, io_size.out_v);
    //ime_Path1HAverage(0);
    ime_Path1OutputFormat(out_format);
    ime_Path1HOutputSize(io_size.out_h-1);
    ime_Path1VOutputSize(io_size.out_v-1);
    ime_OutputPath1YcbCrAddr0(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath1YcbCrAddr1(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath1YcbCrAddr2(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath1YcbCrOffset(io_size.out_h);

    //Output path2
    ime_OutputPath2SizeScale(io_size.in_h, io_size.in_v, io_size.out_h, io_size.out_v);
    //ime_Path2HAverage(0);
    ime_Path2OutputFormat(out_format);
    ime_Path2HOutputSize(io_size.out_h-1);
    ime_Path2VOutputSize(io_size.out_v-1);
    ime_OutputPath2YcbCrAddr0(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath2YcbCrOffset(io_size.out_h);

    ime_load();
    ime_ClearFrameEnd();
    ime_StatustClear(Ime_INT_ALL);
    #endif
}
#endif

//For RGB TIFF FileFmt Header
void Set_RGB_TIFF_Header(PTIFF_Param pTIFFparam, PRGB_IMAGE_TIFF_HEADER pRGB_TIFFhearder)
{
    UINT32 ImgHsize, ImgVsize, StripByteCounts,StripOffsets1,StripOffsets2,StripOffsets3,temp;
    ImgHsize = pTIFFparam->ImageHsize;
    ImgVsize = pTIFFparam->ImageVsize;
    StripByteCounts = pTIFFparam->PlaneByteCounts;
    StripOffsets1 = 228;//RGB_TIFF_Header length
    StripOffsets2 = StripOffsets1+StripByteCounts;
    StripOffsets3 = StripOffsets2+StripByteCounts;

    //set ImageWidth value
    temp = ( (0xff000000)&(ImgHsize) );
    temp = (temp>>24);
    pRGB_TIFFhearder->ImageWidth.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(ImgHsize) );
    temp = (temp>>16);
    pRGB_TIFFhearder->ImageWidth.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgHsize) );
    temp = (temp>>8);
    pRGB_TIFFhearder->ImageWidth.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgHsize) );
    pRGB_TIFFhearder->ImageWidth.Value_Offset[3] = (UCHAR)(temp);

    //set ImageLength value
    //set RowsPerStrip value
    temp = ( (0xff000000)&(ImgVsize) );
    temp = (temp>>24);
    pRGB_TIFFhearder->ImageLength.Value_Offset[0] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(ImgVsize) );
    temp = (temp>>16);
    pRGB_TIFFhearder->ImageLength.Value_Offset[1] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgVsize) );
    temp = (temp>>8);
    pRGB_TIFFhearder->ImageLength.Value_Offset[2] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgVsize) );
    pRGB_TIFFhearder->ImageLength.Value_Offset[3] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[3] = (UCHAR)(temp);

    //set StripOffset2 value
    temp = ( (0xff000000)&(StripOffsets2) );
    temp = (temp>>24);
    pRGB_TIFFhearder->StripOffsetsValue[1][0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripOffsets2) );
    temp = (temp>>16);
    pRGB_TIFFhearder->StripOffsetsValue[1][1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripOffsets2) );
    temp = (temp>>8);
    pRGB_TIFFhearder->StripOffsetsValue[1][2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripOffsets2) );
    pRGB_TIFFhearder->StripOffsetsValue[1][3] = (UCHAR)(temp);

    //set StripOffset3 value
    temp = ( (0xff000000)&(StripOffsets3) );
    temp = (temp>>24);
    pRGB_TIFFhearder->StripOffsetsValue[2][0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripOffsets3) );
    temp = (temp>>16);
    pRGB_TIFFhearder->StripOffsetsValue[2][1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripOffsets3) );
    temp = (temp>>8);
    pRGB_TIFFhearder->StripOffsetsValue[2][2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripOffsets3) );
    pRGB_TIFFhearder->StripOffsetsValue[2][3] = (UCHAR)(temp);

    //set StripByteCounts value
    temp = ( (0xff000000)&(StripByteCounts) );
    temp = (temp>>24);
    pRGB_TIFFhearder->StripByteCountsValue[0][0] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][0] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripByteCounts) );
    temp = (temp>>16);
    pRGB_TIFFhearder->StripByteCountsValue[0][1] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][1] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripByteCounts) );
    temp = (temp>>8);
    pRGB_TIFFhearder->StripByteCountsValue[0][2] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][2] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripByteCounts) );
    pRGB_TIFFhearder->StripByteCountsValue[0][3] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][3] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][3] = (UCHAR)(temp);
}

//For JPEG TIFF FileFmt Header
void Set_JPEG_TIFF_Header(PTIFF_Param pTIFFparam, PJPEG_IMAGE_TIFF_HEADER pJPEG_TIFFhearder)
{
    UINT32 ImgHsize, ImgVsize, StripByteCounts,temp;
    ImgHsize = pTIFFparam->ImageHsize;
    ImgVsize = pTIFFparam->ImageVsize;
    StripByteCounts = pTIFFparam->ImageByteCounts;

    //set ImageWidth value
    //set TileWidth value
    temp = ( (0xff000000)&(ImgHsize) );
    temp = (temp>>24);
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[0] = (UCHAR)(temp);;

    temp = ( (0x00ff0000)&(ImgHsize) );
    temp = (temp>>16);
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgHsize) );
    temp = (temp>>8);
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgHsize) );
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[3] = (UCHAR)(temp);

    //set ImageLength value
    //set RowsPerStrip value
    temp = ( (0xff000000)&(ImgVsize) );
    temp = (temp>>24);
    pJPEG_TIFFhearder->ImageLength.Value_Offset[0] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(ImgVsize) );
    temp = (temp>>16);
    pJPEG_TIFFhearder->ImageLength.Value_Offset[1] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgVsize) );
    temp = (temp>>8);
    pJPEG_TIFFhearder->ImageLength.Value_Offset[2] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgVsize) );
    pJPEG_TIFFhearder->ImageLength.Value_Offset[3] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[3] = (UCHAR)(temp);

    //set StripByteCounts value
    temp = ( (0xff000000)&(StripByteCounts) );
    temp = (temp>>24);
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripByteCounts) );
    temp = (temp>>16);
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripByteCounts) );
    temp = (temp>>8);
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripByteCounts) );
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[3] = (UCHAR)(temp);
}

//TIFF header copy
UINT TIFF_RGB_CopyHeader(UINT TIFFHeaderAddr,
                        UINT TIFFHsize,
                        UINT TIFFVsize)
{
    TIFF_Param TIFFParam;
    UINT TIFFHearderSize;

    TIFFParam.ImageHsize=TIFFHsize;
    TIFFParam.ImageVsize=TIFFVsize;
    TIFFParam.ImageByteCounts=TIFFHsize*TIFFVsize*3;
    TIFFParam.PlaneByteCounts=TIFFHsize*TIFFVsize;
    TIFFHearderSize=sizeof(RGB_TIFF_Header);
    Set_RGB_TIFF_Header(&TIFFParam, &RGB_TIFF_Header);

    memcpy((void*)TIFFHeaderAddr, (void*)&RGB_TIFF_Header, TIFFHearderSize);

    return TIFFHearderSize;
}
/*
#define    S_Y(addr,vsize,i,j)             *(volatile unsigned char *)(addr+vsize*j+i)
void    Y_LPF1( UINT Addr,
                UINT Hsize,
                UINT Vsize,
                UINT LineOffset)
{
    int     i=0,j=0;

    for(j=1;j<Vsize-1;j++)
    {
        for(i=1;i<Hsize-1;i++)
        {
        S_Y(Addr,LineOffset,i,j)=(  (UINT)(S_Y(Addr,LineOffset,(i),(j))*4)
                                    +(UINT)(S_Y(Addr,LineOffset,(i-1),(j-1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i-1),(j+1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i+1),(j-1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i+1),(j+1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i),(j-1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i),(j+1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i-1),(j)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i+1),(j)))
                                    )/12;


        }
    }
}
#define    getW(addr)             *(volatile unsigned long *)(addr)
void    Y_LPF2( UINT Addr,
                UINT Hsize,
                UINT Vsize,
                UINT LineOffset)
{
    UINT8   ucBuff[12], ucBuffOut[4];
    UINT32*  puiBuff[3];
    UINT32* puiBuffOut;
    UINT32 iAddr, jAddr;
    UINT32  jLoopMax;
    int     i=0,j=0, k=0;

    puiBuff[0] = (UINT32 *)&ucBuff[0];
    puiBuff[1] = (UINT32 *)&ucBuff[4];
    puiBuff[2] = (UINT32 *)&ucBuff[8];
    puiBuffOut = (UINT32 *)&ucBuffOut[0];

    jLoopMax = Vsize-1;

    iAddr = Addr;

    for(i=0;i<Hsize;i+=4)
    {
        jAddr = iAddr;

        *(UINT32 *)puiBuff[0]=getW(jAddr);
        jAddr += LineOffset;
        *(UINT32 *)puiBuff[1]=getW(jAddr);
        jAddr += LineOffset;
        k=2;

        for(j=1;j<jLoopMax;j++)
        {
            *(UINT32 *)puiBuff[k]=getW(jAddr+LineOffset);
            k=(k+1)%3;

            ucBuffOut[0] = ((UINT32)ucBuff[0]+ucBuff[4]+ucBuff[8])/3;
            ucBuffOut[1] = ((UINT32)ucBuff[1]+ucBuff[5]+ucBuff[9])/3;
            ucBuffOut[2] = ((UINT32)ucBuff[2]+ucBuff[6]+ucBuff[10])/3;
            ucBuffOut[3] = ((UINT32)ucBuff[3]+ucBuff[7]+ucBuff[11])/3;

            getW(jAddr)=*puiBuffOut;

            jAddr += LineOffset;
        }
        iAddr += 4;
    }
}
void    Y_LPF3( UINT Addr,
                UINT Hsize,
                UINT Vsize,
                UINT LineOffset,
                UINT BuffAddr)
{
    UINT32  BuffA;

    {//mem. assign
        BuffA = BuffAddr;
    }

    {//step 1:  copy to buffer
        grph_open();
        grph_enableDMA();
        grph_setImg1(   Addr,
                        LineOffset,
                        Vsize,
                        Hsize);
        grph_setImg2(   BuffA,
                        LineOffset);
        grph_setAOP(    0,          //BOOL b16BitPrc
                        GRPH_DST_2,         //GRPH_DST_SEL dstSel
                        0x00,       //GRPH_AOP_MODE aopMode
                        0x00);  //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        grph_close();//mark this line or the second picture will make the cam. abnormal
    }

    {//step 2:  [n-1] + [n+1]
        grph_open();
        grph_enableDMA();
        grph_setImg1(   BuffA+LineOffset*2,
                        LineOffset,
                        Vsize-2,
                        Hsize);
        grph_setImg2(   Addr,
                        LineOffset);
        grph_setAOP(    0,          //BOOL b16BitPrc
                        0,          //GRPH_DST_SEL dstSel
                        0x0D,       //GRPH_AOP_MODE aopMode
                        0x09);  //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        grph_close();//mark this line or the second picture will make the cam. abnormal
    }
    {//step 3:  +[n]
        grph_open();
        grph_enableDMA();
        grph_setImg1(   Addr+LineOffset,
                        LineOffset,
                        Vsize-2,
                        Hsize);
        grph_setImg2(   BuffA+LineOffset*2,
                        LineOffset);
        grph_setAOP(    0,          //BOOL b16BitPrc
                        0,          //GRPH_DST_SEL dstSel
                        0x0D,       //GRPH_AOP_MODE aopMode
                        0x2e);//0x35);  //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        grph_close();//mark this line or the second picture will make the cam. abnormal
    }
}

UINT YGaussianBlur(UINT YAddr, //YAddr
                UINT LineOffset,//CbCr lineoffset uint:byte
                UINT Hsize, //CbCr Hsize
                UINT Vsize, //CbCr Vsize
                UB  YRadius)// pixel
{
    UB  i;

    if(YRadius == 0)
        return E_OK;
    grph_open();

    for(i=0;i<YRadius;i++)
    {
//smooth horizontal
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(   YAddr,
                        Hsize,
                        Vsize,
                        Hsize-1);
        grph_setImg2(   YAddr+1,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
//smooth vertical
        if(i<5)
        {
        grph_enableDMA();
        grph_setImg1(   YAddr,
                        Hsize,
                        Vsize-1,
                        Hsize);
        grph_setImg2(   YAddr+(1)*LineOffset,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        }

    }

    grph_close();

    return  E_OK;
}

UINT CbCrGaussianBlur(UINT CbAddr,//CrAddr
                UINT CrAddr, //CrAddr
                UINT LineOffset,//CbCr lineoffset uint:byte
                UINT Hsize, //CbCr Hsize
                UINT Vsize, //CbCr Vsize
                UB  CbRadius,// pixel
                UB  CrRadius)// pixel
{
    UB  i;

    if((CbRadius == 0) && (CrRadius == 0))
        return E_OK;
    grph_open();

    for(i=0;i<CbRadius;i++)
    {
//smooth horizontal
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(   CbAddr,
                        Hsize,
                        Vsize,
                        Hsize-4);
        grph_setImg2(   CbAddr+4,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        (((i+1)%2)<<5),         //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
//smooth vertical
        if(i<8)
        {
        grph_enableDMA();
        grph_setImg1(   CbAddr,
                        Hsize,
                        Vsize-1,
                        Hsize);
        grph_setImg2(   CbAddr+(1)*LineOffset,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        }

    }

    for(i=0;i<CrRadius;i++)
    {
//smooth horizontal
        grph_enableDMA();
        grph_setImg1(   CrAddr,
                        Hsize,
                        Vsize,
                        Hsize-4);
        grph_setImg2(   CrAddr+4,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        (((i+1)%2)<<5),         //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
//smooth vertical
        if(i<8)
        {
        grph_enableDMA();
        grph_setImg1(   CrAddr,
                        Hsize,
                        Vsize-1,
                        Hsize);
        grph_setImg2(   CrAddr+(1)*LineOffset,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        }

    }
    grph_close();

    return  E_OK;
}
*/



//#NT#2009/10/20#Adrian Jiang -end


ER  SetIPEVarAccInfo(IPE_SIZEINFO *SizeInfo, STR_VA_INFOR *VAInfo)
{
    UINT8 PowerNum;
    UINT32 hsize, vsize, stx, sty, hnf=0;
    UINT32 hsize_total, vsize_total, hsize_total_max=511*3, vsize_total_max=255*3, win_num=3;

    //96220 va limit
    //start x 0~2046  start y 0~1022
    //va window x 0~511  y 0~255

    //Horizontal win calculation
    if(SizeInfo->INSIZE.hsize > hsize_total_max)
        hsize_total = hsize_total_max;
    else
        hsize_total = SizeInfo->INSIZE.hsize;

    hsize = ((SizeInfo->INSIZE.hsize - 16)/(win_num));
    //hsize = ((hsize >> 1) << 1);

/*
    if(SizeInfo->HSTRIPE.m > 0)
    {
        hstripe_n = (SizeInfo->HSTRIPE.n) << 4;
        if(hsize > hstripe_n - 8)
            hsize = hstripe_n - 8;
    }
*/
    stx = ((hsize_total - 16) - hsize * win_num) >> 1;
    //stx = (stx >> 1) << 1;

/*
    if(SizeInfo->HSTRIPE.m > 0)
    {
        hstripe_n = (SizeInfo->HSTRIPE.n) << 4;
        if(stx > hstripe_n - 8 - hsize)
            stx = hstripe_n - 8 - hsize;
    }
*/
    //#NT#2010/09/02#YH Lee -begin
    //#Modify 9-line buffer calculation
    //Vertical win calculation (fixed for 9-line buffer, Vpad ON)

    //Horizontal win calculation
    if(SizeInfo->INSIZE.vsize > vsize_total_max)
        vsize_total = vsize_total_max;
    else
        vsize_total = SizeInfo->INSIZE.vsize;

    vsize = ((vsize_total - 8)/(win_num));
    sty = ((SizeInfo->INSIZE.vsize - 8) - vsize * win_num) >> 1;

    if(sty > 2)
        sty -= 2;
    else
        sty = 0;
    //#NT#2010/09/02#YH Lee -end

    PowerNum = 0;
    while (1){
        PowerNum++;
        if ( (hsize >> PowerNum) == 0)
            break;
        if (PowerNum > 15)
            return E_SYS;
    }

    //Enhance VA value
    if (PowerNum >= 4)
        PowerNum -= 4;

    hnf = PowerNum;
//KH mark
#if 0
    if (PowerNum > IPE_VAC_HNF_MINIMUN )
        hnf = PowerNum - IPE_VAC_HNF_MINIMUN;
    else
        hnf = 0;
#endif

    //Return results
    VAInfo->vacc_mode = VACC_AUTO_UPDATED;
    VAInfo->hsz = hsize;
    VAInfo->vsz = vsize;
    VAInfo->stx = stx;
    VAInfo->sty = sty;
    VAInfo->hnf = hnf;

    return E_OK;
}

//#NT#2009/10/21#Bowen Li -begin
//#remove the unused function
#if 0
//#NT#2008/06/11#Lawrence Ho -Begin
UINT32  CPU_UpdatedIPEVarAccTab(IPE_PARAM *IpeParam)
{
    UINT32 cnt_y, cnt_x, ReadData, ReadNumData, ReadVaccData;
    UINT32 BufStart;

    IPEModeInfo.OUTINFO.OutSrcSel = VADATA_TYPE;
    IPEModeInfo.OUTINFO.Adr0 = IpeParam->VAAddr;
    ipe_changeOutput(&IPEModeInfo.OUTINFO);

    ipe_waitDMAEnd(IPE_CLRFLG);

    BufStart = IpeParam->VAAddr;

    if ((IpeParam->VaccInfo.vacc_num == WINDOWS_8X8) && (IpeParam->VaccInfo.vacc_hv == COMBINED_HV))
    {

        for (cnt_y = 0 ; cnt_y < VAC_WIN8 ;  cnt_y++){
            for (cnt_x = 0 ; cnt_x < VAC_WIN8 ;  cnt_x++){
                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;
                // Horizontal & Vertical Combined

                    IPE_Vacc_TAb1[cnt_y][cnt_x] = (UINT32) (ReadNumData);
                    IPE_Vacc_TAb2[cnt_y][cnt_x] = (UINT32) (ReadVaccData);

                BufStart += 4;
            }
        }
                return TAb1_TAb2_UPDATED;
    }
    /*
    }else if ((IpeParam->VaccInfo.vacc_num == WINDOWS_8X8) && (IpeParam->VaccInfo.vacc_hv == SEPARATE_HV))
    {

        for (cnt_y = 0 ; cnt_y < VAC_WIN8 ;  cnt_y++){
            for (cnt_x = 0 ; cnt_x < VAC_WIN8 ;  cnt_x++){
                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                // Horizontal
                if (ReadNumData != 0)
                    IPE_Vacc_TAb1[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                else
                    IPE_Vacc_TAb1[cnt_y][cnt_x] = 0;

                BufStart += 4;

                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                // Vertical
                if (ReadNumData != 0)
                    IPE_Vacc_TAb2[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                else
                    IPE_Vacc_TAb2[cnt_y][cnt_x] = 0;

                }
                BufStart += 4;

            }
                    return TAb1_TAb2_UPDATED;

        */
        if ((IpeParam->VaccInfo.vacc_num == WINDOWS_16X16) && (IpeParam->VaccInfo.vacc_hv == COMBINED_HV))
        {

            for (cnt_y = 0 ; cnt_y < VAC_WIN16 ;  cnt_y++){
                for (cnt_x = 0 ; cnt_x < VAC_WIN16 ;  cnt_x++){
                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;
                // Horizontal & Vertical Combined
                    IPE_Vacc_TAb3[cnt_y][cnt_x] = (UINT32) (ReadNumData);
                    IPE_Vacc_TAb3[cnt_y][cnt_x] = (UINT32) (ReadVaccData);
                    BufStart += 4;

                }
            }

                    return TAb3_TAb4_UPDATED;
        }
        /*
        }else if ((IpeParam->VaccInfo.vacc_num == WINDOWS_16X16) && (IpeParam->VaccInfo.vacc_hv == SEPARATE_HV))
        {

            for (cnt_y = 0 ; cnt_y < VAC_WIN16 ;  cnt_y++){
                for (cnt_x = 0 ; cnt_x < VAC_WIN16 ;  cnt_x++){
                    ReadData = *(volatile UINT32 *)BufStart;
                    ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                    ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                    // Horizontal
                    if (ReadNumData != 0)
                        PE_Vacc_TAb3[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                    else
                        PE_Vacc_TAb3[cnt_y][cnt_x] = 0;

                    BufStart += 4;
                    // Vertical
                    ReadData = *(volatile UINT32 *)BufStart;
                    ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                    ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                    if (ReadNumData != 0)
                        IPE_Vacc_TAb4[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                    else
                        IPE_Vacc_TAb4[cnt_y][cnt_x] = 0;

                    }
                    BufStart += 4;

                }
                return TAb3_TAb4_UPDATED;
        }
        */
            return 0xFFFFFFFF; // Fail
}
//#NT#2008/06/11#Lawrence Ho -End
#endif
//#NT#2009/10/21#Bowen Li -end

ER User_VABegin(AF_IPEINFO *AfIpeInfo)
{
//1. Should not disable all ipe interrupt here
//2. Open IPE DMAEND & FRAM interrupt for af used
    sie_waitVD(1);

    IPEModeInfo.IQINFO.VA_PARAM.vacc_gain = AfIpeInfo->vacc_gain;
    IPEModeInfo.IQINFO.VA_PARAM.va_th = AfIpeInfo->va_th;

    //IPE VA parameters
    SetIPEVarAccInfo(&IPEModeInfo.SIZEINFO, &IPEModeInfo.IQINFO.VA_PARAM);
//#NT#2009/11/17#Bowen Li -begin
//#add ipe hnf interface
    if (AfIpeInfo->hnfsel == IPE_HNF_MANUAL)
    {
        IPEModeInfo.IQINFO.VA_PARAM.hnf = AfIpeInfo->va_hnf;
    }
//#NT#2009/11/17#Bowen Li -end
    //IPE IQ parameters & IPE setting
    SenFlowFunc(IPE_PRV_AF_SETTING);

    return E_OK;
}

ER User_VAEnd(AF_IPEINFO *AFIpeInfo)
{
    SenFlowFunc(IPE_PRV_SETTING);
    ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
    return E_OK;
}

extern INT32 abs(INT32 i);
//#NT#2008/07/25#YH Lee -begin
//#NT#ARRANGE Integrate IPP functions
void SetIPPFuncInfor(UINT32 FuncsID, UINT32 SetValue)
{
    switch(FuncsID)
    {
        case DFS_EN:
            gIPPFuncInfor.DfsEn = SetValue;
            break;

        case DFS_CNT:
            gIPPFuncInfor.DfsCnt = SetValue;
            break;

        case SIED2D_EN:
            gIPPFuncInfor.SIED2DEn = SetValue;
            break;

        case NLIGHT_CAP_EN:
            gIPPFuncInfor.NLightCapEn = SetValue;
            break;

        case NLIGHT_CAP_SEL:
            gIPPFuncInfor.NLightCapSel = SetValue;
            break;

        case FQV_EN:
            gIPPFuncInfor.FQVEn = SetValue;
            break;

        case CAP_ISO:
            gIPPFuncInfor.CapISO = SetValue;
            break;

        case CHRFILT_EN:
            gIPPFuncInfor.ChrFiltEn = SetValue;
            break;

        case CHRFILT_NUM:
            gIPPFuncInfor.ChrFiltNum = SetValue;
            break;

        case FQV_INPROCESS:
            gIPPFuncInfor.FQVInProcess = SetValue;
            break;

        case THUMBIMG_INPROCESS:
            gIPPFuncInfor.ThumbImgInProcess = SetValue;
            break;

        case PRIIMG_INPROCESS:
            gIPPFuncInfor.MainImgInProcess = SetValue;
            break;

        case CHRCURVE_EN:
            gIPPFuncInfor.ChrCurveEn = SetValue;
            break;

        case YCURVE_EN:
            gIPPFuncInfor.YCurveEn = SetValue;
            break;

        case YCURVE:
            gIPPFuncInfor.YCurve = (UINT32 *)SetValue;
            break;

        case CHRCBCURVE:
            gIPPFuncInfor.ChrCBCurve = (UINT32 *)SetValue;
            break;

        case CHRCRCURVE:
            gIPPFuncInfor.ChrCRCurve = (UINT32 *)SetValue;
            break;

        case CAPGAMMACURVE:
            gIPPFuncInfor.CapGamma = (UINT32 *)SetValue;
            break;

        case GEO_EN:
            gIPPFuncInfor.GEOEn = SetValue;
            break;

        case BRC_EN:
            gIPPFuncInfor.BRCEn = SetValue;
            break;

        case FRAME_MODE_EN:
            gIPPFuncInfor.FrameMode = SetValue;
            break;

        case IFE_EMBEDDED_EN:
            gIPPFuncInfor.IfeEmbeddedEn = SetValue;
            break;

        case IFE_RAW_EN:
            gIPPFuncInfor.IfeRawEn = SetValue;
            break;

        case IFE_YCC_EN:
            gIPPFuncInfor.IfeYccEn = SetValue;
            break;

        case IFE_FUNC_TABLE_ADDR:
            gIPPFuncInfor.IfeFuncTableAddr = SetValue;
            break;

        case IFE_RAW_TABLE_ADDR:
            gIPPFuncInfor.IfeRawTableAddr = SetValue;
            break;

        case IFE_YCC_TABLE_ADDR:
            gIPPFuncInfor.IfeYccTableAddr = SetValue;
            break;

        //#NT#2010/07/09#Linkin Huang -begin
        //#NT# IFE for Y channel
        case IFE_Y_EN:
            gIPPFuncInfor.IfeYEn = SetValue;
            break;

        case IFE_Y_TABLE_ADDR:
            gIPPFuncInfor.IfeYTableAddr = SetValue;
            break;
        //#NT#2010/07/09#Linkin Huang -end

        case ECS_EN:
            gIPPFuncInfor.EcsEn = SetValue;
            break;
        case VID_Datestamp_EN:
            gIPPFuncInfor.VidDatestampEn = SetValue;
            break;
        case FQV_FRAMEMDOE_EN:
            gIPPFuncInfor.FQVFrameModeEn = SetValue;
            break;
        case JPG_DCOUT_EN:
            gIPPFuncInfor.JPGDcoutEn = SetValue;
            break;
        case JPG_EN:
            gIPPFuncInfor.JPGEn = SetValue;
            break;
        default:
            debug_err(("-E- FuncsID(%d) > MaxID(%d)\r\n", FuncsID, IPPFUNC_MAXID));
            break;
    }
}

UINT32 GetIPPFuncInfor(UINT32 FuncsID)
{
    UINT32 ReturnValue = 0;
    switch(FuncsID)
    {
        case DFS_EN:
            ReturnValue = gIPPFuncInfor.DfsEn;
            break;

        case DFS_CNT:
            ReturnValue = gIPPFuncInfor.DfsCnt;
            break;

        case SIED2D_EN:
            ReturnValue = gIPPFuncInfor.SIED2DEn;
            break;

        case NLIGHT_CAP_EN:
            ReturnValue = gIPPFuncInfor.NLightCapEn;
            break;

        case NLIGHT_CAP_SEL:
            ReturnValue = gIPPFuncInfor.NLightCapSel;
            break;

        case FQV_EN:
            ReturnValue = gIPPFuncInfor.FQVEn;
            break;

        case CAP_ISO:
            ReturnValue = gIPPFuncInfor.CapISO;
            break;

        case CHRFILT_EN:
            ReturnValue = gIPPFuncInfor.ChrFiltEn;
            break;

        case CHRFILT_NUM:
            ReturnValue = gIPPFuncInfor.ChrFiltNum;
            break;

        case FQV_INPROCESS:
            ReturnValue = gIPPFuncInfor.FQVInProcess;
            break;

        case THUMBIMG_INPROCESS:
            ReturnValue = gIPPFuncInfor.ThumbImgInProcess;
            break;

        case PRIIMG_INPROCESS:
            ReturnValue = gIPPFuncInfor.MainImgInProcess;
            break;

        case CHRCURVE_EN:
            ReturnValue = gIPPFuncInfor.ChrCurveEn;
            break;

        case YCURVE_EN:
            ReturnValue = gIPPFuncInfor.YCurveEn;
            break;

        case YCURVE:
            ReturnValue = (UINT32)gIPPFuncInfor.YCurve;
            break;

        case CHRCBCURVE:
            ReturnValue = (UINT32)gIPPFuncInfor.ChrCBCurve;
            break;

        case CHRCRCURVE:
            ReturnValue = (UINT32)gIPPFuncInfor.ChrCRCurve;
            break;

        case CAPGAMMACURVE:
            ReturnValue = (UINT32)gIPPFuncInfor.CapGamma;
            break;

        case GEO_EN:
            ReturnValue = gIPPFuncInfor.GEOEn;
            break;

        case BRC_EN:
            ReturnValue = gIPPFuncInfor.BRCEn;
            break;

        case FRAME_MODE_EN:
            ReturnValue = gIPPFuncInfor.FrameMode;
            break;

        case IFE_EMBEDDED_EN:
            ReturnValue = gIPPFuncInfor.IfeEmbeddedEn;
            break;

        case IFE_RAW_EN:
            ReturnValue = gIPPFuncInfor.IfeRawEn;
            break;

        case IFE_YCC_EN:
            ReturnValue = gIPPFuncInfor.IfeYccEn;
            break;

        case IFE_FUNC_TABLE_ADDR:
            ReturnValue = gIPPFuncInfor.IfeFuncTableAddr;
            break;

        case IFE_RAW_TABLE_ADDR:
            ReturnValue = gIPPFuncInfor.IfeRawTableAddr;
            break;

        case IFE_YCC_TABLE_ADDR:
            ReturnValue = gIPPFuncInfor.IfeYccTableAddr;
            break;

        case IFE_Y_EN:
            ReturnValue = gIPPFuncInfor.IfeYEn;
            break;

        case IFE_Y_TABLE_ADDR:
            ReturnValue = gIPPFuncInfor.IfeYTableAddr;
            break;

        case ECS_EN:
            ReturnValue = gIPPFuncInfor.EcsEn;
            break;

        case VID_Datestamp_EN:
            ReturnValue = gIPPFuncInfor.VidDatestampEn;
            break;
        case FQV_FRAMEMDOE_EN:
            ReturnValue = gIPPFuncInfor.FQVFrameModeEn;
            break;
        case JPG_DCOUT_EN:
            ReturnValue = gIPPFuncInfor.JPGDcoutEn;
            break;
        case JPG_EN:
            ReturnValue = gIPPFuncInfor.JPGEn;
            break;
        default:
            debug_err(("-E- FuncsID(%d) > MaxID(%d)\r\n", FuncsID, IPPFUNC_MAXID));
            break;
    }

    return ReturnValue;
}
/*
void SetDFSEn(UCHAR Enable)
{
    gDFS_EN = Enable;
    gDFS_CNT = DFS_FIRSTPIC;
}

UCHAR GetDFSEn(void)
{
    return gDFS_EN;
}

void SetDFSCnt(UCHAR Cnt)
{
    gDFS_CNT = Cnt;
}

UCHAR GetDFSCnt(void)
{
    return gDFS_CNT;
}
*/
//#NT#2008/07/25#YH Lee -end

//#NT#2009/10/20#Adrian Jiang, Rearrange the code start. -begin
//#NT#Rearrange the code
//#NT#2009/10/20 remove the unused code -Adrian
#if 0
//#NT#2008/07/08#YH Lee -begin
void ChromaFilter(UINT CbAddr,//CbAddr
                UINT CrAddr, //CrAddr
                UINT bufferAddr,
                UINT LineOffset,//CbCr lineoffset uint:byte
                UINT Hsize, //CbCr Hsize
                UINT Vsize,
                UINT *Chroma_Param) //CbCr Vsize
{
    /*
    FDE_CHR_PRAM chroma_info;
    UINT32          bufferAddr2;
    UINT32 i;

    bufferAddr2= bufferAddr + Hsize*Vsize;
    chroma_info.filt_pram = (FDE_CHR_FILT_PRAM *)Chroma_Param;

    grph_open();
    grph_swReset();
    grph_enableDMA();

    FDE_ChrOpen();
    for(i = 0; i < chroma_info.filt_pram->repeat; i ++)
    {
        //Cb Horizontal filtering
        chroma_info.in_addr = CbAddr;
        chroma_info.out_addr = bufferAddr;
        chroma_info.width = Hsize;
        chroma_info.height = Vsize;
        chroma_info.lineoffset = LineOffset;
        fde_setCHRMode(FDE_CHR_CHROMAFILTER, &chroma_info);

        //Cr Horizontal filtering
        chroma_info.in_addr = CrAddr;
        chroma_info.out_addr = bufferAddr2;
        chroma_info.width = Hsize;
        chroma_info.height = Vsize;
        chroma_info.lineoffset = LineOffset;
        fde_setCHRMode(FDE_CHR_CHROMAFILTER, &chroma_info);

        //Cb rotate
        grph_setImg1(bufferAddr, LineOffset, Vsize, Hsize);
        grph_setImg2(CbAddr, Vsize);
        grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);
        //Cb Verticle filtering
        chroma_info.in_addr = CbAddr;
        chroma_info.out_addr = bufferAddr;
        chroma_info.width = Vsize;
        chroma_info.height = Hsize;
        chroma_info.lineoffset = Vsize;
        fde_setCHRMode(FDE_CHR_CHROMAFILTER, &chroma_info);

        //Cr rotate
        grph_setImg1(bufferAddr2, LineOffset, Vsize, Hsize);
        grph_setImg2(CrAddr, Vsize);
        grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);

        //Cr Verticle filtering
        chroma_info.in_addr = CrAddr;
        chroma_info.out_addr = bufferAddr2;
        chroma_info.width = Vsize;
        chroma_info.height = Hsize;
        chroma_info.lineoffset = Vsize;
        fde_setCHRMode(FDE_CHR_CHROMAFILTER, &chroma_info);

        //Cb rotate
        grph_setImg1(bufferAddr, Vsize, Hsize, Vsize);
        grph_setImg2(CbAddr, LineOffset);
        grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);

        //Cr rotate
        grph_setImg1(bufferAddr2, Vsize, Hsize, Vsize);
        grph_setImg2(CrAddr, LineOffset);
        grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);
    }
    grph_disableDMA();
    grph_close();
    FDE_ChrClose();
    */
}
//#NT#2008/07/08#YH Lee -end
#endif
//#NT#2009/10/20#Adrian Jiang -end
/*
//#NT#2008/07/11#Bowen Li -begin
void SubSampleRaw(UINT32 SourceAdd, UINT32 TargetAdd, UINT32 InH, UINT32 InV,UINT32 OutH, UINT32 OutV, UINT32 OutOfs, UINT32 FieldNum)
{
    unsigned long SourceData,TargetData,i,j,k;
    unsigned long TempAdd,TempData;
    unsigned long SampleH,SampleV;

    SampleH = InH/(OutH-16);
    if(SampleH%2==0)
        k=1;
    else
        k=0;
    for(j=0;j<OutV;j++)
    {
        SampleV = (InV/FieldNum)*j/OutV;
        if((j%2) != (SampleV%2))
            SampleV+=1;
        TempAdd = SourceAdd+SampleV*FieldNum*InH;

        for(i=0;i<OutH-16;i+=4)
        {
            SourceData = *(UB *)(TempAdd + SampleH*i);
            TempData = SourceData&0xff;
            SourceData = *(UB *)(TempAdd + SampleH*(i+1)-k);
            TempData |= ((SourceData<<8&0xff00));
            SourceData = *(UB *)(TempAdd + SampleH*(i+2));
            TempData |= ((SourceData<<16&0xff0000));
            SourceData = *(UB *)(TempAdd + SampleH*(i+3)-k);
            TempData |= ((SourceData<<24&0xff000000));

            TargetData = TempData;
            *((UW *)(TargetAdd+i+8)) = TargetData;
        }

        TargetData = *((UH *)(TargetAdd+8));
        for(i=0;i<8;i+=2)
        {
            *((UH *)(TargetAdd+i)) = TargetData;
        }

        TargetData = *((UH *)(TargetAdd+OutH-10));
        for(i=OutH-8;i<OutH;i+=2)
        {
            *((UH *)(TargetAdd+i)) = TargetData;
        }
        TargetAdd += OutOfs;
    }
}
//#NT#2008/07/11#Bowen Li -end
*/
UINT32 CalculateRawY(UINT Add, UINT StartX,UINT StartY, UINT SizeX, UINT SizeY,UINT Lineoffset,UINT sample,UINT8 StartPixel,UINT32 *R,UINT32 *G,UINT32 *B)
{
    UINT SumR,SumGr,SumGb,SumB,AvgR,AvgG,AvgB,Data[4];
    UINT i,j,Y,count=0;
    unsigned char Pixel_R,Pixel_Gr,Pixel_Gb,Pixel_B;

    Data[0]=0;
    Data[1]=0;
    Data[2]=0;
    Data[3]=0;
    if(StartPixel == 0)// R
    {
        Pixel_R = 0;
        Pixel_Gr = 1;
        Pixel_Gb = 2;
        Pixel_B = 3;
    }
    else if (StartPixel == 1) // Gr
    {
        Pixel_Gr = 0;
        Pixel_R = 1;
        Pixel_B = 2;
        Pixel_Gb = 3;
    }
    else if (StartPixel == 2) // Gb
    {
        Pixel_Gb = 0;
        Pixel_B = 1;
        Pixel_R = 2;
        Pixel_Gr = 3;
    }
    else if (StartPixel == 3) // B
    {
        Pixel_B = 0;
        Pixel_Gb = 1;
        Pixel_Gr = 2;
        Pixel_R = 3;
    }
    else
    {
        return 0;
    }

    for(j=StartY;j<(StartY+SizeY);j+=(sample*2))
    {
        for(i=StartX;i<(StartX+SizeX);i+=(sample*2))
        {
            Data[Pixel_Gb]+=*((UB*)(Add+i+j*Lineoffset));
            Data[Pixel_B]+=*((UB*)(Add+i+1+j*Lineoffset));
            Data[Pixel_R]+=*((UB*)(Add+i+(j+1)*Lineoffset));
            Data[Pixel_Gr]+=*((UB*)(Add+i+1+(j+1)*Lineoffset));
            count++;
        }
    }
    if((StartX+StartY)%2==0)
    {
        if(StartX%2==0)
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
        if(StartX%2==0)
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
    AvgR=(SumR<<4)/count;
    AvgB=(SumB<<4)/count;
    AvgG=((SumGr+SumGb)<<4)/(count*2);
    *R=AvgR;
    *G=AvgG;
    *B=AvgB;

    Y=(77*AvgR+150*AvgG+29*AvgB)>>8;

    return Y;
}

UINT Sen_GetIPECfaPat(UINT OrgPat, UINT FlipType)
{
    UINT8 PatTable[4][4] =
    {
        {IPE_PATR, IPE_PATGR, IPE_PATGB, IPE_PATB},
        {IPE_PATGR, IPE_PATR, IPE_PATB, IPE_PATGB},
        {IPE_PATGB, IPE_PATB, IPE_PATR, IPE_PATGR},
        {IPE_PATB, IPE_PATGB, IPE_PATGR, IPE_PATR}
    };
    //#NT#2011/10/28#Ethan Lau -begin
    //#NT#add sensor flip condition
    if(FlipType > _DSP_Flip_H_V)
        return OrgPat;
    //#NT#2011/10/28#Ethan Lau -end
    return PatTable[OrgPat][FlipType];
}

UINT32 Sen_GetSIEAddrOfs(UINT8 FlipType, UINT32 PreOutVSize, UINT32 PreOutLineOfs)
{
    UINT32 OfsValue;

    OfsValue = 0;
    if (FlipType == _DSP_Flip_H)
    {
        OfsValue = PreOutLineOfs -1;
    }
    else if (FlipType == _DSP_Flip_V)
    {
        OfsValue = PreOutLineOfs * (PreOutVSize - 1);
    }
    else if (FlipType == _DSP_Flip_H_V)
    {
        OfsValue = PreOutLineOfs * (PreOutVSize - 1) + (PreOutLineOfs-1);
    }
    return OfsValue;
}

UINT32 Sen_GetSIEFlipParam(UINT8 FlipType)
{
    UINT32 FlipTable[_Flip_Max_type] =
    {
        //dps                                                           sensor
        0, SIE_HFLIP_EN, SIE_VFLIP_EN, (SIE_HFLIP_EN|SIE_VFLIP_EN),     0,0,0
    };
    //debug_msg("%s:%x(%d) \r\n",__func__,FlipTable[FlipType],FlipType);
    return FlipTable[FlipType];
}

void Sen_GetImeStripeInfo(IMEStripeInfo *Imestipe)
{
    STR_IME_STRIPE_INFOR GetStripe;

    GetStripe = ime_getHstripe();
    Imestipe->Hstripe.n = GetStripe.n;
    Imestipe->Hstripe.l = GetStripe.l;
    Imestipe->Hstripe.m = GetStripe.m;

    GetStripe = ime_getVstripe();

    Imestipe->Vstripe.n = GetStripe.n;
    Imestipe->Vstripe.l = GetStripe.l;
    Imestipe->Vstripe.m = GetStripe.m;
}

//#NT#2010/08/09/#Linkin Huang -begin
//#NT# modify function interface
//#NT#2009/10/07#Linkin Huang -begin
//#NT#add a new parameter "LineOffset"
void ImgYCCProc(ImgYCC_POST_PROCESS_INFO *ImgYCCParam)
{
    //#NT#2011/04/07#Ethan Lau -begin
    //#NT#marked,220 have no pre and ipe d2d
    #if 0
    UINT32 IME_OUT_Y_Addr = ImgYCCParam->AddrY, IME_OUT_Cb_Addr = ImgYCCParam->AddrCb, IME_OUT_Cr_Addr = ImgYCCParam->AddrCr;
    UINT32 widthY  = ImgYCCParam->WidthY,  heightY = ImgYCCParam->HeightY,   LineOffsetY  = ImgYCCParam->LineoffsetY;
    UINT32 widthCx = ImgYCCParam->WidthCx, heightCx = ImgYCCParam->HeightCx, LineOffsetCx = ImgYCCParam->LineoffsetCx;


    IMG_FILTER_PARAM ImgFilterInfo;
    PRE_YCC_D2D_PARAM PreD2DInfo;

    //#NT#2010/07/13#Linkin Huang -begin
    //#NT# IPE for Y channel
    IPE_PARAM IpeParam;
    //#NT#2010/07/13#Linkin Huang -end

    //#NT#2010/07/13#Linkin Huang -begin
    //#NT# IPE for Y channel
    if(GetIPPFuncInfor(YCURVE_EN))
    {
       //do IPE for Y channel
       IpeParam.InSizeH = widthY + IPE_HSIZE_IODIFF;
       IpeParam.InSizeV = heightY;
 //KH mark
#if 0
       IpeParam.InLineOfs = IpeParam.OutLineOfs = LineOffsetY;
#endif
       IpeParam.BitDepth = IPE_PACK_08bit;
       IpeParam.Add0 = IpeParam.Add1 = IME_OUT_Y_Addr - (IPE_HSIZE_IODIFF >> 1);
       IpeParam.VAAddr = IME_OUT_Y_Addr;
//KH mark
#if 0
       IpeParam.OutSrcSel = YDATA_TYPE;
       IpeParam.D2DInFormat = SINGLE_CHANNEL_IN;
       ipe_Dram2Dram(&IpeParam, IPED2D_Y, IPE_NOWAIT);
#endif
    }
    //#NT#2010/07/13#Linkin Huang -end

    if (GetIPPFuncInfor(IFE_YCC_EN))
    {
        ImgFilterInfo.Mode = IFE_YCC;
        ImgFilterInfo.ImgAddr = IME_OUT_Cb_Addr;
        ImgFilterInfo.HSize = widthCx;
        ImgFilterInfo.VSize = heightCx;
//KH mark
#if 0
        ImgFilterInfo.InBits = IFE_8BIT;
        ImgFilterInfo.OutBits = ImgFilterInfo.InBits;
#endif
        //#NT#2009/10/07#Linkin Huang -begin
        //#NT# set IFE lineoffset
        ImgFilterInfo.InLineOfs = LineOffsetCx;
        ImgFilterInfo.OutLineOfs = LineOffsetCx;
        //#NT#2009/10/07#Linkin Huang -end
        ImgFilterInfo.WaitEnd = ENABLE;
        ImageFilter(&ImgFilterInfo);

        ImgFilterInfo.ImgAddr = IME_OUT_Cr_Addr;
        ImageFilter(&ImgFilterInfo);

        //#NT#2010/04/23#Linkin Huang -begin
        //#NT# ipp debug
        #if IPP_DEBUG
        {
          if(GetIPPFuncInfor(PRIIMG_INPROCESS) == ENABLE)
          {
           char filename[64];

           sprintf(filename, "A:\\RegBin\\IFE_Cx.bin");
           IppDebug_WriteEngineRegInfo(filename, IppDebug_IFE);
          }
        }
        #endif
        //#NT#2010/04/23#Linkin Huang -end
    }
    //#NT#2010/07/13#Linkin Huang -begin
    //#NT# IPE for Y channel
    if(GetIPPFuncInfor(YCURVE_EN))
    {
      //IPE wait frame end for Y channel
      ipe_Dram2DramWaitEnd();

      #if IPP_DEBUG
      {
        if(GetIPPFuncInfor(PRIIMG_INPROCESS) == ENABLE)
        {
          char filename[64];

          sprintf(filename, "A:\\RegBin\\IPE_Y.bin");
          IppDebug_WriteEngineRegInfo(filename, IppDebug_IPE);
        }
      }
      #endif
    }
    //#NT#2010/07/13#Linkin Huang -end

    //#NT#2010/07/13#Linkin Huang -bebin
    //#NT# pipe due to IPE Y channel flow
    //#NT#2010/07/09#Linkin Huang -begin
    //#NT#IFE for Y channel
    if(GetIPPFuncInfor(IFE_Y_EN))
    {
        ImgFilterInfo.Mode = IFE_Y;
        ImgFilterInfo.ImgAddr = IME_OUT_Y_Addr;
        ImgFilterInfo.HSize = widthY;
        ImgFilterInfo.VSize = heightY;
//KH mark
#if 0
        ImgFilterInfo.InBits = IFE_8BIT;
        ImgFilterInfo.OutBits = ImgFilterInfo.InBits;
#endif
        ImgFilterInfo.InLineOfs = LineOffsetY;
        ImgFilterInfo.OutLineOfs = LineOffsetY;
        ImgFilterInfo.WaitEnd = DISABLE;//ENABLE;
        ImageFilter(&ImgFilterInfo);

        //#NT#2010/07/09#Linkin Huang -begin
        //#NT# ipp debug
        #if IPP_DEBUG
        {
          if(GetIPPFuncInfor(PRIIMG_INPROCESS) == ENABLE)
          {
           char filename[64];

           sprintf(filename, "A:\\RegBin\\IFE_Y.bin");
           IppDebug_WriteEngineRegInfo(filename, IppDebug_IFE);
          }
        }
        #endif
        //#NT#2010/07/09#Linkin Huang -end
    }
    //N#T#2010/07/09#Linkin Huang -end
    //#NT#2010/07/13#Linkin Huang -end

    //chroma curve
    if(GetIPPFuncInfor(CHRCURVE_EN))
    {
        PreD2DInfo.FuncSel = PRE_GAMMA_EN;
        PreD2DInfo.SrcAddr = IME_OUT_Cb_Addr;
        PreD2DInfo.DestAddr = IME_OUT_Cb_Addr;
        PreD2DInfo.HSize = widthCx;
        PreD2DInfo.VSize = heightCx;
        //#NT#2009/10/07#Linkin Huang -begin
        //#NT# set PRE lineoffset
        PreD2DInfo.LineOffset = LineOffsetCx;
        //#NT#2009/10/07#Linkin Huang -end
        PreD2DInfo.pre_gammaLUT = (UINT32 *)GetIPPFuncInfor(CHRCBCURVE);
        PreD2DInfo.WaitEnd = ENABLE;
        pre_YCCD2DProc(&PreD2DInfo);

        PreD2DInfo.FuncSel = PRE_GAMMA_EN;
        PreD2DInfo.SrcAddr = IME_OUT_Cr_Addr;
        PreD2DInfo.DestAddr = IME_OUT_Cr_Addr;
        PreD2DInfo.HSize = widthCx;
        PreD2DInfo.VSize = heightCx;
        //#NT#2009/10/07#Linkin Huang -begin
        //#NT# set PRE lineoffset
        PreD2DInfo.LineOffset = LineOffsetCx;
        //#NT#2009/10/07#Linkin Huang -end
        PreD2DInfo.pre_gammaLUT = (UINT32 *)GetIPPFuncInfor(CHRCRCURVE);
        PreD2DInfo.WaitEnd = ENABLE;
        pre_YCCD2DProc(&PreD2DInfo);

        //#NT#2010/04/23#Linkin Huang -begin
        //#NT# ipp debug
        #if IPP_DEBUG
        {
          if(GetIPPFuncInfor(PRIIMG_INPROCESS) == ENABLE)
          {
           char filename[64];

           sprintf(filename, "A:\\RegBin\\PRE_CxCv.bin");
           IppDebug_WriteEngineRegInfo(filename, IppDebug_PRE);
          }
        }
        #endif
        //#NT#2010/04/23#Linkin Huang -end
    }


    //#NT#2010/07/13#Linkn Huang -begin
    //#NT# pipe due to IPE Y channel flow
    if(GetIPPFuncInfor(IFE_Y_EN))
    {
       ImageFilterWaitEnd();
    }
    //N#T#2010/07/13#Linkin Huang -end


    //#NT#2010/07/13#Linkin Huang -begin
    //#NT# removed due to IPE for Y channel and pipe reason
    //#if 0
    //#NT#2010/07/09#Linkn Huang -begin
    //#NT#IFE for Y channel
    //if(GetIPPFuncInfor(IFE_Y_EN))
    //{

        //ImgFilterInfo.Mode = IFE_Y;
        //ImgFilterInfo.ImgAddr = YAddr;
        //ImgFilterInfo.HSize = ImgHSize;
        //ImgFilterInfo.VSize = ImgVSize;
        //ImgFilterInfo.InBits = IFE_8BIT;
        //ImgFilterInfo.OutBits = ImgFilterInfo.InBits;
        //ImgFilterInfo.InLineOfs = LineOffset;
        //ImgFilterInfo.OutLineOfs = LineOffset;
        //ImgFilterInfo.WaitEnd = ENABLE;
        //ImageFilter(&ImgFilterInfo);

        //#NT#2010/07/09#Linkin Huang -begin
        //#NT# ipp debug
        //#if IPP_DEBUG
        //{
          //if(GetIPPFuncInfor(PRIIMG_INPROCESS) == ENABLE)
          //{
           //char filename[64];

           //sprintf(filename, "A:\\RegBin\\IFE_Y.bin");
           //IppDebug_WriteEngineRegInfo(filename, IppDebug_IFE);
          //}
        //}
        //#endif
        //#NT#2010/07/09#Linkin Huang -end

    //}
    //N#T#2010/07/09#Linkin Huang -end
    //#endif
    //#NT#2010/07/13#Linkin Huang -end
#endif
}
//#NT#2009/10/07#Linkin Huang -end
//#NT#2010/08/09#Linkin Huang -end

//#NT#2009/10/07#Linkin Huang -begin
//#NT# add a new parameter "LineOffset"
//#NT#2009/10/13#Bowen Li -begin
//#modify capture flow for new memory arrange
//#NT#2010/08/09/#Linkin Huang -begin
//#NT# modify function interface
void ImgYCCPostProcess(ImgYCC_POST_PROCESS_INFO *ImgYCCParam, ImageAlgInfo* AlgInfo)
{
    //IPPBUF_BUFFER BufIdx;

    ImgYCCProc(ImgYCCParam);
}
//#NT#2010/08/09#Linkin Huang -end
//#NT#2009/10/13#Bowen Li -end
//#NT#2009/10/07#Linkin Huang -end

//#NT#2011/02/14#Yuppie Lin -begin
//#NT#modify SliceSize Gen to more condition
ER SliceSize_Gen(pSliceSizeInfo _pOutInfo, SliceGenIn _InInfo)
{
    UINT32  IpeInH, IpeInV;
    UINT32  ImeInH, ImeInV;
    UINT32  ImeOutH, ImeOutV;

    //#NT#2011/08/12#Yuppie Lin -begin
    //#NT#patch for NOT 16x input
    //IpeInH = _InInfo.IpeInH;
    IpeInH = ((_InInfo.IpeInH + 15)>>4)<<4;
    //#NT#2011/08/12#Yuppie Lin ¡Vend

    IpeInV = _InInfo.IpeInV;
    ImeInH = IpeInH - 16;
    ImeInV = IpeInV;// - 8;
    ImeOutH = _InInfo.ImeOutH;
    ImeOutV = _InInfo.ImeOutV;

    if((ImeInH % 16) != 0)
    {
        debug_err(("^RIME horizontal input size must be 16x!!\r\n"));
    }
    if(_InInfo.ImeMode == IPE2IME_FRAME_MODE) {

        _InInfo.ImeSliceH = 4096;
    }
    //#NT#2011/02/15#ethanlau -begin
    //#NT#add ime slice mode or not
    if((_InInfo.ImeMode == IPE2IME_SLICE_MODE)||(_InInfo.ImeMode == DRAM2IME_SLICE_MODE))
    {
        if(ImeOutV < _InInfo.ImeSliceH)
        {
          debug_err(("^RDoubt for IME Slice Height setting may be due to output size of vertical smaller than the Slice-Height\r\n"));
        }
    }
    //#NT#2011/02/15#ethanlau -end

    //IME Hstripe Calculate
    if((ImeInH >> 4) <= (22 - 1))//single
    {
        //_pOutInfo->Ime_Hstripe.n = 0;
        //_pOutInfo->Ime_Hstripe.m = 0;
        //_pOutInfo->Ime_Hstripe.l = ImeInH >> 4;

        _pOutInfo->Ime_Hstripe.n = (ImeInH >> 5) +1;
        _pOutInfo->Ime_Hstripe.m = 1;
        _pOutInfo->Ime_Hstripe.l = (ImeInH >> 4) - ((_pOutInfo->Ime_Hstripe.n - 1) * _pOutInfo->Ime_Hstripe.m);
    }
    else//multi
    {
        _pOutInfo->Ime_Hstripe.n = 21;//Max linebuffer => 336/16 = 21
        _pOutInfo->Ime_Hstripe.m = (ImeInH >> 4) / (_pOutInfo->Ime_Hstripe.n - 1);
        _pOutInfo->Ime_Hstripe.l = (ImeInH >> 4) - ((_pOutInfo->Ime_Hstripe.n - 1) * _pOutInfo->Ime_Hstripe.m);
        while(((_pOutInfo->Ime_Hstripe.l) < 8) && (_pOutInfo->Ime_Hstripe.n))
        {
            _pOutInfo->Ime_Hstripe.n --;
            _pOutInfo->Ime_Hstripe.m = (ImeInH >> 4) / (_pOutInfo->Ime_Hstripe.n - 1);
            _pOutInfo->Ime_Hstripe.l = (ImeInH >> 4) - ((_pOutInfo->Ime_Hstripe.n - 1) * _pOutInfo->Ime_Hstripe.m);
        }
    }

    //IME Vstripe Calculate
    if(ImeOutV <= _InInfo.ImeSliceH)
    {
        if((_InInfo.IpeMode == IPE_AUTOSLICE) && (ImeInV > (_InInfo.Sie_SliceH_Default - 8))){
            _pOutInfo->Ime_Vstripe.n = _InInfo.Sie_SliceH_Default - 8;
            _pOutInfo->Ime_Vstripe.n -= (_pOutInfo->Ime_Vstripe.n & 0x1);
            _pOutInfo->Ime_Vstripe.m = ImeInV / (_pOutInfo->Ime_Vstripe.n - 4);
            _pOutInfo->Ime_Vstripe.l = ImeInV - (_pOutInfo->Ime_Vstripe.n - 4) * _pOutInfo->Ime_Vstripe.m;
            while(((_pOutInfo->Ime_Vstripe.l) < 8) && (_pOutInfo->Ime_Vstripe.n))
            {
                _pOutInfo->Ime_Vstripe.m --;
                _pOutInfo->Ime_Vstripe.l += _pOutInfo->Ime_Vstripe.n - 4;
            }
            _pOutInfo->Ime_Vstripe.n--;
            _pOutInfo->Ime_Vstripe.l--;
        }
        else {
            _pOutInfo->Ime_Vstripe.n = 0;
            _pOutInfo->Ime_Vstripe.m = 0;
            _pOutInfo->Ime_Vstripe.l = ImeInV - 1;
        }
    }
    else
    {
        _pOutInfo->Ime_Vstripe.n = ((ImeInV * _InInfo.ImeSliceH + (ImeOutV - 1)) / ImeOutV) + 4;
        _pOutInfo->Ime_Vstripe.n += (_pOutInfo->Ime_Vstripe.n & 0x1);
        _pOutInfo->Ime_Vstripe.m = ImeInV / (_pOutInfo->Ime_Vstripe.n - 4);
        _pOutInfo->Ime_Vstripe.l = ImeInV - (_pOutInfo->Ime_Vstripe.n - 4) * _pOutInfo->Ime_Vstripe.m;
        while(((_pOutInfo->Ime_Vstripe.l) < 8) && (_pOutInfo->Ime_Vstripe.n))
        {
            _pOutInfo->Ime_Vstripe.m --;
            _pOutInfo->Ime_Vstripe.l += _pOutInfo->Ime_Vstripe.n - 4;
        }
        _pOutInfo->Ime_Vstripe.n--;
        _pOutInfo->Ime_Vstripe.l--;
    }

    //IPE Hstripe Calculate
    if(_pOutInfo->Ime_Hstripe.m == 0)
    {
        _pOutInfo->Ipe_Hstripe.n = 0;
        _pOutInfo->Ipe_Hstripe.m = 0;
        _pOutInfo->Ipe_Hstripe.l = _pOutInfo->Ime_Hstripe.l + 1;
    }
    else
    {
        _pOutInfo->Ipe_Hstripe.n = _pOutInfo->Ime_Hstripe.n + 1;
        _pOutInfo->Ipe_Hstripe.m = _pOutInfo->Ime_Hstripe.m;
        _pOutInfo->Ipe_Hstripe.l = _pOutInfo->Ime_Hstripe.l + 1;
    }

    //IPE Vstripe Calculate
    if(_pOutInfo->Ime_Vstripe.m == 0)
    {
        _pOutInfo->Ipe_Vstripe.n = 0;
        _pOutInfo->Ipe_Vstripe.m = 0;
        _pOutInfo->Ipe_Vstripe.l = _pOutInfo->Ime_Vstripe.l;
    }
    else
    {
        _pOutInfo->Ipe_Vstripe.n = _pOutInfo->Ime_Vstripe.n + 8;
        _pOutInfo->Ipe_Vstripe.m = _pOutInfo->Ime_Vstripe.m;
        _pOutInfo->Ipe_Vstripe.l = _pOutInfo->Ime_Vstripe.l;
    }
    if(_InInfo.IpeMode == IPE_AUTOSLICE) {
        if(_InInfo.Sie_SliceH_Default < _pOutInfo->Ipe_Vstripe.n)
        {
            debug_err(("^R SIE Slice is too small, %d is needed\r\n",_pOutInfo->Ipe_Vstripe.n));

            _pOutInfo->Sie_SliceH = _pOutInfo->Ipe_Vstripe.n;
        }
        else
        {
            _pOutInfo->Sie_SliceH = (IpeInV + 2) / 3;
            if(_pOutInfo->Sie_SliceH > _InInfo.Sie_SliceH_Default)
            {
               _pOutInfo->Sie_SliceH = _InInfo.Sie_SliceH_Default;
            }
            else if(_pOutInfo->Sie_SliceH  < _pOutInfo->Ipe_Vstripe.n)
            {
                _pOutInfo->Sie_SliceH = _pOutInfo->Ipe_Vstripe.n;
            }
        }
        _pOutInfo->Sie_SliceH += ( _pOutInfo->Sie_SliceH & 0x1);
    }
    return E_OK;
}
//#NT#2011/02/14#Yuppie Lin -end

//#NT#2011/01/11#ethanlau -begin
//#NT#Simplfy Ime D2D
void Exce_IME_D2D(pIME_FBADDR_PRAM _Ime_fb)
{
    IME_EXTEN _Ime_Ext;
    IME_OPENOBJ IMEOpenObj;
    IME_PARAM OneIme;

    _Ime_Ext.p1_out_scale_filter.sfCoefCalMode = IME_SCALE_FILTER_COEF_AUTO_MODE;

    OneIme.pIme_Ext = &_Ime_Ext;
    OneIme.pIme_fbaddr = _Ime_fb;
    IPPMgrReq.ImeParam1 = OneIme;

    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IMEOpenObj.FP_IMEISR_CB = NULL;

    ime_open(&IMEOpenObj);
    if ( IME_BeginIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_BeginIPP() error \n"));

    if ( IME_AllocatResourceIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_AllocatResourceIPP() error \n"));

    if ( IME_ExecIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
    if ( IME_EndIPP(&IPPMgrReq, IME_D2D) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
    ime_close();
}
//#NT#2011/01/11#ethanlau -end
///////////////////////////////////////////////////////////////////////////////////////////
#if 0
#pragma mark -
#endif
//#NT#2011/04/07#Ethan Lau -begin
//#NT#Package up Subsample out information
void GetSubOutRGBinfo(UINT32* R,UINT32* G,UINT32* B,UINT32 uiSize)
{
    STR_AAA_CA_PARA ca_para;
    //PImageAlgInfo pimginfo;
    //pimginfo = UI_GetParameter();

    if(sie_checkFunctionEnable(SIE_SUBOUT_EN) == DISABLE)
    {
        debug_err(("%s: Sub-sample out is not enabled\r\n",__func__));
        return;
    }

    ca_para.DramAddr = ippbuf_GetBuf(IPPBUF_CA);
    ca_para.DramLofs = GetIPPSizeInfor(_CALineOffset);
    ca_para.ImgHsize = GetIPPSizeInfor(_CAHsize);
    ca_para.ImgVsize = GetIPPSizeInfor(_CAVsize);
    ca_para.R_Data = R;
    ca_para.G_Data = G;
    ca_para.B_Data = B;
    ca_para.fmt = SENSOR_OUT_PROGRESSIVE;
    if(uiSize == 64)
    {
        ca_para.Win_x = 8;
        ca_para.Win_y = 8;
    }
    else if(uiSize == 256)
    {
        ca_para.Win_x = 16;
        ca_para.Win_y = 16;
    }
    else
        debug_err(("%s: only support 8x8 window\r\n",__func__));

    ca_para.Cfapat = SIE_PRE_Para_prv.SieCfaPat;
    ca_para.bitdepth = _SieBitDepth_8;//only support 8-bit only
    aaa_GetCAResult(&ca_para);
}

//#NT#set sie color gain,prv only
void SetColorGain(UINT32 Rg,UINT32 Bg,UINT32 Gg)
{
    STR_SIE_CG_INFOR cginfo = {0};
    IPE_CfaPatEnum  CfaPat;
    //PImageAlgInfo pimginfo;

    //pimginfo = UI_GetParameter();
    CfaPat = SIE_PRE_Para_prv.SieCfaPat;

    if( (Rg<1024)&&(Gg<1024)&&(Bg<1024) )
        cginfo.GainSel = SIECG_2_8;
    else if( (Rg<2048)&&(Gg<2048)&&(Bg<2048) )
    {
        Rg /=2;
        Gg /=2;
        Bg /=2;
        cginfo.GainSel = SIECG_3_7;
    }
    else
        debug_err(("^R %s: Color gain out of range!!\r\n",__func__));

    switch(CfaPat)
    {
        case IPE_PATR:
            cginfo.Gain0 = Rg; cginfo.Gain1 = Gg; cginfo.Gain2 = Gg; cginfo.Gain3 = Bg;
            break;
        case IPE_PATGR:
            cginfo.Gain0 = Gg; cginfo.Gain1 = Rg; cginfo.Gain2 = Bg; cginfo.Gain3 = Gg;
            break;
        case IPE_PATGB:
            cginfo.Gain0 = Gg; cginfo.Gain1 = Bg; cginfo.Gain2 = Rg; cginfo.Gain3 = Gg;
            break;
        case IPE_PATB:
            cginfo.Gain0 = Bg; cginfo.Gain1 = Gg; cginfo.Gain2 = Gg; cginfo.Gain3 = Rg;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }
    sie_changeCG(&cginfo);
}
//#NT#2011/04/07#Ethan Lau -end


//#NT#2012/06/14#Harry Tsai -begin
void SetECSInputAdd(UINT32 ECS_TblAddr)
{

    sie_changeInputAdd1(ECS_TblAddr);
}
//#NT#2012/06/14#Harry Tsai -end

void SetSensorMode(UINT32 uiMode)
{
    SENSOR_STATUS SensorStatus;
    Sensor_GetStatus_Indicated(uiMode, &SensorStatus);

    SetIPPSizeInfor(_SieXcenter, SensorStatus.pSyncInfo->Act_CenterX);
    SetIPPSizeInfor(_SieYcenter, SensorStatus.pSyncInfo->Act_CenterY);
    SetIPPSizeInfor(_HDTotal, SensorStatus.pSyncInfo->HD_total);
    SetIPPSizeInfor(_HDSync, SensorStatus.pSyncInfo->HD_sync);
    SetIPPSizeInfor(_HDActX, SensorStatus.pSyncInfo->Act_StartX);
    SetIPPSizeInfor(_HDActY, SensorStatus.pSyncInfo->Act_StartY);
    SetIPPSizeInfor(_HDActSizeX, SensorStatus.pSyncInfo->Act_SizeX);
    SetIPPSizeInfor(_HDActSizeY, SensorStatus.pSyncInfo->Act_SizeY);
    SetIPPSizeInfor(_VDTotal, SensorStatus.pSyncInfo->VD_total);
    SetIPPSizeInfor(_VDSync, SensorStatus.pSyncInfo->VD_sync);

    SetIPPSizeInfor(_OBWinX0, SensorStatus.pSyncInfo->OB_X0);
    SetIPPSizeInfor(_OBWinX1, SensorStatus.pSyncInfo->OB_X1);
    SetIPPSizeInfor(_OBWinY0, SensorStatus.pSyncInfo->OB_Y0);
    SetIPPSizeInfor(_OBWinY1, SensorStatus.pSyncInfo->OB_Y1);
    SetIPPSizeInfor(_OBSubSample, SensorStatus.pSyncInfo->OB_SubSample);
    SetIPPSizeInfor(_OBThreshold, SensorStatus.pSyncInfo->OB_Thres);
    SetIPPSizeInfor(_OBGain, SensorStatus.pSyncInfo->OB_Gain);
}

//#NT#2011/07/20#Ethan Lau -begin
//#NT#for 220 image ratio chg
//need to close ipp and reopen ipp
ER ChgImgRatio(void)
{
    IPL_OBJ IplObj;
    IPL_MODE iplmode;
    //debug_msg("%s:%d\r\n",__func__,IPL_GetMode());
    iplmode = IPL_GetMode();
    if(iplmode == IPL_MODE_OFF)
        return E_OK;

    if( (iplmode == IPL_MODE_CAP)     ||
        (iplmode == IPL_MODE_VIDEOREC))
    {
        return E_SYS;
    }

    IplObj.uiImeOutHeight1 = DispSizeInfo.uiDisplayFBHeight;
    IplObj.uiImeOutWidth1 = DispSizeInfo.uiDisplayFBWidth;
    IplObj.uiImeOutYoft1 = DispSizeInfo.uiDisplayFBWidth;
    IplObj.uiDzoomStep = Get_DzoomIndex();
    switch(GetIPPSizeInfor(_ImePath1Fmt))
    {
        case IME_OUTPUT_YCC_444:        IplObj.uiImgFmt = IMEOUT2_FMT444;
            break;
        case IME_OUTPUT_YCC_422_COS:    IplObj.uiImgFmt = IMEOUT2_FMT422_1;
            break;
        case IME_OUTPUT_YCC_422_CEN:    IplObj.uiImgFmt = IMEOUT2_FMT422_2;
            break;
        case IME_OUTPUT_YCC_420_COS:    IplObj.uiImgFmt = IMEOUT2_FMT420_1;
            break;
        case IME_OUTPUT_YCC_420_CEN:    IplObj.uiImgFmt = IMEOUT2_FMT420_2;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }

    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_OFF;
    IPL_SetCmd(&IplObj);

    IplObj.Mode = iplmode;
    if(IPL_SetCmd(&IplObj) != E_OK)
        return E_SYS;
    else
        return E_OK;
}
//#NT#2011/07/20#Ethan Lau -end
//#NT#2011/10/27#Ethan Lau -begin
ER ChgSensorFlip(UINT32 set_value)
{
    //SENSOR_STATUS sen_status;
    //UINT32 Rg,Gg,Bg;
    IPL_OBJ IplObj = {0};
    IPL_MODE iplmode  = IPL_GetMode();

    //config sensor flip
    if(Sensor_Flip(set_value)!= E_OK)
        return E_NOSPT;
#if 0
    //get sensor cfa pattern
    Sensor_GetStatus(&sen_status);
    //chg sie color gain cfa pattern
    SIE_PRE_Para_prv.SieCfaPat = sen_status.uiBayer;
    AWB_GetColorGian(&Rg,&Gg,&Bg);
    SetColorGain(Rg,Bg,Gg);
    //chg ipe cfa pattern
    IPE_Para.CfaPat= sen_status.uiBayer;
    IPEModeInfo.SIZEINFO.CfaPat = sen_status.uiBayer;
    ipe_changeSize(&IPEModeInfo.SIZEINFO);
    ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
#else
    //not support runing-change at cap/videorec. mode
    if( (iplmode == IPL_MODE_CAP)     ||
        (iplmode == IPL_MODE_VIDEOREC))
    {
        return E_SYS;
    }
    //get pipeline current information
    IplObj.uiImeOutHeight1 = DispSizeInfo.uiDisplayFBHeight;
    IplObj.uiImeOutWidth1 = DispSizeInfo.uiDisplayFBWidth;
    IplObj.uiImeOutYoft1 = DispSizeInfo.uiDisplayFBWidth;
    IplObj.uiDzoomStep = Get_DzoomIndex();
    switch(GetIPPSizeInfor(_ImePath1Fmt))
    {
        case IME_OUTPUT_YCC_444:        IplObj.uiImgFmt = IMEOUT2_FMT444;
            break;
        case IME_OUTPUT_YCC_422_COS:    IplObj.uiImgFmt = IMEOUT2_FMT422_1;
            break;
        case IME_OUTPUT_YCC_422_CEN:    IplObj.uiImgFmt = IMEOUT2_FMT422_2;
            break;
        case IME_OUTPUT_YCC_420_COS:    IplObj.uiImgFmt = IMEOUT2_FMT420_1;
            break;
        case IME_OUTPUT_YCC_420_CEN:    IplObj.uiImgFmt = IMEOUT2_FMT420_2;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }

    //stop pipeline and re-open pipeline
    if(iplmode != IPL_MODE_OFF)
    {
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }
    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = iplmode;
    IPL_SetCmd(&IplObj);
    return E_OK;
#endif
}
//#NT#2011/10/27#Ethan Lau -end

//#NT#2011/07/21#Jarkko Chang -begin
//#NT# copy from 96632 dvtk
BOOL MotionDetection(void)
{
    static UINT32 PrevG_acc[64], DetectionCnt=0;
    UINT32 i, Diff, Total_diff=0, DiffCnt=0;
    UINT32 r[64],g[64],b[64];   //ca 8x8 windows
    UINT32 tblsize = 64;    //ca 8x8 windows

    GetSubOutRGBinfo(r,g,b,tblsize);
    for(i=0;i<tblsize;i++)
    {
        if(PrevG_acc[i]>g[i])
            Diff = (PrevG_acc[i]-g[i]);
        else
            Diff = (g[i]-PrevG_acc[i]);

        Total_diff += Diff;
        if(Diff > BlockDiffThrs)
            DiffCnt ++;

        PrevG_acc[i] = g[i];
    }

    DetectionCnt ++;
    if(DetectionCnt < 2) // for initialization
        return FALSE;

    if((DiffCnt > BlockCntThres) || (Total_diff > TotalDiffThres))
        return TRUE;
    else
        return FALSE;
}
//#NT#2011/07/21#Jarkko Chang -end
