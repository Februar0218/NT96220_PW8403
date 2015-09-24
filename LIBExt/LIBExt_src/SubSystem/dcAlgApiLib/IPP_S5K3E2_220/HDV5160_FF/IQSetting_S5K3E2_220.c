#include <string.h>
#include <stdio.h>
#include "Type.h"

#if S5K3E2_220
#include "IPP_S5K3E2_220_int.h"
#include "IQSetting_S5K3E2_220.h"   //#include local folder .h file
__align(4) IPE_ISO_PARAM  IPE_ISOParam[Total_ISONum] =
{
//                   NRSel,            FStabSel,            FDtabsel,                 EdgeSel,                     KerSel,            EStabSel
    {&IPE_NRParam[ISONUM0], IPE_fmStab[ISONUM0], IPE_fmDtab[ISONUM0], &IPE_EdgeParam[ISONUM0], IPE_edgeKernelDiv[ISONUM0], IPE_emStab[ISONUM0]},
    {&IPE_NRParam[ISONUM1], IPE_fmStab[ISONUM1], IPE_fmDtab[ISONUM1], &IPE_EdgeParam[ISONUM1], IPE_edgeKernelDiv[ISONUM1], IPE_emStab[ISONUM1]},
    {&IPE_NRParam[ISONUM2], IPE_fmStab[ISONUM2], IPE_fmDtab[ISONUM2], &IPE_EdgeParam[ISONUM2], IPE_edgeKernelDiv[ISONUM2], IPE_emStab[ISONUM2]},
    {&IPE_NRParam[ISONUM3], IPE_fmStab[ISONUM3], IPE_fmDtab[ISONUM3], &IPE_EdgeParam[ISONUM3], IPE_edgeKernelDiv[ISONUM3], IPE_emStab[ISONUM3]},
//capture
    {&IPE_NRParam[ISONUM4], IPE_fmStab[ISONUM4], IPE_fmDtab[ISONUM4], &IPE_EdgeParam[ISONUM4], IPE_edgeKernelDiv[ISONUM4], IPE_emStab[ISONUM4]},
    {&IPE_NRParam[ISONUM5], IPE_fmStab[ISONUM5], IPE_fmDtab[ISONUM5], &IPE_EdgeParam[ISONUM5], IPE_edgeKernelDiv[ISONUM5], IPE_emStab[ISONUM5]},
    {&IPE_NRParam[ISONUM6], IPE_fmStab[ISONUM6], IPE_fmDtab[ISONUM6], &IPE_EdgeParam[ISONUM6], IPE_edgeKernelDiv[ISONUM6], IPE_emStab[ISONUM6]},
    {&IPE_NRParam[ISONUM7], IPE_fmStab[ISONUM7], IPE_fmDtab[ISONUM7], &IPE_EdgeParam[ISONUM7], IPE_edgeKernelDiv[ISONUM7], IPE_emStab[ISONUM7]}
};

__align(4) STR_IPE_NOVANR_PARA IPE_NRParam[Total_ISONum]=
{
    // GBAL, Nsmar, Lpfw, Sumsel
    {     0,     4,    0,      0},
    {     0,     4,    0,      0},
    {     0,     4,    0,      0},
    {     0,     4,    0,      0},
//capure    
    {     0,     4,    0,      0},
    {     0,     4,    0,      0},
    {     0,     4,    0,      0},
    {     0,     8,    0,      0}
};

__align(4) STR_IPE_EDGE_ENHMAP_PARA IPE_EdgeParam[Total_ISONum]=
{
    // EnhP, EnhN, EmapA, EmapB, EmapC, EmapD, EmapN, EmapM, EmapE
    {   108,  94,    7,     15, 200,   200,     1,      1,  580},//from yuppie
    {   100,  90,   16,     22, 200,   200,     1,      1,  580},
    {   100,  90,   20,     36, 200,   200,     1,      1,  520},
    {   100,  90,   48,     48, 200,   200,     1,      1,  580},
//capture
    {   212,  200,    12,     16, 200,   200,     1,      1,  580},//from yuppie  < 50
    {   212,  200,    16,     18, 200,   200,     1,      1,  580},//                <100//{   132,  115,    24,     30, 200,   200,     1,      1,  580},//
    {   216,  200,    33,     40, 200,   200,     1,      1,  580},//                <200
    {   242,  220,    50,     50, 200,   200,     1,      1,  580},//                <400
};

