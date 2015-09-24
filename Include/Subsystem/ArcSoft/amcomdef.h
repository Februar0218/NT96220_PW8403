/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
and confidential information.

The information and code contained in this file is only for authorized ArcSoft
employees to design, create, modify, or review.

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy,
distribute, modify, or take any action in reliance on it.

If you have received this file in error, please immediately notify ArcSoft and
permanently delete the original and any copy of any file and any printout
thereof.
*******************************************************************************/
#ifndef _ARCSOFT_BASE_H_
#define _ARCSOFT_BASE_H_

#define _DM350_

/************************************************************************/
/*                           Comman Definition                          */
/************************************************************************/
typedef long                    MLong;
typedef float                   MFloat;
typedef double                  MDouble;
typedef unsigned char           MByte;
typedef unsigned short          MWord;
typedef unsigned long           MDWord;
typedef void*                   MHandle;
typedef char                    MChar;
typedef long                    MBool;
typedef void                    MVoid;
typedef void*                   MPVoid;
typedef char*                   MPChar;
typedef short                   MShort;
typedef const char*             MPCChar;
typedef MLong                   MRESULT;
typedef MDWord                  MCOLORREF;

typedef signed      char        MInt8;
typedef unsigned    char        MUInt8;
typedef signed      short       MInt16;
typedef unsigned    short       MUInt16;
typedef signed      long        MInt32;
typedef unsigned    long        MUInt32;

#if defined(_MSC_VER)
typedef signed      __int64     MInt64;
typedef unsigned    __int64     MUInt64;
#else
typedef signed      long long   MInt64;
typedef unsigned    long long   MUInt64;
#endif

typedef struct __tag_rect
{
    MLong left;
    MLong top;
    MLong right;
    MLong bottom;
} MRECT, *PMRECT;

typedef struct __tag_point
{
    MLong x;
    MLong y;
} MPOINT, *PMPOINT;

#define MTChar MChar

#define MNull   0
#define MFalse  0
#define MTrue   1

#define MMAX(x,y) (((x)>=(y))?(x):(y))
#define MMIN(x,y) (((x)<=(y))?(x):(y))

/************************************************************************/
/*                           Error Definition                           */
/************************************************************************/

#define MERR_NONE    0
#define MOK          0

#define MERR_BASIC_BASE                 0X0001
#define MERR_UNKNOWN                    MERR_BASIC_BASE
#define MERR_INVALID_PARAM              (MERR_BASIC_BASE+1)
#define MERR_UNSUPPORTED                (MERR_BASIC_BASE+2)
#define MERR_NO_MEMORY                  (MERR_BASIC_BASE+3)
#define MERR_BAD_STATE                  (MERR_BASIC_BASE+4)
#define MERR_USER_CANCEL                (MERR_BASIC_BASE+5)
#define MERR_EXPIRED                    (MERR_BASIC_BASE+6)
#define MERR_USER_PAUSE                 (MERR_BASIC_BASE+7)
#define MERR_BUFFER_OVERFLOW            (MERR_BASIC_BASE+8)
#define MERR_BUFFER_UNDERFLOW           (MERR_BASIC_BASE+9)
#define MERR_NO_DISKSPACE               (MERR_BASIC_BASE+10)

#define MERR_FILE_BASE                  0X1000
#define MERR_FILE_GENERAL               MERR_FILE_BASE
#define MERR_FILE_NOT_EXIST             (MERR_FILE_BASE+1)
#define MERR_FILE_EXIST                 (MERR_FILE_BASE+2)
#define MERR_FILE_EOF                   (MERR_FILE_BASE+3)
#define MERR_FILE_FULL                  (MERR_FILE_BASE+4)
#define MERR_FILE_SEEK                  (MERR_FILE_BASE+5)
#define MERR_FILE_READ                  (MERR_FILE_BASE+6)
#define MERR_FILE_WRITE                 (MERR_FILE_BASE+7)
#define MERR_FILE_OPEN                  (MERR_FILE_BASE+8)
#define MERR_FILE_DELETE                (MERR_FILE_BASE+9)
#define MERR_FILE_RENAME                (MERR_FILE_BASE+10)

#define MERR_KERNEL_BASE                0X2000
#define MERR_KERNEL_GENERAL             MERR_KERNEL_BASE
#define MERR_THREAD_CREATE              (MERR_KERNEL_BASE+1)
#define MERR_THREAD_SET_PRIORITY        (MERR_KERNEL_BASE+2)

