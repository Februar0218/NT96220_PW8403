/************************ "EK302DL MEMS Device Library" *************************************

Name                          : LIS302DL_REG.h

Project                       : EK302DL evaluation kit

Author                        : STMicroelectronics
                              : Firmware    ->   C.Iascone -
                                MEMS Product DIvision - Castelletto - Italy

Date                          : May 30th, 2007

Revision                      : 1-2-1

H/W platform                  : ST72F651 microcontroller 

MEMS platform                 : digital output LIS302DL

S/W platform                  : cosmic CXST7 for ST7

Copyright (c) 2007 STMicroelectronics.

THIS PROGRAM IS PROVIDED  "AS  IS"  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED,
INCLUDING  BUT NOT LIMITED TO,  THE IMPLIED WARRANTY  OF  MERCHANTABILITY  AND  FITNESS FOR A
PARTICULAR  PURPOSE.  THE ENTIRE  RISK  AS TO THE QUALITY  AND  PERFORMANCE OF THE PROGRAM IS
WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

THIS DOCUMENT  CONTAINS  PROPRIETARY AND CONFIDENTIAL  INFORMATION  OF THE STMICROELECTRONICS
GROUP.
INFORMATION FURNISHED IS BELIEVED TO BE ACCURATE  AND  RELIABLE.  HOWEVER, STMICROELECTRONICS
ASSUMES NO RESPONSIBILITY FOR THE CONSEQUENCES OF USE OF SUCH INFORMATION.
SPECIFICATIONS MENTIONED IN THIS PUBLICATION ARE SUBJECT TO CHANGE WITHOUT NOTICE.
THIS PUBLICATION SUPERSEDES AND REPLACES ALL INFORMATION PREVIOUSLY SUPPLIED.
STMICROELECTRONICS PRODUCTS ARE NOT AUTHORIZED FOR USE AS CRITICAL COMPONENTS IN LIFE SUPPORT
DEVICES OR SYSTEMS WITHOUT EXPRESS WRITTEN APPROVAL OF STMICROELECTRONICS.

STMicroelectronics GROUP OF COMPANIES

Australia - Belgium - Brazil - Canada - China - France - Germany - Italy - Japan - Korea -
Malaysia - Malta - Morocco - The Netherlands - Singapore - Spain - Sweden - Switzerland -
Taiwan - Thailand - United Kingdom - U.S.A.
STMicroelectronics Limited is a member of the STMicroelectronics Group.
*********************************************************************************************
* HISTORY:                                                                                  *
* Date      | Modification               | Author                                           *
* xx.02.07  | Initial revision           | C.Iascone                                        *
* 30.05.07  | Click function registers   | C.Iascone                                        *
*           | added                      |                                                  *
* --.--.--  | ----------------           | -------, -----                                   *
*********************************************************************************************/
#ifndef LIS302DL_REG_H
#define LIS302DL_REG_H

#define BIT(x) (1<<(x))

#define WHO_AM_I          0x0F // device identification register - default value 0x3B
#define I_AM_LIS302DL     0x3B 

/*Control Register 1*/
#define CTRL_REG1         0x20 

#define CTRL_REG1_DR      BIT(7)  // Data Rate Selection 
#define CTRL_REG1_DR_100  0       // 0: 100 Hz output data rate (default)
#define CTRL_REG1_DR_400  BIT(7)  // 1: 400 Hz output data rate

#define CTRL_REG1_PD      BIT(6)  // Power Down Control
#define CTRL_REG1_PD_OFF  0       // 0: power down mode (default)
#define CTRL_REG1_PD_ON   BIT(6)  // 1: active mode

#define CTRL_REG_FS       BIT(5)  // Full Scale Selection
#define CTRL_REG_FS_2G    0       // 0: 2g Full Scale (default)
#define CTRL_REG_FS_8G    BIT(5)  // 1: 8g Full Scale

#define CTRL_REG1_STP     BIT(4)  // Self Test P Enable
                                  // 0: normal mode (default)
                                  // 1: self test P enabled

