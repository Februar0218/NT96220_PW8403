/**
    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.

    @file       UserPStore.h
    @ingroup    mIPRJAPCfg

    @brief      User defined PStore section handle functions
                User defined PStore section handle functions

    @note       Nothing.

    @date       2010/05/18
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _USER_PSTORE_H
#define _USER_PSTORE_H

#include "PStore.h"



/**
    UserPStore error code
*/
#define E_USERPS_OK             0x00            ///< Normal
#define E_USERPS_FILE           0x01            ///< File error
#define E_USERPS_PSECTION       0x02            ///< PStore section handle error


extern UINT8*   UserPS_InitUniFontData(void);
extern UINT32   UserPS_WriteUniFontFile(CHAR *pFilename);


#endif

//@}
