/**
    @file       NT99140.c

    @brief      NT99140

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
#include "NT99140.h"

#define SEN_READ_ID 0x55
#define SEN_WRITE_ID 0x54

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif
//utility
//static UINT CfaPat(UINT OrgPat, SEN_FLIP FlipType);
// function prototype
static void NT99140_SifInit(UINT32 *pPort);
static void NT99140_Init(void);
static void NT99140_PwrOn(UINT32 uiStage);
static void NT99140_PwrOff(UINT32 uiStage);
static void NT99140_Sleep(UINT32 uiStage);
static void NT99140_WakeUp(UINT32 uiStage);
static void NT99140_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void NT99140_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void NT99140_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void NT99140_GetCurStatus(PSENSOR_STATUS pStatus);
static void NT99140_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void NT99140_ChgMode(UINT32 uiMode);
static void NT99140_GetExpo(EXPOSURE_SETTING *Expo);
static void NT99140_CalLine(UINT32 uiTime);
static void NT99140_CalGain(UINT32 uiIso);
static void NT99140_SetFrame(UINT32 uiFrame);
static void NT99140_SetLine(UINT32 uiLine);
static void NT99140_SetPixel(UINT32 uiStep);
static void NT99140_SetVDLength(UINT32 uiVDLength);
static void NT99140_SetGain(UINT32 uiGain);
//#NT#2011/10/26#Ethan Lau -begin
static void NT99140_Flip(UINT32 Direction);
//#NT#2011/10/26#Ethan Lau -end
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL NT99140_checkExposureLongTime(void);
static void NT99140_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 NT99140_GetCalMshutterDelayTime(void);
static void NT99140_TriggerGlobalReset(void);
static void NT99140_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pNT99140Drv =
{
    // TG control
    NT99140_SifInit,
    NT99140_Init,
    NT99140_PwrOn,
    NT99140_PwrOff,
    NT99140_Sleep,
    NT99140_WakeUp,
    NT99140_WriteReg,
    NT99140_ReadReg,
    NT99140_GetStatus,
    NT99140_GetCurStatus,
    NT99140_ChgMode,
    //feature,
    NT99140_Flip,
    // AE
    NT99140_GetExpo,
    NT99140_CalLine,
    NT99140_CalGain,
    NT99140_SetFrame,
    NT99140_SetLine,
    NT99140_SetPixel,
    NT99140_SetVDLength,
    NT99140_SetGain,
    NT99140_checkExposureLongTime,
    NT99140_SetCalMshutterDelayTime,
    NT99140_GetCalMshutterDelayTime,
    NT99140_TriggerGlobalReset,
    NT99140_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gNT99140Status;
static EXPOSURE_SETTING gNT99140ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pNT99140Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pNT99140Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        NT99140_prv_mode,  // pPrvMode
        NT99140_still_mode   // pStillMode
    };
    //g_pNT99140Info.pPrvMode = &NT99140_prv_mode[CurrentSensorMode];
    return &g_pNT99140Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void NT99140_SifInit(UINT32 *pPort)
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

static void NT99140_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("NT99140_init...\r\n");

    NT99140_GetRegInfo(NT99140_INIT, &CmdAddr, &CmdLength);
    //soft reset
    //NT99140_WriteReg(0x12, 0x80);

    //TimerDelayMs(10);

    for (idx = 0;idx < CmdLength;idx++)
    {
        NT99140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        NT99140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
        TimerDelayMs(5);
    }

    // Initial local status
    memset(&gNT99140Status, 0, sizeof(SENSOR_STATUS));
    memset(&gNT99140ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void NT99140_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void NT99140_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void NT99140_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void NT99140_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void NT99140_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulReg2, ulData;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (uiAddr>>8);
    ulReg2      = ((uiAddr)&(0x000000ff));
    ulData      = ((uiValue)&(0x000000ff));
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

        erReturn=i2c_masterTransmit(ulData, 0, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }
    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

static void NT99140_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulReg2, ulData, ulData1;

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

static void NT99140_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gNT99140Status;
}

static void NT99140_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     NT99140_SetStatus(uiMode,pStatus);
}

static void NT99140_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    NT99140_GetDataInfo(uiMode,&Data,NULL);
    NT99140_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
        case SENSOR_MODE_1280X720:
        case SENSOR_MODE_1280X960:
        case SENSOR_MODE_1440X1080:
        case SENSOR_MODE_1920X1080:
        case SENSOR_MODE_FULL:
            pStatus->uiSensorMode = SENSOR_MODE_FULL;
            pStatus->uiReadoutMode = 0; //SENSOR_OUT_PROGRESSIVE;
            pStatus->uiRatio = SENSOR_RATIO_16_9;
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

static void NT99140_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    NT99140_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        NT99140_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        NT99140_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    NT99140_SetStatus(uiMode,&gNT99140Status);
}

/**
 * AE driver API
 */

