/**
    Auto Exposure parameter.

    ae parameter.

    @file       ae_sample_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "type.h"
#include "Awb_alg.h"
#include "Awb_sample_int.h"

#define GRAYWORLD ENABLE

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
    {AWBALG_TYPE_Y,      170,  20},
    {AWBALG_TYPE_RG,     200, 100},
    {AWBALG_TYPE_BG,     200, 100},
    {AWBALG_TYPE_RBG,    200, 100},
    {AWBALG_TYPE_SAT,    200, 100},
    {AWBALG_TYPE_RBGSUM, 200, 100},
    {AWBALG_TYPE_RXBG,   200, 100},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,       170, 20},
    {AWBALG_TYPE_RG,     200, 100},
    {AWBALG_TYPE_BG,     200, 100},
    {AWBALG_TYPE_RBG,    200, 100},
    {AWBALG_TYPE_SAT,    200, 100},
    {AWBALG_TYPE_RBGSUM, 200, 100},
    {AWBALG_TYPE_RXBG,   200, 100},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_TAB AwbTAB[AWB_TAB_CNT] =
{
    {((1 << 9) * 10), ((1 << 4) * 10), element1},
    {((1 << 17) * 10), ((1 << 9) * 10), element2}
};
#endif



const UINT32 MwbTAB[AWB_MODE_MAX][2] =
{
    { 256, 256},
    { 400, 320},
    { 336, 332},
    { 276, 400},
    { 340, 336},
    { 340, 336},
    { 417, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
};


