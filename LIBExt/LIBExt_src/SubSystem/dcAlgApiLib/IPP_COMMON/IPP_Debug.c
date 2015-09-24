/*
    IPP Debug APIs

    @file         IPP_Debug.c
    @author    Linkin Huang
    @notes     None

    Copyright (c) 2010 Novatek Microelectronics Corporation.  All Rights Reserved.

    Revision History: 

*/
#include <string.h>
#include <stdio.h>

#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "debug.h"
#include "Timer.h"
#include "Filesystsk.h"
#include "Utility.h"
#include "IPP_Debug.h"

//use to show IPP debug msg when writing register info. of engines
//this variable is related to IPP control flow
//Due to flow emergence issue, please think thrice before enabling it 
#define IppDebug_wfMsgShow  0


// define Base address of engines
#define IppDebug_SIE_ADDR  0x9A000000
#define IppDebug_PRE_ADDR  0x9A001000
#define IppDebug_DCE_ADDR  0x9A002000
#define IppDebug_IPE_ADDR  0x9A100000
#define IppDebug_IME_ADDR  0x9A200000
#define IppDebug_DIS_ADDR  0x9A300000
#define IppDebug_FDE_ADDR  0x9A400000  
#define IppDebug_IFE_ADDR  0x9A402000

#define IppDebug_RegNum_SIE  119
#define IppDebug_RegNum_PRE  960
#define IppDebug_RegNum_DCE  65
#define IppDebug_RegNum_IPE  321
#define IppDebug_RegNum_IME  55
#define IppDebug_RegNum_DIS  28
#define IppDebug_RegNum_FDE  176
#define IppDebug_RegNum_IFE  32

//#NT#2010/04/23#Linkin Huang -begin
//#NT# used for writing engine's register info
void IppDebug_WriteEngineRegInfo(char *Filename, IppDebug_EngineSel DbgEngine)
{
  UINT32 filesize    = 0;
  UINT32 EngBaseAddr = 0x0;
  int WriteFileStatus;

  switch(DbgEngine)
  {
    case IppDebug_SIE:
        EngBaseAddr = (UINT32)IppDebug_SIE_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_SIE;
        break;

    case IppDebug_PRE:
        EngBaseAddr = (UINT32)IppDebug_PRE_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_PRE;
        break;

    case IppDebug_DCE:
        EngBaseAddr = (UINT32)IppDebug_DCE_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_DCE;
        break;
        
    case IppDebug_IPE:
        EngBaseAddr = (UINT32)IppDebug_IPE_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_IPE;
        break;
        
    case IppDebug_IME:
        EngBaseAddr = (UINT32)IppDebug_IME_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_IME;
        break;
        
    case IppDebug_DIS:
        EngBaseAddr = (UINT32)IppDebug_DIS_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_DIS;
        break;
        
    case IppDebug_FDE:
        EngBaseAddr = (UINT32)IppDebug_FDE_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_FDE;
        break;
        
    case IppDebug_IFE:
        EngBaseAddr = (UINT32)IppDebug_IFE_ADDR;
        filesize    = 4 * (UINT32)IppDebug_RegNum_IFE;
        break;

    default:
        debug_err(("^R[ IPP Debug ] - Engine Selection error\r\n"));
        break;        
  }


  FilesysWaitCmdFinish(FST_TIME_INFINITE);
  WriteFileStatus = FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, Filename, (UINT8*)EngBaseAddr, &filesize, 0, FST_TIME_NO_WAIT);
  FilesysWaitCmdFinish(FST_TIME_INFINITE);
  

  if(WriteFileStatus != FST_STA_OK)
  {
    #if IppDebug_wfMsgShow
    debug_err(("^R[ IPP Debug ] : Write file(%s) from Addr(0x%08x) ...fail\r\n", Filename, DbgEngine));   
    #endif
  }
  else
  {
    #if IppDebug_wfMsgShow
    debug_msg("^Y[ IPP Debug ] - Write file : %s ...done\r\n", Filename);
    #endif
  }
}
//#NT#2010/04/23#Linkin Huang -end

//#NT#2010/07/20#Linkin Huang -begin
//#NT# new for Encode jpeg file (used only for debug)
ER JpegWriteOutUsedForTest(JpegWriteInfo *JpgInfo)
{
    ER rtStatus = E_OK;

#if 0
    JPGCmpParam myJPGParam = {0};
    UINT8 ucJPEGQuality = 0;
    UINT MainImgSize, retry;
    UINT32 myJpegSize = 0;

    myJPGParam.RawImageAddr[0] = JpgInfo->AddrY;
    myJPGParam.RawImageAddr[1] = JpgInfo->AddrCb;
    myJPGParam.RawImageAddr[2] = JpgInfo->AddrCr;    
    myJPGParam.JPGBSAddr      = JpgInfo->AddrJpeg;      
    myJPGParam.JPGBSBufSize   = (UINT *)JpgInfo->BufSizeJpeg;
    myJPGParam.Width          = JpgInfo->width;
    myJPGParam.Height         = JpgInfo->height;
    myJPGParam.LineOffsetY    = JpgInfo->LineOffsetY;
    myJPGParam.LineOffsetCbCr = JpgInfo->LineOffsetCx;
    myJPGParam.Mode           = JpgInfo->JpegMode;
    //#NT#2011/06/16#Ben Wang -begin
    //#NT#refine UserJpeg API
    //myJPGParam.bOuputJpgDC    = JpgInfo->JpegDC;
    myJPGParam.DCOutCfg.DCEnable = JpgInfo->JpegDC;
    //#NT#2011/06/16#Ben Wang -end

    ucJPEGQuality = JpgInfo->JpegQv;

    
    retry = 0;
    rtStatus = E_OK;

    while(retry < 25)
    {
        MainImgSize = GetJpgBufSize();

        rtStatus = JpegEncOnePic(&myJPGParam, (ucJPEGQuality - (retry * 5)));

        if (E_OK == rtStatus)
        {
            break;
        }
        retry ++;
    }  

   
   //rtStatus = JpegEncOnePic(&myJPGParam, ucJPEGQuality);
   if (rtStatus == E_OK)
   {
        debug_msg("Jpeg encode OK ...\r\n");

        rtStatus = E_OK;
   }
   else
   {
        debug_msg("Jpeg encode error ...\r\n");

        rtStatus = E_SYS;
   }

   myJpegSize = *(myJPGParam.JPGBSBufSize);   
   FilesysWaitCmdFinish(FST_TIME_INFINITE);
   FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, JpgInfo->filename, (UINT8*)myJPGParam.JPGBSAddr, &myJpegSize, 0, FST_TIME_NO_WAIT);
   FilesysWaitCmdFinish(FST_TIME_INFINITE);
#endif

   return rtStatus;
}
//#NT#2010/07/20#Linkin Huang -end



