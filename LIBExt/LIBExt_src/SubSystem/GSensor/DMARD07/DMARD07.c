
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
#include "DMARD07.h"

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

#define DMARD07_DEV_WRITE_ADDRESS	0x4e
#define abs(x) (((x) < 0) ? -(x) : (x))

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
	debug_err(("<<380=====================I2C_Init=======================>>\r\n"));

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
	unsigned char device_addr = DMARD07_DEV_WRITE_ADDRESS;

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
	unsigned char device_addr = DMARD07_DEV_WRITE_ADDRESS;
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

void InitGSENSOR(GSENSOR_DEVICE_OBJ *gsensordeviceobj)
{
    //GSensor_DeviceObj = *gsensordeviceobj;
       gpio_setDir(GPIO_GSENSOR_SCK, GPIO_DIR_OUTPUT);
       gpio_setDir(GPIO_GSENSOR_SDA, GPIO_DIR_OUTPUT);

	gpio_setPin(GPIO_GSENSOR_SCK);
	gpio_setPin(GPIO_GSENSOR_SDA);
#if 0	
    gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

	gpio_setPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_clk);
	gpio_setPin(GSensor_DeviceObj.GSensorCtrl_Pin.uigpio_data);
#endif
	I2C_Init();
}

void DMARD07_WriteReg(UINT32 addr, UINT32 value)
{
	i2c_write((unsigned char)addr, (unsigned char)value);
}

unsigned char DMARD07_ReadReg(UINT32 addr)
{
	unsigned char value;
	
	value = i2c_read((unsigned char)addr);
	debug_err(("DMARD07___________ReadReg(0x%02X) = 0x%02X\r\n",addr,value));
	return value;
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

int mir3da_register_read( unsigned char addr, unsigned char *data){
    int     res = 0;

    res = i2c_read_byte_data(addr, data);
    if(res != 0) {
          return res;
    }	

    return res;
}

int mir3da_register_write( unsigned char addr, unsigned char data){
    int     res = 0;

    res = i2c_write_byte_data(addr, data);
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_read_continuously( unsigned char addr, unsigned char count, unsigned char *data)
{
    int     res = 0;

    res = (count==i2c_read_block_data(addr, count, data)) ? 0 : 1;
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_mask_write(unsigned char addr, unsigned char mask, unsigned char data){
    int     res = 0;
    unsigned char      tmp_data;

    res = mir3da_register_read(addr, &tmp_data);
    if(res) {
        return res;
    }

    tmp_data &= ~mask; 
    tmp_data |= data & mask;
    res = mir3da_register_write(addr, tmp_data);

    return res;
}

/*int mir3da_i2c_init(void)
{
	 GSENSOR_INFO GsensorInfo;  
	 GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE; 
	 GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;  
	 GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;    
	 GsensorInfo.I2C_Slave_WAddr = 0x4e;   
	 GsensorInfo.I2C_Slave_RAddr = 0x4f;   

	 if (GSensor_I2C_Init(GsensorInfo) == TRUE)    {     
	 	debug_msg("------mir3da i2c init OK-----\r\n"); 
		return 0;
	 }

	 return -1;
}*/

/*return value: 0: is ok    other: is failed*/
BOOL mir3da_chip_init(void){
	int             res = 0;
	unsigned char data=0;

/*
	if(mir3da_i2c_init()){
		return -1;
		debug_msg("------mir3da i2c init error-----\r\n"); 
	}
*/
	mir3da_register_read(NSA_REG_WHO_AM_I,&data);
	if(data != 0x13){
	       debug_msg("------mir3da read chip id  error= %x-----\r\n",data); 
		return -1;
	}

	debug_msg("------mir3da chip id = %x-----\r\n",data); 

	res =  mir3da_register_mask_write(NSA_REG_SPI_I2C, 0x24, 0x24);

	delay(5);

	res |= mir3da_register_mask_write(NSA_REG_G_RANGE, 0x03, 0x02);
	res |= mir3da_register_mask_write(NSA_REG_POWERMODE_BW, 0xFF, 0x1E);
	res |= mir3da_register_mask_write(NSA_REG_ODR_AXIS_DISABLE, 0xFF, 0x07);
	
	res |= mir3da_register_mask_write(NSA_REG_INT_LATCH, 0x8F, 0x8F);
	
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x83);
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x69);
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0xBD);

	return res;	    	
}


