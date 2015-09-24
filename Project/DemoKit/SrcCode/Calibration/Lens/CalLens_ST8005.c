/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       CalLens_ST8005.c
    @ingroup    mISYSLens

    @brief      Lens calibration library

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/21
*/

#include <stdio.h>
#include <string.h>
#include "SysCfg.h"
#include "Lens.h"
#include "CalLens.h"
#include "UILensObj.h"
////#include "AF_Lib.h"
#include "PhotoTsk.h"
#include "KeyScanTsk.h"
#include "sie_lib.h"
#include "IDE.h"
#include "Utility.h"
#include "UIGraphics.h"
#include "CalibrationInt.h"

#if (_CALIBRATION_MODE_ == ENABLE)

UINT32 CalLens_ZoomBLAdjust(void);
UINT32 CalLens_FocusBLAdjust(void);

UINT32 CalLens_FarFocusAdjust(void)
{
    UINT32  i;
    UINT16  StartIdx, EndIdx;
    //UINT32  ErrorCode=ERRORCODE_OK;
    UINT32  AF_Step;
    UINT32  maxVA=0;//, minVA=0;
    UINT    uiFlag=0;
    char    str1[32];
    BOOL    bFocusNG = FALSE;
    IPL_OBJ IplObj;

    ide_enable_video(IDE_VIDEOID_1);
    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    //Cal_ClearScreen();
    //Cal_ShowStringWithColor("1. Face ISO chart (far)", 0, 0, CAL_COLOR_MSG);
    //Cal_ShowStringWithColor("2. Press shutter key", 0, 30, CAL_COLOR_MSG);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, 0x00);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, 0x00);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2 | FLGKEY_ACT_MASK);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    //Cal_ClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD));
    sie_waitVD(1);
    KeyScan_Suspend();

    // Step1. Get zoom and focus BL.
    #if 1
    {
        ide_disable_video(IDE_VIDEOID_1);

        sprintf(str1, "Please wait...");
        //Cal_ShowStringWithColor(str1, 0, 0, CAL_COLOR_MSG);

        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);

        gCalLens_Info.uiLensZoomBL  = CalLens_ZoomBLAdjust();

        sprintf(str1, "zoomBL=%ld", gCalLens_Info.uiLensZoomBL);
        //Cal_ShowStringWithColor(str1, 0, 0, CAL_COLOR_MSG);

        Lens_Init(LENS_INIT_ZOOM_PART1);
        Lens_Init(LENS_INIT_ZOOM_PART2);

        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_PREVIEW;
        IPL_SetCmd(&IplObj);

        ide_enable_video(IDE_VIDEOID_1);
    }
    #endif

    for(i=0;i<Lens_Zoom_GetMaxSection();i++)
    {
        // Re-init lens
        {
            ide_disable_video(IDE_VIDEOID_1);

            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_OFF;
            IPL_SetCmd(&IplObj);

            Lens_Retract(0);
            TimerDelayMs(500);
            Lens_Init(LENS_INIT_ZOOM_PART1);
            Lens_Init(LENS_INIT_ZOOM_PART2);

            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_PREVIEW;
            IPL_SetCmd(&IplObj);

            ide_enable_video(IDE_VIDEOID_1);
        }

        Lens_Zoom_Goto(i+1);

        StartIdx = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 1);
        EndIdx   = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 0);
        Lens_Focus_Goto(EndIdx);

        Photo_LockAE(1);
        ////WaitAeConverge2(120);

        if(i==0)
            AF_Step=5;
        else
            AF_Step=6;

        UILens_AFPrepare(TRUE);
        ////gCalLens_Info.Far_Focus[i]=AF_SearchFullRange7(StartIdx,EndIdx,AF_Step,&maxVA,&minVA,3,TRUE,FALSE);
        Lens_OnOff(MOTOR_POWER_OFF, 0); // turn off motor power
        UILens_AFPrepare(FALSE);

        //Cal_ClearScreen();
        sprintf(str1, "S=%ld,Far=%ld,VA=%ld", i+1, gCalLens_Info.Far_Focus[i]-130, maxVA);
        //Cal_ShowStringWithColor(str1, 0, 10*(i/2), CAL_COLOR_MSG);

        if(maxVA < 2500 || gCalLens_Info.Far_Focus[i] > (845+130)) //#NT#2011/01/14#Chris Chung
        {
            debug_msg("bFocusNG: maxVA=%d, Far_Focus[%d]=%d\r\n", maxVA, gCalLens_Info.Far_Focus[i]);
            bFocusNG = TRUE;
        }
    }

    if(bFocusNG)
        gCalLens_Info.bFarAdjust = FALSE;
    else
        gCalLens_Info.bFarAdjust = TRUE;

    CalLens_WriteCalData();
    KeyScan_Resume();

    /************************************************************/
    //Cal_ClearScreen();
    //if(bFocusNG)
    //    Cal_ShowStringWithColor("1. Adjust NG (far)", 0, 0, CAL_COLOR_NG);
    //else
    //    Cal_ShowStringWithColor("1. Adjust done (far)", 0, 0, CAL_COLOR_OK);
    //Cal_ShowStringWithColor("2. Press left key to leave", 0, 30, CAL_COLOR_MSG);
    clr_flg(FLG_ID_KEY, FLGKEY_LEFT);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    /*************************************************************/

    if(bFocusNG)
        return CALLENS_STA_FAIL;
    else
        return CALLENS_STA_OK;
}

