/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanSD.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect SD card status
                Detect SD card insert/remove, lock/unlock

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "SysCfg.h"
#include "PrimaryTsk.h"
#include "KeyScanInt.h"
#include "FilesysTsk.h"
#include "KeyScanTsk.h"
#include "UIFlow.h"
#include "GlobalVar.h"
#include "GPIOMapping.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UIFlow.h"
#include "CalibrationTsk.h"
#include "Utility.h"



#if (_CALIBRATION_MODE_ == ENABLE)
BOOL bCalSDinsert =  FALSE;
#endif

BOOL g_bFilesysAbortCmd = FALSE;

static volatile UINT32  uiStrgCardStatus    = KEYSCAN_CARD_UNKNOWN;
//static volatile UINT32  uiStrgMacroStatus   = KEYSCAN_MACRO_UNKNOWN;
static volatile BOOL    bStrgCardLock       = FALSE;

extern UINT32 MacroShowFlag;

static BOOL g_b1stTimeStrgInit = TRUE;

#if (KEYSCAN_CARDDET_INT == ENABLE)
static volatile UINT32  uiStrgCardIntCnt  = 0;

/**
  ISR of SD card detection

  ISR of SD card detection

  @param void
  @return void
*/
static void KeyScan_DetStrgCardIsr(void)
{
    uiStrgCardIntCnt++;

    // Debounce
    if (uiStrgCardIntCnt > 1)
    {
        if (uiStrgCardStatus == KEYSCAN_CARD_REMOVED)
        {
            // Set interrupt type to level trigger, active high
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);

            Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);
            uiStrgCardStatus = KEYSCAN_CARD_INSERTED;

#if (KEYSCAN_SDWRPTDET_GPIO == ENABLE)
            bStrgCardLock = GPIOMap_DetStrgCardWP();
#endif
        }
        else
        {
            // Set interrupt type to level trigger, active low
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);

            Primary_ChangeStorage(PRIMARY_STGTYPE_NAND);
            uiStrgCardStatus = KEYSCAN_CARD_REMOVED;
            bStrgCardLock = FALSE;
        }

        uiStrgCardIntCnt = 0;
    }
}
#endif

