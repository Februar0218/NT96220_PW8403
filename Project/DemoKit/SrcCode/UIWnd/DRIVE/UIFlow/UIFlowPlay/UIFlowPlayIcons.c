#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "PlaybackTsk.h"
#include "Utility.h"
#include "audio.h"
#include "MediaPlayAPI.h"


/*
    DSCMode/Flash/EV Icon/Image size/WB/image quality/sharpness
    DZoom/Battery/Histogram/Protect/Date/Time
*/

static void FlowPB_IconDrawDSCMode(BOOL bShow)
{
    UINT32 uiFlag;

    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_DSCModeCtrl, FALSE);
        return;
    }

    //show icon
    if (UIPlay_GetData(PLAY_CURRMODE) ==PLAYMODE_AVI)
    {
        uiFlag = ICON_FILE_VIDEO;
    }
    else
    {
        uiFlag = ICON_MODE_PLAYBACK;
    }

    UxStatic_SetData(&UIFlowWndPlay_StaticICN_DSCModeCtrl,STATIC_VALUE,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_DSCModeCtrl, TRUE);
}

static void FlowPB_IconDrawFlash(BOOL bShow)
{
    UINT32 uiFlag;
    PJPGHEAD_DEC_CFG pJpgInfo;

    // Hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_FlashCtrl, FALSE);
        return;
    }
    //show icon
    pJpgInfo  = PB_GetCurrImageInfo();

    switch (pJpgInfo->pExifInfo->StrobeFlash)
    {
    case (StrobeFlashForceON+StrobeNoFired):
    case (StrobeFlashForceON+StrobeFired):
        uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_ON;
        break;

    case (StrobeFlashAutoRedEye+StrobeNoFired):
    case (StrobeFlashAutoRedEye+StrobeFired):
    case (StrobeFlashForceONRedEye+StrobeNoFired):
    case (StrobeFlashForceONRedEye+StrobeFired):
        uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_RED_EYE;
        break;

    case StrobeFlashForceOFF:
        uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_OFF;
        break;

    case (StrobeFlashAuto+StrobeNoFired):
    case (StrobeFlashAuto+StrobeFired):
    default:
        uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_AUTO;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_FlashCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_FlashCtrl, TRUE);
}


