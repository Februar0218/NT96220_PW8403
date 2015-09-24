/*
    @file       xD.h
    @ingroup    mIStgNAND

    @brief      Smartmedia Card driver export variables and function prototypes.

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

*/


#ifndef _XD_H
#define _XD_H

#include "Type.h"
#include "StrgDef.h"
#include "NAND.h"

/*
@addtogroup mIStgNAND
*/
//@{

// ------------------------------------------------------------------------
// xD driver error
// ------------------------------------------------------------------------
//bit 31...28    Card type xD : 1
//bit 27...24    Card manufacture
//bit 23...12    xD error
//bit 11...0     NAND / SMC /xD error
#define XD_ERROR_OK                         0x10000000

//xD error
#define XD_ERROR_INSUFFICIENT_FREE_BLOCK    0x10010000
#define XD_ERROR_CORRUPT_CIS                0x10020000
#define XD_ERROR_DOUBLE_MAPPING             0x10040000
#define XD_ERROR_TOO_MANY_BAD_UNITS         0x10080000 //Too many bad blocks result in write protection rasen.(write and erase is prohibited)
#define XD_ERROR_BAD_CARD_ID                0x10100000
#define XD_ERROR_BAD_DEVICE_ID              0x10200000
#define XD_ERROR_RESERVED1                  0x10400000 //reserved
#define XD_ERROR_RESERVED2                  0x10800000 //reserved

//Card manufacture
#define XD_CARD_MANUFACTURE_OLYMPUS         0x11000000
#define XD_CARD_MANUFACTURE_FUJIFILM        0x12000000
#define XD_RESERVED3                        0x14000000 //reserved
#define XD_CARD_MANUFACTURE_UNKNOWN         0x18000000


//NAND / SMC /xD common error (Please keep below error code number the same with NAND driver (NAND.h))
//Data error
#define XD_ERROR_DATA_STATUS                0x10000001 //Data status is 0x00
#define XD_ERROR_ECC_FIELD1                 0x10000002
#define XD_ERROR_ECC_FIELD2                 0x10000004
#define XD_RESERVED4                        0x10000008 //reserved

//Operation error
#define XD_ERROR_READ_ERROR                 0x10000010
#define XD_ERROR_WRITE_ERROR                0x10000020
#define XD_ERROR_DEVICE_ERROR               0x10000040
#define XD_RESERVED6                        0x10000080 //reserved

#define XD_MANUFACTURE_MASK                 0x0F000000 //Mask the bit of manufacture


//Card Type
#define XD_TPTE_H_MPLUS                     0xC3

// ------------------------------------------------------------------------
// extern functions
// ------------------------------------------------------------------------
typedef void (*PrFunc) (BOOL);

/** Exported object for filesystem*/
extern PSTRG_TAB xd_getStorageObject(void);

/*
extern ER        xd_open(void);
extern ER        xd_close(void);
extern ER        xd_writeSectors(CHAR *pcBuf, ULONG ulLBAddr, ULONG ulSctCnt);
extern ER        xd_readSectors(CHAR *pcBuf, ULONG ulLBAddr, ULONG ulSctCnt);
extern ER        xd_format(void);
extern ER        xd_getLastError(void);
extern UINT32    xd_getValidSectorNum(void);
*/

extern void      xd_xDCardInserted(void);
extern void      xd_xDCardRemoved(void);
extern void      xd_initCardIOFunc(PrFunc CardPowIOFunc);
extern BOOL      xd_getDeviceInfo(PNandXdInfoStruct pNandInfo);
//@}

#endif /* _XD_H */