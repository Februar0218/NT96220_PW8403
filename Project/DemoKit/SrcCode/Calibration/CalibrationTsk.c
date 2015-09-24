/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.c
    @ingroup    mIPRJAPTest

    @brief      Calibration task
                Calibration task

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include "Kernel.h"
#include "SysCfg.h"
#include "CalibrationTsk.h"
#include "CalibrationInt.h"
//#include "ShowOSDFuncs.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"
#include "AppInit.h"
#include "UIFlow.h"

#if (_CALIBRATION_MODE_ == ENABLE)
#include "UIFramework.h"
#include "UIGraphics.h"
//static DC**     pDCList;

// -------------------------------------------------------------------
// Internal Global variables
// -------------------------------------------------------------------
// Declare all global variables used in Calibration task here.
// Don't reference these variables in your code.
volatile BOOL               g_bCalbirationOpened = FALSE;
         RECT               g_CalStringRect;
         RECT               g_CalString2Rect;
//volatile CAL_APPOBJ         g_CalAppObj = { NULL, NULL };

/**
  Calibration task

  Calibration task

  @param void
  @return void
*/
void CalibrationTsk(void)
{
    //UINT32  uiFlag = 0;

    AppInit_Close();
    GPIOMap_TurnOnLCDBacklight();
    KeyScan_EnableAutoPoweroff(FALSE);

    MenuCalibration_Open();

    while (1)
    {
        //wai_flg((UINT *)&uiFlag, FLG_ID_KEY, KEY_EVENT_CAL, TWF_ORW | TWF_CLR);
        //debug_err(("uiFlag %d\r\n", uiFlag));
    }

    /**
    UINT32 uiSelectNum =CAL_INVALID_DEFAULT , uiFlag =0;
    INT8   StringBuffer[32];
    BOOL   ubLockKey = TRUE; //use queck key

    //avoid press shutter2 too long while init
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);

    Primary_CheckIdle(PRI_TIME_INFINITE);
    // Close all other task
    AppInit_Close();

    // Clear OSD
    //ShowOSD_ClearOSD(IDE_OSDID_1);
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_Clear(pDCList[GxGfx_OSD]);
    UI_EndScreen((UINT32)pDCList);

    // Disable auto power off
    KeyScan_EnableAutoPoweroff(FALSE);
    // Disable sleep mode
    //KeyScan_EnableSleep(FALSE);

    // Run default calibration item
    // Ex:
    // If the SD card exist a file named "0000000A.CAL"
    // Calibration task will only run item that number is "0x0000000A"
    //uiSelectNum = Cal_GetDefaultInRoot("CAL");
    //uiSelectNum = Cal_GetDefaultCal("A:\\CAL.TXT");
    ReadCalData();
    debug_err(("^G item %d\r\n",AAA_Adjust.currentItem));
    if(AAA_Adjust.currentItem > g_CalMaxItemNum )
        AAA_Adjust.currentItem =0 ;
    else
        AAA_Adjust.currentItem ++;
    uiSelectNum = AAA_Adjust.currentItem;
    debug_err(("^G item %d\r\n",uiSelectNum));

    while (1)
    {
        // Show item number and name
        //sprintf(StringBuffer, "%s   %d %s ",GetModelString(),uiSelectNum , g_CalItem[uiSelectNum].pItemName);
        Cal_ShowStringWithColor(StringBuffer, 20,30, _OSD_INDEX_RED);
        //sprintf(StringBuffer, "%s    %s ",GetVersionString(),GetReleaseDate());
        Cal_ShowStringWithColor(StringBuffer, 20,60, _OSD_INDEX_RED);
        if(ubLockKey)
        {
            sprintf(StringBuffer, "CCD     Left ");
            //sprintf(StringBuffer, "shutterReliability  Left ");
            Cal_ShowStringWithColor(StringBuffer, 20,120, _OSD_INDEX_RED);
            sprintf(StringBuffer, "EV       Right  ");
            Cal_ShowStringWithColor(StringBuffer, 20,140, _OSD_INDEX_RED);
            sprintf(StringBuffer, "Burnin     Up ");
            Cal_ShowStringWithColor(StringBuffer, 20,160, _OSD_INDEX_RED);
            sprintf(StringBuffer, "Check Result   Down ");
            Cal_ShowStringWithColor(StringBuffer, 20,180, _OSD_INDEX_RED);

            sprintf(StringBuffer, "shutterReliability    PB ");
            Cal_ShowStringWithColor(StringBuffer, 20,200, _OSD_INDEX_RED);

        }
        wai_flg((UINT *)&uiFlag, FLG_ID_KEY, KEY_EVENT_CAL, TWF_ORW | TWF_CLR);
        debug_err(("uiFlag  %d ",uiFlag));
        if(!ubLockKey)
        {
            // Previous item
            if (uiFlag & FLGKEY_UP)
            {
                if(uiSelectNum == 0)
                {
                    uiSelectNum = g_CalMaxItemNum;
                }
                else
                {
                    uiSelectNum--;
                }
            }

            // Next item
            if (uiFlag & FLGKEY_DOWN)
            {
                if(uiSelectNum == g_CalMaxItemNum)
                {
                    uiSelectNum = 0;
                }
                else
                {
                    uiSelectNum++;
                }
            }


        }
        else
        {
            if (uiFlag & FLGKEY_LEFT)
            {
                uiSelectNum =0; //"Defect Pixel"
            }

            if (uiFlag & FLGKEY_RIGHT)
            {
                uiSelectNum =1; //"Mshutter ,AWB"
            }
            if (uiFlag & FLGKEY_UP)
            {
                uiSelectNum =2; //"Burn In"
            }

            if (uiFlag & FLGKEY_DOWN)
            {
                uiSelectNum =3; //"Read Cal. Status"
            }

            if (uiFlag & FLGKEY_PLAYBACK)
            {
                uiSelectNum =5; //""Del CAL"
            }
        }
        // Run process
        if (uiFlag & FLGKEY_SHUTTER2)
        {
            AAA_Adjust.currentItem = uiSelectNum;
            // Clear OSD
            //ShowOSD_ClearOSD(IDE_OSDID_1);
            pDCList = (DC**)UI_BeginScreen();
            GxGfx_Clear(pDCList[GxGfx_OSD]);
            UI_EndScreen((UINT32)pDCList);

            // Disable auto power off
            KeyScan_EnableAutoPoweroff(FALSE);
            // Disable sleep mode
            //KeyScan_EnableSleep(FALSE);

            // Run test process
            if( g_CalItem[uiSelectNum].ItemFunction != NULL)
            {
                g_CalItem[uiSelectNum].ItemFunction();
            }

            // Close all task
            AppInit_Close();

            // Clear OSD
            //ShowOSD_ClearOSD(IDE_OSDID_1);
            pDCList = (DC**)UI_BeginScreen();
            GxGfx_Clear(pDCList[GxGfx_OSD]);
            UI_EndScreen((UINT32)pDCList);
        }

        #if 0  //no menu key
        // Exit calibration task
        if (uiFlag & FLGKEY_MENU)
        {
            g_bCalbirationOpened = FALSE;

            // Return to photo task
            Primary_ChangeMode(PRIMARY_MODE_PHOTO);

            // Enable auto power off
            KeyScan_EnableAutoPoweroff(TRUE);
            // Enable sleep mode
            //KeyScan_EnableSleep(TRUE);

            ext_tsk();
        }
        #endif
    }
    **/
}

#endif

//@}
