#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "UIFlow.h"
#include "FwVersion.h"
#include "PrimaryTsk.h"
#include "AviRecAPI.h"
#include "SystemInit.h"
#include "Interface.h"
#include "UIConfig.h"
#include "sdio.h"
#include "MediaRecAPI.h"

#if 0
#include "debug.h"
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

static UINT32 guiBatteryLevel       = UIDFT_BATTERY_LVL;
static UINT32 guiCardStatus         = UIDFT_CARD_STATUS;
static UINT32 guiFsStatus           = UIDFT_FS_STATUS;
static UINT32 guiMP3FtbRenewIndex   = UIDFT_MP3FTB_RENEW;
static UINT32 guiAudioVolume        = UIDFT_AUDIO_VOLUME;

// Version
//static CHAR gMainVer[]              = DSC_FW_VERSION_STR;
//static CHAR gVersion[MAX_VER_STR_LEN];

// Current menu info
static UIMenuStoreInfo gCurrentMenuInfo;

// Must in the same order of those in enum FS_STATUS
static const CHAR *strFSStatusTable[FS_STATUS_TOTAL_NUM] =
{
    "FS NOT INIT",
    "FS INIT OK",
    "FS UNFORMATTED",
    "FS UNKNOWN FORMAT",
    "FS DISK ERROR"
};

// Bootloader info
//#NT#2011/10/04#KS Hung -begin
static BLINFO_OBJ gBLInfo={0};
//#NT#2011/10/04#KS Hung -end

// Social network
#define SN_DATA_OFFSET              7
static CHAR sSocialNetworkData[] =  "ViviLnk0";

//-----------------------------------------------------------------------------
// Set/Get System Flags
//-----------------------------------------------------------------------------
void SysSetFlag(UINT32 uiFlag, UINT32 uiValue)
{
    gCurrentMenuInfo.uiSysFlag[uiFlag] = uiValue;
}

UINT32 SysGetFlag(UINT32 uiFlag)
{
    return (gCurrentMenuInfo.uiSysFlag[uiFlag]);
}

char* SysGetNZHCarNoStamp(void)
{
    return gCurrentMenuInfo.ubCarNumber;
}
void SysSetNZHCarNoStamp(char* pCustomerStamp)
{
    strcpy(gCurrentMenuInfo.ubCarNumber,pCustomerStamp);
}

char* SysGetZHCarNoStamp(void)
{
    return gCurrentMenuInfo.strZHCarNoStamp;
}
void SysSetZHCarNoStamp(char* pCustomerStamp)
{
    strcpy(gCurrentMenuInfo.strZHCarNoStamp,pCustomerStamp);
}

static void SysLimitFlag(UINT32 uiFlag, UINT32 uiMinId, UINT32 uiMaxId, UINT32 uiDefaultId)
{
    UINT32  uiValue = SysGetFlag(uiFlag);

    #if 0
    LIMIT(uiValue, uiMinId, uiMaxId-1);
    #else
    if ((uiValue < uiMinId) || (uiValue > uiMaxId-1))
    {
        uiValue = uiDefaultId;
    }
    #endif

    SysSetFlag(uiFlag, uiValue);
}

