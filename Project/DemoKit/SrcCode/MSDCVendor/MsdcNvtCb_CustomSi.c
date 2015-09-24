#include "OSCommon.h"
#include "MSDCVendorNVTApi.h"
#include "MsdcNvtCb_Int.h"
#include "MsdcNvtCb_CustomSi.h"
#include "SysCfg.h"

//Step 1: Declare custom functions
//Declare Gets Functions
static void GetSayHi(void);
static void GetSayHello(void);
//Declare Sets Functions
static void SetSayHi(void);
static void SetSayHello(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void)=
{
        GetSayHi,                   // 0, #define ID_GET_SAY_HI    0 //in AP Side
        GetSayHello,                // 1, #define ID_GET_SAY_HELLO 1 //in AP Side
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void)=
{
        SetSayHi,                   // 0, #define ID_SET_SAY_HI    0 //in AP Side
        SetSayHello,                // 1, #define ID_SET_SAY_HELLO 1 //in AP Side        
};

//Step 4: Provide API for Register Single Direction Functions
BOOL CustomSi_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets))
{
    return fpMSDCVendorNVT_AddCallback_Si(MSDCCustomGet,sizeof(MSDCCustomGet)/sizeof(MSDCCustomGet[0]),MSDCCustomSet,sizeof(MSDCCustomSet)/sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
static void GetSayHi(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();
        
    if(uiLength!=sizeof(UINT32)) //invalid data
        return;             
        
    debug_msg("@@@@@@ Get Say Hi @@@@@@\r\n");
    *pData = 12345;
}

static void GetSayHello(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    debug_msg("uiLength=%d\r\n",uiLength);
    
    if(uiLength!=sizeof(UINT32)) //invalid data
        return;             

    debug_msg("@@@@@@ Get Say Hello @@@@@@\r\n");
    *pData = 67890;
}

static void SetSayHi(void)
{
    UINT32  i;
    UINT32* pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32  uiLength = MsdcNvt_GetTransSize();
    
    debug_msg("###### Set Say Hi ###### \r\n");

    if(uiLength)
    {
        for(i=0;i<uiLength;i+=4)
        {
            debug_err(("Data[%d]=%d\r\n", i, *pData));
            pData++;
        }
    }
}

static void SetSayHello(void)
{
    UINT32  i;
    UINT32* pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32  uiLength = MsdcNvt_GetTransSize();
    
    debug_msg("###### Set Say Hello ###### \r\n");

    if(uiLength)
    {
        for(i=0;i<uiLength;i+=4)
        {
            debug_err(("Data[%d]=%d\r\n", i, *pData));
            pData++;
        }
    }
}