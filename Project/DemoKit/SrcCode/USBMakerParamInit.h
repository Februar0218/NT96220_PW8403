#ifndef _USBPARAM_INIT_H_
#define _USBPARAM_INIT_H_

#include    "usb.h"
#include    "UMSDC.h"
#include    "USBPCC.h"
//#NT#2009/03/24#Marx Chiu -begin
#include    "Usicd.h"
//#NT#2009/03/24#Marx Chiu -end

void USBMakerInit_UMSD(USB_MSDC_INFO *pgUSBClassInfo);
void USBMakerInit_UPCC(USB_PCC_INFO *pgUSBClassInfo);

//#NT#2009/03/24#Marx Chiu -begin
//MarxChiu_20090324 void USBMakerInit_USICD(USB_SICD_INFO *pgUSBClassInfo);
void USBMakerInit_USICD(USB_SICD_INFO *pgUSBClassInfo);
//#NT#2009/03/24#Marx Chiu -end


#endif  // _USBPARAM_INIT_H_