#define CTRL_REG1_STM     BIT(3)  // Self Test M Enable
                                  // 0: normal mode (default)
                                  // 1: self test M enabled
                                  
#define CTRL_REG1_ZEN     BIT(2)  // Z-axis enable
                                  // 0: Z axis disabled 
                                  // 1: Z axis enabled (default)
#define CTRL_REG1_YEN     BIT(1)  // Y axis enable
                                  // 0: Y axis disabled  
                                  // 1: Y axis enabled (default)
#define CTRL_REG1_XEN     BIT(0)  // X axis enable 
                                  // 0: X axis disabled 
                                  // 1: x axis enabled (default)

/*Control Register 2*/
#define CTRL_REG2           0x21

#define CTRL_REG2_SIM       BIT(7)  // SPI Serial Interface Mode selection
                                    // 0: 4-wire interface (default)
                                    // 1: 3-wire interface
#define CTRL_REG2_BOOT      BIT(6)  // Reboot memory content
                                    // 0: normal mode (default)
                                    // 1: reboot memory content
#define CTRL_REG2_FDS       BIT(4)  // Filtered Data Selection
                                    // 0: internal filter bypassed (default)
                                    // 1: data from internal filter sent to output register
#define CTRL_REG2_HP_FF_WU2 BIT(3)  // High Pass filter enabled for FreeFall/WakeUp #2
                                    // 0: filter bypassed (default)
                                    // 1: filter enabled 
#define CTRL_REG2_HP_FF_WU1 BIT(2)  // High Pass filter enabled for FreeFall/WakeUp #1
                                    // 0: filter bypassed (default)
                                    // 1: filter enabled 
#define CTRL_REG2_HP_coef   BIT(1)|BIT(0)// High Pass filter cut-off frequency configuration
#define CTRL_REG2_HP_coef_0 0            // 00: 2 Hz (with DR = 100Hz) or
                                         //     8 Hz (with DR = 400Hz) (default value)
#define CTRL_REG2_HP_coef_1 BIT(0)       // 01: 1 Hz (with DR = 100Hz) or
                                         //     4 Hz (with DR = 400Hz)
#define CTRL_REG2_HP_coef_2 BIT(1)       // 10: 0.5 Hz (with DR = 100Hz) or
                                         //     2 Hz (with DR = 400Hz)
#define CTRL_REG2_HP_coef_3 BIT(1)|BIT(0)// 11: 0.25 Hz (with DR = 100Hz) or
                                         //     1 Hz (with DR = 400Hz)
/*Interrupt CTRL Register*/                                        
#define CTRL_REG3             0x22

#define CTRL_REG3_IHL         BIT(7)    // Interrupt active high or low
                                      // 0: active high (default)
                                      // 1: active low
#define CTRL_REG3_PP_OD       BIT(6)    // Push-Pull/Open Drain selection or interrupt pad
                                      // 0: push-pull (default)
                                      // 1: open drain
#define CTRL_REG3_I2CFG       BIT(5)|BIT(4)|BIT(3) // Data Signal on Int2 pad control bits
#define CTRL_REG3_I2CFG_GND   0                    // 000: Int2 Pad = GND (default)
#define CTRL_REG3_I2CFG_FFWU1 BIT(3)               // 001: Int2 Pad = FF_WU_1
#define CTRL_REG3_I2CFG_FFWU2 BIT(4)               // 010: Int2 Pad = FF_WU_2
#define CTRL_REG3_I2CFG_1OR2  BIT(4)|BIT(3)        // 011: Int2 Pad = FF_WU_1 OR FF_WU_2
#define CTRL_REG3_I2CFG_DR    BIT(5)               // 100: Int2 Pad = Dataready
#define CTRL_REG3_I2CFG_CI    BIT(5)|BIT(4)|BIT(3) // 111: Int2 Pad = Click Interrupt
                                                   // 110: Not used 
                                                   // 101: Not used
