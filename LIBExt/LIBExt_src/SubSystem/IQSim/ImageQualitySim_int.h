#ifndef IQSIM_INT_H
#define IQSIM_INT_H

#include "Type.h"
#include "Debug.h"
#include "sra_lib.h"
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ife_lib.h"
#include "ime_lib.h"
#include "jpgenc.h"
#include "pll.h"
#include "string.h"
#include "utility.h"
#include "stdio.h"

#include "ImageQualitySim.h"

#include "NT96220_BASIC_IO_Reg.h"
#include "NT96220_SIE_Reg.h"
#include "NT96220_SRA_Reg.h"
#include "NT96220_IPE_Reg.h"
#include "NT96220_IFE_Reg.h"
#include "NT96220_IME_Reg.h"
#include "NT96220_IPP_EXTRN_Reg.h"

#define SHIFT_IFE_RAW
#define SHIFT_IFE_YCC
#define IFE_RAW_TIMES_MAX    3 //5
#define IFE_YCC_TIMES_MAX    15

#define SIE_EXEC    1
#define IFE_EXEC    2
#define IME_EXEC    3
#define IFE2_EXEC   4
#define JPG_EXEC    5
#define ALL_EXEC    JPG_EXEC

//capture
#define CAP_DEBUG_EXEC    ALL_EXEC //control capture flow last execute pipe
#define CAP_DEBUG_FLOW    ENABLE //trace capture main flow
#define CAP_DEBUG_FLOW2   DISABLE //trace capture sub flow
#define CAP_DEBUG_DUMP    ENABLE //dump register setting to BIN file

//input bin
#define BIN_DEBUG_DUMP    DISABLE //dump in BIN to BIN file
#define BIN_DEBUG_CONV    DISABLE //convert & dump in BIN to new BIN file
//input raw
#define RAW_DEBUG_DUMP    DISABLE //dump in RAW to RAW file
//sie output raw
#define SIE_DEBUG_FLOW    DISABLE //trace sie flow
#define SIE_DEBUG_DUMP    ENABLE //dump SIE out to RAW file
//ife output raw
#define IFE_DEBUG_FLOW    DISABLE //trace ife flow
#define IFE_DEBUG_DUMP    ENABLE //dump IFE out to RAW file
//ipe,ime output ycc
#define IPE_DEBUG_FLOW    DISABLE //trace ipe flow
#define IME_DEBUG_FLOW    DISABLE //trace ime flow
#define IME_DEBUG_DUMP    ENABLE //dump IME out to RAW file
//ife output ycc
#define IFE2_DEBUG_FLOW   DISABLE //trace ife2 flow
#define IFE2_DEBUG_DUMP   ENABLE //dump IFE2 out to RAW file
//jpg output bistream
#define JPG_DEBUG_FLOW    DISABLE //trace jpg flow
#define JPG_DEBUG_DUMP    ENABLE //dump JPG out to JPG file

