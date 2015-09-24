/**
    Public header file for PRE module.

    @file       pre_lib.h
    @ingroup    mIIPPPRE

    @brief

    How to start:\n
    [Preview]\n
    1. pre_attach\n
    2. pre_open\n
    3. PRE_Operation = PRE_ENGINE_SET2PRV, pre_setMode\n
    4. PRE_Operation = PRE_ENGINE_SET2RUN, pre_setMode\n

    [Preview to Capture]\n
    1. PRE_operation = PRE_ENGINE_SET2PAUSE, pre_setMode\n
    2. PRE_Operation = PRE_ENGINE_SET2CAP, pre_setMode\n
    3. PRE_Operation = PRE_ENGINE_SET2RUN, pre_setMode\n
    4. PRE_operation = PRE_ENGINE_SET2PAUSE, pre_setMode\n


    How to stop:\n
    1. PRE_operation = PRE_ENGINE_SET2PAUSE, pre_setMode\n
    2. pre_close\n
    3. pre_detach\n


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _PRE_LIB_H
#define _PRE_LIB_H

#include "Type.h"
#include "sie_lib.h"

#define PRE_DEBUG   0
#define PRE_DEBUG_INFO  0

#define PRE_BLOCK_BUF_SIZE   (24 * 4) //byte

/** \addtogroup mIIPPPRE */
//@{
/*
#define pre_Load                    pre_load
#define pre_CheckBusy               pre_checkBusy
#define pre_SetVcropSize            pre_setVcropSize
#define pre_SetHcropSize            pre_setHcropSize
#define pre_SetOutputSize           pre_setOutputSize
#define pre_SetOutputLineOffset     pre_setOutputLineOffset
#define pre_GetCropHIni             pre_getCropHIni
#define pre_GetCropVIni             pre_getCropVIni
#define pre_ReadCurrentInBufferID   pre_readCurrentInBufferID
#define pre_ReadCurrentOutBufferID  pre_readCurrentOutBufferID
#define pre_GetSaoAdd               pre_getSaoAdd
#define pre_SetBreakPoint           pre_setBreakPoint
#define pre_ClrIntrStatus           pre_clrIntrStatus
#define pre_SetHScaling             pre_setHScaling
#define pre_SetHBining              pre_setHBining
#define pre_SetColorInfo            pre_setColorInfo
#define pre_SetCfa                  pre_setCfa
#define pre_SetGeoPosition          pre_setGeoPosition //for compatibility
#define pre_SetGeoParamNormGain     pre_setGeoParamNormGain //for compatibility
#define pre_SetGeoParamDist         pre_setGeoParamDist
#define pre_SetGeoParamCorrGain     pre_setGeoParamCorrGain
#define pre_SetGeo                  pre_setGeo
#define pre_GetGeoCorrTerm          pre_getGeoCorrTerm
*/

/*
    Define PRE function enable.
*/
//@{
#define PRE_SRC_SEL                         0x00000003
#define PRE_LUT_CTRL                        0x00000004
#define PRE_LINE_BLM_EN                     0x00000010  ///< enable function: vertical defect line compensation
#define PRE_HDR_EN                          0x00000020  ///< enable function: HDR
#define PRE_BLM_EN                          0x00000100  ///< enable function: defect pixel compensation
#define PRE_DF_EMBED_EN                     0x00000200  ///< enable function: embedded defect pixel
#define PRE_HSCL_EN                         0x00000400  ///< enable function: horizontal downsampling
#define PRE_CG_EN                           0x00000800  ///< enable function: color gain
#define PRE_GAINCTRL_EN                     0x00001000  ///< enable function: gain control
#define PRE_GAMMA_EN                        0x00002000  ///< enable function: gamma
#define PRE_GEO_EN                          0x00004000  //for compatibility
#define PRE_VACC_EN                         0x00008000  //for compatibility
#define PRE_HINTLV_EN                       0x00010000  ///< enable function: input horizontal interleaved
#define PRE_HINTLV_SEL                      0x00020000  //< 0: two-line interleaved ; 1: three-line interleaved
#define PRE_PADDING_EN                      0x00040000  ///< enable function: horizontal padding
#define PRE_HFLIP_EN                        0x00200000  ///< enable function: horizontal flip
#define PRE_VFLIP_EN                        0x00400000  ///< enable function: vertical flip
#define PRE_VIG_EN                          0x00800000  ///< enable function: vigette correction (lens shading compensation)
#define PRE_CA_EN                           0x01000000  ///< enable function: color accumulation
#define PRE_HINTLV_OUTEN                    0x02000000  ///< enable function: output horizontal interleaved
//#define PRE_HDR_PAD                         0x04000000  //bit 26
#define PRE_DF_SUB_EN                       0x08000000  ///< enable function: dark frame subtraction
//#define PRE_DF_SUB_OP_SEL                   0x30000000  //bit 28:29
//#define PRE_VIG_OP                          0x40000000  //bit 30
#define PRE_DP_CNTRST_EN                    0x80000000  ///< enable function: reset defect pixel counter
//@}

