/*
    Auto White Balance Parameter

    @Sensor     Micron MI3130
    @file       awb_param_MI3130_220.c
    @ingroup        AWB_MI3130_FDHN_FF
    @note       Nothing.

    Porting by Randy Ho     2012/03/08

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#include "type.h"
#include "Awb_alg.h"
#include "Awb_MI3130_220_int.h"

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

// element1: EV4-10
AWBALG_ELEMET element1[] =
{
    {AWBALG_TYPE_Y,      200,  20},
    {AWBALG_TYPE_RG,     155,  84},
    {AWBALG_TYPE_BG,     168,  82},
    {AWBALG_TYPE_RBG,    145,  74},
    {AWBALG_TYPE_SAT,     50,  1},
    {AWBALG_TYPE_RBGSUM, 285, 171},
    {AWBALG_TYPE_RXBG,   198, 86},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}

/*
    // Calcuted by Gray World data [NTK#RandyHo]<2012/06/28>
    {AWBALG_TYPE_Y,      200,  20},
    {AWBALG_TYPE_RG,     121,  84},
    {AWBALG_TYPE_BG,     113,  82},
    {AWBALG_TYPE_RBG,    113,  74},
    {AWBALG_TYPE_SAT,     20,  0},
    {AWBALG_TYPE_RBGSUM, 227, 171},
    {AWBALG_TYPE_RXBG,   117, 86},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
*/

 /*
    {AWBALG_TYPE_Y,      200,  24},
    {AWBALG_TYPE_RG,     168,  108}, // 145, 83
    {AWBALG_TYPE_BG,     195, 92}, // 113, 88
    {AWBALG_TYPE_RBG,    220,  76}, // 116
    {AWBALG_TYPE_SAT,     38,  2},
    {AWBALG_TYPE_RBGSUM, 325, 175}, // 325 // 235,177
    {AWBALG_TYPE_RXBG,   230, 87}, // 200 // 125, 87
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
   */
};

// element2: EV10-EV17
AWBALG_ELEMET element2[] =
{
    {AWBALG_TYPE_Y,      200,  20},
    {AWBALG_TYPE_RG,     154,  90},
    {AWBALG_TYPE_BG,     160,  86},
    {AWBALG_TYPE_RBG,    142,  72},
    {AWBALG_TYPE_SAT,     50,  3},
    {AWBALG_TYPE_RBGSUM, 278, 186},
    {AWBALG_TYPE_RXBG,   190, 88},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}

/*
    // Calcuted by Gray World data [NTK#RandyHo]<2012/06/28>
    {AWBALG_TYPE_Y,      200,  20},
    {AWBALG_TYPE_RG,     123,  90},
    {AWBALG_TYPE_BG,     109,  86},
    {AWBALG_TYPE_RBG,    102,  72},
    {AWBALG_TYPE_SAT,     50,  3},
    {AWBALG_TYPE_RBGSUM, 229, 184},
    {AWBALG_TYPE_RXBG,   119, 88},
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
*/

/*
    // This group of settings are created at NTK office, 2012/05/23, checked at ADPK office in 2012/05/24
    {AWBALG_TYPE_Y,      200,  24},
    {AWBALG_TYPE_RG,     168, 80},  // 163 // 126, 95
    {AWBALG_TYPE_BG,     140, 80},  // 107, 87
    {AWBALG_TYPE_RBG,    110,  65}, // 102, 71
    {AWBALG_TYPE_SAT,     35,  3},  // 13
    {AWBALG_TYPE_RBGSUM, 305, 175}, // 290// 226, 180
    {AWBALG_TYPE_RXBG,   210, 85},  // 200 // 114,92
    {AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}

*/
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
    { 330, 304},//AWB_MODE_DAYLIGHT
    { 310, 332},//AWB_MODE_CLOUDY
    { 274, 375},//AWB_MODE_TUNGSTEN
    { 300, 354},//AWB_MODE_FLUORESCENT1
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}

/*  // before 2012/05/23    [NTK#RandyHo]
    { 256, 256},
    { 442, 308},//AWB_MODE_DAYLIGHT
    { 405, 340},//AWB_MODE_CLOUDY
    { (197*12/10), (583*83/100)},//AWB_MODE_TUNGSTEN
    { 327, 418},//AWB_MODE_FLUORESCENT1
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
*/
};

