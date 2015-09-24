#include "ImageQualitySim_int.h"

#if (CAP_DEBUG_FLOW == ENABLE)
#define debug_cap   debug_err
#else
#define debug_cap   debug_ind
#endif

#if (CAP_DEBUG_FLOW2 == ENABLE)
#define debug_cap2   debug_err
#else
#define debug_cap2   debug_ind
#endif

BOOL gb_sie_dump = FALSE;

BOOL gb_ife_dump = FALSE;

BOOL gb_ime_dump = FALSE;

BOOL gb_ife2_dump = FALSE;

BOOL gb_jpg_dump = FALSE;


UINT32 Cap_SliceMode_GetSliceCount(void)
{
    //output buffer number
    return IME_OUTPUT_PPB_5; //for current 220 DRAM size and bandwidth
}
UINT32 Cap_SliceMode_GetSliceHeight(void)
{
    //output slice height
    return 32; //for current 220 DRAM size and bandwidth
}

//////////////////////////////////////////////////////////////////

static STR_SIE_REG SieParam = {0};

void IQSim_SIE_ProcessRAW_Frame(int IQSimMode, UINT32 times_RAW)
{
    #if (RAW_DEBUG_DUMP == ENABLE)
    debug_msg("RAW w,h: %dx%d \r\n", gBuf_RAW.w, gBuf_RAW.h);
    debug_msg("RAW addr:0x%x size:%d\r\n", gBuf_RAW.addr[0], gBuf_RAW.size);
    IQ_BUF_Dump(&gBuf_RAW, "A:\\IQSIM\\RAW", 0, 0);
    debug_msg("RAW ok\r\n");
    #endif

    memset(&SieParam, 0, sizeof(STR_SIE_REG));
    
    IQSim_SIE_Open();
    
    IQSim_SIE_SetSrc(&SieParam, IQSimMode);
    IQSim_SIE_SetSrcModifier(&SieParam, IQSimMode);
    IQSim_SIE_SetDest(&SieParam, IQSimMode, times_RAW);
    IQSim_SIE_SetDestModifier(&SieParam, IQSimMode);
    IQSim_SIE_SetIQ_2(&SieParam, IQSimMode);
    IQSim_SIE_Start(&SieParam, IQSimMode, SIE_FRAME_MODE); //capture is using frame mode
    IQSim_SIE_Wait();
    #if (CAP_DEBUG_DUMP == ENABLE)
    IQSim_SIE_DumpSetting();
    #endif

    //IQSim_SIE_Stop();
    IQSim_SIE_Close();
    
    #if (SIE_DEBUG_DUMP == ENABLE)
    if(gb_sie_dump)
    {
        debug_msg("SIE dump...\r\n");
        IQ_BUF_Dump(&gBuf_SIE, "A:\\IQSIM\\SIE", 0, 0);
        debug_msg("SIE dump ok: w,h=%dx%d, loff=%d, size=%d\r\n", gBuf_SIE.w, gBuf_SIE.h, gBuf_SIE.lineoff, gBuf_SIE.size);
    }        
    #endif
}

//////////////////////////////////////////////////////////////////

static IFE_PARAM IfeParam = {0};

int IQSim_IFE_GetTimes(IQ_CNL cnl)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_BASIC_IO_REG_STRUCT* pBasic = &(pBin->Basic); 
    if(cnl == IQ_CNL_RAW)
    {
        BOOL bIfeRaw = pBasic->BASIC_IO_Register_4.Bit.IFE_RAW_EN;
        NT96220_IFE_REG_STRUCT* pIfeRaw = &(pBin->IfeRaw); 
        int nIfeRaw = pIfeRaw->IFE_Register_0.Bit.IFE_TIMES;
#ifdef SHIFT_IFE_RAW
        if(nIfeRaw > IFE_RAW_TIMES_MAX)nIfeRaw=IFE_RAW_TIMES_MAX;
#else
        if(nIfeRaw > 1)nIfeRaw=1;
#endif
        if(nIfeRaw == 0)bIfeRaw = FALSE;
        if(bIfeRaw == FALSE)nIfeRaw = 0;
        debug_cap(("IFE raw en=%d times=%d\r\n", bIfeRaw, nIfeRaw));
        return nIfeRaw;
    }
    else
    if(cnl == IQ_CNL_Y)
    {
        BOOL bIfeY = pBasic->BASIC_IO_Register_4.Bit.IFE_Y_EN;
        NT96220_IFE_REG_STRUCT* pIfeY = &(pBin->IfeY); 
        int nIfeY = pIfeY->IFE_Register_0.Bit.IFE_TIMES;
#ifdef SHIFT_IFE_YCC
        if(nIfeY > IFE_YCC_TIMES_MAX)nIfeY=IFE_YCC_TIMES_MAX;
#else
        if(nIfeY > 1)nIfeY=1;
#endif
        if(nIfeY == 0)bIfeY = FALSE;
        if(bIfeY == FALSE)nIfeY = 0;
        debug_cap(("IFE y en=%d times=%d\r\n", bIfeY, nIfeY));
        return nIfeY;
    }
    else
    if((cnl == IQ_CNL_Cb) || (cnl == IQ_CNL_Cr))
    {
        BOOL bIfeCC = pBasic->BASIC_IO_Register_4.Bit.IFE_CX_EN;
        NT96220_IFE_REG_STRUCT* pIfeCC = &(pBin->IfeCC); 
        int nIfeCC = pIfeCC->IFE_Register_0.Bit.IFE_TIMES;
#ifdef SHIFT_IFE_YCC
        if(nIfeCC > IFE_YCC_TIMES_MAX)nIfeCC=IFE_YCC_TIMES_MAX;
#else
        if(nIfeCC > 1)nIfeCC=1;
#endif
        if(nIfeCC == 0)bIfeCC = FALSE;
        if(bIfeCC == FALSE)nIfeCC = 0;
        debug_cap(("IFE cc en=%d times=%d\r\n", bIfeCC, nIfeCC));
        return nIfeCC;
    }
    return 1;
}