/*
    Define PRE interrput enable.
*/
//@{
#define PRE_INTE_FLDEND                     0x00000004  ///< enable interrupt: field end
#define PRE_INTE_LNEDN                      0x00000008  ///< enable interrupt: end of each line
#define PRE_INTE_LN16END                    0x00000010  ///< enable interrupt: end of 16 lines
#define PRE_INTE_BP1                        0x00000020  ///< enable interrupt: break point 1
#define PRE_INTE_OVFL1                      0x00000080  ///< enable interrupt: input ping-pong buffer overflow
#define PRE_INTE_CA_OVFL                    0x00000100  ///< enable interrupt: color accumulation overflow
//#define PRE_INTE_GAMMAERR                   0x00000200  //bit 9
#define PRE_INTE_DPCF                       0x00001000  ///< enable interrupt: defect pixel compensation failed
#define PRE_INTE_DRAMI_END                  0x00002000  ///< enable interrupt: end of dram input
#define PRE_INTE_CA_END                     0x00004000  ///< enable interrupt: end of color accumulation
#define PRE_INTE_VA                         0x00008000  //for compatibility
#define PRE_INTE_CROPST                     0x00010000  ///< enable interrupt: crop start
#define PRE_INTE_CROPEND                    0x00020000  ///< enable interrupt: crop end
#define PRE_INTE_DRAMI2_FAIL                0x00080000  ///< enable interrupt: dram input 2 failed
#define PRE_INTE_FRMEND                     0x00100000  ///< enable interrupt: frame end
#define PRE_INTE_OVFL2                      0x00200000  ///< enable interrupt: output dram buffer overlfow
#define PRE_INTE_VAERR                      0x00400000  //for compatibility
#define PRE_INTE_LINEERR                    0x00800000  //for compatibility
#define PRE_INTE_ALL                        0x003fffff  ///< enable interrupt: Enable all interrupts
//@}

/*
    Define PRE interrput status.
*/
//@{
#define PRE_INT_FLDEND                      0x00000004  ///< interrupt status: field end
#define PRE_INT_LNEDN                       0x00000008  ///< interrupt status: end of each line
#define PRE_INT_LN16END                     0x00000010  ///< interrupt status: end of 16 lines
#define PRE_INT_BP1                         0x00000020  ///< interrupt status: break point 1
#define PRE_INT_OVFL1                       0x00000080  ///< interrupt status: input ping-pong buffer overflow
#define PRE_INT_CA_OVFL                     0x00000100  ///< interrupt status: color accumulation overflow
//#define PRE_INT_GAMMAERR                    0x00000200  //bit 9
#define PRE_INT_DPCF                        0x00001000  ///< interrupt status: defect pixel compensation failed
#define PRE_INT_DRAMI_END                   0x00002000  ///< interrupt status: end of dram input
#define PRE_INT_CA_END                      0x00004000  ///< interrupt status: end of color accumulation
#define PRE_INT_VA                          0x00008000  //for compatibility
#define PRE_INT_CROPST                      0x00010000  ///< interrupt status: crop start
#define PRE_INT_CROPEND                     0x00020000  ///< interrupt status: crop end
#define PRE_INT_DRAMI2_FAIL                 0x00080000  ///< interrupt status: dram input 2 failed
#define PRE_INT_FRMEND                      0x00100000  ///< interrupt status: frame end
#define PRE_INT_OVFL2                       0x00200000  ///< interrupt status: output dram buffer overlfow
#define PRE_INT_VAERR                       0x00400000  ///for compatibility
#define PRE_INT_LINEERR                     0x00800000  //for compatibility
#define PRE_INT_GEO_ERR                     0x01000000  //for compatibility
#define PRE_INT_ALL                         0x003fffff  ///< interrupt status: all interrupts
//@}