static void SysCheckFlag(void)
{
    // Check and limit system flag value

    // Photo
    SysLimitFlag(FL_PHOTO_SIZE,         0,  PHOTO_SIZE_ID_MAX,         DEFAULT_PHOTO_SIZE);
    SysLimitFlag(FL_SCENE,              0,  SCENE_ID_MAX,              DEFAULT_SCENE);
    SysLimitFlag(FL_QUALITY,            0,  QUALITY_ID_MAX,            DEFAULT_PHOTO_QUALITY);
    SysLimitFlag(FL_SELFTIMER,          0,  SELFTIMER_ID_MAX,          DEFAULT_SELFTIMER);
    SysLimitFlag(FL_ANTISHAKE,          0,  ANTISHAKE_ID_MAX,          DEFAULT_ANTISHAKING);
    SysLimitFlag(FL_FD,                 0,  FD_ID_MAX,                 DEFAULT_FD);
    SysLimitFlag(FL_WB,                 0,  WB_ID_MAX,                 DEFAULT_WB);
    SysLimitFlag(FL_COLOR_EFFECT,       0,  COLOR_EFFECT_ID_MAX,       DEFAULT_PHOTO_COLOR);
    SysLimitFlag(FL_CONTINUE_SHOT,      0,  CONTINUE_SHOT_SETTING_MAX, DEFAULT_CONTINUE_SHOT);
    SysLimitFlag(FL_SHARPNESS,          0,  SHARPNESS_ID_MAX,          DEFAULT_SHARPNESS);
    SysLimitFlag(FL_EV,                 0,  EV_ID_MAX,                 DEFAULT_EV);
    SysLimitFlag(FL_ISO,                0,  ISO_ID_MAX,                DEFAULT_ISO);
    SysLimitFlag(FL_METERING,           0,  METERING_ID_MAX,           DEFAULT_METERING);
    SysLimitFlag(FL_PHOTO_VIVILINK,     0,  PHOTO_VIVILINK_ID_MAX,     DEFAULT_PHOTO_VIVILINK);

    // Movie
    SysLimitFlag(FL_MOVIE_SIZE,         0,  MOVIE_SIZE_ID_MAX,         DEFAULT_MOVIE_SIZE);
    SysLimitFlag(FL_MOVIE_VIVILINK,     0,  MOVIE_VIVILINK_ID_MAX,     DEFAULT_MOVIE_VIVILINK);
    SysLimitFlag(FL_MOVIE_LOOP_REC,     0,  MOVIE_CYCLICREC_ID_MAX,         DEFAULT_MOVIE_CYCLICREC);
    SysLimitFlag(FL_MOVIE_MOTION_DET,   0,  MOVIE_MOTIONDET_SETTING_MAX,    DEFAULT_MOVIE_MOTION_DET);
    SysLimitFlag(FL_MOVIE_AUDIO,        0,  MOVIE_AUDIOENABLE_SETTING_MAX,  DEFAULT_MOVIE_AUDIO);
    SysLimitFlag(FL_MOVIE_DATEIMPRINT,  0,  MOVIE_DateImprint_SETTING_MAX,  DEFAULT_MOVIE_DATEIMPRINT);
    SysLimitFlag(FL_SENSOR_ROTATE,      0,  SEN_ROTATE_MAX,                 DEFAULT_SENSOR_ROTATE);
    SysLimitFlag(FL_MOVIE_PARKING,      0,  MOVIE_PARKING_MAX,          DEFAULT_MOVIE_PARKING);
    SysLimitFlag(FL_MOVIE_TIMESHRINK,      0,  MOVIE_TIMESHRINK_MAX,          DEFAULT_MOVIE_TIMESHRINK);
    // Playback
    SysLimitFlag(FL_PROTECT,            0,  FILE_PROTECT_ID_MAX,    DEFAULT_PROTECT);
    SysLimitFlag(FL_SLIDE_SHOW,         0,  SLIDE_SHOW_ID_MAX,      DEFAULT_SLIDE_SHOW);

    // System
    SysLimitFlag(FL_LANGUAGE,           0,  LANG_ID_MAX,            DEFAULT_LANGUAGE);
    SysLimitFlag(FL_DATE_TIME,          0,  DATE_TIME_ID_MAX,       DEFAULT_DATE_TIME);
    SysLimitFlag(FL_BEEP,               0,  BEEP_ID_MAX,            DEFAULT_BEEP);
    //#NT#2011/08/03#KS Hung -begin
    SysLimitFlag(FL_SILENT,             0,  SILENT_ID_MAX,          DEFAULT_SILENT);
    //#NT#2011/08/03#KS Hung -end
    SysLimitFlag(FL_AUTO_POWER_OFF,     0,  AUTO_POWER_OFF_ID_MAX,  DEFAULT_AUTO_POWER_OFF);
    SysLimitFlag(FL_FREQUENCY,          0,  FREQUENCY_ID_MAX,       DEFAULT_FREQUENCY);
    SysLimitFlag(FL_TV_MODE,            0,  TV_MODE_ID_MAX,         DEFAULT_TV_MODE);
    SysLimitFlag(FL_PLATENUMBER,          0,  PLATENUMBER_ID_MAX,       DEFAULT_MOVIE_CARNUMBER);

    SysLimitFlag(FL_DATE_FORMAT,        0,  DATE_FORMAT_ID_MAX,     DEFAULT_DATE_FORMAT);
    SysLimitFlag(FL_OPENING_LOGO,       0,  OPENING_LOGO_ID_MAX,    DEFAULT_OPENING_LOGO);
    SysLimitFlag(FL_LCD_DISPLAY,        0,  DISPLAY_ID_MAX,         DEFAULT_LCD_DISPLAY);
    SysLimitFlag(FL_LCD_BRIGHTNESS,     0,  LCDBRT_LVL_ID_MAX,      DEFAULT_LCD_BRIGHTNESS);
    SysLimitFlag(FL_MACRO,              0,  MACRO_ID_MAX,           DEFAULT_MACRO);
    SysLimitFlag(FL_USB_MODE,           0,  USB_MODE_ID_MAX,        DEFAULT_USB_MODE);

    // Sepcial flags
    SysLimitFlag(FL_DCF_DIR_ID,         MIN_DCF_DIR_NUM,    MAX_DCF_DIR_NUM,    MIN_DCF_DIR_NUM);
    SysLimitFlag(FL_DCF_FILE_ID,        MIN_DCF_FILE_NUM,   MAX_DCF_FILE_NUM,   MIN_DCF_FILE_NUM);

    SysLimitFlag(FL_GSENSOR,           0,  GSENSOR_ID_MAX,       DEFAULT_MOVIE_GSENSOR);
    SysLimitFlag(FL_LCD_AUTOOFF,           0,  LCD_AUTO_POWER_OFF_ID_MAX,        DEFAULT_LCD_AUTO_OFF);
    SysLimitFlag(FL_POWEROFF_DELAY,           0,  DELAY_POWER_OFF_ID_MAX,        1);

}

