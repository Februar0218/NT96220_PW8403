#include "OV5633.h"
#include "debug.h"
const SENSOR_CMD OV5633_INI_REG[]=
{
{0x30fa ,0x00},
{0x300f ,0x00},
{0x30b2 ,0x20},//~~~
{0x3084 ,0x40},
{0x3016 ,0x01},
{0x308a ,0x25},
{0x3013 ,0x00},
{0x3015 ,0x03},
{0x3065 ,0x50},
{0x3068 ,0x08},
{0x30bf ,0x02},
{0x3069 ,0x00},
{0x30ac ,0x05},
{0x309d ,0x00},
{0x309e ,0x24},
{0x3098 ,0x5c},
{0x3091 ,0x04},
{0x3075 ,0x22},
{0x3076 ,0x23},
{0x3077 ,0x24},
{0x3078 ,0x25},
{0x30b5 ,0x0c},
{0x3090 ,0x67},
{0x30f9 ,0x11},
{0x3311 ,0x80},
{0x3312 ,0x1f},
{0x3103 ,0x10},
{0x305c ,0x01},
{0x305d ,0x29},
{0x305e ,0x00},
{0x305f ,0xf7},
{0x308d ,0x0b},
{0x30ad ,0x20},
{0x3072 ,0x0d},
{0x308b ,0x82},
{0x3317 ,0x9c},
{0x3318 ,0x22},
{0x3025 ,0x20},
{0x3027 ,0x08},
{0x3029 ,0x3f},
{0x302b ,0xa3},
{0x3319 ,0x22},
{0x30a1 ,0xc4},
{0x306a ,0x05},
{0x3315 ,0x22},
{0x30ae ,0x25},
{0x3304 ,0x40},
{0x3099 ,0x49},
{0x30be ,0x50},
{0x300e ,0xb0},
{0x300f ,0x00},
{0x3010 ,0x03},
{0x3011 ,0x40},
{0x30f8 ,0x00},
{0x30f8 ,0x01},
{0x30f8 ,0x09},
{0x3083 ,0x00},
{0x3086 ,0x24},
{0x308c ,0x88},
{0x30f9 ,0x13},
{0x30fb ,0x81},
{0x308a ,0x26},
{0x3016 ,0x02},
{0x3069 ,0x80},
{0x309d ,0x04},
{0x309e ,0x24},
{0x3071 ,0x34},
{0x331f ,0x22},
{0x3098 ,0x54},
{0x331e ,0x07},
{0x3322 ,0x10},
{0x3323 ,0x10},
{0x331e ,0x05},
{0x30af ,0x10},
{0x304a ,0x00},
{0x304d ,0x00},
{0x304e ,0x22},
{0x304d ,0xa0},
{0x3058 ,0x00},
{0x3059 ,0xff},
{0x305a ,0x00},
{0x30e9 ,0x04},
{0x3090 ,0x67},
{0x3084 ,0x44},
{0x30e9 ,0x04},
{0x30b5 ,0x1c},
{0x331f ,0x22},
{0x3020 ,0x02},
{0x3021 ,0x04},
{0x3022 ,0x07},
{0x3023 ,0x80},
{0x3024 ,0x00},
{0x3025 ,0x2c},
{0x3026 ,0x00},
{0x3027 ,0x14},
{0x3028 ,0x0a},
{0x3029 ,0x2f},
{0x302a ,0x07},
{0x302b ,0xa3},
{0x302c ,0x02},
{0x302d ,0x80},
{0x302e ,0x01},
{0x302f ,0xe0},
{0x30b3 ,0x09},
{0x3301 ,0xc1},
{0x3313 ,0xf1},
{0x3314 ,0x05},
{0x3315 ,0x04},
{0x3316 ,0x01},
{0x3317 ,0xe4},
{0x3318 ,0x20},
{0x300f ,0x01},
{0x3022 ,0x08},
{0x3023 ,0x10},
{0x3300 ,0xd3},
{0x3302 ,0xe0},
{0x30ae ,0x15},
{0x3304 ,0x4c},
{0x30fa ,0x01},
{0x300f ,0x31},
{0x3018 ,0x3c},
{0x3019 ,0x38},
{0x301a ,0x82},
{0x301e ,0x00},
{0x301f ,0x00},
//{0x3002,0x03},
//{0x3003,0x20},
//{0x3000,0x20},

};


