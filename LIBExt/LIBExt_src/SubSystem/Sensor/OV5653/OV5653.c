/**
    @file       OV5653.c

    @brief      OV5653

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
#include "OV5653.h"
#include "Calibration.h"

#define SEN_READ_ID 0x6D
#define SEN_WRITE_ID 0x6C

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif

//#NT#2011/07/11#Ethan Lau -begin
static UINT32   m_uiAE_Maxline;
//#NT#2011/07/11#Ethan Lau -end
// function prototype
static void OV5653_SifInit(UINT32 *pPort);
static void OV5653_Init(void);
static void OV5653_PwrOn(UINT32 uiStage);
static void OV5653_PwrOff(UINT32 uiStage);
static void OV5653_Sleep(UINT32 uiStage);
static void OV5653_WakeUp(UINT32 uiStage);
static void OV5653_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void OV5653_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void OV5653_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void OV5653_GetCurStatus(PSENSOR_STATUS pStatus);
static void OV5653_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void OV5653_ChgMode(UINT32 uiMode);
static void OV5653_GetExpo(EXPOSURE_SETTING *Expo);
static void OV5653_CalLine(UINT32 uiTime);
static void OV5653_CalGain(UINT32 uiIso);
static void OV5653_SetFrame(UINT32 uiFrame);
static void OV5653_SetLine(UINT32 uiLine);
static void OV5653_SetPixel(UINT32 uiStep);
static void OV5653_SetVDLength(UINT32 uiVDLength);
static void OV5653_SetGain(UINT32 uiGain);

static SENSOR_TAB g_pOV5653Drv =
{
    // TG control
    OV5653_SifInit,
    OV5653_Init,
    OV5653_PwrOn,
    OV5653_PwrOff,
    OV5653_Sleep,
    OV5653_WakeUp,
    OV5653_WriteReg,
    OV5653_ReadReg,
    OV5653_GetStatus,
    OV5653_GetCurStatus,
    OV5653_ChgMode,
    //feature,
    NULL,
    // AE
    OV5653_GetExpo,
    OV5653_CalLine,
    OV5653_CalGain,
    OV5653_SetFrame,
    OV5653_SetLine,
    OV5653_SetPixel,
    OV5653_SetVDLength,
    OV5653_SetGain
};

static SENSOR_STATUS gOV5653Status;
static EXPOSURE_SETTING gOV5653ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pOV5653Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pOV5653Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        OV5653_prv_mode,  // pPrvMode
        OV5653_still_mode   // pStillMode
    };
    //g_pOV5653Info.pPrvMode = &OV5653_prv_mode[CurrentSensorMode];
    return &g_pOV5653Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void OV5653_SifInit(UINT32 *pPort)
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

    erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_200KHZ, I2C_TSR_200KHZ, I2C_GSR_200KHZ);

    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }
}

static void OV5653_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("OV5653_init...\r\n");

    OV5653_GetRegInfo(OV5653_INIT, &CmdAddr, &CmdLength);
    TimerDelayMs(100);

    //soft reset
    OV5653_WriteReg(0x3008, 0x82);

    TimerDelayMs(100);

    for (idx = 0;idx < CmdLength;idx++)
    {
        OV5653_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        OV5653_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    // Initial local status
    memset(&gOV5653Status, 0, sizeof(SENSOR_STATUS));
    memset(&gOV5653ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void OV5653_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5653_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5653_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5653_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5653_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1,ulReg2, ulData1;

    i2c_lock();
    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (uiAddr>>8);
    ulReg2      = ((uiAddr)&(0x000000ff));
    ulData1     = uiValue;
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
    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

static void OV5653_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1,ulReg2, ulData, ulData1;

    i2c_lock();

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

        erReturn=i2c_masterReceive(&ulData1, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);
    ulData=ulData1;
    i2c_unlock();

    *uiValue = ulData;
}

static void OV5653_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gOV5653Status;
}

static void OV5653_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     OV5653_SetStatus(uiMode,pStatus);
}

static void OV5653_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    OV5653_GetDataInfo(uiMode,&Data,NULL);
    OV5653_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
        case SENSOR_MODE_640X480:
        case SENSOR_MODE_1280X960:
        case SENSOR_MODE_1280X480:
        case SENSOR_MODE_2560X960:
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
        case SENSOR_MODE_1280X720:
            pStatus->uiSensorMode = uiMode;
            pStatus->uiReadoutMode = 0; //SENSOR_OUT_PROGRESSIVE;
            pStatus->uiRatio = SENSOR_RATIO_16_9;;
            pStatus->uiBayer = ((PREVIEW_DATA_INFO*)Data)->uiBayer;
            pStatus->uiPclk = ((PREVIEW_DATA_INFO*)Data)->uiPclk;
             //#NT#2011/11/23#Ethan Lau -begin
            pStatus->uiBinning = ((PREVIEW_DATA_INFO*)Data)->uiBinning;
            pStatus->uiSkip = ((PREVIEW_DATA_INFO*)Data)->uiSkip;
            //#NT#2011/11/23#Ethan Lau -end
            pStatus->pSyncInfo = ((SYNC_INFO*)Sync);
            break;
        case SENSOR_MODE_1280X720P50:
        case SENSOR_MODE_1280X720P60:
        case SENSOR_MODE_1920X1080:
            pStatus->uiSensorMode = uiMode;
            pStatus->uiReadoutMode = 0; //SENSOR_OUT_PROGRESSIVE;
            pStatus->uiRatio = SENSOR_RATIO_16_9;;
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

static void OV5653_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    OV5653_Init();

    OV5653_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        OV5653_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        OV5653_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    //#NT#2011/07/11#Ethan Lau -begin
    //#NT#get cur mode max expo line
    {
        UINT32 low,high;
        low = Sensor_ReadReg(0x380f);
        high = Sensor_ReadReg(0x380e);
        //debug_msg("a(%2x%2x)\r\n",high,low);
        m_uiAE_Maxline = ((low+(high<<8))-6);
    }
    //#NT#2011/07/11#Ethan Lau -end
    OV5653_SetStatus(uiMode,&gOV5653Status);
}

/**
 * AE driver API
 */

