#include "MI5100.h"
#include "debug.h"
const SENSOR_CMD MI5100_INI_REG[]=
{
    //{0x05, 0x0000},
    //{0x06, 0x0019},
    {0x0100, 0x00},
    {0x0104, 0x01},
    {0x08, 0x0000},
    {0x09, 0x00C8},//shutter lower
    {0x0A, 0x0000},
    //{0x20, 0x0840},
    {0x29, 0x0481},//RESERVED
    {0x3E, 0x0087},//RESERVED
    {0x3F, 0x0007},//RESERVED
    {0x41, 0x0003},//RESERVED
    {0x4B, 0x0000},
    {0x5F, 0x1C16},//RESERVED
    {0x62, 0x0000},//0x0003},
    {0x73, 0x0300},//RESERVED
    {0x75, 0x4B08},//RESERVED
    {0x76, 0x664C},//RESERVED
    {0x78, 0xA567},//RESERVED
    {0x2B, 0x0008},//Green1_Gain
    {0x2C, 0x0008},//Blue_Gain
    {0x2D, 0x0008},//Red_Gain
    {0x2E, 0x0008},//Green2_Gain
    {0x0C, 0x0000},
    {0x0104, 0x00},
    {0x0100, 0x01},
};


const SENSOR_CMD MI5100_REG_VGA[]=
{
    {0x01, 0x0036},
    {0x02, 0x0010},
    {0x03, 0x079d},
    {0x04, 0x0A2f},
    {0x22, 0x0013},
    {0x23, 0x0013},
    {0x1E, 0x4406}
};

const SENSOR_CMD MI5100_REG_1280X960[]=
{
    {0x01, 0x0036},
    {0x02, 0x0010},
    {0x03, 0x079d},
    {0x04, 0x0A2f},
    {0x22, 0x0001},
    {0x23, 0x0001},
    {0x1E, 0x4406}
};

const SENSOR_CMD MI5100_REG_FULL[]=
{
    {0x01, 0x0036},
    {0x02, 0x0010},
    {0x03, 0x079d},
    {0x04, 0x0A2F},
    {0x22, 0x0000},
    {0x23, 0x0000},
    {0x1E, 0x4406}
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       1200
#define ISOBASE         50

/*********** 1280 x 960 ***********/
#define VD_Sync_Prv_1           9
#define HD_Sync_Prv_1           312
#define VD_Total_Prv_1          994
#define HD_Total_Prv_1          2199//2192
#define ACT_STRX_Prv_1          5
#define ACT_STRY_Prv_1          4
#define IMAGESIZE_H_PRV_1       1264//1280
#define IMAGESIZE_V_PRV_1       948//960
#define AE_PRE_MAXLINE_1        (VD_Total_Prv_1 - 2)
#define SIE_PRV_XCENTER_1       (ACT_STRX_Prv_1 + (IMAGESIZE_H_PRV_1/2))
#define SIE_PRV_YCENTER_1       (ACT_STRY_Prv_1 + (IMAGESIZE_V_PRV_1/2))

/*********** 640 x 480 ***********/
#define VD_Sync_Prv_2          9
#define HD_Sync_Prv_2          312
#define VD_Total_Prv_2         514
#define HD_Total_Prv_2         2204
#define ACT_STRX_Prv_2         5
#define ACT_STRY_Prv_2         4
#define IMAGESIZE_H_PRV_2      640
#define IMAGESIZE_V_PRV_2      480
#define AE_PRE_MAXLINE_2        (VD_Total_Prv_2 - 2)
#define SIE_PRV_XCENTER_2       (ACT_STRX_Prv_2 + (IMAGESIZE_H_PRV_2/2))
#define SIE_PRV_YCENTER_2       (ACT_STRY_Prv_2 + (IMAGESIZE_V_PRV_2/2))

/*********** capture ***********/
#define VD_Sync_Cap           9
#define HD_Sync_Cap           312
#define VD_Total_Cap          1978
#define HD_Total_Cap          3500//3504
#define ACT_STRX_Cap          3//5
#define ACT_STRY_Cap          2// 4

#define AE_CAP_MAXLINE          (VD_Total_Cap - 2)

#define SensorCapTotalFld       1

#define Padding_Prv             0
#define Padding_Cap             0

#define IMAGESIZE_H_CAP         2592
#define IMAGESIZE_V_CAP         1944
#define SIE_CAP_XCENTER         (ACT_STRX_Cap + (IMAGESIZE_H_CAP/2))
#define SIE_CAP_YCENTER         (ACT_STRY_Cap + (IMAGESIZE_V_CAP/2))

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

#define OB_OFS_PRV     (0x28) // (0x28)   // (36)//10 bit
#define OB_OFS_CAP     (0x28) //(0x28)   // (36)//10 bit

/************** SIE CFA pattern selection **************/
#define CFAPAT_PRV    SIECFA_B
#define CFAPAT_CAP    SIECFA_R

/*********************************************/
//unit:mhz
#define PCLK      60    //48 // 60

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
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
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

PREVIEW_DATA_INFO MI5100_prv_mode[] = {
    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        2, // skip_max
        PCLK, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)MI5100_REG_1280X960,
        //sizeof(MI5100_REG_1280X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI5100_REG_VGA,
        //sizeof(MI5100_REG_VGA)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO MI5100_still_mode[] = {
    {
        4336, // cap_width
        3240, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)MI5100_REG_FULL,
        //sizeof(MI5100_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void MI5100_GetRegInfo(MI5100_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5100_INIT:
            *addr = (UINT32)MI5100_INI_REG;
            *size = (sizeof(MI5100_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case MI5100_VGA:
            *addr = (UINT32)MI5100_REG_VGA;
            *size = (sizeof(MI5100_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5100_1280X960:
            *addr = (UINT32)MI5100_REG_1280X960;
            *size = (sizeof(MI5100_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5100_STILL:
            *addr = (UINT32)MI5100_REG_FULL;
            *size = (sizeof(MI5100_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5100_GetSigInfo(MI5100_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5100_VGA:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5100_1280X960:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5100_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI5100_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5100_GetDataInfo(MI5100_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI5100_VGA:
            *addr = (UINT32)&MI5100_prv_mode[1];
            //*size = (sizeof(MI5100_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI5100_1280X960:
            *addr = (UINT32)&MI5100_prv_mode[0];
            //*size = (sizeof(MI5100_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case MI5100_STILL:
            *addr = (UINT32)&MI5100_still_mode;
            //*size = (sizeof(MI5100_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI5100_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI5100_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}
