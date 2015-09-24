#ifndef _MSDCNVTCALLBACK_H
#define _MSDCNVTCALLBACK_H

#ifndef _WIN32
#include "UMSDC.h"
#include "LcdTv.h"
#else
#include <wTypes.h>
typedef unsigned char UINT8, *PUINT8;
#endif

typedef struct _MSDCEXT_PARENT{
    UINT32 biSize;      //!< Structure Size
    UINT32 bHandled;    //!< Indicate Device Handle this function
    UINT32 bOK;         //!< Indicate Function Action is OK or Not
    UINT8  Reversed[2]; //!< Reversed Value for Structure DWORD Alignment
}tMSDCEXT_PARENT;

typedef struct _MSDCEXT_BOOL{
    tMSDCEXT_PARENT tParent;
    UINT32          bValue; //!< a BOOL value
}tMSDCEXT_BOOL;

typedef struct _MSDCEXT_UINT32{
    tMSDCEXT_PARENT tParent;
    UINT32          uiValue;//!< a UINT32 value
}tMSDCEXT_UINT32;

typedef struct _MSDCEXT_INT32{
    tMSDCEXT_PARENT tParent;
    INT32           iValue; //!< a INT32 value
}tMSDCEXT_INT32;
//------------------------------------------------------------------------------
// Photo Data Types for MSDCVendorNVT_AddCallback_Bi
//------------------------------------------------------------------------------
typedef struct _MSDCEXT_DISPLAY_SETTING{ //!< PC <- DSC
    tMSDCEXT_PARENT Parent;
    UINT32 AddrY[3];    //!< IDE 3 Buffers of Y
    UINT32 AddrU[3];    //!< IDE 3 Buffers of U
    UINT32 AddrV[3];    //!< IDE 3 Buffers of V
    UINT32 PitchY;      //!< IDE 3 Buffers of LineOffset Y
    UINT32 PitchUV;     //!< IDE 3 Buffers of LineOffset UV
    UINT32 Width;       //!< IDE 3 Buffers of Width
    UINT32 HeightY;     //!< IDE Height of Y
    UINT32 HeightUV;    //!< IDE Height of UV
}tMSDCEXT_DISPLAY_SETTING;

typedef struct _MSDCEXT_DISPLAY_CURRENT{ //!< PC <- DSC
    tMSDCEXT_PARENT Parent;
    UINT32 OPTBUF;      //!< Current IDE Buffer Index
    UINT32 BUFNUM;      //!< Total IDE Buffers
}tMSDCEXT_DISPLAY_CURRENT;

typedef struct _MSDCEXT_INFO_CAPTURE_JPG{ //!< PC <- DSC
    tMSDCEXT_PARENT Parent;
    UINT32  JpgAddr;    //!< Jpeg Data Address
    UINT32  JpgFileSize;//!< Jpg File Size
}tMSDCEXT_INFO_CAPTURE_JPG;

typedef struct _MSDCEXT_INFO_CAPTURE_RAW{ //!< PC <- DSC
    tMSDCEXT_PARENT Parent;
    UINT32  RawAddr;    //!< Memory Address of Raw Data
    UINT32  RawFileSize;//!< Raw Data Size
    UINT32  RawWidth;   //!< Raw Width
    UINT32  RawHeight;  //!< Raw Height
    UINT32  RawNbit;    //!< Raw 8bits or 12bits
}tMSDCEXT_INFO_CAPTURE_RAW;

