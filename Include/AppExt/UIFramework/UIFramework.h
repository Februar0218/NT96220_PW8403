/**
    @file       UIFramework.h
    @ingroup    mIAPPExtUIFrmwk

    @brief      UI framework header file
                Declare data and function prototype of UI framework

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef UIFRAMEWORK_H
#define UIFRAMEWORK_H

#include "Type.H"
#include "NVTReturn.h"
#include "MessageElement.h"


/** @addtogroup mIAPPExtUIFrmwk

UI framework provides some data structures as window object to define
the behavior. Programmers can create instances based on the data type
and implement the object content. Using UI framework APIs is able to
implement the UI flow with UI framework task. For detail of the usage
of UI framework, please refer to NDK UI framework programming guide.


UI background provides a backgound task for running long time jobs.
Programmers define a table of pairs of events and functions.
The table is set with SetBackgroundExecFuncTable().
When a long time function tends to be run, use Ux_BackgroundPostEvent()
starting the function. A NVTEVT_BACKGROUND_DONE event is issued
after the function finished.


*/
//@{


typedef INT32 COMMONFLAG;

typedef enum
{
    NVTEVT_PASS = 0,
    NVTEVT_CONSUME,
    NVTEVT_FLOW_MAX
}NVTEVT_FLOW;

// indicate a dispaly area
typedef struct _Ux_RECT
{
    INT16  x1;
    INT16  y1;
    INT16  x2;
    INT16  y2;
} Ux_RECT, *PUx_RECT;
/**
@name Window Control Data Structures
@{
*/



/**
    @brief VControl Object Prototype

    VControl defines the behavior of a component
*/
typedef struct _VControl
{
    char         *Name;                             ///< Name of this object. Used for debug and identification.
    struct _EVENT_ENTRY  *EventList;           ///< An array of EVENT_ENTRY arrays (msg handles)
    struct _ITEM_BASE   **ShowList;                  ///< Tool would generate show obj in list
    UINT32       wType;                             ///< Control type
    void         *CtrlData;                         ///< Depend on control type,there is different data structure
    struct _VControl **ControlList;                 ///< An array of (VControl *)
    struct _VControl *pParent;                      ///< Pointer to contorl parent
    Ux_RECT rect;                                ///< The original point of the control (usually 0,0)
    COMMONFLAG flag;                                ///< if dirty -> post NVTEVT_REDRAW
    void        *usrData;                           ///< user extended data
} VControl;

