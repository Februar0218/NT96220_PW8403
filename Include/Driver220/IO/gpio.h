/**
    General Purpose I/O controller header file

    General Purpose I/O controller header file

    @file       gpio.h
    @ingroup    mIIOGPIO
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _GPIO_H
#define _GPIO_H

#include "Type.h"

/**
    @addtogroup mIIOGPIO
*/
//@{

/**
    @name   GPIO Direction

    GPIO direction

    GPIO direction definition for gpio_setDir () and gpio_getDir ()
*/
//@{
#define GPIO_DIR_INPUT              0       ///< GPIO is input direction
#define GPIO_DIR_OUTPUT             1       ///< GPIO is output direction
//@}

/**
    @name   GPIO Interrupt Type

    GPIO interrupt type

    GPIO interrupt type definition for type argument of gpio_setIntTypePol()
*/
//@{
#define GPIO_INTTYPE_EDGE           0       ///< GPIO interrupt is edge trigger
#define GPIO_INTTYPE_LEVEL          1       ///< GPIO interrupt is level trigger
//@}

/**
    @name   GPIO Interrupt Polarity

    GPIO interrupt polarity

    GPIO interrupt polarity definition for pol argument of gpio_setIntTypePol()
*/
//@{
#define GPIO_INTPOL_POSHIGH         0       ///< GPIO interrupt polarity is \n
                                            ///< - @b positvie edge for edge trigger
                                            ///< - @b high level for level trigger
#define GPIO_INTPOL_NEGLOW          1       ///< GPIO interrupt polarity is \n
                                            ///< - @b negative edge for edge trigger
                                            ///< - @b low level for level trigger
#define GPIO_INTPOL_BOTHEDGE        2       ///< GPIO interrupt polarity is \n
                                            ///< - @b both edge for edge trigger
//@}



/**
    @name   GPIO pins ID

    GPIO pins ID definition

    For detail GPIO pin out, please refer to NT96220 data sheet.
*/
//@{
/*Storage GPIO - CGPIO*/
#define C_GPIO_0    0                       ///< CGPIO[0]
#define C_GPIO_1    1                       ///< CGPIO[1]
#define C_GPIO_2    2                       ///< CGPIO[2]
#define C_GPIO_3    3                       ///< CGPIO[3]
#define C_GPIO_4    4                       ///< CGPIO[4]
#define C_GPIO_5    5                       ///< CGPIO[5]
#define C_GPIO_6    6                       ///< CGPIO[6]
#define C_GPIO_7    7                       ///< CGPIO[7]
#define C_GPIO_8    8                       ///< CGPIO[8]
#define C_GPIO_9    9                       ///< CGPIO[9]
#define C_GPIO_10   10                      ///< CGPIO[10]
#define C_GPIO_11   11                      ///< CGPIO[11]
#define C_GPIO_12   12                      ///< CGPIO[12]
#define C_GPIO_13   13                      ///< CGPIO[13]
#define C_GPIO_14   14                      ///< CGPIO[14]
#if 1
#define C_GPIO_15   32                      ///< backward compatible
#define C_GPIO_16   32                      ///< backward compatible
#define C_GPIO_17   32                      ///< backward compatible
#define C_GPIO_18   32                      ///< backward compatible
#define C_GPIO_19   32                      ///< backward compatible
#define C_GPIO_20   32                      ///< backward compatible
#define C_GPIO_21   32                      ///< backward compatible
#define C_GPIO_22   32                      ///< backward compatible
#define C_GPIO_23   32                      ///< backward compatible
#define C_GPIO_24   32                      ///< backward compatible
#define C_GPIO_25   32                      ///< backward compatible
#define C_GPIO_26   32                      ///< backward compatible
#define C_GPIO_27   32                      ///< backward compatible
#define C_GPIO_28   32                      ///< backward compatible
#define C_GPIO_29   32                      ///< backward compatible
#define C_GPIO_30   32                      ///< backward compatible
#define C_GPIO_31   32                      ///< backward compatible
#define C_GPIO_32   32                      ///< backward compatible
#define C_GPIO_33   32                      ///< backward compatible
#define C_GPIO_34   32                      ///< backward compatible
#define C_GPIO_35   32                      ///< backward compatible
#define C_GPIO_36   32                      ///< backward compatible
#define C_GPIO_37   32                      ///< backward compatible
#else
#define C_GPIO_15   15                      ///< CGPIO[15]
#define C_GPIO_16   16                      ///< CGPIO[16]
#define C_GPIO_17   17                      ///< CGPIO[17]
#define C_GPIO_18   18                      ///< CGPIO[18]
#define C_GPIO_19   19                      ///< CGPIO[19]
#define C_GPIO_20   20                      ///< CGPIO[20]
#define C_GPIO_21   21                      ///< CGPIO[21]
#define C_GPIO_22   22                      ///< CGPIO[22]
#define C_GPIO_23   23                      ///< CGPIO[23]
#define C_GPIO_24   24                      ///< CGPIO[24]
#define C_GPIO_25   25                      ///< CGPIO[25]
#define C_GPIO_26   26                      ///< CGPIO[26]
#define C_GPIO_27   27                      ///< CGPIO[27]
#define C_GPIO_28   28                      ///< CGPIO[28]
#define C_GPIO_29   29                      ///< CGPIO[29]
#define C_GPIO_30   30                      ///< CGPIO[30]
#define C_GPIO_31   31                      ///< CGPIO[31]
#define C_GPIO_32   32                      ///< CGPIO[32]
#define C_GPIO_33   33                      ///< CGPIO[33]
#define C_GPIO_34   34                      ///< CGPIO[34]
#define C_GPIO_35   35                      ///< CGPIO[35]
#define C_GPIO_36   36                      ///< CGPIO[36]
#define C_GPIO_37   37                      ///< CGPIO[37]
#endif

