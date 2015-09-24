/**
    ScanmerTG header file

    The ScannerTG controller bridging the Contact-Image-Sensor(CIS), Analog-Front-End(AFE), and scroll module to image-pipeine.
    The controller generates the control signals that CIS and AFE needed and then rceiving the AFE's output data signals to SIE.
    The design of the control signals' generation are very flexible, so the user has to take care each signal's timing configuration.

    @file       scantg.h
    @ingroup    mIIOScanTG
    @note       The following descriptions are the sugguestion programming flow of the ScannerTG module.
\n
\n              (1) Call scantg_open() to open the ScannerTG module. The scantg_open() is protected by the OS semaphore.
\n              (2) Remember to call scantg_setEnable(FALSE) before you changing the ScannerTG's configurations.
                    Most of the settings can only be set when ScannerTG is disable
                    and validate the configurations when ScannerTG is enabled.
\n              (3) Call scantg_setIntEnable() to enable the interrupt source needed in the application.
\n              (4) Call scantg_initTG() to initialize ScannerTG's configurations, such as operation modes, trigger source,
                    data sample mode, and the scan toggle count.
\n              (5) Call scantg_initCisTiming() to initialize the CIS (Contact Image Sensor) signals' timing, such as
                    LED-RGB / CISCLK-frequency / CISFS-Period / CISDPI 's control timing waveform.
\n              (6) Call scantg_initAfeTiming() to initialize the AFE (Analog Front End) signals' timing, such as
                    ADCCLK-frequency / CDSCLK1 / CDSCLK2 / ACYC 's control timing waveform.
\n              (7) Call scantg_setPixclkDiv() to set the PIXCLK frequency according to the Scanner Module Type.
\n              (8) Call scantg_setSmplClkInv() / scantg_setCisSigInv() / scantg_setAfeSigInv() to setup the output control signals'
                    waveform polarity.
\n              (9) Call scantg_setEnable(TRUE) to validate all the configurations and enable the ScannerTG controller's operation.
\n              10) Call scantg_setScanTrigger() to scan ONE-LINE if MANUAL mode is selected in step-3.
\n             (11) Call scantg_setFrameStart() to send one VD pulse to SIE
                    and this pulse will be regarded as frame start signal in the scanning.
\n             (12) Call scantg_setEnable(FALSE) when scanning is done.
\n             (13) Call scantg_close() after you dont need ScannerTG module any more.
\n

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/


#ifndef _SCANTG_H
#define _SCANTG_H

#include "OsCommon.h"

/**
    @addtogroup mIIOScanTG
*/
//@{



/**
    ScannerTG controller ISR callback function prototype
*/
//@{
typedef void (*ISRCallbackFunc)(UINT32 uiCallID, UINT32 uiParam);
//@}


/**
    ScannerTG controller ISR callback status ID or the interrupt enable/disable ID

    These IDs can be used in the callback function in the ScanTG ISR to verify which interrupt is issued.
    The callback function is assigned in scantg_open(CallBackFunc).
*/
typedef enum
{
    SCANTG_VD_STS               = 0x0001,      ///<    Vsync interrupt status ID.
                                               ///<    This status would be regarded as scanning start signal in each scanning image.
    SCANTG_HD_STS               = 0x0002,      ///<    Hsync interrupt status ID.
                                               ///<    This status would be regarded as scanning start signal in each scanning line.
    SCANTG_CISFS_STS            = 0x0004,      ///<    CISFS interrupt status ID.
                                               ///<    This interrupt means CISFS active pulse is sent on the bus.
    SCANTG_OS_STS               = 0x0008,      ///<    Scan Over Speed interrupt status ID.
                                               ///<    Once too much scan trigger pulse is got during one scan line. This interrupt is issued.
    SCANTG_OS_TOT_STS           = 0x0010,      ///<    Scan Over Speed Total Count interrupt status ID.
                                               ///<    If the over-speed counts exceed the over speed total threshold, this interrupt status would be triggered.
    SCANTG_OS_LINE_STS          = 0x0020,      ///<    Scan Over Speed Line Count interrupt status ID
                                               ///<    If scan trigger event numbers are got exceed over speed line count, this interrupt would be issued.
    SCANTG_TRIG_START_RDY_STS   = 0x0100,      ///<    Trigger start is ready for next scan pulse status ID.
                                               ///<    This interrupt is issued when the controller is ready to received next scan trigger pulse.

    SCANTG_STS_ALL      = 0x013F,
    ENUM_DUMMY4WORD(SCANTG_INTID)
}SCANTG_INTID;