const SENSOR_CMD OV5633_REG_VGA[]=
{
    {0x3020 ,0x02},
    {0x3021 ,0x04},
    {0x3022 ,0x0c},
    {0x3023 ,0x1c},
    {0x3024 ,0x00},
    {0x3025 ,0x2c},
    //{0x3024 ,0x03},
    //{0x3025 ,0xed},

    {0x3026 ,0x00},
    {0x3027 ,0x14},
    //{0x3026 ,0x06},
    //{0x3027 ,0x6d},

    {0x3028 ,0x0a},
    {0x3029 ,0x2f},
    {0x302a ,0x07},
    {0x302b ,0xa3},
    {0x302c ,0x02},
    {0x302d ,0x80},
    //{0x302c ,0x05},
    //{0x302d ,0x00},

    {0x302e ,0x01},
    {0x302f ,0xe0},
    //{0x302e ,0x03},
    //{0x302f ,0xc0},

    {0x30b3 ,0x09},
    {0x3301 ,0xc1},
    {0x3313 ,0xf1},
    {0x3314 ,0x05},
    {0x3315 ,0x04},
    {0x3316 ,0x01},
    {0x3317 ,0xe4},
    {0x3318 ,0x20},
    {0x3070 ,0x24},
    {0x3092 ,0x00},
    {0x30f8 ,0x09},
    {0x3083 ,0x00},
    {0x3086 ,0x24},
    {0x308c ,0x88},
    {0x30f9 ,0x13},
    {0x30fb ,0x81},
    {0x308a ,0x26},
    {0x3016 ,0x02},
    {0x300e ,0xb0},
    {0x300f ,0x11},
    {0x3010 ,0x03},
    {0x3011 ,0x40},
        /*
    {0x301e ,0x00},
    {0x301f ,0x00}
    */

};

const SENSOR_CMD OV5633_REG_1280X720[]=
{
    {0x3020,0x03},
    {0x3021,0xe4},
    {0x3022,0x07},
    {0x3023,0xd6},//{0x3023,0x80},
    {0x3024,0x00},
    {0x3025,0x30},
    {0x3026,0x00},
    {0x3027,0x10},
    {0x3028,0x0a},
    {0x3029,0x2f},
    {0x302a,0x07},
    {0x302b,0x97},
    {0x302c,0x05},
    {0x302d,0x00},
    {0x302e,0x03},
    {0x302f,0xc0},
    {0x30b3,0x00},
    {0x3301,0xc0},
    {0x3313,0xf0},
    {0x3314,0x05},
    {0x3315,0x02},
    {0x3316,0x03},
    {0x3317,0xc4},
    {0x3318,0x22},
    {0x3070,0x24},
    {0x3092,0x00},
    {0x30f8,0x05},

    {0x3083,0x08},
    {0x3086,0x00},
    {0x308c,0x80},
    {0x30f9,0x11},
    {0x30fb,0x80},
    {0x308a,0x25},
    {0x3016,0x01},

    {0x300e,0xb6},
    {0x300f,0x00},//,0x00,0xfor,0x50fps,0xPCLK=96MHz,0x,,0x10,0xfor,0x25fps,0xPCLK=48MHz,,0x20,0xfor,0x16.7fps,0xPCLK=,0x32,0xMHz,,0x30,0xfor,0x12.5fps,0xPCLK=24MHz
    {0x3010,0x07},
    {0x3011,0x40},
/*
    {0x3002,0x00},
    {0x3003,0x00},

    {0x301e,0x00},
    {0x301f,0x00}
*/
};

