/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitPhoto.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for Photo mode
                Get memory and set parameters for Photo mode

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include <stdio.h>
#include <string.h>
#include "WavStudioTsk.h"
#include "UIFlow.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "SieCapTsk.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "fd_lib.h"
#include "SD_lib.h"
//#NT#2011/04/29#Ethan Lau -begin
//#NT#registor ae/awb
#include "ae_api.h"
#include "awb_api.h"
//#NT#2011/04/29#Ethan Lau -end
//#NT#2011/10/18#Jarkko Chang -begin
//#NT#add for AF task
#include "caf_lib.h"
#include "AFTsk.h"
#include "af_api.h"
//#NT#2011/10/18#Jarkko Chang -end
//#NT#2011/04/29#Ethan Lau -begin
//#NT#fpga test,for datestamp read from sd card
#include "datastamp.h"
//#NT#2011/04/29#Ethan Lau -end
#include "disp_rotate.h"
// Define date imprint buff size for using the rest SICD buff when capture
//#define DATE_IMPRINT_TOTAL_BUFSIZE      0x20000
#define DATE_IMPRINT_TOTAL_BUFSIZE      (POOL_SIZE_DATE_STAMP + POOL_SIZE_DATE_STAMP_THUMB)

extern void     FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo);

/**
  Initialize application for Photo mode

  Initialize application for Photo mode.

  @param void
  @return void
*/
void AppInit_ModePhoto(void)
{
    UINT32              uiPoolAddr, uiPoolAddr2;
    PHOTO_APPOBJ        PhotoObj;
    CAPTURE_APPOBJ      CaptureObj;
    PHOTODISPLAY_APPOBJ PhotoDisplayObj;
#if _WAVSTUDIO_MODE_
    WAVSTUDIO_APPOBJ    WavObj;
    WAVSTUDIO_FMT       WavFormat;
#endif

    //#NT#2011/11/23#Philex Lin -begin
    //#NT#Modify for display rotation
    if (KeyScan_IsHDMIPlugIn() == FALSE && KeyScan_IsTVPlugIn() == FALSE)
    {
        UINT32          uiAddr, uiSize;
        PDISP_OBJ       pDispObj;
        DISPLAYER_PARAM DispLyr;
        DISPROT_PARAM   RotParam;

        disprot_lock();
        RotParam.SEL.GET_ROTATE.DispID      = DISP_1;
        RotParam.SEL.GET_ROTATE.DispLayer   = DISPLAYER_VDO1;
        RotParam.SEL.GET_ROTATE.RotDir      = DISPROT_ROTATE_NONE;  // default value
        disprot_setParameter(DISPROT_OP_GET_ROTATE, &RotParam);     // get setting
        disprot_unlock();

        if (RotParam.SEL.GET_ROTATE.RotDir != DISPROT_ROTATE_NONE)
        {
            FWRotate_SetStatus(RotParam.SEL.SET_ROTATE.RotDir);
            SysGetDispRotateAddr(&uiAddr);
            uiSize = 320*360;//g_LCDSize.uiWidth * g_LCDSize.uiHeight;

            disprot_lock();

            pDispObj = disp_getDisplayObject(DISP_1);


            // Should set display window size since it may be changed in preview mode (such as 16:9 image).
            DispLyr.SEL.SET_WINSIZE.uiWinWidth   = g_LCDSize.uiWinHeight;   // rotate: w => h
            DispLyr.SEL.SET_WINSIZE.uiWinHeight  = g_LCDSize.uiWinWidth;    // rotate: h => w
            DispLyr.SEL.SET_WINSIZE.iWinOfsX     = 0;
            DispLyr.SEL.SET_WINSIZE.iWinOfsY     = 0;
            pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_WINSIZE, &DispLyr);

            // Set the display rotation buffer for playback mode
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
        }else {
            FWRotate_SetStatus(DISPROT_ROTATE_NONE);
        }
    }else {
        FWRotate_SetStatus(DISPROT_ROTATE_NONE);
    }
    //#NT#2011/11/23#Philex Lin -end

    // Get memory for Capture task
    #if (DRAM_END <= 0x00800000)
    get_blk((VP *)&uiPoolAddr, POOL_ID_DEFECT_PIXEL);
    rel_blk(POOL_ID_DEFECT_PIXEL, (VP)uiPoolAddr);
    #else
    get_blk((VP *)&uiPoolAddr, POOL_ID_FB);
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);
    #endif

    SysGetPhotoEndAddr(&uiPoolAddr2);

    memset(&CaptureObj, 0x00, sizeof(CAPTURE_APPOBJ));
    CaptureObj.uiMemoryAddr         = uiPoolAddr;
    CaptureObj.uiMemorySize         = uiPoolAddr2 - uiPoolAddr;
    CaptureObj.CallBackFunc         = (FPALGCALLBACK)FlowPhoto_ImageCB;

    debug_ind(("[AppInitPhoto]: Capture uiMemoryAddr = 0x%08x, uiMemorySize = 0x%08x\r\n", CaptureObj.uiMemoryAddr, CaptureObj.uiMemorySize));

    // Open Capture task
    if (ImgCapture_Open(&CaptureObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for capture task\r\n"));
    }

#if _WAVSTUDIO_MODE_
    // Setup parameter for WAV studio task
    SysGetRamDiskAddr(&uiPoolAddr2);
    WavObj.uiMemoryAddr         = uiPoolAddr2 - POOL_SIZE_WAV;
    WavObj.uiMemorySize         = POOL_SIZE_WAV;
    WavObj.WavStudioCB          = (WAVSTUDIO_CB)FlowCommon_WAVStudioCB;

    WavFormat.AudioChannel      = AUDIO_CH_RIGHT;   //AUDIO_CH_LEFT;
    WavFormat.AudioSamplingRate = AUDIO_SR_11025;
    WavFormat.AudioCodec        = AUDIOCODEC_IMA_ADPCM;
    WavStudio_SetFormat(&WavFormat);

    WavStudio_SetMaxRecTime(10);

    // Open Wav studio task
    if (WavStudio_Open(&WavObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for wav studio task\r\n"));
    }
#endif

    // Get memory for Photo task
    get_blk((VP *)&uiPoolAddr, POOL_ID_FB);
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);

    memset(&PhotoObj, 0x00, sizeof(PHOTO_APPOBJ));
    PhotoObj.uiMemoryAddr           = uiPoolAddr;
    PhotoObj.uiMemorySize           = POOL_SIZE_FB;
    PhotoObj.CallBackFunc           = (FPALGCALLBACK)FlowPhoto_ImageCB;

#if 0
    if (KeyScan_IsTVPlugIn() == TRUE)
    {
        PhotoObj.uiDisplayFBWidth   = g_LCDSize.uiWidth >> 1;
        PhotoObj.uiDisplayFBHeight  = g_LCDSize.uiHeight >> 1;
    }
    else
    {
        PhotoObj.uiDisplayFBWidth   = g_LCDSize.uiWidth;
        PhotoObj.uiDisplayFBHeight  = g_LCDSize.uiHeight;
    }
#else // don't resize the frame buffer
    if (g_LCDSize.uiWidth < 320)
    {
        PhotoObj.uiDisplayFBWidth       = 320;
        PhotoObj.uiDisplayFBHeight      = 240;
    }
    else
    {
        PhotoObj.uiDisplayFBWidth       = g_LCDSize.uiWidth;
        PhotoObj.uiDisplayFBHeight      = g_LCDSize.uiHeight;
    }
#endif

    // Open Photo task
    if (Photo_Open(&PhotoObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo task\r\n"));
    }

    memset(&PhotoDisplayObj, 0x00, sizeof(PHOTODISPLAY_APPOBJ));
    PhotoDisplayObj.uiDisplayFBWidth    = PhotoObj.uiDisplayFBWidth;
    PhotoDisplayObj.uiDisplayFBHeight   = PhotoObj.uiDisplayFBHeight;
    PhotoDisplayObj.uiDisplayOutWidth      = g_LCDSize.uiWinWidth;
    PhotoDisplayObj.uiDisplayOutHeight     = g_LCDSize.uiWinHeight;

    // Open PhotoDisplay task
    if (PhotoDisplay_Open(&PhotoDisplayObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo display task\r\n"));
    }

    // Register AE & AWB function pointer
    Photo_RegisterAE((FPPHOTOAE)AE_Process);
    Photo_RegisterAWB((FPPHOTOAWB)AWB_Process);

#if (_FD_FUNCTION_ == ENABLE)
    // Get memory for FD/SD function
    SysGetFdSdAddr(&uiPoolAddr2);
    fd_SetWorkingBufAddr(uiPoolAddr2);

    //Register FD entry
    fd_SetFaceSize(40);
    Photo_RegisterFD((FPPHOTOFD)fd_FaceDetectionProcess);
#else
    Photo_RegisterFD(NULL);
#endif


#if (_SD_FUNCTION_ == ENABLE)
    //Register SD entry
    Photo_RegisterSD((FPPHOTOSD)sd_SmileDetectionProcess);
#else
    Photo_RegisterSD(NULL);
#endif

    if (FWRotate_GetStatus()==TRUE)
    {
        FWRotate_Open();
    }

}

//@}
