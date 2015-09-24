/** \addtogroup mIPRJAPTest */
//@{
#ifndef _CALIBRATION_H
#define _CALIBRATION_H
#include "Type.h"
#include "Alg_IPLMode.h"
/**
  Get calibration RAW
*/
typedef enum
{
    RAW_TYPE_PREVIEW = 0x00,
    RAW_TYPE_STILL,

    ENUM_DUMMY4WORD(RAW_TYPE)
} RAW_TYPE;

typedef struct
{
//    UINT32 Addr;
    UINT32 uiTotalHeight;
    UINT32 uiTotalWidth;
    UINT32 uiStartX;
    UINT32 uiStartY;
    UINT32 uiSizeX;
    UINT32 uiSizeY;
    UINT32 uiSample;
    UINT32 uiStartPixel;
} CAL_RAW_INFO;

/**
  Calibration AE setting
*/
typedef struct
{
    UINT32 uiTarget; // taget luminance(before gamma)
    UINT32 uiLum; // result Luminance(before gamma)
    UINT32 uiAEType; // AE converge type
    UINT32 uiCAType; // CA type
    UINT32 uiExpT; // exposure time
    UINT32 uiISO; // ISO
    UINT32 uiAdjStep; // AE adjust step
    UINT32 uiTimeout; // timeout count
    UINT32 uiResult; // AE result
} CAL_AE_INFO;

typedef enum
{
    AE_FIX_GAIN = 0x00,
    AE_FIX_TIME,
    AE_FIX_TIMEnGAIN,

    ENUM_DUMMY4WORD(CAL_AE_TYPE)
} CAL_AE_TYPE;

typedef enum
{
    CAL_AE_RUNNING = 0x00,
    CAL_AE_TIMEOUT,
    CAL_AE_STABLE,

    ENUM_DUMMY4WORD(CAL_AE_ERRORCODE)
} CAL_AE_ERRORCODE;

typedef enum
{
    CAL_CASOURCE_SUBOUT = 0x00,
    CAL_CASOURCE_RAW,

    ENUM_DUMMY4WORD(CAL_CASOURCE)
} CAL_CASOURCE;

/**
  Defect pixel compensation setting
*/
typedef enum
{
    DPC_TYPE_BRIGHT_PREVIEW = 0x00,
    DPC_TYPE_BRIGHT_STILL,
    DPC_TYPE_DARK_PREVIEW,
    DPC_TYPE_DARK_STILL,

    ENUM_DUMMY4WORD(DPC_TYPE)
} DPC_TYPE;

typedef struct _DPC_ALG_INFO
{
    UINT32 Type;  ///< DPC type
    UINT32 BayAddr;  ///< DRAM address of RAW data
    UINT32 DPAddr;  ///< DRAM address of DP data
    UINT32 Count;   ///< DP start count
    UINT32 MaxCount;///< DP max count
    UINT32 Threshold; ///< DP detect threshold
    UINT32 Block_X; ///< DP search block X
    UINT32 Block_Y; ///< DP search block Y
    UINT32 SkipXL;  ///< DP search left start position
    UINT32 SkipXR;  ///< DP search right start position
    UINT32 SkipYU;  ///< DP search top start position
    UINT32 SkipYD;  ///< DP search bottom start position
} DPC_ALG_INFO, *PDPC_ALG_INFO;

typedef struct _CAL_DPC_PARAM
{
    UINT32 uiPrvMode; ///< preview mode
    UINT32 uiOBofs; ///< OB offset
    UINT32 uiMsht; ///< mshutter delay
    BOOL bSaveRaw; ///< save raw data

    // AE setting
    UINT32 uiTarget; ///< taget luminance(before gamma)
    UINT32 uiAEType; ///< AE converge type
    UINT32 uiTime; ///< exposure time
    UINT32 uiISO; ///< agc gain
    UINT32 uiAdjStep; ///< AE adjust step
    UINT32 uiExpRatio; ///< preview / capture exposure difference

    DPC_ALG_INFO DPCInfo; ///< setting of DPC

    // Result
    UINT32 uiDPNum; ///< pointer to the # of defect pixel
} CAL_DPC_PARAM, *PCAL_DPC_PARAM;

/**
  Parameters for ISO calibration
*/
typedef struct _CAL_ISO_PARAM
{
    UINT32 uiPrvMode; ///< preview mode

    // AE setting
    UINT32 uiTarget; ///< taget luminance(before gamma)
    UINT32 uiTime; ///< exposure time
    UINT32 uiGain; ///< agc gain
    UINT32 uiAdjStep; ///< AE adjust step
    UINT32 uiISOBase; ///< Calibration ISO base

    // Result
    UINT32 uiISOBaseGain;  ///< ISO100 calibration value
} CAL_ISO_PARAM, *PCAL_ISO_PARAM;

