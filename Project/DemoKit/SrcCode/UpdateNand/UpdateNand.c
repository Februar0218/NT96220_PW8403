/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UpdateNand.c
    @ingroup    mIPRJAPInit

    @brief      Update data in NAND flash reserved area
                Update FW in NAND flash reserved area.
                Only support read FW from SD or NAND now.

    @note       Nothing.

    @date       2006/01/26
*/

/** \addtogroup mIPRJAPInit*/
//@{

#include <string.h>
#include "Kernel.h"
#include "SysCfg.h"
#include "UpdateNand.h"
#include "UpdateNandProgress.h"
#include "UpdateNandReset.h"
#include "CopyRest.h"
#include "Nand.h"
#include "FileSysTsk.h"

// FW checking
#define FW_CHECK_NOCHECK            0
#define FW_CHECK_CHECKSUM           1
#define FW_CHECK_CRC                2
#define FW_CHECK_METHOD             FW_CHECK_CHECKSUM

#define FW_CHECK_PSEUDO_STR         "DSC_STR"

extern UINT32   Load$$LOAD_RESET$$Base;
extern UINT32   Image$$LOAD_RESET$$Base;
extern UINT32   Image$$LOAD_RESET$$Length;
extern UINT32   Image$$LOAD_RESET$$ZI$$Base;
extern UINT32   Image$$LOAD_RESET$$ZI$$Limit;

static UINT8    uiUpdateFWName[]    = "A:\\FW96612A.BIN";
static UINT8    uiLoadFWName[]      = "A:\\FW96612T.BIN";

#if (FW_CHECK_METHOD == FW_CHECK_CRC)
static UINT32   uiCRCTable[256];

/**
  Make CRC table

  Generate CRC table

  @param void
  @return void
*/
static void UpdateNand_MakeCRCTable(void)
{
    UINT32 uiValue, i, j;

    for (i=0; i<256; i++)
    {
        uiValue = i;
        for (j=0; j<8; j++)
        {
            uiValue = uiValue & 1 ? 0xEDB88320 ^ (uiValue >> 1) : (uiValue >> 1);
        }
        uiCRCTable[i] = uiValue;
    }
}
#endif

