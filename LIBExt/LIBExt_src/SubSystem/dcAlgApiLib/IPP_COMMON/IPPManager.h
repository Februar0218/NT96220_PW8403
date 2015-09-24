
#ifndef _JOBMANAGER_H_
#define _JOBMANAGER_H_

#include    "Type.h"
#include    "imageAlgInfor.h"
#include    "sie_lib.h"
#include    "ipe_lib.h"
#include    "ime_lib.h"
#include    "ife_lib.h"

#define THREE_LINE_DUMMY_PIX            6
#define PADDING_REPEATING_PIX           16

/*--------------------------------------*/
/*   IPP system interface return code defination   */
/*--------------------------------------*/
#define IPP_OFFSET          (-256)
#define IPP_OK              (0)                       // IPP ok
#define IPP_FAIL            ((-1)+ IPP_OFFSET)        // IPP fail
#define IPP_NOMORE_SPACE    ((-2)+ IPP_OFFSET)        // IPP no memory
#define IPP_NOT_EXIST       ((-3)+ IPP_OFFSET)        // IPP ID Not exist

typedef enum
{
    DUMMY_JOB,
    PREVIEW_JOB,
    VIDEO_JOB,
    CAPTURE_JOB,
    IFE_DRAM_RAWIMGIN_JOB,
    IPE_DRAM_RAWIMGIN_JOB,
    IPE_IME_FRAME_1PATH_JOB,
    IPE_IME_FRAME_2PATH_JOB,
    SIE_D2D,
    IME_D2D,
    IME_RTCHG,          //Runtime Chg IME Param
    IFE_D2D,
    IFE_RTCHG,          //Runtime Chg IFE FILTER
    ENUM_DUMMY4WORD(JobTypeEnum)
}JobTypeEnum;
//#NT#2010/12/30#ethanlau -begin
//#NT#for ime sliceH,sliceNum,Fmt use
typedef enum
{
    _UNKNOW = 0,
    _Vid = 1,
    _Cap = 2,
    _Prv = 3,
    _CapFQV = 4,
    _MaxJob = 5,
    ENUM_DUMMY4WORD(Job2ImeInfo)
}Job2ImeInfo;
//#NT#2010/12/30#ethanlau -end
/////////////////////////////////////////////////////////////////////////////
//SIE relative infor

/**
    Sensor Format Selection.

    Sensor format.
*/
//@{
typedef enum
{
    SENSOR_OUT_PROGRESSIVE = 0, ///< progressive data
    SENSOR_OUT_TWO_LINE = 1, ///< two-line interleaved data
    SENSOR_OUT_THREE_LINE = 2, ///< three-line interleaved data
    ENUM_DUMMY4WORD(SENSORFormatEnum)
} SENSORFormatEnum;

