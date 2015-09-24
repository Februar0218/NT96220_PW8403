
#ifndef __LIS33DE_H
#define __LIS33DE_H
#include "Gsensor.h"
#include "Gpio.h"

typedef struct _GSENSOR_PIN_DEF
{
    UINT32  uigpio_sen;                 ///< GPIO pin number for SEN, valid when uimode == ENDE_CTRL_MODE_GPIO
    UINT32  uigpio_clk;                 ///< GPIO pin number for Clock, valid when uimode == ENDE_CTRL_MODE_GPIO
    UINT32  uigpio_data;                ///< GPIO pin number for Data, valid when uimode == ENDE_CTRL_MODE_GPIO
} GSENSOR_PIN_DEF, *PGSENSOR_PIN_DEF;


typedef struct _GSENSOR_DEVICE_OBJ
{
    GSENSOR_PIN_DEF  GSensorCtrl_Pin;///
    void          (*fpPostAdjusement)(void);///
} GSENSOR_DEVICE_OBJ, *PGSENSOR_DEVICE_OBJ;

//typedef bit                 BOOL;       /* Boolean */

typedef unsigned char       UB8;        /* 8-bit data */
typedef unsigned short int  UW16;       /*16-bit data */
typedef unsigned long       UW32;       /*32-bit data */

typedef signed char         SB8;        /* 8-bit data */
typedef signed short int    SW16;       /*16-bit data */
typedef signed long         SW32;       /*32-bit data */

#define VP_MAD 0xc0

#define I2C_SM          0
#define I2C_FM          1
#define I2C_FMPLUS      2
#define I2C_CFG         I2C_FM


/* ::::::::::::::::::::::::::::::::::::
    [3]37
    I2C Standard Mode   (Sm, 100 kbit/s)
    I2C Fast-mode       (Fm, 400 kbit/s)
    I2C Fast-mode Plus  (Fm+, 1 Mbit/s)
   :::::::::::::::::::::::::::::::::::: */
/*
  Standard-mode (Sm), with a bool rate up to 100 kbit/s
  Fast-mode (Fm), with a bool rate up to 400 kbit/s
  Fast-mode Plus (Fm+), with a bool rate up to 1 Mbit/s
  High-speed mode (Hs-mode), with a bool rate up to 3.4 Mbit/s.

                             Sm             Fm          Fm+
Symbol  Parameter Conditions Standard-mode  Fast-mode   Fast-mode Plus  Unit
                               Min  Max     Min Max     Min Max
fSCL    SCL clock frequency     0   100     0   400     0   1000        kHz


tHD;STA hold time (repeated)    4.0 -       0.6 -       0.26 -          us
        START condition
        After this period, the
        first clock pulse is
        generated.

tLOW    LOW period of the SCL   4.7 -       1.3 -       0.5  -          us
        clock

tHIGH   HIGH period of the SCL  4.0 -       0.6 -       0.26 -          us
        clock

tSU;STA set-up time for a       4.7 -       0.6 -       0.26 -          us
        repeatedSTART condition

tHD;DAT I2C-bus devices         0[3] -[4]   0[3] -[4]   0    -          us

        data hold time[2]       5.0 -       -   -       -    -          us
        CBUS compatible masters
        (see Remark in Section 4.1)

tSU;DAT data set-up time        250 -       100[5] -    50   -          ns

tr      rise time of both SDA   -   1000    20+ 300     -    120        ns
        and SCL signals

tf      fall time of both SDA   -   300     20+ 300     -    120        ns
        and SCL signals

tSU;STO set-up time for STOP    4.0 -       0.6 -       0.26 -          us
        condition

tBUF    bus free time between   4.7 -       1.3 -       0.5  -          us
        a STOP and START
        condition

tVD;DAT data valid time[10]     -   3.45[4] -   0.9[4]  -    0.45[4]    us

tVD;ACK data valid acknowledge  -   3.45[4] -   0.9[4]  -    0.45[4]    us
        time[11]
*/


