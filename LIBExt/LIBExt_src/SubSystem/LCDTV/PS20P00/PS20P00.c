/*
    @file       PS20P00.c

    @ingroup    mILIBLCDPS20P00

    @brief      LCD Driver of PS20P00 panel

    @note       Nothing.

    @author

    @date       2011/06/20

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

*/

/**
@addtogroup mILIBLCDPS20P00

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0000).

@endverbatim

@{
*/
#include "OSCommon.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "pll.h"
#include "Utility.h"

#define PS20P00_RGBD_Buf_width          320
#define PS20P00_RGBD_Buf_height         240
#define PS20P00_RGBD_Win_width          320
#define PS20P00_RGBD_Win_height         240

#define PS20P00_RAW_Buf_width           160
#define PS20P00_RAW_Buf_height          240
#define PS20P00_RAW_Win_width           160
#define PS20P00_RAW_Win_height          240

#define PS20P00_CCIR656_Buf_width       720
#define PS20P00_CCIR656_Buf_height      480
#define PS20P00_CCIR656_Win_width       720
#define PS20P00_CCIR656_Win_height      480

//function declaration
static void delay(UINT32 times);
static void sendtoLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value);
static void readLCDreg_gpio(UINT8 addr, UINT8 *value);
static void sendtoLCDreg(UINT8 addr, UINT8 value);

static void Init_PS20P00_LCD(void);
static void OpenPS20P00(LCDSIZE *psizeDisplay);
static void ClosePS20P00(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;

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
            LCD_DeviceObj.fIDEDstCLK = 24.55; // RGBD 320
            break;

        case PINMUX_LCDMODE_RGB_THROUGH:
            LCD_DeviceObj.fIDEDstCLK = 9.7;
            break;

        case PINMUX_LCDMODE_CCIR656:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        }
    }

    OpenPS20P00(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    ClosePS20P00();
}

/**
    Write LCD Internal Register

    Optional Implementation

    @param[in] addr     LCD panel register address
    @param[in] value    Write value

    @return void
*/
void LCD_RegWrite(UINT32 addr, UINT32 value)
{
    sendtoLCDreg(addr, value);
}

void LCD_RegRead(UINT8 addr, UINT8 *value)
{
    readLCDreg_gpio(addr, value);
}

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
static void sendtoLCDreg_sif(UINT8 addr, UINT8 value)
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
static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value)
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
        if (data & 0x8000)
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(200);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);

        data <<= 1;
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

void readLCDreg_gpio(UINT8 addr, UINT8 *value)
{
    UINT32 data;
    UINT32 j;
    UINT32 temp;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)(addr | 0x08) << 8;

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    delay(500);

    for(j = 16; j > 8; j--)
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

    *value = 0;
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_INPUT);

    delay(200);
    for(j = 0; j < 8; j++)
    {
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        temp = gpio_getPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        *value |= temp << (7 - j);
        delay(200);
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

/*
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
static void sendtoLCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoLCDreg_sif(addr, value);
    }
    else
    {
         sendtoLCDreg_gpio(addr, value);
    }
}

/*
    Initial LCD of AUCN01, configure SIF intarface and send data to panel register to configure

*/
static void Init_PS20P00_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = SIF_MODE_0000;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;

        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    if(LCD_DeviceObj.fpPostAdjusement)
    {
        LCD_DeviceObj.fpPostAdjusement();
    }
    else
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case PINMUX_LCDMODE_RGBD320:
            sendtoLCDreg(0x00, 0x07);
            delay(100);
            sendtoLCDreg(0x00, 0x0F); // power enable
            sendtoLCDreg(0x60, 0x01); // RGBD 24.54MHz
            //sendtoLCDreg(0x40, 0x03);
            //sendtoLCDreg(0xC0, 0x06);
            //sendtoLCDreg(0xE0, 0x13);
            break;

        case PINMUX_LCDMODE_RGB_THROUGH:
            sendtoLCDreg(0x00, 0x07);
            delay(100);
            sendtoLCDreg(0x00, 0x0F); // power enable
            sendtoLCDreg(0x40, 0x03);
            sendtoLCDreg(0x60, 0x00); // RAW 9.7MHz
            sendtoLCDreg(0xC0, 0x06);
            break;

        case PINMUX_LCDMODE_CCIR656:
            sendtoLCDreg(0x00, 0x07);
            delay(100);
            sendtoLCDreg(0x00, 0x0F); // power enable
            sendtoLCDreg(0x60, 0x07); // CCIR656 27MHz
            break;

        default:
            debug_err(("Init_PS20P00_LCD() not support %d\r\n", LCD_DeviceObj.uiLCDMode));
            break;
        }
    }
}

