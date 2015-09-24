#ifndef _MI5130_H
#define _MI5130_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _MI5130_MODE
{
    MI5130_INIT     = 0,
    MI5130_VGA      = SENSOR_MODE_640X480,
    MI5130_1280X960 = SENSOR_MODE_1280X960,
    MI5130_STILL    = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(MI5130_MODE)
}MI5130_MODE;

extern void MI5130_GetRegInfo(MI5130_MODE mode,UINT32* addr,UINT32* size);
extern void MI5130_GetSigInfo(MI5130_MODE mode,UINT32* addr,UINT32* size);
extern void MI5130_GetDataInfo(MI5130_MODE mode,UINT32* addr,UINT32* size);
extern void MI5130_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
