/**
    @file       S5K3E2.c

    @brief      S5K3E2

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
#include "S5K3E2.h"
#include "Calibration.h"
#include "ImageAlgInfor.h"

#define SEN_READ_ID 0x21
#define SEN_WRITE_ID 0x20

#define SENDRV_DEBUG  DISABLE//DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif

//#NT#2011/07/11#Ethan Lau -begin
static UINT32   m_uiAE_Maxline;
//#NT#2011/07/11#Ethan Lau -end
// function prototype
static void S5K3E2_SifInit(UINT32 *pPort);
static void S5K3E2_Init(void);
static void S5K3E2_PwrOn(UINT32 uiStage);
static void S5K3E2_PwrOff(UINT32 uiStage);
static void S5K3E2_Sleep(UINT32 uiStage);
static void S5K3E2_WakeUp(UINT32 uiStage);
static void S5K3E2_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void S5K3E2_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void S5K3E2_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void S5K3E2_GetCurStatus(PSENSOR_STATUS pStatus);
static void S5K3E2_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void S5K3E2_ChgMode(UINT32 uiMode);
static void S5K3E2_GetExpo(EXPOSURE_SETTING *Expo);
static void S5K3E2_CalLine(UINT32 uiTime);
static void S5K3E2_CalGain(UINT32 uiIso);
static void S5K3E2_SetFrame(UINT32 uiFrame);
static void S5K3E2_SetLine(UINT32 uiLine);
static void S5K3E2_SetPixel(UINT32 uiStep);
static void S5K3E2_SetVDLength(UINT32 uiVDLength);
static void S5K3E2_SetGain(UINT32 uiGain);
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL S5K3E2_checkExposureLongTime(void);
static void S5K3E2_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 S5K3E2_GetCalMshutterDelayTime(void);
static void S5K3E2_TriggerGlobalReset(void);
static void S5K3E2_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pS5K3E2Drv =
{
    // TG control
    S5K3E2_SifInit,
    S5K3E2_Init,
    S5K3E2_PwrOn,
    S5K3E2_PwrOff,
    S5K3E2_Sleep,
    S5K3E2_WakeUp,
    S5K3E2_WriteReg,
    S5K3E2_ReadReg,
    S5K3E2_GetStatus,
    S5K3E2_GetCurStatus,
    S5K3E2_ChgMode,
    //feature,
    NULL,
    // AE
    S5K3E2_GetExpo,
    S5K3E2_CalLine,
    S5K3E2_CalGain,
    S5K3E2_SetFrame,
    S5K3E2_SetLine,
    S5K3E2_SetPixel,
    S5K3E2_SetVDLength,
    S5K3E2_SetGain,
    S5K3E2_checkExposureLongTime,
    S5K3E2_SetCalMshutterDelayTime,
    S5K3E2_GetCalMshutterDelayTime,
    S5K3E2_TriggerGlobalReset,
    S5K3E2_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gS5K3E2Status;
static EXPOSURE_SETTING gS5K3E2ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pS5K3E2Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pS5K3E2Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        S5K3E2_prv_mode,  // pPrvMode
        S5K3E2_still_mode   // pStillMode
    };
    //g_pS5K3E2Info.pPrvMode = &S5K3E2_prv_mode[CurrentSensorMode];
    return &g_pS5K3E2Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void S5K3E2_SifInit(UINT32 *pPort)
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

static void S5K3E2_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("S5K3E2_init...\r\n");

    S5K3E2_GetRegInfo(S5K3E2_INIT, &CmdAddr, &CmdLength);
    TimerDelayMs(100);

    //soft reset
    S5K3E2_WriteReg(0x0103, 0x01);

    TimerDelayMs(100);

    for (idx = 0;idx < CmdLength;idx++)
    {
        S5K3E2_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        S5K3E2_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    // Initial local status
    memset(&gS5K3E2Status, 0, sizeof(SENSOR_STATUS));
    memset(&gS5K3E2ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void S5K3E2_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void S5K3E2_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void S5K3E2_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void S5K3E2_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void S5K3E2_WriteReg(UINT32 uiAddr, UINT32 uiValue)
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

static void S5K3E2_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
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

static void S5K3E2_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gS5K3E2Status;
}

static void S5K3E2_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     S5K3E2_SetStatus(uiMode,pStatus);
}

static void S5K3E2_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    S5K3E2_GetDataInfo(uiMode,&Data,NULL);
    S5K3E2_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
        case SENSOR_MODE_320X240:
        case SENSOR_MODE_320X240P60:
        case SENSOR_MODE_320X240P120:
        case SENSOR_MODE_640X480:
        case SENSOR_MODE_640X480P60:
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

static void S5K3E2_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;
    //#NT#2012/05/20#KS Hung -begin
    //#NT#It is a pointer, but it doesn't point to a memory space.
    //ImageAlgInfo  *gImageAlgInfo;
    //#NT#2012/05/20#KS Hung -end

    S5K3E2_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    //#NT#2012/05/27#KS Hung -begin
    //if (gImageAlgInfo.DriveMode == _IMGDriveMode_Single)
    //    Sensor_WriteReg(0x0103,0x01);
    //#NT#2012/05/27#KS Hung -end

    for (idx = 0; idx < CmdLength; idx++)
    {
        S5K3E2_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        S5K3E2_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
   // TimerDelayMs(300);
#if 1
    //#NT#2011/07/11#Ethan Lau -begin
    //#NT#get cur mode max expo line
    {
        UINT32 low,high;
        low = Sensor_ReadReg(0x0341);//frame legth high byte
        high = Sensor_ReadReg(0x0340);//frame legth low byte
        //debug_msg("a(%2x%2x)\r\n",high,low);
        m_uiAE_Maxline = ((low+(high<<8))-6);
    }
    //#NT#2011/07/11#Ethan Lau -end
#endif
    S5K3E2_SetStatus(uiMode,&gS5K3E2Status);
}

/**
 * AE driver API
 */

