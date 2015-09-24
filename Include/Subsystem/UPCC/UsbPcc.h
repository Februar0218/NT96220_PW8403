/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       USBPCC.h
    @ingroup    mISYSUPCC

    @brief      USB PCC device class API header.
                USB PCC device class API header.

*/

#ifndef _USBPCC_H
#define _USBPCC_H

/*-----------------------------------
    Includes
-----------------------------------*/
#include "Type.h"
#include "USB.h"

//#NT#2007/04/20#KH Chang -begin
//#Move this definition to UsbvideoTsk.h
//#define USB_VIDEO_CLASS ENABLE
//#NT#2007/04/20#KH Chang -end

/**
    \addtogroup mISYSUPCC
@{
*/

/**
    USB Infomation Table for PCC.
*/
typedef enum {
    PCC_MAX_SIZE_VGA,
    PCC_MAX_SIZE_HD
} PCC_MAX_SIZE;   //TODO220: Temporary add only for compile issue

typedef struct {

    USB_STRING_DESC     *pManuStringDesc;    ///< USB Manufacturer String Descriptor ptr
    USB_STRING_DESC     *pProdStringDesc;    ///< USB Product String Descriptor ptr
    USB_STRING_DESC     *pSerialStringDesc;  ///< USB Serial Number String Descriptor ptr
    UINT16              VID;                 ///< Customized USB Vendor ID
    UINT16              PID;                 ///< Customized USB Product ID
    UINT32              UsbPccMemAdr;        ///< Buffer address for USB operation
    UINT32              UsbPccMemSize;       ///< Buffer size for USB operation
    //#NT#2009/07/09#Chris Chung -begin
    //#NT#add to control PCC vendor function
    BOOL                bEnableVendorFunc;   //TODO220: Temporary add only for compile issue
    //#NT#2009/07/09#Chris Chung -end
    //#NT#2011/01/05#Hideo Lin -begin
    //#NT#For PCC maximum size setting
    PCC_MAX_SIZE        PccMaxSize;          //TODO220: Temporary add only for compile issue
    //#NT#2011/01/05#Hideo Lin -end

    //******************************************************************************
    // Flash CallBack, Feb 10, 2006 Added.
    //******************************************************************************
    UINT32    (*BurnIn)(UINT32 BlockID, UINT8 *Buf, UINT32 Length);   //CallBack Function for Nane Burn-In
    UINT32    (*IsFinish)(void);                                      //CallBack Function for Check Ready
    UINT32    (*FlashRead)(UINT32 BlockID, UINT32 Offset, UINT32 *Size, UINT8 *RBuf);      //CallBack Function for flash read

    //#NT#2008/08/14#Irene -begin
    //#NT#CallBack Function for Vendor Request
    // New:
    // 1. Add a callback function for vendor request
    UINT32    (*VendorProcess)(UINT8 bReguest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8 *bBuf);
    //#NT#2008/08/14#Irene -end

} USB_PCC_INFO;

//TODO220: Temporary add only for compile issue
//#NT#2009/07/03#Chris Chung -begin
//#NT#add for LSC PC calibration
typedef void (*FPSETPCC2CAPRAWCB)(UINT *rawAddr, UINT *rawWidth, UINT *rawHeight, UINT *rawBitDepth, UINT *jpgAddr, UINT *jpgSize);
typedef void (*FPSETPCC2CAPCB)(UINT32 *jpgAddr, UINT32 *jpgSize);
//#NT#2009/07/03#Chris Chung -end

//For compatible
#define UPccRegisterVendorCmd(startReq, endReq)   USBPccRegisterVendorCmd(startReq, endReq)


extern UINT     UPccOpen(USB_PCC_INFO *pClassInfo);
extern void     UPccClose(void);
extern void     USBPccVideoTsk(void);

//TODO220: Temporary add only for compile issue
//#NT#2009/07/03#Chris Chung -begin
//#NT#add for LSC PC calibration
extern void     USBPccRegSetPCC2CapRawCB(FPSETPCC2CAPRAWCB fp);
extern void     USBPccRegSetPCC2CapCB(FPSETPCC2CAPCB fp);
//#NT#2009/07/03#Chris Chung -end
extern ER       UPccRegisterVendorCmd(UINT8 startReq, UINT8 endReq);

//@}

#endif  // _USBPCC_H

