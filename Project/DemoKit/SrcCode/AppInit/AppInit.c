/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInit.c
    @ingroup    mIPRJAPCfg

    @brief      Init appication required information
                Get the memory, init application parameters...

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "PrimaryTsk.h"
#include "AppInit.h"
#include "DeviceCtrl.h"
#include "UIFlow.h"
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "SystemClock.h"
#include "WavStudioTsk.h"
#include "KeyScanTsk.h"
#include "Alg_IPLMode.h"
#include "MediaPlayAPI.h"
#include "MovieInterface.h"
#include "SieCapTsk.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#if (_CALIBRATION_MODE_ == ENABLE)
#include "CalibrationTsk.h"
#endif
#include "DeviceCtrl.h"

/**
  Initialize application/device.

  Primary task will call this API when try to change different mode.
  Here we do devic control and initialize application and UI.
  The flow should be the following:
  1. Initialize device
  2. Initialize application and open application
  3. Initialize UI and open UI

  @param void
  @return void
*/
void AppInit_Open(UINT32 uiMode)
{
    static UINT32 uiLastMode = PRIMARY_MODE_PHOTO;

    debug_msg("AppInit_Open(%d) +++\r\n", uiMode);

    if (uiLastMode == PRIMARY_MODE_USBMSDC ||
       uiLastMode == PRIMARY_MODE_USBPCC)
    {
        DevCtrl_ModeUSBRemove();
    }
    uiLastMode = uiMode;

    switch (uiMode)
    {
    case PRIMARY_MODE_PHOTO:
        //--------------------------------------------------------------------------
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Note !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // The flow should be 1. DevCtrl 2. AppInit 3. UIFlow
        // But the DevCtrl will set image pipeline to new mode, and the sensor
        // library require some golbal variables to set the correct memory address.
        // The variables are assigned in Photo_Open() API now. we have to run
        // AppInit before DevCtrl now!
        // The sensor library have to be modified. All modes that need sensor will
        // have the same situation.
        // Ex: Photo, ASF/AVI recording, PCC mode...
        //--------------------------------------------------------------------------

        // Init Photo and Capture task and open
        AppInit_ModePhoto();
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModePhoto();
        // Open UI Flow Photo
        FlowPhoto_Open();
        break;

    case PRIMARY_MODE_SETUP:
        DevCtrl_ModeSetup();
        AppInit_ModeSetup();
        MenuSetup_Open();
        break;

    //##2010/04/28#Creator -begin
    case PRIMARY_MODE_INIT_SETUP:
        UI_ClearOSD(_OSD_INDEX_BLACK);
        ide_disable_video(IDE_VIDEOID_1); // avoid seeing garbage
        // Control setup mode
        DevCtrl_ModeInitSetup();
        AppInit_ModeInitSetup();
        FlowInitSetup_Open();
        break;

    //#PIC#2010/04/28#Creator -end

    case PRIMARY_MODE_ENGINEER:
#if (_CALIBRATION_MODE_ == ENABLE)
        //#NT#2011/06/22#KS Hung -begin
        //Cal_Open(NULL);
        Cal_Open();
        GPIOMap_TurnOnLCDBacklight();
        KeyScan_EnableAutoPoweroff(FALSE);
        MenuCalibration_Open();
        //#NT#2011/06/22#KS Hung -end
#endif
        break;

    case PRIMARY_MODE_PLAYBACK:
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModePlayback();
        // Init playback task and open
        AppInit_ModePlayback();
        // Open UI Flow Playback
        FlowPB_Open();
        break;

    case PRIMARY_MODE_USBMSDC:
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeUSBMSDC();
        // Init USB MSDC task and open
        AppInit_ModeUSBMSDC();
        MenuMSDC_Open();
        break;

    case PRIMARY_MODE_USBSICD:
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeUSBSICD();
        // Init USB SICD task and open
        AppInit_ModeUSBSICD();
        //#NT#2008/10/29#Marx Chiu -begin
        // Open Menu Flow Print
        MenuPrint_Open();
        //#NT#2008/10/29#Marx Chiu -end
        break;

    case PRIMARY_MODE_USBPCC:
        // Init USB PCC task and open
        AppInit_ModeUSBPCC();
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeUSBPCC();
        MenuPCC_Open();
        break;

    case PRIMARY_MODE_MUSIC:
        #if (_MP3_MODE_ == ENABLE)
        DevCtrl_ModeMp3();
        AppInit_ModeMp3();
        FlowMP3_Open();
        #endif
        break;

    case PRIMARY_MODE_MOVIE:
        //ide_disable_video(IDE_VIDEOID_1);   //#NewUx porting: Still need this? Marked first
        AppInit_ModeAVI();
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeMovie();
        // Open UI Flow Movie
        FlowMovie_Open();
        break;

    case PRIMARY_MODE_CUSTOM1:
        /*
        DevCtrl_ModeCustom1();
        AppInit_ModeCustom1();
        */
        MenuUSB_Open();
        break;

    case PRIMARY_MODE_DUMMY:
        /*
        DevCtrl_ModeCustom1();
        AppInit_ModeCustom1();
        */
        DevCtrl_ModeDummy();
        break;

#if (_USBWRITE_MODE_ == ENABLE)
    case PRIMARY_MODE_CUSTOM2:
        DevCtrl_ModeCustom2();
        AppInit_ModeCustom2();
        break;
#endif

    default:
        debug_wrn(("Unknown operation mode\r\n"));
        break;
    }

#if (PRJ == APC3)
    KeyScan_EnableLEDToggle(KEYSCAN_LED_GREEN, FALSE);
#endif

    debug_err(("AppInit_Open(%d) ---\r\n", uiMode));
}

