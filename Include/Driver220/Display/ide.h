/**
    Header file for Display library

    Header file for Display library.

    @file       ide.h
    @ingroup    mIDISPIDE
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IDE_H
#define _IDE_H

#include "Type.h"

/**
    @addtogroup mIDISPIDE
*/
//@{

/**
    @name Display plane (IDE video and OSD) enable.
*/
//@{
#define DISPLAY_OSD1_EN                 0x02
#define DISPLAY_OSD2_EN                 0x04
#define DISPLAY_VIDEO1_EN               0x08
#define DISPLAY_VIDEO2_EN               0x10
//@}

/**
    Display device type.
*/
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


/**
    Video buffer operation mode
*/
typedef enum {
    IDE_VIDEO_BJMODE_CANT_CROSS_WRITE =   0,   ///< Automatically change buffer according to IME signal
    IDE_VIDEO_BJMODE_RW_INDEPENDENT =     1,   ///< Automatically change buffer according to IDE's VSync signal
    IDE_VIDEO_BJMODE_BUFFER_REPEAT =      2    ///< Repeat the same buffer until user change buffer
} IDE_BJMODE;

/**
    Total video buffer number
*/
typedef enum {
    IDE_VIDEO_BUFFER_NUM_3 =              0,   ///< Total 3 buffers (Buffer 0, 1, 2)
    IDE_VIDEO_BUFFER_NUM_2 =              1,   ///< Total 2 buffers (Buffer 0, 1)
    IDE_VIDEO_BUFFER_NUM_1 =              2    ///< Only 1 buffer (Buffer 0)
} IDE_BUF_NUM;

/**
    Video buffer in operation
*/
typedef enum {
    IDE_VIDEO_BUFFER_OPT_0 =             0,   ///< Buffer 0 in operation
    IDE_VIDEO_BUFFER_OPT_1 =             1,   ///< Buffer 1 in operation
    IDE_VIDEO_BUFFER_OPT_2 =             2    ///< Buffer 2 in operation
} IDE_OP_BUF;

/**
    Video/OSD buffer horizontal read mode
*/
typedef enum {
    IDE_BUFFER_READ_L2R = 0,                  ///< Read from left to right
    IDE_BUFFER_READ_R2L = 1                   ///< Read from right to left
} IDE_HOR_READ;

/**
    Video/OSD buffer vertical read mode
*/
typedef enum {
    IDE_BUFFER_READ_T2B = 0,                  ///< Read from top to bottom
    IDE_BUFFER_READ_B2T = 1                   ///< Read from bottom to top
} IDE_VER_READ;

/**
    @name Video buffer select for video buffer adddress setting
*/
//@{
#define IDE_VIDEO_ADDR_SEL_ALL              (0x00)
#define IDE_VIDEO_ADDR_SEL_BUFFER0          (0x01 << 0)
#define IDE_VIDEO_ADDR_SEL_BUFFER1          (0x01 << 1)
#define IDE_VIDEO_ADDR_SEL_BUFFER2          (0x01 << 2)
//@}

/**
    Video window ID select.
*/
typedef enum
{
    IDE_VIDEOID_1 = 1,
    IDE_VIDEOID_2 = 2
} IDE_VIDEOID;

/**
    OSD window ID select.
*/
typedef enum
{
    IDE_OSDID_1 = 1,    ///< OSD plane 1
    IDE_OSDID_2 = 2,    ///< OSD plane 2, some chip revision may not have second OSD plane
    IDE_OSDID_3 = 3,    ///< OSD plane 3, might same as IDE_VIDEOID_1
    IDE_OSDID_4 = 4     ///< OSD plane 4, might same as IDE_VIDEOID_2
} IDE_OSDID;

/**
    Video blending with Video Key operation select.

    Only applied to 96432/96200/96220. Video1/Video2 blends with VideoKey definitions.
    This is for argument of ide_set_video_blend() and return value of
    ide_get_video_blend();
*/
typedef enum {
    IDE_VIDEO_BLEND_VIDEOCK      = 0x00,    ///< VideoCK
    IDE_VIDEO_BLEND_VIDEOCK7_8   = 0x01,    ///< VideoCK * (7/8) + Video * (1/8)
    IDE_VIDEO_BLEND_VIDEOCK3_4   = 0x02,    ///< VideoCK * (3/4) + Video * (1/4)
    IDE_VIDEO_BLEND_VIDEOCK1_2   = 0x03,    ///< VideoCK * (1/2) + Video * (1/2)
    IDE_VIDEO_BLEND_VIDEOCK1_4   = 0x04,    ///< VideoCK * (1/4) + Video * (3/4)
    IDE_VIDEO_BLEND_VIDEOCK1_8   = 0x05,    ///< VideoCK * (1/8) + Video * (7/8)
    IDE_VIDEO_BLEND_VIDEO1OR2    = 0x06     ///< Video1 or Video2
} IDE_VIDEO_BLEND_OP;

#if 1
/**
    Video1 blending with Video2 operation select.
    
    Only applied to 96610/96611. Video1 blends with Video2 definitions.
*/
typedef enum {
    IDE_VIDEO_BLEND_VIDEO2       = 0x00,    ///< Video2
    IDE_VIDEO_BLEND_VIDEO2_7_8   = 0x01,    ///< Video2 * (7/8) + Video1 * (1/8)
    IDE_VIDEO_BLEND_VIDEO2_3_4   = 0x02,    ///< Video2 * (3/4) + Video1 * (1/4)
    IDE_VIDEO_BLEND_VIDEO2_1_2   = 0x03,    ///< Video2 * (1/2) + Video1 * (1/2)
    IDE_VIDEO_BLEND_VIDEO2_1_4   = 0x04,    ///< Video2 * (1/4) + Video1 * (3/4)
    IDE_VIDEO_BLEND_VIDEO2_1_8   = 0x05,    ///< Video2 * (1/8) + Video1 * (7/8)
    IDE_VIDEO_BLEND_VIDEO1       = 0x06     ///< Video1
} IDE_V1_V2_BLEND_OP;
#endif

