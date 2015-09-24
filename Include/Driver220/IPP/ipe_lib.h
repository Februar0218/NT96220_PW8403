/**
    IPE module driver

    NT96220 IPE driver extern header file.

    @file       Ipe_lib.h
    @ingroup    mIIPPIPE
    @note
\n IPE Driver has 4 states (Idel, Ready, Pause, Run)
\n Idle state: No operation, and IPE semaphore is not occupied.
\n Ready state: No operation, IPE parameters are not configured yet, but IPE semaphore is occupied.
\n Pause state: No operation, IPE parameters are already configured for operation, IPE semaphore is occupied.
\n Run state: In operation, some parameters are allowed to be changed dynamically, IPE semaphore is occupied.
\n ipe_open    Get IPE semaphore before operation starts
\n
\n Function list:
\n ipe_open()           Get IPE semaphore before operation starts
\n ipe_close()          Release IPE semaphore after operation ends
\n ipe_changeSize()     Set all Dzoom size related params
\n ipe_changeParam()    Set IQ related params
\n ipe_changeInput()    Set input address, sync selection, and PPB control
\n ipe_changeOutput()   Set output source, address, and line offset
\n ipe_setMode()        Set all IPE params, including size, IQ param, I/O
\n ipe_start()          Trigger IPE start
\n ipe_pause()          Trigger IPE stop
\n ipe_reset()          SW reset IPE
\n ipe_enableDMAOut()   Turn on/off IPE output DMA channel
\n ipe_clrFrameEnd()    Clear IPE frame end flag
\n ipe_waitFrameEnd()   Wait IPE frame end flag
\n ipe_clrDMAEnd()      Clear IPE DMA done flag
\n ipe_waitDMAEnd()     Wait IPE DMA done flag
\n
\n Dram to Dram usage example:
\n (1)ipe_open()
\n (2)ipe_setMode()
\n (3)ipe_start()
\n (4)ipe_waitFrameEnd()
\n (5)ipe_close()

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _IPE_LIB_H
#define _IPE_LIB_H

#define IPE_DEBUG       0
#define IPE_DEBUG_INFO  0

#define GetIpeVACnt ipe_getVACnt

/** \addtogroup mIIPPIPE */
//@{

/**
    @name Define IPE function enable.
*/
//@{
#define IPE_CGAIN_EN                   0x00000001  ///< Color gain function enable
#define IPE_NR_EN                      0x00000002  ///< NR function enable
#define IPE_RLPF_EN                    0x00000004  ///< R channel low-pass filter function enable
#define IPE_GLPF_EN                    0x00000008  ///< G channel low-pass filter function enable
#define IPE_BLPF_EN                    0x00000010  ///< B channel low-pass filter function enable
#define IPE_CCR_EN                     0x00000020  ///< Color correction function enable
#define IPE_CTRL_EN                    0x00000080  ///< Color control function enable
#define IPE_HADJ_EN                    0x00000100  ///< Hue adjustment function enable
#define IPE_CADJ_EN                    0x00000400  ///< Color component adjust function enable
#define IPE_CADJ_YENH_EN               0x00000800  ///< Color component edge enhance function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_YLPF_EN               0x00001000  ///< Color component Y low-pass filter function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_YCON_EN               0x00002000  ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_CLPF_EN               0x00004000  ///< Color component CbCr low-pass filter function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_CCON_EN               0x00008000  ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJ_EN
#define IPE_VACC_EN                    0x00010000  ///< VA function enable
#define IPE_YCTH_EN                    0x00020000  ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJ_EN
//@}

/**
    @name Define IPE interrput enable.
*/
//@{
#define IPE_INTE_ST                    0x00000001  ///< Interrupt enable for The current stripe is done
#define IPE_INTE_SLI                   0x00000002  ///< Interrupt enable for Slice done
#define IPE_INTE_FRM                   0x00000004  ///< Interrupt enable for All stripes in the image are done
#define IPE_INTE_VACC                  0x00000010  ///< Interrupt enable for VA register is all ready for cpu read
#define IPE_INTE_FRMERR                0x00000020  ///< Interrupt enable for Fail in ipe_ppbi_en=1 & SST mode (frame buffer overflow)
#define IPE_INTE_SLIERR                0x00000040  ///< Interrupt enable for Fail in ipe_ppbi_en=1 & Auto Slice mode (initial slice buffer id mismatch)
#define IPE_INTE_BUF0FAIL              0x00000100  ///< Interrupt enable for Fail in buf id0
#define IPE_INTE_BUF1FAIL              0x00000200  ///< Interrupt enable for Fail in buf id1
#define IPE_INTE_BUF2FAIL              0x00000400  ///< Interrupt enable for Fail in buf id2
#define IPE_INTE_ALL                   0x00000777  ///< All interrupt enable
//@}