/*
typedef struct _IQSIM_BIN
{
    NT96220_BASIC_IO_REG_STRUCT Basic;
    UINT32 reserved_1[10];
    NT96220_SIE_REG_STRUCT Sie;
    UINT32 reserved_2[10];
    NT96220_SRA_REG_STRUCT SieGamma;
    UINT32 reserved_3[10];
    NT96220_IFE_REG_STRUCT IfeRaw;
    UINT32 reserved_4[10];
    NT96220_IPE_REG_STRUCT Ipe;
    UINT32 reserved_5[10];
    NT96220_IFE_REG_STRUCT IfeY;
    UINT32 reserved_6[10];
    NT96220_IFE_REG_STRUCT IfeCC;
    UINT32 reserved_7[10];
    NT96220_SRA_REG_STRUCT IfeCCCrv;
    UINT32 reserved_8[10];
}
IQSIM_BIN;
*/
typedef struct _IQSIM_BIN
{
    NT96220_BASIC_IO_REG_STRUCT Basic; //20
    UINT32 reserved_x1[3]; //12
    UINT32 reserved_1[8]; //32
    
    NT96220_SIE_REG_STRUCT Sie; //336
    UINT32 reserved_2[8]; //32
    
    NT96220_SRA_REG_STRUCT SieGamma; //84
    UINT32 reserved_x3[3]; //12
    UINT32 reserved_3[8]; //32
    
    NT96220_IFE_REG_STRUCT IfeRaw; //116
    UINT32 reserved_x4[3]; //12
    UINT32 reserved_4[8]; //32
    
    NT96220_IPE_REG_STRUCT Ipe; //460
    UINT32 reserved_x5[1]; //4
    UINT32 reserved_5[8]; //32
    
    NT96220_IFE_REG_STRUCT IfeY; //116
    UINT32 reserved_x6[3]; //12
    UINT32 reserved_6[8]; //32
    
    NT96220_IFE_REG_STRUCT IfeCC; //116
    UINT32 reserved_x7[3]; //12
    UINT32 reserved_7[8]; //32
    
    NT96220_SRA_REG_STRUCT IfeCCCrv; //84
    UINT32 reserved_x8[3]; //12
    UINT32 reserved_8[8]; //32
    
    NT96220_IME_REG_STRUCT Ime; //260
    UINT32 reserved_x9[3]; //12
    UINT32 reserved_9[8]; //32

    NT96220_IPP_EXTRN_REG_STRUCT IfeExt; //56
    UINT32 reserved_xa[226]; //904
    UINT32 reserved_a[8]; //32
}
IQSIM_BIN;

#if (BIN_DEBUG_CONV == ENABLE)
typedef struct _IQSIM_BIN2
{
    NT96220_BASIC_IO_REG_STRUCT Basic; //20
    UINT32 reserved_x1[3]; //12
    UINT32 reserved_1[8]; //32
    
    NT96220_SIE_REG_STRUCT Sie; //336
    UINT32 reserved_2[8]; //32
    
    NT96220_SRA_REG_STRUCT SieGamma; //84
    UINT32 reserved_x2[3]; //12
    UINT32 reserved_3[8]; //32
    
    NT96220_IFE_REG_STRUCT IfeRaw; //116
    UINT32 reserved_x3[3]; //12
    UINT32 reserved_4[8]; //32
    
    NT96220_IPE_REG_STRUCT Ipe; //460
    UINT32 reserved_x4[1]; //4
    UINT32 reserved_5[8]; //32
    
    NT96220_IFE_REG_STRUCT IfeY; //116
    UINT32 reserved_x5[3]; //12
    UINT32 reserved_6[8]; //32
    
    NT96220_IFE_REG_STRUCT IfeCC; //116
    UINT32 reserved_x6[3]; //12
    UINT32 reserved_7[8]; //32
    
    NT96220_SRA_REG_STRUCT IfeCCCrv; //84
    UINT32 reserved_x7[3]; //12
    UINT32 reserved_8[8]; //32
}
IQSIM_BIN2;
#endif

// define Base address of engines
#define IQSim_SIE_ADDR  0x9A000000
#define IQSim_IPE_ADDR  0x9A100000
#define IQSim_IME_ADDR  0x9A200000
#define IQSim_SRA_ADDR  0x9A302000
#define IQSim_IFE_ADDR  0x9A402000

//define register numbers of engines
#define IQSim_RegNum_SIE  ((0x14C/4)+1)
#define IQSim_RegNum_IPE  ((0x1C8/4)+1)
#define IQSim_RegNum_IME  ((0x100/4)+1)
#define IQSim_RegNum_SRA  ((0x100/4)+1)
#define IQSim_RegNum_IFE  ((0x50/4)+1)

#if (CAP_DEBUG_DUMP == ENABLE)
extern IQSIM_BIN dumpBin;
#endif
#if (BIN_DEBUG_CONV == ENABLE)
extern IQSIM_BIN2 dumpBin2;
#endif

