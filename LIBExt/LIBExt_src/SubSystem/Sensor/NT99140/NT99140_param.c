#include "NT99140.h"
#include "debug.h"
const SENSOR_CMD NT99140_INI_REG[]=
{
    {0x32F0,0x01},
    {0x3028,0x05},
    {0x3029,0x02},
    {0x302a,0x00},
    {0x3336,0x20},
    {0x3270,0x00},
    {0x3271,0x04},
    {0x3272,0x0E},
    {0x3273,0x28},
    {0x3274,0x3F},
    {0x3275,0x50},
    {0x3276,0x6E},
    {0x3277,0x88},
    {0x3278,0xA0},
    {0x3279,0xB3},
    {0x327A,0xD2},
    {0x327B,0xE8},
    {0x327C,0xF5},
    {0x327D,0xFF},
    {0x327E,0xFF},
    {0x3302,0x00},
    {0x3303,0x01},
    {0x3304,0x00},
    {0x3305,0xD3},
    {0x3306,0x00},
    {0x3307,0x2B},
    {0x3308,0x07},
    {0x3309,0xA4},
    {0x330A,0x07},
    {0x330B,0x2D},
    {0x330C,0x01},
    {0x330D,0x30},
    {0x330E,0x00},
    {0x330F,0xC9},
    {0x3310,0x07},
    {0x3311,0x4D},
    {0x3312,0x07},
    {0x3313,0xEA},
    {0x32DB,0x90},
    {0x3300,0x70},
    {0x3301,0x40},
    {0x3024,0x00},

    {0x3040,0x0C},//{0x3040,0x04},
    {0x3041,0x02},
    {0x3042,0xFF},
    {0x3043,0x14},
    
    {0x3052,0xC0},
        
    {0x3053,0x10},
    {0x3057,0x80},
    {0x3058,0x00},
    {0x3059,0x2F},
    {0x305f,0x44},

    {0x32b0,0x00},
    {0x32b1,0x90},
    {0x32BB,0x1b},
    {0x32bd,0x10},
    {0x32be,0x05},
    {0x32bf,0x49},
    {0x32c0,0x40},
    {0x32C3,0x0F},
    {0x32c5,0x24},
    {0x32cd,0x01},
    {0x32d3,0x00},
    {0x32f6,0x0c},
    {0x3118,0xF2},
    {0x3119,0xF2},
    {0x311A,0x13},
    {0x3106,0x01},
    {0x3108,0x55},
    {0x3105,0x47},

    {0x3300,0x70},
    {0x3301,0x40},
    {0x3331,0x08},
    {0x3332,0x40},
    {0x32bf,0x49},
    {0x3200,0x3e},
    {0x3201,0x3f},
    {0x3012,0x01},// exposure time = 0x1B8
    {0x3013,0xB8},
    {0x3028,0x13},
    {0x3029,0x20},
    {0x302a,0x00},

    {0x3022,0x24},
    //{0x3022,0x27},// Rotate

    {0x3023,0x24},
    {0x3002,0x00},// X start = 0x2
    {0x3003,0x02},
    {0x3004,0x00},// Y start = 0x2
    {0x3005,0x02},
    {0x3006,0x05},// X end = 0x503
    {0x3007,0x03},
    {0x3008,0x02},// Y end = 0x2D3
    {0x3009,0xd3},
    {0x300a,0x07},// Hsync = 0x70E
    {0x300b,0x0E},
    {0x300c,0x02},// Vsync = 0x2E2
    {0x300d,0xE2},
    {0x300e,0x05},// Output H size = 0x502
    {0x300f,0x02},
    {0x3010,0x02},// Output V size = 0x2D2
    {0x3011,0xD2},

    {0x301d,0x24},// Global Analog gain
    
    {0x301E,0x12}, 
        
    {0x32bb,0x1b},
    {0x32bc,0x40},
    {0x32c1,0x2b},
    {0x32c2,0x82},
    {0x32c8,0xdc},
    {0x32c9,0xb8},
    {0x32c4,0x00},
    {0x3201,0x0f}, // Disable AE, AWB
    {0x3021,0x06},
    {0x3021,0x16},
    {0x32f0,0x5C},
    {0x306A,0x02}, // Driving 4mA
    {0x3106,0x03},
    {0x3060,0x01},

    //BP Setting
    {0x3080,0x40},
    {0x3081,0x00},
    {0x3082,0x0A},
    {0x3083,0x00},
    {0x3084,0xFF},
    {0x3085,0xFF},
    {0x3086,0xFF},
    {0x3087,0xE4},
    {0x3088,0x00},
    {0x3089,0xFF}
};

/*
const SENSOR_CMD NT99140_INIT_REG_25FPS[]=
{

};

SENSOR_CMD NT99140_REG_FULL[]=
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
#define VD_Total_Cap              738
#define HD_Total_Cap              1806

#define IMAGESIZE_H_CAP           1280
#define IMAGESIZE_V_CAP           720

#define ACT_STRX_Cap              3// 8
#define ACT_STRY_Cap              2// 1
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



CAPTURE_DATA_INFO NT99140_still_mode[] = {
    {
        1264, // cap_width
        708, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_STILL, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)NT99140_REG_FULL,
        //sizeof(NT99140_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void NT99140_GetRegInfo(NT99140_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99140_INIT:
            *addr = (UINT32)NT99140_INI_REG;
            *size = (sizeof(NT99140_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case NT99140_1280X720:
        case NT99140_1280X960:
        case NT99140_1440X1080:
        case NT99140_1920X1080:
        case NT99140_STILL:
            *addr = (UINT32)NT99140_INI_REG;
            *size = (sizeof(NT99140_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99140_GetSigInfo(NT99140_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99140_1280X720:
        case NT99140_1280X960:
        case NT99140_1440X1080:
        case NT99140_1920X1080:
        case NT99140_STILL:
            *addr = (UINT32)&still_mode;
            *size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case NT99140_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99140_GetDataInfo(NT99140_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99140_1280X720:
        case NT99140_1280X960:
        case NT99140_1440X1080:
        case NT99140_1920X1080:
        case NT99140_STILL:
            *addr = (UINT32)&NT99140_still_mode;
            *size = (sizeof(NT99140_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case NT99140_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99140_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISOBASE;
}