void FlowPB_IconDrawEVIcon(BOOL bShow)
{
    PJPGHEAD_DEC_CFG pJpgInfo;
    UINT32           uiFlag;
    char             *str;

    // Hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_EVCtrl,FALSE);
        return;
    }
    //show icon
    pJpgInfo  = PB_GetCurrImageInfo();
    str = (char *)pJpgInfo->pExifInfo->ExpBiasValue;

    if      (!strncmp(str,"-2.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M2P0;
    else if (!strncmp(str,"-5/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P6;
    else if (!strncmp(str,"-4/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P3;
    else if (!strncmp(str,"-1.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P0;
    else if (!strncmp(str,"-2/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M0P6;
    else if (!strncmp(str,"-1/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M0P3;
    else if (!strncmp(str,"+1/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P3;
    else if (!strncmp(str,"+2/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P6;
    else if (!strncmp(str,"+1.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P0;
    else if (!strncmp(str,"+4/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P3;
    else if (!strncmp(str,"+5/3",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P6;
    else if (!strncmp(str,"+2.0",4)) uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P2P0;
    else                             uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P0;

    UxState_SetData(&UIFlowWndPlay_StatusICN_EVCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_EVCtrl,TRUE);
}


static void FlowPB_IconDrawDCFFileID(BOOL bShow)
{
    static char item1_Buf[32];

    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_FilenameCtrl, FALSE);
        return;
    }

    //show icon
    snprintf(item1_Buf,32,"%03ld-%04ld",PB_GetCurFileDirId(),PB_GetCurFileNameId());

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_FilenameCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_FilenameCtrl, TRUE);
}

static void FlowPB_IconDrawImageSize(BOOL bShow)
{
    PJPGHEAD_DEC_CFG pJpgInfo;
    static char item1_Buf[32];

    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SizeCtrl, FALSE);
        return;
    }
    //show icon
    pJpgInfo  = PB_GetCurrImageInfo();

//    if (UIPlay_GetData(PLAY_CURRMODE) == PLAYMODE_PRIMARY)
    if (UIPlay_GetData(PLAY_FILEFORMAT)&PLAYFMT_JPG)
    {
        snprintf(item1_Buf,32,"%dx%d",pJpgInfo->ori_imagewidth,pJpgInfo->ori_imageheight);
    }
    else
    {
        pJpgInfo = PB_GetCurrImageInfoBeforeDec(FALSE);
        switch(pJpgInfo->imagewidth)
        {
        case 1440:
            snprintf(item1_Buf,32,"1080P");
            break;
        case 1280:
            snprintf(item1_Buf,32,"720P");
            break;
        case 848:
            snprintf(item1_Buf,32,"WVGA");
            break;
        case 640:
            snprintf(item1_Buf,32,"VGA");
            break;
        case 320:
            snprintf(item1_Buf,32,"QVGA");
            break;
        default:
            snprintf(item1_Buf,32,"%dx%d",pJpgInfo->imagewidth,pJpgInfo->imageheight);
        }
    }

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_SizeCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SizeCtrl, TRUE);
}


static void FlowPB_IconDrawWB(BOOL bShow)
{
    PJPGHEAD_DEC_CFG pJpgInfo;
    UINT32        uiFlag;

    //hide icon
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_WBCtrl, FALSE);
        return;
    }
    //show icon
    pJpgInfo  = PB_GetCurrImageInfo();

    switch (pJpgInfo->pExifInfo->WhiteBalance)
    {
    case AutoWhite:
    case ManualWhite:
        //Todo: implement all kind of manual white
    default:
        uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_AUTO;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_WBCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_WBCtrl, TRUE);
}


static void FlowPB_IconDrawImageQuality(BOOL bShow)
{
    PJPGHEAD_DEC_CFG pJpgInfo;
    UINT32 uiFlag;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_QualityCtrl, FALSE);
        return;
    }
    pJpgInfo  = PB_GetCurrImageInfo();

    switch(pJpgInfo->pExifInfo->JPGQuality)
    {
    //case Quality_SuperHigh:
        //Rev
        //break;
    case Quality_High:
        uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_FINE;
        break;
    case Quality_Economy:
        uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_NORMAL;
        break;
    case Quality_Undefine:
    default:
        uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_BASIC;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_QualityCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_QualityCtrl, TRUE);
}


static void FlowPB_IconDrawSharpness(BOOL bShow)
{
    PJPGHEAD_DEC_CFG pJpgInfo;
    UINT32  uiFlag;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_SharpnessCtrl, FALSE);
        return;
    }
    pJpgInfo  = PB_GetCurrImageInfo();

    switch(pJpgInfo->pExifInfo->Sharpness)
    {
    case HARD_SHARPNESS:
        uiFlag = UIFlowWndPlay_StatusICN_Sharpness_ICON_SHARPNESS_HIGH;
        break;
    case NORMAL_SHARPNESS:
        uiFlag = UIFlowWndPlay_StatusICN_Sharpness_ICON_SHARPNESS_MED;
        break;
    case SOFT_SHARPNESS:
    default:
        uiFlag = UIFlowWndPlay_StatusICN_Sharpness_ICON_SHARPNESS_LOW;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_SharpnessCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_SharpnessCtrl, TRUE);
}


static void FlowPB_IconDrawStorage(BOOL bShow)
{
    if (bShow==FALSE)
    {
       UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_StorageCtrl, FALSE);
       return;
    }
    switch(GetCardStatus())
    {
    case CARD_REMOVED:
        UxState_SetData(&UIFlowWndPlay_StatusICN_StorageCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_Storage_ICON_INTERNAL_FLASH);
        break;
    default:
        UxState_SetData(&UIFlowWndPlay_StatusICN_StorageCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_Storage_ICON_SD_CARD);
        break;
    }

    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_StorageCtrl, TRUE);
}

static void FlowPB_IconDrawBattery(BOOL bShow)
{
    UINT32 uiFlag;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_BatteryCtrl, FALSE);
        return;
    }
    switch(GetBatteryLevel())
    {
    case BATTERY_LOW:
        uiFlag= UIFlowWndPlay_StatusICN_Battery_ICON_BATTERY_LOW;
        break;
    case BATTERY_EMPTY:
        uiFlag= UIFlowWndPlay_StatusICN_Battery_ICON_BATTERY_EMPTY;
        break;
    case BATTERY_FULL:
        uiFlag= UIFlowWndPlay_StatusICN_Battery_ICON_BATTERY_FULL;
        break;
    case BATTERY_MED:
        uiFlag= UIFlowWndPlay_StatusICN_Battery_ICON_BATTERY_MED;
        break;
    default:
        uiFlag= UIFlowWndPlay_StatusICN_Battery_ICON_BATTERY_EMPTY;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_BatteryCtrl,STATE_CURITEM,uiFlag);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_BatteryCtrl, TRUE);
}


static void FlowPB_IconDrawHistogram(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_HistoCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_HistoCtrl, FALSE);
}


static void FlowPB_IconDrawFileLock(BOOL bShow)
{
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, FALSE);
        return;
    }

    if (UIPlay_GetData(PLAY_LOCKSTATUS))
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, TRUE);
    }
    else
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, FALSE);
    }
}

