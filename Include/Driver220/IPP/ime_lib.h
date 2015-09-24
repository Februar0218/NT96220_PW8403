/**
    Public header file for IME module.

    @file       ime_lib.h
    @ingroup    mIIPPIME
    
    @brief

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/


#ifndef _IME_LIB_H_
#define _IME_LIB_H_

#include "Type.h"

#define STRIEP_CALC_RTOS  1

//------------------------------------------------------------------------------

typedef void (*IMEIntCB)(UINT32);

#define IME_STRIPE_UNIT      16

#define IME_STRIPE_HN_MAX    21
#define IME_STRIPE_HN_MIN    2
#define IME_STRIPE_HL_MAX    21
#define IME_STRIPE_HL_MIN    8  // 4
#define IME_STRIPE_HM_MAX    255
#define IME_STRIPE_HM_MIN    1

#define IME_STRIPE_VN_MAX    4096
#define IME_STRIPE_VN_MIN    5
#define IME_STRIPE_VL_MIN    8  // 3
#define IME_STRIPE_VM_MIN    1
#define IME_H_OVERLAP        16
#define IME_V_OVERLAP        4

//------------------------------------------------------------------------------
//enum data type

typedef enum
{
    IME_CHANNEL_Y   = 0,        ///< Y channel
    IME_CHANNEL_CB  = 1,        ///< CB channel
    IME_CHANNEL_CR  = 2,        ///< CR channel
    ENUM_DUMMY4WORD(_IME_CHANNEL)
} _IME_CHANNEL;


typedef enum
{
    IME_BUFNUM_0    = 0,        ///< using 1 buffer
    IME_BUFNUM_1    = 1,        ///< using 2 ping-pong buffers
    IME_BUFNUM_2    = 2,        ///< using 3 ping-pong buffers
    IME_BUFNUM_3    = 3,        ///< using 4 ping-pong buffers
    IME_BUFNUM_4    = 4,        ///< using 5 ping-pong buffers
    ENUM_DUMMY4WORD(_IME_BUFNUM)
} _IME_BUFNUM;


/*
typedef enum
{
    IMEALG_DISABLE  = 0,        ///< disable function
    IMEALG_ENABLE   = 1,        ///< enable function
    ENUM_DUMMY4WORD(_IME_FUN_EN)
} _IME_FUN_EN;
*/




//------------------------------------------------------------------------------
/** \addtogroup mIIPPIME */
//@{

//define vairable
/**
    @name IME Define - IME Interrupt Enable and Status
*/
//@{
#define IME_INTE_FRM        0x00000004    ///< frame-end enable
#define IME_INTE_V1O16LN    0x00000080    ///< 16-lines enable
#define IME_INTE_LINE_BP1   0x00000100    ///< break-point1 enable
#define IME_INTE_SLICE_DN1  0x00000200    ///< slice-done enable
#define IME_INTE_LINE_BP2   0x00000400    ///< break-point2 enable
#define IME_INTE_ALL        (IME_INTE_FRM | IME_INTE_V1O16LN | IME_INTE_LINE_BP1 | IME_INTE_SLICE_DN1 | IME_INTE_LINE_BP2)   ///< all interrupt enable


#define IME_INT_FRM         0x00000004    ///< frame-end status
#define IME_INT_V1O16LN     0x00000080    ///< 16-lines status
#define IME_INT_LINE_BP1    0x00000100    ///< break-point1 status
#define IME_INT_SLICE_DN1   0x00000200    ///< slice-done status
#define IME_INT_LINE_BP2    0x00000400    ///< break-point2 status
#define IME_INT_ALL         (IME_INT_FRM | IME_INT_V1O16LN | IME_INT_LINE_BP1 | IME_INT_SLICE_DN1 | IME_INT_LINE_BP2)  ///< all interrupt status
//@}



/**
    @name IME enum - Engine Control
*/
//@{
typedef enum
{
  IME_ENGINE_STOP = 0,   ///< Stop engine
  IME_ENGINE_START = 1,  ///< Start engine
  ENUM_DUMMY4WORD(_IME_ENGINE_OP)
} _IME_ENGINE_OP;