void IQSim_DumpBin(char *Filename, IQSIM_BIN* pBin);
#if (BIN_DEBUG_CONV == ENABLE)
void IQSim_DumpBin2(char *Filename, IQSIM_BIN* pBin, IQSIM_BIN2* pBin2);
#endif

typedef enum
{
    IQSim_SIE = 1,
    IQSim_IPE = 2,
    IQSim_IME = 3,
    IQSim_SRA = 4,
    IQSim_IFE = 5 
} IQSim_EngineSel;

void IQSim_DumpEngine(char *Filename, IQSim_EngineSel DbgEngine);


////////////////////////////////////////
//for auto-slice sie/ipe/ime in/out size config
typedef struct _IQSim_SliceSizeInfo_In
{
    UINT32 ImeOutH;
    UINT32 ImeOutV;
    UINT32 ImeSliceH;
    UINT32 IpeInH;
    UINT32 IpeInV;
    UINT32 Sie_SliceH_Default;
    IPE_SripeModeEnum IpeMode;   ///< IPE stripe mode selection
    _IME_MODE_SEL     ImeMode;   ///< IME performing mode
}IQSim_SliceSizeInfo_In;

typedef struct _IQSim_StripeInfo
{
    UINT32 n;
    UINT32 l;
    UINT32 m;
}IQSim_StripeInfo;

typedef struct _IQSim_SliceSizeInfo_Out
{
    IQSim_StripeInfo Ipe_Hstripe;
    IQSim_StripeInfo Ipe_Vstripe;
    IQSim_StripeInfo Ime_Hstripe;
    IQSim_StripeInfo Ime_Vstripe;
    UINT32 Sie_SliceH;
}IQSim_SliceSizeInfo_Out;

int IQSim_SliceSize_Gen(IQSim_SliceSizeInfo_Out* _pOutInfo, IQSim_SliceSizeInfo_In* _pInInfo);

extern IQSim_SliceSizeInfo_Out _OutInfo;
extern IQSim_SliceSizeInfo_In _InInfo;




////////////////////////////////////////
typedef struct _SLICE_CTRL
{
    UINT32 slice_h;
    /////////////////
    UINT32 total_h;
    /////////////////
    INT32 total_cnt;
    INT32 curr_cnt;
    INT32 ime_id;
    INT32 ife_id;
    INT32 jpg_id;
    /////////////////
    INT32 total_id;
    INT32 curr_id;
    INT32 iid;
    INT32 uid;
    INT32 cid;
    INT32 did;
    INT32 jid;
    /////////////////
    UINT32 ime_h;
    UINT32 ife_h;
    UINT32 jpg_h;
    UINT32 uiImeStatus;
    UINT32 uiIfeStatus;
    UINT32 uiJpgStatus;
}
SLICE_CTRL;


////////////////////////////////////////
typedef enum
{
    IQSim_DEF, //default
    IQSim_PC,  //load from PC IQ Sim Tool
    Total_ParamNum
}IPE_ParamNumEnum;


void IQSim_SIE_NewBuffer(UINT32 w, UINT32 h);
void IQSim_IME_NewBuffer(UINT32 w, UINT32 h);
void IQSim_IFE_NewBuffer(UINT32 w, UINT32 h);
void IQSim_JPG_NewBuffer(UINT32 w, UINT32 h);
UINT32 Cap_SliceMode_GetSliceCount(void);
UINT32 Cap_SliceMode_GetSliceHeight(void);

//RAW Parameter-------------------------------------------------------
extern UINT32 gRAW_bpp;
extern UINT32 gRAW_cfa;
void IQSim_RAW_SetBPP(int bpp);
void IQSim_RAW_SetCFA(int cfa);

