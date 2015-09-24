/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2004.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: AAA Library Module                                                 *
* Description:                                                             *
* Author: YuppieLin                                                        *
****************************************************************************/

/** \addtogroup mIIPPAAA */
//@{

#ifndef _AAA_LIB_H
#define _AAA_LIB_H
#include "Type.h"


#define AAA_FLAG_AE_ENABLE                  (0x00000001)
#define AAA_FLAG_AWB_ENABLE                 (0x00000001<<2)
#define AAA_FLAG_AE_CALIBRATION             (0x00000001<<3)
#define AAA_FLAG_AWB_CALIBRATION            (0x00000001<<5)
#define AAA_FLAG_FLASH_CALIBRATION          (0x00000001<<6)
#define AAA_FLAG_MSH_CALIBRATION            (0x00000001<<7)
#define AAA_FLAG_OB_CALIBRATION             (0x00000001<<8)
#define AAA_FLAG_VIG_CALIBRATION            (0x00000001<<9)
//#NT#2011/10/18#Jarkko Chang -begin
//#NT#add caf mode 
#define AAA_FLAG_CAF_ENABLE                 (0x00000001<<10)
//#NT#2011/10/18#Jarkko Chang -end

#define AE_LOCK         (uiAAAFlags &= (~AAA_FLAG_AE_ENABLE))
#define AE_UNLOCK       (uiAAAFlags |=   AAA_FLAG_AE_ENABLE)
#define AWB_UNLOCK      (uiAAAFlags |=   AAA_FLAG_AWB_ENABLE)
#define AWB_LOCK        (uiAAAFlags &= (~AAA_FLAG_AWB_ENABLE))
//#NT#2011/10/18#Jarkko Chang -begin
//#NT# add caf mode
#define CAF_LOCK        (uiAAAFlags &= (~AAA_FLAG_CAF_ENABLE))
#define CAF_UNLOCK      (uiAAAFlags |=   AAA_FLAG_CAF_ENABLE)
//#NT#2011/10/18#Jarkko Chang -end

#define AE_CAL_ON       (uiAAAFlags |=   AAA_FLAG_AE_CALIBRATION)
#define AE_CAL_OFF      (uiAAAFlags &= (~AAA_FLAG_AE_CALIBRATION))
#define AWB_CAL_ON      (uiAAAFlags |=   AAA_FLAG_AWB_CALIBRATION)
#define AWB_CAL_OFF     (uiAAAFlags &= (~AAA_FLAG_AWB_CALIBRATION))
#define FLASH_CAL_ON    (uiAAAFlags |=   AAA_FLAG_FLASH_CALIBRATION)
#define FLASH_CAL_OFF   (uiAAAFlags &= (~AAA_FLAG_FLASH_CALIBRATION))
#define MSH_CAL_ON      (uiAAAFlags |=   AAA_FLAG_MSH_CALIBRATION)
#define MSH_CAL_OFF     (uiAAAFlags &= (~AAA_FLAG_MSH_CALIBRATION))
#define OB_CAL_ON       (uiAAAFlags |=   AAA_FLAG_OB_CALIBRATION)
#define OB_CAL_OFF      (uiAAAFlags &= (~AAA_FLAG_OB_CALIBRATION))
#define VIG_CAL_ON      (uiAAAFlags |=   AAA_FLAG_VIG_CALIBRATION)
#define VIG_CAL_OFF     (uiAAAFlags &= (~AAA_FLAG_VIG_CALIBRATION))

#define LCD_Y(Yaddr,offset,i,j)  *(volatile unsigned int *)(Yaddr+(offset*(j))+4*(i/4))

#define CA_WINDOW_X             8
#define CA_WINDOW_Y             8
#define CA_Window_Num           (CA_WINDOW_X*CA_WINDOW_Y)
#define VA_WINDOW_X             3
#define VA_WINDOW_Y             3
#define VA_Window_Num           (VA_WINDOW_X*VA_WINDOW_Y)

#define AWBScaleBit             8
#define LOCK                    1
#define UNLOCK                  0

//channel define for aaa_EstimateY()
#define CA_CHANNEL_R            0
#define CA_CHANNEL_G            1
#define CA_CHANNEL_B            2
#define CA_CHANNEL_Y            3

#define CA_SOURCE_PRECG         0
#define CA_SOURCE_POSTCG        1
#define CA_SOURCE_POSTGAMMA     2

