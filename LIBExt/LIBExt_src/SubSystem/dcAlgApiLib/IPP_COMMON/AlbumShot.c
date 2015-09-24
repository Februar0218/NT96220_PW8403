/*
    Album Shot Mode

    This file is the API of album shot.

    @file       AlbumShot.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

/** \addtogroup mISYSAlg */
//@{

#include "AlbumShot.h"
#include "grph.h"
#include "filesystsk.h"
#include "pll.h"
#include "JpgEnc.h"
#include "ide.h"

#define BG_Y_PAT    0x00000000
#define BG_CB_PAT   0x80808080
#define BG_CR_PAT   0x80808080

#define IMG_BG_Y_PAT    0x80808080
#define IMG_BG_CB_PAT   0x80808080
#define IMG_BG_CR_PAT   0x80808080

#define IMG_FRAME_Y_PAT    0xffffffff
#define IMG_FRAME_CB_PAT   0x80808080
#define IMG_FRAME_CR_PAT   0x80808080

#define IMG_PRV_FRAME_H     8
#define IMG_PRV_FRAME_V     4

#define IMG_CAP_FRAME_H     24
#define IMG_CAP_FRAME_V     24
//#define IMG_CAP_FMT         IME_OUT_FMT_420_COSITED
#define IMG_CAP_FMT         IME_OUTPUT_YCC_422_COS

UINT32 PosRatioX[ALBUMSHOT_MAX_NUM] = {6, 52, 30}; // %
UINT32 PosRatioY[ALBUMSHOT_MAX_NUM] = {15, 15, 45}; // %
//UINT32 ImgRatio[ALBUMSHOT_MAX_NUM] = {35, 35, 40};  // %
UINT32 ImgRatio[ALBUMSHOT_MAX_NUM] = {35, 35, 35};  // %
UINT32 ASModeEnable = DISABLE;

ALBUMSHOT_INFO AlbumInfo;

#define AlignTo(a, b) ((a + (b - 1)) / b * b)

/**
    Reset capture counter to 0.

    @return void.
*/
void ASCapCounterReset(void)
{
    AlbumInfo.CurIdx = 0;
}

/**
    Set capture counter +1.

    @return void.
*/
void ASCapCounterAdd(void)
{
    AlbumInfo.CurIdx ++;
}

/**
    Get current capture counter.

    @return current capture counter.
*/
UINT32 ASGetCapCounter(void)
{
    return AlbumInfo.CurIdx;
}

/**
    Get the maximum limit of the counter.

    @return the maximum limit of the counter.
*/
UINT32 ASGetMaxCounter(void)
{
    return ALBUMSHOT_MAX_NUM;
}


/**
    Get image Y/Cb/Cr ratio.

    @param[in] Numerator the numerator of the ratio.
    @param[in] Denominator the denominator of the ratio.
    @return void.
*/
void ASGetBufRatio(UINT32 *Numerator, UINT32 *Denominator)
{
#if (IMG_CAP_FMT == IME_OUT_FMT_420_COSITED)
    *Numerator = 15;
    *Denominator = 10;
#elif (IMG_CAP_FMT == IME_OUTPUT_YCC_422_COS)
    *Numerator = 20;
    *Denominator = 10;
#endif
}

/**
    check album shot mode is start or not.

    @return
        - @b ENABLE: Album shot mode is start.
        - @b DISABLE: Album shot mode is stop.
*/
UINT32 ASModeIsEnable(void)
{
    return ASModeEnable;
}

/**
    Enable album shot mode.

    Initial flow for Album Shot operation.

    @return void.
*/
void ASModeStart(void)
{
    ASModeEnable = ENABLE;
    ASCapCounterReset();
    ASPrvParamInit();
    ASSetDispPos(ASGetCapCounter());
}

/**
    Reset album shot mode.

    Reset all parameters to initial value.

    @return void.
*/
void ASModeReset(void)
{
    SetIPPSizeInfor(_ImeOut1Hsize, AlbumInfo.DispSizeH);
    SetIPPSizeInfor(_ImeOut1Vsize, AlbumInfo.DispSizeV);
    IME_D1_Para.y0_addr = AlbumInfo.DispBufAddrY[0];
    IME_D1_Para.cb0_addr = AlbumInfo.DispBufAddrCb[0];
    IME_D1_Para.cr0_addr = AlbumInfo.DispBufAddrCr[0];
    IME_D1_Para.y1_addr = AlbumInfo.DispBufAddrY[1];
    IME_D1_Para.cb1_addr = AlbumInfo.DispBufAddrCb[1];
    IME_D1_Para.cr1_addr = AlbumInfo.DispBufAddrCr[1];
    IME_D1_Para.y2_addr = AlbumInfo.DispBufAddrY[2];
    IME_D1_Para.cb2_addr = AlbumInfo.DispBufAddrCb[2];
    IME_D1_Para.cr2_addr = AlbumInfo.DispBufAddrCr[2];
    IME_D1_Para.out_h = GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v = GetIPPSizeInfor(_ImeOut1Vsize);

    ASCapCounterReset();
    ASPrvParamInit();
    ASSetDispPos(ASGetCapCounter());
}

/**
    Disable album shot mode.

    Close flow for album shot operation

    @return void.
*/
void ASModeEnd(void)
{
    ASModeEnable = DISABLE;
    ASSetDispFull();
}

/**
    Process raw data to YCC data.

    Process raw to YCC data which output to designate buffer.

    @param[in] Idx buffer index.
    @return void.
*/
void ASImgProcess(UINT32 Idx)
{
//KH mark
#if 0
    IME_MODE_PRAM OneIme;
    IME_OPENOBJ IMEOpenObj;
    IPE_IME_FM_PARAM fmInfo;

    //source
    fmInfo.BayerAddr = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    fmInfo.BayerHsize = SIE_PRE_Para_cap.PreCropSizeH;
    fmInfo.BayerVsize = SIE_PRE_Para_cap.PreCropSizeV;

    //dest
    fmInfo.ImeOutPath = IMEALG_2PATH;
    fmInfo.OutputAddr1Y = AlbumInfo.ImgInfo[Idx].CapImgAddr;
    fmInfo.OutputAddr1Cb = fmInfo.OutputAddr1Y + (AlbumInfo.ImgInfo[Idx].CapImgH * AlbumInfo.ImgInfo[Idx].CapImgV);
    fmInfo.OutputAddr1Cr = fmInfo.OutputAddr1Y + (AlbumInfo.ImgInfo[Idx].CapImgH * AlbumInfo.ImgInfo[Idx].CapImgV * 3 / 2);
    fmInfo.ImeHsize1 = AlbumInfo.ImgInfo[Idx].CapImgH;
    fmInfo.ImeVsize1 = AlbumInfo.ImgInfo[Idx].CapImgV;
    fmInfo.ImeLineoffset1 = AlbumInfo.ImgInfo[Idx].CapImgH;
    fmInfo.ImeFmt1 = IMG_CAP_FMT;

    fmInfo.OutputAddr2Y = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    fmInfo.OutputAddr2Cb = fmInfo.OutputAddr1Y + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize));
    fmInfo.OutputAddr2Cr = fmInfo.OutputAddr1Y + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 3 / 2);
    fmInfo.ImeHsize2 = GetIPPSizeInfor(_QVPreHsize);
    fmInfo.ImeVsize2 = GetIPPSizeInfor(_QVPreVsize);
    fmInfo.ImeLineoffset2 = GetIPPSizeInfor(_QVPreHsize);
    fmInfo.ImeFmt2 = IME_OUTPUT_YCC_422_COS;
    ipe_ime_Framemode(fmInfo);

    //scale to thumbnail
    OneIme.op_mode = _D2D;
