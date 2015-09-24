#include    <string.h>
#include    <stdlib.h>
#include    <stdio.h>
#include    "kernel.h"
#include    "sensor.h"
#include    "debug.h"
#include    "utility.h"
#include    "Calibration.h"
#include    "CalibrationInt.h"
#include    "Cal_MI5130_220.h"
#include    "Syscfg.h"
#include    "Cache.h"
#include    "KeyScanTsk.h"
#include    "UIResource.h"
#include    "CalibrationTsk.h"
#include    "phototsk.h"
#if (SENSOR_LSC_ENABLE==ENABLE)
#include    "liblcmobile.h"
#endif
#include    "filesystsk.h"


ULONG gLCSRegIdx = 0;
ULONG gLCSRegData[256];
INT8 CalStringBuffer[32];

/**
  AAA_Adjust structure is flexible by project,
  so its prototype is in different folder instead
  in common file
*/
volatile Adjust_Struct  AAA_Adjust;

#define ID_LCS_CMD_INIT 0
#define ID_LCS_CMD_ADD  1
#define ID_LCS_CMD_END  2
#define ID_LCS_CMD_AE   3

static ER CheckAwbGS(void)
{
    ER ercode = 0;
    UW size=7;
    UB addr[7];
    int fsStatus;
    char filename[64];
    sprintf(filename, "A:\\AWBGS.txt");
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    fsStatus = FilesysReadWriteByName2(
                    FST_CMD_READ_BY_NAME,
                    filename,
                    (UINT8*)addr,
                    &size,
                    0,
                    FST_TIME_INFINITE);
    debug_msg("%x %x %x %x %x %x\r\n",addr[0],addr[1],addr[2],addr[4],addr[5],addr[6]);
    if(fsStatus == FST_STA_OK)
    {
        AAA_Adjust.AWBGS.Rgain=(addr[0]-0x30)*100+(addr[1]-0x30)*10+(addr[2]-0x30);
        AAA_Adjust.AWBGS.Bgain=(addr[4]-0x30)*100+(addr[5]-0x30)*10+(addr[6]-0x30);
        AAA_Adjust.AWBGS_Status = _AWB_Status;

        sprintf(CalStringBuffer, "AWBGS: %d %d",AAA_Adjust.AWBGS.Rgain,AAA_Adjust.AWBGS.Bgain);
        Cal_ShowStringWithColor(CalStringBuffer,20, 40, 4);
        //TimerDelayMs(2000);
        ercode= E_OK;
    }
    else
    {
        sprintf(CalStringBuffer, "No  AWBGS.txt");
        Cal_ShowStringWithColor(CalStringBuffer,20, 40, 4);
        //TimerDelayMs(2000);
        AAA_Adjust.AWBGS.Rgain = 0x100;
        AAA_Adjust.AWBGS.Bgain = 0x100;
        AAA_Adjust.AWBGS_Status = 0;
        ercode = E_SYS;
    }

    return ercode;
}

#if 0
#endif
void Cal_PassAdjustData(void)
{
    if((AAA_Adjust.DP_Status.Prv_White == _Prv_W_Status) && \
        (AAA_Adjust.DP_Status.Prv_Black== _Prv_B_Status))
    {
        Cal_SetCalData(CAL_DATANUM_STATUS_PrvDP_W, (UINT*)&(AAA_Adjust.DP_Status.Prv_White));
        Cal_SetCalData(CAL_DATANUM_STATUS_PrvDP_B, (UINT*)&(AAA_Adjust.DP_Status.Prv_Black));
        Cal_SetCalData(CAL_DATANUM_DP_PreviewAddr, (UINT*)&AAA_Adjust.DPC_Addr[DPC_PRV]);
    }

    if((AAA_Adjust.DP_Status.Cap_White == _Cap_W_Status) && \
        (AAA_Adjust.DP_Status.Cap_Black == _Cap_B_Status))
    {
        Cal_SetCalData(CAL_DATANUM_STATUS_CapDP_W, (UINT*)&(AAA_Adjust.DP_Status.Cap_White));
        Cal_SetCalData(CAL_DATANUM_STATUS_CapDP_B, (UINT*)&(AAA_Adjust.DP_Status.Cap_Black));
        Cal_SetCalData(CAL_DATANUM_DP_CaptureAddr, (UINT*)&AAA_Adjust.DPC_Addr[DPC_CAP]);
    }
/*
    if (AAA_Adjust.LSC_Status == _LSC_Status)
    {
        Cal_SetCalData(CAL_DATANUM_STATUS_LSC, (UINT*)&AAA_Adjust.LSC_Status);
        Cal_SetCalData(CAL_DATANUM_LSC_Table, (UINT*)&AAA_Adjust.LSC_Table);
        Cal_SetCalData(CAL_DATANUM_LSC_TableTotalCnt, (UINT*)&AAA_Adjust.LSCTblCnt);
    }
    */
    if(AAA_Adjust.AWB_Status==_AWB_Status)
    {
    }
    else
    {
        AAA_Adjust.AWB.Rgain = 0x100;//Default_AWB_Rgain;
        AAA_Adjust.AWB.Bgain = 0x100;//Default_AWB_Bgain;
    }

    Cal_SetCalData(CAL_DATANUM_STATUS_AWB, (UINT*)&AAA_Adjust.AWB_Status);
    //ethan_20110801_addawbgain
    Cal_SetCalData(CAL_DATANUM_STATIS_AWBGS, (UINT*)&AAA_Adjust.AWBGS_Status);
    //ethan_20110801_addawbgain
    Cal_SetCalData(CAL_DATANUM_AWB_GAIN, (UINT*)&AAA_Adjust.AWB.Rgain);
    //ethan_20110801_addawbgsgain
    Cal_SetCalData(CAL_DATANUM_AWB_GAIN_GS, (UINT*)&AAA_Adjust.AWBGS.Rgain);
    Cal_SetCalData(CAL_DATANUM_STATUS_ECS, (UINT*)&AAA_Adjust.ECS_Status);
    Cal_SetCalData(CAL_DATANUM_ECS_Addr, (UINT*)&AAA_Adjust.ECS_Addr[ECS_FOV1]);
}

