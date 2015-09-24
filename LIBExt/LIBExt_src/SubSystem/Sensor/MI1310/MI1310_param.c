#include "MI1310.h"
#include "debug.h"
const SENSOR_CMD MI1310_INI_REG[]=
{
    {0x01,  0x002C},    //ROW_WINDOW_START_REG
    {0x02,  0x001E},    //COL_WINDOW_START_REG
    {0x03,  0x03C0},    //ROW_WINDOW_SIZE_REG
    {0x04,  0x0500},    //COL_WINDOW_SIZE_REG
    {0x05,  0x00CA},    //HORZ_BLANK_REG
    {0x06,  0x000E},    //VERT_BLANK_REG
    {0x20,  0x420C}    //ROW_COL_SKIP
};
const SENSOR_CMD MI1310_REG_VGA[]=
{
    {0x01,  0x002C},    //ROW_WINDOW_START_REG
    {0x02,  0x001E},    //COL_WINDOW_START_REG
    {0x03,  0x03C0},    //ROW_WINDOW_SIZE_REG
    {0x04,  0x0500},    //COL_WINDOW_SIZE_REG
    {0x05,  0x00CA},    //HORZ_BLANK_REG
    {0x06,  0x000E},    //VERT_BLANK_REG
    {0x20,  0x420C}    //ROW_COL_SKIP
};

const SENSOR_CMD MI1310_REG_1280X960[]=
{
    {0x01,  0x000C},    //ROW_WINDOW_START_REG
    {0x02,  0x001E},    //COL_WINDOW_START_REG
    {0x03,  0x0400},    //ROW_WINDOW_SIZE_REG
    {0x04,  0x0500},    //COL_WINDOW_SIZE_REG
    {0x05,  0x0110},    //HORZ_BLANK_REG
    {0x06,  0x0032},    //VERT_BLANK_REG
    {0x20,  0x4200}    //ROW_COL_SKIP
};


const SENSOR_CMD MI1310_REG_1440X1080[]=
{
    {0x01,  0x000C},    //ROW_WINDOW_START_REG
    {0x02,  0x001E},    //COL_WINDOW_START_REG
    {0x03,  0x0400},    //ROW_WINDOW_SIZE_REG
    {0x04,  0x0500},    //COL_WINDOW_SIZE_REG
    {0x05,  0x0110},    //HORZ_BLANK_REG
    {0x06,  0x0032},    //VERT_BLANK_REG
    {0x20,  0x4200}    //ROW_COL_SKIP
};

