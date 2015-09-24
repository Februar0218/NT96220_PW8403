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
#include "Awb_H22_220_int.h"

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
    {AWBALG_TYPE_Y,      170,  15},
    {AWBALG_TYPE_RG,     240,  98},//    {AWBALG_TYPE_RG,     180,  78},
    {AWBALG_TYPE_BG,     210, 112},//    {AWBALG_TYPE_BG,     238, 124},
    {AWBALG_TYPE_RBG,    256,  70},
    {AWBALG_TYPE_SAT,     45,  13},
    {AWBALG_TYPE_RBGSUM, 372, 264},
    {AWBALG_TYPE_RXBG,   338, 160},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,      170,  20},
    {AWBALG_TYPE_RG,     198,98},//178, 98},
    {AWBALG_TYPE_BG,     210, 120},//178, 124},
    {AWBALG_TYPE_RBG,    132,  70},
    {AWBALG_TYPE_SAT,     29,  16},
    {AWBALG_TYPE_RBGSUM, 330, 273},
    {AWBALG_TYPE_RXBG,   269, 186},
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
    //#NTK#2012/01/31#Robin Chen begin
    { 352, 349},    //Daylight
    { 332, 369},    //Cloudy   (7470K+5180K)/2
    { 215, 531},    //Tungsten
    { 275, 484},    //Fluorescent
    //#NTK#2012/01/31#Robin Chen end
    { 340, 336},
    { 417, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
};