#define VA_SOURCE_EVAR          0
#define VA_SOURCE_DIFF          1
#define VA_SOURCE_EEXT          2

#define STARTPIXEL_R            0x1b//R Gr Gb B : 8bit to present RGrGbB order 00 01 10 11 = 0x1b
#define STARTPIXEL_Gr           0x4e//Gr R B Gb : 8bit to present RGrGbB order
#define STARTPIXEL_Gb           0xb1
#define STARTPIXEL_B            0xe4

/**
    VGA Format Selection.

    Select VGA format.
*/
//@{
typedef enum
{
    PROGRESSIVE = 0, ///< progressive data
    TWO_LINE = 1, ///< two-line interleaved data
    THREE_LINE = 2, ///< three-line interleaved data
    ENUM_DUMMY4WORD(Sensor_FormatEnum)
} Sensor_FormatEnum;

// define the color accumulation structure
typedef struct _STR_AAA_CA_PARA{
    UINT32 DramAddr;
    UINT32 DramLofs;
    UINT32 ImgHsize;
    UINT32 ImgVsize;
    UINT32 *R_Data;
    UINT32 *G_Data;
    UINT32 *B_Data;
    UINT32 Sub_rate;
    UINT32 Win_x;
    UINT32 Win_y;
    UINT32 Cfapat;
    UINT32 bitdepth;
    Sensor_FormatEnum fmt;
}STR_AAA_CA_PARA;
UINT32  aaa_BlockAverageY(UINT32 YAddr,UINT32 x_start,UINT32 y_start,UINT32 width,UINT32 height,UINT32 offset);

// -------------------------------------------------
// The general api for AAA driver
//
// ---- AE ----
extern void     aaa_Read_HIST_RSLT(UINT32 *Hist);
extern void     aaa_SetAeWindow(UINT8 *Window);
extern UINT32   aaa_EstimateY2(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset);//Estimate Y at IME output;
extern UINT32   aaa_EstimateY_Spot(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset);//Estimate Y at IME output
extern void     aaa_EstBlkAverBayerG(UINT32 Addr,UINT32 h_size,UINT32 v_size,UINT32 Linoft,UINT32 Cfa,UINT32* BlockY,UINT32 TblSize,UINT32* Hist,UINT32 HistSize,UINT32 BinNum);
extern void     aaa_SetAeRate(UINT8 rate);
extern UINT8    aaa_GetAeRate(void);
extern void     aaa_AeLock(UINT8 lock);
extern BOOL     Is_AELock(void);

// ---- AWB ----
extern void     aaa_SetAwbRate(UINT8 rate);
extern UINT8    aaa_GetAwbRate(void);
extern void     aaa_AwbLock(UINT8 lock);
extern BOOL     Is_AWBLock(void);
extern void     aaa_GetCAResult(STR_AAA_CA_PARA * pCA_PARA);

//#NT#2011/10/18#Jarkko Chang -begin
//#NT#add caf 
// ---- AF/CAF ----
extern void aaa_SetCafRate(UINT8 rate);
extern UINT8 aaa_GetCafRate(void);
extern void aaa_CafLock(UINT8 lock);
extern BOOL Is_CAFLock(void);
extern UINT32   aaa_GetVariation(UINT8 i);
//#NT#2011/10/18#Jarkko Chang -end

// ---- Calibration ----
extern void     aaa_SetAeCalibration(UINT8 cal);
extern UINT32   aaa_GetAeCalibration(void);
extern void     aaa_SetAwbCalibration(UINT8 cal);
extern UINT32   aaa_GetAwbCalibration(void);
extern void     aaa_SetFlashCalibration(UINT8 cal);
extern UINT32   aaa_GetFlashCalibration(void);
extern void     aaa_SetMshCalibration(UINT8 cal);
extern UINT32   aaa_GetMshCalibration(void);
extern void     aaa_SetOBCalibration(UINT8 cal);
extern UINT32   aaa_GetOBCalibration(void);
extern void     aaa_SetVIGCalibration(UINT8 cal);
extern UINT32   aaa_GetVIGCalibration(void);

// ---- Option ----
extern UINT8    aaa_Get_AE_BlockValue(UINT8 i, UINT8 j);
extern UINT32 aaa_MotionDetect(UINT8 *Block_Avg_Now, UINT8 Yavg, UINT8 WinNumX, UINT8 WinNumY);
#endif
//@}