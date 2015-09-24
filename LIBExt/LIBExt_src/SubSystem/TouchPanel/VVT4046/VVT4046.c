/*
    VVT4046 Touch panel controller interface

    VVT4046 Touch panel controller interface

    @file       VVT4046.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include <string.h>
#include "Type.h"
#include "GPIO.h"
#include "Debug.h"
#include "Utility.h"
#include "TouchPanel.h"
#include "pad.h"
//-------------------------------------------------------------------------------------------------
#define VVT4046_STARTBIT            0x80
#define VVT4046_MEASURE_Y           0x10
#define VVT4046_MEASURE_X           0x50
#define VVT4046_MEASURE_Z1          0x30
#define VVT4046_MEASURE_Z2          0x40
#define VVT4046_MODE_8BIT           0x08
#define VVT4046_MODE_12BIT          0x00
#define VVT4046_REF_SINGLE          0x04
#define VVT4046_REF_DIFF            0x00
#define VVT4046_PWR_DOWN            0x00
#define VVT4046_PWR_REFOFF_ADCON    0x01
#define VVT4046_PWR_REFON_ADCOFF    0x02
#define VVT4046_PWR_FULL            0x03

#define VVT4046_REF_MODE            VVT4046_REF_DIFF
#define VVT4046_PWR_MODE            VVT4046_PWR_FULL

#define VVT4046_MODE                (VVT4046_REF_MODE|VVT4046_PWR_MODE)

// X, Y use 12 bits to have better precision
#define VVT4046_GETX                (VVT4046_STARTBIT|VVT4046_MEASURE_X|VVT4046_MODE_12BIT|VVT4046_MODE)
#define VVT4046_GETY                (VVT4046_STARTBIT|VVT4046_MEASURE_Y|VVT4046_MODE_12BIT|VVT4046_MODE)

// 8 bits is enough for pressure
#define VVT4046_GETP1               (VVT4046_STARTBIT|VVT4046_MEASURE_Z1|VVT4046_MODE_8BIT|VVT4046_MODE)
#define VVT4046_GETP2               (VVT4046_STARTBIT|VVT4046_MEASURE_Z2|VVT4046_MODE_8BIT|VVT4046_MODE)

#define VVT4046_PRESSURE_TH         245
//-------------------------------------------------------------------------------------------------
static TP_INIT_STRUCT Vvt4046;
//-------------------------------------------------------------------------------------------------
__inline void vvt4046_delay(void)
{
    TimerDelayUs(1);
    //do nothing

}
//-------------------------------------------------------------------------------------------------
static void vvt4046_toggleClock(void)
{
    vvt4046_delay();
    // DCLK high
    gpio_setPin(Vvt4046.Clk);

    vvt4046_delay();
    // DCLK low
    gpio_clearPin(Vvt4046.Clk);
}
//-------------------------------------------------------------------------------------------------
void vvt4046_getPressure(
    UINT32*     pZ1,
    UINT32*     pZ2)
{
    UINT32 i;

    // CS, DCLK low
    gpio_clearPin(Vvt4046.Cs);
    gpio_clearPin(Vvt4046.Clk);

    vvt4046_delay();

    // Output command
    for (i=8; i>0; i--)
    {
        if (((VVT4046_GETP1 >> (i - 1)) & 0x01) == 0)
        {
            gpio_clearPin(Vvt4046.Out);
        }
        else
        {
            gpio_setPin(Vvt4046.Out);
        }

        // Toggle clock to produce one clock pulse
        vvt4046_toggleClock();
    }

    // DOUT low
    gpio_clearPin(Vvt4046.Out);

    // Toggle clock to produce one clock pulse
    vvt4046_toggleClock();

    // Read data
    *pZ1 = 0;

    for (i=14; i>0; i--)
    {
        vvt4046_delay();

        // Output command
        if (i<9)
        {
            if (((VVT4046_GETP2 >> (i - 1)) & 0x01) == 0)
            {
                gpio_clearPin(Vvt4046.Out);
            }
            else
            {
                gpio_setPin(Vvt4046.Out);
            }
        }

        // DCLK high
        gpio_setPin(Vvt4046.Clk);

        // Read data
        if (i > 6)
        {
            *pZ1 = *pZ1 | (gpio_getPin(Vvt4046.In) << (i - 7));
        }

        vvt4046_delay();

        // DCLK low
        gpio_clearPin(Vvt4046.Clk);
    }

    // DOUT low
    gpio_clearPin(Vvt4046.Out);

    // Toggle clock to produce one clock pulse
    vvt4046_toggleClock();

    // Read data
    *pZ2 = 0;

    for (i=8; i>0; i--)
    {
        vvt4046_delay();

        // DCLK high
        gpio_setPin(Vvt4046.Clk);

        // Read data
        *pZ2 = *pZ2 | (gpio_getPin(Vvt4046.In) << (i - 1));

        vvt4046_delay();

        // DCLK low
        gpio_clearPin(Vvt4046.Clk);
    }

    vvt4046_delay();

    // CS high
    gpio_setPin(Vvt4046.Cs);
}
//-------------------------------------------------------------------------------------------------
void vvt4046_getPos(
    UINT32*     pX,
    UINT32*     pY)
{
    UINT32 i;

    // CS, DCLK low
    gpio_clearPin(Vvt4046.Cs);
    gpio_clearPin(Vvt4046.Clk);

    // Output command
    for (i=8; i>0; i--)
    {
        if (((VVT4046_GETX >> (i - 1)) & 0x01) == 0)
        {
            gpio_clearPin(Vvt4046.Out);
        }
        else
        {
            gpio_setPin(Vvt4046.Out);
        }

        // Toggle clock to produce one clock pulse
        vvt4046_toggleClock();
    }

    // DOUT low
    gpio_clearPin(Vvt4046.Out);

    // Toggle clock to produce one clock pulse
    vvt4046_toggleClock();

    // Read data
    *pX = 0;

    for (i=14; i>0; i--)
    {
        vvt4046_delay();

        // Output command
        if (i<9)
        {
            if (((VVT4046_GETY >> (i - 1)) & 0x01) == 0)
            {
                gpio_clearPin(Vvt4046.Out);
            }
            else
            {
                gpio_setPin(Vvt4046.Out);
            }
        }

        // DCLK high
        gpio_setPin(Vvt4046.Clk);

        // Read data
        if (i > 2)
        {
            *pX = *pX | (gpio_getPin(Vvt4046.In) << (i - 3));
        }

        vvt4046_delay();

        // DCLK low
        gpio_clearPin(Vvt4046.Clk);
    }

    // DOUT low
    gpio_clearPin(Vvt4046.Out);

    // Toggle clock to produce one clock pulse
    vvt4046_toggleClock();

    // Read data
    *pY = 0;

    for (i=12; i>0; i--)
    {
        vvt4046_delay();

        // DCLK high
        gpio_setPin(Vvt4046.Clk);

        // Read data
        *pY = *pY | (gpio_getPin(Vvt4046.In) << (i - 1));

        vvt4046_delay();

        // DCLK low
        gpio_clearPin(Vvt4046.Clk);
    }

    vvt4046_delay();

    // CS high
    gpio_setPin(Vvt4046.Cs);
}
//-------------------------------------------------------------------------------------------------
void tp_init(TP_INIT_STRUCT* pPin)
{
    memcpy((void*)&Vvt4046, pPin, sizeof(TP_INIT_STRUCT));

    gpio_setDir(Vvt4046.Cs, GPIO_DIR_OUTPUT);
    gpio_setPin(Vvt4046.Cs);
    gpio_setDir(Vvt4046.Clk, GPIO_DIR_OUTPUT);
    gpio_clearPin(Vvt4046.Clk);
    gpio_setDir(Vvt4046.Out, GPIO_DIR_OUTPUT);
    gpio_clearPin(Vvt4046.Out);
    gpio_setDir(Vvt4046.In, GPIO_DIR_INPUT);
    pad_setPullUpDown(Vvt4046.InPad, PAD_PULLUP);
    gpio_setDir(Vvt4046.Busy, GPIO_DIR_INPUT);
    pad_setPullUpDown(Vvt4046.BusyPad, PAD_PULLUP);
}
//-------------------------------------------------------------------------------------------------
BOOL tp_getPos(
    UINT32*     pPosX,
    UINT32*     pPosY)
{
    BOOL IsPress;

    vvt4046_getPressure(pPosX,pPosY);

    if((*pPosY) < ((*pPosX) + VVT4046_PRESSURE_TH))
    {
        IsPress = TRUE;
    }
    else
    {
        IsPress = FALSE;
    }

    vvt4046_getPos(pPosX,pPosY);

    return IsPress;
}
//-------------------------------------------------------------------------------------------------

