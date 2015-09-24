/**
    @file       H22.c

    @brief      H22

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
#include "H22.h"

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
static void H22_SifInit(UINT32 *pPort);
static void H22_Init(void);
static void H22_PwrOn(UINT32 uiStage);
static void H22_PwrOff(UINT32 uiStage);
static void H22_Sleep(UINT32 uiStage);
static void H22_WakeUp(UINT32 uiStage);
static void H22_WriteReg(UINT32 uiAddr, UINT32 uiValue);
static void H22_ReadReg(UINT32 uiAddr, UINT32 *uiValue);
//#NT#2011/11/23#Ethan Lau -begin
static void H22_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
static void H22_GetCurStatus(PSENSOR_STATUS pStatus);
static void H22_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus);
//#NT#2011/11/23#Ethan Lau -end
static void H22_ChgMode(UINT32 uiMode);
static void H22_GetExpo(EXPOSURE_SETTING *Expo);
static void H22_CalLine(UINT32 uiTime);
static void H22_CalGain(UINT32 uiIso);
static void H22_SetFrame(UINT32 uiFrame);
static void H22_SetLine(UINT32 uiLine);
static void H22_SetPixel(UINT32 uiStep);
static void H22_SetVDLength(UINT32 uiVDLength);
static void H22_SetGain(UINT32 uiGain);
//#NT#2011/10/26#Ethan Lau -begin
static void H22_Flip(UINT32 Direction);
//#NT#2011/10/26#Ethan Lau -end
//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL H22_checkExposureLongTime(void);
static void H22_SetCalMshutterDelayTime(UINT32 DelayTime);
static UINT32 H22_GetCalMshutterDelayTime(void);
static void H22_TriggerGlobalReset(void);
static void H22_ClearGlobalReset(void);
//#NT#2012/05/28#KS Hung -end

static SENSOR_TAB g_pH22Drv =
{
    // TG control
    H22_SifInit,
    H22_Init,
    H22_PwrOn,
    H22_PwrOff,
    H22_Sleep,
    H22_WakeUp,
    H22_WriteReg,
    H22_ReadReg,
    H22_GetStatus,
    H22_GetCurStatus,
    H22_ChgMode,
    //feature,
    H22_Flip,
    // AE
    H22_GetExpo,
    H22_CalLine,
    H22_CalGain,
    H22_SetFrame,
    H22_SetLine,
    H22_SetPixel,
    H22_SetVDLength,
    H22_SetGain,
    H22_checkExposureLongTime,
    H22_SetCalMshutterDelayTime,
    H22_GetCalMshutterDelayTime,
    H22_TriggerGlobalReset,
    H22_ClearGlobalReset
};

static SENSOR_STATUS gH22Status;
static EXPOSURE_SETTING gH22ExpoSetting;
//static UINT32 CurrentSensorMode=0;

//#NT#2011/11/23#Ethan Lau -begin
//#NT#only used in initialization when sensor open
PSENSOR_TAB  Sensor_GetDrvTab(void)
{
    return &g_pH22Drv;
}
/*
PSENSOR_INFO  Sensor_GetDrvInfo(void)
{
    SENSOR_INFO g_pH22Info =
    {
        0, // uiType
        1, // uiSenID
        143,   // uiCellWidth
        143,  // uiCellHeight

        H22_prv_mode,  // pPrvMode
        H22_still_mode   // pStillMode
    };
    //g_pH22Info.pPrvMode = &H22_prv_mode[CurrentSensorMode];
    return &g_pH22Info;
}
//#NT#2011/11/23#Ethan Lau -end
*/
/**
* Sensor control API
*/
static void H22_SifInit(UINT32 *pPort)
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

