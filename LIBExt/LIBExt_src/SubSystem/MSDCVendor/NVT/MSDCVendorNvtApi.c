#include "OSCommon.h"
#include "MSDCVendor.h"
#include "MSDCVendorNvtInt.h"
#include "uart.h"
#include <string.h>

//Local Functions
static BOOL MsdcVendorNvt_Function(void);
static BOOL CalcAccessAddressWithSize(void);
static void MsdcVendorNvt_RegisterAccess(UINT32* pDst,UINT32* pSrc,UINT32 nDWORDs);

//Local Variable
static tMSDCVENDOR_CTRL gCtrl = {0};

//Internal APIs
tMSDCVENDOR_CTRL* MsdcVendorNvt_GetCtrl(void)
{
    return &gCtrl;
}

//APIs
BOOL MSDCVendor_Verify_CB(UINT32 pCmdBuf, UINT32 *pDataBuf) //!< Callback for USB_MSDC_INFO.msdc_check_cb
{
    BOOL bOK = TRUE;
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;

    //Check Internal Status
    if(!pCtrl->bInit)
    {
        debug_err(("MSDCVendor_Verify_CB(): Not Initial\r\n"));
        return FALSE;
    }

    pHost->pCBW = (tMSDCVENDOR_CBW*)pCmdBuf;
    pHost->iCmd = pHost->pCBW->CBWCB[0];

    //Check Novatek Tag
    if(pHost->pCBW->CBWCB[1]!=CDB_01_NVT_TAG)
        return FALSE;
    else if(!CalcAccessAddressWithSize())
        return FALSE;
    else if(pCtrl->tHostInfo.LB_Length > pCtrl->tHostInfo.nPoolSize)
    {
        debug_err(("MSDCVendor_Verify_CB(): data exceed %d Bytes\r\n",pCtrl->tHostInfo.nPoolSize));
        return FALSE;
    }


    switch(pHost->iCmd)
    {
        case SBC_CMD_MSDCVENDOR_READ:
        case SBC_CMD_MSDCVENDOR_WRITE:
            if(pHost->LB_Address > CFG_REGISTER_BEGIN_ADDR)
                *pDataBuf = (UINT32)pHost->pPoolMem; //Register
            else
                *pDataBuf = pHost->LB_Address;       //DRAM
            break;
        case SBC_CMD_MSDCVENDOR_FUNCTION:
            *pDataBuf = (UINT32)pHost->pPoolMem;
            break;
        default:
            return FALSE;
    }
    USBMSDC_ChgVenInBufAddr(*pDataBuf);

    return bOK;
}

void MSDCVendor_Vendor_CB(PMSDCVendorParam pBuf) //!< Callback for USB_MSDC_INFO.msdc_vendor_cb
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;

    pHost->pCBW = (tMSDCVENDOR_CBW*)pBuf->VendorCmdBuf;
    pHost->pCSW = (tMSDCVENDOR_CSW*)pBuf->VendorCSWBuf;

    switch(pHost->iCmd)
    {
        case SBC_CMD_MSDCVENDOR_READ:
            if(pHost->LB_Address > CFG_REGISTER_BEGIN_ADDR) //Register
                MsdcVendorNvt_RegisterAccess((UINT32*)pHost->pPoolMem,(UINT32*)pHost->LB_Address,pHost->LB_Length>>2);
            else //DRAM
                pBuf->InDataBuf = pHost->LB_Address;
            break;
        case SBC_CMD_MSDCVENDOR_WRITE:
            if(pHost->LB_Address > CFG_REGISTER_BEGIN_ADDR) //Register
                MsdcVendorNvt_RegisterAccess((UINT32*)pHost->LB_Address,(UINT32*)pHost->pPoolMem,pHost->LB_Length>>2);
            else //DRAM
                pBuf->InDataBuf = pHost->LB_Address;
            break;
        case SBC_CMD_MSDCVENDOR_FUNCTION:
            MsdcVendorNvt_Function();
            pBuf->InDataBuf = (UINT32)pHost->pPoolMem;
            break;
        default:
            debug_err(("MSDCVendor_Vendor_CB(): unknown command id: %d\r\n",pHost->iCmd));
            return;
    }

    pBuf->InDataBufLen = pBuf->OutDataBufLen = pHost->LB_Length;
}

