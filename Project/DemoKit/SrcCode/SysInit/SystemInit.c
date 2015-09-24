/**
    @file       SystemInit.c
    @ingroup    mIPRJAPCfg

    @brief      Initialize system.

                Initialize HW and FW.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

/**
    @addtogroup mIPRJAPCfg
*/
//@{

#include "SystemInit.h"
#include "SystemTask.h"
#include "SysCfg.h"
#include "GPIOMapping.h"
#include "Driver.h"
#include "usb.h"
#include "LCDTV.h"
#include "ide.h"
#include "SIF.h"
#include "I2C.h"
#include "Top.h"
#include "JpgDec.h"
#include "Audio.h"
#include "Pll.h"
#include "UART.h"
//#include "ImageAlgInfor.h"
#include "PrimaryTsk.h"
#include "GlobalVar.h"
#include "SystemClock.h"
#include "VoltageDet.h"
#include "CopyRest.h"
#include "Utility.h"
#include "DMA.h"
#include "SensorFP.h"
#include "UserGPIO.h"
#include "Clock.h"
#include "PlaySoundTsk.h"
#include "PlaySoundInt.h"
#include "Nand.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
//#include "AF_Lib.h"
#include "RamDisk.h"
#include "PStore.h"
#include "PSDrv.h"
#include "IrRx.h"
#include "FileSysTsk.h"
#include "Calibrationint.h"
#include "pad.h"
#include "Timer.h"
#include "UIFramework.h"
#include "UIFlow.h"
#include "TouchPanel.h"
#include "UICustomer.h"
#include "VendorEXIF.h"
#include "sdio.h"
#include "GPIOMapping.h"
#include "ExifRW.h"
#include "UserEXIF.h"
#include "UserPStore.h"
#include "sensor.h"
#include "KeyADC.h"
#include "eac.h"
#include "display.h"
#include "UIConfig.h"
#include "pad.h"
#include "CalLens.h"
#include "display.h"
#include "disp_rotate.h"

//#include "LIS33DE.h"
#include "DMARD07.h"

#include "ck235.h"
#include <stdlib.h>
#include "GlobalVar.h"

#if 1
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

#define ENCRYPTE_NONE   0
#define CK235_PW01    1
#define CK235_PW03    2

#define  _ENCRYPTE_   ENCRYPTE_NONE//CK235_PW03//ENABLE

#define tmp_dev_addr 0xC0
#define tmp_sub_addr 0xA0
unsigned char BIT1612M_BANK1[8] = {0x0A,0x04,0x03,0x01,0x04,0x03,0x01};

unsigned char BIT1612M_ITU656_BANK0[252] = 
{
		                         0x00,0x08,0x00,0x00,0x00,0x00,0x2D,0x3A,0xCA,0x89,0x8D,
0x06,0x2C,0x0E,0x20,0x91,0x1F,0x22,0x05,0x09,0x19,0x28,0x45,0x00,0x00,0x00,0x20,
0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x06,0x43,0xC0,0x00,0x01,0x00,0x00,0x00,0x3F,0xFF,0x00,0x0B,0x00,0xB5,0x60,
0x30,0x1A,0x39,0x10,0x7D,0x58,0x30,0x15,0x06,0x10,0xC0,0x08,0x80,0x10,0x00,0xE0,
0x07,0x1C,0x11,0x23,0x0E,0x20,0x20,0x20,0x33,0x00,0x7C,0x01,0x03,0xDB,0x00,0xD9,
0xE0,0x10,0x00,0x00,0x00,0x00,0x00,0x0C,0x01,0xE0,0x09,0x2A,0x10,0x03,0x00,0x00,
0xDE,0x00,0x00,0x03,0x15,0x5F,0x46,0x03,0x11,0x00,0x00,0x00,0x82,0x7E,0x7E,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x1F,0x00,0x00,0x00,0xC0,0x03,0xC0,0x4C,0x0C,0xA2,
0x02,0x87,0x78,0x99,0x67,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x92,0x6C,0x80,0x00,0x80,0x80,0x44,0x44,
0x05,0x20,0x03,0x25,0x0C,0x4A,0x28,0xF8,0xE2,0xB8,0xF5,0x03,0x03,0x00,0x80,0xA0,
0x26,0x9F,0x02,0x90,0x00,0xD5,0x08,0x21,0x00,0x03,0x80,0x80,0x50,0x50,0xC4,0x29,
0x38,0x00,0x0B,0x3C,0x00,0x00,0x10,0x10,0x0A,0x07,0x24,0xAA,0x73,0x18,0x30,0x30,
0x54,0x25,0x40,0x3F,0x00,0x04,0xD9,0x25,0x40,0x3F,0x00,0x04,0x60,0x1F,0xCE,0x03,
0x60,0x0D,0x05,0x80,0x80,0xC8,0x00,0x00,0x00,0x00,0x00,0x67,0x03,0x00,0xFF,0x10
};

unsigned char BIT1216M_CVBS_REG[][2]=
{
       {0x0D,0x8A},  
	{0x0E,0xEC},  
	{0x10,0x38},  
	{0x14,0x8B},  //0X8E
	{0x15,0x1D},  
	//{0x1A,0x33},  
	{0x3C,0x8B}, 
	{0x3e,0x89}, 
	{0x44,0x92}, 
	{0x4B,0x5B},  
	{0x4C,0x88},  
	{0x4D,0x02},
	{0x5a,0x78},
	{0x6E,0x01},  
	{0x6F,0xEE},  
	{0x70,0xDD},  
	{0x71,0x5B},  
	{0x78,0x10},  
	{0x79,0xCA},  
	{0x7A,0x94},  
	{0x7B,0x35},  
	{0xFC,0x23}
};
unsigned char BIT1216M_ITU656_REG[][2]=
{
	{0x0D,0xCA},  
	{0x0E,0x89},  
	{0x10,0x06},  
	{0x14,0x91},  
	{0x15,0x1F},//0X1F  
	//{0x1A,0x3F},  
	{0x3C,0x0B},  
	{0x3e,0xb5}, 
	{0x44,0x7d}, 
	{0x4B,0x08},  
	{0x4C,0x80},  
	{0x4D,0x10},  
	{0x5a,0x7C}, 
	{0x6E,0x00},  
	{0x6F,0x00},  
	{0x70,0xDE},  
	{0x71,0x00},  
	{0x78,0x11},  
	{0x79,0x00},  
	{0x7A,0x00},  
	{0x7B,0x00},  
	{0xFC,0x03}
};

static BOOL g_GsensorPowerOn = FALSE;
void SetGsensorPowerOn(BOOL gPwron)
{
    g_GsensorPowerOn = gPwron;
}
BOOL GetGsensorPowerOn(void)
{
    return g_GsensorPowerOn;
}

/**
  Init essential HW

  Init essentail HW that might used in any kind of FW code.
  For example: UART (output debug message)...

  @param void
  @return void
*/
static void SysInit_InitEssentialHW(void)
{
    // Initialize driver, must be executed before any driver open API
    drv_init();
    pll_configClkAutoGating(0, 0);
    pll_configPclkAutoGating(0, 0);

    //#NT#2011/07/14#Hideo Lin -begin
    //#NT#Pinmux init settings (depend on project)
    GPIOMap_PinmuxInit();
    //#NT#2011/07/14#Hideo Lin -end

    // Init UART
    // Usually, the UART output message ability is enabled in boot loader.
    // And this is for UART getting character.
    // But if your boot loader didn't enable UART or you load code via
    // ARM AXD, this should be called before any task might output message.
    #if (_GPS_RECEIVER_ == ENABLE)
    #else
    uart_open();
    // Wait for all TX data are sent
    // During kernel initialization phase, there might be some data in UART
    // FIFO, uart_init() will clear these data. To make sure all data are sent,
    // we have to wait for TX empty.
    // >> NOTE << You can mark these codes, but you might lose some error message.
    // Recommend: Mark these codes if there is no more error message when boot up.
    while ((uart_checkIntStatus() & UART_INT_STATUS_TX_EMPTY) == 0)
    {
        ;
    }
    // Init UART to baud rate 115200, length 8 bits, stop bits 1, parity none
    uart_init(UART_BAUDRATE_115200, UART_LEN_L8_S1, UART_PARITY_NONE);
    #endif
    // Init RTC
    // Ususally, the RTC is enabled and force updated in boot loader.
    // But if your boot loader didn't enable RTC or you load code via
    // ARM AXD, this should be called ASAP.
    rtc_open();

    // Sometimes RTC power alarm will be enabled accidentally.
    // If you don't need the RTC power alarm function, keep this code to
    // prevent power up by RTC. Otherwise, please implement your code to
    // control power alarm function.
    rtc_disablePWRAlarm();

    // Set RTC base date
    rtc_setBaseDate(2015, 1, 1);
	if (rtc_isPowerLost())//henry 20141209
	{
		rtc_setDate(2015, 1, 1);
		rtc_setTime(0, 0, 0);
		rtc_triggerCSET();
	}

    // Since clk_open() won't increase the power consumption but calling
    // clk_open() and clk_close() when changing clock will decrease the
    // performance. We move clk_open() to here and don't call clk_close()
    // when FW is active.
    //#NT#2011/07/11#Steven Wang -begin
    //#NT#Call SysClk_Init() instead of clk_open()
    //clk_open();
    SysClk_Init();
    //#NT#2011/07/11#Steven Wang -end
    // Reset shutdown timer
    rtc_resetShutdownTimer();

    //#NT#2008/12/16#Philex - begin
    //# set permanet storage obejct here
    #if (_PSTOREDEVICE_ != _PSTOREDEVICE_NANDPS_)  //SPI FLASH
        debug_err(("ramdsk_getStorageObject()\r\n"));
        Primary_SetPerStrgObject(ramdsk_getStorageObject());
    #else // nand object
        debug_err(("nand_getStorageObject()\r\n"));
        Primary_SetPerStrgObject(nand_getStorageObject());
    #endif
}

