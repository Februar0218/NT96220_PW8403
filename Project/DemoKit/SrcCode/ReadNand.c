//#NT#2009/01/06#Chris Hsu -begin
//#NT#Re-write the function to prevent command task hang and improve performance
/*
    Dump Nand data to storage card

    This file is dumping nand data to storage card

    @file       ReadNand.c
    @ingroup    DumpNandData
    @note       2K per page is not verified yet.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include "SysCfg.h"
#include "NAND.h"
#include "PrimaryTsk.h"
#include "FilesysTsk.h"
#include "ReadNand.h"
#include "GPIOMapping.h"

extern ER   nand_read2KOperation(INT8 *buffer, UINT32 pageAddress, UINT32 numPage);
extern ER   nand_read2KPageSpareDataTotal(UINT32 *spare0,
                                          UINT32 *spare1,
                                          UINT32 *spare2,
                                          UINT32 *spare3,
                                          UINT32 pageAddress);
extern ER   nand_readOperation(INT8 *buffer, UINT32 pageAddress, UINT32 numPage);
extern ER   nand_readPageSpareDataTotal(UINT32 *spare0,
                                        UINT32 *spare1,
                                        UINT32 *spare2,
                                        UINT32 *spare3,
                                        UINT32 pageAddress);

/**
    @addtogroup DumpNandData
*/
//@{

// ON:  Main data size per loop is decided by POOL_SIZE_CAPTURE
// OFF: User define main data size per loop
#define READNAND_LOOPSIZE_BY_MEM    OFF

#if (READNAND_LOOPSIZE_BY_MEM == ON)
  #if   (POOL_SIZE_CAPTURE < 0x00400000)
    #error Not enough memory to dump NAND data, POOL_SIZE_CAPTURE must >= 4MB
  #elif (POOL_SIZE_CAPTURE < 0x00800000)
    #define READNAND_LOOP_SIZE      0x00400000      // 4 MB
  #elif (POOL_SIZE_CAPTURE < 0x01000000)
    #define READNAND_LOOP_SIZE      0x00800000      // 8 MB
  #elif (POOL_SIZE_CAPTURE < 0x02000000)
    #define READNAND_LOOP_SIZE      0x01000000      // 16 MB
  #else
    #define READNAND_LOOP_SIZE      0x02000000      // 32 MB
  #endif
#else
    #define READNAND_LOOP_SIZE      0x00800000      // 8 MB
//KH mark    
#if 0    
    #if (POOL_SIZE_CAPTURE < READNAND_LOOP_SIZE)
      #error Not enough memory to dump NAND data, POOL_SIZE_CAPTURE must >= READNAND_LOOP_SIZE
    #endif
#endif    
#endif

// Since we can't get page size information from NAND driver,
// We can't change read function dynamically. (2009/01/06)
#define READNAND_PAGE_SIZE_512B     0x200
#define READNAND_PAGE_SIZE_2KB      0x800
#define READNAND_PAGE_SIZE          READNAND_PAGE_SIZE_512B