static void S5K3E2_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gS5K3E2ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL S5K3E2_checkExposureLongTime(void)
{
    return FALSE;
}

static void S5K3E2_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 S5K3E2_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void S5K3E2_TriggerGlobalReset(void)
{

}

static void S5K3E2_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void S5K3E2_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;
    //debug_msg(">>S5K3E2_CalLine=%d,%d\r\n",  gS5K3E2Status.uiPclk,gS5K3E2Status.pSyncInfo->HD_total);
    temp_line = uiTime * gS5K3E2Status.uiPclk / gS5K3E2Status.pSyncInfo->HD_total;
    //debug_msg(">>temp_line=%d,uiTime = %d\r\n",  temp_line,uiTime);
    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0001)
       temp_line = 0x0001;

    gS5K3E2ExpoSetting.uiFrame = 0;
    gS5K3E2ExpoSetting.uiLine = temp_line;
    gS5K3E2ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gS5K3E2Status.pSyncInfo->HD_total / gS5K3E2Status.uiPclk;
    else
        linetime = uiTime;

    if (linetime < uiTime)
        gS5K3E2ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    else
        gS5K3E2ExpoSetting.uiExpoRatio = 100;
}

static void S5K3E2_CalGain(UINT32 uiIso)
{
    CAL_DATA CalInfo;
    UINT32 Cal_gain=0, fragain =0;
    UINT32 AGCBase,ISOBase;
    S5K3E2_GetISOInfo(&ISOBase, &AGCBase);

    Cal_GetCalData(&CalInfo);

    //if(uiIso > 800)
    //    uiIso = 800;

    if(CalInfo.CalStatus_ISO == _ISO_Status)
        Cal_gain = CalInfo.Cal_ISO_Data[0];
    else
        Cal_gain = AGCBase;

    Cal_gain= Cal_gain * 100 * uiIso / ISOBase / 100;

/**************************************************
    1.Manual Analog Gain --> 0x0204[7:0] high byte, 0x0205[7:0],low byte,  for analog
    gain =(Bit[15:0]/32)

    ex.     gain 1x=> 0x0204=0x00,0x0205=0x0020
        gain 2x=> 0x0204=0x00,0x0205=0x40
        gain 3x=> 0x0204=0x00,0x0205=0x60
        gain 4x=> 0x0204=0x00,0x0205=0x80
        gain 6x=> 0x0204=0x00,0x0205=0xc0
        gain 8x=> 0x0204=0x01,0x0205=0x00

*******************************************************/
 // debug_msg(">>Cal_gain=%d\r\n",Cal_gain);
    if(Cal_gain<1000)
        Cal_gain = 1000;

    fragain= (Cal_gain) * 32 / 1000;
#if 0
    if(Cal_gain<2000)// 1x ~ 2x gain
    {
        fragain= (Cal_gain-1000) * 32 / 1000;
     fragain|=0x0020;
    }
    else if(Cal_gain<4000)// 2x ~ 4x gain
    {
        Cal_gain /=2;
        fragain= (Cal_gain-1000) * 32 / 1000;
        fragain|=0x0060;
    }
    else if(Cal_gain<8000)// 4x ~ 8x gain
    {
        Cal_gain /=4;
        fragain= (Cal_gain-1000) * 32 / 1000;
        fragain|=0x00e0;
    }
    else if(Cal_gain<16000)// 8x ~ 16x gain
    {
        Cal_gain /=8;
        fragain= (Cal_gain-1000) * 32 / 1000;
        fragain|=0x01e0;
    }
   else if(Cal_gain<16000)// 16x ~ 32x gain
    {
        Cal_gain /=16;
        fragain= (Cal_gain-1000) * 32 / 1000;
        fragain|=0x03e0;
    }
     else if(Cal_gain<32000)// 32x ~ 64x gain
    {
        Cal_gain /=32;
        fragain= (Cal_gain-1000) * 32 / 1000;
        fragain|=0x07e0;
    }
    else
        fragain = 0xffff;
#endif
    //debug_msg(">>fragain=%x\r\n",fragain);
    gS5K3E2ExpoSetting.uiGain= fragain;
}

