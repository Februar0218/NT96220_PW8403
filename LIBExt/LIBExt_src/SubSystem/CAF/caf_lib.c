#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Type.h"
#include "debug.h"
#include "ImageAlgInfor.h"
#include "aaa_lib.h"
#include "oscommon.h"
#include "ipe_lib.h"
#include "ippmanager.h"
#include "sen_common.h"
#include "Lens.h"
#include "Caf_lib.h"
#include "Ae_alg.h"
#include "Ae_Api.h"
#include "Utility.h"

#define CAF_DebugMsg  DISABLE
#if(CAF_DebugMsg)
#define CAF_MSG(...)       debug_msg("CAF MSG:"__VA_ARGS__)
#else
#define CAF_MSG(...)
#endif
#define CAF_ERR(...)      debug_msg("CAF ERR:"__VA_ARGS__)

BOOL FocusTrack_Enable = FALSE;
UINT16 Tracking_StableCnt=0;
UINT32 cafHist[64], AvgY;

UINT32 LastAF_Value;

INT16 Recode_FocusStep[100];
UINT16 Index_Cnt=0;
BOOL Scene_Change = FALSE;
BOOL Double_Check = TRUE;
UINT32 caf_converge_check = 0;
UINT32 PrevEV = 99999;
UINT32 PrevHist = 99999;
UINT32 va_change_cnt;
BOOL ev_change;
UINT32 hist_change;
UINT32 ev_stable_lowlight;
UINT32 PrevY, Y_stable_cnt, step_ratio = 1,damp_ratio = 1;
UINT32 Scene_chang_cnt = 0, hist_stable_cnt = 0;

#define VA_UartMsg 0

const UINT8 VAWTable[8][8]={
        {0,0,0, 0, 0,0,0,0},
        {0,2,2, 2, 2,2,2,0},
        {0,2,3, 3, 3,3,2,0},
        {0,2,3, 8, 8,3,2,0},
        {0,2,4, 8, 8,4,2,0},
        {0,2,3, 4, 4,3,2,0},
        {0,2,2, 3, 3,2,2,0},
        {0,0,0, 0, 0,0,0,0},
        };

typedef struct _CAFALG_PARAM
{
    MOTOR_FOCUS_ACT RT_AF_Direction;
    UINT32 RT_AF_Status;
    INT16  RT_AF_MinIndex;
    INT16  RT_AF_MaxIndex;
    UINT32 RT_AF_Step;
    UINT32 RT_AF_BestIdx;
    BOOL   Inv_Direction;
    UINT32 Damp_CNT;
    UINT32 DampCnt_TH;
    BOOL b_Falling;
}CAFALG_PARAM;

BOOL Inv_Direction=FALSE;
INT16 RTAF_MinIndex,RTAF_MaxIndex;
INT16 RTAF_BestIdx;
UINT32 DampCnt = 0;
unsigned char RT_AF_Status=0;
BOOL RT_AF_Direction;
unsigned char RT_AF_Step;

static CAFALG_PARAM CafAlg_Param = {0};
static CAF_PARAM Caf_Param = {0};
BOOL CafStatus;

BOOL RtAFLockFlag = TRUE;//FALSE;
BOOL InitialFlag = TRUE;    //setting va information when TRUE
BOOL TrigRestart = TRUE;    //reset caf status and converge again
BOOL VAEnableFlag = FALSE;
UINT32 VACnt;

extern UINT32 AeGetStableCount(void);
extern void AF_AFPrepare(BOOL bOP);
extern void AF_GetCurrentRange(INT16* piStartIdx, INT16* piEndIdx, INT8* piStep, INT16 *iDefaultIdx);
extern ER TimerDelayMs(UINT32 ms);
BOOL caf_ChkVAValue(UINT32 Threshold);
//#NT#2010/02/10#Jarkko Chang -begin
//#NT#modify for caf
//extern UINT32 User_VABegin(UINT8 *Param);
ER User_VABegin(AF_IPEINFO *AfIpeInfo);
ER User_VAEnd(AF_IPEINFO *AfIpeInfo);
//extern int  User_VAEnd(UINT8 *Param);
//#NT#2010/02/10#Jarkko Chang -end
extern BOOL IsFocusInitFinish(void);
//extern double GetPreciseEV();
extern UINT16 caf_GetFocusDist(void);

//#NT#2010/02/22#Jarkko Chang -begin
static FPCALLBACK_GETSearchRange g_fpGetSearchRange = NULL;

