/*
    @file       ToppolyA3AA.c
    @brief      LCD Driver of Toppoly A3AA Panel
    @author     Niven Cho
    @date       2009/08/31
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "OSCommon.h"
#include "Utility.h"

#define OPTION_SIF_0000  0x0   ///< SIF bus mode 0000
#define OPTION_SIF_0001  0x1   ///< SIF bus mode 0001
#define OPTION_SIF_0010  0x2   ///< SIF bus mode 0010
#define OPTION_SIF_0011  0x3   ///< SIF bus mode 0011
#define OPTION_SIF_0100  0x4   ///< SIF bus mode 0100
#define OPTION_SIF_0101  0x5   ///< SIF bus mode 0101
#define OPTION_SIF_0110  0x6   ///< SIF bus mode 0110
#define OPTION_SIF_0111  0x7   ///< SIF bus mode 0111
#define OPTION_SIF_1000  0x8   ///< SIF bus mode 1000
#define OPTION_SIF_1001  0x9   ///< SIF bus mode 1001
#define OPTION_SIF_1010  0xA   ///< SIF bus mode 1010
#define OPTION_SIF_1011  0xB   ///< SIF bus mode 1011
#define OPTION_SIF_1100  0xC   ///< SIF bus mode 1100
#define OPTION_SIF_1101  0xD   ///< SIF bus mode 1101
#define OPTION_SIF_1110  0xE   ///< SIF bus mode 1110
#define OPTION_SIF_1111  0xF   ///< SIF bus mode 1111


typedef struct _PANEL_CMD{
    UINT8 Address;
    UINT8 Value;
}tPANEL_CMD;

typedef struct _PANEL_PARAM{
    LCD_MODE    LCDMode;
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
    ICST_TYPE       icst;
    BOOL            dithering[2];
    IDE_DEVICE_TYPE device;
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Step 1: Set All Parameters
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--- Check SIF of Settings ---//
//#NT#2009/09/14#Niven Cho -begin
//#NT#Modified., Fit to Spec
#define SETTING_SIF_MODE                    OPTION_SIF_0100 //SIF MODE: SIF_MODE_0000 ~ SIF_MODE_1111
#define SETTING_PANEL_REG_BIT_SHIFT_ADDRESS 8               //D[?] is Address LSB
#define SETTING_PANEL_REG_BIT_SHIFT_VALUE   0               //D[?] is Content LSB
#define SETTING_PANEL_REG_BIT_WRITE         14              //D[?] is Bit of Write Command
#define SETTING_PANEL_REG_MODE_WRITE        0               //BIT High(1) or LOW(0) is Write Mode
#define SETTING_PANEL_REG_MODE_READ         1               //BIT High(1) or LOW(0) is Read  Mode
//#NT#2009/09/14#Niven Cho -end

//--- Prepare LCD Command Queue ---//
static const tPANEL_CMD tCmdStandby[] = { //Standby Mode, for ClosePanel()
    {0x2B, 0x00}, //STB
};

//Following Register Settings is by TJ027NA01FA (TJ027NA01FA1) Software Design Guide
static const tPANEL_CMD tCmdModeYUV[] = {
    {0x2B, 0x01}, //STB
    {0x00, 0x4C}, //VCOM_AC
    {0x01, 0x80}, //VCOM_DC
    {0x04, 0x4B}, //YUV640
    {0x16, 0x52}, //Select Auto or Manual Gamma Setting
    {0x17, 0x86}, //gamma V16,V8
    {0x18, 0xA9}, //gamma V50,V32
    {0x19, 0xCC}, //gamma V96,V72
    {0x1A, 0x99}, //gamma V120,V110
    {0x1B, 0x63}, //amma V0,V127
    {0x2F, 0x6D}, //LC_type
};

//--- Make Specification ---//
static const tLCD_PARAM tMode[] = {
    {//LCD_MODE_YUV Begin
        {
            PINMUX_LCDMODE_YUV640,   //!< LCDMode;
            24.54f,          //!< DCLK;
            1560,           //!< HSyncTotalPeriod;
            1280,           //!< HSyncActivePeriod;
            240,            //!< HSyncBackPorch;
            262,            //!< VSyncTotalPeriod;
            240,            //!< VSyncActivePeriod;
            21,             //!< VSyncBackPorchOdd;
            21,             //!< VSyncBackPorchEven;
            640,            //!< BufferWidth;
            240,            //!< BufferHeight;
            640,            //!< WindowWidth;
            240,            //!< WindowHeight;
            TRUE            //!< bYCbCrFormat;
        },
        {
            PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS, //!< pinmux_select_lcd;
            ICST_CCIR601,           //!< icst;
            {FALSE,FALSE},          //!< dithering[2];
            DISPLAY_DEVICE_TOPPOLY, //!< device;
            FALSE,                  //!< pdir;
            1,                      //!< odd;
            2,                      //!< even;
            TRUE,                   //!< hsinv;
            TRUE,                   //!< vsinv;
            FALSE,                  //!< hvldinv;
            FALSE,                  //!< vvldinv;
            TRUE,                   //!< clkinv;
            FALSE,                  //!< fieldinv;
            FALSE,                  //!< rgbdummy;
            FALSE,                  //!< interlace;
            FALSE,                  //!< startfield;
            0x40,                   //!< ctrst;
            0x00,                   //!< brt;
            0x40,                   //!< cmults;
            FALSE,                  //!< cex;
            FALSE,                  //!< standby_signal;
            TRUE,                   //!< clamp;
            TRUE,                   //!< tv_powerdown;
            {0x00,0x00},            //!< sif_startend[2];
        },
        (tPANEL_CMD*)tCmdModeYUV,                  //!< pCmdQueue
        sizeof(tCmdModeYUV)/sizeof(tPANEL_CMD),    //!< nCmd
    },//LCD_MODE_YUV End
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Step 1: End
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//function declaration
static void delay(UINT uiUs);
static void sendtoLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoLCDreg(UINT8 addr, UINT8 value);
static void readLCDreg_sif(UINT8 addr, UINT8 *value);
static void readLCDreg_gpio(UINT8 addr, UINT8 *value);
static void readLCDreg(UINT8 addr, UINT8 *value);

static void DeviceInit(void);
static void DeviceOpen(LCDSIZE *psizeDisplay);
static void DeviceClose(void);

//static variable
static tLCD_PARAM* pMode; //Current Mode
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
    UINT32  i;

    LCD_DeviceObj = *lcddeviceobj;
    LCD_DeviceObj.uiLCDMode = pinmux_getDispMode(PINMUX_FUNC_ID_LCD);

    //Check Mode Valid
    pMode = NULL;
    for(i=0;i<sizeof(tMode)/sizeof(tLCD_PARAM);i++)
    {
        if(tMode[i].Panel.LCDMode==LCD_DeviceObj.uiLCDMode)
        {
            pMode = (tLCD_PARAM*)&tMode[i];
            break;
        }
    }

    if(pMode==NULL)
    {
        debug_err(("g_LCDCtrlObj.uiLCDMode=%d not support\r\n",LCD_DeviceObj.uiLCDMode));
        return;
    }

    LCD_DeviceObj.fIDEDstCLK = pMode->Panel.DCLK;

    DeviceOpen(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    DeviceClose();
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
    sendtoLCDreg(addr, value);
}

void LCD_RegRead(UINT32 addr, UINT32* value)
{
    *value = 0;
    readLCDreg(addr, (UINT8*)value);
}
#endif

/*
  delay time period

  @param times : time period

*/
void delay(UINT uiUs)
{
    UINT32 i;

    // When CPU_CLK is 80Mhz, uiNs = 1 will do 1.057 us.
    // Use FCLKSEL_CLK80 as base.
    switch (pll_getClockRate(PLL_CLKSEL_CPU))
    {
    case PLL_CLKSEL_CPU_48:
        uiUs = uiUs * 48 / 80;
        break;
    case PLL_CLKSEL_CPU_60:
        uiUs = uiUs * 60 / 80;
        break;
    case PLL_CLKSEL_CPU_120:
        uiUs = uiUs * 120 / 80;
        break;
    case PLL_CLKSEL_CPU_160:
        uiUs = uiUs * 160 / 80;
        break;
    case PLL_CLKSEL_CPU_240:
        uiUs = uiUs * 240 / 80;
        break;
    case PLL_CLKSEL_CPU_80:     // when uiUs is 1: delay 1.057 us
    default:
        // ratio: 1/1
        break;
    }

    for (i=((uiUs<<3)+(uiUs<<2)); i>0; i--)
    {
        __asm
        {
            nop;
        }
    }
}

