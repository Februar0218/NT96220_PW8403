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
#include "awb_MI9130_220_int.h"

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
    //indoor
    {AWBALG_TYPE_Y     ,220,20},
    {AWBALG_TYPE_RG    ,181,75},
    {AWBALG_TYPE_BG    ,201,101},
    {AWBALG_TYPE_RBG   ,256,67},
    {AWBALG_TYPE_SAT   ,44,13},
    {AWBALG_TYPE_RBGSUM,319,242},
    {AWBALG_TYPE_RXBG  ,244,141},
    {AWBALG_TYPE_USER  ,0,0},
    {AWBALG_TAB_END    ,0,0} 
};

AWBALG_ELEMET element2[] =
{
    //outdoor
    {AWBALG_TYPE_Y     ,220,20},
    {AWBALG_TYPE_RG    ,174,104},
    {AWBALG_TYPE_BG    ,157,101},
    {AWBALG_TYPE_RBG   ,143,67},
    {AWBALG_TYPE_SAT   ,27,13},
    {AWBALG_TYPE_RBGSUM,307,242},
    {AWBALG_TYPE_RXBG  ,212,150},
    {AWBALG_TYPE_USER  ,0,0},
    {AWBALG_TAB_END    ,0,0} 
};

AWBALG_TAB AwbTAB[AWB_TAB_CNT] =
{
    {((1 << 10) * 10), ((1 << 4) * 10), element1},
    {((1 << 17) * 10), ((1 << 10) * 10), element2}
};
#endif



const UINT32 MwbTAB[AWB_MODE_MAX][2] =
{
    { 256, 256},//auto
    //7/14 2012/07/27 final
    { 388, 277},//daylight      
    { 368, 293},//cloudy        349   310
    { 267, 400},//tungsten
    { 286, 373},//fluoresent1
    
    { 292, 386},//fluoresent2
    { 417, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256},
    { 256, 256}
};