/**
    Read NAND total main / spare data

    Read NAND total data include spare data and write to storage card.

    @param[in] uiMainDataSize   How many main data will be dumped. Always dump block size alignment.
                                If uiMainDataSize == 0 (READNAND_MDATASIZE_ALL) will dump all main data.
    @return void
*/
void ReadNand_Total(UINT32 uiMainDataSize)
{
    UINT32              i, j, loop, uiPoolAddr, uiTransSize;
    UINT32              ui32pageAddress, uiLoopCnt;
    UINT8               *pui8Buf;
    NandXdInfoStruct    NandInfo;
    PSTRG_TAB           pCardObj, pNANDObj;

    if(nand_getDeviceInfo(&NandInfo) == FALSE)
    {
        debug_msg("Can't get NAND information!\r\n");
        return;
    }

    // Check storage card exist or not
    if(GPIOMap_DetStrgCard() != TRUE)
    {
        debug_msg("Storage card doesn't exist!\r\n");
        return;
    }

    //NandInfo.uiPhysicalBlockCount = 2048;

    // Get memory for buffer
    get_blk((VP *)&uiPoolAddr, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    // Check buffer size for spare data
#if   (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_512B)
    if ((NandInfo.uiPagePerBlock * 16 * NandInfo.uiPhysicalBlockCount) > POOL_SIZE_CAPTURE)
#elif (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_2KB)
    // NAND driver will return real page per block * 4 for 2KB per page NAND
    if (((NandInfo.uiPagePerBlock >> 2) * 16 * NandInfo.uiPhysicalBlockCount) > POOL_SIZE_CAPTURE)
#endif
    {
        debug_msg("Not enough memory to dump NAND data!\r\n");
        return;
    }

    // Assign page starting address
    ui32pageAddress = 0;

    // Assign main data size to dump
    if (uiMainDataSize == READNAND_MDATASIZE_ALL ||
        uiMainDataSize > (NandInfo.uiBlockSize * NandInfo.uiPhysicalBlockCount))
    {
        uiMainDataSize = NandInfo.uiBlockSize * NandInfo.uiPhysicalBlockCount;
    }

    // Calculate loop counter
    uiLoopCnt = uiMainDataSize / READNAND_LOOP_SIZE;
    if (uiLoopCnt == 0)
    {
        uiLoopCnt = 1;
    }

    // Change storage to card
    if (Primary_GetStorage() != PRIMARY_STGTYPE_CARD)
    {
        debug_msg("Initialize card!\r\n");

        Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);
    }

    // Wait for change storage done
    Primary_Wait4ChangeStorageDone();

    // Wait for File system command finished
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    // Get storage objects
    pNANDObj = Primary_GetStorageObj(PRIMARY_STGTYPE_NAND);
    pCardObj = Primary_GetStorageObj(PRIMARY_STGTYPE_CARD);

    debug_msg("Dumping main data! Total %ld KB, %ld MB per loop", uiMainDataSize >> 10, READNAND_LOOP_SIZE >> 20);

    for (loop=0; loop<uiLoopCnt; loop++)
    {
        debug_msg("\r\nLoop: %02d\r\n", loop);

        // Switch storage from card to NAND to read main data
        pCardObj->CloseMemBus();
        pNANDObj->OpenMemBus();

        pui8Buf         = (UINT8 *)uiPoolAddr;
        uiTransSize     = 0;

        // Read NAND data
        for (i=loop * (NandInfo.uiPhysicalBlockCount / uiLoopCnt);
             i<((loop + 1) * (NandInfo.uiPhysicalBlockCount / uiLoopCnt)); i++)
        {
#if   (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_512B)
            nand_readOperation((INT8 *)pui8Buf, ui32pageAddress, NandInfo.uiPagePerBlock);
#elif (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_2KB)
            nand_read2KOperation((INT8 *)pui8Buf, ui32pageAddress, NandInfo.uiPagePerBlock);
#endif
            ui32pageAddress += NandInfo.uiBlockSize;
            pui8Buf         += NandInfo.uiBlockSize;
            uiTransSize     += NandInfo.uiBlockSize;

            // All data is dumped?
            if (uiTransSize >= uiMainDataSize)
            {
                break;
            }

            // Ouput progress message
            if ((i & 0x3F) == 0)
            {
                debug_msg(".");
            }
        }

        // Switch storage from NAND to card to write data
        pNANDObj->CloseMemBus();
        pCardObj->OpenMemBus();

        // Write to card
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                         (UB *)uiPoolAddr,
                         uiTransSize,
                         0,
                         TRUE,
                         FST_FMT_JPG,
                         FST_TIME_INFINITE);
    }

    debug_msg("\r\nDumping spare data!\r\n");

    // Switch storage from card to NAND to read spare data
    pCardObj->CloseMemBus();
    pNANDObj->OpenMemBus();

    pui8Buf         = (UINT8 *)uiPoolAddr;
    uiTransSize     = 0;
    ui32pageAddress = 0;

    // Read spare data from NAND.
    // Both 512B or 2KB per page will read 16 byte spare data per page.
    for (i=0; i<NandInfo.uiPhysicalBlockCount; i++)
    {
#if   (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_512B)
        for (j=0; j<NandInfo.uiPagePerBlock; j++)
#elif (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_2KB)
        // NAND driver will return real page per block * 4 for 2KB per page NAND
        for (j=0; j<(NandInfo.uiPagePerBlock>>2); j++)
#endif
        {
#if   (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_512B)
            nand_readPageSpareDataTotal((UINT32 *)(pui8Buf),
#elif (READNAND_PAGE_SIZE == READNAND_PAGE_SIZE_2KB)
            nand_read2KPageSpareDataTotal((UINT32 *)(pui8Buf),
#endif
                                        (UINT32 *)(pui8Buf +  4),
                                        (UINT32 *)(pui8Buf +  8),
                                        (UINT32 *)(pui8Buf + 12),
                                        ui32pageAddress);
            pui8Buf         += 16;
            ui32pageAddress += READNAND_PAGE_SIZE;
        }
        uiTransSize += (j * 16);

        // Ouput progress message
        if ((i & 0x3F) == 0)
        {
            debug_msg(".");
        }
    }

    // Switch storage from NAND to card to write data
    pNANDObj->CloseMemBus();
    pCardObj->OpenMemBus();

    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                     (UB *)uiPoolAddr,
                     uiTransSize,
                     0,
                     TRUE,
                     FST_FMT_RAW,
                     FST_TIME_INFINITE);

    debug_msg("\r\nDone!\r\n");
}

//@}
//#NT#2009/01/06#Chris Hsu -end
