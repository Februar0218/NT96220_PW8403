#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "Debug.h"
#include "PlaybackTsk.h"
#include "PrimaryTsk.h"
#include "SieCapTsk.h"
#include "FilesysTsk.h"
#include "KeyScanTsk.h"
#include "GlobalVar.h"
#include "fd_lib.h"
#include "KeyScanTsk.h"
#include "Display.h"
#include "UIMenuWndSetupCommonCarNumber.h"
/////////////////////////////////////////////////////////////////////////////////////////
// Photo/Movie mode         Playback mode            PictBridge mode
// ----------------------   ----------------------   ----------------------
// | POOL_ID_FB         |   | POOL_ID_FB         |   | POOL_ID_FB         |
// ----------------------   ----------------------   ----------------------
// | POOL_ID_CAPTURE    |   | POOL_ID_CAPTURE    |   | POOL_ID_CAPTURE    |
// ----------------------   |                    |   |                    |
// | FD/SD Buffer       |   |                    |   |                    |
// ----------------------   |                    |   ----------------------
// | Date Stamp Primary |   |                    |   | PTP Object Buffer  |
// ----------------------   |                    |   ----------------------
// | Date Stamp Thumb   |   |                    |   | SICD Buffer        |
// ----------------------   ----------------------   ----------------------
// | Display Rotate     |   | Display Rotate     |   | Display Rotate     |
// ----------------------   ----------------------   ----------------------
// | RAM Disk           |   | RAM Disk           |   | RAM Disk           |
// ----------------------   ----------------------   ----------------------
/////////////////////////////////////////////////////////////////////////////////////////

int SysGetCaptureAddr(UINT32 *addr)
{
    int err = get_blk((VP)addr, POOL_ID_CAPTURE);
    if (err == E_OK)
        rel_blk(POOL_ID_CAPTURE, (VP)*addr);
    //debug_msg("[SysGetCaptureAddr]: addr = 0x%x\r\n", *addr);
    return (err);
}

int SysGetRamDiskAddr(UINT32 *addr)
{
    *addr = END_MEM - POOL_SIZE_RAM_DISK;
    //debug_msg("[SysGetRamDiskAddr]: addr = 0x%x\r\n", *addr);
    return E_OK;
}

UINT32 SysGetDispRotateBuffSize(void)
{
    // YUV420, 2 buffers
//    UINT32 uiBuffSize = (g_LCDSize.uiWidth * g_LCDSize.uiHeight * 7) >> 1;
//    UINT32 uiBuffSize = 320*360 * 3;
    #if (_LCDTYPE_ == _LCDTYPE_LIL9341_)
    UINT32 uiBuffSize = 480*272 * 3;
    #else
    UINT32 uiBuffSize = 0;
    #endif
    debug_msg("SysGetDispRotateBuffSize() 0x%x\r\n", uiBuffSize);
    return uiBuffSize;
}
int SysGetDispRotateAddr(UINT32 *addr)
{
    if (GPIOMap_DetStrgCard() == FALSE)
        *addr = END_MEM - POOL_SIZE_RAM_DISK - SysGetDispRotateBuffSize();
    else
        *addr = END_MEM  - SysGetDispRotateBuffSize();
    debug_msg("^R[SysGetDispRotateAddr]: addr = 0x%x\r\n", *addr);
    return E_OK;
}

int SysGetSICDAddr(UINT32 *addr)
{
    //#NT#2011/08/24#KS Hung -begin
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_SICD - SysGetDispRotateBuffSize();
    }
    else
    {
        *addr = END_MEM - POOL_SIZE_SICD- SysGetDispRotateBuffSize();
    }
    return E_OK;
}

int SysGetPTPObjectAddr(UINT32 *addr)
{
    //#NT#2011/08/24#KS Hung -begin
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_SICD - POOL_SIZE_PTP_OBJECT - SysGetDispRotateBuffSize();
    }
    else
    {
        *addr = END_MEM - POOL_SIZE_SICD - POOL_SIZE_PTP_OBJECT - SysGetDispRotateBuffSize();
    }
    //#NT#2011/08/24#KS Hung -end
    return E_OK;
}

