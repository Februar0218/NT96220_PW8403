/*
    @file       RZWT27P02.c

    @ingroup    RZWT27P02

    @brief      AU LCD Driver of RZWT27P02 panel

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDRZWT27P02

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0100).

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

#define RZW_T27P02_UPS051_Buf_width       960  /*480 For Analog signal*/
#define RZW_T27P02_UPS051_Buf_height      240
#define RZW_T27P02_UPS051_Win_width       960  /*480 For Analog signal*/
#define RZW_T27P02_UPS051_Win_height      240

#define RZW_T27P02_THROUGH_Buf_width      160
#define RZW_T27P02_THROUGH_Buf_height     240
#define RZW_T27P02_THROUGH_Win_width      160
#define RZW_T27P02_THROUGH_Win_height     240

#define RZW_T27P02_UPS052_Buf_width       320
#define RZW_T27P02_UPS052_Buf_height      240
#define RZW_T27P02_UPS052_Win_width       320
#define RZW_T27P02_UPS052_Win_height      240

#define RZW_T27P02_CCIR656_Buf_width      720
#define RZW_T27P02_CCIR656_Buf_height     480
#define RZW_T27P02_CCIR656_Win_width      720
#define RZW_T27P02_CCIR656_Win_height     480

#define RZW_T27P02_YUV640_Buf_width       640
#define RZW_T27P02_YUV640_Buf_height      240
#define RZW_T27P02_YUV640_Win_width       640
#define RZW_T27P02_YUV640_Win_height      240


//0: YUV or CCIR601     1: CCIR656
#define LCDPTV 0

#if (LCDPTV==0)

#define LCDPTV_720 1
#if LCDPTV_720
#define RZW_T27P02_LCDTV_Buf_width       720
#define RZW_T27P02_LCDTV_Buf_height      480
#define RZW_T27P02_LCDTV_Win_width       720
#define RZW_T27P02_LCDTV_Win_height      480
#else
#define RZW_T27P02_LCDTV_Buf_width       640
#define RZW_T27P02_LCDTV_Buf_height      448
#define RZW_T27P02_LCDTV_Win_width       640
#define RZW_T27P02_LCDTV_Win_height      448
#endif
#endif

// Function declaration
static void delay(UINT32 times);
static void sendtoRZWLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoRZWLCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoRZWLCDreg(UINT8 addr, UINT8 value);

static void Init_RZW_LCD(void);
static void OpenRZW(LCDSIZE *psizeDisplay);
static void CloseRZW(void);

// Static variable
static LCD_DEVICE_OBJ   LCD_DeviceObj;


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

    debug_err(("RZW LCD \r\n"));
    if (LCD_DeviceObj.fIDEDstCLK == 0)
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
            case PINMUX_LCDMODE_RGBD320:
                LCD_DeviceObj.fIDEDstCLK = 27;
                break;
            case PINMUX_LCDMODE_CCIR656:
                LCD_DeviceObj.fIDEDstCLK = 27;
                break;
            case PINMUX_LCDMODE_CCIR601:
            case PINMUX_LCDMODE_YUV640:
                LCD_DeviceObj.fIDEDstCLK = 24.55;
                break;
            //case LCD_MODE_LCDTV:
            //    LCD_DeviceObj.fIDEDstCLK = 27;
            //    break;
            case PINMUX_LCDMODE_RGB_SERIAL:
            case PINMUX_LCDMODE_RGB_THROUGH:
                LCD_DeviceObj.fIDEDstCLK = 9.7;
                break;
        }
    }

    OpenRZW(psizeDisplay);
}

/**
  Close LCD driver, public function for global use

  Release LCD driver

*/
void ClosePanel(void)
{
    CloseRZW();
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
    sendtoRZWLCDreg(addr, value);
}


