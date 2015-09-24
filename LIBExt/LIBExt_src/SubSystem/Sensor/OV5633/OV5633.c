/**
    @file       OV5633.c

    @brief      OV5633

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
#include "OV5633.h"
#include "Calibration.h"

#define SEN_READ_ID 0x6D
#define SEN_WRITE_ID 0x6C

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif

// function prototype
static void OV5633_SifInit(UINT32 *pPort);
static void OV5633_Init(void);
static void OV5633_PwrOn(UINT32 uiStage);
static void OV5633_PwrOff(UINT32 uiStage);
static void OV5633_Sleep(UINT32 uiStage);
static void OV5633_WakeUp(UINT32 uiStage);
static void OV5633_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void OV5633_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void OV5633_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void OV5633_GetCurStatus(PSENSOR_STATUS pStatus);
static void OV5633_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void OV5633_ChgMode(UINT32 uiMode);
static void OV5633_GetExpo(EXPOSURE_SETTING *Expo);
static void OV5633_CalLine(UINT32 uiTime);
static void OV5633_CalGain(UINT32 uiIso);
static void OV5633_SetFrame(UINT32 uiFrame);
static void OV5633_SetLine(UINT32 uiLine);
static void OV5633_SetPixel(UINT32 uiStep);
static void OV5633_SetVDLength(UINT32 uiVDLength);
static void OV5633_SetGain(UINT32 uiGain);
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL OV5633_checkExposureLongTime(void);
static void OV5633_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 OV5633_GetCalMshutterDelayTime(void);
static void OV5633_TriggerGlobalReset(void);
static void OV5633_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pOV5633Drv =
{
    // TG control
    OV5633_SifInit,
    OV5633_Init,
    OV5633_PwrOn,
    OV5633_PwrOff,
    OV5633_Sleep,
    OV5633_WakeUp,
    OV5633_WriteReg,
    OV5633_ReadReg,
    OV5633_GetStatus,
    OV5633_GetCurStatus,
    OV5633_ChgMode,
    //feature,
    NULL,
    // AE
    OV5633_GetExpo,
    OV5633_CalLine,
    OV5633_CalGain,
    OV5633_SetFrame,
    OV5633_SetLine,
    OV5633_SetPixel,
    OV5633_SetVDLength,
    OV5633_SetGain,
    OV5633_checkExposureLongTime,
    OV5633_SetCalMshutterDelayTime,
    OV5633_GetCalMshutterDelayTime,
    OV5633_TriggerGlobalReset,
    OV5633_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gOV5633Status;
static EXPOSURE_SETTING gOV5633ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pOV5633Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pOV5633Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        OV5633_prv_mode,  // pPrvMode
        OV5633_still_mode   // pStillMode
    };
    //g_pOV5633Info.pPrvMode = &OV5633_prv_mode[CurrentSensorMode];
    return &g_pOV5633Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void OV5633_SifInit(UINT32 *pPort)
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

static void OV5633_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("OV5633_init...\r\n");

    OV5633_GetRegInfo(OV5633_INIT, &CmdAddr, &CmdLength);
    TimerDelayMs(100);

    //soft reset
    OV5633_WriteReg(0x3012, 0x80);

    TimerDelayMs(100);

    for (idx = 0;idx < CmdLength;idx++)
    {
        OV5633_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        OV5633_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    // Initial local status
    memset(&gOV5633Status, 0, sizeof(SENSOR_STATUS));
    memset(&gOV5633ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void OV5633_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5633_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5633_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5633_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV5633_WriteReg(UINT32 uiAddr, UINT32 uiValue)
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

static void OV5633_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
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

static void OV5633_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gOV5633Status;
}

static void OV5633_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     OV5633_SetStatus(uiMode,pStatus);
}

static void OV5633_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    OV5633_GetDataInfo(uiMode,&Data,NULL);
    OV5633_GetSigInfo(uiMode,&Sync,NULL);
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

static void OV5633_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    OV5633_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        OV5633_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        OV5633_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    OV5633_SetStatus(uiMode,&gOV5633Status);
}

/**
 * AE driver API
 */

static void OV5633_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gOV5633ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL OV5633_checkExposureLongTime(void)
{
    return FALSE;
}

static void OV5633_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 OV5633_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void OV5633_TriggerGlobalReset(void)
{

}

