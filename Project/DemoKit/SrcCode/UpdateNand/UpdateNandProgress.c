/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UpdateNandProgress.c
    @ingroup    mIPRJAPInit

    @brief      Show progress of Updating data in NAND flash reserved area
                Show progress of Updating data in NAND flash reserved area.

    @note       Nothing.

    @date       2006/01/26
*/

/** \addtogroup mIPRJAPInit*/
//@{

#include "Kernel.h"
#include "SysCfg.h"
#include "UpdateNandProgress.h"

#define UPDNAND_FW_BAR_WIDTH        280
#define UPDNAND_FW_BAR_5P_WIDTH     14

/**
  Show FW updating progress

  Show FW updating progress.

  @param UINT32 uiID Which status
  @param UINT32 uiParam1 Parameter1. Only valid in writing status, blocks written
  @param UINT32 uiParam2 Parameter2. Only valid in writing status, total blocks
  @return void
*/
void UpdateNand_ProgressFW(UINT32 uiID, UINT32 uiParam1, UINT32 uiParam2)
{
#if 0
    RECT            UpdateRect  = {20, 104, 280, 32};
    RECT            BarRect     = {20, 136, UPDNAND_FW_BAR_WIDTH, 10};
    SHOWOSD_STRING  DrawString;
    SHOWOSD_RECT    DrawRect;

    DrawRect.pRect  = &UpdateRect;
    ShowOSD_ClearRect(IDE_OSDID_1, &DrawRect);

    DrawString.pRect                = &UpdateRect;
    DrawString.uiColorIndex         = SHOWOSD_COLOR_WHITE;
    DrawString.uiBorderColorIndex   = SHOWOSD_COLOR_GRAY_50;
    DrawString.uiAlignment          = KV_CENTER | KH_LEFT;
    DrawString.bClear               = FALSE;

    DrawRect.pRect                  = &BarRect;
    DrawRect.DrawOP                 = OSD_DRAW_FILL;
    DrawRect.uiColorIndex           = SHOWOSD_COLOR_WHITE;
    switch (uiID)
    {
    case UPDNAND_PGS_FW_READING:
        debug_ind(("Reading FW\r\n"));
        DrawString.pString = "Reading FW: ...";
        ShowOSD_DrawString(IDE_OSDID_1, &DrawString);
        ShowOSD_ClearRect(IDE_OSDID_1, &DrawRect);
        break;

    case UPDNAND_PGS_FW_READ_OK:
        debug_ind(("Reading FW OK\r\n"));
        DrawString.pString = "Reading FW: OK";
        ShowOSD_DrawString(IDE_OSDID_1, &DrawString);
        // 10%
        BarRect.uiWidth = UPDNAND_FW_BAR_5P_WIDTH << 1;
        ShowOSD_DrawRect(IDE_OSDID_1, &DrawRect);
        break;

    case UPDNAND_PGS_FW_READ_CHK_OK:
        debug_ind(("Verify reading FW OK\r\n"));
        DrawString.pString = "Verify FW: OK";
        ShowOSD_DrawString(IDE_OSDID_1, &DrawString);
        // 15%
        BarRect.uiWidth = UPDNAND_FW_BAR_5P_WIDTH * 3;
        ShowOSD_DrawRect(IDE_OSDID_1, &DrawRect);
        break;

    case UPDNAND_PGS_FW_WRITING:
        debug_ind(("Writing FW: %d%%\r\n"));
        DrawString.pString = "Writing FW: ...";
        ShowOSD_DrawString(IDE_OSDID_1, &DrawString);
        // 15% - 95%
        BarRect.uiWidth = UPDNAND_FW_BAR_5P_WIDTH * 3 + (UPDNAND_FW_BAR_5P_WIDTH << 4) * uiParam1 / uiParam2;
        ShowOSD_DrawRect(IDE_OSDID_1, &DrawRect);
        break;

    case UPDNAND_PGS_FW_WRITE_OK:
        debug_ind(("Writing FW OK\r\n"));
        DrawString.pString = "Writing FW: OK";
        ShowOSD_DrawString(IDE_OSDID_1, &DrawString);
        // 95%
        BarRect.uiWidth = UPDNAND_FW_BAR_WIDTH - UPDNAND_FW_BAR_5P_WIDTH;
        ShowOSD_DrawRect(IDE_OSDID_1, &DrawRect);
        break;

    case UPDNAND_PGS_FW_WRITE_CHK_OK:
        debug_ind(("Verify Writing FW OK\r\n"));
        DrawString.pString = "Verify FW: OK";
        ShowOSD_DrawString(IDE_OSDID_1, &DrawString);
        // 100%
        BarRect.uiWidth = UPDNAND_FW_BAR_WIDTH;
        ShowOSD_DrawRect(IDE_OSDID_1, &DrawRect);
        break;

    default:
        break;
    }
#endif
}

//@}
