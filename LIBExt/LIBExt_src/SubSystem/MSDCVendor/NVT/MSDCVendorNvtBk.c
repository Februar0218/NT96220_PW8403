#include "OSCommon.h"
#include "MSDCVendor.h"
#include "MSDCVendorNvtInt.h"
#include "Utility.h"
#include <string.h>


void MsdcVendorNvt_MemPushHostToBk(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &pCtrl->tBkCtrl;

    pBk->nTransmitSize = pHost->LB_Length;
    memcpy(pBk->pPoolMem,pHost->pPoolMem,pBk->nTransmitSize);
}

void MsdcVendorNvt_MemPopBkToHost(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &pCtrl->tBkCtrl;
    memcpy(pHost->pPoolMem,pBk->pPoolMem,pBk->nTransmitSize);
}

BOOL MsdcVendorNvt_Bk_RunCmd(void (*pCall)(void))
{    
    INT32 nRetry = 10;
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &MsdcVendorNvt_GetCtrl()->tBkCtrl;

    if(pBk->bCmdRunning)
    {
        debug_err(("MsdcVendorNvt_Bk_RunCmd(): Is Running\r\n"));
        return FALSE;
    }
    
    pBk->bCmdRunning = TRUE;
    pBk->pCall = pCall;
    while(sta_tsk(pBk->TaskID)!=E_OK && nRetry-- && pBk->bCmdRunning)
    {
        TimerDelayMs(10);
    }

    if(nRetry<0)
        return FALSE;

    return TRUE;
}

BOOL MsdcVendorNvt_Bk_IsFinish(void)
{
    if(MsdcVendorNvt_GetCtrl()->tBkCtrl.bCmdRunning)
        return FALSE;
        
    return TRUE;    
}

BOOL MsdcVendorNvt_Bk_HostLock(void)
{
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &MsdcVendorNvt_GetCtrl()->tBkCtrl;
    
    if(pBk->bServiceLock)
    {
        debug_err(("^RMsdcVendorNvt_Bk_HostLock: already locked!\r\n"));
        return FALSE;
    }

    pBk->bServiceLock = TRUE;
    return TRUE;
}

BOOL MsdcVendorNvt_Bk_HostUnLock(void)
{
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &MsdcVendorNvt_GetCtrl()->tBkCtrl;

    if(!pBk->bServiceLock)
    {
        debug_err(("^RMsdcVendorNvt_Bk_HostUnLock: not locked!\r\n"));
        return FALSE;
    }
    else if(pBk->bCmdRunning)
    {
        debug_err(("^RMsdcVendorNvt_Bk_HostUnLock Bk is Running\r\n"));
        return FALSE;
    }

    pBk->bServiceLock = FALSE;
    return TRUE;
}

BOOL MsdcVendorNvt_Bk_HostIsLock(void)
{
    return MsdcVendorNvt_GetCtrl()->tBkCtrl.bServiceLock;
}

void MsdcVendorNvtTsk(void)
{
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &MsdcVendorNvt_GetCtrl()->tBkCtrl;

    if(pBk->pCall)
        pBk->pCall();
        
    pBk->bCmdRunning = FALSE;
    ext_tsk();
}
