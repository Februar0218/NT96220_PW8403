#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Pll.h"
#include "debug.h"
#include "top.h"

#if 0
#define TV_NTSC_Buf_width              720
#define TV_NTSC_Buf_height             480
#define TV_NTSC_Win_width              720
#define TV_NTSC_Win_height             480
#else
#define TV_NTSC_Buf_width              640
#define TV_NTSC_Buf_height             448
#define TV_NTSC_Win_width              640
#define TV_NTSC_Win_height             448
#endif


//Different FSADJ need different configuration
//HW FSADJ 3.6 K
#define BURST_LEVEL_3_6                 0x38
#define BLAND_LEVEL_3_6                 0x67
#define Y_SCALE_3_6                     0x78
#define CB_SCALE_3_6                    0x87
#define CR_SCALE_3_6                    0xBC

//HW FSADJ 3.3 K
#define BURST_LEVEL_3_3                 0x4B
#define BLAND_LEVEL_3_3                 0x5F
#define Y_SCALE_3_3                     0x6B
#define CB_SCALE_3_3                    0x7B
#define CR_SCALE_3_3                    0xA9


//HW FSADJ 3.0 K
#define BURST_LEVEL_3_0                 0x33
#define BLAND_LEVEL_3_0                 0x60
#define Y_SCALE_3_0                     0x76
#define CB_SCALE_3_0                    0x72
#define CR_SCALE_3_0                    0x9A


//static void delay_ntsc(UINT32 times);

static TV_DEVICE_OBJ TVNTSC_Obj = {270, LCD_SRCCLK_PLL2, TV_FSADJ_DEFAULT};

//static void delay_ntsc(UINT32 times)
//{
//    for ( ; times>0; times--)
//        ;
//}

/**
    Init TV/NTSC device object.

    Init TV/NTSC device object for proper clock configuration.

    @param[in] ptvntscobj   Pointer to TV device object.

    @return void
*/
void InitTVNTSC_OBJ(TV_DEVICE_OBJ *ptvntscobj)
{
    TVNTSC_Obj = *ptvntscobj;
}

/**
    Open TV/NTSC display.

    Open TV/NTSC display.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

   @param[in]   psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

   @return  void
*/
void OpenTVNTSC(LCDSIZE *psizeDisplay)
{
    //Different FSADJ need different configuration
    UINT8 uiBurstLevel = 0;
    UINT8 uiBlandLevel = 0;
    UINT8 uiYScale = 0, uiCbScale = 0, uiCrScale = 0;
    UINT8 uiClockDiv;

    if (TVNTSC_Obj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {

        TVNTSC_Obj.uiIDESrcCLK = pll_getPLL1Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
    }
    else
    {

        TVNTSC_Obj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }

    //#NT#2011/06/28#Klins Chen -begin
    //#NT#Apply new pinmux control
    if ((pinmux_getDispMode(PINMUX_FUNC_ID_TV)&PINMUX_TV_HDMI_CFG_MASK) == PINMUX_TV_HDMI_CFG_NORMAL)
        pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_GPIO);
    //pinmux_select_lcd(PINMUX_LCD_GPIO);
    //#NT#2011/06/28#Klins Chen -end

    //set to 27MHz
    uiClockDiv = (float)TVNTSC_Obj.uiIDESrcCLK/(float)27/(float)2;
    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    // Wait for Power Stable
    //delay_ntsc(0x10000);

    if(ide_open() != E_OK)
        return;

    ide_setICST(FALSE);
    ide_setDithering(FALSE, FALSE);
    ide_setDevice(DISPLAY_DEVICE_TV);
    ide_setPdir(IDE_PDIR_RGB);
    ide_setOdd(IDE_LCD_R);
    ide_setEven(IDE_LCD_G);
    ide_setHsInv(FALSE);
    ide_setVsInv(FALSE);
    ide_setHvldInv(FALSE);
    ide_setVvldInv(FALSE);
    ide_setClkInv(FALSE);
    ide_setFldInv(FALSE);
    ide_setRgbd(FALSE);
    ide_setThrough(FALSE);
    //ide_set_shw_st();

    #if 0
    ide_setHorTiming(0x7F, 0x6B3, 0xFA-0x19, 0x699-0x19);
    ide_setVerTiming(0x05, 0x20C, 0x16, 0x20B, 0x17, 0x20C);
    #else
    ide_set_hor_timing(0x7F, 0x6B3, 0x144, 0x69D);
    ide_set_ver_timing(0x05, 0x20C, 0x32, 0x205, 0x33, 0x206);
    #endif
    ide_setInterlace(TRUE);
    ide_setStartField(TRUE);

    ide_setCtrst(0x40);
    ide_setBrt(0x00);
    ide_setCmults(0x40);
    ide_setCex(FALSE);
    ide_setTvMode(IDE_TV_NTSC_M);
    ide_setTvBurst(0x00);//0xFC = -4.
    ide_setTvSlaveMode(TRUE);

    ide_setTvChromaFilter(TRUE, NULL);


    //Different FSADJ need different configuration
    if(TVNTSC_Obj.uiTVFSADJ == TV_FSADJ_3_6_K)
    {
        uiBurstLevel = BURST_LEVEL_3_6;
        uiBlandLevel = BLAND_LEVEL_3_6;
        uiYScale = Y_SCALE_3_6;
        uiCbScale = CB_SCALE_3_6;
        uiCrScale = CR_SCALE_3_6;
    }
    else if(TVNTSC_Obj.uiTVFSADJ == TV_FSADJ_3_0_K)
    {
        uiBurstLevel = BURST_LEVEL_3_0;
        uiBlandLevel = BLAND_LEVEL_3_0;
        uiYScale = Y_SCALE_3_0;
        uiCbScale = CB_SCALE_3_0;
        uiCrScale = CR_SCALE_3_0;
    }
    else
    {
        uiBurstLevel = BURST_LEVEL_3_3;
        uiBlandLevel = BLAND_LEVEL_3_3;
        uiYScale = Y_SCALE_3_3;
        uiCbScale = CB_SCALE_3_3;
        uiCrScale = CR_SCALE_3_3;
    }

    ide_setTvSearchEn(TRUE);
    ide_setTvCkpnPol(TRUE);
    ide_setTvNtscSetupLevel(0x27);                 //RC: 0x20

    ide_setTvBrl(uiBurstLevel);                    //RC: 0x38
    ide_setTvBll(uiBlandLevel);                    //RC: 0x5B
    ide_setTvScale(uiYScale, uiCbScale, uiCrScale);//RC: (,,)
    ide_setTvSela(0x3);
    //ide_set_sif_startend(0x00, 0x00);
    //ide_set_tv_amp(0x2D, 0x10, 0x10, 0x10);

    #if _FPGA_EMULATION_
    ide_setTvPowerDown(TRUE);
    #else
    ide_setTvPowerDown(FALSE);
    #endif


    psizeDisplay->uiWidth       = TV_NTSC_Buf_width;
    psizeDisplay->uiHeight      = TV_NTSC_Buf_height;
    psizeDisplay->uiWinWidth    = TV_NTSC_Win_width;
    psizeDisplay->uiWinHeight   = TV_NTSC_Win_height;

}

/**
    Close TV/NTSC display.

    Call this api to close the TV/NTSC device.

    @return void
*/
void CloseTVNTSC(void)
{
    // Disable IDE
    ide_setEn(FALSE);
    ide_waitFrameEnd();

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);

    ide_setTvPowerDown(TRUE);
}
