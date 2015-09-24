/*
    Copyright   Novatek Microelectronics Corp. 2006.  All rights reserved.

    @file       StrgDef.h

    @brief      Definitions for storage.
                Definitions for storage (NAND, CF, SMC, xD...)

    @note       Nothing.

    @version    V1.00.000
    @date       2006/10/30
*/

#ifndef _STRGDEF_H
#define _STRGDEF_H

#include "Type.h"

/*==================================================================
* Storage Status information
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*  |1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|1|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
*  |F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|F|E|D|C|B|A|9|8|7|6|5|4|3|2|1|0|
*  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*   | | | | | | |   |                               | | | | | | | |
*   | | | | | | |   |                               | | | | | | | *--> 1 = Storage detached / 0 = Storage attached
*   | | | | | | |   |                               | | | | | | *----> 1 = Storage device error
*   | | | | | | |   |                               | | | | | *------> 1 = Storage full
*   | | | | | | |   |                               | | | | *--------> 1 = Host DMA error
*   | | | | | | |   |                               | | | *----------> 1 = Storage write fault
*   | | | | | | |   |                               | | *------------> 1 = Storage read fault
*   | | | | | | |   |                               | *--------------> 1 = Host is no buffer for i/o
*   | | | | | | |   |                               *----------------> 1 = Storage is processing
*   | | | | | | |   |
*   | | | | | | |   *------------------------------------------------> 1 = Media Changed. This flage is reported too upper system if the the media is changed.
*   | | | | | | |
*   | | | | | | *----------------------------------------------------> 1 = MS/MSPro card
*   | | | | | *------------------------------------------------------> 1 = SDRAM device
*   | | | | *--------------------------------------------------------> 1 = NOR Flash device
*   | | | *----------------------------------------------------------> 1 = NAND flash    // Dicky modified
*   | | *------------------------------------------------------------> 1 = SD card
*   | *--------------------------------------------------------------> 1 = SMC/xD card
*   *----------------------------------------------------------------> 1 = CF card
*=================================================================*/
#define STORAGE_READY           0x00000000      // Storage device is ready for read/write
#define STORAGE_DETACH          0x00000001      // Storage detached
#define STORAGE_DEVICE_ERROR    0x00000002      // Storage device error
#define STORAGE_FULL            0x00000004      // Storage full
#define STORAGE_DMA_ERROR       0x00000008      // DMA access error
#define STORAGE_WRITE_FAULT     0x00000010      // Storage write fault
#define STORAGE_READ_FAULT      0x00000020      // Storage read fault
#define STORAGE_HOST_NO_BUF     0x00000040      // Host is no buffer for i/o
#define STORAGE_PROCESSING      0x00000080      // Storage is processing
//#NT#2008/08/04#HH Chuang -begin
//#NT#meet SD spec
#define STORAGE_VOLTAGE_ERROR   0x00000100      // Voltage error
#define STORAGE_OPEN_FAIL       0x00000200      // Storage open/init fail
#define STORAGE_LOCK            0x00000400      // Storage device is locked
//#NT#2008/08/04#HH Chuang -end

#define STORAGE_CFC             0x80000000      // CF card
#define STORAGE_SMC             0x40000000      // SMC/xD card
#define STORAGE_SDC             0x20000000      // SD card
#define STORAGE_NAND            0x10000000      // NAND flash
#define STORAGE_FLASH           0x08000000      // NOR Flash device
#define STORAGE_SDRAM           0x04000000      // SDRAM device
#define STORAGE_MSPro           0x02000000      // MS/MS_Pro device
#define STORAGE_SDIO            0x01000000      // SDIO device

#define STORAGE_CHANGED         0x00800000      // Media Change Flag. This flage is reported to upper system if the the media is changed.
//#define STORAGE_WRITEPROT       0x00400000      // Media Write Protect Flag. This flage is reported to upper system if the the media is write protected.
#define STORAGE_TYPE_MASK       0xFF000000      // MSB 7 bits for different storage type
#define STORAGE_STATUS_MASK     0x00FFFFFF      // LSB 24 bits for storage status

// Macro for FAT
#define FAT_ROOT_SECTOR_NUM     32              // Sector number of root area (512 * 32) / 512

/*==========================
   Error Status Definitions
  ==========================*/
#define STRG_ER_NAND                0x00000000
#define STRG_ER_XD                  0x10000000
#define STRG_ER_SMC                 0x20000000
#define STRG_ER_SDMMC               0x30000000
#define STRG_ER_CFC                 0x40000000
#define STRG_ER_MS                  0x50000000
#define STRG_ER_ATA                 0x60000000
#define STRG_ER_RAMDISK             0x70000000
#define STRG_ER_NOR                 0x80000000

#define STRG_ER_TYPE_MASK           0xF0000000      //> MSB 4 bits for different storage type
#define STRG_ER_STATUS_MASK         0x0FFFFFFF      //> LSB 28 bits for storage error status

//#NT#2007/04/25#Ethan Wang -begin
//#add format callback
/*==========================
 * Callback Message ID
 *==========================*/
#define STRG_CB_ID_FORMAT              11   // format the the storage, ref: FST_CMD_FORMAT

/*===================================
    Callback Message Format
    -----------------------
    MessageID : STRG_CB_ID_FORMAT
    Param1    : Total Write Sectors
    Param2    : Written Sectors
    Param3    : reserved

  ===================================
 */
