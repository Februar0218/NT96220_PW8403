#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "Utility.h"
#include "CK235.h"
#include "debug.h"

#if 1 //PW01

#if 1
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

//delay time
#define I2C_DELAY delay(400)
#define I2C_DELAY_LONG delay(3000)

//static variable
static ENDE_DEVICE_OBJ ENDE_DeviceObj;

void InitEnDe(ENDE_DEVICE_OBJ *endedeviceobj)
{
    ENDE_DeviceObj = *endedeviceobj;
	
    gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

	gpio_setPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk);
	gpio_setPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data);	
}

static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

void _i2c_start(void)
{

	DEBUG_MSG(("<<=====================_i2c_start=======================>>\r\n"));
	SDA_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	SCL_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	SDA_LOW;
	I2C_DELAY;
	SCL_LOW;
	I2C_DELAY;
}

void _i2c_stop(void)
{
	
	DEBUG_MSG(("<<=====================_i2c_stop=======================>>\r\n"));
	
	SDA_LOW;
	I2C_DELAY;
	SCL_HIGH;
	I2C_DELAY;
	SDA_HIGH;
	I2C_DELAY_LONG;
}

unsigned char _i2c_ack_detect(void)
{

	DEBUG_MSG(("<<=====================_i2c_ack_detect=======================>>\r\n"));

	SDA_HIGH;
	I2C_DELAY;
	SDA_IN; // SDA Input Mode
	SCL_HIGH;
	I2C_DELAY;

	if (SDA_DETECT)
	{
		SDA_OUT;
		DEBUG_MSG(("<<=====================Ack Error!=======================>>\r\n"));
		return ERROR_CODE_FALSE; // false
	}

	I2C_DELAY;
	SCL_LOW;
	SDA_OUT;
	return ERROR_CODE_TRUE; // true
}

void _i2c_ack_send(void)
{

	DEBUG_MSG(("<<=====================_i2c_ack_send=======================>>\r\n"));

	SDA_LOW;
	I2C_DELAY;
	SCL_HIGH;
	I2C_DELAY;
	SCL_LOW;
	I2C_DELAY;
}
unsigned char _i2c_write_byte(unsigned char data)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_write_byte=======================>>\r\n"));
	I2C_DELAY;
	for(i = 0; i< 8; i++)
	{
		if( (data << i) & 0x80) SDA_HIGH;
		else SDA_LOW;
		I2C_DELAY;
		SCL_HIGH;
		I2C_DELAY;
		SCL_LOW;
		I2C_DELAY;
	}

	if(_i2c_ack_detect())
	{
		DEBUG_MSG(("<<=====================Acknoledge Error=======================>>\r\n"));
		return ERROR_CODE_FALSE;
	}
	return ERROR_CODE_TRUE;
}

unsigned char _i2c_read_byte(void)
{
	unsigned char i, data;

	DEBUG_MSG(("<<=====================_i2c_read_byte=======================>>\r\n"));

	data = 0;
	//SDA_HIGH;
	I2C_DELAY;
	SDA_IN;
	for(i = 0; i< 8; i++)
	{
		data <<= 1;
		I2C_DELAY;
		SCL_HIGH;
		I2C_DELAY;
		if (SDA_DETECT) data |= 0x01;
		SCL_LOW;
		I2C_DELAY;
	}
	SDA_OUT;
	_i2c_ack_send();
	return data;
}

