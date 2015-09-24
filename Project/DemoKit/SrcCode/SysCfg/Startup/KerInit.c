/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       KerInit.c
    @ingroup

    @brief      kernel initialization for user defined task, event...
                kernel initialization for user defined task, event...

    @note       Nothing.
*/

#include "Kernel.h"
#include "SysCfg.h"


extern  TASKINFO                user_systasktabl[];
extern  SYSTEM_EVF_TABLE        user_sysevftabl[];
extern  SYSTEM_SEM_TABLE        user_syssemtabl[];
extern  SYSTEM_MBX_TABLE        user_sysmbxtabl[];
extern  SYSTEM_MEMPOOL_TABLE    user_sysfpltabl[];

//======================================================================
// Module Task Stack Memory Allocation
//======================================================================
// Playback
#if(_PLAYBACK_MODE_ == ENABLE)
__align(4) UCHAR pucStkPlayback[STKSIZE_PLAYBACK];
#endif

// IQ
#if(_IQ_MODE_ == ENABLE)
__align(4) UCHAR pucStk1stPhoto[STKSIZE_1STPHOTO];
__align(4) UCHAR pucStk2ndPhoto[STKSIZE_2NDPHOTO];
__align(4) UCHAR pucStk3rdPhoto[STKSIZE_3RDPHOTO];
__align(4) UCHAR pucStkImgSlice[STKSIZE_IMGSLICETSK];
__align(4) UCHAR pucStkSieCap[STKSIZE_SIECAPTSK];
__align(4) UCHAR pucStkDisplayPhoto[STKSIZE_DISPLAYPHOTO];
__align(4) UCHAR pucStkLensCtrlTask[STKSIZE_LENSCTRL];
__align(4) UCHAR pucStkAETask[STKSIZE_AE];
__align(4) UCHAR pucStkAAWBTask[STKSIZE_AWB];
//#NT#2011/10/18#Jarkko Chang -begin
//#NT#add for AF task
__align(4) UCHAR pucStkAF[STKSIZE_AFTSK];
__align(4) UCHAR pucStkAF2[STKSIZE_AFTSK2];
//#NT#2011/10/18#Jarkko Chang -end
#endif

// USB
__align(4) UCHAR pucStkUsbCtrl[STKSIZE_USBCTRL];
__align(4) UCHAR pucStkUsbSetup[STKSIZE_USBSETUP];
__align(4) UCHAR pucStkUsbStrgVnd[STKSIZE_USBSTRGVND];
__align(4) UCHAR pucStkUsbPcCamVideo[STKSIZE_USBPCCAMVIDEO];
__align(4) UCHAR pucStkUsbSICDVnd[STKSIZE_USBSICDVND];
__align(4) UCHAR pucStkUsbSICDEP3[STKSIZE_USBSICDEP3];

// AVI
__align(4) UCHAR pucStkAviRecManager[STKSIZE_AVIRECMANAGER];
__align(4) UCHAR pucStkRecVideo[STKSIZE_RECVIDEO];

// MP3
#if(_MP3_MODE_ == ENABLE)
__align(4) UCHAR pucStkMP3Play[STKSIZE_MP3PLAY];
__align(4) UCHAR pucStkMP3PlayFileRead[STKSIZE_MP3PLAYFILESYS];
#endif

// WAV
__align(4) UCHAR pucStkWavStudio[STKSIZE_WAVSTUDIO];

// Media Framework
__align(4) UCHAR pucStkMediaPlay[STKSIZE_MEDIAPLAY];
__align(4) UCHAR pucStkMediaVideoPlay[STKSIZE_MEDIAVIDEOPLAY];
__align(4) UCHAR pucStkMediaAudioPlay[STKSIZE_MEDIAAUDIOPLAY];

#if(_MEDIA_REC_ == _APPMEDIAREC_)
__align(4) UCHAR pucStkMediaRec[STKSIZE_MEDIAREC];
__align(4) UCHAR pucStkMediaVideoRec[STKSIZE_MEDIAVIDEOREC];
__align(4) UCHAR pucStkMediaAudioEnc[STKSIZE_MEDIAAUDIOENC];
__align(4) UCHAR pucStkMediaRecFS[STKSIZE_MEDIARECFS];
__align(4) UCHAR pucStkMediaRecCmd[STKSIZE_MEDIARECCMD];
#endif

// Picture In Movie
#if (_MOVIE_PIM_MODE_ == ENABLE)
__align(4) UCHAR pucStkRawEnc[STKSIZE_RAWENC];
#endif

// GPS
#if(_GPS_RECEIVER_ == ENABLE)
__align(4) UCHAR pucStkGPSReceive[STKSIZE_GPSRECEIVE];
#endif