static void OV5633_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void OV5633_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;

    temp_line = uiTime * gOV5633Status.uiPclk / gOV5633Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0001)
       temp_line = 0x0001;

    gOV5633ExpoSetting.uiFrame = 0;
    gOV5633ExpoSetting.uiLine = temp_line;
    gOV5633ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gOV5633Status.pSyncInfo->HD_total / gOV5633Status.uiPclk;
    else
        linetime = uiTime;

    if (linetime < uiTime)
        gOV5633ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    else
        gOV5633ExpoSetting.uiExpoRatio = 100;
}

static void OV5633_CalGain(UINT32 uiIso)
{
    CAL_DATA CalInfo;
    UINT32 Cal_gain=0, fragain =0;
    UINT32 AGCBase,ISOBase;
    OV5633_GetISOInfo(&ISOBase, &AGCBase);

    Cal_GetCalData(&CalInfo);

    if(uiIso > 800)
        uiIso = 800;

    if(CalInfo.CalStatus_ISO == _ISO_Status)
        Cal_gain = CalInfo.Cal_ISO_Data[0];
    else
        Cal_gain=AGCBase;

    Cal_gain = Cal_gain*uiIso/ISOBase;

/**************************************************
    1.Manual Analog Gain --> 0x3013[2]=0 , 0x3300[3]=1, 0x3302[6]=1; 0x3000[6..0] for analog
    gain =(Bit[6]+1)x(Bit[5]+1)x(Bit[4]+1)x(1+Bit[3:0]/16)

    ex.     gain 1x=> 0x3000=0x00
        gain 2x=> 0x3000=0x10
        gain 3x=> 0x3000=0x18
        gain 4x=> 0x3000=0x30
        gain 6x=> 0x3000=0x38
        gain 8x=> 0x3000=0x70
        only valid within~
                    0x10~0x1f
                    0x30~0x3f
                    0x70~0x7f
        Sensor_WriteReg(0x3000, AGC_Idx);
*******************************************************/

    if(Cal_gain<1000)
        Cal_gain = 1000;

    if(Cal_gain<2000)// 1x ~ 2x gain
    {
        fragain= (Cal_gain-1000) * 16 / 1000;
    }
    else if(Cal_gain<4000)// 2x ~ 4x gain
    {
        Cal_gain /=2;
        fragain= (Cal_gain-1000) * 16 / 1000;
        fragain|=0x10;
    }
    else if(Cal_gain<8000)// 4x ~ 8x gain
    {
        Cal_gain /=4;
        fragain= (Cal_gain-1000) * 16 / 1000;
        fragain|=0x30;
    }
    else if(Cal_gain<16000)
    {
        Cal_gain /=8;
        fragain= (Cal_gain-1000) * 16 / 1000;
        fragain|=0x70;
    }
    else
        fragain = 0x7f;

    gOV5633ExpoSetting.uiGain= fragain;
}

static void OV5633_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

UINT32 dummyline = 0;
// OV5633 use extend lin for long time exposure
void SetPreExtendline(UINT32 nEntendLine)
{
    UINT16 Highbyte,Lowbyte;

    Highbyte = (UINT16)nEntendLine&0X0000FF00;
    Highbyte = Highbyte>>8;
    Lowbyte = (UINT16)nEntendLine&0X000000FF;
    //debug_err(("nEntendLine = %6ld\r\n", nEntendLine));
    OV5633_WriteReg(0x301e, Highbyte);
    OV5633_WriteReg(0x301f, Lowbyte);
}


static void OV5633_SetLine(UINT32 uiLine)
{
    UINT32 High,Low;
    //nAE_Maxline = GetAeMaxExptline();

    if(uiLine > gOV5633Status.pSyncInfo->VD_total)
    {
        dummyline = uiLine - gOV5633Status.pSyncInfo->VD_total;

        High = (gOV5633Status.pSyncInfo->VD_total>>8)&0x00ff;
        Low = gOV5633Status.pSyncInfo->VD_total&0x00ff;
    }
    else
    {
        High = (uiLine>>8)&0x00ff;
        Low = uiLine&0x00ff;
        dummyline = 0;
    }
    OV5633_WriteReg(0x3002, High);
    OV5633_WriteReg(0x3003, Low);
    if(dummyline!=0)
        SetPreExtendline(dummyline);
}

static void OV5633_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void OV5633_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void OV5633_SetGain(UINT32 uiGain)
{
    //debug_msg(">>OV5633_SetGain=%d\r\n", AGC);
    OV5633_WriteReg(0x3000, uiGain); //analog gain
    if(dummyline == 0)
        SetPreExtendline(dummyline);
}
//@@ EOF
