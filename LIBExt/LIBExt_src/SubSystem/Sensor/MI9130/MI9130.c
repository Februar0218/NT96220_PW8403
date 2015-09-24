/**
    @file       MI9130.c

    @brief      MI9130

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
#include "MI9130.h"
#include "ImageAlgInfor.h"
#include "Calibration.h"

#define SEN_READ_ID 0x21//0x6D
#define SEN_WRITE_ID 0x20//0x6C

#define SENDRV_DEBUG  DISABLE

#if (SENDRV_DEBUG)
#define SENDRV_MSG(...)    debug_msg (" SENDRV: "__VA_ARGS__);
#else
#define SENDRV_MSG(...)
#endif

// function prototype
static void MI9130_SifInit(UINT32 *pPort);
static void MI9130_Init(void);
static void MI9130_PwrOn(UINT32 uiStage);
static void MI9130_PwrOff(UINT32 uiStage);
static void MI9130_Sleep(UINT32 uiStage);
static void MI9130_WakeUp(UINT32 uiStage);
static void MI9130_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void MI9130_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void MI9130_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void MI9130_GetCurStatus(PSENSOR_STATUS pStatus);
static void MI9130_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void MI9130_ChgMode(UINT32 uiMode);
static void MI9130_GetExpo(EXPOSURE_SETTING *Expo);
static void MI9130_CalLine(UINT32 uiTime);
static void MI9130_CalGain(UINT32 uiIso);
static void MI9130_SetFrame(UINT32 uiFrame);
static void MI9130_SetLine(UINT32 uiLine);
static void MI9130_SetPixel(UINT32 uiStep);
static void MI9130_SetVDLength(UINT32 uiVDLength);
static void MI9130_SetGain(UINT32 uiGain);
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI9130_checkExposureLongTime(void);
static void MI9130_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 MI9130_GetCalMshutterDelayTime(void);
static void MI9130_TriggerGlobalReset(void);
static void MI9130_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static SENSOR_TAB g_pMI9130Drv =
{
    // TG control
    MI9130_SifInit,
    MI9130_Init,
    MI9130_PwrOn,
    MI9130_PwrOff,
    MI9130_Sleep,
    MI9130_WakeUp,
    MI9130_WriteReg,
    MI9130_ReadReg,
    MI9130_GetStatus,
    MI9130_GetCurStatus,
    MI9130_ChgMode,
    //feature,
    NULL,
    // AE
    MI9130_GetExpo,
    MI9130_CalLine,
    MI9130_CalGain,
    MI9130_SetFrame,
    MI9130_SetLine,
    MI9130_SetPixel,
    MI9130_SetVDLength,
    MI9130_SetGain,
    MI9130_checkExposureLongTime,
    MI9130_SetCalMshutterDelayTime,
    MI9130_GetCalMshutterDelayTime,
    MI9130_TriggerGlobalReset,
    MI9130_ClearGlobalReset
};
//#NT#2012/05/28#KS Hung -end

static SENSOR_STATUS gMI9130Status;
static EXPOSURE_SETTING gMI9130ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pMI9130Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pMI9130Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        MI9130_prv_mode,  // pPrvMode
        MI9130_still_mode   // pStillMode
    };
    //g_pMI9130Info.pPrvMode = &MI9130_prv_mode[CurrentSensorMode];
    return &g_pMI9130Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void MI9130_SifInit(UINT32 *pPort)
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

    erReturn = i2c_initMaster(I2C_CLOCK_DIVIDER_100KHZ, I2C_TSR_100KHZ, I2C_GSR_100KHZ);

    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }
}

static void MI9130_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("MI9130_init...\r\n");

    MI9130_GetRegInfo(MI9130_INIT, &CmdAddr, &CmdLength);
    MI9130_WriteReg(0x0103, 0x0001);
    TimerDelayMs(300);
    #if 0//for 1280x960 MCLK=24MHz Pixclk=78MHz
    MI9130_WriteReg(0x0300, 0x0004);//vt_pix_clk_div
    MI9130_WriteReg(0x0302, 0x0001);//vt_sys_clk_div
    MI9130_WriteReg(0x0304, 0x0002);//pre_pll_clk_div
    MI9130_WriteReg(0x0306, 0x0034);//pll_multiplier
    MI9130_WriteReg(0x0308, 0x0008);//op_pix_clk_div
    MI9130_WriteReg(0x030A, 0x0001);//op_sys_clk_div
    MI9130_WriteReg(0x3016, 0x0111);//row_speed
    #elif 0//for 960x720 MCLK=12MHz Pixclk=48MHz
    MI9130_WriteReg(0x0300, 0x0004);//vt_pix_clk_div
    MI9130_WriteReg(0x0302, 0x0001);//vt_sys_clk_div
    MI9130_WriteReg(0x0304, 0x0002);//pre_pll_clk_div
    MI9130_WriteReg(0x0306, 0x0060);//pll_multiplier
    MI9130_WriteReg(0x0308, 0x000c);//op_pix_clk_div
    MI9130_WriteReg(0x030A, 0x0001);//op_sys_clk_div
    MI9130_WriteReg(0x3016, 0x0111);//row_speed    
    #else//for 640 x480 MCLK=12MHz Pixclk=33MHz
    MI9130_WriteReg(0x0300, 0x0006);//vt_pix_clk_div
    MI9130_WriteReg(0x0302, 0x0001);//vt_sys_clk_div
    MI9130_WriteReg(0x0304, 0x0001);//pre_pll_clk_div
    MI9130_WriteReg(0x0306, 0x0042);//pll_multiplier
    MI9130_WriteReg(0x0308, 0x000C);//op_pix_clk_div
    MI9130_WriteReg(0x030A, 0x0001);//op_sys_clk_div
    MI9130_WriteReg(0x3016, 0x0111);//row_speed
    #endif
    //soft reset
    //MI9130_WriteReg(0x0100, 0x00);

    for (idx = 0;idx < CmdLength;idx++)
    {
        MI9130_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        MI9130_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    MI9130_WriteReg(0x0104, 0x01);
    //MI9130_WriteReg(0x0104, 0x00);
    //MI9130_WriteReg(0x0100, 0x01);

    // Initial local status
    memset(&gMI9130Status, 0, sizeof(SENSOR_STATUS));
    memset(&gMI9130ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void MI9130_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI9130_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI9130_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI9130_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void MI9130_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulReg2, ulData1, ulData2;

    i2c_lock();

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

static void MI9130_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulReg2, ulData, ulData1, ulData2;

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

static void MI9130_GetCurStatus(PSENSOR_STATUS pStatus)
{
     *pStatus = gMI9130Status;
}

static void MI9130_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
     MI9130_SetStatus(uiMode,pStatus);
}

static void MI9130_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    MI9130_GetDataInfo(uiMode,&Data,NULL);
    MI9130_GetSigInfo(uiMode,&Sync,NULL);
    switch (uiMode)
    {
        case SENSOR_MODE_640X480:
        case SENSOR_MODE_960X720:
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

static void MI9130_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;

    MI9130_GetRegInfo(uiMode,&CmdAddr,&CmdLength);

    for (idx = 0; idx < CmdLength; idx++)
    {
        MI9130_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        MI9130_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    //TimerDelayMs(300);

    MI9130_SetStatus(uiMode,&gMI9130Status);
}

/**
 * AE driver API
 */