void FlowPB_IconDrawMovPlayVolumn(UINT32 uiVolumn)
{
    UINT32 uiFlag;

    switch(uiVolumn)
    {
    case AUDIO_VOL_MUTE:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_0;
        break;
    case AUDIO_VOL_0:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_1;
        break;
    case AUDIO_VOL_1:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_2;
        break;
    case AUDIO_VOL_2:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_3;
        break;
    case AUDIO_VOL_3:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_4;
        break;
    case AUDIO_VOL_4:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_5;
        break;
    case AUDIO_VOL_5:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_6;
        break;
    case AUDIO_VOL_6:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_7;
        break;
    case AUDIO_VOL_7:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_8;
        break;
    default:
        uiFlag= UIFlowWndPlay_StatusICN_Volumn_ICON_VOL_0;
        break;
    }

    UxState_SetData(&UIFlowWndPlay_StatusICN_VolumnCtrl,STATE_CURITEM,uiFlag);
    //UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_VolumnCtrl, TRUE);
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_VolumnCtrl, FALSE);	
}

void FlowPB_IconHideMovPlayVolumn(void)
{
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_VolumnCtrl, FALSE);
}

void FlowPB_IconDrawMovBwd(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovBwdCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovBwdCtrl, FALSE);
}

void FlowPB_IconDrawMovFwd(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovFwdCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovFwdCtrl, FALSE);
}

void FlowPB_IconDrawPrint(BOOL bShow)
{
    if (bShow == FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_PrintCtrl, FALSE);
    } else {
        if (UIPlay_GetData(PLAY_DPOFQTY))
        {
            UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_PrintCtrl, TRUE);
        }
        else
        {
            UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_PrintCtrl, FALSE);
        }
    }
}

void FlowPB_IconDrawDate(BOOL bShow)
{
    UINT    Date[3];
    UINT    uiFileFormat;
    static char item1_Buf[32];

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_DateCtrl, FALSE);
        return;
    }
    switch(UIPlay_GetData(PLAY_CURRMODE))
    {
    case PLAYMODE_ASF:
    case PLAYMODE_AVI:
        uiFileFormat = FST_FMT_AVI;
        break;
    case PLAYMODE_MOVMJPG:
        uiFileFormat = FST_FMT_MOV;
        break;
    case PLAYMODE_WAV:
        uiFileFormat = FST_FMT_WAV;
        break;
    default:
        uiFileFormat = FST_FMT_JPG;
    }

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysGetFileTime2(PB_GetCurFileDirId(), PB_GetCurFileNameId(), uiFileFormat, FST_TIMEMODE_MODIFY_DATE, &Date[0], &Date[1], &Date[2]);

    Date[0] %= 100;
    if (Date[0] > 50)  Date[0] = 50;
    if (Date[1] > 12)  Date[0] = 12;
    if (Date[2] > 31)  Date[0] = 31;

    snprintf(item1_Buf,20,"%02d/%02d/%02d",Date[0],Date[1],Date[2]);

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_DateCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_DateCtrl, TRUE);
}


static void FlowPB_IconDrawTime(BOOL bShow)
{
    UINT  Time[3];
    UINT  uiFileFormat;
    static char item1_Buf[32];

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_TimeCtrl, FALSE);
        return;
    }
    switch(UIPlay_GetData(PLAY_CURRMODE))
    {
    case PLAYMODE_ASF:
    case PLAYMODE_AVI:
        uiFileFormat = FST_FMT_AVI;
        break;
    case PLAYMODE_MOVMJPG:
        uiFileFormat = FST_FMT_MOV;
        break;
    case PLAYMODE_WAV:
        uiFileFormat = FST_FMT_WAV;
        break;
    default:
        uiFileFormat = FST_FMT_JPG;
    }

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysGetFileTime2(PB_GetCurFileDirId(), PB_GetCurFileNameId(), uiFileFormat, FST_TIMEMODE_MODIFY_TIME, &Time[0], &Time[1], &Time[2]);

    if (Time[0]>23)   Time[0]=0;
    if (Time[1]>59)   Time[1]=0;

    snprintf(item1_Buf,20,"%02d:%02d",Time[0],Time[1]);

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_TimeCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_TimeCtrl, TRUE);
}


