#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "debug.h"
#include "interrupt.h"
#include "ImageAlgInfor.h"
#include "sra_lib.h"
#include "IPPManager.h"
#include "sen_common.h"

/*******************************************
Note :
    a.  HD_Division is not equal to job_req->hd_DivisionNum.Because Hardware Register settings is HD_Division-1
    HD_Division is used for calculation, but job_req->hd_DivisionNum is used for register settings.

    b.  SIE  Act_width & Act_height description and PRE crop parameter descriptions are below drawings.

    Horizontal (HD sognal) :
             __________________________________________   ____________________________________________
           _/                                          \_/                                            \

                ^              ^
                |              |
        Act_startX          CenterX

                |<--------------------------->|  (Act_width = HD_Total_Prv - ACT_STRX_Prv

                ^
                |__
                    Pre cropping start pos( Pre_hcrop_ini = 0 )

                  |<--------------->| ( Pre_hcropping_size = RawHsize)

                |<--->| ( Pre_hcrop_ini = (CenterX - Act_startX)-Pre_hcropping_size/2

----------------------------------------------------------------------------------------------------------

        Vertical (HD sognal) :  SensorCapTotalFld = N , RawSize = RawHsize*RawVsize
             __________   __________   __________                         __________   __________   __________
           _/          \_/          \_/          \        ~~~~~         _/          \_/          \_/


                          ^                                    ^
                          |                                    |
                        Act_startY                           CenterY                (Act_height = VD_Total_Prv - ACT_STRY_Prv - LAST_ONE_LINE_OMITT

                              |<------------------------------------------------------------------>|

                          ^
                          |__
                              Pre cropping start( Pre_vcrop_ini = 0 )

                           |<--------------------------------------------->| (Pre_vcropping_size = RawVsize/N )


                            |<---------->| ( Pre_vcrop_ini = (CenterY - Act_startY)- Pre_vcropping_size/2


*********************************************/
//IPE_PARAM IPE_Para;
//#NT#2010/12/22#ethanlau -begin
//#NT#double layer(one for application, one for driver
//D2 for bitstream under vid,for thumbnail/screen under cap
//D1 for panel under prv/vid, for bitstream/FQV under cap
IME_FBADDR_PRAM IME_D1_Para,IME_D2_Para;
//#NT#2010/12/24#ethanlau -begin
//#NT#ime extension struct, cause old ime struct design do not support ime driver to extend ime func.
IME_EXTEN IME_D1_Para_ext,IME_D2_Para_ext;
//#NT#2010/12/24#ethanlau -end
SIE_PRE_PARAM SIE_PRE_Para_prv;
SIE_PRE_PARAM SIE_PRE_Para_cap;
SIE_PRE_PARAM SIE_PRE_Para_d2d;
IPE_PARAM IPE_Para;
IFE_ENG_SETTING Ife_Param;
IPP_PARAM Ipp_Param;

//#NT#this layer should only appear in ippmanger, but inorder to be in consistence with 4xx chip code base,
//still maintain orignal design.(relative prob.: ipp control api(xxx_chgparam()) have separate within sensor lib.)
STR_SIE_REG SIE_Param = {0};
IPE_MODEINFO IPEModeInfo = {0};
IME_MODE_PRAM IMEParam= {0};
static IFE_PARAM g_IFEParam = {0};
static IFE_FILT_PARA g_IfeFilter = {0};
//#NT#2010/12/22#ethanlau -end
#define LAST_ONE_LINE_OMITT 1

struct PUBLIC_STR_JOBREQS  IPPMgrReq;

int Sensor_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    SIE_PRE_PARAM *pSieSetting;
    if( (JobType == PREVIEW_JOB)||(JobType == VIDEO_JOB) )
    {
        pSieSetting = job_req->SIE_PRE_PRV;
    }
    else if (JobType == CAPTURE_JOB)
    {
        pSieSetting = job_req->SIE_PRE_CAP;
    }
    else
        return IPP_FAIL;

    SIE_Param.SieHdVdSync.HD_sync = pSieSetting->HDSync;
    SIE_Param.SieHdVdSync.HD_total = pSieSetting->HDTotal;
    SIE_Param.SieHdVdSync.VD_sync = pSieSetting->VDSync;
    SIE_Param.SieHdVdSync.VD_total = pSieSetting->VDTotal;
    SIE_Param.SieHdVdSync.HD_inv = pSieSetting->SieHDInv;
    SIE_Param.SieHdVdSync.VD_inv = pSieSetting->SieVDInv;
    SIE_Param.SieActParam.Hstart = pSieSetting->HDActX;
    SIE_Param.SieActParam.Vstart = pSieSetting->HDActY;
    SIE_Param.SieActParam.Hsize = pSieSetting->HDActSizeX;
    SIE_Param.SieActParam.Vsize = pSieSetting->HDActSizeY;
    return IPP_OK;

}

