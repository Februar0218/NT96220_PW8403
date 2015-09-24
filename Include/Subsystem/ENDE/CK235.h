#ifndef _CK235_H_
#define _CK235_H_

/***********************************************************************************
 *#defines
 ***********************************************************************************/

//I2C control pin
#define SCL_OUT				gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT)
#define SCL_IN				gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk, GPIO_DIR_INPUT)
#define SCL_HIGH			gpio_setPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk)
#define SCL_LOW				gpio_clearPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_clk)

#define SDA_OUT				gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT)
#define SDA_IN				gpio_setDir(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data, GPIO_DIR_INPUT)
#define SDA_HIGH 			gpio_setPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data)
#define SDA_LOW  			gpio_clearPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data)
#define SDA_DETECT			gpio_getPin(ENDE_DeviceObj.ENDECtrl_Pin.uigpio_data)

#define ERROR_CODE_TRUE			0
#define ERROR_CODE_FALSE		1
#define ERROR_CODE_WRITE_ADDR	10
#define ERROR_CODE_WRITE_DATA	20
#define ERROR_CODE_READ_ADDR	30
#define ERROR_CODE_READ_DATA	40
#define ERROR_CODE_START_BIT	50
#define ERROR_CODE_APROCESS		60
#define ERROR_CODE_DENY			70


typedef struct _ENDE_PIN_DEF
{
    UINT32  uigpio_sen;                 ///< GPIO pin number for SEN, valid when uimode == ENDE_CTRL_MODE_GPIO
    UINT32  uigpio_clk;                 ///< GPIO pin number for Clock, valid when uimode == ENDE_CTRL_MODE_GPIO
    UINT32  uigpio_data;                ///< GPIO pin number for Data, valid when uimode == ENDE_CTRL_MODE_GPIO
} ENDE_PIN_DEF, *PENDE_PIN_DEF;


typedef struct _ENDE_DEVICE_OBJ
{
    ENDE_PIN_DEF  ENDECtrl_Pin;                ///
    void          (*fpPostAdjusement)(void);    ///
} ENDE_DEVICE_OBJ, *PENDE_DEVICE_OBJ;


/***********************************************************************************
 * Variables
 ***********************************************************************************/

/***********************************************************************************
 * External Variables
 ***********************************************************************************/

/***********************************************************************************
 * Function Prototypes
 ***********************************************************************************/
void _i2c_start(void);
void _i2c_stop(void);
void _i2c_ack_send(void);
unsigned char _i2c_ack_detect(void);
unsigned char _i2c_write_byte(unsigned char);
unsigned char _i2c_read_byte(void);

/***********************************************************************************
 * External Function
 ***********************************************************************************/
extern unsigned char _i2c_write(unsigned char , unsigned char, unsigned char *, int);
extern unsigned char _i2c_read(unsigned char , unsigned char, unsigned char *, int);
extern void InitEnDe(ENDE_DEVICE_OBJ *endedeviceobj);

#endif //_CK235_H_
