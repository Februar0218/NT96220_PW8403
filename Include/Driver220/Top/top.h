/**
    TOP controller header

    Sets the pinmux of each module.

    @file       top.h
    @ingroup    mIHALSysTOP
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _TOP_H
#define _TOP_H

#include "Type.h"

/**
    @addtogroup mIHALSysTOP
*/
//@{

//#NT#2011/06/23#HH Chuang -begin
//#NT#add new top architecture
/**
    Function group

    @note For pinmux_init()
*/
typedef enum
{
    PIN_FUNC_SDIO,      ///< SDIO
    PIN_FUNC_SPI,       ///< SPI
    PIN_FUNC_NAND,      ///< NAND
    PIN_FUNC_SENSOR,    ///< sensor interface
    PIN_FUNC_SCANNER,   ///< scanner interface
    PIN_FUNC_I2C,       ///< I2C
    PIN_FUNC_SIF,       ///< SIF
    PIN_FUNC_UART,      ///< UART
    PIN_FUNC_PWM,       ///< PWM
    PIN_FUNC_LCD,       ///< LCD interface
    PIN_FUNC_LCD2,      ///< LCD2 interface
    PIN_FUNC_TV,        ///< TV interface
    PIN_FUNC_HDMI,      ///< HDMI
    ENUM_DUMMY4WORD(PIN_FUNC)
} PIN_FUNC;

/**
    PIN config for SDIO

    @note For pinmux_init()
*/
typedef enum
{
    PIN_SDIO_CFG_NONE,
    PIN_SDIO_CFG_4BITS = 0x01,      ///< 4 bits wide (MC0~4, MC8)
    PIN_SDIO_CFG_8BITS = 0x02,      ///< 8 bits wide (MC0~4, MC8~12)
    ENUM_DUMMY4WORD(PIN_SDIO_CFG)
} PIN_SDIO_CFG;

/**
    PIN config for SPI

    @note For pinmux_init()
*/
typedef enum
{
    PIN_SPI_CFG_NONE,
    PIN_SPI_CFG_NORMAL = 0x01,      ///< 1 bit full duplex (MC0~2, MC14)
    PIN_SPI_CFG_2BITS = 0x01,       ///< support 2 bits half duplex (MC0~2, MC14)
    PIN_SPI_CFG_4BITS = 0x02,       ///< support 4 bits half duplex (MC0~4, MC14)
    ENUM_DUMMY4WORD(PIN_SPI_CFG)
} PIN_SPI_CFG;

/**
    PIN config for NAND

    @note For pinmux_init()
*/
typedef enum
{
    PIN_NAND_CFG_NONE,
    PIN_NAND_CFG_1CS = 0x01,        ///< 1 chip select (MC0~7, MC9~14)
    ENUM_DUMMY4WORD(PIN_NAND_CFG)
} PIN_NAND_CFG;

/**
    PIN config for Sensor

    @note For pinmux_init()
*/
typedef enum
{
    PIN_SENSOR_CFG_NONE,
    PIN_SENSOR_CFG_DATASWAP = 0x01,     ///< swap data bit order
    PIN_SENSOR_CFG_4BITS = 0x02,        ///< 4 bits sensor (SN_PXCLK, SN_VD, SN_HD, SN_D6~9)
    PIN_SENSOR_CFG_8BITS = 0x04,        ///< 8 bits sensor (SN_PXCLK, SN_VD, SN_HD, SN_D2~9)
    PIN_SENSOR_CFG_9BITS = 0x08,        ///< 9 bits sensor (SN_PXCLK, SN_VD, SN_HD, SN_D1~9)
    PIN_SENSOR_CFG_10BITS = 0x10,       ///< 10 bits sensor (SN_PXCLK, SN_VD, SN_HD, SN_D0~9)
    PIN_SENSOR_CFG_SHUTTER = 0x20,      ///< Enable shutter (SN_SHUTTER)
    PIN_SENSOR_CFG_MCLK = 0x40,         ///< Enable MCLK (SN_MCLK)
    PIN_SENSOR_CFG_MES0 = 0x80,         ///< Enable MES0 (SN_MES0)
    PIN_SENSOR_CFG_MES1 = 0x100,        ///< Enable MES1 (SN_MES1)
    PIN_SENSOR_CFG_FLCTR = 0x200,       ///< Enable FLCTR (SN_FLCTR)
    PIN_SENSOR_CFG_STROBE = 0x400,      ///< Enable STROBE (DGPIO1)
    ENUM_DUMMY4WORD(PIN_SENSOR_CFG)
} PIN_SENSOR_CFG;

