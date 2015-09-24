#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ife_lib.h"
#include "ime_lib.h"
#include "dcout.h"
#include "sen_isr.h"
#include "prv_common.h"
#include "imagealginfor.h"
#include "sen_common.h"
#include "ippmanager.h"
#define IpeBugPatch ENABLE
#define ISR_DEBUG   DISABLE
#if (ISR_DEBUG)
#define ISR_MSG(...)    debug_msg ("ISR: "__VA_ARGS__)
#else
#define ISR_MSG(...)
#endif

//dzoom flow flag define
#define DZOOM_SET_IPEIME    0x00000004
#define DZOOM_SET_CA        0x00000008
#define DZOOM_SET_SIEIPEADDR 0x00000010  //patch 220 ipe slice mode ic bug

//type define
typedef struct _IMEISRINFO
{
    UINT32 ImeSliceNum;
    _IME_MODE_SEL ImeOpmode;
    //UINT32 SieSliceNum;
    SIE_OutputModeEnum SieOpmode;
}ISRINFO,*pISRINFO;

//local value
static volatile ISR_CB m_Isr_CB = {NULL,NULL,NULL,NULL};
static volatile BOOL g_bIPLSTOP = FALSE;
static volatile StopIplStage g_IplStatus = _Start;
static volatile UINT32 gImeSliceCounter = 0;
static volatile UINT32 gIme_fc = 0;
static volatile UINT32 DZoom_Flag = DZOOM_IDLE;
static volatile UINT32 gChgModeFlag = CHGMODE_IDLE;
static ISRINFO m_IsrInfo;

//static api/////////////////////////
static void SetIPLStopStatus(StopIplStage status)
{
    //check status legal or not
    if(status!=(g_IplStatus+1) )
    {
       debug_err(("IPL stop error!(%d -> %d)\r\n",g_IplStatus,status));
       return;
    }
    g_IplStatus = status;
    StopPreviewDramData(g_IplStatus);
    if(g_IplStatus==_IPE_IME_RESET)
    {
        g_bIPLSTOP = FALSE;
        g_IplStatus = _Start;
    }
}

//extern api////////////////////////
//#NT#return TRUE if IPL during stop ,return FALSE if stop complete
UINT32 GetIPLTriggerStop(void)
{
    return g_bIPLSTOP;
}

void IPLTriggerStop(void)
{
    g_bIPLSTOP = TRUE;
}

UINT32 GetIPLStopStatus(void)
{
    return g_IplStatus;
}

void RstImeCounter(void)
{
    gImeSliceCounter = 0;
    gIme_fc = 0;
}

//#NT#get ime opmode/slicenum from driver
void SetIsrInfo(void* _Value)
{
    m_IsrInfo = *(pISRINFO)_Value;
}

void IsrCBReg(PISR_CB pIsr_cb)
{
    if(pIsr_cb != NULL)
    {
        if(pIsr_cb->SIEISR_CB != NULL)
        {
            m_Isr_CB.SIEISR_CB = pIsr_cb->SIEISR_CB;
        }
        if(pIsr_cb->IPEISR_CB != NULL)
        {
            m_Isr_CB.IPEISR_CB = pIsr_cb->IPEISR_CB;
        }
        if(pIsr_cb->IMEISR_CB != NULL)
        {
            m_Isr_CB.IMEISR_CB = pIsr_cb->IMEISR_CB;
        }
        if(pIsr_cb->IFEISR_CB != NULL)
        {
            m_Isr_CB.IFEISR_CB = pIsr_cb->IFEISR_CB;
        }
    }
}