void CAF_RegGetRTAF_SearchRangeCB(FPCALLBACK_GETSearchRange getSearchRange)
{
    g_fpGetSearchRange = getSearchRange;
}
//#NT#2010/02/22#Jarkko Chang -end

//#NT#2010/03/12#Jarkko Chang -begin
void RTAF_SearchRange(INT16 MinIndex, INT16 MaxIndex)
{
    INT16 IdxSwitch;

    if (MaxIndex < MinIndex)
    {
        IdxSwitch = MaxIndex;
        MaxIndex = MinIndex;
        MinIndex = IdxSwitch;
    }
    CafAlg_Param.RT_AF_MinIndex = MinIndex;
    CafAlg_Param.RT_AF_MaxIndex = MaxIndex;
}
//#NT#2010/03/12#Jarkko Chang -end

BOOL IsTrackingEn(void)
{
    return FocusTrack_Enable;
}

void SetTrackingFocusEnable(BOOL TrackingEn)
{
    FocusTrack_Enable = TrackingEn;
}

unsigned long GetTrackingStableCnt(void)
{
    return Tracking_StableCnt;
}

static INT16 caf_GetNextFocusPos(MOTOR_FOCUS_ACT dir, UINT32 step)
{
    INT16 NextIdx;
    if(dir == MOTOR_FOCUS_FWD)
        NextIdx=Lens_Focus_GetCurrentPosition() + step;
    else
        NextIdx=Lens_Focus_GetCurrentPosition() - step;

    if(NextIdx >= CafAlg_Param.RT_AF_MaxIndex)
        NextIdx = CafAlg_Param.RT_AF_MaxIndex;
    else if(NextIdx <= CafAlg_Param.RT_AF_MinIndex)
        NextIdx = CafAlg_Param.RT_AF_MinIndex;

//    CAF_MSG("LENS GO %d %d %d\r\n",NextIdx,CafAlg_Param.RT_AF_MaxIndex,CafAlg_Param.RT_AF_MinIndex);
    return NextIdx;
}

UINT32 caf_calHistPeak(void)
{
    UINT32 i,peak = 0,peak_idx = 0,sum = 0;

    AE_GetHisto(cafHist);
    for (i = 1; i < 63; i++)
    {
//        sum = cafHist[i-2]+cafHist[i-1]+cafHist[i]+cafHist[i+1]+cafHist[i+2];
        sum = cafHist[i-1]+cafHist[i]+cafHist[i+1];
        if (sum >= peak)
        {
            peak = sum;
            peak_idx = i;
        }
        if (sum > 512)
            return peak_idx;
    }
    return peak_idx;
}

UINT32 caf_ChkHist(UINT32 Percent, UINT32 Step)
{
    UINT32 i, Avg, Center, BlockW, BlockH;


    BlockW = GetIPPSizeInfor(_ImeOut1Hsize) / 8 * 2;
    BlockH = GetIPPSizeInfor(_ImeOut1Vsize) / 8 * 2;
    Center = AvgY / 4;
    if (Center > Step)
    {
        Center -= Step;
    }
    else
    {
        Center = 0;
    }

    Avg = 0;
    for (i = 0; i < (Step * 2); i ++)
    {
        Avg += cafHist[Center];
        Center ++;
        if (Center >= 64)
        {
            break;
        }
    }
    Avg = (Avg * 100) / (BlockH * BlockW);
    //sprintf(message,"%d", Avg);
    //SenFP_ShowOSDString(message,20,50,1);
    if (Avg >= Percent)
    {
        return 1;
    }
    return 0;
}

UINT32 caf_calHist(void)
{
    UINT32 i, j, BlockW, BlockH, StartX, StartY, Avg;
    UINT8 *Addr;

    GetActImgFBAddr_Path1(&FB_para1);
    BlockW = GetIPPSizeInfor(_ImeOut1Hsize) / 8 * 2;
    BlockH = GetIPPSizeInfor(_ImeOut1Vsize) / 8 * 2;
    StartX = GetIPPSizeInfor(_ImeOut1Hsize) / 2 - (BlockW / 2);
    StartY = GetIPPSizeInfor(_ImeOut1Vsize) / 2 - (BlockH / 2);
    Addr = (UINT8 *)(FB_para1.y_addr + (StartY * GetIPPSizeInfor(_ImeOut1LineOffset)) + StartX);
    for (i = 0; i < 64; i++)
    {
        cafHist[i] = 0;
    }

    Avg = 0;
    for (j = 0; j < BlockH; j ++)
    {
        for (i = 0; i < BlockW; i ++)
        {
            Avg += Addr[i];
            cafHist[(Addr[i] / 4)] ++;
        }
        Addr += GetIPPSizeInfor(_ImeOut1LineOffset);
    }
    Avg = Avg / (BlockW * BlockH);

#if 0
    debug_err(("_____________________________________ %d\r\n", Avg));
    for (i = 0; i < 8; i ++)
    {
        debug_err(("%d %d %d %d %d %d %d %d\r\n",
                    cafHist[i*8+0], cafHist[i*8+1], cafHist[i*8+2], cafHist[i*8+3],
                    cafHist[i*8+4], cafHist[i*8+5], cafHist[i*8+6], cafHist[i*8+7]
                    ));
    }
#endif
    return Avg;
}