//!< for Register Callback Function
BOOL MSDCVendorNVT_AddCallback_Bi(char* szName,void (*fp)(void* pData))
{
    tMSDCVENDOR_BI_CALL_CTRL* pBiCall = &MsdcVendorNvt_GetCtrl()->tBiCall;
    UINT32 iCall = pBiCall->n_Callback;
    UINT32 n_Callback = sizeof(pBiCall->tTblCallback)/sizeof(tMSDCVENDOR_BI_CALL_HANDLE);

    //check is table full?
    if(pBiCall->n_Callback>=n_Callback)
    {
        debug_err(("MSDCVendorNVT_AddCallback_Bi(): registered callbacks is full! %s (%d)\r\n",szName,n_Callback));
        return FALSE;
    }

    if(strlen(szName)>pBiCall->nMaxNameSize)
        debug_err(("MSDCVendorNVT_AddCallback_Bi(): strlen(szName) < %d bytes\r\n",pBiCall->nMaxNameSize));

    memcpy(pBiCall->tTblCallback[iCall].szName,szName,sizeof(pBiCall->tTblCallback[iCall].szName));
    pBiCall->tTblCallback[iCall].fp = fp;
    pBiCall->n_Callback++;

    return TRUE;
}

BOOL MSDCVendorNVT_AddCallback_Si(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets)
{
    tMSDCVENDOR_SI_CALL_CTRL* pSiCtrl = &MsdcVendorNvt_GetCtrl()->tSiCall;
    pSiCtrl->fpTblGet = fpTblGet;
    pSiCtrl->nGets = nGets;
    pSiCtrl->fpTblSet = fpTblSet;
    pSiCtrl->nSets = nSets;

    return TRUE;
}

UINT8* MSDCVendorNVT_GetDataBufferAddress(void)
{
    return MsdcVendorNvt_GetCtrl()->tBkCtrl.pPoolMem;
}

UINT32 MSDCVendorNVT_GetDataBufferSize(void)
{
    return MsdcVendorNvt_GetCtrl()->tBkCtrl.nPoolSize;
}

UINT32 MSDCVendorNVT_GetTransSize(void)
{
    return MsdcVendorNvt_GetCtrl()->tBkCtrl.nTransmitSize;
}

BOOL MSDCVendorNVT_Configure(tMSDCVENDOR_CFG* pCfg)
{
    INT32 i;
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_BI_CALL_CTRL* pBiCall = &pCtrl->tBiCall;
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &pCtrl->tBkCtrl;
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &pCtrl->tDbgSys;
    UINT8* pMemPool = pCfg->pMemCache;
    UINT32 nMemSize = pCfg->nSizeCache;

    memset(pCtrl,0,sizeof(tMSDCVENDOR_CTRL));

    //Check Valid
    if(pCfg->nSizeCache<CFG_MIN_WORKING_BUF_SIZE)
    {
        debug_err(("MSDCVendorNVT_Configure(): nSizeCache minimum need %d bytes\r\n",CFG_MIN_WORKING_BUF_SIZE));
        return FALSE;
    }

    //Set Host Ctrl
    pHost->pPoolMem = pMemPool;
    pHost->nPoolSize = CFG_MIN_HOST_MEM_SIZE;
    pMemPool += pHost->nPoolSize;
    nMemSize -= pHost->nPoolSize;
    //Set Bi-Direction Call
    pBiCall->nMaxNameSize = sizeof(pBiCall->tTblCallback[0].szName) - 1;
    //Set Background Ctrl
    pBk->TaskID = pCfg->TaskID;
    pBk->pPoolMem = pMemPool;
    pBk->nPoolSize = CFG_MIN_BACKGROUND_MEM_SIZE;
    pMemPool += pBk->nPoolSize;
    nMemSize -= pBk->nPoolSize;

    if(pCfg->fpRegHookDbgMsg && pCfg->SemID && pCfg->TaskID)
    {
        //Set DbgSys
        pDbgSys->SemID = pCfg->SemID;
        pDbgSys->MsgCntMask   = CFG_DBGSYS_MSG_PAYLOAD_NUM-1;
        pDbgSys->PayloadNum   = CFG_DBGSYS_MSG_PAYLOAD_NUM;
        pDbgSys->PayloadSize  = nMemSize / CFG_DBGSYS_MSG_PAYLOAD_NUM;
        pDbgSys->fpDoCommand  = pCfg->fpDoCommand;
        pDbgSys->fpRegHookDbgMsg = pCfg->fpRegHookDbgMsg;
        pDbgSys->fpUartPutString = pCfg->fpUartPutString;
        if(pDbgSys->fpUartPutString==NULL)
            pDbgSys->fpUartPutString = uart_putString;

        for(i=0;i<CFG_DBGSYS_MSG_PAYLOAD_NUM;i++)
        {
            tMSDCVENDOR_DBGSYS_UNIT* pUnit = &pDbgSys->Queue[i];
            pUnit->pMsg = pMemPool;
            pMemPool += pDbgSys->PayloadSize;
            nMemSize -= pDbgSys->PayloadSize;
        }

        pDbgSys->bInit = TRUE;
    }

    pCtrl->bInit = TRUE;
    return TRUE;
}

