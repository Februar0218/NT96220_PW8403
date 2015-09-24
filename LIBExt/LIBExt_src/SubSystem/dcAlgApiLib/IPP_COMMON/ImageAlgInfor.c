#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "ImageAlgInfor.h"
#include "IPPManager.h"
#include "sen_common.h"
#include "dzoom.h"
#include "exif.h"
//#NT#2011/06/14#Hideo Lin -begin
//#NT#For using new EXIF
#include "ExifRW.h"
//#NT#2011/06/14#Hideo Lin -end
#include "grph.h"
#include "dcout.h"
#include "IPPCtrl.h"
#include "alg_iplmode.h"
#include "sensorflowfunc.h"
#include "Pad.h"

ACTFBADDR_PRAM  FB_para1;
AlgMsgInfo gAlgMsg;
PHOTO_APPOBJ DispSizeInfo;
static UINT32 gIPPSizeInfor[Max_IPPSizeEnum];
volatile SEN_ISR_FP gFPIsrEventFunc[SEN_ISR_FP_NUM];

//UCHAR JPEG_Quality[3] = {95, 93, 80};
//#NT#2009/12/12#Niven Cho -begin
//#NT#BRC by FileCtrl Fail
UCHAR JPEG_Quality[_IMGQuality_MaxNum] = {96,93,85,50};
//#NT#2009/12/12#Niven Cho -end

//#NT#2009/12/08#Linkin Huang -begin
//#NT# used to test function GRAPH_Rearrange
//UINT32 fileBufferAddr = 0, TestData = 0;
//#NT#2009/12/08#Linkin Huang -end

//AlgCallback Function
FPALGCALLBACK g_fpAlgExport=NULL ;

ImageAlgInfo gImageAlgInfo=
{
_DSC_Auto,                  //    1 ,  UINT    DSCMode;            // Please reference enum:_DSCMode_sel
_SceneMode_Auto,            //    2 ,  UINT    SceneMode;          // Please reference enum:_IMGSceneMode_sel
_IMGSize_7M,                //    3 ,  UINT    IMGSize;            // Please reference enum:_IMGSize_sel
_IMGQuality_Fine,           //    4 ,  UINT    Quality;            // Please reference enum:_IMGQuality_sel
_IMGDriveMode_Single,       //    5 ,  UINT    DriveMode;          // Please reference enum:_IMGDriveMode_sel
_BurstNum_1,                //    6 ,  UINT    BurstDriveNum;      // Please reference enum:_IMGBurstNum_sel
_IMGSharp_Normal    ,       //    7 ,  UINT    Sharpness;          // Please reference enum:_IMGSharpness_sel
_IMGSaturation_Normal,      //    8 ,  UINT    Saturation;         // Please reference enum:_IMGEffect_sel
_IMGDZoomIdx_00,            //    9 ,  UINT    DZoomIndex;         // Please reference enum:_IMGDZoomIdx_sel
_IMGFlashMode_ForceOFF,     //    10,  UINT    FlashMode;          // Please reference enum:_IMGFlashMode_sel
_POWERFreq_60Hz,            //    11,  UINT    Frequency;          // Please reference enum:_POWERFreq_sel
_IMGEffect_Off,             //    12,  UINT    ImageEffect;        // Please reference enum:_IMGEffect_sel
_IMGAEMode_CenterWeighted,  //    13,  UINT    AEMode;             // Please reference enum:_IMGAEMode_sel
_IMGEVValue_00,             //    14,  UINT    AEEVValue;          // Please reference enum:_IMGEVValue_sel
_IMGWBMode_Auto,            //    15,  UINT    WBMode;             // Please reference enum:_IMGWBMode_sel
_IMGAFMode_Normal,          //    16,  UINT    AFMode;             // Please reference enum:_IMGAFMode_sel
_IMGAFWindow_Center,        //    17,  UINT    AFWindow;           // Please reference enum:_IMGAFWindow_sel
_IMGAFPosition_Center,      //    18,  UINT    AFPosition;         // Please reference enum:_IMGAFPosition_sel
_IMGISO_Auto,               //    19,  UINT    ISO;                // Please reference enum:_IMGISO_sel
_FileFmt_Jpg,               //    20,  UINT    FileFmt;            // Please reference enum:_IMGFileFmt_sel
_PostGamma,                 //    21,  UINT    SiePath;            // Please reference enum:_IMGSiePath_sel
_SieBitDepth_8,             //    22,  UINT    SieBitDepth;        // Please reference enum:_IMGSieBitDepth_sel
_SieBitDepth_10,            //    23,           SieInBitDepth
_SieBitDepth_8,             //    24,           SieCapBitDepth
_SieBitDepth_10,            //    25,           SieCapinBitDepth
_FaceDetectionMode_Off,     //    28,  UINT    FaceDetectionMode;  // Please reference enum:_IMGFaceDetectionMode_sel
_SmileDetectionMode_Off,    //    29,  UINT    SmileDetectionMode; // Please reference enum:_IMGSmileDetectionMode_sel
_BlinkDetectionMode_Off,    //    30,  UINT    BlinkDetectionMode; // Please reference enum:_IMGSceneMode_sel
_DISMode_Off,               //    31,  UINT    DISMode;            // Please reference enum:_IMGDISMode_sel
_Ratio_4_3,                 //    32,  UINT    ImageRatio;         // Please reference enum:_IMGRatio_sel
_Ratio_4_3,                 //    33,  UINT    DispRatio;          // Please reference enum:_IMGRatio_sel
_Flip_Off,                  //    34,  UINT    ImageFlip;          // Please reference enum:_IMGFlip_sel
_CaptureMode_FixQ,          //    35,  UINT    ReEncMechanism;     // Please reference enum:_IMG_CaptureMode_sel
ENABLE,                     //    36,  UINT    SegmentModeEn       Enable/Disable Filesystem segment write
_Resolution_640x480,        //    37   UINT    Prv_Resolution;     Please reference enum:_IMGResolution_sel
_Resolution_640x480,        //    38   UINT    Vid_Resolution;     Please reference enum:_IMGResolution_sel
_Level0,                    //    39   UINT    AntiShakeLevel      plz reference enum:_IMGStillAntiShaking
_ERS,                       //    40   UINT    Exposuremode_UI;    Plz    reference enum:_IMGExposureMode_Sel
_IMGSize_12M,               //    41   UINT    MaxJpgSize;         Please reference enum:_IMGSize_sel
DISABLE,                    //    44   UINT    RamDskEn;                                 _IMGRamDskEn_Sel
0x00,                       //    45   UINT    RamDskAddr;
0x00,                       //    46   UINT    RamDskSize;
_IMGSize_5M,                //    47   UINT    SensorSize;                               _IMGSize_sel
//#NT#2011/08/22#Bowen Li -begin
//#NT#add image filter
_IMGFilter_Off,              //    48   UINT    ImgFilter;                                _IMGFilter_sel
//#NT#2011/08/22#Bowen Li -end
_WindowFitAuto,
};
//#NT#2008/10/14#Yuppie Lin -end


