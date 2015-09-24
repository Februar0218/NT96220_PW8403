#include "NT99141.h"
#include "debug.h"
const SENSOR_CMD NT99141_INI_REG[]=
{
{0x3109, 0x04},
{0x3040, 0x04},
{0x3041, 0x02},
{0x3042, 0xFF},
{0x3043, 0x08},
{0x3052, 0xE0},
{0x305F, 0x33},
{0x3106, 0x03},
{0x3100, 0x03},
{0x3108, 0xF0},
{0x3110, 0x22},
{0x3111, 0x54},
{0x3112, 0x22},
{0x3113, 0x44},
{0x3114, 0x04},
{0x3135, 0x00},
{0x3200, 0x00},
{0x3201, 0x08},
{0x32BF, 0x60},
{0x32C0, 0x58},
{0x32C1, 0x58},
{0x32C2, 0x58},
{0x32C3, 0x00},
{0x32C4, 0x20},
{0x32C5, 0x10},
{0x32C6, 0x18},
{0x32C7, 0x00},
{0x32C8, 0x78},
{0x32C9, 0x58},
{0x32CA, 0x68},
{0x32CB, 0x68},
{0x32CC, 0x70},
{0x32CD, 0x70},
{0x32DB, 0x6E},
{0x3028, 0x27},
{0x3029, 0x20},
{0x302A, 0x04},
{0x3022, 0x24},
//{0x3022, 0x27}, //Rotate
{0x3023, 0x24},
{0x3002, 0x00},
{0x3003, 0x02},
{0x3004, 0x00},
{0x3005, 0x02},
{0x3006, 0x05},
{0x3007, 0x05},
{0x3008, 0x02},
{0x3009, 0xD5},
{0x300A, 0x07}, 
{0x300B, 0x13}, 
{0x300C, 0x02},
{0x300D, 0xe0},
{0x300E, 0x05},
{0x300F, 0x04},
{0x3010, 0x02},
{0x3011, 0xD4},
{0x301E, 0x12},
{0x32B8, 0x48},
{0x32B9, 0x38},
{0x32BB, 0x87},
{0x32BC, 0x40},
{0x32BD, 0x44},
{0x32BE, 0x3C},
{0x3021, 0x06},
{0x32f0, 0x50},
{0x3024, 0x00},

// NR
{0x3363, 0x07},
{0x333F, 0x07},
{0x3360, 0x08}, // IQ_PARAM_0
{0x3361, 0x18}, // IQ_PARAM_1
{0x3362, 0x30}, // IQ_PARAM_2
{0x336D, 0x20}, // DPC_0
{0x336E, 0x1C},// DPC_1
{0x336F, 0x18},// DPC_2
{0x3370, 0x12},// DPC_3
{0x3371, 0x10}, // NRW_0	
{0x3372, 0x20},// NRW_1
{0x3373, 0x30},// NRW_2
{0x3374, 0x3F},// NRW_3

#if 1
{0x3375, 0x04}, // NTHR_0
{0x3376, 0x06},// NTHR_1
{0x3377, 0x08},// NTHR_2
{0x3378, 0x10},// NTHR_3
#else
{0x3375, 0x04}, // NTHR_0
{0x3376, 0x06},// NTHR_1
{0x3377, 0x08},// NTHR_2
{0x3378, 0x0A},// NTHR_3
#endif

{0x3379, 0x10}, // COMPMAX_0
{0x337A, 0x10},// COMPMAX_1
{0x337B, 0x10},// COMPMAX_2
{0x337C, 0x10},// COMPMAX_3
{0x3021, 0x06}, 
{0x32F0, 0x80}, // Output: 0x50 for Raw, 0x80 for NR-Raw
{0x3024, 0x00},
// Exposure
{0x3012, 0x03},
{0x3013, 0x20},
{0x301D, 0x00},
{0x3060, 0x01},
};

/*
const SENSOR_CMD NT99141_INIT_REG_25FPS[]=
{

};

SENSOR_CMD NT99141_REG_FULL[]=
{
};
*/

/**
* ISO to AGC usage
*/
#define AGC_ISOBASE       0// 1000
#define ISOBASE         100

/**************Default Sensor mode selection for preview**************/

/************** STILL_MODE **************/
#define VD_Sync_Cap               1
#define HD_Sync_Cap               1
#define VD_Total_Cap              736//738
#define HD_Total_Cap              1812//1806

#define IMAGESIZE_H_CAP           1280
#define IMAGESIZE_V_CAP           720