/**
    @name Define IPE interrput status.
*/
//@{
#define IPE_INT_ST                     0x00000001  ///< Interrupt status for The current stripe is done
#define IPE_INT_SLI                    0x00000002  ///< Interrupt status for Slice done
#define IPE_INT_FRM                    0x00000004  ///< Interrupt status for All stripes in the image are done
#define IPE_INT_VACC                   0x00000010  ///< Interrupt status for VA register is all ready for cpu read
#define IPE_INT_FRMERR                 0x00000020  ///< Interrupt status for Fail in ipe_ppbi_en=1 & SST mode (frame buffer overflow)
#define IPE_INT_SLIERR                 0x00000040  ///< Interrupt status for Fail in ipe_ppbi_en=1 & Auto Slice mode (initial slice buffer id mismatch)
#define IPE_INT_BUF0FAIL               0x00000100  ///< Interrupt enable for Fail in buf id0
#define IPE_INT_BUF1FAIL               0x00000200  ///< Interrupt enable for Fail in buf id1
#define IPE_INT_BUF2FAIL               0x00000400  ///< Interrupt enable for Fail in buf id2
#define IPE_INT_ALL                    0x00000777  ///< All interrupt status
//@}

/**
    @name Define IPE engine value.
*/
//@{
#define IPE_HSIZE_IODIFF            16              ///< IPE horizontal output size is 16 pix smaller than input size
#define VAC_WIN                     3               ///< VA window number 3

//#define IPE_MAX_COLOR_OFFSET        255
//#define IPE_MAX_COLOR_GAIN          1023
//@}

/**
    @name Define IPE function setting selection.
*/
//@{
#define IPE_SET_NONE            0x00000000  ///< Select don't update ANY parameters
#define IPE_SET_IFE             0x00000001  ///< Select to embeded IFE
#define IPE_SET_NR              0x00000002  ///< Select to update NR parameters
#define IPE_SET_EDGE            0x00000004  ///< Select to update edge related parameters
#define IPE_SET_COLOR           0x00000008  ///< Select to update color correction parameters
#define IPE_SET_COLOR_GAIN      0x00000010  ///< Select to update color gain parameters
#define IPE_SET_COLOREFFECT     0x00000020  ///< Select to update color effect parameters
#define IPE_SET_INVERSEEFFECT   0x00000040  ///< Select to update inverse effect parameters
#define IPE_SET_YCFIXEFFECT     0x00000080  ///< Select to update YCbCr substitution parameters
#define IPE_SET_EDGEEFFECT      0x00000200  ///< Select to update edge effect parameters
#define IPE_SET_COLOR_OFS       0x00002000  ///< Select to update color offset parameters
//#NT#2011/06/14#Yuppie Lin -begin
//#NT#Add extension color setting
#define IPE_SET_COLOR_EXT       0x00004000  ///< Select to update extension color parameters
//#NT#2011/06/14#Yuppie Lin -end
#define IPE_SET_VA              0x80000000  ///< Select to update VA parameters
#define IPE_SET_ALL             0xffffffff  ///< Select to update ALL IQ parameters
//@}

/**
    @name Enum IPE input control.
*/
//@{

/**
    IPE input Bayer data start CFA selection

    Select intput Bayer raw data start channel - R, Gr, Gb, B.
\n Used for ipe_changeSize().
*/
typedef enum
{
    IPE_PATR = 0,           ///< Start channel is R for input Bayer raw data
    IPE_PATGR = 1,          ///< Start channel is Gr for input Bayer raw data
    IPE_PATGB = 2,          ///< Start channel is Gb for input Bayer raw data
    IPE_PATB = 3,           ///< Start channel is B for input Bayer raw data
    ENUM_DUMMY4WORD(IPE_CfaPatEnum)
}IPE_CfaPatEnum;

