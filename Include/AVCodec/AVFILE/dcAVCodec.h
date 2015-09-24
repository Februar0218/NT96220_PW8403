/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       dcAVCodec.h
    @ingroup    mIAVFILE

    @brief      Audio/Video related information header file
                Audio/Video related information header file (Video size, Audio codec...)

    @note       Nothing.

    @version    V1.00.000
    @author     Elvis Chuang
    @date       2005/06/07
*/

/** \addtogroup mIAVFILE */
//@{

#ifndef _AVCODEC_H
#define _AVCODEC_H

#include "Audio.h"

//#NT#2007.04.12#Elvis Chuang -begin
//Support the D1 size resolution
#define D1_WIDTH                    720
#define D1_HEIGHT                   480
//#NT#2007.04.12#Elvis Chuang -end
#define VGA_WIDTH                   640
#define VGA_HEIGHT                  480
#define QVGA_WIDTH                  320
#define QVGA_HEIGHT                 240
#define QQVGA_WIDTH                 160
#define QQVGA_HEIGHT                120

#define CIF_WIDTH                   352
#define CIF_HEIGHT                  288
#define QCIF_WIDTH                  176
#define QCIF_HEIGHT                 144

#define SIF_WIDTH                   352
#define SIF_HEIGHT                  240
#define QSIF_WIDTH                  176
#define QSIF_HEIGHT                 120

#define PACKETSIZE_2K               (2048)
#define PACKETSIZE_4K               (4096)
#define PACKETSIZE_8K               (8192)
#define PACKETSIZE_16K              (16384)
#define PACKETSIZE_32K              (32768)

/**
   @name Video Codec
   ASF file format support video codecs:MP4S, M4S2, DX50
*/
//@{
typedef enum {
   VIDEOCODEC_MP4S,                        ///< MP4S FOURCC
   VIDEOCODEC_M4S2,
   VIDEOCODEC_DX50
} VIDEOCODEC, VIDEO_CODEC;
//@}

// The AUDIOCODEC declaration should be removed
/** @name Audio Codec
   ASF file format support audio codecs:
*/
//@{
typedef enum {
   AUDIOCODEC_PCM8K16BMONO,                ///< mono,   8k,        16bits PCM
   AUDIOCODEC_PCM8K16BSTEREO,              ///< stereo, 8k,        16bits PCM
   AUDIOCODEC_PCM11K16BMONO,               ///< mono,   11.025k,   16bits PCM
   AUDIOCODEC_PCM11K16BSTEREO,             ///< stereo, 11.025k,   16bits PCM
   AUDIOCODEC_PCM12K16BMONO,               ///< mono,   12k,       16bits PCM
   AUDIOCODEC_PCM12K16BSTEREO,             ///< stereo, 12k,       16bits PCM
   AUDIOCODEC_PCM16K16BMONO,               ///< mono,   16k,       16bits PCM
   AUDIOCODEC_PCM16K16BSTEREO,             ///< stereo, 16k,       16bits PCM
   AUDIOCODEC_PCM22K16BMONO,               ///< mono,   22.05k,    16bits PCM
   AUDIOCODEC_PCM22K16BSTEREO,             ///< stereo, 22.05k,    16bits PCM
   AUDIOCODEC_PCM24K16BMONO,               ///< mono,   24k,       16bits PCM
   AUDIOCODEC_PCM24K16BSTEREO,             ///< stereo, 24k,       16bits PCM
   AUDIOCODEC_PCM32K16BMONO,               ///< mono,   32k,       16bits PCM
   AUDIOCODEC_PCM32K16BSTEREO,             ///< stereo, 32k,       16bits PCM
   AUDIOCODEC_PCM44K16BMONO,               ///< mono,   44.1k,     16bits PCM
   AUDIOCODEC_PCM44K16BSTEREO,             ///< stereo, 44.1k,     16bits PCM
   AUDIOCODEC_PCM48K16BMONO,               ///< mono,   48k,       16bits PCM
   AUDIOCODEC_PCM48K16BSTEREO,             ///< stereo, 48k,       16bits PCM
   AUDIOCODEC_IMAADPCM8K16BMONO,           ///< mono,   8k,        16bits IMA-ADPCM
   AUDIOCODEC_IMAADPCM8K16BSTEREO,         ///< stereo, 8k,        16bits IMA-ADPCM
   AUDIOCODEC_IMAADPCM11K16BMONO,          ///< mono,   11.025k,   16bits IMA-ADPCM
   AUDIOCODEC_IMAADPCM11K16BSTEREO,        ///< stereo, 11.025k,   16bits IMA-ADPCM
   AUDIOCODEC_IMAADPCM22K16BMONO,          ///< mono,   22.05k,    16bits IMA-ADPCM
   AUDIOCODEC_IMAADPCM22K16BSTEREO,        ///< stereo, 22.05k,    16bits IMA-ADPCM
   AUDIOCODEC_IMAADPCM44K16BMONO,          ///< mono,   44.1k,     16bits IMA-ADPCM
   AUDIOCODEC_IMAADPCM44K16BSTEREO,        ///< stereo, 44.1k,     16bits IMA-ADPCM
   AUDIOCODEC_G726_32K,
   AUDIOCODEC_G726_24K
} AUDIOCODEC;
//@}

typedef enum
{
    AUDIOCODEC_PCM,                     ///< PCM
    AUDIOCODEC_IMA_ADPCM,               ///< IMA ADPCM
    AUDIOCODEC_G726,                    ///< G.726
    AUDIOCODEC_MP3                      ///< MP3
} AUDIO_CODEC;

/**
   @brief Video Frame Resolution.

*/
//@{
typedef enum
{
   VIDEO_VGA,       /**< Frame resolution 640X480(VGA)      */
   VIDEO_QVGA,      /**< Frame resolution 320X240(QVGA)     */
   VIDEO_CIF,       /**< Frame resolution 352X288(CIF)      */
   VIDEO_QCIF,      /**< Frame resolution 176X144(QCIF)     */
   VIDEO_SIF,       /**< Frame resolution 352X240(SIF)      */
//#NT#2007.04.12#Elvis Chuang -begin
//Support the D1 size resolution
   VIDEO_QQVGA,      /**< Frame resolution 160X120(QQVGA)    */
   VIDEO_D1         /**< Frame resolution 720X480(QQVGA)    */
//#NT#2007.04.12#Elvis Chuang -end
} VIDEO_SIZE;
//@}

#endif //_AVCODEC_H

//@}
