/**
    Scanner Module Configuration Header

    Scanner Module Subsystem. This file defines API for interfacing to driver.

    @file       Scanner.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _SCANNER_H
#define _SCANNER_H

#include "OsCommon.h"
#include "scantg.h"




/**
    SIF pin definition descriptor for Scanner control.

    SIF is used as main control interface of Scanner module.
\n  This structure defines the interface is using SIF or using GPIO to emulate SIF.
*/
//@{
typedef struct
{
    UINT32  uimode;                     ///< Use SCAN_CTRL_MODE_SIF or SCAN_CTRL_MODE_GPIO
    UINT32  uisif_channel;              ///< SIF channel SIF_CH0 ~ 1, refer to SIF.h, valid in SCAN_CTRL_MODE_SIF
    UINT32  uigpio_sen;                 ///< GPIO pin number for SEN, valid when uimode == SCAN_CTRL_MODE_GPIO
    UINT32  uigpio_clk;                 ///< GPIO pin number for Clock, valid when uimode == SCAN_CTRL_MODE_GPIO
    UINT32  uigpio_data;                ///< GPIO pin number for Data, valid when uimode == SCAN_CTRL_MODE_GPIO
} SCAN_SIFPIN_DEF, *PSCAN_SIFPIN_DEF;

#define     SCAN_CTRL_MODE_SIF       0   ///< SCANNER control use serial interface(SIF).
#define     SCAN_CTRL_MODE_GPIO      1   ///< SCANNER control use GPIO.
//@}





/**
    ScannerTG Source clock selection.

    This specifies the clock source of ScanTG engine.
*/
//@{
typedef enum
{
    SCAN_SRCCLK_PLL1     = 0,            ///< ScanTG Source clock is PLL1
    SCAN_SRCCLK_PLL2     = 1,            ///< ScanTG Source clock is PLL2

    ENUM_DUMMY4WORD(SCAN_SRCCLKSEL)
} SCAN_SRCCLKSEL;
//@}


typedef enum
{
    SCAN_300DPI,
    SCAN_600DPI,
    SCAN_1200DPI,

    ENUM_DUMMY4WORD(SCAN_DPI)
}SCAN_DPI;





/**
    Scanner device main object

    This object defines the main control parameters for Scanner device.
*/
//@{
typedef struct
{
    SCAN_SRCCLKSEL      ScanSrcClk;                 ///< SCAN_SRCCLKSEL type: the ScanTG source clock is PLL1 or PLL2.
    SCAN_SIFPIN_DEF     ScanCtrlPin;                ///< Serial interface(SIF) pin definition which used to control the LCD panel.

    SCAN_DPI            ScanDpi;                    ///< Select Scanner operation at 300/600/1200DPI
    SCANTG_MODE         ScanMode;                   ///< Select Scanner is operate at Color or Gray scanning mode.
    SCANTG_TRIGMODE     ScanTrigMode;               ///< Select Scanner's operation trigger is AUTO or MANUAL mode
    SCANTG_TRIGSRC      AutoScanTrigSrc;            ///< Select AUTO trigger source if select AUTO scan trigger in ScanTrigMode.

    ISRCallbackFunc     fpIsrCallback;              ///< Callback Function in ScanTG's ISR
} SCAN_OBJ, *PSCAN_OBJ;
//@}












/**
    Open Scanner Module
*/
extern void OpenScanner(PSCAN_OBJ pScanObj);

/**
    Close Scanner Module
*/
extern void CloseScanner(void);


/**
    Change Scanner DPI Mode
*/
extern void ChgScanDPI(SCAN_DPI ScanDpi);





#endif