/**
  Display startup screen

  Display startup screen

  @param void
  @return void
*/
void SysInit_DisplaySplashScreen(UINT32 uiLogo,UINT8 *pBgBuf, UINT32 uiSize)
{
    UINT32              uiPoolAddr;
    DECJPG_INFO         DecOneJPGInfo = {0};
    JPGHEAD_DEC_CFG     JPGCfg = {0};
    VIDEO_BUF_ADDR      FB_Addr;
    VIDEO_BUF_ATTR      FB_attr;
    VOSD_WINDOW_ATTR    FB_win;


    if (get_blk((void*)&uiPoolAddr, POOL_ID_CAPTURE) != E_OK)
    {
        return;
    }
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    // JPG information
    DecOneJPGInfo.pJPGScalarHandler = NULL;
    DecOneJPGInfo.pDstAddr          = (UINT8 *)uiPoolAddr;
    DecOneJPGInfo.jdcfg_p           = &JPGCfg;
    DecOneJPGInfo.IfDecodeThumb     = DEC_PRIMARY;
    DecOneJPGInfo.bEnableTimeOut = FALSE;
    DecOneJPGInfo.bEnableCrop = FALSE;

    if (uiLogo == LOGO_POWERON)
    {
        DecOneJPGInfo.pSrcAddr      = (UINT8 *)g_ucBGOpening;//g_uiScreen_160x120;
        DecOneJPGInfo.JPGFileSize   = Logo_getBGOpening_size();//SCREEN_BS_SIZE;
    } else if (uiLogo == LOGO_POWEROFF)
    {
        DecOneJPGInfo.pSrcAddr      = (UINT8 *)g_ucBGGoodbye;//g_uiScreen_PwrOff;
        DecOneJPGInfo.JPGFileSize   = Logo_getBGGoodbye_size();//SCREEN_PWROFF_SIZE;
    }else
    {
        DecOneJPGInfo.pSrcAddr          = pBgBuf;//custom' logo
        DecOneJPGInfo.JPGFileSize       = uiSize;
    }

    // Decode startup JPG, DecodeOneJPG() will open JPEG driver
    if (DecodeOneJPG(&DecOneJPGInfo) != E_OK)
    {
        return;
    }

    FB_Addr.B0_Y            = DecOneJPGInfo.uiOutAddrY;
    FB_Addr.B0_CB           = DecOneJPGInfo.uiOutAddrCb;
    FB_Addr.B0_CR           = DecOneJPGInfo.uiOutAddrCr;
    FB_Addr.uiBufSel        = IDE_VIDEO_ADDR_SEL_BUFFER0;

    FB_attr.Buf_W           = JPGCfg.imagewidth;
    FB_attr.Buf_H           = JPGCfg.imageheight;
    FB_attr.Buf_Lineoffset  = JPGCfg.imagewidth >> 2;
    FB_attr.Buf_L2R         = IDE_BUFFER_READ_L2R;
    FB_attr.Buf_T2B         = IDE_BUFFER_READ_T2B;
    FB_attr.Buf_BJmode      = IDE_VIDEO_BJMODE_BUFFER_REPEAT;
    FB_attr.Buf_Opt         = IDE_VIDEO_BUFFER_OPT_0;
    FB_attr.Buf_Num         = IDE_VIDEO_BUFFER_NUM_1;

    FB_win.Source_W         = JPGCfg.imagewidth;
    FB_win.Source_H         = JPGCfg.imageheight;
    FB_win.Des_W            = g_LCDSize.uiWinWidth;
    FB_win.Des_H            = g_LCDSize.uiWinHeight;
    FB_win.Win_Format       = JPGCfg.fileformat + 6;
    FB_win.Win_X            = 0;
    FB_win.Win_Y            = 0;
    FB_win.High_Addr        = 0;

#if (SYSINIT_LOGO_V1 == ENABLE)
    if (FWRotate_GetStatus()==FALSE)
    {
        ide_set_video_buf_addr(IDE_VIDEOID_1, &FB_Addr);
        ide_set_video_buf_attr(IDE_VIDEOID_1, &FB_attr);
        ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
    } else  {
         DISPROT_TRIG DispRotTrig;
         DISPROT_PARAM  RotParam;
         DISPLAYER  CurDisplayer;
         DISPLAYER_PARAM EmuDispLyr;
         PDISP_OBJ       pDispObj;


            FWRotate_SetVideoAttrib(IDE_VIDEOID_1,&FB_Addr,&FB_attr,&FB_win);

            disprot_lock();
            RotParam.SEL.SET_INPUT_DIM.DispID      = DISP_1;
            RotParam.SEL.SET_INPUT_DIM.DispLayer   = DISPLAYER_VDO1;
            RotParam.SEL.SET_INPUT_DIM.uiSrcWidth  = FB_attr.Buf_W;//pVdosd->uiWidth;
            RotParam.SEL.SET_INPUT_DIM.uiSrcHeight = FB_attr.Buf_H;
            RotParam.SEL.SET_INPUT_DIM.uiSrcLoffY   = (FB_attr.Buf_W+3)&0xFFFFFFFC;

            switch (JPGCfg.fileformat)
            {
              case JPEG_FORMAT_422:
              default:
                RotParam.SEL.SET_INPUT_DIM.uiSrcLoffUV   = RotParam.SEL.SET_INPUT_DIM.uiSrcLoffY;
                break;
              case JPEG_FORMAT_420:
                RotParam.SEL.SET_INPUT_DIM.uiSrcLoffUV   = RotParam.SEL.SET_INPUT_DIM.uiSrcLoffY>>1;
                break;
            }
            disprot_setParameter(DISPROT_OP_SET_INPUT_DIM, &RotParam);

            DispRotTrig.DispID      = DISP_1;
            DispRotTrig.DispLayer   = DISPLAYER_VDO1;
            DispRotTrig.SrcBufAddr[DISPROT_BUF_Y] = FB_Addr.B0_Y;
            DispRotTrig.SrcBufAddr[DISPROT_BUF_U] = FB_Addr.B0_CB;
            DispRotTrig.SrcBufAddr[DISPROT_BUF_V] = FB_Addr.B0_CR;

            DispRotTrig.bLoadDisp = TRUE;
            disprot_trigger(&DispRotTrig);
            disprot_unlock();
        }
    // Enable video1
    if (uiLogo != LOGO_POWERON)
        ide_wait_frame_end();
    ide_enable_video(IDE_VIDEOID_1);
#endif
}

