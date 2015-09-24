/**

    Header file for ADC module.

    This file is the header file that define the API for ADC module.

    @file       adc.h
    @ingroup    mIIOADC
    @note       The following descriptions are the sugguestion programming flow of the ADC module.
\n
\n              (1) Call adc_setControl() / adc_setSampleAvg() to set ADC clock for all the ADC channels.
\n              (2) Call adc_open() to open one of the ADC channel, clock will be enabled.
\n              (3) Call adc_setMode() to setup the Continuous/OneShot Mode, interrupt enable, and hook the ISR Callback.
\n              (4) Call adc_setChDivider() to setup the channel divider of specified ADC channel.
\n              (5) Call adc_enable() to enable ADC operations.
\n              (6) If in OneShot Mode, Call adc_triggerOneShot() to trigger OneShot for specified ADC channel.
\n              (7) Call adc_close() to close specified ADC channel after you dont need specified ADC channel any more.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _ADC_H
#define _ADC_H

#include "Type.h"


/**
    @addtogroup mIIOADC
*/
//@{


/**
    Definition for ADC channel

    ADC channel ID, which is used to specify one of the ADC channel to access.
*/
typedef enum
{
    ADC_CHANNEL_0,              ///< ADC channel 0
    ADC_CHANNEL_1,              ///< ADC channel 1
    ADC_CHANNEL_2,              ///< ADC channel 2

    ADC_TOTAL_CH,               // Total ADC channel number

    ADC_CHANNEL_3,              // ADC channel 3, backward compatible usage
    ADC_CHANNEL_4,              // ADC channel 4, backward compatible usage
    ADC_CHANNEL_5,              // ADC channel 5, backward compatible usage
    ADC_CHANNEL_6,              // ADC channel 6, backward compatible usage
    ADC_CHANNEL_7,              // ADC channel 7, backward compatible usage

    ENUM_DUMMY4WORD(ADC_CHANNEL)
}ADC_CHANNEL;

//#define ADC_CLOCK               12000000         // 12MHz

/**
    @name   ADC sampling divider range

    Definition for ADC sampling divider range.
    The ADC operation clock frequency = 12MHz /(2*(SAMPDIV + 1)).
*/
//@{
#define ADC_SAMPDIV_MIN         0x2            ///< Minimum sampling divider value
#define ADC_SAMPDIV_MAX         0x1F           ///< Maximum sampling divider value
//@}

/**
    @name   ADC channel sampling divider range

    Definition for ADC channel sampling divider range
*/
//@{
#define ADC_CHSAMPDIV_MIN         0x0            ///< Minimum channel sampling divider value
#define ADC_CHSAMPDIV_MAX         0xFF           ///< Maximum channel sampling divider value
//@}

/**
    @name   ADC channel external sampling counts

    Definition for ADC channel external sampling clock-counts range
*/
//@{
#define ADC_EXTCNT_MIN         0x0          ///< Minimum channel external sampling counts value
#define ADC_EXTCNT_MAX         0xF          ///< Maximum channel external sampling counts value
//@}


/**
    Definition ADC sampling times every sequence

    ADC controller can continuous sample several times and take the average to output.
*/
typedef enum
{
    ADC_SAMPAVG_1,  ///< Sample one  times and then output this value.
    ADC_SAMPAVG_2,  ///< Sample two  times and then output the average value.
    ADC_SAMPAVG_4,  ///< Sample four times and then output the average value.

    ENUM_DUMMY4WORD(ADC_SAMPLEAVG)
} ADC_SAMPLEAVG;


/**
    Definitions for setup value trigger interrupt's value trigger range.

    ADC controller's value trigger interrupt can select to issue interrupt when the sample value is within specified range.
    This is used in adc_setValTrigMode().
*/
typedef enum
{
    ADC_VALTRIG_RANGE_IN,   ///< The value trigger interrupt is issued when sample value is inside the specified range.
    ADC_VALTRIG_RANGE_OUT,  ///< The value trigger interrupt is issued when sample value is outside the specified range.

    ENUM_DUMMY4WORD(ADC_VALTRIG_RANGE)
}ADC_VALTRIG_RANGE;

/**
    Definition for setup the value trigger interrupt is edge trigger or level trigger interrupt.

    This is used in adc_setValTrigMode().
*/
typedef enum
{
    ADC_VALTRIG_MODE_LVL,
    ADC_VALTRIG_MODE_EDGE,

    ENUM_DUMMY4WORD(ADC_VALTRIG_MODE)
}ADC_VALTRIG_MODE;

//
//  ADC Extern API Definitions
//

extern ER       adc_open(UINT32 uiChannel);
extern ER       adc_close(UINT32 uiChannel);
extern ID       adc_getLockStatus(UINT32 uiChannel);

extern void     adc_setControl(UINT32 uiSampDiv, BOOL bSeq);
extern void     adc_getControl(UINT32 *uiSampDiv, BOOL *bSeq);

