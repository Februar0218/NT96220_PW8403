#include <string.h>
#include <stdio.h>
#include "Type.h"

#if OV5633_220
#include "IPP_OV5633_220_int.h"
#include "IQSetting_OV5633_220.h"   //#include local folder .h file
__align(4) IPE_ISO_PARAM  IPE_ISOParam[Total_ISONum] =
{
//                   NRSel,            FStabSel,            FDtabsel,                 EdgeSel,                     KerSel,            EStabSel
    {&IPE_NRParam[ISONUM0], IPE_fmStab[ISONUM0], IPE_fmDtab[ISONUM0], &IPE_EdgeParam[ISONUM0], IPE_edgeKernelDiv[ISONUM0], IPE_emStab[ISONUM0]},
    {&IPE_NRParam[ISONUM1], IPE_fmStab[ISONUM1], IPE_fmDtab[ISONUM1], &IPE_EdgeParam[ISONUM1], IPE_edgeKernelDiv[ISONUM1], IPE_emStab[ISONUM1]},
    {&IPE_NRParam[ISONUM2], IPE_fmStab[ISONUM2], IPE_fmDtab[ISONUM2], &IPE_EdgeParam[ISONUM2], IPE_edgeKernelDiv[ISONUM2], IPE_emStab[ISONUM2]},
    {&IPE_NRParam[ISONUM3], IPE_fmStab[ISONUM3], IPE_fmDtab[ISONUM3], &IPE_EdgeParam[ISONUM3], IPE_edgeKernelDiv[ISONUM3], IPE_emStab[ISONUM3]}
};

__align(4) STR_IPE_NOVANR_PARA IPE_NRParam[Total_ISONum]=
{
    // GBAL, Nsmar, Lpfw, Sumsel
    {     0,     4,    0,      0},
    {     0,     4,    0,      0},
    {     0,     8,    0,      0},
    {     0,     8,    0,      0}
};

__align(4) STR_IPE_EDGE_ENHMAP_PARA IPE_EdgeParam[Total_ISONum]=
{
    // EnhP, EnhN, EmapA, EmapB, EmapC, EmapD, EmapN, EmapM, EmapE
    {   128,  104,     11,     25, 200,   200,     1,      1,  580},//from yuppie
    {   128,  104,     11,     25, 200,   200,     1,      1,  580},
    {   106,   92,     14,     24, 116,   169,     1,      2,  369},
    {   135,  117,     11,     63, 158,   243,     1,      2,  443},
};

__align(4) STR_IPE_EDGE_ENHMAP_PARA IPE_EdgeParam_Sketch=
    {   128,  104,     8,     25, 200,   200,     1,      1,  580};


__align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1] = {
    //   P1,     N,     N1
    {    128,    64,   32},
    {    128,    64,   32},
    {    128,    64,   32},
    {    128,    64,   32},
};

__align(4) INT16 IPE_AFedgeKernelDiv[10]=
    {  -1,  -1,  -1,
       -1,   1,   2,
       -1,   2,   4,  4};