/** Event handler function prototype */
typedef INT32 (* NVTEVT_EVENT_FUNC_PTR)(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

/**
    @brief Command mapping table entry
*/
typedef struct _EVENT_ENTRY
{
   NVTEVT event;                              ///< The handled event
   NVTEVT_EVENT_FUNC_PTR pfn;                   ///< The event handler of the event
} EVENT_ENTRY;

//@}

#define UX_ROOT_WND_IDX         0
#define UX_2NDLAYER_WND_IDX     1
#define UX_3RDLAYER_WND_IDX     2

extern NVTRET Ux_OpenWindow(VControl *pCtrl, UINT32 paramNum, ...);
extern NVTRET Ux_CloseWindow(VControl *pCtrl, UINT32 paramNum, ...);
extern NVTRET Ux_CloseWindowClear(VControl *pCtrl, UINT32 paramNum, ...);
extern NVTRET Ux_GetRootWindow(VControl **pRootCtrl);
extern NVTRET Ux_GetParentWindow(VControl *pCtrl, VControl **pParentWnd);
extern NVTRET Ux_GetFocusedWindow(VControl **ppFocusWnd);
extern NVTRET Ux_GeWindowByIndex(VControl **ppWnd, UINT8 index);


/**
@name Display Control Data Structures
@{
*/


typedef UINT32 UIScreen;

/**
    @brief Rendering Flow Callback
*/
typedef struct _UIRender
{
    UIScreen (*pfn_BeginScreen)(void);                                        ///< Begin screen function, need to return ScreenObj
    void (*pfn_EndScreen)(UIScreen ScreenObj);                                ///< End screen function
    void (*pfn_BeginWindow)(VControl* pCtrl, UIScreen ScreenObj);              ///< Begin window function
    void (*pfn_EndWindow)(VControl* pCtrl, UIScreen ScreenObj);                ///< End window function
    void (*pfn_BeginControl)(VControl* pCtrl, VControl* pControl, UIScreen ScreenObj);  ///< Begin control function
    void (*pfn_EndControl)(VControl* pCtrl, VControl* pControl, UIScreen ScreenObj);    ///< End control function
    void (*pfn_ClearScreen)(UIScreen ScreenObj);                              ///< Clear screen function. May called by CloseWindowClear().
}
UIRender;

//@}


#define ONEVENT_PARAM_INDEX_SCREEN    0
//#define ONEVENT_PARAM_INDEX_PVAREA    1


extern void UxCtrl_SetAllChildShow(VControl *pCtrl, BOOL bShow);
extern void UxCtrl_SetShow(VControl *pCtrl, BOOL bShow);
extern BOOL UxCtrl_IsShow(VControl* pCtrl);
extern void UxCtrl_SetPos(VControl *pCtrl, Ux_RECT rect);
extern Ux_RECT UxCtrl_GetPos(VControl *pCtrl);
extern void UxCtrl_SetDirty(VControl *pCtrl, BOOL bDirty);
extern BOOL UxCtrl_IsDirty(VControl* pCtrl);

extern void UxCtrl_SetChildDirty(VControl *pCtrl, INT32 nChildIndex, BOOL bDirty);

extern VControl *UxCtrl_GetChild(VControl *pCtrl,UINT32 index);
extern void UxCtrl_SetChild(VControl *pParent,UINT32 index,VControl *pCtrl);
extern void UxCtrl_SetLock(VControl *pCtrl,BOOL bLock);
extern VControl *UxCtrl_GetLock(void);
extern VControl *UxCtrl_GetLeafFocus(VControl *pCtrl);
extern void UxCtrl_SetShowTable(VControl *pCtrl, struct _ITEM_BASE   **showTable);
extern struct _ITEM_BASE   **UxCtrl_GetShowTable(VControl *pCtrl);
extern void UxCtrl_SetDataTable(VControl *pCtrl,void *DataTable);
extern void *UxCtrl_GetDataTable(VControl *pCtrl);



/**
@name UI Background Data Structures
@{
*/

/** Background execution function prototype */
typedef UINT32 (* NVT_BKG_FUNC_PTR)(void);

/**
   @brief UI Background job execution table entry
*/
typedef struct _BACKGROUND_JOB_ENTRY
{
    NVTEVT              event;         ///< The event to start the function
    NVT_BKG_FUNC_PTR    pfn;             ///< The pointer of the long time function
} BACKGROUND_JOB_ENTRY;
//@}

#define ONDONE_PARAM_INDEX_CMD     0
#define ONDONE_PARAM_INDEX_RET     1

extern BOOL     Ux_GetBackgroundTskBusy(void);
extern void     Ux_ResetBackgroundTsk(void);
extern void     Ux_SetBackgroundExecFuncTable(BACKGROUND_JOB_ENTRY *pBackgroundFuncTbl);
extern NVTEVT   Ux_GetBackgroundTskExeuteCommand(void);

//@}


/**
@name UI Framework Open Data Structures
@{
*/

typedef struct _UIFRAMEWORK_OBJ
{
    ID UIFrameworkTaskID;                           ///< UI framework task ID
    ID UIBackgroundTaskID;                          ///< UI background task ID
    ID UIFrameworkFlagID;                           ///< Event flag group ID for UI framework task
    ID UIFrameworkWindowSemID;                      ///< Semaphore ID for window open/close
    ID UIFrameworkDrawSemID;                        ///< Semaphore ID for draw function
    BACKGROUND_JOB_ENTRY *pDefaultBackgroundFuncTbl;    ///< Default UI background function table. set to NULL if not used
} UIFRAMEWORK_OBJ;

/**
@}
*/
extern INT32 Ux_SendEvent(VControl *pCtrl, NVTEVT evt,  UINT32 paramNum, ...);
extern INT32 Ux_DefaultEvent(VControl *pCtrl, NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);
extern void Ux_InitMessage(ID UIFrameworkMbxID, ID UIBackgroundMbxID);
extern BOOL Ux_IsInitMessage(void);
extern void Ux_SetRender(UIRender* pUIRender);
extern NVTRET Ux_Redraw(void);
extern NVTRET Ux_RedrawAllWind(void);
extern void Ux_PostEvent(NVTEVT evt, UINT32 paramNum, ...);
extern void Ux_WaitEvent(NVTEVT *evt, UINT32 *paramNum, UINT32 *paramArray);
extern void Ux_FlushEvent(void);
extern void Ux_BackgroundPostEvent(NVTEVT evt);
extern void Ux_BackgroundWaitEvent(NVTEVT *evt);
extern void Ux_BackgroundFlushEvent(void);

extern ER Ux_Open(UIFRAMEWORK_OBJ *param);
extern void Ux_DumpStatus(void);
extern UINT8 *Ux_GetUIVersion(void);
#endif
