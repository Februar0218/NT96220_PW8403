#include "MI9130.h"
#include "debug.h"
#define SENSOR_MIRROR_AND_FLIP  0
const SENSOR_CMD MI9130_INI_REG[]=
{
    {0x301A, 0x10D0}, // RESET_REGISTER
    {0x0008, 0x0150},   // DATA_PEDESTAL
    {0x301A, 0x10D8},   // RESET_REGISTER
    {0x316C, 0xB468},   // Reserved
    {0x3044, 0x0594},   // Reserved
    {0x3080, 0x0FE1},  // BITFIELD//Only Bit7
    //{0x3080, 0x0FE6},   // BITFIELD//Only Bit7
    {0x308E, 0xD66A},   // Reserved
    {0x3098, 0xECA4},   // Reserved
    {0x309A, 0xA5A4},   // Reserved
    {0x309C, 0x9900},   // Reserved
    {0x30B2, 0xC000},   // Reserved
    {0x30E2, 0x00FF},   // Reserved
    {0x30E6, 0x00FF},   // Reserved
    {0x30EA, 0xA404},   // Reserved
    {0x30EE, 0xA40C},   // Reserved
    {0x30F2, 0xA404},   // Reserved
    {0x30FE, 0x705A},   // Reserved
    {0x3100, 0x6C5C},   // Reserved
    {0x3102, 0xA25A},   // Reserved
    {0x3106, 0x581C},   // Reserved
    {0x311A, 0x6E1C},   // Reserved
    {0x3128, 0xA402},   // Reserved
    {0x312A, 0xA602},   // Reserved
    {0x3170, 0x0814},   // Reserved
    {0x317A, 0x8002},   // Reserved
    {0x317C, 0x8002},   // Reserved
    {0x31C0, 0x00FF},   // Reserved
    {0x31C8, 0xA608},   // Reserved
    {0x31CA, 0x00FF},   // Reserved
    {0x31CC, 0x00FF},   // Reserved
    {0x31CE, 0xA502},   // Reserved
    {0x31D0, 0x00FF},   // Reserved
    {0x3088, 0x60F8},   // Reserved
    {0x301A, 0x10DC}, // RESET_REGISTER - enable

    {0x3174, 0x1818},// DAC_LP_VCM_ADC
    {0x3176, 0x1818},// DAC_LP_ASC2_ASC1
    {0x3178, 0x1818},// DAC_LP_VCL_VREF
    //{0x3174, 0x0E0E},// DAC_LP_VCM_ADC
    //{0x3176, 0x0E0E},// DAC_LP_ASC2_ASC1
    //{0x3178, 0x0E0E},// DAC_LP_VCL_VREF
    //Set Dynamic Power mode
    {0x308e, 0xD66A},// DAC_LD_14_15
    {0x3170, 0x0815},// ANALOG_CONTROL
    //{0x3170, 0x080F},// ANALOG_CONTROL
    /*Frame Rate Improvement*/
    {0x30E0, 0x9A02},
    {0x3126, 0x00FF},
    {0x31C6, 0x00FF},
    {0x31C8, 0x9A08},
    {0x31CA, 0x00FF},
    {0x31C2, 0x00FF},
    {0x31C4, 0x00FF},
    {0x30F2, 0x9804},
    {0x3128, 0x9802},
    {0x31C0, 0x00FF},
    {0x3114, 0x00FF},
    {0x30F4, 0x00FF},
    {0x30EA, 0x9804},
    {0x31CE, 0x9902},
    {0x30EE, 0x990C},
    {0x3106, 0x4C16},
    {0x31CC, 0x00FF},
    {0x30FE, 0x644E},
    {0x311A, 0x6050},
    {0x3100, 0x6153},
    {0x30E2, 0x00FF},
    {0x31D0, 0x00FF},
    {0x30E6, 0x00FF},
    {0x3102, 0x964E},
    {0x310A, 0x9802},
    {0x310C, 0x9802},
    {0x310E, 0x9814},
    {0x3110, 0xA802},
    {0x3112, 0x00FF},
    {0x312A, 0x9A02},
    {0x3170, 0x0C14},// ANALOG_CONTROL
    {0x312C, 0x009C},// SAMP_READOUT
    {0x312E, 0x34AA},// SAMP_RESET_DONE
};


