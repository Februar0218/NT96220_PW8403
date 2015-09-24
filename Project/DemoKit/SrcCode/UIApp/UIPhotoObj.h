#ifndef UI_PHOTOOBJ_H
#define UI_PHOTOOBJ_H

#include "Type.h"
#include "UIFramework.h"
#include "ImageAlgInfor.h"
#include "NVTUserCommand.h"


//-----------------------------------------------------------------------------
// Global Definitions
//-----------------------------------------------------------------------------
// Zoom control
#define UI_ZOOM_CTRL_STOP               0
#define UI_ZOOM_CTRL_IN                 1
#define UI_ZOOM_CTRL_OUT                2
#define UI_ZOOM_CTRL_RESET_DZOOM        3
#define UI_DZOOM_IDX_MIN                10
#define UI_DZOOM_IDX_MAX                40  // 4X digital zoom

// DZoom string
#define MAX_DZOOM_STR_LEN               16

//-----------------------------------------------------------------------------
// Type Define
//-----------------------------------------------------------------------------
// Current zoom interface
typedef enum
{
    ZOOM_IF_OPTICAL = 0,
    ZOOM_IF_DIGITAL,
    ZOOM_IF_TOTAL_NUM
} ZOOM_IF;

//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern VControl UIPhotoObjCtrl;

extern AlgMsgInfo *Get_AlgMsgInfo(void);
extern void     Set_AlgMsgInfo(AlgMsgInfo *pInfo);

extern UINT32   Get_FlashValue(void);
extern UINT32   Get_SizeValue(void);
extern const CHAR *Get_SizeString(UINT32 uiIndex);
extern UINT32   Get_QualityValue(void);
extern UINT32   Get_ColorValue(void);
extern UINT32   Get_SceneModeValue(void);
extern UINT32   Get_EVValue(void);
extern UINT32   Get_MeteringValue(void);
extern UINT32   Get_WBValue(void);
extern UINT32   Get_AntiShakeValue(void);
extern UINT32   Get_ISOValue(void);
extern UINT32   Get_SaturationValue(void);
extern UINT32   Get_SharpnessValue(void);
extern UINT32   Get_QuickReviewValue(void);
extern CHAR     *Get_SelftimerCntString(UINT32 uiCount);

extern void     Set_CurrentZoomIF(UINT32 uiIndex);
extern UINT32   Get_CurrentZoomIF(void);
extern void     Set_DZoomResetting(BOOL bReset);
extern BOOL     Get_DZoomResetting(void);
extern CHAR     *Get_DZoomRatioString(AlgMsgInfo *pInfo);

extern void     UI_ZoomControl(UINT32 uiZoomCtrl, BOOL bDZoomEnable);




#endif
