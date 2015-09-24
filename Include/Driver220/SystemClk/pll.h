/**
    PLL Configuration module header

    PLL Configuration module header file.

    @file       pll.h
    @ingroup    mIHALSysCG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _PLL_H
#define _PLL_H

#include "Type.h"

/** \addtogroup mIHALSysCG */
//@{


#define PLL_CLKSEL_R1_OFFSET      0
#define PLL_CLKSEL_R2_OFFSET      32
#define PLL_CLKSEL_ADO_OFFSET     64
#define PLL_CLKSEL_R3_OFFSET      96
#define PLL_CLKSEL_R4_OFFSET      128
#define PLL_CLKSEL_R5_OFFSET      160
#define PLL_CLKSEL_R6_OFFSET      192

/**
    Clock select ID

    Clock select ID for pll_setClockRate() & pll_getClockRate().
*/
typedef enum
{
    //Clock Rate 1 bit definition
    PLL_CLKSEL_CPU =            0,         ///< Clock Select Module ID: CPU
    PLL_CLKSEL_AHB =            4,         ///< Clock Select Module ID: AHB
    PLL_CLKSEL_APB =            8,         ///< Clock Select Module ID: APB
    PLL_CLKSEL_DMA =            12,        ///< Clock Select Module ID: DMA
    PLL_CLKSEL_IPE =            16,        ///< Clock Select Module ID: IPE
    PLL_CLKSEL_IME =            20,        ///< Clock Select Module ID: IME
    PLL_CLKSEL_JPEG =           24,        ///< Clock Select Module ID: JPEG

    //Clock rate 2 bit definition
    PLL_CLKSEL_SIE_MCLKSRC =    32 + 1,    ///< Clock Select Module ID: SIE MCLK source
//    PLL_CLKSEL_ADO_CLKSRC =     32 + 4,    ///< Clock Select Module ID: ADO clock source (reserved for backword compatible)
    PLL_CLKSEL_SIE_MCLKDIV =    32 + 8,    ///< Clock Select Module ID: SIE MCLK divider
    PLL_CLKSEL_IDE_CLKDIV =     32 + 16,   ///< Clock Select Module ID: IDE clock divider
    PLL_CLKSEL_ADO_CLKMUX =     32 + 24,   ///< Clock Select Module ID: ADO clock mux

    //ADO Clock rate bit definition
//    PLL_CLKSEL_ADO_INT_CLKDIV = 64 + 0,    ///< Clock Select Module ID: ADO clock internal clock divider
//    PLL_CLKSEL_ADO_INTMETHOD =  64 + 16,   ///< Clock Select Module ID: ADO clock internal method
//    PLL_CLKSEL_ADO_BCLKRATIO =  64 + 20,   ///< Clock Select Module ID: ADO bit clock ratio

    //Clock rate 3 bit definition
//    PLL_CLKSEL_PRE =            96 + 0,    ///< Clock Select Module ID: PRE
    PLL_CLKSEL_SIE_PXCLKSRC =   96 + 3,    ///< Clock Select Module ID: SIE pixel clock source
//    PLL_CLKSEL_FDE =            96 + 4,    ///< Clock Select Module ID: FDE
    PLL_CLKSEL_IFE =            96 + 8,    ///< Clock Select Module ID: IFE
//    PLL_CLKSEL_SDIO2 =          96 + 12,   ///< Clock Select Module ID: SDIO2
//    PLL_CLKSEL_DCE =            96 + 16,   ///< Clock Select Module ID: DCE
//    PLL_CLKSEL_DIS =            96 + 20,   ///< Clock Select Module ID: DIS
    PLL_CLKSEL_GRAPHIC =        96 + 24,   ///< Clock Select Module ID: GRAPHIC
    PLL_CLKSEL_SDIO =           96 + 28,   ///< Clock Select Module ID: SDIO
    PLL_CLKSEL_NAND =           96 + 28,   ///< Clock Select Module ID: NAND

    //Clock rate 4 bit definition
    PLL_CLKSEL_IDE_CLKSRC =     128 + 0,   ///< Clock Select Module ID: IDE clock source
//    PLL_CLKSEL_LVDS_CLKPHASE =  128 + 4,   ///< Clock Select Module ID: LVDS clock phase
    PLL_CLKSEL_MI_CLKDIV =      128 + 8,   ///< Clock Select Module ID: MI clock divider
    PLL_CLKSEL_SIE_PXCLKDIV =   128 + 16,  ///< Clock Select Module ID: SIE pixel clock divider

    //Clock rate 5 bit definition
    PLL_CLKSEL_SPI_CLKDIV =     160 + 0,   ///< Clock Select Module ID: SPI clock divider
    PLL_CLKSEL_SDIO_CLKDIV =    160 + 8,   ///< Clock Select Module ID: SDIO clock divider
//    PLL_CLKSEL_SDIO2_CLKDIV =   160 + 20,  ///< Clock Select Module ID: SDIO2 clock divider

    //Clock rate 6 bit definition
//    PLL_CLKSEL_I2C_CLKDIV =     192 + 0,   ///< Clock Select Module ID: I2C clock divider
//    PLL_CLKSEL_DAI_CLKDIV =     192 + 8,   ///< Clock Select Module ID: DAI clock divider

    ENUM_DUMMY4WORD(PLL_CLKSEL)
}PLL_CLKSEL;

// Backword compatible
#define PLL_CLKSEL_PRE_CLKSRC   PLL_CLKSEL_SIE_PXCLKSRC
#define PLL_CLKSEL_MS       PLL_CLKSEL_SDIO
#define PLL_CLKSEL_DCE      E_PLL_CLKSEL
#define PLL_CLKSEL_DIS      E_PLL_CLKSEL
#define PLL_CLKSEL_FDE      E_PLL_CLKSEL
#define PLL_CLKSEL_SDIO2    E_PLL_CLKSEL
#define PLL_CLKSEL_SDIO2_CLKDIV E_PLL_CLKSEL
#define PLL_CLKSEL_PRE      E_PLL_CLKSEL
#define PLL_CLKSEL_DAI_CLKDIV   E_PLL_CLKSEL
#define PLL_CLKSEL_ADO_CLKSRC   E_PLL_CLKSEL
#define PLL_CLKSEL_ADO_INT_CLKDIV   E_PLL_CLKSEL
#define PLL_CLKSEL_ADO_INTMETHOD    E_PLL_CLKSEL
#define PLL_CLKSEL_ADO_BCLKRATIO    E_PLL_CLKSEL