#if (FW_CHECK_METHOD != FW_CHECK_NOCHECK)
/**
  FW code checking

  FW code checking.

  @param UINT32 uiAddress: FW code in DRAM starting address
  @param UINT32 uiFileLen: FW code length
  @return BOOL: TRUE -> check OK, FALSE -> check failed
*/
static BOOL UpdateNand_CheckFW(UINT32 uiAddress, UINT32 uiFileLen)
{
    #if (FW_CHECK_METHOD == FW_CHECK_CHECKSUM)
    UINT32 i, uiSum, uiStrLen, uiTempLen;
    UINT16 *puiValue;

    uiSum = 0;
    puiValue = (UINT16 *)uiAddress;

    // Padding pseudo string
    uiStrLen = strlen(FW_CHECK_PSEUDO_STR);
    if (uiStrLen > 0)
    {
        uiTempLen = (uiStrLen + 3) & ~3;
        memset((void *)(uiAddress + uiFileLen), 0, uiTempLen);
        memcpy((void *)(uiAddress + uiFileLen), (const void *)FW_CHECK_PSEUDO_STR, uiStrLen);
        uiFileLen += uiTempLen;
    }

    for (i=0; i<(uiFileLen >> 1); i++)
    {
        uiSum += (*(puiValue + i) + i);
    }

    if ((uiSum & 0x0000FFFF) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

    #elif (FW_CHECK_METHOD == FW_CHECK_CRC)
    UINT32 uiCRC, uiValue, uiStrLen, uiTempLen;
    UINT8 *puiTemp;

    // Padding pseudo string
    uiStrLen = strlen(FW_CHECK_PSEUDO_STR);
    if (uiStrLen > 0)
    {
        uiTempLen = (uiStrLen + 3) & ~3;
        memset((void *)(uiAddress + uiFileLen), 0, uiTempLen);
        memcpy((void *)(uiAddress + uiFileLen), (const void *)FW_CHECK_PSEUDO_STR, uiStrLen);
        uiFileLen += uiTempLen;
    }

    // Check binary TAG
    if (*(UINT32 *)(uiAddress + FW_BINARY_TAG_OFFSET) != FW_BINARY_TAG)
    {
        return FALSE;
    }

    // Check length
    if ((*(UINT32 *)(uiAddress + FW_LENGTH_OFFSET) ^ *(UINT32 *)(uiAddress + FW_LENGTH_CHECK_OFFSET)) != FW_BINARY_TAG)
    {
        return FALSE;
    }

    // Read CRC and reset the CRC value
    uiCRC = *(UINT32 *)(uiAddress + FW_CHECK_OFFSET);
    *(UINT32 *)(uiAddress + FW_CHECK_OFFSET) = 0;

    // Check CRC
    uiValue = 0xFFFFFFFF;
    puiTemp = (UINT8 *)uiAddress;
    while (uiFileLen--)
    {
        uiValue = uiCRCTable[(uiValue ^ (*puiTemp++)) & 0xFF] ^ (uiValue >> 8);
    }

    if (uiCRC != (uiValue ^ 0xFFFFFFFF))
    {
        return FALSE;
    }

    // Restore CRC value
    *(UINT32 *)(uiAddress + FW_CHECK_OFFSET) = uiCRC;
    return TRUE;
    #endif
}
#endif

/**
  Update FW

  Read FW code from storage and write to NAND flash

  @param UINT32 uiStorage: Read FW from what storage, refer to PrimaryTsk.h
    PRIMARY_STGTYPE_NAND    (NAND)
    PRIMARY_STGTYPE_CARD    (Storage Card)
  @return UINT32 Status: please refer to UpdateNand.h
*/
UINT32 UpdateNand_UpdateFW(UINT32 uiStorage)
{
//#NT#2011/01/03#Chris Chung -begin
//#NT#mark un-used file system API
#if 0
    UINT32 i;
    UINT32 uiAddress, uiFileLen;
    UINT32 uiBlockSize, uiBlockNum;

    // Part 1: Read FW code from storage

    // Does storage correct?
    if (Primary_GetStorage() != uiStorage)
    {
        debug_err(("Incorrect storage\r\n"));
        return UPDNAND_STS_FW_INVALID_STG;
    }
    // Show progress
    UpdateNand_ProgressFW(UPDNAND_PGS_FW_READING, 0, 0);

    // Get memory for buffer of reading FW code
    // Please make sure that there is no other task using the memory!
    get_blk((VP *)&uiAddress, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiAddress);

    // Wait for previous command finished
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    // Open FW code and read to the memory
    uiFileLen = FW_CODE_SIZE;
    if (FilesysReadWriteByName2(FST_CMD_READ_BY_NAME,
                                (char *)uiUpdateFWName,
                                (UB *)uiAddress,
                                &uiFileLen,
                                0,
                                FST_TIME_INFINITE) != FST_STA_OK)
    {
        debug_err(("Error opening %s\r\n", uiUpdateFWName));
        return UPDNAND_STS_FW_READ_ERR;
    }
    // Show progress
    UpdateNand_ProgressFW(UPDNAND_PGS_FW_READ_OK, 0, 0);

#if (FW_CHECK_METHOD != FW_CHECK_NOCHECK)
    #if (FW_CHECK_METHOD == FW_CHECK_CRC)
    // Make CRC table
    UpdateNand_MakeCRCTable();
    #endif

    // Check FW
    if (UpdateNand_CheckFW(uiAddress, uiFileLen) != TRUE)
    {
        debug_err(("FW check failed\r\n"));
        return UPDNAND_STS_FW_READ_CHK_ERR;
    }
#endif

    // Show progress
    UpdateNand_ProgressFW(UPDNAND_PGS_FW_READ_CHK_OK, 0, 0);

    // Part 2: Write FW code to NAND flash

    uiBlockSize = FilesysNANDGetBlockSize();
    uiBlockNum  = uiFileLen / uiBlockSize;
    if (uiFileLen % uiBlockSize != 0)
    {
        uiBlockNum++;
    }

    // Write FW code to NAND flash
    for (i=0; i<uiBlockNum; i++)
    {
        // Wait for file system finished
        FilesysWaitCmdFinish(FST_TIME_INFINITE);

        if (FilesysNANDWriteReservedBlock((INT8 *)(uiAddress + (i * uiBlockSize)),
                                          FW_START_NAND_BLOCK + i, 1) != E_OK)
        {
            debug_err(("Write FW code to NAND flash fail\r\n"));
            return UPDNAND_STS_FW_WRITE_ERR;
        }
        else
        {
            UpdateNand_ProgressFW(UPDNAND_PGS_FW_WRITING, i + 1, uiBlockNum);
        }
    }
    // Show progress
    UpdateNand_ProgressFW(UPDNAND_PGS_FW_WRITE_OK, 0, 0);

#if (FW_CHECK_METHOD != FW_CHECK_NOCHECK)
    // Wait for file system finished
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    // Read FW code from NAND and check
    if (FilesysNANDReadReservedBlock((INT8 *)uiAddress, FW_START_NAND_BLOCK, uiBlockNum) != E_OK)
    {
        debug_err(("Read FW code from NAND fail (for write checking)\r\n"));
        return UPDNAND_STS_FW_READ2_ERR;
    }

    // Check FW
    if (UpdateNand_CheckFW(uiAddress, uiFileLen) != TRUE)
    {
        debug_err(("FW check failed\r\n"));
        return UPDNAND_STS_FW_WRITE_CHK_ERR;
    }
#endif

    // Show progress
    UpdateNand_ProgressFW(UPDNAND_PGS_FW_WRITE_CHK_OK, 0, 0);

    return UPDNAND_STS_FW_OK;
#else
    return UPDNAND_STS_RPARAM_ERR;
#endif
//#NT#2011/01/03#Chris Chung -end
}

/**
  Load FW

  Read FW code from storage and run

  @param UINT32 uiStorage: Read FW from what storage, refer to PrimaryTsk.h
    PRIMARY_STGTYPE_NAND    (NAND)
    PRIMARY_STGTYPE_CARD    (Storage Card)
  @return void
*/
void UpdateNand_LoadFW(UINT32 uiStorage)
{
    UINT32 uiAddress, uiFileLen;

    // Part 1: Read FW code from storage

    // Does storage correct?
    if (Primary_GetStorage() != uiStorage)
    {
        debug_err(("Incorrect storage\r\n"));
        return;
    }

    // Get memory for buffer of reading FW code
    // Please make sure that there is no other task using the memory!
    get_blk((VP *)&uiAddress, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiAddress);

    // Wait for previous command finished
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    // Open FW code and read to the memory
    uiFileLen = FW_CODE_SIZE;
    if (FilesysReadWriteByName2(FST_CMD_READ_BY_NAME,
                                (char *)uiLoadFWName,
                                (UB *)uiAddress,
                                &uiFileLen,
                                0,
                                FST_TIME_INFINITE) != FST_STA_OK)
    {
        debug_err(("Error opening %s\r\n", uiLoadFWName));
        return;
    }

#if (FW_CHECK_METHOD != FW_CHECK_NOCHECK)
    #if (FW_CHECK_METHOD == FW_CHECK_CRC)
    // Make CRC table
    UpdateNand_MakeCRCTable();
    #endif

    // Check FW
    if (UpdateNand_CheckFW(uiAddress, uiFileLen) != TRUE)
    {
        debug_err(("FW check failed\r\n"));
        return;
    }
#endif

    // Copy code and RW data
    // Image is execution section, and Load is binary file(load) section
    memcpy((void *)(&Image$$LOAD_RESET$$Base), (const void *)(&Load$$LOAD_RESET$$Base), (UINT32)&Image$$LOAD_RESET$$Length);

    // Init ZI section
    // Image is execution section, and Load is binary file(load) section
    if (&Image$$LOAD_RESET$$ZI$$Limit > &Image$$LOAD_RESET$$ZI$$Base)
    {
        memset((void *)(&Image$$LOAD_RESET$$ZI$$Base), 0, &Image$$LOAD_RESET$$ZI$$Limit - &Image$$LOAD_RESET$$ZI$$Base);
    }

    UpdateNand_Reset(uiAddress, uiFileLen);
}

//Add APIs to update system parameter in NAND, for writing serial number via USB
/**
  Write system parameter block

  Write system parameter block.

  @param UINT32 uiBlockID: Which NAND block to write
  @param UINT8 puiBuf: Buffer stored data to write
  @param UINT32 uiLength: Buffer length
  @return UINT32
*/
UINT32 UpdateNand_WriteParam(UINT32 uiBlockID, UINT8 *puiBuf, UINT32 uiLength)
{
//#NT#2011/01/03#Chris Chung -begin
//#NT#mark un-used file system API
#if 0
    // Wait for file system finish it's job to make sure NAND writing is OK
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    // Write data to NAND
    if (FilesysNANDWriteSysParamBlockByBytes((INT8 *)puiBuf, (UINT16)uiBlockID, (UINT16)uiLength) != FST_STA_OK)
    {
        return UPDNAND_STS_WPARAM_ERR;
    }
    else
    {
        return UPDNAND_STS_WPARAM_OK;
    }
#else
    return UPDNAND_STS_RPARAM_ERR;
#endif
//#NT#2011/01/03#Chris Chung -end
}

/**
  Read system parameter block from NAND

  Read system parameter block from NAND.

  @param UINT32 uiBlockID: Which NAND block to read data
  @param UINT32 uiOffset: Offset to the block starting address
  @param UINT32 uiLength: data length
  @param UINT8 puiBuf: Buffer to store data
  @return UINT32
*/
UINT32 UpdateNand_ReadParam(UINT32 uiBlockID, UINT32 uiOffset, UINT32 *uiLength, UINT8 *puiBuf)
{
//#NT#2011/01/03#Chris Chung -begin
//#NT#mark un-used file system API
#if 0
    UINT32 uiAddress, uiNANDReadLength;

    // Get memory for buffer of reading FW code
    get_blk((VP *)&uiAddress, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiAddress);

    uiNANDReadLength = *uiLength + uiOffset;

    // Wait for file system finish it's job to make sure NAND reading is OK
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    // Read data from NAND
    if (FilesysNANDReadSysParamBlockByBytes((INT8 *)uiAddress, (UINT16)uiBlockID, (UINT16)uiNANDReadLength) != FST_STA_OK)
    {
        *uiLength = 0;
        return UPDNAND_STS_RPARAM_ERR;
    }
    else
    {
        *uiLength = uiNANDReadLength - uiOffset;
        memcpy((void *)puiBuf, (const void *)(uiAddress + uiOffset), *uiLength);
        return UPDNAND_STS_RPARAM_OK;
    }
#else
    return UPDNAND_STS_RPARAM_ERR;
#endif
//#NT#2011/01/03#Chris Chung -end
}

//@}
