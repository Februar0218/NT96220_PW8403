#ifndef _CALSENSOR_H
#define _CALSENSOR_H

extern void Cal_DefectPixel(void);
extern void Cal_ISO(void);
extern void Cal_Flash(void);
extern void Cal_FlashAWB(void);
extern void Cal_FlashRedeyeTest(void);
extern void Cal_FlashGnoShots(void);
extern void Cal_MShutter(void);
extern void Cal_MShutter_sIris(void);
extern void Cal_AWB(void);
extern void Cal_Write2File(void);
extern void Cal_ReadCalStatus(void);
extern void Cal_VIG(void);
extern void Cal_ECS(void);
extern void Cal_WriteAWBGS(void);
#endif