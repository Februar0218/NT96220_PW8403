#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "ErrorNo.h"
#include "sie_lib.h"
#include "ime_lib.h"
#include "sd_lib.h"
#include "AlbumShot.h"
#include "ImageEffect.h"
#include "IPPBuffer.h"
#include "IPPManager.h"
#include "sen_common.h"
#include "jpgenc.h"         //for 220 Jpg Eng include in ipp
#include "DataStamp.h"
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
// This area is for ippbuf stuff

// Message APIs
#define IppBuff_DEBUG   ENABLE
#if IppBuff_DEBUG
#define ippbuf_msg(...) debug_msg("IPPBUFFER: "__VA_ARGS__)
#else
#define ippbuf_msg(...)
#endif
//define
#define ImeSize(h_y,v,h_cc,fmt) ( (fmt>FMT422)?(h_y*v + 2*h_cc*(v/2)):(h_y*v + 2*h_cc*(v/1)) )
//#define ECSTBLSIZE(MAP) ((MAP>ECSMAP49x49)?(65*65):((MAP<ECSMAP49x49)?(33*33):(49*49))) // removed by Harry
//static func
static BOOL ippbuf_MinDramsizeCheck_Cap(UINT _Size,UINT* _MinSize);
static IPPBUF_RST ippbuf_SampleAssigner_Cap_Normal(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);
static IPPBUF_RST ippbuf_SampleAssigner_Cap_AlbumShot(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);
static IPPBUF_RST ippbuf_SampleAssigner_Cap_CoupleShot(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);
static IPPBUF_RST ippbuf_SampleAssigner_Cap_PreShot(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);
static IPPBUF_RST ippbuf_SampleAssigner_Cap_Normal_Frame(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);
static IPPBUF_RST ippbuf_SampleAssigner_ContF2F(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);
static IPPBUF_RST ippbuf_SampleAssigner_ContS2F(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);
static IPPBUF_RST ippbuf_SampleAssigner_ContS2S(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo);

// Static global variable/struct
//#NT#2012/08/09#KS Hung -begin
//#NT#Set the JPG&RAW Buffer overlap or not
static BOOL g_bJpgRawBufOverlap = FALSE;
//#NT#2012/08/09#KS Hung -end
static BOOL ippbuf_ResetFlag = ENABLE;
static UINT32 ippbuf_BufObject[IPPBUF_MAX_INDEX]={0};
static FPIPPBUFASSIGNER ippbuf_AssignerObject[IPPBUF_MODE_MAX_INDEX] =
{
    ippbuf_SampleAssigner_ContF2F,
    ippbuf_SampleAssigner_ContS2F,
    ippbuf_SampleAssigner_ContS2S,
    ippbuf_SampleAssigner_Cap_Normal,
    ippbuf_SampleAssigner_Cap_AlbumShot,
    ippbuf_SampleAssigner_Cap_CoupleShot,
    ippbuf_SampleAssigner_Cap_PreShot,
    ippbuf_SampleAssigner_Cap_Normal_Frame
};

// Small tool
#define ippbuf_WordAlign(x) (((x)+3)/4*4)
#define IPPBUF_NULL 0xffffffff
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This area is for ippbuf external arrangement
void ippbuf_SetResetFlag(BOOL Flag)
{
    ippbuf_ResetFlag = Flag;
}