int SysGetFdSdAddr(UINT32 *addr)
{
#if (_FD_FUNCTION_ == ENABLE)
    //#NT#2011/08/24#KS Hung -begin
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_DATE_STAMP_THUMB - POOL_SIZE_DATE_STAMP - fd_GetWorkingBufSize() - SysGetDispRotateBuffSize();
    }
    else
    {
        *addr = END_MEM - POOL_SIZE_DATE_STAMP_THUMB - POOL_SIZE_DATE_STAMP - fd_GetWorkingBufSize() - SysGetDispRotateBuffSize();
    }
    //#NT#2011/08/24#KS Hung -end
#endif
    return E_OK;
}

int SysGetDateStampAddr(UINT32 *addr)
{
    //#NT#2011/08/24#KS Hung -begin
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_DATE_STAMP_THUMB - POOL_SIZE_DATE_STAMP - SysGetDispRotateBuffSize();
    }
    else
    {
        *addr = END_MEM - POOL_SIZE_DATE_STAMP_THUMB - POOL_SIZE_DATE_STAMP - SysGetDispRotateBuffSize();
    }
    //#NT#2011/08/24#KS Hung -end
    return E_OK;
}

int SysGetDateStampThumbAddr(UINT32 *addr)
{
    //#NT#2011/08/24#KS Hung -begin
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_DATE_STAMP_THUMB - SysGetDispRotateBuffSize();
    }
    else
    {
        *addr = END_MEM - POOL_SIZE_DATE_STAMP_THUMB - SysGetDispRotateBuffSize();
    }
    //#NT#2011/08/24#KS Hung -end
    return E_OK;
}

int SysGetPhotoEndAddr(UINT32 *addr)
{
    //#NT#2011/08/24#KS Hung -begin
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_DATE_STAMP_THUMB - POOL_SIZE_DATE_STAMP - SysGetDispRotateBuffSize();
    }
    else
    {
        *addr = END_MEM - POOL_SIZE_DATE_STAMP_THUMB - POOL_SIZE_DATE_STAMP - SysGetDispRotateBuffSize();
    }
    //#NT#2011/08/24#KS Hung -end
    return E_OK;
}

int SysGetMovieDSAddr(UINT32 *addr)//get movie recording data stamp temp buffer address
{
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - POOL_SIZE_MOVIE_DATE_STAMP - SysGetDispRotateBuffSize();
    } else {
        *addr = END_MEM - POOL_SIZE_MOVIE_DATE_STAMP - SysGetDispRotateBuffSize();
    }
    return E_OK;
}
int SysGetMovieDSAddr_2nd(UINT32 *addr)//get movie recording data stamp temp buffer address
{
    if (GPIOMap_DetStrgCard() == FALSE)
    {
        *addr = END_MEM - POOL_SIZE_RAM_DISK - (POOL_SIZE_MOVIE_DATE_STAMP)/2 - SysGetDispRotateBuffSize();
    } else {
        *addr = END_MEM - (POOL_SIZE_MOVIE_DATE_STAMP)/2 - SysGetDispRotateBuffSize();
    }
    return E_OK;
}

UINT32 Get_BeepValue(void)
{
    switch(SysGetFlag(FL_BEEP))
    {
        case BEEP_OFF:
            return FALSE;
        case BEEP_ON:
            return TRUE;
    }
    return FALSE;
}

UINT32 Get_PowerOffValue(void)
{
    switch(SysGetFlag(FL_AUTO_POWER_OFF))
    {
        case AUTO_POWER_OFF_3MIN:
            return 180;
        case AUTO_POWER_OFF_5MIN:
            return 300;
        case AUTO_POWER_OFF_DISABLE:
            return KEYSCAN_AUTOPOWEROFF_DISABLED;
    }
    return KEYSCAN_AUTOPOWEROFF_DISABLED;
}
UINT32 Get_LCDOffValue(UINT32 uiIndex)
{
    switch(SysGetFlag(FL_LCD_AUTOOFF))
    {
        case LCD_AUTO_POWER_OFF_1MIN:
            return 60;
        case LCD_AUTO_POWER_OFF_2MIN:
            return 120;
		case LCD_AUTO_POWER_OFF_DISABLE:
	  default:
	  	return 0;
    }
}