void IQSim_IFE_ProcessRAW_Frame(int IQSimMode, UINT32 times_RAW)
{
    UINT32 i;

    memset(&IfeParam, 0, sizeof(IFE_PARAM));
    
    if(times_RAW)
    {
    IQSim_IFE_Open();

    IQSim_IFE_BufBegin(IQ_CNL_RAW, 0, times_RAW);
    for(i = 0;i < times_RAW; i++)
    {
        #if ((CAP_DEBUG_FLOW == ENABLE) && (CAP_DEBUG_FLOW2 != ENABLE))
        debug_cap(("CAP do IFE frame...%d\r", i+1));
        #endif
        //process RAW channel
        IQSim_IFE_SetSrc(&IfeParam, IQSimMode, IQ_CNL_RAW, 0);
        IQSim_IFE_SetDest(&IfeParam, IQSimMode, IQ_CNL_RAW, 0);
        IQSim_IFE_SetIQ_2(&IfeParam, IQSimMode, IQ_CNL_RAW, i);
        IQSim_IFE_Start(&IfeParam, IQSimMode, IFE_MODE_D2D);
        IQSim_IFE_Wait();
        IQSim_IFE_BufNext(IQ_CNL_RAW);
        #if (CAP_DEBUG_DUMP == ENABLE)
        IQSim_IFEraw_DumpSetting(i);
        #endif
    }
    IQSim_IFE_BufEnd(IQ_CNL_RAW, 0);
    #if ((CAP_DEBUG_FLOW == ENABLE) && (CAP_DEBUG_FLOW2 != ENABLE))
    debug_cap(("\r\n"));
    #endif
    
    IQSim_IFE_Close();
    }
    else
    {
    IQSim_IFE_RawDummy();  
    //#if (CAP_DEBUG_DUMP == ENABLE)
    //IQSim_IFEraw_DumpSetting();
    //#endif
    }
    
    #if (IFE_DEBUG_DUMP == ENABLE)
    if(gb_ife_dump)
    {
        debug_msg("IFE dump...\r\n");
        IQ_BUF_Dump(&gBuf_SIE, "A:\\IQSIM\\IFE", 0, 0);
        debug_msg("IFE dump ok: w,h=%dx%d, loff=%d, size=%d\r\n", gBuf_SIE.w, gBuf_SIE.h, gBuf_SIE.lineoff, gBuf_SIE.size);
    }        
    #endif
}

//////////////////////////////////////////////////////////////////

static IME_MODE_PRAM ImeParam = {0};
static IME_IQ_FLOW_INFO ImeIqInfo = {0};
static IPE_MODEINFO IpeParam = {0};
static UINT32 ImeIqCrvmap[16] = {0};

#if (IME_DEBUG_DUMP == ENABLE)
static UINT32 IME_y_offset = 0;
static UINT32 IME_cb_offset = 0;
static UINT32 IME_cr_offset = 0;
#endif

void IQSim_IME_ProcessYCC_Slice_Begin(int IQSimMode, SLICE_CTRL* pSliceCtrl)
{
    memset(&ImeParam, 0, sizeof(IME_MODE_PRAM));
    memset(&ImeIqInfo, 0, sizeof(IME_IQ_FLOW_INFO));
    memset(&IpeParam, 0, sizeof(IPE_MODEINFO));
    
    if(pSliceCtrl->ime_id == 0) //first slice
    {
        pSliceCtrl->ime_h = 0;
        #if (IME_DEBUG_DUMP == ENABLE)
        IME_y_offset = 0;
        IME_cb_offset = 0;
        IME_cr_offset = 0;
        #endif        
    }        
    if(pSliceCtrl->ime_id == 0) //first slice
    {
        ImeParam.imeIqInfo = &ImeIqInfo;
        ImeIqInfo.CurveMappingInfo.uCrvMapLUT = ImeIqCrvmap;
        
        IQSim_IME_SetSrc(&ImeParam, IQSimMode);
        IQSim_IME_SetDest(&ImeParam, IQSimMode, pSliceCtrl);
        IQSim_IME_SetScaleIQ(&ImeParam, IQSimMode);
        IQSim_IME_SetIQ_2(&ImeParam, IQSimMode);

        IQSim_IPE_SetSrc(&IpeParam, IQSimMode);
        IQSim_IPE_SetDest(&IpeParam, IQSimMode);
        IQSim_IPE_SetIQ_2(&IpeParam, IQSimMode);
        
        debug_cap2(("CAP slice IME %d buf[in=%d,out=%d]\r\n", pSliceCtrl->ime_id, pSliceCtrl->iid, pSliceCtrl->iid));
        IQSim_IME_Start(&ImeParam, IQSimMode, IPE2IME_SLICE_MODE);
        IQSim_IPE_Start(&IpeParam, IQSimMode, IPE_AUTOSTP);
    }
    else if ((pSliceCtrl->ime_id > 0) && (pSliceCtrl->ime_id < pSliceCtrl->total_cnt)) //other slice
    {
        debug_cap2(("CAP slice IME %d buf[in=%d,out=%d]\r\n", pSliceCtrl->ime_id, pSliceCtrl->iid, pSliceCtrl->iid));
        IQSim_IME_Next();
    }
}

