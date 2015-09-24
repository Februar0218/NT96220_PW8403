#include "ImageQualitySim_int.h"
#include "NT96220_BASIC_IO_Reg.h"

#if (IME_DEBUG_FLOW == ENABLE)
#define debug_ime   debug_err
#else
#define debug_ime   debug_ind
#endif

void IQSim_IME_SetScaleIQ(IME_MODE_PRAM *imeParam, int IQSimMode)
{
    //scaling interpolation method
    imeParam->bil = IMEALG_BILINEAR;
    //scaling filter
    imeParam->p1_out_scale_filter.sfCoefCalMode = IME_SCALE_FILTER_COEF_AUTO_MODE; //use default
    imeParam->p1_out_scale_filter.hsfEN = 0;
    imeParam->p1_out_scale_filter.hFilterCoef = 0;
    imeParam->p1_out_scale_filter.vsfEN = 0;
    imeParam->p1_out_scale_filter.vFilterCoef = 0;
}

void IQSim_IME_SetIQ(IME_MODE_PRAM *imeParam, int IQSimMode)
{
    IME_IQ_FLOW_INFO* imeIqInfo = 0;
    imeIqInfo = imeParam->imeIqInfo;
    //CST/ICST    
    imeIqInfo->ColorSpaceTrans.cstEN = IME_CST_DISABLE;
    imeIqInfo->ColorSpaceTrans.cstSel = IME_CST_RGB2YCC;
    //chroma filter
    imeIqInfo->ChromaFilterInfo.HavgEN = IME_HAVG_DISABLE;
    imeIqInfo->ChromaFilterInfo.HavgSPLEN = IME_HAVG_SPL_DISABLE;
    imeIqInfo->ChromaFilterInfo.HavgPWSel = IME_AVG_PXLWIDTH_2;
    imeIqInfo->ChromaFilterInfo.HavgCoef = 0;
    imeIqInfo->ChromaFilterInfo.HavgTh = 0;
    imeIqInfo->ChromaFilterInfo.HavgSPLTap = 0;
    //curve mapping
    imeIqInfo->CurveMappingInfo.CrvEn = DISABLE;
    imeIqInfo->CurveMappingInfo.StepSel = IME_CRV_STEP_1;
    imeIqInfo->CurveMappingInfo.CenterCb = 0x80;
    imeIqInfo->CurveMappingInfo.CenterCr = 0x80;
    imeIqInfo->CurveMappingInfo.uCrvMapLUT = NULL;
    //filmG
    imeIqInfo->FileGrainInfo.fgEn = DISABLE;
    imeIqInfo->FileGrainInfo.fgW0 = 0;
    imeIqInfo->FileGrainInfo.fgW1 = 0;
    imeIqInfo->FileGrainInfo.fgVal = 0;
}

