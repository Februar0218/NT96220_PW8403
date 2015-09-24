/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       JPEGBRC.h
    @ingroup    mIAVJPEG

    @brief      JPEG Bit Rate Control

*/

#ifndef _JPEGBITRATECTRL_H
#define _JPEGBITRATECTRL_H

/**
    JPEG BRC debug level
*/
typedef enum
{
    BRC_DEBUG_LEVEL_DISABLE = 0,
    BRC_DEBUG_LEVEL_SIMPLE  = 1,
    BRC_DEBUG_LEVEL_FULL    = 2,
    ENUM_DUMMY4WORD(BRC_DEBUG_LEVEL)
} BRC_DEBUG_LEVEL;

/**
    JPEG BRC YUV type

    @note For JPEGBRC_predictQuality()
*/
typedef enum
{
    BRC_COMBINED_YUV420,                    ///< Combined YUV420 (predict with a full Y/U/V 420 image)
    BRC_COMBINED_YUV422,                    ///< Combined YUV422 (predict with a full Y/U/V 422 image)
    BRC_SEPERATE_YUV420,                    ///< Seperate YUV420 (predit with YUV420 seperate phase)
    BRC_SEPERATE_YUV422 ,                   ///< Seperate YUV420 (predit with YUV422 seperate phase)
    ENUM_DUMMY4WORD(BRC_INPUT_RAW)
} BRC_INPUT_RAW;

//#NT#2011/08/04#HH Chuang -begin
//#NT#Report detection overflow/underflow
/**
    JPEG BRC predict result

    @note For JPEGBRC_predictQuality()
*/
typedef enum
{
    BRC_RESULT_OK,                          ///< Prediction OK
    BRC_RESULT_OVERFLOW,                    ///< Prediction overflow
    BRC_RESULT_UNDERFLOW,                   ///< Prediction underflow

    ENUM_DUMMY4WORD(BRC_RESULT)
} BRC_RESULT;
//#NT#2011/08/04#HH Chuang -end

/**
    JPEG BRC parameter

    @note For JPEGBRC_predictQuality()
*/
typedef struct
{
    UINT32 Width;                           ///< Raw Width (input parameter)
    UINT32 Height;                          ///< Raw Height (input parameter)
    UINT32 YEncByte;                        ///< Y Channel Encode BS Size (input parameter)
    UINT32 UEncByte;                        ///< U Channel Encode BS Size (input parameter)
    UINT32 VEncByte;                        ///< V Channel Encode BS Size (input parameter)
    UINT32 YUVEncByte;                      ///< YUV Channel Encode BS Size (input parameter)

    UINT32 UBoundByte;                      ///< BRC Up Bound BS Size (input parameter)
    UINT32 TargetByte;                      ///< Bit Rate Control Target BS Size (input parameter)
    UINT32 LBoundByte;                      ///< BRC Low Bound BS Size (input parameter)

    UINT32 Reserved[32];                    ///< Reserved for Bit Rate Control Information (input/output parameter)
} BRCParam, *PBRCParam;

//#NT#2011/08/04#HH Chuang -begin
//#NT#Report detection overflow/underflow
/**
    JPEG BRC information

    @note For JPEGBRC_predictQuality()
*/
typedef struct
{
    BRC_INPUT_RAW inputRaw;                 ///< YUV information of raw image (input parameter)
    UINT32 current_qf;                      ///< current QF  (input parameter)
    UINT32 current_rate;                    ///< current bit stream size  (input parameter)
    PBRCParam pBRCParam;                    ///< Bit rate control parameter  (input/output parameter)

    BRC_RESULT brcResult;                   ///< Result of next QF prediction (output parameter)
} BRCInfo, *PBRCInfo;
//#NT#2011/08/04#HH Chuang -end

extern void     JPEGBRC_selectDebugLevel(BRC_DEBUG_LEVEL BRCDebugLevel);
//#NT#2011/08/04#HH Chuang -begin
//#NT#Report detection overflow/underflow
extern UINT32   JPEGBRC_predictQuality(PBRCInfo pBrcInfo);
//extern UINT32   JPEGBRC_predictQuality(BRC_INPUT_RAW InputRaw, UINT32 current_qf, UINT32 current_rate, PBRCParam pBRCParam);
//#NT#2011/08/04#HH Chuang -end
extern void     JPEGBRC_genQYtable(UINT32 uiQuality, const UINT8* pStdYtbl, UINT8* pYtbl);
extern void     JPEGBRC_genQUVtable(UINT32 uiQuality, const UINT8* pStdUVtbl, UINT8* pUVtbl);

#endif
