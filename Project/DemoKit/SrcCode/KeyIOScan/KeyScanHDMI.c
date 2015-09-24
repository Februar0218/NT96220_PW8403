/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanHDMI.c
    @ingroup    mIPRJAPKey

    @brief      Detect HDMI is plugging in or unplugged
                Detect HDMI is plugging in or unplugged

    @note       Nothing.

    @date       2009/07/06
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "PlaybackTsk.h"
#include "UIFlow.h"
#include "IDE.h"
#include "Top.h"
#include "GlobalVar.h"
#include "Utility.h"
#include "Audio.h"
#include "SystemInit.h"
#include "hdmitx.h"
#include "top.h"
//#NT#2010/10/13#KS Hung -begin
#include "dma.h"
//#NT#2010/10/13#KS Hung -end
#include "CoordinateTrans.h"
#include "pll.h"
//#NT#2011/08/03#Klins Chen -begin
#include "display.h"
//#NT#2011/08/03#Klins Chen -end
#include "disp_rotate.h"

static BOOL     bHDMIStatus = FALSE;
static BOOL     bLastHDMIDet     = FALSE;
static BOOL     bLastHDMIStatus  = FALSE;

//#NT#2009/12/02#Philex -begin
static BOOL     bStartupHDMIStatus  = FALSE;
//#NT#2009/12/02#Philex -end

//#NT#2009/12/10#Philex -begin
void KeyScan_HDMIAudioSetting(AUDIO_SETTING  AudioSetting,BOOL bEnable)
{

    if (aud_getLockStatus() == TASK_LOCKED)
    {
        aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, bEnable);
        aud_setOutput(AudioSetting.Output);
        aud_setSamplingRate(AudioSetting.SamplingRate);
    }
    else
    {
        AUDIO_DEVICE_OBJ    AudObj;

        aud_getDeviceObject(&AudObj);
        aud_open(&AudObj);
        aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, bEnable);
        aud_setOutput(AudioSetting.Output);
        aud_setSamplingRate(AudioSetting.SamplingRate);
        aud_close();
    }

}
//#NT#2009/12/10#Philex -end