// Filesystem background task
__align(4) UCHAR pucStkFsBackground[STKSIZE_FSBACKGROUND];

//#NT#2011/09/14#Klins Chen -begin
//Disp rotate
#if(_DISPROTATE_MODE_ == ENABLE)
__align(4) UCHAR pucStkDispRotate[STKSIZE_DISPROTATE];
#endif
//#NT#2011/09/14#Klins Chen -end

//#NT#2011/10/26#Philex Lin -begin
__align(4) UCHAR pucStkFW_ROTATE[STKSIZE_FW_ROTATE];
//#NT#2011/10/26#Philex Lin -end


//======================================================================
// Module Task Configration Infomation
//======================================================================
// Playback
#if(_PLAYBACK_MODE_ == ENABLE)
extern void PlaybackTsk(void);
TASKINFO PlaybackAPPCfg[1];
#endif

// IQ
#if(_IQ_MODE_ == ENABLE)
extern void Photo1stTsk(void);
extern void Photo2ndTsk(void);
extern void Photo3rdTsk(void);
extern void ImgSliceTsk(void);
extern void SieCapTsk(void);
extern void PhotoDisplayTsk(void);
extern void LensCtrlTsk(void);
extern void AETsk(void);
extern void AWBTsk(void);
//#NT#2011/10/18#Jarkko Chang -begin
//#NT#add for AF task
TASKINFO IQTaskAPPCfg[11];
extern void AFTsk(void);
extern void AFTsk2(void);
//#NT#2011/10/18#Jarkko Chang -end
#endif

// USB
extern void USBCtlStartTsk(void);
extern void USBSetupHalTsk(void);
extern void USBMSDC_StrgTsk(void);
extern void USBPccVideoTsk(void);
extern void UsicdTsk(void);
extern void UsicdEP3Tsk(void);
TASKINFO USBSubsystemCfg[6];

// AVI
extern void AviRecManagerTsk(void);
extern void RecVideoTsk(void);
TASKINFO AVIRecDecCfg[2];

// MP3
#if(_MP3_MODE_ == ENABLE)
extern void MP3PlayTsk(void);
extern void MP3PlayFileReadTsk(void);
TASKINFO MP3AppCfg[2];
#endif

// WAV
extern void WavStudioTsk(void);
TASKINFO WavAppCfg;

//Media Framework
extern void MediaPlayTsk(void);
extern void MediaVideoPlayTsk(void);
extern void MediaAudioPlayTsk(void);

#if(_MEDIA_REC_ == _APPMEDIAREC_)
extern void MediaRecTsk(void);
extern void MediaVideoRecTsk(void);
extern void MediaAudioEncTsk(void);
extern void MediaRecFSTsk(void);
extern void MediaRecCmdTsk(void);
TASKINFO MediaFrameworkCfg[8];
#else
TASKINFO MediaFrameworkCfg[3];
#endif

// Picture In Movie
#if (_MOVIE_PIM_MODE_ == ENABLE)
extern void RawEncTsk(void);
TASKINFO RawEncCfg[1];
#endif

// GPS
#if(_GPS_RECEIVER_ == ENABLE)
extern void GPSRecTsk(void);
TASKINFO GPSReceiverAppCfg;
#endif

// Filesystem background task
extern void FsBackground_Task(void);
TASKINFO FsBackgroundCfg;

//#NT#2011/09/14#Klins Chen -begin
// Display Rotation
#if(_DISPROTATE_MODE_ == ENABLE)
extern void disprot_task(void);
TASKINFO DispRotateCfg;
#endif
//#NT#2011/09/14#Klins Chen -end

//#NT#2011/10/26#Philex Lin -begin
extern void FWRotate_Tsk(void);
TASKINFO FWRotateCfg;
//#NT#2011/10/26#Philex Lin -end


