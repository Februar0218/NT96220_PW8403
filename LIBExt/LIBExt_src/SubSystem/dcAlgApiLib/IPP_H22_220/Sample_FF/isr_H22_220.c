#if H22_220
#include "IPP_H22_220_int.h"

void H22_sie_isrCB(UINT32 IntStatus)
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
}

void H22_ime_isrCB(UINT32 IntStatus)
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
}
void H22_ipe_isrCB(UINT32 IntStatus)
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
}

void H22_ife_isrCB(UINT32 IntStatus)
{
    //customized op
    if (IntStatus & IFE_INTE_FRMEND)
    {
    }
}
#endif