UINT32 caf_SceneChk(void)
{
    INT32 i;
    UINT32 HighMax, LowMax, tempValue;
    UINT32 StartIdx;

    StartIdx = AvgY / 4;
    LowMax = StartIdx;
    HighMax = StartIdx;

    tempValue = 0;
    for (i = StartIdx; i > 0; i --)
    {
        if (cafHist[i] > tempValue)
        {
            tempValue = cafHist[i];
            LowMax = i;
        }
    }


    tempValue = 0;
    for (i = StartIdx; i < 64; i ++)
    {
        if (cafHist[i] > tempValue)
        {
            tempValue = cafHist[i];
            HighMax = i;
        }
    }

    if (((HighMax >= 62) && (HighMax <= 63)) && (LowMax >= (StartIdx - 8)))
    {
        return 1;
    }
    return 0;
}

UINT32 caf_GetVA(void)
{
    UINT32 i,j,va=0;
    UINT32 VA_Num,VA_Cnt = 0;
    STR_VA_INFOR    VA_Info;

    ipe_getVA_Infor(&VA_Info);
//sum all va windows with center weighting matrix
    VA_Num = 3;

    for(i=0;i<VA_Num;i++)
    {
        for(j=0;j<VA_Num;j++)
        {
            va += aaa_GetVariation(VA_Cnt)*VAWTable[i][j];
            VA_Cnt++;
        }
    }

    va = va/VA_Window_Num;
    return va;
}
//#NT#2010/03/12#Jarkko Chang -end

static UINT32 caf_GetMaxIndex(void)
{
    UINT32 i, BestIdx = Caf_Param.Index_Cnt, VAMAX = 0;

    //Sometimes 0 is unstable
    for(i=1; i<=Caf_Param.Index_Cnt; i++)
    {
        if(Caf_Param.uiVAValue[i] > VAMAX)
        {
            VAMAX = Caf_Param.uiVAValue[i];
            BestIdx = i;
        }
        //CAF_MSG("i %d VA %d\r\n",i,Caf_Param.uiVAValue[i]);
    }
    CAF_MSG("MAX =%d VA %d\r\n",BestIdx,Caf_Param.uiVAValue[BestIdx]);
    return BestIdx;
}

BOOL GetAeConvergeState(void)
{
    return AEAlg_IsEVBound();
}

UINT32 GetAeStableCnt(void)
{
    return AEAlg_GetStbCnt();
}

static void caf_Algorithm(UINT32 AF_Value)
{
    UINT32 VAFirst = 0, VASecond = 0, VALast = 0;
    INT16 NextIdx = 0;
    UINT32 Index_Diff;

//    CafAlg_Param.RT_AF_Step = caf_GetFocusStep();

    Index_Diff = abs(Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(),0)-Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(),Lens_Focus_GetZoomTableMaxIdx()) );

    if  (Index_Diff == 0)
        Index_Diff = 1;
    CafAlg_Param.RT_AF_Step = (Index_Diff+29)/30;

    switch(CafAlg_Param.RT_AF_Status)
    {
        //---------------------------------------------------------
        // Step0: Get VA and Guess direction
        //---------------------------------------------------------
        case 0:
            //Resum_TrackingFocus = IsTrackingEn();
//            ipe_waitDMAEnd(TRUE);
//            AF_Value = caf_GetVA();
            LastAF_Value = AF_Value;

            Caf_Param.uiStatus[Caf_Param.Index_Cnt] = CafAlg_Param.RT_AF_Status;
            Caf_Param.iFocusPos[Caf_Param.Index_Cnt] = Lens_Focus_GetCurrentPosition();
            Caf_Param.uiVAValue[Caf_Param.Index_Cnt] = AF_Value;

            CAF_MSG("%d: Status=%d, Focus=%d, VA=%d\r\n",Caf_Param.Index_Cnt,Caf_Param.uiStatus[Caf_Param.Index_Cnt],Caf_Param.iFocusPos[Caf_Param.Index_Cnt],Caf_Param.uiVAValue[Caf_Param.Index_Cnt]);

            if(!CafAlg_Param.Inv_Direction)
            {
                if(Lens_Focus_GetCurrentPosition()<((CafAlg_Param.RT_AF_MinIndex + CafAlg_Param.RT_AF_MaxIndex)/2))
                    CafAlg_Param.RT_AF_Direction = MOTOR_FOCUS_FWD;
                else
                    CafAlg_Param.RT_AF_Direction = MOTOR_FOCUS_BWD;
            }
            else
            {
                CafAlg_Param.Inv_Direction = FALSE;
            }
            NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step);
            Lens_Focus_Goto(NextIdx);

            VACnt = ipe_getVACnt();
            CafAlg_Param.RT_AF_Status ++;
            Caf_Param.Index_Cnt ++ ;
            CafStatus = FALSE;
            break;

        case 1:
            //---------------------------------------------------------
            // Step1: Continue Go step and record VA
            //---------------------------------------------------------
            if (VACnt == ipe_getVACnt())
            {
                return;
            }