#define ACT_STRX_Cap              4//3// 8
#define ACT_STRY_Cap              3//2// 1
#define ACT_STRY_Cap2             0xff
#define ACT_STRY_Cap3             0xff
#define ACT_STRY_Cap4             0xff
#define ACT_STRY_Cap5             0xff
#define ACT_STRY_Cap6             0xff
#define ACT_STRY_Cap7             0xff
#define ACT_STRY_Cap8             0xff

#define SensorCapTotalFld         1
#define Field_Num1                1
#define Field_Num2                0xff
#define Field_Num3                0xff
#define Field_Num4                0xff
#define Field_Num5                0xff
#define Field_Num6                0xff
#define Field_Num7                0xff
#define Field_Num8                0xff

#define Padding_Cap               0
#define SIE_CAP_XCENTER           (ACT_STRX_Cap+(IMAGESIZE_H_CAP/2))
#define SIE_CAP_YCENTER           (ACT_STRY_Cap+(IMAGESIZE_V_CAP/2))
/**************Default Sensor mode selection for preview**************/
//#define SENSOR_PRV_MODE             MONITOR_MODE_3
//#define SENSOR_AF_MODE              MONITOR_MODE_4
//#define SENSOR_PRV_VGATYPE          SENSOR_OUT_PROGRESSIVE

/************** OB Setting **************/
#define PRV_OB_GAIN         0x20
#define PRV_OB_THRES        0xF0
#define PRV_OB_SUBSAMPLE    0x02
#define PRV_OB_X0           2
#define PRV_OB_X1           1280
#define PRV_OB_Y0           2
#define PRV_OB_Y1           720

#define CAP_OB_GAIN         0x20
#define CAP_OB_THRES        0xF0
#define CAP_OB_SUBSAMPLE    0x05
#define CAP_OB_X0           1200
#define CAP_OB_X1           4900
#define CAP_OB_Y0           17
#define CAP_OB_Y1           17

#define OB_OFS_PRV          1// 10 bit
#define OB_OFS_CAP          1// 10 bit

/************** SIE CFA pattern selection **************/
#define CFAPAT_PRV    SIECFA_R
#define CFAPAT_CAP    SIECFA_R

/*********************************************/
//unit:mhz
#define PCLK      40
//#define PCLK_15FPS  21
//#define PCLK_20FPS  28
//#define PCLK_25FPS  36
#define PCLK_STILL  40

SYNC_INFO still_mode = {
    ACT_STRX_Cap, // act_start_x;
    ACT_STRY_Cap, // act_start_y;
    SIE_CAP_XCENTER, // act_center_x;
    SIE_CAP_YCENTER, // act_center_y;
    IMAGESIZE_H_CAP  , // Act_SizeX;
    IMAGESIZE_V_CAP, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Cap, //    HD_sync
    VD_Sync_Cap, //    VD_sync
    HD_Total_Cap, //    HD_total
    VD_Total_Cap, //    VD_total

    // ob_area
    CAP_OB_X0, //    OB_X0
    CAP_OB_Y0, //    OB_Y0
    CAP_OB_X1, //    OB_X1
    CAP_OB_Y1, //    OB_Y1
    CAP_OB_THRES, //    OB_Thres
    CAP_OB_SUBSAMPLE, //    OB_SubSample
    CAP_OB_GAIN, // OB_Gain
    OB_OFS_CAP, // OB_Offset;

    Padding_Cap // padding;
};



CAPTURE_DATA_INFO NT99141_still_mode[] = {
    {
        1264, // cap_width
        708, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_STILL, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)NT99141_REG_FULL,
        //sizeof(NT99141_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void NT99141_GetRegInfo(NT99141_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99141_INIT:
            *addr = (UINT32)NT99141_INI_REG;
            *size = (sizeof(NT99141_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case NT99141_1280X720:
        case NT99141_1280X960:
        case NT99141_1440X1080:
        case NT99141_1920X1080:
        case NT99141_STILL:
            *addr = (UINT32)NT99141_INI_REG;
            *size = (sizeof(NT99141_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99141_GetSigInfo(NT99141_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99141_1280X720:
        case NT99141_1280X960:
        case NT99141_1440X1080:
        case NT99141_1920X1080:
        case NT99141_STILL:
            *addr = (UINT32)&still_mode;
            *size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case NT99141_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99141_GetDataInfo(NT99141_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99141_1280X720:
        case NT99141_1280X960:
        case NT99141_1440X1080:
        case NT99141_1920X1080:
        case NT99141_STILL:
            *addr = (UINT32)&NT99141_still_mode;
            *size = (sizeof(NT99141_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case NT99141_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99141_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISOBASE;
}