static void MI9130_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gMI9130ExpoSetting;
}

static BOOL Sensor_LongExpo = FALSE;
static INT32 GR_rst_end, GR_shutter_start, GR_read_start;
static UINT32 gCalMshutterDelayTime=9999;

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL MI9130_checkExposureLongTime(void)
{
    return Sensor_LongExpo;
}

static void MI9130_SetCalMshutterDelayTime(UINT32 DelayTime)
{
    gCalMshutterDelayTime = DelayTime;
}

static UINT32 MI9130_GetCalMshutterDelayTime(void)
{
    return gCalMshutterDelayTime;
}

static void MI9130_TriggerGlobalReset(void)
{
    UINT32 value;
    value=Sensor_ReadReg(0x3160);
    value=value | 0x00000001;
    Sensor_WriteReg(0x3160, value);

    //debug_msg("Sensor_TriggerGlobalReset\r\n");
}

static void MI9130_ClearGlobalReset(void)
{
    UINT32 value;
    value=Sensor_ReadReg(0x3160);
    value=value & 0xFFFFFFFE;
    //debug_err(("clr val 0x%x\r\n",value));
    Sensor_WriteReg(0x3160, value);
}
//#NT#2012/05/28#KS Hung -end

void MI9130_SetBulbMode(BOOL Enable)
{
    UINT32 value;

    value=Sensor_ReadReg(0x3160);
    if(Enable)
        value=value | 0x00000002;
    else
        value=value & (~0x00000002);
    Sensor_WriteReg(0x3160, value);
}

