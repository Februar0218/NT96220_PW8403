/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       JpgEec.h
    @ingroup    mIAVJPEG

    @brief      Jpeg encoder header

*/

#ifndef _JPGENC_H
#define _JPGENC_H

#include "OSCommon.h"
#include "JpegCommon.h"
#include "Exif.h"
//#NT#2011/07/18#HH Chuang -begin
#include "JPEGBRC.h"
//#NT#2011/07/18#HH Chuang -end

/** \addtogroup mIAVJPEG
*/
//@{

#define JPG_Enc_Thumbnail           0x00000010
//#NT#In order to add a hidden thumbnail image inside MakerNote, we add one option JPG_Enc_ThumbnailB
#define JPG_Enc_ThumbnailB          0x00000020  //JPG_Enc_ThumbnailB including JPG_Enc_Thumbnail

#define JPGHeaderSize               sizeof(JPG_HEADER)

#define THUMBNAIL_WIDTH             160     //320
#define THUMBNAIL_HEIGHT            120     //240
#define VERSION_CODE_ADDRESS        0x80004028

//Add for JPEG file size control
#define JPEG_QVAL_MINIMUM           31

//#NT#2009/07/26#Daniel -begin
//#Add for JPEG Bit Rate Control
#define JPG_Enc_FileSZCtl           0x00000100
#define JPG_Enc_VAEstCtl            0x00000200
#define JPG_Enc_RhoBRCtl            0x00000400

//#NT#2011/07/05#Ben Wang -begin
//#NT#Add ring buffer mechanism for JPEG bitstream
//parameter for JPG_DBG_LEVEL
#define JPG_ENC_BS_DBG_NONE         0x00000000
#define JPG_ENC_BS_DBG_FRAME        0x00000001
#define JPG_ENC_BS_DBG_ISR          0x00000002
#define JPG_ENC_BS_DBG_QVALUE       0x00000004
//#NT#2011/07/05#Ben Wang -end


//#NT#2011/07/12#Meg Lin -begin
#define EXIFENCHID_INMARKER         0x00000001
#define EXIFENCHID_PADAFTERBS       0x00000002
//#NT#2011/07/12#Meg Lin -end
typedef enum
{
    JPEG_VID_QV5 = 0,
    JPEG_VID_MOV = 1,
    ENUM_DUMMY4WORD(JPG_VID_MODE)
} JPG_VID_MODE;

typedef enum
{
    JPG_CHANNEL_ALL,
    JPG_CHANNEL_Y,
    JPG_CHANNEL_U,
    JPG_CHANNEL_V,
    ENUM_DUMMY4WORD(JPG_CHANNEL)
} JPG_CHANNEL;

/*
    JPEG planar info

    (for bit rate control path)
*/
typedef struct
{
    UINT32 uiImgAddr;           ///< Planar raw image address
    UINT32 uiLineOffset;        ///< Planar raw image line offset
} JPG_BRC_PLANARINFO;

/*
    JPEG image info

    (for bit rate control path)
*/
typedef struct
{
    JPG_BRC_PLANARINFO yPlanarInfo; ///< Y channel planar info
    JPG_BRC_PLANARINFO uPlanarInfo; ///< U channel planar info
    JPG_BRC_PLANARINFO vPlanarInfo; ///< V channel planar info
} JPG_BRC_IMGINFO, *PJPG_BRC_IMGINFO;
//#NT#2011/07/05#Ben Wang -begin
//#NT#Add ring buffer mechanism for JPEG bitstream
typedef enum
{
    JPG_TARGET_FRAME_SIZE,
    JPG_MAX_VALID_FRAME_SIZE_RATIO,  //100~200, unit:%, base on JPG_TARGET_FRAME_SIZE
    JPG_BRC_WEIGHTING,               //0~100
    JPG_MAX_FRAME_SIZE,              //bitstream buffer allocation should base on this value
    JPG_DBG_LEVEL,
//#NT#2011/08/15#Ben Wang -begin
//#NT#Refine API for the boundary of target size
    JPG_TARGET_SIZE_UPPER_BOUND,     //100~, unit:%, base on JPG_TARGET_FRAME_SIZE
    JPG_TARGET_SIZE_LOWER_BOUND,     //0~100, unit:%, base on JPG_TARGET_FRAME_SIZE
//#NT#2011/08/15#Ben Wang -end
    ENUM_DUMMY4WORD(JPG_ENC_DATA_SET)
} JPG_ENC_DATA_SET;
//#NT#2011/07/05#Ben Wang -end