BOOL ippbuf_GetResetFlag(void)
{
    return ippbuf_ResetFlag;
}
#if 0
UINT32 ippbuf_Arrange(void)
{
    UINT32 i;
    UINT32 AllocSize, MinAddr;
    UINT32 BufAddr, BufSize;

    AllocSize = 0;
    MinAddr = 0xffffffff;
    for (i = 0; i < IPPBUF_RES_MAX_INDEX; i ++)
    {
        ippbuf_GetResource(i, &BufAddr, &BufSize);
        if (BufAddr != IPPBUF_NULL)
        {
            MinAddr = (MinAddr < BufAddr) ? MinAddr : BufAddr;
            ippbuf_SetBuf(IPPBUF_TOTAL_START, MinAddr);
            AllocSize += BufSize;
        }
    }
    ippbuf_SetBuf(IPPBUF_TOTAL_END, (MinAddr + AllocSize));

    //check memory is continuous or not
    if ((ippbuf_GetBuf(IPPBUF_PRV_START) != IPPBUF_NULL) && (ippbuf_GetBuf(IPPBUF_CAP_START) != IPPBUF_NULL))
    {
        if ((ippbuf_GetBuf(IPPBUF_PRV_START) == ippbuf_GetBuf(IPPBUF_CAP_END)) ||
            (ippbuf_GetBuf(IPPBUF_CAP_START) == ippbuf_GetBuf(IPPBUF_PRV_END)))
        {
            return IPPBUF_E_OK;
        }
        ippbuf_msg("ippbuf_Arrange: continuous mode fail\r\n");
        return IPPBUF_E_OUTOFBUFF;

    }
    return IPPBUF_E_OK;
}
#endif
//IPL frame to frame
static IPPBUF_RST ippbuf_SampleAssigner_ContF2F(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 BuffCurrPtr=BufAddr;
    UINT32 TmpAddr = BufAddr;
    UINT32 tmpsize;
    IPPBUF_RST rst;

    //ippbuf_SetBuf(IPPBUF_PRV_START, BuffCurrPtr);
    //ippbuf_SetBuf(IPPBUF_PRV_END, BuffCurrPtr+BufSize);
    ippbuf_msg("F2F:Start : 0x%x End : 0x%x\r\n",BufAddr,BuffCurrPtr+BufSize);

    //IME Path-1 Img output
    tmpsize = ImeSize(pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeOutV[ImePath1],pInfo->ImeOutCCLineOffset[ImePath1],pInfo->Ime_Fmt[ImePath1]);
    ippbuf_msg("        ImePath1 size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeOutV[ImePath1],pInfo->Ime_Fmt[ImePath1]);
    TmpAddr = BuffCurrPtr;
    switch(pInfo->ImeBufferNum[ImePath1])
    {
        case IME_OUTPUT_PPB_3:
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1_3, TmpAddr + 2*tmpsize);
            BuffCurrPtr += tmpsize;
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1_2, TmpAddr + tmpsize);
            BuffCurrPtr += tmpsize;
        case IME_OUTPUT_PPB_1:
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1, TmpAddr);
            BuffCurrPtr += tmpsize;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        break;
    }
    ippbuf_msg("        ImePath1 buferrend : 0x%x\r\n",BuffCurrPtr);
    //IME Path-2 Img output
    if(pInfo->ImePath2Enable)
    {
        tmpsize = ImeSize(pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->ImeOutCCLineOffset[ImePath2],pInfo->Ime_Fmt[ImePath2]);
        ippbuf_msg("        ImePath2 size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->Ime_Fmt[ImePath2]);
        TmpAddr = BuffCurrPtr;
        switch(pInfo->ImeBufferNum[ImePath2])
        {
            case IME_OUTPUT_PPB_4:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_3, TmpAddr + 3*tmpsize);
                BuffCurrPtr += tmpsize;
            case IME_OUTPUT_PPB_3:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_3, TmpAddr + 2*tmpsize);
                BuffCurrPtr += tmpsize;
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_2, TmpAddr + tmpsize);
                BuffCurrPtr += tmpsize;
            case IME_OUTPUT_PPB_1:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2, TmpAddr);
                BuffCurrPtr += tmpsize;
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
        }
        ippbuf_msg("        ImePath2 buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    //SIE Img output
    tmpsize = pInfo->CropH*pInfo->SieBitDepth/8*pInfo->CropV;
    TmpAddr = BuffCurrPtr;
    ippbuf_msg("        sie size : %d %d %d\r\n",pInfo->CropH,pInfo->CropV,pInfo->SieBitDepth);
    switch(pInfo->SieBufferNum)
    {
        case TWO_OUTPUT_BUF:
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_2, TmpAddr + tmpsize);
            BuffCurrPtr += tmpsize;
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_1, TmpAddr);
            BuffCurrPtr += tmpsize;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        break;
    }
    ippbuf_msg("        sie buferrend : 0x%x\r\n",BuffCurrPtr);
    //ECS table
    //ippbuf_SetBuf(IPPBUF_ECS, BuffCurrPtr);
    //BuffCurrPtr += pInfo->EcsMapSize;
    //BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    //ippbuf_msg("        ecs buferrend : 0x%x\r\n",BuffCurrPtr);
    //CA
    ippbuf_SetBuf(IPPBUF_CA,BuffCurrPtr);
    BuffCurrPtr += SIE_SubOut_H * SIE_SubOut_V;
    ippbuf_msg("        ca buferrend : 0x%x\r\n",BuffCurrPtr);

    if(GetIPPFuncInfor(VID_Datestamp_EN))
    {
        //stamp
        DS_STAMP_INFOR VidStampInfo = {0};
        datastamp_GetStampInfor(&VidStampInfo);
        ippbuf_SetBuf(IPPBUF_VID_DATA_STAMP, BuffCurrPtr);
        BuffCurrPtr += (VidStampInfo.Width*VidStampInfo.Height*2);
        ippbuf_msg("        STAMP buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    //User JPG
    if(GetIPPFuncInfor(JPG_EN))
    {
        ippbuf_SetBuf(IPPBUF_JPGADDR, BuffCurrPtr);
        BuffCurrPtr += (JpegEncGetData(JPG_MAX_FRAME_SIZE)* JPG_BUF_NUM);
        ippbuf_msg("        JPG buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_PRV_CURREND, BuffCurrPtr);

    ////Final Check
    if(BuffCurrPtr>(BufAddr+BufSize))
    {
        debug_err(("ippbuf_SampleAssigner_f2f: Buf. not enough 0x%x<0x%x\r\n", BufSize, (BuffCurrPtr-BufAddr)));
        rst.er_code = E_BOVR;
    }
    else
        rst.er_code = E_OK;
    rst.uibuffer = BuffCurrPtr-BufAddr;
    return rst;
}

//IPL slice to slice,must have jpg dcout to support
static IPPBUF_RST ippbuf_SampleAssigner_ContS2S(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 BuffCurrPtr = BufAddr;
    UINT32 TmpAddr = BufAddr;
    UINT32 tmpsize;
    IPPBUF_RST rst;

    //ippbuf_SetBuf(IPPBUF_PRV_START, BuffCurrPtr);
    //ippbuf_SetBuf(IPPBUF_PRV_END, BuffCurrPtr+BufSize);
    ippbuf_msg("S2S:Start : 0x%x End : 0x%x\r\n",BufAddr,BuffCurrPtr+BufSize);
    //IME Path-1 Img output
    tmpsize = ImeSize(pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeOutV[ImePath1],pInfo->ImeOutCCLineOffset[ImePath1],pInfo->Ime_Fmt[ImePath1]);
    TmpAddr = BuffCurrPtr;
    ippbuf_msg("        ImePath1 size : %d %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeOutCCLineOffset[ImePath1],pInfo->ImeOutV[ImePath1],pInfo->Ime_Fmt[ImePath1]);
    switch(pInfo->ImeBufferNum[ImePath1])
    {
        case IME_OUTPUT_PPB_3:
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1_3, TmpAddr + 2*tmpsize);
            BuffCurrPtr += tmpsize;
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1_2, TmpAddr + tmpsize);
            BuffCurrPtr += tmpsize;
        case IME_OUTPUT_PPB_1:
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1, TmpAddr);
            BuffCurrPtr += tmpsize;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        break;
    }
    ippbuf_msg("        imepath1 buferrend : 0x%x\r\n",BuffCurrPtr);
    //IME Path-2 Img output
    if(pInfo->ImePath2Enable)
    {
        tmpsize = ImeSize(pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeSliceH,pInfo->ImeOutCCLineOffset[ImePath2],pInfo->Ime_Fmt[ImePath2]);
        TmpAddr = BuffCurrPtr;
        ippbuf_msg("        ImePath2 size : %d %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutCCLineOffset[ImePath2],pInfo->ImeSliceH,pInfo->Ime_Fmt[ImePath2]);
        switch(pInfo->ImeBufferNum[ImePath2])
        {
            case IME_OUTPUT_PPB_5:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_5, TmpAddr + 4*tmpsize);
                BuffCurrPtr += tmpsize;
            case IME_OUTPUT_PPB_4:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_4, TmpAddr + 3*tmpsize);
                BuffCurrPtr += tmpsize;
            case IME_OUTPUT_PPB_3:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_3, TmpAddr + 2*tmpsize);
                BuffCurrPtr += tmpsize;
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_2, TmpAddr + tmpsize);
                BuffCurrPtr += tmpsize;
            case IME_OUTPUT_PPB_1:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2, TmpAddr);
                BuffCurrPtr += tmpsize;
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
        }
        ippbuf_msg("        imepath2 buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    //SIE Img output
    tmpsize = pInfo->CropH*pInfo->SieBitDepth/8*pInfo->SieSliceH;//reference dztbl
    TmpAddr = BuffCurrPtr;
    ippbuf_msg("        sie size : %d %d %d\r\n",pInfo->CropH,pInfo->SieSliceH,pInfo->SieBitDepth);
    switch(pInfo->SieBufferNum)
    {
        case THREE_OUTPUT_BUF:
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_3, TmpAddr + 2*tmpsize);
            BuffCurrPtr += tmpsize;
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_2, TmpAddr + tmpsize);
            BuffCurrPtr += tmpsize;
        case ONE_OUTPUT_BUF:
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_1, TmpAddr);
            BuffCurrPtr += tmpsize;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        break;
    }
    ippbuf_msg("        sie buferrend : 0x%x\r\n",BuffCurrPtr);
    //ECS table
    //ippbuf_SetBuf(IPPBUF_ECS, BuffCurrPtr);
    //BuffCurrPtr += pInfo->EcsMapSize;
    //BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    //ippbuf_msg("        ecs buferrend : 0x%x\r\n",BuffCurrPtr);
    //CA
    ippbuf_SetBuf(IPPBUF_CA,BuffCurrPtr);
    BuffCurrPtr += SIE_SubOut_H * SIE_SubOut_V;
    ippbuf_msg("        ca buferrend : 0x%x\r\n",BuffCurrPtr);

    if(GetIPPFuncInfor(VID_Datestamp_EN))
    {
        //stamp
        ippbuf_SetBuf(IPPBUF_VID_DATA_STAMP, 0x0);
        ippbuf_msg("        STAMP buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    //User JPG
    if(GetIPPFuncInfor(JPG_EN))
    {
        ippbuf_SetBuf(IPPBUF_JPGADDR, BuffCurrPtr);
        BuffCurrPtr += (JpegEncGetData(JPG_MAX_FRAME_SIZE)* JPG_BUF_NUM);
        ippbuf_msg("        JPG buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    //#NT#2010/12/27#ethanlau -begin
    //#NT#ipe va only output to ipe registor
    //Preview IPE VA output
    //ippbuf_SetBuf(IPPBUF_PRV_VA, BuffCurrPtr);
    //BuffCurrPtr += (IPE_MAXVAWORD << 2);
    //#NT#2010/12/27#ethanlau -end


    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_PRV_CURREND, BuffCurrPtr);

    ////Final Check
    if(BuffCurrPtr>(BufAddr+BufSize))
    {
        debug_err(("ippbuf_SampleAssigner_s2s: Buf. not enough 0x%x<0x%x\r\n", BufSize, (BuffCurrPtr-BufAddr)));
        rst.er_code = E_BOVR;
    }
    else
        rst.er_code = E_OK;
    rst.uibuffer = BuffCurrPtr-BufAddr;
    return rst;
}

//IPL slice to frame
static IPPBUF_RST ippbuf_SampleAssigner_ContS2F(UINT32 BufAddr, UINT32 BufSize, pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 BuffCurrPtr=BufAddr;
    UINT32 TmpAddr = BufAddr;
    UINT32 tmpsize;
    IPPBUF_RST rst;
    //ippbuf_SetBuf(IPPBUF_PRV_START, BuffCurrPtr);
    //ippbuf_SetBuf(IPPBUF_PRV_END, BuffCurrPtr+BufSize);
    ippbuf_msg("S2F:Start : 0x%x End : 0x%x\r\n",BufAddr,BuffCurrPtr+BufSize);

    //IME Path-1 Img output
    tmpsize = ImeSize(pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeOutV[ImePath1],pInfo->ImeOutCCLineOffset[ImePath1],pInfo->Ime_Fmt[ImePath1]);
    TmpAddr = BuffCurrPtr;
    ippbuf_msg("        ImePath1 size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeOutV[ImePath1],pInfo->Ime_Fmt[ImePath1]);
    switch(pInfo->ImeBufferNum[ImePath1])
    {
        case IME_OUTPUT_PPB_3:
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1_3, TmpAddr + 2*tmpsize);
            BuffCurrPtr += tmpsize;
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1_2, TmpAddr+ tmpsize);
            BuffCurrPtr += tmpsize;
        case IME_OUTPUT_PPB_1:
            ippbuf_SetBuf(IPPBUF_PRV_IME_P1, TmpAddr);
            BuffCurrPtr += tmpsize;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        break;
    }
    ippbuf_msg("        imepath1 buferrend : 0x%x\r\n",BuffCurrPtr);
    //IME Path-2 Img output
    if(pInfo->ImePath2Enable)
    {
        tmpsize = ImeSize(pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->ImeOutCCLineOffset[ImePath2],pInfo->Ime_Fmt[ImePath2]);
        TmpAddr = BuffCurrPtr;
        ippbuf_msg("        ImePath2 size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->Ime_Fmt[ImePath2]);
        switch(pInfo->ImeBufferNum[ImePath2])
        {
            case IME_OUTPUT_PPB_4:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_4, TmpAddr + 3*tmpsize);
                BuffCurrPtr += tmpsize;
            case IME_OUTPUT_PPB_3:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_3, TmpAddr + 2*tmpsize);
                BuffCurrPtr += tmpsize;
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2_2, TmpAddr + tmpsize);
                BuffCurrPtr += tmpsize;
            case IME_OUTPUT_PPB_1:
                ippbuf_SetBuf(IPPBUF_PRV_IME_P2, TmpAddr);
                BuffCurrPtr += tmpsize;
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
            break;
        }
        ippbuf_msg("        imepath2 buferrend : 0x%x\r\n",BuffCurrPtr);
    }
    //SIE Img output
    tmpsize = pInfo->CropH*pInfo->SieBitDepth/8*pInfo->SieSliceH;//reference dztbl
    TmpAddr = BuffCurrPtr;
    ippbuf_msg("        sie size : %d %d %d\r\n",pInfo->CropH,pInfo->SieSliceH,pInfo->SieBitDepth);
    switch(pInfo->SieBufferNum)
    {
        case THREE_OUTPUT_BUF:
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_3, TmpAddr + 2*tmpsize);
            BuffCurrPtr += tmpsize;
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_2, TmpAddr + tmpsize);
            BuffCurrPtr += tmpsize;
        case ONE_OUTPUT_BUF:
            ippbuf_SetBuf(IPPBUF_PRV_SIERAW_1, TmpAddr);
            BuffCurrPtr += tmpsize;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        break;
    }
    ippbuf_msg("        sie buferrend : 0x%x\r\n",BuffCurrPtr);
    //ECS table
    //ippbuf_SetBuf(IPPBUF_ECS, BuffCurrPtr);
    //BuffCurrPtr += pInfo->EcsMapSize;
    //BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    //ippbuf_msg("        ecs buferrend : 0x%x\r\n",BuffCurrPtr);
    //CA
    ippbuf_SetBuf(IPPBUF_CA,BuffCurrPtr);
    BuffCurrPtr += SIE_SubOut_H * SIE_SubOut_V;
    ippbuf_msg("        ca buferrend : 0x%x\r\n",BuffCurrPtr);

    if(GetIPPFuncInfor(VID_Datestamp_EN))
    {
        //stamp
        DS_STAMP_INFOR VidStampInfo = {0};
        datastamp_GetStampInfor(&VidStampInfo);
        ippbuf_SetBuf(IPPBUF_VID_DATA_STAMP, BuffCurrPtr);
        BuffCurrPtr += (VidStampInfo.Width*VidStampInfo.Height*2);
        ippbuf_msg("        STAMP buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    //User JPG
    if(GetIPPFuncInfor(JPG_EN))
    {
        ippbuf_SetBuf(IPPBUF_JPGADDR, BuffCurrPtr);
        BuffCurrPtr += (JpegEncGetData(JPG_MAX_FRAME_SIZE)* JPG_BUF_NUM);
        ippbuf_msg("        JPG buferrend : 0x%x\r\n",BuffCurrPtr);
    }

    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_PRV_CURREND, BuffCurrPtr);

    ////Final Check
    if(BuffCurrPtr>(BufAddr+BufSize))
    {
        debug_err(("ippbuf_SampleAssigner_s2f: Buf. not enough 0x%x<0x%x\r\n", BufSize, (BuffCurrPtr-BufAddr)));
        rst.er_code = E_BOVR;
    }
    else
        rst.er_code = E_OK;
    rst.uibuffer = BuffCurrPtr-BufAddr;
    return rst;
}