/**
    IPE input data bit number selection

    Select intput data bit number - 8, 10, 12, 16 bit.
\n Used for ipe_changeInput().
*/
typedef enum
{
    IPE_PACK_12bit = 0,     ///< Input data fornat is 12 bit pack
    IPE_PACK_10bit = 1,     ///< Input data fornat is 10 bit pack
    IPE_PACK_08bit = 2,     ///< Input data fornat is 8 bit pack
    IPE_PACK_16bit = 3,     ///< Input data fornat is 16 bit pack
    ENUM_DUMMY4WORD(IPE_PackBitEnum)
}IPE_PackBitEnum;

/**
    IPE input ping-pong buffer initial ID selection

    Select intput ping-pong buffer initial ID.
\n Used for ipe_setIOPPB().
*/
typedef enum
{
    PPBINI_0 = 0,           ///< Set initial input ping-pong buffer ID to 0
    PPBINI_1 = 1,           ///< Set initial input ping-pong buffer ID to 1
    PPBINI_2 = 2,           ///< Set initial input ping-pong buffer ID to 2
    ENUM_DUMMY4WORD(IPE_InPPBIniEnum)
}IPE_InPPBIniEnum;

/**
    @name Enum IPE operation mode.
*/
//@{

/**
    IPE stripe mode selection

    Select IPE intput stripe mode
\n Used for ipe_changeSize().
*/
typedef enum
{
    SINGLE_STRIPE_MODE = 0, ///< Single stripe mode, IPE processes one stripe according to HL and VL setting
    AUTO_SLICE_MODE = 1,    ///< This mode is not supported
    AUTO_FRAME_MODE = 2,    ///< Auto frame mode, IPE processes multi-stripes according to HN, HL, HM, VN, VL, VM setting
    DVI_MODE = 3,           ///< DVI mode, IPE processes CCIR601 data format(Y Cb Y Cr) in single stripe mode according to HL and VL setting
    ENUM_DUMMY4WORD(IPE_StripeEnum)
}IPE_StripeEnum;

//@}

/**
    @name Enum IPE DVI.
*/
//@{

/**
    IPE DVI input start pixel selection

    Select IPE input start pixel for DVI mode
*/
typedef enum
{
    DVI_CBYCRY = 0,         ///< DVI input order is Cb, Y, Cr, Y
    DVI_CRYCBY = 1,         ///< DVI input order is Cr, Y, Cb, Y
    DVI_YCRYCB = 2,         ///< DVI input order is Y, Cr, Y, Cb
    DVI_YCBYCR = 3,         ///< DVI input order is Y, Cb, Y, Cr
    ENUM_DUMMY4WORD(IPE_DVIFormatInEnum)
}IPE_DVIFormatInEnum;

/**
    IPE DVI input start pixel selection

    Select IPE horizontal subsample ratio for DVI mode
*/
typedef enum
{
    DVI_HSUB1 = 0,          ///< 1X H subsample, 640x480 input Y->640x480 output Y
    DVI_HSUB2 = 1,          ///< 2X H subsample, 640x480 input Y->320x480 output Y
    DVI_HSUB4 = 2,          ///< 4X H subsample, 640x480 input Y->160x480 output Y
    ENUM_DUMMY4WORD(IPE_DVIHSubOutEnum)
}IPE_DVIHSubOutEnum;

//@}

/**
    @name Enum IPE output control.
*/
//@{

/**
    IPE Cb Cr filter method selection

    Select method for Cb Cr smooth filter
\n Not open for modification.
*/
typedef enum
{
    LOWPASS = 0,            ///< 3 tap low pass filtering method
    MEDIAN = 1,             ///< Median value in 3 method
    ENUM_DUMMY4WORD(IPE_CbCrLMFSelEnum)
}IPE_CbCrLMFSelEnum;

//#NT#2011/06/14#Yuppie Lin -begin
//#NT# Add/Modify function/para type
/**
    IPE color gain format setting
*/
typedef enum
{
    FMT_2_8 = 0,            ///< 2 bit integer and 8 bit decimal
    FMT_3_7 = 1,            ///< 3 bit integer and 7 bit decimal
    ENUM_DUMMY4WORD(IPE_CGFmtSelEnum)
}IPE_CGFmtSelEnum;
//#NT#2011/06/14#Yuppie Lin -end

//@}

/**
    @name Enum IPE overlap enable.
*/
//@{

