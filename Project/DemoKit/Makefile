
include MakeConfig.txt

#----------------------------------------------------------------------
# set the project name here
#----------------------------------------------------------------------
PRJ_NAME = $(CHIP)Demo

#----------------------------------------------------------------------
# set the scatter file name here
#----------------------------------------------------------------------
SCATTER = $(PRJ_NAME).scf

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../Include \

#----------------------------------------------------------------------
# add/delete the project assembly files here
#----------------------------------------------------------------------
ASM = \
	SrcCode/SysCfg/VersionInfo.s \
	SrcCode/SysCfg/Startup/Loader.s \

#----------------------------------------------------------------------
# add/delete the project C source files here
#----------------------------------------------------------------------
SRC = \
	SrcCode/BuildDateInfo.c \
	SrcCode/CommandTsk.c \
	SrcCode/USBMakerParamInit.c \
	SrcCode/GlobalVar.c \
	SrcCode/SensorFP.c \
	SrcCode/SystemClock.c \
	SrcCode/SystemInfoGet.c \
	SrcCode/qsort.c \
	SrcCode/UIConfig.c \
	SrcCode/SysInit/SystemInit.c \
	SrcCode/SysInit/SystemTask.c \
	SrcCode/SysCfg/SysCfg.c \
	SrcCode/SysCfg/Startup/CopyRest.c \
	SrcCode/SysCfg/Startup/DATKER.c \
	SrcCode/SysCfg/Startup/InitCLibMMU.c \
	SrcCode/SysCfg/Startup/KerInit.c \
	SrcCode/PlaySound/PlaySoundAPI.c \
	SrcCode/PlaySound/PlaySoundTsk.c \
	SrcCode/PlaySound/PlaySoundUtil.c \
	SrcCode/PlaySound/SoundData.c \
	SrcCode/KeyIOScan/KeyScanFlashLight.c \
	SrcCode/KeyIOScan/KeyScanPWR.c \
	SrcCode/KeyIOScan/KeyScanTsk.c \
	SrcCode/KeyIOScan/KeyScanUSB.c \
	SrcCode/KeyIOScan/KeyScanLED.c \
	SrcCode/KeyIOScan/KeyScanLCD.c \
	SrcCode/KeyIOScan/KeyScanSD.c \
	SrcCode/KeyIOScan/KeyScanSound.c \
	SrcCode/KeyIOScan/KeyScanSensor.c \
	SrcCode/KeyIOScan/KeyScanTv.c \
	Srccode/KeyIOscan/KeyScanHDMI.c \
	SrcCode/KeyIOScan/KeyScanAC.c \
	SrcCode/KeyIOScan/KeyScanKey.c \
	SrcCode/KeyIOScan/Interface.c \
	SrcCode/KeyIOScan/KeyScanMisc.c \
	SrcCode/KeyIOScan/KeyScanIrRx.c \
	SrcCode/KeyIOScan/IrRxParser.c \
	SrcCode/KeyIOScan/UserGPIO.c \
	SrcCode/KeyIOScan/$(PCB_DIR)/GPIOMapping.c \
	SrcCode/KeyIOScan/$(PCB_DIR)/KeyADC.c \
	SrcCode/KeyIOScan/$(PCB_DIR)/VoltageDet.c \
	SrcCode/DeviceControl/DeviceCtrl.c \
	SrcCode/DeviceControl/DevCtrlMovie.c \
	SrcCode/DeviceControl/DevCtrlMp3.c \
	SrcCode/DeviceControl/DevCtrlPhoto.c \
	SrcCode/DeviceControl/DevCtrlSetup.c \
	SrcCode/DeviceControl/DevCtrlPlayback.c \
	SrcCode/DeviceControl/DevCtrlUSB.c \
	SrcCode/DeviceControl/DevCtrlCustom.c \
	SrcCode/DeviceControl/DevCtrlDummy.c \
	SrcCode/DeviceControl/DevCtrlInitSetup.c \
	SrcCode/AppInit/AppInitUSB.c \
	SrcCode/AppInit/AppInitAVI.c \
	SrcCode/AppInit/AppInitMp3.c \
	SrcCode/AppInit/AppInitPhoto.c \
	SrcCode/AppInit/AppInitPlayback.c \
	SrcCode/AppInit/AppInitSetup.c \
	SrcCode/AppInit/AppInitInitSetup.c \
	SrcCode/AppInit/AppInitCustom.c \
	SrcCode/AppInit/AppInit.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/CalibrationItem.c \
	SrcCode/Calibration/Sensor/CalSensor.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/MISC/CalMISC.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/UserPStore/UserPStore.c \
	SrcCode/UserEXIF/UserEXIF.c \
	SrcCode/UserPhotoFrame/UserPhotoFrame.c \
	SrcCode/UIApp/UIPhotoObj.c \
	SrcCode/UIApp/UIPlayObj.c \
	SrcCode/UIApp/UIPlayStamp.c \
	SrcCode/UIApp/UIMovieObj.c \
	SrcCode/UIApp/UISystemObj.c \
	SrcCode/UIApp/UIPrintObj.c \
	SrcCode/UIApp/UIBackgroundObj.c \
	SrcCode/UIWnd/UIFlow.c \
	SrcCode/DateImprint/DateStampFont10x16.c \
	SrcCode/DateImprint/DateStampFont12x20.c \
	SrcCode/DateImprint/DateStampFont36x60.c \
	SrcCode/DateImprint/GenDateImprintData.c \
	SrcCode/MSDCVendor/MsdcNvtCb_CustomSi.c \
	SrcCode/MSDCVendor/MsdcNvtCb_Int.c \
	SrcCode/MSDCVendor/MsdcNvtCb_Adj.c \
	SrcCode/MSDCVendor/MsdcNvtCb_File.c \
	SrcCode/MSDCVendor/MsdcNvtCb_IQSim.c \
	SrcCode/MSDCVendor/MsdcNvtManager.c \
	SrcCode/UsbCustomCmd/UsbCustomVendorCmd.c \
	SrcCode/OSDScreenDump/OSDScreenDump.c