//#NT#2009/11/13#Bowen Li -begin
//#set ime bil
    OneIme.bil = IMEALG_BICUBIC;
//#NT#2009/11/13#Bowen Li -end
    OneIme.in_src = IMEALG_DMA2IME;
    OneIme.in_addr.y_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    OneIme.in_addr.cb_addr = OneIme.in_addr.y_addr + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize));
    OneIme.in_addr.cr_addr = OneIme.in_addr.cb_addr + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) / 2);
    OneIme.format.in_format = IMEALG_INYUV422;
    OneIme.path_num = IMEALG_1PATH;
    OneIme.in_size.h_size = GetIPPSizeInfor(_QVPreHsize);
    OneIme.in_size.v_size = GetIPPSizeInfor(_QVPreVsize);
    OneIme.line_ofs.in_lineofs = GetIPPSizeInfor(_QVPreHsize);
    OneIme.p1_out0_addr.y_addr = AlbumInfo.ImgInfo[Idx].ThumbImgAddr;
    OneIme.p1_out0_addr.cb_addr = OneIme.p1_out0_addr.y_addr + (AlbumInfo.ImgInfo[Idx].PrvImgH * AlbumInfo.ImgInfo[Idx].PrvImgV);
    OneIme.p1_out0_addr.cr_addr = OneIme.p1_out0_addr.cb_addr + (AlbumInfo.ImgInfo[Idx].PrvImgH * AlbumInfo.ImgInfo[Idx].PrvImgV / 2);;
    OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
    OneIme.p1_out_size.h_size = AlbumInfo.ImgInfo[Idx].PrvImgH;
    OneIme.p1_out_size.v_size = AlbumInfo.ImgInfo[Idx].PrvImgV;
    OneIme.line_ofs.out1_lineofs = AlbumInfo.ImgInfo[Idx].PrvImgH;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    ime_open(&IMEOpenObj);
#if 0
    pll_enableSystemReset(IME_RSTN);
    pll_disableSystemReset(IME_RSTN);
#endif
    ime_setMode(&OneIme);
    ime_clrIntFlag();
    ime_setStart();
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
    ime_setStop();
#endif
    ime_close();
}

/**
    Form thumbnail image.

    Use capture image YCbCr data to form thumbnail image.

    @return void.
*/
void ASPasteThumbnail(void)
{
    UINT32 i, j;
    UINT32 SourBufY, SourBufCb, SourBufCr, DestBufY, DestBufCb, DestBufCr;
    UINT32 SizeH, SizeV;
    grph_open();
    grph_swReset();
    grph_enableDMA();
    for (i = 0; i < AlbumInfo.CurIdx; i ++)
    {
        SourBufY = AlbumInfo.ImgInfo[i].ThumbImgAddr;
        SourBufCb = AlbumInfo.ImgInfo[i].ThumbImgAddr + (AlbumInfo.ImgInfo[i].PrvImgH * AlbumInfo.ImgInfo[i].PrvImgV);
        SourBufCr = AlbumInfo.ImgInfo[i].ThumbImgAddr + (AlbumInfo.ImgInfo[i].PrvImgH * AlbumInfo.ImgInfo[i].PrvImgV * 3 / 2);
        SizeH = AlbumInfo.ImgInfo[i].PrvImgH;
        SizeV = AlbumInfo.ImgInfo[i].PrvImgV;

        for (j = 0; j < 3; j ++)
        {
            DestBufY = AlbumInfo.DispBufAddrY[j] + AlbumInfo.ImgInfo[i].PrvOfsY;
            DestBufCb = AlbumInfo.DispBufAddrCb[j] + AlbumInfo.ImgInfo[i].PrvOfsCbCr;
            DestBufCr = AlbumInfo.DispBufAddrCr[j] + AlbumInfo.ImgInfo[i].PrvOfsCbCr;

            //y
            grph_setImg1(SourBufY, SizeH, SizeV, SizeH);
            grph_setImg2(DestBufY, AlbumInfo.DispSizeH);
            grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
            //cb
            grph_setImg1(SourBufCb, (SizeH >> 1), SizeV, (SizeH >> 1));
            grph_setImg2(DestBufCb, (AlbumInfo.DispSizeH >> 1));
            grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
            //cr
            grph_setImg1(SourBufCr, (SizeH >> 1), SizeV, (SizeH >> 1));
            grph_setImg2(DestBufCr, (AlbumInfo.DispSizeH >> 1));
            grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
        }
    }
    grph_disableDMA();
    grph_close();
    for (i = AlbumInfo.CurIdx; i < ALBUMSHOT_MAX_NUM; i ++)
    {
        ASDrawDispWindow(i);
    }
}


/**
    Initiate capture parameters.

    Initial function for album shot's capture mode,
    include buffer, image size, and border width.

    @return void.
*/