/**
    IPE multi-stripe overlap selection

    Select IPE overlap method in multi-stripe mode
\n Used for ipe_changeSize().
*/
typedef enum
{
    IPE_OLAP_OFF = 0,       ///< Disable stripe overlap
    IPE_OLAP_ON = 1,        ///< Enable stripe overlap, overlap must be enabled when direct output to IME
    ENUM_DUMMY4WORD(IPE_OlapEnum)
}IPE_OlapEnum;

//@}


/**
    @name Enum IPE VA param.
*/
//@{

/**
    IPE variation accumulation DRAM update mode selection

    Select DRAM update mode for VA
\n Used for ipe_changeParam().
*/
typedef enum
{
    VACC_AUTO_UPDATED = 0,       ///< VA result is updated to DRAM every frame
    VACC_MAN_UPDATED = 1,        ///< VA result is updated to DRAM after interrupt status IPE_INT_DMAEND cleared
    ENUM_DUMMY4WORD(IPE_VAModeEnum)
}IPE_VAModeEnum;

//@}


/**
    @name Enum IPE wait frame end selection.
*/
//@{

/**
    IPE wait frame-end selection

    Select clear flag or not before waiting for IPE frame-end
\n Used for ipe_waitFrameEnd().
*/
typedef enum
{
    IPE_NOCLRFLG = 0,           ///< Select not to clear previous flag before waiting for frame-end
    IPE_CLRFLG = 1,             ///< Select to clear previous flag before waiting for frame-end
    ENUM_DUMMY4WORD(IPE_FlgClrEnum)
}IPE_FlgClrEnum;
//@}

/**
    @name Enum IPE stripe mode for external selection.
*/
//@{

/**
    IPE stripe mode selection

    Select stripe mode for IPE operation
\n Used for ipe_changeSize().
*/
typedef enum
{
    IPE_SSTP = 0,           ///< Single stripe mode, IPE processes one stripe according to HL and VL setting
    IPE_MSTP = 1,           ///<  Multi stripe mode, IPE processes multi-stripes according to HN, HL, HM, VN, VL, VM setting
    IPE_AUTOSTP = 2,        ///<  Auto stripe mode, based on input sizes, IPE driver automatically decides stripe modes and calculates stripe sizes
    IPE_AUTOSLICE = 3,      ///<  Auto Slice mode, IPE handshake with SIE in SLICE mode
    IPE_DVIMODE = 4,        ///<  DVI mode, IPE processes CCIR601 data format in single stripe mode according to HL and VL setting
    ENUM_DUMMY4WORD(IPE_SripeModeEnum)
}IPE_SripeModeEnum;
//@}


/**
    @name Enum IPE function setting mode.
*/
//@{
/**
    IPE function set/enable/disable selection

    Select IPE function update mode
\n Used for ipe_changeParam().
*/
typedef enum
{
    IPE_FUNC_ENABLE = 0,    ///< Enable functions, FuncEn |= FuncSel, 1s in FuncSel are enabled
    IPE_FUNC_DISABLE = 1,   ///< Disable functions, FuncEn &= (~FuncSel), 1s in FuncSel are disabled
    IPE_FUNC_SET = 2,       ///< Set functions, FuncEn = FuncSel, 1s in FuncSel are enabled while 0s in FuncSel are disabled
    IPE_FUNC_NOUPDATE = 3,  ///< Don't update functions, FuncEn = FuncEn, disregard FuncSel
    ENUM_DUMMY4WORD(IPE_FuncUpdateEnum)
}IPE_FuncUpdateEnum;
//@}

/**
    @name Enum IPE VA hnf.
*/
//@{
/**
    IPE VA hnf auto/manual selection

    Select IPE VA h normalization method
\n Used for aaa_GetVariation().
*/
typedef enum
{
    IPE_HNF_AUTO = 0,       ///< IPE driver automatically calculates hnf according to sizes
    IPE_HNF_MANUAL = 1,     ///< IPE driver accepts manual hnf input
    ENUM_DUMMY4WORD(AF_HNFSelEum)
}AF_HNFSelEum;
//@}

//######################################################
// the following for structure define

/**
    IPE structure - IPE size.
*/
//@{
typedef struct _STR_IMG_SIZE{
    UINT32    hsize;        ///< Horizontal image size, unit : pix
    UINT32    vsize;        ///< Vertical image size, uint : line
}STR_IMG_SIZE;
//@}

