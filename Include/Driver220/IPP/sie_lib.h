/**
    Public header file for SIE module.

    @file       sie_lib.h
    @ingroup    mIIPPSIE

    @brief

    How to start:\n
    [Preview]\n
    1. sie_attach\n
    2. sie_open\n
    3. SIE_Operation = SIE_ENGINE_SET2PRV, sie_setMode\n
    4. SIE_Operation = SIE_ENGINE_SET2RUN, sie_setMode\n

    [Preview to Capture]\n
    1. SIE_operation = SIE_ENGINE_SET2PAUSE, sie_setMode\n
    2. SIE_Operation = SIE_ENGINE_SET2CAP, sie_setMode\n
    3. SIE_Operation = SIE_ENGINE_SET2RUN, sie_setMode\n
    4. SIE_operation = SIE_ENGINE_SET2PAUSE, sie_setMode\n


    How to stop:\n
    1. SIE_operation = SIE_ENGINE_SET2PAUSE, sie_setMode\n
    2. sie_close\n
    3. sie_detach\n


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _SIE_LIB_H
#define _SIE_LIB_H

#include "Type.h"


/** \addtogroup mIIPPSIE */
//@{

//@{
#define  SlaveMode       0 ///< SIE in slave mode
#define  MasterMode      1 ///< SIE in master mode
#define  Notinvert   0 ///< signal is not inverted
#define  Invert      1 ///< signal is inverted
//@}

/*
    Define SIE function enable.
*/
//@{
#define SIE_LINE_BLM_EN                 0x00000040  ///< enable function: vertical defect line compensation
#define SIE_CROP_EN                        0x00000080  ///< enable function: sensor data in
#define SIE_OB_AVER_EN                  0x00000100  ///< enable function: OB average calculation
#define SIE_OB_EN                            0x00000200  ///< enable function: using the calculated OB average
#define SIE_BLM_EN                          0x00000400  ///< enable function: defect pixel compensation
#define SIE_HSCL_EN                        0x00000800  ///< enable function: horizontal downsampling
#define SIE_CG_EN                            0x00001000  ///< enable function: color gain
#define SIE_GAMMA_EN                    0x00002000  ///< enable function: gamma
#define SIE_SUBOUT_EN                    0x00004000  ///< enable function: ca out
#define SIE_PADDING_EN                  0x00040000  ///< enable function: horizontal padding
#define SIE_CHOUT_EN                      0x00100000  ///< enable function: multi-channel output decode
#define SIE_HFLIP_EN                        0x00200000  ///< enable function: horizontal flip
#define SIE_VFLIP_EN                        0x00400000  ///< enable function: vertical flip
#define SIE_ECS_EN                           0x01000000  ///< enable function: ECS
#define SIE_DF_SUB_EN                     0x08000000  ///< enable function: dark frame subtraction
#define SIE_DF_EMBED_EN                 0x40000000  ///< enable function: dark frame subtraction with DP embed
//@}

/*
    Define SIE interrput enable.
*/
//@{
#define SIE_INTE_FLDEND              0x00000004  ///< enable interrupt: field/frame end
#define SIE_INTE_BP1                    0x00000020  ///< enable interrupt: break point 1
#define SIE_INTE_BP2                    0x00000040  ///< enable interrupt: break point 2
#define SIE_INTE_OVFL1                0x00000080  ///< enable interrupt: output DMA buffer overflow
#define SIE_INTE_SUB                    0x00000200  ///< enable interrupt: subsample output done
#define SIE_INTE_FLSH                  0x00000400  ///< enable interrupt: flash end
#define SIE_INTE_MSH                    0x00000800  ///< enable interrupt: Mshutter end
#define SIE_INTE_DPCF                  0x00001000  ///< enable interrupt: defect pixel compensation failed
#define SIE_INTE_DRAMO_END       0x00002000  ///< enable interrupt: end of dram output
#define SIE_INTE_DFS_DP_FAIL     0x00004000  ///< enable interrupt: DFS or DP input fail
#define SIE_INTE_SLICE_END         0x00008000  ///< enable interrupt: end of slice output
#define SIE_INTE_OVFL3                0x00010000  ///< enable interrupt: Subsample output DMA buffer overflow
#define SIE_INTE_FLSHCTR            0x00020000  ///< enable interrupt: flash control signal detection
#define SIE_INTE_MSHCTRL           0x00040000  ///< enable interrupt: Mshutter control sigmal detection
#define SIE_INTE_ECS_FAIL           0x00080000  ///< enable interrupt: ECS DRAM input fail
#define SIE_INTE_ACTST                0x00100000  ///< enable interrupt: active window start
#define SIE_INTE_ACTEND              0x00200000  ///< enable interrupt: active window end
#define SIE_INTE_ALL                     0x003fffff  ///< enable interrupt: all interrupts
//@}