/**
    PRE Engine Operation Selection.

    Select PRE module operation.
*/
//@{
typedef enum
{
    PRE_ENGINE_OPEN = 0, ///< open module
    PRE_ENGINE_CLOSE = 1, ///< close module
    PRE_ENGINE_SET2READY = 2, ///< set module to ready state
    PRE_ENGINE_SET2PRV = 3, ///< set module to preview mode
    PRE_ENGINE_SET2CAP = 4, ///< set module to capture mode
    PRE_ENGINE_SET2PAUSE = 5, ///< pause module
    PRE_ENGINE_SET2RUN = 6, ///< set module to run
    PRE_ENGINE_SET2D2D = 7, ///< set module to Dram to Dram mode
    PRE_ENGINE_ATTACH = 8, ///< attach module (enable clock)
    PRE_ENGINE_DETACH = 9, ///< detach module (disable clock)
    ENUM_DUMMY4WORD(PRE_ENGINE_OPERATION)
} PRE_ENGINE_OPERATION;
//@}

/*
typedef enum
{
    PRE_FUN_DISABLE = 0,
    PRE_FUN_ENABLE = 1
}PRE_FUN_EN;
*/

/**
    PRE Gamma LUT Selection.

    Select PRE LUT type.
*/
//@{
typedef enum
{
    PRE_LUT257X1 = 0, ///< one curve with 257 taps
    PRE_LUT65X4 = 1, ///< four curve with 65 taps
    PRE_LUT289X1 = 2, //for compatibility
    ENUM_DUMMY4WORD(PRE_GAMMA_LUT_CTRL)
}PRE_GAMMA_LUT_CTRL;
//@}

/**
    PRE HDR Operation Selection.

    Select PRE HDR operation type.
*/
//@{
typedef enum
{
    PRE_HDROP_1CH = 0, ///< one channel
    PRE_HDROP_1CHPAD = 1, ///< one channel with padding
    PRE_HDROP_2CH = 2, ///< two channels with gain map
    ENUM_DUMMY4WORD(PRE_HDR_OPT)
}PRE_HDR_OPT;
//@}

/**
    PRE Vignette Operation Selection.

    Select PRE vignette operation (lens shading compensation) type.
*/
//@{
typedef enum
{
    PRE_VIG_ORIGIN = 0, ///< original
    PRE_VIG_SMOOTH = 1, ///< smooth
    ENUM_DUMMY4WORD(PRE_VIG_OPT)
}PRE_VIG_OPT;
//@}

/**
    PRE Ping-Pong Buffer Height Selection.

    Select PRE buffer height of the PPBs between SIE and PRE.
*/
//@{
typedef enum
{
    PPB_8LINES = 0, ///< 8 lines
    PPB_16LINES = 1, ///< 16 lines
    PPB_32LINES = 2, ///< 32 lines
    PPB_64LINES = 3, ///< 64 lines
    PPB_96LINES = 4, ///< 96 lines
    PPB_128LINES = 5, ///< 128 lines
    PPB_192LINES = 6, ///< 192 lines
    PPB_FULL_HEIGHT = 7, ///< full frame
    ENUM_DUMMY4WORD(PRE_PPB_HEIGHT)
}PRE_PPB_HEIGHT; //PRE_PPB_HeightEnum;
//@}

/**
    PRE Input Packing Selection.

    Select PRE input packing mode.
*/
//@{
typedef enum
{
    PRE_INPACK_12BIT = 0, ///< 12 bits data input
    PRE_INNOT_USED1 = 1, ///< reserved
    PRE_INPACK_8BIT = 2, ///< 8 bits data input
    PRE_INPACK_16BIT = 3, ///< 16 bits data input
    ENUM_DUMMY4WORD(PRE_InPackBitSelEnum)
}PRE_InPackBitSelEnum;
//@}

/**
    PRE Output Packing Selection.

    Select PRE output packing mode.
*/
//@{
typedef enum
{
    PRE_OUTPACK_12BIT = 0, ///< 12 bits data output
    PRE_OUTPACK_10BIT = 1, ///< 10 bits data output
    PRE_OUTPACK_8BIT = 2, ///< 8 bits data output
    PRE_OUTPACK_16BIT = 3, ///< 16 bits data output
    ENUM_DUMMY4WORD(PRE_OutPackBitSelEnum)
}PRE_OutPackBitSelEnum;
//@}

