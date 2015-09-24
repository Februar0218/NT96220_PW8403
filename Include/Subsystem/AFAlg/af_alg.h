#ifndef _AF_ALG_H_
#define _AF_ALG_H_

#include "Type.h"
#include "Lens.h"

#define AF_TOTAL_STEP           256
#define AF_TOTAL_WIN            (3*3)
#define AF_DEFAULT_WIN          (AF_TOTAL_WIN/2)
#define AF_NOSELECT_WIN         0xFF

#define AFALG_STS_SUCCESS       0x0
#define AFALG_STS_TOONEAR       0x1
#define AFALG_STS_TOOFAR        0x2
#define AFALG_STS_TREMBLE       0x4
#define AFALG_STS_LOWVA         0x8
#define AFALG_STS_NOTARGET      0x10
#define AFALG_STS_SMOOTH        0x20
#define AFALG_STS_VASMALL       0x40

typedef UINT32 (*FPGETVA)(UINT8 uiIdx);
typedef UINT32 (*FPSTOPLENS)(LENS_AFSTATUS status);

typedef enum
{
    AF_ALG_SMODE_FULLSEARCH = 0,
    AF_ALG_SMODE_FALLNORMAL,
    AF_ALG_SMODE_FALLOFF_STRONG,
    AF_ALG_SMODE_FALLNORMAL_NOFD,
    ENUM_DUMMY4WORD(AF_ALG_SEARCH_MODE)
}AF_ALG_SEARCH_MODE;

typedef enum {
    AFALG_WINDOW_CENTER,
    AFALG_WINDOW_AUTO,
    AFALG_WINDOW_MANUIAL,
    ENUM_DUMMY4WORD(AFALG_WINDOW_SEL)
}AFALG_WINDOW_SEL;

typedef enum
{
    AFALG_STOP = 0,
    AFALG_RUN,
    AFALG_END,
    ENUM_DUMMY4WORD(AFALG_STATUS)
}AFALG_STATUS;

typedef struct _AFALG_INFO_IN
{
    AFALG_WINDOW_SEL Window_sel;
    AF_ALG_SEARCH_MODE Search_mode;
    BOOL bObj;
    INT32 iStartIdx;
    INT32 iEndIdx;
    UINT32 iStep;
    INT32 iDefaultIdx;
    INT32 iOffsetStep;
}AFALG_INFO_IN, *PAFALG_INFO_IN;

//#NT#2011/05/30#Jarkko Chang -begin
//#NT# modify for multi af
typedef struct _AFALG_INFO_OUT
{
    INT32 iPeak;
    UINT32 uiSelectWin[AF_TOTAL_WIN];
    UINT32 uiStatus;
}AFALG_INFO_OUT, *PAFALG_INFO_OUT;
//#NT#2011/05/30#Jarkko Chang -end

typedef struct _AFALG_INFO
{
    AFALG_INFO_IN afalg_in;
    AFALG_INFO_OUT afalg_out;
}AFALG_INFO, *PAFALG_INFO;

extern void AFALG_SetParam(void);
extern UINT32 AFALG_GetAFStatus(void);
extern void AFALG_GetAFReaultBystep(UINT8 uiWindowIdx, UINT8 uiStepIdx, INT16 *piIdx, UINT32 *puiVA);
extern PAFALG_INFO AFALG_GetINFO(void);
extern void AFALG_MainFlow(void);
extern void AFALG_RegisterGetVA_Func(FPGETVA pGetVA);
extern void AFALG_RegisterStopLens_Func(FPSTOPLENS pStopLens);
extern void AFALG_SetStatus(AFALG_STATUS Status);
//For Calibration test
extern void AFALG_SetSNTest(void);

#endif