static void H22_Init(void)
{
    UINT32 idx, value,CmdLength;
    UINT32 CmdAddr = 0;
    SENDRV_MSG("H22_init...\r\n");
    
    H22_GetRegInfo( H22_INIT, &CmdAddr, &CmdLength );
    // soft reset
    H22_WriteReg( 0x12, 0x80 );
    
    TimerDelayMs( 10 );
    
    for (idx = 0;idx < CmdLength;idx++)
    {
        H22_WriteReg(((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        H22_ReadReg(((SENSOR_CMD*)CmdAddr)->uiAddr, &value);
        SENDRV_MSG("^G %d :Sensor Init W:0x%x\r\n", idx, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr+= (sizeof(SENSOR_CMD));
        TimerDelayMs(5);
    }
    
    // Initial local status
    memset(&gH22Status, 0, sizeof(SENSOR_STATUS));
    memset(&gH22ExpoSetting, 0, sizeof(EXPOSURE_SETTING));
}

static void H22_PwrOn(UINT32 uiStage)
{
    /*Do not support*/
}

static void H22_PwrOff(UINT32 uiStage)
{
    /*Do not support*/
}

static void H22_Sleep(UINT32 uiStage)
{
    /*Do not support*/
}

static void H22_WakeUp(UINT32 uiStage)
{
    /*Do not support*/
}

static void H22_WriteReg(UINT32 uiAddr, UINT32 uiValue)
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

static void H22_ReadReg(UINT32 uiAddr, UINT32 *uiValue)
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

static void H22_GetCurStatus(PSENSOR_STATUS pStatus)
{
    *pStatus = gH22Status;
}

static void H22_GetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    H22_SetStatus(uiMode,pStatus);
}

static void H22_SetStatus(UINT32 uiMode,PSENSOR_STATUS pStatus)
{
    UINT32 Data = 0 ,Sync = 0;
    H22_GetDataInfo(uiMode,&Data,NULL);
    H22_GetSigInfo(uiMode,&Sync,NULL);
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

static void H22_ChgMode(UINT32 uiMode)
{
    UINT8 idx;
    UINT32 value, CmdLength;
    UINT32 CmdAddr = 0;
    H22_GetRegInfo( uiMode, &CmdAddr, &CmdLength );
    
    for (idx = 0; idx < CmdLength; idx++)
    {
        H22_WriteReg( ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData );
        H22_ReadReg(  ((SENSOR_CMD*)CmdAddr)->uiAddr, &value );
        SENDRV_MSG("%d :Sensor mode(%d) A:0x%x W:0x%x\r\n", idx, uiMode, ((SENSOR_CMD*)CmdAddr)->uiAddr, ((SENSOR_CMD*)CmdAddr)->uiData);
        CmdAddr += (sizeof(SENSOR_CMD));
    }
    //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
    TimerDelayMs( 300 );
    
    H22_SetStatus( uiMode, &gH22Status );
}

/**
 * AE driver API
 */

static void H22_GetExpo(EXPOSURE_SETTING *pExpo)
{
    *pExpo = gH22ExpoSetting;
}

//#NT#2012/05/28#KS Hung -begin
//#NT#for M-shutter calibration
static BOOL H22_checkExposureLongTime(void)
{
    return FALSE;
}

static void H22_SetCalMshutterDelayTime(UINT32 DelayTime)
{

}

static UINT32 H22_GetCalMshutterDelayTime(void)
{
    return 0;
}

static void H22_TriggerGlobalReset(void)
{

}

static void H22_ClearGlobalReset(void)
{

}
//#NT#2012/05/28#KS Hung -end

static void H22_CalLine(UINT32 uiTime)
{
    UINT32 temp_line,linetime;

    temp_line = uiTime * gH22Status.uiPclk / gH22Status.pSyncInfo->HD_total;
/////////////////////kevin chen///////////////////
    if(temp_line > 0x09FE)//(temp_line > 0xFFFF)
      temp_line = 0x09FF;// temp_line = 0xFFFF;

    if(temp_line < 0x0001)
       temp_line = 0x0001;

    gH22ExpoSetting.uiFrame = 0;
    gH22ExpoSetting.uiLine = temp_line;
    gH22ExpoSetting.uiPixel = 0;

    if(temp_line !=0x09ff)// 0xFFFF)
        linetime = temp_line * gH22Status.pSyncInfo->HD_total / gH22Status.uiPclk;
    else
        linetime = uiTime;

    if (linetime < uiTime)
        gH22ExpoSetting.uiExpoRatio = uiTime * 100 / linetime;
    else
        gH22ExpoSetting.uiExpoRatio = 100;
}

static void H22_CalGain(UINT32 uiIso)
{
    #define GAIN_STEP       625
    #define AGC_GAP         16
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT16 shf;
    UINT32 AGCBase,ISOBase;
    
    H22_GetISOInfo( &ISOBase, &AGCBase );
    
    if(uiIso > 800)//(uiIso > 800)
        uiIso = 800;
    
    Calcul_AGC_Ratio = (UINT32)(((UINT32)100*(UINT32)uiIso)/(UINT32)ISOBase);
    
    shf = 0;
    
    while(Calcul_AGC_Ratio/((1<<(shf+1))*100))
        shf++;
    
    Gain_factor = ((Calcul_AGC_Ratio-((1<<shf)*100))*100)/(GAIN_STEP<<shf) + AGCBase + AGC_GAP*shf;
    ///////////////////kevinchen/////////////////
    if (Gain_factor > 0xff)
    {
        Gain_factor = 0x1f;
    }
    //////////////////////////////////////////////////////////
    
    gH22ExpoSetting.uiGain = Gain_factor;
}

static void H22_SetFrame(UINT32 uiFrame)
{
    /*Do not support*/
}

static void H22_SetLine(UINT32 uiLine)
{

    UINT32 LineTemp;

    if( uiLine > 0xffff )
    {
        uiLine = 0xffff;
    	
    }
    LineTemp     = uiLine;
    
    if( LineTemp > 0x799)
    {
        LineTemp = 0xffff;
    }
//  H22_WriteReg( 0x02, ((uiLine & 0xff00) >> 8) );
//  H22_WriteReg( 0x01, (uiLine & 0x00ff));
    H22_WriteReg( 0x02, ((LineTemp & 0xff00) >> 8) );
    H22_WriteReg( 0x01, (LineTemp & 0x00ff));
    //debug_err(("exposure time (Line) !!\r\n"));
}

static void H22_SetPixel(UINT32 uiStep)
{
    /*Do not support*/
}

static void H22_SetVDLength(UINT32 uiVdLength)
{
    /*Do not support*/
}


static void H22_SetGain(UINT32 uiGain)
{
    
    UINT32 Line;
    UINT16 Temp,Temp1,Temp2;
    UINT16 AGC = 0;
    UINT16 TempGain; 

     //i2c_unlock();

//  Temp1=uiGain;
//  debug_err(("UI  gain = 0x%x\r\n",uiGain));
    /*if(uiGain>0x14)
        Temp1=Temp1+4;
    else if(uiGain>0x17)
        Temp1=(uiGain<<1);//Temp1+3;
    */
    //uiGain=Temp1;
//  Temp = (Temp1 >> 4);
    //Temp = (uiGain >> 4);
//  debug_err(("uiGain  = 0x%x\r\n", uiGain));
/*  
    if( Temp > 0 )
    	{
    ///////////////////////////kevinchen//////////////////////
        //AGC = (((1 << Temp) - 1) << 4);

        if(Temp==1)
                AGC=(Temp<<4);
        	else
                if(Temp==2)
                    AGC=(Temp<<4);
        		else
                	if(Temp==3)
                    	AGC=(Temp<<4);
    
    }
*/  
    Temp = (uiGain >> 4);

    if(Temp > 0)
        AGC = (((1 << Temp) - 1) << 4);
//  AGC =  (Temp << 4);

    Temp = (uiGain & 0x0F);
    
    AGC = AGC + Temp;

    


    if(AGC > 0x1F)
    {
        AGC = 0x1F;	
    }
    if(AGC<0X01)
    {     
        H22_WriteReg( 0x49, 0x00);//0x83);
        H22_WriteReg( 0x0C, 0x00); //Black sun

    }
    else
    {
        H22_WriteReg( 0x49, 0x00);//0x83);
        H22_WriteReg( 0x0C, 0x40); //Black sun

    }
    
/*
       if(AGC>0X1c)
    {
//      H22_WriteReg( 0x00, (((unsigned char)AGC) & 0x0f));
        H22_WriteReg( 0x00, (unsigned char)AGC );
    }
    else
    {
        H22_WriteReg( 0x00, (unsigned char)AGC );   //analog gain
    }
*/
//  AGC = 0x1e;
//  H22_WriteReg( 0x49,0x06);//0x03);
//  H22_WriteReg( 0x00, (unsigned char)AGC );       //analog gain
//  H22_WriteReg( 0x00, 0x1c);
//  H22_WriteReg( 0x49, 0x83);
//  H22_WriteReg( 0x0d, 0x41);

/*
    if(AGC<0X01)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }	
    else if(AGC<0X02)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X03)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X04)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X05)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X06)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X07)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }	
    else if(AGC<0X08)
    {     
//      H22_WriteReg( 0x0d, 0x40);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X09)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X0a)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X0b)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X0c)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X0d)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x10);
    }
    else if(AGC<0X0e)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X0f)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X10)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }	
    else if(AGC<0X11)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X12)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X13)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X14)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X15)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X16)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X17)
    {     
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X18)
    {
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X19)
    {
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X1a)
    {
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X1b)
    {
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X1c)
    {
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
    else if(AGC<0X1d)
    {
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }	
    else
    {
//      H22_WriteReg( 0x0d, 0x41);
        H22_WriteReg( 0x49, 0x00);
    }
*/  
    H22_WriteReg( 0x00, (unsigned char)AGC );       //analog gain
    H22_WriteReg( 0x49,0x00);
/*
    if(AGC < 0x11)
    {

        H22_WriteReg( 0x00, (unsigned char)AGC );
    }
    else
    {
        H22_WriteReg( 0x00, (((unsigned char)AGC) & 0x0f));
    }
*/  
    //Dummy line
    /////////////////kevinchen/////////////////////////////////////////////
    if( gH22ExpoSetting.uiLine > (gH22Status.pSyncInfo->VD_total-4))
    {
        /////kevinchen//////Line = gH22ExpoSetting.uiLine - gH22Status.pSyncInfo->VD_total;
        Line= gH22ExpoSetting.uiLine+4;

    	
              //if( Line > 0x3C4 )
        //if( Line > 0x4dB )//20fps
        if( Line > 0x81B )//12fps
    	{
        //  Line = 0x3C4;
        //  Line = 0x67C; //20fps
            Line = 0x81B; //12fps
    		
    	}
    	
        //if( Line > 0xffff )
        //{
        //  Line = 0xffff;
        //}
/////////////////kevinchen/////////////////////////////////////////////
        H22_WriteReg( 0x23, ((Line & 0xff00) >> 8) );
        H22_WriteReg( 0x22, (Line & 0x00ff));
///     H22_WriteReg( 0x2E, ((Line & 0xff00) >> 8) );
///     H22_WriteReg( 0x2D, ((Line & 0x00ff) >> 0) );
    }
    else
    {
        //Line = 0;
        H22_WriteReg( 0x23, 0x03 ); //25ps 42M
        H22_WriteReg( 0x22, 0xE4 );
//      H22_WriteReg( 0x23, 0x03);  //30fps
//      H22_WriteReg( 0x22, 0x3E);
//      H22_WriteReg( 0x22, 0x3D );
    //  H22_WriteReg( 0x23, ((Line & 0xff00) >> 8) );
    //  H22_WriteReg( 0x22, ((Line & 0x00ff) >> 0) );
    }
    
        debug_err(("Dummy line = 0x%x\r\n",Line));
        //i2c_lock();
}


static void H22_Flip(UINT32 Direction)
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


    H22_GetRegInfo(H22_1920X1080, &Cmd1, &CmdLength);

    H22_GetRegInfo(H22_STILL, &Cmd, &CmdLength);
    Reg0 = ((SENSOR_CMD*)(Cmd+Cap_Reg*sizeof(SENSOR_CMD)))->uiData;

    H22_GetDataInfo(H22_1920X1080, &DataCap1, NULL);
    H22_GetDataInfo(H22_STILL, &DataCap, NULL);
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
//            ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);  //Jh flip mirr color error 
//        ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
            //patch registor set
            Reg0 = OrgData | H_flip;         //flip function in table idx 5

            break;
        case sen_Flip_V:
            //patch bayer pattern
      //      ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
      //      ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = CfaPat(OrgCFA,(SEN_FLIP)Direction);
      //patch registor set
            Reg0 = OrgData | V_flip;         //flip function in table idx 5

            break;
        case sen_Flip_H_V:
            //patch bayer pattern
      //      ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = CfaPat(OrgCFA,sen_Flip_V);
      //     ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = CfaPat(OrgCFA,sen_Flip_V);
            //patch registor set
            Reg0 = OrgData | (H_flip|V_flip);    //flip function in table idx 5
           //H22_WriteReg( 0x12, 0x00 );
           //H22_WriteReg( 0x27, 0xE9 );
           //H22_WriteReg( 0x28, 0x0d );


            break;
        case sen_Flip_NULL:
      //    patch bayer pattern
       //    ((CAPTURE_DATA_INFO*)DataCap)->uiBayer = OrgCFA;
          //  ((CAPTURE_DATA_INFO*)DataCap1)->uiBayer = OrgCFA;
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
/////////////org///////
/*
        {SIECFA_R,  SIECFA_Gr,  SIECFA_Gb,  SIECFA_B},
        {SIECFA_Gr, SIECFA_R,   SIECFA_B,   SIECFA_Gb},
        {SIECFA_Gb, SIECFA_B,   SIECFA_R,   SIECFA_Gr},
        {SIECFA_B,  SIECFA_Gb,  SIECFA_Gr,  SIECFA_R}        
*/        

        {SIECFA_R,  SIECFA_Gr,  SIECFA_Gb,  SIECFA_B},
        {SIECFA_Gr, SIECFA_R,   SIECFA_B,   SIECFA_Gb},
        {SIECFA_Gb, SIECFA_B,   SIECFA_R,   SIECFA_Gr},
        {SIECFA_B,  SIECFA_Gb,  SIECFA_Gr,  SIECFA_R} 

////////////////correct/////////////  H22 {0x27,0xea},{0x28,0x0c}
/*
        {SIECFA_Gr,  SIECFA_R,  SIECFA_R,  SIECFA_R},
        {SIECFA_R, SIECFA_Gr,   SIECFA_R,   SIECFA_R},
        {SIECFA_R, SIECFA_R,   SIECFA_R,   SIECFA_R},
        {SIECFA_R,  SIECFA_R,  SIECFA_R,  SIECFA_Gr}               
*/        

    };
    
    return PatTable[OrgPat][FlipType];
}
//@@ EOF
