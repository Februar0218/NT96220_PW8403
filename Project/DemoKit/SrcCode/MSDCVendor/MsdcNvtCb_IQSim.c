#include "SysCfg.h"

#if (_IQ_SIM_ == ENABLE)
#include "OSCommon.h"
#include "MSDCVendorNVTApi.h"
#include "MsdcNvtCb_Int.h"
#include "MsdcNvtCb_CustomSi.h"
#include "ImageQualitySim.h"

/*
Application flow of "PC IQSim Tool":

Step 1. GetSysStatus();
Step 2. GetBinAddr();
Step 3. (write bin data to Host)
Step 4. GetRawAddr();
Step 5. (write raw data to Host)
Step 6. SetConvert();
Step 7. GetJpgOk();
Step 8. GetJpgAddr();
Step 9. GetJpgSize();
Step 10.(read jpg data from Host)
*/

//#include "UMSDC.h"

//Step 1: Declare custom functions
//Declare Gets Functions
void GetBinAddr(void);
void GetRawAddr(void);
void GetJpgAddr(void);
void GetJpgOk(void);
void GetJpgSize(void);
void GetSysStatus(void);

//Declare Sets Functions
void SetConvert(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void)=
{
        GetBinAddr,  // 0 - get input bin address
        GetRawAddr,  // 1 - get input raw address
        GetJpgAddr,  // 2 - get output JPG address
        GetJpgOk,    // 3 - get output JPG status
        GetJpgSize,  // 4 - get output JPG size
        GetSysStatus, // 5 - get status (ready or not)
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void)=
{
        SetConvert, // 0 - trigger simulation process
};

//Step 4: Provide API for Register Single Direction Functions
BOOL IQSim_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets))
{
    return fpMSDCVendorNVT_AddCallback_Si(MSDCCustomGet,sizeof(MSDCCustomGet)/sizeof(MSDCCustomGet[0]),MSDCCustomSet,sizeof(MSDCCustomSet)/sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
void GetBinAddr(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();
    debug_msg("[MSDC] ===== Get Bin Addr =====\r\n");

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;
    *pData = uiBinAddr;
    debug_msg("[MSDC] data=%08x\r\n", *pData);
}
void GetRawAddr(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    debug_msg("[MSDC] ===== Get Raw Addr =====\r\n");
    if(uiLength!=sizeof(UINT32)) //invalid data
        return;
    *pData = ((uiBinAddr + uiBinSize)+15)/16*16;
    debug_msg("[MSDC] data=%08x\r\n", *pData);
}

void GetJpgAddr(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();
    
    debug_msg("[MSDC] ===== Get JPEG Addr =====\r\n");
    if(uiLength!=sizeof(UINT32)) //invalid data
        return;
    *pData = gBuf_JPG.addr[0];
    debug_msg("[MSDC] data=%08x\r\n", *pData);
}

void GetJpgSize(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    debug_msg("[MSDC] ===== Transmit JPEG Data =====\r\n");
    if(uiLength!=sizeof(UINT32)) //invalid data
        return;
    *pData = gBuf_JPG.size;
    debug_msg("[MSDC] size=%08x\r\n", *pData);
}

void GetJpgOk(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    debug_msg("[MSDC] ===== Get JPEG OK =====\r\n");
    if(uiLength!=sizeof(UINT32)) //invalid data
        return;
    *pData = bIQSimExec;
    debug_msg("[MSDC] result=%08x\r\n", *pData);
}

#define ConnectToDSP_OK  0
#define ConnectToDSP_Fail 1
UINT32 ConnectToDSP = ConnectToDSP_Fail;

void GetSysStatus(void)
{
    UINT32   uiPoolAddr;
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();    

    debug_msg("[MSDC] ===== Get System Status =====\r\n");
    if (get_blk((void*)&uiPoolAddr, POOL_ID_FB) != E_OK)
        return;
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);
    uiTotalAddr = uiPoolAddr;
    uiTotalSize = END_MEM - uiPoolAddr - POOL_SIZE_RAM_DISK;
    debug_msg("memory addr=%08x, size=%08x\r\n", uiTotalAddr, uiTotalSize);
    /*
    debug_msg("memory fill zero...");
    memset(uiTotalAddr, 0, uiTotalSize);
    debug_msg("ok\r\n");
    */
    uiBinAddr = uiPoolAddr;

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;
    if(IQSim_Init() == 0)
    {
        ConnectToDSP = ConnectToDSP_Fail;
        *pData = ConnectToDSP;
        return;
    }
    ConnectToDSP = ConnectToDSP_OK;
    *pData = ConnectToDSP;
    debug_msg("[MSDC] result=%08x\r\n", *pData);
}

void SetConvert(void)
{
    UINT32* pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32  uiLength = MsdcNvt_GetTransSize();

    debug_msg("[MSDC] ===== Start Convert =====\r\n");
    if(uiLength && (pData[0]==TRUE))
    {
        IQSim_Exec();
    }
    debug_msg("[MSDC] Finish\r\n");
}

#endif
