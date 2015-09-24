#ifndef _CAF_LIB_H
#define _CAF_LIB_H


#define CAF_TOTAL_STEP  30

typedef struct _CAF_PARAM
{
    UINT32 uiStatus[CAF_TOTAL_STEP];
    INT32  iFocusPos[CAF_TOTAL_STEP];
    UINT32 uiVAValue[CAF_TOTAL_STEP];
    UINT32 Index_Cnt;
}CAF_PARAM,*PCAF_PARAM;
//#NT#2010/02/22#Jarkko Chang -begin
typedef BOOL (*FPCALLBACK_GETSearchRange)(void);
//#NT#2010/02/22#Jarkko Chang -end
void caf_AutoFocusProcess(void);
//#NT#2009/02/14#Bowen Li -begin
void caf_setInitFlag(BOOL flag);
//#NT#2009/02/23#Bowen Li -begin
void caf_VAPprepare(BOOL flag);
//#NT#2009/02/23#Bowen Li -end
//#NT#2009/02/14#Bowen Li -end
//#NT#2009/02/28#Bowen Li -begin
UINT16 caf_GetFocusDist(void);
//#NT#2009/02/28#Bowen Li -end
UINT32 caf_ChkHist(UINT32 Percent, UINT32 Step);
//#NT#2010/03/12#Jarkko Chang -begin
UINT32 caf_GetVA(void);
//#NT#2010/03/12#Jarkko Chang -end
BOOL caf_GetVAFlag(void);

//#NT#2010/09/07#KS Hung -begin
//#NT#Removing the warning message
UINT32 GetAeStableCnt(void);
BOOL GetAeConvergeState(void);
//#NT#2010/09/07#KS Hung -end

//#NT#2010/10/13#Jarkko Chang -begin
//#NT#modify caf control api
extern void caf_Lock(BOOL flag);
extern BOOL Is_cafLock(void);
extern void caf_PrintCAFInfo(void);
//#NT#2010/10/13#Jarkko Chang -end


typedef enum {
    Search_Step_1  =1,
    Search_Step_2  =2,
    Search_Step_4  =4,
    Search_Step_6  =6,
    Search_Step_8  =8,
    Search_Step_10 =10
} Caf_Search_Step;
#endif