//            ipe_waitDMAEnd(TRUE);
//            AF_Value = caf_GetVA();
            LastAF_Value=AF_Value;

            Caf_Param.uiStatus[Caf_Param.Index_Cnt] = CafAlg_Param.RT_AF_Status;
            Caf_Param.iFocusPos[Caf_Param.Index_Cnt] = Lens_Focus_GetCurrentPosition();
            Caf_Param.uiVAValue[Caf_Param.Index_Cnt] = AF_Value;

            CAF_MSG("%d: Status=%d, Focus=%d, VA=%d\r\n",Caf_Param.Index_Cnt,Caf_Param.uiStatus[Caf_Param.Index_Cnt],Caf_Param.iFocusPos[Caf_Param.Index_Cnt],Caf_Param.uiVAValue[Caf_Param.Index_Cnt]);

            NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step);
            Lens_Focus_Goto(NextIdx);

            VACnt = ipe_getVACnt();
            Caf_Param.Index_Cnt ++;
            CafAlg_Param.RT_AF_Status++;
//            CafStatus = FALSE;
            break;
        case 2:
            //---------------------------------------------------------
            // Step2: Continue Go step and record VA
            //---------------------------------------------------------
            if (VACnt == ipe_getVACnt())
            {
                return;
            }

//            ipe_waitDMAEnd(TRUE);
//            AF_Value = caf_GetVA();
            LastAF_Value=AF_Value;

            Caf_Param.uiStatus[Caf_Param.Index_Cnt] = CafAlg_Param.RT_AF_Status;
            Caf_Param.iFocusPos[Caf_Param.Index_Cnt] = Lens_Focus_GetCurrentPosition();
            Caf_Param.uiVAValue[Caf_Param.Index_Cnt] = AF_Value;

            CAF_MSG("%d: Status=%d, Focus=%d, VA=%d\r\n",Caf_Param.Index_Cnt,Caf_Param.uiStatus[Caf_Param.Index_Cnt],Caf_Param.iFocusPos[Caf_Param.Index_Cnt],Caf_Param.uiVAValue[Caf_Param.Index_Cnt]);

            NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step);

            Lens_Focus_Goto(NextIdx);

            VACnt = ipe_getVACnt();
            CafAlg_Param.RT_AF_Status ++;
            Caf_Param.Index_Cnt ++;
//            CafStatus = FALSE;
            break;

        case 3:
            //---------------------------------------------------------
            // Step3: record VA and judge direction
            //---------------------------------------------------------
            if (VACnt == ipe_getVACnt())
            {
                return;
            }

