/**
    Header file for SIF module.

    This file is the header file that define the API and data type
    for SIF module. The NT9622X's SIF controller has two SIF channels and share the same DATA/CLK pins.

    @file       sif.h
    @ingroup    mIIOSIF
    @note       The following descriptions are the sugguestion programming flow of the SIF module.
\n
\n              (1) Call sif_open() to open this module, clock will be enabled.
\n              (2) Call sif_setControl() to initialize the transmit timing configurations of specified SIF channel.
\n              (3) Call sif_setSens() to setup the SENS timing value if needed.
\n              (4) Call sif_sendData() to trigger transmitting the specified Data in LSB or MSB order.
\n              (5) Call sif_waitTransferEnd() to wait transmitting done.
\n              (6) Repeat Step-4 to Step-5 if you have bursts of data to send.
\n              (7) Call sif_close() if no more data needed to send.
\n

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SIF_H
#define _SIF_H

#include "Type.h"

/**
    @addtogroup mIIOSIF
*/
//@{

/**
    SIF channel ID

    SIF channel ID, which is used to specify one of the SIF channel to access.
*/
typedef enum
{
    SIF_CH0,                ///< SIF channel 0
    SIF_CH1,                ///< SIF channel 1

    SIF_TOTAL_CHANNEL,      //   SIF Total channel number

    SIF_CH2,                ///< SIF channel 2. Backward compatible usage
    SIF_CH3,                ///< SIF channel 3. Backward compatible usage

    ENUM_DUMMY4WORD(SIF_CH)
} SIF_CH;

/**
    SIF sync source ID

    Backward compatible usage. Useless in current chip version.
*/
typedef enum
{
    SIF_SYNCSRC_NONE,       ///< SIF none Sync Source.Backward compatible usage. Useless in current chip version.

    ENUM_DUMMY4WORD(SIF_SYNCSRC)
} SIF_SYNCSRC;

/**
    SIF bus mode ID

    SIF has 16 different transmit timing mode specified in the bus mode ID.
    Please refer to SIF application note for the description daigrams of SIF controller.
*/
typedef enum
{
    SIF_MODE_0000,          ///< SIF bus mode 0000
    SIF_MODE_0001,          ///< SIF bus mode 0001
    SIF_MODE_0010,          ///< SIF bus mode 0010
    SIF_MODE_0011,          ///< SIF bus mode 0011
    SIF_MODE_0100,          ///< SIF bus mode 0100
    SIF_MODE_0101,          ///< SIF bus mode 0101
    SIF_MODE_0110,          ///< SIF bus mode 0110
    SIF_MODE_0111,          ///< SIF bus mode 0111
    SIF_MODE_1000,          ///< SIF bus mode 1000
    SIF_MODE_1001,          ///< SIF bus mode 1001
    SIF_MODE_1010,          ///< SIF bus mode 1010
    SIF_MODE_1011,          ///< SIF bus mode 1011
    SIF_MODE_1100,          ///< SIF bus mode 1100
    SIF_MODE_1101,          ///< SIF bus mode 1101
    SIF_MODE_1110,          ///< SIF bus mode 1110
    SIF_MODE_1111,          ///< SIF bus mode 1111
    ENUM_DUMMY4WORD(SIF_MODE)
} SIF_MODE;

/**
    @name   SIF maximum / minimum value

    Definition for SIF maximum / minimum value
*/
//@{
#define SIF_MAX_SENDUR          0x0F    ///< Maximum SEN duration
#define SIF_MAX_TXSZ            0x60    ///< Maximum transmit size
#define SIF_MIN_TXSZ            0x01    ///< Minimum transmit size
#define SIF_MAX_CLKDIV          0x3FF   ///< Maximum clock divider
#define SIF_MIN_CLKDIV          0x00    ///< Minimum clock divider
#define SIF_MAX_SENH            0x07    ///< Maximum SEN hold time
#define SIF_MAX_SENS            0x07ul  ///< Maximum SEN Setup time
#define SIF_MAX_ADDRBITS        0xFF    ///< Maximum address bits (for continuous mode)
#define SIF_MAX_DELAY           0xFF     ///< Maximum delay
//@}