int Sensor_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int Sensor_ExecIPP(struct  PUBLIC_STR_JOBREQS * job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int Sensor_EndIPP(struct  PUBLIC_STR_JOBREQS * job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int SIE_BeginIPP(struct  PUBLIC_STR_JOBREQS * job_req, JobTypeEnum JobType)
{
    SIE_PRE_PARAM* pSieSetting;

    if( (JobType == PREVIEW_JOB)||(JobType == VIDEO_JOB) )
    {
        pSieSetting = job_req->SIE_PRE_PRV;
        //in/out mode sel
        SIE_Param.SieDRAMInEn = FALSE;
        SIE_Param.SieOutMode = job_req->SIE_PRE_PRV->SieOutModSel;

        if(job_req->SIE_PRE_PRV->SieOutModSel == SIE_SLICE_MODE)
            SIE_Param.SieSliceHeight = job_req->SIE_PRE_PRV->SieSliceHight;
        else// SIE_FRAME_MODE
            SIE_Param.SieSliceHeight = 0;
        SIE_Param.SieOutAddr.OutBufNum = job_req->SIE_PRE_PRV->SieOutBufNum;

        //SIE_INTE_DRAMO_END for digital zoom
        //BP1 for phototsk ae/ca event
        SIE_Param.SieIntEn = SIE_INTE_DRAMO_END|SIE_INTE_FLDEND|SIE_INTE_BP1|job_req->SIE_PRE_PRV->SieIntEn;
    }
    else if (JobType == CAPTURE_JOB)
    {
        pSieSetting = job_req->SIE_PRE_CAP;
        //in/out mode sel
        SIE_Param.SieDRAMInEn = job_req->SIE_PRE_CAP->SieDRAMInEn;
        SIE_Param.SieOutMode = job_req->SIE_PRE_CAP->SieOutModSel;

        if(job_req->SIE_PRE_CAP->SieOutModSel == SIE_SLICE_MODE)
        {
            SIE_Param.SieSliceHeight = job_req->SIE_PRE_CAP->SieSliceHight;
            SIE_Param.SieOutAddr.OutBufNum = THREE_OUTPUT_BUF;
        }
        else// SIE_FRAME_MODE
        {
            SIE_Param.SieSliceHeight = 0;
            SIE_Param.SieOutAddr.OutBufNum = ONE_OUTPUT_BUF;
        }

        SIE_Param.SieIntEn = job_req->SIE_PRE_CAP->SieIntEn;
    }
    else if(JobType == SIE_D2D)
    {
        pSieSetting = job_req->SIE_PRE_D2D;
        SIE_Param.SieDRAMInEn = ENABLE;
        SIE_Param.SieOutMode = SIE_FRAME_MODE;
        SIE_Param.SieSliceHeight = 0;
        SIE_Param.SieOutAddr.OutBufNum = ONE_OUTPUT_BUF;
        SIE_Param.SieIntEn = job_req->SIE_PRE_CAP->SieIntEn;
        if( (pSieSetting->SieFuncEn & SIE_DF_SUB_EN)||(pSieSetting->SieFuncEn & SIE_BLM_EN))
        {
            debug_err(("^R D2D use dram input chanel 0"));
            pSieSetting->SieFuncEn &= ~(SIE_DF_SUB_EN|SIE_BLM_EN);
        }
    }
    else
        return IPP_FAIL;

    //func_en
    SIE_Param.SieFuncEn = pSieSetting->SieFuncEn;
    if(SIE_Param.SieFuncEn & SIE_CHOUT_EN)
    {
        SIE_Param.SieFuncEn &= (~SIE_CHOUT_EN);
        debug_err(("CMOS don't need SIE_CHOUT_EN so far\r\n"));
    }
    //bit sel
    SIE_Param.SieInPack = pSieSetting->SieInBitDepth;
    //only df support 10bit out
    if(pSieSetting->SieFuncEn & SIE_DF_SUB_EN)
        SIE_Param.SieOutPack = pSieSetting->SieBitDepth;
    else
    {
        //if(pSieSetting->SieBitDepth != SIE_OUT_PACK_8BIT)
        //    debug_err(("sie out only support 8 bit when SIE_DF_SUB_EN OFF,force ouput bit chg to 8 bit"));
        SIE_Param.SieOutPack = pSieSetting->SieBitDepth;//SIE_OUT_PACK_8BIT;
    }
    //act size
    //Sie_PreHsize = SIE_Param.SieHdVdSync.HD_total - SIE_Param.SieActParam.Hstart;
    //Sie_PreHsize = (Sie_PreHsize / 4) * 4;
    //Sie_PreVsize = SIE_Param.SieHdVdSync.VD_total - SIE_Param.SieActParam.Vstart - LAST_ONE_LINE_OMITT;
    //SIE_Param.SieActParam.Hsize = Sie_PreHsize;
    //SIE_Param.SieActParam.Vsize = Sie_PreVsize;
    //crop
    SIE_Param.SieCropParam.Hstart = pSieSetting->RawCenterX - pSieSetting->SieCropSizeH/2 - SIE_Param.SieActParam.Hstart;
    SIE_Param.SieCropParam.Vstart = pSieSetting->RawCenterY - pSieSetting->SieCropSizeV/2 - SIE_Param.SieActParam.Vstart;
    SIE_Param.SieCropParam.Hsize = pSieSetting->SieCropSizeH;
    SIE_Param.SieCropParam.Vsize = pSieSetting->SieCropSizeV;
    //inputsize,only worked under dram in
    SIE_Param.SieInAddr0 = 0;
    SIE_Param.SieInAddr1 = 0;
    SIE_Param.SieInLofs = 0;
    if(JobType == SIE_D2D)
    {
        //input addr
        SIE_Param.SieInAddr0 = pSieSetting->SiePreAddr0;
        //inputsize
        SIE_Param.SieInLofs = pSieSetting->SieDramInLineOfs;
        SIE_Param.SieCropParam.Hstart = 0;
        SIE_Param.SieCropParam.Vstart = 0;
        SIE_Param.SieCropParam.Hsize = pSieSetting->SieDramInSizeH;
        SIE_Param.SieCropParam.Vsize = pSieSetting->SieDramInSizeV;
    }
    //output addr
    SIE_Param.SieOutAddr.OutAddr0 = pSieSetting->SieOutAdd0;
    SIE_Param.SieOutAddr.OutAddr1 = pSieSetting->SieOutAdd1;
    SIE_Param.SieOutAddr.OutAddr2 = pSieSetting->SieOutAdd2;
    //outputsize
    SIE_Param.SieOutSize.Hsize = pSieSetting->SieOutSizeH;
    SIE_Param.SieOutSize.lineofs = pSieSetting->SieOutLineOfs;

    SIE_Param.SieAutoOBEn = pSieSetting->AutoObEn;
    if(SIE_Param.SieAutoOBEn == TRUE)
    {
        SIE_Param.SieOBParam = pSieSetting->SieObParam;
    }
    SIE_Param.SieOBOfs = pSieSetting->SieObOfs;

    SIE_Param.SieOutSrc = pSieSetting->SiePathSel;

    SIE_Param.SieIntBP1 = pSieSetting->SieBp1;
    SIE_Param.SieIntBP2 = pSieSetting->SieBp2;
    SIE_Param.SieSyncPhase.Master= pSieSetting->SieMaster;
    SIE_Param.SieSyncPhase.Data_Phase = pSieSetting->SieDataPhase;
    SIE_Param.SieSyncPhase.HD_Phase = pSieSetting->SieHDPhase;
    SIE_Param.SieSyncPhase.VD_Phase = pSieSetting->SieVDPhase;

    return IPP_OK;
}


int SIE_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    UINT32 offset;
    SIE_PRE_PARAM *pSieSetting;
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    if ((JobType == PREVIEW_JOB) || (JobType == VIDEO_JOB ))
    {
        pSieSetting = job_req->SIE_PRE_PRV;

    }
    else if (JobType == CAPTURE_JOB )
    {
        pSieSetting = job_req->SIE_PRE_CAP;
    }
    else if(JobType == SIE_D2D )
    {
        pSieSetting = job_req->SIE_PRE_D2D;
    }
    else
        return IPP_FAIL;

    SIE_Param.SieFuncEn = pSieSetting->SieFuncEn;
    SIE_Param.SieFuncEn |= Sen_GetSIEFlipParam(pimginfo->ImageFlip);

    //conflict check
    if((SIE_Param.SieFuncEn & SIE_DF_SUB_EN) && (SIE_Param.SieFuncEn & SIE_DF_EMBED_EN))
    {
      SIE_Param.SieFuncEn &= (~SIE_LINE_BLM_EN);
      SIE_Param.SieFuncEn &= (~SIE_BLM_EN);
      debug_err(("sie func conflict - case 1\r\n"));
    }
    if((SIE_Param.SieFuncEn & SIE_BLM_EN)&&(SIE_Param.SieFuncEn & SIE_DF_SUB_EN))
    {
        debug_err(("sie func conflict - case 2\r\n"));
        SIE_Param.SieFuncEn &=  (~SIE_BLM_EN);
    }

    //sie func setting
    if (SIE_Param.SieFuncEn & SIE_ECS_EN)
    {
        if((SIE_Param.SieActParam.Hsize) < (65*2 + 8))//provide by sie driver
        {
            debug_err(("Active H size too small for ECS ..."));
            SIE_Param.SieFuncEn &= (~SIE_ECS_EN);
        }
        else
        {
            SIE_Param.SieECSParam= pSieSetting->Sie_ECS;
            if(SIE_Param.SieECSParam.MapSizeSel != ECSMAP65x65)
                debug_err(("^R ECS table size only support 65x65\r\n"));
        }
        SIE_Param.SieInAddr1 = pSieSetting->SiePreAddr1;
    }
    if (SIE_Param.SieFuncEn & SIE_CG_EN)
    {
        //#NT#2011/11/30#Ethan Lau -begin
        //#NT#patch gain_sel
        if( (job_req->sie_cg_info.Gain0<1024)&&(job_req->sie_cg_info.Gain1<1024)&&(job_req->sie_cg_info.Gain2<1024)&&(job_req->sie_cg_info.Gain3<1024) )
        {
            job_req->sie_cg_info.GainSel = SIECG_2_8;
        }
        else if( (job_req->sie_cg_info.Gain0<2048)&&(job_req->sie_cg_info.Gain1<2048)&&(job_req->sie_cg_info.Gain2<2048)&&(job_req->sie_cg_info.Gain3<2048) )
        {
            job_req->sie_cg_info.Gain0 /=2;
            job_req->sie_cg_info.Gain1 /=2;
            job_req->sie_cg_info.Gain2 /=2;
            job_req->sie_cg_info.Gain3 /=2;
            job_req->sie_cg_info.GainSel = SIECG_3_7;
        }
        else
        {
            //#NT#2011/12/20#Ethan Lau -begin
            job_req->sie_cg_info.Gain0 = (job_req->sie_cg_info.Gain0>2047 ? 2047:job_req->sie_cg_info.Gain0);
            job_req->sie_cg_info.Gain0 /=2;
            job_req->sie_cg_info.Gain1 = (job_req->sie_cg_info.Gain1>2047 ? 2047:job_req->sie_cg_info.Gain1);
            job_req->sie_cg_info.Gain1 /=2;
            job_req->sie_cg_info.Gain2 = (job_req->sie_cg_info.Gain2>2047 ? 2047:job_req->sie_cg_info.Gain2);
            job_req->sie_cg_info.Gain2 /=2;
            job_req->sie_cg_info.Gain3 = (job_req->sie_cg_info.Gain3>2047 ? 2047:job_req->sie_cg_info.Gain3);
            job_req->sie_cg_info.Gain3 /=2;
            job_req->sie_cg_info.GainSel = SIECG_3_7;
            //#NT#2011/12/20#Ethan Lau -end
            debug_err(("^R %s: sie color gain over limit\r\n",__func__));
        }
        SIE_Param.SieCGParam = job_req->sie_cg_info;
        //#NT#2011/11/30#Ethan Lau -end
        SIE_Param.SieCofsParam = job_req->sie_cofs_info;
    }
    if (SIE_Param.SieFuncEn & SIE_GAMMA_EN)
    {
        //#NT#2011/05/20#Connie Yu -begin
        //#NT#update SRA API
        //sra_setReadSel(SRA_READBY_SIE);
        //sra_setGamma(job_req->sra_gammaLUT);
        sra_setGammaTable(SRA_READBY_SIE, job_req->sra_gammaLUT);
        //#NT#2011/05/20#Connie Yu -end
    }
    if (SIE_Param.SieFuncEn & SIE_LINE_BLM_EN)
    {
        SIE_Param.SieDefNfact = pSieSetting->SieDefNfact;
        SIE_Param.SieDefColumn = pSieSetting->SieDefColumn;
    }
    if(SIE_Param.SieFuncEn & SIE_DF_SUB_EN)
    {
        SIE_Param.SieDFSParam = job_req->sie_dfs_setting;

        SIE_Param.SieInAddr0 = pSieSetting->SiePreAddr2;
        if(SIE_Param.SieInPack == SIE_IN_PACK_8BIT)
        {
            SIE_Param.SieInLofs = SIE_Param.SieOutSize.Hsize;
        }
        else
        {
            SIE_Param.SieInLofs = ((SIE_Param.SieOutSize.Hsize*10) + (8-1))/8;
            SIE_Param.SieInLofs = ((SIE_Param.SieInLofs + (4-1)) >> 2) << 2;
        }
    }
    else if(SIE_Param.SieFuncEn & SIE_BLM_EN)
    {
        SIE_Param.SieInAddr0 = pSieSetting->SiePreAddr2;
    }

    if(SIE_Param.SieFuncEn & SIE_SUBOUT_EN)
    {
        SIE_Param.SieSubParam = pSieSetting->SieSubParam;

        if((SIE_Param.SieSubParam.Hsub > 5)||(SIE_Param.SieSubParam.Vsub > 5))
        {
            debug_err(("Sie sub out only support 1~5"));
        }

        if((SIE_Param.SieSubParam.Hcnt == 0) || (SIE_Param.SieSubParam.Vcnt == 0))
        {
            debug_err(("Subsample output out of range ...")) ;
        }

        SIE_Param.SieSubAddr = pSieSetting->SieSubAddr;
        SIE_Param.SieSubOutLofs = SIE_Param.SieSubParam.Hcnt;

        //enable subout interrupt
        SIE_Param.SieIntEn |= SIE_INTE_SUB;
    }
    //#NT#2011/10/27#Ethan Lau -begin
    //#NT#fix flip under sie slice mode
    if((SIE_Param.SieFuncEn & SIE_HFLIP_EN)||(SIE_Param.SieFuncEn & SIE_VFLIP_EN))
    {
        if((SIE_Param.SieFuncEn & SIE_VFLIP_EN) && (SIE_Param.SieOutMode == SIE_SLICE_MODE))
        {
            SIE_Param.SieFuncEn &= (~SIE_VFLIP_EN);
            //re-write image-flip value
            if(pimginfo->ImageFlip == _DSP_Flip_V)
                pimginfo->ImageFlip = _Flip_Off;
            if(pimginfo->ImageFlip == _DSP_Flip_H_V)
                pimginfo->ImageFlip = _DSP_Flip_H;

            debug_err(("^R SIE slice mode do not support Vertical FLIP,force to disable\r\n"));
        }
        offset = Sen_GetSIEAddrOfs(pimginfo->ImageFlip,
                                    SIE_Param.SieCropParam.Vsize,
                                    SIE_Param.SieOutSize.lineofs);
    }
    else
        offset = 0;
    //#NT#2011/10/27#Ethan Lau -end

    SIE_Param.SieOutAddr.OutAddr0 += offset;
    SIE_Param.SieOutAddr.OutAddr1 += offset;
    SIE_Param.SieOutAddr.OutAddr2 += offset;

    if(SIE_Param.SieFuncEn & SIE_DF_EMBED_EN)
    {
        SIE_Param.SieOutSize.Hsize = ((SIE_Param.SieCropParam.Hsize) >> 2) << 2;
    }
    SIE_Param.SieHBinParam = pSieSetting->SieHBinParam;
    if(SIE_Param.SieHBinParam.PixAdd != 0)
    {
        if(SIE_Param.SieCropParam.Hsize <= SIE_Param.SieOutSize.Hsize)
        {
            debug_err(("sie HSCL_EN is disable, force Bining func turning to off\r\n"));
            SIE_Param.SieHBinParam.PixAdd = 0;
        }
    }
    return IPP_OK;
}

int SIE_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if( (JobType == PREVIEW_JOB) || (JobType == VIDEO_JOB) )
    {
        SIE_Param.SieOperation = SIE_ENGINE_SET2RUN;
        sie_setMode(&SIE_Param);
    }
    else if (JobType == CAPTURE_JOB)
    {
        SIE_Param.SieOperation = SIE_ENGINE_SETMODE;
        sie_setMode(&SIE_Param);
    }
    else if (JobType == SIE_D2D)
    {
        SIE_Param.SieOperation = SIE_ENGINE_SETMODE;
        //sie_setMode(&SIE_Param);
        sie_Dram2Dram(&SIE_Param,SIE_WAIT);
    }
    else
    {
        return IPP_FAIL;
    }

    return IPP_OK;
}