static void SysSetFixedFlag(void)
{
    // Fixed some system flags as system reset

    // Photo
    //SysSetFlag(FL_COLOR_EFFECT,          DEFAULT_PHOTO_COLOR);
    //SysSetFlag(FL_EV,                     DEFAULT_EV);
    //SysSetFlag(FL_ISO,                  DEFAULT_ISO);
    //SysSetFlag(FL_WB,                   DEFAULT_WB);
    //SysSetFlag(FL_METERING,             DEFAULT_METERING);
    //SysSetFlag(FL_SHARPNESS,            DEFAULT_SHARPNESS);
    //SysSetFlag(FL_SATURATION,           DEFAULT_SATURATION);
    SysSetFlag(FL_CONTINUE_SHOT,          DEFAULT_CONTINUE_SHOT);
    //SysSetFlag(FL_FD,                   DEFAULT_FD);
    //SysSetFlag(FL_ANTISHAKING,          DEFAULT_ANTISHAKING);
    SysSetFlag(FL_SELFTIMER,            DEFAULT_SELFTIMER);
    //SysSetFlag(FL_FLASH_MODE,           DEFAULT_FLASH_MODE);
    //SysSetFlag(FL_PHOTO_VIVILINK,       DEFAULT_PHOTO_VIVILINK);

    // Movie
    //SysSetFlag(FL_MOVIE_COLOR,          DEFAULT_MOVIE_COLOR);
    //SysSetFlag(FL_MOVIE_SELFTIMER,      DEFAULT_MOVIE_SELFTIMER);
    //SysSetFlag(FL_MOVIE_VIVILINK,       DEFAULT_MOVIE_VIVILINK);

    // Playback
    SysSetFlag(FL_PROTECT,              DEFAULT_PROTECT);

    // System
    SysSetFlag(FL_LCD_DISPLAY,          DEFAULT_LCD_DISPLAY);
    SysSetFlag(FL_LCD_BRIGHTNESS,       DEFAULT_LCD_BRIGHTNESS);
    //SysSetFlag(FL_MACRO,              DEFAULT_MACRO);
    //SysSetFlag(FL_USB_MODE,           DEFAULT_USB_MODE);

    /*
    // designated system flags
    SysSetFL(FL_AUDIO_VOLUME,           AUDIO_VOL_5);
    SysSetFL(FL_FLASH_READY,            FALSE);
    */
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        //SysSetFlag(FL_PHOTO_SIZE, PHOTO_SIZE_5M);
        //SysSetFlag(FL_MOVIE_SIZE, MOVIE_SIZE_720P);
    }

}