ifeq "$(UI_STYLE)" "UI_STYLE_DRIVE"
	SRC += \
	SrcCode/UIWnd/DRIVE/UIInfo/UISystemStatusSettings.c \
	SrcCode/UIWnd/DRIVE/UIInfo/UIMenuPrintSettings.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowMovie/UIFlowCBRecMovie.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowMovie/UIFlowMovieIcons.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowMovie/UIFlowWndMovie.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPhoto/UIFlowCBPhoto.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c \
	SrcCode/UIwnd/DRIVE/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowCBPlayMovie.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowCBWav.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlay.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowPlayIcons.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPlay/UIFlowWndPlayNoFile.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowPrint/UIFlowWndPrint.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowCommon/UIFlowCommonIcons.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowCommon/UIFlowWndWrnStrgHotPlug.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
	SrcCode/UIWnd/DRIVE/UIFlow/UIFlowCommon/UIFlowWndFwVersion.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuMovie/MenuMovie.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPhoto/MenuPhoto.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPhoto/UIMenuWndPhotoExposure.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayProtect.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayCopyToCard.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayDPOF.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlaySetDPOF.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayQuickConfirmDel.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/UIMenuWndPlayQuickDel.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPlay/MenuPlayback.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupLoaderVersion.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupCommonCarNumber.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/UIMenuWndSetupCarNumConfirm.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuSetup/MenuSetup.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_SelectImages.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintAll.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintSetting.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_Confirm.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintProgress.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_Crop.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintDPOF.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuCalibration/UIMenuWndCalibration.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuCommon/MenuCommonItem.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuCommon/MenuCommonOption.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuCommon/MenuCommonEffect.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuCommon/TabMenu.c \
	SrcCode/UIWnd/DRIVE/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
	SrcCode/UIDisplay/D320x240_DRIVE/BGDB/BG_Images.c \
	SrcCode/UIDisplay/D320x240_DRIVE/BGDB/BG_Opening.c \
	SrcCode/UIDisplay/D320x240_DRIVE/BGDB/screen_160x120.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIGraphics.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_Font.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_Image.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_Palette.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_EN.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_DE.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_ES.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_IT.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_FR.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_SC.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_TC.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_JP.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_KR.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_PO.c \
	SrcCode/UIDisplay/D320x240_DRIVE/UIResource/DemoKit_String_RU.c
	
	
endif
ifneq "$(TouchPanel)" "DISABLE"
	SRC += \
	SrcCode/UIWnd/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest.c \
	SrcCode/UIWnd/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest_Cal.c
endif

ifeq "$(LENS_DRV)" "LensDrv_FF"
	SRC += \
	SrcCode/Calibration/Lens/CalLens.c \
	SrcCode/Calibration/Lens/CalLens_FF.c \
	SrcCode/UIApp/Lens/UILensObj.c
endif

