/**
    Copyright   Novatek Microelectronics Corp. 2007. All rights reserved.

    @file       NEC.c
    @ingroup    mISYSIrRx

    @brief      NEC IR remote control protocol driver.
                This file is the driver for NEC IR remote control protocol.

    @note       Nothing.

    @version    V1.00.000
    @author     JustinLee
    @date       2007/01/22
*/

/** \addtogroup mISYSIrRx */
//@{

#include <string.h>
#include "OSCommon.h"
#include "pad.h"
#include "GPIO.h"
#include "Timer.h"
#include "Debug.h"
#include "IrRx.h"


// IR pulse width definitions (NEC protocol case), in us (micro-second) unit
#define IR_PW_LEADER_HIGH               9000
#define IR_PW_LEADER_SPACING            4500
#define IR_PW_REPEAT_SPACING            2250
#define IR_PW_DATABIT_HIGH              560
#define IR_PW_DATABIT1_SPACING          1690
#define IR_PW_DATABIT0_SPACING          560
#define IR_PW_STOPBIT_HIGH              560

// IR total data bits number in one frame
#define IR_DATA_BITS_NUM                32          // Address/Custom Code (16bits) + Command/Data Code (16bits)

// IrRx pulse width check macro, with a (15%+250us) tolerance
#define CHECK_PULSE_WIDTH(x, t_us)      (((x) > (t_us)* 85/100 - 250) && \
                                         ((x) < (t_us)*115/100 + 250))


// ----------------------------------------------------------------------------
// Global Variables
// ----------------------------------------------------------------------------
static BOOL bIrRxOpened = FALSE;
static ID IrRxLockStatus = NO_TASK_LOCKED;
UINT g_uiIrRxTimerID;

static IRRX_DEVICE_OBJ IrRxDeviceObj;
IRRX_DATA_INFO g_IrRxDataInfo;

static IRRX_DATA_QUEUE IrRxDataQueue[IRRX_DATA_BUF_NUM];
static PIRRX_DATA_QUEUE pIrRxQFront = NULL;
static PIRRX_DATA_QUEUE pIrRxQRear = NULL;
static IRRX_DATA_QUEUE IrRxPutDataQ = {0, 0, 0};

static UINT32 uiLastRepeatID = 0;
static BOOL bIsDataPicked = TRUE;