/*Peripheral GPIO - PGPIO*/
#define P_GPIO_SHIFT_BASE 64
#define P_GPIO_0    (0 +P_GPIO_SHIFT_BASE)  ///< PGPIO[0]
#define P_GPIO_1    (1 +P_GPIO_SHIFT_BASE)  ///< PGPIO[1]
#define P_GPIO_2    (2 +P_GPIO_SHIFT_BASE)  ///< PGPIO[2]
#define P_GPIO_3    (3 +P_GPIO_SHIFT_BASE)  ///< PGPIO[3]
#define P_GPIO_4    (4 +P_GPIO_SHIFT_BASE)  ///< PGPIO[4]
#define P_GPIO_5    (5 +P_GPIO_SHIFT_BASE)  ///< PGPIO[5]
#define P_GPIO_6    (6 +P_GPIO_SHIFT_BASE)  ///< PGPIO[6]
#define P_GPIO_7    (7 +P_GPIO_SHIFT_BASE)  ///< PGPIO[7]
#define P_GPIO_8    (8 +P_GPIO_SHIFT_BASE)  ///< PGPIO[8]
#define P_GPIO_9    (9 +P_GPIO_SHIFT_BASE)  ///< PGPIO[9]
#define P_GPIO_10   (10+P_GPIO_SHIFT_BASE)  ///< PGPIO[10]
#define P_GPIO_11   (11+P_GPIO_SHIFT_BASE)  ///< PGPIO[11]
#define P_GPIO_12   (12+P_GPIO_SHIFT_BASE)  ///< PGPIO[12]
#define P_GPIO_13   (13+P_GPIO_SHIFT_BASE)  ///< PGPIO[13]
#define P_GPIO_14   (14+P_GPIO_SHIFT_BASE)  ///< PGPIO[14]
#define P_GPIO_15   (15+P_GPIO_SHIFT_BASE)  ///< PGPIO[15]
#if 1
#define P_GPIO_16   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_17   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_18   (18+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_19   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_20   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_21   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_22   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_23   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#define P_GPIO_24   (31+P_GPIO_SHIFT_BASE)  ///< backward compatible
#else
#define P_GPIO_13   (13+P_GPIO_SHIFT_BASE)  ///< PGPIO[13]
#define P_GPIO_14   (14+P_GPIO_SHIFT_BASE)  ///< PGPIO[14]
#define P_GPIO_15   (15+P_GPIO_SHIFT_BASE)  ///< PGPIO[15]
#define P_GPIO_16   (16+P_GPIO_SHIFT_BASE)  ///< PGPIO[16]
#define P_GPIO_17   (17+P_GPIO_SHIFT_BASE)  ///< PGPIO[17]
#define P_GPIO_18   (18+P_GPIO_SHIFT_BASE)  ///< PGPIO[18]
#define P_GPIO_19   (19+P_GPIO_SHIFT_BASE)  ///< PGPIO[19]
#define P_GPIO_20   (20+P_GPIO_SHIFT_BASE)  ///< PGPIO[20]
#define P_GPIO_21   (21+P_GPIO_SHIFT_BASE)  ///< PGPIO[21]
#define P_GPIO_22   (22+P_GPIO_SHIFT_BASE)  ///< PGPIO[22]
#define P_GPIO_23   (23+P_GPIO_SHIFT_BASE)  ///< PGPIO[23]
#define P_GPIO_24   (24+P_GPIO_SHIFT_BASE)  ///< PGPIO[24]
#endif

