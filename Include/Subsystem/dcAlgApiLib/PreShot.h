/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: PreShot Header                                                  *
* Description:                                                             *
* Author:                                                                  *
****************************************************************************/

#ifndef _PRE_SHOT_H
#define _PRE_SHOT_H

typedef struct PRESHOT_INFO_STRUCT
{
    UINT32 PrvVideo2AddrY;
    UINT32 PrvVideo2AddrCb;
    UINT32 PrvVideo2AddrCr;
    UINT32 PrvVideo2SizeV;
    UINT32 PrvVideo2SizeH;
    UINT32 PrvVideo2LineOffset;
    UINT32 CapBufAddr;
    UINT32 CapSizeV;
    UINT32 CapSizeH;
    UINT32 CapLineOffset;
    UINT32 CurIdx;

} PRESHOT_INFO;

typedef enum {
    PRESHOT_0=0,
    PRESHOT_1
} PRESHOT_NUMBER;

UINT32 PSGetCapCounter(void);
void PSCapCounterReset(void);
void PSCapCounterAdd(void);
void PSParamInit(void);
void PSCopyPrvYCC(void);
//void PSMergeCapRAW(void);
void PSSetCouplePrv(void);
void PSSetIDE(pIME_FBADDR_PRAM pFB_Para);

#endif
