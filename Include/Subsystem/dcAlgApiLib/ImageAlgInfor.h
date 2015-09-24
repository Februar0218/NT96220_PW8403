#ifndef _IMAGEALGINFOR_H
#define _IMAGEALGINFOR_H

//----------------------------------------------------------------------------------------------------
//************************************************************************
// File Name    : ImageAlgInfor.h
// Description  : Define the image quality selection items
//************************************************************************
#include "type.h"
//#include "ipe_lib.h"
//#include "ime_lib.h"
#include "sif.h"
#include    "af_alg.h"
//#include "dis_lib.h"
//#include "ife_lib.h"

#define MAX_FB_W            640
#define MAX_FB_H            576


//#NT#2009/10/21#Adrian -begin
//#NT# add doxygen comment

//********AlgCallback Function********
//MessageID define
#define ALGMSG_PREVIEW              (1)     ///<The imagepipeline prepare entering preview mode, not in preview yet
#define ALGMSG_FOCUS                (1<<1)  ///<Prepare to enter auto focus process
#define ALGMSG_FOCUSEND             (1<<2)  ///<Auto focus process end
#define ALGMSG_PREFLASH             (1<<3)  ///<Prepare to preflash
#define ALGMSG_PREFLASHEND          (1<<4)  ///<Preflash end.
#define ALGMSG_FIRSTRAW             (1<<5)  ///<Prepare to start capture raw data
#define ALGMSG_JPEGOK               (1<<6)  ///<Jpeg file in dram is ready, quick view data is ready
#define ALGMSG_VIDEO_RECORDING      (1<<7)  ///<Prepare to enter ime 2 path out, one for display and the other for movie recording
#define ALGMSG_PREVIEWSTABLE        (1<<8)  ///<The imagepipeline is running in preview mode
#define ALGMSG_CHGDZOOMSTEP         (1<<9)  ///<Digital zoom has changed in one step, UI can update the zoom bar
#define ALGMSG_JPGEXIFMODIFY        (1<<10) ///<ImageInfor.c has write some basic exif data, the system F/W can write the needed exif data at this moment
#define ALGMSG_LENSJAM              (1<<11) ///<The Zoom lens is in a jam
#define ALGMSG_SUSKEY               (1<<12) ///<The imagepipeline prepare to enter raw capture mode, please the system F/W should block any key---switch mode/zoom
                                            ///s1/s2/....
#define ALGMSG_SUSPHOTO             (1<<13) ///<The imagepipeline prepare to enter raw capture mode, photo task should not do any operation till next preview mode
#define ALGMSG_SUSFLOWPRV           (1<<14) ///<The imagepipeline prepare to enter raw capture mode, please the system F/W should block any key---switch mode/zoom
                                            ///s1/s2/....,any opertion.
#define ALGMSG_RSMKEY               (1<<15) ///<When back to PreviewIdle mode, the system F/W return normal operation
#define ALGMSG_RSMPHOTO             (1<<16) ///<When back to PreviewIdle mode, the system F/W return normal operation
#define ALGMSG_RSMFLOWPRV           (1<<17) ///<When back to PreviewIdle mode, the system F/W return normal operation
#define ALGMSG_CHGOZOOMSTEP         (1<<18) ///<Optical zoom has changed in one step, UI can update the zoom bar
#define ALGMSG_CAPFSTOK             (1<<19) ///<Capture FST Write OK
#define ALGMSG_FDEND                (1<<20) ///<Face-Detection process end
#define ALGMSG_QVSTART              (1<<21) ///<Quick view data is ready.
#define ALGMSG_SDEND                (1<<22) ///<Smile-Detection end
#define ALGMSG_IMEFRMEND            (1<<23) ///<IME frame end
#define ALGMSG_CAPEND               (1<<24) ///<Capture End
#define ALGMSG_VidJpgCfg            (1<<25) ///<Video JpgEng config
#define ALGMSG_VidDateStampCfg      (1<<26) ///<Video datestamp config
#define ALGMSG_VIDEO_RECSTOP        (1<<27) ///<IPL had been stopped under video rec. mode
#define ALGMSG_IPL_BUFND            (1<<28) ///<return IPL buf end addr
#define ALGMSG_IPL_STOP             (1<<29) ///<IPL have been stopped
#define ALGMSG_SHUTTERSOUND         (1<<30) ///<Timeing for play shutter sound
//********Alg Menu Information********
#define _IMGSatus_NoChange          0xF0

//----------Type Definition--------------------------------------------------------------------------



//#NT#2009/10/13#Bowen Li -begin
//#modify capture flow for new memory arrange
//********AlgCallback Function********
/**
 * AlgCallback Function Message
 */
/**
    @name sturct  AlgMsgInfo
*/
//@{
typedef struct _AlgMsgInfo_{
unsigned char   CapStatus_Mode;     ///< Capture Status Operating Mode----not used now
unsigned char   Alg_Mode;           ///< Image Pipe Operating Mode, see enum _Alg_Callback_Mode
UINT32          AF_Status;          ///<AF Status, see enum _Alg_Callback_AFStatus
UINT32          AF_Window[AF_TOTAL_WIN];          ///<AF Lock Window
unsigned char   Jpeg_Num;           ///<Jpeg Number
unsigned int    Jpeg0_Addr;         ///<Jpeg0 Starting Address
unsigned short  Jpeg0_Hsize;        ///<Jpeg0 Hsize
unsigned short  Jpeg0_Vsize;        ///<Jpeg0 Vsize
unsigned int    Jpeg0_Size;         ///<Jpeg0 BitstreamSize
unsigned int    Jpeg1_Addr;         ///<Jpeg1 Starting Address
unsigned short  Jpeg1_Hsize;        ///<Jpeg1 Hsize
unsigned short  Jpeg1_Vsize;        ///<Jpeg1 Vsize
unsigned int    Jpeg2_Addr;         ///<Jpeg2 Starting Address
unsigned short  Jpeg2_Hsize;        ///<Jpeg2 Hsize
unsigned short  Jpeg2_Vsize;        ///<Jpeg2 Vsize
UINT32          VidJpg_Hsize;       ///<video jpg hsize
UINT32          VidJpg_Vsize;       ///<video jpg hsize
UINT32          VidStampBuf;        ///<vid stamp buf
unsigned short  Thumbnail_Hsize;    ///<Thumbnail_Hsize
unsigned short  Thumbnail_Vsize;    ///<Thumbnail_Vsize
unsigned int    Thumbnail_Format;   ///<Thumbnail_Format
unsigned int    Thumbnail0_Y_Addr;  ///<Thumbnail0_Y_Addr
unsigned int    Thumbnail0_Cb_Addr; ///<Thumbnail0_Cb_Addr
unsigned int    Thumbnail0_Cr_Addr; ///<Thumbnail0_Cr_Addr
unsigned int    Thumbnail0_LineOffset;///<Thumbnail0_LineOffset
unsigned int    Thumbnail1_Y_Addr;  ///<Thumbnail1_Y_Addr
unsigned int    Thumbnail1_Cb_Addr; ///<Thumbnail1_Cb_Addr
unsigned int    Thumbnail1_Cr_Addr; ///<Thumbnail1_Cr_Addr
unsigned int    Thumbnail1_LineOffset;  ///<Thumbnail1_LineOffset
unsigned int    Thumbnail2_Y_Addr;  ///<Thumbnail2_Y_Addr
unsigned int    Thumbnail2_Cb_Addr; ///<Thumbnail2_Cb_Addr
unsigned int    Thumbnail2_Cr_Addr; ///<Thumbnail2_Cr_Addr
unsigned int    Thumbnail2_LineOffset;///<Thumbnail2_LineOffset
unsigned char   DzoomIndex;         ///<Digital Zoom Index
unsigned char   OzoomIndex;         ///<Optical Zoom Index
unsigned int    JpgExifAddr;        //<JpgExifAddr
UINT8           BlinkDetectStatus;  ///<BlinkDetectStatus
UINT8           FaceDetectNum;      ///<Detected FaceDetectNum
RECT            FaceDetectRect[36]; ///<FaceDetectRect of Deteced Face
UINT16          CapCurrentNum;      ///<CapCurrentNum
UINT32          IPL_BufEndAddr;
} AlgMsgInfo,*pAlgMsgInfo;
//#NT#2009/10/13#Bowen Li -end
//@}

typedef void (*FPALGCALLBACK)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
typedef void (*FPMPEG4PutSrcImgIdxCallBack)(void);
void SetupMPEG4CallBackFun(FPMPEG4PutSrcImgIdxCallBack pmp4callback);

//********Sensor Photo Capture App Callback********
typedef BOOL (*FPS2PRESSDET)(void);
/**
    @name input object for ImgCapture_Open()
*/
//@{
typedef struct _CAPTURE_APPOBJ{
    UINT32  uiMemoryAddr;///< Required memory address
    UINT32  uiMemorySize;///< Required memory size
    void    (*CallBackFunc)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);//< Callback function
    UINT32  uiDisplayFBWidth;///<  Display Hszie
    UINT32  uiDisplayFBHeight;///< Display Vsize
    FPS2PRESSDET FpS2PressDet;///<function pointer of key detection of Shutter2
} CAPTURE_APPOBJ,*PCAPTURE_APPOBJ;//SensorLibraryNeedInfo,*pSensorLibraryNeedInfo;
//@}

/**
    @name Capture Mode
*/
//@{
//********Capture Mode********
typedef enum {
    CAPMODE_PRVIDLE,///< Preview idle mode
    CAPMODE_PRVBUSY///<Preview busy mode(file writing)
} _CapMode;
//@}

