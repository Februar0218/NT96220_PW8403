/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AviPlayManagerTsk.h
    @ingroup    mIAPPAVIMJPEG

    @brief      All functions for AVI playback mode header file

    @note

*/

#ifndef _AVIPLAYAPI_H
#define _AVIPLAYAPI_H

#include "Type.h"
#include "Audio.h"
#include "AppCommon.h"
//#NT#2009/08/18#Corey Ke -begin
//#NT#Support index view(view by date)
#include "FileDB.h"
//#NT#2009/08/18#Corey Ke -end

//#NT#2009/10/05#Photon Lin#[0006424] -begin
//#Patch for 15X speed play
#define AVIPLAY_15X_PATCH               1
#define AVIPLAY_15X_THR_SPEED           2  // {1,2,4,8,16}, 2 correspond to 4X
//#NT#2009/10/05#Photon Lin#[0006424] -end
//#NT#2009/10/16#Photon Lin -begin
//#Patch for 0.5X speed play
#define AVIPLAY_0_5X_PATCH              0
#define AVIPLAY_0_5X_THR_SPEED          4
//#NT#2009/10/16#Photon Lin -end

//#NT#2009/11/02#Photon Lin -begin
//#Add Display task for play usage
//#NT#2009/12/07#Photon Lin -begin
//#Using another task due to plug TV issue (640*448 spend 30ms)
#define AVIPLAY_DISP_BY_OTHER_TSK       1
//#NT#2009/12/07#Photon Lin -end
#define AVIPLAY_JPEG_BUF_NUM            2
//#NT#2009/11/02#Photon Lin -end
#define AVIPLAYFORWARD                  0
#define AVIPLAYBACKWARD                 1
//#NT#2009/10/16#Photon Lin -begin
//#Mark unused defination
/*
#define AVIPLAYSPEED1X                  0
#define AVIPLAYSPEED2X                  1
#define AVIPLAYSPEED4X                  2
#define AVIPLAYSPEED5X                  3
#define AVIPLAYSPEED8X                  4
#define AVIPLAYSPEEDHALF                5
*/
//#NT#2009/10/16#Photon Lin -end
//#NT#2009/06/08#Photon Lin -begin
//Remove unused definition
//#define AVI_PLAY_CURR                   0
//#define AVI_PLAY_NEXT                   1
//#define AVI_PLAY_PREV                   2
//#NT#2009/06/08#Photon Lin -end
//#NT#2009/12/07#Photon Lin -begin
//#Define maximum width and height
#define AVIPLAY_MAX_WIDTH               1280
#define AVIPLAY_MAX_HEIGHT              720
//#NT#2009/12/07#Photon Lin -end
#define AVIPLAY_FIRST_FRAME             0
#define AVIPLAY_LAST_FRAME              0xFFFFFFFF

/**
@name Values: EVENT ID
@{
*/
#define AVIPLAY_EVENT_FINISH            1   ///< Finish
#define AVIPLAY_EVENT_ONE_SECOND        2   ///< One second arrives
#define AVIPLAY_EVENT_ONE_VIDEOFRAME    3   ///< Decode one video-frame
#define AVIPLAY_EVENT_START             4   ///< Start AVI playback
//#NT#2009/01/06#Photon Lin -begin
//#Add INIT event
#define AVIPLAY_EVENT_INIT_PROCESSING   5
#define AVIPLAY_EVENT_INIT_OK           6
//#NT#2009/01/06#Photon Lin -end
//#NT#2009/06/08#Photon Lin -begin
//#Add ERROR event
#define AVIPLAY_EVENT_ERROR             7
//#NT#2009/06/08#Photon Lin -begin


/**
    AVI status for AviPlayManagerTsk() API
*/
typedef enum
{
    AVIPLAY_INIT,
    AVIPLAY_FW,
    AVIPLAY_BW,
    AVIPLAY_PAUSE,
    AVIPLAY_STOPPED

} AVIPlayStatusID;



