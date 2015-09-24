#ifndef _MI1310_H
#define _MI1310_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _MI1310_MODE
{
    MI1310_INIT     = 0,
    MI1310_VGA      = SENSOR_MODE_640X480,
    MI1310_1280X960 = SENSOR_MODE_1280X960,
    MI1310_1440X1080= SENSOR_MODE_1280X480,
    MI1310_STILL    = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(MI1310_MODE)
}MI1310_MODE;

extern void MI1310_GetRegInfo(MI1310_MODE mode,UINT32* addr,UINT32* size);
extern void MI1310_GetSigInfo(MI1310_MODE mode,UINT32* addr,UINT32* size);
extern void MI1310_GetDataInfo(MI1310_MODE mode,UINT32* addr,UINT32* size);
extern void MI1310_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