/**
  Scaling OSD size between pannel and HDMI

  Scaling OSD size between pannel and HDMI.
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_ScaleDisplayHDMI(void)
{
    VOSD_WINDOW_ATTR    FB_win;
    UINT32              uiSourceW, uiSourceH, uiSourceOff;
    IDE_COLOR_FORMAT    ColorFormat;

    // OSD1
    ide_get_osd1_buf_dim(&uiSourceW, &uiSourceH, &uiSourceOff);
    ide_get_osd1_win_format(&ColorFormat);

    FB_win.Source_W     = uiSourceW;
    FB_win.Source_H     = uiSourceH;
    FB_win.Des_W        = g_LCDSize.uiWinWidth;
    FB_win.Des_H        = g_LCDSize.uiWinHeight;
    FB_win.Win_Format   = ColorFormat;
    FB_win.Win_X        = 0;
    FB_win.Win_Y        = 0;
    FB_win.High_Addr    = 0;
    ide_set_osd_win_attr(IDE_OSDID_1, &FB_win);

    // Video1
    ide_get_video1_buf_dim(&uiSourceW, &uiSourceH, &uiSourceOff);
    ide_get_v1_win_format(&ColorFormat);

    FB_win.Source_W     = uiSourceW;
    FB_win.Source_H     = uiSourceH;
    FB_win.Des_W        = g_LCDSize.uiWinWidth;
    FB_win.Des_H        = g_LCDSize.uiWinHeight;
    FB_win.Win_Format   = ColorFormat;
    FB_win.Win_X        = 0;
    FB_win.Win_Y        = 0;
    FB_win.High_Addr    = 0;

    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
}

/**
  Detect HDMI is plugging in or unplugged

  Detect HDMI is plugging in or unplugged.

  @param void
  @return void
*/
void KeyScan_DetHDMI(void)
{
    BOOL            bCurHDMIDet, bCurHDMIStatus;
    //IMEIDE_ATTR     ImeIdeAttr;
    UINT32          i;
    //#NT#2011/08/03#Klins Chen -begin
    PDISP_OBJ       pDispObj;
    DISPDEV_PARAM   DispDev;
    DISPCTRL_PARAM  DispCtrl;
    //#NT#2009/08/18#Klins Chen#[0006129] -begin
    //#Add to select audio channel to HDMI interface
    AUDIO_SETTING   AudioSetting;
    AudioSetting.SamplingRate       = AUDIO_SR_32000;               // Sampling rate = Clock / 256
    //#NT#2009/08/18#Klins Chen#[0006129] -end

    if ((KeyScan_IsUSBPlug() ==TRUE) || (KeyScan_IsTVPlugIn()==TRUE))
    {
         return;
    }

    pDispObj = disp_getDisplayObject(DISP_1);

    bCurHDMIDet = GPIOMap_DetHDMI();
    bCurHDMIStatus = (BOOL)(bCurHDMIDet & bLastHDMIDet);

    if (bCurHDMIStatus != bLastHDMIStatus)
    {
        PrimaryModeID       CurrMode;

        CurrMode = Primary_GetCurrentMode();        // backup current mode
        //AppInit_Close();
        //debug_err(("[KeyScan_DetHDMI]Primary_ChangeMode(PRIMARY_MODE_DUMMY)\n\r"));
        Primary_ChangeMode(PRIMARY_MODE_DUMMY);     // enter dummy mode firstly
        Primary_Wait4ChangeModeDone();
        //debug_err(("[KeyScan_DetHDMI]Primary_Wait4ChangeModeDone\n\r"));
        // HDMI is plugging in
        if (bCurHDMIStatus == TRUE)
        {
            UINT32          uiWindowEn;

            debug_err(("# HDMI is plugging in #\n\r"));
            bHDMIStatus = TRUE;

            //#NT#2011/07/06#Hideo Lin -begin
            //#NT#For setting IDE DMA priority & burst length
            dma_setChannelPriority(DMA_CH_IDE_Y_INPUT,      DMA_PRIORITY_HIGH);
            dma_setChannelPriority(DMA_CH_IDE_CB_INPUT,     DMA_PRIORITY_HIGH);
            dma_setChannelPriority(DMA_CH_IDE_CR_INPUT,     DMA_PRIORITY_HIGH);
            dma_setChannelPriority(DMA_CH_IDE_OSD_INPUT,    DMA_PRIORITY_MIDDLE);
            ide_setV1Burst(IDE_VIDEO1_Y_BURST_32, IDE_VIDEO1_C_BURST_16);
            ide_setO1Burst(IDE_OSD1_BURST_32);
            //#NT#2011/07/06#Hideo Lin -end

            // Turn off LCD backlight
            GPIOMap_TurnOffLCDBacklight();
            //#NT#2010/05/24#KS Hung -begin
            //#Temp for white screen, why?
            TimerDelayMs(100);
            //#NT#2010/05/24#KS Hung -end
            GPIOMap_TurnOffLCDPower();

            // Disable all windows
            pDispObj->dispCtrl(DISPCTRL_GET_ALL_LYR_EN, &DispCtrl);
            uiWindowEn = DispCtrl.SEL.GET_ALL_LYR_EN.DispLyr;

            DispCtrl.SEL.SET_ALL_LYR_EN.bEn = FALSE;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(TRUE);

            // Close LCD panel
            pDispObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);


            //pinmux_select_lcd(PINMUX_GPIO);
            //Init HDMI
            // This settings is customized by project
            //hdmitx_setHwReset(DGPIO_HDMI_RESET);

            DispDev.SEL.SET_HDMIMODE.VideoID = HDMI_1280X720P60;
            DispDev.SEL.SET_HDMIMODE.AudioID = HDMI_AUDIO32KHZ;
            pDispObj->devCtrl(DISPDEV_SET_HDMIMODE, &DispDev);

            DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_TVHDMI;
            pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);

            pDispObj->devCtrl(DISPDEV_GET_DISPSIZE,&DispDev);
            g_LCDSize.uiWidth     = DispDev.SEL.GET_DISPSIZE.uiBufWidth;
            g_LCDSize.uiHeight    = DispDev.SEL.GET_DISPSIZE.uiBufHeight;
            g_LCDSize.uiWinWidth  = DispDev.SEL.GET_DISPSIZE.uiWinWidth;
            g_LCDSize.uiWinHeight = DispDev.SEL.GET_DISPSIZE.uiWinHeight;
            //#NT#2010/08/09#KS Hung -begin
            //#NT#Display ratio
            UI_SetParameter(_DispRatio, _Ratio_16_9);
            //UI_SetParameter(_CenterViewEn, _CenterView_Off);
            //#NT#2010/08/09#KS Hung -end

            //#NT#2011/11/29#Philex Lin - begin
            // non FW ration while pluging HDMI
            #if (_LCDTYPE_ == _LCDTYPE_LIL9341_)
            FWRotate_SetStatus(DISPROT_ROTATE_NONE);
            // Init OSD buffer again
            UI_BeginDisplay(DD_OSD, TRUE);
            #endif
            // Scaling OSD to new size
            KeyScan_ScaleDisplayHDMI();

            // Enable IDE
            pDispObj->waitFrmEnd();
            DispCtrl.SEL.SET_ENABLE.bEn = TRUE;
            pDispObj->dispCtrl(DISPCTRL_SET_ENABLE,&DispCtrl);


            // Delay to wait for IDE stable
            for (i=6; i>0; i--)
            {
                pDispObj->waitFrmEnd();
            }

            // Enable all windows
            DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = TRUE;
            DispCtrl.SEL.SET_ALL_LYR_EN.DispLyr = uiWindowEn;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(FALSE);

            //#NT#2010/05/03#KS Hung -begin
            //#NT#Temp. Don't support AUDIO_OUTPUT_NONE.
//            aud_setSysClkCtrl(TRUE);
            //AudioSetting.Output             = AUDIO_OUTPUT_NONE;
            AudioSetting.Output             = AUDIO_OUTPUT_LINE;
            KeyScan_HDMIAudioSetting(AudioSetting,TRUE);
            //#NT#2010/05/03#KS Hung -end
            //pinmux_select_cdacclk(PINMUX_AUDIO_CDACCLK);
            //pinmux_select_audio(PINMUX_AUDIO_I2S);
        }
        // HDMI is unplugged
        else
        {
            UINT32 uiWindowEn;

            debug_err(("# HDMI is unplugged #\n\r"));

            bHDMIStatus = FALSE;
            //#NT#2010/06/09#KS Hung -begin
            //#NT#Display ratio
            //#NT#2010/11/12#Hideo Lin -begin
            //#NT#For LCD panel with 16:9 ratio (but 4:3 pixel resolution)
            UI_SetParameter(_DispRatio, _Ratio_4_3);
            //#NT#2010/11/12#Hideo Lin -end
            //#NT#2010/06/09#KS Hung -end

            // Disable all windows
            pDispObj->dispCtrl(DISPCTRL_GET_ALL_LYR_EN, &DispCtrl);
            uiWindowEn = DispCtrl.SEL.GET_ALL_LYR_EN.DispLyr;

            DispCtrl.SEL.SET_ALL_LYR_EN.bEn = FALSE;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(TRUE);

            // Close HDMI
            pDispObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);

            // Open LCD panel
            //OpenPanel(&g_LCDCtrlObj, &g_LCDSize);
            DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_PANEL;
            pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);

            pDispObj->devCtrl(DISPDEV_GET_DISPSIZE,&DispDev);
            g_LCDSize.uiWidth     = DispDev.SEL.GET_DISPSIZE.uiBufWidth;
            g_LCDSize.uiHeight    = DispDev.SEL.GET_DISPSIZE.uiBufHeight;
            g_LCDSize.uiWinWidth  = DispDev.SEL.GET_DISPSIZE.uiWinWidth;
            g_LCDSize.uiWinHeight = DispDev.SEL.GET_DISPSIZE.uiWinHeight;

            CoordinateTrans_SetSizeTarget(g_LCDSize.uiWidth,g_LCDSize.uiHeight);


            // Scaling OSD to new size
            KeyScan_ScaleDisplayHDMI();

            //#NT#2011/11/29#Philex Lin - begin
            // FW ration while unpluging HDMI
            #if (_LCDTYPE_ == _LCDTYPE_LIL9341_)
            FWRotate_SetStatus(DISPROT_ROTATE90CCW);
            // Init OSD buffer again
            UI_BeginDisplay(DD_OSD, TRUE);
            #endif
            //#NT#2011/11/29#Philex Lin - end

            // Enable all windows
            //ide_set_allwindow_en(uiWindowEn);
            DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = TRUE;
            DispCtrl.SEL.SET_ALL_LYR_EN.DispLyr = uiWindowEn;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(FALSE);

            // Delay 6 IDE frames to avoid seeing garbage display
            for (i=6; i>0; i--)
            {
                pDispObj->waitFrmEnd();
            }

            //#NT#2009/08/18#Klins Chen#[0006129] -begin
            //#Add to select audio channel to HDMI interface
            AudioSetting.Output             = AUDIO_OUTPUT_SPK;
            KeyScan_HDMIAudioSetting(AudioSetting,FALSE);
            //#NT#2009/08/18#Klins Chen#[0006129] -end

            // Turn on LCD backlight and power
            //GPIOMap_TurnOnLCDBacklight();
            //gpio_setPin(GPIO_LCD_PWM);
            GPIOMap_TurnOnLCDPower();

            //pinmux_select_cdacclk(PINMUX_GPIO);
            //pinmux_select_audio(PINMUX_GPIO);

        }
        //#NT#2010/11/03#KS Hung -begin
        /*
        dma_setChannelPriority(DMA_CH_IDE_Y_INPUT,DMA_PRIORITY_HIGH);
        dma_setChannelPriority(DMA_CH_IDE_CB_INPUT,DMA_PRIORITY_HIGH);
        dma_setChannelPriority(DMA_CH_IDE_CR_INPUT,DMA_PRIORITY_HIGH);
        dma_setChannelPriority(DMA_CH_IDE_OSD_INPUT,DMA_PRIORITY_HIGH);
        */
        debug_err(("[KeyScan_DetHDMI]Primary_ChangeMode(%d)\n\r",CurrMode));
        //Primary_ChangeMode(CurrMode);
        Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
        //#NT#2010/11/03#KS Hung -end
        Primary_Wait4ChangeModeDone();
        debug_err(("[KeyScan_DetHDMI]Primary_Wait4ChangeModeDone\n\r"));
    }

    bLastHDMIDet     = bCurHDMIDet;
    bLastHDMIStatus  = bCurHDMIStatus;
    //#NT#2011/08/03#Klins Chen -end
}


/**
  Return TV plug in or unplugged

  Return TV plug in or unplugged.
  TRUE  -> TV plug in.
  FALSE -> TV is unplugged.

  @param void
  @return BOOL: TRUE -> TV plug in, FALSE -> TV is unplugged
*/
BOOL KeyScan_IsHDMIPlugIn(void)
{
    return bHDMIStatus;
}
//#NT#2009/12/2#philex - begin
BOOL KeyScan_IsHDMIStartup(void)
{
    return bStartupHDMIStatus;
}

void KeyScan_SetHDMIStartup(BOOL bzHDMIStatus)
{
    bStartupHDMIStatus = bzHDMIStatus;
}

//#NT#2009/12/2#philex - end
//@}
