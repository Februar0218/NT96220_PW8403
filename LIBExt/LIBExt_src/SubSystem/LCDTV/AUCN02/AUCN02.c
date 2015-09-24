/*
    @file       AUCN02.c

    @ingroup    mILIBLCDAUCN02

    @brief      AU LCD Driver of AUCN02 panel

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDAUCN02

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0000).

@endverbatim

@{
*/
#include "OSCommon.h"
#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "Utility.h"

//Settings
//UPS052 (RGBD)
#define SETTING_UPS052_DCLK                     24.55
#define SETTING_UPS052_HSYNC_PERIOD             1560
#define SETTING_UPS052_HSYNC_DISPLAY_PERIOD     1280
#define SETTING_UPS052_HSYNC_BACK_PORCH         252
#define SETTING_UPS052_VSYNC_PERIOD             262
#define SETTING_UPS052_VSYNC_DISPLAY_PERIOD     240
#define SETTING_UPS052_VSYNC_BACK_PORCH_ODD     13
#define SETTING_UPS052_VSYNC_BACK_PORCH_EVEN    13
//CCIR656
#define SETTING_CCIR656_DCLK                    27
#define SETTING_CCIR656_HSYNC_PERIOD            1716
#define SETTING_CCIR656_HSYNC_DISPLAY_PERIOD    1440
#define SETTING_CCIR656_HSYNC_BACK_PORCH        273
#define SETTING_CCIR656_VSYNC_PERIOD            524   //(ODD+EVEN)
#define SETTING_CCIR656_VSYNC_DISPLAY_PERIOD    480   //(ODD+EVEN)
#define SETTING_CCIR656_VSYNC_BACK_PORCH_ODD    36    //(ODD+EVEN)
#define SETTING_CCIR656_VSYNC_BACK_PORCH_EVEN   36+1  //(ODD+EVEN)
//YUV:
#define SETTING_YUV_DCLK                        24.55
#define SETTING_YUV_HSYNC_PERIOD                1560
#define SETTING_YUV_HSYNC_DISPLAY_PERIOD        1280
#define SETTING_YUV_HSYNC_BACK_PORCH            252
#define SETTING_YUV_VSYNC_PERIOD                262
#define SETTING_YUV_VSYNC_DISPLAY_PERIOD        240
#define SETTING_YUV_VSYNC_BACK_PORCH_ODD        13
#define SETTING_YUV_VSYNC_BACK_PORCH_EVEN       13
//UPS051
#define SETTING_UPS051_DCLK                     9.7
#define SETTING_UPS051_HSYNC_PERIOD             616
#define SETTING_UPS051_HSYNC_DISPLAY_PERIOD     480
#define SETTING_UPS051_HSYNC_BACK_PORCH         100
#define SETTING_UPS051_VSYNC_PERIOD             262
#define SETTING_UPS051_VSYNC_DISPLAY_PERIOD     234
#define SETTING_UPS051_VSYNC_BACK_PORCH_ODD     16
#define SETTING_UPS051_VSYNC_BACK_PORCH_EVEN    16


#define AU_CN01_UPS051_Buf_width       502  /*480 For Analog signal*/
#define AU_CN01_UPS051_Buf_height      240
#define AU_CN01_UPS051_Win_width       502  /*480 For Analog signal*/
#define AU_CN01_UPS051_Win_height      240

#define AU_CN01_UPS052_Buf_width       320
#define AU_CN01_UPS052_Buf_height      240
#define AU_CN01_UPS052_Win_width       320
#define AU_CN01_UPS052_Win_height      240

#define AU_CN01_CCIR656_Buf_width      720
#define AU_CN01_CCIR656_Buf_height     480
#define AU_CN01_CCIR656_Win_width      720
#define AU_CN01_CCIR656_Win_height     480

//#NT#2007/03/07#Shirley Cheng -begin
//Add YUV 640 mode support
#define AU_CN01_YUV640_Buf_width       640
#define AU_CN01_YUV640_Buf_height      240
#define AU_CN01_YUV640_Win_width       640
#define AU_CN01_YUV640_Win_height      240
//#NT#2007/03/07#Shirley Cheng -end

//UINT8 gAUCN02_VCOM_DC = 0x18;

//function declaration
static void delay(UINT32 times);
static void sendtoAUCN02LCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoAUCN02LCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoAUCN02LCDreg(UINT8 addr, UINT8 value);