/**
    @name Alg Operating Mode
*/
//@{
//********Alg Operating Mode********
typedef enum {
    MODE_Preview,           ///<IPP in Preview
    MODE_Captrue_Focusing,  ///<IPP in Focusing
    MODE_Captrue_FocusEnd,  ///<IPP in FocusEnd
    MODE_Captrue_FirstRaw,  ///<IPP enter to capture raw data
    MODE_Captrue_LastSlice, ///<IPP in capture last slice
    MODE_Captrue_QVSTART,   ///<IPP ready to quick view start
    MODE_Captrue_JpegOK,    ///<IPP jpeg bitstream ready
    MODE_Video_Preview,     ///<IPP in Video Preview
    MODE_Video_Recording,   ///<IPP in Video Recording
    MODE_PCCam              ///IPP in PCCAM
} _Alg_Callback_Mode;
//@}

/**
    @name AF Status
*/
//@{
//********AF Status********
typedef enum {
    AFStatus_OK,
    AFStatus_FAIL,
    AFStatus_TOONEAR,
    AFStatus_TOOFAR
} _Alg_Callback_AFStatus;
//@}

/**
     param out_format 0:444, 1:422 co-sited, 2:422 centered, 3:420 co-sited, 4:420 centered, 5-7:reserved.

     Detailed description.
     @note (Optional)
*/
typedef enum _IME_OUTFMT
{
    IMEOUT2_FMT444   = 0,     ///< IME out_format 444
    IMEOUT2_FMT422_1 = 1,     ///< IME out_format 422 co-sited
    IMEOUT2_FMT422_2 = 2,     ///< IME out_format 422 centered
    IMEOUT2_FMT420_1 = 3,     ///< IME out_format 420 co-sited
    IMEOUT2_FMT420_2 = 4,     ///< IME out_format 420 centere
    ENUM_DUMMY4WORD(IME_OUTFMT)
}IME_OUTFMT;

/**
     param out_format 0:444, 1:422, 2:420, 3:reserved.

     Detailed description.
     @note (Optional)
*/
typedef enum _IME_INFMT
{
    IMEIN_FMT444 = 0,           ///< YCC444 format for input
    IMEIN_FMT422 = 1,           ///< YCC422 format for input
    IMEIN_FMT420 = 2,           ///< YCC420 format for input
    ENUM_DUMMY4WORD(IME_INFMT)
}IME_INFMT;

/**
    @name IME Scale and FB Params
*/
//@{
//********IME Scale and FB Param********
typedef struct _FBADDR_PRAM
{
    unsigned int y0_addr;  ///<y0_addr
    unsigned int cb0_addr; ///<cb0_addr
    unsigned int cr0_addr; ///<cr0_addr

    unsigned int y1_addr; ///<y1_addr
    unsigned int cb1_addr; ///<cb1_addr
    unsigned int cr1_addr; ///<cr1_addr

    unsigned int y2_addr; ///<y2_addr
    unsigned int cb2_addr; ///<cb2_addr
    unsigned int cr2_addr; ///<cr2_addr

    unsigned int ime_format; ///<ime_format
    unsigned int Ipe_fc; ///<Ipe_fc
    unsigned int Ime_fc; ///<Ime_fc
} FBADDR_PRAM;
//@}

/**
    @name IME Setting Parameter
*/
//@{
typedef struct _IME_FBADDR_PRAM
{
    //D2D In or DirectPath Out
    unsigned int y0_addr;           ///<y0_addr
    unsigned int cb0_addr;          ///<cb0_addr
    unsigned int cr0_addr;          ///<cr0_addr
    //D2D Out or DirectPath Out
    unsigned int y1_addr;           ///<y1_addr
    unsigned int cb1_addr;          ///<cb1_addr
    unsigned int cr1_addr;          ///<cb1_addr
    //DirectPath Out
    unsigned int y2_addr;           ///<y2_addr
    unsigned int cb2_addr;          ///<cb2_addr
    unsigned int cr2_addr;          ///<cr2_addr
    //DirectPath Out
    unsigned int y3_addr;           ///<y2_addr
    unsigned int cb3_addr;          ///<cb2_addr
    unsigned int cr3_addr;          ///<cr2_addr
    //DirectPath Out
    unsigned int y4_addr;           ///<y2_addr
    unsigned int cb4_addr;          ///<cb2_addr
    unsigned int cr4_addr;          ///<cr2_addr

    unsigned int in_h;              ///<input Hsize
    unsigned int in_v;              ///<input Vsize
    unsigned int out_h;             ///output Hsize
    unsigned int out_v;             ///<output Vsize

    unsigned int out_lineoffset;    ///<output Y-lineoffset
    UINT32       out_Cb_lineoffset; ///<output Cb-lineoffset
    UINT32       out_Cr_lineoffset; ///<output Cr-lineoffset
    UINT32       in_lineoffset;     ///<input Y-lineoffset
    UINT32       in_Cb_lineoffset;  ///<output Cb-lineoffset
    UINT32       in_Cr_lineoffset;  ///<output Cr-lineoffset

    IME_INFMT    in_format;         ///<input data format
    IME_OUTFMT   out_format;        ///<output data format
} IME_FBADDR_PRAM,*pIME_FBADDR_PRAM;
//@}

//@

/**
    @name IDE setting parameter
*/
//@{
typedef struct _ACTFBADDR_PRAM
{
    unsigned int y_addr;
    unsigned int cb_addr;
    unsigned int cr_addr;
    unsigned int focus_value[16];
    BOOL bUsingDIS;
    UINT32 BorderY_Addr;
    UINT32 BorderU_Addr;
    UINT32 BorderV_Addr;
    UINT32 DisY_Addr;
    UINT32 DisU_Addr;
    UINT32 DisV_Addr;
    UINT32 SrcYLineOffset;
    UINT32 SrcUVLineOffset;
} ACTFBADDR_PRAM;
//@}

/**
    @name sturct ColorGainInfo
*/
//@{
//********IQ related********
typedef struct    t_ColorGainInfo{
    char    R_Adj;///< R gain addjust value
    char    G_Adj;///< G gain addjust value
    char    B_Adj;///< B gain addjust value
} ColorGainInfo, *PColorGainInfo;
//@}


/**
    @name sturct FOCUS_WINDOW_INFOR
*/
//@{
//********AAA********
typedef struct _FOCUS_WINDOW_INFOR{
    unsigned long    stx;///<VA starting x
    unsigned long    sty;///<VA starting y
    unsigned long    hsz;///<VA window h_size
    unsigned long    vsz;///<VA window v_size
}FOCUS_WINDOW_INFOR;
//@}

/**
    @name sturct ImageAlgInfo
*/
//@{
//********Alg Menu Information********
typedef struct    t_ImageAlgInfo{
    UINT           DSCMode;            ///< 1 , Please reference enum:_DSCMode_sel
    UINT           SceneMode;          ///< 2 , Please reference enum:_IMGSceneMode_sel
    UINT           IMGSize;            ///< 3 , Please reference enum:_IMGSize_sel
    UINT           Quality;            ///< 4 , Please reference enum:_IMGQuality_sel
    UINT           DriveMode;          ///< 5 , Please reference enum:_IMGDriveMode_sel
    UINT           BurstDriveNum;      ///< 6 , Please reference enum:_IMGBurstNum_sel
    UINT           Sharpness;          ///< 7 , Please reference enum:_IMGSharpness_sel
    UINT           Saturation;         ///< 8 , Please reference enum:_IMGEffect_sel
    UINT           DZoomIndex;         ///< 9 , Please reference enum:_IMGDZoomIdx_sel
    UINT           FlashMode;          ///< 10, Please reference enum:_IMGFlashMode_sel
    UINT           Frequency;          ///< 11, Please reference enum:_POWERFreq_sel
    UINT           ImageEffect;        ///< 12, Please reference enum:_IMGEffect_sel
    UINT           AEMode;             ///< 13, Please reference enum:_IMGAEMode_sel
    UINT           AEEVValue;          ///< 14, Please reference enum:_IMGEVValue_sel
    UINT           WBMode;             ///< 15, Please reference enum:_IMGWBMode_sel
    UINT           AFMode;             ///< 16, Please reference enum:_IMGAFMode_sel
    UINT           AFWindow;           ///< 17, Please reference enum:_IMGAFWindow_sel
    UINT           AFPosition;         ///< 18, Please reference enum:_IMGAFPosition_sel
    UINT           ISO;                ///< 19, Please reference enum:_IMGISO_sel
    UINT           FileFmt;            ///< 20, Please reference enum:_IMGFileFmt_sel
    UINT           SiePath;            ///< 21, Please reference enum:_IMGSiePath_sel
    UINT           SieBitDepth;        ///< 22, Please reference enum:_IMGSieBitDepth_sel
    UINT           SieInBitDepth;      ///< 23,                       _IMGSieBitDepth_sel
    UINT           SieCapBitDepth;     ///< 24, Please reference enum:_IMGSieBitDepth_sel
    UINT           SieCapInBitDepth;   ///< 25,                       _IMGSieBitDepth_sel
    //UINT           PrePrvBitDepth;     ///< 26, Please reference enum:_IMGPreBitDepth_sel
    //UINT           PreCapBitDepth;     ///< 27, Please reference enum:_IMGPreBitDepth_sel
    UINT           FaceDetectionMode;  ///< 28, Please reference enum:_IMGFaceDetectionMode_sel
    UINT           SmileDetectionMode; ///< 29, Please reference enum:_IMGSmileDetectionMode_sel
    UINT           BlinkDetectionMode; ///< 30, Please reference enum:_IMGBlinkDetectionMode_sel
    UINT           DISMode;            ///< 31, Please reference enum:_IMGDISMode_sel
    UINT           ImageRatio;         ///< 32, Please reference enum:_IMGRatio_sel
    UINT           DispRatio;          ///< 33, Please reference enum:_IMGRatio_sel
    UINT           ImageFlip;          ///< 34, Please reference enum:_IMGFlip_sel
    UINT           ReEncMechanism;     ///< 35, Please reference enum:_IMG_CaptureMode_sel
    //#NT#2009/10/09#Bowen Li -begin
    //#NT# add segment write switch
    UINT           SegmentModeEn;      ///< 36, Enable/Disable Filesystem segment write
    //#NT#2009/10/09#Bowen Li -end
    UINT           Prv_Resolution;     ///  37, Please reference enum:_IMGResolution_sel
    UINT           Vid_Resolution;     ///  38, Please reference enum:_IMGResolution_sel
    UINT           AntiShakeLevel;     ///  39, plz reference enum:_IMGStillAntiShaking
    UINT           Exposuremode_UI;    ///  40, Plz    reference enum:_IMGExposureMode_Sel
    UINT           MaxJpgSize;         ///  41, Please reference enum:_IMGSize_sel
    UINT           RamDskEn;           ///  44,                  _IMGRamDskEn_Sel
    UINT           RamDskAddr;         ///  45,
    UINT           RamDskSize;         //   46,
    UINT           SensorSize;         //   47,                  _IMGSize_sel
//#NT#2011/08/22#Bowen Li -begin
//#NT#add image filter
    UINT           ImageFilter;        //   48,
//#NT#2011/08/22#Bowen Li -end
    UINT           DsiplayFitWindow;        //   48,
} ImageAlgInfo, *PImageAlgInfo;
//@}

