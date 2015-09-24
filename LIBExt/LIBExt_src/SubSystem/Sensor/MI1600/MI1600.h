#ifndef _MI1600_H
#define _MI1600_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _MI1600_MODE
{
    MI1600_INIT     = 0,
    MI1600_INIT2    = 0XFFFF,
    MI1600_VGA      = SENSOR_MODE_640X480,
    MI1600_1280X720 = SENSOR_MODE_1280X720,
    MI1600_1440X1080= SENSOR_MODE_1440X1080,
    MI1600_1440X540 = SENSOR_MODE_1440X540,
    MI1600_STILL    = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(MI1600_MODE)
}MI1600_MODE;

extern void MI1600_GetRegInfo(MI1600_MODE mode,UINT32* addr,UINT32* size);
extern void MI1600_GetSigInfo(MI1600_MODE mode,UINT32* addr,UINT32* size);
extern void MI1600_GetDataInfo(MI1600_MODE mode,UINT32* addr,UINT32* size);
extern void MI1600_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