void IQSim_IME_SetIQ_2(IME_MODE_PRAM *imeParam, int IQSimMode)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_IME_REG_STRUCT* pIme = &(pBin->Ime);

    IME_IQ_FLOW_INFO* imeIqInfo = 0;
    imeIqInfo = imeParam->imeIqInfo;
    //CST/ICST    
    imeIqInfo->ColorSpaceTrans.cstEN = IME_CST_DISABLE;
    imeIqInfo->ColorSpaceTrans.cstSel = IME_CST_RGB2YCC;
    //chroma filter
    imeIqInfo->ChromaFilterInfo.HavgEN = pIme->IME_Register_59.Bit.HAVGEN;
    imeIqInfo->ChromaFilterInfo.HavgSPLEN = pIme->IME_Register_59.Bit.HAVG_SPL_ONLY_EN;
    imeIqInfo->ChromaFilterInfo.HavgPWSel = pIme->IME_Register_59.Bit.HAVGSIZE;
    imeIqInfo->ChromaFilterInfo.HavgCoef = pIme->IME_Register_59.Bit.HAVGCOEFF;
    imeIqInfo->ChromaFilterInfo.HavgTh = pIme->IME_Register_59.Bit.HAVG_TH;
    imeIqInfo->ChromaFilterInfo.HavgSPLTap = pIme->IME_Register_59.Bit.HAVG_SPL_TAP;
    //curve mapping
    imeIqInfo->CurveMappingInfo.CrvEn = pIme->IME_Register_60.Bit.CRVMAP_EN;
    imeIqInfo->CurveMappingInfo.StepSel = pIme->IME_Register_60.Bit.CRVMAP_DELTA;
    imeIqInfo->CurveMappingInfo.CenterCb = pIme->IME_Register_60.Bit.CRVMAP_CENTER_CB;
    imeIqInfo->CurveMappingInfo.CenterCr = pIme->IME_Register_60.Bit.CRVMAP_CENTER_CR;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIme->IME_Register_61.Word);
        for(i=0;i<=16;i++)
            imeIqInfo->CurveMappingInfo.uCrvMapLUT[i] = (pSrc[i] & 0x3f); //6 bits only
    }
    //filmG
    imeIqInfo->FileGrainInfo.fgEn = pIme->IME_Register_58.Bit.FMGRN_1_EN;
    imeIqInfo->FileGrainInfo.fgW0 = pIme->IME_Register_58.Bit.FMGRN_1_WEIGHT0;
    imeIqInfo->FileGrainInfo.fgW1 = pIme->IME_Register_58.Bit.FMGRN_1_WEIGHT1;
    imeIqInfo->FileGrainInfo.fgVal = pIme->IME_Register_58.Bit.FMGRN_1_VAL;
}

void IQSim_IME_DumpIQ(void)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_IME_REG_STRUCT* pIme = &(pBin->Ime);

    sprintf(SLOG_buf, "//IME parameters-------------------------------------------------------\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) IME_CHROMA_FILTER_INFO IQ_IME_ChromaFilter=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  EN, SPLEN, PWSel, Coef, Th, SPLTap\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d\r\n",
        pIme->IME_Register_59.Bit.HAVGEN,
        pIme->IME_Register_59.Bit.HAVG_SPL_ONLY_EN,
        pIme->IME_Register_59.Bit.HAVGSIZE,
        pIme->IME_Register_59.Bit.HAVGCOEFF,
        pIme->IME_Register_59.Bit.HAVG_TH,
        pIme->IME_Register_59.Bit.HAVG_SPL_TAP
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) IME_CURVE_MAPPING_INFO IQ_IME_crvmapSet=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  EN, StepSel, CenterCb, CenterCr\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        pIme->IME_Register_60.Bit.CRVMAP_EN,
        pIme->IME_Register_60.Bit.CRVMAP_DELTA,
        pIme->IME_Register_60.Bit.CRVMAP_CENTER_CB,
        pIme->IME_Register_60.Bit.CRVMAP_CENTER_CR
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) UINT32 IQ_IME_crvmapLUT[16]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  16 LUT values\r\n");SLOG_OUT;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIme->IME_Register_61.Word);
        i=0;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
            pSrc[i+0],pSrc[i+1],pSrc[i+2],pSrc[i+3],
            pSrc[i+4],pSrc[i+5],pSrc[i+6],pSrc[i+7],
            pSrc[i+8],pSrc[i+9],pSrc[i+10],pSrc[i+11],
            pSrc[i+12],pSrc[i+13],pSrc[i+14],pSrc[i+15]
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) IME_FILM_GRAIN_INFO IQ_IME_filmGrain=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  EN, W0, W1, Val\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        pIme->IME_Register_58.Bit.FMGRN_1_EN,
        pIme->IME_Register_58.Bit.FMGRN_1_WEIGHT0,
        pIme->IME_Register_58.Bit.FMGRN_1_WEIGHT1,
        pIme->IME_Register_58.Bit.FMGRN_1_VAL
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
}