/*
    Define SIE interrput status.
*/
//@{
#define SIE_INT_FLDEND              0x00000004  ///< enable status: field/frame end
#define SIE_INT_BP1                    0x00000020  ///< enable status: break point 1
#define SIE_INT_BP2                    0x00000040  ///< enable status: break point 2
#define SIE_INT_OVFL1                0x00000080  ///< enable status: output DMA buffer overflow
#define SIE_INT_SUB                    0x00000200  ///< enable status: subsample output done
#define SIE_INT_FLSH                  0x00000400  ///< enable status: flash end
#define SIE_INT_MSH                    0x00000800  ///< enable status: Mshutter end
#define SIE_INT_DPCF                  0x00001000  ///< enable status: defect pixel compensation failed
#define SIE_INT_DRAMO_END       0x00002000  ///< enable status: end of dram output
#define SIE_INT_DFS_DP_FAIL     0x00004000  ///< enable status: DFS or DP input fail
#define SIE_INT_SLICE_END         0x00008000  ///< enable status: end of slice output
#define SIE_INT_OVFL3                0x00010000  ///< enable status: Subsample output DMA buffer overflow
#define SIE_INT_FLSHCTR            0x00020000  ///< enable status: flash control signal detection
#define SIE_INT_MSHCTRL           0x00040000  ///< enable status: Mshutter control sigmal detection
#define SIE_INT_ECS_FAIL           0x00080000  ///< enable status: ECS DRAM input fail
#define SIE_INT_ACTST                0x00100000  ///< enable status: active window start
#define SIE_INT_ACTEND              0x00200000  ///< enable status: active window end
#define SIE_INT_ALL                     0x003fffff  ///< enable status: all interrupts
//@}

/**
    SIE Engine Operation Selection.

    Select SIE module operation.
*/
//@{
typedef enum
{
    SIE_ENGINE_OPEN = 0, ///< open module
    SIE_ENGINE_CLOSE = 1, ///< close module
    SIE_ENGINE_ATTACH = 2, ///< attach module (enable clock)
    SIE_ENGINE_DETACH = 3, ///< detach module (disable clock)
    SIE_ENGINE_SETMODE = 4, ///< set module to capture mode
    SIE_ENGINE_SET2PAUSE = 5, ///< pause module
    SIE_ENGINE_SET2RUN = 6, ///< set module to run
    SIE_ENGINE_CHGPARAM = 7, ///< change module param
    ENUM_DUMMY4WORD(SIE_ENGINE_OPERATION)
} SIE_ENGINE_OPERATION;
//@}

/**
    SIE Number of Output Ping-Pong Buffers.

    Select SIE number of output PPBs.
*/
//@{
typedef enum
{
    ONE_OUTPUT_BUF = 0, ///< one output PPB
    TWO_OUTPUT_BUF = 1, ///< two output PPBs
    THREE_OUTPUT_BUF = 2, ///< three output PPBs
    ENUM_DUMMY4WORD(SIE_OUTBUFNUM)
}SIE_OUTBUFNUM;
//@}