/**
    PRE Input Source Selection.

    Select PRE input source.
*/
//@{
typedef enum
{
    PATH_DRAM_INPUT = 0, ///< single frame input from DRAM
    PATH_SIE_DIRECT_INPUT = 1, ///< direct input from SIE
    PATH_SIE_PINGPONG_INPUT = 2, ///< DRAM ping-pong buffers with SIE
    ENUM_DUMMY4WORD(PRE_InputSourceSelEnum)
}PRE_InputSourceSelEnum;
//@}

/**
    PRE Input Source Selection.

    Select PRE input source.
*/
//@{
typedef enum
{
    PRE_DRAM_INPUT = 0, ///< single frame input from DRAM
    PRE_SIE_DIRECT_INPUT = 1, ///< direct input from SIE
    PRE_SIE_PPB_INPUT = 2, ///< DRAM ping-pong buffers with SIE
    ENUM_DUMMY4WORD(PRE_SRC_SELECT)
}PRE_SRC_SELECT;
//@}

/**
    PRE Path of Output Source Selection.

    Select PRE path of output source.
*/
//@{
//KH mark
#if 0
typedef enum
{
    PATH_PRE_CLOLRGAIN = 0, ///< output before color gain
    PATH_PRE_GAMMA = 1, ///< output before gamma
    PATH_POST_GAMMA = 2, ///< output after gamma
    ENUM_DUMMY4WORD(PRE_OutputPathEnum)
}PRE_OutputPathEnum;
#endif
//@}

/**
    PRE VGA Format Selection.

    Select PRE VGA format.
*/
//@{
typedef enum
{
    SENSOR_OUT_PROGRESSIVE = 0, ///< progressive data
    SENSOR_OUT_TWO_LINE = 1, ///< two-line interleaved data
    SENSOR_OUT_THREE_LINE = 2, ///< three-line interleaved data
    ENUM_DUMMY4WORD(PRE_VGAFormatEnum)
} PRE_VGAFormatEnum;
//@}

/**
    PRE CFA Pattern Selection.

    Select start pixel of PRE CFA pattern.
*/
//@{
typedef enum
{
    PRECFA_R = 0, ///< start pixel is R
    PRECFA_Gr = 1, ///< start pixel is GR
    PRECFA_Gb = 2, ///< start pixel is GB
    PRECFA_B = 3, ///< start pixel is B
    ENUM_DUMMY4WORD(PRECFA)
}PRECFA;
//@}

/**
    PRE Subsample Ratio Selection.

    Select PRE subsample ratio.
*/
//@{
typedef enum
{
    PRE_SUB_1_2 = 0, ///< subsample to 1/2
    PRE_SUB_1_4 = 1, ///< subsample to 1/4
    PRE_SUB_1_8 = 2, ///< subsample to 1/8
    ENUM_DUMMY4WORD(PRE_SUBRATIO)
} PRE_SUBRATIO;
//@}

/**
    PRE Horizontal Pixel Adding Selection.

    Select PRE horizontal pixel addition.
*/
//@{
//KH mark
#if 0
typedef enum
{
    HSCL_ADD_1_PIXEL = 0, ///< by itself
    HSCL_ADD_2_PIXEL = 1, ///< addition of two pixels
    ENUM_DUMMY4WORD(PRE_ADD_PIXEL)
} PRE_ADD_PIXEL;
#endif
//@}

/**
    PRE Horizontal Pixel Division Selection.

    Select PRE horizontal pixel division.
*/
//@{
//KH mark
#if 0
typedef enum
{
    HSCL_DIV_1_PIXEL = 0, ///< divided by one
    HSCL_DIV_2_PIXEL = 1, ///< divided by two
    ENUM_DUMMY4WORD(PRE_DIV_PIXEL)
} PRE_DIV_PIXEL;
#endif
//@}

/**
    PRE DFS Operation Selection.

    Select PRE dark frame subtraction operation.
*/
//@{
//KH mark
#if 0
typedef enum
{
    DFS_A_MINUS_B = 0,///< A - B
    DFS_B_MINUS_A = 1, ///< B - A
    DFS_A_PLUS_B = 2, ///< A + B
    DFS_A_MUL_B_SHIFT = 3, ///< (A * B) >> shift
    DFS_A_MUL_B_SHIFT_P2P = 4, ///< (A * B) >> shift, point on point
    DFS_ABS_A_B = 5, ///< |A - B|
    ENUM_DUMMY4WORD(PRE_DFS_OP)
} PRE_DFS_OP;
#endif
//@}

