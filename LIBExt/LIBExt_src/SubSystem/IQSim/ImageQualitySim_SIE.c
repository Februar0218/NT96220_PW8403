#include "ImageQualitySim_int.h"
#include "top.h"
#include "pll.h"

#if (SIE_DEBUG_FLOW == ENABLE)
#define debug_sie   debug_err
#else
#define debug_sie   debug_ind
#endif

UINT32 gRAW_cfa = 0;
UINT32 gRAW_bpp = 0;

void IQSim_RAW_SetBPP(int bpp)
{
    gRAW_bpp = bpp;
}

void IQSim_RAW_SetCFA(int cfa)
{
    gRAW_cfa = cfa;
}

void IQSim_SIE_SetIQ(STR_SIE_REG *SieParam, int IQSimMode)
{
///////////////////////////////////////////////////////////////////////////////
//  IQ Sim Parameter - Color Gain & Color Offset
    //enable function: color gain
    //if(pIQSim_SIE->SIE_Register_1.Bit.CG_EN)
        SieParam->SieFuncEn |= SIE_CG_EN;
    if (SieParam->SieFuncEn & SIE_CG_EN)
    {
        SieParam->SieCGParam = SIE_ColorGain[IQSim_DEF]; ///< color gain
        SieParam->SieCofsParam = SIE_ColorOffset[IQSim_DEF]; ///< color offset
    }
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  IQ Sim Parameter - Gamma
    //enable function: gamma
    //if(pIQSim_SIE->SIE_Register_1.Bit.GAMMA_EN)
        SieParam->SieFuncEn |= SIE_GAMMA_EN;
    if (SieParam->SieFuncEn & SIE_GAMMA_EN)
    {
        sra_setGammaTable(SRA_READBY_SIE, _SRA_SIE_GammaLUT[IQSim_DEF]);
    }
///////////////////////////////////////////////////////////////////////////////

    //enable function: horizontal flip
    SieParam->SieFuncEn &= ~SIE_HFLIP_EN;
    //enable function: vertical flip
    SieParam->SieFuncEn &= ~SIE_VFLIP_EN;
}

void IQSim_SIE_SetIQ_2(STR_SIE_REG *SieParam, int IQSimMode)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_SIE_REG_STRUCT* pSie = &(pBin->Sie);
    NT96220_SRA_REG_STRUCT* pSra = &(pBin->SieGamma);

///////////////////////////////////////////////////////////////////////////////
//  IQ Sim Parameter - Color Gain & Color Offset
    //enable function: color gain
    if(pSie->SIE_Register_1.Bit.CG_EN)
        SieParam->SieFuncEn |= SIE_CG_EN;
    if (SieParam->SieFuncEn & SIE_CG_EN)
    {
        SIE_ColorGain[IQSim_PC].Gain0 = pSie->SIE_Register_33.Bit.GAIN0; ///< color gain of even line, even pixel
        SIE_ColorGain[IQSim_PC].Gain1 = pSie->SIE_Register_33.Bit.GAIN1; ///< color offset of even line, odd pixel
        SIE_ColorGain[IQSim_PC].Gain2 = pSie->SIE_Register_34.Bit.GAIN2; ///< color offset of odd line, even pixel
        SIE_ColorGain[IQSim_PC].Gain3 = pSie->SIE_Register_34.Bit.GAIN3; ///< color offset of odd line, odd pixel
        SIE_ColorGain[IQSim_PC].GainSel = pSie->SIE_Register_34.Bit.GAIN_SEL; ///< gain selection
        SIE_ColorOffset[IQSim_PC].Ofs0 = pSie->SIE_Register_29.Bit.CG_OFS0; ///< color offset of even line, even pixel
        SIE_ColorOffset[IQSim_PC].Ofs1 = pSie->SIE_Register_29.Bit.CG_OFS1; ///< color offset of even line, odd pixel
        SIE_ColorOffset[IQSim_PC].Ofs2 = pSie->SIE_Register_30.Bit.CG_OFS2; ///< color offset of odd line, even pixel
        SIE_ColorOffset[IQSim_PC].Ofs3 = pSie->SIE_Register_30.Bit.CG_OFS3; ///< color offset of odd line, odd pixel
        SieParam->SieCGParam = SIE_ColorGain[IQSim_PC]; ///< color gain
        SieParam->SieCofsParam = SIE_ColorOffset[IQSim_PC]; ///< color offset
    }
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  IQ Sim Parameter - Gamma
    //enable function: gamma
    if(pSie->SIE_Register_1.Bit.GAMMA_EN)
        SieParam->SieFuncEn |= SIE_GAMMA_EN;
    if (SieParam->SieFuncEn & SIE_GAMMA_EN)
    {
        {
            int i;
            UINT8* pSrc = (UINT8*)&(pSra->SRA_Register_4.Word);
            for(i=0;i<=64;i++)
                _SRA_SIE_GammaLUT[IQSim_PC][i] = pSrc[i];
        }
        sra_setGammaTable(SRA_READBY_SIE, _SRA_SIE_GammaLUT[IQSim_PC]);
    }
