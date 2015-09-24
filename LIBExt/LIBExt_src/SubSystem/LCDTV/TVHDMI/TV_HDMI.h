/*
    TV HDMI TX module internal header file

    TV HDMI TX module internal header file

    @file       TV_HDMI.h
    @ingroup    mIDISPHDMITx
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/


#ifndef _TVHDMI_H
#define _TVHDMI_H

#include "hdmitx.h"

#define HDMI_TX_INT_STS         (0x1<<0)
#define HDMI_CEC_INT_STS        (0x1<<1)

#define ACT_HIGH                    0
#define ACT_LOW                     1



typedef struct
{
    HDMI_VIDEOID    VID;                ///< Video ID

    BOOL            HS;                 ///< H-Sync Active edge
    BOOL            VS;                 ///< V-Sync Active edge
    BOOL            HVLD;               ///< H-Valid Active edge
    BOOL            VVLD;               ///< V-Valid Active edge
    BOOL            CLK;                ///< Pixel-Clock Active edge
    BOOL            DE;                 ///< Data Enable Active edge
    BOOL            PROGRESSIVE;        ///< Progressive/Interlaced Selection

    UINT32          H_SYNCTIME;         ///< H-Sync Time
    UINT32          H_TOTAL;            ///< H-Total Time
    UINT32          H_STARTTIME;        ///< H-Start Time
    UINT32          H_ENDTIME;          ///< H-End Time

    UINT32          V_SYNCTIME;         ///< V-Sync Time
    UINT32          V_TOTAL;            ///< V-Total Time
    UINT32          V_ODD_STARTTIME;    ///< V-Odd field Start Time
    UINT32          V_ODD_ENDTIME;      ///< V-Odd field End Time
    UINT32          V_EVEN_STARTTIME;   ///< V-Even field Start Time
    UINT32          V_EVEN_ENDTIME;     ///< V-Even field End Time

    UINT32          WINX_OFS;           ///< IDE Window Offset X when TV overscan
    UINT32          WINY_OFS;           ///< IDE Window Offset Y when TV overscan
}TVHDMI_TIMING,*PTVHDMI_TIMING;



#define HDMI_432_PLL2_LOOPDIV       0x48    // 432 = 12*72/2/1
#define HDMI_432_PLL2_PREDIV        0x02

#define HDMI_504_PLL2_LOOPDIV       0x54    // 504 = 12*84/2/1
#define HDMI_504_PLL2_PREDIV        0x02

#define HDMI_594_PLL2_LOOPDIV       0x63    // 594 = 12*99/2/1
#define HDMI_594_PLL2_PREDIV        0x02

#endif
