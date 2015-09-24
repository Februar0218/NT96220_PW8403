/*
    @Sensor     Micron MI3130
    @file       MI3130.c
    @ingroup        Sensor\MI3130
    @note       Nothing.

    Porting by Randy Ho  2012/03/08

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
//----------------------------------------------------------------------------------------------------
#include <string.h>
#include "i2c.h"
#include "Type.h"
#include "Debug.h"
#include "Top.h"
#include "utility.h"
#include "Sensor.h"
#include "MI3130.h"

#define SEN_READ_ID         0x21  // mi3130 read address
#define SEN_WRITE_ID        0x20  // mi3130 write address

#define SENDRV_DEBUG    DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif
//utility
static UINT CfaPat(UINT OrgPat, SEN_FLIP FlipType);
// function prototype
static void MI3130_SifInit(UINT32 *pPort);
static void MI3130_Init(void);
static void MI3130_PwrOn(UINT32 uiStage);
static void MI3130_PwrOff(UINT32 uiStage);
static void MI3130_Sleep(UINT32 uiStage);
static void MI3130_WakeUp(UINT32 uiStage);
static void MI3130_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI3130_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI3130_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI3130_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI3130_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI3130_ChgMode(UINT32 uiMode);
static void MI3130_GetExpo(EXPOSURE_SETTING *Expo);
static void MI3130_CalLine(UINT32 uiTime);
static void MI3130_CalGain(UINT32 uiIso);
static void MI3130_SetFrame(UINT32 uiFrame);
static void MI3130_SetLine(UINT32 uiLine);
static void MI3130_SetPixel(UINT32 uiStep);
static void MI3130_SetVDLength(UINT32 uiVDLength);
static void MI3130_SetGain(UINT32 uiGain);
//#NT#2011/10/26#Ethan Lau -begin
static void MI3130_Flip(UINT32 Direction);
//#NT#2011/10/26#Ethan Lau -end
static SENSOR_TAB g_pMI3130Drv =
{
    // TG control
    MI3130_SifInit,
    MI3130_Init,
    MI3130_PwrOn,
    MI3130_PwrOff,
    MI3130_Sleep,
    MI3130_WakeUp,
    MI3130_WriteReg,
    MI3130_ReadReg,
    MI3130_GetStatus,
    MI3130_GetCurStatus,
    MI3130_ChgMode,
    //feature,
    MI3130_Flip,
    // AE
    MI3130_GetExpo,
    MI3130_CalLine,
    MI3130_CalGain,
    MI3130_SetFrame,
    MI3130_SetLine,
    MI3130_SetPixel,
    MI3130_SetVDLength,
    MI3130_SetGain
};

static SENSOR_STATUS gMI3130Status;
static EXPOSURE_SETTING gMI3130ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI3130Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI3130Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI3130_prv_mode,  // pPrvMode
        MI3130_still_mode   // pStillMode
    };
    //g_pMI3130Info.pPrvMode = &MI3130_prv_mode[CurrentSensorMode];
    return &g_pMI3130Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI3130_SifInit(UINT32 *pPort)
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

    return;
}

static void MI3130_Init(void)
{
    UINT32 idx, CmdLength;
    UINT32 CmdAddr = 0;

    MI3130_GetRegInfo(MI3130_INIT, &CmdAddr, &CmdLength);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI3130_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        //MI3130_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        //SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    /*
    {
        //Read SENSOR ID/INFO
        UINT32 Read_Value;

        MI3130_ReadReg(0x0001, &Read_Value);
        SENDRV_MSG("^G MODEL_ID=0x%04X\r\n",  Read_Value);

        MI3130_ReadReg(0x0002, &Read_Value);
        SENDRV_MSG("^G REVISION_NUMBER=0x%04X\r\n",  Read_Value);

        MI3130_ReadReg(0x0003, &Read_Value);
        SENDRV_MSG("^G MANUFACTURER_ID=0x%04X\r\n",  Read_Value);
    }
    */

    // Initial local status
    memset(&gMI3130Status, 0, sizeof(SENSOR_STATUS));
    memset(&gMI3130ExpoSetting, 0, sizeof(EXPOSURE_SETTING));

    return;
}

static void MI3130_PwrOn(UINT32 uiStage)
{
    /*Do not support*/

    return;
}

static void MI3130_PwrOff(UINT32 uiStage)
{
    /*Do not support*/

    return;
}

static void MI3130_Sleep(UINT32 uiStage)
{
    /*Do not support*/

    return;
}

static void MI3130_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
    return;
}