//======================================================================
// Init Module Configs
//======================================================================
void KerInitModules(void)
{
    //======================================================================
    // Module Configrations
    //======================================================================
    // Playback
    #if(_PLAYBACK_MODE_ == ENABLE)
    PlaybackAPPCfg[0].Task.tskid       = PLAYBAKTSK_ID;
    PlaybackAPPCfg[0].Task.itskpri     = PRI_PLAYBACK;
    PlaybackAPPCfg[0].Task.stksz       = STKSIZE_PLAYBACK;
    PlaybackAPPCfg[0].Task.istack      = pucStkPlayback;
    PlaybackAPPCfg[0].Task.task        = PlaybackTsk;
    PlaybackAPPCfg[0].TaskName         = "PlaybackTsk()";
    OSCommon_CfgModuleTask(&PlaybackAPPCfg[0]);
    #endif

    // IQ
    #if(_IQ_MODE_ == ENABLE)
    IQTaskAPPCfg[0].Task.tskid       = PHOTO1STTSK_ID;
    IQTaskAPPCfg[0].Task.itskpri     = PRI_1STPHOTO;
    IQTaskAPPCfg[0].Task.stksz       = STKSIZE_1STPHOTO;
    IQTaskAPPCfg[0].Task.istack      = pucStk1stPhoto;
    IQTaskAPPCfg[0].Task.task        = Photo1stTsk;
    IQTaskAPPCfg[0].TaskName         = "Photo1stTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[0]));

    IQTaskAPPCfg[1].Task.tskid       = PHOTO2NDTSK_ID;
    IQTaskAPPCfg[1].Task.itskpri     = PRI_2NDPHOTO;
    IQTaskAPPCfg[1].Task.stksz       = STKSIZE_2NDPHOTO;
    IQTaskAPPCfg[1].Task.istack      = pucStk2ndPhoto;
    IQTaskAPPCfg[1].Task.task        = Photo2ndTsk;
    IQTaskAPPCfg[1].TaskName         = "Photo2ndTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[1]));

    IQTaskAPPCfg[2].Task.tskid       = PHOTO3RDTSK_ID;
    IQTaskAPPCfg[2].Task.itskpri     = PRI_3RDPHOTO;
    IQTaskAPPCfg[2].Task.stksz       = STKSIZE_3RDPHOTO;
    IQTaskAPPCfg[2].Task.istack      = pucStk3rdPhoto;
    IQTaskAPPCfg[2].Task.task        = Photo3rdTsk;
    IQTaskAPPCfg[2].TaskName         = "Photo3rdTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[2]));

    IQTaskAPPCfg[3].Task.tskid       = IMGSLICETSK_ID;
    IQTaskAPPCfg[3].Task.itskpri     = PRI_IMGSLICE;
    IQTaskAPPCfg[3].Task.stksz       = STKSIZE_IMGSLICETSK;
    IQTaskAPPCfg[3].Task.istack      = pucStkImgSlice;
    IQTaskAPPCfg[3].Task.task        = ImgSliceTsk;
    IQTaskAPPCfg[3].TaskName         = "ImgSliceTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[3]));

    IQTaskAPPCfg[4].Task.tskid       = SIECAPTSK_ID;
    IQTaskAPPCfg[4].Task.itskpri     = PRI_SIECAP;
    IQTaskAPPCfg[4].Task.stksz       = STKSIZE_SIECAPTSK;
    IQTaskAPPCfg[4].Task.istack      = pucStkSieCap;
    IQTaskAPPCfg[4].Task.task        = SieCapTsk;
    IQTaskAPPCfg[4].TaskName         = "SieCapTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[4]));

    IQTaskAPPCfg[5].Task.tskid       = PHOTODISPLAYTSK_ID;
    IQTaskAPPCfg[5].Task.itskpri     = PRI_DISPLAYPHOTO;
    IQTaskAPPCfg[5].Task.stksz       = STKSIZE_DISPLAYPHOTO;
    IQTaskAPPCfg[5].Task.istack      = pucStkDisplayPhoto;
    IQTaskAPPCfg[5].Task.task        = PhotoDisplayTsk;
    IQTaskAPPCfg[5].TaskName         = "PhotoDisplayTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[5]));

    IQTaskAPPCfg[6].Task.tskid       = LENSCTRLTSK_ID;
    IQTaskAPPCfg[6].Task.itskpri     = PRI_LENSCTRL;
    IQTaskAPPCfg[6].Task.stksz       = STKSIZE_LENSCTRL;
    IQTaskAPPCfg[6].Task.istack      = pucStkLensCtrlTask;
    IQTaskAPPCfg[6].Task.task        = LensCtrlTsk;
    IQTaskAPPCfg[6].TaskName         = "LensCtrlTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[6]));

    IQTaskAPPCfg[7].Task.tskid       = AETSK_ID;
    IQTaskAPPCfg[7].Task.itskpri     = PRI_AE;
    IQTaskAPPCfg[7].Task.stksz       = STKSIZE_AE;
    IQTaskAPPCfg[7].Task.istack      = pucStkAETask;
    IQTaskAPPCfg[7].Task.task        = AETsk;
    IQTaskAPPCfg[7].TaskName         = "AETsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[7]));

    IQTaskAPPCfg[8].Task.tskid       = AWBTSK_ID;
    IQTaskAPPCfg[8].Task.itskpri     = PRI_AWB;
    IQTaskAPPCfg[8].Task.stksz       = STKSIZE_AWB;
    IQTaskAPPCfg[8].Task.istack      = pucStkAAWBTask;
    IQTaskAPPCfg[8].Task.task        = AWBTsk;
    IQTaskAPPCfg[8].TaskName         = "AWBTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[8]));

    //#NT#2011/04/20#Chris Chung -begin
    //#NT#add for AF task
    IQTaskAPPCfg[9].Task.tskid       = AFTSK_ID;
    IQTaskAPPCfg[9].Task.itskpri     = PRI_AFTSK;
    IQTaskAPPCfg[9].Task.stksz       = STKSIZE_AFTSK;
    IQTaskAPPCfg[9].Task.istack      = pucStkAF;
    IQTaskAPPCfg[9].Task.task        = AFTsk;
    IQTaskAPPCfg[9].TaskName         = "AFTsk()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[9]));

    IQTaskAPPCfg[10].Task.tskid       = AFTSK2_ID;
    IQTaskAPPCfg[10].Task.itskpri     = PRI_AFTSK2;
    IQTaskAPPCfg[10].Task.stksz       = STKSIZE_AFTSK2;
    IQTaskAPPCfg[10].Task.istack      = pucStkAF2;
    IQTaskAPPCfg[10].Task.task        = AFTsk2;
    IQTaskAPPCfg[10].TaskName         = "AFTsk2()";
    OSCommon_CfgModuleTask(&(IQTaskAPPCfg[10]));
    //#NT#2011/04/20#Chris Chung -end
    #endif

    // USB
    USBSubsystemCfg[0].Task.tskid       = USBCTRLTSK_ID;
    USBSubsystemCfg[0].Task.itskpri     = PRI_USBCTRL;
    USBSubsystemCfg[0].Task.stksz       = STKSIZE_USBCTRL;
    USBSubsystemCfg[0].Task.istack      = pucStkUsbCtrl;
    USBSubsystemCfg[0].Task.task        = USBCtlStartTsk;
    USBSubsystemCfg[0].TaskName         = "USBCtlStartTsk()";
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[0]));

    USBSubsystemCfg[1].Task.tskid       = USBSETUPTSK_ID;
    USBSubsystemCfg[1].Task.itskpri     = PRI_USBSETUP;
    USBSubsystemCfg[1].Task.stksz       = STKSIZE_USBSETUP;
    USBSubsystemCfg[1].Task.istack      = pucStkUsbSetup;
    USBSubsystemCfg[1].Task.task        = USBSetupHalTsk;
    USBSubsystemCfg[1].TaskName         = "USBSetupHalTsk()";
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[1]));

    USBSubsystemCfg[2].Task.tskid       = USBSTRGTSK_ID;
    USBSubsystemCfg[2].Task.itskpri     = PRI_USBSTRG;
    USBSubsystemCfg[2].Task.stksz       = STKSIZE_USBSTRGVND;
    USBSubsystemCfg[2].Task.istack      = pucStkUsbStrgVnd;
    USBSubsystemCfg[2].Task.task        = USBMSDC_StrgTsk;
    USBSubsystemCfg[2].TaskName         = "USBMSDC_StrgTsk()";
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[2]));

    USBSubsystemCfg[3].Task.tskid       = USBPCCVIDEOTSK_ID;
    USBSubsystemCfg[3].Task.itskpri     = PRI_USBPCCVIDEO;
    USBSubsystemCfg[3].Task.stksz       = STKSIZE_USBPCCAMVIDEO;
    USBSubsystemCfg[3].Task.istack      = pucStkUsbPcCamVideo;
    USBSubsystemCfg[3].Task.task        = USBPccVideoTsk;
    USBSubsystemCfg[3].TaskName         = "USBPccVideoTsk()";
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[3]));

    USBSubsystemCfg[4].Task.tskid       = USBSICDTSK_ID;
    USBSubsystemCfg[4].Task.itskpri     = PRI_USBSICD;
    USBSubsystemCfg[4].Task.stksz       = STKSIZE_USBSICDVND;
    USBSubsystemCfg[4].Task.istack      = pucStkUsbSICDVnd;
    USBSubsystemCfg[4].Task.task        = UsicdTsk;
    USBSubsystemCfg[4].TaskName         = "USBSICDTsk()";
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[4]));

    USBSubsystemCfg[5].Task.tskid       = USBSICDEP3TSK_ID;
    USBSubsystemCfg[5].Task.itskpri     = PRI_USBSICDEP3;
    USBSubsystemCfg[5].Task.stksz       = STKSIZE_USBSICDEP3;
    USBSubsystemCfg[5].Task.istack      = pucStkUsbSICDEP3;
    USBSubsystemCfg[5].Task.task        = UsicdEP3Tsk;
    USBSubsystemCfg[5].TaskName         = "USBSICDEP3Tsk()";
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[5]));

    // AVI
    AVIRecDecCfg[0].Task.tskid       = AVIRECMANAGERTSK_ID;
    AVIRecDecCfg[0].Task.itskpri     = PRI_AVIRECMANAGER;
    AVIRecDecCfg[0].Task.stksz       = STKSIZE_AVIRECMANAGER;
    AVIRecDecCfg[0].Task.istack      = pucStkAviRecManager;
    AVIRecDecCfg[0].Task.task        = AviRecManagerTsk;
    AVIRecDecCfg[0].TaskName         = "AviRecManagerTsk()";
    OSCommon_CfgModuleTask(&(AVIRecDecCfg[0]));

    AVIRecDecCfg[1].Task.tskid       = RECVIDEOTSK_ID;
    AVIRecDecCfg[1].Task.itskpri     = PRI_RECVIDEO;
    AVIRecDecCfg[1].Task.stksz       = STKSIZE_RECVIDEO;
    AVIRecDecCfg[1].Task.istack      = pucStkRecVideo;
    AVIRecDecCfg[1].Task.task        = RecVideoTsk;
    AVIRecDecCfg[1].TaskName         = "RecVideoTsk()";
    OSCommon_CfgModuleTask(&(AVIRecDecCfg[1]));

    // Media Play Framework
    MediaFrameworkCfg[0].Task.tskid       = MEDIAPLAYTSK_ID;
    MediaFrameworkCfg[0].Task.itskpri     = PRI_MEDIAPLAY;
    MediaFrameworkCfg[0].Task.stksz       = STKSIZE_MEDIAPLAY;
    MediaFrameworkCfg[0].Task.istack      = pucStkMediaPlay;
    MediaFrameworkCfg[0].Task.task        = MediaPlayTsk;
    MediaFrameworkCfg[0].TaskName         = "MediaPlayTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[0]));

    MediaFrameworkCfg[1].Task.tskid       = MEDIAVIDEOPLAYTSK_ID;
    MediaFrameworkCfg[1].Task.itskpri     = PRI_MEDIAVIDEOPLAY;
    MediaFrameworkCfg[1].Task.stksz       = STKSIZE_MEDIAVIDEOPLAY;
    MediaFrameworkCfg[1].Task.istack      = pucStkMediaVideoPlay;
    MediaFrameworkCfg[1].Task.task        = MediaVideoPlayTsk;
    MediaFrameworkCfg[1].TaskName         = "MediaVideoPlayTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[1]));

    MediaFrameworkCfg[2].Task.tskid       = MEDIAAUDIOPLAYTSK_ID;
    MediaFrameworkCfg[2].Task.itskpri     = PRI_MEDIAAUDIOPLAY;
    MediaFrameworkCfg[2].Task.stksz       = STKSIZE_MEDIAAUDIOPLAY;
    MediaFrameworkCfg[2].Task.istack      = pucStkMediaAudioPlay;
    MediaFrameworkCfg[2].Task.task        = MediaAudioPlayTsk;
    MediaFrameworkCfg[2].TaskName         = "MediaAudioPlayTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[2]));

