/*
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       SystemClock.c
    @ingroup    mIPRJAPCfg

    @brief      Change system clock (CPU/AHB/DMA)
                Change system clcok based on different operation mode

    @note       Nothing.
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SystemClock.h"
#include "DMA.h"
#include "Clock.h"
#include "top.h"
#include "pll.h"
#include "ide.h"
#include "Pad.h"
#include "GPIOMapping.h"
#include "ImageAlgInfor.h"
#include "sie_Lib.h"
#include "FileSysTsk.h"
#include "Audio.h"
#include "SystemInit.h"
#include "Alg_IPLMode.h"
#include "KeyScanTsk.h"

// Internal compile option for IDE control in DMA clock change
// Because this compile option is not a feature selection and is implementation dependent,
// we do not put this setting in global header.
// method 1, wait VD only.  But if not all interrupts are disabled and there is task scheduling during VD period,
// this method might fail because it may leave VD period after return from ISR or scheduler,
// and this might cause system crash potentially
#define WAIT_IDE_VD     1
// method 2, disable/enable IDE, this method might cause potential very short white blinking
#define WAIT_IDE_IDE    2
// method 3, disable/enable Video/OSD buffer, this method might cause potential very short black blinking
#define WAIT_IDE_WIN    3
#define CHANGE_DMA_CLOCK_IDE_OPT        WAIT_IDE_VD

//#NT#2010/09/14#Cliff Lin -begin
//#NT#Support for New Dram controller
#define SDRAM_PARAM_VERSION         DMA_VER_220_000   ///< Use Sdram paramter version


#define DDRI_DMA80MHz_CONFIG                0x00000040
#define DDRI_DMA80MHz_TIMING0               0x00101131
#define DDRI_DMA80MHz_TIMING1               0x02700006
#define DDR1_DMA80MHz_DELAYCHAIN            0x80C30F00

#define DDRI_DMA120MHz_CONFIG               0x00000040
#define DDRI_DMA120MHz_TIMING0              0x00102142
#define DDRI_DMA120MHz_TIMING1              0x03A8000A
#define DDR1_DMA120MHz_DELAYCHAIN           0x80820E00

#define DDRI_DMA160MHz_CONFIG               0x00000050
#define DDRI_DMA160MHz_TIMING0              0x00202162
#define DDRI_DMA160MHz_TIMING1              0x04E0000C
#define DDR1_DMA160MHz_DELAYCHAIN           0x90C30F00

#define SDR_DMA80MHz_CONFIG                 0x00000145  //CL=2T
#define SDR_DMA80MHz_TIMING0                0x00101131
#define SDR_DMA80MHz_TIMING1                0x04E20004
#define SDR_DMA80MHz_DELAYCHAIN             0x90A28F00

#define SDR_DMA120MHz_CONFIG                0x00000165  //CL=3T
#define SDR_DMA120MHz_TIMING0               0x00102162
#define SDR_DMA120MHz_TIMING1               0x09C4000A
#define SDR_DMA120MHz_DELAYCHAIN            0x90C30F00

#define SDR_DMA160MHz_CONFIG                0x00000165  //CL=3T
#define SDR_DMA160MHz_TIMING0               0x00102162
#define SDR_DMA160MHz_TIMING1               0x09C4000A
#define SDR_DMA160MHz_DELAYCHAIN            0x81860400

//Input : Delay chain  Output : Delay chain  PARAM1-->0x98700014   PARAM2-->0x9800000
#define DELAY_DELAY_DMA48MHz_PARAM1     0
#define DELAY_DELAY_DMA48MHz_PARAM2     0
#define DELAY_DELAY_DMA60MHz_PARAM1     0
#define DELAY_DELAY_DMA60MHz_PARAM2     0

// Static variables
static UINT32       uiDMAClock;
static UINT32       uiCPUClock;
static UINT32       uiAHBClock;
static UINT32       uiDRAMType;
static BOOL         bIsCropEnabled;
static BOOL         bIDEIsEnabled;
#if CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_WIN
static UINT32       uiIDEWinEN;
#endif
static BOOL         bIsAudioBusy;

/**
  Get 120 MHz DMA clock setting

  @param[in/out] pDMASetting: DMA timing setting

  @return void
*/
void SysClk_GetDMA120MHzSetting(PDMA_SETTING pDMASetting)
{
    if (uiDRAMType == DMA_DRAM_TYPE_DDR1)
    {
        //debug_err(("SysClk: current 450 doesn't support DDR-I yet\r\n"));
        pDMASetting->uiConfig      = DDRI_DMA120MHz_CONFIG;
        pDMASetting->uiTiming0     = DDRI_DMA120MHz_TIMING0;
        pDMASetting->uiTiming1     = DDRI_DMA120MHz_TIMING1;
        pDMASetting->uiEngineer    = 0x00000009;
        pDMASetting->uiModeReg     = 0x00110045;
        pDMASetting->uiDelayChain  = DDR1_DMA120MHz_DELAYCHAIN;

    }
    else // SDR
    {
        pDMASetting->uiConfig      = SDR_DMA120MHz_CONFIG;
        pDMASetting->uiTiming0     = SDR_DMA120MHz_TIMING0;
        pDMASetting->uiTiming1     = SDR_DMA120MHz_TIMING1;
        //#NT#2011/07/08#Steven Wang -begin
        pDMASetting->uiEngineer    = 0x06000009;
        pDMASetting->uiModeReg     = 0x00190045;
        //#NT#2011/07/08#Steven Wang -end
        pDMASetting->uiDelayChain  = SDR_DMA120MHz_DELAYCHAIN;
    }
}