void IQSim_IME_ProcessYCC_Slice_End(int IQSimMode, SLICE_CTRL* pSliceCtrl)
{
    if ((pSliceCtrl->ime_id >= 0) && (pSliceCtrl->ime_id < pSliceCtrl->total_cnt))
    {
        IQSim_IME_Wait();
        
        #if (CAP_DEBUG_DUMP == ENABLE)
        if(pSliceCtrl->ime_id == 1)
        {
            IQSim_IPE_DumpSetting();
            IQSim_IME_DumpSetting();
        }
        #endif
        
        pSliceCtrl->ime_h += pSliceCtrl->slice_h; //ime process h
        if(pSliceCtrl->ime_h > pSliceCtrl->total_h)
            pSliceCtrl->ime_h = pSliceCtrl->total_h;
        
        #if (IME_DEBUG_DUMP == ENABLE)
        if(gb_ime_dump)
        {
            IME_y_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->iid], "A:\\IQSIM\\IME", 0, IME_y_offset);
            IME_cb_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->iid], "A:\\IQSIM\\IME", 1, IME_cb_offset);
            IME_cr_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->iid], "A:\\IQSIM\\IME", 2, IME_cr_offset);
        }
        #endif
    }
}

//////////////////////////////////////////////////////////////////

static IFE_PARAM IfeParam2 = {0};

#if (IFE2_DEBUG_DUMP == ENABLE)
/*
static UINT32 IFE_ya_offset = 0;
static UINT32 IFE_cba_offset = 0;
static UINT32 IFE_cra_offset = 0;
static UINT32 IFE_yb_offset = 0;
static UINT32 IFE_cbb_offset = 0;
static UINT32 IFE_crb_offset = 0;
static UINT32 IFE_yc_offset = 0;
static UINT32 IFE_cbc_offset = 0;
static UINT32 IFE_crc_offset = 0;
*/
static UINT32 IFE_y_offset = 0;
static UINT32 IFE_cb_offset = 0;
static UINT32 IFE_cr_offset = 0;
#endif

