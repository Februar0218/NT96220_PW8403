#include "MI5131E.h"
#include "debug.h"
#define CAP_15FPS       DISABLE
SENSOR_CMD MI5131E_INI_REG[]=
{
    //PLL
    {0x0104, 0x0001},
    {0x316C, 0x24F0},
    {0x309A, 0xa500},
    {0x3088, 0x6ff0},
    {0x3044, 0x8590},
    {0x31AE, 0x0201},
    {0x0104, 0x0000},
    {0x0100, 0x0   },
    {0x0300, 0x06  },
    {0x0302, 0x01  },
    {0x0304, 0x02  },
    {0x0306, 0xBA  },
    {0x0308, 0xA   },
    {0x030A, 0x01  },
    {0x0100, 0x1   },
    //INIT SETTING
    {0x0104, 0x0001},
    {0x316C, 0x24F0},
    {0x309A, 0xa500},
    {0x3044, 0x8590},
    {0x3088, 0x6FF0},
    {0x3055, 0x8590},
    {0x31AE, 0x0201},
    {0x0104, 0x0000},
    {0x30E0, 0xE302},
    {0x30E2, 0xB67C},
    {0x30E4, 0xE1B6},
    {0x30E6, 0xE103},
    {0x30EA, 0x3F06},
    {0x30EC, 0x7C3F},
    {0x30F2, 0xE103},
    {0x30FE, 0x8C6F},
    {0x3100, 0x8C6A},
    {0x3102, 0xB58D},
    {0x3104, 0xDCB7},
    {0x3106, 0x3E0A},
    {0x3108, 0x683F},
    {0x310A, 0xE102},
    {0x310C, 0xE102},
    {0x310E, 0xE10A},
    {0x3110, 0xE201},
    {0x3112, 0xE101},
    {0x3116, 0xE202},
    {0x311A, 0x8D6C},
    {0x3128, 0xE109},
    {0x312A, 0xDD02},
    {0x312C, 0x00E4},
    {0x312E, 0x42E4},
    {0x3140, 0x4101},
    {0x3142, 0x4002},
    {0x3144, 0x3F03},
    {0x3146, 0x3804},
    {0x3148, 0x3601},
    {0x314A, 0x3804},
    {0x314E, 0x4002},
    {0x3170, 0x299A},
    {0x3086, 0x2465},
    {0x3088, 0x6FF0},
    {0x3094, 0x4656},
    {0x3096, 0x5652},
    {0x309a, 0xaD00},
};


SENSOR_CMD MI5131E_REG_VGA[]=
{
    {0x0104, 0x0001},
    {0x0344, 0x0018},
    {0x0348, 0x0A11},
    {0x0346, 0x0014},
    {0x034A, 0x078D},
    {0x3040, 0x15C7},
    {0x034C, 0x0280},
    {0x034E, 0x01E0},
    {0x0342, 0x0DE0},
    {0x0340, 0x0233},
    {0x0202, 0x0010},
    {0x3014, 0x0702},
    {0x3010, 0x015C},
    {0x0104, 0x0000},
    {0x301A, 0x10DE},
};

