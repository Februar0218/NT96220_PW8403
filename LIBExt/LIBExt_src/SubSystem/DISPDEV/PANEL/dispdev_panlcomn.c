/*
    Panel device open/close Common control.

    @file       dispdev_panel.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Oscommon.h"
#include "dispdev_panlcomn.h"
#include "display.h"
#include "ide.h"
#include "pll.h"
#include "Utility.h"

/*
    Set Display open

    Open the specified display engine with the specified engine parameters.
*/
ER dispdev_setDisplay(DISPDEV_IOCTRL pDispDevControl, tLCD_INF LcdInf, tLCD_PARAM *pMode, PINMUX_FUNC_ID PinFuncID)
{
    UINT8                   uiClockDiv;
    tIDE_PARAM*             pIDE    = &pMode->IDE;
    tPANEL_PARAM*           pPanel  = &pMode->Panel;
    UINT32                  uiSrcClkFreq;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    dispanl_debug(("^Gdispdev_setDisplay START\r\n"));

    pinmux_setPinmux(PinFuncID, pIDE->pinmux_select_lcd);

    pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
    if( DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_MI_ENGINE)
    {
        // This path is for MI engine DRAM direct path display object
        DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = pPanel->uiBufferWidth;
        DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = pPanel->uiBufferHeight;
        DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = pPanel->uiWindowWidth;
        DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = pPanel->uiWindowHeight;
        pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);
        return E_OK;
    }

    // Make sure IDE
    pDispDevControl(DISPDEV_IOCTRL_GET_ENABLE, &DevIOCtrl);
    if(DevIOCtrl.SEL.GET_ENABLE.bEn == TRUE)
    {
        DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
        pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
        pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
    }

    // Set IDE Clock Source as specified
    pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
    if ( DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL1)
    {
        uiSrcClkFreq = pll_getPLL1Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
    }
    else if ( DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL2)
    {
        uiSrcClkFreq = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
    }
    else
    {
        return E_SYS;
    }

    // Set IDE PIXEL clock frequency
    if(pPanel->bYCbCrFormat)
    {
        uiClockDiv = (UINT8)((float)uiSrcClkFreq/(float)pPanel->fDCLK);

        DevIOCtrl.SEL.SET_ICST_EN.bEn = FALSE;
        DevIOCtrl.SEL.SET_ICST_EN.Select = CST_RGB2YCBCR;
        pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);
    }
    else
    {
        uiClockDiv = (UINT8)((float)uiSrcClkFreq/(float)pPanel->fDCLK/(float)2.0);

        DevIOCtrl.SEL.SET_ICST_EN.bEn = TRUE;
        DevIOCtrl.SEL.SET_ICST_EN.Select = CST_YCBCR2RGB;
        pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);
    }

    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

    // Enable IDE Clock
    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    if(LcdInf == tLCD_INF_SERIAL_8BITS)
    {
        switch(pPanel->LCDMode)
        {
        case PINMUX_LCDMODE_RGB_SERIAL:
            DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_SERIAL;
            break;
        case PINMUX_LCDMODE_YUV640:
        case PINMUX_LCDMODE_YUV720:
            DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_YUV;
            break;
        case PINMUX_LCDMODE_RGBD360:
        case PINMUX_LCDMODE_RGBD320:
            DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGBD;
            break;
        case PINMUX_LCDMODE_RGB_THROUGH:
            DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_THROUGH;
            break;
        case PINMUX_LCDMODE_CCIR601:
            DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_CCIR601_8BIT;
            break;
        case PINMUX_LCDMODE_CCIR656:
            DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_CCIR656_8BIT;
            break;
        default:
            debug_err(("Invalid LCDMode %d\r\n",pPanel->LCDMode));
            return E_NOSPT;
        }
        pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);

    }
    else if(LcdInf == tLCD_INF_MI)
    {
        if((pPanel->LCDMode>=PINMUX_LCDMODE_MI_FMT0)&&(pPanel->LCDMode<=PINMUX_LCDMODE_MI_SERIAL_SEP))
        {
            DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_MI;
            pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);
        }
        else
        {
            debug_err(("Invalid LCDMode %d\r\n",pPanel->LCDMode));
        }
    }
    else
    {
        debug_err(("LcdInf Has not implemented!(%d)\r\n",LcdInf));
        return E_NOSPT;
    }

    DevIOCtrl.SEL.SET_DITHER_EN.bEn      = pIDE->dithering[0];
    DevIOCtrl.SEL.SET_DITHER_EN.bFreeRun = pIDE->dithering[1];
    DevIOCtrl.SEL.SET_DITHER_EN.Rbits    = pIDE->DitherBits[0];
    DevIOCtrl.SEL.SET_DITHER_EN.Gbits    = pIDE->DitherBits[1];
    DevIOCtrl.SEL.SET_DITHER_EN.Bbits    = pIDE->DitherBits[2];
    pDispDevControl(DISPDEV_IOCTRL_SET_DITHER_EN, &DevIOCtrl);


    DevIOCtrl.SEL.SET_SRGB_OUTORDER.PixOrder     = pIDE->pdir;
    DevIOCtrl.SEL.SET_SRGB_OUTORDER.OddStart     = pIDE->odd;
    DevIOCtrl.SEL.SET_SRGB_OUTORDER.EvenStart    = pIDE->even;
    pDispDevControl(DISPDEV_IOCTRL_SET_SRGB_OUTORDER, &DevIOCtrl);


    DevIOCtrl.SEL.SET_SYNC_INVERT.bHsInv  = pIDE->hsinv;
    DevIOCtrl.SEL.SET_SYNC_INVERT.bVsInv  = pIDE->vsinv;
    DevIOCtrl.SEL.SET_SYNC_INVERT.bClkInv = pIDE->clkinv;
    pDispDevControl(DISPDEV_IOCTRL_SET_SYNC_INVERT, &DevIOCtrl);


    DevIOCtrl.SEL.SET_VLD_INVERT.bHvldInv    = pIDE->hvldinv;
    DevIOCtrl.SEL.SET_VLD_INVERT.bVvldInv    = pIDE->vvldinv;
    DevIOCtrl.SEL.SET_VLD_INVERT.bFieldInv   = pIDE->fieldinv;
    DevIOCtrl.SEL.SET_VLD_INVERT.bDeInv      = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_VLD_INVERT, &DevIOCtrl);


    if(pPanel->bYCbCrFormat)
    {
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = pPanel->uiHSyncSYNCwidth;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = (pPanel->uiHSyncTotalPeriod>>1)-1;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = pPanel->uiHSyncBackPorch>>1;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = ((pPanel->uiHSyncActivePeriod+pPanel->uiHSyncBackPorch)>>1)-1;
        pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);
    }
    else
    {
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = pPanel->uiHSyncSYNCwidth;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = pPanel->uiHSyncTotalPeriod-1;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = pPanel->uiHSyncBackPorch;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = (pPanel->uiHSyncActivePeriod+pPanel->uiHSyncBackPorch)-1;
        pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);
    }


    DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = pPanel->uiVSyncSYNCwidth;
    DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = pPanel->uiVSyncTotalPeriod-1;
    DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = pPanel->uiVSyncBackPorchOdd;
    DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = (pPanel->uiVSyncActivePeriod+pPanel->uiVSyncBackPorchOdd)-1;
    DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart= pPanel->uiVSyncBackPorchEven;
    DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = (pPanel->uiVSyncActivePeriod+pPanel->uiVSyncBackPorchEven)-1;
    pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);


    DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bInterlaced = pIDE->interlace;
    DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bFieldOddSt = TRUE;
    pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE, &DevIOCtrl);


    DevIOCtrl.SEL.SET_CSB_EN.bEn             = TRUE;
    DevIOCtrl.SEL.SET_CSB_EN.uiContrast      = pIDE->ctrst;
    DevIOCtrl.SEL.SET_CSB_EN.uiSaturation    = pIDE->cmults;
    DevIOCtrl.SEL.SET_CSB_EN.uiBrightness    = pIDE->brt;
    pDispDevControl(DISPDEV_IOCTRL_SET_CSB_EN,&DevIOCtrl);


    DevIOCtrl.SEL.SET_YC_EXCHG.bCbCrExchg    = pIDE->cex;
    DevIOCtrl.SEL.SET_YC_EXCHG.bYCExchg      = pIDE->YCex;
    pDispDevControl(DISPDEV_IOCTRL_SET_YC_EXCHG,&DevIOCtrl);

    ide_setTvPowerDown(pIDE->tv_powerdown);

    DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = pPanel->uiBufferWidth;
    DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = pPanel->uiBufferHeight;
    DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = pPanel->uiWindowWidth;
    DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = pPanel->uiWindowHeight;
    pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);

    DevIOCtrl.SEL.SET_ENABLE.bEn = TRUE;
    pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE,&DevIOCtrl);
    dispanl_debug(("^Gdispdev_setDisplay Config Done\r\n"));

    pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
    dispanl_debug(("^Gdispdev_setDisplay IDE GO\r\n"));
    return E_OK;
}

#if 0
/*
    Display close

    Close the specified display engine.
*/
ER dispdev_closeDisplay(DISPDEV_IOCTRL pDispDevControl)
{
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    dispanl_debug(("^GDispDevClose\r\n"));

    DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
    pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

    /*
    // Disable IDE
    if(pDevIF->DispID == DISP_1)
    {
        pDevIF->waitFrmEnd();
        pll_disableClock(IDE1_CLK);
        pll_disableClock(IDE2_CLK);
    }
    else if (pDevIF->DispID == DISP_2)
    {
        TimerDelayMs(40);
    }
    */

    return E_OK;
}
#endif