//SIE Parameter-------------------------------------------------------
extern __align(4) STR_SIE_CG_INFOR SIE_ColorGain[Total_ParamNum];
extern __align(4) STR_SIE_COFS_INFOR SIE_ColorOffset[Total_ParamNum];
extern __align(4) UINT32  _SRA_SIE_GammaLUT[Total_ParamNum][65];
//---------------------------------------------------------------------
void IQSim_SIE_Open(void);
void IQSim_SIE_SetIQ(STR_SIE_REG *SieParam, int IQSimMode); //custom
void IQSim_SIE_SetIQ_2(STR_SIE_REG *SieParam, int IQSimMode); //pc IQ sim tool parameter
void IQSim_SIE_DumpIQ(void);
void IQSim_SIE_SetSrcModifier(STR_SIE_REG *SieParam, int IQSimMode);
void IQSim_SIE_SetSrc(STR_SIE_REG *SieParam, int IQSimMode);
void IQSim_SIE_SetDest(STR_SIE_REG *SieParam, int IQSimMode, UINT32 times_RAW);
void IQSim_SIE_SetDestModifier(STR_SIE_REG *SieParam, int IQSimMode);
void IQSim_SIE_Start(STR_SIE_REG *SieParam, int IQSimMode, SIE_OutputModeEnum mode);
UINT32 IQSim_SIE_Wait(void);
void IQSim_SIE_Stop(void);
void IQSim_SIE_Close(void);
void IQSim_SIE_DumpSetting(void);

//IPE Parameter-------------------------------------------------------
extern __align(4) STR_IPE_NOVANR_PARA _IPE_NRParam[Total_ParamNum];
extern __align(4) STR_IPE_EDGE_ENHMAP_PARA _IPE_EdgeParam[Total_ParamNum];
extern __align(4) INT16 _IPE_EdgeKernelDiv[Total_ParamNum][10];
extern __align(4) IPE_COLOR_EFFECT_PARAM _IPE_ColorEffectParam[Total_ParamNum];
extern __align(4) UINT8 _IPE_emStab[Total_ParamNum][16];
extern __align(4) UINT8 _IPE_fmStab[Total_ParamNum][16];
extern __align(4) UINT8 _IPE_fmDtab[Total_ParamNum][16];
extern __align(4) UINT8 _IPE_hueTab[Total_ParamNum][24];
//#NT#2011/08/08#Yuppie Lin -begin
//#NT# modify signed varible to INT32
extern __align(4) INT32 _IPE_satTab[Total_ParamNum][24];
extern __align(4) INT32 _IPE_intTab[Total_ParamNum][24];
//#NT#2011/08/08#Yuppie Lin -end
extern __align(4) UINT8 _IPE_ddsTab[Total_ParamNum][8];
extern __align(4) STR_COLOR_INFOR _IPE_ColorGain[Total_ParamNum];
extern __align(4) STR_COLOR_INFOR _IPE_ColorOffset[Total_ParamNum];
extern __align(4) INT16 _IPE_ColorMatrixParam[Total_ParamNum][9];
extern __align(4) IPE_INVERSE_EFFECT_PARAM _IPE_InvertEffectParam[Total_ParamNum];
extern __align(4) IPE_YCFIX_EFFECT_PARAM _IPE_YCFixEffectParam[Total_ParamNum];
extern __align(4) IPE_EDGE_EFFECT_PARAM _IPE_EdgeEffectParam[Total_ParamNum];
extern __align(4) STR_IPE_COLOR_PARA_EXT _IPE_ColorParamExt[Total_ParamNum];
//---------------------------------------------------------------------
void IQSim_IPE_Open(void);
void IQSim_IPE_SetSrc(IPE_MODEINFO *ipeParam, UINT32 IQSimMode);
void IQSim_IPE_SetDest(IPE_MODEINFO *ipeParam, UINT32 IQSimMode);
void IQSim_IPE_SetIQ(IPE_MODEINFO *ipeParam, UINT32 IQSimMode); //custom
void IQSim_IPE_SetIQ_2(IPE_MODEINFO *ipeParam, UINT32 IQSimMode); //pc IQ sim tool parameter
void IQSim_IPE_DumpIQ(void);
void IQSim_IPE_Start(IPE_MODEINFO *ipeParam, int IQSimMode, IPE_StripeEnum mode);
UINT32 IQSim_IPE_Wait(void);
void IQSim_IPE_Stop(void);
void IQSim_IPE_Close(void);
void IQSim_IPE_DumpSetting(void);

