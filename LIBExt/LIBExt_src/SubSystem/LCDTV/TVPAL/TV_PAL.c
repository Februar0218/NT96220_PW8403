#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Pll.h"
#include "debug.h"
#include "top.h"

#if 0
#define TV_PAL_Buf_width               720
#define TV_PAL_Buf_height              576
#define TV_PAL_Win_width               720
#define TV_PAL_Win_height              576
#else
#define TV_PAL_Buf_width               640
#define TV_PAL_Buf_height              528
#define TV_PAL_Win_width               640
#define TV_PAL_Win_height              528
#endif

//Different FSADJ need different configuration
//HW FSADJ 3.6 K
#define BURST_LEVEL_3_6                 0x47
#define BLAND_LEVEL_3_6                 0x6F
#define Y_SCALE_3_6                     0x7D
#define CB_SCALE_3_6                    0xA8
#define CR_SCALE_3_6                    0xEC

//HW FSADJ 3.3 K
#define BURST_LEVEL_3_3                 0x44
#define BLAND_LEVEL_3_3                 0x65
#define Y_SCALE_3_3                     0x76
#define CB_SCALE_3_3                    0x95
#define CR_SCALE_3_3                    0xD9

//HW FSADJ 3.0 K
#define BURST_LEVEL_3_0                 0x26
#define BLAND_LEVEL_3_0                 0x65
#define Y_SCALE_3_0                     0x75
#define CB_SCALE_3_0                    0x76
#define CR_SCALE_3_0                    0xA4

//static void delay_pal(UINT32 times);

static TV_DEVICE_OBJ TVPAL_Obj = {270, LCD_SRCCLK_PLL2, TV_FSADJ_DEFAULT};

//static void delay_pal(UINT32 times)
//{
//    for ( ; times>0; times--)
//        ;
//}

/**
    Init TV/PAL device object.

    Init TV/PAL device object for proper clock configuration.

    @param[in] ptvpalobj    Pointer to TV device object.

    @return void
*/
void InitTVPAL_OBJ(TV_DEVICE_OBJ *ptvpalobj)
{
    TVPAL_Obj = *ptvpalobj;
}

/**
    Open TV/PAL display.

    Open TV/PAL display.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

   @param[in]  psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

   @return  void
*/
void OpenTVPAL(LCDSIZE *psizeDisplay)
{
    //Different FSADJ need different configuration
    UINT8 uiBurstLevel = 0;
    UINT8 uiBlandLevel = 0;
    UINT8 uiYScale = 0, uiCbScale = 0, uiCrScale = 0;
    UINT8 uiClockDiv;


    if (TVPAL_Obj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {
        TVPAL_Obj.uiIDESrcCLK = pll_getPLL1Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
    }
    else
    {
        TVPAL_Obj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
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
    uiClockDiv = (float)TVPAL_Obj.uiIDESrcCLK/(float)27/(float)2;
    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    // Wait for Power Stable
    //delay_pal(0x10000);

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
    ide_setHorTiming(0x01, 0x6BF, 0xF4, 0x693);//0x15C, 0x69B);
    ide_setVerTiming(0x04, 0x270, 0x20, 0x25F, 0x21, 0x260);//0x040, 0x267, 0x041, 0x268);
    #else
    ide_set_hor_timing(0x01, 0x6BF, 0x15C, 0x69B);
    ide_set_ver_timing(0x04, 0x270, 0x040, 0x267, 0x041, 0x268);
    #endif

    ide_setInterlace(TRUE);
    ide_setStartField(TRUE);

    ide_setCtrst(0x40);
    ide_setBrt(0x00);
    ide_setCmults(0x40);
    ide_setCex(FALSE);

    ide_setTvMode(IDE_TV_PAL_BDGHI);
    ide_setTvBurst(0x00);//0xEC = -20
    ide_setTvSlaveMode(TRUE);

    ide_setTvChromaFilter(TRUE, NULL);


    //Different FSADJ need different configuration
    if(TVPAL_Obj.uiTVFSADJ == TV_FSADJ_3_6_K)
    {
        uiBurstLevel = BURST_LEVEL_3_6;
        uiBlandLevel = BLAND_LEVEL_3_6;
        uiYScale = Y_SCALE_3_6;
        uiCbScale = CB_SCALE_3_6;
        uiCrScale = CR_SCALE_3_6;
    }
    else if(TVPAL_Obj.uiTVFSADJ == TV_FSADJ_3_0_K)
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
    ide_setTvNtscSetupLevel(0x00);

    ide_setTvBrl(uiBurstLevel);
    ide_setTvBll(uiBlandLevel);
    ide_setTvScale(uiYScale, uiCbScale, uiCrScale);
    ide_setTvSela(0x3);

    //ide_set_sif_startend(0x00, 0x00);

    //ide_set_tv_amp(0x2D, 0x11, 0x11, 0x10);

    #if _FPGA_EMULATION_
    ide_setTvPowerDown(TRUE);
    #else
    ide_setTvPowerDown(FALSE);
    #endif


    psizeDisplay->uiWidth       = TV_PAL_Buf_width;
    psizeDisplay->uiHeight      = TV_PAL_Buf_height;
    psizeDisplay->uiWinWidth    = TV_PAL_Win_width;
    psizeDisplay->uiWinHeight   = TV_PAL_Win_height;

    //delay_pal(0x2000);
}

/**
    Close TV/PAL display.

    Call this api to close the TV/PAL device.

    @return void
*/
void CloseTVPAL(void)
{
    // Disable IDE
    ide_set_en(FALSE);
    ide_waitFrameEnd();

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);

    ide_setTvPowerDown(TRUE);
}