void IQSim_IFE_ProcessYCC_Slice_Begin(int IQSimMode, SLICE_CTRL* pSliceCtrl, int nIfeY, int nIfeCC)
{
    memset(&IfeParam2, 0, sizeof(IFE_PARAM));

    if (pSliceCtrl->ife_id == 0)
    {
        pSliceCtrl->ife_h = 0;
        #if (IFE2_DEBUG_DUMP == ENABLE)
        /*
        IFE_ya_offset = 0;
        IFE_cba_offset = 0;
        IFE_cra_offset = 0;
        IFE_yb_offset = 0;
        IFE_cbb_offset = 0;
        IFE_crb_offset = 0;
        IFE_yc_offset = 0;
        IFE_cbc_offset = 0;
        IFE_crc_offset = 0;
        */
        IFE_y_offset = 0;
        IFE_cb_offset = 0;
        IFE_cr_offset = 0;
        #endif  
    }
    if ((pSliceCtrl->ife_id >= 0) && (pSliceCtrl->ife_id < (INT32)pSliceCtrl->total_cnt))
    {
        debug_cap2(("CAP slice IFE %d buf[in=%d,%d,%d,out=%d]\r\n", pSliceCtrl->ife_id, pSliceCtrl->did, pSliceCtrl->cid, pSliceCtrl->uid, pSliceCtrl->did));

        #if (IFE2_DEBUG_DUMP == ENABLE)
        if(gb_ife2_dump)
        {
            /*
            IFE_ya_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->did], "A:\\IQSIM\\IFE2A", 0, IFE_ya_offset);
            IFE_cba_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->did], "A:\\IQSIM\\IFE2A", 1, IFE_cba_offset);
            IFE_cra_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->did], "A:\\IQSIM\\IFE2A", 2, IFE_cra_offset);
            IFE_yb_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->cid], "A:\\IQSIM\\IFE2B", 0, IFE_yb_offset);
            IFE_cbb_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->cid], "A:\\IQSIM\\IFE2B", 1, IFE_cbb_offset);
            IFE_crb_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->cid], "A:\\IQSIM\\IFE2B", 2, IFE_crb_offset);
            IFE_yc_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->uid], "A:\\IQSIM\\IFE2C", 0, IFE_yc_offset);
            IFE_cbc_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->uid], "A:\\IQSIM\\IFE2C", 1, IFE_cbc_offset);
            IFE_crc_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->uid], "A:\\IQSIM\\IFE2C", 2, IFE_crc_offset);
            */
        }
        #endif
        
        //copy from 3 slices to ife tmp slice
        IQSim_IFE_BufBegin(IQ_CNL_Y, pSliceCtrl, nIfeY); 
        if(nIfeY)
        {
            int i;
            for(i=0;i<nIfeY;i++)
            {
                //process Y channel
                IQSim_IFE_SetSrc(&IfeParam2, IQSimMode, IQ_CNL_Y, pSliceCtrl);
                IQSim_IFE_SetDest(&IfeParam2, IQSimMode, IQ_CNL_Y, pSliceCtrl);
                IQSim_IFE_SetIQ_2(&IfeParam2, IQSimMode, IQ_CNL_Y, i);
                IQSim_IFE_Start(&IfeParam2, IQSimMode, IFE_MODE_D2D); 
                IQSim_IFE_Wait();
                if((nIfeY>1) && (i<nIfeY-1)) //more then 1 and not last
                {
                    IQSim_IFE_BufNext(IQ_CNL_Y);
                }
                #if (CAP_DEBUG_DUMP == ENABLE)
                if(i == 0)
                    IQSim_IFEy_DumpSetting();
                #endif
            }
        }                
        //copy back current slice from ife tmp slice
        IQSim_IFE_BufEnd(IQ_CNL_Y, pSliceCtrl); 
        
        //copy from 3 slices to ife tmp slice
        IQSim_IFE_BufBegin(IQ_CNL_Cb, pSliceCtrl, nIfeCC);
        if(nIfeCC)
        {
            int i;
            for(i=0;i<nIfeCC;i++)
            {
                //process Cb channel
                IQSim_IFE_SetSrc(&IfeParam2, IQSimMode, IQ_CNL_Cb, pSliceCtrl);
                IQSim_IFE_SetDest(&IfeParam2, IQSimMode, IQ_CNL_Cb, pSliceCtrl);
                IQSim_IFE_SetIQ_2(&IfeParam2, IQSimMode, IQ_CNL_Cb, i);
                IQSim_IFE_Start(&IfeParam2, IQSimMode, IFE_MODE_D2D);
                IQSim_IFE_Wait();
                if((nIfeCC>1) && (i<nIfeCC-1)) //more then 1 and not last
                {
                    IQSim_IFE_BufNext(IQ_CNL_Cb);
                }
                #if (CAP_DEBUG_DUMP == ENABLE)
                if(i == 0)
                    IQSim_IFEcc_DumpSetting();
                #endif
            }
        }
        //copy back current slice from ife tmp slice
        IQSim_IFE_BufEnd(IQ_CNL_Cb, pSliceCtrl); 
        
        //copy from 3 slices to ife tmp slice
        IQSim_IFE_BufBegin(IQ_CNL_Cr, pSliceCtrl, nIfeCC);
        if(nIfeCC)
        {
            int i;
            for(i=0;i<nIfeCC;i++)
            {
                //process Cr channel
                IQSim_IFE_SetSrc(&IfeParam2, IQSimMode, IQ_CNL_Cr, pSliceCtrl);
                IQSim_IFE_SetDest(&IfeParam2, IQSimMode, IQ_CNL_Cr, pSliceCtrl);
                IQSim_IFE_SetIQ_2(&IfeParam2, IQSimMode, IQ_CNL_Cr, i);
                IQSim_IFE_Start(&IfeParam2, IQSimMode, IFE_MODE_D2D);
                IQSim_IFE_Wait();
                if((nIfeCC>1) && (i<nIfeCC-1)) //more then 1 and not last
                {
                    IQSim_IFE_BufNext(IQ_CNL_Cr);
                }
                #if (CAP_DEBUG_DUMP == ENABLE)
                if(i == 0)
                    IQSim_IFEcc_DumpSetting();
                #endif
            }
        }
        //copy back current slice from ife tmp slice
        IQSim_IFE_BufEnd(IQ_CNL_Cr, pSliceCtrl);
    }
}

void IQSim_IFE_ProcessYCC_Slice_End(int IQSimMode, SLICE_CTRL* pSliceCtrl)
{
    if ((pSliceCtrl->ife_id >= 0) && (pSliceCtrl->ife_id < (INT32)pSliceCtrl->total_cnt))
    {
        pSliceCtrl->ife_h += pSliceCtrl->slice_h; //ife process h
        if(pSliceCtrl->ife_h > pSliceCtrl->total_h)
            pSliceCtrl->ife_h = pSliceCtrl->total_h;
        
        #if (IFE2_DEBUG_DUMP == ENABLE)
        if(gb_ife2_dump)
        {
            IFE_y_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->did], "A:\\IQSIM\\IFE2", 0, IFE_y_offset);
            IFE_cb_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->did], "A:\\IQSIM\\IFE2", 1, IFE_cb_offset);
            IFE_cr_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->did], "A:\\IQSIM\\IFE2", 2, IFE_cr_offset);
        }
        #endif
    }
}


