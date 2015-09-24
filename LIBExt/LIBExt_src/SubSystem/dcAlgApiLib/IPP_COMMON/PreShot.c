/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: PreShot Module                                                  *
* Description:                                                             *
* Author:                                                                  *
****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "PreShot.h"
#include "grph.h"
#include "IDE.h"

PRESHOT_INFO PreShotInfo;

void PSCapCounterReset(void)
{
    PreShotInfo.CurIdx = 0;
}

void PSCapCounterAdd(void)
{
    PreShotInfo.CurIdx ++;
}

UINT32 PSGetCapCounter(void)
{
    return PreShotInfo.CurIdx;
}

void PSParamInit(void)
{
    PreShotInfo.PrvVideo2AddrY = ippbuf_GetBuf(IPPBUF_CAP_PRESHOTPRV);
    PreShotInfo.PrvVideo2AddrCb = PreShotInfo.PrvVideo2AddrY + GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize);
    PreShotInfo.PrvVideo2AddrCr = PreShotInfo.PrvVideo2AddrY + GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize)*3/2;
    PreShotInfo.PrvVideo2SizeV = GetIPPSizeInfor(_ImeOut1Vsize);
    PreShotInfo.PrvVideo2SizeH = GetIPPSizeInfor(_ImeOut1Hsize);
    PreShotInfo.PrvVideo2LineOffset = GetIPPSizeInfor(_ImeOut1Hsize);
    PreShotInfo.CapBufAddr = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    PreShotInfo.CapSizeV = GetIPPSizeInfor(_PreCropCapVsize);
    PreShotInfo.CapSizeH = GetIPPSizeInfor(_PreCropCapHsize);
    PreShotInfo.CapLineOffset= GetIPPSizeInfor(_PreOutLineOffset);
}

void PSCopyPrvYCC(void)
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
    DstAddr = PreShotInfo.PrvVideo2AddrY;
    DstLnOff = PreShotInfo.PrvVideo2LineOffset;
    grph_setImg1(SrcAddr,SrcLnOff,SrcLnCnt,SrcLnLength);
    grph_setImg2(DstAddr,DstLnOff);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    //Cb
    SrcAddr = FB_para.cb_addr;
    SrcLnOff = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    SrcLnCnt = GetIPPSizeInfor(_ImeOut1Vsize);
    SrcLnLength = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    DstAddr = PreShotInfo.PrvVideo2AddrCb;
    DstLnOff = PreShotInfo.PrvVideo2LineOffset/2;
    grph_setImg1(SrcAddr,SrcLnOff,SrcLnCnt,SrcLnLength);
    grph_setImg2(DstAddr,DstLnOff);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    //Cr
    SrcAddr = FB_para.cr_addr;
    SrcLnOff = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    SrcLnCnt = GetIPPSizeInfor(_ImeOut1Vsize);
    SrcLnLength = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    DstAddr = PreShotInfo.PrvVideo2AddrCr;
    DstLnOff = PreShotInfo.PrvVideo2LineOffset/2;
    grph_setImg1(SrcAddr,SrcLnOff,SrcLnCnt,SrcLnLength);
    grph_setImg2(DstAddr,DstLnOff);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    grph_disableDMA();
    grph_close();
}

#if 0
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
#endif

void PSSetCouplePrv(void)
{
    IME_FBADDR_PRAM PS_IDE_Para;

    PS_IDE_Para.y0_addr=PreShotInfo.PrvVideo2AddrY;
    PS_IDE_Para.cb0_addr=PreShotInfo.PrvVideo2AddrCb;
    PS_IDE_Para.cr0_addr=PreShotInfo.PrvVideo2AddrCr;
    PS_IDE_Para.in_format=IME_INPUT_YCC_422;
    PS_IDE_Para.out_format = COLOR_YCBCR422;
    PS_IDE_Para.in_h=PreShotInfo.PrvVideo2SizeH;
    PS_IDE_Para.in_v=PreShotInfo.PrvVideo2SizeV;
    PS_IDE_Para.out_h=PreShotInfo.PrvVideo2SizeH;//
    PS_IDE_Para.out_v=PreShotInfo.PrvVideo2SizeV;//
    PS_IDE_Para.out_lineoffset=PreShotInfo.PrvVideo2SizeH;
    PSSetIDE(&PS_IDE_Para);
}

void PSSetIDE(pIME_FBADDR_PRAM pFB_Para)
{
    VIDEO_BUF_ADDR FB_Addr;
    VIDEO_BUF_ATTR FB_attr;
    VOSD_WINDOW_ATTR FB_win;

    FB_Addr.B0_Y =  pFB_Para->y0_addr;
    FB_Addr.B0_CB = pFB_Para->cb0_addr;
    FB_Addr.B0_CR = pFB_Para->cr0_addr;
    FB_Addr.uiBufSel = IDE_VIDEO_ADDR_SEL_BUFFER0;

    FB_attr.Buf_W = pFB_Para->in_h;
    FB_attr.Buf_H = pFB_Para->in_v;
    FB_attr.Buf_Lineoffset = pFB_Para->out_lineoffset/4;
    FB_attr.Buf_L2R = 0;
    FB_attr.Buf_T2B = 0;
    FB_attr.Buf_BJmode = 2;
    FB_attr.Buf_Opt = 0;
    FB_attr.Buf_Num = 2;

    FB_win.Source_W = pFB_Para->in_h;
    FB_win.Source_H = pFB_Para->in_v;
    FB_win.Des_W = pFB_Para->out_h;
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

