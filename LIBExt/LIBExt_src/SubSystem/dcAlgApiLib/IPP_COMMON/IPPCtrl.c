/**
    IPP flow control API

    This file is the API of the IPP flow control.

    @file       IPPCtrl.c
    @ingroup    Predefined_group_name
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
//include
#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "debug.h"
#include "ErrorNo.h"
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ife_lib.h"
#include "ime_lib.h"
#include "sra_lib.h"
#include "ImageAlgInfor.h"
#include "IPPCtrl.h"
#include "Utility.h"
#include "IPPManager.h"
#include "sen_common.h"
#include "Sensor.h"
#include "dzoom.h"
#include "sen_isr.h"
#include "pll.h"
#include "Alg_IPLMode.h"
//define
#define IPPCTRL_DEBUG ENABLE
#define Patch_IMEBUG ENABLE
#if IPPCTRL_DEBUG
#define IPPCTRL_MSG(...) debug_msg("IPPCTRL: "__VA_ARGS__)
#else
#define IPPCTRL_MSG(...)
#endif

typedef ER (*fpLocalFunc)(PIPPCTRL_OBJ pIPPCtrl_obj);

typedef struct _IPPENG_OBJ
{
    SIE_OPENOBJ SIEOBJ;
    IPE_OPENOBJ IPEOBJ;
    IME_OPENOBJ IMEOBJ;
    IFE_OPENOBJ IFEOBJ;
}IPPENG_OBJ,*PIPPENG_OBJ;

//extern ipp isr
extern void sie_isrCB(UINT32 IntStatus);
extern void ime_isrCB(UINT32 IntStatus);
extern void ipe_isrCB(UINT32 IntStatus);
extern void ife_isrCB(UINT32 IntStatus);
extern void RstImeCounter(void);

//local func. declaretion
static ER IPPctrl_Open2(PIPPCTRL_OBJ pIPPCtrl_obj);
static ER IPPctrl_Close2(PIPPCTRL_OBJ pIPPCtrl_obj);
static ER IPPctrl_Start2(PIPPCTRL_OBJ pIPPCtrl_obj);
static ER IPPctrl_Stop2(PIPPCTRL_OBJ pIPPCtrl_obj);
static ER IPPctrl_CapStop2(PIPPCTRL_OBJ pIPPCtrl_obj);
//static global var.
static IPPCTRL_RST m_IPPctrl_rst = {0};
static IPPCTRL_OBJ m_IPPctrl_Curobj = {0};
static IPPCTRL_FLOW m_IPPctrl_CurFlow = IPP_MaxFlowdefine;

//#NT#2011/09/28#Ethan Lau -begin
//#NT#mark
/*
static FPIPPBUFASSIGNER m_IPPctrl_flowchkseq[IPP_MaxFlowdefine] =
{
    ippbuf_SampleAssigner_ContF2F,
    ippbuf_SampleAssigner_ContF2F,
    ippbuf_SampleAssigner_ContS2F,
    ippbuf_SampleAssigner_ContS2S,
    NULL,
};
*/
static fpLocalFunc m_IPPctrl_Act[IPP_MaxOpmode] =
{
    IPPctrl_Close2,
    IPPctrl_Open2,
    IPPctrl_Start2,
    IPPctrl_Stop2,
    IPPctrl_CapStop2,
};
#if 0
#pragma mark - local func. definition
#endif
////////////////////////////////////////////////////////////////////////////////
static void IPPCTRL_initippbufinfo(PIPPCTRL_OBJ pIPPCtrl_obj,pIPPBUF_ENG2DRAMINFO pippbufinfo, IPPCTRL_FLOW flow)
{
    UINT32 dzoomtbl[8];

    Get_DZoomMaxInfor(dzoomtbl);

    //assign default info(F2F/S2F) for ipp_buf
    switch(pIPPCtrl_obj->IPPctrl_info.uiImgFmt)
    {
        case IMEOUT2_FMT444:
            pippbufinfo->Ime_Fmt[ImePath1] = FMT444;
            break;
        case IMEOUT2_FMT422_1:
        case IMEOUT2_FMT422_2:
            pippbufinfo->Ime_Fmt[ImePath1] = FMT422;
            break;
        case IMEOUT2_FMT420_1:
        case IMEOUT2_FMT420_2:
            pippbufinfo->Ime_Fmt[ImePath1] = FMT420;
            break;
        default:
            pippbufinfo->Ime_Fmt[ImePath1] = FMT422;
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }

    pippbufinfo->ImeBufferNum[ImePath1] = IME_OUTPUT_PPB_3;
    if((GetIPPFuncInfor(JPG_DCOUT_EN) == ENABLE) || (pIPPCtrl_obj->opmode == IPP_CAP_STOP) || (IPL_GetNextMode() == IPL_MODE_PCC))
    {
        pippbufinfo->ImeOutLineOffset[ImePath1] =  pIPPCtrl_obj->IPPctrl_info.uiPath1OutLineoffset;
        pippbufinfo->ImeOutCCLineOffset[ImePath1] = pIPPCtrl_obj->IPPctrl_info.uiPath1OutCCLineoffset;
        pippbufinfo->ImeOutV[ImePath1] = pIPPCtrl_obj->IPPctrl_info.uiPath1OutsizeV;
    }
    else
    {
        pippbufinfo->ImeOutLineOffset[ImePath1] = dzoomtbl[6];
        pippbufinfo->ImeOutCCLineOffset[ImePath1] = pippbufinfo->ImeOutLineOffset[ImePath1] /2;
        pippbufinfo->ImeOutV[ImePath1] = dzoomtbl[7];
    }
    pippbufinfo->SieBitDepth = pIPPCtrl_obj->IPPctrl_info.uiSieBitDepth;
    pippbufinfo->SieBufferNum = TWO_OUTPUT_BUF;
    pippbufinfo->EcsMapSize = ECS_TblSize;
    pippbufinfo->CropH = dzoomtbl[2];
    pippbufinfo->CropV = dzoomtbl[3];
    //220 have no path2,so path2 fmt is the same with path2
    pippbufinfo->Ime_Fmt[ImePath2] = pippbufinfo->Ime_Fmt[ImePath1];

    pippbufinfo->ImePath2Enable = pIPPCtrl_obj->IPPctrl_info.ImePath2Enable;
    if(pippbufinfo->ImePath2Enable)
    {
        //4 ping-pong buf is needed when video slice mode ycc post-porc. conditoin
        pippbufinfo->ImeBufferNum[ImePath2] = IME_OUTPUT_PPB_4;//IME_OUTPUT_PPB_3;
        pippbufinfo->ImeOutLineOffset[ImePath2] =  pIPPCtrl_obj->IPPctrl_info.uiPath2OutLineoffset;
        pippbufinfo->ImeOutCCLineOffset[ImePath2] = pIPPCtrl_obj->IPPctrl_info.uiPath2OutCCLineoffset;
        pippbufinfo->ImeOutV[ImePath2] = pIPPCtrl_obj->IPPctrl_info.uiPath2OutsizeV;
        //#NT#2012/03/20#Harry Tsai -begin
        //#NT#Modify slice height form 64 to 256 lines for 720P60, it's not suitable for 8MB DRAM
        if(pippbufinfo->ImeOutV[ImePath2] != 720)
            pippbufinfo->ImeSliceH = SLICE_64;//video slice mode always 64 line height
        else
            pippbufinfo->ImeSliceH = SLICE_256;//video slice mode always 256 line height
        //#NT#2012/03/20#Harry Tsai -end
    }
    //flow constraint
    switch(flow)
    {
        case IPP_SliceMode_S2S:
            pippbufinfo->SieBufferNum = THREE_OUTPUT_BUF;
            pippbufinfo->SieSliceH = (pippbufinfo->CropV+(THREE_OUTPUT_BUF))/(THREE_OUTPUT_BUF+1);
            if(pippbufinfo->SieSliceH & 1)
                pippbufinfo->SieSliceH++;
            break;
        case IPP_SliceMode_S2F:
            pippbufinfo->SieBufferNum = THREE_OUTPUT_BUF;
            pippbufinfo->SieSliceH = (pippbufinfo->CropV+(THREE_OUTPUT_BUF))/(THREE_OUTPUT_BUF+1);
            if(pippbufinfo->SieSliceH & 1)
                pippbufinfo->SieSliceH++;
        case IPP_FrameMode_HW:
        case IPP_FrameMode_FW:
            //#NT#2011/09/08#Ethan Lau -begin
            //#NT#because ide, ime only need three buf. under ime frame mode
            if(pippbufinfo->ImePath2Enable)
                pippbufinfo->ImeBufferNum[ImePath2] = IME_OUTPUT_PPB_3;
            //#NT#2011/09/08#Ethan Lau -end
            break;
        default://use pipe line default param. cfg
            //do nothing
            break;
    }
}

