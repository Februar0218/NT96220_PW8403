#include "OSCommon.h"
#include "MSDCVendor.h"
#include "MSDCVendorNvtInt.h"
#include <string.h>

//Used CMD Field to be Command Type
#define CDB_IDX_CALL_ID         12   //!< Use CDB[12] to be Function ID

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_SI_CALL Message
//------------------------------------------------------------------------------
#define CDB_11_SI_CALL_UNKNOWN                  0x00 //!< Invalid Cmd
#define CDB_11_SI_CALL_PC_GET_RUN_CMD           0x01 //!< Single Direction,from PC-Get Step 0, Use CDB[12] to be Function ID
#define CDB_11_SI_CALL_PC_GET_GET_RESULT        0x02 //!< Single Direction,from PC-Get Step 1, Use CDB[12] to be Function ID
#define CDB_11_SI_CALL_PC_SET                   0x03 //!< Single Direction,from PC-Set, Use CDB[12] to be Function ID

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_SI_CALL Handler
//------------------------------------------------------------------------------
static void SiCall_Single_Get_RunCmd(void* pInfo);      //!< Send Data to PC (Single Direction)
static void SiCall_Single_Get_GetResult(void* pInfo);   //!< Send Data to PC (Single Direction)
static void SiCall_Single_Set(void* pInfo);             //!< Recevice PC (Single Direction)
static void SiCall_RunCmdGet(void);                     //!< Callback for Background Thread Run Command for Get
static void SiCall_RunCmdSet(void);                     //!< Callback for Background Thread Run Command for Set

//Local Variables
static tMSDCVENDOR_FUNCTION_CMD_HANDLE CallMap[] = {
    {CDB_11_SI_CALL_UNKNOWN,NULL},
    {CDB_11_SI_CALL_PC_GET_RUN_CMD,     SiCall_Single_Get_RunCmd},    //Get Step 0: Run Command
    {CDB_11_SI_CALL_PC_GET_GET_RESULT,  SiCall_Single_Get_GetResult}, //Get Setp 1: Get Result Data
    {CDB_11_SI_CALL_PC_SET,             SiCall_Single_Set},
};

void MsdcVendorNvt_Function_SiCall(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();    
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    UINT8 iCmd = pHost->pCBW->CBWCB[11];

    if(iCmd<sizeof(CallMap)/sizeof(tMSDCVENDOR_FUNCTION_CMD_HANDLE) 
        && iCmd == CallMap[iCmd].iCall
        && CallMap[iCmd].pCall!=NULL)
    {
        CallMap[iCmd].pCall(NULL); //pInfo is not used
    }
    else
    {
        debug_err(("MsdcVendorNvt_Function_Utility: Call Map Wrong!\r\n"));
    }
}

static void SiCall_Single_Get_RunCmd(void* pInfo)
{
    MsdcVendorNvt_MemPushHostToBk();
    MsdcVendorNvt_Bk_RunCmd(SiCall_RunCmdGet);
}

static void SiCall_Single_Get_GetResult(void* pInfo)
{
    MsdcVendorNvt_MemPopBkToHost();
}

static void SiCall_Single_Set(void* pInfo)
{
    MsdcVendorNvt_MemPushHostToBk();
    MsdcVendorNvt_Bk_RunCmd(SiCall_RunCmdSet);
    MsdcVendorNvt_MemPopBkToHost();
}

static void SiCall_RunCmdGet(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_SI_CALL_CTRL* pSiCtrl = &pCtrl->tSiCall;
    
    UINT iCall = pHost->pCBW->CBWCB[CDB_IDX_CALL_ID];
    
    if(iCall >= pSiCtrl->nGets)
    {
        debug_err(("SBC_Vendor_Function_Single_Get(): function index out of range\r\n"));
        return;
    }

    pSiCtrl->fpTblGet[iCall]();
}

static void SiCall_RunCmdSet(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_SI_CALL_CTRL* pSiCtrl = &pCtrl->tSiCall;
    
    UINT iCall = pHost->pCBW->CBWCB[CDB_IDX_CALL_ID];
    
    if(iCall >= pSiCtrl->nSets)
    {
        debug_err(("SBC_Vendor_Function_Single_Set(): function index out of range\r\n"));
        return;
    }

    pSiCtrl->fpTblSet[iCall]();
}