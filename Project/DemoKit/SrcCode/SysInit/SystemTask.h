/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemTask.h
    @ingroup    mIPRJAPCfg

    @brief      Header file for start system task
                Header file for start system task (KeyScan, Command, Lens, PlaySound...)

    @note       Nothing.

    @date       2006/12/28
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _SYSTEMTASK_H
#define _SYSTEMTASK_H

#include "OSCommon.h"

extern void         SysTask_StartTaskPhase1(void);
extern void         SysTask_StartTaskPhase2(void);

#endif

//@}
