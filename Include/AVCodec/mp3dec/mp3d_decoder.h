/*
 * libmad - MPEG audio decoder library
 * Copyright (C) 2000-2004 Underbit Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: decoder.h,v 1.17 2004/01/23 09:41:32 rob Exp $
 */

# ifndef LIBMAD_DECODER_H
# define LIBMAD_DECODER_H

# include "mp3d_stream.h"
# include "mp3d_frame.h"
# include "mp3d_synth.h"

#include "audec_apil.h"

enum mad_decoder_mode {
  MAD_DECODER_MODE_SYNC  = 0,
  MAD_DECODER_MODE_ASYNC
};

/*enum mad_flow {
  AUDEC_FLOW_CONTINUE = 0x0000,    // continue normally 
  AUDEC_FLOW_STOP     = 0x0010,    // stop decoding normally 
  AUDEC_FLOW_BREAK    = 0x0011,    // stop decoding and signal an error 
  AUDEC_FLOW_IGNORE   = 0x0020    // ignore the current frame 
};
*/


typedef struct _Async 
{
    long pid;
    int in;
    int out;
} Async;

typedef struct _MP3DWorkSpace_t 
{
    MP3DStream_t stream;
    MP3DFrame_t frame;
    MP3DSynth_t synth;
} MP3DWorkSpace_t; 

typedef struct _MP3DObject_t {

  MP3DWorkSpace_t *sync;


  AUDECFlow_t (*input_func)(void *pdecoder, void *data);
  AUDECFlow_t (*header_func)(void *, mad_header const *);
  AUDECFlow_t (*filter_func)(void *, void *);
  AUDECFlow_t (*output_func)(void *, void *data);
  AUDECFlow_t (*error_func)(void *, void *);
  AUDECFlow_t (*message_func)(void *, void *, unsigned int *);
  unsigned long   ulPlayMP3TotalSamples;
  unsigned long      ulPlayMP3TotalFrames;
  unsigned long   ulPlayMP3ParseFrames;
  unsigned long   ulPlayMP3OutputSize;
} MP3DObject_t;


extern AUDECFlow_t MP3D_DecoderInit(void *ddecoder, void *data,
              AUDECFlow_t (*input_func)(void *pdecoder,
                          void *data),
              AUDECFlow_t (*header_func)(
                                void *decoder,
                                void *header),
              AUDECFlow_t (*filter_func)(void *,
                           void *),
              AUDECFlow_t (*output_func)(void *,
                           void *data),
              AUDECFlow_t (*error_func)(void *,
                          void *),
              AUDECFlow_t (*message_func)(void *,
                            void *));

extern void MP3D_DecodeEnd(void *);
extern AUDECFlow_t MP3D_DecoderSkipOneFrame(void *decoder);
extern AUDECFlow_t MP3D_DecodePoll(void *);

# define mad_decoder_options(decoder, opts)  \
    ((void) ((decoder)->options = (opts)))

//extern int mad_decoder_start(MP3DObject_t *);
extern AUDECFlow_t message_default(void *, void *, unsigned int *);

//@Ryan:

//typedef struct mp3d_memblk_info {
//    void         *ptr;
//    unsigned int size;
//    unsigned int align;
//} mp3d_memblk_info_t;
typedef struct mp3d_memblk_info {
    int    align;
    int size;
    void *ptr;
} mp3d_memblk_info_t;



//typedef mad_decoder *mp3d_object_t;
    
extern unsigned int MP3D_GetMemBlkNum(void *pdecoder);
extern void         MP3D_GetMemBlkInfo(void *pdecoder, MemBlkInfo_t *memblk);
extern void *MP3D_CreateObject(void *pdecoder, MemBlkInfo_t *memblk);
extern void         MP3D_GetMemblkPtr(void *decoder, MemBlkInfo_t *memblk);
extern void         MP3D_SetOutputBufferAddr(void *decoder, short *ptr);
extern short *MP3D_GetOutputBufferAddr(void *decoder);
extern void         MP3D_TotalFramesUpdate(void *pdecoder);
extern void         MP3D_ParseFramesUpdate(void *pdecoder);
extern unsigned long MP3D_GetTotalFrames(void *pdecoder);
extern void         MP3D_SetTotalFrames(void *pdecoder, unsigned long frames);
extern unsigned long MP3D_GetNextFrameAddr(void *pdecoder);
extern void         MP3D_SetNextFrameAddr(void *pdecoder, unsigned long addr);
extern unsigned long MP3D_GetDataLengthPerFrame(void *pdecoder);
extern void         MP3D_SetTotalSamples(void *pdecoder, unsigned long samples);
extern unsigned long MP3D_GetTotalSamples(void *pdecoder);
extern void MP3D_SetCurParseFrames(void *pdecoder, unsigned long frames);
extern unsigned long MP3D_GetCurParseFrames(void *pdecoder);
extern unsigned long MP3D_GetOutputSize(void *pdecoder);
extern void MP3D_SetOutputSize(void *pdecoder, unsigned long size);
extern unsigned long MP3D_GetSampleRate(void *pdecoder);
extern unsigned long MP3D_GetFramesPerSecond(void *pdecoder);
extern unsigned long mp3d_get_bit_rate(void *pdecoder);
extern unsigned long MP3D_GetChannelMode(void *pdecoder);
extern unsigned long MP3D_GetFrameOffsetInFile(void *pdecoder);
extern void MP3D_SetFrameOffsetInFile(void *pdecoder, unsigned long offset);
extern void MP3D_SetCurDecodedFrames(void *pdecoder, unsigned long frames);
extern unsigned long MP3D_GetCurDecodedFrames(void *pdecoder);


# endif
