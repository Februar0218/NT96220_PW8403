#include "ImageQualitySim_int.h"

#if (IPE_DEBUG_FLOW == ENABLE)
#define debug_ipe   debug_err
#else
#define debug_ipe   debug_ind
#endif

void IQSim_IPE_SetIQ(IPE_MODEINFO *ipeParam, UINT32 IQSimMode)
{
    IPE_IQINFO* iqInfo = &(ipeParam->IQINFO);
    
    ///< IPE function set/enable/disable selection, combined usage with FuncUpdateSel
    iqInfo->FuncSel =   
        IPE_NR_EN |
        IPE_RLPF_EN |
        IPE_GLPF_EN |
        IPE_BLPF_EN |
        IPE_CGAIN_EN |
        IPE_CCR_EN |
        IPE_CTRL_EN |
        IPE_HADJ_EN |
        IPE_CADJ_EN |
        IPE_CADJ_YENH_EN |
        IPE_CADJ_YCON_EN |
        IPE_CADJ_CCON_EN |
        IPE_CADJ_YLPF_EN |
        IPE_CADJ_CLPF_EN |
        IPE_YCTH_EN;

    ///< IPE function set/enable/disable selection
    iqInfo->FuncUpdateSel = 
        IPE_FUNC_SET;

    ///< IPE parameters update selection, combined usage with definition IPE_SET_XXX
    iqInfo->ParamUpdateSel = 
        IPE_SET_NR |
        IPE_SET_EDGE |
        IPE_SET_COLOR |
        IPE_SET_COLOR_GAIN |
        IPE_SET_COLOR_OFS |
        IPE_SET_COLOREFFECT |
        IPE_SET_INVERSEEFFECT |
        IPE_SET_YCFIXEFFECT |
        IPE_SET_EDGEEFFECT |
        IPE_SET_COLOR_EXT;

    ///< NR related parameters
    iqInfo->NR_PARAM = &_IPE_NRParam[IQSim_DEF];
    ///< Edge related parameters
    iqInfo->EDGE_PARAM = &_IPE_EdgeParam[IQSim_DEF];
    ///< Edge extraction kernel setting
    iqInfo->EdgeKerDiv = _IPE_EdgeKernelDiv[IQSim_DEF];
    ///< Estab table defines edge weighting for different luminance
    iqInfo->Estab = _IPE_emStab[IQSim_DEF];
    ///< Color gain control
    iqInfo->COLOR_PARAM.ColorGain = _IPE_ColorGain[IQSim_DEF];
    iqInfo->COLOR_PARAM.ColorOfs = _IPE_ColorOffset[IQSim_DEF];
    ///< Color correction control
    iqInfo->COLOR_PARAM.ColorCorrect = _IPE_ColorMatrixParam[IQSim_DEF];
    ///< Color suppress control
    iqInfo->COLOR_PARAM.ColorSupWeight = 22;
    ///< FStab table defines color suppress weighting for different luminance
    iqInfo->FStab = _IPE_fmStab[IQSim_DEF];
    ///< FDtab table defines color suppress weighting for different color saturation
    iqInfo->FDtab = _IPE_fmDtab[IQSim_DEF];
    ///< Color effect related parameters
    iqInfo->COLOR_EFFECT_PARAM = &_IPE_ColorEffectParam[IQSim_DEF];
    ///< color control hue adjust table
    iqInfo->HueTab = _IPE_hueTab[IQSim_DEF];
    ///< color control sat adjust table
    iqInfo->SatTab = _IPE_satTab[IQSim_DEF];
    ///< color control int adjust table
    iqInfo->IntTab = _IPE_intTab[IQSim_DEF];
    ///< color control weighting table for SatTab, IntTab, and EdgTab
    iqInfo->DDSTab = _IPE_ddsTab[IQSim_DEF];
    ///< Inverse effect related parameters
    iqInfo->INVERSE_EFFECT_PARAM = &_IPE_InvertEffectParam[IQSim_DEF];
    ///< Fix YC substitution effect related parameters
    iqInfo->YCFIX_EFFECT_PARAM = &_IPE_YCFixEffectParam[IQSim_DEF];
    ///< Edge effect related parameters
    iqInfo->EDGE_EFFECT_PARAM = &_IPE_EdgeEffectParam[IQSim_DEF];
    ///< Extension Color parameters
    iqInfo->COLOR_PARAM_EXT = &_IPE_ColorParamExt[IQSim_DEF];
    
    ///< VA related parameters
    //iqInfo->VA_PARAM =;               
}