#if(_MEDIA_REC_ == _APPMEDIAREC_)
    // Media Rec Framework
    MediaFrameworkCfg[3].Task.tskid       = MEDIARECTSK_ID;
    MediaFrameworkCfg[3].Task.itskpri     = PRI_MEDIAREC;
    MediaFrameworkCfg[3].Task.stksz       = STKSIZE_MEDIAREC;
    MediaFrameworkCfg[3].Task.istack      = pucStkMediaRec;
    MediaFrameworkCfg[3].Task.task        = MediaRecTsk;
    MediaFrameworkCfg[3].TaskName         = "MediaRecTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[3]));

    MediaFrameworkCfg[4].Task.tskid       = MEDIAVIDEORECTSK_ID;
    MediaFrameworkCfg[4].Task.itskpri     = PRI_MEDIAVIDEOREC;
    MediaFrameworkCfg[4].Task.stksz       = STKSIZE_MEDIAVIDEOREC;
    MediaFrameworkCfg[4].Task.istack      = pucStkMediaVideoRec;
    MediaFrameworkCfg[4].Task.task        = MediaVideoRecTsk;
    MediaFrameworkCfg[4].TaskName         = "MediaVideoRecTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[4]));

    MediaFrameworkCfg[5].Task.tskid       = MEDIAAUDIOENCTSK_ID;
    MediaFrameworkCfg[5].Task.itskpri     = PRI_MEDIAAUDIOENC;
    MediaFrameworkCfg[5].Task.stksz       = STKSIZE_MEDIAAUDIOENC;
    MediaFrameworkCfg[5].Task.istack      = pucStkMediaAudioEnc;
    MediaFrameworkCfg[5].Task.task        = MediaAudioEncTsk;
    MediaFrameworkCfg[5].TaskName         = "MediaAudioEncTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[5]));

    MediaFrameworkCfg[6].Task.tskid       = MEDIARECFSTSK_ID;
    MediaFrameworkCfg[6].Task.itskpri     = PRI_MEDIARECFS;
    MediaFrameworkCfg[6].Task.stksz       = STKSIZE_MEDIARECFS;
    MediaFrameworkCfg[6].Task.istack      = pucStkMediaRecFS;
    MediaFrameworkCfg[6].Task.task        = MediaRecFSTsk;
    MediaFrameworkCfg[6].TaskName         = "MediaRecFSTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[6]));

    MediaFrameworkCfg[7].Task.tskid       = MEDIARECCMDTSK_ID;
    MediaFrameworkCfg[7].Task.itskpri     = PRI_MEDIARECCMD;
    MediaFrameworkCfg[7].Task.stksz       = STKSIZE_MEDIARECCMD;
    MediaFrameworkCfg[7].Task.istack      = pucStkMediaRecCmd;
    MediaFrameworkCfg[7].Task.task        = MediaRecCmdTsk;
    MediaFrameworkCfg[7].TaskName         = "MediaRecCmdTsk()";
    OSCommon_CfgModuleTask(&(MediaFrameworkCfg[7]));