/**
  Init Touch Panel

  Init Touch Panel

  @param void
  @return void
*/
void SysInit_InitTouchPanel(void)
{
    #if (_TouchPanel_ != DISABLE)
    GESTURE_INIT_STRUCT         TempGestureInit;
    GESTURE_CALI_DATA_STRUCT    TempGestureCalData;
    TP_INIT_STRUCT              TempTPInit;

    GPIOMap_SetTPPin(&TempTPInit);

        #if (_TouchPanel_ == _TouchPanel_VVT4046_)
        tp_init(&TempTPInit);
        #endif

    TempGestureInit.Threshold.SlideRange = 30;
    TempGestureInit.Threshold.HoldRange = 30;
    TempGestureInit.Threshold.HoldTime = 100;
    TempGestureInit.Threshold.DoubleClickTime = 10;
    TempGestureInit.LcdInfo.Width = g_LCDSize.uiWinWidth;
    TempGestureInit.LcdInfo.Height = g_LCDSize.uiWinHeight;

    Gesture_Init(&TempGestureInit);

    TempGestureCalData.ReverseX = FALSE;
    TempGestureCalData.ReverseY = TRUE;
    TempGestureCalData.MinX = 196;
    TempGestureCalData.MinY = 377;
    TempGestureCalData.MaxX = 3864;
    TempGestureCalData.MaxY = 3816;
    TempGestureCalData.TopLeftX = TempGestureCalData.MinX;
    TempGestureCalData.TopLeftY = TempGestureCalData.MinY;
    TempGestureCalData.BottomRightX = TempGestureCalData.MaxX;
    TempGestureCalData.BottomRightY = TempGestureCalData.MaxY;

    Gesture_SetCalData(&TempGestureCalData);
    #endif
}

//#NT#2009/02/05#Hideo Lin -begin
//#Should delay for a while to wait power stable for GiantPlus LCD panel, otherwise will see some garbages
//#To avoid influencing power up time performance, use timer to turn on LCD backlight power
static volatile UINT32  guiLCDPowerDelayTimerID;

void SysInit_LCDPowerDelayTimerHdl(void)
{
    timer_close(guiLCDPowerDelayTimerID);
    GPIOMap_TurnOnLCDBacklight();
}

void SysInit_LCDPowerDelay(void)
{
    if (timer_open((UINT *)&guiLCDPowerDelayTimerID, SysInit_LCDPowerDelayTimerHdl) != E_OK)
        return;

    timer_set(guiLCDPowerDelayTimerID, 140, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
}
//#NT#2009/02/05#Hideo Lin -end

/**
  Init LCD

  Init LCD

  @param void
  @return void
*/
void SysInit_InitLCD(void)
{
    //#NT#2011/08/03#Klins Chen -begin
    PDISP_OBJ       pDispObj;
    DISPCTRL_PARAM  DispCtrl;
    DISPDEV_PARAM   DispDev;
    #if (_LCDTYPE_ == _LCDTYPE_LIL9341_)
    DISPLAYER_PARAM EmuDispLyr;
    UINT32  uiAddr, uiSize;
    DISPROT_PARAM   RotParam;
    #endif


    pDispObj = disp_getDisplayObject(DISP_1);
    pDispObj->open();

    //
    //  Hook Panel / HDMI / TV Display Device Object
    //
    DispDev.SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_TVNTSC;
    DispDev.SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getTvDevObj();
    pDispObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT,&DispDev);

    DispDev.SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_TVHDMI;
    DispDev.SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getHdmiDevObj();
    pDispObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT,&DispDev);

    DispDev.SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_PANEL;
    DispDev.SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getLcd1DevObj();
    pDispObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT,&DispDev);

    DispCtrl.SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL2;
    pDispObj->dispCtrl(DISPCTRL_SET_SRCCLK ,&DispCtrl);

    GPIOMap_SetLCDSifPin(&(g_LCDCtrlObj.LCDCtrl_Pin));

		#if (_LCDTYPE_ == _LCDTYPE_TXDT270M_ && PRJ == JXW)
		DispDev.SEL.SET_REG_IF.LcdCtrl     = DISPDEV_LCDCTRL_SIF;
		#else
    DispDev.SEL.SET_REG_IF.LcdCtrl     = DISPDEV_LCDCTRL_GPIO;
    #endif
    DispDev.SEL.SET_REG_IF.uiSifCh     = SIF_CH0;
    DispDev.SEL.SET_REG_IF.uiGpioSen   = g_LCDCtrlObj.LCDCtrl_Pin.uigpio_sen;
    DispDev.SEL.SET_REG_IF.uiGpioClk   = g_LCDCtrlObj.LCDCtrl_Pin.uigpio_clk;
    DispDev.SEL.SET_REG_IF.uiGpioData  = g_LCDCtrlObj.LCDCtrl_Pin.uigpio_data;
    pDispObj->devCtrl(DISPDEV_SET_REG_IF,&DispDev);

    dma_setChannelPriority(DMA_CH_IDE_Y_INPUT,DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDE_CB_INPUT,DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDE_CR_INPUT,DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_SIE0,DMA_PRIORITY_HIGH);

    //OpenPanel(&g_LCDCtrlObj, &g_LCDSize);
    DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_PANEL;
    pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);

    pDispObj->devCtrl(DISPDEV_GET_DISPSIZE,&DispDev);
    g_LCDSize.uiWidth     = DispDev.SEL.GET_DISPSIZE.uiBufWidth;
    g_LCDSize.uiHeight    = DispDev.SEL.GET_DISPSIZE.uiBufHeight;
    g_LCDSize.uiWinWidth  = DispDev.SEL.GET_DISPSIZE.uiWinWidth;
    g_LCDSize.uiWinHeight = DispDev.SEL.GET_DISPSIZE.uiWinHeight;
    //#NT#2011/08/03#Klins Chen -end

    #if (_LCDTYPE_ == _LCDTYPE_LIL9341_)
        // FW rotation code
        // set status of using GxGfx cpu library
        FWRotate_SetGxCpuStatus(TRUE);
        // start disp_rotate task
        disprot_open();

        EmuDispLyr.SEL.SET_MODE.BufFormat   = DISPBUFFORMAT_YUV420;
        EmuDispLyr.SEL.SET_MODE.BufMode     = DISPBUFMODE_BUFFER_REPEAT;
        EmuDispLyr.SEL.SET_MODE.BufNumber   = DISPBUFNUM_3;
        pDispObj->dispLyrCtrl(DISPLAY_VIDEO1_EN, DISPLAYER_OP_SET_MODE, &EmuDispLyr);

        EmuDispLyr.SEL.SET_WINSIZE.uiWinWidth   = g_LCDSize.uiHeight;//pVdosd->uiWinWidth;
        EmuDispLyr.SEL.SET_WINSIZE.uiWinHeight  = g_LCDSize.uiWidth;//pVdosd->uiWinHeight;
        EmuDispLyr.SEL.SET_WINSIZE.iWinOfsX     = 0;
        EmuDispLyr.SEL.SET_WINSIZE.iWinOfsY     = 0;
        pDispObj->dispLyrCtrl(DISPLAY_VIDEO1_EN, DISPLAYER_OP_SET_WINSIZE, &EmuDispLyr);

        // set FW rotate parameters

        // get FW rotation buffer address&size
        SysGetDispRotateAddr(&uiAddr);
//        uiSize = 320*360;//.uiWidth*g_LCDSize.uiHeight;
        uiSize = 480*272;//.uiWidth*g_LCDSize.uiHeight;

        disprot_lock();

        RotParam.SEL.SET_ROTATE.DispID      = DISP_1;
        RotParam.SEL.SET_ROTATE.DispLayer   = DISPLAYER_VDO1;
        RotParam.SEL.SET_ROTATE.RotDir      = DISPROT_ROTATE90CCW;
        disprot_setParameter(DISPROT_OP_SET_ROTATE, &RotParam);
        FWRotate_SetStatus(RotParam.SEL.SET_ROTATE.RotDir);


        RotParam.SEL.SET_BUF_FMT.DispID      = DISP_1;
        RotParam.SEL.SET_BUF_FMT.DispLayer   = DISPLAYER_VDO1;
        RotParam.SEL.SET_BUF_FMT.BufFmt      = DISPBUFFORMAT_YUV420;
        disprot_setParameter(DISPROT_OP_SET_BUF_FMT, &RotParam);


        RotParam.SEL.SET_OUTBUF_ADDR.DispID     = DISP_1;
        RotParam.SEL.SET_OUTBUF_ADDR.DispLayer  = DISPLAYER_VDO1;
        RotParam.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_0][DISPROT_BUF_Y] = uiAddr;
        RotParam.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_0][DISPROT_BUF_U] = uiAddr + uiSize;
        RotParam.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_0][DISPROT_BUF_V] = uiAddr + (uiSize * 5) / 4;
        RotParam.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_1][DISPROT_BUF_Y] = uiAddr + (uiSize * 6) / 4;
        RotParam.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_1][DISPROT_BUF_U] = uiAddr + (uiSize * 10) / 4;
        RotParam.SEL.SET_OUTBUF_ADDR.uiBufAddr[DISPACTBUF_1][DISPROT_BUF_V] = uiAddr + (uiSize * 11) / 4;
        disprot_setParameter(DISPROT_OP_SET_OUTBUF_ADDR, &RotParam);

        disprot_unlock();
    #else
        // set status of using GxGfx cpu library
        FWRotate_SetGxCpuStatus(FALSE);
    #endif

    #if (PRJ == ADPK_DSC_DEMO)
    {
        PDISPDEV_OBJ pDispDevObj = dispdev_getLcd1DevObj();
        pDispDevObj->rotate(DISPDEV_LCD_ROTATE_180);
    }
    #endif
	
    //DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_180;//ERIC EDIT
    //pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);//ERIC EDIT
        //DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_NONE;
        //pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);
		
    //#NT#2009/02/05#Hideo Lin -begin
    //#Should delay for a while to wait power stable for GiantPlus LCD panel, otherwise will see some garbages
    //#To avoid influencing power up time performance, use timer to turn on LCD backlight power
    SysInit_LCDPowerDelay();
    //#NT#2009/02/05#Hideo Lin -end

    SysInit_InitTouchPanel();
}