/**
    ScanTG controller operation mode

    ScanTG controller operation mode.
    The control signals of the LED-R/G/B's behavior would be different in the Gray / Color Mode.
    The scanning speed of the gray mode is three times faster than color mode.
    This selection controls the bus LED-R/G/B signals' behavior.
*/
typedef enum
{
    SCANTG_MODE_GRAY,       ///< GRAY operation mode.
    SCANTG_MODE_COLOR,      ///< COLOR operation mode.

    ENUM_DUMMY4WORD(SCANTG_MODE)
}SCANTG_MODE;


/**
    ScanTG scan trigger mode.

    ScanTG scan trigger mode.
    The Controller can be selected to be triggered from Manual(/SW) or from the auto-trigger-signal.
\n  If set to manual trigger mode, user must use scantg_setScanTrigger() to trigger each scan-line.
\n  If set to auto trigger mode, the trigger input can be selected as ScanTrigger from PAD-PIN or set to
    ADC-CH1 or ADC-CH2 as scan trigger input. The different between ScanTrigger pad-pin and ADC-CH1/2 are the ADC-CH1/2
    can use arbitrary voltage levels as trigger level and pad-pin must be 3.3V trigger pulse.
*/
typedef enum
{
    SCANTG_TRIGMODE_AUTO,       ///< AUTO scan trigger mode. Please use SCANTG_TRIGSRC to assign trigger source as pad-pin or ADC channel.
    SCANTG_TRIGMODE_MANUAL,     ///< MANUAL(or Software) scan trigger mode. The SCANTG_TRIGSRC parameter is useless.

    ENUM_DUMMY4WORD(SCANTG_TRIGMODE)
}SCANTG_TRIGMODE;


/**
    Auto Trigger Source.

    Auto Trigger Source.
    There are three different auto-trigger source, they are from pad-input / ADC-CH1 / ADC-CH2.
    This selection is effective only when SCANTG_TRIGMODE set to SCANTG_TRIGMODE_AUTO.
    The different between ScanTrigger pad-pin and ADC-CH1/2 are the ADC-CH1/2 can use arbitrary
    voltage levels as trigger level and pad-pin must be 3.3V trigger pulse.
*/
typedef enum
{
    SCANTG_TRIGSRC_PADPIN,      ///<    Auto scan trigger signal from PAD-INPUT.
    SCANTG_TRIGSRC_ADCCH1,      ///<    Auto scan trigger signal from ADC-CHANNEL-1.
    SCANTG_TRIGSRC_ADCCH2,      ///<    Auto scan trigger signal from ADC-CHANNEL-2.

    ENUM_DUMMY4WORD(SCANTG_TRIGSRC)
}SCANTG_TRIGSRC;

/**
    ScanTG Data Sample Mode.

    ScanTG Data Sample Mode.
    There are four kinds of data sample mode in the controller.
    The mode selection depends on AFE output bus width and output behavior.
    Basically, the ScanTG controller supports 8bits bus DDR/SDR and 4bits bus DDR/SDR.
    So ScanTG can support up to 16bits per pixel on the AFE output.
*/
typedef enum
{
    SCANTG_SAMPLMODE_BUS8_PIX8,     ///< Data Bus is 8bits wide and is 8-bits per pixel. (SDR ADCCLK)
    SCANTG_SAMPLMODE_BUS8_PIX16,    ///< Data Bus is 8bits wide and is 16-bits per pixel.(DDR ADCCLK)
    SCANTG_SAMPLMODE_BUS4_PIX16,    ///< Data Bus is 4bits wide and is 16-bits per pixel.(DDR ADCCLK)
    SCANTG_SAMPLMODE_BUS4_PIX8,     ///< Data Bus is 4bits wide and is 8-bits per pixel. (SDR ADCCLK)

    ENUM_DUMMY4WORD(SCANTG_SAMPLMODE)
}SCANTG_SAMPLMODE;

/**
    Signal Active Edge.

    Signal Active Edge. This is used to set the CDSCLK1/2's active edge is active HIGH/LOW.
*/
typedef enum
{
    SCANTG_ACTEDGE_ACTIVEHIGH,      ///< The control signal is active HIGH
    SCANTG_ACTEDGE_ACTIVELOW,       ///< The control signal is active LOW

    ENUM_DUMMY4WORD(SCANTG_ACTEDGE)
}SCANTG_ACTEDGE;

/**
    ScanTG data sample delay.

    ScanTG data sample delay. The typical setting of SCANTG_SAMPL_DELAY is SCANTG_SAMPL_DELAY0.
    Others may be used if the SCANTG_SAMPLMODE set to SCANTG_SAMPLMODE_BUS8_PIX16 / SCANTG_SAMPLMODE_BUS4_PIX16 / SCANTG_SAMPLMODE_BUS4_PIX8 during
    bus has too much delay time.
*/
typedef enum
{
    SCANTG_SAMPL_DELAY0,    ///< Data Sample Delay 0 clock cycles.
    SCANTG_SAMPL_DELAY1,    ///< Data Sample Delay 1 clock cycles.
    SCANTG_SAMPL_DELAY2,    ///< Data Sample Delay 2 clock cycles.
    SCANTG_SAMPL_DELAY3,    ///< Data Sample Delay 3 clock cycles.

    ENUM_DUMMY4WORD(SCANTG_SAMPL_DELAY)
}SCANTG_SAMPL_DELAY;