//            ipe_waitDMAEnd(TRUE);
//            AF_Value = caf_GetVA();

            Caf_Param.uiStatus[Caf_Param.Index_Cnt] = CafAlg_Param.RT_AF_Status;
            Caf_Param.iFocusPos[Caf_Param.Index_Cnt] = Lens_Focus_GetCurrentPosition();
            Caf_Param.uiVAValue[Caf_Param.Index_Cnt] = AF_Value;

            CAF_MSG("%d: Status=%d, Focus=%d, VA=%d\r\n",Caf_Param.Index_Cnt,Caf_Param.uiStatus[Caf_Param.Index_Cnt],Caf_Param.iFocusPos[Caf_Param.Index_Cnt],Caf_Param.uiVAValue[Caf_Param.Index_Cnt]);
            //Record Last VA
            if(Caf_Param.Index_Cnt >= 3)
            {
                VAFirst = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-3]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2])/2;
                VASecond = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1])/2;
                VALast = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt])/2;
            }

            //VA curve is falling, direction is wrong, invert and back to 0!.
            if( (VALast < VASecond) && (VASecond < VAFirst) )
            {
                if(CafAlg_Param.b_Falling == FALSE)
                {
                    CafAlg_Param.RT_AF_Direction = (CafAlg_Param.RT_AF_Direction+1)%2;
                    NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step*3));
                    Lens_Focus_Goto(NextIdx);
                    CafAlg_Param.Inv_Direction = TRUE;
                    CafAlg_Param.RT_AF_Status = 0;
                    CafAlg_Param.b_Falling = TRUE;
                }
                else
                {
                    //falling and falling again, find best one
                    CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                    CafAlg_Param.RT_AF_Status = 5;
                    CafAlg_Param.b_Falling = FALSE;
                }
            }
            //VA curve is rising, direction is right, keep going and go to 4!
            else if( (VALast >= VASecond) && (VASecond >= VAFirst) )
            {
                NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step);
                Lens_Focus_Goto(NextIdx);
                CafAlg_Param.RT_AF_Status ++;
            }
            //VA curve is damping, keep going to check again
            else
            {
                CafAlg_Param.Damp_CNT ++;
                //More than damping (TH) times, check max VA index.
                if (CafAlg_Param.Damp_CNT > CafAlg_Param.DampCnt_TH)
                {
                    CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                    CafAlg_Param.RT_AF_Status = 5;
                    CafAlg_Param.Damp_CNT = 0;
                }
                else
                {
                    NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step);
                    Lens_Focus_Goto(NextIdx);
                }
            }
            VACnt = ipe_getVACnt();
            Caf_Param.Index_Cnt++;
            break;
        case 4:
            //---------------------------------------------------------
            // Step4: VA is rising, keep going until falling.
            //---------------------------------------------------------
            if (VACnt == ipe_getVACnt())
            {
                return;
            }

//            ipe_waitDMAEnd(TRUE);
//            AF_Value = caf_GetVA();

            Caf_Param.uiStatus[Caf_Param.Index_Cnt] = CafAlg_Param.RT_AF_Status;
            Caf_Param.iFocusPos[Caf_Param.Index_Cnt] = Lens_Focus_GetCurrentPosition();
            Caf_Param.uiVAValue[Caf_Param.Index_Cnt] = AF_Value;

            CAF_MSG("%d: Status=%d, Focus=%d, VA=%d\r\n",Caf_Param.Index_Cnt,Caf_Param.uiStatus[Caf_Param.Index_Cnt],Caf_Param.iFocusPos[Caf_Param.Index_Cnt],Caf_Param.uiVAValue[Caf_Param.Index_Cnt]);

            if(Caf_Param.Index_Cnt >= 3)
            {
                VAFirst = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-3]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2])/2;
                VASecond = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-2]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1])/2;
                VALast = (Caf_Param.uiVAValue[Caf_Param.Index_Cnt-1]+Caf_Param.uiVAValue[Caf_Param.Index_Cnt])/2;
            }

            //VA curve is falling, Find Best index!.
            if( (VALast < VASecond) && (VASecond < VAFirst) )
            {
                CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                CafAlg_Param.RT_AF_Status = 5;
            }
            //VA curve is rising, keep going.
            else if( (VALast >= VASecond) && (VASecond >= VAFirst) )
            {
                NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step/2);
                if(NextIdx == CafAlg_Param.RT_AF_MaxIndex || NextIdx == CafAlg_Param.RT_AF_MinIndex)
                {
                    CafAlg_Param.RT_AF_BestIdx = Caf_Param.Index_Cnt;
                    CafAlg_Param.RT_AF_Status = 5;
                    CAF_ERR("CAF: Focus out of boundary %d!!!!\r\n",NextIdx);
                }
                Lens_Focus_Goto(NextIdx);
                VACnt = ipe_getVACnt();
                Caf_Param.Index_Cnt++;
            }
            //VA curve is damping, keep going to check again
            else
            {
                CafAlg_Param.Damp_CNT ++;
                //More than damping (TH) times, check max VA index.
                if (CafAlg_Param.Damp_CNT > CafAlg_Param.DampCnt_TH)
                {
                    CafAlg_Param.RT_AF_BestIdx = caf_GetMaxIndex();
                    CafAlg_Param.RT_AF_Status = 5;
                    CafAlg_Param.Damp_CNT = 0;
                    CAF_ERR("CAF: Damping!!!!\r\n");
                }
                else
                {
                    NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,CafAlg_Param.RT_AF_Step/2);
                    if(NextIdx == CafAlg_Param.RT_AF_MaxIndex || NextIdx == CafAlg_Param.RT_AF_MinIndex)
                    {
                        CafAlg_Param.RT_AF_BestIdx = Caf_Param.Index_Cnt;
                        CafAlg_Param.RT_AF_Status = 5;
                        CAF_ERR("CAF: Focus out of boundary %d!!!!\r\n",NextIdx);
                    }
                    Lens_Focus_Goto(NextIdx);
                    VACnt = ipe_getVACnt();
                    Caf_Param.Index_Cnt++;
                }
            }
