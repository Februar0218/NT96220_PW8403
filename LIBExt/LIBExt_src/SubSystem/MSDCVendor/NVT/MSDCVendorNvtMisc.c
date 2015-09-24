#include "OSCommon.h"
#include "MSDCVendor.h"
#include "MSDCVendorNvtInt.h"
#include "FileSysTsk.h"
#include "nand.h"
#include "wdt.h"
#include "cache.h"
#include <string.h>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
            ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) |   \
            ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

#define CFG_MSDC_VENDOR_NVT_VERSION  0x09090100 //Year:Month:Day:Hour
#define CFG_MSDC_VENDOR_NVT_TAG      MAKEFOURCC('N','O','V','A')

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_MISC Message
//------------------------------------------------------------------------------
#define CDB_11_MISC_UNKNOWN                     0x00 //!< Invalid Cmd
#define CDB_11_MISC_VERSION                     0x01 //!< Get Version
#define CDB_11_MISC_NOVATAG                     0x02 //!< Get Tag
#define CDB_11_MISC_BK_IS_LOCK                  0x03 //!< Query Background is Lock
#define CDB_11_MISC_BK_IS_FINISH                0x04 //!< Query Background is finish his work
#define CDB_11_MISC_BK_LOCK                     0x05 //!< Lock Background Service by Host
#define CDB_11_MISC_BK_UNLOCK                   0x06 //!< UnLock Background Service by Host
#define CDB_11_MISC_UPD_FW_GET_INFO             0x07 //!< Update Firmware (PC Get Info)
#define CDB_11_MISC_UPD_FW_SET_BLOCK            0x08 //!< Update Firmware (PC Set Block Data)

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_MISC Data Type
//------------------------------------------------------------------------------
//Parent for Host <- Device
typedef struct _MISC_RESPONSE{
    UINT32 biSize;      //!< Structure Size
    UINT32 bHandled;    //!< Indicate Device Handle this function (Don't Use BOOL, for 433 Compatiable)
    UINT32 bOK;         //!< Indicate Function Action is OK or Not (Don't Use BOOL, for 433 Compatiable)
    UINT8  Reversed[2]; //!< Reversed Value for Structure DWORD Alignment
}tMISC_RESPONSE;

//Parent for Host -> PC
typedef struct _MISC_DELIVER{
    UINT32 biSize;      //!< Structure Size
}tMISC_DELIVER;

//PC Get Version or NovaTag
typedef struct _MISC_GET_UINT32{
    tMISC_RESPONSE tResponse;   //!< Parent
    UINT32         Value;       //!< UINT32 Value
}tMISC_GET_UINT32;

//PC Get Nand Block Information
typedef struct _MISC_UPDFW_GET_BLOCK_INFO{
    tMISC_RESPONSE tResponse;   //!< Parent
    UINT32 nBytesPerBlock;      //!< Each Block Size
    UINT32 nBytesTempBuf;       //!< Transmit Swap Buffer Size (inc structure size)
}tMISC_UPDFW_GET_BLOCK_INFO;

//PC Send Nand Block Data
typedef struct _MISC_UPDFW_SET_BLOCK_INFO{
    tMISC_DELIVER tDeliver;     //!< Parent
    UINT32 iBlock;              //!< Block Index of this Packet
    UINT32 IsLastBlock;         //!< Indicate Last Block for Reset System
    UINT32 EffectDataSize;      //!< Effective Data Size
}t_MISC_UPDFW_SET_BLOCK_INFO;

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_MISC Handler
//------------------------------------------------------------------------------
static void Misc_Get_Version(void* pInfo);          //!< Get MSDC Vendor Nvt Version
static void Misc_Get_NovaTag(void* pInfo);          //!< Get 'N','O','V','A' Tag
static void Misc_Get_Bk_IsLock(void* pInfo);        //!< Host Query Background Service is lock
static void Misc_Get_Bk_IsFinish(void* pInfo);      //!< Host Query Background Service is Finish his work
static void Misc_Get_Bk_Lock(void* pInfo);          //!< Host Lock Background Service
static void Misc_Get_Bk_UnLock(void* pInfo);        //!< Host UnLock Background Service
static void Misc_Set_UpdFw_GetInfo(void* pInfo);    //!< Update Firmware (PC Get Info)
static void Misc_Set_UpdFw_SetBlock(void* pInfo);   //!< Update Firmware (PC Set Block Data)

//Local Variables
static tMSDCVENDOR_FUNCTION_CMD_HANDLE CallMap[] = {
    {CDB_11_MISC_UNKNOWN,NULL},
    {CDB_11_MISC_VERSION,           Misc_Get_Version},
    {CDB_11_MISC_NOVATAG,           Misc_Get_NovaTag},
    {CDB_11_MISC_BK_IS_LOCK,        Misc_Get_Bk_IsLock},
    {CDB_11_MISC_BK_IS_FINISH,      Misc_Get_Bk_IsFinish},
    {CDB_11_MISC_BK_LOCK,           Misc_Get_Bk_Lock},
    {CDB_11_MISC_BK_UNLOCK,         Misc_Get_Bk_UnLock},
    {CDB_11_MISC_UPD_FW_GET_INFO,   Misc_Set_UpdFw_GetInfo},
    {CDB_11_MISC_UPD_FW_SET_BLOCK,  Misc_Set_UpdFw_SetBlock},
};

