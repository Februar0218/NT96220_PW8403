/**
    Header file of kernel version and build date information.

    @file       KerVerInfo.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _KERVERINFO_H
#define _KERVERINFO_H

#include "Type.h"

/**
    @addtogroup mIKernelCore
*/
//@{

extern CHAR     *Ker_GetVerInfo(void);
extern CHAR     *Ker_GetBuildDate(void);

//@}

#endif