/**
  Parameters for AWB calibration
*/
typedef enum
{
    CAL_AWBTYPE_PREVIEW,
    CAL_AWBTYPE_CAPTURE,

    ENUM_DUMMY4WORD(CAL_AWBTYPE)
} CAL_AWBTYPE;

typedef struct _CAL_AWB_PARAM
{
    //UINT32 uiBayAddr; ///< DRAM address for bayer data
    UINT32 uiAWBType; ///< AWB calibration type
    UINT32 uiPrvMode; ///< preview mode
    UINT32 uiOBofs; ///< OB offset
    UINT32 uiMsht; ///< mshutter delay
    BOOL bSaveRaw; ///< save raw data

    // AE setting
    UINT32 uiExpT; ///< exposure time
    UINT32 uiISO; ///< agc gain

    // Result
    UINT16 uiRGB[4]; ///< Y / R / G / B channel statistics
} CAL_AWB_PARAM, *PCAL_AWB_PARAM;

/**
  Parameters for mshutter calibration
*/
typedef struct _CAL_MSHT_PARAM
{
    UINT32 uiPrvMode; ///< preview mode
    UINT32 uiOBofs; ///< OB offset
    UINT32 uiMsht; ///< mshutter delay
    BOOL bSaveRaw; ///< save raw data

    // mshutter boundary
    UINT32 uiMshtMax;
    UINT32 uiMshtMin;

    // AE setting
    UINT32 uiExpT; ///< exposure time
    UINT32 uiISO; ///< agc gain

    // Result
    UINT16 uiRGB[4]; ///< Y / R / G / B channel statistics
} CAL_MSHT_PARAM, *PCAL_MSHT_PARAM;

typedef struct _CAL_ECS_PARAM
{
    //UINT32 uiBayAddr; ///< DRAM address for bayer data
    UINT32 uiCalibMode; ///< The Sensor mode will be calib
    UINT32 uiOBofs; ///< OB offset
    UINT32 uiMsht; ///< mshutter delay
    UINT32 uiCmpRatio; ///< ECS compensation ratio
    BOOL bSaveRaw; ///< save raw data
    UINT32 uiWidth; ///< Raw width
    UINT32 uiHeight; ///< Raw height
    UINT32 uiTableAddr; ///< ECS table addr
    // AE setting
    UINT32 uiExpT; ///< exposure time
    UINT32 uiISO; ///< agc gain
} CAL_ECS_PARAM, *PCAL_ECS_PARAM;

// for OB calibration [NTK#RandyHo]<2012/04/28>
typedef struct _CAL_OB_PARAM
{
    UINT32 uiCalibMode; ///< The Sensor mode will be calib
    UINT32 uiMsht; ///< mshutter delay
    BOOL bSaveRaw; ///< save raw data
    UINT32 uiWidth; ///< Raw width
    UINT32 uiHeight; ///< Raw height
    // AE setting
    UINT32 uiExpT; ///< exposure time
    UINT32 uiISO; ///< agc gain

    UINT Y;
    UINT Y_R;
    UINT Y_G;
    UINT Y_B;
} CAL_OB_PARAM, *PCAL_OB_PARAM;

/**
  Calibration Status
*/
#define _Mshutter_Test_Status   0x11

#define _Burnin_Status          0x33
#define _Language_Status        0x34

#define _ISO_Status 0x20
#define _Mshutter_Status 0x1B
#define _Prv_W_Status 0x41
#define _Prv_B_Status 0x42
#define _Cap_W_Status 0x45
#define _Cap_B_Status 0x46
#define _Dzoom_W_Status 0x4A
#define _Dzoom_B_Status 0x4B
#define _AWB_Status 0x52
#define _Flash_Status 0x61
#define _OB_Status 0x71
#define _Shading_Status 0x81
#define _LSC_Status 0x90
#define _ECS_Status 0x92