static __inline void Get_IPPCB(PIPPENG_OBJ pIPPcallback)
{
    (pIPPcallback->SIEOBJ.FP_SIEISR_CB) = sie_isrCB;
    (pIPPcallback->IPEOBJ.FP_IPEISR_CB) = ipe_isrCB;
    (pIPPcallback->IPEOBJ.IPE_CLOCKSEL) = PLL_CLKSEL_IPE_96;//PLL_CLKSEL_IPE_60;
    (pIPPcallback->IMEOBJ.FP_IMEISR_CB) = ime_isrCB;
    (pIPPcallback->IMEOBJ.IME_CLOCKSEL) = PLL_CLKSEL_IME_120;//PLL_CLKSEL_IME_80;
    (pIPPcallback->IFEOBJ.FP_IFEISR_CB) = ife_isrCB;
}

static IPPBUF_MODE ModeMap_IPL2IPPBuf(IPPCTRL_FLOW flow)
{
    switch(flow)
    {
        case IPP_FrameMode_HW:
            return IPPBUF_MODE_F2F;
        case IPP_FrameMode_FW:
            return IPPBUF_MODE_F2F;
        case IPP_SliceMode_S2F:
            return IPPBUF_MODE_S2F;
        case IPP_SliceMode_S2S:
            return IPPBUF_MODE_S2S;
        case IPP_SliceMode_F2S:
            return IPPBUF_MODE_CAP_NORMAL;
        default:
            debug_err(("^R %s %d\r\n",__func__,__LINE__));
            return IPPBUF_MODE_F2F;
    }
}

