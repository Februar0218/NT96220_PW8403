/*
    @file       SPIFLASH.c
    @ingroup    mISPIFLASH

    @brief      SPI Flash driver

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include <String.h>
#include "spiflash_int.h"
#include "Debug.h"
#include "Strgdef.h"
#include "ErrorNo.h"
#include "spi.h"
#include "nor.h"
#include "cache.h"

static STRG_TAB gSpiFlashObj = {NULL, NULL, nor_open, NULL, NULL, NULL, nor_close, NULL, NULL, NULL, NULL, NULL, NULL,NULL};

//#NT#2010/08/31#HH Chuang -begin
//#NT#support W25P16
static SPI_FLASH_INFO vSpiFlashTable[] = {
    // ESMT F25L16PA (2MB) creator 2012/05/17
    {   FLASH_ESMT_ID,      0x21,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    TRUE,   TRUE,   TRUE,
        30000,              2000,   300,    1
    },
    // ESMT F25L016A (2MB)
    {   FLASH_ESMT_ID,      0x20,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    TRUE,   TRUE,   TRUE,
        30000,              2000,   300,    1
    },
    // ESMT F25L032PA (4MB)
    {   FLASH_ESMT_ID,      0x20,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    TRUE,   FALSE,  TRUE,
        100000,             2000,   300,    5
    },
    // GigaDevice GD25Q16 (2MB)
    {   FLASH_GD_ID,        0x40,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE, TRUE,
        30000,              2000,   300,    5
    },
    // Winbond W25X16 (2MB)
    {   FLASH_WINBOND_ID,   0x30,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE, TRUE,
        40000,              2000,   300,    3
    },
    // Winbond W25X32 (4MB)
    {   FLASH_WINBOND_ID,   0x30,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE, TRUE,
        80000,              2000,   300,    3
    },
    // Winbond W25X64 (8MB)
    {   FLASH_WINBOND_ID,   0x30,   0x17,
        0x800000,           0x1000, 0x1000, 0x800000/0x1000,    FALSE,   FALSE, TRUE,
        100000,             1000,   200,    3
    },
    // Winbond W25Q16 (2MB)
    {   FLASH_WINBOND_ID,   0x40,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE, TRUE,
        40000,              1500,   200,    3
    },
    // Winbond W25Q32 (4MB)
    {   FLASH_WINBOND_ID,   0x40,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE, TRUE,
        80000,              1500,   200,    3
    },
    // Winbond W25P16 (2MB)
    {   FLASH_WINBOND_ID,   0x20,   0x15,
        0x200000,           0x10000,0x10000,0x200000/0x10000,   FALSE,   FALSE, FALSE,
        80000,              1500,   1500,   3
    },
    // MXIC MX25L8006E (1MB)
    {   FLASH_MXIC_ID,      0x20,   0x14,
        0x100000,           0x1000, 0x1000, 0x100000/0x1000,    FALSE,   FALSE, TRUE,
        15000,              2000,   300,    5
    },
    // MXIC MX25L1605D (2MB)
    {   FLASH_MXIC_ID,      0x20,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE, TRUE,
        //#NT#2011/08/03#Shirley Cheng -begin
        //#Page program timeout from 5ms to 10ms
        30000,              2000,   300,    10
        //#NT#2011/08/03#Shirley Cheng -end
    },
    // MXIC MX25L3205D (4MB)
    {   FLASH_MXIC_ID,      0x20,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE, TRUE,
        50000,              2000,   300,    5
    },
    // MXIC MX25L6405D (8MB)
    {   FLASH_MXIC_ID,      0x20,   0x17,
        0x800000,           0x1000, 0x1000, 0x800000/0x1000,    FALSE,   FALSE, TRUE,
        80000,              2000,   300,    5
    },
    // EON EN25F16 (2MB)
    {
        FLASH_EON_ID,       0x31,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE, TRUE,
        35000,              2000,   300,    5
    },
    //#NT#2012/03/08#Shirley Cheng -begin
    // NS N25S16 (2MB)
    {
        FLASH_NS_ID,        0x30,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE, TRUE,
        35000,              2000,   300,    5
    },
    //#NT#2012/03/08#Shirley Cheng -end
    //#NT#2012/04/16#Shirley Cheng -begin
    // PCT PCT25VF016 (2MB)
    {
        FLASH_PCT_ID,       0x25,   0x41,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    TRUE,    TRUE,  TRUE,
        50000,              2500,   2500,   5
    }
    //#NT#2012/04/16#Shirley Cheng -end
};
#if 0
static SPI_FLASH_INFO vSpiFlashTable[] = {
    // ESMT F25L016A (2MB)
    {   FLASH_ESMT_ID,      0x20,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    TRUE,   TRUE,
        30000,              2000,   300,    1
    },
    // ESMT F25L032PA (4MB)
    {   FLASH_ESMT_ID,      0x20,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    TRUE,   FALSE,
        100000,             2000,   300,    5
    },
    // GigaDevice GD25Q16 (2MB)
    {   FLASH_GD_ID,        0x40,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        30000,              2000,   300,    5
    },
    // Winbond W25X16 (2MB)
    {   FLASH_WINBOND_ID,   0x30,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        40000,              2000,   300,    3
    },
    // Winbond W25X32 (4MB)
    {   FLASH_WINBOND_ID,   0x30,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        80000,              2000,   300,    3
    },
    // Winbond W25X64 (8MB)
    {   FLASH_WINBOND_ID,   0x30,   0x17,
        0x800000,           0x1000, 0x1000, 0x800000/0x1000,    FALSE,   FALSE,
        100000,             1000,   200,    3
    },
    // Winbond W25Q16 (2MB)
    {   FLASH_WINBOND_ID,   0x40,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        40000,              1500,   200,    3
    },
    // Winbond W25Q32 (4MB)
    {   FLASH_WINBOND_ID,   0x40,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        80000,              1500,   200,    3
    },
    // MXIC MX25L1605D (2MB)
    {   FLASH_MXIC_ID,      0x20,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        30000,              2000,   300,    5
    },
    // MXIC MX25L3205D (4MB)
    {   FLASH_MXIC_ID,      0x20,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        50000,              2000,   300,    5
    },
    // MXIC MX25L6405D (8MB)
    {   FLASH_MXIC_ID,      0x20,   0x17,
        0x800000,           0x1000, 0x1000, 0x800000/0x1000,    FALSE,   FALSE,
        80000,              2000,   300,    5
    }
};
#endif
//#NT#2010/08/31#HH Chuang -end

static BOOL bIsOpened = FALSE;
static UINT32 uiSpiFlashIdx = 0;
//#NT#2010/06/30#HH Chuang -begin
//#NT#detect invalid nor flash access
static UINT32 uiSpiProtectBaseAddress = 0;
static UINT32 uiSpiProtectLength = 0;
static NOR_INVALID_WRITE_CB pSpiInvalidAccessHdl = NULL;
//#NT#2010/06/30#HH Chuang -end

static ER_SPI nor_waitReady(UINT32 uiWaitMs);
static void nor_readData(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8 *pBuf);
static BOOL nor_pageProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf);
static BOOL nor_aaiProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf);
static void nor_enableWriteLatch(void);
static void nor_disableWriteLatch(void);
static ER   nor_unlock(void);
static ER   nor_lock(void);
//#NT#2010/08/31#HH Chuang -begin
//#NT#support W25P16
static void nor_readID(UINT32 *puiMfgID, UINT32 *puiDevID, UINT32 *puiTypeID);
//static void nor_readID(UINT32 *puiMfgID, UINT32 *puiDevID);
//#NT#2010/08/31#HH Chuang -end

/**
    @addtogroup mISPIFLASH
*/
//@{