BOOL TAM_mode=FALSE; // TV AV Manual mode
UINT        gADC_Value=0;
unsigned char AEB_Idx=0;
unsigned short JPGIMG_SIZE[Max_ImgSize+1][2]=
{
    //{4640,3480}, // 16M
    {4608,3456}, // 16M
    {4480,3360}, // 15M
    {4416,3312}, // 14M
    //{4320,3240}, // 14M
    {4032,3024}, // 12.2M
    {4032,3024},//{4000,3000}, // 12M
    {3648,2736}, // 10M
    {3456,2592}, // 9M
    {3264,2448}, // 8M
    {3648,2048}, // 7M_16_9
    {3072,2304}, // 7M
    {2848,2136}, // 6M
    {2688,2016}, // 5.4M
    {2592,1944}, // 5M
    {2560,1920}, // 4.9M
    {2432,1824}, // 4.4M
    {2272,1704}, // 4M
    {2240,1680}, // 3.8M
    {2048,1536}, // 3M
    {1856,1392}, // 2.6M
    {1664,1248}, // 2.1M
    {1920,1080}, // 2M_16_9
    {1600,1200}, // 2M
    {1408,1056}, // 1.5M
    {1280,960},  // 1M
    {1088,816},  // 0.8M
    {896,672},   // 0.6M
    {768,576},   // 0.4M
    {640,480},   // VGA
    {576,432},   // 0.2M
    {1920,1080}  // 16:9
};

void SetJpgResolution(_IMGSize_sel SizeIdx, UINT32 Hsize, UINT32 Vsize)
{
    if(SizeIdx > Max_ImgSize)
    {
        debug_err(("JPEG resolution setting error!!\r\n"));
    }
    else
    {
        JPGIMG_SIZE[SizeIdx][0] = Hsize;
        JPGIMG_SIZE[SizeIdx][1] = Vsize;
    }
}

void ClearAEB_Idx(void)
{
    AEB_Idx = 0;
}

void SetAWB(unsigned char WB)
{
#if 0
    aaa_AwbLock(1);
    //sie_waitVD(4);
    if(WB ==_IMGWBMode_Auto)
        aaa_AwbLock(0);
    else
        aaa_MWBprocess(WB);
#endif
}

#if 0 //Useless
//#NT#2007/03/05#Bowen Li -begin
//fix typing error
SceneModeInfo SceneModeSet;

void GetSceneParam(UB mode, UB* Flashmode,UB* EVcomp,UB* WBmode,UB* Saturation,UB* Sharpness)
{
    UB i;

    for(i=0;i<Max_SceneMode;i++)
    {
        if(mode == gSceneModeInfo[i].SceneMode)
            break;
    }

    *(UB*)Flashmode=gSceneModeInfo[i].FlashMode;
    *(UB*)EVcomp=gSceneModeInfo[i].EVComp;
    *(UB*)WBmode=gSceneModeInfo[i].WBMode;
    *(UB*)Saturation=gSceneModeInfo[i].Saturation;
    *(UB*)Sharpness=gSceneModeInfo[i].Sharpness;
}
//#NT#2007/03/05#Bowen Li -end
#endif

ImageAlgInfo* UI_GetParameter(void)
{
    return &gImageAlgInfo;
}

