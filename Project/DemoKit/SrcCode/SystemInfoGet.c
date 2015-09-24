/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemInfoGet.c
    @ingroup    GetSystemInfo

    @brief      Get System information
                This file is getting System information

    @note       Nothing.

    @version    V1.00.000
    @author     Shirley Cheng
    @date       2005/12/20
*/

/** \addtogroup GetSystemInfo */
//@{

#include <stdio.h>
#include <string.h>
#include "Debug.h"
#include "SystemInfoGet.h"

#if (_SYSINFO_MODE_ == ENABLE)

// Global varable define
UINT16 g_uiPLL1_Freq;
UINT16 g_uiPLL2_Freq;
UINT16 g_uiFCLK_Freq;
UINT16 g_uiIDESrc_Freq;

typedef __packed struct _SYSTEM_INFO
{
    INT8     *piItemName;
    UINT32   uiRegAddr;
    UINT8    uiRegBitNum;
    UINT8    uiRegBitOffset;
    UINT32   (*puiItemFunc)(UINT32 uivalue);

} SYSTEM_INFO, *PSYSTEM_INFO;

#define inw(addr)                       (*(UINT32 volatile *)(addr))

// Constant symbol define
#define EX_OSC1_96610                   12000    //KHz
#define EX_OSC2_96610                   13500    //KHz

#define EX_OSC1_96611                   12000    //KHz
#define EX_OSC2_96611                   12000    //KHz

#define SYSTEM_END                      NULL
#define UNDEF_VALUE                     0xFFFF
#define RESERVED_VALUE                  0xF0F0

// Define register address
#define CHIP_TOP_CTRL                   0x98100000
#define CHIP_VERSION                    0x98100010
#define CHIP_PLL1                       0x98101000
#define CHIP_PLL2                       0x98101004
#define CLOCK_OFF_10                    0x98101010
#define CLOCK_OFF_14                    0x98101014
#define CLOCK_OFF_20                    0x98101020
#define CLOCK_OFF_24                    0x98101024
#define PAD_OFF_0                       0x98102000
#define PAD_OFF_4                       0x98102004
#define PAD_OFF_8                       0x98102008
#define PAD_OFF_C                       0x9810200C
#define PAD_OFF_10                      0x98102010
#define PAD_OFF_14                      0x98102014
#define PAD_OFF_18                      0x98102018
#define PAD_OFF_1C                      0x9810201C
#define PAD_OFF_20                      0x98102020
#define PAD_OFF_24                      0x98102024
#define PAD_OFF_28                      0x98102028
#define PAD_OFF_2C                      0x9810202C
#define PAD_OFF_30                      0x98102030
#define PAD_OFF_34                      0x98102034
#define IDE_OFF_F8                      0x9a8000f8

//define register bit number and bit offset
// top control
#define DRAMSIZE_BIT_NUM                3
#define DRAMSIZE_BIT_OFF                3

// clock gen
#define FCLK_BIT_NUM                    3
#define FCLK_BIT_OFF                    0
#define HCLK_BIT_NUM                    2
#define HCLK_BIT_OFF                    4
#define PCLK_BIT_NUM                    3
#define PCLK_BIT_OFF                    8
#define DMA_BIT_NUM                     3
#define DMA_BIT_OFF                     12
#define IPE_BIT_NUM                     2
#define IPE_BIT_OFF                     16
#define IME_BIT_NUM                     3
#define IME_BIT_OFF                     20
#define JPEG_BIT_NUM                    3
#define JPEG_BIT_OFF                    24
#define MPEG_BIT_NUM                    3
#define MPEG_BIT_OFF                    28
#define IDESRC_BIT_NUM                  1
#define IDESRC_BIT_OFF                  3
#define IDE_BIT_NUM                     8
#define IDE_BIT_OFF                     16

//clock enable
#define BIT_NUM_1                       1
#define IPE_EN_BIT_OFF                  3
#define IME_EN_BIT_OFF                  4
#define IDE_EN_BIT_OFF                  5
#define JPEG_EN_BIT_OFF                 7
#define MPEG_EN_BIT_OFF                 8
#define USB_EN_BIT_OFF                  9

#define IDE_TV_DAC_OFF                  16

