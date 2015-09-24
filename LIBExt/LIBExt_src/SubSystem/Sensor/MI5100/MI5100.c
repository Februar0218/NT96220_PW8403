/**
    @file       MI5100.c

    @brief      MI5100

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
#include "MI5100.h"
#include "ImageAlgInfor.h"
#include "Calibration.h"

#define SEN_WRITE_ID 0x90  // 20111207 for CDOE_NT96223
#define SEN_READ_ID 0x91   // 20111207 for CDOE_NT96223

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif

// function prototype
static void MI5100_SifInit(UINT32 *pPort);
static void MI5100_Init(void);
static void MI5100_PwrOn(UINT32 uiStage);
static void MI5100_PwrOff(UINT32 uiStage);
static void MI5100_Sleep(UINT32 uiStage);
static void MI5100_WakeUp(UINT32 uiStage);
static void MI5100_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI5100_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI5100_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI5100_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI5100_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI5100_ChgMode(UINT32 uiMode);
static void MI5100_GetExpo(EXPOSURE_SETTING *Expo);
static void MI5100_CalLine(UINT32 uiTime);
static void MI5100_CalGain(UINT32 uiIso);
static void MI5100_SetFrame(UINT32 uiFrame);
static void MI5100_SetLine(UINT32 uiLine);
static void MI5100_SetPixel(UINT32 uiStep);
static void MI5100_SetVDLength(UINT32 uiVDLength);
static void MI5100_SetGain(UINT32 uiGain);
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5100_checkExposureLongTime(void);
static void MI5100_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 MI5100_GetCalMshutterDelayTime(void);
static void MI5100_TriggerGlobalReset(void);
static void MI5100_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pMI5100Drv =
{
    // TG control
    MI5100_SifInit,
    MI5100_Init,
    MI5100_PwrOn,
    MI5100_PwrOff,
    MI5100_Sleep,
    MI5100_WakeUp,
    MI5100_WriteReg,
    MI5100_ReadReg,
    MI5100_GetStatus,
    MI5100_GetCurStatus,
    MI5100_ChgMode,
    //feature,
    NULL,
    // AE
    MI5100_GetExpo,
    MI5100_CalLine,
    MI5100_CalGain,
    MI5100_SetFrame,
    MI5100_SetLine,
    MI5100_SetPixel,
    MI5100_SetVDLength,
    MI5100_SetGain,
    MI5100_checkExposureLongTime,
    MI5100_SetCalMshutterDelayTime,
    MI5100_GetCalMshutterDelayTime,
    MI5100_TriggerGlobalReset,
    MI5100_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gMI5100Status;
static EXPOSURE_SETTING gMI5100ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI5100Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI5100Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI5100_prv_mode,  // pPrvMode
        MI5100_still_mode   // pStillMode
    };
    //g_pMI5100Info.pPrvMode = &MI5100_prv_mode[CurrentSensorMode];
    return &g_pMI5100Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI5100_SifInit(UINT32 *pPort)
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

    erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_200KHZ, I2C_TSR_200KHZ, I2C_GSR_200KHZ);//(96, 10, 4);

    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }
}

static void MI5100_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("MI5100_init...\r\n");

    MI5100_GetRegInfo(MI5100_INIT, &CmdAddr, &CmdLength);
    TimerDelayMs(100);

    //soft reset
    MI5100_WriteReg(0x0D, 0x01);

    TimerDelayMs(100);
    MI5100_WriteReg(0x0D, 0x00);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5100_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData );
        MI5100_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    // Initial local status
    memset(&gMI5100Status, 0, sizeof(SENSOR_STATUS));
    memset(&gMI5100ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void MI5100_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5100_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5100_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5100_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5100_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData1, ulData2;

    i2c_lock();
    ulWriteAddr = SEN_WRITE_ID;
    ulReg1       = uiAddr;
    ulData1     = (uiValue>>8);
    ulData2     = (uiValue&0x000000ff);

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

static void MI5100_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulData, ulData1, ulData2;

    i2c_lock();

    ulWriteAddr =SEN_WRITE_ID;
    ulReadAddr  =SEN_READ_ID;
    ulReg1       =uiAddr;
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

        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);

        if (erReturn !=  I2C_SUCCESS/*ER_I2C_AL*/)
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

static void MI5100_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gMI5100Status;
}

static void MI5100_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI5100_SetStatus(uiMode,pStatus);
}

static void MI5100_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    MI5100_GetDataInfo(uiMode,&Data,NULL);
    MI5100_GetSigInfo(uiMode,&Sync,NULL);
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

static void MI5100_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    MI5100_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        MI5100_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        MI5100_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    //TimerDelayMs(300);

    MI5100_SetStatus(uiMode,&gMI5100Status);
}

/**
 * AE driver API
 */

static void MI5100_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI5100ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5100_checkExposureLongTime(void)
{
    return FALSE;
}

static void MI5100_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 MI5100_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void MI5100_TriggerGlobalReset(void)
{

}

