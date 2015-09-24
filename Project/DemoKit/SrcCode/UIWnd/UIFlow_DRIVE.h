#ifndef _UIFLOW_TURNKEY_H
#define _UIFLOW_TURNKEY_H

//UI Display
#include "UIDisplay/D320x240_DRIVE/BGDB/BG_Images.h"
#include "UIDisplay/D320x240_DRIVE/BGDB/BG_Opening.h"
#include "UIDisplay/D320x240_DRIVE/BGDB/screen_160x120.h"

//UI Info
#include "DRIVE/UIInfo/SysFlags.h"
#include "DRIVE/UIInfo/UICfgDefault.h"
#include "DRIVE/UIInfo/UISystemStatusSettings.h"
#include "DRIVE/UIInfo/UIMenuPrintSettings.h"

//UIFlow common
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowCommonIcons.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndFwVersion.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndUSB.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndUSBAPI.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWaitMomentAPI.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnMsgAPI.h"
#include "DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnStrgHotPlug.h"

//UIFlow movie
#include "DRIVE/UIFlow/UIFlowMovie/UIFlowMovieFuncs.h"
#include "DRIVE/UIFlow/UIFlowMovie/UIFlowMovieIcons.h"
#include "DRIVE/UIFlow/UIFlowMovie/UIFlowWndMovie.h"

//UIFlow photo
#include "DRIVE/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.h"
#include "DRIVE/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.h"
#include "DRIVE/UIFlow/UIFlowPhoto/UIFlowWndPhoto.h"

//UIFlow play
#include "DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlay.h"
#include "DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlayNoFile.h"
#include "DRIVE/UIFlow/UIFlowPlay/UIFlowPlayFuncs.h"
#include "DRIVE/UIFlow/UIFlowPlay/UIFlowPlayIcons.h"
#include "DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.h"
#include "DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.h"

//UIFlow printer
#include "DRIVE/UIFlow/UIFlowPrint/UIFlowWndPrint.h"

//UIMenu Common
#include "DRIVE/UIMenu/UIMenuCommon/TabMenu.h"
#include "DRIVE/UIMenu/UIMenuCommon/MenuId.h"
#include "DRIVE/UIMenu/UIMenuCommon/MenuCommonItem.h"   //Item
#include "DRIVE/UIMenu/UIMenuCommon/MenuCommonOption.h" //Option

//UIMenu calibration
#include "DRIVE/UIMenu/UIMenuCalibration/UIMenuWndCalibration.h"

//UIMenu movie
#include "DRIVE/UIMenu/UIMenuMovie/MenuMovie.h"

//UIMenu photo
#include "DRIVE/UIMenu/UIMenuPhoto/MenuPhoto.h"
#include "DRIVE/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.h"
#include "DRIVE/UIMenu/UIMenuPhoto/UIMenuWndPhotoExposure.h"
#include "DRIVE/UIMenu/UIMenuPhoto/UIMenuWndPhotoQuickSetting.h"
#include "DRIVE/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.h"

//UIMenu playback
#include "DRIVE/UIMenu/UIMenuPlay/MenuPlayback.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayCopyToCard.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayDPOF.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayProtect.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayQuickConfirmDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayQuickDel.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySetDPOF.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.h"
#include "DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.h"

//UIMenu printer
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_Confirm.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_Crop.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintAll.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintDPOF.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintProgress.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintSetting.h"
#include "DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_SelectImages.h"

//UIMenu Setup
#include "DRIVE/UIMenu/UIMenuSetup/MenuSetup.h"
#include "DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.h"
#include "DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.h"
#include "DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.h"
#include "DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.h"
#include "DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupLoaderVersion.h"
#include "DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.h"

//UIMenu USB
#include "DRIVE/UIMenu/UIMenuUSB/UIMenuWndUSB.h"
#endif