typedef enum
{
  IME_RESET_DISABLE = 0,   ///< disable engine soft-reset
  IME_RESET_ENABLE = 1,    ///< enable engine soft-reset
  ENUM_DUMMY4WORD(_IME_RESET_EN)
} _IME_RESET_EN;

typedef enum
{
  IPE_To_IME  = 0,         ///< IPE to IME input port
  DRAM_To_IME = 1,         ///< DMA to IME input port
  ENUM_DUMMY4WORD(_IME_INPUT_SRC_SEL)
} _IME_INPUT_SRC_SEL;

typedef enum
{
  IME_PPB_ID_RESET_DISABLE = 0,   ///< disable ping-pong buffer ID soft-reset
  IME_PPB_ID_RESET_ENABLE = 1,    ///< enable ping-pong buffer ID soft-reset
  ENUM_DUMMY4WORD(_IME_PPB_ID_RESET_EN)
} _IME_PPB_ID_RESET_EN;

typedef enum
{
    IMEALG_IPE2IME = 0,             ///< source IPE
    IMEALG_DMA2IME = 1,             ///< source DRAM
    ENUM_DUMMY4WORD(_IMEALG_SRC)
} _IMEALG_SRC;

typedef enum
{
  IPE2IME_SLICE_MODE   = 0,  ///< Input: IPE to IME mode,  Output: Slice mode
  IPE2IME_FRAME_MODE   = 1,  ///< Input: IPE to IME mode,  Output: Frame mode
  DRAM2IME_SLICE_MODE  = 2,  ///< Input: Dram to IME mode,  Output: Slice mode
  DRAM2IME_FRAME_MODE  = 3,  ///< Input: Dram to IME mode,  Output: Frame mode
  ENUM_DUMMY4WORD(_IME_MODE_SEL)
} _IME_MODE_SEL;
//@}
#define _D2D            DRAM2IME_FRAME_MODE
#define _SLICE_1PATH    IPE2IME_SLICE_MODE


//------------------------------------------------------------------------------
/**
    @name IME enum - Engine Input Control
*/
//@{
typedef enum
{
  IME_STRIPE_AUTO_MODE = 0,    ///< Auto stripe information computation mode
  IME_STRIPE_USER_MODE = 1,    ///< User stripe information computation mode
  ENUM_DUMMY4WORD(_IME_STRIPE_CAL_MODE)
} _IME_STRIPE_CAL_MODE;


typedef enum
{
  IME_INPUT_YCC_444 = 0,         ///< YCC444 format for input
  IME_INPUT_YCC_422 = 1,         ///< YCC422 format for input
  IME_INPUT_YCC_420 = 2,         ///< YCC420 format for input
  IME_INPUT_RGB     = 3,
  ENUM_DUMMY4WORD(_IME_INPUT_IMG_FORMAT_SEL)
} _IME_INPUT_IMG_FORMAT_SEL;

//@}
#define IME_IN_FMT_444     IME_INPUT_YCC_444
#define IME_IN_FMT_422     IME_INPUT_YCC_422
#define IME_IN_FMT_420     IME_INPUT_YCC_420
#define IMEALG_INYUV444    IME_INPUT_YCC_444
#define IMEALG_INYUV422    IME_INPUT_YCC_422
#define IMEALG_INYUV420    IME_INPUT_YCC_420
#define IMEALG_INRGB444    IME_INPUT_RGB

//------------------------------------------------------------------------------
/**
    @name IME enum - Engine Output Control
*/
//@{
typedef enum
{
  IME_OUTPUT_YCC_444     = 0,         ///< YCC444 format for output
  IME_OUTPUT_YCC_422_COS = 1,         ///< YCC422 cosited format for output
  IME_OUTPUT_YCC_422_CEN = 2,         ///< YCC422 centered format for output
  IME_OUTPUT_YCC_420_COS = 3,         ///< YCC420 cosited format for output
  IME_OUTPUT_YCC_420_CEN = 4,         ///< YCC420 centered format for output
  IME_OUTPUT_BAYER       = 5,         ///< Bayer format for output
  IME_OUTPUT_RGB888      = 6,         ///< RGB888 packing format for output
  IME_OUTPUT_RGB444      = 7,         ///< RGB444 format for output
  ENUM_DUMMY4WORD(_IME_OUTPUT_IMG_FORMAT_SEL)
} _IME_OUTPUT_IMG_FORMAT_SEL;

