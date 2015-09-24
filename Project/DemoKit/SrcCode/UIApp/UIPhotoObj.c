#include <stdio.h>
#include <string.h>
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIFlow.h"
#include "ImageAlgInfor.h"
#include "SieCapTsk.h"
#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "PlaySoundTsk.h"
#include "GenDateImprintData.h"
#include "UserEXIF.h"
#include "GPIOMapping.h"
#include "Utility.h"
#include "DataStamp.h"

static UINT32 g_uiCurrentZoomIF     = ZOOM_IF_OPTICAL;
static BOOL g_bDZoomResetting       = FALSE;
static CHAR g_cDZoomRatioStr[MAX_DZOOM_STR_LEN] = {0};
static CHAR g_cSelftimerCntStr[8]   = {0};
static AlgMsgInfo g_AlgMsgInfo      = 0;

// Must in the same order of those in enum SIZE_SETTING
#if (UI_STYLE == UI_STYLE_DRIVE)
static const CHAR *g_pcSizeCfgTable[PHOTO_SIZE_ID_MAX] =
{
    "12M",
    "10M",
    "8M",
    "5M",
    "3M",
    "2MHD",
    "VGA",
    "1.3M",
};
#else
static const CHAR *g_pcSizeCfgTable[PHOTO_SIZE_ID_MAX] =
{
    "12M",
    "10M",
    "7MHD",
    "5M",
    "3M",
    "2MHD",
    "VGA"
};
#endif

#if 0
// Scene mode IQ config
static SceneModeInfo g_SceneInfoTbl[Max_SceneMode] =
{
    //SceneMode                 FlashMode                       Evcomp                      Saturation              Sharpness               WB                      Focus   FlashChangeAE
    {_SceneMode_Auto,           _IMGFlashMode_Auto,             _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      1},
    {_SceneMode_Program,        _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGSatus_NoChange,         0,      1},
    {_SceneMode_Landscape,      _IMGFlashMode_ForceOFF,         _IMGEVValue_00,             _IMGSaturation_P1,      _IMGSharp_P1,           _IMGWBMode_Auto,            0,      1},
    {_SceneMode_Potrait,        _IMGFlashMode_AutoRedEye,       _IMGEVValue_P1_3,           _IMGSaturation_Normal,  _IMGSharp_N1,           _IMGWBMode_Auto,            0,      1},
    {_SceneMode_NightScene,     _IMGFlashMode_ForceON,          _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Daylight,        0,      0},
    {_SceneMode_Sport,          _IMGFlashMode_ForceOFF,         _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      1},
    {_SceneMode_AV,             _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0},
    {_SceneMode_TV,             _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0},
    {_SceneMode_Manual,         _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0},
    {_SceneMode_NightPotrait,   _IMGFlashMode_NightStrobe,      _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      0},
    {_SceneMode_NightLandscape, _IMGFlashMode_ForceOFF,         _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Daylight,        0,      0},
    {_SceneMode_Party,          _IMGFlashMode_AutoRedEye,       _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      1},
    {_SceneMode_SunSet,         _IMGFlashMode_ForceOFF,         _IMGEVValue_N1,             _IMGSaturation_P1,      _IMGSharp_Normal,       _IMGWBMode_Daylight,        0,      0},
    {_SceneMode_BackLight,      _IMGFlashMode_ForceON,          _IMGEVValue_00,             _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0}
};
#endif

INT32 OnExe_Flash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 OnExe_EV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 OnExe_Saturation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 OnExe_Sharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 OnExe_WB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

//-----------------------------------------------------------------------------
// Photo Flow Object APIs
//-----------------------------------------------------------------------------
AlgMsgInfo *Get_AlgMsgInfo(void)
{
    return (void *)&g_AlgMsgInfo;
}
void Set_AlgMsgInfo(AlgMsgInfo *pInfo)
{
    memcpy((void *)&g_AlgMsgInfo, pInfo, sizeof(AlgMsgInfo));
}