typedef enum _CAL_DATANUM
{
    CAL_DATANUM_STATUS_AWB,
    CAL_DATANUM_STATIS_AWBGS,
    CAL_DATANUM_STATUS_PrvDP_W,
    CAL_DATANUM_STATUS_PrvDP_B,
    CAL_DATANUM_STATUS_CapDP_W,
    CAL_DATANUM_STATUS_CapDP_B,
    CAL_DATANUM_STATUS_ISO,
    CAL_DATANUM_STATUS_Mshutter,
    CAL_DATANUM_STATUS_LensShading,
    CAL_DATANUM_STATUS_ColorShading,
    CAL_DATANUM_STATUS_FlashTriggerPointer,
    CAL_DATANUM_STATUS_FlashShading,
    CAL_DATANUM_STATUS_FlashFire,
    CAL_DATANUM_STATUS_FlashAWB,
    CAL_DATANUM_STATUS_FlashADC,
    CAL_DATANUM_STATUS_PrvDL,
    CAL_DATANUM_STATUS_CapDL,
    CAL_DATANUM_STATUS_LSC,
    CAL_DATANUM_STATUS_ECS,
    CAL_DATANUM_AWB_Statistic,
    CAL_DATANUM_AWB_GAIN,           ///<AWB R/B gain
    CAL_DATANUM_AWB_GAIN_GS,        ///<AWB GS R/B gain
    CAL_DATANUM_DP_PreviewAddr,
    CAL_DATANUM_DP_VideoAddr,
    CAL_DATANUM_DP_CaptureAddr,
    CAL_DATANUM_DP_PreviewNum,
    CAL_DATANUM_DP_CaptureNum,
    CAL_DATANUM_Mshutter,
    CAL_DATANUM_ISO,
    CAL_DATANUM_LensShading,
    CAL_DATANUM_ColorShading,
    CAL_DATANUM_Flash_TriggerPoint,
    CAL_DATANUM_Flash_Fire,
    CAL_DATANUM_Flash_AWB,
    CAL_DATANUM_DL_PrvPos,
    CAL_DATANUM_DL_CapPos,
    CAL_DATANUM_LSC_Table,
    CAL_DATANUM_LSC_TableTotalCnt,
    CAL_DATANUM_LSC_ZoomTotalCnt,
    CAL_DATANUM_ECS_Addr,
    CAL_DATANUM_STATUS_OB, // [NTK#RandyHo]<2012/04/28>
    CAL_DATANUM_OB_OFFSET, // [NTK#RandyHo]<2012/04/28>
    CAL_DATANUM_TOTAL,
    ENUM_DUMMY4WORD(CAL_DATANUM)
} CAL_DATANUM;

typedef struct _CAL_AWBDATA
{
    UINT R;
    UINT G;
    UINT B;
    UINT Rgain;
    UINT Ggain;
    UINT Bgain;
} CAL_AWBDATA;

typedef struct _CAL_FLASHDATA{
    UINT FlashTriggerPoint;
    UINT FlashFire;
    UINT FlashR;
    UINT FlashG;
    UINT FlashB;
} CAL_FLASHDATA;

typedef struct _Cal_DATA{
    UINT    CalStatus_AWB;
    UINT    CalStatus_AWBGS;
    UINT    CalStatus_PrvDP_W;
    UINT    CalStatus_PrvDP_B;
    UINT    CalStatus_CapDP_W;
    UINT    CalStatus_CapDP_B;
    UINT    CalStatus_ISO;
    UINT    CalStatus_OB;
    UINT    CalStatus_Mshutter;
    UINT    CalStatus_LensShading;
    UINT    CalStatus_ColorShading;
    UINT    CalStatus_FlashTriggerPointer;
    UINT    CalStatus_FlashShading;
    UINT    CalStatus_FlashFire;
    UINT    CalStatus_FlashAWB;
    UINT    CalStatus_FlashADC;
//#NT#2009/11/16#Chris Yang -begin
//#NT#add line defect status
    UINT    CalStatus_DL_Cap;
    UINT    CalStatus_DL_Prv;
//#NT#2009/11/16#Chris Yang -end
    UINT    CalStatus_ECS;
    CAL_AWBDATA   Cal_AWBStatistic;
    CAL_AWBDATA     Cal_AWBGain;
    CAL_AWBDATA     Cal_AWBGSGain;
    CAL_FLASHDATA Cal_Flash;
    UINT    Cal_DP_PreviewAddr;
    UINT    Cal_DP_VideoAddr;    
    UINT    Cal_DP_CaptureAddr;
    UINT    Cal_DP_PreviewNum;
    UINT    Cal_DP_CaptureNum;
    UINT    Cal_Mshutterr_Data[3];
    UINT    Cal_ISO_Data[3];
//#NT#2009/11/16#Chris Yang -begin
//#NT#add line defect position
    UINT    Cal_DL_CapturePos[16];
    UINT    Cal_DL_PreviewPos[16];
//#NT#2009/11/16#Chris Yang -end
    UINT    Cal_ECS_Addr;
    UINT    OBoffset;   // [NTK#RandyHo]<2012/04/28>
} CAL_DATA;

/**
  Calibration object
*/
typedef struct
{
    void (*fpSetMode)(UINT32 ,UINT32);
    void (*fpLock3A)(UINT32);

    void (*fpGetRAWSize)(UINT32, CAL_RAW_INFO*);
    void (*fpGetPrvAddr)(UINT32*);
    void (*fpGetPrvRAW)(UINT32, UINT32, UINT32, UINT32, UINT32);
    void (*fpGetCapRAW)(UINT32, UINT32, UINT32, UINT32, UINT32, UINT32);
    void (*fpSaveRAW)(UINT32, UINT32, UINT32);
    void (*fpGetStatistics)(UINT32, UINT32, CAL_RAW_INFO, UINT16*);
    UINT32 (*fpAEProcess)(CAL_AE_INFO*);
} CALIBRATION_API, *PCALIBRATION_API;

