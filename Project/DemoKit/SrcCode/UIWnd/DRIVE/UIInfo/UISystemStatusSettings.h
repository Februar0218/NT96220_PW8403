#ifndef UI_SYSTEM_STATUS_SETTING_H
#define UI_SYSTEM_STATUS_SETTING_H

#include "Type.h"
#include "UIFlow.h"
#include "Gesture_Public.h"

// Maximum system flag number
#define MAX_SYS_FLAG_NUM            128

// Firmware version string max length
#define MAX_VER_STR_LEN             32

// Storage paramter
#define STORAGE_RESERVED_CLUS_NUM   4

#define CAR_NUMBER_STAMP_STR			"AAAAAA"

// Storage validation
#define STORAGE_CHECK_FULL          0
#define STORAGE_CHECK_LOCKED        1
#define STORAGE_CHECK_ERROR         2
#define STORAGE_CHECK_SPACE_FULL    3
#define STORAGE_CHECK_CLUSTER       4
#define STORAGE_CHECK_SDCLASS4      5
#define STORAGE_CHECK_SDCLASS6      6

// Definition of min number
#define PHOTO_PIC_NUM_MIN           0   // Minimum acceptable free picture number
#define PANO_PIC_NUM_MIN            3   // Minimum acceptable free picture number in panorama mode
#define MOVIE_REC_TIME_MIN          3   // Minimum acceptable free movie recording time

// Definition valided record cluster size
#define STORAGE_VALID_CLUSTER_SIZE  (0x8000)

//-----------------------------------------------------------------------------
// FS Status
//-----------------------------------------------------------------------------
typedef enum
{
    FS_NOT_INIT = 0,
    FS_INIT_OK,
    FS_UNFORMATTED,
    FS_UNKNOWN_FORMAT,
    FS_DISK_ERROR,
    FS_STATUS_TOTAL_NUM
} FS_STATUS;

//-----------------------------------------------------------------------------
// Card Status
//-----------------------------------------------------------------------------
typedef enum
{
    CARD_REMOVED = 0,
    CARD_INSERTED,
    CARD_LOCKED,
    CARD_STATUS_TOTAL_NUM
} CARD_STATUS;

//-----------------------------------------------------------------------------
// Battery Status
//-----------------------------------------------------------------------------
typedef enum
{
    BATTERY_FULL = 0,
    BATTERY_MED,
    BATTERY_LOW,
    BATTERY_EMPTY,
    BATTERY_EXHAUSTED,
    BATTERY_CHARGE,
    BATTERY_STATUS_TOTAL_NUM
} BATTERY_STATUS;

//-----------------------------------------------------------------------------
// MP3 FileTable Renew
//-----------------------------------------------------------------------------
typedef enum
{
    MP3FTB_RENEW_OFF = 0,
    MP3FTB_RENEW_ON,
    MP3FTB_RENEW_SETTING_MAX
} MP3FTB_RENEW_SETTING;

//-----------------------------------------------------------------------------
// Social Network
//-----------------------------------------------------------------------------
typedef enum
{
    SOCIALNETWORK_NONE = 0,
    SOCIALNETWORK_FACEBOOK=1,
    SOCIALNETWORK_TWITGOO=2,
    SOCIALNETWORK_TWITPIC=3,
    SOCIALNETWORK_PHOTOBUCKET=4,
    SOCIALNETWORK_FLICKR=5,
    SOCIALNETWORK_MYSPACE=6,
    SOCIALNETWORK_PICASA_ALBUMS=7,
    SOCIALNETWORK_YOUTUBE=8,
    SOCIALNETWORK_SETTING_MAX
} SOCIALNETWORK_SETTING;

//-----------------------------------------------------------------------------
// Bootloader Info
//-----------------------------------------------------------------------------
#define BLINFO_DRAMEND_OFFSET       0x00010000      // 64K bytes (32K bytes are fw c runtime heap size)
#define BLINFO_START_ADDR           (DRAM_END - BLINFO_DRAMEND_OFFSET)

