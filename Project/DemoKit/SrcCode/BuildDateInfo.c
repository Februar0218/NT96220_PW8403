/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       BuildDateInfo.c
    @ingroup    mIPRJAPCfg

    @brief      Project source build time information
                Project source build time information

    @note       The project source build time info will be updated every time you build the image.

    @date       2007/11/06
*/

/** \addtogroup mIPRJAPCfg*/
//@{

#include <stdio.h>
#include "BuildDateInfo.h"
#include "Type.h"


// C standard format: Mmm dd yyyy HH:MM:SS (ex. Nov 06 2007 11:15:11)
static CHAR gPrjBuildDateStd[] = __DATE__ " " __TIME__;

// User defined format: yyyy/mm/dd HH:MM:SS (ex. 2007/11/06 11:15:11)
static CHAR gPrjBuildDateUser[32];

// Retrieve year info
#define OS_YEAR     ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
                        + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

// Retrieve month info
#define OS_MONTH    (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
                        : __DATE__ [2] == 'b' ? 2 \
                        : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                        : __DATE__ [2] == 'y' ? 5 \
                        : __DATE__ [2] == 'l' ? 7 \
                        : __DATE__ [2] == 'g' ? 8 \
                        : __DATE__ [2] == 'p' ? 9 \
                        : __DATE__ [2] == 't' ? 10 \
                        : __DATE__ [2] == 'v' ? 11 : 12)

// Retrieve day info
#define OS_DAY      ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
                        + (__DATE__ [5] - '0'))

// Retrieve hour info
#define OS_HOUR     ((__TIME__ [0] - '0') * 10 + (__TIME__ [1] - '0'))

// Retrieve minute info
#define OS_MINUTE   ((__TIME__ [3] - '0') * 10 + (__TIME__ [4] - '0'))

// Retrieve second info
#define OS_SECOND   ((__TIME__ [6] - '0') * 10 + (__TIME__ [7] - '0'))




//-----------------------------------------------------------------------------
// Project build time info, C standard (US) format
//-----------------------------------------------------------------------------
char* Prj_GetBuildDateStd(void)
{
    return gPrjBuildDateStd;
}

//-----------------------------------------------------------------------------
// Project build time info, user defined format
//-----------------------------------------------------------------------------
char* Prj_GetBuildDateUser(void)
{
    sprintf(gPrjBuildDateUser, "%d/%02d/%02d %s", OS_YEAR, OS_MONTH, OS_DAY, __TIME__);
    return gPrjBuildDateUser;
}


//@}
