/**
    Header file for DMA module.

    This file is the header file that define the API for DMA module.

    @file       DMA.h
    @ingroup    mIHALDMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _DMA_H
#define _DMA_H

#include "Type.h"

/**
    @addtogroup mIHALDMA
*/
//@{

/**
    @name Write protect channel status type define
*/
//@{
typedef UINT64      WP_STS_TYPE;
//@}


/**
    @name DMA controller write protect status bit generation

    Generate write protect status bit according to write protect channel.
    One channel at a time. ch must be one of DMA_WRITEPROT_CH enum.
*/
//@{
#define DMA_WP_STS_BITS(ch)                 (WP_STS_TYPE)(1ull << (ch))
//@}


/**
    @name Write protect call back function

    @note for dma_configWPFunc()
*/
//@{
typedef void (*DMA_WPCallbackFunc)(void);
//@}

/**
    @name DMA priority setting

    NT96220 DMA priority setting (High > Middle > Normal)

    @note Used in dma_setChannelPriority(), dma_getChannelPriority()
*/
//@{
typedef enum
{
    DMA_PRIORITY_NORMAL,        ///< Normal priority
    DMA_PRIORITY_MIDDLE,        ///< Middle priority
    DMA_PRIORITY_HIGH,          ///< High priority
    DMA_PRIORITY_INVALID,       ///< invalid priority
    ENUM_DUMMY4WORD(DMA_PRIORITY)
} DMA_PRIORITY;
//@}

/**
    @name DMA channel encoding

    NT96220 DMA channel setting

    @note Used in dma_setChannelPriority(), dma_getChannelPriority(),
                  dma_enableChannel(), dma_disableChannel()
*/
//@{
typedef enum
{
    DMA_CH_RSV = 0x00,          ///< RSV
    DMA_CH_SIE0,                ///< SIE0   (Write)
    DMA_CH_SIE1,                ///< SIE1   (Write)
    DMA_CH_SIE2,                ///< SIE2   (Read)
    DMA_CH_SIE3,                ///< SIE3   (Read)
    DMA_CH_IDE_Y_INPUT,         ///< IDE Y  (Read)
    DMA_CH_IDE_CB_INPUT,        ///< IDE CB (Read)
    DMA_CH_IDE_CR_INPUT,        ///< IDE CR (Read)
    DMA_CH_IDE_OSD_INPUT,       ///< IDE OSD(Read)
    DMA_CH_MPU,                 ///< MPU    (Read)
    DMA_CH_DAC0,                ///< DAC0   (Read)
    DMA_CH_IPE0,                ///< IPE0   (Read)
    DMA_CH_IME0,                ///< IME0   (Read)
    DMA_CH_IME1,                ///< IME1   (Read)
    DMA_CH_IME2,                ///< IME2   (Read)
    DMA_CH_IME3,                ///< IME3   (Write)
    DMA_CH_IME4,                ///< IME4   (Write)
    DMA_CH_IME5,                ///< IME5   (Write)
    DMA_CH_IPE1,                ///< IPE1   (Read)
    DMA_CH_IME7,                ///< IME7   (Write)
    DMA_CH_IME8,                ///< IME8   (Write)
    DMA_CH_IPE2,                ///< IPE2   (Read)
    DMA_CH_JPG_IMG,             ///< JPG IMG(Read/Write)
    DMA_CH_JPG_BS,              ///< JPG BS (Read/Write)
    DMA_CH_DC_OUTPUT,           ///< JPG Encode mode DC output (Write)
    DMA_CH_AHBC,                ///< AHBC   (Read/Write)
    DMA_CH_GRPH_INPUT0,         ///< Graphic input channel 0    (Read)
    DMA_CH_GRPH_INPUT1,         ///< Graphic input channel 1    (Read)
    DMA_CH_SDIO,                ///< SDIO   (Read/Write)
    DMA_CH_SPI,                 ///< SPI    (Read/Write)
    DMA_CH_DAI0,                ///< DAI0   (Read/Write)
    DMA_CH_DAI1,                ///< DAI1   (Read/Write)
    DMA_CH_IFE0,                ///< IFE0   (Read)
    DMA_CH_IFE1,                ///< IFE1   (Write)
    DMA_CH_GRPH_OUTPUT,         ///< Graphic Output channel (Write)
    DMA_CH_COUNT,
    ENUM_DUMMY4WORD(DMA_CH)
} DMA_CH;
//@}