#endif

    // Picture In Movie
    #if (_MOVIE_PIM_MODE_ == ENABLE)
    RawEncCfg[0].Task.tskid       = RAWENC_ID;
    RawEncCfg[0].Task.itskpri     = PRI_RAWENC;
    RawEncCfg[0].Task.stksz       = STKSIZE_RAWENC;
    RawEncCfg[0].Task.istack      = pucStkRawEnc;
    RawEncCfg[0].Task.task        = RawEncTsk;
    RawEncCfg[0].TaskName         = "RawEncTsk()";
    OSCommon_CfgModuleTask(&(RawEncCfg[0]));
    #endif

    // MP3
    #if(_MP3_MODE_ == ENABLE)
    MP3AppCfg[0].Task.tskid       = MP3PLAYTSK_ID;
    MP3AppCfg[0].Task.itskpri     = PRI_MP3PLAY;
    MP3AppCfg[0].Task.stksz       = STKSIZE_MP3PLAY;
    MP3AppCfg[0].Task.istack      = pucStkMP3Play;
    MP3AppCfg[0].Task.task        = MP3PlayTsk;
    MP3AppCfg[0].TaskName         = "MP3PlayTsk()";
    OSCommon_CfgModuleTask(&(MP3AppCfg[0]));

    MP3AppCfg[1].Task.tskid       = MP3PLAYFILETSK_ID;
    MP3AppCfg[1].Task.itskpri     = PRI_MP3PLAYFILE;
    MP3AppCfg[1].Task.stksz       = STKSIZE_MP3PLAYFILESYS;
    MP3AppCfg[1].Task.istack      = pucStkMP3PlayFileRead;
    MP3AppCfg[1].Task.task        = MP3PlayFileReadTsk;
    MP3AppCfg[1].TaskName         = "MP3PlayFileReadTsk()";
    OSCommon_CfgModuleTask(&(MP3AppCfg[1]));
    #endif

    // WAV
    WavAppCfg.Task.tskid       = WAVSTUDIOTSK_ID;
    WavAppCfg.Task.itskpri     = PRI_WAVSTUDIO;
    WavAppCfg.Task.stksz       = STKSIZE_WAVSTUDIO;
    WavAppCfg.Task.istack      = pucStkWavStudio;
    WavAppCfg.Task.task        = WavStudioTsk;
    WavAppCfg.TaskName         = "WavStudioTsk()";
    OSCommon_CfgModuleTask(&(WavAppCfg));

    // GPS
