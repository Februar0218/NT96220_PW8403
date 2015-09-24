#ifndef _SIE_NT96220_H_
#define _SIE_NT96220_H_

#include "DataType.h"

//#define _SIE_REG_BASE_ADDR    0x9A000000

typedef struct
{

  union
  {
    struct
    {
      unsigned SIE_RST           : 1;
      unsigned SIE_LOAD          : 1;
      unsigned                   : 2;
      unsigned DRAM_IEN          : 1;
      unsigned FLSH_EN           : 1;
      unsigned MSH_EN            : 1;
      unsigned FLSH_CTRL         : 1;
      unsigned FLSH_PHASE        : 1;
      unsigned MSH_CTRL          : 1;
      unsigned MSH_PHASE         : 1;
      unsigned SAVE_SIE_OUT      : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_0; // 0x0000

  union
  {
    struct
    {
      unsigned SYN_MASTER              : 1;
      unsigned                         : 1;
      unsigned SLICE_EN                : 1;
      unsigned VD_PHASE                : 1;
      unsigned HD_PHASE                : 1;
      unsigned DATA_PHASE              : 1;
      unsigned LINE_BLM_EN             : 1;
      unsigned CROP_EN                 : 1;
      unsigned OB_AVER_EN              : 1;
      unsigned OB_EN                   : 1;
      unsigned BLM_EN                  : 1;
      unsigned HSCL_EN                 : 1;
      unsigned CG_EN                   : 1;
      unsigned GAMMA_EN                : 1;
      unsigned _SUBOUT_E               : 1;
      unsigned DRAM_OUTPACK_SEL        : 1;
      unsigned OUT_SRC                 : 2;
      unsigned PADDING_EN              : 1;
      unsigned                         : 1;
      unsigned CHOUT_EN                : 1;
      unsigned HFLIP_EN                : 1;
      unsigned VFLIP_EN                : 1;
      unsigned                         : 1;
      unsigned ECS_EN                  : 1;
      unsigned DRAM_INPACK_SEL         : 1;
      unsigned                         : 1;
      unsigned DF_SUB                  : 1;
      unsigned DF_SUB_OP               : 2;
      unsigned DF_EMBED                : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_1; // 0x0004

  union
  {
    struct
    {
      unsigned CH_TOTAL        : 2;
      unsigned                 : 2;
      unsigned CH0_NUM         : 3;
      unsigned                 : 1;
      unsigned CH1_NUM         : 3;
      unsigned                 : 1;
      unsigned CH2_NUM         : 3;
      unsigned                 : 1;
      unsigned CH3_NUM         : 3;
      unsigned                 : 1;
      unsigned CH4_NUM         : 3;
      unsigned                 : 1;
      unsigned CH5_NUM         : 3;
    } Bit;
    UINT32 Word;
  } SIE_Register_2; // 0x0008

  union
  {
    struct
    {
      unsigned BP1        : 16;
      unsigned BP2        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_3; // 0x000c

  union
  {
    struct
    {
      unsigned SIE_INTERRUPT_EN        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_4; // 0x0010

  union
  {
    struct
    {
      unsigned SIE_INTERRUPT_STATUS        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_5; // 0x0014

  union
  {
    struct
    {
      unsigned HVDPRD         : 16;
      unsigned HVDSPRD        : 15;
      unsigned VD_INV         : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_6; // 0x0018

  union
  {
    struct
    {
      unsigned HDPRD         : 14;
      unsigned               : 2;
      unsigned HDSPRD        : 13;
      unsigned               : 2;
      unsigned HD_INV        : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_7; // 0x001c

  union
  {
    struct
    {
      unsigned                     : 2;
      unsigned DRAM_OFSO_CH        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_8; // 0x0020

  union
  {
    struct
    {
      unsigned DF_SUB_TH              : 10;
      unsigned DF_SUB_THSOURCE        : 1;
      unsigned                        : 1;
      unsigned DF_SUB_SHIFT           : 4;
    } Bit;
    UINT32 Word;
  } SIE_Register_9; // 0x0024

  union
  {
    struct
    {
      unsigned LN_CNT         : 16;
      unsigned VSYNC          : 1;
      unsigned HSYNC          : 1;
      unsigned                : 2;
      unsigned F4PX_CNT        : 12;
    } Bit;
    UINT32 Word;
  } SIE_Register_10; // 0x0028

  union
  {
    struct
    {
      unsigned                      : 2;
      unsigned CURR_DRAM_SAO        : 23;
      unsigned                      : 4;
      unsigned CURR_DRAM_OID        : 2;
    } Bit;
    UINT32 Word;
  } SIE_Register_11; // 0x002c

  union
  {
    struct
    {
      unsigned VV_INI           : 16;
      unsigned VV_HEIGHT        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_12; // 0x0030

  union
  {
    struct
    {
      unsigned HV_INI          : 14;
      unsigned                 : 2;
      unsigned HV_WIDTH        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_13; // 0x0034

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_SAO0        : 23;
    } Bit;
    UINT32 Word;
  } SIE_Register_14; // 0x0038

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_OFSO        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_15; // 0x003c

  union
  {
    struct
    {
      unsigned DRAM_BUFNUM        : 2;
      unsigned DRAM_SAO1          : 23;
    } Bit;
    UINT32 Word;
  } SIE_Register_16; // 0x0040

  union
  {
    struct
    {
      unsigned                    : 13;
      unsigned PATGEN_MODE        : 3;
      unsigned PATGEN_VAL         : 12;
      unsigned SIE_CFAPAT         : 2;
    } Bit;
    UINT32 Word;
  } SIE_Register_17; // 0x0044

  union
  {
    struct
    {
      unsigned ACT_WIDTH         : 14;
      unsigned                   : 2;
      unsigned ACT_HEIGHT        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_18; // 0x0048

  union
  {
    struct
    {
      unsigned HV_ACT        : 14;
      unsigned               : 2;
      unsigned VV_ACT        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_19; // 0x004c

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_SAI0        : 23;
    } Bit;
    UINT32 Word;
  } SIE_Register_20; // 0x0050

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_OFSI        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_21; // 0x0054

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_22; // 0x0058

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_SAI1        : 23;
    } Bit;
    UINT32 Word;
  } SIE_Register_23; // 0x005c

  union
  {
    struct
    {
      unsigned OB_X0        : 14;
      unsigned              : 2;
      unsigned OB_Y0        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_24; // 0x0060

  union
  {
    struct
    {
      unsigned OB_X1        : 14;
      unsigned              : 2;
      unsigned OB_Y1        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_25; // 0x0064

  union
  {
    struct
    {
      unsigned OB_THRES        : 10;
      unsigned                 : 6;
      unsigned OB_XDIV         : 8;
    } Bit;
    UINT32 Word;
  } SIE_Register_26; // 0x0068

  union
  {
    struct
    {
      unsigned OB_AVER        : 10;
      unsigned                : 6;
      unsigned OB_SCNT        : 9;
    } Bit;
    UINT32 Word;
  } SIE_Register_27; // 0x006c

  union
  {
    struct
    {
      unsigned OB_OFS        : 10;
    } Bit;
    UINT32 Word;
  } SIE_Register_28; // 0x0070

  union
  {
    struct
    {
      unsigned CG_OFS0        : 10;
      unsigned                : 6;
      unsigned CG_OFS1        : 10;
    } Bit;
    UINT32 Word;
  } SIE_Register_29; // 0x0074

  union
  {
    struct
    {
      unsigned CG_OFS2        : 10;
      unsigned                : 6;
      unsigned CG_OFS3        : 10;
    } Bit;
    UINT32 Word;
  } SIE_Register_30; // 0x0078

  union
  {
    struct
    {
      unsigned HSCL_FRT        : 8;
      unsigned HSCL_INT        : 5;
      unsigned                 : 3;
      unsigned HSCL_ADD        : 1;
      unsigned                 : 1;
      unsigned HSCL_DIV        : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_31; // 0x007c

  union
  {
    struct
    {
      unsigned                   : 2;
      unsigned LINE_WIDTH        : 12;
    } Bit;
    UINT32 Word;
  } SIE_Register_32; // 0x0080

  union
  {
    struct
    {
      unsigned GAIN0        : 10;
      unsigned              : 6;
      unsigned GAIN1        : 10;
    } Bit;
    UINT32 Word;
  } SIE_Register_33; // 0x0084

  union
  {
    struct
    {
      unsigned GAIN2           : 10;
      unsigned                 : 6;
      unsigned GAIN3           : 10;
      unsigned                 : 2;
      unsigned GAIN_SEL        : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_34; // 0x0088

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_35; // 0x008c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_36; // 0x0090

  union
  {
    struct
    {
      unsigned                    : 2;
      unsigned DRAM_SAOSUB        : 23;
    } Bit;
    UINT32 Word;
  } SIE_Register_37; // 0x0094

  union
  {
    struct
    {
      unsigned SUB_MODE            : 1;
      unsigned                     : 1;
      unsigned DRAM_OFSOSUB        : 14;
      unsigned SUB_SMPH            : 3;
      unsigned                     : 1;
      unsigned SUB_SMPV            : 3;
      unsigned                     : 7;
      unsigned DRAM_OSRCSUB        : 2;
    } Bit;
    UINT32 Word;
  } SIE_Register_38; // 0x0098

  union
  {
    struct
    {
      unsigned SUB_HCNT        : 13;
      unsigned                 : 3;
      unsigned SUB_VCNT        : 12;
    } Bit;
    UINT32 Word;
  } SIE_Register_39; // 0x009c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_40; // 0x00a0

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_41; // 0x00a4

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_42; // 0x00a8

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_43; // 0x00ac

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_44; // 0x00b0

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_45; // 0x00b4

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_46; // 0x00b8

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_47; // 0x00bc

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_48; // 0x00c0

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_49; // 0x00c4

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_50; // 0x00c8

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_51; // 0x00cc

  union
  {
    struct
    {
      unsigned FLSH_DLY1        : 16;
      unsigned FLSH_DLY2        : 13;
      unsigned FLSH_IMD         : 1;
      unsigned FLSH_POL         : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_52; // 0x00d0

  union
  {
    struct
    {
      unsigned FLSH_WIDTH        : 24;
    } Bit;
    UINT32 Word;
  } SIE_Register_53; // 0x00d4

  union
  {
    struct
    {
      unsigned MSH_DLY2        : 16;
      unsigned MSH_DLY3        : 13;
      unsigned                 : 1;
      unsigned MSH_DLY1        : 1;
    } Bit;
    UINT32 Word;
  } SIE_Register_54; // 0x00d8

  union
  {
    struct
    {
      unsigned MSH_WIDTH        : 24;
      unsigned MSH_SWAP         : 1;
      unsigned MSH_POL          : 1;
      unsigned MSH_MODE         : 1;
      unsigned                  : 1;
      unsigned MSH_FLD          : 4;
    } Bit;
    UINT32 Word;
  } SIE_Register_55; // 0x00dc

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_56; // 0x00e0

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_57; // 0x00e4

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_58; // 0x00e8

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_59; // 0x00ec

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_60; // 0x00f0

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_61; // 0x00f4

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } SIE_Register_62; // 0x00f8

  union
  {
    struct
    {
      unsigned DEFECT_NFACT        : 2;
    } Bit;
    UINT32 Word;
  } SIE_Register_63; // 0x00fc

  union
  {
    struct
    {
      unsigned DEFECT_LINE0        : 14;
      unsigned                     : 2;
      unsigned DEFECT_LINE1        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_64; // 0x0100

  union
  {
    struct
    {
      unsigned DEFECT_LINE2        : 14;
      unsigned                     : 2;
      unsigned DEFECT_LINE3        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_65; // 0x0104

  union
  {
    struct
    {
      unsigned DEFECT_LINE4        : 14;
      unsigned                     : 2;
      unsigned DEFECT_LINE5        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_66; // 0x0108

  union
  {
    struct
    {
      unsigned DEFECT_LINE6        : 14;
      unsigned                     : 2;
      unsigned DEFECT_LINE7        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_67; // 0x010c

  union
  {
    struct
    {
      unsigned DEFECT_LINE8        : 14;
      unsigned                     : 2;
      unsigned DEFECT_LINE9        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_68; // 0x0110

  union
  {
    struct
    {
      unsigned DEFECT_LINE10        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE11        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_69; // 0x0114

  union
  {
    struct
    {
      unsigned DEFECT_LINE12        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE13        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_70; // 0x0118

  union
  {
    struct
    {
      unsigned DEFECT_LINE14        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE15        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_71; // 0x011c

  union
  {
    struct
    {
      unsigned DEFECT_LINE16        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE17        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_72; // 0x0120

  union
  {
    struct
    {
      unsigned DEFECT_LINE18        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE19        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_73; // 0x0124

  union
  {
    struct
    {
      unsigned DEFECT_LINE20        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE21        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_74; // 0x0128

  union
  {
    struct
    {
      unsigned DEFECT_LINE22        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE23        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_75; // 0x012c

  union
  {
    struct
    {
      unsigned DEFECT_LINE24        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE25        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_76; // 0x0130

  union
  {
    struct
    {
      unsigned DEFECT_LINE26        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE27        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_77; // 0x0134

  union
  {
    struct
    {
      unsigned DEFECT_LINE28        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE29        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_78; // 0x0138

  union
  {
    struct
    {
      unsigned DEFECT_LINE30        : 14;
      unsigned                      : 2;
      unsigned DEFECT_LINE31        : 14;
    } Bit;
    UINT32 Word;
  } SIE_Register_79; // 0x013c

  union
  {
    struct
    {
      unsigned MAP_SIZESEL           : 2;
      unsigned                       : 2;
      unsigned MAP_DTHR_EN           : 1;
      unsigned MAP_DTHR_RST          : 1;
      unsigned                       : 2;
      unsigned MAP_DTHR_LEVEL        : 3;
      unsigned                       : 5;
      unsigned MAP_SHIFT             : 4;
    } Bit;
    UINT32 Word;
  } SIE_Register_80; // 0x0140

  union
  {
    struct
    {
      unsigned MAP_HSCL        : 16;
      unsigned MAP_VSCL        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_81; // 0x0144

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_SAO2        : 23;
    } Bit;
    UINT32 Word;
  } SIE_Register_82; // 0x0148

  union
  {
    struct
    {
      unsigned SLICE_HEIGHT        : 16;
    } Bit;
    UINT32 Word;
  } SIE_Register_83; // 0x014c

} NT96220_SIE_REG_STRUCT;


#endif
