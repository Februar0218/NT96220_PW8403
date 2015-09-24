/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Sensor Common Header                                                   *
* Description:                                                             *
* Author: Adrian                                                               *
****************************************************************************/
#ifndef _SEN_COMMON_H
#define _SEN_COMMON_H
#include "Type.h"
#include "ippmanager.h"
#include "Sie_lib.h"
/** \addtogroup SEN_COMMON_Lib */
//@{

/**********************************************************************/

//IPE_VaccTabUpdatedEnum: CPU_UpdatedIPEVarAccTab return value
enum IPE_VaccTabUpdatedEnum
{
    NO_TAB_UPDATED,
    TAb1_UPDATED,
    TAb1_TAb2_UPDATED,
    TAb3_UPDATED,
    TAb3_TAb4_UPDATED
};

enum DFSPICEnum
{
    DFS_FIRSTPIC,
    DFS_SECONDPIC
};

enum CapInfoEnum
{
    Cap_SyncMode,
    Cap_ChromaParam,
    Cap_CFAPat,
    Cap_MaxID
};

//#NT#Add IPP mode ID
enum IPPIMGMode
{
    IPPImg_Preview,
    IPPImg_Video,
    IPPImg_PCC,
    IPPImg_AF,
    IPPImg_BeforCap,
    IPPImg_DzoomEnter,
    IPPImg_DzoomLeave,
    IPPImg_MaxID
};

enum
{
    GET_BIN_ADDR,
    GET_RAW_ADDR,
    GET_JPG_ADDR,
    GET_JPG_SIZE,
    IMG_CONVERT,
    IMG_WAIT_COMPLETE
};

typedef struct
{
    UINT GainCtrlA_MapA;
    UINT GainCtrlA_MapB;
    UINT GainCtrlB_MapA;
    UINT GainCtrlB_MapB;
    UINT GainCtrlA_GainA;
    UINT GainCtrlA_GainB;
    UINT GainCtrlA_Switch;
    UINT GainCtrlA_OffsetA;
    UINT GainCtrlA_OffsetB;
    UINT GainCtrlB_GainA;
    UINT GainCtrlB_GainB;
    UINT GainCtrlB_Switch;
    UINT GainCtrlB_OffsetA;
    UINT GainCtrlB_OffsetB;
    UINT GainCtrlEnable;
}IndepGainCtrl_Param, *PIndepGainCtrl_Param;


typedef enum
{
    SEN_MEM_SEPARATE_MODE = 0,
    SEN_MEM_COMBINE_MODE
} SEN_MEM_MODE;


#define DEFAULT_SHUTTER_TIME 31500

//--------------------------------------------------------------------------------------------------------------------------
void GetActImgFBAddr_Path1(ACTFBADDR_PRAM *pActFBAddr_para);
void GetActImgFBAddr_Path2(ACTFBADDR_PRAM *pActFBAddr_para);
//--------------------------------------------------------------------------------------------------------------------------
//#NT#2010/12/24#ethanlau -begin
//#NT#mark
//void ime_Set2VideoForModeS(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2);
//#NT#2010/12/24#ethanlau -end
//#NT#2010/12/24#ethanlau -begin
//#NT#mark
//void ime_Set2Capture(STR_YCBCR_ADDR in_addr,STR_YCBCR_ADDR out_addr,unsigned int in_format,unsigned int out_format,STR_SCALE_SIZE_INFO io_size,BOOL src);
//#NT#2010/12/24#ethanlau -end
//--------------------------------------------------------------------------------------------------------------------------
typedef void (*FPMPEG4PutSrcImgIdxCallBack)(void);
void SetupMPEG4CallBackFun(FPMPEG4PutSrcImgIdxCallBack pmp4callback);
//--------------------------------------------------------------------------------------------------------------------------

void TrgImeSlice(void);
void Time2_Line_Pixel2(UINT32 TimeUs, UINT32 PixClkKHz, UINT32 HDTotal, UINT32 *Line, UINT32 *Pixel2);