/**
    @name   CPU clock rate value

    CPU clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_CPU).
*/
//@{
#define PLL_CLKSEL_CPU_240        (0x06 << PLL_CLKSEL_CPU)  ///< CPU clock rate = 240 MHz
#define PLL_CLKSEL_CPU_160        (0x07 << PLL_CLKSEL_CPU)  ///< CPU clock rate = 160 MHz
#define PLL_CLKSEL_CPU_120        (0x00 << PLL_CLKSEL_CPU)  ///< CPU clock rate = 120 MHz
#define PLL_CLKSEL_CPU_80         (0x01 << PLL_CLKSEL_CPU)  ///< CPU clock rate = 80 MHz
#define PLL_CLKSEL_CPU_60         (0x02 << PLL_CLKSEL_CPU)  ///< CPU clock rate = 60 MHz
#define PLL_CLKSEL_CPU_48         (0x03 << PLL_CLKSEL_CPU)  ///< CPU clock rate = 48 MHz
#define PLL_CLKSEL_CPU_PLL2_D3    (0x04 << PLL_CLKSEL_CPU)  ///< CPU clock rate = PLL2/3 MHz
//@}
//Backward compatible
#define PLL_CLKSEL_CPU_24         (PLL_CLKSEL_CPU_48)       //< CPU clock rate = 24 MHz (NOT supported by NT96220)

/**
    @name   AHB clock rate value

    AHB clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_AHB).
*/
//@{
#define PLL_CLKSEL_AHB_CPUCLK     (0x00 << PLL_CLKSEL_AHB)  ///< AHB clock rate = CPU Frequency
#define PLL_CLKSEL_AHB_CPUCLK_D2  (0x01 << PLL_CLKSEL_AHB)  ///< AHB clock rate = CPU Frequency / 2
//@}

/**
    @name   APB clock rate value

    APB clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_APB).
*/
//@{
#define PLL_CLKSEL_APB_80         (0x00 << PLL_CLKSEL_APB)  ///< APB clock rate = 80 MHz
#define PLL_CLKSEL_APB_48         (0x01 << PLL_CLKSEL_APB)  ///< APB clock rate = 48 MHz
//@}
//Backward compatible
#define PLL_CLKSEL_APB_60         (PLL_CLKSEL_APB_48)       //< APB clock rate = 60 MHz (NOT supported by NT96220)

/**
    @name   DMA clock rate value

    DMA clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_DMA).
*/
//@{
#define PLL_CLKSEL_DMA_80           (0x00 << PLL_CLKSEL_DMA)    ///< DMA clock rate = 80 MHz
#define PLL_CLKSEL_DMA_120          (0x01 << PLL_CLKSEL_DMA)    ///< DMA clock rate = 120 MHz
#define PLL_CLKSEL_DMA_160          (0x02 << PLL_CLKSEL_DMA)    ///< DMA clock rate = 160 MHz
#define PLL_CLKSEL_DMA_PLL2_D3      (0x03 << PLL_CLKSEL_DMA)    ///< DMA clock rate = PLL2/3 MHz
//@}
// Backward compatible
#define PLL_CLKSEL_DMA_240          (PLL_CLKSEL_DMA_PLL2_D3)
#define PLL_CLKSEL_DMA_96           (PLL_CLKSEL_DMA_80)

/**
    @name   IPE clock rate value

    IPE clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_IPE).
*/
//@{
#define PLL_CLKSEL_IPE_48         (0x00 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET)) ///< IPE clock rate = 48 MHz
#define PLL_CLKSEL_IPE_60         (0x01 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET)) ///< IPE clock rate = 60 MHz
#define PLL_CLKSEL_IPE_80         (0x02 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET)) ///< IPE clock rate = 80 MHz
#define PLL_CLKSEL_IPE_96         (0x03 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET)) ///< IPE clock rate = 96 MHz
//@}
//Backward compatible
#define PLL_CLKSEL_IPE_40         (PLL_CLKSEL_IPE_48)
#define PLL_CLKSEL_IPE_30         (PLL_CLKSEL_IPE_48)

/**
    @name   IME clock rate value

    IME clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_IME).
*/
//@{
#define PLL_CLKSEL_IME_60         (0x00 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET)) ///< IME clock rate = 60 MHz
#define PLL_CLKSEL_IME_80         (0x01 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET)) ///< IME clock rate = 80MHz
#define PLL_CLKSEL_IME_96         (0x02 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET)) ///< IME clock rate = 96 MHz
#define PLL_CLKSEL_IME_120        (0x03 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET)) ///< IME clock rate = 120 MHz
//@}
//Backward compatible
#define PLL_CLKSEL_IME_48         (PLL_CLKSEL_IME_60)
#define PLL_CLKSEL_IME_40         (PLL_CLKSEL_IME_60)
#define PLL_CLKSEL_IME_30         (PLL_CLKSEL_IME_60)

/**
    @name   JPEG clock rate value

    JPEG clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_JPEG).
*/
//@{
//#define PLL_CLKSEL_JPEG_160       (0x00 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_JPEG_120       (0x01 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R1_OFFSET))    ///< JPEG clock rate = 120 MHz
#define PLL_CLKSEL_JPEG_80        (0x02 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R1_OFFSET))    ///< JPEG clock rate = 80 MHz
#define PLL_CLKSEL_JPEG_60        (0x03 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R1_OFFSET))    ///< JPEG clock rate = 60 MHz
//@}