static void SysResetFlag(void)
{
    // Set system flag default value here

    // Photo
    SysSetFlag(FL_PHOTO_SIZE,           DEFAULT_PHOTO_SIZE);
    SysSetFlag(FL_QUALITY,        DEFAULT_PHOTO_QUALITY);
    SysSetFlag(FL_COLOR_EFFECT,          DEFAULT_PHOTO_COLOR);
    SysSetFlag(FL_SCENE,                DEFAULT_SCENE);
    SysSetFlag(FL_EV,                   DEFAULT_EV);
    SysSetFlag(FL_ISO,                  DEFAULT_ISO);
    SysSetFlag(FL_WB,                   DEFAULT_WB);
    SysSetFlag(FL_METERING,             DEFAULT_METERING);
    SysSetFlag(FL_SHARPNESS,            DEFAULT_SHARPNESS);
    SysSetFlag(FL_SATURATION,           DEFAULT_SATURATION);
    SysSetFlag(FL_FD,                   DEFAULT_FD);
    SysSetFlag(FL_ANTISHAKE,            DEFAULT_ANTISHAKING);
    SysSetFlag(FL_QUICK_REVIEW,         DEFAULT_QUICK_REVIEW);
    SysSetFlag(FL_DATE_STAMP,           DEFAULT_DATE_STAMP);
    SysSetFlag(FL_SELFTIMER,            DEFAULT_SELFTIMER);
    SysSetFlag(FL_FLASH_MODE,           DEFAULT_FLASH_MODE);
    SysSetFlag(FL_PHOTO_VIVILINK,       DEFAULT_PHOTO_VIVILINK);
    SysSetFlag(FL_CONTINUE_SHOT ,       DEFAULT_CONTINUE_SHOT);

    // Movie
    //#NT#2011/09/23#KS Hung -begin
    SysSetFlag(FL_MOVIE_SIZE,           DEFAULT_MOVIE_SIZE);
    //#NT#2011/09/23#KS Hung -end
    SysSetFlag(FL_MOVIE_QUALITY,        DEFAULT_MOVIE_QUALITY);
    SysSetFlag(FL_MOVIE_COLOR,          DEFAULT_MOVIE_COLOR);
    SysSetFlag(FL_MOVIE_SELFTIMER,      DEFAULT_MOVIE_SELFTIMER);
    SysSetFlag(FL_MOVIE_EV,             DEFAULT_MOVIE_EV);
    SysSetFlag(FL_MOVIE_VIVILINK,       DEFAULT_MOVIE_VIVILINK);
    SysSetFlag(FL_MOVIE_LOOP_REC,       DEFAULT_MOVIE_CYCLICREC);
    SysSetFlag(FL_MOVIE_MOTION_DET,     DEFAULT_MOVIE_MOTION_DET);
    SysSetFlag(FL_MOVIE_AUDIO,          DEFAULT_MOVIE_AUDIO);
    SysSetFlag(FL_MOVIE_DATEIMPRINT,    DEFAULT_MOVIE_DATEIMPRINT);
    SysSetFlag(FL_SENSOR_ROTATE,        DEFAULT_SENSOR_ROTATE);
    SysSetFlag(FL_MOVIE_PARKING,        DEFAULT_MOVIE_PARKING);
    SysSetFlag(FL_MOVIE_TIMESHRINK,    DEFAULT_MOVIE_TIMESHRINK);
    // Playback
    SysSetFlag(FL_PROTECT,              DEFAULT_PROTECT);
    SysSetFlag(FL_SLIDE_SHOW,           DEFAULT_SLIDE_SHOW);

    // System
    SysSetFlag(FL_AUTO_POWER_OFF,       DEFAULT_AUTO_POWER_OFF);
    SysSetFlag(FL_DATE_TIME,            DEFAULT_DATE_TIME);
    SysSetFlag(FL_BEEP,                 DEFAULT_BEEP);
    SysSetFlag(FL_SILENT,               DEFAULT_SILENT);
    SysSetFlag(FL_LANGUAGE,             DEFAULT_LANGUAGE);
    SysSetFlag(FL_FREQUENCY,            DEFAULT_FREQUENCY);
    SysSetFlag(FL_TV_MODE,              DEFAULT_TV_MODE);
    SysSetFlag(FL_DATE_FORMAT,          DEFAULT_DATE_FORMAT);
    SysSetFlag(FL_PLATENUMBER,    DEFAULT_MOVIE_CARNUMBER);
    SysSetFlag(FL_OPENING_LOGO,         DEFAULT_OPENING_LOGO);

    SysSetFlag(FL_LCD_DISPLAY,          DEFAULT_LCD_DISPLAY);
    SysSetFlag(FL_LCD_BRIGHTNESS,       DEFAULT_LCD_BRIGHTNESS);
    SysSetFlag(FL_MACRO,                DEFAULT_MACRO);
    SysSetFlag(FL_USB_MODE,             DEFAULT_USB_MODE);

    #if (UPDATE_CFG == UPDATE_CFG_YES)
    SysSetFlag(FL_MOVIE_SIZE,           UI_GetMovieSizeDefaultIndex());
    if (GPIOMap_DetStrgCard() == FALSE) {
        SysSetFlag(FL_MOVIE_SIZE,           UI_GetMovieSizeDefaultIndex());
        SysSetFlag(FL_PHOTO_SIZE,           UI_GetSizeDefaultIndex());
    } else {
        SysSetFlag(FL_MOVIE_SIZE,           UI_GetMovieSizeDefaultIndex());
        SysSetFlag(FL_PHOTO_SIZE,           UI_GetRamSizeDefaultIndex());
    }
    SysSetFlag(FL_LANGUAGE,             UI_GetLanguageDefaultIndex());
    SysSetFlag(FL_FREQUENCY,            UI_GetFreqDefaultIndex());
    SysSetFlag(FL_TV_MODE,              UI_GetTVModeDefaultIndex());
    SysSetFlag(FL_ANTISHAKE,            UI_GetAntiShakeDefaultIndex());
    SysSetFlag(FL_DATE_TIME,            UI_GetDateImprintDefaultIndex());
    SysSetFlag(FL_AUTO_POWER_OFF, UI_GetAutoPowerOffDefaultIndex());
    SysSetFlag(FL_LCD_AUTOOFF,                UI_GetLcdOffDefaultIndex());
    SysSetFlag(FL_BEEP,                   UI_GetBeepDefaultIndex());
	
    SysSetFlag(FL_GSENSOR, UI_GetGsensorDefaultIndex());
    SysSetFlag(FL_MOVIE_PARKING, UI_GetParkingDefaultIndex());
    SysSetFlag(FL_MOVIE_LOOP_REC, UI_GetCyclicRecDefaultIndex());

    #endif
    //SysSetFlag(FL_GSENSOR,              2);
    //SysSetFlag(FL_LCD_AUTOOFF,       DEFAULT_LCD_AUTO_OFF);
    //SysSetFlag(FL_POWEROFF_DELAY,              0);
	
    SysSetFixedFlag();

    //#NT#2011/09/23#KS Hung -begin
    //SysSetFlag(FL_MOVIE_SIZE,           DEFAULT_MOVIE_SIZE);
    //#NT#2011/09/23#KS Hung -begin
    /*
    // reset all system parameters
    SysSetFL(FL_AUDIO_VOLUME,           AUDIO_VOL_5);
    SysSetFL(FL_MP3_REPEAT_MODE,        MP3_REPEAT_ALL);
    SysSetFL(FL_LCD_BRIGHTNESS,         BRIGHTNESS_N2);
    */
}

