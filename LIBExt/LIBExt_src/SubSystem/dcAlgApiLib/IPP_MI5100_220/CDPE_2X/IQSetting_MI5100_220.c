#include <string.h>
#include <stdio.h>
#include "Type.h"

#if MI5100_220
#include "IPP_MI5100_220_int.h"
#include "IQSetting_MI5100_220.h"   //#include local folder .h file
__align(4) IPE_ISO_PARAM  IPE_ISOParam[Total_ISONum] =
{
//                   NRSel,            FStabSel,            FDtabsel,                 EdgeSel,                     KerSel,            EStabSel
    {&IPE_NRParam[ISONUM0], IPE_fmStab[ISONUM0], IPE_fmDtab[ISONUM0], &IPE_EdgeParam[ISONUM0], IPE_edgeKernelDiv[ISONUM0], IPE_emStab[ISONUM0]},
    {&IPE_NRParam[ISONUM1], IPE_fmStab[ISONUM1], IPE_fmDtab[ISONUM1], &IPE_EdgeParam[ISONUM1], IPE_edgeKernelDiv[ISONUM1], IPE_emStab[ISONUM1]},
    {&IPE_NRParam[ISONUM2], IPE_fmStab[ISONUM2], IPE_fmDtab[ISONUM2], &IPE_EdgeParam[ISONUM2], IPE_edgeKernelDiv[ISONUM2], IPE_emStab[ISONUM2]},
    {&IPE_NRParam[ISONUM3], IPE_fmStab[ISONUM3], IPE_fmDtab[ISONUM3], &IPE_EdgeParam[ISONUM3], IPE_edgeKernelDiv[ISONUM3], IPE_emStab[ISONUM3]},
//Cap
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
    {     0,     4,    0,      0},
    {     0,     8,    0,      0},
//Cap
    {     0,     4,    0,      0},
    {     0,     4,    0,      0},
    {     0,     6,    0,      0},
    {     0,     8,    0,      0}
};

__align(4) STR_IPE_EDGE_ENHMAP_PARA IPE_EdgeParam[Total_ISONum]=
{
    // EnhP, EnhN, EmapA, EmapB, EmapC, EmapD, EmapN, EmapM, EmapE
    {   98,    84,        10,        15,         200,        200,     1,      1,  580},//from yuppie
    {   98,    84,        10,        15,         200,        200,     1,      1,  580},
    {   96,    80,        11,        25,         200,        200,     1,      1,  580},
    {   90,    80,        15,        30,         200,        200,     1,      1,  580},
//Cap
    {   368,    234,        13,        15,         200,        200,     1,      1,  580},//from yuppie
    {   358,    224,        13,        15,         200,        200,     1,      1,  580},
    {   346,    210,        18,        25,         200,        200,     1,      1,  580},
    {   360,    230,        18,        25,         200,        200,     1,      1,  580},//{   130,    100,        23,        30,         200,        200,     1,      1,  580},
};

__align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1] = {
    //   P1,     N,     N1
    /*{    32,    64,    128},
    {    32,    64,    128},
    {    32,    64,    128},
    {    32,    64,    128},
//Cap
    {    64,    96,    128},
    {    64,    96,    128},
    {    48,    80,    112},
    {    48,    80,    112},*/
    {    128,    64,    32},
    {    128,    64,    32},
    {    128,    64,    32},
    {    128,    64,    32},
//Cap
    {    128,    64,    32},
    {    128,    64,    32},
    {    128,    64,    32},
    {    128,    64,    32},
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
//Cap
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
     -3,  1,   7,
     -4,  7,  12,  3}
    /*{-1, -3,  -4,
     -3, -1,   5,
     -4,  5,  28,  3}*/
};

 __align(4) UINT8 IPE_fmStab[Total_ISONum][16]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255},
//Cap
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
//Cap
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
//Cap
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
     {4,2,1,2,1,1},
     {4,2,1,2,1,1},
     {0,0,0,0,0,0},
     {0,0,0,0,0,0},
//Cap
     {4,2,1,2,1,1},
     {4,2,1,2,1,1},
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
//Cap
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{8, 10},{ 11,14}},
    {{12,14},{13,16}}
};

//rngth_g/rngth_rb rdelta
INT32 IFE_rdelta_emb[Total_ISONum][IFE_RB+1][4]=
{
    //rdelta_g  rdelta_rb
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}},
    {{2,2,2,0},{2,2,2,0}},
//Cap
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
    {     0,   0,    0},
//Cap
    {   128,  32,   32},
    {     0,   0,    0},
    {     0,   0,    0},
    {     0,   0,    0}
};

UINT32 IFE_weight_raw[Total_ISONum][6]=
{
     {4,2,1,2,1,1},
     {4,2,1,2,1,1},
     {0,0,0,0,0,0},
     {0,0,0,0,0,0},
//Cap
     {4,2,1,2,1,1},
     {4,2,1,2,1,1},
     {0,0,0,0,0,0},
     {0,0,0,0,0,0}
};

UINT32 IFE_rngth_raw[Total_ISONum][IFE_RB+1][2]=
{
    //rngth_g  rngth_rb  //range filter 0~255
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{6, 8},{ 8,10}},
    {{8,10},{11,14}},
//Cap
    {{2, 4},{ 3, 5}},
    {{5, 6},{ 6, 8}},
    {{15, 16},{16,17}},
    {{21,23},{22,24}}
};