/**
  Get 160 MHz DMA clock setting

  @param[in/out] pDMASetting: DMA timing setting

  @return void
*/
void SysClk_GetDMA160MHzSetting(PDMA_SETTING pDMASetting)
{
    if (uiDRAMType == DMA_DRAM_TYPE_DDR1)
    {
        pDMASetting->uiConfig      = DDRI_DMA160MHz_CONFIG;
        pDMASetting->uiTiming0     = DDRI_DMA160MHz_TIMING0;
        pDMASetting->uiTiming1     = DDRI_DMA160MHz_TIMING1;
        pDMASetting->uiEngineer    = 0x00000009;
        pDMASetting->uiModeReg     = 0x00310045;
        pDMASetting->uiDelayChain  = DDR1_DMA160MHz_DELAYCHAIN;
    }
    else
    {
        pDMASetting->uiConfig      = SDR_DMA160MHz_CONFIG;
        pDMASetting->uiTiming0     = SDR_DMA160MHz_TIMING0;
        pDMASetting->uiTiming1     = SDR_DMA160MHz_TIMING1;
        pDMASetting->uiEngineer    = 0x06000009;
        pDMASetting->uiModeReg     = 0x00190045;
        pDMASetting->uiDelayChain  = SDR_DMA160MHz_DELAYCHAIN;
    }
}


/**
  Get 80 MHz DMA clock setting

  @param[in/out] pDMASetting: DMA timing setting

  @return void
*/
void SysClk_GetDMA80MHzSetting(PDMA_SETTING pDMASetting)
{
    if (uiDRAMType == DMA_DRAM_TYPE_DDR1)
    {
        pDMASetting->uiConfig      = DDRI_DMA80MHz_CONFIG;
        pDMASetting->uiTiming0     = DDRI_DMA80MHz_TIMING0;
        pDMASetting->uiTiming1     = DDRI_DMA80MHz_TIMING1;
        pDMASetting->uiEngineer    = 0x00000009;
        pDMASetting->uiModeReg     = 0x00110045;
        pDMASetting->uiDelayChain  = DDR1_DMA80MHz_DELAYCHAIN;
    }
    else
    {
        pDMASetting->uiConfig      = SDR_DMA80MHz_CONFIG;
        pDMASetting->uiTiming0     = SDR_DMA80MHz_TIMING0;
        pDMASetting->uiTiming1     = SDR_DMA80MHz_TIMING1;
        pDMASetting->uiEngineer    = 0x06000009;
        pDMASetting->uiModeReg     = 0x00110045;
        pDMASetting->uiDelayChain  = SDR_DMA80MHz_DELAYCHAIN;
    }
}