/**
    Video color key select.
    
    Only applied to 96432/96613/96200. This is for argument of ide_set_video_colorkey_op() and
    return value of ide_get_video_colorkey_op()
*/
typedef enum {
    IDE_VIDEO_COLORKEY_VIDEO1OR2 = 0x00,    ///< videock = video1 or video2;
    IDE_VIDEO_COLORKEY_YSMALLKEY = 0x01,    ///< videock = (video_Y < VDO_YKEY) ?
    IDE_VIDEO_COLORKEY_YEQUKEY   = 0x02,    ///< videock = (video_Y == VDO_KEY && video_CB == VDO_CBKEY && video_CR == VDO_CRKEY) ?
    IDE_VIDEO_COLORKEY_YBIGKEY   = 0x03     ///< videock = (video_Y > VDO_YKEY) ?
} IDE_VIDEO_COLORKEY_OP;

/**
    Video color key compare source select.
    
    Only applied to 96432/96613/96200/96220. This is for argument of ide_set_video_colorkey_sel() and
    return value of ide_get_video_colorkey_sel()
*/
typedef enum {
    IDE_VIDEO_COLORKEY_COMPAREVIDEO2 = 0x00, ///< Colorkey will compare with video2
    IDE_VIDEO_COLORKEY_COMPAREVIDEO1 = 0x01  ///< Colorkey will compare with video1
} IDE_VIDEO_COLORKEY_SEL;

/**
    OSD blending operation select.
*/
typedef enum {
    IDE_OSD_BLEND_OSD2           = 0x00,    ///< OSD2 only
    IDE_OSD_BLEND_OSD2_7_8       = 0x01,    ///< OSD2 * (7/8) + OSD1 * (1/8)
    IDE_OSD_BLEND_OSD2_3_4       = 0x02,    ///< OSD2 * (3/4) + OSD1 * (1/4)
    IDE_OSD_BLEND_OSD2_1_2       = 0x03,    ///< OSD2 * (1/2) + OSD1 * (1/2)
    IDE_OSD_BLEND_OSD2_1_4       = 0x04,    ///< OSD2 * (1/4) + OSD1 * (3/4)
    IDE_OSD_BLEND_OSD2_1_8       = 0x05,    ///< OSD2 * (1/8) + OSD1 * (7/8)
    IDE_OSD_BLEND_OSD1           = 0x06     ///< OSD1 only
} IDE_OSD_BLEND_OP;

/**
    OSD palette set select.
*/
typedef enum {
    IDE_PALETTE_LOW256           = 0x00,    ///< IDE Palette will use lower 256
    IDE_PALETTE_HIGH256          = 0x01     ///< IDE palette will use higher 256
} IDE_PALETTE_SEL;

/**
    OSD & Video blending operation select.
*/
typedef enum {
    IDE_OV_BLEND_VIDEO          = 0x00,     ///< Video only
    IDE_OV_BLEND_VIDEO_7_8      = 0x01,     ///< Video * (7/8) + OSD * (1/8)
    IDE_OV_BLEND_VIDEO_3_4      = 0x02,     ///< Video * (3/4) + OSD * (1/4)
    IDE_OV_BLEND_VIDEO_1_2      = 0x03,     ///< Video * (1/2) + OSD * (1/2)
    IDE_OV_BLEND_VIDEO_1_4      = 0x04,     ///< Video * (1/4) + OSD * (3/4)
    IDE_OV_BLEND_VIDEO_1_8      = 0x05,     ///< Video * (1/8) + OSD * (7/8)
    IDE_OV_BLEND_OSD            = 0x06,     ///< OSD only
    IDE_OV_BLEND_VIDEO_AND_OSD  = 0x07,     ///< Video & OSD
    IDE_OV_BLEND_VIDEO_OR_OSD   = 0x08,     ///< Video | OSD
    IDE_OV_BLEND_VIDEO_XOR_OSD  = 0x09,     ///< Video ^ OSD
    IDE_OV_BLEND_NOT_VIDEO      = 0x0A      ///< !Video
} IDE_OV_BLEND_OP;

/**
    Display ICST output type select.
*/
typedef enum {
    ICST_CCIR601    = 0,
    ICST_ITU_BT_709 = 1
} ICST_TYPE;

//#NT#2011/07/15#Shirley Cheng -begin
/**
    Display YCC output Range select.
*/
typedef enum {
    YCC_CLAMP_1 = 0,    ///< Output range YCC: 1~254
    YCC_CLAMP_16 = 1    ///< Output range Y: 16~235, CC :16~240
} CLAMP_TYPE;
//#NT#2011/07/15#Shirley Cheng -end

/**
    CST type select.
*/
typedef enum {
    CST_RGB2YCBCR = 0,
    CST_YCBCR2RGB = 1
} CST_SEL;

/**
    CST accuracy select.
*/
typedef enum {
    ACCRCY_1_2_8 = 0,
    ACCRCY_1_3_7 = 1
} ACCRCY_SEL;

