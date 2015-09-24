#ifndef _IFE_NT96220_H_
#define _IFE_NT96220_H_

#include "DataType.h"

//#define _IFE_REG_BASE_ADDR    0x9A402000

typedef struct
{

  union
  {
    struct
    {
      unsigned FILT_START        : 1;
      unsigned SW_CLR            : 1;
      unsigned                   : 2;
      unsigned IFE_SCALE         : 4;
      unsigned IFE_TIMES         : 4;
      unsigned SAVE_IFE_OUT      : 1;
    } Bit;
    UINT32 Word;
  } IFE_Register_0; // 0x0000

  union
  {
    struct
    {
      unsigned IFE_MODE          : 1;
      unsigned SRC_FMT           : 1;
      unsigned IFE_CFASEL        : 2;
      unsigned ROWDEF_EN         : 1;
      unsigned OUTL_EN           : 1;
      unsigned FILTER_EN         : 1;
      unsigned CRVMAP_EN         : 1;
    } Bit;
    UINT32 Word;
  } IFE_Register_1; // 0x0004

  union
  {
    struct
    {
      unsigned S_WEIGHT0         : 3;
      unsigned S_WEIGHT1         : 3;
      unsigned S_WEIGHT2         : 3;
      unsigned S_WEIGHT3         : 3;
      unsigned S_WEIGHT4         : 3;
      unsigned S_WEIGHT5         : 3;
      unsigned                   : 2;
      unsigned BINNING           : 3;
      unsigned                   : 5;
      unsigned S_ONLY_EN         : 1;
      unsigned S_ONLY_LEN        : 1;
    } Bit;
    UINT32 Word;
  } IFE_Register_2; // 0x0008

  union
  {
    struct
    {
      unsigned RTH_G0        : 8;
      unsigned RTH_G1        : 8;
    } Bit;
    UINT32 Word;
  } IFE_Register_3; // 0x000c

  union
  {
    struct
    {
      unsigned RTH_RB0        : 8;
      unsigned RTH_RB1        : 8;
    } Bit;
    UINT32 Word;
  } IFE_Register_4; // 0x0010

  union
  {
    struct
    {
      unsigned                     : 2;
      unsigned IMAGE_SADDR0        : 23;
    } Bit;
    UINT32 Word;
  } IFE_Register_5; // 0x0014

  union
  {
    struct
    {
      unsigned                     : 2;
      unsigned IMAGE_SLOFF0        : 14;
    } Bit;
    UINT32 Word;
  } IFE_Register_6; // 0x0018

  union
  {
    struct
    {
      unsigned                 : 1;
      unsigned LINE_CNT        : 15;
      unsigned                 : 2;
      unsigned WIN_X           : 12;
    } Bit;
    UINT32 Word;
  } IFE_Register_7; // 0x001c

  union
  {
    struct
    {
      unsigned                    : 2;
      unsigned IMAGE_DADDR        : 23;
    } Bit;
    UINT32 Word;
  } IFE_Register_8; // 0x0020

  union
  {
    struct
    {
      unsigned                    : 2;
      unsigned IMAGE_DLOFF        : 14;
    } Bit;
    UINT32 Word;
  } IFE_Register_9; // 0x0024

  union
  {
    struct
    {
      unsigned CLAMP_TH           : 8;
      unsigned CLAMPMUL_G         : 7;
      unsigned                    : 1;
      unsigned CLAMPMUL_RB        : 7;
    } Bit;
    UINT32 Word;
  } IFE_Register_10; // 0x0028

  union
  {
    struct
    {
      unsigned                       : 1;
      unsigned CRVMAP_DELTA          : 2;
      unsigned                       : 5;
      unsigned CRVMAP_INOFFS         : 5;
      unsigned                       : 3;
      unsigned CRVMAP_OUTOFFS        : 5;
    } Bit;
    UINT32 Word;
  } IFE_Register_11; // 0x002c

  union
  {
    struct
    {
      unsigned INTE_FRMEND             : 1;
      unsigned INTE_BP1                : 1;
      unsigned INTE_ROWDEF_FAIL        : 1;
      unsigned INTE_STRPEND            : 1;
    } Bit;
    UINT32 Word;
  } IFE_Register_12; // 0x0030

  union
  {
    struct
    {
      unsigned INT_FRMEND             : 1;
      unsigned INT_BP1                : 1;
      unsigned INT_ROWDEF_FAIL        : 1;
      unsigned INT_STRPEND            : 1;
    } Bit;
    UINT32 Word;
  } IFE_Register_13; // 0x0034

  union
  {
    struct
    {
      unsigned IFE_BUSY             : 1;
      unsigned IFE_STRIPECNT        : 6;
      unsigned                      : 1;
      unsigned BP1                  : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_14; // 0x0038

  union
  {
    struct
    {
      unsigned RDLT_G0        : 6;
      unsigned                : 2;
      unsigned RDLT_G1        : 6;
      unsigned                : 2;
      unsigned RDLT_G2        : 6;
      unsigned                : 2;
      unsigned RDLT_G3        : 6;
    } Bit;
    UINT32 Word;
  } IFE_Register_15; // 0x003c

  union
  {
    struct
    {
      unsigned RDLT_RB0        : 6;
      unsigned                 : 2;
      unsigned RDLT_RB1        : 6;
      unsigned                 : 2;
      unsigned RDLT_RB2        : 6;
      unsigned                 : 2;
      unsigned RDLT_RB3        : 6;
    } Bit;
    UINT32 Word;
  } IFE_Register_16; // 0x0040

  union
  {
    struct
    {
      unsigned UPSRC_EN            : 1;
      unsigned                     : 1;
      unsigned IMAGE_SADDR1        : 23;
    } Bit;
    UINT32 Word;
  } IFE_Register_17; // 0x0044

  union
  {
    struct
    {
      unsigned DOWNSRC_EN          : 1;
      unsigned                     : 1;
      unsigned IMAGE_SADDR2        : 23;
    } Bit;
    UINT32 Word;
  } IFE_Register_18; // 0x0048

  union
  {
    struct
    {
      unsigned OUTLTH_BRI         : 8;
      unsigned OUTLTH_DARK        : 8;
      unsigned OUTCNT_SEL         : 1;
    } Bit;
    UINT32 Word;
  } IFE_Register_19; // 0x004c

  union
  {
    struct
    {
      unsigned ROWDEF_FACT         : 2;
      unsigned                     : 2;
      unsigned ROWDEF_STIDX        : 4;
      unsigned ROWDEF_OFFS         : 15;
    } Bit;
    UINT32 Word;
  } IFE_Register_20; // 0x0050

  union
  {
    struct
    {
      unsigned ROW0        : 16;
      unsigned ROW1        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_21; // 0x0054

  union
  {
    struct
    {
      unsigned ROW2        : 16;
      unsigned ROW3        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_22; // 0x0058

  union
  {
    struct
    {
      unsigned ROW4        : 16;
      unsigned ROW5        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_23; // 0x005c

  union
  {
    struct
    {
      unsigned ROW6        : 16;
      unsigned ROW7        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_24; // 0x0060

  union
  {
    struct
    {
      unsigned ROW8        : 16;
      unsigned ROW9        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_25; // 0x0064

  union
  {
    struct
    {
      unsigned ROW10        : 16;
      unsigned ROW11        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_26; // 0x0068

  union
  {
    struct
    {
      unsigned ROW12        : 16;
      unsigned ROW13        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_27; // 0x006c

  union
  {
    struct
    {
      unsigned ROW14        : 16;
      unsigned ROW15        : 16;
    } Bit;
    UINT32 Word;
  } IFE_Register_28; // 0x0070

  
} NT96220_IFE_REG_STRUCT;


#endif
