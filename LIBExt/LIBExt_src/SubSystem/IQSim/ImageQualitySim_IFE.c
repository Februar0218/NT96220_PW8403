#include "ImageQualitySim_int.h"

#if (IFE_DEBUG_FLOW == ENABLE)
#define debug_ife   debug_err
#else
#define debug_ife   debug_ind
#endif

#if (IFE2_DEBUG_FLOW == ENABLE)
#define debug_ife2   debug_err
#else
#define debug_ife2   debug_ind
#endif

UINT32 ci_table[5] = {0,0,0,1,2};

void IQSim_IFE_SetIQ(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, int ti)
{
 
    if(cnl == IQ_CNL_RAW)
    {
    ///< enable of outlier filter
    ifeParam->outlEn = ENABLE;      
    ///< outlier filter setting
    ifeParam->outlSet = _IFE_outlSet_raw[IQSim_DEF];   
    
    ///< enable of bilateral filter
    ifeParam->filterEn = ENABLE;   
    ///< spatial only enable
    ifeParam->sOnlyEn = _IFE_rngSet_raw[IQSim_DEF].sOnlyEn;     
    ///< filter length of spatial only
    ifeParam->sOnlyLen = _IFE_rngSet_raw[IQSim_DEF].sOnlyLen;    
    ///< image binning
    ifeParam->binn = _IFE_rngSet_raw[IQSim_DEF].binn;   
    ///< spatial weighting
    ifeParam->weights = _IFE_weight_raw[IQSim_DEF];  
    ///< range filter threshold setting for GR & GB channels or progressive data
    ifeParam->rngth_g.rngth = _IFE_rngth_raw[IQSim_DEF][IFE_G];  
    ifeParam->rngth_g.rdelta = _IFE_rdelta_raw[IQSim_DEF][IFE_G];  
    ///< range filter threshold setting for R & B channels
    ifeParam->rngth_rb.rngth = _IFE_rngth_raw[IQSim_DEF][IFE_RB];     
    ifeParam->rngth_rb.rdelta = _IFE_rdelta_raw[IQSim_DEF][IFE_RB];  
    ///< clamp and weighting setting
    ifeParam->clampWtSet = _IFE_clampWtSet_raw[IQSim_DEF]; 
    
    ///< disable of curve mapping
    ifeParam->crvmapEn = DISABLE;
    }
    else if(cnl == IQ_CNL_Y)
    {
    ///< enable of outlier filter
    ifeParam->outlEn = ENABLE;      
    ///< outlier filter setting
    ifeParam->outlSet = _IFE_outlSet_y[IQSim_DEF];   
    
    ///< enable of bilateral filter
    ifeParam->filterEn = ENABLE;   
    ///< spatial only enable
    ifeParam->sOnlyEn = _IFE_rngSet_y[IQSim_DEF].sOnlyEn;   
    ///< filter length of spatial only
    ifeParam->sOnlyLen = _IFE_rngSet_y[IQSim_DEF].sOnlyLen;    
    ///< image binning
    ifeParam->binn = _IFE_rngSet_y[IQSim_DEF].binn;   
    ///< spatial weighting
    ifeParam->weights = _IFE_weight_y[IQSim_DEF];  
    ///< range filter threshold setting for GR & GB channels or progressive data
    ifeParam->rngth_g.rngth = _IFE_rngth_y[IQSim_DEF][IFE_G];  
    ifeParam->rngth_g.rdelta = _IFE_rdelta_y[IQSim_DEF][IFE_G];  
    ///< range filter threshold setting for R & B channels
    ifeParam->rngth_rb.rngth = _IFE_rngth_y[IQSim_DEF][IFE_RB];     
    ifeParam->rngth_rb.rdelta = _IFE_rdelta_y[IQSim_DEF][IFE_RB];  
     ///< clamp and weighting setting
    ifeParam->clampWtSet = _IFE_clampWtSet_y[IQSim_DEF]; 
   
    ///< disable of curve mapping
    ifeParam->crvmapEn = DISABLE;
    }
    else if(cnl == IQ_CNL_Cb || cnl == IQ_CNL_Cr)
    {
    ///< enable of outlier filter
    ifeParam->outlEn = ENABLE;      
    ///< outlier filter setting
    ifeParam->outlSet = _IFE_outlSet_cc[IQSim_DEF];   
    
    ///< enable of bilateral filter
    ifeParam->filterEn = ENABLE;   
    ///< spatial only enable
    ifeParam->sOnlyEn = _IFE_rngSet_cc[IQSim_DEF].sOnlyEn;   
    ///< filter length of spatial only
    ifeParam->sOnlyLen = _IFE_rngSet_cc[IQSim_DEF].sOnlyLen;   
    ///< image binning
    ifeParam->binn = _IFE_rngSet_cc[IQSim_DEF].binn;   
    ///< spatial weighting
    ifeParam->weights = _IFE_weight_cc[IQSim_DEF];  
    ///< range filter threshold setting for GR & GB channels or progressive data
    ifeParam->rngth_g.rngth = _IFE_rngth_cc[IQSim_DEF][IFE_G];  
    ifeParam->rngth_g.rdelta = _IFE_rdelta_cc[IQSim_DEF][IFE_G];  
    ///< range filter threshold setting for R & B channels
    ifeParam->rngth_rb.rngth = _IFE_rngth_cc[IQSim_DEF][IFE_RB];     
    ifeParam->rngth_rb.rdelta = _IFE_rdelta_cc[IQSim_DEF][IFE_RB];  
    ///< clamp and weighting setting
    ifeParam->clampWtSet = _IFE_clampWtSet_cc[IQSim_DEF]; 
    
    ///< enable of curve mapping
    //ifeParam->crvmapEn = ENABLE;
    ifeParam->crvmapEn = DISABLE;
    ///< curve mapping setting
    ifeParam->crvmapSet = _IFE_crvmapSet_cc[IQSim_DEF];   
    ifeParam->crvmapSet.crvmapGamma = _IFE_crvmapGamma_cc[IQSim_DEF];   
    }


    ///< enable of row defect concealment
    ifeParam->rowDefEn = DISABLE;    
    ///< vertical start of cropped window
    ifeParam->rowDefVIni = 0; 
    ///< table of row defect concealment
    ifeParam->rowDefTbl = _IFE_rowdefTbl[IQSim_DEF];          
}