//////////////////////////////////////////////////////////////////
static JPGCmpParam JPGParam = {0};
static UINT JPG_BSize = 0;

#if (JPG_DEBUG_DUMP == ENABLE)
/*
static UINT32 JPG_y_offset = 0;
static UINT32 JPG_cb_offset = 0;
static UINT32 JPG_cr_offset = 0;
*/
#endif

void IQSim_JPG_ProcessYCC_Slice_Begin(int IQSimMode, SLICE_CTRL* pSliceCtrl)
{
    memset(&JPGParam, 0, sizeof(JPGCmpParam));

    if (pSliceCtrl->jpg_id == 0) //first slice
    {
        pSliceCtrl->jpg_h = 0;
        JPGParam.JPGBSBufSize = &JPG_BSize;
        IQSim_JPG_SetIQ_2(&JPGParam, IQSimMode);
        IQSim_JPG_SetSrc(&JPGParam, IQSimMode, pSliceCtrl);
        IQSim_JPG_SetDest(&JPGParam, IQSimMode);
        
        #if (JPG_DEBUG_DUMP == ENABLE)
        if(gb_jpg_dump)
        {
            /*
            JPG_y_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->jid], "A:\\IQSIM\\JPG", 0, JPG_y_offset);
            JPG_cb_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->jid], "A:\\IQSIM\\JPG", 1, JPG_cb_offset);
            JPG_cr_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->jid], "A:\\IQSIM\\JPG", 2, JPG_cr_offset);
            */
        }
        #endif
        
        //slice start
        IQSim_JPG_Start(&JPGParam, IQSimMode, TRUE);
        debug_cap2(("CAP slice JPG %d buf[in=%d,out=%d]\r\n", pSliceCtrl->jpg_id, pSliceCtrl->jid, pSliceCtrl->jid));
    }
    else if ((pSliceCtrl->jpg_id > 0) && (pSliceCtrl->jpg_id < (INT32)pSliceCtrl->total_cnt)) //other slice
    {
        IQSim_JPG_SetSrc(&JPGParam, IQSimMode, pSliceCtrl);
        
        #if (JPG_DEBUG_DUMP == ENABLE)
        if(gb_jpg_dump)
        {
            /*
            JPG_y_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->jid], "A:\\IQSIM\\JPG", 0, JPG_y_offset);
            JPG_cb_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->jid], "A:\\IQSIM\\JPG", 1, JPG_cb_offset);
            JPG_cr_offset += IQ_BUF_Dump(&gBuf_IME[pSliceCtrl->jid], "A:\\IQSIM\\JPG", 2, JPG_cr_offset);
            */
        }
        #endif
        
        //next slice start
        IQSim_JPG_Next();
        debug_cap2(("CAP slice JPG %d buf[in=%d,out=%d]\r\n", pSliceCtrl->jpg_id, pSliceCtrl->jid, pSliceCtrl->jid));
    }
}
void IQSim_JPG_ProcessYCC_Slice_End(int IQSimMode, SLICE_CTRL* pSliceCtrl)
{
    if ((pSliceCtrl->jpg_id >= 0) && (pSliceCtrl->jpg_id < (INT32)pSliceCtrl->total_cnt-1))
    {
        //wait slice end
        pSliceCtrl->uiJpgStatus = IQSim_JPG_Wait();
        if (pSliceCtrl->uiJpgStatus  & _JPEG_Buf_End)
        {
            debug_err(("CAP slice JPG error: OUT OF BUFFER!\r\n"));
            pSliceCtrl->jpg_id = pSliceCtrl->total_cnt; //abort process
            
        }            
        pSliceCtrl->jpg_h += pSliceCtrl->slice_h; //ife process h
#if 0
        if(pSliceCtrl->uiJpgStatus & _JPEG_Slice_Done)
            debug_msg("ok\r\n");
        else
            debug_msg("fail\r\n");
#endif  
     }
    if (pSliceCtrl->jpg_id == (INT32)pSliceCtrl->total_cnt-1)
    {
        //wait frame end
        pSliceCtrl->uiJpgStatus = IQSim_JPG_Wait();
        if (pSliceCtrl->uiJpgStatus  & _JPEG_Buf_End)
        {
            debug_err(("CAP slice JPG error: OUT OF BUFFER!\r\n"));
            pSliceCtrl->jpg_id = pSliceCtrl->total_cnt; //abort process
            
        }            
        pSliceCtrl->jpg_h += pSliceCtrl->slice_h; //ife process h
#if 0
        if(pSliceCtrl->jpg_h > pSliceCtrl->total_h)
            pSliceCtrl->jpg_h = pSliceCtrl->total_h;
        if(pSliceCtrl->uiJpgStatus & _JPEG_Frame_End)
            debug_msg("ok\r\n");
        else
            debug_msg("fail\r\n");
#endif
    }
}

