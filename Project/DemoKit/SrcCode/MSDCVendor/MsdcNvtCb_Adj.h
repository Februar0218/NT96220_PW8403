#ifndef _MSDCNVTCB_ADJ_H
#define _MSDCNVTCB_ADJ_H

//APIs for MSDCVendorNVT_AddCallback_Bi
void MsdcNvtCb_AdjGetNumOfItem(void* pData);
void MsdcNvtCb_AdjGetItemsDesc(void* pData);
void MsdcNvtCb_AdjSetItemsData(void* pData);
void MsdcNvtCb_AdjGetItemsData(void* pData);

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_BICALL_REG_ADJ() \
    REGISTER_BI_CALLBACK(MsdcNvtCb_AdjGetNumOfItem); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_AdjGetItemsDesc); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_AdjSetItemsData); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_AdjGetItemsData); 
    
#endif    