void IQSim_IFE_SetIQ_2(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, int ti)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_IFE_REG_STRUCT* pIfe = 0;
    NT96220_SRA_REG_STRUCT* pSra = 0;
    NT96220_IPP_EXTRN_REG_STRUCT* pIfeExt = 0;

    IFE_OUTLSET (*_IFE_outlSet_tmp)[Total_ParamNum] = 0;
    IFE_RANGESET2 (*_IFE_rngSet_tmp)[Total_ParamNum] = 0;
    UINT32 (*_IFE_weight_tmp)[Total_ParamNum][6] = 0;
    UINT32 (*_IFE_rngth_tmp)[Total_ParamNum][IFE_RB+1][2] = 0;
    INT32 (*_IFE_rdelta_tmp)[Total_ParamNum][IFE_RB+1][4] = 0;
    IFE_CLAMPWTSET (*_IFE_clampWtSet_tmp)[Total_ParamNum] = 0;
    IFE_CRVMAPSET (*_IFE_crvmapSet_tmp)[Total_ParamNum] = 0;
    UINT32 (*_IFE_crvmapGamma_tmp)[Total_ParamNum][65] = 0;
    BOOL bSupportCrvmap = FALSE;
    
    if(cnl == IQ_CNL_RAW)
    {
        pIfe = &(pBin->IfeRaw);
        pIfeExt = &(pBin->IfeExt);
        _IFE_outlSet_tmp = &_IFE_outlSet_raw;
        _IFE_rngSet_tmp = &_IFE_rngSet_raw;
        _IFE_weight_tmp = &_IFE_weight_raw;
        _IFE_rngth_tmp = &_IFE_rngth_raw;
        _IFE_rdelta_tmp = &_IFE_rdelta_raw;
        _IFE_clampWtSet_tmp = &_IFE_clampWtSet_raw;
    }        
    if(cnl == IQ_CNL_Y)
    {
        pIfe = &(pBin->IfeY);
        _IFE_outlSet_tmp = &_IFE_outlSet_y;
        _IFE_rngSet_tmp = &_IFE_rngSet_y;
        _IFE_weight_tmp = &_IFE_weight_y;
        _IFE_rngth_tmp = &_IFE_rngth_y;
        _IFE_rdelta_tmp = &_IFE_rdelta_y;
        _IFE_clampWtSet_tmp = &_IFE_clampWtSet_y;
    }        
    if(cnl == IQ_CNL_Cb || cnl == IQ_CNL_Cr)
    {
        pIfe = &(pBin->IfeCC);
        pSra = &(pBin->IfeCCCrv);
        _IFE_outlSet_tmp = &_IFE_outlSet_cc;
        _IFE_rngSet_tmp = &_IFE_rngSet_cc;
        _IFE_weight_tmp = &_IFE_weight_cc;
        _IFE_rngth_tmp = &_IFE_rngth_cc;
        _IFE_rdelta_tmp = &_IFE_rdelta_cc;
        _IFE_clampWtSet_tmp = &_IFE_clampWtSet_cc;
        bSupportCrvmap = TRUE;
        _IFE_crvmapSet_tmp = &_IFE_crvmapSet_cc;
        _IFE_crvmapGamma_tmp = &_IFE_crvmapGamma_cc;
    }        
    
    ///< enable of row defect concealment
    ifeParam->rowDefEn = DISABLE;    
    ///< vertical start of cropped window
    ifeParam->rowDefVIni = 0; 
    ///< table of row defect concealment
    ifeParam->rowDefTbl = _IFE_rowdefTbl[IQSim_PC];          

    if((cnl == IQ_CNL_RAW && ti==0)
        || (cnl == IQ_CNL_Y)
        || (cnl == IQ_CNL_Cb || cnl == IQ_CNL_Cr))
    {
    //-------------------------------------------------------------------------
    //if(cnl == IQ_CNL_RAW && ti==0)
        //debug_msg("IFE RAW-1\r\n");
    
    ///< enable of outlier filter
    ifeParam->outlEn = pIfe->IFE_Register_1.Bit.OUTL_EN;      
    if(ifeParam->outlEn)
    {
    ///< outlier filter setting
    (*_IFE_outlSet_tmp)[IQSim_PC].outlBriTh = pIfe->IFE_Register_19.Bit.OUTLTH_BRI;
    (*_IFE_outlSet_tmp)[IQSim_PC].outlDarkTh = pIfe->IFE_Register_19.Bit.OUTLTH_DARK;
    (*_IFE_outlSet_tmp)[IQSim_PC].outlOp = pIfe->IFE_Register_19.Bit.OUTCNT_SEL;
    ifeParam->outlSet = (*_IFE_outlSet_tmp)[IQSim_PC];   
    }

    ///< enable of bilateral filter
    ifeParam->filterEn = pIfe->IFE_Register_1.Bit.FILTER_EN;   
    if(ifeParam->filterEn)
    {
    (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyEn = pIfe->IFE_Register_2.Bit.S_ONLY_EN;
    (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyLen = pIfe->IFE_Register_2.Bit.S_ONLY_LEN;
    (*_IFE_rngSet_tmp)[IQSim_PC].binn = pIfe->IFE_Register_2.Bit.BINNING;
    ///< spatial only enable
    ifeParam->sOnlyEn = (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyEn;     
    ///< filter length of spatial only
    ifeParam->sOnlyLen = (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyLen;    
    ///< image binning
    ifeParam->binn = (*_IFE_rngSet_tmp)[IQSim_PC].binn;   
    ///< spatial weighting
    (*_IFE_weight_tmp)[IQSim_PC][0] = pIfe->IFE_Register_2.Bit.S_WEIGHT0;
    (*_IFE_weight_tmp)[IQSim_PC][1] = pIfe->IFE_Register_2.Bit.S_WEIGHT1;
    (*_IFE_weight_tmp)[IQSim_PC][2] = pIfe->IFE_Register_2.Bit.S_WEIGHT2;
    (*_IFE_weight_tmp)[IQSim_PC][3] = pIfe->IFE_Register_2.Bit.S_WEIGHT3;
    (*_IFE_weight_tmp)[IQSim_PC][4] = pIfe->IFE_Register_2.Bit.S_WEIGHT4;
    (*_IFE_weight_tmp)[IQSim_PC][5] = pIfe->IFE_Register_2.Bit.S_WEIGHT5;
    ifeParam->weights = (*_IFE_weight_tmp)[IQSim_PC];  
    ///< range filter threshold setting for GR & GB channels or progressive data
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_G][0] = pIfe->IFE_Register_3.Bit.RTH_G0;
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_G][1] = pIfe->IFE_Register_3.Bit.RTH_G1;
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][0] = SIGN(pIfe->IFE_Register_15.Bit.RDLT_G0,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][1] = SIGN(pIfe->IFE_Register_15.Bit.RDLT_G1,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][2] = SIGN(pIfe->IFE_Register_15.Bit.RDLT_G2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][3] = SIGN(pIfe->IFE_Register_15.Bit.RDLT_G3,6);
    ifeParam->rngth_g.rngth = (*_IFE_rngth_tmp)[IQSim_PC][IFE_G];  
    ifeParam->rngth_g.rdelta = (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G];  
    ///< range filter threshold setting for R & B channels
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB][0] = pIfe->IFE_Register_4.Bit.RTH_RB0;
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB][1] = pIfe->IFE_Register_4.Bit.RTH_RB1;
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][0] = SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB0,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][1] = SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB1,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][2] = SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][3] = SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB3,6);
    ifeParam->rngth_rb.rngth = (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB];     
    ifeParam->rngth_rb.rdelta = (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB];     
    ///< clamp and weighting setting
    (*_IFE_clampWtSet_tmp)[IQSim_PC].clampTh = pIfe->IFE_Register_10.Bit.CLAMP_TH;
    (*_IFE_clampWtSet_tmp)[IQSim_PC].wtMul_g = pIfe->IFE_Register_10.Bit.CLAMPMUL_G;
    (*_IFE_clampWtSet_tmp)[IQSim_PC].wtMul_rb = pIfe->IFE_Register_10.Bit.CLAMPMUL_RB;
    ifeParam->clampWtSet = (*_IFE_clampWtSet_tmp)[IQSim_PC]; 
    }
    
    ///< enable of curve mapping
    ifeParam->crvmapEn = (bSupportCrvmap)?(pIfe->IFE_Register_1.Bit.CRVMAP_EN):(DISABLE);
    if(ifeParam->crvmapEn)
    {
    ///< curve mapping setting
    (*_IFE_crvmapSet_tmp)[IQSim_PC].crvmapDelta = pIfe->IFE_Register_11.Bit.CRVMAP_DELTA;
    (*_IFE_crvmapSet_tmp)[IQSim_PC].crvmapInOffs = SIGN(pIfe->IFE_Register_11.Bit.CRVMAP_INOFFS,5);
    (*_IFE_crvmapSet_tmp)[IQSim_PC].crvmapOutOffs = SIGN(pIfe->IFE_Register_11.Bit.CRVMAP_OUTOFFS,5);
    ifeParam->crvmapSet = (*_IFE_crvmapSet_tmp)[IQSim_PC]; 
    {
        int i;
        UINT8* pSrc = (UINT8*)&(pSra->SRA_Register_4.Word);
        for(i=0;i<=64;i++)
            (*_IFE_crvmapGamma_tmp)[IQSim_PC][i] = pSrc[i];
    }
    ifeParam->crvmapSet.crvmapGamma = (*_IFE_crvmapGamma_tmp)[IQSim_PC];   
    }
    
    //-------------------------------------------------------------------------
    }
    else
    if(cnl == IQ_CNL_RAW && ti==1)
    {
    //-------------------------------------------------------------------------
        //debug_msg("IFE RAW-2\r\n");
    
    ///< enable of outlier filter
    ifeParam->outlEn = pIfeExt->IPP_Ext_Register_0.Bit.OUTL_EN_2;      
    if(ifeParam->outlEn)
    {
    ///< outlier filter setting
    (*_IFE_outlSet_tmp)[IQSim_PC].outlBriTh = pIfeExt->IPP_Ext_Register_5.Bit.OUTLTH_BRI_2;
    (*_IFE_outlSet_tmp)[IQSim_PC].outlDarkTh = pIfeExt->IPP_Ext_Register_5.Bit.OUTLTH_DARK_2;
    (*_IFE_outlSet_tmp)[IQSim_PC].outlOp = pIfeExt->IPP_Ext_Register_5.Bit.OUTCNT_SEL_2;
    ifeParam->outlSet = (*_IFE_outlSet_tmp)[IQSim_PC];   
    }

    ///< enable of bilateral filter
    ifeParam->filterEn = pIfeExt->IPP_Ext_Register_0.Bit.FILTER_EN_2;   
    if(ifeParam->filterEn)
    {
    (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyEn = pIfeExt->IPP_Ext_Register_0.Bit.S_ONLY_EN_2;
    (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyLen = pIfeExt->IPP_Ext_Register_0.Bit.S_ONLY_LEN_2;
    (*_IFE_rngSet_tmp)[IQSim_PC].binn = pIfeExt->IPP_Ext_Register_0.Bit.BINNING_2;
    ///< spatial only enable
    ifeParam->sOnlyEn = (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyEn;     
    ///< filter length of spatial only
    ifeParam->sOnlyLen = (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyLen;    
    ///< image binning
    ifeParam->binn = (*_IFE_rngSet_tmp)[IQSim_PC].binn;   
    ///< spatial weighting
    (*_IFE_weight_tmp)[IQSim_PC][0] = pIfeExt->IPP_Ext_Register_0.Bit.S_WEIGHT0_2;
    (*_IFE_weight_tmp)[IQSim_PC][1] = pIfeExt->IPP_Ext_Register_0.Bit.S_WEIGHT1_2;
    (*_IFE_weight_tmp)[IQSim_PC][2] = pIfeExt->IPP_Ext_Register_0.Bit.S_WEIGHT2_2;
    (*_IFE_weight_tmp)[IQSim_PC][3] = pIfeExt->IPP_Ext_Register_0.Bit.S_WEIGHT3_2;
    (*_IFE_weight_tmp)[IQSim_PC][4] = pIfeExt->IPP_Ext_Register_0.Bit.S_WEIGHT4_2;
    (*_IFE_weight_tmp)[IQSim_PC][5] = pIfeExt->IPP_Ext_Register_0.Bit.S_WEIGHT5_2;
    ifeParam->weights = (*_IFE_weight_tmp)[IQSim_PC];  
    ///< range filter threshold setting for GR & GB channels or progressive data
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_G][0] = pIfeExt->IPP_Ext_Register_1.Bit.RTH_G0_2;
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_G][1] = pIfeExt->IPP_Ext_Register_1.Bit.RTH_G1_2;
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][0] = SIGN(pIfeExt->IPP_Ext_Register_3.Bit.RDLT_G0_2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][1] = SIGN(pIfeExt->IPP_Ext_Register_3.Bit.RDLT_G1_2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][2] = SIGN(pIfeExt->IPP_Ext_Register_3.Bit.RDLT_G2_2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][3] = SIGN(pIfeExt->IPP_Ext_Register_3.Bit.RDLT_G3_2,6);
    ifeParam->rngth_g.rngth = (*_IFE_rngth_tmp)[IQSim_PC][IFE_G];  
    ifeParam->rngth_g.rdelta = (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G];  
    ///< range filter threshold setting for R & B channels
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB][0] = pIfeExt->IPP_Ext_Register_2.Bit.RTH_RB0_2;
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB][1] = pIfeExt->IPP_Ext_Register_2.Bit.RTH_RB1_2;
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][0] = SIGN(pIfeExt->IPP_Ext_Register_4.Bit.RDLT_RB0_2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][1] = SIGN(pIfeExt->IPP_Ext_Register_4.Bit.RDLT_RB1_2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][2] = SIGN(pIfeExt->IPP_Ext_Register_4.Bit.RDLT_RB2_2,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][3] = SIGN(pIfeExt->IPP_Ext_Register_4.Bit.RDLT_RB3_2,6);
    ifeParam->rngth_rb.rngth = (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB];     
    ifeParam->rngth_rb.rdelta = (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB];     
    ///< clamp and weighting setting
    (*_IFE_clampWtSet_tmp)[IQSim_PC].clampTh = pIfeExt->IPP_Ext_Register_6.Bit.CLAMP_TH_2;
    (*_IFE_clampWtSet_tmp)[IQSim_PC].wtMul_g = pIfeExt->IPP_Ext_Register_6.Bit.CLAMPMUL_G_2;
    (*_IFE_clampWtSet_tmp)[IQSim_PC].wtMul_rb = pIfeExt->IPP_Ext_Register_6.Bit.CLAMPMUL_RB_2;
    ifeParam->clampWtSet = (*_IFE_clampWtSet_tmp)[IQSim_PC]; 
    }
    
    ///< enable of curve mapping
    ifeParam->crvmapEn = DISABLE;
    ifeParam->crvmapSet.crvmapGamma = 0;   
    
    //-------------------------------------------------------------------------
    }
    else
    if(cnl == IQ_CNL_RAW && ti==2)
    {
    //-------------------------------------------------------------------------
        //debug_msg("IFE RAW-3\r\n");
    
    ///< enable of outlier filter
    ifeParam->outlEn = pIfeExt->IPP_Ext_Register_7.Bit.OUTL_EN_3;      
    if(ifeParam->outlEn)
    {
    ///< outlier filter setting
    (*_IFE_outlSet_tmp)[IQSim_PC].outlBriTh = pIfeExt->IPP_Ext_Register_12.Bit.OUTLTH_BRI_3;
    (*_IFE_outlSet_tmp)[IQSim_PC].outlDarkTh = pIfeExt->IPP_Ext_Register_12.Bit.OUTLTH_DARK_3;
    (*_IFE_outlSet_tmp)[IQSim_PC].outlOp = pIfeExt->IPP_Ext_Register_12.Bit.OUTCNT_SEL_3;
    ifeParam->outlSet = (*_IFE_outlSet_tmp)[IQSim_PC];   
    }

    ///< enable of bilateral filter
    ifeParam->filterEn = pIfeExt->IPP_Ext_Register_7.Bit.FILTER_EN_3;   
    if(ifeParam->filterEn)
    {
    (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyEn = pIfeExt->IPP_Ext_Register_7.Bit.S_ONLY_EN_3;
    (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyLen = pIfeExt->IPP_Ext_Register_7.Bit.S_ONLY_LEN_3;
    (*_IFE_rngSet_tmp)[IQSim_PC].binn = pIfeExt->IPP_Ext_Register_7.Bit.BINNING_3;
    ///< spatial only enable
    ifeParam->sOnlyEn = (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyEn;     
    ///< filter length of spatial only
    ifeParam->sOnlyLen = (*_IFE_rngSet_tmp)[IQSim_PC].sOnlyLen;    
    ///< image binning
    ifeParam->binn = (*_IFE_rngSet_tmp)[IQSim_PC].binn;   
    ///< spatial weighting
    (*_IFE_weight_tmp)[IQSim_PC][0] = pIfeExt->IPP_Ext_Register_7.Bit.S_WEIGHT0_3;
    (*_IFE_weight_tmp)[IQSim_PC][1] = pIfeExt->IPP_Ext_Register_7.Bit.S_WEIGHT1_3;
    (*_IFE_weight_tmp)[IQSim_PC][2] = pIfeExt->IPP_Ext_Register_7.Bit.S_WEIGHT2_3;
    (*_IFE_weight_tmp)[IQSim_PC][3] = pIfeExt->IPP_Ext_Register_7.Bit.S_WEIGHT3_3;
    (*_IFE_weight_tmp)[IQSim_PC][4] = pIfeExt->IPP_Ext_Register_7.Bit.S_WEIGHT4_3;
    (*_IFE_weight_tmp)[IQSim_PC][5] = pIfeExt->IPP_Ext_Register_7.Bit.S_WEIGHT5_3;
    ifeParam->weights = (*_IFE_weight_tmp)[IQSim_PC];  
    ///< range filter threshold setting for GR & GB channels or progressive data
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_G][0] = pIfeExt->IPP_Ext_Register_8.Bit.RTH_G0_3;
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_G][1] = pIfeExt->IPP_Ext_Register_8.Bit.RTH_G1_3;
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][0] = SIGN(pIfeExt->IPP_Ext_Register_10.Bit.RDLT_G0_3,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][1] = SIGN(pIfeExt->IPP_Ext_Register_10.Bit.RDLT_G1_3,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][2] = SIGN(pIfeExt->IPP_Ext_Register_10.Bit.RDLT_G2_3,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G][3] = SIGN(pIfeExt->IPP_Ext_Register_10.Bit.RDLT_G3_3,6);
    ifeParam->rngth_g.rngth = (*_IFE_rngth_tmp)[IQSim_PC][IFE_G];  
    ifeParam->rngth_g.rdelta = (*_IFE_rdelta_tmp)[IQSim_PC][IFE_G];  
    ///< range filter threshold setting for R & B channels
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB][0] = pIfeExt->IPP_Ext_Register_9.Bit.RTH_RB0_3;
    (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB][1] = pIfeExt->IPP_Ext_Register_9.Bit.RTH_RB1_3;
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][0] = SIGN(pIfeExt->IPP_Ext_Register_11.Bit.RDLT_RB0_3,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][1] = SIGN(pIfeExt->IPP_Ext_Register_11.Bit.RDLT_RB1_3,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][2] = SIGN(pIfeExt->IPP_Ext_Register_11.Bit.RDLT_RB2_3,6);
    (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB][3] = SIGN(pIfeExt->IPP_Ext_Register_11.Bit.RDLT_RB3_3,6);
    ifeParam->rngth_rb.rngth = (*_IFE_rngth_tmp)[IQSim_PC][IFE_RB];     
    ifeParam->rngth_rb.rdelta = (*_IFE_rdelta_tmp)[IQSim_PC][IFE_RB];     
    ///< clamp and weighting setting
    (*_IFE_clampWtSet_tmp)[IQSim_PC].clampTh = pIfeExt->IPP_Ext_Register_13.Bit.CLAMP_TH_3;
    (*_IFE_clampWtSet_tmp)[IQSim_PC].wtMul_g = pIfeExt->IPP_Ext_Register_13.Bit.CLAMPMUL_G_3;
    (*_IFE_clampWtSet_tmp)[IQSim_PC].wtMul_rb = pIfeExt->IPP_Ext_Register_13.Bit.CLAMPMUL_RB_3;
    ifeParam->clampWtSet = (*_IFE_clampWtSet_tmp)[IQSim_PC]; 
    }
    
    ///< enable of curve mapping
    ifeParam->crvmapEn = DISABLE;
    ifeParam->crvmapSet.crvmapGamma = 0;   
    
    //-------------------------------------------------------------------------
    }
}