//-----------------------------------------------------------------------------
// Firmware Version String
//-----------------------------------------------------------------------------

CHAR *GetVersionString(void)
{
    //return GetUIConfigSoftwareVer();
    return pFwVersion;
}

CHAR *GetVersionString2(void)
{
//    return GetUIConfigSoftwareVer();
    return pFwVersion;
}


CHAR *GetMakerString(void)
{
    return pMakerString;
}

CHAR *GetModelString(void)
{
    return pModelString;
}

//-----------------------------------------------------------------------------
// Card Status
//-----------------------------------------------------------------------------
void SetCardStatus(UINT32 uiStatus)
{
    guiCardStatus = uiStatus;
}
UINT32 GetCardStatus(void)
{
    return guiCardStatus;
}

//-----------------------------------------------------------------------------
// FS Status
//-----------------------------------------------------------------------------
void SetFSStatus(UINT32 uiStatus)
{
    guiFsStatus = uiStatus;
    debug_err(("[SetFSStatus]%d\r\n", guiFsStatus));
}
UINT32 GetFSStatus(void)
{
    debug_err(("[GetFSStatus]%d\r\n", guiFsStatus));
    return guiFsStatus;
}
const CHAR *GetFSStatusString(UINT32 uiStatus)
{
    if(uiStatus < FS_STATUS_TOTAL_NUM)
        return strFSStatusTable[uiStatus];
    else
        return NULL;
}

//-----------------------------------------------------------------------------
// Battery Level
//-----------------------------------------------------------------------------
void SetBatteryLevel(UINT32 uiStatus)
{
    guiBatteryLevel = uiStatus;
}
UINT32 GetBatteryLevel(void)
{
    return guiBatteryLevel;
}

//-----------------------------------------------------------------------------
// MP3 FileTable Renew
//-----------------------------------------------------------------------------
void Set_MP3FtbRenewIndex(UINT32 uiIndex)
{
    guiMP3FtbRenewIndex = uiIndex;
}
UINT32 Get_MP3FtbRenewIndex(void)
{
    return guiMP3FtbRenewIndex;
}

//-----------------------------------------------------------------------------
// Audio Volume
//-----------------------------------------------------------------------------
void SetAudioVolume(UINT32 uiVolume)
{
    guiAudioVolume = uiVolume;
}
UINT32 GetAudioVolume(void)
{
    return guiAudioVolume;
}

//-----------------------------------------------------------------------------
// Bootloader Info
//-----------------------------------------------------------------------------
void Init_BLInfo(void)
{
    memcpy((void *)&gBLInfo, (void *)BLINFO_START_ADDR, sizeof(BLINFO_OBJ));

    debug_err(("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n"));
    debug_err(("Bootloader Info\r\n"));
    debug_err(("BL Name     : %s\r\n", gBLInfo.strBLName));
    debug_err(("FW Name     : %s\r\n", gBLInfo.strFWName));
    debug_err(("FWT Name    : %s\r\n", gBLInfo.strFWTName));
    debug_err(("BL Ver      : %s\r\n", gBLInfo.strBLVer));
    debug_err(("BL Date     : %s\r\n", gBLInfo.strBLDate));
    debug_err(("BL Time     : %s\r\n", gBLInfo.strBLTime));
    debug_err(("DRAM Type   : %s\r\n", gBLInfo.strDramType));
    debug_err(("DRAM Size   : %sMb\r\n", gBLInfo.strDramSize));
    debug_err(("Storage Int : %s\r\n", gBLInfo.strStorageInt));
    debug_err(("IsUpdateFW  : %d\r\n", gBLInfo.uiUpdateFwFlag));
    debug_err(("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n"));
}
PBLINFO_OBJ Get_BLInfo(void)
{
    return &gBLInfo;
}

//-----------------------------------------------------------------------------
// Social network data
//-----------------------------------------------------------------------------
void SysSetSocialNetworkData(UINT8 ucData)
{
    sSocialNetworkData[SN_DATA_OFFSET] = ucData;
}

CHAR* SysGetSocialNetworkData(void)
{
    return sSocialNetworkData;
}