const SENSOR_CMD MI9130_REG_VGA[]=
{
#if SENSOR_MIRROR_AND_FLIP
    {0x3040, 0xe7c7},
#else
    {0x3040, 0x27c7},
#endif
    //Set Low Power mode start
    {0x0344, 0x0008},// x_addr_start=(8,8)
    {0x0348, 0x0DA1},// x_addr_end=(3489,3489)
    {0x0346, 0x0008},// y_addr_start=(8,8)
    {0x034A, 0x0A39},// y_addr_end=(2617,2617)
    {0x0400, 0x0002},// scaling_mode
    {0x0404, 0x0016},// scale_m
    {0x034C, 0x0288},// x_output_size=(648,3488)
    {0x034E, 0x01E4},// y_output_size=(484,2616)
    {0x0342, 0x0AC8},// line_length_pck=(2760,7006)
    {0x0340, 0x0323},// frame_length_lines=(803,2759)
    {0x3010, 0x0100},// fine_correction
    {0x3012, 0x0010},
    {0x3014, 0x056A},// fine_integration_time=(1386,6072)
    {0x3018, 0x0000},// extra_delay
    {0x30d4, 0x9080},// column double sampling
    {0x3044, 0x0594},
    {0x301A, 0x90DE},
    {0x0104, 0x0000}
};

const SENSOR_CMD MI9130_REG_1280X960[]= //need to modify
{
#if SENSOR_MIRROR_AND_FLIP
    {0x3040, 0xd2c3},
#else
    {0x3040, 0x12c3},
#endif
    //Set Low Power mode start
    {0x0344, 0x0008},// x_addr_start=(8,8)
    {0x0348, 0x0DA5},// x_addr_end=(3489,3489)
    {0x0346, 0x0008},// y_addr_start=(8,8)
    {0x034A, 0x0A3D},// y_addr_end=(2617,2617)
    {0x0400, 0x0002},// scaling_mode
    {0x0404, 0x0015},// scale_m
    {0x034C, 0x0530},// x_output_size=(1328,3488)
    {0x034E, 0x03E4},// y_output_size=(996,2616)
    {0x0342, 0x0DBE},// line_length_pck=(3518,7006)
    {0x0340, 0x071B},// frame_length_lines=(1819,2759)
    {0x3010, 0x0100},// fine_correction
    {0x3012, 0x0221},
    {0x3014, 0x0A18},// fine_integration_time=(1386,6072)
    {0x3018, 0x0000},// extra_delay
    {0x30d4, 0x9080},// column double sampling
    {0x3044, 0x0594},
    {0x301A, 0x90DE},
    {0x0104, 0x0000}
};

const SENSOR_CMD MI9130_REG_960X720[]= //need to modify
{
#if SENSOR_MIRROR_AND_FLIP
    {0x3040, 0xd2c3},
#else
    {0x3040, 0x12c3},
#endif
    //Set Low Power mode start
    {0x0344, 0x0070},// x_addr_start=(8,8)
    {0x0348, 0x0E05},// x_addr_end=(3489,3489)
    {0x0346, 0x0008},// y_addr_start=(8,8)
    {0x034A, 0x0A37},// y_addr_end=(2617,2617)
    {0x0400, 0x0002},// scaling_mode
    {0x0404, 0x001D},// scale_m
    {0x034C, 0x03C0},// x_output_size=(1328,3488)
    {0x034E, 0x02D0},// y_output_size=(996,2616)
    {0x0342, 0x0CF2},// line_length_pck=(3518,7006)
    {0x0340, 0x05A8},// frame_length_lines=(1819,2759)
    {0x3010, 0x0134},// fine_correction
    {0x3012, 0x05A8},
    {0x3014, 0x07B2},// fine_integration_time=(1386,6072)
    {0x3018, 0x0000},// extra_delay
    {0x30d4, 0x9080},// column double sampling
    {0x3044, 0x0594},
    {0x301A, 0x90DE},
    {0x0104, 0x0000}
};

