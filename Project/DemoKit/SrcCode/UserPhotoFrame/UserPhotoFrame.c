#include <string.h>
#include "UserPhotoFrame.h"
#include "ide.h"
#include "PhotoFrame.h"
#include "PStore.h"
#include "FilesysTsk.h"
#include "Type.h"

static PHOTO_FRAME_INFO PhotoFrameInfo[PHOTO_FRAME_ID_MAX] = {0};
static BOOL     bInitPhotoFrame = FALSE;
static UINT32   uiPhotoFrameTotalNum = 0;

static BOOL UserPF_ReadHeader(void)
{
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32 i;

    if(bInitPhotoFrame)
    {
        return TRUE;
    }

    memset((void *)(&PhotoFrameInfo[0]), 0, sizeof(PHOTO_FRAME_INFO) * PHOTO_FRAME_ID_MAX);

    //PStore_EnablePS();

    if ((pSection = PStore_OpenSection(PS_PHOTO_FRAME_HEADER, PS_RDONLY)) != E_PS_SECHDLER)
    {
        if (PStore_ReadSection((UINT8 *)(&PhotoFrameInfo[0]), 0, sizeof(PHOTO_FRAME_INFO) * PHOTO_FRAME_ID_MAX, pSection) != E_OK)
        {
            PStore_CloseSection(pSection);
            //PStore_DisablePS();
            debug_err(("UserPF_ReadHeader: read section error\r\n"));
            return FALSE;
        }
    }
    else
    {
        debug_err(("UserPF_ReadHeader: open section error\r\n"));
        //PStore_DisablePS();
        return FALSE;
    }

    PStore_CloseSection(pSection);
    //PStore_DisablePS();

    for(i=0; i<PHOTO_FRAME_ID_MAX; i++)
    {
        if(PhotoFrameInfo[i].uiFileSize > 0)
        {
            uiPhotoFrameTotalNum++;
            debug_err(("frameId=%d, filesize=%d, addrOffset=%d\r\n",
                        i, PhotoFrameInfo[i].uiFileSize, PhotoFrameInfo[i].uiAddrOffset));
        }
        else
        {
            break;
        }
    }

    bInitPhotoFrame = TRUE;

    return TRUE;
}

BOOL UserPF_ReadFrame(UINT32 uiFrameID, UINT32 uiBufAddr, PPHOTO_FRAME_INFO pFrameInfo)
{
    PPSTORE_SECTION_HANDLE  pSection;
    PPHOTO_FRAME_INFO       pInfo;

    if(UserPF_ReadHeader()==FALSE)
    {
        return FALSE;
    }

    if(uiFrameID > uiPhotoFrameTotalNum)
    {
        return FALSE;
    }

    pInfo = (PPHOTO_FRAME_INFO)(&PhotoFrameInfo[uiFrameID]);

    //PStore_EnablePS();

    if ((pSection = PStore_OpenSection(PS_PHOTO_FRAME_DATA, PS_RDONLY)) != E_PS_SECHDLER)
    {
        if (PStore_ReadSection((UINT8 *)uiBufAddr, pInfo->uiAddrOffset, pInfo->uiFileSize, pSection) != E_OK)
        {
            PStore_CloseSection(pSection);
            //PStore_DisablePS();
            debug_err(("UserPF_ReadFrame: read section error\r\n"));
            memset((void *)pFrameInfo, 0, sizeof(PHOTO_FRAME_INFO));
            return FALSE;
        }
        /*
        else
        {
            // dump data test
            UINT32 i;
            debug_err(("*** pInfo->uiFileSize = %d\r\n", pInfo->uiFileSize));
            i = 0;
            while (i < pInfo->uiFileSize)
            {
                debug_msg("%02x ", *((UINT8 *)uiBufAddr + i));
                i++;
                if ((i % 16) == 0)
                {
                    debug_msg("\r\n");
                }
            }
            debug_msg("\r\n");
        }
        */
    }
    else
    {
        debug_err(("UserPF_ReadFrame: open section error\r\n"));
        //PStore_DisablePS();
        memset((void *)pFrameInfo, 0, sizeof(PHOTO_FRAME_INFO));
        return FALSE;
    }

    PStore_CloseSection(pSection);
    //PStore_DisablePS();

    memcpy((void *)pFrameInfo, (const void *)pInfo, sizeof(PHOTO_FRAME_INFO));

    return TRUE;
}

