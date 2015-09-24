/**
    @file       JpegCommon.h
    @ingroup    mIJPEG

    @brief      Header file for JPEG module
                This file is the header file that define the common API for JPEG module

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

*/

#ifndef _JPEGCOMMON_H
#define _JPEGCOMMON_H

#include "Type.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//Reserved for old API use these definition
#define JPEG_FORMAT_420         0x00100000
#define JPEG_FORMAT_444         0x00400000
#define JPEG_FORMAT_422         0x00800000

#define JPG_HW_FMT_420          0x00100000
#define JPG_HW_FMT_211          0x00800000
///////////////////////////////////////////////////////////////////////////////////////////////////

//YUV Format
typedef enum
{
    JPGFormatYUV420  = 0x00100000,          //JPEG macroblock color format is YUV420  (16x16, 8x8, 8x8)
    JPGFormatYUV422  = 0x00200000,          //JPEG macroblock color format is YUV422  (16x16, 8x16, 8x16)
    JPGFormatYUV444  = 0x00400000,          //JPEG macroblock color format is YUV444  (16x8, 16x8, 16x8)
    JPGFormatYUV211  = 0x00800000,          //JPEG macroblock color format is YUV211  (16x8, 8x8, 8x8)
    JPGFormatYUV211V = 0x01000000,          //JPEG macroblock color format is YUV211V (8x16, 8x8, 8x8)
    JPGFormatYUV100  = 0x02000000,          //JPEG macroblock color format is Y only  (8x8)
    ENUM_DUMMY4WORD(JPG_YUV_FORMAT)
} JPG_YUV_FORMAT;

//Codec Mode
typedef enum
{
    JPG_Enc_AsyncMode    = 0x00001000,
    JPG_Enc_Enable_Slice = 0x00002000,
    JPG_Enc_SrcSliceMode = 0x0000A000,
    ENUM_DUMMY4WORD(JPG_CODEC_MODE)
} JPG_CODEC_MODE;

//BRC Mode
typedef enum
{
    JPG_Enc_Constant     = 0,
    JPG_Enc_FileSZCtl    = 1,
    JPG_Enc_VAEstCtl     = 2,
    JPG_Enc_RhoBRCtl     = 3,
    ENUM_DUMMY4WORD(JPG_BRC_MODE)
} JPG_BRC_MODE;

//Interrupt Mode
typedef enum
{
    _JPG_Int_FrameEnd  = 0x00000001,
    _JPG_Int_SliceDone = 0x00000002,
    _JPG_Int_DecodeErr = 0x00000004,
    _JPG_Int_BufEnd    = 0x00000008,
    ENUM_DUMMY4WORD(JPG_INTR_MODE)
} JPG_INTR_MODE;

//Interrupt Status
typedef enum
{
    _JPEG_Frame_End  = 0x00000001,
    _JPEG_Slice_Done = 0x00000002,
    _JPEG_Decode_Err = 0x00000004,
    _JPEG_Buf_End    = 0x00000008,
    ENUM_DUMMY4WORD(JPG_INTR_STATUS)
} JPG_INTR_STATUS;

//BS Output Mode
typedef enum
{
    JPG_Enc_BSOUTEN  = 0x00000010,
    JPG_Enc_BSOUTDIS = 0x00000020,
    ENUM_DUMMY4WORD(JPG_BS_OUT)
} JPG_BS_OUT;

//Rotation Mode
typedef enum
{
    JPGROTATE_DISABLE = 0,          //Disable JPEG rotate
    JPGROTATE_CW      = 1,          //Enable JPEG clockwise rotate
    JPGROTATE_CCW     = 2,          //Enable JPEG counter-clockwise rotate
    JPGROTATE_180     = 3,          //Enable JPEG 180 degree clockwise rotate
    ENUM_DUMMY4WORD(JPG_ROTATE_MODE)
} JPG_ROTATE_MODE;

extern ER     JpegOpen(void);
extern ER     JpegClose(void);
extern void   JpegSWReset(void);
extern ER     JpegSetEnableInt(UINT32 INTEnable);
extern ER     JpegSetFileFormat(UINT32 ImgWidth, UINT32 ImgHeight, JPG_YUV_FORMAT YUVFormat);
extern void   JpegSetImgStartAddr(UINT32 ImgYAddr, UINT32 ImgUAddr, UINT32 ImgVAddr);
extern ER     JpegSetCmpStartAddr(UINT32 BSAddr, UINT32 BufSize);
extern ER     JpegSetSliceEncAddr(UINT32 LineNum, UINT32 YAddr, UINT32 UAddr, UINT32 VAddr);
extern ER     JpegSetImgLineOffset(UINT32 ImgYOfs, UINT32 ImgCBCROfs);
extern ER     JpegSetScaleYUVLineOffset(UINT32 YLineOft, UINT32 UVLineOft, JPG_YUV_FORMAT YUVFormat);
extern void   JpegGetScaleYUVLineOffset(UINT32 *pYLineOft, UINT32 *pUVLineOft);
extern ER     JpegSetRestartInterval(UINT32 MCUNum);
extern ER     JpegSetScaleMode(UINT32 ScaleMode);
extern ER     JpegSetRotateMode(UINT32 RotateMode);
extern UINT32 JpegGetSize(void);
extern UINT32 JpegGetStatus(void);
extern UINT32 JpegWaitDone(void);
extern BOOL   JpegWaitDonePolling(void);
extern UINT32 JpegSetRestartEOF(void);

#endif // _JPEGCOMMON_H