//support 8m dram, No FQV, No alpha blending
IPPBUF_RST ippbuf_Cap_MinCase(UINT32 BufAddr, UINT32 BufSize,pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 BuffCurrPtr,_addr;
    UINT32 JpgBitstreamSize;
    UINT32 _ImeSliceSize;
    PImageAlgInfo _pimginfo;
    IPPBUF_RST rst = {0};
    BOOL bJpgStrAddrFlg = FALSE;//True when header + screen + thumb is bigger than slice buf,header will be moved below to slice buf
    _pimginfo = UI_GetParameter();

    ////Initial Check
    BuffCurrPtr = BufAddr;
#if 0
    ////Main Assignment
    //Capture Chroma-Filter Buffer
    ippbuf_SetBuf(IPPBUF_CAP_CHROMA_FILTER, BuffCurrPtr);

    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * CHROMA_BUF_NUM / 2);
#endif
    //Capture ECS table
    //ippbuf_SetBuf(IPPBUF_ECS, BuffCurrPtr);
    //BuffCurrPtr += pInfo->EcsMapSize;
    //BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);

    //exif header is overlap with ime_slice_buf
    //exif header
    _addr = BuffCurrPtr;
    ippbuf_SetBuf(IPPBUF_CAP_HEADER, _addr);
    _addr +=JpgHeaderSize;
    ippbuf_msg("    Header : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_HEADER));

    //QV&Thumb_YCC is from FQV , and overlap with ime_slice_buf (produce after bitstream ready)
    //screennail
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, _addr);
    ippbuf_msg("    Screennail size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->Ime_Fmt[ImePath2]);
    _addr += (pInfo->ImeOutLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2]+
    2*pInfo->ImeOutCCLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2]/(pInfo->Ime_Fmt[ImePath2]/2));
    ippbuf_msg("    Screennail : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_SCREEN));

    //thumbnail
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, _addr);
    ippbuf_msg("    Thumbnail size : %d %d\r\n",CAP_THUMB_H,CAP_THUMB_V);
    _addr += (CAP_THUMB_H * CAP_THUMB_V + CAP_THUMB_H * CAP_THUMB_V);//thumb always 422 fmt
    ippbuf_msg("    Thumbnail : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_THUMBNAIL));

    //8M Dram have no frame mode
    if(GetIPPFuncInfor(FRAME_MODE_EN))
        SetIPPFuncInfor(FRAME_MODE_EN,DISABLE);
    //Capture IME path-1 Output (Slice-Buffer)
    ippbuf_SetBuf(IPPBUF_CAP_SLC_BUF, BuffCurrPtr);
    ippbuf_msg("    SLC_BUF size : %d %d %d(%d)\r\n",pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeSliceH,pInfo->Ime_Fmt[ImePath1],(pInfo->ImeBufferNum[ImePath1]+1));
    //IPPBUF_CAP_QV_PRE use ime slice buffer and then scaling down to FQV
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF));
    _ImeSliceSize = (pInfo->ImeOutLineOffset[ImePath1] * pInfo->ImeSliceH * (pInfo->ImeBufferNum[ImePath1]+1))
        + 2*(pInfo->ImeOutCCLineOffset[ImePath1] * pInfo->ImeSliceH * (pInfo->ImeBufferNum[ImePath1]+1))/(pInfo->Ime_Fmt[ImePath1]/2);
    BuffCurrPtr += _ImeSliceSize;
    ippbuf_msg("    SLC_BUF : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF));

    //capture IFE path-1 output buf
    if(GetIPPFuncInfor(IFE_YCC_EN) || GetIPPFuncInfor(IFE_Y_EN))
    {
        SetIPPFuncInfor(IFE_YCC_EN, DISABLE);
        SetIPPFuncInfor(IFE_Y_EN, DISABLE);
        debug_err(("^R 8m dram don't support IFE YCC\r\n"));
        /*
        ippbuf_SetBuf(IPPBUF_CAP_IFESLICE, BuffCurrPtr);
        ippbuf_msg("    IFED2D size(Max Repeat:%d) : %d %d\r\n",IFE_YCCD2DMAX,(pInfo->ImeOutLineOffset[ImePath1]+IFE_OVERLAP_H*(IFE_YCCD2DMAX-1)),(pInfo->ImeSliceH + (IFE_YCCD2DMAX*2*IFE_OVERLAP_V) ));
        BuffCurrPtr += ((pInfo->ImeOutLineOffset[ImePath1]+IFE_OVERLAP_H*(IFE_YCCD2DMAX-1)) * (pInfo->ImeSliceH + (IFE_YCCD2DMAX*2*IFE_OVERLAP_V) ));
        ippbuf_msg("    IFED2D : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_IFESLICE));

        ippbuf_SetBuf(IPPBUF_CAP_IFESLICE_SRC_Y, BuffCurrPtr);
        ippbuf_msg("    IFED2D size(Boundry Y) : %d %d\r\n",pInfo->ImeOutLineOffset[ImePath1],(IFE_YCCD2DMAX*IFE_OVERLAP_V));
        BuffCurrPtr += (pInfo->ImeOutLineOffset[ImePath1] * IFE_YCCD2DMAX*IFE_OVERLAP_V );//Y
        ippbuf_msg("    IFED2D(Boundry Y) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_Y));
        ippbuf_SetBuf(IPPBUF_CAP_IFESLICE_SRC_CC, BuffCurrPtr);
        ippbuf_msg("    IFED2D size(Boundry CC) : %d %d\r\n",pInfo->ImeOutLineOffset[ImePath1]/2,(IFE_YCCD2DMAX*IFE_OVERLAP_V));
        BuffCurrPtr += (pInfo->ImeOutLineOffset[ImePath1]/2 * IFE_YCCD2DMAX*IFE_OVERLAP_V );//Cb
        BuffCurrPtr += (pInfo->ImeOutLineOffset[ImePath1]/2 * IFE_YCCD2DMAX*IFE_OVERLAP_V );//Cr
        ippbuf_msg("    IFED2D(Boundry CC) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_CC));
        */
    }

    //header + screen + thumb is bigger than slice buf,header will be moved just next to slice buf
    //otherwise,header is in slice buf
    if(_addr > BuffCurrPtr)
    {
        //if screen + thumb is bigger than slice buf
        if((_addr - JpgHeaderSize)> BuffCurrPtr)
        {
            debug_err(("^R %s: slice buf is smaller than screen + thumb\r\n",__func__));
            rst.er_code = E_BOVR;
            goto IPP_END;
        }
        bJpgStrAddrFlg = TRUE;
        ippbuf_SetBuf(IPPBUF_CAP_HEADER, BuffCurrPtr);
        BuffCurrPtr +=JpgHeaderSize;
        ippbuf_msg("    Re allocate Header : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_HEADER));
    }

    //Capture IME QV Image Buffer (Display img), Disable in here
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, ippbuf_GetBuf(IPPBUF_CAP_QV_SCREEN));
    ippbuf_msg("    FQV(%d) : Addr:0x%x\r\n",GetIPPFuncInfor(FQV_EN),ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY));

    //Capture Data Stamp Alpha Blending Buffer,disable alpha blending , only use colorkey
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, 0);
    ippbuf_msg("    AlphaBlending : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_DATA_STAMP));

    ippbuf_msg("\r\nJPG and RAW overlap\r\n");

    //FQV Raw data is from sie raw
    //Capture Raw ,only take care of one shot only
    _addr = ippbuf_GetBuf(IPPBUF_CAP_END);

    if(GetIPPFuncInfor(IFE_RAW_EN))
        _addr -= ((pInfo->CropH * pInfo->SieBitDepth / 8 + IFE_OVERLAP_H) * pInfo->CropV) ;
    else
        _addr -= pInfo->CropH * pInfo->CropV * pInfo->SieBitDepth / 8;

    ippbuf_SetBuf(IPPBUF_CAP_RAW_1, _addr);
    ippbuf_msg("    SieRaw : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_RAW_1));

    if(_addr < (ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF) + _ImeSliceSize + CAP_JPGLeadingMinSize))
    {
        rst.er_code = E_BOVR;
        debug_err(("Buf. not enough 0x%x<0x%x\r\n", _addr, (ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF) + _ImeSliceSize + CAP_JPGLeadingMinSize)));
        goto IPP_END;
    }
    else
    {
        rst.er_code = E_OK;
    }

    //Jpg buffer
    if(bJpgStrAddrFlg)
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1, (ippbuf_GetBuf(IPPBUF_CAP_HEADER) + JpgHeaderSize));
    else
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1, (ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF) + _ImeSliceSize));

    JpgBitstreamSize = ippbuf_GetBuf(IPPBUF_CAP_END) - ippbuf_GetBuf(IPPBUF_CAP_JPG_1);
    JpgBitstreamSize -= CAP_DEBUGINFO_SIZE;
    ippbuf_SetBuf(IPPBUF_CAP_JPG_BTSSIZE, JpgBitstreamSize);

    if(_pimginfo->RamDskEn)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1, _pimginfo->RamDskAddr);
        JpgBitstreamSize = _pimginfo->RamDskSize - CAP_DEBUGINFO_SIZE;
        ippbuf_SetBuf(IPPBUF_CAP_JPG_BTSSIZE, JpgBitstreamSize);
    }
    ippbuf_msg("    Jpg : Addr:0x%x Size:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_JPG_1),JpgBitstreamSize);

IPP_END:
    return rst;
}
//For slice mode condition
IPPBUF_RST ippbuf_Cap_NormalCase(UINT32 BufAddr, UINT32 BufSize,pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 i;
    UINT32 BuffCurrPtr,tempBuffCurrPtr;
    UINT32 BuffRemainSize;
    UINT32 JpgBitstreamSize;//, SIEOutSize;
    UINT8  RawNum, JpgNum;
    UINT32 _ImeSliceSize,_FQV_YCC_Size=0;
    IPPBUF_RST rst;
    PImageAlgInfo _pimginfo;
    pIFE_FILTER     pife_para = NULL;
    UINT32 repeat1=0,repeat2=0;
    _pimginfo = UI_GetParameter();
    ////Extra Prepare
    RawNum = Sen_GetRawNum();
    JpgNum = Sen_GetJpgNum();

    ////Initial Check
    BuffCurrPtr = BufAddr;

#if 0
    ////Main Assignment
    //Capture Chroma-Filter Buffer
    ippbuf_SetBuf(IPPBUF_CAP_CHROMA_FILTER, BuffCurrPtr);

    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * CHROMA_BUF_NUM / 2);