/**
    PRE Open Object.

    Open Object is used for opening PRE module.
    \n Used for pre_open()
*/
//@{
typedef struct _PRE_OPENOBJ{
    void (*FP_PREISR_CB)(UINT32 intstatus); ///< isr callback function
} PRE_OPENOBJ;
//@}

/**
    PRE Input Size.

    Structure of input horizontal/vertical sizes and line offset.
*/
//@{
typedef struct _STR_PREIN_SIZE{
    UINT16    Hsize; ///< input horizontal size
    UINT16    Vsize; ///< input vertical size
    UINT16    lineoft; ///< input line offset
} STR_PREIN_SIZE;
//@}

/**
    PRE Color Gain and Offset Information.

    Structure of color gain and offset information.
*/
//@{
typedef struct _STR_PRE_CG_INFOR{
    UINT16 OfsR; ///< color offset of R
    UINT16 OfsGr; ///< color offset of GR
    UINT16 OfsGb; ///< color offset of GB
    UINT16 OfsB; ///< color offset of B
    UINT16 GainR; ///< color gain of R
    UINT16 GainGr; ///< color offset of GR
    UINT16 GainGb; ///< color offset of GB
    UINT16 GainB; ///< color offset of B
    UINT16 GainSel; ///< gain selection
}STR_PRE_CG_INFOR;
//@}

/**
    PRE Vignette Correction Information.

    Structure of vignette correction (lens shading compensation) information.
*/
//@{
typedef struct _STR_PRE_VIG_INFOR{
    UINT16 VIG_XR; ///< x center of R
    UINT16 VIG_YR; ///< y center of R
    UINT16 VIG_XGR; ///< x center of GR
    UINT16 VIG_YGR; ///< y center of GR
    UINT16 VIG_XGB; ///< x center of GB
    UINT16 VIG_YGB; ///< y center of GB
    UINT16 VIG_XB; ///< x center of B
    UINT16 VIG_YB; ///< y center of B
    UINT8 VIG_MUL; ///< vignette distance gain. unsigned 2.6
    UINT16 VIG_XDIV; ///< vignette scaling factor in x direction. unsigned 0.6
    UINT16 VIG_YDIV; ///< vignette scaling factor in y direction. unsigned 0.6
    UINT8 VIG_YDIST; ///< vignette Y subsampling factor
    UINT8 VIG_T; ///< vignette disabled area
    UINT8 *VIG_R_LUT; ///< vignette LUT of R channel
    UINT8 *VIG_G_LUT; ///< vignette LUT of G channel
    UINT8 *VIG_B_LUT; ///< vignette LUT of B channel
}STR_PRE_VIG_INFOR;
//@}

/**
    PRE Linear Shading Compensation Information.

    Structure of linear shading compensation information.
*/
//@{
typedef struct _STR_PRE_VIGLINE_INFOR{
    UINT8 Xcf; ///< x coefficient of the line
    UINT8 Ycf; ///< y coefficient of the line
    UINT8 Scale; ///< scaling factor of the x and y coefficients of the line
    UINT16 Dist_T; ///< distance of disabled compensation
    UINT16 XSub; ///< set to 0 if XInv == 0; set to image width if XInv == 1
    UINT8 XInv; ///< Inversion of x direction
    UINT8 *VIG_R2_LUT; ///< linear shading LUT of R channel
    UINT8 *VIG_G2_LUT; ///< linear shading LUT of G channel
    UINT8 *VIG_B2_LUT; ///< linear shading LUT of B channel
}STR_PRE_VIGLINE_INFOR;
//@}

/**
    PRE DFS Information.

    Structure of dark frame subtraction information.
*/
//@{
//KH mark
#if 0
typedef struct _STR_PRE_DFS_INFOR{
    //UINT32 DFS_OP;
    PRE_DFS_OP DFS_OP; ///< DFS operation
    UINT32 DFS_TH; ///< DFS threshold. if pixel value > DFS_TH, it is a defect pixel
    UINT32 DFS_THSRC; ///< source image of the defect pixels. 0: A, data from SAI2; 1: B, data from SIE or SAI0/SAI1
}STR_PRE_DFS_INFOR;
#endif
//@}

/**
    PRE Gain Control Information.

    Structure of gain control information.
*/
//@{
typedef struct _STR_PRE_GAINCTRL_INFOR{
    UINT32 MapA; ///< A point on map
    UINT32 MapB; ///< B point on map
    UINT16 GainA; ///< gain A
    UINT16 GainB; ///< gain B
    UINT16 Switch; ///< choose one of the two channels
    UINT16 OffsetA; ///< offset A
    UINT16 OffsetB; ///< offset B
}STR_PRE_GAINCTRL_INFOR;
//@}