//------------------------------------------------------------------------------
// Lcd Driver Implementation Data Types for MSDCVendorNVT_AddCallback_Bi
//------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32 //!< Visual C Used Data Type -begin
/////////////////////////////////////////////////////////////////////////////
#define PINMUX_LCD_SHIFT            0
#define PINMUX_LCD_CCIR656          (0x01 << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_CCIR601          (0x02 << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_SERIAL_RGB_8BIT  (0x04 << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_SERIAL_RGB_6BIT  (0x05 << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_SERL_YCBCR_8BIT  (0x06 << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_PARALLEL_24BIT   (0x08 << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_PARALLEL_18BIT   (0x09 << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_HDMI_16BIT       (0x0A << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_MEMIF            (0x0B << PINMUX_LCD_SHIFT)
#define PINMUX_LCD_BITMASK          (0x0F << PINMUX_LCD_SHIFT)
typedef enum
{
    LCD_MODE_RGB        = 0,            ///< LCD MODE is RGB
    LCD_MODE_RGBD       = 1,            ///< LCD MODE is RGB Dummy
    LCD_MODE_YUV        = 2,            ///< LCD MODE is YUV422 320X240
    LCD_MODE_RGBD_360   = 3,            ///< LCD MODE is RGB Dummy whit 360 * 240 resolution
    LCD_MODE_CCIR656    = 4,            ///< LCD MODE is CCIR656
    LCD_MODE_CCIR601    = 5,            ///< LCD MODE is CCIR601
    LCD_MODE_UPS051     = 6,            ///< LCD MODE is UPS051
    LCD_MODE_YUV_360    = 7,            ///< LCD MODE is YUV422 360X240
    LCD_MODE_MI_FMT0    = 8,            ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT1    = 9,            ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT2    =10,            ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT3    =11,            ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT4    =12,            ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT5    =13,            ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT6    =14,            ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT7    =15,            ///< LCD MODE is Memory(Parallel Interface) 9bits
    LCD_MODE_SERIAL_BI  =16,            ///< LCD MODE is Serial Interface bi-direction
    LCD_MODE_SERIAL_SEP =17,            ///< LCD MODE is Serial Interface separation
    LCD_MODE_RGB_THROUGH=18,
    LCD_MODE_HDMI_480P  =19,
    LCD_MODE_HDMI_576P  =20,
    LCD_MODE_HDMI_720P  =21
} LCD_MODE;
typedef enum {
    ICST_CCIR601    = 0,
    ICST_ITU_BT_709 = 1
} ICST_TYPE;
typedef enum {
    //DISPLAY_DEVICE_CASIO1G          = 0x0,
    DISPLAY_DEVICE_CASIO2G          = 0x1,
    DISPLAY_DEVICE_AU               = 0x2,
    DISPLAY_DEVICE_TOPPOLY          = 0x3,
    DISPLAY_DEVICE_CCIR656          = 0x4,
    DISPLAY_DEVICE_CCIR601          = 0x5,
    DISPLAY_DEVICE_TV               = 0x6,
    DISPLAY_DEVICE_HDMI_24BIT       = 0x7,
    DISPLAY_DEVICE_HDMI_16BIT       = 0x8,
    DISPALY_DEVICE_PARALLEL         = 0x9,
    DISPLAY_DEVICE_CCIR656_16BIT    = 0xA,
    DISPLAY_DEVICE_CCIR601_16BIT    = 0xB,
    DISPLAY_DEVICE_MI               = 0x0C
} IDE_DEVICE_TYPE;
typedef struct {
    UINT32  uiWidth;        ///< Display buffer width
    UINT32  uiHeight;       ///< Display buffer height
    UINT32  uiWinWidth;     ///< Display window width
    UINT32  uiWinHeight;    ///< Display window height
} LCDSIZE, *PLCDSIZE;
/////////////////////////////////////////////////////////////////////////////
#endif //!< Visual C Used Data Type -end
/////////////////////////////////////////////////////////////////////////////

typedef struct _PANEL_CMD{
    UINT8 Address;
    UINT8 Value;
}tPANEL_CMD;

typedef struct _PANEL_PARAM{
    UINT32      LCDMode; //!< cast to LCD_MODE
    float       DCLK;
    UINT32      HSyncTotalPeriod;
    UINT32      HSyncActivePeriod;
    UINT32      HSyncBackPorch;
    UINT32      VSyncTotalPeriod;
    UINT32      VSyncActivePeriod;
    UINT32      VSyncBackPorchOdd;
    UINT32      VSyncBackPorchEven;
    UINT32      BufferWidth;
    UINT32      BufferHeight;
    UINT32      WindowWidth;
    UINT32      WindowHeight;
    BOOL        bYCbCrFormat; //!< TRUE(YUV): YUV/UPS051/UPS052, FALSE(RGB): RGB/RGBD
}tPANEL_PARAM;

