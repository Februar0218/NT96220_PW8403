/**
    PAD controller header

    PAD controller header

    @file       pad.h
    @ingroup    mIHALSysPAD
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _PAD_H
#define _PAD_H

#include "Type.h"

/** \addtogroup mIHALSysPAD */
//@{

/**
    Pad type select

    Pad type select

    Pad type value for pad_setPullUpDown(), pad_getPullUpDown().
*/
typedef enum
{
    PAD_NONE         =          0x00,      ///< none of pull up/down
    PAD_PULLDOWN     =          0x01,      ///< 75K pull down
    PAD_PULLUP       =          0x02,      ///< 75K pull up
    PAD_KEEPER       =          0x03,      ///< 75K keeper

    ENUM_DUMMY4WORD(PAD_PULL)
} PAD_PULL;

/**
    Pad slew rate select

    Pad slew rate select

    Pad slew rate value for pad_setSlewRate(), pad_getSlewRate().
*/
typedef enum
{
    PAD_SLEWRATE_FAST   =       0x00,      ///< Pad slew rate fast
    PAD_SLEWRATE_SLOW   =       0x01,      ///< Pad slew rate slow

    ENUM_DUMMY4WORD(PAD_SLEWRATE)
} PAD_SLEWRATE;

/**
    Pad driving select

    Pad driving select

    Pad driving value for pad_setDrivingSink(), pad_getDrivingSink().
*/
typedef enum
{
    PAD_DRIVINGSINK_2P5MA  =    0x01,       ///< Pad driver/sink 2.5mA
    PAD_DRIVINGSINK_5MA  =      0x02,       ///< Pad driver/sink 5mA
    PAD_DRIVINGSINK_7P5MA  =    0x04,       ///< Pad driver/sink 7.5mA
    PAD_DRIVINGSINK_10MA  =     0x08,       ///< Pad driver/sink 10mA
    PAD_DRIVINGSINK_15MA =      0x10,       ///< Pad driver/sink 15mA
    PAD_DRIVINGSINK_20MA =      0x20,       ///< Pad driver/sink 20mA

    ENUM_DUMMY4WORD(PAD_DRIVINGSINK)
} PAD_DRIVINGSINK;

//
// Backward compatible definition
//
#define PAD_DRIVINGSINK_2MA     PAD_DRIVINGSINK_2P5MA  ///< Pad driver/sink 2mA
#define PAD_DRIVINGSINK_4MA     PAD_DRIVINGSINK_5MA    ///< Pad driver/sink 4mA
#define PAD_DRIVINGSINK_6MA     PAD_DRIVINGSINK_7P5MA  ///< Pad driver/sink 6mA
#define PAD_DRIVINGSINK_8MA     PAD_DRIVINGSINK_10MA   ///< Pad driver/sink 8mA
#define PAD_DRIVINGSINK_12MA    PAD_DRIVINGSINK_15MA   ///< Pad driver/sink 12mA
#define PAD_DRIVINGSINK_16MA    PAD_DRIVINGSINK_20MA   ///< Pad driver/sink 16mA