/*Sensor GPIO - SGPIO*/
#define S_GPIO_SHIFT_BASE 128
#define S_GPIO_0    (0 +S_GPIO_SHIFT_BASE)  ///< SGPIO[0]
#define S_GPIO_1    (1 +S_GPIO_SHIFT_BASE)  ///< SGPIO[1]
#define S_GPIO_2    (2 +S_GPIO_SHIFT_BASE)  ///< SGPIO[2]
#define S_GPIO_3    (3 +S_GPIO_SHIFT_BASE)  ///< SGPIO[3]
#define S_GPIO_4    (4 +S_GPIO_SHIFT_BASE)  ///< SGPIO[4]
#define S_GPIO_5    (5 +S_GPIO_SHIFT_BASE)  ///< SGPIO[5]
#define S_GPIO_6    (6 +S_GPIO_SHIFT_BASE)  ///< SGPIO[6]
#define S_GPIO_7    (7 +S_GPIO_SHIFT_BASE)  ///< SGPIO[7]
#define S_GPIO_8    (8 +S_GPIO_SHIFT_BASE)  ///< SGPIO[8]
#define S_GPIO_9    (9 +S_GPIO_SHIFT_BASE)  ///< SGPIO[9]
#define S_GPIO_10   (10+S_GPIO_SHIFT_BASE)  ///< SGPIO[10]
#define S_GPIO_11   (11+S_GPIO_SHIFT_BASE)  ///< SGPIO[11]
#define S_GPIO_12   (12+S_GPIO_SHIFT_BASE)  ///< SGPIO[12]
#define S_GPIO_13   (13+S_GPIO_SHIFT_BASE)  ///< SGPIO[13]
#define S_GPIO_14   (14+S_GPIO_SHIFT_BASE)  ///< SGPIO[14]
#define S_GPIO_15   (15+S_GPIO_SHIFT_BASE)  ///< SGPIO[15]
#define S_GPIO_16   (16+S_GPIO_SHIFT_BASE)  ///< SGPIO[16]
#define S_GPIO_17   (17+S_GPIO_SHIFT_BASE)  ///< SGPIO[17]
#define S_GPIO_18   (18+S_GPIO_SHIFT_BASE)  ///< SGPIO[18]
#define S_GPIO_19   (19+S_GPIO_SHIFT_BASE)  ///< SGPIO[19]
#if 1
#define S_GPIO_20   (31+S_GPIO_SHIFT_BASE)  ///< backward compatible
#define S_GPIO_21   (31+S_GPIO_SHIFT_BASE)  ///< backward compatible
#define S_GPIO_22   (31+S_GPIO_SHIFT_BASE)  ///< backward compatible
#define S_GPIO_23   (31+S_GPIO_SHIFT_BASE)  ///< backward compatible
#define S_GPIO_24   (31+S_GPIO_SHIFT_BASE)  ///< backward compatible
#define S_GPIO_25   (31+S_GPIO_SHIFT_BASE)  ///< backward compatible
#define S_GPIO_26   (31+S_GPIO_SHIFT_BASE)  ///< backward compatible
#else
#define S_GPIO_20   (20+S_GPIO_SHIFT_BASE)  ///< SGPIO[20]
#define S_GPIO_21   (21+S_GPIO_SHIFT_BASE)  ///< SGPIO[21]
#define S_GPIO_22   (22+S_GPIO_SHIFT_BASE)  ///< SGPIO[22]
#define S_GPIO_23   (23+S_GPIO_SHIFT_BASE)  ///< SGPIO[23]
#define S_GPIO_24   (24+S_GPIO_SHIFT_BASE)  ///< SGPIO[24]
#define S_GPIO_25   (25+S_GPIO_SHIFT_BASE)  ///< SGPIO[25]
#define S_GPIO_26   (26+S_GPIO_SHIFT_BASE)  ///< SGPIO[26]
#endif

