#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fd_lib.h"
#include "fd_int.h"
#include "ImageAlgInfor.h"
#include "debug.h"
#include "ammem.h"
#include "ippbuffer.h"
#include "oscommon.h"
#include "grph.h"
#include "ide.h"
#include "Arcsoft_Face_Tracking.h"
#include "FWRotate.h"

#define FD_DEBUG    DISABLE

#if (FD_DEBUG == ENABLE)
#define FD_MSG(...)    debug_msg (" FD: "__VA_ARGS__);
#else
#define FD_MSG(...)
#endif

UINT8 g_FDRate = FD_RATE;
volatile UINT32 fdInitialFlag = ON;
volatile UINT32 fdCurInitialFlag;
UINT32 fdFrameWidth = 0, fdFrameHeight = 0;
UINT32 fdScreenWidth = 0, fdScreenHeight = 0;
UINT32 fdFaceSize = 0;

static AMFPAF_ENGINE FaceEngine;
static AMFPAF_FACERES FaceRes;
static AMFPAF_IMAGEINFO ImageInfo;
static void * membuffer = NULL;
static MHandle memHandle = NULL;
static UINT8 *BufAddr = NULL;
//#NT#2010/09/16#JJ -begin
static UINT8 *ImgYAddr = NULL;
static UINT8 *ImgCbAddr = NULL;
static UINT8 *ImgCrAddr = NULL;
//#NT#2010/09/16#JJ -end
//#NT#2009/08/20#Jeffery Chuang -begin
static UINT32 FDBuff_directAddr=0;
//#NT#2009/08/20#Jeffery Chuang -end

void fd_PassFaceInfor(void)
{
    UINT8 i;
    LPAMFPAF_FACERES pFcaceRes = &FaceRes;

    if (pFcaceRes->nFace > 0)
    {
        //draw current rect
        for(i = 0;i < pFcaceRes->nFace; i ++)
        {
            gAlgMsg.FaceDetectRect[i].uiLeft =  pFcaceRes->rcFace[i].left * fdScreenWidth / fdFrameWidth;
            gAlgMsg.FaceDetectRect[i].uiTop =   pFcaceRes->rcFace[i].top * fdScreenHeight / fdFrameHeight;
            gAlgMsg.FaceDetectRect[i].uiWidth = (pFcaceRes->rcFace[i].right - pFcaceRes->rcFace[i].left) * fdScreenWidth / fdFrameWidth;
            gAlgMsg.FaceDetectRect[i].uiHeight =(pFcaceRes->rcFace[i].bottom - pFcaceRes->rcFace[i].top) * fdScreenHeight / fdFrameHeight;
        }
    }
    gAlgMsg.FaceDetectNum = pFcaceRes->nFace;
    if (gAlgMsg.FaceDetectNum != 0)
    {
        for (i = 0; i < gAlgMsg.FaceDetectNum; i++)
        {
            FD_MSG("fd %d/%d (%d %d) %dx%d\r\n", i, gAlgMsg.FaceDetectNum,
                                            gAlgMsg.FaceDetectRect[i].uiLeft,
                                            gAlgMsg.FaceDetectRect[i].uiTop,
                                            gAlgMsg.FaceDetectRect[i].uiWidth,
                                            gAlgMsg.FaceDetectRect[i].uiHeight);
        }
    }
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FDEND, &gAlgMsg);
    }
}

MLong pDrawRectangleCallback(MVoid *pParam1, MVoid *pParam2)
{
#if 0
    MLong i;
    LPAMFPAF_FACERES pFcaceRes = (LPAMFPAF_FACERES)pParam1;

    //clear previous rect
    for(i = 0; i < g_prevfacenum; i ++)
    {
    }

    if (pFcaceRes->nFace > 0)
    {
        //draw current rect
        for(i = 0;i < pFcaceRes->nFace; i ++)
        {
            gAlgMsg.FaceDetectRect[i].uiLeft = pFcaceRes->rcFace[i].left * VIEW_SCREEN_WIDTH / SIZE_WIDTHFRAME;
            gAlgMsg.FaceDetectRect[i].uiTop = pFcaceRes->rcFace[i].top * VIEW_SCREEN_HEIGHT / SIZE_HEIGHTFRAME;
            gAlgMsg.FaceDetectRect[i].uiWidth = (pFcaceRes->rcFace[i].right - pFcaceRes->rcFace[i].left) * VIEW_SCREEN_WIDTH / SIZE_WIDTHFRAME;
            gAlgMsg.FaceDetectRect[i].uiHeight = (pFcaceRes->rcFace[i].bottom - pFcaceRes->rcFace[i].top) * VIEW_SCREEN_HEIGHT / SIZE_HEIGHTFRAME;
        }
    }
    g_prevfacenum = pFcaceRes->nFace;
#endif
    return 0;
}