#define CTRL_REG3_I1CFG       BIT(2)|BIT(1)|BIT(0) // Data Signal on Int1 pad control bits
#define CTRL_REG3_I1CFG_GND   0                    // 000: Int1 Pad = GND (default)
#define CTRL_REG3_I1CFG_FFWU1 BIT(0)               // 001: Int1 Pad = FF_WU_1
#define CTRL_REG3_I1CFG_FFWU2 BIT(1)               // 010: Int1 Pad = FF_WU_2
#define CTRL_REG3_I1CFG_1OR2  BIT(1)|BIT(0)        // 011: Int1 Pad = FF_WU_1 OR FF_WU_2
#define CTRL_REG3_I1CFG_DR    BIT(2)               // 100: Int1 Pad = Dataready
#define CTRL_REG3_I1CFG_CI    BIT(2)|BIT(1)|BIT(0) // 111: Int2 Pad = Click Interrupt
                                                   // 110: Not used 
                                                   // 101: Not used
/*Dummy register*/
#define HP_FILTER_RESET   0x23    // Read this address to reset the high pass filter


/*Status register*/
#define STATUS_REG        0x27

#define STATUS_REG_ZYXOR  BIT(7) // X, Y and Z axis Data Overrun 
                                 // 0: no overrun has occurred (default)
                                 // 1: new data has over written the previous one
#define STATUS_REG_ZOR    BIT(6) // Z axis Data Overrun
                                 // 0: no overrun has occurred (default)
                                 // 1: new Z-axis data has over written the previous one
#define STATUS_REG_YOR    BIT(5) // Y axis Data Overrun 
                                 // 0: no overrun has occurred (default)
                                 // 1: new Y-axis data has over written the previous one
#define STATUS_REG_XOR    BIT(4) // X axis Data Overrun 
                                 // 0: no overrun has occurred (default)
                                 // 1: new X-axis data has over written the previous one
#define STATUS_REG_ZYXDA  BIT(3) // X, Y and Z axis new Data Available
                                 // O: a new set of data is not yet available (default)
                                 // 1: a new set of data is available 
#define STATUS_REG_ZDA    BIT(2) // Z axis new Data Available
                                 // 0: a new data for the Z-Axis is not available (default)
                                 // 1: a new data for the Z-Axis is available
#define STATUS_REG_YDA    BIT(1) // Y axis new Data Available
                                 // 0: a new data for the Y-Axis is not available (default)
                                 // 1: a new data for the Y-Axis is available
#define STATUS_REG_XDA    BIT(0) // X axis new Data Available
                                 // 0: a new data for the X-Axis is not available (default)
                                 // 1: a new data for the X-Axis is available

/* X axis output register*/
#define OUT_X            0x29  

/* Y axis output register*/
#define OUT_Y            0x2B   

/* Z axis output register*/
#define OUT_Z            0x2D  

/* Free-fall and inertial wake-up configuration register 1*/
#define FF_WU_CFG_1      0x30    

#define FF_WU_CFG1_AOI   BIT(7) // AND/OR combination of interrupt events
                                // 0: OR combination of interrupt events (default)
                                // 1: AND combination of interrupt events
#define FF_WU_CFG1_LIR   BIT(6) // Latch interrupt request
                                // 0: interrupt request not latched (default)
                                // 1: interrupt request latched 
#define FF_WU_CFG1_ZHIE  BIT(5) // Enable Interrupt request on Z high event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG1_ZLIE  BIT(4) // Enable Interrupt request on Z low event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG1_YHIE  BIT(3) // Enable Interrupt request on Y high event 
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request          
#define FF_WU_CFG1_YLIE  BIT(2) // Enable Interrupt request on Y low  event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG1_XHIE  BIT(1) // Enable Interrupt request on X high event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG1_XLIE  BIT(0) // Enable Interrupt request on X low event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request  
                               
/*Free-fall and wake-up source register 1*/                              
#define FF_WU_SRC_1      0x31

