/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanTV.c
    @ingroup    mIPRJAPKey

    @brief      Detect TV is plugging in or unplugged
                Detect TV is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "PlaybackTsk.h"
#include "AVIPlayAPI.h"
#include "UIFlow.h"
#include "IDE.h"
#include "Top.h"
#include "GlobalVar.h"
#include "Utility.h"
#include "Audio.h"
#include "Alg_IPLMode.h"
#include "PhotoDisplayTsk.h"
//#NT#2011/08/03#Klins Chen -begin
#include "display.h"
//#NT#2011/08/03#Klins Chen -end
#include "disp_rotate.h"
#include "SystemInit.h"
#include "KeyScanInt.h"


static UINT32   uiCurrentTVMode;
static BOOL     bTVStatus       = FALSE;
static BOOL     bLastTVDet      = FALSE;
static BOOL     bLastTVStatus   = FALSE;

/**
  Scaling OSD size between pannel and TV

  Scaling OSD size between pannel and TV.
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_ScaleDisplay(void)
{
    VOSD_WINDOW_ATTR    FB_win;
    UINT32              uiSourceW, uiSourceH, uiSourceOff;
    IDE_COLOR_FORMAT    ColorFormat;
    //#NT#2010/09/21#KS Hung -begin
    //#NT#16:9 display for NTSC/PAL
    ImageAlgInfo *AlgInfo;
    UINT32 DisplayInSizeH;
    //#NT#2010/09/21#KS Hung -end

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
    ide_set_osd_win_attr(1, &FB_win);

    // Video1
    ide_get_video1_buf_dim(&uiSourceW, &uiSourceH, &uiSourceOff);
    ide_get_v1_win_format(&ColorFormat);

    //#NT#2010/09/21#KS Hung -begin
    //#NT#16:9 display for NTSC/PAL
    FB_win.Source_W     = uiSourceW;
    FB_win.Source_H     = uiSourceH;
    FB_win.Des_W        = uiSourceW;
    FB_win.Des_H        = uiSourceH;
    FB_win.Win_Format   = ColorFormat;

    AlgInfo = UI_GetParameter();
    DisplayInSizeH = PhotoDisplay_GetDisplaySizeInfor(_DisplayOutHsize);
    if (AlgInfo->ImageRatio != AlgInfo->DispRatio)
    {
        FB_win.Win_Y    = (g_LCDSize.uiWinHeight - uiSourceH)>>1;
    }
    else
    {
        FB_win.Win_Y    = 0;
    }
    FB_win.Win_X    = 0;
    FB_win.High_Addr    = 0;

    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
}

/**
  Detect TV is plugging in or unplugged

  Detect TV is plugging in or unplugged.

  @param void
  @return void
*/
void KeyScan_DetTV(void)
{
#if (_TV_FUNCTION_ == ENABLE)
    BOOL            bCurTVDet, bCurTVStatus;
    UINT32          i, uiWindowEn;
    PrimaryModeID   CurrMode;
    //#NT#2011/08/03#Klins Chen -begin
    //TV_DEVICE_OBJ   TVObj;
    PDISP_OBJ       pDispObj;
    DISPDEV_PARAM   DispDev;
    DISPCTRL_PARAM  DispCtrl;
    DISPLAYER_PARAM DispLyr;
    AUDIO_DEVICE_OBJ    AudioDevObj={0};

    if ((KeyScan_IsUSBPlug() == TRUE) || (KeyScan_IsHDMIPlugIn() == TRUE))
    {
        return;
    }

    pDispObj = disp_getDisplayObject(DISP_1);

   bCurTVDet = GPIOMap_DetTV();
   //debug_msg("KeyScan_DetTV*********************************ERIC_DEBUG\n\r");//ERIC_DEBUG
   //debug_msg("bCurTVDet = %d*********************************ERIC_DEBUG\n\r", bCurTVDet);//ERIC_DEBUG
    bCurTVStatus = (BOOL)(bCurTVDet & bLastTVDet);

    if (bCurTVStatus != bLastTVStatus)
    {
        debug_err(("TV is plugged GPIOMap_DetTV = %d\r\n",bCurTVDet));

        // Get current mode
        CurrMode = Primary_GetCurrentMode();

        // Enter dummy mode firstly
        Primary_ChangeMode(PRIMARY_MODE_DUMMY);
        Primary_Wait4ChangeModeDone();

        // TV is plugging in
        if (bCurTVStatus == TRUE)
        {

            bTVStatus = TRUE;

            //#NT#2011/09/07#KS Hung -begin
            //Change Audio Out To Line Out
            /*aud_getDeviceObject(&AudioDevObj);
            aud_open(&AudioDevObj);
            aud_setOutput(AUDIO_OUTPUT_SPK);
            aud_setFeature(AUDIO_FEATURE_SPK_RIGHT_EN, FALSE);
            aud_close();*/ //henry 20141225
            //#NT#2011/09/07#KS Hung -end

            // Turn off LCD backlight
            GPIOMap_TurnOffLCDBacklight();

            //ide_disable_video(IDE_VIDEOID_1);
            //ide_disable_video(IDE_VIDEOID_2);
            DispLyr.SEL.SET_ENABLE.bEn = FALSE;
            pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_ENABLE, &DispLyr);
            pDispObj->dispLyrCtrl(DISPLAYER_VDO2, DISPLAYER_OP_SET_ENABLE, &DispLyr);
            pDispObj->load(TRUE);

            // Close LCD panel
            pDispObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);

            GPIOMap_TurnOffLCDPower();

            // Disable all windows
            pDispObj->dispCtrl(DISPCTRL_GET_ALL_LYR_EN, &DispCtrl);
            uiWindowEn = DispCtrl.SEL.GET_ALL_LYR_EN.DispLyr;

            DispCtrl.SEL.SET_ALL_LYR_EN.bEn = FALSE;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(TRUE);

            DispDev.SEL.SET_TVADJUST.TvAdjust = DISPDEV_TVADJUST_3_3_K;
            pDispObj->devCtrl(DISPDEV_SET_TVADJUST,&DispDev);

            // Init TV object
            if (SysGetFlag(FL_TV_MODE) == TV_MODE_NTSC)
            {
                // NTSC
                DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_TVNTSC;
                pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);
                uiCurrentTVMode = TV_MODE_NTSC;
            }
            else
            {
                // PAL
                DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_TVPAL;
                pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);
                uiCurrentTVMode = TV_MODE_PAL;
            }

            pDispObj->devCtrl(DISPDEV_GET_DISPSIZE,&DispDev);
            g_LCDSize.uiWidth     = DispDev.SEL.GET_DISPSIZE.uiBufWidth;
            g_LCDSize.uiHeight    = DispDev.SEL.GET_DISPSIZE.uiBufHeight;
            g_LCDSize.uiWinWidth  = DispDev.SEL.GET_DISPSIZE.uiWinWidth;
            g_LCDSize.uiWinHeight = DispDev.SEL.GET_DISPSIZE.uiWinHeight;

            //#NT#2011/11/29#Philex Lin - begin
            // non FW ration while pluging TV
            #if (_LCDTYPE_ == _LCDTYPE_LIL9341_)
            FWRotate_SetStatus(DISPROT_ROTATE_NONE);
            // Init OSD buffer again
            UI_BeginDisplay(DD_OSD, TRUE);
            #endif
            //#NT#2011/11/29#Philex Lin - end

            // Scaling OSD to new size
            KeyScan_ScaleDisplay();

            // Enable IDE
            pDispObj->waitFrmEnd();
            DispCtrl.SEL.SET_ENABLE.bEn = TRUE;
            pDispObj->dispCtrl(DISPCTRL_SET_ENABLE,&DispCtrl);

            // Delay to wait for TV DAC stable
            for (i=6; i>0; i--)
            {
                pDispObj->waitFrmEnd();
            }

            // Enable all windows
            DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = TRUE;
            DispCtrl.SEL.SET_ALL_LYR_EN.DispLyr = uiWindowEn;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(FALSE);
        }
        // TV is unplugged
        else
        {
            debug_err(("TV is unplugged, GPIOMap_DetTV = %d\r\n", bCurTVDet));
            bTVStatus = FALSE;

            //#NT#2011/09/07#KS Hung -begin
            //Change Audio Out To Speak
            aud_getDeviceObject(&AudioDevObj);
            aud_open(&AudioDevObj);
            aud_setFeature(AUDIO_FEATURE_SPK_RIGHT_EN, TRUE);
            aud_close();
            //#NT#2011/09/07#KS Hung -end

            // Disable all windows
            pDispObj->dispCtrl(DISPCTRL_GET_ALL_LYR_EN, &DispCtrl);
            uiWindowEn = DispCtrl.SEL.GET_ALL_LYR_EN.DispLyr;

            DispCtrl.SEL.SET_ALL_LYR_EN.bEn = FALSE;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(TRUE);

            // Close TV
            pDispObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);

            // Open LCD panel
            GPIOMap_TurnOnLCDPower();
			
            DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_PANEL;
            pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);

            pDispObj->devCtrl(DISPDEV_GET_DISPSIZE,&DispDev);
            g_LCDSize.uiWidth     = DispDev.SEL.GET_DISPSIZE.uiBufWidth;
            g_LCDSize.uiHeight    = DispDev.SEL.GET_DISPSIZE.uiBufHeight;
            g_LCDSize.uiWinWidth  = DispDev.SEL.GET_DISPSIZE.uiWinWidth;
            g_LCDSize.uiWinHeight = DispDev.SEL.GET_DISPSIZE.uiWinHeight;
	     DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_180;//ERIC EDIT
            pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);//ERIC EDIT	
            // Scaling OSD to new size
            KeyScan_ScaleDisplay();

            //#NT#2011/11/29#Philex Lin - begin
            //  FW ration while unpluging TV
            #if (_LCDTYPE_ == _LCDTYPE_LIL9341_)
            FWRotate_SetStatus(DISPROT_ROTATE90CCW);
            // Init OSD buffer again
            UI_BeginDisplay(DD_OSD, TRUE);
            #endif
            //#NT#2011/11/29#Philex Lin - end
			
            // Enable all windows
            DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = TRUE;
            DispCtrl.SEL.SET_ALL_LYR_EN.DispLyr = uiWindowEn;
            pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            pDispObj->load(FALSE);

            // Delay 6 IDE frames to avoid seeing garbage display
            for (i=6; i>0; i--)
            {
                pDispObj->waitFrmEnd();
            }

            // Turn on LCD backlight and power
            //GPIOMap_TurnOnLCDBacklight();
        }

        if (IPL_GetMode() != IPL_MODE_OFF)
        {
            IPL_OBJ IplObj;
            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_OFF;
            IPL_SetCmd(&IplObj);
        }

        Primary_ChangeMode(CurrMode);               // back to current mode
        Primary_Wait4ChangeModeDone();
    }

    bLastTVDet     = bCurTVDet;
    bLastTVStatus  = bCurTVStatus;
    //#NT#2011/08/03#Klins Chen -begin
