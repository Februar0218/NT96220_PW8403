/**
    @file       MI5131E.c

    @brief      MI5131E

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
#include "MI5131E.h"

#define SEN_READ_ID 0x21
#define SEN_WRITE_ID 0x20

#define SENDRV_DEBUG    DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif
//utility
static UINT CfaPat(UINT OrgPat, SEN_FLIP FlipType);
// function prototype
static void MI5131E_SifInit(UINT32 *pPort);
static void MI5131E_Init(void);
static void MI5131E_PwrOn(UINT32 uiStage);
static void MI5131E_PwrOff(UINT32 uiStage);
static void MI5131E_Sleep(UINT32 uiStage);
static void MI5131E_WakeUp(UINT32 uiStage);
static void MI5131E_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI5131E_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI5131E_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI5131E_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI5131E_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI5131E_ChgMode(UINT32 uiMode);
static void MI5131E_GetExpo(EXPOSURE_SETTING *Expo);
static void MI5131E_CalLine(UINT32 uiTime);
static void MI5131E_CalGain(UINT32 uiIso);
static void MI5131E_SetFrame(UINT32 uiFrame);
static void MI5131E_SetLine(UINT32 uiLine);
static void MI5131E_SetPixel(UINT32 uiStep);
static void MI5131E_SetVDLength(UINT32 uiVDLength);
static void MI5131E_SetGain(UINT32 uiGain);
//#NT#2011/10/26#Ethan Lau -begin
static void MI5131E_Flip(UINT32 Direction);
//#NT#2011/10/26#Ethan Lau -end
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5131E_checkExposureLongTime(void);
static void MI5131E_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 MI5131E_GetCalMshutterDelayTime(void);
static void MI5131E_TriggerGlobalReset(void);
static void MI5131E_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pMI5131EDrv =
{
    // TG control
    MI5131E_SifInit,
    MI5131E_Init,
    MI5131E_PwrOn,
    MI5131E_PwrOff,
    MI5131E_Sleep,
    MI5131E_WakeUp,
    MI5131E_WriteReg,
    MI5131E_ReadReg,
    MI5131E_GetStatus,
    MI5131E_GetCurStatus,
    MI5131E_ChgMode,
    //feature,
    MI5131E_Flip,
    // AE
    MI5131E_GetExpo,
    MI5131E_CalLine,
    MI5131E_CalGain,
    MI5131E_SetFrame,
    MI5131E_SetLine,
    MI5131E_SetPixel,
    MI5131E_SetVDLength,
    MI5131E_SetGain,
    MI5131E_checkExposureLongTime,
    MI5131E_SetCalMshutterDelayTime,
    MI5131E_GetCalMshutterDelayTime,
    MI5131E_TriggerGlobalReset,
    MI5131E_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gMI5131EStatus;
static EXPOSURE_SETTING gMI5131EExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI5131EDrv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI5131EInfo =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI5131E_prv_mode,  // pPrvMode
        MI5131E_still_mode   // pStillMode
    };
    //g_pMI5131EInfo.pPrvMode = &MI5131E_prv_mode[CurrentSensorMode];
    return &g_pMI5131EInfo;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI5131E_SifInit(UINT32 *pPort)
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

static void MI5131E_Init(void)
{
    UINT32 idx, CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("^G MI5131E_init...\r\n");
    MI5131E_GetRegInfo(MI5131E_INIT, &CmdAddr, &CmdLength);
    MI5131E_WriteReg(0x301A, 0x10C8);
    TimerDelayUs(300);
    MI5131E_WriteReg(0x3064, 0x0805);

    for (idx = 0;idx < CmdLength;idx++)
    {
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        if ((((SENSOR_CMD*)CmdAddr)->uiAddr == 0x100) && (((SENSOR_CMD*)CmdAddr)->uiData == 0x1))
        {
            TimerDelayMs(1);
        }
        MI5131E_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    Sensor_WriteReg(0x301A, 0x10CC);
    TimerDelayUs(30);
    Sensor_WriteReg(0x30C0, 0x1020);
    Sensor_WriteReg(0x400, 0x2);
    Sensor_WriteReg(0x306E, 0x48B0);

    // Initial local status
    memset(&gMI5131EStatus, 0, sizeof(SENSOR_STATUS));
    memset(&gMI5131EExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void MI5131E_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5131E_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5131E_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5131E_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI5131E_WriteReg(UINT32 uiAddr, UINT32 uiValue)
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
            debug_err(("Error transmit data (write addr = %x)!!\r\n",ulWriteAddr));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg1 = %x) !!\r\n",ulReg1));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg2 = %x) !!\r\n",ulReg2));
            break;
        }

        erReturn=i2c_masterTransmit(ulData1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1 = %x) !!\r\n",ulData1));
            break;
        }

        erReturn=i2c_masterTransmit(ulData2, 0, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data2 = %x) !!\r\n",ulData2));
            break;
        }

    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

static void MI5131E_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
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

static void MI5131E_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gMI5131EStatus;
}

static void MI5131E_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI5131E_SetStatus(uiMode,pStatus);
}

static void MI5131E_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    MI5131E_GetDataInfo(uiMode,&Data,NULL);
    MI5131E_GetSigInfo(uiMode,&Sync,NULL);
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

static void MI5131E_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 CmdLength;
    UINT32 CmdAddr = 0;

    MI5131E_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        //#NT#2011/11/14#Ethan Lau -begin
        //#NT#add for mi5131e, addr == 0x0x meas to delay (uiData) ms.
        if(((SENSOR_CMD*)CmdAddr)->uiAddr == 0x0000)
            TimerDelayMs(((SENSOR_CMD*)CmdAddr)->uiData);
        else
            MI5131E_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //#NT#2011/11/14#Ethan Lau -end
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs(300);

    MI5131E_SetStatus(uiMode,&gMI5131EStatus);
}

/**
 * AE driver API
 */

