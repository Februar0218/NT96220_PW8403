/*
    @file       ToppolyTD25THEA1.c

    @ingroup    mILIBLCDToppolyTD25THEA1

    @brief      AU LCD Driver of ToppolyTD24THEA1 panel, Only support YUV and RGBD mode now.

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDToppolyTD24THEA1

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0100).

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

// Address 0x02
#define INPUT_FORMAT_RGBD           0x00
#define INPUT_FORMAT_YUV            0x01
#define INPUT_FORMAT_THROUGH        0x02
#define INPUT_FORMAT_STD_NTSC       0x00
#define INPUT_FORMAT_STD_PAL        0x04
#define INPUT_VALID_DATA_1280       0x00
#define INPUT_VALID_DATA_1440       0x08
#define INPUT_CLK_EDGE_POS          0x00
#define INPUT_CLK_EDGE_NEG          0x10
#define INPUT_HD_POL_LOW            0x00
#define INPUT_HD_POL_HIGH           0x20
#define INPUT_VD_POL_LOW            0x00
#define INPUT_VD_POL_HIGH           0x40

// Address 0x03
#define INTERLACE_MODE              0x00
#define NONINTERLACE_MODE           0x01

// Address 0x04
#define POWER_STANDBY               0x00
#define POWER_NORMAL                0x01
#define CP_CLK_DISABLE              0x00
#define CP_CLK_ENABLE               0x02
#define PWM_OUTPUT_DISABLE          0x00
#define PWM_OUTPUT_ENABLE           0x04
#define PRE_CHARGE_DISABLE          0x00
#define PRE_CHARGE_ENABLE           0x08
#define OUTPUT_DRIVER_50            0x00
#define OUTPUT_DRIVER_65            0x10
#define OUTPUT_DRIVER_80            0x20
#define OUTPUT_DRIVER_100           0x30

// Window and buffer size
// Through, YUV mode
#define TOPPOLY_BUF_WIDTH           640
#define TOPPOLY_BUF_HEIGHT          240
#define TOPPOLY_WIN_WIDTH           640
#define TOPPOLY_WIN_HEIGHT          240

// RGB dummy mode
#define TOPPOLY_RGBD_BUF_WIDTH      320
#define TOPPOLY_RGBD_BUF_HEIGHT     240
#define TOPPOLY_RGBD_WIN_WIDTH      320
#define TOPPOLY_RGBD_WIN_HEIGHT     240

// Function declaration
static void delay_Toppoly(UINT32 times);
static void sendtoToppolyLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoToppolyLCDreg_gpio(UINT8 addr, UINT8 value);

static void sendtoToppolyLCDreg(UINT8 addr, UINT8 value);

static void Init_Toppoly_LCD(void);
static void OpenToppoly(LCDSIZE *psizeDisplay);
static void CloseToppoly(void);

// Static variable
static LCD_DEVICE_OBJ   LCD_DeviceObj;

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
        case PINMUX_LCDMODE_YUV640:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        case PINMUX_LCDMODE_RGBD320:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        }
    }

    OpenToppoly(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseToppoly();
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
    sendtoToppolyLCDreg(addr, value);
}
#endif

/*
  delay time period

  @param times : time period

*/
static void delay_Toppoly(UINT32 times)
{
    for( ; times>0; times--)
       ;
}

