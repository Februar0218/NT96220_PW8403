#if NT99143_220
#include "IPP_NT99143_220_int.h"
#include "IQSetting_NT99143_220.h"   //#include local folder .h file

UINT8 bGammaTrigger = FALSE;
UINT32 sie_gamma_tbl[65];
UINT32 auto_gamma_weight;

void NT99143_sie_isrCB(UINT32 IntStatus)
{
	UINT32 i;
	
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
        #if 0
		for(i=0; i<65; i++)
			sie_gamma_tbl[i] = (((AUTO_GAMMA_ISO_RANGE-auto_gamma_weight)*SRA_SIE_GammaLUT[i]) + (auto_gamma_weight * SRA_SIE_GammaLUT_Night[i]))/AUTO_GAMMA_ISO_RANGE;
		#endif

		if(bGammaTrigger == TRUE)
		{
			bGammaTrigger = FALSE;
			sra_setGammaTable(SRA_READBY_SIE, sie_gamma_tbl);
		}
		//debug_err(("Index = %d\r\n", sie_gamma_tbl[10]));
    }
    if (IntStatus & SIE_INT_BP1)
    {
    }
    if (IntStatus & SIE_INT_BP2)
    {
    }
}

void NT99143_ime_isrCB(UINT32 IntStatus)
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
void NT99143_ipe_isrCB(UINT32 IntStatus)
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

void NT99143_ife_isrCB(UINT32 IntStatus)
{
    //customized op
    if (IntStatus & IFE_INTE_FRMEND)
    {
    }
}
#endif
