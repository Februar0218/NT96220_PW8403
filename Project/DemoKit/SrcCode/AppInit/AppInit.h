/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInit.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of AppInit.c
                Header file of AppInit.c

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _APPINIT_H
#define _APPINIT_H

#include "Type.h"
#include "AppInitPhoto.h"
#include "AppInitSetup.h"
#include "AppInitAVI.h"
#include "AppInitMp3.h"
#include "AppInitPlayback.h"
#include "AppInitUSB.h"
#include "AppInitCustom.h"
#include "AppInitSetup.h"
#include "AppInitInitSetup.h"

extern void     AppInit_Open(UINT32 uiMode);
extern void     AppInit_Close(void);

#endif

//@}
