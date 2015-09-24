/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitMp3.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for Mp3 playback
                Get memory and set parameters for Mp3 playback

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"

#if (_MP3_MODE_ == ENABLE)
#include "FilesysTsk.h"
#include "MP3Api.h"
#include "UIFlow.h"

/**
  Initialize application for MP3 mode

  Initialize application for MP3 mode.

  @param void
  @return void
*/
void AppInit_ModeMp3(void)
{
    UINT32              uiPoolAddr;
    MP3PLAYFILE_PARAM   MP3PlayFileParam;

    FilesysWaitInitFinish(FST_TIME_INFINITE);

    // Wait for new file be writed to SD card
    if(FilesysWaitCmdFinish(FST_TIME_NO_WAIT) == FST_STA_BUSY)
        FilesysWaitCmdFinish(FST_TIME_INFINITE);

    // Get memory for MP3 playfile task
    get_blk((VP *)&uiPoolAddr, POOL_ID_FTABLE);
    rel_blk(POOL_ID_FTABLE, (VP)uiPoolAddr);

    memset((void *)&MP3PlayFileParam, 0x00, sizeof(MP3PlayFileParam));
    MP3PlayFileParam.indexTableAddr = (UINT8 *)uiPoolAddr;
    MP3PlayFileParam.FileReadMode = MP3FILEREADMODE_BYINDEX;
    MP3PlayFileParam.readModeParam = MP3FILEREADMODE_BYINDEX;
    MP3PlayFileParam.nandRevFileNum = 0;
    MP3PlayFileParam.NeedInitIndexTable = (Get_MP3FtbRenewIndex() == MP3FTB_RENEW_ON)? 1:0;
    MP3PlayFileParam.NeedResetIndexTable = 1;

    if (MP3PlayFileRead_Open(&MP3PlayFileParam) == E_SYS)
    {
        debug_err(("No MP3 file !!!\r\n"));
        return;
    }
    MP3PlyaFileApi_ResetTableAndGetMp3Num();

    // Reset mp3 filetable renew request to off
    Set_MP3FtbRenewIndex(MP3FTB_RENEW_OFF);
}

#endif //_MP3_MODE_

//@}
