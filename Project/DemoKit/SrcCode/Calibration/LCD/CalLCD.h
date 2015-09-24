/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalLCD.h
    @ingroup    mIPRJAPTest

    @brief      Header file for LCD testing
                This file is the header file for LCD testing

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALLCD_H
#define _CALLCD_H

#include "Type.h"

#define LCD_VERIFY_WIDTH        320
#define LCD_VERIFY_HEIGHT       240
#define LCD_VERIFY_SIZE         0x12C00     // 320*240

#define LCD_VERIFY_WHITE        0
#define LCD_VERIFY_RED          1
#define LCD_VERIFY_GREEN        2
#define LCD_VERIFY_BLUE         3
#define LCD_VERIFY_BLACK        4
#define LCD_VERIFY_BW           5
#define LCD_VERIFY_WB           6
#define LCD_VERIFY_MAX          7

typedef struct
{
    UINT8       R;
    UINT8       G;
    UINT8       B;
} RGB_COLOR, *PRGB_COLOR;

typedef struct
{
    UINT8       Y;
    UINT8       CB;
    UINT8       CR;
} YCBCR_COLOR, *PYCBCR_COLOR;

extern void     Cal_LCDVerify(void);

#endif

//@}