extern void     adc_setMode(UINT32 uiChannel, BOOL bContMode, BOOL bIntEn, FP Isr);
extern void     adc_getMode(UINT32 uiChannel, BOOL *bContMode, BOOL *bIntEn);
extern void     adc_setChDivider(UINT32 uiChannel, UINT32 uiChSampDiv);
extern void     adc_getChDivider(UINT32 uiChannel, UINT32 *puiChSampDiv);
extern void     adc_setExternalSampCnt(UINT32 uiExtCnt);
extern void     adc_getExternalSampCnt(UINT32 *puiExtCnt);
extern void     adc_setSampleAvg(ADC_SAMPLEAVG SampAvg);
extern void     adc_getSampleAvg(ADC_SAMPLEAVG *pSampAvg);

extern void     adc_enable(void);
extern void     adc_disable(void);
extern BOOL     adc_isEnabled(void);

extern void     adc_triggerOneShot(UINT32 uiChannel);

extern UINT32   adc_readData(UINT32 uiChannel);
extern BOOL     adc_isDataReady(UINT32 uiChannel);
extern void     adc_clearDataReady(UINT32 uiChannel);

extern UINT32   adc_readVoltage(UINT32 uiChannel);

extern void     adc_setValTrigThreshold(UINT32 uiChannel, UINT32 uiStart, UINT32 uiEnd);
extern void     adc_getValTrigThreshold(UINT32 uiChannel, UINT32 *puiStart, UINT32 *puiEnd);
extern void     adc_setValTrigVoltage(UINT32 uiChannel, UINT32 uiStart, UINT32 uiEnd);
extern void     adc_getValTrigVoltage(UINT32 uiChannel, UINT32 *puiStart, UINT32 *puiEnd);
extern void     adc_setValTrigMode(UINT32 uiChannel, ADC_VALTRIG_RANGE Range, ADC_VALTRIG_MODE Mode);
extern void     adc_setValTrig(UINT32 uiChannel, BOOL TrigEn, BOOL bIntEn, FP TrigIsr);

//#NT#2011/05/31#Klins Chen -begin
//#NT# Add new API for TV DAC Calibration usage
extern void     adc_setTvCalibEn(BOOL bEn);
//#NT#2011/05/31#Klins Chen -end

//@}

/*
    @name   TP internal resistor range

    Backward compatible usage. None of usage in current chip version.
    Definition for internal resistor range.

*/
//@{
#define TP_INTRES_MIN         0x0            ///< Minimum internal resistor value
#define TP_INTRES_MAX         0x3F           ///< Maximum internal resistor value
//@}


/*
    @name   TP sampling cycle range

    Backward compatible usage. None of usage in current chip version.
    Definition for TP sampling cycle range
*/
//@{
#define TP_SAMPCYC_MIN         0x0            ///< Minimum sampling cycle value
#define TP_SAMPCYC_MAX         0xFF           ///< Maximum sampling cycle value
//@}


/*
    @name   TP pen-down gsr range

    Backward compatible usage. None of usage in current chip version.
    Definition for TP pen-down gsr range
*/
//@{
#define TP_PDGSR_MIN         0x0            ///< Minimum pen-down gsr value
#define TP_PDGSR_MAX         0xFF           ///< Maximum pen-down gsr value
//@}


/*
    @name TP operation mode

    Backward compatible usage. None of usage in current chip version.
    Definition for TP operation
*/
typedef enum
{
    TP_MODE0,
    TP_MODE1,
    TP_MODE2,
    TP_MODE3,

    ENUM_DUMMY4WORD(TP_OPMODE)
} TP_OPMODE;

/*
    @name TP measurement mode

    Backward compatible usage. None of usage in current chip version.
    Definition for TP measurement mode
*/
typedef enum
{
    TP_PD,
    TP_POS,
    TP_PRE,

    ENUM_DUMMY4WORD(TP_MSMODE)
} TP_MSMODE;

#define KEYSCAN_TP_PRESSURE_TH      245     // 255 - 10

//
//  Backward Compatible APIs
//
#define adc_setTPControl(uiGsr, uiRes, uiSampCyc)       do{ }while(0)
#define adc_setTPMode(Opmode, Msmode, bIntEn, TPIsr)    do{ }while(0)
#define adc_startTP()                                   do{ }while(0)
#define adc_stopTP()                                    do{ }while(0)
#define adc_isTPDataReady()                             FALSE
#define adc_clearTPDataReady()                          do{ }while(0)
#define adc_getValidTPXY(puiX, puiY)                    do{ }while(0)
#define adc_readTPPressure(puiP1, puiP2)                do{ }while(0)
#define adc_readTPXY(puiX, puiY)                        do{ }while(0)
#define adc_initTP(pSetting)                            do{ }while(0)
#define adc_detTPGesture(pData)                         0
#define adc_setOISChannle(gyro_x, gyro_y, hall_x, hall_y)   0
#define adc_setOISOutputEN(uiEN)                        0

#endif