unsigned char _i2c_write(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_write=======================>>\r\n"));

	_i2c_start();
	I2C_DELAY;
	if(_i2c_write_byte(device_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	if(_i2c_write_byte(sub_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	for(i = 0; i<ByteNo; i++) {
		if(_i2c_write_byte(buff[i])) {
			_i2c_stop();
			DEBUG_MSG(("<<=====================Write Error - Data=======================>>\r\n"));
			return ERROR_CODE_WRITE_DATA;
		}
	}
	I2C_DELAY;
	_i2c_stop();
	I2C_DELAY_LONG;
	return ERROR_CODE_TRUE;
}

unsigned char _i2c_read(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_read=======================>>\r\n"));	
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;
	I2C_DELAY_LONG;	
	_i2c_start();
	I2C_DELAY;
	if(_i2c_write_byte(device_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	if(_i2c_write_byte(sub_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	_i2c_start();
	I2C_DELAY;
	if(_i2c_write_byte(device_addr+1)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	for(i = 0; i<ByteNo; i++) buff[i] = _i2c_read_byte();
	
	I2C_DELAY;
	_i2c_stop();
	I2C_DELAY_LONG;
	return ERROR_CODE_TRUE;
}


#else
#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

//delay time
#define I2C_DELAY delay(400)
#define I2C_DELAY_LONG delay(2000)

//static variable
static ENDE_DEVICE_OBJ ENDE_DeviceObj;
static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}



void InitEnDe(ENDE_DEVICE_OBJ *endedeviceobj)
{
    ENDE_DeviceObj = *endedeviceobj;
	
    gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

	gpio_setPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk);
	gpio_setPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data);	
}



void _i2c_start(void)
{

	DEBUG_MSG(("<<=====================_i2c_start=======================>>\r\n"));
	SDA_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	SCL_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	SDA_LOW;
	I2C_DELAY;
	SCL_LOW;
	I2C_DELAY;
}

void _i2c_stop(void)
{
	
	DEBUG_MSG(("<<=====================_i2c_stop=======================>>\r\n"));
	
	SDA_LOW;
	I2C_DELAY;
	SCL_HIGH;
	I2C_DELAY;
	SDA_HIGH;
	I2C_DELAY_LONG;
}

unsigned char _i2c_ack_detect(void)
{

	DEBUG_MSG(("<<=====================_i2c_ack_detect=======================>>\r\n"));

	SDA_HIGH;
	I2C_DELAY;
	SDA_IN; // SDA Input Mode
	SCL_HIGH;
	I2C_DELAY_LONG;//Jason
	I2C_DELAY_LONG;

	if (SDA_DETECT)
	{
		SDA_OUT;
		DEBUG_MSG(("<<=====================Ack Error!=======================>>\r\n"));
		return ERROR_CODE_FALSE; // false
	}

	I2C_DELAY;
	SCL_LOW;
	I2C_DELAY;
	SDA_OUT;
	return ERROR_CODE_TRUE; // true
}

void _i2c_ack_send(void)
{

	DEBUG_MSG(("<<=====================_i2c_ack_send=======================>>\r\n"));

	SDA_LOW;
	I2C_DELAY;
	SCL_HIGH;
	I2C_DELAY;
	SCL_LOW;
	I2C_DELAY;
}
unsigned char _i2c_write_byte(unsigned char data)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_write_byte=======================>>\r\n"));
	I2C_DELAY;
	for(i = 0; i< 8; i++)
	{
		if( (data << i) & 0x80) SDA_HIGH;
		else SDA_LOW;
		I2C_DELAY;
		SCL_HIGH;
		I2C_DELAY;
		SCL_LOW;
		I2C_DELAY;
	}

	if(_i2c_ack_detect())
	{
		DEBUG_MSG(("<<=====================Acknoledge Error=======================>>\r\n"));
		return ERROR_CODE_FALSE;
	}
	return ERROR_CODE_TRUE;
}

unsigned char _i2c_read_byte(void)
{
	unsigned char i, data;

	DEBUG_MSG(("<<=====================_i2c_read_byte=======================>>\r\n"));

	data = 0;
	//SDA_HIGH;
	I2C_DELAY;
	SDA_IN;
	for(i = 0; i< 8; i++)
	{
		data <<= 1;
		I2C_DELAY;
		SCL_HIGH;
		I2C_DELAY_LONG;
		I2C_DELAY_LONG;
		if (SDA_DETECT) data |= 0x01;
		SCL_LOW;
		I2C_DELAY;
	}
	SDA_OUT;
	_i2c_ack_send();
	return data;
}

unsigned char _i2c_write(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_write=======================>>\r\n"));

	_i2c_start();
	I2C_DELAY;
	if(_i2c_write_byte(device_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	if(_i2c_write_byte(sub_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	for(i = 0; i<ByteNo; i++) {
		if(_i2c_write_byte(buff[i])) {
			_i2c_stop();
			DEBUG_MSG(("<<=====================Write Error - Data=======================>>\r\n"));
			return ERROR_CODE_WRITE_DATA;
		}
	}
	I2C_DELAY;
	_i2c_stop();
	I2C_DELAY_LONG;
	return ERROR_CODE_TRUE;
}

unsigned char _i2c_read(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_read=======================>>\r\n"));	
	I2C_DELAY_LONG;
	_i2c_start();
	I2C_DELAY;
	if(_i2c_write_byte(device_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	if(_i2c_write_byte(sub_addr)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	_i2c_start();
	I2C_DELAY;
	if(_i2c_write_byte(device_addr+1)) {
		_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	for(i = 0; i<ByteNo; i++) buff[i] = _i2c_read_byte();
	
	I2C_DELAY;
	_i2c_stop();
	I2C_DELAY_LONG;
	return ERROR_CODE_TRUE;
}
#endif