///////////////////////////////////////////////////////////////////////////////
/*
    //enable function: horizontal flip
    if(pSie->SIE_Register_1.Bit.HFLIP_EN)
        SieParam->SieFuncEn |= SIE_HFLIP_EN;
    else
        SieParam->SieFuncEn &= ~SIE_HFLIP_EN;

    //enable function: vertical flip
    if(pSie->SIE_Register_1.Bit.VFLIP_EN)
        SieParam->SieFuncEn |= SIE_VFLIP_EN;
    else
        SieParam->SieFuncEn &= ~SIE_VFLIP_EN;
*/
    //enable function: horizontal flip
    SieParam->SieFuncEn &= ~SIE_HFLIP_EN;
    //enable function: vertical flip
    SieParam->SieFuncEn &= ~SIE_VFLIP_EN;

}

void IQSim_SIE_DumpIQ(void)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_SIE_REG_STRUCT* pSie = &(pBin->Sie);
    NT96220_SRA_REG_STRUCT* pSra = &(pBin->SieGamma);

    sprintf(SLOG_buf, "//SIE parameters-------------------------------------------------------\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) STR_SIE_CG_INFOR IQ_SIE_ColorGain=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  Gain0, Gain1, Gain2, Gain3, GainSel\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d,%d\r\n",
        pSie->SIE_Register_33.Bit.GAIN0, ///< color gain of even line, even pixel
        pSie->SIE_Register_33.Bit.GAIN1, ///< color offset of even line, odd pixel
        pSie->SIE_Register_34.Bit.GAIN2, ///< color offset of odd line, even pixel
        pSie->SIE_Register_34.Bit.GAIN3, ///< color offset of odd line, odd pixel
        pSie->SIE_Register_34.Bit.GAIN_SEL ///< gain selection
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) STR_SIE_COFS_INFOR IQ_SIE_ColorOffset=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  Ofs0, Ofs1, Ofs2, Ofs3\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        pSie->SIE_Register_29.Bit.CG_OFS0, ///< color offset of even line, even pixel
        pSie->SIE_Register_29.Bit.CG_OFS1, ///< color offset of even line, odd pixel
        pSie->SIE_Register_30.Bit.CG_OFS2, ///< color offset of odd line, even pixel
        pSie->SIE_Register_30.Bit.CG_OFS3 ///< color offset of odd line, odd pixel
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) UINT32 IQ_SRA_SIE_GammaLUT[65]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  65 gamma values\r\n");SLOG_OUT;
    {
        int i,j;
        UINT8* pSrc = (UINT8*)&(pSra->SRA_Register_4.Word);
        for(j=0;j<8;j++)
        {
        i=j*8;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,\r\n",
            pSrc[i+0],pSrc[i+1],pSrc[i+2],pSrc[i+3],pSrc[i+4],pSrc[i+5],pSrc[i+6],pSrc[i+7]
            );SLOG_OUT;
        }
        j=8;
        i=j*8;
        sprintf(SLOG_buf, "    %d\r\n",
            pSrc[i+0]
            );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
}

