#ifndef _IME_NT96220_H_
#define _IME_NT96220_H_

#include "DataType.h"

//#define _IME_REG_BASE_ADDR    0x9A200000

typedef struct
{

  union
  {
    struct
    {
      unsigned IME_RST               : 1;
      unsigned IME_START             : 1;
      unsigned IME_LOAD              : 1;
      unsigned IME_ID_RST_EN1        : 1;
    } Bit;
    UINT32 Word;
  } IME_Register_0; // 0x0000

  union
  {
    struct
    {
      unsigned IME_SRC              : 1;
      unsigned IME_PATH             : 2;
      unsigned                      : 1;
      unsigned IME_IMAT             : 2;
      unsigned IME_STEN             : 1;
      unsigned                      : 1;
      unsigned IME_OPPBE1           : 3;
      unsigned                      : 1;
      unsigned ICST_EN              : 1;
      unsigned ICST_OPT             : 1;
      unsigned IME_OPPBNUM1         : 3;
      unsigned                      : 1;
      unsigned IME_OPPBNUMR1        : 3;
      unsigned                      : 1;
      unsigned SLICE_MODE1          : 1;
      unsigned                      : 1;
      unsigned SLICE_H              : 3;
      unsigned SLICE_TRIG_EN        : 1;
    } Bit;
    UINT32 Word;
  } IME_Register_1; // 0x0004

  union
  {
    struct
    {
      unsigned ST_HN        : 5;
      unsigned              : 1;
      unsigned ST_HL        : 5;
      unsigned              : 1;
      unsigned ST_HM        : 8;
    } Bit;
    UINT32 Word;
  } IME_Register_2; // 0x0008

  union
  {
    struct
    {
      unsigned ST_VN        : 12;
      unsigned ST_VL        : 12;
      unsigned ST_VM        : 8;
    } Bit;
    UINT32 Word;
  } IME_Register_3; // 0x000c

  union
  {
    struct
    {
      unsigned H1_UD              : 1;
      unsigned V1_UD              : 1;
      unsigned H1_DNRATE          : 5;
      unsigned V1_DNRATE          : 5;
      unsigned IME_OMAT1          : 3;
      unsigned H1_FILTMODE        : 1;
      unsigned H1_FILTCOEF        : 6;
      unsigned V1_FILTMODE        : 1;
      unsigned V1_FILTCOEF        : 6;
      unsigned                    : 1;
      unsigned IME_CFAPAT         : 2;
    } Bit;
    UINT32 Word;
  } IME_Register_4; // 0x0010

  union
  {
    struct
    {
      unsigned H1_SFACT        : 16;
      unsigned V1_SFACT        : 16;
    } Bit;
    UINT32 Word;
  } IME_Register_5; // 0x0014

  union
  {
    struct
    {
      unsigned H1_OSIZE        : 16;
      unsigned V1_OSIZE        : 16;
    } Bit;
    UINT32 Word;
  } IME_Register_6; // 0x0018

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_7; // 0x001c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_8; // 0x0020

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_9; // 0x0024

  union
  {
    struct
    {
      unsigned                   : 2;
      unsigned DRAM_SAIY0        : 23;
    } Bit;
    UINT32 Word;
  } IME_Register_10; // 0x0028

  union
  {
    struct
    {
      unsigned                    : 2;
      unsigned DRAM_SAICB0        : 23;
    } Bit;
    UINT32 Word;
  } IME_Register_11; // 0x002c

  union
  {
    struct
    {
      unsigned                    : 2;
      unsigned DRAM_SAICR0        : 23;
    } Bit;
    UINT32 Word;
  } IME_Register_12; // 0x0030

  union
  {
    struct
    {
      unsigned IME_STRIPE_BP1        : 13;
      unsigned                       : 3;
      unsigned IME_STRIPE_BP2        : 13;
    } Bit;
    UINT32 Word;
  } IME_Register_13; // 0x0034

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_14; // 0x0038

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_15; // 0x003c

  union
  {
    struct
    {
      unsigned                    : 2;
      unsigned DRAM_OFSI_Y        : 14;
    } Bit;
    UINT32 Word;
  } IME_Register_16; // 0x0040

  union
  {
    struct
    {
      unsigned DRAM_SAO0_1Y        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_17; // 0x0044

  union
  {
    struct
    {
      unsigned DRAM_SAO0_1CB        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_18; // 0x0048

  union
  {
    struct
    {
      unsigned DRAM_SAO0_1CR        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_19; // 0x004c

  union
  {
    struct
    {
      unsigned DRAM_SAO1_1Y        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_20; // 0x0050

  union
  {
    struct
    {
      unsigned DRAM_SAO1_1CB        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_21; // 0x0054

  union
  {
    struct
    {
      unsigned DRAM_SAO1_1CR        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_22; // 0x0058

  union
  {
    struct
    {
      unsigned DRAM_SAO2_1Y        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_23; // 0x005c

  union
  {
    struct
    {
      unsigned DRAM_SAO2_1CB        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_24; // 0x0060

  union
  {
    struct
    {
      unsigned DRAM_SAO2_1CR        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_25; // 0x0064

  union
  {
    struct
    {
      unsigned                     : 2;
      unsigned DRAM_OFSO_1Y        : 16;
    } Bit;
    UINT32 Word;
  } IME_Register_26; // 0x0068

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_27; // 0x006c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_28; // 0x0070

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_29; // 0x0074

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_30; // 0x0078

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_31; // 0x007c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_32; // 0x0080

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_33; // 0x0084

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_34; // 0x0088

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_35; // 0x008c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_36; // 0x0090

  union
  {
    struct
    {
      unsigned                       : 2;
      unsigned INTE_FRM              : 1;
      unsigned                       : 4;
      unsigned INTE_V1O16LN          : 1;
      unsigned INTE_LINE_BP1         : 1;
      unsigned INTE_SLICE_DN1        : 1;
      unsigned INTE_LINE_BP2         : 1;
    } Bit;
    UINT32 Word;
  } IME_Register_37; // 0x0094

  union
  {
    struct
    {
      unsigned                      : 2;
      unsigned INT_FRM              : 1;
      unsigned                      : 4;
      unsigned INT_V1O16LN          : 1;
      unsigned INT_LINE_BP1         : 1;
      unsigned INT_SLICE_DN1        : 1;
      unsigned INT_LINE_BP2         : 1;
    } Bit;
    UINT32 Word;
  } IME_Register_38; // 0x0098

  union
  {
    struct
    {
      unsigned CST_PROM0        : 8;
      unsigned CST_PROM1        : 8;
      unsigned CST_PROM2        : 8;
      unsigned CST_PROM3        : 8;
    } Bit;
    UINT32 Word;
  } IME_Register_39; // 0x009c

  union
  {
    struct
    {
      unsigned OVCNT1          : 16;
      unsigned                 : 12;
      unsigned IME_BUSY        : 1;
    } Bit;
    UINT32 Word;
  } IME_Register_40; // 0x00a0

  union
  {
    struct
    {
      unsigned DRAM_SAO3_1Y        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_41; // 0x00a4

  union
  {
    struct
    {
      unsigned DRAM_SAO3_1CB        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_42; // 0x00a8

  union
  {
    struct
    {
      unsigned DRAM_SAO3_1CR        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_43; // 0x00ac

  union
  {
    struct
    {
      unsigned DRAM_SAO4_1Y        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_44; // 0x00b0

  union
  {
    struct
    {
      unsigned DRAM_SAO4_1CB        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_45; // 0x00b4

  union
  {
    struct
    {
      unsigned DRAM_SAO4_1CR        : 25;
    } Bit;
    UINT32 Word;
  } IME_Register_46; // 0x00b8

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_47; // 0x00bc

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_48; // 0x00c0

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_49; // 0x00c4

  union
  {
    struct
    {
      unsigned IME_P1NEAR        : 1;
    } Bit;
    UINT32 Word;
  } IME_Register_50; // 0x00c8

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_51; // 0x00cc

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IME_Register_52; // 0x00d0

  union
  {
    struct
    {
      unsigned PPBO_INIT_ID1        : 3;
    } Bit;
    UINT32 Word;
  } IME_Register_53; // 0x00d4

  union
  {
    struct
    {
      unsigned Reserved            : 2;
      unsigned DRAM_OFSI_CB        : 14;
    } Bit;
    UINT32 Word;
  } IME_Register_54; // 0x00d8

  union
  {
    struct
    {
      unsigned Reserved            : 2;
      unsigned DRAM_OFSI_CR        : 14;
    } Bit;
    UINT32 Word;
  } IME_Register_55; // 0x00dc

  union
  {
    struct
    {
      unsigned Reserved             : 2;
      unsigned DRAM_OFSO_1CB        : 16;
    } Bit;
    UINT32 Word;
  } IME_Register_56; // 0x00e0

  union
  {
    struct
    {
      unsigned Reserved             : 2;
      unsigned DRAM_OFSO_1CR        : 16;
    } Bit;
    UINT32 Word;
  } IME_Register_57; // 0x00e4

  union
  {
    struct
    {
      unsigned FMGRN_1_EN             : 1;
      unsigned Reserved               : 3;
      unsigned FMGRN_1_WEIGHT0        : 8;
      unsigned FMGRN_1_WEIGHT1        : 8;
      unsigned FMGRN_1_VAL            : 8;
    } Bit;
    UINT32 Word;
  } IME_Register_58; // 0x00e8

  union
  {
    struct
    {
      unsigned HAVGEN                  : 1;
      unsigned HAVGSIZE                : 2;
      unsigned HAVGCOEFF               : 5;
      unsigned HAVG_TH                 : 8;
      unsigned HAVG_SPL_ONLY_EN        : 1;
      unsigned                         : 3;
      unsigned HAVG_SPL_TAP            : 4;
    } Bit;
    UINT32 Word;
  } IME_Register_59; // 0x00ec

  union
  {
    struct
    {
      unsigned CRVMAP_EN               : 1;
      unsigned CRVMAP_DELTA            : 2;
      unsigned                         : 1;
      unsigned CRVMAP_CENTER_CB        : 8;
      unsigned                         : 4;
      unsigned CRVMAP_CENTER_CR        : 8;
    } Bit;
    UINT32 Word;
  } IME_Register_60; // 0x00f0

  union
  {
    struct
    {
      unsigned CRVMAP_LUT00        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT01        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT02        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT03        : 6;
    } Bit;
    UINT32 Word;
  } IME_Register_61; // 0x00f4

  union
  {
    struct
    {
      unsigned CRVMAP_LUT04        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT05        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT06        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT07        : 6;
    } Bit;
    UINT32 Word;
  } IME_Register_62; // 0x00f8

  union
  {
    struct
    {
      unsigned CRVMAP_LUT08        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT09        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT10        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT11        : 6;
    } Bit;
    UINT32 Word;
  } IME_Register_63; // 0x00fc

  union
  {
    struct
    {
      unsigned CRVMAP_LUT12        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT13        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT14        : 6;
      unsigned                     : 2;
      unsigned CRVMAP_LUT15        : 6;
    } Bit;
    UINT32 Word;
  } IME_Register_64; // 0x0100

} NT96220_IME_REG_STRUCT;


#endif