int mir3da_open_interrupt(int num){
	int   res = 0;

	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x03);
	res = mir3da_register_write(NSA_REG_ACTIVE_DURATION,0x03);
	res = mir3da_register_write(NSA_REG_ACTIVE_THRESHOLD,0x07);//0x30//DEBUG//38
			
	switch(num){

		case 0:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x04 );
			break;

		case 1:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x04 );
			break;
	}

	return res;
}

int mir3da_close_interrupt(int num){
	int   res = 0;

	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x00 );
			
	switch(num){

		case 0:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x00 );
			break;

		case 1:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x00 );
			break;
	}

	return res;
}

/*return value: 1:has intterupt     0: no intterupt*/
int mir3da_read_interrupt_statu(){

	unsigned char data=0;

	mir3da_register_read(NSA_REG_MOTION_FLAG,&data);

	if(data&0x04)
		return 1;

	return 0;
}

/*return value: 0: is ok    other: is failed*/
int mir3da_read_data(short *x, short *y, short *z)
{
    unsigned char    tmp_data[6] = {0};

    if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, tmp_data) != 0) {
        return -1;
    }
    
    *x = ((short)(tmp_data[1] << 8 | tmp_data[0]))>> 4;
    *y = ((short)(tmp_data[3] << 8 | tmp_data[2]))>> 4;
    *z = ((short)(tmp_data[5] << 8 | tmp_data[4]))>> 4;

     //debug_msg("------mir3da x= %4d y= %4d z= %4d-----\r\n",*x,*y,*z); 	

    return 0;
}

int rotate(char *rot,char *upd,short x,short y ,short z)
{
    static char old_rot = 0;

   *rot = old_rot;

/*
    if(y > 160&&abs(z) <220&&abs(x)< 220)////200//120
    {
        *rot = 0;
    }
    if(x > 160&&abs(z) <100&&abs(y)< 100)////200//120
    {
        *rot = 1;
    }
    if(-y > 160&&abs(z) <220&&abs(x)< 220)////200//120
    {
        *rot = 2;
    }
    if(-x > 160&&abs(z) <120&&abs(y)< 100)////200//120
    {
        *rot = 3;
    }
    */
/*
    if(y > (-30)&&abs(z) <200&&abs(x)< 120)////200//120
    {
        *rot = 0;
    }
    if(x > 160&&abs(z) <100&&abs(y)< 100)////200//120
    {
        *rot = 1;
    }
    if(-y > 160&&abs(z) <120&&abs(x)< 120)////200//120
    {
        *rot = 2;
    }
    if(-x > 160&&abs(z) <120&&abs(y)< 100)////200//120
    {
        *rot = 3;
    }
    */

    if((z < (-150))&&(z > (-250))&&(abs(y)< 60)&&(x < (-20)))////200//120
    {
        *rot = 0;
    }
    if((z > 160)&&(z < 360)&&(abs(y)< 60)&&(x < (-20)))////200//120
    {
        *rot = 2;
    }
    if(old_rot != *rot)
    {
        old_rot = *rot;
        *upd = 1;
    }
    else
    {
        *upd = 0;
    }

    //debug_msg("x=%4d y=%4d z=%4d rot=%4d upd=%4d\r\n",x,y,z,*rot,*upd);

}


