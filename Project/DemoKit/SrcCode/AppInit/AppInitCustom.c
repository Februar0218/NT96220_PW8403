/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitCustom.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for custom mode
                Get memory and set parameters for custom mode

    @note       Nothing.

    @date       2006/02/07
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#if (_USBWRITE_MODE_ == ENABLE)
#include "Kernel.h"
#include "UpdateNand.h"
#include "USBMakerParamInit.h"
#endif

/**
  Initialize application for custom mode 1

  Initialize application for custom mode 1.

  @param void
  @return void
*/
void AppInit_ModeCustom1(void)
{
}

/**
  Initialize application for custom mode 2

  Initialize application for custom mode 2.

  @param void
  @return void
*/
void AppInit_ModeCustom2(void)
{
#if (_USBWRITE_MODE_ == ENABLE)
    USB_PCC_INFO    PCCInfo;
    UINT32          uiPoolAddr;

    memset((void*)&PCCInfo, 0, sizeof(USB_PCC_INFO));

    // Get memory for PCC task
    SysGetSICDAddr(&uiPoolAddr);

    PCCInfo.UsbPccMemAdr    = uiPoolAddr;
    PCCInfo.UsbPccMemSize   = POOL_SIZE_SICD;

    //For write serial number to NAND via PCC
    #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
    PCCInfo.BurnIn          = UpdateNand_WriteParam;
    PCCInfo.FlashRead       = UpdateNand_ReadParam;
    PCCInfo.IsFinish        = NULL;
    #else
    PCCInfo.BurnIn          = NULL;
    PCCInfo.FlashRead       = NULL;
    PCCInfo.IsFinish        = NULL;
    #endif

    //#NT#2008/08/14#Irene -begin
    //#NT#CallBack Function for Vendor Request
    // New:
    // 1. Add a callback function for vendor request
    PCCInfo.VendorProcess   = NULL;
    //#NT#2008/08/14#Irene -end

    USBMakerInit_UPCCWrite(&PCCInfo);

    if (UPccOpen(&PCCInfo) != E_OK)
    {
        debug_err(("Error open USB PCC task\r\n"));
    }
#endif
}

//@}
