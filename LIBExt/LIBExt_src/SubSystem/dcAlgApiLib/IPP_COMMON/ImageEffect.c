/**
   Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.
    @file       ImageEffect.c
    @ingroup
    @brief      Image effect API for Multi-Preview/Preview/Capture
    @note       Nothing.
    @date       2009/06/30
*/

#include "ImageEffectParam.h"
#include "ImageEffect.h"
#include "OSCommon.h"
#include "Ime_lib.h"
#include "ErrorNo.h"
#include "Utility.h"
#include "Grph.h"
#include "fde_lib.h"
#include "pll.h"
#include "JpgDec.h"


ImgEffectInfo           gImgEffectInfo;
ImgEffectParam          gImgEffectParam;
ImgEffectShadingJPG     gImgEffectShadingJPG;
UINT32                  gAwbChCnt=IE_CH_1;
UINT32                  gAwbChMark=IE_CH_1;

ER IE_EnableImgEffect(ImgEffectSetting* IE_Setting)
{
    ER  Error;

    if(IE_Setting->Type == IE_TYPE_CAPTURE)
        Error = IE_InitCaptureImgEffect(IE_Setting);
    else if(IE_Setting->Type == IE_TYPE_SINGLE)
        Error = IE_InitSingleImgEffect(IE_Setting);
    else //if(IE_Setting->Type == IE_TYPE_MULTI)
        Error = IE_InitMultiImgEffect(IE_Setting);

    return  Error;
}

ER IE_InitCaptureImgEffect(ImgEffectSetting* IE_Setting)
{
    if(IE_Setting->Mode != IE_MODE_ARF)
    {
        debug_err(("ImgEffect capture only support ARF mode\r\n"));
        return E_NOSPT;
    }

    //initial gImgEffectInfo parameter
    gImgEffectInfo.Type = IE_Setting->Type;
    gImgEffectInfo.Mode = IE_Setting->Mode;
    gImgEffectInfo.CapStatus = FALSE;
    gImgEffectInfo.CaptureCH.Index = IE_Setting->CaptureChannelIndex;

    return E_OK;
}

ER IE_InitSingleImgEffect(ImgEffectSetting* IE_Setting)
{
    UINT32  SourceAddr, DisplayAddr, ChannelAddr, EffectAddr;
    UINT32  SourceFBSize, DisplayFBSize, ChannelFBSize, EffectFBSize;
    UINT32  DisplayFBHsize, DisplayFBVsize, DisplayFBLineoffset;
    UINT32  SourceFBHsize, SourceFBVsize, SourceFBLineoffset;

    if(IE_Setting->Mode != IE_MODE_ARF)
    {
        debug_err(("ImgEffect single view only support ARF mode\r\n"));
        return E_NOSPT;
    }

    //initial gImgEffectInfo parameter
    gImgEffectInfo.Type = IE_Setting->Type;
    gImgEffectInfo.Mode = IE_Setting->Mode;
    gImgEffectInfo.PrvStatus = FALSE;
    gImgEffectInfo.SingleCH.Index = IE_Setting->SingleChannelIndex;
    DisplayFBHsize = IE_Setting->DisplayFBHsize;
    DisplayFBVsize = IE_Setting->DisplayFBVsize;
    DisplayFBLineoffset= IE_Setting->DisplayFBLineoffset;
    SourceFBHsize = IE_Setting->SourceFBHsize;
    SourceFBVsize = IE_Setting->SourceFBVsize;
    SourceFBLineoffset= IE_Setting->SourceFBLineoffset;

    gImgEffectInfo.ChannelFBActID = FB_ID_1;
    gImgEffectInfo.ChannelFBRdyID = FB_ID_1;
    gImgEffectInfo.ChannelFBLock[FB_ID_0] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_1] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_2] = FALSE;

    //initial gImgEffectInfo display frame buffer
    DisplayAddr = ippbuf_GetBuf(IPPBUF_PRV_IEDISPLAY_BUF);
    DisplayFBSize = DisplayFBVsize * DisplayFBLineoffset;
    gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0]  = DisplayAddr;
    gImgEffectInfo.DisplayFBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize;
    gImgEffectInfo.DisplayFBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize * 3/2;
    gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize * 2;
    gImgEffectInfo.DisplayFBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_1] + DisplayFBSize;
    gImgEffectInfo.DisplayFBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_1] + DisplayFBSize * 3/2;
    gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize * 4;
    gImgEffectInfo.DisplayFBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_2] + DisplayFBSize;
    gImgEffectInfo.DisplayFBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_2] + DisplayFBSize * 3/2;
    gImgEffectInfo.DisplayFBInfo.HSize = DisplayFBHsize;
    gImgEffectInfo.DisplayFBInfo.VSize = DisplayFBVsize;
    gImgEffectInfo.DisplayFBInfo.LineOffset = DisplayFBLineoffset;
    gImgEffectInfo.DisplayFBInfo.Format= IME_OUTPUT_YCC_422_COS;

    //check IPPBUF_PRV_IEDISPLAY_BUF buffer size
    if( DisplayFBSize*6 > IE_DISPLAY_MAX_SIZE)
    {
        debug_err(("Prv IPPBUF_PRV_IEDISPLAY_BUF is not enough\r\n"));
        debug_err(("IPPBUF_PRV_IEDISPLAY_BUF=0x%x\r\n",IE_DISPLAY_MAX_SIZE));
        debug_err(("IEDISPLAY_BUF=0x%x\r\n",DisplayFBSize*6));

        return E_NOMEM;
    }

    //initial gImgEffectInfo source frame buffer
    SourceAddr = ippbuf_GetBuf(IPPBUF_PRV_IE_BUF);
    SourceFBSize = SourceFBLineoffset * SourceFBVsize;
    gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0]  = SourceAddr;
    gImgEffectInfo.SourceFBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize;
    gImgEffectInfo.SourceFBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize * 3/2;
    gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize * 2;
    gImgEffectInfo.SourceFBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_1] + SourceFBSize;
    gImgEffectInfo.SourceFBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_1] + SourceFBSize * 3/2;
    gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize * 4;
    gImgEffectInfo.SourceFBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_2] + SourceFBSize;
    gImgEffectInfo.SourceFBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_2] + SourceFBSize * 3/2;
    gImgEffectInfo.SourceFBInfo.HSize = SourceFBHsize;
    gImgEffectInfo.SourceFBInfo.VSize = SourceFBVsize;
    gImgEffectInfo.SourceFBInfo.LineOffset= SourceFBLineoffset;
    gImgEffectInfo.SourceFBInfo.Format= IME_OUTPUT_YCC_422_COS;

    //initial gImgEffectInfo single view channel frame buffer
    ChannelAddr = SourceAddr + SourceFBSize * 6;
    ChannelFBSize = SourceFBSize;
    gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_0]  = ChannelAddr;
    gImgEffectInfo.SingleCH.FBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_0] + ChannelFBSize;
    gImgEffectInfo.SingleCH.FBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 3/2;
    gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 2;
    gImgEffectInfo.SingleCH.FBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_1] + ChannelFBSize;
    gImgEffectInfo.SingleCH.FBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_1] + ChannelFBSize * 3/2;
    gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 4;
    gImgEffectInfo.SingleCH.FBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_2] + ChannelFBSize;
    gImgEffectInfo.SingleCH.FBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.SingleCH.FBInfo.YAddr[FB_ID_2] + ChannelFBSize * 3/2;
    gImgEffectInfo.SingleCH.FBInfo.HSize = SourceFBHsize;
    gImgEffectInfo.SingleCH.FBInfo.VSize = SourceFBVsize;
    gImgEffectInfo.SingleCH.FBInfo.LineOffset= SourceFBLineoffset;
    gImgEffectInfo.SingleCH.FBInfo.Format= IME_OUTPUT_YCC_422_COS;

    //initial gImgEffectInfo channel parameters
    EffectAddr = ChannelAddr + ChannelFBSize * 6;
    EffectFBSize = 0;
    EffectFBSize = IE_InitSingleARF(EffectAddr);
    EffectAddr += EffectFBSize;

    //check IPPBUF_PRV_IE_BUF buffer size
    if((EffectAddr-ippbuf_GetBuf(IPPBUF_PRV_IE_BUF)) > IE_PRV_MAX_SIZE)
    {
        debug_err(("Prv IPPBUF_PRV_IE_BUF is not enough\r\n"));
        debug_err(("IPPBUF_PRV_IE_BUF=0x%x\r\n",IE_PRV_MAX_SIZE));
        debug_err(("IE_BUF=0x%x\r\n",(EffectAddr-ippbuf_GetBuf(IPPBUF_PRV_IE_BUF))));

        return E_NOMEM;
    }
    else
        return E_OK;
}


