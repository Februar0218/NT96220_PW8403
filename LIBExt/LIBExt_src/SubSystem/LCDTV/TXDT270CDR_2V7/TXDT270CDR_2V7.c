/*
    @file       TXDT270CDR_2V7.c

    @ingroup    mILIBLCDTXDT270CDR_2V7

    @brief      LCD Driver for TXDT270CDR_2V7 panel

    @note       Nothing.

    @author

    @date       2011/05/13

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

*/

/**
@addtogroup mILIBLCDTXDT270CDR_2V7

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
#include "Utility.h"


#define TXDT270CDR_2V7_UPS051_Buf_width       960
#define TXDT270CDR_2V7_UPS051_Buf_height      240
#define TXDT270CDR_2V7_UPS051_Win_width       960
#define TXDT270CDR_2V7_UPS051_Win_height      240

#if 1
#define TXDT270CDR_2V7_UPS052_Buf_width       320
#define TXDT270CDR_2V7_UPS052_Buf_height      240
#define TXDT270CDR_2V7_UPS052_Win_width       320
#define TXDT270CDR_2V7_UPS052_Win_height      240
#else
#define TXDT270CDR_2V7_UPS052_Buf_width       360
#define TXDT270CDR_2V7_UPS052_Buf_height      240
#define TXDT270CDR_2V7_UPS052_Win_width       360
#define TXDT270CDR_2V7_UPS052_Win_height      240
#endif

#define TXDT270CDR_2V7_CCIR656_Buf_width      720
#define TXDT270CDR_2V7_CCIR656_Buf_height     480
#define TXDT270CDR_2V7_CCIR656_Win_width      720
#define TXDT270CDR_2V7_CCIR656_Win_height     480

//Add YUV 640 mode support
#define TXDT270CDR_2V7_YUV640_Buf_width       640
#define TXDT270CDR_2V7_YUV640_Buf_height      240
#define TXDT270CDR_2V7_YUV640_Win_width       640
#define TXDT270CDR_2V7_YUV640_Win_height      240

//UINT8 gAUCN01_VCOM_DC = 0x18;

//function declaration
static void delay(UINT32 times);
static void sendToLcdReg_sif(UINT8 addr, UINT8 value);
static void sendToLcdReg_gpio(UINT8 addr, UINT8 value);
static void sendToLcdReg(UINT8 addr, UINT8 value);

static void Init_TXDT270CDR_2V7_LCD(void);
static void OpenTXDT270CDR_2V7(LCDSIZE *psizeDisplay);
static void CloseTXDT270CDR_2V7(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;

static const UINT8 gamma[] =
{

      0, 11, 23, 40, 55, 74, 93,112,
    130,150,169,187,203,219,234,244,
    255
};

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

        case PINMUX_LCDMODE_RGBD320:
            LCD_DeviceObj.fIDEDstCLK = 24.54;
            break;
        case PINMUX_LCDMODE_YUV640:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        }
    }

    OpenTXDT270CDR_2V7(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseTXDT270CDR_2V7();
}

/**
  delay time period

  @param times : time period

*/
static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/**
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendToLcdReg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    //debug_msg("\r\n\r\n****** sendToLcdReg_sif(%x, %x)\r\n\r\n", addr, value);

    uiSIFData = ((((UINT8)addr << 8)|(UINT8)value ) << 16);
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/**
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendToLcdReg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    //debug_msg("\r\n\r\n****** sendToLcdReg_gpio(%x, %x)\r\n\r\n", addr, value);

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

/**
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
static void sendToLcdReg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendToLcdReg_sif(addr, value);
    }
    else
    {
         sendToLcdReg_gpio(addr, value);
    }
}

/**
    Initial LCD of AU027DN01, configure SIF intarface and send data to panel register to configure

*/
static void Init_TXDT270CDR_2V7_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = SIF_MODE_0000;
        //SIFCtrl.bSyncEn       = FALSE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        //SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;

        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    switch (LCD_DeviceObj.uiLCDMode)
    {
    case PINMUX_LCDMODE_RGB_SERIAL:
        //sendToLcdReg(0x00,0x03);//sendtoAUBL02LCDreg(0x00,0x0F);
        //sendToLcdReg(0x40,0x03);
        sendToLcdReg(0x04,0x0B);  //UPS052 360x240 --> 2B
        sendToLcdReg(0x05,0x5D);
        break;

    case PINMUX_LCDMODE_RGBD320:
        debug_msg("\r\n\r\n****** Init_TXDT270CDR_2V7_LCD()\r\n\r\n");
        sendToLcdReg(0x05,0x1e);  //set power mode (standby)
        TimerDelayMs(150);
        sendToLcdReg(0x05,0x5d);
        TimerDelayMs(150);
        sendToLcdReg(0x0b,0x81); //set source driver output driving selection
        ////sendToLcdReg(0x01,0x8B); // 0x9D); //adjusting VCOM DC
        sendToLcdReg(0x01,0x90); // adjusting VCOM DC
        ////sendToLcdReg(0x00,0x01); // adjusting VCOM AC
        ////sendToLcdReg(0x00,0x05); // adjusting VCOM AC
        sendToLcdReg(0x00,0x07); // adjusting VCOM AC
        sendToLcdReg(0x05,0x5d); // set power mode(standby)
        sendToLcdReg(0x06,0x15);  //set VBLK
        sendToLcdReg(0x07,0x00); // set HBLK
        sendToLcdReg(0x97,0x03);
        sendToLcdReg(0x96,0x02);
        sendToLcdReg(0x04,0x14);  // 0x1f);  //set scan &source output direction

        // set gamma
        sendToLcdReg(0x16,0x56); //(Gamma offset function turn off; using default gamma 2.2)
        ////sendToLcdReg(0x16,0x52); //(Gamma offset function turn on)
        sendToLcdReg(0x17,0xd9);
        sendToLcdReg(0x18,0xdc);
        sendToLcdReg(0x19,0xff);
        sendToLcdReg(0x1a,0xab);

        #if 0
        sendToLcdReg(0x0d,0x44); //RGB contrast
        sendToLcdReg(0x0e,0x4c); //R contrast
        sendToLcdReg(0x0f,0x44); //R brightness
        sendToLcdReg(0x10,0x34); //B contrast
        sendToLcdReg(0x11,0x40); //B brightness
        #else
        sendToLcdReg(0x03,0x44); //RGB brightness
        ////sendToLcdReg(0x0d,0x42); //RGB contrast
        sendToLcdReg(0x0d,0x38); //RGB contrast
        ////sendToLcdReg(0x0e,0x48); //R contrast
        sendToLcdReg(0x0e,0x50); //R contrast
        ////sendToLcdReg(0x0f,0x48); //R brightness
        //sendToLcdReg(0x0f,0x4c); //R brightness
        sendToLcdReg(0x0f,0x44); //R brightness
        ////sendToLcdReg(0x10,0x48); //B contrast
        sendToLcdReg(0x10,0x4c); //B contrast
        ////sendToLcdReg(0x11,0x40); //B brightness
        sendToLcdReg(0x11,0x44); //B brightness
        #endif

        sendToLcdReg(0xac,0xff);
        sendToLcdReg(0xb3,0x01);//add 100111
        sendToLcdReg(0xb5,0x06);
        //-------
        break;

    case PINMUX_LCDMODE_YUV640:
        sendToLcdReg(0x00,0x43); //sendtoAUBL02LCDreg(0x00,0x0F);
        //sendToLcdReg(0x40,0x03);
        sendToLcdReg(0x04,0x4B); //YUV 640x240
        sendToLcdReg(0x05,0x5D);
        break;
    }
}

