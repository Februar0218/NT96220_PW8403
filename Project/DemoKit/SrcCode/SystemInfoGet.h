/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemInfoGet.h
    @ingroup    GetSystemInfo

    @brief      Header file for Getting System Informaiton
                This file is the header file for getting System Informaiton

    @note       Nothing.

    @version    V1.00.000
    @author     Shirley Cheng
    @date       2005/12/20
*/

/** \addtogroup GetSystemInfo */

//@{

#ifndef _SYSTEMINFOGET_H
#define _SYSTEMINFOGET_H

#include "SysCfg.h"

#if (_SYSINFO_MODE_ == ENABLE)

// Extern function
extern void SYSInfo_GetSystemInfo(void);

#endif

#endif
//@}