//ethan_20110807_chg value type for ramdisk addr and size
INT32 UI_SetParameter(UB item, UINT32 set_value)
{
    SEN_MSG("Item=%d\t value=%d\n\r",(UINT)item,(UINT)set_value);

    switch(item)
    {
        case _DSCMode: // 0
            if(set_value>Max_DSCMode)
                return E_PAR;
            gImageAlgInfo.DSCMode=set_value;
            if(gImageAlgInfo.DSCMode==_DSC_Auto)
            {
                gImageAlgInfo.WBMode=_IMGWBMode_Auto;

            }
            break;
        case _SceneMode: // 1
            if(set_value>Max_SceneMode)
                return E_PAR;
            gImageAlgInfo.SceneMode=set_value;
            break;
        case _ImgSize: // 2
            if(set_value>Max_ImgSize)
                return E_PAR;
            gImageAlgInfo.IMGSize=set_value;
            SetImageOutSize(JPGIMG_SIZE[gImageAlgInfo.IMGSize][0],JPGIMG_SIZE[gImageAlgInfo.IMGSize][1]);
            break;
        case _Quality: // 3
            if(set_value>Max_ImgQuality)
                return E_PAR;
            gImageAlgInfo.Quality=set_value;
            break;
        case _FlashMode: // 4
            if(set_value>Max_FlashMode)
                return E_PAR;
            gImageAlgInfo.FlashMode=set_value;
            //flash_Evaluate();
            break;
        case _ImageEffect: // 5
            if(set_value>Max_ImgEffect)
                return E_PAR;
            gImageAlgInfo.ImageEffect=set_value;
            SenFlowFunc(IPE_COLOREFFECT);
            break;
        case _AEMode: // 6
            if(set_value>Max_AeMode)
                return E_PAR;
            gImageAlgInfo.AEMode=set_value;
            break;
        case _AEEV: // 7
            if(set_value>Max_EvValue)
                return E_PAR;
            gImageAlgInfo.AEEVValue=set_value;
            break;
        case _WBMode:  // 8
            if(set_value>Max_WBMode)
                return E_PAR;
            gImageAlgInfo.WBMode=set_value;
            SetAWB(gImageAlgInfo.WBMode);
            break;
        case _AFMode: // 9
            if(set_value>Max_AfMode)
                return E_PAR;
            gImageAlgInfo.AFMode=set_value;
            break;
        case _AFWindow: // 10
            if(set_value>Max_AFWindow)
                return E_PAR;
            gImageAlgInfo.AFWindow=set_value;
            break;
        case _Frequency:    //ethan_20110725_add
            if(set_value>Max_FrequencyMode)
                return E_PAR;
            gImageAlgInfo.Frequency = set_value;
            break;
        case _AFPosition: // 11
            if(set_value>Max_AFPosition)
                return E_PAR;
            gImageAlgInfo.AFPosition=set_value;
            break;
        case _ISOIndex: // 12
            if(set_value > Max_ISO)
                return E_PAR;
            gImageAlgInfo.ISO=set_value;
            break;
        case _DZoomIndex: // 13
            gImageAlgInfo.DZoomIndex=set_value;
            SetDzoom2Preview(gImageAlgInfo.DZoomIndex);
            break;
      case _BurstNum:  // 14
          gImageAlgInfo.BurstDriveNum=set_value;
          break;
        case _DriveMode:  // 16
            if(set_value>Max_DriveMode)
                return E_PAR;
            gImageAlgInfo.DriveMode=set_value;
            break;
        case _Sharpness:  // 17
            if(set_value>Max_Sharp)
                return E_PAR;
            gImageAlgInfo.Sharpness=set_value;
            SenFlowFunc(IPE_SHARPNESS);
            break;
        case _Saturation:  // 18
            if(set_value>Max_Saturation)
                return E_PAR;
            gImageAlgInfo.Saturation=set_value;
            SenFlowFunc(IPE_SATURATION);
            break;
        case _FileFmt:  // x
            gImageAlgInfo.FileFmt=set_value;
            break;
        case _SieBitDepth:  // x
            gImageAlgInfo.SieBitDepth=set_value;
            break;
        case _SieInBitDepth:
            gImageAlgInfo.SieInBitDepth = set_value;
            break;
        case _SieCapBitDepth:
            gImageAlgInfo.SieCapBitDepth = set_value;
            break;
        case _SieCapInBitDepth:
            gImageAlgInfo.SieCapInBitDepth = set_value;
            break;
        case _SiePath:  // x
            gImageAlgInfo.SiePath=set_value;
            break;
        case _FaceDetectionMode:
            if(set_value > Max_FaceDetectionMode)
                return E_PAR;
            gImageAlgInfo.FaceDetectionMode = set_value;
            break;
        case _SmileDetectionMode:
            if(set_value > Max_SmileDetectionMode)
                return E_PAR;
            gImageAlgInfo.SmileDetectionMode = set_value;
            break;
        case _BlinkDetectionMode:
            if(set_value > Max_BlinkDetectionMode)
                return E_PAR;
            gImageAlgInfo.BlinkDetectionMode = set_value;
            break;
        case _DISMode:
            if(set_value > Max_DISMode)
                return E_PAR;
            gImageAlgInfo.DISMode = set_value;
            break;
        case _ImageRatio:
            if (set_value > _Ratio_Num)
                return E_PAR;
            if(set_value != gImageAlgInfo.ImageRatio)
            {
                UINT32 tmp;
                tmp = gImageAlgInfo.ImageRatio;
                gImageAlgInfo.ImageRatio = set_value;
                if(ChgImgRatio() != E_OK)
                {
                    debug_err(("^R chg image ratio fail\r\n"));
                    gImageAlgInfo.ImageRatio = tmp;
                }
            }
            break;
        case _DispRatio:
            if (set_value > _Ratio_Num)
                return E_PAR;
            if(IPL_GetMode() != IPL_MODE_OFF)
            {
                debug_err(("^R must close ipp before chg Dispratio !\r\n"));
                return E_PAR;
            }
            gImageAlgInfo.DispRatio = set_value;
            break;
        case _ImageFlip:
            if (set_value > _Flip_Max_type)
                return E_PAR;
            //#NT#2011/12/13#Ethan Lau -begin
            if( (set_value<= _DSP_Flip_H_V)&&(set_value >=_DSP_Flip_H))
            {
                debug_err(("^R 220 do not support image flip(chip func.) under slice mode, plz use sen flip\r\n"));
            }
            //#NT#2011/12/13#Ethan Lau -end
            //use sensor flip
            if( (set_value >= _SEN_Flip_H)||(set_value == _Flip_Off) )
            {
                if(set_value != gImageAlgInfo.ImageFlip)
                {
                    UINT32 tmp;
                    //map to enum:SEN_FLIP
                    if(set_value >= _SEN_Flip_H)
                    {
                        tmp = set_value - _SEN_Flip_H;
                        tmp ++;
                    }
                    else
                        tmp = set_value;    //tmp = _Flip_Off
                    if(ChgSensorFlip(tmp)!= E_OK)
                    {
                        debug_err(("^R chg sensor flip fail\r\n"));
                    }
                }
            }
            gImageAlgInfo.ImageFlip = set_value;
            break;
        case _ReEncMechanism:
            gImageAlgInfo.ReEncMechanism=set_value;
            break;
//#NT#2009/10/09#Bowen Li -begin
//#NT# add segment write switch
        case _SegmentModeEn:
            gImageAlgInfo.SegmentModeEn = set_value;
            break;
//#NT#2009/10/09#Bowen Li -end
        case _Exposuremode_UI:
            if(set_value>=_MaxExposureMode)
                return E_PAR;
            gImageAlgInfo.Exposuremode_UI = set_value;
            break;
        case _MaxJpgSize:
            if(set_value>Max_ImgSize)
                return E_PAR;
            gImageAlgInfo.MaxJpgSize = set_value;
            break;
        case _RamDskEn:
            if(set_value > ON)
                return E_PAR;
            gImageAlgInfo.RamDskEn = set_value;
            break;
        case _RamDskSize:
            //if(set_value > Max_ImgSize)
            //    return E_PAR;
            //ethan_20110808_ramdisksize is bitstream size from projlayer
            gImageAlgInfo.RamDskSize = set_value;//JPGIMG_SIZE[set_value][0]*JPGIMG_SIZE[set_value][1];
            break;
        case _RamDskAddr:
            gImageAlgInfo.RamDskAddr = set_value;
            break;
        case _Prv_Resolution:
            if(set_value>_Resolution_Max)
                return E_PAR;
            gImageAlgInfo.Prv_Resolution = set_value;
            break;
        case _Vid_Resolution:
            if(set_value>_Resolution_Max)
                return E_PAR;
            gImageAlgInfo.Vid_Resolution = set_value;
            break;
        case _StillAntiShaking:
            if(set_value>_Level_Max)
                return E_PAR;
            gImageAlgInfo.AntiShakeLevel = set_value;
            break;
        case _SensorSize:
            if(set_value>Max_ImgSize)
                return E_PAR;
            gImageAlgInfo.SensorSize = set_value;
            break;
//#NT#2011/08/22#Bowen Li -begin
//#NT#add image filter
        case _ImgFilter:
            if(set_value>_IMGFilter_Max)
                return E_PAR;
            gImageAlgInfo.ImageFilter = set_value;
            break;
//#NT#2011/08/22#Bowen Li -end
        case _DsiplayFitWindow:
            if(set_value>_WindowFitMax)
                return E_PAR;
            gImageAlgInfo.DsiplayFitWindow = set_value;
            break;
        default:
            return E_PAR;
    }
    return E_OK;
}

