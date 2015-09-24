/**
    @file       WAVLib.h
    @ingroup    mIAVWAV

    @brief      Header file for WAV file reader/writer.

                This file is the header file that define the API and data type
                for WAV file reader/writer.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

/**
    @addtogroup mIAVWAV
*/
//@{

#ifndef _WAVLIB_H
#define _WAVLIB_H

#include "dcAVCodec.h"
#include "Audio.h"
#include "Adpcm.h"

//#NT#2008/04/28#Chris Hsu -begin
//#NT#1. Support 8 bits per sample PCM format
//#NT#2. Support user data in header
//#NT#3. Optimize code to read / write data
#define WAV_DIR_FILE_AUTO               0       ///< Auto create file name based on DCF specification

/**
    @name   WAV Library return status

    Definittion for WAV Library return status
*/
//@{
#define WAV_STATUS_OK                   0       ///< Operation OK
#define WAV_STATUS_INVALID_FILE         1       ///< Invalid file (Not a RIFF type WAV format)
#define WAV_STATUS_INVALID_PARAM        2       ///< Invalid parameter
#define WAV_STATUS_WRITE_ERR            3       ///< Write file error
#define WAV_STATUS_READ_ERR             4       ///< Read file error
//@}

/**
    @name   WAV file bits per sample

    Definittion for WAV file bits per sample
*/
//@{
#define WAV_BITS_PER_SAM_PCM_8          0x0008  ///< 8 bits PCM
#define WAV_BITS_PER_SAM_PCM_16         0x0010  ///< 16 bits PCM
#define WAV_BITS_PER_SAM_ADPCM          0x0004  ///< 4 bits ADPCM
//@}

/**
    PCM Wave Header

    @note Size is 44 bytes
*/
typedef struct
{
    UINT32      uiHeaderID;         ///< Wave Header
                                    ///< @note Should be "RIFF" (0x46464952)
    UINT32      uiHeaderSize;       ///< Header Size
                                    ///< @note = FileSize - 8 or DataChunkSize + 36 + UserDataSize
    UINT32      uiRIFFType;         ///< RIFF Type
                                    ///< @note Should be "WAVE" (0x45564157)
    UINT32      uiFormatID;         ///< Format Chunk ID
                                    ///< @note Should be "fmt " (0x20746D66)
    UINT32      uiFormatSize;       ///< Format Chunk Size
                                    ///< @note Should be 0x00000010
    UINT16      uiCompressionCode;  ///< Compression Code
                                    ///< @note Should be 0x0001 for PCM format WAV file
    UINT16      uiNumOfChannels;    ///< Number Of Channels
                                    ///< - @b 0x0001: Mono
                                    ///< - @b 0x0002: Stereo
    UINT32      uiSamplingRate;     ///< Sampling rate
    UINT32      uiAvgBytePerSec;    ///< Average Byte Per Second
                                    ///< @note = Sampling Rate * Block Align
    UINT16      uiBlockAlign;       ///< Block Align
                                    ///< @note = Significant Bits Per Sample * Number Of Channles / 8
    UINT16      uiSigBitsPerSample; ///< Significant Bits Per Sample
                                    ///< @note Should be 0x0010 or 0x0008, PCM is 8/16 bits per sample
    UINT32      uiDataID;           ///< Data Chunk ID
                                    ///< @note Should be "data" (0x61746164)
    UINT32      uiDataSize;         ///< Data Chunk Size
                                    ///<
                                    ///< Data byte count
} WAV_PCMHEADER, *PWAV_PCMHEADER;