int SIE_EndIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return TRUE;
}

//=============================================================================================================
int IPE_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{

    if((JobType == PREVIEW_JOB)||(JobType == VIDEO_JOB)||(JobType == CAPTURE_JOB)||(JobType == IPE_IME_FRAME_1PATH_JOB))
    {
        IPEModeInfo.IntEn = job_req->IpeParam->IntEn;
        IPEModeInfo.SIZEINFO.StripeMode = job_req->IpeParam->StripeMode;
        IPEModeInfo.SIZEINFO.HSTRIPE.n = job_req->IpeParam->HStripe.stripe.n;
        IPEModeInfo.SIZEINFO.HSTRIPE.l = job_req->IpeParam->HStripe.stripe.l;
        IPEModeInfo.SIZEINFO.HSTRIPE.m = job_req->IpeParam->HStripe.stripe.m;
        IPEModeInfo.SIZEINFO.HSTRIPE.olap = job_req->IpeParam->HStripe.olap;

        IPEModeInfo.SIZEINFO.VSTRIPE.n = job_req->IpeParam->VStripe.stripe.n;
        IPEModeInfo.SIZEINFO.VSTRIPE.l = job_req->IpeParam->VStripe.stripe.l;
        IPEModeInfo.SIZEINFO.VSTRIPE.m = job_req->IpeParam->VStripe.stripe.m;
        IPEModeInfo.SIZEINFO.VSTRIPE.olap = job_req->IpeParam->VStripe.olap;


        IPEModeInfo.SIZEINFO.CfaPat = job_req->IpeParam->CfaPat;

        IPEModeInfo.SIZEINFO.INSIZE.hsize = job_req->IpeParam->InSizeH;
        IPEModeInfo.SIZEINFO.INSIZE.vsize = job_req->IpeParam->InSizeV;
        IPEModeInfo.SIZEINFO.LofsIn = job_req->IpeParam->InLineOfs;
        IPEModeInfo.ININFO.Adr0 = job_req->IpeParam->Add0;
        if((JobType == PREVIEW_JOB)||(JobType == VIDEO_JOB))
        {
            IPEModeInfo.ININFO.Adr1 = job_req->IpeParam->Add1;
            IPEModeInfo.ININFO.Adr2 = job_req->IpeParam->Add2;
            IPEModeInfo.ININFO.PPBEn = ENABLE;
        }
        else
            IPEModeInfo.ININFO.PPBEn = DISABLE;
        IPEModeInfo.ININFO.PackBit = job_req->IpeParam->BitDepth;
        IPEModeInfo.ININFO.RGBInEn = DISABLE;
        IPEModeInfo.ININFO.PPBSel = DISABLE;
        /*
        if(IPEModeInfo.ININFO.PPBSel!= ENABLE)
            IPEModeInfo.ININFO.PPBIni = job_req->IpeParam.ININFO.PPBIni;
        */

        if(IPEModeInfo.SIZEINFO.StripeMode == IPE_AUTOSLICE)
            ipe_setSliceH(job_req->IpeParam->IpeSliceH);
        else
            ipe_setSliceH(0);
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}


int IPE_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int IPE_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if( (JobType == PREVIEW_JOB)||(JobType == VIDEO_JOB )||(JobType == CAPTURE_JOB )||(JobType == IPE_IME_FRAME_1PATH_JOB ) )
    {
        if(job_req->IpeParam->IFEembed_en)
        {
            IPEModeInfo.IQINFO.IFEembed_en = ENABLE;
            IPEModeInfo.IQINFO.ParamUpdateSel |= IPE_SET_IFE;
            //debug_msg("IPL will Start under IFE embedded in IPE(jobtype:%d)\r\n",JobType);
        }
        else
        {
            IPEModeInfo.IQINFO.IFEembed_en = DISABLE;
            IPEModeInfo.IQINFO.ParamUpdateSel |= IPE_SET_IFE;
            //debug_msg("IPL will Start under DISABLE IFE embedded in IPE(jobtype:%d)\r\n",JobType);
        }
        ipe_setMode(&IPEModeInfo);
        if((JobType == PREVIEW_JOB) || (JobType == VIDEO_JOB ))
            ipe_start();
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int IPE_EndIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return IPP_OK;
}

int IME_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    IME_HV_STRIPE_INFO _ImeStripeInfo;
    if( (JobType == PREVIEW_JOB)||(JobType == IPE_IME_FRAME_1PATH_JOB) )
    {
        IMEParam.op_mode = IPE2IME_FRAME_MODE;
        IMEParam.in_src = IMEALG_IPE2IME;
        IMEParam.bil = IMEALG_BILINEAR;
        IMEParam.path_num = IMEALG_1PATH;
        IMEParam.interrupt_en = IME_INTE_FRM;
/*
        if(IMEParam.interrupt_en & IME_INTE_LINE_BP1)
            IMEParam.breakPoint1 = job_req->ImeParam1.pIme_Ext->Bp1;
        if(IMEParam.interrupt_en & IME_INTE_LINE_BP2)
            IMEParam.breakPoint2 = job_req->ImeParam1.pIme_Ext->Bp2;
*/
        IMEParam.in_size.h_size = job_req->ImeParam1.pIme_fbaddr->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1.pIme_fbaddr->in_v;
        IMEParam.line_ofs.in_lineofs.LineOffsetY = job_req->ImeParam1.pIme_fbaddr->in_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCb = job_req->ImeParam1.pIme_fbaddr->in_Cb_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCr = job_req->ImeParam1.pIme_fbaddr->in_Cr_lineoffset;

        IMEParam.format.in_format = IME_INPUT_YCC_444;//job_req->ImeParam1.pIme_fbaddr->in_format;
        IMEParam.format.out1_format = job_req->ImeParam1.pIme_fbaddr->out_format;
        if(job_req->ImeParam1.pIme_fbaddr->out_format==IME_OUTPUT_BAYER)
            IMEParam.p1_out_cfa = job_req->ImeParam1.pIme_Ext->p1_out_cfa;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1.pIme_fbaddr->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1.pIme_fbaddr->out_v;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetY = job_req->ImeParam1.pIme_fbaddr->out_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCb = job_req->ImeParam1.pIme_fbaddr->out_Cb_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCr = job_req->ImeParam1.pIme_fbaddr->out_Cr_lineoffset;


        IMEParam.p1_out_addr.oPPBNums = job_req->ImeParam1.pIme_Ext->Ime_BuffNum;
        switch(IMEParam.p1_out_addr.oPPBNums)
        {
            case IME_OUTPUT_PPB_5:
                IMEParam.p1_out_addr.oDMA4.y_addr = job_req->ImeParam1.pIme_fbaddr->y4_addr;
                IMEParam.p1_out_addr.oDMA4.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb4_addr;
                IMEParam.p1_out_addr.oDMA4.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr4_addr;
            case IME_OUTPUT_PPB_4:
                IMEParam.p1_out_addr.oDMA3.y_addr = job_req->ImeParam1.pIme_fbaddr->y3_addr;
                IMEParam.p1_out_addr.oDMA3.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb3_addr;
                IMEParam.p1_out_addr.oDMA3.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr3_addr;
            case IME_OUTPUT_PPB_3:
                IMEParam.p1_out_addr.oDMA2.y_addr = job_req->ImeParam1.pIme_fbaddr->y2_addr;
                IMEParam.p1_out_addr.oDMA2.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb2_addr;
                IMEParam.p1_out_addr.oDMA2.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr2_addr;
            case IME_OUTPUT_PPB_2:
                IMEParam.p1_out_addr.oDMA1.y_addr = job_req->ImeParam1.pIme_fbaddr->y1_addr;
                IMEParam.p1_out_addr.oDMA1.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb1_addr;
                IMEParam.p1_out_addr.oDMA1.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr1_addr;
            case IME_OUTPUT_PPB_1:
            default:
                IMEParam.p1_out_addr.oDMA0.y_addr = job_req->ImeParam1.pIme_fbaddr->y0_addr;
                IMEParam.p1_out_addr.oDMA0.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb0_addr;
                IMEParam.p1_out_addr.oDMA0.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr0_addr;
                break;
        }
        IMEParam.p1_out_scale_filter = job_req->ImeParam1.pIme_Ext->p1_out_scale_filter;
        IMEParam.p1_out_scale_filter.sfCoefCalMode = IME_SCALE_FILTER_COEF_AUTO_MODE;

        if(job_req->ImeParam1.pIme_Ext->ImeStripe.Ime_StripeMode == IME_STRIPE_USER_MODE)
        {
            _ImeStripeInfo.uHN = job_req->ImeParam1.pIme_Ext->ImeStripe.Hstripe.n;
            _ImeStripeInfo.uHL = job_req->ImeParam1.pIme_Ext->ImeStripe.Hstripe.l;
            _ImeStripeInfo.uHM = job_req->ImeParam1.pIme_Ext->ImeStripe.Hstripe.m;
            //IME v stripe
            _ImeStripeInfo.uVN = job_req->ImeParam1.pIme_Ext->ImeStripe.Vstripe.n;
            _ImeStripeInfo.uVL = job_req->ImeParam1.pIme_Ext->ImeStripe.Vstripe.l;
            _ImeStripeInfo.uVM = job_req->ImeParam1.pIme_Ext->ImeStripe.Vstripe.m;
            //IME stripe mode
            _ImeStripeInfo.StripeCalMode = IME_STRIPE_USER_MODE;
        }
        else
            _ImeStripeInfo.StripeCalMode = IME_STRIPE_AUTO_MODE;
        ime_setUserStripeInfo(&_ImeStripeInfo);
    }
    else if (JobType == VIDEO_JOB)
    {
        IMEParam.op_mode = job_req->ImeParam2.pIme_Ext->op_mode;
        IMEParam.in_src = IMEALG_IPE2IME;
        IMEParam.bil = IMEALG_BILINEAR;
        IMEParam.path_num = IMEALG_1PATH;
        IMEParam.interrupt_en = job_req->ImeParam2.pIme_Ext->ImeInten;

        if(IMEParam.interrupt_en & IME_INTE_LINE_BP1)
            IMEParam.breakPoint1 = job_req->ImeParam2.pIme_Ext->Bp1;
        if(IMEParam.interrupt_en & IME_INTE_LINE_BP2)
            IMEParam.breakPoint2 = job_req->ImeParam2.pIme_Ext->Bp2;

        IMEParam.in_size.h_size = job_req->ImeParam2.pIme_fbaddr->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam2.pIme_fbaddr->in_v;
        //if(IMEParam.op_mode == IPE2IME_SLICE_MODE)
        //    IMEParam.in_size.v_size -= 8;//cause ipe out vertical will be minus 8;
        IMEParam.line_ofs.in_lineofs.LineOffsetY = job_req->ImeParam2.pIme_fbaddr->in_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCb = job_req->ImeParam2.pIme_fbaddr->in_Cb_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCr = job_req->ImeParam2.pIme_fbaddr->in_Cr_lineoffset;

        IMEParam.format.in_format = job_req->ImeParam2.pIme_fbaddr->in_format;
        IMEParam.format.out1_format = job_req->ImeParam2.pIme_fbaddr->out_format;
        if(job_req->ImeParam2.pIme_fbaddr->out_format==IME_OUTPUT_BAYER)
            IMEParam.p1_out_cfa = job_req->ImeParam2.pIme_Ext->p1_out_cfa;
        IMEParam.p1_out_size.h_size = job_req->ImeParam2.pIme_fbaddr->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam2.pIme_fbaddr->out_v;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetY = job_req->ImeParam2.pIme_fbaddr->out_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCb = job_req->ImeParam2.pIme_fbaddr->out_Cb_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCr = job_req->ImeParam2.pIme_fbaddr->out_Cr_lineoffset;


        IMEParam.p1_out_addr.oPPBNums = job_req->ImeParam2.pIme_Ext->Ime_BuffNum;
        IMEParam.p1_out_addr.oDMA0.y_addr = job_req->ImeParam2.pIme_fbaddr->y0_addr;
        IMEParam.p1_out_addr.oDMA0.cb_addr = job_req->ImeParam2.pIme_fbaddr->cb0_addr;
        IMEParam.p1_out_addr.oDMA0.cr_addr = job_req->ImeParam2.pIme_fbaddr->cr0_addr;
        IMEParam.p1_out_addr.oDMA1.y_addr = job_req->ImeParam2.pIme_fbaddr->y1_addr;
        IMEParam.p1_out_addr.oDMA1.cb_addr = job_req->ImeParam2.pIme_fbaddr->cb1_addr;
        IMEParam.p1_out_addr.oDMA1.cr_addr = job_req->ImeParam2.pIme_fbaddr->cr1_addr;
        IMEParam.p1_out_addr.oDMA2.y_addr = job_req->ImeParam2.pIme_fbaddr->y2_addr;
        IMEParam.p1_out_addr.oDMA2.cb_addr = job_req->ImeParam2.pIme_fbaddr->cb2_addr;
        IMEParam.p1_out_addr.oDMA2.cr_addr = job_req->ImeParam2.pIme_fbaddr->cr2_addr;
        switch(IMEParam.p1_out_addr.oPPBNums)
        {
            case IME_OUTPUT_PPB_5:
                IMEParam.p1_out_addr.oDMA4.y_addr = job_req->ImeParam2.pIme_fbaddr->y4_addr;
                IMEParam.p1_out_addr.oDMA4.cb_addr = job_req->ImeParam2.pIme_fbaddr->cb4_addr;
                IMEParam.p1_out_addr.oDMA4.cr_addr = job_req->ImeParam2.pIme_fbaddr->cr4_addr;
            case IME_OUTPUT_PPB_4:
            default:
                IMEParam.p1_out_addr.oDMA3.y_addr = job_req->ImeParam2.pIme_fbaddr->y3_addr;
                IMEParam.p1_out_addr.oDMA3.cb_addr = job_req->ImeParam2.pIme_fbaddr->cb3_addr;
                IMEParam.p1_out_addr.oDMA3.cr_addr = job_req->ImeParam2.pIme_fbaddr->cr3_addr;
                break;
        }
        if(job_req->ImeParam2.pIme_Ext->op_mode == IPE2IME_SLICE_MODE)
        {
            _IME_SLICE_HEIGHT_SEL slice_H = 0;
            if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 32)
                slice_H = SLICE_32_LINES;
            else if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 48)
                slice_H = SLICE_48_LINES;
            else if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 64)
                slice_H = SLICE_64_LINES;
            else if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 128)
                slice_H = SLICE_128_LINES;
            else if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 256)
                slice_H = SLICE_256_LINES;
            else if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 512)
                slice_H = SLICE_512_LINES;
            else if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 1024)
                slice_H = SLICE_1024_LINES;
            else if(job_req->ImeParam2.pIme_Ext->p1_out_slice_height == 2048)
                slice_H = SLICE_2048_LINES;
            else
            {
                debug_err(("%d %s\r\n",__LINE__,__func__));
            }

            ime_setSliceHeight(slice_H);
        }
        if(job_req->ImeParam1.pIme_Ext->ImeStripe.Ime_StripeMode == IME_STRIPE_USER_MODE)
        {
            //IME h stripe
            _ImeStripeInfo.uHN = job_req->ImeParam2.pIme_Ext->ImeStripe.Hstripe.n;
            _ImeStripeInfo.uHL = job_req->ImeParam2.pIme_Ext->ImeStripe.Hstripe.l;
            _ImeStripeInfo.uHM = job_req->ImeParam2.pIme_Ext->ImeStripe.Hstripe.m;
            //IME v stripe
            _ImeStripeInfo.uVN = job_req->ImeParam2.pIme_Ext->ImeStripe.Vstripe.n;
            _ImeStripeInfo.uVL = job_req->ImeParam2.pIme_Ext->ImeStripe.Vstripe.l;
            _ImeStripeInfo.uVM = job_req->ImeParam2.pIme_Ext->ImeStripe.Vstripe.m;
            //IME stripe mode
            _ImeStripeInfo.StripeCalMode = job_req->ImeParam2.pIme_Ext->ImeStripe.Ime_StripeMode;
            ime_setUserStripeInfo(&_ImeStripeInfo);
        }
        IMEParam.p1_out_scale_filter = job_req->ImeParam2.pIme_Ext->p1_out_scale_filter;
        IMEParam.p1_out_scale_filter.sfCoefCalMode = IME_SCALE_FILTER_COEF_AUTO_MODE;
    }
    else if (JobType == CAPTURE_JOB )
    {
        IMEParam.op_mode = job_req->ImeParam1.pIme_Ext->op_mode;
        IMEParam.in_src = IMEALG_IPE2IME;
        IMEParam.bil = IMEALG_BILINEAR;
        IMEParam.path_num = IMEALG_1PATH;
        IMEParam.interrupt_en = IME_INTE_FRM|IME_INTE_SLICE_DN1|job_req->ImeParam1.pIme_Ext->ImeInten;

        if(IMEParam.interrupt_en & IME_INTE_LINE_BP1)
            IMEParam.breakPoint1 = job_req->ImeParam1.pIme_Ext->Bp1;
        if(IMEParam.interrupt_en & IME_INTE_LINE_BP2)
            IMEParam.breakPoint2 = job_req->ImeParam1.pIme_Ext->Bp2;

        IMEParam.in_size.h_size = job_req->ImeParam1.pIme_fbaddr->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1.pIme_fbaddr->in_v;
        //if(IMEParam.op_mode == IPE2IME_SLICE_MODE)
        //    IMEParam.in_size.v_size -= 8;//cause ipe out vertical will be minus 8;
        IMEParam.line_ofs.in_lineofs.LineOffsetY = job_req->ImeParam1.pIme_fbaddr->in_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCb = job_req->ImeParam1.pIme_fbaddr->in_Cb_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCr = job_req->ImeParam1.pIme_fbaddr->in_Cr_lineoffset;

        IMEParam.format.in_format = job_req->ImeParam1.pIme_fbaddr->in_format;
        IMEParam.format.out1_format = job_req->ImeParam1.pIme_fbaddr->out_format;
        if(job_req->ImeParam1.pIme_fbaddr->out_format==IME_OUTPUT_BAYER)
            IMEParam.p1_out_cfa = job_req->ImeParam1.pIme_Ext->p1_out_cfa;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1.pIme_fbaddr->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1.pIme_fbaddr->out_v;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetY = job_req->ImeParam1.pIme_fbaddr->out_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCb = job_req->ImeParam1.pIme_fbaddr->out_Cb_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCr = job_req->ImeParam1.pIme_fbaddr->out_Cr_lineoffset;


        IMEParam.p1_out_addr.oPPBNums = job_req->ImeParam1.pIme_Ext->Ime_BuffNum;
        switch(IMEParam.p1_out_addr.oPPBNums)
        {
            case IME_OUTPUT_PPB_5:
                IMEParam.p1_out_addr.oDMA4.y_addr = job_req->ImeParam1.pIme_fbaddr->y4_addr;
                IMEParam.p1_out_addr.oDMA4.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb4_addr;
                IMEParam.p1_out_addr.oDMA4.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr4_addr;
            case IME_OUTPUT_PPB_4:
                IMEParam.p1_out_addr.oDMA3.y_addr = job_req->ImeParam1.pIme_fbaddr->y3_addr;
                IMEParam.p1_out_addr.oDMA3.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb3_addr;
                IMEParam.p1_out_addr.oDMA3.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr3_addr;
            default:
                IMEParam.p1_out_addr.oDMA0.y_addr = job_req->ImeParam1.pIme_fbaddr->y0_addr;
                IMEParam.p1_out_addr.oDMA0.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb0_addr;
                IMEParam.p1_out_addr.oDMA0.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr0_addr;
                IMEParam.p1_out_addr.oDMA1.y_addr = job_req->ImeParam1.pIme_fbaddr->y1_addr;
                IMEParam.p1_out_addr.oDMA1.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb1_addr;
                IMEParam.p1_out_addr.oDMA1.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr1_addr;
                IMEParam.p1_out_addr.oDMA2.y_addr = job_req->ImeParam1.pIme_fbaddr->y2_addr;
                IMEParam.p1_out_addr.oDMA2.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb2_addr;
                IMEParam.p1_out_addr.oDMA2.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr2_addr;
                break;
        }
        if(job_req->ImeParam1.pIme_Ext->op_mode == IPE2IME_SLICE_MODE)
        {
            _IME_SLICE_HEIGHT_SEL SliceH = SLICE_32_LINES;
            if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 32)
                SliceH = SLICE_32_LINES;
            else if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 48)
                SliceH = SLICE_48_LINES;
            else if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 64)
                SliceH = SLICE_64_LINES;
            else if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 128)
                SliceH = SLICE_128_LINES;
            else if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 256)
                SliceH = SLICE_256_LINES;
            else if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 512)
                SliceH = SLICE_512_LINES;
            else if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 1024)
                SliceH = SLICE_1024_LINES;
            else if(job_req->ImeParam1.pIme_Ext->p1_out_slice_height == 2048)
                SliceH = SLICE_2048_LINES;

            ime_setSliceHeight(SliceH);
            //IME h stripe
            _ImeStripeInfo.uHN = job_req->ImeParam1.pIme_Ext->ImeStripe.Hstripe.n;
            _ImeStripeInfo.uHL = job_req->ImeParam1.pIme_Ext->ImeStripe.Hstripe.l;
            _ImeStripeInfo.uHM = job_req->ImeParam1.pIme_Ext->ImeStripe.Hstripe.m;
            //IME v stripe
            _ImeStripeInfo.uVN = job_req->ImeParam1.pIme_Ext->ImeStripe.Vstripe.n;
            _ImeStripeInfo.uVL = job_req->ImeParam1.pIme_Ext->ImeStripe.Vstripe.l;
            _ImeStripeInfo.uVM = job_req->ImeParam1.pIme_Ext->ImeStripe.Vstripe.m;
            //IME stripe mode
            _ImeStripeInfo.StripeCalMode = IME_STRIPE_USER_MODE;

            ime_setUserStripeInfo(&_ImeStripeInfo);
        }
        IMEParam.p1_out_scale_filter = job_req->ImeParam1.pIme_Ext->p1_out_scale_filter;
        IMEParam.p1_out_scale_filter.sfCoefCalMode = IME_SCALE_FILTER_COEF_AUTO_MODE;
    }
    else if (JobType == IME_D2D)
    {
        IMEParam.op_mode = DRAM2IME_FRAME_MODE;
        IMEParam.in_src = IMEALG_DMA2IME;
        IMEParam.bil = IMEALG_BILINEAR;
        IMEParam.path_num = IMEALG_1PATH;
        IMEParam.interrupt_en = IME_INTE_FRM;

        if(IMEParam.interrupt_en & IME_INTE_LINE_BP1)
            IMEParam.breakPoint1 = job_req->ImeParam1.pIme_Ext->Bp1;
        if(IMEParam.interrupt_en & IME_INTE_LINE_BP2)
            IMEParam.breakPoint2 = job_req->ImeParam1.pIme_Ext->Bp2;

        IMEParam.in_size.h_size = job_req->ImeParam1.pIme_fbaddr->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1.pIme_fbaddr->in_v;
        IMEParam.line_ofs.in_lineofs.LineOffsetY = job_req->ImeParam1.pIme_fbaddr->in_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCb = job_req->ImeParam1.pIme_fbaddr->in_Cb_lineoffset;
        IMEParam.line_ofs.in_lineofs.LineOffsetCr = job_req->ImeParam1.pIme_fbaddr->in_Cr_lineoffset;

        IMEParam.format.in_format = job_req->ImeParam1.pIme_fbaddr->in_format;
        IMEParam.format.out1_format = job_req->ImeParam1.pIme_fbaddr->out_format;
        if(job_req->ImeParam1.pIme_fbaddr->out_format==IME_OUTPUT_BAYER)
            IMEParam.p1_out_cfa = job_req->ImeParam1.pIme_Ext->p1_out_cfa;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1.pIme_fbaddr->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1.pIme_fbaddr->out_v;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetY = job_req->ImeParam1.pIme_fbaddr->out_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCb = job_req->ImeParam1.pIme_fbaddr->out_Cb_lineoffset;
        IMEParam.line_ofs.p1_out_lineofs.LineOffsetCr = job_req->ImeParam1.pIme_fbaddr->out_Cr_lineoffset;

        IMEParam.in_addr.y_addr = job_req->ImeParam1.pIme_fbaddr->y0_addr;
        IMEParam.in_addr.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb0_addr;
        IMEParam.in_addr.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr0_addr;

        IMEParam.p1_out_addr.oPPBNums = IME_OUTPUT_PPB_1;
        IMEParam.p1_out_addr.oDMA0.y_addr = job_req->ImeParam1.pIme_fbaddr->y1_addr;
        IMEParam.p1_out_addr.oDMA0.cb_addr = job_req->ImeParam1.pIme_fbaddr->cb1_addr;
        IMEParam.p1_out_addr.oDMA0.cr_addr = job_req->ImeParam1.pIme_fbaddr->cr1_addr;


        IMEParam.p1_out_scale_filter = job_req->ImeParam1.pIme_Ext->p1_out_scale_filter;
        IMEParam.p1_out_scale_filter.sfCoefCalMode = IME_SCALE_FILTER_COEF_AUTO_MODE;
        _ImeStripeInfo.StripeCalMode = IME_STRIPE_AUTO_MODE;

    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}


