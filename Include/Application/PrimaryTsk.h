/**
    Header file of Primary Task

    Exported header file of primary task.

    @file       PrimaryTsk.h
    @ingroup    mIAPPMain
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _PRIMARYTSK_H
#define _PRIMARYTSK_H

#include "OSCommon.h"
#include "StrgDef.h"

/**
    @addtogroup mIAPPMain
*/
//@{

/**
    Mode for Primary_ChangeMode() API
*/
typedef enum
{
    PRIMARY_MODE_UNKNOWN,       ///< No primary mode is selected
    PRIMARY_MODE_PHOTO,         ///< Photo capture mode
    PRIMARY_MODE_PLAYBACK,      ///< Playback mode
    PRIMARY_MODE_SETUP,         ///< Setup mode
    PRIMARY_MODE_USBMSDC,       ///< USB MSDC mode
    PRIMARY_MODE_USBSICD,       ///< USB SICD/Pictbridge mode
    PRIMARY_MODE_USBPCC,        ///< USB PC camera mode
    PRIMARY_MODE_MOVIE,         ///< Movie recording mode
    PRIMARY_MODE_MUSIC,         ///< Music playback mode
    PRIMARY_MODE_DPFMENU,       ///< DPF menu mode
    PRIMARY_MODE_VIDEO,         ///< Video playback mode
//#NT#2011/08/06#KS Hung -begin
//#NT#For ADPK HDCF project
    PRIMARY_MODE_INIT_SETUP,    ///< Init setup after reset default
//#NT#2011/08/06#KS Hung -end
    PRIMARY_MODE_CUSTOM1=16,    ///< Custom mode 1
    PRIMARY_MODE_CUSTOM2,       ///< Custom mode 2
    PRIMARY_MODE_VOICE,         ///< Custom mode 3
    PRIMARY_MODE_BROWSER,       ///< Custom mode 4
    PRIMARY_MODE_USB,           ///< Custom mode 5
    PRIMARY_MODE_MP3,           ///< Custom mode 6
    PRIMARY_MODE_COPY_RAM,      ///< Custom mode 7
    PRIMARY_MODE_ENGINEER,      ///< Custom mode 8
    PRIMARY_MODE_DUMMY,         ///< Custom mode 9
    PRIMARY_MODE_CUSTOM10,      ///< Custom mode 10
    PRIMARY_MODE_CUSTOM11,      ///< Custom mode 11
    PRIMARY_MODE_CUSTOM12       ///< Custom mode 12
} PrimaryModeID;

/**
    Storage type for Primary_ChangeStorage() API
*/
typedef enum
{
    PRIMARY_STGTYPE_UNKNOWN,    ///< No/Not valid storage type is selected
    PRIMARY_STGTYPE_NAND=28,    ///< Storage type is on board NAND flash
    //#NT#2008/03/07#Chris Chung -begin
    //#Support NAND2 storage
    PRIMARY_STGTYPE_NAND2,  ///< Storage type is partition 2 of on board NAND flash
    //#NT#2008/03/07#Chris Chung -end
    PRIMARY_STGTYPE_CARD,       ///< Storage type is card
    PRIMARY_STGTYPE_CARD2   ///< Reserved for twin card slot, but not for current chip revision
} PrimaryStorageID;

/**
    DCF name structure
*/
typedef struct
{
    BOOL    bCreateDir;         ///< Create DCF folder when init File System or not
    CHAR    pDirName[6];        ///< Folder name, 5 characters
    CHAR    pFileName[5];       ///< File name, 4 characters
    CHAR    pVideoName[5];      ///< Video file name, 4 characters
    CHAR    pVideoExt[4];       ///< Video file extension, 3 characters
    CHAR    pAudioName[5];      ///< Audio file name, 4 characters
    CHAR    pAudioExt[4];       ///< Audio file extension, 3 characters
    CHAR    pRAWName[5];        ///< RAW file name, 4 characters
    CHAR    pTIFFName[5];       ///< TIFF file name, 4 characters
    //#NT#2011/04/08#Chris Chung -begin
    //#add custom type
    CHAR    pCustomExt[4];  ///< Custom file extension, 3 characters
    //#NT#2011/04/08#Chris Chung -end
} PRIMARY_DCFNAME, *PPRIMARY_DCFNAME;

/**
    Primary task file system call back prototype
*/
typedef void (*PRIMARY_FSINITCB)(UINT32);


#define PRI_TIME_NO_WAIT            (0)
#define PRI_TIME_INFINITE           (0xFFFFFFFF)

#define PRI_STA_BUSY                (-1)
#define PRI_STA_IDLE                (0)

// Initialize operations
extern void             Primary_InitDCFName(PPRIMARY_DCFNAME pDCFName);
extern void             Primary_InitStorageCard(PSTRG_TAB pStrgObj);

// Storage operations
extern BOOL             Primary_ChangeStorage(PrimaryStorageID StorageID);
//#NT#2008/12/16#Chris Hsu -begin
//#NT#Add the function to wait for change storage done
extern void             Primary_Wait4ChangeStorageDone(void);
//#NT#2008/12/16#Chris Hsu -end
extern PrimaryStorageID Primary_GetChangeStorage(void);
extern PrimaryStorageID Primary_GetStorage(void);
extern PSTRG_TAB        Primary_GetStorageObj(PrimaryStorageID StorageID);
extern void             Primary_SetFSInitCB(PRIMARY_FSINITCB FSInitCB);
//#NT#2008/12/16# philex - begin
//# add this API to get/set permanet storage object in systeminit() function calls
extern void             Primary_SetPerStrgObject(PSTRG_TAB pPriPertStrgObj);
extern PSTRG_TAB        Primary_GetPerStrgObject(void);
//#NT#2008/12/16# philex - end

// Operating mode operations
extern BOOL             Primary_ChangeMode(PrimaryModeID ModeID);
//#NT#2008/12/16#Chris Hsu -begin
//#NT#Add the function to wait for change mode done
extern void             Primary_Wait4ChangeModeDone(void);
//#NT#2008/12/16#Chris Hsu -end
extern PrimaryModeID    Primary_GetChangeMode(void);
extern PrimaryModeID    Primary_GetCurrentMode(void);
extern PrimaryModeID    Primary_GetPreviousMode(void);
extern INT32            Primary_CheckIdle(UINT32 uiTimeOut);
#endif

//@}