#if(_GPS_RECEIVER_ == ENABLE)
    GPSReceiverAppCfg.Task.tskid       = GPSRECEIVE_ID;
    GPSReceiverAppCfg.Task.itskpri     = PRI_GPSRECEIVE;
    GPSReceiverAppCfg.Task.stksz       = STKSIZE_GPSRECEIVE;
    GPSReceiverAppCfg.Task.istack      = pucStkGPSReceive;
    GPSReceiverAppCfg.Task.task        = GPSRecTsk;
    GPSReceiverAppCfg.TaskName         = "GPSRecTsk()";
    OSCommon_CfgModuleTask(&(GPSReceiverAppCfg));
#endif

    // Filesystem background task
    FsBackgroundCfg.Task.tskid       = FSBACKGROUNDTSK_ID;
    FsBackgroundCfg.Task.itskpri     = PRI_FSBACKGROUND;
    FsBackgroundCfg.Task.stksz       = STKSIZE_FSBACKGROUND;
    FsBackgroundCfg.Task.istack      = pucStkFsBackground;
    FsBackgroundCfg.Task.task        = FsBackground_Task;
    FsBackgroundCfg.TaskName         = "FsBackground_Task()";
    OSCommon_CfgModuleTask(&(FsBackgroundCfg));

    //#NT#2011/09/14#Klins Chen -begin
    #if(_DISPROTATE_MODE_ == ENABLE)
    // DISPROTATE
    DispRotateCfg.Task.tskid       = DISPROTATE_ID;
    DispRotateCfg.Task.itskpri     = PRI_DISPROTATE;
    DispRotateCfg.Task.stksz       = STKSIZE_DISPROTATE;
    DispRotateCfg.Task.istack      = pucStkDispRotate;
    DispRotateCfg.Task.task        = disprot_task;
    DispRotateCfg.TaskName         = "DispRotateTsk()";
    OSCommon_CfgModuleTask(&(DispRotateCfg));
    #endif
    //#NT#2011/09/14#Klins Chen -end

    //#NT#2011/10/26#Philex Lin -begin
    FWRotateCfg.Task.tskid       = FW_ROTATETSK_ID;
    FWRotateCfg.Task.itskpri     = 15;//PRI_FW_ROTATE;
    FWRotateCfg.Task.stksz       = STKSIZE_FW_ROTATE;
    FWRotateCfg.Task.istack      = pucStkFW_ROTATE;
    FWRotateCfg.Task.task        = FWRotate_Tsk;
    FWRotateCfg.TaskName         = "FWRotate_Tsk()";
    OSCommon_CfgModuleTask(&(FWRotateCfg));
    //#NT#2011/10/26#Philex Lin -end

}