#endif
    //Capture ECS table
    //ippbuf_SetBuf(IPPBUF_ECS, BuffCurrPtr);
    //BuffCurrPtr += pInfo->EcsMapSize;
    //BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);

    //exif header
    ippbuf_SetBuf(IPPBUF_CAP_HEADER, BuffCurrPtr);
    BuffCurrPtr += JpgHeaderSize;
    ippbuf_msg("    ExifHeader : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_HEADER));
    //Capture IME path-1 Output (Slice-Buffer)
    ippbuf_SetBuf(IPPBUF_CAP_SLC_BUF, BuffCurrPtr);
    ippbuf_msg("    SLC_BUF size : %d %d %d(%d)\r\n",pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeSliceH,pInfo->Ime_Fmt[ImePath1],(pInfo->ImeBufferNum[ImePath1]+1));
    _ImeSliceSize = (pInfo->ImeOutLineOffset[ImePath1] * pInfo->ImeSliceH * (pInfo->ImeBufferNum[ImePath1]+1))
        + 2*(pInfo->ImeOutCCLineOffset[ImePath1] * pInfo->ImeSliceH * (pInfo->ImeBufferNum[ImePath1]+1))/(pInfo->Ime_Fmt[ImePath1]/2);
    //#NT#2012/07/10#Harry -begin
    if(_ImeSliceSize < (160*1024))
        _ImeSliceSize = (160*1024);
    //#NT#2012/07/10#Harry -end
    BuffCurrPtr += _ImeSliceSize;
    ippbuf_msg("    SLC_BUF : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF));

    //capture IFE path-1 output buf
    //ethan_20110805_buf is allocated by ife repeat times
    if(GetIPPFuncInfor(IFE_YCC_EN) || GetIPPFuncInfor(IFE_Y_EN))
    {
        if(GetIPPFuncInfor(IFE_YCC_EN)==ENABLE)
        {
            UINT32 j=0;
            repeat1 = 0;
            pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_YCC_TABLE_ADDR);
            for(j=0; pife_para[j].repeat != 0xFF; j++)
            {
                repeat1 += pife_para[j].repeat;
            }
        }
        if(GetIPPFuncInfor(IFE_Y_EN)==ENABLE)
        {
            UINT32 j=0;
            repeat2 = 0;
            pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_Y_TABLE_ADDR);
            for(j=0; pife_para[j].repeat != 0xFF; j++)
            {
                repeat2 += pife_para[j].repeat;
            }
        }
        if(repeat1 < repeat2)
            repeat1 = repeat2;

        ippbuf_SetBuf(IPPBUF_CAP_IFESLICE, BuffCurrPtr);
        ippbuf_msg("    IFEYCCD2D size(Repeat:%d) : %d %d\r\n",repeat1,(pInfo->ImeOutLineOffset[ImePath1]+IFE_OVERLAP_H*(repeat1-1)),(pInfo->ImeSliceH + (repeat1*2*IFE_OVERLAP_V) ));
        BuffCurrPtr += ((pInfo->ImeOutLineOffset[ImePath1]+IFE_OVERLAP_H*(repeat1-1)) * (pInfo->ImeSliceH + (repeat1*2*IFE_OVERLAP_V) ));
        ippbuf_msg("    IFED2D : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_IFESLICE));

        ippbuf_SetBuf(IPPBUF_CAP_IFESLICE_SRC_Y, BuffCurrPtr);
        ippbuf_msg("    IFED2D size(Boundry Y) : %d %d\r\n",pInfo->ImeOutLineOffset[ImePath1],(repeat1*IFE_OVERLAP_V));
        BuffCurrPtr += (pInfo->ImeOutLineOffset[ImePath1] * repeat1*IFE_OVERLAP_V );//Y
        ippbuf_msg("    IFED2D(Boundry Y) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_Y));
        ippbuf_SetBuf(IPPBUF_CAP_IFESLICE_SRC_CC, BuffCurrPtr);
        ippbuf_msg("    IFED2D size(Boundry CC) : %d %d\r\n",pInfo->ImeOutCCLineOffset[ImePath1],(repeat1*IFE_OVERLAP_V));
        BuffCurrPtr += (pInfo->ImeOutCCLineOffset[ImePath1] * repeat1*IFE_OVERLAP_V );//Cb
        BuffCurrPtr += (pInfo->ImeOutCCLineOffset[ImePath1] * repeat1*IFE_OVERLAP_V );//Cr
        ippbuf_msg("    IFED2D(Boundry CC) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_IFESLICE_SRC_CC));
    }
    //Screennail
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    ippbuf_msg("    Screennail size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->Ime_Fmt[ImePath2]);
    BuffCurrPtr += (pInfo->ImeOutLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2] +
    2*pInfo->ImeOutCCLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2]/(pInfo->Ime_Fmt[ImePath2]/2));
    ippbuf_msg("    Screennail : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_SCREEN));

    //Thumbnail
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtr);
    //thumb always 422 fmt
    ippbuf_msg("    Thumbnail size : %d %d\r\n",CAP_THUMB_H,CAP_THUMB_V);
    BuffCurrPtr += (CAP_THUMB_H * CAP_THUMB_V + CAP_THUMB_H * CAP_THUMB_V);
    ippbuf_msg("    Thumbnail : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_THUMBNAIL));

    //Capture IME QV Image Buffer (Display img)
    if(GetIPPFuncInfor(FQV_EN))
    {
        _FQV_YCC_Size = (pInfo->ImeOutLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2] +
        2*pInfo->ImeOutCCLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2]/(pInfo->Ime_Fmt[ImePath2]/2));
        ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtr);
        ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);//not support 2-stage FQV
        ippbuf_msg("    FQV size(FrameMode) : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->Ime_Fmt[ImePath2]);
        BuffCurrPtr += _FQV_YCC_Size;
        ippbuf_msg("    FQV(QV_DISPLAY) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY));

        //encode dcout buf allocate.QV_PRE first,than QV_DISPLAY.
        if(!GetIPPFuncInfor(FQV_FRAMEMDOE_EN))
        {
            ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
            ippbuf_msg("    FQV size(DCOUT) : %d %d %d\r\n",GetIPPSizeInfor(_QVPreYLineOffset),GetIPPSizeInfor(_QVPreVsize),pInfo->Ime_Fmt[ImePath2]);
            BuffCurrPtr += (GetIPPSizeInfor(_QVPreYLineOffset)* GetIPPSizeInfor(_QVPreVsize) +
                            2*GetIPPSizeInfor(_QVPreUVLineOffset)* GetIPPSizeInfor(_QVPreVsize)/(pInfo->Ime_Fmt[ImePath2]/2));
            ippbuf_msg("    FQV(QV_PRE) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_PRE));
        }
    }

    tempBuffCurrPtr = BuffCurrPtr;

    //sie raw
    for (i = 0; i < RawNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_RAW_1 + i, BuffCurrPtr);
        if(GetIPPFuncInfor(IFE_RAW_EN))
        {
            UINT32 j=0;
            repeat1=0;
            pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_RAW_TABLE_ADDR);
            for(j=0; pife_para[j].repeat != 0xFF; j++)
            {
                repeat1 += pife_para[j].repeat;
            }
            BuffCurrPtr += ((pInfo->CropH * pInfo->SieBitDepth / 8 + repeat1*IFE_OVERLAP_H) * pInfo->CropV);
        }
        else
            BuffCurrPtr += pInfo->CropH * pInfo->CropV * pInfo->SieBitDepth / 8;

        ippbuf_msg("    RAW(%d) : Addr:0x%x(%d %d),IFE repeat:%d\r\n"
            ,i
            ,ippbuf_GetBuf(IPPBUF_CAP_RAW_1+i)
            ,pInfo->CropH * pInfo->SieBitDepth / 8,pInfo->CropV
            ,repeat1);
    }

    if(GetIPPFuncInfor(DFS_EN))
    {
        ippbuf_SetBuf(IPPBUF_CAP_RAW_DARKFRAME, BuffCurrPtr);
        BuffCurrPtr += pInfo->CropH * pInfo->CropV * 10 / 8;//DFS always 10-bit output
        ippbuf_msg("    DFS : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME));
    }

    //Capture Data Stamp Alpha Blending Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
    BuffCurrPtr += GetIPPSizeInfor(_ImeCapOutHsize) * pInfo->ImeSliceH;
    ippbuf_msg("    Alpha blending : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_DATA_STAMP));

    if(_pimginfo->RamDskEn)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1, _pimginfo->RamDskAddr);
        JpgBitstreamSize = _pimginfo->RamDskSize - CAP_DEBUGINFO_SIZE;
        ippbuf_SetBuf(IPPBUF_CAP_JPG_BTSSIZE, JpgBitstreamSize);
    }
    else
    {
        //Capture Jpg buffer
        BuffRemainSize = ippbuf_GetBuf(IPPBUF_CAP_END)-BuffCurrPtr;
        //inorder to get each jpg available size
        {
            UINT32 tmp;
            tmp = (BuffRemainSize / JpgNum);
            if(tmp > (JpgHeaderSize + CAP_DEBUGINFO_SIZE))
            {
                tmp = tmp - (JpgHeaderSize + CAP_DEBUGINFO_SIZE);
            }
            else
                tmp = 0;
            tmp -= (tmp % 4);
            JpgBitstreamSize = tmp;
        }
        ippbuf_SetBuf(IPPBUF_CAP_JPG_BTSSIZE, JpgBitstreamSize);
        for (i = 0; i < JpgNum; i ++)
        {
            ippbuf_SetBuf(IPPBUF_CAP_JPG_1+ i, BuffCurrPtr + JpgHeaderSize);
            BuffCurrPtr += (JpgBitstreamSize+(JpgHeaderSize + CAP_DEBUGINFO_SIZE));
            ippbuf_msg("    JPG(%d) : Addr:0x%x,size:0x%x\r\n",i,ippbuf_GetBuf(IPPBUF_CAP_JPG_1+i),JpgBitstreamSize);
        }

        //#NT#2012/08/09#KS Hung -begin
        //#NTK#2012/07/12#Robin Chen -begin
        //Because JPG Buffer only about 1M, modify JPG&RAW Buffer overlap to CDPD Project use
        if (g_bJpgRawBufOverlap)
        {
            UINT32 _rawsize;
            UINT32 maxidx;
            UINT32 pdzoomtbl;
            UINT32 (*DzTable)[6];


            pdzoomtbl = SenMode2Tbl(7/*SENSOR_MODE_FULL*/,&maxidx);
            DzTable = (UINT32(*)[6])pdzoomtbl;
            if(GetIPPFuncInfor(IFE_RAW_EN))
            {
                UINT32 j;
                repeat1=0;
                pife_para = (pIFE_FILTER)GetIPPFuncInfor(IFE_RAW_TABLE_ADDR);
                for(j=0; pife_para[j].repeat != 0xFF; j++)
                {
                    repeat1 += pife_para[j].repeat;
                }
                //_rawsize= ((pInfo->CropH * pInfo->SieBitDepth / 8 + repeat1*IFE_OVERLAP_H) * pInfo->CropV);
                _rawsize= ((DzTable[Get_DzoomIndex()-10][2] * pInfo->SieBitDepth / 8 + repeat1*IFE_OVERLAP_H) * DzTable[Get_DzoomIndex()-10][3] );
            }
            else
                //_rawsize= pInfo->CropH * pInfo->CropV * pInfo->SieBitDepth / 8;
                _rawsize= DzTable[Get_DzoomIndex()-10][2]  * DzTable[Get_DzoomIndex()-10][3]  * pInfo->SieBitDepth / 8;

            //if((JpgBitstreamSize<_rawsize*2/4)&&(JpgNum==1))
            {

                ippbuf_msg("---------->JPG&RAW Buffer Overlap<-----------\r\n");
                i = 0;
                BuffCurrPtr = tempBuffCurrPtr;
                //Capture Data Stamp Alpha Blending Buffer
                ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
                BuffCurrPtr += GetIPPSizeInfor(_ImeCapOutHsize) * pInfo->ImeSliceH;
                ippbuf_msg("    Alpha blending : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_DATA_STAMP));


                ippbuf_SetBuf(IPPBUF_CAP_JPG_1, BuffCurrPtr + JpgHeaderSize);
                JpgBitstreamSize = ippbuf_GetBuf(IPPBUF_CAP_END)-JpgHeaderSize-CAP_DEBUGINFO_SIZE;
                ippbuf_SetBuf(IPPBUF_CAP_JPG_BTSSIZE, JpgBitstreamSize);
                ippbuf_msg("    JPG(%d) : Addr:0x%x,size:0x%x\r\n",i,ippbuf_GetBuf(IPPBUF_CAP_JPG_1+i),JpgBitstreamSize);

                //raw address
                BuffCurrPtr = ippbuf_GetBuf(IPPBUF_CAP_END)-_rawsize;
                ippbuf_SetBuf(IPPBUF_CAP_RAW_1, BuffCurrPtr);
                /*ippbuf_msg("    RAW(%d) : Addr:0x%x(%d %d),IFE repeat:%d\r\n"
                    ,i
                    ,ippbuf_GetBuf(IPPBUF_CAP_RAW_1+i)
                    ,pInfo->CropH * pInfo->SieBitDepth / 8,pInfo->CropV
                    ,repeat1);*/
                ippbuf_msg("    RAW(%d) : Addr:0x%x(%d %d),IFE repeat:%d\r\n"
                    ,i
                    ,ippbuf_GetBuf(IPPBUF_CAP_RAW_1+i)
                    ,DzTable[Get_DzoomIndex()-10][2] * pInfo->SieBitDepth / 8,DzTable[Get_DzoomIndex()-10][3]
                    ,repeat1);
            }
        }
        //#NTK#2012/07/12#Robin Chen -end
        //#NT#2012/08/09#KS Hung -end
    }
    //Current End pointer
    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, BuffCurrPtr);

    if(BuffCurrPtr>(BufAddr+BufSize))
    {
        debug_err(("Cap_NormalCase: Buf. not enough 0x%x<0x%x\r\n", BufSize, (BuffCurrPtr-BufAddr)));
        rst.er_code = E_BOVR;
    }
    else
        rst.er_code = E_OK;
    rst.uibuffer = BuffCurrPtr-BufAddr;
    return rst;
}


