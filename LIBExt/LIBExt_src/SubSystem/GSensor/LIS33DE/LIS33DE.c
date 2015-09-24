
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
#include "debug.h"
#include "I2c.h"
#include "Gsensor.h"
#include "spi.h"
#include "LIS33DE.h"

#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

static INT32 GsXData = 0,OldGsXData = 0;
static INT32 GsYData = 0,OldGsYData = 0;
static INT32 GsZData = 0,OldGsZData = 0;
#define moving_step 4
#define SENSOR_DATA_SIZE 		3 
INT32	bufferave[3][moving_step];	// 16 buffer
INT32	sum[3];		//  sum of moving_step 
INT32	avg[3];		// moving average result

static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

#define I2C_DELAY delay(400)
#define I2C_DELAY_LONG delay(2000)

//-----------------------------------BIT1612-----------------------------


/* ------------------------------------
    Function Prototypes
   ------------------------------------ */

/* -------------------------------------------------------------------
    Name: I2C_Delay -
    Purpose: .
    Passed: None.
    Returns:
    Notes:
  ------------------------------------------------------------------- */
void I2C_Delay (UW16 Time)
{
//0000 DFFE              DJNZ    R7,?C0001
//0002         ?C0002:
//  bTime >>= 2;

    while (--Time)
        ;
} /* I2C_Delay */



/* -------------------------------------------------------------------
    Name: I2C_GetACK -
    Purpose: .
    Passed: None.
    Returns: LOW if OK
    Notes:
  ------------------------------------------------------------------- */
BOOL I2C_GetACK (void)
{
    BOOL    fResult;


    //I2C_SET_SDA(HIGH);      // Added by JC 10:31PM  2010/04/03
    BIT1216M_I2C_SDA_HIGH;
    I2C_Delay(I2C_SU_DAT);

    fResult = BIT1216M_I2C_SDA_DETECT;
	//debug_err(("fResult----%d\r\n",fResult));
    //fResult = I2C_GET_SDA;

    //I2C_SET_SCL(HIGH);
    BIT1216M_I2C_SCL_HIGH;
    I2C_Delay(I2C_HIGH);

    //I2C_SET_SCL(LOW);
    BIT1216M_I2C_SCL_LOW;
    I2C_Delay(I2C_LOW);

    return( fResult );
} /* I2C_GetACK */


/* -------------------------------------------------------------------
    Name: I2C_Init -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
  ------------------------------------------------------------------- */
void BIT_I2C_Init (void)
{
    	//I2C_SET_SDA(HIGH);
    	//I2C_SET_SCL(HIGH);
	BIT1216M_I2C_SCL_OUT;
	BIT1216M_I2C_SDA_OUT;
	
    	BIT1216M_I2C_SDA_HIGH;
    	BIT1216M_I2C_SCL_HIGH;
} /* I2C_Init */


#if (I2C_RX_BURST)
/* -------------------------------------------------------------------
    Name: I2C_RxBurst -
    Purpose: To receive bulk data from I2C slave device.
    Passed:
        bIdx: 1..255. (Excludes slave address and sub-address)
    Returns: None.
    Notes:
        The bSLA should be the slave device's 'WRITE' module addres,
        not 'READ' one.
   ------------------------------------------------------------------- */
void I2C_RxBurst (
UB8  bSLA,          /* I2C slave address */
UB8  bREG,          /* I2C sub-address */
UB8  bCNT,          /* The number of data which will be transmitted */
UB8  *pbDATA        /* Point to the first DATA item */
)
{
    UB8     bIdx;


    // START
    I2C_Start();

    /* --------------------------------
        Write
       -------------------------------- */
    // MAD for WRITE
    I2C_TxData(bSLA);
    I2C_GetACK();                     

    // SAD
    I2C_TxData(bREG);
    I2C_GetACK();


    /* --------------------------------
        Read
       -------------------------------- */
    I2C_Start();                       
    
    // MAD for READ
    I2C_TxData(bSLA | 0x01);
    I2C_GetACK();


    for (bIdx = 0; bIdx < (bCNT - 1); bIdx++)
    {
        *pbDATA++ = I2C_RxData();

        I2C_SetACK();
    } /* for */


    /* --------------------------------
        Last Read
       -------------------------------- */
    *pbDATA = I2C_RxData();

    // GetNAK
    I2C_SetNAK();

    // STOP
    I2C_Stop();
} /* I2C_RxBurst */
#endif