/**
    @name   SPI Flash Driver API
*/
//@{


/**
    Open NOR flash driver

    @return
        - @b E_OK: open success
        - @b Else: open fail
*/
ER nor_open(void)
{
//    UINT32 uiMfgID, uiDevID;
    NORSTRG norStrg;
    ER erReturn;

    if ((erReturn = nor_lock()) != E_OK)
    {
        return erReturn;
    }

    if (spi_open() != E_OK)
    {
        return E_SYS;
    }

    bIsOpened = TRUE;

    if (nor_getDeviceInfo(&norStrg) == FALSE)
    {
        //#NT#2011/07/11#HH Chuang -begin
        //#NT#fix the hung-up if board has no SPI flash
        spi_close();
        //#NT#2011/07/11#HH Chuang -end
        bIsOpened = FALSE;
        return E_SYS;
    }

    return E_OK;
}

/**
    Close NOR flash driver

    @return
        - @b E_OK: close success
        - @b Else: close fail
*/
ER nor_close(void)
{
    ER erReturn;

    if(nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime) != FLASH_STATUS_READY)
    {
        debug_err(("SPI flash: close: wait ready timeout\r\n"));
    }

    bIsOpened = FALSE;

    if ((erReturn = spi_close()) != E_OK)
    {
        return erReturn;
    }
    nor_unlock();

    return E_OK;
}

