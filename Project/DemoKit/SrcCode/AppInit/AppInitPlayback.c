/*
    @file       AppInitPlayback.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for Playback mode.

                Get memory and set parameters for Playback mode.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

/**
    @addtogroup mIPRJAPCfg
*/
//@{

#include <string.h>
#include "WavStudioTsk.h"
#include "UIFlow.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "MediaPlayAPI.h"
#include "UISystemStatusSettings.h"
#include "disp_rotate.h"

//#NT#2009/03/24#Hideo Lin -begin
//#NT#For wav playback test
#if (_WAVSTUDIO_MODE_ == ENABLE)
extern void FlowPB_WAVStudioCB(UINT32 uiEventID);
#endif
//#NT#2009/03/24#Hideo Lin -end

UINT32  g_uiMovieBGAddrY;       //< image Y buffer address for movie background
UINT32  g_uiMovieBGAddrCb;      //< image Cb buffer address for movie background
UINT32  g_uiMovieBGAddrCr;      //< image Cr buffer address for movie background
UINT32  g_uiBadFileBGAddrY;     //< image Y buffer address for Bad File background
UINT32  g_uiBadFileBGAddrCb;    //< image Cb buffer address for Bad File background
UINT32  g_uiBadFileBGAddrCr;    //< image Cr buffer address for Bad File background

//#NT#2008/04/28#Chris Hsu -begin
//#NT#Add sample code for user data
#if (_WAV_SINGLE_USERDATA_ == ENABLE)
UINT8   g_uiWAVHeaderUserData[180] =
    {
    'L', 'I', 'S', 'T',
    0x30, 0x00, 0x00, 0x00,
    'I', 'N', 'F', 'O',
    'I', 'N', 'A', 'M',
    0x10, 0x00, 0x00, 0x00,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
    'I', 'C', 'R', 'D',
    0x0C, 0x00, 0x00, 0x00,
    '2', '0', '0', '8', '-', '0', '4', '-', '2', '5', 0x00, 0x00,
    'L', 'I', 'S', 'T',
    0x74, 0x00, 0x00, 0x00,
    'e', 'x', 'i', 'f',
    'e', 'v', 'e', 'r',
    0x04, 0x00, 0x00, 0x00,
    '0', '2', '2', '1',
    'e', 'r', 'e', 'l',
    0x0E, 0x00, 0x00, 0x00,
    'I', 'M', 'A', 'G', '0', '0', '0', '1', '.', 'J', 'P', 'G', 0x00, 0x00,
    'e', 't', 'i', 'm',
    0x0E, 0x00, 0x00, 0x00,
    '1', '2', ':', '3', '4', ':', '5', '6', '.', '7', '8', '9', 0x00, 0x00,
    'e', 'c', 'o', 'r',
    0x18, 0x00, 0x00, 0x00,
    'N', 'o', 'v', 'a', 't', 'e', 'k', ' ', 'M', 'i',
    'c', 'o', 'e', 'l', 'e', 'c', ' ', 'C', 'O', 'R',
    'P', ' ', 0x00, 0x00,
    'e', 'm', 'd', 'l',
    0x10, 0x00, 0x00, 0x00,
    'N', 'T', '9', '6', '4', '3', '2', '-', '2', '0',
    '0', '8', '-', '0', '4', 0x00
};
#endif
//#NT#2008/04/28#Chris Hsu -end

// Slide effect speed table
//#NT#2009/04/29#Hideo Lin -begin
//#NT#Should use different slide effect steps for TV/LCD output
//static UINT8 SlideSpeedTable[SLIDE_EFFECT_IMAGE_STEP] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
static UINT8 SlideSpeedTableTV[32];
//static UINT8 SlideSpeedTableLCD[10];
static UINT8 SlideSpeedTableLCD[32];
//#NT#2009/04/29#Hideo Lin -end
MEDIA_DSP_1STV  gDsp1stV;//2011/08/11 Meg
void AppInit_SlideSpeedTable(PLAY_OBJ *PlayObj)
{
}