/**
    @name   SIE MCLK clock source value

    SIE MCLK clock source value

    @note This is for pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC).
*/
//@{
#define PLL_CLKSEL_SIE_MCLKSRC_PLL1     (0x00 << (PLL_CLKSEL_SIE_MCLKSRC - PLL_CLKSEL_R2_OFFSET))   ///< SIE MCLK from PLL1
#define PLL_CLKSEL_SIE_MCLKSRC_PLL2     (0x01 << (PLL_CLKSEL_SIE_MCLKSRC - PLL_CLKSEL_R2_OFFSET))   ///< SIE MCLK from PLL2
//@}
//Backward compatible
#define PLL_CLKSEL_SIE_MCLKSRC_PLL2_D2  PLL_CLKSEL_SIE_MCLKSRC_PLL2

/*
    @name   Audio clock source value

    Audio clock source value (Backward compatible)

    @note This is for pll_setClockRate(PLL_CLKSEL_ADO_CLKSRC).
*/
//@{
#define PLL_CLKSEL_ADO_CLKSRC_EXT       (0x00)
#define PLL_CLKSEL_ADO_CLKSRC_INT       (0x01)
//@}

/**
    @name   SIE clock divider

    SIE clock divider

    @param[in] x    MCLK divider. MCLK frequency will be (Clock from PLL_CLKSEL_SIE_MCLKSRC) / (x + 1).

    @note This if for pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV).
*/
//@{
#define PLL_SIE_MCLKDIV(x)              (x << (PLL_CLKSEL_SIE_MCLKDIV - PLL_CLKSEL_R2_OFFSET))  ///< MCLK DIV Macro
//@}


/**
    @name   IDE clock divider

    IDE clock divider

    @param[in] x    IDE CLK divider. IDE frequency will be:
                    - IDE_CLK1 = (Clock from PLL_CLKSEL_IDE_CLKSRC) / (x + 1)
                    - IDE_CLK2 = (Clock from PLL_CLKSEL_IDE_CLKSRC) / (2 * (x + 1))

    @note This if for pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV).
*/
//@{
#define PLL_IDE_CLKDIV(x)               (x << (PLL_CLKSEL_IDE_CLKDIV - PLL_CLKSEL_R2_OFFSET))   ///< IDE CLK DIV Macro
//@}

/**
    @name   Audio internal clock mux value

    Audio internal clock mux value

    @note This is for pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX).
*/
//@{
#define PLL_CLKSEL_ADO_CLKMUX_24P576    (0x00 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 24.576 MHz
#define PLL_CLKSEL_ADO_CLKMUX_12P288    (0x01 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 12.288 MHz
#define PLL_CLKSEL_ADO_CLKMUX_8P192     (0x02 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 8.192 MHz
#define PLL_CLKSEL_ADO_CLKMUX_6P144     (0x03 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 6.144 MHz
#define PLL_CLKSEL_ADO_CLKMUX_4P096     (0x04 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 4.096 MHz
#define PLL_CLKSEL_ADO_CLKMUX_3P072     (0x05 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 3.072 MHz
#define PLL_CLKSEL_ADO_CLKMUX_2P048     (0x06 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 2.048 MHz
#define PLL_CLKSEL_ADO_CLKMUX_11P2896   (0x07 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 11.2896 MHz
#define PLL_CLKSEL_ADO_CLKMUX_5P6448    (0x08 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 5.6448 MHz
#define PLL_CLKSEL_ADO_CLKMUX_2P8224    (0x09 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R2_OFFSET))    ///< Audio internal clock = 2.8224 MHz
//@}

/*
    @name   I2C clock divider

    I2C clock divider (Backward compatible)

    @note This if for pll_setClockRate(PLL_CLKSEL_I2C_CLKDIV).
*/
//@{
 #define PLL_I2C_CLKDIV(x)               (x)
//@}

/*
    @name   DAI clock divider

    DAI clock divider

    @note This if for pll_setClockRate(PLL_CLKSEL_DAI_CLKDIV).
*/
//@{
//#define PLL_DAI_CLKDIV(x)               (x << (PLL_CLKSEL_DAI_CLKDIV - PLL_CLKSEL_R6_OFFSET))
//@}

/*
    @name   Audio internal clock divider

    Audio internal clock divider (Backward compatible)

    This if for pll_setClockRate(PLL_CLKSEL_ADO_INT_CLKDIV).
*/
//@{
#define PLL_ADO_CLKDIV(x)               (x << (PLL_CLKSEL_ADO_INT_CLKDIV - PLL_CLKSEL_ADO_OFFSET))
//@}

/*
    @name   Audio Internal clock method value

    Audio Internal clock method value (Backward compatible)

    @brief This is for pll_setClockRate(PLL_CLKSEL_ADO_INTMETHOD).
*/
//@{
#define PLL_CLKSEL_ADO_INTMETHOD_MUX    (0x00)
#define PLL_CLKSEL_ADO_INTMETHOD_DIV    (0x01)
//@}


/*
    @name Audio Bit-Clock Ratio

    Audio Bit-Clock Ratio (Backward compatible)

    @brief This is for pll_setClockRate(PLL_CLKSEL_ADO_BCLKRATIO).
*/
//@{
#define PLL_CLKSEL_ADO_BCLKRATIO_4      (0x0 << (PLL_CLKSEL_ADO_BCLKRATIO - PLL_CLKSEL_ADO_OFFSET))
#define PLL_CLKSEL_ADO_BCLKRATIO_8      (0x1 << (PLL_CLKSEL_ADO_BCLKRATIO - PLL_CLKSEL_ADO_OFFSET))
#define PLL_CLKSEL_ADO_BCLKRATIO_12     (0x2 << (PLL_CLKSEL_ADO_BCLKRATIO - PLL_CLKSEL_ADO_OFFSET))
#define PLL_CLKSEL_ADO_BCLKRATIO_16     (0x3 << (PLL_CLKSEL_ADO_BCLKRATIO - PLL_CLKSEL_ADO_OFFSET))
#define PLL_CLKSEL_ADO_BCLKRATIO_24     (0x4 << (PLL_CLKSEL_ADO_BCLKRATIO - PLL_CLKSEL_ADO_OFFSET))
#define PLL_CLKSEL_ADO_BCLKRATIO_32     (0x5 << (PLL_CLKSEL_ADO_BCLKRATIO - PLL_CLKSEL_ADO_OFFSET))
//@}