/**
    @name Pad type pin ID.

    Pad type pin ID.

    Pad ID of pad_setPullUpDown(), pad_getPullUpDown().
*/
#define PAD_PIN_NOT_EXIST       (15)                        // For backward compatible
//@{
//MCx group begin
#define PAD_PIN_MC_BASE         0
#define PAD_PIN_MC0             (PAD_PIN_MC_BASE + 0)         ///< MC0
#define PAD_PIN_MC1             (PAD_PIN_MC_BASE + 1)         ///< MC1
#define PAD_PIN_MC2             (PAD_PIN_MC_BASE + 2)         ///< MC2
#define PAD_PIN_MC3             (PAD_PIN_MC_BASE + 3)         ///< MC3
#define PAD_PIN_MC4             (PAD_PIN_MC_BASE + 4)         ///< MC4
#define PAD_PIN_MC5             (PAD_PIN_MC_BASE + 5)         ///< MC5
#define PAD_PIN_MC6             (PAD_PIN_MC_BASE + 6)         ///< MC6
#define PAD_PIN_MC7             (PAD_PIN_MC_BASE + 7)         ///< MC7
#define PAD_PIN_MC8             (PAD_PIN_MC_BASE + 8)         ///< MC8
#define PAD_PIN_MC9             (PAD_PIN_MC_BASE + 9)         ///< MC9
#define PAD_PIN_MC10            (PAD_PIN_MC_BASE + 10)        ///< MC10
#define PAD_PIN_MC11            (PAD_PIN_MC_BASE + 11)        ///< MC11
#define PAD_PIN_MC12            (PAD_PIN_MC_BASE + 12)        ///< MC12
#define PAD_PIN_MC13            (PAD_PIN_MC_BASE + 13)        ///< MC13
#define PAD_PIN_MC14            (PAD_PIN_MC_BASE + 14)        ///< MC14
//Backward Compatible
#define PAD_PIN_MC15            (PAD_PIN_MC_BASE + 15)        // Not Exist in 96220
#define PAD_PIN_MC16            (PAD_PIN_MC_BASE + 16)        // Not Exist in 96220
#define PAD_PIN_MC17            (PAD_PIN_MC_BASE + 17)        // Not Exist in 96220
#define PAD_PIN_MC18            (PAD_PIN_MC_BASE + 18)        // Not Exist in 96220
#define PAD_PIN_MC19            (PAD_PIN_MC_BASE + 19)        // Not Exist in 96220
#define PAD_PIN_MC20            (PAD_PIN_MC_BASE + 20)        // Not Exist in 96220
#define PAD_PIN_MC21            (PAD_PIN_MC_BASE + 21)        // Not Exist in 96220
#define PAD_PIN_MC22            (PAD_PIN_MC_BASE + 22)        // Not Exist in 96220
#define PAD_PIN_MC23            (PAD_PIN_MC_BASE + 23)        // Not Exist in 96220
#define PAD_PIN_MC24            (PAD_PIN_MC_BASE + 24)        // Not Exist in 96220
#define PAD_PIN_MC25            (PAD_PIN_MC_BASE + 25)        // Not Exist in 96220
#define PAD_PIN_MC26            (PAD_PIN_MC_BASE + 26)        // Not Exist in 96220
#define PAD_PIN_MC27            (PAD_PIN_MC_BASE + 27)        // Not Exist in 96220
#define PAD_PIN_MC28            (PAD_PIN_MC_BASE + 28)        // Not Exist in 96220
#define PAD_PIN_MC29            (PAD_PIN_MC_BASE + 29)        // Not Exist in 96220
#define PAD_PIN_MC30            (PAD_PIN_MC_BASE + 30)        // Not Exist in 96220
#define PAD_PIN_MC31            (PAD_PIN_MC_BASE + 31)        // Not Exist in 96220
#define PAD_PIN_MC32            (PAD_PIN_MC_BASE + 32)        // Not Exist in 96220
#define PAD_PIN_MC33            (PAD_PIN_MC_BASE + 33)        // Not Exist in 96220
#define PAD_PIN_MC34            (PAD_PIN_MC_BASE + 34)        // Not Exist in 96220
#define PAD_PIN_MC35            (PAD_PIN_MC_BASE + 35)        // Not Exist in 96220
#define PAD_PIN_MC36            (PAD_PIN_MC_BASE + 36)        // Not Exist in 96220
#define PAD_PIN_MC37            (PAD_PIN_MC_BASE + 37)        // Not Exist in 96220
//MCx group end

//SN group begin
#define PAD_PIN_SN_BASE         100
#define PAD_PIN_SN_PCLK         (PAD_PIN_SN_BASE + 0)         ///< SN_PCLK
#define PAD_PIN_SN_MCLK         (PAD_PIN_SN_BASE + 1)         ///< SN_MCLK
#define PAD_PIN_SN_VD           (PAD_PIN_SN_BASE + 2)         ///< SN_VD
#define PAD_PIN_SN_HD           (PAD_PIN_SN_BASE + 3)         ///< SN_HD
#define PAD_PIN_SN_D0           (PAD_PIN_SN_BASE + 4)         ///< SN_D0
#define PAD_PIN_SN_D1           (PAD_PIN_SN_BASE + 5)         ///< SN_D1
#define PAD_PIN_SN_D2           (PAD_PIN_SN_BASE + 6)         ///< SN_D2
#define PAD_PIN_SN_D3           (PAD_PIN_SN_BASE + 7)         ///< SN_D3
#define PAD_PIN_SN_D4           (PAD_PIN_SN_BASE + 8)         ///< SN_D4
#define PAD_PIN_SN_D5           (PAD_PIN_SN_BASE + 9)         ///< SN_D5
#define PAD_PIN_SN_D6           (PAD_PIN_SN_BASE + 10)        ///< SN_D6
#define PAD_PIN_SN_D7           (PAD_PIN_SN_BASE + 11)        ///< SN_D7
#define PAD_PIN_SN_D8           (PAD_PIN_SN_BASE + 12)        ///< SN_D8
#define PAD_PIN_SN_D9           (PAD_PIN_SN_BASE + 13)        ///< SN_D9
#define PAD_PIN_SN_MES0         (PAD_PIN_SN_BASE + 18)        ///< SN_MES0
#define PAD_PIN_SN_MES1         (PAD_PIN_SN_BASE + 19)        ///< SN_MES1
#define PAD_PIN_SN_FLCTR        (PAD_PIN_SN_BASE + 20)        ///< SN_FLCTR
#define PAD_PIN_SN_SHUTTER      (PAD_PIN_SN_BASE + 21)        ///< SN_SHUTTER
//Backward Compatible
#define PAD_PIN_SN_D10          (PAD_PIN_SN_BASE + 14)        // Not Exist in 96220
#define PAD_PIN_SN_D11          (PAD_PIN_SN_BASE + 15)        // Not Exist in 96220
#define PAD_PIN_SN_D12          (PAD_PIN_SN_BASE + 16)        // Not Exist in 96220
#define PAD_PIN_SN_D13          (PAD_PIN_SN_BASE + 17)        // Not Exist in 96220

