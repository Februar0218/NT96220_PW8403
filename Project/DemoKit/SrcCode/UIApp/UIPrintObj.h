#ifndef UI_PRINTOBJ_H
#define UI_PRINTOBJ_H
//-------------------------------------------------------------------------------------------------
#include "Debug.h"
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "KeyScanTsk.h"
//-------------------------------------------------------------------------------------------------
#define UIPRINT_DEBUG_MSG   0
#define UIPRINT_DEBUG_ERR   1
//-------------------------------------------------------------------------------------------------
#if (UIPRINT_DEBUG_MSG&DEBUG_LEVEL)
#define UIPRINT_MSG(msg)    debug_msg msg
#else
#define UIPRINT_MSG(msg)
#endif

#if (UIPRINT_DEBUG_ERR&DEBUG_LEVEL)
#define UIPRINT_ERR(msg)    debug_msg msg
#else
#define UIPRINT_ERR(msg)
#endif
//-------------------------------------------------------------------------------------------------
#define UIPRINT_SUPPORT_ABORT_JOB       1
#define UIPRINT_SUPPORT_CONTINUE_JOB    1
//-------------------------------------------------------------------------------------------------
#define UIPRINT_KEYMAP_NONE         0
#define UIPRINT_KEYMAP_ENTER        1
#define UIPRINT_KEYMAP_UP           2
#define UIPRINT_KEYMAP_DOWN         3
#define UIPRINT_KEYMAP_LEFT         4
#define UIPRINT_KEYMAP_RIGHT        5
#define UIPRINT_KEYMAP_ZOOMIN       6
#define UIPRINT_KEYMAP_ZOOMOUT      7
#define UIPRINT_KEYMAP_MENU         8
#define UIPRINT_KEYMAP_SHUTTER2     9

#define UIPRINT_KEY_ENTER           UIPRINT_KEYMAP_ENTER
#define UIPRINT_KEY_UP              UIPRINT_KEYMAP_UP
#define UIPRINT_KEY_DOWN            UIPRINT_KEYMAP_DOWN
#define UIPRINT_KEY_LEFT            UIPRINT_KEYMAP_LEFT
#define UIPRINT_KEY_RIGHT           UIPRINT_KEYMAP_RIGHT
#define UIPRINT_KEY_ZOOMIN          UIPRINT_KEYMAP_NONE
#define UIPRINT_KEY_ZOOMOUT         UIPRINT_KEYMAP_NONE
#define UIPRINT_KEY_MENU            UIPRINT_KEYMAP_MENU
#define UIPRINT_KEY_SHUTTER2        UIPRINT_KEYMAP_NONE
//-------------------------------------------------------------------------------------------------
#define UIPRINT_KEYACT_NONE        0x00
#define UIPRINT_KEYACT_PRESS       0x01
#define UIPRINT_KEYACT_RELEASE     0x02

#define UIPRINT_KEYACT_TYPE        UIPRINT_KEYACT_PRESS
//-------------------------------------------------------------------------------------------------
extern VControl UIPrintObjCtrl;
extern BOOL UIPrint_GetDPOFExist(void);
extern UINT32 UIPrint_GetDCFFileAmount(void);
extern UINT32 UIPrint_GetPaperType(void);
extern UINT32 UIPrint_Getlayout(void);
extern UINT32 UIPrint_Playback_GetCurrWidth(void);
extern UINT32 UIPrint_Playback_GetCurrHeight(void);
extern UINT16 UIPrint_Playback_GetDirId(void);
extern UINT16 UIPrint_Playback_GetFileId(void);
extern void   UIPrint_Playback_Open(void);
//-------------------------------------------------------------------------------------------------
#endif

