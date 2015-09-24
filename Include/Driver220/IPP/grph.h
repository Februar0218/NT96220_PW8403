/**
    Graphic module driver

    This module driver use 3 types of state(IDLE, READY, RUN)
\n  IDLE: Driver is not opened, change register setting is not allowed.
\n  READY: driver is open and is ready for using, change register setting is allowed.
\n  RUN: graphic engine is running, change register setting is not allowed.

    @file       grph.h
    @ingroup    mIIPPGraph
    @note
\n              -grph_attach() to enable the graphic clock
\n              -grph_open() to start this module, hook the ISR.
\n              -grph_close() to close this module, and un-hook the ISR.
\n              -grph_detach() to close the graphic clock
\n
\n              This module will have to enable and wait the system interrupt(_INT_GRAPHY).
\n
\n              -grph_isr() will be called when Graph interrupt is on.
\n
\n              -grph_setImg1, grph_setImg2, grph_setImg3. set parameters of images.
\n
\n              -grph_setAOP, grph_setAOP2, grph_setGOP, grph_setGOP2 to set either operation.
\n               and engine will start at the end of these function.
\n
\n              Usage example:
\n
\n              (1)grph_attach();
\n              (2)grph_open();
\n              (3)grph_setImg1(...);
\n                 grph_setImg2(...);
\n                 grph_setGOP(...);
\n
\n                 grph_setImg1(...);
\n                 grph_setImg2(...);
\n                 grph_setAOP(...);
\n
\n                 grph_setImg1(...);
\n                 grph_setImg2(...);
\n                 grph_setGOP2(...);
\n
\n                 grph_setImg1(...);
\n                 grph_setImg2(...);
\n                 grph_setAOP2(...);
\n
\n                      :
\n              (4)grph_close();
\n              (5)grph_detach();

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef   _GRPH_H
#define   _GRPH_H


/** \addtogroup mIIPPGraph */
//@{

/**
    Destination output selection

    Select which destination output for graphic engine operation result.

    @note For grph_setGOP(), grph_setGOP2(), grph_setAOP()
*/
typedef enum
{
    GRPH_DST_1 = 0x00,                  ///< Destination use image 1
    GRPH_DST_2 = 0x20                   ///< Destination use image 2
} GRPH_DST_SEL;

/**
    Destination bitskip selection

    Select which destination bitskip selection for graphic engine 10/12 bits operation.

    @note For grph_setAOP2()
*/
typedef enum
{
    GRPH_DST_BITSKIP1 = 0x00,           ///< Destination use bitskip 1
    GRPH_DST_BITSKIP2 = 0x20            ///< Destination use bitskip 2
} GRPH_DST_BITSKIP_SEL;

/**
    Geometric operation mode

    Select which Geometric operation mode for graphic engine operation.
    @note For grph_setGOP(), grph_setGOP2().
*/
typedef enum
{
    GRPH_GOP_ROT_90,            //0x00        ///< Rotate 90 degree clockwise
    GRPH_GOP_ROT_270,           //0x01        ///< Rotate 270 degree clockwise
    GRPH_GOP_ROT_180,           //0x02        ///< Rotate 180 degree
    GRPH_GOP_HRZ_FLIP,          //0x03        ///< Horizontal flip
    GRPH_GOP_VTC_FLIP,          //0x04        ///< Vertical flip
    GRPH_GOP_HRZ_CFLT,          //0x05        ///< Horizontal chroma filter. (NOT supported by NT96220)
    GRPH_GOP_VTC_CFLT,          //0x06        ///< Vertical chroma filter. (NOT supported by NT96220)
    GRPH_GOP_WARP,              //0x07        ///< Image Warping (NOT supported by NT96220)
    GRPH_GOP_SCALE_DOWN         //0x08        ///< Scaling down
} GRPH_GOP_MODE;

