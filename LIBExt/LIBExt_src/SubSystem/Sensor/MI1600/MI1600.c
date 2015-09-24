/**
    @file       MI1600.c

    @brief      MI1600

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
#include "MI1600.h"

#define SEN_READ_ID 0xB9
#define SEN_WRITE_ID 0xB8

#define SEN_EXPOTIME  ENABLE
#define SEN_GAIN      ENABLE

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif

// function prototype
static void MI1600_SifInit(UINT32 *pPort);
static void MI1600_Init(void);
static void MI1600_PwrOn(UINT32 uiStage);
static void MI1600_PwrOff(UINT32 uiStage);
static void MI1600_Sleep(UINT32 uiStage);
static void MI1600_WakeUp(UINT32 uiStage);
static void MI1600_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI1600_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI1600_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI1600_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI1600_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI1600_ChgMode(UINT32 uiMode);
static void MI1600_GetExpo(EXPOSURE_SETTING *Expo);
static void MI1600_CalLine(UINT32 uiTime);
static void MI1600_CalGain(UINT32 uiIso);
static void MI1600_SetFrame(UINT32 uiFrame);
static void MI1600_SetLine(UINT32 uiLine);
static void MI1600_SetPixel(UINT32 uiStep);
static void MI1600_SetVDLength(UINT32 uiVDLength);
static void MI1600_SetGain(UINT32 uiGain);
static void MI1600_Flip(UINT32 Direction);
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI1600_checkExposureLongTime(void);
static void MI1600_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 MI1600_GetCalMshutterDelayTime(void);
static void MI1600_TriggerGlobalReset(void);
static void MI1600_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pMI1600Drv =
{
    // TG control
    MI1600_SifInit,
    MI1600_Init,
    MI1600_PwrOn,
    MI1600_PwrOff,
    MI1600_Sleep,
    MI1600_WakeUp,
    MI1600_WriteReg,
    MI1600_ReadReg,
    MI1600_GetStatus,
    MI1600_GetCurStatus,
    MI1600_ChgMode,
    //feature,
    MI1600_Flip,
    // AE
    MI1600_GetExpo,
    MI1600_CalLine,
    MI1600_CalGain,
    MI1600_SetFrame,
    MI1600_SetLine,
    MI1600_SetPixel,
    MI1600_SetVDLength,
    MI1600_SetGain,
    MI1600_checkExposureLongTime,
    MI1600_SetCalMshutterDelayTime,
    MI1600_GetCalMshutterDelayTime,
    MI1600_TriggerGlobalReset,
    MI1600_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gMI1600Status;
static EXPOSURE_SETTING gMI1600ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI1600Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI1600Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI1600_prv_mode,  // pPrvMode
        MI1600_still_mode   // pStillMode
    };
    //g_pMI1600Info.pPrvMode = &MI1600_prv_mode[CurrentSensorMode];
    return &g_pMI1600Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI1600_SifInit(UINT32 *pPort)
{
    UINT erReturn;

    erReturn = i2c_open();

    if(erReturn != 0 /*E_OK*/)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }

    i2c_enable();

    //pinmux_select_i2c(PINMUX_I2C);

    //from I2C AppNote:
    //I2CSCL-freq = APB_Clk / (2*CLK_DIV + 6 + GSR)
    //I2CSCL-freq = 80M Hz / (2*CLK_DIV + 6 + GSR)
    //TSR  = CLK_DIV/2
    //Limitation: CLK_DIV must be larger than 3+GSR+TSR

    //Ex: we want 100K Hz for I2C bus frequence
    //GSR = 7 = 0x07!
    //I2C BUS = 100000 = 80*1000*1000/(2*CLK_DIV+6+7) = 80000000/(2*CLK_DIV+13)
    //2*CLK_DIV+13 = 80000000/100000
    //2*CLK_DIV+13 = 800
    //2*CLK_DIV = 787
    //CLK_DIV = 393 = 0x189!
    //TSR  = CLK_DIV/2 = 393/2 = 0xC4!

    //erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_100KHZ, I2C_TSR_100KHZ, I2C_GSR_100KHZ);
    erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_200KHZ, I2C_TSR_200KHZ, I2C_GSR_200KHZ);

    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }
}

