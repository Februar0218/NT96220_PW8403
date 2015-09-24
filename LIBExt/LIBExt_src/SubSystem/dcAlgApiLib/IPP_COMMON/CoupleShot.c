/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Couple Shot Module                                                  *
* Description:                                                             *
* Author:                                                                  *
****************************************************************************/
#include "CoupleShot.h"
#include "grph.h"
#include "IDE.h"

COUPLESHOT_INFO CoupleInfo;

void CSCapCounterReset(void)
{
    CoupleInfo.CurIdx = 0;
}

void CSCapCounterAdd(void)
{
    CoupleInfo.CurIdx ++;
}

UINT32 CSGetCapCounter(void)
{
    return CoupleInfo.CurIdx;
}

void CSParamInit(void)
{
    CoupleInfo.PrvVideo2AddrY = ippbuf_GetBuf(IPPBUF_CAP_COUPLESHOTPRV);
    CoupleInfo.PrvVideo2AddrCb = CoupleInfo.PrvVideo2AddrY + GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize);
    CoupleInfo.PrvVideo2AddrCr = CoupleInfo.PrvVideo2AddrY + GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize)*3/2;
    CoupleInfo.PrvVideo2SizeV = GetIPPSizeInfor(_ImeOut1Vsize);
    CoupleInfo.PrvVideo2SizeH = GetIPPSizeInfor(_ImeOut1Hsize);
    CoupleInfo.PrvVideo2LineOffset = GetIPPSizeInfor(_ImeOut1Hsize);
    CoupleInfo.CapBufAddr0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    CoupleInfo.CapBufAddr1 = ippbuf_GetBuf(IPPBUF_CAP_RAW_2);
    CoupleInfo.CapSizeV = GetIPPSizeInfor(_PreCropCapVsize);
    CoupleInfo.CapSizeH = GetIPPSizeInfor(_PreCropCapHsize);
    CoupleInfo.CapLineOffset= GetIPPSizeInfor(_PreOutLineOffset);
}

void CSCopyPrvYCC(void)
{
    UINT32 SrcAddr, SrcLnOff, SrcLnCnt, SrcLnLength;
    UINT32 DstAddr, DstLnOff;
    ACTFBADDR_PRAM  FB_para;

    //get IME active buffer address
    GetActImgFBAddr_Path1(&FB_para);

    //Graphic copy
    grph_open();
    grph_swReset();
    grph_enableDMA();

    //Y
    SrcAddr = FB_para.y_addr;
    SrcLnOff = GetIPPSizeInfor(_ImeOut1Hsize);
    SrcLnCnt = GetIPPSizeInfor(_ImeOut1Vsize);
    SrcLnLength = GetIPPSizeInfor(_ImeOut1Hsize);
    DstAddr = CoupleInfo.PrvVideo2AddrY;
    DstLnOff = CoupleInfo.PrvVideo2LineOffset;
    grph_setImg1(SrcAddr,SrcLnOff,SrcLnCnt,SrcLnLength);
    grph_setImg2(DstAddr,DstLnOff);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    //Cb
    SrcAddr = FB_para.cb_addr;
    SrcLnOff = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    SrcLnCnt = GetIPPSizeInfor(_ImeOut1Vsize);
    SrcLnLength = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    DstAddr = CoupleInfo.PrvVideo2AddrCb;
    DstLnOff = CoupleInfo.PrvVideo2LineOffset/2;
    grph_setImg1(SrcAddr,SrcLnOff,SrcLnCnt,SrcLnLength);
    grph_setImg2(DstAddr,DstLnOff);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    //Cr
    SrcAddr = FB_para.cr_addr;
    SrcLnOff = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    SrcLnCnt = GetIPPSizeInfor(_ImeOut1Vsize);
    SrcLnLength = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    DstAddr = CoupleInfo.PrvVideo2AddrCr;
    DstLnOff = CoupleInfo.PrvVideo2LineOffset/2;
    grph_setImg1(SrcAddr,SrcLnOff,SrcLnCnt,SrcLnLength);
    grph_setImg2(DstAddr,DstLnOff);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    grph_disableDMA();
    grph_close();
}

