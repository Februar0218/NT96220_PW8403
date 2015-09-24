#include "OSCommon.h"
#include "MSDCVendor.h"
#include "MSDCVendorNvtInt.h"
#include "Utility.h"
#include "uart.h"
#include <string.h>

#define CFG_MAX_COMMAND_LINE_BYTES              64   //!< Command Task Line Max Lens

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_DBGSYS Message
//------------------------------------------------------------------------------
#define CDB_11_DBGSYS_UNKNOWN                   0x00 //!< Invalid Cmd
#define CDB_11_DBGSYS_OPEN                      0x01 //!< Stopping Command Tsk and Hooking debug_msg
#define CDB_11_DBGSYS_CLOSE                     0x02 //!< Stopping DbgSys
#define CDB_11_DBGSYS_QUERY_MSG                 0x03 //!< Query Msg is Existing in Buffer Queue
#define CDB_11_DBGSYS_GET_MSG                   0x04 //!< Getting Msg from Buffer Queue
#define CDB_11_DBGSYS_SEND_CMD                  0x05 //!< Sending Command from PC -> RTOS
#define CDB_11_DBGSYS_PROPERTY_SET              0x06 //!< Set Some Property from PC
#define CDB_11_DBGSYS_PROPERTY_GET              0x07 //!< Get Some Property from PC

//------------------------------------------------------------------------------
// CDB_11_DBGSYS_SET_PROPERTY / CDB_11_DBGSYS_GET_PROPERTY Message
//------------------------------------------------------------------------------
#define CDB_12_DBGSYS_PROPERTY_UNKNOWN          0x00 //!< Invalid Type
#define CDB_12_DBGSYS_ALSO_OUTPUT_UART          0x01 //!< Also Output Uart Msg (ENABLE / DISABLE)

//------------------------------------------------------------------------------
// CDB_10_FUNCTION_DBGSYS Data Type
//------------------------------------------------------------------------------
//Parent for Host <- Device
typedef struct _DBGSYS_RESPONSE{
    UINT32 biSize;      //!< Structure Size
    UINT32 bHandled;    //!< Indicate Device Handle this function (Don't Use BOOL, for 433 Compatiable)
    UINT32 bOK;         //!< Indicate Function Action is OK or Not (Don't Use BOOL, for 433 Compatiable)
    UINT8  Reversed[2]; //!< Reversed Value for Structure DWORD Alignment
}tDBGSYS_RESPONSE;

//Parent for Host -> PC
typedef struct _DBGSYS_DELIVER{
    UINT32 biSize;      //!< Structure Size
}tDBGSYS_DELIVER;

typedef struct _DBGSYS_MSG_QUERY{
    tDBGSYS_RESPONSE tResponse;//!< Parent
    UINT32           nByteMsg; //!< nBytes Msg Need to be get
}tDBGSYS_MSG_QUERY;

typedef struct _DBGSYS_MSG_GET{
    tDBGSYS_RESPONSE tResponse;//!< Parent
    UINT32           nByteMsg; //!< nBytes Msg in following buffer
}tDBGSYS_MSG_GET;

typedef struct _DBGSYS_CMD_SEND{
    tDBGSYS_DELIVER  tDeliver; //!< Parent
    UINT32           nBytesCmd;//!< Following Data Size
}tDBGSYS_CMD_SEND;

typedef struct _DBGSYS_PROPERTY_GET{
    tDBGSYS_RESPONSE tResponse;//!< Parent
    UINT32           Type;     //!< Property Type
    UINT32           Value;    //!< Property Value
}tDBGSYS_PROPERTY_GET;

typedef struct _DBGSYS_PROPERTY_SET{
    tDBGSYS_DELIVER  tDeliver; //!< Parent
    UINT32           Type;     //!< Property Type
    UINT32           Value;    //!< Property Value
}tDBGSYS_PROPERTY_SET;