/*LCD GPIO - LGPIO*/
#define L_GPIO_SHIFT_BASE 160
#define L_GPIO_0    (0 +L_GPIO_SHIFT_BASE)  ///< LGPIO[0]
#define L_GPIO_1    (1 +L_GPIO_SHIFT_BASE)  ///< LGPIO[1]
#define L_GPIO_2    (2 +L_GPIO_SHIFT_BASE)  ///< LGPIO[2]
#define L_GPIO_3    (3 +L_GPIO_SHIFT_BASE)  ///< LGPIO[3]
#define L_GPIO_4    (4 +L_GPIO_SHIFT_BASE)  ///< LGPIO[4]
#define L_GPIO_5    (5 +L_GPIO_SHIFT_BASE)  ///< LGPIO[5]
#define L_GPIO_6    (6 +L_GPIO_SHIFT_BASE)  ///< LGPIO[6]
#define L_GPIO_7    (7 +L_GPIO_SHIFT_BASE)  ///< LGPIO[7]
#define L_GPIO_8    (8 +L_GPIO_SHIFT_BASE)  ///< LGPIO[8]
#define L_GPIO_9    (9 +L_GPIO_SHIFT_BASE)  ///< LGPIO[9]
#define L_GPIO_10   (10+L_GPIO_SHIFT_BASE)  ///< LGPIO[10]
#if 1
#define L_GPIO_11   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_12   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_13   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_14   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_15   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_16   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_17   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_18   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_19   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_20   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_21   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_22   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_23   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_24   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_25   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_26   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#define L_GPIO_27   (31+L_GPIO_SHIFT_BASE)  ///< backward compatible
#else
#define L_GPIO_11   (11+L_GPIO_SHIFT_BASE)  ///< LGPIO[11]
#define L_GPIO_12   (12+L_GPIO_SHIFT_BASE)  ///< LGPIO[12]
#define L_GPIO_13   (13+L_GPIO_SHIFT_BASE)  ///< LGPIO[13]
#define L_GPIO_14   (14+L_GPIO_SHIFT_BASE)  ///< LGPIO[14]
#define L_GPIO_15   (15+L_GPIO_SHIFT_BASE)  ///< LGPIO[15]
#define L_GPIO_16   (16+L_GPIO_SHIFT_BASE)  ///< LGPIO[16]
#define L_GPIO_17   (17+L_GPIO_SHIFT_BASE)  ///< LGPIO[17]
#define L_GPIO_18   (18+L_GPIO_SHIFT_BASE)  ///< LGPIO[18]
#define L_GPIO_19   (19+L_GPIO_SHIFT_BASE)  ///< LGPIO[19]
#define L_GPIO_20   (20+L_GPIO_SHIFT_BASE)  ///< LGPIO[20]
#define L_GPIO_21   (21+L_GPIO_SHIFT_BASE)  ///< LGPIO[21]
#define L_GPIO_22   (22+L_GPIO_SHIFT_BASE)  ///< LGPIO[22]
#define L_GPIO_23   (23+L_GPIO_SHIFT_BASE)  ///< LGPIO[23]
#define L_GPIO_24   (24+L_GPIO_SHIFT_BASE)  ///< LGPIO[24]
#define L_GPIO_25   (25+L_GPIO_SHIFT_BASE)  ///< LGPIO[25]
#define L_GPIO_26   (26+L_GPIO_SHIFT_BASE)  ///< LGPIO[26]
#define L_GPIO_27   (27+L_GPIO_SHIFT_BASE)  ///< LGPIO[27]
#endif