__align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1] = {
    //   P1,     N,     N1
    {    32,    64,    128},
    {    32,    64,    128},
    {    32,    64,    128},
    {    32,    64,    128},
//capture
    {    32,    64,    128},
    {    32,    64,    128},
    {    32,    64,    128},
    {    32,    64,    128},
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
     -3, -1,   5,
     -4,  5,  28,  3},
    {-1, -3,  -4,
     -3, -1,   5,
     -4,  5,  28,  3},
//capture
    {-1, -3,  -4,
     -3, -1,   5,
     -4,  5,  28,  3},
    {-1, -3,  -4,
     -3, -1,   5,
     -4,  5,  28,  3},// 2
    {-1, -3,  -4,
     -3, -1,   5,
     -4,  5,  28,  3},
    {-1, -3,  -4,
     -3,  1,   7,
     -4,  7,  12,  3}
};

 __align(4) UINT8 IPE_fmStab[Total_ISONum][16]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
//capture
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255}
};
 __align(4) UINT8 IPE_fmDtab[Total_ISONum][16]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//capture
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

  __align(4) UINT8 IPE_emStab[Total_ISONum][16]=
 {
     {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
     {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
     {24,28,36,52,64,64,64,64,64,64,64,64,64,64,64,64},
     {24,28,36,52,64,64,64,64,64,64,64,64,64,64,64,64},
//capture
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
     {0,0,0,0,0,0},
//capture     
     {4,2,1,4,2,1},
     {4,2,1,4,2,1},
     {0,0,0,0,0,0},
     {0,0,0,0,0,0}
};

UINT32 IFE_rngth_emb[Total_ISONum][IFE_RB+1][2]=
{
    //rngth_g  rngth_rb
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{6, 8},{ 8,10}},
    {{8,10},{11,14}},
//capture
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{6, 8},{ 8,10}},
    {{8,10},{11,14}},
};

//rngth_g/rngth_rb rdelta
INT32 IFE_rdelta_emb[Total_ISONum][IFE_RB+1][4]=
{
    //rdelta_g  rdelta_rb
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}},
    {{2,2,2,0},{2,2,2,0}},
//capture
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}},
    {{2,2,2,0},{2,2,2,0}},
};

UINT32 IFE_clampWtSet_emb[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {   128,  32,   32},
    {     0,   0,    0},
    {     0,   0,    0},
    {     0,   0,    0},
//capture
    {   128,  32,   32},
    {     0,   0,    0},
    {     0,   0,    0},
    {     0,   0,    0}
};

UINT32 IFE_weight_raw[Total_ISONum][6]=
{
     {0,2,1,2,1,1},
     {0,2,1,2,1,1},
     {0,0,0,0,0,0},
     {0,0,0,0,0,0},
//capture
     {4,2,1,4,2,1},
     {4,2,1,4,2,1},
     {0,0,0,0,0,0},
     {0,0,0,0,0,0}
};

UINT32 IFE_rngth_raw[Total_ISONum][IFE_RB+1][2]=
{
    //rngth_g  rngth_rb
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{6, 8},{ 8,10}},
    {{8,10},{11,14}},
//capture
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{6, 8},{ 8,10}},
    {{8,10},{11,14}},
};

//rngth_g/rngth_rb rdelta
INT32 IFE_rdelta_raw[Total_ISONum][IFE_RB+1][4]=
{
    //rdelta_g  rdelta_rb
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}},
    {{2,2,2,0},{2,2,2,0}},
//capture
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}},
    {{2,2,2,0},{2,2,2,0}}
};

