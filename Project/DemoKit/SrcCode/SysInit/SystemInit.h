/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemInit.h
    @ingroup    mIPRJAPCfg

    @brief      Initialize system
                Initialize system (HW)

    @note       Nothing.

    @date       2005/12/13
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _SYSTEMINIT_H
#define _SYSTEMINIT_H

#include "OSCommon.h"

// Pll2 OSC clock, 12MHz
#define SYSINIT_PLL2_OSC_CLOCK_MHZ  12

// Enable/Disable MCLK ouput
#define SYSINIT_OUTPUT_MCLK         ENABLE
// ENABLE -> Logo will display on Video1, DISABLE -> Logo will display on Video2
#define SYSINIT_LOGO_V1             ENABLE

#if (SYSINIT_OUTPUT_MCLK == ENABLE)
// OSC = 12 MHz, PLL2 output = 12 * LOOPDIV / 2 = 540 MHz
#define SYSINIT_PLL2_LOOPDIV        90
#define SYSINIT_PLL2_PREDIV         2
// 270 / (1 + 3) = 67.5 MHz
#define SYSINIT_MCLKDIV             22  // 3
#else
// OSC = 12 MHz, PLL2 output = 12 * 9 / 1 = 108 MHz
#define SYSINIT_PLL2_LOOPDIV        9
#define SYSINIT_PLL2_PREDIV         1
#endif

extern void         SystemInit(void);
extern void         SysInit_InitLCD(void);
extern INT32    SysInit_GetPStoreStatus(void);
extern void         SysInit_InitPStore(void);
extern void         SysInit_InitLens(void);
extern void SysInit_DisplaySplashScreen(UINT32 uiLogo,UINT8 *pBgBuf, UINT32 uiSize);
extern UINT32   SysInit_GetPStoreSize(void);

extern unsigned char BIT1612M_CVBS_BANK0[252];
extern unsigned char BIT1612M_BANK1[8];
extern unsigned char BIT1612M_ITU656_BANK0[252];
extern unsigned char BIT1216M_CVBS_REG[][2];
extern unsigned char BIT1216M_ITU656_REG[][2];
#endif

//@}
