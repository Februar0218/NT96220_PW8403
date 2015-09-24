#ifndef _CALLENS_H
#define _CALLENS_H

#include "PStore.h"

#define PS_CAL_LENS                         MAKE_PS_SECNAME_ID('L', 'E', 'N', 'S')

#if (_LENSLIB_ == _LENSLIB_DZ5311_)
#define CALLENS_ZOOM_SECTION_NUM            25
#elif (_LENSLIB_ == _LENSLIB_ST8003_)
#define CALLENS_ZOOM_SECTION_NUM            7
#elif (_LENSLIB_ == _LENSLIB_ST8005_)
#define CALLENS_ZOOM_SECTION_NUM            3
#elif (_LENSLIB_ == _LENSLIB_A300_)
#define CALLENS_ZOOM_SECTION_NUM            11
#else
#define CALLENS_ZOOM_SECTION_NUM            1
#endif

typedef struct {
    BOOL        bFarAdjust;
    BOOL        bNearAdjust;
    INT32       Far_Focus[CALLENS_ZOOM_SECTION_NUM];
    INT32       Near_Focus[CALLENS_ZOOM_SECTION_NUM];
    UINT32      uiLensFocusBL;
    UINT32      uiLensZoomBL;
    INT32       Far_Focus_Small[CALLENS_ZOOM_SECTION_NUM];
    INT32       Near_Focus_Small[CALLENS_ZOOM_SECTION_NUM];
    BOOL        bFocusStepMissTest;
    UINT32      uiZoomMissStep;
    UINT32      uiFocusMissStep;
} CALLENS_INFO, *PCALLENS_INFO;

typedef enum {
    CALLENS_STA_OK,
    CALLENS_STA_FAIL
} _CALLENS_STATUS_;

extern volatile CALLENS_INFO gCalLens_Info;

extern void     Cal_AFFar(void);
extern void     Cal_AFNear(void);
extern void     Cal_FocusOffset(void);
extern void     Cal_LENS(void);
extern void     Cal_MShutterSWTest(void);
extern void     Cal_ApertureSWTest(void);
extern void     Cal_FAETest(void);
extern void     Cal_ZoomSpeedTest(void);
extern void     Cal_ZoomStepMissTest(void);
extern void     Cal_ZoomStop2PR(void);
extern void     Cal_FocusStepMissTest(void);
extern void     Cal_AFTest(void);

extern UINT32   CalLens_GetFocusBL(void);
extern UINT32   CalLens_GetZoomBL(void);
extern INT32    CalLens_ReadCalData(void);
extern INT32    CalLens_WriteCalData(void);
extern BOOL     CalLens_IsTestMissStep(UINT32 *pZoomMissStep, UINT32 *pFocusMissStep);
extern BOOL     CalLens_IsFarAdjust(void);
extern BOOL     CalLens_IsNearAdjust(void);
extern INT32    CalLens_GetFarFocusStep(UINT32 zoomSection, BOOL bCompensate);
extern INT32    CalLens_GetNearFocusStep(UINT32 zoomSection, BOOL bCompensate);

#endif // EOF
