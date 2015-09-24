/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemClock.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of SystemClock.c
                Header file of SystemClock.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _SYSTEMCLOCK_H
#define _SYSTEMCLOCK_H

#include "Type.h"
#include "Clock.h"

#define SPEED_ID_PHOTO          0
#define SPEED_ID_CAPTURE        1
#define SPEED_ID_VIDEO          2
#define SPEED_ID_PLAYBACK       3
#define SPEED_ID_MP3            4
#define SPEED_ID_USBMSDC        5
#define SPEED_ID_USBSICD        6
#define SPEED_ID_USBPCC         7
#define SPEED_ID_USBMSDCVDRCMD  8

//#NT#2011/07/12#Steven Wang -begin
//#NT#call SysClk_Init instead of clk_open()
extern void SysClk_Init(void);
//#NT#2009/09/30#HH Chuang -end
extern BOOL SysClk_SetClk(UINT32 uiSpeedID);

extern void SysClk_GetDMA80MHzSetting(PDMA_SETTING pDMASetting);
extern void SysClk_GetDMA120MHzSetting(PDMA_SETTING pDMASetting);
extern void SysClk_GetDMA160MHzSetting(PDMA_SETTING pDMASetting);

#endif

//@}