/*
    @name   PRE clock rate value

    PRE clock rate value
    (Backward compatible)

    @note This is for pll_setClockRate(PLL_CLKSEL_PRE).
*/
//@{
#define PLL_CLKSEL_PRE_96         (0)
#define PLL_CLKSEL_PRE_80         (0)
#define PLL_CLKSEL_PRE_60         (0)
#define PLL_CLKSEL_PRE_40         (0)
//#define PLL_CLKSEL_PRE_96         (0x03 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_PRE_80         (0x02 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_PRE_60         (0x01 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_PRE_40         (0x00 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R3_OFFSET))
//@}

/**
    @name   SIE pixel clock source value

    SIE pixel clock source value

    @note This is for pll_setClockRate(PLL_CLKSEL_SIE_PXCLKSRC).
*/
//@{
#define PLL_CLKSEL_SIE_PXCLKSRC_PXCLK   (0x00 << (PLL_CLKSEL_SIE_PXCLKSRC - PLL_CLKSEL_R3_OFFSET))  ///< SIE PXCLK from PXCLK PIN
#define PLL_CLKSEL_SIE_PXCLKSRC_SIEMCLK (0x01 << (PLL_CLKSEL_SIE_PXCLKSRC - PLL_CLKSEL_R3_OFFSET))  ///< SIE PXCLK from SIE MCLK
//@}
//Backward compatible
#define PLL_CLKSEL_PRE_CLKSRC_PRE   PLL_CLKSEL_SIE_PXCLKSRC_SIEMCLK
#define PLL_CLKSEL_PRE_CLKSRC_PXCLK PLL_CLKSEL_SIE_PXCLKSRC_PXCLK


/*
    @name   FDE clock rate value

    FDE clock rate value
    (Backward compatible)

    @note This is for pll_setClockRate(PLL_CLKSEL_FDE).
*/
//@{
#define PLL_CLKSEL_FDE_80         (0)
#define PLL_CLKSEL_FDE_60         (0)
#define PLL_CLKSEL_FDE_40         (0)
#define PLL_CLKSEL_FDE_30         (0)
//#define PLL_CLKSEL_FDE_80         (0x03 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_FDE_60         (0x02 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_FDE_40         (0x01 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_FDE_30         (0x00 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R3_OFFSET))
//@}

/**
    @name   IFE clock rate value

    IFE clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_IFE).
*/
//@{
#define PLL_CLKSEL_IFE_120        (0x00 << (PLL_CLKSEL_IFE - PLL_CLKSEL_R3_OFFSET)) ///< IFE clock rate = 120 MHz
#define PLL_CLKSEL_IFE_96         (0x01 << (PLL_CLKSEL_IFE - PLL_CLKSEL_R3_OFFSET)) ///< IFE clock rate = 96 MHz
#define PLL_CLKSEL_IFE_80         (0x02 << (PLL_CLKSEL_IFE - PLL_CLKSEL_R3_OFFSET)) ///< IFE clock rate = 80 MHz
#define PLL_CLKSEL_IFE_60         (0x03 << (PLL_CLKSEL_IFE - PLL_CLKSEL_R3_OFFSET)) ///< IFE clock rate = 60 MHz
//@}


/*
    @name   SDIO2 clock rate value

    SDIO2 clock rate value
    (Backward compatible)

    @note This is for pll_setClockRate(PLL_CLKSEL_SDIO2).
*/
//@{
#define PLL_CLKSEL_SDIO2_SSPLL    (0)
#define PLL_CLKSEL_SDIO2_SSPLL_D2 (1)
#define PLL_CLKSEL_SDIO2_SSPLL_D3 (2)
#define PLL_CLKSEL_SDIO2_SSPLL_D4 (3)
#define PLL_CLKSEL_SDIO2_80       (4)
#define PLL_CLKSEL_SDIO2_60       (5)
#define PLL_CLKSEL_SDIO2_48       (6)
#define PLL_CLKSEL_SDIO2_24       (7)
//#define PLL_CLKSEL_SDIO2_SSPLL    (0x00 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_SDIO2_SSPLL_D2 (0x01 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_SDIO2_SSPLL_D3 (0x02 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_SDIO2_SSPLL_D4 (0x03 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_SDIO2_80       (0x04 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_SDIO2_60       (0x05 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_SDIO2_48       (0x06 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_SDIO2_24       (0x07 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))
//@}

/*
    @name   DCE clock rate value

    DCE clock rate value
    (Backward compatible)

    @note This is for pll_setClockRate(PLL_CLKSEL_DCE).
*/
//@{
#define PLL_CLKSEL_DCE_160      (0)
#define PLL_CLKSEL_DCE_120      (1)
#define PLL_CLKSEL_DCE_96       (2)
#define PLL_CLKSEL_DCE_60       (3)
//#define PLL_CLKSEL_DCE_160      (0x03 << (PLL_CLKSEL_DCE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_DCE_120      (0x00 << (PLL_CLKSEL_DCE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_DCE_96       (0x01 << (PLL_CLKSEL_DCE - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_DCE_60       (0x02 << (PLL_CLKSEL_DCE - PLL_CLKSEL_R3_OFFSET))
//@}