UINT32 IFE_clampWtSet_raw[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {   128,  32,   32},
    {     0,   0,    0},
    {     0,   0,    0},
    {     0,   0,    0},
//capture
    {   128,  32,   32},
    {     0,   0,    0},
    {     0,   0,    0},
    {     0,   0,    0}
};

 UINT32 IFE_weight_y[Total_ISONum][6]=
{
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
//capture
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
    {9,11},
//capture
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
    {0,2,4,6},
//capture
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
    {     255,    128,    128},
//capture
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
     {1,1,1,1,1,1},
//capture
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1},
     {1,1,1,1,1,1}
};

UINT32 IFE_rngth_ycc[Total_ISONum][2]=
{
    {30, 42},
    {30, 42},
    {30, 42},
    {32, 40},
//capture
    {30, 42},
    {30, 42},
    {30, 42},
    {32, 40}
};

INT32 IFE_rdelta_ycc[Total_ISONum][4]=
{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
//capture
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
    {255,0,0},
//capture
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {255,0,0}
};

CRVMAPSET IFE_crvmapSet[Total_ISONum]=
{
    {IFE_DELTA_1,0,0},
    {IFE_DELTA_1,0,0},
    {IFE_DELTA_1,0,0},
    {IFE_DELTA_1,0,0},
//capture
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

UINT32 IFE_outlSet[Total_ISONum][3]=
{
    {255,255,IFE_EIGHT_NEIGHBOURS},
    {255,255,IFE_EIGHT_NEIGHBOURS},
    {255,255,IFE_EIGHT_NEIGHBOURS},
    {255,255,IFE_EIGHT_NEIGHBOURS},
//capture
    {255,255,IFE_EIGHT_NEIGHBOURS},
    {255,255,IFE_EIGHT_NEIGHBOURS},
    {255,255,IFE_EIGHT_NEIGHBOURS},
    {255,255,IFE_EIGHT_NEIGHBOURS}
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
//capture
    {DISABLE,  DISABLE,     DISABLE,    DISABLE},
    {DISABLE,  DISABLE,     DISABLE,    DISABLE},
    {ENABLE,    ENABLE,     DISABLE,    DISABLE},
    {ENABLE,    ENABLE,     DISABLE,    DISABLE},
};

__align(4) IFE_FILTER IFE_param_embeded[Total_ISONum] =
{
    //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn  sOnlyLen       weights                  rngth_g                        rdelta_g                         rngth_rb                       rdelta_g                         clampWtSet                   crvmapSet                crvmapGamma  outlSet               rowDefVIni,  rowDefTbl, repeat
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM0], IFE_rngth_emb[ISONUM0][IFE_G], IFE_rdelta_emb[ISONUM0][IFE_G], IFE_rngth_emb[ISONUM0][IFE_RB], IFE_rdelta_emb[ISONUM0][IFE_RB], IFE_clampWtSet_emb[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[ISONUM0],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM1], IFE_rngth_emb[ISONUM1][IFE_G], IFE_rdelta_emb[ISONUM1][IFE_G], IFE_rngth_emb[ISONUM1][IFE_RB], IFE_rdelta_emb[ISONUM1][IFE_RB], IFE_clampWtSet_emb[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[ISONUM1],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM2], IFE_rngth_emb[ISONUM2][IFE_G], IFE_rdelta_emb[ISONUM2][IFE_G], IFE_rngth_emb[ISONUM2][IFE_RB], IFE_rdelta_emb[ISONUM2][IFE_RB], IFE_clampWtSet_emb[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[ISONUM2],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM3], IFE_rngth_emb[ISONUM3][IFE_G], IFE_rdelta_emb[ISONUM3][IFE_G], IFE_rngth_emb[ISONUM3][IFE_RB], IFE_rdelta_emb[ISONUM3][IFE_RB], IFE_clampWtSet_emb[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[ISONUM3],       0,     NULL,      1},
//capture
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM4], IFE_rngth_emb[ISONUM4][IFE_G], IFE_rdelta_emb[ISONUM4][IFE_G], IFE_rngth_emb[ISONUM4][IFE_RB], IFE_rdelta_emb[ISONUM4][IFE_RB], IFE_clampWtSet_emb[ISONUM4], &IFE_crvmapSet[ISONUM4], crvmapGamma, IFE_outlSet[ISONUM4],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM5], IFE_rngth_emb[ISONUM5][IFE_G], IFE_rdelta_emb[ISONUM5][IFE_G], IFE_rngth_emb[ISONUM5][IFE_RB], IFE_rdelta_emb[ISONUM5][IFE_RB], IFE_clampWtSet_emb[ISONUM5], &IFE_crvmapSet[ISONUM5], crvmapGamma, IFE_outlSet[ISONUM5],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM6], IFE_rngth_emb[ISONUM6][IFE_G], IFE_rdelta_emb[ISONUM6][IFE_G], IFE_rngth_emb[ISONUM6][IFE_RB], IFE_rdelta_emb[ISONUM6][IFE_RB], IFE_clampWtSet_emb[ISONUM6], &IFE_crvmapSet[ISONUM6], crvmapGamma, IFE_outlSet[ISONUM6],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM7], IFE_rngth_emb[ISONUM7][IFE_G], IFE_rdelta_emb[ISONUM7][IFE_G], IFE_rngth_emb[ISONUM7][IFE_RB], IFE_rdelta_emb[ISONUM7][IFE_RB], IFE_clampWtSet_emb[ISONUM7], &IFE_crvmapSet[ISONUM7], crvmapGamma, IFE_outlSet[ISONUM7],       0,     NULL,      1},
};

