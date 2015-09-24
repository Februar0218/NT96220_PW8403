/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlPhoto.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for photo mode
                Control LCD/LCD backlight on/off, CCD power on/off.
                Lens...

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "DeviceCtrl.h"
#include "GPIOMapping.h"
#include "ImageAlgInfor.h"
#include "ide.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"
#include "SystemClock.h"
#include "Alg_IPLMode.h"
#include "sie_lib.h"
#include "GlobalVar.h"
#include "UIFlow.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "UILensObj.h"

//#NT#2011/9/21#KS Hung -begin
//#NT#Removed warning message
extern void UIFlowWndPhoto_InitCfgSetting(void);
//#NT#2011/9/21#KS Hung -end

/**
  Control the devices for Photo mode

  Control the devices before entering Photo mode.

  @param void
  @return void
*/
void DevCtrl_ModePhoto(void)
{
    IPL_OBJ IplObj;

    // Turn on CCD power
    if (GPIOMap_IsSensorPowerOn() == FALSE)
    {
        GPIOMap_TurnOnSensorPower();
    }

     //UI_SetParameter(_ImageFlip, _Flip_Off);//eric 20150906 sensor FLIP
	//UI_SetParameter(_ImageFlip, _SEN_Flip_H_V); //eric 20150906 sensor FLIP
    if (SysGetFlag(FL_SENSOR_ROTATE) == SEN_ROTATE_OFF)
    	{
    	        //UI_SetParameter(_ImageFlip, _SEN_Flip_H_V);
	        UI_SetParameter(_ImageFlip, _Flip_Off);
		}
		else
		{
	        UI_SetParameter(_ImageFlip, _SEN_Flip_H_V);
	        //UI_SetParameter(_ImageFlip, _Flip_Off);
		}
		//UI_SetParameter(_ImageFlip, _Flip_Off);
		
    //UI_SetParameter(_ImageFlip, _SEN_Flip_H_V);
    /*if (KeyScan_IsSensorRotate())
    {
        UI_SetParameter(_ImageFlip, _Flip_Off);
    }else
    {
        UI_SetParameter(_ImageFlip,_SEN_Flip_H_V );
    }*/

    if (IPL_GetMode() != IPL_MODE_PREVIEW)
    {
        if (IPL_GetMode() == IPL_MODE_OFF)
        {
            ide_disable_video(IDE_VIDEOID_1);
            ide_disable_video(IDE_VIDEOID_2);
        }

        UIFlowWndPhoto_InitCfgSetting(); // temporary using the function of UI window, need to move to UI obj?
        //#NT#2011/10/19#KS Hung -begin
        //#NT#For LCD panel with 16:9 ratio (but 4:3 pixel resolution)
        #if (DISPLAY_169HD == DISPLAY_169HD_YES)
        if (KeyScan_IsTVPlugIn() == TRUE)
        {
            UI_SetParameter(_DispRatio, _Ratio_4_3);
        }
        else if (KeyScan_IsHDMIPlugIn() == TRUE)
        {
            UI_SetParameter(_DispRatio, _Ratio_16_9);
        }
        else
        {
            UI_SetParameter(_DispRatio, _Ratio_16_9);
        }
        #endif
        //#NT#2011/10/19#KS Hung -end

        #if (_LENSLIB_ != _LENSLIB_FF_)
        if((Primary_GetPreviousMode() == PRIMARY_MODE_UNKNOWN) ||
           (Primary_GetPreviousMode() == PRIMARY_MODE_DUMMY) ||
           (Primary_GetPreviousMode() == PRIMARY_MODE_ENGINEER) ||
           (Primary_GetPreviousMode() == PRIMARY_MODE_PLAYBACK && Lens_Module_GetState() == LENS_INITOK))
        {
            //#NT#2011/06/22#Chris Chung -begin
            // Retract zoom first to avoid zoom init timeout
            if(Primary_GetCurrentMode() == PRIMARY_MODE_ENGINEER)
            {
                Lens_Retract(0);
            }
            //#NT#2011/06/22#Chris Chung -end

            // Start zoom initialization
            Lens_Init(LENS_INIT_ZOOM_PART1);

            // Wait for zoom initialization done
            Lens_Init(LENS_INIT_ZOOM_PART2);

            // TO DO...
            UILens_FocusToFar(1);
            //Photo_Shutter_Move(OPEN);
            //Photo_Aperture_Move(0);

            // Go to current zoom section
            //Lens_Zoom_Goto(Lens_Zoom_GetSection());

            // Turn off lens motor power
            Lens_OnOff(MOTOR_POWER_OFF, 0);

            // Register lens zoom change callback function
            //LensCtrl_RegCB((FPLENSCALLBACK)FlowPhoto_ImageCB);
        }
        #endif

        IplObj.uiImeOutWidth1   = GetDisplayH();
        IplObj.uiImeOutHeight1  = GetDisplayV();
        IplObj.uiImeOutYoft1    = GetDisplayH();
        IplObj.uiDzoomStep      = Get_DzoomIndex();
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_PREVIEW;
        IplObj.uiImgFmt         = IMEOUT2_FMT422_2;
        if(IPL_SetCmd(&IplObj) == E_BOVR)
        {
            IplObj.uiImgFmt = IMEOUT2_FMT420_2;
        IPL_SetCmd(&IplObj);
        }

        //PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW, TRUE);

        //#NT#2011/11/28#Philex Lin - begin
        // photo display set mode function
//        FlowPhoto_SetModeCtrl();
        //#NT#2011/11/28#Philex Lin - end

        // Delay 2 frames to avoid seeing error picture
        //sie_waitVD(2);

        // Enable IDE video1 output
        //ide_enable_video(IDE_VIDEOID_1);

        // Wait for zoom initialization done
        //Photo_OpticalZoom_InitEnd();

        //Photo_Shutter_Move(OPEN);
        //Photo_Aperture_Move(0);
    }

    // Turn on LCD backlight if TV is un plugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE &&
        KeyScan_IsTVPlugIn() == FALSE &&
        KeyScan_IsHDMIPlugIn() == FALSE)
    {
        GPIOMap_TurnOnLCDBacklight();
    }

    SysClk_SetClk(SPEED_ID_PHOTO);
}

//@}