static void MI1600_Init(void)
{
    UINT32 idx,CmdLength;
    UINT32 CmdAddr = 0;
    #if (SENDRV_DEBUG == ENABLE)
    UINT32 value;
    #endif

    SENDRV_MSG("MI1600_init...\r\n");

    MI1600_GetRegInfo(MI1600_INIT, &CmdAddr, &CmdLength);
    //TimerDelayMs(100);

    //soft reset
    MI1600_WriteReg(0x0D, 0x0001);
    MI1600_WriteReg(0x0D, 0x0000);

    //PLL config
    for (idx = 0;idx < CmdLength;idx++)
    {
        MI1600_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
    }
    TimerDelayMs(10);
    MI1600_GetRegInfo(MI1600_INIT2, &CmdAddr, &CmdLength);
    //recommended setting
    for (idx = 0;idx < CmdLength;idx++)
    {
        MI1600_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
    }

    //restart
    MI1600_WriteReg(0x0B, 0x0001);
    TimerDelayMs(100);
    MI1600_WriteReg(0x0B, 0x0000);


    // Initial local status
    memset(&gMI1600Status, 0, sizeof(SENSOR_STATUS));
    memset(&gMI1600ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void MI1600_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1600_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1600_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1600_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI1600_WriteReg(UINT32 uiAddr, UINT32 uiValue)
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

static void MI1600_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
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

static void MI1600_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gMI1600Status;
}

static void MI1600_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI1600_SetStatus(uiMode,pStatus);
}

static void MI1600_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    MI1600_GetDataInfo(uiMode,&Data,NULL);
    MI1600_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
        case SENSOR_MODE_640X480:
        case SENSOR_MODE_1440X1080:
        case SENSOR_MODE_1440X540:
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
            pStatus->uiRatio = SENSOR_RATIO_16_9;
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

static void MI1600_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    MI1600_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        MI1600_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        MI1600_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    MI1600_SetStatus(uiMode,&gMI1600Status);
}

/**
 * AE driver API
 */

static void MI1600_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI1600ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI1600_checkExposureLongTime(void)
{
    return FALSE;
}

static void MI1600_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 MI1600_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void MI1600_TriggerGlobalReset(void)
{

}

static void MI1600_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void MI1600_CalLine(UINT32 uiTime)
{
#if (SEN_EXPOTIME == ENABLE)
    UINT32 temp_line,linetime;
    linetime=gMI1600Status.pSyncInfo->HD_total/gMI1600Status.uiPclk;
    //linetime=(gMI1600Status.pSyncInfo->HD_total + (gMI1600Status.uiPclk - 1))/gMI1600Status.uiPclk;

    temp_line = uiTime/linetime;
    //temp_line = (uiTime + (linetime-1))/linetime;

    if(temp_line > 0xFFFFF)
       temp_line = 0xFFFFF;

    if(temp_line < 0x0001)
       temp_line = 0x0001;

    //fill exposure frame (optional)
    gMI1600ExpoSetting.uiFrame = 0;
    //fill exposure line
    gMI1600ExpoSetting.uiLine = temp_line;
    //fill exposure pixel (optional)
    gMI1600ExpoSetting.uiPixel = 0;

    //calculate real_linetime = linetime * temp_line
    if(temp_line != 0xFFFFF)
        linetime *= temp_line;
    else
        linetime = uiTime;

    //fill ratio of (ideal expo_time)/(real expo_time), this ratio >= 100
    if(linetime < uiTime)
        gMI1600ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    else
        gMI1600ExpoSetting.uiExpoRatio = 100;
    //debug_msg("AE: expotime=%d, expoline=%d, ratio=%d\r\n", uiTime, gMI1600ExpoSetting.uiLine, gMI1600ExpoSetting.uiExpoRatio);
#endif
}