//#NT#2011/07/18#HH Chuang -begin
/*
typedef enum
{
    BRC_COMBINED_YUV    = 0,
    BRC_SEPERATE_YUV420 = 1,
    BRC_SEPERATE_YUV422 = 2,
    ENUM_DUMMY4WORD(BRC_INPUT_RAW)
} BRC_INPUT_RAW;
*/
//#NT#2011/07/18#HH Chuang -end
/*
typedef struct
{
    UINT32 Width;                           ///< Raw Width
    UINT32 Height;                          ///< Raw Height
    UINT32 YEncByte;                        ///< Y Channel Encode BS Size
    UINT32 UEncByte;                        ///< U Channel Encode BS Size
    UINT32 VEncByte;                        ///< V Channel Encode BS Size
    UINT32 YUVEncByte;                      // YUV Channel Encode BS Size

    UINT32 UBoundByte;                      // BRC Up Bound BS Size
    UINT32 TargetByte;                      ///< Bit Rate Control Target BS Size
    UINT32 LBoundByte;                      // BRC Low Bound BS Size

    UINT32 Reserved[32];                    ///< Reserved for Bit Rate Control Information
} BRCParam, *PBRCParam;
*/
#if 0
typedef struct
{
    UINT32 Width;                           ///< Raw Width
    UINT32 Height;                          ///< Raw Height
    UINT32 YEncByte;                        ///< Y Channel Encode BS Size
    UINT32 UEncByte;                        ///< U Channel Encode BS Size
    UINT32 VEncByte;                        ///< V Channel Encode BS Size
    UINT32 TargetByte;                      ///< Bit Rate Control Target BS Size
    UINT32 Reserved[32];                    ///< Reserved for Bit Rate Control Information
} BRCParam, *PBRCParam;
#endif
//#NT#2009/07/26#Daniel -end

typedef struct
{
    UINT32 TargetByte;
    UINT32 YVA[64];
    UINT32 UVA[64];
    UINT32 VVA[64];
    UINT32 RCBound[64];
    UINT32 RCMCURate[64];
} VACmpParam, *PVACmpParam;

//DC Output Mode Start Address Structure
typedef struct
{
    UINT32  DCYAddr;
    UINT32  DCUAddr;
    UINT32  DCVAddr;

} JPGLIB_DC_OUT_ADDR,*PJPGLIB_DC_OUT_ADDR;

//DC Output Mode Configure Structure
typedef struct
{
    UINT32  DCEnable;

    JPG_YUV_FORMAT  YUVFormat;

    UINT32  FrameWidth;
    UINT32  FrameHeight;

    UINT32  PanelWidth;
    UINT32  PanelHeight;

    UINT32  DCYAddr;
    UINT32  DCUAddr;
    UINT32  DCVAddr;

    UINT32  DCWidth;            // Return HW condition to API
    UINT32  DCHeight;           // Return HW condition to API

    UINT32  DCYLineOffset;      // Return HW condition to API
    UINT32  DCULineOffset;      // Return HW condition to API
    UINT32  DCVLineOffset;      // Return HW condition to API

    UINT32  DCXRatio;
    UINT32  DCYRatio;

} JPGLIB_DC_OUT_CFG,*PJPGLIB_DC_OUT_CFG;

//Slice Mode Configure Structure
typedef struct
{
    UINT32  FrameWidth;
    UINT32  FrameHeight;
    UINT32  BSBufAddr;
    UINT32  BSBufSize;
    BOOL    SliceMode;
    JPG_YUV_FORMAT  YUVFormat;
    JPG_BRC_MODE    BrcMode;
    UINT32  RestartMarkerNum;
    void    (*FP_JPGLIB_CB)(UINT32 IntStatus); // ISR callback function
} JPGLIB_CFG,*PJPGLIB_CFG;

//#NT#2011/06/17#Ben Wang -begin
//#NT#refine UserJpeg API
//Video Mode Trigger Structure
typedef struct
{
    UINT32  YAddr;
    UINT32  UAddr;
    UINT32  VAddr;
    UINT32  SliceHeight;
} JPGLIB_ENC_TRIG,*PJPGLIB_ENC_TRIG;
//#NT#2011/06/17#Ben Wang -end

//Bitstream Information Structure
typedef struct
{
    UINT32 BSInfoAddr1;
    UINT32 BSInfoSize1;

    UINT32 BSInfoAddr2;
    UINT32 BSInfoSize2;

} JPGLIB_BS_INFO,*PJPGLIB_BS_INFO;

