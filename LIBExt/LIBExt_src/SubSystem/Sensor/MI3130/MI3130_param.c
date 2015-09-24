/*
    Settings for MI3130

    @Sensor     Micron MI3130
    @file       MI3130_param.c
    @ingroup        Sensor\MI3130
    @note       Nothing.

    Porting by Randy Ho  2012/03/08

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#include "MI3130.h"
#include "debug.h"

/*
    According to data book (MT9T013) address 0x4000-0xFFFF are reserved/undefined.

    So, for convenience to delay in a group of register settings

    addr = 0xFFFF means delay x(value) ms

    for detail, pls read **** void MI3130_WriteReg(UINT32 addr, UINT32 value) ***
    [NTK#RandyHo]
*/

SENSOR_CMD MI3130_INI_REG[]=
{
    {0x0103, 0x0001},       // software reset
    {0xFFFF, 300},          // delay 300ms

    {0x301A, 0x10CC},
    {0x3064, 0x0805},
    {0x0100, 0x0000},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
    {0x0300, 0x000A},       // VT_PIX_CLK_DIV   (default: 0x000A)
    {0x0302, 0x0001},       // VT_SYS_CLK_DIV   (default: 0x0001)
    {0x0304, 0x0001},       // PRE_PLL_CLK_DIV (default: 0x0002)
    {0x0306, 0x003C},       // PLL_MULTIPLIER (default: 0x0040)
    {0x0308, 0x000A},       // OP_PIX_CLK_DIV (default: 0x000A)
    {0x030A, 0x0001},       // OP_SYS_CLK_DIV   (default: 0x0001)
    {0xFFFF, 5},
    {0x0100, 0x0001},
    {0x0104, 0x0001},       // GROUPED_PARAMETER_HOLD (default: 0x0000)
    {0x0202, 0x0406},       // 1030 // COARSE_INTEGRATION_TIME (default: 0x0010)
    {0x300C, 0x0B7A},       // 2938 // LINE_LENGTH_PCK_ (default: 0x0D00)
    {0x3084, 0x241B},
    {0x3092, 0x0A5B},
    {0x3094, 0x5B5B},
    {0x3096, 0x5B5B},

//  {0x3070, 0x0002},       // Test Pattern, 2-100% color bar
    {0x0104, 0x0000}        // GROUPED_PARAMETER_HOLD
};

SENSOR_CMD MI3130_REG_VGA[]=
{
    {0x0100, 0x0000},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
    {0x0300, 0x000A},       // VT_PIX_CLK_DIV   (default: 0x000A)
    {0x0302, 0x0001},       // VT_SYS_CLK_DIV   (default: 0x0001)
    {0x0304, 0x0003},       // PRE_PLL_CLK_DIV (default: 0x0002)
    {0x0306, 0x0064},       // PLL_MULTIPLIER (default: 0x0040)
    {0x0308, 0x000A},       // OP_PIX_CLK_DIV (default: 0x000A)
    {0x030A, 0x0001},       // OP_SYS_CLK_DIV   (default: 0x0001)

    // Allow PLL to lock
    {0x3064, 0x0805},       // RESERVED_MFR_3064 = 0x0805
    {0x0104, 0x0001},       // Grouped Parameter Hold = 0x0001
    {0x034C, 0x0280},       // Output Width = 0x0400 - 1024
    {0x034E, 0x01E0},       // Output Height = 0x300 - 768
    {0x0344, 0x0008},       // Column Start = 0x0008
    {0x0346, 0x0008},       // Row Start = 0x0008
    {0x0348, 0x0805},       // Column End = 0x0805
    {0x034A, 0x0605},       // Row End = 0x0605
    {0x3040, 0x006C},       // Read Mode = 0x006C
    {0x0340, 0x0355},       // Frame Lines
    {0x0342, 0x077A},       // Line Length
       //{0x3070, 0x0002},
    {0x0104, 0x0000},       // Grouped Parameter Hold
    {0x0100, 0x0001},       // Mode Select

};