#define MAX_BLINFO_BLNAME_LEN       16              // Bootloader bin name, ex: 12345678.BIN
#define MAX_BLINFO_FWNAME_LEN       16              // Firmware bin name, ex: 12345678.BIN
#define MAX_BLINFO_FWTNAME_LEN      16              // Firmware T.bin name, ex: 1234567T.BIN
#define MAX_BLINFO_VER_LEN          16              // Bootloader version, ex: v1.0
#define MAX_BLINFO_DATE_LEN         16              // Bootloader build date, ex: 10/25/2010
#define MAX_BLINFO_TIME_LEN         16              // Bootloader build time, ex: 18:14:52
#define MAX_BLINFO_DRAMTYPE_LEN     16              // Dram type, ex: SDR/DDR
#define MAX_BLINFO_DRAMSIZE_LEN     16              // Dram size, ex: 64/128/256
#define MAX_BLINFO_STORAGEINT_LEN   16              // Internal storage, ex: NandEccHamming/SpiFlash

//#NT#2011/10/04#KS Hung -begin
//#NT#Add the flag of update FW
// Bootloader info object
typedef struct _BLINFO_OBJ
{
    char    strBLName[MAX_BLINFO_BLNAME_LEN];           ///< Bootloader bin name
    char    strFWName[MAX_BLINFO_FWNAME_LEN];           ///< Firmware bin name
    char    strFWTName[MAX_BLINFO_FWTNAME_LEN];         ///< Firmware T.bin name
    char    strBLVer[MAX_BLINFO_VER_LEN];               ///< Bootloader version
    char    strBLDate[MAX_BLINFO_DATE_LEN];             ///< Bootloader build date
    char    strBLTime[MAX_BLINFO_TIME_LEN];             ///< Bootloader build time
    char    strDramType[MAX_BLINFO_DRAMTYPE_LEN];       ///< Dram type
    char    strDramSize[MAX_BLINFO_DRAMSIZE_LEN];       ///< Dram size
    char    strStorageInt[MAX_BLINFO_STORAGEINT_LEN];   ///< Internal storage
    UINT32  uiUpdateFwFlag;
} BLINFO_OBJ, *PBLINFO_OBJ;
//#NT#2011/10/04#KS Hung -end

//-----------------------------------------------------------------------------
// Menu Store Info
//-----------------------------------------------------------------------------
typedef struct _UIMenuUIMenuStoreInfo
{
    /*--- General ---*/
    UINT32      uiSysFlag[MAX_SYS_FLAG_NUM];
    CHAR        ubCarNumber[MAX_VER_STR_LEN];
    CHAR        strZHCarNoStamp[MAX_VER_STR_LEN];
    CHAR        strFwVersion[MAX_VER_STR_LEN];
    UINT32      uiInfoSize;

    /*--- Print ---*/

    /*--- Gesture ---*/
    #if (_TouchPanel_ != DISABLE)
    GESTURE_CALI_DATA_STRUCT    GestureCaliData;
    #endif

    //Rsvd

} UIMenuStoreInfo;

//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern void     SysSetFlag(UINT32 uiFlag, UINT32 uiValue);
extern UINT32   SysGetFlag(UINT32 uiFlag);

extern CHAR     *GetVersionString(void);

extern void     SetCardStatus(UINT32 uiStatus);
extern UINT32   GetCardStatus(void);

extern void     SetBatteryLevel(UINT32 uiStatus);
extern UINT32   GetBatteryLevel(void);

extern void     SetAudioVolume(UINT32 uiVolume);
extern UINT32   GetAudioVolume(void);

extern void     SetFSStatus(UINT32 uiStatus);
extern UINT32   GetFSStatus(void);
extern const CHAR *GetFSStatusString(UINT32 uiStatus);

extern void     Set_MP3FtbRenewIndex(UINT32 uiIndex);
extern UINT32   Get_MP3FtbRenewIndex(void);

extern void     Init_BLInfo(void);
extern PBLINFO_OBJ Get_BLInfo(void);

extern void     SysSetSocialNetworkData(UINT8 ucData);
extern CHAR*    SysGetSocialNetworkData(void);
extern UINT32   SysGetSocialNetworkDataSize(void);

extern char* SysGetNZHCarNoStamp(void);
extern void SysSetNZHCarNoStamp(char* pCustomerStamp);
extern char* SysGetZHCarNoStamp(void);
extern void SysSetZHCarNoStamp(char* pCustomerStamp);

extern BOOL     IsDCIMWritable(void);
extern BOOL     UI_Validate_Storage(UINT32 uiCheckItem, UINT32 *pFreeCount);

extern void     Init_MenuInfo(void);
extern void     Save_MenuInfo(void);
extern void     SysReset_MenuInfo(void);

extern CHAR *GetVersionString(void);
extern CHAR *GetMakerString(void);
extern CHAR *GetModelString(void);



#endif
