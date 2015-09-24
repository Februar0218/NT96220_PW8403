/**
    uITRON Error Code values. The error number of the return status.

    @file       ErrorNo.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _ERRORNO_H
#define _ERRORNO_H

/**
    @addtogroup mIKernelCore
*/
//@{

/**
    @name   uITRON error code value

    Definition for uITRON error code value
*/
//@{
#define E_OK                0           ///< Normal termination
#define E_SYS               (-5)        ///< System error
#define E_NOMEM             (-10)       ///< No system memory
#define E_NOSPT             (-17)       ///< Unsupported function
#define E_INOSPT            (-18)       ///< Function not supported in ITRON/FILE specification
#define E_RSFN              (-20)       ///< Reserved function code number
#define E_RSATR             (-24)       ///< Reserved attribute
#define E_PAR               (-33)       ///< Parameter error
#define E_IDOVR             (-35)       ///< Outside ID number range
#define E_VECN              (-42)       ///< Illegal Vector number
#define E_EXS               (-51)       ///< Object already exists
#define E_NOEXS             (-52)       ///< Object does not yet exist
#define E_BOVR              (-58)       ///< Buffer Overflow
#define E_OBJ               (-63)       ///< Other errors related to object status
#define E_MACV              (-65)       ///< Memory access violation
#define E_OACV              (-66)       ///< Object or handler access violation
#define E_CTX               (-69)       ///< Context error
#define E_QOVR              (-73)       ///< Queue overflow
#define E_DLT               (-81)       ///< Wait object deleted
#define E_TMOUT             (-85)       ///< Time-out
#define E_RLWAI             (-86)       ///< Forced release wait
//@}

//#NT#2010/03/03#Elvis Chuang - begin
//Please refer to the c-runtime library assert.h
#ifndef __assert_h
#  define __assert_h
#  ifdef __cplusplus
      extern "C" void __assert(const char *, const char *, int);
#  else
      extern void __assert(const char *, const char *, int);
#  endif
#else
#  undef ASSERT
#endif

#ifdef USING_ASSERT
#  ifdef __STDC__
#    define ASSERT(e) ((e) ? (void)0 : __assert(#e, __func__, __LINE__))
#  else
#    define ASSERT(e) ((e) ? (void)0 : __assert("e", __func__, __LINE__))
#  endif
#else
#  define ASSERT(ignore) ((void)0)
#endif
//#NT#2010/03/03#Elvis Chuang - end
//@}

#endif
