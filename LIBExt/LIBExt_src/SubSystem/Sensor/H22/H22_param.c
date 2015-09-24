#include "H22.h"
#include "debug.h"


//#define   _SENSOR_SETTING         	0		//
//#define   _SENSOR_SETTING             1       // 1280x800 @30fps
//#define       _SENSOR_SETTING             2       // 1280x736 @30fps
//#define   _SENSOR_SETTING             3       // 1280x736 @25fps
//#define   _SENSOR_SETTING             4       // 1280x720 @30fps
//#define   _SENSOR_SETTING             5       // 1280x720 @25fps

//#define     _SENSOR_SETTING             1       // 1280x732 @25fps
 #define   _SENSOR_SETTING             2       // 1280x732 @30fps

const SENSOR_CMD H22_INI_REG[] = {
    #if (_SENSOR_SETTING == 1) //1280x732-25fps.reg //42Pclk
    //[JXH22(AA)_1280x732x25_Fx3_DVP_8b.reg]
    //INI Start
    {0x12,0x40},
    //DVP Setting
    {0x0D,0x40},//0x40},
    {0x1F,0x04},
    //PLL Setting
    {0x0E,0x1D},//0x1D},
    {0x0F,0x09},//0x09},
    {0x10,0x23},//0x23},
    {0x11,0x80},
    //Frame/Window
    {0x20,0x90},//0x98},
    {0x21,0x06},
//  {0x22,0x3E},//30fps
//  {0x23,0x03},
    {0x22,0xE4},//25fps
    {0x23,0x03},
    {0x24,0x00},
    {0x25,0xDC},
    {0x26,0x25},
    {0x27,0x3E},//0x3F}
    {0x28,0x0a},//0x09}
    {0x29,0x01},
    {0x2A,0x24},
    {0x2B,0x29},
    {0x2C,0x03},
    {0x2D,0x00},
    {0x2E,0xBB},
    {0x2F,0x00},
    //Sensor Timing
    {0x30,0x92},
    {0x31,0x10},
    {0x32,0xAA},
    {0x33,0x10},
    {0x34,0x35},
    {0x35,0x54},
    {0x42,0x41},
    {0x43,0x40},
    //Interface
    {0x1D,0xFF},
    {0x1E,0x9F},//0x1F},
    {0x6C,0x90},
    {0x73,0xB3},
    {0x70,0x68},
    {0x76,0x40},
    {0x77,0x06},
    //Array/AnADC/PWC
    {0x60,0xA4},
    {0x61,0xFF},
    {0x62,0x40},
//  {0x63,0x51},
    {0x65,0x00},
    {0x67,0x30},
    {0x68,0x04},
    {0x69,0x74},//0x74},
    {0x6A,0x09},
    {0x6F,0x04},
    //AE/AG/ABLC
    {0x13,0x67},//0x66},//86
    {0x14,0x80},
    {0x16,0xC0},
    {0x17,0x40},
    {0x18,0xF8},
    {0x38,0x38},
    {0x39,0x92},
    {0x4a,0x03},
    {0x49,0x00},//0x83},
    //INI End
    {0x12,0x30},
    //PWDN Setting

    
    #elif (_SENSOR_SETTING == 2) //1280x732-30fps.reg
    //[JXH22(AA)_1280x732x30_Fx3_DVP_8b.reg]
    //INI Start
    {0x12,0x40},//eric edit
    //{0x12,0x00},//eric edit
    //DVP Setting
    {0x0D,0x40},
    {0x1F,0x04},
    //PLL Setting
    {0x0E,0x1D},
    {0x0F,0x09},
    {0x10,0x23},
    {0x11,0x80},
    //Frame/Window
    {0x20,0x98},
    {0x21,0x06},
    {0x22,0x3E},
    {0x23,0x03},

    {0x24,0x00},//0x500*0x2DC
    {0x25,0xDC},
    {0x26,0x25},

    {0x27,0x3F},//3B//eric edit
    {0x28,0x09},//0D//eric edit
    //{0x27,0xE9},//3B//eric edit
    //{0x28,0x0D},//0D//eric edit
    {0x29,0x01},
    {0x2A,0x24},
    {0x2B,0x29},
    {0x2C,0x03},
    {0x2D,0x00},
    {0x2E,0xBB},
    {0x2F,0x00},
    //Sensor Timing
    {0x30,0x92},
    {0x31,0x10},
    {0x32,0xAA},
    {0x33,0x10},
    {0x34,0x35},
    {0x35,0x54},
    {0x42,0x41},
    {0x43,0x40},
    //Interface
    {0x1D,0xFF},
    {0x1E,0x1F},
    {0x6C,0x90},
    {0x73,0xB3},
    {0x70,0x68},
    {0x76,0x40},
    {0x77,0x06},
    //Array/AnADC/PWC
    {0x60,0xA4},
    {0x61,0xFF},
    {0x62,0x40},
    {0x65,0x00},
    {0x67,0x30},
    {0x68,0x04},
    {0x69,0x74},
    {0x6F,0x04},
    //AE/AG/ABLC
    {0x13,0x66},//86
    {0x14,0x80},
    {0x16,0xC0},
    {0x17,0x40},
    {0x18,0xF8},
    {0x38,0x38},
    {0x39,0x92},
    {0x4a,0x03},
    {0x49,0x10},
    //INI End
    {0x12,0x00},
    //PWDN Setting
    #endif
};

