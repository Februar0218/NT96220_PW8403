/*
    Copyright   Novatek Microelectronics Corp. 2006.  All rights reserved.

    @file       AppCommon.h
    @ingroup    mIAPP

    @brief      Application Common header file
                Application Common header file

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2006/07/21
*/

/** \addtogroup mIAPP */
//@{

#ifndef _APPCOMMON_H
#define _APPCOMMON_H

#include "Type.h"

typedef struct _IMEIDE_ATTR
{
    UINT32  uiImeOutH;  // IME output width (For display)
    UINT32  uiImeOutV;  // IME output height (For display)
    UINT32  uiIdeOutH;  // IDE output width
    UINT32  uiIdeOutV;  // IDE output height
    UINT32  uiHratio;   // IDE output width ratio //2011/03/10 Meg Lin
    UINT32  uiVratio;   // IDE output height ratio //2011/03/10 Meg Lin
} IMEIDE_ATTR, *PIMEIDE_ATTR;

#endif

//@}