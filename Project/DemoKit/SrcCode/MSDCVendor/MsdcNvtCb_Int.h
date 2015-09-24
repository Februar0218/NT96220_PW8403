#ifndef _MSDCNVTCB_INT_H
#define _MSDCNVTCB_INT_H

#define BEGIN_MSDC_NVT_CB(type,p_data) (type*)MsdcNvt_ChkParameters(p_data,sizeof(type)) //!< Check Begin Parameters from Host
#define REGISTER_BI_CALLBACK(x) MSDCVendorNVT_AddCallback_Bi(#x,(x))                     //!< Register Bi-Direction Function Callback

//------------------------------------------------------------
// Local Utility Functions
//------------------------------------------------------------
//Check Parameters for all MsdcNvtCb_xxxxx
void*   MsdcNvt_ChkParameters(void* pData,UINT32 uiSize);
//Get USB Data Buffer Address
UINT8*  MsdcNvt_GetDataBufferAddress(void);
//Get USB Data Buffer Length
UINT32  MsdcNvt_GetTransSize(void);

#endif