//ethan_20110801_chg to read only specific section
ER Cal_ReadData(UINT32 uisection)
{
    UINT tmpDataArea,ECSDataArea;
    PPSTORE_SECTION_HANDLE  pSectionHdl;
    ER Error_check;

    CPUCleanInvalidateDCacheBlock((UINT32)&AAA_Adjust, (UINT32)&AAA_Adjust + sizeof(AAA_Adjust));
    CPUDrainWriteBuffer();

    Error_check = get_blk((void *)&(tmpDataArea), POOL_ID_DEFECT_PIXEL);
    if(Error_check != E_OK)
    {
        debug_err(("Get dpfect pixel buffer error!!\r\n"));
    }
    else
        rel_blk(POOL_ID_DEFECT_PIXEL, (VP)tmpDataArea);

    //memset((void *)tmpDataArea,0,POOL_SIZE_DEFECT_PIXEL);//ethan_20110804_avoid to reset dp coordinate

    Error_check = get_blk((void *)&(ECSDataArea), POOL_ID_ECS);
    if(Error_check != E_OK)
    {
        debug_err(("Get ECS buffer error!!\r\n"));
    }
    else
        rel_blk(POOL_ID_ECS, (VP)ECSDataArea);

    //memset((void *)ECSDataArea,0,POOL_SIZE_ECS);

    // Enable PStore
    //PStore_EnablePS();
    switch(uisection)
    {
        case PSTORE_SEC_SENSOR:
            if ((pSectionHdl = PStore_OpenSection(PS_CAL_HEAD, PS_RDWR)) != E_PS_SECHDLER)
            {
                PStore_ReadSection((UINT8 *)&AAA_Adjust, 0, sizeof(AAA_Adjust), pSectionHdl);
                PStore_CloseSection(pSectionHdl);
            }
            else
                debug_err(("Pstore open to read FAIL\r\n"));
            break;

        case PSTORE_SEC_DP:
            if ((pSectionHdl = PStore_OpenSection(PS_CAL_DATA, PS_RDWR)) != E_PS_SECHDLER)
            {
                AAA_Adjust.DPC_Addr[DPC_PRV] = tmpDataArea;

                PStore_ReadSection((UINT8 *)AAA_Adjust.DPC_Addr[DPC_PRV], sizeof(AAA_Adjust),
                                (sizeof(UW)*(DP_Prv_MAX+DP_Cap_MAX)),
                                pSectionHdl);

                PStore_CloseSection(pSectionHdl);
            }
            else
                debug_err(("Pstore open to read FAIL\r\n"));
        break;
        case PSTORE_SEC_ECS:
            if ((pSectionHdl = PStore_OpenSection(PS_CAL_DATA2, PS_RDWR)) != E_PS_SECHDLER)
            {
                AAA_Adjust.ECS_Addr[ECS_FOV1] = ECSDataArea;
                PStore_ReadSection((UINT8 *)AAA_Adjust.ECS_Addr[ECS_FOV1], sizeof(AAA_Adjust)+(sizeof(UW)*(DP_Prv_MAX+DP_Cap_MAX)),
                                (sizeof(UW)*(ECS_MAX)),
                                pSectionHdl);
                PStore_CloseSection(pSectionHdl);
            }
            else
                debug_err(("Pstore open to read FAIL\r\n"));
        break;
        default://read all section
            Cal_ReadData(PSTORE_SEC_SENSOR);
            Cal_ReadData(PSTORE_SEC_DP);
            Cal_ReadData(PSTORE_SEC_ECS);
            break;
    }
    // disable PStore
   // PStore_DisablePS();

#if (_CALIBRATION_MODE_ == ENABLE)
    Cal_PassAdjustData();
#endif
    return E_OK;
}

//ethan_20110801_readAllCalibrationData,for Systeminit only
ER Cal_ReadCalData(void)
{
    ER ercode;
    UINT tmpDataArea,ECSDataArea;
    ercode = get_blk((void *)&(tmpDataArea), POOL_ID_DEFECT_PIXEL);
    if(ercode != E_OK)
    {
        debug_err(("Get dpfect pixel buffer error!!\r\n"));
    }
    else
    {
        rel_blk(POOL_ID_DEFECT_PIXEL, (VP)tmpDataArea);
        memset((void *)tmpDataArea,0,POOL_SIZE_DEFECT_PIXEL);
    }

    ercode = get_blk((void *)&(ECSDataArea), POOL_ID_ECS);
    if(ercode != E_OK)
    {
        debug_err(("Get ECS buffer error!!\r\n"));
    }
    else
    {
        rel_blk(POOL_ID_ECS, (VP)ECSDataArea);
        memset((void *)ECSDataArea,0,POOL_SIZE_ECS);
    }

    ercode = Cal_ReadData(NULL);
#if (_CALIBRATION_MODE_ == ENABLE)
    Cal_PassAdjustData();
#endif
    return ercode;
}

ER Cal_WriteCalData(UINT32 uiSection)
{
    PPSTORE_SECTION_HANDLE  pSectionHdl;

    CPUCleanInvalidateDCacheBlock((UINT32)&AAA_Adjust, (UINT32)&AAA_Adjust + sizeof(AAA_Adjust));
    CPUDrainWriteBuffer();

    //PStore_EnablePS();

    switch (uiSection)
    {
        case PSTORE_SEC_SENSOR:
            if ((pSectionHdl = PStore_OpenSection(PS_CAL_HEAD, PS_RDWR|PS_CREATE)) != E_PS_SECHDLER)
            {
                PStore_WriteSection((UINT8 *)&AAA_Adjust, 0, sizeof(AAA_Adjust), pSectionHdl);
                PStore_CloseSection(pSectionHdl);
            }
            else
                debug_err(("Pstore open to write FAIL\r\n"));
            break;

        case PSTORE_SEC_DP:
            if ((pSectionHdl = PStore_OpenSection(PS_CAL_DATA, PS_RDWR|PS_CREATE)) != E_PS_SECHDLER)
            {
                PStore_WriteSection((UINT8 *)AAA_Adjust.DPC_Addr[DPC_PRV], sizeof(AAA_Adjust),
                            (sizeof(UW)*(DP_Prv_MAX+DP_Cap_MAX)),
                            pSectionHdl);
                PStore_CloseSection(pSectionHdl);
            }
            else
                debug_err(("Pstore open to write FAIL\r\n"));
            break;
        case PSTORE_SEC_ECS:
            if ((pSectionHdl = PStore_OpenSection(PS_CAL_DATA2, PS_RDWR|PS_CREATE)) != E_PS_SECHDLER)
            {
                PStore_WriteSection((UINT8 *)AAA_Adjust.ECS_Addr[ECS_FOV1], sizeof(AAA_Adjust)+(sizeof(UW)*(DP_Prv_MAX+DP_Cap_MAX)),
                            (sizeof(UW)*(ECS_MAX)),
                            pSectionHdl);
                PStore_CloseSection(pSectionHdl);
            }
            else
                debug_err(("Pstore open to write FAIL\r\n"));
            break;
    }

    // Disable PStore
    //PStore_DisablePS();
    //Read AAA_Adjust from NAND again to avoid Nand_Data value be set to AAA_Adjust
    //Cal_ReadCalData(); //ethan_20110731_marked,for speed up
    Cal_ReadData(uiSection);//ethan_20110801_add

    return  E_OK;
}
ER Cal_DeleteCalData(void)
{
    return E_OK;
}