//SN group end

//PWM group begin
#define PAD_PIN_PWM_BASE        150
#define PAD_PIN_PWM0            (PAD_PIN_PWM_BASE + 0)        ///< PWM0
#define PAD_PIN_PWM1            (PAD_PIN_PWM_BASE + 1)        ///< PWM1
#define PAD_PIN_PWM2            (PAD_PIN_PWM_BASE + 2)        ///< PWM2
//Backward Compatible
#define PAD_PIN_PWM3            (PAD_PIN_PWM_BASE + 3)        // Not Exist in 96220
#define PAD_PIN_PWM4            (PAD_PIN_PWM_BASE + 4)        // Not Exist in 96220
#define PAD_PIN_PWM5            (PAD_PIN_PWM_BASE + 5)        // Not Exist in 96220
#define PAD_PIN_PWM6            (PAD_PIN_PWM_BASE + 6)        // Not Exist in 96220
#define PAD_PIN_PWM7            (PAD_PIN_PWM_BASE + 7)        // Not Exist in 96220
#define PAD_PIN_PWM8            (PAD_PIN_PWM_BASE + 8)        // Not Exist in 96220
#define PAD_PIN_PWM9            (PAD_PIN_PWM_BASE + 9)        // Not Exist in 96220
#define PAD_PIN_PWM10           (PAD_PIN_PWM_BASE + 10)       // Not Exist in 96220
#define PAD_PIN_PWM11           (PAD_PIN_PWM_BASE + 11)       // Not Exist in 96220
//PWM group end


//I2C group begin
#define PAD_PIN_I2C_BASE        170
#define PAD_PIN_I2CSDA          (PAD_PIN_I2C_BASE + 0)        ///< I2CSDA
#define PAD_PIN_I2CSCL          (PAD_PIN_I2C_BASE + 1)        ///< I2CSCL
//I2C group end

//I2S group begin
#define PAD_PIN_I2S_BASE        180
//Backward Compatible
#define PAD_PIN_ASBCLK          (PAD_PIN_I2S_BASE + 0)        // Not Exist in 96220
#define PAD_PIN_ASYNC           (PAD_PIN_I2S_BASE + 1)        // Not Exist in 96220
#define PAD_PIN_ASDO            (PAD_PIN_I2S_BASE + 2)        // Not Exist in 96220
#define PAD_PIN_ASDI            (PAD_PIN_I2S_BASE + 3)        // Not Exist in 96220
#define PAD_PIN_ASMCLK          (PAD_PIN_I2S_BASE + 4)        // Not Exist in 96220
//I2S group end


//SIF group begin
#define PAD_PIN_SB_BASE         190
#define PAD_PIN_SBCK0           (PAD_PIN_SB_BASE + 0)         ///< SBCK
#define PAD_PIN_SBDAT0          (PAD_PIN_SB_BASE + 1)         ///< SBDAT
#define PAD_PIN_SBCS0           (PAD_PIN_SB_BASE + 2)         ///< SBCS0
#define PAD_PIN_SBCS1           (PAD_PIN_SB_BASE + 3)         ///< SBCS1
//Backward Compatible
#define PAD_PIN_SBCS2           (PAD_PIN_SB_BASE + 4)         // Not Exist in 96220
#define PAD_PIN_SBCS3           (PAD_PIN_SB_BASE + 5)         // Not Exist in 96220
#define PAD_PIN_SBCK1           (PAD_PIN_SB_BASE + 6)         // Not Exist in 96220
#define PAD_PIN_SBDAT1          (PAD_PIN_SB_BASE + 7)         // Not Exist in 96220
#define PAD_PIN_SBCK23          (PAD_PIN_SB_BASE + 8)         // Not Exist in 96220
#define PAD_PIN_SBDAT23         (PAD_PIN_SB_BASE + 9)         // Not Exist in 96220
//SIF group end

//UART group begin
#define PAD_PIN_UART_BASE       205
#define PAD_PIN_UART_TX         (PAD_PIN_UART_BASE + 0)       ///< UART TX
#define PAD_PIN_UART_RX         (PAD_PIN_UART_BASE + 1)       ///< UART RX
//Backward Compatible
#define PAD_PIN_UART2_TX        (PAD_PIN_UART_BASE + 2)       // Not Exist in 96220
#define PAD_PIN_UART2_RX        (PAD_PIN_UART_BASE + 3)       // Not Exist in 96220
#define PAD_PIN_UART2_CTS       (PAD_PIN_UART_BASE + 4)       // Not Exist in 96220
#define PAD_PIN_UART2_RTS       (PAD_PIN_UART_BASE + 5)       // Not Exist in 96220
//UART group end

