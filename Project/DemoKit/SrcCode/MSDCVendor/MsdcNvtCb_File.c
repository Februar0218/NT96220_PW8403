#include "OSCommon.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Int.h"
#include "MsdcNvtCb_File.h"
#include "SysCfg.h"
#include "FilesysTsk.h"

//Function Implementation
void MsdcNvtCb_FileGetWorkingMemory(void* pData)
{
    tMSDCEXT_FILE_WORKING_MEM* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_FILE_WORKING_MEM,pData);
    if(pDesc==NULL)
        return;    

    get_blk((void *)&pDesc->Address,  POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)pDesc->Address);
    pDesc->Size = POOL_SIZE_CAPTURE;
    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_FileCopyPcToDevice(void* pData)
{
    UW BufSize;
    tMSDCEXT_FILE_INFO* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_FILE_INFO,pData);
    if(pDesc==NULL)
        return;

    BufSize = pDesc->TransmitSize;
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    if(pDesc->TransmitOffset==0) //First In, Delete File First
    {
        FilesysDeleteByName(pDesc->FilePath,FALSE);        
    }
    
    if(FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME
        ,pDesc->FilePath
        ,(UB*)pDesc->WorkingMem
        ,&BufSize
        ,pDesc->TransmitOffset
        ,FST_TIME_INFINITE)==E_OK)
    {
        pDesc->tParent.bOK = TRUE;
    }

}

void MsdcNvtCb_FileCopyDeviceToPc(void* pData)
{
    UW BufSize;
    tMSDCEXT_FILE_INFO* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_FILE_INFO,pData);
    if(pDesc==NULL)
        return;

    BufSize = pDesc->TransmitSize;
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME
        ,pDesc->FilePath
        ,(UB*)pDesc->WorkingMem
        ,&BufSize
        ,pDesc->TransmitOffset
        ,FST_TIME_INFINITE)==E_OK)
    {
        pDesc->tParent.bOK = TRUE;
    }
}

void MsdcNvtCb_FileDeleteDeviceFile(void* pData)
{
    tMSDCEXT_FILE_INFO* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_FILE_INFO,pData);
    if(pDesc==NULL)
        return;

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    if(FilesysDeleteByName(pDesc->FilePath,FALSE)==E_OK)
    {
        pDesc->tParent.bOK = TRUE;
    }
}