void IQSim_IPE_SetIQ_2(IPE_MODEINFO *ipeParam, UINT32 IQSimMode)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_IPE_REG_STRUCT* pIpe = &(pBin->Ipe);
    IPE_IQINFO* iqInfo = &(ipeParam->IQINFO);
    
    ///< IPE function set/enable/disable selection, combined usage with FuncUpdateSel
    iqInfo->FuncSel = 0;
    if(pIpe->IPE_Register_2.Bit.RLPF_EN)
        iqInfo->FuncSel |= IPE_RLPF_EN;
    if(pIpe->IPE_Register_2.Bit.GLPF_EN)
        iqInfo->FuncSel |= IPE_GLPF_EN;
    if(pIpe->IPE_Register_2.Bit.BLPF_EN)
        iqInfo->FuncSel |= IPE_BLPF_EN;
    if(pIpe->IPE_Register_2.Bit.CGAIN_EN)
        iqInfo->FuncSel |= IPE_CGAIN_EN;
    if(pIpe->IPE_Register_2.Bit.CR_EN)
        iqInfo->FuncSel |= IPE_CCR_EN;
    if(pIpe->IPE_Register_2.Bit.CTRL_EN)
        iqInfo->FuncSel |= IPE_CTRL_EN;
    if(pIpe->IPE_Register_2.Bit.HADJ_EN)
        iqInfo->FuncSel |= IPE_HADJ_EN;
    if(pIpe->IPE_Register_2.Bit.CADJ_EN)
        iqInfo->FuncSel |= IPE_CADJ_EN;
    if(pIpe->IPE_Register_2.Bit.CADJ_YENH_EN)
        iqInfo->FuncSel |= IPE_CADJ_YENH_EN;
    if(pIpe->IPE_Register_2.Bit.CADJ_YLPF_EN)
        iqInfo->FuncSel |= IPE_CADJ_YLPF_EN;
    if(pIpe->IPE_Register_2.Bit.CADJ_CLPF_EN)
        iqInfo->FuncSel |= IPE_CADJ_CLPF_EN;
    if(pIpe->IPE_Register_2.Bit.CADJ_YCON_EN)
        iqInfo->FuncSel |= IPE_CADJ_YCON_EN;
    if(pIpe->IPE_Register_2.Bit.CADJ_CCON_EN)
        iqInfo->FuncSel |= IPE_CADJ_CCON_EN;
    if(pIpe->IPE_Register_2.Bit.CADJ_YCTH_EN)
        iqInfo->FuncSel |= IPE_YCTH_EN;

    ///< IPE function set/enable/disable selection
    iqInfo->FuncUpdateSel = 
        IPE_FUNC_SET;

    ///< IPE parameters update selection, combined usage with definition IPE_SET_XXX
    iqInfo->ParamUpdateSel = 
        IPE_SET_NR |
        IPE_SET_EDGE |
        IPE_SET_COLOR |
        IPE_SET_COLOR_GAIN |
        IPE_SET_COLOR_OFS |
        IPE_SET_COLOREFFECT |
        IPE_SET_INVERSEEFFECT |
        IPE_SET_YCFIXEFFECT |
        IPE_SET_EDGEEFFECT |
        IPE_SET_COLOR_EXT;

    //ipe_setColorGainMask(pIpe->IPE_Register_31.Bit.CGAIN_MASK);
    //ipe_setColorCorrect(pIpe->IPE_Register_31.Bit.CGAIN_RANGE, iqInfo->COLOR_PARAM.ColorCorrect);
    //ipe_setCbCrLmfSel(pIpe->IPE_Register_60.Bit.CADJ_LMFSEL);
    
    ///< NR related parameters
    _IPE_NRParam[IQSim_PC].GBAL = pIpe->IPE_Register_20.Bit.GBAL;
    _IPE_NRParam[IQSim_PC].Nsmar = pIpe->IPE_Register_22.Bit.INTG_NSMAR;
    _IPE_NRParam[IQSim_PC].Lpfw = pIpe->IPE_Register_22.Bit.LPFW;
    _IPE_NRParam[IQSim_PC].Sumsel = pIpe->IPE_Register_22.Bit.SUM_SEL;
    iqInfo->NR_PARAM = &_IPE_NRParam[IQSim_PC];
    ///< Edge related parameters
    _IPE_EdgeParam[IQSim_PC].EdgEnhanceP = pIpe->IPE_Register_58.Bit.Y_ENH_P;
    _IPE_EdgeParam[IQSim_PC].EdgEnhanceN = pIpe->IPE_Register_58.Bit.Y_ENH_N;
    _IPE_EdgeParam[IQSim_PC].EmapA = pIpe->IPE_Register_75.Bit.EMAPA;
    _IPE_EdgeParam[IQSim_PC].EmapB = pIpe->IPE_Register_75.Bit.EMAPB;
    _IPE_EdgeParam[IQSim_PC].EmapC = pIpe->IPE_Register_75.Bit.EMAPC;
    _IPE_EdgeParam[IQSim_PC].EmapD = pIpe->IPE_Register_76.Bit.EMAPD;
    _IPE_EdgeParam[IQSim_PC].EmapN = pIpe->IPE_Register_76.Bit.EMAPN;
    _IPE_EdgeParam[IQSim_PC].EmapM = pIpe->IPE_Register_76.Bit.EMAPM;
    _IPE_EdgeParam[IQSim_PC].EmapE = pIpe->IPE_Register_76.Bit.EMAPE;
    iqInfo->EDGE_PARAM = &_IPE_EdgeParam[IQSim_PC];
    ///< Edge extraction kernel setting
    _IPE_EdgeKernelDiv[IQSim_PC][0] = pIpe->IPE_Register_74.Bit.EEXT5;
    _IPE_EdgeKernelDiv[IQSim_PC][1] = pIpe->IPE_Register_74.Bit.EEXT4;
    _IPE_EdgeKernelDiv[IQSim_PC][2] = pIpe->IPE_Register_73.Bit.EEXT2_V;
    _IPE_EdgeKernelDiv[IQSim_PC][3] = pIpe->IPE_Register_74.Bit.EEXT4;
    _IPE_EdgeKernelDiv[IQSim_PC][4] = pIpe->IPE_Register_74.Bit.EEXT3;
    _IPE_EdgeKernelDiv[IQSim_PC][5] = pIpe->IPE_Register_73.Bit.EEXT1_V;
    _IPE_EdgeKernelDiv[IQSim_PC][6] = pIpe->IPE_Register_73.Bit.EEXT2_H;
    _IPE_EdgeKernelDiv[IQSim_PC][7] = pIpe->IPE_Register_73.Bit.EEXT1_H;
    _IPE_EdgeKernelDiv[IQSim_PC][8] = pIpe->IPE_Register_73.Bit.EEXT0_H << 1;
    _IPE_EdgeKernelDiv[IQSim_PC][9] = pIpe->IPE_Register_74.Bit.EEXTDIV;
    iqInfo->EdgeKerDiv = _IPE_EdgeKernelDiv[IQSim_PC];
    ///< Estab table defines edge weighting for different luminance
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_103.Word);
        for(i=0;i<=24;i++)
            _IPE_emStab[IQSim_PC][i] = pSrc[i];
    }
    iqInfo->Estab = _IPE_emStab[IQSim_PC];
    ///< Color gain control
    _IPE_ColorGain[IQSim_PC].ChR = pIpe->IPE_Register_32.Bit.CGAIN0;
    _IPE_ColorGain[IQSim_PC].ChGr = pIpe->IPE_Register_32.Bit.CGAIN1;
    _IPE_ColorGain[IQSim_PC].ChGb = pIpe->IPE_Register_33.Bit.CGAIN2;
    _IPE_ColorGain[IQSim_PC].ChB = pIpe->IPE_Register_33.Bit.CGAIN3;
    iqInfo->COLOR_PARAM.ColorGain = _IPE_ColorGain[IQSim_PC];
    _IPE_ColorOffset[IQSim_PC].ChR = pIpe->IPE_Register_34.Bit.COFS0;
    _IPE_ColorOffset[IQSim_PC].ChGr = pIpe->IPE_Register_34.Bit.COFS1;
    _IPE_ColorOffset[IQSim_PC].ChGb = pIpe->IPE_Register_34.Bit.COFS2;
    _IPE_ColorOffset[IQSim_PC].ChB = pIpe->IPE_Register_34.Bit.COFS3;
    iqInfo->COLOR_PARAM.ColorOfs = _IPE_ColorOffset[IQSim_PC];
    ///< Color correction control
    _IPE_ColorMatrixParam[IQSim_PC][0] = pIpe->IPE_Register_23.Bit.COEF_RR;
    _IPE_ColorMatrixParam[IQSim_PC][1] = pIpe->IPE_Register_24.Bit.COEF_RG;
    _IPE_ColorMatrixParam[IQSim_PC][2] = pIpe->IPE_Register_24.Bit.COEF_RB;
    _IPE_ColorMatrixParam[IQSim_PC][3] = pIpe->IPE_Register_25.Bit.COEF_GR;
    _IPE_ColorMatrixParam[IQSim_PC][4] = pIpe->IPE_Register_25.Bit.COEF_GG;
    _IPE_ColorMatrixParam[IQSim_PC][5] = pIpe->IPE_Register_26.Bit.COEF_GB;
    _IPE_ColorMatrixParam[IQSim_PC][6] = pIpe->IPE_Register_26.Bit.COEF_BR;
    _IPE_ColorMatrixParam[IQSim_PC][7] = pIpe->IPE_Register_27.Bit.COEF_BG;
    _IPE_ColorMatrixParam[IQSim_PC][8] = pIpe->IPE_Register_27.Bit.COEF_BB;
    iqInfo->COLOR_PARAM.ColorCorrect = _IPE_ColorMatrixParam[IQSim_PC];
    ///< Color suppress control
    iqInfo->COLOR_PARAM.ColorSupWeight = pIpe->IPE_Register_23.Bit.CCR_SET;
    ///< FStab table defines color suppress weighting for different luminance
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_107.Word);
        for(i=0;i<=24;i++)
            _IPE_fmStab[IQSim_PC][i] = pSrc[i];
    }
    iqInfo->FStab = _IPE_fmStab[IQSim_PC];
    ///< FDtab table defines color suppress weighting for different color saturation
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_111.Word);
        for(i=0;i<=24;i++)
            _IPE_fmDtab[IQSim_PC][i] = pSrc[i];
    }
    iqInfo->FDtab = _IPE_fmDtab[IQSim_PC];
    ///< Color effect related parameters
    _IPE_ColorEffectParam[IQSim_PC].CbOfs = pIpe->IPE_Register_60.Bit.C_CBOFS;
    _IPE_ColorEffectParam[IQSim_PC].CrOfs = pIpe->IPE_Register_60.Bit.C_CROFS;
    _IPE_ColorEffectParam[IQSim_PC].YCon = pIpe->IPE_Register_59.Bit.Y_CON;
    _IPE_ColorEffectParam[IQSim_PC].CCon = pIpe->IPE_Register_60.Bit.C_CON;
    _IPE_ColorEffectParam[IQSim_PC].IntOfs = pIpe->IPE_Register_36.Bit.INT_OFS;
    _IPE_ColorEffectParam[IQSim_PC].SatOfs = pIpe->IPE_Register_36.Bit.SAT_OFS;
    iqInfo->COLOR_EFFECT_PARAM = &_IPE_ColorEffectParam[IQSim_PC];
    ///< color control hue adjust table
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_37.Word);
        for(i=0;i<=24;i++)
            _IPE_hueTab[IQSim_PC][i] = pSrc[i];
    }
    iqInfo->HueTab = _IPE_hueTab[IQSim_PC];
    ///< color control sat adjust table
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_49.Word);
        for(i=0;i<=24;i++)
            _IPE_satTab[IQSim_PC][i] = SIGN(pSrc[i],8);
    }
    iqInfo->SatTab = _IPE_satTab[IQSim_PC];
    ///< color control int adjust table
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_43.Word);
        for(i=0;i<=24;i++)
            _IPE_intTab[IQSim_PC][i] = SIGN(pSrc[i],8);
    }
    iqInfo->IntTab = _IPE_intTab[IQSim_PC];
    ///< color control weighting table for SatTab, IntTab, and EdgTab
    _IPE_ddsTab[IQSim_PC][0] = pIpe->IPE_Register_55.Bit.DDS0;
    _IPE_ddsTab[IQSim_PC][1] = pIpe->IPE_Register_55.Bit.DDS1;
    _IPE_ddsTab[IQSim_PC][2] = pIpe->IPE_Register_55.Bit.DDS2;
    _IPE_ddsTab[IQSim_PC][3] = pIpe->IPE_Register_55.Bit.DDS3;
    _IPE_ddsTab[IQSim_PC][4] = pIpe->IPE_Register_56.Bit.DDS4;
    _IPE_ddsTab[IQSim_PC][5] = pIpe->IPE_Register_56.Bit.DDS5;
    _IPE_ddsTab[IQSim_PC][6] = pIpe->IPE_Register_56.Bit.DDS6;
    _IPE_ddsTab[IQSim_PC][7] = pIpe->IPE_Register_56.Bit.DDS7;
    iqInfo->DDSTab = _IPE_ddsTab[IQSim_PC];
    ///< Inverse effect related parameters
    _IPE_InvertEffectParam[IQSim_PC].Inv = pIpe->IPE_Register_31.Bit.CGAIN_INV;
    _IPE_InvertEffectParam[IQSim_PC].Hinv = pIpe->IPE_Register_31.Bit.CGAIN_HINV;
    iqInfo->INVERSE_EFFECT_PARAM = &_IPE_InvertEffectParam[IQSim_PC];
    ///< Fix YC substitution effect related parameters
    _IPE_YCFixEffectParam[IQSim_PC].YFix = pIpe->IPE_Register_57.Bit.Y_FIX;
    _IPE_YCFixEffectParam[IQSim_PC].CbFix = pIpe->IPE_Register_57.Bit.CB_FIX;
    _IPE_YCFixEffectParam[IQSim_PC].CrFix = pIpe->IPE_Register_57.Bit.CR_FIX;
    _IPE_YCFixEffectParam[IQSim_PC].YFixETh = pIpe->IPE_Register_58.Bit.Y_ETHY;
    _IPE_YCFixEffectParam[IQSim_PC].CFixETh = pIpe->IPE_Register_59.Bit.Y_ETHC;
    _IPE_YCFixEffectParam[IQSim_PC].CFixYTh.th_high = pIpe->IPE_Register_77.Bit.CADJ_YTH_H;
    _IPE_YCFixEffectParam[IQSim_PC].CFixYTh.th_low = pIpe->IPE_Register_77.Bit.CADJ_YTH_L;
    _IPE_YCFixEffectParam[IQSim_PC].CFixCbTh.th_high = pIpe->IPE_Register_78.Bit.CADJ_CBTH_H;
    _IPE_YCFixEffectParam[IQSim_PC].CFixCbTh.th_low = pIpe->IPE_Register_78.Bit.CADJ_CBTH_L;
    _IPE_YCFixEffectParam[IQSim_PC].CFixCrTh.th_high = pIpe->IPE_Register_78.Bit.CADJ_CRTH_H;
    _IPE_YCFixEffectParam[IQSim_PC].CFixCrTh.th_low = pIpe->IPE_Register_78.Bit.CADJ_CRTH_L;
    iqInfo->YCFIX_EFFECT_PARAM = &_IPE_YCFixEffectParam[IQSim_PC];
    ///< Edge effect related parameters
    _IPE_EdgeEffectParam[IQSim_PC].EInvP = pIpe->IPE_Register_58.Bit.Y_EINV_P;
    _IPE_EdgeEffectParam[IQSim_PC].EInvN = pIpe->IPE_Register_58.Bit.Y_EINV_N;
    iqInfo->EDGE_EFFECT_PARAM = &_IPE_EdgeEffectParam[IQSim_PC];
    ///< Extension Color parameters
    _IPE_ColorParamExt[IQSim_PC].ColorGain_range = pIpe->IPE_Register_31.Bit.CGAIN_RANGE;
    _IPE_ColorParamExt[IQSim_PC].ColorGain_mask = pIpe->IPE_Register_31.Bit.CGAIN_MASK;
    _IPE_ColorParamExt[IQSim_PC].Ccr_set = pIpe->IPE_Register_23.Bit.CCR_SET;
    _IPE_ColorParamExt[IQSim_PC].Cadj_lmfsel = pIpe->IPE_Register_60.Bit.CADJ_LMFSEL;
    iqInfo->COLOR_PARAM_EXT = &_IPE_ColorParamExt[IQSim_PC];
    
    ///< VA related parameters
    //iqInfo->VA_PARAM =;               
}

