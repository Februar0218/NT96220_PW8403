/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitAVI.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for AVI record
                Get memory and set parameters for AVI record

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include <stdio.h>
#include <string.h>
#include "UIFlow.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "AppInitAVI.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "ae_api.h"
#include "awb_api.h"
#include "Movieinterface.h"
#include "AFTsk.h"
#include "af_api.h"
#include "caf_lib.h"
#include "RawEncAPI.h"
#include "disp_rotate.h"


#if 1
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif


// front-end buffer request for video recording:
// IDE buffer (IME path 1, YUV422, 3 buffers) + SIE raw buffer (ping-pong) + CA buffer + AVI-frame buffer (IME path 2, YUV422, 3 buffers)
#define SENSOR_RECORD_SIZE(ideInSize, sieOutSize, videoMaxW, videoMaxH) \
        (6 * ideInSize + 2 * sieOutSize + sieOutSize / 4 + 6 * videoMaxW * videoMaxH)

#define AUD_RECORD_SIZE(sampleRate, bits, seconds)      (16*(sampleRate*bits/8+8) + seconds*4)

#if (_MOVIE_MAX_RES_ == _MOVIE_MAX_RES_1080P_)
#define AVI_MAX_W               1920            // AVI maximum width
#define AVI_MAX_H               1080            // AVI maximum height
#define AVI_TARGET_BIT_RATE     (200*1024)      // Bytes per frame
#elif (_MOVIE_MAX_RES_ == _MOVIE_MAX_RES_720P_)
#define AVI_MAX_W               1280            // AVI maximum width
#define AVI_MAX_H               720             // AVI maximum height
#define AVI_TARGET_BIT_RATE     (70*1024)       // Bytes per frame
#else
#define AVI_MAX_W               640             // AVI maximum width
#define AVI_MAX_H               480             // AVI maximum height
#define AVI_TARGET_BIT_RATE     (30*1024)       // Bytes per frame
#endif

#if (_OSD_SCREEN_DUMP_ == ENABLE)
#define AVI_MAX_SECOND          (60 * 30)       // 30 minutes if enable OSD dump function (otherwise the buffer is not enough)
#else
#define AVI_MAX_SECOND          (60 * 60 * 1.5)   // 1.5 hours
#endif

#define AUD_SAMPLE_RATE     AUDIO_SR_32000   // 8KHz
#define AUD_SAMPLE_BITS     16              // 16-bits

extern void FlowMovie_RecCB(UINT32 uiEventID, UINT32 uiSeconds);
extern void FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo);
UINT32 gOpenAviAddr, gOpenAviSize;//2011/06/15 Meg Lin


