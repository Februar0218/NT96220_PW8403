#include "MI1600.h"
#include "debug.h"
//PLL: 12 Mhz Input, 49 Mhz Output
const SENSOR_CMD MI1600_INI_REG[]=
{
    {0x10, 0x0051}, //PLL_CONTROL
    {0x10, 0x0050}, //PLL_CONTROL
    {0x10, 0x0051}, //PLL_CONTROL
    {0x11, 0x3101}, //PLL_CONFIG_1: Multiplier = 0x31, Divider = 0x01+1, PLL config = 49/2
    {0x10, 0x0053}  //PLL_CONTROL
};
//Sensor PXCLK = (12MHz*49/2)/6 = 49Mhz
//aclk = 49 Mhz /2 = 24.5 MHz = 40.81us ~ 40.77us
//[scope] extclk = 12Mhz
//[scope] pxlclk = 49Mhz


const SENSOR_CMD MI1600_INI2_REG[]=
{
    {0x2B, 0x0010}, //Gain Green1 = 0x10
    {0x2C, 0x001C}, //Gain Blue = 0x1C
    {0x2D, 0x0010}, //Gain Red = 0x10
    {0x2E, 0x0010}, //Gain Green2 = 0x10
    //{0x20, 0x0047}, //READ_MODE_2
    //{0x20, (0x0047|0x1800|0xC000)}, //READ_MODE_2
    {0x20, (0x0047|0x1800)}, //READ_MODE_2
    // ===> enable OB output to extend effective data region for SIE active window
    // ===> disable Mirror Row & Mirror Column
    {0x41, 0x0000}, //CLAMP_VOLTAGE
    {0x42, 0x0003}, //ADC_REF_LO_VOLTAGE
    {0x43, 0x0003}, //ADC_REF_HI_VOLTAGE
    {0x7F, 0x0000}, //RD_EN_VLN_SH_TIMING
    {0x1E, 0x8006}, //Set parallel mode
    {0x9F, 0x3070}, //Parallel data and clock out
    {0x9E, 0x111E}  //FV_LV timing adjustment
};




//[scope] VD = 30.67 Hz
// N * 27.71us = 32.60ms = 32600us, N = 1176, VD_total = 1176
// blank = 19.1ms, N * 27.71us = 19.1ms = 19100us, N = 689
// VD_effect = VD_total - VD_sync = 1176 - 689 = 487
//[scope] HD = 36.08 KHz
// N aclk / 24.5MHz = 27.71us, N = 689, HD_total = N*2 = 1358
// blank = 13.0us, N aclk / 24.5MHz = 13.0us, N = 319 aclk, HD_sync = N*2 = 319*2 = 637
// HD_effect = HD_total - HD_sync = 1358 - 637 = 721

const SENSOR_CMD MI1600_REG_VGA[]=
{
    {0x20, (0x0047|0x1800)}, //READ_MODE_2
    //{0x01, 0x0168}, //StartRaws = 360 = 60+(1080-480)/2
    //{0x02, 0x01A0}, //StartCols = 416 = 16+(1440-640)/2
    //Vendor Settings:
    {0x01, 0x01A0}, //StartRaws = 416
    {0x02, 0x0134}, //StartCols = 306
    {0x03, 0x01DF}, //ActiveRaws = 480
    {0x04, 0x027F}, //ActiveCols = 640
    {0x05, 0x0162}, //BlankCols = 354
    {0x06, 0x02B7}, //BlankRaws = 695
    {0x22, 0x0000},  //Row Mode = 0x0
    {0x23, 0x0000},  //Column Mode = 0x0
    {0x0C, 0x00AD}  //ShutterDelay = 173
};

//[scope] VD = 30.50 Hz = 32.74ms
// N * 40.77us = 32.74ms = 32740us, N = 803, VD_total = 803
// blank = 3.07ms, N * 40.77us = 3.07ms = 3070us, N = 75
// VD_effect = VD_total - VD_sync = 803 - 75 = 728
//[scope] HD = 24.52 KHz = 40.77us
// N aclk / 24.5MHz = 40.77us, N = 999 aclk, HD_total = N*2 = 1998
// blank = 13.1us, N aclk / 24.5MHz = 13.1us, N = 321 aclk, HD_sync = N*2 = 321*2 = 642
// HD_effect = HD_total - HD_sync = 1998 - 642 = 1356

