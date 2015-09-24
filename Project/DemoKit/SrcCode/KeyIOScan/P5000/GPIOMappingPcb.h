/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMapphing.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of GPIO mapping
                This file is the header file of GPIO mapping

    @note       Nothing.
    @date       2005/12/05
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _GPIOMAPPINGPCB_H
#define _GPIOMAPPINGPCB_H

#include "pad.h"
#include "GPIO.h"
#include "SysCfg.h"
//-------------------------------------------------------------------------------------------------

// LCD
#define GPIO_LCD_BL_CTRL            D_GPIO_0
#define GPIO_LCD_BL_PWR             (D_GPIO_20)
#define GPIO_LCD_RESET              (D_GPIO_0)
//#define GPIO_LCD_RESET              (P_GPIO_8)

// TV
#define GPIO_TV_DETECT              (D_GPIO_20)
#define GPIOINT_TV_DETECT           (GPIO_INT_52)
#define DRAM_KEY_TV_DETECT          (DMA_KEY_A4)

//#define GPIOINT_LCD_BL_PWR          (GPIO_INT_52)
//#define DRAM_KEY_LCD_PWR            (DMA_KEY_A4)

#define GPIO_LCD_SIF_SEN            P_GPIO_2
#define GPIO_LCD_SIF_SCK            P_GPIO_0
#define GPIO_LCD_SIF_SDA            P_GPIO_1
//-------------------------------------------------------------------------------------------------
// Storage card detect
#define GPIO_CARD_DETECT            (D_GPIO_17)
#define GPIOINT_CARD_DETECT         (GPIO_INT_49)
#define DRAM_KEY_CARD_DETECT        (DMA_KEY_A1)
//-------------------------------------------------------------------------------------------------
// SD card write protect
#define GPIO_CARD_WP                   (D_GPIO_25)// (D_GPIO_21)//(D_GPIO_16)
#define GPIOINT_CARD_WP             (GPIO_INT_57)// (GPIO_INT_53)//(GPIO_INT_48)
#define DRAM_KEY_CARD_WP          (DMA_KEY_A9)// (DMA_KEY_A5)//  (DMA_KEY_A0)
//-------------------------------------------------------------------------------------------------
// Storage card power control pin
#define DGPIO_CARD_POWER            (9 | GPIO_IS_DGPIO)
//-------------------------------------------------------------------------------------------------
// IR Remote
#define GPIO_IR_RECEIVER            (3 | GPIO_IS_DGPIO)
#define PAD_IR_RECEIVER             PAD_PIN_DGPIO3
#define IR_RECEIVER_INTR_ID         GPIO_INT_11
//-------------------------------------------------------------------------------------------------
//Video Light
#define VIDEO_FLOOD_LIGHT           S_GPIO_18
//-------------------------------------------------------------------------------------------------
//Back Det
#define GPIO_BACK_DET                    (D_GPIO_16)
#define GPIOINT_BACK_DET              (GPIO_INT_48)
#define DRAM_KEY_BACK_DET           (DMA_KEY_A0)

// Sensor
#define GPIO_SENSOR_RESET            S_GPIO_17 //D_GPIO_8

//-------------------------------------------------------------------------------------------------
// Key Define
#if (_LCDTYPE_ == _LCDTYPE_TXDT240C_)
#define GPIO_KEY_ENTER              D_GPIO_25
#define GPIOINT_KEY_ENTER           (GPIO_INT_57)
#define DRAM_KEY_ENTER              (DMA_KEY_A9)
#else   // _LCDTYPE_LIL9341C_
// REC key
#define GPIO_KEY_REC            (D_GPIO_25)
#define GPIOINT_KEY_REC         (GPIO_INT_57)
#define DRAM_KEY_KEY_REC        (DMA_KEY_A9)
// Playback key
#define GPIO_KEY_PLAYBACK       (D_GPIO_18)
#define GPIOINT_KEY_PLAYBACK    (GPIO_INT_50)
#define DRAM_KEY_KEY_PLAYBACK   (DMA_KEY_A2)
// Mode key
#define GPIO_KEY_MODE           (D_GPIO_23)
#define GPIOINT_KEY_MODE        (GPIO_INT_55)
#define DRAM_KEY_KEY_MODE       (DMA_KEY_A7)
// Menu key
#define GPIO_KEY_MENU           (D_GPIO_24)
#define GPIOINT_KEY_MENU        (GPIO_INT_56)
#define DRAM_KEY_KEY_MENU       (DMA_KEY_A8)

//LCD ROTATE DETECT
#define GPIO_LCD_ROTATE         (D_GPIO_19)
#define GPIOINT_LCD_ROTATE      (GPIO_INT_51)
#define DRAM_LCD_ROTATE         (DMA_KEY_A3)

#define GPIO_LCD_ROTATE2        (D_GPIO_27)
#define GPIOINT_LCD_ROTATE2     (GPIO_INT_59)
#define DRAM_LCD_ROTATE2        (DMA_KEY_A11)

// LCD close pin
#define GPIO_LCD_CLOSE          (D_GPIO_22)
#define GPIOINT_LCD_CLOSE       (GPIO_INT_54)
#define DRAM_LCD_CLOSE          (DMA_KEY_A6)

//Sensor Rotate 
#define GPIO_SENSOR_ROTATE          (D_GPIO_19)
#define GPIOINT_SENSOR_ROTATE      (GPIO_INT_51)
#define DRAM_SENSOR_ROTATE         (DMA_KEY_A3)

#define GPIO_LENS_IO_IN_2           (D_GPIO_3)//SN_MES0

#define GPIO_ENDE_SCL         P_GPIO_0

#define GPIO_ENDE_SDA        D_GPIO_1//C_GPIO_1

//#define GPIO_GSENSOR_SCK      S_GPIO_16
//#define GPIO_GSENSOR_SDA       S_GPIO_15
#endif

#endif

//@}