//REMOTE group begin
#define PAD_PIN_RMT_BASE        215
//Backward Compatible
#define PAD_PIN_RM_TX           (PAD_PIN_RMT_BASE + 0)         // Not Exist in 96220
//REMOTE group end

//LCD group begin
#define PAD_PIN_LCD_BASE        220
#define PAD_PIN_LCD0            (PAD_PIN_LCD_BASE + 0)         ///< LCD0
#define PAD_PIN_LCD1            (PAD_PIN_LCD_BASE + 1)         ///< LCD1
#define PAD_PIN_LCD2            (PAD_PIN_LCD_BASE + 2)         ///< LCD2
#define PAD_PIN_LCD3            (PAD_PIN_LCD_BASE + 3)         ///< LCD3
#define PAD_PIN_LCD4            (PAD_PIN_LCD_BASE + 4)         ///< LCD4
#define PAD_PIN_LCD5            (PAD_PIN_LCD_BASE + 5)         ///< LCD5
#define PAD_PIN_LCD6            (PAD_PIN_LCD_BASE + 6)         ///< LCD6
#define PAD_PIN_LCD7            (PAD_PIN_LCD_BASE + 7)         ///< LCD7
#define PAD_PIN_LCD8            (PAD_PIN_LCD_BASE + 8)         ///< LCD8
#define PAD_PIN_LCD9            (PAD_PIN_LCD_BASE + 9)         ///< LCD9
#define PAD_PIN_LCD10           (PAD_PIN_LCD_BASE + 10)        ///< LCD10
//Backward Compatible
#define PAD_PIN_LCD11           (PAD_PIN_LCD_BASE + 11)        // Not Exist in 96220
#define PAD_PIN_LCD12           (PAD_PIN_LCD_BASE + 12)        // Not Exist in 96220
#define PAD_PIN_LCD13           (PAD_PIN_LCD_BASE + 13)        // Not Exist in 96220
#define PAD_PIN_LCD14           (PAD_PIN_LCD_BASE + 14)        // Not Exist in 96220
#define PAD_PIN_LCD15           (PAD_PIN_LCD_BASE + 15)        // Not Exist in 96220
#define PAD_PIN_LCD16           (PAD_PIN_LCD_BASE + 16)        // Not Exist in 96220
#define PAD_PIN_LCD17           (PAD_PIN_LCD_BASE + 17)        // Not Exist in 96220
#define PAD_PIN_LCD18           (PAD_PIN_LCD_BASE + 18)        // Not Exist in 96220
#define PAD_PIN_LCD19           (PAD_PIN_LCD_BASE + 19)        // Not Exist in 96220
#define PAD_PIN_LCD20           (PAD_PIN_LCD_BASE + 20)        // Not Exist in 96220
#define PAD_PIN_LCD21           (PAD_PIN_LCD_BASE + 21)        // Not Exist in 96220
#define PAD_PIN_LCD22           (PAD_PIN_LCD_BASE + 22)        // Not Exist in 96220
#define PAD_PIN_LCD23           (PAD_PIN_LCD_BASE + 23)        // Not Exist in 96220
#define PAD_PIN_LCD24           (PAD_PIN_LCD_BASE + 24)        // Not Exist in 96220
#define PAD_PIN_LCD25           (PAD_PIN_LCD_BASE + 25)        // Not Exist in 96220
#define PAD_PIN_LCD26           (PAD_PIN_LCD_BASE + 26)        // Not Exist in 96220
#define PAD_PIN_LCD27           (PAD_PIN_LCD_BASE + 27)        // Not Exist in 96220
//LCD group end

//RESET & TEST group begin
#define PAD_PIN_TEST_BASE       250
//Backward Compatible
#define PAD_PIN_RESET           (PAD_PIN_TEST_BASE + 0)        // Not Exist in 96220
#define PAD_PIN_TESTEN          (PAD_PIN_TEST_BASE + 1)        // Not Exist in 96220
//RESET & TEST group end

//ICE group begin
#define PAD_PIN_ICE_BASE        255
#define PAD_PIN_IDI             (PAD_PIN_ICE_BASE + 0)         ///< IDI
#define PAD_PIN_ICK             (PAD_PIN_ICE_BASE + 1)         ///< ICK
#define PAD_PIN_GOICE           (PAD_PIN_ICE_BASE + 2)         ///< GOICE
#define PAD_PIN_IMS             (PAD_PIN_ICE_BASE + 3)         ///< IMS
//Backward Compatible
#define PAD_PIN_SRST            (PAD_PIN_ICE_BASE + 4)         // Not Exist in 96220
//ICE group end

