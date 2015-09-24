#ifndef _MSDCNVTCB_FILE_H
#define _MSDCNVTCB_FILE_H

//APIs for MSDCVendorNVT_AddCallback_Bi
void MsdcNvtCb_FileGetWorkingMemory(void* pData);
void MsdcNvtCb_FileCopyPcToDevice(void* pData);
void MsdcNvtCb_FileCopyDeviceToPc(void* pData);
void MsdcNvtCb_FileDeleteDeviceFile(void* pData);

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_BICALL_REG_FILE() \
    REGISTER_BI_CALLBACK(MsdcNvtCb_FileGetWorkingMemory); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_FileCopyPcToDevice); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_FileCopyDeviceToPc); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_FileDeleteDeviceFile); 
#endif