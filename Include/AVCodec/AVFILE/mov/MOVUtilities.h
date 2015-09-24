/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       MOVUtilities.h
    @ingroup    mIAVMOV

    @brief      MOV file utility functions

    @note       Nothing.

*/
#ifndef _MOVUTILITIES_H
#define _MOVUTILITIES_H

#include "type.h"
#include "AVILib.h"
#include "JpgDec.h"


#define UINT32_MAX   0xFFFFFFFF

#define TRACK_INDEX_VIDEO   0x01
#define TRACK_INDEX_AUDIO   0x02
#define TRACK_INDEX_OTHER   0x04

#define FF_ASPECT_EXTENDED 15
#define FF_RC_STRATEGY_XVID 1
#define FF_BUG_AUTODETECT       1  ///< autodetection
#define FF_BUG_OLD_MSMPEG4      2
#define FF_BUG_XVID_ILACE       4
#define FF_BUG_UMP4             8
#define FF_BUG_NO_PADDING       16
#define FF_BUG_AMV              32
#define FF_BUG_AC_VLC           0  ///< Will be removed, libavcodec can now handle these non-compliant files by default.
#define FF_BUG_QPEL_CHROMA      64
#define FF_BUG_STD_QPEL         128
#define FF_BUG_QPEL_CHROMA2     256
#define FF_BUG_DIRECT_BLOCKSIZE 512
#define FF_BUG_EDGE             1024
#define FF_BUG_HPEL_CHROMA      2048
#define FF_BUG_DC_CLIP          4096
#define FF_BUG_MS               8192 ///< Work around various bugs in Microsoft's broken decoders.
#define FF_COMPLIANCE_VERY_STRICT   2 ///< Strictly conform to a older more strict version of the spec or reference software.
#define FF_COMPLIANCE_STRICT        1 ///< Strictly conform to all the things in the spec no matter what consequences.
#define FF_COMPLIANCE_NORMAL        0
#define FF_COMPLIANCE_INOFFICIAL   -1 ///< Allow inofficial extensions.
#define FF_COMPLIANCE_EXPERIMENTAL -2 ///< Allow nonstandardized experimental things.
#define FF_ER_CAREFUL         1
#define FF_ER_COMPLIANT       2
#define FF_ER_AGGRESSIVE      3
#define FF_ER_VERY_AGGRESSIVE 4
#define FF_DCT_AUTO    0
#define FF_DCT_FASTINT 1
#define FF_DCT_INT     2
#define FF_DCT_MMX     3
#define FF_DCT_MLIB    4
#define FF_DCT_ALTIVEC 5
#define FF_DCT_FAAN    6
#define FF_IDCT_AUTO         0
#define FF_IDCT_INT          1
#define FF_IDCT_SIMPLE       2
#define FF_IDCT_SIMPLEMMX    3
#define FF_IDCT_LIBMPEG2MMX  4
#define FF_IDCT_PS2          5
#define FF_IDCT_MLIB         6
#define FF_IDCT_ARM          7
#define FF_IDCT_ALTIVEC      8
#define FF_IDCT_SH4          9
#define FF_IDCT_SIMPLEARM    10
#define FF_IDCT_H264         11
#define FF_IDCT_VP3          12
#define FF_IDCT_IPP          13
#define FF_IDCT_XVIDMMX      14
#define FF_IDCT_CAVS         15
#define FF_IDCT_SIMPLEARMV5TE 16
#define FF_IDCT_SIMPLEARMV6  17

#define FF_EC_GUESS_MVS   1
#define FF_EC_DEBLOCK     2
#define FF_MM_FORCE    0x80000000 /* Force usage of selected flags (OR) */
    /* lower 16 bits - CPU features */
#ifdef HAVE_MMX
#define FF_MM_MMX      0x0001 /* standard MMX */
#define FF_MM_3DNOW    0x0004 /* AMD 3DNOW */
#define FF_MM_MMXEXT   0x0002 /* SSE integer functions or AMD MMX ext */
#define FF_MM_SSE      0x0008 /* SSE functions */
#define FF_MM_SSE2     0x0010 /* PIV SSE2 functions */
#define FF_MM_3DNOWEXT 0x0020 /* AMD 3DNowExt */
#endif /* HAVE_MMX */
#ifdef HAVE_IWMMXT
#define FF_MM_IWMMXT   0x0100 /* XScale IWMMXT */
#endif /* HAVE_IWMMXT */