void AppinitMedia_Decode1stVideo(MEDIA_PLAY_1ST_INFO *pMediaInfo)
{
    MEDIA_GET1STV   get1stV;
    MEDIA_FIRST_INFO media1stV;
    VIDEO_DECODER   *pDecobj;
    CONTAINERPARSER *ptrContainer;

    get1stV.addr = pMediaInfo->fileStartAddr;
    get1stV.size = pMediaInfo->freeSize;
    get1stV.validType = MEDIA_FILEFORMAT_AVI;
    MediaPlay_Show1stVideo(&get1stV, 0, &media1stV);
    if (media1stV.type == 0)
    {
        debug_err(("not valid filetype\r\n"));
        pMediaInfo->imageWidth = 0;
        pMediaInfo->imageHeight= 0;
        return;
    }
    //#NT#2010/08/09#Meg Lin -begin
    else if (media1stV.dur == 0)
    {
        debug_err(("moov atom error\r\n"));
        pMediaInfo->imageWidth = 0;
        pMediaInfo->imageHeight= 0;
        return;
    }
    //#NT#2010/08/09#Meg Lin -end

#if 0
    if (media1stV.type == MEDIA_PARSEINDEX_MP3)
    {
        pMediaInfo->filetype = media1stV.type;
        pMediaInfo->videoCodec = 0xff;


        MediaPlay_SetDecode1stParam(pMediaInfo);
        return;
    }
#endif
    //#NT#2011/08/11#Meg Lin -begin
    //use global gDsp1stV
    gDsp1stV.addr = (get1stV.addr+ media1stV.pos);
    gDsp1stV.size = media1stV.size;
    gDsp1stV.decodeBuf = pMediaInfo->yAddr;
    gDsp1stV.decodeBufSize = 1280*720*4+0x1000;//2011/06/15 Meg Lin *4 for 422 720p
    debug_msg("MediaDecode1st: BS addr=0x%lx, size=0x%lx, YUV addr=0x%lx!\r\n", get1stV.addr, media1stV.size, gDsp1stV.decodeBuf);

    ptrContainer = gFormatParser[MEDIA_PARSEINDEX_AVI].parser;
    gDsp1stV.width = media1stV.wid;
    gDsp1stV.height= media1stV.hei;
    gDsp1stV.decodeMode = MEMORY_NORMAL;//2011/04/06 Meg Lin
    pMediaInfo->videoCodec = MEDIAPLAY_VIDEO_MJPG;
    //#NT#2010/11/03#Meg Lin -begin
    pMediaInfo->userDataOffset = media1stV.udta_pos;
    pMediaInfo->totalSec = media1stV.dur;
    if (media1stV.audSR != 0)
        pMediaInfo->bAudSupport = 1;
    else
        pMediaInfo->bAudSupport = 0;
    //#NT#2010/11/03#Meg Lin -end
    pDecobj = MP_MjpgDec_getVideoDecodeObject();
    gDsp1stV.maxDecWidth = pMediaInfo->maxDecWidth;//2011/08/08 Meg Lin


    if (pDecobj->CustomizeFunc)
    {
        (pDecobj->CustomizeFunc)(MEDIAVID_CUSTOM_DECODE1ST, &gDsp1stV);


    }
    pMediaInfo->yAddr = gDsp1stV.y_Addr;
    pMediaInfo->cbAddr = gDsp1stV.cb_Addr;
    pMediaInfo->crAddr = gDsp1stV.cr_Addr;
    pMediaInfo->imageWidth= gDsp1stV.width;//media1stV.wid;//2011/08/08 Meg Lin
    pMediaInfo->imageHeight= gDsp1stV.height;//media1stV.hei;//2011/08/08 Meg Lin
    pMediaInfo->wid_lineoft= gDsp1stV.wid_lineoft;//2011/11/14 Meg Lin
    pMediaInfo->wid_lineoftuv = gDsp1stV.wid_lineoftuv;//2011/11/14 Meg Lin
    pMediaInfo->filetype = media1stV.type;
    pMediaInfo->jpgType = gDsp1stV.jpgType;
    //#NT#2011/08/11#Meg Lin -end


    MediaPlay_SetDecode1stParam(pMediaInfo);
    {
        ER rv;
        UINT32 temp, temp2;
        rv = MediaPlay_GetFileInfo(MEDIA_INFO_DURATION, &temp);
        if (rv == E_OK)
        {
            debug_msg("dur = %d sec\r\n", temp);
        }
        rv = MediaPlay_GetFileInfo(MEDIA_INFO_WIDTH, &temp);
        if (rv == E_OK)
        {
            rv = MediaPlay_GetFileInfo(MEDIA_INFO_HEIGHT, &temp2);
            debug_msg("video resolution %dx%d \r\n", temp, temp2);
        }
    }
}