void IQSim_SIE_SetSrcModifier(STR_SIE_REG *SieParam, int IQSimMode)
{
#if 0
    //enable function:  auto OB
    SieParam->SieAutoOBEn = FALSE;
    //OB window parameters
    SieParam->SieOBParam.X0 = 0; ///< left end of the window
    SieParam->SieOBParam.Y0 = 0; ///< top of the window
    SieParam->SieOBParam.X1 = 0; ///< right end of the window
    SieParam->SieOBParam.Y1 = 0; ///< bottom of the window
    SieParam->SieOBParam.Thres = 0; ///< calculation threshld. if pixel value < threshld, it will be taken into account.
    SieParam->SieOBParam.SubSample = 0; ///< horizontal subsample rate: 1/(1+subsample).
/*
    SetIPPSizeInfor(_OBWinX0, SensorStatus.pSyncInfo->OB_X0);
    SetIPPSizeInfor(_OBWinX1, SensorStatus.pSyncInfo->OB_X1);
    SetIPPSizeInfor(_OBWinY0, SensorStatus.pSyncInfo->OB_Y0);
    SetIPPSizeInfor(_OBWinY1, SensorStatus.pSyncInfo->OB_Y1);
    SetIPPSizeInfor(_OBSubSample, SensorStatus.pSyncInfo->OB_SubSample);
    SetIPPSizeInfor(_OBThreshold, SensorStatus.pSyncInfo->OB_Thres);
*/
    //OB offset
    SieParam->SieOBOfs = 0;
/*
    SetIPPSizeInfor(_OBGain, SensorStatus.pSyncInfo->OB_Gain);
    SetIPPSizeInfor(_OBOffset, SensorStatus.pSyncInfo->OB_Gain);
*/

    //ch decode
    SieParam->SieCHDecodeParam.TotalCh = 0; ///< total number of fields. 0: 1 field; 1: 2 fields, ..., 15: 16 fields.
    SieParam->SieCHDecodeParam.Ch0 = 0; ///< field num of the 1st input channel
    SieParam->SieCHDecodeParam.Ch1 = 0; ///< field num of the 2nd input channel
    SieParam->SieCHDecodeParam.Ch2 = 0; ///< field num of the 3rd input channel
    SieParam->SieCHDecodeParam.Ch3 = 0; ///< field num of the 4th input channel
    SieParam->SieCHDecodeParam.Ch4 = 0; ///< field num of the 5th input channel
    SieParam->SieCHDecodeParam.Ch5 = 0; ///< field num of the 6th input channel
    SieParam->SieChOutLofs = 0;

    //enable function: vertical defect line compensation
    SieParam->SieFuncEn &= ~SIE_LINE_BLM_EN;
    {
        SieParam->SieDefNfact = DEFECT_NFACT_0;
        SieParam->SieDefColumn = NULL;
    }
    //enable function: OB average calculation
    SieParam->SieFuncEn &= ~SIE_OB_AVER_EN;
    //enable function: using the calculated OB average
    SieParam->SieFuncEn &= ~SIE_OB_EN;
    //enable function: defect pixel compensation
    SieParam->SieFuncEn &= ~SIE_BLM_EN;
    {
        SieParam->SieInAddr0 = NULL;
    }
    //enable function: ECS
    SieParam->SieFuncEn &= ~SIE_ECS_EN;
    {
        SieParam->SieECSParam.MapSizeSel= ECSMAP65x65;
        SieParam->SieECSParam.MapDthrEn = ENABLE;
        SieParam->SieECSParam.MapDthrRst = ENABLE;
        SieParam->SieECSParam.MapDthrLevel = 0;
        SieParam->SieECSParam.MapShift = 0;
        SieParam->SieECSParam.CfaPat = pIQSim_basic->BASIC_IO_Register_4.IN_CFA;
        SieParam->SieInAddr1 = 0;
    }
    //enable function: dark frame subtraction
    SieParam->SieFuncEn &= ~SIE_DF_SUB_EN;
    {
        SieParam->SieDFSParam.DfsOp = DFS_A_MINUS_B;
        SieParam->SieDFSParam.DfsThSrc = DFS_TH_DRAM;
        SieParam->SieDFSParam.DfsTh = 0;
        SieParam->SieDFSParam.DfsShiftNum = 0;
        SieParam->SieInAddr0 = 0;
        SieParam->SieInLofs = 0;
        /*
        if(SieParam->SieInPack == SIE_IN_PACK_8BIT)
        {
            SieParam->SieInLofs = SieParam->SieOutSize.Hsize;
        }
        else
        {
            SieParam->SieInLofs = ((SieParam->SieOutSize.Hsize*10) + (8-1))/8;
            SieParam->SieInLofs = ((SieParam->SieInLofs + (4-1)) >> 2) << 2;
        }
        */
    }
    //enable function: dark frame subtraction with DP embed
    SieParam->SieFuncEn &= ~SIE_DF_EMBED_EN;
    {
        //SieParam->SieOutSize.Hsize = ((SieParam->SieCropParam.Hsize) >> 2) << 2;
        SieParam->SieOutSize.Hsize = 0;
    }
#endif
}