/*
H22 PLL Setting
CLK1 = MCLK/REG5C[6:5]
CLK2 = CLK1*(32-REG5C[4:0])
CLK3 = CLK2/(REG5D[3:2]+1)
PCLK = CLK3/((REG11[5:0]+1)*2)
*/

const SENSOR_CMD H22_INIT_REG_15FPS[] = {
    {0x00, 0x00},
};

const SENSOR_CMD H22_INIT_REG_20FPS[] = {
    {0x00, 0x00},
};

const SENSOR_CMD H22_INIT_REG_25FPS[] = {
    {0x00, 0x00},
};

SENSOR_CMD H22_REG_FULL[] = {
    {0x00, 0x00},
};

/**
* ISO to AGC usage
*/
#define     AGC_ISOMin  			0
#define     ISOBASE     			100

/**************Default Sensor mode selection for preview**************/

/************** STILL_MODE **************/
#define     VD_Sync_Cap     		1
#define     HD_Sync_Cap     		1
#define     VD_Total_Cap                996//830//996//995//853	//804{22,23}
#define     HD_Total_Cap                1700//1680//1500//1500//1688{20,21} W

#define     IMAGESIZE_H_CAP         1280//1280
///#define  IMAGESIZE_V_CAP     	800
#if ((_SENSOR_SETTING == 1) || (_SENSOR_SETTING == 2)) //1280x720
    #define IMAGESIZE_V_CAP     	732
#elif ((_SENSOR_SETTING == 3) || (_SENSOR_SETTING == 4)) //1280x720
    #define IMAGESIZE_V_CAP     	732
#else //1280x800
    #define IMAGESIZE_V_CAP     	800
#endif

#define     ACT_STRX_Cap        	9	// 8
#define     ACT_STRY_Cap        	2	// 1
#define     ACT_STRY_Cap2       	0xff
#define     ACT_STRY_Cap3       	0xff
#define     ACT_STRY_Cap4       	0xff
#define     ACT_STRY_Cap5       	0xff
#define     ACT_STRY_Cap6       	0xff
#define     ACT_STRY_Cap7       	0xff
#define     ACT_STRY_Cap8       	0xff

#define     SensorCapTotalFld		1
#define     Field_Num1      		1
#define     Field_Num2      		0xff
#define     Field_Num3      		0xff
#define     Field_Num4      		0xff
#define     Field_Num5      		0xff
#define     Field_Num6      		0xff
#define     Field_Num7      		0xff
#define     Field_Num8      		0xff

#define     Padding_Cap     		0
#define     SIE_CAP_XCENTER         (ACT_STRX_Cap+(IMAGESIZE_H_CAP/2))
#define     SIE_CAP_YCENTER         (ACT_STRY_Cap+(IMAGESIZE_V_CAP/2))