UINT32 Mshutter_Adjust(void)
{
    UINT ExposureTime;
    UINT8 ErrorCode = CAL_ERR_OK;
    CAL_AE_INFO AeSetting;
    CAL_MSHT_PARAM pMSHTParam;
    UINT32 PrvMode = SENSOR_MODE_1280X960;

//    Cal_AlgClearScreen();
//    KeyScan_Suspend();
//    aaa_SetAeCalibration(1);
    AAA_Adjust.Mshutter_Status=0;
//    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
//    ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
//    ImgCapture_SetParameter(_SiePath,_PreGamma);

//    Photo_LockAE(1);
//    Photo_LockAWB(1);
//    SensorDelayVD(1);

    // Set IPP to OFF
    Cal_SetMode(IPL_MODE_OFF, 0);

    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    // Lock 3A, FD and SD
    Cal_Lock3A(1);

    ExposureTime = 520; //LV15, ISO50. Should be able to reach normal luminance

    // calibration AE
    AeSetting.uiTarget = 128;
    AeSetting.uiLum = 0;
    AeSetting.uiAEType = AE_FIX_TIME;
    AeSetting.uiCAType = CAL_CASOURCE_RAW;
    AeSetting.uiExpT = (ExposureTime / 2); //Preview binning 2x
    AeSetting.uiISO = 50;
    AeSetting.uiAdjStep = 10;
    AeSetting.uiTimeout = 300;
    AeSetting.uiResult = 0;

    Cal_WaitAeStable(&AeSetting);

    //Calculate proper ISO/AGC value
//    AGC = WaitAeStableISO(128, ExpoTime2TGValue(ExposureTime/2,GetHDTotalPrv(),PixelClk));  //Preview binning 2x
    //sie_Set_ColorGain(256,256,256,256);
//    SetRBgain_White();

//    MShutterAdjFlag = TRUE;
    pMSHTParam.uiPrvMode = PrvMode;
    pMSHTParam.uiOBofs = 0x28;
    pMSHTParam.uiMsht = 200000 + ExposureTime;
    pMSHTParam.bSaveRaw = TRUE;

    // mshutter boundary
    pMSHTParam.uiMshtMax = Mshutter_Max;
    pMSHTParam.uiMshtMin = Mshutter_Min;

    // AE setting
    pMSHTParam.uiExpT = ExposureTime;
    pMSHTParam.uiISO = AeSetting.uiISO;

    Cal_MshtAdjust(&pMSHTParam);

    AAA_Adjust.Mshutter = pMSHTParam.uiMsht;
    if(AAA_Adjust.Mshutter > Mshutter_Max)
    {
        ErrorCode = CAL_ERR_MSHUTTER_MAX;
        AAA_Adjust.Mshutter=Default_Mshutter;
    }
    else if(AAA_Adjust.Mshutter < Mshutter_Min)
    {
        ErrorCode = CAL_ERR_MSHUTTER_MIN;
        AAA_Adjust.Mshutter=Default_Mshutter;
    }
    debug_err(("AAA_Adjust.Mshutter=%d\r\n",AAA_Adjust.Mshutter));

    if(ErrorCode != CAL_ERR_OK)
        AAA_Adjust.Mshutter_Status = 0;
    else
        AAA_Adjust.Mshutter_Status = _Mshutter_Status;

//    Cal_AlgClearScreen();
//    Cal_AlgDisplayErrorCode(20,80,ErrorCode);
//    sprintf(message, "M shutter=%4d", AAA_Adjust.Mshutter);
//    Cal_AlgShowString(message, 20, 110);

    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    if (ErrorCode == CAL_ERR_OK)
    {
        Cal_WriteCalData(PSTORE_SEC_SENSOR);
    }

    return ErrorCode;
}