/**
    ippbuf_SampleAssigner_Cap_Normal
        To allocate IPP-Buffers for Capture slice mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
static IPPBUF_RST ippbuf_SampleAssigner_Cap_Normal(UINT32 BufAddr, UINT32 BufSize,pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 BuffCurrPtr;
    PImageAlgInfo _pimginfo;
    IPPBUF_RST rst;
    _pimginfo = UI_GetParameter();

    ////Initial Check
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    //ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    //ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);
    ippbuf_msg("Cap:Start : 0x%x End : 0x%x\r\n",BufAddr,BuffCurrPtr+BufSize);
    ippbuf_msg("    NormalCase:\r\n");

    rst = ippbuf_Cap_NormalCase(BufAddr,BufSize,pInfo);

    if(rst.er_code != E_OK)
    {
        if(GetIPPFuncInfor(FQV_EN)==TRUE)
            SetIPPFuncInfor(FQV_EN, DISABLE);

        if(GetIPPFuncInfor(DFS_EN)==TRUE)
            SetIPPFuncInfor(DFS_EN, DISABLE);
        ippbuf_msg("    MinCase:\r\n");
        rst = ippbuf_Cap_MinCase(BufAddr,BufSize,pInfo);
    }

    return rst;
}

//frame mode
static IPPBUF_RST ippbuf_SampleAssigner_Cap_Normal_Frame(UINT32 BufAddr, UINT32 BufSize,pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 i;
    UINT32 BuffCurrPtr;
    UINT32 BuffRemainSize;
    UINT32 JpgBitstreamSize;//, SIEOutSize;
    UINT8  RawNum, JpgNum;
    UINT32 _FQV_YCC_Size=0;
    IPPBUF_RST rst;
    ////Extra Prepare
    RawNum = Sen_GetRawNum();
    JpgNum = Sen_GetJpgNum();

    ////Initial Check
    BuffCurrPtr = BufAddr;

    //Capture ECS table
    //ippbuf_SetBuf(IPPBUF_ECS, BuffCurrPtr);
    //BuffCurrPtr += ECSTBLSIZE(pInfo->EcsMapSize);
    //BuffCurrPtr += pInfo->EcsMapSize;
    //BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);

    //exif header
    ippbuf_SetBuf(IPPBUF_CAP_HEADER, BuffCurrPtr);
    BuffCurrPtr += JpgHeaderSize;
    ippbuf_msg("    ExifHeader : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_HEADER));

    //capture IFE path-1 output buf
    ippbuf_SetBuf(IPPBUF_CAP_FRAME_Y, BuffCurrPtr);
    ippbuf_msg("    FRAME_BUF_Y size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath1],pInfo->ImeOutV[ImePath1],pInfo->Ime_Fmt[ImePath1]);
    BuffCurrPtr += (pInfo->ImeOutLineOffset[ImePath1] * pInfo->ImeOutV[ImePath1]);
    ippbuf_msg("    FRAME_BUF_Y : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_FRAME_Y));
    ippbuf_SetBuf(IPPBUF_CAP_FRAME_CB, BuffCurrPtr);
    BuffCurrPtr += (pInfo->ImeOutCCLineOffset[ImePath1] * pInfo->ImeOutV[ImePath1]/(pInfo->Ime_Fmt[ImePath1]/2));
    ippbuf_msg("    FRAME_BUF_CB : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_FRAME_CB));
    ippbuf_SetBuf(IPPBUF_CAP_FRAME_CR, BuffCurrPtr);
    BuffCurrPtr += (pInfo->ImeOutCCLineOffset[ImePath1] * pInfo->ImeOutV[ImePath1]/(pInfo->Ime_Fmt[ImePath1]/2));
    ippbuf_msg("    FRAME_BUF_CR : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_FRAME_CR));


    //Screennail
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    ippbuf_msg("    Screennail size : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->Ime_Fmt[ImePath2]);
    BuffCurrPtr += (pInfo->ImeOutLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2] +
    2*pInfo->ImeOutCCLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2]/(pInfo->Ime_Fmt[ImePath2]/2));
    ippbuf_msg("    Screennail : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_SCREEN));

    //Thumbnail
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtr);
    //thumb always 422 fmt
    ippbuf_msg("    Thumbnail size : %d %d\r\n",CAP_THUMB_H,CAP_THUMB_V);
    BuffCurrPtr += (CAP_THUMB_H * CAP_THUMB_V + CAP_THUMB_H * CAP_THUMB_V);
    ippbuf_msg("    Thumbnail : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_THUMBNAIL));

    //Capture IME QV Image Buffer (Display img)
    if(GetIPPFuncInfor(FQV_EN))
    {
        _FQV_YCC_Size = (pInfo->ImeOutLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2] +
        2*pInfo->ImeOutCCLineOffset[ImePath2]* pInfo->ImeOutV[ImePath2]/(pInfo->Ime_Fmt[ImePath2]/2));
        ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtr);
        ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);//not support 2-stage FQV
        ippbuf_msg("    FQV size(FrameMode) : %d %d %d\r\n",pInfo->ImeOutLineOffset[ImePath2],pInfo->ImeOutV[ImePath2],pInfo->Ime_Fmt[ImePath2]);
        BuffCurrPtr += _FQV_YCC_Size;
        ippbuf_msg("    FQV(QV_DISPLAY) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_DISPLAY));

        //encode dcout buf allocate.QV_PRE first,than QV_DISPLAY.
        if(!GetIPPFuncInfor(FQV_FRAMEMDOE_EN))
        {
            ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
            ippbuf_msg("    FQV size(DCOUT) : %d %d %d\r\n",GetIPPSizeInfor(_QVPreYLineOffset),GetIPPSizeInfor(_QVPreVsize),pInfo->Ime_Fmt[ImePath2]);
            BuffCurrPtr += (GetIPPSizeInfor(_QVPreYLineOffset)* GetIPPSizeInfor(_QVPreVsize) +
                            2*GetIPPSizeInfor(_QVPreUVLineOffset)* GetIPPSizeInfor(_QVPreVsize)/(pInfo->Ime_Fmt[ImePath2]/2));
            ippbuf_msg("    FQV(QV_PRE) : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_QV_PRE));
        }
    }

    //sie raw
    for (i = 0; i < RawNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_RAW_1 + i, BuffCurrPtr);
        BuffCurrPtr += pInfo->CropH * pInfo->CropV * pInfo->SieBitDepth / 8;
        ippbuf_msg("    RAW(%d) : Addr:0x%x\r\n",i,ippbuf_GetBuf(IPPBUF_CAP_RAW_1+i));
    }

    if(GetIPPFuncInfor(DFS_EN))
    {
        ippbuf_SetBuf(IPPBUF_CAP_RAW_DARKFRAME, BuffCurrPtr);
        BuffCurrPtr += pInfo->CropH * pInfo->CropV * 10 / 8;//DFS always 10-bit output
        ippbuf_msg("    DFS : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME));
    }

    //Capture Data Stamp Alpha Blending Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
    BuffCurrPtr += GetIPPSizeInfor(_ImeCapOutHsize) * pInfo->ImeSliceH;
    ippbuf_msg("    Alpha blending : Addr:0x%x\r\n",ippbuf_GetBuf(IPPBUF_CAP_DATA_STAMP));

    //Capture Jpg buffer
    BuffRemainSize = ippbuf_GetBuf(IPPBUF_CAP_END)-BuffCurrPtr;
    //inorder to get each jpg available size
    {
        UINT32 tmp;
        tmp = (BuffRemainSize / JpgNum);
        if(tmp > (JpgHeaderSize + CAP_DEBUGINFO_SIZE))
        {
            tmp = tmp - (JpgHeaderSize + CAP_DEBUGINFO_SIZE);
        }
        else
            tmp = 0;
        tmp -= (tmp % 4);
        JpgBitstreamSize = tmp;
    }
    ippbuf_SetBuf(IPPBUF_CAP_JPG_BTSSIZE, JpgBitstreamSize);
    for (i = 0; i < JpgNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1+ i, BuffCurrPtr + JpgHeaderSize);
        BuffCurrPtr += (JpgBitstreamSize+(JpgHeaderSize + CAP_DEBUGINFO_SIZE));
        ippbuf_msg("    JPG(%d) : Addr:0x%x,size:0x%x\r\n",i,ippbuf_GetBuf(IPPBUF_CAP_JPG_1+i),JpgBitstreamSize);
    }

    //Current End pointer
    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, BuffCurrPtr);

    if(BuffCurrPtr>(BufAddr+BufSize))
    {
        debug_err(("Cap_NormalCase: Buf. not enough 0x%x<0x%x\r\n", BufSize, (BuffCurrPtr-BufAddr)));
        rst.er_code = E_BOVR;
    }
    else
        rst.er_code = E_OK;
    rst.uibuffer = BuffCurrPtr-BufAddr;
    return rst;
}

/**
    ippbuf_SampleAssigner_Cap_AlbumShot
        To allocate IPP-Buffers for Capture mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
static IPPBUF_RST ippbuf_SampleAssigner_Cap_AlbumShot(UINT32 BufAddr, UINT32 BufSize,pIPPBUF_ENG2DRAMINFO pInfo)
{
    IPPBUF_RST rst = {0};
#if 0
    UINT32 BuffCurrPtr, BuffCurrPtrTmp;
    UINT32 Numerator, Denominator;

    ////Initial Check
    BufSizeInNeed = 0;
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);

    //Capture Raw buffer
    ippbuf_SetBuf(IPPBUF_CAP_RAW_1, BuffCurrPtr);

    ASGetBufRatio(&Numerator, &Denominator);
    if (Get_DzoomCapMaxBuffer(&SIE_PRE_Para_prv) > (GetIPPSizeInfor(_ImeCapOutHsize) * GetIPPSizeInfor(_ImeCapOutVsize) * Numerator / Denominator))
    {
        BuffCurrPtr += Get_DzoomCapMaxBuffer(&SIE_PRE_Para_prv);
    }
    else
    {
        BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * GetIPPSizeInfor(_ImeCapOutVsize) * Numerator / Denominator);
    }

    //Capture IME path-2 Output (Small Image, which would be scaled again as QV img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 2 * 1);

    BuffCurrPtrTmp = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    //Capture IME QV Image Buffer (Display img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtrTmp);
    BuffCurrPtrTmp += (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 2 * 1);
    //Capture IME QV Image Buffer (Screenail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ScreenHsize) * GetIPPSizeInfor(_ScreenVsize) * 2);
    //Capture IME QV Image Buffer (thumbnail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtrTmp);
    BuffCurrPtrTmp += (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 2 * 1);
    //Capture Data Stamp Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtrTmp);
//KH mark
#if 0
    BuffCurrPtrTmp += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2)/6;
#endif
    ippbuf_SetBuf(IPPBUF_CAP_JPG_1, BuffCurrPtr);
    if(ippbuf_GetBuf(IPPBUF_CAP_END) > BuffCurrPtr)
    SetJpgBufSize(ippbuf_GetBuf(IPPBUF_CAP_END) - BuffCurrPtr);
    else
        SetJpgBufSize(0);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, ippbuf_GetBuf(IPPBUF_CAP_END));

    ////Final Check
    BufSizeInNeed += ippbuf_GetBuf(IPPBUF_CAP_CURREND) -
                     ippbuf_GetBuf(IPPBUF_CAP_START);
    if(ippbuf_GetBuf(IPPBUF_CAP_JPG_1)>ippbuf_GetBuf(IPPBUF_CAP_END))
    {
        ippbuf_msg("ippbuf_SampleAssigner_Cap_AlbumShot: Buf. not enough 0x%x<0x%x\r\n", ippbuf_GetBuf(IPPBUF_CAP_JPG_1), ippbuf_GetBuf(IPPBUF_CAP_END));
    }
#endif
    return rst;
}

/**
    ippbuf_SampleAssigner_Cap_CoupleShot
        To allocate IPP-Buffers for Capture couple shot mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
static IPPBUF_RST ippbuf_SampleAssigner_Cap_CoupleShot(UINT32 BufAddr, UINT32 BufSize,pIPPBUF_ENG2DRAMINFO pInfo)
{
    IPPBUF_RST rst = {0};
#if 0
    UINT32 i;
    UINT32 BuffCurrPtr;
    UINT32 BuffRemainSize;
    UINT32 JpgBitstreamSize;
    UINT8  ShotNum, RawNum, JpgNum;

    ////Extra Prepare
    RawNum = Sen_GetRawNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    JpgNum = Sen_GetJpgNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    ShotNum=1;


    ////Initial Check
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);

    ////Main Assignment
    //Capture Chroma-Filter Buffer
    ippbuf_SetBuf(IPPBUF_CAP_CHROMA_FILTER, BuffCurrPtr);
//KH mark
#if 0
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * CHROMA_BUF_NUM / 2);
    //Capture IME path-1 Output (Slice-Buffer)
    ippbuf_SetBuf(IPPBUF_CAP_SLC_BUF, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2);
#endif
    //Capture IME path-2 Output (Small Image, which would be scaled again as QV img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Display img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Screenail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ScreenHsize) * GetIPPSizeInfor(_ScreenVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (thumbnail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 2 * ShotNum);
    //Capture Data Stamp Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
//KH mark
#if 0
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2)/6;
#endif

    //Couple shot preview buffer
    ippbuf_SetBuf(IPPBUF_CAP_COUPLESHOTPRV,BuffCurrPtr);
    BuffCurrPtr += 2*GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize);

    //Capture Raw
    ippbuf_SetBuf(IPPBUF_CAP_RAW_1, BuffCurrPtr);
    BuffCurrPtr += pInfo->CropH * pInfo->CropV * pInfo->SieBitDepth / 8;
    ippbuf_SetBuf(IPPBUF_CAP_RAW_2, BuffCurrPtr);

    ippbuf_SetBuf(IPPBUF_CAP_RAW_GEOMETRIC, BuffCurrPtr);

    //Capture Jpg buffer
    if(ippbuf_GetBuf(IPPBUF_CAP_END)>BuffCurrPtr)
    {
        BuffRemainSize = ippbuf_GetBuf(IPPBUF_CAP_END)-BuffCurrPtr;
        {
            UINT32 tmp;
            tmp = (BuffRemainSize / JpgNum);
            if(tmp > (JpgHeaderSize + CAP_DEBUGINFO_SIZE))
            {
                tmp = tmp - (JpgHeaderSize + CAP_DEBUGINFO_SIZE);
            }
            else
                tmp = 0;
            tmp -= (tmp % 4);
            JpgBitstreamSize = tmp;
            SetJpgBufSize(JpgBitstreamSize);
        }
    }
    else
    {
        JpgBitstreamSize = 0;// zero is not working, but let's make the normal error-handle deal with this shortage
        SetJpgBufSize(0);
    }
    if(JpgBitstreamSize<CAP_JPG_BSTM_MIN_SIZE)
    {
        ippbuf_msg("ippbuf_SampleAssigner_Cap_Normal: JpgBitStremSize 0x%x<0x%x\r\n", JpgBitstreamSize, CAP_JPG_BSTM_MIN_SIZE);
    }

    for (i = 0; i < JpgNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1+ i, BuffCurrPtr+JpgHeaderSize);
        BuffCurrPtr += (JpgBitstreamSize+(JpgHeaderSize + CAP_DEBUGINFO_SIZE));
    }

    ////Regular Assign, CURREND
    //Current End pointer
    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, BuffCurrPtr);

    ////Final Check
    if(BuffCurrPtr>(BufAddr+BufSize))
    {
        debug_err(("ippbuf_SampleAssigner_cap_coupleshot: Buf. not enough 0x%x<0x%x\r\n", BufSize, (BuffCurrPtr-BufAddr)));
        rst.er_code = E_SYS;
    }
    else
        rst.er_code = E_OK;
    rst.uibuffer = BuffCurrPtr-BufAddr;
#endif
    return rst;
}

//#NT#for preshot function
/**
    ippbuf_SampleAssigner_Cap_PreShot
        To allocate IPP-Buffers for Capture preshot mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
static IPPBUF_RST ippbuf_SampleAssigner_Cap_PreShot(UINT32 BufAddr, UINT32 BufSize,pIPPBUF_ENG2DRAMINFO pInfo)
{
    IPPBUF_RST rst = {0};
#if 0
    UINT32 i;
    UINT32 BuffCurrPtr;
    UINT32 BuffRemainSize;
    UINT32 JpgBitstreamSize;
    UINT8  ShotNum, RawNum, JpgNum;

    ////Extra Prepare
    RawNum = Sen_GetRawNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    JpgNum = Sen_GetJpgNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    ShotNum=1;


    ////Initial Check
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);

    ////Main Assignment
    //Capture Chroma-Filter Buffer
    ippbuf_SetBuf(IPPBUF_CAP_CHROMA_FILTER, BuffCurrPtr);
//KH mark
#if 0
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * CHROMA_BUF_NUM / 2);
    //Capture IME path-1 Output (Slice-Buffer)
    ippbuf_SetBuf(IPPBUF_CAP_SLC_BUF, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2);
#endif
    //Capture IME path-2 Output (Small Image, which would be scaled again as QV img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Display img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Screenail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ScreenHsize) * GetIPPSizeInfor(_ScreenVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (thumbnail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 2 * ShotNum);
    //Capture Data Stamp Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
//KH mark
#if 0
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2)/6;
#endif
    //Couple shot preview buffer
    ippbuf_SetBuf(IPPBUF_CAP_PRESHOTPRV,BuffCurrPtr);
    BuffCurrPtr += 2*GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize);

    //Capture Raw
    ippbuf_SetBuf(IPPBUF_CAP_RAW_1, BuffCurrPtr);
    BuffCurrPtr += pInfo->CropH * pInfo->CropV * pInfo->SieBitDepth / 8;

    ippbuf_SetBuf(IPPBUF_CAP_RAW_GEOMETRIC, BuffCurrPtr);

    //Capture Jpg buffer
    if(ippbuf_GetBuf(IPPBUF_CAP_END)>BuffCurrPtr)
    {
        BuffRemainSize = ippbuf_GetBuf(IPPBUF_CAP_END)-BuffCurrPtr;
        {
            UINT32 tmp;
            tmp = (BuffRemainSize / JpgNum);
            if(tmp > (JpgHeaderSize + CAP_DEBUGINFO_SIZE))
            {
                tmp = tmp - (JpgHeaderSize + CAP_DEBUGINFO_SIZE);
            }
            else
                tmp = 0;
            tmp -= (tmp % 4);
            JpgBitstreamSize = tmp;
            SetJpgBufSize(JpgBitstreamSize);
        }
    }
    else
    {
        JpgBitstreamSize = 0;// zero is not working, but let's make the normal error-handle deal with this shortage
        SetJpgBufSize(0);
    }
    if(JpgBitstreamSize<CAP_JPG_BSTM_MIN_SIZE)
    {
        ippbuf_msg("ippbuf_SampleAssigner_Cap_Normal: JpgBitStremSize 0x%x<0x%x\r\n", JpgBitstreamSize, CAP_JPG_BSTM_MIN_SIZE);
    }

    for (i = 0; i < JpgNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1+ i, BuffCurrPtr+JpgHeaderSize);
        BuffCurrPtr += (JpgBitstreamSize+(JpgHeaderSize + CAP_DEBUGINFO_SIZE));
    }

    ////Regular Assign, CURREND
    //Current End pointer
    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, BuffCurrPtr);

    ////Final Check
    if(BuffCurrPtr>(BufAddr+BufSize))
    {
        debug_err(("ippbuf_SampleAssigner_Cap_preshot: Buf. not enough 0x%x<0x%x\r\n", BufSize, (BuffCurrPtr-BufAddr)));
        rst.er_code = E_SYS;
    }
    else
        rst.er_code = E_OK;
    rst.uibuffer = BuffCurrPtr-BufAddr;
#endif
    return rst;
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif

/**
    Execute IPP Buffer Assigner

    Assign IPP buffers for certain mode

    @param[in] modeIdx      mode index
               SIE_Param_Addr    sie struct addr

    @return
        - @b BufferSize In Need, if return
        - @b Otherwise: error
*/
IPPBUF_RST ippbuf_ExecuteAssigner(IPPBUF_MODE modeIdx,pIPPBUF_ENG2DRAMINFO pInfo)
{
    UINT32 BufAddr, BufSize;
    UINT32 uiReturn=IPPBUF_E_OK;
    IPPBUF_RST rst;

    if (modeIdx > IPPBUF_MODE_MAX_INDEX)
    {
        ippbuf_msg("ippbuf_ExecuteAssigner: out of index\r\n");
        uiReturn = IPPBUF_E_NOINDEX;
    }
    else
    {
        if (modeIdx >= IPPBUF_MODE_CAP_NORMAL)
        {
            ippbuf_GetResource(IPPBUF_RES_CAP, &BufAddr, &BufSize);
        }
        else
        {
            ippbuf_GetResource(IPPBUF_RES_PRV, &BufAddr, &BufSize);
        }
        rst = (ippbuf_AssignerObject[modeIdx])(BufAddr, BufSize, pInfo);
    }
    return rst;
}