//Bitstream Buffer Structure
typedef struct
{
    UINT32  Start;
    UINT32  Tail;

    UINT32  TotalSize;
    //UINT32  UsedSize;//#NT#2011/07/05#Ben Wang
    UINT32  AvailSize;

} JPG_BS_BUF,*PJPG_BS_BUF;


/**
    JPEG Encode Parameter.
*/
typedef struct
{
    UINT    RawImageAddr[3];                ///< Raw Image Y address. CB CR will following this address.

    UINT    JPGBSAddr;                      ///< Compressed data ouput location.
    UINT    *JPGBSBufSize;                  ///< Buffer Size allocated for JPG ouput bitstream. (IN/OUT Parameter)
//#NT#2011/06/16#Ben Wang -begin
//#NT#refine UserJpeg API
//#NT#2007/01/11#Scottie -begin
//#Add for supporting JPEG-DCOutput.
    //UINT32  JPGDCAddr[3];                   ///< JPEG-DCOutput_Y location. CB CR will following this address.
    //BOOL    bOuputJpgDC;                    ///< Enable 1/8 scaling down output (JPEG-DCOutput).
//#NT#2007/01/11#Scottie -end
//#NT#2011/06/16#Ben Wang -end
    UINT    Width;                          ///< Raw Image Width, This value should be 16-multiplier.
    UINT    Height;                         ///< Raw Image Height, This value should be 8 or 16 multiplier, depends on output bs format.
    UINT    Mode;                           ///< Encoding Mode. Including Slice/JPG BS format
    UINT32  LineOffsetY;
    UINT32  LineOffsetU;
    UINT32  LineOffsetV;
    //UINT32  LineOffsetCbCr;
    //PVACmpParam pVAParam;
    //#NT#2011/06/16#Ben Wang -end
    JPGLIB_DC_OUT_CFG  DCOutCfg;
    UINT32  RestartMarkerNum;
    UINT32  VideoMode;
} JPGCmpParam, *PJPGCmpParam;
//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
typedef struct
{
    UINT32  frameCount;
    UINT32  baseOffset;
    UINT32  addOffset;
}  MJPEG_MOVVIDEOINFO;
//#NT#2007/10/17#Meg Lin -end
/**
    JPEG Encode parameter.
    Recommend to use JPGCmpParam replace EXIFEncParam.
*/
typedef struct
{
    UINT    RawImageAddr_Y;                 ///< Thumbnail Raw Image Address for Y,  size 160*120. (width*height)
    UINT    RawImageAddr_CB;                ///< Thumbnail Raw Image Address for CB, size depends on format.
    UINT    RawImageAddr_CR;                ///< Thumbnail Raw Image Address for CR,

    UINT    EXIFBSAddr;                     ///< EXIF Header.
    UINT    *EXIFBufSize;                   ///< Buffer Size allocated for EXIF ouput bitstream. (IN/OUT Parameter)
    //#NT#2011/06/16#Ben Wang -begin
    //#NT#refine UserJpeg API                                            ///< This variable will also return the output result buffer size!!
//#NT#2007/02/13#Scottie -begin
//#Add for supporting JPEG-DCOutput.
    //UINT32  JPGDCAddr[3];                   ///< JPEG-DCOutput_Y location. CB CR will following this address.
    //BOOL    bOuputJpgDC;                    ///< Enable 1/8 scaling down output (JPEG-DCOutput).
//#NT#2007/02/13#Scottie -end
    UINT    Width;                          ///< Raw Image Width, This value should be 16-multiplier. Remark:This value is ORIGINAL image Size!!
    UINT    Height;                         ///< Raw Image Height, Remark:This value is ORIGINAL image Size!!
    UINT    Mode;                           ///< Thumbnail Encoding Mode.
    UINT32  LineOffsetY;
    UINT32  LineOffsetU;
    UINT32  LineOffsetV;
    //UINT32  LineOffsetCbCr;
//#NT#Modified, EXIFEncParam head declare must be sync with JPGCmpParam
    //PVACmpParam pVAParam;
    //#NT#2011/06/16#Ben Wang -end
    JPGLIB_DC_OUT_CFG  DCOutCfg;
    UINT32  RestartMarkerNum;
    UINT32  HiddenMode;                     ///< EXIFENCHID_INMARKER, EXIFENCHID_PADAFTERBS, 2011/07/12 Meg
} EXIFEncParam, *PEXIFEncParam;