#define FF_WU_SRC1_IA    BIT(6) // Interrupt Active
                                // 0: no interrupt has been generated (default)
                                // 1: one ore more interrupt has been generated
#define FF_WU_SRC1_ZH    BIT(5) // Z High
                                // 0: no interrupt (default)
                                // 1: ZH event has occurred
#define FF_WU_SRC1_ZL    BIT(4) // Z Low
                                // 0: no interrupt (default)
                                // 1: ZL event has occurred
#define FF_WU_SRC1_YH    BIT(3) // Y High
                                // 0: no interrupt (default)
                                // 1: YH event has occurred
#define FF_WU_SRC1_YL    BIT(2) // Y Low
                                // 0: no interrupt (default)
                                // 1: YL event has occurred
#define FF_WU_SRC1_XH    BIT(1) // X High 
                                // 0: no interrupt (default)
                                // 1: XH event has occurred
#define FF_WU_SRC1_XL    BIT(0) // X Low
                                // 0: no interrupt (default)
                                // 1: XL event has occurred

/*Free-fall and inertial wake-up threshold 1*/
#define FF_WU_THS_1      0x32

#define FF_WU_THS1_DCRM  BIT(7) // Resetting mode selection
                                // 0: counter resetted
                                // 1: counter decremented
#define FF_WU_THS1_THS   BIT(6)|BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1)|BIT(0) 
                                //Free-fall / wake up threshold
                                
/*Duration register for Free_Fall/Wake-Up interrupt 1*/
#define FF_WU_DURATION_1 0x33

/* Free-fall and inertial wake-up configuration register 2*/
#define FF_WU_CFG_2      0x34    

#define FF_WU_CFG2_AOI   BIT(7) // AND/OR combination of interrupt events
                                // 0: OR combination of interrupt events (default)
                                // 1: AND combination of interrupt events
#define FF_WU_CFG2_LIR   BIT(6) // Latch interrupt request
                                // 0: interrupt request not latched (default)
                                // 1: interrupt request latched 
#define FF_WU_CFG2_ZHIE  BIT(5) // Enable Interrupt request on Z high event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG2_ZLIE  BIT(4) // Enable Interrupt request on Z low event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG2_YHIE  BIT(3) // Enable Interrupt request on Y high event 
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request          
#define FF_WU_CFG2_YLIE  BIT(2) // Enable Interrupt request on Y low  event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG2_XHIE  BIT(1) // Enable Interrupt request on X high event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request 
#define FF_WU_CFG2_XLIE  BIT(0) // Enable Interrupt request on X low event
                                // 0: disable interrupt request (default)
                                // 1: enable interrupt request  

/*Free-fall and wake-up source register 2*/                              
#define FF_WU_SRC_2      0x35

#define FF_WU_SRC2_IA    BIT(6) // Interrupt Active
                                // 0: no interrupt has been generated (default)
                                // 1: one ore more interrupt has been generated
#define FF_WU_SRC2_ZH    BIT(5) // Z High
                                // 0: no interrupt (default)
                                // 1: ZH event has occurred
#define FF_WU_SRC2_ZL    BIT(4) // Z Low
                                // 0: no interrupt (default)
                                // 1: ZL event has occurred
#define FF_WU_SRC2_YH    BIT(3) // Y High
                                // 0: no interrupt (default)
                                // 1: YH event has occurred
#define FF_WU_SRC2_YL    BIT(2) // Y Low
                                // 0: no interrupt (default)
                                // 1: YL event has occurred
#define FF_WU_SRC2_XH    BIT(1) // X High 
                                // 0: no interrupt (default)
                                // 1: XH event has occurred
#define FF_WU_SRC2_XL    BIT(0) // X Low
                                // 0: no interrupt (default)
                                // 1: XL event has occurred

/*Free-fall and inertial wake-up threshold 2*/
#define FF_WU_THS_2      0x36

