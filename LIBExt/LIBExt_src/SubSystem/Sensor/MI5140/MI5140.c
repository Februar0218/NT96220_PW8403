/**
    @file       MI5140.c

    @brief      MI5140

    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
//----------------------------------------------------------------------------------------------------
#include <string.h>
#include "i2c.h"
#include "Type.h"
#include "Debug.h"
#include "Top.h"
#include "utility.h"
#include "Sensor.h"
#include "MI5140.h"
#include "Calibration.h"

#define SEN_READ_ID 0x79
#define SEN_WRITE_ID 0x78

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif
//utility
static UINT CfaPat(UINT OrgPat, SEN_FLIP FlipType);
// function prototype
static void MI5140_SifInit(UINT32 *pPort);
static void MI5140_Init(void);
static void MI5140_PwrOn(UINT32 uiStage);
static void MI5140_PwrOff(UINT32 uiStage);
static void MI5140_Sleep(UINT32 uiStage);
static void MI5140_WakeUp(UINT32 uiStage);
static void MI5140_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI5140_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI5140_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI5140_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI5140_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI5140_ChgMode(UINT32 uiMode);
static void MI5140_GetExpo(EXPOSURE_SETTING *Expo);
static void MI5140_CalLine(UINT32 uiTime);
static void MI5140_CalGain(UINT32 uiIso);
static void MI5140_SetFrame(UINT32 uiFrame);
static void MI5140_SetLine(UINT32 uiLine);
static void MI5140_SetPixel(UINT32 uiStep);
static void MI5140_SetVDLength(UINT32 uiVDLength);
static void MI5140_SetGain(UINT32 uiGain);
//#NT#2011/10/26#Ethan Lau -begin
static void MI5140_Flip(UINT32 Direction);
//#NT#2011/10/26#Ethan Lau -end
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5140_checkExposureLongTime(void);
static void MI5140_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 MI5140_GetCalMshutterDelayTime(void);
static void MI5140_TriggerGlobalReset(void);
static void MI5140_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pMI5140Drv =
{
    // TG control
    MI5140_SifInit,
    MI5140_Init,
    MI5140_PwrOn,
    MI5140_PwrOff,
    MI5140_Sleep,
    MI5140_WakeUp,
    MI5140_WriteReg,
    MI5140_ReadReg,
    MI5140_GetStatus,
    MI5140_GetCurStatus,
    MI5140_ChgMode,
    //feature,
    MI5140_Flip,
    // AE
    MI5140_GetExpo,
    MI5140_CalLine,
    MI5140_CalGain,
    MI5140_SetFrame,
    MI5140_SetLine,
    MI5140_SetPixel,
    MI5140_SetVDLength,
    MI5140_SetGain,
    MI5140_checkExposureLongTime,
    MI5140_SetCalMshutterDelayTime,
    MI5140_GetCalMshutterDelayTime,
    MI5140_TriggerGlobalReset,
    MI5140_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gMI5140Status;
static EXPOSURE_SETTING gMI5140ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI5140Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI5140Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI5140_prv_mode,  // pPrvMode
        MI5140_still_mode   // pStillMode
    };
    //g_pMI5140Info.pPrvMode = &MI5140_prv_mode[CurrentSensorMode];
    return &g_pMI5140Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI5140_SifInit(UINT32 *pPort)
{
    UINT erReturn;

    erReturn = i2c_open();

    if(erReturn != 0 /*E_OK*/)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }

    i2c_enable();
    //removed, pinmux select are ctrled by proj layer
    //pinmux_select_i2c(PINMUX_I2C);

    //#NT#2011/08/19#Ethan Lau -begin
    //if cost down sensor need lower i2c clk
    switch(pPort[0])
    {
        case (1):
            erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_50KHZ, I2C_TSR_50KHZ, I2C_GSR_50KHZ);
            break;
        case (2):
            erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_100KHZ, I2C_TSR_100KHZ, I2C_GSR_100KHZ);
            break;
        case (3):
            erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_200KHZ, I2C_TSR_200KHZ, I2C_GSR_200KHZ);
            break;
        case (4):
            erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_400KHZ, I2C_TSR_400KHZ, I2C_GSR_400KHZ);
            break;
        case (0):
        default:
            erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_200KHZ, I2C_TSR_200KHZ, I2C_GSR_200KHZ);
            break;
    }
    //#NT#2011/08/19#Ethan Lau -end
    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }
}

