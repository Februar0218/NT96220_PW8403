#ifndef _AFDRV_SONY14M_ADDI9004_450_H_
#define _AFDRV_SONY14M_ADDI9004_450_H_
#include "ImageAlgInfor.h"
#include "af_alg.h"

typedef struct _AFDRV_MANUAL
{
    INT16 iManualStartIdx;
    INT16 iManualEndIdx;
    INT8  iManualStep;
}AFDRV_MANUAL;

//#NT#2010/09/09#Jeffery Chuang -begin
typedef enum
{
    Model_Normal = 0,
    Model_FallOff_Strong,
    ENUM_DUMMY4WORD(AF_AETRANSFERMODE)
}AF_AETRANSFERMODE;//Decide AFAE transfer mode
//#NT#2010/09/09#Jeffery Chuang -end

 extern UINT16 AFdrv_GetFocusDist(void);
extern void AFdrv_SetManualRange(AFDRV_MANUAL ManualSet);
extern void AFdrv_GetCurrentRange(PAFALG_INFO_IN af_Obj, _IMGAFMode_sel AFMode);
extern void AFdrv_AFPrepare(BOOL bOP, UINT32 step);
extern void AFdrv_AEPrepare(BOOL bOP, UINT32 step);
extern void AFdrv_VAPrepare(BOOL bface, BOOL bOP, UINT32 step);
extern void AFdrv_SetAFModeChange(BOOL EN);
extern BOOL AFdrv_CheckAFModeChange(void);
extern void AFdrv_AASwitch(BOOL bOP);
extern AF_AETRANSFERMODE AFdrv_GetAFModel(void);

//Temp
extern UINT16 AF_GetFocusDist(void);

#endif
