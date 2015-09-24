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
 * $Id: bit.h,v 1.12 2004/01/23 09:41:32 rob Exp $
 */

#ifndef LIBMAD_BIT_H
#define LIBMAD_BIT_H

#define OPT_BIT_0    0    // inline bit->read
//#NT#2007/07/24#Meg Lin -begin
//optimize mp3 performance
#define OPT_BIT_1   0   // word  /*The original definition: OPT_BIT_1  1, modified by libra*/
//#NT#2007/07/24#Meg Lin -end

#if (OPT_BIT_1)
    #define WORD_BIT    32
    struct mad_bitptr {
        unsigned int *word;
        unsigned int cache;
        unsigned int left;
    };
#else
    struct mad_bitptr {
        unsigned char const *byte;
        unsigned short cache;
        unsigned short left;
    };
#endif


extern void mp3d_bit_init(struct mad_bitptr *, unsigned char const *);

# define mad_bit_finish(bitptr)        /* nothing */

extern unsigned int mad_bit_length(struct mad_bitptr const *,
                struct mad_bitptr const *);

# define mad_bit_bitsleft(bitptr)  ((bitptr)->left)
extern unsigned char const *mad_bit_nextbyte(struct mad_bitptr const *);

extern void mad_bit_skip(struct mad_bitptr *, unsigned int);
//void mad_bit_write(struct mad_bitptr *, unsigned int, unsigned long);
extern unsigned short mad_bit_crc(struct mad_bitptr, unsigned int, unsigned short);

#if (OPT_BIT_0)
    #ifndef CHAR_BIT
    #define CHAR_BIT    8
    #endif
    __inline unsigned long mad_bit_read(struct mad_bitptr *bitptr, unsigned int len)
    {
    #if (OPT_BIT_1)
          register unsigned long value;
          //@Ryan: ??
        if (len < bitptr->left) {
            value = (bitptr->cache & ((1 << bitptr->left) - 1)) >> (bitptr->left - len);
            bitptr->left -= len;
            //printf("Cache %08X, left %02d bits\n", bitptr->cache, bitptr->left);
            return value;
        }
        /* remaining bits in current byte */
        value = bitptr->cache & ((1 << bitptr->left) - 1);
        len  -= bitptr->left;
        bitptr->word++;
        bitptr->left = WORD_BIT;
        bitptr->cache = byte_swap(*bitptr->word);
        value = (value << len) | (bitptr->cache >> (WORD_BIT - len));
        bitptr->left -= len;
        //printf("Cache %08X, left %02d bits\n", bitptr->cache, bitptr->left);
        return value;
    #else
          register unsigned long value;
        //if (bitptr->left == CHAR_BIT)
        //     bitptr->cache = *bitptr->byte;
        if (len < bitptr->left) {
            value = (bitptr->cache & ((1 << bitptr->left) - 1)) >> (bitptr->left - len);
            bitptr->left -= len;
            return value;
        }
        /* remaining bits in current byte */
        value = bitptr->cache & ((1 << bitptr->left) - 1);
        len  -= bitptr->left;
        bitptr->byte++;
        bitptr->left = CHAR_BIT;
        /* more bytes */
        while (len >= CHAR_BIT) {
            value = (value << CHAR_BIT) | *bitptr->byte++;
            len  -= CHAR_BIT;
          }
          //if (len > 0) {
            bitptr->cache = *bitptr->byte;
            value = (value << len) | (bitptr->cache >> (CHAR_BIT - len));
            bitptr->left -= len;
        //}
        return value;
    #endif
    }
#else
    extern unsigned long mad_bit_read(struct mad_bitptr *, unsigned int);    
#endif

#endif
