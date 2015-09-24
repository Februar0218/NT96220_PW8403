/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitUSB.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for USB mode
                Get memory and set parameters for USB mode

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include <stdio.h>
#include <string.h>
#include "AppInitUSB.h"
#include "USBMakerParamInit.h"
#include "GlobalVar.h"
#include "PrimaryTsk.h"
#include "PlaybackTsk.h"
#include "PhotoTsk.h"
#include "SieCapTsk.h"
#include "UIFlow.h"
#include "KeyScanTsk.h"
#include "RamDisk.h"
#include "GPIOMapping.h"
#include "NAND.h"
//#NT#2009/03/24#Marx Chiu -begin
#include "DPS_Header_Public.h"
#include "PTP_API.h"
//#NT#2009/03/24#Marx Chiu -end
#include "UsbCustomVendorCmd.h"
//#NT#2011/04/29#Ethan Lau -begin
//#NT#registor ae/awb
#include "ae_api.h"
#include "awb_api.h"
//#NT#2011/04/29#Ethan Lau -end

//#NT#2009/07/13#Chris Chung -begin
//#NT#add to control PCC vendor function
static BOOL g_bAppInitUSBPCCVendorFunc = FALSE;
//#NT#2009/07/13#Chris Chung -end

/**
  Initialize application for USB MSDC mode

  Initialize application for USB MSDC mode.

  @param void
  @return void
*/
void MSDC_LED_CB(void)
{
}

void AppInit_ModeUSBMSDC(void)
{
    USB_MSDC_INFO       MSDCInfo;
    UINT32              uiPoolAddr, i;
#if ((_MSDC_MODE_ == _MSDC_SINGLE_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND3_))
    PrimaryStorageID    uiStgType;
    PSTRG_TAB           pStrgObj;
#endif

    memset((void*)&MSDCInfo, 0, sizeof(USB_MSDC_INFO));

    // Get buffer memory for MSDC task, MSDC share the same buffer memory with SICD,
    // make sure the buffer memory size is enough for both
    SysGetSICDAddr(&uiPoolAddr);

    MSDCInfo.uiMsdcBufAddr = uiPoolAddr;
    MSDCInfo.uiMsdcBufSize = POOL_SIZE_SICD;
    USBMakerInit_UMSD(&MSDCInfo);

    //The callback functions for the MSDC Vendor command.
    //If project doesn't need the MSDC Vendor command, set this callback function as NULL.
    MSDCInfo.msdc_check_cb = NULL;
    // Vendor command callback function
    MSDCInfo.msdc_vendor_cb = NULL;
    // Read / Write MSDC callback function
    MSDCInfo.msdc_RW_Led_CB = MSDC_LED_CB;
    // Host issue StopUnit command callback function
    MSDCInfo.msdc_StopUnit_Led_CB = NULL;
    // Host issue USB suspend
    MSDCInfo.msdc_UsbSuspend_Led_CB = NULL;

    // Set Default Luns as Storage
    for(i=0;i<MAX_LUN;i++)
        MSDCInfo.msdc_type[i] = MSDC_STRG;


#if ((_MSDC_MODE_ == _MSDC_SINGLE_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_)||(_MSDC_MODE_ == _MSDC_SINGLE_NAND3_))

    //Get the idChangeToStorage to distinguish bad FAT from bad card
    uiStgType = Primary_GetChangeStorage();
    pStrgObj = Primary_GetStorageObj(uiStgType);

    MSDCInfo.pStrgDevObj[0] = pStrgObj;

    if(uiStgType == PRIMARY_STGTYPE_NAND)
    {
        MSDCInfo.msdc_storage_detCB[0] = NULL;
        MSDCInfo.msdc_strgLock_detCB[0] = NULL;
    }
    else if(uiStgType == PRIMARY_STGTYPE_CARD)
    {
        MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)GPIOMap_DetStrgCard;
        MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)GPIOMap_DetStrgCardWP;
    }

    #if (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_)
        MSDCInfo.pStrgDevObj[1] = nand2_getStorageObject();
        MSDCInfo.msdc_storage_detCB[1] = NULL;
        MSDCInfo.msdc_strgLock_detCB[1] = NULL;

        MSDCInfo.LUNs = 2;
        MSDCInfo.msdc_type[0] = MSDC_STRG;
        MSDCInfo.msdc_type[1] = MSDC_CDROM;
    #elif (_MSDC_MODE_ == _MSDC_SINGLE_NAND3_)
        MSDCInfo.pStrgDevObj[1] = nand3_getStorageObject();
        MSDCInfo.msdc_storage_detCB[1] = NULL;
        MSDCInfo.msdc_strgLock_detCB[1] = NULL;

        MSDCInfo.LUNs = 2;
        MSDCInfo.msdc_type[0] = MSDC_STRG;
        MSDCInfo.msdc_type[1] = MSDC_CDROM;
    #else
        MSDCInfo.LUNs = 1;
        MSDCInfo.msdc_type[0] = MSDC_STRG;
    #endif