//IME Parameter-------------------------------------------------------
//---------------------------------------------------------------------
void IQSim_IME_Open(void);
void IQSim_IME_SetSrc(IME_MODE_PRAM *imeParam, int IQSimMode);
void IQSim_IME_SetDest(IME_MODE_PRAM *imeParam, int IQSimMode, SLICE_CTRL* pSliceCtrl);
void IQSim_IME_SetScaleIQ(IME_MODE_PRAM *imeParam, int IQSimMode);
void IQSim_IME_SetIQ(IME_MODE_PRAM *imeParam, int IQSimMode);
void IQSim_IME_SetIQ_2(IME_MODE_PRAM *imeParam, int IQSimMode);
void IQSim_IME_DumpIQ(void);
void IQSim_IME_Start(IME_MODE_PRAM *imeParam, int IQSimMode, _IME_MODE_SEL mode);
UINT32 IQSim_IME_Wait(void);
void IQSim_IME_Next(void);
void IQSim_IME_Stop(void);
void IQSim_IME_Close(void);
void IQSim_IME_DumpSetting(void);

//IFE Parameter-------------------------------------------------------
typedef struct _IFE_RANGESET2
{
    BOOL sOnlyEn;               ///< spatial only enable
    _IFE_SONLYLEN  sOnlyLen;    ///< filter length of spatial only
    _IFE_BINNSEL  binn;         ///< image binning
} IFE_RANGESET2;
typedef enum
{
    IFE_CH_RAW = 0,
    IFE_CH_Y   = 1,
    IFE_CH_CC  = 2,
    ENUM_DUMMY4WORD(IFE_CHANNEL_SEL)
} IFE_CHANNEL;
#define IFE_G    0
#define IFE_RB   1
#define IFE_Y_OVERLAP 2
#define IFE_X_OVERLAP 4