/**
    ADPCM Wave Header

    @note Size is 60 bytes
*/
typedef struct
{
    UINT32      uiHeaderID;         ///< Wave Header
                                    ///< @note Should be "RIFF" (0x46464952)
    UINT32      uiHeaderSize;       ///< Header Size
                                    ///< @note = FileSize - 8 or DataChunkSize + 52 + UserDataSize
    UINT32      uiRIFFType;         ///< RIFF Type
                                    ///< @note Should be "WAVE" (0x45564157)
    UINT32      uiFormatID;         ///< Format Chunk ID
                                    ///< @note Should be "fmt " (0x20746D66)
    UINT32      uiFormatSize;       ///< Format Chunk Size
                                    ///< @note Should be 0x00000014
    UINT16      uiCompressionCode;  ///< Compression Code
                                    ///< @note Should be 0x0011 for IMA ADPCM format WAV file
    UINT16      uiNumOfChannels;    ///< Number Of Channels
                                    ///< - @b 0x0001: Mono
                                    ///< - @b 0x0002: Stereo
    UINT32      uiSamplingRate;     ///< Sampling rate
    UINT32      uiAvgBytePerSec;    ///< Average Byte Per Second
                                    ///< @note = Sampling Rate * Block Align / Samples per block
    UINT16      uiBlockAlign;       ///< Block Align
                                    ///< @note = 256 * uiNumOfChannels * max(1, uiSamplingRate/11.025KHz)
    UINT16      uiSigBitsPerSample; ///< Significant Bits Per Sample
                                    ///< @note Should be 0x0004, IMA ADPCM is 4bits per sample
    UINT16      uiExtraFormatBytes; ///< Extra Format bytes
                                    ///< @note Should be 0x0002
    UINT16      uiExtraFormat;      ///< Extra Format Data
                                    ///< Indicates Samples per block
    UINT32      uiFactID;           ///< Fact Chunk ID
                                    ///< @note Should be "fact" (0x74636166)
    UINT32      uiFactSize;         ///< Fact Chunk Size
                                    ///< @note Should be 0x00000004
    UINT32      uiSampleCount;      ///< Samples count of this file
    UINT32      uiDataID;           ///< Data Chunk ID
                                    ///< @note Should be "data" (0x61746164)
    UINT32      uiDataSize;         ///< Data Chunk Size
                                    ///<
                                    ///< Data byte count
} WAV_ADPCMHEADER, *PWAV_ADPCMHEADER;

#define WAVFILE_ASF_ENABLE      DISABLE

#if (WAVFILE_ASF_ENABLE == ENABLE)
/**
    WAV File Type
*/
typedef enum
{
    WAVFILE_RIFF,     ///< RIFF format
    WAVFILE_ASF       ///< ASF  format
} WAVFILETYPE;
#endif

/**
    WAV File Information
*/
typedef struct
{
    UINT32          uiDirID;            ///< Directory ID (DCF specification)
    UINT32          uiFileID;           ///< File ID (DCF specification)
    UINT32          uiDataSize;         ///< Data chunk size
    UINT32          uiSampleCount;      ///< Total sample count
    UINT32          uiBitsPerSample;    ///< Bits per sample
    AUDIO_CODEC     AudioCodec;         ///< What kind of codec
    AUDIO_SR        AudioSamplingRate;  ///< Sampling rate
    AUDIO_CH        AudioChannel;       ///< Audio channel
#if (WAVFILE_ASF_ENABLE == ENABLE)
    WAVFILETYPE     FileFormat;         ///< File format
#endif
    //#NT#2009/05/27#Lily Kao -begin
    UINT32          uiChannelCnt; //lilyt0620
    //#NT#2009/05/27#Lily Kao -end
} WAV_FILEINFO, *PWAV_FILEINFO;

extern UINT32       WAV_Convert16to8(UINT32 uiSrcAddr, UINT32 uiDstAddr, UINT32 uiSize);
extern UINT32       WAV_Convert8to16(UINT32 uiSrcAddr, UINT32 uiDstAddr, UINT32 uiSize);

extern UINT32       WAV_SetHeaderBuffer(UINT32 uiBufferAddr, UINT32 uiBufferSize);

extern UINT32       WAVWriter_SetUserData(UINT32 uiDataAddr, UINT32 uiDataSize);

extern UINT32       WAVWriter_WriteHeader(PWAV_FILEINFO pWAVFileInfo, UINT32 uiAddress);
extern UINT32       WAVWriter_WriteData(UINT32 uiBufAddr, UINT32 uiDataSize);
extern UINT32       WAVWriter_UpdateHeader(PWAV_FILEINFO pWAVFileInfo, UINT32 uiAddress);

extern UINT32       WAVReader_ReadHeader(PWAV_FILEINFO pWAVFileInfo, UINT32 uiAddress);
extern UINT32       WAVReader_ReadData(UINT32 uiBufAddr, UINT32 uiDataSize);
extern UINT32       WAVReader_CloseFile(void);
//#NT#2008/04/28#Chris Hsu -end

#endif

//@}