/**
    @name DRAM type encoding

    DRAM type encoding returned by dma_getDRAMType()
*/
//@{
// DRAM TYPE
typedef enum
{
    DMA_DRAM_TYPE_DDR1,          ///< DDR1 DRAM
    DMA_DRAM_TYPE_SDR,           ///< SDR  DRAM
    ENUM_DUMMY4WORD(DMA_DDR_TYPE)
} DMA_DDR_TYPE;
//@}

/**
    @name DRAM protect set

    DRAM type encoding returned by dma_getDRAMType()

    @note Used in dma_chkDmaWR2ProtectAddr(), dma_clrWPStatus(), dma_getWPStatus(),
                  dma_configWPFunc(), dma_enableWPFunc(), dma_disableWPFunc()
*/
//@{
// Protect Source
typedef enum
{
    DMA_WPSET_0,            ///< Write protect function set 0
    DMA_WPSET_1,            ///< Write protect function set 1
    DMA_WPSET_2,            ///< Write protect function set 2
    DMA_WPSET_TOTAL,
    ENUM_DUMMY4WORD(DMA_WRITEPROT_SET)
} DMA_WRITEPROT_SET;
//@}


/**
    @name DRAM write protect source
*/
//@{
// Protect Source
typedef enum
{
    DMA_WPSRC_CPU,          ///< from CPU, including AMBA DMA, AHBC.
    DMA_WPSRC_DMA,          ///< DMA
    DMA_WPSRC_ALL,          ///< CPU+DMA
    ENUM_DUMMY4WORD(DMA_WRITEPROT_SRC)
} DMA_WRITEPROT_SRC;
//@}


/**
    @name DRAM write protect detect level
*/
//@{
// Protect level
typedef enum
{
    DMA_WPLEL_DETECT,       ///< Only detect write action but allow write access.
    DMA_WPLEL_UNWRITE,      ///< Not only detect write action but also denial access.
    ENUM_DUMMY4WORD(DMA_WRITEPROT_LEVEL)
} DMA_WRITEPROT_LEVEL;
//@}

/**
    @name DMA write protect channel name
*/
// The Channel of Writing to protected address
typedef enum
{
    DMA_WPCH_FIRST_CHANNEL = 2,
    DMA_WPCH_SIE0 = DMA_WPCH_FIRST_CHANNEL, ///< SIE0     (Write)
    DMA_WPCH_SIE1,                          ///< SIE1     (Write)
    DMA_WPCH_SIE2,                          ///< SIE2     (Read)
    DMA_WPCH_SIE3,                          ///< SIE3     (Read)
    DMA_WPCH_IDE_Y_INPUT,                   ///< IDE Y    (Read)
    DMA_WPCH_IDE_CB_INPUT,                  ///< IDE CB   (Read)
    DMA_WPCH_IDE_CR_INPUT,                  ///< IDE CR   (Read)
    DMA_WPCH_IDE_OSD_INPUT,                 ///< IDE OSD  (Read)
    DMA_WPCH_MPU,                           ///< MPU      (Read)
    DMA_WPCH_DAC0,                          ///< DAC0     (Read)
    DMA_WPCH_IPE0,                          ///< IPE0     (Read)
    DMA_WPCH_IME0,                          ///< IME0     (Read)
    DMA_WPCH_IME1,                          ///< IME1     (Read)
    DMA_WPCH_IME2,                          ///< IME2     (Read)
    DMA_WPCH_IME3,                          ///< IME3     (Write)
    DMA_WPCH_IME4,                          ///< IME4     (Write)
    DMA_WPCH_IME5,                          ///< IME5     (Write)
    DMA_WPCH_IPE1,                          ///< IPE1     (Read)
    DMA_WPCH_IME7,                          ///< IME7     (Write)
    DMA_WPCH_IME8,                          ///< IME8     (Write)
    DMA_WPCH_IPE2,                          ///< IPE2     (Read)
    DMA_WPCH_JPG_IMG,                       ///< JPG IMG  (Read/Write)
    DMA_WPCH_JPG_BS,                        ///< JPG BS   (Read/Write)
    DMA_WPCH_DC_OUTPUT,                     ///< JPG Encode mode DC output (Write)
    DMA_WPCH_AHBC,                          ///< AHBC     (including CPU & AMBA access, not local DMA channel is AHBC)(Read/Write)
    DMA_WPCH_GRPH_INPUT0,                   ///< Graphic  (Read)
    DMA_WPCH_GRPH_INPUT1,                   ///< Graphic  (Read)
    DMA_WPCH_SDIO,                          ///< SDIO     (Read/Write)
    DMA_WPCH_SPI,                           ///< SPI      (Read/Write)
    DMA_WPCH_DAI0,                          ///< DAI0     (Read/Write)
    DMA_WPCH_DAI1,                          ///< DAI1     (Read/Write)
    DMA_WPCH_IFE0,                          ///< IFE0     (Read)
    DMA_WPCH_IFE1,                          ///< IFE0(33) (Write)
    DMA_WPCH_GRPH_OUTPUT,                   ///< Graphic Output channel (Write)
    DMA_WPCH_ALL,
    ENUM_DUMMY4WORD(DMA_WRITEPROT_CH)
} DMA_WRITEPROT_CH;
//@}


