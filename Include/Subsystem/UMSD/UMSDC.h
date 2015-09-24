/**
    @file       UMSDC.h
    @ingroup    mISYSUMSD

    @brief      Header file for USB MSDC Task APIs .

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _UMSDC_H
#define _UMSDC_H

#include "Type.h"
#include "StrgDef.h"
#include "usb_desc.h"
#include "usb_define.h"
//#NT#2010/04/20#Klins Chen -begin
#include "sdio.h"
//#NT#2010/04/20#Klins Chen -end

/** \addtogroup mISYSUMSD */
//@{

// MSDC state
#define  MSDC_COMMANDOK          0x0
#define  MSDC_CLOSED_STATE       0x1
#define  MSDC_USBRESETED         0x2
#define  MSDC_PHASEERROR         0x3
#define  MSDC_READ_BUSY_STATE    0x4
#define  MSDC_WRITE_BUSY_STATE   0x5

/**
    Structure used for MSDC vendor command callback functions
*/
typedef struct
{
    UINT32    OutDataBufLen;          ///< PC->Device data buffer length

    UINT32    InDataBuf;              ///< Device->PC data buffer, MSDC should prepare this buffer for callback function.
    UINT32    InDataBufLen;           ///< PC wants how many bytes for In buffer data.

    UINT32    VendorCmdBuf;           ///< Vendor CBW buffer that device will receive from host for CBW buffer. Fixed 31 bytes

    UINT32    VendorCSWBuf;           ///< Vendor CSW buffer that device will return to host for CSW buffer, MSDC task should prepare this buffer for callback function, Fixed 13 bytes.

} MSDCVendorParam, *PMSDCVendorParam;


typedef void (*MSDC_Vendor_CB)(PMSDCVendorParam pBuf);
typedef BOOL (*MSDC_Verify_CB)(UINT32 pCmdBuf, UINT32 *pDataBuf);

typedef BOOL (*MSDC_StorageDet_CB)(void);
typedef BOOL (*MSDC_StrgLockDet_CB)(void);
typedef void (*MSDC_Led_CB)(void);
//#NT#2010/04/20#Klins Chen -begin
typedef PSDIO_MISC_INFORMATION (*MSDC_getSdInfo_CB)(void);
//#NT#2010/04/20#Klins Chen -end

typedef enum
{
    MSDC_STRG = 0,
    MSDC_CDROM
}MSDC_TYPE;

/**
    Information needed for USB MSDC Task.
*/
typedef struct {
    MSDC_TYPE           msdc_type[MAX_LUN];     ///< MSDC_STRG or MSDC_CDROM

    UINT32              uiMsdcBufAddr;          ///< Buffer address for USB operation
    UINT32              uiMsdcBufSize;          ///< Buffer size for USB operation

    USB_STRING_DESC     *pManuStringDesc;       ///< USB Manufacturer String Descriptor ptr
    USB_STRING_DESC     *pProdStringDesc;       ///< USB Product String Descriptor ptr
    USB_STRING_DESC     *pSerialStringDesc;     ///< USB Serial Number String Descriptor ptr
    UINT8               *pInquiryData;          ///< SCSI Inquire data
    UINT16              VID;                    ///< Customized USB Vendor ID
    UINT16              PID;                    ///< Customized USB Product ID

    MSDC_Verify_CB      msdc_check_cb;          ///< CB function to check if the command is a vendor command
    MSDC_Vendor_CB      msdc_vendor_cb;         ///< Vendor command CB function

    UINT8               LUNs;                   ///< Number of LUNs supported by this device
    PSTRG_TAB           pStrgDevObj[MAX_LUN];         ///< Pointer of each LUN's storage objects
    MSDC_StorageDet_CB  msdc_storage_detCB[MAX_LUN];  ///< CB function to detect storage plug status, if there is none, please set to NULL
    MSDC_StrgLockDet_CB msdc_strgLock_detCB[MAX_LUN]; ///< CB function to detect storage lock status, if there is none, please set to NULL

    // Add to Oly-Project backward compatible
    MSDC_Led_CB         msdc_RW_Led_CB;               ///< callback when RW storage is running.
    MSDC_Led_CB         msdc_StopUnit_Led_CB;         ///< callback when host issue StopUnit cmd.
    MSDC_Led_CB         msdc_UsbSuspend_Led_CB;       ///< callback when host issue usb suspend.

    //#NT#2010/04/20#Klins Chen -begin
    MSDC_getSdInfo_CB   msdc_sdioInfo_CB[MAX_LUN];
    //#NT#2010/04/20#Klins Chen -end

} USB_MSDC_INFO;

extern ER     USBMSDC_Open(USB_MSDC_INFO *pClassInfo);
extern void   USBMSDC_Close(void);
extern UINT32 USBMSDC_GetStatus(void);
extern void   USBMSDC_ForceLUNToEject(UINT8 LUN, BOOL EjectorNot);
extern void   USBMSDC_SetLUNWriteProtected(UINT8 LUN,BOOL ifWriteProtect);
extern BOOL   USBMSDC_CheckIdle(void);
extern void   USBMSDC_ChgVenInBufAddr(UINT32 uiVenInBufAddr);


//
// Add Oly-Project backward compatible APIs
//
extern void     USBMSDC_ForceStorageToEject(BOOL EjectorNot);
extern BOOL     USBMSDC_GetStorageEject(void);
//#NT#2009/12/23#Klins Chen -begin
//#NT#Support chnage the storage object when storage detached
extern void     USBMSDC_chgStrgObject(UINT32 uiLun, STRG_TAB *pStrgDev, MSDC_StorageDet_CB DetStrgCB, MSDC_StrgLockDet_CB DetLockCB);
//#NT#2009/12/23#Klins Chen -end

//@}

#endif //_UMSDC_H