ER IE_InitMultiImgEffect(ImgEffectSetting* IE_Setting)
{
    UINT32  SourceAddr, DisplayAddr, ChannelAddr, EffectAddr;
    UINT32  SourceFBSize, DisplayFBSize, ChannelFBSize, EffectFBSize;
    UINT32  DisplayFBHsize, DisplayFBVsize, DisplayFBLineoffset;
    UINT32  SourceFBHsize, SourceFBVsize, SourceFBLineoffset;

    //initial gImgEffectInfo parameter
    gImgEffectInfo.Type = IE_Setting->Type;
    gImgEffectInfo.Mode = IE_Setting->Mode;
    gImgEffectInfo.PrvStatus = FALSE;
    gImgEffectInfo.MultiCH[IE_CH_1].Index = IE_Setting->MultiChannelIndex[IE_CH_1];
    gImgEffectInfo.MultiCH[IE_CH_2].Index = IE_Setting->MultiChannelIndex[IE_CH_2];
    gImgEffectInfo.MultiCH[IE_CH_3].Index = IE_Setting->MultiChannelIndex[IE_CH_3];
    gImgEffectInfo.MultiCH[IE_CH_4].Index = IE_Setting->MultiChannelIndex[IE_CH_4];

    if( (gImgEffectInfo.Mode == IE_MODE_EVARF) || (gImgEffectInfo.Mode == IE_MODE_AWBARF) )
    {
        gImgEffectInfo.MultiCHARFSubIndex = IE_Setting->MultiChannelARFSubIndex;

        if( (gImgEffectInfo.MultiCHARFSubIndex >= IE_INDEX_ARF_MAX) || (gImgEffectInfo.MultiCHARFSubIndex < IE_INDEX_ARF_MIN) )
        {
            debug_err(("ImgEffect multiple view sub-effect only support ARF mode\r\n"));
            return E_NOSPT;
        }
    }
    else
        gImgEffectInfo.MultiCHARFSubIndex = IE_INDEX_NULL;

    DisplayFBHsize = IE_Setting->DisplayFBHsize;
    DisplayFBVsize = IE_Setting->DisplayFBVsize;
    DisplayFBLineoffset= IE_Setting->DisplayFBLineoffset;
    SourceFBHsize = IE_Setting->SourceFBHsize;
    SourceFBVsize = IE_Setting->SourceFBVsize;
    SourceFBLineoffset= IE_Setting->SourceFBLineoffset;

    gImgEffectInfo.ChannelFBActID = FB_ID_1;
    gImgEffectInfo.ChannelFBRdyID = FB_ID_1;
    gImgEffectInfo.ChannelFBLock[FB_ID_0] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_1] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_2] = FALSE;

    //initial gImgEffectInfo display frame buffer
    DisplayAddr = ippbuf_GetBuf(IPPBUF_PRV_IEDISPLAY_BUF);
    DisplayFBSize = DisplayFBVsize * DisplayFBLineoffset;
    gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0]  = DisplayAddr;
    gImgEffectInfo.DisplayFBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize;
    gImgEffectInfo.DisplayFBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize * 3/2;
    gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize * 2;
    gImgEffectInfo.DisplayFBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_1] + DisplayFBSize;
    gImgEffectInfo.DisplayFBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_1] + DisplayFBSize * 3/2;
    gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_0] + DisplayFBSize * 4;
    gImgEffectInfo.DisplayFBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_2] + DisplayFBSize;
    gImgEffectInfo.DisplayFBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.DisplayFBInfo.YAddr[FB_ID_2] + DisplayFBSize * 3/2;
    gImgEffectInfo.DisplayFBInfo.HSize = DisplayFBHsize;
    gImgEffectInfo.DisplayFBInfo.VSize = DisplayFBVsize;
    gImgEffectInfo.DisplayFBInfo.LineOffset = DisplayFBLineoffset;
    gImgEffectInfo.DisplayFBInfo.Format= IME_OUTPUT_YCC_422_COS;

    //check IPPBUF_PRV_IEDISPLAY_BUF buffer size
    if( DisplayFBSize*6 > IE_DISPLAY_MAX_SIZE)
    {
        debug_err(("Prv IPPBUF_PRV_IEDISPLAY_BUF is not enough\r\n"));
        debug_err(("IPPBUF_PRV_IEDISPLAY_BUF=0x%x\r\n",IE_DISPLAY_MAX_SIZE));
        debug_err(("IEDISPLAY_BUF=0x%x\r\n",DisplayFBSize*6));

        return E_NOMEM;
    }

    //initial gImgEffectInfo source frame buffer
    SourceAddr = ippbuf_GetBuf(IPPBUF_PRV_IE_BUF);
    SourceFBSize = SourceFBLineoffset * SourceFBVsize;
    gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0]  = SourceAddr;
    gImgEffectInfo.SourceFBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize;
    gImgEffectInfo.SourceFBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize * 3/2;
    gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize * 2;
    gImgEffectInfo.SourceFBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_1] + SourceFBSize;
    gImgEffectInfo.SourceFBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_1] + SourceFBSize * 3/2;
    gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_0] + SourceFBSize * 4;
    gImgEffectInfo.SourceFBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_2] + SourceFBSize;
    gImgEffectInfo.SourceFBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.SourceFBInfo.YAddr[FB_ID_2] + SourceFBSize * 3/2;
    gImgEffectInfo.SourceFBInfo.HSize = SourceFBHsize;
    gImgEffectInfo.SourceFBInfo.VSize = SourceFBVsize;
    gImgEffectInfo.SourceFBInfo.LineOffset= SourceFBLineoffset;
    gImgEffectInfo.SourceFBInfo.Format= IME_OUTPUT_YCC_422_COS;

    //initial gImgEffectInfo multiview channel frame buffer
    ChannelAddr = SourceAddr + SourceFBSize * 6;
    ChannelFBSize = SourceFBSize;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_0]  = ChannelAddr;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_0] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 2;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_1] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_1] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 4;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_2] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[FB_ID_2] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.HSize = SourceFBHsize;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.VSize = SourceFBVsize;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.LineOffset= SourceFBLineoffset;
    gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.Format= IME_OUTPUT_YCC_422_COS;

    ChannelAddr += ChannelFBSize * 6;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_0]  = ChannelAddr;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_0] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 2;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_1] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_1] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 4;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_2] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[FB_ID_2] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.HSize = SourceFBHsize;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.VSize = SourceFBVsize;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.LineOffset= SourceFBLineoffset;
    gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.Format= IME_OUTPUT_YCC_422_COS;

    ChannelAddr += ChannelFBSize * 6;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_0]  = ChannelAddr;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_0] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 2;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_1] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_1] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 4;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_2] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[FB_ID_2] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.HSize = SourceFBHsize;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.VSize = SourceFBVsize;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.LineOffset= SourceFBLineoffset;
    gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.Format= IME_OUTPUT_YCC_422_COS;

    ChannelAddr += ChannelFBSize * 6;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_0]  = ChannelAddr;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.CbAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_0] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.CrAddr[FB_ID_0] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_1]  = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 2;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.CbAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_1] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.CrAddr[FB_ID_1] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_1] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_2]  = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_0] + ChannelFBSize * 4;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.CbAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_2] + ChannelFBSize;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.CrAddr[FB_ID_2] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[FB_ID_2] + ChannelFBSize * 3/2;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.HSize = SourceFBHsize;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.VSize = SourceFBVsize;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.LineOffset= SourceFBLineoffset;
    gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.Format= IME_OUTPUT_YCC_422_COS;
    ChannelAddr += ChannelFBSize * 6;

    //initial gImgEffectInfo channel parameters
    EffectAddr = ChannelAddr;
    EffectFBSize = 0;
    switch (gImgEffectInfo.Mode)
    {
        case IE_MODE_EV:
            EffectFBSize = IE_InitMultiEV(EffectAddr);
        break;
        case IE_MODE_AWB:
            EffectFBSize = IE_InitMultiAWB(EffectAddr);
        break;
        case IE_MODE_ARF:
            EffectFBSize = IE_InitMultiARF(EffectAddr);
        break;
        case IE_MODE_EVARF:
            EffectFBSize = IE_InitMultiEVARF(EffectAddr);
        break;
        case IE_MODE_AWBARF:
            EffectFBSize = IE_InitMultiAWBARF(EffectAddr);
        break;
    }
    EffectAddr += EffectFBSize;

    //check IPPBUF_PRV_IE_BUF buffer size
    if((EffectAddr-ippbuf_GetBuf(IPPBUF_PRV_IE_BUF)) > IE_PRV_MAX_SIZE)
    {
        debug_err(("Prv IPPBUF_PRV_IE_BUF is not enough\r\n"));
        debug_err(("IPPBUF_PRV_IE_BUF=0x%x\r\n",IE_PRV_MAX_SIZE));
        debug_err(("IE_BUF=0x%x\r\n",(EffectAddr-ippbuf_GetBuf(IPPBUF_PRV_IE_BUF))));

        return E_NOMEM;
    }
    else
        return E_OK;
}

void IE_DisableImgEffect(void)
{
    gAwbChCnt=IE_CH_1;
}

void IE_SetPrvStatus(BOOL PrvStatus)
{
    gImgEffectInfo.PrvStatus = PrvStatus;
}

BOOL IE_GetPrvStatus(void)
{
    return  gImgEffectInfo.PrvStatus;
}

IE_MODE IE_GetPrvMode(void)
{
    return  gImgEffectInfo.Mode;
}

void IE_SetCapStatus(BOOL CapStatus)
{
    gImgEffectInfo.CapStatus = CapStatus;
}

BOOL IE_GetCapStatus(void)
{
    return  gImgEffectInfo.CapStatus;
}

IE_INDEX IE_GetCapIndex(void)
{
    return  gImgEffectInfo.CaptureCH.Index;
}

FB_ID IE_LockRdyFB(void)
{
    gImgEffectInfo.ChannelFBLock[FB_ID_0] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_1] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_2] = FALSE;
    gImgEffectInfo.ChannelFBLock[gImgEffectInfo.ChannelFBRdyID] = TRUE;
    //debug_err(("IE_LockRdyFB=%d\r\n",gImgEffectInfo.ChannelFBRdyID));

    return  gImgEffectInfo.ChannelFBRdyID;
}

void IE_UnLockRdyFB(void)
{
    gImgEffectInfo.ChannelFBLock[FB_ID_0] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_1] = FALSE;
    gImgEffectInfo.ChannelFBLock[FB_ID_2] = FALSE;
}

void IE_SetMultiChannelIndex(IE_CH Channel, IE_INDEX CH_Index)
{
    //debug_err(("CH=%d Index=%d\r\n",Channel, CH_Index));
    gImgEffectInfo.MultiCH[Channel].Index = CH_Index;
}

void IE_GetMultiChannelFBInfo(IE_CH Channel, FBInfo* FB_Info)
{
    *(FB_Info) = gImgEffectInfo.MultiCH[Channel].FBInfo;
}

void IE_GetSingleChannelFBInfo(FBInfo* FB_Info)
{
    *(FB_Info) = gImgEffectInfo.SingleCH.FBInfo;
}

void IE_GetDisplayFBInfo(FBInfo* FB_Info)
{
    *(FB_Info) = gImgEffectInfo.DisplayFBInfo;
}

void IE_PrvProcess(void)
{
    if(gImgEffectInfo.Type == IE_TYPE_SINGLE)
        IE_SinglePrvProcess();
    else//(gImgEffectInfo.View == IE_TYPE_MULTI)
        IE_MultiPrvProcess();
}