/**
    Supporting Hidden Thumbnail Images.
    This structure used for JpegCopyEXIFHeader only.
*/
typedef struct
{
    UINT32    *EXIFIMGAddr;                 ///< EXIF IMAGE START ADDRESS.

    UINT32    *EXIFIMGBufSize;              ///< Buffer Size allocated for WHOLE EXIF ouput bitstream. (IN/OUT Parameter)
                                            ///< This variable will also return the output result buffer size!!
} EXIFIMGPARAM, *PEXIFIMGPARAM;

//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag

/**
    Exif GPS tag information structure.
*/
typedef struct
{
    UINT    Latitude_DegNum;
    UINT    Latitude_DegDenum;
    UINT    Latitude_SecNum;
    UINT    Latitude_SecDenum;
    UINT    Latitude_MinNum;
    UINT    Latitude_MinDenum;

    UINT    Longitude_DegNum;
    UINT    Longitude_DegDenum;
    UINT    Longitude_SecNum;
    UINT    Longitude_SecDenum;
    UINT    Longitude_MinNum;
    UINT    Longitude_MinDenum;

    UINT8   LatitudeRef;                            //N or S
    UINT8   LongitudeRef;                           //E or W
    UINT16  Padding;
} EXIFGPSINFO, *PEXIFGPSINFO;
#endif
//#NT#2009/03/18#Photon Lin -end
//#NT#2011/08/15#Ben Wang -begin
//#NT#Refine API for the boundary of target size
typedef struct
{
    BOOL            bBrcEn;             //Bit rate control enable/disable
    UINT32          uiInitialQF;        //Initial QF value in bit rate control
    UINT32          uiTargetSize;       //target compressed size when bit rate control is enabled
    UINT32          uiLowBoundSize;     //bit stream lower boundary size(error tolerance)
    UINT32          uiUpBoundSize;      //bit stream upper boundary size(error tolerance)
    JPG_YUV_FORMAT  yuvFormat;          //YUV format
    UINT32          uiWidth;            //image width
    UINT32          uiHeight;           //image height
} JPG_BRC_CFG, *PJPG_BRC_CFG;
//#NT#2011/08/15#Ben Wang -end
/**
@name JPEG compression quality setting.
    These values are predefined meaning with specific range.
*/
//@{
#define  Quality_Undefine           0       ///< use default quality value
#define  Quality_SuperHigh          1       ///< uiQuality as 0, 97~95
#define  Quality_High               2       ///< uiQuality as 94~90
#define  Quality_Economy            3       ///< uiQuality as 89~80
//@}

//#NT#2007/03/06#Scottie -begin
/**
@name JPEG quantization value macros.
    These values are predefined meaning with specific compression ratio (vs YUV422 RAW Data).
*/
//@{
#define  JPEG_Q_NORMAL_2P7          98      ///< normal situation, compression ratio: 2.7x
#define  JPEG_Q_NORMAL_4            95      ///< normal situation, compression ratio: 4x
#define  JPEG_Q_NORMAL_6            90      ///< normal situation, compression ratio: 6x
#define  JPEG_Q_NORMAL_7            87      ///< normal situation, compression ratio: 7x
#define  JPEG_Q_NORMAL_8            84      ///< normal situation, compression ratio: 8x

#define  JPEG_Q_PORTRAIT_2P7        99      ///< Portrait, compression ratio: 2.7x
#define  JPEG_Q_PORTRAIT_4          96      ///< Portrait, compression ratio: 4x
#define  JPEG_Q_PORTRAIT_6          93      ///< Portrait, compression ratio: 6x
#define  JPEG_Q_PORTRAIT_7          92      ///< Portrait, compression ratio: 7x
#define  JPEG_Q_PORTRAIT_8          90      ///< Portrait, compression ratio: 8x

#define  JPEG_Q_LANDSCAPE_2P7       97      ///< Landscape, compression ratio: 2.7x
#define  JPEG_Q_LANDSCAPE_4         94      ///< Landscape, compression ratio: 4x
#define  JPEG_Q_LANDSCAPE_6         85      ///< Landscape, compression ratio: 6x
#define  JPEG_Q_LANDSCAPE_7         81      ///< Landscape, compression ratio: 7x
#define  JPEG_Q_LANDSCAPE_8         78      ///< Landscape, compression ratio: 8x
//@}
//#NT#2007/03/06#Scottie -end


