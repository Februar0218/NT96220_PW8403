/**
    Header file of video decoding codec library

    Exported header file of video decoding codec library.

    @file       VideoDecode.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEODECODE_H
#define _VIDEODECODE_H


#include "Audio.h"
#include "FileDB.h"
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"
/**
    @addtogroup mIAVCODEC
*/
//@{


#define VIDDEC_YCBCRMAXBLOCK    11
#define VIDDEC_YCBCRMINBLOCK    3 //2010/05/20 Meg Lin
/**
    type of getting information from video decoding codec library.
    format: ER (*GetInfo)(VIDCODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3); 
*/
typedef enum
{
    MEDIAVID_GETINFO_NOWYCBCR   =0x1,   ///< p1:y addr(out), p2:cb addr(out), p3: cr addr(out)  
    MEDIAVID_GETINFO_PREVYCBCR  =0x2,   ///< p1:prev y addr(out), p2: prev cb addr(out), p3: prev cr addr(out) 
    MEDIAVID_GETINFO_LASTJPGTYPE =0x3,    ///< p1: last decoded JPG type, JPGFormatYUV420 or JPGFormatYUV422
    MEDIAVID_GETINFO_DECODE_W_H  =0x4,  ///< p1:decode width (out), p2:decode height(out)
    MEDIAVID_GETINFO_DECODE_LOFT  =0x5,  ///< p1:decode Y lineoffset (out), p2:decode cbcr lineoffset(out)////2011/11/14 Meg Lin

    ENUM_DUMMY4WORD(VIDCODEC_GETINFO_TYPE)
} VIDCODEC_GETINFO_TYPE;

/**
    type of setting information from video decoding codec library.
    format: ER (*SetInfo)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAVID_SETINFO_LCDSIZE  =0x1,     ///< p1:lcd panel width(in), p2:lcd panel height(in)
    MEDIAVID_SETINFO_DECODEMODE=0x2,    ///< p1:decode mode(in), MEMORY_TINY or MEMORY_NORMAL
    MEDIAVID_SETINFO_MAXDECSIZE=0x3,    ///< p1:max dec width(in), p2: max dec height (in)//2011/06/07 Meg Lin
    MEDIAVID_SETINFO_264Addr  =0x81,    ///< p1:address for H264 description (in)
    MEDIAVID_SETINFO_264YAddr =0x82,    ///< p1:y addr(in), p2:UV addr(in)
    MEDIAVID_SETINFO_CLOSEBUS =0x83,    ///< no parameters
    MEDIAVID_SETINFO_264BBPLAY=0x84,    ///< p1:Backward or not(in)
    MEDIAVID_SETINFO_RESETGOP =0x85,    ///< no parameters
    MEDIAVID_SETINFO_RAWTYPE  =0x86,    ///< p1:rawtype (H264_UVPACKING), compatible to JPG 
    ENUM_DUMMY4WORD(VIDCODEC_SETINFO_TYPE)
} VIDCODEC_SETINFO_TYPE;

/**
    @name type for CustomizeFunc() in VIDEO_DECODER

    type for CustomizeFunc() in VIDEO_DECODER.
*/
//@{
#define MEDIAVID_CUSTOM_DECODE1ST   0x01    ///< p1:(MEDIA_DSP_1STV *) object(out)
//@}


/**
    struture of video frame information. Used for CustomizeFunc().
*/
typedef struct 
{
    UINT32 addr;        ///< in:video frame addr
    UINT32 size;        ///< in:video frame size
    UINT32 decodeBuf;   ///< in:output raw addr
    UINT32 decodeBufSize;///< in:max size for outputing raw
    UINT32 lcdWidth;    ///< in: lcd width
    UINT32 lcdHeight;   ///< in: lcd height
    UINT32 decodeMode;  ///< in: decode mode, MEMORY_TINY or MEMORY_NORMAL
    UINT32 maxDecWidth; ///< in: max raw width 4 panel //2011/08/08 Meg Lin
    UINT32 y_Addr;      ///< out:output Y addr
    UINT32 cb_Addr;     ///< out:output cb addr
    UINT32 cr_Addr;     ///< out:output cr addr
    UINT32 width;       ///< out:video frame width
    UINT32 height;      ///< out:video frame height
    UINT32 wid_lineoft; ///< out:width line offset //2011/11/14 Meg Lin
    UINT32 wid_lineoftuv; ///< out:width line offset//2011/11/14 Meg Lin 
    UINT32 h264DescAddr;///< in:h264 description addr 
    UINT32 h264DescLen; ///< in:h264 description length
    UINT32 jpgType;     ///< out:jpg type, JPGFormatYUV420 or others
} MEDIA_DSP_1STV;