/*
    @name   DIS clock rate value

    DIS clock rate value
    (Backward compatible)

    @note This is for pll_setClockRate(PLL_CLKSEL_DIS).
*/
//@{
#define PLL_CLKSEL_DIS_80       (0)
#define PLL_CLKSEL_DIS_60       (0)
#define PLL_CLKSEL_DIS_48       (0)
#define PLL_CLKSEL_DIS_40       (0)
//#define PLL_CLKSEL_DIS_80       (0x03 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_DIS_60       (0x02 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_DIS_48       (0x01 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R3_OFFSET))
//#define PLL_CLKSEL_DIS_40       (0x00 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R3_OFFSET))
//@}

/**
    @name   Graphic clock rate value

    Graphic clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_GRAPHIC).
*/
//@{
#define PLL_CLKSEL_GRAPHIC_40   (0x00 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R3_OFFSET))   ///< Graphic clock rate = 40 MHz
#define PLL_CLKSEL_GRAPHIC_80   (0x01 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R3_OFFSET))   ///< Graphic clock rate = 80 MHz
#define PLL_CLKSEL_GRAPHIC_120  (0x02 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R3_OFFSET))   ///< Graphic clock rate = 120 MHz
#define PLL_CLKSEL_GRAPHIC_160  (0x03 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R3_OFFSET))   ///< Graphic clock rate = 160 MHz
//@}

/**
    @name   SDIO clock rate value

    SDIO clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_SDIO).
*/
//@{
#define PLL_CLKSEL_SDIO_40          (0x00 << (PLL_CLKSEL_SDIO - PLL_CLKSEL_R3_OFFSET))   ///< SDIO clock rate = 40 MHz
#define PLL_CLKSEL_SDIO_48          (0x01 << (PLL_CLKSEL_SDIO - PLL_CLKSEL_R3_OFFSET))   ///< SDIO clock rate = 48 MHz
#define PLL_CLKSEL_SDIO_60          (0x02 << (PLL_CLKSEL_SDIO - PLL_CLKSEL_R3_OFFSET))   ///< SDIO clock rate = 60 MHz
#define PLL_CLKSEL_SDIO_PLL2_D12    (0x03 << (PLL_CLKSEL_SDIO - PLL_CLKSEL_R3_OFFSET))   ///< SDIO clock rate = PLL2/12 MHz
//@}

/**
    @name   NAND clock rate value

    NAND clock rate value

    @note This is for pll_setClockRate(PLL_CLKSEL_NAND).
*/
//@{
#define PLL_CLKSEL_NAND_40          (0x00 << (PLL_CLKSEL_NAND - PLL_CLKSEL_R3_OFFSET))   ///< NAND clock rate = 40 MHz
#define PLL_CLKSEL_NAND_48          (0x01 << (PLL_CLKSEL_NAND - PLL_CLKSEL_R3_OFFSET))   ///< NAND clock rate = 48 MHz
#define PLL_CLKSEL_NAND_60          (0x02 << (PLL_CLKSEL_NAND - PLL_CLKSEL_R3_OFFSET))   ///< NAND clock rate = 60 MHz
#define PLL_CLKSEL_NAND_PLL2_D12    (0x03 << (PLL_CLKSEL_NAND - PLL_CLKSEL_R3_OFFSET))   ///< NAND clock rate = PLL2/12 MHz
//@}
//Backward compatible
#define PLL_CLKSEL_NAND_20          (0x04 << (PLL_CLKSEL_NAND - PLL_CLKSEL_R3_OFFSET))

/*
    @name   MS clock rate value

    MS clock rate value
    (Backward compatible)

    @note This is for pll_setClockRate(PLL_CLKSEL_MS).
*/
//@{
#define PLL_CLKSEL_MS_40            (0x00 << (PLL_CLKSEL_MS - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_MS_48            (0x01 << (PLL_CLKSEL_MS - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_MS_60            (0x02 << (PLL_CLKSEL_MS - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_MS_PLL2_D12      (0x03 << (PLL_CLKSEL_MS - PLL_CLKSEL_R3_OFFSET))
//Backward compatible
#define PLL_CLKSEL_MS_20            (0x04 << (PLL_CLKSEL_MS - PLL_CLKSEL_R3_OFFSET))
//@}

/**
    @name   IDE clock source value

    IDE clock source value

    @note This is for pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC).
*/
//@{
#define PLL_CLKSEL_IDE_CLKSRC_PLL1  (0x00 << (PLL_CLKSEL_IDE_CLKSRC - PLL_CLKSEL_R4_OFFSET))    ///< IDE clock from PLL1
#define PLL_CLKSEL_IDE_CLKSRC_PLL2  (0x01 << (PLL_CLKSEL_IDE_CLKSRC - PLL_CLKSEL_R4_OFFSET))    ///< IDE clock from PLL2
//@}
//Backward compatible
#define PLL_CLKSEL_IDE_CLKSRC_240   PLL_CLKSEL_IDE_CLKSRC_PLL1
//#define PLL_CLKSEL_IDE_CLKSRC_SSPLL (0x02 << (PLL_CLKSEL_IDE_CLKSRC - PLL_CLKSEL_R4_OFFSET))

/*
    @name   LVDS clock phase

    LVDS clock phase

    This if for pll_setClockRate(PLL_CLKSEL_LVDS_CLKPHASE)
*/
//@{
//#define PLL_LVDS_CLKPHASE_NOINV     (0x00 << (PLL_CLKSEL_LVDS_CLKPHASE - PLL_CLKSEL_R4_OFFSET))
//#define PLL_LVDS_CLKPHASE_INV       (0x01 << (PLL_CLKSEL_LVDS_CLKPHASE - PLL_CLKSEL_R4_OFFSET))
//@}


/**
    @name   MI clock divider

    MI clock divider

    @param[in] x    MI clock divider. MI frequency will be 240MHz/(x + 1). (3 <= x <= 255)

    @note This if for pll_setClockRate(PLL_CLKSEL_MI_CLKDIV)
*/
//@{
#define PLL_MI_CLKDIV(x)            (x << (PLL_CLKSEL_MI_CLKDIV - PLL_CLKSEL_R4_OFFSET))    ///< MI CLK DIV Macro
//@}

