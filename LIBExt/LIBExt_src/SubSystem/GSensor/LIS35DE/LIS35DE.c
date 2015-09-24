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
#include "LIS35DE.h"
//#include "LIS35DE_REG.h"
#include "debug.h"
#include "I2c.h"

#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

#if 1

//delay time
#define I2C_DELAY delay(400)
#define I2C_DELAY_LONG delay(2000)
UINT8 I2C_GetAck();

//static variable
static GSENSOR_DEVICE_OBJ GSensor_DeviceObj;

void InitGSENSOR(GSENSOR_DEVICE_OBJ *gsensordeviceobj)
{
    GSensor_DeviceObj = *gsensordeviceobj;
	
    gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

	gpio_setPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk);
	gpio_setPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data);

	I2C_Init();
#if 1
	#if 0
	i2c_write(DEVICEADRR,CTRL_REG1,0xef);
	test = i2c_read(DEVICEADRR, CTRL_REG1);
	debug_err(("test ===>%x\r\n",test));
	i2c_write(DEVICEADRR,CTRL_REG2,0x5e);
	test = i2c_read(DEVICEADRR, CTRL_REG2);
	debug_err(("test ===>%x\r\n",test));
	i2c_write(DEVICEADRR,CTRL_REG3,0x63);
	test = i2c_read(DEVICEADRR, CTRL_REG3);
	debug_err(("test ===>%x\r\n",test));

	i2c_write(DEVICEADRR,FF_WU_CFG_1,0x6a);
	test = i2c_read(DEVICEADRR, FF_WU_CFG_1);
	debug_err(("test ===>%x\r\n",test));
	i2c_write(DEVICEADRR,FF_WU_THS_1,0x0f);
	test = i2c_read(DEVICEADRR, FF_WU_THS_1);
	debug_err(("test ===>%x\r\n",test));
	i2c_write(DEVICEADRR,FF_WU_DURATION_1,0x8f);
	test = i2c_read(DEVICEADRR, FF_WU_DURATION_1);
	debug_err(("test ===>%x\r\n",test));
	#endif

	//i2c_write(DEVICEADRR,CTRL_REG2, BOOT | FDS | HP_FF_WU1 | HP_coeff1);
	//i2c_write(DEVICEADRR,CTRL_REG3, I1CFG1 | I1CFG0);
	
	#if 0
	i2c_write(DEVICEADRR,FF_WU_CFG_1, LIR | ZHIE | YHIE | XHIE);
	i2c_write(DEVICEADRR,FF_WU_THS_1,0x04);
	i2c_write(DEVICEADRR,FF_WU_DURATION_1,0x8f);
	#else
	i2c_write(DEVICEADRR,FF_WU_CFG_1, LIR | ZHIE | YHIE | XHIE);//0x65
	i2c_write(DEVICEADRR,FF_WU_THS_1,0x03);
	i2c_write(DEVICEADRR,FF_WU_DURATION_1,0x8f);
	#endif
	i2c_write(DEVICEADRR,CTRL_REG1, DR | PD | FS | ZEN | YEN | XEN);
#endif

}


void GSensorSensitivity(UINT32 level)
{
    switch(level)
    {
    case 0:
		i2c_write(DEVICEADRR,FF_WU_CFG_1,0x6a);
		i2c_write(DEVICEADRR,FF_WU_THS_1,0x05);
		i2c_write(DEVICEADRR,FF_WU_DURATION_1,0x8f);

        break;
    case 1:
		i2c_write(DEVICEADRR,FF_WU_CFG_1,0x6a);
		i2c_write(DEVICEADRR,FF_WU_THS_1,0x03);
		i2c_write(DEVICEADRR,FF_WU_DURATION_1,0x8f);

        break;
    case 2:
		i2c_write(DEVICEADRR,FF_WU_CFG_1,0x6a);
		i2c_write(DEVICEADRR,FF_WU_THS_1,0x01);
		i2c_write(DEVICEADRR,FF_WU_DURATION_1,0x8f);

        break;
    default:
		i2c_write(DEVICEADRR,FF_WU_CFG_1,0x6a);
		i2c_write(DEVICEADRR,FF_WU_THS_1,0x03);
		i2c_write(DEVICEADRR,FF_WU_DURATION_1,0x8f);

        break;
    }

}