/**
    SIE CFA Pattern Selection.

    Select start pixel of SIE CFA pattern.
*/
//@{
typedef enum
{
    SIECFA_R = 0, ///< start pixel is R
    SIECFA_Gr = 1, ///< start pixel is GR
    SIECFA_Gb = 2, ///< start pixel is GB
    SIECFA_B = 3, ///< start pixel is B
    ENUM_DUMMY4WORD(SIE_CFAEnum)
}SIE_CFAEnum;
//@}

/**
    SIE Output Packing Selection.

    Select SIE output packing mode.
*/
//@{
typedef enum
{
    SIE_OUT_PACK_8BIT = 0, ///< 8 bits data output
    SIE_OUT_PACK_10BIT = 1, ///< 10 bits data output
    ENUM_DUMMY4WORD(SIE_OutPackBitSelEnum)
}SIE_OutPackBitSelEnum;
//@}

/**
    SIE Input Packing Selection.

    Select SIE input packing mode.
*/
//@{
typedef enum
{
    SIE_IN_PACK_8BIT = 0, ///< 8 bits data input
    SIE_IN_PACK_10BIT = 1, ///< 10 bits data input
    ENUM_DUMMY4WORD(SIE_InPackBitSelEnum)
}SIE_InPackBitSelEnum;
//@}

/**
    SIE Horizontal Pixel Adding Selection.

    Select SIE horizontal pixel addition.
*/
//@{
typedef enum
{
    HSCL_ADD_1_PIXEL = 0, ///< by itself
    HSCL_ADD_2_PIXEL = 1, ///< addition of two pixels
    ENUM_DUMMY4WORD(SIE_AddPixelEnum)
} SIE_AddPixelEnum;
//@}

/**
    SIE Horizontal Pixel Division Selection.

    Select SIE horizontal pixel division.
*/
//@{
typedef enum
{
    HSCL_DIV_1_PIXEL = 0, ///< divided by one
    HSCL_DIV_2_PIXEL = 1, ///< divided by two
    ENUM_DUMMY4WORD(SIE_DivPixelEnum)
} SIE_DivPixelEnum;
//@}

/**
    SIE Defect compensation Selection.

    Select SIE defect compensation factor.
*/
//@{
typedef enum
{
    DEFECT_NFACT_0 = 0, ///< Right shift 0
    DEFECT_NFACT_1 = 1, ///< Right shift 1
    DEFECT_NFACT_2 = 2, ///< Right shift 2
    DEFECT_NFACT_3 = 3, ///< Right shift 3
    ENUM_DUMMY4WORD(SIE_DefectNfactEnum)
} SIE_DefectNfactEnum;
//@}

/**
    SIE flash Trigger Type.

    Select SIE flash to be high active or low active.
*/
//@{
typedef enum
{
    FLASH_HIGH_ACTIVE = 0,  ///< flash is high active
    FLASH_LOW_ACTIVE = 1, ///< flash is low active
    ENUM_DUMMY4WORD(SieFlashHighActEnum)
}SieFlashHighActEnum;
//@}

/**
    SIE Mshutter Trigger Mode.

    Trigger Mshutter by level or pulse.
*/
//@{
typedef enum
{
    MSHUTTER_LEVEL_MODE = 0, ///< Mshutter in level mode
    MSHUTTER_PULSE_MODE = 1, ///< Mshutter in pulse mode
    ENUM_DUMMY4WORD(SieMshtrModeEnum)
}SieMshtrModeEnum;
//@}

/**
    SIE Mshutter Trigger Type.

    Select SIE Mshutter to be high active or low active.
*/
//@{
typedef enum
{
    MSHUTTER_HIGH_ACTIVE = 0,  ///< Mshutter is high active
    MSHUTTER_LOW_ACTIVE = 1, ///< Mshutter is low active
    ENUM_DUMMY4WORD(SieMshtrHighActEnum)
}SieMshtrHighActEnum;
//@}

/**
    SIE Mshutter Take Action at Current or Next Frame.

    Select Mshutter to start from current or next frame.
*/
//@{
typedef enum
{
    MSHUTTER_CURRENT_FRAME = 0, ///< Mshutter takes action at current frame
    MSHUTTER_NEXT_FRAME = 1, ///< Mshutter takes action at next frame
    ENUM_DUMMY4WORD(SieMshtrFrameDelayEnum)
}SieMshtrFrameDelayEnum;
//@}