/**
    The structure of the ScanTG controller opeation mode configurations.

    The structure of the ScanTG controller opeation mode configurations.
*/
typedef struct
{
    SCANTG_MODE         ColorMode;      ///< Select ScanTG as Color or Gray mode. Use SCANTG_MODE_GRAY / SCANTG_MODE_COLOR.

    SCANTG_SAMPLMODE    SampleMode;     ///< Select ScanTG input bus width and pixel bits number.
    SCANTG_SAMPL_DELAY  SampleDelay;    ///< Select ScanTG Sample Delay.

    SCANTG_TRIGMODE     TrigMode;       ///< Select ScanTG trigger mode as Auto or Manual mode.
    SCANTG_TRIGSRC      AutoTrigSrc;    ///< Select ScanTG auto trigger source. This field is effective if TrigMode = SCANTG_TRIGMODE_AUTO.
    UINT32              uiTrigToggleCnt;///< Select ScanTG auto trigger toggle counter.
                                        ///< The (SCANTRIG_TOGGLE+1) number of level change after deglitch function would be thinked as one ActiveScan signal to the controller.

}SCANTG_CONFIG,*PSCANTG_CONFIG;


/**
    The Timing control structure of the AFE control signals.

    The Timing control structure of the AFE control signals, such as ADCCLK / CDSCLK1 / CDSCLK2 / ACYC.
*/
typedef struct
{
    //  ADCCLK configuration
    UINT32 uiAdcClkDivider;     ///< The ADC Clock frequency = (Module-Clock)/(uiAdcClkDivider+1). Valid Range from 1~255.
    UINT32 uiAdcClkPhaseOfs;    ///< The phase offset to ADC clock. Unit in module clock cycles. Valid Range from 0~255.

    //  CDSCLK1 configuration
    UINT32 uiCdsClk1PhaseOfs;   ///< The phase offset to CDSCLK1 signal. Unit in module clock cycles. Valid Range from 0~255.
    UINT32 uiCdsClk1Width;      ///< The pulse width of the CDSCLK1 signal. Unit in module clock cycles. Valid Range from 0~255.

    //  CDSCLK2 configuration
    UINT32 uiCdsClk2PhaseOfs;   ///< The phase offset to CDSCLK2 signal. Unit in module clock cycles. Valid Range from 0~255.
    UINT32 uiCdsClk2Width;      ///< The pulse width of the CDSCLK2 signal. Unit in module clock cycles. Valid Range from 0~255.

    //  ACYC configuration
    UINT32 uiAcycOfs;           ///< The ACYC signal offset time from CISFS active pulse. Unit is half of the ADCCLK cycles. Valid range from 0x0~0x7FFF.
    UINT32 uiAcycWidth;         ///< The ACYC signal pulse width. Unit is half of the ADCCLK cycles. Valid range from 0x0~0x7FFF.

}SCANTG_AFETIMING,*PSCANTG_AFETIMING;


/**
    The Timing control structure of the CIS Sensor control signals.

    The Timing control structure of the CIS Sensor control signals, such as CISFS / CISCLK / CISDPI / LEDR / LEDG / LEDB.
*/
typedef struct
{
    // CISCLK configuration
    UINT32          uiCisClkDivider;    ///< The CISCLK Clock frequency = (Module-Clock)/(uiCisClkDivider+1). Valid Range from 1~255.

    // CISFS configuration
    UINT32          uiFsPeriod;         ///< The CISFS pulse period. Unit is half of the CISCLK cycles. Valid range from 0x2~0x7FFF.
    UINT32          uiFsWidth;          ///< The CISFS pulse width. Unit is half of the CISCLK cycles. Valid range from 0x0~0x7FFF.

    // CISDPI configuration
    UINT32          uiDpiOfs;           ///< The CISDPI pulse offset. Unit is half of the CISCLK cycles. Valid range from 0x0~0x7FFF.
    UINT32          uiDpiWidth;         ///< The CISDPI pulse width. Unit is half of the CISCLK cycles. Valid range from 0x0~0x7FFF.

    // LED-R configuration
    UINT32          uiLedROfs;          ///< The LED-R pulse offset. Unit is half of the CISCLK cycles. Valid range from 0x0~0x7FFF.
    UINT32          uiLedRWidth;        ///< The LED-R pulse width. Unit is half of the CISCLK cycles. Valid range from 0x1~0x7FFF.

    // LED-G configuration
    UINT32          uiLedGOfs;          ///< The LED-G pulse offset. Unit is half of the CISCLK cycles. Valid range from 0x0~0x7FFF.
    UINT32          uiLedGWidth;        ///< The LED-G pulse width. Unit is half of the CISCLK cycles. Valid range from 0x1~0x7FFF.

    // LED-B configuration
    UINT32          uiLedBOfs;          ///< The LED-B pulse offset. Unit is half of the CISCLK cycles. Valid range from 0x0~0x7FFF.
    UINT32          uiLedBWidth;        ///< The LED-B pulse width. Unit is half of the CISCLK cycles. Valid range from 0x1~0x7FFF.
}SCANTG_CISTIMING,*PSCANTG_CISTIMING;