static ER IPPctrl_Open2(PIPPCTRL_OBJ pIPPCtrl_obj)
{
    ER er_code = E_OK,rt = E_OK;
    IPPBUF_RST rst={0};
    UINT32 i = 0;
    UINT32 dzoomtbl[8];
    IPPBUF_ENG2DRAMINFO ippbufinfo;
    PImageAlgInfo pimginfo;
    UINT32 maxidx;
    SENSOR_STATUS SensorStatus;
    IPPCTRL_FLOW flow;
    PIPPCTRL_RST pIPPCtrl_rst;

    pimginfo = UI_GetParameter();
    Sensor_GetStatus(&SensorStatus);
    //config dzoom tbl according sensor mode
    //#NT#2011/08/01#Jarkko Chang -begin
//    Set_DZoomInfor(SenMode2Tbl(SensorStatus.uiSensorMode,&maxidx),_Ratio_4_3,pimginfo->ImageRatio,pimginfo->DispRatio);
    Set_DZoomInfor(SenMode2Tbl(SensorStatus.uiSensorMode,&maxidx),SensorStatus.uiRatio,pimginfo->ImageRatio,pimginfo->DispRatio, pimginfo->DsiplayFitWindow);
    //#NT#2011/08/01#Jarkko Chang -end
    Set_DzoomMaxIndex(maxidx);
    Get_DZoomMaxInfor(dzoomtbl);

    pIPPCtrl_rst = pIPPCtrl_obj->pIPPctrl_rst;

    switch(pIPPCtrl_obj->opmode)
    {
        case IPP_OPEN:
            er_code = E_OK;
            break;
        case IPP_CLOSE:
            sie_close();
            //#NT#2011/05/20#Connie Yu -begin
            //#NT#update SRA API
            //sra_close();
            //#NT#2011/05/20#Connie Yu -end
            ipe_close();
            ife_close();
            ime_close();
            er_code = E_OK;
        case IPP_START:
            //condition when IPP have requested flow
            //if buf is not enough for this requested flow,it will be assign a
            //flow which can be worked under limited buf. size automatically.
            if(pIPPCtrl_obj->fpStrongFlow != NULL)
            {
                IPPCTRL_MSG("prepare to chk strong flow(%d)\r\n",pIPPCtrl_obj->fpStrongFlow());
                IPPCTRL_initippbufinfo(pIPPCtrl_obj,&ippbufinfo,pIPPCtrl_obj->fpStrongFlow());
                //add sie cfg

                if(pIPPCtrl_obj->fpStrongFlow() > IPP_MaxFlowdefine)
                {
                    debug_err(("IPPCTRL do not support this strong flow\r\n"));
                    rt = E_SYS;
                }

                rst = ippbuf_ExecuteAssigner(ModeMap_IPL2IPPBuf(pIPPCtrl_obj->fpStrongFlow()),&ippbufinfo);
                rt = rst.er_code;

                if(rt!=E_OK)
                {
                    IPPCTRL_MSG("strong flow(%d) fail,buffer is not enough\r\n",pIPPCtrl_obj->fpStrongFlow());
                    pIPPCtrl_obj->fpStrongFlow = NULL;
                }
                else if(rt == E_OK)
                {
                    if(g_fpAlgExport)
                    {
                        gAlgMsg.IPL_BufEndAddr = ippbuf_GetBuf(IPPBUF_PRV_CURREND);
                        g_fpAlgExport(ALGMSG_IPL_BUFND, &gAlgMsg);
                    }
                }
            }
            //automatically assign a IPP flow under limited buf size
            if(pIPPCtrl_obj->fpStrongFlow == NULL)
            {
                for(i=0;i<IPP_MaxFlowdefine;i++)
                {
                    IPPCTRL_initippbufinfo(pIPPCtrl_obj,&ippbufinfo,i);

                    rst = ippbuf_ExecuteAssigner(ModeMap_IPL2IPPBuf(i),&ippbufinfo);
                    rt = rst.er_code;

                    if(rt == E_OK)
                    {
                        if(g_fpAlgExport)
                        {
                            gAlgMsg.IPL_BufEndAddr = ippbuf_GetBuf(IPPBUF_PRV_CURREND);
                            g_fpAlgExport(ALGMSG_IPL_BUFND, &gAlgMsg);
                        }
                        break;
                    }
                }
            }

            //prepare to start available flow
            flow = (pIPPCtrl_obj->fpStrongFlow==NULL)?(i):(pIPPCtrl_obj->fpStrongFlow());
            //start to run pipe-line
            if(rt == E_OK)
            {
                pIPPCtrl_rst->ImeFmt[ImePath1] = ippbufinfo.Ime_Fmt[ImePath1];
                pIPPCtrl_rst->ImeFmt[ImePath2] = ippbufinfo.Ime_Fmt[ImePath2];

                Set_IME_OutInfo(flow, ImeOutInfo_FMT, (UINT32)pIPPCtrl_rst->ImeFmt[ImePath2]);
                memcpy(pIPPCtrl_rst->imebufnum,ippbufinfo.ImeBufferNum,sizeof(UINT32)*MAXPathNum);
                pIPPCtrl_rst->siebufnum = ippbufinfo.SieBufferNum;
                switch(flow)
                {
                    case IPP_FrameMode_HW:
                    case IPP_FrameMode_FW:
                        pIPPCtrl_rst->SieMode = SIE_FRAME_MODE;
                        pIPPCtrl_rst->IpeMode = IPE_AUTOSTP;
                        pIPPCtrl_rst->ImeMode = IPE2IME_FRAME_MODE;
                        break;
                    case IPP_SliceMode_S2F:
                        pIPPCtrl_rst->SieMode = SIE_SLICE_MODE;
                        pIPPCtrl_rst->IpeMode = IPE_AUTOSLICE;
                        pIPPCtrl_rst->ImeMode = IPE2IME_FRAME_MODE;
                        break;
                    case IPP_SliceMode_S2S:
                        pIPPCtrl_rst->SieMode = SIE_SLICE_MODE;
                        pIPPCtrl_rst->IpeMode = IPE_AUTOSLICE;
                        pIPPCtrl_rst->ImeMode = IPE2IME_SLICE_MODE;
                        pIPPCtrl_rst->uiImeSliceH = ippbufinfo.ImeSliceH;
                        break;
                    default:
                        debug_err(("%d %s\r\n",__LINE__,__func__));
                        break;
                }
                IPPCTRL_MSG("pipeline start in flow(%d)\r\n",flow);

                if(IPL_GetNextMode() == IPL_MODE_PCC)
                {
                    //chg ime out size
                    if(pIPPCtrl_obj->IPPctrl_info.uiPath1OutsizeH != DispSizeInfo.uiDisplayFBWidth)
                        DispSizeInfo.uiDisplayFBWidth = pIPPCtrl_obj->IPPctrl_info.uiPath1OutsizeH;
                    if(pIPPCtrl_obj->IPPctrl_info.uiPath1OutsizeV != DispSizeInfo.uiDisplayFBHeight)
                        DispSizeInfo.uiDisplayFBHeight = pIPPCtrl_obj->IPPctrl_info.uiPath1OutsizeV;
                }

                //gIPPctrl_flowtbl[flow]((void*)pIPPCtrl_obj);
                //Get_IPPCtrlFlow(flow, (void*)pIPPCtrl_obj);
#if 1
                //#NT#2011/09/07#Ethan Lau -begin
                //#NT#assign ime initinal working buf idx
                ime_resetPPBID_P1(IME_PPB_ID_RESET_ENABLE);
                ime_setInitPPBOperatingID_P1(0);
                ime_setReset();
                ime_resetPPBID_P1(IME_PPB_ID_RESET_DISABLE);
                //#NT#2011/09/07#Ethan Lau -end
#endif
                pIPPCtrl_obj->fpIppFlowApi(&pIPPCtrl_obj->IPPctrl_info, pIPPCtrl_rst);//set2preview,set2video...etc.
                m_IPPctrl_CurFlow = flow;
                er_code = E_OK;

                //if dcout en, stable will be called after first jpg frameend
                if(GetIPPFuncInfor(JPG_DCOUT_EN)!=ENABLE)
                {
                    if(g_fpAlgExport)
                        g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);//img is good to go,ready to open ide
                }
            }
            else
            {
                er_code = E_SYS;
                debug_err(("^R assigned buffer is too smalll,pipeline all supported flow are fail \r\n"));
            }
            break;
        case IPP_STOP:
        case IPP_CAP_STOP:
            er_code = E_SYS;
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
            er_code = E_SYS;
            break;
    }
    return er_code;
}

