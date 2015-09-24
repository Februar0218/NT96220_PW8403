/**
    Header file of audio decoding codec library

    Exported header file of audio decoding codec library.

    @file       AudioDecode.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05
    
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _AUDIODECODE_H
#define _AUDIODECODE_H


#include "Audio.h"
#include "FileDB.h"
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"
/**
    @addtogroup mIAVCODEC
*/
//@{

#define ZERO_PAD_SAMPLERATE 8000
#define ZERO_PAD_CHUNKSIZE  16000
#define ZERO_PAD_BLOCK      2 //2011/07/15 Meg, since audio Q is 5 for 220
#define ZERO_PAD_SEC        10//zero block pad seconds


/**
    type of getting information from audio decoding codec library.
    format: ER (*GetInfo)(AUDIDEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3); 
*/
typedef enum
{
    MEDIAAUD_GETINFO_NOWOUTRAWADDR   =0x1,   ///< p1:now addr of output audio RAW data(out)
    MEDIAAUD_GETINFO_SECOND2FN       =0x2,   ///< p1:sec(in), p2:audioByteOffset to sec(in), p3:related audio frame number(out)
    MEDIAAUD_GETINFO_RAWSTARTADDR    =0x3,   ///< p1:starting address of raw data(out)
    ENUM_DUMMY4WORD(AUDCODEC_GETINFO_TYPE)
} AUDCODEC_GETINFO_TYPE;

/**
    type of setting information from audio decoding codec library.
    format: ER (*SetInfo)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAAUD_SETINFO_USEDADDR   = 0x1,///< p1:address to use(in), p2:size(in)
    MEDIAAUD_SETINFO_RESETRAW   = 0x2,///< reset raw to starting
    MEDIAAUD_SETINFO_FAKEDECODE = 0x3,///< start fake decoding
    MEDIAAUD_SETINFO_REALDECODE = 0x4,///< start real decoding
    ENUM_DUMMY4WORD(AUDCODEC_SETINFO_TYPE)
} AUDCODEC_SETINFO_TYPE;

/**
    struture of audio playing information.
*/
typedef struct 
{
    UINT32 sampleRate;///< in: audio sample rate
    UINT32 channels;  ///< in: audio channels
    UINT32 filesize;  ///< in: filesize
    UINT32 chunksize; ///< out: chunksize, in bytes

} AUDIO_PLAYINFO;

/**
    struture of audio outputing raw.
*/
typedef struct
{
    UINT32 outRawAddr; ///< out audio raw address
    UINT32 outRawSize; ///< out audio raw data (in bytes)

} AUDIO_OUTRAW;



/**
    Struture of audio decoder
*/
typedef struct
{
    ER (*Initialize)(UINT32 fileAddr, AUDIO_PLAYINFO *pobj);///< initialize decoder
    ER (*GetInfo)(AUDCODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);///< get information from decoder
    ER (*SetInfo)(AUDCODEC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3); ///< set information to decoder
    ER (*DecodeOne)(UINT32 type, UINT32 fileAddr, UINT32 p2, UINT32 *p3);///< decode one frame
    ER (*WaitDecodeDone)(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);///< wait decoding finished, p1: E_OK or not (out), p2:(AUDIO_OUTRAW *) object(out)
    ER (*CustomizeFunc)(UINT32 type, void *pobj);///< customize function
    UINT32 checkID;///< check id of decoder
} AUDIO_DECODER, *PAUDIO_DECODER;

/*
    Public funtions to get audio decoding object
*/
//@{
extern PAUDIO_DECODER MP_PCMDec_getAudioDecodeObject(void);
extern PAUDIO_DECODER MP_AACDec_getAudioDecodeObject(void);
extern PAUDIO_DECODER MP_ZeroDec_getAudioDecodeObject(void);
//@}

//@}

#endif //_AUDIODECODE_H