/*
  delay time period

  @param times : time period

*/
void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/**
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoRZWLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = (((UINT32)addr << 12)|(UINT32)value ) << 16;
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/**
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoRZWLCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    //data = (UINT32)addr << 8 | (UINT32)value;
    data = (UINT32)addr << 8 | (UINT32)value;//#Linfg@0322

    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j=16; j>0; j--)
    {
        if (data & 0x8000)
        {
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        }
        else
        {
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        }

        delay(100);

        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay(200);

        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay(100);

        data <<= 1;
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

/**
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
static void sendtoRZWLCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
         sendtoRZWLCDreg_sif(addr, value);
    }
    else
    {
         sendtoRZWLCDreg_gpio(addr, value);
    }
}

/**
    Initial LCD of RZWTD24THEB5, configure SIF intarface and send data to panel register to configure

*/
static void Init_RZW_LCD(void)
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
            sendtoRZWLCDreg(0x04,0x1b);////8-bit RGB interface,VDIR/HDIR   0B
            break;
        case PINMUX_LCDMODE_CCIR656:
            break;
        case PINMUX_LCDMODE_CCIR601:
            break;
        //case LCD_MODE_LCDTV:
        //    break;
        case PINMUX_LCDMODE_RGB_SERIAL:
            sendtoRZWLCDreg(0x04,0x08);////8-bit RGB interface,VDIR/HDIR   0B
            break;
        case PINMUX_LCDMODE_RGB_THROUGH:
            break;
        case PINMUX_LCDMODE_YUV640:
            sendtoRZWLCDreg(0x04,0x08);//wr_cmd(0x04);wr_data(0x08);//8-bit RGB interface,VDIR/HDIR   0B        break;
        break;
         default:
        //debug_err(("Init_AUCN01_LCD() not support %d\r\n", LCD_DeviceObj.uiLCDMode));
            break;
    }
    // Set LCD register
    sendtoRZWLCDreg(0x00,0x09);//wr_cmd(0x00);wr_data(0x09);//VCOMAC  00   04
    sendtoRZWLCDreg(0x01,0x90);//wr_cmd(0x01);wr_data(0x90);//VCOMDC  95   92
    sendtoRZWLCDreg(0x03,0x40);//wr_cmd(0x03);wr_data(0x40);//brightness setting
    //sendtoRZWLCDreg(0x04,0x08);//wr_cmd(0x04);wr_data(0x08);//8-bit RGB interface,VDIR/HDIR   0B
    sendtoRZWLCDreg(0x05,0x5f);//wr_cmd(0x05);wr_data(0x5F);//GRB STB
    sendtoRZWLCDreg(0x06,0x15);//wr_cmd(0x06);wr_data(0x15);
    sendtoRZWLCDreg(0x07,0x46);//wr_cmd(0x07);wr_data(0x46);
    sendtoRZWLCDreg(0x08,0x00);//wr_cmd(0x08);wr_data(0x00);
    sendtoRZWLCDreg(0x0b,0x83);//wr_cmd(0x0B);wr_data(0x83);
    sendtoRZWLCDreg(0x0c,0x06);//wr_cmd(0x0C);wr_data(0x06);

    sendtoRZWLCDreg(0x0d,0x40);//wr_cmd(0x0D);wr_data(0x40);//contrast setting
    sendtoRZWLCDreg(0x0e,0x40);//wr_cmd(0x0E);wr_data(0x40);
    sendtoRZWLCDreg(0x0f,0x40);//wr_cmd(0x0F);wr_data(0x40);
    sendtoRZWLCDreg(0x10,0x40);//wr_cmd(0x10);wr_data(0x40);
    sendtoRZWLCDreg(0x11,0x40);//wr_cmd(0x11);wr_data(0x40);

    sendtoRZWLCDreg(0x12,0x00);//wr_cmd(0x12);wr_data(0x00);
    sendtoRZWLCDreg(0x13,0x01);//wr_cmd(0x13);wr_data(0x01);//Delta color filter   01

    sendtoRZWLCDreg(0x16,0x04);//wr_cmd(0x16);wr_data(0x04);//Gamma setting  2.2
    sendtoRZWLCDreg(0x17,0x54);//wr_cmd(0x17);wr_data(0x54);
    sendtoRZWLCDreg(0x18,0x54);//wr_cmd(0x18);wr_data(0x54);
    sendtoRZWLCDreg(0x19,0x43);//wr_cmd(0x19);wr_data(0x43);
    sendtoRZWLCDreg(0x1A,0x54);//wr_cmd(0x1A);wr_data(0x54);

    sendtoRZWLCDreg(0x95,0x00);//wr_cmd(0x95);wr_data(0x00);
    sendtoRZWLCDreg(0x96,0x03);//wr_cmd(0x96);wr_data(0x03);
    sendtoRZWLCDreg(0x97,0x02);//wr_cmd(0x97);wr_data(0x02);

        }
}