#if (I2C_RX_BYTE)
/* -------------------------------------------------------------------
    Name: I2C_RxByte -
    Purpose: To receive one byte data from I2C slave device.
    Passed:
        UB8 bSLA,          I2C slave address
        UB8 bREG           I2C sub-address

    Returns:
        One byte received data.
    Notes:
        The bSLA should be the slave device's 'WRITE' mode address,
        not 'READ' one.
   ------------------------------------------------------------------- */
UB8 I2C_RxByte (
UB8 bSLA,          /* I2C slave address */
UB8 bREG           /* I2C sub-address */
)
{
    UB8     bData;


    I2C_RxBurst(bSLA, bREG, 1, &bData);

    return( bData );
} /* I2C_RxByte */
#endif


/* -------------------------------------------------------------------
    Name: I2C_RxData -
    Purpose:
        To do I2C parallel serial conversion for reception.
    Passed: None.
    Returns:
    Notes:
  ------------------------------------------------------------------- */
UB8 I2C_RxData (void)
{
    UB8     bResult;
    UB8     bMask;


    bResult = 0x00;                 /* Clear value */

    // Release SDA
    //I2C_SET_SDA(HIGH);
    BIT1216M_I2C_SDA_HIGH;


    /* MSB is read first */
    for (bMask = 0x80; bMask; bMask >>= 1)
    {
        //I2C_SET_SCL(HIGH);
        BIT1216M_I2C_SCL_HIGH;
        I2C_Delay(I2C_HIGH);

        //if ( I2C_GET_SDA )
        if (BIT1216M_I2C_SDA_DETECT)
            bResult |= bMask;

        //I2C_SET_SCL(LOW);
        BIT1216M_I2C_SCL_LOW;
        I2C_Delay(I2C_LOW);         // Added by JC 06:31PM  2011/01/12
    } /* for */

    return( bResult );
} /* I2C_RxData */


#if (I2C_RX_WORD)
/* -------------------------------------------------------------------
    Name: I2C_RxWord -
    Purpose: To receive one WORD data from I2C slave device.
    Passed:
    Returns: one WORD data.
    Notes:
        1) Only for little-endian storage format devices !!!
        2) Keil C51 uses *big-endian* storage format. [1]144

   ------------------------------------------------------------------- */
UW16 I2C_RxWord (
UB8 bSLA,                       /* I2C slave address */
UB8 bREG                        /* I2C sub-address */
)
{
    UW16    wDATA;


    I2C_RxBurst(bSLA, bREG, 2, &wDATA);

    return( wDATA );
} /* I2C_RxWord */
#endif


#if (I2C_SET_ACK)
/* -------------------------------------------------------------------
    Name: I2C_SetACK -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_SetACK (void)
{
   // I2C_SET_SDA(LOW);
    BIT1216M_I2C_SDA_LOW;
    I2C_Delay(I2C_SU_DAT);

    //I2C_SET_SCL(HIGH);
    BIT1216M_I2C_SCL_HIGH;
    I2C_Delay(I2C_HIGH);

    //I2C_SET_SCL(LOW);
    BIT1216M_I2C_SCL_LOW;
    I2C_Delay(I2C_LOW);

    //I2C_SET_SDA(HIGH);      // Added by JC 09:32AM  2010/04/04
    BIT1216M_I2C_SDA_HIGH;
} /* I2C_SetACK */
#endif


