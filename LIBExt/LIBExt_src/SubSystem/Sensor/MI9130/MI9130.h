#ifndef _MI9130_H
#define _MI9130_H
#include "sensor.h"
#define Token(n)    #n
typedef enum _MI9130_MODE
{
    MI9130_INIT     = 0,
    MI9130_VGA      = SENSOR_MODE_640X480,
    MI9130_960X720 = SENSOR_MODE_960X720,
    MI9130_1280X960 = SENSOR_MODE_1280X960,
    MI9130_STILL    = SENSOR_MODE_FULL,
    ENUM_DUMMY4WORD(MI9130_MODE)
}MI9130_MODE;

#define DEFAULT_MSHUTTER_TIME  2750

extern void MI9130_GetRegInfo(MI9130_MODE mode,UINT32* addr,UINT32* size);
extern void MI9130_GetSigInfo(MI9130_MODE mode,UINT32* addr,UINT32* size);
extern void MI9130_GetDataInfo(MI9130_MODE mode,UINT32* addr,UINT32* size);
extern void MI9130_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase);
extern void TriggerGlobalReset(void);
extern void ClearGlobalReset(void);
extern BOOL checkExposureLongTime(void);
#endif