void UI_SetAllParam(void)
{
    //UI_SetParameter(_SceneMode, gImageAlgInfo.SceneMode);
    UI_SetParameter(_ImgSize,gImageAlgInfo.IMGSize);
    UI_SetParameter(_Quality,gImageAlgInfo.Quality);
    UI_SetParameter(_ImageEffect,gImageAlgInfo.ImageEffect);
    UI_SetParameter(_AEMode,gImageAlgInfo.AEMode);
    UI_SetParameter(_FlashMode,gImageAlgInfo.FlashMode);
    UI_SetParameter(_AEEV,gImageAlgInfo.AEEVValue);
    UI_SetParameter(_WBMode,gImageAlgInfo.WBMode);
    UI_SetParameter(_ISOIndex,gImageAlgInfo.ISO);
    UI_SetParameter(_Sharpness,gImageAlgInfo.Sharpness);
    UI_SetParameter(_Saturation,gImageAlgInfo.Saturation);
    UI_SetParameter(_AFMode,gImageAlgInfo.AFMode);
    UI_SetParameter(_AFWindow,gImageAlgInfo.AFWindow);
    UI_SetParameter(_AFPosition,gImageAlgInfo.AFPosition);
}
void Write_EXIF_Infor(UINT paddress)
{
//#NT#2011/06/14#Hideo Lin -begin
//#NT#Modify to use new EXIF
#if 0 // old EXIF method

    UH Flash_Mode;
    JPEGWriteAPP1ADC(paddress , gADC_Value);
    Flash_Mode=0;
    Write_AE_EXIF_Infor(paddress);
    //AEMode
    switch(gImageAlgInfo.AEMode)
    {
        case _IMGAEMode_CenterWeighted:
            JPEGWriteAPP1MeteringMode(paddress,MeterCenterWeightedAvg);
        break;
        case _IMGAEMode_Spot:
            JPEGWriteAPP1MeteringMode(paddress,MeterSpot);
        break;
        case _IMGAEMode_Matrix:
            JPEGWriteAPP1MeteringMode(paddress,MeterPattern);
        break;
        case _IMGAEMode_Evaluative:
            JPEGWriteAPP1MeteringMode(paddress,MeterPattern);
        break;
    }
    switch(gImageAlgInfo.WBMode)
    {
        case _IMGWBMode_Auto:
            JPEGWriteAPP1LightSource(paddress, LightUnknown);
        break;
        case _IMGWBMode_Daylight:
            JPEGWriteAPP1LightSource(paddress, LightDaylight);
        break;
        case _IMGWBMode_Cloudy :
            JPEGWriteAPP1LightSource(paddress, 10);
        break;
        case _IMGWBMode_Tungsten:
            JPEGWriteAPP1LightSource(paddress, LightTungsten);
        break;
        case _IMGWBMode_Fluorescent1:
            JPEGWriteAPP1LightSource(paddress, LightFluorescent);
        break;
        default:
            JPEGWriteAPP1LightSource(paddress, LightUnknown);
        break;
    }
    //WhiteBalance
    if(gImageAlgInfo.WBMode == _IMGWBMode_Auto)
    {
        JPEGWriteAPP1WhiteBalance(paddress, AutoWhite);
    }
    else
    {
        JPEGWriteAPP1WhiteBalance(paddress, ManualWhite);
    }
    debug_ind(("gImageAlgInfo.FlashMode=%d\r\n",gImageAlgInfo.FlashMode));
    //Flashlight
    switch(gImageAlgInfo.FlashMode)
    {
        case _IMGFlashMode_Auto:
            Flash_Mode=(1<<4)+(1<<3);
        break;
        case _IMGFlashMode_ForceOFF:
            Flash_Mode=(1<<4);
        break;
        case _IMGFlashMode_ForceON:
            Flash_Mode=(1<<3);
        break;
        case _IMGFlashMode_AutoRedEye:
            Flash_Mode=(1<<6)+(1<<4)+(1<<3);
        break;
        case _IMGFlashMode_ForceONRedEye:
            Flash_Mode=(1<<6)+(1<<3);
        break;
        case _IMGFlashMode_NightStrobe:
            Flash_Mode=(1<<6)+(1<<4)+(1<<3);
        break;
        default:
            Flash_Mode=(1<<4)+(1<<3);
        break;
    }
    if(CheckFlashFireStatus())
        JPEGWriteAPP1StrobeFlash(paddress,StrobeFired+Flash_Mode);
    else
        JPEGWriteAPP1StrobeFlash(paddress,StrobeNoFired+Flash_Mode);
    //AEB
    if(gImageAlgInfo.DriveMode == _IMGAEBreaketing_ON)
        JPEGWriteAPP1ExposureMode(paddress,AutoBracket);
    else
        JPEGWriteAPP1ExposureMode(paddress,AutoExposure);
    //ExposureProgram
    if(gImageAlgInfo.SceneMode==_SceneMode_Auto)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgNormal);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Manual)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgManual);
    else if(gImageAlgInfo.SceneMode==_SceneMode_AV)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgAperture);
    else if(gImageAlgInfo.SceneMode==_SceneMode_TV)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgShutter);
    else if((gImageAlgInfo.SceneMode==_SceneMode_Party)||(gImageAlgInfo.SceneMode==_SceneMode_SunSet))
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgCreative);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Sport)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgAction);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Potrait)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgPortrait);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Landscape)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgLandscape);
    else
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgNoDef);
    //SceneCaptureType
    if(gImageAlgInfo.SceneMode==_SceneMode_Landscape)
        JPEGWriteAPP1SceneCaptureType(paddress,LandScapeScene);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Potrait)
        JPEGWriteAPP1SceneCaptureType(paddress,PortraitScene);
    else if((gImageAlgInfo.SceneMode==_SceneMode_NightScene)||(gImageAlgInfo.SceneMode==_SceneMode_NightPotrait)||(gImageAlgInfo.SceneMode==_SceneMode_NightLandscape))
        JPEGWriteAPP1SceneCaptureType(paddress,NightScene);
    else
        JPEGWriteAPP1SceneCaptureType(paddress,StandardScene);
    //ImageEffect(Normal/Sepia/BW)
    if(gImageAlgInfo.ImageEffect ==_IMGEffect_Off)
        JPEGWriteAPP1CustomRendered(paddress,NormalRendered);
    else
        JPEGWriteAPP1CustomRendered(paddress,CustomRendered);
    //Quality
    if(gImageAlgInfo.Quality ==_IMGQuality_Best)
        JPEGWriteJPGQulaity(paddress, Quality_SuperHigh);
    else if(gImageAlgInfo.Quality ==_IMGQuality_Fine)
        JPEGWriteJPGQulaity(paddress, Quality_High);
    else if(gImageAlgInfo.Quality ==_IMGQuality_Normal)
        JPEGWriteJPGQulaity(paddress, Quality_Economy);
    else if(gImageAlgInfo.Quality ==_IMGQuality_Economy)
        JPEGWriteJPGQulaity(paddress, Quality_Undefine);
    else
        JPEGWriteJPGQulaity(paddress, Quality_Undefine);