/**
    Lock SPI module

    @return
        - @b E_OK: lock success
        - @b Else: lock fail
*/
ER nor_lock(void)
{
//    return = wai_sem(SEMID_SPI);
    return E_OK;
}

/**
    Unlock SPI module

    @return
        - @b E_OK: lock success
        - @b Else: lock fail
*/
ER nor_unlock(void)
{
 //   return = sig_sem(SEMID_SPI);
    return E_OK;
}

/**
    Get Storage object

    Get storage object of SPI flash driver.

    @return pointer to storage object
*/
PSTRG_TAB nor_getStorageObject(void)
{
    return &gSpiFlashObj;
}

/**
    Get device information

    @param[out] pNorInfo    pointer point to PNORSTRG

    @return
        - @b TRUE: get information success
        - @b FALSE: get information fail, maybe flash ID not match
*/
BOOL nor_getDeviceInfo(PNORSTRG pNorInfo)
{
    UINT32  i;
    //#NT#2010/08/31#HH Chuang -begin
    //#NT#support W25P16
    UINT32  uiMfgID, uiDevID, uiTypeID;
//    UINT32  uiMfgID, uiDevID;
    //#NT#2010/08/31#HH Chuang -end

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: get devInfo: driver not opened\r\n"));
        return FALSE;
    }

    //#NT#2010/08/31#HH Chuang -begin
    //#NT#support W25P16
    nor_readID(&uiMfgID, &uiDevID, &uiTypeID);
//    nor_readID(&uiMfgID, &uiDevID);
    //#NT#2010/08/31#HH Chuang -end

    for (i=0; i<sizeof(vSpiFlashTable)/sizeof(vSpiFlashTable[0]); i++)
    {
        //#NT#2010/08/31#HH Chuang -begin
        //#NT#support W25P16
        if ((vSpiFlashTable[i].uiMfgID == uiMfgID) &&
            (vSpiFlashTable[i].uiMemCapacity == uiDevID) &&
            (vSpiFlashTable[i].uiMemType == uiTypeID))
//        if ((vSpiFlashTable[i].uiMfgID == uiMfgID) &&
//            (vSpiFlashTable[i].uiMemCapacity == uiDevID))
        //#NT#2010/08/31#HH Chuang -end
        {
            pNorInfo->ulSectorSize = vSpiFlashTable[i].uiSectorSize;
            pNorInfo->ulNumberOfSector = vSpiFlashTable[i].uiSectorCnt;
            pNorInfo->ulTotalSize  = vSpiFlashTable[i].uiTotalSize;
            pNorInfo->ulBlockSize = vSpiFlashTable[i].uiBlockSize;

            pNorInfo->cBootDirection = NOR_BOOT_BOTTOM;
            pNorInfo->ucDeviceID = uiDevID;
            pNorInfo->ucSiliconID = uiMfgID;

            uiSpiFlashIdx = i;

            return TRUE;
        }
    }

    debug_err(("SPI flash: get devInfo: Manufacture ID dosen't support. MFG ID:0x%lx, Device ID:0x%lx\r\n", uiMfgID, uiDevID));
    return FALSE;
}

