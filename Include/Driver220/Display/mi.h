/**
    IDE Memory Interface header

    IDE Memory Interface header

    @file       mi.h
    @ingroup    mIDISPMI
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/


#ifndef _MI_H
#define _MI_H

#include "Type.h"

/** \addtogroup mIDISPMI */
//@{


/**
    memory interface mode

    memory interface mode, used for mi_setCtrl().
*/
typedef enum {
    PARALLEL_I80,       ///< Intel 8080 system interface
    PARALLEL_M68,       ///< Motorola 6800 system interface
    SERIAL_3WSPI,       ///< 3 wire serial interface with RS bit.
    SERIAL_4WSPI,       ///< 4 wire serial interface with RS pin.
    SERIAL_3WSPI_SB,    ///< 3 wire serial interface with start byte.

    ENUM_DUMMY4WORD(MI_TYPE)
} MI_TYPE;

/**
    memory interface input source

    memory interface input source, used for mi_setSrc().
*/
typedef enum {
    SRC_IDE,        ///< From IDE
    SRC_DRAM,       ///< From DRAM

    ENUM_DUMMY4WORD(MI_SRC)
} MI_SRC;

/**
    memory interface auto mode

    memory interface auto mode, used for mi_setCtrl().
*/
typedef enum {
    MI_AUTOFMT0 ,       ///< One frame only, auto clear MI enable after each frame.
    MI_AUTOFMT1 ,       ///< FRM1 + FRM2 + ...
    MI_AUTOFMT2 ,       ///< CMDs + FRM1 + FRM2 + ...
    MI_AUTOFMT3 ,       ///< CMDs + FRM1 + CMDs + FRM2 + ...

    ENUM_DUMMY4WORD(MI_AUTO_FMT)
} MI_AUTO_FMT;

/**
    memory interface auto dataformat.

    memory interface auto dataformat, used for mi_setCtrl().
*/
typedef enum {
    MI_DATAFMT0 ,       ///< 8bit/Serial - RGB444
    MI_DATAFMT1 ,       ///< 8bit/Serial - RGB565
    MI_DATAFMT2 ,       ///< 8bit/Serial - RGB565
    MI_DATAFMT3 ,       ///< 8bit/Serial - RGB666
    MI_DATAFMT4 ,       ///< 8bit/Serial - RGB666
    MI_DATAFMT5 ,       ///< 8bit/Serial - RGB666
    MI_DATAFMT6 ,       ///< 8bit/Serial - RGB675
    MI_DATAFMT7 ,       ///< 9bit - RGB666
    MI_DATAFMT8 ,       ///< 16bit - RGB565
    MI_DATAFMT9 ,       ///< 16bit - RGB666
    MI_DATAFMT10,       ///< 18bit - RGB666
    MI_DATAFMT11,       ///< 8bit - RGB888
    MI_DATAFMT12,       ///< 16bit - RGB888
    MI_DATAFMT13,       ///< 16bit - RGB666
    MI_DATAFMT14,       ///< 16bit - RGB666

    ENUM_DUMMY4WORD(MI_DATAFMT)
} MI_DATAFMT;

/**
    memory interface CMD type.

    memory interface CMD type, used for mi_setCmd*().
*/
typedef enum {
    WrCMD=0  ,          ///< Write command
    WrDATA   ,          ///< Write data
    RdCMD    ,          ///< Read command
    RdDATA   ,          ///< Read data

    ENUM_DUMMY4WORD(MI_CMD_TYPE)
} MI_CMD_TYPE;

/**
    memory interface SCL mode.

    memory interface SCL mode, used for mi_setPolCtrl().
*/
typedef enum {
    SCLMOD0=0,
    SCLMOD1  ,
    SCLMOD2  ,
    SCLMOD3  ,

    ENUM_DUMMY4WORD(MI_SCLMODE)
} MI_SCLMODE;

/**
    MI dithering valid bits.

    MI dithering valid bits, used for mi_setDitherVbits
*/
typedef enum {
    MI_DITHER_4BITS = 0,
    MI_DITHER_5BITS = 1,
    MI_DITHER_6BITS = 2,
    MI_DITHER_7BITS = 3
} MI_DITHER_VBITS;

