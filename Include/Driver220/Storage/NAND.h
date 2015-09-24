/**
    @file       NAND.h
    @ingroup    mIStgNAND

    @brief      Nand flash driver export variables and function prototypes.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

*/

#ifndef _NAND_H
#define _NAND_H

#ifndef _NAND2_SUPPORT_
#define _NAND2_SUPPORT_ 1
#endif

#include "StrgDef.h"

/**
@addtogroup mIStgNAND
*/
//@{

//------------------------------------------------------------
// compatible with earily code base
//------------------------------------------------------------
/**
    @name Backward compatible APIs

    For old version NAND driver
*/
//@{
#define EraseAllNANDBlockMaping()       nand_eraseAllBlockMapping()

#define nand_setSysParamBlockNum(m)
//@}

//------------------------------------------------------------
// The general api for the Nand device driver
//------------------------------------------------------------

//NAND driver error code
//bit 31...28    Card type : NAND : 0
//bit 27...12    NAND error
//bit 11...0     NAND / SMC /xD error

//#NT#2010/08/05#Steven Wang -begin
//#NT#Sector size use as definition
#define BYTES_PER_SECTOR        0x200
//#NT#2010/08/05#Steven Wang -end

// ------------------------------------------------------------------------
// NAND driver error code
// ------------------------------------------------------------------------
//
#define NAND_ERROR_OK                       0x00000000

#define NAND_ERROR_INSUFFICIENT_FREE_BLOCK  0x00010000
#define NAND_ERROR_TOO_MANY_BAD_UNITS       0x00080000 //Too many bad blocks result in write protection rasen.(write and erase is prohibited)

//NAND / SMC /xD common error (Please keep below error code number the same with xD driver (xD.h))
//Data error
#define NAND_ERROR_DATA_STATUS              0x00000001 //Data status is corrupt
#define NAND_ERROR_ECC_FIELD1               0x00000002
#define NAND_ERROR_ECC_FIELD2               0x00000004
#define NAND_ERROR_DOUBLE_MAPPING           0x00000008

//#NT#2010/06/23#Steven Wang -begin
//#NT#Add UN correct ecc to take over ECC_Field1 / ECC_Field2 error
#define NAND_ERROR_UNCORRECT_ECC            0x00000080
//#NT#2010/06/23#Steven Wang -end

//Operation error
#define NAND_ERROR_READ_ERROR               0x00000010
#define NAND_ERROR_WRITE_ERROR              0x00000020
#define NAND_ERROR_DEVICE_ERROR             0x00000040

typedef void (*PrCardProtectFunc) (BOOL);



/**
    @name   NAND device information

    @note for nand_getDeviceInfo()
*/
typedef struct
{
    UINT32 uiPhysicalBlockCount;    ///< blocks in the device
    UINT32 uiPagePerBlock;          ///< pages in a block
    UINT32 uiBytesPerPageData;      ///< data size in bytes (excluding spare area)
    UINT32 uiSparePerPage     ;     ///< spare size in each page
    UINT32 uiBlockSize;             ///< block size in bytes
    UINT16 uiCardType;              ///< Card type
    UINT8  ucMakerCode;             ///< 0xEC:samsung, 0x98:toshiba
    UINT8  ucDeviceCode;
} NandXdInfoStruct, *PNandXdInfoStruct;
//#NT#2010/12/06#Cliff -begin
//#Added for formatting level of nand flash
// Nand Format level
//#NT#2011/06/10#Steven Wang -begin
//NAND hamming / NAND RS identify
#define     SM_NAND_HAM                     0   //< nand flash with hamming ecc correction
#define     SM_NAND_RS                      1   //< nand flash with RS ecc correction
#define     SM_xD_CARD                      2   //< xD card

#define HAMMING_ERROR                       0x00000001  //< NAND controller hamming error
#define SOLOMON_ERROR                       0x00000002  //< NAND controller rs error
//#define COMMAND_ERROR                     0x00000003  //< NAND controller command error
#define HAMMING_ECC_ERROR                   0x00000004  //< NAND controller hamming ecc field error
#define SOLOMON_ECC_ERROR                   0x00000005  //< NAND controller solomon ecc field error

