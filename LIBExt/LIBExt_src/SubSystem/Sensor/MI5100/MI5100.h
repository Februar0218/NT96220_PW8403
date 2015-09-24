#ifndef _MI5100_H
#define _MI5100_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _MI5100_MODE
{
    MI5100_INIT     = 0,
    MI5100_VGA      = SENSOR_MODE_640X480,
    MI5100_1280X960 = SENSOR_MODE_1280X960,
    MI5100_STILL    = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(MI5100_MODE)
}MI5100_MODE;

extern void MI5100_GetRegInfo(MI5100_MODE mode,UINT32* addr,UINT32* size);
extern void MI5100_GetSigInfo(MI5100_MODE mode,UINT32* addr,UINT32* size);
extern void MI5100_GetDataInfo(MI5100_MODE mode,UINT32* addr,UINT32* size);
extern void MI5100_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