/**
    @name sturct ImageAlgInfo
*/
//@{
typedef struct    t_SceneModeInfo{
    unsigned char    SceneMode;
    unsigned char    FlashMode;
    unsigned char    EVComp;
    unsigned char    Saturation;
    unsigned char    Sharpness;
    unsigned char    WBMode;
    unsigned char    Focus;
    unsigned char    FlashChangeAE;
}  SceneModeInfo;
//@}

/**
    @name Alg UI Item
*/
//@{
//********Alg UI Item********
/*UI Item*/
typedef enum {
    _DSCMode=1,             //< 1. Please reference enum:_DSCMode_sel
    _SceneMode,             //< 2, Please reference enum:_IMGSceneMode_sel
    _ImgSize,               //< 3, Please reference enum:_IMGSize_sel
    _Quality,               //< 4, Please reference enum:_IMGQuality_sel
    _DriveMode,             //< 5, Please reference enum:_IMGDriveMode_sel
    _BurstNum,              //< 6, Please reference enum:_IMGBurstNum_sel
    _Sharpness,             //< 7, Please reference enum:_IMGSharpness_sel
    _Saturation,            //< 8,  Please reference enum:_IMGSaturation_sel
    _DZoomIndex,            //< 9,  Please reference enum:_IMGDZoomIdx_sel
    _FlashMode,             //< 10, Please reference enum:_IMGFlashMode_sel
    _Frequency,             //< 11, Please reference enum:_POWERFreq_sel
    _ImageEffect,           //< 12, Please reference enum:_IMGEffect_sel
    _AEMode,                //< 13, Please reference enum:_IMGAEMode_sel
    _AEEV,                  //< 14, Please reference enum:_IMGEVValue_sel
    _WBMode,                //< 15, Please reference enum:_IMGWBMode_sel
    _AFMode,                //< 16, Please reference enum:_IMGAFMode_sel
    _AFWindow,              //< 17, Please reference enum:_IMGAFWindow_sel
    _AFPosition,            //< 18, Please reference enum:_IMGAFPosition_sel
    _ISOIndex,              //< 19, Please reference enum:_IMGISO_sel
    _FileFmt,               //< 20, Please reference enum:_IMGFileFmt_sel
    _SiePath,               //< 21, Please reference enum:_IMGSiePath_sel
    _SieBitDepth,           //< 22, Please reference enum:_IMGSieBitDepth_sel
    _SieInBitDepth,         //< 23,
    _SieCapBitDepth,        //< 24, Please reference enum:_IMGSieBitDepth_sel
    _SieCapInBitDepth,      //< 25,
    _PrePrvBitDepth,        //< 26, Please reference enum:_IMGPreBitDepth_sel
    _PreCapBitDepth,        //< 27, Please reference enum:_IMGPreBitDepth_sel
    _FaceDetectionMode,     //< 28, Please reference enum:_IMGFaceDetectionMode_sel
    _SmileDetectionMode,    //< 29, Please reference enum:_IMGSmileDetectionMode_sel
    _BlinkDetectionMode,    //< 30, Please reference enum:_IMGBlinkDetectionMode_sel
    _DISMode,               //< 31, Please reference enum:_IMGDISMode_sel
    _ImageRatio,            //< 32, Please reference enum:_IMGRatio_sel
    _DispRatio,             //< 33,
    _ImageFlip,             //< 34, Please reference enum:_IMGFlip_sel
    _ReEncMechanism,        //< 35, Please reference enum:_IMG_CaptureMode_sel
    //#NT#2009/10/09#Bowen Li -begin
    //#NT# add segment write switch
    _SegmentModeEn,         //< 36,Enable/Disable Filesystem segment write
    //#NT#2009/10/09#Bowen Li -end
    _Prv_Resolution,        //  37,Please reference enum:_IMGResolution_sel
    _Vid_Resolution,        //  39,Please reference enum:_IMGResolution_sel
    _StillAntiShaking,      //  39,plz reference enum:_IMGStillAntiShaking
    _Exposuremode_UI,       //  40                   _IMGExposureMode_Sel
    _MaxJpgSize,            //  41                   _IMGSize_sel
    _FlashType,             //  42                   _IMGFLASHTYPE_Sel
    _FlashCurve,            //  43                   _IMGFLASHIGBT_Sel or _IMGFLASHSCR_Sel
    _RamDskEn,              //  44                   _IMGRamDskEn_Sel
    _RamDskAddr,            //  45
    _RamDskSize,            //  46
    _SensorSize,            //  47                   _IMGSize_sel
//#NT#2011/08/22#Bowen Li -begin
//#NT#add image filter
    _ImgFilter,             //  48
//#NT#2011/08/22#Bowen Li -end
    _DsiplayFitWindow,
    ENUM_DUMMY4WORD(_UI_Item_sel)
} _UI_Item_sel;
//@}
//***************************************************************************
typedef enum{
    _OFF=0,
    _ON,
    ENUM_DUMMY4WORD(_IMGRamDskEn_Sel)
}_IMGRamDskEn_Sel;

typedef enum{
    _ERS,
    _GRS,
    _MaxExposureMode,
    ENUM_DUMMY4WORD(_IMGExposureMode_Sel)
}_IMGExposureMode_Sel;

typedef enum{
    IGBT,
    SCR,
    FlashTypeMax,
    ENUM_DUMMY4WORD(_IMGFLASHTYPE_Sel)
}_IMGFLASHTYPE_Sel;
#define MaxFlashTYPE FlashTypeMax

typedef enum{
    IGBT_1=0,
    IGBT_2=1,
    IGBT_3=2,
    IGBT_4=3,
    IGBT_Max=4,
    ENUM_DUMMY4WORD(_IMGFLASHIGBT_Sel)
}_IMGFLASHIGBT_Sel;
#define MaxFlashIGBT IGBT_Max

typedef enum{
    SCR_1=10,
    SCR_2=11,
    SCR_3=12,
    SCR_4=13,
    SCR_Max=14,
    ENUM_DUMMY4WORD(_IMGFLASHSCR_Sel)
}_IMGFLASHSCR_Sel;
#define MaxFlashSCR SCR_Max
//****************************************************************************************************
//_Level0 == disable
typedef enum{
    _Level0,
    _Level1,
    _Level2,
    _Level_Max,
    ENUM_DUMMY4WORD(_IMGStillAntiShaking)
}_IMGStillAntiShaking;

#define Max_AntiShakeLevel _Level_Max
//#NT#20091008#ethanlau -end
//****************************************************************************************************
//****************************************************************************************************
/**
    @name _DSCMode_sel
*/
//@{
/*DSC mode*/
typedef enum {
    _DSC_Auto,
    _DSC_Manual,
    ENUM_DUMMY4WORD(_DSCMode_sel)
//    _DSC_Vdeo,
//    _DSC_Playback
 } _DSCMode_sel;
//@}
#define Max_DSCMode _DSC_Manual
//****************************************************************************************************
typedef enum{
    _Resolution_320x240,
    _Resolution_352x240,
    _Resolution_640x480,
    _Resolution_720x480,
    _Resolution_1280x720,
    _Resolution_1280x960,
    _Resolution_1280x480,
    _Resolution_1440x1080,
    _Resolution_848x480,
    _Resolution_2560x1920,
    _Resolution_1920x1080,
    _Resolution_1280x720P60,
    _Resolution_640x480P60,
    _Resolution_320x240P60,
    _Resolution_320x240P120,
    _Resolution_Max,
    ENUM_DUMMY4WORD(_IMGResolution_sel)
}_IMGResolution_sel;
//****************************************************************************************************
/**
    @name _IMGSceneMode_sel
*/
//@{
/*SceneMode*/
typedef enum {
    _SceneMode_Auto,
    _SceneMode_Landscape,
    _SceneMode_Potrait,
    _SceneMode_Sport,
    _SceneMode_NightScene,
    _SceneMode_AV,
    _SceneMode_TV,
    _SceneMode_Manual,
    _SceneMode_NightPotrait,
    _SceneMode_NightLandscape,
    _SceneMode_Party,
    _SceneMode_SunSet,
    _SceneMode_BackLight,
    _SceneMode_Document,
    _SceneMode_Firework,
    _SceneMode_Beach,
    _SceneMode_BMode,
    _SceneMode_Program,
    _SceneMode_HighISO,
    ENUM_DUMMY4WORD(_IMGSceneMode_sel)
} _IMGSceneMode_sel;
//@}
#define Max_SceneMode _SceneMode_HighISO