BOOL UserPF_WriteHeader(void)
{
    PPSTORE_SECTION_HANDLE  pSection;

    debug_err(("UserPF_WriteHeader()\r\n"));

    //PStore_EnablePS();

    if ((pSection = PStore_OpenSection(PS_PHOTO_FRAME_HEADER, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        if (PStore_WriteSection((UINT8 *)(&PhotoFrameInfo[0]), 0, sizeof(PHOTO_FRAME_INFO) * PHOTO_FRAME_ID_MAX, pSection) != E_OK)
        {
            PStore_CloseSection(pSection);
            //PStore_DisablePS();
            debug_err(("UserPF_WriteHeader: write section error\r\n"));
            return FALSE;
        }
    }
    else
    {
        debug_err(("UserPF_WriteHeader: open section error\r\n"));
        //PStore_DisablePS();
        return FALSE;
    }

    PStore_CloseSection(pSection);
    //PStore_DisablePS();

    return TRUE;
}

BOOL UserPF_WriteFrame(UINT32 uiFrameID, char *pFileName)
{
    UINT32                  uiPoolAddr;
    UINT32                  uiBufSize;
    PPSTORE_SECTION_HANDLE  pSection;
    PPHOTO_FRAME_INFO       pInfo;

    // Get memory for photo frame bitstream buffer
    //get_blk((VP*)&(uiPoolAddr), POOL_ID_SIDC);
    //rel_blk(POOL_ID_SIDC, (VP)uiPoolAddr);

    // Note: overlap with date stamp buffer as writing photo frame bitstream to PStore
    uiPoolAddr = END_MEM - (POOL_SIZE_DATE_STAMP + POOL_SIZE_DATE_STAMP_THUMB);

    pInfo = (PPHOTO_FRAME_INFO)(&PhotoFrameInfo[uiFrameID]);

    if(uiFrameID == 0)
    {
        pInfo->uiAddrOffset = 0;
    }
    else
    {
        //The address offset must be word-alignment.
        pInfo->uiAddrOffset = ((PhotoFrameInfo[uiFrameID-1].uiAddrOffset + PhotoFrameInfo[uiFrameID-1].uiFileSize + 4 - 1)/4)*4;
    }

    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, pFileName, (UB *)uiPoolAddr, &uiBufSize, 0, FST_TIME_INFINITE) != FST_STA_OK)
    {
        return FALSE;
    }

    if(uiBufSize == 0)
    {
        return FALSE;
    }
    else
    {
        pInfo->uiFileSize = uiBufSize;
    }

    //PStore_EnablePS();

    if ((pSection = PStore_OpenSection(PS_PHOTO_FRAME_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        if (PStore_WriteSection((UINT8 *)uiPoolAddr, pInfo->uiAddrOffset, pInfo->uiFileSize, pSection) != E_OK)
        {
            PStore_CloseSection(pSection);
            //PStore_DisablePS();
            debug_err(("UserPF_WriteFrame: write section error\r\n"));
            return FALSE;
        }
    }
    else
    {
        debug_err(("UserPF_WriteFrame: open section error\r\n"));
        //PStore_DisablePS();
        return FALSE;
    }

    PStore_CloseSection(pSection);
    //PStore_DisablePS();

    debug_err(("UserPF_WriteFrame: write frameId=%d, filename=%s, filesize=%d, addrOffset=%d ok!!\r\n",
                uiFrameID, pFileName, pInfo->uiFileSize, pInfo->uiAddrOffset));

    return TRUE;
}

UINT32 UserPF_GetTotalNum(void)
{
    UserPF_ReadHeader();

    return uiPhotoFrameTotalNum;
}

void UserPF_DeleteAll(void)
{
    bInitPhotoFrame = FALSE;

    uiPhotoFrameTotalNum = 0;

    memset((void *)(&PhotoFrameInfo[0]), 0, sizeof(PHOTO_FRAME_INFO) * PHOTO_FRAME_ID_MAX);

    //PStore_EnablePS();

    PStore_DeleteSection(PS_PHOTO_FRAME_HEADER);

    PStore_DeleteSection(PS_PHOTO_FRAME_DATA);

    //PStore_DisablePS();
}

BOOL UserPF_OpenPhotoFrame(UINT32 uiMode, UINT32 uiFrameID)
{
    UINT32              uiPoolAddr;
    PHOTO_FRAME_INFO    FrameInfo;
    IMAGE_BORDER_INFO   BorderInfo;

    //debug_err(("Open Phofo Frame=%d\r\n", uiFrameID));

    if ((uiMode != PRIMARY_MODE_PHOTO) && (uiMode != PRIMARY_MODE_PLAYBACK))
    {
        return FALSE;
    }

    // Avoid to open twice when mode change.
    ////if (frameId == SysGetFL(FL_PHOTO_FRAME) &&
    ////    PhotoFrame_GetImageBorderStatus() == TRUE)
    if ((uiMode == PRIMARY_MODE_PHOTO) && (PhotoFrame_GetImageBorderStatus() == TRUE))
    {
        return TRUE;
    }

    // Photo Frame
    // Get memory for photo frame bitstream buffer
    //get_blk((VP*)&(uiPoolAddr), POOL_ID_SIDC);
    //rel_blk(POOL_ID_SIDC, (VP)uiPoolAddr);

    // Note: overlap with date stamp buffer as photo frame open
    uiPoolAddr = END_MEM - (POOL_SIZE_DATE_STAMP + POOL_SIZE_DATE_STAMP_THUMB);

    UserPF_ClosePhotoFrame();

    if (UserPF_ReadFrame(uiFrameID, uiPoolAddr, &FrameInfo) == TRUE)
    {
        UINT32 fbWidth, fbHeight, fbOffset;

        ide_getV1BufDim(&fbWidth, &fbHeight, &fbOffset);

        // Set photo frame selection JPEG to image pipeline
        BorderInfo.uiBorderFileAddr         = (UINT32)uiPoolAddr;
        BorderInfo.uiBorderFileSize         = FrameInfo.uiFileSize;
        BorderInfo.uiBorderWidth            = 640;
        BorderInfo.uiBorderHeight           = 480;
        //BorderInfo.uiBorderColorKeyY        = 6;
        BorderInfo.uiBorderColorKeyY        = 245;
        BorderInfo.uiBorderColorKeyCb       = 128;
        BorderInfo.uiBorderColorKeyCr       = 128;
        //BorderInfo.bYOperationIsLess        = TRUE; // for black background (using < color key operation)
        BorderInfo.bYOperationIsLess        = FALSE; // for white background (using >= color key operation)
        //#NT#2009/05/25#York Dai -begin
        // Fix the photoframe during TV-Out because the frame buffer size is not correct
        if (uiMode == PRIMARY_MODE_PHOTO)
        {
            BorderInfo.uiPrvHsize           = 320;//FD_BUF_HSIZE; //???
            BorderInfo.uiPrvVsize           = 240;//FD_BUF_VSIZE; //???
        }
        else
        {
            BorderInfo.uiPrvHsize           = fbWidth;
            BorderInfo.uiPrvVsize           = fbHeight;
        }
        //#NT#2009/05/25#York Dai -end
        BorderInfo.uiPrvOffset              = 0;
        //debug_err(("Photo DisplayFB: %dx%d\r\n", BorderInfo.uiPrvHsize, BorderInfo.uiPrvVsize));
        PhotoFrame_SetImageBorder(uiMode, &BorderInfo);
        PhotoFrame_EnableImageBorder(TRUE);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void UserPF_ClosePhotoFrame(void)
{
    if (PhotoFrame_GetImageBorderStatus() == TRUE)
    {
        PhotoFrame_EnableImageBorder(FALSE);    //Photo mode need this to avoid preview lag!!
        ImageBorder_CloseImageBorder();
        ////SensorDelayVD(1);
    }
}
