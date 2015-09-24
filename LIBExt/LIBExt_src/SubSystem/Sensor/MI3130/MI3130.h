/*
    MI3130 Driver Interface header file

    @Sensor     Micron MI3130
    @file       MI3130.h
    @ingroup        Sensor\MI3130
    @note       Nothing.

    Porting by Randy Ho   2012/03/08

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _MI3130_H
#define _MI3130_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _MI3130_MODE
{
    MI3130_INIT = 0,
    MI3130_VGA = SENSOR_MODE_640X480,
    MI3130_XGA = SENSOR_MODE_1024X768,  // According to SPEC, XGA(1024*768) Programmable up to 30fps
    MI3130_HD = SENSOR_MODE_1280X720,
    MI3130_STILL  = SENSOR_MODE_FULL,   // According to SPEC, QXGA(2048*1536) programmable up to 15fps
    ENUM_DUMMY4WORD(MI3130_MODE)
}MI3130_MODE;

extern void MI3130_GetRegInfo(MI3130_MODE mode,UINT32* addr,UINT32* size);
extern void MI3130_GetSigInfo(MI3130_MODE mode,UINT32* addr,UINT32* size);
extern void MI3130_GetDataInfo(MI3130_MODE mode,UINT32* addr,UINT32* size);
extern void MI3130_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
