/*
    @file       SPIFLASH_int.h
    @ingroup    mISPIFLASH

    @brief      SPI Flash driver internal header file

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _spiflash_INT_H
#define _spiflash_INT_H

#include "Type.h"
#include "Strgdef.h"
#include "SPI.h"

#define SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE   (256)
#define SPI_BLOCK_SIZE                      (0x10000)
#define SPI_SECTOR_SIZE                     (0x1000)
#define SPI_PAGE_PROGRAM_TIME               (3)     // max 3ms per page
#define SPI_ERASE_SECTOR_TIME               (300)   // max 300ms per sector
#define SPI_ERASE_BLOCK_TIME                (2000000)   // max 2sec per block
#define SPI_ERASE_CHIP_TIME                 (4294000)   // max 100sec per CHIP, but time max timeout is 4294 sec

//Commands for Winbond's serial flash
#define FLASH_WINBOND_CMD_WREN          0x06            //FLASH CMD WREN
#define FLASH_WINBOND_CMD_WRDI          0x04            //FLASH CMD WRDI
#define FLASH_WINBOND_CMD_RDSR          0x05            //FLASH CMD RDSR
#define FLASH_WINBOND_CMD_WRSR          0x01            //FLASH CMD WRSR
#define FLASH_WINBOND_CMD_READ          0x03            //FLASH CMD READ
#define FLASH_WINBOND_CMD_FASTREAD      0x0B            //FLASH CMD FAST READ
#define FLASH_WINBOND_CMD_PAGEPROG      0x02            //FLASH CMD PAGE PROGRAM
#define FLASH_WINBOND_CMD_SECTOR_ERASE  0x20            //FLASH CMD SECTOR ERASE
#define FLASH_WINBOND_CMD_BLOCK_ERASE   0xD8            //FLASH CMD BLOCK ERASE
#define FLASH_WINBOND_CMD_CHIP_ERASE    0xC7            //FLASH CMD CHIP ERASE
#define FLASH_WINBOND_CMD_POWER_UP      0xAB            //Release power down
#define FLASH_WINBOND_CMD_POWER_DOWN    0xB9            //Power down
#define FLASH_WINBOND_CMD_RD_ALL_ID     0x90            //Read manufacture and device ID
#define FLASH_WINBOND_CMD_RD_JEDEC_ID   (0x9Ful)        //Read JEDEC ID

#define SPI_FLASH_CMD_AAI               (0xADul)        //(AAI)Single-WORD program
#define SPI_FLASH_CMD_EWSR              (0x50)          //Enable Write Status Register


#define FLASH_WINBOND_ID                0xEF            //Manufacturer ID of Winbond serial flash
#define FLASH_ESMT_ID                   0x8C            //Manufacturer ID of ESMT serial flash
#define FLASH_GD_ID                     0xC8            //Manufacturer ID of GigaDevice serial flash
#define FLASH_MXIC_ID                   0xC2            //Manufacturer ID of MXIC serial flash
#define FLASH_EON_ID                    0x1C            //Manufacturer ID of EON serial flash
//#NT#2012/03/08#Shirley Cheng -begin
#define FLASH_NS_ID                     0xD5            //Manufacturer ID of NS serial flash
//#NT#2012/03/08#Shirley Cheng -end
//#NT#2012/04/16#Shirley Cheng -begin
#define FLASH_PCT_ID                    0xBF            //Manufacturer ID of PCT serial flash
//#NT#2012/04/16#Shirley Cheng -end


#define FLASH_WINBOND_PROCESSING_BIT    0x01

#define FLASH_STATUS_WP_BITS            (0x1C)
#define FLASH_STATUS_WEL_BITS           (0x02)


typedef enum{
    FLASH_STATUS_READY        = 0x00,           //FLASH is ready for access
    FLASH_STATUS_BUSY         = 0x01,           //FLASH is still under processing
    FLASH_STATUS_DONE         = 0x02,           //FLASH processing is done.
    FLASH_STATUS_EXISTING     = 0x03,           //FLASH is existing.
    FLASH_STATUS_NOT_EXIST    = 0xFF,           //FLASH is not existing
    ENUM_DUMMY4WORD(ER_SPI)
} ER_SPI;

/*
    SPI flash identification structure

    @note For nor_open()
*/
typedef struct
{
    UINT32      uiMfgID;            ///< manufacture ID
    UINT32      uiMemType;          ///< memory type
    UINT32      uiMemCapacity;      ///< memory capacity
    UINT32      uiTotalSize;        ///< total size (unit: byte)
    UINT32      uiSectorSize;       ///< sector size (unit: byte)
    UINT32      uiBlockSize;        ///< block size (unit: byte)
    UINT32      uiSectorCnt;        ///< sectr count (unit: sector)
    BOOL        bSupportEWSR;       ///< support EWSR command
    BOOL        bSupportAAI;        ///< support AAI command
    //#NT#2010/08/31#HH Chuang -begin
    //#NT#support W25P16
    BOOL        bSupportSecErase;   ///< support SECTOR_ERASE command (0xD8)
    //#NT#2010/08/31#HH Chuang -end
    UINT32      uiChipEraseTime;    ///< CHIP erase time (unit: ms)
    UINT32      uiBlockEraseTime;   ///< Block erase time (unit: ms)
    UINT32      uiSectorEraseTime;  ///< Sector erase time (unit: ms)
    UINT32      uiPageProgramTime;  ///< page program time (unit: ms)
} SPI_FLASH_INFO, *PSPI_FLASH_INFO;

#endif