SENSOR_CMD MI5131E_REG_1280X960[]=
{
    {0x0104, 0x0001},
    {0x0344, 0x0000},//x_addr_start_
    {0x0348, 0x0A1d},//x_addr_end_
    {0x0346, 0x0000},//y_addr_start_
    {0x034A, 0x0795},//y_addr_end_
    {0x3040, 0xD4C3},
    {0x034C, 0x0510},//x_output_size
    {0x034E, 0x03cc},//y_output_size
    {0x0342, 0x0EFB},//line_length_pck_
    {0x0340, 0x0436},//frame_length_lines_
    {0x0202, 0x0010},//coarse_integration_time_
    {0x3014, 0x0702},//fine_integration_time_
    {0x3010, 0x015C},//fine_correction
    {0x0104, 0x0000},//group parameter Hold
    {0x301A, 0x10DE},
};
//full 10fps
SENSOR_CMD MI5131E_REG_FULL[]=
{
    {0x0104, 0x0001},
    {0x0344, 0x000a},
    {0x0348, 0x0A27},
    {0x0346, 0x0000},
    {0x034A, 0x0795},
    {0x3040, 0xC041},
    {0x034C, 0x0A20},
    {0x034E, 0x0798},
    {0x0342, 6571  },
    {0x0340, 0X87C },
    {0x0202, 1826  },
    {0x3014, 1354  },
    {0x3010, 0x009C},
    {0x0104, 0x0000},
    {0x301A, 0x10DE},
};
//full 15fps,don't work so far
SENSOR_CMD MI5131E_REG_FULL_15FPS[]=
{
    //pll setting
    {0x0104, 0x0001},
    {0x316C, 0x24F0},
    {0x309A, 0x0001},
    {0x3088, 0x000F},
    {0x3044, 0x0004},
    {0x3080, 0x00C0},
    {0x0104, 0x0000},
    {0x0100, 0x0   },
    {0x0300, 0x06  },
    {0x0302, 0x01  },
    {0x0304, 0x02  },
    {0x0306, 0x3C  },
    {0x0308, 0x08  },
    {0x030A, 0x01  },
    {0x0000, 0x1   },       //delay 1ms

    {0x0100, 0x1   },
    {0x0104, 0x0001},
    {0x034c, 0x0A20},
    {0x034e, 0x0798},
    {0x0344, 0x0008},
    {0x0346, 0x0008},
    {0x0348, 0x0A27},
    {0x034A, 0x079F},
    {0x3040, 0x0041},
    {0x0400, 0x0000},
    {0x0404, 0x0010},
    {0x3012, 0x07EC},
    {0x3014, 0x06CB},
    {0x3010, 0x009C},
    {0x0340, 0x07ED},
    {0x0342, 0x0F66},
    {0x0104, 0x0   },
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       2110//      4220 -> (standard iso adj.)
#define ISOBASE         50

/************** MONITOR_MODE_1 (640 x 480)**************************/
#define VD_Sync_Prv_1                 9
#define HD_Sync_Prv_1                 312
#define VD_Total_Prv_1                563
#define HD_Total_Prv_1                1456

#define ACT_STRX_Prv_1                10
#define ACT_STRY_Prv_1                10
#define IMAGESIZE_H_PRV_1             638
#define IMAGESIZE_V_PRV_1             478

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
#define VD_Total_Prv_2                 1078
#define HD_Total_Prv_2                 2230

#define VD_DataOut_Prv_2                0
#define HD_DataOut_Prv_2                0
#define VD_DataEnd_Prv_2                0
#define HD_DataEnd_Prv_2                0
#define HD_RightDummy_Prv_2             0
#define VD_BottomDummy_Prv_2            0

#define ACT_STRX_Prv_2                  10
#define ACT_STRY_Prv_2                  10

#define IMAGESIZE_H_PRV_2               1280
#define IMAGESIZE_V_PRV_2               960

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

/************** Capture mode,10FPS **************/
#define VD_Sync_Cap             82
#define HD_Sync_Cap             4
#define VD_Total_Cap            2029
#define HD_Total_Cap            3814

#define VD_ImgOutStart_Cap      0
#define VD_ImgOutEnd_Cap        0
#define HD_ImgOutStart_Cap      0
#define HD_ImgOutEnd_Cap        0

#define IMAGESIZE_H_CAP         2560
#define IMAGESIZE_V_CAP         1920

#define ACT_STRX_Cap            10
#define ACT_STRY_Cap            10

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

#define OB_OFS_PRV      0x34//10 bit
#define OB_OFS_CAP      0x34//10 bit
/************** Capture mode,15FPS **************/
#define VD_Sync_Cap_2             82
#define HD_Sync_Cap_2             4
#define VD_Total_Cap_2            2029
#define HD_Total_Cap_2            3942

#define VD_ImgOutStart_Cap_2      0
#define VD_ImgOutEnd_Cap_2        0
#define HD_ImgOutStart_Cap_2      0
#define HD_ImgOutEnd_Cap_2        0

#define IMAGESIZE_H_CAP_2         2560
#define IMAGESIZE_V_CAP_2         1920

#define ACT_STRX_Cap_2            10
#define ACT_STRY_Cap_2            10

#define SensorCapTotalFld_2       1

#define Padding_Cap_2             0

#define SIE_CAP_XCENTER_2         (ACT_STRX_Cap+IMAGESIZE_H_CAP/2)//(ACT_STRX_Cap+1+(2560/2) + 16)
#define SIE_CAP_YCENTER_2         (ACT_STRY_Cap+IMAGESIZE_V_CAP/2)//(ACT_STRY_Cap+8+(1920/2) + 4)

#define CAP_OB_GAIN_2         0x20
#define CAP_OB_THRES_2        0xF0
#define CAP_OB_SUBSAMPLE_2    0x05
#define CAP_OB_X0_2           0
#define CAP_OB_X1_2           0
#define CAP_OB_Y0_2           0
#define CAP_OB_Y1_2           0

#define OB_OFS_PRV_2      0x2C//10 bit
#define OB_OFS_CAP_2      0x2C//10 bit
/************** PRE output for IPE CFA pattern selection **************/
#define CFAPAT_PRV    SIECFA_R
#define CFAPAT_CAP    SIECFA_R

/*********************************************/
//unit:mhz
#define PCLK      72
#define PCLK_15FPS  120//internal clk

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

SYNC_INFO still_15FPS_mode = {
    ACT_STRX_Cap_2, // act_start_x;
    ACT_STRY_Cap_2, // act_start_y;
    SIE_CAP_XCENTER_2, // act_center_x;
    SIE_CAP_YCENTER_2, // act_center_y;
    IMAGESIZE_H_CAP_2, // Act_SizeX;
    IMAGESIZE_V_CAP_2, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Cap_2, //    HD_sync
    VD_Sync_Cap_2, //    VD_sync
    HD_Total_Cap_2, //    HD_total
    VD_Total_Cap_2, //    VD_total

    // ob_area
    CAP_OB_X0_2, //    OB_X0
    CAP_OB_Y0_2, //    OB_Y0
    CAP_OB_X1_2, //    OB_X1
    CAP_OB_Y1_2, //    OB_Y1
    CAP_OB_THRES_2, //    OB_Thres
    CAP_OB_SUBSAMPLE_2, //    OB_SubSample
    CAP_OB_GAIN_2, // OB_Gain
    OB_OFS_CAP_2, // OB_Offset;

    Padding_Cap_2 // padding;
};

PREVIEW_DATA_INFO MI5131E_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)MI5131E_REG_VGA,
        //sizeof(MI5131E_REG_VGA)/sizeof(SENSOR_CMD)
    },
    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI5131E_REG_1280X960,
        //sizeof(MI5131E_REG_1280X960)/sizeof(SENSOR_CMD)
    },
};

