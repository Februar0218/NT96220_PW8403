#ifndef _SRA_NT96220_H_
#define _SRA_NT96220_H_

#include "DataType.h"

//#define _SRA_REG_BASE_ADDR    0x9A302000

typedef struct
{

  union
  {
    struct
    {
      unsigned GAMMA_READSEL        : 1;
    } Bit;
    UINT32 Word;
  } SRA_Register_0; // 0x0000

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SRA_Register_1; // 0x0004

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SRA_Register_2; // 0x0008

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SRA_Register_3; // 0x000c

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_4; // 0x0010

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_5; // 0x0014

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_6; // 0x0018

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_7; // 0x001c

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_8; // 0x0020

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_9; // 0x0024

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_10; // 0x0028

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_11; // 0x002c

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_12; // 0x0030

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_13; // 0x0034

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_14; // 0x0038

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_15; // 0x003c

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_16; // 0x0040

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_17; // 0x0044

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_18; // 0x0048

  union
  {
    struct
    {
      unsigned RGB_LUT_4N0        : 8;
      unsigned RGB_LUT_4N1        : 8;
      unsigned RGB_LUT_4N2        : 8;
      unsigned RGB_LUT_4N3        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_19; // 0x004c

  union
  {
    struct
    {
      unsigned RGB_LUT_64        : 8;
    } Bit;
    UINT32 Word;
  } SRA_Register_20; // 0x0050

} NT96220_SRA_REG_STRUCT;


#endif