// [1]117,125 Internal Oscillator = 12.0 +/- 0.18 MHz = 12.0 +/- 1.5% MHz
// SYSCLK = 1 / 12.000MHz = 83.33 ns = 0.08333 us
//#define SYSCLK_PERIOD           83

// SYSCLK = 1 / 24.000MHz = 41.66 ns = 0.04166 us
#define SYSCLK_PERIOD           7



#if (I2C_CFG == I2C_SM)

    #define I2C_SU_STA          (4700 / SYSCLK_PERIOD)
    #define I2C_HD_STA          (4000 / SYSCLK_PERIOD)
    #define I2C_SU_DAT          ( 250 / SYSCLK_PERIOD)
    #define I2C_HD_DAT          (   0 / SYSCLK_PERIOD)
    #define I2C_HIGH            (4000 / SYSCLK_PERIOD)
    #define I2C_LOW             (4700 / SYSCLK_PERIOD)
    #define I2C_SU_STO          (4000 / SYSCLK_PERIOD)
    #define I2C_BUF             (4700 / SYSCLK_PERIOD)
    #define I2C_VD_DAT          (3450 / SYSCLK_PERIOD)

#elif (I2C_CFG == I2C_FM)

    #define I2C_SU_STA          ( 600 / SYSCLK_PERIOD)
    #define I2C_HD_STA          ( 600 / SYSCLK_PERIOD)
    #define I2C_SU_DAT          ( 100 / SYSCLK_PERIOD)
    #define I2C_HD_DAT          (   0 / SYSCLK_PERIOD)
    #define I2C_HIGH            ( 600 / SYSCLK_PERIOD)
    #define I2C_LOW             (1300 / SYSCLK_PERIOD)
    #define I2C_SU_STO          ( 600 / SYSCLK_PERIOD)
    #define I2C_BUF             (1300 / SYSCLK_PERIOD)
    #define I2C_VD_DAT          ( 900 / SYSCLK_PERIOD)

#elif (I2C_CFG == I2C_FMPLUS)

    #define I2C_SU_STA          ( 260 / SYSCLK_PERIOD)
    #define I2C_HD_STA          ( 260 / SYSCLK_PERIOD)
    #define I2C_SU_DAT          (  50 / SYSCLK_PERIOD)
    #define I2C_HD_DAT          (   0 / SYSCLK_PERIOD)
    #define I2C_HIGH            ( 260 / SYSCLK_PERIOD)
    #define I2C_LOW             ( 500 / SYSCLK_PERIOD)
    #define I2C_SU_STO          ( 260 / SYSCLK_PERIOD)
    #define I2C_BUF             ( 500 / SYSCLK_PERIOD)
    #define I2C_VD_DAT          ( 450 / SYSCLK_PERIOD)

#else
    #error I2C.H - invalid I2C_CFG !
#endif

#ifdef NOT_JUNK
/* ::::::::::::::::::::::::::::::::::::
    I2C Mode Configuration
   :::::::::::::::::::::::::::::::::::: */
#define I2C_MODE_SLOW               1           // < 100 kbit/s
#define I2C_MODE_STANDARD           2           // 100 kbit/s       108K Hz for CLK HIGH interval
#define I2C_MODE_FAST               3           // 400 kbit/s       400K Hz for CLK HIGH interval
#define I2C_MODE_HIGHSPEED          4           // 3.4 Mbit/s       1.19M Hz for CLK HIGH interval

#define I2C_MODE_CFG                I2C_MODE_STANDARD
//#define I2C_MODE_CFG                I2C_MODE_SLOW
#endif

/* ::::::::::::::::::::::::::::::::::::
    I2C Configuration
   :::::::::::::::::::::::::::::::::::: */
#define I2C_RX_BURST        ON
#define I2C_RX_BYTE         ON
#define I2C_RX_WORD         OFF

#define I2C_SET_ACK         ON

#define I2C_TX_BURST        ON
#define I2C_TX_BYTE         ON
#define I2C_TX_REPEAT       ON
#define I2C_TX_WORD         OFF

#define HIGH 1
#define LOW 1
/* ::::::::::::::::::::::::::::::::::::
    BITEK Configuration
   :::::::::::::::::::::::::::::::::::: */