UINT32 Get_FlashValue(void)
{
    _IMGFlashMode_sel val;

    switch(SysGetFlag(FL_FLASH_MODE))
    {
        case FLASH_AUTO:
            val = _IMGFlashMode_Auto;
            break;
        case FLASH_ON:
            val = _IMGFlashMode_ForceON;
            break;
        case FLASH_RED_EYE:
            val = _IMGFlashMode_AutoRedEye;
            break;
        case FLASH_OFF:
            val = _IMGFlashMode_ForceOFF;
            break;
        default:
            val = _IMGFlashMode_Auto;
            break;
    }
    return val;
}


UINT32 Get_SizeValue(void)
{
    _IMGSize_sel val;

    switch(SysGetFlag(FL_PHOTO_SIZE))
    {
        case PHOTO_SIZE_15M:
            val = _IMGSize_15M;
            break;
        case PHOTO_SIZE_14M:
            val = _IMGSize_14M;
            break;
        case PHOTO_SIZE_12M:
            val = _IMGSize_12M;
            break;
        case PHOTO_SIZE_10M:
            val = _IMGSize_10M;
            break;
        case PHOTO_SIZE_8M:
            val = _IMGSize_8M;
            break;
        case PHOTO_SIZE_7M:
            val = _IMGSize_7M;
            break;
        case PHOTO_SIZE_7MHD:
            val = _IMGSize_7M_169;
            break;
        case PHOTO_SIZE_5M:
            val = _IMGSize_5M;
            break;
        case PHOTO_SIZE_4M:
            val = _IMGSize_4M;
            break;
        case PHOTO_SIZE_3M:
            val = _IMGSize_3M;
            break;
        case PHOTO_SIZE_2M:
            val = _IMGSize_2M;
            break;
        case PHOTO_SIZE_2MHD:
            val = _IMGSize_2M_169;
            break;
        case PHOTO_SIZE_1M:
            val = _IMGSize_1M;
            break;
        case PHOTO_SIZE_VGA:
            val = _IMGSize_VGA;
            break;
        default:
            val = _IMGSize_VGA;
            break;
    }
    return val;
}

const CHAR *Get_SizeString(UINT32 uiIndex)
{
    if(uiIndex < PHOTO_SIZE_ID_MAX)
        return g_pcSizeCfgTable[uiIndex];
    else
        return NULL;
}

UINT32 Get_QualityValue(void)
{
    _IMGQuality_sel val;

    switch(SysGetFlag(FL_QUALITY))
    {
        case QUALITY_NORMAL:
            val = _IMGQuality_Normal;
            break;
        case QUALITY_FINE:
            val = _IMGQuality_Fine;
            break;
        case QUALITY_ECONOMY:
            val = _IMGQuality_Economy;
            break;
        default:
            val = _IMGQuality_Fine;
            break;
    }
    return val;
}

UINT32 Get_ColorValue(void)
{
    _IMGEffect_sel val;

    switch(SysGetFlag(FL_COLOR_EFFECT))
    {
        case COLOR_EFFECT_STANDARD:
            val = _IMGEffect_Off;
            break;
        case COLOR_EFFECT_SEPIA:
            val = _IMGEffect_Sepia;
            break;
        case COLOR_EFFECT_MONOCHROME:
            val = _IMGEffect_BW;
            break;
        case COLOR_EFFECT_VIVID:
            val = _IMGEffect_Vivid;
            break;
        default:
            val = _IMGEffect_Off;
            break;
    }
    return val;
}

UINT32 Get_SceneModeValue(void)
{
    _IMGSceneMode_sel val;

    switch (SysGetFlag(FL_SCENE))
    {
        case SCENE_AUTO:
            val = _SceneMode_Auto;
            break;
        case SCENE_SCENERY:
            val = _SceneMode_Landscape;
            break;
        case SCENE_PORTRAIT:
            val = _SceneMode_Potrait;
            break;
        case SCENE_NIGHTSCENERY:
            val = _SceneMode_NightLandscape;
            break;
        case SCENE_NIGHTPORTRAIT:
            val = _SceneMode_NightPotrait;
            break;
        case SCENE_HIGHSENSITIVITY:
            val = _SceneMode_HighISO;
            break;
        case SCENE_BEACH:
            val = _SceneMode_Beach;
            break;
        case SCENE_SPORT:
            val = _SceneMode_Sport;
            break;
        case SCENE_PARTY:
            val = _SceneMode_Party;
            break;
        default:
            val = _SceneMode_Auto;
            break;
    }
    return val;
}

