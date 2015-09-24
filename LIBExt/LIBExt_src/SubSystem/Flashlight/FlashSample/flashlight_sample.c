#include <stdio.h>

#include "type.h"
#include "top.h"
#include "OSCommon.h"
#include "Utility.h"
#include "Flashlight_sample.h"
#include "flash_alg.h"
#include "ImageAlgInfor.h"
#include "aaa_lib.h"
#include "Sie_lib.h"
#include "Sensor.h"
#include "Ae_Alg.h"
#include "Flashlight_api.h"
#include "debug.h"


static FLASH_PARAM FlashParam={0};
FLASH_INPUTPARAM FlashInputParam;


FLASH_PARAM* GetFlashParam(void)
{
    return &FlashParam;
}

//    flash trigger

void flash_RedEyeReduct(void)
{
    UINT32 DelayLine, RedEyeFlashWidth,IntrLvBtwnRedEyePrvFlsh;
    SENSOR_STATUS SensorStatus;

    if(FlashParam.Enable && ((gImageAlgInfo.FlashMode == _IMGFlashMode_AutoRedEye)||(gImageAlgInfo.FlashMode == _IMGFlashMode_ForceONRedEye)))
    {
        Sensor_GetStatus(&SensorStatus);
        DelayLine = 1;
        RedEyeFlashWidth = REDEYE_WIDTH*SensorStatus.uiPclk;// preflash width(us) * pixel clock(MHz)
        IntrLvBtwnRedEyePrvFlsh = 300;//ms
        flash_ConfigPreFlash(DelayLine,RedEyeFlashWidth,FALSE);    //Configure preflash width and delay
        flash_TriggerFlash();
        sie_waitVD(1);
        TimerDelayMs(IntrLvBtwnRedEyePrvFlsh);
    }
}

/************************ Modified by Project ***********************************/

#define PreFlashY_n 14
#define MainTargetY_n 6
UINT32 NoFlash_R[8][8],NoFlash_G[8][8],NoFlash_B[8][8],PreFlash_R[8][8],PreFlash_G[8][8],PreFlash_B[8][8];

