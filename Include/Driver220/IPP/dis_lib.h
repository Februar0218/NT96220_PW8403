/**
    Public header file for DIS module.

    @file       dis_lib.h
    @ingroup    mIIPPDIS

    @brief

    How to start:\n
    [Movie]\n
    1. dis_attach\n
    2. dis_open\n
    3. DIS_operation = DIS_ENGINE_SET2PRV, dis_setMode\n
    4. DIS_operation = DIS_ENGINE_SET2RUN, dis_setMode\n


    How to stop:\n
    1. DIS_operation = DIS_ENGINE_SET2PAUSE, dis_setMode\n
    2. dis_close\n
    3. dis_detach\n


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _DIS_LIB_H
#define _DIS_LIB_H

#include "Type.h"

/**
    @addtogroup mIIPPDIS
*/
//@{


#define MAXMDSNUM   16

/*
    Define DIS interrput enable.
*/
//@{
#define DIS_INT_FRM                    0x00000001    ///< interrupt enablestatus: frame end
#define DIS_INT_MDS                    0x00000002    //bit 1
#define DIS_INT_BLK                    0x00000004    //bit 2
#define DIS_INT_ALL                    0x00000007
//@}

// Structure definition
/**
    DIS Engine Operation Selection.

    Select DIS module operation.
*/
//@{
typedef enum
{
    DIS_ENGINE_OPEN = 0, ///< open module
    DIS_ENGINE_CLOSE = 1, ///< close module
    DIS_ENGINE_SET2READY = 2, ///< set module to ready state
    DIS_ENGINE_SET2PRV = 3, ///< set module to preview mode
    //DIS_ENGINE_SET2CAP = 4,
    DIS_ENGINE_SET2PAUSE = 5, ///< pause module
    DIS_ENGINE_SET2RUN = 6, ///< set module to run
    DIS_ENGINE_ATTACH = 7, ///< attach module (enable clock)
    DIS_ENGINE_DETACH = 8, ///< detach module (disable clock)
    ENUM_DUMMY4WORD(DIS_ENGINE_OPERATION)
} DIS_ENGINE_OPERATION;
//@}

typedef enum {
    DIS_IPPMODE_Preview = 0,
    DIS_IPPMODE_VideoRec = 1,
    ENUM_DUMMY4WORD(DIS_MODE)
} DIS_MODE;

/**
    DIS Process Event Selection.

    Select DIS process event. Event control is carried out by PhotoDisTsk
*/
//@{
typedef enum
{
    DIS_PROC_RESTART = 0, ///< process restart
    DIS_PROC_UPDATE = 1, ///< process update display information
    DIS_PROC_PAUSE = 2, ///< process pause
    DIS_PROC_NOOP = 3, ///< no operation
    ENUM_DUMMY4WORD(DIS_PROC_EVENT)
} DIS_PROC_EVENT;
//@}

/**
    DIS Adaptive Threshold Operation Selection.

    Select DIS threshold to increase or to decrease.
*/
//@{
typedef enum
{
    DIS_ADATH_INCREASE = 0, ///< increase threshold
    DIS_ADATH_DECREASE = 1, ///< decrease threhold
    DIS_ADATH_NOOP = 2, ///< keep threshold
    ENUM_DUMMY4WORD(DIS_ADATH_OP)
} DIS_ADATH_OP;
//@}

/**
    DIS Open Object.

    Open Object is used for opening DIS module.
    \n Used for dis_open()

*/
//@{
typedef struct _DIS_OPENOBJ{
    void (*FP_DISISR_CB)(UINT32 intstatus); ///< isr callback function
} DIS_OPENOBJ;
//@}

/**
    DIS Motion Information.

    Structure of information in a motion vector.
*/
//@{
typedef struct _MOTION_INFOR{
    INT32 x;  ///< x component
    INT32 y;  ///< y component
    UINT32 sad;   ///< sum of absolute difference
    INT32 cnt; ///< number of edge pixels (src block)
    BOOL valid;  ///< reliability of MV
}MOTION_INFOR;
//@}

/**
    DIS Motion Vector.

    Structure of directional motion vector elements.
*/
//@{
typedef struct _MOTION_VECTOR{
    INT32 x; ///< x component
    INT32 y; ///< y component
}MOTION_VECTOR;
//@}

/**
    DIS Vector.

    Structure of non-directional vector elements.
*/
//@{
typedef struct _VECTOR{
    UINT32 x; ///< x component
    UINT32 y; ///< y component
}VECTOR;
//@}

/**
    DIS Compensation Information

    Structure of compensation parameters.
*/
//@{
typedef struct _COMPENSATION_INFO{
    UINT32 boundry_hori; ///< horizontal boundary
    UINT32 boundry_vert; ///< vertical boundary
    UINT8 dzoomRate; ///< dzoom rate
}COMPENSATION_INFO;
//@}

/**
    DIS Entire Configuration

    Structure of DIS module parameters when one wants to configure this module.
*/
//@{
typedef struct _DIS_PARAM_STRUCT
{
    DIS_ENGINE_OPERATION DIS_operation; ///< DIS engine operation
    UINT32 InSizeH; ///< input horizontal size
    UINT32 InSizeV; ///< input vertical size
    UINT32 InLineOfs; ///< input line offset
    UINT32 InAdd0; ///< input starting address 0
    UINT32 InAdd1; ///< input starting address 1
    UINT32 InAdd2; ///< input starting address 2
    UINT32 OutAdd0; ///< output starting address 0
    UINT32 OutAdd1; ///< output starting address 1
    UINT32 intEn; ///< interrupt enable switch
} DIS_PARAM;
//@}

//KH mark
#if 0
extern void dis_attach(void);
extern void dis_detach(void);
extern ER dis_open(DIS_OPENOBJ *pObjCB);
extern ER dis_close(void);
extern ER dis_lock(void);
extern ER dis_unlock(void);
extern ER dis_start(void);
extern ER dis_pause(void);
extern void dis_isr(void);
extern ER dis_setMode(DIS_PARAM *DISaddrSize);
extern void dis_load(void);
extern BOOL dis_isEnabled(void);
extern void dis_clrFrameEnd(void);
extern void dis_waitFrameEnd(BOOL bClrFlag);
extern UINT dis_getCurOppbID(void);
//interrupt
extern void dis_enableInt(BOOL Int_enable, UINT32 dis_int_mask);
extern UINT32 dis_getIntrStatus(void);
extern void dis_clrIntrStatus(UINT32 status);
//motion information
extern void dis_getMotionResults(MOTION_INFOR *MotResult);
extern void dis_getVideoIMV(MOTION_VECTOR *intMV, MOTION_INFOR *disMV, COMPENSATION_INFO comp);
extern DIS_ADATH_OP dis_checkAdaptiveThres(MOTION_INFOR *MotVec, UINT32 decLev, UINT32 incLev);
#endif
//@}

#endif