/*
  Open AUCN01 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenPS20P00(LCDSIZE *psizeDisplay)
{
    UINT8           uiClockDiv;

    if (ide_open() != E_OK)
        return;

    // Disable IDE
    if(ide_getEn())
    {
        ide_setEn(FALSE);
        ide_waitFrameEnd();
    }

    // Set IDE Clock Source
    if (LCD_DeviceObj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL1Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
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

        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_setDithering(FALSE, FALSE);
        ide_setDevice(DISPLAY_DEVICE_AU);
        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_R);
        ide_setEven(IDE_LCD_G);
        ide_setHsInv(TRUE);
        ide_setVsInv(TRUE);
        ide_setHvldInv(FALSE);
        ide_setVvldInv(FALSE);
        ide_setClkInv(TRUE);
        ide_setFldInv(FALSE);
        ide_setRgbd(TRUE);
        ide_setThrough(FALSE);

        ide_setHorTiming(0, 1559, 253, 1533);
        //ide_setVerTiming(1, 261, 12, 252, 12, 252);
        ide_setVerTiming(1, 261, 12+2, 252+2, 12+2, 252+2);

        ide_setInterlace(FALSE);
        ide_setStartField(FALSE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(FALSE);

        ide_setTvPowerDown(TRUE);

        psizeDisplay->uiWidth       = PS20P00_RGBD_Buf_width;
        psizeDisplay->uiHeight      = PS20P00_RGBD_Buf_height;
        psizeDisplay->uiWinWidth    = PS20P00_RGBD_Win_width;
        psizeDisplay->uiWinHeight   = PS20P00_RGBD_Win_height;
        break;

    case PINMUX_LCDMODE_RGB_THROUGH:
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_RGB_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        //ide_set_icst(0);
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_setDithering(FALSE, FALSE);
        ide_setDevice(DISPLAY_DEVICE_CASIO2G);
        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_R);
        ide_setEven(IDE_LCD_G);
        ide_setHsInv(TRUE);
        ide_setVsInv(TRUE);
        ide_setHvldInv(FALSE);
        ide_setVvldInv(FALSE);
        ide_setClkInv(TRUE);
        ide_setFldInv(FALSE);
        ide_setRgbd(FALSE);
        ide_setThrough(TRUE);

        ide_setHorTiming(0x00, 616-1, 100, (480+100)-1);
        ide_setVerTiming(0x05, 262, 18, (240+18)-1, 18, (240+18)-1);

        ide_setInterlace(FALSE);
        ide_setStartField(FALSE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(FALSE);

        ide_setTvPowerDown(TRUE);

        psizeDisplay->uiWidth       = PS20P00_RAW_Buf_width;
        psizeDisplay->uiHeight      = PS20P00_RAW_Buf_height;
        psizeDisplay->uiWinWidth    = PS20P00_RAW_Win_width;
        psizeDisplay->uiWinHeight   = PS20P00_RAW_Win_height;
        break;

    case PINMUX_LCDMODE_CCIR656:
        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_CCIR656|PINMUX_LCD_SEL_DE_ENABLE);
        //pinmux_select_lcd(PINMUX_LCD_CCIR656);
        //pinmux_enable_de();

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        //ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_configOutputLimit(1);
        ide_setDithering(FALSE, FALSE);
        ide_setDevice(DISPLAY_DEVICE_CCIR656);
        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_R);
        ide_setEven(IDE_LCD_G);
        ide_setHsInv(FALSE);
        ide_setVsInv(FALSE);
        ide_setHvldInv(FALSE);
        ide_setVvldInv(FALSE);
        ide_setClkInv(TRUE);
        ide_setFldInv(FALSE);
        ide_setRgbd(FALSE);
        ide_setThrough(FALSE);

        ide_setHorTiming(0, 857, 136, 855);
        ide_setVerTiming(5, 524, 36, 515, 37, 516);

        ide_setDigitalTiming(524, 22, 262, 284, 4, 266);

        ide_setInterlace(TRUE);
        ide_setStartField(TRUE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(FALSE);

        ide_setTvPowerDown(TRUE);

        //Subpixel
        ide_setSubpixel(TRUE, FALSE, FALSE, FALSE);
        ide_setSubpixel(FALSE, FALSE, FALSE, FALSE);

        psizeDisplay->uiWidth       = PS20P00_CCIR656_Buf_width;
        psizeDisplay->uiHeight      = PS20P00_CCIR656_Buf_height;
        psizeDisplay->uiWinWidth    = PS20P00_CCIR656_Win_width;
        psizeDisplay->uiWinHeight   = PS20P00_CCIR656_Win_height;
        break;

    default:
        debug_err(("OpenPS20P00() Not support %d mode\r\n",LCD_DeviceObj.uiLCDMode));
        break;
    }

    ide_setEn(TRUE);
    //avoid vsync active area
    ide_waitFrameEnd();

    Init_PS20P00_LCD();
}

/*
  Close AUCN01 driver

  Release AUCN01 driver, private function for internal used
*/
static void ClosePS20P00(void)
{
    // Disable IDE
    ide_setEn(FALSE);
    ide_waitFrameEnd();

    // Change to standby mode
    //Fix change to standby mode register setting not match with spec.
    sendtoLCDreg(0x00,0x08);
    ide_setDevNo(FALSE);

    delay(0x1000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    ide_close();

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}
//@}