/**
    @name   SIE pixel clock divider

    SIE pixel clock divider

    @param[in] x    SIE PXCLK divider. SIE PXCLK frequency will be (Clock from PLL_CLKSEL_SIE_PXCLKSRC) / (x + 1)

    @note This if for pll_setClockRate(PLL_CLKSEL_SIE_PXCLKDIV)
*/
//@{
#define PLL_SIE_PXCLKDIV(x)         (x << (PLL_CLKSEL_SIE_PXCLKDIV - PLL_CLKSEL_R4_OFFSET)) ///< SIE PXCLK DIV Macro
//@}

/**
    @name   SPI clock divider

    SPI clock divider

    @param[in] x    SPI clock divider. SPI frequency will be 480MHz/(x + 1). (4 <= x <= 255)

    @note This if for pll_setClockRate(PLL_CLKSEL_SPI_CLKDIV)
*/
//@{
#define PLL_SPI_CLKDIV(x)           (x << (PLL_CLKSEL_SPI_CLKDIV - PLL_CLKSEL_R5_OFFSET))   ///< SPI CLK DIV Macro
//@}

/**
    @name   SDIO clock divider

    SDIO clock divider

    @param[in] x    SDIO clock divider. SDIO frequency will be (Clock from PLL_CLKSEL_SDIO)/(x + 1)

    @note This if for pll_setClockRate(PLL_CLKSEL_SDIO_CLKDIV)
*/
//@{
#define PLL_SDIO_CLKDIV(x)           (x << (PLL_CLKSEL_SDIO_CLKDIV - PLL_CLKSEL_R5_OFFSET)) ///< SDIO CLK DIV Macro
//@}

/*
    @name   SDIO2 clock divider

    SDIO2 clock divider
    (Backward compatible)

    @note This if for pll_setClockRate(PLL_CLKSEL_SDIO2_CLKDIV)
*/
//@{
#define PLL_SDIO2_CLKDIV(x)           (x)
//#define PLL_SDIO2_CLKDIV(x)           (x << (PLL_CLKSEL_SDIO2_CLKDIV - PLL_CLKSEL_R5_OFFSET))
//@}

/**
    Clock Enable ID

    Clock Enable ID

    @note This is for pll_enableClock() and pll_disableClock().
*/
typedef enum
{
    BMC_CLK,                ///< BMC clock enable
    SIE_MCLK,               ///< SIE MCLK enable
    SIE_PXCLK,              ///< SIE PXCLK enable
    IPE_CLK,                ///< IPE clock enable
    IME_CLK,                ///< IME clock enable
    IDE1_CLK,               ///< IDE1 clock enable
    IDE2_CLK,               ///< IDE2 clock enable
    JPG_CLK,                ///< JPEG clock enable
    USB_HCLK = 9,           ///< USB HCLK enable
    DMA_CLK,                ///< DMA clock enable
//    PRE_CLK = 16,
    IFE_CLK = 18,           ///< IFE clock enable
    SPI_CLK,                ///< SPI clock enable
    MI_CLK = 22,            ///< MI clock enable
    HDMI_CLK,               ///< HDMI clock enable
    SCANTG_CLK,             ///< Scanner TG clock enable
    XD_CLK = 32 + 1,        ///< NAND/xD clock enable
    SDIO_CLK,               ///< SDIO clock enable
    I2C_CLK = 32 + 6,       ///< I2C clock enable
    SIF_CLK,                ///< SIF clock enable
    UART2_CLK,              ///< UART2 clock enable
    UART_CLK,               ///< UART clock enable
//    TMR_PCLK,
    TMR_CLK = 32 + 11,      ///< Timer clock enable
    WDT_CLK = 32 + 16,      ///< Watchdog clock enable
//    RTC_CLK,
    DAI_CLK = 32 + 18,      ///< DAI clock enable
    ADC_CLK,                ///< ADC clock enable
    ADO_ADC_CLK,            ///< EAC AD clock enable
    ADO_DAC_CLK,            ///< EAC DA clock enable
    PWM_CLK,                ///< PWM CCNT clock enable
    PWM0_CLK,               ///< PWM0 clock enable
    PWM1_CLK,               ///< PWM1 clock enable
    PWM2_CLK,               ///< PWM2 clock enable
    GRAPH_CLK = 32 + 30,    ///< Graphic clock enable

    ENUM_DUMMY4WORD(CG_EN)
} CG_EN;

//Backward Compatible
#define BOOT_HCLK   BMC_CLK
#define ADO_SCLK    DAI_CLK
#define TMR_PCLK    TMR_CLK
#define TMR0_CLK    (E_CG_EN)
#define TMR1_CLK    (E_CG_EN)
#define TMR2_CLK    (E_CG_EN)
#define TMR3_CLK    (E_CG_EN)
#define TMR4_CLK    (E_CG_EN)
#define TMR5_CLK    (E_CG_EN)
#define TMR6_CLK    (E_CG_EN)
#define TMR7_CLK    (E_CG_EN)
#define PWM3_CLK    PWM2_CLK
#define PWM4_CLK    PWM2_CLK
#define PWM5_CLK    PWM2_CLK
#define PWM6_CLK    PWM2_CLK
#define PWM7_CLK    PWM2_CLK
#define PWM8_CLK    PWM2_CLK
#define PWM9_CLK    PWM2_CLK
#define PWM10_CLK   PWM2_CLK
#define PWM11_CLK   PWM2_CLK
#define SDIO2_CLK   SDIO_CLK
#define DCE_CLK     (E_CG_EN)
#define DIS_CLK     (E_CG_EN)
#define FDE_CLK     (E_CG_EN)
#define PRE_CLK     (E_CG_EN)
#define RTC_CLK     (E_CG_EN)