void IQSim_IFE_DumpIQ(IQ_CNL cnl)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_IFE_REG_STRUCT* pIfe = 0;
    NT96220_SRA_REG_STRUCT* pSra = 0;
    char sName[3][8] = {"raw","y","cc"};
    char* cName = 0;

    if(cnl==IQ_CNL_RAW)
    {    
        cName = sName[0];
        pIfe = &(pBin->IfeRaw);    
        pSra = 0;        
    }
    if(cnl==IQ_CNL_Y)
    {    
        cName = sName[1];
        pIfe = &(pBin->IfeY);    
        pSra = 0;        
    }
    if((cnl==IQ_CNL_Cb)||(cnl==IQ_CNL_Cr))
    {    
        cName = sName[2];
        pIfe = &(pBin->IfeCC);    
        pSra = &(pBin->SieGamma);        
    }
    
    sprintf(SLOG_buf, "//IFE parameters-------------------------------------------------------\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) IFE_OUTLSET IQ_IFE_outlSet_%s=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  outlBriTh, outlDarkTh, OutlOp\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d\r\n",
        pIfe->IFE_Register_19.Bit.OUTLTH_BRI,
        pIfe->IFE_Register_19.Bit.OUTLTH_DARK,
        pIfe->IFE_Register_19.Bit.OUTCNT_SEL
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;

    sprintf(SLOG_buf, "__align(4) IFE_RANGESET2 IQ_IFE_rngSet_%s=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  sOnlyEn, sOnlyLen, binn\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d\r\n",
        pIfe->IFE_Register_2.Bit.S_ONLY_EN,
        pIfe->IFE_Register_2.Bit.S_ONLY_LEN,
        pIfe->IFE_Register_2.Bit.BINNING
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) UINT32 IQ_IFE_weight_%s[6]=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  6 spatial weights\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,%d,%d,%d\r\n",
        pIfe->IFE_Register_2.Bit.S_WEIGHT0,
        pIfe->IFE_Register_2.Bit.S_WEIGHT1,
        pIfe->IFE_Register_2.Bit.S_WEIGHT2,
        pIfe->IFE_Register_2.Bit.S_WEIGHT3,
        pIfe->IFE_Register_2.Bit.S_WEIGHT4,
        pIfe->IFE_Register_2.Bit.S_WEIGHT5
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) UINT32 IQ_IFE_rngth_%s[2][2]=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  rth {g0,g1},{rb0,rb1}\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    {%d,%d},{%d,%d}\r\n",
        pIfe->IFE_Register_3.Bit.RTH_G0,
        pIfe->IFE_Register_3.Bit.RTH_G1,
        pIfe->IFE_Register_4.Bit.RTH_RB0,
        pIfe->IFE_Register_4.Bit.RTH_RB1
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) INT32 IQ_IFE_rdelta_%s[2][4]=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  rdelta {g0,g1,g2,g3},{rb0,rb1,rb2,rb3} : (6-bits signed)\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    {%d,%d,%d,%d},{%d,%d,%d,%d}\r\n",
        SIGN(pIfe->IFE_Register_15.Bit.RDLT_G0,6),
        SIGN(pIfe->IFE_Register_15.Bit.RDLT_G1,6),
        SIGN(pIfe->IFE_Register_15.Bit.RDLT_G2,6),
        SIGN(pIfe->IFE_Register_15.Bit.RDLT_G3,6),
        SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB0,6),
        SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB1,6),
        SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB2,6),
        SIGN(pIfe->IFE_Register_16.Bit.RDLT_RB3,6)
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) IFE_CLAMPWTSET IQ_IFE_clampWtSet_%s=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  clampTh, wtMul_g, wtMul_rb\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d\r\n",
        pIfe->IFE_Register_10.Bit.CLAMP_TH,
        pIfe->IFE_Register_10.Bit.CLAMPMUL_G,
        pIfe->IFE_Register_10.Bit.CLAMPMUL_RB
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    if((cnl==IQ_CNL_Cb)||(cnl==IQ_CNL_Cr))
    {    
    sprintf(SLOG_buf, "__align(4) IFE_CRVMAPSET IQ_IFE_crvmapSet_%s=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  crvmapDelta, crvmapInOffs, crvmapOutOffs, NULL\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "    %d,%d,%d,NULL\r\n",
        pIfe->IFE_Register_11.Bit.CRVMAP_DELTA,
        SIGN(pIfe->IFE_Register_11.Bit.CRVMAP_INOFFS,5),
        SIGN(pIfe->IFE_Register_11.Bit.CRVMAP_OUTOFFS,5)
        );SLOG_OUT;
    sprintf(SLOG_buf, "};\r\n\r\n");SLOG_OUT;
    
    sprintf(SLOG_buf, "__align(4) UINT32 IQ_SRA_IFE_crvmapGamma_%s[65]=\r\n", cName);SLOG_OUT;
    sprintf(SLOG_buf, "{\r\n");SLOG_OUT;
    sprintf(SLOG_buf, "//  65 curve mapping values\r\n");SLOG_OUT;
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
}