//rngth_g/rngth_rb rdelta
INT32 IFE_rdelta_raw[Total_ISONum][IFE_RB+1][4]=
{
    //rdelta_g  rdelta_rb //range mapping -31 ~ 31
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}},
    {{2,2,2,0},{2,2,2,0}},
//Cap
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{21,21,21,21},{22,22,22,22}},
    {{21,21,21,21},{22,22,22,22}}
};

UINT32 IFE_clampWtSet_raw[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {   128,  32,   32},
    {     0,   0,    0},
    {     0,   0,    0},
    {     0,   0,    0},
//Cap    
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
//Cap
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
//Cap
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
//Cap
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
//Cap
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
//Cap
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
//Cap
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
//Cap
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
//Cap
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
//Cap
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
//Cap
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
    {DISABLE,    DISABLE,     DISABLE,    DISABLE},//<50
    {DISABLE,    DISABLE,     DISABLE,    DISABLE},//<100
    {ENABLE,     ENABLE,     DISABLE,    DISABLE},//<200
    {ENABLE,     ENABLE,     DISABLE,    DISABLE},//<400
//Cap
    {DISABLE,    DISABLE,     DISABLE,    DISABLE},//<50
    {DISABLE,    DISABLE,     DISABLE,    DISABLE},//<100
    {ENABLE,     ENABLE,     DISABLE,    DISABLE},//<200
    {ENABLE,     ENABLE,     DISABLE,    DISABLE},//<400
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

__align(4)INT16 Normal_CC[9] =
{
    //Calculated on 2012/06/18 [NTK#RandyHo]
   /* 0x4c, 0x96, 0x1d,
    0x7d7,0x737,0xf3,
    0xb2,0x75d,0x7f1*/
0x4c,0x96,0x1d,
0x7d2,0x714,0x11a,
0xde,0x734,0x7ef
};

 __align(4) UINT8 IPE_hueTab[24]=
{
  /*  128,128,125,125,125,125,128,130,
    132,130,128,128,106,95,110,108,
    114,132,132,128,133,133,133,128
*/

    // standard format
    120,128,136,142,136,140,148,152,
    162,152,148,136,128,128,132,138,
    138,132,128,128,128,128,128,120

};

 __align(4) INT32 IPE_satTab[24]=
{
    // standard format
    -10,0,0,0,4,8,14,16,
    20,14,0,0,0,0,-10,-10,
    4,10,14,0,0,0,0,-10

/*    -10,-3,12,12,12,12,0,8,
    8,8,0,6,12,20,20,10,
    -10,0,0,0,0,0,0,-5

    -5,-3,12,12,12,12,0,18,
    18,18,0,6,12,20,20,10,
    -10,0,0,0,0,0,0,0
*/
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
__align(4)UINT32  SRA_SIE_GammaLUT[65]=
{
/*     0,18,31,42,51,59,67,74,80,86,
    92,98,103,108,113,118,123,128,133,137,
    142,146,150,154,158,162,166,170,173,177,
    180,183,186,188,190,193,196,198,201,203,
    206,209,212,213,216,218,220,222,224,226,
    228,230,232,234,236,238,240,242,243,245,
    247,249,251,253,255
 
    // calculated on JUN 19, 2012  [NTK#RandyHo]
    0,14,23,33,40,48,54,60,67,73,
    79,85,91,96,102,107,113,118,124,128,
    133,139,144,148,152,156,160,163,167,171,
    175,179,182,186,190,193,196,198,201,203,
    206,209,212,213,216,218,220,222,224,226,
    228,230,232,234,236,238,240,242,243,245,
    247,249,251,253,255

0,11,21,31,41,49,57,65,73,81,88,95,102,107,113,118,124,130,134,138,143,147,151,155,159,164,
168,171,175,178,181,185,187,190,193,197,200,202,205,209,211,214,218,220,222,225,226,229,231,
232,235,237,238,241,242,244,246,246,248,249,251,252,253,254,255
0,11,21,31,41,52,62,72,81,89,97,103,110,116,122,127,132,137,141,146,150,154,158,162,166,169,173,
176,180,183,186,189,192,195,198,200,203,205,208,210,213,214,218,220,222,225,226,229,231,232,235,
237,238,241,242,244,246,247,248,249,251,252,253,254,255
0,15,28,40,50,60,69,77,85,92,99,105,110,116,122,127,132,137,141,146,150,154,158,162,
166,169,173,176,180,183,186,189,192,195,198,200,203,205,208,210,212,214,216,218,220,
222,224,226,228,230,231,232,234,236,238,239,240,241,243,245,247,249,251,253,255*/
0,11,24,37,49,60,69,77,85,92,99,105,110,116,122,127,132,137,141,146,150,154,158,162,
166,169,173,176,180,183,186,189,192,195,198,200,203,205,208,210,212,214,216,218,220,
222,224,226,228,230,231,232,234,236,238,239,240,241,243,245,247,249,251,253,255
};

__align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1] = {
    {128, 128, 128, 128, 0,20},    //_IMGEffect_Off, 24
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


__align(4) INT16 Saturation[Total_ISONum][Max_Saturation+1] = {
//_IMGSaturation_P1,    _IMGSaturation_Normal,  _IMGSaturation_N1
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32,
//Cap
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32,
   -32,                 0,                    32
};

#endif
