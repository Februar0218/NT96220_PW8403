/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       BuildDateInfo.h
    @ingroup    mIPRJAPCfg

    @brief      Project source build time information
                Project source build time information

    @note       The project source build time info will be updated every time you build the image.

    @date       2007/11/06
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _BUILD_DATE_INFO_H
#define _BUILD_DATE_INFO_H

#include "Type.h"


extern CHAR* Prj_GetBuildDateStd(void);
extern CHAR* Prj_GetBuildDateUser(void);

#endif
//@}