#define BITEK_RxByte(wREG)                  \
    I2C_RxByte((VP_MAD) | (((wREG) >> 7) & 0x1E), (wREG))


#define BITEK_TxBurst(wREG, wCNT, pbDATA)   \
    I2C_TxBurst((VP_MAD) | (((wREG) >> 7) & 0x1E), (wREG), (wCNT), (pbDATA))


#define BITEK_TxByte(wREG, bDATA)           \
    I2C_TxByte((VP_MAD) | (((wREG) >> 7) & 0x1E), (wREG), (bDATA))


#define BITEK_TxRepeat(wREG, wCNT, bDATA)   \
    I2C_TxRepeat((VP_MAD) | (((wREG) >> 7) & 0x1E), (wREG), (wCNT), (bDATA))


/* ------------------------------------
    Type Definitions
   ------------------------------------ */

/* ------------------------------------
    Variables Definitions/Declarations
   ------------------------------------ */

/* ------------------------------------
    Function Prototypes
   ------------------------------------ */
void I2C_Delay(UW16 Time);

extern BOOL I2C_GetACK(void);
extern void BIT_I2C_Init(void);


#if (I2C_RX_BURST)
extern void I2C_RxBurst(UB8 bSLA, UB8 bREG, UB8 bCNT, UB8 *pbDATA);
#endif

#if (I2C_RX_BYTE)
extern UB8  I2C_RxByte(UB8 bSLA, UB8 bREG);
#endif

extern UB8  I2C_RxData(void);

#if (I2C_RX_WORD)
extern UW16 I2C_RxWord(UB8 bSLA, UB8 bREG);
#endif


#if (I2C_SET_ACK)
extern void I2C_SetACK(void);
#endif

extern void I2C_SetNAK(void);

extern void I2C_Start(void);
extern void I2C_Stop(void);


#if (I2C_TX_BURST)
extern void I2C_TxBurst(UB8 bSLA, UB8 bREG, UB8 bCNT, UB8 *pbDATA);
#endif

#if (I2C_TX_BYTE)
extern void I2C_TxByte(UB8 bSLA, UB8 bREG, UB8 bDATA);
#endif

extern void I2C_TxData(UB8 bData);

#if (I2C_TX_REPEAT)
extern void I2C_TxRepeat(UB8 bSLA, UW16 wREG, UB8 bCNT, UB8 bDATA);
#endif


#if (I2C_TX_WORD)
extern void I2C_TxWord(UB8 bSLA, UB8 bREG, UW16 wDATA);
#endif


//I2C control pin
#if 1////SA0=0
#define GMA301_WRITE_ID 0x30
#define GMA301_READ_ID 0x31
#else////SA0=1
#define MM3A310DA311_WRITE_ID 0x4e
#define MM3A310DA311_READ_ID 0x4f
#endif

#define GPIO_GSENSOR_SCK      P_GPIO_0
#define GPIO_GSENSOR_SDA      P_GPIO_1

#define GPIO_BIT1612_SCK      D_GPIO_1
#define GPIO_BIT1612_SDA      D_GPIO_0
//模拟I2C总线的引脚定义

//I2C control pin
#define I2C_SCL_OUT				gpio_setDir(GPIO_GSENSOR_SCK, GPIO_DIR_OUTPUT)
#define I2C_SCL_IN				gpio_setDir(GPIO_GSENSOR_SCK, GPIO_DIR_INPUT)
#define I2C_SCL_HIGH			gpio_setPin(GPIO_GSENSOR_SCK)
#define I2C_SCL_LOW				gpio_clearPin(GPIO_GSENSOR_SCK)
#define I2C_SCL_DETECT			gpio_getPin(GPIO_GSENSOR_SCK)

#define I2C_SDA_OUT				gpio_setDir(GPIO_GSENSOR_SDA, GPIO_DIR_OUTPUT)
#define I2C_SDA_IN				gpio_setDir(GPIO_GSENSOR_SDA, GPIO_DIR_INPUT)
#define I2C_SDA_HIGH 			gpio_setPin(GPIO_GSENSOR_SDA)
#define I2C_SDA_LOW  			gpio_clearPin(GPIO_GSENSOR_SDA)
#define I2C_SDA_DETECT			gpio_getPin(GPIO_GSENSOR_SDA)