//            PrevHist = AE_GetHistLowCnt();
            PrevEV = AEAlg_GetPrvEVValue();
            PrevHist = caf_calHistPeak();
            break;
        case 5:
            CafAlg_Param.RT_AF_Status ++;
            Lens_Focus_Goto(Caf_Param.iFocusPos[CafAlg_Param.RT_AF_BestIdx]);
            LastAF_Value = Caf_Param.uiVAValue[CafAlg_Param.RT_AF_BestIdx];
            VACnt = ipe_getVACnt();
//            PrevHist = AE_GetHistLowCnt();
            PrevEV = AEAlg_GetPrvEVValue();
            PrevHist = caf_calHistPeak();
            CAF_MSG("MAXFocus=%d\r\n",Caf_Param.iFocusPos[CafAlg_Param.RT_AF_BestIdx]);
            break;

        case 6:
            if (VACnt == (ipe_getVACnt()+3))
            {
                return;
            }

//            ipe_waitDMAEnd(TRUE);
//            AF_Value = caf_GetVA();
            if (caf_converge_check < 3)
            {
                LastAF_Value = AF_Value;
                caf_converge_check++;
            }
            if ((AF_Value > LastAF_Value * 60 / 100) && (AF_Value < LastAF_Value * 140 / 100))
            {
                VACnt = ipe_getVACnt();
                CafStatus = TRUE;
                Scene_chang_cnt = 0;
            }
            else
            {
                if (Scene_chang_cnt > 5)
                {
                    Scene_chang_cnt = 0;
                    Scene_Change = TRUE;
                }
                else
                {
                    Scene_chang_cnt++;
                }
            }
            break;
    }
}

void caf_PrintCAFInfo(void)
{
    UINT32 i;

    debug_msg("Focus: \r\n");
    for(i=0; i<=Caf_Param.Index_Cnt; i++)
    {
        debug_msg("%d\t : %d\r\n",Caf_Param.iFocusPos[i],Caf_Param.uiVAValue[i]);
    }
}

void caf_setInitFlag(BOOL flag)
{
    InitialFlag = flag;
}

BOOL caf_GetVAFlag(void)
{
    return VAEnableFlag;
}

//#NT#2010/02/10#Jarkko Chang -begin
//#NT#modify for caf
void caf_VAPprepare(BOOL flag)
{
    AF_IPEINFO CAFIPEInfo;

    if (VAEnableFlag == flag)
    {
        return;
    }

    if (flag == TRUE)
    {
        CAFIPEInfo.vacc_gain = 0;
        CAFIPEInfo.va_th = 8;
        CAFIPEInfo.hnfsel = IPE_HNF_AUTO;
        CAFIPEInfo.va_hnf = 0;

        User_VABegin(&CAFIPEInfo);
    }
    else
    {
        User_VAEnd(&CAFIPEInfo);
    }
    VAEnableFlag = flag;
}
//#NT#2010/02/10#Jarkko Chang -end

void caf_Lock(BOOL flag)
{
    if (flag == TRUE)
    {
        caf_VAPprepare(FALSE);
    }
    else
    {
        caf_setInitFlag(TRUE);
    }
    RtAFLockFlag = flag;
}


//#NT#2010/02/05#Jarkko Chang -begin
//#NT#add for continue af

/**
    Check CAF Lock status
    @return CAF Lock TRUE/FALSE
*/
BOOL Is_cafLock(void)
{
    if(RtAFLockFlag)
        return TRUE;
    else
        return FALSE;
}
//#NT#2010/02/05#Jarkko Chang -end

