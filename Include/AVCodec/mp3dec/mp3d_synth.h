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
 * $Id: synth.h,v 1.15 2004/01/23 09:41:33 rob Exp $
 */

# ifndef LIBMAD_SYNTH_H
# define LIBMAD_SYNTH_H

#include "mp3d_fixed.h"
#include "mp3d_frame.h"

struct mad_pcm {
  unsigned int samplerate;        /* sampling frequency (Hz) */
  unsigned short channels;        /* number of channels */
  unsigned short length;        /* number of samples per channel */
  //@Ryan: change pointer
//  mad_fixed_t samples[2][1152];    /* PCM output samples [ch][sample] */
//  mad_fixed_t *samples;            /* PCM output samples [ch][sample] */
  short *samples;        
};

typedef struct _MP3DSynth_t {
  mad_fixed_t filter[2][2][2][16][8];    /* polyphase filterbank outputs */
                      /* [ch][eo][peo][s][v] */

  unsigned int phase;            /* current processing phase */

  struct mad_pcm pcm;            /* PCM output */
} MP3DSynth_t;

/* single channel PCM selector */
enum {
  MAD_PCM_CHANNEL_SINGLE = 0
};

/* dual channel PCM selector */
enum {
  MAD_PCM_CHANNEL_DUAL_1 = 0,
  MAD_PCM_CHANNEL_DUAL_2 = 1
};

/* stereo PCM selector */
enum {
  MAD_PCM_CHANNEL_STEREO_LEFT  = 0,
  MAD_PCM_CHANNEL_STEREO_RIGHT = 1
};

extern void mad_synth_init(MP3DSynth_t *);

# define mad_synth_finish(synth)  /* nothing */

extern void mad_synth_mute(MP3DSynth_t *);

extern void mad_synth_frame(MP3DSynth_t *, MP3DFrame_t const *);

extern mad_fixed_t const D[17][32];

# endif
