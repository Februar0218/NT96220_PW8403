/**
    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.

    @file       UserPStore.c
    @ingroup    mIPRJAPCfg

    @brief      User defined PStore section handle functions
                User defined PStore section handle functions

    @note       Nothing.

    @date       2010/05/18
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "Kernel.h"
#include "UserPStore.h"
#include "Type.h"
#include "Debug.h"
#include "SysCfg.h"
#include "PStore.h"
#include "FileSysTsk.h"


UINT8* UserPS_InitUniFontData(void)
{
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32                  uiReadSize;
    UINT8                   *pMemPool;

    get_blk((void *)&pMemPool,  POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, pMemPool);

    // Read Unicode font data from PStore to this memory pool
    pMemPool = (pMemPool + POOL_SIZE_CAPTURE - POOL_SIZE_UNIFONT);

    //PStore_EnablePS();

    if((pSection = PStore_OpenSection(PS_UNIFONT_DATA, PS_RDONLY)) != E_PS_SECHDLER)
    {
        // Get Unicode font data size first (leading 4 bytes)
        if(PStore_ReadSection(pMemPool, 0, 4, pSection) != E_PS_OK)
        {
            debug_err(("UserPS: PStore read Unicode font data size fail\r\n"));
            return NULL;
        }

        uiReadSize = MAKE_LONG(MAKE_WORD(pMemPool[0], pMemPool[1]), MAKE_WORD(pMemPool[2], pMemPool[3]));
        debug_err(("UserPS: Gotta read Unicode font data size = %ld\r\n", uiReadSize));
        if(uiReadSize > POOL_SIZE_UNIFONT)
        {
            debug_err(("UserPS: Font data size too large!\r\n"));
            return NULL;
        }

        // Get Unicode font data
        if(PStore_ReadSection(pMemPool, 4, uiReadSize, pSection) != E_PS_OK)
        {
            debug_err(("UserPS: PStore read Unicode font data fail\r\n"));
            return NULL;
        }

        PStore_CloseSection(pSection);
        //PStore_DisablePS();

        #if 0   //Just verify the data integrity
        FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, "A:\\VFONT.BIN", (UINT8 *)(pMemPool),
                            &uiReadSize, 0, FST_TIME_INFINITE);
        #endif
    }
    else
    {
        debug_err(("UserPS: PStore open section fail\r\n"));
        //PStore_DisablePS();
        return NULL;
    }

    return pMemPool;
}

UINT32 UserPS_WriteUniFontFile(CHAR *pFilename)
{
    PPSTORE_SECTION_HANDLE  pSection;
    HNVT_FILE               *pFile;
    UINT32                  uiFileSize, uiWriteSize;
    UINT8                   *pMemPool;

    FilesysWaitInitFinish(FST_TIME_INFINITE);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    pFile = Filesys_fopen((char *)pFilename, "R");
    if(pFile == NULL)
    {
        debug_err(("UserPS: Error opening %s\r\n", pFilename));
        return E_USERPS_FILE;
    }

    uiFileSize = pFile->fileSize;
    uiWriteSize = uiFileSize;
    debug_err(("UserPS: File size = %ld\r\n", uiFileSize));
    FilesysCloseActFile();

    if(uiFileSize > POOL_SIZE_UNIFONT)
    {
        debug_err(("UserPS: File size too large!\r\n"));
        return E_USERPS_FILE;
    }

    get_blk((void *)&pMemPool, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, pMemPool);

    pMemPool = (pMemPool + POOL_SIZE_CAPTURE - POOL_SIZE_UNIFONT);
    FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, pFilename, (UINT8 *)(pMemPool+4),
                            &uiWriteSize, 0, FST_TIME_INFINITE);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    debug_err(("UserPS: Gotta write file size = %ld\r\n", uiWriteSize));

    *pMemPool = LO_BYTE(LO_WORD(uiWriteSize));
    *(pMemPool+1) = HI_BYTE(LO_WORD(uiWriteSize));
    *(pMemPool+2) = LO_BYTE(HI_WORD(uiWriteSize));
    *(pMemPool+3) = HI_BYTE(HI_WORD(uiWriteSize));

    //PStore_EnablePS();

    if((pSection = PStore_OpenSection(PS_UNIFONT_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        if(PStore_WriteSection(pMemPool, 0, (uiWriteSize+4), pSection) != E_PS_OK)
        {
            debug_err(("UserPS: PStore write Unicode font data fail\r\n"));
        }
        PStore_CloseSection(pSection);
        //PStore_DisablePS();
    }
    else
    {
        debug_err(("UserPS: PStore open section fail\r\n"));
        //PStore_DisablePS();
        return E_USERPS_PSECTION;
    }

    return E_USERPS_OK;
}




//@}