/**
    Pause all modules which will affect the stability of system clock change.
*/
static void SysClk_PauseAll(void)
{
    //IPL_OBJ IplObj;

    debug_ind(("SysClk_PauseAll enter\r\n"));

    // Ask file system to pause any read/write
    FilesysPause();
    FilesysWaitPaused();

    // Pause audio
    bIsAudioBusy = aud_isBusy();
    if (bIsAudioBusy == TRUE)
    {
        //debug_ind(("Pause Audio\r\n"));

        aud_pause();
    }

    // Pause image pipeline
    if (sie_isEnabled() == TRUE &&                          // SIE is enabled
        pll_isClockEnabled(SIE_PXCLK) == TRUE &&            // Pixel clock is enabled
    #if (SYSINIT_OUTPUT_MCLK == ENABLE)
        //pinmux_get_mclk() == PINMUX_MCLK &&                 // CCD clock is on
        pll_isClockEnabled(SIE_MCLK) == TRUE &&
    #endif
        GPIOMap_IsSensorPowerOn() == TRUE)

    {
        //debug_ind(("Pause SIE\r\n"));

        bIsCropEnabled = TRUE;
        //#NT#2007/08/09Adrian Jiang -begin
        //#Stop ImagePipeline properly.
        //StopPreviewDramData();
        //IplObj.uiCmd = IPL_CMD_CHG_MODE;
        //IplObj.Mode = IPL_MODE_OFF;
        //IPL_SetCmd(&IplObj);
        IPP_PauseEng_SysclkTest();
        //#NT#2007/08/09#Adrian Jiang -end
    }
    else
    {
        bIsCropEnabled = FALSE;
    }

    // Pause IDE
    // Although do ide_wait_frame_end() without disable IDE seems work in most cases,
    // Chances are that it may be interrupted between ide_wait_frame_end and the following
    // clk_changeDMA() in some implementation with different tasks configuration.
    // So it is safe to always disable IDE before calling to clk_changeDMA().
    // However, it will have LCD display to be freezing or flashing for two frames time, so
    // care must be taken in switching clock frequency.
    bIDEIsEnabled = ide_get_en();
    if (bIDEIsEnabled)
    {
        //debug_ind(("Pause IDE\r\n"));

#if CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_VD
        ide_wait_frame_end();
#elif CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_WIN
        uiIDEWinEN = ide_get_window_en();
        ide_set_allwindow_dis();
        ide_set_load();
        ide_wait_frame_end();
#elif CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_IDE
        ide_wait_frame_end();
        ide_set_en(FALSE);
#endif
    }

    debug_ind(("SysClk_PauseAll leave\r\n"));
}

/**
    Resume modules after system clock change.
*/
static void SysClk_ResumeAll(void)
{
    debug_ind(("SysClk_ResumeAll enter\r\n"));

    // Resume file system read / write
    FilesysResume();

    // Resume audio
    if (bIsAudioBusy == TRUE)
    {
        //debug_ind(("Resume Audio\r\n"));

        aud_resume();
    }

    // Resume image pipeline
    if (bIsCropEnabled)
    {
        //debug_ind(("Resume SIE\r\n"));

        //#NT#2007/08/09Adrian Jiang -begin
        //#Because Image Pipeline will be start at Set2Preview()/Set2Video()/Set2PCC latter,
        //#we do't need to restart Imagepipeline here.
        //sie_Function_Enable(TRUE, SIE_CROP_EN);
        //SensorDelayVD(1);
        //#NT#2007/08/09#Adrian Jiang -end
    }

    if (bIDEIsEnabled)
    {
        //debug_ind(("Resume IDE\r\n"));

#if CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_VD
        //ide_wait_frame_end();
#elif CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_WIN
        ide_set_allwindow_en(uiIDEWinEN);
        ide_set_load();
        ide_wait_frame_end();
#elif CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_IDE
        ide_set_en(TRUE);
        ide_wait_frame_end();
#endif
    }

    debug_ind(("SysClk_ResumeAll leave\r\n"));
}

/**
    Init system clock.

    @note This function should be called in SysInit_InitEssentialHW().

    @return void
*/
void SysClk_Init(void)
{
    uiDMAClock = pll_getClockRate(PLL_CLKSEL_DMA);
    uiCPUClock = pll_getClockRate(PLL_CLKSEL_CPU);
    uiAHBClock = pll_getClockRate(PLL_CLKSEL_AHB);
 //#NT#2009/11/03#Scottie -begin
 //#NT#Add for checking DRAM type in system init
    uiDRAMType = dma_getDRAMType();
//#NT#2009/11/03#Scottie -end

    clk_open();
}