ifeq "$(LENS_DRV)" "LensDrv_ST8003"
	SRC += \
	SrcCode/Calibration/Lens/CalLens.c \
	SrcCode/Calibration/Lens/CalLens_ST8003.c \
	SrcCode/UIApp/Lens/UILensObj.c \
	SrcCode/UIApp/Lens/UILensObj_ST8003.c
endif

ifeq "$(LENS_DRV)" "LensDrv_ST8005"
	SRC += \
	SrcCode/Calibration/Lens/CalLens.c \
	SrcCode/Calibration/Lens/CalLens_ST8005.c \
	SrcCode/UIApp/Lens/UILensObj.c \
	SrcCode/UIApp/Lens/UILensObj_ST8005.c
endif

ifeq "$(LENS_DRV)" "LensDrv_A300"
	SRC += \
	SrcCode/Calibration/Lens/CalLens.c \
	SrcCode/Calibration/Lens/CalLens_FF.c \
	SrcCode/UIApp/Lens/UILensObj.c
endif

ifeq "$(LENS_DRV)" "LensDrv_DZ5311"
	SRC += \
	SrcCode/Calibration/Lens/CalLens.c \
	SrcCode/Calibration/Lens/CalLens_DZ5311.c \
	SrcCode/UIApp/Lens/UILensObj.c \
	SrcCode/UIApp/Lens/UILensObj_DZ5311.c
endif

ifeq "$(SENSOR)" "CMOS_MI5130"
	SRC += \
	SrcCode/Calibration/Sensor/Cal_MI5130_220/Cal_MI5130_220.c
else ifeq "$(SENSOR)" "CMOS_OV9712"
	SRC += \
	SrcCode/Calibration/Sensor/Cal_OV9712_220/Cal_OV9712_220.c
else ifeq "$(SENSOR)" "CMOS_MI5131E"
	SRC += \
	SrcCode/Calibration/Sensor/Cal_MI5131E_220/Cal_MI5131E_220.c
else
	SRC += \
	SrcCode/Calibration/Sensor/Cal_Dummy_220/Cal_Dummy_220.c
endif

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(PRJ_NAME)_Data

#----------------------------------------------------------------------
# set the forced rebuild objects
#----------------------------------------------------------------------
FORCED_REBUILD_OBJ = $(PRJ_NAME)_Data/Release/ObjectCode/BuildDateInfo.o

#----------------------------------------------------------------------
# set the library directory here
#----------------------------------------------------------------------
APP_BASE_DIR = ../../Application
APPEX_BASE_DIR = ../../AppExt
LIB_BASE_DIR = ../../LIB
LIBEX_BASE_DIR = ../../LIBExt

#----------------------------------------------------------------------
# add additional C and assembly flags here, for example, -Otime
#----------------------------------------------------------------------
CFLAGS = \
	$(PRJ_PARAM) \
	$(CHIP_PARAM) \
	$(DRAMSIZE_PARAM) \
	$(FREEMEMORYSIZE_PARAM) \
	$(FWCOMPRESS_PARAM) \
	$(LCD_PARAM) \
	$(LCD2_PARAM) \
	$(TOUCH_PANEL_PARAM) \
	$(SENSOR_PARAM) \
	$(LENS_PARAM) \
	$(MOTOR_PARAM) \
	$(AUDIO_PARAM) \
	$(STORAGE_PARAM) \
	$(PSTORE_PARAM) \
	$(IRRX_PARAM) \
	$(HDMI_PARAM) \
	$(SCAN_PARAM) \
	$(FD_PARAM) \
	$(SD_PARAM) \
	$(REDEYE_PARAM) \
	$(SENSOR_EXPOSURE_PARAM) \
	$(FLASH_LIGHT_PARAM) \
	$(GPS_PARAM) \
	$(MSDCVENDOR_PARAM) \
	$(MEDIAREC_PARAM) \
	$(RAM_DISK_PARAM) \
	$(UI_STYLE_PARAM) \
	$(UPDATE_CFG_PARAM)\
	$(DISPLAY_169HD_PARAM)

ASMFLAGS = \
	$(CHIP_PARAM_ASM) \
	$(DRAMSIZE_PARAM_ASM) \
	$(FWCOMPRESS_PARAM_ASM)

CFLAGS_D = \
	$(CFLAGS)

ASMFLAGS_D = \
	$(ASMFLAGS)

CFLAGS_R = \
	$(CFLAGS)

ASMFLAGS_R = \
	$(ASMFLAGS)

GCCFLAGS = \
	$(CFLAGS)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputImg.txt
