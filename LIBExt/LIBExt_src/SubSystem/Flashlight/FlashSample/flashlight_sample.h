#ifndef _FLASHLIGHT_SAMPLE_H_
#define _FLASHLIGHT_SAMPLE_H_

#include "mIType.h"



#define PRE_FLASH_WIDTH 20
#define REDEYE_WIDTH 20
#define FLASH_TRIGGERPOINT 70 // LV7.0

#define FLASH_DEF_RGAIN 346
#define FLASH_DEF_GGAIN 256
#define FLASH_DEF_BGAIN 266

////////////////////////////////////////////////////////////////////////////////
///////////////////////           Information API            ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern void flash_PreFlashFlow(void);
extern void flash_RedEyeReduct(void);
extern void flash_Evaluate(void);

#endif
