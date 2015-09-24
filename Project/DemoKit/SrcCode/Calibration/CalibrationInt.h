/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationInt.h
    @ingroup    mIPRJAPTest

    @brief      Internal header file of calibration task
                Internal header file of calibration task

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{
#ifndef _CALIBRATIONINT_H
#define _CALIBRATIONINT_H

#include <stdio.h>
#include "Type.h"

#define CAL_INVALID_DEFAULT         0xFFFFFFFF
#define KEY_EVENT_CAL               (FLGKEY_UP | FLGKEY_DOWN | FLGKEY_SHUTTER2 | FLGKEY_MENU | FLGKEY_RIGHT | FLGKEY_LEFT | FLGKEY_PLAYBACK |FLGKEY_ENTER)
#define CAL_AUTO                    DISABLE

/**
  Calibration Error code
*/
#define CAL_ERR_OK 0
#define CAL_ERR_ISO_MIN 11
#define CAL_ERR_ISO_MAX 12
#define CAL_ERR_MSHUTTER_MIN 16
#define CAL_ERR_MSHUTTER_MAX 17
#define CAL_ERR_FTP_MIN 21//flash trigger point
#define CAL_ERR_FTP_MAX 22
#define CAL_ERR_DPPW_MAX 31//defect pixels of preview white over the max
#define CAL_ERR_DPPB_MAX 32//defect pixels of preview black over the max
#define CAL_ERR_DPCW_MAX 33//defect pixels of capture white over the max
#define CAL_ERR_DPCB_MAX 34//defect pixels of capture black over the max
#define CAL_ERR_DPDW_MAX 35//defect pixels of dzoom white over the max
#define CAL_ERR_DPDB_MAX 36//defect pixels of dzoom black over the max
#define CAL_ERR_DP_CTNE_PRV 37
#define CAL_ERR_DP_CTNE_CAP 38
#define CAL_ERR_DP_CTNE_DZOOM 39
#define CAL_ERR_AWBR_MIN 51
#define CAL_ERR_AWBR_MAX 52
#define CAL_ERR_AWBB_MIN 53
#define CAL_ERR_AWBB_MAX 54
#define CAL_ERR_AWBGS_NO 55
#define CAL_ERR_FYAVGP_MIN 61
#define CAL_ERR_FYAVGP_MAX 62
#define CAL_ERR_FYVAR_MAX   63
#define CAL_ERR_FWBR_MIN 66
#define CAL_ERR_FWBR_MAX 67
#define CAL_ERR_FWBB_MIN 68
#define CAL_ERR_FWBB_MAX 69
//#define CAL_ERR_FAGCP_MIN 61
//#define CAL_ERR_FAGCP_MAX 62
//#define CAL_ERR_FAGCC_MIN 63
//#define CAL_ERR_FAGCC_MAX 64
#define CAL_ERR_FAGC_BRIGHT 66
#define CAL_ERR_DPFIELD_MAX 72//defect pixels of certain field over the max

#define CAL_ERR_VIGXCENT_MIN  73
#define CAL_ERR_VIGXCENT_MAX  74

#define CAL_ERR_VIGYCENT_MIN  75
#define CAL_ERR_VIGYCENT_MAX  76

#define CAL_ERR_VIGGAIN_MAX   77
#define CAL_ERR_LSC_FAIL      101
#define CAL_ERR_PS_FAIL       105
#define CAL_ERR_NULL          255

/**
    Calibration Item
*/
typedef enum _CAL_ITEM
{
    CAL_ITEM_ISO = 0,                ///< Calibration Station CAL_ISO
    CAL_ITEM_MSHUTTER,           ///< Calibration Station CAL_MSHUTTER
    CAL_ITEM_MSHUTTER_sIris,     ///< Calibration Station CAL_MSHUTTER_sIris
    CAL_ITEM_DEFECTPIXEL,        ///< Calibration Station CAL_DEFECTPIXEL
    CAL_ITEM_AWB,                ///< Calibration Station CAL_AWB
    CAL_ITEM_SAVEAWBGS,          ///< Calibration Station Cal_WriteAWBGS
    CAL_ITEM_FLASH,              ///< Calibration Station CAL_FLASH
    CAL_ITEM_FLASH_AWB,          ///< Calibration Station CAL_FLASH_AWB
    CAL_ITEM_FLASH_REDEYE_TEST,  ///< Calibration Station CAL_FLASH_REDEYE_TEST
    CAL_ITEM_FLASH_GNO_SHOTS,    ///< Calibration Station CAL_FLASH_GNO_SHOTS
    CAL_ITEM_AF_FAR,             ///< Calibration Station CAL_AF_FAR
    CAL_ITEM_AF_NEAR,            ///< Calibration Station CAL_AF_NEAR
    CAL_ITEM_ECS,                ///< Calibration Station CAL_ECS
    CAL_ITEM_WR2FILE,            ///< Calibration Station CAL_WR2FILE
    CAL_ITEM_READSTATUS,         ///< Calibration Station CAL_READSTATUS
    CAL_ITEM_DNPSTATION,         ///< Calibration Station CAL_DNPSTATION
    CAL_ITEM_LENS,               ///< Calibration Station CAL_LENS
    CAL_ITEM_VIG,                ///< Calibration Station CAL_VIG
    CAL_ITEM_DEFECTPIXEL_WHITE,  ///< Calibration Station CAL_DEFECTPIXEL_WHITE
    CAL_ITEM_DEFECTPIXEL_DARK,   ///< Calibration Station CAL_DEFECTPIXEL_DARK
    CAL_ITEM_MSHUTTER_LASTING,   ///< Calibration Station CAL_MSHUTTER_LASTING
    CAL_ITEM_MSHUTTER_SW_TEST,   ///< Calibration Station CAL_MSHUTTER_SW_TEST
    CAL_ITEM_APERTURE_SW_TEST,   ///< Calibration Station CAL_APERTURE_SW_TEST
    CAL_ITEM_FAE_TEST,           ///< Calibration Station CAL_FAE_TEST
    CAL_ITEM_ZOOM_SPEED_TEST,    ///< Calibration Station CAL_ZOOM_SPEED_TEST
    CAL_ITEM_ZOOM_STEPMISS_TEST, ///< Calibration Station CAL_ZOOM_STEPMISS_TEST
    CAL_ITEM_FOCUS_STEPMISS_TEST,///< Calibration Station CAL_FOCUS_STEPMISS_TEST
    CAL_ITEM_AF_TEST,            ///< Calibration Station CAL_AF_TEST
    CAL_ITEM_STAIN_TEST,         ///< Calibration Station CAL_STAIN_TEST
    CAL_ITEM_TVRESOLUTION_TEST,  ///< Calibration Station CAL_TVRESOLUTION_TEST
    CAL_ITEM_ZOOM_STOP2PR,       ///< Calibration Station CAL_ZOOM_STOP2PR
    CAL_ITEM_FOCUSOFFSET,        ///< Calibration Station CAL_FOCUSOFFSET
    CAL_ITEM_SHTRCLOSE_TEST ,
    CAL_ITEM_SAVEAF_TEST,
    CAL_ITEM_ClearStatus,
    CAL_ITEM_DEFECTLINE,
    CAL_ITEM_SAVEWB_TEST,
    CAL_ITEM_LC_LET,
    CAL_ITEM_LC_ISO,
    CAL_ITEM_EFA_SCAN,
    CAL_ITEM_TOTAL,
    ENUM_DUMMY4WORD(CAL_ITEM)
} CAL_ITEM;

typedef struct
{
    INT8    *pItemName;
    FP      ItemFunction;
} CAL_TERM;

extern volatile BOOL g_bCalbirationOpened;
extern RECT g_CalStringRect;
extern UINT32 g_CalMaxItemNum;
extern CAL_TERM g_CalItem[];
extern UINT32 g_CalMaxItemNum;

/**
  Calibration function protptype
*/
extern UINT32 Mshutter_Adjust(void);
extern UINT32 ISO_Adjust(void);
extern UINT32 AWB_Adjust(void);
extern UINT32 LSC_Adjust(void);
extern UINT32 DP_Adjust(void);
extern UINT32 ECS_Adjust(void);

/**
  Calibration common API
*/
// Entry of different station
extern UINT32 Cal_RunItem(CAL_ITEM AlgCal_item);

extern UINT32 Cal_GetDefaultCal(INT8 * pFileName);
extern UINT32 Cal_GetDefaultInRoot(INT8 * pExt);

extern void Cal_SetDefault(void);
extern void Cal_NoFunction(void);
extern void Cal_FormatPstoreSPI(void);
extern ER Cal_ReadCalData(void);//ethan_20110801_add
extern ER Cal_ShowCalRst(void); //ethan_20110801_add
extern ER Cal_SaveAWBGS(void);//ethan_20110801_add
extern void Cal_RstCalRst(void);
#endif

//@}