// ----------------------------------------------------------------------------
// Function Declarations
// ----------------------------------------------------------------------------
/**
  ISR of IR receiver.

  Note that the ISR flow design is based on NEC IR remote control protocol.

  Description:

  Step A: Whenever entering the ISR, we recognize it a burst (protocol view) or not.
          Mark the time stamp, and then we can get the width of the previous
          low/high pulse by calculating the time difference.

  >> Rising edge (protocol view) handling:
  Step B:
          B-1: Is a Leader Code?
               B-1a) HIGH(9ms) + LOW(4.5ms)  -> 1st Leader Code
               B-1b) HIGH(9ms) + LOW(2.25ms) -> Repeat Code

          B-2: If already got a Leader Code, and uiBitNo < 32, then check it's a data bit or not.
               B-2a) HIGH(0.56ms) + LOW(0.56ms) -> DATA '0'
               B-2b) HIGH(0.56ms) + LOW(1.69ms) -> DATA '1'

  >> Falling edge (protocol view) handling:
  Step C:
          C-1: Is a Stop Bit?
               C-1a) 1st Leader Code + 32bits data + HIGH(0.56ms)
               C-1b) Repeat Code + HIGH(0.56ms)

  Step D: Change the next IR receiver GPIO interrupt trigger polariy.


  @param void
  @return void
*/
static void irrx_isr(void)
{
    UINT32 uiGPIOPinStatus, uiNextIntrPol, uiIrRxBurst;

    // Clear GPIO interrupt status first
    gpio_clearIntStatus(IrRxDeviceObj.uiGPIOIntr);

    // IR receiver is active-low in GPIO view, thus a pin status LOW indicates a burst
    uiGPIOPinStatus = gpio_getPin(IrRxDeviceObj.uiGPIOPin);
    uiNextIntrPol = (uiGPIOPinStatus)? GPIO_INTPOL_NEGLOW:GPIO_INTPOL_POSHIGH;

    // We prefer the protocol view, so use the inverse of the pin status
    uiIrRxBurst = !uiGPIOPinStatus;


    //---------------------------------------------------------------------------------
    // Step A:
    // Recognize it a burst or not.
    // Get the width of the previous low/high pulse by calculating the time difference.
    //---------------------------------------------------------------------------------
    if(!g_IrRxDataInfo.bIsBurst && uiIrRxBurst)
    {
        // Detected a rising (protocol view), calculate the previous LOW pulse width
        g_IrRxDataInfo.bIsBurst = TRUE;
        g_IrRxDataInfo.uiTimePos = irrx_getCurrTimerCnt();
        g_IrRxDataInfo.uiWidthLow = g_IrRxDataInfo.uiTimePos - g_IrRxDataInfo.uiTimeNeg;
    }
    else if(g_IrRxDataInfo.bIsBurst && !uiIrRxBurst)
    {
        // Detected a falling (protocol view), calculate the previous HIGH pulse width
        g_IrRxDataInfo.bIsBurst = FALSE;
        g_IrRxDataInfo.uiTimeNeg = irrx_getCurrTimerCnt();
        g_IrRxDataInfo.uiWidthHigh = g_IrRxDataInfo.uiTimeNeg - g_IrRxDataInfo.uiTimePos;
    }
    else
    {
        // Error, trigger not sync, drop it
        g_IrRxDataInfo.uiError = IRRX_ERR_UNSYNC;
        goto WaitNextInterrupt;
    }



    if(g_IrRxDataInfo.bIsBurst)
    {
        //---------------------------------------------------------------------------------
        // Rising edge (protocol view) handling:
        //---------------------------------------------------------------------------------

        //---------------------------------------------------------------------------------
        // Step B-1: Is a Leader Code?
        //           B-1a) HIGH(9ms) + LOW(4.5ms)  -> 1st Leader Code
        //           B-1b) HIGH(9ms) + LOW(2.25ms) -> Repeat Code
        //---------------------------------------------------------------------------------
        if(CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthHigh, IR_PW_LEADER_HIGH))
        {
            if(CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthLow, IR_PW_LEADER_SPACING))
            {
                // Leader Code, reset other identifiers!!!
                g_IrRxDataInfo.bLeaderCode = TRUE;
                g_IrRxDataInfo.bRepeatCode = FALSE;
                g_IrRxDataInfo.bStopBit = FALSE;

                g_IrRxDataInfo.uiBitNo = 0;
                g_IrRxDataInfo.uiRepeatCnt = 0;
                g_IrRxDataInfo.uiError = IRRX_ERR_OK;

                // Reset the following global var for outside data picking up
                bIsDataPicked = FALSE;
                uiLastRepeatID = 0;

                goto WaitNextInterrupt;
            }
            else if(CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthLow, IR_PW_REPEAT_SPACING))
            {
                // Repeat Code
                g_IrRxDataInfo.bRepeatCode = TRUE;
                goto WaitNextInterrupt;
            }
        }

        // Not a Leader Code, it might be a data bit...

        // However, filter out some cases first:
        // Have no Leader Code, or have Repeat Code, or have Stop Bit -> Don't handle data bits
        // bLeaderCode will be cleared after data expiration check
        // bRepeatCode will be cleared after getting Stop Bit
        if(!g_IrRxDataInfo.bLeaderCode || g_IrRxDataInfo.bRepeatCode || g_IrRxDataInfo.bStopBit)
            goto WaitNextInterrupt;

        //---------------------------------------------------------------------------------
        // Step B-2: Is a data bit?
        //           B-2a) HIGH(0.56ms) + LOW(0.56ms) -> DATA '0'
        //           B-2b) HIGH(0.56ms) + LOW(1.69ms) -> DATA '1'
        //---------------------------------------------------------------------------------

        // Valid data bit high pulse width check
        if(!CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthHigh, IR_PW_DATABIT_HIGH))
        {
            g_IrRxDataInfo.uiError = IRRX_ERR_INVALID_BURST;
            goto WaitNextInterrupt;
        }

        // Valid total bits number check
        if(g_IrRxDataInfo.uiBitNo < IR_DATA_BITS_NUM)
        {
            // Recognize '1' or '0' by checking the data spacing width
            if(CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthLow, IR_PW_DATABIT0_SPACING))
            {
                // Bit '0'
                g_IrRxDataInfo.uiRxDataBits &= ~(1 << g_IrRxDataInfo.uiBitNo);
                g_IrRxDataInfo.uiBitNo++;
            }
            else if(CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthLow, IR_PW_DATABIT1_SPACING))
            {
                // Bit '1'
                g_IrRxDataInfo.uiRxDataBits |= (1 << g_IrRxDataInfo.uiBitNo);
                g_IrRxDataInfo.uiBitNo++;
            }
            else
            {
                // Error, invalid bit
                g_IrRxDataInfo.uiError = IRRX_ERR_INVALID_BIT;
            }
        }
        else
        {
            // Error, too many bits
            g_IrRxDataInfo.uiBitNo++;
            g_IrRxDataInfo.uiError = IRRX_ERR_TOO_MANY_BIT;
        }

    }
    else
    {
        //---------------------------------------------------------------------------------
        // Falling edge (protocol view) handling:
        //---------------------------------------------------------------------------------

        // Drop any before we've gotten a Leader Code
        if(!g_IrRxDataInfo.bLeaderCode)
            goto WaitNextInterrupt;

        //---------------------------------------------------------------------------------
        // Step C-1: Is a Stop Bit?
        //           C-1a) 1st Leader Code + 32bits data + HIGH(0.56ms)
        //           C-1b) Repeat Code + HIGH(0.56ms)
        //---------------------------------------------------------------------------------
        if(g_IrRxDataInfo.uiBitNo == IR_DATA_BITS_NUM && !g_IrRxDataInfo.bStopBit)
        {
            // Valid Stop Bit high pulse width check
            if(!CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthHigh, IR_PW_STOPBIT_HIGH))
            {
                g_IrRxDataInfo.uiError = IRRX_ERR_INVALID_BURST;
                goto WaitNextInterrupt;
            }

            //---------------------------------------------------------
            // Hello world, we already got one entire data frame!!!
            //---------------------------------------------------------
            g_IrRxDataInfo.bStopBit = TRUE;
            g_IrRxDataInfo.uiRxDataTime = irrx_getCurrTimerCnt();
            // Put the data in IR receiver data queue
            IrRxPutDataQ.uiRxData = g_IrRxDataInfo.uiRxDataBits;
            IrRxPutDataQ.uiRxTime = g_IrRxDataInfo.uiRxDataTime;
            irrx_enqueueData((PIRRX_DATA_QUEUE)&IrRxPutDataQ);

            goto WaitNextInterrupt;
        }

        if(g_IrRxDataInfo.bRepeatCode)
        {
            // Valid Stop Bit high pulse width check
            if(!CHECK_PULSE_WIDTH(g_IrRxDataInfo.uiWidthHigh, IR_PW_STOPBIT_HIGH))
            {
                g_IrRxDataInfo.uiError = IRRX_ERR_INVALID_BURST;
                goto WaitNextInterrupt;
            }

            //---------------------------------------------------------
            // Hello world, we already got one Repeat Code!!!
            //---------------------------------------------------------
            g_IrRxDataInfo.bRepeatCode = FALSE;
            g_IrRxDataInfo.uiRepeatCnt++;
            g_IrRxDataInfo.uiRxDataTime = irrx_getCurrTimerCnt();
            // Put a Repeat Code identifier in IR receiver data queue
            IrRxPutDataQ.uiRxData = IRRX_REPEAT_CODE_IDT;
            IrRxPutDataQ.uiRxTime = g_IrRxDataInfo.uiRxDataTime;
            irrx_enqueueData((PIRRX_DATA_QUEUE)&IrRxPutDataQ);

            goto WaitNextInterrupt;
        }

    }


