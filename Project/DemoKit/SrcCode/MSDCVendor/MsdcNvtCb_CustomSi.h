#ifndef _MSDCNVTCB_CUSTOMSI_H
#define _MSDCNVTCB_CUSTOMSI_H

//APIs for MSDCVendorNVT_AddCallback_Si
BOOL CustomSi_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets));

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_SICALL_REG_CUSTOMSI() \
    CustomSi_Register_SiCall(MSDCVendorNVT_AddCallback_Si); 

#endif