/**
    Set Memory Resource

    Set memory resources for certain mode

    @param[in] ResIdx       resource index
    @param[in] BufAddr      memory resource start-ptr
    @param[in] BufSize      memory resource start-ptr

    @return
        - @b IPPBUF_E_OK:   operation sucessful
        - @b Otherwise:     error
*/
UINT32 ippbuf_SetResource(IPPBUF_RESOURCE ResIdx, UINT32 BufAddr, UINT32 BufSize)
{
    UINT _MinSize = 0;

    //after executing the Photo_Close or ImgCapture_Close function,
    //we must be reset ipp buffer for memory continuous mode.
    if (ippbuf_GetResetFlag() == ENABLE)
    {
        ippbuf_SetResetFlag(DISABLE);
        ippbuf_SetBuf(IPPBUF_CAP_START, IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_CAP_END,   IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_PRV_START, IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_PRV_END,   IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_TOTAL_START, IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_TOTAL_END,   IPPBUF_NULL);
    }

    //assigned resource
    switch(ResIdx)
    {
        case IPPBUF_RES_CAP:
            if(!ippbuf_MinDramsizeCheck_Cap(BufSize,&_MinSize))
            {
                debug_err(("\r\n ********************************** \r\n"));
                debug_err(("Cap allocated size is not enough(0x%x < 0x%x )\r\n",BufSize,_MinSize));
                debug_err(("system may be crash\r\n"));
                debug_err(("\r\n ********************************** \r\n"));
                return IPPBUF_E_OUTOFBUFF;
            }

            ippbuf_SetBuf(IPPBUF_CAP_START, BufAddr);
            ippbuf_SetBuf(IPPBUF_CAP_END,   BufAddr+BufSize);
            break;

        case IPPBUF_RES_PRV:
            ippbuf_SetBuf(IPPBUF_PRV_START, BufAddr);
            ippbuf_SetBuf(IPPBUF_PRV_END,   BufAddr+BufSize);
            break;

        default:
            debug_err(("ippbuf_SetResource: out of index\r\n"));
            return IPPBUF_E_NOINDEX;
    }
    /*
    if (Sen_GetMemMode() == SEN_MEM_COMBINE_MODE)
    {
        ippbuf_Arrange();
    }
    */
    return IPPBUF_E_OK;
}

