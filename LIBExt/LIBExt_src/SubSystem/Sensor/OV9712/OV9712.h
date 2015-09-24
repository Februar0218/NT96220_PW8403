#ifndef _OV9712_H
#define _OV9712_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _OV9712_MODE
{
    OV9712_INIT         = 0,
    OV9712_1440X1080    = SENSOR_MODE_1440X1080,
    OV9712_1920X1080    = SENSOR_MODE_1920X1080,
    OV9712_STILL        = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(OV9712_MODE)
}OV9712_MODE;

extern void OV9712_GetRegInfo(OV9712_MODE mode,UINT32* addr,UINT32* size);
extern void OV9712_GetSigInfo(OV9712_MODE mode,UINT32* addr,UINT32* size);
extern void OV9712_GetDataInfo(OV9712_MODE mode,UINT32* addr,UINT32* size);
extern void OV9712_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