UINT32 Get_EVValue(void)
{
    _IMGEVValue_sel val;

    switch(SysGetFlag(FL_EV))
    {
        case EV_N30:
            val = _IMGEVValue_N3;
            break;
        case EV_N20:
            val = _IMGEVValue_N2;
            break;
        case EV_N16:
            val = _IMGEVValue_N5_3;
            break;
        case EV_N15:
            val = _IMGEVValue_N3_2;
            break;
        case EV_N13:
            val = _IMGEVValue_N4_3;
            break;
        case EV_N10:
            val = _IMGEVValue_N1;
            break;
        case EV_N06:
            val = _IMGEVValue_N2_3;
            break;
        case EV_N03:
            val = _IMGEVValue_N1_3;
            break;
        case EV_00:
            val = _IMGEVValue_00;
            break;
        case EV_P03:
            val = _IMGEVValue_P1_3;
            break;
        case EV_P06:
            val = _IMGEVValue_P2_3;
            break;
        case EV_P10:
            val = _IMGEVValue_P1;
            break;
        case EV_P13:
            val = _IMGEVValue_P4_3;
            break;
        case EV_P15:
            val = _IMGEVValue_P3_2;
            break;
        case EV_P16:
            val = _IMGEVValue_P5_3;
            break;
        case EV_P20:
            val = _IMGEVValue_P2;
            break;
        case EV_P30:
            val = _IMGEVValue_P3;
            break;
        default:
            val = _IMGEVValue_00;
            break;
    }
    return val;
}

UINT32 Get_MeteringValue(void)
{
    _IMGAEMode_sel val;

    switch(SysGetFlag(FL_METERING))
    {
        case METERING_CENTER:
            val = _IMGAEMode_CenterWeighted;
            break;
        case METERING_MULTI:
            val = _IMGAEMode_Matrix;
            break;
        case METERING_SPOT:
            val = _IMGAEMode_Spot;
            break;
        default:
            val = _IMGAEMode_CenterWeighted;
            break;
    }
    return val;
}

UINT32 Get_WBValue(void)
{
    _IMGWBMode_sel val;

    switch(SysGetFlag(FL_WB))
    {
        case WB_AUTO:
            val = _IMGWBMode_Auto;
            break;
        case WB_DAYLIGHT:
            val = _IMGWBMode_Daylight;
            break;
        case WB_CLOUDY:
            val = _IMGWBMode_Cloudy;
            break;
        case WB_FLUORESCENT:
            val = _IMGWBMode_Fluorescent1;
            break;
        case WB_TUNGSTEN:
            val = _IMGWBMode_Tungsten;
            break;
        default:
            val = _IMGWBMode_Auto;
            break;
    }
    return val;
}

UINT32 Get_AntiShakeValue(void)
{
    _IMGStillAntiShaking val;

    switch(SysGetFlag(FL_ANTISHAKE))
    {
        case ANTISHAKE_ON:
            val = _Level2;
            break;
        case ANTISHAKE_OFF:
            val = _Level0;
            break;
        default:
            val = _Level0;
            break;
    }
    return val;
}

UINT32 Get_ISOValue(void)
{
    _IMGISO_sel val;

    switch(SysGetFlag(FL_ISO))
    {
        case ISO_AUTO:
            val = _IMGISO_Auto;
            break;
        case ISO_100:
            val = _IMGISO_100;
            break;
        case ISO_200:
            val = _IMGISO_200;
            break;
        case ISO_400:
            val = _IMGISO_400;
            break;
        case ISO_800:
            val = _IMGISO_800;
            break;
        case ISO_1600:
            val = _IMGISO_1600;
            break;
        case ISO_3200:
            val = _IMGISO_3200;
            break;
        default:
            val = _IMGISO_Auto;
            break;
    }
    return val;
}