//////////////////////////////////////////////////////////////////

void SLICE_Init(SLICE_CTRL* pSliceCtrl)
{
    UINT32 buf_n = pSliceCtrl->total_id;
    
    //buf id
    pSliceCtrl->curr_cnt = 0;
    pSliceCtrl->ime_id = pSliceCtrl->curr_cnt;
    pSliceCtrl->ife_id = pSliceCtrl->ime_id-1;
    pSliceCtrl->jpg_id = pSliceCtrl->ife_id-0;
    pSliceCtrl->curr_id = 0;
    pSliceCtrl->iid = 0;
    pSliceCtrl->uid = 0;
    pSliceCtrl->cid = buf_n-1;
    pSliceCtrl->did = buf_n-2;
    pSliceCtrl->jid = buf_n-2;
}

void SLICE_Next(SLICE_CTRL* pSliceCtrl)
{
    UINT32 buf_n = pSliceCtrl->total_id;
    
    //buf id
    pSliceCtrl->curr_cnt ++;
    pSliceCtrl->ime_id = pSliceCtrl->curr_cnt;
    pSliceCtrl->ife_id = pSliceCtrl->ime_id-1;
    pSliceCtrl->jpg_id = pSliceCtrl->ife_id-0;
    pSliceCtrl->curr_id ++; if(pSliceCtrl->curr_id >= buf_n) pSliceCtrl->curr_id -= buf_n;
    pSliceCtrl->iid ++; if(pSliceCtrl->iid >= buf_n) pSliceCtrl->iid -= buf_n;
    pSliceCtrl->uid ++; if(pSliceCtrl->uid >= buf_n) pSliceCtrl->uid -= buf_n;
    pSliceCtrl->cid ++; if(pSliceCtrl->cid >= buf_n) pSliceCtrl->cid -= buf_n;
    pSliceCtrl->did ++; if(pSliceCtrl->did >= buf_n) pSliceCtrl->did -= buf_n;
    pSliceCtrl->jid ++; if(pSliceCtrl->jid >= buf_n) pSliceCtrl->jid -= buf_n;
}

BOOL SLICE_IsEnd(SLICE_CTRL* pSliceCtrl)
{
    return (pSliceCtrl->jpg_id >= pSliceCtrl->total_cnt);
}

void SLICE_Exit(SLICE_CTRL* pSliceCtrl)
{
}