const SENSOR_CMD MI1600_REG_1280X720[]=
{
    {0x20, (0x0047|0x1800)}, //READ_MODE_2
    //Vendor Settings:
    {0x01, 0x00F0}, //StartRaws = 240 = 60+(1080-720)/2
    {0x02, 0x0060}, //StartCols = 96 = 16+(1440-1280)/2
    {0x03, 0x02CF}, //ActiveRaws = 720
    {0x04, 0x04FF}, //ActiveCols = 1280
    {0x05, 0x0162}, //BlankCols = 354
    {0x06, 0x004E}, //BlankRaws = 78
    {0x22, 0x0000}, //Row Mode = 0x0
    {0x23, 0x0000}, //Column Mode = 0x0
    {0x0C, 0x01ED}  //ShutterDelay = 493
};

//[scope] VD = 15.00 Hz = 66.60ms
// N * 44.04us = 66.60ms = 66600us, N = 1512, VD_total = 1512
// blank = 18.7ms, N * 44.04us = 18.7ms = 18700us, N = 425, VD_sync = N = 425
// VD_effect = VD_total - VD_sync = 1512 - 425 = 1087
//[scope] HD = 22.71 KHz = 44.04us
// N aclk / 24.5MHz = 44.04us, N = 1079, HD_total = N*2 = 2158
// blank = 13.00us, N aclk / 24.5MHz = 13.00us, N = 319 aclk, HD_sync = N*2 = 319*2 = 648
// HD_effect = HD_total - HD_sync = 2158 - 648 = 1510

// total = 47.8ms, N * 44.04us = 47.8ms = 47800us, N = 1085, VD_total = N = 1085 //after enable OB
// total = 31.00us, N aclk / 24.5MHz = 31.00us, N = 760 aclk, HD_total = N*2 = 1520 //after enable OB
const SENSOR_CMD MI1600_REG_1440X1080[]=
{
    {0x20, (0x0047|0x1800)}, //READ_MODE_2
    //Vendor Settings:
    {0x01, 0x003C}, //StartRaws = 60
    {0x02, 0x0010}, //StartCols = 16
    {0x03, 0x0437}, //ActiveRaws = 1080
    {0x04, 0x059F}, //ActiveCols = 1440
    {0x05, 0x0162}, //BlankCols = 354
    {0x06, 0x01B0}, //BlankRaws = 432
    {0x22, 0x0000}, //Row Mode = 0x0
    {0x23, 0x0000}, //Column Mode = 0x0
    {0x0C, 0x023D}  //ShutterDelay = 573
};


//[scope] VD = 29.92 Hz = 33.42ms
// N * 60.46us = 33.42ms = 33420us, N = 553, VD_total = 553
// blank = 182us, N * 60.46us = 182us, N = 3, VD_sync = N = 3
// VD_effect = VD_total - VD_sync = 553 - 3 = 550
//[scope] HD = 16.54 KHz = 60.46us
// N aclk / 24.5MHz = 60.46us, N = 1481, HD_total = N*2 = 2962
// blank = 29.00us, N aclk / 24.5MHz = 29.00us, N = 711 aclk, HD_sync = N*2 = 1421
// HD_effect = HD_total - HD_sync = 2962 - 1421 = 1541
const SENSOR_CMD MI1600_REG_1440X540[]=
{
    {0x20, (0x0047|0x1800)}, //READ_MODE_2
    //Vendor Settings:
    {0x01, 0x0036}, //StartRaws = 60
    {0x02, 0x0010}, //StartCols = 16
    {0x03, 0x0438}, //ActiveRaws = 1081
    {0x04, 0x05A0}, //ActiveCols = 1441
    {0x05, 0x02F7}, //BlankCols = 759
    {0x06, 0x0008}, //BlankRaws = 8
    {0x22, 0x0001}, //Row Mode = 0x1
    {0x23, 0x0000}, //Column Mode = 0x0
    {0x0C, 0x0000}  //ShutterDelay = 0
};


