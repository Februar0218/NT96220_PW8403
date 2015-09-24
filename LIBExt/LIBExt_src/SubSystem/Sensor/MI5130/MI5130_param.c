#include "MI5130.h"
#include "debug.h"
SENSOR_CMD MI5130_INI_REG[]=
{
    {0x0104, 0x0001},
    {0x301A, 0x10CC},
    {0x0112, 0x0A0A},
    {0x3064, 0x0805},
    {0x31AE, 0x0201},
    {0x0104, 0x0000},
    {0x0100, 0x0000}, //Mode Select = 0x0
    {0x0300, 0x0004}, //vt_pix_clk_div = 0x5
    {0x0302, 0x0001}, //vt_sys_clk_div = 0x1
    {0x0304, 0x0003}, //pre_pll_clk_div = 0x1
    {0x0306, 0x0050}, //pll_multiplier = 0x24
    {0x0308, 0x000a}, //op_pix_clk_div = 0x8
    {0x030A, 0x0001}, //op_sys_clk_div = 0x1
    {0x0100, 0x0001} //Mode Select = 0x1
};

SENSOR_CMD MI5130_REG_VGA[]=
{
    {0x0104, 0x0001},
    {0x0344, 0x0008},
    {0x0348,   2561},
    {0x0346, 0x0008},
    {0x034A,   1921},
    {0x3040, 0x15c7},
    {0x0400, 0x0000},
    {0x0404,     16},
    {0x034C,    640},
    {0x034E,    480},
    {0x0342,   2912},
    {0x0340,    563},
    {0x0202,    642},
    {0x3014,  0x702},
    {0x3010,  0x15c},
    {0x0104, 0x0000},
    {0x301A, 0x10DE}
};

SENSOR_CMD MI5130_REG_1280X960[]=
{
    {0x0104, 0x0001},
    {0x0344, 0x0008},
    {0x0348,   2565},
    {0x0346, 0x0008},
    {0x034A,   1925},
    {0x3040, 0x14C3},
    {0x0400, 0x0000},
    {0x0404,     16},
    {0x034C,   1280},
    {0x034E,    960},
    {0x0342,   3835},
    {0x0340,   1043},
    {0x0202,   1032},
    {0x3014,   2041},
    {0x3010,    348},
    {0x0104, 0x0000},
    {0x301A, 0x10DE}
};