WaitNextInterrupt:

    //---------------------------------------------------------------------------------
    // Step D:
    // Change the next IR receiver GPIO interrupt trigger polariy.
    //---------------------------------------------------------------------------------
    gpio_setIntTypePol(IrRxDeviceObj.uiGPIOIntr, GPIO_INTTYPE_EDGE, uiNextIntrPol);

}

#if 0   // Reserved
/**
  Lock IR receiver.

  This function lock IrRx module.

  @param void
  @return E_OK if success
*/
static ER irrx_lock(void)
{
    // Reserved
    return E_OK;
}

/**
  Unlock IR receiver.

  This function unlock IrRx module.

  @param void
  @return E_OK if success
*/
static ER irrx_unlock(void)
{
    // Reserved
    return E_OK;
}
#endif

/**
  Get the lock status of IrRx.

  Get the lock status of IrRx.

  @param void
  @return NO_TASK_LOCKED: IrRx is free, no application is using IrRx
          TASK_LOCKED   : IrRx is locked by some application
*/
ID irrx_getLockStatus(void)
{
    return IrRxLockStatus;
}

/**
  Initialize IR receiver I/O pad.

  Also enable IR receiver GPIO interrupt.

  @param void
  @return void
*/
static void irrx_startIO(void)
{
    // Init IR receiver GPIO to input direction and pullup (active-low)
    gpio_setDir(IrRxDeviceObj.uiGPIOPin, GPIO_DIR_INPUT);
    pad_setPullUpDown(IrRxDeviceObj.uiGPIOPad, PAD_PULLUP);

    // Set IR receiver GPIO interrupt to negative edge trigger (falling edge)
    gpio_setIntTypePol(IrRxDeviceObj.uiGPIOIntr, GPIO_INTTYPE_EDGE, GPIO_INTPOL_NEGLOW);

    // Hook IR receiver GPIO interrupt ISR for data pulse detection
    gpio_setIntIsr(IrRxDeviceObj.uiGPIOIntr, irrx_isr);

    // Enable IR receiver GPIO interrupt
    gpio_clearIntStatus(IrRxDeviceObj.uiGPIOIntr);
    gpio_enableInt(IrRxDeviceObj.uiGPIOIntr);
}