#else // new EXIF method

    UH Flash_Mode = 0;
    UINT32 value = 0;
    EXIF_MODIFYDATATAG modifyDataTag;
    //EXIF_MODIFYPTRTAG modifyPtrTag;

    Write_AE_EXIF_Infor(paddress);

    //AEMode
    switch(gImageAlgInfo.AEMode)
    {
    case _IMGAEMode_CenterWeighted:
        value = MeterCenterWeightedAvg;
        break;

    case _IMGAEMode_Spot:
        value = MeterSpot;
        break;

    case _IMGAEMode_Matrix:
        value = MeterPattern;
        break;

    case _IMGAEMode_Evaluative:
        value = MeterPattern;
        break;
    }
    modifyDataTag.tagId = TagMeteringMode;
    modifyDataTag.tagDataValue = value;
    modifyDataTag.tagLen = TagMeteringLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //Lightsource
    if(CheckFlashFireStatus())
    {
        value = LightFlash;
    }
    else
    {
        switch(gImageAlgInfo.WBMode)
        {
        case _IMGWBMode_Auto:
            value = LightUnknown;
            break;

        case _IMGWBMode_Daylight:
            value = LightDaylight;
            break;

        case _IMGWBMode_Cloudy:
            value = LightCloudy;
            break;

        case _IMGWBMode_Tungsten:
            value = LightTungsten;
            break;

        case _IMGWBMode_Fluorescent1:
            value = LightFluorescent;
            break;

        default:
            value = LightUnknown;
            break;
        }
    }
    modifyDataTag.tagId = TagLightSource;
    modifyDataTag.tagDataValue = value;
    modifyDataTag.tagLen = TagLightSrcLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //WhiteBalance
    if(gImageAlgInfo.WBMode == _IMGWBMode_Auto)
    {
        value = AutoWhite;
    }
    else
    {
        value = ManualWhite;
    }
    modifyDataTag.tagId = TagWhiteBalance;
    modifyDataTag.tagDataValue= value;
    modifyDataTag.tagLen = TagWhiteBNLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //Flashlight
    debug_ind(("gImageAlgInfo.FlashMode=%d\r\n",gImageAlgInfo.FlashMode));
    switch(gImageAlgInfo.FlashMode)
    {
    case _IMGFlashMode_Auto:
        Flash_Mode = StrobeModeAuto;
        break;

    case _IMGFlashMode_ForceOFF:
        Flash_Mode = StrobeForceOff;
        break;

    case _IMGFlashMode_ForceON:
        Flash_Mode = StrobeForceOn;
        break;

    case _IMGFlashMode_AutoRedEye:
        Flash_Mode = (StrobeModeAuto | StrobeRedEyeOn);
        break;

    case _IMGFlashMode_ForceONRedEye:
        Flash_Mode = (StrobeForceOn | StrobeRedEyeOn);
        break;

    case _IMGFlashMode_NightStrobe:
        Flash_Mode = (StrobeModeAuto | StrobeRedEyeOn);
        break;

    default:
        Flash_Mode = StrobeModeAuto;
        break;
    }
    if(CheckFlashFireStatus())
    {
        value = (StrobeFired | Flash_Mode);
    }
    else
    {
        value = (StrobeNoFired | Flash_Mode);
    }
    modifyDataTag.tagId = TagStrobeFlash;
    modifyDataTag.tagDataValue= value;
    modifyDataTag.tagLen = TagStrobeLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //AEB
    if(gImageAlgInfo.DriveMode == _IMGAEBreaketing_ON)
    {
        value = AutoBracket;
    }
    else
    {
        value = AutoExposure;
    }
    modifyDataTag.tagId = TagExposureMode;
    modifyDataTag.tagDataValue= value;
    modifyDataTag.tagLen = TagExpModeLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //ExposureProgram
    switch (gImageAlgInfo.SceneMode)
    {
    case _SceneMode_Auto:
        value = ExpPrgNormal;
        break;

    case _SceneMode_Manual:
        value = ExpPrgManual;
        break;

    case _SceneMode_AV:
        value = ExpPrgAperture;
        break;

    case _SceneMode_TV:
        value = ExpPrgShutter;
        break;

    case _SceneMode_Party:
    case _SceneMode_SunSet:
        value = ExpPrgCreative;
        break;

    case _SceneMode_Sport:
        value = ExpPrgAction;
        break;

    case _SceneMode_Potrait:
        value = ExpPrgPortrait;
        break;

    case _SceneMode_Landscape:
        value = ExpPrgLandscape;
        break;

    default:
        value = ExpPrgNoDef;
        break;
    }
    modifyDataTag.tagId = TagExposureProgram;
    modifyDataTag.tagDataValue= value;
    modifyDataTag.tagLen = TagExpProgLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //SceneCaptureType
    switch (gImageAlgInfo.SceneMode)
    {
    case _SceneMode_Landscape:
        value = LandScapeScene;
        break;

    case _SceneMode_Potrait:
        value = PortraitScene;
        break;

    case _SceneMode_NightScene:
    case _SceneMode_NightPotrait:
    case _SceneMode_NightLandscape:
        value = NightScene;
        break;

    default:
        value = StandardScene;
        break;
    }
    modifyDataTag.tagId = TagSceneCaptureType;
    modifyDataTag.tagDataValue = value;
    modifyDataTag.tagLen = TagSNECaptLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //ImageEffect(Normal/Sepia/BW)
    if(gImageAlgInfo.ImageEffect ==_IMGEffect_Off)
    {
        value = NormalRendered;
    }
    else
    {
        value = CustomRendered;
    }
    modifyDataTag.tagId = TagCustomRendered;
    modifyDataTag.tagDataValue = value;
    modifyDataTag.tagLen = TagCMRenderLen;
    ExifW_ModifyDataTag(&modifyDataTag);

    //Quality
    switch (gImageAlgInfo.Quality)
    {
    case _IMGQuality_Best:
        value = Quality_SuperHigh;
        break;

    case _IMGQuality_Fine:
        value = Quality_High;
        break;

    case _IMGQuality_Normal:
        value = Quality_Economy;
        break;

    case _IMGQuality_Economy:
    default:
        value = Quality_Undefine;
        break;
    }
    ExifW_HideQuality(value);