void IE_MultiPrvProcess(void)
{
    UINT32  ImeFBID;
    UINT32  ImeFBSize, ImeFBYOffset, ImeFBCOffset;
    UINT32  Ratio, SourceHSize, SourceVSize;
    ImageAlgInfo *AlgInfo;

    //debug_err(("ChannelFBActID=%d\r\n",gImgEffectInfo.ChannelFBActID));
    //debug_err(("ChannelFBRdyID=%d\r\n",gImgEffectInfo.ChannelFBRdyID));

    //Image ratio convert
    AlgInfo = UI_GetParameter();
    if (GetIPPSizeInfor(_SensorRatioIdx) != AlgInfo->ImageRatio)
    {
        Ratio = Get_HRatio(AlgInfo->ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
        SourceHSize = Adj_HSize(gImgEffectInfo.SourceFBInfo.HSize, Ratio, 16);
        Ratio = Get_VRatio(AlgInfo->ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
        SourceVSize = Adj_VSize(gImgEffectInfo.SourceFBInfo.VSize, Ratio, 4);

//KH mark
#if 0
        ImeFBID = ime_getPath1CurrentBufferIndex();
#endif
        ImeFBSize = gImgEffectInfo.SourceFBInfo.LineOffset * SourceVSize;
        ImeFBYOffset =  (gImgEffectInfo.SourceFBInfo.VSize - SourceVSize)/2 * gImgEffectInfo.SourceFBInfo.LineOffset +
                        (gImgEffectInfo.SourceFBInfo.HSize - SourceHSize)/2;
        ImeFBCOffset =  (gImgEffectInfo.SourceFBInfo.VSize - SourceVSize)/2 * gImgEffectInfo.SourceFBInfo.LineOffset/2 +
                        (gImgEffectInfo.SourceFBInfo.HSize/2 - SourceHSize/2)/2;
    }
    else
    {
        Ratio = 0;
        SourceHSize = gImgEffectInfo.SourceFBInfo.HSize;
        SourceVSize = gImgEffectInfo.SourceFBInfo.VSize;
//KH mark
#if 0
        ImeFBID = ime_getPath1CurrentBufferIndex();
#endif
        ImeFBSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;
        ImeFBYOffset = 0;
        ImeFBCOffset = 0;
    }

    //IME FB to Source FB
    hwmem_open();

    if( (gImgEffectInfo.Mode == IE_MODE_AWB) || (gImgEffectInfo.Mode == IE_MODE_AWBARF) )
    {
        switch (gAwbChCnt)
        {
            case IE_CH_1:
                gAwbChMark = IE_CH_3;
            break;

            case IE_CH_2:
                gAwbChMark = IE_CH_4;
            break;

            case IE_CH_3:
                gAwbChMark = IE_CH_1;
            break;

            case IE_CH_4:
                gAwbChMark = IE_CH_2;
            break;
        }
    }

//KH mark
#if 0
    hwmem_memcpy(   gImgEffectInfo.SourceFBInfo.YAddr[ImeFBID] + ImeFBYOffset,
                    ime_getOutputPath1Add(ImeFBID,IME_CHANNEL_Y),
                    ImeFBSize );

    hwmem_memcpy(   gImgEffectInfo.SourceFBInfo.CbAddr[ImeFBID] + ImeFBCOffset,
                    ime_getOutputPath1Add(ImeFBID,IME_CHANNEL_CB),
                    ImeFBSize/2 );

    hwmem_memcpy(   gImgEffectInfo.SourceFBInfo.CrAddr[ImeFBID] + ImeFBCOffset,
                    ime_getOutputPath1Add(ImeFBID,IME_CHANNEL_CR),
                    ImeFBSize/2 );
#endif
    hwmem_close();

    //Image effect multiple view process
    switch (gImgEffectInfo.Mode)
    {
        case IE_MODE_EV:
            IE_ProcessMultiEV(ImeFBID, gImgEffectInfo.ChannelFBActID);
        break;
        case IE_MODE_AWB:
            IE_ProcessMultiAWB(ImeFBID, gImgEffectInfo.ChannelFBActID);
        break;
        case IE_MODE_ARF:
            IE_ProcessMultiARF(ImeFBID, gImgEffectInfo.ChannelFBActID);
        break;
        case IE_MODE_EVARF:
            IE_ProcessMultiEVARF(ImeFBID, gImgEffectInfo.ChannelFBActID);
        break;
        case IE_MODE_AWBARF:
            IE_ProcessMultiAWBARF(ImeFBID, gImgEffectInfo.ChannelFBActID);
        break;
    }

    //Image ratio convert
    if (GetIPPSizeInfor(_SensorRatioIdx) != AlgInfo->ImageRatio)
    {
        IE_CH Channel;

        for(Channel=IE_CH_1; Channel<IE_CH_MAX; Channel++)
        {
            if( (gImgEffectInfo.MultiCH[Channel].Index == IE_INDEX_ARF_Sketch) || (gImgEffectInfo.MultiCHARFSubIndex == IE_INDEX_ARF_Sketch) )
            {
                hwmem_open();
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[gImgEffectInfo.ChannelFBActID], 0x00, ImeFBYOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[gImgEffectInfo.ChannelFBActID] + ImeFBYOffset + ImeFBSize-gImgEffectInfo.SourceFBInfo.LineOffset*2, 0x00, ImeFBYOffset+gImgEffectInfo.SourceFBInfo.LineOffset*2);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2-gImgEffectInfo.SourceFBInfo.LineOffset/2*2, 0x80, ImeFBCOffset+gImgEffectInfo.SourceFBInfo.LineOffset/2*2);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2-gImgEffectInfo.SourceFBInfo.LineOffset/2*2, 0x80, ImeFBCOffset+gImgEffectInfo.SourceFBInfo.LineOffset/2*2);
                hwmem_close();
            }
            else
            {
                hwmem_open();
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[gImgEffectInfo.ChannelFBActID], 0x00, ImeFBYOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[gImgEffectInfo.ChannelFBActID] + ImeFBYOffset + ImeFBSize, 0x00, ImeFBYOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2, 0x80, ImeFBCOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
                hwmem_memset(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2, 0x80, ImeFBCOffset);
                hwmem_close();
            }
        }
    }

    gImgEffectInfo.ChannelFBRdyID = gImgEffectInfo.ChannelFBActID;
    gImgEffectInfo.ChannelFBActID++;

    if (gImgEffectInfo.ChannelFBActID >= FB_ID_MAX)
        gImgEffectInfo.ChannelFBActID = FB_ID_0;

    if (gImgEffectInfo.ChannelFBLock[gImgEffectInfo.ChannelFBActID] == TRUE)
    {
        gImgEffectInfo.ChannelFBActID++;
        if (gImgEffectInfo.ChannelFBActID >= FB_ID_MAX)
            gImgEffectInfo.ChannelFBActID = FB_ID_0;
    }

    //Update next ID FB for all channels AWB/AWBARF mode
    if( (gImgEffectInfo.Mode == IE_MODE_AWB) || (gImgEffectInfo.Mode == IE_MODE_AWBARF) )
    {
        IE_CH Channel;
        UINT32 CHFBSize=gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;

        for(Channel=IE_CH_1; Channel<IE_CH_MAX; Channel++)
        {
            hwmem_open();
            hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[gImgEffectInfo.ChannelFBActID],
                            gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[gImgEffectInfo.ChannelFBRdyID],
                            CHFBSize );

            hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID],
                            gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[gImgEffectInfo.ChannelFBRdyID],
                            CHFBSize/2 );

            hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID],
                            gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[gImgEffectInfo.ChannelFBRdyID],
                            CHFBSize/2 );
            hwmem_close();
        }
    }
}

void IE_SinglePrvProcess(void)
{
    UINT32  ImeFBID;
    UINT32  ImeFBSize, ImeFBYOffset, ImeFBCOffset;
    ImageAlgInfo *AlgInfo;
    UINT32 Ratio, SourceHSize, SourceVSize;

    //debug_err(("ChannelFBActID=%d\r\n",gImgEffectInfo.ChannelFBActID));
    //debug_err(("ChannelFBRdyID=%d\r\n",gImgEffectInfo.ChannelFBRdyID));

    //Image ratio convert
    AlgInfo = UI_GetParameter();
    if (GetIPPSizeInfor(_SensorRatioIdx) != AlgInfo->ImageRatio)
    {
        Ratio = Get_HRatio(AlgInfo->ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
        SourceHSize = Adj_HSize(gImgEffectInfo.SourceFBInfo.HSize, Ratio, 16);
        Ratio = Get_VRatio(AlgInfo->ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
        SourceVSize = Adj_VSize(gImgEffectInfo.SourceFBInfo.VSize, Ratio, 4);

//KH mark
#if 0
        ImeFBID = ime_getPath1CurrentBufferIndex();
#endif
        ImeFBSize = gImgEffectInfo.SourceFBInfo.LineOffset * SourceVSize;
        ImeFBYOffset =  (gImgEffectInfo.SourceFBInfo.VSize - SourceVSize)/2 * gImgEffectInfo.SourceFBInfo.LineOffset +
                        (gImgEffectInfo.SourceFBInfo.HSize - SourceHSize)/2;
        ImeFBCOffset =  (gImgEffectInfo.SourceFBInfo.VSize - SourceVSize)/2 * gImgEffectInfo.SourceFBInfo.LineOffset/2 +
                        (gImgEffectInfo.SourceFBInfo.HSize/2 - SourceHSize/2)/2;
    }
    else
    {
        Ratio = 0;
        SourceHSize = gImgEffectInfo.SourceFBInfo.HSize;
        SourceVSize = gImgEffectInfo.SourceFBInfo.VSize;
//KH mark
#if 0
        ImeFBID = ime_getPath1CurrentBufferIndex();
#endif
        ImeFBSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;
        ImeFBYOffset = 0;
        ImeFBCOffset = 0;
    }

    //IME FB to Source FB
    hwmem_open();
//KH mark
#if 0
    hwmem_memcpy(   gImgEffectInfo.SourceFBInfo.YAddr[ImeFBID] + ImeFBYOffset,
                    ime_getOutputPath1Add(ImeFBID,IME_CHANNEL_Y),
                    ImeFBSize );

    hwmem_memcpy(   gImgEffectInfo.SourceFBInfo.CbAddr[ImeFBID] + ImeFBCOffset,
                    ime_getOutputPath1Add(ImeFBID,IME_CHANNEL_CB),
                    ImeFBSize/2 );

    hwmem_memcpy(   gImgEffectInfo.SourceFBInfo.CrAddr[ImeFBID] + ImeFBCOffset,
                    ime_getOutputPath1Add(ImeFBID,IME_CHANNEL_CR),
                    ImeFBSize/2 );
#endif
    hwmem_close();

    //Image effect single view process
    IE_ProcessSingleARF(ImeFBID, gImgEffectInfo.ChannelFBActID);

    //Image ratio convert
    if (GetIPPSizeInfor(_SensorRatioIdx) != AlgInfo->ImageRatio)
    {
        //#NT#2009/11/23#YC Peng -begin
        //#modify ARF sketch bug
        if(gImgEffectInfo.SingleCH.Index == IE_INDEX_ARF_Sketch)
        {
            hwmem_open();
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.YAddr[gImgEffectInfo.ChannelFBActID], 0x00, ImeFBYOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.YAddr[gImgEffectInfo.ChannelFBActID] + ImeFBYOffset + ImeFBSize-gImgEffectInfo.SourceFBInfo.LineOffset*2, 0x00, ImeFBYOffset+gImgEffectInfo.SourceFBInfo.LineOffset*2);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2-gImgEffectInfo.SourceFBInfo.LineOffset*2/2, 0x80, ImeFBCOffset+gImgEffectInfo.SourceFBInfo.LineOffset*2/2);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2-gImgEffectInfo.SourceFBInfo.LineOffset*2/2, 0x80, ImeFBCOffset+gImgEffectInfo.SourceFBInfo.LineOffset*2/2);
            hwmem_close();
        }
        else
        {
            hwmem_open();
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.YAddr[gImgEffectInfo.ChannelFBActID], 0x00, ImeFBYOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.YAddr[gImgEffectInfo.ChannelFBActID] + ImeFBYOffset + ImeFBSize, 0x00, ImeFBYOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CbAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2, 0x80, ImeFBCOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID], 0x80, ImeFBCOffset);
            hwmem_memset(gImgEffectInfo.SingleCH.FBInfo.CrAddr[gImgEffectInfo.ChannelFBActID] + ImeFBCOffset + ImeFBSize/2, 0x80, ImeFBCOffset);
            hwmem_close();
        }
        //#NT#2009/11/23#YC Peng -end
    }

    gImgEffectInfo.ChannelFBRdyID = gImgEffectInfo.ChannelFBActID;
    gImgEffectInfo.ChannelFBActID++;

    if (gImgEffectInfo.ChannelFBActID >= FB_ID_MAX)
        gImgEffectInfo.ChannelFBActID = FB_ID_0;

    if (gImgEffectInfo.ChannelFBLock[gImgEffectInfo.ChannelFBActID] == TRUE)
    {
        gImgEffectInfo.ChannelFBActID++;
        if (gImgEffectInfo.ChannelFBActID >= FB_ID_MAX)
            gImgEffectInfo.ChannelFBActID = FB_ID_0;
    }
}

