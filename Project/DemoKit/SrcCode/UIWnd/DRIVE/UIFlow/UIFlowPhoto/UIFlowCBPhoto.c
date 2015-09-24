#include "Type.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "SystemClock.h"
#include "ImageAlgInfor.h"
#include "KeyScanTsk.h"
#include "UIFlow.h"
#include "FilesysTsk.h"
//#NT#2011/01/06#ethanlau -begin
//#NT#for fpga test
#include "PhotoDisplayTsk.h"
#include "PhotoTsk.h"
//#NT#2011/01/06#ethanlau -end
#include "PlaySoundTsk.h"
#include "PrimaryTsk.h"
#include "movieInterface.h"
#include "GenDateImprintData.h"
#include "Utility.h"
#include "RawEncAPI.h"
#include "DataStamp.h"

#if 0
#include "debug.h"
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

//#NT#2010/06/01#KS Hung -begin
//#NT#For continue_shot
static BOOL gb_FirstContinueShotSound = FALSE;
//#NT#2010/06/01#KS Hung -end

//#NT#2011/07/26#Jeah Yen -begin
void AppInit_AviSetAddr(UINT32 NewAddr);
//#NT#2011/07/26#Jeah Yen -end

/**
  FlowPhoto_ImageCB

  Image (capture) process call back function

  @param uiEventID,pInfo
  @return void
*/
void FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo)
{
    UINT32 uiDateFormat=0;

    switch (uiEventID)
    {
        case ALGMSG_PREVIEW:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_PREVIEW\r\n"));
            SysClk_SetClk(SPEED_ID_PHOTO);
            break;

        case ALGMSG_FOCUS:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_FOCUS\r\n"));
            break;

        case ALGMSG_FOCUSEND:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_FOCUSEND\r\n"));
            break;

        case ALGMSG_CHGOZOOMSTEP:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_CHGOZOOMSTEP (%d)\r\n", pInfo->OzoomIndex));
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_OZOOMSTEPCHG, 0);
            break;

        case ALGMSG_CHGDZOOMSTEP:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_CHGDZOOMSTEP (%d)\r\n", pInfo->DzoomIndex));
            Set_AlgMsgInfo(pInfo);
            if(Get_DZoomResetting() == FALSE)
            {
                Ux_PostEvent(NVTEVT_CB_DZOOMSTEPCHG, 0);
            }
            else
            {
                /* Ignore the msg while process dzoom reset action!! And clear the dzoom reset flag! */
                Set_DZoomResetting(FALSE);
            }
            break;

        case ALGMSG_PREFLASH:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_PREFLASH\r\n"));
            break;

        case ALGMSG_PREFLASHEND:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_PREFLASHEND\r\n"));
            break;

        case ALGMSG_FIRSTRAW:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_FIRSTRAW\r\n"));
            if(SysGetFlag(FL_CONTINUE_SHOT)== CONTINUE_SHOT_ON)
            {
                gb_FirstContinueShotSound = TRUE;
            }

            if (SysGetFlag(FL_BEEP)== BEEP_ON)
            {
                KeyScan_PlaySound(PLAYSOUND_SOUND_KEY_SHUTTER2);
            }
            else if ((SysGetFlag(FL_BEEP)== BEEP_OFF) && (SysGetFlag(FL_SELFTIMER) != SELFTIMER_SINGLE))
            {
                PlaySound_Play(PLAYSOUND_SOUND_KEY_SHUTTER2);
            }

             /* Get date format */
            switch(SysGetFlag(FL_DATE_FORMAT))
            {
            case DATE_FORMAT_DMY:
                uiDateFormat = STAMP_DD_MM_YY;
                break;

            case DATE_FORMAT_MDY:
                uiDateFormat = STAMP_MM_DD_YY;
                break;

            case DATE_FORMAT_YMD:
            default:
                uiDateFormat = STAMP_YY_MM_DD;
                break;
            }

            /* Generate date imprint */
            switch(SysGetFlag(FL_DATE_STAMP))
            {
                case DATE_STAMP_DATE:
                    DateImprint_Setup(
                        STAMP_ON |
                        STAMP_AUTO |
                        STAMP_DATE |
                        STAMP_BOTTOM_LEFT |
                        uiDateFormat,DS_YCCFORMAT_422);
                    break;

                case DATE_STAMP_DATETIME:
                    DateImprint_Setup(
                        STAMP_ON |
                        STAMP_AUTO |
                        STAMP_DATE_TIME |
                        STAMP_BOTTOM_LEFT |
                        uiDateFormat,DS_YCCFORMAT_422);
                    break;

                case DATE_STAMP_OFF:
                default:
                    DateImprint_Setup(STAMP_OFF,DS_YCCFORMAT_422);
                    break;
            }

            SysClk_SetClk(SPEED_ID_CAPTURE);
            break;

        case ALGMSG_QVSTART:
            debug_err(("[FlowPhoto_ImageCB]: ALGMSG_QVSTART\r\n"));
            if(SysGetFlag(FL_CONTINUE_SHOT)== CONTINUE_SHOT_ON)
            {
                UINT32 uiFreePicNum;
                if( UI_Validate_Storage(STORAGE_CHECK_FULL, &uiFreePicNum) == FALSE)
                {
                }
                else
                {
                    if (gb_FirstContinueShotSound == TRUE)
                    {
                        gb_FirstContinueShotSound = FALSE;
                    }
                    else
                    {
                        if (SysGetFlag(FL_BEEP)== BEEP_ON)
                        {
                            KeyScan_PlaySound(PLAYSOUND_SOUND_KEY_SHUTTER2);
                        }
                    }
                }
            }
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_QVSTART, 0);
            break;

        case ALGMSG_JPEGOK:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_JPEGOK\r\n"));
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_JPGOK, 0);
            break;

        case ALGMSG_VIDEO_RECORDING:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_VIDEO_RECORDING\r\n"));
            SysClk_SetClk(SPEED_ID_VIDEO);
            break;

        case ALGMSG_IPL_STOP:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_IPL_STOP\r\n"));