__align(4) INT16 IPE_edgeKernelDiv[Total_ISONum][10]=
{
    {-1, -3,  -4,
     -3, -1,   5,
     -4,  5,  28,  3},
    {-1, -3,  -4,
     -3, -1,   5,
     -4,  5,  28,  3},
    {-1, -3,  -4,
     -3,  1,   7,
     -4,  7,  12,  3},
    {-1, -3,  -4,
     -3,  1,   7,
     -4,  7,  12,  3}
};

 __align(4) UINT8 IPE_fmStab[Total_ISONum][16]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255}
};
 __align(4) UINT8 IPE_fmDtab[Total_ISONum][16]=
{
    {128,64,32,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {128,64,32,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {128,64,32,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {128,64,32,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

  __align(4) UINT8 IPE_emStab[Total_ISONum][16]=
 {
     {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
     {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
     {24,28,36,52,64,64,64,64,64,64,64,64,64,64,64,64},
     {24,28,36,52,64,64,64,64,64,64,64,64,64,64,64,64}
 };

__align(4) UINT CHR_Param[5][28];

//IFE Parameter-------------------------------------------------------
#define IFE_G 0
#define IFE_RB 1

UINT32 IFE_weight_emb[Total_ISONum][6]=
{
     {0,2,1,2,1,1},
     {0,2,1,2,1,1},
     {0,0,0,0,0,0},
     {0,0,0,0,0,0}
};

UINT32 IFE_rngth_emb[Total_ISONum][IFE_RB+1][2]=
{
    //rngth_g  rngth_rb
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{6, 8},{ 8,10}},
    {{8,10},{11,14}}
};

//rngth_g/rngth_rb rdelta
INT32 IFE_rdelta_emb[Total_ISONum][IFE_RB+1][4]=
{
    //rdelta_g  rdelta_rb
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}},
    {{2,2,2,0},{2,2,2,0}}
};

UINT32 IFE_clampWtSet_emb[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {   128,  32,   32},
    {     0,   0,    0},
    {     0,   0,    0},
    {     0,   0,    0}
};

UINT32 IFE_weight_raw[IFE_RAW_Param][Total_ISONum][6]=
{
    {
        {0,2,1,2,1,1},
        {0,2,1,2,1,1},
        {0,2,1,2,1,1},
        {0,2,1,2,1,1}
    },
    {
        {0,2,1,2,1,1},
        {0,2,1,2,1,1},
        {0,2,1,2,1,1},
        {4,2,1,2,1,1}
    },

};

UINT32 IFE_rngth_raw[IFE_RAW_Param][Total_ISONum][IFE_RB+1][2]=
{
        //rngth_g  rngth_rb
    {
        {{ 2, 4},{ 3, 5}},
        {{ 5, 6},{ 6, 8}},
        {{ 6,10},{ 9,13}},
        {{15,20},{20,26}}
    },
    {
        {{ 2, 4},{ 3, 5}},
        {{ 5, 6},{ 6, 8}},
        {{ 6,10},{ 9,13}},
        {{ 4, 6},{ 9,12}}
    },
};

//rngth_g/rngth_rb rdelta
INT32 IFE_rdelta_raw[Total_ISONum][IFE_RB+1][4]=
{
    //rdelta_g  rdelta_rb
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{0,1,2,1},{0,1,2,1}},
    {{0,-1,-3,-5},{0,-1,-3,-5}}
};

UINT32 IFE_clampWtSet_raw[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {   128,  32,   32},
    {     0,   0,    0},
    {   255,  64,   64},
    {   255,  64,   64}
};

 UINT32 IFE_weight_y[Total_ISONum][6]=
{
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1}
};

UINT32 IFE_rngth_y[Total_ISONum][2]=
{
    {0, 0},
    {3, 0},
    {9,11},
    {9,11}
};

INT32 IFE_rdelta_y[Total_ISONum][4]=
{
    {0,0,0,0},
    {0,0,0,0},
    {0,2,4,6},
    {0,2,4,6}
};

UINT32 IFE_clampWtSet_y[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {     0,    0,    0},
    {     0,    0,    0},
    {     255,    128,    128},
    {     255,    128,    128}
};


 UINT32 IFE_weight_ycc[Total_ISONum][6]=
{
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1}
};

UINT32 IFE_rngth_ycc[Total_ISONum][2]=
{
    {30, 42},
    {30, 42},
    {20, 36},
    {20, 36}
};

INT32 IFE_rdelta_ycc[Total_ISONum][4]=
{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};

UINT32 IFE_clampWtSet_ycc[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

CRVMAPSET IFE_crvmapSet[Total_ISONum]=
{
    {IFE_DELTA_1,0,0},
    {IFE_DELTA_1,0,0},
    {IFE_DELTA_1,0,0},
    {IFE_DELTA_1,0,0}
};

UINT32 crvmapGamma[65] = {
    96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
    112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
    128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
    160
};

UINT32 IFE_outlSet[IFE_RAW_Param][Total_ISONum][3]=
{
    {
        {255,255,IFE_EIGHT_NEIGHBOURS},
        {255,255,IFE_EIGHT_NEIGHBOURS},
        { 64, 64,IFE_EIGHT_NEIGHBOURS},
        { 64, 64,IFE_EIGHT_NEIGHBOURS}
    },
    {
        {255,255,IFE_EIGHT_NEIGHBOURS},
        {255,255,IFE_EIGHT_NEIGHBOURS},
        { 64, 64,IFE_EIGHT_NEIGHBOURS},
        { 16, 16,IFE_EIGHT_NEIGHBOURS}
    },
};

/**
    Capture IFE Flow
    IFE Embedded and IFE_Y and IFE_YCC are exclusive
    if d2d func enable,IFE_FILTER.repeat must be >= 1
*/
__align(4) BOOL IFE_Func[Total_ISONum][IFE_MAXMODE] =
{
    //IFE_RAW   IFE_YCC     IFE_Y       IFE_EMB
    {ENABLE,    ENABLE,     DISABLE,    DISABLE},
    {ENABLE,    ENABLE,     DISABLE,    DISABLE},
    {ENABLE,    ENABLE,     DISABLE,    DISABLE},
    {ENABLE,    ENABLE,     DISABLE,    DISABLE},
};

__align(4) IFE_FILTER IFE_param_embeded[Total_ISONum] =
{
    //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn  sOnlyLen       weights                  rngth_g                        rdelta_g                         rngth_rb                       rdelta_rb                         clampWtSet                   crvmapSet                crvmapGamma  outlSet               rowDefVIni,  rowDefTbl, repeat
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM0], IFE_rngth_emb[ISONUM0][IFE_G], IFE_rdelta_emb[ISONUM0][IFE_G], IFE_rngth_emb[ISONUM0][IFE_RB], IFE_rdelta_emb[ISONUM0][IFE_RB], IFE_clampWtSet_emb[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[0][ISONUM0],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM1], IFE_rngth_emb[ISONUM1][IFE_G], IFE_rdelta_emb[ISONUM1][IFE_G], IFE_rngth_emb[ISONUM1][IFE_RB], IFE_rdelta_emb[ISONUM1][IFE_RB], IFE_clampWtSet_emb[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[0][ISONUM1],       0,     NULL,      1},
    {DISABLE,    ENABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM2], IFE_rngth_emb[ISONUM2][IFE_G], IFE_rdelta_emb[ISONUM2][IFE_G], IFE_rngth_emb[ISONUM2][IFE_RB], IFE_rdelta_emb[ISONUM2][IFE_RB], IFE_clampWtSet_emb[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[0][ISONUM2],       0,     NULL,      1},
    {DISABLE,    ENABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM3], IFE_rngth_emb[ISONUM3][IFE_G], IFE_rdelta_emb[ISONUM3][IFE_G], IFE_rngth_emb[ISONUM3][IFE_RB], IFE_rdelta_emb[ISONUM3][IFE_RB], IFE_clampWtSet_emb[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[0][ISONUM3],       0,     NULL,      1},
};

/**
group have sequence order, must finish the repeat time of the group,then use next group param
repeat time ,0xff, as finish symbol
*/
__align(4) IFE_FILTER IFE_param_raw[Total_ISONum][IFE_RAW_Param+1] =
{
        //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn  sOnlyLen       weights                  rngth_g                        rdelta_g                         rngth_rb                       rdelta_rb                         clampWtSet                   crvmapSet                crvmapGamma  outlSet               rowDefVIni,  rowDefTbl, repeat
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[0][ISONUM0], IFE_rngth_raw[0][ISONUM0][IFE_G], IFE_rdelta_raw[ISONUM0][IFE_G], IFE_rngth_raw[0][ISONUM0][IFE_RB], IFE_rdelta_raw[ISONUM0][IFE_RB], IFE_clampWtSet_raw[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[0][ISONUM0],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[0][ISONUM1], IFE_rngth_raw[0][ISONUM1][IFE_G], IFE_rdelta_raw[ISONUM1][IFE_G], IFE_rngth_raw[0][ISONUM1][IFE_RB], IFE_rdelta_raw[ISONUM1][IFE_RB], IFE_clampWtSet_raw[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[0][ISONUM1],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,    ENABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[0][ISONUM2], IFE_rngth_raw[0][ISONUM2][IFE_G], IFE_rdelta_raw[ISONUM2][IFE_G], IFE_rngth_raw[0][ISONUM2][IFE_RB], IFE_rdelta_raw[ISONUM2][IFE_RB], IFE_clampWtSet_raw[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[0][ISONUM2],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,    ENABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[0][ISONUM3], IFE_rngth_raw[0][ISONUM3][IFE_G], IFE_rdelta_raw[ISONUM3][IFE_G], IFE_rngth_raw[0][ISONUM3][IFE_RB], IFE_rdelta_raw[ISONUM3][IFE_RB], IFE_clampWtSet_raw[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[0][ISONUM3],       0,     NULL,      1},
        {DISABLE,    ENABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[1][ISONUM3], IFE_rngth_raw[1][ISONUM3][IFE_G], IFE_rdelta_raw[ISONUM3][IFE_G], IFE_rngth_raw[1][ISONUM3][IFE_RB], IFE_rdelta_raw[ISONUM3][IFE_RB], IFE_clampWtSet_raw[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[1][ISONUM3],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
};

__align(4) IFE_FILTER IFE_param_y[Total_ISONum][IFE_Y_Param+1]=
{
        //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn       sOnlyLen           weights        rngth_g                 rdelta_g              rngth_rb                 rdelta_g           clampWtSet                crvmapSet            crvmapGamma    outlSet             rowDefVIni, rowDefTbl, repeat
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM0], IFE_rngth_y[ISONUM0], IFE_rdelta_y[ISONUM0], IFE_rngth_y[ISONUM0], IFE_rdelta_y[ISONUM0], IFE_clampWtSet_y[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[0][ISONUM0],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM1], IFE_rngth_y[ISONUM1], IFE_rdelta_y[ISONUM1], IFE_rngth_y[ISONUM1], IFE_rdelta_y[ISONUM1], IFE_clampWtSet_y[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[0][ISONUM1],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM2], IFE_rngth_y[ISONUM2], IFE_rdelta_y[ISONUM2], IFE_rngth_y[ISONUM2], IFE_rdelta_y[ISONUM2], IFE_clampWtSet_y[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[0][ISONUM2],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM3], IFE_rngth_y[ISONUM3], IFE_rdelta_y[ISONUM3], IFE_rngth_y[ISONUM3], IFE_rdelta_y[ISONUM3], IFE_clampWtSet_y[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[0][ISONUM3],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
};

__align(4) IFE_FILTER IFE_param_ycc[Total_ISONum][IFE_YCC_Param+1]=
{
        //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn       sOnlyLen           weights        rngth_g                 rdelta_g                 rngth_rb                   rdelta_g              clampWtSet                   crvmapSet            crvmapGamma    outlSet             rowDefVIni, rowDefTbl, repeat
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM0], IFE_rngth_ycc[ISONUM0], IFE_rdelta_ycc[ISONUM0], IFE_rngth_ycc[ISONUM0], IFE_rdelta_ycc[ISONUM0], IFE_clampWtSet_ycc[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[0][ISONUM0],       0,     NULL,      2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM1], IFE_rngth_ycc[ISONUM1], IFE_rdelta_ycc[ISONUM1], IFE_rngth_ycc[ISONUM1], IFE_rdelta_ycc[ISONUM1], IFE_clampWtSet_ycc[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[0][ISONUM1],       0,     NULL,      2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_clampWtSet_ycc[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[0][ISONUM2],       0,     NULL,      2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_clampWtSet_ycc[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[0][ISONUM3],       0,     NULL,      2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
};

//--------------------------------------------------------------------
__align(4)INT16 Indenity_CC[9] = {
    0x004C, 0x0096, 0x001D,
    0x07D5, 0x07AC, 0x0080,
    0x0080, 0x0795, 0x07EC
};

__align(4)INT16 Normal_CC[9] = {
    0x4c,       0x96,       0x1d,       
    0x79f,      0x6fe,      0x164,      
    0x141,      0x68a,      0x35,       
};

 __align(4) UINT8 IPE_hueTab[24]=
{
        108,118,128,128,100,80,
        60,100,114,128,128,128,
        128,114,100,100,100,100,
        100,100,100,100,100,100
};
//#NT#2011/08/08#Yuppie Lin -begin
//#NT# modify signed varible to INT32
 __align(4) INT32 IPE_satTab[24]=
{
        -10,0,10,10,10,10,
        10,0,-4,0,0,5,
        0,7,14,22,10,5,
        0,-5,-10,-10,-10,-5
};
 __align(4) INT32 IPE_intTab[24]=
{
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
};
//#NT#2011/08/08#Yuppie Lin -end
 __align(4) UINT8 IPE_ddsTab[8]=
{
     32,32,32,32,32,32,32,32
};

//--------------------------------------------------------------------
//gamma_mod4
__align(4)UINT32  SRA_SIE_GammaLUT[65]={
0, 5, 21, 32, 43, 55, 65, 73, 82, 88,
94, 100, 106, 110, 114, 119, 124, 130, 135, 139,
143, 147, 152, 157, 161, 165, 169, 172, 176, 180,
183, 186, 189, 192, 195, 197, 200, 203, 205, 207,
209, 211, 213, 215, 217, 219, 221, 222, 224, 226,
228, 230, 232, 234, 236, 238, 239, 241, 243, 245,
247, 249, 251, 253, 255,
};

__align(4)UINT32  SRA_SIE_GammaLUT_RockEffect[65]={
     0,   0,  0,  0,  0,  0,  0,  0,
     0,   0,  0,  0,  0,  0,  0, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255
};

__align(4)UINT32  SRA_SIE_GammaLUT_BeautyWhite[65]={
    0,  1,  3,  10, 16, 25, 38, 52,
    68, 86, 102,118,132,148,160,170,
    179,187,195,203,209,215,219,223,
    226,229,231,233,235,236,237,238,
    239,240,241,242,243,244,245,246,
    247,248,249,250,251,252,253,254,
    255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255
};

__align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1] = {
    {128, 128, 128, 128, 0,0},    //_IMGEffect_Off,
    {128, 128, 128,   0, 0, 0},    //_IMGEffect_BW,
    {108, 146, 128,   0, 0, 0},    //_IMGEffect_Sepia,
    {128, 128, 128, 128, 0,40},    //_IMGEffect_Vivid,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Neutral,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_LowSharping,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Fasion1,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Fasion2,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_CustomManual,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Red,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Green,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Blue,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Yellow,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Purple
};

UINT32 *SRA_SIE_GammaLUT_Using = SRA_SIE_GammaLUT;


__align(4) INT16 Saturation[Total_ISONum][Max_Saturation+1] = {
//_IMGSaturation_P1,    _IMGSaturation_Normal,  _IMGSaturation_N1
   0,                 0,                    0,
   0,                 0,                    0,
   0,                 0,                    0,
   0,                 0,                    0
};


//#NT#2011/08/22#Bowen Li -begin
//#NT#add image filter
__align(4) IPE_YCFIX_EFFECT_PARAM ImgFilter_YCFix[_IMGFilter_Max] =
{
    /*
    {YFix, CbFix, CrFix, YFixETH, CFixETH, {CFixYTh.th_low, CFixYTh.th_high}, {CFixCbTh.th_low, CFixCbTh.th_high}, {CFixCrTh.th_low, CFixCrTh.th_high}},
    */
    {0,     0,      0,      0,      0,      {0,  0},     {0,     0},     {0,    0}}, //_IMGFilter_Off,
    {128,   128,    128,    0,      0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_MonoChrome,
    {128,   100,    160,    0,      0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_Class,
    {128,   100,    215,    0,      0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_Cyborg,
    {128,   128,    128,    0,      0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_HighConstMono,
    {128,   156,    87,     0,      0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_XRay,
    {128,   128,    128,    0,      0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_Infrared,
    {0,     0,      0,      0,      0,      {0,  0},     {0,     0},     {0,    0}}, //_IMGFilter_PopArt,
    {0,     0,      0,      0,      0,      {0,  0},     {0,     0},     {0,    0}}, //_IMGFilter_DayDream,
    {192,   0,      0,      12,     0,      {0,  0},     {0,     0},     {0,    0}}, //_IMGFilter_ColorPencil,
    {255,   128,    128,    12,     0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_PencilSketch,
    {0,     128,    128,    12,     0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_ChalkSketch,
    {0,     128,    128,    12,     3,      {0,  0},     {0,     0},     {0,    0}}, //_IMGFilter_GrowingEdge,
    {128,   100,    160,    0,      0,      {0,  255},   {0,     255},   {0,    255}}, //_IMGFilter_RockMode,
//    {0,     128,    128,    0,      0,      {0,  255},   {105,   128},   {128,  151}}, //_IMGFilter_BeautyWhite,
    {0,     0,      0,      0,      0,      {0,  0},     {0,     0},     {0,    0}}, //_IMGFilter_BeautyWhite,
    {0,     0,      0,      0,      0,      {0,  0},     {0,     0},     {0,    0}}, //_IMGFilter_FancySpot,
};



__align(4) IPE_COLOR_EFFECT_PARAM ImgFilter_Color[_IMGFilter_Max] =
{
    /*
    {CbOfs, CrOfs, YCon, CCon, IntOfs, SatOfs},
    */
    {131, 131, 128, 128,   0,   18},//_IMGFilter_Off,
    {128, 128, 128,   0,   0,   0},//_IMGFilter_MonoChrome,
    {100, 160, 128,   0,   0,   0},//_IMGFilter_Class,
    {100, 215, 220,   0,   0,   0},//_IMGFilter_Cyborg,
    {128, 128, 192,   0, -64,   0},//_IMGFilter_HighConstMono,
    {156,  87, 128,   0,   0,   0},//_IMGFilter_XRay,
    {128, 128, 128,   0,   0,   0},//_IMGFilter_Infrared,
    {128, 128, 128, 128,   0,  50},//_IMGFilter_PopArt,
    {128, 128,  64, 128,  64, -20},//_IMGFilter_DayDream,
    {128, 128, 128, 128,   0,   0},//_IMGFilter_ColorPencil,
    {128, 128, 128,   0,   0,   0},//_IMGFilter_PencilSketch,
    {128, 128, 128,   0,   0,   0},//_IMGFilter_ChalkSketch,
    {128, 128, 128, 128,   0,   0},//_IMGFilter_GrowingEdge,
    {100, 160, 128,   0,   0,   0},//_IMGFilter_RockMode,
    {138, 118, 128, 128,   0,   0},//_IMGFilter_BeautyWhite,
    {128, 128, 128, 128,   0,   0},//_IMGFilter_FancySpot,
};



__align(4) IPE_EDGE_EFFECT_PARAM ImgFilter_Edge[_IMGFilter_Max] =
{
    /*
    {EInvP, EInvN},
    */
    {0, 0},//_IMGFilter_Off,
    {0, 0},//_IMGFilter_MonoChrome,
    {0, 0},//_IMGFilter_Class,
    {0, 0},//_IMGFilter_Cyborg,
    {0, 0},//_IMGFilter_HighConstMono,
    {0, 0},//_IMGFilter_XRay,
    {0, 0},//_IMGFilter_Infrared,
    {0, 0},//_IMGFilter_PopArt,
    {0, 0},//_IMGFilter_DayDream,
    {1, 0},//_IMGFilter_ColorPencil,
    {1, 0},//_IMGFilter_PencilSketch,
    {0, 1},//_IMGFilter_ChalkSketch,
    {0, 1},//_IMGFilter_GrowingEdge,
    {0, 0},//_IMGFilter_RockMode,
    {0, 0},//_IMGFilter_BeautyWhite,
    {0, 0},//_IMGFilter_FancySpot,
};

__align(4) IPE_INVERSE_EFFECT_PARAM ImgFilter_Inv[_IMGFilter_Max] =
{
    /*
    {Inv, HInv},
    */
    {0, 0},//_IMGFilter_Off,
    {0, 0},//_IMGFilter_MonoChrome,
    {0, 0},//_IMGFilter_Class,
    {0, 0},//_IMGFilter_Cyborg,
    {0, 0},//_IMGFilter_HighConstMono,
    {1, 0},//_IMGFilter_XRay,
    {0, 0},//_IMGFilter_Infrared,
    {0, 0},//_IMGFilter_PopArt,
    {0, 0},//_IMGFilter_DayDream,
    {0, 0},//_IMGFilter_ColorPencil,
    {0, 0},//_IMGFilter_PencilSketch,
    {0, 0},//_IMGFilter_ChalkSketch,
    {0, 0},//_IMGFilter_GrowingEdge,
    {0, 0},//_IMGFilter_RockMode,
    {0, 0},//_IMGFilter_BeautyWhite,
    {0, 0},//_IMGFilter_FancySpot,
};

__align(4) STR_COLOR_INFOR ImgFilter_ColorInfo[_IMGFilter_Max] =
{
    /*
    {ChR, ChGr, ChGb, ChB},
    */
    {256, 256, 256, 256},//_IMGFilter_Off,
    {256, 256, 256, 256},//_IMGFilter_MonoChrome,
    {256, 256, 256, 256},//_IMGFilter_Class,
    {256, 256, 256, 256},//_IMGFilter_Cyborg,
    {256, 256, 256, 256},//_IMGFilter_HighConstMono,
    {256, 256, 256, 256},//_IMGFilter_XRay,
    { 64, 384, 384, 16},//_IMGFilter_Infrared,
    {256, 256, 256, 256},//_IMGFilter_PopArt,
    {224, 256, 256, 288},//_IMGFilter_DayDream,
    {256, 256, 256, 256},//_IMGFilter_ColorPencil,
    {256, 256, 256, 256},//_IMGFilter_PencilSketch,
    {256, 256, 256, 256},//_IMGFilter_ChalkSketch,
    {256, 256, 256, 256},//_IMGFilter_GrowingEdge,
    {256, 256, 256, 256},//_IMGFilter_RockMode,
    {256, 256, 256, 256},//_IMGFilter_BeautyWhite,
    {256, 256, 256, 256},//_IMGFilter_FancySpot,
};


__align(4) UINT32 GrainNoise[_IMGFilter_Max][4] =
{
    //En, W0, W1, Val
    {0, 0,  0,  0},//_IMGFilter_Off,
    {0, 0,  0,  0},//_IMGFilter_MonoChrome,
    {0, 0,  0,  0},//_IMGFilter_Class,
    {0, 0,  0,  0},//_IMGFilter_Cyborg,
    {1, 0, 50, 10},//_IMGFilter_HighConstMono,
    {0, 0,  0,  0},//_IMGFilter_XRay,
    {0, 0,  0,  0},//_IMGFilter_Infrared,
    {0, 0,  0,  0},//_IMGFilter_PopArt,
    {0, 0,  0,  0},//_IMGFilter_DayDream,
    {0, 0,  0,  0},//_IMGFilter_ColorPencil,
    {0, 0,  0,  0},//_IMGFilter_PencilSketch,
    {0, 0,  0,  0},//_IMGFilter_ChalkSketch,
    {0, 0,  0,  0},//_IMGFilter_GrowingEdge,
    {0, 0,  0,  0},//_IMGFilter_RockMode,
    {0, 0,  0,  0},//_IMGFilter_BeautyWhite,
    {0, 0,  0,  0},//_IMGFilter_FancySpot,
};

__align(4) STR_IPE_COLOR_PARA_EXT ColorParamExt[_IMGFilter_Max] =
{
//  ColorGain_range, ColorGain_mask, Ccr_set, Cadj_lmfsel
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_Off,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_MonoChrome,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_Class,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_Cyborg,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_HighConstMono,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_XRay,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_Infrared,
    {FMT_2_8, 0xC0, FMT_2_8, LOWPASS},//_IMGFilter_PopArt,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_DayDream,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_ColorPencil,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_PencilSketch,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_ChalkSketch,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_GrowingEdge,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_RockMode,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_BeautyWhite,
    {FMT_2_8, 0xff, FMT_2_8, LOWPASS},//_IMGFilter_FancySpot,
};


__align(4) IMAGE_FILTER ImgFilterTab[_IMGFilter_Max] =
{
    {&ImgFilter_YCFix[0],   &ImgFilter_Edge[0],     &ImgFilter_Color[0],    &ImgFilter_Inv[0],      &ImgFilter_ColorInfo[0],    GrainNoise[0],      &ColorParamExt[0],      &SRA_SIE_GammaLUT[0],               0,                          0},//_IMGFilter_Off,
    {&ImgFilter_YCFix[1],   &ImgFilter_Edge[1],     &ImgFilter_Color[1],    &ImgFilter_Inv[1],      &ImgFilter_ColorInfo[1],    GrainNoise[1],      &ColorParamExt[1],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CTRL_EN)},//_IMGFilter_MonoChrome,
    {&ImgFilter_YCFix[2],   &ImgFilter_Edge[2],     &ImgFilter_Color[2],    &ImgFilter_Inv[2],      &ImgFilter_ColorInfo[2],    GrainNoise[2],      &ColorParamExt[2],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CTRL_EN)},//_IMGFilter_Class,
    {&ImgFilter_YCFix[3],   &ImgFilter_Edge[3],     &ImgFilter_Color[3],    &ImgFilter_Inv[3],      &ImgFilter_ColorInfo[3],    GrainNoise[3],      &ColorParamExt[3],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CTRL_EN)},//_IMGFilter_Cyborg,
    {&ImgFilter_YCFix[4],   &ImgFilter_Edge[4],     &ImgFilter_Color[4],    &ImgFilter_Inv[4],      &ImgFilter_ColorInfo[4],    GrainNoise[4],      &ColorParamExt[4],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CTRL_EN)},//_IMGFilter_HighConstMono,
    {&ImgFilter_YCFix[5],   &ImgFilter_Edge[5],     &ImgFilter_Color[5],    &ImgFilter_Inv[5],      &ImgFilter_ColorInfo[5],    GrainNoise[5],      &ColorParamExt[5],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CTRL_EN|IPE_CGAIN_EN)},//_IMGFilter_XRay,
    {&ImgFilter_YCFix[6],   &ImgFilter_Edge[6],     &ImgFilter_Color[6],    &ImgFilter_Inv[6],      &ImgFilter_ColorInfo[6],    GrainNoise[6],      &ColorParamExt[6],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CTRL_EN|IPE_CGAIN_EN)},//_IMGFilter_Infrared,
    {&ImgFilter_YCFix[7],   &ImgFilter_Edge[7],     &ImgFilter_Color[7],    &ImgFilter_Inv[7],      &ImgFilter_ColorInfo[7],    GrainNoise[7],      &ColorParamExt[7],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CTRL_EN|IPE_CGAIN_EN)},//_IMGFilter_PopArt,
    {&ImgFilter_YCFix[8],   &ImgFilter_Edge[8],     &ImgFilter_Color[8],    &ImgFilter_Inv[8],      &ImgFilter_ColorInfo[8],    GrainNoise[8],      &ColorParamExt[8],      &SRA_SIE_GammaLUT[0],               0,                          (IPE_CADJ_EN|IPE_CGAIN_EN|IPE_CTRL_EN|IPE_CADJ_YCON_EN|IPE_CADJ_CCON_EN)},//_IMGFilter_DayDream,
    {&ImgFilter_YCFix[9],   &ImgFilter_Edge[9],     &ImgFilter_Color[9],    &ImgFilter_Inv[9],      &ImgFilter_ColorInfo[9],    GrainNoise[9],      &ColorParamExt[9],      &SRA_SIE_GammaLUT[0],               &IPE_EdgeParam_Sketch,      (IPE_CADJ_EN|IPE_YCTH_EN|IPE_CTRL_EN)},//_IMGFilter_ColorPencil,
    {&ImgFilter_YCFix[10],  &ImgFilter_Edge[10],    &ImgFilter_Color[10],   &ImgFilter_Inv[10],     &ImgFilter_ColorInfo[10],   GrainNoise[10],     &ColorParamExt[10],     &SRA_SIE_GammaLUT[0],               &IPE_EdgeParam_Sketch,      (IPE_CADJ_EN|IPE_YCTH_EN|IPE_CTRL_EN)},//_IMGFilter_PencilSketch,
    {&ImgFilter_YCFix[11],  &ImgFilter_Edge[11],    &ImgFilter_Color[11],   &ImgFilter_Inv[11],     &ImgFilter_ColorInfo[11],   GrainNoise[11],     &ColorParamExt[11],     &SRA_SIE_GammaLUT[0],               &IPE_EdgeParam_Sketch,      (IPE_CADJ_EN|IPE_YCTH_EN|IPE_CTRL_EN)},//_IMGFilter_ChalkSketch,
    {&ImgFilter_YCFix[12],  &ImgFilter_Edge[12],    &ImgFilter_Color[12],   &ImgFilter_Inv[12],     &ImgFilter_ColorInfo[12],   GrainNoise[12],     &ColorParamExt[12],     &SRA_SIE_GammaLUT[0],               &IPE_EdgeParam_Sketch,      (IPE_CADJ_EN|IPE_YCTH_EN)},//_IMGFilter_GrowingEdge,
    {&ImgFilter_YCFix[13],  &ImgFilter_Edge[13],    &ImgFilter_Color[13],   &ImgFilter_Inv[13],     &ImgFilter_ColorInfo[13],   GrainNoise[13],     &ColorParamExt[13],     &SRA_SIE_GammaLUT_RockEffect[0],    0,                          (IPE_CADJ_EN|IPE_YCTH_EN)},//_IMGFilter_RockMode,
    {&ImgFilter_YCFix[14],  &ImgFilter_Edge[14],    &ImgFilter_Color[14],   &ImgFilter_Inv[14],     &ImgFilter_ColorInfo[14],   GrainNoise[14],     &ColorParamExt[14],     &SRA_SIE_GammaLUT_BeautyWhite[0],   0,                          (IPE_CADJ_EN|IPE_YCTH_EN)},//_IMGFilter_BeautyWhite,
    {&ImgFilter_YCFix[15],  &ImgFilter_Edge[15],    &ImgFilter_Color[15],   &ImgFilter_Inv[15],     &ImgFilter_ColorInfo[15],   GrainNoise[15],     &ColorParamExt[15],     &SRA_SIE_GammaLUT[0],               0,                          0},//_IMGFilter_FancySpot,
};
//#NT#2011/08/22#Bowen Li -end
#endif
