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
#include "Awb_MI5100_int.h"

#define GRAYWORLD DISABLE//ENABLE

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
/*    {AWBALG_TYPE_Y,      200,  20},
    {AWBALG_TYPE_RG,     150, 80},
    {AWBALG_TYPE_BG,     150, 80},
    {AWBALG_TYPE_RBG,    150, 95},
    {AWBALG_TYPE_SAT,    50, 0},
    {AWBALG_TYPE_RBGSUM, 210, 190},
    {AWBALG_TYPE_RXBG,   120, 80},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
*/

    // before 2012/06/16
    {AWBALG_TYPE_Y,      185,  9},
    {AWBALG_TYPE_RG,     185, 70},
    {AWBALG_TYPE_BG,     165, 75},//160, 75},//149, 100},
    {AWBALG_TYPE_RBG,    200, 68},
    {AWBALG_TYPE_SAT,    65, 16},
    {AWBALG_TYPE_RBGSUM, 310, 100},
    {AWBALG_TYPE_RXBG,   243, 100},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}

};

AWBALG_ELEMET element2[] =
{
/*    {AWBALG_TYPE_Y,       200, 20},
    {AWBALG_TYPE_RG,     105, 95},
    {AWBALG_TYPE_BG,     105, 95},
    {AWBALG_TYPE_RBG,    110, 90},
    {AWBALG_TYPE_SAT,    50, 0},
    {AWBALG_TYPE_RBGSUM, 210, 190},
    {AWBALG_TYPE_RXBG,   110, 90},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
*/
    //before 2012/06/16
    {AWBALG_TYPE_Y,       200, 15},       // 100
    {AWBALG_TYPE_RG,     181, 100},    // 110
    {AWBALG_TYPE_BG,     158, 100},//148, 100},    // 120
    {AWBALG_TYPE_RBG,    183, 63},     //130
    {AWBALG_TYPE_SAT,    50, 10},      // 140
    {AWBALG_TYPE_RBGSUM, 322, 100}, // 150
    {AWBALG_TYPE_RXBG,   256, 100},   // 160
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


