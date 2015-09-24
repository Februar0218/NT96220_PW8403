/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       UserGPIO.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of UserGPIO.c

    @note       Nothing.

    @date       2007/02/02
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _USERGPIO_H
#define _USERGPIO_H

#include "SysCfg.h"

#if (_STORAGECARD_ == _STORAGECARD_SDIO_)
extern    BOOL    SDIOCardUser_CheckCardInserted(void);
extern    BOOL    SDIOCardUser_CheckCardWP(void);
#endif


#endif

//@}
