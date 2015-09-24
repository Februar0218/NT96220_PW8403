/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CopyRest.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of copy rest FW code
                Header file of copy rest FW code.

    @note       Nothing.

    @date       2006/02/08
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _COPYREST_H
#define _COPYREST_H

#include "SysCfg.h"

// Maximum FW code size
#define FW_CODE_SIZE                FRE_MEM

// Total SPI/nand storage size
#define OnBorad_Storage_Size        0x200000


// The following definitions should be sync to definitions in Boot Loader

// FW code in NAND block
#define FW_START_NAND_BLOCK         2

// The offset is related to Startup.s and VersionInfo.s
// Module name
#define FW_MODULE_OFFSET            0x50
// Length
#define FW_LENGTH_OFFSET            (FW_MODULE_OFFSET + 0x18)
// Checksum or CRC or ...
#define FW_CHECK_OFFSET             (FW_MODULE_OFFSET + 0x1C)
// Length checking
#define FW_LENGTH_CHECK_OFFSET      (FW_MODULE_OFFSET + 0x20)
// How many bytes that loader did FW partial loading
#define FW_PARTIAL_LOADING_OFFSET   (FW_MODULE_OFFSET + 0x24)
//#NT#2009/02/04#Scottie -begin
// Main code Reserved NAND Size
#define FW_RESERVED_SIZE_OFFSET     (FW_MODULE_OFFSET + 0x28)
//#NT#2009/02/04#Scottie -end
// Binary tag
#define FW_BINARY_TAG_OFFSET        (FW_MODULE_OFFSET + 0x2C)
//#NT#2009/07/03#Chris Chung -begin
//#NT#support running partial compressed FW from card
// Reserved1
#define FW_RESERVED1_OFFSET         (FW_MODULE_OFFSET + 0x30)

//#define FW_CRC_TAG_OFFSET           (FW_MODULE_OFFSET + 0x34)
//#NT#2009/11/10#Steven Wang -begin
//#NT#This param reserved for NT96450 compatible
//#define FW_RESERVED2_FLAG_OFFSET    (FW_MODULE_OFFSET + 0x38)
//#NT#if this offset == 0, remain code in NAND did not compress
//#NT#if this offset == 1, remain code in NAND was compressed
#define FW_COMPRESS_FLAG_OFFSET     (FW_MODULE_OFFSET + 0x3C)

#define FLASH_SIZE_OFFSET           (FW_MODULE_OFFSET + 0x40)
//#NT#2009/11/10#Steven Wang -end
// Binary tag value
#define FW_BINARY_TAG               0xAA55544E // 'N' 'T' 0x55 0xAA

//#NT#2008/12/09#Chris Chung -begin
//#Support Partial Load and FW Decompression
#define FW_COMPRESS_HEADER_SIZE     16          // header size for compressed FW (16 bytes)

#define FW_COMPRESS_BUF_ADDR        0x00500000  // use from 5M Byte
//#NT#2008/12/09#Chris Chung -end

//#NT#2008/11/27#Hideo Lin -begin
//#NT#Define partial load related parameters (use constant temporary)
#define LOADER_SIZE         0x4000      // 16KB
#define PART_ONE_FW_SIZE    0x00030000  // 192KB
#define SPI_SECTOR_SIZE     0x1000      // 4KB
#define SPI_BLOCK_SIZE      0x10000     // 64KB
//#NT#2008/11/27#Hideo Lin -end

extern void     CopyRest_RunPartOne(void);
extern void     CopyRest_CopyPartTwo(void);
extern void     CopyRest_RunPartTwo(void);
//extern void     CopyRest_CopyPartThree(void);

#endif

//@}
