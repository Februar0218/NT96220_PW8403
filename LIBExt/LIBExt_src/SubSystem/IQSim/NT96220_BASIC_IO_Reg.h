#ifndef _BASIC_IO_NT96220_H_
#define _BASIC_IO_NT96220_H_

#include "DataType.h"

//#define _BASIC_IO_REG_BASE_ADDR    0x9A200000

typedef struct
{

  union
  {
    struct
    {
      unsigned IN_WIDTH        : 32;        // bits : 31_0
    } Bit;
    UINT32 Word;
  } BASIC_IO_Register_0; // 0x0000

  union
  {
    struct
    {
      unsigned IN_HEIGHT        : 32;       // bits : 31_0
    } Bit;
    UINT32 Word;
  } BASIC_IO_Register_1; // 0x0004

  union
  {
    struct
    {
      unsigned OUT_WIDTH        : 32;       // bits : 31_0
    } Bit;
    UINT32 Word;
  } BASIC_IO_Register_2; // 0x0008

  union
  {
    struct
    {
      unsigned OUT_HEIGHT        : 32;      // bits : 31_0
    } Bit;
    UINT32 Word;
  } BASIC_IO_Register_3; // 0x000c

  union
  {
    struct
    {
      unsigned IN_PACKBITS        : 2;      // bits : 1_0
      unsigned                    : 2;
      unsigned IN_CFA             : 2;      // bits : 5_4
      unsigned                    : 10;
      unsigned JPEG_QVAL          : 8;      // bits : 23_16
      unsigned IFE_RAW_EN         : 1;          // bits : 24
      unsigned IFE_Y_EN           : 1;          // bits : 25
      unsigned IFE_CX_EN          : 1;          // bits : 26
    } Bit;
    UINT32 Word;
  } BASIC_IO_Register_4; // 0x0010

} NT96220_BASIC_IO_REG_STRUCT;


#endif