// write 16-bit & 8-bit data
// In NT9T013 data book, some registers are written in 8-bit format, others are in 16-bit format.
// for details, pls refer to registers (page 18) & register map(page 22)
void MI3130_WriteReg(UINT32 addr, UINT32 value)
{
    UINT32  RegAddr1 = 0, RegAddr2 = 0;
    UINT32  ulData1 = 0, ulData2 = 0;
    BOOL    b8bitData=0;

    //debug_err(("MI3130_WriteReg:addr = 0x%X, value = 0x%X\r\n",addr,value));

    // 0x4000 - 0xFFFF are reserved, so use 0xFFFF to determin if delay
    if(addr == 0xFFFF)  // when addr=0xFFFF, delay & return, unit (ms)
    {
        TimerDelayMs(value);

        return;
    }

    // determin if 8-bit register, pls check MT9T013 data book
    switch(addr)
    {
    case 0x0002:    // REVISION_NUMBER
    case 0x0100:    // MODE_SELECT
    case 0x0101:    // IMAGE_ORIENTATION
    case 0x0103:    // SOFTWARE_RESET
    case 0x0104:    // GROUPED_PARAMETER_HOLD
    case 0x0110:    // CHANNEL_IDENTIFIER
    case 0x0120:    // GAIN_MODE
    case 0x301C:    // MODE_SELECT_
    case 0x301D:    // IMAGE_ORIENTATION_
    case 0x3021:    // SOFTWARE_RESET_
    case 0x3022:    // GROUPED_PARAMETER_HOLD_
    case 0x3023:    // MASK_CORRUPTED_FRAMES_

        b8bitData =1;   // set 8-bit register flag to 1
        break;

    // this group below means the register is read only/reserved, do nothing & return
    case 0x0003:    // MANUFACTURER_ID
    case 0x0004:    // Reserved
    case 0x0005:    // FRAME_COUNT
    case 0x0006:    // PIXEL_ORDER
    case 0x0040:    // FRAME_FORMAT_MODEL_TYPE
    case 0x0041:    // FRAME_FORMAT_MODEL_SUBTYPE
    case 0x00C0:
    case 0x00C1:
    case 0x0111:
        return;

    default:
        b8bitData=0;
    }

    RegAddr1 = ((addr>>8)&0x000000FF);
    RegAddr2 = (addr&0x000000FF);

    if(i2c_lock() != I2C_SUCCESS)
    {
        debug_err(("MI3130_WriteReg:Lock error\r\n"));

        return;
    }

    if(i2c_masterTransmit(SEN_WRITE_ID, TRUE, FALSE) != I2C_SUCCESS)
    {
        debug_err(("MI3130_WriteReg:Write MI3130 Addr error\r\n"));
        i2c_unlock();

        return;
    }

    if(i2c_masterTransmit(RegAddr1, FALSE, FALSE) != I2C_SUCCESS)
    {
        debug_err(("MI3130_WriteReg:Addr MSB error\r\n"));
        i2c_unlock();

        return;
    }

    if(i2c_masterTransmit(RegAddr2, FALSE, FALSE) != I2C_SUCCESS)
    {
        debug_err(("MI3130_WriteReg:Addr LSB error\r\n"));
        i2c_unlock();

        return;
    }

    if(b8bitData)   //  write data in 8-bit format
    {
        if(i2c_masterTransmit(value, FALSE, TRUE) != I2C_SUCCESS)
        {
            debug_err(("MI3130_WriteReg:Data (8bit) error\r\n"));
            i2c_unlock();

            return;
        }

    }
    else    // write data in 16-bit format
    {
        ulData1 = ((value>>8)&0x000000FF);
        ulData2 = ((value)&(0x000000FF));

        if(i2c_masterTransmit(ulData1, FALSE, FALSE) != I2C_SUCCESS)
        {
            debug_err(("MI3130_WriteReg:Data MSB error\r\n"));
            i2c_unlock();

            return;
        }

        if(i2c_masterTransmit(ulData2, FALSE, TRUE) != I2C_SUCCESS)
        {
            debug_err(("MI3130_WriteReg:Data LSB error\r\n"));
            i2c_unlock();

            return;
        }
    }

    i2c_unlock();

    return;
}