typedef struct
{
    BOOL   protectArea1EN;          //< Area 1 protect enable / disable
    BOOL   protectArea2EN;          //< Area 2 protect enable / disable
    UINT32 area1ProtectstartBlock;  //< Area 1 protect start block
    UINT32 area1ProtectendBlock;    //< Area 1 protect end block
    UINT32 area2ProtectstartBlock;  //< Area 2 protect start block
    UINT32 area2ProtectendBlock;    //< Area 2 protect end block
} SMCSetupProtectArea_t, * PSMCSetupProtectArea;

typedef enum
{
    SM_PIO_MODE = 0x0,
    SM_DMA_MODE,

    SMC_TRANS_MODE_NUM,
    ENUM_DUMMY4WORD(SM_TRANS_MODE)
} SM_TRANS_MODE;

typedef enum
{
    SM_SINGLE_PAGE = 0x0,
    SM_MULTIPLE_PAGE,

    SM_TRANS_TYPE_NUM,
    ENUM_DUMMY4WORD(SM_TRANS_TYPE)
} SM_TRANS_TYPE;
//#NT#2011/06/10#Steven Wang -end

/**
    @name   NAND format operation condition definition

    @note for nand_setFormatLevel()
*/
typedef enum
{
    NAND_FMTLEL_LOW,     ///< low level format
    NAND_FMTLEL_HIGH     ///< high level format
} NAND_FMTLEL;

//#NT#2010/12/06#Cliff -end

//#NT#2010/11/17#Cliff Lin -begin
//#NT#Mark bad block at valid time
// Nand  Feature
/**
    @name   NAND specific operation condition definition
\n          Some condition can not process anytime. Need turn on/off flag

    @note for nand_setFeature(), nand_getFeature()
*/
typedef enum
{
    NAND_ERASE_LOADER,      ///< TRUE: valid for erasing loader area
    NAND_ERASE_FW,          ///< TRUE: valid for erasing FW area
    NAND_ERASE_PSTORE,      ///< TRUE: valid for erasing Pstore area
    NAND_MARK_PSTORE        ///< TRUE: valid for mark bad block at Pstore area
} NAND_FEATRUE;
//#NT#2010/11/17#Cliff Lin -end


extern STRG_TAB gNandObj;
// Driver API

//#NT#2010/10/19#Steven Wang -begin
//#NT#Definition of nand_setInfo definition
/**
    @name NAND set information parameter

    @note for nand_setInfo()
*/
//@{
#define NAND_INFOR_END_TAG          0x444E4549      ///< Identify end tag of setting => 'I''E''N''D'
#define NAND_INFOR_PLL_TAG          0x47544C50      ///< Identify PLL tag of setting => 'P''L''T''G'
#define NAND_TIMING0_REG            0x98D0002C      ///< NAND controller timing0 register
#define NAND_TIMING1_REG            0x98D00030      ///< NAND controller timing1 register
#define NAND_SET_INFO_VER           0x00010001      ///< NAND setInfo version
//@}
//#NT#2010/10/19#Steven Wang -end

#define E_PROTECT1_AREA_ACCESS              (-200)
#define E_PROTECT2_AREA_ACCESS              (-201)

