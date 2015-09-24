/**
    Header file of video encoding codec library

    Exported header file of video encoding codec library.

    @file       VideoEncode.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEOENCODE_H
#define _VIDEOENCODE_H

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


/**
    type of getting information from video encoding codec library.
    format: ER (*GetInfo)(VIDCOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3); 
*/
typedef enum
{
    MEDIAENC_GETINFO_VIDENCPARAM = 0x1,  //vide encoding parmater. p1: MEDIAREC_VIDENC_PARAM pointer
    MEDIAENC_GETINFO_H264DESC    = 0x90, //h264 description. p1: addr, p2: len
    MEDIAENC_GETINFO_ISIFRAME    = 0x91, //i frame or not, p1:true of false
    ENUM_DUMMY4WORD(VIDCOENC_GETINFO_TYPE)
} VIDCOENC_GETINFO_TYPE;

/**
    type of setting information to video encoding codec library.
    format: ER (*SetInfo)(VIDCOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAENC_SETINFO_YCBCRADDR   = 0x1, ///< set YCbCr addr, p1= y addr, p2= cb addr, p3 = cr addr
    MEDIAENC_SETINFO_WID_HEI     = 0x2, ///< set width/height, p1= width, p2= height
    MEDIAENC_SETINFO_QUALITY     = 0x3, ///< set YCbCr addr, p1= y addr, p2= cb addr, p3 = cr addr
    MEDIAENC_SETINFO_ENCODEBUF   = 0x4, ///< set codec buf, p1= addr, p2=size
    MEDIAENC_SETINFO_RATECONTROL = 0x5,//p1 = target rate (in bytes, 100K~4M)
    MEDIAENC_SETINFO_VIDRATE     = 0x6,//p1 = frame rate
    MEDIAENC_SETINFO_YUVOFFSET   = 0x7, ///< set Y, UV offset, p1=Y offset, p2= UV offset//2010/01/14 Meg add
    MEDIAENC_SETINFO_DISADDR     = 0x8,//y, u, v addr //2010/01/25 Meg, add for DIS
    MEDIAENC_SETINFO_JPG_MODE    = 0x80,
    MEDIAENC_SETINFO_JPG_DCOUTPUT= 0x81,
    MEDIAENC_SETINFO_JPG_CLOSE   = 0x82, //jpg close
    MEDIAENC_SETINFO_H264_SETPARAM= 0x90,//no need param
    MEDIAENC_SETINFO_H264_ENDENC  = 0x91,//no need param
    MEDIAENC_SETINFO_H264_IPRATIO = 0x92,///< set I-P ratio, p1: I+n*P (in)
    ENUM_DUMMY4WORD(VIDCOENC_SETINFO_TYPE)
} VIDCOENC_SETINFO_TYPE;

/**
    Video encoding parameters structure for EncodeOne()
*/
typedef struct
{
    UINT32 encAddr;    ///< codec needed memory
    UINT32 encSize;    ///< size for codec memory
    UINT32 yAddr;      ///< encoding y address
    UINT32 cbAddr;     ///< encoding cb address
    UINT32 crAddr;     ///< encoding cr address
    UINT32 mode;       ///< codec encoding mode
    UINT32 thisSize;   ///< [out] size of output frame
    UINT32 targetRate; ///< target rate (in bytes)
    UINT32 DisY_Addr;  ///< encoding DIS y addr //2010/01/25 Meg, add for DIS
    UINT32 DisU_Addr;  ///< encoding DIS u addr 
    UINT32 DisV_Addr;  ///< encoding DIS v addr 
    UINT16 width;      ///< frame width
    UINT16 height;     ///< frame height
    UINT16 frameRate;  ///< frame rate
    UINT16 rev16_2;
    UINT8  ip_ratio;   ///< p frame number to make a GOP
    UINT8  rev8_2;
    UINT8  rev8_3;
    UINT8  rev8_4;     ///< initilizing q value
    UINT32 qValue;
    //#NT#2010/01/14#Meg Lin -begin
    //#NT#add for DIS
    UINT16 usYDISLineOffset;
    UINT16 usUVDISLineOffset;
    //#NT#2010/01/14#Meg Lin -end
} MEDIAREC_VIDENC_PARAM;


/**
    Struture of video encoder
*/
typedef struct
{
    ER (*Initialize)(void);  ///< initializing codec
    ER (*GetInfo)(VIDCOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);  ///< get information from codec library
    ER (*SetInfo)(VIDCOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3); ///< set parameters to codec library
    ER (*EncodeOne)(UINT32 type, UINT32 outputAddr, UINT32 *pSize, MEDIAREC_VIDENC_PARAM *ptr); ///< encoding one video frame
    ER (*AdjustBPS)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3); ///< adjusting BPS
    ER (*CustomizeFunc)(UINT32 type, void *pobj); ///< codec customize function
    UINT32 checkID;     ///< check ID 
} VIDEO_ENCODER, *PVIDEO_ENCODER;


/*
    Public funtions to get video encoding object
*/
//@{
extern PVIDEO_ENCODER MP_H264Enc_getVideoEncodeObject(void);
extern PVIDEO_ENCODER MP_MjpgEnc_getVideoEncodeObject(void);
extern PVIDEO_ENCODER MP_MjpgEnc_getVideoSliceEncodeObject(void);
//@}
#endif

//@}
