/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       JpgParseHeader.h
    @ingroup    mIAVJPEG

    @brief      Jpeg parser header

*/


#ifndef _JPEGPARSEHEADER_H
#define _JPEGPARSEHEADER_H

#include "Type.h"
#include "Exif.h"

/** \addtogroup mIAVJPEG
*/
//@{

/**
    Decode JPEG type.

    One jpeg file contains one primary image, one 160x120 thumbnail image,
    and one hidden 320x240 thumbnail image.
    For MJPEG AVI file, the frame inside also a JPEG file.
*/
typedef enum {
    DEC_PRIMARY,        ///<0x00, decode primary image
    DEC_THUMBNAIL,      ///<0x01, decode thumbnail, or decode primary image
    DEC_AVI,            ///<0x02, decode MJPEG AVI first frame
    DEC_HIDDEN,         ///<0x03, decode hidden thumb, or decode primary image
    DEC_THUMB_ONLY,     ///<0x04, decode thumbnail only
    DEC_MOVMJPG         ///<0x05, decode motion jpg MOV
} JPG_DEC_TYPE;

/**
    Parse EXIF Information.

    These elements are filled with EXIF information in JPEG header parsing.
*/
typedef struct
{
    UINT32          OffsetQTabY;
    UINT32          OffsetQTabUV;
    UINT32          OffsetAPP1Size;
    UINT32          OffsetThumbJPGSize;
    UINT32          ThumbAPP1Size;
    UINT32          ThumbExifHeaderSize;
    UINT32          ThumbJPGHeaderSize;
    UINT32          ThumbBitstreamSize;
    UINT32          OffsetEDITTYPE;
    UINT32          OffsetPLAYPIC_FORMAT;
    UINT32          OffsetPLAYPIC_OFFSET;
    UINT32          OffsetPLAYPIC_LENGTH;
    UINT32          OffsetHiddenJPG;
    UINT32          OffsetHiddenQTabY;
    UINT32          OffsetHiddenQTabUV;
    UINT32          ThunbJPGSize;
    UINT32          OffsetJPEGCSRate;
    UINT32          MakerNoteOffsetVal;
    UINT32          OffsetFFD8;
    UINT32          ScreeNailSize;
    UINT32          OffsetMakerNoteSize;
    UINT32          MakerNoteSize;
    UINT32          MakerNoteVer;
    UCHAR           *pTagMakerNote;
    UINT32          Reserved1;

    UCHAR           ModelStr[32];                   //Model (tag:0x0110)
    UINT32          Orientation;                    //Orientation (tag:0x0112)
    UINT32          OffsetPixelXDimension;
    UINT32          OffsetPixelYDimension;
    UCHAR           JPGQuality;
    UCHAR           ByteOrder;
    UCHAR           MakerNoteConfirm;
    UCHAR           Reserved[1];

    UINT32          ISOValue;                       //ISO speed ratings (tag:0x8827)
    UINT32          ExpProg;                        //Exposure Program. It's index. (tag:0x8822)
    UINT32          ImageWidth;                     //Not necessary information in EXIF.
    UINT32          ImageHeight;                    //We will provide this information as we can, otherwise 0.
    UINT32          WhiteBalance;                   //White Balance Index, (tag:0xA403)
    UINT32          MeteringMode;                   //Metering Mode, Index. (tag:0x9207)
//#NT#2007/09/05#Scottie -begin
//#Add EXIF-Sharpness tag
    UINT32          Sharpness;                      //Sharpness Index, (tag:0xA40A)
//#NT#2007/09/05#Scottie -end
    UCHAR           ExpBiasValue[6];                //ExposureBiasValue (tag:0x9204) -99.9 ~ 99.9
    UINT32          ExposureMode;                   //Exposure Mode. Index. 0: Auto, 1:Manual, 2: Auto bracket (tag:A402)
    UINT32          StrobeFlash;                    //StrobeFlash (tag:0x9209)
    UCHAR           ExposureTime[8];                //ExposureTime (tag:0x829A)  Format: X/XX
    UCHAR           FNumber[8];                     //F number (tag:0x829D)  Format: X.X
    UCHAR           ApertureValue[8];               //Aperture (tag:0x9202)  Format: X.X
    RTCStruct       CaptureTime;                    //20 bytes string, zero-end string. define in EXIF.h

//#NT#2009/06/03#Scottie -begin
//#NT#Add for PrintIM3
    UINT32          PrintIMOffsetVal;               //PrintIM (tag:0xC4A5)
    UCHAR           *pTagPrintIM;
//#NT#2009/06/03#Scottie -end

    //#NT#2009/03/18#Photon Lin -begin
    UINT32          GPSVer;
    UCHAR           GPSLatRef;
    UCHAR           GPSLonRef;
    UINT32          GPSLatNum1;
    UINT32          GPSLatNum2;
    UINT32          GPSLatNum3;
    UINT32          GPSLatDenNum1;
    UINT32          GPSLatDenNum2;
    UINT32          GPSLatDenNum3;
    UINT32          GPSLonNum1;
    UINT32          GPSLonNum2;
    UINT32          GPSLonNum3;
    UINT32          GPSLonDenNum1;
    UINT32          GPSLonDenNum2;
    UINT32          GPSLonDenNum3;
    //#NT#2009/03/18#Photon Lin -end

//#NT#2011/06/08#Meg Lin -begin
    //#NT#Add User Comment tag
    char            UserComment[TagUserCommentLen];
//#NT#2011/06/08#Meg Lin -end

} EXIFHeaderInfo, *PEXIFHeaderInfo;

