/**
    Header file for UART

    This file is the header file for UART driver

    @file       uart.h
    @ingroup    mIIOUART
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

/**
    @addtogroup mIIOUART
*/
//@{

#ifndef _UART_H
#define _UART_H

#include "Type.h"

/**
    UART baud rate value

    This definition is used in uart_initHW() to specify the used UART Baudrate.
*/
typedef enum
{
    UART_BAUDRATE_2400,     ///< Baud rate 2400
    UART_BAUDRATE_4800,     ///< Baud rate 4800
    UART_BAUDRATE_9600,     ///< Baud rate 9600
    UART_BAUDRATE_19200,    ///< Baud rate 19200
    UART_BAUDRATE_38400,    ///< Baud rate 38400
    UART_BAUDRATE_57600,    ///< Baud rate 57600
    UART_BAUDRATE_115200,   ///< Baud rate 115200
    UART_BAUDRATE_1500000,  ///< Baud rate 1500000
    //#NT#2012/02/13#Klins Chen -begin
    UART_BAUDRATE_3000000,  ///< Baud rate 3000000
    //#NT#2012/02/13#Klins Chen -end

    UART_FIRPRESCALER3,     ///< UART2 PRESCALER = 3, for test)
    UART_FIRPRESCALER6,     ///< for FIR, UART2 : 48/6 = 8(MHZ), => output rate : 4 Mbps)

    ENUM_DUMMY4WORD(UART_BAUDRATE)
}UART_BAUDRATE;


/**
    UART character length and stop bits

    This definition is used in uart_initHW() to specify the used UART bit length and the stop bit number.
*/
typedef enum
{
    UART_LEN_L5_S1,         ///< Character length 5, stop bits 1
    UART_LEN_L6_S1,         ///< Character length 6, stop bits 1
    UART_LEN_L7_S1,         ///< Character length 7, stop bits 1
    UART_LEN_L8_S1,         ///< Character length 8, stop bits 1
    UART_LEN_L5_S1P5,       ///< Character length 5, stop bits 1.5
    UART_LEN_L6_S2,         ///< Character length 6, stop bits 2
    UART_LEN_L7_S2,         ///< Character length 7, stop bits 2
    UART_LEN_L8_S2,         ///< Character length 8, stop bits 2

    ENUM_DUMMY4WORD(UART_LENGTH)
}UART_LENGTH;

/**
    UART parity selection

    This definition is used in uart_initHW() to specify the used parity selection.
*/
typedef enum
{
    UART_PARITY_NONE,       ///< No parity checking
    UART_PARITY_EVEN,       ///< Even parity
    UART_PARITY_ODD,        ///< Odd parity
    UART_PARITY_ONE,        ///< One parity
    UART_PARITY_ZERO,       ///< Zero parity

    ENUM_DUMMY4WORD(UART_PARITY)
}UART_PARITY;

/**
    UART flow control mode
*/
typedef enum
{
    UART_FC_MODE_NONE,      ///< UART doesn't support any flow control.
    UART_FC_MODE_HW_ON,     ///< UART support hardware flow control, and turn it on.
    UART_FC_MODE_HW_OFF,    ///< UART support hardware flow control, and turn it off.
    UART_FC_MODE_SW_ON,     ///< UART support software flow control (GPIO simulation), and turn it on.
    UART_FC_MODE_SW_OFF,    ///< UART support software flow control (GPIO simulation), and turn it off.

    ENUM_DUMMY4WORD(UART_FC_MODE)
}UART_FC_MODE;

/**
    UART flow control pin number.

    Only for flow control by GPIO emulation.
*/
typedef struct
{
    UINT32      uiRts;        ///< RTS pin number
    UINT32      uiCts;        ///< CTS pin number
}UART_FC_PIN_NUM;

/**
    UART2 pinmux pin select
*/
typedef enum
{
    UART_PINMUX_GPIO,     ///< NO UART2 pin, all GPIO
    UART_PINMUX_2PIN,     ///< UART2 TX, RX, others are GPIO
    UART_PINMUX_4PIN,     ///< UART2 TX, RX, CTS, RTS, others are GPIO
    UART_PINMUX_8PIN,     ///< all UART2 pin, including modem pins

    ENUM_DUMMY4WORD(UART_PINMUX_NUM)
}UART_PINMUX_NUM;