const SENSOR_CMD MI1600_REG_FULL[]=
{
    {0x20, (0x0047|0x1800)}, //READ_MODE_2
    //Vendor Settings:
    {0x01, 0x003C}, //StartRaws = 60
    {0x02, 0x0010}, //StartCols = 16
    {0x03, 0x0437}, //ActiveRaws = 1080
    {0x04, 0x059F}, //ActiveCols = 1440
    {0x05, 0x0162}, //BlankCols = 354
    {0x06, 0x01B0}, //BlankRaws = 432
    {0x22, 0x0000}, //Row Mode = 0x0
    {0x23, 0x0000}, //Column Mode = 0x0
    {0x0C, 0x023D}  //ShutterDelay = 573
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       1000
#define ISOBASE         50



//crop window
// crop x,y is calculate from "active center point" & dzoom[0].w, h
// crop w,h is come from dzoom[0].w, h


/*********** 640 x 480 ***********/
//VDHD period (only for SIE master mode)
#define VD_Sync_Prv_1           689
#define HD_Sync_Prv_1           637
#define VD_Total_Prv_1          1176
#define HD_Total_Prv_1          1358
//active window -> must locate inside [effect window]
#define ACT_STRX_Prv_1          36 //6 (shift right for skip OB)
#define ACT_STRY_Prv_1          2
#define IMAGESIZE_H_PRV_1       721 //640
#define IMAGESIZE_V_PRV_1       488 //480
#define AE_PRE_MAXLINE_1        (VD_Total_Prv_1 - 2)
//active center point
#define SIE_PRV_XCENTER_1       (ACT_STRX_Prv_1 + (IMAGESIZE_H_PRV_1/2) + 1)
#define SIE_PRV_YCENTER_1       (ACT_STRY_Prv_1 + (IMAGESIZE_V_PRV_1/2))

/*********** 1280 x 720 ***********/
//VDHD period (only for SIE master mode)
#define VD_Sync_Prv_2           75
#define HD_Sync_Prv_2           642
#define VD_Total_Prv_2          803
#define HD_Total_Prv_2          1998
//active window -> must locate inside [effect window]
#define ACT_STRX_Prv_2          36 //8 (shift right for skip OB)
#define ACT_STRY_Prv_2          2
#define IMAGESIZE_H_PRV_2       1356 //1280
#define IMAGESIZE_V_PRV_2       728 //720
#define AE_PRE_MAXLINE_2        (VD_Total_Prv_2 - 2)
//active center point
#define SIE_PRV_XCENTER_2       (ACT_STRX_Prv_2 + (IMAGESIZE_H_PRV_2/2) + 1)
#define SIE_PRV_YCENTER_2       (ACT_STRY_Prv_2 + (IMAGESIZE_V_PRV_2/2))

/*********** 1440 x 1080 ***********/
//VDHD period (only for SIE master mode)
#define VD_Sync_Prv_3           425
#define HD_Sync_Prv_3           648
#define VD_Total_Prv_3          1512
#define HD_Total_Prv_3          2158
//active window -> must locate inside [effect window]
#define ACT_STRX_Prv_3          40 //6 (shift right for skip OB)
#define ACT_STRY_Prv_3          2
#define IMAGESIZE_H_PRV_3       1510 //1440
#define IMAGESIZE_V_PRV_3       1088 //1080
#define AE_PRE_MAXLINE_3        (VD_Total_Prv_3 - 2)
//active center point
#define SIE_PRV_XCENTER_3       (ACT_STRX_Prv_3 + (IMAGESIZE_H_PRV_3/2))
#define SIE_PRV_YCENTER_3       (ACT_STRY_Prv_3 + (IMAGESIZE_V_PRV_3/2))


/*********** 1440 x 540 ***********/
//VDHD period (only for SIE master mode)
#define VD_Sync_Prv_4           3
#define HD_Sync_Prv_4           1421
#define VD_Total_Prv_4          553
#define HD_Total_Prv_4          2962
//active window -> must locate inside [effect window]
#define ACT_STRX_Prv_4          40 //6 (shift right for skip OB)
#define ACT_STRY_Prv_4          2
#define IMAGESIZE_H_PRV_4       1541
#define IMAGESIZE_V_PRV_4       550
#define AE_PRE_MAXLINE_4        (VD_Total_Prv_4 - 2)
//active center point
#define SIE_PRV_XCENTER_4       (ACT_STRX_Prv_4 + (IMAGESIZE_H_PRV_4/2) + 1)
#define SIE_PRV_YCENTER_4       (ACT_STRY_Prv_4 + (IMAGESIZE_V_PRV_4/2) + 1)


/*********** capture ***********/
//VDHD period (only for SIE master mode)
#define VD_Sync_Cap             425
#define HD_Sync_Cap             648
#define VD_Total_Cap            1512
#define HD_Total_Cap            2158

//active window -> must locate inside [effect window]
#define ACT_STRX_Cap            52//40+52 //6 (shift right for skip OB)
#define ACT_STRY_Cap            4// 2+16
#define AE_CAP_MAXLINE          (VD_Total_Cap - 2)

#define SensorCapTotalFld       1

#define Padding_Prv             0
#define Padding_Cap             0

#define IMAGESIZE_H_CAP         1510 //1440
#define IMAGESIZE_V_CAP         1088 //1080
#define SIE_CAP_XCENTER         (ACT_STRX_Cap + (IMAGESIZE_H_CAP/2) + 1)
#define SIE_CAP_YCENTER         (ACT_STRY_Cap + (IMAGESIZE_V_CAP/2))

/************** OB Setting **************/
#define PRV_OB_GAIN             0x20
#define PRV_OB_THRES            0xF0
#define PRV_OB_SUBSAMPLE        0x02
#define PRV_OB_X0               0
#define PRV_OB_X1               0
#define PRV_OB_Y0               0
#define PRV_OB_Y1               0

#define CAP_OB_GAIN             0x20
#define CAP_OB_THRES            0xF0
#define CAP_OB_SUBSAMPLE        0x05
#define CAP_OB_X0               0
#define CAP_OB_X1               0
#define CAP_OB_Y0               0
#define CAP_OB_Y1               0

#define OB_OFS_PRV              (45)//10 bit
#define OB_OFS_CAP              (50)//10 bit

/************** SIE CFA pattern selection **************/
#define CFAPAT_PRV              SIECFA_B
#define CFAPAT_CAP              SIECFA_B

/*********************************************/
//unit:mhz
#define PCLK                    49 //48 // 60

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

    Padding_Prv // padding;
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

PREVIEW_DATA_INFO MI1600_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)MI1600_REG_VGA,
        //sizeof(MI1600_REG_VGA)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)MI1600_REG_1280X720,
        //sizeof(MI1600_REG_1280X720)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1440X1080
        1, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode3, // prv_sync_info
        //(SENSOR_CMD*)MI1600_REG_1440X1080,
        //sizeof(MI1600_REG_1440X1080)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1440X540
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK, // pclk
        //&prv_mode4, // prv_sync_info
        //(SENSOR_CMD*)MI1600_REG_1440X540,
        //sizeof(MI1600_REG_1440X540)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO MI1600_still_mode[] = {
    {
        4336, // cap_width
        3240, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)MI1600_REG_FULL,
        //sizeof(MI1600_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void MI1600_GetRegInfo(MI1600_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI1600_INIT:
            *addr = (UINT32)MI1600_INI_REG;
            *size = (sizeof(MI1600_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case MI1600_INIT2:
            addr = (UINT32*)MI1600_INI2_REG;
            *size = (sizeof(MI1600_INI2_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT2)));
            break;
        case MI1600_VGA:
            *addr = (UINT32)MI1600_REG_VGA;
            *size = (sizeof(MI1600_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI1600_1280X720:
            *addr = (UINT32)MI1600_REG_1280X720;
            *size = (sizeof(MI1600_REG_1280X720)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case MI1600_1440X1080:
            *addr = (UINT32)MI1600_REG_1440X1080;
            *size = (sizeof(MI1600_REG_1440X1080)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X1080)));
            break;
        case MI1600_1440X540:
            *addr = (UINT32)MI1600_REG_1440X540;
            *size = (sizeof(MI1600_REG_1440X540)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X540)));
            break;
        case MI1600_STILL:
            *addr = (UINT32)MI1600_REG_FULL;
            *size = (sizeof(MI1600_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI1600_GetSigInfo(MI1600_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI1600_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI1600_1280X720:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case MI1600_1440X1080:
            *addr = (UINT32)&prv_mode3;
            //*size = (sizeof(prv_mode3)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X1080)));
            break;
        case MI1600_1440X540:
            *addr = (UINT32)&prv_mode4;
            //*size = (sizeof(prv_mode4)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X540)));
            break;
        case MI1600_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI1600_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        case MI1600_INIT2:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT2)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI1600_GetDataInfo(MI1600_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case MI1600_VGA:
            *addr = (UINT32)&MI1600_prv_mode[0];
            //*size = (sizeof(MI1600_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case MI1600_1280X720:
            *addr = (UINT32)&MI1600_prv_mode[1];
            //*size = (sizeof(MI1600_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case MI1600_1440X1080:
            *addr = (UINT32)&MI1600_prv_mode[2];
            //*size = (sizeof(MI1600_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X1080)));
            break;
        case MI1600_1440X540:
            *addr = (UINT32)&MI1600_prv_mode[3];
            //*size = (sizeof(MI1600_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1440X540)));
            break;
        case MI1600_STILL:
            *addr = (UINT32)&MI1600_still_mode;
            //*size = (sizeof(MI1600_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case MI1600_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        case MI1600_INIT2:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT2)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void MI1600_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}