/**
  Initialize application for Playback mode

  Initialize application for Playback mode.

  @param void
  @return void
*/
void AppInit_ModePlayback(void)
{
    UINT32              uiPoolAddr;
    PLAY_OBJ            PlayObj = {0};
//    UINT32              uiBufSize, uiNeedBufSize;
#if (_WAVSTUDIO_MODE_ == ENABLE)
    WAVSTUDIO_APPOBJ    WavObj;
    WAVSTUDIO_FMT       WavFormat;
#endif

    //#NT#2012/01/06#Philex Lin -begin
    // flush all event againg before entering playback mode
    Ux_FlushEvent();
    //#NT#2012/01/06#Philex Lin -end
    //#NT#2011/11/11#Hideo Lin -begin
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
            FWRotate_SetStatus(RotParam.SEL.GET_ROTATE.RotDir);
            SysGetDispRotateAddr(&uiAddr);
            uiSize = g_LCDSize.uiWidth * g_LCDSize.uiHeight;

            disprot_lock();
            pDispObj = disp_getDisplayObject(DISP_1);

            DispLyr.SEL.SET_MODE.BufFormat       = DISPBUFFORMAT_YUV420;
            DispLyr.SEL.SET_MODE.BufMode         = DISPBUFMODE_BUFFER_REPEAT;
            DispLyr.SEL.SET_MODE.BufNumber       = DISPBUFNUM_1;
            pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_MODE, &DispLyr);

            /*
            // It will not be changed; don't need to set again.
            DispLyr.SEL.SET_BUFSIZE.uiBufWidth   = g_LCDSize.uiHeight;      // rotate: w => h
            DispLyr.SEL.SET_BUFSIZE.uiBufHeight  = g_LCDSize.uiWidth;       // rotate: h => w
            DispLyr.SEL.SET_BUFSIZE.uiBufLineOfs = g_LCDSize.uiHeight;
            pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_BUFSIZE, &DispLyr);
            */

            // Should set display window size since it may be changed in preview mode (such as 16:9 image).
            DispLyr.SEL.SET_WINSIZE.uiWinWidth   = g_LCDSize.uiWinHeight;   // rotate: w => h
            DispLyr.SEL.SET_WINSIZE.uiWinHeight  = g_LCDSize.uiWinWidth;    // rotate: h => w
            DispLyr.SEL.SET_WINSIZE.iWinOfsX     = 0;
            DispLyr.SEL.SET_WINSIZE.iWinOfsY     = 0;
            pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_WINSIZE, &DispLyr);

            /*
            // It will not be changed; don't need to set again.
            RotParam.SEL.SET_ROTATE.DispID          = DISP_1;
            RotParam.SEL.SET_ROTATE.DispLayer       = DISPLAYER_VDO1;
            RotParam.SEL.SET_ROTATE.RotDir          = DISPROT_ROTATE90CCW;
            disprot_setParameter(DISPROT_OP_SET_ROTATE, &RotParam);
            */

            /*
            // It will not be changed; don't need to set again.
            RotParam.SEL.SET_BUF_FMT.DispID         = DISP_1;
            RotParam.SEL.SET_BUF_FMT.DispLayer      = DISPLAYER_VDO1;
            RotParam.SEL.SET_BUF_FMT.BufFmt         = DISPBUFFORMAT_YUV420;
            disprot_setParameter(DISPROT_OP_SET_BUF_FMT, &RotParam);
            */

            /*
            // It will not be changed; don't need to set again.
            RotParam.SEL.SET_INPUT_DIM.DispID       = DISP_1;
            RotParam.SEL.SET_INPUT_DIM.DispLayer    = DISPLAYER_VDO1;
            RotParam.SEL.SET_INPUT_DIM.uiSrcWidth   = g_LCDSize.uiWidth;
            RotParam.SEL.SET_INPUT_DIM.uiSrcHeight  = g_LCDSize.uiHeight;
            RotParam.SEL.SET_INPUT_DIM.uiSrcLoffY   = (g_LCDSize.uiWidth+3)&0xFFFFFFFC;
            RotParam.SEL.SET_INPUT_DIM.uiSrcLoffUV  = RotParam.SEL.SET_INPUT_DIM.uiSrcLoffY/2; // fixed to 420
            disprot_setParameter(DISPROT_OP_SET_INPUT_DIM, &RotParam);
            */

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

    //#NT#2011/11/11#Hideo Lin -end

    memset(&PlayObj, 0, sizeof(PLAY_OBJ));

    get_blk((VP*)&(uiPoolAddr), POOL_ID_FB);
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);

    PlayObj.uiMemoryAddr        = uiPoolAddr;
    //#NT#2011/11/11#Hideo Lin -begin
    //#NT#Modify for display rotation
    PlayObj.uiMemorySize        = END_MEM - uiPoolAddr - POOL_SIZE_RAM_DISK - SysGetDispRotateBuffSize();
    //#NT#2011/11/11#Hideo Lin -end

    PlayObj.CallBackFunc        = NULL;