UINT32 Get_FreqValue(void)
{
    switch(SysGetFlag(FL_FREQUENCY))
    {
        case FREQUENCY_50HZ:
            return _POWERFreq_50Hz;
        case FREQUENCY_60HZ:
            return _POWERFreq_60Hz;
    }
    return _POWERFreq_60Hz;
}

UINT32 Get_LanguageValue(UINT32 uiIndex)
{
    switch(uiIndex)
    {
        case LANG_EN:
            return (UINT32)&gDemoKit_String_EN;
        case LANG_DE:
            return (UINT32)&gDemoKit_String_DE;
        case LANG_FR:
            return (UINT32)&gDemoKit_String_FR;
        case LANG_ES:
            return (UINT32)&gDemoKit_String_ES;
        case LANG_IT:
            return (UINT32)&gDemoKit_String_IT;
        case LANG_TC:
            return (UINT32)&gDemoKit_String_TC;
        case LANG_SC:
            return (UINT32)&gDemoKit_String_SC;
        case LANG_RU:
            return (UINT32)&gDemoKit_String_RU;
        case LANG_JP:
            return (UINT32)&gDemoKit_String_JP;
	case LANG_PO:
            return (UINT32)&gDemoKit_String_PO;
	case LANG_KR:
            return (UINT32)&gDemoKit_String_KR;
    }
    return (UINT32)&gDemoKit_String_SC;
}
///////////////////////////////////////////////////////////////////////

INT32 OnExe_Format(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_BackgroundPostEvent(NVTEVT_BKW_FORMAT);
    return NVTEVT_CONSUME;
}