//Open standard storage API for low level access
extern ER        nand_open(void);
extern ER        nand_close(void);
extern ER        nand_readSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt);
extern ER        nand_writeSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt);
extern ER        nand_format(void);
extern UINT32    nand_getLastError(void);
extern void      nand_clearLastError(void);
#if _NAND2_SUPPORT_
extern PSTRG_TAB nand2_getStorageObject(void);
extern ER        nand2_writeSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern ER        nand2_readSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern void      nand2_setSize(UINT32 size);
ER               nand2_format(void);
void             nand2_FormatEraseNAND(void);
#endif
//#NT#2010/09/07#Steven Wang -begin
//#NT#Due to change param, do not public to upper layer if not necessary
//extern UINT32    nand_readID(void);
//#NT#2010/09/07#Steven Wang -end
extern void      nand_setReservedSize(UINT32 size);
//#NT#2009/02/04#Scottie -begin
//#NT#Add for new booting method
extern void      nand_setReservedStartBlockIndex(UINT32 BlockIndex);
//#NT#2009/02/04#Scottie -end
//extern void      nand_setSysParamBlockNum(UINT32 size);
extern UINT32    nand_getBlockSize(void);
//#NT#2010/06/23#Steven Wang -begin
//#NT#Get NAND page size
extern UINT32    nand_getPageSize(void);
//#NT#2010/06/23#Steven Wang -end
extern ER        nand_readSysParamBlockSeq(INT8 *pcBuf, UINT16 block, UINT16 count);
extern ER        nand_writeSysParamBlockSeq(INT8 *pcBuf, UINT16 block, UINT16 count);
extern ER        nand_readSysParamBlock(INT8 *pcBuf, UINT16 block);
extern ER        nand_writeSysParamBlock(INT8 *pcBuf, UINT16 block);
extern ER        nand_readSysParamBlockByBytes(INT8 *pcBuf, UINT16 block, UINT32 bytes);
extern ER        nand_writeSysParamBlockByBytes(INT8 *pcBuf, UINT16 block, UINT32 bytes);
extern ER        nand_readReservedBlock(INT8 *pcBuf, UINT16 uiLogicalBlockNum, UINT16 uiBlkCnt);
extern ER        nand_writeReservedBlock(INT8 *pcBuf, UINT16 uiLogicalBlockNum, UINT16 uiBlkCnt);
extern ER        nand_readReservedBlockByBytes(INT8 *pcBuf, UINT16 uiLogicalBlockNum, UINT32 bytes);
extern ER        nand_setIntEn(BOOL bEn);
extern void      nand_enableCardProtection(BOOL bProtection);
extern PSTRG_TAB nand_getStorageObject(void);
extern PRSVSTRG_TAB nand_getReserveObject(void);
extern void      nand_regCardWriteProtectFunc(PrCardProtectFunc CardWriteProtectFunc);
extern BOOL      nand_getDeviceInfo(PNandXdInfoStruct pNandInfo);

extern BOOL      nand_eraseCheckBadBlock(UINT32 block_num);
extern UINT32    nand_eraseCheckFAT(void);
extern BOOL      nand_readCheckBadBlock(UINT32 block_num);
extern UINT32    nand_readCheckAll(BOOL bDumpAllBlock);
extern UINT32    nand_readCheckAllBadBlocks(BOOL bMarkErrorBlock);

extern ER        nand_readOperation(INT8 *buffer, UINT32 pageAddress, UINT32 numPage);
extern ER        nand_readPageSpareData1(UINT32 *spare1, UINT32 pageAddress);
extern ER        nand_readPageSpareDataTotal(UINT32 *spare0,UINT32 *spare1, UINT32 *spare2, UINT32 *spare3, UINT32 pageAddress);
//#NT#2010/04/07#Steven Wang -begin
//#NT#Multispare read by DMA access
extern ER        nand_readMultiSpareData(UINT8 * pulBuf, UINT32 block, UINT32 interval, UINT32 cnt);
//#NT#2010/04/07#Steven Wang -end
//#NT#2009/01/12#Brad Chen -begin
//dump P_STore Table
extern void      nandps_getpstoreMapTable(UINT16 ** MapTable);
//#NT#2009/01/12#Brad Chen -end

//#NT#2009/02/04#Scottie -begin
//#NT#Earse all nand data except block 0,1,2,4
extern void      nand_eraseAllBlockMapping(void);
//#NT#2009/02/04#Scottie -end




//#NT#2009/04/13#Brad Chen -begin
//#NT# Protect erase_block with valid timming
//extern void nand_setValidErase(BOOL bValidErase);
//#NT#2009/04/13#Brad Chen -end
//#NT#2011/01/11#Cliff Lin -begin
//#NT#Remove sram from NT96220
#if 0
extern void nand_setCPUAccessErr(BOOL bcpu_Acc);
extern void nand_setCPUAccessSpare(BOOL bcpu_Acc);
#endif
//#NT#2011/01/11#Cliff Lin -end
//#NT#2009/09/09#Cliff Lin -begin
extern ER     nand_writeOneReservedBlock(UINT32 PhyBlkIdx, UINT32 LogBlkIdx, UINT8 *Buf);
extern UINT32 nand_getReservedSize(void);
extern UINT32 nand_getReservedStartBlockIndex(void);
//#NT#2009/09/09#Cliff Lin -end