/*
  delay time period

  @param times : time

*/
static void delay(UINT32 times)
{
	#if 0
    int i,j;
      
    for(i=0;i<times * 5;i++)
    {  
        for(j=0;j<47;j++)
        {;}
    }
	#else
    for(;times > 0; times--)
        ;
	#endif
}

/*

函数：I2C_Init()

功能：I2C总线初始化，使总线处于空闲状态

说明：在main()函数的开始处，通常应当要执行一次本函数

*/

void I2C_Init()
{
	DEBUG_MSG(("<<=====================I2C_Init=======================>>\r\n"));

	I2C_SCL_OUT;

	I2C_SDA_OUT;

	I2C_SCL_HIGH;
	
	I2C_DELAY;
	
	I2C_SDA_HIGH;
	
	I2C_DELAY;

}

void g_i2c_start(void)
{

	DEBUG_MSG(("<<=====================_i2c_start=======================>>\r\n"));
	I2C_SDA_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	I2C_SCL_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	I2C_SDA_LOW;
	I2C_DELAY;
	I2C_SCL_LOW;
	I2C_DELAY;
}

void g_i2c_stop(void)
{
	
	DEBUG_MSG(("<<=====================_i2c_stop=======================>>\r\n"));
	
	I2C_SDA_LOW;
	I2C_DELAY;
	I2C_SCL_HIGH;
	I2C_DELAY;
	I2C_SDA_HIGH;
	I2C_DELAY_LONG;
}

unsigned char g_i2c_ack_detect(void)
{
	int i = 0;
	DEBUG_MSG(("<<=====================_i2c_ack_detect=======================>>\r\n"));

	I2C_SDA_HIGH;
	//I2C_DELAY;
	delay(20);
	I2C_SDA_IN; // SDA Input Mode
	I2C_SCL_HIGH;

	//I2C_DELAY;
	while(i<400)
	{
		i++;
		if (I2C_SDA_DETECT)
		{
			delay(2);
		}
		else
		{
			break;
		}
	}
	if (I2C_SDA_DETECT)
	{
		I2C_SDA_OUT;
		DEBUG_MSG(("<<=====================Ack Error!=======================>>\r\n"));
		return ERROR_CODE_FALSE; // false
	}

	I2C_DELAY;
	I2C_SCL_LOW;
	I2C_SDA_OUT;
	return ERROR_CODE_TRUE; // true
}

void g_i2c_ack_send(void)
{

	DEBUG_MSG(("<<=====================_i2c_ack_send=======================>>\r\n"));

	I2C_SDA_LOW;
	I2C_DELAY;
	I2C_SCL_HIGH;
	I2C_DELAY;
	I2C_SCL_LOW;
	I2C_DELAY;
}
unsigned char g_i2c_write_byte(unsigned char data)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_write_byte=======================>>\r\n"));
	I2C_DELAY;
	for(i = 0; i< 8; i++)
	{
		if( (data << i) & 0x80) I2C_SDA_HIGH;
		else I2C_SDA_LOW;
		I2C_DELAY;
		I2C_SCL_HIGH;
		I2C_DELAY;
		I2C_SCL_LOW;
		I2C_DELAY;
	}

	if(g_i2c_ack_detect())
	{
		DEBUG_MSG(("<<=====================Acknoledge Error=======================>>\r\n"));
		return ERROR_CODE_FALSE;
	}
	return ERROR_CODE_TRUE;
}