//////////////////////////////////////////////////////////////////
void IQSim_IME_SetSrc(IME_MODE_PRAM *imeParam, int IQSimMode)
{
    UINT32 IPEOut_w = gBuf_SIE.w - 16; //NOTE! left 8 pixels and right 8 pixels will be dropped by IPE
    UINT32 IPEOut_h = gBuf_SIE.h;
    
    //input format
    imeParam->format.in_format = IME_INPUT_YCC_444;//direct path ipe always out 444 fmt
    //input image size
    imeParam->in_size.h_size = IPEOut_w;
    imeParam->in_size.v_size = IPEOut_h;
    //if(imeParam->op_mode == IPE2IME_SLICE_MODE)
    //    imeParam->in_size.v_size -= 8;//cause ipe out vertical will be minus 8;
    //input lineoffset
    imeParam->line_ofs.in_lineofs.LineOffsetY = IPEOut_w;
    imeParam->line_ofs.in_lineofs.LineOffsetCb = IPEOut_w/2;
    imeParam->line_ofs.in_lineofs.LineOffsetCr = IPEOut_w/2;
    //input DMA address (only for D2D)
    imeParam->in_addr.y_addr = 0;
    imeParam->in_addr.cb_addr = 0;
    imeParam->in_addr.cr_addr = 0;
}

void IQSim_IME_SetDest(IME_MODE_PRAM *imeParam, int IQSimMode, SLICE_CTRL* pSliceCtrl)
{
    //output format
    imeParam->format.out1_format = IME_OUTPUT_YCC_422_COS; //or IME_OUTPUT_YCC_422_CEN
    //output bayer cfa (when output format is IME_OUTPUT_BAYER)
    imeParam->p1_out_cfa = IME_BAYER_RGGB;
    //output image size
    imeParam->p1_out_size.h_size = gBuf_JPG.w;
    imeParam->p1_out_size.v_size = gBuf_JPG.h;
    //output lineoffset
    imeParam->line_ofs.p1_out_lineofs.LineOffsetY = gBuf_IME[0].lineoff;
    imeParam->line_ofs.p1_out_lineofs.LineOffsetCb = gBuf_IME[0].lineoff/2;
    imeParam->line_ofs.p1_out_lineofs.LineOffsetCr = gBuf_IME[0].lineoff/2;

    if(imeParam->op_mode == IPE2IME_SLICE_MODE)
    {
        IME_HV_STRIPE_INFO _ImeStripeInfo;
        _IME_SLICE_HEIGHT_SEL SliceH = SLICE_32_LINES;
        //output slice height
        UINT32 sh = pSliceCtrl->slice_h;
        //output buffer number
        UINT32 buf_n = pSliceCtrl->total_id;
        UINT32 shift_x = 0;
        imeParam->p1_out_addr.oPPBNums = buf_n-1;
#ifdef SHIFT_IFE_YCC   
#else
        shift_x = IFE_X_OVERLAP;
#endif

        //output DMA address
        switch(buf_n)
        {
        case 5:
            imeParam->p1_out_addr.oDMA4.y_addr = gBuf_IME[4].addr[0] + shift_x;
            imeParam->p1_out_addr.oDMA4.cb_addr = gBuf_IME[4].addr[1] + shift_x;
            imeParam->p1_out_addr.oDMA4.cr_addr = gBuf_IME[4].addr[2] + shift_x;
        case 4:
            imeParam->p1_out_addr.oDMA3.y_addr = gBuf_IME[3].addr[0] + shift_x;
            imeParam->p1_out_addr.oDMA3.cb_addr = gBuf_IME[3].addr[1] + shift_x;
            imeParam->p1_out_addr.oDMA3.cr_addr = gBuf_IME[3].addr[2] + shift_x;
        case 3:
            imeParam->p1_out_addr.oDMA0.y_addr = gBuf_IME[0].addr[0] + shift_x;
            imeParam->p1_out_addr.oDMA0.cb_addr = gBuf_IME[0].addr[1] + shift_x;
            imeParam->p1_out_addr.oDMA0.cr_addr = gBuf_IME[0].addr[2] + shift_x;
        case 2:
            imeParam->p1_out_addr.oDMA1.y_addr = gBuf_IME[1].addr[0] + shift_x;
            imeParam->p1_out_addr.oDMA1.cb_addr = gBuf_IME[1].addr[1] + shift_x;
            imeParam->p1_out_addr.oDMA1.cr_addr = gBuf_IME[1].addr[2] + shift_x;
        case 1:
        default:
            imeParam->p1_out_addr.oDMA2.y_addr = gBuf_IME[2].addr[0] + shift_x;
            imeParam->p1_out_addr.oDMA2.cb_addr = gBuf_IME[2].addr[1] + shift_x;
            imeParam->p1_out_addr.oDMA2.cr_addr = gBuf_IME[2].addr[2] + shift_x;
            break;
        }
        
        //set slice height
        if(sh == 32)
            SliceH = SLICE_32_LINES;
        else if(sh == 48)
            SliceH = SLICE_48_LINES;
        else if(sh == 64)
            SliceH = SLICE_64_LINES;
        else if(sh == 128)
            SliceH = SLICE_128_LINES;
        else if(sh == 256)
            SliceH = SLICE_256_LINES;
        else if(sh == 512)
            SliceH = SLICE_512_LINES;
        else if(sh == 1024)
            SliceH = SLICE_1024_LINES;
        else if(sh == 2048)
            SliceH = SLICE_2048_LINES;
        ime_setSliceHeight(SliceH);
        
        //set in/out stripe
        _ImeStripeInfo.uHN = _OutInfo.Ime_Hstripe.n;
        _ImeStripeInfo.uHL = _OutInfo.Ime_Hstripe.l;
        _ImeStripeInfo.uHM = _OutInfo.Ime_Hstripe.m;
        _ImeStripeInfo.uVN = _OutInfo.Ime_Vstripe.n;
        _ImeStripeInfo.uVL = _OutInfo.Ime_Vstripe.l;
        _ImeStripeInfo.uVM = _OutInfo.Ime_Vstripe.m;
        _ImeStripeInfo.StripeCalMode = IME_STRIPE_USER_MODE;
        ime_setUserStripeInfo(&_ImeStripeInfo);
    }
}