BOOL DMARD07_GsensorInit(void)
{
	UINT32 ReadValue;


	ReadValue = 0;
	ReadValue = DMARD07_ReadReg(SW_RESET);
	debug_err(("G sensor sw reset = %02X\r\n", ReadValue));
	
	ReadValue = 0;
	ReadValue = DMARD07_ReadReg(WHO_AM_I);
	debug_err(("G sensor Who am I = %02X\r\n", ReadValue));
	
	// init setting
	// Low Power:32Hz, XYZ enable 
	DMARD07_WriteReg(0x44, 0x47);
	// xy enable
	//DMARD07_WriteReg(0x44, 0x46);
	DMARD07_ReadReg(0x44);
	
	// 2G mode, High Pass Filter for INT1, Low pass filter for data
	//DMARD07_WriteReg(0x45, 0x24);
	// 2G mode, High Pass Filter for INT1, High pass filter for data
	DMARD07_WriteReg(0x45, 0x14);
	// 2G mode, Low Pass Filter for INT1, High pass filter for data
	//DMARD07_WriteReg(0x45, 0x10);
	
	//  High-pass Filter Cutoff for 0.6 Hz 
	DMARD07_WriteReg(0x46, 0x00);
	
	// No latch, INT SRC1 enable, active 1 
	DMARD07_WriteReg(0x47, 0x04);
	// No latch, INT SRC1 enable, active 0
	//DMARD07_WriteReg(0x47, 0x44);
	// latch int 1, int 2, Int SRC1 enable
	//DMARD07_WriteReg(0x47, 0x34);
	// latch int 1
	//DMARD07_WriteReg(0x47, 0x24);
	
	DMARD07_WriteReg(0x48, 0x00);
	
	//  OR all interrupt events, High-G XYZ 
	// xyz +G
	//DMARD07_WriteReg(0x4A, 0x2A);
	// xyz -G
	DMARD07_WriteReg(0x4A, 0x15);	
	// xy +-G
	//DMARD07_WriteReg(0x4A, 0x3c);
	// xy +G
	//DMARD07_WriteReg(0x4A, 0x28);
	// xy -G
	//DMARD07_WriteReg(0x4A, 0x14);
	// xyz +-G
	//DMARD07_WriteReg(0x4A, 0x3f);
	
	// Threshold = 755.9 mg 
	DMARD07_WriteReg(0x4C, 0x30);
	//  Duration = 47.1 ms 
	DMARD07_WriteReg(0x4D, 0x10);
}

void GSensorSensitivity(UINT32 level)
{
    switch(level)
	{
		case 0:
			DMARD07_WriteReg(0x44, 0x00);
			break;
		case 1:
			DMARD07_WriteReg(0x44, 0x47);
			// Threshold = 755.9 mg 
			DMARD07_WriteReg(0x4C, 0x70);
			//  Duration = 47.1 ms 
			DMARD07_WriteReg(0x4D, 0x10);
			debug_err(("GSensorSensitivity()==================>1\r\n"));
			break;
		case 2:
			DMARD07_WriteReg(0x44, 0x47);
			// Threshold = 755.9 mg 
			DMARD07_WriteReg(0x4C, 0x50);
			//  Duration = 47.1 ms 
			DMARD07_WriteReg(0x4D, 0x10);
			debug_err(("GSensorSensitivity()==================>2\r\n"));
			break;
		case 3:
			DMARD07_WriteReg(0x44, 0x47);
			// Threshold = 755.9 mg 
			DMARD07_WriteReg(0x4C, 0x30);
			//  Duration = 47.1 ms 
			DMARD07_WriteReg(0x4D, 0x10);				
			debug_err(("GSensorSensitivity()==================>3\r\n"));
			break;
		default:
			break;
	}
}

void GSensorSensitivityReset(UINT32 level)
{
 	DMARD07_ReadReg(WHO_AM_I);
}

void gsensor_getFP(PGSENSOR_FUNC pGsensorFunc)
{
    memcpy(pGsensorFunc,&DMARD07FP,sizeof(GSENSOR_FUNC));
}

GSENSOR_FUNC DMARD07FP = {
    mir3da_chip_init,        // GsensorInit
    NULL,     					// GetGsensorData
    NULL,   					// GetGsensorStatus
    NULL,      					// OpenInterface
    NULL,     					// closeInterface
};