/* -------------------------------------------------------------------
    Name: I2C_SetNAK -
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_SetNAK (void)
{
    //I2C_SET_SDA(HIGH);
    BIT1216M_I2C_SDA_HIGH;
    I2C_Delay(I2C_SU_DAT);

    //I2C_SET_SCL(HIGH);
    BIT1216M_I2C_SCL_HIGH;
    I2C_Delay(I2C_HIGH);

    //I2C_SET_SCL(LOW);
    BIT1216M_I2C_SCL_LOW;
    I2C_Delay(I2C_LOW);
} /* I2C_SetNAK */


/* -------------------------------------------------------------------
    Name: I2C_Start - START condition (SDA falling edge).
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
    Reference:
        [1] p64
   ------------------------------------------------------------------- */
void I2C_Start (void)
{
    //I2C_SET_SDA(HIGH);
    BIT1216M_I2C_SDA_HIGH;

    //I2C_SET_SCL(HIGH);
    BIT1216M_I2C_SCL_HIGH;
    I2C_Delay(I2C_SU_STA);

    //I2C_SET_SDA(LOW);
    BIT1216M_I2C_SDA_LOW;
    I2C_Delay(I2C_HD_STA);

    //I2C_SET_SCL(LOW);
    BIT1216M_I2C_SCL_LOW;
    I2C_Delay(I2C_LOW);
} /* I2C_Start */


/* -------------------------------------------------------------------
    Name: I2C_Stop - STOP condition (SDA rising edge).
    Purpose: .
    Passed: None.
    Returns: None.
    Notes:
    Reference:
        [1] p64

  ------------------------------------------------------------------- */
void I2C_Stop (void)
{
    //I2C_SET_SDA(LOW);
    BIT1216M_I2C_SDA_LOW;
    //I2C_SET_SCL(HIGH);
    BIT1216M_I2C_SCL_HIGH;
    I2C_Delay(I2C_SU_STO);

    //I2C_SET_SDA(HIGH);
    BIT1216M_I2C_SDA_HIGH;
} /* I2C_Stop */


#if (I2C_TX_BURST)
/* -------------------------------------------------------------------
    Name: I2C_TxBurst -
    Purpose: To transmit bulk data to I2C slave device.
    Passed:
        bCNT: 1..255. (Excludes slave address and sub-address)
    Returns: None.
    Notes:
        The bSLA should be the slave device's 'WRITE' module addres,
        not 'READ' one.
   ------------------------------------------------------------------- */
void I2C_TxBurst (
UB8  bSLA,          /* I2C slave address */
UB8  bREG,          /* I2C sub-address */
UB8  bCNT,          /* The number of data which will be transmitted */
UB8  *pbDATA        /* Point to the first DATA item */
)
{
    UB8 bIdx;


    // START
    I2C_Start();

    // MAD
    I2C_TxData(bSLA);

    // GetACK
    I2C_GetACK();

    // SAD (SAD[7:0])
    I2C_TxData(bREG);

    // GetACK
    I2C_GetACK();

    for (bIdx = 0; bIdx < bCNT; bIdx++)
    {
        I2C_TxData(*pbDATA++);
	 //TimerDelayUs(30);
        // GetACK
        I2C_GetACK();
    } /* for */

    // STOP
    I2C_Stop();
} /* I2C_TxBurst */
#endif


#if (I2C_TX_BYTE)
/* -------------------------------------------------------------------
    Name: I2C_TxByte -
    Purpose: To transmit one byte data to I2C slave device.
    Passed:
    Returns: None.
    Notes:
        The bSLA should be the slave device's 'WRITE' module addres,
        not 'READ' one.
   ------------------------------------------------------------------- */
void I2C_TxByte (
UB8 bSLA,          /* I2C slave address */
UB8 bREG,          /* I2C sub-address */
UB8 bDATA          /* DATA item */
)
{
    I2C_TxBurst(bSLA, bREG, 1, &bDATA);
} /* I2C_TxByte */
#endif