UINT32 IE_InitMultiEV(UINT32 BufAddr)
{
    UINT32  TotalBufSize=0;
    UINT32  i;

    //Init IE_MODE_EV (IE_INDEX_EV_N6 ~ IE_INDEX_EV_P6)
    for(i=0;i<IE_INDEX_EV_TOTAL;i++)
        gImgEffectParam.EV[i].GammaLUT = IE_MODE_EV_GammaLUT[i];

    return  TotalBufSize;
}

UINT32 IE_InitMultiEVARF(UINT32 BufAddr)
{
    UINT32  TotalBufSize=0;
    UINT32  BufSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;
    UINT32  Index, i;

    //Init IE_MODE_EV (IE_INDEX_EV_N6 ~ IE_INDEX_EV_P6)
    for(i=0;i<IE_INDEX_EV_TOTAL;i++)
        gImgEffectParam.EV[i].GammaLUT = IE_MODE_EV_GammaLUT[i];

    switch (gImgEffectInfo.MultiCHARFSubIndex)
    {
        case IE_INDEX_ARF_PopArt:
        {
            Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];

            //memory allocate
            gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN] = BufAddr;
            gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN] = BufAddr + BufSize/2;
            gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 2;
            gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 3;
            BufAddr += BufSize/2 * 4;
            TotalBufSize += BufSize/2 * 4;

            //CbCr gain
            gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN] = IE_MODE_ARF_CbOperator[Index][YCC_OP_GAIN];
            gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN] = IE_MODE_ARF_CrOperator[Index][YCC_OP_GAIN];

            //CbCr offset
            gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CbOperator[Index][YCC_OP_OFFSET];
            gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CrOperator[Index][YCC_OP_OFFSET];

            hwmem_open();
            hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET], BufSize);
            hwmem_close();
        }
        break;

        case IE_INDEX_ARF_ToyPhoto:
        {
            JpgScaleUpDown_t JpgScale;

            Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];

            //memory allocate
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr;
            BufAddr += BufSize;
            TotalBufSize += BufSize;

            //Shading Img
            JpgScale.ScaleLevel  = 0;
            JpgScale.src = (UINT8*)IE_MODE_ARF_ShadingImg;
            JpgScale.dst = (UINT8*)gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
            JpgScale.InputWidth  = IE_MODE_ARF_ShadingImgHsize;
            JpgScale.InputHeight = IE_MODE_ARF_ShadingImgVsize;
            JpgScale.InputOffset = IE_MODE_ARF_ShadingImgHsize;
            JpgScale.OutputWidth = gImgEffectInfo.SourceFBInfo.HSize;
            JpgScale.OutputHeight= gImgEffectInfo.SourceFBInfo.VSize;
            JpgScale.OutputOffset= gImgEffectInfo.SourceFBInfo.LineOffset;
            JpgScaleUpDown(&JpgScale);
        }
        break;
        case IE_INDEX_ARF_FishEye:
        {
            Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];
        }
        break;
        case IE_INDEX_ARF_Sketch:
        {
            Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

            //Y Edge Gain/Offse/MapInit
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN] = IE_MODE_ARF_YOperator[Index][YCC_OP_GAIN];
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET] = IE_MODE_ARF_YOperator[Index][YCC_OP_OFFSET];
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP] = IE_MODE_ARF_YOperator[Index][YCC_OP_MAP];

            //memory allocate
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN] = BufAddr;
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET] = BufAddr + BufSize;
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr + BufSize * 2;
            BufAddr += BufSize * 3;
            TotalBufSize += BufSize * 3;

            hwmem_open();
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP], gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP], BufSize);
            hwmem_close();
        }
        break;
    }

    return  TotalBufSize;
}

UINT32 IE_InitMultiAWB(UINT32 BufAddr)
{
    UINT32  TotalBufSize=0;

    return  TotalBufSize;
}

UINT32 IE_InitMultiAWBARF(UINT32 BufAddr)
{
    UINT32  TotalBufSize=0;
    UINT32  BufSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;
    UINT32  Index;

    switch (gImgEffectInfo.MultiCHARFSubIndex)
    {
        case IE_INDEX_ARF_PopArt:
        {
            Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];

            //memory allocate
            gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN] = BufAddr;
            gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN] = BufAddr + BufSize/2;
            gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 2;
            gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 3;
            BufAddr += BufSize/2 * 4;
            TotalBufSize += BufSize/2 * 4;

            //CbCr gain
            gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN] = IE_MODE_ARF_CbOperator[Index][YCC_OP_GAIN];
            gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN] = IE_MODE_ARF_CrOperator[Index][YCC_OP_GAIN];

            //CbCr offset
            gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CbOperator[Index][YCC_OP_OFFSET];
            gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CrOperator[Index][YCC_OP_OFFSET];

            hwmem_open();
            hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET], BufSize);
            hwmem_close();
        }
        break;

        case IE_INDEX_ARF_ToyPhoto:
        {
            JpgScaleUpDown_t JpgScale;

            Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];

            //memory allocate
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr;
            BufAddr += BufSize;
            TotalBufSize += BufSize;

            //Shading Img
            JpgScale.ScaleLevel  = 0;
            JpgScale.src = (UINT8*)IE_MODE_ARF_ShadingImg;
            JpgScale.dst = (UINT8*)gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
            JpgScale.InputWidth  = IE_MODE_ARF_ShadingImgHsize;
            JpgScale.InputHeight = IE_MODE_ARF_ShadingImgVsize;
            JpgScale.InputOffset = IE_MODE_ARF_ShadingImgHsize;
            JpgScale.OutputWidth = gImgEffectInfo.SourceFBInfo.HSize;
            JpgScale.OutputHeight= gImgEffectInfo.SourceFBInfo.VSize;
            JpgScale.OutputOffset= gImgEffectInfo.SourceFBInfo.LineOffset;
            JpgScaleUpDown(&JpgScale);
        }
        break;
        case IE_INDEX_ARF_FishEye:
        {
            Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];
        }
        break;
        case IE_INDEX_ARF_Sketch:
        {
            Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

            //Y Edge Gain/Offse/MapInit
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN] = IE_MODE_ARF_YOperator[Index][YCC_OP_GAIN];
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET] = IE_MODE_ARF_YOperator[Index][YCC_OP_OFFSET];
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP] = IE_MODE_ARF_YOperator[Index][YCC_OP_MAP];

            //memory allocate
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN] = BufAddr;
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET] = BufAddr + BufSize;
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr + BufSize * 2;
            BufAddr += BufSize * 3;
            TotalBufSize += BufSize * 3;

            hwmem_open();
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP], gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP], BufSize);
            hwmem_close();
        }
        break;
    }

    return  TotalBufSize;
}

UINT32 IE_InitMultiARF(UINT32 BufAddr)
{
    UINT32  TotalBufSize = 0;
    UINT32  BufSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;
    UINT32  i, Index;

    //Init IE_MODE_ARF
    for(i=0;i<IE_INDEX_ARF_TOTAL;i++)
        gImgEffectParam.ARF[i].GammaLUT = IE_MODE_ARF_GammaLUT[i];

    //IE_INDEX_ARF_PopArt
    {
        Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

        //memory allocate
        gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN] = BufAddr;
        gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN] = BufAddr + BufSize/2;
        gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 2;
        gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 3;
        BufAddr += BufSize/2 * 4;
        TotalBufSize += BufSize/2 * 4;

        //CbCr gain
        gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN] = IE_MODE_ARF_CbOperator[Index][YCC_OP_GAIN];
        gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN] = IE_MODE_ARF_CrOperator[Index][YCC_OP_GAIN];

        //CbCr offset
        gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CbOperator[Index][YCC_OP_OFFSET];
        gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CrOperator[Index][YCC_OP_OFFSET];

        hwmem_open();
        hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET], BufSize);
        hwmem_close();
    }

    //IE_INDEX_ARF_ToyPhoto
    {
        JpgScaleUpDown_t JpgScale;

        Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

        //memory allocate
        gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr;
        BufAddr += BufSize;
        TotalBufSize += BufSize;

        //Shading Img
        JpgScale.ScaleLevel  = 0;
        JpgScale.src = (UINT8*)IE_MODE_ARF_ShadingImg;
        JpgScale.dst = (UINT8*)gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
        JpgScale.InputWidth  = IE_MODE_ARF_ShadingImgHsize;
        JpgScale.InputHeight = IE_MODE_ARF_ShadingImgVsize;
        JpgScale.InputOffset = IE_MODE_ARF_ShadingImgHsize;
        JpgScale.OutputWidth = gImgEffectInfo.SourceFBInfo.HSize;
        JpgScale.OutputHeight= gImgEffectInfo.SourceFBInfo.VSize;
        JpgScale.OutputOffset= gImgEffectInfo.SourceFBInfo.LineOffset;
        JpgScaleUpDown(&JpgScale);

        //memory allocate
        gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN] = BufAddr;
        gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN] = BufAddr + BufSize/2;
        gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 2;
        gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 3;
        BufAddr += BufSize/2 * 4;
        TotalBufSize += BufSize/2 * 4;

        //CbCr gain
        gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN] = IE_MODE_ARF_CbOperator[Index][YCC_OP_GAIN];
        gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN] = IE_MODE_ARF_CrOperator[Index][YCC_OP_GAIN];

        //CbCr offset
        gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CbOperator[Index][YCC_OP_OFFSET];
        gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CrOperator[Index][YCC_OP_OFFSET];

        hwmem_open();
        hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET], BufSize);
        hwmem_close();
    }

    //IE_INDEX_ARF_FishEye
    {
        Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;
    }

    //IE_INDEX_ARF_Sketch
    {
        Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

        //Y Edge Gain/Offse/MapInit
        gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN] = IE_MODE_ARF_YOperator[Index][YCC_OP_GAIN];
        gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET] = IE_MODE_ARF_YOperator[Index][YCC_OP_OFFSET];
        gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP] = IE_MODE_ARF_YOperator[Index][YCC_OP_MAP];

        //memory allocate
        gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN] = BufAddr;
        gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET] = BufAddr + BufSize;
        gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr + BufSize * 2;
        BufAddr += BufSize * 3;
        TotalBufSize += BufSize * 3;

        hwmem_open();
        hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET], BufSize);
        hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP], gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP], BufSize);
        hwmem_close();
    }

    //IE_INDEX_ARF_FantasticFocus
    //IE_INDEX_ARF_RoughMonoChorme

    return  TotalBufSize;
}