// XGA mode for preview mode
SENSOR_CMD MI3130_REG_XGA[]=    // According to data book (MT9T013), the sensor can run 30fps in XGA mode
{
    {0x0100, 0x0000},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
    {0x0300, 0x000A},       // VT_PIX_CLK_DIV   (default: 0x000A)
    {0x0302, 0x0001},       // VT_SYS_CLK_DIV   (default: 0x0001)
    {0x0304, 0x0003},       // PRE_PLL_CLK_DIV (default: 0x0002)
    {0x0306, 0x0064},       // PLL_MULTIPLIER (default: 0x0040)
    {0x0308, 0x000A},       // OP_PIX_CLK_DIV (default: 0x000A)
    {0x030A, 0x0001},       // OP_SYS_CLK_DIV   (default: 0x0001)

    {0xFFFF, 1},            // delay 1ms
    // Allow PLL to lock
    {0x3064, 0x0805},       // RESERVED_MFR_3064 = 0x0805
    {0x0104, 0x0001},       // Grouped Parameter Hold = 0x0001
    {0x034C, 0x0400},       // Output Width = 0x0400 - 1024
    {0x034E, 0x0300},       // Output Height = 0x300 - 768
    {0x0344, 0x0008},       // Column Start = 0x0008
    {0x0346, 0x0008},       // Row Start = 0x0008
    {0x0348, 0x0805},       // Column End = 0x0805
    {0x034A, 0x0605},       // Row End = 0x0605
    {0x3040, 0x006C},       // Read Mode = 0x006C
    {0x0340, 0x0355},       // Frame Lines
    {0x0342, 0x077A},       // Line Length
    // {0x3070, 0x0002},    // Test Pattern-2: color bars
    {0x0104, 0x0000},       // Grouped Parameter Hold
    {0x0100, 0x0001},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
};

// HD(1280*720 30fps) mode for video recording  [NTK#RandyHo]<20120605>
SENSOR_CMD MI3130_REG_HD[]=
{
    {0x0100, 0x0000},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
    {0x0300, 0x000A},       // VT_PIX_CLK_DIV   (default: 0x000A)
    {0x0302, 0x0001},       // VT_SYS_CLK_DIV   (default: 0x0001)
    {0x0304, 0x0003},       // PRE_PLL_CLK_DIV (default: 0x0002)
    {0x0306, 0x0064},       // PLL_MULTIPLIER (default: 0x0040)
    {0x0308, 0x000A},       // OP_PIX_CLK_DIV (default: 0x000A)
    {0x030A, 0x0001},       // OP_SYS_CLK_DIV   (default: 0x0001)

    {0xFFFF, 1},            // delay 1ms
    // Allow PLL to lock
    {0x3064, 0x0805},       // RESERVED_MFR_3064 = 0x0805
    {0x0104, 0x0001},       // Grouped Parameter Hold = 0x0001
    {0x034C, 0x0500},       // Output Width = 0x0500 - 1280
    {0x034E, 0x02D0},       // Output Height = 0x2D0 - 720
    {0x0344, 0x0004},       // Column Start = 0x0008
    {0x0346, 0x0000},       // Row Start = 0x0008
    {0x0348, 0x050B},       // Column End = 0x0805
    {0x034A, 0x05AD},       // Row End = 0x0605
    {0x3040, 0x002C},       // Read Mode = 0x002C
    {0x0340, 0x0348},       // Frame Lines
    {0x0342, 0x081C},       // Line Length
    // {0x3070, 0x0002},    // Test Pattern-2: color bars
    {0x0104, 0x0000},       // Grouped Parameter Hold
    {0x0100, 0x0001},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
};