void IQSim_IFE_RawDummy(void)
{
    #ifdef SHIFT_IFE_RAW  
    IQ_BUF_CopyRect(&gBuf_IFE, 0, 0, &gBuf_SIE, IFE_X_OVERLAP, 0, gBuf_SIE.w, gBuf_SIE.h); //for IFE raw overlap, lineoft add 1 word.
    #else    
    IQ_BUF_CopyRect(&gBuf_IFE, 0, 0, &gBuf_SIE, 0, 0, gBuf_SIE.w, gBuf_SIE.h);
    #endif    
}

IQ_BUF ife_slice_up[3];
IQ_BUF ife_slice_in[3];
IQ_BUF ife_slice_down[3];
IQ_BUF ife_slice_out[3];
char fname[128];
int slice_id[3] = {0,0,0};

static UINT32 gIFE_raw_times = 0;
static UINT32 gIFE_raw_t = 0;

static UINT32 gIFE_ycc_times = 0;
static UINT32 gIFE_ycc_t = 0;
static UINT32 gIFE_ycc_h = 0;
static UINT32 gIFE_ycc_w = 0;

void IQSim_IFE_BufBackup(IQ_CNL cnl, SLICE_CTRL* pSliceCtrl);
void IQSim_IFE_BufUpdate(IQ_CNL cnl, SLICE_CTRL* pSliceCtrl);

