/*
    Header file for IPP Debug library

    @file          IPP_Debug.h
    @ingroup    mlIMBND
    @note         None

    Copyright (c) 2010 Novatek Microelectronics Corporation.  All Rights Reserved.

    Revision History:

*/

#ifndef _IPP_DEBUG_H_
#define _IPP_DEBUG_H_

#include <string.h>
#include <stdio.h>
#include  "type.h"
#include  "debug.h"

//#NT#2010/07/20#Linkin Huang -begin
//#NT# add jpeg.h
#include "JpgEnc.h"
//#NT#2010/07/20#Linkin Huang -end

#define IPP_DEBUG  DISABLE

typedef enum
{
  IppDebug_SIE = 1,
  IppDebug_PRE = 2,
  IppDebug_DCE = 3,
  IppDebug_IPE = 4,
  IppDebug_IME = 5,
  IppDebug_DIS = 6,
  IppDebug_FDE = 7,
  IppDebug_IFE = 8
} IppDebug_EngineSel;




//#NT#2010/07/20#Linkin Huang -begin
//#NT# new for Encode jpeg file (used only for debug)
typedef struct
{
  UINT32 width;
  UINT32 height;
  UINT32 LineOffsetY;
  UINT32 LineOffsetCx;
  UINT32 AddrY;
  UINT32 AddrCb;
  UINT32 AddrCr;
  UINT32 AddrJpeg;
  UINT32 BufSizeJpeg;
  UINT JpegMode;
  BOOL JpegDC;
  char *filename;
  UINT32 JpegQv;
} JpegWriteInfo;

extern ER JpegWriteOutUsedForTest(JpegWriteInfo *JpgInfo);
//#NT#2010/07/20#Linkin Huang -end

//#NT#2010/04/23#Linkin Huang -begin
//#NT# used for writing engine's register info
extern void IppDebug_WriteEngineRegInfo(char *Filename, IppDebug_EngineSel DbgEngine);
//#NT#2010/04/23#Linkin Huang -end

//#NT#2012/07/20#KS Hung -begin
//#NT#Set the height of 720P slice mode. It isn't a regular solution and IQ must fix it.
extern void IPPCtrl_IsSliceHeight256(BOOL bIsSliceH256);
//#NT#2012/07/20#KS Hung -end

#endif
