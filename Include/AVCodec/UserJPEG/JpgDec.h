/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       JpgDec.h
    @ingroup    mIAVJPEG

    @brief      Jpeg decoder

*/

#ifndef _JPGDEC_H
#define _JPGDEC_H

#include "OSCommon.h"
#include "JpegCommon.h"
#include "JpgParseHeader.h"

/** \addtogroup mIAVJPEG
*/
//@{

#define FST_READ_THUMB_BUF_SIZE         0x10000     ///< we only need 64k data to get thumbnail
//#NT#2010/04/13#Scottie -begin
//#fix AVI decode error when first frame size is bigger than 128k
#define FST_READ_SOMEAVI_SIZE           0x4B000     ///< read other 300k data for finding AVI first JPG
//#NT#2010/04/13#Scottie -end

//default SliceChunk
#define JPEGDECODER_SLICECHUNK          128
#define JPEGSIZE_7M                     0x6c0000 //3072*2304
//#NT#2008/09/02#JR Huang -begin
//fix bug, assign error value
#define JPEGSIZE_12M                    0xB43000//3968*2976

///Jpeg decoder descriptor
typedef struct _DECJPG_INFO{
    UINT32              (*pJPGScalarHandler)(PJPGHEAD_DEC_CFG, UINT32 *);
    UINT8               *pSrcAddr;      ///< Source bitstream address
    UINT8               *pDstAddr;      ///< Destination YCbCr raw image buffer address
    UINT32              JPGFileSize;    ///< Source bitstream size
    UINT32              AVIFileSize;    ///< AVI source bitstream size
    PJPGHEAD_DEC_CFG    jdcfg_p;        ///< JPEG decode configuration parameters
    UINT32              uiOutAddrY;     ///< Output raw image Y buffer address in pDstAddr
    UINT32              uiOutAddrCb;    ///< Output raw image Cb buffer address in pDstAddr
    UINT32              uiOutAddrCr;    ///< Output raw image Cr buffer address in pDstAddr
    JPG_DEC_TYPE        IfDecodeThumb;  ///< Flag to indicate if to decode thumbnail instead of full image
//#NT#2008/12/03#Scottie -begin
//#NT#Add for new Screennail decoding mechanism
    BOOL                bIfSpeedUpSN;
//#NT#2008/12/03#Scottie -end
//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
#if 0
    UINT32              uiSliceChunk;
    UINT                uiCurOutCnt;
#endif
    BOOL                bEnableTimeOut;
    void                (*pJPGTimerStart)(void);
    void                (*pJPGTimerPause)(void);
    void                (*pJPGTimerWait)(void);
    BOOL                bEnableCrop;
    UINT32              uiCropStartX;
    UINT32              uiCropStartY;
    UINT32              uiCropWidth;
    UINT32              uiCropHeight;
//#NT#2009/05/20#Corey Ke -end
} DECJPG_INFO, *PDECJPG_INFO;

//---- Functions relesse -------------------------------
extern void    JpegDecStart(void);
extern void    JpegDecFinish(void);
extern ER      DecodeOneJPG(PDECJPG_INFO pDecJPG);
extern void    JPEGSetDecodeQTab(UINT8 *, UINT8 *);
//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
#if 0
//#NT#2007/06/20#Scottie -begin
//#Implement JPEG Slice decode function
extern ER      DecodeOneJPG_SliceMode(PDECJPG_INFO pDecJPG, UINT uiSliceAddr, UINT32 uiYStart, UINT32 uiYEnd);
//#NT#2007/06/20#Scottie -end
#endif
//#NT#2009/05/20#Corey Ke -end

//@}

#endif // _JPGDEC_H