void SysInit_InitENDE(void)
{
#if(_ENCRYPTE_ != ENCRYPTE_NONE)
    g_ENDECtrlObj.ENDECtrl_Pin.uigpio_clk     = GPIO_ENDE_SCL;
    g_ENDECtrlObj.ENDECtrl_Pin.uigpio_data    = GPIO_ENDE_SDA;
    InitEnDe(&g_ENDECtrlObj);
#endif
}

/**
  Init Lens Module

  Init Lens Module

  @param void
  @return void
*/
void SysInit_InitLens(void)
{
    LENS_DEVICE_OBJ LensDevice = {0};
    PLENS_TAB       pLensTab = NULL;
    PMOTOR_TAB      pMotorTab = NULL;
    UINT32          uiBufGPIO[LENS_IO_MAX];
    UINT32          uiBufPAD[LENS_PAD_MAX];
    UINT32          uiBufMotorParam[LENS_MOTOR_PARAM_MAX] = {0};

    LensDevice.puiMotorCtrlGPIOArray        = &uiBufGPIO[0];
    LensDevice.puiMotorCtrlPADArray         = &uiBufPAD[0];
    LensDevice.puiMotorCtrlParamArray       = &uiBufMotorParam[0];

    // Init lens object (get lens function table)
    pLensTab                                = lens_getObject();

    // Init motor object (get motor function table)
    pMotorTab                               = motor_getObject();

    // Set lens control related GPIO
    GPIOMap_SetLensGpio(&LensDevice);

    // Register lens calibration information callback
    //LensDevice.fpLensCalCB = (FP_LENS_CAL_CB)UILens_CalInfoCB;

    Lens_Module_Init(&LensDevice, pLensTab, pMotorTab);

    // Register lens callback
    //LensCtrl_RegCB((FPLENSCALLBACK)FlowPhoto_ImageCB);

    // Register lens control key detection callback
    //LensCtrl_RegKeyCB((FP_LENS_KEY_CB)UILens_KeyDetCB);

    #if (_LENSLIB_ != _LENSLIB_FF_)
    CalLens_ReadCalData();
    Lens_Focus_SetBL(CalLens_GetFocusBL());
    Lens_Zoom_SetBL(CalLens_GetZoomBL());
    // If the USB is plugged as power on, don't initilize the lens.
    if (usb_StateChange() == FALSE)
    {
        rtc_resetShutdownTimer();
        Lens_Init(LENS_INIT_ZOOM_PART1);
    }
    #endif
}

/**
  Init sensor

  Init sensor

  @param void
  @return void
*/
#if (_IQ_SIM_ == ENABLE)
static void SysInit_InitSensor(void)
{
    //dummy
}
#else
static void SysInit_InitSensor(void)
{
    SENSOR_APPOBJ   SensorObj;

/*
    //#NT#2010/12/21#ethanlau -begin
    //#NT#tmp for pimux select under 96220
    pinmux_select_sensor(PINMUX_SENSOR_10BIT);//include vd/hd/pclk
    pinmux_select_mclk(PINMUX_MCLK);
    pinmux_swapSensor(PINMUX_GPIO);//normal order
    //#NT#2010/12/21#ethanlau -end
*/

#if (_SENSORLIB_ == _SENSORLIB_OV5653_) // PLL1 is 480MHz, PLL2 is 540MHz
    GPIOMap_TurnOnSensorPower();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv  = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0x13; // 24MHz
    #endif

    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;
#elif (_SENSORLIB_ == _SENSORLIB_MI5100_)
    GPIOMap_TurnOnSensorPower();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv  = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 10; // 48MHz
    #endif

    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;
#elif (_SENSORLIB_ == _SENSORLIB_MI1310_)
    GPIOMap_TurnOnSensorPower();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0x13; // 24MHz
    #endif

    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X720;
#elif (_SENSORLIB_ == _SENSORLIB_MI5130_)
    GPIOMap_TurnOnSensorPower();

    GPIOMap_ResetSensor();
    TimerDelayMs(2); // 2400 * EXTCLK
    GPIOMap_ResumeSensor();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 29; // 18MHz
    #endif

    SensorObj.uiSifch[0] = 2; // for adpk 5130 cost down sensor; 1=>50KHz, 2=>100KHz, 3=>200KHz
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;
#elif (_SENSORLIB_ == _SENSORLIB_OV5633_)
    GPIOMap_TurnOnSensorPower();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

#if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
#else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0x13; // 24MHz
#endif
    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;
#elif (_SENSORLIB_ == _SENSORLIB_MI9130_)
    GPIOMap_TurnOnSensorPower();

    GPIOMap_ResetSensor();
    TimerDelayMs(2);
    GPIOMap_ResumeSensor();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

#if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
#else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 29; // 18MHz
#endif

    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_640X480;
//#NT#2011/06/20#Jarkko Chang -begin
//#NT# add a new sensor lib
#elif (_SENSORLIB_ == _SENSORLIB_OV9712_)
    GPIOMap_TurnOnSensorPower();

    GPIOMap_ResetSensor();
    //TimerDelayMs(2); // 2400 * EXTCLK
    //GPIOMap_ResumeSensor();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0X13; // 24MHz
#endif

    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_FULL;
#elif (_SENSORLIB_ == _SENSORLIB_H22_)

    //TimerDelayMs(2); // 2400 * EXTCLK
    GPIOMap_ResetSensor();    
    //TimerDelayMs(2);
    //GPIOMap_ResumeSensor();
     
    // Register sensor driver and sensor info
     //SensorObj.pSensorDrv = Sensor_GetDrvTab();
     //SensorObj.pSensorInfo = Sensor_GetDrvInfo();
    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0X13; // 24MHz
     #endif

    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(SensorObj.uiMClkDiv));
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, SensorObj.uiMClkSrc);
    pll_enableClock(SIE_MCLK);
    SensorObj.uiPrvMode = SENSOR_MODE_FULL; 
#elif (_SENSORLIB_ == _SENSORLIB_MI1600_)
    GPIOMap_TurnOnSensorPower();

    GPIOMap_ResetSensor();
    TimerDelayMs(2);
    GPIOMap_ResumeSensor();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv  = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 44; // 540/(44+1)=12MHz
    #endif

    SensorObj.uiSifch[0] = 0;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X720;

#elif (_SENSORLIB_ == _SENSORLIB_MI5131E_)
    GPIOMap_TurnOnSensorPower();

    GPIOMap_ResetSensor();
    TimerDelayMs(2); // 2400 * EXTCLK
    GPIOMap_ResumeSensor();

    // Register sensor driver and sensor info
    //SensorObj.pSensorDrv = Sensor_GetDrvTab();
    //SensorObj.pSensorInfo = Sensor_GetDrvInfo();

    #if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL2;
    SensorObj.uiMClkDiv = 1;
    #else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0x3b; // 8MHz
    #endif

    SensorObj.uiSifch[0] = 2;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;
#elif (_SENSORLIB_ == _SENSORLIB_NT99140_)// PLL1 is 480MHz, PLL2 is 540MHz
#if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 1;
#else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0x13; // 8MHz
#endif

    SensorObj.uiSifch[0] = 2;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;

    pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(SensorObj.uiMClkDiv));
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, SensorObj.uiMClkSrc);
    pll_enableClock(SIE_MCLK);
    GPIOMap_TurnOnSensorPower();

    GPIOMap_ResetSensor();
    TimerDelayMs(2); // 2400 * EXTCLK
    GPIOMap_ResumeSensor();