#define BIT1216M_I2C_SCL_OUT				gpio_setDir(GPIO_BIT1612_SCK, GPIO_DIR_OUTPUT)
#define BIT1216M_I2C_SCL_IN				gpio_setDir(GPIO_BIT1612_SCK, GPIO_DIR_INPUT)
#define BIT1216M_I2C_SCL_HIGH			       gpio_setPin(GPIO_BIT1612_SCK)
#define BIT1216M_I2C_SCL_LOW				gpio_clearPin(GPIO_BIT1612_SCK)
#define BIT1216M_I2C_SCL_DETECT			gpio_getPin(GPIO_BIT1612_SCK)

#define BIT1216M_I2C_SDA_OUT				gpio_setDir(GPIO_BIT1612_SDA, GPIO_DIR_OUTPUT)
#define BIT1216M_I2C_SDA_IN				gpio_setDir(GPIO_BIT1612_SDA, GPIO_DIR_INPUT)
#define BIT1216M_I2C_SDA_HIGH 			       gpio_setPin(GPIO_BIT1612_SDA)
#define BIT1216M_I2C_SDA_LOW  			       gpio_clearPin(GPIO_BIT1612_SDA)
#define BIT1216M_I2C_SDA_DETECT			gpio_getPin(GPIO_BIT1612_SDA)


#define ERROR_CODE_TRUE			0
#define ERROR_CODE_FALSE		1
#define ERROR_CODE_WRITE_ADDR	10
#define ERROR_CODE_WRITE_DATA	20
#define ERROR_CODE_READ_ADDR	30
#define ERROR_CODE_READ_DATA	40
#define ERROR_CODE_START_BIT	50
#define ERROR_CODE_APROCESS		60
#define ERROR_CODE_DENY			70

#define OUT_X_L_REG           	0x14
#define OUT_X_H_REG           	0x15
#define OUT_Y_L_REG           	0x16
#define OUT_Y_H_REG           	0x17
#define OUT_Z_L_REG            	0x18
#define OUT_Z_H_REG           	0x19

#define CTRL_FS (1 < 5)
//Full scale selection

#define STATUS_ZXOR (1 < 7)
//X, Y, Z axis Data Overrun. Default value: 0
//(0: no overrun has occurred;
//1: new data has overwritten the previous one before it was read)

#define STATUS_ZOR  (1 < 6)
//Z axis Data Overrun. Default value: 0
//(0: no overrun has occurred;
//1: a new data for the Z-axis has overwritten the previous one)

#define STATUS_YOR  (1 < 5)
//Y axis Data Overrun. Default value: 0
//(0: no overrun has occurred;
//1: a new data for the Y-axis has overwritten the previous one)

#define STATUS_XOR  (1 < 4)
//X axis Data Overrun. Default value: 0
//(0: no overrun has occurred;
//1: a new data for the X-axis has overwritten the previous one)

#define STATUS_ZYXDA (1 < 3)
//X, Y and Z axis new Data Available. Default value: 0
//(0: a new set of data is not yet available; 1: a new set of data is available)

#define STATUS_ZDA (1 < 2)
//Z axis new Data Available. Default value: 0
//(0: a new data for the Z-axis is not yet available;
//1: a new data for the Z-axis is available)

#define STATUS_YDA (1 < 1)
//Y axis new Data Available. Default value: 0
//(0: a new data for the Y-axis is not yet available;
//1: a new data for the Y-axis is available)

#define STATUS_XDA (1 < 0)
//X axis new Data Available. Default value: 0
//(0: a new data for the X-axis is not yet available;
//1: a new data for the X-axis is available)