/**
    PIN config for Scanner

    @note For pinmux_init()
*/
typedef enum
{
    PIN_SCANNER_CFG_NONE,
    PIN_SCANNER_CFG_DATASWAP = 0x01,    ///< swap data bit order
    PIN_SCANNER_CFG_4BITS = 0x02,       ///< 4 bits sensor (CISCLK=SN_MES0, ADCCLK=SN_MES1, CISFS=SN_FLCTR,
                                        ///<              LEDR=PWM0, LEDG=PWM1, LEDB=PWM2, CISDP=DGPIO4,
                                        ///<              SCAN_D4~7=SN_D6~9)
    PIN_SCANNER_CFG_8BITS = 0x04,       ///< 8 bits sensor (CISCLK=SN_MES0, ADCCLK=SN_MES1, CISFS=SN_FLCTR,
                                        ///<              LEDR=PWM0, LEDG=PWM1, LEDB=PWM2, CISDP=DGPIO4,
                                        ///<              SCAN_D0~7=SN_D2~9)
    PIN_SCANNER_CFG_CDSCLK1 = 0x08,     ///< Enable CDSCLK1 (CDSCLK1=SN_PXCLK)
    PIN_SCANNER_CFG_CDSCLK2 = 0x10,     ///< Enable CDSCLK2 (CDSCLK2=SN_MCLK)
    PIN_SCANNER_CFG_SCANTRIG = 0x20,    ///< Enable Scanner Trigger (Scanner Trigger=DGPIO2)
    PIN_SCANNER_CFG_ACYC = 0x40,        ///< Enable ACYC (ACYC=DGPIO3)
    //#NT#2011/11/07#HH Chuang -begin
    //#NT#Support MCLK in scanner application
    PIN_SCANNER_CFG_MCLK = 0x80,        ///< Enable MCLK (SN_MCLK) (Conflict with PIN_SCANNER_CFG_CDSCLK2)
    //#NT#2011/11/07#HH Chuang -end

    ENUM_DUMMY4WORD(PIN_SCANNER_CFG)
} PIN_SCANNER_CFG;

/**
    PIN config for I2C

    @note For pinmux_init()
*/
typedef enum
{
    PIN_I2C_CFG_NONE,
    PIN_I2C_CFG_1CH = 0x01,             ///< Enable 1st I2C channel (I2CSDA, I2CSCL)
    ENUM_DUMMY4WORD(PIN_I2C_CFG)
} PIN_I2C_CFG;

/**
    PIN config for SIF

    *note Conflict with PIN_LCD_CFG = PIN_LCD_CFG_MI_PARALLEL. And for pinmux_init().
*/
typedef enum
{
    PIN_SIF_CFG_NONE,
    PIN_SIF_CFG_1CH = 0x01,             ///< Enable 1st SIF channel (SBCK0, SBDAT0, SBCS0)
    PIN_SIF_CFG_2CH = 0x02,             ///< Enable 2nd SIF channel (SBCK0, SBDAT0, SBCS0, SBCS1)
    ENUM_DUMMY4WORD(PIN_SIF_CFG)
} PIN_SIF_CFG;

