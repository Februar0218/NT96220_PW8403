/**
    @file       jpeg.h
    @ingroup    mICodecJPEG

    @brief      Header file for JPEG module
                This file is the header file that define the API for JPEG module

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

*/

#ifndef _JPEG_H
#define _JPEG_H

#include "OSCommon.h"

/**
    @addtogroup mICodecJPEG
*/
//@{

/*
    JPEG HW block format

    @note For JPG_HW_YUV_FORMAT
*/
typedef enum
{
    BLK_Y_2X    = 0x0800,
    BLK_Y_1X    = 0x0400,
    BLK_Y_2Y    = 0x0200,
    BLK_Y_1Y    = 0x0100,
    BLK_U_2X    = 0x0080,
    BLK_U_1X    = 0x0040,
    BLK_U_2Y    = 0x0020,
    BLK_U_1Y    = 0x0010,
    BLK_V_2X    = 0x0008,
    BLK_V_1X    = 0x0004,
    BLK_V_2Y    = 0x0002,
    BLK_V_1Y    = 0x0001,
    BLK_UV_PACK = 0x8000,
    ENUM_DUMMY4WORD(JPG_HW_BLK_FORMAT)
} JPG_HW_BLK_FORMAT;

/**
    JPEG YUV format

    @note For jpeg_setFileFormat(), and jpeg_setScaleYUVLineOffset().
*/
typedef enum
{
    JPGHWFormatYUV100    = (BLK_Y_1X | BLK_Y_1Y),   ///< YCbCr 100 (Y plane only)
    JPGHWFormatYUV200    = (BLK_Y_2X | BLK_Y_1Y),
    JPGHWFormatYUV111    = (BLK_Y_1X | BLK_Y_1Y | BLK_U_1X | BLK_U_1Y | BLK_V_1X | BLK_V_1Y),
    JPGHWFormatYUV211    = (BLK_Y_2X | BLK_Y_1Y | BLK_U_1X | BLK_U_1Y | BLK_V_1X | BLK_V_1Y),   ///< YCbCr 2h11
    JPGHWFormatYUV211_V  = (BLK_Y_1X | BLK_Y_2Y | BLK_U_1X | BLK_U_1Y | BLK_V_1X | BLK_V_1Y),   ///< YCbCr 2v11
    JPGHWFormatYUV222    = (BLK_Y_2X | BLK_Y_1Y | BLK_U_2X | BLK_U_1Y | BLK_V_2X | BLK_V_1Y),   ///< YCbCr 222h
    JPGHWFormatYUV222_V  = (BLK_Y_1X | BLK_Y_2Y | BLK_U_1X | BLK_U_2Y | BLK_V_1X | BLK_V_2Y),
    JPGHWFormatYUV411    = (BLK_Y_2X | BLK_Y_2Y | BLK_U_1X | BLK_U_1Y | BLK_V_1X | BLK_V_1Y),   ///< YCbCr 411
    JPGHWFormatYUV411_PK = (BLK_Y_2X | BLK_Y_2Y | BLK_U_1X | BLK_U_1Y | BLK_V_1X | BLK_V_1Y | BLK_UV_PACK),
    JPGHWFormatYUV422    = (BLK_Y_2X | BLK_Y_2Y | BLK_U_2X | BLK_U_1Y | BLK_V_2X | BLK_V_1Y),
    JPGHWFormatYUV422_V  = (BLK_Y_2X | BLK_Y_2Y | BLK_U_1X | BLK_U_2Y | BLK_V_1X | BLK_V_2Y),   ///< YCbCr 422v
    ENUM_DUMMY4WORD(JPG_HW_YUV_FORMAT)
} JPG_HW_YUV_FORMAT;

/**
    JPEG slice mode

    @note For jpeg_setSliceMode().
*/
typedef enum
{
    JPG_SLICE_DISABLE = 0,              ///< No slice mode
    JPG_SLICE_RENEW   = 1,              ///< Slice renew (or slice manual) mode
    JPG_SLICE_AUTO    = 2,              ///< Auto (handshake with IME) slice mode

    ENUM_DUMMY4WORD(JPG_HW_SLICE_MODE)
} JPG_HW_SLICE_MODE;