INT32 OnExe_Beep(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    KeyScan_EnableKeyTone(Get_BeepValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_Language(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    //SysSetFlag(FL_LANGUAGE, uiSelect);
    debug_ind(("[OnExe_Language][selected = %d] %x \r\n", uiSelect, Get_LanguageValue(uiSelect)));
    GxGfx_SetStringTable((const STRING_TABLE*)Get_LanguageValue(uiSelect));
    /*if(SysGetFlag(FL_LANGUAGE)!=LANG_SC)//henry 20141215
    {
        SysSetNZHCarNoStamp(CAR_NUMBER_STAMP_STR);
    }*/
    return NVTEVT_CONSUME;
}

INT32 OnExe_TVMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if ((KeyScan_IsTVPlugIn() == TRUE) && (KeyScan_GetCurrentTVMode() != SysGetFlag(FL_TV_MODE)))
    {
        KeyScan_ChangeTVMode();
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_PowerOff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    KeyScan_SetAutoPoweroffTime(Get_PowerOffValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_SysReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    SysReset_MenuInfo();

    return NVTEVT_CONSUME;
}

INT32 OnExe_NumReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    PFILE_TSK_DCF_PARAM pFileParam;

    if(paramNum>0)
    {
        uiSelect= paramArray[0];
    }

//#NT#2011/05/31#Scottie -begin
//#NT#CANNOT call the internal API of PB, use FileSystem API to replace it
//    if((PB_ReadCurrFileSysInfo(PLAYFILEINFO_MAXDIRID) == MIN_DCF_DIR_NUM) && (PB_ReadCurrFileSysInfo(PLAYFILEINFO_DIRNUMS) == 0) && (PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILENUMS) == 0))
    pFileParam = FilesysGetFSTParam();
    if((pFileParam->MaxDirID == MIN_DCF_DIR_NUM) && (pFileParam->TolDirNum == 0) && (pFileParam->TolFileNum == 0))
    {
        SysSetFlag(FL_DCF_DIR_ID, MIN_DCF_DIR_NUM);
    }
    else
    {
//        SysSetFlag(FL_DCF_DIR_ID, PB_ReadCurrFileSysInfo(PLAYFILEINFO_MAXDIRID)+1);
        SysSetFlag(FL_DCF_DIR_ID, pFileParam->MaxDirID + 1);
    }
//#NT#2011/05/31#Scottie -end

    SysSetFlag(FL_DCF_FILE_ID, MIN_DCF_FILE_NUM);
    debug_msg("OnExe_NumReset(), DirNum = %d, FileNum = %d\r\n", SysGetFlag(FL_DCF_DIR_ID), SysGetFlag(FL_DCF_FILE_ID));
    FilesysSetWriteNextFileId(((UINT16)SysGetFlag(FL_DCF_DIR_ID)), ((UINT16)SysGetFlag(FL_DCF_FILE_ID)), TRUE);
    //Save DirNumber & FileNumber
    Save_MenuInfo();

    return NVTEVT_CONSUME;
}

INT32 OnExe_Freq(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImgCapture_SetParameter(_Frequency,(UINT8)Get_FreqValue());
    return NVTEVT_CONSUME;
}

UINT32 Get_LCDBacklightValue()
{
    switch(SysGetFlag(FL_LCD_BACKLIGHT))
    {
        case LCD_BACKLIGHT_OFF:
            return 0;
        case LCD_BACKLIGHT_3MIN:
            return 180;
        case LCD_BACKLIGHT_5MIN:
            return 300;
        case LCD_BACKLIGHT_10MIN:
            return 600;
    }
}

INT32 OnExe_SensorRotate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32        uiSelect = 0;
    PDISP_OBJ     pDispObj = disp_getDisplayObject(DISP_1);
    DISPDEV_PARAM DispDev;
	PrimaryModeID CurMode = Primary_GetCurrentMode();

    if (paramNum > 0)
    {
        uiSelect = paramArray[0];
    }

    SysSetFlag(FL_SENSOR_ROTATE, uiSelect);
    if (SysGetFlag(FL_SENSOR_ROTATE) == SEN_ROTATE_OFF)
    {
        //KeyScan_SetSensorRotate(FALSE);
		if ((CurMode == PRIMARY_MODE_PHOTO) ||(CurMode == PRIMARY_MODE_MOVIE))
		{
	        //UI_SetParameter(_ImageFlip, _SEN_Flip_H_V);
	        UI_SetParameter(_ImageFlip, _Flip_Off);
		}
		//DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_NONE;
            // pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);

            DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_180;
            pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);
#if 0
        ide_setPdir(IDE_PDIR_RGB);
        ide_setOdd(IDE_LCD_R);
        ide_setEven(IDE_LCD_G);
#endif		
    }
    else
    {
        //KeyScan_SetSensorRotate(TRUE);
		if ((CurMode == PRIMARY_MODE_PHOTO) ||(CurMode == PRIMARY_MODE_MOVIE))
		{
		   UI_SetParameter(_ImageFlip, _SEN_Flip_H_V);
	        //UI_SetParameter(_ImageFlip, _Flip_Off);
		}
		
		DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_NONE;
             pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);
#if 0		
        ide_setPdir(IDE_PDIR_RBG);
        ide_setOdd(IDE_LCD_R);
        ide_setEven(IDE_LCD_B);
#endif
    }

    return NVTEVT_CONSUME;
}
/*{
 UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    SysSetFlag(FL_SENSOR_ROTATE, uiSelect);
    return NVTEVT_CONSUME;
}*/

INT32 OnExe_UpdateDCFName(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
 BOOL IsJPG = TRUE;
 CHAR FolderName[6]="MEDIA";
 CHAR FileName[5]="PTDC";
 char DCFpathname[0x20];
 RTC_DATE  curDate;
 RTC_TIME  curTime;
 UINT16 uhFolderId, uhFileId;


    if(paramNum>0)
        IsJPG= (BOOL)paramArray[0]; // IsJPG flag

    FilesysGetDCFNextID(&uhFolderId,&uhFileId);
    debug_err(("--%d,%d---\r\n",uhFolderId,uhFileId));
    SysSetFlag(FL_DCF_DIR_ID, uhFolderId);
    SysSetFlag(FL_DCF_FILE_ID, uhFileId);	
	
    curDate = rtc_getDate();
    curTime = rtc_getTime();
    sprintf(FolderName,"%1d%02d%02d",curDate.s.year%0x0A,curDate.s.month,curDate.s.day);
    sprintf(FileName,"%02d%02d",curTime.s.hour,curTime.s.minute);

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysGetDCFPath((UB *)DCFpathname,TRUE);
    if (strncmp(DCFpathname, FolderName, 5))
    {
        FilesysSetDCFPath((UB *)FolderName, TRUE);
    }
    if (IsJPG==TRUE)
    {
        FilesysSetDCFPath((UB *)FileName, FALSE); // update jpg file name
    } else {
        FilesysSetDCFFilename((INT8 *)FileName, FST_FMT_AVI); // update video file name
    }
}