static void caf_InitParam(void)
{
    CafAlg_Param.RT_AF_Status = 0;
    Caf_Param.Index_Cnt = 0;
    CafAlg_Param.Damp_CNT = 0;
    CafAlg_Param.b_Falling = FALSE;
    CafAlg_Param.DampCnt_TH = 3;
    CafStatus = FALSE;
}

void caf_initial(void)
{
    INT16 StartIdx, EndIdx;
    Index_Cnt = 0;

    if (InitialFlag == TRUE)
    {
        caf_VAPprepare(FALSE);
    }

    caf_VAPprepare(TRUE);
    if(g_fpGetSearchRange == NULL)
    {
        debug_err(("Get SearchRange FP is null!!\r\n"));
        StartIdx = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), Lens_Focus_GetZoomTableMaxIdx( ));
        EndIdx   = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 0);
        RTAF_SearchRange(StartIdx,EndIdx);
    }
    else
    {
        g_fpGetSearchRange();
    }
    caf_InitParam();
}

BOOL caf_ChkVAValue(UINT32 Threshold)
{
    //debug_err(("LastAF_Value = %d\r\n", LastAF_Value));
//    if ((LastAF_Value) < Threshold)
    if ((LastAF_Value) > Threshold)
    {
        return 1;
    }
    return 0;
}