#elif (_SENSORLIB_ == _SENSORLIB_NT99141_)// PLL1 is 480MHz, PLL2 is 540MHz
#if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 1;
#else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0x13; // 8MHz
#endif

    SensorObj.uiSifch[0] = 2;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;

    pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(SensorObj.uiMClkDiv));
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, SensorObj.uiMClkSrc);
    pll_enableClock(SIE_MCLK);
    GPIOMap_TurnOnSensorPower();

    GPIOMap_ResetSensor();
    TimerDelayMs(2); // 2400 * EXTCLK
    GPIOMap_ResumeSensor();
#elif (_SENSORLIB_ == _SENSORLIB_NT99143_)// PLL1 is 480MHz, PLL2 is 540MHz
#if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 1;
#else
    SensorObj.bSieMClkEn = ENABLE;
    SensorObj.uiMClkSrc = PLL_CLKSEL_SIE_MCLKSRC_PLL1;
    SensorObj.uiMClkDiv = 0x13; // 24MHz
#endif

    SensorObj.uiSifch[0] = 2;
    SensorObj.uiSifch[1] = 0; // assign sif channel
    SensorObj.uiPrvMode = SENSOR_MODE_1280X960;

    pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(SensorObj.uiMClkDiv));
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, SensorObj.uiMClkSrc);
    pll_enableClock(SIE_MCLK);
    GPIOMap_TurnOnSensorPower();
    debug_err(("^R_SENSORLIB_NT99143_!!!!!!!!\r\n"));
    GPIOMap_ResetSensor();
    //TimerDelayMs(50); // 2400 * EXTCLK
    //GPIOMap_ResumeSensor();
#else
    debug_err(("^R No Sensor Object!!!!!!!!\r\n"));
#endif
//#NT#2011/06/20#Jarkko Chang -end
    // Open sensor module
    Sensor_Open(&SensorObj);
}
#endif

/**
  Init audio

  Init audio

  @param void
  @return void
*/
static void SysInit_InitAudio(void)
{
    AUDIO_SETTING       AudioSetting;
    AUDIO_DEVICE_OBJ    AudioDevice;

    // Each task using Audio must set the corresponding EventHandler if desired
    AudioDevice.pEventHandler       = NULL;
    // The following settings are "Don't care" for NT96432 built-in audio codec.
    AudioDevice.uiGPIOColdReset     = 0;
    AudioDevice.uiI2SCtrl           = AUDIO_I2SCTRL_NONE;
    AudioDevice.uiChannel           = 0;
    AudioDevice.uiGPIOData          = 0;
    AudioDevice.uiGPIOClk           = 0;
    AudioDevice.uiGPIOCS            = 0;
    AudioDevice.uiADCZero           = 0;

    // The following settings are for NT96432 built-in audio codec.
    // Don't modify the value that is "must be".
    // Set default sampling rate, channel, etc.
    AudioSetting.Clock.bClkExt      = FALSE;                        // Must be
    AudioSetting.Clock.Clk          = AUDIO_CLK_12288;              // Clock = Sampling rate * 256
    AudioSetting.I2S.bMaster        = TRUE;                         // Must be
    AudioSetting.I2S.I2SFmt         = AUDIO_I2SFMT_LIKE_LSB;        // Must be
    AudioSetting.I2S.ClkRatio       = AUDIO_I2SCLKR_256FS_32BIT;    // Must be
    AudioSetting.Fmt                = AUDIO_FMT_I2S;                // Must be
    AudioSetting.SamplingRate       = AUDIO_SR_48000;               // Sampling rate = Clock / 256
    AudioSetting.Channel            = AUDIO_CH_RIGHT;               // Audio channel
    AudioSetting.RecSrc             = AUDIO_RECSRC_MIC;             // Must be
    AudioSetting.Output             = AUDIO_OUTPUT_SPK;             // Output source
    AudioSetting.bEmbedded          = TRUE;                         // Must be, if you want to use the audio I/O pin

    // Open audio driver
    aud_open(&AudioDevice);

    // Init Audio driver
    aud_init(&AudioSetting);

    // Enable ALC function
    aud_setFeature(AUDIO_FEATURE_ALC, TRUE);
    // Disable ALC function
    //aud_setFeature(AUDIO_FEATURE_ALC, FALSE);

    //Enable DC CAN
    eac_configDccan(EAC_DCCAN_TYPE2);


    // Enable Pre-amplifier
    //aud_setFeature(AUDIO_FEATURE_PRE_AMPLIFIER_EN, TRUE);
    // Disable Pre-amplifier
    //aud_setFeature(AUDIO_FEATURE_PRE_AMPLIFIER_EN, FALSE);

    // Enable MIC bias function
    //aud_setFeature(AUDIO_FEATURE_MICBIAS_EN, TRUE);
    // Disable MIC bias function
    //aud_setFeature(AUDIO_FEATURE_MICBIAS_EN, FALSE);

    //aud_setParameter(AUDIO_PARAMETER_NOISETHD_WITHOUT_BOOST,0x17); // 砞
    aud_setParameter(AUDIO_PARAMETER_NOISETHD_WITHOUT_BOOST,0x1c); // 砞

    // Enable MIC boost
    //aud_setFeature(AUDIO_FEATURE_MICBOOST, TRUE);
    //aud_setFeature(AUDIO_FEATURE_NG_WITH_MICBOOST, FALSE);
    // Disable MIC boost (If have Pre-Amp circuit, don't need MIC boost)
    //#NT#2011/08/11#KS Hung -begin
    aud_setFeature(AUDIO_FEATURE_MICBOOST, FALSE);   // 砞

    //#NT#2011/08/11#KS Hung -end
    aud_setFeature(AUDIO_FEATURE_NG_WITH_MICBOOST, FALSE);

    // Enable DC cancellation
    //aud_setFeature(AUDIO_FEATURE_DC_CANCELLATION_EN, TRUE);

    // Enable Noise Gate function
    //aud_setFeature(AUDIO_FEATURE_NOISE_GATE_EN, TRUE);
    // Disable Noise Gate function
    //aud_setFeature(AUDIO_FEATURE_NOISE_GATE_EN, FALSE);

    // Set default Gain
    //aud_setGain(AUDIO_GAIN_7);
    aud_setGain(AUDIO_GAIN_1);

    // Set MAX ALC Gain
    aud_setParameter(AUDIO_PARAMETER_ALC_MAXGAIN, 0x0);

    // Set default Volume
    aud_setVolume(AUDIO_VOL_7);
//    aud_setVolume(GetAudioVolume());
    //#NT#2011/08/01#KS Hung -begin
    // Disable DAC power
    aud_setFeature(AUDIO_FEATURE_SPK_PWR_ALWAYSON,FALSE);
    //#NT#2011/08/01#KS Hung -end
    // Let another task can open Audio driver
    aud_close();
}

/**
  Init sound play method

  Init sound play method

  @param void
  @return void
*/
static void SysInit_InitSoundPlay(void)
{
    //#if (_SUDPLAY_USE_ == _SUDPLAY_USE_TSK_)
    // Open Play Sound Task
    PlaySound_Open(NULL);
    //#endif

    // Play opening sound
    //KeyScan_PlaySound(KEYSCAN_SOUND_STARTUP); //need to read PStore parameter
}

/**
  Init IR Remote Module

  Init IR Remote Module

  @param void
  @return void
*/
#if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
static void SysInit_InitIrRx(void)
{
    IRRX_DEVICE_OBJ IrRxDevice;

    memset((void *)&IrRxDevice, 0, sizeof(IRRX_DEVICE_OBJ));

    // Assign IrRx IO
    GPIOMap_SetIrRxGpio((PIRRX_DEVICE_OBJ)&IrRxDevice);

    // Open IrRx driver
    irrx_open((PIRRX_DEVICE_OBJ)&IrRxDevice);
}
#endif

/**
  Init bootloader info object from memory

  Init bootloader info object from memory

  @param void
  @return void
*/
static void SysInit_InitBLInfo(void)
{
    debug_ind(("SysInit_InitBLInfo\n\r"));

    Init_BLInfo();
}