void fd_FaceDetectionProcess(void)
{
    UINT32 InitialBuf, temp;
    ACTFBADDR_PRAM  FB_para;

    if (fd_CheckInitFalg() == ON)
    {
        //get parameter
        //#NT#2012/07/02#Lincy Lin -begin
        //#NT#Fix FD input w,h error bug
        //fdFrameWidth = GetDisplayH();
        //fdFrameHeight = GetDisplayV();
        fdFrameWidth = GetIPPSizeInfor(_ImeOut1Hsize);
        fdFrameHeight = GetIPPSizeInfor(_ImeOut1Vsize);
        //#NT#2012/07/02#Lincy Lin -end

        //#NT#2011/05/25#Hideo Lin -begin
        //#NT#Fix FD frame coordinates error as FW rotation
        if (FWRotate_GetStatus()==TRUE)
        {
            ide_get_osd1_buf_dim(&fdScreenHeight, &fdScreenWidth, &temp);
        }
        else
        {
        ide_get_osd1_buf_dim(&fdScreenWidth, &fdScreenHeight, &temp);
        }
        //#NT#2011/05/25#Hideo Lin -end
        InitialBuf = FDBuff_directAddr;
        FD_MSG("InitialBuf = %#x, size=%dx%d\r\n", InitialBuf, fdFrameWidth, fdFrameHeight);

        if(InitialBuf == 0)
        {
            debug_err(("FDBuff_directAddr is 0 !!\r\n"));
            return;
        }
        if(fd_Init(InitialBuf, fdFrameWidth, fdFrameHeight, fdFrameWidth, fdFaceSize, ((FWRotate_GetStatus()==TRUE)?AMFPAF_PAF_YUV420:AMFPAF_PAF_PYUV422)))
        //if(fd_Init(InitialBuf, fdFrameWidth, fdFrameHeight, fdFrameWidth, fdFaceSize, AMFPAF_PAF_PYUV422))
        {
            fd_SetInitFlag(ON);
            debug_err(("fd_init fail\r\n"));
        }
    }
    else
    {
        GetActImgFBAddr_Path1(&FB_para);
        fd_FaceDetection(FB_para.y_addr, FB_para.cb_addr, FB_para.cr_addr);
        fd_PassFaceInfor();
    }
}

INT32 fd_FaceDetection(UINT32 SrcYAddr, UINT32 SrcCbAddr, UINT32 SrcCrAddr)
{
    MRESULT result;

    fd_LoadImg((UINT32)BufAddr, SrcYAddr, SrcCbAddr, SrcCrAddr,
                        ImageInfo.lWidth, ImageInfo.lHeight, ImageInfo.lPixelArrayFormat);

    result = AMFPAF_FaceFeatureDetect(memHandle, FaceEngine, BufAddr, &FaceRes, AMFPAF_FOC_0, pDrawRectangleCallback, MNull, MNull, MNull);

    return 0;
}

UINT32 fd_CheckInitFalg(void)
{
    UINT32 rtValue;
    rtValue = OFF;
    //#NT#2012/07/02#Lincy Lin -begin
    //#NT#Fix FD input w,h error bug
    if ((fdFrameWidth != GetIPPSizeInfor(_ImeOut1Hsize)) || (fdFrameHeight != GetIPPSizeInfor(_ImeOut1Vsize)))
    //#NT#2012/07/02#Lincy Lin -end
    {
        rtValue = ON;
    }
    fdCurInitialFlag = fdInitialFlag;
    if (fdCurInitialFlag == ON)
    {
        fdInitialFlag = OFF;
        rtValue = ON;
    }
    return rtValue;
}

UINT32 fd_GetFaceInforAddr(void)
{
    return (UINT32)&FaceRes;
}

void fd_GetResultBufAddr(UINT32 *YAddr, UINT32 *CbAddr, UINT32 *CrAddr, UINT32 *Size)
{
    *YAddr = (UINT32)ImgYAddr;
    *CbAddr = (UINT32)ImgCbAddr;
    *CrAddr = (UINT32)ImgCrAddr;
    *Size = FD_BUFFER_SIZE;
}

void fd_SetWorkingBufAddr(UINT32 fd_address)
{
    FDBuff_directAddr = fd_address;
}

UINT32 fd_GetWorkingBufSize(void)
{
    return FD_BUFFER_SIZE;
}

UINT32 fd_GetScreenWidth(void)
{
    return fdScreenWidth;
}

UINT32 fd_GetScreenHeight(void)
{
    return fdScreenHeight;
}

