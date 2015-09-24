/**
    @file       MI1310.c

    @brief      MI1310

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
#include "MI1310.h"
#include "Gpio.h"

#define SEN_READ_ID 0xBB
#define SEN_WRITE_ID 0xBA

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)     debug_msg ("MI1310:"__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif

#define SENDRV_ERR(...)     debug_err (("MI1310:"__VA_ARGS__));

// function prototype
static void MI1310_SifInit(UINT32 *pPort);
static void MI1310_Init(void);
static void MI1310_PwrOn(UINT32 uiStage);
static void MI1310_PwrOff(UINT32 uiStage);
static void MI1310_Sleep(UINT32 uiStage);
static void MI1310_WakeUp(UINT32 uiStage);
static void MI1310_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI1310_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI1310_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI1310_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI1310_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI1310_ChgMode(UINT32 uiMode);
static void MI1310_GetExpo(EXPOSURE_SETTING *Expo);
static void MI1310_CalLine(UINT32 uiTime);
static void MI1310_CalGain(UINT32 uiIso);
static void MI1310_SetFrame(UINT32 uiFrame);
static void MI1310_SetLine(UINT32 uiLine);
static void MI1310_SetPixel(UINT32 uiStep);
static void MI1310_SetVDLength(UINT32 uiVDLength);
static void MI1310_SetGain(UINT32 uiGain);
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI1310_checkExposureLongTime(void);
static void MI1310_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 MI1310_GetCalMshutterDelayTime(void);
static void MI1310_TriggerGlobalReset(void);
static void MI1310_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pMI1310Drv =
{
    // TG control
    MI1310_SifInit,
    MI1310_Init,
    MI1310_PwrOn,
    MI1310_PwrOff,
    MI1310_Sleep,
    MI1310_WakeUp,
    MI1310_WriteReg,
    MI1310_ReadReg,
    MI1310_GetStatus,
    MI1310_GetCurStatus,
    MI1310_ChgMode,
    //feature,
    NULL,
    // AE
    MI1310_GetExpo,
    MI1310_CalLine,
    MI1310_CalGain,
    MI1310_SetFrame,
    MI1310_SetLine,
    MI1310_SetPixel,
    MI1310_SetVDLength,
    MI1310_SetGain,
    MI1310_checkExposureLongTime,
    MI1310_SetCalMshutterDelayTime,
    MI1310_GetCalMshutterDelayTime,
    MI1310_TriggerGlobalReset,
    MI1310_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gMI1310Status;
static EXPOSURE_SETTING gMI1310ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI1310Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI1310Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI1310_prv_mode,  // pPrvMode
        MI1310_still_mode   // pStillMode
    };
    //g_pMI1310Info.pPrvMode = &MI1310_prv_mode[CurrentSensorMode];
    return &g_pMI1310Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI1310_SifInit(UINT32 *pPort)
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

    erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_50KHZ, I2C_TSR_50KHZ, I2C_GSR_50KHZ);

    if(erReturn != I2C_SUCCESS)
    {
        SENDRV_ERR("Error open I2C driver!!\r\n");
    }
}

static void MI1310_Init(void)
{
    UINT32 idx, CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("MI1310_init...\r\n");

    MI1310_GetRegInfo(MI1310_INIT, &CmdAddr, &CmdLength);
    //soft reset
    MI1310_WriteReg(0x0D, 0x0001);
    MI1310_WriteReg(0x0D, 0x0000);

    for (idx = 0;idx < CmdLength;idx++)
    {
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        if ((((SENSOR_CMD*)CmdAddr)->uiAddr == 0x100) && (((SENSOR_CMD*)CmdAddr)->uiData == 0x1))
        {
            TimerDelayMs(1);
        }
        MI1310_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    // Initial local status
    memset(&gMI1310Status, 0, sizeof(SENSOR_STATUS));
    memset(&gMI1310ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void MI1310_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1310_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1310_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1310_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1310_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData1, ulData2;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = uiAddr;
    ulData1     = (uiValue>>8);
    ulData2     = ((uiValue)&(0x000000ff));

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Error transmit data (write addr)!!\r\n");
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Error transmit data (reg) !!\r\n");
            break;
        }

        erReturn=i2c_masterTransmit(ulData1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Error transmit data (data1) !!\r\n");
            break;
        }


        erReturn=i2c_masterTransmit(ulData2, 0, 1);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Error transmit data (data2) !!\r\n");
            break;
        }

    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

static void MI1310_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulData, ulData1, ulData2;

    i2c_lock();
    ulWriteAddr =SEN_WRITE_ID;
    ulReadAddr  =SEN_READ_ID;
    ulReg1      =uiAddr;
    ulData      =0;

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Read error transmit data(1)!!\r\n");
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Read error transmit data(2)!!\r\n");
            break;
        }

        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);
        if (erReturn !=  I2C_SUCCESS)
        {
            SENDRV_ERR("Read error transmit addr before receive!!\r\n");
            break;
        }

        erReturn=i2c_masterReceive(&ulData1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Read error receive data1!!\r\n");
            break;
        }

        erReturn=i2c_masterReceive(&ulData2, 1, 1);
        if (erReturn != I2C_SUCCESS)
        {
            SENDRV_ERR("Read error receive data2!!\r\n");
            break;
        }

    } while(erReturn != I2C_SUCCESS);
    ulData=((ulData1<<8)|(ulData2));
    *uiValue = ulData;

    i2c_unlock();
}

static void MI1310_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gMI1310Status;
}

static void MI1310_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI1310_SetStatus(uiMode,pStatus);
}

static void MI1310_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    MI1310_GetDataInfo(uiMode,&Data,NULL);
    MI1310_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
        case SENSOR_MODE_640X480:
        case SENSOR_MODE_1280X960:
        case SENSOR_MODE_1440X1080:
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

static void MI1310_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 CmdLength;
    UINT32 CmdAddr = 0;

    MI1310_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        //#NT#2011/11/14#Ethan Lau -begin
        if(((SENSOR_CMD*)CmdAddr)->uiAddr == 0x0000)
            TimerDelayMs(((SENSOR_CMD*)CmdAddr)->uiData);
        else
            MI1310_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //#NT#2011/11/14#Ethan Lau -end
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    MI1310_SetStatus(uiMode,&gMI1310Status);
}

/**
 * AE driver API
 */

