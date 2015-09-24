/**
    LCD/TV Display Subsystem Configuration Header

    LCD/TV Display Subsystem. This module defines API for interfacing to driver.

    @file       LCDTV.h
    @ingroup    mISYSDisp
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _LCDTV_H
#define _LCDTV_H

#include "Type.h"
#include "ide.h"
#include "hdmitx.h"

/**
    \addtogroup mISYSDisp
*/
//@{


/**
    @name Main control of LCD interface
*/
//@{

/**
    SIF pin definition descriptor for LCD control.

    SIF is used as main control interface of LCD panel.
\n  This structure defines the interface is using SIF or using GPIO to emulate SIF.
*/
//@{
typedef struct _LCD_SIFPIN_DEF
{
    UINT32  uimode;                     ///< Use LCD_CTRL_MODE_SIF or LCD_CTRL_MODE_GPIO
    UINT32  uisif_channel;              ///< SIF channel SIF_CH0 ~ 5, refer to SIF.h, valid in LCD_CTRL_MODE_SIF
    UINT32  uigpio_sen;                 ///< GPIO pin number for SEN, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32  uigpio_clk;                 ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32  uigpio_data;                ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO
} LCD_SIFPIN_DEF, *PLCD_SIFPIN_DEF;

#define     LCD_CTRL_MODE_SIF       0   ///< LCD control use serial interface(SIF).
#define     LCD_CTRL_MODE_GPIO      1   ///< LCD control use GPIO.
//@}

/**
    LCD mode selection

    Select the operation mode of LCD interface.
*/
//@{
typedef enum
{
    LCD_MODE_RGB        = 0,    ///< LCD MODE is RGB
    LCD_MODE_RGBD       = 1,    ///< LCD MODE is RGB Dummy
    LCD_MODE_YUV        = 2,    ///< LCD MODE is YUV422 320X240
    LCD_MODE_RGBD_360   = 3,    ///< LCD MODE is RGB Dummy with 360 * 240 resolution
    LCD_MODE_CCIR656    = 4,    ///< LCD MODE is CCIR656
    LCD_MODE_CCIR601    = 5,    ///< LCD MODE is CCIR601
    LCD_MODE_UPS051     = 6,    ///< LCD MODE is UPS051
    LCD_MODE_YUV_360    = 7,    ///< LCD MODE is YUV422 360X240
    LCD_MODE_MI_FMT0    = 8,    ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT1    = 9,    ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT2    = 10,   ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT3    = 11,   ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT4    = 12,   ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT5    = 13,   ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT6    = 14,   ///< LCD MODE is Memory(Parallel Interface) 8bits
    LCD_MODE_MI_FMT7    = 15,   ///< LCD MODE is Memory(Parallel Interface) 9bits
    LCD_MODE_SERIAL_BI  = 16,   ///< LCD MODE is Serial Interface bi-direction
    LCD_MODE_SERIAL_SEP = 17,   ///< LCD MODE is Serial Interface separation
    LCD_MODE_RGB_THROUGH= 18,   ///< LCD MODE is RGB through mode
    LCD_MODE_HDMI_480P  = 19,   ///< LCD MODE is HDMI 480P
    LCD_MODE_HDMI_576P  = 20,   ///< LCD MODE is HDMI 576P
    LCD_MODE_HDMI_720P  = 21,   ///< LCD MODE is HDMI 720P
    LCD_MODE_MI_FMT8    = 22,   ///< LCD MODE is Memory(Parallel Interface) 16bits
    LCD_MODE_MI_FMT9    = 23,   ///< LCD MODE is Memory(Parallel Interface) 16bits
    LCD_MODE_MI_FMT10   = 24,   ///< LCD MODE is Memory(Parallel Interface) 18bits
    LCD_MODE_MI_FMT11   = 25,   ///< LCD MODE is Memory(Parallel Interface)  8bits
    LCD_MODE_MI_FMT12   = 26,   ///< LCD MODE is Memory(Parallel Interface) 16bits
    LCD_MODE_MI_FMT13   = 27,   ///< LCD MODE is Memory(Parallel Interface) 16bits
    LCD_MODE_MI_FMT14   = 28,   ///< LCD MODE is Memory(Parallel Interface) 16bits
    LCD_MODE_LCDTV      = 29,   ///< LCD MODE is LCD+TV


    ENUM_DUMMY4WORD(LCD_MODE)
} LCD_MODE;
//@}

/**
    LCD Source clock selection.

    This specifies the clock source of LCD(/IDE) engine.
*/
//@{
typedef enum
{
    LCD_SRCCLK_PLL1     = 0,            ///< LCD Source clock is PLL1
    LCD_SRCCLK_PLL2     = 1,            ///< LCD Source clock is PLL2
    //#NT#2010/02/25#Klins Chen -begin
    LCD_SRCCLK_PLL3     = 2,            ///< Backward compatible usage, pls use LCD_SRCCLK_SSPLL.
    //#NT#2010/02/25#Klins Chen -end
    //#NT#2010/09/14#Klins Chen -begin
    LCD_SRCCLK_SSPLL    = 2,             ///< LCD Source clock is SSPLL
    //#NT#2010/09/14#Klins Chen -end
    ENUM_DUMMY4WORD(LCD_SRCCLKSEL)
} LCD_SRCCLKSEL;
//@}