void IQSim_IPE_DumpIQ(void)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_IPE_REG_STRUCT* pIpe = &(pBin->Ipe);
    
    sprintf(SLOG_buf, "//IPE parameters-------------------------------------------------------\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) STR_COLOR_INFOR IQ_IPE_ColorGain=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  ChR, ChGr, ChGb, ChB\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        pIpe->IPE_Register_32.Bit.CGAIN0,
        pIpe->IPE_Register_32.Bit.CGAIN1,
        pIpe->IPE_Register_33.Bit.CGAIN2,
        pIpe->IPE_Register_33.Bit.CGAIN3
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) STR_COLOR_INFOR IQ_IPE_ColorOffset=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  ChR, ChGr, ChGb, ChB\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        pIpe->IPE_Register_34.Bit.COFS0,
        pIpe->IPE_Register_34.Bit.COFS1,
        pIpe->IPE_Register_34.Bit.COFS2,
        pIpe->IPE_Register_34.Bit.COFS3
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) INT16 IQ_IPE_ColorMatrixParam[9]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  3x3 ccr-matrix values\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    0x%04X,0x%04X,0x%04X,\r\n",
        pIpe->IPE_Register_23.Bit.COEF_RR,
        pIpe->IPE_Register_24.Bit.COEF_RG,
        pIpe->IPE_Register_24.Bit.COEF_RB
        );SLOG_OUT;
    sprintf(SLOG_buf, "    0x%04X,0x%04X,0x%04X,\r\n",
        pIpe->IPE_Register_25.Bit.COEF_GR,
        pIpe->IPE_Register_25.Bit.COEF_GG,
        pIpe->IPE_Register_26.Bit.COEF_GB
        );SLOG_OUT;
    sprintf(SLOG_buf, "    0x%04X,0x%04X,0x%04X\r\n",
        pIpe->IPE_Register_26.Bit.COEF_BR,
        pIpe->IPE_Register_27.Bit.COEF_BG,
        pIpe->IPE_Register_27.Bit.COEF_BB
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) STR_IPE_NOVANR_PARA IQ_IPE_NRParam=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  GBAL, Nsmar, Lpfw, Sumsel\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        pIpe->IPE_Register_20.Bit.GBAL,
        pIpe->IPE_Register_22.Bit.INTG_NSMAR,
        pIpe->IPE_Register_22.Bit.LPFW,
        pIpe->IPE_Register_22.Bit.SUM_SEL
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) STR_IPE_EDGE_ENHMAP_PARA IQ_IPE_EdgeParam=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  EnhP, EnhN, EmapA, EmapB, EmapC, EmapD, EmapN, EmapM, EmapE\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
        pIpe->IPE_Register_58.Bit.Y_ENH_P,
        pIpe->IPE_Register_58.Bit.Y_ENH_N,
        pIpe->IPE_Register_75.Bit.EMAPA,
        pIpe->IPE_Register_75.Bit.EMAPB,
        pIpe->IPE_Register_75.Bit.EMAPC,
        pIpe->IPE_Register_76.Bit.EMAPD,
        pIpe->IPE_Register_76.Bit.EMAPN,
        pIpe->IPE_Register_76.Bit.EMAPM,
        pIpe->IPE_Register_76.Bit.EMAPE
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) INT16 IQ_IPE_EdgeKernelDiv[10]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  3x3 edge-kernel (5-bits signed) and 1 edge-div (3-bits)\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,\r\n",
        SIGN(pIpe->IPE_Register_74.Bit.EEXT5, 5),
        SIGN(pIpe->IPE_Register_74.Bit.EEXT4, 5),
        SIGN(pIpe->IPE_Register_73.Bit.EEXT2_V, 5)
        );SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,\r\n",
        SIGN(pIpe->IPE_Register_74.Bit.EEXT4, 5),
        SIGN(pIpe->IPE_Register_74.Bit.EEXT3, 5),
        SIGN(pIpe->IPE_Register_73.Bit.EEXT1_V, 5)
        );SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        SIGN(pIpe->IPE_Register_73.Bit.EEXT2_H, 5),
        SIGN(pIpe->IPE_Register_73.Bit.EEXT1_H, 5),
        SIGN(pIpe->IPE_Register_73.Bit.EEXT0_H, 5),
        pIpe->IPE_Register_74.Bit.EEXTDIV
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) UINT8 IQ_IPE_emStab[16]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  16 EStab values\r\n");SLOG_OUT;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_103.Word);
        i=0;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
            pSrc[i+0],pSrc[i+1],pSrc[i+2],pSrc[i+3],
            pSrc[i+4],pSrc[i+5],pSrc[i+6],pSrc[i+7],
            pSrc[i+8],pSrc[i+9],pSrc[i+10],pSrc[i+11],
            pSrc[i+12],pSrc[i+13],pSrc[i+14],pSrc[i+15]
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) UINT8 IQ_IPE_fmStab[16]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  16 FStab values\r\n");SLOG_OUT;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_107.Word);
        i=0;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
            pSrc[i+0],pSrc[i+1],pSrc[i+2],pSrc[i+3],
            pSrc[i+4],pSrc[i+5],pSrc[i+6],pSrc[i+7],
            pSrc[i+8],pSrc[i+9],pSrc[i+10],pSrc[i+11],
            pSrc[i+12],pSrc[i+13],pSrc[i+14],pSrc[i+15]
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) UINT8 IQ_IPE_fmDtab[16]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  16 FDtab values\r\n");SLOG_OUT;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_111.Word);
        i=0;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
            pSrc[i+0],pSrc[i+1],pSrc[i+2],pSrc[i+3],
            pSrc[i+4],pSrc[i+5],pSrc[i+6],pSrc[i+7],
            pSrc[i+8],pSrc[i+9],pSrc[i+10],pSrc[i+11],
            pSrc[i+12],pSrc[i+13],pSrc[i+14],pSrc[i+15]
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) UINT8 IQ_IPE_hueTab[24]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  24 hueTab values\r\n");SLOG_OUT;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_37.Word);
        i=0;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
            pSrc[i+0],pSrc[i+1],pSrc[i+2],pSrc[i+3],
            pSrc[i+4],pSrc[i+5],pSrc[i+6],pSrc[i+7],
            pSrc[i+8],pSrc[i+9],pSrc[i+10],pSrc[i+11],
            pSrc[i+12],pSrc[i+13],pSrc[i+14],pSrc[i+15],
            pSrc[i+16],pSrc[i+17],pSrc[i+18],pSrc[i+19],
            pSrc[i+20],pSrc[i+21],pSrc[i+22],pSrc[i+23]
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) INT16 IQ_IPE_satTab[24]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  24 satTab values : (8-bits signed)\r\n");SLOG_OUT;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_49.Word);
        i=0;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
            SIGN(pSrc[i+0],8),SIGN(pSrc[i+1],8),SIGN(pSrc[i+2],8),SIGN(pSrc[i+3],8),
            SIGN(pSrc[i+4],8),SIGN(pSrc[i+5],8),SIGN(pSrc[i+6],8),SIGN(pSrc[i+7],8),
            SIGN(pSrc[i+8],8),SIGN(pSrc[i+9],8),SIGN(pSrc[i+10],8),SIGN(pSrc[i+11],8),
            SIGN(pSrc[i+12],8),SIGN(pSrc[i+13],8),SIGN(pSrc[i+14],8),SIGN(pSrc[i+15],8),
            SIGN(pSrc[i+16],8),SIGN(pSrc[i+17],8),SIGN(pSrc[i+18],8),SIGN(pSrc[i+19],8),
            SIGN(pSrc[i+20],8),SIGN(pSrc[i+21],8),SIGN(pSrc[i+22],8),SIGN(pSrc[i+23],8)
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) INT16 IQ_IPE_intTab[24]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  24 intTab values : (8-bits signed)\r\n");SLOG_OUT;
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pIpe->IPE_Register_43.Word);
        i=0;
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
            SIGN(pSrc[i+0],8),SIGN(pSrc[i+1],8),SIGN(pSrc[i+2],8),SIGN(pSrc[i+3],8),
            SIGN(pSrc[i+4],8),SIGN(pSrc[i+5],8),SIGN(pSrc[i+6],8),SIGN(pSrc[i+7],8),
            SIGN(pSrc[i+8],8),SIGN(pSrc[i+9],8),SIGN(pSrc[i+10],8),SIGN(pSrc[i+11],8),
            SIGN(pSrc[i+12],8),SIGN(pSrc[i+13],8),SIGN(pSrc[i+14],8),SIGN(pSrc[i+15],8),
            SIGN(pSrc[i+16],8),SIGN(pSrc[i+17],8),SIGN(pSrc[i+18],8),SIGN(pSrc[i+19],8),
            SIGN(pSrc[i+20],8),SIGN(pSrc[i+21],8),SIGN(pSrc[i+22],8),SIGN(pSrc[i+23],8)
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) UINT8 IQ_IPE_ddsTab[8]=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  8 ddsTab values\r\n");SLOG_OUT;
    {
        sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d,%d,%d\r\n",
            pIpe->IPE_Register_55.Bit.DDS0,
            pIpe->IPE_Register_55.Bit.DDS1,
            pIpe->IPE_Register_55.Bit.DDS2,
            pIpe->IPE_Register_55.Bit.DDS3,
            pIpe->IPE_Register_56.Bit.DDS4,
            pIpe->IPE_Register_56.Bit.DDS5,
            pIpe->IPE_Register_56.Bit.DDS6,
            pIpe->IPE_Register_56.Bit.DDS7
        );SLOG_OUT;
    }
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) IPE_COLOR_EFFECT_PARAM IQ_IPE_ColorEffectParam=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  CbOfs, CrOfs, YCon, CCon, IntOfs, SatOfs\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d\r\n",
        pIpe->IPE_Register_60.Bit.C_CBOFS,
        pIpe->IPE_Register_60.Bit.C_CROFS,
        pIpe->IPE_Register_59.Bit.Y_CON,
        pIpe->IPE_Register_60.Bit.C_CON,
        pIpe->IPE_Register_36.Bit.INT_OFS,
        pIpe->IPE_Register_36.Bit.SAT_OFS
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) IPE_INVERSE_EFFECT_PARAM IQ_IPE_InvertEffectParam=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  Inv, Hinv\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d\r\n",
        pIpe->IPE_Register_31.Bit.CGAIN_INV,
        pIpe->IPE_Register_31.Bit.CGAIN_HINV
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) IPE_YCFIX_EFFECT_PARAM IQ_IPE_YCFixEffectParam=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  YFix, CbFix, CrFix, YFixEth, CFixEth, CFixYth.l, CFixYth.h, CFixCbth.l, CFixCbth.h, CFixCrth.l, CFixCrth.h\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,{%d,%d},{%d,%d},{%d,%d}\r\n",
        pIpe->IPE_Register_57.Bit.Y_FIX,
        pIpe->IPE_Register_57.Bit.CB_FIX,
        pIpe->IPE_Register_57.Bit.CR_FIX,
        pIpe->IPE_Register_58.Bit.Y_ETHY,
        pIpe->IPE_Register_59.Bit.Y_ETHC,
        pIpe->IPE_Register_77.Bit.CADJ_YTH_L,
        pIpe->IPE_Register_77.Bit.CADJ_YTH_H,
        pIpe->IPE_Register_78.Bit.CADJ_CBTH_L,
        pIpe->IPE_Register_78.Bit.CADJ_CBTH_H,
        pIpe->IPE_Register_78.Bit.CADJ_CRTH_L,
        pIpe->IPE_Register_78.Bit.CADJ_CRTH_H
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) IPE_EDGE_EFFECT_PARAM IQ_IPE_EdgeEffectParam=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  EinvP, EinvN\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d\r\n",
        pIpe->IPE_Register_58.Bit.Y_EINV_P,
        pIpe->IPE_Register_58.Bit.Y_EINV_N
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) STR_IPE_COLOR_PARA_EXT IQ_IPE_ColorParamExt=\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  ColorGain_range, ColorGain_mask, Ccr_set, Cadj_lmfsel\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d\r\n",
        pIpe->IPE_Register_31.Bit.CGAIN_RANGE,
        pIpe->IPE_Register_31.Bit.CGAIN_MASK,
        pIpe->IPE_Register_23.Bit.CCR_SET,
        pIpe->IPE_Register_60.Bit.CADJ_LMFSEL
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
}