void IQSim_IPEIMEIFEJPG_ProcessYCC_Slice(int IQSimMode, int times_Y, int times_CC)
{
    UINT32 buf_n = 0;
    UINT32 buf_h = 0;
    SLICE_CTRL SliceCtrl = {0};

    #if (IME_DEBUG_DUMP == ENABLE)
    if(gb_ime_dump)
    {
        debug_msg("IME dump...\r\n");
    }
    #endif
    #if (IFE2_DEBUG_DUMP == ENABLE)
    if(gb_ife2_dump)
    {
        debug_msg("IFE2 dump...\r\n");
    }
    #endif
    
    buf_n = Cap_SliceMode_GetSliceCount()+1; //output buffer number
    buf_h = Cap_SliceMode_GetSliceHeight(); //output slice height
    SliceCtrl.slice_h = buf_h;
    debug_cap2(("CAP slice h = %d\r\n", SliceCtrl.slice_h));
    SliceCtrl.total_h = gBuf_JPG.h;
    debug_cap2(("CAP slice total h = %d\r\n", SliceCtrl.total_h));
    SliceCtrl.total_cnt = (gBuf_JPG.h+(buf_h-1))/buf_h;
    debug_cap2(("CAP slice total num = %d\r\n", SliceCtrl.total_cnt));
    SliceCtrl.total_id = buf_n;
    debug_cap2(("CAP slice buf num = %d\r\n", SliceCtrl.total_id));

    //generator slice in/out size of each engine
    _InInfo.IpeInH = gBuf_SIE.w;
    _InInfo.IpeInV = gBuf_SIE.h;
    _InInfo.ImeOutH = gBuf_JPG.w;
    _InInfo.ImeOutV = gBuf_JPG.h;
    _InInfo.ImeSliceH = buf_h;
    _InInfo.Sie_SliceH_Default = gBuf_SIE.h/3;
    _InInfo.IpeMode = IPE_AUTOSTP;
    _InInfo.ImeMode = IPE2IME_SLICE_MODE;
/*    
    debug_msg("SliceSize_Gen In:\r\n");
    debug_msg(" IpeInH=%d, V=%d\r\n", _InInfo.IpeInH, _InInfo.IpeInV);
    debug_msg(" ImeOutH=%d, V=%d\r\n", _InInfo.ImeOutH, _InInfo.ImeOutV);
    debug_msg(" ImeSliceH=%d\r\n", _InInfo.ImeSliceH);
    debug_msg(" Sie_SliceH_Default=%d\r\n", _InInfo.Sie_SliceH_Default);
    debug_msg(" IpeMode=%d\r\n", _InInfo.IpeMode);
    debug_msg(" ImeMode=%d\r\n", _InInfo.ImeMode);
*/    
    IQSim_SliceSize_Gen(&_OutInfo, &_InInfo);
/*    
    debug_msg("SliceSize_Gen Out:\r\n");
    debug_msg(" Ipe_Hstripe.n=%d, l=%d, n=%d\r\n", _OutInfo.Ipe_Hstripe.n, _OutInfo.Ipe_Hstripe.l, _OutInfo.Ipe_Hstripe.m);
    debug_msg(" Ipe_Vstripe.n=%d, l=%d, n=%d\r\n", _OutInfo.Ipe_Vstripe.n, _OutInfo.Ipe_Vstripe.l, _OutInfo.Ipe_Vstripe.m);
    debug_msg(" Ime_Hstripe.n=%d, l=%d, n=%d\r\n", _OutInfo.Ime_Hstripe.n, _OutInfo.Ime_Hstripe.l, _OutInfo.Ime_Hstripe.m);
    debug_msg(" Ime_Vstripe.n=%d, l=%d, n=%d\r\n", _OutInfo.Ime_Vstripe.n, _OutInfo.Ime_Vstripe.l, _OutInfo.Ime_Vstripe.m);
    debug_msg(" Sie_SliceH=%d\r\n", _OutInfo.Sie_SliceH);
*/    
    
    SLICE_Init(&SliceCtrl);
    #if (CAP_DEBUG_EXEC >= IME_EXEC)
    #if (CAP_DEBUG_EXEC >= IFE2_EXEC)
    #if (CAP_DEBUG_EXEC >= JPG_EXEC)
    IQSim_JPG_Open();
    #endif
    IQSim_IFE_Open();
    #endif
    IQSim_IME_Open();
    IQSim_IPE_Open();
    #endif
    debug_cap2(("CAP slice start\r\n"));
    while (!SLICE_IsEnd(&SliceCtrl))
    {
        #if ((CAP_DEBUG_FLOW == ENABLE) && (CAP_DEBUG_FLOW2 == ENABLE))
        debug_cap2(("CAP slice id=%d\r\n", SliceCtrl.curr_cnt));
        #endif
        #if ((CAP_DEBUG_FLOW == ENABLE) && (CAP_DEBUG_FLOW2 != ENABLE))
        debug_cap(("CAP do IPE/IME/IFE/JPG slice...%d\r", SliceCtrl.curr_cnt));
        #endif
        #if (CAP_DEBUG_EXEC >= IME_EXEC)
        IQSim_IME_ProcessYCC_Slice_Begin(IQSimMode, &SliceCtrl);
        IQSim_IME_ProcessYCC_Slice_End(IQSimMode, &SliceCtrl);
        #if (CAP_DEBUG_EXEC >= IFE2_EXEC)
        IQSim_IFE_ProcessYCC_Slice_Begin(IQSimMode, &SliceCtrl, times_Y, times_CC);
        IQSim_IFE_ProcessYCC_Slice_End(IQSimMode, &SliceCtrl);
        #if (CAP_DEBUG_EXEC >= JPG_EXEC)
        IQSim_JPG_ProcessYCC_Slice_Begin(IQSimMode, &SliceCtrl);
        IQSim_JPG_ProcessYCC_Slice_End(IQSimMode, &SliceCtrl);
        #endif
        #endif
        #endif
        SLICE_Next(&SliceCtrl);
    }
    #if ((CAP_DEBUG_FLOW == ENABLE) && (CAP_DEBUG_FLOW2 != ENABLE))
    debug_cap(("\r\n"));
    #endif
    debug_cap2(("CAP slice end\r\n"));
    #if (CAP_DEBUG_EXEC >= IME_EXEC)
    IQSim_IME_Stop();
    IQSim_IPE_Stop();
    #if (CAP_DEBUG_EXEC >= IFE2_EXEC)
    IQSim_IFE_Stop();
    IQSim_IFE_Close();
    #endif
    IQSim_IPE_Close();
    IQSim_IME_Close();
    #if (CAP_DEBUG_EXEC >= JPG_EXEC)
    IQSim_JPG_Stop();
    IQSim_JPG_Close();
    #endif
    #endif
    SLICE_Exit(&SliceCtrl);
    
    #if (IME_DEBUG_DUMP == ENABLE)
    if(gb_ime_dump)
    {
        debug_msg("IME dump ok: w,h=%dx%d, loff=%d, size=%d\r\n", gBuf_JPG.w, gBuf_JPG.h, gBuf_JPG.w, gBuf_JPG.w*gBuf_JPG.h);
    }
    #endif
    #if (IFE2_DEBUG_DUMP == ENABLE)
    if(gb_ife2_dump)
    {
        debug_msg("IFE2 dump ok: w,h=%dx%d, loff=%d, size=%d\r\n", gBuf_JPG.w, gBuf_JPG.h, gBuf_JPG.w, gBuf_JPG.w*gBuf_JPG.h);
    }
    #endif
    
    #if (CAP_DEBUG_EXEC >= JPG_EXEC)
    #if (JPG_DEBUG_DUMP == ENABLE)
    if(gb_jpg_dump)
    {
        debug_msg("JPG dump...\r\n");
        IQ_BUF_Dump(&gBuf_JPG, "A:\\IQSIM\\Out", 0, 0);
        debug_msg("JPG dump ok: w,h=%dx%d, size=%d\r\n", gBuf_JPG.w, gBuf_JPG.h, gBuf_JPG.size);
    }
    #endif
    #endif
}