static void MI5140_Init(void)
{
    UINT32 idx,CmdLength;
    UINT32 CmdAddr = 0;
    //UINT32 tmp=0;
    //UINT32 value;

#if (SENSOR_LSC_ENABLE)
    debug_err(("MI5140_init...\r\n"));
    SENDRV_MSG("MI5140_init...\r\n");

    MI5140_GetRegInfo(MI5140_INIT, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    TimerDelayMs(30); // 2400 * EXTCLK
    MI5140_GetRegInfo(MI5140_2XHP, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_CapFULL, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
        MI5140_WriteReg(0x0982, 0x0000);
    MI5140_GetRegInfo(MI5140_K28A_Rev2_1, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_2, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_3, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_4, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_5, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_6, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_7, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_8, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_9, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_GetRegInfo(MI5140_K28A_Rev2_10, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor K28A W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_WriteReg(0x098E, 0x0016);
    MI5140_WriteReg(0x8016, 0x0838);
    MI5140_WriteReg(0x8002, 0x0001);
    MI5140_WriteReg(0xC8ED, 0x02  );
    TimerDelayMs(30); // 2400 * EXTCLK
    MI5140_WriteReg(0xC8ED, 0x02  );
    MI5140_GetRegInfo(MI5140_Chart, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor Char W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI5140_WriteReg(0x3172, 0x2113);// cause zigzag issue
//**************************
// load LSC data
    MI5140_GetRegInfo(MI5140_LSC, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor LSC W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    } /**/
//***************************
#else
    UINT32 value;

    debug_err(("MI5140_init...\r\n"));
    SENDRV_MSG("MI5140_init...\r\n");
    MI5140_GetRegInfo(MI5140_PLL, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
       // MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    TimerDelayMs(30);

    MI5140_ReadReg(0x0018,&value);
    value &= 0xfffb;
    MI5140_WriteReg(0x0018, value);
   /* TimerDelayMs(5);
    MI5140_GetRegInfo(MI5140_Chart, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor Char W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }  */
    TimerDelayMs(5);  
    MI5140_WriteReg(0x001c, 0x0001);
    MI5140_WriteReg(0x060e, 0x0003);
    MI5140_WriteReg(0x3c00, 0x0001);
    MI5140_WriteReg(0x3c02, 0x4c90);
   // MI5140_WriteReg(0x3012, 0x0420);
   // MI5140_WriteReg(0x305E, 0x11C0);

    

/*    MI5140_GetRegInfo(MI5140_INIT, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }    
*/
    MI5140_WriteReg(0x3172, 0x2113);// cause zigzag issue      
#endif
    TimerDelayMs(100); // 2400 * EXTCLK  
    debug_err(("sensor init end\r\n"));
    // Initial local status
    memset(&gMI5140Status, 0, sizeof(SENSOR_STATUS));
    memset(&gMI5140ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void MI5140_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5140_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5140_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5140_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5140_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulReg2, ulData1, ulData2;

    i2c_lock();
    TimerDelayUs(100);//add for adpk downgrade wafer,ensure i2c stop and next start have enough time gap
    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (uiAddr>>8);
    ulReg2      = ((uiAddr)&(0x000000ff));
    ulData1     = (uiValue>>8);
    ulData2     = ((uiValue)&(0x000000ff));

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
            debug_err(("Error transmit data (reg) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulData1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulData2, 0, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data2) !!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

static void MI5140_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulReg2, ulData, ulData1, ulData2;

    i2c_lock();
    TimerDelayUs(100);//add for adpk downgrade wafer,ensure i2c stop and next start have enough time gap
    ulWriteAddr =SEN_WRITE_ID;
    ulReadAddr  =SEN_READ_ID;
    ulReg1      =(uiAddr>>8);
    ulReg2      =((uiAddr)&(0x000000ff));
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

        erReturn=i2c_masterReceive(&ulData1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData2, 1, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data2!!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);
    ulData=((ulData1<<8)|(ulData2));

    *uiValue = ulData;
    i2c_unlock();
}

static void MI5140_GetCurStatus(PSENSOR_STATUS pStatus)
{
    *pStatus = gMI5140Status;
}

static void MI5140_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI5140_SetStatus(uiMode,pStatus);
}

static void MI5140_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    MI5140_GetDataInfo(uiMode,&Data,NULL);
    MI5140_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
        case SENSOR_MODE_640X480:
        case SENSOR_MODE_1280X960:
            pStatus->uiSensorMode = uiMode;
            pStatus->uiReadoutMode = 0; //SENSOR_OUT_PROGRESSIVE;
            pStatus->uiRatio = SENSOR_RATIO_4_3;
            pStatus->uiBayer = ((PREVIEW_DATA_INFO*)Data)->uiBayer;
            pStatus->uiPclk = ((PREVIEW_DATA_INFO*)Data)->uiPclk;
             //#NT#2011/11/23#Ethan Lau -begin
            pStatus->uiBinning = ((PREVIEW_DATA_INFO*)Data)->uiBinning;
            pStatus->uiSkip = ((PREVIEW_DATA_INFO*)Data)->uiSkip;
            //#NT#2011/11/23#Ethan Lau -end
            pStatus->pSyncInfo = ((SYNC_INFO*)Sync);
            break;

        case SENSOR_MODE_FULL:
            pStatus->uiSensorMode = SENSOR_MODE_FULL;
            pStatus->uiReadoutMode = 0; //SENSOR_OUT_PROGRESSIVE;
            pStatus->uiRatio = SENSOR_RATIO_4_3;
            pStatus->uiBayer = ((CAPTURE_DATA_INFO*)Data)->uiBayer;
            pStatus->uiPclk = ((CAPTURE_DATA_INFO*)Data)->uiPclk;
             //#NT#2011/11/23#Ethan Lau -begin
            pStatus->uiBinning = 1;
            pStatus->uiSkip = 1;
            //#NT#2011/11/23#Ethan Lau -end
            pStatus->pSyncInfo = ((SYNC_INFO*)Sync);
            break;
        default:
            debug_err(("^R %s: mode error(%d)",__func__,uiMode));
            break;
     }
}

static void MI5140_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    MI5140_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        MI5140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        MI5140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    MI5140_SetStatus(uiMode,&gMI5140Status);
}

/**
 * AE driver API
 */

static void MI5140_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI5140ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5140_checkExposureLongTime(void)
{
    return FALSE;
}

static void MI5140_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 MI5140_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void MI5140_TriggerGlobalReset(void)
{

}

static void MI5140_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void MI5140_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;
    UINT32 Expo_CompensateRatio;

    temp_line = uiTime * gMI5140Status.uiPclk / gMI5140Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0002)
       temp_line = 0x0002;

    gMI5140ExpoSetting.uiFrame = 0;
    gMI5140ExpoSetting.uiLine = temp_line;
    gMI5140ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gMI5140Status.pSyncInfo->HD_total / gMI5140Status.uiPclk;
    else
        linetime = uiTime;

    if(linetime<uiTime)
        Expo_CompensateRatio = uiTime*100/linetime;
    else
        Expo_CompensateRatio = 100;

    gMI5140ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
}

#if 1
static void MI5140_CalGain(UINT32 uiIso)
{
    BOOL bit8=FALSE,bit7=FALSE,bit6=FALSE;
    UINT32 digital_gain = 1;
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
    CAL_DATA CalInfo;
    
    Cal_GetCalData(&CalInfo);   
    
    MI5140_GetISOInfo(&ISOBase, &AGCBase);
    
    if (CalInfo.CalStatus_ISO == _ISO_Status)
        Calcul_AGC_Ratio = CalInfo.Cal_ISO_Data[0];
    else
        Calcul_AGC_Ratio = AGCBase;

    Gain_factor= Calcul_AGC_Ratio*100*uiIso/ISOBase/100;

    //upper and lower bound
    if(Gain_factor < 200)
    {
        Gain_factor = 200;
    }
    if(Gain_factor > 24000)
    {
        Gain_factor = 24000;
    }

    //debug_msg("Gain_factor=%d\r\n",Gain_factor);
    //#NT#2011/11/28#Ethan Lau -begin
    //#NT#follow MI5131 spec.
    //convert desire gain to register setting
    if(Gain_factor < 2000)//agc 0x~2x
    {
        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x1000;  //0x1000 for digital gain
    }
    else
    {
        while(Gain_factor >= 16000)//more than 16x,use digital
        {
            Gain_factor/=2;
            digital_gain ++;
            if(digital_gain >7)
            {
                digital_gain= 7;
                debug_err(("^R sensor digital gain out of limit!!\r\n"));
                break;
            }
        }
        while(Gain_factor>=2000)
        {
            if(bit7 == FALSE)
            {
                bit7 = TRUE;
                Gain_factor /= 2;
            }
            else if(bit8 == FALSE)
            {
                bit8 = TRUE;
                Gain_factor /= 2;
            }
            else if(bit6 == FALSE)
            {
                bit6 = TRUE;
                Gain_factor -= 2000;
            }
            else
            {
                debug_err(("^R sensor analog gain out of limit(%d)!!\r\n",Gain_factor));
                break;
            }
        }
        Gain_factor = (Gain_factor * 32 )/1000 ;
        //debug_msg("sensor gain = 0x%x\r\n",Gain_factor);
        Gain_factor |= (digital_gain<<12);//digital
        Gain_factor |= (bit6<<6);//analog
        //debug_msg("sensor gain = 0x%x\r\n",Gain_factor);
        Gain_factor |= (bit7<<7);//analog
        //debug_msg("sensor gain = 0x%x\r\n",Gain_factor);
        Gain_factor |= (bit8<<8);//analog
        //debug_msg("^G sensor gain = 0x%x\r\n",Gain_factor);
    }
    //#NT#2011/11/28#Ethan Lau -end
    gMI5140ExpoSetting.uiGain = Gain_factor;
}
#else
static void MI5140_CalGain(UINT32 uiIso)
{
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
    MI5140_GetISOInfo(&ISOBase, &AGCBase);

    Calcul_AGC_Ratio = AGCBase;

    Gain_factor= Calcul_AGC_Ratio*100*uiIso/ISOBase/100;

    //upper and lower bound
    if(Gain_factor < 1000)
    {
        Gain_factor = 1000;
    }
    if(Gain_factor > 24000)
    {
        Gain_factor = 24000;
    }

    debug_err(("Gain_factor=%d\r\n",Gain_factor));

    //convert desire gain to register setting
    if(Gain_factor < 4000)//Agc 0x~4x
    {
        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x1000;
    }
    else if(Gain_factor < 8000)//Agc 4x~8x
    {
        Gain_factor/=2;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        //Gain_factor |= 0x2080;
        Gain_factor |= 0x1080;
    }
    else if(Gain_factor < 16000)//Agc 8x~16x
    {
        Gain_factor/=4;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        //Gain_factor |= 0x4080;
        Gain_factor |= 0x1180;
    }
    else// if(Gain_factor < 32512) //Agc 16x~24x
    {
        Gain_factor/=8;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x2180;
    }
   /* else if(Gain_factor < 24000) //Agc 24x~48x
    {
        Gain_factor/=12;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x4180;
    }
    else
    {
        Gain_factor= 0x6180;//0x60ff;//0x12FF;
    }*/

    gMI5140ExpoSetting.uiGain = Gain_factor;
}
#endif
static void MI5140_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void MI5140_SetLine(UINT32 uiLine)
{
    if(uiLine > (gMI5140Status.pSyncInfo->VD_total - 2))
    {
        MI5140_WriteReg(0x3012, uiLine); //coarse integration time
        MI5140_WriteReg(0x0340, (uiLine + 2)); //coarse integration time
    }
    else
    {
        MI5140_WriteReg(0x3012, uiLine); //coarse integration time
        MI5140_WriteReg(0x0340, (gMI5140Status.pSyncInfo->VD_total + 2)); //coarse integration time
    }
}

static void MI5140_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void MI5140_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void MI5140_SetGain(UINT32 uiGain)
{
    MI5140_WriteReg(0x305E, uiGain);
}

static void MI5140_Flip(UINT32 Direction)
{
    //flip enable bit
    #define H_flip  0x4000
    #define V_flip  0x8000
    #define Prv_Reg 5
    #define Cap_Reg 6
    UINT32  CmdLength;
    UINT32  Cmd = 0;
    UINT32 DataPrv1 = 0,DataPrv2 = 0,DataCap = 0;
    UINT32 Reg0,Reg1,Reg2;
    static BOOL bInit = TRUE;
    static UINT32 OrgData[3],OrgCFA[2];

    MI5140_GetRegInfo(MI5140_VGA, &Cmd, &CmdLength);
    Reg0 = ((SENSOR_CMD*)(Cmd+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI5140_GetRegInfo(MI5140_1280X960, &Cmd, &CmdLength);
    Reg1 = ((SENSOR_CMD*)(Cmd+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI5140_GetRegInfo(MI5140_STILL, &Cmd, &CmdLength);
    Reg2 = ((SENSOR_CMD*)(Cmd+Cap_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip

    MI5140_GetDataInfo(MI5140_VGA, &DataPrv1, NULL);
    MI5140_GetDataInfo(MI5140_1280X960, &DataPrv2, NULL);
    MI5140_GetDataInfo(MI5140_STILL, &DataCap, NULL);
    if(bInit == TRUE)
    {
        OrgData[0] = Reg0;
        OrgData[1] = Reg1;
        OrgData[2] = Reg2;

        OrgCFA[0]  = ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer;
        OrgCFA[1]  = ((CAPTURE_DATA_INFO*)DataCap)->uiBayer;

        bInit = FALSE;
    }

    switch((SEN_FLIP)Direction)
    {
        case sen_Flip_H:
            //patch bayer pattern
            ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
            ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA[1],(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData[0] | H_flip;         //flip function in table idx 5
            Reg1 = OrgData[1] | H_flip;         //flip function in table idx 5
            Reg2 = OrgData[2] | H_flip;         //flip function in table idx 6
            break;
        case sen_Flip_V:
            //patch bayer pattern
            ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
            ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA[1],(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData[0] | V_flip;         //flip function in table idx 5
            Reg1 = OrgData[1] | V_flip;         //flip function in table idx 5
            Reg2 = OrgData[2] | V_flip;         //flip function in table idx 6
            break;
        case sen_Flip_H_V:
            //patch bayer pattern
            ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
            ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA[1],(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData[0] | (H_flip|V_flip);    //flip function in table idx 5
            Reg1 = OrgData[1] | (H_flip|V_flip);    //flip function in table idx 5
            Reg2 = OrgData[2] | (H_flip|V_flip);    //flip function in table idx 6
            break;
        case sen_Flip_NULL:
            //patch bayer pattern
            ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer = OrgCFA[0];
            ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = OrgCFA[0];
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = OrgCFA[1];
            //patch registor set
            Reg0 = OrgData[0];
            Reg1 = OrgData[1];
            Reg2 = OrgData[2];
            break;
        default:
            debug_err(("^R sensor flip direction error\r\n"));
            break;
    }
    //do not update sensor status,should undate in next sensor chg mode
    /**
    */

}
#if 0
- utility
#endif
static UINT CfaPat(UINT OrgPat, SEN_FLIP FlipType)
{
    UINT8 PatTable[4][4] =
    {
        {SIECFA_R,  SIECFA_Gr,  SIECFA_Gb,  SIECFA_B    },
        {SIECFA_Gr, SIECFA_R,   SIECFA_B,   SIECFA_Gb   },
        {SIECFA_Gb, SIECFA_B,   SIECFA_R,   SIECFA_Gr   },
        {SIECFA_B,  SIECFA_Gb,  SIECFA_Gr,  SIECFA_R    }
    };

    return PatTable[OrgPat][FlipType];
}
//@@ EOF
