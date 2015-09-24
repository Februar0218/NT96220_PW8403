#ifndef _OV9712_H
#define _OV9712_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _H22_MODE
{
    H22_INIT         = 0,
    H22_1440X1080    = SENSOR_MODE_1440X1080,
    H22_1920X1080    = SENSOR_MODE_1920X1080,
    H22_STILL        = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(H22_MODE)
}H22_MODE;

extern void H22_GetRegInfo(H22_MODE mode,UINT32* addr,UINT32* size);
extern void H22_GetSigInfo(H22_MODE mode,UINT32* addr,UINT32* size);
extern void H22_GetDataInfo(H22_MODE mode,UINT32* addr,UINT32* size);
extern void H22_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