static void OV5653_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gOV5653ExpoSetting;
}

static void OV5653_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;

    temp_line = uiTime * gOV5653Status.uiPclk / gOV5653Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0001)
       temp_line = 0x0001;

    gOV5653ExpoSetting.uiFrame = 0;
    gOV5653ExpoSetting.uiLine = temp_line;
    gOV5653ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gOV5653Status.pSyncInfo->HD_total / gOV5653Status.uiPclk;
    else
        linetime = uiTime;

    if (linetime < uiTime)
        gOV5653ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    else
        gOV5653ExpoSetting.uiExpoRatio = 100;
}

static void OV5653_CalGain(UINT32 uiIso)
{
    CAL_DATA CalInfo;
    UINT32 Cal_gain=0, fragain =0;
    UINT32 count=0,decimal=0;
    UINT32 AGCBase,ISOBase;
    OV5653_GetISOInfo(&ISOBase, &AGCBase);

    Cal_GetCalData(&CalInfo);

    if(CalInfo.CalStatus_ISO == _ISO_Status)
        Cal_gain = CalInfo.Cal_ISO_Data[0];
    else
        {
        Cal_gain = AGCBase;
        Cal_gain= Cal_gain * 100 * uiIso / ISOBase / 100;
        }
    if(Cal_gain < 1000)    Cal_gain = 1000;

    while(1)
    {
        if(Cal_gain>=2*1000)
        {
            Cal_gain = Cal_gain>>1;
            count++;
        }
        else
            break;
    }

    if(count==4)
        fragain = 0x7f;
    else
    {
        decimal = 16*(Cal_gain-1000)/1000;
        while(count>0)
            fragain += (0x10*(1<<(--count)));

        fragain+=decimal;
    }

    gOV5653ExpoSetting.uiGain= fragain;
}

static void OV5653_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void OV5653_SetLine(UINT32 uiLine)
{
    UINT32 High, Middle, Low, overframeline = 0;
    //nAE_Maxline = GetAeMaxExptline();

    if(uiLine > m_uiAE_Maxline)
    {
        overframeline = uiLine - m_uiAE_Maxline;

        High = (overframeline>>8)&0x00ff;
        Low = overframeline&0x00ff;

        //OV5653 pixel array crop will be shifted when length dummy line
        if(Low == 2)
            Low = 3;

        OV5653_WriteReg(0x350d,Low);
        OV5653_WriteReg(0x350c,High);
    }
    else
    {
       OV5653_WriteReg(0x350d,0);
       OV5653_WriteReg(0x350c,0);
    }
    uiLine = uiLine << 4;//exposure uint is in 1/16 line
    uiLine = uiLine & 0x0fffff;
    High = (uiLine >> 16) & 0xf;
    Middle = (uiLine >> 8) & 0x0ff;
    Low = uiLine & 0x000ff;
    OV5653_WriteReg(0x3500, High);
    OV5653_WriteReg(0x3501, Middle);
    OV5653_WriteReg(0x3502, Low);
}

static void OV5653_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void OV5653_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void OV5653_SetGain(UINT32 uiGain)
{
    UINT32 AGC;

    AGC = uiGain  & 0x000000ff;
    //debug_msg(">>OV5653_SetGain=%d\r\n", AGC);
    OV5653_WriteReg(0x350B, AGC); //analog gain
}

//@@ EOF