void CSMergeCapRAW(void)
{
    UINT32 SrcAddr, SrcLnOff, SrcLnCnt, SrcLnLength;
    UINT32 DstAddr, DstLnOff;

    //Graphic copy
    grph_open();
    grph_swReset();
    grph_enableDMA();

    //RAW
    SrcAddr = ippbuf_GetBuf(IPPBUF_CAP_RAW_2) + CoupleInfo.CapLineOffset/2;
    SrcLnOff = CoupleInfo.CapLineOffset;
    SrcLnCnt = CoupleInfo.CapSizeV;
    SrcLnLength = CoupleInfo.CapLineOffset/2;
    DstAddr = ippbuf_GetBuf(IPPBUF_CAP_RAW_1) + CoupleInfo.CapLineOffset/2;
    DstLnOff = CoupleInfo.CapLineOffset;
    grph_setImg1(SrcAddr,SrcLnOff,SrcLnCnt,SrcLnLength);
    grph_setImg2(DstAddr,DstLnOff);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    grph_disableDMA();
    grph_close();
}

void CSSetCouplePrv(void)
{
    IME_FBADDR_PRAM CS_IDE_Para;

    CS_IDE_Para.y0_addr=CoupleInfo.PrvVideo2AddrY;
    CS_IDE_Para.cb0_addr=CoupleInfo.PrvVideo2AddrCb;
    CS_IDE_Para.cr0_addr=CoupleInfo.PrvVideo2AddrCr;
    CS_IDE_Para.in_format=IME_INPUT_YCC_422;
    CS_IDE_Para.out_format = COLOR_YCBCR422;
    CS_IDE_Para.in_h=CoupleInfo.PrvVideo2SizeH;
    CS_IDE_Para.in_v=CoupleInfo.PrvVideo2SizeV;
    CS_IDE_Para.out_h=CoupleInfo.PrvVideo2SizeH;//
    CS_IDE_Para.out_v=CoupleInfo.PrvVideo2SizeV;//
    CS_IDE_Para.out_lineoffset=CoupleInfo.PrvVideo2SizeH;
    CSSetIDE(&CS_IDE_Para);
}

void CSSetIDE(pIME_FBADDR_PRAM pFB_Para)
{
    VIDEO_BUF_ADDR FB_Addr;
    VIDEO_BUF_ATTR FB_attr;
    VOSD_WINDOW_ATTR FB_win;

    FB_Addr.B0_Y =  pFB_Para->y0_addr;
    FB_Addr.B0_CB = pFB_Para->cb0_addr;
    FB_Addr.B0_CR = pFB_Para->cr0_addr;
    FB_Addr.uiBufSel = IDE_VIDEO_ADDR_SEL_BUFFER0;

    FB_attr.Buf_W = pFB_Para->in_h/2;
    FB_attr.Buf_H = pFB_Para->in_v;
    FB_attr.Buf_Lineoffset = pFB_Para->out_lineoffset/4;
    FB_attr.Buf_L2R = 0;
    FB_attr.Buf_T2B = 0;
    FB_attr.Buf_BJmode = 2;
    FB_attr.Buf_Opt = 0;
    FB_attr.Buf_Num = 2;

    FB_win.Source_W = pFB_Para->in_h/2;
    FB_win.Source_H = pFB_Para->in_v;
    FB_win.Des_W = pFB_Para->out_h/2;
    FB_win.Des_H = pFB_Para->out_v;
    FB_win.Win_Format =  pFB_Para->out_format;
    FB_win.Win_X = 0;
    FB_win.Win_Y = 0;
    FB_win.High_Addr = 0;

    // blending Video1 and Video2
    ide_wait_frame_end();
    ide_set_v1_blend_v2(IDE_VIDEO_BLEND_VIDEO2_1_2);
    ide_set_video_buf_addr(IDE_VIDEOID_2, &FB_Addr);
    ide_set_video_buf_attr(IDE_VIDEOID_2, &FB_attr);
    ide_set_video_win_attr(IDE_VIDEOID_2, &FB_win);
    ide_set_load();
}