UINT32 AWB_Adjust(void)
{
    CAL_AWB_PARAM AWBParam;
    UINT32 ErrorCode = CAL_ERR_OK;
    UINT32 GS_ercode = E_OK;
    CAL_AE_INFO AeSetting;
    UINT32 PrvMode = SENSOR_MODE_1280X960;
    FLGPTN  uiFlag = 0;

    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    Cal_ShowStringWithColor("Face Light and Press SHUTTER",20, 80, 3);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);

    // Set IPP to OFF
    Cal_SetMode(IPL_MODE_OFF, 0);

    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    // Lock 3A, FD and SD
    Cal_Lock3A(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD|PHOTO_AE|PHOTO_AWB));

    // AE setting
    AeSetting.uiTarget = 120;
    AeSetting.uiLum = 0;
    AeSetting.uiAEType = AE_FIX_GAIN;
    AeSetting.uiCAType = CAL_CASOURCE_SUBOUT;
    AeSetting.uiExpT = 30000;
    AeSetting.uiISO = 100;
    AeSetting.uiAdjStep = 500;
    AeSetting.uiTimeout = 300;
    AeSetting.uiResult = 0;

    Cal_WaitAeStable(&AeSetting);

    //AWBParam.uiBayAddr = Buffer;
    AWBParam.uiAWBType = CAL_AWBTYPE_PREVIEW;
    AWBParam.uiOBofs = 0x28;
    AWBParam.uiMsht = 31500;
    AWBParam.bSaveRaw = FALSE;
    AWBParam.uiExpT = (AeSetting.uiExpT * 2); //Preview binning 2x
    AWBParam.uiISO = AeSetting.uiISO;

    Cal_AWBAdjust(&AWBParam);

    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    debug_err(("R = %d G = %d B = %d\r\n", AWBParam.uiRGB[1], AWBParam.uiRGB[2], AWBParam.uiRGB[3]));

    AAA_Adjust.AWB.Rgain = AWBParam.uiRGB[2]*256/AWBParam.uiRGB[1];
    AAA_Adjust.AWB.Bgain = AWBParam.uiRGB[2]*256/AWBParam.uiRGB[3];

    AAA_Adjust.AWB_Status = _AWB_Status;

    GS_ercode = CheckAwbGS();
    if(GS_ercode == E_OK)
    {
        sprintf(CalStringBuffer,"AWBGS: Rgain %d  Bgain%d",AAA_Adjust.AWBGS.Rgain,AAA_Adjust.AWBGS.Bgain);
        Cal_ShowStringWithColor(CalStringBuffer,20, 40, 4);
    }
    else
    {
        sprintf(CalStringBuffer,"No AWBGS");
        Cal_ShowStringWithColor(CalStringBuffer,20, 40, 4);
    }

    sprintf(CalStringBuffer,"AWB Rgain %d  Bgain %d",(UINT)AAA_Adjust.AWB.Rgain,(UINT)AAA_Adjust.AWB.Bgain);
    Cal_ShowStringWithColor(CalStringBuffer,20, 60, 4);

    sprintf(CalStringBuffer,"AWB Adjust ErrorCode %d",(UINT)ErrorCode);
    Cal_ShowStringWithColor(CalStringBuffer,20, 80, 4);

    if (ErrorCode == CAL_ERR_OK)
    {
        Cal_WriteCalData(PSTORE_SEC_SENSOR);
    }

    sprintf(CalStringBuffer,"Press SHUTTER to return");
    Cal_ShowStringWithColor(CalStringBuffer,20, 100, 4);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);

    return ErrorCode;
}

UINT32 ISO_Adjust(void)
{
//    UINT32 AGCGain;
    CAL_ISO_PARAM ISOParam;
    UINT32 ErrorCode = CAL_ERR_OK;
    UINT32 PrvMode = SENSOR_MODE_1280X960;
    //FLGPTN  uiFlag = 0;

    AAA_Adjust.ISO_Status = 0;
    // Set IPP to OFF
    Cal_SetMode(IPL_MODE_OFF, 0);

    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    // Lock 3A, FD and SD
    Cal_Lock3A(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD|PHOTO_AE|PHOTO_AWB));

    ISOParam.uiPrvMode = SENSOR_MODE_1280X960;
    ISOParam.uiTarget = 128;
    ISOParam.uiTime = 15000;
    ISOParam.uiGain = 2000;
    ISOParam.uiAdjStep = 500;

    Cal_ISOAdjust(&ISOParam);

    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    if(ISOParam.uiISO100 <1)
        ErrorCode = CAL_ERR_ISO_MIN;
    else if(ISOParam.uiISO100 >10000)
        ErrorCode = CAL_ERR_ISO_MAX;
    else
    {
        ErrorCode = CAL_ERR_OK;
        AAA_Adjust.ISO_Status = _ISO_Status;
    }

    AAA_Adjust.Agc_ISO_100 = ISOParam.uiISO100;

    sprintf(CalStringBuffer,"ISO gain %d",(UINT)AAA_Adjust.Agc_ISO_100);
    Cal_ShowStringWithColor(CalStringBuffer,20, 120, 4);

    sprintf(CalStringBuffer,"ISO Adjust ErrorCode %d",(UINT)ErrorCode);
    Cal_ShowStringWithColor(CalStringBuffer,20, 200, 4);

    if (ErrorCode == CAL_ERR_OK)
    {
        Cal_WriteCalData(PSTORE_SEC_SENSOR);
    }

    return ErrorCode;
}

