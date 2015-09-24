#include "OSCommon.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Int.h"
#include "MSDCVendorNvtApi.h"

//Check Parameters, return NULL indicate error parameters
void* MsdcNvt_ChkParameters(void* pData,UINT32 uiSize)
{
    tMSDCEXT_PARENT* pDesc = (tMSDCEXT_PARENT*)pData;
    if(pDesc->biSize != uiSize)
    {
        debug_err(("MsdcNvtCallback: Data Size Wrong, %d(Host) != %d(Device)\r\n",pDesc->biSize,uiSize));
        pDesc->bHandled = TRUE;
        pDesc->bOK = FALSE;
        return NULL;
    }
    return pData;
}

//Get USB Data Buffer Address
UINT8*  MsdcNvt_GetDataBufferAddress(void)
{
    #if(_MSDCVENDOR_== _MSDCVENDOR_NVT_)
        return MSDCVendorNVT_GetDataBufferAddress();
    #else
        return NULL;
    #endif
}

//Get USB Data Buffer Length
UINT32  MsdcNvt_GetTransSize(void)
{
    #if(_MSDCVENDOR_== _MSDCVENDOR_NVT_)
        return MSDCVendorNVT_GetTransSize();
    #else
        return 0;
    #endif
}