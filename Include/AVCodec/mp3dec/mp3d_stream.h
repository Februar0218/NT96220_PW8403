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
 * $Id: stream.h,v 1.20 2004/02/05 09:02:39 rob Exp $
 */

# ifndef LIBMAD_STREAM_H
# define LIBMAD_STREAM_H

# include "mp3d_bit.h"

# define MAD_BUFFER_GUARD    8
# define MAD_BUFFER_MDLEN    (511 + 2048 + MAD_BUFFER_GUARD)
//#NT#2007/07/24#Meg Lin -begin
//optimize mp3 performance

enum mad_error {
  MAD_ERROR_NONE       = 0x0000,    /* no error */

  MAD_ERROR_BUFLEN       = 0x0001,    /* input buffer too small (or EOF) */
  MAD_ERROR_BUFPTR       = 0x0002,    /* invalid (null) buffer pointer */

  MAD_ERROR_NOMEM       = 0x0031,    /* not enough memory */

  MAD_ERROR_LOSTSYNC       = 0x0101,    /* lost synchronization */
  MAD_ERROR_BADLAYER       = 0x0102,    /* reserved header layer value */
  MAD_ERROR_BADBITRATE       = 0x0103,    /* forbidden bitrate value */
  MAD_ERROR_BADSAMPLERATE  = 0x0104,    /* reserved sample frequency value */
  MAD_ERROR_BADEMPHASIS       = 0x0105,    /* reserved emphasis value */

  MAD_ERROR_BADCRC       = 0x0201,    /* CRC check failed */
  MAD_ERROR_BADBITALLOC       = 0x0211,    /* forbidden bit allocation value */
  MAD_ERROR_BADSCALEFACTOR = 0x0221,    /* bad scalefactor index */
  MAD_ERROR_BADMODE        = 0x0222,    /* bad bitrate/mode combination */
  MAD_ERROR_BADFRAMELEN       = 0x0231,    /* bad frame length */
  MAD_ERROR_BADBIGVALUES   = 0x0232,    /* bad big_values count */
  MAD_ERROR_BADBLOCKTYPE   = 0x0233,    /* reserved block_type */
  MAD_ERROR_BADSCFSI       = 0x0234,    /* bad scalefactor selection info */
  MAD_ERROR_BADDATAPTR       = 0x0235,    /* bad main_data_begin pointer */
  MAD_ERROR_BADPART3LEN       = 0x0236,    /* bad audio data length */
  MAD_ERROR_BADHUFFTABLE   = 0x0237,    /* bad Huffman table select */
  MAD_ERROR_BADHUFFDATA       = 0x0238,    /* Huffman data overrun */
  MAD_ERROR_BADSTEREO      = 0x0239,    /* incompatible block_type for JS */
  MAD_ERROR_BADPADDING     = 0x0240  /*padding set where it shouldn't be set*/ //added by libra
};

# define MAD_RECOVERABLE(error)    ((error) & 0xff00)

typedef struct _MP3DStream_t {
    unsigned char const *buffer;        /* input bitstream buffer */
    unsigned char const *bufend;        /* end of buffer */
    unsigned long skiplen;        /* bytes to skip before next frame */

    int sync;                /* stream sync found */
    unsigned long freerate;        /* free bitrate (fixed) */

    unsigned char const *this_frame;    /* start of current frame */
    unsigned char const *next_frame;    /* start of next frame */
    struct mad_bitptr ptr;        /* current processing bit pointer */

    struct mad_bitptr anc_ptr;        /* ancillary bits pointer */
    unsigned int anc_bitlen;        /* number of ancillary bits */

    unsigned char (*main_data)[MAD_BUFFER_MDLEN];
                    /* Layer III main_data() */
    unsigned int md_len;            /* bytes in main_data */

    int options;                /* decoding options (see below) */
    enum mad_error error_result;            /* error code (see above) */
    unsigned int    frame_oft_infile;        // frame offset addr in file
    unsigned int    slots_per_frame;

  unsigned int length;      /* bitstream length of each frame, added by libra */
  unsigned int synclen;     /* sync length, added by libra */
  unsigned int filesize;    /* mp3 file size, added by Meg 2007/09/27 */
  unsigned int lost_sync_times; /* mp3 lost sync times, added by Meg 2007/09/27 */
} MP3DStream_t;
//#NT#2007/07/24#Meg Lin -end

enum {
  MAD_OPTION_IGNORECRC      = 0x0001,    /* ignore CRC errors */
  MAD_OPTION_HALFSAMPLERATE = 0x0002    /* generate PCM at 1/2 sample rate */
# if 0  /* not yet implemented */
  MAD_OPTION_LEFTCHANNEL    = 0x0010,    /* decode left channel only */
  MAD_OPTION_RIGHTCHANNEL   = 0x0020,    /* decode right channel only */
  MAD_OPTION_SINGLECHANNEL  = 0x0030    /* combine channels */
# endif
};

extern void mad_stream_init(MP3DStream_t *);
extern void mad_stream_finish(MP3DStream_t *);

//# define mad_stream_options(stream, opts)
//    ((void) ((stream)->options = (opts)))

extern void mad_stream_buffer(void *,
               unsigned char const *, unsigned long, unsigned int);
extern void mad_stream_skip(MP3DStream_t *, unsigned long);

extern int mad_stream_sync(MP3DStream_t *);

extern char const *mad_stream_errorstr(MP3DStream_t const *);

# endif
