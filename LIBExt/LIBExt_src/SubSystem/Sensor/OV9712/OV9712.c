/**
    @file       OV9712.c

    @brief      OV9712

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
#include "OV9712.h"

#define SEN_READ_ID 0x61
#define SEN_WRITE_ID 0x60

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif
//utility
static UINT CfaPat(UINT OrgPat, SEN_FLIP FlipType);
// function prototype
static void OV9712_SifInit(UINT32 *pPort);
static void OV9712_Init(void);
static void OV9712_PwrOn(UINT32 uiStage);
static void OV9712_PwrOff(UINT32 uiStage);
static void OV9712_Sleep(UINT32 uiStage);
static void OV9712_WakeUp(UINT32 uiStage);
static void OV9712_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void OV9712_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void OV9712_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void OV9712_GetCurStatus(PSENSOR_STATUS pStatus);
static void OV9712_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void OV9712_ChgMode(UINT32 uiMode);
static void OV9712_GetExpo(EXPOSURE_SETTING *Expo);
static void OV9712_CalLine(UINT32 uiTime);
static void OV9712_CalGain(UINT32 uiIso);
static void OV9712_SetFrame(UINT32 uiFrame);
static void OV9712_SetLine(UINT32 uiLine);
static void OV9712_SetPixel(UINT32 uiStep);
static void OV9712_SetVDLength(UINT32 uiVDLength);
static void OV9712_SetGain(UINT32 uiGain);
//#NT#2011/10/26#Ethan Lau -begin
static void OV9712_Flip(UINT32 Direction);
//#NT#2011/10/26#Ethan Lau -end
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL OV9712_checkExposureLongTime(void);
static void OV9712_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 OV9712_GetCalMshutterDelayTime(void);
static void OV9712_TriggerGlobalReset(void);
static void OV9712_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

static SENSOR_TAB g_pOV9712Drv =
{
    // TG control
    OV9712_SifInit,
    OV9712_Init,
    OV9712_PwrOn,
    OV9712_PwrOff,
    OV9712_Sleep,
    OV9712_WakeUp,
    OV9712_WriteReg,
    OV9712_ReadReg,
    OV9712_GetStatus,
    OV9712_GetCurStatus,
    OV9712_ChgMode,
    //feature,
    OV9712_Flip,
    // AE
    OV9712_GetExpo,
    OV9712_CalLine,
    OV9712_CalGain,
    OV9712_SetFrame,
    OV9712_SetLine,
    OV9712_SetPixel,
    OV9712_SetVDLength,
    OV9712_SetGain,
    OV9712_checkExposureLongTime,
    OV9712_SetCalMshutterDelayTime,
    OV9712_GetCalMshutterDelayTime,
    OV9712_TriggerGlobalReset,
    OV9712_ClearGlobalReset
};

static SENSOR_STATUS gOV9712Status;
static EXPOSURE_SETTING gOV9712ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pOV9712Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pOV9712Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        OV9712_prv_mode,  // pPrvMode
        OV9712_still_mode   // pStillMode
    };
    //g_pOV9712Info.pPrvMode = &OV9712_prv_mode[CurrentSensorMode];
    return &g_pOV9712Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void OV9712_SifInit(UINT32 *pPort)
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

static void OV9712_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("OV9712_init...\r\n");

    OV9712_GetRegInfo(OV9712_INIT, &CmdAddr, &CmdLength);
    //soft reset
    OV9712_WriteReg(0x12, 0x80);

    TimerDelayMs(10);

    for (idx = 0;idx < CmdLength;idx++)
    {
        OV9712_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        OV9712_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
        TimerDelayMs(5);
    }

    // Initial local status
    memset(&gOV9712Status, 0, sizeof(SENSOR_STATUS));
    memset(&gOV9712ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void OV9712_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV9712_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV9712_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV9712_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void OV9712_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = ((uiAddr)&(0x000000ff));
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
        erReturn=i2c_masterTransmit(ulData, 0, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }
    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

static void OV9712_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulData, ulData1;

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

static void OV9712_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gOV9712Status;
}

static void OV9712_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     OV9712_SetStatus(uiMode,pStatus);
}

static void OV9712_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    OV9712_GetDataInfo(uiMode,&Data,NULL);
    OV9712_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
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
        case SENSOR_MODE_1440X1080:
            pStatus->uiSensorMode = SENSOR_MODE_1440X1080;
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
        case SENSOR_MODE_1920X1080:
            pStatus->uiSensorMode = SENSOR_MODE_1920X1080;
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

static void OV9712_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    OV9712_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        OV9712_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        OV9712_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    OV9712_SetStatus(uiMode,&gOV9712Status);
}

/**
 * AE driver API
 */

static void OV9712_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gOV9712ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL OV9712_checkExposureLongTime(void)
{
    return FALSE;
}

static void OV9712_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 OV9712_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void OV9712_TriggerGlobalReset(void)
{

}

static void OV9712_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void OV9712_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;

    temp_line = uiTime * gOV9712Status.uiPclk / gOV9712Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0001)
       temp_line = 0x0001;

    gOV9712ExpoSetting.uiFrame = 0;
    gOV9712ExpoSetting.uiLine = temp_line;
    gOV9712ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gOV9712Status.pSyncInfo->HD_total / gOV9712Status.uiPclk;
    else
        linetime = uiTime;

    if (linetime < uiTime)
        gOV9712ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    else
        gOV9712ExpoSetting.uiExpoRatio = 100;
}