//DGPIO group begin
#define PAD_PIN_DGPIO_BASE      260
#define PAD_PIN_DGPIO0          (PAD_PIN_DGPIO_BASE + 0)       ///< DGPIO0
#define PAD_PIN_DGPIO1          (PAD_PIN_DGPIO_BASE + 1)       ///< DGPIO1
#define PAD_PIN_DGPIO2          (PAD_PIN_DGPIO_BASE + 2)       ///< DGPIO2
#define PAD_PIN_DGPIO3          (PAD_PIN_DGPIO_BASE + 3)       ///< DGPIO3
#define PAD_PIN_DGPIO4          (PAD_PIN_DGPIO_BASE + 4)       ///< DGPIO4
//Backward Compatible
#define PAD_PIN_DGPIO5          (PAD_PIN_DGPIO_BASE + 5)       // Not Exist in 96220
#define PAD_PIN_DGPIO6          (PAD_PIN_DGPIO_BASE + 6)       // Not Exist in 96220
#define PAD_PIN_DGPIO7          (PAD_PIN_DGPIO_BASE + 7)       // Not Exist in 96220
#define PAD_PIN_DGPIO8          (PAD_PIN_DGPIO_BASE + 8)       // Not Exist in 96220
//DGPIO group end

//HDMI group begin
#define PAD_PIN_HDMI_BASE       300
#define PAD_PIN_HDMI_DDCCDA     (PAD_PIN_HDMI_BASE + 0)         ///< HDMI DDC CDA
#define PAD_PIN_HDMI_DDCSCL     (PAD_PIN_HDMI_BASE + 1)         ///< HDMI DDC SCL
#define PAD_PIN_HDMI_HOTPLUG    (PAD_PIN_HDMI_BASE + 2)         ///< HDMI Hotplug
//HDMI group end

//DGPIO alias group begin
#define PAD_PIN_USB_DRVBUS      PAD_PIN_DGPIO0                 ///< USB DRVBUS
//DGPIO alias group end

//DMA group begin
#define PAD_PIN_DMA_BASE        (0x80000000)
#define PAD_PIN_DMA_DATA_LSB    (PAD_PIN_DMA_BASE + 0)          ///< DMA DATA LSB
#define PAD_PIN_DMA_DATA_MSB    (PAD_PIN_DMA_BASE + 1)          ///< DMA DATA MSB
#define PAD_PIN_DMA_DQS0        (PAD_PIN_DMA_BASE + 2)          ///< DMA DQS0
#define PAD_PIN_DMA_DQS1        (PAD_PIN_DMA_BASE + 3)          ///< DMA DQS1
#define PAD_PIN_DMA_CTRL        (PAD_PIN_DMA_BASE + 4)          ///< DMA CTRL
//DMA group end
//@}



/**
    @name Pad slew rate pin ID.

    Pad slew rate pin ID.

    Pad ID of pad_setSlewRate(), pad_getSlewRate()
*/
//@{
//Card Slew Rate group begin
#define PAD_SR_CARD             0       ///< Card group
//Card group end

//Sensor Slew Rate group begin
#define PAD_SR_SENSOR           1       ///< Sensor group
//Sensor group end

//PWM Slew Rate group begin
#define PAD_SR_PWM              2       ///< PWM group
//PWM Slew Rate group end

//AC97 & I2S Slew Rate group begin
#define PAD_SR_SERIAL           3       ///< Serial group
//AC97 & I2S Slew Rate group end

//LCD Slew Rate group begin
#define PAD_SR_LCD              4       ///< LCD group
//LCD Slew Rate group end

//LCD Slew Rate group begin
#define PAD_SR_ICE              5       ///< ICE group
//LCD Slew Rate group end

//DGPIO Slew Rate group begin
#define PAD_SR_DGPIO            6       ///< DGPIO group
//DGPIO Slew Rate group end

//DGPIO Slew Rate group begin
#define PAD_SR_HDMI             7       ///< HDMI group
//DGPIO Slew Rate group end

//Backward Compatible
#define PAD_SR_I2S_AC97         8       // Not Exist in 96220

//DMA slew Rate group begin
#define PAD_SR_DMA_BASE         (0x80000000)
#define PAD_SR_DMA_DATA_LSB     (PAD_SR_DMA_BASE+0)             ///< DMA DATA LSB
#define PAD_SR_DMA_DATA_MSB     (PAD_SR_DMA_BASE+1)             ///< DMA DATA MSB
#define PAD_SR_DMA_DQS0         (PAD_SR_DMA_BASE+2)             ///< DMA DQS0
#define PAD_SR_DMA_DQS1         (PAD_SR_DMA_BASE+3)             ///< DMA DQS1
#define PAD_SR_DMA_CTRL         (PAD_SR_DMA_BASE+4)             ///< DMA CTRL
#define PAD_SR_DMA_CLK          (PAD_SR_DMA_BASE+5)             ///< DMA CLK
//DMA slew Rate group end
//@}


