/*
    Public header file for IFE module.

    This file is the header file that define the API and data type for IFE
    module.

    @file       ife_lib.h
    @ingroup    mIIPPIFE

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _IFE_LIB_220_H
#define _IFE_LIB_220_H

/**
    @addtogroup mIIPPIFE
*/
//@{

/*
    Define IFE function enable.
*/
//@{
#define IFE_ROWDEFC_EN                  0x00000010  ///< enable function: row defect concealment
#define IFE_OUTL_EN                     0x00000020  ///< enable function: outlier
#define IFE_FILTER_EN                   0x00000040  ///< enable function: filter
#define IFE_CRVMAP_EN                   0x00000080  ///< enable function: curve mapping
#define IFE_UPSRC_EN                    0x80000000  ///< enable function: up source
#define IFE_DOWNSRC_EN                  0x40000000  ///< enable function: down source

//@}


/**
    Define IFE interrput enable.
*/
//@{
#define IFE_INTE_FRMEND                 0x00000001  ///< frame end interrupt
#define IFE_INTE_BP1                    0x00000002  ///< break point 1 interrupt
#define IFE_INTE_ROWDEFFAIL             0x00000004  ///< row def conc interrupt
#define IFE_INTE_STRPEND                0x00000008  ///< H stripe end interrupt
#define IFE_INTE_ALL                    0x0000000F  ///< all interrupts
//@}

/**
    Define IFE interrput status.
*/
//@{
#define IFE_INT_FRMEND                 0x00000001  ///< frame end interrupt
#define IFE_INT_BP1                    0x00000002  ///< break point 1 interrupt
#define IFE_INT_ROWDEFFAIL             0x00000004  ///< row def conc interrupt
#define IFE_INT_STRPEND                0x00000008  ///< H stripe end interrupt
#define IFE_INT_ALL                    0x0000000F  ///< all interrupts
//@}

/**
    IFE Function Selection.

    IFE Function Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_MODE_D2D = 0,       ///< Input from DRAM and output to DRAM
    IFE_MODE_EMBIPE = 1,       ///< Serve as an embedded function of IPE
    ENUM_DUMMY4WORD(_IFE_MODE)
}_IFE_MODE;
//@}

/**
    IFE Source Format.

    Set IFE input data format
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_FMT_NORMAL   = 0,           ///< Prograssive data input
    IFE_FMT_RAW      = 1,           ///< RAW data input
    ENUM_DUMMY4WORD(_IFE_SRCFMT)
}_IFE_SRCFMT;
//@}

/**
    IFE CFA Pattern Selection.

    Set IFE start pixel of the CFA pattern
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_CFA_R = 0,           ///< start pixel is R
    IFE_CFA_GR = 1,           ///< start pixel is GR
    IFE_CFA_GB = 2,           ///< start pixel is GB
    IFE_CFA_B = 3,           ///< start pixel is B
    ENUM_DUMMY4WORD(_IFE_CFASEL)
}_IFE_CFASEL;
//@}

/**
    IFE binning Selection.

    binning level selection
*/
//@{
typedef enum
{
    IFE_BINNING_DISABLE = 0,        ///< Disable binning
    IFE_BINNINGX2       = 1,        ///< binning x2
    IFE_BINNINGX4       = 2,        ///< binning x4
    IFE_BINNINGX8       = 3,        ///< binning x8
    IFE_BINNINGX16      = 4,        ///< binning x16
    IFE_BINNINGX32      = 5,        ///< binning x32
    IFE_BINNINGX64      = 6,        ///< binning x64
    IFE_BINNINGX128     = 7,        ///< binning x128
    ENUM_DUMMY4WORD(_IFE_BINNSEL)
}_IFE_BINNSEL;
//@}