/**
group have sequence order, must finish the repeat time of the group,then use next group param
repeat time ,0xff, as finish symbol
*/
__align(4) IFE_FILTER IFE_param_raw[Total_ISONum][IFE_RAW_Param+1] =
{
        //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn  sOnlyLen       weights                  rngth_g                        rdelta_g                         rngth_rb                       rdelta_g                         clampWtSet                   crvmapSet                crvmapGamma  outlSet               rowDefVIni,  rowDefTbl, repeat
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM0], IFE_rngth_raw[ISONUM0][IFE_G], IFE_rdelta_raw[ISONUM0][IFE_G], IFE_rngth_raw[ISONUM0][IFE_RB], IFE_rdelta_raw[ISONUM0][IFE_RB], IFE_clampWtSet_raw[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[ISONUM0],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM1], IFE_rngth_raw[ISONUM1][IFE_G], IFE_rdelta_raw[ISONUM1][IFE_G], IFE_rngth_raw[ISONUM1][IFE_RB], IFE_rdelta_raw[ISONUM1][IFE_RB], IFE_clampWtSet_raw[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[ISONUM1],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM2], IFE_rngth_raw[ISONUM2][IFE_G], IFE_rdelta_raw[ISONUM2][IFE_G], IFE_rngth_raw[ISONUM2][IFE_RB], IFE_rdelta_raw[ISONUM2][IFE_RB], IFE_clampWtSet_raw[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[ISONUM2],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM2], IFE_rngth_raw[ISONUM2][IFE_G], IFE_rdelta_raw[ISONUM2][IFE_G], IFE_rngth_raw[ISONUM2][IFE_RB], IFE_rdelta_raw[ISONUM2][IFE_RB], IFE_clampWtSet_raw[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[ISONUM2],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM3], IFE_rngth_raw[ISONUM3][IFE_G], IFE_rdelta_raw[ISONUM3][IFE_G], IFE_rngth_raw[ISONUM3][IFE_RB], IFE_rdelta_raw[ISONUM3][IFE_RB], IFE_clampWtSet_raw[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[ISONUM3],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM3], IFE_rngth_raw[ISONUM3][IFE_G], IFE_rdelta_raw[ISONUM3][IFE_G], IFE_rngth_raw[ISONUM3][IFE_RB], IFE_rdelta_raw[ISONUM3][IFE_RB], IFE_clampWtSet_raw[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[ISONUM3],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
//capture
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM4], IFE_rngth_raw[ISONUM4][IFE_G], IFE_rdelta_raw[ISONUM4][IFE_G], IFE_rngth_raw[ISONUM4][IFE_RB], IFE_rdelta_raw[ISONUM4][IFE_RB], IFE_clampWtSet_raw[ISONUM4], &IFE_crvmapSet[ISONUM4], crvmapGamma, IFE_outlSet[ISONUM4],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM5], IFE_rngth_raw[ISONUM5][IFE_G], IFE_rdelta_raw[ISONUM5][IFE_G], IFE_rngth_raw[ISONUM5][IFE_RB], IFE_rdelta_raw[ISONUM5][IFE_RB], IFE_clampWtSet_raw[ISONUM5], &IFE_crvmapSet[ISONUM5], crvmapGamma, IFE_outlSet[ISONUM5],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM6], IFE_rngth_raw[ISONUM6][IFE_G], IFE_rdelta_raw[ISONUM6][IFE_G], IFE_rngth_raw[ISONUM6][IFE_RB], IFE_rdelta_raw[ISONUM6][IFE_RB], IFE_clampWtSet_raw[ISONUM6], &IFE_crvmapSet[ISONUM6], crvmapGamma, IFE_outlSet[ISONUM6],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM6], IFE_rngth_raw[ISONUM6][IFE_G], IFE_rdelta_raw[ISONUM6][IFE_G], IFE_rngth_raw[ISONUM6][IFE_RB], IFE_rdelta_raw[ISONUM6][IFE_RB], IFE_clampWtSet_raw[ISONUM6], &IFE_crvmapSet[ISONUM6], crvmapGamma, IFE_outlSet[ISONUM6],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM7], IFE_rngth_raw[ISONUM7][IFE_G], IFE_rdelta_raw[ISONUM7][IFE_G], IFE_rngth_raw[ISONUM7][IFE_RB], IFE_rdelta_raw[ISONUM7][IFE_RB], IFE_clampWtSet_raw[ISONUM7], &IFE_crvmapSet[ISONUM7], crvmapGamma, IFE_outlSet[ISONUM7],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[ISONUM7], IFE_rngth_raw[ISONUM7][IFE_G], IFE_rdelta_raw[ISONUM7][IFE_G], IFE_rngth_raw[ISONUM7][IFE_RB], IFE_rdelta_raw[ISONUM7][IFE_RB], IFE_clampWtSet_raw[ISONUM7], &IFE_crvmapSet[ISONUM7], crvmapGamma, IFE_outlSet[ISONUM7],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
};