const SENSOR_CMD MI9130_REG_FULL[]=
{
    {0x301A, 0x10D0},
#if SENSOR_MIRROR_AND_FLIP
    {0x3040, 0xc241},
#else
    {0x3040, 0x0241},
#endif
    //Set Low Power mode start
    {0x0344, 0x0008},// x_addr_start=(8,8)
    {0x0348, 0x0DA1},// x_addr_end=(3489,3489)
    {0x0346, 0x0008},// y_addr_start=(8,8)
    {0x034A, 0x0A39},// y_addr_end=(2617,2617)
    {0x0400, 0x0000},// scaling_mode
    {0x0404, 0x0010},// scale_m
    {0x034C, 0x0DA0},// x_output_size=(648,3488)
    {0x034E, 0x0A38},// y_output_size=(484,2616)
    {0x0342, 0x1B5E},// line_length_pck=(2760,7006)
    {0x0340, 0x0AC7},// frame_length_lines=(803,2759)
    {0x3010, 0x0100},// fine_correction
    {0x3012, 0x0010},
    {0x3014, 0x17B8},// fine_integration_time=(1386,6072)
    {0x3018, 0x0000},// extra_delay
    {0x30d4, 0x9080},// column double sampling
    {0x3044, 0x0594},
    {0x301A, 0x10DC}
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       1200
#define ADJ_ISO         0
#define ISOBASE         50

/************** MONITOR_MODE_1 (640 x 480)**************************/
#define VD_Sync_Prv_1             1//9
#define HD_Sync_Prv_1             1//312
#define VD_Total_Prv_1            803//563
#define HD_Total_Prv_1            1380//2760

#if SENSOR_MIRROR_AND_FLIP
#define ACT_STRX_Prv_1            4//3// 4
#define ACT_STRY_Prv_1            4//3// 4
#else
#define ACT_STRX_Prv_1            3// 4
#define ACT_STRY_Prv_1            3// 4
#endif
#define IMAGESIZE_H_PRV_1         624//1280
#define IMAGESIZE_V_PRV_1         468//960

#define SIE_PRV_XCENTER_1         (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_1/2))
#define SIE_PRV_YCENTER_1         (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_1/2))
#define Padding_Prv_1               0

#define PRV_OB_GAIN_1             0x20
#define PRV_OB_THRES_1            0x50
#define PRV_OB_SUBSAMPLE_1        0x02 //0x02
#define PRV_OB_X0_1               0
#define PRV_OB_X1_1               0
#define PRV_OB_Y0_1               0
#define PRV_OB_Y1_1               0

/********************************************************/

/************** MONITOR_MODE_2 (960 x 720)**************************/
#define VD_Sync_Prv_2             1//9
#define HD_Sync_Prv_2             1//312
#define VD_Total_Prv_2            1448
#define HD_Total_Prv_2            3314

#if SENSOR_MIRROR_AND_FLIP
#define ACT_STRX_Prv_2             4//5
#define ACT_STRY_Prv_2             4//3
#else
#define ACT_STRX_Prv_2             4
#define ACT_STRY_Prv_2             4
#endif
#define IMAGESIZE_H_PRV_2        944//(960-16)
#define IMAGESIZE_V_PRV_2         708//(720-12)

#define SIE_PRV_XCENTER_2         (ACT_STRX_Prv_2+(IMAGESIZE_H_PRV_2/2))
#define SIE_PRV_YCENTER_2         (ACT_STRY_Prv_2+(IMAGESIZE_V_PRV_2/2))
#define Padding_Prv_2               0

#define PRV_OB_GAIN_2             0x20
#define PRV_OB_THRES_2            0x50
#define PRV_OB_SUBSAMPLE_2        0x02 //0x02
#define PRV_OB_X0_2               0
#define PRV_OB_X1_2               0
#define PRV_OB_Y0_2               0
#define PRV_OB_Y1_2               0

/********************************************************/

/************** MONITOR_MODE_3 (1280 x 960)**************************/
#define VD_Sync_Prv_3             1//9
#define HD_Sync_Prv_3             1//312
#define VD_Total_Prv_3            1819
#define HD_Total_Prv_3            3518

#if SENSOR_MIRROR_AND_FLIP
#define ACT_STRX_Prv_3             4//5
#define ACT_STRY_Prv_3             4//3
#else
#define ACT_STRX_Prv_3             4
#define ACT_STRY_Prv_3             4
#endif
#define IMAGESIZE_H_PRV_3         1328
#define IMAGESIZE_V_PRV_3         996

#define SIE_PRV_XCENTER_3         (ACT_STRX_Prv_3+(IMAGESIZE_H_PRV_3/2))
#define SIE_PRV_YCENTER_3         (ACT_STRY_Prv_3+(IMAGESIZE_V_PRV_3/2))
#define Padding_Prv_3               0

#define PRV_OB_GAIN_3             0x20
#define PRV_OB_THRES_3            0x50
#define PRV_OB_SUBSAMPLE_3        0x02 //0x02
#define PRV_OB_X0_3               0
#define PRV_OB_X1_3               0
#define PRV_OB_Y0_3               0
#define PRV_OB_Y1_3               0

/********************************************************/

/************** Capture mode **************/
#define VD_Sync_Cap               1
#define HD_Sync_Cap               1
#define VD_Total_Cap              2759
#define HD_Total_Cap              3503//2628

#if SENSOR_MIRROR_AND_FLIP
#define ACT_STRX_Cap               8
#define ACT_STRY_Cap               8
#else
#define ACT_STRX_Cap              7// 8
#define ACT_STRY_Cap              7// 8
#endif

#define IMAGESIZE_H_CAP           3376//3456
#define IMAGESIZE_V_CAP           2532//2592

#define SensorCapTotalFld         1

#define Padding_Cap               0

#define SIE_CAP_XCENTER           (ACT_STRX_Cap+((IMAGESIZE_H_CAP)/2))
#define SIE_CAP_YCENTER           (ACT_STRY_Cap+((IMAGESIZE_V_CAP)/2))

#define CAP_OB_GAIN               0x20
#define CAP_OB_THRES              0xF0
#define CAP_OB_SUBSAMPLE          0x05
#define CAP_OB_X0                 0
#define CAP_OB_X1                 0
#define CAP_OB_Y0                 0
#define CAP_OB_Y1                 0

#define OB_OFS_PRV                0x50//10 bit
#define OB_OFS_CAP                0x50//10 bit
/************** PRE output for IPE CFA pattern selection **************/
#define CFAPAT_PRV                SIECFA_R
#define CFAPAT_CAP                SIECFA_R

/*********************************************/
//unit:mhz
#define PCLK      33
#define PCLK_PRV_640x480      33//for 640x480 resolution
#define PCLK_PRV_960x720      48//for 960x720 resolution
#define PCLK_PRV_1280x960      78//for 1280x760 resolution


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

PREVIEW_DATA_INFO MI9130_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_PRV_640x480, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)MI9130_REG_VGA,
        //sizeof(MI9130_REG_VGA)/sizeof(SENSOR_CMD)
    },
    { // SEN_MODE_960_720
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_PRV_960x720, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI9130_REG_1280X960,
        //sizeof(MI9130_REG_1280X960)/sizeof(SENSOR_CMD)
    },
    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_PRV_1280x960, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI9130_REG_1280X960,
        //sizeof(MI9130_REG_1280X960)/sizeof(SENSOR_CMD)
    },
};