//pad for 96610
#define DRAM_ADR_DRIVE_BIT_NUM          3
#define DRAM_ADR_DRIVE_BIT_OFF          0
#define DRAM_CLK_DRIVE_BIT_NUM          3
#define DRAM_CLK_DRIVE_BIT_OFF          27
#define DRAM_DATA_DRIVE_BIT_NUM         3
#define DRAM_DATA_DRIVE_BIT_OFF         18
#define CARD_DRIVE_BIT_NUM              3
#define CARD_DRIVE_BIT_OFF              0
#define SENSOR_DRIVE_BIT_NUM            3
#define SENSOR_DRIVE_BIT_OFF            3
#define PWM_DRIVE_BIT_NUM               3
#define PWM_DRIVE_BIT_OFF               6
#define DGPIO_DRIVE_BIT_NUM             3
#define DGPIO_DRIVE_BIT_OFF             9
#define AUDIO_DRIVE_BIT_NUM             3
#define AUDIO_DRIVE_BIT_OFF             12
#define LCD_DRIVE_BIT_NUM               3
#define LCD_DRIVE_BIT_OFF               15
#define OTHER_DRIVE_BIT_NUM             3
#define OTHER_DRIVE_BIT_OFF             21

//pad slew rate of 96610
#define CARD_SLEW_BIT_OFF               0
#define SENSOR_SLEW_BIT_OFF             1
#define PWM_SLEW_BIT_OFF                2
#define DGPIO_SLEW_BIT_OFF              3
#define AUDIO_SLEW_BIT_OFF              4
#define LCD_SLEW_BIT_OFF                5
#define DRAMDATA_SLEW_BIT_OFF           6
#define OTHER_SLEW_BIT_OFF              7
#define DRAMCLK_SLEW_BIT_OFF            8
#define DRAMADR_SLEW_BIT_OFF            9

//pad for 96611
#define DRAM_ADR_DRIVE_BIT_NUM_611      4
#define DRAM_ADR_DRIVE_BIT_OFF_611      4
#define DRAM_CLK_DRIVE_BIT_NUM_611      4
#define DRAM_CLK_DRIVE_BIT_OFF_611      8
#define DRAM_DATA_DRIVE_BIT_NUM_611     4
#define DRAM_DATA_DRIVE_BIT_OFF_611     0
#define CARD_DRIVE_BIT_NUM_611          3
#define CARD_DRIVE_BIT_OFF_611          0
#define SENSOR_DRIVE_BIT_NUM_611        3
#define SENSOR_DRIVE_BIT_OFF_611        3
#define PWM_DRIVE_BIT_NUM_611           3
#define PWM_DRIVE_BIT_OFF_611           6
#define DGPIO_DRIVE_BIT_NUM_611         3
#define DGPIO_DRIVE_BIT_OFF_611         9
#define AUDIO_DRIVE_BIT_NUM_611         3
#define AUDIO_DRIVE_BIT_OFF_611         12
#define LCD_DRIVE_BIT_NUM_611           3
#define LCD_DRIVE_BIT_OFF_611           15
#define DRAM_DQS_PADBIT_NUM_611         3
#define DRAM_DQS_PADBIT_OFF_611         18
#define OTHER_DRIVE_BIT_NUM_611         3
#define OTHER_DRIVE_BIT_OFF_611         21
#define FLCTR_DRIVE_BIT_NUM_611         3
#define FLCTR_DRIVE_BIT_OFF_611         24

//pad slew rate of 96611
#define CARD_SLEW_BIT_OFF_611           0
#define SENSOR_SLEW_BIT_OFF_611         1
#define PWM_SLEW_BIT_OFF_611            2
#define DGPIO_SLEW_BIT_OFF_611          3
#define AUDIO_SLEW_BIT_OFF_611          4
#define LCD_SLEW_BIT_OFF_611            5
#define DRAM_SLEW_BIT_OFF_611           6
#define OTHER_SLEW_BIT_OFF_611          7
#define DRAMDQM0_SLEW_BIT_OFF           8
#define DRAMDQM1_SLEW_BIT_OFF           9
#define DRAMDQS_SLEW_BIT_OFF            10
#define DRAMADR_SLEW_BIT_OFF_611        11
#define DRAMCLK_SLEW_BIT_OFF_611        12

// extern variable