#define PAD_DS_GROUP2468       0
#define PAD_DS_GROUP481216     0x1000

/**
    @name Pad driving pin ID.

    Pad driving pin ID.

    Pad ID of pad_setDrivingSink(), pad_getDrivingSink()
*/
//@{
//MCx group begin
#define PAD_DS_MC_BASE         0
#define PAD_DS_MC0             (PAD_DS_MC_BASE + 0)                         ///< MC0
#define PAD_DS_MC1             (PAD_DS_MC_BASE + 1)                         ///< MC1
#define PAD_DS_MC2             ((PAD_DS_MC_BASE+2) | PAD_DS_GROUP481216)    ///< MC2
#define PAD_DS_MC3             (PAD_DS_MC_BASE + 3)                         ///< MC3
#define PAD_DS_MC4             (PAD_DS_MC_BASE + 4)                         ///< MC4
#define PAD_DS_MC5             (PAD_DS_MC_BASE + 5)                         ///< MC5
#define PAD_DS_MC6             (PAD_DS_MC_BASE + 6)                         ///< MC6
#define PAD_DS_MC7             (PAD_DS_MC_BASE + 7)                         ///< MC7
#define PAD_DS_MC8             ((PAD_DS_MC_BASE+8) | PAD_DS_GROUP481216)    ///< MC8
#define PAD_DS_MC9             (PAD_DS_MC_BASE + 9)                         ///< MC9
#define PAD_DS_MC10            (PAD_DS_MC_BASE + 10)                        ///< MC10
#define PAD_DS_MC11            ((PAD_DS_MC_BASE+11) | PAD_DS_GROUP481216)   ///< MC11
#define PAD_DS_MC12            (PAD_DS_MC_BASE + 12)                        ///< MC12
#define PAD_DS_MC13            (PAD_DS_MC_BASE + 13)                        ///< MC13
#define PAD_DS_MC14            (PAD_DS_MC_BASE + 14)                        ///< MC14
//Backward Compatible
#define PAD_DS_MC15            (PAD_DS_MC_BASE + 15)        // Not Exist in 96220
#define PAD_DS_MC16            (PAD_DS_MC_BASE + 16)        // Not Exist in 96220
#define PAD_DS_MC17            (PAD_DS_MC_BASE + 17)        // Not Exist in 96220
#define PAD_DS_MC18            (PAD_DS_MC_BASE + 18)        // Not Exist in 96220
#define PAD_DS_MC19            (PAD_DS_MC_BASE + 19)        // Not Exist in 96220
#define PAD_DS_MC20            (PAD_DS_MC_BASE + 20)        // Not Exist in 96220
#define PAD_DS_MC21            (PAD_DS_MC_BASE + 21)        // Not Exist in 96220
#define PAD_DS_MC22            (PAD_DS_MC_BASE + 22)        // Not Exist in 96220
#define PAD_DS_MC23            (PAD_DS_MC_BASE + 23)        // Not Exist in 96220
#define PAD_DS_MC24            (PAD_DS_MC_BASE + 24)        // Not Exist in 96220
#define PAD_DS_MC25            (PAD_DS_MC_BASE + 25)        // Not Exist in 96220
#define PAD_DS_MC26            (PAD_DS_MC_BASE + 26)        // Not Exist in 96220
#define PAD_DS_MC27            (PAD_DS_MC_BASE + 27)        // Not Exist in 96220
#define PAD_DS_MC28            (PAD_DS_MC_BASE + 28)        // Not Exist in 96220
#define PAD_DS_MC29            (PAD_DS_MC_BASE + 29)        // Not Exist in 96220
#define PAD_DS_MC30            (PAD_DS_MC_BASE + 30)        // Not Exist in 96220
#define PAD_DS_MC31            (PAD_DS_MC_BASE + 31)        // Not Exist in 96220
#define PAD_DS_MC32            (PAD_DS_MC_BASE + 32)        // Not Exist in 96220
#define PAD_DS_MC33            (PAD_DS_MC_BASE + 33)        // Not Exist in 96220
#define PAD_DS_MC34            (PAD_DS_MC_BASE + 34)        // Not Exist in 96220
#define PAD_DS_MC35            (PAD_DS_MC_BASE + 35)        // Not Exist in 96220
#define PAD_DS_MC36            (PAD_DS_MC_BASE + 36)        // Not Exist in 96220
#define PAD_DS_MC37            (PAD_DS_MC_BASE + 37)        // Not Exist in 96220
//MCx group end

