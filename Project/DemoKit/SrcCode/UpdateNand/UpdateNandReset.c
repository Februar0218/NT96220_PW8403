/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UpdateNandReset.c
    @ingroup    mIPRJAPInit

    @brief      Reset CPU and all modules to run the FW
                Reset CPU and all modules to run the FW.

    @note       Nothing.

    @date       2006/04/17
*/

/** \addtogroup mIPRJAPInit*/
//@{

#include <string.h>
#include "Kernel.h"
#include "SysCfg.h"
#if (_CHIP_ == _CHIP_432_ || _CHIP_ == _CHIP_611_)
#include "sie_Lib.h"
#elif (_CHIP_ == _CHIP_200_)
#include "sie_Lib200.h"
#elif (_CHIP_ == _CHIP_433_)
#include "sie_lib433.h"
#elif (_CHIP_ == _CHIP_210_)
#include "sie_lib210.h"
#endif
#include "ImageAlgInfor.h"
#include "Pll.h"
#include "Audio.h"
#include "IDE.h"
#include "Interrupt.h"
#include "UpdateNand.h"
#include "FileSysTsk.h"
#include "GPIOMapping.h"

// Don't reset the following modules
#define UPDNAND_RESET_MODULE_MASK   (WDMA_RSTN | \
                                     BOOT_RSTN | \
                                     BRG_RSTN | \
                                     AHBC_RSTN | \
                                     TOP_RSTN | \
                                     RTC_RSTN | \
                                     UART_RSTN)
// Don't disable the following modules' clock
#define UPDNAND_CLK_MODULE_MASK1    (BOOT_HCLKEN | \
                                     USB_HCLKEN | \
                                     DMA_CLKEN)
#define UPDNAND_CLK_MODULE_MASK2    (UART_CLKEN | \
                                     RTC_PCLKEN)

extern void start(void);

/**
    Reset and run

    Reset and run

    @param uiStorage    Read FW from what storage, refer to PrimaryTsk.h
        - @bPRIMARY_STGTYPE_NAND:   NAND
        - @bPRIMARY_STGTYPE_SD:     SD Card
    @return void
*/
void UpdateNand_Reset(UINT32 uiAddress, UINT32 uiFileLen)
{
#if (_LOADFW_INMAIN_ == ENABLE)
    UINT32 i;
  #if (_CHIP_ == _CHIP_433_)
    UINT32 pixelcon;
  #endif

    // Close file system
    FilesysClose(FST_TIME_INFINITE);

    // Stop audio
    if (aud_isBusy() == TRUE)
    {
        aud_stop();
        aud_close();
    }

  #if (_CHIP_ == _CHIP_433_)
    // Need to check NT96433 sie function
    pixelcon = sie_ReadPixelCounterStatus();
    TimerDelayUs(1);
    if (pixelcon != sie_ReadPixelCounterStatus())
  #else
    // Disable image pipeline
    if (sie_isEnabled() == TRUE &&
        sie_CheckFunction_Enable(SIE_CROP_EN) == TRUE &&
        pll_isClockEnabled(SIE_PXCLK) == TRUE &&
        // We have to check CCD's clock too.
        // Since demo board's CCD clock is an external OSC,
        // we don't check the clock is enabled or disabled now.
        /*
        GPIOMap_IsCCDClockOn() == TRUE &&
        */
        GPIOMap_IsSensorPowerOn() == TRUE)
  #endif

    {
        StopPreviewDramData();
    }

    // Turn off CCD power
    GPIOMap_TurnOffSensorPower();

    // Turn off LCD backlight
    //GPIOMap_TurnOffLCDBacklight();

    // Turn off lens motor power
    gpio_setPin(GPIO_LENS_RST);

    // Disable IDE
    ide_wait_frame_end();
    ide_set_en(FALSE);

    // Disable all interrupts
    int_disable_multi(_INT_ALL);

    // Disable all modules' clock except some (UPDNAND_CLK_MODULE_MASK1, UPDNAND_CLK_MODULE_MASK2)
    pll_setClockEnableRegister1(pll_getClockEnableRegister1() & UPDNAND_CLK_MODULE_MASK1);
    pll_setClockEnableRegister2(pll_getClockEnableRegister2() & UPDNAND_CLK_MODULE_MASK2);

    //#NT#2008/05/30#Stephen Wang -begin
    // Reset all modules except some (UPDNAND_RESET_MODULE_MASK)
    pll_enableSystemReset1(0xFFFFFFFF & ~UPDNAND_RESET_MODULE_MASK);
    pll_disableSystemReset1(0xFFFFFFFF & ~UPDNAND_RESET_MODULE_MASK);

  #if (_CHIP_ == _CHIP_433_)
    pll_enableSystemReset2(0xFFFFFFFF);
    pll_disableSystemReset2(0xFFFFFFFF);
  #elif (_CHIP_ == _CHIP_613_)
    pll_enableSystemReset2(0xFFFFFFFF);
    pll_disableSystemReset2(0xFFFFFFFF);
  #endif
    //#NT#2008/05/30#Stephen Wang -end

    // Copy FW code to 0x00000000
    // Since original main code will be overwriten by new main code,
    // we can't call ANSI C memcpy() API to copy data.
    for (i=0; i<uiFileLen; i+=4)
    {
        *(UINT32 *)i = *(UINT32 *)(uiAddress + i);
    }

    // Handle I/D Cache
    // Since original main code will be overwriten by new main code,
    // we can't call APIs in Cache.h to handle cache.
    __asm
    {
        MOV     r0, #0

        // Clear and invalidate data cache
        MCR     p15, 0, r0, c7, c14, 0

        // Drain write buffer
        MCR     p15, 0, r0, c7, c10, 4

        // Invalidate instruction cache
        MCR     p15, 0, r0, c7, c5, 0

        // Disable instruction / data cache and MPU
        // FA510 CFGR default value is 0x00000078
        MOV     r0, #0x00000078
        MCR     p15, 0, r0, c1, c0, 0
    }

    // Branch to new main code reset vector and run new main code
    start();
#endif
}

//@}