void DzoomStop(void);
void Set_DzoomFlag(BOOL CDZoom);
void StopPreviewDramData(StopIplStage _stage);
void StartPreviewDramData(void);
void StopIPPForModeSwitch(void);
void ipe_SliceCfg(IPEStripeInfo *Hstripe,IPEStripeInfo *Vstripe,UW InAddr0,UINT cfapat, JobTypeEnum Job);
//#NT#2011/01/10#ethanlau -begin
//#NT#getimeextinfo
void GetIMEExtInfo(pIME_EXTEN _Ime1_ext,pIME_EXTEN _Ime2_ext,JobTypeEnum JobType);
//#NT#2011/01/10#ethanlau -end
void ime_SliceCfg(IME_FBADDR_PRAM *pIME_Para1,IME_FBADDR_PRAM *pIME_Para2,pIME_EXTEN pIME_Ext1,pIME_EXTEN pIME_Ext2, JobTypeEnum Job);
//#NT#2011/01/15#ethanlau -begin
void ife_EmbeddedInIpeCfg(JobTypeEnum Job,_IFE_CFASEL Cfa);
//#NT#2011/01/15#ethanlau -end
void IPL_SliceSize(IPP_PARAM* pIPP_param,_IMEALGSETPATH Path);
void Ime_ChgIOSize(pIME_FBADDR_PRAM pIme_fb,pIME_EXTEN pIme_ext);
void ChromaFilter(UINT CbAddr, UINT CrAddr, UINT bufferAddr,UINT LineOffset, UINT Hsize, UINT Vsize, UINT *Chroma_Param);

typedef struct FLASHPARAM_STRUCT
{
    unsigned long PreAGC;
    unsigned long MainAGC;
    unsigned short FlshEna;
    unsigned short Polarity;
    unsigned short ExpChg;
    unsigned short AgcChg;
    unsigned short RedeyeEn;
    unsigned long DelayLines;
    unsigned long Delay2Pix;
    unsigned long PreWidth;
    unsigned long MainWidth;
    unsigned long FTP;
    unsigned long Prv_ExpoTime;
    unsigned long ChargeStatus;
    unsigned long PreAgcComp;

} FLASH_STRUCT;

#define      _SEN_CHECK_PARAM_       FALSE
#if _SEN_CHECK_PARAM_
    #define SEN_ERRMSG(...)  debug_err(("^G IPPLib:"__VA_ARGS__))
    #define SEN_MSG(...)     debug_msg("^G IPPLib:"__VA_ARGS__)
#else
    #define SEN_ERRMSG(...)
    #define SEN_MSG(...)
#endif

//For TIFF FileFmt Header
typedef __packed struct
{
    UCHAR   Byte_Order[2];
    UCHAR   Version[2];
    UCHAR   IFD_Offset[4];
} IMAGE_FILE_HEADER;

typedef __packed struct
{
    UCHAR   Tag_Code[2];
    UCHAR   Tag_Type[2];
    UCHAR   Count[4];
    UCHAR   Value_Offset[4];
} IFD_Entry; //IMAGE_FILE_DIRECTORY Entry

typedef __packed struct
{
    IMAGE_FILE_HEADER   ImageFileHeader;
    UCHAR               IFD_Entry_Number[2];
    IFD_Entry           NewSubFileType;
    IFD_Entry           ImageWidth;
    IFD_Entry           ImageLength;
    IFD_Entry           BitsPerSample;
    IFD_Entry           Compression;
    IFD_Entry           PhotometricInterpretation;
    IFD_Entry           StripOffsets;
    IFD_Entry           SamplesPerPixel;
    IFD_Entry           RowsPerStrip;
    IFD_Entry           StripByteCounts;
    IFD_Entry           XResolution;
    IFD_Entry           YResolution;
    IFD_Entry           PlanarConfiguration;
    IFD_Entry           ResolutionUnit;
    UCHAR               NextIDFOffset[4];
    UCHAR               BitsPerSampleValue[3][2];
    UCHAR               StripOffsetsValue[3][4];
    UCHAR               StripByteCountsValue[3][4];
    UCHAR               XResolutionValue[8];
    UCHAR               YResolutionValue[8];
} RGB_IMAGE_TIFF_HEADER, *PRGB_IMAGE_TIFF_HEADER; // IFD for RGB full color image

typedef __packed struct
{
    IMAGE_FILE_HEADER   ImageFileHeader;
    UCHAR               IFD_Entry_Number[2];
    IFD_Entry           NewSubFileType;
    IFD_Entry           ImageWidth;
    IFD_Entry           ImageLength;
    IFD_Entry           BitsPerSample;
    IFD_Entry           Compression;
    IFD_Entry           PhotometricInterpretation;
    IFD_Entry           StripOffsets;
    IFD_Entry           SamplesPerPixel;
    IFD_Entry           RowsPerStrip;
    IFD_Entry           StripByteCounts;
    IFD_Entry           PlanarConfiguration;
    IFD_Entry           ReferenceBlackWhite;
    UCHAR               NextIDFOffset[4];
    UCHAR               BitsPerSampleValue[3][2];
    UCHAR               ReferenceBlackWhiteValue[6][8];
} JPEG_IMAGE_TIFF_HEADER, *PJPEG_IMAGE_TIFF_HEADER; // IFD for JPEG image