static void FlowPB_IconDrawThumbSeq(BOOL bShow)
{
    static char item1_Buf[32];
    UINT16  *pThumbSeqID;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl, FALSE);
        return;
    }
    pThumbSeqID = PB_GetAllThumbSeqIdArray();

    snprintf(item1_Buf,32,"%d/%ld",*(pThumbSeqID + PB_GetCurFileIndex() - 1),PB_GetCurrFileSysInfo(PLAYFILEINFO_FILENUMS));

    UxStatic_SetData(&UIFlowWndPlayThumb_ThumbIDCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));
    UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl, TRUE);
}


void FlowPB_IconDrawMovPlay(BOOL bShow)
{
    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovPlayCtrl, FALSE);
        return;
    }
    if(g_PlbData.State == PLB_ST_PLAY_MOV)
    {
        UxState_SetData(&UIFlowWndPlay_StatusICN_MovPlayCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_MovPlay_ICON_OK_PAUSE);
    }
    else
    {
        UxState_SetData(&UIFlowWndPlay_StatusICN_MovPlayCtrl,STATE_CURITEM,UIFlowWndPlay_StatusICN_MovPlay_ICON_OK_PLAY);
    }
    UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovPlayCtrl, TRUE);
}

static void FlowPB_IconDrawMovStop(BOOL bShow)
{
    if (bShow==TRUE)
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovStopCtrl, TRUE);
    else
        UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_MovStopCtrl, FALSE);
}

//void FlowPB_IconDrawMovSpeed(UINT16 iVideoSpeedStep)
void FlowPB_IconDrawMovSpeed(void)
{
  static CHAR VideoSpeedStr[10];
  BOOL bShow = TRUE;

#if 0
     if (g_PlbData.VideoPBSpeed == PLB_FWD_MOV_8x) {
        if (iVideoSpeedStep == 1)
            g_PlbData.VideoPBSpeed = PLB_FWD_MOV_8x;//PLB_FWD_MOV_1x;
        else
            g_PlbData.VideoPBSpeed += iVideoSpeedStep;
     } else if (g_PlbData.VideoPBSpeed == PLB_BWD_MOV_8x) {
        if (iVideoSpeedStep == -1)
            g_PlbData.VideoPBSpeed = PLB_BWD_MOV_8x;//PLB_FWD_MOV_1x;
        else
            g_PlbData.VideoPBSpeed += iVideoSpeedStep;
     } else {
         g_PlbData.VideoPBSpeed += iVideoSpeedStep;
     }
#endif
     switch(g_PlbData.VideoPBSpeed) {
        case PLB_FWD_MOV_2x:
              sprintf(VideoSpeedStr,"2X");
            break;
        case PLB_FWD_MOV_4x:
              sprintf(VideoSpeedStr,"4X");
            break;
        case PLB_FWD_MOV_8x:
              sprintf(VideoSpeedStr,"8X");
            break;
        case PLB_BWD_MOV_2x:
              sprintf(VideoSpeedStr,"-2X");
            break;
        case PLB_BWD_MOV_4x:
              sprintf(VideoSpeedStr,"-4X");
            break;
        case PLB_BWD_MOV_8x:
              sprintf(VideoSpeedStr,"-8X");
            break;
        default:
            break;
     }

     if (g_PlbData.VideoPBSpeed==PLB_FWD_MOV_1x)
     {
        bShow = FALSE;
//        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWDSTOP,0);
     }
     else if (g_PlbData.VideoPBSpeed<=PLB_BWD_MOV_2x)
     { // Backword
//         Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVBWD,1,g_PlbData.VideoPBSpeed);
     }
     else
     { // Forward
//         Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWD,1,g_PlbData.VideoPBSpeed);
     }

     UxStatic_SetData(&UIFlowWndPlay_StaticTXT_SpeedCtrl,STATIC_VALUE,Txt_Pointer(VideoSpeedStr));
     UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SpeedCtrl, bShow);
}