UINT32 Get_SaturationValue(void)
{
    _IMGSaturation_sel val;

    switch(SysGetFlag(FL_SATURATION))
    {
        case SATURATION_HIGH:
            val = _IMGSaturation_P1;
            break;
        case SATURATION_NORMAL:
            val = _IMGSaturation_Normal;
            break;
        case SATURATION_LOW:
            val = _IMGSaturation_N1;
            break;
        default:
            val = _IMGSaturation_Normal;
            break;
    }
    return val;
}

UINT32 Get_SharpnessValue(void)
{
    _IMGSharpness_sel val;

    switch(SysGetFlag(FL_SHARPNESS))
    {
        case SHARPNESS_SOFT:
            val = _IMGSharp_N1;
            break;
        case SHARPNESS_NORMAL:
            val = _IMGSharp_Normal;
            break;
        case SHARPNESS_SHARP:
            val = _IMGSharp_P1;
            break;
        default:
            val = _IMGSaturation_Normal;
            break;
    }
    return val;
}

UINT32 Get_QuickReviewValue(void)
{
    UINT32 val;

    switch(SysGetFlag(FL_QUICKVIEW))
    {
        case QUICK_REVIEW_0SEC:
            val = 0;
            break;
        case QUICK_REVIEW_2SEC:
            val = 2;
            break;
        case QUICK_REVIEW_5SEC:
            val = 5;
            break;
        default:
            val = 0;
            break;
    }
    return val;
}

CHAR *Get_SelftimerCntString(UINT32 uiCount)
{
    sprintf(g_cSelftimerCntStr, "%ld", uiCount);
    return g_cSelftimerCntStr;
}

void Set_CurrentZoomIF(UINT32 uiIndex)
{
    g_uiCurrentZoomIF = uiIndex;
}
UINT32 Get_CurrentZoomIF(void)
{
    return g_uiCurrentZoomIF;
}

void Set_DZoomResetting(BOOL bReset)
{
    g_bDZoomResetting = bReset;
}
BOOL Get_DZoomResetting(void)
{
    return g_bDZoomResetting;
}

CHAR *Get_DZoomRatioString(AlgMsgInfo *pInfo)
{
    UINT32 m, n;
    UINT32 DzoomRealRatio = Get_DzoomRatio((pInfo->DzoomIndex))/10;

    m = DzoomRealRatio/10;
    n = DzoomRealRatio%10;
    sprintf(g_cDZoomRatioStr, "x%ld.%ld", m, n);
    return g_cDZoomRatioStr;
}