/**
  Close application

  Primary task will call this API when try to change different mode.
  Add all the tasks that you want to close when changing mode.

  @param void
  @return void
*/
void AppInit_Close(void)
{
    //#NT#2011/07/19#KS Hung -begin
    //#NT#Only for MSDC vendor command
    extern BOOL bRunningMsdcTool;
    //#NT#2011/07/19#KS Hung -end

    debug_err(("AppInit_Close +++\r\n"));

#if (PRJ == APC3)
    GPIOMap_TurnOffLED(GPIOMAP_LED_GREEN);
    KeyScan_EnableLEDToggle(KEYSCAN_LED_GREEN, TRUE);
#endif

    if (FWRotate_GetStatus()==TRUE)
    {
        FWRotate_Close();
    }

    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CHGMOD_PRESS , 0);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS , 0);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, 0);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, 0);
    disprot_unlock();

    //#NT#2009/03/31#Lily Kao -begin
    //#Flush Cmd to avoid key-in-queue comes into UI while switching mode
    Ux_FlushEvent();
    //#NT#2009/03/31#Lily Kao -end

    //#NT#2011/08/02#KS Hung -begin
    // Close UI Flow
    //#NT#2009/04/3#Janice Huang -begin
    //move all close UI flow befor App close
    //avoid after app close ,UI flow access app
    MenuCalibration_Close();
    FlowPB_Close();
    FlowPhoto_Close();
    FlowMovie_Close();
    MenuUSB_Close();
    #if (UI_STYLE != UI_STYLE_DRIVE)
    MenuPrint_Close();
    FlowInitSetup_Close();
    #endif
    MenuUSB_Close();
    MenuMSDC_Close();
    MenuPCC_Close();
    MenuSetup_Close();
    //#NT#2011/08/02#KS Hung -end

    // Close Application
    // Add more applications here
    MovRec_Close();//AVIRec_Close(AVIREC_WAIT_END);
    MediaPlay_Close();

    //#NT#2011/11/04#KS Hung -begin
    #if (_MOVIE_PIM_MODE_ == ENABLE)
    RawEnc_Close();
    #endif
    //#NT#2011/11/04#KS Hung -end

    Photo_Close();
    PhotoDisplay_Close();
    ImgCapture_Close();
    // stop image pipeline
    {
    IPL_OBJ IplObj;

        // Set to off mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }
    PB_Close(PB_WAIT_INFINITE);
    WavStudio_Close();

    #if (_MP3_MODE_ == ENABLE)
    /* Close mp3 */
    FlowMP3_Close();
    MP3PlayClose();
    if(GetCardStatus() == CARD_REMOVED)
    {
        MP3PlayFileRead_Close_NoCloseFile();
    }
    else
    {
        MP3PlayFileRead_Close();
    }
    #endif

//#NT#2011/07/19#KS Hung -begin
//#NT#Only add commect for MSDC vendor command
#if ((_USBMSDC_MODE_ == ENABLE) && (_MSDCVENDOR_ == _MSDCVENDOR_NVT_))
    //#NT#2009/07/30#Niven Cho -begin
    //#NT#Modified., for MSDCExt with NVT
    //If Running in MSDC_NVT Mode, Don't Disconnect the USB
if(!bRunningMsdcTool)
    USBMSDC_Close();

#else
    USBMSDC_Close();
//#NT#2009/07/30#Niven Cho -end
#endif
//#NT#2011/07/19#KS Hung -end


    //#NT#2009/03/24#Marx Chiu -begin
    //MarxChiu_20090324 usbsicd_close();
    Usicd_Close();
    //#NT#2009/03/24#Marx Chiu -end
    UPccClose();

    //#NT#2009/01/23#Ben Wang -begin
    //#Add for continue key
    //KeyScan_SetCountinueKey_FQ(FLGKEY_ALL,0);
    //#NT#2009/01/23#Ben Wang -end

    //#NT#2009/04/20#Lily Kao -begin
    //#Flush Cmd to avoid key events come-in during above xxx_close functions
    Ux_FlushEvent();
    //#NT#2009/04/20#Lily Kao -end
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CHGMOD_PRESS , FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS , FLGKEY_KEY_MASK_DEFAULT);

    debug_err(("AppInit_Close ---\r\n"));
}

//@}