/**
    PRE Color Accumulation Information.

    Structure of color accumulation with subsampled dram output data.

*/
//@{
typedef struct _STR_PRE_CA_INFOR{
    UINT32 DRAM_SUBSAO; ///< dram output starting address
    UINT32 DRAM_SUBOFSO; ///< dram output line offset
    UINT32 HSUB; ///< horizontal subsampling rate
    UINT32 VSUB; ///< vertical subsampling rate
    UINT32 COLOR_PATH; ///< path of output source
} STR_PRE_CA_INFOR, *PSTR_PRE_CA_INFOR;
//@}

/* //Duplicate with STR_PRE_CA_INFOR
typedef struct {
    UINT32 outAddr;
    UINT32 ofso;
    PRE_SUBRATIO hSub;
    PRE_SUBRATIO vSub;
    PRE_OutputPathEnum path;
} PRE_SUBOUT_REG;
*/

/**
    PRE HDR Information.

    Structure of parameters of high dynamic range image processing.

*/
//@{
typedef struct _STR_PRE_HDR_INFOR{
    UINT32 pre_input2_dram_addr; ///< dram input address of reference layer
    UINT32 pre_input2_lineoft; ///< dram input line offset of reference layer
    UINT32 pre_map_Hsize; ///< horizontal size of reference layer
    UINT32 pre_map_Vsize; ///< vertical size of reference layer
    UINT32 pre_map_Hscale; ///< horizontal scaling factor of reference layer
    UINT32 pre_map_Vscale; ///< vertical scaling factor of reference layer
    //UINT32 pre_map_shift;
    UINT32 *pre_hdr_gammaLUT; ///< HDR 2D LUT 17x17
} STR_PRE_HDR_INFOR, *PSTR_PRE_HDR_INFOR;
//@}

/**
    PRE Division Route Information.

    Structure of solving two cycled division formats.
*/
//@{
typedef struct _PRE_DIVROUTE_INFOR{
    UINT8 PRE_DivRoute[10]; ///< division number of each division in both cycles
    BOOL PRE_HDIV_2CYCLE_EN; ///< 0: disabled; 1: two cycles
} PRE_DIVROUTE_INFOR;
//@}

typedef struct _STR_PRE_GEO_INFOR{ //for compatibility
    UINT32 GEO_CENTX;
    UINT32 GEO_CENTY;
    UINT32 GEO_CENTXS;
    UINT32 GEO_CENTYS;
    UINT32 GEO_NORMFACT;
    UINT32 GEO_NORMBIT;
    UINT32 GEO_RLUTGAIN;
    UINT32 GEO_GLUTGAIN;
    UINT32 GEO_BLUTGAIN;
    UINT32 GEO_DISTSEL;
    UINT32 GEO_XDIST;
    UINT32 GEO_YDIST;
    UINT32 GEO_CORREN;
    UINT32 GEO_CORRNORM;
    UINT32 GEO_RCORRGAIN;
    UINT32 GEO_GRCORRGAIN;
    UINT32 GEO_GBCORRGAIN;
    UINT32 GEO_BCORRGAIN;
    UINT32 *GEO_Lut;
}STR_PRE_GEO_INFOR;

typedef struct _STR_PRE_GEO_IQ_INFOR{  //for compatibility

    UINT32 GEO_RLUTGAIN;
    UINT32 GEO_GLUTGAIN;
    UINT32 GEO_BLUTGAIN;
    UINT32 GEO_CORREN;
    UINT32 GEO_CORRNORM;
    UINT32 GEO_RCORRGAIN;
    UINT32 GEO_GRCORRGAIN;
    UINT32 GEO_GBCORRGAIN;
    UINT32 GEO_BCORRGAIN;
} STR_PRE_GEO_IQ_INFOR;

typedef enum //for compatibility
{
    PRE_2DVA_AUTO_UPDATED = 0,
    PRE_2DVA_MAN_UPDATED = 1
} PRE_VARACC_MODE;

typedef struct _STR_PRE_VA_INFOR{  //for compatibility
    UINT32    stx;
    UINT32    sty;
    PRE_VARACC_MODE  vacc_mode;
    UINT32    vnf;
    UINT32    hnf;
    UINT32    hsz;
    UINT32    vsz;
    UINT32    va_th;
}STR_PRE_VA_INFOR;

