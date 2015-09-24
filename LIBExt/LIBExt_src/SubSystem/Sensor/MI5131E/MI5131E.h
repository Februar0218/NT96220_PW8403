#ifndef _MI5131E_H
#define _MI5131E_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _MI5131E_MODE
{
    MI5131E_INIT     = 0,
    MI5131E_VGA      = SENSOR_MODE_640X480,
    MI5131E_1280X960 = SENSOR_MODE_1280X960,
    MI5131E_STILL    = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(MI5131E_MODE)
}MI5131E_MODE;

extern void MI5131E_GetRegInfo(MI5131E_MODE mode,UINT32* addr,UINT32* size);
extern void MI5131E_GetSigInfo(MI5131E_MODE mode,UINT32* addr,UINT32* size);
extern void MI5131E_GetDataInfo(MI5131E_MODE mode,UINT32* addr,UINT32* size);
extern void MI5131E_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