static void MI3130_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT erReturn;
    UINT32 ulWriteAddr, ulReadAddr, ulReg1, ulReg2, ulData, ulData1, ulData2;

    i2c_lock();
    TimerDelayUs(100);//add for adpk downgrade wafer,ensure i2c stop and next start have enough time gap
    ulWriteAddr = SEN_WRITE_ID;
    ulReadAddr  = SEN_READ_ID;
    ulReg1 = (uiAddr>>8);
    ulReg2 = ((uiAddr)&(0x000000ff));
    ulData = 0;

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("MI3130 Error transmit data!!(read addr)\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("MI3130 Error transmit data!!(read writeulReg1)\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("MI3130 Error transmit data!!(read writeulReg2)\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);
        if (erReturn !=  I2C_SUCCESS)
        {
            debug_err(("MI3130 Error transmit addr before receive!!(ulReadAddr)\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("MI3130 Error receive data1!!(read ulReg1)\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData2, 1, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("MI3130 Error receive data2!!(read ulData2)\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);
    ulData=((ulData1<<8)|(ulData2));

    *uiValue = ulData;
    i2c_unlock();

    return;
}

static void MI3130_GetCurStatus(PSENSOR_STATUS pStatus)
{
    *pStatus = gMI3130Status;

    return;
}

static void MI3130_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI3130_SetStatus(uiMode,pStatus);

     return;
}

static void MI3130_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;

    MI3130_GetDataInfo(uiMode,&Data,NULL);
    MI3130_GetSigInfo(uiMode,&Sync,NULL);

    switch (uiMode)
    {
    case MI3130_VGA:
    case MI3130_XGA:
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

    // HD-mode added by [NTK#RandyHo]<2012/06/07>
    case MI3130_HD:
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

    case MI3130_STILL:
        pStatus->uiSensorMode = MI3130_STILL;
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

    return;
}

static void MI3130_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 CmdLength;
    UINT32 CmdAddr = 0;

    SENDRV_MSG("^GMI3130_ChgMode()...\r\n");

    debug_err(("^R MI3130_ChgMode to %d\r\n", uiMode));

    MI3130_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        MI3130_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        // MI3130_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        // SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }

    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    //TimerDelayMs(300);

    MI3130_SetStatus(uiMode,&gMI3130Status);

    return;
}

/**
 * AE driver API
 */

static void MI3130_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI3130ExpoSetting;

    return;
}

static void MI3130_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;
    UINT32 Expo_CompensateRatio;

    temp_line = uiTime * gMI3130Status.uiPclk / gMI3130Status.pSyncInfo->HD_total;

    if(temp_line > 0xFFFF)
    {
        temp_line = 0xFFFF;
    }

    if(temp_line < 0x0002)
    {
        temp_line = 0x0002;
    }

    gMI3130ExpoSetting.uiFrame = 0;
    gMI3130ExpoSetting.uiLine = temp_line;
    gMI3130ExpoSetting.uiPixel = 0;

    if(temp_line != 0xFFFF)
    {
        linetime = temp_line * gMI3130Status.pSyncInfo->HD_total / gMI3130Status.uiPclk;
    }
    else
    {
        linetime = uiTime;
    }

    if(linetime<uiTime)
    {
        Expo_CompensateRatio = uiTime*100/linetime;
    }
    else
    {
        Expo_CompensateRatio = 100;
    }

    gMI3130ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;

    return;
}

static void MI3130_CalGain(UINT32 uiIso)
{
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
    MI3130_GetISOInfo(&ISOBase, &AGCBase);

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
        Gain_factor |= 0x200;
    }
    else if(Gain_factor < 4000)//Agc 4x~8x
    {
        Gain_factor/=2;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x280;
    }
    else if(Gain_factor < 8000)//Agc 8x~16x
    {
        Gain_factor/=4;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        //Gain_factor |= 0x1180;
        Gain_factor |= 0x480;
    }
    else if(Gain_factor < 12000) //Agc 16x~24x
    {
        Gain_factor/=6;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x680;
    }
    else if(Gain_factor < 24000) //Agc 24x~48x
    {
        Gain_factor/=12;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0xC80;
    }
    else
    {
        Gain_factor= 0xDff;
    }

    gMI3130ExpoSetting.uiGain = Gain_factor;

    return;
}

static void MI3130_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/

    return;
}

static void MI3130_SetLine(UINT32 uiLine)
{
    MI3130_WriteReg(0x3012, uiLine);        // COARSE_INTEGRATION_TIME_

/*  if(uiLine > (gMI3130Status.pSyncInfo->VD_total - 2))
    {
        MI3130_WriteReg(0x3012, uiLine); //coarse integration time
        MI3130_WriteReg(0x0340, (uiLine + 2)); //coarse integration time
    }
    else
    {
        MI3130_WriteReg(0x3012, uiLine); //coarse integration time
        MI3130_WriteReg(0x0340, (gMI3130Status.pSyncInfo->VD_total + 2)); //coarse integration time
    }
*/
    return;
}

static void MI3130_SetPixel(UINT32 uiStep)
{
    /*Do not support*/

    return;
}

static void MI3130_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/

    return;
}

static void MI3130_SetGain(UINT32 uiGain)
{
    MI3130_WriteReg(0x305E, uiGain);        // R0x305E = 0x022C (default)

    return;
}