static void MI1310_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI1310ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI1310_checkExposureLongTime(void)
{
    return FALSE;
}

static void MI1310_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 MI1310_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void MI1310_TriggerGlobalReset(void)
{

}

static void MI1310_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void MI1310_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;
    UINT32 Expo_CompensateRatio;

    temp_line = uiTime * gMI1310Status.uiPclk / gMI1310Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0008)
       temp_line = 0x0008;

    gMI1310ExpoSetting.uiFrame = 0;
    gMI1310ExpoSetting.uiLine = temp_line;
    gMI1310ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gMI1310Status.pSyncInfo->HD_total / gMI1310Status.uiPclk;
    else
        linetime = uiTime;

    if(linetime<uiTime)
        Expo_CompensateRatio = uiTime*100/linetime;
    else
        Expo_CompensateRatio = 100;
}

static void MI1310_CalGain(UINT32 uiIso)
{
    UINT32 Cal_gain;
    UINT32 AGCBase,ISOBase;
    MI1310_GetISOInfo(&ISOBase, &AGCBase);
    Cal_gain = AGCBase;

    Cal_gain = Cal_gain * 100 * uiIso / ISOBase / 100;

    if (Cal_gain < 200)
    {
        Cal_gain = 200;
    }
    else if (Cal_gain >= 64000)
    {
        Cal_gain = (64000 - 1);
    }

    if (Cal_gain < 4000)      //Agc 0x ~ 4x
    {
        Cal_gain = (Cal_gain * 32 )/1000;
    }
    else if (Cal_gain < 8000) //Agc 4x ~ 8x
    {
        Cal_gain /= 2;
        Cal_gain = (Cal_gain * 32 )/1000;
        Cal_gain |= 0x80;
    }
    else if (Cal_gain < 16000) //Agc 8x ~ 16x
    {
        Cal_gain /= 4;
        Cal_gain = (Cal_gain * 32)/1000;
        Cal_gain |= 0x180;
    }
    else if (Cal_gain < 32000) //Agc 16x ~ 32x
    {
        Cal_gain /= 8;
        Cal_gain = (Cal_gain * 32)/1000;
        Cal_gain |= 0x380;
    }
    else if (Cal_gain < 32000) //Agc 16x ~ 32x
    {
        Cal_gain /= 8;
        Cal_gain = (Cal_gain * 32)/1000;
        Cal_gain |= 0x380;
    }
    else if (Cal_gain < 64000) //Agc 32x ~ 64x
    {
        Cal_gain /= 16;
        Cal_gain = (Cal_gain * 32)/1000;
        Cal_gain |= 0x780;
    }

    gMI1310ExpoSetting.uiGain = Cal_gain;
}

static void MI1310_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void MI1310_SetLine(UINT32 uiLine)
{
    if((uiLine < 1) && (uiLine >= 0xFFFF))
        return;
    MI1310_WriteReg(0x09, ((uiLine      ) & 0xFFFF));
    MI1310_WriteReg(0x0C, 0);
}

static void MI1310_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void MI1310_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void MI1310_SetGain(UINT32 uiGain)
{
    MI1310_WriteReg(0x2F, uiGain);
}

//@@ EOF
