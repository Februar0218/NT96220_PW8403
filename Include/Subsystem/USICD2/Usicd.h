/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       Usicd.h
    @ingroup    mISYSUSICD

    @brief      USB SICD device class API header.
                USB SICD device class API header.

*/

#ifndef _USBSICD2_H
#define _USBSICD2_H

/***********************************
*   Includes
************************************/
#include "UART.h"
#define USICDDBGStr(x); {UARTPutString("SICD Debug String: "); UARTPutString(x);}

#include "Type.h"
#include "StrgDef.h"
#include "USB.h"
#include "OSCommon.h"

#define USICD_STATUS_OK                0x01
#define USICD_STATUS_BUSY              0x02

#define USICD_DEVICE_OK                0x00
#define USICD_DEVICE_BUSY              0x01
#define USICD_DEVICE_ONCANCEL          0x02


/**
    \addtogroup mISYSUSICD
@{
*/
typedef void (*USICD_COMMAND_PROCESS)(UINT16);
typedef void (*USICD_STOP_TRANSFER)(void);
typedef void (*USICD_DEVICE_RESET)(void);

/**
    USB Infomation Table for SICD.
*/
typedef struct {

    USB_STRING_DESC     *pManuStringDesc;   ///< USB Manufacturer String Descriptor ptr
    USB_STRING_DESC     *pProdStringDesc;   ///< USB Product String Descriptor ptr
    USB_STRING_DESC     *pSerialStringDesc; ///< USB Serial Number String Descriptor ptr
    UINT16              VID;                ///< Customized USB Vendor ID
    UINT16              PID;                ///< Customized USB Product ID
    char                *pSICDString;       ///< SICD identification string
    UINT16              SICDStringLen;      ///< SICD identification string length
    //cardLock should be set when Usicd_Open()
    UINT16              uiStrgCardLock;

//#NT#2008/11/10#York Dai -begin
// The FW update should be handled by PTP, not USICD.
/*
    // File name of binary file to be downloaded via PTP.
    char                *pPTPFWUpdateFilename;
    // Memory space for PTP firmware download.
    UINT32              uiMemoryAddr;       ///< Memory start address for PTP firmware download.
    UINT32              uiMemorySize;       ///< Memory size for PTP firmware download.
*/
//#NT#2008/11/10#York Dai -end

    USICD_COMMAND_PROCESS fpCommandProcess;
    USICD_STOP_TRANSFER   fpStopTransfer;
    USICD_DEVICE_RESET    fpDeviceReset;
} USB_SICD_INFO;


extern ER       Usicd_Open(STRG_TAB *pStrgDev, USB_SICD_INFO *pClassInfo);
extern void     Usicd_Close(void);
extern UINT8    Usicd_GetStatus(void);

extern void     Usicd_StopTransfer(void);
extern UINT32   Usicd_GetTransactionID(void);
extern UINT32   Usicd_GetDeviceState(void);
extern void     Usicd_SetDeviceState(UINT32 state);
extern void     Usicd_SetStorage(STRG_TAB *pStrgDev);
extern void     Usicd_GetStorage(UINT32 *pstorageAddr);
extern void     Usicd_SetCardLock(UINT16 cardlock);
extern UINT16   Usicd_GetCardLock(void);
extern ER       Usicd_GetCommandBlock(UINT32 *puiTranID, UINT16 *puiCmdCode, UINT16 *puiParamCount, UINT32 **pParams);
extern ER       Usicd_GetDataBlock(UINT32 uiTranID, UINT32 *puiTotalLength, UINT8 *puiBuf, UINT32 *puiLength);
extern ER       Usicd_GetConDataBlock(UINT32 uiTranID, UINT8 *puiBuf, UINT32 *puiLength);
extern ER       Usicd_SendDataBlock(UINT32 uiTranID, UINT32 uiTotalLength, UINT8 *puiBuf, UINT32 uiLength);
extern ER       Usicd_SendConDataBlock(UINT32 uiTranID, UINT8 *puiBuf, UINT32 uiLength);
extern ER       Usicd_SendMassDataBlock(UINT32 uiTranID, UINT32 uiTotalLength, UINT8 *puiBuf, UINT32 uiLength);
extern ER       Usicd_SendConMassDataBlock(UINT32 uiTranID, UINT8 *puiBuf, UINT32 uiLength);
extern ER       Usicd_SendEventBlock(UINT32 uiTranID, UINT16 uiEventCode, UINT16 uiParamCount, UINT32 *pParams);
extern ER       Usicd_SendResponseBlock(UINT32 uiTranID, UINT16 uiRespCode, UINT16 uiParamCount, UINT32 *pParams);
extern UINT32   Usicd_GetSICDStringInfo(UINT8 **str);

//@}

#endif