typedef struct _IDE_PARAM{
    UINT            pinmux_select_lcd;
    UINT32          icst;   //!< cast to ICST_TYPE
    BOOL            dithering[2];
    UINT32          device; //!< cast to IDE_DEVICE_TYPE
    BOOL            pdir;
    UINT32          odd;
    UINT32          even;
    BOOL            hsinv;
    BOOL            vsinv;
    BOOL            hvldinv;
    BOOL            vvldinv;
    BOOL            clkinv;
    BOOL            fieldinv;
    BOOL            rgbdummy;
    BOOL            interlace;
    BOOL            startfield;
    UINT32          ctrst;
    UINT32          brt;
    UINT32          cmults;
    BOOL            cex;
    BOOL            standby_signal;
    BOOL            clamp;
    BOOL            tv_powerdown;
    UINT32          sif_startend[2];
}tIDE_PARAM;

typedef struct _LCD_PARAM{
    tPANEL_PARAM Panel;
    tIDE_PARAM   IDE;
    tPANEL_CMD*  pCmdQueue;
    UINT32       nCmd;
}tLCD_PARAM;

typedef struct _LCD_DEFINE_VALUE{
    UINT32          _SETTING_SIF_MODE;                   //SIF MODE: SIF_MODE_0000 ~ SIF_MODE_1111
    UINT32          _SETTING_PANEL_REG_BIT_SHIFT_ADDRESS;//D? is Address LSB
    UINT32          _SETTING_PANEL_REG_BIT_SHIFT_VALUE;  //D? is Content LSB
    UINT32          _SETTING_PANEL_REG_BIT_WRITE;        //is Bit of Write Command
    UINT32          _SETTING_PANEL_REG_MODE_WRITE;       //BIT High(1) or LOW(0) is Write Mode
    UINT32          _SETTING_PANEL_REG_MODE_READ;        //BIT High(1) or LOW(0) is Read  Mode
}tLCD_DEFINE_VALUE;

typedef struct _MSDCEXT_LCD_DRV_OPEN{
    tMSDCEXT_PARENT     tParent;
    //[Host Set]
    tLCD_DEFINE_VALUE   tDefValues;     //!< for Real Driver Define
    tLCD_PARAM*         pParam;         //!< Host Fill Memory into Pool Addr and Call it to Run DeviceOpen
    //[Device Set]
    LCDSIZE             tLCDSize;       //!< LCD Size
}tMSDCEXT_LCD_DRV_OPEN;

typedef struct _MSDCEXT_LCD_DRV_CLOSE{
    tMSDCEXT_PARENT tParent;
    tPANEL_CMD*     pCmdQueue;
    UINT32          nCmd;
}tMSDCEXT_LCD_DRV_CLOSE;

typedef struct _MSDCEXT_LCD_DRV_MEM_POOL_GET{
    tMSDCEXT_PARENT tParent;
    //[Device Set]
    UINT32          PoolAddr;
    UINT32          PoolSize;
}tMSDCEXT_LCD_DRV_MEM_POOL_GET;

typedef struct _MSDCEXT_LCD_DRV_SHOW_IMG{
    tMSDCEXT_PARENT tParent;
    //[Host Set]
    UINT32          AddrY;
    UINT32          AddrU;
    UINT32          AddrV;
    UINT32          PitchY;
    UINT32          PitchUV;
    UINT32          Width;
    UINT32          Height;
}tMSDCEXT_LCD_DRV_SHOW_IMG;

typedef struct _MSDCEXT_LCD_DRV_REG_WRITE{
    tMSDCEXT_PARENT tParent;
    //[Host Set]
    UINT32          bUseLcdApi; //!< Indication via LCD_RegWrite
    tPANEL_CMD      tCmd;       //!< One Command
}tMSDCEXT_LCD_DRV_REG_WRITE;