CAPTURE_DATA_INFO MI9130_still_mode[] = {
    {
        3376, // cap_width
        2532, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)MI9130_REG_FULL,
        //sizeof(MI9130_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void MI9130_GetRegInfo(MI9130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI9130_INIT:
            *addr = (UINT32)MI9130_INI_REG;
            *size = (sizeof(MI9130_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case MI9130_VGA:
            *addr = (UINT32)MI9130_REG_VGA;
            *size = (sizeof(MI9130_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI9130_960X720:
            *addr = (UINT32)MI9130_REG_960X720;
            *size = (sizeof(MI9130_REG_960X720)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(960X720)));
            break;
        case MI9130_1280X960:
            *addr = (UINT32)MI9130_REG_1280X960;
            *size = (sizeof(MI9130_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI9130_STILL:
            *addr = (UINT32)MI9130_REG_FULL;
            *size = (sizeof(MI9130_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI9130_GetSigInfo(MI9130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI9130_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI9130_960X720:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode960x720)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(960X720)));
            break;
        case MI9130_1280X960:
            *addr = (UINT32)&prv_mode3;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI9130_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI9130_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI9130_GetDataInfo(MI9130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI9130_VGA:
            *addr = (UINT32)&MI9130_prv_mode[0];
            //*size = (sizeof(MI9130_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI9130_960X720:
            *addr = (UINT32)&MI9130_prv_mode[1];
            //*size = (sizeof(MI9130_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(960X720)));
            break;
        case MI9130_1280X960:
            *addr = (UINT32)&MI9130_prv_mode[2];
            //*size = (sizeof(MI9130_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI9130_STILL:
            *addr = (UINT32)&MI9130_still_mode;
            //*size = (sizeof(MI9130_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI9130_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI9130_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}