/**
    Change system clock.

    @note This function need to be customized to change specific clock for
        respective modes.

    @param uiSpeedID Clock speed ID for respective mode
    @return TRUE if successful
*/
BOOL SysClk_SetClk(UINT32 uiSpeedID)
{
    UINT32      uiNewDMAClock, uiNewCPUClock, uiNewAHBClock;
    DMA_SETTING DMASetting;

    debug_err(("SysClk_SetClk enter\r\n"));

    switch (uiSpeedID)
    {
    case SPEED_ID_USBMSDCVDRCMD:
        // CPU = 160 MHz, AHB = 80 MHz, DMA = 160 MHz
        uiNewCPUClock = PLL_CLKSEL_CPU_160;
        uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK_D2;
        uiNewDMAClock = PLL_CLKSEL_DMA_160;
        if (uiDMAClock != uiNewDMAClock)
        {
            SysClk_GetDMA160MHzSetting(&DMASetting);
        }
        break;

    case SPEED_ID_USBMSDC:
    case SPEED_ID_USBSICD:
        // CPU = 80 MHz, AHB = 80 MHz, DMA = 160 MHz
        uiNewCPUClock = PLL_CLKSEL_CPU_80;
        uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK;
        uiNewDMAClock = PLL_CLKSEL_DMA_160;
        if (uiDMAClock != uiNewDMAClock)
        {
            SysClk_GetDMA160MHzSetting(&DMASetting);
        }
        break;

    case SPEED_ID_USBPCC:
    case SPEED_ID_PHOTO:
    case SPEED_ID_CAPTURE:
    case SPEED_ID_VIDEO:
    case SPEED_ID_PLAYBACK:
        // CPU = 160 MHz, AHB = 160 MHz, DMA = 160 MHz
        uiNewCPUClock = PLL_CLKSEL_CPU_160;
        if (KeyScan_IsUSBPlug())
        {
            // AHB should be smaller than 160 MHz as USB plugged
            uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK_D2; // AHB = 80 MHz
        }
        else
        {
        uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK;
        }
        uiNewDMAClock = PLL_CLKSEL_DMA_160;
        if (uiDMAClock != uiNewDMAClock)
        {
            SysClk_GetDMA160MHzSetting(&DMASetting);

        }
        break;

    case SPEED_ID_MP3:
        // CPU = 240 MHz, AHB = 120 MHz, DMA = 160 MHz
        uiNewCPUClock = PLL_CLKSEL_CPU_240;
        uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK_D2;
        uiNewDMAClock = PLL_CLKSEL_DMA_160;
        if (uiDMAClock != uiNewDMAClock)
        {
            SysClk_GetDMA160MHzSetting(&DMASetting);
        }
        break;

    default:
        return FALSE;
    }

    // Only change clock if new clock doesn't match old clock
    if (uiNewDMAClock != uiDMAClock ||
        uiNewCPUClock != uiCPUClock ||
        uiNewAHBClock != uiAHBClock)
    {
        if (uiNewDMAClock != uiDMAClock)
        {
            debug_err(("Change DMA %d\r\n",uiNewDMAClock));
            SysClk_PauseAll();
            clk_changeDMA(uiNewDMAClock, NULL, &DMASetting);
            SysClk_ResumeAll();
            //#NT#2011/06/14#Ethan Lau -begin
            //#NT#ipp ctrl for clk chg temporilly.
            if(uiSpeedID == SPEED_ID_CAPTURE)
            {
                IPP_StartEng_SysclkTest();
            }
            //#NT#2011/06/14#Ethan Lau -end
            uiDMAClock = uiNewDMAClock;
        }

        if (uiNewCPUClock != uiCPUClock ||
            uiNewAHBClock != uiAHBClock)
        {
            debug_ind(("Change CPU/AHB %d\r\n", uiNewCPUClock));

            clk_changeCPUAHB(uiNewCPUClock, uiNewAHBClock);
            uiCPUClock = uiNewCPUClock;
            uiAHBClock = uiNewAHBClock;
        }
    }

    debug_err(("SysClk_SetClk leave\r\n"));

    return TRUE;
}

//@}