UINT32 IE_InitSingleARF(UINT32 BufAddr)
{
    UINT32  TotalBufSize = 0;
    UINT32  BufSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;
    UINT32  Index;

    switch (gImgEffectInfo.SingleCH.Index)
    {
        case IE_INDEX_ARF_PopArt:
        {
            Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];

            //memory allocate
            gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN] = BufAddr;
            gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN] = BufAddr + BufSize/2;
            gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 2;
            gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET] = BufAddr + BufSize/2 * 3;
            BufAddr += BufSize/2 * 4;
            TotalBufSize += BufSize/2 * 4;

            //CbCr gain
            gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN] = IE_MODE_ARF_CbOperator[Index][YCC_OP_GAIN];
            gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN] = IE_MODE_ARF_CrOperator[Index][YCC_OP_GAIN];

            //CbCr offset
            gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CbOperator[Index][YCC_OP_OFFSET];
            gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET] = IE_MODE_ARF_CrOperator[Index][YCC_OP_OFFSET];

            hwmem_open();
            hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CbOperator[YCC_OP_OFFSET], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].CrOperator[YCC_OP_OFFSET], BufSize);
            hwmem_close();
        }
        break;

        case IE_INDEX_ARF_ToyPhoto:
        {
            JpgScaleUpDown_t JpgScale;

            Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];

            //memory allocate
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr;
            BufAddr += BufSize;
            TotalBufSize += BufSize;

            //Shading Img
            JpgScale.ScaleLevel  = 0;
            JpgScale.src = (UINT8*)IE_MODE_ARF_ShadingImg;
            JpgScale.dst = (UINT8*)gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
            JpgScale.InputWidth  = IE_MODE_ARF_ShadingImgHsize;
            JpgScale.InputHeight = IE_MODE_ARF_ShadingImgVsize;
            JpgScale.InputOffset = IE_MODE_ARF_ShadingImgHsize;
            JpgScale.OutputWidth = gImgEffectInfo.SourceFBInfo.HSize;
            JpgScale.OutputHeight= gImgEffectInfo.SourceFBInfo.VSize;
            JpgScale.OutputOffset= gImgEffectInfo.SourceFBInfo.LineOffset;
            JpgScaleUpDown(&JpgScale);
        }
        break;
        case IE_INDEX_ARF_FishEye:
        {
            Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;

            //GammaLUT
            gImgEffectParam.ARF[Index].GammaLUT = IE_MODE_ARF_GammaLUT[Index];
        }
        break;
        case IE_INDEX_ARF_Sketch:
        {
            Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

            //Y Edge Gain/Offse/MapInit
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN] = IE_MODE_ARF_YOperator[Index][YCC_OP_GAIN];
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET] = IE_MODE_ARF_YOperator[Index][YCC_OP_OFFSET];
            gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP] = IE_MODE_ARF_YOperator[Index][YCC_OP_MAP];

            //memory allocate
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN] = BufAddr;
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET] = BufAddr + BufSize;
            gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP] = BufAddr + BufSize * 2;
            BufAddr += BufSize * 3;
            TotalBufSize += BufSize * 3;

            hwmem_open();
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN], gImgEffectParam.ARF[Index].YOperator[YCC_OP_GAIN], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET], gImgEffectParam.ARF[Index].YOperator[YCC_OP_OFFSET], BufSize);
            hwmem_memset(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP], gImgEffectParam.ARF[Index].YOperator[YCC_OP_MAP], BufSize);
            hwmem_close();
        }
        break;
    }

    return  TotalBufSize;
}

UINT32 IE_InitCaptureARF(void)
{
    UINT32  TotalBufSize = 0;
    UINT32  CapBufAddr, PreQVBufAddr;
    UINT32  CapBufSize, PreQVBufSize;
    UINT32  Index;

    CapBufAddr = ippbuf_GetBuf(IPPBUF_CAP_IE_SLC);
    CapBufSize = GetIPPSizeInfor(_ImeCapOutHsize) * GetIPPSizeInfor(_ImeCapOutVsize);

    PreQVBufAddr = ippbuf_GetBuf(IPPBUF_CAP_IE_QV_PRE);
    PreQVBufSize = GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize);

    switch (gImgEffectInfo.CaptureCH.Index)
    {
        case IE_INDEX_ARF_PopArt:
        {
            Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;
        }
        break;

        case IE_INDEX_ARF_ToyPhoto:
        {
            Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

            //memory allocate
            gImgEffectParam.ARFCap[Index].YAddr[YCC_OP_MAP] = CapBufAddr;
            TotalBufSize += CapBufSize;

            gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP] = PreQVBufAddr;
            TotalBufSize += PreQVBufSize;
        }
        break;
        case IE_INDEX_ARF_FishEye:
        {
            Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;
        }
        break;
        case IE_INDEX_ARF_Sketch:
        {
            Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

            //memory allocate
            gImgEffectParam.ARFCap[Index].YAddr[YCC_OP_MAP] = CapBufAddr;
            TotalBufSize += CapBufSize;

            gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP] = PreQVBufAddr;
            TotalBufSize += PreQVBufSize;
        }
        break;
    }

    return  TotalBufSize;
}

void IE_ProcessMultiEV(UINT32 SrcFBID, UINT32 ChannelActFBID)
{
    UINT8*  ChannelGammaLUT[IE_CH_MAX];
    UINT32  ChannelAddr[IE_CH_MAX];
    UINT32  SrcFBSize;
    IE_CH   Channel;

    ChannelAddr[IE_CH_1] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_2] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_3] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_4] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[ChannelActFBID];
    ChannelGammaLUT[IE_CH_1] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_1].Index-IE_INDEX_EV_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_2] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_2].Index-IE_INDEX_EV_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_3] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_3].Index-IE_INDEX_EV_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_4] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_4].Index-IE_INDEX_EV_MIN)].GammaLUT;
    SrcFBSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;

    IE_MultiGammaLUT(ChannelAddr,
                     ChannelGammaLUT,
                     IE_CH_MAX,
                     gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                     gImgEffectInfo.SourceFBInfo.HSize,
                     gImgEffectInfo.SourceFBInfo.VSize);

    hwmem_open();
    for(Channel=IE_CH_1; Channel<IE_CH_MAX; Channel++)
    {
        hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                        gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                        SrcFBSize/2 );

        hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                        gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                        SrcFBSize/2 );
    }
    hwmem_close();
}

void IE_ProcessMultiEVARF(UINT32 SrcFBID, UINT32 ChannelActFBID)
{
    UINT8*  ChannelGammaLUT[IE_CH_MAX];
    UINT32  ChannelAddr[IE_CH_MAX];
    UINT32  Index;
    IE_CH   Channel;

    ChannelAddr[IE_CH_1] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_2] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_3] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_4] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[ChannelActFBID];
    ChannelGammaLUT[IE_CH_1] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_1].Index-IE_INDEX_EV_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_2] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_2].Index-IE_INDEX_EV_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_3] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_3].Index-IE_INDEX_EV_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_4] = gImgEffectParam.EV[(gImgEffectInfo.MultiCH[IE_CH_4].Index-IE_INDEX_EV_MIN)].GammaLUT;

    IE_MultiGammaLUT(ChannelAddr,
                     ChannelGammaLUT,
                     IE_CH_MAX,
                     gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                     gImgEffectInfo.SourceFBInfo.HSize,
                     gImgEffectInfo.SourceFBInfo.VSize);

    grph_open();
    grph_enableDMA();
    for(Channel=IE_CH_1; Channel<IE_CH_MAX; Channel++)
    {
        switch (gImgEffectInfo.MultiCHARFSubIndex)
        {
            case IE_INDEX_ARF_PopArt:
            {
                Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

                //Cb Offset/Gain
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

                //Cr Offset/Gain
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);
            }
            break;

            case IE_INDEX_ARF_ToyPhoto:
            {
                Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

                //Y Shading
                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

                grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_ShadingImgDIV,0,0);

                //CbCr Copy
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

                grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

            }
            break;

            case IE_INDEX_ARF_FishEye:
            {
                #if 0
                GDC_YCC_PRV_PARAM FishEyeParam;

                Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;

                //CbCr Copy
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

                grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

                grph_disableDMA();
                grph_swReset();
                grph_close();

                FishEyeParam.width = gImgEffectInfo.MultiCH[Channel].FBInfo.HSize;
                FishEyeParam.height = gImgEffectInfo.MultiCH[Channel].FBInfo.VSize;
                FishEyeParam.in_Y_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID];
                FishEyeParam.in_Cb_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID];
                FishEyeParam.in_Cr_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID];
                gdc_renderFishEye_YCC(&FishEyeParam);

                grph_open();
                grph_enableDMA();
                #endif
            }
            break;

            case IE_INDEX_ARF_Sketch:
            {
                //Y Edge Extraction
                FDE_OPENOBJ         FDEOpenObj;
                FDE_FDE_PRAM        fde_info;

                Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

                FDEOpenObj.FP_FDEISR_CB = NULL;
//KH mark
#if 0
                fde_open(&FDEOpenObj);

                fde_info.in_addr = gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID];
                fde_info.out_addr0 = gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
                fde_info.opp = DISABLE;
                fde_info.width = gImgEffectInfo.MultiCH[Channel].FBInfo.HSize;
                fde_info.height = gImgEffectInfo.MultiCH[Channel].FBInfo.VSize;
                fde_info.lineoffset = gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset;
                fde_info.hfact = 0;
                fde_info.vfact = fde_info.hfact;
                fde_info.sclnum = 1;
                fde_info.opmode = FDE_OPMODE_EDGHV1;
                fde_setFDMode(FDE_FD_EDGE_EXTRACTION, &fde_info);
                fde_close();
#endif
                //Y Edge Gain/Offset
                grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

                grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setImg3(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

                grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

                grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                //CbCr memset
                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);
            }
            break;

            case IE_INDEX_ARF_FantasticFocus:
            break;

            case IE_INDEX_ARF_RoughMonoChorme:
            break;
        }
    }
    grph_disableDMA();
    grph_swReset();
    grph_close();
}