INT32 fd_Init(UINT32 BuffAddr, UINT32 ImgWidth, UINT32 ImgHeight, UINT32 ImgLineOffset, UINT32 FaceSize, UINT32 ImgFmt)
{
    MLong nscale;
    const ArcSoft_Face_Tracking_Version* lib;

    if ((ImgHeight <= FaceSize) || (FaceSize == 0))
    {
        debug_err(("face size setting fail\r\n"));
        return -1;
    }

    lib = ArcSoft_Face_Tracking_GetVersion();
    debug_msg("FD ver:%s\r\n", lib->Version);

    nscale = ImgHeight / FaceSize;
    if (nscale > FD_MAX_SUPPORT_FACE_SCALE)
    {
        nscale = FD_MAX_SUPPORT_FACE_SCALE;
    }
    membuffer = NULL;
    memHandle = NULL;
    BufAddr = (UINT8 *)BuffAddr;
    //#NT#2010/09/16#JJ -begin
    ImgYAddr = BufAddr;
    ImgCbAddr = ImgYAddr+(ImgHeight * ImgWidth);
    if(ImgFmt==AMFPAF_PAF_YUV420)
    {
        ImgCrAddr = ImgCbAddr+(ImgHeight * ImgWidth/4);
    }
    else if(ImgFmt==AMFPAF_PAF_PYUV422)
    {
        ImgCrAddr = ImgCbAddr+(ImgHeight * ImgWidth/2);
    }
    else
    {
        debug_err(("ERROR! fd_init: format not supported!"));
    }
    //#NT#2010/09/16#JJ -end
    membuffer = (UINT8*)(BufAddr + (ImgHeight * ImgWidth * 2));
    memHandle = MMemMgrCreate(membuffer,(FD_BUFFER_SIZE - (ImgHeight * ImgWidth * 2)));

    ImageInfo.lWidth = ImgWidth;
    ImageInfo.lHeight = ImgHeight;
    ImageInfo.lLineBytes = ImgLineOffset;
    ImageInfo.lPixelArrayFormat = ImgFmt;

    if(AMFPAF_InitialFaceEngine(memHandle, &ImageInfo, &FaceRes, &FaceEngine,
                                FD_MAX_SUPPORT_FACE_ORIENT, nscale, FD_MAX_SUPPORT_FACE_NUM) != MOK)
    {
        debug_err(("initial engine fail\r\n"));
        return -1;
    }
    AMFPAF_SetTrackingFrameNumber(FaceEngine, 5);
    AMFPAF_SetFaceOutPriority(FaceEngine, 1);
    return 0;
}

void fd_LoadImg(UINT32 DestAddr, UINT32 SrcYAddr, UINT32 SrcCbAddr, UINT32 SrcCrAddr,
                UINT32 SrcWidth, UINT32 SrcHeight, UINT32 SrcFmt)
{
    UINT32 DestBuf;

    grph_open();
    grph_swReset();
    grph_enableDMA();

    //Y
    DestBuf = DestAddr;
    grph_setImg1(SrcYAddr, SrcWidth, SrcHeight, SrcWidth);
    grph_setImg2(DestBuf, SrcWidth);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    DestBuf += (SrcHeight * SrcWidth);

    //Cb
    grph_setImg1(SrcCbAddr, (SrcWidth >> 1), SrcHeight, (SrcWidth >> 1));
    grph_setImg2(DestBuf, (SrcWidth >> 1));
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    DestBuf += (SrcHeight * (SrcWidth >> 1));

    //Cr
    grph_setImg1(SrcCrAddr, (SrcWidth >> 1), SrcHeight, (SrcWidth >> 1));
    grph_setImg2(DestBuf, (SrcWidth >> 1));
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    grph_disableDMA();
    grph_swReset();
    grph_close();
}

UINT32 fd_GetInitFlag(void)
{
    return fdCurInitialFlag;
}

void fd_SetFaceSize(UINT32 Size)
{
    fdFaceSize = Size;
}

void fd_SetInitFlag(UINT32 Flag)
{
    fdInitialFlag = Flag;
}

/**
    Lock FD
    @param lock  1:FD Lock  0:FD Unlock
*/
void fd_Lock(UINT8 lock)
{
    if(lock)
    {
        FD_LOCK;
    }
    else
    {
        FD_UNLOCK;
    }
}

/**
    Check FD Lock status
    @return FD Lock TRUE/FALSE
*/
UINT8 fd_IsLock(void)
{
    if (uiFDFlags & FD_FLAG_FD_ENABLE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/**
    Set FD Executing Rate
    @param rate How many frames to do FD once
*/
void fd_SetRate(UINT8 rate)
{
    g_FDRate = rate;
}

/**
    Get FD Executing Rate
    @return FD executing rate
*/
UINT8 fd_GetRate(void)
{
     return g_FDRate;
}
