#include "Type.h"
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ife_lib.h"
#include "ime_lib.h"
#include "ImageQualitySim_int.h"

#define NORMAL_CC       0
#define INDENITY_CC     1

//#define IPE_CC_TYPE     NORMAL_CC
#define IPE_CC_TYPE     INDENITY_CC

//SIE Parameter-------------------------------------------------------
__align(4) STR_SIE_CG_INFOR SIE_ColorGain[Total_ParamNum]=
{
//  Gain0, Gain1, Gain2, Gain3, GainSel
    {538,256,256,524,SIECG_2_8},
    {256,256,256,256,SIECG_2_8}
};

__align(4) STR_SIE_COFS_INFOR SIE_ColorOffset[Total_ParamNum]=
{
//  Ofs0, Ofs1, Ofs2, Ofs3
    {0,0,0,0},
    {0,0,0,0}
};

__align(4) UINT32  _SRA_SIE_GammaLUT[Total_ParamNum][65]=
{
//  65 gamma values
    { 0,   4,   8,  12,  16,  20,  24,  28,  32,  36,
     40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
     80,  84,  88,  92,  96, 100, 104, 108, 112, 116,
    120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
    160, 164, 168, 172, 176, 180, 184, 188, 192, 196,
    200, 204, 208, 212, 216, 220, 224, 228, 232, 236,
    240, 244, 248, 252, 255},
    { 0,   4,   8,  12,  16,  20,  24,  28,  32,  36,
     40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
     80,  84,  88,  92,  96, 100, 104, 108, 112, 116,   
    120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
    160, 164, 168, 172, 176, 180, 184, 188, 192, 196,
    200, 204, 208, 212, 216, 220, 224, 228, 232, 236,
    240, 244, 248, 252, 255}
};
//IPE Parameter-------------------------------------------------------
__align(4) STR_COLOR_INFOR _IPE_ColorGain[Total_ParamNum]=
{
//  ChR, ChGr, ChGb, ChB
    {256,256,256,256},
    {256,256,256,256},
};

__align(4) STR_COLOR_INFOR _IPE_ColorOffset[Total_ParamNum]=
{
//  ChR, ChGr, ChGb, ChB
    {0,0,0,0},
    {0,0,0,0},
};
 

__align(4) INT16 _IPE_ColorMatrixParam[Total_ParamNum][9] = 
{
//  3x3 ccr-matrix values
    {
    #if (IPE_CC_TYPE == INDENITY_CC)
        0x004C, 0x0096, 0x001D,
        0x07D5, 0x07AC, 0x0080,
        0x0080, 0x0795, 0x07EC
    #else
        0x004C, 0x0096, 0x001D,
        0x07BD, 0x0700, 0x0144,
        0x0121, 0x06F7, 0x07E8
    #endif
    },
    {
        0x004C, 0x0096, 0x001D,
        0x07BD, 0x0700, 0x0144,
        0x0121, 0x06F7, 0x07E8
    }
};
 
__align(4) STR_IPE_NOVANR_PARA _IPE_NRParam[Total_ParamNum]=
{
//  GBAL, Nsmar, Lpfw, Sumsel
    {   0,  16,     4,      0},
    {   0,  16,     4,      0}
};
__align(4) STR_IPE_EDGE_ENHMAP_PARA _IPE_EdgeParam[Total_ParamNum]=
{
//  EnhP, EnhN, EmapA, EmapB, EmapC, EmapD, EmapN, EmapM, EmapE
    {    64,    64, 4,  200,  200,  200,  2,  4, 0},
    {    64,    64, 4,  200,  200,  200,  2,  4, 0}
};
__align(4) INT16 _IPE_EdgeKernelDiv[Total_ParamNum][10]=
{
//  3x3 edge-kernel and 1 edge-div
    {-1, -1,  -1,
     -1,  1,   2,
     -1,  2,   4,  4},
    {-1, -1,  -1,
     -1,  1,   2,
     -1,  2,   4,  4}
};

__align(4) UINT8 _IPE_emStab[Total_ParamNum][16]=
{
//  16 EStab values    
    {163,148,133,120,110,105,96,92,88,84,80,66,53,40,30,20},
    {108,107,106,105,104,102,100,98,96,88,80,72,64,48,32,16}
};
  
