/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlMovie.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for movie mode
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
#include "IDE.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "PrimaryTsk.h"
#include "SystemClock.h"
#include "Alg_IPLMode.h"
#include "sie_lib.h"
#include "UIFlow.h"
#include "globalvar.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "UILensObj.h"
#include "disp_rotate.h"

//#NT#2011/9/21#KS Hung -begin
//#NT#Removed warning message
extern void UIFlowWndMovie_Initparam(void);
//#NT#2011/9/21#KS Hung -end

/**
  Control the devices for Movie mode

  Control the devices before entering Movie mode.

  @param void
  @return void
*/
void DevCtrl_ModeMovie(void)
{
    IPL_OBJ IplObj;

    // Turn on CCD power
    if (GPIOMap_IsSensorPowerOn() == FALSE)
    {
        GPIOMap_TurnOnSensorPower();
    }
   //UI_SetParameter(_ImageFlip, _SEN_Flip_H_V);
   //UI_SetParameter(_ImageFlip, _Flip_Off);
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
		
       //UI_SetParameter(_ImageFlip,  _Flip_Off); //eric 20150906 sensor FLIP
      //UI_SetParameter(_ImageFlip, _SEN_Flip_H_V); //eric 20150906 sensor FLIP

    if (IPL_GetMode() != IPL_MODE_VIDEO)
    {
        if (IPL_GetMode() == IPL_MODE_OFF)
        {
            ide_disable_video(IDE_VIDEOID_1);
            ide_disable_video(IDE_VIDEOID_2);
        }

        UIFlowWndMovie_Initparam(); // temporary using the function of UI window, need to move to UI obj?
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
        if((Primary_GetPreviousMode() == PRIMARY_MODE_UNKNOWN)  ||
           (Primary_GetPreviousMode() == PRIMARY_MODE_DUMMY)    ||
           (Primary_GetPreviousMode() == PRIMARY_MODE_ENGINEER) ||
           (Primary_GetPreviousMode() == PRIMARY_MODE_PLAYBACK && Lens_Module_GetState() == LENS_INITOK))
        {
            if(Primary_GetCurrentMode() == PRIMARY_MODE_ENGINEER)
            {
                Lens_Retract(0);
            }

            // Start zoom initialization
            Lens_Init(LENS_INIT_ZOOM_PART1);

            // Wait for zoom initialization done
            Lens_Init(LENS_INIT_ZOOM_PART2);

            // TO DO...
            UILens_FocusToFar(1);
            //Lens_Zoom_DoBL();

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
        IplObj.Mode = IPL_MODE_VIDEO;
        IplObj.uiImgFmt         = IMEOUT2_FMT422_2;
        if(IPL_SetCmd(&IplObj) == E_BOVR)
        {
            IplObj.uiImgFmt = IMEOUT2_FMT420_2;
            IPL_SetCmd(&IplObj);
        }
        //#NT#2009/12/11#Connie Yu -begin
        //#Apply new DIS control in recording flow
        //AlgInfo = UI_GetParameter();
        //if(AlgInfo->DISMode == _DISMode_On)
        //    PhotoDisplay_SetMode(DISPLAY_MODE_DISVIEW, TRUE);
        //else
        //    PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);
        //#NT#2009/12/11#Connie Yu -end

        //#NT#2011/11/28#Philex Lin - begin
        // photo display set mode function
//        FlowPhoto_SetModeCtrl();
        //#NT#2011/11/28#Philex Lin - end

        // Start zoom initialization
        //Photo_OpticalZoom_InitStart();

        // Delay 2 frames to avoid seeing error picture
        //sie_waitVD(2);

        // Enable IDE video1 output
        //ide_enable_video(IDE_VIDEOID_1);

        // Wait for zoom initialization done
        //Photo_OpticalZoom_InitEnd();
    }

    // Turn on LCD backlight if TV is un plugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE &&
        KeyScan_IsTVPlugIn() == FALSE &&
        KeyScan_IsHDMIPlugIn() == FALSE)
    {
        GPIOMap_TurnOnLCDBacklight();
    }
}

//@}
