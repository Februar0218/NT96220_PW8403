
//    ImageJPG
/*
*/

#ifndef _GX_IMAGEJPG_H_
#define _GX_IMAGEJPG_H_

#include "GxCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------------------
//  JPG
//--------------------------------------------------------------------------------------
BOOL Jpeg_DecodeInfo(const UINT8* JpegBitStream, UINT32 uiBitStreamSize,
        UINT16* pW,  UINT16* pH, UINT16* pFMT);

//condition : pBuf must 32 byte alignment
BOOL Jpeg_DecodeImage(const UINT8* JpegBitStream, UINT32 uiBitStreamSize,
        UINT8* pBuf, UINT32 uiBufSize);

BOOL Jpeg_GetImageAddr(UINT16 w, UINT16 h, UINT16 fmt,
        UINT8* pBuf, UINT32* uiOutAddrY, UINT32* uiOutAddrCb, UINT32* uiOutAddrCr);


#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_IMAGEJPG_H_