__align(4) IFE_FILTER IFE_param_y[Total_ISONum][IFE_Y_Param+1]=
{
        //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn       sOnlyLen           weights        rngth_g                 rdelta_g              rngth_rb                 rdelta_g           clampWtSet                crvmapSet            crvmapGamma    outlSet             rowDefVIni, rowDefTbl, repeat
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM0], IFE_rngth_y[ISONUM0], IFE_rdelta_y[ISONUM0], IFE_rngth_y[ISONUM0], IFE_rdelta_y[ISONUM0], IFE_clampWtSet_y[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[ISONUM0],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM1], IFE_rngth_y[ISONUM1], IFE_rdelta_y[ISONUM1], IFE_rngth_y[ISONUM1], IFE_rdelta_y[ISONUM1], IFE_clampWtSet_y[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[ISONUM1],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM2], IFE_rngth_y[ISONUM2], IFE_rdelta_y[ISONUM2], IFE_rngth_y[ISONUM2], IFE_rdelta_y[ISONUM2], IFE_clampWtSet_y[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[ISONUM2],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM3], IFE_rngth_y[ISONUM3], IFE_rdelta_y[ISONUM3], IFE_rngth_y[ISONUM3], IFE_rdelta_y[ISONUM3], IFE_clampWtSet_y[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[ISONUM3],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
//capture
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM4], IFE_rngth_y[ISONUM4], IFE_rdelta_y[ISONUM4], IFE_rngth_y[ISONUM4], IFE_rdelta_y[ISONUM4], IFE_clampWtSet_y[ISONUM4], &IFE_crvmapSet[ISONUM4], crvmapGamma, IFE_outlSet[ISONUM4],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM5], IFE_rngth_y[ISONUM5], IFE_rdelta_y[ISONUM5], IFE_rngth_y[ISONUM5], IFE_rdelta_y[ISONUM5], IFE_clampWtSet_y[ISONUM5], &IFE_crvmapSet[ISONUM5], crvmapGamma, IFE_outlSet[ISONUM5],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM6], IFE_rngth_y[ISONUM6], IFE_rdelta_y[ISONUM6], IFE_rngth_y[ISONUM6], IFE_rdelta_y[ISONUM6], IFE_clampWtSet_y[ISONUM6], &IFE_crvmapSet[ISONUM6], crvmapGamma, IFE_outlSet[ISONUM6],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  DISABLE,  DISABLE,  DISABLE, IFE_SONLY_5x5, IFE_weight_y[ISONUM7], IFE_rngth_y[ISONUM7], IFE_rdelta_y[ISONUM7], IFE_rngth_y[ISONUM7], IFE_rdelta_y[ISONUM7], IFE_clampWtSet_y[ISONUM7], &IFE_crvmapSet[ISONUM7], crvmapGamma, IFE_outlSet[ISONUM7],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
};

