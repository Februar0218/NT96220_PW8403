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
#include "Awb_MI5140_220_int.h"

#define GRAYWORLD DISABLE//ENABLE//


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

AWBALG_ELEMET element3[] =
{
    {AWBALG_TYPE_Y,   0xffffffff,   0},
    {AWBALG_TAB_END,           0,   0}
};

AWBALG_TAB AwbTAB[AWB_TAB_CNT] =
{
    {((1 << 10) * 10), ((1 << 1) * 10), element1},
    //{((1 << 10) * 10), ((1 << 8) * 10), element2},
    {((1 << 20) * 10), ((1 << 10) * 10), element2}    
};


#else

/*
AWBALG_ELEMET element1[] =
{
    {AWBALG_TYPE_Y,      200,  15},
    {AWBALG_TYPE_RG,     194,  80},
    {AWBALG_TYPE_BG,     185, 89},
    {AWBALG_TYPE_RBG,    154,  83},
    {AWBALG_TYPE_SAT,     33,  3},
    {AWBALG_TYPE_RBGSUM, 382, 200},
    {AWBALG_TYPE_RXBG,   220, 88},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,      195,  24},
    {AWBALG_TYPE_RG,     174, 85},
    {AWBALG_TYPE_BG,     168, 91},
    {AWBALG_TYPE_RBG,    154,  82},
    {AWBALG_TYPE_SAT,     31,  3},
    {AWBALG_TYPE_RBGSUM, 380, 200},
    {AWBALG_TYPE_RXBG,   240, 88},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};
*/
AWBALG_ELEMET element1[] =
{
    {AWBALG_TYPE_Y,      195,  15},
    {AWBALG_TYPE_RG,     190,  87},
    {AWBALG_TYPE_BG,     220, 60},
    {AWBALG_TYPE_RBG,    321,  53},
    {AWBALG_TYPE_SAT,     54,  0},
    {AWBALG_TYPE_RBGSUM, 382, 98},
    {AWBALG_TYPE_RXBG,   340, 70},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,      195,  15},
    {AWBALG_TYPE_RG,     190, 80},
    {AWBALG_TYPE_BG,     136, 80},
    {AWBALG_TYPE_RBG,    153,  52},
    {AWBALG_TYPE_SAT,     54,  0},
    {AWBALG_TYPE_RBGSUM, 340, 89},
    {AWBALG_TYPE_RXBG,   330, 61},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_ELEMET element3[] =
{
    {AWBALG_TYPE_Y,      185,  24},
    {AWBALG_TYPE_RG,     160, 88},
    {AWBALG_TYPE_BG,     156, 80},
    {AWBALG_TYPE_RBG,    133,  52},
    {AWBALG_TYPE_SAT,     31,  3},
    {AWBALG_TYPE_RBGSUM, 310, 208},
    {AWBALG_TYPE_RXBG,   240, 101},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
};

AWBALG_TAB AwbTAB[AWB_TAB_CNT] =
{
    {((1 << 10) * 10), ((1 << 4) * 10), element1},
    {((1 << 17) * 10), ((1 << 10) * 10), element2}//,        
    //{((1 << 17) * 10), ((1 << 10) * 10), element3}
};
#endif

const UINT32 MwbTAB[AWB_MODE_MAX][2] =
{
    { 256, 256},
    { 300, 277},//AWB_MODE_DAYLIGHT
    { 277, 309},//AWB_MODE_CLOUDY
    { 216, 446},//{ (197*12/10), (583*83/100)},//AWB_MODE_TUNGSTEN
    { 257, 361},//AWB_MODE_FLUORESCENT1
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
};