UINT32 CalLens_NearFocusAdjust(void)
{
    UINT32  i;
    UINT16  StartIdx, EndIdx;
    //UINT32  ErrorCode=ERRORCODE_OK;
    UINT32  AF_Step;
    UINT32  maxVA=0;//, minVA=0;
    UINT    uiFlag=0;
    char    str1[32];
    BOOL    bFocusNG = FALSE;
    IPL_OBJ IplObj;

    if(!CalLens_IsFarAdjust())
    {
        //Cal_ShowStringWithColor("1. Adjust NG", 0, 0, CAL_COLOR_NG);
        //Cal_ShowStringWithColor("2. Please do AF Far first!", 0, 0, CAL_COLOR_NG);
        return 0;
    }

    ide_enable_video(IDE_VIDEOID_1);
    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    //Cal_ClearScreen();
    //Cal_ShowStringWithColor("1. Face ISO chart (near)", 0, 0, CAL_COLOR_MSG);
    //Cal_ShowStringWithColor("2. Press shutter key", 0, 30, CAL_COLOR_MSG);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    //Cal_ClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD));
    sie_waitVD(1);
    KeyScan_Suspend();

    for(i=0;i<Lens_Zoom_GetMaxSection();i++)
    {
        // Re-init lens
        {
            ide_disable_video(IDE_VIDEOID_1);

            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_OFF;
            IPL_SetCmd(&IplObj);

            Lens_Retract(0);
            TimerDelayMs(500);
            Lens_Init(LENS_INIT_ZOOM_PART1);
            Lens_Init(LENS_INIT_ZOOM_PART2);

            IplObj.uiCmd = IPL_CMD_CHG_MODE;
            IplObj.Mode = IPL_MODE_PREVIEW;
            IPL_SetCmd(&IplObj);

            ide_enable_video(IDE_VIDEOID_1);
        }

        Lens_Zoom_Goto(i+1);

        StartIdx = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 1);
        if(CalLens_IsFarAdjust())
            EndIdx = gCalLens_Info.Far_Focus[i] - 3;
        else
            EndIdx   = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 0);

        Lens_Focus_Goto(EndIdx);

        Photo_LockAE(1);
        ////WaitAeConverge2(120);

        if(i==0)
            AF_Step=5;
        else
            AF_Step=6;

        UILens_AFPrepare(TRUE);
        ////gCalLens_Info.Near_Focus[i]=AF_SearchFullRange7(StartIdx,EndIdx,AF_Step,&maxVA,&minVA,3,TRUE,FALSE);
        Lens_OnOff(MOTOR_POWER_OFF, 0); // turn off motor power
        UILens_AFPrepare(FALSE);

        //Cal_ClearScreen();
        sprintf(str1, "S=%ld,Near=%ld,VA=%ld", i+1, gCalLens_Info.Near_Focus[i]-130, maxVA);
        //Cal_ShowStringWithColor(str1, 0, 10*(i/2), CAL_COLOR_MSG);

        if(CalLens_IsFarAdjust())
        {
            if(gCalLens_Info.Far_Focus[i] < gCalLens_Info.Near_Focus[i])
                bFocusNG = TRUE;
        }

        if(maxVA < 2500)
        {
            bFocusNG = TRUE;
        }
    }

    if(bFocusNG)
        gCalLens_Info.bNearAdjust = FALSE;
    else
        gCalLens_Info.bNearAdjust = TRUE;

    CalLens_WriteCalData();
    KeyScan_Resume();

    /************************************************************/
    //Cal_ClearScreen();
    //if(bFocusNG)
    //    Cal_ShowStringWithColor("1. Adjust NG (near)", 0, 0, CAL_COLOR_NG);
    //else
    //    Cal_ShowStringWithColor("1. Adjust done (near)", 0, 0, CAL_COLOR_OK);
    //Cal_ShowStringWithColor("2. Press left key to leave", 0, 30, CAL_COLOR_MSG);
    clr_flg(FLG_ID_KEY, FLGKEY_LEFT);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    /*************************************************************/

    if(bFocusNG)
        return CALLENS_STA_FAIL;
    else
        return CALLENS_STA_OK;
}

INT32 CalLens_GetFarFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    if(zoomSection > CALLENS_ZOOM_SECTION_NUM)
        return 0;

    return (gCalLens_Info.Far_Focus[zoomSection-1]);
}

INT32 CalLens_GetNearFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    if(zoomSection > CALLENS_ZOOM_SECTION_NUM)
        return 0;

    return (gCalLens_Info.Near_Focus[zoomSection-1]);
}

UINT32 CalLens_ZoomBLAdjust(void)
{
    //UINT32 i, loop=2;
    UINT32 curBL, maxBL=0;

    //for(i=0; i<loop; i++)
    {
        curBL = Lens_Zoom_GetBL();
        TimerDelayMs(100);

        if(curBL > maxBL)
            maxBL = curBL;
    }

    return (maxBL+3);
}

UINT32 CalLens_FocusBLAdjust(void)
{
    UINT32 i, loop=2;
    UINT32 curBL, maxBL=0;

    for(i=0; i<loop; i++)
    {
        curBL = Lens_Focus_GetBL();
        TimerDelayMs(100);

        if(curBL > maxBL)
            maxBL = curBL;
    }

    return (maxBL+3);
}

#endif
