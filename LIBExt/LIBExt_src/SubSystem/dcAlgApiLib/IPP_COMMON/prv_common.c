#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "debug.h"
#include "IPPManager.h"
#include "sen_common.h"
#include "IPPBuffer.h"
#include "prv_common.h"
#include "Alg_IPLMode.h"
#include "IPPCtrl.h"
#include "SensorFlowFunc.h"
#include "SensorSys.h"
#include "sen_isr.h"
#include "pll.h"

#define IpeBugPatch ENABLE

static __inline void SIEReset(void);
static __inline void IPEReset(void);
static __inline void IMEReset(void);

void SetPrvParams(IPP_PARAM *IppParam_Prv,BOOL bPath2)
{
    PImageAlgInfo _pimginfo;
    UINT32 _shift=0;
    SliceSizeInfo _OutInfo;
    SliceGenIn _InInfo;
    UINT32 _ImeVSize;
    UINT32 _Fmt;
    _pimginfo = UI_GetParameter();
    //generator slice in/out size of each engine
    _InInfo.Sie_SliceH_Default = (GetIPPSizeInfor(_SieOutVsize) + IppParam_Prv->Sie_Struct->SieOutBufNum)/(IppParam_Prv->Sie_Struct->SieOutBufNum+1);
    _InInfo.IpeInH = GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF;
    _InInfo.IpeInV = GetIPPSizeInfor(_IpeOutVsize);
    _InInfo.IpeMode = IppParam_Prv->pIpe_Struct->StripeMode;
    if(bPath2)
    {
        _InInfo.ImeOutH = GetIPPSizeInfor(_ImeOut2Hsize);
        _InInfo.ImeOutV = GetIPPSizeInfor(_ImeOut2Vsize);
        if(IppParam_Prv->Ime_Struct2.pIme_Ext->op_mode == IPE2IME_SLICE_MODE)
            _InInfo.ImeSliceH = IppParam_Prv->Ime_Struct2.pIme_Ext->p1_out_slice_height;
        else
            _InInfo.ImeSliceH= 0;
        _InInfo.ImeMode = IppParam_Prv->Ime_Struct2.pIme_Ext->op_mode;
    }
    else
    {
        _InInfo.ImeOutH = GetIPPSizeInfor(_ImeOut1Hsize);
        _InInfo.ImeOutV = GetIPPSizeInfor(_ImeOut1Vsize);
        if(IppParam_Prv->Ime_Struct.pIme_Ext->op_mode == IPE2IME_SLICE_MODE)
            _InInfo.ImeSliceH = IppParam_Prv->Ime_Struct.pIme_Ext->p1_out_slice_height;
        else
            _InInfo.ImeSliceH= 0;
        _InInfo.ImeMode = IppParam_Prv->Ime_Struct.pIme_Ext->op_mode;
    }
    SliceSize_Gen(&_OutInfo,_InInfo);

    //sensor signal setting
    IppParam_Prv->Sie_Struct->VDTotal = GetIPPSizeInfor(_VDTotal);
    IppParam_Prv->Sie_Struct->VDSync = GetIPPSizeInfor(_VDSync);
    IppParam_Prv->Sie_Struct->HDActX = GetIPPSizeInfor(_HDActX);
    IppParam_Prv->Sie_Struct->HDActY = GetIPPSizeInfor(_HDActY);
    IppParam_Prv->Sie_Struct->HDActSizeX = GetIPPSizeInfor(_HDActSizeX);
    IppParam_Prv->Sie_Struct->HDActSizeY = GetIPPSizeInfor(_HDActSizeY);
    IppParam_Prv->Sie_Struct->HDTotal = GetIPPSizeInfor(_HDTotal);
    IppParam_Prv->Sie_Struct->HDSync = GetIPPSizeInfor(_HDSync);

//Sie setting
    IppParam_Prv->Sie_Struct->RawCenterX = GetIPPSizeInfor(_SieXcenter);
    IppParam_Prv->Sie_Struct->RawCenterY = GetIPPSizeInfor(_SieYcenter);
    IppParam_Prv->Sie_Struct->SieCropSizeH = GetIPPSizeInfor(_SieCropHsize);
    IppParam_Prv->Sie_Struct->SieCropSizeV = GetIPPSizeInfor(_SieCropVsize);
    IppParam_Prv->Sie_Struct->SieOutSizeH = GetIPPSizeInfor(_SieOutHsize);
    IppParam_Prv->Sie_Struct->SieOutLineOfs = GetIPPSizeInfor(_SieOutLineOffset);
    if(IppParam_Prv->Sie_Struct->SieOutModSel == SIE_SLICE_MODE)
        IppParam_Prv->Sie_Struct->SieSliceHight = _OutInfo.Sie_SliceH;

    switch(IppParam_Prv->Sie_Struct->SieOutBufNum)
    {
        case THREE_OUTPUT_BUF:
            IppParam_Prv->Sie_Struct->SieOutAdd2 = ippbuf_GetBuf(IPPBUF_PRV_SIERAW_3);
        case TWO_OUTPUT_BUF:
            IppParam_Prv->Sie_Struct->SieOutAdd1 = ippbuf_GetBuf(IPPBUF_PRV_SIERAW_2);
        case ONE_OUTPUT_BUF:
            IppParam_Prv->Sie_Struct->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1);
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
            break;
    }