void UI_ZoomControl(UINT32 uiZoomCtrl, BOOL bDZoomEnable)
{
    static BOOL bIsDigital = FALSE;
    static BOOL bOZoomStop = TRUE;

    switch(uiZoomCtrl)
    {
        case UI_ZOOM_CTRL_STOP:
            if(bIsDigital == FALSE)
            {
                if (bOZoomStop == FALSE)
                {
                    debug_msg("UI_ZoomControl: Lens_Zoom_Stop\r\n");
                    Lens_Zoom_Stop();
                    UILens_FocusToFar(Lens_Zoom_GetSection());
                    bOZoomStop = TRUE;
                }
            }
            else
            {
                debug_msg("UI_ZoomControl: Photo_DZoom_Stop\r\n");
                  Photo_DZoom_Stop();
            }
            break;

        case UI_ZOOM_CTRL_IN:
            if(Lens_Zoom_GetSection() >= Lens_Zoom_GetMaxSection())
            {
                if (bOZoomStop == FALSE)
                {
                    debug_msg("UI_ZoomControl: Lens_Zoom_Stop as UI_ZOOM_CTRL_IN\r\n");
                    Lens_Zoom_Stop();
                    UILens_FocusToFar(Lens_Zoom_GetSection());
                    bOZoomStop = TRUE;
                }

                if(bDZoomEnable)
                {
                    debug_msg("UI_ZoomControl: Photo_DZoom_In\r\n");
                    Photo_DZoom_In();
                    bIsDigital = TRUE;
                    Set_CurrentZoomIF(ZOOM_IF_DIGITAL);
                }
            }
            else
            {
                debug_msg("UI_ZoomControl: Lens_Zoom_In\r\n");
                #if (_LENSLIB_ == _LENSLIB_ST8005_)
                Lens_Zoom_Goto(Lens_Zoom_GetSection()+1);
                Lens_OnOff(MOTOR_POWER_OFF, 0);
                #else
                Lens_Zoom_In();
                #endif
                bIsDigital = FALSE;
                bOZoomStop = FALSE;
                Set_CurrentZoomIF(ZOOM_IF_OPTICAL);
            }
            break;

        case UI_ZOOM_CTRL_OUT:
            if(Photo_GetDZoomIdx() <= UI_DZOOM_IDX_MIN)
            {
                if(Lens_Zoom_GetSection() <= 1) // zoom section 1
                {
                    if (bOZoomStop == FALSE)
                    {
                        UINT32 uiTimeCount;
                        debug_msg("UI_ZoomControl: Lens_Zoom_Stop as UI_ZOOM_CTRL_OUT\r\n");
                        Perf_Open();
                        uiTimeCount = Perf_GetCurrent();
                        Lens_Zoom_Stop();
                        //debug_msg("Lens_Zoom_Stop(): %d ms\r\n", (Perf_GetCurrent()-uiTimeCount)/1000);
                        uiTimeCount = Perf_GetCurrent();
                        UILens_FocusToFar(Lens_Zoom_GetSection());
                        //debug_msg("UILens_FocusToFar(): %d ms\r\n", (Perf_GetCurrent()-uiTimeCount)/1000);
                        bOZoomStop = TRUE;
                    }
                }
                else
                {
                    debug_msg("UI_ZoomControl: Lens_Zoom_Out\r\n");
                    #if (_LENSLIB_ == _LENSLIB_ST8005_)
                    Lens_Zoom_Goto(Lens_Zoom_GetSection()-1);
                    Lens_OnOff(MOTOR_POWER_OFF, 0);
                    #else
                Lens_Zoom_Out();
                    #endif
                bIsDigital = FALSE;
                    bOZoomStop = FALSE;
                Set_CurrentZoomIF(ZOOM_IF_OPTICAL);
                }
            }
            else
            {
                debug_msg("UI_ZoomControl: Photo_DZoom_Out\r\n");
                Photo_DZoom_Out();
                bIsDigital = TRUE;
                Set_CurrentZoomIF(ZOOM_IF_DIGITAL);
            }
            break;

        case UI_ZOOM_CTRL_RESET_DZOOM:
            if((bIsDigital == TRUE) && (Photo_GetDZoomIdx() > UI_DZOOM_IDX_MIN))
            {
                debug_msg("UI_ZoomControl: Photo_DZoom_GotoStep(UI_DZOOM_IDX_MIN)\r\n");
                Set_DZoomResetting(TRUE);
                Photo_DZoom_GotoStep(UI_DZOOM_IDX_MIN);
            }
            bIsDigital = FALSE;
            Set_CurrentZoomIF(ZOOM_IF_OPTICAL);
            break;

        default:
            debug_err(("UI_ZoomControl: Unknown zoom control 0x%x\r\n", uiZoomCtrl));
            break;
    }
}