#endif  // _TV_FUNCTION_
}

/**
  Return TV plug in or unplugged

  Return TV plug in or unplugged.
  TRUE  -> TV plug in.
  FALSE -> TV is unplugged.

  @param void
  @return BOOL: TRUE -> TV plug in, FALSE -> TV is unplugged
*/
BOOL KeyScan_IsTVPlugIn(void)
{
    return bTVStatus;
}

/**
  Return current TV mode

  Return current TV mode.

  @param void
  @return UINT32: TV_MODE_NTSC or TV_MODE_PAL
*/
UINT32 KeyScan_GetCurrentTVMode(void)
{
    return uiCurrentTVMode;
}

/**
  Change IME & IDE setting for TV mode

  Change IME & IDE setting for TV mode.
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_ChangeIMEIDESize(void)
{
    IPL_OBJ             IplObj;
    IME_OUTFMT uiImgFmt = IMEOUT2_FMT422_2;
    PHOTODISPLAY_APPOBJ PhotoDisplayObj = {0};
    UINT32  uiCurrMode = Primary_GetCurrentMode();
    UINT32  uiCurrDZoomIdx = Get_DzoomIndex();

    switch(uiCurrMode)
    {
    case PRIMARY_MODE_PHOTO:
        {
            PhotoDisplay_Close();
            PhotoDisplayObj.uiDisplayFBWidth     = g_LCDSize.uiWidth;
            PhotoDisplayObj.uiDisplayFBHeight    = g_LCDSize.uiHeight;
            PhotoDisplayObj.uiDisplayOutWidth    = g_LCDSize.uiWinWidth;
            PhotoDisplayObj.uiDisplayOutHeight   = g_LCDSize.uiWinHeight;
            PhotoDisplay_Open(&PhotoDisplayObj);
            //#NT#2011/11/01#Ethan Lau -begin
            //#NT#move to uiflowcbphoto.c
            //PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW, TRUE);
            //#NT#2011/11/01#Ethan Lau -end
            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_OFF;
            IPL_SetCmd(&IplObj);

            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_PREVIEW;
            IplObj.uiImeOutWidth1  = g_LCDSize.uiWidth;
            IplObj.uiImeOutHeight1 = g_LCDSize.uiHeight;
            IplObj.uiImeOutYoft1 = g_LCDSize.uiWidth;
            IplObj.uiDzoomStep = uiCurrDZoomIdx;
            //#NT#2011/06/13#Ethan Lau -begin
            //#NT#add IPL img fmt
            IplObj.uiImgFmt = uiImgFmt;
            //#NT#2011/06/13#Ethan Lau -end
            if(IPL_SetCmd(&IplObj) == E_BOVR)
            {
                uiImgFmt = IMEOUT2_FMT420_2;
                IplObj.uiImgFmt = uiImgFmt;
            IPL_SetCmd(&IplObj);
            }
            break;
        }
    case PRIMARY_MODE_MOVIE:
        {
            PhotoDisplay_Close();
            PhotoDisplayObj.uiDisplayFBWidth     = g_LCDSize.uiWidth;
            PhotoDisplayObj.uiDisplayFBHeight    = g_LCDSize.uiHeight;
            PhotoDisplayObj.uiDisplayOutWidth    = g_LCDSize.uiWinWidth;
            PhotoDisplayObj.uiDisplayOutHeight   = g_LCDSize.uiWinHeight;
            PhotoDisplay_Open(&PhotoDisplayObj);
            //#NT#2011/11/01#Ethan Lau -begin
            //#NT#move to uiflowcbphoto.c
            //PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);
            //#NT#2011/11/01#Ethan Lau -end
            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_OFF;
            IPL_SetCmd(&IplObj);

            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_VIDEO;
            IplObj.uiImeOutWidth1  = g_LCDSize.uiWidth;
            IplObj.uiImeOutHeight1 = g_LCDSize.uiHeight;
            IplObj.uiImeOutYoft1 = g_LCDSize.uiWidth;
            IplObj.uiDzoomStep = uiCurrDZoomIdx;
            //#NT#2011/06/13#Ethan Lau -begin
            //#NT#add IPL img fmt
            IplObj.uiImgFmt = uiImgFmt;
            //#NT#2011/06/13#Ethan Lau -end
            if(IPL_SetCmd(&IplObj) == E_BOVR)
            {
                uiImgFmt = IMEOUT2_FMT420_2;
                IplObj.uiImgFmt = uiImgFmt;
            IPL_SetCmd(&IplObj);
            }
            break;
        }
    case PRIMARY_MODE_PLAYBACK:
        {
            IMEIDE_ATTR ImeIdeAttr;

            ImeIdeAttr.uiImeOutH = g_LCDSize.uiWidth;
            ImeIdeAttr.uiImeOutV = g_LCDSize.uiHeight;
            ImeIdeAttr.uiIdeOutH = g_LCDSize.uiWinWidth;
            ImeIdeAttr.uiIdeOutV = g_LCDSize.uiWinHeight;
            PB_SetIMEIDESize(&ImeIdeAttr);

            // Set the position and size of first video frame
            PB_Set1stVideoFrame(0, 0, g_LCDSize.uiWinWidth, g_LCDSize.uiWinHeight);
            break;
        }
    }
}

/**
  Change TV mode

  Change TV mode.

  @param void
  @return void
*/
void KeyScan_ChangeTVMode(void)
{
    UINT32          i, uiWindowEn;
    //#NT#2011/08/03#Klins Chen -begin
    //TV_DEVICE_OBJ   TVObj;
    PDISP_OBJ       pDispObj;
    DISPDEV_PARAM   DispDev;
    DISPCTRL_PARAM  DispCtrl;

    pDispObj = disp_getDisplayObject(DISP_1);

    // Disable all windows
    pDispObj->dispCtrl(DISPCTRL_GET_ALL_LYR_EN, &DispCtrl);
    uiWindowEn = DispCtrl.SEL.GET_ALL_LYR_EN.DispLyr;

    DispCtrl.SEL.SET_ALL_LYR_EN.bEn = FALSE;
    pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
    pDispObj->load(TRUE);

    pDispObj = disp_getDisplayObject(DISP_1);
    pDispObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);

    // Init TV object
    if (SysGetFlag(FL_TV_MODE) == TV_MODE_NTSC)
    {
        // NTSC
        DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_TVNTSC;
        pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);
        uiCurrentTVMode = TV_MODE_NTSC;
        debug_err(("Switch and set TV NTSC...\r\n"));
    }
    else
    {
        // PAL
        DispDev.SEL.OPEN_DEVICE.DevID = DISPDEV_ID_TVPAL;
        pDispObj->devCtrl(DISPDEV_OPEN_DEVICE,&DispDev);
        uiCurrentTVMode = TV_MODE_PAL;
        debug_err(("Switch and set TV PAL..\r\n"));
    }

    pDispObj->devCtrl(DISPDEV_GET_DISPSIZE,&DispDev);
    g_LCDSize.uiWidth     = DispDev.SEL.GET_DISPSIZE.uiBufWidth;
    g_LCDSize.uiHeight    = DispDev.SEL.GET_DISPSIZE.uiBufHeight;
    g_LCDSize.uiWinWidth  = DispDev.SEL.GET_DISPSIZE.uiWinWidth;
    g_LCDSize.uiWinHeight = DispDev.SEL.GET_DISPSIZE.uiWinHeight;

    // Scaling OSD to new size
    KeyScan_ScaleDisplay();

    // Scaling IME & IDE
    KeyScan_ChangeIMEIDESize();

    // Enable IDE
    pDispObj->waitFrmEnd();
    DispCtrl.SEL.SET_ENABLE.bEn = TRUE;
    pDispObj->dispCtrl(DISPCTRL_SET_ENABLE,&DispCtrl);

    // Delay to wait for TV DAC stable
    for (i=6; i>0; i--)
    {
        pDispObj->waitFrmEnd();
    }

    // Enable all windows
    DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = TRUE;
    DispCtrl.SEL.SET_ALL_LYR_EN.DispLyr = uiWindowEn;
    pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
    pDispObj->load(FALSE);
}

