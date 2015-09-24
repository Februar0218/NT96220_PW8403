
#ifndef __LIS331DL_H
#define __LIS331DL_H

#define WHO_AM_I            0x0F
#define CTRL_REG1           0x20
#define CTRL_REG2           0x21
#define CTRL_REG3           0x22
#define HP_FILTER_RESET     0x23
#define STATUS_REG          0x27
#define OUT_X               0x29
#define OUT_Y               0x2B
#define OUT_Z               0x2D
#define FF_WU_CFG_1         0x30
#define FF_WU_SRC_1         0x31
#define FF_WU_THS_1         0x32
#define FF_WU_DURATION_1    0x33


#define LIS331DL_Dev        0x3B

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


extern GSENSOR_FUNC LIS331DLFP;


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

#endif // __LIS331DL_H