/**
    PIN config for UART

    @note For pinmux_init()
*/
typedef enum
{
    PIN_UART_CFG_NONE,
    PIN_UART_CFG_1CH = 0x01,            ///< Enable 1st UART channel (UART1_TX, UART1_RX)
    PIN_UART_CFG_2CH = 0x02,            ///< Enable 2nd UART channel (UART2_TX=DGPIO0, UART2_RX=DGPIO1)
    PIN_UART_CFG_2CH_CTSRTS = 0x04,     ///< Enable CTS/RTS on 2nd UART channel (CTS=SN_VD, RTS=SN_HD)
    PIN_UART_CFG_2CH_CTSRTS_PWM = 0x08, ///< Switch UART2 CTS/RTS to PWM (CTS=PWM0, RTS=PWM1)
    //#NT#2012/06/04#HH Chuang -begin
    //#NT#Support configuration of UART2 TX
    PIN_UART_CFG_2CH_NO_TX = 0x10,      ///< Disable UART2 TX Pinmux
    //#NT#2012/06/04#HH Chuang -end
    ENUM_DUMMY4WORD(PIN_UART_CFG)
} PIN_UART_CFG;

/**
    PIN config for PWM

    @note For pinmux_init()
*/
typedef enum
{
    PIN_PWM_CFG_NONE,
    PIN_PWM_CFG_PWM0 = 0x01,            ///< Enable PWM0
    PIN_PWM_CFG_PWM1 = 0x02,            ///< Enable PWM1
    PIN_PWM_CFG_PWM2 = 0x04,            ///< Enable PWM2
    PIN_PWM_CFG_CCNT = 0x08,            ///< Enable CCNT (Cycle count) (PWMCCNT=DGPIO0)
    PIN_PWM_CFG_PWM1_ON_DGPIO0 = 0x10,  ///< Config PWM1 to DGPIO0 (PWM1=DGPIO0)
    ENUM_DUMMY4WORD(PIN_PWM_CFG)
} PIN_PWM_CFG;