/**
    Erase whole chip

    Erase whole content of nor flash to 0xFF.

    @return
        - @b TRUE: erase success
        - @b FALSE: erase fail
*/
BOOL nor_eraseChip(void)
{
    ER_SPI erReturn;

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: eraseChip: driver not opened\r\n"));
        return FALSE;
    }

    //#NT#2010/06/30#HH Chuang -begin
    //#NT#detect invalid nor flash access
    if (uiSpiProtectLength != 0)
    {
        debug_err(("SPI flash: eraseChip: detect invalid access\r\n"));
        debug_err(("SPI flash: protect base: 0x%lx, protect length: 0x%lx\r\n", uiSpiProtectBaseAddress, uiSpiProtectLength));
        if (pSpiInvalidAccessHdl != NULL)
        {
            pSpiInvalidAccessHdl(NOR_OPERATION_ERASE, 0, vSpiFlashTable[uiSpiFlashIdx].uiTotalSize);
        }
        return FALSE;
    }
    //#NT#2010/06/30#HH Chuang -end

    if(nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime) != FLASH_STATUS_READY)
    {
        debug_err(("SPI flash: eraseChip: wait flash ready timeout\r\n"));
        return FALSE;
    }

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(FLASH_WINBOND_CMD_CHIP_ERASE);
    spi_setCSActive(FALSE);

    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime);
    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        debug_err(("SPI flash: eraseChip: timeout after erase\r\n"));
        return FALSE;
    }
}

/**
    Erase a sector

    Erase a sector of nor flash.
    In Winbond W25X16/32/64 series, size of one sector is 0x1000 bytes.

    @param[in] uiByteAddr   Byte address of erased sector. Should be 0x1000 alignment.

    @return
        - @b TRUE: erase success
        - @b FALSE: erase fail
*/
BOOL nor_eraseSector(UINT32 uiByteAddr)
{
    ER_SPI erReturn;

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: eraseSector: driver not opened\r\n"));
        return FALSE;
    }

    //Check paprameter
    if((uiByteAddr % vSpiFlashTable[uiSpiFlashIdx].uiSectorSize) != 0)
    {
        debug_err(("SPI flash: eraseSector: byte address boundary error 0x%lx\r\n", uiByteAddr));
        return FALSE;
    }
    //#NT#2010/06/30#HH Chuang -begin
    //#NT#detect invalid nor flash access
    if (uiSpiProtectLength != 0)
    {
        if ( ((uiSpiProtectBaseAddress <= uiByteAddr) &&
                (uiByteAddr < (uiSpiProtectBaseAddress+uiSpiProtectLength))) ||
            ((uiSpiProtectBaseAddress < (uiByteAddr+vSpiFlashTable[uiSpiFlashIdx].uiSectorSize))
                && ((uiByteAddr+vSpiFlashTable[uiSpiFlashIdx].uiSectorSize) <= (uiSpiProtectBaseAddress+uiSpiProtectLength))) )
        {
            debug_err(("SPI flash: eraseSector: detect invalid access: 0x%lx\r\n", uiByteAddr));
            debug_err(("SPI flash: protect base: 0x%lx, protect length: 0x%lx\r\n", uiSpiProtectBaseAddress, uiSpiProtectLength));
            if (pSpiInvalidAccessHdl != NULL)
            {
                pSpiInvalidAccessHdl(NOR_OPERATION_ERASE, uiByteAddr, vSpiFlashTable[uiSpiFlashIdx].uiSectorSize);
            }
            return FALSE;
        }
    }
    //#NT#2010/06/30#HH Chuang -end
    if(nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime) != FLASH_STATUS_READY)
    {
        debug_err(("SPI flash: eraseSector: wait ready timeout\r\n"));
        return FALSE;
    }

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
    //#NT#2010/08/31#HH Chuang -begin
    //#NT#support W25P16
    if (vSpiFlashTable[uiSpiFlashIdx].bSupportSecErase == TRUE)
    {
        spi_writeSingle((FLASH_WINBOND_CMD_SECTOR_ERASE<<8) | ((uiByteAddr>>16)&0xFF));
    }
    else
    {
        spi_writeSingle((FLASH_WINBOND_CMD_BLOCK_ERASE<<8) | ((uiByteAddr>>16)&0xFF));
    }