static void MI5100_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void MI5100_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;

    linetime=(gMI5100Status.pSyncInfo->HD_total + (gMI5100Status.uiPclk - 1))/gMI5100Status.uiPclk;

    temp_line = (uiTime + (linetime-1))/linetime;

    gMI5100ExpoSetting.uiFrame = 0;
    gMI5100ExpoSetting.uiLine = temp_line;
    gMI5100ExpoSetting.uiPixel = 0;

    linetime *= temp_line;

    gMI5100ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
}
#if 1
static void MI5100_CalGain(UINT32 uiIso)
{
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
   //  #define ADJ_ISO         8

    // add ISO calibration part for adjustment  [NTK#RandyHo]<2012/06/14>
    CAL_DATA CalInfo;
    Cal_GetCalData(&CalInfo);

    MI5100_GetISOInfo(&ISOBase, &AGCBase);

    if (CalInfo.CalStatus_ISO == _ISO_Status)
    {
        AGCBase= CalInfo.Cal_ISO_Data[0];
    }

    Calcul_AGC_Ratio = 100 * uiIso /ISOBase;    // multiple 100 for accuracy

    Gain_factor = Calcul_AGC_Ratio * AGCBase /100/100; // divided by 100 for Raito accuracy, and another for ISO base accuracy

    SENDRV_MSG("MI5100 CalGain %d..\r\n", Gain_factor);

    if(Gain_factor < 8)
    {
        Gain_factor =8;
    }

    if(Gain_factor > 0x7F)  // According to MI5100 spec, Gain=[1:128]
    {
        Gain_factor = 0x7F;
    }

    gMI5100ExpoSetting.uiGain = Gain_factor;
}
#else
static void MI5100_CalGain(UINT32 uiIso)
{
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
    #define ADJ_ISO         8

    // add ISO calibration part for adjustment  [NTK#RandyHo]<2012/06/14>
    CAL_DATA CalInfo;
    Cal_GetCalData(&CalInfo);

    MI5100_GetISOInfo(&ISOBase, &AGCBase);

    if (CalInfo.CalStatus_ISO == _ISO_Status)
    {
        AGCBase= CalInfo.Cal_ISO_Data[0];
    debug_err((">>cal agc = %d  iso = %d\r\n",AGCBase,uiIso));        
    }
           
    Calcul_AGC_Ratio = 100 * uiIso / ISOBase;
    debug_err((">> agc Ratio= %d  iso = %d base = %d\r\n",AGCBase,uiIso,ISOBase));   
    Gain_factor = ((Calcul_AGC_Ratio-100)*100+(AGCBase>>1))/AGCBase+ ADJ_ISO;
    
    debug_err((">>Gain factor = %d \r\n",Gain_factor));

    if(Gain_factor > 0x7F)  // According to MI5100 spec, Gain=[1:128]
    {
        Gain_factor = 0x7F;
    }

    gMI5100ExpoSetting.uiGain = Gain_factor;
}
#endif
static void MI5100_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void MI5100_SetLine(UINT32 uiLine)
{
    //if((uiLine<1)&&(uiLine>=0xFFFFF))
    //    return;

    if(uiLine < 1)
        uiLine = 1;
    else if(uiLine > 0xFFFFF)
        uiLine = 0xFFFFF;

    MI5100_WriteReg(0x08, ((uiLine >> 16) & 0x000F));
    MI5100_WriteReg(0x09, ((uiLine      ) & 0xFFFF));
    MI5100_WriteReg(0x0C, 0);

    SENDRV_MSG("MI5100 SetLine %d...\r\n", uiLine);
}

static void MI5100_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void MI5100_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void MI5100_SetGain(UINT32 uiGain)
{
    UH temp, AGCtemp;

    temp = 0;
    while(uiGain > 63)
    {
        uiGain -= 32;
        temp++;
    }

    if(temp != 0)
        AGCtemp = (((1 << temp) - 1) << 6) + uiGain;
    else
        AGCtemp = uiGain;

    if(AGCtemp < 8)
        AGCtemp = 8;

    MI5100_WriteReg(0x35, AGCtemp);

    SENDRV_MSG("MI5100 SetGain %d...\r\n", AGCtemp);
}

static BOOL bSleepMode=FALSE;
void MI5100_SleepMode(BOOL enable)
{
    UINT32 value;
    if(enable)
    {
        bSleepMode = ENABLE;
        #if 0
        //HW Standby 
        gpio_setDir((2|0x80000000),GPIO_DIR_OUTPUT);
        gpio_clearPin((2|0x80000000));        
        #else
        //Soft Standby
        MI5100_ReadReg(0x0007,&value);
        MI5100_WriteReg(0x0007,value&0xfffd);
        #endif
    }
    else
    {
        bSleepMode = DISABLE;
        #if 0
        //HW Standby
        gpio_setDir((2|0x80000000),GPIO_DIR_OUTPUT);
        gpio_setPin((2|0x80000000));
        #else
        //Soft Standby
        MI5100_ReadReg(0x0007,&value);
        MI5100_WriteReg(0x0007,value|0x0002);
        #endif
        TimerDelayMs(5);
        MI5100_WriteReg(0x000d,0xffff);
        TimerDelayMs(5);
        MI5100_WriteReg(0x000d,0x0);
    }
}
BOOL GetSensor_SleepMode(void)
{
    return bSleepMode;
}

//@@ EOF