CAPTURE_DATA_INFO MI5131E_still_mode = {
        2592, // cap_width
        1944, // cap_height
        CFAPAT_CAP, // bayer_pattern
#if CAP_15FPS
        PCLK_15FPS,
#else
        PCLK, // pclk
#endif
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)MI5131E_REG_FULL,
        //sizeof(MI5131E_REG_FULL)/sizeof(SENSOR_CMD)
};

extern void MI5131E_GetRegInfo(MI5131E_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5131E_INIT:
            *addr = (UINT32)MI5131E_INI_REG;
            *size = (sizeof(MI5131E_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case MI5131E_VGA:
            *addr = (UINT32)MI5131E_REG_VGA;
            *size = (sizeof(MI5131E_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5131E_1280X960:
            *addr = (UINT32)MI5131E_REG_1280X960;
            *size = (sizeof(MI5131E_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5131E_STILL:
#if CAP_15FPS
            *addr = (UINT32)MI5131E_REG_FULL_15FPS;
            *size = (sizeof(MI5131E_REG_FULL_15FPS)/sizeof(SENSOR_CMD));
#else
            *addr = (UINT32)MI5131E_REG_FULL;
            *size = (sizeof(MI5131E_REG_FULL)/sizeof(SENSOR_CMD));
#endif
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5131E_GetSigInfo(MI5131E_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5131E_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5131E_1280X960:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5131E_STILL:
#if CAP_15FPS
            *addr = (UINT32)&still_15FPS_mode;
            //*size = (sizeof(still_15FPS_mode)/sizeof(SYNC_INFO));
#else
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
#endif
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI5131E_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5131E_GetDataInfo(MI5131E_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5131E_VGA:
            *addr = (UINT32)&MI5131E_prv_mode[0];
            //*size = (sizeof(MI5131E_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5131E_1280X960:
            *addr = (UINT32)&MI5131E_prv_mode[1];
            //*size = (sizeof(MI5131E_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5131E_STILL:
            *addr = (UINT32)&MI5131E_still_mode;
            //*size = (sizeof(MI5131E_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI5131E_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5131E_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}