/*General GPIO - GGPIO*/
#define G_GPIO_SHIFT_BASE 192
#if 1
#define G_GPIO_0    (0 +G_GPIO_SHIFT_BASE)  ///< backward compatible
#define G_GPIO_1    (1 +G_GPIO_SHIFT_BASE)  ///< backward compatible
#define G_GPIO_2    (2 +G_GPIO_SHIFT_BASE)  ///< backward compatible
#define G_GPIO_3    (3 +G_GPIO_SHIFT_BASE)  ///< backward compatible
#else
#define G_GPIO_0    (0 +G_GPIO_SHIFT_BASE)  ///< GGPIO[0]
#define G_GPIO_1    (1 +G_GPIO_SHIFT_BASE)  ///< GGPIO[1]
#define G_GPIO_2    (2 +G_GPIO_SHIFT_BASE)  ///< GGPIO[2]
#define G_GPIO_3    (3 +G_GPIO_SHIFT_BASE)  ///< GGPIO[3]
#endif


/*Dedicated GPIO - DGPIO*/
// In order to backward comaptible, DGPIO is used as " GPIO_IS_DGPIO | D_GPIO_* "
#define D_GPIO_SHIFT_BASE 224
#define D_GPIO_0    (0 +D_GPIO_SHIFT_BASE)  ///< DGPIO[0]
#define D_GPIO_1    (1 +D_GPIO_SHIFT_BASE)  ///< DGPIO[1]
#define D_GPIO_2    (2 +D_GPIO_SHIFT_BASE)  ///< DGPIO[2]
#define D_GPIO_3    (3 +D_GPIO_SHIFT_BASE)  ///< DGPIO[3]
#define D_GPIO_4    (4 +D_GPIO_SHIFT_BASE)  ///< DGPIO[4]
#if 1
#define D_GPIO_5    (31 +D_GPIO_SHIFT_BASE) ///< backward compatible
#define D_GPIO_6    (31 +D_GPIO_SHIFT_BASE) ///< backward compatible
#define D_GPIO_7    (31 +D_GPIO_SHIFT_BASE) ///< backward compatible
#define D_GPIO_8    (31 +D_GPIO_SHIFT_BASE) ///< backward compatible
#else
#define D_GPIO_5    (5 +D_GPIO_SHIFT_BASE)  ///< DGPIO[5]
#define D_GPIO_6    (6 +D_GPIO_SHIFT_BASE)  ///< DGPIO[6]
#define D_GPIO_7    (7 +D_GPIO_SHIFT_BASE)  ///< DGPIO[7]
#define D_GPIO_8    (8 +D_GPIO_SHIFT_BASE)  ///< DGPIO[8]
#endif
#define D_GPIO_16   (16+D_GPIO_SHIFT_BASE)  ///< DGPIO[16], DR_A0
#define D_GPIO_17   (17+D_GPIO_SHIFT_BASE)  ///< DGPIO[17], DR_A1
#define D_GPIO_18   (18+D_GPIO_SHIFT_BASE)  ///< DGPIO[18], DR_A2
#define D_GPIO_19   (19+D_GPIO_SHIFT_BASE)  ///< DGPIO[19], DR_A3
#define D_GPIO_20   (20+D_GPIO_SHIFT_BASE)  ///< DGPIO[20], DR_A4
#define D_GPIO_21   (21+D_GPIO_SHIFT_BASE)  ///< DGPIO[21], DR_A5
#define D_GPIO_22   (22+D_GPIO_SHIFT_BASE)  ///< DGPIO[22], DR_A6
#define D_GPIO_23   (23+D_GPIO_SHIFT_BASE)  ///< DGPIO[23], DR_A7
#define D_GPIO_24   (24+D_GPIO_SHIFT_BASE)  ///< DGPIO[24], DR_A8
#define D_GPIO_25   (25+D_GPIO_SHIFT_BASE)  ///< DGPIO[25], DR_A9
#define D_GPIO_26   (26+D_GPIO_SHIFT_BASE)  ///< DGPIO[26], DR_A10
#define D_GPIO_27   (27+D_GPIO_SHIFT_BASE)  ///< DGPIO[27], DR_A11
//@}