void IQSim_SIE_SetSrc(STR_SIE_REG *SieParam, int IQSimMode)
{
    //in/out mode sel
    SieParam->SieDRAMInEn = TRUE;

#if 0
    //sync and total periods of VD and HD
    SieParam->SieHdVdSync.HD_sync = 0; ///< HD sync period
    SieParam->SieHdVdSync.VD_sync = 0; ///< VD sync period
    SieParam->SieHdVdSync.HD_total = 0; ///< HD total period
    SieParam->SieHdVdSync.VD_total = 0; ///< VD total period
/*
SiePreInfo->VDTotal = GetIPPSizeInfor(_VDTotal);
SiePreInfo->VDSync = GetIPPSizeInfor(_VDSync);
SiePreInfo->HDTotal = GetIPPSizeInfor(_HDTotal);
SiePreInfo->HDSync = GetIPPSizeInfor(_HDSync);
*/
    SieParam->SieHdVdSync.HD_inv = Invert;
    SieParam->SieHdVdSync.VD_inv = Invert;

    //sync phase of VD/HD/Data, and master or slave mode.
    SieParam->SieSyncPhase.Master= SlaveMode;
    SieParam->SieSyncPhase.Data_Phase = 0;
    SieParam->SieSyncPhase.HD_Phase = 1;
    SieParam->SieSyncPhase.VD_Phase = 1;
    //horizontal and vertical active start points
    SieParam->SieActParam.Hstart = 0; ///< horizontal start
    SieParam->SieActParam.Vstart = 0; ///< vertical start
    SieParam->SieActParam.Hsize = 0; ///< horizontal size
    SieParam->SieActParam.Vsize = 0; ///< vertical size
/*
SiePreInfo->HDActX = GetIPPSizeInfor(_HDActX);
SiePreInfo->HDActY = GetIPPSizeInfor(_HDActY);
*/
    /*
    Sie_PreHsize = SieParam->SieHdVdSync.HD_total - SieParam->SieActParam.Hstart;
    Sie_PreHsize = (Sie_PreHsize / 4) * 4;
    Sie_PreVsize = SieParam->SieHdVdSync.VD_total - SieParam->SieActParam.Vstart - LAST_ONE_LINE_OMITT;
    SieParam->SieActParam.Hsize = Sie_PreHsize; ///< horizontal size
    SieParam->SieActParam.Vsize = Sie_PreVsize; ///< vertical size
    */
/*
SiePreInfo->RawCenterX = GetIPPSizeInfor(_SieXcenter);
SiePreInfo->RawCenterY = GetIPPSizeInfor(_SieYcenter);
*/
#endif

///////////////////////////////////////////////////////////////////////////////
// IQ Sim Parameter - Input
    SieParam->SieSyncPhase.Master = MasterMode;
    //input packing mode
    if(gRAW_bpp == 8)
        SieParam->SieInPack = SIE_IN_PACK_8BIT;
    if(gRAW_bpp == 10)
        SieParam->SieInPack = SIE_IN_PACK_10BIT;
    //input size (only worked under dram in)
    SieParam->SieInAddr0 = gBuf_RAW.addr[0]; //DPC or RAW data input
    SieParam->SieInAddr1 = 0; //ECS or DFS data input
    //crop location
    SieParam->SieCropParam.Hstart = 0;
    SieParam->SieCropParam.Vstart = 0;
    //crop size
    SieParam->SieCropParam.Hsize = gBuf_RAW.w;
    SieParam->SieCropParam.Vsize = gBuf_RAW.h;
    SieParam->SieInLofs = gBuf_RAW.lineoff;

///////////////////////////////////////////////////////////////////////////////

#if 0
    //pattern gen
    SieParam->SiePatParam.PatMode = PAT_DISABLE;
    SieParam->SiePatParam.PatValue = 0;

    /*
    if(SieParam->SieHBinParam.PixAdd != 0)
    {
        if(SieParam->SieCropParam.Hsize <= SieParam->SieOutSize.Hsize)
        {
            debug_err(("sie HSCL_EN is disable, force Bining func turning to off\r\n"));
            SieParam->SieHBinParam.PixAdd = 0;
        }
    }
    */
    //enable function: sensor data in
    SieParam->SieFuncEn &= ~SIE_CROP_EN;
#endif
}

