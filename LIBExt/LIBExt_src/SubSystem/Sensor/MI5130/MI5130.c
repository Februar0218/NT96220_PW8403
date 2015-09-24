/**
    @file       MI5130.c

    @brief      MI5130

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
#include "MI5130.h"

#define SEN_READ_ID 0x21
#define SEN_WRITE_ID 0x20

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif
//utility
static UINT CfaPat(UINT OrgPat, SEN_FLIP FlipType);
// function prototype
static void MI5130_SifInit(UINT32 *pPort);
static void MI5130_Init(void);
static void MI5130_PwrOn(UINT32 uiStage);
static void MI5130_PwrOff(UINT32 uiStage);
static void MI5130_Sleep(UINT32 uiStage);
static void MI5130_WakeUp(UINT32 uiStage);
static void MI5130_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI5130_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI5130_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI5130_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI5130_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI5130_ChgMode(UINT32 uiMode);
static void MI5130_GetExpo(EXPOSURE_SETTING *Expo);
static void MI5130_CalLine(UINT32 uiTime);
static void MI5130_CalGain(UINT32 uiIso);
static void MI5130_SetFrame(UINT32 uiFrame);
static void MI5130_SetLine(UINT32 uiLine);
static void MI5130_SetPixel(UINT32 uiStep);
static void MI5130_SetVDLength(UINT32 uiVDLength);
static void MI5130_SetGain(UINT32 uiGain);
//#NT#2011/10/26#Ethan Lau -begin
static void MI5130_Flip(UINT32 Direction);
//#NT#2011/10/26#Ethan Lau -end
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5130_checkExposureLongTime(void);
static void MI5130_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 MI5130_GetCalMshutterDelayTime(void);
static void MI5130_TriggerGlobalReset(void);
static void MI5130_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end


//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pMI5130Drv =
{
    // TG control
    MI5130_SifInit,
    MI5130_Init,
    MI5130_PwrOn,
    MI5130_PwrOff,
    MI5130_Sleep,
    MI5130_WakeUp,
    MI5130_WriteReg,
    MI5130_ReadReg,
    MI5130_GetStatus,
    MI5130_GetCurStatus,
    MI5130_ChgMode,
    //feature,
    MI5130_Flip,
    // AE
    MI5130_GetExpo,
    MI5130_CalLine,
    MI5130_CalGain,
    MI5130_SetFrame,
    MI5130_SetLine,
    MI5130_SetPixel,
    MI5130_SetVDLength,
    MI5130_SetGain,
    MI5130_checkExposureLongTime,
    MI5130_SetCalMshutterDelayTime,
    MI5130_GetCalMshutterDelayTime,
    MI5130_TriggerGlobalReset,
    MI5130_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gMI5130Status;
static EXPOSURE_SETTING gMI5130ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI5130Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI5130Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI5130_prv_mode,  // pPrvMode
        MI5130_still_mode   // pStillMode
    };
    //g_pMI5130Info.pPrvMode = &MI5130_prv_mode[CurrentSensorMode];
    return &g_pMI5130Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI5130_SifInit(UINT32 *pPort)
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

static void MI5130_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("MI5130_init...\r\n");

    MI5130_GetRegInfo(MI5130_INIT, &CmdAddr, &CmdLength);
    Sensor_WriteReg(0x0103, 0x01);// SOFTWARE_RESET
    TimerDelayMs(2); // 2400 * EXTCLK

    MI5130_WriteReg(0x301A, 0x10C8);
    TimerDelayUs(300);
    MI5130_WriteReg(0x3064, 0x0805);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI5130_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        MI5130_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    Sensor_WriteReg(0x301A, 0x10CC);// RESET_REGISTER

    // Initial local status
    memset(&gMI5130Status, 0, sizeof(SENSOR_STATUS));
    memset(&gMI5130ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void MI5130_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5130_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5130_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5130_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5130_WriteReg(UINT32 uiAddr, UINT32 uiValue)
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

static void MI5130_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
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

static void MI5130_GetCurStatus(PSENSOR_STATUS pStatus)
{
    *pStatus = gMI5130Status;
}

static void MI5130_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI5130_SetStatus(uiMode,pStatus);
}

static void MI5130_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    MI5130_GetDataInfo(uiMode,&Data,NULL);
    MI5130_GetSigInfo(uiMode,&Sync,NULL);
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

static void MI5130_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    MI5130_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        MI5130_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        MI5130_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    MI5130_SetStatus(uiMode,&gMI5130Status);
}

/**
 * AE driver API
 */