#endif
//#NT#2011/06/14#Hideo Lin -end

    gAlgMsg.JpgExifAddr=paddress;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_JPGEXIFMODIFY, &gAlgMsg);
    }
}

/**
    Get current AF Window Position
    @param  Focus_Window Structure for AF window stx,sty,hsize,vsize
*/
void Get_AF_Window_Position(unsigned char af_pos,FOCUS_WINDOW_INFOR *Focus_Window)//not finish yet
{
    //STR_VA_INFOR *VA_infor;
    unsigned long stx,sty,hsz,vsz;

    //ipe_GetVA_Infor(VA_infor);
    stx = 320/5;
    sty = 240/5;
    hsz = 320/5;
    vsz = 240/5;

    if(af_pos ==_IMGAFPosition_1)
    {
        Focus_Window->stx = stx+hsz;
        Focus_Window->sty = sty;
    }
    else if(af_pos ==_IMGAFPosition_2)
    {
        Focus_Window->stx = stx;
        Focus_Window->sty = sty+vsz;
    }
    else if(af_pos ==_IMGAFPosition_3)
    {
        Focus_Window->stx = stx+2*hsz;
        Focus_Window->sty = sty+vsz;
    }
    else if(af_pos ==_IMGAFPosition_4)
    {
        Focus_Window->stx = stx+hsz;
        Focus_Window->sty = sty+2*vsz;
    }
    else
    {
        Focus_Window->stx = stx+hsz;
        Focus_Window->sty = sty+vsz;
    }
    Focus_Window->hsz = hsz;
    Focus_Window->vsz = vsz;

}

