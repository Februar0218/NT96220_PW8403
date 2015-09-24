/**
    Header file for Interrupt module

    This file is the header file that define the API for Interrupt module.

    @file       Interrupt.h
    @ingroup    mIHALInterrupt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "Type.h"

/**
    @addtogroup mIHALInterrupt
*/
//@{

//-----------------------------------------------------------------------------
// All interrupt ID
//-----------------------------------------------------------------------------
#define _INT_00             0x00
#define _INT_01             0x01
#define _INT_02             0x02
#define _INT_03             0x03
#define _INT_04             0x04
#define _INT_05             0x05
#define _INT_06             0x06
#define _INT_07             0x07
#define _INT_08             0x08
#define _INT_09             0x09
#define _INT_10             0x0A
#define _INT_11             0x0B
#define _INT_12             0x0C
#define _INT_13             0x0D
#define _INT_14             0x0E
#define _INT_15             0x0F
#define _INT_16             0x10
#define _INT_17             0x11
#define _INT_18             0x12
#define _INT_19             0x13
#define _INT_20             0x14
#define _INT_21             0x15
#define _INT_22             0x16
#define _INT_23             0x17
#define _INT_24             0x18
#define _INT_25             0x19
#define _INT_26             0x1A
#define _INT_27             0x1B

#define _INT_DUMMY          0xFF


/**
      @name     Interrupt Type

      Definition for interrupt type
*/
//@{
#define _INT_TYPE_FIQ       1           ///< FIQ type interrupt
#define _INT_TYPE_IRQ       0           ///< IRQ type interrupt
//@}

/**
      @name     Interrupt ID

      Interrupt ID definition for usage as argument to API of this module.
*/
//@{

#define _INT_TIMER          _INT_00     ///< Timer 0 interrupt
#define _INT_SIE            _INT_01     ///< SIE controller interrupt
#define _INT_IPE            _INT_02     ///< IPE controller interrupt
#define _INT_IME            _INT_03     ///< IME controller interrupt
#define _INT_IFE            _INT_04     ///< IFE interrupt
#define _INT_AUDIO          _INT_05     ///< Audio(DAI) controller interrupt
#define _INT_JPEG           _INT_06     ///< JPEG controller interrupt
#define _INT_GRAPHY         _INT_07     ///< Graphic engine controller interrupt
#define _INT_GPIO           _INT_08     ///< GPIO controller interrupt
#define _INT_PWM            _INT_09     ///< PWM controller interrupt
#define _INT_USBOTG         _INT_10     ///< USB controller interrupt
#define _INT_NAND           _INT_11     ///< NAND/xD interrupt
#define _INT_SDIO           _INT_12     ///< SDIO interrupt
#define _INT_SPI            _INT_13     ///< SPI controller interrupt
#define _INT_SIF            _INT_14     ///< SIF controller interrupt
#define _INT_I2C            _INT_15     ///< I2C controller interrupt
#define _INT_UART           _INT_16     ///< UART controller interrupt
#define _INT_SCANTG         _INT_17     ///< SCANNER TG controller interrupt
#define _INT_UART2          _INT_18     ///< UART2 controller interrupt
#define _INT_ADC            _INT_19     ///< ADC controller interrupt
#define _INT_IDE            _INT_20     ///< IDE controller interrupt
#define _INT_MI             _INT_21     ///< MI controller interrupt
#define _INT_HDMI           _INT_22     ///< HDMI controller interrupt
#define _INT_RTC            _INT_23     ///< Real Time Clock interrupt
#define _INT_WDT            _INT_24     ///< Watchdog timer interrupt
#define _INT_CG             _INT_25     ///< ClockGen interrupt
#define _INT_DMA            _INT_26     ///< DMA controller interrupt
#define _INT_AMBA           _INT_27     ///< AMBA DMA controller interrupt

//@}


#define _INT_ALL            0xFFFFFFFFUL

/*
    Public OS level functions
*/
extern void     int_init(void);

/*
    Public driver specific operating functions
*/
extern void     int_enable(UINT32 intno);
extern void     int_disable(UINT32 intno);
extern void     int_enable_multi(UINT32 intval);
extern void     int_disable_multi(UINT32 intval);
extern UINT32   int_getflag(void);
extern UINT32   int_getfiqid(void);
extern UINT32   int_getirqid(void);
extern UINT32   int_getenable(void);
extern UINT32   int_gettype(void);
extern void     int_settype(UINT32 uiIntNo, UINT32 uiType);
extern void     int_settype_multi(UINT32 uiIntVal, UINT32 uiType);

#endif

//@}