/**
    SIE Path of Output Source Selection.

    Select SIE path of output source.
*/
//@{
typedef enum
{
    PATH_PRE_COLORGAIN = 0, ///< output before color gain
    PATH_PRE_GAMMA = 1, ///< output before gamma
    PATH_POST_GAMMA = 2, ///< output after gamma
    ENUM_DUMMY4WORD(SIE_OutputPathEnum)
}SIE_OutputPathEnum;
//@}

/**
    SIE DFS Operation Selection.

    Select SIE dark frame subtraction operation.
*/
//@{
typedef enum
{
    DFS_A_MINUS_B = 0,///< A - B
    DFS_B_MINUS_A = 1, ///< B - A
    DFS_A_PLUS_B = 2, ///< A + B
    DFS_A_MUL_B_SHIFT_P2P = 3, ///< (A * B) >> shift, point on point
    ENUM_DUMMY4WORD(SIE_DfsOpEnum)
} SIE_DfsOpEnum;
//@}

typedef enum
{
    DFS_TH_DRAM = 0,
    DFS_TH_SENSOR = 1,
    ENUM_DUMMY4WORD(SIE_DfsThSrcEnum)
} SIE_DfsThSrcEnum;

typedef enum
{
    SIECG_2_8 = 0,
    SIECG_3_7 = 1,
    ENUM_DUMMY4WORD(SIE_CGSelEnum)
} SIE_CGSelEnum;

/**
    SIE Channel Number Selection.

    Select SIE total channel decode number.
*/
//@{
typedef enum
{
    CHDEC_TOTAL2CH = 0,///< Channel decode total 2 channels
    CHDEC_TOTAL3CH = 1, ///< B - A
    CHDEC_TOTAL4CH = 2, ///< A + B
    CHDEC_TOTAL6CH = 3, ///< (A * B) >> shift, point on point
    ENUM_DUMMY4WORD(SIE_ChDecTotalEnum)
} SIE_ChDecTotalEnum;
//@}

typedef enum
{
    FRAME_UPDATE = 0,
    CLRINT_UPDATE = 1,
    ENUM_DUMMY4WORD(SIE_SubOutModeEnum)
} SIE_SubOutModeEnum;

typedef enum
{
    ECSMAP65x65 = 0,
    ECSMAP49x49 = 1,
    ECSMAP33x33 = 2,
    ENUM_DUMMY4WORD(SIE_EcsMapSizeEnum)
} SIE_EcsMapSizeEnum;

typedef enum
{
    PAT_DISABLE = 0,
    PAT_COLORBAR = 1,
    PAT_RANDOM = 2,
    PAT_FIXED = 3,
    PAT_HINCREASE = 4,
    PAT_HVINCREASE = 5,
    ENUM_DUMMY4WORD(SIE_PatternModeEnum)
} SIE_PatternModeEnum;

typedef enum
{
    SIE_FRAME_MODE = 0,
    SIE_SLICE_MODE = 1,
    ENUM_DUMMY4WORD(SIE_OutputModeEnum)
} SIE_OutputModeEnum;

//#NT#2011/06/09#YHLee -begin
//#NT#Add SIE D2D api
/**
    SIE D2D wait frame-end selection

    Select wait frame-end or not for Dram2Dram operation
\n Used for sie_Dram2Dram().
*/
typedef enum
{
    SIE_WAIT = 0,       ///< Wait for frame-end before exit api
    SIE_NOWAIT = 1,   ///< Don't wait for frame-end in api, external sie_Dram2DramWaitEnd() should be called to make sure frame operation ends
    ENUM_DUMMY4WORD(SIE_WaitEnum)
}SIE_WaitEnum;
//@}
//#NT#2011/06/09#YHLee -end

