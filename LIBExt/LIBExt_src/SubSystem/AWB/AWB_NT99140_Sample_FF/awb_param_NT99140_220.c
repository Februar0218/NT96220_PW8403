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
#include "Awb_NT99140_220_int.h"

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
    {AWBALG_TYPE_Y,      170,  20},
    {AWBALG_TYPE_RG,     180,  78},
    {AWBALG_TYPE_BG,     215, 100},
    {AWBALG_TYPE_RBG,    256,  65},
    {AWBALG_TYPE_SAT,     45,  13},
    {AWBALG_TYPE_RBGSUM, 352, 250},
    {AWBALG_TYPE_RXBG,   260, 150},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,      170,  20},
    {AWBALG_TYPE_RG,     180, 120},
    {AWBALG_TYPE_BG,     160, 100},
    {AWBALG_TYPE_RBG,    130,  65},
    {AWBALG_TYPE_SAT,     29,  13},
    {AWBALG_TYPE_RBGSUM, 352, 250},
    {AWBALG_TYPE_RXBG,   260, 165},
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