UINT32 PreFlashYTbl[PreFlashY_n] = {255,249,237,207,170,150,122,103,75,55,40,33,22,0}; // 0m,0.3m,0.45m,0.6m,0.75m,0.9m, 1.05m, 1.2m, 1.5m, 1.8m, 2.1m, 2.4m,2.7m infinity
UINT32 MainFlashYTbl[MainTargetY_n] = {125,100,75,50,25,0};
UINT32 MainFlashWidthTbl[MainTargetY_n][PreFlashY_n] ={
{3,4,7,8,10,11,13,15,19,23,33,36,60,150},
{2,3,5,7,8,9,10,13,17,21,31,33,48,100},
{1,3,4,6,7,8,10,12,15,18,22,27,35,50},
{1,2,3,4,5,6,7,8,10,13,17,22,27,33},
{1,1,2,4,6,9,9,10,11,12,13,14,15,18},
{1,1,2,3,4,4,4,4,5,5,5,6,6,7},
};
UINT32 FlashWeightTable[8][8] =
{
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

void flash_Evaluate(void)
{
    switch(gImageAlgInfo.FlashMode)
    {
        case _IMGFlashMode_Auto:
        case _IMGFlashMode_AutoRedEye:
            if(AEAlg_GetPrvLV()<FLASH_TRIGGERPOINT) // LV5.0
                FlashParam.Enable = 1;
            else
                FlashParam.Enable = 0;
            break;
        case _IMGFlashMode_ForceON:
        case _IMGFlashMode_ForceONRedEye:
        case _IMGFlashMode_NightStrobe:
            FlashParam.Enable = 1;
            if(AEAlg_GetPrvLV()>FLASH_TRIGGERPOINT) // LV5.0
                FlashParam.FakeFlash = 1;
            else
                FlashParam.FakeFlash = 0;
            break;

        case _IMGFlashMode_ForceOFF:
        default:
            FlashParam.Enable = 0;
            break;
    }
}

void flash_PreFlashFlow_IGBT(void)
{
    UINT32 i,j,DelayLine, PreFlashWidth, MainFlashWidth, PreFlashBlockY[8][8], BackGndBlockY[8][8];
    SENSOR_STATUS SensorStatus = {0},CapSensorStatus = {0};

    aaa_AeLock(LOCK);    //Stop AE
    flash_Evaluate();   //Evaluate flash status according to menu setting

#if 0//PREFLASH_CHARGE
        SensorInfor.FP_SetFlashCharge(1,0);
#endif

    if(FlashParam.Enable)
    {
        //#NT#2011/11/24#Ethan Lau -begin
        //#NT#replace sensor driver api
        Sensor_GetStatus(&SensorStatus);
        Sensor_GetStatus_Indicated(SENSOR_MODE_FULL, &CapSensorStatus);
        flash_RedEyeReduct();
        //FlashInputParam.WidthPreFlash = 20;
        FlashInputParam.PreflashTbale = &PreFlashYTbl[0];
        FlashInputParam.ExpectYTbale = &MainFlashYTbl[0];
        FlashInputParam.FlashCurveTbale = &MainFlashWidthTbl[0][0];
        //FlashInputParam.PreviewISO = 200;
        FlashInputParam.PreviewExpoTime = SensorStatus.pSyncInfo->HD_total*(SensorStatus.pSyncInfo->VD_total)/SensorStatus.uiPclk;
        FlashInputParam.CaptureExpoTime = CapSensorStatus.pSyncInfo->HD_total*(CapSensorStatus.pSyncInfo->VD_total)/CapSensorStatus.uiPclk;
        //FlashInputParam.DelayLinePreFlash = 960;
        FlashInputParam.PreviewBinning = CapSensorStatus.uiBinning;
        //FlashInputParam.PixelClock = 60;
        FlashInputParam.WeightingTable = &FlashWeightTable[0][0];
        FlashInputParam.CurveTbl_RowNum = PreFlashY_n;
        FlashInputParam.CurveTbl_ColNum = MainTargetY_n;

        DelayLine = SensorStatus.pSyncInfo->VD_total-5;
        PreFlashWidth = PRE_FLASH_WIDTH*SensorStatus.uiPclk;// preflash width(us) * pixel clock(MHz)
        Sensor_SetExpTime(FlashInputParam.PreviewExpoTime);
        Sensor_SetISO(200); // Modify ISO by LV
        sie_waitVD(3);
        //Get_CA_RSLT(&NoFlash_R[0][0],&NoFlash_G[0][0],&NoFlash_B[0][0]);
        Sen_GetSubOutRGBinfo(&NoFlash_R[0][0],&NoFlash_G[0][0],&NoFlash_B[0][0],8*8);
        flash_ConfigPreFlash(DelayLine,PreFlashWidth,FALSE);    //Configure preflash width and delay
        flash_TriggerFlash();
        sie_waitVD(2);
        //Get_CA_RSLT(&PreFlash_R[0][0],&PreFlash_G[0][0],&PreFlash_B[0][0]);
        Sen_GetSubOutRGBinfo(&PreFlash_R[0][0],&PreFlash_G[0][0],&PreFlash_B[0][0],8*8);
        for(j=0;j<8;j++)
        {
            for(i=0;i<8;i++)
            {
                BackGndBlockY[j][i] = NoFlash_G[j][i];
                PreFlashBlockY[j][i] = PreFlash_G[j][i];
            }
        }
        FlashInputParam.BackGroundBlockY = &BackGndBlockY[0][0];
        FlashInputParam.PeflashBlockY = &PreFlashBlockY[0][0];

        MainFlashWidth = flash_AEConverge(&FlashInputParam);
        FlashParam.WidthMainFlash = MainFlashWidth*SensorStatus.uiPclk; // *Pixelclock(MHz)
        FlashParam.ISO_MainFlash = 200; // Modify ISO by LV
        FlashParam.ExpoTime_MainFlash = FlashInputParam.CaptureExpoTime;
        FlashParam.DelayLineMainFlash = CapSensorStatus.pSyncInfo->VD_total-10;
        //#NT#2011/11/24#Ethan Lau -end
    }
    SetFlashFireStatus(FlashParam.Enable);
}

void flash_PreFlashFlow_SCR(void)
{
    //#NT#2011/11/24#Ethan Lau -begin
    //#NT#replace sensor driver api
    SENSOR_STATUS SensorStatus = {0},CapSensorStatus = {0};

    Sensor_GetStatus(&SensorStatus);
    Sensor_GetStatus_Indicated(SENSOR_MODE_FULL, &CapSensorStatus);
    FlashParam.WidthMainFlash = SensorStatus.uiPclk * 1000; // 1ms
    if(AEAlg_GetPrvLV()<60)
        FlashParam.ISO_MainFlash = 200; // Modify ISO by LV
    else if(AEAlg_GetPrvLV()<70)
        FlashParam.ISO_MainFlash = 100; // Modify ISO by LV
    else if(AEAlg_GetPrvLV()<80)
        FlashParam.ISO_MainFlash = 50; // Modify ISO by LV
    FlashParam.ExpoTime_MainFlash = CapSensorStatus.pSyncInfo->HD_total*(CapSensorStatus.pSyncInfo->VD_total-1)/CapSensorStatus.uiPclk;
    FlashParam.DelayLineMainFlash = CapSensorStatus.pSyncInfo->VD_total-10;
    //#NT#2011/11/24#Ethan Lau -end
}


void flash_PreFlashFlow(void)
{
    debug_err(("flash_PreFlashFlow Sample\r\n"));
    aaa_AeLock(LOCK);    //Stop AE
    flash_Evaluate();   //Evaluate flash status according to menu setting
#if 1//(FLASH_LIGHT ==   _FLASH_SCR_)
    flash_PreFlashFlow_SCR();
#else
    flash_PreFlashFlow_IGBT();
#endif
    SetFlashFireStatus(FlashParam.Enable);
}

void Flash_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{

}

//#NT#2012/05/20#KS Hung -begin
UINT32 Flash_WriteDebugInfo(UINT32 Addr)
{
    UINT32 i, j;
    UINT32 FRg=0,FGg=0,FBg=0;

    Flash_GetColorGian(&FRg,&FGg,&FBg);
    
    Addr += sprintf((char *)Addr, "Flash RGain = %ld GGain = %ld BGain = %ld\r\n", FRg, FGg, FBg);
    Addr += sprintf((char *)Addr, "WidthMainFlash:%ld ISOMainFlash:%ld\r\n",FlashParam.WidthMainFlash,FlashParam.ISO_MainFlash);
    Addr += sprintf((char *)Addr, "Y_Effect_Back:%ld Y_Effect_Flash:%ld\r\n",FlashInputParam.FlashReturnInfo.BackLum,FlashInputParam.FlashReturnInfo.PreflashLum);
    Addr += sprintf((char *)Addr, "ColumnIndex:%ld RowIndex:%ld\r\n",FlashInputParam.FlashReturnInfo.ColumnIndex,FlashInputParam.FlashReturnInfo.RowIndex);
    for(j = 0; j < 8; j++)
    {
        Addr += sprintf((char *)Addr,"NoFlash Y  \t: ");

        for (i = 0; i < 8; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld|", NoFlash_G[i][j]);
        }

        Addr += sprintf((char *)Addr,"\r\n");
    }

    Addr += sprintf((char *)Addr,"\t");
    for(j = 0; j < 80; j++)
        Addr += sprintf((char *)Addr,"-");
    Addr += sprintf((char *)Addr,"\r\n");

    for(j = 0; j < 8; j++)
    {
        Addr += sprintf((char *)Addr,"Pre Y  \t: ");

        for (i = 0; i < 8; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld|",PreFlash_G[i][j]);
        }

        Addr += sprintf((char *)Addr,"\r\n");
}
    return Addr;
}

//#NT#2012/05/20#KS Hung -end
/********************************************************************************/