/**
    JPEG Rotate mode

    @note For jpeg_setRotateMode().
*/
typedef enum
{
    JPG_ROTATE_DISABLE = 0,             ///< Not rotate
    JPG_ROTATE_90      = 1,             ///< Rotate clockwise
    JPG_ROTATE_180     = 2,             ///< Rotate 180 degree clockwise
    JPG_ROTATE_270     = 3,             ///< Rotate counter-clockwise

    ENUM_DUMMY4WORD(JPG_HW_ROTATE_MODE)
} JPG_HW_ROTATE_MODE;

//#NT#2011/06/13#HH Chuang -begin
//#NT#Modify JPEG to fit coding rule
/**
    JPEG status

    @note For jpeg_getStatus().
*/
typedef enum
{
    JPG_INT_FRAMEEND = 0x01,            ///< Frame End
    JPG_INT_SLICEDONE = 0x02,           ///< Slice Done
    JPG_INT_DECERR = 0x04,              ///< Decode error
    JPG_INT_BUFEND = 0x08,              ///< Bit stream buffer end (full)
    JPG_INT_ALL = 0xF,                  //< All interrupt

    ENUM_DUMMY4WORD(JPG_INT)
} JPG_INT;

/**
    JPEG Decode Scale Ratio

    @note For jpeg_setScaleRatio().
*/
typedef enum
{
    JPG_HW_DECODE_RATIO_WIDTH_1_2,      ///< 1/2 of Width Only
    JPG_HW_DECODE_RATIO_BOTH_1_2,       ///< 1/2 of Width/Height (support rotation mode)
    JPG_HW_DECODE_RATIO_BOTH_1_4,       ///< 1/4 of Width/Height
    JPG_HW_DECODE_RATIO_BOTH_1_8,       ///< 1/8 of Width/Height

    ENUM_DUMMY4WORD(JPG_HW_DECODE_RATIO)
} JPG_HW_DECODE_RATIO;

/**
    JPEG DC Out Scale Ratio

    @note For JPG_HW_DC_OUT_CFG, PJPG_HW_DC_OUT_CFG.
*/
typedef enum
{
    JPG_HW_DC_OUT_RATIO_NONE,           //< Reserved
    JPG_HW_DC_OUT_RATIO_1_2,            ///< Scaled to 1/2
    JPG_HW_DC_OUT_RATIO_1_4,            ///< Scaled to 1/4
    JPG_HW_DC_OUT_RATIO_1_8,            ///< Scaled to 1/8

    ENUM_DUMMY4WORD(JPG_HW_DC_OUT_RATIO)
} JPG_HW_DC_OUT_RATIO;
//#NT#2011/06/13#HH Chuang -end

/**
    JPEG DC Out Address structure

    @note For jpeg_setDCOutAddr(), and jpeg_getDCOutAddr.
*/
typedef struct
{
    UINT32  DCYAddr;                    ///< Y plane address for DC Out
    UINT32  DCUAddr;                    ///< U(Cb) plane address for DC Out
    UINT32  DCVAddr;                    ///< V(Cr) plane address for DC Out
} JPG_HW_DC_OUT_ADDR,*PJPG_HW_DC_OUT_ADDR;

/**
    JPEG DC Out Configuration structure

    @note For jpeg_setDCOut().
*/
typedef struct
{
    UINT32 DCEnable;                    ///< DC Out enable
                                        ///< - @b TRUE: enable DC Out
                                        ///< - @b FALSE: disable DC Out
    UINT32 DCXRatio;                    ///< DC Out X coordinate (horizontal) ratio
                                        ///< - @b JPG_HW_DC_OUT_RATIO_1_2: scale ratio is 1/2
                                        ///< - @b JPG_HW_DC_OUT_RATIO_1_4: scale ratio is 1/4
                                        ///< - @b JPG_HW_DC_OUT_RATIO_1_8: scale ratio is 1/8
    UINT32 DCYRatio;                    ///< DC Out Y coordinate (vertical) ratio
                                        ///< - @b JPG_HW_DC_OUT_RATIO_1_2: scale ratio is 1/2
                                        ///< - @b JPG_HW_DC_OUT_RATIO_1_4: scale ratio is 1/4
                                        ///< - @b JPG_HW_DC_OUT_RATIO_1_8: scale ratio is 1/8
    UINT32 DCYAddr;                     ///< DC Out Y plane DRAM address
    UINT32 DCUAddr;                     ///< DC Out U(Cb) plane DRAM address
    UINT32 DCVAddr;                     ///< DC Out V(Cr) plane DRAM address
    UINT32 DCYLineOffset;               ///< DC Out Y plane line offset
    UINT32 DCULineOffset;               ///< DC Out U(Cb) plane line offset
    UINT32 DCVLineOffset;               ///< DC Out V(Cr) plane line offset
} JPG_HW_DC_OUT_CFG,*PJPG_HW_DC_OUT_CFG;