/**
    struture of video data.
*/
typedef struct
{
    UINT32 yAddr;   ///< y address
    UINT32 cbAddr;  ///< cb address
    UINT32 crAddr;  ///< cr address
} VIDEO_YCBCRADDR;

/**
    struture of video display setting. Used for Initialize()
*/
typedef struct
{
    UINT32 firstFramePos;   ///<in: first video frame position
    UINT32 firstFrameSize;  ///<in: first video frame size
    UINT32 rawAddr;         ///<in: after decoded, Y addr (cb, cr counted)
    UINT32 rawSize;         ///<in: size for YCbCr
    UINT32 dspWidth;        ///<in: display width
    UINT32 dspHeight;       ///<in: display height
    UINT32 dspType;         ///<in: display type
    UINT32 rawWidth;        ///<in: raw width
    UINT32 rawHeight;       ///<in: raw height
    UINT32 rawType;         ///<in: raw type, (jdcfg.fileformat)
    UINT32 rawCbAddr;       ///<out: after decoded, cb addr
    UINT32 rawCrAddr;       ///<out: after decoded, cr addr 
    UINT32 h264DescAddr;    ///<in: h264 description addr
    UINT32 h264DescLen;     ///<in: h264 description length
    UINT32 jpgType;         ///<in: jpg type   
    UINT32 lcdWidth;        ///<in: lcd width
    UINT32 lcdHeight;       ///<in: lcd height
    UINT32 decodeMode;      ///<in: decode mode, MEMORY_TINY or MEMORY_NORMAL
    UINT32 maxDecWidth;     ///<in: max dec width//2011/06/07 Meg Lin
    UINT32 maxDecHeight;    ///<in: max dec height//2011/06/07 Meg Lin
    UINT32 decodeWidth;     ///<out: size out to memory, related to decode mode, different with rawWidth 
    UINT32 decodeHeight;    ///<out: size out to memory, related to decode mode, different with rawHeight 
    UINT32 decodeLineoft;   ///<out: y line offset of decodeWidth//2011/11/14 Meg Lin
    UINT32 decodeLineoftUV; ///<out: uv line offset of decodeWidth//2011/11/14 Meg Lin
    UINT32 vidRawYsize;     ///<out: decoded raw Y size (maybe 1/2 orignal width,..) //2011/04/06 Meg Lin
    UINT32 vidRawCbsize;    ///<out: decoded raw cb size (420: Ysize/4, 422:Ysize/2) //2011/08/08 Meg Lin
} VIDEO_DISPLAYINFO;

/**
    Struture of video decoder
*/
typedef struct
{
    ER (*Initialize)(UINT32 fileAddr, VIDEO_DISPLAYINFO *pobj);///< initialize decoder
    ER (*GetInfo)(VIDCODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);///< get information from decoder
    ER (*SetInfo)(VIDCODEC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);///< set information to decoder
    ER (*DecodeOne)(UINT32 type, UINT32 fileAddr, UINT32 p2, UINT32 *p3);///< decode one frame, p1: frame addr(in), p2: frame size(in)
    ER (*WaitDecodeDone)(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);///< wait decoding finished, p1: E_OK or not(out)
    ER (*CustomizeFunc)(UINT32 type, void *pobj);///< customize function, if needed
    UINT32 checkID;///< check ID for decoders
} VIDEO_DECODER, *PVIDEO_DECODER;

/*
    Public funtions to get video decoding object
*/
//@{
extern PVIDEO_DECODER MP_H264Dec_getVideoDecodeObject(void);
extern PVIDEO_DECODER MP_MjpgDec_getVideoDecodeObject(void);
//@}


//@}

#endif //_VIDEODECODE_H

