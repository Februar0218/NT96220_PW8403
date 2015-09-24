/*
    Display TV device object

    @file       dispdev_gsinf8bits.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Oscommon.h"
#include "IDE.h"
#include "Pll.h"
#include "top.h"
#include "display.h"
#include "dispdev_ioctrl.h"

static DISPDEV_IOCTRL pTvIoControl=NULL;

static ER dispdev_openTV(void);
static ER dispdev_closeTV(void);
static void dispdev_setTvIOCTRL(FP pIoCtrlFunc);

static DISPDEV_OBJ DispDev_TvObj = { dispdev_openTV, dispdev_closeTV, NULL, NULL, NULL, dispdev_setTvIOCTRL, {NULL,NULL}};

static UINT8 guiTvFilterTap[8] = {8,8,8,8,8,8,8,8};//0.74MHz

#define DISPTV_DEBUG  ENABLE //DISABLE
#if DISPTV_DEBUG
#define disptv_debug(msg)       debug_msg msg
#else
#define disptv_debug(msg)
#endif


#define TV_NTSC_Buf_width                       640
#define TV_NTSC_Buf_height                      448
#define TV_NTSC_Win_width                       640
#define TV_NTSC_Win_height                      448

#define TV_PAL_Buf_width                        640
#define TV_PAL_Buf_height                       528
#define TV_PAL_Win_width                        640
#define TV_PAL_Win_height                       528

//Different FSADJ need different configuration
//HW FSADJ 3.6 K
#define TV_NTSC_BURST_LEVEL_3_6                 0x38
#define TV_NTSC_BLAND_LEVEL_3_6                 0x67
#define TV_NTSC_Y_SCALE_3_6                     0x78
#define TV_NTSC_CB_SCALE_3_6                    0x87
#define TV_NTSC_CR_SCALE_3_6                    0xBC

#define TV_PAL_BURST_LEVEL_3_6                  0x47
#define TV_PAL_BLAND_LEVEL_3_6                  0x6F
#define TV_PAL_Y_SCALE_3_6                      0x7D
#define TV_PAL_CB_SCALE_3_6                     0xA8
#define TV_PAL_CR_SCALE_3_6                     0xEC

//HW FSADJ 3.3 K
#define TV_NTSC_BURST_LEVEL_3_3                 0x4B
#define TV_NTSC_BLAND_LEVEL_3_3                 0x5F
#define TV_NTSC_Y_SCALE_3_3                     0x6B
#define TV_NTSC_CB_SCALE_3_3                    0x7B
#define TV_NTSC_CR_SCALE_3_3                    0xA9

#define TV_PAL_BURST_LEVEL_3_3                  0x44
#define TV_PAL_BLAND_LEVEL_3_3                  0x65
#define TV_PAL_Y_SCALE_3_3                      0x76
#define TV_PAL_CB_SCALE_3_3                     0x95
#define TV_PAL_CR_SCALE_3_3                     0xD9

//HW FSADJ 3.0 K
#define TV_NTSC_BURST_LEVEL_3_0                 0x33
#define TV_NTSC_BLAND_LEVEL_3_0                 0x60
#define TV_NTSC_Y_SCALE_3_0                     0x76
#define TV_NTSC_CB_SCALE_3_0                    0x72
#define TV_NTSC_CR_SCALE_3_0                    0x9A

#define TV_PAL_BURST_LEVEL_3_0                  0x26
#define TV_PAL_BLAND_LEVEL_3_0                  0x65
#define TV_PAL_Y_SCALE_3_0                      0x75
#define TV_PAL_CB_SCALE_3_0                     0x76
#define TV_PAL_CR_SCALE_3_0                     0xA4

#if 1
/*
    Display Device API of open TV Encoder

    Display Device API of open TV Encoder

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_OK:     TV Encoder Open done and success.
*/
static ER dispdev_openTV(void)
{
    UINT32                  uiBurstLevel=0;
    UINT32                  uiBlandLevel=0;
    UINT32                  uiYScale=0,uiCbScale=0,uiCrScale=0;
    UINT32                  uiClockDiv;
    UINT32                  uiSrcClkFreq;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    DISPDEV_IOCTRL          pDispDevControl = pTvIoControl;

    if(pDispDevControl == NULL)
        return E_PAR;

    disptv_debug(("^GDEVTV: open\r\n"));

    pDispDevControl(DISPDEV_IOCTRL_GET_ENABLE, &DevIOCtrl);
    if(DevIOCtrl.SEL.GET_ENABLE.bEn == TRUE)
    {
        DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
        pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
        pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
    }

    pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
    if ( DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL1)
    {
        uiSrcClkFreq = pll_getPLL1Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
    }
    else
    {
        uiSrcClkFreq = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
    }


    if ((pinmux_getDispMode(PINMUX_FUNC_ID_TV)&PINMUX_TV_HDMI_CFG_MASK) == PINMUX_TV_HDMI_CFG_GPIO)
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_GPIO);


    //set to 27MHz
    uiClockDiv = (float)uiSrcClkFreq/(float)27/(float)2;
    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    DevIOCtrl.SEL.SET_ICST_EN.bEn = FALSE;
    DevIOCtrl.SEL.SET_ICST_EN.Select = CST_RGB2YCBCR;
    pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);

    DevIOCtrl.SEL.SET_DITHER_EN.bEn      = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_DITHER_EN, &DevIOCtrl);

    DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_EMBD_TV;
    pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);

    #if 0
    // These parameters would not influence TV out
    ide_setPdir(IDE_PDIR_RGB);
    ide_setOdd(IDE_LCD_R);
    ide_setEven(IDE_LCD_G);
    ide_setHsInv(FALSE);
    ide_setVsInv(FALSE);
    ide_setHvldInv(FALSE);
    ide_setVvldInv(FALSE);
    ide_setClkInv(FALSE);
    ide_setFldInv(FALSE);
    #endif

    pDispDevControl(DISPDEV_IOCTRL_GET_ACT_DEVICE, &DevIOCtrl);

    if (DevIOCtrl.SEL.GET_ACT_DEVICE.DevID == DISPDEV_ID_TVNTSC)
    {
        //NTSC
        ide_setTvMode(IDE_TV_NTSC_M);
        ide_setTvNtscSetupLevel(0x27);

        //ide_setHorTiming(0x7F, 0x6B3, 0x144, 0x69D);
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = 0x7F;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = 0x6B3;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = 0x144;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = 0x69D;
        pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);

        //ide_set_ver_timing(0x05, 0x20C, 0x32, 0x205, 0x33, 0x206);
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = 0x05;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = 0x20C;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = 0x32;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = 0x205;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart= 0x33;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = 0x206;
        pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);

        pDispDevControl(DISPDEV_IOCTRL_GET_TVADJUST, &DevIOCtrl);
        if( DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_6_K)
        {
            uiBurstLevel    = TV_NTSC_BURST_LEVEL_3_6;
            uiBlandLevel    = TV_NTSC_BLAND_LEVEL_3_6;
            uiYScale        = TV_NTSC_Y_SCALE_3_6;
            uiCbScale       = TV_NTSC_CB_SCALE_3_6;
            uiCrScale       = TV_NTSC_CR_SCALE_3_6;
        }
        else if ( DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_0_K)
        {
            uiBurstLevel    = TV_NTSC_BURST_LEVEL_3_0;
            uiBlandLevel    = TV_NTSC_BLAND_LEVEL_3_0;
            uiYScale        = TV_NTSC_Y_SCALE_3_0;
            uiCbScale       = TV_NTSC_CB_SCALE_3_0;
            uiCrScale       = TV_NTSC_CR_SCALE_3_0;
        }
        else
        {
            uiBurstLevel    = TV_NTSC_BURST_LEVEL_3_3;
            uiBlandLevel    = TV_NTSC_BLAND_LEVEL_3_3;
            uiYScale        = TV_NTSC_Y_SCALE_3_3;
            uiCbScale       = TV_NTSC_CB_SCALE_3_3;
            uiCrScale       = TV_NTSC_CR_SCALE_3_3;
        }
    }
    else
    {
        //PAL
        ide_setTvMode(IDE_TV_PAL_BDGHI);
        ide_setTvNtscSetupLevel(0x00);

        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = 0x01;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = 0x6BF;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = 0x15C;
        DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = 0x69B;
        pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);

        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = 0x04;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = 0x270;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = 0x040;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = 0x267;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart= 0x041;
        DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = 0x268;
        pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);

        pDispDevControl(DISPDEV_IOCTRL_GET_TVADJUST, &DevIOCtrl);
        if( DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_6_K)
        {
            uiBurstLevel    = TV_PAL_BURST_LEVEL_3_6;
            uiBlandLevel    = TV_PAL_BLAND_LEVEL_3_6;
            uiYScale        = TV_PAL_Y_SCALE_3_6;
            uiCbScale       = TV_PAL_CB_SCALE_3_6;
            uiCrScale       = TV_PAL_CR_SCALE_3_6;
        }
        else if ( DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_0_K)
        {
            uiBurstLevel    = TV_PAL_BURST_LEVEL_3_0;
            uiBlandLevel    = TV_PAL_BLAND_LEVEL_3_0;
            uiYScale        = TV_PAL_Y_SCALE_3_0;
            uiCbScale       = TV_PAL_CB_SCALE_3_0;
            uiCrScale       = TV_PAL_CR_SCALE_3_0;
        }
        else
        {
            uiBurstLevel    = TV_PAL_BURST_LEVEL_3_3;
            uiBlandLevel    = TV_PAL_BLAND_LEVEL_3_3;
            uiYScale        = TV_PAL_Y_SCALE_3_3;
            uiCbScale       = TV_PAL_CB_SCALE_3_3;
            uiCrScale       = TV_PAL_CR_SCALE_3_3;
        }
    }

    DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bInterlaced = TRUE;
    DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bFieldOddSt = TRUE;
    pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE, &DevIOCtrl);


    DevIOCtrl.SEL.SET_CSB_EN.bEn = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_CSB_EN,&DevIOCtrl);


    DevIOCtrl.SEL.SET_YC_EXCHG.bCbCrExchg = FALSE;
    DevIOCtrl.SEL.SET_YC_EXCHG.bYCExchg   = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_YC_EXCHG,&DevIOCtrl);

    DevIOCtrl.SEL.SET_SUBPIXEL.bOddR     = FALSE;
    DevIOCtrl.SEL.SET_SUBPIXEL.bOddG     = FALSE;
    DevIOCtrl.SEL.SET_SUBPIXEL.bOddB     = FALSE;
    DevIOCtrl.SEL.SET_SUBPIXEL.bEvenR    = FALSE;
    DevIOCtrl.SEL.SET_SUBPIXEL.bEvenG    = FALSE;
    DevIOCtrl.SEL.SET_SUBPIXEL.bEvenB    = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_SUBPIXEL,&DevIOCtrl);

    //
    // TV Related APIs
    //
    ide_setTvBurst(0x00);
    ide_setTvSlaveMode(TRUE);
    ide_setTvChromaFilter(TRUE, guiTvFilterTap);
    ide_setTvSearchEn(TRUE);
    ide_setTvCkpnPol(TRUE);
    ide_setTvBrl((UINT8)uiBurstLevel);
    ide_setTvBll((UINT8)uiBlandLevel);
    ide_setTvScale((UINT8)uiYScale, (UINT8)uiCbScale, (UINT8)uiCrScale);
    ide_setTvSela(0x3);
    ide_setTvPowerDown(FALSE);


    pDispDevControl(DISPDEV_IOCTRL_GET_ACT_DEVICE, &DevIOCtrl);
    if (DevIOCtrl.SEL.GET_ACT_DEVICE.DevID == DISPDEV_ID_TVNTSC)
    {
        DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = TV_NTSC_Buf_width;
        DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = TV_NTSC_Buf_height;
        DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = TV_NTSC_Win_width;
        DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = TV_NTSC_Win_height;
    }
    else
    {
        DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = TV_PAL_Buf_width;
        DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = TV_PAL_Buf_height;
        DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = TV_PAL_Win_width;
        DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = TV_PAL_Win_height;
    }
    pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);

    disptv_debug(("^GDEVTV: init done\r\n"));
    return E_OK;
}

/*
    Display Device API of Close TV Encoder

    Display Device API of Close TV Encoder

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_OK:     TV Encoder Close done and success.
*/
static ER dispdev_closeTV(void)
{
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    DISPDEV_IOCTRL          pDispDevControl = pTvIoControl;

    disptv_debug(("^GDEVTV: close\r\n"));

    // Disable IDE
    DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
    pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);

    ide_setTvPowerDown(TRUE);
    return E_OK;
}

/*
    Set TV Display Device IOCTRL function pointer
*/
static void dispdev_setTvIOCTRL(FP pIoCtrlFunc)
{
    pTvIoControl = (DISPDEV_IOCTRL)pIoCtrlFunc;
}

#endif

/**
    Get TV Display Device Object

    Get TV Display Device Object. This Object is used to control the TV Encoder.

    @return The pointer of TV Display Device object.
*/
PDISPDEV_OBJ dispdev_getTvDevObj(void)
{
    return &DispDev_TvObj;
}