/*
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoToppolyLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = (((UINT32)addr << 10)|(UINT32)value ) << 16;
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/*
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoToppolyLCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)addr << 10 | (UINT32)value;

    delay_Toppoly(500);

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

        delay_Toppoly(100);

        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_Toppoly(200);

        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_Toppoly(100);

        data <<= 1;
    }

    delay_Toppoly(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

/*
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
void sendtoToppolyLCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
         sendtoToppolyLCDreg_sif(addr, value);
    }
    else
    {
         sendtoToppolyLCDreg_gpio(addr, value);
    }
}

/*
    Initial LCD of ToppolyTD24THEA1, configure SIF intarface and send data to panel register to configure

*/
static void Init_Toppoly_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if (LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv        = 7;
        SIFCtrl.uiMode          = SIF_MODE_0000;
        SIFCtrl.uiLength        = 16;
        SIFCtrl.bMSB            = TRUE;
        //SIFCtrl.bSyncEn         = FALSE;
        //SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration   = 0;
        SIFCtrl.uiSenH          = 0;

        // Init SIF
        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    //#NT#2008/09/10#Niven -begin
    //#Modified, Callback Function for user define LCD saturation,brightness,gamma...
    if(LCD_DeviceObj.fpPostAdjusement)
    {
        LCD_DeviceObj.fpPostAdjusement();//sendtoToppolyLCDreg);
    }
    else
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case PINMUX_LCDMODE_RGB_SERIAL:
            break;

        case PINMUX_LCDMODE_YUV640:
            sendtoToppolyLCDreg(0x02, INPUT_FORMAT_YUV|INPUT_FORMAT_STD_NTSC|INPUT_VALID_DATA_1280|INPUT_CLK_EDGE_POS|INPUT_HD_POL_LOW|INPUT_VD_POL_LOW);
            break;

        case PINMUX_LCDMODE_RGBD320:
        default:
            sendtoToppolyLCDreg(0x02, INPUT_FORMAT_RGBD|INPUT_FORMAT_STD_NTSC|INPUT_VALID_DATA_1280|INPUT_CLK_EDGE_POS|INPUT_HD_POL_LOW|INPUT_VD_POL_LOW);
            break;
        }
        sendtoToppolyLCDreg(0x03, NONINTERLACE_MODE);
        sendtoToppolyLCDreg(0x04, POWER_NORMAL|CP_CLK_ENABLE|PWM_OUTPUT_ENABLE|PRE_CHARGE_ENABLE|OUTPUT_DRIVER_50);

        // Gamma
        sendtoToppolyLCDreg(0x14, 0x60);
        sendtoToppolyLCDreg(0x15, 0x22);
        sendtoToppolyLCDreg(0x16, 0x53);
        sendtoToppolyLCDreg(0x17, 0x57);
        sendtoToppolyLCDreg(0x18, 0x00);
    }
}

/*
  Open ToppolyTD25THEA1 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenToppoly(LCDSIZE *psizeDisplay)
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
    case PINMUX_LCDMODE_RGB_SERIAL:
        debug_err(("Not support yet!\r\n"));
        break;

    case PINMUX_LCDMODE_YUV640:
        // Top control for lcd type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        // Set DCLK to 27MHz
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK);
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE1_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007606] -begin
        //#NT#Remove unnecessary dummy delay
        //delay_Toppoly(0x10000);
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

        ide_set_hor_timing(0x00, 0x30B, 0x78, 0x2F7);
        ide_set_ver_timing(0x00, 0x105, 0x15, 0x104, 0x15, 0x104);
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

        psizeDisplay->uiWidth       = TOPPOLY_BUF_WIDTH;
        psizeDisplay->uiHeight      = TOPPOLY_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = TOPPOLY_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = TOPPOLY_WIN_HEIGHT;
        break;

    case PINMUX_LCDMODE_RGBD320:
    default:
        // Top control for LCD type
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_SERIAL_RGB_8BITS);
        //pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK to 27MHz
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv -1));

        // Enable IDE clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007606] -begin
        //#NT#Remove unnecessary dummy delay
        //delay_Toppoly(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007606] -end

        ide_set_icst(0);
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_TOPPOLY);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(2);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(1);
        ide_set_shw_st();

        ide_set_hor_timing(0x00, 0x617, 0x0F0, 0x5EF);
        ide_set_ver_timing(0x00, 0x105, 0x15, 0x104, 0x15, 0x104);
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

        psizeDisplay->uiWidth       = TOPPOLY_RGBD_BUF_WIDTH;
        psizeDisplay->uiHeight      = TOPPOLY_RGBD_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = TOPPOLY_RGBD_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = TOPPOLY_RGBD_WIN_HEIGHT;
        break;
    }

    //#NT#2009/11/04#Klins Chen#[0007606] -begin
    //#NT#Remove unnecessary dummy delay
    ide_set_en(TRUE);
    // Avoid vsync active area
    ide_wait_frame_end();
    //#NT#2009/11/04#Klins Chen#[0007606] -end

    Init_Toppoly_LCD();
}

/*
  Close ToppolyTD25THEA1 driver

  Release ToppolyTD25THEA1 driver, private function for internal used

*/
static void CloseToppoly(void)
{
    // Disable IDE
    //#NT#2009/11/04#Klins Chen#[0007606] -begin
    //#NT#Modify close flow
    ide_set_en(FALSE);
    ide_wait_frame_end();
    //#NT#2009/11/04#Klins Chen#[0007606] -end

    // Change to standby mode
    sendtoToppolyLCDreg(0x04, POWER_STANDBY);

    delay_Toppoly(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}
//@}