SENSOR_CMD MI5130_REG_FULL[]=
{
    {0x301A, 0x10cc},//ethan_20110804_addfromSKL_Sensor_WriteReg(0x301A, 0x10CC);
    {0x0104, 0x0001},
    {0x0344, 0x0008},
    {0x0348,   2599},
    {0x0346, 0x0008},
    {0x034A,   1951},
    {0x3040, 0x0041},
    {0x0400, 0x0000},
    {0x0404,     16},
    {0x034C,   2592},
    {0x034E,   1944},
    {0x0342,   6571},
    {0x0340,   2029},
    {0x0202,   1826},
    {0x3014,   1354},
    {0x3010,    156},
    {0x0104, 0x0000},
    {0x301A, 0x10DE}
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       1150
#define ISOBASE         50

/************** MONITOR_MODE_1 (640 x 480)**************************/
#define VD_Sync_Prv_1                 9
#define HD_Sync_Prv_1                 312
#define VD_Total_Prv_1                563
#define HD_Total_Prv_1                1456

#define ACT_STRX_Prv_1                10
#define ACT_STRY_Prv_1                16// 10
#define IMAGESIZE_H_PRV_1             638//1280
#define IMAGESIZE_V_PRV_1             478//960

#define SIE_PRV_XCENTER_1             (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_1/2))
#define SIE_PRV_YCENTER_1             (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_1/2))
#define Padding_Prv                 0

#define PRV_OB_GAIN_1         0x9f
#define PRV_OB_THRES_1        0x9f
#define PRV_OB_SUBSAMPLE_1    0x08 //0x02
#define PRV_OB_X0_1           0
#define PRV_OB_X1_1           0
#define PRV_OB_Y0_1           0
#define PRV_OB_Y1_1           0

/********************************************************/

/************** MONITOR_MODE_2 (1280 x 960)**************************/
#define VD_Sync_Prv_2                   586
#define HD_Sync_Prv_2                   4
#define VD_Total_Prv_2                 1043
#define HD_Total_Prv_2                 1532

#define VD_DataOut_Prv_2                0
#define HD_DataOut_Prv_2                0
#define VD_DataEnd_Prv_2                0
#define HD_DataEnd_Prv_2                0
#define HD_RightDummy_Prv_2             0
#define VD_BottomDummy_Prv_2            0

#define ACT_STRX_Prv_2                  11// 10
#define ACT_STRY_Prv_2                  11// 10

#define IMAGESIZE_H_PRV_2               1264//1280
#define IMAGESIZE_V_PRV_2               948//960

#define SIE_PRV_XCENTER_2               (ACT_STRX_Prv_2+(IMAGESIZE_H_PRV_2/2))//(ACT_STRX_Prv_1+0+(1280/2))
#define SIE_PRV_YCENTER_2               (ACT_STRY_Prv_2+(IMAGESIZE_V_PRV_2/2))//(ACT_STRY_Prv_1+6+(960/2)+1)

#define PRV_OB_GAIN_2         0x9f
#define PRV_OB_THRES_2        0x9f
#define PRV_OB_SUBSAMPLE_2    0x08 //0x02
#define PRV_OB_X0_2           0
#define PRV_OB_X1_2           0
#define PRV_OB_Y0_2           0
#define PRV_OB_Y1_2           0

/********************************************************/

/************** Capture mode **************/
#define VD_Sync_Cap             82
#define HD_Sync_Cap             4
#define VD_Total_Cap            2029
#define HD_Total_Cap            2628

#define VD_ImgOutStart_Cap      0
#define VD_ImgOutEnd_Cap        0
#define HD_ImgOutStart_Cap      0
#define HD_ImgOutEnd_Cap        0

#define IMAGESIZE_H_CAP         2560
#define IMAGESIZE_V_CAP         1920

#define ACT_STRX_Cap            7
#define ACT_STRY_Cap            3// 2

#define SensorCapTotalFld       1

#define Padding_Cap             0

#define SIE_CAP_XCENTER         (ACT_STRX_Cap+IMAGESIZE_H_CAP/2)//(ACT_STRX_Cap+1+(2560/2) + 16)
#define SIE_CAP_YCENTER         (ACT_STRY_Cap+IMAGESIZE_V_CAP/2)//(ACT_STRY_Cap+8+(1920/2) + 4)

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
#define PCLK      48

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

    Padding_Prv // padding;
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

    Padding_Prv // padding;
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

PREVIEW_DATA_INFO MI5130_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)MI5130_REG_VGA,
        //sizeof(MI5130_REG_VGA)/sizeof(SENSOR_CMD)
    },
    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI5130_REG_1280X960,
        //sizeof(MI5130_REG_1280X960)/sizeof(SENSOR_CMD)
    },
};

CAPTURE_DATA_INFO MI5130_still_mode[] = {
    {
        2592, // cap_width
        1944, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)MI5130_REG_FULL,
        //sizeof(MI5130_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void MI5130_GetRegInfo(MI5130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5130_INIT:
            *addr = (UINT32)MI5130_INI_REG;
            *size = (sizeof(MI5130_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case MI5130_VGA:
            *addr = (UINT32)MI5130_REG_VGA;
            *size = (sizeof(MI5130_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5130_1280X960:
            *addr = (UINT32)MI5130_REG_1280X960;
            *size = (sizeof(MI5130_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5130_STILL:
            *addr = (UINT32)MI5130_REG_FULL;
            *size = (sizeof(MI5130_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5130_GetSigInfo(MI5130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5130_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5130_1280X960:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5130_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI5130_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5130_GetDataInfo(MI5130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5130_VGA:
            *addr = (UINT32)&MI5130_prv_mode[0];
            //*size = (sizeof(MI5130_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5130_1280X960:
            *addr = (UINT32)&MI5130_prv_mode[1];
            //*size = (sizeof(MI5130_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5130_STILL:
            *addr = (UINT32)&MI5130_still_mode;
            //*size = (sizeof(MI5130_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI5130_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5130_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}
