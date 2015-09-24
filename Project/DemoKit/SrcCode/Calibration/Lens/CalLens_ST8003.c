/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       Cal_Lens_ST8003B.c
    @ingroup    mISYSLens

    @brief      Lens calibration library

    @note       Nothing.

    @version    V1.00.000
    @author     iView SA
    @date       2011/02/26
*/

#include    <stdio.h>
#include    <string.h>
#include    "SysCfg.h"
#include    "Lens.h"
#include    "CalLens.h"
#include    "UILensObj.h"
#include    "PhotoTsk.h"
#include    "KeyScanTsk.h"
#include    "IDE.h"
#include    "UIResource.h"
#include    "sie_lib.h"
#include    "Utility.h"

#if (_CALIBRATION_MODE_ == ENABLE)

UINT16 gCalLens_FocusRangeTable[CALLENS_ZOOM_SECTION_NUM][2]={
//  Inf     , 0.5m
{   344-100  , 357+80 },
{   302-100  , 323+80 },
{   254-100  , 287+80 },
{   198-100  , 251+80 },
{   155-100  , 235+80 },
{   139-100  , 247+80 },
{   203-100  , 317+80 },
};

UINT32 CalLens_ZoomBLAdjust(void);
UINT32 CalLens_FocusBLAdjust(void);

UINT8 CalLens_FarFocusAdjust(void)
{
    INT32   maxIdx = 0;
    INT16   StartIdx,EndIdx;
    UINT32  i;
    UINT    AF_Step;
    FLGPTN  uiFlag;
    char    str1[32];
    UINT32  maxVA= 0;
    BOOL    bFocusNG = FALSE;
    IPL_OBJ IplObj;

    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    //Cal_AlgClearScreen();
    //Cal_AlgShowString("1. Face ISO chart(far)",0,0);
    //Cal_AlgShowString("2. Press shutter key",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    //Cal_AlgClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    //Photo_LockCAF(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD));
    sie_waitVD(1);
    KeyScan_Suspend();

    // Get zoom and focus BL.
    #if 1
    {
        ide_disable_video(IDE_VIDEOID_1);

        snprintf(str1, 32, "Please wait...");
        //Cal_AlgShowString(str1,0,0);

        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);

        gCalLens_Info.uiLensZoomBL  = CalLens_ZoomBLAdjust();

        snprintf(str1, 32, "zoomBL=%ld", gCalLens_Info.uiLensZoomBL);
        //Cal_AlgShowString(str1,0,0);

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
        Lens_Focus_GotoHome();
        Lens_Zoom_Goto(i+1);

        StartIdx = gCalLens_FocusRangeTable[i][0];
        EndIdx = gCalLens_FocusRangeTable[i][1];
        Lens_Focus_Goto(StartIdx);
        UILens_WaitAeConverge();

        UILens_AFPrepare(TRUE);

        if(i==0)
            AF_Step = 2;
        else
            AF_Step = 4;

        maxIdx = UILens_SearchFullRange(StartIdx, EndIdx, 10);
        maxIdx = UILens_SearchFullRange(maxIdx-8, maxIdx+8, AF_Step);
        gCalLens_Info.Far_Focus[i] = maxIdx;

        //Cal_AlgClearScreen();
        snprintf(str1, 32, "S=%ld,Far=%ld,VA=%ld", i+1, gCalLens_Info.Far_Focus[i], maxVA);

        if(maxVA > 80000)
        {
            //if(i % 2 == 0)
            //    Cal_AlgShowString(str1,0,10*(i/2));
            //else
            //    Cal_AlgShowString(str1,30,10*(i/2));
        }
        else
        {
            //bFocusNG =TRUE;
            //if(i % 2 == 0)
            //    Cal_AlgShowString(str1,0,10*(i/2));
            //else
            //    Cal_AlgShowString(str1,30,10*(i/2));
        }

        UILens_AFPrepare(FALSE);
    }

    Lens_OnOff(MOTOR_POWER_OFF, 0);

    if(bFocusNG)
        gCalLens_Info.bFarAdjust = FALSE;
    else
        gCalLens_Info.bFarAdjust = TRUE;

    CalLens_WriteCalData();
    KeyScan_Resume();

    /************************************************************/
    //Cal_AlgClearScreen();
    //if(bFocusNG)
    //    Cal_AlgShowString("1. Adjust NG (far)",0,0);
    //else
    //    Cal_AlgShowString("1. Adjust done (far)",0,0);
    //Cal_AlgShowString("2. Press left key to leave",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_LEFT);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    /*************************************************************/
    return CALLENS_STA_OK;
}