#if IpeBugPatch
    if(IppParam_Prv->Sie_Struct->SieOutModSel == SIE_SLICE_MODE)
    {
        IppParam_Prv->Sie_Struct->SieOutAdd1 = IppParam_Prv->Sie_Struct->SieOutAdd0 +
            GetIPPSizeInfor(_SieOutLineOffset)*IppParam_Prv->Sie_Struct->SieSliceHight;
        IppParam_Prv->Sie_Struct->SieOutAdd2 = IppParam_Prv->Sie_Struct->SieOutAdd1 +
            GetIPPSizeInfor(_SieOutLineOffset)*IppParam_Prv->Sie_Struct->SieSliceHight;
    }
#endif
    if(IppParam_Prv->Sie_Struct->SieOutBufNum == TWO_OUTPUT_BUF)
        if(IppParam_Prv->Sie_Struct->SieOutModSel == SIE_SLICE_MODE)
            debug_msg("SIE SIE_SLICE_MODE do not support 2-pingppong!\r\n");

    IppParam_Prv->Sie_Struct->SieObOfs = GetIPPSizeInfor(_OBOffset);

    switch(_pimginfo->SieInBitDepth)
    {
        case _SieBitDepth_8:
            IppParam_Prv->Sie_Struct->SieInBitDepth = SIE_IN_PACK_8BIT;
            break;
        case _SieBitDepth_10:
            IppParam_Prv->Sie_Struct->SieInBitDepth = SIE_IN_PACK_10BIT;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }

    switch(_pimginfo->SieBitDepth)
    {
        case _SieBitDepth_8:
            IppParam_Prv->Sie_Struct->SieBitDepth = SIE_OUT_PACK_8BIT;
            break;
        case _SieBitDepth_10:
            IppParam_Prv->Sie_Struct->SieBitDepth = SIE_OUT_PACK_10BIT;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }
//Ipe setting
    //IPE VA only output to ipe registor
    //IppParam_Prv->pIpe_Struct->VAAddr = ippbuf_GetBuf(IPPBUF_PRV_VA);
    //only prv ife can embeded in ipe, disable this bit must stop IPL first
    IppParam_Prv->pIpe_Struct->IFEembed_en = ENABLE;

    switch(IppParam_Prv->Sie_Struct->SieOutBufNum)
    {
        case THREE_OUTPUT_BUF:
            IppParam_Prv->pIpe_Struct->Add2 = IppParam_Prv->Sie_Struct->SieOutAdd2;
        case TWO_OUTPUT_BUF:
            IppParam_Prv->pIpe_Struct->Add1 = IppParam_Prv->Sie_Struct->SieOutAdd1;
        case ONE_OUTPUT_BUF:
            IppParam_Prv->pIpe_Struct->Add0 = IppParam_Prv->Sie_Struct->SieOutAdd0;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
    }

    IppParam_Prv->pIpe_Struct->BitDepth = IppParam_Prv->Sie_Struct->SieBitDepth;
    //IppParam_Prv->pIpe_Struct->InSizeH = IppParam_Prv->Sie_Struct->SieOutSizeH;
    IppParam_Prv->pIpe_Struct->InSizeH = GetIPPSizeInfor(_IpeOutHsize)+IPE_HSIZE_IODIFF;
    IppParam_Prv->pIpe_Struct->InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    IppParam_Prv->pIpe_Struct->InLineOfs = IppParam_Prv->Sie_Struct->SieOutLineOfs;
    if(IppParam_Prv->pIpe_Struct->StripeMode==IPE_AUTOSLICE)
    {
        IppParam_Prv->pIpe_Struct->IpeSliceH = IppParam_Prv->Sie_Struct->SieSliceHight;
    }
    IppParam_Prv->pIpe_Struct->HStripe.stripe.n = _OutInfo.Ipe_Hstripe.n;
    IppParam_Prv->pIpe_Struct->HStripe.stripe.l = _OutInfo.Ipe_Hstripe.l;
    IppParam_Prv->pIpe_Struct->HStripe.stripe.m = _OutInfo.Ipe_Hstripe.m;

    IppParam_Prv->pIpe_Struct->VStripe.stripe.n = _OutInfo.Ipe_Vstripe.n;
    IppParam_Prv->pIpe_Struct->VStripe.stripe.l = _OutInfo.Ipe_Vstripe.l;
    IppParam_Prv->pIpe_Struct->VStripe.stripe.m = _OutInfo.Ipe_Vstripe.m;

    //stripe overlap
    IppParam_Prv->pIpe_Struct->HStripe.olap = IPE_OLAP_ON;
    IppParam_Prv->pIpe_Struct->VStripe.olap = IPE_OLAP_ON;

//IFE setting
//IME setting
    _Fmt = ((bPath2==TRUE)?(IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_format):(IppParam_Prv->Ime_Struct.pIme_fbaddr->out_format));
    if(bPath2==TRUE)
        SetIPPSizeInfor(_ImePath2Fmt, _Fmt);
    else
        SetIPPSizeInfor(_ImePath1Fmt, _Fmt);

    switch(_Fmt)
    {
        case IME_OUTPUT_YCC_444:
            _shift = 1;
            _Fmt = IMEIN_FMT444;
            break;
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
            _shift = 1;
            _Fmt = IMEIN_FMT422;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            _shift = 2;
            _Fmt = IMEIN_FMT420;
            break;
    }