const SENSOR_CMD MI1310_REG_FULL[]=
{
    {0x01,  0x000C},    //ROW_WINDOW_START_REG
    {0x02,  0x001E},    //COL_WINDOW_START_REG
    {0x03,  0x0400},    //ROW_WINDOW_SIZE_REG
    {0x04,  0x0500},    //COL_WINDOW_SIZE_REG
    {0x05,  0x0110},    //HORZ_BLANK_REG
    {0x06,  0x0032},    //VERT_BLANK_REG
    {0x20,  0x4200}    //ROW_COL_SKIP
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       1000
#define ISOBASE         50

/************** MONITOR_MODE_1 (640 x 480)**************************/
#define VD_Sync_Prv_1                 14
#define HD_Sync_Prv_1                 202
#define VD_Total_Prv_1                (480 + 14)
#define HD_Total_Prv_1                (640 + 202)

#define ACT_STRX_Prv_1                9// 8
#define ACT_STRY_Prv_1                3// 2
#define IMAGESIZE_H_PRV_1                       640
#define IMAGESIZE_V_PRV_1                       480

#define SIE_PRV_XCENTER_1             (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_1/2))
#define SIE_PRV_YCENTER_1             (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_1/2))

#define PRV_OB_GAIN_1         0x20
#define PRV_OB_THRES_1        0xB0  //0xF0
#define PRV_OB_SUBSAMPLE_1    0x08 //0x02
#define PRV_OB_X0_1           0
#define PRV_OB_X1_1           0
#define PRV_OB_Y0_1           0
#define PRV_OB_Y1_1           0

#define Padding_Prv_1                 0
/********************************************************/

/************** MONITOR_MODE_2 (1280 x 960)**************************/
#define VD_Sync_Prv_2                   50
#define HD_Sync_Prv_2                   272
#define VD_Total_Prv_2                  1074
#define HD_Total_Prv_2                  1552
#define IMAGESIZE_H_PRV_2               1280
#define IMAGESIZE_V_PRV_2               1024

#define VD_DataOut_Prv_2                0
#define HD_DataOut_Prv_2                0
#define VD_DataEnd_Prv_2                0
#define HD_DataEnd_Prv_2                0
#define HD_RightDummy_Prv_2             0
#define VD_BottomDummy_Prv_2            0

#define ACT_STRX_Prv_2                  7// 6
#define ACT_STRY_Prv_2                  2

#define SIE_PRV_XCENTER_2               (ACT_STRX_Prv_1+0+(IMAGESIZE_H_PRV_2/2))
#define SIE_PRV_YCENTER_2               (ACT_STRY_Prv_1+1+(IMAGESIZE_V_PRV_2/2))

#define PRV_OB_GAIN_2         0x20
#define PRV_OB_THRES_2        0xB0  //0xF0
#define PRV_OB_SUBSAMPLE_2    0x08 //0x02
#define PRV_OB_X0_2           0
#define PRV_OB_X1_2           0
#define PRV_OB_Y0_2           0
#define PRV_OB_Y1_2           0

#define Padding_Prv_2                 0
/********************************************************/


/************** MONITOR_MODE_3 (1440 x 1080)**************************/
#define VD_Sync_Prv_3                   50
#define HD_Sync_Prv_3                   272
#define VD_Total_Prv_3                  1074
#define HD_Total_Prv_3                  1552
#define IMAGESIZE_H_PRV_3               1280
#define IMAGESIZE_V_PRV_3               1024

#define VD_DataOut_Prv_3                0
#define HD_DataOut_Prv_3                0
#define VD_DataEnd_Prv_3                0
#define HD_DataEnd_Prv_3                0

#define ACT_STRX_Prv_3                  7// 6
#define ACT_STRY_Prv_3                  2

#define SIE_PRV_XCENTER_3               (ACT_STRX_Prv_2+0+(IMAGESIZE_H_PRV_3/2))
#define SIE_PRV_YCENTER_3               (ACT_STRY_Prv_2+1+(IMAGESIZE_V_PRV_3/2))

#define PRV_OB_GAIN_3         0x20
#define PRV_OB_THRES_3        0xF0
#define PRV_OB_SUBSAMPLE_3    0x02
#define PRV_OB_X0_3           0
#define PRV_OB_X1_3           0
#define PRV_OB_Y0_3           0
#define PRV_OB_Y1_3           0

#define Padding_Prv_3                 0
/********************************************************/

/************** Capture mode **************/
#define VD_Sync_Cap             50
#define HD_Sync_Cap             272
#define VD_Total_Cap            1074
#define HD_Total_Cap            1552

#define VD_ImgOutStart_Cap      0
#define VD_ImgOutEnd_Cap        0
#define HD_ImgOutStart_Cap      0
#define HD_ImgOutEnd_Cap        0

#define IMAGESIZE_H_CAP         1280
#define IMAGESIZE_V_CAP         960

#define ACT_STRX_Cap            7// 6
#define ACT_STRX_Cap2           6
#define ACT_STRX_Cap3           6
#define ACT_STRX_Cap4           6
#define ACT_STRX_Cap5           6

#define ACT_STRY_Cap            2
#define ACT_STRY_Cap2           0xff
#define ACT_STRY_Cap3           0xff
#define ACT_STRY_Cap4           0xff
#define ACT_STRY_Cap5           0xff
#define ACT_STRY_Cap6           0xff
#define ACT_STRY_Cap7           0xff
#define ACT_STRY_Cap8           0xff

#define SensorCapTotalFld       1
#define Field_Num1              1
#define Field_Num2              0xff
#define Field_Num3              0xff
#define Field_Num4              0xff
#define Field_Num5              0xff
#define Field_Num6              0xff
#define Field_Num7              0xff
#define Field_Num8              0xff

#define Padding_Cap             0

#define SIE_CAP_XCENTER         (ACT_STRX_Cap+0+(IMAGESIZE_H_CAP/2))
#define SIE_CAP_YCENTER         (ACT_STRY_Cap+1+(IMAGESIZE_V_CAP/2))

#define CAP_OB_GAIN         0x20
#define CAP_OB_THRES        0xF0
#define CAP_OB_SUBSAMPLE    0x05
#define CAP_OB_X0           0
#define CAP_OB_X1           0
#define CAP_OB_Y0           0
#define CAP_OB_Y1           0

#define OB_OFS_PRV      0x28//10 bit
#define OB_OFS_CAP      0x28//10 bit
/************** PRE output for IPE CFA pattern selection **************/
#define CFAPAT_PRV    SIECFA_R
#define CFAPAT_CAP    SIECFA_R

/*********************************************/
//unit:mhz
#define PCLK_PRV    24
#define PCLK_CAP    24

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
    PRV_OB_X0_1, //    OB_X0
    PRV_OB_Y0_1, //    OB_Y0
    PRV_OB_X1_1, //    OB_X1
    PRV_OB_Y1_1, //    OB_Y1
    PRV_OB_THRES_1, //    OB_Thres
    PRV_OB_SUBSAMPLE_1, //    OB_SubSample
    PRV_OB_GAIN_1, // OB_Gain
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
    PRV_OB_X0_2, //    OB_X0
    PRV_OB_Y0_2, //    OB_Y0
    PRV_OB_X1_2, //    OB_X1
    PRV_OB_Y1_2, //    OB_Y1
    PRV_OB_THRES_2, //    OB_Thres
    PRV_OB_SUBSAMPLE_2, //    OB_SubSample
    PRV_OB_GAIN_2, // OB_Gain
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
    PRV_OB_X0_3, //    OB_X0
    PRV_OB_Y0_3, //    OB_Y0
    PRV_OB_X1_3, //    OB_X1
    PRV_OB_Y1_3, //    OB_Y1
    PRV_OB_THRES_3, //    OB_Thres
    PRV_OB_SUBSAMPLE_3, //    OB_SubSample
    PRV_OB_GAIN_3, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_3 // padding;
};