//-----------------------------------------------------------------------------
// Photo Flow Event Handler
//-----------------------------------------------------------------------------
INT32 OnCapture(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiDateFormat = 0;

    debug_err(("UIPhotoObj: OnCapture\r\n"));

    /* Get date format */
    switch(SysGetFlag(FL_DATE_FORMAT))
    {
        case DATE_FORMAT_DMY:
            uiDateFormat = STAMP_DD_MM_YY;
            break;

        case DATE_FORMAT_MDY:
            uiDateFormat = STAMP_MM_DD_YY;
            break;

        case DATE_FORMAT_YMD:
        default:
            uiDateFormat = STAMP_YY_MM_DD;
            break;
    }

    /* Generate date imprint */
    switch(SysGetFlag(FL_DATE_STAMP))
    {
        case DATE_STAMP_DATE:
            DateImprint_Setup(
                STAMP_ON |
                STAMP_AUTO |
                STAMP_DATE |
                STAMP_BOTTOM_LEFT |
                uiDateFormat,DS_YCCFORMAT_422);
            break;

        case DATE_STAMP_DATETIME:
            DateImprint_Setup(
                STAMP_ON |
                STAMP_AUTO |
                STAMP_DATE_TIME |
                STAMP_BOTTOM_LEFT |
                uiDateFormat,DS_YCCFORMAT_422);
            break;

        case DATE_STAMP_OFF:
        default:
            DateImprint_Setup(STAMP_OFF,DS_YCCFORMAT_422);
            break;
    }

    UserEXIF_ModifyTags(); // Modify user defined EXIF tags

    /* Play S2 sound */
    if(SysGetFlag(FL_CONTINUE_SHOT) == CONTINUE_SHOT_OFF)
    {
        /* Set other necessary IQ parameters */
        ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
        ImgCapture_SetParameter(_SieCapBitDepth, _SieBitDepth_8);
        ImgCapture_SetParameter(_SiePath, _PostGamma);
        ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
        ImgCapture_SetParameter(_BurstNum,  _BurstNum_1);
    }
    else
    {
        //#NT#2011/08/16#KS Hung -begin
        if (GPIOMap_DetStrgCard() == FALSE)
        {
            ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieCapBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_SiePath, _PostGamma);
            ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
            ImgCapture_SetParameter(_BurstNum,  _BurstNum_1);
        }
        else
        {
            ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
            ImgCapture_SetParameter(_FileFmt,  _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieCapBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_BurstNum,  _BurstNum_5);
            ImgCapture_SetParameter(_DriveMode,  _IMGDriveMode_SmallBurst16);
        }
        //#NT#2011/08/16#KS Hung -end
    }

    /* update DCF filename while capturing one photo and sync with RTC date/time */
    Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_UPDATE_DCF, 1, TRUE);

    /* Start capture */
    ImgCapture_StartCapture();

    return NVTEVT_CONSUME;
}

INT32 OnZoomIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* Assume digital zoom is always enabled */
    UI_ZoomControl(UI_ZOOM_CTRL_IN, TRUE);
    return NVTEVT_CONSUME;
}

INT32 OnZoomOut(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* Assume digital zoom is always enabled */
    UI_ZoomControl(UI_ZOOM_CTRL_OUT, TRUE);
    return NVTEVT_CONSUME;
}

INT32 OnZoomStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* Assume digital zoom is always enabled */
    UI_ZoomControl(UI_ZOOM_CTRL_STOP, TRUE);
    return NVTEVT_CONSUME;
}

INT32 OnExe_Selftimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

INT32 OnExe_Flash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(SysGetFlag(FL_FLASH_MODE) != FLASH_OFF)
        KeyScan_EnableFlashCharging(TRUE);
    else
        KeyScan_EnableFlashCharging(FALSE);

    ImgCapture_SetParameter(_FlashMode, (UINT8)Get_FlashValue());
    return NVTEVT_CONSUME;
}
INT32 OnExe_Ledlight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    if (paramNum == 0)
    {
        if (GPIOMap_IsFloodLightOn())
        {
            GPIOMap_TurnOffFloodLight();
        }else
        {
            GPIOMap_TurnOnFloodLight();
        }
    }else
    {
        if (uhSelect)
        {
            GPIOMap_TurnOnFloodLight();
        }else
        {
            GPIOMap_TurnOffFloodLight();
        }
    }
    return NVTEVT_CONSUME;
}
INT32 OnExe_Quality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_Quality,(UINT8)Get_QualityValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_Size(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2011/08/23#KS Hung -begin
    if (Primary_GetCurrentMode() == PRIMARY_MODE_MOVIE)
    {
        return NVTEVT_CONSUME;
    }
    //#NT#2011/08/23#KS Hung -end

    ImgCapture_SetParameter(_ImgSize, (UINT8)Get_SizeValue());

    switch(SysGetFlag(FL_PHOTO_SIZE))
    {
    case PHOTO_SIZE_2MHD:
    case PHOTO_SIZE_7MHD:
        UI_SetParameter(_ImageRatio, _Ratio_16_9);
        break;
    default:
        UI_SetParameter(_ImageRatio, _Ratio_4_3);
        break;
    }
    return NVTEVT_CONSUME;
}