//#NT#2010/12/22#ethanlau -begin
//#NT#middle struct , incase of driver struct have any difference
typedef struct SIE_PRE_PARAM_STRUCT
{
    UINT32 RawCenterX;
    UINT32 RawCenterY;
    UINT32 SiePreAddr0;//for d2d raw input addr
    UINT32 SiePreAddr1;//for ECS
    UINT32 SiePreAddr2;//for BLMaddr/DFSaddr
    UINT32 ModeSel;
    UINT32 VDTotal;
    UINT32 HDTotal;
    UINT32 VDSync;
    UINT32 HDSync;
    UINT32 HDActX;
    UINT32 HDActY;
    UINT32 HDActSizeX;
    UINT32 HDActSizeY;
    SIE_InPackBitSelEnum SieInBitDepth;
    SIE_OutPackBitSelEnum SieBitDepth;
    //STR_SIE_OUT_ADDR SieOutAddr;
    UINT32 SieOutAdd0;//for prv/cap raw data output
    UINT32 SieOutAdd1;//for prv raw data output
    UINT32 SieOutAdd2;//for prv raw data output
    UINT32 SieOutBufNum;
    //UINT32 SieOutSizeV;   // Output
    UINT32 SieDramInLineOfs;
    UINT32 SieDramInSizeH;
    UINT32 SieDramInSizeV;
    UINT32 SieCropSizeH;    //crop in
    UINT32 SieCropSizeV;    //crop in,crop out, cause sie vertical can't scale
    UINT32 SieOutSizeH;     //sie out size
    //UINT32 SieOutAdd0;
    //UINT32 SieOutAdd1;
    UINT32 SieOutLineOfs;
    UINT32 SieSliceHight;
    UINT32 SieIntEn;
    UINT32 SieBp1;
    UINT32 SieBp2;
    BOOL SieMaster;
    BOOL SieHDPhase;
    BOOL SieVDPhase;
    BOOL SieDataPhase;
    //STR_PHASE SieSyncPhase;
    SENSORFormatEnum VGAType;
    UINT32 AutoObEn;
    UINT32 SieObOfs;
    //UINT32 SieObGain;
    STR_OB_Param SieObParam;
    SIE_OutputModeEnum SieOutModSel;
    BOOL   SieDRAMInEn;
    BOOL   IsPadding;
    //STR_PRE_VA_INFOR    VaccIn;
    UINT32 MShutterTime;
    UINT32 SieFuncEn;
    UINT32 SieCfaPat;
    SIE_OutputPathEnum SiePathSel;
    UINT32 SieHDInv;
    UINT32 SieVDInv;
    STR_SUBOUT_Param SieSubParam;
    UINT32 SieSubAddr;
    UINT32 SieSubOutLofs;
    UINT32 *SieDefColumn;
    SIE_DefectNfactEnum SieDefNfact;
    STR_SIE_PATTERN SiePatParam;
    STR_SIE_HBIN SieHBinParam;
    STR_SIE_ECS_PARAM Sie_ECS;
    //STR_CHNUM SieCHDecodeParam;//for scaner only
    //UINT32 SieChOutLofs;//for scaner only
} SIE_PRE_PARAM;
/////////////////////////////////////////////////////////////////////////////
//IPE relative infor
typedef struct _StripeInfo
{
    UINT32 n;
    UINT32 l;
    UINT32 m;

}StripeInfo,*pStripeInfo;

typedef struct _IPE_StripeInfo
{
    StripeInfo stripe;
    IPE_OlapEnum olap;
}IPEStripeInfo,*pIPEStripeInfo;


typedef struct IPE_PARAM_STRUCT {
    IPE_SripeModeEnum StripeMode;       ///< IPE stripe mode selection
    UINT32 InSizeH;                     ///< Legal range : multiples of 16, IPE input image horizontal size
    UINT32 InSizeV;                     ///< IPE input image vertical size
    UINT32 InLineOfs;                   ///< Legal range : multiples of 4(word aligned),  IPE input image DRAM line offset
    IPE_PackBitEnum BitDepth;           ///< IPE input data bit number selection
    IPE_CfaPatEnum CfaPat;              ///< IPE input Bayer data start CFA selection
    UINT32 Add0;                        ///< Legal range : multiples of 4(word aligned), IPE input address ID 0
    UINT32 Add1;                        ///< Legal range : multiples of 4(word aligned), IPE input address ID 1
    UINT32 Add2;                        ///< Legal range : multiples of 4(word aligned), IPE input address ID 2
    IPE_InPPBIniEnum BuffID;
    UINT32 VAAddr;                      ///< Legal range : multiples of 4(word aligned), IPE output address, output data type is selected by OutSrcSel
    STR_VA_INFOR VaccInfo;              ///< VA related parameters
    IPEStripeInfo HStripe;                 ///< Input image H stripe, effective if StripeMode is IPE_MSTP, stripe info should be calculated to match IME stripe
    IPEStripeInfo VStripe;                 ///< Input image H stripe, effective if StripeMode is IPE_MSTP, stripe info should be calculated to match IME stripe
    //IPE_D2DInFormatEnum D2DInFormat;    ///< IPE D2D input data type selection
    UINT32 IntEn;
    BOOL IFEembed_en;                   ///< Chg this bit must stop IPL first
    UINT32 IpeSliceH;
}IPE_PARAM,*pIPE_PARAM;

