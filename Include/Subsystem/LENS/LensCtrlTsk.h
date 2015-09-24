/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       LensCtrlTsk.h
    @ingroup    mISYSLens

    @brief      Lens control task functions header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2011/07/13
*/

#ifndef _LENSCTRLTSK_H
#define _LENSCTRLTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"
#include "Lens.h"
#include "OSCommon.h"

//
//FLG_ID_LENS
//
#define FLGLENS_ZOOMFWD                 0x00000001
#define FLGLENS_ZOOMREV                 0x00000002
#define FLGLENS_ZOOMBRK                 0x00000004
#define FLGLENS_ZOOMOFF                 0x00000008
#define FLGLENS_ZOOMCHANGE              0x00000010
#define FLGLENS_FOCUSFWD                0x00000020
#define FLGLENS_FOCUSREV                0x00000040
#define FLGLENS_APERTUREMOVE            0x00000080
#define FLGLENS_SHUTTERMOVE             0x00000100
#define FLGLENS_IDLE                    0x00000200
#define FLGLENS_SHUTTEROFF              0x00000400

#define FLGLENS_FOCUS_CMD_FIN           0x00002000
#define FLGLENS_FOCUS_CMD_FIN2          0x00004000
#define FLGLENS_ZOOM_CMD_FIN            0x00008000
#define FLGLENS_ZOOM_FOCUS_MOVE_IN      0x00010000
#define FLGLENS_ZOOM_FOCUS_MOVE_OUT     0x00020000

// Wait time
#define LENSCTRL_WAIT_NOWAIT            0x00
#define LENSCTRL_WAIT_INFINITE          0xFF

// key detection during lens control
typedef enum
{
    LENS_KEY_WIDE,
    LENS_KEY_TELE,
    LENS_KEY_SHUTTER,
    ENUM_DUMMY4WORD(LENS_KEY_DET)
} LENS_KEY_DET;

// key action during lens control
typedef enum
{
    LENS_KEY_PRESS,
    LENS_KEY_RELEASE,
    ENUM_DUMMY4WORD(LENS_KEY_ACT)
} LENS_KEY_ACT;

typedef void (*FPLENSCALLBACK)(UINT32 MessageID, AlgMsgInfo * pAlgMsg_infor);
typedef UINT32  (*FP_LENS_KEY_CB)(LENS_KEY_DET Key, LENS_KEY_ACT KeyAct);

typedef struct
{
    PLENS_TAB       pLens;
    PMOTOR_TAB      pMotor;
    FPLENSCALLBACK  fpLensCB;
    //#NT#2011/10/03#Hideo Lin -begin
    //#NT#Use callback instead of GPIO since it may use ADC key
    //UINT32        uiWideKey;
    //UINT32        uiTeleKey;
    FP_LENS_KEY_CB  fpLensKeyCB;
    //#NT#2011/10/03#Hideo Lin -end
} LENSCTRL_APPOBJ, *PLENSCTRL_APPOBJ;

typedef struct
{
    UINT32          uiZoomStep;
    UINT32          uiZoomPPS;
    UINT32          uiZoomDirection;
    BOOL            bWaitZoomFinish;
    BOOL            bOpenZoomPR;
    UINT32          uiFocusStep;
    UINT32          uiFocusPPS;
    UINT32          uiFocusDirection;
    UINT32          uiApertureState;
    UINT32          uiShutterState;
} LENSCTRL_PARAMOBJ, *PLENSCTRL_PARAMOBJ;

extern ER               LensCtrl_Open(PLENSCTRL_APPOBJ pLensCtrlObj);
extern ER               LensCtrl_Close(void);
extern ER               LensCtrl_RegCB(FPLENSCALLBACK cb);
extern ER               LensCtrl_RegKeyCB(FP_LENS_KEY_CB cb);
extern PLENSCTRL_APPOBJ LensCtrl_GetObject(void);
extern void             LensCtrl_GetParam(PLENSCTRL_PARAMOBJ pLensCtrlParamObj);
extern ER               LensCtrl_SetParam(PLENSCTRL_PARAMOBJ pLensCtrlParamObj);
extern ER               LensCtrl_WaitCmdFinish(UINT32 TimeOut);
extern ER               LensCtrl_WaitCmdTypeFinish(UINT32 TimeOut,UINT32 cmd_type);
extern void             LensCtrlTsk(void);


#endif