typedef enum
{
  IME_BAYER_RGGB = 0,    ///< Bayer Pattern - RGGB
  IME_BAYER_GRBG = 1,    ///< Bayer Pattern - GRBG
  IME_BAYER_GBRG = 2,    ///< Bayer Pattern - GBRG
  IME_BAYER_BGGR = 3,    ///< Bayer Pattern - BGGR
  ENUM_DUMMY4WORD(_IME_BAYER_SEL)
} _IME_BAYER_SEL;

typedef enum
{
  IMEALG_1PATH  = 1,     ///< output type - YCbCr
  IMEALG_1PATHY = 2,     ///< output type - Only Y
  ENUM_DUMMY4WORD(_IMEALGSETPATH)
} _IMEALGSETPATH;

//#NT#2011/05/26#Linkin Huang -begin
//#NT# modify define value
typedef enum
{
    IMEALG_BILINEAR = 0,        ///< bilinear interpolation
    IMEALG_NEAREST  = 1,        ///< nearest interpolation
    ENUM_DUMMY4WORD(_IMEALGSCALEMODE)
} _IMEALGSCALEMODE;
//#NT#2011/05/26#Linkin Huang -end


typedef enum
{
  IME_SCALEFILTER_DISABLE = 0,   ///< disable scale filtering
  IME_SCALEFILTER_ENABLE = 1,    ///< enable scale filtering
  ENUM_DUMMY4WORD(_IME_SCALEFILTER_EN)
} _IME_SCALEFILTER_EN;


typedef enum
{
  IME_SCALE_FILTER_COEF_AUTO_MODE = 0,    ///< Auto Scale filter coefficeint computation mode
  IME_SCALE_FILTER_COEF_USER_MODE = 1,    ///< User Scale filter coefficeint computation mode
  ENUM_DUMMY4WORD(_IME_SCALE_FILTER_COEF_CAL_MODE)
} _IME_SCALE_FILTER_COEF_CAL_MODE;


typedef enum
{
  IME_OUTPUT_PPB_1 = 0,            ///< using 1 output buffer
  IME_OUTPUT_PPB_2 = 1,            ///< using 2 output ping-pong buffers
  IME_OUTPUT_PPB_3 = 2,            ///< using 3 output ping-pong buffers
  IME_OUTPUT_PPB_4 = 3,            ///< using 4 output ping-pong buffers
  IME_OUTPUT_PPB_5 = 4,            ///< using 5 output ping-pong buffers
  ENUM_DUMMY4WORD(_IME_PPB_NUM_SEL)
} _IME_PPB_NUM_SEL;


typedef enum
{
  SLICE_48_LINES    = 0,         ///< 48 line output slice height
  SLICE_32_LINES    = 1,         ///< 32 line output slice height
  SLICE_64_LINES    = 2,         ///< 64 line output slice height
  SLICE_128_LINES   = 3,         ///< 128 line output slice height
  SLICE_256_LINES   = 4,         ///< 256 line output slice height
  SLICE_512_LINES   = 5,         ///< 512 line output slice height
  SLICE_1024_LINES  = 6,         ///< 1024 line output slice height
  SLICE_2048_LINES  = 7,         ///< 2048 line output slice height
  ENUM_DUMMY4WORD(_IME_SLICE_HEIGHT_SEL)
} _IME_SLICE_HEIGHT_SEL;