UINT32 SysGetSocialNetworkDataSize(void)
{
    return sizeof(sSocialNetworkData);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Validate Storage
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static BOOL UI_Validate_NeedSDClass(void)
{
 BOOL bRet = FALSE;
 UINT32 uiWidth,uiHeight,uiFPS;

   uiWidth = GetMovieSizeWidth(SysGetFlag(FL_MOVIE_SIZE));
   uiHeight = GetMovieSizeHeight(SysGetFlag(FL_MOVIE_SIZE));
   uiFPS = GetMovieFrameRate(SysGetFlag(FL_MOVIE_SIZE));
   if ((uiWidth==1920&&uiHeight==1080)||(uiHeight==720&&uiFPS>30))
   {
    bRet = TRUE;
   }
   return bRet;
}

BOOL UI_Validate_Storage(UINT32 uiCheckItem, UINT32 *pFreeCount)
{
    UINT16 uhFSStatusId = 0;
    UINT32 uiCountTmp = 0;
    BOOL bIsValid = TRUE;
    BOOL bNeedClass6 = FALSE;
    FILE_TSK_DISK_INFO *pDiskInfo;
    PSDIO_MISC_INFORMATION pMiscInfo;

    DEBUG_MSG(("UI_Validate_Storage: Check item = 0x%x\r\n", uiCheckItem));

    FilesysWaitInitFinish(FST_TIME_INFINITE);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    switch(uiCheckItem)
    {
        case STORAGE_CHECK_FULL:
            uhFSStatusId = GetFSStatus();
            if(uhFSStatusId == FS_INIT_OK)
            {
                FST_DCF_INFO *pDCFInfo = FilesysGetDCFInfo();

                if(pDCFInfo->MaxDirID == MAX_DCF_DIR_NUM)
                {
                    //#NT#2010/06/02#Chris Chung -begin
                    //#NT#Use FS API instead of PB API since photo/movie mode will use this funciton.
                    UH dirId = MAX_DCF_DIR_NUM, fileId = MAX_DCF_FILE_NUM, fileFmt = FST_FMT_ANYFORMAT;

                    debug_err(("UI_Validate_Storage: The 999DIR is existing!\r\n"));
                    if(FilesysQueryID(FST_CMD_QUERY_SPECI_ID, &dirId, &fileId, &fileFmt, FALSE, 0) == FST_STA_OK)
                    {
                        debug_err(("UI_Validate_Storage: 999-9999 is existing!\r\n"));
                        bIsValid = FALSE;
                        break;
                    }
                    //#NT#2010/06/02#Chris Chung -end
                }

                //#NT#2008/03/20#Ryan Ho -begin
                //If Directory 999 is existing and ReadOnly, the storage is thought as CARDFULL
                //If Directory 999 is existing and dir name is not 999MEDIA, the storage is thought as CARDFULL
                if(pDCFInfo->MaxDirID == MAX_DCF_DIR_NUM)
                {
                    CHAR FolderName[6], FullFolderName[19];
                    UINT32 uiStatus = 0;
                    UINT8 ucAttrib = 0;

                    FilesysGetDCFFolderName(FolderName, MAX_DCF_DIR_NUM);
                    if(strncmp(FolderName, "MEDIA", 5) != 0)
                    {
                        debug_err(("UI_Validate_Storage: The 999DIR is %s, no valid DCF folder id avaliable!\r\n", FolderName));
                        bIsValid = FALSE;
                        break;
                    }
                    sprintf(FullFolderName, "%s%3d%s\0", "A:\\DCIM\\", MAX_DCF_DIR_NUM, FolderName);

                    uiStatus = FilesysGetAttribByName(FullFolderName ,&ucAttrib);
                    if(ucAttrib & FS_ATTRIB_READ)
                    {
                        debug_err(("UI_Validate_Storage: %s is READONLY.\r\n", FullFolderName));
                        bIsValid = FALSE;
                        break;
                    }
                }
                //#NT#2008/03/20#Ryan Ho -end

                /* Check storage free space */
                switch(Primary_GetCurrentMode())
                {
                    case PRIMARY_MODE_PHOTO:
                        /* Get current free picture number */
                        uiCountTmp = Get_FreePicNum(SysGetFlag(FL_PHOTO_SIZE), SysGetFlag(FL_QUALITY));
                        if(uiCountTmp == PHOTO_PIC_NUM_MIN)
                        {
                            debug_err(("UI_Validate_Storage: STORAGE FULL!\r\n"));
                            bIsValid = FALSE;
                        }
                        break;

                    case PRIMARY_MODE_MOVIE:
                        /* Get current free recording time */
                        #if (_MEDIA_REC_ == _APPMEDIAREC_)
                            MediaRec_ChangeParameter(MEDIAREC_RECPARAM_PWROFFPT, 1, 0, 0);
                            MediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_INDEXBUF,0);
                            uiCountTmp = MediaRec_Disk2SecondWithType(MEDIAREC_COUNTTYPE_FS,0);//AVIRec_Disk2Second();
                        #else
                            uiCountTmp = MovRec_Disk2Second();//AVIRec_Disk2Second();
                        #endif
                        if(uiCountTmp < MOVIE_REC_TIME_MIN)
                        {
                            debug_err(("UI_Validate_Storage: STORAGE FULL!\r\n"));
                            bIsValid = FALSE;
                        }
                        break;

                    default:
                        break;
                }

                /* Update specific free count variable */
                if(pFreeCount)
                    *pFreeCount = uiCountTmp;
            }
            else
            {
                debug_err(("UI_Validate_Storage: STORAGE ERR! - %s (0x%x)\r\n", GetFSStatusString(uhFSStatusId), uhFSStatusId));
            }
            break;

        case STORAGE_CHECK_LOCKED:
            //#NT#2011/03/25#Chris Chung -begin
            //#NT#remove useless DCIM check
            #if 0
            if((GetCardStatus() == CARD_LOCKED) || (IsDCIMWritable() == FALSE))
            #else
            if(GetCardStatus() == CARD_LOCKED)
            #endif
            //#NT#2011/03/25#Chris Chung -end
            {
                debug_err(("UI_Validate_Storage: CARD LOCKED!\r\n"));
                bIsValid = FALSE;
            }
            break;

        case STORAGE_CHECK_ERROR:
            if (GetCardStatus() == CARD_INSERTED)
            {
                pMiscInfo = sdio_getMiscInfo();
                if ((pMiscInfo->cardType == SDIO_CARD_UNKNOWN) || (pMiscInfo->cardType == SDIO_CARD_MMC))
                {
                    bIsValid = FALSE;
                    break;
                }
            }

            uhFSStatusId = GetFSStatus();
            if(uhFSStatusId != FS_INIT_OK)
            {
                debug_err(("UI_Validate_Storage: STORAGE ERR! - %s (0x%x)\r\n", GetFSStatusString(uhFSStatusId), uhFSStatusId));
                bIsValid = FALSE;
            }
            break;

        case STORAGE_CHECK_SPACE_FULL:

            pDiskInfo = FilesysGetDiskInfo();

            if(pDiskInfo->FreeSpaceSizeHi == 0 &&
               pDiskInfo->FreeSpaceSize < (pDiskInfo->ClusterSize*STORAGE_RESERVED_CLUS_NUM))
            {
                bIsValid = FALSE;
            }
            break;

        case STORAGE_CHECK_CLUSTER:
            if (GetCardStatus() == CARD_INSERTED)
            {
                pDiskInfo = FilesysGetDiskInfo();
                if (pDiskInfo->ClusterSize < STORAGE_VALID_CLUSTER_SIZE) // card's cluster size is smaller than 32KB
                {
                    bIsValid = FALSE;
                }
            }
            break;

        case STORAGE_CHECK_SDCLASS4:
            if (GetCardStatus() == CARD_INSERTED)
            {
                pMiscInfo = sdio_getMiscInfo();
                debug_msg("^RsdClass:%d\r\n",pMiscInfo->uiWriteRate);
                if (pMiscInfo->uiWriteRate<4)
                {
                    bIsValid = FALSE;
                }
            }
            break;

        case STORAGE_CHECK_SDCLASS6:
            if (GetCardStatus() == CARD_INSERTED)
            {

                bNeedClass6 = UI_Validate_NeedSDClass();
                pMiscInfo = sdio_getMiscInfo();
		 debug_msg("^RsdClass-------------henry:%d\r\n",pMiscInfo->uiWriteRate);
                if (bNeedClass6 && (pMiscInfo->uiWriteRate<6))
                {
                    bIsValid = FALSE;
                }
            }
            break;

        default:
            debug_err(("UI_Validate_Storage: Unknown storage check item 0x%x\r\n", uiCheckItem));
            break;
    }

    return bIsValid;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Init Menu Info
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void SysExeMenuSettingFuncs(void)
{
    /*--- Photo ---*/
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_QUALITY,        1,  SysGetFlag(FL_QUALITY));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_ISO,            1,  SysGetFlag(FL_ISO));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_EV,             1,  SysGetFlag(FL_EV));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_WB,             1,  SysGetFlag(FL_WB));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_COLOR,          1,  SysGetFlag(FL_COLOR_EFFECT));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_METERING,       1,  SysGetFlag(FL_METERING));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SHARPNESS,      1,  SysGetFlag(FL_SHARPNESS));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SATURATION,     1,  SysGetFlag(FL_SATURATION));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_FD,             1,  SysGetFlag(FL_FD));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_ANTISHAKING,    1,  SysGetFlag(FL_ANTISHAKE));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_QUICKREVIEW,    1,  SysGetFlag(FL_QUICK_REVIEW));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_DATEIMPRINT,    1,  SysGetFlag(FL_DATE_STAMP));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SIZE,           1,  SysGetFlag(FL_PHOTO_SIZE));
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SCENEMODE,      1,  SysGetFlag(FL_SCENE));
    //Ux_SendEvent(&UIPhotoObjCtrl,   NVTEVT_EXE_SOCIALNETWORK,   1,  SysGetFlag(FL_PHOTO_VIVILINK));
    Ux_SendEvent(&UIPhotoObjCtrl,   NVTEVT_EXE_SOCIALNETWORK,   1,  PHOTO_VIVILINK_NONE);

    /*--- Movie ---*/
    
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOVIESIZE,1, MOVIE_SIZE_VGA);
    Ux_SendEvent(&UIMovieObjCtrl, NVTEVT_EXE_MOVIETIMESHRINK, 1, SysGetFlag(FL_MOVIE_TIMESHRINK));
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOVIESIZE,1,SysGetFlag(FL_MOVIE_SIZE));//ERIC EDIT 2015 09 01
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_QUALITY,        1,  SysGetFlag(FL_MOVIE_QUALITY));
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_COLOR,          1,  SysGetFlag(FL_MOVIE_SIZE));
    //Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_SOCIALNETWORK,   1,  SysGetFlag(FL_MOVIE_VIVILINK));
    Ux_SendEvent(&UIMovieObjCtrl,   NVTEVT_EXE_SOCIALNETWORK,   1,  MOVIE_VIVILINK_NONE);
    Ux_SendEvent(&UIMovieObjCtrl,   NVTEVT_EXE_EV,   1,     SysGetFlag(FL_MOVIE_EV));
    Ux_SendEvent(&UIMovieObjCtrl, NVTEVT_EXE_GSENSOR, 1, SysGetFlag(FL_GSENSOR));
	
    /*--- Setup ---*/
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_LANGUAGE,      1,  SysGetFlag(FL_LANGUAGE));
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_TVMODE,        1,  SysGetFlag(FL_TV_MODE));//ERIC EDIT 0901
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_POWEROFF,      1,  SysGetFlag(FL_AUTO_POWER_OFF));
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_BEEP,          1,  SysGetFlag(FL_BEEP));
   Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_LEDLIGHT,          1,  SysGetFlag(FL_SILENT));//ERIC EDIT 0901
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DISPLAY,       1,  SysGetFlag(FL_LCD_DISPLAY));
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_FREQ,          1,  SysGetFlag(FL_FREQUENCY));
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DATEFORMAT,    1,  SysGetFlag(FL_DATE_FORMAT));
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_OPENING,       1,  SysGetFlag(FL_OPENING_LOGO));
    Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_SENSOR_ROTATE, 1, SysGetFlag(FL_SENSOR_ROTATE));//ERIC EDIT 0901
	
    Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_LCD_AUTOOFF, 1, SysGetFlag(FL_LCD_AUTOOFF));
    //Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_POWEROFF_DELAY, 1, SysGetFlag(FL_POWEROFF_DELAY));
    /*--- Print ---*/

}