//------------------------------------------------------------------------------
// CDB_10_FUNCTION_DBGSYS Handler
//------------------------------------------------------------------------------
static ER   DbgSys_Lock(void);
static ER   DbgSys_UnLock(void);
static void DbgSys_Open(void* pInfo);
static void DbgSys_Close(void* pInfo);
static void DbgSys_MsgQuery(void* pInfo);       //!< Query Is There Existing Msg Needs to Flush
static void DbgSys_MsgGet(void* pInfo);         //!< Get Buffer Address and Length
static void DbgSys_CmdSend(void* pInfo);        //!< Send Command Line To CommandTsk
static void DbgSys_PropertySet(void* pInfo);    //!< Set Some DbgSys Property
static void DbgSys_PropertyGet(void* pInfo);    //!< Get Some DbgSys Property


//Provide Callback functions
static ER   DbgSys_MsgReceive(CHAR* pMsg);              //!< Callback for receive message
static void DbgSys_RunCmd(void);                //!< Callback for Background Thread Run Command

static tMSDCVENDOR_FUNCTION_CMD_HANDLE CallMap[] = {
    {CDB_11_DBGSYS_UNKNOWN          ,NULL},
    {CDB_11_DBGSYS_OPEN             ,DbgSys_Open},
    {CDB_11_DBGSYS_CLOSE            ,DbgSys_Close},
    {CDB_11_DBGSYS_QUERY_MSG        ,DbgSys_MsgQuery},
    {CDB_11_DBGSYS_GET_MSG          ,DbgSys_MsgGet},
    {CDB_11_DBGSYS_SEND_CMD         ,DbgSys_CmdSend},
    {CDB_11_DBGSYS_PROPERTY_SET     ,DbgSys_PropertySet},
    {CDB_11_DBGSYS_PROPERTY_GET     ,DbgSys_PropertyGet},
};

//Debug System Vendor Command
void MsdcVendorNvt_Function_DbgSys(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    UINT8 iCmd = pHost->pCBW->CBWCB[11];
    void* pInfo = pHost->pPoolMem;

    if(!pCtrl->tDbgSys.bInit)
    {
         debug_err(("MsdcDbgSys has not been initial\r\n"));
        return;
    }

    if(iCmd<sizeof(CallMap)/sizeof(tMSDCVENDOR_FUNCTION_CMD_HANDLE)
        && iCmd == CallMap[iCmd].iCall
        && CallMap[iCmd].pCall!=NULL)
    {
        CallMap[iCmd].pCall(pInfo);
    }
    else
    {
        debug_err(("MsdcVendorNvt_Function_DbgSys: Call Map Wrong!\r\n"));
    }
}
static ER DbgSys_Lock(void)
{
    return wai_sem(MsdcVendorNvt_GetCtrl()->tDbgSys.SemID);
}

static ER DbgSys_UnLock(void)
{
    return sig_sem(MsdcVendorNvt_GetCtrl()->tDbgSys.SemID);
}

static void DbgSys_Open(void* pInfo)
{
    UINT32 i;
    tDBGSYS_RESPONSE* pResponse = (tDBGSYS_RESPONSE*)pInfo;
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &MsdcVendorNvt_GetCtrl()->tDbgSys;

    memset(pResponse,0,sizeof(tDBGSYS_RESPONSE));
    pResponse->biSize = sizeof(tDBGSYS_RESPONSE);
    pResponse->bHandled = TRUE;

    DbgSys_Lock();

    pDbgSys->iMsgIn = pDbgSys->iMsgOut = 0;
    pDbgSys->fpRegHookDbgMsg(DbgSys_MsgReceive);

    for(i=0;i<pDbgSys->PayloadNum;i++)
        pDbgSys->Queue[i].nUsedBytes = 0;

    DbgSys_UnLock();

    pResponse->bOK = TRUE;
}

