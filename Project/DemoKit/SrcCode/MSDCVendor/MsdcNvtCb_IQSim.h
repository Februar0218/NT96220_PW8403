#ifndef _MSDCNVTCB_IQSIM_H
#define _MSDCNVTCB_IQSIM_H

//APIs for MSDCVendorNVT_AddCallback_Si
BOOL IQSim_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets));

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_SICALL_REG_IQSIM() \
    IQSim_Register_SiCall(MSDCVendorNVT_AddCallback_Si); 
    
#endif