/**
    PIN config of LCD modes

    @note For pinmux_init()
*/
typedef enum
{
    PINMUX_LCDMODE_RGB_SERIAL           =    0,     ///< LCD MODE is RGB Serial or UPS051
    PINMUX_LCDMODE_RGB_PARALL           =    1,     ///< LCD MODE is RGB Parallel
    PINMUX_LCDMODE_YUV640               =    2,     ///< LCD MODE is YUV640
    PINMUX_LCDMODE_YUV720               =    3,     ///< LCD MODE is YUV720
    PINMUX_LCDMODE_RGBD360              =    4,     ///< LCD MODE is RGB Dummy 360
    PINMUX_LCDMODE_RGBD320              =    5,     ///< LCD MODE is RGB Dummy 320
    PINMUX_LCDMODE_RGB_THROUGH          =    6,     ///< LCD MODE is RGB through mode
    PINMUX_LCDMODE_CCIR601              =    7,     ///< LCD MODE is CCIR601
    PINMUX_LCDMODE_CCIR656              =    8,     ///< LCD MODE is CCIR656

    PINMUX_LCDMODE_MI_OFFSET            = 32,       ///< Memory LCD MODE offset
    PINMUX_LCDMODE_MI_FMT0              = 32+0,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT1              = 32+1,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT2              = 32+2,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT3              = 32+3,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT4              = 32+4,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT5              = 32+5,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT6              = 32+6,     ///< LCD MODE is Memory(Parallel Interface) 8bits
    PINMUX_LCDMODE_MI_FMT7              = 32+7,     ///< LCD MODE is Memory(Parallel Interface) 9bits
    PINMUX_LCDMODE_MI_FMT8              = 32+8,     ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT9              = 32+9,     ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT10             = 32+10,    ///< LCD MODE is Memory(Parallel Interface) 18bits
    PINMUX_LCDMODE_MI_FMT11             = 32+11,    ///< LCD MODE is Memory(Parallel Interface)  8bits
    PINMUX_LCDMODE_MI_FMT12             = 32+12,    ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT13             = 32+13,    ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_FMT14             = 32+14,    ///< LCD MODE is Memory(Parallel Interface) 16bits
    PINMUX_LCDMODE_MI_SERIAL_BI         = 32+20,    ///< LCD MODE is Serial Interface bi-direction
    PINMUX_LCDMODE_MI_SERIAL_SEP        = 32+21,    ///< LCD MODE is Serial Interface separation


    PINMUX_HDMIMODE_OFFSET              = 64,       ///< HDMI MODE offset
    PINMUX_HDMIMODE_640X480P60          = 64+1,     ///< HDMI Video format is 640x480 & Progressive 60fps
    PINMUX_HDMIMODE_720X480P60          = 64+2,     ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
    PINMUX_HDMIMODE_720X480P60_16X9     = 64+3,     ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_1280X720P60         = 64+4,     ///< HDMI Video format is 1280x720 & Progressive 60fps
    PINMUX_HDMIMODE_1920X1080I60        = 64+5,     ///< HDMI Video format is 1920x1080 & Interlaced 60fps
    PINMUX_HDMIMODE_720X480I60          = 64+6,     ///< HDMI Video format is 720x480 & Interlaced 60fps
    PINMUX_HDMIMODE_720X480I60_16X9     = 64+7,     ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
    PINMUX_HDMIMODE_720X240P60          = 64+8,     ///< HDMI Video format is 720x240 & Progressive 60fps
    PINMUX_HDMIMODE_720X240P60_16X9     = 64+9,     ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_1440X480I60         = 64+10,    ///< HDMI Video format is 1440x480 & Interlaced 60fps
    PINMUX_HDMIMODE_1440X480I60_16X9    = 64+11,    ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
    PINMUX_HDMIMODE_1440X240P60         = 64+12,    ///< HDMI Video format is 1440x240 & Progressive 60fps
    PINMUX_HDMIMODE_1440X240P60_16X9    = 64+13,    ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_1440X480P60         = 64+14,    ///< HDMI Video format is 1440x480 & Progressive 60fps
    PINMUX_HDMIMODE_1440X480P60_16X9    = 64+15,    ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
    PINMUX_HDMIMODE_720X576P50          = 64+17,    ///< HDMI Video format is 720x576 & Progressive 50fps
    PINMUX_HDMIMODE_720X576P50_16X9     = 64+18,    ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1280X720P50         = 64+19,    ///< HDMI Video format is 1280x720 & Progressive 50fps
    PINMUX_HDMIMODE_1920X1080I50        = 64+20,    ///< HDMI Video format is 1920x1080 & Interlaced 50fps
    PINMUX_HDMIMODE_720X576I50          = 64+21,    ///< HDMI Video format is 720x576 & Interlaced 50fps
    PINMUX_HDMIMODE_720X576I50_16X9     = 64+22,    ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
    PINMUX_HDMIMODE_720X288P50          = 64+23,    ///< HDMI Video format is 720x288 & Progressive 50fps
    PINMUX_HDMIMODE_720X288P50_16X9     = 64+24,    ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1440X576I50         = 64+25,    ///< HDMI Video format is 1440x576 & Interlaced 50fps
    PINMUX_HDMIMODE_1440X576I50_16X9    = 64+26,    ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
    PINMUX_HDMIMODE_1440X288P50         = 64+27,    ///< HDMI Video format is 1440x288 & Progressive 50fps
    PINMUX_HDMIMODE_1440X288P50_16X9    = 64+28,    ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1440X576P50         = 64+29,    ///< HDMI Video format is 1440x576 & Progressive 50fps
    PINMUX_HDMIMODE_1440X576P50_16X9    = 64+30,    ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
    PINMUX_HDMIMODE_1920X1080P50        = 64+31,    ///< HDMI Video format is 1920x1080 & Progressive 50fps
    PINMUX_HDMIMODE_1920X1080P24        = 64+32,    ///< HDMI Video format is 1920x1080 & Progressive 24fps
    PINMUX_HDMIMODE_1920X1080P25        = 64+33,    ///< HDMI Video format is 1920x1080 & Progressive 25fps
    PINMUX_HDMIMODE_1920X1080P30        = 64+34,    ///< HDMI Video format is 1920x1080 & Progressive 30fps
    PINMUX_HDMIMODE_1920X1080I50_VT1250 = 64+39,    ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
    PINMUX_HDMIMODE_1920X1080I100       = 64+40,    ///< HDMI Video format is 1920x1080 & Interlaced  100fps
    PINMUX_HDMIMODE_1280X720P100        = 64+41,    ///< HDMI Video format is 1280X720  & Progressive 100fps
    PINMUX_HDMIMODE_720X576P100         = 64+42,    ///< HDMI Video format is 720X576   & Progressive 100fps
    PINMUX_HDMIMODE_720X576P100_16X9    = 64+43,    ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
    PINMUX_HDMIMODE_720X576I100         = 64+44,    ///< HDMI Video format is 720X576  & Interlaced 100fps
    PINMUX_HDMIMODE_720X576I100_16X9    = 64+45,    ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
    PINMUX_HDMIMODE_1920X1080I120       = 64+46,    ///< HDMI Video format is 1920X1080 & Interlaced 120fps
    PINMUX_HDMIMODE_1280X720P120        = 64+47,    ///< HDMI Video format is 1280X720  & Progressive 120fps
    PINMUX_HDMIMODE_720X480P120         = 64+48,    ///< HDMI Video format is 720X480   & Progressive 120fps
    PINMUX_HDMIMODE_720X480P120_16X9    = 64+49,    ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
    PINMUX_HDMIMODE_720X480I120         = 64+50,    ///< HDMI Video format is 720X480  & Interlaced 120fps
    PINMUX_HDMIMODE_720X480I120_16X9    = 64+51,    ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
    PINMUX_HDMIMODE_720X576P200         = 64+52,    ///< HDMI Video format is 720X576  & Progressive 200fps
    PINMUX_HDMIMODE_720X576P200_16X9    = 64+53,    ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
    PINMUX_HDMIMODE_720X576I200         = 64+54,    ///< HDMI Video format is 720X576  & Interlaced 200fps
    PINMUX_HDMIMODE_720X576I200_16X9    = 64+55,    ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
    PINMUX_HDMIMODE_720X480P240         = 64+56,    ///< HDMI Video format is 720X480  & Progressive 240fps
    PINMUX_HDMIMODE_720X480P240_16X9    = 64+57,    ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
    PINMUX_HDMIMODE_720X480I240         = 64+58,    ///< HDMI Video format is 720X480  & Interlaced 240fps
    PINMUX_HDMIMODE_720X480I240_16X9    = 64+59,    ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9

    ENUM_DUMMY4WORD(PINMUX_LCDINIT)
} PINMUX_LCDINIT;