/**
    IPE structure - IPE stripe.
*/
//@{
typedef struct _STR_STRIPE_INFOR{
    UINT32    n;            ///< Size of stripe except for last one, H unit : 16 pix, V unit : 1 line
    UINT32    l;            ///< Size of last stripe, H unit : 16 pix, V unit : 1 line
    UINT32    m;            ///< Number of total stripe - 1
    IPE_OlapEnum    olap;   ///< Overlap enable selection for multi-stripe mode
}STR_STRIPE_INFOR;
//@}

/**
    IPE structure - IPE threshold.
*/
//@{
typedef struct _STR_TH_INFOR{
    UINT32    th_low;       ///< Lower bound threshold
    UINT32    th_high;      ///< Higher bound threshold
}STR_TH_INFOR;
//@}

/**
    IPE structure - IPE VA.
*/
//@{
//#NT#2012/01/05#YuppieLin -begin
//#NT#modify VA setting legal range
typedef struct _STR_VA_INFOR{
    UINT32    vacc_gain;            ///< Legal range : 0~7, VA contrast value gain before accumulation, con' = con << vacc_gain
    UINT32    stx;                  ///< Legal range : 0~2047, VA window start point horizontal coordinate
    UINT32    sty;                  ///< Legal range : 0~1023, VA window start point vertical coordinate
    IPE_VAModeEnum    vacc_mode;    ///< VA DRAM output mode selection
    UINT32    hnf;                  ///< Legal range : 0~15, VA window horizontal normalization factor
    UINT32    hsz;                  ///< Legal range : 0~511, singlw VA window horizontal size
    UINT32    vsz;                  ///< Legal range : 0~255, singlw VA window vertical size
    UINT32    va_th;                ///< Legal range : 0~255, VA contrast threshold before accumulation, only pixels with contrast larger than va_th are accumulated
}STR_VA_INFOR;
//#NT#2012/01/05#YuppieLin -end
//@}

/**
    IPE structure - IPE color channel.
*/
//@{
typedef struct _STR_COLOR_INFOR{
    UINT32 ChR;     ///< R channel information
    UINT32 ChGr;    ///< Gr channel information
    UINT32 ChGb;    ///< Gb channel information
    UINT32 ChB;     ///< B channel information
}STR_COLOR_INFOR;
//@}

/**
    IPE structure - IPE edge.
*/
//@{
typedef struct _STR_IPE_EDGE_ENHMAP_PARA{
    UINT32     EdgEnhanceP;     ///< Legal range : 0~1023,  positive edge enhancement factor, controls edge overshoot level
    UINT32     EdgEnhanceN;     ///< Legal range : 0~1023,  negative edge enhancement factor, controls edge undershoot level
    UINT32     EmapA;           ///< Legal range : 0~1023,  Emap check point A
    UINT32     EmapB;           ///< Legal range : 0~1023,  Emap check point B
    UINT32     EmapC;           ///< Legal range : 0~1023,  Emap check point C
    UINT32     EmapD;           ///< Legal range : 0~1023,  Emap check point D
    UINT32     EmapN;           ///< Legal range : 0~7,     Emap slope N
    UINT32     EmapM;           ///< Legal range : 0~7,     Emap slope M
    UINT32     EmapE;           ///< Legal range : 0~1023,  Emap check point E
}STR_IPE_EDGE_ENHMAP_PARA;
//@}

/**
    IPE structure - IPE NR.
*/
//@{
typedef struct _STR_IPE_NOVANR_PARA{
    UINT32      GBAL;           ///< Legal range : 0~1, G channel balance filter enable control, set to 1 to smooth Gb Gr mismatch
    UINT32      Nsmar;          ///< Legal range : 0~1023, noise margin for G interpolation, if(neighbor G variation < Nsmar){NO G edge judgment in interpolation}
    UINT32      Lpfw;           ///< Legal range : 0~15, low-pass filter weighting after NR and interpolation, effective when either IPE_RLPF_EN, IPE_GLPF_EN, IPE_BLPF_EN is 1, 0 : NO filter, 63 : MAX filter weighting
    UINT32      Sumsel;         ///< Legal range : 0~1, low-pass filter selection, set to 1 for smoother output
}STR_IPE_NOVANR_PARA;
//@}

/**
    IPE structure - IPE CG CC.
*/
//@{
typedef struct _STR_IPE_COLOR_PARA{
    STR_COLOR_INFOR ColorGain;      ///< 4 channel color gain
    STR_COLOR_INFOR ColorOfs;       ///< 4 channel color offset
    INT16           *ColorCorrect;  ///< RGB to RGB color correction matrix, please refer to IQTT doc for color tuning
    UINT8           ColorSupWeight; ///< Legal range : 0~255, edge color suppress weighting, 0 : NO suppress, 255 : MAX suppress
}STR_IPE_COLOR_PARA;
//@}

