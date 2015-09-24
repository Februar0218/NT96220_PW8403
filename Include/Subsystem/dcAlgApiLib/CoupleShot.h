/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Couple Shot Header                                                  *
* Description:                                                             *
* Author:                                                                  *
****************************************************************************/

#ifndef _COUPLE_SHOT_H
#define _COUPLE_SHOT_H

typedef struct COUPLESHOT_INFO_STRUCT
{
    UINT32 PrvVideo2AddrY;
    UINT32 PrvVideo2AddrCb;
    UINT32 PrvVideo2AddrCr;
    UINT32 PrvVideo2SizeV;
    UINT32 PrvVideo2SizeH;
    UINT32 PrvVideo2LineOffset;
    UINT32 CapBufAddr0;
    UINT32 CapBufAddr1;
    UINT32 CapSizeV;
    UINT32 CapSizeH;
    UINT32 CapLineOffset;
    UINT32 CurIdx;

} COUPLESHOT_INFO;

typedef enum {
    COUPLESHOT_0=0,
    COUPLESHOT_1
} COUPLESHOT_NUMBER;

UINT32 CSGetCapCounter(void);
void CSCapCounterReset(void);
void CSCapCounterAdd(void);
void CSParamInit(void);
void CSCopyPrvYCC(void);
void CSMergeCapRAW(void);
void CSSetCouplePrv(void);
void CSSetIDE(pIME_FBADDR_PRAM pFB_Para);

#endif