/*
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
void sendtoLCDreg(UINT8 addr, UINT8 value)
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

void readLCDreg(UINT8 addr, UINT8* p_value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         readLCDreg_sif(addr, p_value);
    }
    else
    {
         readLCDreg_gpio(addr, p_value);
    }
}

/*
  Open panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
void DeviceOpen(LCDSIZE *psizeDisplay)
{
    UINT8   uiClockDiv;
    tIDE_PARAM*     pIDE = &pMode->IDE;
    tPANEL_PARAM*   pPanel = &pMode->Panel;

    if (ide_open() != E_OK)
    {
        return;
    }

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
    pinmux_setPinmux(PINMUX_FUNC_ID_LCD, pIDE->pinmux_select_lcd);
    //pinmux_select_lcd(pIDE->pinmux_select_lcd);

    // Set DCLK
    //#NT#2010/01/27#Niven Cho -begin
    //#NT#Modified., Use Src Clock 216Mhz, need to get 24Mhz for setting 24.54 Mhz
    if(pPanel->bYCbCrFormat)
    {
        uiClockDiv = (UINT8)((float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK+0.5);
    }
    else
    {
        uiClockDiv = (UINT8)((float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2+0.5);
    }
    //#NT#2010/01/27#Niven Cho -end

    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

    // Enable IDE Clock
    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    ide_set_icst(pIDE->icst);
    //#NT#2009/07/22#Niven Cho -begin
    //#NT#NT96450 Needs to Call This Function
    if(pPanel->bYCbCrFormat)
        ide_configIcst(FALSE, 0);
    else
        ide_configIcst(TRUE, CST_YCBCR2RGB);
    //#NT#2009/07/22#Niven Cho -end
    ide_set_dithering(pIDE->dithering[0],pIDE->dithering[1]);
    ide_set_device(pIDE->device);
    ide_set_pdir(pIDE->pdir);
    ide_set_odd(pIDE->odd);
    ide_set_even(pIDE->even);
    ide_set_hsinv(pIDE->hsinv);
    ide_set_vsinv(pIDE->vsinv);
    ide_set_hvldinv(pIDE->hvldinv);
    ide_set_vvldinv(pIDE->vvldinv);
    ide_set_clkinv(pIDE->clkinv);
    ide_set_fieldinv(pIDE->fieldinv);
    ide_set_rgbdummy(pIDE->rgbdummy);
    ide_set_shw_st();

    if(pPanel->bYCbCrFormat)
    {
        ide_set_hor_timing(0x0
            ,(pPanel->HSyncTotalPeriod>>1)
            ,(pPanel->HSyncBackPorch>>1)
            ,((pPanel->HSyncActivePeriod+pPanel->HSyncBackPorch)>>1)-1);
    }
    else
    {
        ide_set_hor_timing(0x00
            ,pPanel->HSyncTotalPeriod
            ,pPanel->HSyncBackPorch
            ,(pPanel->HSyncActivePeriod+pPanel->HSyncBackPorch)-1);
    }

    ide_set_ver_timing(0x00
        ,pPanel->VSyncTotalPeriod-1
        ,pPanel->VSyncBackPorchOdd
        ,(pPanel->VSyncActivePeriod+pPanel->VSyncBackPorchOdd)-1
        ,pPanel->VSyncBackPorchEven
        ,(pPanel->VSyncActivePeriod+pPanel->VSyncBackPorchEven)-1);

    ide_set_interlace(pIDE->interlace);
    ide_set_startfield(pIDE->startfield);

    ide_set_ctrst(pIDE->ctrst);
    ide_set_brt(pIDE->brt);
    ide_set_cmults(pIDE->cmults);
    ide_set_cex(pIDE->cex);
    ide_set_standby_signal(pIDE->standby_signal);

    ide_set_clamp(pIDE->clamp);
    ide_set_tv_powerdown(pIDE->tv_powerdown);

    ide_set_sif_startend(pIDE->sif_startend[0],pIDE->sif_startend[1]);

    psizeDisplay ->uiWidth      = pPanel->BufferWidth;
    psizeDisplay ->uiHeight     = pPanel->BufferHeight;
    psizeDisplay ->uiWinWidth   = pPanel->WindowWidth;
    psizeDisplay ->uiWinHeight  = pPanel->WindowHeight;

    //#NT#2010/01/21#Niven Cho -begin
    //#NT#Modified., Correct Flow, Should be IDE Setting -> Panel Reg Setting -> Delay VD -> Start IDE Single
    DeviceInit();

    TimerDelayMs(100);
    ide_set_en(TRUE);
    ide_wait_frame_end();
    //#NT#2010/01/21#Niven Cho -end
}

/*
  Close driver

  Release driver, private function for internal used

*/
void DeviceClose(void)
{
    UINT32  i;

    // Disable IDE
    //#NT#2009/11/04#Klins Chen#[0007606] -begin
    //#NT#Modify close flow
    ide_set_en(FALSE);
    ide_wait_frame_end();
    //#NT#2009/11/04#Klins Chen#[0007606] -end

    for(i=0;i<sizeof(tCmdStandby)/sizeof(tPANEL_CMD);i++)
    {
        sendtoLCDreg(tCmdStandby[i].Address,tCmdStandby[i].Value);
    }

    delay(80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

#if (SETTING_SIF_MODE==OPTION_SIF_0000)     //Confirmed
    #define USED_SIF_MODE     SIF_MODE_0000
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0001)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0001
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0010)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0010
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0011)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0011
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0100)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0100
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0101)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0101
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0110)
    #define USED_SIF_MODE     SIF_MODE_0110 //Not Confirmed
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0111)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0111
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_1000)   //Confirmed
    #define USED_SIF_MODE     SIF_MODE_1000
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#else
    #error("This SIF Mode has not implement!");