void IE_updateMultiAWB(void)
{
    UINT32  MwbRgain, MwbBgain, Index;

    gAwbChCnt++;
    if(gAwbChCnt >= IE_CH_MAX)
        gAwbChCnt = IE_CH_1;

    IE_MODE_AWB_ManualRBgain[IE_INDEX_AWB_Auto - IE_INDEX_AWB_MIN][0] = Rgain;
    IE_MODE_AWB_ManualRBgain[IE_INDEX_AWB_Auto - IE_INDEX_AWB_MIN][1] = Bgain;

    Index = gImgEffectInfo.MultiCH[gAwbChCnt].Index - IE_INDEX_AWB_MIN;
    MwbRgain = IE_MODE_AWB_ManualRBgain[Index][0];
    MwbBgain = IE_MODE_AWB_ManualRBgain[Index][1];
    aaa_SetIPEColorgain(MwbRgain, 256, 256, MwbBgain);
}

void IE_ProcessMultiAWB(UINT32 SrcFBID, UINT32 ChannelActFBID)
{
    UINT32  SrcFBSize;
    IE_CH   Channel=gAwbChMark;

    SrcFBSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;

    //YCbCr Copy
    hwmem_open();

    hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                    gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                    SrcFBSize );

    hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                    gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                    SrcFBSize/2 );

    hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                    gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                    SrcFBSize/2 );
    hwmem_close();
}

void IE_ProcessMultiAWBARF(UINT32 SrcFBID, UINT32 ChannelActFBID)
{
    UINT32  SrcFBSize, Index;
    IE_CH   Channel=gAwbChMark;

    SrcFBSize = gImgEffectInfo.SourceFBInfo.LineOffset * gImgEffectInfo.SourceFBInfo.VSize;

    //YCbCr Copy
    hwmem_open();

    hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                    gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                    SrcFBSize );

    hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                    gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                    SrcFBSize/2 );

    hwmem_memcpy(   gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                    gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                    SrcFBSize/2 );
    hwmem_close();


    grph_open();
    grph_enableDMA();
    switch (gImgEffectInfo.MultiCHARFSubIndex)
    {
        case IE_INDEX_ARF_PopArt:
        {
            Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

            //Cb Offset/Gain
            grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

            grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

            //Cr Offset/Gain
            grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

            grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);
        }
        break;

        case IE_INDEX_ARF_ToyPhoto:
        {
            Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

            //Y Shading
            grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

            grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

            grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_ShadingImgDIV,0,0);
        }
        break;

        case IE_INDEX_ARF_FishEye:
        {
            #if 0
            GDC_YCC_PRV_PARAM FishEyeParam;

            Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;

            grph_disableDMA();
            grph_swReset();
            grph_close();

            FishEyeParam.width = gImgEffectInfo.MultiCH[Channel].FBInfo.HSize;
            FishEyeParam.height = gImgEffectInfo.MultiCH[Channel].FBInfo.VSize;
            FishEyeParam.in_Y_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID];
            FishEyeParam.in_Cb_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID];
            FishEyeParam.in_Cr_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID];
            gdc_renderFishEye_YCC(&FishEyeParam);

            grph_open();
            grph_enableDMA();
            #endif
        }
        break;

        case IE_INDEX_ARF_Sketch:
        {
            //Y Edge Extraction
            FDE_OPENOBJ         FDEOpenObj;
            FDE_FDE_PRAM        fde_info;

            Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

            FDEOpenObj.FP_FDEISR_CB = NULL;
//KH mark
#if 0
            fde_open(&FDEOpenObj);
#endif
            fde_info.in_addr = gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID];
            fde_info.out_addr0 = gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
            fde_info.opp = DISABLE;
            fde_info.width = gImgEffectInfo.MultiCH[Channel].FBInfo.HSize;
            fde_info.height = gImgEffectInfo.MultiCH[Channel].FBInfo.VSize;
            fde_info.lineoffset = gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset;
            fde_info.hfact = 0;
            fde_info.vfact = fde_info.hfact;
            fde_info.sclnum = 1;
            fde_info.opmode = FDE_OPMODE_EDGHV1;
//KH mark
#if 0
            fde_setFDMode(FDE_FD_EDGE_EXTRACTION, &fde_info);
            fde_close();
#endif

            //Y Edge Gain/Offset
            grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

            grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

            grph_setImg3(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

            grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

            grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

            grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

            //CbCr memset
            grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

            grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                         gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);
        }
        break;

        case IE_INDEX_ARF_FantasticFocus:
        break;

        case IE_INDEX_ARF_RoughMonoChorme:
        break;
    }
    grph_disableDMA();
    grph_swReset();
    grph_close();
}

void IE_ProcessMultiARF(UINT32 SrcFBID, UINT32 ChannelActFBID)
{
    UINT8*  ChannelGammaLUT[IE_CH_MAX];
    UINT32  ChannelAddr[IE_CH_MAX];
    UINT32  Index;
    IE_CH   Channel;

    ChannelAddr[IE_CH_1] = gImgEffectInfo.MultiCH[IE_CH_1].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_2] = gImgEffectInfo.MultiCH[IE_CH_2].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_3] = gImgEffectInfo.MultiCH[IE_CH_3].FBInfo.YAddr[ChannelActFBID];
    ChannelAddr[IE_CH_4] = gImgEffectInfo.MultiCH[IE_CH_4].FBInfo.YAddr[ChannelActFBID];
    ChannelGammaLUT[IE_CH_1] = gImgEffectParam.ARF[(gImgEffectInfo.MultiCH[IE_CH_1].Index-IE_INDEX_ARF_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_2] = gImgEffectParam.ARF[(gImgEffectInfo.MultiCH[IE_CH_2].Index-IE_INDEX_ARF_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_3] = gImgEffectParam.ARF[(gImgEffectInfo.MultiCH[IE_CH_3].Index-IE_INDEX_ARF_MIN)].GammaLUT;
    ChannelGammaLUT[IE_CH_4] = gImgEffectParam.ARF[(gImgEffectInfo.MultiCH[IE_CH_4].Index-IE_INDEX_ARF_MIN)].GammaLUT;

    IE_MultiGammaLUT(ChannelAddr,
                     ChannelGammaLUT,
                     IE_CH_MAX,
                     gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                     gImgEffectInfo.SourceFBInfo.HSize,
                     gImgEffectInfo.SourceFBInfo.VSize);

    grph_open();
    grph_enableDMA();
    for(Channel=IE_CH_1; Channel<IE_CH_MAX; Channel++)
    {
        //debug_err(("Channel=%d Index=%d\r\n",Channel, gImgEffectInfo.CH[Channel].Index));
        switch (gImgEffectInfo.MultiCH[Channel].Index)
        {
            case IE_INDEX_ARF_PopArt:
            {
                Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

                //Cb Offset/Gain
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

                //Cr Offset/Gain
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);
            }
            break;

            case IE_INDEX_ARF_ToyPhoto:
            {
                Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

                //Y Shading
                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

                grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_ShadingImgDIV,0,0);

                //Cb Offset/Gain
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

                //Cr Offset/Gain
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);
            }
            break;

            case IE_INDEX_ARF_FishEye:
            {
                #if 0
                GDC_YCC_PRV_PARAM FishEyeParam;

                Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;

                //CbCr Copy
                grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

                grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                             gImgEffectInfo.SourceFBInfo.LineOffset/2,
                             gImgEffectInfo.SourceFBInfo.VSize,
                             gImgEffectInfo.SourceFBInfo.HSize/2);

                grph_setImg2(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

                grph_disableDMA();
                grph_swReset();
                grph_close();

                FishEyeParam.width = gImgEffectInfo.MultiCH[Channel].FBInfo.HSize;
                FishEyeParam.height = gImgEffectInfo.MultiCH[Channel].FBInfo.VSize;
                FishEyeParam.in_Y_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID];
                FishEyeParam.in_Cb_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID];
                FishEyeParam.in_Cr_Addr = gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID];
                gdc_renderFishEye_YCC(&FishEyeParam);

                grph_open();
                grph_enableDMA();
                #endif
            }
            break;

            case IE_INDEX_ARF_Sketch:
            {
                //Y Edge Extraction
                FDE_OPENOBJ         FDEOpenObj;
                FDE_FDE_PRAM        fde_info;

                Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

                FDEOpenObj.FP_FDEISR_CB = NULL;
//KH mark
#if 0
                fde_open(&FDEOpenObj);
                fde_info.in_addr = gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID];
                fde_info.out_addr0 = gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
                fde_info.opp = DISABLE;
                fde_info.width = gImgEffectInfo.MultiCH[Channel].FBInfo.HSize;
                fde_info.height = gImgEffectInfo.MultiCH[Channel].FBInfo.VSize;
                fde_info.lineoffset = gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset;
                fde_info.hfact = 0;
                fde_info.vfact = fde_info.hfact;
                fde_info.sclnum = 1;
                fde_info.opmode = FDE_OPMODE_EDGHV1;
                fde_setFDMode(FDE_FD_EDGE_EXTRACTION, &fde_info);
                fde_close();
#endif
                //Y Edge Gain/Offset
                grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

                grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setImg3(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

                grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize);

                grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setImg3(gImgEffectInfo.MultiCH[Channel].FBInfo.YAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset);

                grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

                //CbCr memset
                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CbAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

                grph_setImg1(gImgEffectInfo.MultiCH[Channel].FBInfo.CrAddr[ChannelActFBID],
                             gImgEffectInfo.MultiCH[Channel].FBInfo.LineOffset/2,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.VSize,
                             gImgEffectInfo.MultiCH[Channel].FBInfo.HSize/2);

                grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);
            }
            break;

            case IE_INDEX_ARF_FantasticFocus:
            break;

            case IE_INDEX_ARF_RoughMonoChorme:
            break;
        }
    }
    grph_disableDMA();
    grph_swReset();
    grph_close();
}