//    spi_writeSingle((FLASH_WINBOND_CMD_SECTOR_ERASE<<8) | ((uiByteAddr>>16)&0xFF));
    //#NT#2010/08/31#HH Chuang -end
    spi_writeSingle(uiByteAddr & 0xFFFF);
//    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
//    spi_writeSingle((FLASH_WINBOND_CMD_SECTOR_ERASE<<24) | (uiByteAddr&0xFFFFFF));
    spi_setCSActive(FALSE);

    //#NT#2010/08/31#HH Chuang -begin
    //#NT#support W25P16
    if (vSpiFlashTable[uiSpiFlashIdx].bSupportSecErase == TRUE)
    {
    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiSectorEraseTime);
    }
    else
    {
        erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiBlockEraseTime);
    }
//    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiSectorEraseTime);
    //#NT#2010/08/31#HH Chuang -end
    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        debug_err(("SPI flash: eraseSector: erase timeout\r\n"));
        return FALSE;
    }
}

/**
    Program a sector

    Program a sector of nor flash.
    In Winbond W25X16/32/64 series, size of one sector is 0x1000 bytes.

    @param[in] uiByteAddr   Byte address of programed sector. Should be 0x1000 alignment.
    @param[in] uiSectorSize Byte size of one secotr. Should be 0x1000.
    @param[in] pBuf         pointer point to written data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail
*/
BOOL nor_programSector(UINT32 uiByteAddr, UINT32 uiSectorSize, UINT8* pBuf)
{
    ER_SPI erReturn;
    UINT32 uiProgramCycle, uiCycleIndex;
    UINT32 uiAddrIndex;

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: program: driver not opened\r\n"));
        return FALSE;
    }

    //#NT#2010/06/30#HH Chuang -begin
    //#NT#detect invalid nor flash access
    if (uiSpiProtectLength != 0)
    {
        if ( ((uiSpiProtectBaseAddress <= uiByteAddr) &&
                (uiByteAddr < (uiSpiProtectBaseAddress+uiSpiProtectLength))) ||
            ((uiSpiProtectBaseAddress < (uiByteAddr+uiSectorSize))
                && ((uiByteAddr+uiSectorSize) <= (uiSpiProtectBaseAddress+uiSpiProtectLength))) )
        {
            debug_err(("SPI flash: program: detect invalid access, base: 0x%lx, length: 0x%lx\r\n", uiByteAddr, uiSectorSize));
            debug_err(("SPI flash: protect base: 0x%lx, protect length: 0x%lx\r\n", uiSpiProtectBaseAddress, uiSpiProtectLength));
            if (pSpiInvalidAccessHdl != NULL)
            {
                pSpiInvalidAccessHdl(NOR_OPERATION_WRITE, uiByteAddr, uiSectorSize);
            }
            return FALSE;
        }
    }
    //#NT#2010/06/30#HH Chuang -end

    if (vSpiFlashTable[uiSpiFlashIdx].bSupportAAI == TRUE)
    {
        return nor_aaiProgram(uiByteAddr, uiSectorSize, pBuf);
    }
    else
    {
        uiProgramCycle = uiSectorSize / SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE;

        uiAddrIndex = uiByteAddr;
        for(uiCycleIndex = 0; uiCycleIndex < uiProgramCycle; uiCycleIndex++, uiAddrIndex += SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)
        {
            erReturn = nor_pageProgram(uiAddrIndex, SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE, (pBuf + (uiCycleIndex * SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)));
            if (!erReturn)
            {
                debug_err(("SPI flash: program: fail at flash address 0x%lx\r\n", uiAddrIndex));
                return FALSE;
            }
        }
    }

    return TRUE;
}

