#ifndef _MSDCVENDOR_NVT_H
#define _MSDCVENDOR_NVT_H

#include "Type.h"
#include "MSDCVendorNvtApi.h"

//Configure
#define CFG_DBGSYS_MSG_PAYLOAD_NUM      (1<<7)      //!< Must be 2^n
#define CFG_FUNCTION_BI_NAME_SIZE       64          //!< Name Max is 63 bytes
#define CFG_NUM_BI_CALLS                256         //!< MAX BiCall Support Number
#define CFG_REGISTER_BEGIN_ADDR         0x90000000  //!< Mem Read/Write over this indicate to Register Access

//Configure Working Memory Pool
#define CFG_MIN_HOST_MEM_SIZE           0x10000     //!< Front-End:0x10000
#define CFG_MIN_BACKGROUND_MEM_SIZE     0x10000     //!< Back-End:0x10000
#define CFG_MIN_DBGSYS_MSG_MEM          0x04000     //!< Msg Queue
#define CFG_MIN_WORKING_BUF_SIZE        (CFG_MIN_HOST_MEM_SIZE+CFG_MIN_BACKGROUND_MEM_SIZE+CFG_MIN_DBGSYS_MSG_MEM)

typedef struct _MSDCVENDOR_HOST_INFO{
    //SCSI Command Info
    tMSDCVENDOR_CBW    *pCBW;           //!< SCSI Command Block Wrapper
    tMSDCVENDOR_CSW    *pCSW;           //!< SCSI Command Status Wrapper
    //Calculated Private Data by MSDCVendor_Verify_CB parsed
    UINT    iCmd;                       //!< SBC Command ID, SBC_CMD_MSDCVENDOR_XXXX
    UINT32  LB_Address;                 //!< Logical Block Address of SCSI Command
    UINT32  LB_Length;                  //!< Transfer Length of SCSI Command
    //Universal Common Data Swap Pool (Host <-> Device)
    UINT8*  pPoolMem;                   //!< Host<->Device Swap Common Memory Pool
    UINT32  nPoolSize;                  //!< Host<->Device Common Memory Pool Size (Unit: Bytes)
}tMSDCVENDOR_HOST_INFO;


//------------------------------------------------------------------------------
// Novatek Tag
//------------------------------------------------------------------------------
#define CDB_01_NVT_TAG                          0x07 //!< Novatek Tag in CDB_01

//------------------------------------------------------------------------------
// Host SCSI Extern Command
//------------------------------------------------------------------------------
#define SBC_CMD_MSDCVENDOR_READ                 0xC3 //!< Read Memory / Registers
#define SBC_CMD_MSDCVENDOR_WRITE                0xC4 //!< Write Memory / Registers
#define SBC_CMD_MSDCVENDOR_FUNCTION             0xC5 //!< Call Function in Firmware Side

//------------------------------------------------------------------------------
// SBC_CMD_MSDCVENDOR_FUNCTION Message
//------------------------------------------------------------------------------
#define CDB_10_FUNCTION_UNKNOWN                 0x00 //!< Invalid Type
#define CDB_10_FUNCTION_BI_CALL                 0x01 //!< Bi-Direction Function Type
#define CDB_10_FUNCTION_SI_CALL                 0x02 //!< Single-Direction Function Type
#define CDB_10_FUNCTION_DBGSYS                  0x03 //!< Debug System by Msdc
#define CDB_10_FUNCTION_MISC                    0xFF //!< Misc (Must be Last)

typedef struct _MSDCVENDOR_FUNCTION_CMD_HANDLE{
    UINT32 iCall;                    //!< Calling Type (CDB_11 Value)
    void   (*pCall)(void* pInfo);    //!< Callback for Handler
}tMSDCVENDOR_FUNCTION_CMD_HANDLE;

//------------------------------------------------------------------------------
// Bi-Direction Call for CDB_10_FUNCTION_BI_CALL
//------------------------------------------------------------------------------
//Bi-Direction Call Function Unit
typedef struct _MSDCVENDOR_BI_CALL_HANDLE{
    char   szName[CFG_FUNCTION_BI_NAME_SIZE];           //!< Function Name
    void   (*fp)(void* pData);                          //!< Handler Callback
}tMSDCVENDOR_BI_CALL_HANDLE;

//Bi-Direction Function Control
typedef struct _MSDCVENDOR_BI_CALL_CTRL{
    void                        (*pCall)(void* pData);  //!< Current Function Call (BiDirection)
    UINT32                      n_Callback;             //!< Total Registered Callbacks
    UINT32                      nMaxNameSize;           //!< Equal to CFG_FUNCTION_BI_NAME_SIZE
    tMSDCVENDOR_BI_CALL_HANDLE  tTblCallback[CFG_NUM_BI_CALLS]; //!< Callbacks Description
}tMSDCVENDOR_BI_CALL_CTRL;

//------------------------------------------------------------------------------
// Single-Direction Call for CDB_10_FUNCTION_SI_CALL
//------------------------------------------------------------------------------
//Single-Direction Function Control
typedef struct _MSDCVENDOR_SI_CALL_CTRL{
    FP*     fpTblGet;                   //!< PC Get Functions Mapping Talbe(Single Direction)
    UINT8   nGets;                      //!< Number of PC Get Functions Mapping Talbe(Single Direction)
    FP*     fpTblSet;                   //!< PC Set Functions Mapping Talbe(Single Direction)
    UINT8   nSets;                      //!< Number of PC Set Functions Mapping Talbe(Single Direction)
}tMSDCVENDOR_SI_CALL_CTRL;