typedef struct
{
    UINT32 uiMemoryAddr;
    UINT32 uiMemorySize;
    PCALIBRATION_API pCalApi;
} CAL_APPOBJ, *PCAL_APPOBJ;

/**
    Initialize calibration module

    @param[in] pCalAppObj   pointer to CalAppObj
*/
extern void Cal_Init(PCAL_APPOBJ pCalAppObj);

/**
    Change IPP to preview / capture / off mode
    if change to preview, you need to assign a sensor mode

    @param[in] CalAppObj    pointer to calibration object
*/
extern void Cal_SetMode(UINT32 IppMode, UINT32 SenMode);

/**
    Lock 3A
    if change to preview, you need to assign a sensor mode

    @param[in] isLock   Lock 3A or not
*/
extern void Cal_Lock3A(UINT32 isLock);

/**
    Get total and scale down raw size (25% in the middle of raw data)

    @param[in] Type   raw data type (preview or capture)
    @param[in] RawInfo   pointer to raw size info
*/
extern void Cal_GetRAWSize(UINT32 Type, CAL_RAW_INFO* RawInfo);

/**
    Get previer buffer address (SIE out address)

    @param[in] Addr   pointer to SIE out address (IPPBUF_PRV_SIERAW_1)
*/
extern void Cal_GetPrvAddr(UINT32* Addr);

/**
    Get previer raw data

    @param[in] RawAddr   preview raw data address
    @param[in] RawSizeH   width
    @param[in] RawSizeV   height
    @param[in] ObOfs   preview OB offset
    @param[in] SiePath   SIE ouput path
*/
extern void Cal_GetPrvRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath);

/**
    Get capture raw data

    @param[in] RawAddr   capture raw data address
    @param[in] RawSizeH   width
    @param[in] RawSizeV   height
    @param[in] ObOfs   capture OB offset
    @param[in] SiePath   SIE ouput path
    @param[in] ShutterTimeUs   mshutter delay time (unit: us)
*/
extern void Cal_GetCapRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 ObOfs, UINT32 SiePath, UINT32 ShutterTimeUs);

/**
    Save raw data

    @param[in] RawAddr   capture raw data address
    @param[in] RawSizeH   width
    @param[in] RawSizeV   height
*/
extern void Cal_SaveRAW(UINT32 RawAddr, UINT32 RawSizeH, UINT32 RawSizeV);

/**
    Calculate statistics of input raw data

    @param[in] CAType   statistics type (SIE SUBOUT / RAW)
    @param[in] Addr   RAW data address
    @param[in] pStatistics   pointer to result statistics
*/
extern void Cal_GetStatistics(UINT32 CAType, UINT32 Addr, CAL_RAW_INFO RawInfo, UINT16 *pStatistics);

/**
    Wait AE stable

    @param[in] AeInfo   pointer to AE info
*/
void Cal_WaitAeStable(CAL_AE_INFO *AeInfo);

/**
    Set calibration data to sensor library

    @param[in] CalItem   calibration Item
    @param[in] pCalData   pointer to result calibration data
*/
extern void Cal_SetCalData(UINT CalItem, UINT  *pCalData);

/**
    Get calibration data structure

    @param[in] CalInfo   pointer to result calibration data structure
*/
extern void Cal_GetCalData(CAL_DATA* pCalInfo);

/**
    DP adjust flow

    @param[in] DPCParam   pointer to DPC parameter
*/
extern void Cal_DPCAdjust(CAL_DPC_PARAM* DPCParam);

/**
    ISO adjust flow

    @param[in] ISOParam   pointer to ISO parameter
*/
extern void Cal_ISOAdjust(CAL_ISO_PARAM* ISOParam);

/**
    AWB adjust flow

    @param[in] ISOParam   pointer to AWB parameter
*/
extern void Cal_AWBAdjust(CAL_AWB_PARAM* AWBParam);

/**
    mshutter adjust flow

    @param[in] pMSHTParam   pointer to Mshutter parameter
*/
extern void Cal_MshtAdjust(CAL_MSHT_PARAM* pMSHTParam);

/**
    ECS adjust flow

    @param[in] pECSParam   pointer to ECS parameter
*/
extern void Cal_ECSAdjust(CAL_ECS_PARAM* pECSParam);

extern void Cal_OBAdjust(CAL_OB_PARAM* pOBParam);

/**
    Register Cal API

    @param[out] pointer to calibration API
*/
PCALIBRATION_API Cal_GetCalApi(void);
#endif
//@}