/**
    IFE CFA Pattern Selection.

    Set IFE start pixel of the CFA pattern
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_SONLY_3x3 = 0,           ///< 3x3 spatial filter in 5x5 area
    IFE_SONLY_5x5 = 1,           ///< spatial filter only in 5x5 area
    ENUM_DUMMY4WORD(_IFE_SONLYLEN)
}_IFE_SONLYLEN;
//@}


/**
    IFE Delta of Curve Mapping Selection.

    Set IFE step size between two table entries.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_DELTA_1 = 0,           ///< step size = 1
    IFE_DELTA_2 = 1,           ///< step size = 2
    IFE_DELTA_4 = 2,           ///< step size = 4
    ENUM_DUMMY4WORD(_IFE_CRVMAP_DELTA)
}_IFE_CRVMAP_DELTA;
//@}

/**
    IFE Outlier Count Selection.

    Set IFE number of neighbours to establish outlier condition.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_EIGHT_NEIGHBOURS = 0,           ///< eight neighbours fall in the criteria
    IFE_SEVEN_NEIGHBOURS = 1,           ///< seven neighbours fall in the criteria
    ENUM_DUMMY4WORD(_IFE_OUTL_SEL)
}_IFE_OUTL_SEL;
//@}

/**
    IFE Row Defect Concealment Factor Selection.

    Set IFE factor in row defect concealment.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_FACT_p5 = 0,           ///< factor = 0.5
    IFE_FACT_p25 = 1,          ///< factor = 0.25
    ENUM_DUMMY4WORD(_IFE_ROWDEF_FACT)
}_IFE_ROWDEF_FACT;
//@}

/**
    IFE Wait Mode Selection.

    IFE Wait Mode Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    IFE_WAIT    = 0,                ///< Wait for frame end
    IFE_NOWAIT  = 1,                ///< Do not wait for frame end
    ENUM_DUMMY4WORD(_IFE_WAITMODE)
}_IFE_WAITMODE;
//@}

#if 0
/**
    IFE Bit Depth.

    Set IFE input/output bitdepth
*/
//@{
typedef enum
{
    IFE_8BIT    = 0,                ///< 8bit data
    IFE_12BIT   = 1,                ///< 12bit data
    IFE_16BIT   = 2,                ///< 16bit data
    ENUM_DUMMY4WORD(_IFE_BITDEPTH)
}_IFE_BITDEPTH;
//@}

/**
    IFE Spatial Filter Only Selection.

    Enable/Disable spatial filter only function
*/
//@{
typedef enum
{
    IFE_SONLY_DISABLE   = 0,        ///< Disable spatial filter only function
    IFE_SONLY_ENABLE    = 1,        ///< Enable spatial filter only function
    ENUM_DUMMY4WORD(_IFE_SONLYEN)
}_IFE_SONLYEN;
//@}

/**
    IFE Curve Mapping Selection.

    Enable/Disable curve mapping function
*/
//@{
typedef enum
{
    IFE_CRVMAP_DISABLE   = 0,       ///< Disable curve mapping function
    IFE_CRVMAP_ENABLE    = 1,       ///< Enable curve mapping function
    ENUM_DUMMY4WORD(_IFE_CRVMAPSEL)
}_IFE_CRVMAPSEL;
//@}
#endif

/**
    Struct IFE open object.

    ISR callback function
*/
//@{
typedef struct _IFE_OPENOBJ{
    void (*FP_IFEISR_CB)(UINT32 intstatus); ///< isr callback function
} IFE_OPENOBJ;
//@}

/**
    Struct IFE Range Filter Setting.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGESET{
    UINT32 *rngth;            ///< range filter threshold
    INT32 *rdelta;           ///< range filter threshold adjustment
} IFE_RANGESET;
//@}

/**
    Struct IFE Clamp and Weighting.

    Parameters of clamp and weight
*/
//@{
typedef struct _IFE_CLAMPWTSET{
    UINT32 clampTh;             ///< clamp threshold
    UINT32 wtMul_g;          ///< weighting multiplier for GR & GB channels
    UINT32 wtMul_rb;         ///< weighting multiplier for R & B channels
} IFE_CLAMPWTSET;
//@}

/**
    Struct IFE Curve Mapping.

    Parameters of curve mapping
*/
//@{
typedef struct _IFE_CRVMAPSET{
    _IFE_CRVMAP_DELTA crvmapDelta;  ///< curve mapping delta
    INT32 crvmapInOffs;             ///< input offset of curve mapping
    INT32 crvmapOutOffs;            ///< output offset of curve mapping
    UINT32 *crvmapGamma;            ///< curve mapping gamma
} IFE_CRVMAPSET;
//@}

/**
    Struct IFE Outlier Filter.

    Parameters of outlier filter
*/
//@{
typedef struct _IFE_OUTLSET{
    UINT32 outlBriTh;           ///< outlier threshold for bright points
    UINT32 outlDarkTh;          ///< outlier threshold for dark points
    _IFE_OUTL_SEL outlOp;       ///< outlier option for neighbour condition
} IFE_OUTLSET;
//@}

/**
    Struct IFE Row Defect Concealment.

    Parameters of row defect concealment
*/
//@{
typedef struct _IFE_ROWDEFSET{
    _IFE_ROWDEF_FACT rowDefFact;///< factor of row defect concealment
    UINT32 stidx;               ///< start index of row defect table
    UINT32 offs;                ///< offset of row defect table entry
    UINT32 *rowDefTbl;          ///< table of row defect concealment
} IFE_ROWDEFSET;
//@}