void IE_ProcessSingleARF(UINT32 SrcFBID, UINT32 ChannelActFBID)
{
    UINT32  Index;

    grph_open();
    grph_enableDMA();
    switch (gImgEffectInfo.SingleCH.Index)
    {
        case IE_INDEX_ARF_PopArt:
        {
            Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;

            //Y Copy
            grph_setImg1(gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize);

            grph_setImg2(gImgEffectInfo.SingleCH.FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

            //Cb Offset/Gain
            grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_OFFSET],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

            grph_setImg1(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2,
                         gImgEffectInfo.SingleCH.FBInfo.VSize,
                         gImgEffectInfo.SingleCH.FBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CbAddr[YCC_OP_GAIN],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

            //Cr Offset/Gain
            grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_OFFSET],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

            grph_setImg1(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2,
                         gImgEffectInfo.SingleCH.FBInfo.VSize,
                         gImgEffectInfo.SingleCH.FBInfo.HSize/2);

            grph_setImg2(gImgEffectParam.ARF[Index].CrAddr[YCC_OP_GAIN],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);
        }
        break;

        case IE_INDEX_ARF_ToyPhoto:
        {
            Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;

            //Y Shading
            grph_setImg1(gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize);

            grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.SourceFBInfo.LineOffset);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_ShadingImgDIV,0,0);

            //CbCr Copy
            grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize/2);

            grph_setImg2(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

            grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize/2);

            grph_setImg2(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);
        }
        break;

        case IE_INDEX_ARF_FishEye:
        {
            #if 0
            GDC_YCC_PRV_PARAM FishEyeParam;

            Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;

            //Y Copy
            grph_setImg1(gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize);

            grph_setImg2(gImgEffectInfo.SingleCH.FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

            //CbCr Copy
            grph_setImg1(gImgEffectInfo.SourceFBInfo.CbAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize/2);

            grph_setImg2(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

            grph_setImg1(gImgEffectInfo.SourceFBInfo.CrAddr[SrcFBID],
                         gImgEffectInfo.SourceFBInfo.LineOffset/2,
                         gImgEffectInfo.SourceFBInfo.VSize,
                         gImgEffectInfo.SourceFBInfo.HSize/2);

            grph_setImg2(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

            grph_disableDMA();
            grph_swReset();
            grph_close();

            FishEyeParam.width = gImgEffectInfo.SingleCH.FBInfo.HSize;
            FishEyeParam.height = gImgEffectInfo.SingleCH.FBInfo.VSize;
            FishEyeParam.in_Y_Addr = gImgEffectInfo.SingleCH.FBInfo.YAddr[ChannelActFBID];
            FishEyeParam.in_Cb_Addr = gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID];
            FishEyeParam.in_Cr_Addr = gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID];
            gdc_renderFishEye_YCC(&FishEyeParam);

            grph_open();
            grph_enableDMA();
            #endif
        }
        break;

        case IE_INDEX_ARF_Sketch:
        {
            //Y Edge Extraction
            FDE_OPENOBJ         FDEOpenObj;
            FDE_FDE_PRAM        fde_info;

            Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

            FDEOpenObj.FP_FDEISR_CB = NULL;
//KH mark
#if 0
            fde_open(&FDEOpenObj);
            fde_info.in_addr = gImgEffectInfo.SourceFBInfo.YAddr[SrcFBID];
            fde_info.out_addr0 = gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP];
            fde_info.opp = DISABLE;
            fde_info.width = gImgEffectInfo.SourceFBInfo.HSize;
            fde_info.height = gImgEffectInfo.SourceFBInfo.VSize;
            fde_info.lineoffset = gImgEffectInfo.SourceFBInfo.LineOffset;
            fde_info.hfact = 0;
            fde_info.vfact = fde_info.hfact;
            fde_info.sclnum = 1;
            fde_info.opmode = FDE_OPMODE_EDGHV1;
            fde_setFDMode(FDE_FD_EDGE_EXTRACTION, &fde_info);
            fde_close();
#endif
            //Y Edge Gain/Offset
            grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset,
                         gImgEffectInfo.SingleCH.FBInfo.VSize,
                         gImgEffectInfo.SingleCH.FBInfo.HSize);

            grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_GAIN],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setImg3(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_GainDIV,0,0);

            grph_setImg1(gImgEffectParam.ARF[Index].YAddr[YCC_OP_OFFSET],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset,
                         gImgEffectInfo.SingleCH.FBInfo.VSize,
                         gImgEffectInfo.SingleCH.FBInfo.HSize);

            grph_setImg2(gImgEffectParam.ARF[Index].YAddr[YCC_OP_MAP],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setImg3(gImgEffectInfo.SingleCH.FBInfo.YAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset);

            grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_MINUS_SHF,0,0,0);

            //CbCr memset
            grph_setImg1(gImgEffectInfo.SingleCH.FBInfo.CbAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2,
                         gImgEffectInfo.SingleCH.FBInfo.VSize,
                         gImgEffectInfo.SingleCH.FBInfo.HSize/2);

            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

            grph_setImg1(gImgEffectInfo.SingleCH.FBInfo.CrAddr[ChannelActFBID],
                         gImgEffectInfo.SingleCH.FBInfo.LineOffset/2,
                         gImgEffectInfo.SingleCH.FBInfo.VSize,
                         gImgEffectInfo.SingleCH.FBInfo.HSize/2);

            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);
        }
        break;

        case IE_INDEX_ARF_FantasticFocus:
        break;

        case IE_INDEX_ARF_RoughMonoChorme:
        break;
    }
    grph_disableDMA();
    grph_swReset();
    grph_close();
}

void IE_ProcessCaptureARF(void)
{
    UINT32  Index;
    UINT32  P1YAddr, P1CbAddr, P1CrAddr;
    UINT32  P2YAddr, P2CbAddr, P2CrAddr;
    IME_OPENOBJ IMEOpenObj;
//KH mark
#if 0
    IME_MODE_PRAM OneIme;
#endif

    //IME output path1 address
    P1YAddr  = ippbuf_GetBuf(IPPBUF_CAP_FRAME_Y);
    P1CbAddr = ippbuf_GetBuf(IPPBUF_CAP_FRAME_CB);
    P1CrAddr = ippbuf_GetBuf(IPPBUF_CAP_FRAME_CR);
    P1YAddr  += GetIPPSizeInfor(_DceAddrOffset) + (GetIPPSizeInfor(_ImeCapOutLineOffset) * GetIPPSizeInfor(_PreD2DOffsetLine));
    P1CbAddr += GetIPPSizeInfor(_DceAddrOffset) + ((GetIPPSizeInfor(_ImeCapOutLineOffset) * GetIPPSizeInfor(_PreD2DOffsetLine)) >> 1);
    P1CrAddr += GetIPPSizeInfor(_DceAddrOffset) + ((GetIPPSizeInfor(_ImeCapOutLineOffset) * GetIPPSizeInfor(_PreD2DOffsetLine)) >> 1);

    //IME output path2 address
    P2YAddr  = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    P2CbAddr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE) + (GetIPPSizeInfor(_QVPreLineOffset) * (GetIPPSizeInfor(_QVPreVsize) + GetIPPSizeInfor(_PreD2DOffsetLine)));
    P2CrAddr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE) + (GetIPPSizeInfor(_QVPreLineOffset) * (GetIPPSizeInfor(_QVPreVsize) + GetIPPSizeInfor(_PreD2DOffsetLine)) * 3 / 2);
    P2YAddr  += GetIPPSizeInfor(_DceAddrOffset) + (GetIPPSizeInfor(_QVPreLineOffset) * GetIPPSizeInfor(_PreD2DOffsetLine));
    P2CbAddr += GetIPPSizeInfor(_DceAddrOffset) + ((GetIPPSizeInfor(_QVPreLineOffset) * GetIPPSizeInfor(_PreD2DOffsetLine)) >> 1);
    P2CrAddr += GetIPPSizeInfor(_DceAddrOffset) + ((GetIPPSizeInfor(_QVPreLineOffset) * GetIPPSizeInfor(_PreD2DOffsetLine)) >> 1);

    grph_open();
    grph_enableDMA();
    switch (gImgEffectInfo.CaptureCH.Index)
    {
        case IE_INDEX_ARF_PopArt:
        {
            Index = IE_INDEX_ARF_PopArt - IE_INDEX_ARF_MIN;
        }
        break;

        //#NT#2009/11/23#YC Peng -begin
        //#modify ARF memory arrangement
        case IE_INDEX_ARF_ToyPhoto:
        {
            DECJPG_INFO DecOneJPGInfo = {0};
            JPGHEAD_DEC_CFG JPGCfg = {0};
            UINT32  ShadingAddr;

            Index = IE_INDEX_ARF_ToyPhoto - IE_INDEX_ARF_MIN;
            ShadingAddr = ippbuf_GetBuf(IPPBUF_CAP_IE_SHADING);

            DecOneJPGInfo.pSrcAddr          = (UINT8 *)gImgEffectShadingJPG.BitStreamAddr;
            DecOneJPGInfo.pDstAddr          = (UINT8 *)ShadingAddr;
            DecOneJPGInfo.JPGFileSize       = gImgEffectShadingJPG.BitStreamSize;
            DecOneJPGInfo.jdcfg_p           = &JPGCfg;
            DecOneJPGInfo.IfDecodeThumb     = DEC_PRIMARY;
            DecOneJPGInfo.bEnableTimeOut    = FALSE;
            DecOneJPGInfo.bEnableCrop       = FALSE;
            // Decode startup JPG, DecodeOneJPG() will open JPEG driver
            if (DecodeOneJPG(&DecOneJPGInfo) != E_OK)
                debug_err(("DecodeOneJPG(&DecOneJPGInfo) != E_OK\r\n"));
            if(DecOneJPGInfo.jdcfg_p->fileformat!=JPGFormatYUV100)
                debug_err(("Bitstream is not a Y only Jpeg!\r\n"));

            //PreQV Y Shading
//KH mark
#if 0
            OneIme.op_mode = _D2D;
            OneIme.in_src = IMEALG_DMA2IME;
            OneIme.bil = IMEALG_BICUBIC;
            OneIme.in_addr.y_addr  = (UINT32)ShadingAddr;
            OneIme.in_addr.cb_addr = (UINT32)ShadingAddr;
            OneIme.in_addr.cr_addr = (UINT32)ShadingAddr;
            OneIme.format.in_format = IMEALG_INYUV422;
            OneIme.in_size.h_size = JPGCfg.imagewidth;
            OneIme.in_size.v_size = JPGCfg.imageheight;
            OneIme.line_ofs.in_lineofs = JPGCfg.lineoffsetY;
            OneIme.path_num = IMEALG_1PATHY;
            OneIme.p1_out0_addr.y_addr  = gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.p1_out0_addr.cb_addr = gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.p1_out0_addr.cr_addr = gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
            OneIme.p1_out_size.h_size = GetIPPSizeInfor(_QVPreHsize);
            OneIme.p1_out_size.v_size = GetIPPSizeInfor(_QVPreVsize);
            OneIme.line_ofs.out1_lineofs = GetIPPSizeInfor(_QVPreHsize);
            IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
            IMEOpenObj.FP_IMEISR_CB = NULL;
            ime_open(&IMEOpenObj);
            ime_setMode(&OneIme);
            ime_clrIntFlag();
            ime_setStart();
            ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
            ime_setStop();
            ime_close();

            grph_setImg1(P2YAddr,
                         GetIPPSizeInfor(_QVPreLineOffset),
                         GetIPPSizeInfor(_QVPreVsize),
                         GetIPPSizeInfor(_QVPreHsize));

            grph_setImg2(gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP],
                         GetIPPSizeInfor(_QVPreHsize));

            grph_setImg3(P2YAddr,
                         GetIPPSizeInfor(_QVPreLineOffset));

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_ShadingImgDIV,0,0);

            //Capture Y Shading
            OneIme.op_mode = _D2D;
            OneIme.in_src = IMEALG_DMA2IME;
            OneIme.bil = IMEALG_BICUBIC;
            OneIme.in_addr.y_addr  = (UINT32)ShadingAddr;
            OneIme.in_addr.cb_addr = (UINT32)ShadingAddr;
            OneIme.in_addr.cr_addr = (UINT32)ShadingAddr;
            OneIme.format.in_format = IMEALG_INYUV422;
            OneIme.in_size.h_size = JPGCfg.imagewidth;
            OneIme.in_size.v_size = JPGCfg.imageheight;
            OneIme.line_ofs.in_lineofs = JPGCfg.lineoffsetY;
            OneIme.path_num = IMEALG_1PATHY;
            OneIme.p1_out0_addr.y_addr  = gImgEffectParam.ARFCap[Index].YAddr[YCC_OP_MAP];
            OneIme.p1_out0_addr.cb_addr = gImgEffectParam.ARFCap[Index].YAddr[YCC_OP_MAP];
            OneIme.p1_out0_addr.cr_addr = gImgEffectParam.ARFCap[Index].YAddr[YCC_OP_MAP];
            OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
            OneIme.p1_out_size.h_size = GetIPPSizeInfor(_ImeCapOutHsize);
            OneIme.p1_out_size.v_size = GetIPPSizeInfor(_ImeCapOutVsize);
            OneIme.line_ofs.out1_lineofs = GetIPPSizeInfor(_ImeCapOutHsize);
            IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
            IMEOpenObj.FP_IMEISR_CB = NULL;
            ime_open(&IMEOpenObj);
            ime_setMode(&OneIme);
            ime_clrIntFlag();
            ime_setStart();
            ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
            ime_setStop();
            ime_close();
