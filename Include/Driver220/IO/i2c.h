/**
    I2C module driver

    This I2C module driver supports only for I2C master-transmit/receive modes.
    We can support for standard(100kbps), fast(400kbps), and fast-plus(1Mbps) modes through
    configuring the clock divider register. And can support both the 7-bit, 10-bit and general-call addressing with the slave.
    Multiple-Masters are allowed to reside on the I2C bus at the same time.
    Glitch suppression is applied also through de-bounce circuit in this module.
    Multiple-Tasks are allowed to use this module concurrently by the semaphore protection.

    @file       i2c.h
    @ingroup    mIIOI2C

    @note       The following descriptions are the sugguestion programming flow of the I2C module.
\n
\n              (1) Call i2c_open() to open this module, clock will be enabled.
                    User need not use this api before each transfer. User just need calling it once and then start
                    to transfer bursts of data. And then call the i2c_close() after you dont need this module any more.
\n              (2) Call i2c_lock() to get the access right over the i2c bus.Because it may have multiple tasks
                    using i2c master to access multi-slaves, user must obey this rule to prevent vital access error.
\n              (3) Call i2c_enable() to enable the i2c controller.
\n              (4) Call i2c_initMaster() to initialize i2c's bus signal timing such as clkdiv, tsr, and gsr.
                    The I2C Bus clock SCL's frequency = APB-CLK / ((2*clkdiv)+6+gsr).
                    tsr is used to set the setup/hold time of the SDA signal, the typical value can set tsr = clkdiv/2.
                    gsr is glitch suppression logic which supress the glitch of SCL/SDA.
\n              (5) Call i2c_setPIOSize() to setup the pio transfer size in the next master-transmit/receive, this value can only use 1/2/3/4.
\n              (6) Call i2c_setPIOStartGen() / i2c_setPIOStopGen() / i2c_setPIOAckGen() to setup the START/STOP/ACK bit status
                    you want to in the next master-transmit/receive.
\n              (7) Call i2c_masterTransmit4B() / i2c_masterReceive4B() to trigger transmit/receive operation.
                    User must get the return value of these two APIs to do the error handling if any exceptions occure.
\n                  The return of I2C_SUCCESS means transmission success with no error.
\n                  The return of ER_I2C_NAK means NAK bit is returned by the slave.
                    The controller would send STOP bit automatically after NAK bit is get in master-transmit.
                    User must repeat Step-5 to Step-7 with START bit is set to retry the master-transmit.
\n                  The return of ER_I2C_AL means arbitration lost during muilti-masters environment.
                    User must repeat Step-5 to Step-7 with START bit is set to retry the master-transmit/receive.
\n                  The return of ER_I2C_DATA_ERROR should not be seen, this error code maybe caused by the system error.
\n                  The return of ER_I2C_BUS_NOT_AVAILABLE means SCL is occupied a long period of time due to unknown reasons.
\n              (8) User can repeat Step-5 to Step-7 to transmit/receive bursts of data
                    but remembering to obey the error handling in the Step-7.
\n              (9) Call i2c_disable() after no more data has to transmit/receive.
\n             (10) Call i2c_unlock() to release the bus access right to other users.
\n             (11) Call i2c_close() after you dont need this module any more.
                    The I2C clock will be truely disabled after none of users are using I2C.
\n
\n              So the overall programming flows may like the followings:
\n              i2c_open();                                                                 //(1)
\n              i2c_lock();                                                                 //(2)
\n              i2c_enable();                                                               //(3)
\n              i2c_initMaster(clkdiv,tsr,gsr);                                             //(4)
\n              while(data-burst)                                                           //(8)
\n              {
\n                  i2c_setPIOSize(size);                                                   //(5)
\n                  i2c_setPIOStartGen(...);                                                //(6)
\n                  i2c_setPIOStopGen(...);                                                 //(6)
\n                  i2c_setPIOAckGen(...);                                                  //(6)
\n                  Return =  i2c_masterTransmit4B();  OR  Return =  i2c_masterReceive4B(); //(7)
\n                  if(Return == I2C_SUCCESS)
\n                      Continue transmission next Data.
\n                  else if((Return == ER_I2C_NAK)||(Return == ER_I2C_AL))
\n                      Retry (5) to (7) with START bit is set to retry current data transmit.
\n                  else if(Return == ER_I2C_BUS_NOT_AVAILABLE)
\n                      SCL is held low due to unknown reason, Try to figure out which device holds the SCL low.
\n              }
\n              i2c_disable();                                                              //(9)
\n              i2c_unlock();                                                               //(10)
\n              i2c_close();                                                                //(11)
\n

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.

*/