//********************************************************** 
// 
//                  Register Addresses 
// 
//********************************************************** 
#define	WHO_AM_I			0x0F 
#define	CTRL_REG1			0x20 
#define	CTRL_REG2			0x21 
#define	CTRL_REG3			0x22 
#define	HP_FILTER_RESET		0x23 
#define	STATUS_REG			0x27 
#define	OUT_X				0x29 
#define	OUT_Y				0x2B 
#define	OUT_Z				0x2D 
#define	FF_WU_CFG_1			0x30 
#define	FF_WU_SRC_1			0x31 
#define	FF_WU_THS_1			0x32 
#define	FF_WU_DURATION_1	0x33 
#define	FF_WU_CFG_2			0x34 
#define	FF_WU_SRC_2			0x35 
#define	FF_WU_THS_2			0x36 
#define	FF_WU_DURATION_2	0x37 
#define	CLICK_CFG			0x38 
#define	CLICK_SRC			0x39 
#define	CLICK_THSY_X		0x3B 
#define	CLICK_THSZ   		0x3C
#define CLICK_TIMELIMIT		0X3D 
#define CLICK_LATENCY		0X3E 
#define CLICK_WINDOW		0x3F 
 
//**************************** 
//CTRL_REG1 BITS 
#define	XEN					(1<<0)//X axis enable. Default value: 1 (0: X axis disabled; 1: X axis enabled) 
#define YEN					(1<<1)//Y axis enable. Default value: 1 (0: Y axis disabled; 1: Y axis enabled) 
#define ZEN					(1<<2)//Z axis enable. Default value: 1 (0: Z axis disabled; 1: Z axis enabled) 
#define STM 				(1<<3) 
#define STP 				(1<<4) 
#define FS  				(1<<5)//Full Scale selection. Default value: 0 
#define PD  				(1<<6)//Power Down Control. Default value: 0 (0: power down mode; 1: active mode)
#define DR  				(1<<7)// Data rate selection. Default value: 0 (0: 100 Hz output data rate; 1: 400 Hz output data rate)
//**************************** 
 
//**************************** 
//CTRL_REG2 BITS 
#define	HP_coeff1			(1<<0) 
#define HP_coeff2			(1<<1)//High pass filter cut-off frequency configuration. Default value: 00 
#define HP_FF_WU1			(1<<2)//High Pass filter enabled for FreeFall/WakeUp # 1. Default value: 0 (0: filter bypassed; 1: filter enabled)
#define HP_FF_WU2			(1<<3)//High Pass filter enabled for FreeFall/WakeUp # 2. Default value: 0 (0: filter bypassed; 1: filter enabled) 
#define FDS					(1<<4)//Filtered Data Selection. Default value: 0 (0: internal filter bypassed; 1: data from internal filter sent to output register) 
#define BOOT				(1<<6)//Reboot memory content. Default value: 0 (0: normal mode; 1: reboot memory content) 
#define SIM					(1<<7)//SPI Serial Interface Mode selection. Default value: 0 (0: 4-wire interface; 1: 3-wire interface) 
//**************************** 
 
//**************************** 
//CTRL_REG3 BITS 
#define	I1CFG0				(1<<0) 
#define I1CFG1				(1<<1) 
#define I1CFG2				(1<<2)//Data Signal on Int1 pad control bits. Default value 000. 
#define I2CFG0 				(1<<3) 
#define I2CFG1 				(1<<4) 
#define I2CFG2  			(1<<5)//Data Signal on Int2 pad control bits. Default value 000.
#define PP_OD  				(1<<6)//Push-pull/Open Drain selection on interrupt pad. Default value 0. (0: push-pull; 1: open drain)
#define IHL	  				(1<<7)//Interrupt active high, low. Default value 0. (0: active high; 1: active low) 

/*
I1(2)_CFG2 	I1(2)_CFG1 	I1(2)_CFG0 			Int1(2) pad
	0 			0 			0 				GND
	0 			0 			1 				FF_WU_1
	0 			1 			0 				FF_WU_2
	0 			1 			1 				FF_WU_1 OR FF_WU_2
	1 			0 			0 				Data Ready
	1 			1 			1 				Click interrupt
*/

//**************************** 
 
