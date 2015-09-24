#include "OV9712.h"
#include "debug.h"
const SENSOR_CMD OV9712_INI_REG[]=
{
    {0x96, 0x41},
    {0xBC, 0x68},
    {0x12, 0x00},
    {0x3B, 0x00},
    {0x97, 0x00},
    {0x15, 0x00},
    {0x17, 0x25},
    {0x18, 0xA2},
    {0x19, 0x00},
    {0x1A, 0xCA},
    {0x03, 0x0A},
    {0x32, 0x07},
    {0x98, 0x00},
    {0x99, 0x00},
    {0x9A, 0x00},
    {0x57, 0x00},
    {0x58, 0xC8},
    {0x59, 0xA2},
    {0x4C, 0x13},
    {0x4B, 0x36},
    {0x3D, 0x3C},
    {0x3E, 0x03},
    {0x4E, 0x55},
    {0x4F, 0x55},
    {0x50, 0x55},
    {0x51, 0x55},
    {0x24, 0x55},
    {0x25, 0x48},
    {0x26, 0x92},
    {0x5C, 0x59},   //sensor pll setting
    {0x5D, 0x00},   //sensor pll setting
    {0x11, 0x00},   //sensor pll setting
    {0x2A, 0x9B},
    {0x2B, 0x06},
    {0x2D, 0x00},
    {0x2E, 0x00},
    {0x41, 0x81},
    {0x13, 0x00},
    {0x14, 0x40},

    // General setting
    {0x1E, 0x07},
    {0x5F, 0x18},
    {0x69, 0x04},
    {0x65, 0x2A},
    {0x68, 0x0A},
    {0x39, 0x28},
    {0x4D, 0x90},
    {0xC1, 0x80},
    {0x2A, 0x98},
    {0x96, 0xC1},
    {0x6D, 0x02}
};

/*
OV9712 PLL Setting
CLK1 = MCLK/REG5C[6:5]
CLK2 = CLK1*(32-REG5C[4:0])
CLK3 = CLK2/(REG5D[3:2]+1)
PCLK = CLK3/((REG11[5:0]+1)*2)
*/

const SENSOR_CMD OV9712_INIT_REG_15FPS[]=
{
    {0x04, 0x00},
    {0x5C, 0x59},   //sensor pll setting
    {0x5D, 0x00},   //sensor pll setting
    {0x11, 0x01},   //sensor pll setting IN 24 Out 21
//    {0x11, 0x00},   //sensor pll setting IN 24 Out 42
};

const SENSOR_CMD OV9712_INIT_REG_20FPS[]=
{
    {0x04, 0x00},
    {0x5C, 0x52},   //sensor pll setting
    {0x5D, 0x08},   //sensor pll setting
    {0x11, 0x00},   //sensor pll setting IN 24 Out 28
};

const SENSOR_CMD OV9712_INIT_REG_25FPS[]=
{
    {0x04, 0x00},
    {0x5C, 0x5A},   //sensor pll setting
    {0x5D, 0x00},   //sensor pll setting
    {0x11, 0x00},   //sensor pll setting IN 24 Out 36
};

SENSOR_CMD OV9712_REG_FULL[]=
{
    {0x04, 0x00},
    {0x5C, 0x59},   //sensor pll setting
    {0x5D, 0x00},   //sensor pll setting
    {0x11, 0x00},   //sensor pll setting
};

/**
* ISO to AGC usage
*/
#define AGC_ISOMin       0
#define ISOBASE         100

/**************Default Sensor mode selection for preview**************/

/************** STILL_MODE **************/
#define VD_Sync_Cap               1
#define HD_Sync_Cap               1
#define VD_Total_Cap              804
#define HD_Total_Cap              1688

#define IMAGESIZE_H_CAP           1280
#define IMAGESIZE_V_CAP           800

#define ACT_STRX_Cap              9// 8
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
#define PRV_OB_X0           1264
#define PRV_OB_X1           4900
#define PRV_OB_Y0           9
#define PRV_OB_Y1           9

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
#define PCLK      42
#define PCLK_15FPS  21
#define PCLK_20FPS  28
#define PCLK_25FPS  36
#define PCLK_STILL  42

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

CAPTURE_DATA_INFO OV9712_15fps_mode[] = {
    {
        1280, // cap_width
        800, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_15FPS, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)OV9712_REG_FULL,
        //sizeof(OV9712_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO OV9712_20fps_mode[] = {
    {
        1280, // cap_width
        800, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_20FPS, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)OV9712_REG_FULL,
        //sizeof(OV9712_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO OV9712_25fps_mode[] = {
    {
        1280, // cap_width
        800, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_25FPS, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)OV9712_REG_FULL,
        //sizeof(OV9712_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO OV9712_still_mode[] = {
    {
        1280, // cap_width
        800, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_STILL, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)OV9712_REG_FULL,
        //sizeof(OV9712_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void OV9712_GetRegInfo(OV9712_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV9712_INIT:
            *addr = (UINT32)OV9712_INI_REG;
            *size = (sizeof(OV9712_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case OV9712_1920X1080:
            *addr = (UINT32)OV9712_INIT_REG_25FPS;
            *size = (sizeof(OV9712_INIT_REG_25FPS)/sizeof(SENSOR_CMD));
//            *addr = (UINT32)OV9712_INIT_REG_20FPS;
//            *size = (sizeof(OV9712_INIT_REG_20FPS)/sizeof(SENSOR_CMD));
//            *addr = (UINT32)OV9712_INIT_REG_15FPS;
//            *size = (sizeof(OV9712_INIT_REG_15FPS)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case OV9712_STILL:
        case OV9712_1440X1080:
            *addr = (UINT32)OV9712_REG_FULL;
            *size = (sizeof(OV9712_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV9712_GetSigInfo(OV9712_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV9712_STILL:
        case OV9712_1440X1080:
        case OV9712_1920X1080:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case OV9712_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV9712_GetDataInfo(OV9712_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV9712_STILL:
        case OV9712_1440X1080:
            *addr = (UINT32)&OV9712_still_mode;
            //*size = (sizeof(OV9712_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case OV9712_1920X1080:
            *addr = (UINT32)&OV9712_25fps_mode;
//            *addr = (UINT32)&OV9712_20fps_mode;
//            *addr = (UINT32)&OV9712_15fps_mode;
            break;
        case OV9712_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV9712_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISOMin;
}