#define FF_PRED_LEFT   0
#define FF_PRED_PLANE  1
#define FF_PRED_MEDIAN 2

#define FF_DEBUG_PICT_INFO 1
#define FF_DEBUG_RC        2
#define FF_DEBUG_BITSTREAM 4
#define FF_DEBUG_MB_TYPE   8
#define FF_DEBUG_QP        16
#define FF_DEBUG_MV        32
#define FF_DEBUG_DCT_COEFF 0x00000040
#define FF_DEBUG_SKIP      0x00000080
#define FF_DEBUG_STARTCODE 0x00000100
#define FF_DEBUG_PTS       0x00000200
#define FF_DEBUG_ER        0x00000400
#define FF_DEBUG_MMCO      0x00000800
#define FF_DEBUG_BUGS      0x00001000
#define FF_DEBUG_VIS_QP    0x00002000
#define FF_DEBUG_VIS_MB_TYPE 0x00004000

#define FF_DEBUG_VIS_MV_P_FOR  0x00000001 //visualize forward predicted MVs of P frames
#define FF_DEBUG_VIS_MV_B_FOR  0x00000002 //visualize forward predicted MVs of B frames
#define FF_DEBUG_VIS_MV_B_BACK 0x00000004 //visualize backward predicted MVs of B frames

#define FF_CMP_SAD  0
#define FF_CMP_SSE  1
#define FF_CMP_SATD 2
#define FF_CMP_DCT  3
#define FF_CMP_PSNR 4
#define FF_CMP_BIT  5
#define FF_CMP_RD   6
#define FF_CMP_ZERO 7
#define FF_CMP_VSAD 8
#define FF_CMP_VSSE 9
#define FF_CMP_NSSE 10
#define FF_CMP_W53  11
#define FF_CMP_W97  12
#define FF_CMP_DCTMAX 13
#define FF_CMP_DCT264 14
#define FF_CMP_CHROMA 256

#define FF_DTG_AFD_SAME         8
#define FF_DTG_AFD_4_3          9
#define FF_DTG_AFD_16_9         10
#define FF_DTG_AFD_14_9         11
#define FF_DTG_AFD_4_3_SP_14_9  13
#define FF_DTG_AFD_16_9_SP_14_9 14
#define FF_DTG_AFD_SP_4_3       15

#define FF_DEFAULT_QUANT_BIAS 999999

#define FF_LAMBDA_SHIFT 7
#define FF_LAMBDA_SCALE (1<<FF_LAMBDA_SHIFT)
#define FF_QP2LAMBDA 118 ///< factor to convert from H.263 QP to lambda
#define FF_LAMBDA_MAX (256*128-1)

#define FF_QUALITY_SCALE FF_LAMBDA_SCALE //FIXME maybe remove

#define FF_CODER_TYPE_VLC       0
#define FF_CODER_TYPE_AC        1
#define FF_CODER_TYPE_RAW       2
#define FF_CODER_TYPE_RLE       3
#define FF_CODER_TYPE_DEFLATE   4

#define SLICE_FLAG_CODED_ORDER    0x0001 ///< draw_horiz_band() is called in coded order instead of display
#define SLICE_FLAG_ALLOW_FIELD    0x0002 ///< allow draw_horiz_band() with field slices (MPEG2 field pics)
#define SLICE_FLAG_ALLOW_PLANE    0x0004 ///< allow draw_horiz_band() with 1 component at a time (SVQ1)

#define FF_MB_DECISION_SIMPLE 0        ///< uses mb_cmp
#define FF_MB_DECISION_BITS   1        ///< chooses the one which needs the fewest bits
#define FF_MB_DECISION_RD     2        ///< rate distoration