#define VIDEO_PATH_LEN2 21
static BOOL u_TVInsert  = FALSE;
#if 0
void KeyScan_DetBack(void)
{
    UINT8 i;
    //debug_err(("^RGPIOMap_DetBack() = %d",GPIOMap_DetBack()));
    //debug_err(("^Ru_TVInsert = %d",u_TVInsert));
    
    UINT32 uiBreaklight = FALSE;
    UINT32 uiBreaklight2 = FALSE;
    if (GPIOMap_IsLCDBacklightOn&&(u_TVInsert == FALSE))
    {
        if(GPIOMap_GetTVStatus() == FALSE)
        {
            GPIOMap_TurnOffLCDBacklight();
            TimerDelayMs(500);
            uiBreaklight2 = TRUE;
        }
        
        if ((gMovData.State == MOV_ST_VIEW)||(gMovData.State == (MOV_ST_VIEW|MOV_ST_ZOOM)) ||
            (gMovData.State == MOV_ST_REC)||(gMovData.State == (MOV_ST_REC|MOV_ST_ZOOM)))
        {
        //debug_msg("^R1111111111111\r\n");
            u_TVInsert = TRUE;
            if(uiBreaklight2 = TRUE)
            {
                for(i=0;i<VIDEO_PATH_LEN2;i++)
                {
                    I2C_TxByte (0x00,BIT1216M_CVBS_REG[i][0],BIT1216M_CVBS_REG[i][1]);  
                    g_uiKeyScanAutoLcdoffCnt = 0;       
                                //FlowMovie_IconDrawDaoche();
                }
                TimerDelayMs(500);
                //GPIOMap_TurnOnLCDBacklight();
                TimerDelayMs(200);
                GPIOMap_SetTV(TRUE);
            }
        }
    }
    else if (!GPIOMap_IsLCDBacklightOn()&&(u_TVInsert == TRUE))
    {
        if(GPIOMap_GetTVStatus() == TRUE)
        {
            GPIOMap_TurnOffLCDBacklight();
            TimerDelayMs(500);
            uiBreaklight = TRUE;
        }
        if ((gMovData.State == MOV_ST_VIEW)||(gMovData.State == (MOV_ST_VIEW|MOV_ST_ZOOM)) ||
            (gMovData.State == MOV_ST_REC)||(gMovData.State == (MOV_ST_REC|MOV_ST_ZOOM)))
        {
        //debug_msg("^R222222222222222\r\n");
            u_TVInsert = FALSE;
            if(uiBreaklight = TRUE)
            {
                for(i=0;i<VIDEO_PATH_LEN2;i++)
                {
                    I2C_TxByte (0x00,BIT1216M_ITU656_REG[i][0],BIT1216M_ITU656_REG[i][1]);
                    g_uiKeyScanAutoLcdoffCnt = 0;       
                    //FlowMovie_IconHideDaoche();
                }
                TimerDelayMs(500);
                //GPIOMap_TurnOnLCDBacklight();
                TimerDelayMs(200);
                GPIOMap_SetTV(FALSE);
            }
        }
    }
}
#endif
void KeyScan_DetBack(void)
{
    UINT8 i;
    //debug_err(("^RGPIOMap_DetBack() = %d",GPIOMap_DetBack()));
    //debug_err(("^Ru_TVInsert = %d",u_TVInsert));
    
    //UINT32 uiBreaklight = FALSE;
    //UINT32 uiBreaklight2 = FALSE;
    if ((GPIOMap_IsLCDBacklightOn())&&(GPIOMap_GetTVStatus() == TRUE))
    {
       
            GPIOMap_TurnOffLCDBacklight();
            TimerDelayMs(500);
    }
    else if ((!GPIOMap_IsLCDBacklightOn())&&(GPIOMap_GetTVStatus() == FALSE))
    {
        
            GPIOMap_TurnOffLCDBacklight();
            TimerDelayMs(500);       
    }
}