/**
    JPEG Bit Rate Control Channel

    @note For jpeg_getBRCData().
*/
typedef enum
{
    JPG_BRC_ALL = 0,                    ///< Y/U/V channel
    JPG_BRC_Y   = 1,                    ///< Y channel
    JPG_BRC_U   = 2,                    ///< U channel
    JPG_BRC_V   = 3,                    ///< V channel

    ENUM_DUMMY4WORD(JPG_BRC_CHANNEL)
} JPG_BRC_CHANNEL;

/**
    JPEG Bit Rate Control Information

    @note For jpeg_getBRCInfo().
*/
typedef struct
{
    UINT32 uiBRCInfo1;                  ///< Rho Q/8
    UINT32 uiBRCInfo2;                  ///< Rho Q/4
    UINT32 uiBRCInfo3;                  ///< Rho Q/2
    UINT32 uiBRCInfo4;                  ///< Rho Q
    UINT32 uiBRCInfo5;                  ///< Rho 2Q
    UINT32 uiBRCInfo6;                  ///< Rho 4Q
    //#NT#2011/07/15#HH Chuang -begin
    //#NT#Support bit rate control
    UINT32 uiBRCInfo7;                  ///< Rho 8Q
    //#NT#2011/07/15#HH Chuang -end
} JPG_BRC_INFO,*PJPG_BRC_INFO;

/**
    JPEG Bit Rate Control Data

    @note For jpeg_getBRCData().
*/
typedef struct
{
    JPG_BRC_INFO YBRCInfo;              ///< Bit Rate Control Y Information
    JPG_BRC_INFO UBRCInfo;              ///< Bit Rate Control U Information
    JPG_BRC_INFO VBRCInfo;              ///< Bit Rate Control V Information
    JPG_BRC_INFO YUVBRCInfo;            ///< Bit Rate Control YUV Information
} JPG_BRC_DATA,*PJPG_BRC_DATA;

//#NT#2011/06/13#HH Chuang -begin
//#NT#Modify JPEG to fit coding rule
#if 0
//Slice Trigger Delay Information
typedef struct
{
    UINT32 TrigValid;
    UINT32 SliceLine;
    UINT32 SliceYAddr;
    UINT32 SliceUAddr;
    UINT32 SliceVAddr;
} JPG_SLICE_TRIG,*PJPG_SLICE_TRIG;
#endif
//#NT#2011/06/13#HH Chuang -end

/**
    JPG Open Object.

    Open Object is used before opening JPG module.
    If you need to hook call back routine in JPEG ISR, you should install JPG_OPENOBJ
    through jpeg_registerCB() before invoking jpeg_open().

    @note For jpeg_registerCB().
*/
typedef struct _JPG_OPENOBJ{
    void (*FP_JPGISR_CB)(UINT32 intstatus); ///< isr callback function
} JPG_OPENOBJ;

//#NT#2011/07/12#HH Chuang -begin
//#NT#Add option to enable/disable JPEG decode error detection
/**
    JPEG configuration identifier

    @note For jpeg_setConfig()
*/
typedef enum
{
    JPEG_CONFIG_ID_CHECK_DEC_ERR,           ///< Enable/Disable checking decode error
                                            ///< (Default is DISABLEd)
                                            ///< Context can be:
                                            ///< - @b TRUE: JPEG driver will check decode error
                                            ///< - @b FALSE: JPEG driver will NOT check decode error
    //#NT#2011/12/05#HH Chuang -begin
    //#NT#Output debug message in jpeg wait done API.
    //#NT#Upper layer can enable jpeg driver to output message when jpeg_waitDone() is invoked in video recording mode.
    JPEG_CONFIG_ID_WAITDONE_MSG,            ///< Enable/Disable output message when each time jpeg_waitDone()/jpeg_waitDonePolling() is invoked
                                            ///< (Default is DISABLED) (Reset to DISABLED after jpeg_close())
                                            ///< Context can be:
                                            ///< - @b TRUE: jpeg_waitDone() will output message when it is invoked
                                            ///< - @b FALSE: jpeg_waitDone() will NOT output message when it is invoked
    //#NT#2011/12/05#HH Chuang -end

    ENUM_DUMMY4WORD(JPEG_CONFIG_ID)
} JPEG_CONFIG_ID;
//#NT#2011/07/12#HH Chuang -end

