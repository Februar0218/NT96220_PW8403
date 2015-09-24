//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
    /*************************************************************************

Copyright (C) Microsoft Corporation, 1996 - 1999

Module Name:

    MsAudioDec.h

Abstract:

    Header file for CAudioObjectDecoder.

Author:

    Wei-ge Chen (wchen) 14-July-1998

Revision History:


*************************************************************************/

#ifndef __MSAUDIODEC_H_
#define __MSAUDIODEC_H_

#include <math.h>
#include <limits.h>

#include "wmad_config.h" //@Ryan
#include "wmaerr.h"
#include "wmatypes.h"
#include "msaudio.h"
#include "strmdec_wma.h"
#include "audec_apil.h"
#include "wmaudio.h"
//#include "wmaudio_type.h"

#define MIN_OBJECT_SIZE     24
#define DATA_OBJECT_SIZE    50

typedef enum {BEGIN_PACKET, BEGIN_FRAME, BEGIN_SUBFRAME, DECODE_SUBFRAME, END_SUBFRAME1, END_SUBFRAME2} DecodeStatus;
typedef enum {SUBFRM_HDR, SUBFRM_COEF, SUBFRM_DONE} SubFrmDecodeStatus;
typedef enum {HDR_SIZE, HDR_QUANT, HDR_NOISE1, HDR_NOISE2, HDR_MSKUPD, HDR_BARK, HDR_DONE} HdrDecodeStatus; //and a lot more
typedef enum {VLC, FRAME_END, NORMAL, ESCAPE} RunLevelStatus;

typedef struct WMA_PAYLOADEXTNSYSTEM
{
    GUID GuidExtnSystemId;
    WORD u16ExtnDataSize;
} WMAPAYLOADEXTNSYSTEM;

typedef struct WMA_PAYLOADEXTNSYSTEMLIST
{
    WORD u16ExtnSysCount;
    WMAPAYLOADEXTNSYSTEM  *pExtSys;
} WMAPAYLOADEXTNSYSTEMLIST;

/* parser state  */
typedef enum
{
    csWMA_NotValid = 0,
    csWMA_HeaderStart,
    csWMA_HeaderError,
    csWMA_NewAsfPacket,
    /* packet parsing states */
    csWMA_DecodePayloadStart,
    csWMA_DecodePayload,
    csWMA_DecodePayloadHeader,
    csWMA_DecodeLoopStart,
    csWMA_DecodePayloadEnd,
    csWMA_DecodeCompressedPayload,
    csWMA_End_
} tWMAParseState;


/* internal structure for the ASF header parsing */
typedef struct tagWMAFileHdrStateInternal
{
    DWORD    currPacketOffset;     /* not just for packets */
    DWORD    nextPacketOffset;
    DWORD    pastPacketOffset;
    /* ASF header */
    DWORD    cbHeader;
    DWORD    cbPacketSize;
    DWORD    cbAudioSize;
    DWORD    cPackets;
    DWORD    msDuration;
    DWORD    msPreroll;
    DWORD    cbFirstPacketOffset;
    DWORD    cbLastPacketOffset;
    /* audio prop */
    WORD    nVersion;
    DWORD    nSamplesPerSec;
    DWORD    nAvgBytesPerSec;
    DWORD    nBlockAlign;
    WORD    nChannels;
    DWORD    nSamplesPerBlock;
    WORD    nEncodeOpt;
    /* DRM */
    BYTE                        pbSecretData[32];
    DWORD                       cbSecretData;
    BYTE                        pbType[16];
    BYTE                        pbKeyID[32];
    /* Content Description */
    DWORD                       cbCDOffset;
    WORD                        cbCDTitle;
    WORD                        cbCDAuthor;
    WORD                        cbCDCopyright;
    WORD                        cbCDDescription;
    WORD                        cbCDRating;
    /* Extended Content Description */
    tWMAExtendedContentDesc     *m_pECDesc;
    /* Marker */
    DWORD                       m_dwMarkerNum;
    MarkerEntry                 *m_pMarker;
    /* License Store */
    DWORD                       m_dwLicenseLen;
    BYTE                           *m_pLicData;
    WORD                        wAudioStreamId;
    DWORD                       dwCryptoVersion;
    DWORD                       cbUDLData;
    BYTE                        *pbUDLData;
    wmaBOOL                        bHasDRM;
    WMAPAYLOADEXTNSYSTEMLIST     ExtSysList[127];
    DWORD                       dwUDLRepIndex;
} tWMAFileHdrStateInternal;