void sie_isrCB(UINT32 IntStatus)
{
    //#NT#2011/05/20#Harry Tsai -begin
    STR_SUBOUT_Param CA_param;
    UINT32 CA_Hsize,CA_Vsize;
    UINT32 tOcnt = 1;
#if IpeBugPatch
    UINT32 AddOffset;
    STR_SIE_OUT_ADDR sie_addr = {0};
#endif
    //#NT#2011/05/20#Harry Tsai -end

    if (IntStatus & SIE_INT_SLICE_END)
    {
        //slice mode ipeime dzoom flow
        if(Get_DZoomFlag() & DZOOM_SET_IPEIME)
        {
            ime_clrIntFlag();
            Clr_DZoomFlag(DZOOM_SET_IPEIME);
#if IpeBugPatch
            //fix ipe slice mode bug.
            AddOffset = sie_getSliceHeight() * sie_getOutputLineOffset();
            IPEModeInfo.ININFO.Adr0 = sie_getDramSao(0);
            IPEModeInfo.ININFO.Adr1 = IPEModeInfo.ININFO.Adr0 + AddOffset;
            IPEModeInfo.ININFO.Adr2 = IPEModeInfo.ININFO.Adr1 + AddOffset;
#endif
            ipe_ime_set2dzoom();

#if (IpeBugPatch == DISABLE)//when patch enable, dzoom idle move 2 sie dram end
            Set_DZoomFlag(DZOOM_IDLE);
#endif
        }
    }

    if (IntStatus & SIE_INT_DRAMO_END)
    {
        //frame ipeime mode dzoom flow
        if(Get_DZoomFlag() & DZOOM_SET_IPEIME)
        {
            ime_clrIntFlag();
            Clr_DZoomFlag(DZOOM_SET_IPEIME);
            ipe_ime_set2dzoom();

            Set_DZoomFlag(DZOOM_IDLE);
        }
#if IpeBugPatch
        if(Get_DZoomFlag() & DZOOM_SET_SIEIPEADDR)
        {
            Clr_DZoomFlag(DZOOM_SET_SIEIPEADDR);
            //fix ipe slice mode bug.
            sie_addr.OutAddr0 = IPEModeInfo.ININFO.Adr0;
            sie_addr.OutAddr1 = IPEModeInfo.ININFO.Adr1;
            sie_addr.OutAddr2 = IPEModeInfo.ININFO.Adr2;
            sie_addr.OutBufNum = THREE_OUTPUT_BUF;//sie slice mode always three pingpong buf.
            sie_changeOutputAdd(CHG_ONLY_ADD_1_2,&sie_addr);
            Set_DZoomFlag(DZOOM_IDLE);
        }
#endif
    }

    if (IntStatus & SIE_INT_FLDEND)
    {
        if (Get_DZoomFlag() & DZOOM_SET_CA)
        {
            //#NT#2011/05/20#Harry Tsai -begin
            //#NT#2011/12/05#Ethan Lau -begin
            //#NT#in dzoom condition, if ratio not chg,
            //#NT#it will cause ca output too small when use aaa_GetCAResult()
            {
                UINT32 Hsub=0,Vsub=0;
                sie_getSubOutputParam(&CA_param);
                while(1)
                {
                    CA_Hsize = (((GetIPPSizeInfor(_SieOutHsize) - 1)/(1<<Hsub))>>2)<<2;
                    CA_Vsize = ((GetIPPSizeInfor(_SieOutVsize) - 1)/(1<<Vsub));
                    if( (CA_Hsize <= SIE_SubOut_H)&&(CA_Vsize <= SIE_SubOut_V) )
                    {
                        break;
                    }
                    if(CA_Hsize > SIE_SubOut_H)
                        Hsub++;
                    if(CA_Vsize > SIE_SubOut_V)
                        Vsub++;
                    tOcnt++;
                    if(tOcnt>10)
                    {
                        debug_err(("^R Sie SubOut buffer size OB!!\r\n"));
                        break;
                    }
                }
                CA_param.Hcnt = CA_Hsize;
                CA_param.Vcnt = CA_Vsize;
                CA_param.Hsub = Hsub;
                CA_param.Vsub = Vsub;
            }
            /*
            //#NT#2011/06/02#Ethan Lau -begin
            //#NT#lineoffset can't be chged,and subout cnt need to be chged
            sie_getSubOutputParam(&CA_param);
            CA_Hsize = (((GetIPPSizeInfor(_SieOutHsize) - 1)/(1<<CA_param.Hsub))>>2)<<2;
            CA_Vsize = ((GetIPPSizeInfor(_SieOutVsize) - 1)/(1<<CA_param.Vsub));
            CA_param.Hcnt = CA_Hsize;
            CA_param.Vcnt = CA_Vsize;
            //#NT#2011/09/13#Ethan Lau -begin
            //#NT#incase sub-out size bigger than 320x240
            while(1)
            {
                if(CA_param.Hcnt > SIE_SubOut_H)
                    CA_param.Hsub++;
                if(CA_param.Vcnt > SIE_SubOut_V)
                    CA_param.Vsub++;
                CA_param.Hcnt = (((GetIPPSizeInfor(_SieOutHsize) - 1)/(1<<CA_param.Hsub))>>2)<<2;
                CA_param.Vcnt = ((GetIPPSizeInfor(_SieOutVsize) - 1)/(1<<CA_param.Vsub));
                if( (CA_param.Hcnt <= SIE_SubOut_H)&&(CA_param.Vcnt <= SIE_SubOut_V) )
                {
                    break;
                }
                tOcnt++;
                if(tOcnt>10)
                {
                    debug_err(("^R Sie SubOut buffer size OB!!\r\n"));
                    break;
                }
            }
            //#NT#2011/09/13#Ethan Lau -end
            */
            sie_changeSubOutParam(&CA_param);
            SetIPPSizeInfor(_CAHsize, CA_Hsize);
            SetIPPSizeInfor(_CAVsize, CA_Vsize);
            Clr_DZoomFlag(DZOOM_SET_CA);
            //#NT#2011/06/02#Ethan Lau -end
            //#NT#2011/12/05#Ethan Lau -end
            //#NT#2011/05/20#Harry Tsai -end
        }

        if (Get_DZoomFlag() & DZOOM_SET_SIEPRE)
        {
            Clr_DZoomFlag(DZOOM_SET_SIEPRE);
            sie_Set2Dzoom();

#if IpeBugPatch
            //patch 220 ipe slice mode ic bug
            if(m_IsrInfo.SieOpmode == SIE_SLICE_MODE)
            {
                Set_DZoomFlag(DZOOM_SET_SIEIPEADDR);
            }
#endif
            Set_DZoomFlag(DZOOM_SET_IPEIME);
            Set_DZoomFlag(DZOOM_SET_CA);

        }

        if (Get_ChgModeFlag() & CHGMODE_SIE_SET)
        {
            Clr_ChgModeFlag(CHGMODE_SIE_SET);
            Set_ChgModeFlag(CHGMODE_IME_SET);
            sie_pause();
        }

        if(GetIPLStopStatus()==_SIE_PAUSE)
        {
            SetIPLStopStatus(_IPE_IME_RESET);
        }
    }

    if (IntStatus & SIE_INT_BP1)
    {
    }

    if (IntStatus & SIE_INT_BP2)
    {
    }

    if(m_Isr_CB.SIEISR_CB)
    {
        m_Isr_CB.SIEISR_CB(IntStatus);
    }
    Sen_IsrEventProc(SEN_ISR_PHOTO_SIE, IntStatus);
}