/* -------------------------------------------------------------------
    Name: I2C_TxData -
    Purpose: To do I2C parallel serial conversion for transmission.
    Passed:
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_TxData (UB8 bData)
{
    UB8     bMask;


    /* MSB is sent first */
    for (bMask = 0x80; bMask; bMask >>= 1)
    {
        //I2C_SET_SDA(bData & bMask);
        if (bData & bMask)
	{
		BIT1216M_I2C_SDA_HIGH;
	}
	else
	{
		BIT1216M_I2C_SDA_LOW;
	}
        
        I2C_Delay(I2C_SU_DAT);

        //I2C_SET_SCL(HIGH);
        BIT1216M_I2C_SCL_HIGH;
        I2C_Delay(I2C_HIGH);

        //I2C_SET_SCL(LOW);
        BIT1216M_I2C_SCL_LOW;
        I2C_Delay(I2C_LOW);
    }

    //I2C_SET_SDA(HIGH);      // Added by JC 01:47PM  2005/11/23
    BIT1216M_I2C_SDA_HIGH;
} /* I2C_TxData */


#if (I2C_TX_REPEAT)
/* -------------------------------------------------------------------
    Name: I2C_TxRepeat -
    Purpose: To transmit the same data to I2C slave device repeatly via I2C.
    Passed:
        UB8  bSLA   = I2C slave address.
        UW16 wREG   = I2C register address.
        UB8  bCNT   = The number of data which will be transmitted
            excluding slave and register address (bCNT: 1..255).
        UB8  bDATA  = The repeated data.
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_TxRepeat (
UB8  bSLA,          /* BITEK slave address */
UW16 wREG,          /* BITEK register address */
UB8  bCNT,          /* The number of data which will be transmitted */
UB8  bDATA          /* The repeated DATA */
)
{
    UB8 bIdx;

    if (bCNT == 0)
        return;

    I2C_Start();

    I2C_TxData(bSLA | (((wREG) >> 7) & 0x1E));

    I2C_GetACK();


    I2C_TxData(wREG);
    I2C_GetACK();


    /* --------------------------------
        Write Data
       -------------------------------- */
    for (bIdx = bCNT; bIdx; bIdx--)
    {
        I2C_TxData(bDATA);
        I2C_GetACK();
    } /* for */


    I2C_Stop();
} /* I2C_TxRepeat */
#endif


#if (I2C_TX_WORD)
/* -------------------------------------------------------------------
    Name: I2C_TxWord -
    Purpose: To transmit bulk data to I2C slave device.
    Passed:
    Returns: None.
    Notes:
   ------------------------------------------------------------------- */
void I2C_TxWord (
UB8  bSLA,          /* I2C slave address */
UB8  bREG,          /* I2C sub-address */
UW16 wDATA          /* DATA item */
)
{
    I2C_TxBurst(bSLA, bREG, 2, (UB8 *)&wDATA);
} /* I2C_TxWord */
#endif

//-----------------------------------BIT1612-----------------------------
void I2C_Init()
{
	debug_err(("<<301=====================I2C_Init=======================>>\r\n"));

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
	//g_i2c_ack_send();
	return data;
}