//======================================================================
// Init User Tasks
//======================================================================
void KerInitUser(void)
{
    UINT32  i;
    ER      er;

    //======================================================================
    // Project Level Tasks Configration
    //======================================================================
    if(USER_TSK_MAX_NUM >= guiMaxTaskCount)
    {
        debug_err(("User TSK Config Fail, Ker Tsk Cnt=%d and Cur Tsk Cnt=%d\r\n",
                    guiMaxTaskCount,USER_TSK_MAX_NUM));
        while(1);
    }
    for (i=0; i<USER_TSK_NUM; i++)
    {
        if(NULL != tasknametable[user_systasktabl[i].Task.tskid])
        {
            debug_err(("User task ID=%d duplicated\r\n",
                        user_systasktabl[i].Task.tskid));
            while(1);
        }
        if ((er = icr_tsk(&(user_systasktabl[i].Task))) != E_OK)
        {
            debug_err(("Define user task [%ld, %s] error [%ld]\r\n",
                        user_systasktabl[i].Task.tskid,
                        user_systasktabl[i].TaskName,
                        er));
            while(1);
        }
        // Set stack bottom (lower address) memory to STACK_CHECK_TAG
        // We can check the value for stack overflow situation
        if (user_systasktabl[i].Task.stksz)
        {
            *(UINT32 *)(user_systasktabl[i].Task.istack) = STACK_CHECK_TAG;
        }
        tasknametable[user_systasktabl[i].Task.tskid] = user_systasktabl[i].TaskName;
    }

    //======================================================================
    // Project Level Flags Configration
    //======================================================================
    if(USER_EVF_MAX_NUM >= guiMaxFlagCount)
    {
        debug_err(("User FLG Config Fail, Ker Flg Cnt=%d and Cur Flg Cnt=%d\r\n",
                    guiMaxFlagCount,USER_EVF_MAX_NUM));
        while(1);
    }
    for (i=0; i<USER_EVF_NUM; i++)
    {
        if(NULL != flagnametable[user_sysevftabl[i].evfid])
        {
            debug_err(("User flag ID=%d duplicated\r\n",
                        user_sysevftabl[i].evfid));
            while(1);
        }
        if ((er = icr_flg(user_sysevftabl[i].evfid)) != E_OK)
        {
            debug_err(("Define user flag [%ld, %s] error [%ld]\r\n",
                        user_sysevftabl[i].evfid,
                        user_sysevftabl[i].EventName,
                        er));
            while(1);
        }
        flagnametable[user_sysevftabl[i].evfid] = user_sysevftabl[i].EventName;
    }

    //======================================================================
    // Project Level Semaphore Configration
    //======================================================================
    if(USER_SEM_MAX_NUM >= guiMaxSemaphoreCount)
    {
        debug_err(("User SEM Config Fail, Ker Sem Cnt=%d and Cur Sem Cnt=%d\r\n",
                    guiMaxSemaphoreCount,USER_SEM_MAX_NUM));
        while(1);
    }
    for (i=0; i<USER_SEM_NUM; i++)
    {
        if(NULL != semaphorenametable[user_syssemtabl[i].Semphore.semid])
        {
            debug_err(("User SEM ID=%d duplicated\r\n",
                        user_syssemtabl[i].Semphore.semid));
            while(1);
        }
        if ((er = icr_sem(&(user_syssemtabl[i].Semphore))) != E_OK)
        {
            debug_err(("Define user semaphore [%ld, %s] error [%ld]\r\n",
                        user_syssemtabl[i].Semphore.semid,
                        user_syssemtabl[i].SemphoreName,
                        er));
            while(1);
        }
        semaphorenametable[user_syssemtabl[i].Semphore.semid] = user_syssemtabl[i].SemphoreName;
    }
    //======================================================================
    // Project Level Mailbox Configration
    //======================================================================
    if(USER_MBX_MAX_NUM >= guiMaxMailboxCount)
    {
        debug_err(("User MBX Config Fail, Ker MBX Cnt=%d and Cur MBX Cnt=%d\r\n",
                    guiMaxMailboxCount,USER_MBX_MAX_NUM));
        while(1);
    }
    for (i=0; i<USER_MBX_NUM; i++)
    {
        if(NULL != mailboxnametable[user_sysmbxtabl[i].mbxid])
        {
            debug_err(("User MBX ID=%d duplicated\r\n",
                        user_sysmbxtabl[i].mbxid));
            while(1);
        }
        if ((er = icr_mbx(user_sysmbxtabl[i].mbxid)) != E_OK)
        {
            debug_err(("Define user mailbox [%ld, %s] error [%ld]\r\n",
                        user_sysmbxtabl[i].mbxid,
                        user_sysmbxtabl[i].MailboxName,
                        er));
            while(1);
        }
        mailboxnametable[user_sysmbxtabl[i].mbxid] = user_sysmbxtabl[i].MailboxName;
    }
    //======================================================================
    // Project Level Memory Pool Configration
    //======================================================================
    if(USER_FPL_MAX_NUM >= guiMaxMempoolCount)
    {
        debug_err(("User FPL Config Fail, Ker FPL Cnt=%d and Cur FPL Cnt=%d\r\n",
                        guiMaxMempoolCount,USER_FPL_MAX_NUM));
        while(1);
    }
    for (i=0; i<USER_FPL_NUM; i++)
    {
        if(NULL != mempoolnametable[user_sysfpltabl[i].MemPool.mpfid])
        {
            debug_err(("User Mempool ID=%d duplicated\r\n",
                        user_sysfpltabl[i].MemPool.mpfid));
            while(1);
        }
//#NT#2009/10/02#Scottie -begin
//#NT#Check if memory pool size is 32 bytes alignment
        if(user_sysfpltabl[i].MemPool.blfsz & 0x1F)
        {// Need to 32 bytes align
            debug_err(("User Memory Pool [%ld, %s] size [0x%X] isn't 32 bytes alignment\r\n",
                        user_sysfpltabl[i].MemPool.mpfid,
                        user_sysfpltabl[i].MempoolName,
                        user_sysfpltabl[i].MemPool.blfsz));
            while(1);
        }
        if ((er = icr_mpf(&(user_sysfpltabl[i].MemPool))) != E_OK)
        {
//#NT#Check if memory pool size is ZERO
            if(er == E_PAR)
            {
                debug_err(("User Memory Pool [%ld, %s] size [0x%X] isn't legal\r\n",
                            user_sysfpltabl[i].MemPool.mpfid,
                            user_sysfpltabl[i].MempoolName,
                            user_sysfpltabl[i].MemPool.blfsz));
            }
            else
//#NT#2009/10/02#Scottie -end
            {
                debug_err(("Define user fixed memory pool [%ld, %s] error [%ld]\r\n",
                            user_sysfpltabl[i].MemPool.mpfid,
                            user_sysfpltabl[i].MempoolName,
                            er));
                while(1);
            }
        }
        mempoolnametable[user_sysfpltabl[i].MemPool.mpfid] = user_sysfpltabl[i].MempoolName;
    }
}