static void MI1600_CalGain(UINT32 uiIso)
{
#if (SEN_GAIN == ENABLE)
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 uiSensorGain = 0;
    UINT32 AGCBase,ISOBase;
    UINT32 factor = 0;
    UINT32 d_gain = 0;
    UINT32 a_mul = 0;
    UINT32 a_gain = 0;
    MI1600_GetISOInfo(&ISOBase, &AGCBase);
    //------------------------------------------------------------------------------------------
    //calculate Gain_factor from ISO value

    //scale base on ISO50
    Calcul_AGC_Ratio = 100 * uiIso / ISOBase;
    //=> ISO50 = 100, ISO100 = 200, ISO200 = 400, ISO400 = 800, ...

    Gain_factor = AGCBase * Calcul_AGC_Ratio / 100;
    //Gain_factor = ((Calcul_AGC_Ratio-100)*100+(AGC_ISO50>>1))/AGC_ISO50 + ADJ_ISO;
    //=> ISO50 = 1000, ISO100 = 2000, ISO200 = 4000, ISO400 = 8000, ...

    //------------------------------------------------------------------------------------------
    //1x gain=1000, 2x gain=2000, 4x gain=4000

    //convert to real sensor gain:
    //a_gain [0~5] = (AGAIN/16)     AGAIN=0~63  gain=0~3.9375
    //a_mul [6] = (1+AMUL)          AMUL=0~1    gain=1~2
    //d_gain [8~14]= 1+(DGAIN/8)    DGAIN=0~120 gain=1~16
    //total_gain = a_gain*a_mul*d_gain

    factor = Gain_factor;
    //lower bound clamp
    if (factor < 63) //min Agc = 0.0625x
    {
        factor = 63;
    }
    //upper bound clamp
    else if (factor > 126000) //max Agc = 126x = 3.9375*2*16
    {
        factor = (126000-1);
    }
#if 0
    //Agc 8x ~ 126x
    if (factor > 8000)
    {
        //d_gain range = (1+(DGAIN/8)) * 8 * 1000 = 8000 + DGAIN*1000
        d_gain = (factor - 8000)/1000;
        factor = (factor*8)/(8+d_gain); //update remain factor
    }
    //Agc 4x ~ 8x
    if (factor > 4000)
    {
        a_mul = 1;
        factor /= (1+a_mul); //update remain factor
    }
    //Agc 0x ~ 4x
    {
        a_gain = factor*64/4000;
    }
#endif
#if 0
    if (factor < 4000)      //Agc 0x ~ 4x
    {
        //d_gain(1x) = 0
        d_gain = 0;
        //a_mul(1x) = 0
        a_mul = 0;

        //a_gain: factor = 1000~4000 (1x~4x)
        a_gain = factor*16/1000;
    }
    else if (factor < 8000) //Agc 4x ~ 8x
    {
        //d_gain(1x) = 8*1-8
        d_gain = 8*1-8; //0
        //a_mul(2x) = 1
        a_mul = 1;

        //remove effect of d_gain & a_mul
        //factor /= 2;
        //a_gain: factor = 1000~4000 (1x~4x)
        //a_gain = factor*16/1000/2;
        a_gain = factor*8/1000;
    }
    else if (factor < 16000) //Agc 8x ~ 16x
    {
        //d_gain(2x) = 8*2-8
        d_gain = 8*2-8; //8
        //a_mul(2x) = 1
        a_mul = 1;

        //remove effect of d_gain & a_mul
        //factor /= 4;
        //a_gain: factor = 1000~4000 (1x~4x)
        //a_gain = factor*16/1000/4;
        a_gain = factor*4/1000;
    }
    else if (factor < 32000) //Agc 16x ~ 32x
    {
        //d_gain(4x) = 8*4-8
        d_gain = 8*4-8; //24
        //a_mul(2x) = 1
        a_mul = 1;

        //remove effect of d_gain & a_mul
        //factor /= 8;
        //a_gain: factor = 1000~4000 (1x~4x)
        //a_gain = factor*16/1000/8;
        a_gain = factor*2/1000;
    }
    else if (factor < 64000) //Agc 32x ~ 64x
    {
        //d_gain(8x) = 8*8-8
        d_gain = 8*8-8; //56
        //a_mul(2x) = 1
        a_mul = 1;

        //remove effect of d_gain & a_mul
        //factor /= 16;
        //a_gain: factor = 1000~4000 (1x~4x)
        //a_gain = factor*16/1000/16;
        a_gain = factor/1000;
    }
    else if (factor < 126000) //Agc 64x ~ 126x
    {
        //d_gain(16x) = 8*16-8
        d_gain = 8*16-8; //120
        //a_mul(2x) = 1
        a_mul = 1;

        //remove effect of d_gain & a_mul
        //factor /= 32;
        //a_gain: factor = 1000~4000 (1x~4x)
        //a_gain = factor*16/1000/32;
        a_gain = factor/500;
    }
#endif
    if (factor < 4000)      //Agc 0x ~ 4x
    {
        //UINT32 s_factor;
        a_mul = 0;
        a_gain = factor*16/1000;
        //s_factor = a_gain*(1+a_mul)*1000/16;
        d_gain = 0;
        //s_factor = s_factor*(1+(d_gain/8));
        //debug_msg("AE: 1 f=%d, d_gain=%d, a_mul=%d, a_gain=%d, sf=%d\r\n", factor, d_gain, a_gain, a_mul, s_factor);
    }
    else if (factor < 8000) //Agc 4x ~ 8x
    {
        //UINT32 s_factor;
        a_mul = 1;
        a_gain = factor*8/1000;
        //s_factor = a_gain*(1+a_mul)*1000/16;
        d_gain = 0;
        //s_factor = s_factor*(1+(d_gain/8));
        //debug_msg("AE: 2 f=%d, d_gain=%d, a_mul=%d, a_gain=%d, sf=%d\r\n", factor, d_gain, a_gain, a_mul, s_factor);
    }
    else if (factor < 126000) //Agc 8x ~ 126x
    {
        //UINT32 s_factor;
        a_mul = 1;
        a_gain = 0x3f;
        //s_factor = a_gain*(1+a_mul)*1000/16;
        d_gain = ((128*factor) / (a_gain*(1+a_mul)*1000)) - 8;
        //s_factor = s_factor*(1+(d_gain/8));
        //debug_msg("AE: 3 f=%d, d_gain=%d, a_mul=%d, a_gain=%d, sf=%d\r\n", factor, d_gain, a_gain, a_mul, s_factor);
    }

    uiSensorGain = ((d_gain & 0x7F) << 8)| (a_mul << 6) | (a_gain & 0x3F);
    gMI1600ExpoSetting.uiGain = uiSensorGain;
    //debug_msg("AE: iso=%d, factor=%d, gain=%08x\r\n", uiIso, Gain_factor, gMI1600ExpoSetting.uiGain);
#endif
}