const SENSOR_CMD OV5633_REG_1280X960[]=
{
    {0x3020,0x03},
    {0x3021,0xe4},
    {0x3022,0x07},
    {0x3023,0xd6},//{0x3023,0x80},
    {0x3024,0x00},
    {0x3025,0x30},
    {0x3026,0x00},
    {0x3027,0x10},
    {0x3028,0x0a},
    {0x3029,0x2f},
    {0x302a,0x07},
    {0x302b,0x97},
    {0x302c,0x05},
    {0x302d,0x00},
    {0x302e,0x03},
    {0x302f,0xc0},
    {0x30b3,0x00},
    {0x3301,0xc0},
    {0x3313,0xf0},
    {0x3314,0x05},
    {0x3315,0x02},
    {0x3316,0x03},
    {0x3317,0xc4},
    {0x3318,0x22},
    {0x3070,0x24},
    {0x3092,0x00},
    {0x30f8,0x05},

    {0x3083,0x08},
    {0x3086,0x00},
    {0x308c,0x80},
    {0x30f9,0x11},
    {0x30fb,0x80},
    {0x308a,0x25},
    {0x3016,0x01},

    {0x300e,0xb6},
    {0x300f,0x00},//,0x00,0xfor,0x50fps,0xPCLK=96MHz,0x,,0x10,0xfor,0x25fps,0xPCLK=48MHz,,0x20,0xfor,0x16.7fps,0xPCLK=,0x32,0xMHz,,0x30,0xfor,0x12.5fps,0xPCLK=24MHz
    {0x3010,0x07},
    {0x3011,0x40},
/*
    {0x3002,0x00},
    {0x3003,0x00},

    {0x301e,0x00},
    {0x301f,0x00}
*/
};

const SENSOR_CMD OV5633_REG_1280X480[]= //not ready
{
        {0x3020,0x03},
        {0x3021,0xe4},
        {0x3022,0x07},
        {0x3023,0xd6},//{0x3023,0x80},
        {0x3024,0x00},
        {0x3025,0x30},
        {0x3026,0x00},
        {0x3027,0x10},
        {0x3028,0x0a},
        {0x3029,0x2f},
        {0x302a,0x07},
        {0x302b,0x97},
        {0x302c,0x05},
        {0x302d,0x00},
        {0x302e,0x03},
        {0x302f,0xc0},
        {0x30b3,0x00},
        {0x3301,0xc0},
        {0x3313,0xf0},
        {0x3314,0x05},
        {0x3315,0x02},
        {0x3316,0x03},
        {0x3317,0xc4},
        {0x3318,0x22},
        {0x3070,0x24},
        {0x3092,0x00},
        {0x30f8,0x05},

        {0x3083,0x08},
        {0x3086,0x00},
        {0x308c,0x80},
        {0x30f9,0x11},
        {0x30fb,0x80},
        {0x308a,0x25},
        {0x3016,0x01},

        {0x300e,0xb6},
        {0x300f,0x00},//,0x00,0xfor,0x50fps,0xPCLK=96MHz,0x,,0x10,0xfor,0x25fps,0xPCLK=48MHz,,0x20,0xfor,0x16.7fps,0xPCLK=,0x32,0xMHz,,0x30,0xfor,0x12.5fps,0xPCLK=24MHz
        {0x3010,0x07},
        {0x3011,0x40},
};
const SENSOR_CMD OV5633_REG_2560X960[]= // not ready
{
        {0x3020,0x03},
        {0x3021,0xe4},
        {0x3022,0x07},
        {0x3023,0xd6},//{0x3023,0x80},
        {0x3024,0x00},
        {0x3025,0x30},
        {0x3026,0x00},
        {0x3027,0x10},
        {0x3028,0x0a},
        {0x3029,0x2f},
        {0x302a,0x07},
        {0x302b,0x97},
        {0x302c,0x05},
        {0x302d,0x00},
        {0x302e,0x03},
        {0x302f,0xc0},
        {0x30b3,0x00},
        {0x3301,0xc0},
        {0x3313,0xf0},
        {0x3314,0x05},
        {0x3315,0x02},
        {0x3316,0x03},
        {0x3317,0xc4},
        {0x3318,0x22},
        {0x3070,0x24},
        {0x3092,0x00},
        {0x30f8,0x05},

        {0x3083,0x08},
        {0x3086,0x00},
        {0x308c,0x80},
        {0x30f9,0x11},
        {0x30fb,0x80},
        {0x308a,0x25},
        {0x3016,0x01},

        {0x300e,0xb6},
        {0x300f,0x00},//,0x00,0xfor,0x50fps,0xPCLK=96MHz,0x,,0x10,0xfor,0x25fps,0xPCLK=48MHz,,0x20,0xfor,0x16.7fps,0xPCLK=,0x32,0xMHz,,0x30,0xfor,0x12.5fps,0xPCLK=24MHz
        {0x3010,0x07},
        {0x3011,0x40},
};