//@}
#define IME_OUT_FMT_444             IME_OUTPUT_YCC_444        
#define IME_OUT_FMT_422_COSITED     IME_OUTPUT_YCC_422_COS   
#define IME_OUT_FMT_422_CENTERED    IME_OUTPUT_YCC_422_CEN   
#define IME_OUT_FMT_420_COSITED     IME_OUTPUT_YCC_420_COS   
#define IME_OUT_FMT_420_CENTERED    IME_OUTPUT_YCC_420_CEN 
#define IMEALG_OUTYUV444            IME_OUTPUT_YCC_444
#define IMEALG_OUTYUV422_COS        IME_OUTPUT_YCC_422_COS
#define IMEALG_OUTYUV422_CEN        IME_OUTPUT_YCC_422_CEN
#define IMEALG_OUTYUV420_COS        IME_OUTPUT_YCC_420_COS
#define IMEALG_OUTYUV420_CEN        IME_OUTPUT_YCC_420_CEN
#define IMEALG_BAYER                IME_OUTPUT_BAYER
#define IMEALG_OUTRGB444            IME_OUTPUT_RGB444



//------------------------------------------------------------------------------
/**
    @name IME enum - Color Space Transformation Function
*/
//@{
typedef enum
{
  IME_CST_DISABLE = 0,       ///< disable color space transformation
  IME_CST_ENABLE = 1,        ///< enable color space transformation
  ENUM_DUMMY4WORD(_IME_CST_EN)
} _IME_CST_EN;

typedef enum
{
  IME_CST_RGB2YCC = 0,       ///< RGB to YCC transormation operator
  IME_CST_YCC2RGB = 1,       ///< YCC to RGB transormation operator
  ENUM_DUMMY4WORD(_IME_CST_SEL)
} _IME_CST_SEL;

//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Chroma Filter Function
*/
//@{
typedef enum
{
  IME_HAVG_DISABLE = 0,         ///< disable horizontal average
  IME_HAVG_ENABLE = 1,          ///< enable horizontal average
  ENUM_DUMMY4WORD(_IME_HAVG_EN)
} _IME_HAVG_EN;

typedef enum
{
  IME_AVG_PXLWIDTH_2 = 0,         ///< 2 pixel average width
  IME_AVG_PXLWIDTH_4 = 0,         ///< 4 pixel average width
  IME_AVG_PXLWIDTH_8 = 0,         ///< 8 pixel average width
  IME_AVG_PXLWIDTH_16 = 0,        ///< 16 pixel average width
  ENUM_DUMMY4WORD(_IME_AVG_PXLWIDTH_SEL)
} _IME_AVG_PXLWIDTH_SEL;

typedef enum
{
  IME_HAVG_SPL_DISABLE = 0,         ///< disable spatial center average
  IME_HAVG_SPL_ENABLE = 1,          ///< enable spatial center average
  ENUM_DUMMY4WORD(_IME_HAVG_SPL_EN)
} _IME_HAVG_SPL_EN;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Chroma Curve Mapping Function
*/
//@{
typedef enum
{
  IME_CRV_DISABLE  = 0,         ///< disable curve mapping
  IME_CRV_ENABLE = 1,           ///< enable curve mapping
  ENUM_DUMMY4WORD(_IME_CRV_EN)
} _IME_CRV_EN;

typedef enum
{
  IME_CRV_STEP_1 = 0,           ///< 1 tap
  IME_CRV_STEP_2 = 1,           ///< 2 taps
  IME_CRV_STEP_4 = 2,           ///< 4 taps
  ENUM_DUMMY4WORD(_IME_CRV_STEP_SEL)
} _IME_CRV_STEP_SEL;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Film Grain Function
*/
//@{
typedef enum
{
  IME_FMGRN_DISABLE = 0,         ///< disable film grain
  IME_FMGRN_ENABLE = 1,          ///< enable file grain
  ENUM_DUMMY4WORD(_IME_FMGRN_EN)
} _IME_FMGRN_EN;
//@}