#define MERR_NET_BASE                   0X3000
#define MERR_NET_GENERAL                MERR_NET_BASE
#define MERR_SOCKET_READ                (MERR_NET_BASE+1)
#define MERR_SOCKET_WRITE               (MERR_NET_BASE+2)
#define MERR_SOCKET_CONNECT             (MERR_NET_BASE+3)
#define MERR_HTTP_GENERAL               (MERR_NET_BASE+4)
#define MERR_HTTP_DATA_NOT_READY        (MERR_NET_BASE+5)
#define MERR_HTTP_EOF                   (MERR_NET_BASE+6)
#define MERR_HTTP_TIMEOUT               (MERR_NET_BASE+7)
#define MERR_HTTP_REQUEST_FAIL          (MERR_NET_BASE+8)
#define MERR_HTTP_NOBUFFERS             (MERR_NET_BASE+9)
#define MERR_HTTP_RESPONSE_300          (MERR_HTTP_REQUEST_FAIL+300)
#define MERR_HTTP_RESPONSE_301          (MERR_HTTP_REQUEST_FAIL+301)
#define MERR_HTTP_RESPONSE_302          (MERR_HTTP_REQUEST_FAIL+302)
#define MERR_HTTP_RESPONSE_303          (MERR_HTTP_REQUEST_FAIL+303)
#define MERR_HTTP_RESPONSE_304          (MERR_HTTP_REQUEST_FAIL+304)
#define MERR_HTTP_RESPONSE_305          (MERR_HTTP_REQUEST_FAIL+305)
#define MERR_HTTP_RESPONSE_307          (MERR_HTTP_REQUEST_FAIL+307)
#define MERR_HTTP_RESPONSE_400          (MERR_HTTP_REQUEST_FAIL+400)
#define MERR_HTTP_RESPONSE_401          (MERR_HTTP_REQUEST_FAIL+401)
#define MERR_HTTP_RESPONSE_402          (MERR_HTTP_REQUEST_FAIL+402)
#define MERR_HTTP_RESPONSE_403          (MERR_HTTP_REQUEST_FAIL+403)
#define MERR_HTTP_RESPONSE_404          (MERR_HTTP_REQUEST_FAIL+404)
#define MERR_HTTP_RESPONSE_405          (MERR_HTTP_REQUEST_FAIL+405)
#define MERR_HTTP_RESPONSE_406          (MERR_HTTP_REQUEST_FAIL+406)
#define MERR_HTTP_RESPONSE_407          (MERR_HTTP_REQUEST_FAIL+407)
#define MERR_HTTP_RESPONSE_408          (MERR_HTTP_REQUEST_FAIL+408)
#define MERR_HTTP_RESPONSE_409          (MERR_HTTP_REQUEST_FAIL+409)
#define MERR_HTTP_RESPONSE_410          (MERR_HTTP_REQUEST_FAIL+410)
#define MERR_HTTP_RESPONSE_411          (MERR_HTTP_REQUEST_FAIL+411)
#define MERR_HTTP_RESPONSE_412          (MERR_HTTP_REQUEST_FAIL+412)
#define MERR_HTTP_RESPONSE_413          (MERR_HTTP_REQUEST_FAIL+413)
#define MERR_HTTP_RESPONSE_414          (MERR_HTTP_REQUEST_FAIL+414)
#define MERR_HTTP_RESPONSE_415          (MERR_HTTP_REQUEST_FAIL+415)
#define MERR_HTTP_RESPONSE_417          (MERR_HTTP_REQUEST_FAIL+417)
#define MERR_HTTP_RESPONSE_500          (MERR_HTTP_REQUEST_FAIL+500)
#define MERR_HTTP_RESPONSE_501          (MERR_HTTP_REQUEST_FAIL+501)
#define MERR_HTTP_RESPONSE_502          (MERR_HTTP_REQUEST_FAIL+502)
#define MERR_HTTP_RESPONSE_503          (MERR_HTTP_REQUEST_FAIL+503)
#define MERR_HTTP_RESPONSE_504          (MERR_HTTP_REQUEST_FAIL+504)
#define MERR_HTTP_RESPONSE_505          (MERR_HTTP_REQUEST_FAIL+505)

