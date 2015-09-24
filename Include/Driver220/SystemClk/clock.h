/**
    Change clock APIs header file

    Change clock APIs header.

    @file       clock.h
    @ingroup    mIHALSysCG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _CLOCK_H
#define _CLOCK_H

#include "Type.h"

/**
    @addtogroup mIHALSysCG
*/
//@{

typedef struct
{
    UINT32      uiDMATiming1;
    UINT32      uiDMATiming2;
} DMA_TIMING, *PDMA_TIMING;

/**
    DMA setting

    DMA setting for DMA clock change.

    @note For clk_changeDMA()
*/
typedef struct
{
    UINT32      uiConfig;           ///< Setting for DMA config register
    UINT32      uiTiming0;          ///< Setting for DMA timing 0 register
    UINT32      uiTiming1;          ///< Setting for DMA timing 1 register
    UINT32      uiEngineer;         ///< Setting for DMA engineer register
    UINT32      uiDelayChain;       ///< Setting for DMA delay config register
    UINT32      uiModeReg;          ///< Setting for Mode register on DRAM IC
} DMA_SETTING, *PDMA_SETTING;

typedef struct
{
    UINT32      uiSlewRate_PPPN;
    UINT32      uiSlewRate_DDR;
    UINT32      uiCtlDrvSink_PPPN;
    UINT32      uiConfig_PPPN;
} DMA_DRIVING, *PDMA_DRIVING;

/**
    @name Power Down mode definition

    Power Down mode definition

    @note This definition is used for clk_powerdown().
*/
//@{
#define PLL_PDN_MODE_CLKSCALING     0x00        ///< Clock scaling mode
#define PLL_PDN_MODE_SLEEP1         0x01        ///< Sleep mode 1 (CPU off, PLL1/PLL2 on, OSC on, PCLK=OSC/1024)
#define PLL_PDN_MODE_SLEEP2         0x02        ///< Sleep mode 2 (CPU off, PLL1/PLL2 off, OSC on, PCLK=OSC/1024)
#define PLL_PDN_MODE_SLEEP3         0x03        ///< Sleep mode 3 (CPU off, PLL1/PLL2 off, OSC off, PCLK=RTC)
//@}

/*
    Dram parameter calibration definition

    This definition is used for NT96460 only.
*/
//@{
#define DMA_TWR_Calibration         0x02
//@}



extern ER      clk_open(void);
extern ER      clk_close(void);
extern BOOL    clk_changeDMA(UINT32 uiDMAClk, PDMA_TIMING pDMATiming, PDMA_SETTING pDMASetting);
extern BOOL    clk_changeCPUAHB(UINT32 uiCPUClk, UINT32 uiAHBClk);
extern BOOL    clk_changeAPB(UINT32 uiAPBClk);
extern BOOL    clk_powerdown(UINT32 uiMode);
extern void    clk_waitFlag(void);
extern void    clk_clearFlag(void);

//@}

#endif