/**
    @name DMA controller write protect function configuration

*/
//@{
typedef struct
{
    DMA_WRITEPROT_SRC   uiProtectSrc;   ///< protect source
    DMA_WRITEPROT_LEVEL uiProtectlel;   ///< protect level
    UINT32              uiStartingAddr; ///< DDR2:must be 2 words alignment, DDR3:must be 4 words alignment
    UINT32              uiSize;         ///< DDR2:must be 2 words alignment, DDR3:must be 4 words alignment
} DMA_WRITEPROT_ATTR, *PDMA_WRITEPROT_ATTR;
//@}



/**
    @name DMA controller dram configuration capacity
*/
//@{
typedef enum
{
    DMA_SDRAM_128Mb = 0,        ///< 128 Mb ->16MB
    DMA_SDRAM_256Mb,            ///< 256 Mb ->32MB
    DMA_SDRAM_64Mb = 5,         ///<  64 Mb -> 8MB

    ENUM_DUMMY4WORD(DMA_DRAM_CAPACITY)
} DMA_DRAM_CAPACITY;
//@}



/**
    @name DMA controller down grade dram support type
*/
//@{
// The Down grade Dram type
typedef enum
{
    DMA_NORMAL_DRAM_128M,       ///< Normal Dram support
    DMA_DG_DRAM_A11_EQU_0,      ///< A11 = 0
    DMA_DG_DRAM_A11_EQU_1,      ///< A11 = 1
    DMA_DG_DRAM_A11_EQU_BA1,    ///< A11 = BA1
    DMA_DG_DRAM_A11_EQU_BA1_INV,///< A11 = ~BA1
    DMA_DG_DRAM_BA1_EQU_0,      ///< BA1 = 0
    DMA_DG_DRAM_BA1_EQU_1,      ///< BA1 = 1
    DMA_DG_DRAM_BA1_EQU_BA0,    ///< BA1 = BA0
    DMA_DG_DRAM_BA1_EQU_BA0_INV,///< BA1 = ~BA0
    DMA_DG_DRAM_A11_EQU_BA0,    ///< A11 = BA0
    DMA_DG_DRAM_A11_EQU_BA0_INV,///< A11 = ~BA0
    DMA_DG_DRAM_BA0_EQU_0,      ///< BA0 = 0
    DMA_DG_DRAM_BA0_EQU_1,      ///< BA0 = 1
    DMA_DG_DRAM_TYPE_NUM,

    ENUM_DUMMY4WORD(DOWNGRADE_DRAM_TYPE)
} DOWNGRADE_DRAM_TYPE;
//@}


/**
    @name DMA controller key scan function input signal polarity
*/
//@{
typedef enum
{
    DMA_KEYSCAN_NO_INVERT = 0,              ///< DMA dram keyscan detect no invert original signal -> default low, high active
    DMA_KEYSCAN_INVERT,                     ///< DMA dram keyscan detect do Invert original signal -> default high, low active
    ENUM_DUMMY4WORD(DRAM_KEYSCAN_POLARITY)
} DMA_KEYSCAN_POLARITY;
//@}


#if 0
/*
    @name DMA controller key scan function enable / disable

    Take address 0 ~ 11 as input IO detect
*/
//@{
typedef enum
{
    DRAM_KEYSCAN_DISABLE = 0,   ///< Disable dram address PIN as input IO detection
    DRAM_KEYSCAN_ENABLE,        ///< Enable dram address PIN as input IO detection
    ENUM_DUMMY4WORD(DRAM_KEYSCAN_EN)
} DMA_KEYSCAN_EN;
//@}
#endif