static void MI1600_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static UINT32 uiMI1600ExpoLine = 0;
static UINT32 uiMI1600Gain = 0;

static void MI1600_SetLine(UINT32 uiLine)
{
#if (SEN_EXPOTIME == ENABLE)
    if(uiLine < 1)
        uiLine = 1;
    else if(uiLine > 0xFFFFF)
        uiLine = 0xFFFFF;

    MI1600_WriteReg(0x08, ((uiLine >> 16) & 0x000F));
    MI1600_WriteReg(0x09, ((uiLine      ) & 0xFFFF));
    uiMI1600ExpoLine = uiLine;
    //MI1600_WriteReg(0x0C, 0);
    //debug_msg("SR: W ^Rline=%08x ^Ygain=%08x\r\n", uiMI1600ExpoLine, uiMI1600Gain);
#endif
}

static void MI1600_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void MI1600_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void MI1600_SetGain(UINT32 uiGain)
{
#if (SEN_GAIN == ENABLE)
    MI1600_WriteReg(0x35, uiGain);
    uiMI1600Gain = uiGain;
    //debug_msg("SR: W ^Yline=%08x ^Rgain=%08x\r\n", uiMI1600ExpoLine, uiMI1600Gain);
#endif
}

static void MI1600_Flip(UINT32 Direction)
{
    //flip enable bit
    #define H_flip  0x4000
    #define V_flip  0x8000
    #define Cap_Reg 0
    UINT32  CmdLength;
    UINT32  Cmd1 = 0,Cmd2 = 0,Cmd3 = 0,Cmd4 = 0,Cmd5 = 0;
    UINT32  DataInfo1 = 0,DataInfo2 = 0,DataInfo3 = 0,DataInfo4 = 0;
    UINT32 DataCap0=0;
    UINT32 Reg0;
    static BOOL bInit = TRUE;
    static UINT32 OrgData;

    MI1600_GetRegInfo(MI1600_VGA, &Cmd1, &CmdLength);
    MI1600_GetRegInfo(MI1600_1280X720, &Cmd2, &CmdLength);
    MI1600_GetRegInfo(MI1600_1440X1080, &Cmd3, &CmdLength);
    MI1600_GetRegInfo(MI1600_1440X540, &Cmd4, &CmdLength);
    MI1600_GetRegInfo(MI1600_STILL, &Cmd5, &CmdLength);

    MI1600_GetDataInfo(MI1600_VGA, &DataInfo1, NULL);
    MI1600_GetDataInfo(MI1600_1280X720, &DataInfo2, NULL);
    MI1600_GetDataInfo(MI1600_1440X1080, &DataInfo3, NULL);
    MI1600_GetDataInfo(MI1600_1440X540, &DataInfo4, NULL);
    MI1600_GetDataInfo(MI1600_STILL, &DataCap0, NULL);

    Reg0 = ((SENSOR_CMD*)(Cmd1+Cap_Reg*sizeof(SENSOR_CMD)))->uiData;

    if(bInit == TRUE)
    {
        Reg0 = ((SENSOR_CMD*)(Cmd1))->uiData;
        OrgData = Reg0;
        bInit = FALSE;
    }
    //Direction = sen_Flip_H_V;
    switch((SEN_FLIP)Direction)
    {
        case sen_Flip_H:
            //((PREVIEW_DATA_INFO*)DataInfo1)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData | H_flip;         //flip function in table idx 5
            break;
        case sen_Flip_V:
            //((PREVIEW_DATA_INFO*)DataCap0)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData | V_flip;         //flip function in table idx 5
            break;
        case sen_Flip_H_V:
            ((PREVIEW_DATA_INFO*)DataInfo1)->uiBayer = SIECFA_R;
            ((PREVIEW_DATA_INFO*)DataInfo2)->uiBayer = SIECFA_R;
            ((PREVIEW_DATA_INFO*)DataInfo3)->uiBayer = SIECFA_R;
            ((PREVIEW_DATA_INFO*)DataInfo4)->uiBayer = SIECFA_R;
            ((CAPTURE_DATA_INFO*)DataCap0)->uiBayer = SIECFA_R;
            //patch registor set
            Reg0 = OrgData | (H_flip|V_flip);    //flip function in table idx 5
            break;
        case sen_Flip_NULL:
            ((PREVIEW_DATA_INFO*)DataInfo1)->uiBayer = SIECFA_B;
            ((PREVIEW_DATA_INFO*)DataInfo2)->uiBayer = SIECFA_B;
            ((PREVIEW_DATA_INFO*)DataInfo3)->uiBayer = SIECFA_B;
            ((PREVIEW_DATA_INFO*)DataInfo4)->uiBayer = SIECFA_B;
            ((CAPTURE_DATA_INFO*)DataCap0)->uiBayer = SIECFA_B;
            //patch registor set
            Reg0 = OrgData;
            break;
        default:
            ((PREVIEW_DATA_INFO*)DataInfo1)->uiBayer = SIECFA_B;
            ((PREVIEW_DATA_INFO*)DataInfo2)->uiBayer = SIECFA_B;
            ((PREVIEW_DATA_INFO*)DataInfo3)->uiBayer = SIECFA_B;
            ((PREVIEW_DATA_INFO*)DataInfo4)->uiBayer = SIECFA_B;
            ((CAPTURE_DATA_INFO*)DataCap0)->uiBayer = SIECFA_B;
            ((PREVIEW_DATA_INFO*)DataCap0)->uiBayer = SIECFA_B;
            Reg0 = OrgData;
            debug_err(("^R sensor flip direction error\r\n"));
            break;
    }
    ((SENSOR_CMD*)(Cmd1+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;
    ((SENSOR_CMD*)(Cmd2+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;
    ((SENSOR_CMD*)(Cmd3+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;
    ((SENSOR_CMD*)(Cmd4+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;
    ((SENSOR_CMD*)(Cmd5+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;

    //do not update sensor status,should undate in next sensor chg mode
    /**
    */

}
#if 0
- utility
#endif
//@@ EOF