#ifndef _I2C_H
#define _I2C_H

#include "Type.h"

/**
    I2C transfer return status code definitions

    The return code of the I2C would tell the transmission is successfully or fails due to
    NAK/Lose-Arbitration/System-Error or unknown reason to make bus keeping low level.
    The user of the I2C must get the returned error code and complete the error handling.
*/
typedef enum
{
    I2C_SUCCESS,                        ///< I2C bus transfer success

    ER_I2C_DATA_ERROR,                  ///< Without any I2C status returned, this status should not be seen.
    ER_I2C_NAK,                         ///< Receive a NAK, please follow the error handling flows.
    ER_I2C_BUSY,                        ///< I2C busy between START and STOP, should not be seen in this chip version.
    ER_I2C_BUS_NOT_AVAILABLE,           ///< I2C bus is being used by anothers.
    ER_I2C_AL,                          ///< Lose arbitration
    ER_I2C_SYS_ERR,                     ///< System error due to non-supported function in this chip version.
    I2C_STOP,                           ///< Detect STOP condition (slave mode only), should not be seen in this chip version.

    ENUM_DUMMY4WORD(ER_I2C)
} ER_I2C;

/**
    I2C DATA Register

    This definition is used in i2c_masterTransmit4B().
*/
typedef struct
{
    UINT8  uiData0;      ///< I2C Data Register0
    UINT8  uiData1;      ///< I2C Data Register1
    UINT8  uiData2;      ///< I2C Data Register2
    UINT8  uiData3;      ///< I2C Data Register3
} I2C_DATA_REG, *PI2C_DATA_REG;

/**
    @name Suggestion I2C Clock initialization parameter at different i2c bus rate

    These parameters can be used in the i2c_initMaster() to specify specific i2c bus frequency: 400/200/100/50 Khz.
*/
//@{
#define I2C_CLOCK_DIVIDER_400KHZ    0x5D    ///< 400Khz I2C Clock Divider Value. Which is used in the first paramter of i2c_initMaster().
#define I2C_CLOCK_DIVIDER_200KHZ    0xC1    ///< 200Khz I2C Clock Divider Value. Which is used in the first paramter of i2c_initMaster().
#define I2C_CLOCK_DIVIDER_100KHZ    0x189   ///< 100Khz I2C Clock Divider Value. Which is used in the first paramter of i2c_initMaster().
#define I2C_CLOCK_DIVIDER_50KHZ     0x319   ///<  50Khz I2C Clock Divider Value. Which is used in the first paramter of i2c_initMaster().

#define I2C_TSR_400KHZ              0x2E    ///< 400Khz I2C TSR Value. Which is used in the second paramter of i2c_initMaster().
#define I2C_TSR_200KHZ              0x60    ///< 200Khz I2C TSR Value. Which is used in the second paramter of i2c_initMaster().
#define I2C_TSR_100KHZ              0xC4    ///< 100Khz I2C TSR Value. Which is used in the second paramter of i2c_initMaster().
#define I2C_TSR_50KHZ               0x18C   ///<  50Khz I2C TSR Value. Which is used in the second paramter of i2c_initMaster().