/**
    MI Timing configuration.

    MI Timing configuration.
*/
typedef struct {
    UINT32  uiTCS;       ///< tCS Timing
    UINT32  uiTRS;       ///< tRS Timing
    UINT32  uiTRD_R;     ///< tRD release period for RD signal
    UINT32  uiTRD_A;     ///< tRD active period for RD signal
    UINT32  uiTWR_R;     ///< tWR release period for WR signal
    UINT32  uiTWR_A;     ///< tWR active period for WR signal
    UINT32  uiCMD_DLY;   ///< tCMD delay
} MI_TIMING, *PMI_TIMING;


//-------------------------------------------------
// The general api for MI
//-------------------------------------------------
extern  ER          mi_attach(void);
extern  ER          mi_detach(void);
extern  ER          mi_open(void);
extern  ER          mi_close(void);
extern  BOOL        mi_isOpened(void);

extern  ER          mi_setEn(BOOL bEn);
extern  BOOL        mi_getEn(void);
extern  ER          mi_setCtrl(MI_TYPE Type,MI_AUTO_FMT AutoType,  MI_DATAFMT DataFMT,
                                BOOL bRGBSWAP, BOOL bBITSWAP, BOOL bSync, BOOL bPINMUX);
extern  ER          mi_getCtrl(MI_TYPE *Type,MI_AUTO_FMT *AutoType,  MI_DATAFMT *DataFMT,
                                BOOL *bRGBSWAP, BOOL *bBITSWAP, BOOL *bSync, BOOL *bPINMUX);
extern  MI_TYPE     mi_getType(void);
extern  ER          mi_setManual(BOOL bEn);
extern  MI_DATAFMT  mi_getDataFmt(void);
extern  ER          mi_setSyncTiming(UINT32 uiClkCnt, UINT8 uiLineCnt,BOOL bPolarity, BOOL bSyncSel);

extern  ER          mi_setCmds(UINT8 uiCmdCnt,UINT32 *Cmds,MI_CMD_TYPE *CmdType);
extern  ER          mi_setCmd1(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd2(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd3(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd4(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd5(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd6(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd7(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd8(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmdCnt(UINT8 uiCmdCnt);

extern  UINT32      mi_getReadBack(UINT8 CmdIdx);
extern  ER          mi_setPolCtrl(BOOL bCSINV,BOOL bRSINV, BOOL bRDINV, BOOL bWRINV, MI_SCLMODE bSCLMOD);
extern  ER          mi_setSerialCsCtrl(BOOL bSerCS);
extern  ER          mi_setCsAuto(BOOL bCsAuto);
extern  ER          mi_setCsLevel(BOOL bLevel);
extern  ER          mi_setIntEnable(BOOL bCmdDone, BOOL bFrmEnd,BOOL bFifoOver, BOOL bSyncErr, BOOL bBWF);
extern  ER          mi_getIntEnable(BOOL *pbCmdDone, BOOL *pbFrmEnd,BOOL *pbFifoOver, BOOL *pbSyncErr, BOOL *bBWF);
extern  ER          mi_setSerialAutoRsBitValue(BOOL bVal);

extern  void        mi_setDithering(BOOL b_en, BOOL b_freerun);
extern  void        mi_setDitherVbits(MI_DITHER_VBITS b_rsel, MI_DITHER_VBITS b_gsel, MI_DITHER_VBITS b_bsel);

extern  ER          mi_waitCmdDone(void);
extern  ER          mi_waitFrameEnd(void);

extern  void        mi_isr(void);
extern  ER          mi_setSrc(MI_SRC Src);
extern  ER          mi_setLoad(void);
extern  ER          mi_setBufDim(UINT32 uiWidth, UINT32 uiHeight, UINT32 uilofs);
extern  ER          mi_setAddr0(UINT32 uiBufAddr);

extern  ER          mi_getAddr0(UINT32 *uiBufAddr);
extern  ER          mi_getBufDim(UINT32 *uiWidth, UINT32 *uiHeight, UINT32 *uilofs);

extern  void        mi_setTiming(MI_TIMING mi_timing);
//@}
#endif