INT32 OnExe_Bright(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    SysSetFlag(FL_LCD_BRIGHTNESS, uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_LCDAutoOff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];
	
    debug_err(("---OnExe_LCDAutoOff : %d---\r\n",uiSelect));
    SysSetFlag(FL_LCD_AUTOOFF, uiSelect);
    KeyScan_SetAutoLCDoffTime(Get_LCDOffValue(uiSelect));
	
    return NVTEVT_CONSUME;
}

INT32 OnExe_Car_Number(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;
	if(paramNum>0)
	{
		uiSelect= paramArray[0];
	}

	SysSetFlag(FL_PLATENUMBER, uiSelect);
	if (uiSelect == PLATENUMBER_ON)
	{
		Ux_OpenWindow(&UIMenuWndSetupCommonCarNumberCtrl, 1, 0);
	}
	
    	return NVTEVT_CONSUME;
}


UINT32 Get_DelayPowerOffValue(void)
{
	UINT32 rval = 0;

    switch (SysGetFlag(FL_POWEROFF_DELAY))
    {
		case DELAY_POWER_OFF_DISABLE:
			rval = 0;
			break;
		case DELAY_POWER_OFF_1MIN:
			rval = 60;
			break;
		case DELAY_POWER_OFF_2MIN:
			rval = 120;
			break;
		case DELAY_POWER_OFF_3MIN:
			rval = 180;
			break;
		case DELAY_POWER_OFF_5MIN:
			rval = 300;
			break;
		case DELAY_POWER_OFF_10MIN:
			rval = 600;
			break;
		case DELAY_POWER_OFF_15MIN:
			rval = 900;
			break;
    }
    return rval;
}

INT32 OnExe_PowerOffDelay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];
	
    debug_err(("---OnExe_PowerOffDelay : %d---\r\n",uiSelect));
    SysSetFlag(FL_POWEROFF_DELAY, uiSelect);
    //KeyScan_SetDelayShutdownTime(Get_DelayPowerOffValue());
    return NVTEVT_CONSUME;
}

