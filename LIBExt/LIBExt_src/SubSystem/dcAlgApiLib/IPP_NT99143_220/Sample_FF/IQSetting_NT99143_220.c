#include <string.h>
#include <stdio.h>
#include "Type.h"

#if NT99143_220
#include "IPP_NT99143_220_int.h"
#include "IQSetting_NT99143_220.h"   //#include local folder .h file
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
#if 1  

{	200,  200,		4,	  128, 200,   200,	   1,	   1,  580},
{	200,  200,		4,	  128, 200,   200,	   1,	   1,  580},
{	150,  150,		5,	  128, 200,   200,	   1,	   1,  580},
{	150,  150,		6,	  128, 200,   200,	   1,	   1,  580},



#endif
#if 0
    {   120,  100,      6,     25, 200,   200,     1,      1,  580},
    {   112,  100,      6,     25, 200,   200,     1,      1,  580},
    {   106,  100,     10,     24, 200,   200,     1,      2,  580},
    {   135,  117,     11,     63, 158,   243,     1,      2,  443},
#endif
};

#if 1
__align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1] = {
    //   P1,     N,     N1
    {    128,    64,   32},
    {    128,    64,   32},
    {    128,    64,   32},
    {    128,    64,   32},
};
#else
__align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1] = {
    //   P1,     N,     N1
    {    192,    96,   64},
    {    192,    96,   64},
    {    128,    64,   32},
    {    128,    64,   32},
};
#endif

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
     -4,  5,  28,  3}
};

 __align(4) UINT8 IPE_fmStab[Total_ISONum][16]=
{
#if 0	
    {128, 96,64,32,16, 8, 4, 2, 0, 0, 0, 0,32,64,128,255},
    {128, 96,64,32,16, 8, 4, 2, 0, 0, 0, 0,32,64,128,255},
    {255,128,64,32,16, 8, 4, 2, 0, 0, 0, 0,32,64,128,255},
    {0,0,0,0,0,0,0,0,0,0,0,0,32,64,128,255}
#endif
#if 1
    {128, 96,64,32,16, 8, 4, 2, 0, 0, 0, 0,32,64,128,255},
    {128, 96,64,32,16, 8, 4, 2, 0, 0, 0, 0,32,64,128,255},
    {128, 96,64,32,16, 8, 4, 2, 0, 0, 0, 0,32,64,128,255},
    {144, 116,88,60,30, 15, 8, 2, 0, 0, 0, 0,32,64,128,255},
#endif

};
 __align(4) UINT8 IPE_fmDtab[Total_ISONum][16]=
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

  __align(4) UINT8 IPE_emStab[Total_ISONum][16]=
 {
#if 0 	
     {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
     {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64},
     {42,50,56,60,64,64,64,64,64,64,64,64,64,64,64,64},
     {24,28,36,52,64,64,64,64,64,64,64,64,64,64,64,64}
#endif
#if 1
     {64,64,64,64,68,72,76,80,88,96,96,96, 96, 100, 104, 108},
     {64,64,64,64,68,72,76,80,88,96,96,96, 96, 100, 104, 108},
     {42,50,56,60,64,64,64,64,64,64,64,64,64,64,64,64},
     {42,50,56,60,64,64,64,64,64,64,64,64,64,64,64,64},
#endif	 	

 };

__align(4) UINT CHR_Param[5][28];

//IFE Parameter-------------------------------------------------------
#define IFE_G 0
#define IFE_RB 1

UINT32 IFE_weight_emb[Total_ISONum][6]=
{
     {4,2,1,2,1,1},
     {4,2,1,2,1,1},
     {4,2,1,2,1,1},
     {4,2,1,2,1,1}
};

UINT32 IFE_rngth_emb[Total_ISONum][IFE_RB+1][2]=
{
#if 1	
{{ 6, 8},{ 6, 8}},
{{ 6, 8},{ 6, 8}},
{{ 12,16},{12,16}},
{{ 16,24},{16,24}},
#endif	
#if 0	
{{ 6, 8},{ 6, 8}},
{{ 6, 8},{ 6, 8}},
{{ 6, 8},{ 8, 10}},
{{ 12,16},{16,20}},
#endif	
};

//rngth_g/rngth_rb rdelta
INT32 IFE_rdelta_emb[Total_ISONum][IFE_RB+1][4]=
{
#if 1
	//rdelta_g  rdelta_rb
    {{1,1,2,2},{1,1,2,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{0,1,2,1},{1,2,3,2}},
    {{2,2,2,0},{2,2,2,0}}
#else
		//rdelta_g	rdelta_rb
		{{1,1,2,2},{1,1,2,2}},
		{{0,1,2,1},{1,2,3,2}},
		{{0,1,2,1},{1,2,3,2}},
		{{2,2,2,0},{2,2,2,0}}


#endif

};

UINT32 IFE_clampWtSet_emb[Total_ISONum][3]=
{
    //clampTh, wtMul_g, wtMul_rb
    {   128,  64,   64},
    {   128,  64,   64},
    {   128,  64,   64},
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
        { 16, 16,IFE_EIGHT_NEIGHBOURS},
        { 16, 16,IFE_EIGHT_NEIGHBOURS},
        { 16, 16,IFE_EIGHT_NEIGHBOURS},
        { 16, 16,IFE_EIGHT_NEIGHBOURS}
    },
    {
        {16,16,IFE_EIGHT_NEIGHBOURS},
        {16,16,IFE_EIGHT_NEIGHBOURS},
        { 16, 16,IFE_EIGHT_NEIGHBOURS},
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
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM2], IFE_rngth_emb[ISONUM2][IFE_G], IFE_rdelta_emb[ISONUM2][IFE_G], IFE_rngth_emb[ISONUM2][IFE_RB], IFE_rdelta_emb[ISONUM2][IFE_RB], IFE_clampWtSet_emb[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[0][ISONUM2],       0,     NULL,      1},
    {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_emb[ISONUM3], IFE_rngth_emb[ISONUM3][IFE_G], IFE_rdelta_emb[ISONUM3][IFE_G], IFE_rngth_emb[ISONUM3][IFE_RB], IFE_rdelta_emb[ISONUM3][IFE_RB], IFE_clampWtSet_emb[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[0][ISONUM3],       0,     NULL,      1},
};

/**
group have sequence order, must finish the repeat time of the group,then use next group param
repeat time ,0xff, as finish symbol
*/
__align(4) IFE_FILTER IFE_param_raw[Total_ISONum][IFE_RAW_Param+1] =
{
        //rowDefEn,  outlEn, filterEn, crvmapEn,  sOnlyEn  sOnlyLen       weights                  rngth_g                        rdelta_g                         rngth_rb                       rdelta_rb                         clampWtSet                   crvmapSet                crvmapGamma  outlSet               rowDefVIni,  rowDefTbl, repeat
    {
        {DISABLE,   ENABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[0][ISONUM0], IFE_rngth_raw[0][ISONUM0][IFE_G], IFE_rdelta_raw[ISONUM0][IFE_G], IFE_rngth_raw[0][ISONUM0][IFE_RB], IFE_rdelta_raw[ISONUM0][IFE_RB], IFE_clampWtSet_raw[ISONUM0], &IFE_crvmapSet[ISONUM0], crvmapGamma, IFE_outlSet[0][ISONUM0],       0,     NULL,      1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   ENABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_raw[0][ISONUM1], IFE_rngth_raw[0][ISONUM1][IFE_G], IFE_rdelta_raw[ISONUM1][IFE_G], IFE_rngth_raw[0][ISONUM1][IFE_RB], IFE_rdelta_raw[ISONUM1][IFE_RB], IFE_clampWtSet_raw[ISONUM1], &IFE_crvmapSet[ISONUM1], crvmapGamma, IFE_outlSet[0][ISONUM1],       0,     NULL,      1},
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
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_rngth_ycc[ISONUM2], IFE_rdelta_ycc[ISONUM2], IFE_clampWtSet_ycc[ISONUM2], &IFE_crvmapSet[ISONUM2], crvmapGamma, IFE_outlSet[0][ISONUM2],       0,     NULL,      4},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff},//finish raw
    },
    {
        {DISABLE,   DISABLE,  ENABLE,   DISABLE,  DISABLE, IFE_SONLY_3x3, IFE_weight_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_rngth_ycc[ISONUM3], IFE_rdelta_ycc[ISONUM3], IFE_clampWtSet_ycc[ISONUM3], &IFE_crvmapSet[ISONUM3], crvmapGamma, IFE_outlSet[0][ISONUM3],       0,     NULL,      4},
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
#if 0 //110%
0x004C, 0x009B, 0x0018,
0x079A, 0x06CB, 0x019B,
0x0130, 0x06DE, 0x07F3
#endif

#if 1 // 100%
0x004C, 0x0096, 0x001D,
0x079C, 0x06DA, 0x018A,
0x011D, 0x06F4, 0x07EF
#endif

#if 0
0x004C, 0x0096, 0x001D,
0x07D5, 0x07AC, 0x0080,
0x0080, 0x0795, 0x07EC
#endif
};

 __align(4) UINT8 IPE_hueTab[24]=
{
        128,128,128,128,128,128,128,128,
        128,128,128,128,128,128,128,128,
        128,128,128,128,128,128,128,128
};

 __align(4) INT32 IPE_satTab[24]=
{
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
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
#if 1
   0,   14,   27,   39,   49,   57,   65,   72, 
  79,   85,   92,   98,  103,  109,  114,  119, 
 123,  128,  132,  136,  140,  144,  148,  151, 
 155,  158,  161,  164,  167,  170,  172,  175, 
 178,  180,  183,  185,  187,  190,  192,  194, 
 197,  199,  201,  204,  206,  208,  211,  213, 
 215,  218,  220,  223,  225,  228,  230,  233, 
 235,  238,  240,  243,  245,  248,  250,  253, 
 255
#endif
#if 0
//gamma5
  8,   19,	 31,   42,	 53,   63,	 74,   83,	 92,  100, 
108,  116,	125,  131,	138,  144,	151,  156,	161,  166, 
171,  175,	179,  183,	188,  190,	193,  196,	199,  202, 	
205,  208,	211,  213,	215,  217,	219,  221,	223,  225, 
228,  229,	230,  232,	233,  234,	236,  237,	239,  240,
241,  242,	243,  244,	245,  246,	248,  248,	249,  250, 
251,  251,	252,  253,	255
#endif

#if 0
  0,  16,  29,  40,  51,  61,  71,  81,  89,  95,
101, 107, 114, 120, 126, 132, 137, 141, 145, 150,
155, 159, 162, 166, 170, 175, 179, 183, 187, 191,
194, 197, 200, 202, 204, 207, 210, 212, 214, 217,
219, 221, 224, 226, 228, 231, 232, 234, 235, 237,
238, 239, 240, 242, 243, 244, 245, 247, 249, 250,
251, 252, 253, 254, 255,
#endif
};

__align(4)UINT32  SRA_SIE_GammaLUT_Night[65]={

#if 1
 12,   22,	 33,   45,	 58,   66,	 75,   83,	 91,   97, 

104,  110,	117,  122,	128,  134,	140,  145,	150,  155, 

160,  164,	169,  174,	178,  181,	185,  189,	193,  196, 

200,  204,	208,  210,	212,  214,	217,  219,	221,  223, 

226,  227,	229,  230,	232,  233,	235,  236,	238,  239, 

240,  241,	242,  243,	244,  245,	246,  247,	248,  249, 

250,  251,	252,  253,	255,
#endif
#if 0
 16,   26,	 37,   48,	 61,   69,	 78,   86,	 93,  100, 

106,  112,	120,  124,	130,  136,	142,  147,	151,  157, 

162,  165,	170,  175,	180,  182,	186,  190,	194,  197, 

201,  205,	209,  210,	212,  215,	218,  220,	222,  224, 

226,  227,	229,  230,	232,  233,	236,  237,	239,  240, 

240,  241,	242,  243,	244,  245,	246,  247,	248,  249, 

250,  251,	252,  253,	255
#endif

#if 0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,
#endif
};

__align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1] = {
    {128, 128, 128, 128, 0, 15},    //_IMGEffect_Off,
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
   -32,                 0,                    32
};

#endif
