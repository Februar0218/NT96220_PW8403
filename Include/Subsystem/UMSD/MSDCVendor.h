#ifndef _MSDCVENDOR_H
#define _MSDCVENDOR_H

#include "UMSDC.h"

//Bit Mask with T_MSDCVENDOR_CSW.dCSWTag
#define SBC_CMD_DIR_MASK  0x80
#define SBC_CMD_DIR_IN    0x80
#define SBC_CMD_DIR_OUT   0x00


/* USB Mass Storage Class Bulk only protocol Command Block Wrapper */
typedef struct _MSDCVENDOR_CBW {
    UINT32 dCBWSignature;
    UINT32 dCBWTag;
    UINT32 dCBWDataTransferLength;
    UINT8  bmCBWFlags;
    UINT8  bCBWLUN;
    UINT8  bCBWCBLength;
    UINT8  CBWCB[16];
}tMSDCVENDOR_CBW; //!<for pCmdBuf, PMSDCVendorParam.VendorCmdBuf Casting

/* USB Mass Storage Class Bulk only protocol Command Status Wrapper */
typedef struct _MSDCVENDOR_CSW {
    UINT32 dCSWSignature;
    UINT32 dCSWTag;
    UINT32 dCSWDataResidue;
    UINT8  bCSWStatus;
}tMSDCVENDOR_CSW; //!<for pCmdBuf, PMSDCVendorParam.VendorCSWBuf Casting

extern BOOL MSDCVendor_Verify_CB(UINT32 pCmdBuf, UINT32 *pDataBuf);     //!< Callback for USB_MSDC_INFO.msdc_check_cb
extern void MSDCVendor_Vendor_CB(PMSDCVendorParam pBuf);                //!< Callback for USB_MSDC_INFO.msdc_vendor_cb

#endif