/**
    UART data transfer mode.

    NOTE:Only supports PIO for now, DMA doesn't ready yet.
*/
typedef enum
{
    UART_DATA_TRANS_PIO,    ///< PIO mode, Only use in buffer mode.
    UART_DATA_TRANS_DMA,    ///< DMA mode, Only use in buffer mode.

    ENUM_DUMMY4WORD(UART_DATA_TRAN_MODE)
}UART_DATA_TRAN_MODE;

/**
    UART operate data buffer information.
*/
typedef struct
{
    volatile UINT32 uiSize;     ///< Buffer size, it must be larger than 0.
    UINT8*      pBuffer;        ///< Buffer pointer,
                                ///< it used to get data from RX FIFO to buffer or
                                ///< put data from buffer to TX FIFO.
}UART_BUFFER_INFO;

/**
    UART Event ID.
*/
typedef enum
{
    UART_EVENT_OK_PUT,          ///< UART put data is finish successfully.
    UART_EVENT_OK_GET,          ///< UART get data is finish successfully.
    UART_EVENT_FORCE_STOP_PUT,  ///< UART put data is stopped by user.
    UART_EVENT_FORCE_STOP_GET,  ///< UART get data is stopped by user.
    UART_EVENT_OVERRUN_ERROR,   ///< UART overrun error.
    UART_EVENT_PARITY_ERROR,    ///< UART parity error.
    UART_EVENT_FRAMING_ERROR,   ///< UART framing error.
    UART_EVENT_BREAK_INTERRUPT, ///< UART break interrupt.

    ENUM_DUMMY4WORD(UART_EVENT_ID)
}UART_EVENT_ID;

/**
    UART Event Handler definition.
*/
typedef void (*UART_EH)(UART_EVENT_ID);

/**
    UART init parameters
*/
typedef struct
{
    UART_BAUDRATE           BaudRate;       ///< Baudrate = Source Clock / (PSR x DL x 16)
    UART_LENGTH             Length;         ///< Character length and stop bits
    UART_PARITY             Parity;         ///< Parity
    UART_FC_MODE            FlowCtrlMode;   ///< Flow control mode
    UART_FC_PIN_NUM         FcPinNum;       ///< SW Flow control pin num (GPIO)
    UART_PINMUX_NUM         PinmuxNum;      ///< Top pinmux gpio & UART2 pin select
    UART_DATA_TRAN_MODE     DataTransMode;  ///< Data transfer mode in UART operate data transmission.
    UART_EH                 EventHandler;   ///< Event handler, if you don't use this, please assign NULL.
} UART_INIT_PARA;

/**
    UART operate data action.
*/
typedef enum
{
    UART_ACT_NONE,      ///< Do nothing, driver use only.
    UART_ACT_PUT,       ///< Put data to uart.
    UART_ACT_GET,       ///< Gte data from uart.

    ENUM_DUMMY4WORD(UART_ACTION)
} UART_ACTION;

/**
    UART operate data parameter.
*/
typedef struct
{
    UART_ACTION         Action;         ///< Get or Put
    UART_BUFFER_INFO    BufferInfo;     ///< Buffer information.
} UART_DATA_PARA;


/**
    @name   UART interrupt status

    interrupt status value for uart_checkIntStatus()
*/
//@{
#define UART_INT_STATUS_DATA_READY          0x00000001      ///< Received data ready
#define UART_INT_STATUS_OVERRUN_ERROR       0x00000002      ///< Overrun
#define UART_INT_STATUS_PARITY_ERROR        0x00000004      ///< Parity error
#define UART_INT_STATUS_FRAMING_ERROR       0x00000008      ///< Do not have valid stop bit
#define UART_INT_STATUS_BREAK_INTERRUPT     0x00000010      ///< Receiver's line input RX held at zero for a character time
#define UART_INT_STATUS_THR_EMPTY           0x00000020      ///< Non-FIFO: THR is empty, FIFO: FIFO is empty
#define UART_INT_STATUS_TX_EMPTY            0x00000040      ///< Both THR (or FIFO) and TSR are empty
#define UART_INT_STATUS_RX_FIFO_ERROR       0x00000080      ///< RX FIFO error
//@}