void Init_MenuInfo(void)
{
    PPSTORE_SECTION_HANDLE  pSection;

    // Read system info from PStore
    if (SysInit_GetPStoreStatus() == E_OK)
    {
        if ((pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDONLY)) != E_PS_SECHDLER)
    {
        PStore_ReadSection((UINT8 *)&gCurrentMenuInfo, 0, sizeof(UIMenuStoreInfo), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        debug_err(("[Init_MenuInfo][PStore Read sys param fail]\r\n"));
    }
    }

    // Check FW Version
    //#NT#2011/10/04#KS Hung -begin
    if((strncmp(gCurrentMenuInfo.strFwVersion, GetVersionString(), MAX_VER_STR_LEN) != 0) ||
        (gCurrentMenuInfo.uiInfoSize != sizeof(gCurrentMenuInfo)))
    {
        // If different, do system reset
        SysReset_MenuInfo();
    }
    else if (gBLInfo.uiUpdateFwFlag == TRUE)
    {
        // If update FW, do system reset
        SysReset_MenuInfo();
    }
    else
    {
        SysCheckFlag();             // check flag (limit value)
        SysSetFixedFlag();          // set fixed flag
        SysExeMenuSettingFuncs();   // execute menu setting related functions
        #if (_TouchPanel_ != DISABLE)
        Gesture_SetCalData(&(gCurrentMenuInfo.GestureCaliData));
        #endif
    }
    //#NT#2011/10/04#KS Hung -begin
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Save Menu Info
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Save_MenuInfo(void)
{
    PSTORE_SECTION_HANDLE*  pSection;

    if (SysInit_GetPStoreStatus() != E_OK)
    {
        return;
    }

    /*--- General ---*/
    strncpy(gCurrentMenuInfo.strFwVersion, GetVersionString(), (MAX_VER_STR_LEN - 1));
    gCurrentMenuInfo.strFwVersion[strlen(gCurrentMenuInfo.strFwVersion)] = '\0';
    gCurrentMenuInfo.uiInfoSize = sizeof(UIMenuStoreInfo);

    /*--- TouchPanel ---*/
    #if (_TouchPanel_ != DISABLE)
    Gesture_GetCalData(&(gCurrentMenuInfo.GestureCaliData));
    #endif

    if ((pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        if (PStore_WriteSection((UINT8 *)&gCurrentMenuInfo, 0, sizeof(UIMenuStoreInfo), pSection) != E_PS_OK)
        {
            debug_err(("[Save_MenuInfo]:Write PStore failed\r\n"));
        }
        PStore_CloseSection(pSection);
    }
    else
    {
        debug_err(("[Save_MenuInfo]:Open PStore failed\r\n"));
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Reset Menu Info to Default
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SysReset_MenuInfo(void)
{
    SysResetFlag();             // reset system flags
    //SysSetZHCarNoStamp(CAR_NUMBER_STAMP_STR);
    SysSetNZHCarNoStamp(CAR_NUMBER_STAMP_STR);//henry 20141214
    SysExeMenuSettingFuncs();   // execute menu setting related functions

    /* After reset, save to NAND */
    Save_MenuInfo();
}