#if (_WAVSTUDIO_MODE_ == ENABLE)
    PlayObj.usPlayFileFormat    = PLAYFMT_JPG | PLAYFMT_WAV | PLAYFMT_AVI;
#else
    PlayObj.usPlayFileFormat    = PLAYFMT_JPG | PLAYFMT_AVI;
#endif
    //PlayObj.Decode1stMediaFunc  = AppinitMedia_Decode1stVideo;   //TODO220: For new media play module
    PlayObj.usPlayFileFormat    |= PLAYFMT_AVI; //2011/04/06 Meg Lin
    PlayObj.Decode1stMediaFunc  = AppinitMedia_Decode1stVideo;//2011/04/06 Meg Lin

    PlayObj.uiDeviceWidth       = g_LCDSize.uiWinWidth;
    PlayObj.uiDeviceHeight      = g_LCDSize.uiWinHeight;
    PlayObj.uiDisplayFBWidth    = g_LCDSize.uiWidth;
    PlayObj.uiDisplayFBHeight   = g_LCDSize.uiHeight;
    PlayObj.ui1stPlayMode       = PLAYMODE_UNKNOWN;
    //#NT#2009/04/29#Hideo Lin -begin
    //#NT#Should use different slide effect steps for TV/LCD output
    //PlayObj.uiMoveImgSpeedTable = SlideSpeedTable;
    {
        UINT32  i, step;
        if (KeyScan_IsTVPlugIn())
        {
            step = sizeof(SlideSpeedTableTV);
            for (i = 0; i < step; i++)
                SlideSpeedTableTV[i] = 0x00;
            SlideEffect_SetStep(step);
            PlayObj.uiMoveImgSpeedTable = SlideSpeedTableTV;
        }
        else
        {
            step = sizeof(SlideSpeedTableLCD);
            for (i = 0; i < step; i++)
                SlideSpeedTableLCD[i] = 0x00;
            SlideEffect_SetStep(step);
            PlayObj.uiMoveImgSpeedTable = SlideSpeedTableLCD;
        }
    }
    //#NT#2009/04/29#Hideo Lin -end
    PlayObj.Orientation         = PLAY_HORIZONTAL;
    PlayObj.Flags               = PB_ENABLE_TRANSFROM;
