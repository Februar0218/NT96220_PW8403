/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UserGPIO.c
    @ingroup    mIPRJAPKeyIO

    @brief      GPIO mapping for low level library.
                This file does GPIO initialization and GPIO detection for low level library

    @note       These functions prototype are defined by respective low level library

    @version    V1.00.000
    @author     Stephen Wang
    @date       2006/11/06
*/

#include "SysCfg.h"
#include "GPIOMapping.h"
#include "UserGPIO.h"

/** \addtogroup mIPRJAPKeyIO */
//@{

#if (_STORAGECARD_ == _STORAGECARD_SDIO_)
BOOL SDIOCardUser_CheckCardInserted(void)
{
    return GPIOMap_DetStrgCard();
}

BOOL SDIOCardUser_CheckCardWP(void)
{
    return GPIOMap_DetStrgCardWP();
}
#endif

//@}