#elif ((_MSDC_MODE_ == _MSDC_ALL_) || (_MSDC_MODE_ == _MSDC_ALL_NAND2_) || (_MSDC_MODE_ == _MSDC_ALL_NAND3_))

    MSDCInfo.pStrgDevObj[0] = Primary_GetStorageObj(PRIMARY_STGTYPE_CARD);

        #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
        MSDCInfo.pStrgDevObj[1] = Primary_GetStorageObj(PRIMARY_STGTYPE_NAND);
    #elif (_RAM_DISK_ == ENABLE)
        MSDCInfo.pStrgDevObj[1] = ramdsk_getStorageObject();
        #else
        MSDCInfo.pStrgDevObj[1] = NULL;
        #endif

    MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)GPIOMap_DetStrgCard;
    MSDCInfo.msdc_storage_detCB[1] = NULL;

    MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)GPIOMap_DetStrgCardWP;
    MSDCInfo.msdc_strgLock_detCB[1] = NULL;

    #if (_MSDC_MODE_ == _MSDC_ALL_NAND2_)

        MSDCInfo.pStrgDevObj[2] = nand2_getStorageObject();
        MSDCInfo.msdc_storage_detCB[2] = NULL;
        MSDCInfo.msdc_strgLock_detCB[2] = NULL;

        MSDCInfo.LUNs = 3;
        MSDCInfo.msdc_type[0] = MSDC_STRG;
        MSDCInfo.msdc_type[1] = MSDC_STRG;
        MSDCInfo.msdc_type[2] = MSDC_CDROM;

    #elif (_MSDC_MODE_ == _MSDC_ALL_NAND3_)

        MSDCInfo.pStrgDevObj[2] = nand2_getStorageObject();
        MSDCInfo.msdc_storage_detCB[2] = NULL;
        MSDCInfo.msdc_strgLock_detCB[2] = NULL;
        MSDCInfo.pStrgDevObj[3] = nand3_getStorageObject();
        MSDCInfo.msdc_storage_detCB[3] = NULL;
        MSDCInfo.msdc_strgLock_detCB[3] = NULL;

        MSDCInfo.LUNs = 4;
        MSDCInfo.msdc_type[0] = MSDC_STRG;
        MSDCInfo.msdc_type[1] = MSDC_STRG;
        MSDCInfo.msdc_type[2] = MSDC_STRG;
        MSDCInfo.msdc_type[3] = MSDC_CDROM;

    #else

        #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
        MSDCInfo.LUNs = 2;
        MSDCInfo.msdc_type[0] = MSDC_STRG;
        MSDCInfo.msdc_type[1] = MSDC_STRG;
        #elif (_RAM_DISK_ == ENABLE)
            MSDCInfo.LUNs = 2;
            MSDCInfo.msdc_type[0] = MSDC_STRG;
            MSDCInfo.msdc_type[1] = MSDC_STRG;
        #else
        MSDCInfo.LUNs = 1;
        MSDCInfo.msdc_type[0] = MSDC_STRG;
        #endif

    #endif

