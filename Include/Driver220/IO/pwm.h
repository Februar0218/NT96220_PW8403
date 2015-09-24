/**
    Header file of PWM module driver

    This file is the header file of PWM module

    @file       PWM.h
    @ingroup    mIIOPWM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _PWM_H
#define _PWM_H

#include "Type.h"

/**
    @addtogroup mIIOPWM
*/
//@{

/**
    PWM configuration structure
*/
typedef struct
{
    UINT32  uiDiv;      ///< Clock divider, PWM clock = 3MHZ / (uiDiv + 1), 0 ~ 0xFFF
    UINT32  uiPrd;      ///< How many PWM clock per period, 0 ~ 0xFF
                        ///< @note uiRise <= uiFall <= uiPrd
    UINT32  uiRise;     ///< Rising at which clock
                        ///< @note uiRise <= uiFall <= uiPrd
    UINT32  uiFall;     ///< Falling at which clock
                        ///< @note uiRise <= uiFall <= uiPrd
    UINT32  uiOnCycle;  ///< Output cycle, 0 ~ 0x3FF
                        ///< - @b PWM_FREE_RUN: Free Run
                        ///< - @b Others: How many cycles (PWM will stop after output the cycle counts)
    UINT32  uiInv;      ///< Invert PWM output signal or not
                        ///< - @b PWM_SIGNAL_NORMAL: Don't invert PWM output signal
                        ///< - @b PWM_SIGNAL_INVERT: Invert PWM output signal
} PWM_CFG, *PPWM_CFG;

/**
    PWM CCNT configuration structure
*/
typedef struct
{
    UINT32  uiFilter;   ///< Filter, 0 ~ 7 (1 ~ 8 clock), signal less than this filter will be ignored
    UINT32  uiInv;      ///< Invert CCNT input signal or not
                        ///< - @b PWM_CCNT_SIGNAL_NORMAL: Don't invert CCNT input signal
                        ///< - @b PWM_CCNT_SIGNAL_INVERT: Invert CCNT input signal
    UINT32  uiMode;     ///< CCNT mode
                        ///< - @b PWM_CCNT_MODE_PULSE: Counting pulse
                        ///< - @b PWM_CCNT_MODE_EDGE: Counting edge, both rising and falling edges
} PWM_CCNT_CFG, *PPWM_CCNT_CFG;

/**
    MSTP configuration structure
*/
typedef struct
{
    UINT32  uiDiv;    ///< Clock divider, PWM clock = 3MHZ / (uiDiv + 1), 0 ~ 0xFF
    UINT32  uiCh;     ///< Mirco step channel type ,0: A , 1: B ,2: ~A ,3: ~B
    UINT32  uiPh;     ///< Micro step starting phase ,phase 0 ~ 7
    UINT32  uiDir;    ///< Micro step moving direction ,0: Counterclockwise , 1: Clockwise
    UINT32  uiOnCycle;///< Number of phase for Micro step mode
} MSTP_CFG, *PMSTP_CFG;

// For old version PWM driver
#define PWM_struct              PWM_CFG     ///< For old version PWM driver, don't use anymore
#define PPWM_struct             PPWM_CFG    ///< For old version PWM driver, don't use anymore

#define PWM_FREE_RUN            0           ///< For pwm_set() API, output cycle is free run

/**
    @name PWM output signal mode

    Invert PWM output signal or not for pwm_set() API
*/
//@{
#define PWM_SIGNAL_NORMAL       0           ///< Don't invert PWM output signal
#define PWM_SIGNAL_INVERT       1           ///< Invert PWM output signal
//@}

/**
    @name CCNT input signal mode

    Invert CCNT input signal or not for pwm_setCCNT() API
*/
//@{
#define PWM_CCNT_SIGNAL_NORMAL  0           ///< Don't invert CCNT input signal
#define PWM_CCNT_SIGNAL_INVERT  1           ///< Invert CCNT input signal
//@}

/**
    @name PWM CCNT mode

    PWM CCNT mode for pwm_setCCNT() API
*/
//@{
#define PWM_CCNT_MODE_EDGE         0           ///< Counting pulse
#define PWM_CCNT_MODE_DUALEDGE     1           ///< Counting edge, both rising and falling edges
//@}

/**
    @name PWM ID

    PWM ID for PWM driver API
*/
//@{
#define PWMID_0                 0x00000001  ///< PWM ID 0
#define PWMID_1                 0x00000002  ///< PWM ID 1
#define PWMID_2                 0x00000004  ///< PWM ID 2
#define PWMID_CCNT              0x00000008  ///< PWM ID CCNT
//@}

/**
    @name CCNT EVENT

    CCNT Event for PWM driver API
*/
//@{
typedef void (*CCNTCallbackFunc)(UINT32 EventID);
#define PWM_CCNT_EACHDET        0x00010000  ///< CCNT each detection event
#define PWM_CCNT_TRIG           0x00020000  ///< CCNT trigger hit event
#define PWM_CCNT_TOUT           0x00040000  ///< CCNT time out event
//@}

//#NT#2012/01/03#Chris Hsu -begin
//#NT#Export more PWM APIs (pwm_reload, pwm_delayMs, pwm_delayUs) and correct naming of pwm_stopCCNT
// PWM Driver API
extern ER       pwm_open(UINT32 uiPWMId);
extern ER       pwm_close(UINT32 uiPWMId);

extern ER       pwm_set(UINT32 uiPWMId, PPWM_CFG pPWMCfg);
extern ER       pwm_reload(UINT32 uiPWMId);

extern ER       pwm_en(UINT32 uiPWMId);
extern ER       pwm_stop(UINT32 uiPWMId);
extern ER       pwm_wait(UINT32 uiPWMId);

extern ER       pwm_setCCNT(PPWM_CCNT_CFG pCCNTCfg);
extern ER       pwm_reloadCCNT(void);

extern ER       pwm_enCCNT(void);
extern ER       pwm_stopCCNT(void);

extern ER       pwm_setCCNTCntDir(BOOL bDecrease);
extern ER       pwm_setCCNTStartVal(UINT32 uival);
extern ER       pwm_setCCNTTrigVal(UINT32 uiTrigval);
extern UINT32   pwm_getCCNTCurrVal(void);
extern ER       pwm_setCCNTHdl(CCNTCallbackFunc pIsrHdl);

extern ER       pwm_delayMs(UINT32 uiPwmID, UINT32 uiMs);
extern ER       pwm_delayUs(UINT32 uiPwmID, UINT32 uiUs);
//#NT#2012/01/03#Chris Hsu -end

//@}

#endif