//IME PATH1
    if(bPath2)
    {
        IppParam_Prv->Ime_Struct.pIme_fbaddr->in_format = _Fmt;//this fmt is from jpg encode dc out
        IppParam_Prv->Ime_Struct.pIme_fbaddr->out_format = IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_format;
    }
    else
        IppParam_Prv->Ime_Struct.pIme_fbaddr->in_format = IMEIN_FMT444;//IPE always 444 fmt
    //in size
    IppParam_Prv->Ime_Struct.pIme_fbaddr->in_h=GetIPPSizeInfor(_IpeOutHsize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->in_v=GetIPPSizeInfor(_IpeOutVsize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->in_lineoffset = GetIPPSizeInfor(_IpeOutHsize);
    switch(IppParam_Prv->Ime_Struct.pIme_fbaddr->in_format)
    {
        case IMEIN_FMT444:
            IppParam_Prv->Ime_Struct.pIme_fbaddr->in_Cb_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->in_lineoffset;
            IppParam_Prv->Ime_Struct.pIme_fbaddr->in_Cr_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->in_lineoffset;
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
        case IMEIN_FMT422:
        case IMEIN_FMT420:
            IppParam_Prv->Ime_Struct.pIme_fbaddr->in_Cb_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->in_lineoffset/2;
            IppParam_Prv->Ime_Struct.pIme_fbaddr->in_Cr_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->in_lineoffset/2;
            break;
    }
    //out size
    IppParam_Prv->Ime_Struct.pIme_fbaddr->out_h = GetIPPSizeInfor(_ImeOut1Hsize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->out_v = GetIPPSizeInfor(_ImeOut1Vsize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset = GetIPPSizeInfor(_ImeOut1LineOffset);
    switch(IppParam_Prv->Ime_Struct.pIme_fbaddr->out_format)
    {
        case IME_OUTPUT_YCC_444:
            IppParam_Prv->Ime_Struct.pIme_fbaddr->out_Cb_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset;
            IppParam_Prv->Ime_Struct.pIme_fbaddr->out_Cr_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset;
            break;
        default:
            debug_err(("%d %s \r\n",__LINE__,__func__));
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            IppParam_Prv->Ime_Struct.pIme_fbaddr->out_Cb_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset/2;
            IppParam_Prv->Ime_Struct.pIme_fbaddr->out_Cr_lineoffset = IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset/2;
        break;
    }
    //ime stripe info
    IppParam_Prv->Ime_Struct.pIme_Ext->ImeStripe.Hstripe.n = _OutInfo.Ime_Hstripe.n;
    IppParam_Prv->Ime_Struct.pIme_Ext->ImeStripe.Hstripe.l = _OutInfo.Ime_Hstripe.l;
    IppParam_Prv->Ime_Struct.pIme_Ext->ImeStripe.Hstripe.m = _OutInfo.Ime_Hstripe.m;
    IppParam_Prv->Ime_Struct.pIme_Ext->ImeStripe.Vstripe.n = _OutInfo.Ime_Vstripe.n;
    IppParam_Prv->Ime_Struct.pIme_Ext->ImeStripe.Vstripe.l = _OutInfo.Ime_Vstripe.l;
    IppParam_Prv->Ime_Struct.pIme_Ext->ImeStripe.Vstripe.m = _OutInfo.Ime_Vstripe.m;
    IppParam_Prv->Ime_Struct.pIme_Ext->ImeStripe.Ime_StripeMode = IME_STRIPE_USER_MODE;
    //ime out addr
    _ImeVSize = GetIPPSizeInfor(_ImeOut1Vsize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->cb0_addr=(IppParam_Prv->Ime_Struct.pIme_fbaddr->y0_addr+IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset*_ImeVSize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->cr0_addr=(IppParam_Prv->Ime_Struct.pIme_fbaddr->cb0_addr+IppParam_Prv->Ime_Struct.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->y1_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1_2);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->cb1_addr=(IppParam_Prv->Ime_Struct.pIme_fbaddr->y1_addr+IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset*_ImeVSize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->cr1_addr=(IppParam_Prv->Ime_Struct.pIme_fbaddr->cb1_addr+IppParam_Prv->Ime_Struct.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->y2_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1_3);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->cb2_addr=(IppParam_Prv->Ime_Struct.pIme_fbaddr->y2_addr+IppParam_Prv->Ime_Struct.pIme_fbaddr->out_lineoffset*_ImeVSize);
    IppParam_Prv->Ime_Struct.pIme_fbaddr->cr2_addr=(IppParam_Prv->Ime_Struct.pIme_fbaddr->cb2_addr+IppParam_Prv->Ime_Struct.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
//IME PATH2
    if(bPath2)
    {
        //encode dc out for panel for 96220
        //ime out for jpg encode
        IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_format = IMEIN_FMT444;//IPE always 444 fmt
        //in size
        IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_h=GetIPPSizeInfor(_IpeOutHsize);
        IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_v=GetIPPSizeInfor(_IpeOutVsize);
        IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_lineoffset = GetIPPSizeInfor(_IpeOutHsize);
        switch(IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_format)
        {
            case IMEIN_FMT444:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_Cb_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_lineoffset;
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_Cr_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_lineoffset;
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
            case IMEIN_FMT422:
            case IMEIN_FMT420:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_Cb_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_lineoffset/2;
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_Cr_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->in_lineoffset/2;
                break;
        }
        //out size
        IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_h=GetIPPSizeInfor(_ImeOut2Hsize);
        IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_v=GetIPPSizeInfor(_ImeOut2Vsize);
        IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset=GetIPPSizeInfor(_ImeOut2LineOffset);
        switch(IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_format)
        {
            case IME_OUTPUT_YCC_444:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cb_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset;
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cr_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset;
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
            case IME_OUTPUT_YCC_422_COS:
            case IME_OUTPUT_YCC_422_CEN:
            case IME_OUTPUT_YCC_420_COS:
            case IME_OUTPUT_YCC_420_CEN:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cb_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset/2;
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cr_lineoffset = IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset/2;
                break;
        }
        //ime stripe info
        IppParam_Prv->Ime_Struct2.pIme_Ext->ImeStripe.Hstripe.n = _OutInfo.Ime_Hstripe.n;
        IppParam_Prv->Ime_Struct2.pIme_Ext->ImeStripe.Hstripe.l = _OutInfo.Ime_Hstripe.l;
        IppParam_Prv->Ime_Struct2.pIme_Ext->ImeStripe.Hstripe.m = _OutInfo.Ime_Hstripe.m;
        IppParam_Prv->Ime_Struct2.pIme_Ext->ImeStripe.Vstripe.n = _OutInfo.Ime_Vstripe.n;
        IppParam_Prv->Ime_Struct2.pIme_Ext->ImeStripe.Vstripe.l = _OutInfo.Ime_Vstripe.l;
        IppParam_Prv->Ime_Struct2.pIme_Ext->ImeStripe.Vstripe.m = _OutInfo.Ime_Vstripe.m;
        IppParam_Prv->Ime_Struct2.pIme_Ext->ImeStripe.Ime_StripeMode = IME_STRIPE_USER_MODE;
        //ime out addr
        if(IppParam_Prv->Ime_Struct2.pIme_Ext->op_mode == IPE2IME_SLICE_MODE)
            _ImeVSize = IppParam_Prv->Ime_Struct2.pIme_Ext->p1_out_slice_height;
        else
            _ImeVSize = GetIPPSizeInfor(_ImeOut2Vsize);
        switch(IppParam_Prv->Ime_Struct2.pIme_Ext->Ime_BuffNum)
        {
            case IME_OUTPUT_PPB_5:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->y4_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P2_5);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb4_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->y4_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset*_ImeVSize);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cr4_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb4_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
            case IME_OUTPUT_PPB_4:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->y3_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P2_4);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb3_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->y3_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset*_ImeVSize);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cr3_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb3_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
            case IME_OUTPUT_PPB_3:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->y2_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P2_3);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb2_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->y2_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset*_ImeVSize);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cr2_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb2_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
            case IME_OUTPUT_PPB_2:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->y1_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P2_2);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb1_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->y1_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset*_ImeVSize);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cr1_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb1_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
            case IME_OUTPUT_PPB_1:
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P2);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb0_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->y0_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_lineoffset*_ImeVSize);
                IppParam_Prv->Ime_Struct2.pIme_fbaddr->cr0_addr=(IppParam_Prv->Ime_Struct2.pIme_fbaddr->cb0_addr+IppParam_Prv->Ime_Struct2.pIme_fbaddr->out_Cb_lineoffset*_ImeVSize/_shift);
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
                break;
        }
    }
    //Ife setting
    IppParam_Prv->Ife_Struct->cfaPat = IppParam_Prv->pIpe_Struct->CfaPat;
}