UCHAR Get_CaptureDriverMode(void)
{

    return gImageAlgInfo.DriveMode;
}

void def_AlgCallBackFun(FPALGCALLBACK pAlgcallback)
{
    g_fpAlgExport = pAlgcallback;
}

ER Sen_IsrRegisterEvent(SEN_ISR_FPID IsrId, SEN_ISR_FP EventFp)
{
    if (IsrId > SEN_ISR_FP_NUM)
    {
        return E_SYS;
    }
    gFPIsrEventFunc[IsrId] = EventFp;
    return E_OK;
}

void Sen_IsrEventProc(SEN_ISR_FPID IsrId, UINT32 uiEventStatus)
{
    if (gFPIsrEventFunc[IsrId] != NULL)
    {
        gFPIsrEventFunc[IsrId](uiEventStatus);
    }
}

void Sen_SetPhotoInfo(PHOTO_APPOBJ *PhotoObj)
{
    DispSizeInfo = *PhotoObj;
}

UINT32 GetDisplayH(void)
{
    return DispSizeInfo.uiDisplayFBWidth;//DisplayH;
}
UINT32 GetDisplayV(void)
{
    return DispSizeInfo.uiDisplayFBHeight;//DisplayV;
}

UINT SetIPPSizeInfor(UINT item, UINT value)
{
    UINT *pArray1;

    pArray1=(UINT *) &gIPPSizeInfor;
    *(pArray1+item)=value;
    return E_OK;
}
UINT GetIPPSizeInfor(UINT    item)
{
    UINT value;
    UINT *pArray1;

    pArray1=(UINT *) &gIPPSizeInfor;
    value=    *(pArray1+item);
    return value;
}