//#NT#2011/06/23#YHLee -begin
//#NT#Add output address api
typedef enum
{
    CHG_ALL_OUTPARAM = 0,
    CHG_ALL_ADD = 1, 
    CHG_ONLY_ADD_1_2 = 2, 
    ENUM_DUMMY4WORD(SIE_ChgOutEnum)
}SIE_ChgOutEnum;
//#NT#2011/06/23#YHLee -end


/**
    SIE Open Object.

    Open Object is used for opening SIE module.
    \n Used for sie_open()

*/
//@{
typedef struct _SIE_OPENOBJ{
    void (*FP_SIEISR_CB)(UINT32 intstatus); ///< isr callback function
} SIE_OPENOBJ;
//@}

/**
    SIE HD/VD Sync and Total Periods.

    The sync and total periods of VD and HD.
*/
//@{
typedef struct _STR_HDVD{
    UINT32    HD_sync; ///< HD sync period
    UINT32    VD_sync; ///< VD sync period
    UINT32    HD_total; ///< HD total period
    UINT32    VD_total; ///< VD total period
    BOOL      HD_inv;
    BOOL      VD_inv;
} STR_HDVD;
//@}

/**
    SIE Horizontal and Vertical Active window.

*/
//@{
typedef struct _STR_SIE_ACTIVE{
    UINT32    Hstart; ///< horizontal start
    UINT32    Vstart; ///< vertical start
    UINT32    Hsize; ///< horizontal size
    UINT32    Vsize; ///< vertical size
} STR_SIE_ACTIVE;
//@}

/**
    SIE Horizontal and Vertical Crop window.

*/
//@{
typedef struct _STR_SIE_CROP{
    UINT32    Hstart; ///< horizontal start
    UINT32    Vstart; ///< vertical start
    UINT32    Hsize; ///< horizontal size
    UINT32    Vsize; ///< vertical size
} STR_SIE_CROP;
//@}

/**
    SIE Output Horizontal Sizes.

    The width of scaling output, and line offset of output data.
*/
//@{
typedef struct _STR_SIE_OUT_SIZE{
    UINT32    Hsize; ///< horizontal size
    UINT32    lineofs; ///< line offset
} STR_SIE_OUT_SIZE;
//@}

typedef struct _STR_SIE_PATTERN{
    SIE_PatternModeEnum    PatMode;
    UINT32    PatValue;
} STR_SIE_PATTERN;

typedef struct _STR_SIE_HBIN{
    SIE_AddPixelEnum PixAdd;
    SIE_DivPixelEnum PixDiv;
} STR_SIE_HBIN;
/**
    SIE Output DRAM Addr.

    The width and height of active window, and line offset of output data.
*/
//@{
typedef struct _STR_SIE_OUT_ADDR{
    SIE_OUTBUFNUM                     OutBufNum; ///< number of output ping-pong buffers
    UINT32                                      OutAddr0; ///< output ping-pong buffer address 0
    UINT32                                      OutAddr1; ///< output ping-pong buffer address 1
    UINT32                                      OutAddr2; ///< output ping-pong buffer address 2
} STR_SIE_OUT_ADDR;
//@}

/**
    SIE channel decode.

    The sequence of channel decode numbers and total number of channels.
*/
//@{
typedef struct _STR_CHNUM{
    SIE_ChDecTotalEnum    TotalCh; ///< total number of fields. 0: 1 field; 1: 2 fields, ..., 15: 16 fields.
    UINT32    Ch0; ///< field num of the 1st input channel
    UINT32    Ch1; ///< field num of the 2nd input channel
    UINT32    Ch2; ///< field num of the 3rd input channel
    UINT32    Ch3; ///< field num of the 4th input channel
    UINT32    Ch4; ///< field num of the 5th input channel
    UINT32    Ch5; ///< field num of the 6th input channel
} STR_CHNUM;
//@}