/* internal structure for the WMAudio library state */
typedef struct tagWMAFileStateInternal {
    tWMAFileHdrStateInternal hdr_parse;
    /* parse_state */
    tWMAParseState parse_state;
    /* payload parsing */
    DWORD cbPayloadOffset;
    DWORD cbPayloadLeft;
    wmaBOOL  bBlockStart;
    DWORD cbBlockLeft;

    /* packet parsing */
    PACKET_PARSE_INFO_EX ppex;
    PAYLOAD_MAP_ENTRY_EX payload;
    DWORD iPayload;
    WORD wPayStart;
    /* other states */
    wmaBOOL    bHasDRM;
    DWORD   nSampleCount;
    /* objects */
//    tWMAState wma_state;
//    WMADHANDLE hWMA;
    //WMARawDecHandle hWMA;
    void    *hWMA;
    BYTE bDecInWaitState;
    BYTE bAllFramesDone;
    BYTE bFirst;
    BYTE *pCallBackBuffer;
} tWMAFileStateInternal;

typedef struct CAudioObjectDecoder 
{
    CAudioObject* pau;
    //packetization stuff
    Bool  m_fPacketLoss;
    I16   m_cFrmInPacket;
    U8*   m_pbSrcCurr;       //ptr to src of curr packet, only for non-strm mode
    U16   m_cbSrcCurrLength; //only for non-strm mode
    DecodeStatus           m_decsts;
    SubFrmDecodeStatus     m_subfrmdecsts;
    HdrDecodeStatus        m_hdrdecsts;
    RunLevelStatus         m_rlsts;

    I16                   m_iChannel;  //current channel we are doing
    I16                    m_iBand;     //current band we are doing
    Bool                m_fNoMoreData;

    Bool                m_fLastSubFrame;
    CWMAInputBitStream    m_ibstrm;

    tWMAFileStateInternal m_hState;

    //I don't think we need any of these now
    I16*        m_rgiRunEntry44ssQb;
    I16*        m_rgiLevelEntry44ssQb;
    I16*        m_rgiRunEntry44smQb;
    I16*        m_rgiLevelEntry44smQb;
    I16*        m_rgiRunEntry44ssOb;
    I16*        m_rgiLevelEntry44ssOb;
    I16*        m_rgiRunEntry44smOb;
    I16*        m_rgiLevelEntry44smOb;

    I16*        m_rgiRunEntry16ssOb;
    I16*        m_rgiLevelEntry16ssOb;
    I16*        m_rgiRunEntry16smOb;
    I16*        m_rgiLevelEntry16smOb;

//@Ryan: Emilinated for not used code
//#if 0
//    UInt        m_rgiQuantStepFract[QUANTSTEP_DENOMINATOR - 1]; // Exclude numerator of 0
//#endif    // !defined(SILSMOD)
//#if defined(UNDER_CE) && defined(_ARM_) && !defined(WMA_TARGET_MS_ARM)
//    U32         lPrivate1;  // Used by ARMStub to keep track of ARM structures
//    U32         lPrivate2;  // Used by ARMStub to keep track of current buffer pos
//#endif
//#ifdef ENABLE_EQUALIZER
//#define MAX_NO_EQBAND 10
//    Float       m_rgfltEqBandWeight[MAX_NO_EQBAND];
//    U16         m_rgwEqBandBoundary[MAX_NO_EQBAND + 1];
//    I8          m_cEqBand;
//    Bool        m_fNoEq; 
//    I8          m_rgbBandPower[MAX_NO_EQBAND];
//    Bool        m_fComputeBandPower;
//    Int         m_iEqFrameSize;
//#endif // ENABLE_EQUALIZER
//#if defined(SEEK_DECODE_TEST)
//    tRandState *m_pSaveRandState;
//#endif // SEEK_DECODE_TEST

    WMARESULT (*m_pfnDecodeSubFrame) (struct CAudioObjectDecoder* paudec, Int* piBitCnt);
    WMARESULT (*m_pfnDecodeCoefficient) (struct CAudioObjectDecoder* paudec, PerChannelInfo* ppcinfo, Int* piBitCnt);
    
    //@Ryan: added for initialization
    AUDECFlow_t (*m_pfnInput)    (void *, void *);
    AUDECFlow_t (*m_pfnHeader)    (void *, void *);
    AUDECFlow_t (*m_pfnFilter)    (void *, void *);
    AUDECFlow_t (*m_pfnOutput)    (void *, void *);
    AUDECFlow_t (*m_pfnError)    (void *, void *);
    AUDECFlow_t (*m_pfnMessage)    (void *, void *);
} CAudioObjectDecoder;

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif
//public:
CAudioObjectDecoder* audecNew(void *pMemBuf, const I32 iMemBufSize);
Void audecDelete (CAudioObjectDecoder* paudec);

