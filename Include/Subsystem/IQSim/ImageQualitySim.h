#ifndef IQSIM_H
#define IQSIM_H

#include "Type.h"
#include "Debug.h"

typedef struct _BUF_ALLOC
{
    UINT32 tAddr; //total buffer addr
    UINT32 tSize; //total buffer size
    UINT32 tEnd;  //total buffer boundary
    UINT32 cAddr; //free buffer addr
}
BUF_ALLOC;

void BUF_ALLOC_Assign(BUF_ALLOC* pUserBufAlloc);
void BUF_ALLOC_Open(UINT32 BufAddr, UINT32 BufSize);
UINT32 BUF_ALLOC_New(UINT32 BufSize);
void BUF_ALLOC_Close(void);

typedef enum
{
    IQ_FMT_RAW8 = 0x0,
    IQ_FMT_RAW10 = 0x1,
    IQ_FMT_Y8 = 0x10,
    IQ_FMT_YCC444 = 0x11,
    IQ_FMT_YCC422 = 0x12,
    IQ_FMT_YCC420 = 0x13,
    IQ_FMT_YCCJPG = 0x20,
    ENUM_DUMMY4WORD(IQ_BUF_FMT)
} IQ_BUF_FMT;

typedef enum
{
    IQ_CNL_ALL = 0,
    IQ_CNL_RAW = 1,
    IQ_CNL_Y = 2,
    IQ_CNL_Cb = 3,
    IQ_CNL_Cr = 4,
    ENUM_DUMMY4WORD(IQ_CNL)
} IQ_CNL;

typedef struct
{
    IQ_BUF_FMT fmt;
    UINT32 w;
    UINT32 h;
    UINT32 lineoff;
    UINT32 size;
    UINT32 addr[3];
} IQ_BUF;

void IQ_BUF_New(IQ_BUF* pBuf, IQ_BUF_FMT fmt, UINT32 w, UINT32 h, UINT32 lineoff);
void IQ_BUF_Attach(IQ_BUF* pBuf, IQ_BUF_FMT fmt, UINT32 w, UINT32 h, UINT32 lineoff,
    UINT32 size, UINT32 addr0, UINT32 addr1, UINT32 addr2);
void IQ_BUF_Clone(IQ_BUF* pBuf, IQ_BUF* pSrcBuf);
void IQ_BUF_CloneByChannel(IQ_BUF* pBuf, IQ_BUF* pSrcBuf, IQ_CNL cnl);
void IQ_BUF_CloneByRect(IQ_BUF* pBuf, IQ_BUF* pSrcBuf, UINT32 x, UINT32 y, UINT32 w, UINT32 h);
void IQ_BUF_CloneEx(IQ_BUF* pBuf, IQ_BUF* pSrcBuf, UINT32 x, UINT32 y, UINT32 w, UINT32 h, IQ_CNL cnl);
UINT32 IQ_BUF_GetPixelAddr(IQ_BUF* pBuf, UINT32 x, UINT32 y, IQ_CNL cnl);

void IQ_BUF_Copy(
    IQ_BUF* pBuf, 
    IQ_BUF* pSrcBuf);
void IQ_BUF_FillRect(
    IQ_BUF* pBuf, UINT32 x, UINT32 y, UINT32 w, UINT32 h, 
    UINT32 color);
void IQ_BUF_CopyRect(
    IQ_BUF* pBuf, UINT32 x, UINT32 y,
    IQ_BUF* pSrcBuf, UINT32 sx, UINT32 sy, UINT32 sw, UINT32 sh);

UINT32 IQ_BUF_Dump(IQ_BUF* pBuf, char* filename, UINT32 ci, UINT32 offset);


////////////////////////////////////////

extern BOOL bIQSimExec;
//memory
extern UINT32 uiTotalAddr;
extern UINT32 uiTotalSize;
//bin
extern UINT32 uiBinAddr;
extern UINT32 uiBinSize;
/*
//raw
extern UINT32 uiRawAddr;
extern UINT32 uiRawSize;
extern UINT32 uiRawWidth;
extern UINT32 uiRawHeight;
extern UINT32 uiRawBpp;
//jpg
extern UINT32 uiJpgAddr;
extern UINT32 uiJpgSize;
extern UINT32 uiJpgWidth;
extern UINT32 uiJpgHeight;
*/

//raw (sie in)
extern IQ_BUF gBuf_RAW;
//raw2 (sie out, iferaw in)
extern IQ_BUF gBuf_SIE;
//raw3 (iferaw out, ipe in)
extern IQ_BUF gBuf_IFE; //IFE-raw buf, overlap with gBuf_SIE
//ycc (ipe out, ime in, ime out, ifeycc in, ifeycc out, jpg in)
extern UINT32 uiImeSliceCount;
extern UINT32 uiImeSliceH;
extern IQ_BUF gBuf_IME[5];
//ycc2 (ifeycc in, ifeycc out)
extern IQ_BUF gBuf_IFE;
//jpg (jpg out)
extern IQ_BUF gBuf_JPG;

extern UINT32 IQSim_Init(void);
extern UINT32 IQSim_Exit(void);
extern UINT32 IQSim_Exec(void);
extern void IQSim_GenSourceCode(void);

#endif //IQSIM_H