extern __align(4) UINT32 _IFE_rowdefTbl[Total_ParamNum][16];
extern __align(4) IFE_OUTLSET _IFE_outlSet_raw[Total_ParamNum];
extern __align(4) IFE_RANGESET2 _IFE_rngSet_raw[Total_ParamNum];
extern __align(4) UINT32 _IFE_weight_raw[Total_ParamNum][6];
extern __align(4) UINT32 _IFE_rngth_raw[Total_ParamNum][IFE_RB+1][2];
extern __align(4) INT32 _IFE_rdelta_raw[Total_ParamNum][IFE_RB+1][4];
extern __align(4) IFE_CLAMPWTSET _IFE_clampWtSet_raw[Total_ParamNum];
extern __align(4) IFE_OUTLSET _IFE_outlSet_y[Total_ParamNum];
extern __align(4) IFE_RANGESET2 _IFE_rngSet_y[Total_ParamNum];
extern __align(4) UINT32 _IFE_weight_y[Total_ParamNum][6];
extern __align(4) UINT32 _IFE_rngth_y[Total_ParamNum][IFE_RB+1][2];
extern __align(4) INT32 _IFE_rdelta_y[Total_ParamNum][IFE_RB+1][4];
extern __align(4) IFE_CLAMPWTSET _IFE_clampWtSet_y[Total_ParamNum];
extern __align(4) IFE_OUTLSET _IFE_outlSet_cc[Total_ParamNum];
extern __align(4) IFE_RANGESET2 _IFE_rngSet_cc[Total_ParamNum];
extern __align(4) UINT32 _IFE_weight_cc[Total_ParamNum][6];
extern __align(4) UINT32 _IFE_rngth_cc[Total_ParamNum][IFE_RB+1][2];
extern __align(4) INT32 _IFE_rdelta_cc[Total_ParamNum][IFE_RB+1][4];
extern __align(4) IFE_CLAMPWTSET _IFE_clampWtSet_cc[Total_ParamNum];
extern __align(4) IFE_CRVMAPSET _IFE_crvmapSet_cc[Total_ParamNum];
extern __align(4) UINT32 _IFE_crvmapGamma_cc[Total_ParamNum][65];
//---------------------------------------------------------------------
void IQSim_IFE_Open(void);
void IQSim_IFE_SetSrc(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, SLICE_CTRL* pSliceCtrl);
void IQSim_IFE_SetDest(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, SLICE_CTRL* pSliceCtrl);
void IQSim_IFE_SetIQ(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, int ti); //custom
void IQSim_IFE_SetIQ_2(IFE_PARAM* ifeParam, int IQSimMode, IQ_CNL cnl, int ti); //pc IQ sim tool parameter
void IQSim_IFE_DumpIQ(IQ_CNL cnl);
void IQSim_IFE_Start(IFE_PARAM* ifeParam, int IQSimMode, _IFE_MODE mode);
UINT32 IQSim_IFE_Wait(void);
void IQSim_IFE_Stop(void);
void IQSim_IFE_Close(void);
void IQSim_IFEraw_DumpSetting(int ti);
void IQSim_IFEy_DumpSetting(void);
void IQSim_IFEcc_DumpSetting(void);
void IQSim_IFE_BufBegin(IQ_CNL cnl, SLICE_CTRL* pSliceCtrl, UINT32 times);
void IQSim_IFE_BufNext(IQ_CNL cn);
void IQSim_IFE_BufEnd(IQ_CNL cnl, SLICE_CTRL* pSliceCtrl);
void IQSim_IFE_RawDummy(void);

//JPG Parameter-------------------------------------------------------
extern UINT32 JPG_quality;
void IQSim_JPG_Open(void);
void IQSim_JPG_SetIQ(JPGCmpParam* jpgParam, int IQSimMode); //custom
void IQSim_JPG_SetIQ_2(JPGCmpParam* jpgParam, int IQSimMode);  //pc IQ sim tool parameter
void IQSim_JPG_SetSrc(JPGCmpParam* jpgParam, int IQSimMode, SLICE_CTRL* pSliceCtrl);
void IQSim_JPG_SetDest(JPGCmpParam* jpgParam, int IQSimMode);
void IQSim_JPG_Start(JPGCmpParam* jpgParam, int IQSimMode, BOOL bSlice);
void IQSim_JPG_Next(void);
UINT32 IQSim_JPG_Wait(void);
void IQSim_JPG_Stop(void);
void IQSim_JPG_Close(void);

///////////////////////////////////////////////////////////////////////////////
//flow level
void IQSim_SIE_ProcessRAW_Frame(int IQSimMode, UINT32 times_RAW);
void IQSim_IFE_ProcessRAW_Frame(int IQSimMode, UINT32 times_RAW);
void IQSim_IPEIMEIFEJPG_ProcessYCC_Slice(int IQSimMode, int times_Y, int times_CC);
void IQSim_Capture(int IQSimMode);

///////////////////////////////////////////////////////////////////////////////
//log
void slog_open(char *pFilename);
void slog_out(void);
void slog_close(void);
extern __align(4) char g_sLogBuf[1024];
#define SLOG_buf    g_sLogBuf
#define SLOG_OUT    slog_out()

///////////////////////////////////////////////////////////////////////////////
//util
#define SHIFT_V(bit)    (1<<(bit))
#define SIGN(v, bit)    ((unsigned int)(   ((v)>=SHIFT_V(bit-1))   ?   ((INT32)(v)-SHIFT_V(bit))   :   ((INT32)(v))   ))


#endif //IQSIM_INT_H