//#NT#2007/03/20#KH Chang -begin
//#JPEG header with/without huffman table
/**
@name JPEG Header with/without huffman table.

*/
//@{
#define  JPEG_HDR_HUFFMANTBL_WITHOUT        0
#define  JPEG_HDR_HUFFMANTBL_WITH           1
//@}
//#NT#2007/03/20#KH Chang -end

//
//  Not support yet... wil be added in near future.
//
//---- Functions relesse -------------------------------

extern void     JPEGWriteAPP1Orientation(UINT, UINT32);
extern void     JPEGWriteAPP1ExposureTime(UINT, UINT, UINT);
extern void     JPEGWriteAPP1FNumber(UINT, UINT, UINT);
extern void     JPEGWriteAPP1ShutterSpeed(UINT, INT, INT);
extern void     JPEGWriteAPP1Aperture(UINT, UINT, UINT);
extern void     JPEGWriteAPP1MaxAperture(UINT, UINT, UINT);
extern void     JPEGWriteAPP1FocalLength(UINT, UINT, UINT);
extern void     JPEGWriteAPP1ExposureBias(UINT, INT, INT);
extern void     JPEGWriteAPP1ExposureProgram(UINT, INT);
extern void     JPEGWriteAPP1ISOSpeed(UINT, INT);
extern void     JPEGWriteAPP1LightSource(UINT, INT);
extern void     JPEGWriteAPP1StrobeFlash(UINT, INT);

#if _Support_Brightness_Tag
extern void     JPEGWriteAPP1BrightnessValue(UINT, INT, INT);
#endif

extern void     JPEGWriteAPP1ExposureMode(UINT, INT);
extern void     JPEGWriteAPP1SensingMethod(UINT, INT);
extern void     JPEGWriteAPP1MeteringMode(UINT, INT);
extern void     JPEGWriteAPP1WhiteBalance(UINT, INT);
//#NT#2007/07/16#Scottie -begin
//#Add for O project
extern void     JPEGWriteAPP1DZRatio(UINT, UINT, UINT);
//#NT#2007/07/16#Scottie -end
extern void     JPEGWriteAPP1SceneCaptureType(UINT, INT);
//#NT#2007/07/16#Scottie -begin
//#Add for O project
extern void     JPEGWriteAPP1GainControl(UINT, INT);
extern void     JPEGWriteAPP1Contrast(UINT, INT);
extern void     JPEGWriteAPP1Saturation(UINT, INT);
extern void     JPEGWriteAPP1Sharpness(UINT, INT);
//#NT#2007/07/16#Scottie -end
extern void     JPEGWriteAPP1CustomRendered(UINT, INT);
extern void     JPEGWriteJPGQulaity(UINT, UCHAR );

extern void     JPEGWriteAPP1ADC(UINT , USHORT);
extern void     JPEGWriteAPP1Iteration(UINT, USHORT);

extern ER       JpegEncEXIFHeader(PEXIFEncParam pEXIFParam);

extern void     JpegEncStart(void);
extern void     JpegEncFinish(void);
extern ER       JpegEncOnePic(PJPGCmpParam JpgParam, UINT uiQuality);
//#NT#2009/07/26#Daniel -begin
//#Add for JPEG HW Bit Rate Control
extern ER       JpegEncOneChannel(PJPGCmpParam pJPGParam, UINT16 uiQuality, JPG_CHANNEL Channel);
extern ER       JpegWaitChannelDone(JPG_CHANNEL Channel, PBRCParam pBRCParam);
extern void     JpegSetEncQTable4RhoBRC(UINT32 uiQF, UINT8 *pQTable_Y, UINT8 *pQTable_UV);
//#NT#2009/07/26#Daniel -end
extern void     JpegInit(void);

extern ER       JpegCopyEXIFHeader(PEXIFEncParam pEXIFParam);

// In order to add a hidden thumbnail image inside MakerNote, we modify the prototype of JpegCopyEXIFHeader
// If you don't have the second hidden thumbnail, set pEXIFHIDDENImg= NULL
// pEXIFDCFThumnail could not be NULL.
extern ER       JpegCopyEXIFHeader2(PEXIFIMGPARAM pEXIFIMG, PEXIFEncParam pEXIFDCFThumnail, PEXIFEncParam pEXIFHIDDENImg);

// In order to add debug message into MakerNote data area, we provide this new API for the caller
// to know where it can put its data. (maxmium bytes is MakeNoteLen_DEBUGBUFSIZE bytes)
extern UINT32   JpegGetEXIFHeaderMakeNoteAddr(UINT32 EXIFAddrs);