/**
    @name   GPIO Interrupt ID

    GPIO interrupt ID definition

    GPIO interrupt ID for interrupt related APIs.
*/
//@{
#define GPIO_INT_00                 0       ///< GPIO INT[0]: CGPIO[5]
#define GPIO_INT_01                 1       ///< GPIO INT[1]: CGPIO[6]
#define GPIO_INT_02                 2       ///< GPIO INT[2]: CGPIO[7]
#define GPIO_INT_03                 3       ///< GPIO INT[3]: CGPIO[9]
#define GPIO_INT_04                 4       ///< GPIO INT[4]: CGPIO[10]
#define GPIO_INT_05                 5       ///< GPIO INT[5]: CGPIO[11]
#define GPIO_INT_06                 6       ///< GPIO INT[6]: CGPIO[12]
#define GPIO_INT_07                 7       ///< GPIO INT[7]: CGPIO[13]
#define GPIO_INT_08                 8       ///< GPIO INT[8]: SGPIO[0]
#define GPIO_INT_09                 9       ///< GPIO INT[9]: SGPIO[1]
#define GPIO_INT_10                 10      ///< GPIO INT[10]: SGPIO[13]
#define GPIO_INT_11                 11      ///< GPIO INT[11]: SGPIO[14]
#define GPIO_INT_12                 12      ///< GPIO INT[12]: SGPIO[17]
#define GPIO_INT_13                 13      ///< GPIO INT[13]: SGPIO[18]
#define GPIO_INT_14                 14      ///< GPIO INT[14]: SGPIO[19]
#define GPIO_INT_15                 15      ///< GPIO INT[15]: PGPIO[3]
#define GPIO_INT_16                 16      ///< GPIO INT[16]: PGPIO[4]
#define GPIO_INT_17                 17      ///< GPIO INT[17]: PGPIO[5]
#define GPIO_INT_18                 18      ///< GPIO INT[18]: PGPIO[6]
#define GPIO_INT_19                 19      ///< GPIO INT[19]: USB wakeup
#define GPIO_INT_20                 20      ///< GPIO INT[20]: USB VBUS
#define GPIO_INT_21                 21      ///< GPIO INT[21]: PGPIO[13]
#define GPIO_INT_22                 22      ///< GPIO INT[22]: PGPIO[14]
#define GPIO_INT_23                 23      ///< GPIO INT[23]: PGPIO[15]
#define GPIO_INT_24                 24      ///< backward compatible
#define GPIO_INT_25                 25      ///< backward compatible
#define GPIO_INT_26                 26      ///< backward compatible
#define GPIO_INT_27                 27      ///< backward compatible
#define GPIO_INT_28                 28      ///< backward compatible
#define GPIO_INT_29                 29      ///< backward compatible
#define GPIO_INT_30                 30      ///< backward compatible
#define GPIO_INT_31                 31      ///< backward compatible

#define GPIO_INT_32                 32      ///< DGPIO INT[0]: DGPIO[0]
#define GPIO_INT_33                 33      ///< DGPIO INT[1]: DGPIO[1]
#define GPIO_INT_34                 34      ///< DGPIO INT[2]: DGPIO[2]
#define GPIO_INT_35                 35      ///< DGPIO INT[3]: DGPIO[3]
#define GPIO_INT_36                 36      ///< DGPIO INT[4]: DGPIO[4]
#define GPIO_INT_37                 37      ///< backward compatible
#define GPIO_INT_38                 38      ///< backward compatible
#define GPIO_INT_39                 39      ///< backward compatible
#define GPIO_INT_40                 40      ///< backward compatible