/**
    PIN config for Parallel MI

    @note For pinmux_init()
*/
typedef enum
{
    PINMUX_PMI_CFG_NORMAL = 0x00,                   ///< Normal Parallel MI location (at LCD and SIF)
    PINMUX_PMI_CFG_SIF = 0x00,                      ///< Normal Parallel MI location (at LCD and SIF)
    PINMUX_PMI_CFG_DGPIO = 0x01<<26,                ///< Normal Parallel MI location (at LCD and DGPIO)
    //#NT#2011/10/06#HH Chuang -begin
    //#NT#Support no RD/D8/TE for PIN shortage project
    PINMUX_PMI_CFG_NO_RD_D8_TE = 0x02<<26,          ///< Normal Parallel MI location (at LCD)
    //#NT#2011/10/06#HH Chuang -end

    PINMUX_PMI_CFG_MASK = 0x03<<26,
    ENUM_DUMMY4WORD(PINMUX_PMI_CFG)
} PINMUX_PMI_CFG;

/**
    PIN config for TV/HDMI

    @note For pinmux_init()
*/
typedef enum
{
    PINMUX_TV_HDMI_CFG_GPIO = 0x00,                 ///< TV activation will disable PINMUX to GPIO
    PINMUX_TV_HDMI_CFG_NORMAL = 0x00,               ///< TV activation will disable PANEL which shared the same IDE
    PINMUX_TV_HDMI_CFG_PINMUX_ON = 0x01<<28,        ///< TV activation will keep PINMUX setting

    PINMUX_TV_HDMI_CFG_MASK = 0x03<<28,
    ENUM_DUMMY4WORD(PINMUX_TV_HDMI_CFG)
} PINMUX_TV_HDMI_CFG;

/**
    PIN config for HDMI

    @note For pinmux_init()
*/
typedef enum
{
    PINMUX_HDMI_CFG_GPIO = 0x00,                    ///< HDMI specific PIN to GPIO
    PINMUX_HDMI_CFG_DDC = 0x01<<26,                 ///< HDMI DDC
    PINMUX_HDMI_CFG_HOTPLUG = 0x02<<26,             ///< HDMI HOTPLUG

    PINMUX_HDMI_CFG_MASK = 0x03<<26,
    ENUM_DUMMY4WORD(PINMUX_HDMI_CFG)
} PINMUX_HDMI_CFG;

