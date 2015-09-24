#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sd_lib.h"
#include "fd_lib.h"
#include "sen_common.h"

//#include "aaa_lib.h"
//#include "ImageAlgInfor.h"
//#include "debug.h"
//#include "ammem.h"
//#include "merror.h"
//#include "oscommon.h"
//#include "grph.h"

#define SD_BUFFER_SIZE  (1024 * 600)
#define SD_SIZE_WIDTHFRAME     320
#define SD_SIZE_HEIGHTFRAME    240


UINT8 g_SDRate = SD_RATE;
volatile UINT32 uiSDFlags = FALSE;
UINT32 sdDectionFlag = FALSE;

volatile UINT32 sdInitialFlag = ON;
volatile UINT32 sdCurInitialFlag;
UINT32 sdFrameWidth = 0, sdFrameHeight = 0;

static ASSD_ENGINE g_SmileEngine = MNull;
static ASSD_SMILEDETECTIONRESULT g_SmileRes = {MNull,MNull,0,0};
static ASSD_OFFSCREEN g_SmileImageInfo = {0};
static MVoid *g_Smilemembuffer = MNull;
static MHandle g_SmilememHandle = MNull;

void sd_SmileDetectionProcess(void)
{
    //#NT#2010/09/16#JJ -begin
    UINT32 InitialBuf, fdBufYAddr, fdBufCbAddr, fdBufCrAddr, fdBufSize, tempRsult;//, i;
    //#NT#2010/09/16#JJ -end
    LPAMFPAF_FACERES pFaceRes;
    ASSD_ORIENTCODE FaceOrient[1] = {1};

    pFaceRes = (LPAMFPAF_FACERES)fd_GetFaceInforAddr();
    debug_ind(("face number = %d\r\n", pFaceRes->nFace));
    if (pFaceRes->nFace > 0)
    {
        if (sd_CheckInitFalg() == ON)
        {
            sdDectionFlag = FALSE;
            //get parameter
            //#NT#2010/09/16#JJ -begin
            fd_GetResultBufAddr(&fdBufYAddr, &fdBufCbAddr, &fdBufCrAddr, &fdBufSize);
            InitialBuf = (fdBufYAddr + fdBufSize);
            //#NT#2010/09/16#JJ -end
            //fd_SetFaceSize(50);
            fd_SetInitFlag(ON);
            sdFrameWidth = GetIPPSizeInfor(_ImeOut1Hsize);
            sdFrameHeight = GetIPPSizeInfor(_ImeOut1Vsize);

            //sd initial
            //#NT#2010/09/16#JJ -begin
            if (sd_Init(InitialBuf, sdFrameWidth, sdFrameHeight, sdFrameWidth, ASSD_PAF_PYUV422))
            //#NT#2010/09/16#JJ -end
            {
                sd_setInitFlag(OFF);
                debug_err(("sd_init fail\r\n"));
            }
        }
        else
        {
            //#NT#2010/06/11#Chiyi Lin -begin
            FaceOrient[0] = pFaceRes->lfaceOrient;
            //#NT#2010/06/11#Chiyi Lin -end
            //#NT#2010/09/16#JJ -begin
            fd_GetResultBufAddr(&fdBufYAddr, &fdBufCbAddr, &fdBufCrAddr, &fdBufSize);
            sd_SmileDetection(fdBufYAddr, fdBufCbAddr, fdBufCrAddr, pFaceRes, FaceOrient);
            //#NT#2010/09/16#JJ -end
            sdDectionFlag = FALSE;
            debug_ind(("g_SmileRes.lSmileNumber = %d\r\n", g_SmileRes.lSmileNumber));

            if (g_SmileRes.lSmileNumber > 0)
            {
                tempRsult = TRUE;
                //JJ080417, A735 RC
                //for (i = 0; i < g_SmileRes.lFaceNumber; i++)
                //{


                //    if (g_SmileRes.pFlagArrary[i] == 0)
                //    {
                //        tempRsult = FALSE;
                //        break;
                //    }
                //}
                if(sd_IsLock())
                {
                    tempRsult = FALSE;
                }
                //if(ImgAppUser_CheckFlashCharging())
                //{
                //    tempRsult = FALSE;
                //}
                //~JJ080417, A735 RC
                if (tempRsult == TRUE)
                {
                    if(g_fpAlgExport)
                    {
                        g_fpAlgExport(ALGMSG_SDEND, &gAlgMsg);
                    }
                }
                sdDectionFlag = tempRsult;
            }
        }
    }
    else
    {
        sdDectionFlag = FALSE;
    }
}