SYNC_INFO still_mode = {
    ACT_STRX_Cap, // act_start_x;
    ACT_STRY_Cap, // act_start_y;
    SIE_CAP_XCENTER, // act_center_x;
    SIE_CAP_YCENTER, // act_center_y;
    IMAGESIZE_H_CAP,
    IMAGESIZE_V_CAP,

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

PREVIEW_DATA_INFO MI1310_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_PRV, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)MI1310_REG_VGA,
        //sizeof(MI1310_REG_VGA)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_PRV, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI1310_REG_1280X960,
        //sizeof(MI1310_REG_1280X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1440_1080
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_PRV, // pclk
        //&prv_mode3, // prv_sync_info
        //(SENSOR_CMD*)MI1310_REG_1440X1080,
        //sizeof(MI1310_REG_1440X1080)/sizeof(SENSOR_CMD)
    },
};

CAPTURE_DATA_INFO MI1310_still_mode[] = {
    {
        4336, // cap_width
        3240, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_CAP, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)MI1310_REG_FULL,
        //sizeof(MI1310_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void MI1310_GetRegInfo(MI1310_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI1310_INIT:
            *addr = (UINT32)MI1310_INI_REG;
            *size = (sizeof(MI1310_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case MI1310_VGA:
            *addr = (UINT32)MI1310_REG_VGA;
            *size = (sizeof(MI1310_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI1310_1280X960:
            *addr = (UINT32)MI1310_REG_1280X960;
            *size = (sizeof(MI1310_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI1310_1440X1080:
            *addr = (UINT32)MI1310_REG_1440X1080;
            *size = (sizeof(MI1310_REG_1440X1080)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X1080)));
            break;
        case MI1310_STILL:
            *addr = (UINT32)MI1310_REG_FULL;
            *size = (sizeof(MI1310_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI1310_GetSigInfo(MI1310_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI1310_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI1310_1280X960:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI1310_1440X1080:
            *addr = (UINT32)&prv_mode3;
            //*size = (sizeof(prv_mode3)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X1080)));
            break;
        case MI1310_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI1310_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI1310_GetDataInfo(MI1310_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI1310_VGA:
            *addr = (UINT32)&MI1310_prv_mode[0];
            //*size = (sizeof(MI1310_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI1310_1280X960:
            *addr = (UINT32)&MI1310_prv_mode[1];
            //*size = (sizeof(MI1310_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI1310_1440X1080:
            *addr = (UINT32)&MI1310_prv_mode[2];
            //*size = (sizeof(MI1310_prv_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X1080)));
            break;
        case MI1310_STILL:
            *addr = (UINT32)&MI1310_still_mode;
            //*size = (sizeof(MI1310_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI1310_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI1310_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}