#endif

    // Open MSDC task
    if (USBMSDC_Open(&MSDCInfo) != E_OK)
    {
        debug_err(("Error open USB MSDC task\r\n"));
    }
}

/**
  Initialize application for USB SICD mode

  Initialize application for USB SICD mode.

  @param void
  @return void
*/
void AppInit_ModeUSBSICD(void)
{
    USB_SICD_INFO       SICDInfo;
    UINT32              uiPoolAddr, uiPoolAddr2;
    PrimaryStorageID    uiStgType;
    PSTRG_TAB           pStrgObj;

    memset((void*)&SICDInfo, 0, sizeof(USB_SICD_INFO));

    // Get memory for SICD task
    SysGetSICDAddr(&uiPoolAddr);
    SysGetPTPObjectAddr(&uiPoolAddr2);

    PTP_setSendObjBufAddrSize(uiPoolAddr2, POOL_SIZE_PTP_OBJECT);

    USBMakerInit_USICD(&SICDInfo);
    PTP_Init(uiPoolAddr,POOL_SIZE_SICD);

    // Query current active storage
    uiStgType = Primary_GetStorage();
    pStrgObj = Primary_GetStorageObj(uiStgType);

    // Check card lock status, only storage card has this status, where NAND flash does not
    if (uiStgType == PRIMARY_STGTYPE_CARD)
    {
        SICDInfo.uiStrgCardLock = KeyScan_IsStrgCardLocked();
    }
    else if (uiStgType == PRIMARY_STGTYPE_NAND)
    {
        SICDInfo.uiStrgCardLock = FALSE;
    }

    //#NT#2009/03/24#Marx Chiu -begin
    SICDInfo.fpCommandProcess = PTP_CommandProcess;
    SICDInfo.fpStopTransfer   = PTP_StopTransfer;
    SICDInfo.fpDeviceReset    = PTP_ResetDevice;
    //#NT#2009/03/24#Marx Chiu -end

    // Open SICD task
    //#NT#2009/03/24#Marx Chiu -begin
    //MarxChiu_20090324 if (usbsicd_open(pStrgObj, &SICDInfo) != E_OK)
    if (Usicd_Open(pStrgObj, &SICDInfo) != E_OK)
    //#NT#2009/03/24#Marx Chiu -end
    {
        debug_err(("Error open USB SICD task\r\n"));
    }

    //#NT#2009/03/24#Marx Chiu -begin
    DPS_Init();
    //#NT#2009/03/24#Marx Chiu -end
}

/**
  Initialize application for USB PCC mode

  Initialize application for USB PCC mode.

  @param void
  @return void
*/
void AppInit_ModeUSBPCC(void)
{
    USB_PCC_INFO    PCCInfo = {0};
    UINT32          uiPoolAddr;
    PHOTO_APPOBJ    PhotoObj = {0};

    //memset((void*)&PCCInfo, 0, sizeof(USB_PCC_INFO));

    // Get memory for Photo task
    get_blk((VP *)&uiPoolAddr, POOL_ID_FB);
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);

    PhotoObj.uiMemoryAddr       = uiPoolAddr;
#if (_PCC_720P_FUNC_ == ENABLE)
    //                            SIE*2          IME(YUV422)*3    CA
    PhotoObj.uiMemorySize       = (1280*960*2) + (1280*960*2*3) + 1280*960/4;
    PhotoObj.uiDisplayFBWidth   = 1280;
    PhotoObj.uiDisplayFBHeight  = 960;
#else
    PhotoObj.uiMemorySize       = POOL_SIZE_FB;
    PhotoObj.uiDisplayFBWidth   = 640;
    PhotoObj.uiDisplayFBHeight  = 480;
