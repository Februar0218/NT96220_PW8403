/**
    Copyright   Novatek Microelectronics Corp. 2010. All rights reserved.

    @file       KeyScanIrRx.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan IrRx received command key
                Scan IrRx received command key

    @note       Nothing.

    @version    V1.00.000
    @author     JustinLee
    @date       2010/03/31
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "SysCfg.h"
#include "KeyScanInt.h"
#include "KeyScanTsk.h"
#include "Debug.h"
#include "IrRx.h"
#include "IrRxParser.h"


/**
  Check IR data expiration, to verify IR signal is stop sending or not.

  Need task periodic polling, frequency value must be smaller than 100ms.

  @param void
  @return void
*/
void KeyScan_CheckIrRxExp(void)
{
#if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
    irrx_checkDataExp();
#endif
}

/**
  Scan IrRx received command data and pickup the translated key from parser.

  Scan IrRx received command data and pickup the translated key from parser.

  @param void
  @return void
*/
void KeyScan_DetIrRxKey(void)
{
#if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
    IRRX_DATA_QUEUE IrRxGetDataQ = {0, 0, 0};
    static UINT32 uiIrRxData = 0;
    UINT32 uiMappedKey = 0;
    UINT32 uiTempKeyStatus = 0;

    #if 0   // Use if w/o IrRx data queue
    // Check if there is one data frame ready to pick
    if(irrx_isDataReady())
    {
        uiIrRxData = irrx_getDataContent();
        debug_err(("%010lu> IrRx: CODE: 0x%08X\r\n", irrx_getCurrTimerCnt(), uiIrRxData));
        uiMappedKey = IrRx_ParseKey(g_pIrCodeTb, uiIrRxData);
    }

    // Check if there is one Repeat Code need to report
    if(irrx_isDataRepeat())
    {
        debug_err(("%010lu> IrRx: CODE: REPEAT (0x%08X)\r\n", irrx_getCurrTimerCnt(), uiIrRxData));
    }
    #endif

    if(irrx_dequeueData((PIRRX_DATA_QUEUE)&IrRxGetDataQ))
    {
        if(IrRxGetDataQ.uiRxData == IRRX_REPEAT_CODE_IDT)
        {
            debug_err(("IrRx(%010lu): CODE: REPEAT (0x%08X)\r\n", IrRxGetDataQ.uiRxTime, uiIrRxData));
        }
        else
        {
            uiIrRxData = IrRxGetDataQ.uiRxData;
            debug_err(("IrRx(%010lu): CODE: 0x%08X\r\n", IrRxGetDataQ.uiRxTime, uiIrRxData));

            uiMappedKey = IrRx_ParseKey(g_pIrCodeTb, uiIrRxData);
        }
    }

    if(uiMappedKey)
    {
        debug_ind(("IrRx: Key: 0x%08X\r\n", uiMappedKey));

        uiTempKeyStatus = (uiMappedKey & KeyScan_GetKeyMask(KEYSCAN_KEYMODE_PRESS));

        if (uiTempKeyStatus)
        {
            // Play keypad tone
            //KeyScan_PlaySound(KEYSCAN_SOUND_KEY_OTHERS);

            // Key pressed event
            g_uiKeyScanFlags |= (FLGKEY_PRESSED | uiTempKeyStatus);
        }

        // Reset auto power off counter
        g_uiKeyScanAutoPoweroffCnt = 0;
    }
#endif
}



//@}
