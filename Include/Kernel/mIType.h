/**
    uITRON 2.0 standard data type.

    @file       mIType.h
    @ingroup    mIKernelCore
    @note       This include file only contain uITRON related data type definition.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _MITYPE_H
#define _MITYPE_H

// Check if the ARM CC is latest version
#if __arm
    #if (__ARMCC_VERSION != 120848)
        #error ARM CC version is not 1.2 build 848
    #endif
#endif

/**
    @addtogroup mIKernelCore
*/
//@{

/**
    @name Standard data type for uITRON 2.0.
*/
//@{
typedef char                B;              ///< Signed 8-bit integer
typedef short               H;              ///< Signed 16-bit integer
typedef long                W;              ///< Signed 32-bit integer
typedef unsigned char       UB;             ///< Unsigned 8-bit integer
typedef unsigned short      UH;             ///< Unsigned 16-bit integer
typedef unsigned long       UW;             ///< Unsigned 32-bit integer
typedef long                VW;             ///< Variable data type (32 bits)
typedef short               VH;             ///< Variable data type (16 bits)
typedef char                VB;             ///< Variable data type (8 bits)


typedef void                *VP;            ///< Pointer to unspecified data type
typedef void                (*FP)(void);    ///< Program start address

typedef W                   BOOL;           ///< Boolean
typedef int                 INT;            ///< Not in v2.0. Borrow the name from uITRON 3.0 and later
typedef unsigned int        UINT;           ///< Not in v2.0. Borrow the name from uITRON 3.0 and later

typedef W                   ID;             ///< Object ID number (XXXid)
typedef W                   ER;             ///< Error code
typedef UB                  PRI;            ///< Borrow the name from uITRON 3.0 and later
typedef UB                  ATR;            ///< Object attribute
typedef W                   TMO;            ///< Time-out
typedef UW                  HNO;            ///< Handler number
//@}

typedef TMO                 CYCTIME;
typedef TMO                 ALMTIME;
//#NT#2011/07/07#Elvis Chuang -begin
//#NT# Modify the FLGPTN definition
#define FLGPTN_TYPE_32      0
#define FLGPTN_TYPE_64      1
#define FLGPTN_TYPE         FLGPTN_TYPE_32

#if (FLGPTN_TYPE == FLGPTN_TYPE_32)
// 32 bits flag patterns
typedef unsigned int         FLGPTN;                        ///< Flag patterns
typedef unsigned int         *PFLGPTN;                      ///< Flag patterns (Pointer)
#define FLGPTN_BIT(n)       ((FLGPTN)((FLGPTN)0x1 << (n)))  ///< Bit of flag pattern
#define FLGPTN_BIT_ALL      ((FLGPTN)0xFFFFFFFF)
#elif (FLGPTN_TYPE == FLGPTN_TYPE_64)
// 64 bits flag patterns
typedef unsigned long long  FLGPTN;                     ///< Flag patterns
typedef unsigned long long  *PFLGPTN;                   ///< Flag patterns (Pointer)
#define FLGPTN_BIT(n)       ((FLGPTN)(1ull << (n)))     ///< Bit of flag pattern
#define FLGPTN_BIT_ALL      ((FLGPTN)0xFFFFFFFFFFFFFFFFull)
#else
    #error Incorrect flag pattern type
#endif
//#NT#2011/07/07#Elvis Chuang -end

// NULL
#ifndef NULL
#define NULL        ((void*)0)
#endif

// For BOOL type
#define FALSE               0
#define TRUE                1

/**
    Task entry
*/
typedef struct t_ctsk
{
    UB      tskid;          ///< Task ID
    UB      itskpri;        ///< Task Priority
    UH      stksz;          ///< Task stack size
    FP      task;           ///< Task function point
    VP      istack;         ///< Pointer to task stack
} T_CTSK;

/**
    Semaphore entry
*/
typedef struct t_csem
{
    UB      semid;          ///< Semaphore ID
    ATR     sematr;         ///< Semaphore attribute
                            ///< @note  Don't care now!
    UB      isemcnt;        ///< Semaphore counter
    UB      maxsem;         ///< Semaphore Max counter value
} T_CSEM;

/**
    Message entry
*/
typedef struct t_msg
{
    PRI     msgpri;
    VB      data[3];
    UINT    param1;
    UINT    param2;
    UINT    param3;
} T_MSG;

/**

*/
typedef struct t_cmpf
{
    UINT    mpfid;          ///< Fixed memory pool ID
    UINT    blfsz;          ///< Fixed memory pool size
    UINT    mpfcnt;         ///< Fixed memory pool counter
} T_CMPF;

//@}

#endif
