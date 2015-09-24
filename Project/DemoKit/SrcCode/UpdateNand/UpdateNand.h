/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UpdateNand.h
    @ingroup    mIPRJAPInit

    @brief      Update data in NAND flash reserved area
                Update data (now only support FW) in NAND flash reserved area

    @note       Nothing.

    @date       2006/01/26
*/

/** \addtogroup mIPRJAPInit */
//@{

#ifndef _UPDATENAND_H
#define _UPDATENAND_H

#include "Type.h"
#include "PrimaryTsk.h"

// UpdateNand_UpdateFW() return status
#define UPDNAND_STS_FW_OK               0   // Update FW to NAND OK
#define UPDNAND_STS_FW_INVALID_STG      1   // Invalid source storage
#define UPDNAND_STS_FW_READ_ERR         2   // FW doesn't exist or read error
#define UPDNAND_STS_FW_READ_CHK_ERR     3   // Read FW checksum failed, might be error
#define UPDNAND_STS_FW_WRITE_ERR        4   // Write FW to NAND error
#define UPDNAND_STS_FW_READ2_ERR        5   // Read FW from NAND failed (for write checking)
#define UPDNAND_STS_FW_WRITE_CHK_ERR    6   // Write FW checksum failed

// UpdateNand_WriteParam() return status
#define UPDNAND_STS_WPARAM_OK           0   // Write param to NAND OK
#define UPDNAND_STS_WPARAM_ERR          1

// UpdateNand_ReadParam() return status
#define UPDNAND_STS_RPARAM_OK           0
#define UPDNAND_STS_RPARAM_ERR          1

extern UINT32   UpdateNand_UpdateFW(UINT32 uiStorage);
extern UINT32   UpdateNand_WriteParam(UINT32 uiBlockID, UINT8 *puiBuf, UINT32 uiLength);
extern UINT32   UpdateNand_ReadParam(UINT32 uiBlockID, UINT32 uiOffset, UINT32 *uiLength, UINT8 *puiBuf);
extern void     UpdateNand_LoadFW(UINT32 uiStorage);
#endif

//@}