/**
    CIS Control Signals level control structure

    CIS Control Signals level control structure.
    This is used in scantg_getCisSigInv() / scantg_setCisSigInv() to control the control signals' output level.
*/
typedef struct
{
    BOOL bCisFsInv;     ///< CISFS signal control. TRUE is Active-Low/Invert. FALSE is Active-High/No-Invert.
    BOOL bCisDpiInv;    ///< CISDPI signal control. TRUE is Active-Low/Invert. FALSE is Active-High/No-Invert.
    BOOL bLedRInv;      ///< LED-R signal control. TRUE is Active-Low/Invert. FALSE is Active-High/No-Invert.
    BOOL bLedGInv;      ///< LED-G signal control. TRUE is Active-Low/Invert. FALSE is Active-High/No-Invert.
    BOOL bLedBInv;      ///< LED-B signal control. TRUE is Active-Low/Invert. FALSE is Active-High/No-Invert.
}SCANTG_CISINV,*PSCANTG_CISINV;


//
//  ScanTG Extern API Definitions
//

extern ER   scantg_open(ISRCallbackFunc pEventHdl);
extern ER   scantg_close(void);
extern BOOL scantg_isOpened(void);
extern void scantg_setIntEnable(SCANTG_INTID IntID);
extern void scantg_setIntDisable(SCANTG_INTID IntID);
extern void scantg_getIntEnable(SCANTG_INTID *pIntID);
extern ER   scantg_chgIsrCB(ISRCallbackFunc pEventHdl);

extern void scantg_setEnable(BOOL bEn);
extern BOOL scantg_getEnable(void);
extern void scantg_setScanTrigger(void);
extern BOOL scantg_getScanTrigger(void);
extern void scantg_setFrameStart(void);
extern void scantg_clrOverSpeedCnt(BOOL bTotalCnt, BOOL bLineCnt);
extern void scantg_getOverSpeedCurVal(UINT32 *puiTotCnt, UINT32 *puiLineCnt);


extern void scantg_initCisTiming(PSCANTG_CISTIMING pCisTiming);
extern void scantg_getCisTiming(PSCANTG_CISTIMING pCisTiming);
extern void scantg_setCisFsPeriod(UINT32 uiFsPeriod);
extern void scantg_setDpiTiming(UINT32 uiDpiOfs, UINT32 uiDpiWidth, BOOL bDpiInv);
extern void scantg_setLedWidth(UINT32 uiLedR, UINT32 uiLedG, UINT32 uiLedB);
extern void scantg_setCisSigInv(PSCANTG_CISINV pCisInv);
extern void scantg_getCisSigInv(PSCANTG_CISINV pCisInv);
extern void scantg_setCisFsActEdge(BOOL bActiveEdge);

extern void scantg_initAfeTiming(PSCANTG_AFETIMING  pAfeTiming);
extern void scantg_getAfeTiming(PSCANTG_AFETIMING  pAfeTiming);
extern void scantg_setAfeSigInv(SCANTG_ACTEDGE CdsClk1, SCANTG_ACTEDGE CdsClk2, BOOL bAdcClkInv, BOOL bAcycInv);

extern void scantg_initTG(PSCANTG_CONFIG pTgConfig);
extern void scantg_getTG(PSCANTG_CONFIG pTgConfig);
extern void scantg_setColorMode(BOOL bColor);
extern void scantg_setGlitchSuprCnt(UINT32 uiCnt);
extern void scantg_setSmplClkInv(BOOL bInv);
extern void scantg_getSmplClkInv(BOOL *pbInv);
extern void scantg_setPixclkDiv(UINT32 uiDiv);
extern void scantg_getPixclkDiv(UINT32 *puiDiv);
extern void scantg_setSyncOffset(UINT32 uiHsyncOffset, UINT32 uiVsyncOffset);
extern void scantg_getSyncOffset(UINT32 *puiHsyncOffset, UINT32 *puiVsyncOffset);
extern void scantg_setOverSpeedThresld(UINT32 uiTotTH, UINT32 uiLineTH);
extern void scantg_getOverSpeedThresld(UINT32 *puiTotTH, UINT32 *puiLineTH);

extern void scantg_setRoundingEn(BOOL bEn);
//@}
#endif