/**
    SIE Sensor Phase Parameters.

    SIE in master/slave mode and VD/HD/Data phases.
*/
//@{
typedef struct _STR_PHASE{
    BOOL Master; ///< master or slave mode 0: slave mode; 1: master mode
    BOOL VD_Phase; ///< VD phase 0: rising edge; 1: falling edge
    BOOL HD_Phase; ///< HD phase 0: rising edge; 1: falling edge
    BOOL Data_Phase; ///< Data phase 0: rising edge; 1: falling edge
} STR_PHASE;
//@}

/**
    SIE Parameters of OB Window.

    Horizontal and vertical boundaries of the optical black window, threshold and subsample rate.
*/
//@{
typedef struct _STR_OB_Param{
    UINT32    X0; ///< left end of the window
    UINT32    Y0; ///< top of the window
    UINT32    X1; ///< right end of the window
    UINT32    Y1; ///< bottom of the window
    UINT32    Thres; ///< calculation threshld. if pixel value < threshld, it will be taken into account.
    UINT32    SubSample; ///< horizontal subsample rate: 1/(1+subsample).
} STR_OB_Param;
//@}

/**
    SIE Parameters of Subsample output.

*/
//@{
typedef struct _STR_SUBOUT_Param{
    SIE_SubOutModeEnum SubUpdateMode;
    SIE_OutputPathEnum    SubOutSrc;
    UINT32    Hsub;
    UINT32    Vsub;
    UINT32    Hcnt;
    UINT32    Vcnt;
} STR_SUBOUT_Param;
//@}

typedef struct _STR_DFS_Param{
    SIE_DfsOpEnum DfsOp;
    SIE_DfsThSrcEnum DfsThSrc;
    UINT32 DfsTh;
    UINT32 DfsShiftNum;
} STR_DFS_Param;

//#NT#2011/4/08#YHLee -begin
//#NT#SIE driver mod
/**
    SIE Color Gain Information.

    Structure of color gain information.
*/
//@{
typedef struct _STR_SIE_CG_INFOR{
    UINT32 Gain0; ///< color gain of even line, even pixel
    UINT32 Gain1; ///< color offset of even line, odd pixel
    UINT32 Gain2; ///< color offset of odd line, even pixel
    UINT32 Gain3; ///< color offset of odd line, odd pixel
    SIE_CGSelEnum GainSel; ///< gain selection
}STR_SIE_CG_INFOR;
//@}

/**
    SIE Color Offset Information.

    Structure of color offset information.
*/
//@{
typedef struct _STR_SIE_COFS_INFOR{
    UINT32 Ofs0; ///< color offset of even line, even pixel
    UINT32 Ofs1; ///< color offset of even line, odd pixel
    UINT32 Ofs2; ///< color offset of odd line, even pixel
    UINT32 Ofs3; ///< color offset of odd line, odd pixel
}STR_SIE_COFS_INFOR;
//@}
//#NT#2011/4/08#YHLee -end

/**
    SIE ECS parameters.

    Structure of ECS parameters.
*/
//@{
typedef struct _STR_SIE_ECS_PARAM{
    SIE_EcsMapSizeEnum MapSizeSel;
    BOOL MapDthrEn;
    BOOL MapDthrRst;
    UINT32 MapDthrLevel;
    UINT32 MapShift;
    SIE_CFAEnum CfaPat;
}STR_SIE_ECS_PARAM;
//@}

/**
 SIE flash parameters.
    
 Structure of flash parameters.
*/
//@{
typedef struct _STR_SIE_FLASH_PARAM{
    SieFlashHighActEnum FlashActSel;
    UINT32 LineDelay;
    UINT32 PX2Delay;
    UINT32 FlashWidth;
    BOOL FlashImd;
}STR_SIE_FLASH_PARAM;

/**
 SIE Mshutter parameters.
    
 Structure of Mshutter parameters.
*/
//@{
typedef struct _STR_SIE_MSHTR_PARAM{
    SieMshtrModeEnum MshMode;
    SieMshtrHighActEnum MshActSel;
    SieMshtrFrameDelayEnum FrameDelay;
    UINT32 LineDelay;
    UINT32 PX2Delay;
    UINT32 MshWidth;
    UINT32 CloseField;
    BOOL PinSwap;
}STR_SIE_MSHTR_PARAM;