UINT32 DP_Adjust_Dark(void)
{
    UW DP_Cnt_CD = 0;
    UW DP_Cnt_PD = 0;
    UINT8 ErrorCode = CAL_ERR_OK;
    CAL_DPC_PARAM DPCParam;
    UINT32 PrvMode = SENSOR_MODE_1280X960;
    CAL_AE_INFO AeSetting;

    /**************** preview DP *********************/
    DPCParam.uiPrvMode = PrvMode;
    DPCParam.uiOBofs = 0x28;
    DPCParam.uiMsht = 31500;
    DPCParam.bSaveRaw = FALSE;
    DPCParam.uiTarget = 150;
    DPCParam.uiAEType = AE_FIX_TIMEnGAIN;
    DPCParam.uiTime = 133333;
    DPCParam.uiISO= 100; // 200;
    DPCParam.uiAdjStep = 0;
    DPCParam.uiExpRatio = 4;
    DPCParam.DPCInfo.Type = DPC_TYPE_DARK_PREVIEW;
    DPCParam.DPCInfo.BayAddr = 0;

    DPCParam.DPCInfo.DPAddr = AAA_Adjust.DPC_Addr[DPC_PRV];
    DPCParam.DPCInfo.Count = AAA_Adjust.DPC_Num[DPC_PRV];
    DPCParam.DPCInfo.MaxCount = DP_Prv_MAX;
    DPCParam.DPCInfo.Threshold = Dark_Limit_Prv;
    DPCParam.DPCInfo.Block_X = Block_X_Prv;
    DPCParam.DPCInfo.Block_Y = Block_Y_Prv;
    DPCParam.DPCInfo.SkipXL = SkipXL_Prv;
    DPCParam.DPCInfo.SkipXR = SkipXR_Prv;
    DPCParam.DPCInfo.SkipYU = SkipYU_Prv;
    DPCParam.DPCInfo.SkipYD = SkipYD_Prv;
    //DPCParam.uiDPNum = &DP_Cnt_PD;

    Cal_SetMode(IPL_MODE_OFF, 0);
    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);
    // Lock 3A, FD and SD
    Cal_Lock3A(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD|PHOTO_AE|PHOTO_AWB));

    // calibration AE
    AeSetting.uiTarget = DPCParam.uiTarget;
    AeSetting.uiLum = 0;
    AeSetting.uiAEType = DPCParam.uiAEType;
    AeSetting.uiCAType = CAL_CASOURCE_SUBOUT;//CAL_CASOURCE_RAW;
    AeSetting.uiExpT = DPCParam.uiTime;
    AeSetting.uiISO = DPCParam.uiISO;
    AeSetting.uiAdjStep = DPCParam.uiAdjStep;
    AeSetting.uiTimeout = 300;
    AeSetting.uiResult = 0;

    Cal_WaitAeStable(&AeSetting);

    Cal_DPCAdjust(&DPCParam);//preview

    //Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    DP_Cnt_PD = DPCParam.uiDPNum;

    //check result
    AAA_Adjust.DPC_Num[DPC_PRV] += DP_Cnt_PD;
    if(AAA_Adjust.DPC_Num[DPC_PRV] >= DP_Prv_MAX)
    {
        ErrorCode = CAL_ERR_DPPB_MAX;
        AAA_Adjust.DP_Status.Prv_Black= 0;
        //sprintf((char *)gCalMsg,"PrvW %d, ERCode %d NG", DP_Cnt_PW, ErrorCode);
        debug_msg("PrvD %d, ERCode %d NG\r\n", DP_Cnt_PD, ErrorCode);
    }
    else
    {
        ErrorCode = CAL_ERR_OK;
        AAA_Adjust.DP_Status.Prv_Black = _Prv_B_Status;
        //sprintf((char *)gCalMsg,"PrvW %d, ERCode %d OK", DP_Cnt_PW, ErrorCode);
        debug_msg("PrvD %d, ERCode %d OK\r\n", DP_Cnt_PD, ErrorCode);
    }

    if (1) // (ErrorCode == 0 || DP_SEARCH_TILL_END)
    {
        /**************** capture DP *********************/

        //UI cfg
        UI_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);

        /**************** capture DP *********************/
        DPCParam.uiPrvMode = PrvMode;
        DPCParam.uiOBofs = 0;
        DPCParam.uiMsht = 31500;
        DPCParam.bSaveRaw = FALSE; // Do not save raw
        DPCParam.uiTarget = 150;
        DPCParam.uiAEType = AE_FIX_TIMEnGAIN;
        DPCParam.uiTime = 133333;
        DPCParam.uiISO = 200; // 200;
        DPCParam.uiAdjStep = 0;
        DPCParam.uiExpRatio = 4;
        DPCParam.DPCInfo.Type = DPC_TYPE_DARK_STILL;
        DPCParam.DPCInfo.BayAddr = 0;

        DPCParam.DPCInfo.DPAddr = AAA_Adjust.DPC_Addr[DPC_CAP];
        DPCParam.DPCInfo.Count = AAA_Adjust.DPC_Num[DPC_CAP];
        DPCParam.DPCInfo.MaxCount = DP_Cap_MAX;
        DPCParam.DPCInfo.Threshold = Dark_Limit_Cap;
        DPCParam.DPCInfo.Block_X = Block_X_Cap;
        DPCParam.DPCInfo.Block_Y = Block_Y_Cap;
        DPCParam.DPCInfo.SkipXL = SkipXL_Cap;
        DPCParam.DPCInfo.SkipXR = SkipXR_Cap;
        DPCParam.DPCInfo.SkipYU = SkipYU_Cap;
        DPCParam.DPCInfo.SkipYD = SkipYD_Cap;
//        DPCParam.uiDPNum = &DP_Cnt_CD;

        Cal_DPCAdjust(&DPCParam);//capture

        DP_Cnt_CD = DPCParam.uiDPNum;

        AAA_Adjust.DPC_Num[DPC_CAP] += DP_Cnt_CD;
        if(AAA_Adjust.DPC_Num[DPC_CAP] >= DP_Cap_MAX)
        {
            ErrorCode = CAL_ERR_DPCB_MAX;
            AAA_Adjust.DP_Status.Cap_Black = 0;
            //sprintf((char *)gCalMsg,"CapW %d, ERCode %d NG", DP_Cnt_CW, ErrorCode);
            debug_msg("CapD %d, ERCode %d NG\r\n", DP_Cnt_CD, ErrorCode);
        }
        else
        {
            ErrorCode = CAL_ERR_OK;
            AAA_Adjust.DP_Status.Cap_Black = _Cap_B_Status;
            //sprintf((char *)gCalMsg,"CapW %d, ERCode %d OK", DP_Cnt_CW, ErrorCode);
            debug_msg("CapD %d, ERCode %d OK\r\n", DP_Cnt_CD, ErrorCode);
        }
    }
#if (SaveDPCoor== ENABLE)
    {
        UINT32 i;
        for(i=0;i<DPCParam.uiDPNum<<2;i+=4)
        {
            debug_msg(" %d:(%d,%d) ",i>>2,(*(UINT32*)(DPCParam.DPCInfo.DPAddr+i))&0xffff,(*(UINT32*)(DPCParam.DPCInfo.DPAddr+i))>>16);
        }
    }
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                    (UB*)DPCParam.DPCInfo.DPAddr,
                    DPCParam.uiDPNum<<2,
                    0,
                    1,
                    FST_FMT_RAW,
                    FST_TIME_INFINITE);
#endif

    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);
    return ErrorCode;
}