/**
    PIN location of LCD

    @note For pinmux_init()
*/
typedef enum
{
    PINMUX_DISPMUX_SEL_NONE = 0x00<<28,             ///< PINMUX none
    PINMUX_DISPMUX_SEL_LCD = 0x01<<28,              ///< PINMUX at LCD interface
    PINMUX_DISPMUX_SEL_STRG = 0x02<<28,             ///< PINMUX at storage interface

    PINMUX_DISPMUX_SEL_MASK = 0x03<<28,
    ENUM_DUMMY4WORD(PINMUX_DISPMUX_SEL)
} PINMUX_DISPMUX_SEL;

/**
    Pinmux Function identifier

    @note For pinmux_getDispMode(), pinmux_setPinmux()
*/
typedef enum
{
    PINMUX_FUNC_ID_LCD,                             ///< 1st Panel (LCD), pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO
                                                    ///< - @b PINMUX_LCD_SEL_CCIR656
                                                    ///< - @b PINMUX_LCD_SEL_CCIR601
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_6BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_9BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDIO
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO
                                                    ///< ORed with
                                                    ///< - @b PINMUX_LCD_SEL_DE_ENABLE
                                                    ///< - @b PINMUX_LCD_SEL_TE_ENABLE
    PINMUX_FUNC_ID_LCD2,                            ///< 2nd Panel (LCD), pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO
                                                    ///< - @b PINMUX_LCD_SEL_CCIR656
                                                    ///< - @b PINMUX_LCD_SEL_CCIR601
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_6BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_RGB_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_8BITS
                                                    ///< - @b PINMUX_LCD_SEL_PARALLE_MI_9BITS
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDIO
                                                    ///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO
                                                    ///< ORed with
                                                    ///< - @b PINMUX_LCD_SEL_DE_ENABLE
                                                    ///< - @b PINMUX_LCD_SEL_TE_ENABLE
    PINMUX_FUNC_ID_TV,                              ///< TV, pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO
    PINMUX_FUNC_ID_HDMI,                            ///< HDMI, pinmux can be:
                                                    ///< - @b PINMUX_LCD_SEL_GPIO

    PINMUX_FUNC_ID_SDIO,                            ///< SDIO, pinmux can be:
                                                    ///< - @b PINMUX_STORAGE_SEL_INACTIVE
                                                    ///< - @b PINMUX_STORAGE_SEL_ACTIVE
    PINMUX_FUNC_ID_SPI,                             ///< SPI, pinmux can be:
                                                    ///< - @b PINMUX_STORAGE_SEL_INACTIVE
                                                    ///< - @b PINMUX_STORAGE_SEL_ACTIVE
    PINMUX_FUNC_ID_NAND,                            ///< NAND, pinmux can be:
                                                    ///< - @b PINMUX_STORAGE_SEL_INACTIVE
                                                    ///< - @b PINMUX_STORAGE_SEL_ACTIVE

    PINMUX_FUNC_ID_SN_MES0,                         ///< Sensor MES0, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE
    PINMUX_FUNC_ID_SN_MES1,                         ///< Sensor MES1, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE
    PINMUX_FUNC_ID_SN_FLCTR,                        ///< Sensor Flash Control, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE
    PINMUX_FUNC_ID_SN_MCLK,                         ///< Sensor MCLK, pinmux can be:
                                                    ///< - @b PINMUX_SENSOR_SEL_INACTIVE
                                                    ///< - @b PINMUX_SENSOR_SEL_ACTIVE
    PINMUX_FUNC_ID_SCAN_LEDR,                       ///< Scanner LEDR, pinmux can be:
                                                    ///< - @b PINMUX_SCANNER_SEL_INACTIVE
                                                    ///< - @b PINMUX_SCANNER_SEL_ACTIVE
    PINMUX_FUNC_ID_SCAN_LEDG,                       ///< Scanner LEDG, pinmux can be:
                                                    ///< - @b PINMUX_SCANNER_SEL_INACTIVE
                                                    ///< - @b PINMUX_SCANNER_SEL_ACTIVE
    PINMUX_FUNC_ID_SCAN_LEDB,                       ///< Scanner LEDB, pinmux can be:
                                                    ///< - @b PINMUX_SCANNER_SEL_INACTIVE
                                                    ///< - @b PINMUX_SCANNER_SEL_ACTIVE

    PINMUX_FUNC_ID_COUNT,                           //< Total function count
    ENUM_DUMMY4WORD(PINMUX_FUNC_ID)
} PINMUX_FUNC_ID;

