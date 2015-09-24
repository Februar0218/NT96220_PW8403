/**
    @file       soft_timer.h
    @ingroup    mIHALSoftTimer
    @brief      The software timer module
                This module use a hardware timer to simulate software timers.

    Copyright (C) 2005 By Novatek Microelectronics Corp.
*/


#ifndef _SOFT_TIMER_H
#define _SOFT_TIMER_H

#include "Type.h"
#include "timer.h"


typedef void (*fnStimerCb)(UINT param);

#define FnStimerCB fnStimerCb

int     stimer_open(UINT *pStimerId, UINT FlagID, UINT WhichFlag);
int     stimer_openCB(UINT *pStimerId, FnStimerCB EventHandler, UINT CarryParam);
int     stimer_set(UINT Id, UINT uiIntval, UINT uiMode, UINT uiTmPause);
int     stimer_pausePlay(UINT Id, UINT uiTmPause);
int     stimer_close(UINT Id);
int     stimer_delay(UINT Time);
int     stimer_getStatus(UINT Id);


#define     STIMER_ALL_ID       (-1)
#define     STIMER_PAUSE        _TIMER_PAUSE
#define     STIMER_PLAY         _TIMER_PLAY

#define     STIMER_INVALID_ID       0x00000000
#define     STIMER_STA_OPENED       0x10000000  //
#define     STIMER_STA_ACTTING      0x20000000

#define     STIMER_STA_ONE_SHOT     0x00000000  //bit0 one-shot or free-run mode
#define     STIMER_STA_FREE_RUN     0x00000001

// Flag pattern
//#define FLGSTIMER_LEAVE_GETFLAG 0x80000000
//#define FLGSTIMER_IN_GETFLAG    0x40000000
#define FLGSTIMER_ACCEPT_CMD    0x80000000
// note: the other flags are applied in soft_timer internal
// the others should not use this flag group

#endif // _SOFT_TIMER_H
