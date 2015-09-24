#ifndef _UICONFIG_H
#define _UICONFIG_H

#include "UIFlow.h"

#define UICONFIG_STR_LEN MAX_VER_STR_LEN

#define UI_LOGO_POWERON        0
#define UI_LOGO_POWEROFF       1

typedef struct _UIConfigInfo
{
    /*--- General ---*/
    CHAR        strMakerString[UICONFIG_STR_LEN];
    CHAR        strModelString[UICONFIG_STR_LEN];
    CHAR        strSoftwareVer[UICONFIG_STR_LEN];
    CHAR        strMakeNote[UICONFIG_STR_LEN];
    CHAR        strImageDescription[UICONFIG_STR_LEN];

    UINT32      uiSizeDefaultIndex;
    UINT32      uiRamSizeDefaultIndex;
    UINT32      uiMovieSizeDefaultIndex;
    UINT32      uiLanguageDefaultIndex;
    UINT32      uiAutoPowerOffDefaultIndex;
    UINT32      uiLcdOffDefaultIndex;
    UINT32      uiDelayShutDownDefaultIndex;
    UINT32      uiBeepDefaultIndex;
    UINT32      uiLEDDefaultIndex;//ERIC EDIT 0901
    UINT32      uiCyclicRecDefaultIndex;
    UINT32      uiParkingDefaultIndex;
    UINT32      uiGsensorDefaultIndex;
    UINT32      uiFreqDefaultIndex;
    UINT32      uiAntiShakeDefaultIndex;
    UINT32      uiTVModeDefaultIndex;
    UINT32      uiDateImprintDefaultIndex;
} UIConfigInfo;

extern CHAR *GetUIConfigSoftwareVer(void);
extern CHAR *GetUIConfigMakerString(void);
extern CHAR *GetUIConfigModelString(void);
extern CHAR *GetUIConfigMakeNote(void);
extern CHAR *GetUIConfigImageDescription(void);
extern void UI_SetSizeDefaultIndex(UINT32 index);
extern UINT32 UI_GetSizeDefaultIndex(void);
extern void UI_SetLanguageDefaultIndex(UINT32 index);
extern UINT32 UI_GetLanguageDefaultIndex(void);
extern void UI_SetFreqDefaultIndex(UINT32 index);
extern UINT32 UI_GetFreqDefaultIndex(void);
extern void UI_SetAntiShakeDefaultIndex(UINT32 index);
extern UINT32 UI_GetAntiShakeDefaultIndex(void);
extern void UI_SetTVModeDefaultIndex(UINT32 index);
extern UINT32 UI_GetTVModeDefaultIndex(void);
extern void UI_SetDateImprintDefaultIndex(UINT32 index);
extern UINT32 UI_GetDateImprintDefaultIndex(void);

extern void UI_UpdateCfgFile(void);
extern BOOL UI_LoadCfgFile(void);
extern void UI_UpdateLogoFile(void);
extern BOOL UI_ShowLogoFile(PS_SECNAME_ID psSectID);
extern void UI_ReadNorFlash(void);
#endif