SENSOR_CMD MI3130_REG_FULL[]=   // for capture mode
{
    {0x301A, 0x10CC},
    {0x3064, 0x0805},
    {0x0100, 0x0000},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
    {0x0300, 0x000A},       // VT_PIX_CLK_DIV   (default: 0x000A)
    {0x0302, 0x0001},       // VT_SYS_CLK_DIV   (default: 0x0001)
    {0x0304, 0x0001},       // PRE_PLL_CLK_DIV (default: 0x0002)
    {0x0306, 0x003C},       // PLL_MULTIPLIER (default: 0x0040)
    {0x0308, 0x000A},       // OP_PIX_CLK_DIV (default: 0x000A)
    {0x030A, 0x0001},       // OP_SYS_CLK_DIV   (default: 0x0001)

    {0xFFFF, 1},            // delay 1ms

    {0x0100, 0x0001},       // mode select, 1-streaming mode, 0-low power mode (default: 0x0000)
    {0x0104, 0x0001},       // Grouped Parameter Hold = 0x0001
    {0x0202, 0x0406},       // 1030 // COARSE_INTEGRATION_TIME (default: 0x0010)
    {0x300C, 0x0B7A},       // 2938 // LINE_LENGTH_PCK_ (default: 0x0D00)
    {0x3084, 0x241B},
    {0x3092, 0x0A5B},
    {0x3094, 0x5B5B},
    {0x3096, 0x5B5B},
/*
    {0x0104, 0x0000},
    {0x0104, 0x0001},
    {0x3016, 0x0111},
    {0x3004, 0x0008},
    {0x3008, 2065},
    {0x3002, 8},
    {0x3006, 1543},
    {0x3040, 0x0024},
    {0x0404, 16},
    {0x034C, 2058},
    {0x034E, 1536},
    {0x300C, 2938},
    {0x300A, 1621},
*/

    {0x0382, 1},    // X_ODD_INC
    {0x0386, 1},    // Y_ODD_INC
    {0x0344, 8},    // X_ADDR_START
    {0x0346, 8},    // Y_ADDR_START
    {0x0348, 2055}, // X_ADDR_END
    {0x034A, 1543}, // Y_ADDR_END
    {0x034C, 2048}, // X_OUTPUT_SIZE
    {0x034E, 1536}, // Y_OUTPUT_SIZE

     {0x0104, 0x0000}       // GROUPED_PARAMETER_HOLD
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       1750
#define ISOBASE     50

#define Padding_Prv                 0

/************** Preview Mode VGA (640 x 480)**************************/
#define VD_Sync_Prv_VGA         9
#define HD_Sync_Prv_VGA         312
#define VD_Total_Prv_VGA            853
#define HD_Total_Prv_VGA            1914

#define ACT_STRX_Prv_VGA        3
#define ACT_STRY_Prv_VGA        2
#define IMAGESIZE_H_PRV_VGA     1024
#define IMAGESIZE_V_PRV_VGA     768

#define SIE_PRV_XCENTER_VGA     (ACT_STRX_Prv_VGA+(IMAGESIZE_H_PRV_VGA/2))
#define SIE_PRV_YCENTER_VGA     (ACT_STRY_Prv_VGA+(IMAGESIZE_V_PRV_VGA/2))

#define PRV_OB_GAIN_VGA         0x9f
#define PRV_OB_THRES_VGA        0x9f
#define PRV_OB_SUBSAMPLE_VGA    0x08 //0x02
#define PRV_OB_X0_VGA           0
#define PRV_OB_X1_VGA           0
#define PRV_OB_Y0_VGA           0
#define PRV_OB_Y1_VGA           0

/********************************************************/


/************** Preview Mode XGA (1024x 768)**************************/
#define VD_Sync_Prv_XGA         9
#define HD_Sync_Prv_XGA         312
#define VD_Total_Prv_XGA        853
#define HD_Total_Prv_XGA        1914

#define VD_DataOut_Prv_XGA      0
#define HD_DataOut_Prv_XGA      0
#define VD_DataEnd_Prv_XGA      0
#define HD_DataEnd_Prv_XGA      0
#define HD_RightDummy_Prv_XGA   0
#define VD_BottomDummy_Prv_XGA  0

#define ACT_STRX_Prv_XGA        3
#define ACT_STRY_Prv_XGA        3

#define IMAGESIZE_H_PRV_XGA     1024
#define IMAGESIZE_V_PRV_XGA     768

#define SIE_PRV_XCENTER_XGA         (ACT_STRX_Prv_XGA+(IMAGESIZE_H_PRV_XGA/2))
#define SIE_PRV_YCENTER_XGA         (ACT_STRY_Prv_XGA+(IMAGESIZE_V_PRV_XGA/2))

#define PRV_OB_GAIN_XGA         0x9f
#define PRV_OB_THRES_XGA        0x9f
#define PRV_OB_SUBSAMPLE_XGA    0x08 //0x02
#define PRV_OB_X0_XGA       0
#define PRV_OB_X1_XGA       0
#define PRV_OB_Y0_XGA       0
#define PRV_OB_Y1_XGA       0

/********************************************************/

 // HD-mode added by [NTK#RandyHo]<2012/06/07>
/************** Preview Mode HD (1280x 720)**************************/
#define VD_Sync_Prv_HD         9
#define HD_Sync_Prv_HD        312
#define VD_Total_Prv_HD        853
#define HD_Total_Prv_HD       2076

#define VD_DataOut_Prv_HD      0
#define HD_DataOut_Prv_HD      0
#define VD_DataEnd_Prv_HD      0
#define HD_DataEnd_Prv_HD      0
#define HD_RightDummy_Prv_HD   0
#define VD_BottomDummy_Prv_HD  0

#define ACT_STRX_Prv_HD        3
#define ACT_STRY_Prv_HD        3

#define IMAGESIZE_H_PRV_HD     1280
#define IMAGESIZE_V_PRV_HD      720

#define SIE_PRV_XCENTER_HD         (ACT_STRX_Prv_HD+(IMAGESIZE_H_PRV_HD/2))
#define SIE_PRV_YCENTER_HD         (ACT_STRY_Prv_HD+(IMAGESIZE_V_PRV_HD/2))

#define PRV_OB_GAIN_HD             0x9f
#define PRV_OB_THRES_HD         0x9f
#define PRV_OB_SUBSAMPLE_HD    0x08 //0x02
#define PRV_OB_X0_HD       0
#define PRV_OB_X1_HD       0
#define PRV_OB_Y0_HD       0
#define PRV_OB_Y1_HD       0

/********************************************************/


/************** Still Mode **************/
#define VD_Sync_Cap         9
#define HD_Sync_Cap         312
#define VD_Total_Cap            1621
#define HD_Total_Cap            2938

#define VD_ImgOutStart_Ca       0
#define VD_ImgOutEnd_Cap        0
#define HD_ImgOutStart_Cap      0
#define HD_ImgOutEnd_Cap        0

#define IMAGESIZE_H_CAP     2048
#define IMAGESIZE_V_CAP     1536

#define ACT_STRX_Cap            3
#define ACT_STRY_Cap            3

#define SensorCapTotalFld       1

#define Padding_Cap         0

#define SIE_CAP_XCENTER     (ACT_STRX_Cap+IMAGESIZE_H_CAP/2)
#define SIE_CAP_YCENTER     (ACT_STRY_Cap+IMAGESIZE_V_CAP/2)

#define CAP_OB_GAIN         0x20
#define CAP_OB_THRES        0xF0
#define CAP_OB_SUBSAMPLE    0x05
#define CAP_OB_X0           0
#define CAP_OB_X1           0
#define CAP_OB_Y0           0
#define CAP_OB_Y1           0

#define OB_OFS_PRV          0x28    //10 bit
#define OB_OFS_CAP          0x28    //10 bit
/************** PRE output for IPE CFA pattern selection **************/
#define CFAPAT_PRV          SIECFA_R
#define CFAPAT_CAP          SIECFA_R

/*********************************************/
//unit:mhz
#define PCLK_PRV_XGA       50   // count the signal by oscilloscope

#define PCLK_PRV_HD       50

#define PCLK_CAP        90 //72

SYNC_INFO prv_mode_vga =
{
    ACT_STRX_Prv_VGA, // Act_StartX;
    ACT_STRY_Prv_VGA, // Act_StartY;
    SIE_PRV_XCENTER_VGA, // Act_CenterX;
    SIE_PRV_YCENTER_VGA, // Act_CenterY;
    IMAGESIZE_H_PRV_VGA  , // Act_SizeX;
    IMAGESIZE_V_PRV_VGA, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_VGA, //    HD_sync
    VD_Sync_Prv_VGA, //    VD_sync
    HD_Total_Prv_VGA, //    HD_total
    VD_Total_Prv_VGA, //    VD_total

    // ob_area
    PRV_OB_X0_VGA, //    OB_X0
    PRV_OB_Y0_VGA, //    OB_Y0
    PRV_OB_X1_VGA, //    OB_X1
    PRV_OB_Y1_VGA, //    OB_Y1
    PRV_OB_THRES_VGA, //    OB_Thres
    PRV_OB_SUBSAMPLE_VGA, //    OB_SubSample
    PRV_OB_GAIN_VGA, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv // padding;
};

SYNC_INFO prv_mode_xga =    // XGA
{
    ACT_STRX_Prv_XGA, // act_start_x;
    ACT_STRY_Prv_XGA, // act_start_y;
    SIE_PRV_XCENTER_XGA, // act_center_x;
    SIE_PRV_YCENTER_XGA, // act_center_y;
    IMAGESIZE_H_PRV_XGA , // Act_SizeX;
    IMAGESIZE_V_PRV_XGA, // Act_SizeY;

    // HD/VD_Sync
    HD_Sync_Prv_XGA, //    HD_sync
    VD_Sync_Prv_XGA, //    VD_sync
    HD_Total_Prv_XGA, //    HD_total
    VD_Total_Prv_XGA, //    VD_total

    // ob_area
    PRV_OB_X0_XGA, //    OB_X0
    PRV_OB_Y0_XGA, //    OB_Y0
    PRV_OB_X1_XGA, //    OB_X1
    PRV_OB_Y1_XGA, //    OB_Y1
    PRV_OB_THRES_XGA, //    OB_Thres
    PRV_OB_SUBSAMPLE_XGA, //    OB_SubSample
    PRV_OB_GAIN_XGA, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv // padding;
};

 // HD-mode added by [NTK#RandyHo]<2012/06/07>
SYNC_INFO prv_mode_hd =    // hd
{
    ACT_STRX_Prv_HD, // act_start_x;
    ACT_STRY_Prv_HD, // act_start_y;
    SIE_PRV_XCENTER_HD, // act_center_x;
    SIE_PRV_YCENTER_HD, // act_center_y;
    IMAGESIZE_H_PRV_HD , // Act_SizeX;
    IMAGESIZE_V_PRV_HD, // Act_SizeY;

    // HD/VD_Sync
    HD_Sync_Prv_HD, //    HD_sync
    VD_Sync_Prv_HD, //    VD_sync
    HD_Total_Prv_HD, //    HD_total
    VD_Total_Prv_HD, //    VD_total

    // ob_area
    PRV_OB_X0_HD, //    OB_X0
    PRV_OB_Y0_HD, //    OB_Y0
    PRV_OB_X1_HD, //    OB_X1
    PRV_OB_Y1_HD, //    OB_Y1
    PRV_OB_THRES_HD, //    OB_Thres
    PRV_OB_SUBSAMPLE_HD, //    OB_SubSample
    PRV_OB_GAIN_HD, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv // padding;
};

SYNC_INFO still_mode = {    // Still Mode
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

PREVIEW_DATA_INFO MI3130_prv_mode[] =
{
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_PRV_XGA, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)MI3130_REG_VGA,
        //sizeof(MI3130_REG_VGA)/sizeof(SENSOR_CMD)
    },
    { // SEN_MODE_XGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_PRV_XGA, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI3130_REG_1280X960,
        //sizeof(MI3130_REG_1280X960)/sizeof(SENSOR_CMD)
    },
    { // SEN_MODE_HD
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_PRV_HD, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI3130_REG_1280X960,
        //sizeof(MI3130_REG_1280X960)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO MI3130_still_mode[] =
{
    {
        2048,   // cap_width
        1536,   // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_CAP,   // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)MI3130_REG_FULL,
        //sizeof(MI3130_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void MI3130_GetRegInfo(MI3130_MODE mode, UINT32* addr, UINT32* size)
{
    debug_msg("MI3130_GetRegInfo...\r\n");

    switch(mode)
    {
    case MI3130_INIT:
        *addr = (UINT32)MI3130_INI_REG;
        *size = (sizeof(MI3130_INI_REG)/sizeof(SENSOR_CMD));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        break;

    case MI3130_VGA:
        *addr = (UINT32)MI3130_REG_VGA;
        *size = (sizeof(MI3130_REG_VGA)/sizeof(SENSOR_CMD));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
        break;

    case MI3130_XGA:
        *addr = (UINT32)MI3130_REG_XGA;
        *size = (sizeof(MI3130_REG_XGA)/sizeof(SENSOR_CMD));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
        break;

    // HD-mode added by [NTK#RandyHo]<2012/06/07>
    case MI3130_HD:
        *addr = (UINT32)MI3130_REG_HD;
        *size = (sizeof(MI3130_REG_HD)/sizeof(SENSOR_CMD));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
        break;

    case MI3130_STILL:
        *addr = (UINT32)MI3130_REG_FULL;
        *size = (sizeof(MI3130_REG_FULL)/sizeof(SENSOR_CMD));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
        break;

    default:
        debug_err(("^R %s: mode error(default)",__func__));
        break;
    }

    return;
}

extern void MI3130_GetSigInfo(MI3130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
    case MI3130_VGA:
        *addr = (UINT32)&prv_mode_vga;
        //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
        break;

    case MI3130_XGA:
        *addr = (UINT32)&prv_mode_xga;
        //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
        break;

    // HD-mode added by [NTK#RandyHo]<2012/06/07>
    case MI3130_HD:
        *addr = (UINT32)&prv_mode_hd;
        //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
        break;

    case MI3130_STILL:
        *addr = (UINT32)&still_mode;
        //size = (sizeof(still_mode)/sizeof(SYNC_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
        break;

    case MI3130_INIT:
        //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
    default:
        debug_err(("^R %s: mode error(default)",__func__));
        break;
    }

    return;
}

extern void MI3130_GetDataInfo(MI3130_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
    case MI3130_VGA:
        *addr = (UINT32)&MI3130_prv_mode[0];
        //*size = (sizeof(MI3130_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
        break;

    case MI3130_XGA:
        *addr = (UINT32)&MI3130_prv_mode[1];
        //*size = (sizeof(MI3130_prv_mode)/sizeof(PREVIEW_DATA_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
        break;

    // HD-mode added by [NTK#RandyHo]<2012/06/07>
     case MI3130_HD:
        *addr = (UINT32)&MI3130_prv_mode[2];
        //*size = (sizeof(MI3130_prv_mode)/sizeof(PREVIEW_DATA_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
        break;

    case MI3130_STILL:
        *addr = (UINT32)&MI3130_still_mode;
        //*size = (sizeof(MI3130_still_mode)/sizeof(CAPTURE_DATA_INFO));
        //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
        break;

    case MI3130_INIT:
        //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
    default:
        debug_err(("^R %s: mode error(default)",__func__));
        break;
    }

    return;
}

extern void MI3130_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;

    return;
}