//****************************************************************************************************
/**
    @name _IMGSize_sel
*/
//@{
/*ImgSize*/
typedef enum {
    _IMGSize_16M,
    _IMGSize_15M,
    _IMGSize_14M,
    _IMGSize_12_2M,
    _IMGSize_12M,
    _IMGSize_10M,
    _IMGSize_9M,
    _IMGSize_8M,
    _IMGSize_7M_169,
    _IMGSize_7M,
    _IMGSize_6M,
    _IMGSize_5_4M,
    _IMGSize_5M,
    _IMGSize_4_9M,
    _IMGSize_4_4M,
    _IMGSize_4M,
    _IMGSize_3_8M,
    _IMGSize_3M,
    _IMGSize_2_6M,
    _IMGSize_2_1M,
    _IMGSize_2M_169,
    _IMGSize_2M,
    _IMGSize_1_5M,
    _IMGSize_1M,
    _IMGSize_0_8M,
    _IMGSize_0_6M,
    _IMGSize_0_4M,
    _IMGSize_VGA,
    _IMGSize_0_2M,
    _IMGSize_16_9,
    ENUM_DUMMY4WORD(_IMGSize_sel)
} _IMGSize_sel;
//@}
#define Max_ImgSize _IMGSize_16_9

//****************************************************************************************************
/**
    @name _IMGQuality_sel
*/
//@{
/*IMGQuality*/
typedef enum {
    _IMGQuality_Best,
    _IMGQuality_Fine,
    _IMGQuality_Normal,
    _IMGQuality_Economy,
    //#NT#2009/12/12#Niven Cho -begin
    //#NT#BRC by FileCtrl Fail
    _IMGQuality_MaxNum,
    ENUM_DUMMY4WORD(_IMGQuality_sel)
    //#NT#2009/12/12#Niven Cho -end
} _IMGQuality_sel;
//@}
#define Max_ImgQuality _IMGQuality_MaxNum //_IMGQuality_Economy

//****************************************************************************************************
/**
    @name _IMGDriveMode_sel
*/
//@{
/*IMGDriveMode*/
typedef enum {
    _IMGDriveMode_Single,
    _IMGDriveMode_Burst,
    _IMGDriveMode_InfBurst,
    _IMGDriveMode_SmallBurst16,
    _IMGDriveMode_AEBreaketing,
    _IMGDriveMode_AWBBreaketing,
    _IMGDriveMode_PreShot,
    _IMGDriveMode_CoupleShot,
    _IMGDriveMode_AlbumShot,
    _IMGDriveMode_BurstFlash,
    _IMGDriveMode_DIS3Shots,
    ENUM_DUMMY4WORD(_IMGDriveMode_sel)
} _IMGDriveMode_sel;
//@}
#define Max_DriveMode _IMGDriveMode_DIS3Shots

//****************************************************************************************************

/**
    @name _IMGSharpness_sel
*/
//@{
typedef enum {
    _IMGSharp_P1,
    _IMGSharp_Normal,
    _IMGSharp_N1,
    ENUM_DUMMY4WORD(_IMGSharpness_sel)
} _IMGSharpness_sel;
//@}
#define Max_Sharp _IMGSharp_N1

//****************************************************************************************************
/**
    @name _IMGSaturation_sel
*/
//@{
typedef enum {
    _IMGSaturation_P1,
    _IMGSaturation_Normal,
    _IMGSaturation_N1,
    ENUM_DUMMY4WORD(_IMGSaturation_sel)
} _IMGSaturation_sel;
//@}

#define Max_Saturation _IMGSaturation_N1

//****************************************************************************************************
/**
    @name IMGDZoomIdx_sel
*/
//@{
/*IMGDZoomIdx*/
typedef enum {
    IMGDZoomIdx_00,
    IMGDZoomIdx_01,
    IMGDZoomIdx_02,
    IMGDZoomIdx_03,
    IMGDZoomIdx_04,
    IMGDZoomIdx_05,
    IMGDZoomIdx_06,
    IMGDZoomIdx_07,
    IMGDZoomIdx_08,
    IMGDZoomIdx_09,
    IMGDZoomIdx_10,
    IMGDZoomIdx_11,
    IMGDZoomIdx_12,
    IMGDZoomIdx_13,
    IMGDZoomIdx_14,
    IMGDZoomIdx_15,
    IMGDZoomIdx_16,
    IMGDZoomIdx_17,
    IMGDZoomIdx_18,
    IMGDZoomIdx_19,
    IMGDZoomIdx_20,
    IMGDZoomIdx_21,
    IMGDZoomIdx_22,
    IMGDZoomIdx_23,
    IMGDZoomIdx_24,
    IMGDZoomIdx_25,
    IMGDZoomIdx_26,
    IMGDZoomIdx_27,
    IMGDZoomIdx_28,
    IMGDZoomIdx_29,
    IMGDZoomIdx_30,
    IMGDZoomIdx_31,
    IMGDZoomIdx_32,
    IMGDZoomIdx_33,
    IMGDZoomIdx_34,
    IMGDZoomIdx_35,
    IMGDZoomIdx_36,
    IMGDZoomIdx_37,
    IMGDZoomIdx_38,
    IMGDZoomIdx_39,
    IMGDZoomIdx_40,
    IMGDZoomIdx_41,
    IMGDZoomIdx_42,
    IMGDZoomIdx_43,
    IMGDZoomIdx_44,
    IMGDZoomIdx_45,
    IMGDZoomIdx_46,
    IMGDZoomIdx_47,
    IMGDZoomIdx_48,
    IMGDZoomIdx_49,
    IMGDZoomIdx_50,
    ENUM_DUMMY4WORD(IMGDZoomIdx_sel)
} IMGDZoomIdx_sel;
//@}

//****************************************************************************************************
/**
    @name _IMGDZoomIdx_sel
*/
//@{
typedef enum {
    _IMGDZoomIdx_00,
    _IMGDZoomIdx_01,
    _IMGDZoomIdx_02,
    _IMGDZoomIdx_03,
    _IMGDZoomIdx_04,
    _IMGDZoomIdx_05,
    _IMGDZoomIdx_06,
    _IMGDZoomIdx_07,
    _IMGDZoomIdx_08,
    _IMGDZoomIdx_09,
    _IMGDZoomIdx_10,
    _IMGDZoomIdx_11,
    _IMGDZoomIdx_12,
    _IMGDZoomIdx_13,
    _IMGDZoomIdx_14,
    _IMGDZoomIdx_15,
    _IMGDZoomIdx_16,
    _IMGDZoomIdx_17,
    _IMGDZoomIdx_18,
    _IMGDZoomIdx_19,
    _IMGDZoomIdx_20,
    _IMGDZoomIdx_21,
    _IMGDZoomIdx_22,
    _IMGDZoomIdx_23,
    _IMGDZoomIdx_24,
    _IMGDZoomIdx_25,
    _IMGDZoomIdx_26,
    _IMGDZoomIdx_27,
    _IMGDZoomIdx_28,
    _IMGDZoomIdx_29,
    _IMGDZoomIdx_30,
    _IMGDZoomIdx_31,
    _IMGDZoomIdx_32,
    _IMGDZoomIdx_33,
    _IMGDZoomIdx_34,
    _IMGDZoomIdx_35,
    _IMGDZoomIdx_36,
    _IMGDZoomIdx_37,
    _IMGDZoomIdx_38,
    _IMGDZoomIdx_39,
    _IMGDZoomIdx_40,
    _IMGDZoomIdx_41,
    _IMGDZoomIdx_42,
    _IMGDZoomIdx_43,
    _IMGDZoomIdx_44,
    _IMGDZoomIdx_45,
    _IMGDZoomIdx_46,
    _IMGDZoomIdx_47,
    _IMGDZoomIdx_48,
    _IMGDZoomIdx_49,
    _IMGDZoomIdx_50,
    ENUM_DUMMY4WORD(_IMGDZoomIdx_sel)
} _IMGDZoomIdx_sel;
//@}

//****************************************************************************************************
/**
    @name _IMGFlashMode_sel
*/
//@{
/*IMGFlashMode*/
typedef enum {
    _IMGFlashMode_Auto,
    _IMGFlashMode_ForceOFF,
    _IMGFlashMode_ForceON,
    _IMGFlashMode_AutoRedEye,
    _IMGFlashMode_ForceONRedEye,
    _IMGFlashMode_NightStrobe,
    ENUM_DUMMY4WORD(_IMGFlashMode_sel)
} _IMGFlashMode_sel;
//@}
#define Max_FlashMode _IMGFlashMode_NightStrobe

//****************************************************************************************************
/**
    @name _POWERFreq_sel
*/
//@{
/*POWERFreq*/
typedef enum {
    _POWERFreq_50Hz,
    _POWERFreq_60Hz,
    ENUM_DUMMY4WORD(_POWERFreq_sel)
} _POWERFreq_sel;
//@}
#define Max_FrequencyMode _POWERFreq_60Hz
//****************************************************************************************************
/**
    @name _IMGEffect_sel
*/
//@{
/*IMGEffect*/
typedef enum {
    _IMGEffect_Off,
    _IMGEffect_BW,
    _IMGEffect_Sepia,
    _IMGEffect_Vivid,
    _IMGEffect_Neutral,
    _IMGEffect_LowSharping,
    _IMGEffect_Fasion1,
    _IMGEffect_Fasion2,
    _IMGEffect_CustomManual,
    _IMGEffect_Red,
    _IMGEffect_Green,
    _IMGEffect_Blue,
    _IMGEffect_Yellow,
    _IMGEffect_Purple,
    ENUM_DUMMY4WORD(_IMGEffect_sel)
} _IMGEffect_sel;
//@}
#define Max_ImgEffect _IMGEffect_Purple