static void Init_AUCN02_LCD(void);
static void OpenAUCN02(LCDSIZE *psizeDisplay);
static void CloseAUCN02(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;

#if 1
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
void OpenPanel(LCD_DEVICE_OBJ *lcddeviceobj, LCDSIZE *psizeDisplay)
{
    LCD_DeviceObj = *lcddeviceobj;
    LCD_DeviceObj.uiLCDMode = pinmux_getDispMode(PINMUX_FUNC_ID_LCD);

    if (LCD_DeviceObj.fIDEDstCLK == 0)
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case PINMUX_LCDMODE_RGBD320:
            LCD_DeviceObj.fIDEDstCLK = SETTING_UPS052_DCLK;
            break;
        case PINMUX_LCDMODE_CCIR656:
            LCD_DeviceObj.fIDEDstCLK = SETTING_CCIR656_DCLK;
            break;
        //#NT#2007/03/07#Shirley Cheng -begin
        case PINMUX_LCDMODE_YUV640:
            LCD_DeviceObj.fIDEDstCLK = SETTING_YUV_DCLK;
            break;
        //#NT#2007/03/07#Shirley Cheng -end

        //#NT#2007/10/23#Elvis Chuang -begin
        case PINMUX_LCDMODE_RGB_SERIAL:
            LCD_DeviceObj.fIDEDstCLK = SETTING_UPS051_DCLK;
            break;
        //#NT#2007/10/23#Elvis Chuang -end
        }
    }

    OpenAUCN02(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseAUCN02();
}

/**
    Write LCD Internal Register

    Optional Implementation

    @param[in] addr     LCD panel register address
    @param[in] value    Write value

    @return void
*/
//#NT#2009/11/04#Klins Chen -begin
//#NT#Change prototype from UINT8 to UINT32 to provide more flexibility panel type.
void LCD_RegWrite(UINT32 addr, UINT32 value)
//#NT#2009/11/04#Klins Chen -end
{
    sendtoAUCN02LCDreg(addr, value);
}
#endif

/*
  delay time period

  @param times : time period

*/
void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/*
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoAUCN02LCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = ((((UINT8)addr << 8)|(UINT8)value ) << 16);
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/*
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoAUCN02LCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)addr << 8 | (UINT32)value;

    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 16; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(200);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

/*
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
void sendtoAUCN02LCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoAUCN02LCDreg_sif(addr, value);
    }
    else
    {
         sendtoAUCN02LCDreg_gpio(addr, value);
    }
}

/*
    Initial LCD of AUCN02, configure SIF intarface and send data to panel register to configure

*/
static void Init_AUCN02_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

        //#NT#2007/05/08#Steven Wang -begin
        //#Modify init flow, SIF driver will change pinmux
        /*
        // Init SIF
        switch (LCD_DeviceObj.LCDCtrl_Pin.uisif_channel)
        {
        case SIF_CH0:
        default:
            pinmux_enable_sif(PINMUX_SIF_CH0);
            break;
        case SIF_CH1:
            pinmux_enable_sif(PINMUX_SIF_CH1);
            break;
        case SIF_CH2:
            pinmux_enable_sif(PINMUX_SIF_CH2);
            break;
        case SIF_CH3:
            pinmux_enable_sif(PINMUX_SIF_CH3);
            break;
        }
        */
        //#NT#2007/05/08#Steven Wang -end
        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = SIF_MODE_0000;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        //SIFCtrl.bSyncEn       = FALSE;
        //SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;


        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    if(LCD_DeviceObj.fpPostAdjusement)
        LCD_DeviceObj.fpPostAdjusement();//sendtoAUCN02LCDreg);
    else
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case PINMUX_LCDMODE_RGBD320:
            sendtoAUCN02LCDreg(0x60,0x01);
            sendtoAUCN02LCDreg(0x40,0x03);
            sendtoAUCN02LCDreg(0xC0,0x06);
            sendtoAUCN02LCDreg(0xE0,0x13);
            sendtoAUCN02LCDreg(0x00,0x0F);//sendtoAUBL02LCDreg(0x00,0x0F);
            break;
        case PINMUX_LCDMODE_CCIR656:
            sendtoAUCN02LCDreg(0x60,0x07);
            sendtoAUCN02LCDreg(0x40,0x03);
            sendtoAUCN02LCDreg(0xC0,0x06);
            sendtoAUCN02LCDreg(0x00,0x0F);//sendtoAUBL02LCDreg(0x00,0x0F);
            //sendtoAUCN02LCDreg(0xE0,0x13);
            break;
        //#NT#2007/03/07#Shirley Cheng -begin
        case PINMUX_LCDMODE_YUV640:
            sendtoAUCN02LCDreg(0x60,0x03);
            sendtoAUCN02LCDreg(0x20,0x03);  //PDON(bit1) = 1, bit0:default 1
            sendtoAUCN02LCDreg(0x40,0x03);
            sendtoAUCN02LCDreg(0xC0,0x06);
            sendtoAUCN02LCDreg(0x00,0x0F);
            break;
        //#NT#2007/03/07#Shirley Cheng -end
        //#NT#2007/10/23#Elvis Chuang -begin
        case PINMUX_LCDMODE_RGB_SERIAL:
            sendtoAUCN02LCDreg(0x60,0x00); //R3
            sendtoAUCN02LCDreg(0x40,0x03);
            sendtoAUCN02LCDreg(0xC0,0x06);
            sendtoAUCN02LCDreg(0x00,0x0F); //R0
            break;
        default:
            debug_err(("Init_AUCN02_LCD() not support %d\r\n", LCD_DeviceObj.uiLCDMode));
            break;
        //#NT#2007/10/23#Elvis Chuang -end
        }
    }
}

