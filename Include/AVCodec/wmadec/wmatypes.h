//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*
 * wmatypes.h:
 * Copyright (C) ARM Limited 1999. All Rights Reserved.
 */

//+-------------------------------------------------------------------------
//
//  Microsoft Windows Media
//
//  Copyright (C) Microsoft Corporation, 1999 - 1999
//
//  File:       wmatypes.h
//
//--------------------------------------------------------------------------

#ifndef WMATYPES_H
#define WMATYPES_H

#ifndef __arm
    #ifdef UNDER_CE
        #define PACKED
    #else
        #ifdef _MSC_VER
            #define USE_PACK_HEADERS
            #define PACKED
        #elif defined( WMA_TARGET_MACOS )
            #define USE_PACK_HEADERS
        #else
            #define PACKED  __packed
        #endif

        #ifdef _Embedded_x86
        #define PACKED
        #endif

    #endif //UNDER_CE

    #ifdef USE_PACK_HEADERS
        #pragma warning( push )
        #pragma warning( disable : 4103 )
        #include <pshpack1.h>
        #pragma warning( pop )
    #endif
#else
    #ifdef PACKED
        #undef PACKED
        #define PACKED  __packed
    #endif
#endif //__arm


/****************************************************************************/
#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif /* __cplusplus */
#endif /* NULL */

#if defined(_M_MRX000) || defined(_M_ALPHA) || defined(_M_PPC)
#define UNALIGNED __unaligned
#elif defined(_WIN32_WCE) && !defined(_M_IX86)
// WinCE inc\winnt.h defined this
#ifndef UNALIGNED
#define UNALIGNED __unaligned
#endif
#else
#define UNALIGNED
#endif

#if defined(_Embedded_x86) || defined(__arm)
#define wmaFALSE 0
#define wmaTRUE  1
#endif


/****************************************************************************/
#ifndef _BOOL_DEFINED
#define _BOOL_DEFINED
typedef long wmaBOOL;
#endif /* _BOOL_DEFINED */

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char   BYTE;
#endif /* _BYTE_DEFINED */

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short  WORD;
#endif /* _WORD_DEFINED */

#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
typedef unsigned long    DWORD; /* actually long, but this should be okay */
#endif /* _DWORD_DEFINED */

#if defined (_Embedded_x86) || defined (_MIPS_) || defined (UNDER_CE)
#pragma pack (1)
#endif

#ifndef _QWORD_DEFINED
#define _QWORD_DEFINED

typedef struct tQWORD
{
    DWORD   dwLo;
    DWORD   dwHi;
}   QWORD;
#endif /* _QWORD_DEFINED */


/****************************************************************************/
#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef struct tGUID
{
    DWORD   Data1;
    WORD    Data2;
    WORD    Data3;
    BYTE    Data4[ 8 ];
}   GUID;

#endif


/****************************************************************************/
#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef __packed struct tWAVEFORMATEX
{
    WORD    wFormatTag;         /* format type */
    WORD    nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD   nSamplesPerSec;     /* sample rate */
    DWORD   nAvgBytesPerSec;    /* for buffer estimation */
    WORD    nBlockAlign;        /* block size of data */
    WORD    wBitsPerSample;     /* number of bits per sample of mono data */
    WORD    cbSize;             /* the count in bytes of the size of */

}   WAVEFORMATEX;
typedef const WAVEFORMATEX *LPCWAVEFORMATEX;

#endif

#define WAVE_FORMAT_MSAUDIO1  0x0160
typedef __packed struct tMSAUDIO1WAVEFORMAT
{
    WAVEFORMATEX wfx;
    WORD         wSamplesPerBlock;
    WORD         wEncodeOptions;

}   MSAUDIO1WAVEFORMAT;

#define WAVE_FORMAT_WMAUDIO2  0x0161
typedef __packed struct tWMAUDIO2WAVEFORMAT
{
    WAVEFORMATEX wfx;
    DWORD        dwSamplesPerBlock;
    WORD         wEncodeOptions;
    DWORD        dwSuperBlockAlign;

}   WMAUDIO2WAVEFORMAT;


/****************************************************************************/
typedef __packed struct tAsfXAcmAudioErrorMaskingData
{
    BYTE    span;
    WORD    virtualPacketLen;
    WORD    virtualChunkLen;
    WORD    silenceLen;
    BYTE    silence[1];

}   AsfXAcmAudioErrorMaskingData;


/****************************************************************************/
typedef __packed struct tAsfXSignatureAudioErrorMaskingData
{
    DWORD maxObjectSize;
    WORD  chunkLen;
    WORD  signatureLen;
    BYTE  signature[1];

}   AsfXSignatureAudioErrorMaskingData;

#if defined (_Embedded_x86) || defined (_MIPS_)  || defined (UNDER_CE)
#pragma pack ()
#endif

/****************************************************************************/
typedef struct tPACKET_PARSE_INFO_EX
{
    wmaBOOL fParityPacket;
    DWORD cbParseOffset;

    wmaBOOL fEccPresent;
    BYTE bECLen;

    BYTE bPacketLenType;
    DWORD cbPacketLenTypeOffset;

    BYTE bPadLenType;
    BYTE bSequenceLenType;

    wmaBOOL fMultiPayloads;

    BYTE bOffsetBytes;

    BYTE bOffsetLenType;

    DWORD cbPacketLenOffset;

    DWORD cbExplicitPacketLength; 

    DWORD cbSequenceOffset;
    
    DWORD dwSequenceNum; 

    DWORD cbPadLenOffset;
    DWORD cbPadding;

    DWORD dwSCR;
    WORD wDuration;

    BYTE bPayLenType;
    BYTE bPayBytes;
    DWORD cbPayLenTypeOffset;

    DWORD cPayloads;

}   PACKET_PARSE_INFO_EX;


/****************************************************************************/
typedef struct tPAYLOAD_MAP_ENTRY_EX
{
    WORD cbPacketOffset;
    WORD cbTotalSize;

    BYTE bStreamId;
    BYTE bObjectId;

    DWORD cbObjectOffset;
    DWORD cbObjectSize;
    DWORD msObjectPres;

    BYTE cbRepData;
    BYTE bJunk;
    WORD cbPayloadSize;
//         Added by Amit
//        BYTE bIsKeyFrame;
//        BYTE bFrameRate;
    WORD wBytesRead;
    WORD wTotalDataBytes;
    BYTE bSubPayloadState;
    BYTE bIsCompressedPayload;
    BYTE bNextSubPayloadSize;
    WORD wSubpayloadLeft;

}   PAYLOAD_MAP_ENTRY_EX;


#ifdef USE_PACK_HEADERS
#pragma warning( push )
#pragma warning( disable : 4103 )
#include <poppack.h>
#pragma warning( pop )
#endif

#endif  /* WMATYPES_H */