void IQSim_IFE_BufBegin(IQ_CNL cnl, SLICE_CTRL* pSliceCtrl, UINT32 times)
{
    if(cnl == IQ_CNL_RAW)
    {
    gIFE_raw_times = times;
    gIFE_raw_t = 0;
    }
    else
    if((cnl == IQ_CNL_Y) || (cnl == IQ_CNL_Cb) || (cnl == IQ_CNL_Cr))
    {
#ifdef SHIFT_IFE_YCC   
    UINT32 ci = ci_table[cnl];
    IQ_BUF* ife_bufu = &ife_slice_up[ci];
    IQ_BUF* ife_buf1 = &ife_slice_in[ci];
    IQ_BUF* ife_bufd = &ife_slice_down[ci];
    IQ_BUF* ife_buf2 = &gBuf_IFE;
    IQ_BUF* ife_bufo = &ife_slice_out[ci];
    
    debug_ife2(("IFE cn[%d] times[%d]\r\n", ci, gIFE_ycc_times));
    gIFE_ycc_times = times;
    gIFE_ycc_t = 0;

    //up src
    if(pSliceCtrl->ife_id <= 0)
        ife_bufu = 0;
    else
        IQ_BUF_CloneByChannel(ife_bufu, &(gBuf_IME[pSliceCtrl->did]), cnl);
    
    //in src
    IQ_BUF_CloneByChannel(ife_buf1, &(gBuf_IME[pSliceCtrl->cid]), cnl);
    
    //down src
    if(pSliceCtrl->ife_id >= pSliceCtrl->total_cnt-1) 
        ife_bufd = 0;
    else
        IQ_BUF_CloneByChannel(ife_bufd, &(gBuf_IME[pSliceCtrl->uid]), cnl);
    
    //out dest -> up src
    IQ_BUF_CloneByChannel(ife_bufo, &(gBuf_IME[pSliceCtrl->did]), cnl);
    
    ///< image width
    gIFE_ycc_w = ife_buf1->w;
    ///< image height
    if(pSliceCtrl->ife_h + pSliceCtrl->slice_h > pSliceCtrl->total_h)
        gIFE_ycc_h = (pSliceCtrl->total_h - pSliceCtrl->ife_h); 
    else
        gIFE_ycc_h = pSliceCtrl->slice_h; 

    {
        UINT32 dest_x = 0;
        UINT32 dest_y = 0;
        UINT32 shift_x = gIFE_ycc_times*IFE_X_OVERLAP;
        UINT32 shift_y = gIFE_ycc_times*IFE_Y_OVERLAP;
        
        debug_ife2(("buf begin: ci,shift_x,shift_y=%d,%d,%d\r\n", ci, shift_x, shift_y));
        
        dest_x += shift_x;
        //copy up slice to IFE temp buf (last 2 lines)
        if(ife_bufu)
        IQ_BUF_CopyRect(ife_buf2, dest_x, dest_y,
            ife_bufu, 0, ife_bufu->h-shift_y, gIFE_ycc_w, shift_y);
        dest_y += shift_y;
        //copy in slice to IFE temp buf (all lines)
        if(ife_buf1)
        IQ_BUF_CopyRect(ife_buf2, dest_x, dest_y,
            ife_buf1, 0, 0, gIFE_ycc_w, gIFE_ycc_h);
        dest_y += gIFE_ycc_h;
        //copy down slice to IFE temp buf (first 2 lines)
        if(ife_bufd)
        IQ_BUF_CopyRect(ife_buf2, dest_x, dest_y,
            ife_bufd, 0, 0, gIFE_ycc_w, shift_y);
        dest_y += shift_y;
/*
        if(slice_id[ci] == 1)
        {
            if(ci==0)
                sprintf(fname, "A:\\IQSIM\\IFEy__%d", slice_id[ci]);
            if(ci==1)
                sprintf(fname, "A:\\IQSIM\\IFEcb_%d", slice_id[ci]);
            if(ci==2)
                sprintf(fname, "A:\\IQSIM\\IFEcr_%d", slice_id[ci]);
            IQ_BUF_Dump(ife_buf2, fname, 0, 0);
        }            
        slice_id[ci]++;
*/
    }
#endif  
    }
}

