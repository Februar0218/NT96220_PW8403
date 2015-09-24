/*
    @file       AUA027DN01.c

    @ingroup    mILIBLCDAUA027DN01

    @brief      AU LCD Driver of AUA027DN01 panel

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDAUA027DN01

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

#define AU_DN01_UPS051_Buf_width       960
#define AU_DN01_UPS051_Buf_height      240
#define AU_DN01_UPS051_Win_width       960
#define AU_DN01_UPS051_Win_height      240

#if 0
#define AU_CN01_UPS052_Buf_width       320
#define AU_CN01_UPS052_Buf_height      240
#define AU_CN01_UPS052_Win_width       320
#define AU_CN01_UPS052_Win_height      240
#else
#define AU_DN01_UPS052_Buf_width       360
#define AU_DN01_UPS052_Buf_height      240
#define AU_DN01_UPS052_Win_width       360
#define AU_DN01_UPS052_Win_height      240
#endif

#define AU_DCN01_CCIR656_Buf_width      720
#define AU_DN01_CCIR656_Buf_height     480
#define AU_DN01_CCIR656_Win_width      720
#define AU_DN01_CCIR656_Win_height     480

//#NT#2007/03/07#Shirley Cheng -begin
//Add YUV 640 mode support
#define AU_DN01_YUV640_Buf_width       640
#define AU_DN01_YUV640_Buf_height      240
#define AU_DN01_YUV640_Win_width       640
#define AU_DN01_YUV640_Win_height      240
//#NT#2007/03/07#Shirley Cheng -end

//UINT8 gAUCN01_VCOM_DC = 0x18;

//function declaration
static void delay(UINT32 times);
static void sendtoAUDN01LCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoAUDN01LCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoAUDN01LCDreg(UINT8 addr, UINT8 value);

static void Init_AUDN01_LCD(void);
static void OpenAUDN01(LCDSIZE *psizeDisplay);
static void CloseAUDN01(void);

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
        case PINMUX_LCDMODE_RGB_SERIAL:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;

        case PINMUX_LCDMODE_RGBD360:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        //#NT#2007/03/07#Shirley Cheng -begin
        case PINMUX_LCDMODE_YUV640:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        //#NT#2007/03/07#Shirley Cheng -end

        }
    }

    OpenAUDN01(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseAUDN01();
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
    sendtoAUDN01LCDreg(addr, value);
}
#endif

/*
  delay time period

  @param times : time period

*/
static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/*
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoAUDN01LCDreg_sif(UINT8 addr, UINT8 value)
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
static void sendtoAUDN01LCDreg_gpio(UINT8 addr, UINT8 value)
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
void sendtoAUDN01LCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoAUDN01LCDreg_sif(addr, value);
    }
    else
    {
         sendtoAUDN01LCDreg_gpio(addr, value);
    }
}

/*
    Initial LCD of AU027DN01, configure SIF intarface and send data to panel register to configure

*/
static void Init_AUDN01_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();
        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = SIF_MODE_0000;
        SIFCtrl.bSyncEn       = FALSE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        //SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;

        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    //#NT#2008/09/10#Niven -begin
    //#Added, Callback Function for user define LCD saturation,brightness,gamma...
    if(LCD_DeviceObj.fpPostAdjusement)
        LCD_DeviceObj.fpPostAdjusement();//sendtoAUDN01LCDreg);
    else
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
            case PINMUX_LCDMODE_RGB_SERIAL:
            //sendtoAUDN01LCDreg(0x00,0x03);//sendtoAUBL02LCDreg(0x00,0x0F);
            //sendtoAUDN01LCDreg(0x40,0x03);
            sendtoAUDN01LCDreg(0x04,0x0B);  //UPS052 360x240 --> 2B
            sendtoAUDN01LCDreg(0x05,0x5D);
            break;
        case PINMUX_LCDMODE_RGBD360:
            sendtoAUDN01LCDreg(0x00,0x03);//sendtoAUBL02LCDreg(0x00,0x0F);
            //sendtoAUDN01LCDreg(0x40,0x03);
            sendtoAUDN01LCDreg(0x04,0x2B);  //UPS052 360x240 --> 2B
            sendtoAUDN01LCDreg(0x05,0x5D);  //67
    //        sendtoAUDN01LCDreg(0x60,0x01);
    //        sendtoAUDN01LCDreg(0xC0,0x06);
    //        sendtoAUDN01LCDreg(0xE0,0x13);
             //   sendtoAUBL02LCDreg(0x03,0x40);
             //   sendtoAUBL02LCDreg(0x04,0x1B);  //UPS052
             //   sendtoAUBL02LCDreg(0x04,0x1B);  //UPS052
             //   sendtoAUBL02LCDreg(0x05,0x57);
             //   sendtoAUBL02LCDreg(0x05,0x57);

             //   sendtoAUBL02LCDreg(0x01,gAUBL02_VCOM_DC);
             //   sendtoAUBL02LCDreg(0x01,gAUBL02_VCOM_DC);
            break;
        case PINMUX_LCDMODE_YUV640:
            sendtoAUDN01LCDreg(0x00,0x43);//sendtoAUBL02LCDreg(0x00,0x0F);
            //sendtoAUDN01LCDreg(0x40,0x03);
            sendtoAUDN01LCDreg(0x04,0x4B);  //YUV 640x240
            sendtoAUDN01LCDreg(0x05,0x5D);
            break;

        //#NT#2007/03/07#Shirley Cheng -end
        }
    }
    //#NT#2008/09/10#Niven -end
}