#if (BG_MOVIE)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BG_IN_MOVIE;
#endif
#if (BG_THUMBNAIL)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BACKGROUND_IN_THUMB;
#endif
#if (BG_BADFILE)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BG_IN_BADFILE;
#endif
    //#NT#2010/08/27#Ben Wang -begin
    //#Place Hidden image after primary image
    PlayObj.Flags              |= PB_ENABLE_SPEEDUP_SCREENNAIL;
    //#NT#2010/08/27#Ben Wang -end

#if 0   //Useless
    if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_MOVIE)
    {// Prepare image for movie background on FB2
        AppInit_ModePlayback_BGOnV2();
        PlayObj.uiMovieBGAddrY  = g_uiMovieBGAddrY;
        PlayObj.uiMovieBGAddrCb = g_uiMovieBGAddrCb;
        PlayObj.uiMovieBGAddrCr = g_uiMovieBGAddrCr;
    }

    if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_BADFILE)
    {// Prepare image for bad file background on FB2
        AppInit_ModePlayback_BGOnFB2();
        PlayObj.uiBadFileBGAddrY  = g_uiBadFileBGAddrY;
        PlayObj.uiBadFileBGAddrCb = g_uiBadFileBGAddrCb;
        PlayObj.uiBadFileBGAddrCr = g_uiBadFileBGAddrCr;
    }
#endif

    if (KeyScan_IsHDMIPlugIn() == TRUE)
    {
        // slide show effect will be OK on HDMI if enable PB_ENABLE_SLIDE_SHOW_IN_SMALL_BUF,
        // but the image quality will be poor since it uses 1/2 width and 1/2 height to scale up
        //PlayObj.Flags              |= PB_ENABLE_SLIDE_SHOW_IN_SMALL_BUF;
        //#NT#2011/08/04#KS Hung -begin
        //decode primary image for display quality and ratio while pluging HDMI TV
        PlayObj.Flags                 |= PB_ENABLE_FORCE_TO_DECODE_PRIMARY;
        //#NT#2011/08/04#KS Hung -end
        PlayObj.uiPanelWidth        = g_LCDSize.uiWinWidth;
        PlayObj.uiPanelHeight       = g_LCDSize.uiWinHeight;
    }
    else if (KeyScan_IsTVPlugIn() == TRUE)
    {
        // fixed to 4:3 for TV display
        PlayObj.uiPanelWidth        = 320;
        PlayObj.uiPanelHeight       = 240;
    }
    else
    {
        // for 4:3 LCD panel
        PlayObj.uiPanelWidth        = 320;//PB_PANEL_NO_SCALING;
        PlayObj.uiPanelHeight       = 240;//PB_PANEL_NO_SCALING;
        //#NT#2011/09/19#KS Hung -begin
        //#NT#For LCD panel with 16:9 ratio (but 4:3 pixel resolution)
        #if (DISPLAY_169HD == DISPLAY_169HD_YES)
        PlayObj.uiPanelWidth           = PB_PANEL_NO_SCALING;
        PlayObj.uiPanelHeight          = PB_PANEL_NO_SCALING;
        #endif
        //#NT#2011/09/19#KS Hung -end
    }

    PlayObj.uiThumbShowImageMode    = PB_SHOW_THUMB_IMAGE_ONE_BY_ONE;//PB_SHOW_THUMB_IMAGE_IN_THE_SAME_TIME;
    PlayObj.bEnableAutoRotate       = TRUE;
    PlayObj.ThumbOnlyRect.uiLeft    = PB_THUMB_ONLY_DEFAULT_SETTING;
    PlayObj.ThumbOnlyRect.uiTop     = PB_THUMB_ONLY_DEFAULT_SETTING;
    PlayObj.ThumbOnlyRect.uiWidth   = PB_THUMB_ONLY_DEFAULT_SETTING;
    PlayObj.ThumbOnlyRect.uiHeight  = PB_THUMB_ONLY_DEFAULT_SETTING;
    PlayObj.bDisplayLastImg         = TRUE;//FALSE;

    // Scan DCF again
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysCloseActFile();
    FilesysScanDCFObj();


    // Open playback task
    if (PB_Open(&PlayObj) != E_OK)
    {
        debug_err(("Error open playback task\r\n"));
    }

    PB_SetAviUserDataIdentify((UINT8 *)SysGetSocialNetworkData());

    // Do first video frame object initial
    PB_1stVideoFrameInit();