void MsdcVendorNvt_Function_Misc(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    UINT8 iCmd = pHost->pCBW->CBWCB[11];
    void* pInfo = pHost->pPoolMem;

    if(iCmd<sizeof(CallMap)/sizeof(tMSDCVENDOR_FUNCTION_CMD_HANDLE)
        && iCmd == CallMap[iCmd].iCall
        && CallMap[iCmd].pCall!=NULL)
    {
        CallMap[iCmd].pCall(pInfo);
    }
    else
    {
        debug_err(("MsdcVendorNvt_Function_Utility: Call Map Wrong!\r\n"));
    }
}

static void Misc_Get_Version(void* pInfo)
{
    tMISC_GET_UINT32* pValue = (tMISC_GET_UINT32*)pInfo;
    pValue->tResponse.biSize = sizeof(tMISC_GET_UINT32);
    pValue->tResponse.bHandled = TRUE;

    pValue->Value = CFG_MSDC_VENDOR_NVT_VERSION;

    pValue->tResponse.bOK = TRUE;
}

static void Misc_Get_NovaTag(void* pInfo)
{
    UINT32 *pTag = (UINT32*)pInfo;
    *pTag = CFG_MSDC_VENDOR_NVT_TAG;
}

static void Misc_Get_Bk_IsLock(void* pInfo)
{
    tMISC_RESPONSE* pResponse = (tMISC_RESPONSE*)pInfo;

    memset(pResponse,0,sizeof(tMISC_RESPONSE));
    pResponse->biSize = sizeof(tMISC_RESPONSE);
    pResponse->bHandled = TRUE;
    pResponse->bOK = MsdcVendorNvt_Bk_HostIsLock();
}

static void Misc_Get_Bk_IsFinish(void* pInfo)
{
    tMISC_RESPONSE* pResponse = (tMISC_RESPONSE*)pInfo;

    memset(pResponse,0,sizeof(tMISC_RESPONSE));
    pResponse->biSize = sizeof(tMISC_RESPONSE);
    pResponse->bHandled = TRUE;
    pResponse->bOK = MsdcVendorNvt_Bk_IsFinish();
}

static void Misc_Get_Bk_Lock(void* pInfo)
{
    tMISC_RESPONSE* pResponse = (tMISC_RESPONSE*)pInfo;

    memset(pResponse,0,sizeof(tMISC_RESPONSE));
    pResponse->biSize = sizeof(tMISC_RESPONSE);
    pResponse->bHandled = TRUE;
    pResponse->bOK = MsdcVendorNvt_Bk_HostLock();
}

static void Misc_Get_Bk_UnLock(void* pInfo)
{
    tMISC_RESPONSE* pResponse = (tMISC_RESPONSE*)pInfo;

    memset(pResponse,0,sizeof(tMISC_RESPONSE));
    pResponse->biSize = sizeof(tMISC_RESPONSE);
    pResponse->bHandled = TRUE;
    pResponse->bOK = MsdcVendorNvt_Bk_HostUnLock();
}

//#NT#2011/08/11#Niven Cho -begin
//#NT#Remove Nand Related
static void Misc_Set_UpdFw_GetInfo(void* pInfo)
{
#if 0
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMISC_UPDFW_GET_BLOCK_INFO* pBlkInfo = (tMISC_UPDFW_GET_BLOCK_INFO*)pInfo;

    pBlkInfo->tResponse.biSize = sizeof(tMISC_UPDFW_GET_BLOCK_INFO);
    pBlkInfo->tResponse.bHandled = TRUE;

    pBlkInfo->nBytesTempBuf = pHost->nPoolSize;
    pBlkInfo->nBytesPerBlock = nand_getBlockSize();

    pBlkInfo->tResponse.bOK = TRUE;
#endif
}

static void Misc_Set_UpdFw_SetBlock(void* pInfo)
{
#if 0
    static UINT32 nCntBadBlock = 0;
    t_MISC_UPDFW_SET_BLOCK_INFO* pBlkInfo = (t_MISC_UPDFW_SET_BLOCK_INFO*)pInfo;
    UINT8* pData = (UINT8*)pInfo + sizeof(t_MISC_UPDFW_SET_BLOCK_INFO);
    UINT32 iBlkFwBegin = nand_getReservedStartBlockIndex();
    STRG_TAB      *pNand = nand_getStorageObject();

    if(pBlkInfo->tDeliver.biSize!=sizeof(t_MISC_UPDFW_SET_BLOCK_INFO))
    {
        debug_err(("Misc_Set_UpdFw_SetBlock(): Wrong Data Size!\r\n"));
        return;
    }

    //Reset bad block counter, when we write first f/w block
    if(pBlkInfo->iBlock == 0)
    {
        PFLGPTN uiFlag;

        debug_msg("Start to Update F/W via USB, Force PStore Lock and Filesys Pause!\r\n");
        nCntBadBlock = 0;
        wai_flg((PFLGPTN)&uiFlag, FLG_ID_PSTORE, 0x1, TWF_CLR|TWF_ORW);
        FilesysPause();
        FilesysWaitPaused();
    }

    pNand->Open();
    //don't need to dmaflush here, due to USB driver handled this.
    while(nand_writeOneReservedBlock(iBlkFwBegin+pBlkInfo->iBlock+nCntBadBlock,iBlkFwBegin+pBlkInfo->iBlock,pData)!=E_OK)
    {
        nCntBadBlock++;
    }
    pNand->Close();

    if(pBlkInfo->IsLastBlock)
    {
        //to avoid wdt already in use
        wdt_close();
        //setting wdt start
        wdt_open();
        wdt_setRstEn(TRUE);
        wdt_setMS4B(0);
        wdt_setTrig();
        wdt_setEnable();
        //wait to system reset
        loc_cpu();
        while(1)
        {
            _ASM_NOP
        }
    }
#endif
}
//#NT#2011/08/11#Niven Cho -end