// function
static UINT32 SYSInfo_DRAM_SIZE_96610(UINT32 uivalue);
static UINT32 SYSInfo_DRAM_SIZE_96611(UINT32 uivalue);
static UINT32 SYSInfo_FCLK_Clock(UINT32 uivalue);
static UINT32 SYSInfo_HCLK_Clock(UINT32 uivalue);
static UINT32 SYSInfo_PCLK_Clock(UINT32 uivalue);
static UINT32 SYSInfo_DMA_Clock(UINT32 uivalue);
static UINT32 SYSInfo_IPE_Clock(UINT32 uivalue);
static UINT32 SYSInfo_IME_Clock(UINT32 uivalue);
static UINT32 SYSInfo_JPEG_Clock(UINT32 uivalue);
static UINT32 SYSInfo_MPEG_Clock(UINT32 uivalue);
static UINT32 SYSInfo_MPEG_Clock(UINT32 uivalue);
static UINT32 SYSInfo_IDE_ClockSrc(UINT32 uivalue);
static UINT32 SYSInfo_IDE_Clock(UINT32 uivalue);
static UINT32 SYSInfo_DRAM_ADR_DRIVE(UINT32 uivalue);
static UINT32 SYSInfo_DRAM_ADR_DRIVE_96611(UINT32 uivalue);
static UINT32 SYSInfo_DRAM_CLK_DRIVE(UINT32 uivalue);
static UINT32 SYSInfo_DRAM_CLK_DRIVE_96611(UINT32 uivalue);
static UINT32 SYSInfo_DRAM_DATA_DRIVE(UINT32 uivalue);
static UINT32 SYSInfo_DRAM_DATA_DRIVE_96611(UINT32 uivalue);
static UINT32 SYSInfo_CARD_DRIVE(UINT32 uivalue);
static UINT32 SYSInfo_SENSOR_DRIVE(UINT32 uivalue);
static UINT32 SYSInfo_LCD_DRIVE(UINT32 uivalue);
static UINT32 SYSInfo_FUNC_NOTCONVERT(UINT32 uivalue);

