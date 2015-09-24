/*
    CP2056 Touch panel controller interface

    CP2056 Touch panel controller interface

    @file       CP2056.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include <string.h>
#include "Type.h"
#include "pad.h"
#include "GPIO.h"
#include "Debug.h"
#include "Utility.h"
#include "TouchPanel.h"
//-------------------------------------------------------------------------------------------------
#define CP2056_NEW_PRESSURE         ENABLE

#define CP2056_STARTBIT             0x80
#define CP2056_MEASURE_Y            0x10
#define CP2056_MEASURE_X            0x50
#define CP2056_MEASURE_Z1           0x30
#define CP2056_MEASURE_Z2           0x40
#define CP2056_MODE_8BIT            0x08
#define CP2056_MODE_12BIT           0x00
#define CP2056_REF_SINGLE           0x04
#define CP2056_REF_DIFF             0x00
#define CP2056_PWR_DOWN             0x00
#define CP2056_PWR_REFOFF_ADCON     0x01
#define CP2056_PWR_REFON_ADCOFF     0x02
#define CP2056_PWR_FULL             0x03

#define CP2056_REF_MODE             CP2056_REF_DIFF
#define CP2056_PWR_MODE             CP2056_PWR_FULL

#define CP2056_MODE                 (CP2056_REF_MODE|CP2056_PWR_MODE)

// X, Y use 12 bits to have better precision
#define CP2056_GETX                 (CP2056_STARTBIT|CP2056_MEASURE_X|CP2056_MODE_12BIT|CP2056_MODE)
#define CP2056_GETY                 (CP2056_STARTBIT|CP2056_MEASURE_Y|CP2056_MODE_12BIT|CP2056_MODE)

// 8 bits is enough for pressure
#define CP2056_PRESSURE_8BIT        ENABLE
#if (CP2056_PRESSURE_8BIT == ENABLE)
#define CP2056_GETP1                (CP2056_STARTBIT|CP2056_MEASURE_Z1|CP2056_MODE_8BIT|CP2056_MODE)
#define CP2056_GETP2                (CP2056_STARTBIT|CP2056_MEASURE_Z2|CP2056_MODE_8BIT|CP2056_MODE)
#else //12-bit
#define CP2056_GETP1                (CP2056_STARTBIT|CP2056_MEASURE_Z1|CP2056_MODE_12BIT|CP2056_MODE)
#define CP2056_GETP2                (CP2056_STARTBIT|CP2056_MEASURE_Z2|CP2056_MODE_12BIT|CP2056_MODE)
#endif

#define CP2056_PRESSURE_TH          245  // useless in new method
#define CP2056_PRESSURE_TH_H        6000 // temporary
#define CP2056_PRESSURE_TH_L        300  // temporary
//-------------------------------------------------------------------------------------------------
static TP_INIT_STRUCT CP2056;
//-------------------------------------------------------------------------------------------------
__inline void cp2056_delay(void)
{
    #if 0
    TimerDelayUs(1);
    //do nothing
    #else
    UINT32 i = 1;
    while (i--);
    #endif
}
//-------------------------------------------------------------------------------------------------
//#NT#2012/02/24#Hideo Lin -begin
//#NT#Add read/write data function for CP2056 by serial interface
#if (CP2056_NEW_PRESSURE == ENABLE)
static void cp2056_writeBit(BOOL bBit)
{
    if (bBit)
    {
        gpio_setPin(CP2056.Out);
    }
    else
    {
        gpio_clearPin(CP2056.Out);
    }
    gpio_clearPin(CP2056.Clk);
    cp2056_delay();
    gpio_setPin(CP2056.Clk);
    cp2056_delay();
    gpio_clearPin(CP2056.Clk);
}

static void cp2056_writeData(UINT32 uiData)
{
    UINT32  i;
    UINT32  uiBit = 0x80;

    for (i = 0; i < 8; i++)
    {
        if (uiData & uiBit)
        {
            cp2056_writeBit(1);
        }
        else
        {
            cp2056_writeBit(0);
        }
        uiBit >>= 1;
    }
    gpio_clearPin(CP2056.Out);
}

static UINT32 cp2056_readData(void)
{
    UINT32  i;
    UINT32  uiBit = 0x0800;
    UINT32  uiData = 0;
    //UINT32  uiCount = 0;

    gpio_clearPin(CP2056.Clk);
    cp2056_delay();
    gpio_setPin(CP2056.Clk);
    cp2056_delay();
    gpio_clearPin(CP2056.Clk);
    cp2056_delay();

    #if 0
    while (gpio_getPin(CP2056.Busy))
    {
        gpio_clearPin(CP2056.Clk);
        cp2056_delay();
        gpio_setPin(CP2056.Clk);
        cp2056_delay();
        uiCount++;
        if (uiCount > 1000)
        {
            debug_err(("TP timeout!\r\n"));
            return 0;
        }
    };
    gpio_clearPin(CP2056.Clk);
    cp2056_delay();
    #endif

    for (i = 0; i < 12; i++)
    {
        gpio_setPin(CP2056.Clk);
        cp2056_delay();

        if (gpio_getPin(CP2056.In))
            uiData |= uiBit;

        uiBit >>= 1;

        gpio_clearPin(CP2056.Clk);
        cp2056_delay();
}

    return uiData;
}
#endif
//#NT#2012/02/24#Hideo Lin -end
//-------------------------------------------------------------------------------------------------
#if (CP2056_NEW_PRESSURE == DISABLE)
static void cp2056_toggleClock(void)
{
    cp2056_delay();
    // DCLK high
    gpio_setPin(CP2056.Clk);

    cp2056_delay();
    // DCLK low
    gpio_clearPin(CP2056.Clk);
}
#endif
//-------------------------------------------------------------------------------------------------
#if (CP2056_NEW_PRESSURE == ENABLE)
static BOOL cp2056_getPressure(void)
{
    UINT32  x, z1, z2;
    UINT32  uiPressure = 0;
    BOOL    bResult = FALSE;

    // CS, DCLK low
    gpio_clearPin(CP2056.Cs);
    gpio_clearPin(CP2056.Clk);

    // Get X data
    cp2056_writeData(CP2056_GETX);
    x = cp2056_readData();

    // Get Z1 value
    cp2056_writeData(CP2056_GETP1);
    z1 = cp2056_readData();

    // Get Z2 value
    cp2056_writeData(CP2056_GETP2);
    z2 = cp2056_readData();

    if ((z1 > 0) && (z2 > z1))
    {
        uiPressure = (x*(z2-z1)/z1) >> 2; // Rx*x/4096*(z2/z1-1), assume Rx=1024
        if ((uiPressure > CP2056_PRESSURE_TH_H) || (uiPressure <= CP2056_PRESSURE_TH_L))
            bResult = FALSE;
        else
            bResult = TRUE;
    }

    // CS high
    gpio_setPin(CP2056.Cs);

    if ((z1 > 0) && (z2 > z1))
    {
        //debug_msg("x=%d, z1=%d, z2=%d, pr=%d\r\n", x, z1, z2, uiPressure);
    }

    return bResult;
}
#else
static void cp2056_getPressure(
    UINT32*     pZ1,
    UINT32*     pZ2)
{
    UINT32 i;

    // CS, DCLK low
    gpio_clearPin(CP2056.Cs);
    gpio_clearPin(CP2056.Clk);

    cp2056_delay();

    // Output command
    for (i=8; i>0; i--)
    {
        if (((CP2056_GETP1 >> (i - 1)) & 0x01) == 0)
        {
            gpio_clearPin(CP2056.Out);
        }
        else
        {
            gpio_setPin(CP2056.Out);
        }

        // Toggle clock to produce one clock pulse
        cp2056_toggleClock();
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pZ1 = 0;

    for (i=14; i>0; i--)
    {
        cp2056_delay();

        // Output command
        if (i<9)
        {
            if (((CP2056_GETP2 >> (i - 1)) & 0x01) == 0)
            {
                gpio_clearPin(CP2056.Out);
            }
            else
            {
                gpio_setPin(CP2056.Out);
            }
        }

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        #if (CP2056_PRESSURE_8BIT == ENABLE)
        if (i > 6)
        {
            *pZ1 = *pZ1 | (gpio_getPin(CP2056.In) << (i - 7));
        }
        #else // 12-bit
        if (i > 2)
        {
            *pZ1 = *pZ1 | (gpio_getPin(CP2056.In) << (i - 3));
        }
        #endif

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pZ2 = 0;

    #if (CP2056_PRESSURE_8BIT == ENABLE)
    for (i=8; i>0; i--)
    #else // 12-bit
    for (i=12; i>0; i--)
    #endif
    {
        cp2056_delay();

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        *pZ2 = *pZ2 | (gpio_getPin(CP2056.In) << (i - 1));

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    cp2056_delay();

    // CS high
    gpio_setPin(CP2056.Cs);

    #if (CP2056_PRESSURE_8BIT != ENABLE) // 12-bit
    *pZ1 = (*pZ1)>>4;
    *pZ2 = (*pZ2)>>4;
    #endif
}
#endif
//-------------------------------------------------------------------------------------------------
#if (CP2056_NEW_PRESSURE == ENABLE)
static void cp2056_getPos(
    UINT32*     pX,
    UINT32*     pY)
{

    // CS, DCLK low
    gpio_clearPin(CP2056.Cs);
    gpio_clearPin(CP2056.Clk);

    // Read X data
    cp2056_writeData(CP2056_GETX);
    *pX = cp2056_readData();

    // Read Y data
    cp2056_writeData(CP2056_GETY);
    *pY = cp2056_readData();

    // CS high
    gpio_setPin(CP2056.Cs);

}
#else
static void cp2056_getPos(
    UINT32*     pX,
    UINT32*     pY)
{
    UINT32 i;

    // CS, DCLK low
    gpio_clearPin(CP2056.Cs);
    gpio_clearPin(CP2056.Clk);

    // Output command
    for (i=8; i>0; i--)
    {
        if (((CP2056_GETX >> (i - 1)) & 0x01) == 0)
        {
            gpio_clearPin(CP2056.Out);
        }
        else
        {
            gpio_setPin(CP2056.Out);
        }

        // Toggle clock to produce one clock pulse
        cp2056_toggleClock();
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pX = 0;

    for (i=14; i>0; i--)
    {
        cp2056_delay();

        // Output command
        if (i<9)
        {
            if (((CP2056_GETY >> (i - 1)) & 0x01) == 0)
            {
                gpio_clearPin(CP2056.Out);
            }
            else
            {
                gpio_setPin(CP2056.Out);
            }
        }

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        if (i > 2)
        {
            *pX = *pX | (gpio_getPin(CP2056.In) << (i - 3));
        }

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pY = 0;

    for (i=12; i>0; i--)
    {
        cp2056_delay();

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        *pY = *pY | (gpio_getPin(CP2056.In) << (i - 1));

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    cp2056_delay();

    // CS high
    gpio_setPin(CP2056.Cs);
}
#endif
//-------------------------------------------------------------------------------------------------
void tp_init(TP_INIT_STRUCT* pPin)
{
    memcpy((void*)&CP2056, pPin, sizeof(TP_INIT_STRUCT));

    // IO init
    gpio_setDir(CP2056.Cs, GPIO_DIR_OUTPUT);
    gpio_setPin(CP2056.Cs);
    gpio_setDir(CP2056.Clk, GPIO_DIR_OUTPUT);
    gpio_clearPin(CP2056.Clk);
    gpio_setDir(CP2056.Out, GPIO_DIR_OUTPUT);
    gpio_clearPin(CP2056.Out);
    gpio_setDir(CP2056.In, GPIO_DIR_INPUT);
    pad_setPullUpDown(CP2056.InPad, PAD_PULLUP);
    gpio_setDir(CP2056.Busy, GPIO_DIR_INPUT);
    pad_setPullUpDown(CP2056.BusyPad, PAD_PULLUP);
}
//-------------------------------------------------------------------------------------------------
#if (CP2056_NEW_PRESSURE == ENABLE)
BOOL tp_getPos(
    UINT32*     pPosX,
    UINT32*     pPosY)
{
    BOOL    bIsPress;
    UINT32  uiTmp;
    static UINT32 uiX = 0, uiY = 0; // the last pressed position

    bIsPress = cp2056_getPressure();

    cp2056_getPos(pPosX, pPosY);

    // Recall the last pressed position
    if (bIsPress == FALSE)
    {
        *pPosX = uiX;
        *pPosY = uiY;
    }

    // X, Y change in this panel
    uiTmp = *pPosX;
    *pPosX = *pPosY;
    *pPosY = uiTmp;

    return bIsPress;
}
#else
BOOL tp_getPos(
    UINT32*     pPosX,
    UINT32*     pPosY)
{
    BOOL    bIsPress;
    UINT32  uiTmp;
    static UINT32 uiX = 0, uiY = 0; // the last pressed position

    cp2056_getPressure(pPosX,pPosY);
    if (*pPosX != 0 || *pPosY != 255)
    {
        debug_msg("\r\n[TP_PRE]:x=%d, y=%d\r\n", *pPosX, *pPosY);
    }

    if(((*pPosY) < ((*pPosX) + CP2056_PRESSURE_TH)) && (*pPosX > 0) && (*pPosY < 255))
    {
        bIsPress = TRUE;

        // Store the last pressed position
        uiX = *pPosX;
        uiY = *pPosY;
        Perf_CheckRunTime(2, "TP");
    }
    else
    {
        bIsPress = FALSE;
    }

    cp2056_getPos(pPosX,pPosY);

    // Recall the last pressed position
    if (bIsPress == FALSE)
    {
        *pPosX = uiX;
        *pPosY = uiY;
    }

    // X, Y change in this panel
    uiTmp = *pPosX;
    *pPosX = *pPosY;
    *pPosY = uiTmp;

    return bIsPress;
}
#endif
//-------------------------------------------------------------------------------------------------