/**
    Read NOR flash data

    Read data from NOR flash.

    @param[in] uiByteAddr   Byte address of flash.
    @param[in] uiByteSize   Byte size of read data.
    @param[out] pBuf        Pointer point to store read data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail
*/
BOOL nor_readByBytes(UINT32 uiByteAddr, UINT32 uiByteSize, UINT8 *pBuf)
{
    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: read: driver not opened\r\n"));
        return FALSE;
    }

    nor_readData(uiByteAddr, uiByteSize, pBuf);

    return TRUE;
}

//#NT#2010/06/30#HH Chuang -begin
//#NT#detect invalid nor flash access
/**
    Protect NOR flash data

    Protect partial of NOR flash from erased/programed.

    @param[in] uiByteAddr       Byte address of flash.
    @param[in] uiByteSize       Byte size of read data.
    @param[in] pInvalidWriteHdl call back invoked when invalid access is detected

    @return void
*/
void nor_setProtectRange(UINT32 uiByteAddr, UINT32 uiByteSize, NOR_INVALID_WRITE_CB pInvalidWriteHdl)
{
    uiSpiProtectBaseAddress = uiByteAddr;
    uiSpiProtectLength = uiByteSize;
    pSpiInvalidAccessHdl = pInvalidWriteHdl;
}
//#NT#2010/06/30#HH Chuang -end

//#NT#2011/12/16#Shirley Cheng -begin
/**
    Execute the control operation of nor flash

    Execute the control operation of nor flash

    @param[in] NORCTRL_OP       operation of nor flash.
    @param[in] PNORCTRL_PARAM   parameter of specific operation

    @return
        - @b E_OK: operation success
        - @b Else: operation fail

*/
ER nor_ctrlOp(NORCTRL_OP nor_op, PNORCTRL_PARAM pnor_param)
{
    ER     eReturn;
    UINT32 uiData;
    UINT32 i;
    UINT32 uiByteCnt;
    
    eReturn = E_SYS;
    
    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: read: driver not opened\r\n"));
        return  E_SYS;
    }    
    switch(nor_op)
    {
        case NORCTRL_WRITEREAD_CMD:
            uiByteCnt = pnor_param->SEL.WRITEREAD_CMD.uiCnt;
            
            spi_setCSActive(TRUE);
            spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);

            for (i = 0; i < uiByteCnt; i++)
            {               
                    uiData = pnor_param->SEL.WRITEREAD_CMD.uiByte[i] & 0xFF;
                    
                    if (pnor_param->SEL.WRITEREAD_CMD.uiByte[i] & NOR_WRITEREAD_CMDREAD)
                    {
                        spi_readSingle(&uiData);
                        pnor_param->SEL.WRITEREAD_CMD.uiByte[i] = uiData | NOR_WRITEREAD_CMDREAD;
                    }
                    else
                    {
                        spi_writeSingle(uiData);
                    }
            }
            
            if (pnor_param->SEL.WRITEREAD_CMD.bCsDeSel == TRUE)
            {
                spi_setCSActive(FALSE);
            }
            eReturn = E_OK;
            break;
            
        default:
            debug_err(("SPI flash: read: driver not opened\r\n"));
            eReturn = E_PAR;
            break;

    }
    return eReturn;
}
//#NT#2011/12/16#Shirley Cheng -end

//@}

/*
    @name   SPI Flash Driver Internal API
*/
//@{