static void MI5130_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI5130ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5130_checkExposureLongTime(void)
{
    return FALSE;
}

static void MI5130_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 MI5130_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void MI5130_TriggerGlobalReset(void)
{

}

static void MI5130_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void MI5130_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;
    UINT32 Expo_CompensateRatio;

    temp_line = uiTime * gMI5130Status.uiPclk / gMI5130Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0002)
       temp_line = 0x0002;

    gMI5130ExpoSetting.uiFrame = 0;
    gMI5130ExpoSetting.uiLine = temp_line;
    gMI5130ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gMI5130Status.pSyncInfo->HD_total / gMI5130Status.uiPclk;
    else
        linetime = uiTime;

    if(linetime<uiTime)
        Expo_CompensateRatio = uiTime*100/linetime;
    else
        Expo_CompensateRatio = 100;

    gMI5130ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
}

static void MI5130_CalGain(UINT32 uiIso)
{
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
    MI5130_GetISOInfo(&ISOBase, &AGCBase);

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

    //debug_err(("Gain_factor=%d\r\n",Gain_factor));

    //convert desire gain to register setting
    if(Gain_factor < 2000)//Agc 0x~4x
    {
        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x1080;
    }
    else if(Gain_factor < 4000)//Agc 4x~8x
    {
        Gain_factor/=2;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        //Gain_factor |= 0x1080;
        Gain_factor |= 0x2080;
    }
    else if(Gain_factor < 8000)//Agc 8x~16x
    {
        Gain_factor/=4;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        //Gain_factor |= 0x1180;
        Gain_factor |= 0x4080;
    }
    else if(Gain_factor < 12000) //Agc 16x~24x
    {
        Gain_factor/=6;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x6080;
    }
    else if(Gain_factor < 24000) //Agc 24x~48x
    {
        Gain_factor/=12;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x6180;
    }
    else
    {
        Gain_factor= 0x60ff;//0x12FF;
    }

    gMI5130ExpoSetting.uiGain = Gain_factor;
}

static void MI5130_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void MI5130_SetLine(UINT32 uiLine)
{
    if(uiLine > (gMI5130Status.pSyncInfo->VD_total - 2))
    {
        MI5130_WriteReg(0x3012, uiLine); //coarse integration time
        MI5130_WriteReg(0x0340, (uiLine + 2)); //coarse integration time
    }
    else
    {
        MI5130_WriteReg(0x3012, uiLine); //coarse integration time
        MI5130_WriteReg(0x0340, (gMI5130Status.pSyncInfo->VD_total + 2)); //coarse integration time
    }
}

static void MI5130_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void MI5130_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void MI5130_SetGain(UINT32 uiGain)
{
    MI5130_WriteReg(0x305E, uiGain);
}

static void MI5130_Flip(UINT32 Direction)
{
    //flip enable bit
    #define H_flip  0x4000
    #define V_flip  0x8000
    #define Prv_Reg 5
    #define Cap_Reg 6
    UINT32  CmdLength;
    UINT32  Cmd = 0,Cmd2=0,Cmd3=0;
    UINT32 DataPrv1 = 0,DataPrv2 = 0,DataCap = 0;
    UINT32 Reg0,Reg1,Reg2;
    static BOOL bInit = TRUE;
    static UINT32 OrgData[3],OrgCFA[2];

    MI5130_GetRegInfo(MI5130_VGA, &Cmd, &CmdLength);
    Reg0 = ((SENSOR_CMD*)(Cmd+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI5130_GetRegInfo(MI5130_1280X960, &Cmd2, &CmdLength);
    Reg1 = ((SENSOR_CMD*)(Cmd2+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI5130_GetRegInfo(MI5130_STILL, &Cmd3, &CmdLength);
    Reg2 = ((SENSOR_CMD*)(Cmd3+Cap_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip

    MI5130_GetDataInfo(MI5130_VGA, &DataPrv1, NULL);
    MI5130_GetDataInfo(MI5130_1280X960, &DataPrv2, NULL);
    MI5130_GetDataInfo(MI5130_STILL, &DataCap, NULL);
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
    ((SENSOR_CMD*)(Cmd+Prv_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;
    ((SENSOR_CMD*)(Cmd2+Prv_Reg*sizeof(SENSOR_CMD)))->uiData = Reg1;
    ((SENSOR_CMD*)(Cmd3+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg2;
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