/**
  Init memory write protection

  Init memory write protection.
  Starting address must be word-alignment.

  @param void
  @return void
*/
static void SysInit_InitMemWP(void)
{
    DMA_WRITEPROT_ATTR tDmaAttr = {0};

    tDmaAttr.uiProtectSrc = DMA_WPSRC_DMA;
    tDmaAttr.uiProtectlel = DMA_WPLEL_DETECT;

    /* Write protect area 0: Main code area */
    tDmaAttr.uiStartingAddr = 0x00000000;
    tDmaAttr.uiSize = FRE_MEM;
    dma_configWPFunc(DMA_WPSET_0, &tDmaAttr, NULL);
    dma_enableWPFunc(DMA_WPSET_0);

    /* Write protect area 1: Stack/Heap area */
    tDmaAttr.uiStartingAddr = END_MEM;
    tDmaAttr.uiSize = (DRAM_END - END_MEM);
    dma_configWPFunc(DMA_WPSET_1, &tDmaAttr, NULL);
    dma_enableWPFunc(DMA_WPSET_1);
}

/**
  Init hardware

  Init hardware

  @param void
  @return void
*/
void SysInit_InitGSensor(void)
#if 0
{
    	InitGSENSOR();
	if (GSensor_GMA301_Read_Interrupte())
	{
		//debug_err(("^R##########SetGsensorPowerOn\r\n"));
		SetGsensorPowerOn(TRUE);
	}
	else
	{
		SetGsensorPowerOn(FALSE);
	}
	GSensor_GMA301_Init();
}
#else
{
    InitGSENSOR();

    if (mir3da_read_interrupt_statu())
	{
		SetGsensorPowerOn(TRUE);
	}
	else
	{
		SetGsensorPowerOn(FALSE);
	}
	
    mir3da_chip_init();
}

#endif
static void SysInit_InitHW(void)
{
#if (SYSINIT_OUTPUT_MCLK == ENABLE)
    // SIE output MCLK to sensor
//    pinmux_select_mclk(PINMUX_MCLK);
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_PLL2);
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(SYSINIT_MCLKDIV));
    pll_enableClock(SIE_MCLK);
#endif

#if (_FPGA_EMULATION_ == 0)
    // Default USB high speed enabled
    //#NT#2009/07/14#Niven Cho -begin
    //#NT#Modified., New USB Driver Support
    // Default USB high speed enabled
    usb_PowerOnInit(TRUE);
    //#NT#2009/07/14#Niven Cho -end
#endif
    //#NT#2009/12/15#Lily Kao -begin
    //if the customer wants to change DeviceRescriptor's bcdUSB value,otherwise it is 0x0200 for High Speed
    //and 0x0110 for full speed. The first parameter means High speed ot not.
    usb_SetDevDescUSBVerRelNum(FALSE, 0x0200);
    //#NT#2009/12/15#Lily Kao -end
#if(_ENCRYPTE_ != ENCRYPTE_NONE)
   // Init Encrypte Funtion
    SysInit_InitENDE();
    EnDeFunction();//MP
#endif     

#if 0//henry20150420
    BIT_I2C_Init();

    I2C_TxBurst (0x00, 0x05,sizeof(BIT1612M_ITU656_BANK0),BIT1612M_ITU656_BANK0);
    I2C_TxBurst (0x02, 0x58,sizeof(BIT1612M_BANK1),BIT1612M_BANK1);
    I2C_TxByte (0x02,0x58,0x08);
    I2C_TxByte (0x02,0x58,0x0A);
    I2C_TxByte (0x00,0xee,0xcf);
    I2C_TxByte (0x00,0xee,0xce);
	
    SysInit_InitLCD();
#endif
  
    SysInit_InitSensor();

    SysInit_InitAudio();

    VolDet_Init();

    KeyADC_Init();

    //#NT#2010/03/31#JustinLee -begin
    // Init IR Remote controller
    #if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
    SysInit_InitIrRx();
    #endif
    //#NT#2010/03/31#JustinLee -end

    // Init sound play
    SysInit_InitSoundPlay();
#if (_GSENSOR_FUNC_ == ENABLE)	
    //Init gsensor    
    SysInit_InitGSensor();		
#endif
}

#if 0
#pragma mark -
#endif

/**
  Get system parameters from NAND

  Get system parameters from NAND

  @param void
  @return void
*/
static void SysInit_GetNANDSysParam(void)
{
    // restore UI seting
    Init_MenuInfo();
}

/**
  Get sensor adjust data from NAND

  Get sensor adjust data from NAND

  @param void
  @return void
*/
void SysInit_GetNANDSensorParam(void)
{
    ER err_code;

    //#NT#2011/06/22#Chris Chung -begin
    //#NT#Add to avoid hung-up if PStore is NG.
    if (SysInit_GetPStoreStatus() != E_OK)
    {
        return;
    }
    //#NT#2011/06/22#Chris Chung -end

    err_code = Cal_ReadCalData();
    debug_err(("check num 0x%x\r\n",err_code));
}

/**
  Initialize Persist Store

  Initialize Persist Store.

  @param void
  @return void
*/
// Temporary solution to check PStore init status
static INT32 g_iPStoreStatus = E_OK;
INT32 SysInit_GetPStoreStatus(void)
{
    return g_iPStoreStatus;
}

UINT32 SysInit_GetPStoreSize(void)
{
    UINT32 uiSize = 0;

#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
    // SPI: 320KB, temp size
#if (UI_STYLE == UI_STYLE_DRIVE)
    uiSize = (500 * 1024);
#else
    uiSize = (384 * 1024);   //TODO220: Temporarily changed to smaller size
#endif
#else
    #if 0
    // NAND: 1MB
    uiSize = (1 * 1024 * 1024);
    #else
    // NAND: 6MB, MP3 unicode font need more 5MB size!
    uiSize = (6 * 1024 * 1024);
    #endif
#endif

    //debug_err(("SysInit_GetPStoreSize: PStore size = 0x%08X\n\r", uiSize));
    return uiSize;
}

//#NT#2008/01/04#Chris Hsu -begin
//#PStore size must set before 1st nand_open()
void SysInit_InitPStore(void)
{
    PSTORE                  PStoreStruct;
    PPSTORE_SECTION_HANDLE  pSection;
    PPSSTRG_TAB             pPSStorage;

//#NT#2008/12/04#Hideo Lin -begin
//#NT#Call NAND related functions only in using NAND condition (for partial load issue)
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)

#if ((_MSDC_MODE_ == _MSDC_ALL_NAND3_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_) || (_MSDC_MODE_ == _MSDC_ALL_NAND2_))
    nand2_setSize(NAND2_SIZE);
#endif
#if ((_MSDC_MODE_ == _MSDC_SINGLE_NAND3_) || (_MSDC_MODE_ == _MSDC_ALL_NAND3_))
    nand3_setSize(NAND3_SIZE);
#endif

    // Set reserved size for FW code
    nand_setReservedSize(FW_CODE_SIZE);

    // Set system parameter size, ver 2.00 don't need this size
    nand_setSysParamBlockNum(0);
#endif
//#NT#2008/12/04#Hideo Lin -end

    // Init PSTore size
    PStoreStruct.uiSignature  = 0x55AA55AA;
    PStoreStruct.uiPSVersion  = 0x0001;
    PStoreStruct.uiPStoreSize = SysInit_GetPStoreSize();

    // Get PStore storage object
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
    pPSStorage = nandps_getStorageObject();
#elif (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
    pPSStorage = norps_getStorageObject();
#else
    debug_err(("PStore type has not selected yet and PStore is not supported\r\n"));
#endif

    // Open PStore
    g_iPStoreStatus = PStore_Init(&PStoreStruct, pPSStorage);
    //switch (PStore_Init(&PStoreStruct, pPSStorage))
    switch (g_iPStoreStatus)
    {
    case E_PS_CHKER:
    case E_PS_EMPTY:
    case E_PS_VERNEW:
        // Enable PStore
        PStore_EnablePS();

        // Format PStore
        g_iPStoreStatus = PStore_Format(&PStoreStruct);

        // Create default section
        // System parameter
        pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR | PS_CREATE);
        PStore_CloseSection(pSection);
//#NT#2011/06/03#Ethan Lau -begin
//#NT#remove,PS_CAL_DATA sec remove to ipp lib ext
        // Calibration data
        //pSection = PStore_OpenSection(PS_CAL_DATA, PS_RDWR | PS_CREATE);
        //PStore_CloseSection(pSection);
//#NT#2011/06/03#Ethan Lau -end
        break;
    case E_PS_VEROLD:
        break;

    case E_OK:
        // Everything is OK, open system parameter section?
        break;
    }
}

