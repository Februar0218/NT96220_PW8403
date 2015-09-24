/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       AF_Lib.h
    @ingroup    mISYSLens

    @brief      auto focus function header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/12
*/
#ifndef _AF_LIB_H
#define _AF_LIB_H

#include "Type.h"

typedef enum {
    AF_STA_OK,
    AF_STA_FAIL,
    AF_STA_TIMEOUT,
    AF_STA_TOO_DARK
} _AF_STATUS_;

//#NT#2012/08/06#KS Hung -begin
//#NT#Add the callback function of Detecting GPIO
typedef BOOL (*FPAF_DETGPIOKEY)(void);
//#NT#2012/08/06#KS Hung -end
typedef UINT32 (*FPCALLBACK_GETVA)(UINT8);

//#NT#2012/06/06#KS Hung -begin
#define AF_FOCUS_ARRAY_SIZE     48
//#NT#2012/06/06#KS Hung -end
#define AF_FOCUS_TOTAL_WIN      9

typedef struct {
    INT32      StartIdx;
    INT32      EndIdx;
    INT32      MaxIdx;
    UINT32     MaxVA;
    UINT32     AF_time;
    INT32      AF_GuessIdx;
    INT32      Step[AF_FOCUS_ARRAY_SIZE];
    UINT32     Value[AF_FOCUS_ARRAY_SIZE];
    UINT32     VACnt[AF_FOCUS_ARRAY_SIZE];
    UINT32     AF_Window;
    UINT32     MultiValue[AF_FOCUS_TOTAL_WIN][AF_FOCUS_ARRAY_SIZE];
} AF_INFO, *PAF_INFO;

typedef struct {
    BOOL        bEnable;
    UINT32      Step[AF_FOCUS_ARRAY_SIZE];
    UINT32      Value[AF_FOCUS_ARRAY_SIZE];
} AF_GUESS_INFO, *PAF_GUESS_INFO;

extern void     AF_RegGetVariationCB(FPCALLBACK_GETVA getVariation);
extern void     AF_ResetAFInfo(void);
extern INT32    AF_SearchFullRange(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA);
extern INT32    AF_Process_ST2089(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA);
extern INT32    AF_Process_ST8005(UINT32 startIdx, UINT32 endIdx);
extern INT32    AF_Process_S5K3E2(UINT32 startIdx, UINT32 endIdx, UINT32 jumpStep, UINT32 *pMaxVA);
//#NT#2012/06/01#KS Hung -begin
//#NT#Add the callback function of Detecting GPIO
extern void     AF_RegisterFP_DetGPIOKey(FPAF_DETGPIOKEY pafDetGPIOKey);
//#NT#2012/06/01#KS Hung -end

#endif