__align(4) UINT8 _IPE_fmStab[Total_ParamNum][16]=
{
//  16 FStab values    
    {255,128,64,32,16,8,4,0,0,0,0,0,0,16,32,128},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
 
__align(4) UINT8 _IPE_fmDtab[Total_ParamNum][16]=
{
//  16 FDtab values    
    {96,64,16,4,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
 
__align(4) UINT8 _IPE_hueTab[Total_ParamNum][24]=
{
//  24 hueTab values    
    {120,110,110,120,128,128,128,128,128,128,128,128,128,138,138,128,128,128,128,128,128,128,128,128},
    {128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128}
};
//#NT#2011/08/08#Yuppie Lin -begin
//#NT# modify signed varible to INT32
__align(4) INT32 _IPE_satTab[Total_ParamNum][24]=
{
//  24 satTab values    
    {20,10,0,0,0,0,0,0,0,0,0,0,5,10,15,20,25,20,15,10,5,0,0,10},
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

__align(4) INT32 _IPE_intTab[Total_ParamNum][24]=
{
//  24 intTab values    
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
//#NT#2011/08/08#Yuppie Lin -end
__align(4) UINT8 _IPE_ddsTab[Total_ParamNum][8]=
{
//  8 ddsTab values    
    { 0,8,16,32,32,32,32,32},
    { 0,8,16,32,32,32,32,32}
};

__align(4) IPE_COLOR_EFFECT_PARAM _IPE_ColorEffectParam[Total_ParamNum] = 
{
//  CbOfs, CrOfs, YCon, CCon, IntOfs, SatOfs
    {128, 128, 128, 128, 0, 0},
    {128, 128, 128, 128, 0, 0}
};

__align(4) IPE_INVERSE_EFFECT_PARAM _IPE_InvertEffectParam[Total_ParamNum] =
{
//  Inv, Hinv
    {0, 0},
    {0, 0}
};

__align(4) IPE_YCFIX_EFFECT_PARAM _IPE_YCFixEffectParam[Total_ParamNum] =
{
//  YFix, CbFix, CrFix, YFixEth, CFixEth, CFixYth.l, CFixYth.h, CFixCbth.l, CFixCbth.h, CFixCrth.l, CFixCrth.h
    {0,   0,   0,    0,    0, {0,  0}, {0,  0}, {0,  0}},
    {0,   0,   0,    0,    0, {0,  0}, {0,  0}, {0,  0}},
};

__align(4) IPE_EDGE_EFFECT_PARAM _IPE_EdgeEffectParam[Total_ParamNum] =
{
//  EinvP, EinvN
    {0, 0},
    {0, 0}
};

__align(4) STR_IPE_COLOR_PARA_EXT _IPE_ColorParamExt[Total_ParamNum] =
{
//  ColorGain_range, ColorGain_mask, Ccr_set, Cadj_lmfsel
    {FMT_2_8, 255, FMT_2_8, LOWPASS},
    {FMT_2_8, 255, FMT_2_8, LOWPASS}
};



//IFE Parameter-------------------------------------------------------
__align(4) UINT32 _IFE_rowdefTbl[Total_ParamNum][16] = 
{
    {0xffff,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0xffff,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

__align(4) IFE_OUTLSET _IFE_outlSet_raw[Total_ParamNum]=
{
//  outlBriTh, outlDarkTh, OutlOp
    {128,128,IFE_SEVEN_NEIGHBOURS},
    {128,128,IFE_SEVEN_NEIGHBOURS}
};

__align(4) IFE_RANGESET2 _IFE_rngSet_raw[Total_ParamNum]=
{
    //sOnlyEn, sOnlyLen, binn
    {ENABLE, IFE_SONLY_5x5, IFE_BINNING_DISABLE},
    {ENABLE, IFE_SONLY_5x5, IFE_BINNING_DISABLE}
};

__align(4) UINT32 _IFE_weight_raw[Total_ParamNum][6]=
{
//  6 spatial weight
    {2,1,0,0,0,0}, 
    {1,1,1,1,1,1}
};

__align(4) UINT32 _IFE_rngth_raw[Total_ParamNum][IFE_RB+1][2]=
{
//  rth {g0,g1},{rb0,rb1}
    {{9,11},{11,13}},
    {{9,11},{11,13}} 
};

__align(4) INT32 _IFE_rdelta_raw[Total_ParamNum][IFE_RB+1][4]=
{
//  rdelta {g0,g1,g2,g3},{rb0,rb1,rb2,rb3}
    {{0,2,4,6},{0,2,4,6}}, 
    {{0,2,4,6},{0,2,4,6}} 
};

__align(4) IFE_CLAMPWTSET _IFE_clampWtSet_raw[Total_ParamNum]=
{
//  clampTh, wtMul_g, wtMul_rb
    {0,0,0}, 
    {0,0,0}
};


__align(4) IFE_OUTLSET _IFE_outlSet_y[Total_ParamNum]=
{
//  outlBriTh, outlDarkTh, OutlOp
    {128,128,IFE_SEVEN_NEIGHBOURS},
    {128,128,IFE_SEVEN_NEIGHBOURS}
};

__align(4) IFE_RANGESET2 _IFE_rngSet_y[Total_ParamNum]=
{
    //sOnlyEn, sOnlyLen, binn
    {ENABLE, IFE_SONLY_5x5, IFE_BINNING_DISABLE},
    {ENABLE, IFE_SONLY_5x5, IFE_BINNING_DISABLE}
};

__align(4) UINT32 _IFE_weight_y[Total_ParamNum][6]=
{
//  6 spatial weight
    {4,4,3,2,1,0}, 
    {1,1,1,1,1,1}
};

__align(4) UINT32 _IFE_rngth_y[Total_ParamNum][IFE_RB+1][2]=
{
//  rth {g0,g1},{rb0,rb1}
    {{9,11},{11,13}},
    {{9,11},{11,13}} 
};

__align(4) INT32 _IFE_rdelta_y[Total_ParamNum][IFE_RB+1][4]=
{
//  rdelta {g0,g1,g2,g3},{rb0,rb1,rb2,rb3}
    {{0,2,4,6},{0,2,4,6}}, 
    {{0,2,4,6},{0,2,4,6}} 
};

__align(4) IFE_CLAMPWTSET _IFE_clampWtSet_y[Total_ParamNum]=
{
//  clampTh, wtMul_g, wtMul_rb
    {0,0,0}, 
    {0,0,0}
};

__align(4) IFE_OUTLSET _IFE_outlSet_cc[Total_ParamNum]=
{
//  outlBriTh, outlDarkTh, OutlOp
    {128,128,IFE_SEVEN_NEIGHBOURS},
    {128,128,IFE_SEVEN_NEIGHBOURS}
};

__align(4) IFE_RANGESET2 _IFE_rngSet_cc[Total_ParamNum]=
{
    //sOnlyEn, sOnlyLen, binn
    {ENABLE, IFE_SONLY_5x5, IFE_BINNING_DISABLE},
    {ENABLE, IFE_SONLY_5x5, IFE_BINNING_DISABLE}
};

__align(4) UINT32 _IFE_weight_cc[Total_ParamNum][6]=
{
//  6 spatial weight
    {4,4,3,2,1,0}, 
    {1,1,1,1,1,1}
};

__align(4) UINT32 _IFE_rngth_cc[Total_ParamNum][IFE_RB+1][2]=
{
//  rth {g0,g1},{rb0,rb1}
    {{9,11},{11,13}},
    {{9,11},{11,13}} 
};

__align(4) INT32 _IFE_rdelta_cc[Total_ParamNum][IFE_RB+1][4]=
{
//  rdelta {g0,g1,g2,g3},{rb0,rb1,rb2,rb3}
    {{0,2,4,6},{0,2,4,6}}, 
    {{0,2,4,6},{0,2,4,6}} 
};

__align(4) IFE_CLAMPWTSET _IFE_clampWtSet_cc[Total_ParamNum]=
{
//  clampTh, wtMul_g, wtMul_rb
    {0,0,0}, 
    {0,0,0}
};

__align(4) IFE_CRVMAPSET _IFE_crvmapSet_cc[Total_ParamNum]=
{
//  crvmapDelta, crvmapInOffs, crvmapOutOffs
    {IFE_DELTA_2,0,0,NULL},
    {IFE_DELTA_2,0,0,NULL}
};


__align(4) UINT32 _IFE_crvmapGamma_cc[Total_ParamNum][65] = 
{
//  65 curve mapping values
    { 0,   4,   8,  12,  16,  20,  24,  28,  32,  36,
     40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
     80,  84,  88,  92,  96, 100, 104, 108, 112, 116,
    120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
    160, 164, 168, 172, 176, 180, 184, 188, 192, 196,
    200, 204, 208, 212, 216, 220, 224, 228, 232, 236,
    240, 244, 248, 252, 255},
    { 0,   4,   8,  12,  16,  20,  24,  28,  32,  36,
     40,  44,  48,  52,  56,  60,  64,  68,  72,  76,
     80,  84,  88,  92,  96, 100, 104, 108, 112, 116,   
    120, 124, 128, 132, 136, 140, 144, 148, 152, 156,
    160, 164, 168, 172, 176, 180, 184, 188, 192, 196,
    200, 204, 208, 212, 216, 220, 224, 228, 232, 236,
    240, 244, 248, 252, 255}
};

//IME Parameter-------------------------------------------------------

//JPG Parameter-------------------------------------------------------

UINT32 JPG_quality = 95;