//------------------------------------------------------------------------------
/**
    @name IME enum - Change Funciton Parameter Selection
*/
//@{
typedef enum
{
  IME_CHG_CST           = 0,   ///< color space transformation, please refer to structure IME_CST_INTL_INFO for details
  IME_CHG_CHROMA_FILTER = 1,   ///< chroma filter, please refer to structure IME_CHROMA_FILTER_INFO for details
  IME_CHG_CURVE_MAPPING = 2,   ///< curve mapping, please refer to structure IME_CURVE_MAPPING_INFO for details
  IME_CHG_FILM_GRAIN    = 3,   ///< film grain, please refer to structure IME_FILM_GRAIN_INFO for details
  IME_CHG_IO_SIZE       = 4,   ///< input and output image size, please refer to structure IME_IO_CHG_SIZE for details
  IME_CHG_IO_ADDR       = 5,   ///< input and output DMA address, please refer to structure IME_IO_CHG_ADDR for details
  IME_CHG_SCALE_FILTER  = 6,   ///< scling filter, please refer to structure IME_SCALE_FILTER_INFO for details
  IME_CHG_OUTPUT_FMT    = 7,   ///< output format, please refer to _IME_OUTPUT_IMG_FORMAT_SEL for details
  ENUM_DUMMY4WORD(_IME_CHANGE_FUN_PARAM_SEL)
} _IME_CHANGE_FUN_PARAM_SEL;
//@}

//------------------------------------------------------------------------------
/**
    @name IME enum - Clear Flag
*/
//@{
typedef enum
{
  IME_NO_CLEAR_FLAG = 0,    ///< No clear flag
  IME_CLEAR_FLAG    = 1,    ///< Clear flag
  ENUM_DUMMY4WORD(_IME_CLEAR_FLAG_SEL)
} _IME_CLEAR_FLAG_SEL;
//@}


//------------------------------------------------------------------------------
/**
    @name IME enum - Get Engine Information Selection
*/
//@{
typedef enum
{
  IME_GET_PPB_NUM                     = 0,       ///< get ping-pong buffer numbers
  IME_GET_PPB_OPID                    = 1,       ///< get ping-pong buffer in operation ID
  IME_GET_PPB_RDID                    = 2,       ///< get ping-pong buffer ready ID
  IME_GET_TRISLICE_STATUS             = 3,       ///< get slice trigger status
  IME_GET_INT_ENABLE                  = 4,       ///< get interrupt enable information
  IME_GET_INT_STATUS                  = 5,       ///< get interrupt status
  IME_GET_BUSY_STATUS                 = 6,       ///< get ime busy status
  IME_GET_OUT_LINE_COUNTER            = 7,       ///< get output line counter
  IME_GET_CURVE_EN                    = 8,       ///< get curve mapping enable status
  IME_GET_LOCKED_STATUS               = 9,       ///< get curve mapping enable status
  IME_GET_SLICE_HEIGHT                = 10,      ///< get slice height
  IME_GET_SLICE_EN_STATUS             = 11,      ///< get slice enable status
  IME_GET_STRIPE_MODE                 = 12,      ///< get stripe mode status
  IME_GET_INPUT_SRC                   = 13,      ///< get input source
  IME_GET_OUTPUT_H_SIZE               = 14,      ///< get output horizontal size
  IME_GET_OUTPUT_V_SIZE               = 15,      ///< get output vertical size
  IME_GET_OPERATED_SLICE_NUM          = 16,      ///< get operated slice numbers
  IME_GET_START_STATUS                = 17,      ///< get start status
  IME_GET_SLICE_TRICNT_CALL_STATUS    = 18,      ///< get slice trigger call counter status
  IME_GET_SLICE_TRICNT_STATUS         = 19,      ///< get slice trigger counter status
  IME_GET_SLICE_DONECNT_STATUS        = 20,      ///< get slice done counter status
  ENUM_DUMMY4WORD(_IME_GET_INFO_SEL)
} _IME_GET_INFO_SEL;
//@}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//structure data type