unsigned char i2c_write(unsigned char sub_addr, unsigned char buff)
{
	unsigned char i;
	unsigned char device_addr = GMA301_WRITE_ID;

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

unsigned char i2c_read(unsigned char sub_addr)
{
	unsigned char i;
	unsigned char buff;
	unsigned char device_addr = GMA301_WRITE_ID;
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

//GSENSOR_DEVICE_OBJ GSensor_DeviceObj;

void InitGSENSOR()
{
    //GSensor_DeviceObj = *gsensordeviceobj;	
       gpio_setDir(GPIO_GSENSOR_SCK, GPIO_DIR_OUTPUT);
       gpio_setDir(GPIO_GSENSOR_SDA, GPIO_DIR_OUTPUT);

	gpio_setPin(GPIO_GSENSOR_SCK);
	gpio_setPin(GPIO_GSENSOR_SDA);

	I2C_Init();
}

/*return value: 0: is ok    other: is failed*/
int     i2c_read_byte_data( unsigned char addr, unsigned char *data){
	*data =  (unsigned char)i2c_read(addr);
	return 0;
}

/*return value: 0: is ok    other: is failed*/
int     i2c_write_byte_data( unsigned char addr, unsigned char data){
	i2c_write(addr,data);
	return 0;
}

/*return value: count: is ok    other: is failed*/
int     i2c_read_block_data( unsigned char base_addr, unsigned char count, unsigned char *data){
	int i = 0;
		
	for(i = 0; i < count;i++)
	{
		*data =  (unsigned char)i2c_read(base_addr+i);
               data ++;
	}	
		
	return count;
}

int GMA301_register_read( unsigned char addr, unsigned char *data){
    int     res = 0;

    res = i2c_read_byte_data(addr, data);
    if(res != 0) {
          return res;
    }	

    return res;
}

int GMA301_register_write( unsigned char addr, unsigned char data){
    int     res = 0;

    res = i2c_write_byte_data(addr, data);
    if(res != 0) {
         return res;
    }

    return res;
}

BOOL GSensor_GMA301_Init(void)
{
	unsigned char data=0;

	GMA301_register_read(0X12,&data);
	if(data != 0x55){
	       debug_msg("------GMA301 read chip id  error= %x-----\r\n",data); 
		return -1;
	}

	debug_msg("------GMA301 chip id = %x-----\r\n",data); 

	//init
	GMA301_register_write(0x21,0x52);//
	GMA301_register_write(0x00,0x02);//
	GMA301_register_write(0x00,0x12);//
	GMA301_register_write(0x00,0x02);//
	GMA301_register_write(0x00,0x82);//
	GMA301_register_write(0x00,0x02);//
	GMA301_register_write(0x1F,0x28);//
	GMA301_register_write(0x0C,0x8F);//
	GMA301_register_write(0x00,0x06);//

	//interrupt setup
#if 1		
	GMA301_register_write(0x11,0x04);//IIC 0X07 for no pullup //0x06 High active  0x40 low active
	//set Gsensor Level
	//0x08-->0.5G
	//0X10-->1G
	GMA301_register_write(0x38,0Xff);//
	GMA301_register_write(0x39,0Xff);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g  (8 : 0.5g)

	GMA301_register_write(0x0F,0x00);//
	GMA301_register_write(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
	GMA301_register_write(0x1F,0x28);//To disable micro motion interrupt.
	//TimerDelayMs(10);
	//Latched reference data of micro motion.
	i2c_read(0x12);
	i2c_read(0x13);
	i2c_read(0x14);
	i2c_read(0x15);
	i2c_read(0x16);
	i2c_read(0x17);
	i2c_read(0x18);
	i2c_read(0x19);
#endif
	GMA301_register_write(0x1F,0x38);//To enable micro motion interrupt.
	TimerDelayMs(1); //2014_0819 added 1ms delay for micro motion setup itself.
	i2c_read(0x1C);
	i2c_read(0x1D);
	GMA301_register_write(0x0E,0x00);//To disable interrupt.//parking monitor

    	return TRUE;
}

BOOL GSensor_GMA301_GetStatus(UINT32 sensitivity)
{
	UINT8  abs_cal_x=0,abs_cal_y=0,abs_cal_z=0;
	UINT8 G_INT_COUNT=0;
	UINT32 Threshold = 0xFF;
       UINT32 g_X_Data_L=0,g_X_Data_H=0,g_Y_Data_L=0,g_Y_Data_H=0,g_Z_Data_L=0,g_Z_Data_H=0;

	Threshold = sensitivity;

	i2c_read(0x12);
	i2c_read(0x13);

	g_X_Data_L=i2c_read(OUT_X_L_REG);
	g_Y_Data_L=i2c_read(OUT_Y_L_REG);
	g_Z_Data_L=i2c_read(OUT_Z_L_REG);

	g_X_Data_H=i2c_read(OUT_X_H_REG);
	g_Y_Data_H=i2c_read(OUT_Y_H_REG);
	g_Z_Data_H=i2c_read(OUT_Z_H_REG);


      if((g_X_Data_H&0x03)>=0x02)
		GsXData =-(1023-((g_X_Data_L +((g_X_Data_H&0x03)<<8))));
	else
		GsXData =(g_X_Data_L +((g_X_Data_H&0x03)<<8));

      if((g_Y_Data_H&0x3)>=0x02)
		GsYData =-(1023-(g_Y_Data_L +((g_Y_Data_H&0x03)<<8)));
	else
		GsYData =(g_Y_Data_L +((g_Y_Data_H&0x03)<<8));

      if((g_Z_Data_H&0x3)>=0x02)
		GsZData =-(1023-(g_Z_Data_L +((g_Z_Data_H&0x03)<<8)));
	else
		GsZData =(g_Z_Data_L +((g_Z_Data_H&0x03)<<8));

	abs_cal_x =abs(GsXData-OldGsXData);
	if(abs_cal_x>Threshold)   //30
	    G_INT_COUNT++;

	abs_cal_y =abs(GsYData-OldGsYData);
	if(abs_cal_y>Threshold) //20
	    G_INT_COUNT++;

	abs_cal_z =abs(GsZData-OldGsZData);
	if(abs_cal_z>Threshold) //20
	    G_INT_COUNT++;

	OldGsXData = GsXData;
	OldGsYData = GsYData;
	OldGsZData = GsZData;

     //debug_msg("..........X = %d,Y = %d,Z = %d\r\n",GsXData,GsYData,GsZData);
     //debug_msg("..........DX = %d,DY = %d,DZ = %d th=%d \r\n",abs_cal_x,abs_cal_y,abs_cal_z,Threshold);

	if(G_INT_COUNT>=2)
	{
		debug_msg("----------GMA301_isr EMG----------\n\r");
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void GSensor_GMA301_Open_ParkingMode(void)
{
	//interrupt setup
	GMA301_register_write(0x11,0x04);//IIC 0X07 for no pullup //0x06 High active  0x40 low active
	//Gsensor_WriteReg(0x11,0x06);//IIC 0X06 for no pullup
	//set Gsensor Level
	//0x08-->0.5G
	//0X10-->1G
	GMA301_register_write(0x38,0XFF);//
	GMA301_register_write(0x39,0X10);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g  (8 : 0.5g)

	GMA301_register_write(0x0F,0x00);//
	GMA301_register_write(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
	GMA301_register_write(0x1F,0x28);//To disable micro motion interrupt.
	TimerDelayMs(3);
	//Latched reference data of micro motion.
	i2c_read(0x12);
	i2c_read(0x13);
	i2c_read(0x14);
	i2c_read(0x15);
	i2c_read(0x16);
	i2c_read(0x17);
	i2c_read(0x18);
	i2c_read(0x19);
	
	GMA301_register_write(0x1F,0x38);//To enable micro motion interrupt.
	TimerDelayMs(2);
	i2c_read(0x1C);
	i2c_read(0x1D);		
	GMA301_register_write(0x0E,0x0C);//0x1C//0x00 // 0x00:disable interrupt

	debug_msg("GMA301 Enter parking mode..\r\n");
}

BOOL GSensor_GMA301_Read_Interrupte(void)
{
	UINT32 Value;
	i2c_read(0x12);
      	Value=i2c_read(0x13);
	debug_msg("====interrupt status = %x..\r\n",Value);


	if((Value&0x20)==0x20)
    	{
		debug_msg("----------GMA301_isr EMG----------\n\r");
		return TRUE;
    	}
    	else
	{
		debug_msg("----------GMA301_isr--22--EMG----------\n\r");	
		return FALSE;
	}
}