/**
    IDE frame buffer pixel format.
*/
typedef enum {
    COLOR_4_BIT             = 0,    ///< Color format 4 bit
    COLOR_2_BIT             = 1,    ///< Color format 2 bit
    COLOR_1_BIT             = 2,    ///< Color format 1 bit
    COLOR_8_BIT             = 3,    ///< Color format 8 bit
    COLOR_YCBCR444          = 4,    ///< Color format YCBCR444
    COLOR_YCBCR422          = 5,    ///< Color format YCBCR422
    COLOR_YCBCR420          = 6,    ///< Color format YCBCR420
    COLOR_ARGB4565          = 7,    ///< Color format ARGB4565
    COLOR_ARGB8565          = 8,    ///< Color format ARGB8565
    COLOR_RGB888            = 9     ///< Color format RGB888 packed
} IDE_COLOR_FORMAT;

/**
    Determine which buffer is selected.
*/
typedef enum {
    IDE_VIDEO_BUFFERID_0,       ///< Buffer0 will be selected
    IDE_VIDEO_BUFFERID_1,       ///< Buffer1 will be selected
    IDE_VIDEO_BUFFERID_2,       ///< Buffer2 will be selected
    IDE_VIDEO_BUFFERID_CURRENT  ///< Current Operating buffer will be selected
} IDE_BUFFERID;

/**
    IDE LCD pixel direction.
*/
typedef enum {
    IDE_PDIR_RGB = 0,
    IDE_PDIR_RBG = 1
} IDE_PDIR;

/**
    IDE RGB format begin color.
*/
typedef enum {
    IDE_LCD_R = 0,
    IDE_LCD_G = 1,
    IDE_LCD_B = 2
} IDE_PORDER;

/**
    IDE dithering valid bits.
*/
typedef enum {
    IDE_DITHER_4BITS = 0,
    IDE_DITHER_5BITS = 1,
    IDE_DITHER_6BITS = 2,
    IDE_DITHER_7BITS = 3
} IDE_DITHER_VBITS;

/**
    IDE output component selection.
*/
typedef enum {
    IDE_COMPONENT_R = 0,
    IDE_COMPONENT_G = 1,
    IDE_COMPONENT_B = 2,

    IDE_COMPONENT_Y = 0,
    IDE_COMPONENT_CB = 1,
    IDE_COMPONENT_CR = 2
} IDE_OUT_COMP;

/**
    TV mode.
*/
typedef enum {
    IDE_TV_NTSC_M = 0,
    IDE_TV_PAL_BDGHI = 1,
    IDE_TV_PAL_M = 2,
    IDE_TV_PAL_NC = 3
} IDE_TV_MODE;

/**
    Defines IDE palette capability.
*/
typedef enum {
    IDE_PAL_CAP_16X2 = 0,
    IDE_PAL_CAP_256X1 = 1,
    IDE_PAL_CAP_256X2 = 2
} IDE_PAL_CAP;

/**
    Defines IDE alpha blending layer.
*/
typedef enum {
    IDE_BLEND_V2 = 0,
    IDE_BLEND_O1 = 1,
    IDE_BLEND_O2 = 2
} IDE_BLEND_LAYER;

/**
    Defines IDE alpha blending type.
*/
typedef enum {
    IDE_NO_ALPHA = 0,
    IDE_GLOBAL_ALPHA = 1,
    IDE_SOURCE_ALPHA = 2
} IDE_ALPHA_TYPE;

//#NT#2011/07/05#Shirley Cheng -begin
/**
    Defines IDE V1 Y channel burst lenght
*/
typedef enum {
    IDE_VIDEO1_Y_BURST_16 = 0,
    IDE_VIDEO1_Y_BURST_24 = 1,
    IDE_VIDEO1_Y_BURST_32 = 2,
    IDE_VIDEO1_Y_BURST_48 = 3
} IDE_V1_Y_BURST;

/**
    Defines IDE V1 C channel burst lenght
*/
typedef enum {
    IDE_VIDEO1_C_BURST_16 = 0,
    IDE_VIDEO1_C_BURST_24 = 1
} IDE_V1_C_BURST;

/**
    Defines IDE OSD1 channel burst lenght
*/
typedef enum {
    IDE_OSD1_BURST_16 = 0,
    IDE_OSD1_BURST_24 = 1,
    IDE_OSD1_BURST_32 = 2
} IDE_O1_BURST;
//#NT#2011/07/05#Shirley Cheng -end

/**
    Defines TV DAC Calibration resistor.
*/
typedef enum {
    TV_DAC_6Ref = 0,      ///< 6 R(unit)
    TV_DAC_7Ref = 1,      ///< 6+1 R(unit)
    TV_DAC_8Ref = 2,      ///< 6+2 R(unit)
    TV_DAC_5Ref = 3       ///< 6-1 R(unit)
} TV_DAC_SEL;

/**
    Defines TV DAC Calibation reference current cell.
*/
typedef enum {
    TV_DAC_ISEL_10 = 0,    ///< Select 10 reference current cell
    TV_DAC_ISEL_9 = 1      ///< Select 9 reference current cell, Output Voltage X 1.1
} TV_DAC_ISEL;

/**
    Display buffer and window size.
*/
typedef struct {
    UINT32  uiWidth;        ///< Display buffer width
    UINT32  uiHeight;       ///< Display buffer height
    UINT32  uiWinWidth;     ///< Display window width
    UINT32  uiWinHeight;    ///< Display window height
} LCDSIZE, *PLCDSIZE;

/**
    Video buffer address.
*/
typedef struct {
    UINT32   B0_Y;          ///<Buffer0 Y starting address
    UINT32   B0_CB;         ///<Buffer0 Cb starting address
    UINT32   B0_CR;         ///<Buffer0 Cr starting address
    UINT32   B1_Y;          ///<Buffer1 Y starting address
    UINT32   B1_CB;         ///<Buffer1 Cb starting address
    UINT32   B1_CR;         ///<Buffer1 Cr starting address
    UINT32   B2_Y;          ///<Buffer2 Y starting address
    UINT32   B2_CB;         ///<Buffer2 Cb starting address
    UINT32   B2_CR;         ///<Buffer2 Cr starting address
    UINT8    uiBufSel;      ///<individual buffer number selection. It must be the logic OR operation of IDE_VIDEO_ADDR_SEL_BUFFER0, IDE_VIDEO_ADDR_SEL_BUFFER1 and IDE_VIDEO_ADDR_SEL_BUFFER2
} VIDEO_BUF_ADDR;