#define GPIO_INT_48                 48      ///< DGPIO INT[16]: DRAM_KEY_A0
#define GPIO_INT_49                 49      ///< DGPIO INT[17]: DRAM_KEY_A1
#define GPIO_INT_50                 50      ///< DGPIO INT[18]: DRAM_KEY_A2
#define GPIO_INT_51                 51      ///< DGPIO INT[19]: DRAM_KEY_A3
#define GPIO_INT_52                 52      ///< DGPIO INT[20]: DRAM_KEY_A4
#define GPIO_INT_53                 53      ///< DGPIO INT[21]: DRAM_KEY_A5
#define GPIO_INT_54                 54      ///< DGPIO INT[22]: DRAM_KEY_A6
#define GPIO_INT_55                 55      ///< DGPIO INT[23]: DRAM_KEY_A7
#define GPIO_INT_56                 56      ///< DGPIO INT[24]: DRAM_KEY_A8
#define GPIO_INT_57                 57      ///< DGPIO INT[25]: DRAM_KEY_A9
#define GPIO_INT_58                 58      ///< DGPIO INT[26]: DRAM_KEY_A10
#define GPIO_INT_59                 59      ///< DGPIO INT[27]: DRAM_KEY_A11

#define GPIO_INT_USBWAKEUP              (GPIO_INT_19)   ///< GPIO INT[19]: USB wakeup
#define GPIO_INT_USBPLUGIN              (GPIO_INT_20)   ///< GPIO INT[20]: USB VBUS
#define GPIO_INT_REMOTE                 (GPIO_INT_31)   ///< backward compatible: not exist in NT96220
//@}

//#NT#2011/07/13#HH Chuang -begin
//#NT#Modify gpio_waitIntFlag() to wait single GPIO INT
#if 0
/**
    GPIO wait interrupt ID definition, Used for gpio_waitIntFlag(GPIO_INT_FLAG*)
*/
//@{
#define GPIO_INT_FLAG00                 (1ull << GPIO_INT_00)
#define GPIO_INT_FLAG01                 (1ull << GPIO_INT_01)
#define GPIO_INT_FLAG02                 (1ull << GPIO_INT_02)
#define GPIO_INT_FLAG03                 (1ull << GPIO_INT_03)
#define GPIO_INT_FLAG04                 (1ull << GPIO_INT_04)
#define GPIO_INT_FLAG05                 (1ull << GPIO_INT_05)
#define GPIO_INT_FLAG06                 (1ull << GPIO_INT_06)
#define GPIO_INT_FLAG07                 (1ull << GPIO_INT_07)
#define GPIO_INT_FLAG08                 (1ull << GPIO_INT_08)
#define GPIO_INT_FLAG09                 (1ull << GPIO_INT_09)
#define GPIO_INT_FLAG10                 (1ull << GPIO_INT_10)
#define GPIO_INT_FLAG11                 (1ull << GPIO_INT_11)
#define GPIO_INT_FLAG12                 (1ull << GPIO_INT_12)
#define GPIO_INT_FLAG13                 (1ull << GPIO_INT_13)
#define GPIO_INT_FLAG14                 (1ull << GPIO_INT_14)
#define GPIO_INT_FLAG15                 (1ull << GPIO_INT_15)
#define GPIO_INT_FLAG16                 (1ull << GPIO_INT_16)
#define GPIO_INT_FLAG17                 (1ull << GPIO_INT_17)
#define GPIO_INT_FLAG18                 (1ull << GPIO_INT_18)
#define GPIO_INT_FLAG19                 (1ull << GPIO_INT_19)
#define GPIO_INT_FLAG20                 (1ull << GPIO_INT_20)
#define GPIO_INT_FLAG21                 (1ull << GPIO_INT_21)
#define GPIO_INT_FLAG22                 (1ull << GPIO_INT_22)
#define GPIO_INT_FLAG23                 (1ull << GPIO_INT_23)
#define GPIO_INT_FLAG24                 (1ull << GPIO_INT_24)
#define GPIO_INT_FLAG25                 (1ull << GPIO_INT_25)
#define GPIO_INT_FLAG26                 (1ull << GPIO_INT_26)
#define GPIO_INT_FLAG27                 (1ull << GPIO_INT_27)
#define GPIO_INT_FLAG28                 (1ull << GPIO_INT_28)
#define GPIO_INT_FLAG29                 (1ull << GPIO_INT_29)
#define GPIO_INT_FLAG30                 (1ull << GPIO_INT_30)
#define GPIO_INT_FLAG31                 (1ull << GPIO_INT_31)