void ASCapParamInit(void)
{
    UINT32 i, tmp, tmpX, tmpY;

    AlbumInfo.CapBufAddr = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    AlbumInfo.CapSizeH = JPGIMG_SIZE[gImageAlgInfo.IMGSize][0];
    AlbumInfo.CapSizeV = JPGIMG_SIZE[gImageAlgInfo.IMGSize][1];

    //generate image info
    for (i = 0; i < ALBUMSHOT_MAX_NUM; i ++)
    {
        AlbumInfo.ImgInfo[i].CapFrameH = IMG_CAP_FRAME_H; //must be multiple of 8
        AlbumInfo.ImgInfo[i].CapFrameV = IMG_CAP_FRAME_V; //must be multiple of 8


        //capture information
        tmpY = ((AlbumInfo.CapSizeV * PosRatioY[i] / 100) * AlbumInfo.CapSizeH);
        tmpX = AlignTo((AlbumInfo.CapSizeH * PosRatioX[i] / 100), 8);
        tmp = tmpY + tmpX;
        AlbumInfo.ImgInfo[i].CapOfsY = tmp;
#if (IMG_CAP_FMT == IME_OUT_FMT_420_COSITED)
        tmpY = (((AlbumInfo.CapSizeV >> 1) * PosRatioY[i] / 100) * (AlbumInfo.CapSizeH >> 1));
        tmp = tmpY + (tmpX >> 1);
        AlbumInfo.ImgInfo[i].CapOfsCbCr = tmp;
#elif (IMG_CAP_FMT == IME_OUTPUT_YCC_422_COS)
        tmpY = ((AlbumInfo.CapSizeV * PosRatioY[i] / 100) * (AlbumInfo.CapSizeH >> 1));
        tmp = tmpY + (tmpX >> 1);
        AlbumInfo.ImgInfo[i].CapOfsCbCr = tmp;
#endif
        AlbumInfo.ImgInfo[i].CapImgH = AlignTo((AlbumInfo.CapSizeH * ImgRatio[i] / 100), 16);
        AlbumInfo.ImgInfo[i].CapImgV = AlignTo((AlbumInfo.CapSizeV * ImgRatio[i] / 100), 4);
    }
#if (IMG_CAP_FMT == IME_OUT_FMT_420_COSITED)
    //assign cap image address
    for (i = 0; i < ALBUMSHOT_MAX_NUM; i ++)
    {
        AlbumInfo.ImgInfo[i].CapImgAddr = ippbuf_GetBuf(IPPBUF_CAP_JPG_1) +
                                ((AlbumInfo.ImgInfo[i].CapImgV * AlbumInfo.ImgInfo[i].CapImgH * 15 / 10) * i);
    }

    AlbumInfo.ImgInfo[0].ThumbImgAddr = AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].CapImgAddr +
                                (AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].CapImgH * AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].CapImgV * 15 / 10);
#elif (IMG_CAP_FMT == IME_OUTPUT_YCC_422_COS)
    //assign cap image address
    for (i = 0; i < ALBUMSHOT_MAX_NUM; i ++)
    {
        AlbumInfo.ImgInfo[i].CapImgAddr = ippbuf_GetBuf(IPPBUF_CAP_JPG_1) +
                                ((AlbumInfo.ImgInfo[i].CapImgV * AlbumInfo.ImgInfo[i].CapImgH * 2) * i);
    }

    AlbumInfo.ImgInfo[0].ThumbImgAddr = AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].CapImgAddr +
                                (AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].CapImgH * AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].CapImgV * 2);
#endif
    for (i = 1; i < ALBUMSHOT_MAX_NUM; i ++)
    {
        AlbumInfo.ImgInfo[i].ThumbImgAddr = AlbumInfo.ImgInfo[i - 1].ThumbImgAddr + (AlbumInfo.ImgInfo[i].PrvImgV * AlbumInfo.ImgInfo[i].PrvImgH * 2);
    }
    tmp = AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].ThumbImgAddr + (AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].PrvImgV * AlbumInfo.ImgInfo[(ALBUMSHOT_MAX_NUM - 1)].PrvImgH * 2);
    if (tmp > ippbuf_GetBuf(IPPBUF_CAP_CURREND))
    {
        debug_err(("AlbumShot memory overflow 0x%.8x 0x%.8x\r\n", tmp, ippbuf_GetBuf(IPPBUF_CAP_CURREND)));
    }

}

/**
    Initiate preview parameters.

    Initial function for album shot's preview mode,
    include buffer, image size, border width, and background color.

    @return void.
*/
void ASPrvParamInit(void)
{
    UINT32 i, tmp;

    AlbumInfo.BackGroundColor[0] = BG_Y_PAT;
    AlbumInfo.BackGroundColor[1] = BG_CB_PAT;
    AlbumInfo.BackGroundColor[2] = BG_CR_PAT;

    AlbumInfo.DispBufAddrY[0] = IME_D1_Para.y0_addr;
    AlbumInfo.DispBufAddrCb[0] = IME_D1_Para.cb0_addr;
    AlbumInfo.DispBufAddrCr[0] = IME_D1_Para.cr0_addr;
    AlbumInfo.DispBufAddrY[1] = IME_D1_Para.y1_addr;
    AlbumInfo.DispBufAddrCb[1] = IME_D1_Para.cb1_addr;
    AlbumInfo.DispBufAddrCr[1] = IME_D1_Para.cr1_addr;
    AlbumInfo.DispBufAddrY[2] = IME_D1_Para.y2_addr;
    AlbumInfo.DispBufAddrCb[2] = IME_D1_Para.cb2_addr;
    AlbumInfo.DispBufAddrCr[2] = IME_D1_Para.cr2_addr;
    AlbumInfo.DispSizeH = IME_D1_Para.out_h;
    AlbumInfo.DispSizeV = IME_D1_Para.out_v;

    //generate image info
    for (i = 0; i < ALBUMSHOT_MAX_NUM; i ++)
    {
        //display inforamtion
        tmp = ((AlbumInfo.DispSizeV * PosRatioY[i] / 100) * AlbumInfo.DispSizeH);
        tmp = tmp + AlignTo((AlbumInfo.DispSizeH * PosRatioX[i] / 100), 8);//must be multiple of 8 to avoid CbCr/2 miss match word alignment
        AlbumInfo.ImgInfo[i].PrvOfsY = tmp;
        AlbumInfo.ImgInfo[i].PrvOfsCbCr = (tmp >> 1);
        AlbumInfo.ImgInfo[i].PrvImgH = AlignTo((AlbumInfo.DispSizeH * ImgRatio[i] / 100), 16);
        AlbumInfo.ImgInfo[i].PrvImgV = AlignTo((AlbumInfo.DispSizeV * ImgRatio[i] / 100), 4);
        AlbumInfo.ImgInfo[i].BackGroundColor[0] = IMG_BG_Y_PAT;
        AlbumInfo.ImgInfo[i].BackGroundColor[1] = IMG_BG_CB_PAT;
        AlbumInfo.ImgInfo[i].BackGroundColor[2] = IMG_BG_CR_PAT;

        AlbumInfo.ImgInfo[i].FrameColor[0] = IMG_FRAME_Y_PAT;
        AlbumInfo.ImgInfo[i].FrameColor[1] = IMG_FRAME_CB_PAT;
        AlbumInfo.ImgInfo[i].FrameColor[2] = IMG_FRAME_CR_PAT;

        AlbumInfo.ImgInfo[i].PrvFrameH = IMG_PRV_FRAME_H; //must be multiple of 8
        AlbumInfo.ImgInfo[i].PrvFrameV = IMG_PRV_FRAME_V;
    }
}