WMARESULT audecInit (
    CAudioObjectDecoder* paudec, 
    Int iVersionNumber,
    Int cSubband, 
    Int iSamplingRate, 
    U16 cChannel, 
    Int cBytePerSec, 
    Int cbPacketLength,
    U16 wEncodeOpt,
    U16 wPlayerOpt,
    PFNGETMOREDATA pfnGetMoreData,
    U32 dwUserData,
    I32 *piMemBufUsed);
WMARESULT audecReset (CAudioObjectDecoder* paudec);
WMARESULT audecFlush (
    CAudioObjectDecoder* paudec, 
    U8   *pbDst, 
    Int   cbDstLength,
    Int*  pcbDstUsed,
    U8*   pbEqu,
    Int   cbEquLength,
    Int*  pcbEquUsed,
    U32*  pcSamplesFromBefore);

U32 audecOutputBufferSize(Int iVersion, Int iSamplingRate, Int cChannels, Int cBytePerSec, U16 wEncodeOpt);

//streaming decoding interface
WMARESULT audecDecodeInfo (CAudioObjectDecoder* paudec);
WMARESULT audecDecodeData (CAudioObjectDecoder* paudec, U16* pcSampleReady);
WMARESULT audecGetPCM (CAudioObjectDecoder* paudec, U16* pcSampleReady, U8* pbDst, U32 cbDstLength);

// This function is optionally implemented by the user, but if the user opts for
// non-streaming mode, we supply a standard implementation. Streaming-mode callers
// MUST supply their own version of this function.
WMARESULT prvWMAGetMoreData(U8 **ppBuffer, U32 *pcbBuffer, U32 dwUserData);

//alternative decoding interface
WMARESULT audecDecode (CAudioObjectDecoder* paudec, 
                     const U8*   pbSrc,
                     Int         cbSrcLength,
                     Int*        pcbSrcUsed,
                     U8*         pbDst,
                     Int         cbDstLength,
                     Int*        pcbDstUsed,
                     U8*         pbEqu,
                     Int         cbEqu,
                     Int*        pcbEquUsed,
                     I32*        pcSamplesFromBefore);

#ifdef ENABLE_EQUALIZER            
WMARESULT audecSetEqualizer (CAudioObjectDecoder* paudec, Int iBand, Int iGainIndB);
WMARESULT audecResetEqualizer (CAudioObjectDecoder* paudec);  
Void    audecStartComputeBandPower (CAudioObjectDecoder* paudec);
Void    audecStopComputeBandPower (CAudioObjectDecoder* paudec);
WMARESULT audecGetBandPower (CAudioObjectDecoder* paudec, I8* rgbBandPower);
#endif //ENABLE_EQUALIZER
#ifdef __cplusplus
}
#endif