INT32 sd_Init(UINT32 BufAddr, UINT32 ImgWidth, UINT32 ImgHeight,
                                    UINT32 ImgLineOffset, UINT32 ImgFmt)
{
    MLong nMemSize = SD_BUFFER_SIZE;
    g_Smilemembuffer = (void *)BufAddr;
    g_SmilememHandle = MMemMgrCreate(g_Smilemembuffer,nMemSize);
    if(g_SmilememHandle == MNull)
    {
        return -1;
    }

    g_SmileImageInfo.lWidth = ImgWidth;
    g_SmileImageInfo.lHeight = ImgHeight;
    g_SmileImageInfo.lPixelArrayFormat = ImgFmt;
    g_SmileImageInfo.pixelArray.planar.lLineBytesArray[0] = ImgLineOffset;
    g_SmileImageInfo.pixelArray.planar.lLineBytesArray[1] = (ImgLineOffset >> 1);
    g_SmileImageInfo.pixelArray.planar.lLineBytesArray[2] = (ImgLineOffset >> 1);
    //#NT#2010/09/16#JJ -begin
    if(ASSD_InitSmileEngine(g_SmilememHandle,&g_SmileEngine, ASSD_SENSELEVEL1) != MOK)
    //#NT#2010/09/16#JJ -end
    {
        debug_err(("sd ASSD_InitSmileEngine fail\r\n"));
        return -1;
    }
    return 0;
}

INT32 sd_SmileDetection(UINT32 ImgYAddr, UINT32 ImgCbAddr, UINT32 ImgCrAddr,
                        LPAMFPAF_FACERES pFcaceRes, ASSD_ORIENTCODE *pFaceOrient)
{
    ASSD_FACEINPUT FaceRes;

    g_SmileImageInfo.pixelArray.planar.pPixelArray[0] = (void *)ImgYAddr;
    g_SmileImageInfo.pixelArray.planar.pPixelArray[1] = (void *)ImgCbAddr;
    g_SmileImageInfo.pixelArray.planar.pPixelArray[2] = (void *)ImgCrAddr;

    FaceRes.lFaceNumber = pFcaceRes->nFace;
    FaceRes.pFaceRectArray = pFcaceRes->rcFace;
    FaceRes.pFaceOrientArray = pFaceOrient;

    //#NT#2010/09/16#JJ -begin
    if(ASSD_SmileDetect(g_SmilememHandle,g_SmileEngine,&g_SmileImageInfo,&FaceRes,&g_SmileRes,MNull,MNull) == MOK)
    //#NT#2010/09/16#JJ -end
    {
        return 0;
    }
    return -1;
}

UINT32 sd_CheckInitFalg(void)
{
    UINT32 rtValue;
    rtValue = OFF;
    if ((sdFrameWidth != GetIPPSizeInfor(_ImeOut1Hsize)) || (sdFrameHeight != GetIPPSizeInfor(_ImeOut1Vsize)))
    {
        rtValue = ON;
    }
    sdCurInitialFlag = sdInitialFlag;
    if (sdCurInitialFlag == ON)
    {
        sdInitialFlag = OFF;
        rtValue = ON;
    }
    return rtValue;
}

void sd_setInitFlag(UINT32 Flag)
{
    sdInitialFlag = Flag;
}

/**
    get detection information
    @param lock  1:SD OK  0:SD Fail
*/
UINT32 sd_GetResult(void)
{
    return sdDectionFlag;
}


/**
    Lock SD
    @param lock  1:SD Lock  0:SD Unlock
*/
void sd_Lock(UINT8 lock)
{
    if(lock)
    {
        SD_LOCK;
    }
    else
    {
        SD_UNLOCK;
    }
}

/**
    Check SD Lock status
    @return SD Lock TRUE/FALSE
*/
UINT8 sd_IsLock(void)
{
    if (uiSDFlags & SD_FLAG_SD_ENABLE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/**
    Set SD Executing Rate
    @param rate How many frames to do SD once
*/
void sd_SetRate(UINT8 rate)
{
    g_SDRate = rate;
}

/**
    Get SD Executing Rate
    @return SD executing rate
*/
UINT8 sd_GetRate(void)
{
     return g_SDRate;
}