void FlowPB_IconHideMovSpeed(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPB_IconDrawMovPlayTime(BOOL bShow)
{
    static char item1_Buf[32];
    UINT32 uiPlayTimeSecs;

    if (bShow==FALSE)
    {
        UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, FALSE);
        return;
    }
    uiPlayTimeSecs = MediaPlay_GetCurrPlaySecs();

    snprintf(item1_Buf, 32, "%02ld:%02ld:%02ld", uiPlayTimeSecs/3600, (uiPlayTimeSecs%3600)/60, (uiPlayTimeSecs%3600)%60);

    UxStatic_SetData(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl,STATIC_VALUE,Txt_Pointer(item1_Buf));

    UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, TRUE);
}

void FlowPB_UpdateIcons(BOOL bShow)
{
    if (bShow == FALSE || SysGetFlag(FL_LCD_DISPLAY) == DISPLAY_OFF)
    {
        FlowPB_IconDrawDSCMode(FALSE);
        FlowPB_IconDrawFlash(FALSE);
        FlowPB_IconDrawEVIcon(FALSE);
        FlowPB_IconDrawDCFFileID(FALSE);
        FlowPB_IconDrawImageSize(FALSE);
        FlowPB_IconDrawWB(FALSE);
        FlowPB_IconDrawImageQuality(FALSE);
        FlowPB_IconDrawSharpness(FALSE);
        FlowPB_IconDrawStorage(FALSE);
        FlowPB_IconDrawBattery(FALSE);
        FlowPB_IconDrawHistogram(FALSE);
        FlowPB_IconDrawFileLock(FALSE);
        FlowPB_IconDrawDate(FALSE);
        FlowPB_IconDrawPrint(FALSE);
        FlowPB_IconHideMovPlayVolumn();
        FlowPB_IconDrawTime(FALSE);
        FlowPB_IconDrawMovPlay(FALSE);
        FlowPB_IconDrawMovStop(FALSE);
        FlowPB_IconDrawMovPlayTime(FALSE);
        FlowPB_IconHideMovSpeed(&UIFlowWndPlay_StaticTXT_SpeedCtrl);
    }
    else
    {
        switch(SysGetFlag(FL_LCD_DISPLAY))
        {
        case DISPLAY_ALL:
            FlowPB_IconDrawDSCMode(TRUE);
            FlowPB_IconDrawEVIcon(TRUE);
            FlowPB_IconDrawDCFFileID(TRUE);
            FlowPB_IconDrawImageSize(TRUE);
            FlowPB_IconDrawWB(TRUE);
            FlowPB_IconDrawStorage(TRUE);
            FlowPB_IconDrawBattery(TRUE);
            FlowPB_IconDrawHistogram(TRUE);
            FlowPB_IconDrawFileLock(TRUE);
            FlowPB_IconDrawPrint(TRUE);
            FlowPB_IconDrawDate(TRUE);
            FlowPB_IconDrawTime(TRUE);
            FlowPB_IconHideMovPlayVolumn();
            FlowPB_IconHideMovSpeed(&UIFlowWndPlay_StaticTXT_SpeedCtrl);
            FlowPB_IconDrawMovPlayTime(FALSE);
            if (UIPlay_GetData(PLAY_CURRMODE) == PLAYMODE_PRIMARY)
            {
                FlowPB_IconDrawFlash(TRUE);
                FlowPB_IconDrawImageQuality(TRUE);
                FlowPB_IconDrawSharpness(TRUE);
                FlowPB_IconDrawMovPlay(FALSE);
                FlowPB_IconDrawMovStop(FALSE);
                FlowPB_IconDrawMovBwd(FALSE);
                FlowPB_IconDrawMovFwd(FALSE);
            }
            else
            {
                FlowPB_IconDrawFlash(FALSE);
                FlowPB_IconDrawImageQuality(FALSE);
                FlowPB_IconDrawSharpness(FALSE);
                FlowPB_IconDrawMovPlay(TRUE);
                FlowPB_IconDrawMovStop(TRUE);
                FlowPB_IconDrawMovBwd(TRUE);
                FlowPB_IconDrawMovFwd(TRUE);
            }
            break;
        case DISPLAY_NORMAL:
            FlowPB_IconDrawDSCMode(TRUE);
            FlowPB_IconDrawFlash(FALSE);
            FlowPB_IconDrawEVIcon(FALSE);
            FlowPB_IconDrawDCFFileID(TRUE);
            FlowPB_IconDrawImageSize(TRUE);
            FlowPB_IconDrawWB(FALSE);
            FlowPB_IconDrawImageQuality(FALSE);
            FlowPB_IconDrawSharpness(FALSE);
            FlowPB_IconDrawStorage(FALSE);
            FlowPB_IconDrawBattery(TRUE);
            FlowPB_IconDrawHistogram(FALSE);
            FlowPB_IconDrawFileLock(TRUE);
            FlowPB_IconDrawPrint(TRUE);
            FlowPB_IconHideMovPlayVolumn();
            FlowPB_IconHideMovSpeed(&UIFlowWndPlay_StaticTXT_SpeedCtrl);
            FlowPB_IconDrawDate(TRUE);
            FlowPB_IconDrawTime(TRUE);
            FlowPB_IconDrawMovPlayTime(FALSE);
            if (UIPlay_GetData(PLAY_CURRMODE) == PLAYMODE_PRIMARY)
            {
                FlowPB_IconDrawMovPlay(FALSE);
                FlowPB_IconDrawMovStop(FALSE);
                FlowPB_IconDrawMovBwd(FALSE);
                FlowPB_IconDrawMovFwd(FALSE);
            }
            else
            {
                FlowPB_IconDrawMovPlay(TRUE);
                FlowPB_IconDrawMovStop(TRUE);
                FlowPB_IconDrawMovBwd(TRUE);
                FlowPB_IconDrawMovFwd(TRUE);
            }
            break;
        }
    }
}