typedef struct _STR_PRE_VA_VALUE{  //for compatibility
    UINT16 valH;
    UINT16 valV;
}STR_PRE_VA_VALUE;

typedef struct _STR_PRE_VA_NUM{  //for compatibility
    UINT16 numH;
    UINT16 numV;
}STR_PRE_VA_NUM;

/**
    PRE Entire Configuration.

    Structure of entire PRE configuration.
*/
//@{
//KH mark
#if 0
typedef struct _STR_PRE_REG{
    PRE_ENGINE_OPERATION            PRE_Operation; ///< PRE engine operation
    UINT32                          PRE_EnFunction; ///< functions to be enabled
    PRE_VGAFormatEnum               PRE_VGAType; ///< input data format
    PRE_PPB_HEIGHT                  REG_ppb_buf_height; //ping-pong buffer height between SIE and PRE
    PRE_InPackBitSelEnum            REG_pre_input_pack_bus; ///< input packing mode
    PRE_OutPackBitSelEnum           REG_pre_output_pack_bus; ///< output packing mode
    PRE_InputSourceSelEnum          REG_pre_input_source; ///< input source
    PRE_OutputPathEnum              REG_pre_output_path; ///< path of dram output data
    STR_FLDNUM                      REG_RemapFieldNum; ///< field numbers, for capture mode
    //STR_DIV                         REG_hd_DivisionSeq;
    UINT32                          REG_hd_DivisionNum; ///< total division number
    STR_PREIN_SIZE                  REG_pre_input_Size; ///< PRE input width, height and line offset
    UINT32                          REG_pre_vv_ini; ///< vertical crop start point
    UINT32                          REG_pre_vv_height; ///< vertical crop height
    UINT32                          REG_pre_hv_ini; ///< horizontal crop start point
    UINT32                          REG_pre_hv_width; ///< horizontal crop width
    UINT32                          REG_pre_in_ppb0_addr; ///< input ppb starting address 0
    UINT32                          REG_pre_in_ppb1_addr; ///< input ppb starting address 1
    UINT32                          REG_pre_in_ppb2_addr; ///< input ppb starting address 2
    UINT32                          REG_pre_input_lineoft; ///< dram input line offset
    UINT32                          REG_pre_out_ppb0_addr; ///< output ppb starting address 0
    UINT32                          REG_pre_out_ppb1_addr; ///< output ppb starting address 1
    UINT32                          REG_pre_output_size; ///< output horizontal size
    UINT32                          REG_pre_output_lineoft; ///< dram output line offset
    UINT32                          REG_pre_intrp_en; ///< interrpts to be enabled
    UINT32                          REG_pre_bp1; ///< break point 1
    UINT32                          REG_pre_CfaPat; ///< CFA pattern 1 (for vignette correction)
    UINT32                          REG_pre_CfaPat2; ///< CFA pattern 2 (for color gain/offset, gamma and ECS/EDP/DFS)
    STR_PRE_GAINCTRL_INFOR          pre_gainctrl_A; ///< gain control A
    STR_PRE_GAINCTRL_INFOR          pre_gainctrl_B; ///< gain control B
    STR_PRE_CG_INFOR                pre_cg_ofs; ///< color gain and offsets
    STR_PRE_VIG_INFOR               pre_vig_setting; ///< setting of vignette correction (lens shading compensation)
    STR_PRE_VIGLINE_INFOR           pre_vigline_setting; ///< setting of linear shading compensation
    PRE_VIG_OPT                     pre_vigOp; ///< vignette correction operation
    STR_PRE_GEO_INFOR               pre_geo_setting; //for compatibility
    STR_PRE_DFS_INFOR               pre_dfs_setting; ///< dark frame subtraction setting
    STR_PRE_CA_INFOR                pre_ca_setting; ///< color accumulation (subsampled dram output)
    STR_PRE_HDR_INFOR               pre_hdr_setting; ///< HDR setting
    PRE_HDR_OPT                     pre_hdrOp; ///< HDR option
    PRE_GAMMA_LUT_CTRL              REG_pre_lut_ctrl; ///< Gamma table type
    UINT32                          *pre_gammaLUT; ///< Gamma LUT of 257 taps
    STR_PRE_VA_INFOR                *VaccIn; //for compatibility
    UINT32                          *pre_gammaLUT_R; ///< Gamma LUT of 65 taps for R channel
    UINT32                          *pre_gammaLUT_GR; ///< Gamma LUT of 65 taps for GR channel
    UINT32                          *pre_gammaLUT_GB; ///< Gamma LUT of 65 taps for GB channel
    UINT32                          *pre_gammaLUT_B; ///< Gamma LUT of 65 taps for B channel
    UINT32                          *pre_line_blm_pos; ///< vertical defect line compensation (16 lines)
}STR_PRE_REG;
#endif
//@}
//KH mark
#if 0
extern ER pre_open(PRE_OPENOBJ *pObjCB);
extern ER pre_close(void);
extern ER pre_pause(void);
extern ER pre_start(void);