/**
    IPE structure - IPE color effect.
*/
//@{
typedef struct
{
    UINT8 CbOfs;        ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
    UINT8 CrOfs;        ///< Legal range : 0~255, Cr color offset, 128 : NO color offset
    UINT8 YCon;         ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
    UINT8 CCon;         ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
    INT16 IntOfs;       ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
    INT16 SatOfs;       ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
} IPE_COLOR_EFFECT_PARAM;
//@}

//#NT#2011/06/14#Yuppie Lin -begin
//#NT#Add extension color setting
/**
    IPE structure - IPE extension color parameters.
*/
//@{
typedef struct _STR_IPE_COLOR_PARA_EXT{
    IPE_CGFmtSelEnum    ColorGain_range;  ///< Color gain format selection
    UINT8               ColorGain_mask;   ///< Color gain mask , Legal range : 0~255
    IPE_CGFmtSelEnum    Ccr_set;          ///< Color correction format selection
    IPE_CbCrLMFSelEnum  Cadj_lmfsel;      ///< CbCr lowpass method selection
}STR_IPE_COLOR_PARA_EXT;
//@}
//#NT#2011/06/14#Yuppie Lin -end

/**
    IPE structure - IPE inverse effect.
*/
//@{
typedef struct
{
    UINT32 Inv;     ///< Legal range : 0~1, 0 : normal mode, 1 : invert pixel value for output
    UINT32 Hinv;    ///< Legal range : 0~1, effective only when Inv is 1, 0 : normal mode, 1 : invert pixel value for output
} IPE_INVERSE_EFFECT_PARAM;
//@}

/**
    IPE structure - IPE YC fix effect.
*/
//@{
typedef struct
{
    UINT8 YFix;             ///< Legal range : 0~255, Y substitution value in Cadj, if(Ein < YFixETh){Yout = YFix}, else{Yout = Yin}
    UINT8 CbFix;            ///< Legal range : 0~255, Cb substitution value in Cadj,  if((Ein < CFixETh)||((Yin within CFixYTh)&&(CBin within CFixCbTh)&&(CRin within CFixCrTh))){CBout=CBFix, CRout=CRFix}, else{CBout=CBin, CRout=CRin}
    UINT8 CrFix;            ///< Legal range : 0~255, Cr substitution value in Cadj,  if((Ein < CFixETh)||((Yin within CFixYTh)&&(CBin within CFixCbTh)&&(CRin within CFixCrTh))){CBout=CBFix, CRout=CRFix}, else{CBout=CBin, CRout=CRin}
    UINT32 YFixETh;         ///< Legal range : 0~1023, edge strength threshold for YFix substitution
    UINT32 CFixETh;         ///< Legal range : 0~1023, edge strength threshold for CbFix, CrFix substitution
    STR_TH_INFOR CFixYTh;   ///< Y threshold for CbFix, CrFix substitution
    STR_TH_INFOR CFixCbTh;  ///< Cb threshold for CbFix, CrFix substitution
    STR_TH_INFOR CFixCrTh;  ///< Cr threshold for CbFix, CrFix substitution
} IPE_YCFIX_EFFECT_PARAM;
//@}

/**
    IPE structure - IPE edge effect.
*/
//@{
typedef struct
{
    BOOL EInvP;                         ///< Legal range : 0~1, if EInvP is 1, positive edge(overshoot) is inverted to negative edge(undershoot)
    BOOL EInvN;                         ///< Legal range : 0~1, if EInvN is 1, negative edge(undershoot) is inverted to positive edge(overshoot)
} IPE_EDGE_EFFECT_PARAM;
//@}

/**
    IPE structure - IPE open object.
*/
//@{
typedef struct _IPE_OPENOBJ{
    void (*FP_IPEISR_CB)(UINT32 intstatus);     ///< Call back function pointer to hook with IPE interrupt service routine
    UINT32 IPE_CLOCKSEL;                        ///< IPE engine clock select
} IPE_OPENOBJ,*PIPE_OPENOBJ;
//@}