static void DbgSys_Close(void* pInfo)
{
    tDBGSYS_RESPONSE* pResponse = (tDBGSYS_RESPONSE*)pInfo;
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &MsdcVendorNvt_GetCtrl()->tDbgSys;

    memset(pResponse,0,sizeof(tDBGSYS_RESPONSE));
    pResponse->biSize = sizeof(tDBGSYS_RESPONSE);
    pResponse->bHandled = TRUE;

    DbgSys_Lock();

    pDbgSys->fpRegHookDbgMsg(pDbgSys->fpUartPutString);

    DbgSys_UnLock();

    pResponse->bOK = TRUE;
}

static void DbgSys_MsgQuery(void* pInfo)
{
    tDBGSYS_MSG_QUERY* pMsgQuery = (tDBGSYS_MSG_QUERY*)pInfo;
    tDBGSYS_RESPONSE*  pResponse = (tDBGSYS_RESPONSE*)pInfo;
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &MsdcVendorNvt_GetCtrl()->tDbgSys;

    memset(pMsgQuery,0,sizeof(tDBGSYS_MSG_QUERY));
    pResponse->biSize = sizeof(tDBGSYS_MSG_QUERY);
    pResponse->bHandled = TRUE;

    pMsgQuery->nByteMsg = pDbgSys->Queue[pDbgSys->iMsgOut].nUsedBytes;

    pResponse->bOK = TRUE;
}

static void DbgSys_MsgGet(void* pInfo)
{
    tDBGSYS_MSG_GET* pMsgGet = (tDBGSYS_MSG_GET*)pInfo;
    UINT8* pDst = (UINT8*)pMsgGet+sizeof(tDBGSYS_MSG_GET);
    tDBGSYS_RESPONSE* pResponse = &pMsgGet->tResponse;
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &MsdcVendorNvt_GetCtrl()->tDbgSys;

    memset(pMsgGet,0,sizeof(tDBGSYS_MSG_GET));
    pResponse->biSize = sizeof(tDBGSYS_MSG_GET);
    pResponse->bHandled = TRUE;

    if(pDbgSys->Queue[pDbgSys->iMsgOut].nUsedBytes==0)
    {
        *pDst = 0;
        pMsgGet->nByteMsg = 0;
        pResponse->bOK = FALSE;
        return;
    }

    pMsgGet->nByteMsg = pDbgSys->Queue[pDbgSys->iMsgOut].nUsedBytes;
    memcpy(pDst,pDbgSys->Queue[pDbgSys->iMsgOut].pMsg,pMsgGet->nByteMsg);

    DbgSys_Lock();
    pDbgSys->Queue[pDbgSys->iMsgOut].nUsedBytes = 0;
    pDbgSys->iMsgOut = (pDbgSys->iMsgOut+1)&pDbgSys->MsgCntMask;
    DbgSys_UnLock();

    pResponse->bOK = TRUE;
}

static void DbgSys_CmdSend(void* pInfo)
{
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &MsdcVendorNvt_GetCtrl()->tBkCtrl;

    tDBGSYS_CMD_SEND* pCmdSend = (tDBGSYS_CMD_SEND*)pInfo;
    UINT8* pCmdLine = (UINT8*)pCmdSend + sizeof(tDBGSYS_CMD_SEND);

    if(pCmdSend->tDeliver.biSize != sizeof(tDBGSYS_CMD_SEND))
    {
        debug_err(("DbgSys_CmdSend(): Not Handled\r\n"));
        return;
    }

    pBk->nTransmitSize = CFG_MAX_COMMAND_LINE_BYTES;
    memcpy(pBk->pPoolMem,pCmdLine,pBk->nTransmitSize);
    MsdcVendorNvt_Bk_RunCmd(DbgSys_RunCmd);
}

static void DbgSys_RunCmd(void)
{
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &pCtrl->tDbgSys;
    tMSDCVENDOR_BACKGROUND_CTRL* pBk = &pCtrl->tBkCtrl;
    pDbgSys->fpDoCommand((CHAR*)pBk->pPoolMem);
}