UINT32 Set2Prv_Common(IPP_PARAM *IppParam_Prv, UINT32 SenInitEn, BOOL bPath2)
{
    static BOOL FirstFlg = TRUE;

    SetSensorMode(IppParam_Prv->Sie_Struct->ModeSel);
    SetPrvParams(IppParam_Prv, bPath2);

    if (FirstFlg == TRUE)
    {
        IMEReset();
        IPEReset();
        SIEReset();
    }

    if(bPath2)
        ime_Set2Video(IppParam_Prv->Ime_Struct.pIme_fbaddr,IppParam_Prv->Ime_Struct2.pIme_fbaddr,IppParam_Prv->Ime_Struct.pIme_Ext,IppParam_Prv->Ime_Struct2.pIme_Ext);
    else
        ime_Set2Preview(IppParam_Prv->Ime_Struct.pIme_fbaddr,IppParam_Prv->Ime_Struct.pIme_Ext);

    if(bPath2)
    {
        if(IppParam_Prv->pIpe_Struct->IFEembed_en)
            ipe_ife_Set2Video(IppParam_Prv->pIpe_Struct,IppParam_Prv->Ife_Struct);
        else
            ipe_Set2Video(IppParam_Prv->pIpe_Struct);
    }
    else
    {
        if(IppParam_Prv->pIpe_Struct->IFEembed_en)
            ipe_ife_Set2Preview(IppParam_Prv->pIpe_Struct,IppParam_Prv->Ife_Struct);
        else
            ipe_Set2Preview(IppParam_Prv->pIpe_Struct);
    }

    //ethan_20110803_move 2 here inorder let sie get right Digital Gain
    if (FirstFlg == TRUE)
    {
        SenFlowFunc(AE_PRV_INIT);
        SenFlowFunc(AWB_PRV_INIT);
    }
    else
    {
        SenFlowFunc(AE_PRV_SETTING);
        SenFlowFunc(AWB_PRV_SETTING);
    }
    if(bPath2)
        sie_Set2Video(IppParam_Prv->Sie_Struct);
    else
        sie_Set2Preview(IppParam_Prv->Sie_Struct);

    //cfg isrcb information
    {
        UINT32 Rst[4] = {0};
        Rst[0] = ime_getEngineInfo(IME_GET_OPERATED_SLICE_NUM);
        if(bPath2)
            Rst[1] = IppParam_Prv->Ime_Struct2.pIme_Ext->op_mode;
        else
            Rst[1] = IppParam_Prv->Ime_Struct.pIme_Ext->op_mode;

        Rst[2] = IppParam_Prv->Sie_Struct->SieOutModSel;
        SetIsrInfo((void*)Rst);
    }

    if ((IppParam_Prv->Sie_Struct->SieMaster == MasterMode) && ((SenInitEn == ENABLE)))
    {
    }

    if(bPath2)
    {
        if(GetIPPFuncInfor(VID_Datestamp_EN))
        {
            if(g_fpAlgExport)
            {
                gAlgMsg.VidJpg_Hsize = GetIPPSizeInfor(_ImeOut2Hsize);
                gAlgMsg.VidJpg_Vsize = GetIPPSizeInfor(_ImeOut2Vsize);
                gAlgMsg.VidStampBuf = ippbuf_GetBuf(IPPBUF_VID_DATA_STAMP);
                g_fpAlgExport(ALGMSG_VidDateStampCfg, &gAlgMsg);
            }
        }
    }

    FirstFlg = FALSE;
    sie_start();
    return 0;
}