#endif
            grph_setImg1(P1YAddr,
                         GetIPPSizeInfor(_ImeCapOutLineOffset),
                         GetIPPSizeInfor(_ImeCapOutVsize),
                         GetIPPSizeInfor(_ImeCapOutHsize));

            grph_setImg2(gImgEffectParam.ARFCap[Index].YAddr[YCC_OP_MAP],
                         GetIPPSizeInfor(_ImeCapOutHsize));

            grph_setImg3(P1YAddr,
                         GetIPPSizeInfor(_ImeCapOutLineOffset));

            grph_setAOP2(8,GRPH_DST_1,GRPH_AOP_MULTIPLY_DIV,IE_MODE_ARF_ShadingImgDIV,0,0);
        }
        break;
        //#NT#2009/11/23#YC Peng -end

        case IE_INDEX_ARF_FishEye:
        {
            Index = IE_INDEX_ARF_FishEye - IE_INDEX_ARF_MIN;
        }
        break;

        case IE_INDEX_ARF_Sketch:
        {
            IPE_PARAM IpeParam;

            Index = IE_INDEX_ARF_Sketch - IE_INDEX_ARF_MIN;

//KH mark
#if 0
            IpeParam.InSizeH = GetIPPSizeInfor(_QVPreHsize);
            IpeParam.InSizeV = GetIPPSizeInfor(_QVPreVsize);
            IpeParam.InLineOfs = IpeParam.OutLineOfs = GetIPPSizeInfor(_QVPreLineOffset);
            IpeParam.BitDepth = IPE_PACK_08bit;
            IpeParam.Add0 = IpeParam.Add1 = P2YAddr;
            IpeParam.VAAddr = gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            IpeParam.OutSrcSel = YDATA_TYPE;
            IpeParam.D2DInFormat = SINGLE_CHANNEL_IN;
            ipe_Dram2Dram(&IpeParam, IPED2D_ARF_SKETCH, IPE_WAIT);

            //PreQV Sketch
            OneIme.op_mode = _D2D;
            OneIme.in_src = IMEALG_DMA2IME;
            OneIme.bil = IMEALG_BICUBIC;
            OneIme.in_addr.y_addr  = (UINT32)gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.in_addr.cb_addr = (UINT32)gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.in_addr.cr_addr = (UINT32)gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.format.in_format = IMEALG_INYUV422;
            OneIme.in_size.h_size = GetIPPSizeInfor(_QVPreHsize)-16;
            OneIme.in_size.v_size = GetIPPSizeInfor(_QVPreVsize);
            OneIme.line_ofs.in_lineofs = GetIPPSizeInfor(_QVPreLineOffset);
            OneIme.path_num = IMEALG_1PATHY;
            OneIme.p1_out0_addr.y_addr  = P2YAddr ;
            OneIme.p1_out0_addr.cb_addr = P2CbAddr;
            OneIme.p1_out0_addr.cr_addr = P2CrAddr;
            OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
            OneIme.p1_out_size.h_size = GetIPPSizeInfor(_QVPreHsize);
            OneIme.p1_out_size.v_size = GetIPPSizeInfor(_QVPreVsize);
            OneIme.line_ofs.out1_lineofs = GetIPPSizeInfor(_QVPreLineOffset);
            IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
            IMEOpenObj.FP_IMEISR_CB = NULL;
            ime_open(&IMEOpenObj);
            ime_setMode(&OneIme);
            ime_clrIntFlag();
            ime_setStart();
            ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
            ime_setStop();
            ime_close();


            grph_setImg1(OneIme.p1_out0_addr.cb_addr,
                         OneIme.line_ofs.out1_lineofs/2,
                         OneIme.p1_out_size.v_size,
                         OneIme.p1_out_size.h_size/2);

            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

            grph_setImg1(OneIme.p1_out0_addr.cr_addr,
                         OneIme.line_ofs.out1_lineofs/2,
                         OneIme.p1_out_size.v_size,
                         OneIme.p1_out_size.h_size/2);

            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

            //Capture Sketch
            OneIme.op_mode = _D2D;
            OneIme.in_src = IMEALG_DMA2IME;
            OneIme.bil = IMEALG_BICUBIC;
            OneIme.in_addr.y_addr  = (UINT32)gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.in_addr.cb_addr = (UINT32)gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.in_addr.cr_addr = (UINT32)gImgEffectParam.ARFPreQV[Index].YAddr[YCC_OP_MAP];
            OneIme.format.in_format = IMEALG_INYUV422;
            OneIme.in_size.h_size = GetIPPSizeInfor(_QVPreHsize)-16;
            OneIme.in_size.v_size = GetIPPSizeInfor(_QVPreVsize);
            OneIme.line_ofs.in_lineofs = GetIPPSizeInfor(_QVPreLineOffset);
            OneIme.path_num = IMEALG_1PATHY;
            OneIme.p1_out0_addr.y_addr  = P1YAddr ;
            OneIme.p1_out0_addr.cb_addr = P1CbAddr ;
            OneIme.p1_out0_addr.cr_addr = P1CrAddr ;
            OneIme.format.out1_format = IMEALG_OUTYUV422_COS;
            OneIme.p1_out_size.h_size = GetIPPSizeInfor(_ImeCapOutHsize);
            OneIme.p1_out_size.v_size = GetIPPSizeInfor(_ImeCapOutVsize);
            OneIme.line_ofs.out1_lineofs = GetIPPSizeInfor(_ImeCapOutLineOffset);
            IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
            IMEOpenObj.FP_IMEISR_CB = NULL;
            ime_open(&IMEOpenObj);
            ime_setMode(&OneIme);
            ime_clrIntFlag();
            ime_setStart();
            ime_waitFlagFrameEnd(IME_NO_CLEAR_FLAG);
            ime_setStop();
            ime_close();

            grph_setImg1(OneIme.p1_out0_addr.cb_addr,
                         OneIme.line_ofs.out1_lineofs/2,
                         OneIme.p1_out_size.v_size,
                         OneIme.p1_out_size.h_size/2);

            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);

            grph_setImg1(OneIme.p1_out0_addr.cr_addr,
                         OneIme.line_ofs.out1_lineofs/2,
                         OneIme.p1_out_size.v_size,
                         OneIme.p1_out_size.h_size/2);
#endif
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_TEXT_COPY, 0x80808080);
        }
        break;

        case IE_INDEX_ARF_FantasticFocus:
        break;

        case IE_INDEX_ARF_RoughMonoChorme:
        break;
    }
    grph_disableDMA();
    grph_swReset();
    grph_close();
}


void IE_MultiGammaLUT(UINT32* CHAddr,
                      UINT8** CHGammaLUT,
                      UINT32 CHNum,
                      UINT32 SrcAddr,
                      UINT32 SrcHsize,
                      UINT32 SrcVsize)
{
    UINT32  SrcData, SrcData0, SrcData1, SrcData2, SrcData3;
    UINT32  DstData, DstData0, DstData1, DstData2, DstData3;
    UINT32  i, j, k;

    for(j=0;j<SrcVsize;j++)
    {
        for(i=0;i<SrcHsize;i+=4)
        {
            SrcData=(*(UINT32*)(SrcAddr+i));
            SrcData0=(SrcData&0x000000ff);
            SrcData1=(SrcData&0x0000ff00)>>8;
            SrcData2=(SrcData&0x00ff0000)>>16;
            SrcData3=(SrcData&0xff000000)>>24;

            for(k=0;k<CHNum;k++)
            {
                DstData0=CHGammaLUT[k][SrcData0];
                DstData1=CHGammaLUT[k][SrcData1];
                DstData2=CHGammaLUT[k][SrcData2];
                DstData3=CHGammaLUT[k][SrcData3];
                DstData=( DstData0     &0x000000ff)|
                        ((DstData1<<8 )&0x0000ff00)|
                        ((DstData2<<16)&0x00ff0000)|
                        ((DstData3<<24)&0xff000000);
                (*(UINT32*)(CHAddr[k]+i))=DstData;
            }
        }

        SrcAddr+=SrcHsize;
        for(k=0;k<CHNum;k++)
            CHAddr[k] += SrcHsize;
    }
}

void IE_SetShadingJPG(ImgEffectShadingJPG* IE_ShadingJPG)
{
    gImgEffectShadingJPG.BitStreamAddr = IE_ShadingJPG->BitStreamAddr;
    gImgEffectShadingJPG.BitStreamSize = IE_ShadingJPG->BitStreamSize;
    gImgEffectShadingJPG.Hsize = IE_ShadingJPG->Hsize;
    gImgEffectShadingJPG.Vsize = IE_ShadingJPG->Vsize;
    gImgEffectShadingJPG.LineOffset = IE_ShadingJPG->LineOffset;
}

//#NT#2009/11/18#YC Peng -begin
//#fix JPG shading buffer size bug
UINT32 IE_GetShadingJPGBufSize(void)
{
    return  ( gImgEffectShadingJPG.Vsize * gImgEffectShadingJPG.LineOffset );
}
//#NT#2009/11/18#YC Peng -end