typedef struct
{
    UINT32  ImageHsize;
    UINT32  ImageVsize;
    UINT32  ImageByteCounts;
    UINT32  PlaneByteCounts;
} TIFF_Param, *PTIFF_Param;

#define ECS_TblSel          ECSMAP65x65
#define ECS_TblSize         ((ECS_TblSel == ECSMAP65x65)? (65*65*4):((ECS_TblSel == ECSMAP49x49)? (49*49*4):(33*33*4)))//ECSMAP65x65
#define FQV_SAMPLE_RATE     5
//#NT#2011/01/24#ethanlau -begin
//#NT#sie subsample out size, so far,fix to 320x240
#define SIE_SubOut_H        320
#define SIE_SubOut_V        240
//#NT#2011/01/24#ethanlau -end
//for FQV two stages scaling down
#define CAP_QV_PRE_H        320
#define CAP_QV_PRE_V        240
#define CAP_SCREEN_H        CAP_QV_PRE_H
#define CAP_SCREEN_V        CAP_QV_PRE_V
#define CAP_QV_H            CAP_SCREEN_H
#define CAP_QV_V            CAP_SCREEN_V
#define CAP_THUMB_H         160
#define CAP_THUMB_V         120
#define CHROMA_BUF_NUM      2

#define JpgHeaderSize   (64 * 1024)
#define CAP_DEBUGINFO_SIZE   (64 * 1024)

#define CAP_JPG_BSTM_MIN_SIZE  0x180000
#define CAP_JPGLeadingMinSize  (100 * 1024)

//#NT#2011/07/21#Jarkko Chang -begin
//#NT# copy from 96632 dvtk
#define BlockDiffThrs 10
#define TotalDiffThres 64*3
#define BlockCntThres  4
//#NT#2011/07/21#Jarkko Chang -end

//220 IFE slice constraint
/**
    IFE must have 2 line boundry data at the end of previous ime slice data,
    and at the top of next ime slice data under slice mode
*/
#define IFE_OVERLAP_V 2
//ethan_20110805_marked,repeat time is from iq_Setting
//#define IFE_YCCD2DMAX  5   //ife Y&YCC d2d max repeat time, buf issue
/**
    when ife d2d input and ouput addr are overlap, output addr must shift left
    1 word
*/
#define IFE_OVERLAP_H   4

/**
    User Jpg video buffer ping-pong number
*/
#define JPG_BUF_NUM 3

//#NT#2011/01/24#ethanlau -begin
//#NT#//BuffNum+1 cause to fit ime pingpong num enum,_shift for 420/422fmt Cb/Cr size shfit
#define IME_PRV_MAX_SIZE(H,V,_shift,BuffNum) ((H * V  + 2 * H * V / _shift)*(BuffNum+1))
//#NT#2011/01/24#ethanlau -end
void SetPCC2Cap(UINT *Addr, UINT *JPGsize);
void SetPCC2CapRawJpg(UINT *rawAddr, UINT *rawWidth, UINT *rawHeight, UINT *rawBitDepth, UINT *jpgAddr, UINT *jpgSize);

void Set_RGB_TIFF_Header(PTIFF_Param pTIFFparam, PRGB_IMAGE_TIFF_HEADER pRGB_TIFFhearder);
void Set_JPEG_TIFF_Header(PTIFF_Param pTIFFparam, PJPEG_IMAGE_TIFF_HEADER pJPEG_TIFFhearder);
/*
//for quickview quality improvement
void    Y_LPF1( UINT Addr, UINT Hsize,UINT Vsize,UINT LineOffset);
void    Y_LPF2( UINT Addr, UINT Hsize,UINT Vsize,UINT LineOffset);
void    Y_LPF3( UINT Addr, UINT Hsize,UINT Vsize,UINT LineOffset, UINT BuffAddr);
UINT YGaussianBlur(UINT YAddr, UINT LineOffset, UINT Hsize, UINT Vsize, UB  YRadius);// pixel
UINT CbCrGaussianBlur(UINT CbAddr, UINT CrAddr, UINT LineOffset, UINT Hsize, UINT Vsize, UB  CbRadius, UB  CrRadius);// pixel
*/
UINT32 CPU_UpdatedIPEVarAccTab(IPE_PARAM *IpeParam);