void FlowPB_DrawThumbRect(BOOL bShow)
{
    switch(PB_GetCurFileIndex())
    {
    //Playback fileIndex starts from 1.
    case 1:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel0Ctrl,bShow);
        break;
    case 2:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel1Ctrl,bShow);
        break;
    case 3:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel2Ctrl,bShow);
        break;
    case 4:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel3Ctrl,bShow);
        break;
    case 5:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel4Ctrl,bShow);
        break;
    case 6:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel5Ctrl,bShow);
        break;
    case 7:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel6Ctrl,bShow);
        break;
    case 8:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel7Ctrl,bShow);
        break;
    case 9:
        UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel8Ctrl,bShow);
        break;
    }
}


void FlowPB_ClearAllThumbIcon(void)
{
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel0Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel1Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel2Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel3Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel4Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel5Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel6Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel7Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel8Ctrl,FALSE);

    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect0Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect1Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect2Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect3Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect4Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect5Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect6Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect7Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect8Ctrl,FALSE);

    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm0Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm1Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm2Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm3Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm4Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm5Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm6Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm7Ctrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm8Ctrl,FALSE);

    UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl,FALSE);
}


void FlowPB_ShowAllThumbIcon(void)
{
    UINT32  i, loopcnts,CurrFileFormat ;
    UINT16  *pCurrFileFormat;   // Hideo@0503: variable type should be UINT16

    FlowPB_DrawThumbRect(TRUE);

    FlowPB_IconDrawThumbSeq(TRUE);

    pCurrFileFormat = (UINT16*)PB_GetAllThumbFileFormatArray();
    loopcnts = PB_GetCurThumbNums();

    for (i=0; i<loopcnts; i++)
    {
        CurrFileFormat  = *pCurrFileFormat++;

        if (CurrFileFormat & PLAYFMT_READONLY)
        {
            switch(i)
            {
            case 0:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect0Ctrl,TRUE);
                break;
            case 1:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect1Ctrl,TRUE);
                break;
            case 2:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect2Ctrl,TRUE);
                break;
            case 3:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect3Ctrl,TRUE);
                break;
            case 4:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect4Ctrl,TRUE);
                break;
            case 5:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect5Ctrl,TRUE);
                break;
            case 6:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect6Ctrl,TRUE);
                break;
            case 7:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect7Ctrl,TRUE);
                break;
            case 8:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect8Ctrl,TRUE);
                break;
            }
        }

        if (CurrFileFormat & (PLAYFMT_AVI|PLAYFMT_ASF|PLAYFMT_MPG))
        {
            switch(i)
            {
            case 0:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm0Ctrl,TRUE);
                break;
            case 1:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm1Ctrl,TRUE);
                break;
            case 2:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm2Ctrl,TRUE);
                break;
            case 3:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm3Ctrl,TRUE);
                break;
            case 4:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm4Ctrl,TRUE);
                break;
            case 5:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm5Ctrl,TRUE);
                break;
            case 6:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm6Ctrl,TRUE);
                break;
            case 7:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm7Ctrl,TRUE);
                break;
            case 8:
                UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm8Ctrl,TRUE);
                break;
            }
        }
    }
}