/**
    Initiate preview background.

    Create a background image for preview operation.

    @return void.
*/
void ASPrvBGInit(void)
{
    UINT32 i, j;
    UINT32 Buf;
    UINT32 SizeH, SizeV;

    grph_open();
    grph_swReset();
    grph_enableDMA();
    for (i = 0; i < 3; i ++)
    {
        //fill display background
        //y
        Buf = AlbumInfo.DispBufAddrY[i];
        SizeH = AlbumInfo.DispSizeH;
        SizeV = AlbumInfo.DispSizeV;
        grph_setImg1(Buf, SizeH, SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[0]);

        //Cb
        Buf = AlbumInfo.DispBufAddrCb[i];
        SizeH = (SizeH >> 1);
        grph_setImg1(Buf, SizeH, SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[1]);

        //Cr
        Buf = AlbumInfo.DispBufAddrCr[i];
        grph_setImg1(Buf, SizeH, SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[2]);
        //draw frame & fill image background
        for (j = 0; j < ALBUMSHOT_MAX_NUM; j ++)
        {
            //frame
            //y
            Buf = AlbumInfo.DispBufAddrY[i];
            Buf = Buf + AlbumInfo.ImgInfo[j].PrvOfsY - (AlbumInfo.ImgInfo[j].PrvFrameV * AlbumInfo.DispSizeH) - AlbumInfo.ImgInfo[j].PrvFrameH;
            SizeH = AlbumInfo.ImgInfo[j].PrvImgH + (AlbumInfo.ImgInfo[j].PrvFrameH * 2);
            SizeV = AlbumInfo.ImgInfo[j].PrvImgV + (AlbumInfo.ImgInfo[j].PrvFrameV * 2);
            grph_setImg1(Buf, AlbumInfo.DispSizeH, SizeV, SizeH);
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[j].FrameColor[0]);

            //Cb
            Buf = AlbumInfo.DispBufAddrCb[i];
            Buf = Buf + AlbumInfo.ImgInfo[j].PrvOfsCbCr - (AlbumInfo.ImgInfo[j].PrvFrameV * (AlbumInfo.DispSizeH >> 1)) - (AlbumInfo.ImgInfo[j].PrvFrameH >> 1);
            SizeH = SizeH >> 1;
            grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[j].FrameColor[1]);

            //Cr
            Buf = AlbumInfo.DispBufAddrCr[i];
            Buf = Buf + AlbumInfo.ImgInfo[j].PrvOfsCbCr - (AlbumInfo.ImgInfo[j].PrvFrameV * (AlbumInfo.DispSizeH >> 1)) - (AlbumInfo.ImgInfo[j].PrvFrameH >> 1);
            grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[j].FrameColor[2]);

            //img background
            //y
            Buf = AlbumInfo.DispBufAddrY[i];
            Buf = Buf + AlbumInfo.ImgInfo[j].PrvOfsY;
            SizeH = AlbumInfo.ImgInfo[j].PrvImgH;
            SizeV = AlbumInfo.ImgInfo[j].PrvImgV;
            grph_setImg1(Buf, AlbumInfo.DispSizeH, SizeV, SizeH);
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[j].BackGroundColor[0]);

            //Cb
            Buf = AlbumInfo.DispBufAddrCb[i];
            Buf = Buf + AlbumInfo.ImgInfo[j].PrvOfsCbCr;
            SizeH = SizeH >> 1;
            grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[j].BackGroundColor[1]);

            //Cr
            Buf = AlbumInfo.DispBufAddrCr[i];
            Buf = Buf + AlbumInfo.ImgInfo[j].PrvOfsCbCr;
            grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[j].BackGroundColor[2]);
        }
    }
    grph_disableDMA();
    grph_close();
}

/**
    Redraw window.

    Redraw the designate window.

    @param[in] WinIdx the index of the window.
    @return void.
*/
void ASDrawDispWindow(UINT32 WinIdx)
{
    UINT32 Buf, i;
    UINT32 SizeH, SizeV;

    //redraw display window
    grph_open();
    grph_swReset();
    grph_enableDMA();

    for (i = 0; i < 3; i ++)
    {
        //frame
        //y
        Buf = AlbumInfo.DispBufAddrY[i];
        Buf = Buf + AlbumInfo.ImgInfo[WinIdx].PrvOfsY - (AlbumInfo.ImgInfo[WinIdx].PrvFrameV * AlbumInfo.DispSizeH) - AlbumInfo.ImgInfo[WinIdx].PrvFrameH;
        SizeH = AlbumInfo.ImgInfo[WinIdx].PrvImgH + (AlbumInfo.ImgInfo[WinIdx].PrvFrameH * 2);
        SizeV = AlbumInfo.ImgInfo[WinIdx].PrvImgV + (AlbumInfo.ImgInfo[WinIdx].PrvFrameV * 2);
        grph_setImg1(Buf, AlbumInfo.DispSizeH, SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[WinIdx].FrameColor[0]);

        //Cb
        Buf = AlbumInfo.DispBufAddrCb[i];
        Buf = Buf + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr - (AlbumInfo.ImgInfo[WinIdx].PrvFrameV * (AlbumInfo.DispSizeH >> 1)) - (AlbumInfo.ImgInfo[WinIdx].PrvFrameH >> 1);
        SizeH = SizeH >> 1;
        grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[WinIdx].FrameColor[1]);


        //Cr
        Buf = AlbumInfo.DispBufAddrCr[i];
        Buf = Buf + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr - (AlbumInfo.ImgInfo[WinIdx].PrvFrameV * (AlbumInfo.DispSizeH >> 1)) - (AlbumInfo.ImgInfo[WinIdx].PrvFrameH >> 1);
        grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[WinIdx].FrameColor[2]);

        //img background
        //y
        Buf = AlbumInfo.DispBufAddrY[i];
        Buf = Buf + AlbumInfo.ImgInfo[WinIdx].PrvOfsY;
        SizeH = AlbumInfo.ImgInfo[WinIdx].PrvImgH;
        SizeV = AlbumInfo.ImgInfo[WinIdx].PrvImgV;
        grph_setImg1(Buf, AlbumInfo.DispSizeH, SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[WinIdx].BackGroundColor[0]);

        //Cb
        Buf = AlbumInfo.DispBufAddrCb[i];
        Buf = Buf + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
        SizeH = SizeH >> 1;
        grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[WinIdx].BackGroundColor[1]);

        //Cr
        Buf = AlbumInfo.DispBufAddrCr[i];
        Buf = Buf + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
        grph_setImg1(Buf, (AlbumInfo.DispSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[WinIdx].BackGroundColor[2]);
    }
    grph_disableDMA();
    grph_close();
}