/**
    Get Memory Resource

    Get memory resources

    @param[in] ResIdx       resource index
    @param[out] BufAddr      memory resource start-ptr
    @param[out] BufSize      memory resource start-ptr

    @return
        - @b IPPBUF_E_OK:   operation sucessful
        - @b Otherwise:     error
*/
UINT32 ippbuf_GetResource(IPPBUF_RESOURCE ResIdx, UINT32 *BufAddr, UINT32 *BufSize)
{
    switch(ResIdx)
    {
        case IPPBUF_RES_CAP:
            *BufAddr = ippbuf_GetBuf(IPPBUF_CAP_START);
            *BufSize = (ippbuf_GetBuf(IPPBUF_CAP_END) - *BufAddr);
            break;

        case IPPBUF_RES_PRV:
            *BufAddr = ippbuf_GetBuf(IPPBUF_PRV_START);
            *BufSize = (ippbuf_GetBuf(IPPBUF_PRV_END) - *BufAddr);
            break;

        default:
            *BufAddr = IPPBUF_NULL;
            *BufSize = 0;
            debug_err(("ippbuf_SetResource: out of index\r\n"));
            return IPPBUF_E_NOINDEX;
    }
    return IPPBUF_E_OK;
}

//#NT#IPPBuffer API Intergration080717
/**
    Register Customer Assigner

    Register Assigner for certain mode

    @param[in] modeIdx          mode index
    @param[in] pIPPBufAssigner  customer Assigner

    @return
        - @b IPPBUF_E_OK: Operation successful
        - @b Otherwise: Operation failed
*/
UINT32 ippbuf_RegisterAssigner(IPPBUF_MODE modeIdx, FPIPPBUFASSIGNER pIPPBufAssigner)
{
    //ippbuf_msg(("ippbuf_RegisterAssigner FPIPPBUFASSIGNER %d 0x%08x\r\n", modeIdx, pIPPBufAssigner));
    if(modeIdx < IPPBUF_MODE_MAX_INDEX)
    {
        (ippbuf_AssignerObject[modeIdx]) = pIPPBufAssigner;
    }
    else
    {
        ippbuf_msg("ippbuf_RegisterAssigner: out of index\r\n");
        return IPPBUF_E_NOINDEX;
    }

    return IPPBUF_E_OK;
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This area is for ippbuf - APIs

/**
    Get IPP Buffer

    Set certain IPP buffer

    @param[in] BufferIdx    buffer index
    @param[in] setValue     value to set

    @return
        - @b IPPBUF_E_OK:   operation sucessful
        - @b Otherwise:     error
*/
UINT32 ippbuf_SetBuf(IPPBUF_BUFFER BufferIdx, UINT32 setValue)
{
    //ippbuf_ind(("ippbuf_SetBuf Idx:%02d  0x%08x \r\n", BufferIdx, setValue));
    if(BufferIdx < IPPBUF_MAX_INDEX)
    {
        ippbuf_BufObject[BufferIdx] = setValue;
        return IPPBUF_E_OK;
    }
    else
    {
        ippbuf_msg("ippbuf_SetBuf: out of index\r\n");
        return IPPBUF_E_NOINDEX;
    }
}

/**
    Get IPP Buffer

    Set certain IPP buffer

    @param[in] BufferIdx    buffer index
    @param[in] setValue     value to set

    @return
        - @b Buffer Start-ptr
        - @b IPPBUF_E_NOINDEX(=1): index out of range
*/
UINT32 ippbuf_GetBuf(IPPBUF_BUFFER BufferIdx)
{
    if(BufferIdx < IPPBUF_MAX_INDEX)
    {
        return ippbuf_BufObject[BufferIdx];
    }
    else
    {
        ippbuf_msg("ippbuf_GetBuf: out of index\r\n");
        return IPPBUF_E_NOINDEX;
    }
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//Param: _MinSize: return minsize,
//if size < _MinSzie  ,return FALSE,else return TRUE
BOOL ippbuf_MinDramsizeCheck_Cap(UINT _Size,UINT* _MinSize)
{
    /*
    PImageAlgInfo _pimginfo;
    UINT _Shift;
    _pimginfo = UI_GetParameter();
    switch(Get_IME_OutInfo(IPP_SliceMode_F2S,ImeOutInfo_FMT))
    {
        case IME_OUTPUT_YCC_420_CEN:
        case IME_OUTPUT_YCC_420_COS:
            _Shift = 2;
            break;
        case IME_OUTPUT_YCC_422_CEN:
        case IME_OUTPUT_YCC_422_COS:
            _Shift = 1;
            break;
        default:
            _Shift = 1;
            debug_err(("%d %d %s\r\n",__LINE__,Get_IME_OutInfo(IPP_SliceMode_F2S,ImeOutInfo_FMT),__func__));
            break;
    }
    *(UINT32*)_MinSize = CAP_DEBUGINFO_SIZE + JpgHeaderSize + CAP_JPGLeadingMinSize +
       JPGIMG_SIZE[_pimginfo->MaxJpgSize][1]*IME_SLICE_H(_Cap)*(SLICE_BUF_NUM(_Cap)+1) +
       JPGIMG_SIZE[_pimginfo->MaxJpgSize][1]*IME_SLICE_H(_Cap)*(SLICE_BUF_NUM(_Cap)+1)/_Shift +
       JPGIMG_SIZE[_pimginfo->SensorSize][0]*JPGIMG_SIZE[_pimginfo->SensorSize][1]
       ;

    if(_Size < (*(UINT32*)_MinSize) )
        return FALSE;
    else
        return TRUE;
    */
    return TRUE;
}

//#NT#2012/08/09#KS Hung -begin
//#NT#Set the JPG&RAW Buffer overlap or not
void ippbuf_SetJpgRawOverlap(BOOL Flag)
{
    g_bJpgRawBufOverlap = Flag;
}
//#NT#2012/08/09#KS Hung -end