//Sensor group Driving/Sink begin
#define PAD_DS_SN_BASE          16
#define PAD_DS_SN_D0            (PAD_DS_SN_BASE+0)          ///< Sensor D0
#define PAD_DS_SN_D1            (PAD_DS_SN_BASE+1)          ///< Sensor D1
#define PAD_DS_SN_D2            (PAD_DS_SN_BASE+2)          ///< Sensor D2
#define PAD_DS_SN_D3            (PAD_DS_SN_BASE+3)          ///< Sensor D3
#define PAD_DS_SN_D4            (PAD_DS_SN_BASE+4)          ///< Sensor D4
#define PAD_DS_SN_D5            (PAD_DS_SN_BASE+5)          ///< Sensor D5
#define PAD_DS_SN_SENSOR        (PAD_DS_SN_BASE+6)          ///< Sensor D6~D9
#define PAD_DS_SN_PXCLK         ((PAD_DS_SN_BASE+7) | PAD_DS_GROUP481216)   ///< Sensor PXCLK
#define PAD_DS_SN_VDHD          ((PAD_DS_SN_BASE+8) | PAD_DS_GROUP481216)   ///< Sensor VDHD
#define PAD_DS_SN_SHUTTER       ((PAD_DS_SN_BASE+9) | PAD_DS_GROUP481216)   ///< Sensor SHUTTER
#define PAD_DS_SN_MCLK          ((PAD_DS_SN_BASE+10) | PAD_DS_GROUP481216)  ///< Sensor MCLK
#define PAD_DS_SN_MES           ((PAD_DS_SN_BASE+11) | PAD_DS_GROUP481216)  ///< Sensor MES0/MES1
#define PAD_DS_SN_FLCTR         ((PAD_DS_SN_BASE+12) | PAD_DS_GROUP481216)  ///< Sensor FLCTR
//Sensor group Driving/Sink end

//PWM group Driving/Sink begin
#define PAD_DS_PWM_BASE         38
#define PAD_DS_PWM0             ((PAD_DS_PWM_BASE + 0) | PAD_DS_GROUP481216)     ///< PWM0
#define PAD_DS_PWM1             ((PAD_DS_PWM_BASE + 1) | PAD_DS_GROUP481216)     ///< PWM1
#define PAD_DS_PWM2             ((PAD_DS_PWM_BASE + 2) | PAD_DS_GROUP481216)     ///< PWM2
//Backward Compatible
#define PAD_DS_PWM3            ((PAD_DS_PWM_BASE + 3) | PAD_DS_GROUP481216)     // Not Exist in 96220
#define PAD_DS_PWM4            ((PAD_DS_PWM_BASE + 4) | PAD_DS_GROUP481216)     // Not Exist in 96220
#define PAD_DS_PWM5            ((PAD_DS_PWM_BASE + 5) | PAD_DS_GROUP481216)     // Not Exist in 96220
#define PAD_DS_PWM6            ((PAD_DS_PWM_BASE + 6) | PAD_DS_GROUP481216)     // Not Exist in 96220
#define PAD_DS_PWM7            ((PAD_DS_PWM_BASE + 7) | PAD_DS_GROUP481216)     // Not Exist in 96220
#define PAD_DS_PWM8            ((PAD_DS_PWM_BASE + 8) | PAD_DS_GROUP481216)     // Not Exist in 96220
#define PAD_DS_PWM9            ((PAD_DS_PWM_BASE + 9) | PAD_DS_GROUP481216)     // Not Exist in 96220
#define PAD_DS_PWM10           ((PAD_DS_PWM_BASE + 10) | PAD_DS_GROUP481216)    // Not Exist in 96220
#define PAD_DS_PWM11           ((PAD_DS_PWM_BASE + 11) | PAD_DS_GROUP481216)    // Not Exist in 96220
//PWM group Driving/Sink end

//I2C group Driving/Sink begin
#define PAD_DS_I2C_BASE         29
#define PAD_DS_I2C              (PAD_DS_I2C_BASE + 0)       ///< I2C SDA/SCL
//Backward Compatible
#define PAD_DS_I2C_SDA          (PAD_DS_I2C)
#define PAD_DS_I2C_SCL          (PAD_DS_I2C)
//I2C group Driving/Sink end

//AC97 & I2S group Driving/Sink begin
#define PAD_DS_I2S_BASE        55
#define PAD_DS_I2S_ASMCLK      ((PAD_DS_I2S_BASE + 1) | PAD_DS_GROUP481216)     ///< Audio MCLK
//Backward Compatible
#define PAD_DS_I2S_ASBCLK      (PAD_DS_I2S_BASE + 0)                            // Not Exist in 96460
//AC97 & I2S  group Driving/Sink end


//SIF group Driving/Sink begin
#define PAD_DS_SIF_BASE         30
#define PAD_DS_SIF0             (PAD_DS_SIF_BASE + 0)                           ///< SIF0
#define PAD_DS_SIF1             (PAD_DS_SIF_BASE + 1)                           ///< SIF1
//Backward Compatible
#define PAD_DS_SIF              (PAD_DS_SIF0)
//SIF group Driving/Sink end