//#NT#2007/04/25#Ethan Wang -end

//#NT#2007/07/13#Ethan Wang -begin
//#add abort command
/*==============================
   External Command Definitions
  ==============================*/
#define STRG_EXT_CMD_ABORT          0x00000001
//#NT#2007/07/13#Ethan Wang -end


/*=================*
 *  DRIVER TABLE   *
 *=================*/

 // Capacity of Storage device
typedef struct
{
    UINT    uiTotalCylinders;               // Total cylinders
    USHORT  uiTotalHeads;                   // Total heads
    USHORT  uiSectorsPerTrack;              // Sectors per track
    ULONG   uiTotalSectors;                 // Total sectors
    USHORT  uiBytesPerSector;               // Bytes per setors
} STRG_CAP, *PSTRG_CAP;

typedef struct
{
    ER          (*Open)(void);
    ER          (*SetIntEN)(BOOL);
    ER          (*RdReservedBlock)(INT8 *, UINT16, UINT16);
    ER          (*WrReservedBlock)(INT8 *, UINT16, UINT16);
    UINT32      (*GetBlkSize)(void);
    UINT32      (*GetPageSize)(void);
    ER          (*Close)(void);

} RSVSTRG_TAB, *PRSVSTRG_TAB;

typedef struct
{
    UINT        uiStrgStatus;               // Card current status
    PSTRG_CAP   pStrgCap;
    ER          (*Open)(void);
    ER          (*WrSectors)(CHAR *, ULONG, ULONG);  // write sectors
    ER          (*RdSectors)(CHAR *, ULONG, ULONG);  // read sectors
    ER          (*Format)(void);
    ER          (*Close)(void);
    ER          (*OpenMemBus)(void);
    ER          (*CloseMemBus)(void);
    void        (*FormatErase)(void);
    ER          (*BlockMapping)(void);
    //#NT#2007/04/04#CL Wang -begin
    //Change return data type of the function
    UINT32      (*GetErrStatus)(void);
    //#NT#2007/04/04#CL Wang -end

    //#NT#2007/03/12#Ethan Wang -begin
    //#for partial reading
    //#NT#2007/04/03#CL Wang -begin
    //#Increase variable length for bulk data access
    UINT32      (*GetValidSectors)(void);        // get valid sector number of last read operation
    //#NT#2007/04/03#CL Wang -end
    //#NT#2007/03/12#Ethan Wang -end
    //#NT#2007/04/25#Ethan Wang -begin
    //#add format callback
    void (*Strg_CB)(UINT32 MessageID, UINT32 Param1, UINT32 Param2, UINT32 Param3);
    //#NT#2007/04/25#Ethan Wang -end
    //#NT#2007/07/13#Ethan Wang -begin
    //#add abort command
    UINT32      (*ExtCmd)(UINT32 ExtCommand);       // external command
    //#NT#2007/07/13#Ethan Wang -end
} STRG_TAB, *PSTRG_TAB;

//#NT#2007/03/13#CL Wang -begin
//#For PStore storage device
typedef struct
{
    void      (*PSSetSize)(UINT32);
    ER        (*PSDrvinit)(void);
    ER        (*PSWRByBytes)(UINT8 *, UINT32, UINT32, UINT16);  // write sectors
    ER        (*PSRDByBytes)(UINT8 *, UINT32, UINT32, UINT16);  // read sectors
    void      (*PSErase)(void);
//#NT#2007/05/21#CL Wang -begin
//#Support more sections than 256
    ER        (*PSSetSectionHead)(UINT16, UINT8 *);
    ER        (*PSGetSectionHead)(UINT16, UINT8 *);
//#NT#2007/05/21#CL Wang -end
    ER        (*PSBlockMapping)(void);
    ER        (*PSPrepareTable)(UINT16);

//#NT#2007/04/03#CL Wang -begin
    ER        (*PSReleaseSpace)(UINT16, UINT32, UINT32);
//#NT#2007/04/03#CL Wang -end

//#NT#2007/04/04#CL Wang -begin
    PSTRG_TAB (*PSGetStrgObj)(void);
//#NT#2007/04/04#CL Wang -end

//#NT#2009/06/26#Brad Chen -begin
    ER        (*PSCheckHeader)(UINT32,UINT32 *);
//#NT#2009/06/26#Brad Chen -end

//#NT#2009/06/26#Brad Chen -begin
    ER        (*PSEnUpdateHeader)(UINT32);
//#NT#2009/06/26#Brad Chen -end

//#NT#2009/06/26#Brad Chen -begin
    ER        (*PSRestoreHeader)(UINT8 *, UINT32);
//#NT#2009/06/26#Brad Chen -end

//#NT#2009/07/01#Brad Chen -begin
    ER        (*PSCheckSingleSection)(UINT32,UINT32 *);
//#NT#2009/07/01#Brad Chen -end
//#NT#2010/06/21#Cliff Lin -begin
//#NT# Add new API to check inter mapping table
    ER        (*nandps_checkFreeBlockQueue)(void);
    ER        (*nandps_checkMappingTable)(void);
//#NT#2010/06/21#Cliff Lin -end
//#NT#2010/06/30#Cliff Lin -start
    ER        (*nandps_checkUsedSectionLogBlk)(void);
//#NT#2010/06/30#Cliff Lin -end
}PSSTRG_TAB, *PPSSTRG_TAB;
//#NT#2007/03/13#CL Wang -end

#endif