//private:
WMARESULT prvDecodePacket (CAudioObjectDecoder* paudec,
                           const U8*    pbSrc,
                           Int          cbSrcLength,
                           Int*         pcbSrcUsed,
                           U8*          pbDst,
                           Int          cbDstLength,
                           Int*         pcbDstUsed,
                           U8*          pbEqu,
                           Int          cbEquLength,
                           Int*         pcbEquUsed,
                           I32*         pcSamplesPrevPacket);
WMARESULT prvDecodeFrame (CAudioObjectDecoder* paudec,
                        const U8*   pbSrc, 
                        Int         cbSrcLength, 
                        Int*        pcbSrcUsed, 
                        I16*        piDst, 
                        Int         cbDstLength,
                        Int*        pcbDstUsed,
                        U8*         pbEqu,
                        Int         cbEquLength,
                        Int*        pcbEquUsed);

WMARESULT prvDecodeSubFrame (CAudioObjectDecoder* paudec, Int* piBitCnt);
WMARESULT prvDecodeSubFrameHighRate (CAudioObjectDecoder* paudec, Int* piBitCnt);

WMARESULT prvDecodeFrameHeader (CAudioObjectDecoder* paudec, Int* piBitCnt);
Void    prvSetDetTable  (CAudioObjectDecoder* paudec,  PerChannelInfo* ppcinfo);
WMARESULT prvDecodeFrameHeaderLpc (CAudioObjectDecoder* paudec, Int* piBitCnt);
WMARESULT prvDecodeFrameHeaderNoiseSub (CAudioObjectDecoder* paudec, Int* piBitCnt);
WMARESULT prvGetNextRunDEC (Void* pvoid, PerChannelInfo* ppcinfo, Int *piBitCnt);

// Void    prvGetBandWeightMidRate (CAudioObjectDecoder* paudec);
Void    prvGetBandWeightLowRate (CAudioObjectDecoder* paudec);
WMARESULT prvUpdateSubFrameConfig (CAudioObjectDecoder* paudec, Int iSizeNext, Int iSizePrev, Int iSizeCurr);

WMARESULT prvDecodeCoefficientMono    
                (CAudioObjectDecoder* paudec, PerChannelInfo* ppcinfo, Int* piBitCnt);
WMARESULT prvDecodeCoefficientStereo 
                (CAudioObjectDecoder* paudec, PerChannelInfo* ppcinfo, Int* piBitCnt);
WMARESULT prvDecodeRunLevel  (CAudioObjectDecoder* paudec, PerChannelInfo* ppcinfo,
                              Int iCoefFirst, 
                              Int iCoefLim, 
                              Int* piBitCnt);

//equalize
Void    prvAdaptEqToSubFrame(CAudioObjectDecoder* paudec);
WMARESULT prvEqualize (CAudioObjectDecoder* paudec, PerChannelInfo* ppcinfo);
Void    prvComputeBandPower (CAudioObjectDecoder* paudec);

INLINE Int WMA_get_fNoMoreData(U32 hWMA)
{
    return ((CAudioObjectDecoder *)hWMA)->m_fNoMoreData;
}

INLINE Void WMA_set_fNoMoreData(U32 hWMA, Int iVal)
{
    ((CAudioObjectDecoder *)hWMA)->m_fNoMoreData = iVal;
}

INLINE U8 WMA_get_nHdrBits(U32 hWMA)
{
    const CAudioObject *pau = ((CAudioObjectDecoder *)hWMA)->pau;

    if (pau->m_fAllowSuperFrame)
        return (U8) (pau->m_cBitPackedFrameSize + NBITS_FRM_CNT + NBITS_PACKET_CNT + 3);
    else
        return 0;
}

#endif  //__MSAUDIODEC_H_