void SysInitExifHeader(void)
{
    Vendor_EXIF_HEAD userExifHeader;

    //userExifHeader.pMakerString = (CHAR *)GetMakerString();
    //userExifHeader.pModelString = (CHAR *)GetModelString();
    //userExifHeader.pSoftwareVer = (CHAR *)GetVersionString();

    #if 1//(UPDATE_CFG == UPDATE_CFG_YES)
    //userExifHeader.pMakerString = (CHAR *)GetUIConfigMakerString();
    //userExifHeader.pModelString = (CHAR *)GetUIConfigModelString();
    userExifHeader.pMakerString = (CHAR *)GetMakerString();
    userExifHeader.pModelString = (CHAR *)GetModelString();
    userExifHeader.pSoftwareVer = (CHAR *)GetUIConfigSoftwareVer();
    #endif

    userExifHeader.pMakeNote = NULL;
    userExifHeader.pImageDescription = NULL;
    userExifHeader.pUserComment = SysGetSocialNetworkData();

    JpgSetEXIFString(&userExifHeader);
    ExifW_ClearInsertTag();//2010/12/20 Meg Lin
    UserEXIF_InsertTags(); // Insert user defined EXIF tags
}


#if (UI_STYLE == UI_STYLE_DRIVE)
CHAR pFolderName[6]="MEDIA";
CHAR pFileName[5]="PTDC";

static void SysInit_InitTimeDCFFolder(void)
{

  RTC_DATE  curDate;
  RTC_TIME  curTime;

    curDate = rtc_getDate();
    curTime = rtc_getTime();
    sprintf(pFolderName,"%1d%02d%02d",curDate.s.year%0x0A,curDate.s.month,curDate.s.day);
    sprintf(pFileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
}
#endif

/**
  Init firmware

  Init firmware

  @param void
  @return void
*/
static void SysInit_InitFW(void)
{
    UINT32 uiPoolAddr;

    PRIMARY_DCFNAME DCFName = {
                                FALSE,       // Create folder when init File System or not
                                "MEDIA",    // Folder name
                                "PTDC",     // File name
                                "PTDC",     // Video file name
                                "AVI",      // Video extension name
                                "PTDC",     // Audio file name
//#NT#2009/03/24#Hideo Lin -begin
//#NT#For wav playback test
#if (_WAVSTUDIO_MODE_ == ENABLE)
                                "WAV",      // Audio extension name
#else
                                "",         // Audio extension name     // WAV file is not allowed in TDC8A2VM project
#endif
//#NT#2009/03/24#Hideo Lin -end
                                "PTDC",     // RAW file name
                                "PTDC",     // TIFF file name
                                //#NT#2011/04/08#Chris Chung -begin
                                //#add custom type
                                "BIN",      // Custom extension name
                                //#NT#2011/04/08#Chris Chung -end
                              };

    PSTRG_TAB   pStorageObj;

#if (_RAM_DISK_ == ENABLE)
    RamDiskSettings     RamDisk;
#endif

    // Init Date/Time folder/file name for Car Recorder
#if (UI_STYLE == UI_STYLE_DRIVE)
    SysInit_InitTimeDCFFolder();
    memcpy(DCFName.pDirName,pFolderName,sizeof(pFolderName));
    memcpy(DCFName.pFileName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pVideoName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pAudioName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pRAWName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pTIFFName,pFileName,sizeof(pFileName));
#endif

    // Initialize DCF name
    Primary_InitDCFName(&DCFName);

    //#NT#2007/12/27#HH Chuang -begin
    //#NT#support undelete
    FilesysEnableRecycler(FALSE);
    //#NT#2007/12/27#HH Chuang -end

    // Initialize card's storage object for file system
#if (_STORAGECARD_ == _STORAGECARD_SDIO_)
    pStorageObj = sdio_getStorageObject();
    sdio_setDetectCardExistHdl(SDIOCardUser_CheckCardInserted);
    sdio_setDetectCardProtectHdl(SDIOCardUser_CheckCardWP);

//#NT#2011/07/06#Chris Hsu -begin
//#NT#Add sample code to config SD driver for new APIs
//#NT#Todo: Add sample code to active and use SSPLL
    #if 0
    // Enable High Speed mode (Default is enabled)
//    sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,       (UINT32)(SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS));
    #else
    //#NT#2012/06/12#KS Hung -begin
    sdio_setConfig(SDIO_CONFIG_ID_DIS_CMD17_RANGE_CHECK, TRUE);
    //#NT#2012/06/12#KS Hung -end
    // Enable High Speed mode but max bus speed is 24MHz
    sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,   (UINT32)SDIO_BUS_SPEED_MODE_HS);
    sdio_setConfig(SDIO_CONFIG_ID_HS_MAX_CLK,       (UINT32)24000000);
    #endif

  #if 0
    // Enable setting driving sink of clock pin
    sdio_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,      (UINT32)TRUE);
    // Default driving is 5.0 mA
    /*
    // Set clock pin driving in default speed to 5.0 mA
    sdio_setConfig(SDIO_CONFIG_ID_DS_DRIVING,           (UINT32)50);
    // Set clock pin driving in high speed mode to 5.0 mA
    sdio_setConfig(SDIO_CONFIG_ID_HS_DRIVING,           (UINT32)50);
    */
  #endif

    // Set maximum bus width to 8 bits (Default is 4 bits)
//    sdio_setConfig(SDIO_CONFIG_ID_BUS_WIDTH,            (UINT32)SD_HOST_BUS_8_BITS);

    // Set format OEM ID
    sdio_setConfig(SDIO_CONFIG_ID_FORMAD_OEMID,         (UINT32)"NOVATEK");
    // Set format volume label
    sdio_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_LABLE,     (UINT32)"SD_VOL");
#if 1
    // Random volume ID
    sdio_setConfig(SDIO_CONFIG_ID_FORMAD_RAND_VOL_ID_EN,(UINT32)TRUE);
  #else
    // Fixed volume ID
    sdio_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_ID,        (UINT32)0x00000000);
#endif
//#NT#2011/07/06#Chris Hsu -end
#endif

    Primary_InitStorageCard(pStorageObj);

#if (_RAM_DISK_ == ENABLE)
    SysGetRamDiskAddr(&RamDisk.baseaddr);
    RamDisk.size = POOL_SIZE_RAM_DISK;
    ramdsk_init(&RamDisk);
    ramdsk_open();
    ramdsk_format();
    ramdsk_close();
#endif

    // Init Gx Gfx
    get_blk((VP*)&(uiPoolAddr), POOL_ID_GX);
    rel_blk(POOL_ID_GX, (VP)uiPoolAddr);
    debug_err(("\n\r"));
    debug_err(("GX buf=%08x, size=%d;\n\r",uiPoolAddr, POOL_SIZE_GX));
    GxGfx_Init((UINT32*)uiPoolAddr,POOL_SIZE_GX);       //initial Graphics

    // Init Gx Display
    debug_err(("GX win.w=%d, win.h=%d;\n\r",g_LCDSize.uiWinWidth, g_LCDSize.uiWinHeight));
    GxDisplay_Init(0, g_LCDSize.uiWinWidth, g_LCDSize.uiWinHeight);

    // Init UI Render for UI Framework
    Ux_SetRender(&demoRender);

    // Init OSD buffer
    UI_BeginDisplay(DD_OSD, TRUE);

    // Init VDO buffer
    //UI_BeginDisplay(DD_VDO, TRUE);

    //#NT#2011/08/03#KS Hung -begin
    //#NT#movie to before SysInit_InitFW
    // Read system parameters from NAND
    //SysInit_GetNANDSysParam();
    //#NT#2011/08/03#KS Hung -end

    // Read sensor adjust data from NAND
    SysInit_GetNANDSensorParam();

    // Init EXIF
    SysInitExifHeader();
#if ( _GPS_RECEIVER_LOG_ == ENABLE)
    // Init GPS Log buffer
    // Get memory for GPS log task
    get_blk((VP*)&(uiPoolAddr), POOL_ID_DEFECT_PIXEL);
    rel_blk(POOL_ID_DEFECT_PIXEL, (VP)uiPoolAddr);
    GPSRec_SetGPSBufAddr(uiPoolAddr,(UINT32)(POOL_SIZE_DEFECT_PIXEL));
#endif
    // Init bootloader info object from memory
    //SysInit_InitBLInfo();
}