// the system item-list of 96610
const SYSTEM_INFO SystemInfo_96610[] =
{
    //Dram size
    {"DRAM SIZE", CHIP_TOP_CTRL, DRAMSIZE_BIT_NUM, DRAMSIZE_BIT_OFF, SYSInfo_DRAM_SIZE_96610},
    //Module clock
    {"FCLK", CLOCK_OFF_10, FCLK_BIT_NUM, FCLK_BIT_OFF, SYSInfo_FCLK_Clock},
    {"HCLK", CLOCK_OFF_10, HCLK_BIT_NUM, HCLK_BIT_OFF, SYSInfo_HCLK_Clock},
    {"PCLK", CLOCK_OFF_10, PCLK_BIT_NUM, PCLK_BIT_OFF, SYSInfo_PCLK_Clock},
    {"DMA", CLOCK_OFF_10, DMA_BIT_NUM, DMA_BIT_OFF, SYSInfo_DMA_Clock},
    {"IPE", CLOCK_OFF_10, IPE_BIT_NUM, IPE_BIT_OFF, SYSInfo_IPE_Clock},
    {"IME", CLOCK_OFF_10, IME_BIT_NUM, IME_BIT_OFF, SYSInfo_IME_Clock},
    {"JPEG", CLOCK_OFF_10, JPEG_BIT_NUM, JPEG_BIT_OFF, SYSInfo_JPEG_Clock},
    {"MPEG", CLOCK_OFF_10, MPEG_BIT_NUM, MPEG_BIT_OFF, SYSInfo_MPEG_Clock},
    {"IDESrc", CLOCK_OFF_14, IDESRC_BIT_NUM, IDESRC_BIT_OFF, SYSInfo_IDE_ClockSrc},
    {"IDE",  CLOCK_OFF_14, IDE_BIT_NUM, IDE_BIT_OFF, SYSInfo_IDE_Clock},
    //Module clock enable
    {"IPEEn", CLOCK_OFF_20, BIT_NUM_1, IPE_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"IMEEn", CLOCK_OFF_20, BIT_NUM_1, IME_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"IDEEn", CLOCK_OFF_20, BIT_NUM_1, IDE_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"JPEGEn", CLOCK_OFF_20, BIT_NUM_1, JPEG_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"MPEGEn", CLOCK_OFF_20, BIT_NUM_1, MPEG_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"TVDacOff", IDE_OFF_F8, BIT_NUM_1, IDE_TV_DAC_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"USBEn", CLOCK_OFF_20, BIT_NUM_1, USB_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    //PAD Driving
    {"DRAM ADR Drive", PAD_OFF_34, DRAM_ADR_DRIVE_BIT_NUM, DRAM_ADR_DRIVE_BIT_OFF, SYSInfo_DRAM_ADR_DRIVE},
    {"DRAM CLK Drive", PAD_OFF_30, DRAM_CLK_DRIVE_BIT_NUM, DRAM_CLK_DRIVE_BIT_OFF, SYSInfo_DRAM_CLK_DRIVE},
    {"DRAM DATA Drive", PAD_OFF_30, DRAM_DATA_DRIVE_BIT_NUM, DRAM_DATA_DRIVE_BIT_OFF, SYSInfo_DRAM_DATA_DRIVE},
    {"CARD Drive", PAD_OFF_30, CARD_DRIVE_BIT_NUM, CARD_DRIVE_BIT_OFF, SYSInfo_CARD_DRIVE},
    {"SENSOR Drive", PAD_OFF_30, SENSOR_DRIVE_BIT_NUM, SENSOR_DRIVE_BIT_OFF, SYSInfo_SENSOR_DRIVE},
    {"LCD Drive", PAD_OFF_30, LCD_DRIVE_BIT_NUM, LCD_DRIVE_BIT_OFF, SYSInfo_LCD_DRIVE},
    {"PWM Drive", PAD_OFF_30, PWM_DRIVE_BIT_NUM, PWM_DRIVE_BIT_OFF, SYSInfo_LCD_DRIVE},
    {"DGPIO Drive", PAD_OFF_30, DGPIO_DRIVE_BIT_NUM, DGPIO_DRIVE_BIT_OFF, SYSInfo_LCD_DRIVE},
    {"AUDIO Drive", PAD_OFF_30, AUDIO_DRIVE_BIT_NUM, AUDIO_DRIVE_BIT_OFF, SYSInfo_LCD_DRIVE},
    {"OTHER Drive", PAD_OFF_30, OTHER_DRIVE_BIT_NUM, OTHER_DRIVE_BIT_OFF, SYSInfo_LCD_DRIVE},
    //Slew Rate
    {"CARD Slew", PAD_OFF_2C, BIT_NUM_1, CARD_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"SENSOR Slew", PAD_OFF_2C, BIT_NUM_1, SENSOR_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"PWM Slew", PAD_OFF_2C, BIT_NUM_1, PWM_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"DGPIO Slew", PAD_OFF_2C, BIT_NUM_1, DGPIO_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"AUDIO Slew", PAD_OFF_2C, BIT_NUM_1, AUDIO_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"LCD Slew", PAD_OFF_2C, BIT_NUM_1, LCD_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMDATA Slew", PAD_OFF_2C, BIT_NUM_1, DRAMDATA_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"OTHER Slew", PAD_OFF_2C, BIT_NUM_1, OTHER_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMCLK Slew", PAD_OFF_2C, BIT_NUM_1, DRAMCLK_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMADR Slew", PAD_OFF_2C, BIT_NUM_1, DRAMADR_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {SYSTEM_END, 0, 0, 0, NULL}
};


// the system item-list of 96611
const SYSTEM_INFO SystemInfo_96611[] =
{
    //Dram size
    {"DRAM SIZE", CHIP_TOP_CTRL, DRAMSIZE_BIT_NUM, DRAMSIZE_BIT_OFF, SYSInfo_DRAM_SIZE_96611},
    //Module clock
    {"FCLK", CLOCK_OFF_10, FCLK_BIT_NUM, FCLK_BIT_OFF, SYSInfo_FCLK_Clock},
    {"HCLK", CLOCK_OFF_10, HCLK_BIT_NUM, HCLK_BIT_OFF, SYSInfo_HCLK_Clock},
    {"PCLK", CLOCK_OFF_10, PCLK_BIT_NUM, PCLK_BIT_OFF, SYSInfo_PCLK_Clock},
    {"DMA", CLOCK_OFF_10, DMA_BIT_NUM, DMA_BIT_OFF, SYSInfo_DMA_Clock},
    {"IPE", CLOCK_OFF_10, IPE_BIT_NUM, IPE_BIT_OFF, SYSInfo_IPE_Clock},
    {"IME", CLOCK_OFF_10, IME_BIT_NUM, IME_BIT_OFF, SYSInfo_IME_Clock},
    {"JPEG", CLOCK_OFF_10, JPEG_BIT_NUM, JPEG_BIT_OFF, SYSInfo_JPEG_Clock},
    {"MPEG", CLOCK_OFF_10, MPEG_BIT_NUM, MPEG_BIT_OFF, SYSInfo_MPEG_Clock},
    {"IDESrc", CLOCK_OFF_14, IDESRC_BIT_NUM, IDESRC_BIT_OFF, SYSInfo_IDE_ClockSrc},
    {"IDE",  CLOCK_OFF_14, IDE_BIT_NUM, IDE_BIT_OFF, SYSInfo_IDE_Clock},
    //Module clock enable
    {"IPEEn", CLOCK_OFF_20, BIT_NUM_1, IPE_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"IMEEn", CLOCK_OFF_20, BIT_NUM_1, IME_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"IDEEn", CLOCK_OFF_20, BIT_NUM_1, IDE_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"JPEGEn", CLOCK_OFF_20, BIT_NUM_1, JPEG_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"MPEGEn", CLOCK_OFF_20, BIT_NUM_1, MPEG_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"TVDacOff", IDE_OFF_F8, BIT_NUM_1, IDE_TV_DAC_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"USBEn", CLOCK_OFF_20, BIT_NUM_1, USB_EN_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    //PAD Driving
    {"DRAM ADR Drive", PAD_OFF_2C, DRAM_ADR_DRIVE_BIT_NUM_611, DRAM_ADR_DRIVE_BIT_OFF_611, SYSInfo_DRAM_ADR_DRIVE_96611},
    {"DRAM CLK Drive", PAD_OFF_2C, DRAM_CLK_DRIVE_BIT_NUM_611, DRAM_CLK_DRIVE_BIT_OFF_611, SYSInfo_DRAM_CLK_DRIVE_96611},
    {"DRAM DATA Drive", PAD_OFF_2C, DRAM_DATA_DRIVE_BIT_NUM_611, DRAM_DATA_DRIVE_BIT_OFF_611, SYSInfo_DRAM_DATA_DRIVE_96611},
    {"CARD Drive", PAD_OFF_28, CARD_DRIVE_BIT_NUM_611, CARD_DRIVE_BIT_OFF_611, SYSInfo_CARD_DRIVE},
    {"SENSOR Drive", PAD_OFF_28, SENSOR_DRIVE_BIT_NUM_611, SENSOR_DRIVE_BIT_OFF_611, SYSInfo_SENSOR_DRIVE},
    {"PWM Drive", PAD_OFF_28, PWM_DRIVE_BIT_NUM_611, PWM_DRIVE_BIT_OFF_611, SYSInfo_LCD_DRIVE},
    {"DGPIO Drive", PAD_OFF_28, DGPIO_DRIVE_BIT_NUM_611, DGPIO_DRIVE_BIT_OFF_611, SYSInfo_LCD_DRIVE},
    {"AUDIO Drive", PAD_OFF_28, AUDIO_DRIVE_BIT_NUM_611, AUDIO_DRIVE_BIT_OFF_611, SYSInfo_LCD_DRIVE},
    {"LCD Drive", PAD_OFF_28, LCD_DRIVE_BIT_NUM_611, LCD_DRIVE_BIT_OFF_611, SYSInfo_LCD_DRIVE},
    {"DRAM DQS Drive", PAD_OFF_28, DRAM_DQS_PADBIT_NUM_611, DRAM_DQS_PADBIT_OFF_611, SYSInfo_LCD_DRIVE},
    {"OTHER Drive", PAD_OFF_28, OTHER_DRIVE_BIT_NUM_611, OTHER_DRIVE_BIT_OFF_611, SYSInfo_LCD_DRIVE},
    {"FLCTR Drive", PAD_OFF_28, FLCTR_DRIVE_BIT_NUM_611, FLCTR_DRIVE_BIT_OFF_611, SYSInfo_LCD_DRIVE},
     //Slew Rate
    {"CARD Slew", PAD_OFF_2C, BIT_NUM_1, CARD_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"SENSOR Slew", PAD_OFF_2C, BIT_NUM_1, SENSOR_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"PWM Slew", PAD_OFF_2C, BIT_NUM_1, PWM_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"DGPIO Slew", PAD_OFF_2C, BIT_NUM_1, DGPIO_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"AUDIO Slew", PAD_OFF_2C, BIT_NUM_1, AUDIO_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"LCD Slew", PAD_OFF_2C, BIT_NUM_1, LCD_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"DRAM Slew", PAD_OFF_2C, BIT_NUM_1, DRAM_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"OTHER Slew", PAD_OFF_2C, BIT_NUM_1, OTHER_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMDQM0 Slew", PAD_OFF_2C, BIT_NUM_1, DRAMDQM0_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMDQM1 Slew", PAD_OFF_2C, BIT_NUM_1, DRAMDQM1_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMDQS Slew", PAD_OFF_2C, BIT_NUM_1, DRAMDQS_SLEW_BIT_OFF, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMADR Slew", PAD_OFF_2C, BIT_NUM_1, DRAMADR_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {"DRAMCLK Slew", PAD_OFF_2C, BIT_NUM_1, DRAMCLK_SLEW_BIT_OFF_611, SYSInfo_FUNC_NOTCONVERT},
    {SYSTEM_END, 0, 0, 0, NULL},
};


/**
  Get DRAM SIZE of 96610

  Get DRAM SIZE of 96610
  @param The dram size register value.
  @return The real size value.
*/
static UINT32 SYSInfo_DRAM_SIZE_96610(UINT32 uivalue)
{
    UINT16 SYS_DRAM_SIZE[6] = {32/8, 64/8, 128/8, 256/8, 512/8, 1024/8};

    if (uivalue >= 6)
        return UNDEF_VALUE;
    else
        return SYS_DRAM_SIZE[uivalue];
}

/**
  Get DRAM SIZE of 96611

  Get DRAM SIZE of 96611
  @param The dram size register value.
  @return The real size value.
*/
static UINT32 SYSInfo_DRAM_SIZE_96611(UINT32 uivalue)
{
    UINT16 SYS_DRAM_SIZE[8] = {256/8, 512/8, 1024/8, RESERVED_VALUE, 128/8, 64/8, RESERVED_VALUE, RESERVED_VALUE};

    if (uivalue >= 8)
        return UNDEF_VALUE;
    else
        return SYS_DRAM_SIZE[uivalue];
}

#if 0
#pragma mark -
#endif


/**
  Get FCLK Clock

  Get FCLK Clock
  @param The clock register value of FCLK.
  @return The real clock value.
*/
static UINT32 SYSInfo_FCLK_Clock(UINT32 uivalue)
{
    UINT16 SYS_FCLK_Freq[8] = {120, 80, 60, 48, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE};

    SYS_FCLK_Freq[4] = g_uiPLL2_Freq;
    SYS_FCLK_Freq[5] = g_uiPLL2_Freq/2;
    SYS_FCLK_Freq[6] = g_uiPLL2_Freq/3;
    SYS_FCLK_Freq[7] = g_uiPLL2_Freq/4;
    g_uiFCLK_Freq = SYS_FCLK_Freq[uivalue];

    if (uivalue >= 8)
        return UNDEF_VALUE;
    else
        return SYS_FCLK_Freq[uivalue];
}

/**
  Get HCLK Clock

  Get HCLK Clock
  @param The clock register value of HCLK.
  @return The real clock value.
*/
static UINT32 SYSInfo_HCLK_Clock(UINT32 uivalue)
{
    UINT16 SYS_HCLK_Freq[4] = {UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE};

    SYS_HCLK_Freq[0] = g_uiFCLK_Freq;
    SYS_HCLK_Freq[1] = g_uiFCLK_Freq/2;
    SYS_HCLK_Freq[2] = g_uiFCLK_Freq/3;
    SYS_HCLK_Freq[3] = g_uiFCLK_Freq/4;

    if (uivalue >= 4)
        return UNDEF_VALUE;
    else
        return SYS_HCLK_Freq[uivalue];
}

/**
  Get PCLK Clock

  Get PCLK Clock
  @param The clock register value of PCLK.
  @return The real clock value.
*/
static UINT32 SYSInfo_PCLK_Clock(UINT32 uivalue)
{
    UINT16 SYS_PCLK_Freq[8] = {60, 48, 40, 24, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE};

    SYS_PCLK_Freq[4] = g_uiPLL2_Freq;
    SYS_PCLK_Freq[5] = g_uiPLL2_Freq/2;
    SYS_PCLK_Freq[6] = g_uiPLL2_Freq/3;
    SYS_PCLK_Freq[7] = g_uiPLL2_Freq/4;

    if (uivalue >= 8)
        return UNDEF_VALUE;
    else
        return SYS_PCLK_Freq[uivalue];
}

/**
  Get DMA Clock

  Get DMA Clock
  @param The clock register value of DMA.
  @return The real clock value.
*/
static UINT32 SYSInfo_DMA_Clock(UINT32 uivalue)
{
    UINT16 SYS_DMA_Freq[8] = {120, 80, 60, 48, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE};

    SYS_DMA_Freq[4] = g_uiPLL2_Freq;
    SYS_DMA_Freq[5] = g_uiPLL2_Freq/2;
    SYS_DMA_Freq[6] = g_uiPLL2_Freq/3;
    SYS_DMA_Freq[7] = g_uiPLL2_Freq/4;

    if (uivalue >= 8)
        return UNDEF_VALUE;
    else
        return SYS_DMA_Freq[uivalue];
}

/**
  Get IPE Clock

  Get IPE Clock
  @param The clock register value of IPE.
  @return The real clock value.
*/
static UINT32 SYSInfo_IPE_Clock(UINT32 uivalue)
{
    UINT16 SYS_IPE_Freq[4] = {60, 48, 40, 30};

    if (uivalue >= 4)
        return UNDEF_VALUE;
    else
        return SYS_IPE_Freq[uivalue];
}

/**
  Get IME Clock

  Get IME Clock
  @param The clock register value of IME.
  @return The real clock value.
*/
static UINT32 SYSInfo_IME_Clock(UINT32 uivalue)
{
    UINT16 SYS_IME_Freq[5] = {60, 48, 40, 30, 80};

    if (uivalue >= 5)
        return UNDEF_VALUE;
    else
        return SYS_IME_Freq[uivalue];
}

/**
  Get JPEG Clock

  Get JPEG Clock
  @param The clock register value of JPEG.
  @return The real clock value.
*/
static UINT32 SYSInfo_JPEG_Clock(UINT32 uivalue)
{
    UINT16 SYS_JPEG_Freq[8] = {120, 80, 60, 48, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE};

    SYS_JPEG_Freq[4] = g_uiPLL2_Freq;
    SYS_JPEG_Freq[5] = g_uiPLL2_Freq/2;
    SYS_JPEG_Freq[6] = g_uiPLL2_Freq/3;
    SYS_JPEG_Freq[7] = g_uiPLL2_Freq/4;

    if (uivalue >= 8)
        return UNDEF_VALUE;
    else
        return SYS_JPEG_Freq[uivalue];
}

/**
  Get MPEG Clock

  Get MPEG Clock
  @param The clock register value of MPEG4.
  @return The real clock value.
*/
static UINT32 SYSInfo_MPEG_Clock(UINT32 uivalue)
{
    UINT16 SYS_MPEG_Freq[8] = {RESERVED_VALUE, 80, 60 ,48, RESERVED_VALUE, UNDEF_VALUE, UNDEF_VALUE, UNDEF_VALUE};

    SYS_MPEG_Freq[5] = g_uiPLL2_Freq/2;
    SYS_MPEG_Freq[6] = g_uiPLL2_Freq/3;
    SYS_MPEG_Freq[7] = g_uiPLL2_Freq/4;

    if (uivalue >= 8)
        return UNDEF_VALUE;
    else
        return SYS_MPEG_Freq[uivalue];
}


/**
  Get IDE Clock1 Source Clock

  Get IDE Source Clock
  @param The clock register value of IDE.
  @return The real clock value.
*/
static UINT32 SYSInfo_IDE_ClockSrc(UINT32 uivalue)
{
    UINT16 SYS_IDESrc_Freq[2] = {UNDEF_VALUE, UNDEF_VALUE};

    SYS_IDESrc_Freq[0] = g_uiPLL1_Freq;
    SYS_IDESrc_Freq[1] = g_uiPLL2_Freq;

    g_uiIDESrc_Freq = SYS_IDESrc_Freq[uivalue];

    if (uivalue >= 2)
        return UNDEF_VALUE;
    else
        return SYS_IDESrc_Freq[uivalue];
}

/**
  Get IDE Clock1 Clock

  Get IDE Clock
  @param The clock register value of IDE.
  @return The real clock value.
*/
static UINT32 SYSInfo_IDE_Clock(UINT32 uivalue)
{
    return g_uiIDESrc_Freq/(uivalue + 1)/2;
}

#if 0
#pragma mark -
#endif


/**
  Get DRAM ADDRESS/CTRL Driving/Sink

  Get DRAM ADDRESS/CTRL Driving/Sink
  @param The PAD register value of DRAM ADDRESS/CTRL Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_DRAM_ADR_DRIVE(UINT32 uivalue)
{
    UINT16 SYS_DRAMADR_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    return SYS_DRAMADR_Drive[uivalue];
}

/**
  Get DRAM ADDRESS/CTRL Driving/Sink of 96611

  Get DRAM ADDRESS/CTRL Driving/Sink
  @param The PAD register value of DRAM ADDRESS/CTRL Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_DRAM_ADR_DRIVE_96611(UINT32 uivalue)
{
    //UINT16 SYS_DRAMADR_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    //return SYS_DRAMADR_Drive[uivalue];
    return uivalue;
}

/**
  Get DRAM MEMCLK Driving/Sink

  Get DRAM MEMCLK Driving/Sink
  @param The PAD register value of MEMCLK Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_DRAM_CLK_DRIVE(UINT32 uivalue)
{
    UINT16 SYS_DRAMCLK_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    return SYS_DRAMCLK_Drive[uivalue];
}

/**
  Get DRAM MEMCLK Driving/Sink  of 96611

  Get DRAM MEMCLK Driving/Sink
  @param The PAD register value of MEMCLK Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_DRAM_CLK_DRIVE_96611(UINT32 uivalue)
{
    //UINT16 SYS_DRAMCLK_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    //return SYS_DRAMCLK_Drive[uivalue];
    return uivalue;
}


/**
  Get DRAM DATA Driving/Sink

  Get DRAM DATA Driving/Sink
  @param The PAD register value of DATA Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_DRAM_DATA_DRIVE(UINT32 uivalue)
{
    UINT16 SYS_DRAMDATA_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    return SYS_DRAMDATA_Drive[uivalue];
}

/**
  Get DRAM DATA Driving/Sink of 96611

  Get DRAM DATA Driving/Sink
  @param The PAD register value of DATA Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_DRAM_DATA_DRIVE_96611(UINT32 uivalue)
{
    //UINT16 SYS_DRAMDATA_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    //return SYS_DRAMDATA_Drive[uivalue];
    return uivalue;
}


/**
  Get Card Driving/Sink

  Get Card Driving/Sink
  @param The PAD register value of Card Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_CARD_DRIVE(UINT32 uivalue)
{
    UINT16 SYS_CARD_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    return SYS_CARD_Drive[uivalue];
}

/**
  Get Sensor Driving/Sink

  Get Sensor Driving/Sink
  @param The PAD register value of Sensor Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_SENSOR_DRIVE(UINT32 uivalue)
{
    UINT16 SYS_SENSOR_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    return SYS_SENSOR_Drive[uivalue];
}

/**
  Get LCD Driving/Sink

  Get LCD Driving/Sink
  @param The PAD register value of LCD Driving/Sink.
  @return The real Driving/Sink value.
*/
static UINT32 SYSInfo_LCD_DRIVE(UINT32 uivalue)
{
    UINT16 SYS_LCD_Drive[8] = {2, 4, 6, 8, 10, 12, 14, 16};

    return SYS_LCD_Drive[uivalue];
}

#if 0
#pragma mark -
#endif

/**
  Not Convert and only Return the argument variable.

  Not Convert and only Return the argument variable.
  @param The argument variable.
  @return The same value argument variable.
*/
static UINT32 SYSInfo_FUNC_NOTCONVERT(UINT32 uivalue)
{
    return uivalue;
}


#if 0
#pragma mark -
#endif

/**
  Read Project Name

  Read Project Name
  @param void
  @return 96610 for 96610 project, 96611 for 96611 project.
*/
static UINT32 SYSInfo_Read_Project_Name(void)
{
    UINT32 uiReadReg;

    uiReadReg = inw(CHIP_VERSION);

    if (((uiReadReg >> 24) >=  0x05))
        return 96611;
    else
        return 96610;
}

/**
  Get System PLL frequency

  Get System PLL frequency
  @param uiEX_OSC1 external OSC1 for PLL1 frequency(KHz)
  @param uiEX_OSC2 external OSC2 for PLL2 frequency(KHz)
  @return void
*/
static void SYSInfo_Get_PLL_Value(UINT16 uiEX_OSC1, UINT16 uiEX_OSC2)
{
    UINT32 uiReadReg;
    UINT32 uiNS;
    UINT32 uiMS;

    uiReadReg = inw(CHIP_PLL1);


    uiNS = (uiReadReg >> 8) & 0x3F;
    uiMS = uiReadReg & 0x3F;
    g_uiPLL1_Freq =  (float)uiEX_OSC1*(float)(uiNS)/(float)(uiMS)/1000;

    uiReadReg = inw(CHIP_PLL2);
    uiNS = (uiReadReg >> 8) & 0x3F;
    uiMS = uiReadReg & 0x3F;
    g_uiPLL2_Freq =  (float)uiEX_OSC2*(float)(uiNS)/(float)(uiMS)/1000;

}

#if 0
#pragma mark -
#endif


/**
  Get System Information

  Get System Information
  @param void
  @return void
*/
void SYSInfo_GetSystemInfo(void)
{
    PSYSTEM_INFO    pSystem_Info;
    UINT32          uiReadReg;
    UINT32          uiGetValue;
    UINT32          uiItemValue;

    //#NT#2008/03/20#Chris Hsu -begin
    //#NT#Replace debug_msg with debug_msg
    if (SYSInfo_Read_Project_Name() == 96610)
    {
        debug_msg("*** 96610 ***\n\r");
        pSystem_Info = (PSYSTEM_INFO)&SystemInfo_96610;
        SYSInfo_Get_PLL_Value(EX_OSC1_96610, EX_OSC2_96610);
        debug_msg("PLL1:  %d\n\r", g_uiPLL1_Freq);
        debug_msg("PLL2:  %d\n\r", g_uiPLL2_Freq);
    }
    else
    {
        debug_msg("*** 96611 ***\n\r");
        pSystem_Info = (PSYSTEM_INFO)&SystemInfo_96611;
        SYSInfo_Get_PLL_Value(EX_OSC1_96611, EX_OSC2_96611);
        debug_msg("PLL1:  %d\n\r", g_uiPLL1_Freq);
        debug_msg("PLL2:  %d\n\r", g_uiPLL2_Freq);
    }

    //Module Item
    while ((pSystem_Info->piItemName) != NULL)
    {
        debug_msg(pSystem_Info->piItemName);
        debug_msg(":");
        uiReadReg = inw(pSystem_Info->uiRegAddr);
        uiGetValue = (uiReadReg >> (pSystem_Info->uiRegBitOffset)) & ((1 << pSystem_Info->uiRegBitNum) - 1);
        uiItemValue = pSystem_Info->puiItemFunc(uiGetValue);
        if (uiItemValue == UNDEF_VALUE)
        {
            debug_msg("  Undefined Value\n\r");
        }
        else if (uiItemValue == RESERVED_VALUE)
        {
            debug_msg("  Reserved Value\n\r");
        }
        else
        {
            debug_msg("  %d\n\r", uiItemValue);
        }
        pSystem_Info++;
    }
    //#NT#2008/03/20#Chris Hsu -end
}

#endif

//@}