INT32 OnExe_Display(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect = paramArray[0];

    SysSetFlag(FL_LCD_DISPLAY, uiSelect);

    if(uiSelect == DISPLAY_OFFPANEL)
    {
        GPIOMap_TurnOffLCDBacklight();
    }
    else
    {
        if ((KeyScan_IsTVPlugIn() == FALSE)&&(KeyScan_IsHDMIPlugIn() == FALSE))
        {
            GPIOMap_TurnOnLCDBacklight();
        }
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_DateFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    SysSetFlag(FL_DATE_FORMAT, uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_TimeFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];
    SysSetFlag(FL_TIME_FORMAT, uiSelect);
    return NVTEVT_CONSUME;
}


INT32 OnExe_USB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    switch(uiSelect)
    {
  //  case USB_MODE_PICTBRIDGE:
    //    Primary_ChangeMode(PRIMARY_MODE_USBSICD);
    //    break;
    case USB_MODE_MSDC:
        Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
        break;    
    case USB_MODE_PCC:
        Primary_ChangeMode(PRIMARY_MODE_USBPCC);
        break;
    default:
        //Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
        Ux_PostEvent(NVTEVT_KEY_ENTER, 2, NVTEVT_KEY_PRESS, FLGKEY_PRESSED);                
        break;
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_Opening(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    SysSetFlag(FL_OPENING_LOGO, uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_SocialNetwork(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSelect = 0, uiSetMode = 0;
    UINT32  uiCurrMode = Primary_GetCurrentMode();

    if (paramNum)
    {
        uiSelect = paramArray[0];
        if (paramNum > 1)
        {
            uiSetMode = paramArray[1];
        }
    }

    if (uiCurrMode == PRIMARY_MODE_PLAYBACK)
    {
        // change social network index value in AVI/JPEG
        if (uiSetMode == VIVILINK_SET_ALL)
        {
            PLAY_SINGLE_OBJ playSingleObj;

            memset(&playSingleObj, 0, sizeof(PLAY_SINGLE_OBJ));
            playSingleObj.JumpOffset  = 1;
            playSingleObj.PlayCommand = PB_SINGLE_CURR | PB_SINGLE_PRIMARY;
            playSingleObj.MoveImgMode = SLIDE_EFFECT_NONE;

            PB_SetSocialNetworkValue(PLAY_SOCIALNETWORK_SETALL, uiSelect);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);

            PB_PlaySingleMode(&playSingleObj);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
        }
        else
        {
            PB_SetSocialNetworkValue(PLAY_SOCIALNETWORK_SETONE, uiSelect);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
        }
    }

    return NVTEVT_CONSUME;
}

INT32 OnChangeDscMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiType = DSCMODE_CHGTO_NEXT;
    UINT32 uiCurrMode = Primary_GetCurrentMode();
    UINT32 uiChangeTo = 0;

    if(paramNum>0)
        uiType = paramArray[0];

    switch(uiType)
    {
        case DSCMODE_CHGTO_NEXT:
            switch(uiCurrMode)
            {
                case PRIMARY_MODE_PHOTO:
                    uiChangeTo = PRIMARY_MODE_PLAYBACK;
                    break;

                case PRIMARY_MODE_MOVIE:
                    uiChangeTo = PRIMARY_MODE_PHOTO;
                    break;

                case PRIMARY_MODE_PLAYBACK:
                    uiChangeTo = PRIMARY_MODE_MOVIE;
                    break;

                default:
                    debug_err(("UISystemObj: Unknown current DSC mode 0x%x\r\n", uiCurrMode));
                    break;
            }
            break;

        case DSCMODE_CHGTO_PREV:
            uiChangeTo = Primary_GetPreviousMode();
            break;

        case DSCMODE_CHGTO_CURR:
            uiChangeTo = uiCurrMode;
            break;

        default:
            debug_err(("UISystemObj: Unknown change type 0x%x\r\n", uiType));
            break;
    }

    if (KeyScan_IsHDMIPlugIn()==FALSE)
        Primary_ChangeMode(uiChangeTo);

    return NVTEVT_CONSUME;
}

INT32 OnForceToLiveviewMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiCurrMode = Primary_GetCurrentMode();
    UINT32 uiChangeTo = 0;
    VControl *pCloseWnd = NULL;

    switch(uiCurrMode)
    {
        case PRIMARY_MODE_PHOTO:
        case PRIMARY_MODE_MOVIE:
            /* Close all the window upper than root */
            Ux_GeWindowByIndex(&pCloseWnd, UX_2NDLAYER_WND_IDX);
            if(pCloseWnd)
            {
                Ux_CloseWindow(pCloseWnd, 0);
            }
            break;

        case PRIMARY_MODE_PLAYBACK:
            /* Switch to the previous liveview mode before playback mode */
            /* switch to the previous mode only unpluging HDMI */
            if (KeyScan_IsHDMIPlugIn()==FALSE)
            {
                uiChangeTo = Primary_GetPreviousMode();
                Primary_ChangeMode(uiChangeTo);
            }
            break;

        default:
            debug_err(("UISystemObj: Unknown current DSC mode 0x%x\r\n", uiCurrMode));
            break;
    }

    return NVTEVT_CONSUME;
}

INT32 OnForceToPlaybackMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
    return NVTEVT_CONSUME;
}
INT32 OnExe_MenuAnimation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #define ANIMATION_STEP  3

    UINT32  i, width, step, shift;
    Ux_RECT rect, rectbak;

    if (paramNum == 2)
    {
        rect = rectbak = UxCtrl_GetPos((VControl *)paramArray[0]);
        width = rect.x2 - rect.x1 + 1;
        shift = width / ANIMATION_STEP;
        step = (width % ANIMATION_STEP) ? (ANIMATION_STEP - 1) : ANIMATION_STEP;

        if (paramArray[1] == ANIMATION_ON_OPEN)
        {
            rect.x1 -= width;
            rect.x2 -= width;
            for (i = 0; i < step; i++)
            {
                rect.x1 += shift;
                rect.x2 += shift;
                UxCtrl_SetPos((VControl *)paramArray[0], rect);
                Ux_Redraw();
            }

            if (width % ANIMATION_STEP)
            {
                UxCtrl_SetPos((VControl *)paramArray[0], rectbak);
                Ux_Redraw();
            }
        }
        else
        {
            DC* pDC = UI_GetOSDDC();

            for (i = 0; i < step; i++)
            {
                GxGfx_SetShapeColor(CLRID_IDX_TRANSPART, CLRID_IDX_TRANSPART, 0);
                GxGfx_FillRect(pDC, rectbak.x2 - shift * (i + 1) + 1, rectbak.y1, rectbak.x2 - shift * i, rectbak.y2);
                Ux_Redraw();

                rect.x1 -= shift;
                rect.x2 -= shift;
                UxCtrl_SetPos((VControl *)paramArray[0], rect);
                Ux_Redraw();
            }

            if (width % ANIMATION_STEP)
            {
                GxGfx_SetShapeColor(CLRID_IDX_TRANSPART, CLRID_IDX_TRANSPART, 0);
                GxGfx_FillRect(pDC, 0, rect.y1, rect.x2, rect.y2);
                Ux_Redraw();
            }

            UxCtrl_SetPos((VControl *)paramArray[0], rectbak);
        }
    }

    return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////