/**
    Arithmetric operation control

    Select which Arithmetric operation control mode for graphic engine operation.
    @note For grph_setAOP(), grph_setAOP2().
*/
typedef enum
{
    GRPH_AOP_A_COPY,            //0x00        ///< A -> destination
    GRPH_AOP_PLUS_SHF,          //0x01        ///< (A+(B>>SHF[1:0])) -> destination
    GRPH_AOP_MINUS_SHF,         //0x02        ///< (A-(B>>SHF[1:0])) -> destination
    GRPH_AOP_COLOR_EQ,          //0x03        ///< A -> destination with color key (=)
    GRPH_AOP_COLOR_LE,          //0x04        ///< A -> destination with color key (<)
    GRPH_AOP_A_AND_B,           //0x05        ///< A & B -> destination
    GRPH_AOP_A_OR_B,            //0x06        ///< A | B -> destination
    GRPH_AOP_A_XOR_B,           //0x07        ///< A ^ B -> destination
    GRPH_AOP_TEXT_COPY,         //0x08        ///< text -> destination
    GRPH_AOP_TEXT_AND_A,        //0x09        ///< text & A -> destination
    GRPH_AOP_TEXT_OR_A,         //0x0A        ///< text | A -> destination
    GRPH_AOP_TEXT_XOR_A,        //0x0B        ///< text ^ A -> destination
    GRPH_AOP_TEXT_AND_AB,       //0x0C        ///< (text & A) | (~text & B) -> destination
    GRPH_AOP_BLENDING,          //0x0D        ///< (A * WA) + (B * WB) -> destination
    GRPH_AOP_ACC,               //0x0E        ///< Acc(A):256 pixel accumulation
    GRPH_AOP_MULTIPLY_DIV,      //0x0F        ///< (A * B)>>DIV[2:0] -> destination
    GRPH_AOP_PACKING,           //0x10        ///< Pack(A,B)->C (NOT supported by NT96220)
    GRPH_AOP_DEPACKING,         //0x11        ///< Unpack(A) ->(B,C) (NOT supported by NT96220)
    GRPH_AOP_TEXT_MUL,          //0x12        ///< A*text >> SHF -> destination (NOT supported by NT96220)
    GRPH_AOP_PLANE_BLENDING,    //0x13        ///< (A*B + C*(255-B)) >> 8 -> destination (NOT supported by NT96220)
    GRPH_AOP_ALPHA_SWITCH,      //0x14        ///< (B < 128) ? A : C  -> destination (NOT supported by NT96220)
    GRPH_AOP_HWORD_TO_BYTE,     //0x15        ///< ( A[15..0] >> SHF ) -> C[7..0] (NOT supported by NT96220)


    GRPH_AOP_MINUS_SHF_ABS=0x80,//0x80        ///< abs(A-(B>>SHF[1:0])) -> destination
    GRPH_AOP_COLOR_MR           //0x81        ///< A -> destination with color key (>)
} GRPH_AOP_MODE;

/**
    @name Blending control

    Select which blending control weighting for graphic engine operation.

    @note For uiProperty in grph_setAOP() work with aopMode = GRPH_AOP_BLENDING. Please refer to grph_setAOP() for usage.
*/
//@{
#define GRPH_BLD_WA_PER_2           0x01  ///< Weighting-A Value 1/2
#define GRPH_BLD_WA_PER_4           0x02  ///< Weighting-A Value 1/4
#define GRPH_BLD_WA_PER_8           0x04  ///< Weighting-A Value 1/8
#define GRPH_BLD_WA_PER_16          0x40  ///< Weighting-A Value 1/16

#define GRPH_BLD_WB_PER_2           0x08  ///< Weighting-B Value 1/2
#define GRPH_BLD_WB_PER_4           0x10  ///< Weighting-B Value 1/4
#define GRPH_BLD_WB_PER_8           0x20  ///< Weighting-B Value 1/8
#define GRPH_BLD_WB_PER_16          0x80  ///< Weighting-B Value 1/16
//@}

/**
    @name Advanced blending control

    Select which blending control weighting & blending threshold for graphic engine operation.

    @param[in] WA       Blending weight of image A. valid value: 0~255
    @param[in] WB       Blending weight of image B. valid value: 0~255
    @param[in] THR      Threshold value of AOP GRPH_AOP_BLENDING. valie value: 0~255\n
                        Only apply blending to pixel if abs(A-B)<THR. \n

    @note For uiProperty in grph_setAOP2() work with aopMode = GRPH_AOP_BLENDING. \n
            Please refer to grph_setAOP2() for usage.
*/
//@{
#define GRPH_BLD_WA_WB_THR(WA,WB,THR)   ((WA&0xFF)|((WB&0xFF)<<8)|((THR&0xFF)<<16))  ///< Weighting MACRO for parameter uiProperty of grph_setAOP2().
//@}

/**
    Graphic engine API return code (ER type)

    Return operation result for graphic engine operation.
*/
typedef enum
{
    GRPH_SUCCESS,                       ///< Operation is done and success.
    GRPH_ER_ENGINE_NOT_READY,           ///< Not ready for usage, maybe is running or not opened.
    GRPH_ER_ENGINE_NOT_SUPPORT          ///< Not valid input parameters / Engine no support.

} GRPH_ERROR;


