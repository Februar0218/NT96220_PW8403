/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalCIPA.c
    @ingroup    mIPRJAPTest

    @brief      CIPA Test
                CIPA Test

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <stdio.h>
#include <string.h>
#include "Kernel.h"
#include "SysCfg.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "SieCapTsk.h"
#include "CalibrationTsk.h"
#include "FilesysTsk.h"

#if (_CALIBRATION_MODE_ == ENABLE)

// Temporary solution
// The maximum and minimum section is lens dependent,
// and should be return by lens library. 2006/01/04 by Chris Hsu
#define MAX_ZOOM_SECTION        11
#define MIN_ZOOM_SECTION        1

static UINT32   uiTotalPicNum;

/**
  Update total took picture number in XXXX0001.WAV

  Update total took picture number in XXXX0001.WAV
  [CalCIPA internal API]

  @param void
  @return void
*/
static void Cal_CIPAUpdatePicNum(void)
{
    UINT32 uiSize;

    uiSize = 4;
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysReadWriteSpeciFile(FST_CMD_WRITE_SPECIFIC_FILE,
                              100,
                              1,
                              FST_FMT_WAV,
                              (UB *)&uiTotalPicNum,
                              &uiSize,
                              FST_TIME_INFINITE);
    FilesysCloseActFile();
}

/**
  Read total took picture number from XXXX0001.WAV

  Read total took picture number from XXXX0001.WAV
  [CalCIPA internal API]

  @param void
  @return void
*/
static void Cal_CIPAReadPicNum(void)
{
    UINT32 uiSize;

    uiSize = 4;
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    if (FilesysReadWriteSpeciFile(FST_CMD_READ_SPECIFIC_FILE,
                                  100,
                                  1,
                                  FST_FMT_WAV,
                                  (UB *)&uiTotalPicNum,
                                  &uiSize,
                                  FST_TIME_INFINITE) != FST_STA_OK)
    {
        uiTotalPicNum = 0;
    }
    FilesysCloseActFile();
}

/**
  CIPA test flow

  CIPA test flow

  @param void
  @return void
*/
void Cal_CIPA(void)
{
    UINT32  uiCIPACnt, uiCIPA30Cnt;
    BOOL    bZoomIn, bZoomOut;
    INT8    StringBuffer[32];
    RECT    StringRect = {20, 200, 300, 30};

    // Change to Photo Mode
    Primary_ChangeMode(PRIMARY_MODE_PHOTO);

    uiCIPACnt       = 1;
    uiCIPA30Cnt     = 0;
    Cal_CIPAReadPicNum();

    bZoomIn     = FALSE;
    bZoomOut    = FALSE;

    while (1)
    {
        uiCIPACnt++;

        // Show string
        sprintf(StringBuffer,
                "CIPA: %ld , Z: %s F: %s",
                uiTotalPicNum,
                (uiCIPA30Cnt & 1) ? "Out" : "In",
                (uiCIPA30Cnt & 1) ? "Off" : "On");
        Cal_ShowString(StringBuffer, &StringRect, TRUE);

        if (bZoomIn == TRUE)
        {
            if (Lens_Zoom_GetSection() >= Lens_Zoom_GetMaxSection())
            {
                uiTotalPicNum++;
                Cal_CIPAUpdatePicNum();
                bZoomIn = FALSE;
                set_flg(FLG_ID_KEY_REL, FLGKEY_ZOOMIN);
                set_flg(FLG_ID_KEY, FLGKEY_RELEASED | FLGKEY_SHUTTER1 | FLGKEY_SHUTTER2);

                uiCIPA30Cnt++;
            }
        }
        else if (bZoomOut == TRUE)
        {
            if (Lens_Zoom_GetSection() <= MIN_ZOOM_SECTION)
            {
                uiTotalPicNum++;
                Cal_CIPAUpdatePicNum();
                bZoomOut = FALSE;
                set_flg(FLG_ID_KEY_REL, FLGKEY_ZOOMOUT);
                set_flg(FLG_ID_KEY, FLGKEY_RELEASED | FLGKEY_SHUTTER1 | FLGKEY_SHUTTER2);

                uiCIPA30Cnt++;
            }
        }

        // 30 Second
        if ((uiCIPACnt % 30) == 0)
        {
            // Take 10 pictures
            if (uiCIPA30Cnt < 10)
            {
                if (uiCIPA30Cnt & 1)
                {
                    // Zoom Out and Flash = Off
                    set_flg(FLG_ID_KEY, FLGKEY_ZOOMOUT);
                    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
                    bZoomOut = TRUE;
                }
                else
                {
                    // Zoom In and Flash = FULL
                    set_flg(FLG_ID_KEY, FLGKEY_ZOOMIN);
                    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceON);
                    bZoomIn = TRUE;
                }
            }
            else
            {
                KeyScan_Poweroff();
            }
        }
    }
}

#endif

//@}