void IQSim_IFE_BufNext(IQ_CNL cnl)
{
    if(cnl == IQ_CNL_RAW)
    {
    gIFE_raw_t++;
    }
    else
    if((cnl == IQ_CNL_Y) || (cnl == IQ_CNL_Cb) || (cnl == IQ_CNL_Cr))
    {
    gIFE_ycc_t++;
    }
}

void IQSim_IFE_BufEnd(IQ_CNL cnl, SLICE_CTRL* pSliceCtrl)
{
    if(cnl == IQ_CNL_RAW)
    {
    }
    else
    if((cnl == IQ_CNL_Y) || (cnl == IQ_CNL_Cb) || (cnl == IQ_CNL_Cr))
    {
#ifdef SHIFT_IFE_YCC  
    UINT32 ci = ci_table[cnl];
    //IQ_BUF* ife_buf1 = &ife_slice_in[ci];
    IQ_BUF* ife_buf1 = &ife_slice_out[ci];
    IQ_BUF* ife_buf2 = &gBuf_IFE;
    //copy IFE temp buf to in slice (all lines)
    {
        UINT32 dest_x = 0;
        UINT32 dest_y = 0;
        UINT32 shift_x = 0;
        UINT32 shift_y = gIFE_ycc_times*IFE_Y_OVERLAP;
        dest_x += shift_x;
        dest_y += shift_y;
        IQ_BUF_CopyRect(ife_buf1, 0, 0,
            ife_buf2, dest_x, dest_y, gIFE_ycc_w, gIFE_ycc_h);
        debug_ife2(("buf end: ci,shift_x,shift_y=%d,%d,%d\r\n", ci, shift_x, shift_y));
    }
#endif    
    }
}