//@}

// -------------------------------------------------
// The API for the JPEG codec driver
// -------------------------------------------------
//#NT#2011/06/03#HH Chuang -begin
//#NT#Remove compilation warning
extern void   jpeg_registerCB(JPG_OPENOBJ *pObjCB);
//#NT#2011/06/03#HH Chuang -end
//#NT#2011/07/12#HH Chuang -begin
//#NT#Add option to enable/disable JPEG decode error detection
extern ER     jpeg_setConfig(JPEG_CONFIG_ID configID, UINT32 configContext);
//#NT#2011/07/12#HH Chuang -end
extern ER     jpeg_open(void);
extern ER     jpeg_close(void);
extern BOOL   jpeg_isOpened(void);
//extern ER     jpeg_lock(void);
//extern ER     jpeg_unlock(void);
extern ID     jpeg_getLockStatus(void);

extern UINT32 jpeg_getStatus(void);
extern UINT32 jpeg_getActiveStatus(void);
extern UINT32 jpeg_getComSize(void);
extern void   jpeg_setSWReset(void);

extern void   jpeg_setStartEncode(void);
extern void   jpeg_setStartDecode(void);

extern void   jpeg_setEndEncode(void);
extern void   jpeg_setEndDecode(void);

extern ER     jpeg_setEnableInt(UINT32 INTEnable);
extern void   jpeg_setDisableInt(UINT32 disint);
extern void   jpeg_clrInt(UINT32 intsts);
//#NT#2011/06/13#HH Chuang -begin
//#NT#Modify JPEG to fit coding rule
//extern UINT32 jpeg_getIntStatus(void);
//extern UINT32 jpeg_getIntEnStatus(void);
//#NT#2011/06/13#HH Chuang -end

extern void   jpeg_setImgStartAddr(UINT32 ImgYAddr, UINT32 ImgCBAddr, UINT32 ImgCRAddr);
extern void   jpeg_getImgStartAddr(UINT32 *ImgYAddr, UINT32 *ImgCBAddr, UINT32 *ImgCRAddr);

extern ER     jpeg_setImgLineOffset(UINT32 YLOFS, UINT32 ULOFS);
extern ER     jpeg_setImgLineOffsetByte(UINT32 YLOFS, UINT32 ULOFS, UINT32 VLOFS);
extern UINT32 jpeg_getImgLineOffsetY(void);
extern UINT32 jpeg_getImgLineOffsetU(void);
extern UINT32 jpeg_getImgLineOffsetV(void);




extern ER     jpeg_setFileFormat(UINT32 ImgWidth, UINT32 ImgHeight, JPG_HW_YUV_FORMAT FileFormat);





//Add for checking if there is only one JPEG output path simultaneously.
extern BOOL   jpeg_IsOneOutputPath(void);
//Add for checking if there is limitation on JPEG scale function.
extern BOOL   jpeg_IsScaleLimit(USHORT usWidth);
//Add for removing the necessity of coding wai_flg() & chk_flg() outside JPEG driver.
extern UINT32 jpeg_waitDone(void);
extern BOOL   jpeg_waitDonePolling(void);


extern void   jpeg_checkDelayTrigger(void);

//Bit stream control APIs
extern ER     jpeg_setCmpStartAddr(UINT32 BSAddr, UINT32 BufSize);
extern UINT32 jpeg_getBSStartAddr(void);
extern UINT32 jpeg_getBSCurAddr(void);
extern void   jpeg_setBSOutput(BOOL Enable);

//Restart mark APIs
extern ER     jpeg_setRestartInterval(UINT32 MCUNum);
extern ER     jpeg_setRestartEnable(BOOL Enable);
extern UINT32 jpeg_setRestartEOF(void);