/**
    System Reset ID

    System Reset ID

    @note This is for pll_enableSystemReset() and pll_disableSystemReset().
*/
typedef enum
{
    PLL_RSTN_DRAMC,             ///< DMA async reset
//    FDMA_RSTN,
//    BMC_RSTN = 2,
    BRG_RSTN = 3,               ///< AMBA async reset
    SIE_RSTN,                   ///< SIE async reset
    IPE_RSTN,                   ///< IPE async reset
    IME_RSTN,                   ///< IME async reset
    IDE_RSTN,                   ///< IDE async reset
    JPG_RSTN,                   ///< JPEG async reset
    USB_RSTN = 10,              ///< USB async reset
    XD_RSTN = 12,               ///< NAND/xD async reset
    SDIO_RSTN,                  ///< SDIO async reset
    GRAPH_RSTN = 15,            ///< Graphic async reset
    I2C_RSTN,                   ///< I2C async reset
    SIF_RSTN,                   ///< SIF async reset
    UART_RSTN,                  ///< UART async reset
    TMR_RSTN,                   ///< Timer async reset
    WDT_RSTN,                   ///< Watchdog async reset
    RTC_RSTN,                   ///< RTC async reset
    ADO_RSTN = 23,              ///< DAI async reset
    GPIO_RSTN,                  ///< GPIO async reset
    INTC_RSTN,                  ///< Interrupt Controller async reset
    PWM_RSTN,                   ///< PWM async reset
    TOP_RSTN = 28,              ///< TOP/PINMUX async reset
    ADC_DRSTN,                  ///< ADC digital circuit async reset
    ADC_ARSTN,                  ///< ADC analog circuit async reset
    ADO_CODEC_D_RSTN,           ///< DAC digital circuit async reset
    SPI_RSTN = 32,              ///< SPI async reset
    UART2_RSTN = 32 + 2,        ///< UART2 async reset
    IFE_RSTN,                   ///< IFE async reset
    PAD_RSTN = 32 + 7,          ///< PAD async reset
    MI_RSTN,                    ///< MI async reset
    HDMI_RSTN = 32 + 11,        ///< HDMI async reset
    SCANTG_RSTN = 32 + 12,      ///< Scanner TG async reset

    ENUM_DUMMY4WORD(CG_RSTN)
} CG_RSTN;

// Backward compatible
#define DCE_RSTN    (E_CG_RSTN)
#define SDIO2_RSTN  (E_CG_RSTN)
#define FDE_RSTN    (E_CG_RSTN)
#define DIS_RSTN    (E_CG_RSTN)
#define PRE_RSTN    (E_CG_RSTN)

/**
    Clock Gating Select ID

    @note This is for pll_setClkAutoGating() / pll_clearClkAutoGating() /
    pll_getClkAutoGating() / pll_setPclkAutoGating() /
    pll_clearPclkAutoGating() / pll_getPclkAutoGating().
*/
typedef enum
{
    SIE_GCLK,                   ///< SIE clock gating
    IPE_GCLK = 2,               ///< IPE clock gating
    IME_GCLK,                   ///< IME clock gating
    IFE_GCLK = 7,               ///< IFE clock gating
    GRA_GCLK,                   ///< Graphic clock gating
    IDE_GCLK,                   ///< IDE clock gating
    MI_GCLK,                    ///< MI clock gating
    JPG_GCLK,                   ///< JPEG clock gating
    SDIO_GCLK,                  ///< SDIO clock gating
    XD_GCLK = 14,               ///< NAND/xD clock gating

    ADM_GCLK = 17,              ///< ADC clock gating
    SIF_GCLK,                   ///< SIF clock gating
    SPI_GCLK,                   ///< SPI clock gating
    I2C_GCLK,                   ///< I2C clock gating
    UART_GCLK,                  ///< UART clock gating
    UART2_GCLK,                 ///< UART2 clock gating

    ADO_GCLK = 24,              ///< EAC clock gating
    DAI_GCLK,                   ///< DAI clock gating
    WDT_GCLK,                   ///< Watchdog clock gating
    HDMI_GCLK = 28,             ///< HDMI clock gating
    SCANTG_GCLK,                ///< Scanner TG clock gating

    PWM0_GCLK = 32,             ///< PWM0 clock gating
    PWM1_GCLK,                  ///< PWM1 clock gating
    PWM2_GCLK,                  ///< PWM2 clock gating
    PWM_GCLK = 32 + 8,          ///< PWM CCNT clock gating

    //#NT#2011/08/30#HH Chuang -begin
    //#NT#Align timer gating to NT96220 proposal
    TMR_GCLK = 44,              ///< Timer clock gating
#if 0
    TMR0_GCLK = 44,             ///< Timer 0 clock gating
    TMR1_GCLK,                  ///< Timer 1 clock gating
    TMR2_GCLK,                  ///< Timer 2 clock gating
    TMR3_GCLK,                  ///< Timer 3 clock gating
    TMR4_GCLK,                  ///< Timer 4 clock gating
    TMR5_GCLK,                  ///< Timer 5 clock gating
    TMR6_GCLK,                  ///< Timer 6 clock gating
    TMR7_GCLK,                  ///< Timer 7 clock gating
#endif
    //#NT#2011/08/30#HH Chuang -end
    CLKGAT_MAXNUM,

    GPIO_GCLK = 52,             ///< GPIO clock gating
    INTC_GCLK,                  ///< Interrupt Controller clock gating
    RTC_GCLK,                   ///< RTC clock gating
    DDR_GCLK,                   ///< DDR clock gating
    PCLKGAT_MAXNUM,

    ENUM_DUMMY4WORD(GATECLK)
}GATECLK;

/**
    Clock frequency select ID

    @note This is for pll_setClockFreq().
*/
typedef enum
{
    SIEMCLK_FREQ,           ///< SIE MCLK freq Select ID
    IDECLK_FREQ,            ///< IDE CLK freq Select ID
    MICLK_FREQ,             ///< MI freq Select ID
    ADOCLK_FREQ,            ///< Audio CLK freq Select ID
    SIEPXCLK_FREQ,          ///< SIE PXCLK freq select ID

    PLL_CLKFREQ_MAXNUM,
    ENUM_DUMMY4WORD(PLL_CLKFREQ)
} PLL_CLKFREQ;