//////////////////////////////////////////////////////////////////
void IQSim_IME_Open(void)
{
    IME_OPENOBJ IMEOpenObj;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    ime_open(&IMEOpenObj);
}    
void IQSim_IME_Start(IME_MODE_PRAM *imeParam, int IQSimMode, _IME_MODE_SEL mode)
{
    //performing mode
    imeParam->op_mode = mode;

    if(mode == IPE2IME_SLICE_MODE)
    {
        //interrupt
        imeParam->interrupt_en = IME_INTE_FRM|IME_INTE_SLICE_DN1;
        imeParam->breakPoint1 = 0;
        imeParam->breakPoint2 = 0;
        //input source
        imeParam->in_src = IMEALG_IPE2IME;
        //output path selection, YCC or only Y pathes
        imeParam->path_num = IMEALG_1PATH;
    debug_ime(("IME in w,h,fmt=%d,%d,%d\r\n", 
        imeParam->in_size.h_size,
        imeParam->in_size.v_size,
        imeParam->format.in_format
        ));
    debug_ime(("IME in lineoff[3]=%d,%d,%d\r\n", 
        imeParam->line_ofs.in_lineofs.LineOffsetY,
        imeParam->line_ofs.in_lineofs.LineOffsetCb,
        imeParam->line_ofs.in_lineofs.LineOffsetCr
        ));
    debug_ime(("IME out w,h,fmt=%d,%d,%d\r\n", 
        imeParam->p1_out_size.h_size,
        imeParam->p1_out_size.v_size,
        imeParam->format.out1_format
        ));
    debug_ime(("IME out linoff[3]=%d,%d,%d\r\n", 
        imeParam->line_ofs.p1_out_lineofs.LineOffsetY,
        imeParam->line_ofs.p1_out_lineofs.LineOffsetCb,
        imeParam->line_ofs.p1_out_lineofs.LineOffsetCr
        ));
    debug_ime(("IME out buf num=%d\r\n", 
        imeParam->p1_out_addr.oPPBNums
        ));
    debug_ime(("IME out addr[0]=%08x,%08x,%08x\r\n", 
            imeParam->p1_out_addr.oDMA0.y_addr,
            imeParam->p1_out_addr.oDMA0.cb_addr,
            imeParam->p1_out_addr.oDMA0.cr_addr
            ));
    debug_ime(("IME out addr[1]=%08x,%08x,%08x\r\n", 
            imeParam->p1_out_addr.oDMA1.y_addr,
            imeParam->p1_out_addr.oDMA1.cb_addr,
            imeParam->p1_out_addr.oDMA1.cr_addr
            ));
    debug_ime(("IME out addr[2]=%08x,%08x,%08x\r\n", 
            imeParam->p1_out_addr.oDMA2.y_addr,
            imeParam->p1_out_addr.oDMA2.cb_addr,
            imeParam->p1_out_addr.oDMA2.cr_addr
            ));
    debug_ime(("IME out addr[3]=%08x,%08x,%08x\r\n", 
            imeParam->p1_out_addr.oDMA3.y_addr,
            imeParam->p1_out_addr.oDMA3.cb_addr,
            imeParam->p1_out_addr.oDMA3.cr_addr
            ));
    debug_ime(("IME out addr[4]=%08x,%08x,%08x\r\n", 
            imeParam->p1_out_addr.oDMA4.y_addr,
            imeParam->p1_out_addr.oDMA4.cb_addr,
            imeParam->p1_out_addr.oDMA4.cr_addr
            ));
    debug_ime(("IME start\r\n"));
        ime_setMode(imeParam);
        ime_triggerSlice();
        ime_setStart();
    }        
}

