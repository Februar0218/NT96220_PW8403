#ifndef _AF_LIB_H_
#define _AF_LIB_H_

#include "debug.h"
#include "af_alg.h"
#include "afdrv_common.h"

typedef void (*FPSETISRFRMCNT)(void);

typedef enum
{
    FACE_WEIGHT_NONE = 0,
    FACE_WEIGHT_SMALL = 10,
    FACE_WEIGHT_BIG = 80,
    FACE_WEIGHT_FULL = 120,
    ENUM_DUMMY4WORD(FACE_WEIGHT)
}FACE_WEIGHT;
typedef enum
{
    GETCNT_START = 0,
    GETCNT_END,
    GETCNT_SIE,
    GETCNT_PRE,
    GETCNT_DCE,
    GETCNT_IPE,
    ENUM_DUMMY4WORD(CNT_INFO)
}CNT_INFO;

typedef enum
{
    SIE_CNT = 0,
    PRE_CNT,
    DCE_CNT,
    IPE_CNT,
    MAX_CNT,
    ENUM_DUMMY4WORD(DRV_ISRCNT)
}DRV_ISRCNT;

extern void AF_ParamInit(AF_ALG_SEARCH_MODE SearchMode);
extern void AF_AEIrisSwitch(BOOL on);
extern void AF_PrepareEN(BOOL bface, BOOL en);
extern void AF_GetResult(INT16* piStartIdx, INT16* piEndIdx, INT8* piStep, INT16* piOffsetStep, INT16 *piMaxIdx, UINT8 *puiSelectWin, UINT8 *puiStatus);
extern void AF_SetManualRange(INT16 iStartIdx, INT16 iEndIdx, INT8 iStep);
extern UINT32 AF_TimingPrepare(CNT_INFO op);
extern void AF_SetFrmCnt(DRV_ISRCNT drv);
extern void AF_LockAF(BOOL OP);
extern UINT32 AF_GetAFFDWT(UINT8 i);
//#NT#2011/05/05#Chris Chung -begin
extern UINT32 AF_GetVariation_OBJ(UINT8 uiIdx);
extern UINT32 AF_GetVariation_3X3(UINT8 uiIdx);
//#NT#2011/05/05#Chris Chung -end
//#NT#2011/11/17#KS Hung -begin
//remove the warning message
extern void AF_GetVariation_OBJInit(void);
//#NT#2011/11/17#KS Hung -end
//#NT#2011/12/02#Jarkko Chang -begin
extern UINT32 AF_WriteDebugInfo(UINT32 Addr);
//#NT#2011/12/02#Jarkko Chang -end
#endif
