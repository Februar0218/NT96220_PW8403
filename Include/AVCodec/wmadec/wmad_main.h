#ifndef _WMAD_MAIN_H
#define _WMAD_MAIN_H

#include "wmatypes.h"
#include "audec_apil.h"
#include "wmadecS_api.h"
#include "msaudiodec.h"

typedef struct
{
    BYTE    *pStart;
    DWORD    dwLength;
    DWORD    dwOffset;
} tFileBuffer;

typedef struct
{
    tFileBuffer                 fbInput;
    tWMAFileHdrStateInternal    hdr_parse;
    tWMAParseState              parse_state;
    DWORD                       cbPayloadOffset;
    DWORD                       cbPayloadLeft;
    wmaBOOL                     bBlockStart;
    DWORD                       cbBlockLeft;
    PACKET_PARSE_INFO_EX        ppex;
    PAYLOAD_MAP_ENTRY_EX        payload;
    DWORD                       iPayload;
    WORD                        wPayStart;
    wmaBOOL                     bHasDRM;
    DWORD                       nSampleCount;
    WMARawDecHandle             hWMA;
    BYTE                        bDecInWaitState;
    BYTE                        bAllFramesDone;
    BYTE                        bFirst;
    BYTE                        *pCallBackBuffer;
    I16                         *pbDst;
    U32                         iPlayWMATotalSamples;
    U32                         iSamplesInLastFrame;
} WMADObject_t;

DWORD WMAD_ParseAsfHeader(WMADObject_t *audec);
void WMAD_GetMemBlkInfo(void *info, MemBlkInfo_t *memblk);
unsigned WMAD_GetMemBlkNum(void *info);
void *WMAD_CreateObject(void *info, MemBlkInfo_t *memblk);
AUDECFlow_t WMAD_DecoderInit(
              void *object,
              void *data,
              AUDECFlow_t (*input_func)(void *, void *),
              AUDECFlow_t (*header_func)(void *, void *),
              AUDECFlow_t (*filter_func)(void *, void *),
              AUDECFlow_t (*output_func)(void *, void *),
              AUDECFlow_t (*error_func)(void *, void *),
              AUDECFlow_t (*message_func)(void *, void *));
AUDECFlow_t WMAD_DecodePoll(void *object);
AUDECFlow_t WMAD_DecodeSkipOnePoll(void *object);

void WMAD_SetStreamBuffer(void *object, unsigned char const *buffer, unsigned long length);
unsigned long WMAD_GetTotalSamples(void *object);
void WMAD_SetTotalSamples(void *object, unsigned long samples);

unsigned long WMAD_GetDataLengthPerFrame(void *object);
unsigned long WMAD_GetNextFrameAddr(void *object);
void WMAD_SetNextFrameAddr(void *object, unsigned long addr);
unsigned long WMAD_GetTotalFrames(void *object);
void WMAD_SetTotalFrames(void *object, unsigned long frames);

short *WMAD_GetOutputBufferAddr(void *decoder);
void WMAD_SetOutputBufferAddr(void *decoder, short *ptr);
unsigned long WMAD_GetSampleRate(void *object);
unsigned long WMAD_GetChannelMode(void *object);
void WMAD_DecodeEnd(void *object);
void WMAD_SetCurParseFrames(void *object, unsigned long frames);
unsigned long WMAD_GetCurParseFrames(void *object);
void WMAD_SetCurDecodedFrames(void *object, unsigned long frames);
unsigned long WMAD_GetCurDecodedFrames(void *object);
unsigned long WMAD_GetOutputSize(void *object);
unsigned long WMAD_GetFramesPerSecond(void *object);
unsigned long WMAD_GetFrameOffsetInFile(void *object);
void WMAD_SetFrameOffsetInFile(void *object, unsigned long offset);

#endif