#define FF_AA_AUTO    0
#define FF_AA_FASTINT 1 //not implemented yet
#define FF_AA_INT     2
#define FF_AA_FLOAT   3

#define FF_PROFILE_UNKNOWN -99

#define FF_LEVEL_UNKNOWN -99

#define X264_PART_I4X4 0x001  /* Analyse i4x4 */
#define X264_PART_I8X8 0x002  /* Analyse i8x8 (requires 8x8 transform) */
#define X264_PART_P8X8 0x010  /* Analyse p16x8, p8x16 and p8x8 */
#define X264_PART_P4X4 0x020  /* Analyse p8x4, p4x8, p4x4 */
#define X264_PART_B8X8 0x100  /* Analyse b16x8, b8x16 and b8x8 */
#define FF_COMPRESSION_DEFAULT -1


typedef UINT32  MOV_Offset;

#define CODEC_TYPE_VIDEO 0
#define CODEC_TYPE_AUDIO 1

#define CODEC_ID_ASV1  0x20
#define MOV_MDAT_HEADER_SIZE    8//not wide

typedef struct MOV_Ientry {
    UINT32      pos;
    UINT32      size;
    UINT32      duration;
    UINT16      key_frame;
    UINT16      rev;
} MOV_Ientry;//16 byte

typedef struct _MOV_Atom
{
    UINT32      type;
    MOV_Offset  offset;
    MOV_Offset  size;
    UINT8       index;
    UINT8       parentIndex;
    UINT8       tag;
    UINT8       isAudio;

} MOV_Atom;


typedef struct _MOV_Tag
{
    UINT32 tag;
    UINT8 leaf;
    UINT8 rev;
    UINT16 rev2;
    UINT16  (*pFunc)(char *pb, MOV_Atom *pAtom);

} MOV_Tag;


typedef struct _MOV_Context {
    UINT16 width;
    UINT16 height;//coded_width/height

    //wave atom
    UINT32 nSamplePerSecond; // samples per second, sampleRate
    UINT32 nAvgBytesPerSecond;
    UINT16 nBlockAlign;//bytes per packet
    UINT16 nchannel;//audio only
    UINT16 wBitsPerSample;//
    UINT16 cbSize;//extra data size
    UINT16 nSamplePerBlock;// frame size
    UINT16 nNumCoeff;//coeff number

    char *codec_name;
    UINT8 codec_type; // see CODEC_TYPE_xxx
    UINT8 codec_id; // see CODEC_ID_xxx

    char *rc_eq;//rate control equation
    int rc_max_rate;//maximum bitrate
    int rc_min_rate;//minimum bitrate
    int rc_buffer_size;//decoder bitstream buffer size
} MOV_Context;

typedef struct _MOV_Track {
    UINT32      mode;
    UINT32      entry;
    UINT32      timescale;
    UINT32      ctime;//create time
    UINT32      mtime;//modify time
    INT64       trackDuration;
    UINT32      sampleCount;
    UINT32      sampleSize;
    UINT32      hasKeyframes;
    UINT32      hasBframes;
    UINT16      language;
    UINT16      quality;
    UINT32      trackID;
    UINT32      tag;
    UINT32      audioSize;
    UINT32      frameNumber;
    MOV_Context *context;

    UINT32      vosLen;
    UINT8       *vosData;
    MOV_Ientry  *cluster;
    UINT32      audio_vbr;
} MOV_Track;

extern void MOV_MakeAtomInit(void *pMovBaseAddr);
extern UINT32 MOVWriteSTCOTag(char *pb, MOV_Track *ptrack);
extern UINT32 MOVWriteSTSDTag(char *pb, MOV_Track *ptrack);
extern UINT32 MOVWriteSTSSTag(char *pb, MOV_Track *ptrack);
extern UINT32 MOVWriteMOOVTag(char *pb, MOV_Track *ptrack, UINT8 trackIndex);

extern UINT32 MOV_Read_SearchAudioSize(UINT32 offset);
extern void MOVRead_SetMoovErrorReason(UINT32 reason);
extern UINT8 MOV_RearCheckImageWH(void);
extern void MOV_Read_SearchMax_I_FrameSize(void);

#endif
