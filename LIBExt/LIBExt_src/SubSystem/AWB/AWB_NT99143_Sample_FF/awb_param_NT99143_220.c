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
#include "Awb_NT99143_220_int.h"

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
#if 0
    {AE_MIN_EV, ((1 << 1) * 10), element1},
    {((1 << 20) * 10), AE_MIN_EV, element2}
#endif	
#if 1	
    {((1 << 9) * 10), ((1 << 1) * 10), element1},
    {((1 << 20) * 10), ((1 << 9) * 10), element2}
#endif	
};


#else

AWBALG_ELEMET element1[] =
{
#if 0
    {AWBALG_TYPE_Y,      1023,   0},
	{AWBALG_TYPE_RG,	 1023, 0},
	{AWBALG_TYPE_BG,	 1023, 0},
	{AWBALG_TYPE_RBG,	 1023,  0},
	{AWBALG_TYPE_SAT,	  1023,  0},
	{AWBALG_TYPE_RBGSUM, 1023, 0},
	{AWBALG_TYPE_RXBG,	 1023, 0},
	{AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
#endif	
#if 1
    {AWBALG_TYPE_Y,      170,  10},
	{AWBALG_TYPE_RG,	 243, 100},
	{AWBALG_TYPE_BG,	 256, 137},
	{AWBALG_TYPE_RBG,	 158,  72},
	{AWBALG_TYPE_SAT,	  42,  14},
	{AWBALG_TYPE_RBGSUM, 464, 250},
	{AWBALG_TYPE_RXBG,	 452, 153},
	{AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
#endif	
};

AWBALG_ELEMET element2[] =
{
#if 1
    {AWBALG_TYPE_Y,      170,  10},
	{AWBALG_TYPE_RG,	 243, 100},
	{AWBALG_TYPE_BG,	 256, 137},
	{AWBALG_TYPE_RBG,	 158,  72},
	{AWBALG_TYPE_SAT,	  42,  14},
	{AWBALG_TYPE_RBGSUM, 464, 250},
	{AWBALG_TYPE_RXBG,	 452, 153},
	{AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
#endif	
#if 0
    {AWBALG_TYPE_Y,      170,  10},
	{AWBALG_TYPE_RG,	 243, 100},
	{AWBALG_TYPE_BG,	 256, 137},
	{AWBALG_TYPE_RBG,	 158,  72},
	{AWBALG_TYPE_SAT,	  42,  14},
	{AWBALG_TYPE_RBGSUM, 464, 250},
	{AWBALG_TYPE_RXBG,	 452, 153},
	{AWBALG_TYPE_USER,     0,   0},
    {AWBALG_TAB_END,       0,   0}
#endif	
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
    { 500, 440},
    { 456, 379},
    { 500, 500}
};