/*
    Read SPI flash ID

    Read manufacture ID and device ID

    @param[out] puiMfgID    Returned manufacture ID
    @param[out] puiDevID    Returned device ID
    @param[out] puiTypeID   Retruned mem type ID

    @return void
*/
//#NT#2010/08/31#HH Chuang -begin
//#NT#support W25P16
static void nor_readID(UINT32 *puiMfgID, UINT32 *puiDevID, UINT32 *puiTypeID)
//static void nor_readID(UINT32 *puiMfgID, UINT32 *puiDevID)
//#NT#2010/08/31#HH Chuang -end
{
    UINT32 uiId1, uiId2;

    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
//    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
    spi_writeReadSingle(FLASH_WINBOND_CMD_RD_JEDEC_ID<<8, &uiId1);
    spi_writeReadSingle(0, &uiId2);
//    spi_writeReadSingle(FLASH_WINBOND_CMD_RD_JEDEC_ID<<24, &uiId);

    spi_setCSActive(FALSE);

    *puiMfgID = uiId1 & 0xFF;       // Manufacture ID
    *puiDevID = uiId2 & 0xFF;       // Device ID
    //#NT#2010/08/31#HH Chuang -begin
    //#NT#support W25P16
    *puiTypeID = uiId2 >> 8;
    //#NT#2010/08/31#HH Chuang -end
}

/*
    Wait SPI flash ready

    Wait SPI flash returned to ready state

    @param[in] uiWaitMs     Timeout setting. (Unit: ms)

    @return
        - @b FLASH_STATUS_READY: success
        - @b FLASH_STATUS_BUSY: fail. Maybe timeout.
*/
static ER_SPI nor_waitReady(UINT32 uiWaitMs)
{
    ER erReturn;

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_enBitMatch(FLASH_WINBOND_CMD_RDSR, 0, FALSE);
    erReturn = spi_waitBitMatch(uiWaitMs);
    spi_setCSActive(FALSE);

    if (erReturn == E_OK)
    {
        return FLASH_STATUS_READY;
    }
    else
    {
        return FLASH_STATUS_BUSY;
    }
}

/*
    Read SPI flash

    @param[in] uiSPIADDR        Byte address of SPI flash
    @param[in] uiByteSize       Byte count of read data. Should be word alignment (i.e. multiple of 4).
    @param[out] pBuf            Pointer point to store read data

    @return void
*/
static void nor_readData(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8 *pBuf)
{
    UINT32 i, uiWord;
    UINT32 uiCount;
    UINT32 uiBufOffset = 0;

    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
    spi_writeSingle((FLASH_WINBOND_CMD_READ<<8) | ((uiSPIADDR>>16)&0xFF));
    spi_writeSingle(uiSPIADDR & 0xFFFF);
//    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
//    spi_writeSingle((FLASH_WINBOND_CMD_READ<<24) | (uiSPIADDR&0xFFFFFF));
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    while (uiByteSize != 0)
    {
        if ((uiByteSize/4) > ((SPI_MAX_TRANSFER_CNT>>2)<<2))
        {
            uiCount = (SPI_MAX_TRANSFER_CNT>>2) << 2;

            spi_writeReadData(uiCount, NULL, (UINT32*)(&pBuf[uiBufOffset]), TRUE);
            spi_waitDataDone();

            uiByteSize -= uiCount * 4;
            uiBufOffset += uiCount * 4;
        }
        else
        {
            uiCount = (uiByteSize / 16) * 4;

            if (uiCount != 0)
            {
                spi_writeReadData(uiCount, NULL, (UINT32*)(&pBuf[uiBufOffset]), TRUE);
                spi_waitDataDone();
            }
            if (uiByteSize % 16)
            {
                for (i=0; i<(uiByteSize % 16); i++)
                {
                    spi_readSingle(&uiWord);
                    pBuf[uiBufOffset+uiCount*4+i] = uiWord & 0xFF;
                }
            }

            uiByteSize = 0;
        }
#if 0
        spi_writeReadData(uiCount, NULL, (UINT32*)(&pBuf[uiBufOffset]), TRUE);
        spi_waitDataDone();

        uiByteSize -= uiCount * 4;
        uiBufOffset += uiCount * 4;
#endif
    }

    spi_setCSActive(FALSE);
}