//**************************** 
//FF_WU_CFG_1 BITS 
#define	XLIE				(1<<0)//Enable interrupt generation on X low event. Default value: 0 (0: disable interrupt request; 1: enable interrupt request on measured accel. value lower than preset threshold) 
#define XHIE				(1<<1)//Enable interrupt generation on X high event. Default value: 0 (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold) 
#define YLIE				(1<<2)//Enable interrupt generation on Y low event. Default value: 0 (0: disable interrupt request; 1: enable interrupt request on measured accel. value lower than preset threshold) 
#define YHIE 				(1<<3)//Enable interrupt generation on Y high event. Default value: 0 (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold) 
#define ZLIE 				(1<<4)//Enable interrupt generation on Z low event. Default value: 0 (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold) 
#define ZHIE  				(1<<5)//Enable interrupt generation on Z high event. Default value: 0 (0: disable interrupt request; 1: enable interrupt request on measured accel. value lower than preset threshold) 
#define LIR  				(1<<6)//Latch Interrupt request into FF_WU_SRC reg with the FF_WU_SRC reg cleared by reading FF_WU_SRC_1 reg. Default value: 0 
#define AOI	  				(1<<7)//And/Or combination of Interrupt events. Default value: 0 (0: OR combination of interrupt events; 1: AND combination of interrupt events) 
//**************************** 
 
//**************************** 
//Read only register.
//FF_WU_SRC_1 BITS 
#define	XL					(1<<0)//X Low. Default value: 0 (0: no interrupt, 1: XL event has occurred) 
#define XH					(1<<1)//X High. Default value: 0 (0: no interrupt, 1: XH event has occurred) 
#define YL					(1<<2)//Y Low. Default value: 0 (0: no interrupt, 1: YL event has occurred) 
#define YH 					(1<<3)//Y High. Default value: 0 (0: no interrupt, 1: YH event has occurred)
#define ZL 					(1<<4)//Z Low. Default value: 0 (0: no interrupt; 1: ZL event has occurred)
#define ZH 					(1<<5)//Z High. Default value: 0 (0: no interrupt, 1: ZH event has occurred) 
#define IA 					(1<<6)//Interrupt Active. Default value: 0 (0: no interrupt has been generated; 1: one ore more interrupt has been generated) 

//**************************** 
//CLICK_CFG BITS 
#define	Single_X			(1<<0) 
#define Double_X			(1<<1) 
#define	Single_Y			(1<<2) 
#define Double_Y			(1<<3) 
#define	Single_Z			(1<<4) 
#define Double_Z			(1<<5) 


//定义I2C总线时钟的延时值，要根据实际情况修改，取值1～255

//SCL信号周期约为(I2C_DELAY_VALUE*4+15)个机器周期

#define I2C_DELAY_VALUE   12

//定义I2C总线停止后在下一次开始之前的等待时间，取值1～65535

//等待时间约为(I2C_STOP_WAIT_VALUE*8)个机器周期

//对于多数器件取值为1即可；但对于某些器件来说，较长的延时是必须的

#define I2C_STOP_WAIT_VALUE 1200

//I2C总线初始化，使总线处于空闲状态

extern void I2C_Init();

//I2C总线综合发送函数，向从机发送多个字节的数据

extern UINT8 I2C_Puts(UINT8 SlaveAddr,UINT32 SubAddr,UINT8 SubMod,UINT8 *dat,UINT32 Size);

//I2C总线综合接收函数，从从机接收多个字节的数据

extern UINT8 I2C_Gets(UINT8 SlaveAddr,UINT32 SubAddr,UINT8 SubMod,UINT8 *dat,UINT32 Size);

extern void InitGSENSOR();

extern unsigned char i2c_read(unsigned char sub_addr);
extern unsigned char i2c_write(unsigned char sub_addr, unsigned char buff);
extern unsigned char g_i2c_write_byte(unsigned char data);
extern void g_i2c_start(void);
extern void g_i2c_stop(void);

extern BOOL GSensor_GMA301_Init(void);
extern BOOL GSensor_GMA301_GetStatus(UINT32 sensitivity);
extern void GSensor_GMA301_Open_ParkingMode(void);
extern BOOL GSensor_GMA301_Read_Interrupte(void);
#endif // __LIS33DE_H

