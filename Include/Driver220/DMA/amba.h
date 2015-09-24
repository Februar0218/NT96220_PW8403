/**
    Public header file for AMBA DMA module.

    This file is the header file that define the API and data type for AMBA
    DMA module.

    @file       amba.h
    @ingroup    mIHALAMBADMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _AMBA_H
#define _AMBA_H

#include "Type.h"

/**
    @addtogroup mIHALAMBADMA
*/
//@{

/**
    AMBA DMA channel ID definition.

    AMBA DMA channel ID definition.
*/
typedef enum
{
    AMBA_CH_A,              ///< AMBA DMA channel A
    AMBA_CH_B,              ///< AMBA DMA channel B
    AMBA_CH_C,              ///< AMBA DMA channel C
    AMBA_NUM,               ///< Total number of AMBA DMA channels
    ENUM_DUMMY4WORD(AMBACH)
} AMBACH;

/**
   AMBA request command setting.

   AMBA request command setting.
*/
typedef struct
{
    UINT32  uiSrcAddr;      ///< Source address
    UINT32  uiDstAddr;      ///< Destination address
    UINT32  uiCycleCnt;     ///< Cycle count
    BOOL    bBurstEn;       ///< Burst Enable
    UINT32  uiSrcSel;       ///< Source module select
    UINT32  uiDstSel;       ///< Destination module select
    UINT32  uiSrcAddrInc;   ///< Source address increment
    UINT32  uiDstAddrInc;   ///< Destination address increment
    UINT32  uiDataWidth;    ///< Data width
    UINT32  uiBurstMode;    ///< Burst mode
    UINT32  uiReqSel;       ///< Request select
} T_AMBA_CH_REQ_CMD;

/**
    AMBA request select setting.

    AMBA request select setting.
*/
typedef enum
{
    AMBA_REQ_SEL_DRAM,      ///< DMA request from DRAM
    AMBA_REQ_SEL_UART2_TX,  ///< DMA request from UART2 TX
    AMBA_REQ_SEL_UART2_RX,  ///< DMA request from UART2 RX
    AMBA_REQ_SEL_NAND,      ///< DMA request from NAND
    AMBA_REQ_SEL_USB,       ///< DMA request from USB
    ENUM_DUMMY4WORD(AMBA_REQ_SEL)
} AMBA_REQ_SEL;

/**
    AMBA request type setting.

    AMBA request type setting.
*/
typedef enum
{
    AMBA_REQ_TYPE_APB,      ///< DMA request type is APB
    AMBA_REQ_TYPE_AHB,      ///< DMA request type is AHB
    ENUM_DUMMY4WORD(AMBA_REQ_TYPE)
} AMBA_REQ_TYPE;

/**
    AMBA address increment setting.

    AMBA address increment setting.
*/
typedef enum
{
    AMBA_ADDR_INC_0,        ///< No address increment after one DMA cycle
    AMBA_ADDR_INC_1,        ///< Address increment 1 after one DMA cycle
    AMBA_ADDR_INC_2,        ///< Address increment 2 after one DMA cycle
    AMBA_ADDR_INC_3,        ///< Address increment 4 after one DMA cycle
    ENUM_DUMMY4WORD(AMBA_ADDR_INC)
} AMBA_ADDR_INC;

/**
    AMBA data width setting.

    AMBA data width setting.
*/
typedef enum
{
    AMBA_DATA_WID_WORD,     ///< Transfer data width is word
    AMBA_DATA_WID_HALFWORD, ///< Transfer data width is halfword
    AMBA_DATA_WID_BYTE,     ///< Transfer data width is byte
    ENUM_DUMMY4WORD(AMBA_DATA_WID)
} AMBA_DATA_WID;

/**
    AMBA burst mode setting.

    AMBA burst mode setting.
*/
typedef enum
{
    AMBA_BURST_MODE_4,      ///< DMA burst 4 mode
    AMBA_BURST_MODE_8,      ///< DMA burst 8 mode
    ENUM_DUMMY4WORD(AMBA_BURST_MODE)
} AMBA_BURST_MODE;

#define AMBA_DMA_BUSRT_SIZE_8           8
#define AMBA_DMA_BUSRT_SIZE_4           4

extern ER amba_open(AMBACH *p_uiDMAId);
extern ER amba_close(AMBACH uiDMAId);
extern ER amba_reqChannel(AMBACH uiDMAId, T_AMBA_CH_REQ_CMD *pCmd);
extern ER amba_wait(AMBACH uiDMAId);
extern ER amba_stop(AMBACH uiDMAId);
//#NT#2011/07/14#HH Chuang -begin
//#NT#Add new API to resume AMBA DMA
extern ER amba_resume(AMBACH uiDMAId);
//#NT#2011/07/14#HH Chuang -end
extern BOOL amba_checkEnable(AMBACH uiDMAId);
extern BOOL amba_handleError(AMBACH uiDMAId, BOOL bSynchronous);
extern UINT32 amba_getCycleCnt(AMBACH uiDMAId);
extern UINT32 amba_getAMBAChannleIdle(AMBACH uiDMAId);
//@}

#endif