static void S5K3E2_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void S5K3E2_SetLine(UINT32 uiLine)
{
    UINT32 High, Low;

    S5K3E2_WriteReg(0x0104, 1);// coarse integration,uint line,
    if(uiLine > (gS5K3E2Status.pSyncInfo->VD_total-16))
    {
        High = (uiLine>>8)&0x00ff;
        Low = uiLine&0x00ff;
        S5K3E2_WriteReg(0x0202, High);// coarse integration,uint line,
        S5K3E2_WriteReg(0x0203, Low);// coarse integration

        uiLine+=16;// VD should >= exposure line+16 (by CoAsia)
        High = (uiLine>>8)&0x00ff;
        Low = uiLine&0x00ff;
        S5K3E2_WriteReg(0x0340,High);
        S5K3E2_WriteReg(0x0341,Low);

    }
    else
    {
        High = (uiLine>>8)&0x00ff;
        Low = uiLine&0x00ff;
        S5K3E2_WriteReg(0x0202, High);// coarse integration,uint line,
        S5K3E2_WriteReg(0x0203, Low);// coarse integration

        High = (gS5K3E2Status.pSyncInfo->VD_total>>8)&0x00ff;
        Low = gS5K3E2Status.pSyncInfo->VD_total&0x00ff;
        S5K3E2_WriteReg(0x0340,High);
        S5K3E2_WriteReg(0x0341,Low);
    }
    S5K3E2_WriteReg(0x0104, 0);// coarse integration,uint line,
}

static void S5K3E2_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void S5K3E2_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void S5K3E2_SetGain(UINT32 uiGain)
{
    UINT32 AGC;
#if 0
    S5K3E2_ReadReg(0x0202,&AGC);
       debug_msg(">>uiLine=%x\r\n",AGC);

    S5K3E2_ReadReg(0x0203,&AGC)
    debug_msg(">>uiLine=%x\r\n",AGC);

    return;
#endif
    //
   //     debug_msg(">>S5K3E2_SetGain=%x\r\n", uiGain);

    AGC = uiGain  & 0x000000ff;//low byte
    S5K3E2_WriteReg(0x0205, AGC); //analog gain
    AGC = (uiGain>>8)&0x00ff;//high byte
    S5K3E2_WriteReg(0x0204, AGC); //analog gain
}

//@@ EOF