#define MERR_CAM_BASE                   0X4000
#define MERR_CAM_GENERAL                MERR_CAM_BASE
#define MERR_CAM_INIT                   (MERR_CAM_BASE+1)
#define MERR_CAM_PREVIEW_START          (MERR_CAM_BASE+2)
#define MERR_CAM_PREVIEW_STOP           (MERR_CAM_BASE+3)
#define MERR_CAM_FRAME_GET              (MERR_CAM_BASE+4)
#define MERR_CAM_PROPERTY_SET           (MERR_CAM_BASE+5)
#define MERR_CAM_PROPERTY_GET           (MERR_CAM_BASE+6)
#define MERR_CAM_PARAM_SET              (MERR_CAM_BASE+7)
#define MERR_CAM_CAPTURE_START          (MERR_CAM_BASE+8)
#define MERR_CAM_CAPTURE_STOP           (MERR_CAM_BASE+9)

#define MERR_DISPLAY_BASE               0X5000
#define MERR_DISPLAY_GENERAL            MERR_DISPLAY_BASE
#define MERR_DISPLAY_ALREADY_INIT       (MERR_DISPLAY_BASE + 1)
#define MERR_DISPLAY_INIT_FAIL          (MERR_DISPLAY_BASE + 2)
#define MERR_DISPLAY_UNINIT_FAIL        (MERR_DISPLAY_BASE + 3)
#define MERR_DISPLAY_NOT_INIT           (MERR_DISPLAY_BASE + 4)
#define MERR_DISPLAY_DEVICE_FAIL        (MERR_DISPLAY_BASE + 5)
#define MERR_DISPLAY_DRAW_FAIL          (MERR_DISPLAY_BASE + 6)
#define MERR_DISPLAY_SHOWOVERLAY_FAIL    (MERR_DISPLAY_BASE + 7)

#define MERR_AUDIO_BASE                 0X6000
#define MERR_AUDIO_GENERAL              MERR_AUDIO_BASE
#define MERR_AUDIO_OUTPUT_OPEN          (MERR_AUDIO_BASE + 1)
#define MERR_AUDIO_OUTPUT_SETVOL        (MERR_AUDIO_BASE + 2)
#define MERR_AUDIO_OUTPUT_GETVOL        (MERR_AUDIO_BASE + 3)
#define MERR_AUDIO_OUTPUT_CLOSE         (MERR_AUDIO_BASE + 4)
#define MERR_AUDIO_OUTPUT_PAUSE         (MERR_AUDIO_BASE + 5)
#define MERR_AUDIO_OUTPUT_STOP          (MERR_AUDIO_BASE + 6)
#define MERR_AUDIO_OUTPUT_PLAYING       (MERR_AUDIO_BASE + 7)
#define MERR_AUDIO_OUTPUT_RESUME        (MERR_AUDIO_BASE + 8)

#define MERR_AUDIO_INPUT_BASE           (MERR_AUDIO_GENERAL + 50)
#define MERR_AUDIO_INPUT_OPEN           (MERR_AUDIO_INPUT_BASE + 9)
#define MERR_AUDIO_INPUT_CLOSE          (MERR_AUDIO_INPUT_BASE + 10)
#define MERR_AUDIO_INPUT_PAUSE          (MERR_AUDIO_INPUT_BASE + 11)
#define MERR_AUDIO_INPUT_STOP           (MERR_AUDIO_INPUT_BASE + 12)
#define MERR_AUDIO_INPUT_RECORDING      (MERR_AUDIO_INPUT_BASE + 13)
#define MERR_AUDIO_INPUT_RESUME         (MERR_AUDIO_INPUT_BASE + 14)

/************************************************************************/
/*                           Memory Interface                           */
/************************************************************************/
typedef struct __tag_mem_info
{
    MDWord    dwTotalMemUsed;
    MDWord    dwTotalMemFree;
}MMEMINFO, *LPMMEMINFO;

#ifdef __cplusplus
extern "C" {
#endif

MVoid* MMemAlloc(MHandle hContext, MLong lSize);
MVoid* MMemRealloc(MHandle hContext, MVoid* pMem, MLong lSize);
MVoid   MMemFree(MHandle hContext, MVoid* pMem);
MVoid   MMemSet(MVoid* pMem, MByte byVal, MLong lSize);
MVoid   MMemCpy(MVoid* pDst, const MVoid* pSrc, MLong lSize);
MVoid   MMemMove(MVoid* pDst, MVoid* pSrc, MLong lSize);
MLong   MMemCmp(MVoid* pBuf1, MVoid* pBuf2, MLong lSize);

MHandle MMemMgrCreate(MVoid* pMem, MLong lMemSize);
MVoid   MMemMgrDestroy(MHandle hMemMgr);

MBool   MGetMemInfo(LPMMEMINFO pMemInfo);

#ifdef __cplusplus
}
#endif

#endif // _ARCSOFT_BASE_H_