/**
    Pinmux selection for LCD

    @note For PINMUX_FUNC_ID_LCD, PINMUX_FUNC_ID_LCD2
*/
typedef enum
{
    PINMUX_LCD_SEL_GPIO,                            ///< GPIO
    PINMUX_LCD_SEL_CCIR656,                         ///< CCIR-656
    PINMUX_LCD_SEL_CCIR601,                         ///< CCIR-601
    PINMUX_LCD_SEL_SERIAL_RGB_6BITS,                ///< Serial RGB 6 bits
    PINMUX_LCD_SEL_SERIAL_RGB_8BITS,                ///< Serial RGB 8 bits
    PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS,              ///< Serial YCbCr 8 bits
    PINMUX_LCD_SEL_PARALLE_MI_8BITS,                ///< Parallel MI 8 bits
    PINMUX_LCD_SEL_PARALLE_MI_9BITS,                ///< Parallel MI 9 bits
    PINMUX_LCD_SEL_SERIAL_MI_SDIO,                  ///< Serial MI SDIO bi-direction
    PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO,               ///< Serial MI SDI/SDO seperate

    PINMUX_LCD_SEL_DE_ENABLE = 0x01<<24,            ///< DE Enable (For non-MI)
    PINMUX_LCD_SEL_TE_ENABLE = 0x01<<25,            ///< TE Enable (For Parallel MI)

    ENUM_DUMMY4WORD(PINMUX_LCD_SEL)
} PINMUX_LCD_SEL;

/**
    Pinmux selection for Storage

    @note For PINMUX_FUNC_ID_SDIO, PINMUX_FUNC_ID_SPI, PINMUX_FUNC_ID_NAND
*/
typedef enum
{
    PINMUX_STORAGE_SEL_INACTIVE,                    ///< Inactive storage
    PINMUX_STORAGE_SEL_ACTIVE,                      ///< Active storage
    PINMUX_STORAGE_SEL_INEXIST,                     ///< Inexist storage
    PINMUX_STORAGE_SEL_EXIST,                       ///< Exist storage
    ENUM_DUMMY4WORD(PINMUX_STORAGE_SEL)
} PINMUX_STORAGE_SEL;

/**
    Pinmux selection for sensor

    @note For PINMUX_FUNC_ID_SN_MES0, PINMUX_FUNC_ID_SN_MES1, PINMUX_FUNC_ID_SN_FLCTR
*/
typedef enum
{
    PINMUX_SENSOR_SEL_INACTIVE,                     ///< Inactive
    PINMUX_SENSOR_SEL_ACTIVE,                       ///< Active
    ENUM_DUMMY4WORD(PINMUX_SENSOR_SEL)
} PINMUX_SENSOR_SEL;

/**
    Pinmux selection for scanner

    @note For PINMUX_FUNC_ID_SCAN_LEDR, PINMUX_FUNC_ID_SCAN_LEDG, PINMUX_FUNC_ID_SCAN_LEDB
*/
typedef enum
{
    PINMUX_SCANNER_SEL_INACTIVE,                    ///< Inactive
    PINMUX_SCANNER_SEL_ACTIVE,                      ///< Active
    ENUM_DUMMY4WORD(PINMUX_SCANNER_SEL)
} PINMUX_SCANNER_SEL;

typedef struct
{
    PIN_FUNC    pinFunction;            ///< PIN Function group
    UINT32      config;                 ///< Configuration for pinFunction
} PIN_GROUP_CONFIG;
//#NT#2011/06/23#HH Chuang -end