static void OV9712_CalGain(UINT32 uiIso)
{
    #define GAIN_STEP       625
    #define AGC_GAP         16
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT16 shf;
    UINT32 AGCBase,ISOBase;
    OV9712_GetISOInfo(&ISOBase, &AGCBase);

    if(uiIso > 800)
        uiIso = 800;

    Calcul_AGC_Ratio = (UINT32)(((UINT32)100*(UINT32)uiIso)/(UINT32)ISOBase);

    shf = 0;

    while(Calcul_AGC_Ratio/((1<<(shf+1))*100))
        shf++;

    Gain_factor = ((Calcul_AGC_Ratio-((1<<shf)*100))*100)/(GAIN_STEP<<shf) + AGCBase + AGC_GAP*shf;

    if (Gain_factor > 0xff)
    {
        Gain_factor = 0xff;
    }

    gOV9712ExpoSetting.uiGain= Gain_factor;
}

static void OV9712_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void OV9712_SetLine(UINT32 uiLine)
{
    if (uiLine > 0xffff)
    {
        uiLine = 0xffff;
    }

    OV9712_WriteReg(0x16, ((uiLine & 0xff00) >> 8));
    OV9712_WriteReg(0x10, ((uiLine & 0x00ff) >> 0));
}

static void OV9712_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void OV9712_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void OV9712_SetGain(UINT32 uiGain)
{
    UINT32 Line;
    UINT16 Temp;
    UINT16 AGC = 0;

    Temp = (uiGain >> 4);

    if(Temp > 0)
        AGC = (((1 << Temp) - 1) << 4);

    Temp = (uiGain & 0x0F);
    AGC = AGC + Temp;
    //debug_msg(">>OV9712_SetGain=%d\r\n", AGC);
    if(AGC > 0x7F)
    {
        debug_err(("AE_Param.ISO_Gain = %x\r\n",uiGain));
        debug_err(("register overflow\r\n"));
        AGC = 0x7F;
    }
    OV9712_WriteReg(0x0, (unsigned char)AGC); //analog gain

    //Dummy line
    if (gOV9712ExpoSetting.uiLine > gOV9712Status.pSyncInfo->VD_total)
    {
        Line = gOV9712ExpoSetting.uiLine - gOV9712Status.pSyncInfo->VD_total;
        if (Line > 0xffff)
        {
            Line = 0xffff;
        }
        OV9712_WriteReg(0x2E, ((Line & 0xff00) >> 8));
        OV9712_WriteReg(0x2D, ((Line & 0x00ff) >> 0));
    }
    else
    {
        Line= 0;
        OV9712_WriteReg(0x2E, ((Line & 0xff00) >> 8));
        OV9712_WriteReg(0x2D, ((Line & 0x00ff) >> 0));
    }

}
static void OV9712_Flip(UINT32 Direction)
{
    //flip enable bit
    #define H_flip  0x0080
    #define V_flip  0x0040
    #define Cap_Reg 0
    UINT32  CmdLength;
    UINT32  Cmd = 0,Cmd1 = 0;
    UINT32 DataCap=0,DataCap1=0;
    UINT32 Reg0;
    static BOOL bInit = TRUE;
    static UINT32 OrgData,OrgCFA;

    OV9712_GetRegInfo(OV9712_1920X1080, &Cmd1, &CmdLength);

    OV9712_GetRegInfo(OV9712_STILL, &Cmd, &CmdLength);
    Reg0 = ((SENSOR_CMD*)(Cmd+Cap_Reg*sizeof(SENSOR_CMD)))->uiData;

    OV9712_GetDataInfo(OV9712_1920X1080, &DataCap1, NULL);
    OV9712_GetDataInfo(OV9712_STILL, &DataCap, NULL);
    if(bInit == TRUE)
    {
        OrgData = Reg0;
        OrgCFA  = ((CAPTURE_DATA_INFO*)DataCap)->uiBayer;
        bInit = FALSE;
    }
    switch((SEN_FLIP)Direction)
    {
        case sen_Flip_H:
            //patch bayer pattern
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
            ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData | H_flip;         //flip function in table idx 5
            break;
        case sen_Flip_V:
            //patch bayer pattern
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
            ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData | V_flip;         //flip function in table idx 5
            break;
        case sen_Flip_H_V:
            //patch bayer pattern
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA,sen_Flip_V);
            ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = CfaPat(OrgCFA,sen_Flip_V);
            //patch registor set
            Reg0 = OrgData | (H_flip|V_flip);    //flip function in table idx 5
            break;
        case sen_Flip_NULL:
            //patch bayer pattern
            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = OrgCFA;
            ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = OrgCFA;
            //patch registor set
            Reg0 = OrgData;
            break;
        default:
            debug_err(("^R sensor flip direction error\r\n"));
            break;
    }
    ((SENSOR_CMD*)(Cmd+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;
    ((SENSOR_CMD*)(Cmd1+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;

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
