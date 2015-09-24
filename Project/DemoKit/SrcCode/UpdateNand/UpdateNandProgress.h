/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UpdateNandProgress.h
    @ingroup    mIPRJAPInit

    @brief      Show progress of Updating data in NAND flash reserved area
                Show progress of Updating data in NAND flash reserved area.

    @note       Nothing.

    @date       2006/01/26
*/

/** \addtogroup mIPRJAPInit */
//@{

#ifndef _UPDATENANDPROGRESS_H
#define _UPDATENANDPROGRESS_H

#include "Type.h"

// UpdateNand_PregressFW() uiID
#define UPDNAND_PGS_FW_READING      0
#define UPDNAND_PGS_FW_READ_OK      1
#define UPDNAND_PGS_FW_READ_CHK_OK  2
#define UPDNAND_PGS_FW_WRITING      3
#define UPDNAND_PGS_FW_WRITE_OK     4
#define UPDNAND_PGS_FW_WRITE_CHK_OK 5

extern void UpdateNand_ProgressFW(UINT32 uiID, UINT32 uiParam1, UINT32 uiParam2);

#endif

//@}