void IQSim_IFE_SetSrc(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, SLICE_CTRL* pSliceCtrl)
{
    if(cnl == IQ_CNL_RAW)
    {
        ///< source format select
        ifeParam->srcFmt = IFE_FMT_RAW;
        ///< CFA pattern
        ifeParam->cfaPat = gRAW_cfa;
        
        ///< image width
        ifeParam->width = gBuf_SIE.w; 
        ///< image height
        ifeParam->height = gBuf_SIE.h;       
        ///< image input lineoffset
        ifeParam->ofsi = gBuf_SIE.lineoff;                
        ///< input starting address of upper source
        ifeParam->up_addr = 0;  
        ///< input starting address
#ifdef SHIFT_IFE_RAW   
        //debug_err(("IFE raw src=%d\r\n", (gIFE_raw_times-gIFE_raw_t)));
        ifeParam->in_addr = gBuf_SIE.addr[0] + (gIFE_raw_times-gIFE_raw_t)*IFE_X_OVERLAP;   //for IFE raw overlap, in addr must shift right 1 word   
#else
        ifeParam->in_addr = gBuf_SIE.addr[0];
#endif
        ///< input starting address of down source
        ifeParam->down_addr = 0;   
    }
    else
    {   
        /*
        IQ_BUF gBuf_IME_uid, gBuf_IME_cid, gBuf_IME_did;
        //up buf = (last 2 lines of channel cnl) 
        IQ_BUF_CloneEx(&gBuf_IME_uid, &gBuf_IME[gSliceCtrl->uid], 0, gBuf_IME[gSliceCtrl->uid].h-IFE_Y_OVERLAP, gBuf_IME[gSliceCtrl->uid].w, 2, cnl);
        //current buf = (all of channel cnl) 
        IQ_BUF_CloneEx(&gBuf_IME_cid, &gBuf_IME[gSliceCtrl->cid], 0, 0, gBuf_IME[gSliceCtrl->cid].w, gBuf_IME[gSliceCtrl->cid].h, cnl);
        //down buf = (first 2 lines of channel cnl) 
        IQ_BUF_CloneEx(&gBuf_IME_did, &gBuf_IME[gSliceCtrl->did], 0, 0, gBuf_IME[gSliceCtrl->did].w, 2, cnl);
        
        ///< source format select
        ifeParam->srcFmt = IFE_FMT_NORMAL;
        ///< CFA pattern
        ifeParam->cfaPat = IFE_CFA_R;  //don't care    
        
        ///< image width
        ifeParam->width = gBuf_IME_cid.w;   
        ///< image height
        ifeParam->height = gBuf_IME_cid.h;       
        ///< image input lineoffset
        ifeParam->ofsi = gBuf_IME_cid.lineoff;                
        ///< input starting address of upper source (last 2 lines)
        ifeParam->up_addr = gBuf_IME_uid.addr[0];
        ///< input starting address
        ifeParam->in_addr = gBuf_IME_cid.addr[0];    
        ///< input starting address of down source (first 2 lines)
        ifeParam->down_addr = gBuf_IME_did.addr[0]; 
        */
#ifdef SHIFT_IFE_YCC   
        //UINT32 ci = ci_table[cnl];
        //IQ_BUF* ife_buf1 = &ife_slice_in[ci];
        IQ_BUF* ife_buf2 = &gBuf_IFE;
        UINT32 prev_h = gIFE_ycc_t*IFE_Y_OVERLAP;
        UINT32 up_h = IFE_Y_OVERLAP;
        UINT32 in_h = gIFE_ycc_h + (gIFE_ycc_times-gIFE_ycc_t-1)*IFE_Y_OVERLAP*2;
        UINT32 down_h = IFE_Y_OVERLAP;
        UINT32 next_h = gIFE_ycc_t*IFE_Y_OVERLAP;

        ///< source format select
        ifeParam->srcFmt = IFE_FMT_NORMAL;
        ///< CFA pattern
        ifeParam->cfaPat = 0;   //don't care
        ///< image width
        ifeParam->width = gIFE_ycc_w;   
        ///< image input lineoffset
        ifeParam->ofsi = ife_buf2->lineoff; 
        ///< input starting address
        {
        UINT32 shift_x = (gIFE_ycc_times-gIFE_ycc_t)*IFE_X_OVERLAP;
        UINT32 shift_y = 0;

        if(pSliceCtrl->ife_id <= 0)
        {
            //extend blank
            prev_h = gIFE_ycc_times*IFE_Y_OVERLAP;
            //skip up src (out of image up boundary)
            up_h = 0;
            //skip up side data (out of image up boundary)
            in_h -= (gIFE_ycc_times-gIFE_ycc_t-1)*IFE_Y_OVERLAP;
        }
        if(pSliceCtrl->ife_id >= pSliceCtrl->total_cnt-1) 
        {
            //skip down side data (out of image down boundary)
            in_h -= (gIFE_ycc_times-gIFE_ycc_t-1)*IFE_Y_OVERLAP;
            //skip down src (out of image down boundary)
            down_h = 0;
            //extend blank
            next_h = gIFE_ycc_times*IFE_Y_OVERLAP;
        }
        
        //prev blank region (out of filter bank)
        shift_y += prev_h;
        
        //up src
        if(up_h)
            ifeParam->up_addr = IQ_BUF_GetPixelAddr(ife_buf2, shift_x, shift_y, IQ_CNL_Y);
        else
            ifeParam->up_addr = 0;
        shift_y += up_h;
        
        //in src
        ifeParam->in_addr = IQ_BUF_GetPixelAddr(ife_buf2, shift_x, shift_y, IQ_CNL_Y);
        shift_y += in_h;

        //down src
        if(down_h)
            ifeParam->down_addr = IQ_BUF_GetPixelAddr(ife_buf2, shift_x, shift_y, IQ_CNL_Y);
        else
            ifeParam->down_addr = 0;
        shift_y += down_h;
        
        //next blank region (out of filter bank)
        shift_y += next_h;
        
        ///< image height
        ifeParam->height = in_h;
        
        debug_ife2(("IFE filter y range[%d,%d,%d,%d,%d] sum=%d\r\n", 
            prev_h, up_h, in_h, down_h, next_h, (prev_h+up_h+in_h+down_h+next_h)));
        }
#else
        UINT32 ci = ci_table[cnl];
        UINT32 wShift = 0;
        UINT32 hShift = 0;
        UINT32 shift_y = 0;
        UINT32 shift_x = 0;
        if((gBuf_IME[pSliceCtrl->cid].fmt == IQ_FMT_YCC422) && (ci>0))
        {
            wShift = 1; //CbCr: subsample H
        }
        if((gBuf_IME[pSliceCtrl->cid].fmt == IQ_FMT_YCC420) && (ci>0))
        {
            wShift = 1; hShift = 1;  //CbCr: subsample H & V
        }
        shift_x = IFE_X_OVERLAP;
        
        ///< source format select
        ifeParam->srcFmt = IFE_FMT_NORMAL;
        ///< CFA pattern
        ifeParam->cfaPat = 0;   //don't care
        
        ///< image width
        ifeParam->width = gBuf_IME[pSliceCtrl->cid].w >> wShift;   
        ///< image height
        if(pSliceCtrl->ife_h + pSliceCtrl->slice_h > pSliceCtrl->total_h)
            ifeParam->height = (pSliceCtrl->total_h - pSliceCtrl->ife_h) >> hShift; 
        else
            ifeParam->height = pSliceCtrl->slice_h >> hShift; 
        shift_y = (pSliceCtrl->slice_h >> hShift)-IFE_Y_OVERLAP;
        
        ///< image input lineoffset
        ifeParam->ofsi = gBuf_IME[pSliceCtrl->cid].lineoff >> wShift;                
        
        ///< input starting address of upper source (last 2 lines)
        ifeParam->up_addr = gBuf_IME[pSliceCtrl->did].addr[ci] + shift_x + shift_y*ifeParam->ofsi; 
        if(pSliceCtrl->ife_id >= pSliceCtrl->total_cnt-1) 
            ifeParam->up_addr = 0x0; //last id, cannot define its did, mark address as 0
            
        ///< input starting address
        ifeParam->in_addr = gBuf_IME[pSliceCtrl->cid].addr[ci] + shift_x;  //for IFE ycc overlap, in addr need shift right 4 pixels
        
        ///< input starting address of down source (first 2 lines)
        ifeParam->down_addr = gBuf_IME[pSliceCtrl->uid].addr[ci] + shift_x;  
        if(pSliceCtrl->ife_id == 0)
            ifeParam->down_addr = 0x0; //first id, cannot define its uid, mark address as 0
#endif            
    }   
    ///< enable of upper source
    if(ifeParam->up_addr == 0x0)
        ifeParam->upSrcEn = DISABLE;
    else
        ifeParam->upSrcEn = ENABLE;
    ///< enable of down source
    if(ifeParam->down_addr == 0x0)
        ifeParam->downSrcEn = DISABLE;
    else
        ifeParam->downSrcEn = ENABLE;
}
void IQSim_IFE_SetDest(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, SLICE_CTRL* pSliceCtrl)
{
    if(cnl == IQ_CNL_RAW)
    {
        ///< output starting address
#ifdef SHIFT_IFE_RAW   
        //debug_err(("IFE raw dest=%d\r\n", (gIFE_raw_times-gIFE_raw_t-1)));
        ifeParam->out_addr = gBuf_SIE.addr[0] + (gIFE_raw_times-gIFE_raw_t-1)*IFE_X_OVERLAP;
#else
        ifeParam->out_addr = gBuf_SIE.addr[0]; //for IFE ycc overlap, outaddr must equal to in_addr  
#endif
        ///< image output lineoffset
        ifeParam->ofso = gBuf_SIE.lineoff;                
    }
    else
    {
#ifdef SHIFT_IFE_YCC   
        IQ_BUF* ife_buf2 = &gBuf_IFE;
        ///< image output lineoffset
        ifeParam->ofso = ife_buf2->lineoff;  
        ///< output starting address
        {
        ifeParam->out_addr = ifeParam->in_addr - IFE_X_OVERLAP; //for IFE ycc overlap
        }
#else        
        UINT32 ci = ci_table[cnl];
        UINT32 wShift = 0;
        UINT32 hShift = 0;
        if((gBuf_IME[pSliceCtrl->cid].fmt == IQ_FMT_YCC422) && (ci>0))
        {
            wShift = 1; //CbCr: subsample H
        }
        if((gBuf_IME[pSliceCtrl->cid].fmt == IQ_FMT_YCC420) && (ci>0))
        {
            wShift = 1; hShift = 1;  //CbCr: subsample H & V
        }
        
        ///< output starting address
        ifeParam->out_addr = gBuf_IME[pSliceCtrl->cid].addr[ci]; //for IFE ycc overlap, out addr not shift 
        ///< image output lineoffset
        ifeParam->ofso = gBuf_IME[pSliceCtrl->cid].lineoff >> wShift;  
#endif        
    }   
}