void IQSim_IPE_SetSrc(IPE_MODEINFO *ipeParam, UINT32 IQSimMode)
{
    IPE_IQINFO* iqInfo = &(ipeParam->IQINFO);
    ///< Enable IFE embeded in IPE
    iqInfo->IFEembed_en = FALSE;
    
    ipeParam->SIZEINFO.INSIZE.hsize = gBuf_SIE.w;
    ipeParam->SIZEINFO.INSIZE.vsize = gBuf_SIE.h;
    ipeParam->SIZEINFO.LofsIn = gBuf_SIE.lineoff;
    ipeParam->SIZEINFO.CfaPat = gRAW_cfa;
    ipeParam->ININFO.Adr0 = gBuf_SIE.addr[0]; //raw input addr (sie out addr)
    ipeParam->ININFO.Adr1 = 0;
    ipeParam->ININFO.Adr2 = 0;
    ipeParam->ININFO.PPBEn = DISABLE;
    ipeParam->ININFO.PPBSel = DISABLE; //Auto
    ipeParam->ININFO.PPBIni = PPBINI_0;
    if(gBuf_SIE.fmt == IQ_FMT_RAW8)
    ipeParam->ININFO.PackBit = IPE_PACK_08bit;
    if(gBuf_SIE.fmt == IQ_FMT_RAW10)
    ipeParam->ININFO.PackBit = IPE_PACK_10bit;
    ipeParam->ININFO.RGBInEn = DISABLE; //RAW or RGB
    ipeParam->SIZEINFO.DVIformat = DVI_CBYCRY;
}