#if (BG_MOVIE)
    // Set the position and size of first video frame
    if (KeyScan_IsTVPlugIn() == TRUE)
    {
        if (SysGetFlag(FL_TV_MODE) == TV_MODE_NTSC)
        {
            PB_Set1stVideoFrame(VIDFRAME_POS_X_NTSC, VIDFRAME_POS_Y_NTSC,
                                VIDFRAME_WIDTH_NTSC, VIDFRAME_HEIGHT_NTSC);
        }
        else
        {
            PB_Set1stVideoFrame(VIDFRAME_POS_X_PAL, VIDFRAME_POS_Y_PAL,
                                VIDFRAME_WIDTH_PAL, VIDFRAME_HEIGHT_PAL);
        }
    }
    else
    {
        PB_Set1stVideoFrame(VIDFRAME_POS_X, VIDFRAME_POS_Y,
                            VIDFRAME_WIDTH, VIDFRAME_HEIGHT);
    }
#endif

//#NT#2008/11/20#Hideo Lin -begin
//#NT#Use _WAVSTUDIO_MODE_ to control WAV settings
#if (_WAVSTUDIO_MODE_ == ENABLE)
    //#NT#2008/04/28#Chris Hsu -begin
    //#NT#Add sample code for bits per sample and user data
    WavFormat.AudioChannel      = AUDIO_CH_RIGHT;
    WavFormat.AudioSamplingRate = AUDIO_SR_11025;
#if (_VM_FORMAT_ == _VM_FORMAT_PCM8_)
    // PCM 8 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_8;
#elif (_VM_FORMAT_ == _VM_FORMAT_PCM16_)
    // PCM 16 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_16;
#elif (_VM_FORMAT_ == _VM_FORMAT_ADPCM_)
    // ADPCM
    WavFormat.AudioCodec        = AUDIOCODEC_IMA_ADPCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_ADPCM;
#elif (_VM_FORMAT_ == _VM_FORMAT_ASFPCM_)
    // ASF PCM
    #if (WAVFILE_ASF_ENABLE == ENABLE)
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.FileFormat        = WAVFILE_ASF;
    #endif
#endif
    WavStudio_SetFormat(&WavFormat);

    // Set maximum recording time
    WavStudio_SetMaxRecTime(10);

    // Set user data
#if (_WAV_SINGLE_USERDATA_ == ENABLE)
    WavStudio_SetUserData((UINT32)&g_uiWAVHeaderUserData, (UINT32)sizeof(g_uiWAVHeaderUserData));
#else
    WavStudio_SetUserData(0, 0);
#endif

    // Set WAV application object
    WavObj.uiMemoryAddr         = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_WAV;
    WavObj.uiMemorySize         = POOL_SIZE_WAV;
    //#NT#2009/03/24#Hideo Lin -begin
    //#NT#For wav playback in playback mode
    //WavObj.WavStudioCB        = (WAVSTUDIO_CB)FlowCommon_WAVStudioCB;
    WavObj.WavStudioCB          = (WAVSTUDIO_CB)FlowPB_WAVStudioCB;
    //#NT#2009/03/24#Hideo Lin -end
    //#NT#2008/04/28#Chris Hsu -end

    // Open Wav studio task
    if (WavStudio_Open(&WavObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for wav studio task\r\n"));
    }
#endif
//#NT#2008/11/20#Hideo Lin -end
}

//@}
