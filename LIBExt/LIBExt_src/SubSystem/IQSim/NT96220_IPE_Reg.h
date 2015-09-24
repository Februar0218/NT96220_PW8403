#ifndef _IPE_NT96220_H_
#define _IPE_NT96220_H_

#include "DataType.h"

//#define _IPE_REG_BASE_ADDR    0x9A100000

typedef struct
{

  union
  {
    struct
    {
      unsigned IPE_CLR            : 1;
      unsigned IPE_START          : 1;
      unsigned IPE_LOAD           : 1;
      unsigned                    : 1;
      unsigned SLI_GO             : 1;
      unsigned                    : 22;
      unsigned SAVE_IME_OUT       : 1;
      unsigned IPE_VERSION        : 4;
    } Bit;
    UINT32 Word;
  } IPE_Register_0; // 0x0000

  union
  {
    struct
    {
      unsigned IPE_CFAPAT                : 2;
      unsigned IPE_DVI                   : 1;
      unsigned                           : 1;
      unsigned IPE_AUTOTRG_EN            : 1;
      unsigned IPE_RGBIN_EN              : 1;
      unsigned IFE_EMBED_EN              : 1;
      unsigned                           : 1;
      unsigned IPE_OMAT                  : 1;
      unsigned                           : 1;
      unsigned IPE_MODE                  : 2;
      unsigned IPE_PPBI_EN               : 1;
      unsigned IPE_PPBI_SEL              : 1;
      unsigned IPE_PPBI_INI              : 2;
      unsigned                           : 1;
      unsigned IPE_VPAD_EN               : 1;
      unsigned                           : 2;
      unsigned DVI_FORMAT                : 2;
      unsigned DVI_HSMP                  : 2;
      unsigned                           : 1;
      unsigned SLICE_TRIG_SRC            : 1;
      unsigned SLICE_REG_LOAD_SRC        : 1;
      unsigned                           : 1;
      unsigned IPE_DEBUGSEL              : 2;
    } Bit;
    UINT32 Word;
  } IPE_Register_1; // 0x0004

  union
  {
    struct
    {
      unsigned CGAIN_EN            : 1;
      unsigned                     : 1;
      unsigned RLPF_EN             : 1;
      unsigned GLPF_EN             : 1;
      unsigned BLPF_EN             : 1;
      unsigned CR_EN               : 1;
      unsigned                     : 1;
      unsigned CTRL_EN             : 1;
      unsigned HADJ_EN             : 1;
      unsigned                     : 1;
      unsigned CADJ_EN             : 1;
      unsigned CADJ_YENH_EN        : 1;
      unsigned CADJ_YLPF_EN        : 1;
      unsigned CADJ_YCON_EN        : 1;
      unsigned CADJ_CLPF_EN        : 1;
      unsigned CADJ_CCON_EN        : 1;
      unsigned VACC_EN             : 1;
      unsigned CADJ_YCTH_EN        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_2; // 0x0008

  union
  {
    struct
    {
      unsigned HN            : 5;
      unsigned               : 1;
      unsigned HL            : 5;
      unsigned               : 1;
      unsigned HM            : 4;
      unsigned HOLAP         : 1;
      unsigned HM_MSB        : 4;
      unsigned VM_MSB        : 2;
    } Bit;
    UINT32 Word;
  } IPE_Register_3; // 0x000c

  union
  {
    struct
    {
      unsigned VN           : 12;
      unsigned VL           : 12;
      unsigned VM           : 6;
      unsigned VOLAP        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_4; // 0x0010

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_SAI0        : 23;
    } Bit;
    UINT32 Word;
  } IPE_Register_5; // 0x0014

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_SAI1        : 23;
    } Bit;
    UINT32 Word;
  } IPE_Register_6; // 0x0018

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_OFSI        : 13;
    } Bit;
    UINT32 Word;
  } IPE_Register_7; // 0x001c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_8; // 0x0020

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned DRAM_SAI2        : 23;
    } Bit;
    UINT32 Word;
  } IPE_Register_9; // 0x0024

  union
  {
    struct
    {
      unsigned SLICE_HEIGHT        : 12;
    } Bit;
    UINT32 Word;
  } IPE_Register_10; // 0x0028

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_11; // 0x002c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_12; // 0x0030

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_13; // 0x0034

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_14; // 0x0038

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_15; // 0x003c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_16; // 0x0040

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_17; // 0x0044

  union
  {
    struct
    {
      unsigned INTE_ST              : 1;
      unsigned INTE_SLI             : 1;
      unsigned INTE_FRM             : 1;
      unsigned                      : 1;
      unsigned INTE_VACC            : 1;
      unsigned INTE_FRMERR          : 1;
      unsigned INTE_SLIERR          : 1;
      unsigned                      : 1;
      unsigned INTE_BUF0FAIL        : 1;
      unsigned INTE_BUF1FAIL        : 1;
      unsigned INTE_BUF2FAIL        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_18; // 0x0048

  union
  {
    struct
    {
      unsigned INT_ST              : 1;
      unsigned INT_SLI             : 1;
      unsigned INT_FRM             : 1;
      unsigned                     : 1;
      unsigned INT_VACC            : 1;
      unsigned INT_FRMERR          : 1;
      unsigned INT_SLIERR          : 1;
      unsigned                     : 1;
      unsigned INT_BUF0FAIL        : 1;
      unsigned INT_BUF1FAIL        : 1;
      unsigned INT_BUF2FAIL        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_19; // 0x004c

  union
  {
    struct
    {
      unsigned             : 10;
      unsigned GBAL        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_20; // 0x0050

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_21; // 0x0054

  union
  {
    struct
    {
      unsigned INTG_NSMAR        : 8;
      unsigned                   : 3;
      unsigned LPFW              : 4;
      unsigned SUM_SEL           : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_22; // 0x0058

  union
  {
    struct
    {
      unsigned CCR_SET        : 1;
      unsigned                : 15;
      unsigned COEF_RR        : 11;
    } Bit;
    UINT32 Word;
  } IPE_Register_23; // 0x005c

  union
  {
    struct
    {
      unsigned COEF_RG        : 11;
      unsigned                : 5;
      unsigned COEF_RB        : 11;
    } Bit;
    UINT32 Word;
  } IPE_Register_24; // 0x0060

  union
  {
    struct
    {
      unsigned COEF_GR        : 11;
      unsigned                : 5;
      unsigned COEF_GG        : 11;
    } Bit;
    UINT32 Word;
  } IPE_Register_25; // 0x0064

  union
  {
    struct
    {
      unsigned COEF_GB        : 11;
      unsigned                : 5;
      unsigned COEF_BR        : 11;
    } Bit;
    UINT32 Word;
  } IPE_Register_26; // 0x0068

  union
  {
    struct
    {
      unsigned COEF_BG        : 11;
      unsigned                : 5;
      unsigned COEF_BB        : 11;
    } Bit;
    UINT32 Word;
  } IPE_Register_27; // 0x006c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_28; // 0x0070

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_29; // 0x0074

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_30; // 0x0078

  union
  {
    struct
    {
      unsigned CGAIN_INV          : 1;
      unsigned CGAIN_HINV         : 1;
      unsigned CGAIN_RANGE        : 1;
      unsigned                    : 5;
      unsigned CGAIN_MASK         : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_31; // 0x007c

  union
  {
    struct
    {
      unsigned CGAIN0        : 10;
      unsigned               : 6;
      unsigned CGAIN1        : 10;
    } Bit;
    UINT32 Word;
  } IPE_Register_32; // 0x0080

  union
  {
    struct
    {
      unsigned CGAIN2        : 10;
      unsigned               : 6;
      unsigned CGAIN3        : 10;
    } Bit;
    UINT32 Word;
  } IPE_Register_33; // 0x0084

  union
  {
    struct
    {
      unsigned COFS0        : 8;
      unsigned COFS1        : 8;
      unsigned COFS2        : 8;
      unsigned COFS3        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_34; // 0x0088

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_35; // 0x008c

  union
  {
    struct
    {
      unsigned INT_OFS        : 8;
      unsigned SAT_OFS        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_36; // 0x0090

  union
  {
    struct
    {
      unsigned CHUEM0        : 8;
      unsigned CHUEM1        : 8;
      unsigned CHUEM2        : 8;
      unsigned CHUEM3        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_37; // 0x0094

  union
  {
    struct
    {
      unsigned CHUEM4        : 8;
      unsigned CHUEM5        : 8;
      unsigned CHUEM6        : 8;
      unsigned CHUEM7        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_38; // 0x0098

  union
  {
    struct
    {
      unsigned CHUEM8         : 8;
      unsigned CHUEM9         : 8;
      unsigned CHUEM10        : 8;
      unsigned CHUEM11        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_39; // 0x009c

  union
  {
    struct
    {
      unsigned CHUEM12        : 8;
      unsigned CHUEM13        : 8;
      unsigned CHUEM14        : 8;
      unsigned CHUEM15        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_40; // 0x00a0

  union
  {
    struct
    {
      unsigned CHUEM16        : 8;
      unsigned CHUEM17        : 8;
      unsigned CHUEM18        : 8;
      unsigned CHUEM19        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_41; // 0x00a4

  union
  {
    struct
    {
      unsigned CHUEM20        : 8;
      unsigned CHUEM21        : 8;
      unsigned CHUEM22        : 8;
      unsigned CHUEM23        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_42; // 0x00a8

  union
  {
    struct
    {
      unsigned CINTM0        : 8;
      unsigned CINTM1        : 8;
      unsigned CINTM2        : 8;
      unsigned CINTM3        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_43; // 0x00ac

  union
  {
    struct
    {
      unsigned CINTM4        : 8;
      unsigned CINTM5        : 8;
      unsigned CINTM6        : 8;
      unsigned CINTM7        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_44; // 0x00b0

  union
  {
    struct
    {
      unsigned CINTM8         : 8;
      unsigned CINTM9         : 8;
      unsigned CINTM10        : 8;
      unsigned CINTM11        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_45; // 0x00b4

  union
  {
    struct
    {
      unsigned CINTM12        : 8;
      unsigned CINTM13        : 8;
      unsigned CINTM14        : 8;
      unsigned CINTM15        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_46; // 0x00b8

  union
  {
    struct
    {
      unsigned CINTM16        : 8;
      unsigned CINTM17        : 8;
      unsigned CINTM18        : 8;
      unsigned CINTM19        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_47; // 0x00bc

  union
  {
    struct
    {
      unsigned CINTM20        : 8;
      unsigned CINTM21        : 8;
      unsigned CINTM22        : 8;
      unsigned CINTM23        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_48; // 0x00c0

  union
  {
    struct
    {
      unsigned CSATM0        : 8;
      unsigned CSATM1        : 8;
      unsigned CSATM2        : 8;
      unsigned CSATM3        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_49; // 0x00c4

  union
  {
    struct
    {
      unsigned CSATM4        : 8;
      unsigned CSATM5        : 8;
      unsigned CSATM6        : 8;
      unsigned CSATM7        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_50; // 0x00c8

  union
  {
    struct
    {
      unsigned CSATM8         : 8;
      unsigned CSATM9         : 8;
      unsigned CSATM10        : 8;
      unsigned CSATM11        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_51; // 0x00cc

  union
  {
    struct
    {
      unsigned CSATM12        : 8;
      unsigned CSATM13        : 8;
      unsigned CSATM14        : 8;
      unsigned CSATM15        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_52; // 0x00d0

  union
  {
    struct
    {
      unsigned CSATM16        : 8;
      unsigned CSATM17        : 8;
      unsigned CSATM18        : 8;
      unsigned CSATM19        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_53; // 0x00d4

  union
  {
    struct
    {
      unsigned CSATM20        : 8;
      unsigned CSATM21        : 8;
      unsigned CSATM22        : 8;
      unsigned CSATM23        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_54; // 0x00d8

  union
  {
    struct
    {
      unsigned DDS0        : 6;
      unsigned DDS1        : 6;
      unsigned DDS2        : 6;
      unsigned DDS3        : 6;
    } Bit;
    UINT32 Word;
  } IPE_Register_55; // 0x00dc

  union
  {
    struct
    {
      unsigned DDS4        : 6;
      unsigned DDS5        : 6;
      unsigned DDS6        : 6;
      unsigned DDS7        : 6;
    } Bit;
    UINT32 Word;
  } IPE_Register_56; // 0x00e0

  union
  {
    struct
    {
      unsigned Y_FIX         : 8;
      unsigned CB_FIX        : 8;
      unsigned CR_FIX        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_57; // 0x00e4

  union
  {
    struct
    {
      unsigned Y_ENH_P         : 10;
      unsigned Y_ENH_N         : 10;
      unsigned Y_ETHY          : 10;
      unsigned Y_EINV_P        : 1;
      unsigned Y_EINV_N        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_58; // 0x00e8

  union
  {
    struct
    {
      unsigned Y_ETHC        : 10;
      unsigned               : 6;
      unsigned Y_CON         : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_59; // 0x00ec

  union
  {
    struct
    {
      unsigned C_CBOFS            : 8;
      unsigned C_CROFS            : 8;
      unsigned C_CON              : 8;
      unsigned CADJ_LMFSEL        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_60; // 0x00f0

  union
  {
    struct
    {
      unsigned VDET_DIV        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_61; // 0x00f4

  union
  {
    struct
    {
      unsigned                  : 2;
      unsigned VACC_GAIN        : 3;
      unsigned                  : 1;
      unsigned STX              : 10;
      unsigned STY              : 9;
      unsigned VACC_MODE        : 1;
      unsigned STX_EX           : 1;
      unsigned STY_EX           : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_62; // 0x00f8

  union
  {
    struct
    {
      unsigned HNF          : 4;
      unsigned              : 4;
      unsigned HSZ          : 9;
      unsigned VSZ          : 8;
      unsigned VA_TH        : 7;
    } Bit;
    UINT32 Word;
  } IPE_Register_63; // 0x00fc

  union
  {
    struct
    {
      unsigned VACC0        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_64; // 0x0100

  union
  {
    struct
    {
      unsigned VACC1        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_65; // 0x0104

  union
  {
    struct
    {
      unsigned VACC2        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_66; // 0x0108

  union
  {
    struct
    {
      unsigned VACC3        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_67; // 0x010c

  union
  {
    struct
    {
      unsigned VACC4        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_68; // 0x0110

  union
  {
    struct
    {
      unsigned VACC5        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_69; // 0x0114

  union
  {
    struct
    {
      unsigned VACC6        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_70; // 0x0118

  union
  {
    struct
    {
      unsigned VACC7        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_71; // 0x011c

  union
  {
    struct
    {
      unsigned VACC8        : 18;
    } Bit;
    UINT32 Word;
  } IPE_Register_72; // 0x0120

  union
  {
    struct
    {
      unsigned EEXT0_H        : 5;
      unsigned EEXT0_V        : 5;
      unsigned EEXT1_H        : 5;
      unsigned EEXT1_V        : 5;
      unsigned EEXT2_H        : 5;
      unsigned EEXT2_V        : 5;
    } Bit;
    UINT32 Word;
  } IPE_Register_73; // 0x0124

  union
  {
    struct
    {
      unsigned EEXT3          : 5;
      unsigned EEXT4          : 5;
      unsigned EEXT5          : 5;
      unsigned EEXTDIV        : 3;
    } Bit;
    UINT32 Word;
  } IPE_Register_74; // 0x0128

  union
  {
    struct
    {
      unsigned EMAPA        : 10;
      unsigned EMAPB        : 10;
      unsigned EMAPC        : 10;
    } Bit;
    UINT32 Word;
  } IPE_Register_75; // 0x012c

  union
  {
    struct
    {
      unsigned EMAPD        : 10;
      unsigned EMAPN        : 3;
      unsigned EMAPM        : 3;
      unsigned EMAPE        : 10;
    } Bit;
    UINT32 Word;
  } IPE_Register_76; // 0x0130

  union
  {
    struct
    {
      unsigned CADJ_YTH_H        : 8;
      unsigned CADJ_YTH_L        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_77; // 0x0134

  union
  {
    struct
    {
      unsigned CADJ_CBTH_H        : 8;
      unsigned CADJ_CBTH_L        : 8;
      unsigned CADJ_CRTH_H        : 8;
      unsigned CADJ_CRTH_L        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_78; // 0x0138

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_79; // 0x013c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_80; // 0x0140

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_81; // 0x0144

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_82; // 0x0148

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_83; // 0x014c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_84; // 0x0150

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_85; // 0x0154

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_86; // 0x0158

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_87; // 0x015c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_88; // 0x0160

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_89; // 0x0164

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_90; // 0x0168

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_91; // 0x016c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_92; // 0x0170

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_93; // 0x0174

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_94; // 0x0178

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_95; // 0x017c

  union
  {
    struct
    {
      unsigned CUR_FBID         : 2;
      unsigned PRE_FBID         : 2;
      unsigned PRE1_FBID        : 2;
      unsigned PRE2_FBID        : 2;
      unsigned                  : 8;
      unsigned HCNT             : 4;
      unsigned                  : 4;
      unsigned VCNT             : 6;
    } Bit;
    UINT32 Word;
  } IPE_Register_96; // 0x0180

  union
  {
    struct
    {
      unsigned STATUS_Y         : 1;
      unsigned STATUS_CB        : 1;
      unsigned STATUS_CR        : 1;
    } Bit;
    UINT32 Word;
  } IPE_Register_97; // 0x0184

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_98; // 0x0188

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_99; // 0x018c

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_100; // 0x0190

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_101; // 0x0194

  union
  {
    struct
    {
      unsigned Reserved        : 32;
    } Bit;
    UINT32 Word;
  } IPE_Register_102; // 0x0198

  union
  {
    struct
    {
      unsigned ESTAB0        : 8;
      unsigned ESTAB1        : 8;
      unsigned ESTAB2        : 8;
      unsigned ESTAB3        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_103; // 0x019c

  union
  {
    struct
    {
      unsigned ESTAB4        : 8;
      unsigned ESTAB5        : 8;
      unsigned ESTAB6        : 8;
      unsigned ESTAB7        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_104; // 0x01a0

  union
  {
    struct
    {
      unsigned ESTAB8         : 8;
      unsigned ESTAB9         : 8;
      unsigned ESTAB10        : 8;
      unsigned ESTAB11        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_105; // 0x01a4

  union
  {
    struct
    {
      unsigned ESTAB12        : 8;
      unsigned ESTAB13        : 8;
      unsigned ESTAB14        : 8;
      unsigned ESTAB15        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_106; // 0x01a8

  union
  {
    struct
    {
      unsigned FSTAB0        : 8;
      unsigned FSTAB1        : 8;
      unsigned FSTAB2        : 8;
      unsigned FSTAB3        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_107; // 0x01ac

  union
  {
    struct
    {
      unsigned FSTAB4        : 8;
      unsigned FSTAB5        : 8;
      unsigned FSTAB6        : 8;
      unsigned FSTAB7        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_108; // 0x01b0

  union
  {
    struct
    {
      unsigned FSTAB8         : 8;
      unsigned FSTAB9         : 8;
      unsigned FSTAB10        : 8;
      unsigned FSTAB11        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_109; // 0x01b4

  union
  {
    struct
    {
      unsigned FSTAB12        : 8;
      unsigned FSTAB13        : 8;
      unsigned FSTAB14        : 8;
      unsigned FSTAB15        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_110; // 0x01b8

  union
  {
    struct
    {
      unsigned FDTAB0        : 8;
      unsigned FDTAB1        : 8;
      unsigned FDTAB2        : 8;
      unsigned FDTAB3        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_111; // 0x01bc

  union
  {
    struct
    {
      unsigned FDTAB4        : 8;
      unsigned FDTAB5        : 8;
      unsigned FDTAB6        : 8;
      unsigned FDTAB7        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_112; // 0x01c0

  union
  {
    struct
    {
      unsigned FDTAB8         : 8;
      unsigned FDTAB9         : 8;
      unsigned FDTAB10        : 8;
      unsigned FDTAB11        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_113; // 0x01c4

  union
  {
    struct
    {
      unsigned FDTAB12        : 8;
      unsigned FDTAB13        : 8;
      unsigned FDTAB14        : 8;
      unsigned FDTAB15        : 8;
    } Bit;
    UINT32 Word;
  } IPE_Register_114; // 0x01c8

} NT96220_IPE_REG_STRUCT;


#endif