const SENSOR_CMD OV5633_REG_FULL[]=
{
    {0x3020 ,0x07},
    {0x3021 ,0xbc},
    {0x3022 ,0x0c},
    {0x3023 ,0xa0},
    {0x3024 ,0x00},
    {0x3025 ,0x20},
    {0x3026 ,0x00},
    {0x3027 ,0x08},
    {0x3028 ,0x0a},
    {0x3029 ,0x3f},
    {0x302a ,0x07},
    {0x302b ,0xa3},
    {0x302c ,0x0a},
    {0x302d ,0x20},
    {0x302e ,0x07},
    {0x302f ,0x98},
    {0x30b3 ,0x00},
    {0x3301 ,0xc0},
    {0x3313 ,0xf0},
    {0x3314 ,0x0a},
    {0x3315 ,0x22},
    {0x3316 ,0x07},
    {0x3317 ,0x9c},
    {0x3318 ,0x22},

    {0x3070 ,0x24},
    {0x3092 ,0x00},
    {0x30f8 ,0x00},

    {0x3083 ,0x08},
    {0x3086 ,0x00},
    {0x308c ,0x80},
    {0x30f9 ,0x11},
    {0x30fb ,0x80},
    {0x308a ,0x25},
    {0x3016  ,0x01},

    {0x300e ,0xb5},//{0x300e ,0xb0},
    {0x300f ,0x0},//{0x300f ,0x10}, //30 for 3.75fps PCLK=24MHz, 20 for 5fps PCLK=32MHz, 10 for 7.5fps PCLK=48MHZ, 00 for 15fps PCLK=96fps
    //#NT#2010/08/24#ethanlau -begin
    //#NT# mclk = 24, pclk=24
    /*
    {0x300e ,0xb0},
    {0x300f ,0x30},
    */
    //#NT#2010/08/24#ethanlau -end
    {0x3010 ,0x07},
    {0x3011 ,0x40},
    //{0x301e ,0x00},
    //{0x301f ,0x00}
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       2000
#define ISOBASE         50

/************** MONITOR_MODE_1(VGA) **************/

#define VD_Sync_Prv_1             2
#define HD_Sync_Prv_1             2
#define VD_Total_Prv_1            516
#define HD_Total_Prv_1            1550

#define ACT_STRX_Prv_1            4
#define ACT_STRY_Prv_1            3

#define IMAGESIZE_H_PRV_1         640
#define IMAGESIZE_V_PRV_1         480
#define SIE_PRV_XCENTER_1         (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_1/2))
#define SIE_PRV_YCENTER_1         (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_1/2))

#define Padding_Prv_1               0

/************** MONITOR_MODE_2(720p) **************///not ready
#define VD_Sync_Prv_2             2
#define HD_Sync_Prv_2             2
#define VD_Total_Prv_2            996
#define HD_Total_Prv_2            2006

#define ACT_STRX_Prv_2            4
#define ACT_STRY_Prv_2            3

#define IMAGESIZE_H_PRV_2         1264
#define IMAGESIZE_V_PRV_2         948
#define SIE_PRV_XCENTER_2         (ACT_STRX_Prv_2+(IMAGESIZE_H_PRV_2/2))
#define SIE_PRV_YCENTER_2         (ACT_STRY_Prv_2+(IMAGESIZE_V_PRV_2/2))

#define Padding_Prv_2             0

/************** MONITOR_MODE_3(1280*960)**************/
#define VD_Sync_Prv_3             2
#define HD_Sync_Prv_3             2

#define VD_Total_Prv_3            996
#define HD_Total_Prv_3            2006


#define ACT_STRX_Prv_3            5// 4
#define ACT_STRY_Prv_3            3

#define IMAGESIZE_H_PRV_3         1264
#define IMAGESIZE_V_PRV_3         948
#define SIE_PRV_XCENTER_3         (ACT_STRX_Prv_3+(IMAGESIZE_H_PRV_3/2))
#define SIE_PRV_YCENTER_3         (ACT_STRY_Prv_3+(IMAGESIZE_V_PRV_3/2))


#define Padding_Prv_3             0

/************** MONITOR_MODE_4(1280*480)**************///not ready

#define VD_Sync_Prv_4             2
#define HD_Sync_Prv_4             2

#define VD_Total_Prv_4            996
#define HD_Total_Prv_4            2006


#define ACT_STRX_Prv_4            4
#define ACT_STRY_Prv_4            3

#define IMAGESIZE_H_PRV_4         1264
#define IMAGESIZE_V_PRV_4         948
#define SIE_PRV_XCENTER_4         (ACT_STRX_Prv_3+(IMAGESIZE_H_PRV_4/2))
#define SIE_PRV_YCENTER_4         (ACT_STRY_Prv_3+(IMAGESIZE_V_PRV_4/2))


#define Padding_Prv_4             0
/************** MONITOR_MODE_5(2560*960)**************///not ready
#define VD_Sync_Prv_5             2
#define HD_Sync_Prv_5             2

#define VD_Total_Prv_5            996
#define HD_Total_Prv_5            2006


#define ACT_STRX_Prv_5            4
#define ACT_STRY_Prv_5            3

#define IMAGESIZE_H_PRV_5         1248
#define IMAGESIZE_V_PRV_5         936
#define SIE_PRV_XCENTER_5         (ACT_STRX_Prv_3+(IMAGESIZE_H_PRV_5/2))
#define SIE_PRV_YCENTER_5         (ACT_STRY_Prv_3+(IMAGESIZE_V_PRV_5/2))


#define Padding_Prv_5             0
/************** STILL_MODE **************/
#define VD_Sync_Cap               2
#define HD_Sync_Cap               2
#define VD_Total_Cap              1948// 1980 = 1948 + blank32
#define HD_Total_Cap              3232

#define IMAGESIZE_H_CAP           2528//2464
#define IMAGESIZE_V_CAP           1896//1860

#define ACT_STRX_Cap              69// 68
#define ACT_STRY_Cap              5// 4
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

#define OB_OFS_PRV          20// 10 bit
#define OB_OFS_CAP          12//14//20// 10 bit

/************** SIE CFA pattern selection **************/
#define CFAPAT_PRV    SIECFA_Gb
#define CFAPAT_CAP    SIECFA_Gb

/*********************************************/
//unit:mhz
#define PCLK_1      24
#define PCLK_2      59//not ready
#define PCLK_3      59
#define PCLK_4      69//not ready
#define PCLK_5      70//not ready
#define PCLK_STILL  64

SYNC_INFO prv_mode1 = {
    ACT_STRX_Prv_1, // Act_StartX;
    ACT_STRY_Prv_1, // Act_StartY;
    SIE_PRV_XCENTER_1, // Act_CenterX;
    SIE_PRV_YCENTER_1, // Act_CenterY;
    IMAGESIZE_H_PRV_1  , // Act_SizeX;
    IMAGESIZE_V_PRV_1, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_1, //    HD_sync
    VD_Sync_Prv_1, //    VD_sync
    HD_Total_Prv_1, //    HD_total
    VD_Total_Prv_1, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_1 // padding;
};

SYNC_INFO prv_mode2 = {
    ACT_STRX_Prv_2, // act_start_x;
    ACT_STRY_Prv_2, // act_start_y;
    SIE_PRV_XCENTER_2, // act_center_x;
    SIE_PRV_YCENTER_2, // act_center_y;
    IMAGESIZE_H_PRV_2  , // Act_SizeX;
    IMAGESIZE_V_PRV_2, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_2, //    HD_sync
    VD_Sync_Prv_2, //    VD_sync
    HD_Total_Prv_2, //    HD_total
    VD_Total_Prv_2, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_2 // padding;
};

SYNC_INFO prv_mode3 = {
    ACT_STRX_Prv_3, // act_start_x;
    ACT_STRY_Prv_3, // act_start_y;
    SIE_PRV_XCENTER_3, // act_center_x;
    SIE_PRV_YCENTER_3, // act_center_y;
    IMAGESIZE_H_PRV_3  , // Act_SizeX;
    IMAGESIZE_V_PRV_3, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_3, //    HD_sync
    VD_Sync_Prv_3, //    VD_sync
    HD_Total_Prv_3, //    HD_total
    VD_Total_Prv_3, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_3 // padding;
};

SYNC_INFO prv_mode4 = {
    ACT_STRX_Prv_4, // act_start_x;
    ACT_STRY_Prv_4, // act_start_y;
    SIE_PRV_XCENTER_4, // act_center_x;
    SIE_PRV_YCENTER_4, // act_center_y;
    IMAGESIZE_H_PRV_4  , // Act_SizeX;
    IMAGESIZE_V_PRV_4, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_4, //    HD_sync
    VD_Sync_Prv_4, //    VD_sync
    HD_Total_Prv_4, //    HD_total
    VD_Total_Prv_4, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_4 // padding;
};

SYNC_INFO prv_mode5 = {
    ACT_STRX_Prv_5, // act_start_x;
    ACT_STRY_Prv_5, // act_start_y;
    SIE_PRV_XCENTER_5, // act_center_x;
    SIE_PRV_YCENTER_5, // act_center_y;
    IMAGESIZE_H_PRV_5  , // Act_SizeX;
    IMAGESIZE_V_PRV_5, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_5, //    HD_sync
    VD_Sync_Prv_5, //    VD_sync
    HD_Total_Prv_5, //    HD_total
    VD_Total_Prv_5, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_5 // padding;
};

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

PREVIEW_DATA_INFO OV5633_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_1, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)OV5633_REG_VGA,
        //sizeof(OV5633_REG_VGA)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_720P
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_2, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)OV5633_REG_1280X720,
        //sizeof(OV5633_REG_1280X720)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_3, // pclk
        //&prv_mode3, // prv_sync_info
        //(SENSOR_CMD*)OV5633_REG_1280X960,
        //sizeof(OV5633_REG_1280X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_480
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_4, // pclk
        //&prv_mode4, // prv_sync_info
        //(SENSOR_CMD*)OV5633_REG_1280X480,
        //sizeof(OV5633_REG_1280X480)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_2560_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_5, // pclk
        //&prv_mode5, // prv_sync_info
        //(SENSOR_CMD*)OV5633_REG_2560X960,
        //sizeof(OV5633_REG_2560X960)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO OV5633_still_mode[] = {
    {
        2560, // cap_width
        1920, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_STILL, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)OV5633_REG_FULL,
        //sizeof(OV5633_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void OV5633_GetRegInfo(OV5633_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV5633_INIT:
            *addr = (UINT32)OV5633_INI_REG;
            *size = (sizeof(OV5633_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case OV5633_VGA:
            *addr = (UINT32)OV5633_REG_VGA;
            *size = (sizeof(OV5633_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case OV5633_1280X720:
            *addr = (UINT32)OV5633_REG_1280X720;
            *size = (sizeof(OV5633_REG_1280X720)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5633_1280X960:
            *addr = (UINT32)OV5633_REG_1280X960;
            *size = (sizeof(OV5633_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case OV5633_1280X480:
            *addr = (UINT32)OV5633_REG_1280X480;
            *size = (sizeof(OV5633_REG_1280X480)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case OV5633_2560X960:
            *addr = (UINT32)OV5633_REG_2560X960;
            *size = (sizeof(OV5633_REG_2560X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case OV5633_STILL:
            *addr = (UINT32)OV5633_REG_FULL;
            *size = (sizeof(OV5633_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV5633_GetSigInfo(OV5633_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV5633_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case OV5633_1280X720:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5633_1280X960:
            *addr = (UINT32)&prv_mode3;
            //*size = (sizeof(prv_mode3)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case OV5633_1280X480:
            *addr = (UINT32)&prv_mode4;
            //*size = (sizeof(prv_mode4)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case OV5633_2560X960:
            *addr = (UINT32)&prv_mode5;
            //*size = (sizeof(prv_mode5)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case OV5633_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case OV5633_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV5633_GetDataInfo(OV5633_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV5633_VGA:
            *addr = (UINT32)&OV5633_prv_mode[0];
            //*size = (sizeof(OV5633_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case OV5633_1280X720:
            *addr = (UINT32)&OV5633_prv_mode[1];
            //*size = (sizeof(OV5633_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5633_1280X960:
            *addr = (UINT32)&OV5633_prv_mode[2];
            //*size = (sizeof(OV5633_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case OV5633_1280X480:
            *addr = (UINT32)&OV5633_prv_mode[3];
            //*size = (sizeof(OV5633_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case OV5633_2560X960:
            *addr = (UINT32)&OV5633_prv_mode[4];
            //*size = (sizeof(OV5633_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case OV5633_STILL:
            *addr = (UINT32)&OV5633_still_mode;
            //*size = (sizeof(OV5633_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case OV5633_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV5633_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}

