/*
    @file       RawEncAPI.h
    @ingroup    mIAPPRAWENC

    @brief      header file of raw encoder applications

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _RAWRECAPI_H
#define _RAWRECAPI_H

/**
    Callback function type for media recording. Event types are defined in MediaRecAPI.h.
*/
typedef void (RawEncCallbackType)(UINT32 event_id);
/**
    @name raw encoding EVENT ID

    callback ID of raw encoder.
*/
//@{
#define     RAWENC_EVENT_RESULT_OK        1    ///< Finish normally
#define     RAWENC_EVENT_RESULT_ERR       2    ///< encode Error
#define     RAWENC_EVENT_RESULT_WRITE_ERR 3    ///< filesystem write fails
//@}

#define RAWENC_JPGSIZE_1280x720   204800//200K
#define RAWENC_JPGSIZE_1920x1080  409600//400K


typedef enum
{
    RAW_FORMAT_444 = 0x0,   ///< YCbCr = 1:1:1, unsupported now 2009/12/3
    RAW_FORMAT_422 = 0x1,   ///< YCbCr = 2:1:1, unsupported now 2009/12/3
    RAW_FORMAT_420 = 0x2,   ///< YCbCr = 4:1:1, unsupported now 2009/12/3
    RAW_FORMAT_420_UVPACK = 0x3, ///< YCbCr = 2:1:1, and uv packing
    ENUM_DUMMY4WORD(RAW_FORMAT_TYPE)
} RAW_FORMAT_TYPE;

typedef enum
{
    INPUT_MEMORYADDR = 0x0,   ///< input width, param1: width(in)
    INPUT_MEMORYSIZE = 0x1,   ///< input height, param1: height(in)
    ENUM_DUMMY4WORD(RAW_CHANGE_TYPE)
} RAW_CHANGE_TYPE;

typedef struct
{
    UINT32              uiMemAddr;          ///< starting address for recording
    UINT32              uiMemSize;          ///< size for recording
    UINT32              uiMaxWidth;         ///< max encode pic width
    UINT32              uiMaxHeight;        ///< max encode pic height
//    UINT32              uiInputWidth;       ///< raw input width
//    UINT32              uiInputHeight;      ///< raw input height
//    UINT32              uiEncQ;             ///< input encoding Q //2010/01/04 Meg
//    RAW_FORMAT_TYPE     uiRawFormat;        ///< raw format, RAW_FORMAT_444 or others
    RawEncCallbackType  *CallBackFunc;     ///< event inform callback
} RAW_ENC_OBJ;

extern ER   RawEnc_Open(RAW_ENC_OBJ *pObj);
extern ER   RawEnc_Close(void);
extern void RawEnc_SetRawAddr2Encode(UINT32 yaddr, UINT32 cbaddr, UINT32 craddr, UINT32 ylineoffset);//2010/02/22
extern ER   RawEnc_ChangeParameter(RAW_CHANGE_TYPE type, UINT32 p1, UINT32 p2, UINT32 p3);
extern UINT32 RawEnc_GetRawFilesize(void);//2010/01/07 Meg
extern void RawEnc_SetCaptureOne(void);//2011/08/16 Meg


#endif //_RAWRECAPI_H