void IQSim_SIE_SetDest(STR_SIE_REG *SieParam, int IQSimMode, UINT32 times_RAW)
{
///////////////////////////////////////////////////////////////////////////////
// IQ Sim Parameter - Output
    //output src
    SieParam->SieOutSrc = PATH_POST_GAMMA;
    //output packing mode
    SieParam->SieOutPack = SIE_OUT_PACK_8BIT;
    //horizontal and vertical sizes of output/active window
    //SieParam->SieOutSize.Hsize = gBuf_SIE.w;
    SieParam->SieOutSize.Hsize = ((SieParam->SieCropParam.Hsize) >> 2) << 2;
    SieParam->SieOutSize.lineofs = gBuf_SIE.lineoff;
/*
SiePreInfo->SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);
*/
    //output ping-pong buffers
#ifdef SHIFT_IFE_RAW
    if(times_RAW)
    {
        SieParam->SieOutAddr.OutAddr0 = gBuf_SIE.addr[0] + IFE_X_OVERLAP*times_RAW; //for IFE raw overlap, outaddr must shift right 1 word
        //debug_err(("IFE raw src=%d\r\n", times_RAW));
    }
    else
    {
        SieParam->SieOutAddr.OutAddr0 = gBuf_SIE.addr[0] + IFE_X_OVERLAP; //for IFE raw overlap, outaddr must shift right 1 word
    }        
#else
    SieParam->SieOutAddr.OutAddr0 = gBuf_SIE.addr[0];
#endif
    SieParam->SieOutAddr.OutAddr1 = 0;
    SieParam->SieOutAddr.OutAddr2 = 0;
    SieParam->SieOutAddr.OutBufNum = ONE_OUTPUT_BUF; //capture is using frame mode
///////////////////////////////////////////////////////////////////////////////

#if 0
    //slice h
    SieParam->SieSliceHeight = 0;
    //binning
    SieParam->SieHBinParam.PixAdd = 0;
    SieParam->SieHBinParam.PixDiv = 0;

    //enable function: ca out (CA is used by AE,AWB)
    SieParam->SieFuncEn &= ~SIE_SUBOUT_EN;
    {
        SieParam->SieSubParam.SubUpdateMode = FRAME_UPDATE;
        SieParam->SieSubParam.SubOutSrc = PATH_PRE_COLORGAIN;
        SieParam->SieSubParam.Hsub = 1;
        SieParam->SieSubParam.Vsub = 1;
        SieParam->SieSubParam.Hcnt = 1;
        SieParam->SieSubParam.Vcnt = 1;
        SieParam->SieSubAddr = 0;
        SieParam->SieSubOutLofs = SieParam->SieSubParam.Hcnt;
        SieParam->SieIntEn &= ~SIE_INTE_SUB;
    }
#endif
}