//****************************************************************************************************
//#NT#2011/08/22#Bowen Li -begin
//#NT#add image filter
/**
    @name _IMGFilter_sel
*/
//@{
/*IMGFilter*/
typedef enum {
    _IMGFilter_Off,
    _IMGFilter_MonoChrome,
    _IMGFilter_Class,
    _IMGFilter_Cyborg,
    _IMGFilter_HighConstMono,
    _IMGFilter_XRay,
    _IMGFilter_Infrared,
    _IMGFilter_PopArt,
    _IMGFilter_DayDream,
    _IMGFilter_ColorPencil,
    _IMGFilter_PencilSketch,
    _IMGFilter_ChalkSketch,
    _IMGFilter_GrowingEdge,
    _IMGFilter_RockMode,
    _IMGFilter_BeautyWhite,
    _IMGFilter_FancySpot,
    _IMGFilter_Max,
    ENUM_DUMMY4WORD(_IMGFilter_sel)
} _IMGFilter_sel;
//#NT#2011/08/22#Bowen Li -end
//@}
//****************************************************************************************************

/**
    @name _IMGAWBBreaketing_sel
*/
//@{
/*IMGAWBBreaketing*/
typedef enum {
    _IMGAWBBreaketing_OFF,
    _IMGAWBBreaketing_ON,
    ENUM_DUMMY4WORD(_IMGAWBBreaketing_sel)
} _IMGAWBBreaketing_sel;
//@}

//****************************************************************************************************
/**
    @name _IMGWBMode_sel
*/
//@{
/*IMGWBMode*/
typedef enum {
    _IMGWBMode_Auto,
    _IMGWBMode_Daylight,
    _IMGWBMode_Cloudy,
    _IMGWBMode_Tungsten,
    _IMGWBMode_Fluorescent1,
    _IMGWBMode_Fluorescent2,
    _IMGWBMode_Sunset,
    _IMGWBMode_BirthdayParty,
    _IMGWBMode_Customer1,
    _IMGWBMode_Customer2,
    _IMGWBMode_Customer3,
    ENUM_DUMMY4WORD(_IMGWBMode_sel)
}_IMGWBMode_sel;
//@}
#define Max_WBMode _IMGWBMode_Customer3

//****************************************************************************************************
/**
    @name _IMGWBAdjust_sel
*/
//@{
/*IMGWBMode*/
typedef enum {
    _IMGWBAdjust_Off,
    _IMGWBAdjust_On,
    ENUM_DUMMY4WORD(_IMGWBAdjust_sel)
}_IMGWBAdjust_sel;
//@}
#define Max_WBAdjust _IMGWBAdjust_On

//****************************************************************************************************
/**
    @name _IMGEVValue_sel
*/
//@{
/*IMGEVValue*/
typedef enum {
    _IMGEVValue_N3,
    _IMGEVValue_N8_3,
    _IMGEVValue_N5_2,
    _IMGEVValue_N7_3,
    _IMGEVValue_N2,
    _IMGEVValue_N5_3,
    _IMGEVValue_N3_2,
    _IMGEVValue_N4_3,
    _IMGEVValue_N1,
    _IMGEVValue_N2_3,
    _IMGEVValue_N1_2,
    _IMGEVValue_N1_3,
    _IMGEVValue_00,
    _IMGEVValue_P1_3,
    _IMGEVValue_P1_2,
    _IMGEVValue_P2_3,
    _IMGEVValue_P1,
    _IMGEVValue_P4_3,
    _IMGEVValue_P3_2,
    _IMGEVValue_P5_3,
    _IMGEVValue_P2,
    _IMGEVValue_P7_3,
    _IMGEVValue_P5_2,
    _IMGEVValue_P8_3,
    _IMGEVValue_P3,
    ENUM_DUMMY4WORD(_IMGEVValue_sel)
} _IMGEVValue_sel;
//@}
#define Max_EvValue _IMGEVValue_P3

//****************************************************************************************************
/**
    @name _IMGAEMode_sel
*/
//@{
/*IMGAEMode*/
typedef enum {
    _IMGAEMode_CenterWeighted,
    _IMGAEMode_Spot,
    _IMGAEMode_Matrix,
    _IMGAEMode_Evaluative,
    _IMGAEMode_FaceDetection,
    ENUM_DUMMY4WORD(_IMGAEMode_sel)
} _IMGAEMode_sel;
//@}
#define Max_AeMode _IMGAEMode_FaceDetection

//****************************************************************************************************
/**
    @name _IMGAEBreaketing_sel
*/
//@{
/*IMGAEBreaketing*/
typedef enum {
    _IMGAEBreaketing_OFF,
    _IMGAEBreaketing_ON,
    ENUM_DUMMY4WORD(_IMGAEBreaketing_sel)
} _IMGAEBreaketing_sel;
//@}

//****************************************************************************************************
/**
    @name _IMGISO_sel
*/
//@{
/*IMGISO*/
typedef enum {
    _IMGISO_Auto,
    _IMGISO_50,
    _IMGISO_100,
    _IMGISO_200,
    _IMGISO_400,
    _IMGISO_800,
    _IMGISO_1600,
    _IMGISO_3200,
    ENUM_DUMMY4WORD(_IMGISO_sel)
} _IMGISO_sel;
//@}
#define Max_ISO _IMGISO_3200

//****************************************************************************************************
/**
    @name _IMGAFMode_sel
*/
//@{
/*IMGAFMode*/
typedef enum {
    _IMGAFMode_Normal,
    _IMGAFMode_Macro,
    _IMGAFMode_Super_Macro,
    _IMGAFMode_Inifinite,
    _IMGAFMode_ContinueFocus,
    _IMGAFMode_FaceDetection,
    _IMGAFMode_Manual,
    ENUM_DUMMY4WORD(_IMGAFMode_sel)
} _IMGAFMode_sel;
//@}
#define Max_AfMode _IMGAFMode_FaceDetection

//****************************************************************************************************
/**
    @name _IMGAFWindow_sel
*/
//@{
/*IMGAFWindow*/
typedef enum {
    _IMGAFWindow_Center,
    _IMGAFWindow_Auto,
    _IMGAFWindow_ManualPosition,
    ENUM_DUMMY4WORD(_IMGAFWindow_sel)
} _IMGAFWindow_sel;
//@}
#define Max_AFWindow _IMGAFWindow_ManualPosition

//****************************************************************************************************
/**
    @name _IMGAFPosition_sel
*/
//@{
/*IMGAFPosition*/
typedef enum {
    _IMGAFPosition_Center,
    _IMGAFPosition_1,
    _IMGAFPosition_2,
    _IMGAFPosition_3,
    _IMGAFPosition_4,
    _IMGAFPosition_5,
    _IMGAFPosition_6,
    ENUM_DUMMY4WORD(_IMGAFPosition_sel)
} _IMGAFPosition_sel;
//@}

#define Max_AFPosition _IMGAFPosition_4
//****************************************************************************************************
/**
    @name _IMGBurstNum_sel
*/
//@{
/*IMG_BurstNum*/
typedef enum {
    _BurstNum_Dummy = 0,
    _BurstNum_1,
    _BurstNum_2,
    _BurstNum_3,
    _BurstNum_4,
    _BurstNum_5,
    _BurstNum_6,
    _BurstNum_30,
    _BurstNum_Inf,
    _BurstNum_End,
    Max_BurstNum,
    ENUM_DUMMY4WORD(_IMGBurstNum_sel)
} _IMGBurstNum_sel;
//@}

//****************************************************************************************************
/**
    @name _IMGFileFmt_sel
*/
//@{
/*IMG_FileFmt*/
typedef enum {
    _FileFmt_None   =0x0,
    _FileFmt_Raw    =0x1,//bit 0
    _FileFmt_Jpg    =0x2,//bit 1
    _FileFmt_RawJpg =0x3,//bit 0 & 1
    _FileFmt_TIFF   =0x4, //bit 2
    _FileFmt_TIFFJpg =0x6, //bit 0 & 2
    ENUM_DUMMY4WORD(_IMGFileFmt_sel)
} _IMGFileFmt_sel;
#define Max_FileFmt _FileFmt_TIFFJpg
//@}

//****************************************************************************************************
/**
    @name _IMGSiePath_sel
*/
//@{
/*IMG_SiePath*/
typedef enum {
    _PreColorGain,
    _PreGamma ,
    _PostGamma,
    ENUM_DUMMY4WORD(_IMGSiePath_sel)
} _IMGSiePath_sel;
#define Max_SiePath _PostGamma
//@}


//****************************************************************************************************
/**
    @name _IMGSieBitDepth_sel
*/
//@{
/*IMG_SieBitDepth*/
typedef enum {
    _SieBitDepth_8  =8,
    _SieBitDepth_10 =10,
    ENUM_DUMMY4WORD(_IMGSieBitDepth_sel)
} _IMGSieBitDepth_sel;
#define Max_SieBitDepth _SieBitDepth_10
//@}

//****************************************************************************************************
/**
    @name _IMGPreBitDepth_sel
*/
//@{
/*IMG_SieBitDepth*/
typedef enum {
    _PreBitDepth_8  =8,
    _PreBitDepth_10 =10,
    _PreBitDepth_12 =12,
    _PreBitDepth_16 =16,
    ENUM_DUMMY4WORD(_IMGPreBitDepth_sel)
} _IMGPreBitDepth_sel;
//@}

//****************************************************************************************************
/**
    @name _IMGFaceDetectionMode_sel
*/
//@{
/*FaceDetectionMode*/
typedef enum {
    _FaceDetectionMode_Off,
    _FaceDetectionMode_On,
    ENUM_DUMMY4WORD(_IMGFaceDetectionMode_sel)
} _IMGFaceDetectionMode_sel;
//@}
#define Max_FaceDetectionMode _FaceDetectionMode_On

//****************************************************************************************************
/**
    @name _IMGSmileDetectionMode_sel
*/
//@{
/*SmileDetectionMode*/
typedef enum {
    _SmileDetectionMode_Off,
    _SmileDetectionMode_On,
    ENUM_DUMMY4WORD(_IMGSmileDetectionMode_sel)
} _IMGSmileDetectionMode_sel;
//@}
#define Max_SmileDetectionMode _SmileDetectionMode_On

