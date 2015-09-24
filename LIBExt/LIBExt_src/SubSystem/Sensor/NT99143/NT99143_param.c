#include "NT99143.h"
#include "debug.h"
const SENSOR_CMD NT99143_INI_REG[]=
{
{0x3069, 0x01},	  //Driving
{0x306A, 0x02},
{0x3100, 0x03}, //analog setting demo
{0x3101, 0x00},
{0x3102, 0x0B},
{0x3103, 0x00},
{0x3105, 0x03},
{0x3106, 0x04},
{0x3107, 0x10},
{0x3108, 0x00},
{0x3109, 0x00},
{0x310A, 0x05},
{0x310C, 0x00},
{0x310D, 0xC0},
{0x3110, 0x33},
{0x3110, 0x59},
{0x3112, 0x44},
{0x3113, 0x66},
{0x3114, 0x66},
{0x311D, 0x40},
{0x3129, 0x44},
{0x3136, 0x53},
{0x307D, 0x00},
{0x3012, 0x02},  //Init ExLine
{0x3013, 0x00},
{0x3262, 0x10},  //+----[4] CT_LSC_Auto_En
{0x32B3, 0x80},  //LA_Clamp
{0x32B4, 0x20},
{0x32CB, 0x30},  //AE_Target_Dark
{0x32CC, 0x70},  //AE_EV_Thr_B
{0x32CD, 0xA0},  //AE_EV_Thr_D
{0x3297, 0x03},  //AWB_Speed
{0x30A1, 0x23},	 //FPN_Ctrl
{0x30A2, 0x70},
{0x30A3, 0x01},
{0x30A0, 0x03},
{0x303E, 0x04},
{0x303F, 0x32},
{0x3055, 0x00},
{0x3056, 0x18},
//{0x305F, 0x33},
{0x305F, 0x00},
{0x3268, 0x01},
{0x3051, 0x3A},  //OB
{0x308E, 0x3A},
{0x3360, 0x14},  //IQ_Param_Sel_0 2.25x
{0x3361, 0x24},  //IQ_Param_Sel_1 4.25x
{0x3362, 0x28},  //IQ_Param_Sel_2 6x
{0x3363, 0x01},  //IQ_Auto_Ctrl
{0x324F, 0x00},  //0: Load default LSC parameter
#if 1
{0x3210, 0x0C},  //Gain0 of R ?
{0x3211, 0x0C},  //Gain1 of R ?
{0x3212, 0x0C},  //Gain2 of R ?
{0x3213, 0x0C},  //Gain3 of R ?
{0x3214, 0x08},  //Gain0 of Gr
{0x3215, 0x08},  //Gain1 of Gr
{0x3216, 0x08},  //Gain2 of Gr
{0x3217, 0x08},  //Gain3 of Gr
{0x3218, 0x08},  //Gain0 of Gb
{0x3219, 0x08},  //Gain1 of Gb
{0x321A, 0x08},  //Gain2 of Gb
{0x321B, 0x08},  //Gain3 of Gb
{0x321C, 0x0C},  //Gain0 of B
{0x321D, 0x0C},  //Gain1 of B
{0x321E, 0x0C},  //Gain2 of B
{0x321F, 0x0C},  //Gain3 of B
#else
{0x3210, 0x00},  //Gain0 of R ?
{0x3211, 0x00},  //Gain1 of R ?
{0x3212, 0x00},  //Gain2 of R ?
{0x3213, 0x00},  //Gain3 of R ?
{0x3214, 0x10},  //Gain0 of Gr
{0x3215, 0x10},  //Gain1 of Gr
{0x3216, 0x10},  //Gain2 of Gr
{0x3217, 0x10},  //Gain3 of Gr
{0x3218, 0x10},  //Gain0 of Gb
{0x3219, 0x10},  //Gain1 of Gb
{0x321A, 0x10},  //Gain2 of Gb
{0x321B, 0x10},  //Gain3 of Gb
{0x321C, 0x10},  //Gain0 of B
{0x321D, 0x10},  //Gain1 of B
{0x321E, 0x10},  //Gain2 of B
{0x321F, 0x10},  //Gain3 of B

#endif
{0x3230, 0x00},  //Inner R gain (signed  -16 to 15) ?
{0x3231, 0x17},  //Inner Gr gain (signed  -16 to 15)
{0x3232, 0x09},  //Inner Gb gain (signed  -16 to 15)
{0x3233, 0x09},  //Inner B gain (signed  -16 to 15) ?
{0x3234, 0x00},  //Outer R gain (signed  -16 to 15) ?
{0x3235, 0x00},  //Outer Gr gain (signed  -16 to 15)
{0x3236, 0x00},  //Outer Gb gain (signed  -16 to 15)
{0x3237, 0x00},  //Outer B gain (signed  -16 to 15) ?
{0x3238, 0x18},  //Inner and outer inflection distance of R
{0x3239, 0x18},  //Inner and outer inflection distance of Gr or Gb
{0x323A, 0x18},  //Inner and outer inflection distance of B
{0x3243, 0xC3},  //
{0x3244, 0x00},  //
{0x3245, 0x00},  //
{0x3270, 0x00},  // Gamma
{0x3271, 0x0B},
{0x3272, 0x16},
{0x3273, 0x2B},
{0x3274, 0x3F},
{0x3275, 0x51},
{0x3276, 0x72},
{0x3277, 0x8F},
{0x3278, 0xA7},
{0x3279, 0xBC},
{0x327A, 0xDC},
{0x327B, 0xF0},
{0x327C, 0xFA},
{0x327D, 0xFE},
{0x327E, 0xFF},
#if 1
{0x3371, 0x00},  //NR_Wgt_0
{0x3372, 0x00},  //NR_Wgt_1
{0x3374, 0x00},  //NR_Wgt_2
{0x3379, 0x00},  //NR_Comp_Max_0
{0x337A, 0x00},  //NR_Comp_Max_1
{0x337C, 0x00},  //NR_Comp_Max_2
{0x33A9, 0x00},  //NR_Post_Thr_0
{0x33AA, 0x00},  //NR_Post_Thr_1
{0x33AC, 0x00},  //NR_Post_Thr_2
{0x33AD, 0x00},  //NR_Post_EThr_0
{0x33AE, 0x00},  //NR_Post_EThr_1
{0x33B0, 0x00},  //NR_Post_EThr_2
#endif
#if 0
{0x3371, 0x10},  //NR_Wgt_0
{0x3372, 0x16},  //NR_Wgt_1
{0x3374, 0x1A},  //NR_Wgt_2
{0x3379, 0x08},  //NR_Comp_Max_0
{0x337A, 0x0C},  //NR_Comp_Max_1
{0x337C, 0x10},  //NR_Comp_Max_2
{0x33A9, 0x00},  //NR_Post_Thr_0
{0x33AA, 0x00},  //NR_Post_Thr_1
{0x33AC, 0x00},  //NR_Post_Thr_2
{0x33AD, 0x00},  //NR_Post_EThr_0
{0x33AE, 0x00},  //NR_Post_EThr_1
{0x33B0, 0x00},  //NR_Post_EThr_2
#endif
#if 0
{0x3371, 0x00},  //NR_Wgt_0
{0x3372, 0x00},  //NR_Wgt_1
{0x3374, 0x00},  //NR_Wgt_2
{0x3379, 0x00},  //NR_Comp_Max_0
{0x337A, 0x00},  //NR_Comp_Max_1
{0x337C, 0x00},  //NR_Comp_Max_2
{0x33A9, 0x00},  //NR_Post_Thr_0
{0x33AA, 0x00},  //NR_Post_Thr_1
{0x33AC, 0x00},  //NR_Post_Thr_2
{0x33AD, 0x00},  //NR_Post_EThr_0
{0x33AE, 0x00},  //NR_Post_EThr_1
{0x33B0, 0x00},  //NR_Post_EThr_2
#endif
{0x3710, 0x07},  //Dynamic Gamma Setting
{0x371E, 0x02},  //DG_EQ_Wgt_Max_Limit
{0x371F, 0x02},  //DG_EQ_Wgt_Min_Limit
{0x3364, 0x0B},  //DarkGamma_Auto_En
{0x33BD, 0x00},  //DarkGamma_Wgt_0
{0x33BE, 0x08},  //DarkGamma_Wgt_1
{0x33BF, 0x10},  //DarkGamma_Wgt_2
{0x3700, 0x00},  // Dark Gamma
{0x3701, 0x0C},
{0x3702, 0x18},
{0x3703, 0x32},
{0x3704, 0x44},
{0x3705, 0x54},
{0x3706, 0x70},
{0x3707, 0x88},
{0x3708, 0x9D},
{0x3709, 0xB0},
{0x370A, 0xCF},
{0x370B, 0xE2},
{0x370C, 0xEF},
{0x370D, 0xF7},
{0x370E, 0xFF},
{0x3250, 0x35},  //CA window
{0x3251, 0x21},
{0x3252, 0x30},
{0x3253, 0x10},
{0x3254, 0x3E},
{0x3255, 0x2A},
{0x3256, 0x2A},
{0x3257, 0x1B},
{0x3258, 0x4D},
{0x3259, 0x3C},
{0x325A, 0x28},
{0x325B, 0x16},
{0x325C, 0x80},  
{0x325D, 0x08},  
{0x3813, 0x07},  //False_Color_Shift  };
             
{0x32BB, 0xE7},  //AE Start
{0x32B8, 0x08}, 
{0x32B9, 0x04}, 
{0x32BC, 0x40}, 
{0x32BD, 0x04}, 
{0x32BE, 0x02}, 
{0x32BF, 0x60}, 
{0x32C0, 0x6A}, 
{0x32C1, 0x6A}, 
{0x32C2, 0x6A}, 
{0x32C3, 0x00}, 
{0x32C4, 0x30}, 
{0x32C5, 0x3F}, 
{0x32C6, 0x3F}, 
{0x32D3, 0x00}, 
{0x32D4, 0xE3}, 
{0x32D5, 0x7C}, 
{0x32D6, 0x00}, 
{0x32D7, 0xBD}, 
{0x32D8, 0x77},  //AE End
{0x32F0, 0x20},  //Output Format
{0x3200, 0x20},  //Mode Control
{0x3201, 0x08},  //Mode Control
{0x302A, 0x80},  //PLL Start
{0x302C, 0x17}, 
{0x302D, 0x11},  //PLL End
{0x3022, 0x03},  //Timing Start
//{0x3022, 0x00},  //Timing Start

{0x300A, 0x06}, 
{0x300B, 0x32}, 
{0x300C, 0x02}, 
{0x300D, 0xF4},  //Timing End
{0x301E, 0x08},
{0x3024, 0x01},	
{0x320A, 0x00}, 
{0x3021, 0x02}, 
//{0x3060, 0x01}, 
};

