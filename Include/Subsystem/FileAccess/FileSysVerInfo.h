/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       FileSysVerInfo.h
    @ingroup

    @brief      Header file of File system version and build date information
                Header file of File system version and build date information

    @note       Nothing.

    @version    V1.00.000
    @author     Janice Huang
    @date       2010/12/14
*/

/** \addtogroup */
//@{

#ifndef _FILESYSVERINFO_H
#define _FILESYSVERINFO_H

#include "Type.h"

extern UINT8 *FileSys_GetVerInfo(void);
extern UINT8 *FileSys_GetBuildDate(void);

#endif

//@}