//****************************************************************************************************
/**
    @name _IMGBlinkDetectionMode_sel
*/
//@{
/*BlinkDetectionMode*/
typedef enum {
    _BlinkDetectionMode_Off,
    _BlinkDetectionMode_On,
    ENUM_DUMMY4WORD(_IMGBlinkDetectionMode_sel)
} _IMGBlinkDetectionMode_sel;
//@}
#define Max_BlinkDetectionMode _BlinkDetectionMode_On

//****************************************************************************************************
/**
    @name _IMGDISMode_sel
*/
//@{
/*DISMode (Digital Image Stabilizer mode)*/
typedef enum {
    _DISMode_Off,
    _DISMode_On,
    ENUM_DUMMY4WORD(_IMGDISMode_sel)
} _IMGDISMode_sel;
//@}
#define Max_DISMode _DISMode_On

//****************************************************************************************************
/**
    @name _IMGRatio_sel
*/
//@{
/* pipeline ratio */
typedef enum {
    _Ratio_9_16,
    _Ratio_2_3,
    _Ratio_3_4,
    _Ratio_1_1,
    _Ratio_4_3,
    _Ratio_3_2,
    _Ratio_16_9,
    _Ratio_Num,
    ENUM_DUMMY4WORD(_IMGRatio_sel)
} _IMGRatio_sel;
//@}


//****************************************************************************************************
/**
    @name _DsiplayWindow_sel
*/
//@{
/* pipeline ratio */
typedef enum {
    _WindowFitAuto,
    _WindowFitHori,
    _WindowFitVert,
    _WindowFitMax,
    ENUM_DUMMY4WORD(_DsiplayWindow_sel)
} _DsiplayWindow_sel;
//@}


//****************************************************************************************************
/**
    @name _IMGFlip_sel
*/
//@{
/* image H or V flip */
//#NT#2011/10/26#Ethan Lau -begin
//#NT#expend,220 dsp flip only be supported when sie is in frame mode
typedef enum {
    _Flip_Off,
    _DSP_Flip_H,    //DSP flip function
    _DSP_Flip_V,
    _DSP_Flip_H_V,
    _SEN_Flip_H,    //Sensor flip function
    _SEN_Flip_V,
    _SEN_Flip_H_V,
    _Flip_Max_type,
    ENUM_DUMMY4WORD(_IMGFlip_sel)
} _IMGFlip_sel;
//#NT#2011/10/26#Ethan Lau -end
//@}

//****************************************************************************************************

//#NT#2009/09/29#Bowen Li -begin
//#NT#add BRC function
/**
    @name _IMG_CaptureMode_sel
*/
//@{
typedef enum {
    _CaptureMode_FixQ = 0,
    _CaptureMode_FSizeCtrl = 1,
    _CaptureMode_VAEstCtrl = 2,
    ENUM_DUMMY4WORD(_IMG_CaptureMode_sel)
} _IMG_CaptureMode_sel;
//@}
//#NT#2009/09/29#Bowen Li -end
//****************************************************************************************************

//#NT#2009/10/13#Bowen Li -begin
//#modify capture flow for new memory arrange
//Important Note:
//Must the same sequence and number with struct IPPSizeInfor
/**
    @name _IPPSize_enum
*/
//@{
typedef enum{
    _SieInHsize=0,//must increase from 0
    _SieInVsize,
    _SieXcenter,
    _SieYcenter,
    _SieCropHsize,
    _SieCropVsize,
    _SieOutHsize,
    _SieOutVsize,
    _SieOutLineOffset,
    _SieCropCapHsize,
    _SieCropCapVsize,
    _IpeOutHsize,
    _IpeOutVsize,
    _IpeCapOutHsize,
    _IpeCapOutVsize,
    _ImeOut1Hsize,
    _ImeOut1Vsize,
    _ImeOut1LineOffset,
    _ImeOut2Hsize,
    _ImeOut2Vsize,
    _ImeOut2LineOffset,
    _ImeCapOutHsize,
    _ImeCapOutVsize,
    _ImeCapOutYLineOffset,
    _ImeCapOutCCLineOffset,
    _ImePath1Fmt,
    _ImePath2Fmt,
    _JpgDCoutHsize,
    _JpgDCoutVsize,
    _JpgDCoutYlineoffset,
    _JpgDCoutUVlineoffset,
    _IdeDisplayHsize,
    _IdeDisplayVsize,
    _JpegCapHsize,
    _JpegCapVsize,
    _JpegCapLineOffset,
    _ThumbHsize,
    _ThumbVsize,
    _ThumbLineOffset,
    _ScreenHsize,
    _ScreenVsize,
    _ScreenLineOffset,
    _QVHsize,
    _QVVsize,
    _QVLineOffset,
    _QVPreHsize,
    _QVPreVsize,
    _QVPreYLineOffset,
    _QVPreUVLineOffset,
    _HDActX,
    _HDActY,
    _HDActSizeX,
    _HDActSizeY,
    _HDTotal,
    _HDSync,
    _VDTotal,
    _VDSync,
    _PrvBufEndAddr,
    _CapBufEndAddr,
    _CapTotalField,
    _FQVSampleRate,
    _IMESliceModeH,
    _CAHsize,
    _CAVsize,
    //#NT#2012/02/10#Jarkko Chang -begin
    //#NT#add for dzoom flow
    _CAHsub,
    _CAVsub,
    //#NT#2012/02/10#Jarkko Chang -end
    _CALineOffset,
    _OBWinX0,
    _OBWinX1,
    _OBWinY0,
    _OBWinY1,
    _OBSubSample,
    _OBThreshold,
    _OBGain,
    _OBOffset,
    _SensorRatioIdx,
    _DisDisplayHSize,
    _DisDisplayVSize,
    _DisRecordHSize,
    _DisRecordVSize,
    _HDRightDummy_Prv,
    _VDBottomDummy_Prv,
    _DceAddrOffset,
    _PreD2DOffsetLine,//special application for capture frame mode
    Max_IPPSizeEnum,
    ENUM_DUMMY4WORD(_IPPSize_enum)
}_IPPSize_enum;
//@}
//#NT#2009/10/13#Bowen Li -end

//IPP_FUNC_INFOR: Declared for IPP function integration
/**
    @name IPP_FUNC_INFOR_STRUCT
*/
//@{
typedef struct IPP_FUNC_INFOR_STRUCT
{
    UINT32 DfsEn;              // 0
    UINT32 DfsCnt;             // 1
    UINT32 SIED2DEn;           // 2
    UINT32 NLightCapEn;        // 3
    UINT32 NLightCapSel;       // 4
    UINT32 FQVEn;              // 5
    UINT32 CapISO;             // 6
    UINT32 ChrFiltEn;          // 7
    UINT32 ChrFiltNum;         // 8
    UINT32 FQVInProcess;       // 9
    UINT32 ThumbImgInProcess;  // 10
    UINT32 MainImgInProcess;   // 11
    UINT32 ChrCurveEn;         // 12
    UINT32 *ChrCBCurve;        // 13
    UINT32 *ChrCRCurve;        // 14
    UINT32 *CapGamma;          // 15
    UINT32 YCurveEn;           // 16
    UINT32 *YCurve;            // 17
    UINT32 GEOEn;              // 18
    UINT32 BRCEn;              // 19
    UINT32 FrameMode;          // 20
    UINT32 IfeEmbeddedEn;      // 21
    UINT32 IfeRawEn;           // 22
    UINT32 IfeYccEn;           // 23
    UINT32 IfeFuncTableAddr;   // 24
    UINT32 IfeRawTableAddr;    // 25
    UINT32 IfeYccTableAddr;    // 26
    UINT32 IfeYEn;             // 27
    UINT32 IfeYTableAddr;      // 28
    UINT32 EcsEn;              // 29
    UINT32 VidDatestampEn;     // 30
    UINT32 FQVFrameModeEn;     // 31
    UINT32 JPGDcoutEn;         // 32
    UINT32 JPGEn;              // 33
} IPP_FUNC_INFOR;
//@}

//IPPFuncIDEnum: IPP functions ID
/**
    @name IPPFuncIDEnum
*/
//@{
typedef enum
{
    DFS_EN,                // 0
    DFS_CNT,               // 1
    SIED2D_EN,             // 2
    NLIGHT_CAP_EN,         // 3
    NLIGHT_CAP_SEL,        // 4
    FQV_EN,                // 5 //If Disable,use jpg decode scaling down to produce QV
    CAP_ISO,               // 6
    CHRFILT_EN,            // 7
    CHRFILT_NUM,           // 8
    FQV_INPROCESS,         // 9
    THUMBIMG_INPROCESS,    // 10
    PRIIMG_INPROCESS,      // 11
    CHRCURVE_EN,           // 12
    YCURVE_EN,             // 13
    YCURVE,                // 14
    CHRCBCURVE,            // 15
    CHRCRCURVE,            // 16
    CAPGAMMACURVE,         // 17
    GEO_EN,                // 18
    BRC_EN,                // 19
    FRAME_MODE_EN,         // 20
    IFE_EMBEDDED_EN,       // 21
    IFE_RAW_EN,            // 22
    IFE_YCC_EN,            // 23//CbCr
    IFE_FUNC_TABLE_ADDR,   // 24
    IFE_RAW_TABLE_ADDR,    // 25
    IFE_YCC_TABLE_ADDR,    // 26
    IFE_Y_EN,              // 27//Y
    IFE_Y_TABLE_ADDR,      // 28
    ECS_EN,                // 29
    VID_Datestamp_EN,      // 30
    FQV_FRAMEMDOE_EN,      // 31
    JPG_DCOUT_EN,          // 32
    JPG_EN,                // 33
    IPPFUNC_MAXID,         // 34
    ENUM_DUMMY4WORD(IPPFuncIDEnum)
}IPPFuncIDEnum;
//@}

/**
    @name NlightCapSelEnum
*/
//@{
enum NlightCapSelEnum
{
    NLCap_PrvDataIn,
    NLCap_QVDataIn,
    NLCap_MaxID
};
//@}