/**
  System RaiseHighClk

  Do system RaiseHighClk

  @param void
  @return void
*/
//#NT#2011/06/14#Meg Lin -begin
//#NT#Increase system clock to improve power on time (test)
void SysInit_RaiseHighClk(void)
{
    //DMA_TIMING  DMATiming;
    DMA_SETTING DMASetting;
    UINT32      uiNewCPUClock, uiNewAHBClock, uiNewDMAClock;

    // CPU = 240MHz, AHB = 120MHz, DMA = 160MHz
    uiNewCPUClock = PLL_CLKSEL_CPU_240;
    uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK_D2;
    uiNewDMAClock = PLL_CLKSEL_DMA_160;
    SysClk_GetDMA160MHzSetting(&DMASetting);

    clk_changeDMA(uiNewDMAClock, NULL, &DMASetting);
    clk_changeCPUAHB(uiNewCPUClock, uiNewAHBClock);

    if(dma_getDRAMType() == DMA_DRAM_TYPE_SDR)
    {
        #if (DRAM_END == 0x00800000)
        debug_err((">>>>>>>> SDRAM 8MB type\n\r"));
        #else
        debug_err((">>>>>>>> SDRAM 16MB type\n\r"));
        #endif
    }
    else
    {
        #if (DRAM_END == 0x00800000)
        debug_err((">>>>>>>> DDRI 8MB type"));
        #elif(DRAM_END == 0x01000000)
        debug_err((">>>>>>>> DDRI 16MB type"));
        #elif(DRAM_END == 0x02000000)
        debug_err((">>>>>>>> DDRI 32MB type"));
        #endif
    }
}
//#NT#2011/06/14#Meg Lin -end


#define RAND_MAX_NUM 100

#if(_ENCRYPTE_ == CK235_PW01)
int EnDeFunction(void)
{
	//	8字节随机数
	UINT8 randata[8]={0x29,0xf5,0x25,0xf3,0xaa,0x6f,0x84,0xe9};
	//	10字节加密后的数据
	UINT8 returndata[10]={0};
	//	8字节解密后的数据
	UINT8 exdata[8]={0};

	UINT8 i=0;
	UINT8 j=0;

 	RTC_TIME  curTime;

    curTime = rtc_getTime();
    //sprintf(pFileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
	//srand( (UINT8)time( NULL ) );
	srand( (UINT8)(curTime.s.second) );

	for(i=0;i<8;i++)
	{
		randata[i]=1+(UINT8)(100.0*rand()/(RAND_MAX_NUM+1.0));
		DEBUG_MSG(("EnDeFunction Rand data[%d] 0x%x\r\n",i,randata[i]));
	}
    _i2c_write(tmp_dev_addr, tmp_sub_addr,randata, 8);
    _i2c_read(tmp_dev_addr, tmp_sub_addr,returndata, 10);

	//	对returndata用密钥key解密,结果在exdata
	EDesEn_Crypt(returndata,exdata);

	//	比较原始数据randata与解密出来的exdata
	for(j=0;j<8;j++)
	{
		if(randata[j] !=exdata[j])
		{
			DEBUG_MSG(("exdata[%d] : 0x%x\r\n",j,exdata[j]));
			break;
		}
		else
		{
			DEBUG_MSG(("EnDeFunction================================>ok %d\r\n",j));
		}
	}
	if(j==8)
	{
		DEBUG_MSG(("EnDeFunction================================>ok\r\n"));
	}
	else
	{
		DEBUG_MSG(("EnDeFunction================================>Err\r\n"));
		KeyScan_PoweroffFlow();
	}
	return 0;
}
#else
int EnDeFunction(void)
{
	//	8字节随机数
	UINT8 randata[8]={0x29,0xf5,0x25,0xf3,0xaa,0x6f,0x84,0xe9};
	//	10字节加密后的数据
	UINT8 returndata[8]={0};
	UINT8 tmpdata[8]={0};
	//	8字节解密后的数据
	UINT8 exdata[8]={0};

	UINT8 a;
	UINT8 b;
	UINT8 c;
	UINT8 d;
	UINT8 e;
	UINT8 f;
	UINT8 g;
	UINT8 h;
	
	UINT8 i=0;
	UINT8 j=0;

 	RTC_TIME  curTime;

    curTime = rtc_getTime();
    //sprintf(pFileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
	//srand( (UINT8)time( NULL ) );
	srand( (UINT8)(curTime.s.second) );

	#if 1
	for(i=0;i<8;i++)
	{
		randata[i]=1+(UINT8)(100.0*rand()/(RAND_MAX_NUM+1.0));
		//DEBUG_MSG(("EnDeFunction================================> %x\r\n",randata[i]));
		debug_msg("EnDeFunction================================> %x\r\n",randata[i]);
	}
	#endif
	//randata[8] = {A*B , B-H, C+D, E*C, A-C, B+F, F*H, A-E}; 

	for(i=0;i<8;i++)
	{
		//randata[i]= i;
		DEBUG_MSG(("EnDeFunction================================> %x\r\n",randata[i]));
		//debug_msg("Send================================> %x\r\n",randata[i]);
	}

	if(randata[0]==0xC1)
	{
		randata[0] = 0xBB;
	}
	a = randata[0];
	b = randata[1];
	c = randata[2];
	d = randata[3];
	e = randata[4];
	f = randata[5];
	g = randata[6];
	h = randata[7];

	tmpdata[0] = a*b;//3d
	tmpdata[1] = b-h;//0c
	tmpdata[2] = c+d;//18
	tmpdata[3] = e*c;//92
	tmpdata[4] = a-c;//04
	tmpdata[5] = b+f;//64
	tmpdata[6] = f*h;//07
	tmpdata[7] = a-e;//7f

	for(i=0;i<8;i++)
	{
		//DEBUG_MSG(("tmpdata================================>Err %x\r\n",randata[i]));
		debug_msg("tmpdata================================>tmpdata %x\r\n",tmpdata[i]);
	}
    _i2c_write(tmp_dev_addr, tmp_sub_addr,randata, 8);
	TimerDelayMs(30);
    _i2c_read(tmp_dev_addr, tmp_sub_addr,returndata, 8);

	for(i=0;i<8;i++)
	{
		//returndata[i]= i;
		DEBUG_MSG(("Read================================> %x\r\n",returndata[i]));
		debug_msg("Read================================> %x\r\n",returndata[i]);
	}		
	//	对returndata用密钥key解密,结果在exdata
	//EDesEn_Crypt(returndata,exdata);

	//	比较原始数据randata与解密出来的exdata
	for(j=0;j<8;j++)
	{
		if(tmpdata[j] !=returndata[j])
		{
			DEBUG_MSG(("EnDeFunction================================>Err %x\r\n",j));
			break;
		}
		else
		{
			DEBUG_MSG(("EnDeFunction================================>ok %x\r\n",j));
		}
	}
	if(j==8)
	{
		DEBUG_MSG(("EnDeFunction================================>ok\r\n"));
	}
	else
	{
		DEBUG_MSG(("EnDeFunction================================>Err\r\n"));
		KeyScan_PoweroffFlow();
	}
	return 0;
}
#endif
/**
  System initialization

  Do system initialization

  @param void
  @return void
*/
void SystemInit(void)
{
    // Init essential HW
    SysInit_InitEssentialHW();

    //#NT#2009/11/27#Hideo Lin -begin
    //#NT#Increase system clock to improve power on time (test)
    //SysInit_RaiseHighClk();
    //#NT#2009/11/27#Hideo Lin -end

    // Start some task before HW and FW initialization
    SysTask_StartTaskPhase1();

    // For partial loading
    // Run part one code
    CopyRest_RunPartOne();

    // Copy part two FW code
    CopyRest_CopyPartTwo();

    // Run part two FW code
    CopyRest_RunPartTwo();

    // Initialize memory write protection
    SysInit_InitMemWP();

    // Initialize HW
    SysInit_InitHW();

    // Init bootloader info object from memory
    SysInit_InitBLInfo();

    //#NT#2011/08/03#KS Hung -begin
    //#NT#get key tone is enable
    SysInit_GetNANDSysParam();
    //#NT#2011/08/03#KS Hung -end

    //#NT#2011/08/18#KS Hung -begin
    // play start up sound
    KeyScan_PlaySound(PLAYSOUND_SOUND_STARTUP);
    //#NT#2011/08/18#KS Hung -end

    // Initialize FW
    SysInit_InitFW();

    // Start some task after HW and FW initialization
    SysTask_StartTaskPhase2();
}

//@}