extern ER       JpegEncOneVideoFrame(PJPGCmpParam JpgParam, UINT8 *QTable, BOOL bHuffmanTbl);
//#NT#2006/08/02#Eric Wann - End

//#NT#2008/08/29#Scottie -begin
//#Add for JPEG bit-stream size control
extern void     JpegUpdateQtable4Photo(UINT uiQuality, UINT8 *QTable);
extern void     JpegInitQValue4Photo(UINT uiQuality);
//#NT#2010/06/03#Scottie -begin
//#NT#Support "Bit Rate Control" mechanism for Playback re-encoding
extern void     JpegSetEncQTable(UINT32 uiQuality, UINT8 *QTable, UINT8 *QTable_UV);
//#NT#2010/06/03#Scottie -end
//#NT#2008/09/12#Scottie -begin
//#Modify 2nd parameter
//extern void     JPEG_RCInit_Photo(UINT32 BitRate, UINT8 QPVal);
extern void     JPEG_RCInit_Photo(UINT32 BitRate, UINT32 ulQPVal);
//#NT#2008/09/12#Scottie -end
extern UINT32   JPEG_RCCal_Photo(UINT32 uiFILE_LEN, UINT32 uiFineTuneVal);
//#NT#2008/08/29#Scottie -end
extern void   JPEG_RCQInit_Photo(UINT32 ulQPVal);
extern UINT32 JPEG_RCEstimate(UINT32 EstSize, UINT64 TarBitRate);
extern UINT32 JpegVAtoRC(PJPGCmpParam pJpgParam, PVACmpParam pVAParam);


//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
extern void  JPEGWriteAPP1GPSInfo(PEXIFGPSINFO pGPSInfo);
//extern void  JPEGWriteAPP1GPSInfo(UINT pAPP1, PEXIFGPSINFO pGPSInfo);
//#NT#2009/03/18#Photon Lin -end
#endif
//#NT#2007/3/15#Connie Yu -begin
//Clear warnings
ER JpegEncThumbnail(PEXIFEncParam pEXIFParam, UINT uiQuality);
//#NT#2007/3/15#Connie Yu -end
//---- data relesse ------------------------------------

//#NT#2007/08/17#Scottie -begin
//#Add for checking system time if it is set..
extern BOOL     g_bIsSysTimeSet;
//#NT#2007/08/17#Scottie -end

//------------------------------------------------------

extern ER   JpegGetDCOutConfig(PJPGLIB_DC_OUT_CFG JpgDCOut);
extern ER   JpegEncDCOutConfig(PJPGLIB_DC_OUT_CFG JpgDCOut);
extern void JpegSetDCOutAddr(PJPGLIB_DC_OUT_ADDR JpgDCOutAddr);
extern void JpegGetDCOutAddr(PJPGLIB_DC_OUT_ADDR JpgDCOutAddr);
//#NT#2011/06/17#Ben Wang -begin
//#NT#refine UserJpeg API
extern ER   JpegEncVidConfig(PJPGLIB_CFG JpgCfg);
extern ER   JpegEncVidTrigger(PJPGLIB_ENC_TRIG pJpgTrig);
extern ER   JpegGetBitStream(PJPGLIB_BS_INFO pBSInfo);
extern ER   JpegEncRSTHeight(UINT32 Height);
//extern void JpegEncBSBufInit(PJPGLIB_CFG JpgCfg);
//extern void JpegEncBSBufCheck(void);
extern void JPEGGetAPP1DateTime(RTCStruct *pRTC);//2011/06/10 Meg, move to here

extern void JpegEncSetData(JPG_ENC_DATA_SET attribute, UINT32 Value);
extern UINT32 JpegEncGetData(JPG_ENC_DATA_SET attribute);
//#NT#2011/06/17#Ben Wang -end

//#NT#2011/07/18#HH Chuang -begin
//#NT#2011/08/15#Ben Wang -begin
//#NT#Refine API for the boundary of target size
extern ER JpegSetBitRateCtrl(PJPG_BRC_CFG pBrcCfg);
//#NT#2011/08/15#Ben Wang -end
extern ER JpegEstimateBitRate(JPG_CHANNEL yuvChannel, JPG_YUV_FORMAT yuvFormat, UINT32 uiWidth, UINT32 uiHeight,
                        PJPG_BRC_IMGINFO pImgInfo);
extern ER   JpegRhoBRCGetQF(BRC_INPUT_RAW InputRaw, UINT32 *pQF, UINT32 BSSize);
//#NT#2011/07/18#HH Chuang -end
//@}

#endif // _JPGENC_H
