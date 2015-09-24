/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemTask.c
    @ingroup    mIPRJAPCfg

    @brief      Start system task
                Start system task (KeyScan, Command, Lens, PlaySound...)

    @note       Nothing.

    @date       2005/12/28
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#include "SystemTask.h"
#include "UIFramework.h"
#include "UIBackgroundObj.h"

/**
  Start phase 1 tasks

  Start phase 1 tasks.
  Now are the tasks that should start before HW / FW initialization.

  @param void
  @return void
*/
void SysTask_StartTaskPhase1(void)
{
    // Start lens control task and play sound task...
//    LensCtrl_Open(NULL);
}

/**
  Start phase 2 tasks

  Start phase 2 tasks.
  Now are the tasks that should start after HW / FW initialization.

  @param void
  @return void
*/
void SysTask_StartTaskPhase2(void)
{
    UIFRAMEWORK_OBJ uiFrameworkObj;
    
    // Start KeyScan task and Command Task
    sta_tsk(KEYSCANTSK_ID);
#if (_COMMANDTSK_MODE_ == ENABLE)
    #if (_GPS_RECEIVER_ == ENABLE)
    #else
    sta_tsk(COMMANDTSK_ID);
    #endif
#endif

    uiFrameworkObj.UIFrameworkTaskID      = UIFRAMEWORKTSK_ID;
    uiFrameworkObj.UIBackgroundTaskID     = UIBACKGROUNDTSK_ID;
    uiFrameworkObj.UIFrameworkFlagID      = FLG_ID_FRAMEWORK;
    uiFrameworkObj.UIFrameworkWindowSemID = SEMID_FRAMEWORK_WINDOW;
    uiFrameworkObj.UIFrameworkDrawSemID   = SEMID_FRAMEWORK_DRAW;
    uiFrameworkObj.pDefaultBackgroundFuncTbl = gBackgroundExtFuncTable;
    Ux_Open(&uiFrameworkObj);
}

//@}
