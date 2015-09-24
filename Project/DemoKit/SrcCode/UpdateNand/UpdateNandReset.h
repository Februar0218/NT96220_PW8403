/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UpdateNandReset.h
    @ingroup    mIPRJAPInit

    @brief      Header file of UpdateNandReset.c
                Reset CPU and all modules to run the FW.

    @note       Nothing.

    @date       2006/04/17
*/

/** \addtogroup mIPRJAPInit */
//@{

#ifndef _UPDATENANDRESET_H
#define _UPDATENANDRESET_H

#include "Type.h"

extern void     UpdateNand_Reset(UINT32 uiAddress, UINT32 uiFileLen);

#endif

//@}
