#ifndef _MI5140_H
#define _MI5140_H
#include "sensor.h"
#define Token(n)    #n

#define SENSOR_LSC_ENABLE ENABLE//DISABLE//
typedef enum _MI5140_MODE
{
    MI5140_INIT     = 0,
    MI5140_VGA      = SENSOR_MODE_640X480,
    MI5140_1280X960 = SENSOR_MODE_1280X960,
    MI5140_STILL    = SENSOR_MODE_FULL,
    MI5140_2XHP = SENSOR_MODE_FULL+1,
    MI5140_PLL,
    MI5140_CapFULL,
    MI5140_Chart ,
    MI5140_K28A_Rev2_1,
    MI5140_K28A_Rev2_2,
    MI5140_K28A_Rev2_3,
    MI5140_K28A_Rev2_4,
    MI5140_K28A_Rev2_5,
    MI5140_K28A_Rev2_6,
    MI5140_K28A_Rev2_7,
    MI5140_K28A_Rev2_8,
    MI5140_K28A_Rev2_9,
    MI5140_K28A_Rev2_10,
    MI5140_LSC ,
    ENUM_DUMMY4WORD(MI5140_MODE)
}MI5140_MODE;

extern void MI5140_GetRegInfo(MI5140_MODE mode,UINT32* addr,UINT32* size);
extern void MI5140_GetSigInfo(MI5140_MODE mode,UINT32* addr,UINT32* size);
extern void MI5140_GetDataInfo(MI5140_MODE mode,UINT32* addr,UINT32* size);
extern void MI5140_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);

#endif