UINT32 DP_Adjust_White(void)
{
    UW DP_Cnt_CW = 0;
    UW DP_Cnt_PW = 0;
    UINT8 ErrorCode = CAL_ERR_OK;
    CAL_DPC_PARAM DPCParam;
    UINT32 PrvMode = SENSOR_MODE_1280X960;
    CAL_AE_INFO AeSetting;

    /**************** preview DP *********************/
    DPCParam.uiPrvMode = PrvMode;
    DPCParam.uiOBofs = 0x28;
    DPCParam.uiMsht = 31500;
    DPCParam.bSaveRaw = FALSE; // Do not save raw
    DPCParam.uiTarget = 150;
    DPCParam.uiAEType = AE_FIX_GAIN;
    DPCParam.uiTime = 33333;
    DPCParam.uiISO = 50;
    DPCParam.uiAdjStep = 500;
    DPCParam.uiExpRatio = 4;
    DPCParam.DPCInfo.Type = DPC_TYPE_BRIGHT_PREVIEW;
    DPCParam.DPCInfo.BayAddr = 0;
    DPCParam.DPCInfo.DPAddr = AAA_Adjust.DPC_Addr[DPC_PRV];
    DPCParam.DPCInfo.BayAddr = 0;
    DPCParam.DPCInfo.Count = AAA_Adjust.DPC_Num[DPC_PRV];
    DPCParam.DPCInfo.MaxCount = DP_Prv_MAX;
    DPCParam.DPCInfo.Threshold = Bright_Limit_Prv;
    DPCParam.DPCInfo.Block_X = Block_X_Prv;
    DPCParam.DPCInfo.Block_Y = Block_Y_Prv;
    DPCParam.DPCInfo.SkipXL = SkipXL_Prv;
    DPCParam.DPCInfo.SkipXR = SkipXR_Prv;
    DPCParam.DPCInfo.SkipYU = SkipYU_Prv;
    DPCParam.DPCInfo.SkipYD = SkipYD_Prv;
//    DPCParam.uiDPNum = &DP_Cnt_PW;

    Cal_SetMode(IPL_MODE_OFF, 0);
    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);
    // Lock 3A, FD and SD
    Cal_Lock3A(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD|PHOTO_AE|PHOTO_AWB));

    // calibration AE
    AeSetting.uiTarget = DPCParam.uiTarget;
    AeSetting.uiLum = 0;
    AeSetting.uiAEType = DPCParam.uiAEType;
    AeSetting.uiCAType = CAL_CASOURCE_SUBOUT;//CAL_CASOURCE_RAW;
    AeSetting.uiExpT = DPCParam.uiTime;
    AeSetting.uiISO = DPCParam.uiISO;
    AeSetting.uiAdjStep = DPCParam.uiAdjStep;
    AeSetting.uiTimeout = 300;
    AeSetting.uiResult = 0;

    Cal_WaitAeStable(&AeSetting);

    Cal_DPCAdjust(&DPCParam);//preview

    //Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    DP_Cnt_PW = DPCParam.uiDPNum;

    //check result
    AAA_Adjust.DPC_Num[DPC_PRV] += DP_Cnt_PW;
    if(AAA_Adjust.DPC_Num[DPC_PRV] >= DP_Prv_MAX)
    {
        ErrorCode = CAL_ERR_DPPW_MAX;
        AAA_Adjust.DP_Status.Prv_White = 0;
        //sprintf((char *)gCalMsg,"PrvW %d, ERCode %d NG", DP_Cnt_PW, ErrorCode);
        debug_msg("PrvW %d, ERCode %d NG\r\n", DP_Cnt_PW, ErrorCode);
    }
    else
    {
        ErrorCode = CAL_ERR_OK;
        AAA_Adjust.DP_Status.Prv_White = _Prv_W_Status;
        //sprintf((char *)gCalMsg,"PrvW %d, ERCode %d OK", DP_Cnt_PW, ErrorCode);
        debug_msg("PrvW %d, ERCode %d OK\r\n", DP_Cnt_PW, ErrorCode);
    }

    if (1) // (ErrorCode == 0 || DP_SEARCH_TILL_END)
    {
        /**************** capture DP *********************/

        //UI cfg
        UI_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);

        /**************** capture DP *********************/
        DPCParam.uiPrvMode = PrvMode;
        DPCParam.uiOBofs = 0;
        DPCParam.uiMsht = 31500;
        DPCParam.bSaveRaw = FALSE; // Do not save raw
        DPCParam.uiTarget = 150;
        DPCParam.uiAEType = AE_FIX_TIMEnGAIN;
        DPCParam.uiTime = 8333;
        DPCParam.uiISO = 50;
        DPCParam.uiAdjStep = 0;
        DPCParam.uiExpRatio = 4;
        DPCParam.DPCInfo.Type = DPC_TYPE_BRIGHT_STILL;
        DPCParam.DPCInfo.BayAddr = 0;

        DPCParam.DPCInfo.DPAddr = AAA_Adjust.DPC_Addr[DPC_CAP];
        DPCParam.DPCInfo.BayAddr = 0;
        DPCParam.DPCInfo.Count = AAA_Adjust.DPC_Num[DPC_CAP];
        DPCParam.DPCInfo.MaxCount = DP_Cap_MAX;
        DPCParam.DPCInfo.Threshold = Bright_Limit_Cap;
        DPCParam.DPCInfo.Block_X = Block_X_Cap;
        DPCParam.DPCInfo.Block_Y = Block_Y_Cap;
        DPCParam.DPCInfo.SkipXL = SkipXL_Cap;
        DPCParam.DPCInfo.SkipXR = SkipXR_Cap;
        DPCParam.DPCInfo.SkipYU = SkipYU_Cap;
        DPCParam.DPCInfo.SkipYD = SkipYD_Cap;
//        DPCParam.uiDPNum = &DP_Cnt_CW;

        Cal_DPCAdjust(&DPCParam);//capture

        DPCParam.uiDPNum = DP_Cnt_CW;

        AAA_Adjust.DPC_Num[DPC_CAP] += DP_Cnt_CW;
        if(AAA_Adjust.DPC_Num[DPC_CAP] >= DP_Cap_MAX)
        {
            ErrorCode = CAL_ERR_DPCW_MAX;
            AAA_Adjust.DP_Status.Cap_White=0;
            //sprintf((char *)gCalMsg,"CapW %d, ERCode %d NG", DP_Cnt_CW, ErrorCode);
            debug_msg("CapW %d, ERCode %d NG\r\n", DP_Cnt_CW, ErrorCode);
        }
        else
        {
            ErrorCode = CAL_ERR_OK;
            AAA_Adjust.DP_Status.Cap_White=_Cap_W_Status;
            //sprintf((char *)gCalMsg,"CapW %d, ERCode %d OK", DP_Cnt_CW, ErrorCode);
            debug_msg("CapW %d, ERCode %d OK\r\n", DP_Cnt_CW, ErrorCode);
        }
    }
#if (SaveDPCoor == ENABLE)
    {
        UINT32 i;
        for(i=0;i<DPCParam.uiDPNum<<2;i+=4)
        {
            debug_msg(" %d:(%d,%d) ",i>>2,(*(UINT32*)(DPCParam.DPCInfo.DPAddr+i))&0xffff,(*(UINT32*)(DPCParam.DPCInfo.DPAddr+i))>>16);
        }
    }
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                    (UB*)DPCParam.DPCInfo.DPAddr,
                    DPCParam.uiDPNum<<2,
                    0,
                    1,
                    FST_FMT_RAW,
                    FST_TIME_INFINITE);