void IQSim_SIE_SetDestModifier(STR_SIE_REG *SieParam, int IQSimMode)
{
#if 0
    //enable function: horizontal downsampling
    SieParam->SieFuncEn &= ~SIE_HSCL_EN;
    //enable function: horizontal padding
    SieParam->SieFuncEn &= ~SIE_PADDING_EN;
    //enable function: multi-channel output decode
    SieParam->SieFuncEn &= ~SIE_CHOUT_EN;
#endif
}

void IQSim_SIE_Open(void)
{
#define SYSINIT_SIE_MCLKDIV     4
#define SYSINIT_SIE_PCLKDIV     0

    SIE_OPENOBJ SIEOpenObj;

    //////////////////////////////////////////////////////
    // for SIE D2D path, we need to use master mode
    //////////////////////////////////////////////////////
#if (SIE_DEBUG_FLOW == ENABLE)
    //calculate SIE pxclk
    UINT32 r_PLL1_CLK = pll_getPLL1Freq()/1000000;
    UINT32 r_SIE_MCLK = r_PLL1_CLK/(SYSINIT_SIE_MCLKDIV+1);
    UINT32 r_SIE_PXCLK = r_SIE_MCLK/(SYSINIT_SIE_PCLKDIV+1);
    debug_msg("PLL1_CLK=%d\r\n", r_PLL1_CLK);
    debug_msg("SIE_MCLK=%d\r\n", r_SIE_MCLK);
    debug_msg("SIE_PXCLK=%d\r\n", r_SIE_PXCLK);
#endif
    //removed, all pimux-select are ctrled by proj layer
    debug_err(("^R pimmux api removed\r\n"));
    //pinmux_select_mclk(PINMUX_MCLK);
    //SIE-MCLK clock generate by PLL1
    //SIE-MCLK clock range must not exceed 96 MHz (for SIE limit)
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_PLL1); //PLL1 clk default output = 12 * 40 = 480 MHz
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(SYSINIT_SIE_MCLKDIV)); //SIE_MCLK = SIE_MCLKSRC/(SIE_MCLKDIV+1) = 480/(4+1) = 96 MHz
    //SIE_MCLK = 480/(4+1) = 96 MHz
    //SIE-PXCLK clock generate by SIE-MCLK
    pll_setClockRate(PLL_CLKSEL_SIE_PXCLKSRC, PLL_CLKSEL_SIE_PXCLKSRC_SIEMCLK);
    pll_setClockRate(PLL_CLKSEL_SIE_PXCLKDIV, PLL_SIE_PXCLKDIV(SYSINIT_SIE_PCLKDIV)); //SIE_CLK = SIE_PXCLKSRC /(SIE_PXCLKDIV + 1) = 96 /(0+1) = 96 MHz
    //remove these 2 lines: already enabled by sie_attach();
    //pll_enableClock(SIE_MCLK);
    //pll_enableClock(SIE_PXCLK);

    SIEOpenObj.FP_SIEISR_CB = NULL;
    sie_attach();
    sie_open(&SIEOpenObj);
}

