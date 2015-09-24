/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       VendorExif.h
    @ingroup    mIAVJPEG

    @brief      Vendor EXIF Setting Header Declaration.
                Customers can use this file declaration to change the default setting tag value in EXIF header.
                This header file should be exported to our customers.
*/

#ifndef _VENDOREXIF_H
#define _VENDOREXIF_H

#include <Type.h>

/** \addtogroup mIAVJPEG
*/
//@{

/**
  Customers can set the value in the structure Vendor_EXIF_HEAD to change the following setting
  1. Maker String. (32 bytes, NULL for default string.)
  2. Model String. (16 bytes, NULL for default string)
  3. Software Version String. (20 bytes, NULL for default string)
  4. MakeNote String. (128 bytes.)
**/
typedef struct {
    CHAR     *pMakerString;
    CHAR     *pModelString;
    CHAR     *pSoftwareVer;
    CHAR     *pMakeNote;
    //Reason: add one interface for setting image description
    CHAR     *pImageDescription;
    //#NT#2010/01/29#Hideo Lin -begin
    //#Add User Comment tag
    CHAR     *pUserComment;
    //#NT#2010/01/29#Hideo Lin -end
}Vendor_EXIF_HEAD, *PVendor_EXIF_HEAD;


//
// exported Function
//
ER JpgSetEXIFString(PVendor_EXIF_HEAD pCustomerEXIF);


//#NT#2008/03/28#Jades Shih - Begin

// structure for user to define their own EXIF Tags
typedef struct {
    USHORT       usTag;
    USHORT       usType;
    ULONG        ulCount;
    ULONG        ulValOffset;
    UCHAR        *pData;
}ExifInfo, *pExifInfo;

// structure for user to define their own EXIF header
typedef struct {
    pExifInfo    Table0th;
    UINT         TagNum0th;
    UINT         Size0th;
    pExifInfo    TableExif;
    UINT         TagNumExif;
    UINT         SizeExif;
#if _Support_GPSInfo_Tag
    pExifInfo    TableGPS;
    UINT         TagNumGPS;
    UINT         SizeGPS;
#endif
    pExifInfo    TableInter;
    UINT         TagNumInter;
    UINT         SizeInter;
    pExifInfo    Table1th;
    UINT         TagNum1th;
    UINT         Size1th;
//    ULONG        ulOffset1th;
}ExifTable, *pExifTable;


// exported function for vendor to register their own EXIF

//#NT#2008/06/11#Jades Shih - Begin
// add Callback function
typedef void (*VENDORCB)(void);

/**
    Register the vendor EXIF header structure

    @param pExifInfoTable : the vendor defined EXIF Table
    @param uType :
           0. no hidden image
           1. Hidden image : 320x240, hidden in maker note
           2. Screen nail : 640x480, saved after main JPEG image,
              the offset & size stored in maker note
           3. Screen nail : 640x480, saved in APP2
    @param pCallback : set the callback function for modifing vendor Exif
           at the beginning of JpegCopyVendorEXIF()
    @return : Success returns 1. Failure returns 0.
*/
extern BOOL SetVendorExif(pExifTable pExifInfoTable, UINT uType, VENDORCB pCallback);
//#NT#2008/06/11#Jades Shih - End

/**
    Set the designated tag value. (only set one tag in one IFD)
    This function must be called after all other Set###IFD() functions !!!
    Because we need the correct offset value...

    @param EXIFAddrs : start address of EXIF header (FF D8 ...)
    @param uiIFG : which IFD to set
    @param pExifInfoTag : the address of ExifInfo
    @param ChangeOffset : change the value of "ulValOffset" or not
    @return : Success returns 1. Failure returns 0.
*/
extern BOOL SetTagVal(UINT32 EXIFAddrs, UINT uiIFG, pExifInfo pExifInfoTag, BOOL ChangeOffset);

/**
    Set the MakerNote value.
    The data is written in byte !!

    @param EXIFAddrs : start address of EXIF header (FF D8 ...)
    @param uiOffsetByte : write from which bytes
    @param uiNumByte : how many bytes to write
    @param pData : the data array to be written
    @return : Success returns 1. Failure returns 0.
*/
extern BOOL SetMakerNoteString(UINT32 EXIFAddrs, UINT uiOffsetByte, UINT uiNumByte, UCHAR *pData);

//#NT#2008/03/28#Jades Shih - End


//@}

#endif