/**
    Set display image.

    Set display image to designate window.

    @param[in] WinIdx the index of the window.
    @param[in] WaitCmdEnd TRUE or FALSE, wait command finish or not.
    @return void.
*/
void ASSetDispWindow(UINT32 WinIdx, UINT32 WaitCmdEnd)
{

    SetIPPSizeInfor(_ImeOut1Hsize, AlbumInfo.ImgInfo[WinIdx].PrvImgH);
    SetIPPSizeInfor(_ImeOut1Vsize, AlbumInfo.ImgInfo[WinIdx].PrvImgV);
    IME_D1_Para.y0_addr = AlbumInfo.DispBufAddrY[0] + AlbumInfo.ImgInfo[WinIdx].PrvOfsY;
    IME_D1_Para.cb0_addr = AlbumInfo.DispBufAddrCb[0] + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
    IME_D1_Para.cr0_addr = AlbumInfo.DispBufAddrCr[0] + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
    IME_D1_Para.y1_addr = AlbumInfo.DispBufAddrY[1] + AlbumInfo.ImgInfo[WinIdx].PrvOfsY;
    IME_D1_Para.cb1_addr = AlbumInfo.DispBufAddrCb[1] + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
    IME_D1_Para.cr1_addr = AlbumInfo.DispBufAddrCr[1] + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
    IME_D1_Para.y2_addr = AlbumInfo.DispBufAddrY[2] + AlbumInfo.ImgInfo[WinIdx].PrvOfsY;
    IME_D1_Para.cb2_addr = AlbumInfo.DispBufAddrCb[2] + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
    IME_D1_Para.cr2_addr = AlbumInfo.DispBufAddrCr[2] + AlbumInfo.ImgInfo[WinIdx].PrvOfsCbCr;
    IME_D1_Para.out_h = GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v = GetIPPSizeInfor(_ImeOut1Vsize);

//KH mark
#if 0
    ime_clrIntFlag();
#endif
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
    if (Is_FDLock())
    {
        ime_SetDzoom2Video_Path1(&IME_D1_Para);
    }
    else
    {
        ime_SetDzoom2Video(&IME_D1_Para, &IME_D2_Para);
    }
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
}


/**
    Imprint date and time.
    Imprint date and time on primary image.

    @return void.
*/
void ASDateImprint(void)
{
    UINT32 uiXPos, uiYPos;
    DS_STAMP_INFOR BigStamp;
    DS_BACKGROUND_INFOR BackgroundImg;
    if (datastamp_IsEnable() == FALSE)
    {
        return;
    }
    datastamp_GetStampInfor(&BigStamp);

    ///Imprinting
    datastamp_GetPosition(&uiXPos, &uiYPos);
    BackgroundImg.AddrY  = AlbumInfo.CapBufAddr;
    BackgroundImg.AddrCb = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeV * AlbumInfo.CapSizeH);
    BackgroundImg.AddrCr = BackgroundImg.AddrY + (AlbumInfo.CapSizeV * AlbumInfo.CapSizeH) * 3 / 2;
    BackgroundImg.LineOffset = AlbumInfo.CapSizeH;

    //compute stamp position
    uiYPos = (uiYPos * AlbumInfo.CapSizeV / 1024);
    uiYPos &= 0xfffffffe;
    uiXPos = (uiXPos * AlbumInfo.CapSizeH / 1024);
    uiXPos &= 0xfffffff8;//keep it word-align for both Y/Cb/Cr

    //add Y shift for 422 format
    BackgroundImg.AddrY  += uiYPos * BackgroundImg.LineOffset;
    BackgroundImg.AddrCb += (uiYPos) * (BackgroundImg.LineOffset / 2);
    BackgroundImg.AddrCr += (uiYPos) * (BackgroundImg.LineOffset / 2);
    //add X shift for 422 format
    BackgroundImg.AddrY  += uiXPos;
    BackgroundImg.AddrCb += uiXPos / 2;
    BackgroundImg.AddrCr += uiXPos / 2;

    //Imprinting API
    datastamp_ImprintYCC(&BackgroundImg, &BigStamp, ippbuf_GetBuf(IPPBUF_CAP_JPG_1));
}


/**
    Form JPEG file.

    Use primary and thumbnail bit stream to form jpeg file.

    @param[out] JpgAddr jpeg address.
    @param[out] JpgSize jpeg size.
    @return void.
*/
void ASFormJpgFile(UINT32 *JpgAddr, UINT32 *JpgSize)
{
    EXIFEncParam EXIFParam;
    EXIFIMGPARAM MainImgParam;
    EXIFEncParam EXIFParamB;
    JPGCmpParam JPGParam;
//KH mark
#if 0
    IME_MODE_PRAM OneIme;
#endif
    IME_OPENOBJ IMEOpenObj;
    UINT MainImgSize, ThumbImgSize, ThumbImgSizeB, retry;
    UINT32 SourBuf, DestBuf, i, SizeH, SizeV;
    UINT32 Ratio;



    grph_open();
    grph_swReset();
    grph_enableDMA();

#if (IMG_CAP_FMT == IME_OUT_FMT_420_COSITED)
    //y
    DestBuf = AlbumInfo.CapBufAddr;
    grph_setImg1(DestBuf, AlbumInfo.CapSizeH, AlbumInfo.CapSizeV, AlbumInfo.CapSizeH);
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[0]);

    //cb
    DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV);
    grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), (AlbumInfo.CapSizeV >> 1), (AlbumInfo.CapSizeH >> 1));
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[1]);

    //cr
    DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 5 / 4);
    grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), (AlbumInfo.CapSizeV >> 1), (AlbumInfo.CapSizeH >> 1));
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[2]);


    for (i = 0; i < ALBUMSHOT_MAX_NUM; i++)
    {
        //draw frame

        //y
        DestBuf = AlbumInfo.CapBufAddr;
        DestBuf = DestBuf + AlbumInfo.ImgInfo[i].CapOfsY - (AlbumInfo.ImgInfo[i].CapFrameV * AlbumInfo.CapSizeH) - AlbumInfo.ImgInfo[i].CapFrameH;
        SizeH = AlbumInfo.ImgInfo[i].CapImgH + (AlbumInfo.ImgInfo[i].CapFrameH * 2);
        SizeV = AlbumInfo.ImgInfo[i].CapImgV + (AlbumInfo.ImgInfo[i].CapFrameV * 2);
        grph_setImg1(DestBuf, AlbumInfo.CapSizeH, SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[i].FrameColor[0]);

        //cb
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH  * AlbumInfo.CapSizeV);
        DestBuf = DestBuf + AlbumInfo.ImgInfo[i].CapOfsCbCr - ((AlbumInfo.ImgInfo[i].CapFrameV >> 1) * (AlbumInfo.CapSizeH >> 1)) - (AlbumInfo.ImgInfo[i].CapFrameH >> 1);
        SizeH = (AlbumInfo.ImgInfo[i].CapImgH >> 1) + ((AlbumInfo.ImgInfo[i].CapFrameH >> 1) * 2);
        SizeV = (AlbumInfo.ImgInfo[i].CapImgV >> 1) + ((AlbumInfo.ImgInfo[i].CapFrameV >> 1) * 2);
        grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[i].FrameColor[1]);

        //cr
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH  * AlbumInfo.CapSizeV * 5 / 4);
        DestBuf = DestBuf + AlbumInfo.ImgInfo[i].CapOfsCbCr - ((AlbumInfo.ImgInfo[i].CapFrameV >> 1) * (AlbumInfo.CapSizeH >> 1)) - (AlbumInfo.ImgInfo[i].CapFrameH >> 1);
        SizeH = (AlbumInfo.ImgInfo[i].CapImgH >> 1) + ((AlbumInfo.ImgInfo[i].CapFrameH >> 1) * 2);
        SizeV = (AlbumInfo.ImgInfo[i].CapImgV >> 1) + ((AlbumInfo.ImgInfo[i].CapFrameV >> 1) * 2);
        grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[i].FrameColor[2]);

        //paste image
        //y
        SourBuf = AlbumInfo.ImgInfo[i].CapImgAddr;
        DestBuf = AlbumInfo.CapBufAddr + AlbumInfo.ImgInfo[i].CapOfsY;
        grph_setImg1(SourBuf, AlbumInfo.ImgInfo[i].CapImgH, AlbumInfo.ImgInfo[i].CapImgV, AlbumInfo.ImgInfo[i].CapImgH);
        grph_setImg2(DestBuf, AlbumInfo.CapSizeH);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        //cb
        SourBuf = AlbumInfo.ImgInfo[i].CapImgAddr + (AlbumInfo.ImgInfo[i].CapImgH * AlbumInfo.ImgInfo[i].CapImgV);
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV) + AlbumInfo.ImgInfo[i].CapOfsCbCr;
        grph_setImg1(SourBuf, (AlbumInfo.ImgInfo[i].CapImgH >> 1), (AlbumInfo.ImgInfo[i].CapImgV >> 1), (AlbumInfo.ImgInfo[i].CapImgH >> 1));
        grph_setImg2(DestBuf, (AlbumInfo.CapSizeH >> 1));
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        //cr
        SourBuf = AlbumInfo.ImgInfo[i].CapImgAddr + (AlbumInfo.ImgInfo[i].CapImgH * AlbumInfo.ImgInfo[i].CapImgV * 5 / 4);
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 5 / 4) + AlbumInfo.ImgInfo[i].CapOfsCbCr;
        grph_setImg1(SourBuf, (AlbumInfo.ImgInfo[i].CapImgH >> 1), (AlbumInfo.ImgInfo[i].CapImgV >> 1), (AlbumInfo.ImgInfo[i].CapImgH >> 1));
        grph_setImg2(DestBuf, (AlbumInfo.CapSizeH >> 1));
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }

#elif (IMG_CAP_FMT == IME_OUTPUT_YCC_422_COS)
    //y
    DestBuf = AlbumInfo.CapBufAddr;
    grph_setImg1(DestBuf, AlbumInfo.CapSizeH, AlbumInfo.CapSizeV, AlbumInfo.CapSizeH);
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[0]);

    //cb
    DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV);
    grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), AlbumInfo.CapSizeV, (AlbumInfo.CapSizeH >> 1));
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[1]);

    //cr
    DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 3 / 2);
    grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), AlbumInfo.CapSizeV, (AlbumInfo.CapSizeH >> 1));
    grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.BackGroundColor[2]);

    for (i = 0; i < ALBUMSHOT_MAX_NUM; i++)
    {
        //draw frame

        //y
        DestBuf = AlbumInfo.CapBufAddr;
        DestBuf = DestBuf + AlbumInfo.ImgInfo[i].CapOfsY - (AlbumInfo.ImgInfo[i].CapFrameV * AlbumInfo.CapSizeH) - AlbumInfo.ImgInfo[i].CapFrameH;
        SizeH = AlbumInfo.ImgInfo[i].CapImgH + (AlbumInfo.ImgInfo[i].CapFrameH * 2);
        SizeV = AlbumInfo.ImgInfo[i].CapImgV + (AlbumInfo.ImgInfo[i].CapFrameV * 2);
        grph_setImg1(DestBuf, AlbumInfo.CapSizeH, SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[i].FrameColor[0]);

        //cb
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH  * AlbumInfo.CapSizeV);
        DestBuf = DestBuf + AlbumInfo.ImgInfo[i].CapOfsCbCr - (AlbumInfo.ImgInfo[i].CapFrameV * (AlbumInfo.CapSizeH >> 1)) - (AlbumInfo.ImgInfo[i].CapFrameH >> 1);
        SizeH = (AlbumInfo.ImgInfo[i].CapImgH >> 1) + ((AlbumInfo.ImgInfo[i].CapFrameH >> 1) * 2);
        SizeV = AlbumInfo.ImgInfo[i].CapImgV + (AlbumInfo.ImgInfo[i].CapFrameV * 2);
        grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[i].FrameColor[1]);

        //cr
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH  * AlbumInfo.CapSizeV * 3 / 2);
        DestBuf = DestBuf + AlbumInfo.ImgInfo[i].CapOfsCbCr - (AlbumInfo.ImgInfo[i].CapFrameV * (AlbumInfo.CapSizeH >> 1)) - (AlbumInfo.ImgInfo[i].CapFrameH >> 1);
        SizeH = (AlbumInfo.ImgInfo[i].CapImgH >> 1) + ((AlbumInfo.ImgInfo[i].CapFrameH >> 1) * 2);
        SizeV = AlbumInfo.ImgInfo[i].CapImgV + (AlbumInfo.ImgInfo[i].CapFrameV * 2);
        grph_setImg1(DestBuf, (AlbumInfo.CapSizeH >> 1), SizeV, SizeH);
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, AlbumInfo.ImgInfo[i].FrameColor[2]);

        //paste image
        //y
        SourBuf = AlbumInfo.ImgInfo[i].CapImgAddr;
        DestBuf = AlbumInfo.CapBufAddr + AlbumInfo.ImgInfo[i].CapOfsY;
        grph_setImg1(SourBuf, AlbumInfo.ImgInfo[i].CapImgH, AlbumInfo.ImgInfo[i].CapImgV, AlbumInfo.ImgInfo[i].CapImgH);
        grph_setImg2(DestBuf, AlbumInfo.CapSizeH);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        //cb
        SourBuf = AlbumInfo.ImgInfo[i].CapImgAddr + (AlbumInfo.ImgInfo[i].CapImgH * AlbumInfo.ImgInfo[i].CapImgV);
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV) + AlbumInfo.ImgInfo[i].CapOfsCbCr;
        grph_setImg1(SourBuf, (AlbumInfo.ImgInfo[i].CapImgH >> 1), AlbumInfo.ImgInfo[i].CapImgV, (AlbumInfo.ImgInfo[i].CapImgH >> 1));
        grph_setImg2(DestBuf, (AlbumInfo.CapSizeH >> 1));
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

        //cr
        SourBuf = AlbumInfo.ImgInfo[i].CapImgAddr + (AlbumInfo.ImgInfo[i].CapImgH * AlbumInfo.ImgInfo[i].CapImgV * 3 / 2);
        DestBuf = AlbumInfo.CapBufAddr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 3 / 2) + AlbumInfo.ImgInfo[i].CapOfsCbCr;
        grph_setImg1(SourBuf, (AlbumInfo.ImgInfo[i].CapImgH >> 1), AlbumInfo.ImgInfo[i].CapImgV, (AlbumInfo.ImgInfo[i].CapImgH >> 1));
        grph_setImg2(DestBuf, (AlbumInfo.CapSizeH >> 1));
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }
#endif
    grph_disableDMA();
    grph_close();

    ASDateImprint();


    //compress image to jpeg(---->JpgBufAddr)
    JPGParam.RawImageAddr[0] = AlbumInfo.CapBufAddr;
    JPGParam.RawImageAddr[1] = JPGParam.RawImageAddr[0] + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV);
