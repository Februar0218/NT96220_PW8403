#include "OSCommon.h"
#include "MSDCVendor.h"
#include "MSDCVendorNvtInt.h"
#include <string.h>

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_BI_CALL Message
//------------------------------------------------------------------------------
#define CDB_11_BI_CALL_UNKNOWN                  0x00 //!< Invalid Cmd
#define CDB_11_BI_CALL_GET_PROC_STEP0           0x01 //!< Get Function Proc Address (PC Send Function String To F.W)
#define CDB_11_BI_CALL_GET_PROC_STEP1           0x02 //!< Get Function Proc Address (PC Query, Then F.W Return Function Point)
#define CDB_11_BI_CALL_CALL                     0x03 //!< Call Function,
#define CDB_11_BI_CALL_GET_DATA                 0x04 //!< Get  Result Data after Finish Function

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_BI_CALL Handler
//------------------------------------------------------------------------------
static void BiCall_GetProc_Step0(void* pInfo); //!< PC Query Function Name
static void BiCall_GetProc_Step1(void* pInfo); //!< Return Function Point Address to PC
static void BiCall_Call(void* pInfo);          //!< Recevice PC Data & Call Function (Bi-Direction Step 1)
static void BiCall_GetData(void* pInfo);       //!< Send Result Data to PC (Bi-Direction Step 2)
static void BiCall_RunCmd(void);               //!< Callback for Background Thread Run Command

//Local Variables
static tMSDCVENDOR_FUNCTION_CMD_HANDLE CallMap[] = {
    {CDB_11_BI_CALL_UNKNOWN,NULL},
    {CDB_11_BI_CALL_GET_PROC_STEP0,BiCall_GetProc_Step0},
    {CDB_11_BI_CALL_GET_PROC_STEP1,BiCall_GetProc_Step1},
    {CDB_11_BI_CALL_CALL,BiCall_Call},
    {CDB_11_BI_CALL_GET_DATA,BiCall_GetData},
};

void MsdcVendorNvt_Function_BiCall(void)
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
        debug_err(("MsdcVendorNvt_Function_BiCall: Call Map Wrong!\r\n"));
    }
}

static void BiCall_GetProc_Step0(void* pInfo)
{
    MsdcVendorNvt_MemPushHostToBk(); //Push Name to Bk
}

static void BiCall_GetProc_Step1(void* pInfo)
{
    UINT32   i;
    BOOL     bFind = FALSE;
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_BI_CALL_CTRL* pBiCtrl = &pCtrl->tBiCall;
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &pCtrl->tBkCtrl;
    char*    szName = (char*)pBk->pPoolMem;
    UINT32*  pProcAddress = (UINT32*)pHost->pPoolMem;

    for(i=0; i<pBiCtrl->n_Callback; i++)
    {
        if(strncmp(szName,pBiCtrl->tTblCallback[i].szName,pBiCtrl->nMaxNameSize)==0)
        {
            bFind = TRUE;
            *pProcAddress = (UINT32)pBiCtrl->tTblCallback[i].fp;
            break;
        }
    }

    if(!bFind)
    {
        debug_err(("BiCall_GetProc_Step1(): No Found: %s\r\n",szName));
        *pProcAddress = 0;
    }
    return;
}

static void BiCall_Call(void* pInfo)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_BI_CALL_CTRL* pBiCtrl = &pCtrl->tBiCall;

    pBiCtrl->pCall = (void (*)(void*))pHost->LB_Address;

    if(pBiCtrl->pCall)
    {
        MsdcVendorNvt_MemPushHostToBk();
        MsdcVendorNvt_Bk_RunCmd(BiCall_RunCmd);
    }
    else
    {
        debug_err(("BiCall_Call(): gCtrl.pCall==%d\r\n",pBiCtrl->pCall));
    }
}

static void BiCall_GetData(void* pInfo)
{
    tMSDCVENDOR_BI_CALL_CTRL* pBiCtrl = &MsdcVendorNvt_GetCtrl()->tBiCall;

    if(pBiCtrl->pCall==NULL)
    {
        debug_err(("BiCall_GetData(): gCtrl.pCall==%d\r\n",pBiCtrl->pCall));
    }
    else
    {
        MsdcVendorNvt_MemPopBkToHost();
    }
    return;
}

static void BiCall_RunCmd(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &pCtrl->tBkCtrl;
    tMSDCVENDOR_BI_CALL_CTRL* pBiCtrl = &pCtrl->tBiCall;
    pBiCtrl->pCall(pBk->pPoolMem);
}