/**
    @name IpeD2DParamEnum
*/
//@{
typedef enum
{
    IPED2D_BRC,
    IPED2D_GDC,

    //#NT#2009/11/02#YC Peng -begin
    //#NT# Add ARF Sketch capture new flow
    IPED2D_ARF_SKETCH,
    //#NT#2009/11/02#YC Peng -end

    //#NT#2010/07/13#Linkin Huang -begin
    //#NT# IPE for Y channel
    IPED2D_Y,
    //#NT#2010/07/13#Linkin Huang -end
    IPPD2DPARAM_MAXID,
    ENUM_DUMMY4WORD(IpeD2DParamEnum)
}IpeD2DParamEnum;
//@}

#define BLACK_Y     0
#define BLACK_CB    128
#define BLACK_CR    128

/**
    @name IPE_IME_FM_PARAM
*/
//@{
typedef struct _IPE_IME_FM_PARAM
{
    UINT32 BayerAddr;
    UINT32 BayerHsize;
    UINT32 BayerVsize;
    UINT32 OutputAddr1Y;
    UINT32 OutputAddr1Cb;
    UINT32 OutputAddr1Cr;
    UINT32 ImeHsize1;
    UINT32 ImeVsize1;
    UINT32 ImeFmt1;             //Path1 IME out fmt
    //UINT32 OutputAddr2Y;      ///< 220 have no path2
    //UINT32 OutputAddr2Cb;
    //UINT32 OutputAddr2Cr;
    //UINT32 ImeHsize2;
    //UINT32 ImeVsize2;
    //UINT32 ImeFmt2;             //Path2 IME out fmt
    UINT32 ImeOutPath;
    UINT32 ImeLineoffset1;
    UINT32 ImeLineCCoffset1;
    //UINT32 ImeLineoffset2;
} IPE_IME_FM_PARAM;
//@}

//#NT#2009/07/31#Linkin Huang -begin
//#NT# add enum type, used for function Generic_DataProcess(*)
/**
    @name _IMGQuality_sel
*/
//@{
typedef enum
{
  DataT_Y = 0,
  DataT_Bayer = 1,
  ENUM_DUMMY4WORD(_ProcessedDataType)
} _ProcessedDataType;
//@}

/**
    @name _BayerStartPattern
*/
//@{
typedef enum
{
  Bayer_R   = 0,
  Bayer_Gr  = 1,
  Bayer_Gb  = 2,
  Bayer_B   = 3,
  ENUM_DUMMY4WORD(_BayerStartPattern)
} _BayerStartPattern;
//@}

/**
    @name _GetBayerElement
*/
//@{
typedef enum
{
  BayerE_R   = 0,
  BayerE_Gr  = 1,
  BayerE_Gb  = 2,
  BayerE_B   = 3,
  ENUM_DUMMY4WORD(_GetBayerElement)
} _GetBayerElement;
//@}

/**
    @name _DataBitDepth
*/
//@{
typedef enum
{
  Bit8  = 8,
  Bit12 = 12,
  Bit16 = 16,
  ENUM_DUMMY4WORD(_DataBitDepth)
} _DataBitDepth;
//@}
//#NT#2009/07/31#Linkin Huang -end

//----------Variables and Functions Extern-----------------------------------------------------------

//INT32 Open_Sensor(PSENSOR_APPOBJ pSensorObj);
//extern PSENSOR_APPOBJ   pSensorInfor;
//extern SENSOR_APPOBJ    SensorInfor;
extern PCAPTURE_APPOBJ  pCapInfor;
extern CAPTURE_APPOBJ   CapInfor;


//********AlgCallback Function********
extern FPALGCALLBACK    g_fpAlgExport ;
extern void def_AlgCallBackFun(FPALGCALLBACK pAlgcallback);

//********SIE IPE IME JPG Information********
extern unsigned short JPGIMG_SIZE[Max_ImgSize+1][2];
extern void SetJpgResolution(_IMGSize_sel SizeIdx, UINT32 Hsize, UINT32 Vsize);
extern void SetImeVideoFormat(UINT ImeFmt);
extern UINT GetImeVideoFormat(void);
extern void GetActImgFBAddr_Path1(ACTFBADDR_PRAM *pActFBAddr_para);
extern void GetActImgFBAddr_Path2(ACTFBADDR_PRAM *pActFBAddr_para);
//#NT#2011/04/07#Ethan Lau -begin
//#NT#Package up Subsample out information
extern void Sen_GetSubOutRGBinfo(UINT32* R,UINT32* G,UINT32* B,UINT32 uiSize);
//#NT#2011/04/07#Ethan Lau -end
//#NT#2010/12/31#ethanlau -begin
//#NT#move from Imgslicetsk.c
extern void ImgFQVProcess(ImageAlgInfo *AlgInfo,UINT32 SourceAddr);
//#NT#2010/12/31#ethanlau -end
//#NT#2011/01/11#ethanlau -begin
extern void DecodeDCout(UINT _SurAddr,UINT _TmpAddr,UINT _jpgsize,UINT OutAddr,UINT OutH,UINT OutV,UINT LineOffset);
extern void Exce_IME_D2D(pIME_FBADDR_PRAM _Ime_fb);
//#NT#2011/01/11#ethanlau -end
extern void Sen_SetColorGain(UINT32 Rg,UINT32 Bg,UINT32 Gg);
extern void Sen_GetSubOutRGBinfo(UINT32* R,UINT32* G,UINT32* B,UINT32 uiSize);
extern void Sen_GetSubOutImgInfo(UINT32* ImgH,UINT32* ImgLinoft,UINT32* ImgV,UINT32* Addr,UINT32* Cfa);
//KH mark
#if 0
extern unsigned int GetIpeVACnt(void);
#else
extern UINT32 GetIpeVACnt(void);
#endif
extern void DrawCAWindow(void);
extern void ime_SetDzoom2Video_Path1(pIME_FBADDR_PRAM  pIME_Para1);
extern void ime_SetDzoom2Video_Path1_TV(pIME_FBADDR_PRAM  pIME_Para1);
extern unsigned int    GetIpeOutHsize(void);
extern unsigned int    GetIpeOutVsize(void);
//********AAA and Mode Switch********
extern void sd_SmileDetectionProcess(void);
extern void SetDzoom2Preview(unsigned int ZoomIndex);
extern void SetDzoom2Video(unsigned int ZoomIndex);
extern UB PCC2CAP;
extern ER SliceMode_Process(UINT BayerAddr, UINT JpgDAddr, UINT8 JpgQuality, UINT *JpgSize);
extern ER FrameMode_Process(UINT BayerAddr, UINT JpgDAddr, UINT8 JpgQuality, UINT *JpgSize);
//********ISR and Operation********

extern UINT32 WriteDebugData(unsigned char *Add);
//********Dzoom********
extern BOOL Get_DzoomFlag(void);
extern void Set_DzoomFlag(BOOL CDZoom);
extern void DzoomStop(void);
extern UINT32 Get_DzoomMaxIndex(void);
extern UINT32 Get_DzoomIndex(void);
extern void Set_DzoomIndex(INT idx);
extern UINT32 Get_DzoomRatio(UINT32 DzoomIdx);
//********IQ related********
//extern void AntiFlicker_AE(UINT8 nFlicker);
//extern void SceneMode_AE(unsigned char scene);
//extern void AEMode_AE(UINT8 nMode);
//********UI related********
extern ImageAlgInfo gImageAlgInfo;
//extern SceneModeInfo SceneModeSet;
//#NT#2007/03/05#Bowen Li -end
extern INT32 UI_SetParameter(UB item, UINT32 set_value);
extern ImageAlgInfo* UI_GetParameter(void);
//extern void GetSceneParam(UB mode, UB* Flashmode,UB* EVcomp,UB* WBmode,UB* Saturation,UB* Sharpness);
void UI_Param_Reset(void);
void UI_SetAllParam(void);
extern void     Write_EXIF_Infor(UINT paddress);
extern UINT     GetAlgCapUsedSize(void);
extern void     SetAlgCapUsedSize(UINT value);
extern UCHAR    Get_CaptureDriverMode(void);
extern UINT32   Sensor_IsParameterError(void);
//********flash related*****
/////////////////////////////////////////////////////////////////////////
//#NT#2011/03/23#Harry Tsai -begin
//#NT#move from flash.h
extern void flash_TriggerFlash(void);
extern void flash_ConfigPreFlash(UINT16 uiDelayLine, UINT32 uiOnTimeUs,BOOL uiFlashIMD);
extern void flash_ConfigMainFlash(UINT32 uiDelayLine, UINT32 uiOnTimeUs,BOOL uiFlashIMD);
extern void SetFlashFireStatus(BOOL Fire);
extern BOOL CheckFlashFireStatus(void);
//#NT#2011/03/23#Harry Tsai -end
//********Slice Mode Process********
extern char     message[];
extern void CaptureNIn1 (UINT8 NumBlockX, UINT8 NumBlockY, UINT32 BlockWidth, UINT32 BlockHeight, UINT8 DelayVD);
//********Exif Related********
void Write_AE_EXIF_Infor(UINT paddress);
//********Lens Related********
//zoom
//extern void Zoom_Init_Start(void);
//extern unsigned char Zoom_init_Finish(void);
//extern void zoom_dc_forward_rotation(void);
//extern void zoom_dc_reverse_rotation(void);
//extern unsigned char Get_Zoom_Section(void);
//extern void Zoom_Retract(void);
//#NT#2011/06/27#Chris Chung -begin
//#NT#remove lens related code
#if 0
extern void LensInit(void);
extern void lens_pins_init(void);
extern INT32 LensRetract(void);
extern void ZOOM_RunStep(unsigned char dir,unsigned short step);
extern void FocusPowerOnOff(BOOL On);
extern void Focus_RunStep(unsigned char dir,unsigned short step);
extern BOOL Check_Zoom_PI(void);
extern BOOL Check_Zoom_PR(void);
extern BOOL Check_Focus_PI(void);
//#NT#2007/3/15#Connie Yu -begin
//Clear warnings
extern void Lens_OnOff(unsigned char ON_OFF);
//#NT#2007/3/15#Connie Yu -end
#endif
//#NT#2011/06/27#Chris Chung -end
//********TIFF FileFmt Related********
extern UINT TIFF_RGB_CopyHeader(UINT TIFFHeaderAddr,
                            UINT TIFFHsize,
                            UINT TIFFVsize);