#if (IMG_CAP_FMT == IME_OUT_FMT_420_COSITED)
    JPGParam.RawImageAddr[2] = JPGParam.RawImageAddr[0] + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 5 / 4);
#elif (IMG_CAP_FMT == IME_OUTPUT_YCC_422_COS)
    JPGParam.RawImageAddr[2] = JPGParam.RawImageAddr[0] + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 3 / 2);
#endif
    JPGParam.JPGBSAddr = ippbuf_GetBuf(IPPBUF_CAP_JPG_1);
    JPGParam.JPGBSBufSize = &MainImgSize;
    JPGParam.Width = AlbumInfo.CapSizeH;
    JPGParam.Height = AlbumInfo.CapSizeV;
#if (IMG_CAP_FMT == IME_OUT_FMT_420_COSITED)
    JPGParam.Mode  = JPEG_FORMAT_420;
#elif (IMG_CAP_FMT == IME_OUTPUT_YCC_422_COS)
    JPGParam.Mode  = JPEG_FORMAT_422;
#endif
    JPGParam.bOuputJpgDC = 0;

    Ratio = JPEG_Quality[gImageAlgInfo.Quality];
    retry = 0;
    while(retry < 19)
    {
        MainImgSize = GetJpgBufSize();

        if (E_OK == JpegEncOnePic(&JPGParam, (Ratio - (retry * 5))))
        {
            break;
        }
        retry ++;
    }

//KH mark
#if 0
    //scale to 640 X 480
    OneIme.op_mode = _D2D;
//#NT#2009/11/13#Bowen Li -begin
//#set ime bil
    OneIme.bil = IMEALG_BICUBIC;
//#NT#2009/11/13#Bowen Li -end
    OneIme.in_src = IMEALG_DMA2IME;
    OneIme.in_addr.y_addr = AlbumInfo.CapBufAddr;
    OneIme.in_addr.cb_addr = OneIme.in_addr.y_addr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV);

#if (IMG_CAP_FMT == IME_OUT_FMT_420_COSITED)
    OneIme.in_addr.cr_addr = OneIme.in_addr.y_addr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 5 / 4);
    OneIme.format.in_format = IMEALG_INYUV420;
#elif (IMG_CAP_FMT == IME_OUTPUT_YCC_422_COS)
    OneIme.in_addr.cr_addr = OneIme.in_addr.y_addr + (AlbumInfo.CapSizeH * AlbumInfo.CapSizeV * 5 / 4);
    OneIme.format.in_format = IMEALG_INYUV422;
#endif
    OneIme.in_size.h_size = AlbumInfo.CapSizeH;
    OneIme.in_size.v_size = AlbumInfo.CapSizeV;
    OneIme.line_ofs.in_lineofs = AlbumInfo.CapSizeH;
    OneIme.path_num = IMEALG_1PATH;

    OneIme.p1_out0_addr.y_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    OneIme.p1_out0_addr.cb_addr = OneIme.p1_out0_addr.y_addr + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize));
    OneIme.p1_out0_addr.cr_addr = OneIme.p1_out0_addr.cb_addr + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) / 2);
    OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
    OneIme.p1_out_size.h_size = GetIPPSizeInfor(_QVPreHsize);
    OneIme.p1_out_size.v_size = GetIPPSizeInfor(_QVPreVsize);
    OneIme.line_ofs.out1_lineofs = GetIPPSizeInfor(_QVPreHsize);
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    ime_open(&IMEOpenObj);
#if 0
    pll_enableSystemReset(IME_RSTN);
    pll_disableSystemReset(IME_RSTN);
#endif
    ime_setMode(&OneIme);
    ime_clrIntFlag();
    ime_setStart();
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
    ime_setStop();
    ime_close();

    //scale to 320 X 240
    OneIme.op_mode = _D2D;
//#NT#2009/11/13#Bowen Li -begin
//#set ime bil
    OneIme.bil = IMEALG_BICUBIC;
//#NT#2009/11/13#Bowen Li -end
    OneIme.in_src = IMEALG_DMA2IME;
    OneIme.in_addr.y_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    OneIme.in_addr.cb_addr = OneIme.in_addr.y_addr + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize));
    OneIme.in_addr.cr_addr = OneIme.in_addr.cb_addr + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) / 2);
    OneIme.format.in_format = IMEALG_INYUV422;

    OneIme.in_size.h_size = GetIPPSizeInfor(_QVPreHsize);
    OneIme.in_size.v_size = GetIPPSizeInfor(_QVPreVsize);
    OneIme.line_ofs.in_lineofs = GetIPPSizeInfor(_QVPreHsize);
    OneIme.path_num = IMEALG_1PATH;

    OneIme.p1_out0_addr.y_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY);
    OneIme.p1_out0_addr.cb_addr = OneIme.p1_out0_addr.y_addr + (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize));
    OneIme.p1_out0_addr.cr_addr = OneIme.p1_out0_addr.cb_addr + (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) / 2);
    OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
    OneIme.p1_out_size.h_size = GetIPPSizeInfor(_QVHsize);
    OneIme.p1_out_size.v_size = GetIPPSizeInfor(_QVVsize);
    OneIme.line_ofs.out1_lineofs = GetIPPSizeInfor(_QVHsize);
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    ime_open(&IMEOpenObj);
#if 0
    pll_enableSystemReset(IME_RSTN);
    pll_disableSystemReset(IME_RSTN);
#endif
    ime_setMode(&OneIme);
    ime_clrIntFlag();
    ime_setStart();
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
    ime_setStop();
    ime_close();

    //scale to 160 X 120
    OneIme.op_mode = _D2D;
//#NT#2009/11/13#Bowen Li -begin
//#set ime bil
    OneIme.bil = IMEALG_BICUBIC;