__align(4) IFE_FILTER IFE_param_ycc[Total_ISONum][IFE_YCC_Param+1]=
{
        //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn       sOnlyLen           weights        rngth_g                 rdelta_g                 rngth_rb                   rdelta_g              clampWtSet                   crvmapSet            crvmapGamma    outlSet             rowDefVIni, rowDefTbl, repeat
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM0], IFE_rngth_ycc[ISONUM0], IFE_rdelta_ycc[ISONUM0], IFE_rngth_ycc[ISONUM0], IFE_rdelta_ycc[ISONUM0], IFE_clampWtSet_ycc[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[ISONUM0],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM0], IFE_rngth_ycc[ISONUM0], IFE_rdelta_ycc[ISONUM0], IFE_rngth_ycc[ISONUM0], IFE_rdelta_ycc[ISONUM0], IFE_clampWtSet_ycc[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[ISONUM0],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM1], IFE_rngth_ycc[ISONUM1], IFE_rdelta_ycc[ISONUM1], IFE_rngth_ycc[ISONUM1], IFE_rdelta_ycc[ISONUM1], IFE_clampWtSet_ycc[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[ISONUM1],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM1], IFE_rngth_ycc[ISONUM1], IFE_rdelta_ycc[ISONUM1], IFE_rngth_ycc[ISONUM1], IFE_rdelta_ycc[ISONUM1], IFE_clampWtSet_ycc[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[ISONUM1],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_clampWtSet_ycc[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[ISONUM2],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_clampWtSet_ycc[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[ISONUM2],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_clampWtSet_ycc[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[ISONUM3],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_clampWtSet_ycc[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[ISONUM3],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
//capture
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM4], IFE_rngth_ycc[ISONUM4], IFE_rdelta_ycc[ISONUM4], IFE_rngth_ycc[ISONUM4], IFE_rdelta_ycc[ISONUM4], IFE_clampWtSet_ycc[ISONUM4], &IFE_crvmapSet[ISONUM4], crvmapGamma, IFE_outlSet[ISONUM4],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM4], IFE_rngth_ycc[ISONUM4], IFE_rdelta_ycc[ISONUM4], IFE_rngth_ycc[ISONUM4], IFE_rdelta_ycc[ISONUM4], IFE_clampWtSet_ycc[ISONUM4], &IFE_crvmapSet[ISONUM4], crvmapGamma, IFE_outlSet[ISONUM4],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM5], IFE_rngth_ycc[ISONUM5], IFE_rdelta_ycc[ISONUM5], IFE_rngth_ycc[ISONUM5], IFE_rdelta_ycc[ISONUM5], IFE_clampWtSet_ycc[ISONUM5], &IFE_crvmapSet[ISONUM5], crvmapGamma, IFE_outlSet[ISONUM5],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM5], IFE_rngth_ycc[ISONUM5], IFE_rdelta_ycc[ISONUM5], IFE_rngth_ycc[ISONUM5], IFE_rdelta_ycc[ISONUM5], IFE_clampWtSet_ycc[ISONUM5], &IFE_crvmapSet[ISONUM5], crvmapGamma, IFE_outlSet[ISONUM5],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM6], IFE_rngth_ycc[ISONUM6], IFE_rdelta_ycc[ISONUM6], IFE_rngth_ycc[ISONUM6], IFE_rdelta_ycc[ISONUM6], IFE_clampWtSet_ycc[ISONUM6], &IFE_crvmapSet[ISONUM6], crvmapGamma, IFE_outlSet[ISONUM6],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM6], IFE_rngth_ycc[ISONUM6], IFE_rdelta_ycc[ISONUM6], IFE_rngth_ycc[ISONUM6], IFE_rdelta_ycc[ISONUM6], IFE_clampWtSet_ycc[ISONUM6], &IFE_crvmapSet[ISONUM6], crvmapGamma, IFE_outlSet[ISONUM6],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM7], IFE_rngth_ycc[ISONUM7], IFE_rdelta_ycc[ISONUM7], IFE_rngth_ycc[ISONUM7], IFE_rdelta_ycc[ISONUM7], IFE_clampWtSet_ycc[ISONUM7], &IFE_crvmapSet[ISONUM7], crvmapGamma, IFE_outlSet[ISONUM7],       0,     NULL,      1},
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,   ENABLE, IFE_SONLY_5x5, IFE_weight_ycc[ISONUM7], IFE_rngth_ycc[ISONUM7], IFE_rdelta_ycc[ISONUM7], IFE_rngth_ycc[ISONUM7], IFE_rdelta_ycc[ISONUM7], IFE_clampWtSet_ycc[ISONUM7], &IFE_crvmapSet[ISONUM7], crvmapGamma, IFE_outlSet[ISONUM7],       0,     NULL,      1},
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
    0x4c,   0x96,   0x1d,
    0x7d8,  0x71d,  0x10c,
    0xe2,   0x700,  0x1d,
};
/*
 __align(4) UINT8 IPE_hueTab[24]=
{
        144,135,128,128,128,128,128,128,
        128,128,128,128,128,115,120,128,
        128,135,128,128,128,128,128,135
};

 __align(4) INT32 IPE_satTab[24]=
{
        -13,-5,0,-8,-10,-7,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,-10
};
*/
  __align(4) UINT8 IPE_hueTab[24]=
{
        108,108,98,98,98,128,128,128,
        128,128,128,128,128,128,128,128,
        128,128,128,128,128,128,128,108
};

 __align(4) INT32 IPE_satTab[24]=
{
        -10,-8,-8,0,-10,-40,-40,-30,
        -10,0,0,0,-10,-20,-40,-40,
        -20,-10,0,0,0,0,0,-8
};
 
 __align(4) INT32 IPE_intTab[24]=
{
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
};

 __align(4) UINT8 IPE_ddsTab[8]=
{
     32,32,32,32,32,32,32,32
};