/**
    Accumulation control

    Accumulation skip control, used for grph_setAccCtl().
*/
typedef enum
{
    GRPH_ACC_SKIP_NONE,                 ///< Accumulation none skip.
    GRPH_ACC_SKIP_ODD,                  ///< Accumulation odd skip.
    GRPH_ACC_SKIP_EVEN                  ///< Accumulation even skip.

} GRPH_ACC_SKIP_CTL;



/**
    @name Text MUL control

    Text MUL control for GRPH_AOP_TEXT_MUL

    - Byte Mode: C[7..0] = A*Text[11..4] >> SHF[3..0]
                -# A and C support IN/OUT operation respectively.
                -# Text supports signed/unsigned value.
    - HWORD Mode: C[15..0] = (A*Text[11..4] >> SHF[3..0]) + B[15..0]
                -# A support IN operation.
                -# C[15..0] & B[15..0] are signed value.
                -# Text supports signed/unsigned value.

    @note GRPH_AOP_TEXT_MUL is NOT supported by NT96220. \n
          Text MUL control is reserved for software compatability.
*/
//@{
#define GRPH_TEXTMUL_BYTE         0x00000000    ///< Byte Mode. (Exclusive use with GRPH_TEXTMUL_HWORD)
#define GRPH_TEXTMUL_HWORD        0x80000000    ///< Half-word Mode. (Exclusive use with GRPH_TEXTMUL_BYTE)

#define GRPH_TEXTMUL_SIGNED       0x40000000    ///< Signed Multiply (Exclusive use with GRPH_TEXTMUL_UNSIGNED)
#define GRPH_TEXTMUL_UNSIGNED     0x00000000    ///< Un-signed Multiply (Exclusive use with GRPH_TEXTMUL_SIGNED)
//@}



/**
    @name Plane-Blending & alpha-switch control

    Plane-Blending & alpha-switch control for GRPH_AOP_PLANE_BLENDING, GRPH_AOP_ALPHA_SWITCH

    Select the operation mode for plane blending and alpha switch operation.

    @note GRPH_AOP_PLANE_BLENDING and GRPH_AOP_ALPHA_SWITCH are NOT supported by NT96220.\n
            These MACROs are reserved for software compatability.
*/
//@{
#define GRPH_ENABLE_COLOR_CONST     0x10000     ///< Enable constant color source instead of image A.
                                                ///< If constant color is enabled, GRPH_COLOR_CONST() will decide color source.
#define GRPH_ENABLE_ALPHA_CONST     0x20000     ///< Enable constant alpha source instead of image B.
                                                ///< If constant alpha is enabled, GRPH_ALPHA_CONST() will decide color source.
#define GRPH_COLOR_ALPHA_MASK       (GRPH_ENABLE_COLOR_CONST|GRPH_ENABLE_ALPHA_CONST)   ///< Bit mask to filter GRPH_ENABLE_COLOR_CONST and GRPH_ENABLE_ALPHA_CONST

#define GRPH_COLOR_CONST(x)         ((x & 0xFF) <<  8)  ///< Constant color when GRPH_ENABLE_COLOR_CONST is enabled
#define GRPH_ALPHA_CONST(x)         ((x & 0xFF) << 16)  ///< Constant alpha when GRPH_ENABLE_ALPHA_CONST is enabled
#define GRPH_CONST_MASK             0x0000FFFF          ///< Bit mask to filter constant color/alpha
//@}


/**
    @name Half-Word to Byte control

    Half-Word to Byte control for GRPH_AOP_HWORD_TO_BYTE

    Select the operation mode of signed/unsigned control for HWORD-to-BYTE control operation.

    @note GRPH_AOP_HWORD_TO_BYTE is NOT supported by NT96220. \n
          Half-Word to Byte control is reserved for software compatability.
*/
//@{
#define GRPH_HWORD_TO_BYTE_SIGNED       0x00000000      ///< Signed half-word to unsigned byte
#define GRPH_HWORD_TO_BYTE_UNSIGNED     0x80000000      ///< Unsigned half-word to unsigned byte
//@}



/**
    INPUT operation control

    Definition for the INPUT operation control.

    @note For GRPH_INOP and PGRPH_INOP. \n
        (NOT supported by NT96220)
*/
typedef enum
{
    GRPH_INOP_NONE,         //0x00      ///< None:              A -> A
    GRPH_INOP_INVERT,       //0x01      ///< Invert:            A -> ~A
    GRPH_INOP_SHIFTR_ADD,   //0x02      ///< Shift right & Add: A -> ((A>>SHF[3..0]) + Constant)
    GRPH_INOP_SHIFTL_ADD,   //0x03      ///< Shift left & Add:  A -> ((A<<SHF[3..0]) + Constant)
    GRPH_INOP_SHIFTL_SUB    //0x04      ///< Shift left & Sub:  A -> ((A<<SHF[3..0]) - Constant)
}GRPH_INOPCTL;