/**
  Release IR receiver I/O pad.

  Mainly disable IR receiver GPIO interrupt.
  Note that you can NOT call irrx_stopIO() before doing irrx_startIO().

  @param void
  @return void
*/
static void irrx_stopIO(void)
{
    // Disable IR receiver GPIO interrupt
    gpio_disableInt(IrRxDeviceObj.uiGPIOIntr);
    gpio_clearIntStatus(IrRxDeviceObj.uiGPIOIntr);
}

/**
  Open IR receiver and start a reference timer.

  Open IR receiver and start a reference timer.

  @param pIrRxDevObj: IrRx device object pointer
  @return E_OK if success, E_SYS if already opened or timer is not available
*/
ER irrx_open(PIRRX_DEVICE_OBJ pIrRxDevObj)
{
    // Already opened?
    if(bIrRxOpened)
    {
        debug_err(("irrx_open: IrRx is already opened!\r\n"));
        return E_SYS;
    }

    // Require a timer for IR receiver detecting data pulse time stamp reference
    if(timer_open((UINT *)&g_uiIrRxTimerID, NULL) != E_OK)
    {
        debug_err(("irrx_open: Open IrRx timer failed\r\n"));
        return E_SYS;
    }

    bIrRxOpened = TRUE;

    // Save IrRx device object setting
    memset((void *)&IrRxDeviceObj, 0, sizeof(IRRX_DEVICE_OBJ));
    memcpy((void *)&IrRxDeviceObj, (void *)pIrRxDevObj, sizeof(IRRX_DEVICE_OBJ));

    // Init IR receiver data info structure
    memset((void *)&g_IrRxDataInfo, 0, sizeof(IRRX_DATA_INFO));

    // Set the timeout variable to max value
    // Note that the tick resolution is 1 us
    timer_set_us(g_uiIrRxTimerID, 0xFFFFFFFF, _TIMER_CTRL_FREE_RUN, _TIMER_PLAY);

    // Init IR receiver data queue
    irrx_initDataQueue();

    // Open GPIO driver
    gpio_open();

    // Init IR receiver I/O pad and enable the GPIO interrupt
    irrx_startIO();

    return E_OK;
}

/**
  Close IR receiver and release the reference timer.

  Close IR receiver and release the reference timer.

  @param void
  @return E_OK if success
*/
ER irrx_close(void)
{
    if(!bIrRxOpened)
        return E_OK;

    // Close GPIO driver
    gpio_close();

    // Release IR receiver I/O pad and disable the GPIO interrupt
    irrx_stopIO();

    // Release timer
    timer_close(g_uiIrRxTimerID);

    bIrRxOpened = FALSE;
    return E_OK;
}

/**
  Initialize IR receiver data queue.

  FIFO, circular linked list queue.

  QFront: Where a data to GET from.
  QRear : Where a data to PUT to.

  @param void
  @return void
*/
void irrx_initDataQueue(void)
{
    UINT32 i;

    memset((void *)&IrRxDataQueue[0], 0, sizeof(IRRX_DATA_QUEUE)*IRRX_DATA_BUF_NUM);

    for(i = 0; i < IRRX_DATA_BUF_NUM; i++)
    {
        IrRxDataQueue[i].pNext = &IrRxDataQueue[(i+1) % IRRX_DATA_BUF_NUM];
    }

    pIrRxQFront = (PIRRX_DATA_QUEUE)&IrRxDataQueue[0];
    pIrRxQRear  = (PIRRX_DATA_QUEUE)&IrRxDataQueue[0];
}

/**
  Check IR receiver data queue is full or not.

  This function checks IR receiver data queue is full or not.

  @param void
  @return TRUE: Data queue is full
*/
BOOL irrx_isDataQueueFull(void)
{
    return (BOOL)(pIrRxQRear->pNext == pIrRxQFront);
}