static BOOL CalcAccessAddressWithSize(void)
{
    UINT   CDB_cmdid;
    UINT   CDB_DataXferLen,CBW_DataXferLen;
    tMSDCVENDOR_HOST_INFO* pHost = &MsdcVendorNvt_GetCtrl()->tHostInfo;

    CDB_cmdid = pHost->pCBW->CBWCB[0];
    CBW_DataXferLen = pHost->pCBW->dCBWDataTransferLength;

    pHost->LB_Address =  pHost->pCBW->CBWCB[2] << 24;
    pHost->LB_Address |= pHost->pCBW->CBWCB[3] << 16;
    pHost->LB_Address |= pHost->pCBW->CBWCB[4] << 8;
    pHost->LB_Address |= pHost->pCBW->CBWCB[5];

    pHost->LB_Length =  pHost->pCBW->CBWCB[6] << 24;
    pHost->LB_Length |= pHost->pCBW->CBWCB[7] << 16;
    pHost->LB_Length |= pHost->pCBW->CBWCB[8] << 8;
    pHost->LB_Length |= pHost->pCBW->CBWCB[9];

    CDB_DataXferLen = pHost->LB_Length;

    //Check DWORD Alignment with Starting Address
    if(pHost->LB_Address&0x3)
    {
        return FALSE;
    }
    //Case(1) : Hn = Dn
    else if((CBW_DataXferLen == 0) && (CDB_DataXferLen == 0))
    {
        return TRUE;
    }
    // case(2): Hn < Di , case(3): Hn < Do
    else if ((CBW_DataXferLen == 0) && (CDB_DataXferLen > 0))
    {
        return FALSE;
    }
    // case(8): Hi <> Do
    else if((CDB_cmdid==SBC_CMD_MSDCVENDOR_WRITE) && ((pHost->pCBW->bmCBWFlags & SBC_CMD_DIR_MASK) != SBC_CMD_DIR_OUT ))
    {
        return FALSE;
    }
    // case(10): Ho <> Di
    else if((CDB_cmdid==SBC_CMD_MSDCVENDOR_READ)  && ((pHost->pCBW->bmCBWFlags & SBC_CMD_DIR_MASK) != SBC_CMD_DIR_IN))
    {
        return FALSE;
    }
    // case(4): Hi>Dn, case(5): hi>Di, case(11): Ho>Do
    else if (CBW_DataXferLen > CDB_DataXferLen)
    {
        return FALSE;
    }
    // case(7): Hi<Di, case(13): Ho < Do
    else if (CBW_DataXferLen < CDB_DataXferLen)
    {
        return FALSE;
    }

    return TRUE;
}

static void MsdcVendorNvt_RegisterAccess(UINT32* pDst,UINT32* pSrc,UINT32 nDWORDs)
{
    while(nDWORDs--)
        *(pDst++) = *(pSrc++);
}

static BOOL MsdcVendorNvt_Function(void)
{
    tMSDCVENDOR_HOST_INFO* pHost = &MsdcVendorNvt_GetCtrl()->tHostInfo;

    switch(pHost->pCBW->CBWCB[10])
    {
        case CDB_10_FUNCTION_DBGSYS: //!< DBGSYS Needs to High Priority
            MsdcVendorNvt_Function_DbgSys();
            break;
        case CDB_10_FUNCTION_BI_CALL:
            MsdcVendorNvt_Function_BiCall();
            break;
        case CDB_10_FUNCTION_SI_CALL:
            MsdcVendorNvt_Function_SiCall();
            break;
        case CDB_10_FUNCTION_MISC:
            MsdcVendorNvt_Function_Misc();
            break;
        default:
            debug_err(("MsdcVendorNvt_Function(): Not Handled ID: %d\r\n",pHost->pCBW->CBWCB[10]));
    }
    return TRUE;
}