/**
  Detect SD card is inserted or not

  Detect SD card is inserted or not.
  If SD card is inserted, then detect SD card write protect status and
  store it in bStrgCardLock
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetStrgCard(void)
{
    static UINT32   uiStrgCardPrevSts = KEYSCAN_CARD_UNKNOWN;
    UINT32          uiStrgCardCurSts;

    if (GPIOMap_DetStrgCard() == TRUE)
    {
        uiStrgCardCurSts = KEYSCAN_CARD_INSERTED;
    }
    else
    {
        uiStrgCardCurSts = KEYSCAN_CARD_REMOVED;
    }

    // Debounce
    if ((uiStrgCardCurSts == uiStrgCardPrevSts) &&
        (uiStrgCardCurSts != uiStrgCardStatus))
    {
        if (uiStrgCardCurSts == KEYSCAN_CARD_INSERTED)
        {
#if (KEYSCAN_CARDDET_INT == ENABLE)
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
#endif

            bStrgCardLock = GPIOMap_DetStrgCardWP();

            SetFSStatus(FS_NOT_INIT);
            Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);

#if (KEYSCAN_SDWRPTDET_GPIO == ENABLE)
            bStrgCardLock = GPIOMap_DetStrgCardWP();
#endif
            if (bStrgCardLock)
            {
                SetCardStatus(CARD_LOCKED);
            }
            else
            {
                SetCardStatus(CARD_INSERTED);
            }

            debug_err(("Storage Card inserted!\r\n"));
        }
        else
        {
#if (KEYSCAN_CARDDET_INT == ENABLE)
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);
#endif

            SetFSStatus(FS_NOT_INIT);
            Primary_ChangeStorage(PRIMARY_STGTYPE_NAND);

            bStrgCardLock = FALSE;

            SetCardStatus(CARD_REMOVED);

            debug_err(("Storage Card removed!\r\n"));
        }

        if (uiStrgCardStatus != KEYSCAN_CARD_UNKNOWN)
        {
#if (KEYSCAN_CARD_HOTPLUG_FUNC == ENABLE)
            //#NT#2011/08/10#KS Hung -begin
            switch (FlowMovie_GetMovDataState())
            {
            case MOV_ST_REC:
            case MOV_ST_REC | MOV_ST_ZOOM:
                 MovRec_Stop(TRUE);
		   TimerDelayMs(1000);//henry 20141218
                break;
            default:
                //KeyScan_PoweroffFlow();
                break;
            }
            //#NT#2011/08/10#KS Hung -end
            // Open storage change notice window to wait NVTEVT_STORAGE_CHANGE evt
            // Then utilize mode change (to current mode) to re-init current mode and ui window
            if(!GPIOMap_IsLCDBacklightOn())
            {
                g_uiKeyScanAutoLcdoffCnt = 0;
                GPIOMap_TurnOnLCDBacklight(); 
            }
            Ux_SendEvent(&UISystemObjCtrl,NVTEVT_CHANGE_DSCMODE,1,DSCMODE_CHGTO_CURR);

#else
            //power off while insert or remove card
            FilesysAbortCmd(FST_CMD_CLOSE_FILE      );
            FilesysAbortCmd(FST_CMD_DELETE_FILE     );
            FilesysAbortCmd(FST_CMD_FORMAT          );
            FilesysAbortCmd(FST_CMD_STORAGE_COPY_TO );
            g_bFilesysAbortCmd = TRUE;

            //set fst done for PB_WAIT_INFINITE
            set_flg(FLG_ID_FILESYS, FLGFILESYS_DONE);
            if(!GPIOMap_IsLCDBacklightOn())
            {
                g_uiKeyScanAutoLcdoffCnt = 0;
               GPIOMap_TurnOnLCDBacklight();
            }
            KeyScan_PoweroffFlow();
#endif
        }

        uiStrgCardStatus = uiStrgCardCurSts;

#if (KEYSCAN_CARDDET_INT == ENABLE)
        gpio_setIntIsr(KEYSCAN_CD_GPIO_INT, KeyScan_DetStrgCardIsr);
        gpio_enableInt(KEYSCAN_CD_GPIO_INT);
#endif
    }

    uiStrgCardPrevSts = uiStrgCardCurSts;
}

/**
  Storage callback function

  Storage (file system) initialization callback function.
  You can know the status of file system initialization.

  @param UINT32 uiStatus: The status of file system initialization
    FST_STA_OK              (Initialization OK)
    FLGKEY_STG_INITFAIL     (Unknown format, FAT error...)
    FLGKEY_STG_DISKERR      (Storage error)
  @return void
*/
void KeyScan_StorageCB(UINT32 uiStatus)
{
    switch (uiStatus)
    {
        case FST_STA_OK:
            debug_msg("FST_STA_OK!\r\n");
            SetFSStatus(FS_INIT_OK);
            break;
        case FST_STA_DISK_UNFORMAT:
            debug_msg("FST_STA_DISK_UNFORMAT!\r\n");
            SetFSStatus(FS_UNFORMATTED);
            break;
        case FST_STA_DISK_UNKNOWN_FORMAT:
            debug_msg("FST_STA_DISK_UNKNOWN_FORMAT!\r\n");
            SetFSStatus(FS_UNKNOWN_FORMAT);
            break;
        case FST_STA_CARD_ERR:
            debug_msg("FST_STA_CARD_ERR!\r\n");
            SetFSStatus(FS_DISK_ERROR);
            break;
        case FST_STA_OUT_OF_ID:
            debug_msg("FST_STA_OUT_OF_ID!\r\n");
            SetFSStatus(FS_INIT_OK);
            break;
        default:
            break;
    }

    if(g_b1stTimeStrgInit)
    {
        Ux_PostEvent(NVTEVT_STORAGE_INIT, 0);
        g_b1stTimeStrgInit = FALSE;
    }
    else
    {
        //We must make sure UIFlowWndWrnStrgHotPlug is opened!!!
        TimerDelayMs(200);
        Ux_PostEvent(NVTEVT_STORAGE_CHANGE, 0);
    }
}

/**
  Get SD card lock status

  Get SD card lock status

  @param void
  @return BOOL: TRUE -> Card is locked, FALSE -> Card is not locked
*/
BOOL KeyScan_IsStrgCardLocked(void)
{
    return bStrgCardLock;
}

//@}
