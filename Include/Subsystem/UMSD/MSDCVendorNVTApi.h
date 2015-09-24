/**
    Msdc-Nvt Vendor

    This system for PC control device(DSC) via MSDC. User can register callbacks to respond PC command.

    @file       MSDCVendorNVTApi.h
    @ingroup    mISYSMSDCNVT
    @note       require to set MSDCVENDOR = MSDCVENDOR_NVT in MakeConfig.txt.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _MSDCVENDORNVTAPI_H
#define _MSDCVENDORNVTAPI_H

#include "OSCommon.h"

/**
     Msdc-Nvt vendor configuration strucutre

     Set working buffer, semaphore, task id for Msdc-Nvt system using via MSDCVendorNVT_Configure() before calling USBMSDC_Open()
*/
typedef struct _MSCDVENDOR_CFG{
    UINT8* pMemCache;                               ///< Cache-able Memory (0x24040 Bytes Requirement) Front-End:0x10000,Back-End:0x10000,Msg:0x4000,CBW:0x40
    UINT32 nSizeCache;                              ///< n Bytes of Cache Memory
    ID     SemID;                                   ///< Semaphore ID for DBGSYS
    ID     TaskID;                                  ///< Working Task ID for Command Send
    void   (*fpDoCommand)(CHAR *pcCmdStr);          ///< Callback for calling command
    void   (*fpRegHookDbgMsg)(ER (*PutStr)(CHAR*)); ///< Register debug_msg output function
    ER     (*fpUartPutString)(CHAR*);               ///< Select to uart_putString or uart2_putString to real uart output
}MSDCVENDOR_CFG,*PMSDCVENDOR_CFG;


/**
     Msdc-Nvt vendor configuration

     Configure Msdc-Nvt for system initial

     @param[in] pCfg       Configuration strucutre
     @return
         - @b TRUE:   configuration success.
         - @b FALSE:  configuration failed.
*/
extern BOOL MSDCVendorNVT_Configure(MSDCVENDOR_CFG* pCfg);

/**
     Get host-device common data buffer address

     Get data address for single directtion callback function

     @return
         - @b NULL:     failed to get buffer address.
         - @b NON-NULL: buffer address.
*/
extern UINT8* MSDCVendorNVT_GetDataBufferAddress(void);

/**
     Get host-device common data buffer size (byte unit)

     Get data size for single directtion callback function

     @return
         @return
         - @b 0:        failed to get buffer size.
         - @b Non-Zero: buffer size.
*/
extern UINT32 MSDCVendorNVT_GetDataBufferSize(void);

/**
     Register bi-direction type callback function

     PlugIn vendor function(bi-direction, form PC set then get automatic)

     @param[in] szName function name with 63 characters limitation
     @param[in] fp     callback function you want to register
     @return
         - @b TRUE:    success to register.
         - @b FALSE:   failed to register callback function.
*/
extern BOOL MSDCVendorNVT_AddCallback_Bi(char* szName,void (*fp)(void* pData));

/**
     Register single-direction type callback function

     Plugin vendor function(single-direction,from PC-Get and PC-Set)

     @param[in] fpTblGet function table for host get data
     @param[in] nGets    number of functions in fpTblGet table
     @param[in] fpTblSet function table for host set data
     @param[in] nSets    number of functions in fpTblSet table
     @return
         - @b TRUE:    success to register.
         - @b FALSE:   failed to register callback function.
*/
extern BOOL MSDCVendorNVT_AddCallback_Si(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets);

/**
     Get transmitted data size

     For MSDCVendorNVT_AddCallback_Si functions, get pc how many data is transmitted

     @return data size (byte unit).
*/
extern UINT32 MSDCVendorNVT_GetTransSize(void);

/**
     Msdc-Nvt vendor main task

     This task is used for do background command, currently.
*/
extern void MsdcVendorNvtTsk(void);


/**
     @name Backward compatiable
     @note Just old version convert to new version
*/
//@{
typedef MSDCVENDOR_CFG tMSDCVENDOR_CFG; ///< Change name from tMSDCVENDOR_CFG to MSDCVENDOR_CFG
//@}

#endif