/**
  Open TXDT270CDR_2V7 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
void OpenTXDT270CDR_2V7(LCDSIZE *psizeDisplay)
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
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL1Freq() / 1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
    }
    else
    {
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL2Freq() / 1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }

    // Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case PINMUX_LCDMODE_RGBD320:
        debug_msg("\r\n\r\n****** OpenTXDT270CDR_2V7()\r\n\r\n");

        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_RGB_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv-1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_setDithering(FALSE, FALSE);
        ide_setDevice(DISPLAY_DEVICE_AU);

        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_R);
        ide_setEven(IDE_LCD_B);

        ide_setHsInv(TRUE);
        ide_setVsInv(TRUE);
        ide_setHvldInv(FALSE);
        ide_setVvldInv(FALSE);
        ide_setClkInv(TRUE);
        ide_setFldInv(FALSE);
        ide_setRgbd(TRUE);
        ide_setThrough(FALSE);

        ide_setHorTiming(1, 1560, 241, 241+1280);
        ide_setVerTiming(1, 262, 21, 21+240, 21, 21+240);
        ide_setInterlace(0x00);
        ide_setStartField(0x00);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ////ide_setCmults(0x40);
        ide_setCmults(0x58);
        ////ide_setCmults(0x50);
        ide_setCex(FALSE);

        //ide_set_clamp(0x01);
        ide_setTvPowerDown(TRUE);
        ide_fillGamma((UINT8*)gamma);

        //ide_set_sif_startend(0x00, 0x00);

        psizeDisplay -> uiWidth = TXDT270CDR_2V7_UPS052_Buf_width;
        psizeDisplay -> uiHeight = TXDT270CDR_2V7_UPS052_Buf_height;
        psizeDisplay -> uiWinWidth = TXDT270CDR_2V7_UPS052_Win_width;
        psizeDisplay -> uiWinHeight = TXDT270CDR_2V7_UPS052_Win_height;
        break;
    }

    ide_setEn(TRUE);
    //avoid vsync active area
    ide_waitFrameEnd();

    Init_TXDT270CDR_2V7_LCD();
}

/**
  Close AU027DN01 driver

  Release AU027DN01 driver, private function for internal used

*/
void CloseTXDT270CDR_2V7(void)
{
    debug_msg("\r\n\r\n****** CloseTXDT270CDR_2V7()\r\n\r\n");

    // Disable IDE
    ide_setEn(FALSE);
    ide_waitFrameEnd();

    // Change to standby mode
    sendToLcdReg(0x2B,0x00);
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

void TXDT270CDR_2V7OpenLcdBacklightReg(void)
{
    sendToLcdReg(0x05,0x5f);
}

//#NT#For LCD rotation
void LCD_Rotate(UINT32 bRotate)
{
    /*
    if (bRotate)
    {
        sendToLcdReg(0x04,0x18);
        ide_wait_frame_end();
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(2);
        ide_set_load();
    }
    else
    {
        sendToLcdReg(0x04,0x1B);
        ide_wait_frame_end();
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(2);
        ide_set_load();
    }
    */
}

//#NT#2010/04/26#Chris Chung -begin
//#NT#add for compile issue
void LCD_Standby(BOOL bStadnby)
{
    debug_msg("\r\n\r\n****** LCD_Standby()\r\n\r\n");
    sendToLcdReg(0x2B,bStadnby);
}
//#NT#2010/04/26#Chris Chung -end

void LCD_WriteReg(UINT8 addr, UINT8 value)
{
    sendToLcdReg(addr,value);
}

//@}