/**
    Struct IFE Filter Parameters.

    IFE filter detailed parameters.
*/
//@{
typedef struct _IFE_FILT_PARA{
    _IFE_SRCFMT  srcFmt;        ///< source format select
    _IFE_CFASEL cfaPat;         ///< CFA pattern
    BOOL rowDefEn;              ///< enable of row defect concealment
    BOOL outlEn;                ///< enable of outlier filter
    BOOL filterEn;              ///< enable of IFE filter
    BOOL crvmapEn;              ///< enable of curve mapping
    UINT32 *weights;            ///< spatial weighting
    IFE_RANGESET rngth_g;      ///< range filter threshold setting for GR & GB channels or progressive data
    IFE_RANGESET rngth_rb;     ///< range filter threshold setting for R & B channels
    _IFE_BINNSEL  binn;         ///< image binning
    BOOL sOnlyEn;               ///< spatial only enable
    _IFE_SONLYLEN  sOnlyLen;    ///< filter length of spatial only
    IFE_CLAMPWTSET clampWtSet; ///< clamp and weighting setting
    IFE_CRVMAPSET crvmapSet;   ///< curve mapping setting
    IFE_OUTLSET outlSet;       ///< outlier filter setting
    UINT32 rowDefVIni;         ///< vertical start of cropped window
    UINT32 *rowDefTbl;          ///< table of row defect concealment
    UINT32 repeat;             ///< repeat times
}IFE_FILT_PARA;
//@}

/**
    Struct IFE Information.

    IFE parameter and function selection

\n  Used for ife_setMode()
*/
//@{
typedef struct _IFE_PARAM
{
    _IFE_MODE mode;             ///< operation mode
    _IFE_SRCFMT  srcFmt;        ///< source format select
    _IFE_CFASEL cfaPat;         ///< CFA pattern
    BOOL rowDefEn;              ///< enable of row defect concealment
    BOOL outlEn;                ///< enable of outlier filter
    BOOL filterEn;              ///< enable of IFE filter
    BOOL crvmapEn;              ///< enable of curve mapping
    UINT32 *weights;            ///< spatial weighting
    IFE_RANGESET rngth_g;      ///< range filter threshold setting for GR & GB channels or progressive data
    IFE_RANGESET rngth_rb;     ///< range filter threshold setting for R & B channels
    _IFE_BINNSEL  binn;         ///< image binning
    BOOL sOnlyEn;               ///< spatial only enable
    _IFE_SONLYLEN  sOnlyLen;    ///< filter length of spatial only
    IFE_CLAMPWTSET clampWtSet; ///< clamp and weighting setting
    IFE_CRVMAPSET crvmapSet;   ///< curve mapping setting
    IFE_OUTLSET outlSet;       ///< outlier filter setting
    UINT32 rowDefVIni;         ///< vertical start of cropped window
    UINT32 *rowDefTbl;          ///< table of row defect concealment
    UINT32 in_addr;             ///< input starting address
    UINT32 out_addr;            ///< output starting address
    UINT32 width;               ///< image width
    UINT32 height;              ///< image height
    UINT32 ofsi;                ///< image input lineoffset
    UINT32 ofso;                ///< image output lineoffset
    UINT32 bp1;                 ///< break point 1
    UINT32 intrEn;              ///< interrupt enable
    BOOL upSrcEn;               ///< enable of upper source
    UINT32 up_addr;             ///< starting address of upper source
    BOOL downSrcEn;             ///< enable of down source
    UINT32 down_addr;           ///< starting address of down source
} IFE_PARAM;
//@}

extern ER       ife_setMode(IFE_PARAM *filt_info, _IFE_WAITMODE waitmode);
extern ID       ife_getLockStatus(void);
extern ER       ife_open(IFE_OPENOBJ *pObjCB);
extern BOOL     ife_isOpened(void);
extern ER       ife_close(void);
extern ER       ife_enableInt(UINT32 intr);
extern UINT32   ife_getIntStatus(void);
extern ER       ife_clearInt(UINT32 intr);
extern void     ife_waitFlagFrameEnd(void);
extern ER       ife_enableFunction(BOOL func_enable, UINT32 func);
extern BOOL     ife_checkFunctionEnable(UINT32 func);
//#NT#2011/05/20#Connie Yu -begin
//#NT#new API for SRA check
extern _IFE_MODE ife_checkOpMode(void);
extern BOOL ife_D2DisBusy(void);
//#NT#2011/05/20#Connie Yu -end
//#NT#2011/07/01#Connie Yu -begin
//#NT#new api for dynamically changing para
extern void ife_changeParam(IFE_FILT_PARA *para);
//#NT#2011/07/01#Connie Yu -end

#endif

//@}