static void NT99140_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gNT99140ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL NT99140_checkExposureLongTime(void)
{
    return FALSE;
}

static void NT99140_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 NT99140_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void NT99140_TriggerGlobalReset(void)
{

}

static void NT99140_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void NT99140_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;

    temp_line = uiTime * gNT99140Status.uiPclk / gNT99140Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0001)
       temp_line = 0x0001;

    gNT99140ExpoSetting.uiFrame = 0;
    gNT99140ExpoSetting.uiLine = temp_line;
    gNT99140ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gNT99140Status.pSyncInfo->HD_total / gNT99140Status.uiPclk;
    else
        linetime = uiTime;

    if (linetime < uiTime)
        gNT99140ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    else
        gNT99140ExpoSetting.uiExpoRatio = 100;
}

static void NT99140_CalGain(UINT32 uiIso)
{
    #define GAIN_STEP       625
    #define AGC_GAP         16
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT16 shf;
    UINT32 AGCBase,ISOBase;

    NT99140_GetISOInfo(&ISOBase, &AGCBase);

    Calcul_AGC_Ratio = ((100*uiIso)/ISOBase);

    shf = 0;

    while(Calcul_AGC_Ratio/((1<<(shf+1))*100))
        shf++;

    Gain_factor = ((Calcul_AGC_Ratio-((1<<shf)*100))*100)/(GAIN_STEP<<shf) + AGCBase + AGC_GAP*shf;

    if (Gain_factor > 0xff)
    {
        Gain_factor = 0xff;
    }
    gNT99140ExpoSetting.uiGain= Gain_factor;
}

static void NT99140_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void NT99140_SetLine(UINT32 uiLine)
{
    if (uiLine > 0xffff)
    {
        uiLine = 0xffff;
    }

    NT99140_WriteReg(0x3012, ((uiLine & 0xff00) >> 8));
    NT99140_WriteReg(0x3013, ((uiLine & 0x00ff) >> 0));
    NT99140_WriteReg(0x3060, 0x02);
}

static void NT99140_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void NT99140_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void NT99140_SetGain(UINT32 uiGain)
{
    UINT16 Temp;
    UINT16 AGC = 0;

    Temp = (uiGain >> 4);

    if(Temp > 0)
        AGC = (((1 << Temp) - 1) << 4);

    Temp = (uiGain & 0x0F);
    AGC = AGC + Temp;
    //debug_msg(">>NT99140_SetGain=%d\r\n", AGC);
    if(AGC > 0x7F)
    {
        debug_err(("AE_Param.ISO_Gain = %x\r\n",uiGain));
        debug_err(("register overflow\r\n"));
        AGC = 0x7F;
    }
    NT99140_WriteReg(0x301d, (unsigned char)AGC); //analog gain
    NT99140_WriteReg(0x3060, 0x02);
}
static void NT99140_Flip(UINT32 Direction)
{
    //flip enable bit
    #define H_flip  0x02
    #define V_flip  0x01
    UINT32  CmdLength;
    UINT32  Cmd = 0;
    UINT32 Reg0,i;
    static BOOL bInit = TRUE;
    static UINT32 OrgData;

    NT99140_GetRegInfo(NT99140_INIT, &Cmd, &CmdLength);

    for(i=0; i< CmdLength; i++)
    {
        if((((SENSOR_CMD*)(Cmd+i*sizeof(SENSOR_CMD)))->uiAddr) == 0x3022)//Flip addr
            break;
    }

    if(bInit == TRUE)
    {
        Reg0 = ((SENSOR_CMD*)(Cmd+i*sizeof(SENSOR_CMD)))->uiData;
        OrgData = Reg0;
        bInit = FALSE;
    }
    switch((SEN_FLIP)Direction)
    {
        case sen_Flip_H:
            //patch registor set
            Reg0 = OrgData | H_flip;         //flip function in table idx 5
            break;
        case sen_Flip_V:
            //patch registor set
            Reg0 = OrgData | V_flip;         //flip function in table idx 5
            break;
        case sen_Flip_H_V:
            //patch registor set
            Reg0 = OrgData | (H_flip|V_flip);    //flip function in table idx 5
            break;
        case sen_Flip_NULL:
            //patch registor set
            Reg0 = OrgData;
            break;
        default:
            Reg0 = OrgData;
            debug_err(("^R sensor flip direction error\r\n"));
            break;
    }
    ((SENSOR_CMD*)(Cmd+i*sizeof(SENSOR_CMD)))->uiData = Reg0;
    //do not update sensor status,should undate in next sensor chg mode
    /**
    */
}
#if 0
- utility
#endif
/*
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
*/
//@@ EOF