static void MI3130_Flip(UINT32 Direction)
{
    //flip enable bit
    #define H_flip  0x4000
    #define V_flip  0x8000
    #define Prv_Reg 5
    #define Cap_Reg 6
    UINT32  CmdLength;
    UINT32  Cmd = 0,Cmd2=0,Cmd3=0, Cmd4=0;
    UINT32 DataPrv1 = 0,DataPrv2 = 0, DataPrv3 = 0, DataCap = 0;
    UINT32 Reg0,Reg1,Reg2, Reg3;
    static BOOL bInit = TRUE;
    static UINT32 OrgData[4],OrgCFA[2];

    SENDRV_MSG("MI3130_Flip()...\r\n");

    MI3130_GetRegInfo(MI3130_VGA, &Cmd, &CmdLength);
    Reg0 = ((SENSOR_CMD*)(Cmd+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI3130_GetRegInfo(MI3130_XGA, &Cmd2, &CmdLength);
    Reg1 = ((SENSOR_CMD*)(Cmd2+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI3130_GetRegInfo(MI3130_HD, &Cmd3, &CmdLength);
    Reg2 = ((SENSOR_CMD*)(Cmd3+Prv_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip
    MI3130_GetRegInfo(MI3130_STILL, &Cmd4, &CmdLength);
    Reg3 = ((SENSOR_CMD*)(Cmd4+Cap_Reg*sizeof(SENSOR_CMD)))->uiData;                                   //flip

    MI3130_GetDataInfo(MI3130_VGA, &DataPrv1, NULL);
    MI3130_GetDataInfo(MI3130_XGA, &DataPrv2, NULL);
    MI3130_GetDataInfo(MI3130_HD, &DataPrv3, NULL);
    MI3130_GetDataInfo(MI3130_STILL, &DataCap, NULL);
    if(bInit == TRUE)
    {
        OrgData[0] = Reg0;
        OrgData[1] = Reg1;
        OrgData[2] = Reg2;
        OrgData[3] = Reg3;

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
        ((PREVIEW_DATA_INFO*)DataPrv3)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
        ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA[1],(SEN_FLIP)Direction);
        //patch registor set
        Reg0 = OrgData[0] | H_flip;         //flip function in table idx 5
        Reg1 = OrgData[1] | H_flip;         //flip function in table idx 5
        Reg2 = OrgData[2] | H_flip;         //flip function in table idx 6
        Reg3 = OrgData[3] | H_flip;
        break;

    case sen_Flip_V:
        //patch bayer pattern
        ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
        ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
         ((PREVIEW_DATA_INFO*)DataPrv3)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
        ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA[1],(SEN_FLIP)Direction);
        //patch registor set
        Reg0 = OrgData[0] | V_flip;         //flip function in table idx 5
        Reg1 = OrgData[1] | V_flip;         //flip function in table idx 5
        Reg2 = OrgData[2] | V_flip;         //flip function in table idx 6
        Reg3 = OrgData[3] | V_flip;
        break;

    case sen_Flip_H_V:
        //patch bayer pattern
        ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
        ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
         ((PREVIEW_DATA_INFO*)DataPrv3)->uiBayer = CfaPat(OrgCFA[0],(SEN_FLIP)Direction);
        ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA[1],(SEN_FLIP)Direction);
        //patch registor set
        Reg0 = OrgData[0] | (H_flip|V_flip);    //flip function in table idx 5
        Reg1 = OrgData[1] | (H_flip|V_flip);    //flip function in table idx 5
        Reg2 = OrgData[2] | (H_flip|V_flip);    //flip function in table idx 6
        Reg3 = OrgData[3] | (H_flip|V_flip);
        break;

    case sen_Flip_NULL:
        //patch bayer pattern
        ((PREVIEW_DATA_INFO*)DataPrv1)->uiBayer = OrgCFA[0];
        ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = OrgCFA[0];
        ((PREVIEW_DATA_INFO*)DataPrv2)->uiBayer = OrgCFA[0];
        ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = OrgCFA[1];
        //patch registor set
        Reg0 = OrgData[0];
        Reg1 = OrgData[1];
        Reg2 = OrgData[2];
        Reg3 = OrgData[3];
        break;

    default:
        debug_err(("^R sensor flip direction error\r\n"));
        break;
    }

    ((SENSOR_CMD*)(Cmd+Prv_Reg*sizeof(SENSOR_CMD)))->uiData = Reg0;
    ((SENSOR_CMD*)(Cmd2+Prv_Reg*sizeof(SENSOR_CMD)))->uiData = Reg1;
     ((SENSOR_CMD*)(Cmd3+Prv_Reg*sizeof(SENSOR_CMD)))->uiData = Reg2;
    ((SENSOR_CMD*)(Cmd4+Cap_Reg*sizeof(SENSOR_CMD)))->uiData = Reg3;

    //do not update sensor status,should undate in next sensor chg mode
    /**
    */

    return;
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