/**
    LDO select ID

    @note This is for pll_setLDO().
*/
typedef enum
{
    PLL_LDO_ID_1,           ///< LDO1
    PLL_LDO_ID_2,           ///< LDO2

    PLL_LDO_ID_MAX,
    ENUM_DUMMY4WORD(PLL_LDO_ID)
} PLL_LDO_ID;

/**
    LDO output voltage

    @note This is for pll_setLDO().
*/
typedef enum
{
    PLL_LDO_VOLTAGE_1_5V,   ///< Output 1.5V
    PLL_LDO_VOLTAGE_1_8V,   ///< Output 1.8V
    PLL_LDO_VOLTAGE_2_5V,   ///< Output 2.5V
    PLL_LDO_VOLTAGE_2_8V,   ///< Output 2.8V

    PLL_LDO_VOLTAGE_MAX,
    ENUM_DUMMY4WORD(PLL_LDO_VOLTAGE)
} PLL_LDO_VOLTAGE;

/**
    SSPLL mode

    @note This is for PLL_SSPLL_STRUCT.
*/
typedef enum
{
    PLL_SSPLL_MODE_LEGACY,          ///< Legacy mode
    PLL_SSPLL_MODE_FRACTIONAL,      ///< Fractional-N mode
    PLL_SSPLL_MODE_SREADSPECTRUM,   ///< Spread spectrum mode

    ENUM_DUMMY4WORD(PLL_SSPLL_MODE)
} PLL_SSPLL_MODE;

/**
    SSPLL charge pump current

    @note This is for PLL_SSPLL_STRUCT.
*/
typedef enum
{
    PLL_SSPLL_CHARGEPUMP_40UA,      ///< 40 uA
    PLL_SSPLL_CHARGEPUMP_60UA,      ///< 60 uA
    PLL_SSPLL_CHARGEPUMP_80UA,      ///< 80 uA
    PLL_SSPLL_CHARGEPUMP_100UA,     ///< 100 uA

    ENUM_DUMMY4WORD(PLL_SSPLL_CHARGEPUMP)
} PLL_SSPLL_CHARGEPUMP;

/**
    SSPLL structure

    @note This is for pll_setPLL2Sspll().
*/
typedef struct
{
    PLL_SSPLL_MODE mode;                ///< FRACTIONAL/SPREADSPECTRUM mode
    PLL_SSPLL_CHARGEPUMP chargePump;    ///< charge pump current
    UINT32  KVCO_BIAS;                  ///< Bias setting for KVCO curve (valid setting: 0x00 ~ 0x0F)
    UINT32  SS_I;                       ///< Initial up/down counter (valid setting: 0x000 ~ 0xFFF)
    UINT32  SS_U;                       ///< Upper bound setting (valid setting: 0x000 ~ 0xFFF)
    UINT32  SS_K;                       ///< Lower bound setting (valid setting: 0x000 ~ 0xFFF)
    BOOL    DIV16;                      ///< Spread spectrum range
} PLL_SSPLL_STRUCT;

extern void     pll_setPLL1(UINT32 uiLoopDiv, UINT32 uiPreDiv);
extern void     pll_getPLL1(UINT32 *puiLoopDiv, UINT32 *puiPreDiv);
extern void     pll_setPLL1En(BOOL bEnable);
extern UINT32   pll_getPLL1Freq(void);

extern void     pll_setPLL2(UINT32 uiLoopDiv, UINT32 uiPreDiv);
extern void     pll_getPLL2(UINT32 *puiLoopDiv, UINT32 *puiPreDiv);
extern void     pll_setPLL2En(BOOL bEnable);
extern UINT32   pll_getPLL2Freq(void);
extern void     pll_setPLL2Sspll(PLL_SSPLL_STRUCT* pSspllStruct);
extern void     pll_setPLL2SpreadSpectrum(UINT32 uiLowerFreq, UINT32 uiUpperFreq);
extern void     pll_setPLL2Fractional(UINT32 uiFreq);

extern void     pll_setCPUCCKR(UINT32 uiCCKR);
extern void     pll_setClockRate(PLL_CLKSEL uiNum, UINT32 uiValue);
extern UINT32   pll_getClockRate(PLL_CLKSEL uiNum);

extern void     pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv);
extern void     pll_setTMRClockRate(UINT32 uiTMRNumber, UINT32 uiDiv);

extern void     pll_enableClock(CG_EN Num);
extern void     pll_disableClock(CG_EN Num);
extern BOOL     pll_isClockEnabled(CG_EN Num);
extern ER       pll_setClockFreq(PLL_CLKFREQ ClkID, UINT32 uiFreq);

extern void     pll_enableSystemReset(CG_RSTN Num);
extern void     pll_disableSystemReset(CG_RSTN Num);

extern void     pll_setClkAutoGating(GATECLK ClkSel);
extern void     pll_clearClkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getClkAutoGating(GATECLK ClkSel);
extern void     pll_configClkAutoGating(UINT32 uiClkSel1, UINT32 uiClkSel2);


extern void     pll_setPclkAutoGating(GATECLK ClkSel);
extern void     pll_clearPclkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getPclkAutoGating(GATECLK ClkSel);
extern void     pll_configPclkAutoGating(UINT32 uiClkSel1, UINT32 uiClkSel2);

extern void     pll_setLDO(PLL_LDO_ID ldoID, BOOL bOPSEL, BOOL bISEL, PLL_LDO_VOLTAGE outVoltage);
extern void     pll_setLDOEn(PLL_LDO_ID ldoID, BOOL bEnable);
extern BOOL     pll_getLDOEn(PLL_LDO_ID ldoID);
//@}

#endif