int IME_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return IPP_OK;
}

int IME_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if(JobType == PREVIEW_JOB)
    {
        ime_setMode(&IMEParam);
        ime_setStart();
    }
    else if (JobType == VIDEO_JOB )
    {
        ime_setMode(&IMEParam);
        if(IMEParam.op_mode == IPE2IME_SLICE_MODE)
        {
            ime_triggerSlice();
        }
        ime_setStart();

    }
    else if (JobType == IPE_IME_FRAME_1PATH_JOB)
    {
        ime_setMode(&IMEParam);
    }
    else if (JobType == CAPTURE_JOB )
    {
        ime_setMode(&IMEParam);
        if(IMEParam.op_mode == IPE2IME_SLICE_MODE)
        {
            ime_triggerSlice();
        }
    }
    else if (JobType== IME_D2D)
    {
        ime_setMode(&IMEParam);
        ime_setStart();
        ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
        ime_pause();
    }
    else if(JobType == IME_RTCHG)
    {

    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int IME_EndIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return IPP_OK;
}

int IFE_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    switch(JobType)
    {
        case PREVIEW_JOB:
        case VIDEO_JOB:
            g_IFEParam.mode = IFE_MODE_EMBIPE;
            break;
        case IFE_D2D:
            g_IFEParam.mode = IFE_MODE_D2D;
            break;
        case CAPTURE_JOB:
        case IPE_IME_FRAME_1PATH_JOB:
            g_IFEParam.mode = job_req->pIfeParam->mode;
            break;
        default:
            return IPP_FAIL;
    }

    g_IFEParam.intrEn = IFE_INTE_FRMEND|job_req->pIfeParam->intrEn;
    g_IFEParam.binn = 0;//job_req->pIfeParam->binn;
    g_IFEParam.cfaPat = job_req->pIfeParam->cfaPat;
    if(g_IFEParam.mode == IFE_MODE_EMBIPE)
    {
        g_IFEParam.srcFmt = IFE_FMT_RAW;
        //addr
        g_IFEParam.in_addr = 0;
        g_IFEParam.down_addr = 0;
        g_IFEParam.downSrcEn = 0;
        g_IFEParam.up_addr = 0;
        g_IFEParam.upSrcEn = 0;
        g_IFEParam.out_addr = 0;
        //size
        g_IFEParam.height = 0;
        g_IFEParam.width = 0;
        g_IFEParam.ofsi = 0;
        g_IFEParam.ofso = 0;
    }
    else    //D2D
    {
        g_IFEParam.srcFmt = job_req->pIfeParam->srcFmt;
        //addr
        g_IFEParam.in_addr = job_req->pIfeParam->in_addr;
        g_IFEParam.down_addr = job_req->pIfeParam->down_addr;
        g_IFEParam.downSrcEn = job_req->pIfeParam->downSrcEn;
        g_IFEParam.up_addr = job_req->pIfeParam->up_addr;
        g_IFEParam.upSrcEn = job_req->pIfeParam->upSrcEn;
        g_IFEParam.out_addr = job_req->pIfeParam->out_addr;
        //size
        g_IFEParam.height =  job_req->pIfeParam->height;
        g_IFEParam.width = job_req->pIfeParam->width;
        g_IFEParam.ofsi = job_req->pIfeParam->ofsi;
        g_IFEParam.ofso = job_req->pIfeParam->ofso;
    }
    return IPP_OK;
}

