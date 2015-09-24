/**
    Public header file for timer module.

    This file is the header file that define the API and data type for timer
    module.

    @file       timer.h
    @ingroup    mIHALTimer
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _TIMER_H
#define _TIMER_H

#include "Type.h"

/**
    @addtogroup mIHALTimer
*/
//@{

/**
    Timer ID.

    Timer ID.
*/
typedef enum
{
    TIMER_0,                        ///< Timer ID for Timer 0
    TIMER_1,                        ///< Timer ID for Timer 1
    TIMER_2,                        ///< Timer ID for Timer 2
    TIMER_3,                        ///< Timer ID for Timer 3
    TIMER_4,                        ///< Timer ID for Timer 4
    TIMER_5,                        ///< Timer ID for Timer 5
    TIMER_6,                        ///< Timer ID for Timer 6
    TIMER_7,                        ///< Timer ID for Timer 7
    TIMER_NUM                       ///< Timer number
} TIMER_IDTEMP;

#define TIMER_INVALID   0xFFFFFFFF  ///< Invalid Timer ID

typedef UINT            TIMER_ID;
typedef UINT            *PTIMER_ID;


/**
    Timer mode.

    Timer mode.
*/
typedef enum
{
    TIMER_MODE_CLKSRC_DIV0  = 0x00000000,   ///< Timer clock source is from divider 0
    TIMER_MODE_CLKSRC_DIV1  = 0x00000001,   ///< Timer clock source is from divider 1
    TIMER_MODE_ONE_SHOT     = 0x00000010,   ///< Timer operation mode is "one shot"
    TIMER_MODE_FREE_RUN     = 0x00000020,   ///< Timer operation mode is "free run"
    TIMER_MODE_ENABLE_INT   = 0x00000040,   ///< Enable timeout interrupt
    ENUM_DUMMY4WORD(TIMER_MODE)
} TIMER_MODE, *PTIMER_MODE;

/**
    Timer state.

    Timer state.
*/
typedef enum
{
    TIMER_STATE_PAUSE,              ///< Stop timer
    TIMER_STATE_PLAY,               ///< Start timer
    ENUM_DUMMY4WORD(TIMER_STATE)
} TIMER_STATE, *PTIMER_STATE;

//Timer Driver API
extern ER       timer_open(PTIMER_ID pTimerID, FP EventHandler);
extern ER       timer_close(TIMER_ID TimerID);
extern ER       timer_openAutoClose(PTIMER_ID pTimerID, FP EventHandler);
extern ER       timer_cfg(TIMER_ID TimerID, UINT32 uiIntval, TIMER_MODE TimerMode, TIMER_STATE TimerState);
extern ER       timer_pausePlay(TIMER_ID TimerID, TIMER_STATE TimerState);
extern ER       timer_waitTimeup(TIMER_ID TimerID);
extern ER       timer_checkTimeup(TIMER_ID TimerID, BOOL *pbEn);

//Timer Utility API
extern UINT32   timer_getCurrentCount(TIMER_ID TimerID);
extern UINT32   timer_getTargetCount(TIMER_ID TimerID);
extern UINT32   timer_getCurrHWTimerLockNums(void);

//@}

/**
    @name Backward compatible definitions

    For old version timer driver
*/
//@{
#define timer_set(uiTIMERId, uiIntval, uiMode, uiState)         timer_cfg((uiTIMERId), 1000*(uiIntval), (uiMode), (uiState))
#define timer_set_us(uiTIMERId, uiIntval, uiMode, uiState)      timer_cfg((uiTIMERId), (uiIntval), (uiMode), (uiState))
#define timer_openSpecific(TimerID, EventHandler)               debug_err(("timer_openSpecific() is not supported any more!\r\n"));

#define _TIMER_CTRL_FREE_RUN    TIMER_MODE_FREE_RUN
#define _TIMER_CTRL_ONE_SHOT    TIMER_MODE_ONE_SHOT
#define _TIMER_CTRL_INT_ENABLE  TIMER_MODE_ENABLE_INT

#define _TIMER_PAUSE            TIMER_STATE_PAUSE
#define _TIMER_PLAY             TIMER_STATE_PLAY

#define _TIMER_CLKSRC_DIV0      TIMER_MODE_CLKSRC_DIV0
#define _TIMER_CLKSRC_DIV1      TIMER_MODE_CLKSRC_DIV1
//@}

#endif // _TIMER_H