/**
    IPE structure - IPE size info.
*/
//@{
//#NT#2011/01/20#Yuppie Lin -begin
//#NT# Add for DVI mode
typedef struct _IPE_SIZEINFO{
    IPE_SripeModeEnum StripeMode;   ///< IPE stripe mode selection
    STR_IMG_SIZE INSIZE;            ///< Input image H V size, effective if StripeMode is IPE_SSTP or IPE_AUTOSTP. If StripeMode is IPE_MSTP, HSTRIPE and VSTRIPE must be input
    STR_STRIPE_INFOR HSTRIPE;       ///< Input image H stripe, effective if StripeMode is IPE_MSTP, stripe info should be calculated to match IME stripe
    STR_STRIPE_INFOR VSTRIPE;       ///< Input image V stripe, effective if StripeMode is IPE_MSTP, stripe info should be calculated to match IME stripe
    IPE_CfaPatEnum CfaPat;          ///< IPE input Bayer data start CFA selection
    UINT32 LofsIn;                  ///< Legal range : multiples of 4(word aligned), IPE input image DRAM line offset
    IPE_DVIFormatInEnum DVIformat;  ///< IPE DVI input start pixel selection
} IPE_SIZEINFO,*PIPE_SIZEINFO;
//#NT#2011/01/20#Yuppie Lin -end
//@}

/**
    IPE structure - IPE input info.
*/
//@{
typedef struct _IPE_INPUTINFO{
    UINT32 PPBEn;                   ///< Legal range : 0~1, input ping-pong buffer enable selection, if PPBEn is 1, auto handshake between IPE and PPBISrc engine is enabled
    BOOL PPBSel;                    ///< Input ping-pong buffer initial ID select, 0: auto, 1: initial from PPBIni
    IPE_InPPBIniEnum PPBIni;        ///< IPE input ping-pong buffer initial ID selection
    UINT32 Adr0;                    ///< Legal range : multiples of 4(word aligned), IPE input address ID 0
    UINT32 Adr1;                    ///< Legal range : multiples of 4(word aligned), IPE input address ID 1
    UINT32 Adr2;                    ///< Legal range : multiples of 4(word aligned), IPE input address ID 2
    IPE_PackBitEnum PackBit;        ///< IPE input data bit number selection
    BOOL RGBInEn;                   ///< Enable RGB input
} IPE_INPUTINFO,*PIPE_INPUTINFO;
//@}

/**
    IPE structure - IPE IQ info.
*/
//@{
typedef struct _IPE_IQINFO{
    UINT32                      FuncSel;                ///< IPE function set/enable/disable selection, combined usage with FuncUpdateSel
    IPE_FuncUpdateEnum          FuncUpdateSel;          ///< IPE function set/enable/disable selection
    UINT32                      ParamUpdateSel;         ///< IPE parameters update selection, combined usage with definition IPE_SET_XXX
    BOOL                        IFEembed_en;            ///< Enable IFE embeded in IPE
    STR_IPE_NOVANR_PARA         *NR_PARAM;              ///< NR related parameters
    STR_IPE_EDGE_ENHMAP_PARA    *EDGE_PARAM;            ///< Edge related parameters
    INT16                       *EdgeKerDiv;            ///< Edge extraction kernel setting
    UINT8                       *Estab;                 ///< Legal range : 0~255, Estab table defines edge weighting for different luminance, 0 : NO edge, 255 : Max edge
    STR_IPE_COLOR_PARA          COLOR_PARAM;            ///< Color gain, color correction, and color suppress control
    UINT8                       *FStab;                 ///< Legal range : 0~255, FStab table defines color suppress weighting for different luminance, 0 : NO color suppress, 255 : Max color suppress
    UINT8                       *FDtab;                 ///< Legal range : 0~255, FDtab table defines color suppress weighting for different color saturation, 0 : NO color suppress, 255 : Max color suppress
    IPE_COLOR_EFFECT_PARAM      *COLOR_EFFECT_PARAM;    ///< Color effect related parameters
    UINT8                       *HueTab;                ///< Legal range : 0~255, color control hue adjust table, 128 : NO hue adjust
    //#NT#2011/08/08#Yuppie Lin -begin
    //#NT# modify signed varible to INT32
    INT32                       *SatTab;                ///< Legal range : -128~127, color control sat adjust table, 0 : NO sat adjust, -128 : Min sat suppress, 127 : Max sat enhance
    INT32                       *IntTab;                ///< Legal range : -128~127, color control int adjust table, 0 : NO int adjust, -128 : Min int suppress, 127 : Max int enhance
    //#NT#2011/08/08#Yuppie Lin -end
    UINT8                       *DDSTab;                ///< Legal range : 0~255, color control weighting table for SatTab, IntTab, and EdgTab, 0 : NO adjust, 32 : 1X adjust weighting
    IPE_INVERSE_EFFECT_PARAM    *INVERSE_EFFECT_PARAM;  ///< Inverse effect related parameters
    IPE_YCFIX_EFFECT_PARAM      *YCFIX_EFFECT_PARAM;    ///< Fix YC substitution effect related parameters
    IPE_EDGE_EFFECT_PARAM       *EDGE_EFFECT_PARAM;     ///< Edge effect related parameters
    //#NT#2011/06/14#Yuppie Lin -begin
    //#NT#Add extension color setting
    STR_IPE_COLOR_PARA_EXT      *COLOR_PARAM_EXT;       ///< Extension Color parameters
    //#NT#2011/06/14#Yuppie Lin -end
    STR_VA_INFOR                VA_PARAM;               ///< VA related parameters
} IPE_IQINFO,*PIPE_IQINFO;
//@}