/*
const SENSOR_CMD NT99143_INIT_REG_25FPS[]=
{

};

SENSOR_CMD NT99143_REG_FULL[]=
{
};
*/

/**
* ISO to AGC usage
*/
#define AGC_ISOBASE       0// 1000
#define ISOBASE         75//100

/**************Default Sensor mode selection for preview**************/

/************** STILL_MODE **************/
#define VD_Sync_Cap               1
#define HD_Sync_Cap               1
#define VD_Total_Cap              0x2F4//738
#define HD_Total_Cap              0x632//1806

#define IMAGESIZE_H_CAP           1280
#define IMAGESIZE_V_CAP           720

#define ACT_STRX_Cap              306// 8
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
#define PCLK      36
//#define PCLK_15FPS  21
//#define PCLK_20FPS  28
//#define PCLK_25FPS  36
#define PCLK_STILL  36

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



CAPTURE_DATA_INFO NT99143_still_mode[] = {
    {
        1264, // cap_width
        708, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_STILL, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)NT99143_REG_FULL,
        //sizeof(NT99143_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void NT99143_GetRegInfo(NT99143_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99143_INIT:
            *addr = (UINT32)NT99143_INI_REG;
            *size = (sizeof(NT99143_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case NT99143_1280X720:
        case NT99143_1280X960:
        case NT99143_1440X1080:
        case NT99143_1920X1080:
        case NT99143_STILL:
            *addr = (UINT32)NT99143_INI_REG;
            *size = (sizeof(NT99143_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99143_GetSigInfo(NT99143_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99143_1280X720:
        case NT99143_1280X960:
        case NT99143_1440X1080:
        case NT99143_1920X1080:
        case NT99143_STILL:
            *addr = (UINT32)&still_mode;
            *size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case NT99143_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99143_GetDataInfo(NT99143_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case NT99143_1280X720:
        case NT99143_1280X960:
        case NT99143_1440X1080:
        case NT99143_1920X1080:
        case NT99143_STILL:
            *addr = (UINT32)&NT99143_still_mode;
            *size = (sizeof(NT99143_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case NT99143_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void NT99143_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISOBASE;
}