/**
    @name DMA controller key scan function address bit

    Take address 0 ~ 11 as input IO detect
*/
//@{
typedef enum
{
    DMA_KEY_A0 = 0,            ///< enable dram address PIN A0 as input IO detection
    DMA_KEY_A1,                ///< enable dram address PIN A1 as input IO detection
    DMA_KEY_A2,                ///< enable dram address PIN A2 as input IO detection
    DMA_KEY_A3,                ///< enable dram address PIN A3 as input IO detection
    DMA_KEY_A4,                ///< enable dram address PIN A4 as input IO detection
    DMA_KEY_A5,                ///< enable dram address PIN A5 as input IO detection
    DMA_KEY_A6,                ///< enable dram address PIN A6 as input IO detection
    DMA_KEY_A7,                ///< enable dram address PIN A7 as input IO detection
    DMA_KEY_A8,                ///< enable dram address PIN A8 as input IO detection
    DMA_KEY_A9,                ///< enable dram address PIN A9 as input IO detection
    DMA_KEY_A10,               ///< enable dram address PIN A10 as input IO detection
    DMA_KEY_A11,               ///< enable dram address PIN A11 as input IO detection
    TOTAL_KEYSCAN_NUM,
    ENUM_DUMMY4WORD(DRAM_KEYSCAN_ADDR_BIT)
} DMA_KEYSCAN_ADDR_BIT;
//@}


#if 0
extern void         dma_setPriority(UINT32 uiP0, UINT32 uiP1);
extern void         dma_getPriority(UINT32 *puiP0, UINT32 *puiP1);
#endif

extern UINT32       dma_getDQSInDelay(void);
extern UINT32       dma_getDQOutDelay(void);
extern UINT32       dma_getClockDelay(void);


extern ER           dma_setChannelPriority(DMA_CH Channel, DMA_PRIORITY uiPriority);
extern ER           dma_getChannelPriority(DMA_CH Channel, DMA_PRIORITY* pPriority);

extern DMA_DDR_TYPE dma_getDRAMType(void);
extern void         dma_setPrechargePowerDown(BOOL flag);

extern void         dma_flushReadCache(UINT32 uiStartAddr, UINT32 uiLength);
extern void         dma_flushWriteCache(UINT32 uiStartAddr, UINT32 uiLength);

//#NT#2011/01/25#Steven Wang -begin
//#NT#Removed these two api, not necessary get / release access right
//extern BOOL       dma_getWPFuncRight(DMA_WRITEPROT_SET WpSet);
//extern BOOL       dma_releaseWPFuncRight(DMA_WRITEPROT_SET WpSet);
//#NT#2011/01/25#Steven Wang -end
extern BOOL         dma_chkDmaWR2ProtectAddr(DMA_WRITEPROT_SET WpSet, DMA_WRITEPROT_CH chkChannel);
extern void         dma_clrWPStatus(DMA_WRITEPROT_SET WpSet, DMA_WRITEPROT_CH chkChannel);
//#NT#Change parameter about status from UINT32 to UINT64(use typedef WP_STS_TYPE)
extern void         dma_getWPStatus(DMA_WRITEPROT_SET WpSet, WP_STS_TYPE * pWpSts);
extern void         dma_configWPFunc(DMA_WRITEPROT_SET WpSet, PDMA_WRITEPROT_ATTR PprotectAttr, DMA_WPCallbackFunc pIsrHdl);
extern void         dma_enableWPFunc(DMA_WRITEPROT_SET WpSet);
extern void         dma_disableWPFunc(DMA_WRITEPROT_SET WpSet);

extern ER           dma_enableChannel(DMA_CH Channel);
extern ER           dma_disableChannel(DMA_CH Channel);

extern UINT32       dma_getDRAMCapacity(void);
//#NT#2011/07/20#Steven Wang -begin
//#NT#Not export to project layer
//extern ER          dma_setDownGradeDramType(DOWNGRADE_DRAM_TYPE uiDgDramType);
//#NT#2011/07/20#Steven Wang -end

extern ER           dma_setKeyScanEN(DMA_KEYSCAN_ADDR_BIT uiAddrBit, BOOL uiEN);
extern UINT32       dma_getKeyScanEN(void);
extern ER           dma_setKeyScanPolarity(DMA_KEYSCAN_ADDR_BIT uiAddrBit, DMA_KEYSCAN_POLARITY uiPol);
extern void         dma_showWPChannel(DMA_WRITEPROT_SET WpSet, WP_STS_TYPE WpSts);
extern DMA_KEYSCAN_POLARITY dma_getKeyScanPolarity(DMA_KEYSCAN_ADDR_BIT uiAddrBit);

//@}

#endif
