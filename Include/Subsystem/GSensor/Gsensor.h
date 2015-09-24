/**
    Header file for Gsensor module

    This file is the header file that define the API and data type
    for Gsensor module.

    @file       Gsensor.h
    @ingroup    mISYSGsensor
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _GSENSOR_H
#define _GSENSOR_H

#include "Type.h"
#include "I2C.h"
#include "string.h"
#include "debug.h"
#include "OSCommon.h"

/**
    @addtogroup mISYSGsensor
*/
//@{

#define GSENSOR_ERR     1
#if GSENSOR_ERR
#define GSENSOR_err(...)    debug_msg("GSensor:"__VA_ARGS__)
#else
#define GSENSOR_err(...)
#endif

#define GSENSOR_DBG     1
#if GSENSOR_DBG
#define GSENSOR_dbg(EXP,...)    if (!(EXP)) debug_msg("GSensor:"__VA_ARGS__)
#else
#define GSENSOR_dbg(EXP,...)
#endif

#define FLGGS_IDLE      (0x00000001)
#define FLGGS_INT       (0x00000002)

// for axis data
typedef enum
{
    GSENSOR_AXIS_X,
    GSENSOR_AXIS_Y,
    GSENSOR_AXIS_Z,
    ENUM_DUMMY4WORD(GSENSOR_AXIS)
} GSENSOR_AXIS;


// for get status
typedef enum
{
    GSENSOR_ORIENT_0,
    GSENSOR_ORIENT_90,
    GSENSOR_ORIENT_180,
    GSENSOR_ORIENT_270,
    ENUM_DUMMY4WORD(GSENSOR_ORIENT)
} GSENSOR_ORIENT;

typedef enum
{
    GSENSOR_SHAKE_LEFT,
    GSENSOR_SHAKE_RIGHT,
    ENUM_DUMMY4WORD(GSENSOR_SHAKE)
} GSENSOR_SHAKE;

typedef struct
{
   UINT32      Xacc;
   UINT32      Yacc;
   UINT32      Zacc;
} AXIS_DATA;

// for get status
typedef enum
{
    ORIENT_0,
    ORIENT_90,
    ORIENT_180,
    ORIENT_270,
    ENUM_DUMMY4WORD(ORIENT)
} ORIENT;

typedef enum
{
    SHAKE_L,
    SHAKE_R,
    ENUM_DUMMY4WORD(SHAKE)
} SHAKE;

typedef struct {
    AXIS_DATA   Axis;
    ORIENT      Ori;
    SHAKE       SH;
} Gsensor_Data,*pGsensor_Data;

// for set feature
// 1. orientation detect time interval
// 2. shake detect time interval
// 3. shake threshold
typedef enum
{
    GSENSOR_FEATURE_ORIENT,
    GSENSOR_FEATURE_SHAKE_TIME,
    GSENSOR_FEATURE_SHAKE_THRESHOLD,
    ENUM_DUMMY4WORD(GSENSOR_FEATURE)
} GSENSOR_FEATURE;

typedef struct
{
    BOOL        (*GsensorInit)(void);
    void        (*GetGsensorData)(Gsensor_Data *GS_Data);
    void        (*GetGsensorStatus)(UINT32 status,UINT32 *uiData);
    void        (*GsensorOpenInterface)(void);
    void        (*GsensorCloseInterface)(void);
} GSENSOR_FUNC, *PGSENSOR_FUNC;

typedef struct
{
    UINT32      RegAddr;         //Register address
    UINT32      Data;            //Data
}GS_I2C_CMD;


extern BOOL gsensor_open(void);
extern BOOL gsensor_close(void);
extern BOOL gsensor_Init(void);
extern void gsensor_GetData(Gsensor_Data *GS_Data);
extern void gsensor_GetStatus(UINT32 status,UINT32 *uiData);
extern void gsensor_getFP(PGSENSOR_FUNC pGsensorFunc);
extern void gsensor_OpenInterface(void);
extern void gsensor_CloseInterface(void);
extern void LIS331DL_WriteData(UINT32 *buf);
extern void LIS331DL_ReadData(UINT32 *buf);

//@}

#endif