int IFE_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if( (JobType == PREVIEW_JOB)||(JobType == VIDEO_JOB) ||(JobType == CAPTURE_JOB)||(JobType == IPE_IME_FRAME_1PATH_JOB)||(JobType == IFE_D2D))
    {
        //raw def conceal
        g_IFEParam.rowDefEn = job_req->pIfeParam->IFE_filter.rowDefEn;
        g_IFEParam.rowDefVIni = 0;
        g_IFEParam.rowDefTbl = job_req->pIfeParam->IFE_filter.rowDefTbl;
        //outl
        g_IFEParam.outlEn = job_req->pIfeParam->IFE_filter.outlEn;
        g_IFEParam.outlSet.outlBriTh = job_req->pIfeParam->IFE_filter.outlSet[0];
        g_IFEParam.outlSet.outlDarkTh = job_req->pIfeParam->IFE_filter.outlSet[1];
        g_IFEParam.outlSet.outlOp = job_req->pIfeParam->IFE_filter.outlSet[2];
        //filter
        g_IFEParam.filterEn = job_req->pIfeParam->IFE_filter.filterEn;
        g_IFEParam.weights = job_req->pIfeParam->IFE_filter.weights;

        g_IFEParam.sOnlyEn = job_req->pIfeParam->IFE_filter.sOnlyEn;
        g_IFEParam.sOnlyLen = job_req->pIfeParam->IFE_filter.sOnlyLen;

        g_IFEParam.clampWtSet.clampTh = job_req->pIfeParam->IFE_filter.clampWtSet[0];
        g_IFEParam.clampWtSet.wtMul_g = job_req->pIfeParam->IFE_filter.clampWtSet[1];
        g_IFEParam.clampWtSet.wtMul_rb = job_req->pIfeParam->IFE_filter.clampWtSet[2];
        g_IFEParam.rngth_g.rngth = job_req->pIfeParam->IFE_filter.rngth_g;
        g_IFEParam.rngth_g.rdelta = job_req->pIfeParam->IFE_filter.rdelta_g;
        g_IFEParam.rngth_rb.rngth = job_req->pIfeParam->IFE_filter.rngth_rb;
        g_IFEParam.rngth_rb.rdelta = job_req->pIfeParam->IFE_filter.rdelta_rb;
        //curve map
        g_IFEParam.crvmapEn = job_req->pIfeParam->IFE_filter.crvmapEn;
        g_IFEParam.crvmapSet.crvmapDelta = job_req->pIfeParam->IFE_filter.pcrvmapSet->crvmapDelta;
        g_IFEParam.crvmapSet.crvmapInOffs = job_req->pIfeParam->IFE_filter.pcrvmapSet->crvmapInOffs;
        g_IFEParam.crvmapSet.crvmapOutOffs = job_req->pIfeParam->IFE_filter.pcrvmapSet->crvmapOutOffs;
        g_IFEParam.crvmapSet.crvmapGamma = job_req->pIfeParam->IFE_filter.crvmapGamma;
    }
    else if(JobType == IFE_RTCHG)
    {
        g_IfeFilter.binn = g_IFEParam.binn;
        g_IfeFilter.cfaPat = g_IFEParam.cfaPat;
        memcpy(&g_IfeFilter.clampWtSet,job_req->pIfeParam->IFE_filter.clampWtSet,sizeof(UINT32)*3);
        g_IfeFilter.crvmapEn = job_req->pIfeParam->IFE_filter.crvmapEn;
        memcpy(&g_IfeFilter.crvmapSet,job_req->pIfeParam->IFE_filter.pcrvmapSet,sizeof(UINT32)*3);
        g_IfeFilter.crvmapSet.crvmapGamma = job_req->pIfeParam->IFE_filter.crvmapGamma;
        g_IfeFilter.filterEn = job_req->pIfeParam->IFE_filter.filterEn;
        g_IfeFilter.outlEn = job_req->pIfeParam->IFE_filter.outlEn;
        memcpy(&g_IfeFilter.outlSet,job_req->pIfeParam->IFE_filter.outlSet,sizeof(UINT32)*3);
        g_IfeFilter.rngth_g.rngth = job_req->pIfeParam->IFE_filter.rngth_g;
        g_IfeFilter.rngth_g.rdelta = job_req->pIfeParam->IFE_filter.rdelta_g;
        g_IfeFilter.rngth_rb.rngth = job_req->pIfeParam->IFE_filter.rngth_rb;
        g_IfeFilter.rngth_rb.rdelta = job_req->pIfeParam->IFE_filter.rdelta_rb;
        g_IfeFilter.rowDefEn = job_req->pIfeParam->IFE_filter.rowDefEn;
        g_IfeFilter.rowDefTbl = job_req->pIfeParam->IFE_filter.rowDefTbl;
        g_IfeFilter.rowDefVIni = job_req->pIfeParam->IFE_filter.rowDefVIni;
        g_IfeFilter.sOnlyEn = job_req->pIfeParam->IFE_filter.sOnlyEn;
        g_IfeFilter.srcFmt = g_IFEParam.srcFmt;
        g_IfeFilter.weights = job_req->pIfeParam->IFE_filter.weights;
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int IFE_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if( (JobType == PREVIEW_JOB)||(JobType == VIDEO_JOB))
    {
        ife_setMode(&g_IFEParam, IFE_NOWAIT);//_ife_waitmode is don't care when embedded in ipe
    }
    else if((JobType == CAPTURE_JOB)||(JobType == IPE_IME_FRAME_1PATH_JOB))
    {
        ife_setMode(&g_IFEParam, IFE_NOWAIT);//_ife_waitmode is don't care when embedded
    }
    else if(JobType == IFE_D2D)
    {
        ife_setMode(&g_IFEParam, IFE_NOWAIT);//_ife_waitmode is don't care when embedded
    }
    else if(JobType == IFE_RTCHG)
    {
        ife_changeParam(&g_IfeFilter);
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int IFE_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return IPP_OK;
}