/////////////////////////////////////////////////////////////////////////////
//IME relative infor
//can used multiple/add to map to real ime slicehgiht
typedef enum
{
  SLICE_2    = 2,
  SLICE_4    = 4,
  SLICE_8    = 8,
  SLICE_16   = 16,
  SLICE_32   = 32,
  SLICE_64   = 64,
  SLICE_128   = 128,
  SLICE_256   = 256,
  SLICE_512   = 512,
  ENUM_DUMMY4WORD(IME_SLICE_HEIGHT_SEL)
}IME_SLICE_HEIGHT_SEL;

//#define IME_FRAME_NUM_MAX       IME_OUTPUT_PPB_3
//#define IME_SLICE_NUM_MAX       IME_OUTPUT_PPB_5
//#define SLICE_BUF_NUM(mode)     ((mode==_Vid)?(IME_OUTPUT_PPB_4):(IME_OUTPUT_PPB_5))
//#define IME_SLICE_H(mode)       ((mode==_Vid)?(Vid_ImeSliceH):((mode==_Cap)?(Cap_ImeSliceH):(Prv_ImeSliceH)))

//#NT#2010/12/24#ethanlau -begin
//#NT#temp solution of ime structure which is used in sensor lib , all extension ime setting put here
typedef struct _IMEStripeInfo
{
    StripeInfo Hstripe;
    StripeInfo Vstripe;
    _IME_STRIPE_CAL_MODE Ime_StripeMode;
}IMEStripeInfo,*pIMEStripeInfo;

typedef struct IME_EXTEN_STURCT {
    _IME_MODE_SEL            op_mode;
    UINT32                  ImeInten;
    UINT32                  Bp1;
    UINT32                  Bp2;
    _IME_BAYER_SEL          p1_out_cfa;      //only work under output path is bayer
    _IME_PPB_NUM_SEL        Ime_BuffNum;
    UINT32                  p1_out_slice_height;
    IMEStripeInfo           ImeStripe;
    IME_SCALE_FILTER_INFO   p1_out_scale_filter;
    _IMEALGSCALEMODE        bil;
    _IMEALGSETPATH          path_num;
    IME_CST_INFO            ColorSpaceTrans;
    IME_CHROMA_FILTER_INFO  ChromaFilterInfo;
    IME_CURVE_MAPPING_INFO  CurveMappingInfo;
    IME_FILM_GRAIN_INFO     FileGrainInfo;
}IME_EXTEN,*pIME_EXTEN;

typedef struct IME_PARAM_STRUCT {
    pIME_FBADDR_PRAM         pIme_fbaddr;     //this struct is used for all modules which are need to know ime in/ouput addr/size ...etc.
    pIME_EXTEN               pIme_Ext;

}IME_PARAM,*pIME_PARAM;
//#NT#2010/12/24#ethanlau -end
//#NT#2010/12/22#ethanlau -end

/////////////////////////////////////////////////////////////////////////////
//IFE relative infor
//IFE struct
typedef struct
{
    UINT32 crvmapDelta;
    INT32 crvmapInOffs;
    INT32 crvmapOutOffs;
}CRVMAPSET;

//#NT#2010/12/28#ethanlau -begin
//#NT#IFE Struct
typedef struct IFE_FILTER_PARAM_STRUCT
{
    BOOL rowDefEn;              ///< enable of row defect concealment
    BOOL outlEn;                ///< enable of outlier filter
    BOOL filterEn;              ///< enable of IFE filter
    BOOL crvmapEn;              ///< enable of curve mapping
    BOOL sOnlyEn;               ///< spatial only enable
    _IFE_SONLYLEN  sOnlyLen;    ///< filter length of spatial only
    UINT32 *weights;            ///< spatial weighting
    //IFE_RANGESET rngth_g;      ///< range filter threshold setting for GR & GB channels or progressive data
    UINT32 *rngth_g;
    INT32 *rdelta_g;
    //IFE_RANGESET rngth_rb;     ///< range filter threshold setting for R & B channels
    UINT32 *rngth_rb;
    INT32 *rdelta_rb;
    //IFE_CLAMPWTSET clampWtSet; ///< clamp and weighting setting
    UINT32 *clampWtSet;
    //IFE_CRVMAPSET crvmapSet;   ///< curve mapping setting
    CRVMAPSET *pcrvmapSet;
    UINT32 *crvmapGamma;
    //IFE_OUTLSET outlSet;       ///< outlier filter setting
    UINT32 *outlSet;
    UINT32 rowDefVIni;         ///< vertical start of cropped window
    UINT32 *rowDefTbl;          ///< table of row defect concealment
    UINT32 repeat;             ///< repeat times
}IFE_FILTER,*pIFE_FILTER;