/**
    SIE Entire Configuration.

    Structure of entire SIE configuration.
*/
//@{
typedef struct _STR_SIE_REG {
    SIE_ENGINE_OPERATION         SieOperation; ///< SIE engine operation
    BOOL                                       SieDRAMInEn;
    SIE_OutputModeEnum                          SieOutMode;
    UINT32                                      SieFuncEn;
    BOOL                                       SieAutoOBEn;
    UINT32                                      SieIntEn; ///< enable interrupt
    UINT32                                      SieIntBP1; ///< break point 1
    UINT32                                      SieIntBP2; ///< break point 2
    STR_HDVD                               SieHdVdSync; ///< sync and total periods of VD and HD
    STR_PHASE                             SieSyncPhase; ///< sync phase of VD/HD/Data, and master or slave mode.
    STR_SIE_ACTIVE                      SieActParam; ///< horizontal and vertical active start points
    STR_SIE_CROP                        SieCropParam;
    SIE_OutputPathEnum                SieOutSrc;
    SIE_OutPackBitSelEnum           SieOutPack; ///< output packing mode
    STR_SIE_OUT_SIZE                  SieOutSize; ///< horizontal and vertical sizes of output/active window
    STR_SIE_OUT_ADDR                SieOutAddr; ///< output ping-pong buffers
    UINT32                            SieSliceHeight;
    SIE_InPackBitSelEnum              SieInPack; ///< input packing mode
    UINT32                                      SieInLofs;  
    UINT32                                      SieInAddr0; ///< intput ping-pong buffer address 0
    UINT32                                      SieInAddr1; ///< intput ping-pong buffer address 1
    STR_OB_Param                        SieOBParam; ///< parameters of OB window
    UINT32                                      SieOBOfs; ///< OB offset
    STR_CHNUM                             SieCHDecodeParam;
    UINT32                                      SieChOutLofs;  
    //#NT#2011/4/08#YHLee -begin
    //#NT#SIE driver mod
    STR_SIE_CG_INFOR                 SieCGParam;
    STR_SIE_COFS_INFOR             SieCofsParam;
    //#NT#2011/4/08#YHLee -end
    STR_SIE_ECS_PARAM              SieECSParam;
    STR_DFS_Param                   SieDFSParam;
    STR_SUBOUT_Param                SieSubParam;
    UINT32                                      SieSubAddr;  
    UINT32                                      SieSubOutLofs;  
    UINT32                                      *SieDefColumn;
    SIE_DefectNfactEnum                         SieDefNfact;
    STR_SIE_HBIN                        SieHBinParam;
    STR_SIE_PATTERN                   SiePatParam;
}STR_SIE_REG;
//@}

extern volatile unsigned int sie_fc;

extern void sie_attach(void);
extern void sie_detach(void);
extern ER sie_lock(void);
extern ER sie_unlock(void);
extern ER sie_open(SIE_OPENOBJ *pObjCB);
extern ER sie_close(void);
extern ER sie_pause(void);
extern ER sie_start(void);
extern ER sie_setMode(STR_SIE_REG *SIE_PARAM);
//#NT#2011/1/31#YHLee -begin
//#NT#Add output line offset
extern ER sie_changeOutputSize(STR_SIE_CROP *CropParam, UINT32 OutHSize, UINT32 OutLofs);
//#NT#2011/1/31#YHLee -end
//#NT#2011/06/23#YHLee -begin
//#NT#Add output address api
ER sie_changeOutputAdd(SIE_ChgOutEnum ChgSelect, STR_SIE_OUT_ADDR *SieOutAddr);
//#NT#2011/06/23#YHLee -end
//#NT#2011/4/08#YHLee -begin
//#NT#SIE driver mod
//#NT#2011/3/22#YHLee -begin
//#NT#Add color gain offset
extern ER sie_changeCG(STR_SIE_CG_INFOR *CGParam);
extern ER sie_changeCofs(STR_SIE_COFS_INFOR *CofsParam);
extern ER sie_changeSubOutParam(STR_SUBOUT_Param *SubOutParam);
//#NT#2011/3/22#YHLee -end
//#NT#2011/4/08#YHLee -end
//#NT#2012/6/14#YHLee -begin
//#NT#Add input address api
extern ER sie_changeInputAdd1(UINT32 SieInAdd1);
//#NT#2012/6/14#YHLee -end