IME_FBADDR_PRAM* Sen_GetImePath1Param(void)
{
    return &IME_D1_Para;
}

IME_FBADDR_PRAM* Sen_GetImePath2Param(void)
{
    return &IME_D2_Para;
}

//#NT#2010/12/21#ethanlau -begin
//#NT#tmp for compile ok
UINT32 gAeIso=0, gAeExpoTime=0;
void Sen_SetAeCapIso(UINT32 Iso)
{
    gAeIso = Iso;
}

UINT32 Sen_GetAeCapIso(void)
{
    return gAeIso;
}
void Sen_SetAeCapExpoTime(UINT32 ExpoTime)
{
    gAeExpoTime = ExpoTime;
}

UINT32 Sen_GetAeCapExpoTime(void)
{
    return gAeExpoTime;
}
//#NT#2010/12/21#ethanlau -end

//#NT#2011/02/11#ethanlau -begin
//#NT#return IPL in which stop stage
UINT32 Sen_GetIPLStopStatus(void)
{
    return GetIPLStopStatus();
}
//#NT#2011/02/11#Ethan Lau -end

BOOL g_bawbinfo = FALSE;
void Sen_SetColorGain(UINT32 Rg,UINT32 Bg,UINT32 Gg)
{
    SetColorGain(Rg,Bg,Gg);
    if(g_bawbinfo)
    {
        debug_msg("colorgain = %d %d %d\r\n",Rg,Gg,Bg);
    }
}

void Sen_GetSubOutRGBinfo(UINT32* R,UINT32* G,UINT32* B,UINT32 uiSize)
{
    //UINT32 i=0;
    GetSubOutRGBinfo(R,G,B,uiSize);
}

//Cfa = ,0=>R,1=>Gr,2=>Gb,3=>B
//only support streaming image
void Sen_GetSubOutImgInfo(UINT32* ImgH,UINT32* ImgLinoft,UINT32* ImgV,UINT32* Addr,UINT32* Cfa)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    *ImgH = GetIPPSizeInfor(_CAHsize);
    *ImgV = GetIPPSizeInfor(_CAVsize);
    *ImgLinoft = GetIPPSizeInfor(_CALineOffset);
    *Addr = ippbuf_GetBuf(IPPBUF_CA);
    *Cfa = SIE_PRE_Para_prv.SieCfaPat;
}

//#NT#2011/03/23#Harry Tsai -begin
//#NT#move from flash.c
STR_SIE_FLASH_PARAM FlashControl;
BOOL FlashFired = FALSE;
//#NT#2011/03/23#Harry Tsai -end
//#NT#2011/03/23#Harry Tsai -begin
//#NT#move from flash.c
//flash hardware pin select
#include "Top.h"
void flash_CtrlPinSelect(void)
{
    //removed, all pimux-select are ctrled by proj layer
    debug_err(("^R pimmux api removed\r\n"));
    //pinmux_select_flctr(PINMUX_FLCTR);
    pad_setDrivingSink(PAD_DS_SN_FLCTR, PAD_DRIVINGSINK_16MA);
}

//pre-flash setting
void flash_ConfigPreFlash(UINT16 uiDelayLine, UINT32 uiOnTimeUs,BOOL uiFlashIMD)
{
    FlashControl.LineDelay = uiDelayLine;
    FlashControl.FlashWidth = uiOnTimeUs;
    FlashControl.FlashImd = uiFlashIMD;
    sie_setFlashParam(FlashControl);
}

//main-flash setting
void flash_ConfigMainFlash(UINT32 uiDelayLine, UINT32 uiOnTimeUs,BOOL uiFlashIMD)
{
    FlashControl.LineDelay = uiDelayLine;
    FlashControl.FlashWidth = uiOnTimeUs;
    FlashControl.FlashImd = uiFlashIMD;
    sie_setFlashParam(FlashControl);
}

void flash_TriggerFlash(void)
{
    flash_CtrlPinSelect();
    sie_setFlashStart();//sie_FlashStart();
}


void SetFlashFireStatus(BOOL Fire)
{
    FlashFired = Fire;
}

BOOL CheckFlashFireStatus(void)
{
    return FlashFired;
}
//#NT#2011/03/23#Harry Tsai -end


//#NT#2011/06/14#Ethan Lau -begin
//#NT#Only add for system clk chg,EVB Test only
void IPP_PauseEng_SysclkTest(void)
{
    debug_msg("^G %s\r\n",__func__);
    StopPreviewDramData(NULL);
}

void IPP_StartEng_SysclkTest(void)
{
    debug_msg("^G %s\r\n",__func__);
    StartPreviewDramData();
}
//#NT#2011/06/14#Ethan Lau -end