unsigned char g_i2c_read_byte(void)
{
	unsigned char i, data;

	DEBUG_MSG(("<<=====================_i2c_read_byte=======================>>\r\n"));

	data = 0;
	//I2C_SDA_HIGH;
	I2C_DELAY;
	I2C_SDA_IN;
	for(i = 0; i< 8; i++)
	{
		data <<= 1;
		I2C_DELAY;
		I2C_SCL_HIGH;
		I2C_DELAY;
		if (I2C_SDA_DETECT) data |= 0x01;
		I2C_SCL_LOW;
		I2C_DELAY;
	}
	I2C_SDA_OUT;
	g_i2c_ack_send();
	return data;
}

unsigned char i2c_write(unsigned char device_addr, unsigned char sub_addr, unsigned char buff)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_write=======================>>\r\n"));

	g_i2c_start();
	I2C_DELAY;
	if(g_i2c_write_byte(device_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	if(g_i2c_write_byte(sub_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	if(g_i2c_write_byte(buff)) 
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Data=======================>>\r\n"));
		return ERROR_CODE_WRITE_DATA;
	}

	I2C_DELAY;
	g_i2c_stop();
	I2C_DELAY_LONG;
	return ERROR_CODE_TRUE;
}

unsigned char i2c_read(unsigned char device_addr, unsigned char sub_addr)
{
	unsigned char i;
	unsigned char buff;
	DEBUG_MSG(("<<=====================_i2c_read=======================>>\r\n"));	

	g_i2c_start();
	I2C_DELAY;
	if(g_i2c_write_byte(device_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	if(g_i2c_write_byte(sub_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	g_i2c_start();
	I2C_DELAY;
	if(g_i2c_write_byte(device_addr+1))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	
	buff = g_i2c_read_byte();

	I2C_DELAY;
	g_i2c_stop();
	I2C_DELAY_LONG;
	return buff;
}







#if 0
/*

函数：I2C_Start()

功能：产生I2C总线的起始状态

说明：

SCL处于高电平期间，当SDA出现下降沿时启动I2C总线

不论SDA和SCL处于什么电平状态，本函数总能正确产生起始状态

本函数也可以用来产生重复起始状态

本函数执行后，I2C总线处于忙状态

*/

	

void I2C_Start()
{
	DEBUG_MSG(("<<=====================I2C_Start=======================>>\r\n"));

	I2C_SDA_HIGH;
	
	I2C_DELAY;
	
	I2C_SCL_HIGH;
	
	I2C_DELAY;
	
	I2C_SDA_LOW;
	
	I2C_DELAY;
	
	I2C_SCL_LOW;
	
	I2C_DELAY;

}


/*

函数：I2C_Stop()

功能：产生I2C总线的停止状态

说明：

SCL处于高电平期间，当SDA出现上升沿时停止I2C总线

不论SDA和SCL处于什么电平状态，本函数总能正确产生停止状态

本函数执行后，I2C总线处于空闲状态

*/

void I2C_Stop()
{
	UINT32 t = I2C_STOP_WAIT_VALUE;
	DEBUG_MSG(("<<=====================I2C_Stop=======================>>\r\n"));

	I2C_SDA_LOW;
	
	I2C_DELAY;
	
	I2C_SCL_HIGH;
	
	I2C_DELAY;
	
	I2C_SDA_HIGH;
	
	I2C_DELAY;	
	
while ( --t != 0 );   //在下一次产生Start之前，要加一定的延时
}

/*

函数：I2C_Write()

功能：向I2C总线写1个字节的数据

参数：

dat：要写到总线上的数据

*/

void I2C_Write(UINT8 dat)
{

	UINT8 t = 8;
	DEBUG_MSG(("<<=====================I2C_Write=======================>>\r\n"));

	for(t = 0; t< 8; t++)
	{
		if( (dat << t) & 0x80) I2C_SDA_HIGH;
		else I2C_SDA_LOW;
		
		I2C_SCL_HIGH;
		
		I2C_DELAY;
		
		I2C_SCL_LOW;
		
		I2C_DELAY;
	}

}

/*

函数：I2C_Read()

功能：从从机读取1个字节的数据

返回：读取的一个字节数据

*/

unsigned char i2c_ack_detect(void)
{

	//DEBUG_MSG(("<<=====================_i2c_ack_detect=======================>>\r\n"));

	I2C_SDA_HIGH;
	I2C_SDA_IN; // SDA Input Mode
	I2C_SCL_HIGH;
	I2C_DELAY;

	if (I2C_SDA_DETECT)
	{
		I2C_SDA_OUT;
		//DEBUG_MSG(("<<=====================Ack Error!=======================>>\r\n"));
		return ERROR_CODE_FALSE; // false
	}

	I2C_SCL_LOW;
	I2C_SDA_OUT;
	return ERROR_CODE_TRUE; // true
}

void i2c_ack_send(void)
{

	//DEBUG_MSG(("<<=====================_i2c_ack_send=======================>>\r\n"));

	I2C_SDA_LOW;
	I2C_DELAY;
	I2C_SCL_HIGH;
	I2C_DELAY;
	I2C_SCL_LOW;
	I2C_DELAY;
}
unsigned char i2c_write_byte(unsigned char data)
{
	unsigned char i;

	//DEBUG_MSG(("<<=====================_i2c_write_byte=======================>>\r\n"));
	for(i = 0; i< 8; i++)
	{
		if( (data << i) & 0x80) I2C_SDA_HIGH;
		else I2C_SDA_LOW;
		I2C_DELAY;
		I2C_SCL_HIGH;
		I2C_DELAY;
		I2C_SCL_LOW;
		I2C_DELAY;
	}

	//if(i2c_ack_detect())
	if(I2C_GetAck())	
	{
		//DEBUG_MSG(("<<=====================Acknoledge Error=======================>>\r\n"));
		return ERROR_CODE_FALSE;
	}
	return ERROR_CODE_TRUE;
}

unsigned char i2c_read_byte(void)
{
	UINT8 dat;
	
	UINT8 t = 8;
	DEBUG_MSG(("<<=====================I2C_Read=======================>>\r\n"));	

	I2C_SDA_HIGH; //在读取数据之前，要把SDA拉高

	I2C_DELAY;
	
	I2C_SDA_IN;
	
	for(t = 0; t< 8; t++)
	{
		dat <<= 1;
		I2C_DELAY;
		
		I2C_SCL_HIGH;

		I2C_SCL_IN;
		
		I2C_DELAY;
		while(!I2C_SCL_DETECT);
		
		if (I2C_SDA_DETECT) dat |= 0x01;
		
		I2C_SCL_LOW;
		
		I2C_DELAY;
	}
	
	I2C_SCL_OUT;
	I2C_SDA_OUT;

	return dat;	

}


UINT8 I2C_Read()

{
	UINT8 dat;
	
	UINT8 t = 8;
	DEBUG_MSG(("<<=====================I2C_Read=======================>>\r\n"));	

	I2C_SDA_HIGH; //在读取数据之前，要把SDA拉高

	I2C_DELAY;
	
	I2C_SDA_IN;
	
	for(t = 0; t< 8; t++)
	{
		dat <<= 1;
		I2C_DELAY;
		
		I2C_SCL_HIGH;

		I2C_SCL_IN;
		
		I2C_DELAY;
		while(!I2C_SCL_DETECT);
		
		if (I2C_SDA_DETECT) dat |= 0x01;
		
		I2C_SCL_LOW;
		
		I2C_DELAY;
	}
	
	I2C_SCL_OUT;
	I2C_SDA_OUT;

	return dat;	
}

/*

函数：I2C_GetAck()

功能：读取从机应答位

返回：

0：从机应答

1：从机非应答

说明：

从机在收到每个字节的数据后，要产生应答位

从机在收到最后1个字节的数据后，一般要产生非应答位

*/

UINT8 I2C_GetAck()

{

	UINT8 ack;
	DEBUG_MSG(("<<=====================I2C_GetAck=======================>>\r\n"));	
#if 0
	I2C_SDA = 1;

	I2C_Delay();

	I2C_SCL = 1;

	I2C_Delay();

	ack = I2C_SDA;

	I2C_SCL = 0;

	I2C_Delay();
#else
	I2C_SDA_HIGH;
	
	I2C_SCL_HIGH;
	
	I2C_SDA_IN;
	
	I2C_DELAY;
		
	ack = I2C_SDA_DETECT;

	I2C_SDA_OUT;
	
	I2C_SCL_LOW;

#endif
	return ack;

}

/*

函数：I2C_PutAck()

功能：主机产生应答位或非应答位

参数：

ack=0：主机产生应答位

ack=1：主机产生非应答位

说明：

主机在接收完每一个字节的数据后，都应当产生应答位

主机在接收完最后一个字节的数据后，应当产生非应答位

*/

void I2C_PutAck(UINT8 ack)

{
	DEBUG_MSG(("<<=====================I2C_PutAck=======================>>\r\n"));
#if 0
	I2C_SDA = ack;

	I2C_Delay();

	I2C_SCL = 1;

	I2C_Delay();

	I2C_SCL = 0;

	I2C_Delay();
#else
	if(ack)
	{
		I2C_SDA_HIGH;	
	}
	else
	{
		I2C_SDA_LOW;
	}
	
	I2C_SCL_HIGH;
	
	I2C_DELAY;
	
	I2C_SCL_LOW;
	
	I2C_SDA_HIGH;	
#endif
}


/*

函数：I2C_Puts()

功能：I2C总线综合发送函数，向从机发送多个字节的数据

参数：

SlaveAddr：从机地址（7位纯地址，不含读写位）

SubAddr：从机的子地址

SubMod：子地址模式，0－无子地址，1－单字节子地址，2－双字节子地址

*dat：要发送的数据

Size：数据的字节数

返回：

0：发送成功

1：在发送过程中出现异常

说明：

本函数能够很好地适应所有常见的I2C器件，不论其是否有子地址

当从机没有子地址时，参数SubAddr任意，而SubMod应当为0

*/

UINT8 I2C_Puts

(

UINT8 SlaveAddr,

UINT32 SubAddr,

UINT8 SubMod,

UINT8 *dat,

UINT32 Size

)

{

//定义临时变量

UINT8 i;

UINT8 a[3];
	DEBUG_MSG(("<<=====================I2C_Puts=======================>>\r\n"));
//检查长度

if ( Size == 0 ) return 0;

//准备从机地址

a[0] = (SlaveAddr << 1);

//检查子地址模式

if ( SubMod > 2 ) SubMod = 2;

//确定子地址

switch ( SubMod )

{

case 0:

   break;

case 1:

   a[1] = (UINT8)(SubAddr);

   break;

case 2:

   a[1] = (UINT8)(SubAddr >> 8);

   a[2] = (UINT8)(SubAddr);

   break;

default:

   break;

}

//发送从机地址，接着发送子地址（如果有子地址的话）

SubMod++;

I2C_Start();

for ( i=0; i<SubMod; i++ )

{

   I2C_Write(a[i]);

   if ( I2C_GetAck() )

   {

    I2C_Stop();

    return 1;

   }

}

//发送数据

do

{

   I2C_Write(*dat++);

   if ( I2C_GetAck() ) break;

} while ( --Size != 0 );

//发送完毕，停止I2C总线，并返回结果

I2C_Stop();

if ( Size == 0 )

{

   return 0;

}

else

{

   return 1;

}

}

/*

函数：I2C_Gets()

功能：I2C总线综合接收函数，从从机接收多个字节的数据

参数：

SlaveAddr：从机地址（7位纯地址，不含读写位）

SubAddr：从机的子地址

SubMod：子地址模式，0－无子地址，1－单字节子地址，2－双字节子地址

*dat：保存接收到的数据

Size：数据的字节数

返回：

0：接收成功

1：在接收过程中出现异常

说明：

本函数能够很好地适应所有常见的I2C器件，不论其是否有子地址

当从机没有子地址时，参数SubAddr任意，而SubMod应当为0

*/

UINT8 I2C_Gets

(

UINT8 SlaveAddr,

UINT32 SubAddr,

UINT8 SubMod,

UINT8 *dat,

UINT32 Size

)

{

//定义临时变量

UINT8 i;

UINT8 a[3];
	DEBUG_MSG(("<<=====================I2C_Gets=======================>>\r\n"));
//检查长度

if ( Size == 0 ) return 0;

//准备从机地址

a[0] = (SlaveAddr << 1);

//检查子地址模式

if ( SubMod > 2 ) SubMod = 2;

//如果是有子地址的从机，则要先发送从机地址和子地址

if ( SubMod != 0 )

{

//确定子地址

   if ( SubMod == 1 )

   {

    a[1] = (UINT8)(SubAddr);

   }

   else

   {

    a[1] = (UINT8)(SubAddr >> 8);

    a[2] = (UINT8)(SubAddr);

   }

//发送从机地址，接着发送子地址

   SubMod++;

   I2C_Start();

   for ( i=0; i<SubMod; i++ )

   {

    I2C_Write(a[i]);

    if ( I2C_GetAck() )

    {

     I2C_Stop();

     return 1;

    }

   }

}

//这里的I2C_Start()对于有子地址的从机是重复起始状态

//对于无子地址的从机则是正常的起始状态

I2C_Start();

//发送从机地址

I2C_Write(a[0]+1);

if ( I2C_GetAck() )

{

   I2C_Stop();

   return 1;

}

//接收数据

for (;;)

{

   *dat++ = I2C_Read();

   if ( --Size == 0 )

   {

    I2C_PutAck(1);

    break;

   }

   I2C_PutAck(0);

}

//接收完毕，停止I2C总线，并返回结果

I2C_Stop();

return 0;

}
#else
#if 0
#if 1
//SDO = 0
#define GSEN_READ_ID 0x39
#define GSEN_WRITE_ID 0x38
#else
//SDO = 1
#define GSEN_READ_ID 0x3B
#define GSEN_WRITE_ID 0x3A
#endif

const UINT32 GSensor_Param[][2] =
{
    {0x20 ,0xe7},//CTRL_REG1
    {0x21 ,0x5e},//CTRL_REG2
    {0x22 ,0x63},//CTRL_REG3
    
    {0x30 ,0x6a},//FF_WU_CFG_1
    {0x32 ,0x0f},//FF_WU_THS_1  
    {0x33 ,0x8f},//FF_WU_DURATION_1
    
    {0x34 ,0x6a},//FF_WU_CFG_2
    {0x36 ,0x0f},//FF_WU_THS_2
    {0x37 ,0x8f},//FF_WU_DURATION_2
    //Click Reg
    #if 0
    {0x38 ,0x15},//CLICK_CFG
    {0x3B ,0x77},//CLICK_THSY_X
    {0x3C ,0x07},//CLICK_THSZ
    
    {0x3D ,0x0},//CLICK_TimeLimit
    {0x3E ,0x10},//CLICK_Latency
    {0x3F ,0x50},//CLICK_Window
    #endif
};


void GSensor_OpenI2C(void)
{
    // Open I2C driver
    if (i2c_open() != E_OK)
    {
        DEBUG_MSG(("Error open I2C driver!\r\n"));
    }

    // Enable I2C controller
    i2c_enable();

    // Set pinmux to I2C
    pinmux_select_i2c(PINMUX_I2C);

    debug_err(("0x001 <= tsr <= 0x3FB\r\n"));
    debug_err(("0 <= gsr <= 7\r\n"));

    // Set I2C bus clock to 240 KHz
    // I2C bus clock = APB clock / (2*clkdiv + 4 + GSR)
    // = 48000000 / (2*96 + 4 + 4) = 240000 Hz
    if (i2c_initMaster(96, 10, 4) != I2C_SUCCESS)
	//if (i2c_initMaster(160, 7, 3) != I2C_SUCCESS)
    {
        DEBUG_MSG(("Init I2C driver fail!\r\n"));
    }

}

// For Gsensor close I2C, Add by Jason
void GSensor_CloseI2C(void)
{
    i2c_disable();

    i2c_close();
}
#if 0
void GSensor_WriteReg(UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1,ulReg2, ulData1;//, ulData2;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (addr>>8);
    ulReg2      = ((addr)&(0x000000ff));
    ulData1     = value;//(value>>8);
    //ulData2     = ((value)&(0x000000ff));

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (write addr)!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg1) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg2) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulData1, 0, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }

#if 0
        erReturn=i2c_masterTransmit(ulData2, 0, 1);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data2) !!\r\n"));
            break;
        }
#endif
    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}


UINT32 GSensor_ReadReg(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1,ulReg2, ulData, ulData1;//, ulData2;
    i2c_lock();
    ulWriteAddr =SEN_WRITE_ID;
    ulReadAddr  =SEN_READ_ID;
    ulReg1      =(addr>>8);
    ulReg2      =((addr)&(0x000000ff));
    ulData      =0;
    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);

        if (erReturn !=  I2C_SUCCESS)
        {
            debug_err(("Error transmit addr before receive!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData1, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }
#if 0
        erReturn=i2c_masterReceive(&ulData2, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data2!!\r\n"));
            break;
        }
#endif
    } while(erReturn != I2C_SUCCESS);
     ulData=ulData1;//((ulData1<<8)|(ulData2));
    i2c_unlock();
    return ulData;
}
#else
void GSensor_WriteReg(UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg, ulData;

    ulWriteAddr = GSEN_WRITE_ID;
    ulReg       = addr;
    ulData      = value;

    i2c_lock();

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            DEBUG_MSG(("Error transmit data (write addr)!!\r\n"));
            break;
        }
        erReturn=i2c_masterTransmit(ulReg, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            DEBUG_MSG(("Error transmit data (reg) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulData, 0, 1);

        if (erReturn != I2C_SUCCESS)
        {
            DEBUG_MSG(("Error transmit data (data) !!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}


UINT32 GSensor_ReadReg(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg, ulData;

    ulWriteAddr =GSEN_WRITE_ID;
    ulReadAddr  =GSEN_READ_ID;
    ulReg       =addr;

    i2c_lock();

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg, 0, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);

        if (erReturn !=  I2C_SUCCESS/*ER_I2C_AL*/)
        {
            DEBUG_MSG(("Error transmit addr before receive!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            DEBUG_MSG(("Error receive data!!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
	DEBUG_MSG(("GSensor_ReadReg addr = %d, Data = %d",addr,ulData));
    return ulData;
}
#endif


void GSensor_Init(void)
{
    UINT32 idx, table_size,tmp;

	GSensor_OpenI2C();

	if(i2c_isOpened())
	{
		DEBUG_MSG(("i2c_Opened...ok\r\n"));
	}
	else
	{
		DEBUG_MSG(("i2c_Opened...err\r\n"));
	}
    table_size = (sizeof(GSensor_Param)/2);
	
    DEBUG_MSG(("Gsensor init...\r\n"));
	
    for(idx=0;idx<table_size;idx++)
    {
        GSensor_WriteReg(GSensor_Param[idx][0], GSensor_Param[idx][1]);
    }

    DEBUG_MSG(("Gsensor init finished...\r\n"));
}
#endif
#endif
#endif