typedef struct IFE_PARAM_STRUCT {
    IFE_FILTER IFE_filter;      //ife filter setting,inculde ife filter repead time
    _IFE_MODE mode;             ///< operation mode
    _IFE_SRCFMT  srcFmt;        ///< source format select
    _IFE_CFASEL cfaPat;         ///< CFA pattern
    _IFE_BINNSEL  binn;         ///< image binning
    UINT32 in_addr;             ///< input starting address
    UINT32 out_addr;            ///< output starting address
    UINT32 width;               ///< image width
    UINT32 height;              ///< image height
    UINT32 ofsi;                ///< image input lineoffset
    UINT32 ofso;                ///< image output lineoffset
    UINT32 bp1;                 ///< break point 1
    UINT32 intrEn;              ///< interrupt enable
    BOOL upSrcEn;               ///< enable of upper source
    UINT32 up_addr;             ///< starting address of upper source
    BOOL downSrcEn;             ///< enable of down source
    UINT32 down_addr;           ///< starting address of down source
}IFE_ENG_SETTING,*pIFE_ENG_SETTING;
//#NT#2010/12/28#ethanlau -end
typedef struct IPP_PARAM_STRUCT
{
    SIE_PRE_PARAM *Sie_Struct;
    //IPE_PARAM *Ipe_Struct;
    pIPE_PARAM pIpe_Struct;
    //IME_FBADDR_PRAM *Ime_Struct;
    //ime struct have double layer,be care of the point
    IME_PARAM Ime_Struct;       //for panel under prv/vid, for bitstream under cap
    IME_PARAM Ime_Struct2;      //for bitstream under vid,for thumbnail/qv under cap
    pIFE_ENG_SETTING  Ife_Struct;
} IPP_PARAM;

struct PUBLIC_STR_JOBREQS {

    UINT32                      FunType;
    SIE_PRE_PARAM               *SIE_PRE_PRV;
    SIE_PRE_PARAM               *SIE_PRE_CAP;
    SIE_PRE_PARAM               *SIE_PRE_D2D;
    STR_SIE_CG_INFOR            sie_cg_info;
    STR_SIE_COFS_INFOR          sie_cofs_info;
    STR_DFS_Param               sie_dfs_setting;
    UINT32                      *sra_gammaLUT;
    pIPE_PARAM                  IpeParam;
    IME_PARAM                   ImeParam1;
    IME_PARAM                   ImeParam2;
    pIFE_ENG_SETTING            pIfeParam;
};

extern int Sensor_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int Sensor_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int Sensor_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int Sensor_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int SIE_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int SIE_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int SIE_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int SIE_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int IPE_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IPE_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IPE_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IPE_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int IME_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IME_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IME_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IME_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int IFE_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IFE_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IFE_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IFE_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern struct PUBLIC_STR_JOBREQS  IPPMgrReq;
extern IPE_PARAM IPE_Para;
extern SIE_PRE_PARAM SIE_PRE_Para_prv;
extern SIE_PRE_PARAM SIE_PRE_Para_cap;
extern SIE_PRE_PARAM SIE_PRE_Para_d2d;
extern IME_FBADDR_PRAM IME_D1_Para,IME_D2_Para;
extern IME_EXTEN IME_D1_Para_ext,IME_D2_Para_ext;
extern IFE_ENG_SETTING Ife_Param;
extern IPP_PARAM Ipp_Param;

//******************************//
//reference ippmaneger.c variable definition
extern IPE_MODEINFO IPEModeInfo;
extern IME_MODE_PRAM IMEParam;
//extern IFE_PARAM IFEParam;
//extern STR_SIE_REG SIE_Param;
//******************************//
#endif