void IQSim_IPE_SetDest(IPE_MODEINFO *ipeParam, UINT32 IQSimMode)
{
    //set IPE out stripe
    ipeParam->SIZEINFO.HSTRIPE.n = _OutInfo.Ipe_Hstripe.n;
    ipeParam->SIZEINFO.HSTRIPE.l = _OutInfo.Ipe_Hstripe.l;
    ipeParam->SIZEINFO.HSTRIPE.m = _OutInfo.Ipe_Hstripe.m;
    ipeParam->SIZEINFO.HSTRIPE.olap = IPE_OLAP_ON;
    ipeParam->SIZEINFO.VSTRIPE.n = _OutInfo.Ipe_Vstripe.n;
    ipeParam->SIZEINFO.VSTRIPE.l = _OutInfo.Ipe_Vstripe.l;
    ipeParam->SIZEINFO.VSTRIPE.m = _OutInfo.Ipe_Vstripe.m;
    ipeParam->SIZEINFO.VSTRIPE.olap = IPE_OLAP_ON;
    //if(IPEModeInfo.SIZEINFO.StripeMode == IPE_AUTOSLICE)
    //    ipe_setSliceH(job_req->IpeParam->IpeSliceH);
    //else
        ipe_setSliceH(0);
}

void IQSim_IPE_Start(IPE_MODEINFO *ipeParam, int IQSimMode, IPE_StripeEnum mode)
{
    //set IPE out control
    ipeParam->SIZEINFO.StripeMode = mode;
    
    ipe_setMode(ipeParam);
    //if((JobType == PREVIEW_JOB) || (JobType == VIDEO_JOB ))
    //    ipe_start();
    debug_ipe(("IPE in w,h,fmt,cfa=%d,%d,%d,%d\r\n", 
        ipeParam->SIZEINFO.INSIZE.hsize,
        ipeParam->SIZEINFO.INSIZE.vsize,
        ipeParam->ININFO.PackBit,
        ipeParam->SIZEINFO.CfaPat
        ));
    debug_ipe(("IPE in lineoff=%d\r\n", 
        ipeParam->SIZEINFO.LofsIn
        ));
    debug_ipe(("IPE in addr=%08x\r\n", 
        ipeParam->ININFO.Adr0
        ));
    debug_ipe(("IPE start\r\n"));
    ipe_start();
}

//////////////////////////////////////////////////////////////////

void IQSim_IPE_Open(void)
{
    IPE_OPENOBJ IPEOpenObj;
    IPEOpenObj.FP_IPEISR_CB = NULL;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_96;
    ipe_open(&IPEOpenObj);
}    
UINT32 IQSim_IPE_Wait(void)
{
    debug_ipe(("IPE wait\r\n"));
    ipe_waitFrameEnd(FALSE);
    debug_ipe(("IPE finish\r\n"));
    return 1;
}
void IQSim_IPE_Stop(void)
{
}

void IQSim_IPE_Close(void)
{
    ipe_close();
}    

void IQSim_IPE_DumpSetting(void)
{
    memcpy(&dumpBin.Ipe, (void*)IQSim_IPE_ADDR, sizeof(NT96220_IPE_REG_STRUCT));
}