UINT32 IQSim_IME_Wait(void)
{
    UINT32 uiImeStatus;
    debug_ime(("IME wait\r\n"));
    while(1)
    {
        uiImeStatus = ime_waitDone();
        if (uiImeStatus & (IME_INT_FRM|IME_INT_SLICE_DN1))
        {
            break;
        }
    }
    debug_ime(("IME finish\r\n"));
    return uiImeStatus;
}
void IQSim_IME_Next(void)
{
    debug_ime(("IME next\r\n"));
    //ime slice start
    ime_triggerSlice();
}
void IQSim_IME_Stop(void)
{
    ime_pause();
    ipe_pause();
    ipe_reset(ON);
    ipe_reset(OFF);
    ime_setReset();
}
void IQSim_IME_Close(void)
{
    ime_close();
}    
//////////////////////////////////////////////////////////////////

IQ_BUF gBuf_IME[5] = {0};

void IQSim_IME_NewBuffer(UINT32 w, UINT32 h)
{
    //if(imeParam->op_mode == IPE2IME_SLICE_MODE)
    {
        UINT32 buf_n = 0;
        UINT32 buf_w = 0;
        UINT32 buf_h = 0;
        UINT32 buf_lineoff = 0;
        IQ_BUF_FMT buf_fmt = 0;
        IQ_BUF_FMT fmt_lut[5] = {IQ_FMT_YCC444, IQ_FMT_YCC422, IQ_FMT_YCC422, IQ_FMT_YCC420, IQ_FMT_YCC420};
        int i = 0;
        //output buffer number (slice)
        buf_n = Cap_SliceMode_GetSliceCount()+1;
        //output height (slice)
        buf_h = Cap_SliceMode_GetSliceHeight();
        //output width (slice)
        buf_w = w;
        //output format (slice)
        buf_fmt = fmt_lut[IME_OUTPUT_YCC_422_COS];
        //output lineoff (must align to 8 or 16 for JPG input)
        buf_lineoff = ((buf_w + 15)>>4)<<4; // 16 align
        //alloc buffer (slice)
#ifdef SHIFT_IFE_YCC
        for(i=0; i<buf_n; i++)
            IQ_BUF_New(&gBuf_IME[i], buf_fmt, buf_w, buf_h, buf_lineoff);
#else        
        for(i=0; i<buf_n; i++)
            IQ_BUF_New(&gBuf_IME[i], buf_fmt, buf_w, buf_h, buf_w + IFE_X_OVERLAP*2);
#endif        
    }
}

void IQSim_IME_DumpSetting(void)
{
    memcpy(&dumpBin.Ime, (void*)IQSim_IME_ADDR, sizeof(NT96220_IME_REG_STRUCT));
}