//#NT#2009/09/30#Cliff Lin -begin
//#NT#Add new function to check nand
extern UINT32 nand_readwriteCheckFAT(INT8 *pBuf);
extern UINT32 nand_readwriteCheckAll(INT8 *pBuf, UINT32 uistart_block);
//#NT#2009/09/30#Cliff Lin -end

//#NT#2009/09/30#Cliff Lin -begin
//#NT#Add new function to set spare directly
//Move to nand_int.h, internal usage
//extern void nand_setSpare(UINT32 spare0,UINT32 spare1, UINT32 spare2, UINT32 spare3);
//#NT#2009/09/30#Cliff Lin -end

//#NT#2010/01/28#Cliff Lin -start
//#NT# Add general feature to config Nand driver.
extern ER nand_setFeature(NAND_FEATRUE Nand_Feature, UINT32 uiParameter);
extern ER nand_getFeature(NAND_FEATRUE Nand_Feature, UINT32 *uiParameter);
//#NT#2010/01/28#Cliff Lin -end

//#NT#2010/06/08#Cliff -begin
//#NT#Add new API for saving system parameter
extern ER nand_readSysParamArea(UINT8 *pcBuffer,UINT32 BufferSize);
extern ER nand_writeSysParamArea(UINT8 *pcBuffer,UINT32 BufferSize);
//#NT#2010/06/08#Cliff -end
extern UINT32 nand_readCheckInvalidAll(UINT32 StartBlock,BOOL MarkBadBlock);
extern UINT32 nand_readCheckAllNand(UINT32 StartBlock);

//#NT#2010/10/19#Steven Wang -begin
extern UINT32 nand_getTiming0(void);
extern UINT32 nand_getTiming1(void);
extern ER nand_setInfo(UINT32 * uiInfo);
extern void nand_readTimeInfo(void);
//#NT#2010/10/19#Steven Wang -end
//#NT#2010/12/06#Cliff -begin
//#Added for formatting level of nand flash
extern void nand_setFormatLevel(NAND_FMTLEL FMTLEVEL);
//#NT#2010/12/06#Cliff -end

//#NT#2011//06/10#Steven Wang -begin
//#NT#For Emulation test
extern ER nand3_open(UINT32 uiModule, UINT32 uiMode, UINT32 uiType, UINT32 uiClockRate);
extern UINT32 nand3_getBlockSize(void);
extern ER nand3_readSectors(UINT32 uiModule, UINT32 startblk, UINT32 length, UINT8 * buffer, BOOL uiECCOn);
extern ER nand3_writeSectors(UINT32 uiModule, UINT32 startblk, UINT32 length, UINT8 * buffer, BOOL uiECCOn);
extern ER nand3_setupProtectArea(PSMCSetupProtectArea pSetupProtectArea);
extern UINT32 nand3_getTotalSize(void);
extern UINT32 nand3_getPageSize(void);
extern void nand3_setupWriteReadCheckPage(BOOL uiEN);
extern void nand3_setupMakeECCPattern(BOOL uiEN);
extern BOOL nand3_getIsMLC(void);
extern ER  SMHostEraseBlock(UINT32 module, UINT32 block_num);
extern ER nand3_readSpareData(UINT32 module, UINT32 startblk, UINT32 pageCnt, UINT8 * buffer);
extern void nand3_setupMakeECCFieldPattern(BOOL uiEN);
extern UINT32 * nand3_getSpareAreaData(void);
extern UINT32 * nand3_getTempSpareAreaData(void);
extern UINT32 * nand3_getReadSpareAreaData(void);
extern UINT32 nand3_getRdtSizePerBlock(void);
//#NT#2011//06/10#Steven Wang -end

#endif // _NAND_H

//@}
