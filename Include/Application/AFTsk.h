#ifndef _AFTSK_H
#define _AFTSK_H

#include "Type.h"
#include "af_alg.h"
#include "af_api.h"
#include "AF_Lib.h"

/**
     Function pointer for af application.
     1.AF parameters init.
     2.AF process for VA/AE/AFMode setting.
     3.AF process for iris switch.
     4.CAF process
     5.AF algorithm.
     user can choose which function you needed.
*/

typedef void (*FPAF_PARAM)(AF_ALG_SEARCH_MODE SearchMode);
typedef void (*FPAF_PREPARE)(BOOL bface, BOOL en);
typedef void (*FPAF_IRISSWITCH)(BOOL on);
typedef UINT32 (*FPAF_TIMGING)(CNT_INFO op);
typedef void (*FPAFTSKCAF)(void);

#define FLGAF_IDLE          FLGPTN_BIT(0)
#define FLGAF_START         FLGPTN_BIT(1)
#define FLGAF_CAF           FLGPTN_BIT(2)

#define FLGAF2_IDLE         FLGPTN_BIT(3)
#define FLGAF2_START        FLGPTN_BIT(4)
#define FLGAF2_END          FLGPTN_BIT(5)
#define FLGAF2_AFGO2DONE    FLGPTN_BIT(6)
#define FLGAF2_AFPROC       FLGPTN_BIT(7)

#define FLGAF_MASK          (FLGPTN)(~0)

//#NT#2012/06/01#KS Hung -begin
//#NT#Add the callback function of Detecting GPIO
extern void AFTsk_RegisterFP_DetGPIOKey(FPAF_DETGPIOKEY pafDetGPIOKey);
//#NT#2012/06/01#KS Hung -end
extern void AFTsk_RegisterFP_Param(FPAF_PARAM pafparaminit);
extern void AFTsk_RegisterFP_Prepare(FPAF_PREPARE pafprepareEn);
extern void AFTsk_RegisterFP_IrisSwtich(FPAF_IRISSWITCH pafirisswitch);
extern void AFTsk_RegisterFP_AFTiming(FPAF_TIMGING pafirisswitch);
extern void AFTsk_SetStart(void);
extern void AFTsk_RegisterCAF(FPAFTSKCAF pPhotoCAF);

/**
     For AFTsk open/close. If user use AFtsk, need to open/close it.
*/
extern ER AF_TskOpen(void);
extern ER AF_TskClose(void);

/**
     If AFtsk is used, need to wait tsk idle after calling AF process.
*/
extern void AFTsk_WaitIdle(void);

/**
     For Normal AF process.
*/
extern void AF_Process(void);

/**
     For Normal AF process but no FD detect.
*/
extern void AF_Process_NOFD(void);

/**
     For full search AF process.
     Make sure setting search range(AF_SetManualRange)before calling this function.
*/
extern void AF_FullRangeSearch(void);

/**
     If user need to quickly stop AF process, call this function and AF will stop
     in default position you set.
*/
extern void AF_Release(void);

/**
    Calling this function do nothing.
*/
extern void AF_Process_PunchThrough(void);

/**
    This is for user need to choose full search function but just put the botton.
    Call this function first and AF_Process will be AF_FullRangeSearch.
*/
extern void AF_SetFullSearch_Process(void);

/**
    CAF process.
*/
extern void AF_CAFProcess(void);

/**
    Set CAF rate. User can decide how many frames doing CAF.
*/
extern void AF_SetCafRate(UB rate);

/**
    Get CAF rate. User can Get rate now.
*/
extern UINT32 AF_GetCafRate(void);

/**
    CAF Lock/Unlock.
*/
extern void AF_LockCAF(UB lock);

/**
    Make sure CAF is Lock/Unlock.
*/
extern BOOL AF_IsCAFLock(void);

/**
    AFTsk/AFTsk2 two task. If user uses these task, need to register them.
*/
extern void AFTsk(void);
extern void AFTsk2(void);

#endif // _AFTSK_H
