/**
    Header file of SPI module driver

    This file is the header file of SPI module

    @file       SPI.h
    @ingroup    mIIOSPI
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _SPI_H
#define _SPI_H

#include "Type.h"

/**
    @addtogroup mIIOSPI
*/
//@{

/**
    SPI max transfer count definition. (unit: word)

    @note For spi_writeReadData()
*/
//@{
//#NT#2011/05/27#Cliff Lin -begin
//#Fix the Max SPI transfer length
#define SPI_MAX_TRANSFER_CNT    (0x3FFFFF)  ///< max transfer count for spi_writeReadData()
//#NT#2011/05/27#Cliff Lin -begin
//@}

/**
    SPI mode

    @note For SPI_CONFIG, PSPI_CONFIG, and spi_init()
*/
typedef enum
{
    SPI_MODE_0,                 ///< MODE 0
    SPI_MODE_1,                 ///< MODE 1
    SPI_MODE_2,                 ///< MODE 2
    SPI_MODE_3,                 ///< MODE 3
    ENUM_DUMMY4WORD(SPI_MODE)
} SPI_MODE;

/**
    SPI bus width

    @note For spi_setBusWidth()
*/
typedef enum
{
    SPI_BUS_WIDTH_1_BIT,        //< Bus width is 1 bit
    SPI_BUS_WIDTH_2_BITS,       //< Bus width is 2 bits
    SPI_BUS_WIDTH_4_BITS,       //< Bus width is 4 bits
    ENUM_DUMMY4WORD(SPI_BUS_WIDTH)
} SPI_BUS_WIDTH;

/**
    SPI wide bus order

    @note For SPI_CONFIG, PSPI_CONFIG, and spi_init()
*/
typedef enum
{
    SPI_WIDE_BUS_ORDER_NORMAL,  ///< Normal order. Transmit sequence is SPI_IO3, SPI_IO2, SPI_IO1, SPI_IO0.
    SPI_WIDE_BUS_ORDER_INVERT,  ///< Invert order. Transmit sequence is SPI_IO0, SPI_IO1, SPI_IO2, SPI_IO3.
    ENUM_DUMMY4WORD(SPI_WIDE_BUS_ORDER)
} SPI_WIDE_BUS_ORDER;

/**
    SPI pinmux selection

    @note For SPI_CONFIG, PSPI_CONFIG, and spi_init()
*/
typedef enum
{
    SPI_PINMUX_STORAGE,         ///< Pinmux to storage interface
    SPI_PINMUX_SERIAL,          ///< Pinmux to serial interface
    ENUM_DUMMY4WORD(SPI_PINMUX)
} SPI_PINMUX;

/**
    SPI transfer length

    @note For spi_setTransferLen()
*/
typedef enum
{
    SPI_TRANSFER_LEN_1BYTE,     ///< 1 byte
    SPI_TRANSFER_LEN_2BYTES,    ///< 2 bytes
    SPI_TRANSFER_LEN_3BYTES,    ///< 3 bytes
    SPI_TRANSFER_LEN_4BYTES,    ///< 4 bytes
    ENUM_DUMMY4WORD(SPI_TRANSFER_LEN)
} SPI_TRANSFER_LEN;

/**
    SPI initialization infomation

    @note For spi_init()
*/
typedef struct
{
    BOOL        bCSActiveLow;   ///< CS polarity
                                ///< - TRUE: CS is active LOW
                                ///< - FALSE: CS is active HIGH
    BOOL        bMasterMode;    ///< Master mode
                                ///< - TRUE: master mode
                                ///< - FALSE: slave mode
    UINT32      uiClkDiv;       ///< Clock divider: 5 ~ 255. SPI clock frequency = 480Mhz / (uiClkDiv+1)
    SPI_MODE    spiMODE;        ///< SPI MODE select
                                ///< - SPI_MODE_0: mode 0
                                ///< - SPI_MODE_1: mode 1
                                ///< - SPI_MODE_2: mode 2
                                ///< - SPI_MODE_3: mode 3
    BOOL        bLSB;           ///< LSB mode select (bit order)
                                ///< - TRUE: LSB mode
                                ///< - FALSE: MSB mode
    SPI_WIDE_BUS_ORDER wideBusOrder;    ///< Bus order is wide bus mode (SPI_BUS_WIDTH_2_BITS or SPI_BUS_WIDTH_4_BITS)
                                        ///< - SPI_WIDE_BUS_ORDER_NORMAL: normal order
                                        ///< - SPI_WIDE_BUS_ORDER_INVERT: inverted order
    SPI_PINMUX  pinmuxChannel;  ///< SPI pinmux channel
                                ///< - SPI_PINMUX_STORAGE
                                ///< - SPI_PINMUX_SERIAL
    UINT32      uiDelay;        ///< - SPI delay time: 0~ 255
    UINT32      uiHoldTime;     ///< - SPI delay time: 0~ 15
    UINT32      uiDuration;     ///< - SPI delay time: 0~ 15
} SPI_INIT_INFO, *PSPI_INIT_INFO;

extern void spi_attach(void);
extern void spi_detach(void);
extern ER   spi_open(void);
extern ER   spi_close(void);
extern BOOL spi_isOpened(void);
extern ER   spi_init(PSPI_INIT_INFO pSpiInitInfo);

extern ER   spi_setTransferLen(SPI_TRANSFER_LEN length);
extern void spi_setCSActive(BOOL bCSActive);
extern ER   spi_writeReadData(UINT32 uiWordCount, UINT32* pTxBuf, UINT32* pRxBuf, BOOL bDmaMode);
extern ER   spi_waitDataDone(void);
extern ER   spi_writeSingle(UINT32 uiTxWord);
extern ER   spi_readSingle(UINT32* pRxWord);
extern ER   spi_writeReadSingle(UINT32 uiTxWord, UINT32* pRxWord);

extern ER   spi_enBitMatch(UINT32 uiCmd, UINT32 uiBitPosition, BOOL bWaitValue);
extern ER   spi_waitBitMatch(UINT32 uiTimeoutMs);
extern ER   spi_setBusWidth(SPI_BUS_WIDTH busWidth);

extern void spi_getCurrStatus(UINT32 *puiCtrl);
//@}

#endif