//////////////////////////////////////////////////////////////////

void IQSim_IFE_Open(void)
{
    IFE_OPENOBJ IFEOpenObj;
    IFEOpenObj.FP_IFEISR_CB = NULL;
    ife_open(&IFEOpenObj);
}    
void IQSim_IFE_Stop(void)
{
}
void IQSim_IFE_Close(void)
{
    ife_close();
}

BOOL bIfeStart = FALSE;
BOOL bIfe2Start = FALSE;

void IQSim_IFE_Start(IFE_PARAM* ifeParam, int IQSimMode, _IFE_MODE mode)
{
    ///< operation mode
    ifeParam->mode = mode;  
    ///< break point 1
    ifeParam->bp1 = 0;    
    ///< interrupt enable
    ifeParam->intrEn = IFE_INTE_FRMEND; 

    if(ifeParam->crvmapEn)
    {
        sra_setGammaTable(SRA_READBY_IFE, ifeParam->crvmapSet.crvmapGamma);
    }        
    if(ifeParam->srcFmt == IFE_FMT_RAW)
    {
        debug_ife(("IFE in w,h,fmt=%d,%d,%d\r\n", 
            ifeParam->width,
            ifeParam->height,
            ifeParam->srcFmt
            ));
        debug_ife(("IFE in lineoff=%d\r\n", 
            ifeParam->ofsi
            ));
        debug_ife(("IFE in addr=%08x\r\n", 
            ifeParam->in_addr
            ));
        debug_ife(("IFE out lineoff=%d\r\n", 
            ifeParam->ofso
            ));
        debug_ife(("IFE out addr=%08x\r\n", 
            ifeParam->out_addr
            ));
        debug_ife(("IFE start\r\n"));
        ife_setMode(ifeParam, IFE_NOWAIT);//_ife_waitmode is don't care when embedded
        bIfeStart = TRUE;
    }        
    else
    {
        debug_ife2(("IFE2 in w,h,fmt=%d,%d,%d\r\n", 
            ifeParam->width,
            ifeParam->height,
            ifeParam->srcFmt
            ));
        debug_ife2(("IFE2 in lineoff=%d\r\n", 
            ifeParam->ofsi
            ));
        debug_ife2(("IFE2 in addr[3]=%08x,%08x,%08x\r\n", 
            ifeParam->up_addr,
            ifeParam->in_addr,
            ifeParam->down_addr
            ));
        debug_ife2(("IFE2 out lineoff=%d\r\n", 
            ifeParam->ofso
            ));
        debug_ife2(("IFE2 out addr=%08x\r\n", 
            ifeParam->out_addr
            ));
        debug_ife2(("IFE2 start\r\n"));
        ife_setMode(ifeParam, IFE_NOWAIT);//_ife_waitmode is don't care when embedded
        bIfe2Start = TRUE;
    }     
}
UINT32 IQSim_IFE_Wait(void)
{
    if(bIfeStart)
    {
        debug_ife(("IFE wait\r\n"));
        ife_waitFlagFrameEnd();
        debug_ife(("IFE finish\r\n"));
        bIfeStart = FALSE;
    }  
    else if(bIfe2Start)
    {
        debug_ife2(("IFE2 wait\r\n"));
        ife_waitFlagFrameEnd();
        debug_ife2(("IFE2 finish\r\n"));
        bIfe2Start = FALSE;
    }     
    else
        return 0;
    return 1;
}

//////////////////////////////////////////////////////////////////
IQ_BUF gBuf_IFE = {0}; //ife temporal slice for ycc process

void IQSim_IFE_NewBuffer(UINT32 w, UINT32 h)
{
#ifdef SHIFT_IFE_YCC
    //if(imeParam->op_mode == IPE2IME_SLICE_MODE)
    {
        UINT32 buf_w = 0;
        UINT32 buf_h = 0;
        IQ_BUF_FMT buf_fmt = 0;
        //IQ_BUF_FMT fmt_lut[5] = {IQ_FMT_YCC444, IQ_FMT_YCC422, IQ_FMT_YCC422, IQ_FMT_YCC420, IQ_FMT_YCC420};
        //output height (slice)
        buf_h = Cap_SliceMode_GetSliceHeight();
        //output width (slice)
        buf_w = w;
        //output format (slice)
        buf_fmt = IQ_FMT_Y8;
        //alloc buffer (slice)
        IQ_BUF_New(&gBuf_IFE, buf_fmt, 
            //for IFE ycc process, w+IFE_X_OVERLAP for each pass
            buf_w + IFE_YCC_TIMES_MAX*IFE_X_OVERLAP,
            //for IFE ycc process, h+IFE_Y_OVERLAP for each pass, 2 is prepare for both up side & down side
            buf_h + IFE_YCC_TIMES_MAX*IFE_Y_OVERLAP*2, 
            0
            ); 
    }
#endif
}

void IQSim_IFEraw_DumpSetting(int ti)
{
    if(ti == 0)
    {
        memcpy(&dumpBin.IfeRaw, (void*)IQSim_IFE_ADDR, sizeof(NT96220_IFE_REG_STRUCT));
    }
    else if((ti == 1) || (ti == 2))
    {
        UINT32* pDst = 0;
        volatile UINT32* pSrc = (volatile UINT32*)IQSim_IFE_ADDR;
        if(ti == 1)
        {
            pDst = (UINT32*)(&dumpBin.IfeExt);
        }
        if(ti == 2)
        {
            pDst = (UINT32*)(((char*)&dumpBin.IfeExt)+0x001c);
        }
        {
            //arrange current state to new struct
            UINT32 FILTER_EN = (pSrc[0x0004/4] & 1L<<6)>>6;
            UINT32 OUTL_EN = (pSrc[0x0004/4] & 1L<<5)>>5;
            pDst[0x0000/4] = pSrc[0x0008/4]; //0x0000 <- 0x0008
            pDst[0x0004/4] = pSrc[0x000c/4]; //0x0004 <- 0x000c
            pDst[0x0008/4] = pSrc[0x0010/4]; //0x0008 <- 0x0010
            pDst[0x000c/4] = pSrc[0x003c/4]; //0x000c <- 0x003c
            pDst[0x0010/4] = pSrc[0x0040/4]; //0x0010 <- 0x0040
            pDst[0x0014/4] = pSrc[0x004c/4]; //0x0014 <- 0x004c
            pDst[0x0018/4] = pSrc[0x0028/4]; //0x0018 <- 0x0028
            pDst[0x0000/4] |= ((FILTER_EN<<30) | (OUTL_EN<<31));
        }
    }
}

void IQSim_IFEy_DumpSetting(void)
{
    memcpy(&dumpBin.IfeY, (void*)IQSim_IFE_ADDR, sizeof(NT96220_IFE_REG_STRUCT));
}

void IQSim_IFEcc_DumpSetting(void)
{
    memcpy(&dumpBin.IfeCC, (void*)IQSim_IFE_ADDR, sizeof(NT96220_IFE_REG_STRUCT));
    memcpy(&dumpBin.IfeCCCrv, (void*)(IQSim_SRA_ADDR), sizeof(NT96220_SRA_REG_STRUCT));
}