#define GPIO_INT_FLAG32                 (1ull << GPIO_INT_32)
#define GPIO_INT_FLAG33                 (1ull << GPIO_INT_33)
#define GPIO_INT_FLAG34                 (1ull << GPIO_INT_34)
#define GPIO_INT_FLAG35                 (1ull << GPIO_INT_35)
#define GPIO_INT_FLAG36                 (1ull << GPIO_INT_36)
#define GPIO_INT_FLAG37                 (1ull << GPIO_INT_37)
#define GPIO_INT_FLAG38                 (1ull << GPIO_INT_38)
#define GPIO_INT_FLAG39                 (1ull << GPIO_INT_39)
#define GPIO_INT_FLAG40                 (1ull << GPIO_INT_40)
#define GPIO_INT_FLAG48                 (1ull << GPIO_INT_48)
#define GPIO_INT_FLAG49                 (1ull << GPIO_INT_49)
#define GPIO_INT_FLAG50                 (1ull << GPIO_INT_50)
#define GPIO_INT_FLAG51                 (1ull << GPIO_INT_51)
#define GPIO_INT_FLAG52                 (1ull << GPIO_INT_52)
#define GPIO_INT_FLAG53                 (1ull << GPIO_INT_53)
#define GPIO_INT_FLAG54                 (1ull << GPIO_INT_54)
#define GPIO_INT_FLAG55                 (1ull << GPIO_INT_55)
#define GPIO_INT_FLAG56                 (1ull << GPIO_INT_56)
#define GPIO_INT_FLAG57                 (1ull << GPIO_INT_57)
#define GPIO_INT_FLAG58                 (1ull << GPIO_INT_58)
#define GPIO_INT_FLAG59                 (1ull << GPIO_INT_59)
//@}
#endif
//#NT#2011/07/13#HH Chuang -end

//Total number of GPIO interrupts
#define GPIO_INT_NUM                (24)

// In order to backward comaptible, DGPIO is used as " GPIO_IS_DGPIO | D_GPIO_* " or "D_GPIO_*"
#define GPIO_IS_DGPIO               (224)


//
// The general api for the GPIO device driver
//
extern ER       gpio_open(void);
extern ER       gpio_close(void);

//#NT#2011/07/13#HH Chuang -begin
//#NT#Modify gpio_waitIntFlag() to wait single GPIO INT
extern ER       gpio_waitIntFlag(UINT32 gpioIntID);
//extern ER       gpio_waitIntFlag(FLGPTN WaitFlags);
//#NT#2011/07/13#HH Chuang -end

// ---- GPIO ----
extern void     gpio_setDir(UINT32 pin, UINT32 dir);
extern UINT32   gpio_getDir(UINT32 pin);
extern void     gpio_setPin(UINT32 pin);
extern void     gpio_clearPin(UINT32 pin);
extern UINT32   gpio_getPin(UINT32 pin);
extern void     gpio_pullSet(UINT32 pin);
extern void     gpio_pullClear(UINT32 pin);
extern UINT32   gpio_readData(UINT32 dataidx);
extern void     gpio_writeData(UINT32 dataidx, UINT32 value);

// ---- DGPIO ----
extern UINT32   dgpio_readData(void);
extern void     dgpio_writeData(UINT32 value);

// ---- Interrupt Control ----
extern void     gpio_enableInt(UINT32 ipin);
extern void     gpio_disableInt(UINT32 ipin);
extern UINT32   gpio_getIntEnable(UINT32 ipin);
extern void     gpio_clearIntStatus(UINT32 ipin);
extern UINT32   gpio_getIntStatus(UINT32 ipin);
extern void     gpio_setIntTypePol(UINT32 ipin, UINT32 type, UINT32 pol);
extern void     gpio_setIntIsr(UINT32 ipin, FP fpIsr);
extern BOOL     gpio_getIntIsrStatus(UINT32 ipin);
extern void     gpio_clrIntIsrStatus(UINT32 ipin);

//@}
#endif