/**
    Video buffer dimension and attribute.
*/
typedef struct {
    UINT32   Buf_W;             ///<Buffer width
    UINT32   Buf_H;             ///<Buffer height
    UINT32   Buf_Lineoffset;    ///<Buffer Y line offset.
                                ///<In YCBCR 444 format, CB/CR lineoffset = Y, Y lineoffset must be word alignment.
                                ///<In YCBCR 422/420 format, CB/CR lineoffset = Y/2, Y lineoffset must be  2word alignment.
    BOOL     Buf_L2R;           ///<Read direction, IDE_BUFFER_READ_L2R or IDE_BUFFER_READ_R2L
    BOOL     Buf_T2B;           ///<Read direction, IDE_BUFFER_READ_T2B or IDE_BUFFER_READ_B2T
    UINT32   Buf_BJmode;        ///<Buffer operation mode, refer to IDE_VIDEO_BJMODE_XX definition
    UINT32   Buf_Opt;           ///<Buffer in operation, IDE_VIDEO_BUFFER_OPT_0 ~ 2
    UINT32   Buf_Num;           ///<Total buffer number, refer to IDE_VIDEO_BUFFER_NUM_X definition
} VIDEO_BUF_ATTR;

/**
    OSD buffer dimension and attribute.
*/
typedef struct {
    UINT32   Buf_W;             ///<Buffer width
    UINT32   Buf_H;             ///<Buffer height
    UINT32   Buf_Lineoffset;    ///<Buffer line offset (word unit, word alignment).
    BOOL     Buf_L2R;           ///<Read direction, IDE_BUFFER_READ_L2R or IDE_BUFFER_READ_R2L
    BOOL     Buf_T2B;           ///<Read direction, IDE_BUFFER_READ_T2B or IDE_BUFFER_READ_B2T
} OSD_BUF_ATTR;

/**
    YCbCr color pixel components.
*/
typedef struct {
    UINT8    ColorY;            ///<Y Color Value
    UINT8    ColorCB;           ///<CB Color Value
    UINT8    ColorCR;           ///<CR Color Value
} YUVCOLOR, *PYUVCOLOR;

/**
    RGB color pixel components.
*/
typedef struct {
    UINT8    ColorR;            ///<R Color Value
    UINT8    ColorG;            ///<G Color Value
    UINT8    ColorB;            ///<B Color Value
} RGBCOLOR, *PRGBCOLOR;

/**
    OSD display dimension and format.
*/
typedef struct {
    UINT32   Source_W;          ///<Buffer width
    UINT32   Source_H;          ///<Buffer height
    UINT32   Des_W;             ///<Window width
    UINT32   Des_H;             ///<Window height
    UINT32   Win_Format;        ///<Window format, refer to IDE_COLOR_FORMAT type
    UINT32   Win_X;             ///<Window start position X
    UINT32   Win_Y;             ///<Window start position Y
    UINT32   High_Addr;         ///<Select palette
} VOSD_WINDOW_ATTR;

/**
    OSD palette entry definition.
*/
typedef struct {
    UINT32   Entry;
    YUVCOLOR OSDColor;
    UINT8    Blend_OP;
    UINT8    Blink;
} PALETTE_ENTRY, *PPALETTE_ENTRY;

/**
    IDE Interrupt Call Back Function
*/
typedef void (*FPIDEInterruptCallBack)(void);

//-------------------------------------------------
// IDE register programm APIs
//-------------------------------------------------

// IDE control
void ide_setDithering(BOOL b_en, BOOL b_freerun);
void ide_setDevice(IDE_DEVICE_TYPE uidevice);
void ide_setPdir(IDE_PDIR b_pdir);
void ide_setOdd(IDE_PORDER uiODD);
void ide_setEven(IDE_PORDER uiEVEN);
void ide_setHsInv(BOOL b_inv);
void ide_setVsInv(BOOL b_inv);
void ide_setHvldInv(BOOL b_inv);
void ide_setVvldInv(BOOL b_inv);
void ide_setClkInv(BOOL b_inv);
void ide_setFldInv(BOOL b_inv);
void ide_setDeInv(BOOL b_inv);
void ide_setRgbd(BOOL b_en);
void ide_setThrough(BOOL b_en);
void ide_setHdmiDdr(BOOL b_en);
void ide_setReset(void);
void ide_setEn(BOOL b_en);
void ide_setLoad(void);
void ide_setDevNo(BOOL b_no);
//void ide_setHLPFEn(BOOL b_en);
//BOOL ide_getHLPFEn(void);

BOOL ide_getEn(void);
BOOL ide_getLoad(void);
BOOL ide_getThrough(void);
BOOL ide_getRgbd(void);
IDE_DEVICE_TYPE ide_getDevice(void);

UINT32 ide_getWindowEn(void);
void ide_setAllWindowDis(void);
void ide_setAllWindowEn(UINT32 uiWins);

// Scaling control
//void ide_setDimCvert(void);
//void ide_setCvertFactor(UINT32 uiHFT, UINT32 uiVFT);