UINT32 g_iCapCount = 0;

void IQSim_Capture(int IQSimMode)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_BASIC_IO_REG_STRUCT* pBasic = &(pBin->Basic); 
    #if (BIN_DEBUG_CONV == ENABLE)
    IQSIM_BIN2* pBin2 = (IQSIM_BIN2*)&dumpBin2;
    #endif
    int nIfeRaw = IQSim_IFE_GetTimes(IQ_CNL_RAW);
    int nIfeY = IQSim_IFE_GetTimes(IQ_CNL_Y);
    int nIfeCC = IQSim_IFE_GetTimes(IQ_CNL_Cb);
    #if (SIE_DEBUG_DUMP == ENABLE)
    if(pBin->Sie.SIE_Register_0.Bit.SAVE_SIE_OUT)
        gb_sie_dump = TRUE;
    else
        gb_sie_dump = FALSE;
    #endif
    #if (IME_DEBUG_DUMP == ENABLE)
    if(pBin->Ipe.IPE_Register_0.Bit.SAVE_IME_OUT)
        gb_ime_dump = TRUE;
    else
        gb_ime_dump = FALSE;
    #endif
    #if (IFE_DEBUG_DUMP == ENABLE)
    if(pBin->IfeRaw.IFE_Register_0.Bit.SAVE_IFE_OUT)
        gb_ife_dump = TRUE;
    else
        gb_ife_dump = FALSE;
    #endif

    #if (BIN_DEBUG_DUMP == ENABLE)
    IQSim_DumpBin("A:\\IQSIM\\INPUT.BIN", pBin);
    #endif
    #if (BIN_DEBUG_CONV == ENABLE)
    IQSim_DumpBin2("A:\\IQSIM\\INPUT2.BIN", pBin, pBin2);
    #endif
    #if (CAP_DEBUG_DUMP == ENABLE)
    memset(&dumpBin, 0, sizeof(IQSIM_BIN));
    memcpy(&dumpBin.Basic, pBasic, sizeof(NT96220_BASIC_IO_REG_STRUCT));
    #endif
    
    debug_cap(("CAP start\r\n"));
    #if (CAP_DEBUG_EXEC >= SIE_EXEC)
    debug_cap(("CAP do SIE frame...\r\n"));
    IQSim_SIE_ProcessRAW_Frame(0, nIfeRaw);
    #if (CAP_DEBUG_EXEC >= IFE_EXEC)
    debug_cap(("CAP do IFE frame...\r\n"));
    IQSim_IFE_ProcessRAW_Frame(0, nIfeRaw);
    #endif
    #if ((CAP_DEBUG_FLOW == ENABLE) && (CAP_DEBUG_FLOW2 == ENABLE))
    debug_cap(("CAP do IPE/IME/IFE/JPG slice...\r\n"));
    #endif
    IQSim_IPEIMEIFEJPG_ProcessYCC_Slice(0, nIfeY, nIfeCC);
    
    #if (CAP_DEBUG_EXEC >= JPG_EXEC)
    #if (CAP_DEBUG_DUMP == ENABLE)
    IQSim_DumpBin("A:\\IQSIM\\OUTPUT.BIN", &dumpBin);
    #endif
    #endif
    
    #endif
    g_iCapCount++;
    debug_cap(("CAP finish\r\n\r\n"));
}

void IQSim_GenSourceCode(void)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    if(g_iCapCount == 0)
    {
        debug_err(("GEN code fail, please do CAP first!\r\n"));
        return;
    }
    
    debug_cap(("GEN code start\r\n"));
    debug_cap(("file: %s\r\n", "A:\\IQSIM\\NT96220.c"));
    IQSim_DumpBin("A:\\IQSIM\\INPUT.BIN", pBin);
    slog_open("A:\\IQSIM\\NT96220.c");
    debug_cap(("-sie\r\n"));
    IQSim_SIE_DumpIQ();
    debug_cap(("-ife raw\r\n"));
    IQSim_IFE_DumpIQ(IQ_CNL_RAW);
    debug_cap(("-ipe\r\n"));
    IQSim_IPE_DumpIQ();
    debug_cap(("-ife y\r\n"));
    IQSim_IFE_DumpIQ(IQ_CNL_Y);
    debug_cap(("-ife cx\r\n"));
    IQSim_IFE_DumpIQ(IQ_CNL_Cb);
    debug_cap(("-ime\r\n"));
    IQSim_IME_DumpIQ();
    slog_close();
    debug_cap(("GEN code finish\r\n\r\n"));
}