/**************Default Sensor mode selection for preview**************/
//#define   SENSOR_PRV_MODE         MONITOR_MODE_3
//#define   SENSOR_AF_MODE          MONITOR_MODE_4
//#define   SENSOR_PRV_VGATYPE      SENSOR_OUT_PROGRESSIVE

/************** OB Setting **************/
#define     PRV_OB_GAIN     	0x20
#define     PRV_OB_THRES    		0xF0
#define     PRV_OB_SUBSAMPLE    	0x02
#define     PRV_OB_X0       		1264
#define     PRV_OB_X1       		4900
#define     PRV_OB_Y0   			9
#define     PRV_OB_Y1   			9

#define     CAP_OB_GAIN     	0x20
#define     CAP_OB_THRES    		0xF0
#define     CAP_OB_SUBSAMPLE    	0x05
#define     CAP_OB_X0       		1200
#define     CAP_OB_X1       		4900
#define     CAP_OB_Y0   			17
#define     CAP_OB_Y1   			17

#define     OB_OFS_PRV              4//4//1// 1	// 10 bit
#define     OB_OFS_CAP              4//4//1// 1	// 10 bit

/************** SIE CFA pattern selection **************/
#define     CFAPAT_PRV          	SIECFA_R
#define     CFAPAT_CAP          	SIECFA_R

/*********************************************/
//unit:mhz
#define PCLK      42
#define PCLK_15FPS  42//21
#define PCLK_20FPS  42//28
#define PCLK_25FPS  42//36
#define PCLK_STILL  42