#endif

    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);
    return ErrorCode;
}

UINT32 DP_Adjust(void)
{
    UINT8 ErrorCode;
    UINT32 BufAddr;
    ER Error_check;
    FLGPTN  uiFlag = 0;

    Cal_SetCalData(CAL_DATANUM_STATUS_PrvDP_W, 0);
    Cal_SetCalData(CAL_DATANUM_STATUS_PrvDP_B, 0);
    Cal_SetCalData(CAL_DATANUM_STATUS_CapDP_W, 0);
    Cal_SetCalData(CAL_DATANUM_STATUS_CapDP_B, 0);

    //get DP address & clear status
    Error_check = get_blk((void *)&(BufAddr), POOL_ID_DEFECT_PIXEL);
    if(Error_check != E_OK)
    {
        ;
    }
    else
    {
        rel_blk(POOL_ID_DEFECT_PIXEL, (VP)BufAddr);
    }

    memset((void *)BufAddr, 0x0, POOL_SIZE_DEFECT_PIXEL);
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    Cal_ShowStringWithColor("Face Light and Press SHUTTER",20, 80, 3);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);

    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    Cal_ShowStringWithColor("Process...",20, 80, 3);

    AAA_Adjust.DPC_Addr[DPC_PRV] = BufAddr;
    AAA_Adjust.DPC_Addr[DPC_CAP] = BufAddr + (DP_Prv_MAX<<2);
    AAA_Adjust.DPC_Num[DPC_PRV] = 0;
    AAA_Adjust.DPC_Num[DPC_CAP] = 0;
    AAA_Adjust.DP_Status.Prv_White = AAA_Adjust.DP_Status.Prv_Black = 0;
    AAA_Adjust.DP_Status.Cap_White = AAA_Adjust.DP_Status.Cap_Black = 0;

    ErrorCode = DP_Adjust_White();
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    sprintf(CalStringBuffer,"Prv Dark pixel %d",(UINT)AAA_Adjust.DPC_Num[DPC_PRV]);
    Cal_ShowStringWithColor(CalStringBuffer,20, 20, 4);
    sprintf(CalStringBuffer,"Cap Dark pixel %d",(UINT)AAA_Adjust.DPC_Num[DPC_CAP]);
    Cal_ShowStringWithColor(CalStringBuffer,20, 60, 4);
    TimerDelayMs(1500);

    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    Cal_ShowStringWithColor("Cover Lens and Press SHUTTER",20, 80, 3);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    Cal_ShowStringWithColor("Process...",20, 80, 3);

    ErrorCode = DP_Adjust_Dark();
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    sprintf(CalStringBuffer,"Prv Total pixel %d",(UINT)AAA_Adjust.DPC_Num[DPC_PRV]);
    Cal_ShowStringWithColor(CalStringBuffer,20, 80, 4);
    sprintf(CalStringBuffer,"Cap Total pixel %d",(UINT)AAA_Adjust.DPC_Num[DPC_CAP]);
    Cal_ShowStringWithColor(CalStringBuffer,20, 100, 4);

    sprintf(CalStringBuffer,"DP ErrorCode %d",ErrorCode);
    Cal_ShowStringWithColor(CalStringBuffer,20, 120, 4);

    if(ErrorCode == CAL_ERR_OK)
    {
        Cal_WriteCalData(PSTORE_SEC_SENSOR);
        Cal_WriteCalData(PSTORE_SEC_DP);
        //Cal_ReadCalData();
    }

    sprintf(CalStringBuffer,"Press SHUTTER to return");
    Cal_ShowStringWithColor(CalStringBuffer,20, 160, 4);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);
    return ErrorCode;
}


UINT32 ECS_Adjust(void)
{
    CAL_ECS_PARAM ECSParam;
    UINT32 BufAddr;
    UINT32 ErrorCode = CAL_ERR_NULL;
    ER Error_check;
    UINT32 PrvMode = SENSOR_MODE_1280X960;
    CAL_AE_INFO AeSetting;
    UINT32 i;
    FLGPTN  uiFlag = 0;

    Error_check = get_blk((void *)&(BufAddr), POOL_ID_ECS);
    if(Error_check != E_OK)
    {
        ;
    }
    else
    {
        rel_blk(POOL_ID_ECS, (VP)BufAddr);
    }

    debug_err(("POOL_ID_ECS Addr=0x%x\r\n",BufAddr));
    //memset((void *)BufAddr, 0x10040100, POOL_SIZE_ECS);
    for(i=0;i<ECS_MAX;i++)
        *(UINT32 *)(BufAddr + i*4) = 0x10040100;

    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    Cal_ShowStringWithColor("Face Light and Press SHUTTER",20, 80, 3);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);

    AAA_Adjust.ECS_Status = 0;
    Cal_SetCalData(CAL_DATANUM_STATUS_ECS, (UINT*)&AAA_Adjust.ECS_Status);

    Cal_SetMode(IPL_MODE_OFF, 0);

    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    // Lock 3A, FD and SD
    Cal_Lock3A(1);
    Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD|PHOTO_AE|PHOTO_AWB));

    // AE setting
    AeSetting.uiTarget = 150;
    AeSetting.uiLum = 0;
    AeSetting.uiAEType = AE_FIX_TIME;
    AeSetting.uiCAType = CAL_CASOURCE_SUBOUT;
    AeSetting.uiExpT = 4167;
    AeSetting.uiISO = 100;
    AeSetting.uiAdjStep = 1;
    AeSetting.uiTimeout = 300;
    AeSetting.uiResult = 0;

    Cal_WaitAeStable(&AeSetting);
    //AWBParam.uiBayAddr = Buffer;
    ECSParam.uiCalibMode = SENSOR_MODE_FULL;
    ECSParam.uiOBofs = 0x28;
    ECSParam.uiMsht = 31500;
    ECSParam.uiCmpRatio= 100; // don't over than 100(%)
    ECSParam.bSaveRaw = FALSE;
    ECSParam.uiTableAddr= BufAddr;
    ECSParam.uiExpT = (AeSetting.uiExpT * 2); //Preview binning 2x
    ECSParam.uiISO = AeSetting.uiISO;

    Cal_ECSAdjust(&ECSParam);

    AAA_Adjust.ECS_Status = _ECS_Status;
    AAA_Adjust.ECS_Addr[ECS_FOV1] = ECSParam.uiTableAddr;

    Cal_SetCalData(CAL_DATANUM_STATUS_ECS, (UINT*)&AAA_Adjust.ECS_Status);
    Cal_SetCalData(CAL_DATANUM_ECS_Addr, (UINT*)&AAA_Adjust.ECS_Addr[ECS_FOV1]);
    // Set IPP to preview
    Cal_SetMode(IPL_MODE_PREVIEW, PrvMode);

    ErrorCode = CAL_ERR_OK;

    if(ErrorCode == CAL_ERR_OK)
    {
        Cal_WriteCalData(PSTORE_SEC_SENSOR);
        Cal_WriteCalData(PSTORE_SEC_ECS);
    }
    sprintf(CalStringBuffer,"ECS ErrorCode %d",(UINT)ErrorCode);
    Cal_ShowStringWithColor(CalStringBuffer,20, 120, 4);

    sprintf(CalStringBuffer,"Press SHUTTER to return");
    Cal_ShowStringWithColor(CalStringBuffer,20, 160, 4);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);

    return ErrorCode;
}