/**
    INPUT operation

    Structure for the INPUT operation.

    @note For grph_setImage1IN() and grph_setImage2IN(). \n
        (NOT supported by NT96220)
*/
typedef struct
{
    GRPH_INOPCTL    InOpCtl;        ///< Input Operation Control
    UINT8           uiInSHF;        ///< Shift Value for Input Operation
    UINT8           uiInConst;      ///< Constant value for Input Operation

}GRPH_INOP,*PGRPH_INOP;



/**
    OUTPUT operation control

    Definition for the OUTPUT operation control.

    @note For GRPH_OUTOP and PGRPH_OUTOP. \n
        (NOT supported by NT96220)
*/
typedef enum
{
    GRPH_OUTOP_SHF,                 ///< Shift:         C = (C << SHF[3..0])
    GRPH_OUTOP_INVERT,              ///< Invert:        C = ~C
    GRPH_OUTOP_ADD,                 ///< Add:           C = C + Constant
    GRPH_OUTOP_SUB,                 ///< Sub:           C = C - Constant
    GRPH_OUTOP_ABS                  ///< Absolute       C = ABS(C)
}GRPH_OUTOPCTL;

/**
    OUTPUT operation

    Structure for the OUTPUT operation.

    @note For grph_setImage3OUT(). \n
        (NOT supported by NT96220)
*/
typedef struct
{
    GRPH_OUTOPCTL   OutOpCtl;       ///< Output Operation Control
    UINT8           uiOutSHF;       ///< Shift Value for Output Operation
    UINT8           uiOutConst;     ///< Constant value for Output Operation
}GRPH_OUTOP,*PGRPH_OUTOP;


/**
    ColorKey filter control

    Definition for the color key filter control.

    @note For grph_setAOP2() work with aopMode = GRPH_AOP_COLOR_EQ or GRPH_AOP_COLOR_LE. \n
        (NOT supported by NT96220)
*/
typedef struct
{
    UINT32   FKEY1;         ///< color key filter key 1
    UINT32   FKEY2;         ///< color key filter key 2
    UINT32   RKEY;          ///< color key filter replace key
}GRPH_CKEYFILTER,*PGRPH_CKEYFILTER;


#define grph_swReset()
#define grph_enableDMA()
#define grph_disableDMA()

extern ER       grph_attach(void);
extern ER       grph_detach(void);
extern ER       grph_open(void);
extern BOOL     grph_isOpened(void);
extern ER       grph_close(void);
extern ER       grph_init(UINT32 uiClkRate);

extern ER       grph_setImg1(UINT32 uiAddr, UINT32 uiLnOff, UINT32 uiLnCnt, UINT32 uiXRgn);
extern ER       grph_setImg2(UINT32 uiAddr, UINT32 uiLnOff);
extern ER       grph_setImg3(UINT32 uiAddr, UINT32 uiLnOff);
extern ER       grph_setImage1IN(UINT32 uiAddr, UINT32 uiLnOff, UINT32 uiLnCnt, UINT32 uiXRgn, PGRPH_INOP pInOP);
extern ER       grph_setImage2IN(UINT32 uiAddr, UINT32 uiLnOff, PGRPH_INOP pInOP);
extern ER       grph_setImage3OUT(UINT32 uiAddr, UINT32 uiLnOff, PGRPH_OUTOP pOutOp);
extern ER       grph_setAccCtl(GRPH_ACC_SKIP_CTL uiSkipCtl);
extern ER       grph_getAccResult(BOOL *bCntFull, UINT8 *uiPixCnt, UINT32 *uiVldPixCnt, UINT32 *uiAccResult);
extern ER       grph_setGOP(BOOL b24BitPrc, GRPH_DST_SEL dstSel, BOOL bByteSwap, GRPH_GOP_MODE gopMode);
extern ER       grph_setGOP2(BOOL b24BitPrc, GRPH_DST_SEL dstSel, GRPH_GOP_MODE gopMode, UINT32 uiProperty);
extern ER       grph_setAOP(BOOL b16BitPrc, GRPH_DST_SEL dstSel, GRPH_AOP_MODE aopMode, UINT32 uiProperty);
extern ER       grph_setAOP2(UINT8 BitPrc, GRPH_DST_BITSKIP_SEL dstSel, GRPH_AOP_MODE aopMode, UINT32 uiProperty,UINT32 bitskip1, UINT32 bitskip2);
#if _FPGA_EMULATION_
extern ER       grph_getCheckSumResult(UINT16 *uiChecksumA, UINT8 *uiChecksumB, UINT8 *uiChecksumC);
#endif
//@}

#endif
