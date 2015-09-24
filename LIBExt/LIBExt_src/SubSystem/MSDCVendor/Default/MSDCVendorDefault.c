#include "OSCommon.h"
#include "MSDCVendor.h"

BOOL MSDCVendor_Verify_CB(UINT32 pCmdBuf, UINT32 *pDataBuf) //!< Callback for USB_MSDC_INFO.msdc_check_cb
{    
    return FALSE;
}

void MSDCVendor_Vendor_CB(PMSDCVendorParam pBuf) //!< Callback for USB_MSDC_INFO.msdc_vendor_cb
{
}