/**
    @file       PSDrv.h
    @ingroup    mIStgNAND

    @brief      PStore low level driver export variables and function prototypes.

    Copyright   Novatek Microelectronics Corp. 2006.  All rights reserved.

*/


//#NT#2007/03/16#CL Wang -begin
#ifndef _PS_DRV_H
#define _PS_DRV_H
#include "StrgDef.h"
//#NT#2007/03/16#CL Wang -end
/**
@addtogroup mIStgNAND
*/
//@{
#define PSTORE_HEAD_SECTION  0

//#NT#2007/03/16#CL Wang -begin
extern PSSTRG_TAB* nandps_getStorageObject(void);
//#NT#2007/03/16#CL Wang -end

//#NT#2007/05/21#CL Wang -begin
extern PSSTRG_TAB* norps_getStorageObject(void);
//#NT#2007/05/21#CL Wang -end
//@}

#endif