//#NT#2009/10/20#Adrian Jiang -begin
//#NT#Rearrange the code,
//#NT#the code related to preview flow, move from sen_common.c to prv_common.c
//
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void sie_Set2Preview(SIE_PRE_PARAM *SiePreParam)
{
    IPPMgrReq.SIE_PRE_PRV = SiePreParam;

    if (Sensor_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    SenFlowFunc(SIE_PRV_SETTING);

    if ( SIE_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_AllocatResourceIPP() error \n"));


    if ( SIE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
}

void sie_Set2Video(SIE_PRE_PARAM *SiePreParam)
{
    IPPMgrReq.SIE_PRE_PRV = SiePreParam;

    if (Sensor_BeginIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    SenFlowFunc(SIE_PRV_SETTING);

    if ( SIE_AllocatResourceIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("SIE_AllocatResourceIPP() error \n"));

    if ( SIE_ExecIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
}

void sie_Set2Dzoom(void)
{
    STR_SIE_CROP CropParam;

    CropParam.Hstart = SIE_PRE_Para_prv.RawCenterX - SIE_PRE_Para_prv.SieCropSizeH/2 - SIE_PRE_Para_prv.HDActX;
    CropParam.Vstart = SIE_PRE_Para_prv.RawCenterY - SIE_PRE_Para_prv.SieCropSizeV/2 - SIE_PRE_Para_prv.HDActY;
    CropParam.Hsize = SIE_PRE_Para_prv.SieCropSizeH;
    CropParam.Vsize = SIE_PRE_Para_prv.SieCropSizeV;
    sie_setSliceHeight(SIE_PRE_Para_prv.SieSliceHight);
    sie_changeOutputSize(&CropParam, SIE_PRE_Para_prv.SieOutSizeH,SIE_PRE_Para_prv.SieOutLineOfs);

}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void ipe_Set2Preview(IPE_PARAM *IpeParam)
{
    IPPMgrReq.IpeParam = IpeParam;

    if ( IPE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \r\n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \r\n"));

    SenFlowFunc(IPE_PRV_SETTING);

    if ( IPE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_ExecIPP() error \r\n"));
}
//ife embedded in ipe
void ipe_ife_Set2Preview(pIPE_PARAM _IpeParam, pIFE_ENG_SETTING _pIfeParam)
{
    IPPMgrReq.IpeParam = _IpeParam;
    IPPMgrReq.pIfeParam = _pIfeParam;

    if ( IPE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \r\n"));

    if ( IFE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IFE_BeginIPP() error \r\n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \r\n"));

    SenFlowFunc(IFE_PRV_SETTING);

    SenFlowFunc(IPE_PRV_SETTING);

    if ( IPE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_ExecIPP() error \r\n"));

}

void ipe_Set2Video(IPE_PARAM *IpeParam)
{
    IPPMgrReq.IpeParam = IpeParam;

    if ( IPE_BeginIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \r\n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \r\n"));

    SenFlowFunc(IPE_PRV_SETTING);

    if ( IPE_ExecIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IPE_ExecIPP() error \r\n"));
}

//ife embedded in ipe
void ipe_ife_Set2Video(pIPE_PARAM _IpeParam, pIFE_ENG_SETTING _pIfeParam)
{
    IPPMgrReq.IpeParam = _IpeParam;
    IPPMgrReq.pIfeParam = _pIfeParam;

    if ( IPE_BeginIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \r\n"));

    if ( IFE_BeginIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IFE_BeginIPP() error \r\n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \r\n"));

    SenFlowFunc(IFE_VIDREC_SETTING);

    SenFlowFunc(IPE_PRV_SETTING);

    if ( IPE_ExecIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IPE_ExecIPP() error \r\n"));

}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void ime_Set2Preview(pIME_FBADDR_PRAM  pIME_Para1,pIME_EXTEN pIME_Ext1)
{
    IPPMgrReq.ImeParam1.pIme_fbaddr = pIME_Para1;
    IPPMgrReq.ImeParam1.pIme_Ext = pIME_Ext1;

    if ( IME_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IME_BeginIPP() error \n"));

    if ( IME_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IME_AllocatResourceIPP() error \n"));

    SenFlowFunc(IME_PRV_SETTING);

    if ( IME_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
}

void ime_Set2Video(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2,pIME_EXTEN pIME_Ext1,pIME_EXTEN pIME_Ext2)
{
    IPPMgrReq.ImeParam1.pIme_fbaddr = pIME_Para1;
    IPPMgrReq.ImeParam2.pIme_fbaddr = pIME_Para2;
    IPPMgrReq.ImeParam1.pIme_Ext = pIME_Ext1;
    IPPMgrReq.ImeParam2.pIme_Ext = pIME_Ext2;

    if ( IME_BeginIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IME_BeginIPP() error \n"));

    if ( IME_AllocatResourceIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IME_AllocatResourceIPP() error \n"));

    if ( IME_ExecIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
}


void ime_SetDzoom2Video(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2,pIME_EXTEN pIME_Ext1,pIME_EXTEN pIME_Ext2)
{
//KH mark
#if 0
    IMEParam.op_mode = _CHANGESIZE_2PATH;
    IMEParam.in_size.h_size = pIME_Para1->in_h;
    IMEParam.in_size.v_size = pIME_Para1->in_v;
    IMEParam.format.in_format = pIME_Para1->in_format;

    IMEParam.format.out1_format = pIME_Para1->out_format;
    IMEParam.line_ofs.out1_lineofs = pIME_Para1->out_lineoffset;
    IMEParam.p1_out_size.h_size = pIME_Para1->out_h;
    IMEParam.p1_out_size.v_size = pIME_Para1->out_v;
    IMEParam.p1_out0_addr.y_addr = pIME_Para1->y0_addr;
    IMEParam.p1_out0_addr.cb_addr = pIME_Para1->cb0_addr;
    IMEParam.p1_out0_addr.cr_addr = pIME_Para1->cr0_addr;
    IMEParam.p1_out1_addr.y_addr = pIME_Para1->y1_addr;
    IMEParam.p1_out1_addr.cb_addr = pIME_Para1->cb1_addr;
    IMEParam.p1_out1_addr.cr_addr = pIME_Para1->cr1_addr;
    IMEParam.p1_out2_addr.y_addr = pIME_Para1->y2_addr;
    IMEParam.p1_out2_addr.cb_addr = pIME_Para1->cb2_addr;
    IMEParam.p1_out2_addr.cr_addr = pIME_Para1->cr2_addr;

    IMEParam.format.out2_format = pIME_Para2->out_format;
    IMEParam.line_ofs.out2_lineofs = pIME_Para2->out_lineoffset;
    IMEParam.p2_out_size.h_size = pIME_Para2->out_h;
    IMEParam.p2_out_size.v_size = pIME_Para2->out_v;
    IMEParam.p2_out0_addr.y_addr = pIME_Para2->y0_addr;
    IMEParam.p2_out0_addr.cb_addr = pIME_Para2->cb0_addr;
    IMEParam.p2_out0_addr.cr_addr = pIME_Para2->cr0_addr;
    IMEParam.p2_out1_addr.y_addr = pIME_Para2->y1_addr;
    IMEParam.p2_out1_addr.cb_addr = pIME_Para2->cb1_addr;
    IMEParam.p2_out1_addr.cr_addr = pIME_Para2->cr1_addr;
    IMEParam.p2_out2_addr.y_addr = pIME_Para2->y2_addr;
    IMEParam.p2_out2_addr.cb_addr = pIME_Para2->cb2_addr;
    IMEParam.p2_out2_addr.cr_addr = pIME_Para2->cr2_addr;

    ime_setMode(&IMEParam);
    ime_setStart();
#endif
}

//comment -adrian

void ime_SetDzoom2Video_Path2(pIME_FBADDR_PRAM  pIME_Para2,pIME_EXTEN pIME_Ext2)
{
    //#NT#2011/01/13#ethanlau -begin
    //#NT# IME N/L/M and IME format is not enough
#if 1
    IME_HV_STRIPE_INFO _ImeStripeInfo;
    IME_IO_CHG_SIZE _ImeSize;

    _ImeSize.p1_out_addr.oPPBNums = pIME_Ext2->Ime_BuffNum;
    switch(_ImeSize.p1_out_addr.oPPBNums)
    {
        case IME_OUTPUT_PPB_5:
            _ImeSize.p1_out_addr.oDMA4.y_addr = pIME_Para2->y4_addr;
            _ImeSize.p1_out_addr.oDMA4.cb_addr = pIME_Para2->cb4_addr;
            _ImeSize.p1_out_addr.oDMA4.cr_addr = pIME_Para2->cr4_addr;
        case IME_OUTPUT_PPB_4:
            _ImeSize.p1_out_addr.oDMA3.y_addr = pIME_Para2->y3_addr;
            _ImeSize.p1_out_addr.oDMA3.cb_addr = pIME_Para2->cb3_addr;
            _ImeSize.p1_out_addr.oDMA3.cr_addr = pIME_Para2->cr3_addr;
        case IME_OUTPUT_PPB_3:
            _ImeSize.p1_out_addr.oDMA2.y_addr = pIME_Para2->y2_addr;
            _ImeSize.p1_out_addr.oDMA2.cb_addr = pIME_Para2->cb2_addr;
            _ImeSize.p1_out_addr.oDMA2.cr_addr = pIME_Para2->cr2_addr;
        case IME_OUTPUT_PPB_2:
            _ImeSize.p1_out_addr.oDMA1.y_addr = pIME_Para2->y1_addr;
            _ImeSize.p1_out_addr.oDMA1.cb_addr = pIME_Para2->cb1_addr;
            _ImeSize.p1_out_addr.oDMA1.cr_addr = pIME_Para2->cr1_addr;
        case IME_OUTPUT_PPB_1:
            _ImeSize.p1_out_addr.oDMA0.y_addr = pIME_Para2->y0_addr;
            _ImeSize.p1_out_addr.oDMA0.cb_addr = pIME_Para2->cb0_addr;
            _ImeSize.p1_out_addr.oDMA0.cr_addr = pIME_Para2->cr0_addr;
            break;
    }
    _ImeSize.in_size.h_size = pIME_Para2->in_h;
    _ImeSize.in_size.v_size = pIME_Para2->in_v;
    _ImeSize.line_ofs.in_lineofs.LineOffsetY = pIME_Para2->in_lineoffset;
    _ImeSize.line_ofs.in_lineofs.LineOffsetCb = pIME_Para2->in_Cb_lineoffset;
    _ImeSize.line_ofs.in_lineofs.LineOffsetCr = pIME_Para2->in_Cr_lineoffset;

    _ImeSize.p1_out_size.h_size = pIME_Para2->out_h;
    _ImeSize.p1_out_size.v_size = pIME_Para2->out_v;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetY = pIME_Para2->out_lineoffset;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetCb = pIME_Para2->out_Cb_lineoffset;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetCr = pIME_Para2->out_Cr_lineoffset;

    //IME h stripe
    _ImeStripeInfo.uHN = pIME_Ext2->ImeStripe.Hstripe.n;
    _ImeStripeInfo.uHL = pIME_Ext2->ImeStripe.Hstripe.l;
    _ImeStripeInfo.uHM = pIME_Ext2->ImeStripe.Hstripe.m;
    //IME v stripe
    _ImeStripeInfo.uVN = pIME_Ext2->ImeStripe.Vstripe.n;
    _ImeStripeInfo.uVL = pIME_Ext2->ImeStripe.Vstripe.l;
    _ImeStripeInfo.uVM = pIME_Ext2->ImeStripe.Vstripe.m;
    //IME stripe mode
    _ImeStripeInfo.StripeCalMode = pIME_Ext2->ImeStripe.Ime_StripeMode;
    ime_setUserStripeInfo(&_ImeStripeInfo);

    ime_chgParam((void*)&_ImeSize,IME_CHG_IO_SIZE);
#endif
    //#NT#2011/01/13#ethanlau -end
}

//comment -adrian

void ime_SetDzoom2Video_Path1(pIME_FBADDR_PRAM  pIME_Para1)
{
    IME_IO_CHG_SIZE _ImeSize;
    IME_HV_STRIPE_INFO _ImeStripeInfo;
    _ImeSize.p1_out_addr.oPPBNums = IME_OUTPUT_PPB_3;
    switch(_ImeSize.p1_out_addr.oPPBNums)
    {
        case IME_OUTPUT_PPB_5:
            _ImeSize.p1_out_addr.oDMA4.y_addr = pIME_Para1->y4_addr;
            _ImeSize.p1_out_addr.oDMA4.cb_addr = pIME_Para1->cb4_addr;
            _ImeSize.p1_out_addr.oDMA4.cr_addr = pIME_Para1->cr4_addr;
        case IME_OUTPUT_PPB_4:
            _ImeSize.p1_out_addr.oDMA3.y_addr = pIME_Para1->y3_addr;
            _ImeSize.p1_out_addr.oDMA3.cb_addr = pIME_Para1->cb3_addr;
            _ImeSize.p1_out_addr.oDMA3.cr_addr = pIME_Para1->cr3_addr;
        case IME_OUTPUT_PPB_3:
            _ImeSize.p1_out_addr.oDMA2.y_addr = pIME_Para1->y2_addr;
            _ImeSize.p1_out_addr.oDMA2.cb_addr = pIME_Para1->cb2_addr;
            _ImeSize.p1_out_addr.oDMA2.cr_addr = pIME_Para1->cr2_addr;
        case IME_OUTPUT_PPB_2:
            _ImeSize.p1_out_addr.oDMA1.y_addr = pIME_Para1->y1_addr;
            _ImeSize.p1_out_addr.oDMA1.cb_addr = pIME_Para1->cb1_addr;
            _ImeSize.p1_out_addr.oDMA1.cr_addr = pIME_Para1->cr1_addr;
        case IME_OUTPUT_PPB_1:
            _ImeSize.p1_out_addr.oDMA0.y_addr = pIME_Para1->y0_addr;
            _ImeSize.p1_out_addr.oDMA0.cb_addr = pIME_Para1->cb0_addr;
            _ImeSize.p1_out_addr.oDMA0.cr_addr = pIME_Para1->cr0_addr;
            break;
    }
    _ImeSize.in_size.h_size = pIME_Para1->in_h;
    _ImeSize.in_size.v_size = pIME_Para1->in_v;
    _ImeSize.line_ofs.in_lineofs.LineOffsetY = pIME_Para1->in_lineoffset;
    _ImeSize.line_ofs.in_lineofs.LineOffsetCb = pIME_Para1->in_Cb_lineoffset;
    _ImeSize.line_ofs.in_lineofs.LineOffsetCr = pIME_Para1->in_Cr_lineoffset;

    _ImeSize.p1_out_size.h_size = pIME_Para1->out_h;
    _ImeSize.p1_out_size.v_size = pIME_Para1->out_v;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetY = pIME_Para1->out_lineoffset;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetCb = pIME_Para1->out_Cb_lineoffset;
    _ImeSize.line_ofs.p1_out_lineofs.LineOffsetCr = pIME_Para1->out_Cr_lineoffset;

    _ImeStripeInfo.StripeCalMode = IME_STRIPE_AUTO_MODE;

    ime_setUserStripeInfo(&_ImeStripeInfo);
    ime_chgParam((void*)&_ImeSize,IME_CHG_IO_SIZE);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------

void ipe_ime_set2dzoom(void)
{
    IPEModeInfo.SIZEINFO.INSIZE.hsize = GetIPPSizeInfor(_IpeOutHsize)+IPE_HSIZE_IODIFF;
    IPEModeInfo.SIZEINFO.INSIZE.vsize = GetIPPSizeInfor(_IpeOutVsize);
    IPEModeInfo.SIZEINFO.CfaPat = IPE_Para.CfaPat;
    IPEModeInfo.SIZEINFO.StripeMode = IPE_Para.StripeMode;
    IPEModeInfo.SIZEINFO.LofsIn = GetIPPSizeInfor(_SieOutLineOffset);

    //ime cfg
    if(gAlgMsg.Alg_Mode==MODE_Video_Recording)
    {
        Ime_ChgIOSize(&IME_D2_Para,&IME_D2_Para_ext);
    }
    else
    {
        Ime_ChgIOSize(&IME_D1_Para,&IME_D1_Para_ext);
    }
    //debug_msg(" n l m = %d %d %d\r\n",pimeStripe->Hstripe.n,pimeStripe->Hstripe.l,pimeStripe->Hstripe.m);

    IPEModeInfo.SIZEINFO.HSTRIPE.n = IPE_Para.HStripe.stripe.n;
    IPEModeInfo.SIZEINFO.HSTRIPE.l = IPE_Para.HStripe.stripe.l;
    IPEModeInfo.SIZEINFO.HSTRIPE.m = IPE_Para.HStripe.stripe.m;
    IPEModeInfo.SIZEINFO.HSTRIPE.olap = IPE_Para.HStripe.olap;
    IPEModeInfo.SIZEINFO.VSTRIPE.n = IPE_Para.VStripe.stripe.n;
    IPEModeInfo.SIZEINFO.VSTRIPE.l = IPE_Para.VStripe.stripe.l;
    IPEModeInfo.SIZEINFO.VSTRIPE.m = IPE_Para.VStripe.stripe.m;
    IPEModeInfo.SIZEINFO.VSTRIPE.olap = IPE_Para.VStripe.olap;

    //ipe cfg
    ipe_setSliceH(IPE_Para.IpeSliceH);
    ipe_changeSize(&IPEModeInfo.SIZEINFO);
    ipe_changeInput(&IPEModeInfo.ININFO);
}

static __inline void SIEReset(void)
{
    pll_enableSystemReset(SIE_RSTN);
    pll_disableSystemReset(SIE_RSTN);
    sie_reset(ENABLE);
    sie_reset(DISABLE);
}

static __inline void IPEReset(void)
{
    pll_enableSystemReset(IPE_RSTN);
    pll_disableSystemReset(IPE_RSTN);
    ipe_reset(ENABLE);
    ipe_reset(DISABLE);
}

static __inline void IMEReset(void)
{
    pll_enableSystemReset(IME_RSTN);
    pll_disableSystemReset(IME_RSTN);
    ime_setReset();
}