static void MI5131E_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI5131EExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI5131E_checkExposureLongTime(void)
{
    return FALSE;
}

static void MI5131E_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 MI5131E_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void MI5131E_TriggerGlobalReset(void)
{

}

static void MI5131E_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void MI5131E_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;
    UINT32 Expo_CompensateRatio;

    temp_line = uiTime * gMI5131EStatus.uiPclk / gMI5131EStatus.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
       temp_line = 0xFFFF;

    if(temp_line < 0x0008)
       temp_line = 0x0008;

    gMI5131EExpoSetting.uiFrame = 0;
    gMI5131EExpoSetting.uiLine = temp_line;
    gMI5131EExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
        linetime = temp_line * gMI5131EStatus.pSyncInfo->HD_total / gMI5131EStatus.uiPclk;
    else
        linetime = uiTime;

    if(linetime<uiTime)
        Expo_CompensateRatio = uiTime*100/linetime;
    else
        Expo_CompensateRatio = 100;

    gMI5131EExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
}

static void MI5131E_CalGain(UINT32 uiIso)
{
    BOOL bit8=FALSE,bit7=FALSE,bit6=FALSE;
    UINT32 digital_gain = 1;
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
    MI5131E_GetISOInfo(&ISOBase, &AGCBase);

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
#if 0
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
#else
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
#endif
    gMI5131EExpoSetting.uiGain = Gain_factor;
}

static void MI5131E_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void MI5131E_SetLine(UINT32 uiLine)
{
    if(uiLine > (gMI5131EStatus.pSyncInfo->VD_total - 2))
    {
        MI5131E_WriteReg(0x3012, uiLine); //coarse integration time
        MI5131E_WriteReg(0x0340, (uiLine + 2)); //coarse integration time
    }
    else
    {
        MI5131E_WriteReg(0x3012, uiLine); //coarse integration time
        MI5131E_WriteReg(0x0340, gMI5131EStatus.pSyncInfo->VD_total); //coarse integration time
    }
}

static void MI5131E_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void MI5131E_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void MI5131E_SetGain(UINT32 uiGain)
{
    MI5131E_WriteReg(0x305E, uiGain);
}

static void MI5131E_Flip(UINT32 Direction)
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

    MI5131E_GetRegInfo(MI5131E_VGA, &Cmd, &CmdLength);
    Reg0 = ((SENSOR_CMD*)(Cmd+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI5131E_GetRegInfo(MI5131E_1280X960, &Cmd2, &CmdLength);
    Reg1 = ((SENSOR_CMD*)(Cmd2+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI5131E_GetRegInfo(MI5131E_STILL, &Cmd3, &CmdLength);
    Reg2 = ((SENSOR_CMD*)(Cmd3+Cap_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip

    MI5131E_GetDataInfo(MI5131E_VGA, &DataPrv1, NULL);
    MI5131E_GetDataInfo(MI5131E_1280X960, &DataPrv2, NULL);
    MI5131E_GetDataInfo(MI5131E_STILL, &DataCap, NULL);
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