extern ER pre_setMode(STR_PRE_REG *PRE_PARAM);

extern ER pre_lock(void);
extern ER pre_unlock(void);
extern void pre_attach(void);
extern void pre_detach(void);
extern void pre_load(void);
extern void pre_isr(void);
extern BOOL pre_checkBusy(void);
extern void pre_clrFieldEnd(void);
extern void pre_waitFieldEnd(BOOL bClrFlag);
extern void pre_clrFrameEnd(void);
extern void pre_waitFrameEnd(BOOL bClrFlag);
#endif

//Input & Output
extern void pre_setVcropSize(UINT32 vv_ini, UINT32 vv_height);
extern void pre_setHcropSize(UINT32 hv_ini, UINT32 hv_width);
extern UINT32 pre_getCropHIni(void);
extern UINT32 pre_getCropVIni(void);
extern UINT32 pre_readCurrentInBufferID(void);
extern void pre_setOutputSize(UINT32 hsizeOut);
extern void pre_setOutputLineOffset(UINT32 LineOffset);
extern UINT32 pre_readCurrentOutBufferID(void);
extern UINT32 pre_getSaoAdd(UINT32 BufID);
extern void pre_setOutputStartAdd(UINT32 addr);
extern void pre_setOutputStartAdd1(UINT32 addr);
#if 0
//Interrupt
extern void pre_enableInt(BOOL Int_enable, UINT32 pre_Intrpt);
extern UINT32 pre_getIntrStatus(void);
extern void pre_clrIntrStatus(UINT32 status);
extern void pre_setBreakPoint(UINT32 BP1);
//Function
extern void pre_enableFunction(BOOL Fnc_enable,UINT32 pre_Function);
extern BOOL pre_checkFunctionEnable(UINT32 pre_Function);
extern void pre_setColorInfo(STR_PRE_CG_INFOR *CG_Info);
extern void pre_setDefectLine(UINT32 *defectLinePos);
extern void pre_setCfa(PRECFA VIG_CFA,PRECFA Gamma_CFA);
//void pre_setCfa1(PRECFA cfa);
//void pre_setCfa2(PRECFA cfa);
extern PRECFA pre_getCfa1(void);
extern PRECFA pre_getCfa2(void);
#endif
//Hscale
extern void pre_setHScale(unsigned short InputH, unsigned short OutputH);
extern void pre_setHBining(UINT8 pixel_add, UINT8 value_div);
extern void pre_setHScaling(UINT8 hscl_int, UINT8 hscl_frt);

#if 0
//for compatibility
extern void pre_setGeoPosition(UINT32 GEO_CENTX, UINT32 GEO_CENTY, UINT32 GEO_CENTXS, UINT32 GEO_CENTYS);
extern void pre_setGeoParamNormGain(UINT32 GEO_NORMFACT, UINT32 GEO_NORMBIT, UINT32 GEO_RLUTGAIN, UINT32 GEO_GLUTGAIN, UINT32 GEO_BLUTGAIN);
extern void pre_setGeoParamDist(UINT32 GEO_DISTSEL, UINT32 GEO_XDIST, UINT32 GEO_YDIST);
extern void pre_setGeoParamCorrGain(UINT32 GEO_CORREN, UINT32 GEO_CORRNORM, UINT32 GEO_RCORRGAIN, UINT32 GEO_GRCORRGAIN, UINT32 GEO_GBCORRGAIN, UINT32 GEO_BCORRGAIN);
extern void pre_setGeo(UINT32 *pre_Geo_Lut);
extern void pre_getGeoCorrTerm(UINT8 *geo_corren, UINT8 *geo_corrnorm);
extern ER pre_setVaccParam(STR_PRE_VA_INFOR *VaccInfo);
extern void pre_getVaccValue(STR_PRE_VA_VALUE *VaccValue);
extern void pre_getVaccNum(STR_PRE_VA_NUM *VaccNum);
extern void pre_waitVAEnd(BOOL bClrFlag);
#endif
#endif
//@}