SYNC_INFO still_mode = {
    ACT_STRX_Cap,       // act_start_x;
    ACT_STRY_Cap,       // act_start_y;
    SIE_CAP_XCENTER,    // act_center_x;
    SIE_CAP_YCENTER,    // act_center_y;
    IMAGESIZE_H_CAP,    // Act_SizeX;
    IMAGESIZE_V_CAP,    // Act_SizeY;
    
    // HDVD_Sync
    HD_Sync_Cap,        // HD_sync
    VD_Sync_Cap,        // VD_sync
    HD_Total_Cap,       // HD_total
    VD_Total_Cap,       // VD_total
    
    // ob_area
    CAP_OB_X0,          // OB_X0
    CAP_OB_Y0,          // OB_Y0
    CAP_OB_X1,          // OB_X1
    CAP_OB_Y1,          // OB_Y1
    CAP_OB_THRES,       // OB_Thres
    CAP_OB_SUBSAMPLE,   // OB_SubSample
    CAP_OB_GAIN,        // OB_Gain
    OB_OFS_CAP,         // OB_Offset;
    
    Padding_Cap         // padding;
};
#if 0
SYNC_INFO still_mode_24fps = {
    ACT_STRX_Cap,       // act_start_x;
    ACT_STRY_Cap,       // act_start_y;
    SIE_CAP_XCENTER,    // act_center_x;
    SIE_CAP_YCENTER,    // act_center_y;
    IMAGESIZE_H_CAP,    // Act_SizeX;
    IMAGESIZE_V_CAP,    // Act_SizeY;
    
    // HDVD_Sync
    HD_Sync_Cap,        // HD_sync
    VD_Sync_Cap,        // VD_sync
    HD_Total_Cap,       // HD_total
    995,        // VD_total
    
    // ob_area
    CAP_OB_X0,          // OB_X0
    CAP_OB_Y0,          // OB_Y0
    CAP_OB_X1,          // OB_X1
    CAP_OB_Y1,          // OB_Y1
    CAP_OB_THRES,       // OB_Thres
    CAP_OB_SUBSAMPLE,   // OB_SubSample
    CAP_OB_GAIN,        // OB_Gain
    OB_OFS_CAP,         // OB_Offset;
    
    Padding_Cap         // padding;
};
#endif
CAPTURE_DATA_INFO H22_15fps_mode[] = {
    {
        1280,           // cap_width
        #if ((_SENSOR_SETTING == 1) || (_SENSOR_SETTING == 2)) //1280x720
        732,            // cap_height
        #elif ((_SENSOR_SETTING == 3) || (_SENSOR_SETTING == 4)) //1280x720
        720,            // cap_height
        #else //1280x800
        800,            // cap_height
        #endif
        CFAPAT_CAP,     // bayer_pattern
        PCLK_15FPS,     // pclk
        //&still_mode,  // cap_sync_info;
        //(SENSOR_CMD*)H22_REG_FULL,
        //sizeof(H22_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO H22_20fps_mode[] = {
    {
        1280,           // cap_width
        #if ((_SENSOR_SETTING == 1) || (_SENSOR_SETTING == 2)) //1280x720
        732,            // cap_height
        #elif ((_SENSOR_SETTING == 3) || (_SENSOR_SETTING == 4)) //1280x720
        720,            // cap_height
        #else //1280x800
        800,            // cap_height
        #endif
        CFAPAT_CAP,     // bayer_pattern
        PCLK_20FPS,     // pclk
        //&still_mode,  // cap_sync_info;
        //(SENSOR_CMD*)H22_REG_FULL,
        //sizeof(H22_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO H22_25fps_mode[] = {
    {
        1280,           // cap_width
        #if ((_SENSOR_SETTING == 1) || (_SENSOR_SETTING == 2)) //1280x720
        732,            // cap_height
        #elif ((_SENSOR_SETTING == 3) || (_SENSOR_SETTING == 4)) //1280x720
        720,            // cap_height
        #else //1280x800
        800,            // cap_height
        #endif
        CFAPAT_CAP,     // bayer_pattern
        PCLK_25FPS,     // pclk
        //&still_mode,  // cap_sync_info;
        //(SENSOR_CMD*)H22_REG_FULL,
        //sizeof(H22_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO H22_still_mode[] = {
    {
        1280,           // cap_width
        #if ((_SENSOR_SETTING == 1) || (_SENSOR_SETTING == 2)) //1280x736
        732,            // cap_height
        #elif ((_SENSOR_SETTING == 3) || (_SENSOR_SETTING == 4)) //1280x720
        720,            // cap_height
        #else //1280x800
        800,            // cap_height
        #endif
        CFAPAT_CAP,     // bayer_pattern
        PCLK_STILL,     // pclk
        //&still_mode,  // cap_sync_info;
        //(SENSOR_CMD*)H22_REG_FULL,
        //sizeof(H22_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void H22_GetRegInfo(H22_MODE mode,UINT32* addr,UINT32* size)
{
    switch( mode )
    {
        case H22_INIT:
            *addr = (UINT32)H22_INI_REG;
            *size = (sizeof(H22_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case H22_1920X1080:
            *addr = (UINT32)H22_INIT_REG_25FPS;
            *size = (sizeof(H22_INIT_REG_25FPS)/sizeof(SENSOR_CMD));
//          *addr = (UINT32)H22_INIT_REG_20FPS;
//          *size = (sizeof(H22_INIT_REG_20FPS)/sizeof(SENSOR_CMD));
//          *addr = (UINT32)H22_INIT_REG_15FPS;
//          *size = (sizeof(H22_INIT_REG_15FPS)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case H22_STILL:
        case H22_1440X1080:
            *addr = (UINT32)H22_REG_FULL;
            *size = (sizeof(H22_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void H22_GetSigInfo(H22_MODE mode,UINT32* addr,UINT32* size)
{
    switch( mode )
    {
        case H22_STILL:
        case H22_1440X1080:		
        case H22_1920X1080:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case H22_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void H22_GetDataInfo(H22_MODE mode,UINT32* addr,UINT32* size)
{
    switch( mode )
    {
        case H22_STILL:
        case H22_1440X1080:
            *addr = (UINT32)&H22_still_mode;
            //*size = (sizeof(H22_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case H22_1920X1080:
            *addr = (UINT32)&H22_25fps_mode;
//          *addr = (UINT32)&H22_20fps_mode;
//          *addr = (UINT32)&H22_15fps_mode;
            break;
        case H22_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void H22_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISOMin;
}