/**
    IME structure - Engine open object
*/
//@{
typedef struct
{
  void (*FP_IMEISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
  UINT32 IME_CLOCKSEL;                      ///< Engine clock selection
} IME_OPENOBJ,*pIME_OPENOBJ;
//@}


/**
    IME structure - Horizontal and vertical stripe setting structure
*/
//@{
typedef struct
{
  UINT32 uHN;  ///< IME input first and middle stripe size for horizontal direction
  UINT32 uHL;  ///< IME input last stripe size for horizontal direction
  UINT32 uHM;  ///< IME input stripe number for horizontal direction
  UINT32 uVN;  ///< IME input first and middle stripe size for vertical direction
  UINT32 uVL;  ///< IME input last stripe size for vertical direction
  UINT32 uVM;  ///< IME input stripe number for vertical direction
  _IME_STRIPE_CAL_MODE StripeCalMode;  ///< IME input stripe information setting mode
} IME_HV_STRIPE_INFO;

typedef struct _STR_IME_STRIPE_INFOR
{
    UINT32    n;        ///< stripe size in each stripe
    UINT32    l;        ///< stripe size of last stripe
    UINT32    m;        ///< stripe number
}STR_IME_STRIPE_INFOR;

//@}


/**
    IME structure - Input image setting structure
*/
//@{
typedef struct
{
  _IME_INPUT_SRC_SEL        SrcSel;  ///< Source of input image
  _IME_INPUT_IMG_FORMAT_SEL FmtSel;  ///< Format of input image
} IME_INPUT_IMAGE_INFO;
//@}

/**
    IME structure - Color space transformation setting structure
*/
//@{
typedef struct
{
  _IME_CST_EN   cstEN;    ///< Color space transformation enable
  _IME_CST_SEL  cstSel;   ///< Color space transformation selection
} IME_CST_INFO;
//@}

/**
    IME structure - Chroma filter parameter setting structure
*/
//@{
typedef struct
{
  _IME_HAVG_EN           HavgEN;        ///< chroma filter enable
  _IME_HAVG_SPL_EN       HavgSPLEN;     ///< chroma filter spatial only mode selection
  _IME_AVG_PXLWIDTH_SEL  HavgPWSel;     ///< chroma filter taps
  UINT32                 HavgCoef;      ///< chroma filter parameters
  UINT32                 HavgTh;        ///< chroma filter threshold
  UINT32                 HavgSPLTap;    ///< chroma filter spatial only taps
} IME_CHROMA_FILTER_INFO;
//@}


/**
    IME structure - Curve Mapping Parameter setting structure
*/
//@{
typedef struct
{
  _IME_CRV_EN        CrvEn;        ///< chroma curve mapping enable
  _IME_CRV_STEP_SEL  StepSel;      ///< chroma curve mapping tap selection
  UINT32             CenterCb;     ///< chroma center for Cb channel
  UINT32             CenterCr;     ///< chroma center for Cr channel
  UINT32             *uCrvMapLUT;  ///< chroma curve mapping LUT
} IME_CURVE_MAPPING_INFO;
//@}

/**
    IME structure - Film grain parameter setting structure
*/
//@{
typedef struct
{
  _IME_FMGRN_EN  fgEn;    ///< film grain enable
  UINT32         fgW0;    ///< film grain weighting 1
  UINT32         fgW1;    ///< film grain weighting 2
  UINT32         fgVal;   ///< film grain initial value
} IME_FILM_GRAIN_INFO;
//@}


/**
    IME structure - Scale filter setting structure
*/
//@{
typedef struct
{
  _IME_SCALEFILTER_EN             hsfEN;           ///< scaling filter enable of horizontal direction
  UINT32                          hFilterCoef;     ///< scaling filter parameters of horizontal direction
  _IME_SCALEFILTER_EN             vsfEN;           ///< scaling filter enable of vertical direction
  UINT32                          vFilterCoef;     ///< scaling filter parameters of vertical direction
  _IME_SCALE_FILTER_COEF_CAL_MODE sfCoefCalMode;   ///< filter parameter mode
} IME_SCALE_FILTER_INFO;
//@}



/**
    IME structure - DMA address setting structure
*/
//@{
typedef struct
{
  UINT32 y_addr;    ///< DMA address of Y channel
  UINT32 cb_addr;   ///< DMA address of Cb channel
  UINT32 cr_addr;   ///< DMA address of Cr channel
} IME_DMA_ADDR_INFO;
//@}

/**
    IME structure - Lineoffset setting structure
*/
//@{
typedef struct
{
  UINT32 LineOffsetY;   ///< line-offset of Y channel
  UINT32 LineOffsetCb;  ///< line-offset of Cb channel
  UINT32 LineOffsetCr;  ///< line-offset of Cr channel
} IME_LINEOFFSET_INFO;
//@}

/**
    IME structure - Input DMA address setting structure
*/
//@{
typedef struct
{
  IME_DMA_ADDR_INFO  iDMA0;  ///< input DMA channel address
} IME_INPUT_DMA_INFO;
//@}

/**
    IME structure - Output DMA address setting structure
*/
//@{
typedef struct
{
  _IME_PPB_NUM_SEL   oPPBNums;   ///< output buffer numbers selection
  IME_DMA_ADDR_INFO  oDMA0;      ///< output DMA channel address of buffer 0
  IME_DMA_ADDR_INFO  oDMA1;      ///< output DMA channel address of buffer 1
  IME_DMA_ADDR_INFO  oDMA2;      ///< output DMA channel address of buffer 2
  IME_DMA_ADDR_INFO  oDMA3;      ///< output DMA channel address of buffer 3
  IME_DMA_ADDR_INFO  oDMA4;      ///< output DMA channel address of buffer 4
} IME_OUTPUT_DMA_INFO;
//@}

//------------------------------------------------------------------------------
// global flow structure

/**
    IME structure - Image size setting structure
*/
//@{
typedef struct
{
  UINT32 h_size;                  ///< horizontal size
  UINT32 v_size;                  ///< vertical size
} IME_SIZE_INFO;
//@}


/**
    IME structure - Engine IQ setting structure
*/
//@{
typedef struct
{
  IME_CST_INFO           ColorSpaceTrans;    ///< color space transformation parameters
  IME_CHROMA_FILTER_INFO ChromaFilterInfo;   ///< chroma filter parameters
  IME_CURVE_MAPPING_INFO CurveMappingInfo;   ///< curve mapping parameters
  IME_FILM_GRAIN_INFO    FileGrainInfo;      ///< film grain parameters
} IME_IQ_FLOW_INFO;
//@}


/**
    IME structure - Input and output format setting structure
*/
//@{
typedef struct _IME_FORMAT_INFO
{
    _IME_INPUT_IMG_FORMAT_SEL   in_format;    ///< input format
    _IME_OUTPUT_IMG_FORMAT_SEL  out1_format;  ///< output path1 format
} IME_FORMAT_INFO;
//@}


/**
    IME structure - Input and output lineoffset setting structure
*/
//@{
typedef struct _IME_LINEOFS_INFO
{
    IME_LINEOFFSET_INFO     in_lineofs;        ///< input line offset
    IME_LINEOFFSET_INFO     p1_out_lineofs;    ///< output lineoffset
}IME_LINEOFS_INFO;
//@}


/**
    IME structure - Engine flow setting structure
*/
//@{
typedef struct
{
  // input info
  _IME_MODE_SEL          op_mode;               ///< IME performing mode
  _IMEALG_SRC            in_src;                ///< IME source
  IME_FORMAT_INFO        format;                ///< input/output format
  IME_SIZE_INFO          in_size;               ///< input image size
  IME_DMA_ADDR_INFO      in_addr;               ///< input DMA address

  //output info
  _IMEALGSETPATH         path_num;              ///< output path selection, YCC or only Y pathes
  IME_SIZE_INFO          p1_out_size;           ///< output image size
  IME_OUTPUT_DMA_INFO    p1_out_addr;           ///< output DMA address
  IME_LINEOFS_INFO       line_ofs;              ///< input/output lineoffset
  _IMEALGSCALEMODE       bil;                   ///< scaling interpolation method
  UINT32                 interrupt_en;          ///< interrupt enable
  UINT32                 breakPoint1;           ///< Break-point 1
  UINT32                 breakPoint2;           ///< Break-point 2

  IME_SCALE_FILTER_INFO  p1_out_scale_filter;   ///< scaling filter parameters
  _IME_BAYER_SEL         p1_out_cfa;            ///< CFA pattern selection
  IME_IQ_FLOW_INFO       *imeIqInfo;            ///< IQ related parameters
}IME_MODE_PRAM,*pIME_MODE_PRAM;
//@}


/**
    IME structure - Change input/output image size structure
*/
//@{
typedef struct
{
  IME_SIZE_INFO           in_size;        ///< input image size
  IME_DMA_ADDR_INFO       in_addr;        ///< input DMA address
  IME_SIZE_INFO           p1_out_size;    ///< output image size
  IME_OUTPUT_DMA_INFO     p1_out_addr;    ///< output DMA address
  IME_LINEOFS_INFO        line_ofs;       ///< input and output lineoffset
} IME_IO_CHG_SIZE;
//@}

/**
    IME structure - Change input/output DMA address structure
*/
//@{
typedef struct
{
  IME_INPUT_DMA_INFO      *in_addr;       ///< input DMA address
  IME_OUTPUT_DMA_INFO     *p1_out_addr;    ///< output DMA address
} IME_IO_CHG_ADDR;
//@}


//@}


//------------------------------------------------------------------------------
extern ER ime_triggerSlice(void);
extern void ime_resetPPBID_P1(_IME_PPB_ID_RESET_EN op);
extern void ime_setInitPPBOperatingID_P1(UINT32 uVal);


extern UINT32 ime_waitDone(void);
extern void ime_waitFlagFrameEnd(_IME_CLEAR_FLAG_SEL isClrFlag);
extern ER ime_clearFlagFrameEnd(void);
extern void ime_waitFlagBP1(_IME_CLEAR_FLAG_SEL isClear);
extern void ime_clrFlagBP1(void);
extern void ime_waitFlagBP2(_IME_CLEAR_FLAG_SEL isClear);
extern void ime_clrFlagBP2(void);
extern void ime_waitFalgSliceDone(_IME_CLEAR_FLAG_SEL isClear);
extern void ime_clrFlagSliceDone(void);
extern void ime_clrInterruptStatus(UINT32 uiInt);
extern ER ime_clrIntFlag(void);

//#NT#2011/05/31#Linkin Huang -begin
//#NT# remove
//extern void ime_StatustClear(UINT32 int_status_bits);
//#NT#2011/05/31#Linkin Huang -end


extern ER ime_setStart(void);
extern ER ime_setReset(void);
extern BOOL ime_isOpen(void);
extern ER ime_open(IME_OPENOBJ *PIME_OpenInfo);
extern ER ime_setMode(IME_MODE_PRAM *EngineInfo);
extern ER ime_pause(void);
extern ER ime_close(void);
extern ER ime_chgParam(void *ptr, _IME_CHANGE_FUN_PARAM_SEL FunSel);


// APIs for getting ime engine information
extern UINT32 ime_getInterruptStatus(void);
extern UINT32 ime_getSliceHeight(void);
extern ER ime_getInputDMAAddrInfo(IME_DMA_ADDR_INFO *inAddr);
extern ER ime_getInputLineoffsetInfo(IME_LINEOFFSET_INFO *inLineoffset);
extern ER ime_getOutputDMAAddrInfo_P1(IME_DMA_ADDR_INFO *outAddr, _IME_PPB_NUM_SEL ppbID);
extern ER ime_getOutputLineoffsetInfo_P1(IME_LINEOFFSET_INFO *outLineoffset);

extern UINT32 ime_getOutputPath1Add(_IME_BUFNUM Bufnum, _IME_CHANNEL Channel);
extern UINT32 ime_getActOutputPath1Add(_IME_CHANNEL Channel);
extern UINT8 ime_getPath1CurrentBufferIndex(void);

//extern UINT32 ime_getOutputPath2Add(_IME_BUFNUM Bufnum, _IME_CHANNEL Channel);
//extern UINT32 ime_getActOutputPath2Add(_IME_CHANNEL Channel);
//extern UINT8 ime_getPath2CurrentBufferIndex(void);

//extern void ime_getHVStripeInfo(IME_HV_STRIPE_INFO *hvStripeInfo);
extern STR_IME_STRIPE_INFOR ime_getHstripe(void);
extern STR_IME_STRIPE_INFOR ime_getVstripe(void);
extern void ime_setUserStripeInfo(IME_HV_STRIPE_INFO *uiStripeInfo);
extern void ime_setSliceHeight(_IME_SLICE_HEIGHT_SEL uiSliceHeight);

extern UINT32 ime_getEngineInfo(_IME_GET_INFO_SEL sel);


#endif