// Palette Control
void ide_setPalEntry(UINT32 uiEntry, UINT8 uiColorY, UINT8 uiColorCB, UINT8 uiColorCR, UINT8 uiAlpha);
void ide_getPalEntry(UINT32 uiEntry, UINT8 *uiColorY, UINT8 *uiColorCB, UINT8 *uiColorCR, UINT8 *uiAlpha);
void ide_setPal(UINT32 uiEntry, UINT32 uiPalette);
void ide_getPal(UINT32 uiEntry, UINT32 *uiPalette);

// Misc
void ide_setBackground(UINT8 uiColorY, UINT8 uiColorCB, UINT8 uiColorCR);


// Timing
void ide_setHorTiming(UINT32 uiHsynct, UINT32 uiHtotal, UINT32 uiHvalidst, UINT32 uiHvalided);
void ide_setVerTiming(UINT32 uiVsynct, UINT32 uiVtotal, UINT32 uiODDVvalidst, UINT32 uiODDVvalided, UINT32 uiEVENVvalidst, UINT32 uiEVNEVvalided);
void ide_getHorTiming(UINT32 *puiHsynct, UINT32 *puiHtotal, UINT32 *puiHvalidst, UINT32 *puiHvalided);
void ide_getVerTiming(UINT32 *puiVsynct, UINT32 *puiVtotal, UINT32 *puiODDVvalidst, UINT32 *puiODDVvalided, UINT32 *puiEVENVvalidst, UINT32 *puiEVNEVvalided);

void ide_setSyncDelay(UINT8 uiHsDealy, UINT8 uiVsDelay);
void ide_setInterlace(BOOL b_inter);
void ide_setStartField(BOOL b_oddst);
BOOL ide_getInterlace(void);
BOOL ide_getStartField(void);
BOOL ide_getCurField(void);
void ide_setDigitalTiming(UINT32 uiCoddBlkSt, UINT32 uiCoddBlkEd, UINT32 uiCevenBlkSt, UINT32 uiCevenBlkEd, UINT32 uiCfidSt, UINT32 uiCfidEd);

// CSB
void ide_fillGamma(UINT8 *uipgamma);
void ide_getGamma(UINT8 *uipgamma);
void ide_setCtrst(UINT32 uiCTRST);
UINT32 ide_getCtrst(void);
void ide_setBrt(UINT32 uiBRT);
INT8 ide_getBrt(void);
void ide_setCmults(UINT32 uiCMULTS);
UINT32 ide_getCmults(void);
void ide_setCex(BOOL b_cex);
void ide_setYCex(BOOL b_ycex);
BOOL ide_getYCex(void);

void ide_setICST(BOOL b_en);
BOOL ide_getICST(void);
void ide_setCsbEn(BOOL b_en);
BOOL ide_getCsbEn(void);
void ide_setGammaEn(BOOL b_en);
BOOL ide_getGammaEn(void);
void ide_setDitherVbits(IDE_DITHER_VBITS b_rsel, IDE_DITHER_VBITS b_gsel, IDE_DITHER_VBITS b_bsel);
void ide_setOutComp(IDE_OUT_COMP uiComp0, IDE_OUT_COMP uiComp1, IDE_OUT_COMP uiComp2, BOOL b_BitSwp, BOOL b_Len);
void ide_getOutComp(IDE_OUT_COMP *uiComp0, IDE_OUT_COMP *uiComp1, IDE_OUT_COMP *uiComp2, BOOL *b_BitSwp, BOOL *b_Len);

// Interrupt
UINT32 ide_getInterruptStatus(void);
void ide_clearInterruptStatus(UINT32 uiINTstatus);
void ide_setInterruptEn(UINT32 uiINTen);
void ide_clrInterruptEn(UINT32 uiINT);

// Color Space Transform
void ide_setFcstCoef(UINT32 *uipCoef);
void ide_setIcstPreOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void ide_setIcstCoef(UINT32 *uipCoef);
void ide_setOutOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void ide_setOutLimit(UINT8 uiYLow, UINT8 uiYUp, UINT8 uiCbLow, UINT8 uiCbUp, UINT8 uiCrLow, UINT8 uiCrUp);
void ide_getIcstPreOffset(INT32 *uiY, INT32 *uiCb, INT32 *uiCr);
void ide_getIcstCoef(INT32 *uipCoef);
void ide_getOutOffset(INT32 *uiY, INT32 *uiCb, INT32 *uiCr);
//#NT#2011/07/15#Shirley Cheng -begin
void ide_setYCCClamp(CLAMP_TYPE uiclamp);
//#NT#2011/07/15#Shirley Cheng -end

// Alpha Blending
void ide_setAlphaBlending(IDE_BLEND_LAYER uiLayer, IDE_ALPHA_TYPE uiAlphaType, UINT8 uiGlobalAlpha);

// Sub-Pixel interpolation
void ide_setSubpixel(BOOL bOddLine, BOOL bR, BOOL bG, BOOL bB);
void ide_getSubpixel(BOOL bOddLine, BOOL *bR, BOOL *bG, BOOL *bB);

//-------------------------------------------------
// IDE Video register programm APIs
//-------------------------------------------------

// Video control
void ide_setV1En(BOOL b_en);
void ide_setV2En(BOOL b_en);
BOOL ide_getV1En(void);
BOOL ide_getV2En(void);

