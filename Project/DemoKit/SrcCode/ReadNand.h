//#NT#2009/01/06#Chris Hsu -begin
//#NT#Re-write the function to prevent command task hang and improve performance
/**
    Header file for dump Nand data to storage card

    This file is the header file for dumping nand data to storage card

    @file       ReadNand.h
    @ingroup    DumpNandData
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _READNAND_H
#define _READNAND_H

/**
    @addtogroup DumpNandData
*/
//@{

#define READNAND_MDATASIZE_ALL      0
#define READNAND_MDATASIZE_1MB      0x00100000
#define READNAND_MDATASIZE_2MB      0x00200000
#define READNAND_MDATASIZE_4MB      0x00400000
#define READNAND_MDATASIZE_8MB      0x00800000
#define READNAND_MDATASIZE_16MB     0x01000000
#define READNAND_MDATASIZE_32MB     0x02000000

// Extern function
extern void ReadNand_Total(UINT32 uiMainDataSize);

#endif

//@}
//#NT#2009/01/06#Chris Hsu -end