void ipe_isrCB(UINT32 IntStatus)
{
    if(m_Isr_CB.IPEISR_CB)
    {
        m_Isr_CB.IPEISR_CB(IntStatus);
    }
    Sen_IsrEventProc(SEN_ISR_PHOTO_IPE, IntStatus);
}


void ime_isrCB(UINT32 IntStatus)
{
    if(IntStatus &IME_INT_SLICE_DN1)
    {
#if ISR_DEBUG
        if(ime_getEngineInfo(IME_GET_TRISLICE_STATUS) == 1)
        {
            debug_err(("^R IME "));
        }
#endif
        if( (gImeSliceCounter%m_IsrInfo.ImeSliceNum) == (m_IsrInfo.ImeSliceNum-1) )
        {
            // ime last slice done of a frame,next frame ime trig is at frame end.
            ISR_MSG(" ~ ");
        }
        else
        {
            ISR_MSG(" + ");
            Dcout_TrigCtrl(DCOUT_ISR_IME);
        }

        gImeSliceCounter++;
    }

    if (IntStatus & IME_INT_FRM)
    {
        if(g_fpAlgExport)
        {
            g_fpAlgExport(ALGMSG_IMEFRMEND, &gAlgMsg);
        }
#if ISR_DEBUG
        if(ime_getEngineInfo(IME_GET_TRISLICE_STATUS) == 1)
        {
            debug_err(("^R IME "));
        }
#endif
        if (Get_ChgModeFlag() & CHGMODE_WAIT_IMEEND)
        {
            Clr_ChgModeFlag(CHGMODE_WAIT_IMEEND);
            Set_ChgModeFlag(CHGMODE_IDLE);
        }

        if(g_bIPLSTOP)
        {
            if(GetIPLStopStatus()==_IPEIME_PAUSE)
            {
                SetIPLStopStatus(_SIE_PAUSE);
            }
            else if(GetIPLStopStatus()== _Start)
            {
                SetIPLStopStatus(_IPEIME_PAUSE);
                if(m_IsrInfo.ImeOpmode == IPE2IME_SLICE_MODE)
                    Dcout_TrigCtrl(DCOUT_ISR_IME);//ime trigger last frame
            }
        }
        else
        {
            if(m_IsrInfo.ImeOpmode == IPE2IME_SLICE_MODE)
            {
                Dcout_TrigCtrl(DCOUT_ISR_IME);//ime trigger next frame
            }
            else
            {
                //in ime slice mode, IDE fw ctrl event alyway be sent at jpg frame end.
                //in ime frame mode, IDE fw ctrl event only be sent when dcout disable,
                //                   if dcout enable,event be sent at jpg frame end.
                if(GetIPPFuncInfor(JPG_DCOUT_EN) != ENABLE)
                    Sen_IsrEventProc(SEN_ISR_PHOTODISP_IDE, IntStatus);//ctrl ide idx
            }
        }
        ISR_MSG(" !%d \r\n",Get_DzoomIndex());
        gIme_fc++;
        gImeSliceCounter = 0;//rst , incase of last slice done have not interrupt occurred
    }

    if(m_Isr_CB.IMEISR_CB)
    {
        m_Isr_CB.IMEISR_CB(IntStatus);
    }
    Sen_IsrEventProc(SEN_ISR_PHOTO_IME, IntStatus);
}

void ife_isrCB(UINT32 IntStatus)
{
    if(m_Isr_CB.IFEISR_CB)
    {
        m_Isr_CB.IFEISR_CB(IntStatus);
    }
}

void Set_DZoomFlag(UINT32 Flag)
{
    DZoom_Flag |= Flag;
}

void Clr_DZoomFlag(UINT32 Flag)
{
    DZoom_Flag &= (~Flag);
}

UINT32 Get_DZoomFlag(void)
{
    return DZoom_Flag;
}

void Clr_ChgModeFlag(UINT32 Flag)
{
    gChgModeFlag &= (~Flag);
}

UINT32 Get_ChgModeFlag(void)
{
    return gChgModeFlag;
}

void Set_ChgModeFlag(UINT32 Flag)
{
    gChgModeFlag |= Flag;
}