#ifndef _USBCUSTOMCMD_H
#define _USBCUSTOMCMD_H
#include "Type.h"

extern UINT32 UsbCustomVerdorCmdCB(UINT8 bReguest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8 *bBuf);

#endif