/**
    IPE structure - IPE mode info.
*/
//@{
typedef struct _IPE_MODEINFO{
    UINT32 IntEn;           ///< Interrupt enable selection
    IPE_SIZEINFO SIZEINFO;  ///< IPE size and stripe related parameters
    IPE_INPUTINFO ININFO;   ///< IPE DRAM input related parameters
    IPE_IQINFO IQINFO;      ///< IPE IQ related parameters
} IPE_MODEINFO,*PIPE_MODEINFO;
//@}

/**
    IPE structure - IPE AF info.
*/
//@{
typedef struct _AF_IPEINFO{
    UINT32    vacc_gain;    ///< Legal range : 0~7, VA contrast value gain before accumulation, con' = con << vacc_gain
    UINT32    va_th;        ///< Legal range : 0~255, VA contrast threshold before accumulation, only pixels with contrast larger than va_th are accumulated
    AF_HNFSelEum hnfsel;    ///< IPE VA hnf auto/manual selection
    UINT32 va_hnf;          ///< Legal range : 0~15, VA window horizontal normalization factor, effective when hnfsel is IPE_HNF_MANUAL
} AF_IPEINFO,*PAF_IPEINFO;
//@}

extern UINT32   ipe_getVersion(void);
extern void     ipe_clrFrameEnd(void);
extern void     ipe_waitFrameEnd(IPE_FlgClrEnum ClrFlgSel);
extern ER       ipe_open(IPE_OPENOBJ *PIPE_OpenInfo);
extern ER       ipe_close(void);
extern ER       ipe_start(void);
extern ER       ipe_pause(void);
extern ER       ipe_reset(BOOL ResetEn);
extern ER       ipe_changeSize(IPE_SIZEINFO *HVsize);
extern ER       ipe_changeParam(IPE_IQINFO *IQinfo);
extern ER       ipe_changeInterrupt(UINT32 IntEn);
extern ER       ipe_changeInput(IPE_INPUTINFO *ININFO);
extern ER       ipe_setMode(IPE_MODEINFO *ModeInfo);
extern void     ipe_setSliceH(UINT32 slice_h);
extern ER       ipe_getInputinfor(IPE_INPUTINFO *ININFO);
extern UINT32   ipe_getCFAPat(void);
extern BOOL     ipe_getIFEembed(void);
extern UINT32   ipe_getFeature(void);
extern BOOL     ipe_checkFunctionEnable(UINT32 ipe_Function);
extern STR_STRIPE_INFOR ipe_getHstripe(void);
extern STR_STRIPE_INFOR ipe_getVstripe(void);
extern UINT32   ipe_getIntEnable(void);
extern UINT32   ipe_getIntStatus(void);
extern void     ipe_clearInt(unsigned long intr);
extern UINT32   ipe_getVACnt(void);
extern void     ipe_getVA_Infor(STR_VA_INFOR *VA_Infor);
extern void     ipe_getVA(UINT32   *VA_rslt);
extern UINT32   ipe_getDebugStatus(void);

//extern BOOL ipe_checkBusy(void);
//extern UINT32 ipe_getEdgeThCnt(void);
//extern UINT16 ipe_getFrmCnt(void);
//extern UINT32 ipe_getPPBEOutID(void);
//@}

#endif