static ER IPPctrl_Close2(PIPPCTRL_OBJ pIPPCtrl_obj)
{
    ER er_code;
    IPPENG_OBJ IPPcallback;

    Get_IPPCB(&IPPcallback);

    switch(pIPPCtrl_obj->opmode)
    {
        case IPP_OPEN:
            sie_open(&IPPcallback.SIEOBJ);
            //#NT#2011/05/20#Connie Yu -begin
            //#NT#update SRA API
            //sra_open();
            //#NT#2011/05/20#Connie Yu -end
            ipe_open(&IPPcallback.IPEOBJ);
            ife_open(&IPPcallback.IFEOBJ);
            ime_open(&IPPcallback.IMEOBJ);
        case IPP_CLOSE:
            er_code = E_OK;
            break;
        case IPP_START:
        case IPP_STOP:
        case IPP_CAP_STOP:
            er_code = E_SYS;
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
            er_code = E_SYS;
        break;
    }
    return er_code;
}

static ER IPPctrl_Start2(PIPPCTRL_OBJ pIPPCtrl_obj)
{
    ER er_code;
    IPPBUF_RST rst;
    ImageAlgInfo *AlgInfo;
    IPPBUF_ENG2DRAMINFO ippbufinfo = {0};
    UINT32 uiCapBufFlow = IPPBUF_MODE_CAP_NORMAL;

    AlgInfo = UI_GetParameter();
    switch(pIPPCtrl_obj->opmode)
    {
        case IPP_OPEN:
        case IPP_START:
        case IPP_CLOSE:
            er_code = E_SYS;
            break;
        case IPP_CAP_STOP:
            ime_waitFlagFrameEnd(IME_CLEAR_FLAG);
            ipe_pause();
            ime_pause();
            ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
            ipe_reset(ON);
            ipe_reset(OFF);
            ime_setReset();
            ipe_close();
            ife_close();
            ime_close();

            if(g_fpAlgExport)
                    g_fpAlgExport(ALGMSG_IPL_STOP, &gAlgMsg);//img is stopped,ready to close ide
            //sie ACT_EN will be disable in sie_set2capture()

            //chk buffer base on driveMode
            if ((AlgInfo->DriveMode == _IMGDriveMode_BurstFlash) ||
                    (AlgInfo->DriveMode == _IMGDriveMode_Burst) ||
                    (AlgInfo->DriveMode == _IMGDriveMode_InfBurst) ||
                    (AlgInfo->DriveMode == _IMGDriveMode_AEBreaketing) ||
                    (AlgInfo->DriveMode == _IMGDriveMode_AWBBreaketing) ||
                    (AlgInfo->DriveMode == _IMGDriveMode_SmallBurst16) ||
                    (AlgInfo->DriveMode == _IMGDriveMode_Single)
                    )
            {

                IPPCTRL_initippbufinfo(pIPPCtrl_obj,&ippbufinfo,uiCapBufFlow);
                //Capture flow Eng cfg
                ippbufinfo.SieBufferNum = ONE_OUTPUT_BUF;
                ippbufinfo.ImeBufferNum[ImePath1] = IME_OUTPUT_PPB_5;
                ippbufinfo.ImeBufferNum[ImePath2] = IME_OUTPUT_PPB_1;
                ippbufinfo.ImeSliceH = SLICE_32;
#if Patch_IMEBUG
                if((GetIPPSizeInfor(_SieCropCapVsize) == 0)||
                    (GetIPPSizeInfor(_ImeCapOutVsize) == 0) )
                {
                    debug_err(("^R%s %d: ime inputV or ime outputV =0!!",__func__,__LINE__));
                }
                while(1)
                {
                    if( (ippbufinfo.ImeSliceH * GetIPPSizeInfor(_SieCropCapVsize)*10
                         / GetIPPSizeInfor(_ImeCapOutVsize)) <= (2*10) ) //enlarge 10x
                    {
                        if(ippbufinfo.ImeSliceH == SLICE_32)
                            ippbufinfo.ImeSliceH = (SLICE_8*(SLICE_2+SLICE_4));
                        else
                        {
                            debug_err(("^R %s %d: ime scaling ratio too large\r\n",__func__,__LINE__));
                            break;
                        }
                    }
                    else
                        break;
                }
#endif
                if(ippbufinfo.ImePath2Enable != ENABLE)
                {
                    debug_err(("%s: Capture flwo Path2 must be enable\r\n",__func__));
                    return E_SYS;
                }

                //so far, capture flow must use F2S or FW framemode
                if (GetIPPFuncInfor(FRAME_MODE_EN) == ENABLE)
                {
                    uiCapBufFlow = IPPBUF_MODE_CAP_NORMAL_FRAME;
                }
                else
                {
                    uiCapBufFlow = IPPBUF_MODE_CAP_NORMAL;
                }
                rst = ippbuf_ExecuteAssigner(uiCapBufFlow,&ippbufinfo);

                if(rst.er_code != E_OK)
                {
                    debug_err(("%s : IPP_SliceMode_F2S buffer is not enough!\r\n",__func__));
                    return rst.er_code;
                }

                Set_IME_OutInfo(IPP_SliceMode_F2S,ImeOutInfo_FMT,(UINT32)ippbufinfo.Ime_Fmt[ImePath1]);
                Set_IME_OutInfo(IPP_FrameMode_FW,ImeOutInfo_FMT,(UINT32)ippbufinfo.Ime_Fmt[ImePath2]);
                Set_IME_OutInfo(IPP_SliceMode_F2S,ImeOutInfo_BufferNum,(UINT32)ippbufinfo.ImeBufferNum[ImePath1]);
                Set_IME_OutInfo(IPP_FrameMode_FW,ImeOutInfo_BufferNum,(UINT32)ippbufinfo.ImeBufferNum[ImePath2]);
                Set_IME_OutInfo(IPP_SliceMode_F2S,ImeOutInfo_SliceH,(UINT32)ippbufinfo.ImeSliceH);
            }
            else
            {
                //not support other drivemode so far
                debug_err(("%d %s\r\n",__LINE__,__func__));
            }
            er_code = E_OK;
            break;
        case IPP_STOP:
            StopPreviewDramData(_Start);
            while(1)
            {
                if(!GetIPLTriggerStop())
                {
                    break;
                }
                sie_waitVD(1);//ethan_20110803
            }
            if(g_fpAlgExport)
                    g_fpAlgExport(ALGMSG_IPL_STOP, &gAlgMsg);//img is stopped,ready to close ide

            //rst isr counter if needed
            RstImeCounter();

            er_code = E_OK;
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
            er_code = E_SYS;
        break;
    }
    return er_code;
}