#endif

static void DeviceInit(void)
{
    UINT32      i;
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();
        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = USED_SIF_MODE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;
        sif_setControl((SIF_CH)LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    if(LCD_DeviceObj.fpPostAdjusement)
    {
        LCD_DeviceObj.fpPostAdjusement();//sendtoLCDreg);
    }
    else if(pMode->pCmdQueue!=NULL)
    {
        for(i=0;i<pMode->nCmd;i++)
        {
            sendtoLCDreg(pMode->pCmdQueue[i].Address,pMode->pCmdQueue[i].Value);
        }
    }
}

static void sendtoLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = (((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)
        |((UINT32)value<<SETTING_PANEL_REG_BIT_SHIFT_VALUE)
        |(SETTING_PANEL_REG_MODE_WRITE<<SETTING_PANEL_REG_BIT_WRITE)) << 16;
    sif_sendMSB((SIF_CH)LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    SIF_SEN_INIT();
    SIF_CLK_INIT();
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

    data = ((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)
        | ((UINT32)value<<SETTING_PANEL_REG_BIT_SHIFT_VALUE)
        | (SETTING_PANEL_REG_MODE_WRITE<<SETTING_PANEL_REG_BIT_WRITE);

    delay(500);

    SIF_SEN_BEGIN();
    for(j = 16; j > 0; j--)
    {
        if(data & 0x8000)
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        SIF_CLK_BEGIN();
        delay(200);
        SIF_CLK_END();
        delay(100);
        data <<= 1;
    }

    delay(500);
    SIF_SEN_END();
}

static void readLCDreg_sif(UINT8 addr, UINT8* p_value)
{
    *p_value = 0;
    debug_err(("readLCDreg_sif() is not support!\r\n"));
}

void readLCDreg_gpio(UINT8 addr, UINT8* p_value)
{
    UINT32 data;
    UINT32 j;
    UINT32 temp;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    SIF_SEN_INIT();
    SIF_CLK_INIT();
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

    data = ((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)
        | (SETTING_PANEL_REG_MODE_READ<<SETTING_PANEL_REG_BIT_WRITE);

    SIF_SEN_BEGIN();

    delay(500);

    for(j = 16; j > 8; j--)
    {
        if(data & 0x8000)
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        SIF_CLK_BEGIN();
        delay(200);
        SIF_CLK_END();
        delay(100);
        data <<= 1;
    }

    *p_value = 0;
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_INPUT);

    delay(200);
    for(j = 0; j < 8; j++)
    {
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        temp = gpio_getPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        *p_value |= temp << (7 - j);
        delay(200);
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}