//#NT#2010/03/12#Jarkko Chang -begin
void caf_AutoFocusProcess(void)
{
    UINT32 CurEV, CurHist, CurY, CurVa,va_ratio,CurEV_lowLight;

    if (RtAFLockFlag == FALSE)
    {
        //---------------------------------------------------------
        // CAF converage condition
        //---------------------------------------------------------
        CurEV = AEAlg_GetPrvEVValue();
        CurHist = caf_calHistPeak();
        CurY = AE_GetPrvCurLum();
        CurVa = caf_GetVA();
        CurEV_lowLight = CurEV;

        if (GetAeConvergeState())
            va_ratio = 25;
        else
            va_ratio = 20;

        if ((CurVa >= LastAF_Value*(100-va_ratio)/100) && (CurVa <= LastAF_Value*(100+va_ratio)/100))
            va_change_cnt = 0;
        else
            va_change_cnt++;

        if ((CurEV >= (PrevEV*9/10)) && (CurEV <= (PrevEV*11/10)))
            ev_change = FALSE;
        else
            ev_change = TRUE;

        if ((CurEV_lowLight > (PrevEV*9/10)) && (CurEV_lowLight < (PrevEV*11/10)))
            ev_stable_lowlight++;
        else
            ev_stable_lowlight = 0;

        if ((CurHist >= (PrevHist-1)) && (CurHist <= (PrevHist+1)))
            hist_change = 0;
        else
            hist_change++;

        if ((CurY >= (PrevY*9/10)) && (CurY <= (PrevY*11/10)))
            Y_stable_cnt++;
        else
            Y_stable_cnt = 0;

        PrevY = CurY;
#if 0
        SensorInfor.FP_ClearOSD();
        if (GetAeConvergeState())
        {
            sprintf(message, "%d, %d, %d, %d, %d",GetAeStableCnt(),CurVa,LastAF_Value,CurEV,PrevEV);
            SensorInfor.FP_ShowOSDString (message,20,20,4);
            sprintf(message, "%d: %d, %d, %d",CafStatus,Scene_Change,va_change_cnt,ev_change);
            SensorInfor.FP_ShowOSDString (message,20,40,4);
        }
        else
        {
//            sprintf(message, "%d, %d, %d, %d, %d",Y_stable_cnt,CurHist,PrevHist,CurVa,LastAF_Value);
//            sprintf(message, "%d, %d, %d, %d, %d, %d, %d",ev_stable_lowlight,CurEV_lowLight,PrevEV,CurHist,PrevHist,CurVa,LastAF_Value);
            sprintf(message, "%d, %d, %d, %d, %d",ev_stable_lowlight,CurEV_lowLight,PrevEV,CurVa,LastAF_Value);
            SensorInfor.FP_ShowOSDString (message,20,100,4);
            sprintf(message, "%d: %d, %d, %d, %d",CafStatus,CafAlg_Param.RT_AF_Status,va_change_cnt,hist_change,Scene_Change);
            SensorInfor.FP_ShowOSDString (message,20,120,4);
        }
#endif
        if (Scene_Change || InitialFlag)
        {
            TrigRestart = TRUE;
            Scene_Change = FALSE;
        }
        else if (GetAeConvergeState())
        {
            if (GetAeStableCnt() > 4)
            {
                if (CafStatus == TRUE)
                {
                    if (va_change_cnt > 5 || ev_change)
                    {
                        va_change_cnt = 0;
                        caf_InitParam();
                    }
                    else if ((va_change_cnt == 0) && (ev_change == FALSE))
                    {
                        return;
                    }
                    else
                    {
                    }
                }
                else
                {
//                    if ((va_change_cnt == 0) && (ev_change == FALSE))
                    if (ev_change == FALSE)
                    {
                        va_change_cnt = 0;
                    }
                    else if (CafAlg_Param.RT_AF_Status > 3)
                    {
                        va_change_cnt = 0;
                    }
                    else
                    {
//                        PrevEV = CurEV;
                        PrevEV = CurEV_lowLight;
                        caf_InitParam();
                    }
                }
            }
            else
            {
                if (CafStatus == TRUE)
                {
                    return;
                }
                else if ((CafAlg_Param.RT_AF_Status > 3)&& (ev_change == FALSE))
                {
                }
                else
                {
                    return;
                }
            }
        }
        else
        {
            #if 0//orig
            if (hist_change == FALSE && Y_stable_cnt> 5)
            {
                hist_stable_cnt++;
                if (hist_stable_cnt > 5)
                {
                }
                else
                {
                    TrigRestart = TRUE;
                    Scene_Change = FALSE;
                }
            }
            else
            {
                hist_stable_cnt = 0;
                TrigRestart = TRUE;
                Scene_Change = FALSE;
            }
            #elif 1
            if (ev_stable_lowlight > 16)
            {
                if (CafStatus == TRUE)
                {
                    if (va_change_cnt > 5 || hist_change > 5)
//                    if (va_change_cnt > 5)
                    {
                        va_change_cnt = 0;
                        caf_InitParam();
                    }
                    else if ((va_change_cnt == 0) && (hist_change < 5))
//                    else if ((va_change_cnt == 0))
                    {
                        return;
                    }
                    else
                    {
                    }
                }
                else
                {
//                    if ((va_change_cnt == 0) && (hist_change == FALSE))
                    if ((va_change_cnt < 5) && (hist_change < 5))
//                    if ((va_change_cnt < 5))
                    {
                    }
                    else if (CafAlg_Param.RT_AF_Status > 3)
                    {
                        va_change_cnt = 0;
                    }
                    else
                    {
                        PrevEV = CurEV_lowLight;
                        PrevHist = CurHist;
                        caf_InitParam();
                    }
                }
            }
            else
            {
                if (CafStatus == TRUE)
                {
                    PrevEV = CurEV_lowLight;
                    return;
                }
//                else if ((CafAlg_Param.RT_AF_Status > 3)&& (hist_change == FALSE))
                else if ((CafAlg_Param.RT_AF_Status > 3))
                {
                }
                else
                {
                    PrevEV = CurEV_lowLight;
                    TrigRestart = TRUE;
                }
            }
            #endif
        }

        if(Lens_Module_GetState( ) == LENS_IDLE)
        {
            if ((InitialFlag == TRUE) || (TrigRestart == TRUE))
            {
                caf_initial();
                CafStatus = FALSE;
                InitialFlag = FALSE;
                TrigRestart = FALSE;
//                PrevEV = CurEV;
                PrevY = CurY;
                PrevHist = CurHist;
            }
            else
            {
                caf_Algorithm(CurVa);
            }
        }
        else
        {
            InitialFlag = TRUE;
        }
    }
}
//#NT#2010/03/12#Jarkko Chang -end

UINT16 caf_GetFocusDist(void)
{
#if 1
    return 0;
#else
    UINT32 i;
    UINT32 uiAval, uiBval, uiAintvl, uiBintvl;

    for (i = 0; i < 12; i ++)
    {
        if (Get_FocusPosition() < Focus_Zoom_Tbl[Get_Zoom_Section()-1][i])
            break;
    }
    if (i == 0)
    {
        return Focus_Zoom_Tbl_Dist[0];
    }
    else if (i == 12)
    {
        return Focus_Zoom_Tbl_Dist[11];
    }
    uiAval = Focus_Zoom_Tbl_Dist[i-1];
    uiBval = Focus_Zoom_Tbl_Dist[i];
    uiAintvl = Get_FocusPosition() - Focus_Zoom_Tbl[Get_Zoom_Section()-1][i-1];
    uiBintvl = Focus_Zoom_Tbl[Get_Zoom_Section()-1][i] - Get_FocusPosition();
    return (UINT16)((uiAval*uiBintvl + uiBval*uiAintvl)/(uiAintvl+uiBintvl));
#endif
}