/**
    LCD device main object

    This object defines the main control parameters for LCD device.
*/
//@{
typedef struct _LCD_DEVICE_OBJ
{
    LCD_SIFPIN_DEF  LCDCtrl_Pin;                ///< Serial interface(SIF) pin definition which used to control the LCD panel.
    LCD_MODE        uiLCDMode;                  ///< Mode select of LCD
    UINT32          uiIDESrcCLK;                ///< IDE source clock in MHz
    LCD_SRCCLKSEL   bIDESrcPLL2;                ///< LCD_SRCCLKSEL type: the IDE source clock is PLL1 or PLL2.
    float           fIDEDstCLK;                 ///< the IDE destination clock, ex:13.5
    void          (*fpPostAdjusement)(void);    ///< Callback Function for user set LCD saturation,brightness,gamma...
} LCD_DEVICE_OBJ, *PLCD_DEVICE_OBJ;
//@}

//@}



/**
    @name Main control of TV interface
*/
//@{

//Different FSADJ need different configuration,
//Add a new param for this issue
//Depend on hw design
//TV_FSADJ_3_0_K : 3K / TV_FSADJ_3_3_K : 3.3K / TV_FSADJ_3_6_K : 3.6K
typedef enum
{
    TV_FSADJ_3_6_K = 0,
    TV_FSADJ_3_3_K,
    TV_FSADJ_3_0_K,
    TV_FSADJ_TOTAL,
    TV_FSADJ_DEFAULT = TV_FSADJ_3_0_K,

    ENUM_DUMMY4WORD(LCD_TV_FSADJ)
} LCD_TV_FSADJ;

/**
    TV device main object

    This object defines the main control parameters for TV device.
*/
//@{
typedef struct _TV_DEVICE_OBJ
{
    UINT32          uiIDESrcCLK;        ///< IDE source clock in MHz
    LCD_SRCCLKSEL   bIDESrcPLL2;        ///< LCD_SRCCLKSEL type: the IDE source clock is PLL1 or PLL2.
    UINT32          uiTVFSADJ;          ///< TV FSADJ : 3.6K / 3.3K / 3.0K depend on Hw design
} TV_DEVICE_OBJ, *PTV_DEVICE_OBJ;
//@}

//@}


/**
    Open LCD panel driver

    This function should be called before calling any other functions during initializing LCD driver.
    It would initilize the settings for specified type of LCD device and set it to specified operation mode.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

    @param[in,out]  lcddeviceobj    LCD device main object, refer the definition of LCD_DEVICE_OBJ.
    @param[in]      psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

    @return void
*/
extern void OpenPanel(LCD_DEVICE_OBJ *lcddeviceobj, LCDSIZE *psizeDisplay);


/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
extern void ClosePanel(void);

/**
    Init TV/NTSC device object.

    Init TV/NTSC device object for proper clock configuration.

    @param[in] ptvntscobj   Pointer to TV device object.

    @return void
*/
extern void InitTVNTSC_OBJ(TV_DEVICE_OBJ *ptvntscobj);

/**
    Init TV/PAL device object.

    Init TV/PAL device object for proper clock configuration.

    @param[in] ptvpalobj    Pointer to TV device object.

    @return void
*/
extern void InitTVPAL_OBJ(TV_DEVICE_OBJ *ptvpalobj);

/**
    Open TV/NTSC display.

    Open TV/NTSC display.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

   @param[in]   psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

   @return  void
*/
extern void OpenTVNTSC(LCDSIZE *psizeDisplay);

/**
    Close TV/NTSC display.

    Call this api to close the TV/NTSC device.

    @return void
*/
extern void CloseTVNTSC(void);

/**
    Open TV/PAL display.

    Open TV/PAL display.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

   @param[in]  psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

   @return  void
*/
extern void OpenTVPAL(LCDSIZE *psizeDisplay);

/**
    Close TV/PAL display.

    Call this api to close the TV/PAL device.

    @return void
*/
extern void CloseTVPAL(void);

/**
    Write LCD Internal Register

    Optional Implementation

    @param[in] addr     LCD panel register address
    @param[in] value    Write value

    @return void
*/
extern void LCD_RegWrite(UINT32 addr, UINT32 value);


/**
    Init TV HDMI device operation mode.

    This API must be called before OpenTVHDMI() to setup the Video/Audio format of HDMI-Tx.
    The settings of Video/Audio formats will be taken effective in OpenTVHDMI().

    @param[in] VideoID  Video format ID of HDMI-Tx. The supported formats are as follows:
     - @b HDMI_640X480P60
     - @b HDMI_720X480P60
     - @b HDMI_1280X720P60
     - @b HDMI_1920X1080I60
     - @b HDMI_1440X480I60
     - @b HDMI_720X576P50
     - @b HDMI_1280X720P50
     - @b HDMI_1920X1080I50
    @param[in] AudioID  Audio format ID of HDMI-Tx. The supported formats are as follows:
     - @b HDMI_32KHZ
     - @b HDMI_44_1KHZ
     - @b HDMI_48KHZ

    @return void
*/
extern void InitTVHDMI(HDMI_VIDEOID VideoID, HDMI_AUDIOFMT AudioID);

/**
    Open TV HDMI driver

    It would initilize the settings for specified type of HDMI mode and set it to specified operation mode.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

    @param[in,out]  lcddeviceobj    LCD device main object, refer the definition of LCD_DEVICE_OBJ.
    @param[in]      psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

    @return void
*/
extern void OpenTVHDMI(LCD_DEVICE_OBJ *lcddeviceobj, LCDSIZE *psizeDisplay);

/**
    Close TV HDMI

    Call this api to close the TV HDMI transmitter.

    @return void
*/
extern void CloseTVHDMI(void);

//@}


#endif