/**
  Open GPM1050 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenRZW(LCDSIZE *psizeDisplay)
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

        //ide_setHorTiming(0x00, 0x617, 0xFD, 0x5FD);
        //ide_setVerTiming(0x01, 0x105, 0x0C, 0xFC, 0x0C, 0xFC);


          //ide_set_hor_timing(0, 1306, 2, 1304);
        ide_set_hor_timing(1, 1559, 241, 1280+241-1);
        //ide_set_ver_timing(0, 243, 1, 241, 2, 242);
        ide_set_ver_timing(1, 261, 21, 240+21-1, 21, 240+21-1);

        ide_setInterlace(FALSE);
        ide_setStartField(FALSE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(FALSE);

        ide_setTvPowerDown(TRUE);


        psizeDisplay->uiWidth      = RZW_T27P02_UPS052_Buf_width;
        psizeDisplay->uiHeight     = RZW_T27P02_UPS052_Buf_height;
        psizeDisplay->uiWinWidth   = RZW_T27P02_UPS052_Win_width;
        psizeDisplay->uiWinHeight  = RZW_T27P02_UPS052_Win_height;
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

        ide_setHorTiming(0x00, 0x359, 0x88, 0x357);
        ide_setVerTiming(0x05, 0x20C, 0x24, 0x203, 0x25, 0x204);

        ide_setDigitalTiming(0x20C, 0x16, 0x106, 0x11C, 0x04, 0x10A);

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

        psizeDisplay->uiWidth = RZW_T27P02_CCIR656_Buf_width;
        psizeDisplay->uiHeight = RZW_T27P02_CCIR656_Buf_height;
        psizeDisplay->uiWinWidth = RZW_T27P02_CCIR656_Win_width;
        psizeDisplay->uiWinHeight = RZW_T27P02_CCIR656_Win_height;
        break;

    case PINMUX_LCDMODE_YUV640:
        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        // Set DCLK
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK);
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);


        //ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_setDithering(FALSE, FALSE);
        ide_setDevice(DISPLAY_DEVICE_TOPPOLY);
        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_G);
        ide_setEven(IDE_LCD_B);
        ide_setHsInv(TRUE);
        ide_setVsInv(TRUE);
        ide_setHvldInv(FALSE);
        ide_setVvldInv(FALSE);
        ide_setClkInv(TRUE);
        ide_setFldInv(FALSE);
        ide_setRgbd(FALSE);
        ide_setThrough(FALSE);

        ide_setHorTiming(0x00, 0x30C, 0x7E, 0x2FE);
        ide_setVerTiming(0x00, 0x105, 0x0D, 0x0FC, 0x0D, 0xFC);
        ide_setInterlace(FALSE);
        ide_setStartField(FALSE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(0x00);

        ide_setTvPowerDown(TRUE);

        //Subpixel
        ide_setSubpixel(TRUE, FALSE, FALSE, FALSE);
        ide_setSubpixel(FALSE, FALSE, FALSE, FALSE);

        psizeDisplay->uiWidth       = RZW_T27P02_YUV640_Buf_width;
        psizeDisplay->uiHeight      = RZW_T27P02_YUV640_Buf_height;
        psizeDisplay->uiWinWidth    = RZW_T27P02_YUV640_Win_width;
        psizeDisplay->uiWinHeight   = RZW_T27P02_YUV640_Win_height;
        break;

    case PINMUX_LCDMODE_CCIR601:
        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_CCIR601);
        //pinmux_select_lcd(PINMUX_LCD_CCIR601);

        // Set DCLK
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK);
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);


        //ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_setDithering(FALSE, FALSE);
        ide_setDevice(DISPLAY_DEVICE_CCIR601);
        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_G);
        ide_setEven(IDE_LCD_B);
        ide_setHsInv(TRUE);
        ide_setVsInv(TRUE);
        ide_setHvldInv(FALSE);
        ide_setVvldInv(FALSE);
        ide_setClkInv(TRUE);
        ide_setFldInv(FALSE);
        ide_setRgbd(FALSE);
        ide_setThrough(FALSE);

        ide_setHorTiming(0x00, 0x30C, 0x7E, 0x2FE);
        ide_setVerTiming(0x00, 0x105, 0x0D, 0x0FC, 0x0D, 0xFC);
        ide_setInterlace(FALSE);
        ide_setStartField(FALSE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(0x00);

        ide_setTvPowerDown(TRUE);

        //Subpixel
        ide_setSubpixel(TRUE, FALSE, FALSE, FALSE);
        ide_setSubpixel(FALSE, FALSE, FALSE, FALSE);

        psizeDisplay->uiWidth       = RZW_T27P02_YUV640_Buf_width;
        psizeDisplay->uiHeight      = RZW_T27P02_YUV640_Buf_height;
        psizeDisplay->uiWinWidth    = RZW_T27P02_YUV640_Win_width;
        psizeDisplay->uiWinHeight   = RZW_T27P02_YUV640_Win_height;
        break;

    #if 0
    case LCD_MODE_LCDTV:
        // Set DCLK
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK)/2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);


        #if (LCDPTV == 0)

        #if 0
        //Pretend CCIR601 as YUV to test LCD+CCIR601
        pinmux_select_lcd(PINMUX_LCD_CCIR601);
        ide_setDevice(DISPLAY_DEVICE_CCIR601);
        #else
        pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);
        ide_setDevice(DISPLAY_DEVICE_TOPPOLY);
        #endif


        ide_setHsInv(TRUE);
        ide_setVsInv(TRUE);
        ide_setStartField(FALSE);
        ide_configIcst(FALSE, 0);
        ide_setRgbd(FALSE);

        #elif (LCDPTV == 1)
        pinmux_select_lcd(PINMUX_LCD_CCIR656);
        ide_setDevice(DISPLAY_DEVICE_CCIR656);
        ide_configOutputLimit(1);
        ide_setInterlace(TRUE);
        ide_setStartField(TRUE);
        ide_setDigitalTiming(0x20C, 0x16, 0x106, 0x11C, 0x04, 0x10A);
        ide_setHsInv(FALSE);
        ide_setVsInv(FALSE);
        ide_configIcst(FALSE, 0);
        ide_setRgbd(FALSE);



        #elif(LCDPTV == 2)
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);
        ide_setDevice(DISPLAY_DEVICE_CASIO2G);
        ide_setHsInv(TRUE);
        ide_setVsInv(TRUE);
        ide_setStartField(FALSE);
        ide_setRgbd(TRUE);

        ide_configIcst(TRUE, CST_YCBCR2RGB);
        #endif


        ide_setDithering(FALSE, FALSE);
        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_G);
        ide_setEven(IDE_LCD_B);
        ide_setHvldInv(FALSE);
        ide_setVvldInv(FALSE);
        ide_setClkInv(TRUE);
        ide_setFldInv(FALSE);
        ide_setThrough(FALSE);

        #if 0
        ide_setHorTiming(0x7F, 0x6B3, 0x144, 0x69D);
        ide_setVerTiming(0x05, 0x20C, 0x32, 0x1f9, 0x33, 0x1fa);
        #endif

        #if (LCDPTV == 1)
        // CCIR656's blanking start must be multiple of 4
        ide_setHorTiming(0x7F, 0x6B3, 0xFA+2, 0x699+2);
        ide_setVerTiming(0x05, 0x20C, 0x16, 0x20B, 0x17, 0x20C);
        #else
            #if ((LCDPTV == 0)&&(!LCDPTV_720))
            //ide_setHorTiming(0x7F, 0x6B3, 0x144, 0x69D);
            //ide_setVerTiming(0x05, 0x20C, 0x32, 0x1f9, 0x33, 0x1fa);
            ide_setHorTiming(0x7F, 0x6B3, 0xFA+4, 0x699+4);
            ide_setVerTiming(0x05, 0x20C, 0x16, 0x20B, 0x17, 0x20C);
            ide_setConstantWindowOffset(0, 0);
            #else
            ide_setHorTiming(0x7F, 0x6B3, 0xFA+4, 0x699+4);
            ide_setVerTiming(0x05, 0x20C, 0x16, 0x20B, 0x17, 0x20C);
            ide_setConstantWindowOffset(40, 19);
            #endif
        #endif
        ide_setDevNo(TRUE);

        ide_setInterlace(TRUE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(0x00);

        #if _FPGA_EMULATION_
        ide_setTvPowerDown(TRUE);
        #else
        ide_setTvPowerDown(FALSE);
        #endif

        ide_setTvMode(IDE_TV_NTSC_M);
        ide_setTvBurst(0x00);
        ide_setTvSlaveMode(TRUE);
        ide_setTvSearchEn(TRUE);
        ide_setTvCkpnPol(TRUE);
        ide_setTvNtscSetupLevel(0x24);

        ide_setTvChromaFilter(TRUE, NULL);

        psizeDisplay->uiWidth       = RZW_T27P02_LCDTV_Buf_width;
        psizeDisplay->uiHeight      = RZW_T27P02_LCDTV_Buf_height;
        psizeDisplay->uiWinWidth    = RZW_T27P02_LCDTV_Win_width;
        psizeDisplay->uiWinHeight   = RZW_T27P02_LCDTV_Win_height;
        break;
    #endif
    case PINMUX_LCDMODE_RGB_SERIAL:
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
        ide_setThrough(FALSE);

        ide_setHorTiming(0x00, 616-1, 100, (480+100) - 1);
        ide_setVerTiming(0x05, 262, 18, (240+18)-1, 18, (240+18)-1);

        ide_setInterlace(FALSE);
        ide_setStartField(FALSE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(FALSE);

        //ide_set_clamp(0x01);
        ide_setTvPowerDown(TRUE);

        //Subpixel
        ide_setSubpixel(TRUE, TRUE, TRUE, TRUE);
        ide_setSubpixel(FALSE, FALSE, FALSE, FALSE);

        psizeDisplay->uiWidth       = RZW_T27P02_UPS051_Buf_width;
        psizeDisplay->uiHeight      = RZW_T27P02_UPS051_Buf_height;
        psizeDisplay->uiWinWidth    = RZW_T27P02_UPS051_Win_width;
        psizeDisplay->uiWinHeight   = RZW_T27P02_UPS051_Win_height;
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


        ide_setHorTiming(0x00, 616-1, 100, (480+100) - 1);
        ide_setVerTiming(0x05, 262, 18, (240+18)-1, 18, (240+18)-1);

        ide_setInterlace(FALSE);
        ide_setStartField(FALSE);

        ide_setCtrst(0x40);
        ide_setBrt(0x00);
        ide_setCmults(0x40);
        ide_setCex(FALSE);

        ide_setTvPowerDown(TRUE);

        psizeDisplay->uiWidth       = RZW_T27P02_THROUGH_Buf_width;
        psizeDisplay->uiHeight      = RZW_T27P02_THROUGH_Buf_height;
        psizeDisplay->uiWinWidth    = RZW_T27P02_THROUGH_Win_width;
        psizeDisplay->uiWinHeight   = RZW_T27P02_THROUGH_Win_height;
        break;


    default:
        //debug_err(("OpenAUCN01() Not support %d mode\r\n",LCD_DeviceObj.uiLCDMode));
        break;
    }

    ide_setEn(TRUE);
    //avoid vsync active area
    ide_waitFrameEnd();

    Init_RZW_LCD();
}

/**
  Close RZWTD24THEB5 driver

  Release RZWTD24THEB5 driver, private function for internal used

*/
static void CloseRZW(void)
{
    // Disable IDE
    ide_setEn(FALSE);
    ide_waitFrameEnd();

    // Change to standby mode
    //sendtoRZWLCDreg(0x00, STB_OFF);
    ide_setDevNo(FALSE);

    delay(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    ide_close();

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

//@}