//#NT#2009/11/13#Bowen Li -end
    OneIme.in_src = IMEALG_DMA2IME;
    OneIme.in_addr.y_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY);
    OneIme.in_addr.cb_addr = OneIme.in_addr.y_addr + (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize));
    OneIme.in_addr.cr_addr = OneIme.in_addr.cb_addr + (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) / 2);
    OneIme.format.in_format = IMEALG_INYUV422;

    OneIme.in_size.h_size = GetIPPSizeInfor(_QVHsize);
    OneIme.in_size.v_size = GetIPPSizeInfor(_QVVsize);
    OneIme.line_ofs.in_lineofs = GetIPPSizeInfor(_QVHsize);
    OneIme.path_num = IMEALG_1PATH;

    OneIme.p1_out0_addr.y_addr = ippbuf_GetBuf(IPPBUF_CAP_QV_THUMBNAIL);
    OneIme.p1_out0_addr.cb_addr = OneIme.p1_out0_addr.y_addr + (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize));
    OneIme.p1_out0_addr.cr_addr = OneIme.p1_out0_addr.cb_addr + (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) / 2);
    OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
    OneIme.p1_out_size.h_size = GetIPPSizeInfor(_ThumbHsize);
    OneIme.p1_out_size.v_size = GetIPPSizeInfor(_ThumbVsize);
    OneIme.line_ofs.out1_lineofs = GetIPPSizeInfor(_ThumbHsize);
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    ime_open(&IMEOpenObj);
#if 0
    pll_enableSystemReset(IME_RSTN);
    pll_disableSystemReset(IME_RSTN);
#endif
    ime_setMode(&OneIme);
    ime_clrIntFlag();
    ime_setStart();
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
    ime_setStop();
#endif
    ime_close();

    EXIFParamB.RawImageAddr_Y  = ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY);
    EXIFParamB.RawImageAddr_CB = EXIFParamB.RawImageAddr_Y + (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize));
    EXIFParamB.RawImageAddr_CR = EXIFParamB.RawImageAddr_Y + (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 3 / 2);
    EXIFParamB.EXIFBSAddr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    EXIFParamB.EXIFBufSize = &ThumbImgSizeB;
    EXIFParamB.Width = GetIPPSizeInfor(_QVHsize);
    EXIFParamB.Height = GetIPPSizeInfor(_QVVsize);
    EXIFParamB.Mode = (JPEG_FORMAT_422);
    //#NT#2011/06/16#Ben Wang -begin
    //#NT#refine UserJpeg API
    //EXIFParamB.bOuputJpgDC = 0;
    EXIFParamB.DCOutCfg.DCEnable= 0;
    //#NT#2011/06/16#Ben Wang -end
    retry = 0;
    while(retry < 16)
    {

        ThumbImgSizeB = (40 * 1024); //40K
        if (E_OK == JpegEncOnePic((PJPGCmpParam)&EXIFParamB, (80 - (retry * 5))))
        {
            break;
        }
        retry ++;
    }

    //encode 160 X 120 thumbnail
    EXIFParam.RawImageAddr_Y = ippbuf_GetBuf(IPPBUF_CAP_QV_THUMBNAIL);
    EXIFParam.RawImageAddr_CB = EXIFParam.RawImageAddr_Y + (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize));
    EXIFParam.RawImageAddr_CR = EXIFParam.RawImageAddr_Y + (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 3 / 2);
    EXIFParam.EXIFBSAddr = ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY);
    EXIFParam.EXIFBufSize = &ThumbImgSize;
    EXIFParam.Width = AlbumInfo.CapSizeH;
    EXIFParam.Height = AlbumInfo.CapSizeV;
    EXIFParam.Mode = JPEG_FORMAT_422;
    //#NT#2011/06/16#Ben Wang -begin
    //#NT#refine UserJpeg API
    //EXIFParam.bOuputJpgDC = 0;
    EXIFParam.DCOutCfg.DCEnable= 0;
    //#NT#2011/06/16#Ben Wang -end
    retry = 0;
    while(retry < 16)
    {
        ThumbImgSize = (10 * 1024); //10K
        if (E_OK == JpegEncThumbnail(&EXIFParam, (80 - (retry * 5))))
        {
            break;
        }
        retry ++;
    }
    MainImgParam.EXIFIMGAddr = (UINT32 *)&JPGParam.JPGBSAddr;
    MainImgParam.EXIFIMGBufSize = (UINT32 *)&MainImgSize;

    //copy header(---->HeaderBufAddr+JpgBufAddr)
    JpegCopyEXIFHeader2(&MainImgParam, &EXIFParam, &EXIFParamB);
    Write_EXIF_Infor(*MainImgParam.EXIFIMGAddr);

    *JpgAddr = *MainImgParam.EXIFIMGAddr;
    *JpgSize = *MainImgParam.EXIFIMGBufSize;
}

/**
    Set display position.

    Set display position to designate index.

    @param[in] Idx the index of the display position.
    @return void.
*/
void ASSetDispPos(UINT32 Idx)
{
    ide_disable_video(IDE_VIDEOID_1);

    //set display window
    ASSetDispWindow(Idx, 0);
    //draw background
    ASPrvBGInit();

    if (Idx != 0)
    {
        //paste thumbnail
        ASPasteThumbnail();
    }
    //draw window
    ASDrawDispWindow(Idx);

    ide_enable_video(IDE_VIDEOID_1);
}

/**
    Set full screen display.

    Chnage display type to full screen mode..
    @return void.
*/
void ASSetDispFull(void)
{
    ide_disable_video(IDE_VIDEOID_1);

    SetIPPSizeInfor(_ImeOut1Hsize, AlbumInfo.DispSizeH);
    SetIPPSizeInfor(_ImeOut1Vsize, AlbumInfo.DispSizeV);
    IME_D1_Para.y0_addr = AlbumInfo.DispBufAddrY[0];
    IME_D1_Para.cb0_addr = AlbumInfo.DispBufAddrCb[0];
    IME_D1_Para.cr0_addr = AlbumInfo.DispBufAddrCr[0];
    IME_D1_Para.y1_addr = AlbumInfo.DispBufAddrY[1];
    IME_D1_Para.cb1_addr = AlbumInfo.DispBufAddrCb[1];
    IME_D1_Para.cr1_addr = AlbumInfo.DispBufAddrCr[1];
    IME_D1_Para.y2_addr = AlbumInfo.DispBufAddrY[2];
    IME_D1_Para.cb2_addr = AlbumInfo.DispBufAddrCb[2];
    IME_D1_Para.cr2_addr = AlbumInfo.DispBufAddrCr[2];
    IME_D1_Para.out_h = GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v = GetIPPSizeInfor(_ImeOut1Vsize);
//KH mark
#if 0
    ime_clrIntFlag();
#endif
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
    if (Is_FDLock())
    {
        ime_SetDzoom2Video_Path1(&IME_D1_Para);
    }
    else
    {
        ime_SetDzoom2Video(&IME_D1_Para, &IME_D2_Para);
    }
    ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);

    ide_enable_video(IDE_VIDEOID_1);
}


/**
    Set display type.

    Set display type when in album shot mode.

    @param[in] DispType display type.
    @return void.
*/
void ASChgDisplayType(ALBUM_DISP_TYPE DispType)
{
    if (DispType >= DISP_TOTAL)
    {
        debug_err(("display type fail %d\r\n", DispType));
        return;
    }
    switch(DispType)
    {
        case DISP_FULL:
            ASSetDispFull();
            break;

        default:
        case DISP_NORMAL:
            ASSetDispPos(ASGetCapCounter());
            break;
    }
}

//@}