#define FF_WU_THS2_DCRM  BIT(7) // Resetting mode selection
                                // 0: counter resetted 
                                // 1: counter decremented
#define FF_WU_THS2_THS   BIT(6)|BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1)|BIT(0) 
                                //Free-fall / wake up threshold
                                
/*Duration register for Free_Fall/Wake-Up interrupt 2*/
#define FF_WU_DURATION_2 0x37

/*Click and Double Click interrupt configuration register*/
#define CLICK_CFG           0x38

#define CLICK_CFG_LIR       BIT(6) // Latch interrupt request
                                   // 0: interrupt request not latched (default)
                                   // 1: interrupt request latched 
#define CLICK_CFG_DOUBLE_Z  BIT(5) // Enable double click interrupt on Z axis
                                   // 0: disable interrupt request (default)
                                   // 1: enable interrupt request 
#define CLICK_CFG_SINGLE_Z  BIT(4) // Enable single click interrupt on Z axis
                                   // 0: disable interrupt request (default)
                                   // 1: enable interrupt request 
#define CLICK_CFG_DOUBLE_Y  BIT(3) // Enable double click interrupt on Y axis
                                   // 0: disable interrupt request (default)
                                   // 1: enable interrupt request          
#define CLICK_CFG_SINGLE_Y  BIT(2) // Enable single click interrupt on Y axis
                                   // 0: disable interrupt request (default)
                                   // 1: enable interrupt request 
#define CLICK_CFG_DOUBLE_X  BIT(1) // Enable double click interrupt on X axis
                                   // 0: disable interrupt request (default)
                                   // 1: enable interrupt request 
#define CLICK_CFG_SINGLE_X  BIT(0) // Enable single click interrupt on X axis
                                   // 0: disable interrupt request (default)
                                   // 1: enable interrupt request 
                                   
/*Click and Double Click interrupt source register*/
#define CLICK_SRC           0x39

#define CLICK_SRC_IA        BIT(6) // Interrupt Active
                                   // 0: no interrupt has been generated (default)
                                   // 1: one ore more interrupt has been generated 
#define CLICK_SRC_DOUBLE_Z  BIT(5) // Double click on Z axis event
                                   // 0: no interrupt (default)
                                   // 1: Double click event on Z axis has been generated
#define CLICK_SRC_SINGLE_Z  BIT(4) // Single click on Z axis event
                                   // 0: no interrupt (default)
                                   // 1: Single click event on Z axis has been generated 
#define CLICK_SRC_DOUBLE_Y  BIT(3) // Double click on Y axis event
                                   // 0: no interrupt (default)
                                   // 1: Double click event on Y axis has been generated
#define CLICK_SRC_SINGLE_Y  BIT(2) // Single click on Y axis event
                                   // 0: no interrupt (default)
                                   // 1: Single click event on Y axis has been generated 
#define CLICK_SRC_DOUBLE_X  BIT(1) // Double click on X axis event
                                   // 0: no interrupt (default)
                                   // 1: Double click event on X axis has been generated
#define CLICK_SRC_SINGLE_X  BIT(0) // Single click on X axis event
                                   // 0: no interrupt (default)
                                   // 1: Double click event on X axis has been generated
                                   
/*Click and Double Click threshold Y and X*/
#define CLICK_THSY_X        0x3B

#define CLICK_THS_Y         BIT(7)|BIT(6)|BIT(5)|BIT(4)
#define CLICK_THS_X         BIT(3)|BIT(2)|BIT(1)|BIT(0)

/*Click and Double Click threshold Z*/
#define CLICK_THSZ          0x3C

#define CLICK_THS_Z         BIT(3)|BIT(2)|BIT(1)|BIT(0)

/*Click and Double Click Time Limit*/
#define CLICK_TIMELIMIT     0x3D

/*Click and Double Click Latency*/
#define CLICK_LATENCY       0x3E

/*Click and Double Click Window*/
#define CLICK_WINDOW        0x3F


#endif // LIS302DL_REG.h