EVENT_ENTRY UISystemObjCtrlCmdMap[] =
{
    {NVTEVT_EXE_FORMAT,                 OnExe_Format},
    {NVTEVT_EXE_BEEP,                   OnExe_Beep},
    {NVTEVT_EXE_LANGUAGE,               OnExe_Language},
    {NVTEVT_EXE_TVMODE,                 OnExe_TVMode},
    {NVTEVT_EXE_POWEROFF,               OnExe_PowerOff},
    {NVTEVT_EXE_SYSRESET,               OnExe_SysReset},
    {NVTEVT_EXE_NUMRESET,               OnExe_NumReset},
    {NVTEVT_EXE_FREQ,                   OnExe_Freq},
    {NVTEVT_EXE_SENSOR_ROTATE,          OnExe_SensorRotate},
    {NVTEVT_EXE_UPDATE_DCF,             OnExe_UpdateDCFName},
    {NVTEVT_EXE_BRIGHT,                 OnExe_Bright},      
    {NVTEVT_EXE_LCD_AUTOOFF,			OnExe_LCDAutoOff},
    {NVTEVT_EXE_CAR_NUMBER,               OnExe_Car_Number},
    {NVTEVT_EXE_POWEROFF_DELAY,			OnExe_PowerOffDelay},    
    {NVTEVT_EXE_DISPLAY,                OnExe_Display},
    {NVTEVT_EXE_DATEFORMAT,             OnExe_DateFormat},
    {NVTEVT_EXE_TIMEFORMAT,             OnExe_TimeFormat},
    {NVTEVT_EXE_USB,                    OnExe_USB},
    {NVTEVT_EXE_OPENING,                OnExe_Opening},
    {NVTEVT_EXE_SOCIALNETWORK,          OnExe_SocialNetwork},
    {NVTEVT_EXE_MENU_ANIMATION,         OnExe_MenuAnimation},
    {NVTEVT_CHANGE_DSCMODE,             OnChangeDscMode},
    {NVTEVT_FORCETO_LIVEVIEW_MODE,      OnForceToLiveviewMode},
    {NVTEVT_FORCETO_PLAYBACK_MODE,      OnForceToPlaybackMode},
    {NVTEVT_NULL,                       0},  //End of Command Map
};

VControl UISystemObjCtrl={
    "UISystemObj",
    UISystemObjCtrlCmdMap,
    0,
    CTRL_SYS,   // CTRL_SYS
    0,  //data
    0,
    0,
    {0,0,0,0},
    0,
    0
};
