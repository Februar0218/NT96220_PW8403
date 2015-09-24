/**
    Auto Exposure parameter.

    ae parameter.

    @file       AWB_OV5633_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "Awb_alg.h"
#include "Awb_OV5633_220_int.h"

#define GRAYWORLD DISABLE

#if (GRAYWORLD == ENABLE)

AWBALG_ELEMET element1[] =
{
    {AWBALG_TYPE_Y,   0xffffffff,   0},
    {AWBALG_TAB_END,           0,   0}
};

AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,   0xffffffff,   0},
    {AWBALG_TAB_END,           0,   0}
};


AWBALG_TAB AwbTAB[AWB_TAB_CNT] =
{
    {((1 << 9) * 10), ((1 << 1) * 10), element1},
    {((1 << 20) * 10), ((1 << 9) * 10), element2}
};


#else

AWBALG_ELEMET element1[] =
{
    {AWBALG_TYPE_Y,      185,  12},
    {AWBALG_TYPE_RG,     165,  70},
    {AWBALG_TYPE_BG,     180,  90},
    {AWBALG_TYPE_RBG,    220,  55},
    {AWBALG_TYPE_SAT,     36,  6},
    {AWBALG_TYPE_RBGSUM, 300, 220},
    {AWBALG_TYPE_RXBG,   200,  120},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,      185,  12},
    {AWBALG_TYPE_RG,     165,  120},
    {AWBALG_TYPE_BG,     140,  90},
    {AWBALG_TYPE_RBG,    100,  55},
    {AWBALG_TYPE_SAT,     18,  6},
    {AWBALG_TYPE_RBGSUM, 300, 228},
    {AWBALG_TYPE_RXBG,   220,  140},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_TAB AwbTAB[AWB_TAB_CNT] =
{
    {((1 << 10) * 10), ((1 << 4) * 10), element1},
    {((1 << 17) * 10), ((1 << 10) * 10), element2}
};
#endif

const UINT32 MwbTAB[AWB_MODE_MAX][2] =
{
    { 256, 256},
    { 370, 310},//AWB_MODE_DAYLIGHT
    { 410, 260},//AWB_MODE_CLOUDY
    { 235, 420},//AWB_MODE_TUNGSTEN
    { 400, 300},//AWB_MODE_FLUORESCENT1
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
};