static void MI9130_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;
    UINT32 Expo_CompensateRatio;

    PImageAlgInfo pimginfo;
    CAL_DATA CalInfo;

    UINT32 mshutteroffset,ShutterCloseLeadTime;

    //CAPTURE_DATA_INFO Data;
    //UINT32 vt_pix_clk_freq_mhz;

    pimginfo = UI_GetParameter();
    Cal_GetCalData(&CalInfo);
    //MI9130_GetDataInfo(SENSOR_MODE_FULL,&Data,NULL);
    //vt_pix_clk_freq_mhz = ((CAPTURE_DATA_INFO*)Data)->uiPclk*2;

    if((pimginfo->Exposuremode_UI == _GRS)&&(gMI9130Status.uiSensorMode == SENSOR_MODE_FULL)){
        if(CalInfo.CalStatus_Mshutter == _Mshutter_Status){
            mshutteroffset = CalInfo.Cal_Mshutterr_Data[0];
        }
        else{
            if(MI9130_GetCalMshutterDelayTime() != 9999)
                mshutteroffset = MI9130_GetCalMshutterDelayTime();
            else
                mshutteroffset = DEFAULT_MSHUTTER_TIME;
        }

        if(uiTime > 200000)
            uiTime = 200000;

        GR_rst_end=0x600;
        GR_read_start=uiTime*gMI9130Status.uiPclk*2*2/*vt_pix_clk_freq_mhz*//512+GR_rst_end;//internal clock=PCLK*2
        ShutterCloseLeadTime = mshutteroffset*gMI9130Status.uiPclk*2*2/*vt_pix_clk_freq_mhz*//512;
        if(GR_read_start > ShutterCloseLeadTime)
            GR_shutter_start = GR_read_start - ShutterCloseLeadTime;
        else
            GR_shutter_start = 0;

        if(GR_read_start>0xFF00)
            GR_read_start=0xFF00;

        if(GR_shutter_start>0xFF00)
            GR_shutter_start=0xFF00;
        debug_msg("Time=%d rst end=%d shutter start=%d read start=%d\r\n",uiTime,GR_rst_end,GR_shutter_start,GR_read_start);
        if(uiTime <= 200000)
        {
            MI9130_SetBulbMode(FALSE);
            Sensor_LongExpo = 0;
        }
        else
        {
            MI9130_SetBulbMode(TRUE);
            Sensor_LongExpo = 1;
        }
    }
    else{
        temp_line = uiTime*gMI9130Status.uiPclk/gMI9130Status.pSyncInfo->HD_total;

        if(temp_line > 0xFFFF)
           temp_line = 0xFFFF;

        if(temp_line < 0x0001)
           temp_line = 0x0001;

        gMI9130ExpoSetting.uiFrame = 0;
        gMI9130ExpoSetting.uiLine = temp_line;
        gMI9130ExpoSetting.uiPixel = 0;

        if(temp_line != 0xFFFF)
            linetime = temp_line * gMI9130Status.pSyncInfo->HD_total / gMI9130Status.uiPclk;
        else
            linetime = uiTime;

        if(linetime<uiTime)
            Expo_CompensateRatio = uiTime*100/linetime;
        else
            Expo_CompensateRatio = 100;

        gMI9130ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    }
}

static void MI9130_CalGain(UINT32 uiIso)
{
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT32 AGCBase,ISOBase;
    CAL_DATA CalInfo;
    Cal_GetCalData(&CalInfo);

    MI9130_GetISOInfo(&ISOBase, &AGCBase);

    if (CalInfo.CalStatus_ISO == _ISO_Status)
        AGCBase = CalInfo.Cal_ISO_Data[0];

    Calcul_AGC_Ratio = AGCBase;

    Gain_factor= Calcul_AGC_Ratio*100*uiIso/ISOBase/100;

    //upper and lower bound
    if(Gain_factor < 500)
    {
        Gain_factor = 500;
    }
    if(Gain_factor > 24000)
    {
        Gain_factor = 24000;
    }

    if(Gain_factor < 2000)//Agc 0x~4x
    {
        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x1080;
    }
    else if(Gain_factor < 4000)//Agc 4x~8x
    {
        Gain_factor/=2;

        Gain_factor = (Gain_factor * 32 )/1000 ;
        Gain_factor |= 0x2080;
    }
    else if(Gain_factor < 8000)//Agc 8x~16x
    {
        Gain_factor/=4;

        Gain_factor = (Gain_factor * 32 )/1000 ;
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

    gMI9130ExpoSetting.uiGain = Gain_factor;
}

static void MI9130_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void MI9130_SetLine(UINT32 uiLine)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();

    if((pimginfo->Exposuremode_UI == _GRS)&&(gMI9130Status.uiSensorMode == SENSOR_MODE_FULL)){
            Sensor_WriteReg(0x3162, GR_rst_end);
            Sensor_WriteReg(0x3164, GR_shutter_start);
            Sensor_WriteReg(0x3166, GR_read_start);
    }
    else{
        if(uiLine > (gMI9130Status.pSyncInfo->VD_total-5))
        {
            MI9130_WriteReg(0x3012, uiLine); //coarse integration time
            MI9130_WriteReg(0x0340, uiLine+5); //coarse integration time
        }
        else
        {
            MI9130_WriteReg(0x3012, uiLine); //coarse integration time
            MI9130_WriteReg(0x0340, gMI9130Status.pSyncInfo->VD_total); //coarse integration time
        }
    }
}

static void MI9130_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void MI9130_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}

static void MI9130_SetGain(UINT32 uiGain)
{
    MI9130_WriteReg(0x305E, uiGain);
}

//@@ EOF