static void DbgSys_PropertySet(void* pInfo)
{
    tDBGSYS_PROPERTY_SET* pProperty = (tDBGSYS_PROPERTY_SET*)pInfo;
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &pCtrl->tDbgSys;
    UINT iType = pHost->pCBW->CBWCB[12];

    if(pProperty->tDeliver.biSize != sizeof(tDBGSYS_PROPERTY_SET))
    {
        debug_err(("DbgSys_SetProperty(): Not Handled\r\n"));
        return;
    }

    switch(iType)
    {
        case CDB_12_DBGSYS_ALSO_OUTPUT_UART:
            pDbgSys->bNoOutputUart = (pProperty->Value)?FALSE:TRUE;
            break;
    }
}

static void DbgSys_PropertyGet(void* pInfo)
{
    tDBGSYS_PROPERTY_GET* pProperty = (tDBGSYS_PROPERTY_GET*)pInfo;
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_HOST_INFO* pHost = &pCtrl->tHostInfo;
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &pCtrl->tDbgSys;
    UINT iType = pHost->pCBW->CBWCB[12];

    if(pProperty->tResponse.biSize != sizeof(tDBGSYS_PROPERTY_GET))
    {
        debug_err(("DbgSys_SetProperty(): Not Handled\r\n"));
        return;
    }

    switch(iType)
    {
        case CDB_12_DBGSYS_ALSO_OUTPUT_UART:
            pProperty->Type = CDB_12_DBGSYS_ALSO_OUTPUT_UART;
            pProperty->Value = (pDbgSys->bNoOutputUart)?FALSE:TRUE;
            break;
    }
}

static ER DbgSys_MsgReceive(CHAR* pMsg)
{
    UINT32 nCnt;
    CHAR* pDst;
    CHAR* pSrc = pMsg;
    tMSDCVENDOR_CTRL* pCtrl = MsdcVendorNvt_GetCtrl();
    tMSDCVENDOR_DBGSYS_CTRL* pDbgSys = &pCtrl->tDbgSys;
    UINT32  nMaxLen = pDbgSys->PayloadSize-1;

    pDst = (CHAR*)pDbgSys->Queue[pDbgSys->iMsgIn].pMsg;

    nCnt = 0;
    while((*pMsg)!=0 && nMaxLen--)
    {
        *(pDst++) = *(pMsg++);
        nCnt++;
    }

    *pDst = 0;
    nCnt++;

    DbgSys_Lock();
    pDbgSys->Queue[pDbgSys->iMsgIn].nUsedBytes = nCnt;
    pDbgSys->iMsgIn = (pDbgSys->iMsgIn+1)&pDbgSys->MsgCntMask;
    DbgSys_UnLock();

    if(!pDbgSys->bNoOutputUart)
        pDbgSys->fpUartPutString(pSrc);

    //Check Next Payload is Empty
    nCnt = pDbgSys->Queue[pDbgSys->iMsgIn].nUsedBytes;
    if(pDbgSys->iMsgIn==pDbgSys->iMsgOut && nCnt!=0)
    {
        INT32 nRetry = 500; //Retry 5 sec for Crash Detect
        //Wait to Free Payload by PC Getting
        while(pDbgSys->iMsgIn==pDbgSys->iMsgOut && nRetry--)
            TimerDelayMs(10);

        if(nRetry<0)
        {
            //#NT#2010/01/22#Niven Cho -begin
            //#NT#Fix Bug for both Running telnet message and update fw then update is failed!
            pDbgSys->fpRegHookDbgMsg(pDbgSys->fpUartPutString);
            //#NT#2010/01/22#Niven Cho -end
            debug_err(("^RDbgSys_MsgReceive Closed, due to buffer not empty,pDbgSys->iMsgIn=%d\r\n",pDbgSys->iMsgIn));
            return E_OK;
        }
    }

    return E_OK;
}