//#NT#2011/06/09#YHLee -begin
//#NT#Add SIE D2D api
extern ER sie_Dram2Dram(STR_SIE_REG *SIE_PARAM, SIE_WaitEnum WaitSel);
extern void sie_Dram2DramWaitEnd(void);
//#NT#2011/06/09#YHLee -end

extern void sie_waitVD(UINT32 count);
extern void sie_waitBP1(BOOL bClrFlag);
extern void sie_waitBP2(BOOL bClgFlag);
extern void sie_waitFlashDone(BOOL bClrFlag);
//#NT#2011/07/07#YHLee -begin
//#NT#Re-arrange flag ID
extern void sie_waitDMAEnd(BOOL bClrFlag);
//#NT#2011/07/07#YHLee -end
extern void sie_isr(void);
extern BOOL sie_checkBusy(void);
//extern void sie_resetHardware(void);

//control
extern void sie_reset(BOOL reset);
extern BOOL sie_isEnabled(void);
extern void sie_load(void);
extern void sie_setDRAMIn(BOOL enable);
extern void sie_enableFunction(BOOL Fnc_enable,UINT32 sie_Function);
extern BOOL sie_checkFunctionEnable(UINT32 sie_Function);
extern void sie_enableSensorData(BOOL enable);
extern void sie_setSensorSync(STR_HDVD HDVDSync);
extern UINT32 sie_getPixelCounter(void);
extern void sie_setBreakPoint(UINT32 BP1, UINT32 BP2);
extern void sie_setPatGen(SIE_PatternModeEnum mode, UINT32 value);
//slice mode
extern void sie_setSliceMode(BOOL enable);
extern BOOL sie_getSliceMode(void);
extern void sie_setSliceHeight(UINT32 SliceHeight);
extern UINT32 sie_getSliceHeight(void);
//interrupt
extern void sie_enableInt(BOOL Int_enable, UINT32 sie_Intrpt);
extern UINT32 sie_getIntrStatus(void);
extern void sie_clrIntrStatus(UINT32 status);
//Flash
extern void sie_setFlashStart(void);
extern void sie_setFlashParam(STR_SIE_FLASH_PARAM FlashParam);
//Mshutter
extern void sie_setMshtrStart(void);
extern void sie_setMshtrParam(STR_SIE_MSHTR_PARAM MshParam);
//OB
extern void sie_getOB(UINT32 *OB_Avg, UINT32 *OB_Cnt);
extern void sie_setOBOffset(UINT32 OB_Offset);

extern UINT32 sie_getOutputLineOffset(void);
extern UINT32 sie_getDramSao(UINT8 BufID);
extern UINT32 sie_getDramSai(UINT8 BufID);
extern void sie_getCropWindow(STR_SIE_CROP *CropParam);
extern UINT32 sie_getSubOutputAdd(void);
extern UINT32 sie_getSubOutputLinOffset(void);
extern void sie_getSubOutputParam(STR_SUBOUT_Param *SubOutParam);
//#NT#2011/1/17#YHLee -begin
//#NT#Add SIE function extern
extern UINT32 sie_getLineCounter(void);
extern UINT32 sie_getCurrentOutputAdd(void);
extern BOOL sie_getVsyncStatus(void);
extern BOOL sie_getHsyncStatus(void);
//#NT#2011/1/17#YHLee -end
//#NT#Add SIE function extern

//#NT#2011/11/10#YHLee -begin
//#NT#SIE get phase api
extern void sie_getSyncPhase(STR_PHASE *SyncPhase);
extern void sie_getSensorSync(STR_HDVD *HDVDSync);
//#NT#2011/11/10#YHLee -end
#endif

//@}