static ER IPPctrl_Stop2(PIPPCTRL_OBJ pIPPCtrl_obj)
{
    ER er_code;
    switch(pIPPCtrl_obj->opmode)
    {
        case IPP_OPEN:
        case IPP_CAP_STOP:
            er_code = E_SYS;
            break;
        case IPP_START:
            er_code = IPPctrl_Open2(pIPPCtrl_obj);
            break;
        case IPP_STOP:
            er_code = E_OK;
            break;
        case IPP_CLOSE:
            sie_close();
            //#NT#2011/05/20#Connie Yu -begin
            //#NT#update SRA API
            //sra_close();
            //#NT#2011/05/20#Connie Yu -end
            ipe_close();
            ife_close();
            ime_close();
            er_code = E_OK;
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
            er_code = E_SYS;
        break;
    }
    return er_code;
}
static ER IPPctrl_CapStop2(PIPPCTRL_OBJ pIPPCtrl_obj)
{
    ER er_code;
    UINT32 uiPixel;
    IPPENG_OBJ IPPcallback;

    Get_IPPCB(&IPPcallback);

    switch(pIPPCtrl_obj->opmode)
    {
        case IPP_OPEN:
            er_code = E_SYS;
            break;
        case IPP_START:
            ipe_open(&IPPcallback.IPEOBJ);
            ife_open(&IPPcallback.IFEOBJ);
            ime_open(&IPPcallback.IMEOBJ);
            er_code = IPPctrl_Open2(pIPPCtrl_obj);
            break;
        case IPP_STOP:
            uiPixel = sie_getPixelCounter();
            TimerDelayUs(1);
            if ((uiPixel == sie_getPixelCounter()) || (sie_checkFunctionEnable(SIE_CROP_EN) == 0))
            {
            }
            else
            {
                sie_pause();
                sie_waitVD(1);
            }

            //rst isr counter if needed
            RstImeCounter();

            er_code = E_OK;
            break;
        case IPP_CAP_STOP:
            er_code = E_OK;
            break;
        case IPP_CLOSE:
            er_code = E_SYS;
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
            er_code = E_SYS;
        break;
    }
    return er_code;
}