/*
  Open AU027DN01 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
void OpenAUDN01(LCDSIZE *psizeDisplay)
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
    case PINMUX_LCDMODE_RGBD360:
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

        ide_set_hor_timing(0x01, 1716-1, 241, 1681);
        ide_set_ver_timing(0x01, 0x106, 0x15, 0x105, 0x15, 0x105);
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

        psizeDisplay -> uiWidth = AU_DN01_UPS052_Buf_width;
        psizeDisplay -> uiHeight = AU_DN01_UPS052_Buf_height;
        psizeDisplay -> uiWinWidth = AU_DN01_UPS052_Win_width;
        psizeDisplay -> uiWinHeight = AU_DN01_UPS052_Win_height;
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
        //ide_set_en(1);
        //ide_set_load();
        /* //720x240
        ide_set_hor_timing(0x00, 0x359, 0x7D, 0x34D);
        ide_set_ver_timing(0x00, 0x105, 0x0D, 0xFE, 0x0D, 0xFE);  */
           //640x240
        ide_set_hor_timing(0x00, 0x30B, 0x77, 0x2F7);
        ide_set_ver_timing(0x00, 0x105, 0x15, 0x105, 0x15, 0x105);

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

        psizeDisplay->uiWidth       = AU_DN01_YUV640_Buf_width;
        psizeDisplay->uiHeight      = AU_DN01_YUV640_Buf_height;
        psizeDisplay->uiWinWidth    = AU_DN01_YUV640_Win_width;
        psizeDisplay->uiWinHeight   = AU_DN01_YUV640_Win_height;
        break;
    //#NT#2007/03/07#Shirley Cheng -end

    }

    //#NT#2009/11/04#Klins Chen#[0007606] -begin
    //#NT#Remove unnecessary dummy delay
    ide_set_en(TRUE);
    // Avoid vsync active area
    ide_wait_frame_end();
    //#NT#2009/11/04#Klins Chen#[0007606] -end

    Init_AUDN01_LCD();
}

/*
  Close AU027DN01 driver

  Release AU027DN01 driver, private function for internal used

*/
void CloseAUDN01(void)
{
    // Disable IDE
    //#NT#2009/11/04#Klins Chen#[0007606] -begin
    //#NT#Modify close flow
    ide_set_en(FALSE);
    ide_wait_frame_end();
    //#NT#2009/11/04#Klins Chen#[0007606] -end

    // Change to standby mode
    //#NT#2008/09/25#Niven Cheng -begin
    //Fix change to standby mode register setting not match with spec.
    sendtoAUDN01LCDreg(0x05,0x5E);
    //#NT#2008/09/25#Niven Cheng -end

    delay(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}
//@}