/**
  Put data to IR receiver data queue.

  Put data to the current QRear.

  @param pPutDataQ: Pointer of a data gotta to be added in queue
  @return TRUE: Data added in queue, FALSE: Queue is full
*/
BOOL irrx_enqueueData(PIRRX_DATA_QUEUE pPutDataQ)
{
    if(!pPutDataQ)
    {
        debug_err(("irrx_enqueueData: Err! No queue base.\r\n"));
        return FALSE;
    }

    // Is queue full?
    if(pIrRxQRear->pNext == pIrRxQFront)
    {
        debug_err(("irrx_enqueueData: Err! Queue full.\r\n"));
        return FALSE;
    }

    // Put input data to queue
    pIrRxQRear->uiRxData = pPutDataQ->uiRxData;
    pIrRxQRear->uiRxTime = pPutDataQ->uiRxTime;

    // Move pIrRxQRear to next
    pIrRxQRear = pIrRxQRear->pNext;

    return TRUE;
}

/**
  Get data from IR receiver data queue.

  Get data from the current QFront.

  @param pGetDataQ: Pointer of a data buffer used to record the available data from queue
  @return TRUE: Data copied out from queue, FALSE: Queue is empty
*/
BOOL irrx_dequeueData(PIRRX_DATA_QUEUE pGetDataQ)
{
    if(!pGetDataQ)
    {
        debug_err(("irrx_dequeueData: Err! No queue base.\r\n"));
        return FALSE;
    }

    // Is queue empty?
    if(pIrRxQFront == pIrRxQRear)
    {
        return FALSE;
    }

    // Get data from queue to input data buffer
    pGetDataQ->uiRxData = pIrRxQFront->uiRxData;
    pGetDataQ->uiRxTime = pIrRxQFront->uiRxTime;
    pGetDataQ->pNext = pIrRxQFront->pNext;

    // Move pIrRxQFront to next
    pIrRxQFront = pIrRxQFront->pNext;

    return TRUE;
}

/**
  IR data expiration check, to verify IR signal is stop sending or not.

  For outside task periodic polling.

  @param void
  @return void
*/
void irrx_checkDataExp(void)
{
    UINT32 uiTIdle = 0;

    // Normal IR data frame interval is 108ms, let data expire if the time diff between
    // last data received and current is greater than 120ms
    uiTIdle = irrx_getCurrTimerCnt() - g_IrRxDataInfo.uiRxDataTime;

    if(uiTIdle > 120000)
    {
        if(g_IrRxDataInfo.bLeaderCode && g_IrRxDataInfo.uiBitNo >= IR_DATA_BITS_NUM)
        {
            g_IrRxDataInfo.bLeaderCode = FALSE;
        }
    }
}

/**
  Get current IR receiver timer count from start.

  This function gets the tick difference between current tick time and timer init time.

  @param void
  @return tick count difference from init (Note: In us unit)
*/
UINT32 irrx_getCurrTimerCnt(void)
{
    return timer_getCurrentCount(g_uiIrRxTimerID);
}

/**
  Get IrRx received data bits.

  For outside task picking up the received data contents. (If w/o IrRx data queue)

  @param void
  @return IrRx received data bits
*/
UINT32 irrx_getDataContent(void)
{
    bIsDataPicked = TRUE;
    return g_IrRxDataInfo.uiRxDataBits;
}

/**
  Check whether one data frame is fully received ok or not.

  For outside task checking if there is one data frame ready to pick. (If w/o IrRx data queue)

  @param void
  @return TRUE: Data is ready
*/
BOOL irrx_isDataReady(void)
{
    BOOL bResult = FALSE;

    if(g_IrRxDataInfo.bLeaderCode && g_IrRxDataInfo.uiBitNo == IR_DATA_BITS_NUM &&
            g_IrRxDataInfo.bStopBit && !bIsDataPicked)
        bResult = TRUE;

    return bResult;
}

/**
  Check whether a Repeat Code is received or not.

  For outside task checking if there is one Repeat Code need to report. (If w/o IrRx data queue)

  @param void
  @return TRUE: Repeat Code is still sending
*/
BOOL irrx_isDataRepeat(void)
{
    BOOL bResult = FALSE;

    if(g_IrRxDataInfo.bLeaderCode && g_IrRxDataInfo.uiRepeatCnt > 0)
    {
        if(g_IrRxDataInfo.uiRepeatCnt > uiLastRepeatID)
        {
            // uiLastRepeatID will be reset once getting a new Leader Code
            uiLastRepeatID++;
            bResult = TRUE;
        }
    }
    return bResult;
}




//@}