UINT8 CalLens_NearFocusAdjust(void)
{
    INT32   maxIdx = 0;
    INT16   StartIdx,EndIdx;
    UINT32  i;
    UINT    AF_Step;
    FLGPTN  uiFlag;
    char    str1[32];
    UINT32  maxVA = 0;
    BOOL    bFocusNG = FALSE;
    //IPL_OBJ IplObj;

    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    //Cal_AlgClearScreen();
    //Cal_AlgShowString("1. Face ISO chart (near)",0,0);
    //Cal_AlgShowString("2. Press shutter key",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    //Cal_AlgClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    //Photo_LockCAF(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD));
    sie_waitVD(1);
    KeyScan_Suspend();

    for(i=0;i<Lens_Zoom_GetMaxSection();i++)
    {
        Lens_Focus_GotoHome();
        Lens_Zoom_Goto(i+1);

        if(CalLens_IsFarAdjust())
            StartIdx = gCalLens_Info.Far_Focus[i];  // 2011-06-01 Chris
        else
            StartIdx = gCalLens_FocusRangeTable[i][0];
        EndIdx = gCalLens_FocusRangeTable[i][1];
        Lens_Focus_Goto(StartIdx);
        UILens_WaitAeConverge();

        UILens_AFPrepare(TRUE);

        if(i==0)
            AF_Step = 2;
        else
            AF_Step = 4;

        maxIdx = UILens_SearchFullRange(StartIdx, EndIdx, 10);
        maxIdx = UILens_SearchFullRange(StartIdx, EndIdx, AF_Step);
        gCalLens_Info.Near_Focus[i] = maxIdx;

        if(gCalLens_Info.Near_Focus[i] <= StartIdx)
        {
            bFocusNG =TRUE;
            break;
        }
        else
        {
            //Cal_AlgClearScreen();
            snprintf(str1, 32, "S=%ld,Near=%ld,VA=%ld", i+1, gCalLens_Info.Near_Focus[i], maxVA);

            //if(i % 2 == 0)
            //    Cal_AlgShowString(str1,0,10*(i/2));
            //else
            //    Cal_AlgShowString(str1,30,10*(i/2));
        }

        UILens_AFPrepare(FALSE);
    }

    Lens_OnOff(MOTOR_POWER_OFF, 0);

    if(bFocusNG)
        gCalLens_Info.bNearAdjust = FALSE;
    else
        gCalLens_Info.bNearAdjust = TRUE;

    CalLens_WriteCalData();
    KeyScan_Resume();

    /************************************************************/
    //Cal_AlgClearScreen();
    //if(bFocusNG)
    //    Cal_AlgShowString("1. Adjust NG (near)",0,0);
    //else
    //    Cal_AlgShowString("1. Adjust done (near)",0,0);
    //Cal_AlgShowString("2. Press left key to leave",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_LEFT);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    /*************************************************************/

    return CALLENS_STA_OK;
}

INT32 CalLens_GetFarFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    INT32 retValue = 0;

    if(zoomSection > CALLENS_ZOOM_SECTION_NUM)
        return 0;

    if(CalLens_IsFarAdjust())
    {
        retValue = gCalLens_Info.Far_Focus[zoomSection-1];

        if(bCompensate)
        {
            switch(zoomSection)
            {
            case 1:
                retValue -= 14; // 2011-05-12 Chris
                break;
            case 2:
                retValue -= 14;
                break;
            case 3:
                retValue -= 16;
                break;
            case 4:
                retValue -= 16;
                break;
            case 5:
                retValue -= 20;
                break;
            case 6:
                retValue -= 22;
                break;
            case 7:
                retValue -= 24;
                break;
            }
        }
    }
    else
    {
        retValue = gCalLens_FocusRangeTable[zoomSection-1][0];
    }

    return retValue;
}

INT32 CalLens_GetNearFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    if(zoomSection > CALLENS_ZOOM_SECTION_NUM)
        return 0;

    if(CalLens_IsNearAdjust())
    {
        return (gCalLens_Info.Near_Focus[zoomSection-1]);
    }
    else
    {
        return (gCalLens_FocusRangeTable[zoomSection-1][1]);
    }
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