#endif
    PhotoObj.CallBackFunc       = (FPALGCALLBACK)FlowPhoto_ImageCB;

    // Register AE & AWB function pointer
    Photo_RegisterAE((FPPHOTOAE)AE_Process);
    Photo_RegisterAWB((FPPHOTOAWB)AWB_Process);

    // Register (disable) FD/SD function pointer
    Photo_RegisterFD(NULL);
    Photo_RegisterSD(NULL);

    // Open Photo task for AE & AWB
    if (Photo_Open(&PhotoObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo task\r\n"));
    }

    //#NT#20100126#ethanlau -begin
    //add for Image pipeline from pcc 2 capture
    {
        CAPTURE_APPOBJ      CaptureObj;
        // Get memory for Capture task
        get_blk((VP *)&uiPoolAddr, POOL_ID_CAPTURE);
        rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

        CaptureObj.uiMemoryAddr         = uiPoolAddr;
        CaptureObj.uiMemorySize         = POOL_SIZE_CAPTURE - POOL_SIZE_WAV+POOL_SIZE_FB;
        CaptureObj.CallBackFunc         = (FPALGCALLBACK)FlowPhoto_ImageCB;

        // Open Capture task
        if (ImgCapture_Open(&CaptureObj) == E_NOMEM)
        {
            debug_err(("Not enough memory for capture task\r\n"));
        }
    }

    //#NT#20100126#ethanlau -end
    //#NT#20100126#ethanlau -begin
    //remove this , agc is not setted by imgslicetsk anymore
    /*
    // Open ImgSliceTsk for set AGC, Modify by Iyen 20070702
    ImgSliceOpen();
    */
    //#NT#20100126#ethanlau -end

    // Reserved capture buffer for image capturing and PCC use SICD buffer
    // Get memory for PCC task
#if (_PCC_720P_FUNC_ == ENABLE)
    PCCInfo.UsbPccMemAdr    = END_MEM - POOL_SIZE_PCC_HD;
    PCCInfo.UsbPccMemSize   = POOL_SIZE_PCC_HD;
    PCCInfo.PccMaxSize      = PCC_MAX_SIZE_HD;
#else
    PCCInfo.UsbPccMemAdr    = END_MEM - POOL_SIZE_PCC_VGA;
    PCCInfo.UsbPccMemSize   = POOL_SIZE_PCC_VGA;
    PCCInfo.PccMaxSize      = PCC_MAX_SIZE_VGA;
#endif
    PCCInfo.BurnIn          = NULL;
    PCCInfo.FlashRead       = NULL;
    PCCInfo.IsFinish        = NULL;
    //#NT#2009/07/13#Chris Chung -begin
    //#NT#add to control PCC vendor function
    PCCInfo.bEnableVendorFunc = g_bAppInitUSBPCCVendorFunc;
    //#NT#2009/07/13#Chris Chung -end
    //PCCInfo.VendorProcess   = NULL;
    PCCInfo.VendorProcess   = UsbCustomVerdorCmdCB;

    USBMakerInit_UPCC(&PCCInfo);

    if(UPccRegisterVendorCmd(0x80, 0xFF) != E_OK)
    {
        debug_err(("Register vendor command failed ! \r\n"));
    }

    if (UPccOpen(&PCCInfo) != E_OK)
    {
        debug_err(("Error open USB PCC task\r\n"));
    }
}

//#NT#2009/07/13#Chris Chung -begin
//#NT#add to control PCC vendor function
void AppInit_ModeUSBPCCEnableVendorFunc(BOOL en)
{
    g_bAppInitUSBPCCVendorFunc = en;
}
//#NT#2009/07/13#Chris Chung -end

//#NT#2010/02/27#Winston -begin
//#NT#add to control PCC Focus vendor function
BOOL AppInit_ModeUSBPCCGetVendorFunc(void)
{
    return g_bAppInitUSBPCCVendorFunc;
}
//#NT#2010/02/27#Winston -end
//@}