/**
    JPEG decode configuration.
*/
typedef struct _JPGHEAD_DEC_CFG{
    UINT    headerlen;          ///< length of header
    UCHAR   *inbuf;             ///< input buffer start address
    UCHAR   *pQTabY;            ///< start addr of 0th-Quantization table
    UCHAR   *pQTabUV;           ///< start addr of 1st-Quantization table
//#NT#2010/06/03#Scottie -begin
//#NT#Support Quantization table 2
    UCHAR   *pQTabUV2;          ///< start addr of 2nd-Quantization table
//#NT#2010/06/03#Scottie -end
    UCHAR   *pHuffDC0th;        ///< start addr of 0th-Huff-DC table
    UCHAR   *pHuffDC1th;        ///< start addr of 1st-Huff-DC table
    UCHAR   *pHuffAC0th;        ///< start addr of 0th-Huff-AC table
    UCHAR   *pHuffAC1th;        ///< start addr of 1st-Huff-AC table
    UCHAR   *pTagSOF;           ///< start addr of SOF (image-width, image-height, image-format)
    USHORT  imageheight;        ///< image height
    USHORT  imagewidth;         ///< image width
    USHORT  numcomp;            ///< number of components
    USHORT  scanfreq[3][2];     ///< h and v freqs for each of 3 components
    USHORT  qtype[3];           ///< quantization table type for each component
    UINT32  fileformat;         ///< 0->YUV422 ; 1->YUV420 ;
    USHORT  rstinterval;        ///< restart interval (initially set to 0)
    USHORT  lineoffsetY;        ///< Y  line offset after scaling
    USHORT  lineoffsetUV;       ///< UV line offset after scaling
    USHORT  ori_imageheight;    ///< ori-image height before scaling
    USHORT  ori_imagewidth;     ///< ori-image width  before scaling
//#NT#2008/12/03#Scottie -begin
//#NT#Add for new Screennail decoding mechanism
    BOOL    bBeforeDec;
    BOOL    bIfSpeedUpSN;
//#NT#2008/12/03#Scottie -end

    PEXIFHeaderInfo pExifInfo;

//#NT#2009/12/23#Scottie -begin
//#NT#Add for getting video frame rate
    UINT32  uiVideoFrameRate;
//#NT#2009/12/23#Scottie -end
} JPGHEAD_DEC_CFG, *PJPGHEAD_DEC_CFG;

//---- Functions relesse -------------------------------
//Add for checking the image format if supported
BOOL    jpeg_IsSupportedFormat(USHORT usNumComp, USHORT usScanFreq[3][2], UINT32 *pusFileFormat);
UINT    JpegDecParseHeader(JPGHEAD_DEC_CFG *, JPG_DEC_TYPE);
UCHAR   GetCurByte(UCHAR **);
USHORT  GetCurSHORT(UCHAR **, BOOL);
UINT    GetCurUINT(UCHAR **, BOOL);

UINT    Parse_Exif(UCHAR **buf_p, PEXIFHeaderInfo pEXIFHeader);

extern void GET_EXIF(UINT IFD, UCHAR *pExifData, UINT size);

// define which IFD to get in GET_EXIF
#define GET_0thIFD          1
#define GET_EXIFIFD         2
#define GET_Makernote       3
#define GET_InterOpIFD      4
#define GET_1thIFD          5
#define GET_AllIFD          6

//---- data relesse ------------------------------------
extern UINT   Exif_Width_Offset, Exif_Height_Offset;
extern UINT   ExifByteOrder, EXIFHeaderSize;
//------------------------------------------------------
/**
@name JPEG decoder error messages.
*/
//@{
#define ERROR_JPGD_MARKER       0xFFFF      ///< invalid or reserved marker
#define ERROR_JPGD_SOI_NF       0xD8FF      ///< SOI marker not found
#define ERROR_JPGD_DQT_LEN      0xDB00      ///< invalid DQT segment length
#define ERROR_JPGD_DQT_TYPE     0xDB01      ///< unknown quant table type
#define ERROR_JPGD_SOF_NBL      0xC1CF      ///< encoding mode is not baseline
#define ERROR_JPGD_SOF_P        0xC004      ///< sample precision != 8
#define ERROR_JPGD_SOF_NC       0xC005      ///< num components/frame != 3
#define ERROR_JPGD_SOF_SFY      0xC005      ///< MCU format != YUV420/422
#define ERROR_JPGD_DHT_LEN      0xC400      ///< invalid DHT segment length
#define ERROR_JPGD_DHT_TYPE     0xC401      ///< unknown huffman table type
#define ERROR_JPGD_SOS_NF       0xDAFF      ///< SOS marker not found
#define ERROR_JPGD_RST_NF       0xD0FF      ///< RST marker not found
#define ERROR_JPGD_EOI_NF       0xD9FF      ///< EOI marker not found
#define ERROR_JPGD_APP1         0xE1FF      ///< EOI marker not found
#define PARSE_JPGD_OK           0x0000      ///< JPEG decode OK

#define ERROR_JPGD_AVI_NF       0x5249      ///< AVI marker not found
//@}
//#NT#2011/06/08#Meg Lin -begin
extern void ExposureBiasToStr(INT Rat_num, INT Rat_denum, UCHAR *pstr);   //#NT#2007/08/09#Stan Lin
extern void RationalToStr(INT Rat_num, INT Rat_denum, UCHAR *pstr);
extern void ExposureTimeToStr(INT Rat_num, INT Rat_denum, UCHAR *pstr);
//#NT#2011/06/08#Meg Lin -end

//@}

#endif // _JPEGPARSEHEADER_H