//--------------------------------------------------------------------
//gamma_mod4
__align(4)UINT32  SRA_SIE_GammaLUT[65]={
/*0, 11, 21, 32, 43, 55, 65, 73, 82, 88,94, 100, 106, 110, 114, 119, 124, 130, 135, 139,
143, 147, 152, 157, 161, 165, 169, 172, 176, 180,183, 186, 189, 192, 195, 197, 200, 203, 205, 207,
209, 211, 213, 215, 217, 219, 221, 222, 224, 226,228, 230, 232, 234, 236, 238, 239, 241, 243, 245,
247, 249, 251, 253, 255,*/
/*0,9,18,29,39,50,61,70,79,86,94,100,106,112,118,124,128,133,138,141,146,149,153,157,
161,165,169,172,176,180,183,186,189,192,195,197,200,203,205,207,209,211,213,215,217,
219,221,222,224,226,228,230,232,234,236,238,239,241,243,245,247,249,251,253,255
0,10,23,35,46,56,65,72,80,87,94,101,107,112,118,124,128,133,138,142,146,149,153,157,
161,165,169,172,176,180,183,186,189,192,195,197,200,203,205,208,211,213,215,217,220,
222,224,226,228,231,233,235,237,239,240,242,244,246,247,248,250,251,252,253,255*/
0,6,13,22,32,43,53,63,72,80,88,96,102,108,115,121,126,132,137,141,146,150,154,157,161,
165,169,172,176,180,183,186,189,192,195,197,200,203,205,208,211,213,215,217,220,222,224,
226,228,230,231,233,235,237,238,239,240,242,243,245,247,249,251,253,255
/*0,6,14,26,38,51,62,73,84,93,104,112,120,128,133,140,145,150,155,158,162,165,168,171,174,
177,180,183,186,188,190,192,194,196,198,201,203,205,207,209,211,213,215,217,220,222,224,
226,228,230,231,233,235,236,238,239,240,241,243,245,247,249,251,253,255*/
};