//UART group Driving/Sink begin
#define PAD_DS_UART_BASE        41
#define PAD_DS_UART_TX          (PAD_DS_UART_BASE + 0)                          ///< UART TX
#define PAD_DS_UART_RX          (PAD_DS_UART_BASE + 1)                          ///< UART RX
//Backward Compatible
#define PAD_DS_UART2_BASE      58
#define PAD_DS_UART2_TX        ((PAD_DS_UART2_BASE + 0) | PAD_DS_GROUP481216)   ///< UART TX
#define PAD_DS_UART2_RX        ((PAD_DS_UART2_BASE + 1) | PAD_DS_GROUP481216)   ///< UART RX
#define PAD_DS_UART2_CTS       ((PAD_DS_UART2_BASE + 2) | PAD_DS_GROUP481216)   ///< UART CTS
#define PAD_DS_UART2_RTS       ((PAD_DS_UART2_BASE + 3) | PAD_DS_GROUP481216)   ///< UART RTS
//UART group Driving/Sink end

//REMOTE group  Driving/Sink begin
#define PAD_DS_RMT_BASE        62
#define PAD_DS_RMT_RX          ((PAD_DS_RMT_BASE + 0) | PAD_DS_GROUP481216)     ///< Remote RX
//REMOTE group  Driving/Sink end

//LCD group Driving/Sink begin
#define PAD_DS_LCD_BASE         32
#define PAD_DS_LCD              (PAD_DS_LCD_BASE + 0)                           ///< LCD Other
#define PAD_DS_LCD8             ((PAD_DS_LCD_BASE + 1) | PAD_DS_GROUP481216)    ///< LCD8
#define PAD_DS_LCD10            ((PAD_DS_LCD_BASE + 2) | PAD_DS_GROUP481216)    ///< LCD10
//Backward Compatible
#define PAD_DS_LCD18            ((PAD_DS_LCD_BASE + 2)  | PAD_DS_GROUP481216)   // Not Exist in 96220
#define PAD_DS_LCD19            (PAD_DS_LCD_BASE + 3)                           // Not Exist in 96220
//LCD group Driving/Sink end

//ICE group Driving/Sink begin
#define PAD_DS_ICE_BASE         35
#define PAD_DS_ICE              (PAD_DS_ICE_BASE + 0)                           ///< ICE group
//ICE group Driving/Sink begin

//DGIP group Driving/Sink begin
#define PAD_DS_DGPIO_BASE       48
#define PAD_DS_DGPIO0           ((PAD_DS_DGPIO_BASE + 0) | PAD_DS_GROUP481216)  ///< DGPIO group
#define PAD_DS_DGPIO1           ((PAD_DS_DGPIO_BASE + 1) | PAD_DS_GROUP481216)  ///< DGPI1 group
#define PAD_DS_DGPIO2           ((PAD_DS_DGPIO_BASE + 2) | PAD_DS_GROUP481216)  ///< DGPI2 group
#define PAD_DS_DGPIO3           ((PAD_DS_DGPIO_BASE + 3) | PAD_DS_GROUP481216)  ///< DGPI3 group
#define PAD_DS_DGPIO4           ((PAD_DS_DGPIO_BASE + 4) | PAD_DS_GROUP481216)  ///< DGPI4 group
//Backward Compatible
#define PAD_DS_DGPIO            ((PAD_DS_DGPIO_BASE + 0) | PAD_DS_GROUP481216)  ///< DGPIO group
//DGIP group Driving/Sink end

//HDMI group Driving/Sink begin
#define PAD_DS_HDMI_BASE        36
#define PAD_DS_HDMI_DDC         ((PAD_DS_HDMI_BASE+0) | PAD_DS_GROUP481216)     ///< HDMI DDC
#define PAD_DS_HDMI_HOTPLUG     (PAD_DS_HDMI_BASE + 1)                          ///< HDMI Hotplug
//HDMI group Driving/Sink end
//@}

// for backward compatibility
#define PAD_PIN_SBCK           PAD_PIN_SBCK0                                    ///< SIF SBCK
#define PAD_PIN_SBDAT          PAD_PIN_SBDAT0                                   ///< SIF SBDAT

extern ER pad_setPullUpDown(UINT pin, PAD_PULL pulltype);
extern ER pad_getPullUpDown(UINT pin, PAD_PULL *pulltype);
extern ER pad_setSlewRate(UINT name, PAD_SLEWRATE rate);
extern ER pad_getSlewRate(UINT name, PAD_SLEWRATE *rate);
extern ER pad_setDrivingSink(UINT name, PAD_DRIVINGSINK driving);
extern ER pad_getDrivingSink(UINT name, PAD_DRIVINGSINK *driving);

//extern ER pad_setSSTL2(UINT param, UINT value);
//extern ER pad_getSSTL2(UINT param, UINT *value);

//@}

#endif