/**
   @name debug port enable/disable

   debug port enable/disable

   debug port enable/disable for pinmux_select_debugport ().
*/
#define PINMUX_DEBUGPORTEN_SHIFT    27
#define PINMUX_DEBUGPORT_DISABLE    (0x0000 << PINMUX_DEBUGPORTEN_SHIFT)    ///< debug port disable
#define PINMUX_DEBUGPORT_ENABLE     (0x0001 << PINMUX_DEBUGPORTEN_SHIFT)    ///< debug port enable

/**
    @name Debug port select ID

    Debug port select ID

    Debug port select value for pinmux_select_debugport ().
*/
//@{
#define PINMUX_DEBUGPORT_USB        0x0000      ///< USB
#define PINMUX_DEBUGPORT_AHBC0      0x0001      ///< AHB C0
#define PINMUX_DEBUGPORT_SIE        0x0002      ///< SIE
#define PINMUX_DEBUGPORT_IPE        0x0003      ///< IPE
#define PINMUX_DEBUGPORT_IME        0x0004      ///< IME
#define PINMUX_DEBUGPORT_IDE        0x0005      ///< IDE
#define PINMUX_DEBUGPORT_HDMI       0x0006      ///< HDMI
#define PINMUX_DEBUGPORT_DDR        0x0007      ///< DDR
#define PINMUX_DEBUGPORT_JPEG       0x0008      ///< JPEG
#define PINMUX_DEBUGPORT_AMBARD     0x0009      ///< AMBA RD
#define PINMUX_DEBUGPORT_AMBAWR     0x000A      ///< AMBA WR
#define PINMUX_DEBUGPORT_GRAPHIC    0x000B      ///< GRAPHIC
#define PINMUX_DEBUGPORT_ADM        0x000C      ///< ADC
#define PINMUX_DEBUGPORT_CKG        0x000D      ///< CKGen
#define PINMUX_DEBUGPORT_SDC0       0x000E      ///< SDC0
#define PINMUX_DEBUGPORT_SDC1       0x000F      ///< SDC1
#define PINMUX_DEBUGPORT_AUDIO      0x0010      ///< AUDIO
#define PINMUX_DEBUGPORT_APBBRG     0x0011      ///< APB BRG
#define PINMUX_DEBUGPORT_AHB        0x0012      ///< AHB
#define PINMUX_DEBUGPORT_TMR        0x0013      ///< Timer
#define PINMUX_DEBUGPORT_WDT        0x0014      ///< Watchdog
#define PINMUX_DEBUGPORT_NAND       0x0015      ///< NAND
#define PINMUX_DEBUGPORT_I2C        0x0016      ///< I2C
#define PINMUX_DEBUGPORT_DAI        0x0017      ///< DAI
#define PINMUX_DEBUGPORT_SCANTG     0x0018      ///< Scanner TG
#define PINMUX_DEBUGPORT_IFE        0x0019      ///< IFE
#define PINMUX_DEBUGPORT_DDRIO0     0x001A      ///< DDRIO 0
#define PINMUX_DEBUGPORT_DDRIO1     0x001B      ///< DDRIO 1
//@}

extern void     pinmux_select_debugport(UINT uiDebug);
extern void     pinmux_enable_debugport(void);
extern void     pinmux_disable_debugport(void);

//#NT#2011/06/23#HH Chuang -begin
//#NT#add new top architecture
extern ER       pinmux_init(PIN_GROUP_CONFIG *pConfig);
extern ER       pinmux_setDispMode(PINMUX_FUNC_ID id, PINMUX_LCDINIT dispMode);
extern PINMUX_LCDINIT pinmux_getDispMode(PINMUX_FUNC_ID id);
extern ER       pinmux_setPinmux(PINMUX_FUNC_ID id, UINT32 pinmux);
extern BOOL     pinmux_chkPinmux(PINMUX_FUNC_ID id, UINT32 pinmux);
//#NT#2011/06/23#HH Chuang -end
//@}    //addtogroup mIHALSysCfg
//@}
#endif