//ethan_20110801_add
ER Cal_ShowCalRst(void)
{
    ER ercode = 0;
    FLGPTN  uiFlag = 0;
    ercode = Cal_ReadCalData();
    if(ercode!=E_OK)
    {
        debug_err(("^R %s: read ps error\r\n",__func__));
        return E_SYS;;
    }
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);
    if((AAA_Adjust.DP_Status.Prv_White == _Prv_W_Status) && \
        (AAA_Adjust.DP_Status.Prv_Black== _Prv_B_Status))
    {
        sprintf(CalStringBuffer,"DP Pixel num: Prv:%d",(UINT)AAA_Adjust.DPC_Num[DPC_PRV]);
        Cal_ShowStringWithColor(CalStringBuffer,20, 20, 4);
    }

    if((AAA_Adjust.DP_Status.Cap_White == _Cap_W_Status) && \
        (AAA_Adjust.DP_Status.Cap_Black == _Cap_B_Status))
    {
        sprintf(CalStringBuffer,"DP Pixel num: Cap:%d",(UINT)AAA_Adjust.DPC_Num[DPC_CAP]);
        Cal_ShowStringWithColor(CalStringBuffer,20, 40, 4);
    }

    if(AAA_Adjust.ECS_Status == _ECS_Status)
        sprintf(CalStringBuffer,"ECS Rst: OK");
    else
        sprintf(CalStringBuffer,"ECS Rst: NG");
    Cal_ShowStringWithColor(CalStringBuffer,20, 60, 4);

    if(AAA_Adjust.AWB_Status == _AWB_Status)
        sprintf(CalStringBuffer,"AWB: Rgain:%d Bgain:%d",AAA_Adjust.AWB.Rgain,AAA_Adjust.AWB.Bgain);
    else
        sprintf(CalStringBuffer,"AWB: NG");
    Cal_ShowStringWithColor(CalStringBuffer,20, 80, 4);

    if(AAA_Adjust.AWBGS_Status == _AWB_Status)
        sprintf(CalStringBuffer,"AWB GS: Rgain:%d Bgain:%d",AAA_Adjust.AWBGS.Rgain,AAA_Adjust.AWBGS.Bgain);
    else
        sprintf(CalStringBuffer,"AWBGS: NG");
    Cal_ShowStringWithColor(CalStringBuffer,20, 100, 4);

    sprintf(CalStringBuffer,"Press SHUTTER to return");
    Cal_ShowStringWithColor(CalStringBuffer,20, 120, 4);

    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);
    return E_OK;
}

void Cal_RstCalRst(void)
{
    memset((UINT8 *)&AAA_Adjust, 0, sizeof(Adjust_Struct));
}

//ethan_20110801_add
ER Cal_SaveAWBGS(void)
{
    ER ercode = 0;
    FLGPTN  uiFlag = 0;
    HNVT_FILE *pFile;
    Cal_ClearOSD(_OSD_INDEX_TRANSPART);

    if(AAA_Adjust.AWB_Status == _AWB_Status)
    {
        AAA_Adjust.AWBGS_Status = _AWB_Status;
        AAA_Adjust.AWBGS.Rgain = AAA_Adjust.AWB.Rgain;
        AAA_Adjust.AWBGS.Bgain = AAA_Adjust.AWB.Bgain;
        sprintf(CalStringBuffer,"AWB GS: Rgain:%d Bgain:%d",AAA_Adjust.AWBGS.Rgain,AAA_Adjust.AWBGS.Bgain);
        Cal_ShowStringWithColor(CalStringBuffer,20, 100, 4);
        ercode =  E_OK;
    }
    else
        ercode =  E_SYS;
    sprintf(CalStringBuffer,"Press SHUTTER to return");
    Cal_ShowStringWithColor(CalStringBuffer,20, 120, 4);

    if(ercode == E_OK)
    {
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        pFile = fs_fopen("A:\\AWBGS.txt","w");
        if(pFile == NULL)
        {
            sprintf(CalStringBuffer, "create  AWBGS.txt fail");
            Cal_ShowStringWithColor(CalStringBuffer,20, 40, 4);
            TimerDelayMs(2000);
        }
        else
        {
            sprintf(CalStringBuffer, "%d %d",AAA_Adjust.AWBGS.Rgain,AAA_Adjust.AWBGS.Bgain);
            fs_fwrite(CalStringBuffer, 1, 7, pFile);
            fs_fclose(pFile);
            g_CalStringRect.uiLeft    = 20;
            g_CalStringRect.uiTop     = 200;
            g_CalStringRect.uiWidth   = 300;
            g_CalStringRect.uiHeight  = 30;
            sprintf(CalStringBuffer, "AWBGS.txt OK");
            Cal_ShowStringWithColor(CalStringBuffer,20, 40, 4);
            TimerDelayMs(2000);
        }

        Cal_WriteCalData(PSTORE_SEC_SENSOR);
    }
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW | TWF_CLR);
    return ercode;
}
// EOF