INT32 OnExe_SceneMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_SceneMode, (UINT8)Get_SceneModeValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_EV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_AEEV,(UINT8)Get_EVValue());

    return NVTEVT_CONSUME;
}

INT32 OnExe_Metering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_AEMode,(UINT8)Get_MeteringValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_WB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_WBMode,(UINT8)Get_WBValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_ISO(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_ISOIndex,(UINT8)Get_ISOValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_Color(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_ImageEffect,(UINT8)Get_ColorValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_Saturation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_Saturation,(UINT8)Get_SaturationValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_Sharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_Sharpness,(UINT8)Get_SharpnessValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_FD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 index = 0;
    //use index instead off  (SysGetFlag(FL_FD))
    //because FlowPhoto_ForceLockFdSd not set FL_FD
    if (paramNum > 0)
    {
        index = paramArray[0];
    }

    /* Lock/Unlock FD & SD */
    #if (UI_STYLE == UI_STYLE_DRIVE)
    switch(index)//(SysGetFlag(FL_FD))
    {
        case FD_OFF:
            Photo_LockFD(LOCK);
            Photo_LockSD(LOCK);
            ImgCapture_SetParameter(_FaceDetectionMode,(UINT8)_FaceDetectionMode_Off);
            ImgCapture_SetParameter(_SmileDetectionMode,(UINT8)_SmileDetectionMode_Off);
            break;
        case FD_ON:
            Photo_LockFD(UNLOCK);
            Photo_LockSD(LOCK);
            ImgCapture_SetParameter(_FaceDetectionMode,(UINT8)_FaceDetectionMode_On);
            ImgCapture_SetParameter(_SmileDetectionMode,(UINT8)_SmileDetectionMode_Off);
            break;
        case FD_SMILE:
            Photo_LockFD(UNLOCK);
            Photo_LockSD(UNLOCK);
            ImgCapture_SetParameter(_FaceDetectionMode,(UINT8)_FaceDetectionMode_On);
            ImgCapture_SetParameter(_SmileDetectionMode,(UINT8)_SmileDetectionMode_On);
            break;
        default:
            Photo_LockFD(LOCK);
            Photo_LockSD(LOCK);
            ImgCapture_SetParameter(_FaceDetectionMode,(UINT8)_FaceDetectionMode_Off);
            ImgCapture_SetParameter(_SmileDetectionMode,(UINT8)_SmileDetectionMode_Off);
            break;
    }
    #else
    switch(index)//(SysGetFlag(FL_FACE))
    {
        case FACE_DET_ON:
            Photo_LockFD(UNLOCK);
            ImgCapture_SetParameter(_FaceDetectionMode,(UINT8)_FaceDetectionMode_On);
            break;
        case FACE_DET_OFF:
            Photo_LockFD(LOCK);
            Photo_LockSD(LOCK);
            ImgCapture_SetParameter(_FaceDetectionMode,(UINT8)_FaceDetectionMode_Off);
            ImgCapture_SetParameter(_SmileDetectionMode,(UINT8)_SmileDetectionMode_Off);
            break;
        default:
            Photo_LockFD(LOCK);
            Photo_LockSD(LOCK);
            ImgCapture_SetParameter(_FaceDetectionMode,(UINT8)_FaceDetectionMode_Off);
            ImgCapture_SetParameter(_SmileDetectionMode,(UINT8)_SmileDetectionMode_Off);
            break;
    }
    #endif
    return NVTEVT_CONSUME;
}

INT32 OnExe_SD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

INT32 OnExe_AntiShake(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_StillAntiShaking,(UINT8)Get_AntiShakeValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_QuickReview(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

INT32 OnExe_DateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

INT32 OnExe_ContinueShot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

INT32 OnExe_PhotoSocialNetwork(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    debug_ind(("OnExe_PhotoSocialNetwork: uiSelect %d\r\n", uiSelect));

    //#NT#2011/08/17#KS Hung -begin
    switch (uiSelect)
    {
    case PHOTO_VIVILINK_FACEBOOK:
        SysSetSocialNetworkData(VIVILINK_FACEBOOK);
        break;
    case PHOTO_VIVILINK_TWITGOO:
        SysSetSocialNetworkData(VIVILINK_TWITGOO);
        break;
    case PHOTO_VIVILINK_TWITPIC:
        SysSetSocialNetworkData(VIVILINK_TWITPIC);
        break;
    case PHOTO_VIVILINK_PHOTOBUCKET:
        SysSetSocialNetworkData(VIVILINK_PHOTOBUCKET);
        break;
    case PHOTO_VIVILINK_FLICKR:
        SysSetSocialNetworkData(VIVILINK_FLICKR);
        break;
    case PHOTO_VIVILINK_MYSPACE:
        SysSetSocialNetworkData(VIVILINK_MYSPACE);
        break;
    case PHOTO_VIVILINK_PICASA_ALBUMS:
        SysSetSocialNetworkData(VIVILINK_PICASA_ALBUMS);
        break;
    case PHOTO_VIVILINK_NONE:
        SysSetSocialNetworkData(VIVILINK_NONE);
        break;
    }
    //#NT#2011/08/17#KS Hung -end

    return NVTEVT_CONSUME;
}

EVENT_ENTRY UIPhotoObjCtrlCmdMap[] =
{
    {NVTEVT_CAPTURE,                OnCapture},
    {NVTEVT_ZOOMIN,                 OnZoomIn},
    {NVTEVT_ZOOMOUT,                OnZoomOut},
    {NVTEVT_ZOOMSTOP,               OnZoomStop},
    {NVTEVT_EXE_SELFTIMER,          OnExe_Selftimer},
    {NVTEVT_EXE_FLASH,              OnExe_Flash},
    {NVTEVT_EXE_LEDLIGHT,           OnExe_Ledlight},
    {NVTEVT_EXE_QUALITY,            OnExe_Quality},
    {NVTEVT_EXE_SIZE,               OnExe_Size},
    {NVTEVT_EXE_CONTINUESHOT,       OnExe_ContinueShot},
    {NVTEVT_EXE_SCENEMODE,          OnExe_SceneMode},
    {NVTEVT_EXE_EV,                 OnExe_EV},
    {NVTEVT_EXE_WB,                 OnExe_WB},
    {NVTEVT_EXE_METERING,           OnExe_Metering},
    {NVTEVT_EXE_ISO,                OnExe_ISO},
    {NVTEVT_EXE_COLOR,              OnExe_Color},
    {NVTEVT_EXE_SATURATION,         OnExe_Saturation},
    {NVTEVT_EXE_SHARPNESS,          OnExe_Sharpness},
    {NVTEVT_EXE_FD,                 OnExe_FD},
    {NVTEVT_EXE_QUICKREVIEW,        OnExe_QuickReview},
    {NVTEVT_EXE_ANTISHAKING,        OnExe_AntiShake},
    {NVTEVT_EXE_DATEIMPRINT,        OnExe_DateImprint},
    {NVTEVT_EXE_SOCIALNETWORK,      OnExe_PhotoSocialNetwork},
    {NVTEVT_NULL,                   0},  //End of Command Map
};

VControl UIPhotoObjCtrl={
    "UIPhotoObj",
    UIPhotoObjCtrlCmdMap,
    0,
    CTRL_SYS,
    0,  //data
    0,
    0,
    {0,0,0,0},
    0,
    0
};