//------------------------------------------------------------------------------
// Display Utility Implementation Data Types for MSDCVendorNVT_AddCallback_Bi
//------------------------------------------------------------------------------
typedef enum _MSDCEXT_DISP_FORMAT{
    MSDCEXT_DISP_FORMAT_4_BIT = 0,  // Color format 4 bit
    MSDCEXT_DISP_FORMAT_2_BIT,      // Color format 2 bit
    MSDCEXT_DISP_FORMAT_1_BIT,      // Color format 1 bit
    MSDCEXT_DISP_FORMAT_8_BIT,      // Color format 8 bit
    MSDCEXT_DISP_FORMAT_YCBCR444,   // Color format YCBCR444
    MSDCEXT_DISP_FORMAT_YCBCR422,   // Color format YCBCR422
    MSDCEXT_DISP_FORMAT_YCBCR420,   // Color format YCBCR420
    MSDCEXT_DISP_FORMAT_ARGB4565,   // Color format ARGB4565
    MSDCEXT_DISP_FORMAT_ARGB8565,   // Color format ARGB8565
    MSDCEXT_DISP_FORMAT_YCBCR422_UVPACK, // Color format YCBCR422 with UV packing
    MSDCEXT_DISP_FORMAT_YCBCR420_UVPACK, // Color format YCBCR420 with UV packing
    MSDCEXT_DISP_FORMAT_UNKNOWN = -1 // Unknown Format
}eMSDCEXT_DISP_FORMAT;

typedef enum _MSDCEXT_DISP_ENG{
    MSDCEXT_DISP_ENG_VDO_0 = 0,
    MSDCEXT_DISP_ENG_VDO_1,
    MSDCEXT_DISP_ENG_OSD_0,
    MSDCEXT_DISP_ENG_OSD_1
}eMSDCEXT_DISP_ENG;

typedef struct _MSDCEXT_DISP_VDO_INFO{
    BOOL                    bEnable;
    UINT32                  Format;     //!< cast to eMSDCEXT_DISP_FORMAT
    UINT32                  AddrY[3];   //AddrY[0] is RGB565 when format is ARGB565
    UINT32                  AddrU[3];   //AddrU[0] is Alpha Plane when format is ARGB565
    UINT32                  AddrV[3];
    UINT32                  Width;
    UINT32                  Height;
    UINT32                  PitchY;
    UINT32                  PitchUV;
}tMSDCEXT_DISP_VDO_INFO;

typedef struct _MSDCEXT_DISP_OSD_INFO{
    BOOL                    bEnable;
    UINT32                  Format;     //!< cast to eMSDCEXT_DISP_FORMAT
    UINT32                  AddrBuf;
    UINT32                  AddrAlpha;  //!< ARGB565's Alpha
    UINT32                  Width;
    UINT32                  Height;
    UINT32                  PitchBuf;
    UINT32                  PitchAlpha;
}tMSDCEXT_DISP_OSD_INFO;

typedef struct _MSDCEXT_DISP_CFG{
    tMSDCEXT_PARENT         tParent;
    //[Device Set]
    tMSDCEXT_DISP_VDO_INFO  tInfoVdo[2];
    tMSDCEXT_DISP_OSD_INFO  tInfoOsd[2];
}tMSDCEXT_DISP_CFG;

typedef struct _MSDCEXT_DISP_VDO_CURR_BUFF_IDX{
    tMSDCEXT_PARENT         tParent;
    //[Device Set]
    UINT32                  iSel[2]; //!< 0:Video1, 1:Video2
}tMSDCEXT_DISP_VDO_CURR_BUFF_IDX;

typedef struct _MSDCEXT_DISP_ENG_ONOFF{
    tMSDCEXT_PARENT         tParent;
    //[Host Set]
    UINT32                  eEngine; //!< cast to eMSDCEXT_DISP_ENG
    BOOL                    bEnable;
}tMSDCEXT_DISP_ENG_ONOFF;

typedef struct _MSDCEXT_DISP_OSD_PALETTE{
    tMSDCEXT_PARENT         tParent;
    //[Host Set or Device Set]
    UINT32                  iSel;         //!< 0 or 1 for Palette Select
    UINT32                  nEntrys;      //!< Total effective enterys
    UINT32                  Palette[256]; //!< if is 16 color only use first 16 entrys
}tMSDCEXT_DISP_OSD_PALETTE;