UINT32 IQSim_SIE_Wait(void)
{
    debug_sie(("SIE wait\r\n"));
    //sie_waitVD(1);
    //sie_Dram2DramWaitEnd();
    while(!((*(UINT32*)0x9a000014) & 0x00002000)) //wait for SIE_INT_DRAMO_END
        TimerDelayUs(100);
    sie_pause();
    debug_sie(("SIE finsih\r\n"));
    return 1;
}
void IQSim_SIE_Stop(void)
{
    sie_pause();
}
void IQSim_SIE_Close(void)
{
    sie_close();
    sie_detach();
}

ER sie_Dram2DramStart(void);

void IQSim_SIE_Start(STR_SIE_REG *SieParam, int IQSimMode, SIE_OutputModeEnum mode)
{
    //operation
    SieParam->SieOperation = SIE_ENGINE_SETMODE;

    //in/out mode sel
    SieParam->SieOutMode = mode;

    //enable int
    //SieParam->SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2;
    SieParam->SieIntEn = SIE_INTE_FLDEND;
    //SieParam->SieIntBP1 = 10; // PRV_SIE_BP1
    SieParam->SieIntBP1 = 0;
    //SieParam->SieIntBP2 = SensorStatus.pSyncInfo->Act_CenterY;
    SieParam->SieIntBP2 = 0;
    debug_sie(("SIE in w,h,fmt=%d,%d,%d\r\n",
        SieParam->SieCropParam.Hsize,
        SieParam->SieCropParam.Vsize,
        SieParam->SieInPack
        ));
    debug_sie(("SIE in lineoff=%d\r\n",
        SieParam->SieInLofs
        ));
    debug_sie(("SIE in addr=%08x\r\n",
        SieParam->SieInAddr0
        ));
    debug_sie(("SIE out h,fmt=%d,%d\r\n",
        SieParam->SieOutSize.Hsize,
        SieParam->SieOutPack
        ));
    debug_sie(("SIE out lineoff=%d\r\n",
        SieParam->SieOutSize.lineofs
        ));
    debug_sie(("SIE out addr=%08x\r\n",
        SieParam->SieOutAddr.OutAddr0
        ));
    debug_sie(("SIE start\r\n"));
    sie_setMode(SieParam);
    sie_Dram2DramStart();
}

//////////////////////////////////////////////////////////////////
IQ_BUF gBuf_RAW = {0};
IQ_BUF gBuf_SIE = {0};

void IQSim_SIE_NewBuffer(UINT32 w, UINT32 h)
{
    UINT32 buf_n = 0;
    UINT32 buf_w = 0;
    UINT32 buf_h = 0;
    IQ_BUF_FMT buf_fmt = 0;
    //output buffer number
    buf_n = 1;
    //output height
    buf_h = h;
    //output width
    buf_w = w;
    //output format
    buf_fmt = IQ_FMT_RAW8;
    //alloc buffer
#ifdef SHIFT_IFE_RAW
    IQ_BUF_New(&gBuf_SIE, buf_fmt, buf_w, buf_h, buf_w + IFE_X_OVERLAP*IFE_RAW_TIMES_MAX); //for IFE raw overlap, lineoft add 1 word.
#else
    IQ_BUF_New(&gBuf_SIE, buf_fmt, buf_w, buf_h, 0);
#endif
}


void IQSim_SIE_DumpSetting(void)
{
    memcpy(&dumpBin.Sie, (void*)IQSim_SIE_ADDR, sizeof(NT96220_SIE_REG_STRUCT));
    memcpy(&dumpBin.SieGamma, (void*)(IQSim_SRA_ADDR), sizeof(NT96220_SRA_REG_STRUCT));
}

