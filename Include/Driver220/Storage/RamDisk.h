/**
    Header file of RAM disk drvier.

    @file       RamDisk.h
    @ingroup    mICardRAM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.
*/

#ifndef _RAMDISK_H
#define _RAMDISK_H

#include "StrgDef.h"

/**
    @addtogroup mICardRAM
*/
//@{

#define RAMDSK_BYTE_PER_SECTOR          512
#define RAMDSK_SECTORBITS               9       //512=2^9

typedef struct {
    UINT32 baseaddr;
    UINT32 size;
} RamDiskSettings;

extern ER ramdsk_init(RamDiskSettings *pramset);
extern ER ramdsk_open(void);
extern ER ramdsk_writeSectors(CHAR *, ULONG, ULONG);
extern ER ramdsk_readSectors(CHAR *, ULONG, ULONG);
extern ER ramdsk_format(void);
extern ER ramdsk_close(void);
extern PSTRG_TAB ramdsk_getStorageObject(void);

extern ER ramdsk2_init(RamDiskSettings *pramset);
extern ER ramdsk2_open(void);
extern ER ramdsk2_writeSectors(CHAR *, ULONG, ULONG);
extern ER ramdsk2_readSectors(CHAR *, ULONG, ULONG);
extern ER ramdsk2_format(void);
extern ER ramdsk2_close(void);
extern PSTRG_TAB ramdsk2_getStorageObject(void);
//@}

#endif // _RAMDISK_H