/*
  Open AUCN02 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
void OpenAUCN02(LCDSIZE *psizeDisplay)
{
    UINT8           uiClockDiv;

    if (ide_open() != E_OK)
        return;

    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Set IDE Clock Source
    if (LCD_DeviceObj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {
        LCD_DeviceObj.uiIDESrcCLK = 240;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_240);
    }
    else
    {
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }


    // Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case PINMUX_LCDMODE_RGBD320:
        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_RGB_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);


        // Wait for Power Stable
        //#NT#2009/11/04#Klins Chen#[0007606] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007606] -end

        ide_set_icst(0);
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_AU);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(1);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(1);
        ide_set_shw_st();

        ide_set_hor_timing(0x00
            ,SETTING_UPS052_HSYNC_PERIOD-1
            ,SETTING_UPS052_HSYNC_BACK_PORCH
            ,(SETTING_UPS052_HSYNC_DISPLAY_PERIOD+SETTING_UPS052_HSYNC_BACK_PORCH)-1);

         ide_set_ver_timing(0x00
            , SETTING_UPS052_VSYNC_PERIOD-1
            , SETTING_UPS052_VSYNC_BACK_PORCH_ODD
            , (SETTING_UPS052_VSYNC_DISPLAY_PERIOD+SETTING_YUV_VSYNC_BACK_PORCH_ODD)-1
            , SETTING_UPS052_VSYNC_BACK_PORCH_EVEN
            , (SETTING_UPS052_VSYNC_DISPLAY_PERIOD+SETTING_YUV_VSYNC_BACK_PORCH_EVEN)-1);

        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x01);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay ->uiWidth      = AU_CN01_UPS052_Buf_width;
        psizeDisplay ->uiHeight     = AU_CN01_UPS052_Buf_height;
        psizeDisplay ->uiWinWidth   = AU_CN01_UPS052_Win_width;
        psizeDisplay ->uiWinHeight  = AU_CN01_UPS052_Win_height;
        break;

    case PINMUX_LCDMODE_CCIR656:
        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_CCIR656);
        //pinmux_select_lcd(PINMUX_LCD_CCIR656);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for Power Stable
        //#NT#2009/11/04#Klins Chen#[0007606] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007606] -end

        ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CCIR656);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(1);
        ide_set_hsinv(0);
        ide_set_vsinv(0);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_shw_st();

        ide_set_hor_timing(0x00
            ,(SETTING_CCIR656_HSYNC_PERIOD>>1)
            ,(SETTING_CCIR656_HSYNC_BACK_PORCH>>1)
            ,(SETTING_CCIR656_HSYNC_DISPLAY_PERIOD+SETTING_YUV_HSYNC_BACK_PORCH)>>1);

        ide_set_ver_timing(0x05
            , SETTING_CCIR656_VSYNC_PERIOD-1
            , SETTING_CCIR656_VSYNC_BACK_PORCH_ODD
            , (SETTING_CCIR656_VSYNC_BACK_PORCH_ODD+SETTING_CCIR656_HSYNC_BACK_PORCH)-1
            , SETTING_CCIR656_VSYNC_BACK_PORCH_EVEN
            , (SETTING_CCIR656_VSYNC_BACK_PORCH_EVEN+SETTING_CCIR656_HSYNC_BACK_PORCH)-1);

        ide_set_interlace(0x01);
        ide_set_startfield(0x01);


        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth = AU_CN01_CCIR656_Buf_width;
        psizeDisplay->uiHeight = AU_CN01_CCIR656_Buf_height;
        psizeDisplay->uiWinWidth = AU_CN01_CCIR656_Win_width;
        psizeDisplay->uiWinHeight = AU_CN01_CCIR656_Win_height;
        break;

    case PINMUX_LCDMODE_YUV640:
    //#NT#2007/03/07#Shirley Cheng -begin
        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        // Set DCLK
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK);
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);
        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007606] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007606] -end

        ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_TOPPOLY);
        ide_set_pdir(0);
        ide_set_odd(1);
        ide_set_even(2);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_shw_st();

        ide_set_hor_timing(0x00
            , SETTING_YUV_HSYNC_PERIOD>>1
            , SETTING_YUV_HSYNC_BACK_PORCH>>1
            , (SETTING_YUV_HSYNC_DISPLAY_PERIOD+SETTING_YUV_HSYNC_BACK_PORCH)>>1);

        ide_set_ver_timing(0x00
            , SETTING_YUV_VSYNC_PERIOD-1
            , SETTING_YUV_VSYNC_BACK_PORCH_ODD
            , (SETTING_YUV_VSYNC_DISPLAY_PERIOD+SETTING_YUV_VSYNC_BACK_PORCH_ODD)-1
            , SETTING_YUV_VSYNC_BACK_PORCH_EVEN
            , (SETTING_YUV_VSYNC_DISPLAY_PERIOD+SETTING_YUV_VSYNC_BACK_PORCH_EVEN)-1);

        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth       = AU_CN01_YUV640_Buf_width;
        psizeDisplay->uiHeight      = AU_CN01_YUV640_Buf_height;
        psizeDisplay->uiWinWidth    = AU_CN01_YUV640_Win_width;
        psizeDisplay->uiWinHeight   = AU_CN01_YUV640_Win_height;
        break;
    //#NT#2007/03/07#Shirley Cheng -end
//#NT#2007/10/23#Elvis Chuang -begin
    case PINMUX_LCDMODE_RGB_SERIAL:
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_RGB_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));


        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007606] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007606] -end

        ide_set_icst(0);
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_AU);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(1);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_shw_st();

        ide_set_hor_timing(0x00
                ,SETTING_UPS051_HSYNC_PERIOD-1
                ,SETTING_UPS051_HSYNC_BACK_PORCH
                ,(SETTING_UPS051_HSYNC_DISPLAY_PERIOD+SETTING_UPS051_HSYNC_BACK_PORCH) - 1);

        ide_set_ver_timing(0x05
                ,SETTING_UPS051_VSYNC_PERIOD-1
                ,SETTING_UPS051_VSYNC_BACK_PORCH_ODD
                ,(SETTING_UPS051_VSYNC_DISPLAY_PERIOD+SETTING_UPS051_VSYNC_BACK_PORCH_ODD)-1
                ,SETTING_UPS051_VSYNC_BACK_PORCH_EVEN
                ,(SETTING_UPS051_VSYNC_DISPLAY_PERIOD+SETTING_UPS051_VSYNC_BACK_PORCH_EVEN)-1);


        ide_set_interlace(FALSE);
        ide_set_startfield(FALSE);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(FALSE);
        ide_set_standby_signal(TRUE);

        ide_set_clamp(TRUE);
        ide_set_tv_powerdown(TRUE);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth       = AU_CN01_UPS051_Buf_width;
        psizeDisplay->uiHeight      = AU_CN01_UPS051_Buf_height;
        psizeDisplay->uiWinWidth    = AU_CN01_UPS051_Win_width;
        psizeDisplay->uiWinHeight   = AU_CN01_UPS051_Win_height;
        break;
    default:
        debug_err(("OpenAUCN02() Not support %d mode\r\n",LCD_DeviceObj.uiLCDMode));
//#NT#2007/10/23#Elvis Chuang -end
        break;
    }

    //#NT#2009/11/04#Klins Chen#[0007606] -begin
    //#NT#Remove unnecessary dummy delay
    ide_set_en(TRUE);
    // Avoid vsync active area
    ide_wait_frame_end();
    //#NT#2009/11/04#Klins Chen#[0007606] -end

    Init_AUCN02_LCD();
}

/*
  Close AUCN02 driver

  Release AUCN02 driver, private function for internal used

*/
void CloseAUCN02(void)
{
    // Disable IDE
    //#NT#2009/11/04#Klins Chen#[0007606] -begin
    //#NT#Modify close flow
    ide_set_en(FALSE);
    ide_wait_frame_end();
    //#NT#2009/11/04#Klins Chen#[0007606] -end

    // Change to standby mode
    //#NT#2007/07/31#Shirley Cheng -begin
    //Fix change to standby mode register setting not match with spec.
    sendtoAUCN02LCDreg(0x00,0x0B);
    //#NT#2007/07/31#Shirley Cheng -end

    delay(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}
//@}