/**
  Initialize application for AVI mode

  Initialize application for AVI mode.

  @param void
  @return void
*/
void AppInit_ModeAVI(void)
{
    //AVIREC_OBJ          AVIEncObj;
    PHOTO_APPOBJ        PhotoObj;
    PHOTODISPLAY_APPOBJ PhotoDisplayObj;
    //AUDIO_DEVICE_OBJ    AudioDevice;
    UINT32              uiPoolAddr;
    UINT32              uiSensorRecordSize; // front-end request buffer size as video recording
    UINT32              uiAudioRecordSize;  // audio request buffer size as video recording
    MOVIEREC_OBJ        MovRecObj;
    //#NT#2011/11/04#KS Hung -begin
    #if (_MOVIE_PIM_MODE_ == ENABLE)
    RAW_ENC_OBJ         RawEncObj;
    #endif
    //#NT#2011/11/04#KS Hung -end

    //#NT#2011/04/29#Ethan Lau -begin
    //#NT#todo220 Get_DzoomPrvMaxBuffer() api has been removed
    uiSensorRecordSize = SENSOR_RECORD_SIZE(
                            320*240,                    // IDE maximum size (for LCD only, not considering TV temporary)
                            ////640*960,                // SIE output size (should refer to sensor settings)
                            //Get_DzoomPrvMaxBuffer(),    // SIE output size (should refer to sensor settings)
                            1280*960*3/2,
                            AVI_MAX_W, AVI_MAX_H);      // AVI maximum size
    //#NT#2011/04/29#Ethan Lau -end
//    uiAudioRecordSize = AUD_RECORD_SIZE(AUD_SAMPLE_RATE, AUD_SAMPLE_BITS, AVI_MAX_SECOND);
    uiAudioRecordSize = 0;//AUD_RECORD_SIZE(AUD_SAMPLE_RATE, AUD_SAMPLE_BITS, AVI_MAX_SECOND);
    DEBUG_MSG(("[AppInitAVI]: uiSensorRecordSize = 0x%08x, uiAudioRecordSize = 0x%08x\r\n", uiSensorRecordSize, uiAudioRecordSize));

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
//            uiSize = 320*360;//g_LCDSize.uiWidth * g_LCDSize.uiHeight;
            uiSize = 480*272;//g_LCDSize.uiWidth * g_LCDSize.uiHeight;

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

    // Get memory for Photo task
    get_blk((VP*)&(uiPoolAddr), POOL_ID_FB);
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);

    memset(&PhotoObj, 0x00, sizeof(PHOTO_APPOBJ));
    PhotoObj.uiMemoryAddr           = uiPoolAddr;
    PhotoObj.uiMemorySize           = uiSensorRecordSize;

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
    PhotoObj.CallBackFunc           = (FPALGCALLBACK)FlowPhoto_ImageCB;

    // Open Photo task for AE & AWB
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
    Photo_RegisterFD(NULL);
    Photo_RegisterSD(NULL);

    // Get memory for AVI encoder task
    //get_blk((VP*)&(uiPoolAddr), POOL_ID_CAPTURE);
    //rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    //#NT#2011/10/21#KS Hung -begin
    //When the current frame is the same with previous, it is only updated the index header.
    AviRec_TheSame(1);
    //#NT#2011/10/21#KS Hung -end

    memset(&MovRecObj, 0x00, sizeof(MOVIEREC_OBJ));

    //AVIEncObj.uiMemAddr         = PhotoObj.uiMemoryAddr + uiSensorRecordSize + uiAudioRecordSize;
    //AVIEncObj.uiMemSize         = ((POOL_SIZE_FB + POOL_SIZE_CAPTURE + POOL_SIZE_SICD) - uiSensorRecordSize - uiAudioRecordSize);
    //DEBUG_MSG(("[AppInitAVI]: AVIEncObj.uiMemAddr = 0x%08x, AVIEncObj.uiMemSize = 0x%08x\r\n", AVIEncObj.uiMemAddr, AVIEncObj.uiMemSize));
    MovRecObj.uiMemAddr         = PhotoObj.uiMemoryAddr + uiSensorRecordSize + uiAudioRecordSize;
    MovRecObj.uiMemSize         = END_MEM - MovRecObj.uiMemAddr - POOL_SIZE_RAM_DISK - SysGetDispRotateBuffSize();

    #if (_MOVIE_PIM_MODE_ == ENABLE)
    RawEncObj.uiMemAddr         = MovRecObj.uiMemAddr;
    RawEncObj.uiMemSize         = POOL_SIZE_PIM;
    RawEncObj.CallBackFunc      = FlowMovie_RawEncCB;
    RawEnc_Open(&RawEncObj);

    MovRecObj.uiMemAddr         += RawEncObj.uiMemSize;
    MovRecObj.uiMemSize         -= RawEncObj.uiMemSize;
    #endif

    //2011/07/28 Meg, add dateImprint buffer sample
    //{
    //    UINT32 dateimprintAddr;
    //    SysGetMovieDSAddr(&dateimprintAddr);
    //    MovRecObj.uiMemSize         = dateimprintAddr-MovRecObj.uiMemAddr;//END_MEM - MovRecObj.uiMemAddr - POOL_SIZE_RAM_DISK;
    //}
    DEBUG_MSG(("[AppInitAVI]: AVIEncObj.uiMemAddr = 0x%08x, AVIEncObj.uiMemSize = 0x%08x\r\n", MovRecObj.uiMemAddr, MovRecObj.uiMemSize));
    gOpenAviAddr = MovRecObj.uiMemAddr;
    gOpenAviSize = MovRecObj.uiMemSize;
    //AVIEncObj.CallBackFunc      = NULL;
    MovRecObj.CallBackFunc      = FlowMovie_RecCB;      ///< event inform callback
    //AVIEncObj.uiRecFormat       = AVIREC_AUD_VID_BOTH;
    MovRecObj.uiRecFormat       = MOVIEREC_RECFORMAT_AUD_VID_BOTH;

    // Set video parameters
    //AVIEncObj.uiVidFrameRate    = 30;//GetMovieFrameRate(SysGetFlag(FL_MOVIE_SIZE));
    MovRecObj.uiVidFrameRate    = 30;// 2011/01/12 APPINITAVI_TARGET_FRAME_RATE;
    MovRecObj.ui1stFrameQp      = MOVIEREC_1STQP_NORMAL;
    MovRecObj.uiCBthreshold     = MovRecObj.uiVidFrameRate;      ///< video frames for callback, usually equals to frame rate
    //AVIEncObj.uiWidth           = 1280;//GetMovieSizeWidth(SysGetFlag(FL_MOVIE_SIZE));
    //AVIEncObj.uiHeight          = 720;//GetMovieSizeHeight(SysGetFlag(FL_MOVIE_SIZE));
    //AVIEncObj.uiRecQuality      = AVIREC_QUALITY_NORMAL;//GetMovieQualityValue(SysGetFlag(FL_MOVIE_QUALITY));

    MovRecObj.uiMaxWidth        = AVI_MAX_W;        ///< recording max video width
    MovRecObj.uiMaxHeight       = AVI_MAX_H;        ///< recording max video height
    MovRecObj.uiWidth           = 1280;
    MovRecObj.uiHeight          = 720;
    MovRecObj.uiJpegFormat      = MOVIEREC_JPGFMT_420;   // use 420 to save memory//2010/10/28 Meg
    MovRecObj.uiTargetFramesize = AVI_TARGET_BIT_RATE;

    //AVIEncObj.uiJpegFormat      = AVIREC_JPEG_FORMAT_420;   // use 420 to save memory//2010/10/28 Meg

    //AVIEncObj.bEnableBRCtrl     = TRUE;
    //AVIEncObj.uiTargetBitrate   = 70*1024;//GetMovieTargetBitrate(SysGetFlag(FL_MOVIE_SIZE));   //Bytes per second (Unit: byte)
    //AVIEncObj.ucSaveFrameNum    = 5;//2010/10/28 Meg

    // Set audio parameters
    //AVIEncObj.uiAudSampleRate   = AUD_SAMPLE_RATE;
    //AVIEncObj.uiAudBits         = AUD_SAMPLE_BITS;
    //AVIEncObj.uiAudChannels     = 1;
    MovRecObj.uiAudSampleRate   = AUD_SAMPLE_RATE;
    MovRecObj.uiAudBits         = AUD_SAMPLE_BITS;
    MovRecObj.uiAudChannels     = 1;
    MovRecObj.audioCh=AUDIO_CH_RIGHT;            ///< audio recording channel, AUDIO_CH_LEFT or others//2009/12/22 Meg
    // Setup User Data

    //#Init audio buffer with size parameter
    //AVIEncObj.uiAudioBufferAddr     = PhotoObj.uiMemoryAddr + uiSensorRecordSize;
    //AVIEncObj.uiAudioBufferSize     = uiAudioRecordSize;//2010/10/28 Meg
    MovRecObj.uiAudioBufferAddr     = PhotoObj.uiMemoryAddr + uiSensorRecordSize;
    MovRecObj.uiAudioBufferSize     = uiAudioRecordSize;//2010/10/28 Meg
    //AVIEncObj.uiSavefileduration    = AVIREC_SAVEFILE_HALF_SECOND;
    //AVIEncObj.uiMaxRecSecs          = AVI_MAX_SECOND;
    //AVIEncObj.uiMaxRecFileSize      = 0xFFFFFFFF;   // Set max rec file size (4G size)
    MovRecObj.uiMaxRecSecs          = AVI_MAX_SECOND;

    // If uiNoiseFilterLevel == 0, then the noise filter would be closed.
    //AVIEncObj.uiNoiseFilterLevel = 500;
    //AVIEncObj.uiNoiseFilterNoiseDecay = AVIREC_NOISE_DECAY_1_2;
    //AVIEncObj.uiSystemRAMSize = DRAM_END;
    MovRecObj.ucSeamlessSec=600;      ///< seamless cut second,
    #if (_MEDIA_REC_ == _APPMEDIAREC_)
    MovRecObj.ucEngType=MOVIEREC_MEDIAREC;          ///< engine, MOVIEREC_AVIREC, MOVIEREC_MEDIAREC
    #else
    MovRecObj.ucEngType=MOVIEREC_AVIREC;          ///< engine, MOVIEREC_AVIREC, MOVIEREC_MEDIAREC
    #endif
    MovRecObj.ucEndType=MOVIEREC_ENDTYPE_NORMAL;          ///< end type, CUTOVERLAP, NORMAL
    MovRecObj.ucSaveFrameNum    = 5;//mediaRecObj.ucSaveFrameNum; //if memory enough, can be frame number. else, 3,5,6,10,15,30

    // Open AVI encoder task
    //if (AVIRec_Open(&AVIEncObj) != E_OK)
    //{
    //    debug_err(("Error opening AVI task\r\n"));
    //}

    // Register callback function
    //AVIRec_RegisterCB(FlowMovie_RecCB);
    MovRecObj.uiUserDataAddr=0;
    MovRecObj.uiUserDataSize=0;

    MovRecObj.uiPanelWidth =PhotoObj.uiDisplayFBWidth;//2011/06/10 Meg
    MovRecObj.uiPanelHeight=PhotoObj.uiDisplayFBHeight;//2011/06/10 Meg
    debug_msg("IME P1 = %d!!!!%d!!!!\r\n", MovRecObj.uiPanelWidth, MovRecObj.uiPanelHeight);
    if (MovRec_Open(&MovRecObj) != E_OK)
    {
        debug_err(("Error opening AVI task\r\n"));
    }

    if (FWRotate_GetStatus()==TRUE)
    {
        FWRotate_Open();
    }

}

//@}
