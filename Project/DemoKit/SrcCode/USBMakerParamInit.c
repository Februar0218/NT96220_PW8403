#include    <string.h>
#include    "USBMakerParamInit.h"

__align(4) const UINT16 myUSBManuStrDesc[] =
{
    0x0310,                             // 10: size of String Descriptor = 16 bytes
                                        // 03: String Descriptor type
    'N', 'O', 'V', 'A', 'T', 'E', 'K'   // NOVATEK
};

__align(4) const UINT16 myUSBSICDProdStrDesc[] =
{
    0x0336,                             // 38: size of String Descriptor = 56 bytes
                                        // 03: String Descriptor type
    'D', 'i', 'g', 'i', 't', 'a',       // Digital Camera
    'l', ' ', 'C', 'a', 'm', 'e',
    'r', 'a', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' '
};

//Serial number string descriptor, the content should be updated according to serial number
__align(4) UINT16 myUSBSerialStrDesc3[] =
{
    0x0320,                             // 20: size of String Descriptor = 32 bytes
                                        // 03: String Descriptor type
    '9', '6', '2', '2', '0',            // 96220-00000-001 (default)
    '0', '0', '0', '0', '0',
    '0', '0', '1', '0', '0'
};

__align(4) const UINT8 myUSBSICDString[] =
{
    0x09, // Manufacturer (String), Num of Chars
    'N', 0x00, // N
    'o', 0x00, // o
    'v', 0x00, // v
    'a', 0x00, // a
    't', 0x00, // t
    'e', 0x00, // e
    'k', 0x00, // k
    0x20, 0x00, //
    0x00, 0x00, // NULL

    0x08, // Model (String), Num of Chars
    'C', 0x00, // C
    'a', 0x00, // a
    'm', 0x00, // m
    'e', 0x00, // e
    'r', 0x00, // r
    'a', 0x00, // a
    0x20, 0x00, //
    0x00, 0x00, // NULL

    0x05, // DeviceVersion (String), Num of Chars
    0x31, 0x00, // 1
    0x2E, 0x00, // .
    0x30, 0x00, // 0
    0x30, 0x00, // 0
    0x00, 0x00, // NULL
    0x00 // SerialNumber (String), Num of Chars
};

__align(4) const UINT16 myUSBMSDCProdStrDesc[] =
{
    0x0336,                             // 36: size of String Descriptor = 54 bytes
                                        // 03: String Descriptor type
    'U', 'S', 'B', ' ', 'M', 'a',       // USB Mass Storage
    's', 's', ' ', 'S', 't', 'o',
    'r', 'a', 'g', 'e', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' '
};

__align(4) const UINT16 myUSBPCCProdStrDesc[] =
{
    0x0336,                             // 36: size of String Descriptor = 54 bytes
                                        // 03: String Descriptor type
    'U', 'S', 'B', ' ', 'P', 'C',       // USB PC Camera
    ' ', 'C', 'a', 'm', 'e', 'r',
    'a', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' '
};

//#NT#2007/03/29#Todd Tsai -begin
//#__align(16) removed
UINT8 InquiryData[36]=
//#NT#2007/03/29#Todd Tsai -end
{
    0x00, 0x80, 0x05, 0x02, 0x20, 0x00, 0x00, 0x00,
//    //Vendor identification, PREMIER
    'D', 'i', 'g', 'i', 't', 'a', 'l', ' ',
//    //product identification, DC8365
    'C', 'a', 'm', 'e', 'r', 'a', ' ', 'S',
    't', 'o', 'r', 'a', 'g', 'e', ' ', ' ',
//    //product revision level, 1.00
    '1', '.', '0', '0'
};

void USBMakerInit_SerialNumber(void)
{
#if 0   //example
    int len, i;

    if (AAA_Adjust.SerialNumber[0])
    {
        len = ((myUSBSerialStrDesc3[0] & 0xff) - 1) / 2;

        //initialize serial number string from store data
        for (i=0; i<len && AAA_Adjust.SerialNumber[i]; i++)
            myUSBSerialStrDesc3[i+1] = AAA_Adjust.SerialNumber[i];
        //clear the reset of string to null
        for (; i<len ; i++)
            myUSBSerialStrDesc3[i+1] = 0;
    }
#endif
}

void USBMakerInit_UMSD(USB_MSDC_INFO *pUSBMSDCInfo)
{
    pUSBMSDCInfo->pManuStringDesc = (USB_STRING_DESC *)myUSBManuStrDesc;
    pUSBMSDCInfo->pProdStringDesc = (USB_STRING_DESC *)myUSBMSDCProdStrDesc;
    USBMakerInit_SerialNumber();
    pUSBMSDCInfo->pSerialStringDesc = (USB_STRING_DESC *)myUSBSerialStrDesc3;

    pUSBMSDCInfo->VID = 0x0603;
    pUSBMSDCInfo->PID = 0x8611;

    pUSBMSDCInfo->pInquiryData = (UINT8*)&InquiryData[0];
}

void USBMakerInit_UPCC(USB_PCC_INFO *pUSBPCCInfo)
{
    pUSBPCCInfo->pManuStringDesc = (USB_STRING_DESC *)myUSBManuStrDesc;
    pUSBPCCInfo->pProdStringDesc = (USB_STRING_DESC *)myUSBPCCProdStrDesc;

    USBMakerInit_SerialNumber();
    pUSBPCCInfo->pSerialStringDesc = (USB_STRING_DESC *)myUSBSerialStrDesc3;

    pUSBPCCInfo->VID = 0x0603;
    pUSBPCCInfo->PID = 0x8612;
}

void USBMakerInit_USICD(USB_SICD_INFO *pUSBSICDInfo)
{
    pUSBSICDInfo->pManuStringDesc = (USB_STRING_DESC *)myUSBManuStrDesc;
    pUSBSICDInfo->pProdStringDesc = (USB_STRING_DESC *)myUSBSICDProdStrDesc;

    USBMakerInit_SerialNumber();
    pUSBSICDInfo->pSerialStringDesc = (USB_STRING_DESC *)myUSBSerialStrDesc3;

    pUSBSICDInfo->VID = 0x0603;
    pUSBSICDInfo->PID = 0x8613;

    pUSBSICDInfo->pSICDString = (char *)&myUSBSICDString;
    pUSBSICDInfo->SICDStringLen = sizeof(myUSBSICDString);
}