__align(4)UINT32  SRA_SIE_GammaLUT_Cap[65]={
/*0,9,18,29,39,50,61,70,79,86,94,100,106,112,118,124,128,133,138,141,146,149,153,157,
161,165,169,172,176,180,183,186,189,192,195,197,200,203,205,207,209,211,213,215,217,
219,221,222,224,226,228,230,232,234,236,238,239,241,243,245,247,249,251,253,255
0, 11, 21, 32, 43, 55, 65, 73, 82, 88,94, 100, 106, 110, 114, 119, 124, 130, 135, 139,
143, 147, 152, 157, 161, 165, 169, 172, 176, 180,183, 186, 189, 192, 195, 197, 200, 203, 205, 207,
209, 211, 213, 215, 217, 219, 221, 222, 224, 226,228, 230, 232, 234, 236, 238, 239, 241, 243, 245,
247, 249, 251, 253, 255,
0,8,20,32,43,54,62,71,80,87,94,101,107,112,118,124,128,133,138,142,146,150,154,157,
161,165,169,172,176,180,183,186,189,192,195,197,200,203,205,208,211,213,215,217,220,
222,224,226,228,231,233,235,237,239,240,242,244,246,247,248,250,251,252,253,255
0,8,18,28,39,50,60,69,78,85,92,99,106,111,117,123,127,132,137,141,146,150,154,157,
161,165,169,172,176,180,183,186,189,192,195,197,200,203,205,208,211,213,215,217,220,
222,224,226,228,231,233,235,237,239,240,242,244,246,247,248,250,251,252,253,255
0,8,18,28,39,50,60,69,78,85,92,99,106,111,117,123,127,132,137,141,146,150,154,157,161,165,
169,172,176,180,183,186,189,192,195,197,200,203,205,208,211,213,215,217,220,222,224,226,228,
230,231,233,235,237,238,239,240,242,243,244,245,246,247,248,249
0,8,16,25,34,45,54,63,72,80,89,96,102,108,115,121,126,132,137,141,146,150,154,157,
161,165,169,172,176,180,183,186,189,192,195,197,200,203,205,208,211,213,215,217,220,
222,224,226,228,230,231,233,235,237,238,239,240,242,243,245,247,249,251,253,255*/
0,6,13,22,32,43,53,63,72,80,88,96,102,108,115,121,126,132,137,141,146,150,154,157,161,
165,169,172,176,180,183,186,189,192,195,197,200,203,205,208,211,213,215,217,220,222,224,
226,228,230,231,233,235,237,238,239,240,242,243,245,247,249,251,253,255
/*0,6,14,26,38,51,62,73,84,93,104,112,120,128,133,140,145,150,155,158,162,165,168,171,174,
177,180,183,186,188,190,192,194,196,198,201,203,205,207,209,211,213,215,217,220,222,224,
226,228,230,231,233,235,236,238,239,240,241,243,245,247,249,251,253,255*/
};

__align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1] = {
    {128, 128, 128, 128, 0,0},    //_IMGEffect_Off,
    {128, 128, 128,    0, 0, 0},    //_IMGEffect_BW,
    {108, 146, 128,    0, 0, 0},    //_IMGEffect_Sepia,
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


__align(4) INT16 Saturation[Total_ISONum][Max_Saturation+1] = {
//_IMGSaturation_P1,    _IMGSaturation_Normal,  _IMGSaturation_N1
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32,
//capture
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32
};

#endif
