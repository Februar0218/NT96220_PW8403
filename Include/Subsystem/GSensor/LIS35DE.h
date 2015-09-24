#ifndef _LIS35DE_H_
#define _LIS35DE_H_  


typedef struct _GSENSOR_PIN_DEF
{
    UINT32  uigpio_sen;                 ///< GPIO pin number for SEN, valid when uimode == ENDE_CTRL_MODE_GPIO
    UINT32  uigpio_clk;                 ///< GPIO pin number for Clock, valid when uimode == ENDE_CTRL_MODE_GPIO
    UINT32  uigpio_data;                ///< GPIO pin number for Data, valid when uimode == ENDE_CTRL_MODE_GPIO
} GSENSOR_PIN_DEF, *PGSENSOR_PIN_DEF;


typedef struct _GSENSOR_DEVICE_OBJ
{
    GSENSOR_PIN_DEF  GSensorCtrl_Pin;                ///
    void          (*fpPostAdjusement)(void);    ///
} GSENSOR_DEVICE_OBJ, *PGSENSOR_DEVICE_OBJ;

#if 1
//模拟I2C总线的引脚定义

//I2C control pin
#define I2C_SCL_OUT				gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT)
#define I2C_SCL_IN				gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk, GPIO_DIR_INPUT)
#define I2C_SCL_HIGH			gpio_setPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk)
#define I2C_SCL_LOW				gpio_clearPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk)
#define I2C_SCL_DETECT			gpio_getPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk)

#define I2C_SDA_OUT				gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT)
#define I2C_SDA_IN				gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data, GPIO_DIR_INPUT)
#define I2C_SDA_HIGH 			gpio_setPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data)
#define I2C_SDA_LOW  			gpio_clearPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data)
#define I2C_SDA_DETECT			gpio_getPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data)

#define ERROR_CODE_TRUE			0
#define ERROR_CODE_FALSE		1
#define ERROR_CODE_WRITE_ADDR	10
#define ERROR_CODE_WRITE_DATA	20
#define ERROR_CODE_READ_ADDR	30
#define ERROR_CODE_READ_DATA	40
#define ERROR_CODE_START_BIT	50
#define ERROR_CODE_APROCESS		60
#define ERROR_CODE_DENY			70



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
#define DEVICEADRR			0x38
//#define DEVICEADRR			0x3A

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

extern void InitGSENSOR(GSENSOR_DEVICE_OBJ *gsensordeviceobj);

extern unsigned char i2c_read(unsigned char device_addr, unsigned char sub_addr);
extern unsigned char i2c_write(unsigned char device_addr, unsigned char sub_addr, unsigned char buff);
extern unsigned char g_i2c_write_byte(unsigned char data);
extern void g_i2c_start(void);
extern void g_i2c_stop(void);
extern void GSensorSensitivity(UINT32 level);
#else
extern void GSensor_OpenI2C(void);
extern void GSensor_CloseI2C(void);
extern void GSensor_WriteReg(UINT32 addr, UINT32 value);
extern UINT32 GSensor_ReadReg(UINT32 addr);
extern void GSensor_Init(void);
#endif
#endif //_LIS35DE_H_