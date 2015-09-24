/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyADC.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of key detection by using ADC
                Header file of key detection by using ADC

    @note       Nothing.

    @date       2005/08/24
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _KEYADC_H
#define _KEYADC_H

#include "SysCfg.h"

extern void     KeyADC_Init(void);
extern UINT32   KeyADC_GetKey(void);

#endif

//@}