/*
    Page program

    Program a page of SPI flash

    @param[in] uiSPIADDR        Byte address of SPI flash
    @param[in] uiByteSize       Byte count of written data. Should be <= 256 and word alignment (i.e. multiple of 4).
    @param[out] pBuf            Pointer point to store read data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail. Maybe timeout.
*/
static BOOL nor_pageProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf)
{
    ER_SPI erReturn;

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
    spi_writeSingle((FLASH_WINBOND_CMD_PAGEPROG<<8) | ((uiSPIADDR>>16)&0xFF));
    spi_writeSingle(uiSPIADDR & 0xFFFF);
//    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
//    spi_writeSingle((FLASH_WINBOND_CMD_PAGEPROG<<24) | (uiSPIADDR&0xFFFFFF));

    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeReadData(uiByteSize/4, (UINT32*)pBuf, NULL, TRUE);
    spi_waitDataDone();
    spi_setCSActive(FALSE);

    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiPageProgramTime);
    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
    AAI single word program

    Program single word of SPI flash

    @param[in] uiSPIADDR        Byte address of SPI flash
    @param[in] uiByteSize       Byte count of written data. Should be <= 256 and word alignment (i.e. multiple of 4).
    @param[out] pBuf            Pointer point to store read data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail. Maybe timeout.
*/
static BOOL nor_aaiProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf)
{
    UINT32 i, uiWord;
    ER_SPI erReturn;

    if (uiByteSize == 0)
    {
        return FALSE;
    }

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
    spi_writeSingle((SPI_FLASH_CMD_AAI<<8) | ((uiSPIADDR>>16)&0xFF));
    spi_writeSingle(uiSPIADDR & 0xFFFF);
//    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
//    spi_writeSingle((SPI_FLASH_CMD_AAI<<24) | (uiSPIADDR&0xFFFFFF));

//    spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
    uiWord = (pBuf[0] << 8) | pBuf[1];
    spi_writeSingle(uiWord);
    spi_setCSActive(FALSE);

    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiPageProgramTime);

    for (i=2; i<uiByteSize; i+=2)
    {
        spi_setCSActive(TRUE);

        spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
        spi_writeSingle(SPI_FLASH_CMD_AAI);
        spi_writeSingle(pBuf[i]);
        spi_writeSingle(pBuf[i+1]);
//        spi_setTransferLen(SPI_TRANSFER_LEN_3BYTES);
//        uiWord = (SPI_FLASH_CMD_AAI << 16) | (pBuf[i] << 8) | pBuf[i+1];
//        spi_writeSingle(uiWord);

        spi_setCSActive(FALSE);

        erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiPageProgramTime);
    }

    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void nor_enableWriteLatch(void)
{
    UINT32 uiWord;

    if (vSpiFlashTable[uiSpiFlashIdx].bSupportEWSR == TRUE)
    {
        spi_setCSActive(TRUE);
        spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
        spi_writeReadSingle((FLASH_WINBOND_CMD_RDSR<<8) | (0xFF), &uiWord);
        spi_setCSActive(FALSE);

        uiWord &= ~(FLASH_STATUS_WP_BITS | FLASH_STATUS_WEL_BITS);

        spi_setCSActive(TRUE);

        spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
        spi_writeSingle(SPI_FLASH_CMD_EWSR);

        spi_setCSActive(FALSE);

        spi_setCSActive(TRUE);
        spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
        spi_writeSingle((FLASH_WINBOND_CMD_WRSR<<8) | (uiWord&0xFF));
        spi_setCSActive(FALSE);
    }

    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(FLASH_WINBOND_CMD_WREN);

    spi_setCSActive(FALSE);
}

static void nor_disableWriteLatch(void)
{
    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(FLASH_WINBOND_CMD_WRDI);

    spi_setCSActive(FALSE);
}

//@}

//@}