/**
    @brief AVI play open parameters

    Use this struct to set parameters for AVIPlay_Open()
*/
typedef struct _AVIPLAY_OBJ {
    UINT32      uiMemAddr;                          /** Data buffer starting address*/
    UINT32      uiMemSize;                          /** Data buffer size*/
    void        (*CallBackFunc)(UINT32 event_id);   /** Callback function */
    BOOL        bHWAudio;                          /** Hardware audio support : TRUE or FALSE */
    BOOL        bHWVideo;                          /** Hardware video support : TRUE or FALSE */
    /**
        Initial audio volume (Only effective when bHWAudio == TRUE)

        @note If not assigned, the volume is unpredictable.
    */
    AUDIO_VOL   initAudVol;
    UINT8       uiPlayCommand;                       /** AVI_PLAY_CURR / AVI_PLAY_NEXT / AVI_PLAY_PREV */
//#NT#2009/08/18#Corey Ke -begin
//#NT#Support index view(view by date)
    BOOL        bPlayAllFiles;
    FILEDB_HANDLE AviFileHandle;
//#NT#2009/08/18#Corey Ke -end
    BOOL        bPlayWithBG;
    //#NT#2010/01/20#Photon Lin -begin
    //#Add parameters
    UINT32      uiDisplayFBWidth;
    UINT32      uiDisplayFBHeight;
    UINT32      uiPanelWidth;
    UINT32      uiPanelHeight;
    //#NT#2010/01/20#Photon Lin -end
} AVIPLAY_OBJ;

/** Pointer of AVIPLAY_OBJ */
typedef AVIPLAY_OBJ* PAVIPLAY_OBJ;

//---- Functions relesse -------------------------------
extern INT32            AVIPlay_Open(PAVIPLAY_OBJ    pAVIPlayObj);
extern INT32            AVIPlay_Close(void);
//#NT#2009/06/08#Photon Lin -begin
//#Remove unused function
//extern void             AVIPlayCopyDataTo(UINT32, UINT32, UINT32);
//#NT#2009/06/08#Photon Lin -end
extern void             AVIPlay_Stop(void);
extern void             AVIPlay_Pause(void);
extern void             AVIPlay_FWD(void);
extern void             AVIPlay_BWD(void);
extern void             AVIPlay_SpeedUp(UINT32 ucSpeedUpLevel);
extern void             AVIPlay_SetAudVolUp(void);
extern void             AVIPlay_SetAudVolDown(void);
extern void             AVIPlay_SetAudVol(AUDIO_VOL level);
extern void             AVIPlay_FWDStep(void);
extern void             AVIPlay_BWDStep(void);
//------------------------------------
extern AUDIO_VOL        AVIPlay_GetAudVol(void);
extern UINT32           AVIPlay_GetCurrPlaySecs(void);
extern UINT32           AVIPlay_GetTotalSecs(void);
extern UINT32           AVIPlay_GetSpeedLevel(void);
extern UINT32           AVIPlay_GetCurrPlayDir(void);
extern UINT32           AVIPlay_GetCurrTotalVideoFrame(void);
extern UINT32           AVIPlay_GetCurrCurrVideoFrame(void);
extern void             AVIPlay_SetIMEIDESize(PIMEIDE_ATTR pIMEIDEAttr);
extern void             AVIPlay_SwitchFrame(UINT32 findex);
extern void             AVIPLAY_Set1stVideoFrame(UINT16 uiDstLU_X, UINT16 uiDstLU_Y, UINT16 uiDstSZ_W, UINT16 uiDstSZ_H);
extern BOOL             AVIPLAY_GetAVIOpened(void);
extern void             AVIPLAY_SetCommandAccess(BOOL bEn);
extern BOOL             AVIPLAY_CheckCommandAccess(void);

extern void             PlayVideo_WaitIMENotBusy(void);
//#NT#2009/07/28#Lincy Lin -begin
//#Export function for UI using
extern AVIPlayStatusID  AVIPlay_GetStatus(void);
//#NT#2009/07/28#Lincy Lin -end
//---- data relesse ------------------------------------

//------------------------------------------------------

/**
@}
*/

#endif // _AVIPLAYAPI_H