//------------------------------------------------------------------------------
// Debug System for CDB_10_FUNCTION_DBGSYS
//------------------------------------------------------------------------------
typedef struct _MSDCVENDOR_DBGSYS_UNIT{
    UINT32 nSkip;       //!< Count this payload Msg has been skiped how many times
    UINT32 nUsedBytes;  //!< Used Msg Length
    UINT8* pMsg;        //!< Msg Buffer
}tMSDCVENDOR_DBGSYS_UNIT;

//Background Ctrl
typedef struct _MSDCVENDOR_BACKGROUND_CTRL{
    ID      TaskID;                     //!< Working Task ID for Command Send
    UINT8*  pPoolMem;                   //!< Device<->Background Common Memory Pool
    UINT32  nPoolSize;                  //!< Device<->Background Common Memory Pool Size (Unit: Bytes)
    UINT32  nTransmitSize;              //!< Effect Data Size of this Call
    void    (*pCall)(void);             //!< Device<->Background Thread Action Callback Function
    BOOL    bServiceLock;               //!< Indicate Service is Lock / Unlock by Host
    BOOL    bCmdRunning;                //!< Indicate Background Thread is Running
}tMSDCVENDOR_BACKGROUND_CTRL;

//DbgSys Ctrl
typedef struct MSDCVENDOR_DBGSYS_CTRL{
    BOOL   bInit;                       //!< Indicate DbgSys Init
    ID     SemID;                       //!< Semaphore ID for lock / unlock Msg Receiver
    UINT32 iMsgIn;                      //!< Msg Index for rtos putting
    UINT32 iMsgOut;                     //!< Msg Index for pc   getting
    UINT32 MsgCntMask;                  //!< Mask for Msg Count Add 1
    UINT32 PayloadNum;                  //!< Total Payload
    UINT32 PayloadSize;                 //!< Each tMSDCVENDOR_MSG_UNIT.pMsg Size
    BOOL   bNoOutputUart;               //!< Disable Output to UART
    void   (*fpDoCommand)(CHAR *pcCmdStr);          //!< Callback for calling command
    void   (*fpRegHookDbgMsg)(ER (*PutStr)(CHAR*)); //!< Register debug_msg output function
    ER     (*fpUartPutString)(CHAR*);               //!< default real uart output (uart_putString or uart2_putString)
    tMSDCVENDOR_DBGSYS_UNIT Queue[CFG_DBGSYS_MSG_PAYLOAD_NUM]; //!< Message Queue
}tMSDCVENDOR_DBGSYS_CTRL;

//------------------------------------------------------------------------------
// Global Control Manager
//------------------------------------------------------------------------------
typedef struct _MSDCVENDOR_CTRL{
    BOOL                        bInit;      //!< Inidicate System Initial
    tMSDCVENDOR_HOST_INFO       tHostInfo;  //!< Host Information
    tMSDCVENDOR_SI_CALL_CTRL    tSiCall;    //!< Single-Direction Call Ctrl
    tMSDCVENDOR_BI_CALL_CTRL    tBiCall;    //!< BiDirection Call Ctrl
    tMSDCVENDOR_DBGSYS_CTRL     tDbgSys;    //!< Debug System Ctrl
    tMSDCVENDOR_BACKGROUND_CTRL tBkCtrl;    //!< Background Thread Ctrl
}tMSDCVENDOR_CTRL;


//------------------------------------------------------------------------------
// Internal APIs
//------------------------------------------------------------------------------
tMSDCVENDOR_CTRL* MsdcVendorNvt_GetCtrl(void);  //!< Get Global Control
void              MsdcVendorNvt_MemPushHostToBk(void);  //!< Copy tMSDCVENDOR_HOST_INFO::pPoolMem -> tMSDCVENDOR_BACKGROUND_CTRL::pPoolMem
void              MsdcVendorNvt_MemPopBkToHost(void);   //!< Copy tMSDCVENDOR_BACKGROUND_CTRL::pPoolMem -> tMSDCVENDOR_HOST_INFO::pPoolMem

BOOL              MsdcVendorNvt_Bk_RunCmd(void (*pCall)(void)); //!< Run Callback in Background Thread
BOOL              MsdcVendorNvt_Bk_IsFinish(void);              //!< Query Background Thread is Finish
BOOL              MsdcVendorNvt_Bk_HostLock(void);              //!< Lock Bakcgorund Service by Host
BOOL              MsdcVendorNvt_Bk_HostUnLock(void);            //!< UnLock Bakcgorund Service by Host
BOOL              MsdcVendorNvt_Bk_HostIsLock(void);            //!< Query Bakcgorund Service is Lock

//Vendor Command (CDB[10]) Handler
void MsdcVendorNvt_Function_BiCall(void);       //!< Bi Direction Call
void MsdcVendorNvt_Function_SiCall(void);       //!< Single Direction Call
void MsdcVendorNvt_Function_DbgSys(void);       //!< Debug System Vendor Command
void MsdcVendorNvt_Function_Misc(void);         //!< Debug System Vendor Command

#endif