/*--------------------UART API-------------------------*/
extern ER    uart_open(void);
extern ER    uart_close(void);
extern ER    uart_putChar(CHAR cData);
extern ER    uart_getChar(CHAR* pcData);
extern ER    uart_putString(CHAR* pString);
extern ER    uart_getString(CHAR *pcString, CHAR *pcBufferLen);
extern ER    uart_initHW(UART_INIT_PARA*   pUartInitPara);

__inline void uart_init(UART_BAUDRATE BaudRate, UART_LENGTH Length, UART_PARITY Parity)
{
    UART_INIT_PARA UartInitPara;

    UartInitPara.BaudRate = BaudRate;
    UartInitPara.Length = Length;
    UartInitPara.Parity = Parity;
    UartInitPara.FlowCtrlMode = UART_FC_MODE_NONE;
    UartInitPara.DataTransMode = UART_DATA_TRANS_PIO;
    UartInitPara.EventHandler = NULL;

    uart_initHW(&UartInitPara);
}

__inline void uart_115200Setting(UINT uiDLAB0,UINT uiDLAB1)
{
    UART_INIT_PARA UartInitPara;

    UartInitPara.BaudRate = UART_BAUDRATE_115200;
    UartInitPara.Length = UART_LEN_L8_S1;
    UartInitPara.Parity = UART_PARITY_NONE;
    UartInitPara.FlowCtrlMode = UART_FC_MODE_NONE;
    UartInitPara.DataTransMode = UART_DATA_TRANS_PIO;
    UartInitPara.EventHandler = NULL;

    uart_initHW(&UartInitPara);
}

extern void     uart_putHex(UINT8 inData);
extern void     uart_putReg(UINT32 inReg);
extern UINT32   uart_write(UINT8* pBuffer, UINT32 Length);
extern UINT32   uart_read(UINT8* pBuffer, UINT32 Length);
extern ER       uart_operateData(UART_DATA_PARA* pDataPara);
extern UINT32   uart_stopData(UART_ACTION Action);
extern void     uart_waitData(UART_ACTION Action);
extern ER       uart_chkChar(CHAR* pcData);

extern UINT32  uart_checkIntStatus(void);


/*--------------------UART2 API-------------------------*/
extern ER     uart2_open(void);
extern ER     uart2_close(void);
extern ER     uart2_putChar(CHAR cData);
extern ER     uart2_getChar(CHAR* pcData);
extern ER     uart2_putString(CHAR* pString);
extern ER     uart2_getString(CHAR *pcString, CHAR *pcBufferLen);
extern void   uart2_putHex(UINT8 inData);
extern void   uart2_putReg(UINT32 inReg);
extern UINT32 uart2_write(UINT8* pBuffer, UINT32 Length);
extern UINT32 uart2_read(UINT8* pBuffer, UINT32 Length);
extern ER     uart2_operateData(UART_DATA_PARA* pDataPara);
extern UINT32 uart2_stopData(UART_ACTION Action);
extern void   uart2_waitData(UART_ACTION Action);
extern ER     uart2_initHW(UART_INIT_PARA*   pUartInitPara);

extern UINT32  uart2_checkIntStatus(void);

__inline void uart2_init(UART_BAUDRATE BaudRate, UART_LENGTH Length, UART_PARITY Parity)
{
    UART_INIT_PARA UartInitPara;

    UartInitPara.BaudRate = BaudRate;
    UartInitPara.Length = Length;
    UartInitPara.Parity = Parity;
    UartInitPara.FlowCtrlMode = UART_FC_MODE_NONE;
    UartInitPara.PinmuxNum = UART_PINMUX_2PIN;
    UartInitPara.DataTransMode = UART_DATA_TRANS_PIO;
    UartInitPara.EventHandler = NULL;

    uart2_initHW(&UartInitPara);
}

__inline void uart2_115200Setting(UINT uiDLAB0,UINT uiDLAB1)
{
    UART_INIT_PARA UartInitPara;

    UartInitPara.BaudRate = UART_BAUDRATE_115200;
    UartInitPara.Length = UART_LEN_L8_S1;
    UartInitPara.Parity = UART_PARITY_NONE;
    UartInitPara.FlowCtrlMode = UART_FC_MODE_NONE;
    UartInitPara.DataTransMode = UART_DATA_TRANS_PIO;
    UartInitPara.EventHandler = NULL;

    uart2_initHW(&UartInitPara);
}


//@}

#endif // _UART_H