#if 0
#pragma mark - extern func.
#endif
////////////////////////////////////////////////////////////////////////////////
ER IPPCTRL_SetCmd(IPPCTRL_OBJ IPPCtrl_obj)
{
    ER er_code = E_OK;

    /**
    get static global addr
    seems not necessory here,need to be reivew
    */
    IPPCtrl_obj.pIPPctrl_rst = &m_IPPctrl_rst;
    if(IPPCtrl_obj.opmode == IPP_CHGIOSIZE_DZOOM)
    {
        if ((IPPCtrl_obj.IPPctrl_info.uiDzoomStep >= 10) &&
            (IPPCtrl_obj.IPPctrl_info.uiDzoomStep <= Get_DzoomMaxIndex()))
        {
            Set_DzoomIndex(IPPCtrl_obj.IPPctrl_info.uiDzoomStep);
            if(m_IPPctrl_Curobj.IPPctrl_info.ImePath2Enable)
                SetDzoom2Video(Get_DzoomIndex());
            else
                SetDzoom2Preview(Get_DzoomIndex());
            m_IPPctrl_Curobj.IPPctrl_info.uiDzoomStep = IPPCtrl_obj.IPPctrl_info.uiDzoomStep;
            return E_OK;
        }
        else
        {
            debug_err(("^R zoom index overflow %d\r\n", IPPCtrl_obj.IPPctrl_info.uiDzoomStep));
            er_code = E_SYS;
        }
    }
    else
    {
        er_code = m_IPPctrl_Act[m_IPPctrl_Curobj.opmode](&IPPCtrl_obj);
    }
    if(er_code == E_OK)
    {
        IPPCTRL_MSG("IPPctrl opmode from %d to %d\r\n",m_IPPctrl_Curobj.opmode,IPPCtrl_obj.opmode);
        m_IPPctrl_Curobj = IPPCtrl_obj;
    }
    else
        debug_err(("^R IPPCtrl error opmode from %d to %d\r\n",m_IPPctrl_Curobj.opmode,IPPCtrl_obj.opmode));
    return er_code;
}

IPPCTRL_FLOW IPPCTRL_GetCurFlow(void)
{
    return m_IPPctrl_CurFlow;
}
