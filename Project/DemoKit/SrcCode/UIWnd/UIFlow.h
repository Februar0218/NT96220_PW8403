/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIFlow.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Flow Functions
                This file is the user interface ( for interchange flow control).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UIFLOW_H
#define _UIFLOW_H

#include "SysCfg.h"
//UIApp
#include "UIBackgroundObj.h"
#include "UIPhotoObj.h"
#include "UIMovieObj.h"
#include "UIPlayObj.h"
#include "UISystemObj.h"
#include "UIPrintObj.h"
#include "UIPlayStamp.h"
#include "UILensObj.h"

#if (UI_STYLE == UI_STYLE_DRIVE)
    #include "UIFlow_DRIVE.h"
#endif

//UI Display
#include "UIGraphics.h"
#include "UIResource.h"

#include "Debug.h"

#define TIMER_HALF_SEC               500
#define TIMER_ONE_SEC               1000
#define TIMER_TWO_SEC               2000

extern void     FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo);
extern void     FlowPhoto_SetModeCtrl(void);

extern ER FlowPhoto_Open(void);
extern ER FlowPhoto_Close(void);

extern ER FlowPB_Open(void);
extern ER FlowPB_Close(void);

extern ER FlowMovie_Open(void);
extern ER FlowMovie_Close(void);

extern ER FlowVoice_Open(void);
extern ER FlowVoice_Close(void);

extern ER FlowInitSetup_Open(void);
extern ER FlowInitSetup_Close(void);

extern ER FlowWrnStrgHotPlug_Open(void);
extern ER FlowWrnStrgHotPlug_Close(void);

extern ER MenuUSB_Open(void);
extern ER MenuUSB_Close(void);

extern ER MenuPCC_Open(void);
extern ER MenuPCC_Close(void);

extern ER MenuMSDC_Open(void);
extern ER MenuMSDC_Close(void);
extern ER MenuPrint_Open(void);
extern ER MenuPrint_Close(void);

extern ER MenuSetup_Open(void);
extern ER MenuSetup_Close(void);

extern ER MenuCalibration_Open(void);
extern ER MenuCalibration_Close(void);

#endif