extern UINT SetIPPSizeInfor(UINT item, UINT value);
extern UINT GetIPPSizeInfor(UINT    item);
extern void AeSet_isr(void);

typedef struct _PHOTO_APPOBJ{
    UINT32  uiMemoryAddr;
    UINT32  uiMemorySize;
    void    (*CallBackFunc)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
} PHOTO_APPOBJ,*PPHOTO_APPOBJ;

void ipe_ime_Framemode(IPE_IME_FM_PARAM fmInfo);
extern void SetIPPFuncInfor(UINT32 FuncsID, UINT32 SetValue);
extern UINT32 GetIPPFuncInfor(UINT32 FuncsID);

/***************************************************************************
* Old-APIs(Before 96433) Keep it to avoid compile error                    *
****************************************************************************/

typedef struct    _STAMP_INFOR{
    unsigned int    AddrY;
    unsigned int    AddrCb;
    unsigned int    AddrCr;
    unsigned short  Width;
    unsigned short  Height;
    unsigned char   ColorKeyY;
    unsigned char   ColorKeyCb;
    unsigned char   ColorKeyCr;
    unsigned char   Status;
} STAMP_INFOR, *pSTAMP_INFOR;

typedef struct    _STAMP_SRC_INFOR{
    unsigned int    SrcAddrY;
    unsigned int    SrcAddrCb;
    unsigned int    SrcAddrCr;
    unsigned int    SrcYLineOffset;
    unsigned int    SrcHsize;
    unsigned int    SrcVsize;
    unsigned int    StartH;
    unsigned int    StartV;
} STAMP_SRC_INFOR, *pSTAMP_SRC_INFOR;

extern void DateTimeImprintYCC(STAMP_SRC_INFOR *StampSrcInfo,STAMP_INFOR *StampInfo);
//#NT#2008/09/09#Chris Yang - begin
extern UINT32 IQ_SimCB(UINT8 bReguest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8 *bBuf);
//#NT#2008/09/09#Chris Yang - end
extern UINT8 Sen_GetJpgQuality(void);

typedef enum
{
    SEN_ISR_PHOTO_SIE = 0,
    SEN_ISR_PHOTO_PRE,
    SEN_ISR_PHOTO_DCE,
    SEN_ISR_PHOTO_IPE,
    SEN_ISR_PHOTO_IME,
    //#NT#2009/10/23#Connie Yu -begin
    //#NT#new DIS interface
    SEN_ISR_PHOTODIS_DIS,
    SEN_ISR_PHOTODIS_IPE,
    //#NT#2009/10/23#Connie Yu -end
    //#NT#2011/02/09#ethanlau -begin
    //#NT#IDE fwhandshake interface
    SEN_ISR_PHOTODISP_IDE,
    //#NT#2011/02/09#ethanlau -end
    SEN_ISR_FP_NUM,
    ENUM_DUMMY4WORD(SEN_ISR_FPID)
} SEN_ISR_FPID;
typedef void (*SEN_ISR_FP)(UINT32 uiEventStatus);
ER Sen_IsrRegisterEvent(SEN_ISR_FPID IsrId, SEN_ISR_FP EventFp);
void Sen_IsrEventProc(SEN_ISR_FPID IsrId, UINT32 uiEventStatus);
void Sen_SetPhotoInfo(PHOTO_APPOBJ *PhotoObj);
extern IME_FBADDR_PRAM* Sen_GetImePath1Param(void);
extern IME_FBADDR_PRAM* Sen_GetImePath2Param(void);

UINT8 Sen_GetJpgQuality(void);
UINT32 Sen_GetBurstNum(void);
UINT32 Sen_GetRawNum(void);
UINT32 Sen_GetJpgNum(void);
void Set2SieCap_Init(void);
void Set2SieCap_SetParam(void);
void Set2SieCap(ImageAlgInfo *AlgInfo, UINT32 BufIdx);

//#NT#2011/01/12#ethanlau -begin
//#NT#stage for IPP stop
typedef enum{
    _Start          = 1,
    _IPEIME_PAUSE   = 2,
    _SIE_PAUSE      = 3,
    _IPE_IME_RESET  = 4,
    ENUM_DUMMY4WORD(StopIplStage)
}StopIplStage;
//#NT#2011/01/12#ethanlau -end
//#NT#2011/02/11#ethanlau -begin
UINT32 Sen_GetIPLStopStatus(void);
//#NT#2011/02/11#ethanlau -end
UINT32 Get_HRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
UINT32 Get_VRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
UINT32 Adj_VSize(UINT32 InSize, UINT32 VRatio, UINT32 Align);
UINT32 Adj_HSize(UINT32 InSize, UINT32 HRatio, UINT32 Align);

extern ACTFBADDR_PRAM FB_para1;
extern AlgMsgInfo gAlgMsg;
extern ACTFBADDR_PRAM  FB_para1;
extern UINT JpgSize;
extern UCHAR JPEG_Quality[];
extern unsigned char AEB_Idx;
extern PHOTO_APPOBJ DispSizeInfo;

//#NT#2009/07/30#Linkin Huang -begin
//#NT# add ECS map  rearrange function
//#NT# NOTE that *LUT is a table of 256 levels
//extern UINT32 ECS_ReArragne(UINT32 width, UINT32 height, UINT32 inAddr, UINT32 outAddr);
//#NT#2009/07/30#Linkin Huang -end

//#NT#2009/09/03#Linkin Huang -begin
//#NT# add rearrange function using graphic engine,
//#NT# the original source code was wrote by Jeffery
//extern ER GRAPH_ReArrange(UINT32 width, UINT32 height, UINT32 inAddr);
//#NT#2009/09/03#Linkin Huang -end

//#NT#2009/07/31#Linkin Huang -begin
//#NT# add Generic data process function

//#NT#2009/10/20#Linkin Huang -begin
//#NT# remove no use function
/*
extern UINT32 Generic_DataProcess(UINT32 width, UINT32 height, _ProcessedDataType PDT, _BayerStartPattern BSP, _GetBayerElement GBE, _DataBitDepth DBD, UINT32 inAddr, UINT32 outAddr);
*/
//#NT#2009/10/20#Linkin Huang -end

//#NT#2009/07/31#Linkin Huang -end

/**
    IFE Type Selection
*/
typedef enum
{
    IFE_RAW = 0,
    IFE_YCC = 1,
    IFE_Y   = 2,
    IFE_EMB = 3,
    IFE_MAXMODE = 4,
    ENUM_DUMMY4WORD(IFE_MODE_SEL)
} IFE_MODE_SEL;

typedef struct
{
    IFE_MODE_SEL Mode;
    UINT32 ImgAddr;             ///< input src and final output addr
    UINT32 HSize;
    UINT32 VSize;
    UINT32 InLineOfs;
    UINT32 OutLineOfs;
    UINT32 up_addr;             ///< ending address of previous slice, assign 0 to disable upper source
    UINT32 down_addr;           ///< starting address of next slice, assign 0 to disable down source
    UINT32 BufAddr;             ///< ife tamp buf. or destination buf
    UINT8 WaitEnd;
    void (*pCB)(UINT32,UINT32,UINT32,UINT32);   ///callback timing is just befor ife run last repeat time
} IMG_FILTER_PARAM;
extern ER ImageFilter(IMG_FILTER_PARAM *FilterParam);
extern ER ImageFilterWaitEnd(void);
//#NT#2010/07/20#Linkin Huang -begin
//#NT# add definition
//#NT#2010/08/09/#Linkin Huang -begin
//#NT# arrange parameters as struct for YCC Post Process functions
//N#T# and support 3 YCC formats
typedef struct
{
  UINT32 WidthY;
  UINT32 HeightY;
  UINT32 LineoffsetY;
  UINT32 AddrY;
  UINT32 WidthCx;
  UINT32 HeightCx;
  UINT32 LineoffsetCx;
  UINT32 AddrCb;
  UINT32 AddrCr;
  UINT32 IpeD2DEn;
  void (*YCCPostProcCB)(UINT32 Status);
} ImgYCC_POST_PROCESS_INFO;
//#NT#2009/10/07#Linkin Huang -begin
//#NT# add a new parameter "LineOffset"
extern void ImgYCCPostProcess(ImgYCC_POST_PROCESS_INFO *ImgYCCParam, ImageAlgInfo* AlgInfo);
extern void ImgYCCProc(ImgYCC_POST_PROCESS_INFO *ImgYCCParam);
//#NT#2009/10/07#Linkin Huang -end
//#NT#2010/08/09#Linkin Huang -end
//#NT#2010/07/20#Linkin Huang -end
//#NT#2011/06/08#Ethan Lau -begin
//#NT#for fd lib use
extern UINT32 GetDisplayV(void);
extern UINT32 GetDisplayH(void);
//#NT#2011/06/08#Ethan Lau -end

//#NT#2011/06/14#Ethan Lau -begin
//#NT#Only add for system clk chg,EVB Test only
extern void IPP_StartEng_SysclkTest(void);
extern void IPP_PauseEng_SysclkTest(void);
//#NT#2011/06/14#Ethan Lau -end

//#NT#2012/06/14#Robin Chen -begin
extern void ECS_TableSel(void);
//#NT#2012/06/14#Robin Chen -end

/***************************************************************************
* FILE END                                                                 *
****************************************************************************/
#endif
