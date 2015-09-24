#ifndef _IPP_EXTERN_NT96220_H_
#define _IPP_EXTERN_NT96220_H_

#include "DataType.h"


typedef struct
{
  union
  {
    struct
    {
      unsigned S_WEIGHT0_2         : 3;
      unsigned S_WEIGHT1_2         : 3;
      unsigned S_WEIGHT2_2         : 3;
      unsigned S_WEIGHT3_2         : 3;
      unsigned S_WEIGHT4_2         : 3;
      unsigned S_WEIGHT5_2         : 3;
      unsigned                     : 2;
      unsigned BINNING_2           : 3;
      unsigned                     : 5;
      unsigned S_ONLY_EN_2         : 1;
      unsigned S_ONLY_LEN_2        : 1;
      unsigned FILTER_EN_2         : 1;
      unsigned OUTL_EN_2           : 1;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_0; // 0x0000

  union
  {
    struct
    {
      unsigned RTH_G0_2        : 8;
      unsigned RTH_G1_2        : 8;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_1; // 0x0004

  union
  {
    struct
    {
      unsigned RTH_RB0_2      : 8;
      unsigned RTH_RB1_2      : 8;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_2; // 0x0008

  union
  {
    struct
    {
      unsigned RDLT_G0_2        : 6;
      unsigned                  : 2;
      unsigned RDLT_G1_2        : 6;
      unsigned                  : 2;
      unsigned RDLT_G2_2        : 6;
      unsigned                  : 2;
      unsigned RDLT_G3_2        : 6;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_3; // 0x000c

  union
  {
    struct
    {
      unsigned RDLT_RB0_2        : 6;
      unsigned                   : 2;
      unsigned RDLT_RB1_2        : 6;
      unsigned                   : 2;
      unsigned RDLT_RB2_2        : 6;
      unsigned                   : 2;
      unsigned RDLT_RB3_2        : 6;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_4; // 0x0010

  union
  {
    struct
    {
      unsigned OUTLTH_BRI_2         : 8;
      unsigned OUTLTH_DARK_2        : 8;
      unsigned OUTCNT_SEL_2         : 1;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_5; // 0x0014

  union
  {
    struct
    {
      unsigned CLAMP_TH_2           : 8;
      unsigned CLAMPMUL_G_2         : 7;
      unsigned                      : 1;
      unsigned CLAMPMUL_RB_2        : 7;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_6; // 0x0018

  union
  {
    struct
    {
      unsigned S_WEIGHT0_3         : 3;
      unsigned S_WEIGHT1_3         : 3;
      unsigned S_WEIGHT2_3         : 3;
      unsigned S_WEIGHT3_3         : 3;
      unsigned S_WEIGHT4_3         : 3;
      unsigned S_WEIGHT5_3         : 3;
      unsigned                     : 2;
      unsigned BINNING_3           : 3;
      unsigned                     : 5;
      unsigned S_ONLY_EN_3         : 1;
      unsigned S_ONLY_LEN_3        : 1;
      unsigned FILTER_EN_3         : 1;
      unsigned OUTL_EN_3           : 1;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_7; // 0x001c

  union
  {
    struct
    {
      unsigned RTH_G0_3        : 8;
      unsigned RTH_G1_3        : 8;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_8; // 0x0020

  union
  {
    struct
    {
      unsigned RTH_RB0_3      : 8;
      unsigned RTH_RB1_3      : 8;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_9; // 0x0024

  union
  {
    struct
    {
      unsigned RDLT_G0_3        : 6;
      unsigned                  : 2;
      unsigned RDLT_G1_3        : 6;
      unsigned                  : 2;
      unsigned RDLT_G2_3        : 6;
      unsigned                  : 2;
      unsigned RDLT_G3_3        : 6;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_10; // 0x0028

  union
  {
    struct
    {
      unsigned RDLT_RB0_3        : 6;
      unsigned                   : 2;
      unsigned RDLT_RB1_3        : 6;
      unsigned                   : 2;
      unsigned RDLT_RB2_3        : 6;
      unsigned                   : 2;
      unsigned RDLT_RB3_3        : 6;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_11; // 0x002c

  union
  {
    struct
    {
      unsigned OUTLTH_BRI_3         : 8;
      unsigned OUTLTH_DARK_3        : 8;
      unsigned OUTCNT_SEL_3         : 1;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_12; // 0x0030

  union
  {
    struct
    {
      unsigned CLAMP_TH_3           : 8;
      unsigned CLAMPMUL_G_3         : 7;
      unsigned                      : 1;
      unsigned CLAMPMUL_RB_3        : 7;
    } Bit;
    UINT32 Word;
  } IPP_Ext_Register_13; // 0x0034

  
} NT96220_IPP_EXTRN_REG_STRUCT;


#endif
 