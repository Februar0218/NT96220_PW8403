/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       ExifVM.h
    @ingroup    mIAVJPEG

    @brief To Create a voice memo inside original EXIF2.2 Image.

*/

#ifndef _EXIFVM_H
#define _EXIFVM_H

#include "Type.h"
#include "ErrorNo.h"

/** \addtogroup mIAVJPEG
*/
//@{

/**
    EXIFVM append set.
*/
typedef struct
{
    UINT8   *puiJPGAddr;            ///< JPG address
    INT32   JPGSize;                ///< JPG size
    UINT8   *puiWAVAddr;            ///< WAV address
    INT32   WAVHeaderSize;          ///< WAV header size
    INT32   WAVSize;                ///< WAV size
    UINT8   *puiOutAddr;            ///< Output address (For JPG with WAV in APP2)
    UINT32  *puiOutSize;            ///< Output buffer size
} EXIFVM_APPEND_SET, *PEXIFVM_APPEND_SET;

/**
    EXIFVM extract set.
*/
typedef struct
{
    UINT8   *puiJPGAddr;            ///< JPG address
    INT32   JPGSize;                ///< JPG size
    UINT8   *puiOutAddr;            ///< Output address (For WAV)
    INT32   *puiOutSize;            ///< Output buffer size
} EXIFVM_EXTRACT_SET, *PEXIFVM_EXTRACT_SET;

/**
    EXIF APP2 link.
*/
typedef struct
{
    UINT32  uiAPP2Addr;             ///< APP2 address
    INT32   APP2Size;               ///< APP2 size, including Tag.
} EXIF_APP2_LINK, *PEXIF_APP2_LINK;

/**
    EXIFVM delete set.
*/
typedef struct
{
    UINT8           *puiJPGAddr;    ///< JPG address
    INT32           JPGSize;        ///< JPG size
    EXIF_APP2_LINK  *pApp2Link;     ///< EXIF APP2 Link
} EXIFVM_DELETE_SET, *PEXIFVM_DELETE_SET;

extern ER   exifvm_appendVM(PEXIFVM_APPEND_SET pAppend);
extern ER   exifvm_extractVM(PEXIFVM_EXTRACT_SET pExtract);
extern ER   exifvm_deleteVM(PEXIFVM_DELETE_SET pDelete);
extern UINT32 exifvm_isVMExist(UINT32 uiJPGAddr);

//@}

#endif