//            if (KeyScan_IsHDMIPlugIn() == TRUE || KeyScan_IsTVPlugIn() == TRUE)
                ide_disable_video(IDE_VIDEOID_1);
            PhotoDisplay_fwctrlBufidx_close();
            break;

        case ALGMSG_PREVIEWSTABLE:
//            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_PREVIEWSTABLE\r\n"));
            debug_err(("[FlowPhoto_ImageCB]: ALGMSG_PREVIEWSTABLE\r\n"));
            if (Primary_GetCurrentMode() != PRIMARY_MODE_USBPCC)
                ide_enable_osd(IDE_OSDID_1);
            if (Primary_GetCurrentMode() != PRIMARY_MODE_USBPCC) // temporary solution to avoid photo display hang up
            {
                //UI_ClearOSD(_OSD_INDEX_TRANSPART);
                ide_enable_osd(IDE_OSDID_1);

                if(pInfo->Alg_Mode==MODE_Video_Recording)
                {
                    PhotoDisplay_fwctrlBufidx_open();
                    PhotoDisplay_SetMode(DISPLAY_MODE_VIDEOREC|DISPLAY_MODE_FWHANDSHAKE, TRUE);
                }
                else if (Primary_GetCurrentMode() == PRIMARY_MODE_MOVIE)
                {
                    PhotoDisplay_fwctrlBufidx_open();
                    PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO|DISPLAY_MODE_FWHANDSHAKE, TRUE);
                }
                else
                {
                    PhotoDisplay_fwctrlBufidx_open();
                    PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW|DISPLAY_MODE_FWHANDSHAKE, TRUE);
                }
                //#NT#2011/09/15#KS Hung -begin
                //This is a black vision effect rather than for any bug.
                TimerDelayMs(100);
                //#NT#2011/09/15#KS Hung -end
                PhotoDisplay_EnableVideo(IDE_VIDEOID_1,TRUE);
            }

            Ux_PostEvent(NVTEVT_CB_PREVIEWSTABLE, 1,ALGMSG_PREVIEWSTABLE);
            break;

        case ALGMSG_JPGEXIFMODIFY:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_JPGEXIFMODIFY\r\n"));
            break;

        case ALGMSG_LENSJAM:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_LENSJAM\r\n"));
            break;

        case ALGMSG_SUSKEY:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_SUSKEY\r\n"));
            KeyScan_Suspend();
            break;

        case ALGMSG_SUSPHOTO:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_SUSPHOTO\r\n"));
            //ide_disable_osd(IDE_OSDID_1);
            break;

        case ALGMSG_SUSFLOWPRV:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_SUSFLOWPRV\r\n"));
            //sus_tsk(FLOWPVIEWTSK_ID);
            break;

        case ALGMSG_RSMKEY:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_RSMKEY\r\n"));
            KeyScan_Resume();
            break;

        case ALGMSG_RSMPHOTO:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_RSMPHOTO\r\n"));
            //rsm_tsk(PHOTOTSK_ID);
            break;

        case ALGMSG_RSMFLOWPRV:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_RSMFLOWPRV\r\n"));
            //rsm_tsk(FLOWPVIEWTSK_ID);
            break;

#if (_FD_FUNCTION_ == ENABLE)
        case ALGMSG_FDEND:
//            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_FDEND\r\n"));
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_FDEND, 0);
            break;
#endif

#if (_SD_FUNCTION_ == ENABLE)
        case ALGMSG_SDEND:
//            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_SDEND\r\n"));
            Ux_PostEvent(NVTEVT_CB_SDEND, 0);
            break;
#endif

        case ALGMSG_CAPFSTOK:
            DEBUG_MSG(("[FlowPhoto_ImageCB]: ALGMSG_CAPFSTOK\r\n"));
            /* reset Face detect num after capturing finished */
            pInfo->FaceDetectNum = 0;
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_CAPFSTOK, 0);
            break;

         //#NT#2011/02/11#ethanlau -begin
        //#NT# video datestamp
        case ALGMSG_VidDateStampCfg:
            Photo_ConfigStamp(pInfo->VidJpg_Hsize, pInfo->VidJpg_Vsize, pInfo->VidStampBuf);
            set_flg(FLG_ID_PHOTO, FLGPHOTO_VIDOPENGRPH);
            break;
        //#NT#2011/02/11#ethanlau -end

        case ALGMSG_VIDEO_RECSTOP:
            break;

        //#NT#2011/06/28#Ethan Lau -begin
        case ALGMSG_IPL_BUFND:
            debug_msg("^GIPL BUF END = 0x%x\r\n",pInfo->IPL_BufEndAddr);
            if (Primary_GetCurrentMode() == PRIMARY_MODE_MOVIE)
            {
                UINT32 uiNewMemAddr, uiNewMemSize;

                debug_msg("^GSET BUF END to AVI\r\n");
                //update new address end to AVI
                //#NT#2011/11/04#KS Hung -begin
                uiNewMemAddr = pInfo->IPL_BufEndAddr;
                if (GPIOMap_DetStrgCard() == FALSE)
                {
                    uiNewMemSize = (INT32)END_MEM - uiNewMemAddr - POOL_SIZE_RAM_DISK - POOL_SIZE_MOVIE_DATE_STAMP - SysGetDispRotateBuffSize();
                }
                else
                {
                    uiNewMemSize = (INT32)END_MEM - uiNewMemAddr - POOL_SIZE_MOVIE_DATE_STAMP - SysGetDispRotateBuffSize();
                }

                #if (_MOVIE_PIM_MODE_ == ENABLE)
                RawEnc_ChangeParameter(INPUT_MEMORYADDR, uiNewMemAddr, 0, 0);
                RawEnc_ChangeParameter(INPUT_MEMORYSIZE, POOL_SIZE_PIM, 0, 0);
                uiNewMemAddr += POOL_SIZE_PIM;
                uiNewMemSize -= POOL_SIZE_PIM;
                #endif

                MovRec_ResetMemoryValue(uiNewMemAddr, uiNewMemSize);//2011/08/02 Meg
                debug_msg("^R %s, memsize:0x%08x\r\n",__func__,uiNewMemSize);
                //#NT#2011/11/04#KS Hung -end
            }
            break;
        //#NT#2011/06/28#Ethan Lau -end
        default:
            break;
    }
}


//#NT#2011/11/28#Philex Lin - begin
/**
  FlowPhoto_SetModeCtrl

  Photo display Set mode control  function

  @param void
  @return void
*/
void     FlowPhoto_SetModeCtrl(void)
{
 AlgMsgInfo *pInfo;

    pInfo = 
Get_AlgMsgInfo();
    if (Primary_GetCurrentMode() != PRIMARY_MODE_USBPCC) // temporary solution to avoid photo display hang up
    {

//        ide_enable_osd(IDE_OSDID_1);

        if(pInfo->Alg_Mode==MODE_Video_Recording)
        {
            PhotoDisplay_fwctrlBufidx_open();
            PhotoDisplay_SetMode(DISPLAY_MODE_VIDEOREC|DISPLAY_MODE_FWHANDSHAKE, TRUE);
        }
        else if (Primary_GetCurrentMode() == PRIMARY_MODE_MOVIE)
        {
            PhotoDisplay_fwctrlBufidx_open();
            PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO|DISPLAY_MODE_FWHANDSHAKE, TRUE);
        }
        else
        {
            PhotoDisplay_fwctrlBufidx_open();
            PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW|DISPLAY_MODE_FWHANDSHAKE, TRUE);
        }
        //#NT#2011/09/15#KS Hung -begin
        //This is a black vision effect rather than for any bug.
        TimerDelayMs(100);
        //#NT#2011/09/15#KS Hung -end
        PhotoDisplay_EnableVideo(IDE_VIDEOID_1,TRUE);
    }
}
//#NT#2011/11/28#Philex Lin - end


