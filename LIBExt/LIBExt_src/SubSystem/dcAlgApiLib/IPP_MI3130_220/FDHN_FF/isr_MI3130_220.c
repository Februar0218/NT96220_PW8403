/*
    ISR

    This file is the API of the IPL control.

    @Sensor     Micron MI3130
    @file       isr_MI3130_220.c
    @ingroup        IPP_MI3130_220
    @note       Nothing.

    Porting by Randy Ho (ºÎ˜s¾ý)    2012/03/08

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#if MI3130_220
#include "IPP_MI3130_220_int.h"

void MI3130_sie_isrCB(UINT32 IntStatus)
{
    //customized op
    if (IntStatus & SIE_INT_DRAMO_END)
    {
    }

    if (IntStatus & SIE_INT_FLDEND)
    {
        //#NT#2011/11/24#Jarkko Chang -begin
        AF_SetFrmCnt(SIE_CNT);
        AF_SetFrmCnt(PRE_CNT);
        //#NT#2011/11/24#Jarkko Chang -end
    }

    if (IntStatus & SIE_INT_BP1)
    {
    }

    if (IntStatus & SIE_INT_BP2)
    {
    }

    return;
}

void MI3130_ime_isrCB(UINT32 IntStatus)
{
    //customized op
    if(IntStatus &IME_INT_SLICE_DN1)
    {
    }

    if (IntStatus & IME_INT_FRM)
    {
        if(g_fpAlgExport)
        {
            g_fpAlgExport(ALGMSG_IMEFRMEND, &gAlgMsg);
        }
    }

    return;
}

void MI3130_ipe_isrCB(UINT32 IntStatus)
{
    //customized op
    if (IntStatus & IPE_INT_FRM)
    {
        //#NT#2011/11/24#Jarkko Chang -begin
        AF_SetFrmCnt(IPE_CNT);
        //#NT#2011/11/24#Jarkko Chang -end
    }

    if (IntStatus & IPE_INT_ST)
    {
    }

    if (IntStatus & IPE_INT_SLI)
    {
    }

    return;
}

void MI3130_ife_isrCB(UINT32 IntStatus)
{
    //customized op
    if (IntStatus & IFE_INTE_FRMEND)
    {
    }

    return;
}
#endif