ER  SetIPEVarAccInfo(IPE_SIZEINFO *SizeInfo, STR_VA_INFOR *VAInfo);
//void SubSampleRaw(UINT32 SourceAdd, UINT32 TargetAdd, UINT32 InH, UINT32 InV,UINT32 OutH, UINT32 OutV, UINT32 OutOfs, UINT32 FieldNum);

//#NT#2010/12/29#ethanlau -begin
typedef enum
{
    ImeOutInfo_UNKNOW    = 0,
    ImeOutInfo_FMT       = 1,
    ImeOutInfo_SliceH    = 2,
    ImeOutInfo_BufferNum = 3,
    ImeOutInfo_Max       = 4,
    ENUM_DUMMY4WORD(ImeOutInfo)
}ImeOutInfo;

extern void Set_IME_OutInfo(UINT32 mode,ImeOutInfo Info,UINT32 value);
extern UINT32 Get_IME_OutInfo(UINT32 mode,ImeOutInfo Info);
//#NT#2010/12/29#ethanlau -end
void sie_Set2Capture(SIE_PRE_PARAM *SiePreParam);
UINT Sen_GetIPECfaPat(UINT OrgPat, UINT FlipType);
UINT32 Sen_GetSIEAddrOfs(UINT8 FlipType, UINT32 PreOutVSize, UINT32 PreOutLineOfs);
UINT32 Sen_GetSIEFlipParam(UINT8 FlipType);
void Sen_GetImeStripeInfo(IMEStripeInfo *Imestipe);
//#NT#2011/04/07#Ethan Lau -begin
//#NT#Package up Subsample out information
extern void GetSubOutRGBinfo(UINT32* R,UINT32* G,UINT32* B,UINT32 uiSize);
//#NT#set sie color gain
extern void SetColorGain(UINT32 Rg,UINT32 Bg,UINT32 Gg);
//#NT#2011/04/07#Ethan Lau -end
//#NT#2012/06/14#Harry Tsai -begin
extern void SetECSInputAdd(UINT32 ECS_TblAddr);//VD latch
//#NT#2012/06/14#Harry Tsai -end
extern void SetSensorMode(UINT32 uiMode);

extern RGB_IMAGE_TIFF_HEADER RGB_TIFF_Header;
extern JPEG_IMAGE_TIFF_HEADER JPEG_TIFF_Header;
extern char message[];

ER User_VABegin(AF_IPEINFO *AfIpeInfo);
ER User_VAEnd(AF_IPEINFO *AfIpeInfo);

//////////////////////////////////////////////////////////////////////////////////////
//for auto-slice sie/ipe/ime in/out size config
typedef struct _SliceSizeInfo_In
{
    UINT32 ImeOutH;
    UINT32 ImeOutV;
    UINT32 ImeSliceH;
    UINT32 IpeInH;
    UINT32 IpeInV;
    UINT32 Sie_SliceH_Default;
    IPE_SripeModeEnum IpeMode;   ///< IPE stripe mode selection
    _IME_MODE_SEL     ImeMode;   ///< IME performing mode
}SliceGenIn;

typedef struct _SliceSizeInfo_Out
{
    StripeInfo Ipe_Hstripe;
    StripeInfo Ipe_Vstripe;
    StripeInfo Ime_Hstripe;
    StripeInfo Ime_Vstripe;
    UINT32 Sie_SliceH;
}SliceSizeInfo,*pSliceSizeInfo;
extern ER SliceSize_Gen(pSliceSizeInfo _pOutInfo, SliceGenIn _InInfo);
extern UINT32 CalculateRawY(UINT Add, UINT StartX,UINT StartY, UINT SizeX, UINT SizeY,UINT Lineoffset,UINT sample,UINT8 StartPixel,UINT32 *R,UINT32 *G,UINT32 *B);
//#NT#2011/07/21#Jarkko Chang -begin
extern BOOL MotionDetection(void);
//#NT#2011/07/21#Jarkko Chang -end
extern UINT32 Sen_GetJpgQinfo(void);
//#NT#2011/10/28#Ethan Lau -begin
extern ER ChgImgRatio(void);
extern ER ChgSensorFlip(UINT32 set_value);
//#NT#2011/10/28#Ethan Lau -end
#endif