//Slice mode APIs
extern void   jpeg_setSliceGoEnable(void);
extern void   jpeg_setSliceGoDisable(void);
extern ER     jpeg_setSliceMode(JPG_HW_SLICE_MODE SliceMode);
extern UINT32 jpeg_setSliceCnt(UINT32 LineNum);
extern ER     jpeg_setSliceEncAddr(UINT32 LineNum, UINT32 YAddr, UINT32 UAddr, UINT32 VAddr);

//Rotation mode API
extern ER     jpeg_setRotateMode(JPG_HW_ROTATE_MODE RotateMode);

//Format transform (420 -> 422) APIs [Encode only]
extern void   jpeg_setFmtTranEnable(void);
extern void   jpeg_setFmtTranDisable(void);

//DC output mode APIs [Encode only]
extern ER     jpeg_setDCOut(PJPG_HW_DC_OUT_CFG pDCOutCfg);
extern void   jpeg_setDCOutEnable(void);
extern void   jpeg_setDCOutDisable(void);
extern void   jpeg_setDCOutAddr(PJPG_HW_DC_OUT_ADDR pDCOutAddr);
extern void   jpeg_getDCOutAddr(PJPG_HW_DC_OUT_ADDR pDCOutAddr);

//Crop mode APIs [Decode only]
extern ER     jpeg_setCrop(UINT32 StartX, UINT32 StartY, UINT32 Width, UINT32 Height);
extern void   jpeg_setCropDisable(void);
extern void   jpeg_setCropEnable(void);

//Scaling [Decode only]
extern void   jpeg_setScaleEnable(void);
extern void   jpeg_setScaleDisable(void);
extern void   jpeg_setScaleAddr(UINT32 Y_Addr, UINT32 Cb_Addr, UINT32 Cr_Addr);
extern ER     jpeg_setScaleYUVLineOffset(JPG_HW_YUV_FORMAT YUVFormat, UINT32 *Y_LineOft, UINT32 *UV_LineOft);
extern void   jpeg_getScaleYUVLineOffset(UINT32 *pY_LineOft, UINT32 *pUV_LineOft);
extern ER     jpeg_setScaleRatio(UINT32 ScaleRatio);

//Table access APIs
extern void   jpeg_setBaseIdxTabHW(UCHAR *puiHuffTabLAC, UCHAR *puiHuffTabLDC, UCHAR *puiHuffTabCAC, UCHAR *puiHuffTabCDC);
extern void   jpeg_setMinCodeTabHW(UCHAR *puiHuffTabLAC, UCHAR *puiHuffTabLDC, UCHAR *puiHuffTabCAC, UCHAR *puiHuffTabCDC);
extern void   jpeg_setSymbolTabHW(UCHAR *puiHuffTabLAC, UCHAR *puiHuffTabLDC, UCHAR *puiHuffTabCAC, UCHAR *puiHuffTabCDC);
extern ER     jpeg_setEncodeQTabHW(UCHAR *pucQTabY, UCHAR *pucQTabUV);
extern ER     jpeg_setDecodeQTabHW(UCHAR *pucQTabY, UCHAR *pucQTabUV);
extern void   jpeg_setEncodeHuffTabHW(USHORT *puiHuffTabLAC, USHORT *puiHuffTabLDC, USHORT *puiHuffTabCAC, USHORT *puiHuffTabCDC);
extern void   jpeg_setDecodeHuffTabHW(UINT8 *pHuffDC0th,UINT8 *pHuffDC1th,UINT8 *pHuffAC0th,UINT8 *pHuffAC1th);
extern void   jpeg_getDecodeHuffTab(void);

//Profiling API
extern UINT32 jpeg_getCycleCnt(void);

//BRC control APIs
extern void   jpeg_getBRCData(JPG_BRC_CHANNEL BRCChannel, UINT32* Reserved);
extern void   jpeg_getBRCInfo(PJPG_BRC_INFO pBRCInfo);


//Obseleted APIs
extern void   jpeg_setRCEnable(void);
extern void   jpeg_setRCDisable(void);
extern void   jpeg_setRCData(UINT32 *puiRCData1, UINT32 *puiRCData2);

// -------------------------------------------------

#endif // _JPEG_H

