/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       JpgIMGReSize.h
    @ingroup    mIAVJPEG

    @brief Header for resize JPG Image, including Width/Height or Quality.
*/

#ifndef _JPGIMGRESIZE_H
#define _JPGIMGRESIZE_H

#include "Type.h"
#include "exif.h"
#include "OSCommon.h"
#include "JpgParseHeader.h"

/** \addtogroup mIAVJPEG
*/
//@{
//#NT#2010/12/24#Daniel -begin
#if 0
/**
    JPEG Resize Encode Parameter.
    @note In fact, this structure is the same as JPGCmpParam in JpgEnc.h
*/
typedef struct
{
    UINT    RawImageAddrY;              ///< Raw Image Y address. CB CR will following this address.
    UINT    RawImageAddrCB;             ///< Raw Image Y address. CB CR will following this address.
    UINT    RawImageAddrCR;             ///< Raw Image Y address. CB CR will following this address.

    UINT    JPGBSAddr;                  ///< Compressed data Input/Ouput location.
    UINT    *JPGBSBufSize;              ///< Buffer Size allocated for JPG ouput bitstream. (IN/OUT Parameter)

    UINT    Original_Width;             ///< Original JPG Width, it must be 16-multiplier.
    UINT    Original_Height;            ///< Original JPG Height.
    UINT    Original_Mode;              ///< Original JPG Encoding Mode, ()

    UINT    New_Width;                  ///< ReSize Image Width, This value should be 16-multiplier.
    UINT    New_Height;                 ///< ReSize Image Height, This value should be 8 or 16 multiplier, depends on output BS format.

    UINT    Mode;                       ///< Encoding Mode
} JPGReSizeParam, *PJPGReSizeParam;

/**
    JPEG re-size information structure.
*/
typedef struct _RE_SIZE_INFO
{
    UINT32  uiBufAddrY;         ///< Y starting address
    UINT32  uiBufAddrCb;        ///< Cb starting address
    UINT32  uiBufAddrCr;        ///< Cr starting address
    UINT32  uiOutBufAddr;       ///< Out bitstream buffer starting address
    UINT32  uiOutBufSize;       ///< Out bitstream buffer size

    UINT32  uiTmpBufAddr;       ///< Temp buffer addr for slice data
    UINT32  uiTmpBufSize;       ///< Temp buffer size for slice data

    UINT8   *pQTabY;            ///< Q-table addr pointer Y
    UINT8   *pQTabUV;           ///< Q-table addr pointer UV

    UINT16  usNewWidth;         ///< New image width
    UINT16  usNewHeight;        ///< New image height
    UINT16  usOriWidth;         ///< Ori image width
    UINT16  usOriHeight;        ///< Ori image height

    UINT32  ucNewFormat;        ///< New image format
    UINT32  ucOriFormat;        ///< Ori image format

//#NT#2007/07/20#Scottie -begin
//#Add for Restart marker
    USHORT  usRstInterval;      ///< restart interval (initially set to 0)
//#NT#2007/07/20#Scottie -end
} RE_SIZE_INFO, *PRE_SIZE_INFO;

/**
    JPEG re-encoded information structure.
*/
typedef struct _RE_ENCODE_INFO
{
    UINT32  uiBufAddrY;         ///< Y starting address
    UINT32  uiBufAddrCb;        ///< Cb starting address
    UINT32  uiBufAddrCr;        ///< Cr starting address
    UINT32  uiOutBufAddr;       ///< Out bitstream buffer starting address
    UINT32  uiOutBufSize;       ///< Out bitstream buffer size
    UINT8   *pQTabY;            ///< Q-table addr pointer Y
    UINT8   *pQTabUV;           ///< Q-table addr pointer UV
    UINT16  usImgWidth;         ///< New image width
    UINT16  usImgHeight;        ///< New image height
    UINT32  ucImgFormat;        ///< New image format
    UINT8   ucTPOSEnable;       ///< New rotate enable
    USHORT  usRstInterval;      ///< restart interval (initially set to 0)
} RE_ENCODE_INFO, *PRE_ENCODE_INFO;
#else
typedef struct _RE_ENCODE_INFO
{
    UINT32  uiBufAddrY;         // Y starting address
    UINT32  uiBufAddrCb;        // Cb starting address
    UINT32  uiBufAddrCr;        // Cr starting address
    UINT32  uiOutBufAddr;       // Out bitstream buffer starting address
    UINT32  uiOutBufSize;       // Out bitstream buffer size

    UINT32  uiTmpBufAddr;       // Temp buffer addr for slice data for resize
    UINT32  uiTmpBufSize;       // Temp buffer size for slice data for resize

    UINT8   *pQTabY;            // Q-table addr pointer Y
    UINT8   *pQTabUV;           // Q-table addr pointer UV

    UINT16  usImgWidth;         // New image width
    UINT16  usImgHeight;        // New image height
    UINT32  ucImgFormat;        // New image format

    UINT16  usOriWidth;         // Ori image width for resize
    UINT16  usOriHeight;        // Ori image height for resize
    UINT32  ucOriFormat;        // Ori image format for resize

    UINT32  uiLineOffsetY;      // Luminance Line Offset (byte unit)
    UINT32  uiLineOffsetUV;     // Chrominance Line Offset (byte unit)
    UINT32  Reserved;           // Reserved Chrominance Line Offset (byte unit)

    UINT8   ucTPOSEnable;       // New rotate enable
    USHORT  usRstInterval;      // restart interval (initially set to 0)
} RE_ENCODE_INFO, *PRE_ENCODE_INFO;
#endif
//#NT#2010/12/24#Daniel -end
//
// This function will auto adjust Width/Height information in EXIF header, if there are EXIF header for the
// input bitstream.
// If input uiQuality is 0, this function will do JPG Image Resize only.
// If Width and Height is 0, this function will do JPG Image Q-table recompress only.
//
//extern ER       JpegReSizeOnePic(PJPGReSizeParam JpgReSizeParam, UINT uiQuality);

extern void     JpegReSizeUpdateHeader(PJPGHEAD_DEC_CFG pJPGInfo, UINT32 NewWidth, UINT32 NewHeight, UINT32 NewFormat);
extern void     JpegReQtyUpdateHeader(PJPGHEAD_DEC_CFG pJPGInfo, UINT32 uiQuality);
//#NT#2010/12/24#Daniel -begin
extern ER       JpegReSizeHWSetting(PRE_ENCODE_INFO JpgReSizeParam);
//#NT#2010/12/24#Daniel -end
extern ER       JpegReEncodeHWSetting(PRE_ENCODE_INFO pReEncode);
//#NT#2007/08/30#JR Huang begin
extern void     JpegReSizeGenerateJpegHeader(PJPGHEAD_DEC_CFG pJPGInfo,UCHAR *pOriJPGHeader);
//#NT#2007/08/30#JR Huang end
//#NT#2008/11/17#Scottie -begin
//#NT#Use the standard Thumb Header (QTable/HTable) instead of Ori-Thumb Header
extern UINT32   JpegGetThumbQTable(BOOL ThumbQ_Y);
extern UINT32   JpegGenerateThumbHeader(UCHAR *pOriJPGHeader);
//#NT#2008/11/17#Scottie -end

//@}

#endif