// Video buffer
void ide_setV1Buf0Addr(UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void ide_setV1Buf1Addr(UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void ide_setV1Buf2Addr(UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void ide_setV1Buf0Odd(BOOL b_odd);
void ide_setV1Buf1Odd(BOOL b_odd);
void ide_setV1Buf2Odd(BOOL b_odd);
void ide_setV1BufDim(UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void ide_setV1ReadOrder(IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
void ide_setV1BufOp(IDE_BJMODE uiBJMODE, IDE_OP_BUF uiOPTBUF, IDE_BUF_NUM uiBUFNUM);
void ide_chV1Buf(IDE_OP_BUF uiOPTBUF);
//void ide_setV1Cst(BOOL b_en, CST_SEL b_sel);
void ide_setV1Src(BOOL b_sel);

void ide_setV2Buf0Addr(UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void ide_setV2Buf1Addr(UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void ide_setV2Buf2Addr(UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void ide_setV2Buf0Odd(BOOL b_odd);
void ide_setV2Buf1Odd(BOOL b_odd);
void ide_setV2Buf2Odd(BOOL b_odd);
void ide_setV2BufDim(UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void ide_setV2ReadOrder(IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
void ide_setV2BufOp(IDE_BJMODE uiBJMODE, IDE_OP_BUF uiOPTBUF, IDE_BUF_NUM uiBUFNUM);
void ide_chV2Buf(IDE_OP_BUF uiOPTBUF);
//void ide_setV2Cst(BOOL b_en, CST_SEL b_sel);
void ide_setV2Src(BOOL b_sel);

void ide_getV1Buf0Addr(UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void ide_getV1Buf1Addr(UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void ide_getV1Buf2Addr(UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void ide_getV1Buf0Odd(BOOL *b_odd);
void ide_getV1Buf1Odd(BOOL *b_odd);
void ide_getV1Buf2Odd(BOOL *b_odd);
void ide_getV1BufDim(UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void ide_getV1BufOp(IDE_BJMODE *puiBJMODE, IDE_OP_BUF *puiOPTBUF, IDE_BUF_NUM *puiBUFNUM);
void ide_getV1ReadOrder(IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

void ide_getV2Buf0Addr(UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void ide_getV2Buf1Addr(UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void ide_getV2Buf2Addr(UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void ide_getV2Buf0Odd(BOOL *b_odd);
void ide_getV2Buf1Odd(BOOL *b_odd);
void ide_getV2Buf2Odd(BOOL *b_odd);
void ide_getV2BufDim(UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void ide_getV2BufOp(IDE_BJMODE *puiBJMODE, IDE_OP_BUF *puiOPTBUF, IDE_BUF_NUM *puiBUFNUM);
void ide_getV2ReadOrder(IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

// Video Window
void ide_setV1ScaleCtrl(BOOL b_Hscaleup, BOOL b_Vscaleup);
void ide_setV1ScaleFactor(UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF);
void ide_setV1VsfInit(UINT32 uiInit0, UINT32 uiInit1);
void ide_setV1WinDim(UINT32 uiWinW, UINT32 uiWinH);
void ide_setV1WinPos(UINT32 uiX, UINT32 uiY);
void ide_setV1Fmt(IDE_COLOR_FORMAT uiFmt);
void ide_setV2Hsm(BOOL b_hsm);

void ide_setV2ScaleCtrl(BOOL b_Hscaleup, BOOL b_Vscaleup);
void ide_setV2ScaleFactor(UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF);
void ide_setV2VsfInit(UINT32 uiInit0, UINT32 uiInit1);
void ide_setO1Hsm(BOOL b_hsm);
void ide_setV2WinDim(UINT32 uiWinW, UINT32 uiWinH);
void ide_setV2WinPos(UINT32 uiX, UINT32 uiY);
void ide_setV2Fmt(IDE_COLOR_FORMAT uiFmt);

void ide_getV1ScaleFactor(UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF);
void ide_getV1WinDim(UINT32 *uiWinW, UINT32 *uiWinH);
void ide_getV1WinPos(UINT32 *uiX, UINT32 *uiY);
void ide_getV1Fmt(IDE_COLOR_FORMAT *uiFmt);

void ide_getV2ScaleFactor(UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF);
void ide_getV2WinDim(UINT32 *uiWinW, UINT32 *uiWinH);
void ide_getV2WinPos(UINT32 *uiX, UINT32 *uiY);
void ide_getV2Fmt(IDE_COLOR_FORMAT *uiFmt);

// Video Color key and Blending
void ide_setVideoColorKey(UINT8 uiCkY, UINT8 uiCkCb, UINT8 uiCkCr);
//void ide_setVideoBlendOp(IDE_VIDEO_BLEND_OP uiOp);
void ide_setVideoColorKeySel(IDE_VIDEO_COLORKEY_SEL b_sel);
void ide_setVideoColorKeyOp(IDE_VIDEO_COLORKEY_OP uiCkOp);

void ide_getVideoColorKey(UINT8 *uiCkY, UINT8 *uiCkCb, UINT8 *uiCkCr);
//void ide_getVideoBlendOp(IDE_VIDEO_BLEND_OP *uiOp);
void ide_getVideoColorKeySel(IDE_VIDEO_COLORKEY_SEL *b_sel);
void ide_getVideoColorKeyOp(IDE_VIDEO_COLORKEY_OP *uiCkOp);

// Video Color Space Transform
//void ide_setV1CstPreOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setV1CstPostOffst(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setV1CstCoef(ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//void ide_setV2CstPreOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setV2CstPostOffst(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setV2CstCoef(ACCRCY_SEL b_accuracy, UINT32 *uipCoef);


// Video Auto Blinking
/*void ide_setV1Blink(BOOL b_ovr, BOOL b_und, BOOL b_sel);
void ide_setV1Count(UINT8 uiCnt);
void ide_setV1OvrExpThreshold(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setV1OvrExpColor(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setV1UndExpThreshold(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setV1UndExpColor(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setV2Blink(BOOL b_ovr, BOOL b_und, BOOL b_sel);
void ide_setV2Count(UINT8 uiCnt);
void ide_setV2OvrExpThreshold(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setV2OvrExpColor(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setV2UndExpThreshold(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setV2UndExpColor(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
UINT8 ide_getV1Count(void);
UINT8 ide_getV2Count(void);
void ide_getV1OvrExpThreshold(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void ide_getV2OvrExpThreshold(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void ide_getV1UndExpThreshold(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void ide_getV2UndExpThreshold(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void ide_getV1OvrExpColor(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void ide_getV2OvrExpColor(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void ide_getV1UndExpColor(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void ide_getV2UndExpColor(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);*/
//#NT#2011/07/05#Shirley Cheng -begin
void ide_setV1Burst(IDE_V1_Y_BURST uiYBurst, IDE_V1_C_BURST uiCBurst);
//#NT#2011/07/05#Shirley Cheng -end
//-------------------------------------------------
// IDE OSD register programm APIs
//-------------------------------------------------

// OSD control
void ide_setO1En(BOOL b_en);
void ide_setO2En(BOOL b_en);
BOOL ide_getO1En(void);
BOOL ide_getO2En(void);

// OSD buffer
void ide_setO1BufAddr(UINT32 uiaddr);
void ide_setO1BufAlphaAddr(UINT32 uiaddr);
void ide_setO1BufDim(UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void ide_setO1ReadOrder(IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
//void ide_setO1Cst(BOOL b_en, CST_SEL b_sel);

void ide_setO2BufAddr(UINT32 uiaddr);
void ide_setO2BufDim(UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void ide_setO2ReadOrder(IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
//void ide_setO2Cst(BOOL b_en, CST_SEL b_sel);

void ide_getO1BufAddr(UINT32 *uiaddr);
void ide_getO1AlphaBufAddr(UINT32 *uiaddr);
void ide_getO1BufDim(UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void ide_getO1ReadOrder(IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

void ide_getO2BufAddr(UINT32 *uiaddr);
void ide_getO2BufDim(UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void ide_getO2ReadOrder(IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

// OSD Window
void ide_setO1ScaleCtrl(BOOL b_Hscaleup, BOOL b_Vscaleup);
void ide_setO1ScaleFactor(UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF);
void ide_setO1VsfInit(UINT32 uiInit0, UINT32 uiInit1);
void ide_setO1Hsm(BOOL b_hsm);
void ide_setO1WinDim(UINT32 uiWinW, UINT32 uiWinH);
void ide_setO1WinPos(UINT32 uiX, UINT32 uiY);
void ide_setO1PaletteSel(IDE_PALETTE_SEL uiPsel);
void ide_setO1PaletteHighAddr(UINT8 uiHiAddr);
void ide_setO1Fmt(IDE_COLOR_FORMAT uiFmt);

void ide_setO2ScaleCtrl(BOOL b_Hscaleup, BOOL b_Vscaleup);
void ide_setO2ScaleFactor(UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF);
void ide_setO2VsfInit(UINT32 uiInit0, UINT32 uiInit1);
void ide_setO2WinDim(UINT32 uiWinW, UINT32 uiWinH);
void ide_setO2WinPos(UINT32 uiX, UINT32 uiY);
void ide_setO2PaletteSel(IDE_PALETTE_SEL uiPsel);
void ide_setO2PaletteHighAddr(UINT8 uiHiAddr);
void ide_setO2Fmt(IDE_COLOR_FORMAT uiFmt);

void ide_getO1ScaleFactor(UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF);
void ide_getO1WinDim(UINT32 *uiWinW, UINT32 *uiWinH);
void ide_getO1WinPos(UINT32 *uiX, UINT32 *uiY);
void ide_getO1PaletteSel(IDE_PALETTE_SEL *uiPsel);
void ide_getO1PaletteHighAddr(UINT8 *uiHiAddr);
void ide_getO1Fmt(IDE_COLOR_FORMAT *uiFmt);

void ide_getO2ScaleFactor(UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF);
void ide_getO2WinDim(UINT32 *uiWinW, UINT32 *uiWinH);
void ide_getO2WinPos(UINT32 *uiX, UINT32 *uiY);
void ide_getO2PaletteSel(IDE_PALETTE_SEL *uiPsel);
void ide_getO2PaletteHighAddr(UINT8 *uiHiAddr);
void ide_getO2Fmt(IDE_COLOR_FORMAT *uiFmt);

// OSD Color key and Blending
void ide_setOsdColorKeyEn(BOOL b_en);
void ide_setOsdColorKey(UINT8 uiCk);
//void ide_setOsdBlendOp(IDE_OSD_BLEND_OP uiOp);

void ide_getOsdColorKey(UINT8 *uiCk);
void ide_getOsdColorKeyEn(BOOL *b_en);
//void ide_getOsdBlendOp(IDE_OSD_BLEND_OP *uiOp);

// OSD Color Space Transform
//void ide_setO1CstPreOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO1CstPostOffst(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO1CstCoef(ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//void ide_setO2CstPreOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO2CstPostOffst(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void ide_setO2CstCoef(ACCRCY_SEL b_accuracy, UINT32 *uipCoef);
//#NT#2011/07/05#Shirley Cheng -begin
void ide_setO1Burst(IDE_O1_BURST uiOBurst);
//#NT#2011/07/05#Shirley Cheng -end
//-------------------------------------------------
// TV Encoder register programm APIs
//-------------------------------------------------

void ide_setTvUsePostICST(BOOL b_sel);
void ide_setTvUsePostGamma(BOOL b_sel);
void ide_setTvOutOffset(UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void ide_setTvOutLimit(UINT8 uiYLow, UINT8 uiYUp, UINT8 uiCbLow, UINT8 uiCbUp, UINT8 uiCrLow, UINT8 uiCrUp);

void ide_setTvMode(IDE_TV_MODE uiTVmode);
void ide_setTvTestMode(BOOL b_en);
void ide_setTvSlaveMode(BOOL b_en);
void ide_setTvSacleSwap(BOOL b_en);
void ide_setTvDataSwap(BOOL b_en);
void ide_setTvSinCosSwap(BOOL b_en);
void ide_setTvSearchEn(BOOL b_en);
void ide_setTvCkpnPol(BOOL b_sel);
void ide_setTvBurst(UINT8 uiBurst);
void ide_setTvBll(UINT8 uiBll);
void ide_setTvBrl(UINT8 uiBrl);
void ide_setTvNtscSetupLevel(UINT8 uiNtscSetup);
void ide_setTvScale(UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void ide_setTvPowerDown(BOOL b_pd);
void ide_setTvSela(UINT8 uiSela);
void ide_setTvLowLevel(UINT32 uiLevel);

UINT8 ide_getTvBll(void);
UINT8 ide_getTvBrl(void);
UINT8 ide_getTvNtscSetupLevel(void);
void ide_getTvScale(UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);

void ide_setTvBypassLPF(BOOL bEn);
BOOL ide_getTvBypassLPF(void);
void ide_setTvBypassNotch(BOOL bEn);
BOOL ide_getTvBypassNotch(void);
void ide_setTvChromaFilter(BOOL bPrag, UINT8 *uiTaps);
void ide_setTvYFilter(BOOL bPrag, UINT8 *uiTaps);
void ide_setTvCalEn(BOOL b_en);
void ide_setTvCalRes(TV_DAC_SEL tv_dac_sel, TV_DAC_ISEL tv_dac_isel);

//-------------------------------------------------
// IDE Public Functions
//-------------------------------------------------

BOOL ide_setVideoBufAddr(IDE_VIDEOID VideoID, VIDEO_BUF_ADDR *pV_Buf_Addr);
BOOL ide_getVideoBufAddr(IDE_VIDEOID VideoID, VIDEO_BUF_ADDR *pV_Buf_Addr);
BOOL ide_setVideoBufAttr(IDE_VIDEOID VideoID, VIDEO_BUF_ATTR *pV_Buf_attr);
BOOL ide_getVideoBufAttr(IDE_VIDEOID VideoID, VIDEO_BUF_ATTR *pV_Buf_attr);
BOOL ide_setVideoWinAttrEx(IDE_VIDEOID VideoID, VOSD_WINDOW_ATTR *pV_Win_attr, BOOL bLoad);
BOOL ide_setVideoWinAttr(IDE_VIDEOID VideoID, VOSD_WINDOW_ATTR *pV_Win_attr);
void ide_setVideoVsfInit(IDE_VIDEOID VideoID, UINT32 uiInit0, UINT32 uiInit1);

BOOL ide_setOsdBufAddr(IDE_OSDID OSDID, UINT32 uiOSD_Addr);
BOOL ide_getOsdBufAddr(IDE_OSDID OSDID, UINT32 *puiOSD_Addr);
BOOL ide_setOsdBufAttr(IDE_OSDID OSDID, OSD_BUF_ATTR *pOSD_Buf_attr);
BOOL ide_getOsdBufAttr(IDE_OSDID OSDID, OSD_BUF_ATTR *pOSD_Buf_attr);
BOOL ide_setOsdWinAttrEx(IDE_OSDID OSDID, VOSD_WINDOW_ATTR *pOSD_Win_attr, BOOL bLoad);
BOOL ide_setOsdWinAttr(IDE_OSDID OSDID, VOSD_WINDOW_ATTR *pOSD_Win_attr);
void ide_setOsdVsfInit(IDE_OSDID OSDID, UINT32 uiInit0, UINT32 uiInit1);

void ide_setPaletteEntry(PALETTE_ENTRY *pPalette_entry);
void ide_getPaletteEntry(PALETTE_ENTRY *pPalette_entry);
void ide_setPaletteGroup(UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void ide_getPaletteGroup(UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);

void ide_waitFrameEnd(void);

void ide_disableVideo(IDE_VIDEOID VideoID);
void ide_enableVideo(IDE_VIDEOID VideoID);
BOOL ide_getVideoEnable(IDE_VIDEOID VideoID);

void ide_disableOsd(IDE_OSDID OSDID);
void ide_enableOsd(IDE_OSDID OSDID);
BOOL ide_getOsdEnable(IDE_OSDID OSDID);
IDE_PAL_CAP ide_getPalCapability(void);

void ide_setVideoBufferContent(IDE_VIDEOID VIDEO_ID, IDE_BUFFERID BUFFER_ID, PYUVCOLOR pYCbCr);
void ide_convertRGB2YCbCr(PRGBCOLOR pRGB, PYUVCOLOR pYCbCr);

//void ide_configVideoCst(IDE_VIDEOID VideoID, BOOL b_en, CST_SEL sel);
//void ide_configOsdCst(IDE_OSDID OSDID, BOOL b_en, CST_SEL sel);
//void ide_configIcst(BOOL b_en, CST_SEL sel);
void ide_configIcst(BOOL b_en, CST_SEL sel);
void ide_configOutputLimit(BOOL b_sel);
void ide_setConstantWindowOffset(UINT32 uiX, UINT32 uiY);

//-------------------------------------------------
// IDE OS Level Functions
//-------------------------------------------------

ID ide_getLockStatus(void);
#if 0
void ide_attach(void);
void ide_detach(void);
void ide_init(void);
#endif
ER ide_open(void);
ER ide_close(void);
BOOL ide_isOpened(void);
void ide_registerMiInterruptHandler(FPIDEInterruptCallBack IntrHdl);

//@}

#include "ide_compat.h"

#endif //_IDE_H