/**
    SIF channel configurations

    This structure is used in sif_setControl() to assign SIF configurations for specified SIF channel.
    We should notice that the SIF source clock is APB-clock(Typically 80MHz) in NT9622X.
    So the SIF bus clock is dicided by the 80MHz / (2 * (uiClkDiv + 1)).
*/
typedef struct
{
    SIF_MODE          uiMode;           ///< Bus mode. SIF_MODE_0000 ~ SIF_MODE_1111
    BOOL              bSyncEn;          ///< Enable / Disable sync function. Backward compatible usage. Useless in current chip version.
    SIF_SYNCSRC       uiSyncSrc;        ///< Select sync source. Backward compatible usage. Useless in current chip version.
    UINT32            uiSenDuration;    ///< SEN duration for SIF_MODE_1000 ~ SIF_MODE_1111
                                        ///< @note Valid value: 0x00 ~ 0x0F
    UINT32            uiLength;         ///< Transmit size
                                        ///< @note Valid value: PIO: 0x01 ~ 0x60
    UINT32            uiClkDiv;         ///< Clock divider
                                        ///<
                                        ///< Set SIF bus clock. SIF bus clock = APB clock / (2 * (uiClkDiv + 1))
                                        ///< @note Valid value: 0x001 ~ 0x3FF
    BOOL              bMSB;             ///< MSB or LSB shifted out first
                                        ///< - @b TRUE: MSB shifted out first
                                        ///< - @b FALSE: LSB shifted out first
    UINT32            uiSenH;           ///< SEN hold time, hold for X half SIF bus clock
                                        ///< @note Valid value: 0x00 ~ 0x07
} SIF_CTRL, *PSIF_CTRL;

extern ER       sif_open(void);
extern ER       sif_close(void);
extern BOOL     sif_isOpened(void);
extern ID       sif_getLockStatus(void);

extern void     sif_setControl(SIF_CH uiCh, PSIF_CTRL pSifCtrl);
extern void     sif_setSens(SIF_CH uiCh,UINT32 uiSens);
extern void     sif_send(SIF_CH uiCh, UINT32 uiData0, UINT32 uiData1, UINT32 uiData2);
extern void     sif_sendData(SIF_CH uiCh, UINT32 uiDataAddr, BOOL bintEn);
extern void     sif_sendMSB(SIF_CH uiCh, UINT32 uiData2);
extern void     sif_sendLSB(SIF_CH uiCh, UINT32 uiData0);

extern void     sif_waitTransferEnd(SIF_CH uiCh);

extern void     sif_getCurMode(SIF_CH uiCh, SIF_MODE *pMode, UINT32 *puiTxSz, BOOL *bMSB);
extern void     sif_getCurTiming(SIF_CH uiCh, UINT32 *puiDiv, UINT32 *puiSEND, UINT32 *puiSENH, UINT32 *puiSENS);



//Backward Compatible usage
#define sif_getCurrStatus(Mode, Conf2, Delay, Timing, Timing2, Ctrl, Ctrl2)     do{ }while(0)
#define sif_sendDMA(uiCh, uiDataAddr, uiDMATxWords)                             do{ }while(0)
#define sif_sendContDMA(uiCh, uiDataAddr)                                       do{ }while(0)
#define sif_waitDMATransferEnd()                                                FALSE
#define sif_setDelay(uiCh, uiDelay)                                             do{ }while(0)

#define sif_setContMode(uiCh, bEnContMode, uiAddrBits, uiContDataCount)         do{ }while(0)
#define sif_setContControl(uiCh, uiAddrBits, uiContDataCount)                   do{ }while(0)
//@}





#endif