typedef struct _MSDCEXT_DISP_GAMMA_TBL{
    tMSDCEXT_PARENT         tParent;
    //[Host Set or Device Set]
    UINT8                   GammaTbl[256];//!< Gamma Table, Max size is 256 Level for any chip
}tMSDCEXT_DISP_GAMMA_TBL;

typedef struct _MSDCEXT_DISP_ICST_COEF_INFO{
    tMSDCEXT_PARENT         tParent;
    //[Device Set]
    UINT32                  NumBitsInteger; //!< Numbers of Maxrix Coefficient Integer Bits
    UINT32                  NumBitsFraction;//!< Numbers of Maxrix Coefficient Fraction Bits
    INT32                   PreMatrix[9];   //!< CoefTrans = PreMatrix*GainMatrix*PostMatrix
    INT32                   PostMatrix[9];  //!< CoefTrans = PreMatrix*GainMatrix*PostMatrix
}tMSDCEXT_DISP_ICST_COEF_INFO;

typedef struct _MSDCEXT_DISP_ICST_COEF{
    tMSDCEXT_PARENT         tParent;
    //[Host Set]
    UINT32                  CoefTrans[9]; //!< 3x3 Transform Matrix
}tMSDCEXT_DISP_ICST_COEF;

typedef struct _MSDCEXT_DISP_SATURATION_INFO{
    tMSDCEXT_PARENT         tParent;
    //[Device Set]
    INT32                   ValMax;
    INT32                   ValMin;
    INT32                   ValDefault;
}tMSDCEXT_DISP_SATURATION_INFO;

typedef struct _MSDCEXT_DISP_CONTRAST_INFO{
    tMSDCEXT_PARENT         tParent;
    //[Device Set]
    INT32                   ValMax;
    INT32                   ValMin;
    INT32                   ValDefault;
}tMSDCEXT_DISP_CONTRAST_INFO;

typedef struct _MSDCEXT_DISP_BRIGHTNESS_INFO{
    tMSDCEXT_PARENT         tParent;
    //[Device Set]
    INT32                   ValMax;
    INT32                   ValMin;
    INT32                   ValDefault;
}tMSDCEXT_DISP_BRIGHTNESS_INFO;

//------------------------------------------------------------------------------
// Filesys Utility Implementation Data Types for MSDCVendorNVT_AddCallback_Bi
//------------------------------------------------------------------------------
typedef struct _MSDCEXT_FILE_WORKING_MEM{
    tMSDCEXT_PARENT         tParent;
    //[Device Set]
    UINT32                  Address;
    UINT32                  Size;
}tMSDCEXT_FILE_WORKING_MEM;

typedef struct _MSDCEXT_FILE_INFO{
    tMSDCEXT_PARENT         tParent;
    //[Host Set(Copy PC To Device) or Device Set(Copy Device To PC)]
    char                    FilePath[256]; //!< File Name / Path
    UINT32                  FileSize;      //!< File Total Size
    UINT32                  TransmitOffset;//!< File Transmit Offset
    UINT32                  TransmitSize;  //!< File Transmit Size
    UINT32                  WorkingMem;    //!< Device Wroking Buffer
}tMSDCEXT_FILE_INFO;

//---------------------------------------------------------------------------------
// Universal Adjustment Implementation Data Types for MSDCVendorNVT_AddCallback_Bi
//---------------------------------------------------------------------------------
typedef struct _MSDCEXT_ADJ_ITEM_DESC{
    UINT32 bIsSign;
    UINT32 MaxValue;
    UINT32 MinValue;
    CHAR   Tag[64];
}tMSDCEXT_ADJ_ITEM_DESC;

//tMSDCEXT_ADJ_ITEM_DESC* with following data
typedef struct _MSDCEXT_ADJ_ALL_DESC{
    tMSDCEXT_PARENT         tParent;
}tMSDCEXT_ADJ_ALL_DESC;

//void*  pVars; with following data
typedef struct _MSDCEXT_ADJ_DATA{
    tMSDCEXT_PARENT         tParent;
}tMSDCEXT_ADJ_DATA;
#endif