#define I2C_GSR_400KHZ              0x07    ///< 400Khz I2C GSR Value. Which is used in the third paramter of i2c_initMaster().
#define I2C_GSR_200KHZ              0x07    ///< 200Khz I2C GSR Value. Which is used in the third paramter of i2c_initMaster().
#define I2C_GSR_100KHZ              0x07    ///< 100Khz I2C GSR Value. Which is used in the third paramter of i2c_initMaster().
#define I2C_GSR_50KHZ               0x07    ///<  50Khz I2C GSR Value. Which is used in the third paramter of i2c_initMaster().
//@}


#if 0
// Need not extern interrupt enable/disable api.
// This may cause disturbing.

#define I2C_INT_DT_EN     0x0100
#define I2C_INT_DR_EN     0x0200
#define I2C_INT_BERR_EN   0x0400
#define I2C_INT_STOP_EN   0x0800
#define I2C_INT_SAM_EN    0x1000
#define I2C_INT_AL_EN     0x2000

extern void    i2c_enableInt(UINT32 IntCtrl);
extern void    i2c_disableInt(UINT32 IntCtrl);
#endif



//
//  I2C Extern API Definitions
//

extern ER      i2c_close(void);
extern ER      i2c_open(void);

extern BOOL    i2c_isOpened(void);

extern ER      i2c_lock(void);
extern ER      i2c_unlock(void);

extern void    i2c_enable(void);
extern void    i2c_disable(void);

extern UINT32  i2c_getStatus(void);
extern void    i2c_getClkSetting(UINT32 *clkdiv, UINT32 *tsr, UINT32 *gsr);

extern ER_I2C  i2c_setPIOSize(UINT32 size);
extern void    i2c_setPIOAckGen(BOOL Ack1 , BOOL Ack2 , BOOL Ack3 ,BOOL Ack4);
extern void    i2c_setPIOStartGen(BOOL Sta1 , BOOL Sta2 , BOOL Sta3 ,BOOL Sta4);
extern void    i2c_setPIOStopGen(BOOL St1 , BOOL St2 , BOOL St3 ,BOOL St4);
extern ER_I2C  i2c_masterTransmitB(I2C_DATA_REG uiData);
extern ER_I2C  i2c_masterTransmit4B(I2C_DATA_REG uiData);
extern ER_I2C  i2c_masterReceive4B(UINT32 *puiData);

extern ER_I2C  i2c_initMaster(UINT32 clkdiv, UINT32 tsr, UINT32 gsr);
extern ER_I2C  i2c_masterReceive(UINT32 *puiData, BOOL bNak, BOOL bStop);
extern ER_I2C  i2c_masterTransmit(UINT32 uiData, BOOL bStart, BOOL bStop);

extern void    i2c_setSclTimeout(UINT32 uiThreshold);

//
//  Backward Compatible Usage
//
#define i2c_reset()                                         do{ }while(0)
#define i2c_setSAR(sar, tenBitMode)                         do{ }while(0)
#define i2c_getSlaveAddress(puiSar, bTenBitMode)            do{ }while(0)

#define i2c_masterTransmitDMA(pDMA_Addr,uiDMA_Trans_Len)    ER_I2C_SYS_ERR
#define i2c_masterReceiveDMA(pDMA_Addr,uiDMA_Trans_Len)     ER_I2C_SYS_ERR

#define i2c_slaveTransmitB(uiData)                          ER_I2C_SYS_ERR
#define i2c_slaveTransmit4B(uiData)                         ER_I2C_SYS_ERR
#define i2c_slaveReceive4B(puiData)                         ER_I2C_SYS_ERR
#define i2c_slaveTransmitDMA(pDMA_Addr,uiDMA_Trans_Len)     ER_I2C_SYS_ERR
#define i2c_slaveReceiveDMA(pDMA_Addr,uiDMA_Trans_Len)      ER_I2C_SYS_ERR

#endif
