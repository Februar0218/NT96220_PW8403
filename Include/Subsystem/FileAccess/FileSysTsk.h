///////////////////////////////////////////////////////////////////
/**
    Copyright (C) 2003 By Novatek Microelectronics Corp.

    @file   FileSysTsk.h
    @ingroup mISYSFile
    @brief  Provide the DCF compatible file accessing.
        The file system task provides provide The DCF Compatible file accessing.
        It also provide some useful funcions like copy files from Flash to SD card,
        rename the DCF file name, format and so on. File system task is a independent task,
        Through the file system, it will call low level storage driver to acceess storage.
        If there are errors occured and function call will never return(This situation may happen while
        card error). we can close file system task and open again to re-initialize the file system.

*/


#ifndef _FILESYS_TSK_H
#define _FILESYS_TSK_H

#include "fs_file_op.h"

//!!!!!!! Warning !!!!!!!!
//The flag patterns should be moved to internal header file, don't use these any more
// --------------------------------------------------------------------
// Flag patterns
// --------------------------------------------------------------------

//#NT#2009/05/19#Brad Chen -begin
//# Change Filesys Flag From UINT32 to UINT64 for NT96630 Code Base
#define FLGFILESYS_START            FLGPTN_BIT(0) //0x00000001
#define FLGFILESYS_DONE             FLGPTN_BIT(1) //0x00000002
#define FLGFILESYS_IDLE             FLGPTN_BIT(2) //0x00000004
//#NT#2007/06/27#Chris Hsu -begin
//#Prevent confused with other flag pattern
#define FLGFILESYS_INITIALED        FLGPTN_BIT(3) //0x00000008      // this flag will be raise after FS initialed other task shouldn't clear this flag
//#NT#2007/06/27#Chris Hsu -end
#define FLGFILESYS_TIMEOUT          FLGPTN_BIT(4) // 0x00000010
#define FLGFILESYS_DISKINFO_RDY     FLGPTN_BIT(5) // 0x00000020
#define FLGFILESYS_PAUSED           FLGPTN_BIT(6) // 0x00000040
#define FLGFILESYS_RESUME           FLGPTN_BIT(7) // 0x00000080
//#NT#2008/01/08#HH Chuang -begin
//#NT#add cluster sliding window
#define FLG_FILESYS_BACKGNDTSK      FLGPTN_BIT(8) // 0x00000100      // this flag is for internal functions to inform file system task to do background jobs
//#NT#2010/05/24#Brad Chen -begin
//#NT# add resume'd'
#define FLGFILESYS_RESUMED          FLGPTN_BIT(9) // 0x00000200      // this flag is for internal functions to inform file system task to do background jobs
//#NT#2010/05/24#Brad Chen -end
//#NT#2008/01/08#HH Chuang -end
//#NT#2009/05/19#Brad Chen -end


// --------------------------------------------------------------------
// File system command
// --------------------------------------------------------------------
#define FST_CMD_GET_INFO            1
#define FST_CMD_READ_PRE_FILE       2
#define FST_CMD_READ_NEXT_FILE      3
#define FST_CMD_READ_SPECIFIC_FILE  4   // read by specific File ID
#define FST_CMD_WRITE_NEXT_FILE     5

#define FST_CMD_CONTINUE_ACCESS     6
#define FST_CMD_CLOSE_FILE          7   // if FST have opened file, close it
#define FST_CMD_DELETE_FILE         8   // delete file by specified File ID or folder

#define FST_CMD_SYSINIT             9   // initialize the file system
#define FST_CMD_SYSUNINIT           10  // uninitialize the file sytem
#define FST_CMD_FORMAT              11  // format the the storage
#define FST_CMD_SET_ATTRIB          12  // used to setup the file attrib, if we had make sure that the file had been open we can modify the attrib directory
#define FST_CMD_QUERY_NEXT_ID       13  // query next valid ID
#define FST_CMD_QUERY_PREV_ID       14  // query previous ID
#define FST_CMD_QUERY_SPECI_ID      15  // copy files from one sotrage to another.
#define FST_CMD_STORAGE_COPY_TO     16  // Rename file name
#define FST_CMD_REARRANGE           17  // read whole file at one time
#define FST_CMD_READ_BY_NAME        18  // write whole file at one time
#define FST_CMD_WRITE_BY_NAME       19  // get specified file date and time
#define FST_CMD_GET_FILE_DATE_TIME  20
#define FST_CMD_WRITE_SPECIFIC_FILE 21  //
#define FST_CMD_TEST_FILE_SYSTEM    22
#define FST_CMD_CHANGE_WORKING_DIR  23
#define FST_CMD_WRITE_NEXT_SPEC_ID  24
#define FST_CMD_CHECK_COPY_TO       25
#define FST_CMD_RUN_BENCHMARK       35  // run file system benchmark
//#NT#2010/10/21#Janice Huang -begin
//#NT#add for append file
#define FST_CMD_WRITE_APP_BY_NAME   56
//#NT#2010/10/21#Janice Huang -end
//#NT#2010/11/05#Janice Huang -begin
//#NT#add filter delete
#define FST_CMD_FILTER_DELETE       57
//#NT#2010/11/05#Janice Huang -end
//#NT#2012/04/17#Lincy Lin -begin
//#NT#add func for change DCF DCIM path
#define FST_CMD_SET_VALID_DCFPATH   58
//#NT#2012/04/17#Lincy Lin -end
// --------------------------------------------------------------------
// File system state
// --------------------------------------------------------------------
#define FST_CONDITION_UNINIT        0
#define FST_CONDITION_IDLE          1
#define FST_CONDITION_BUSY          2

// --------------------------------------------------------------------
// File attribute access macro
// --------------------------------------------------------------------
//#NT#2007/03/08#Ethan Wang -begin
#define FST_ATTRIB_READONLY         FS_ATTRIB_READ      //> Indicates the file is read only.
#define FST_ATTRIB_HIDDEN           FS_ATTRIB_HIDDEN    //> Indicates the file should not be shown in normal directory listings.
#define FST_ATTRIB_SYSTEM           FS_ATTRIB_SYSTEM    //> Indicates the file is an operating system file.
#define FST_ATTRIB_VOLUME           FS_ATTRIB_VOLUME    //> Indicates the file is the label for the volume.
#define FST_ATTRIB_DIRECTORY        FS_ATTRIB_DIRECTORY //> Indicates it is a directory
#define FST_ATTRIB_ARCHIVE          FS_ATTRIB_ACHIEVE   //> Indicates the file has been created or modified since the attribute was clear.
//#NT#2007/03/08#Ethan Wang -end

//#NT#2007/07/26#Ethan Wang -begin
//#default newly-created file attribute
// --------------------------------------------------------------------
// Default File attribute
// --------------------------------------------------------------------
#define FST_DEF_ATTRIB_ARCHIVE      (1)
//#NT#2007/07/26#Ethan Wang -end

// --------------------------------------------------------------------
// File system status
// --------------------------------------------------------------------
#define FST_STA_OK                  (0)
#define FST_STA_CHG_DIR             (1)     // chage directory condition had happened
#define FST_STA_OVERSIZE            (2)     // file size is small then caller want.
#define FST_STA_TOTAL_FILES         (3)
#define FST_STA_COPIED_ONE          (4)
#define FST_STA_ACCESS_ONE          (4)
//#NT#2008/09/02#Chris Chung -begin
//#Force to remove file if RECYCLER folder can't be created.
#define FST_STA_DELETE_OK_WITH_UNDO (5)
//#NT#2008/09/02#Chris Chung -end
#define FST_STA_ERROR               (-1)
#define FST_STA_NO_FILE             (-2)
#define FST_STA_READING             (-3)
#define FST_STA_WRITING             (-4)
#define FST_STA_BUSY                (-5)    // file system task is busing
#define FST_STA_EXIST_FILE          (-6)
#define FST_STA_UNKNOW_CMD          (-7)    // unknow command
#define FST_STA_PARAM_ERR           (-9)    // the input parameters are wrong
#define FST_STA_NOFREE_SPACE        (-10)   // there are no more free space
#define FST_STA_SIZE_ZERO           (-11)   // this file size is zero
#define FST_STA_TIME_OUT            (-12)   // used of wait command
#define FST_STA_OUT_OF_ID           (-13)   // the DCF ID reach the maximum

#define FST_STA_FAT32               (FSS_FORMAT_FAT32)  // -14

#define FST_STA_DISKINIT_FAIL       (-15)   // this error code split two case
                                            // FST_STA_DISK_UNFORMAT and
                                            // FST_STA_DISK_UNKNOWN_FORMAT
#define FST_STA_DISK_UNFORMAT       (-15)
#define FST_STA_DISK_UNKNOWN_FORMAT (-18)
#define FST_STA_FUNC_NOT_READY      (-16)   // this function not ready
//#NT#2009/11/24#Lily Kao - begin
//change for compile error
//#define FST_STA_CARD_ERR            FSS_CARD_ERROR   // card error
#define FST_STA_CARD_ERR            ((-17)+ FSS_OFFSET)
//#NT#2009/11/24#Lily Kao - end
//#NT#2008/11/04#Chris Chung -begin
//#NT#add more storage error status
#define FST_STA_FAT_ERR             (-19)
#define FST_STA_UNUSED_CARD         (-20)
#define FST_STA_NODISK_SPACE        (-21)
#define FST_STA_STORAGE_PROCESSING  (-22)
//#NT#2008/11/04#Chris Chung -end
//#NT#2008/12/04#Janice Huang-begin
//#add segment read and abort specific file
#define FST_STA_READ_ABORT          (-23)
//#NT#2008/12/04#Janice Huang-end

#define FST_STA_FS_FAIL              FSS_FAIL             // status fail
#define FST_STA_FS_BUSY              FSS_BUSY             // status busy
#define FST_STA_FS_UNFORMAT          FSS_UNFORMAT         // storage is unformat. if realy want to use please format first
#define FST_STA_FS_EXCESS_SIZE       FSS_EXCESS_SIZE      // access area excess storage's size
#define FST_STA_FS_CANT_ACCESS       FSS_CANT_ACCESS      // try to access reserved area
#define FST_STA_FS_ERROR_PARAM       FSS_ERROR_PARAM      // parameter error
#define FST_STA_FS_UNKNOW_FORMAT     FSS_UNKNOW_FORMAT    // unknow format
#define FST_STA_FS_DUPLEX_NAME       FSS_DUPLEX_NAME      // find the same name in folder
#define FST_STA_FS_NOMORE_SPACE      FSS_NOMORE_SPACE     // nomore space
#define FST_STA_FS_OPEN_TOO_MANY     FSS_OPEN_TOO_MANY    // it open too many files
#define FST_STA_FS_IS_FOLDER         FSS_IS_FOLDER        // we want to open a folder
#define FST_STA_FS_ILLEGAL_FILENAME  FSS_ILLEGAL_FILENAME // filenaem is invalid, usually due to non 8.3 filename or file path deep
#define FST_STA_FS_OPERATION_DENY    FSS_OPERATION_DENY   // don't allow this file operation.
#define FST_STA_FS_FORMAT_FAT32      FSS_FORMAT_FAT32     // FAT 32 format, not support so far
#define FST_STA_FS_NO_MORE_FILES     FSS_NO_MORE_FILES    // use in search file, it means no more file in folder/

// --------------------------------------------------------------------
// File time mode
// --------------------------------------------------------------------
#define FST_TIMEMODE_CREATE_DATE    0
#define FST_TIMEMODE_CREATE_TIME    1
#define FST_TIMEMODE_MODIFY_DATE    2
#define FST_TIMEMODE_MODIFY_TIME    3
#define FST_TIMEMODE_ACT_FILE       0x80000000

// --------------------------------------------------------------------
// File format
// --------------------------------------------------------------------
//#NT#2011/04/08#Chris Chung -begin
//#add custom type
//#NT#2007/10/18#Ethan Wang -begin
//#add mov type
#define FST_FMT_TYPE_NUM            7
#define FST_FMT_NOFILE              (0)
#define FST_FMT_JPG                 (1)     // bit 0
#define FST_FMT_AVI                 (2)     // bit 1
#define FST_FMT_WAV                 (4)     // bit 2
#define FST_FMT_RAW                 (8)     // bit 3
#define FST_FMT_TIF                 (0x10)  // bit 4
#define FST_FMT_MOV                 (0x20)  // bit 5
//#NT#2007/10/18#Ethan Wang -end
#define FST_FMT_CUSTOM              (0x40)  // bit 6
//#NT#2011/04/08#Chris Chung -end
#define FST_FMT_ANYFORMAT           (0xFFFF)
#define FST_FMT_ASF                 (FST_FMT_AVI)
#define FST_FMT_MPG                 (FST_FMT_AVI)
#define CHK_VALID_FMT               1

// --------------------------------------------------------------------
// DCF standard
// --------------------------------------------------------------------
#define MIN_DCF_FILE_NUM            (1)
#define MAX_DCF_FILE_NUM            (9999)      // 0x270F although in DCF spec maximum is 9999, here for perfornmance issue we reduce this number
#define MIN_DCF_DIR_NUM             (100)       // 0x0064
#define MAX_DCF_DIR_NUM             (999)       // 0x03E7
//#NT#2010/05/19#Janice Huang -begin
//#NT#DCF out of range,set next id ERR
#define ERR_DCF_FILE_NUM            (0)
#define ERR_DCF_DIR_NUM             (0)
//#NT#2010/05/19#Janice Huang -end

#define FST_DCF_FILE_ALL            (0xFFFF)    // used in delete file, this should only be filled in FilenaemID member
#define FST_DCF_DIR_ALL             (0xFFFF)    // used in delete file, this should only be filled in DirectoryID member

//#NT#2008/01/17#Chris Chung -begin
//#Support getting max or min DCF file number in folder
#define FST_DCF_SCOPE_ALL           0
#define FST_DCF_SCOPE_VALID         1
//#NT#2008/01/17#Chris Chung -end

// --------------------------------------------------------------------
// File system time out
// --------------------------------------------------------------------
#define FST_TIME_NO_WAIT            (0)
#define FST_TIME_INFINITE           (0xFFFFFFFF)
#define FST_TIME_5_SECONDS          (5000)
#define FST_TIME_10_SECONDS         (10000)
#define FST_TIME_20_SECONDS         (20000)

// --------------------------------------------------------------------
// Copy to card parameter
// --------------------------------------------------------------------
//#NT#2007/05/21#Ethan Wang -begin
//#add description for each mode
#define FST_COPYTO_NEW_DIR          0       ///> copy to latest empty dir whose dir name is equal to initialized name , else copy to new dir
                                            ///> file index from 0001, new dir and file name is as initialized dcf name
#define FST_COPYTO_APPEND           1       ///> append to existing dir
                                            ///> file index from maximum existing file id plus by 1, new dir and file name is as initialized dcf name
#define FST_COPYTO_NO_RENAME        2       ///> copy to latest dir whose dir name is equal to initialized name and first source file id is greater than destination maximum file id, else copy to new dir
                                            ///> file index from source file id or 0001(new dir), new dir and file name is as initialized dcf name
//#NT#2007/03/19#Ethan Wang -begin
//#add new mode
#define FST_COPYTO_NEW_DIR_ORG_FILENAME 3   ///> the same as FST_COPYTO_NEW_DIR, but keep original filename
//#NT#2007/03/19#Ethan Wang -end
//#NT#2007/05/21#Ethan Wang -end

// --------------------------------------------------------------------
// File exist status
// --------------------------------------------------------------------
#define FST_IS_EXIST                1
#define FST_IS_EMPTY                0

// --------------------------------------------------------------------
// Callback function
// --------------------------------------------------------------------
typedef void (*Filesys_CB)(UINT32 MessageID, UINT32 Param1, UINT32 Param2, UINT32 Param3);
//typedef void (*Filesys_DIRCB)(FIND_DATA *findDir, BOOL *bContinue, char *cLongname, UINT32 Param);
//#NT#2007/10/16#lincy-begin
//#Support long file name in FileDB
typedef void (*Filesys_DIRCB)(FIND_DATA *findDir, BOOL *bContinue, UINT16 *cLongname, UINT32 Param);
//#NT#2007/10/16#lincy-end
// --------------------------------------------------------------------
// Assign dir id parameter
// --------------------------------------------------------------------
#define FST_AUTO_DIRID                0

//#NT#2007/12/26#HH Chuang -begin
//#NT#support undelete
// --------------------------------------------------------------------
// Undelete Mode
// --------------------------------------------------------------------
#define FST_UNDELETE_ONE    0
#define FST_UNDELETE_ALL    1
//#NT#2007/12/26#HH Chuang -end

// --------------------------------------------------------------------
// Structure
// --------------------------------------------------------------------
// sturcture used to control file system task, uausally we will not use the structure.
// here [in] means that caller app can fill this field
// and [out] means that caller only can read this field
typedef struct{
    UW      Command;            // [in] operating command
    int     Status;             // [out] return the operating status
    char    *pBuf;              // [in]I/O buffer.
    UW      BufSize;            // [in]size to access
    UW      Position;           // [in/out]In: start access position. out: access size
    UW      AccedSize;          // [in/out]: file access size in bytes
    UW      Offset;             // [in]: DCF file number offset
    UW      CloseImmediate;     // [in]: after access file close file Immediate.
    Filesys_CB FilesysCB;
    // don't change sequence of below member if we chg member of below remember to chg update function and reserved info///
    HNVT_FILE   *pFile;         // [out] only available in multiple times access file
    UW      TolFileNum;         // [out] Total DCF file in system
    UW      TolIDSequ;          // the sequence of ack ID in total ID
    UH      TolDirNum;          // [out] how many DCF folder in DCIM
    UH      MaxDirID;           // [out] maximun directory ID of this system, it may bigger then tol DirNum
    UH      DirectoryID;        // [in/out] current access directory ID.
    UH      FilenameID;         // [in/out] current access file number
    UH      DirFileSequ;        // [out] the sequence number of dir's exist file
    UH      DirFileNum;         // [out] how many DCF file in folder
    UH      MaxFileID;          // [out] maximun file ID of current access directory, it may bigger then DirFileNum
    UH      FileFormat;         // [in/out] point out the access file format it should be FST_FMT_JPG, WAV or AVI
    UH      IdFormat;               // [out] file format, use to indicate why many file format exist in the DCF ID.
}FILE_TSK_DCF_PARAM, *PFILE_TSK_DCF_PARAM;

typedef struct{
    UINT  dirId;
    UINT  fileId;
    UINT  desDirId;     // only use in big file
    UINT  desFileId;    // only use in big file
    UINT  fileType;
    UINT  attrib;
    UINT  readStart;
    UINT  readEnd;
    UINT  fileSize;
//#NT#2007/03/19#Ethan Wang -begin
//#add new mode
    char  filename[8];
//#NT#2007/03/19#Ethan Wang -end
    UINT  nextOffset;   // offset in buffer to next file inlcude the copyto_aux_structure
}COPYTO_AUX_INFO, *PCOPYTO_AUX_INFO;

//#NT#2007/06/01#Ethan Wang -begin
//#add copy specific file
typedef struct{
    STRG_TAB    *pSrcStrg;
    STRG_TAB    *pDstStrg;
    char        *pBuf;
    UINT32      uiBufSize;
    UINT32      Mode;
    UINT16      uhDirId;
    UINT16      uhFileId;
    UINT32      IdFormat;
    BOOL        bSkipbigfile;
    UINT32      uiSkipfileSize;
    BOOL        bDelete;
    Filesys_CB  pFilesysCB;
}COPYTO_INFO, *PCOPYTO_INFO;
//#NT#2007/06/01#Ethan Wang -end

//#NT#2007/09/07#Ethan Wang -begin
//#NT#copy by name
typedef struct{
    STRG_TAB    *pSrcStrg;
    STRG_TAB    *pDstStrg;
    char        *pBuf;
    UINT32      uiBufSize;
    char*       pSrcPath;
    char*       pDstPath;
    BOOL        bDelete;
    Filesys_CB  pFilesysCB;
}COPYTO_BYNAME_INFO, *PCOPYTO_BYNAME_INFO;
//#NT#2007/09/07#Ethan Wang -end

//#NT#2007/06/08#Ethan Wang -begin
//#add speed test
/**
 @struct FS_SPD_TEST_INFO FileSysTsk.h
    This structure contain file r/w speed test result information.
*/
typedef struct{
    UINT32  uiReadTime;
    UINT32  uiWriteTime;
}FS_SPD_TEST_INFO, *PFS_SPD_TEST_INFO;
//#NT#2007/06/08#Ethan Wang -end


/////////////////////////////////////////////////////////////////
/**
    This structuer contain file system task initialize information
*//////////////////////////////////////////////////////////////////
typedef struct{
    FS_INIT_PARAM   FSParam;    ///< the init param for file system
    char *pDirName;     ///< Dir name, Only the first 5 characters are valid. If NULL, it will use default value.
    char *pFileName;    ///< File name, Only the first 4 characters are valid. If NULL, it will use default value.
    char *pVideoExt;      ///< Extension name for video file, only the first 3 characters are valid, If NULL, it will use "AVI"
    char *pAudioExt;      ///< Extension name for audio file, only the first 3 characters are valid, If NULL, it will use "WAV"
    char *pVideoName;     ///< File name for video file, only the first 4 characters are valid, If NULL, it will use default value.
    char *pAudioName;     ///< File name for audio file, only the first 4 characters are valid, If NULL, it will use default value.
    char *pRAWName;     ///< File name for raw file, only the first 4 characters are valid, If NULL, it will use default value.
    char *pTIFFName;     ///< File name for tiff file, only the first 4 characters are valid, If NULL, it will use default value.
    //#NT#2011/04/08#Chris Chung -begin
    //#add custom type
    char *pCustomExt;   ///< Extension name for custom file, only the first 3 characters are valid, If NULL, it will use "BIN"
    //#NT#2011/04/08#Chris Chung -end
    Filesys_CB pDiskErrCB;  ///< Disk Error handle callback
    BOOL        bCreateDir; ///< Create default directory. e.g. DCIM,MISC
} FILE_TSK_INIT_PARAM, *PFILE_TSK_INIT_PARAM;

/**
 @struct FILE_TSK_DISK_INFO FileSysTsk.h
    This structuer contain disk information.
*//////////////////////////////////////////////////////////////////
typedef struct{
    UW      FreeSpaceSize;      ///< Free space size of current activign disk.
    UW      ToltalSize;         ///< Total sizes of storage.
    UW      FreeSpaceSizeHi;    ///< For suppot the storage bigger than 4G,the high-word of free space
    UW      ToltalSizeHi;       ///< For suppot the storage bigger than 4G,the high-word of total space
    UW      ClusterSize;        ///< Each cluster size in storage.
}FILE_TSK_DISK_INFO, *PFILE_TSK_DISK_INFO;

#define FST_FAT12  (0x01)
#define FST_FAT16  (0x02)
#define FST_FAT32  (0x03)

/**
 @struct FILE_TSK_DISK_INFO FileSysTsk.h
    This structuer contain partition information.
*/
typedef struct{
    char    SymbolChar;         /* character represent of the partion, A~Z */
    // partition entry
    UB    PartitionType;        /* system ID, what kind of file system has been install on partition */
    UB    SectPerClus;          /* how many sectors for every cluster */
    UB    NumOfFATs;            /* how many FAT tables on partition */
    UW    PBRSectorAddr;        /* LBA address for the first sector in partition */
    // pbr info
    char    szOEMName[8];       /* 8 bytes ASCII string */
    UH    FSInfo;               /* only for FAT32, do not exist on FAT12 and FAT16 usually is 1*/
    UH    BytesPerSec;          /* how many bytes for every sector */
    UH    ResvdSectCnt;         /* for FAT12 and FAT16 it always be 1, number of reserved sectors in the reserved region of the volume(exclude PBR) */
    UH    RootEntryCnt;         /* how many root entry */
    UH    FATStartSec;          /* where is FAT Start Sector */
    UH    SectorsPerFAT;        /* how many sectors per FAT */
    UW    TolSectors;           /* how many sectors in this partition */
    // addition info
    UW    MaxClusterValue;        /* max valid cluster number of this partition, note the first two cluster is reserved */
    UW    RootClus;             /* In FAT32, The cluster number of the first cluster of the root directory, It is usually 2*/
                                /* In FAT12/16, This is indicate the Start sector of Root entry */
    UW    RootDirSectors;       /* the count of sectors occupied by the root directory, always 0 for fat32*/
    UW    DataStartSect;        /* Data regin start sector, it is map to cluster 2*/
    UW    FreeClusterNum;       /* how many free cluster in this partition, it can use to obtain free size */
    UW    FirstFreeCluster;     /* the first free cluster */
    UW    SizePerCluster;       /* size of each cluster in bytes */
    UH    BkBootSec;            /* defined only for FAT32, It is usually 6*/
} FILE_TSK_PARTI_INFO, *PFILE_TSK_PARTI_INFO;

/**
 @struct FST_DCF_INFO FileSysTsk.h
    This structure contain DCF related information, all the conatain will be updated after any operation.
    @n Notes: Don't change any member's value of this structure.
*//////////////////////////////////////////////////////////////////
typedef struct{
    HNVT_FILE   *pFile;             ///< Reserved for multiple times access file
    UW      TolFileNum;             ///< How many DCF file in whole system
    UW      TolIDSequ;              ///< the sequence of ack ID in total ID
    UH      TolDirNum;              ///< How many DCF folder in DCIM
    UH      MaxDirID;               ///< Maximun directory ID of this system
    UH      DirectoryID;            ///< Current access directory number.
    UH      FilenameID;             ///< Current access file number.
    UH      DirFileSequ;            ///< The file sequence in directory's exist files.
    UH      DirFileNum;             ///< How many DCF file in folder.
    UH      MaxFileID;              ///< Maximun file ID of current access directory, it may bigger then DirFileNum
    UH      FileFormat;             ///< point out the access file format it should be FST_FMT_JPG, WAV or AVI
}FST_DCF_INFO, *PFST_DCF_INFO;

// --------------------------------------------------------------------
// Exposed functions (Operations)
// --------------------------------------------------------------------
extern INT      FilesysOpen(STRG_TAB *pWhichObj,  FILE_TSK_INIT_PARAM *pInitParam);
extern INT      FilesysClose(UW TimeOut);
extern INT      FilesysGetStrgObj(STRG_TAB **pWhichObj);
extern INT      FilesysGetStrgErrStatus(UINT32 *uiErrStatus);
extern void     FilesysRegisterCB(Filesys_CB CB);  //register call back for write file
extern INT      FilesysReadWriteCB(UW Cmd, UB *pBuf, UW BufSize, UW Pos, BOOL CloseFile, UW FileFormat, UW TimeOut,Filesys_CB CB);
extern INT      FilesysReadWrite(UW Cmd, UB *pBuf, UW BufSize, UW Pos, BOOL CloseFile, UW FileFormat, UW TimeOut);
extern INT      FilesysReadWrite2(UW Cmd, UB *pBuf, UW BufSize, UW Pos,UW Offset, BOOL CloseFile, UW FileFormat, UW TimeOut);
//#NT#2010/01/07#Chris Chung -begin
//#NT#add to write next file w/o close previous pFile
extern INT      FilesysReadWrite3(UW Cmd, UB *pBuf, UW BufSize, UW Pos,UW Offset, BOOL CloseFile, UW FileFormat, UW TimeOut, BOOL bClosePreviosPFILE);
//#NT#2010/01/07#Chris Chung -end
extern INT      FilesysReadWriteSpeciFile(UW Cmd, UH DirID, UH FileID, UH FileType, UB *pBuf, UW *pBufSize, UW TimeOut);
extern INT      FilesysSetDiskLabel(char* pLabelName);
//#NT#2011/06/24#Chris Chung -begin
//#NT#add to get disk label
extern INT      FilesysGetDiskLabel(char* pLabelName);
//#NT#2011/06/24#Chris Chung -end
extern INT      FilesysCopyFiles(STRG_TAB *pSrcStrg, STRG_TAB *pDesStrg, char *pBuf, UINT BufSize,UINT32 uiFileNum,Filesys_CB filesysCB,BOOL bDelete);
extern INT      FilesysFormat(STRG_TAB *pWhichStrg, BOOL ChgDisk);
extern INT      FilesysCheckCopySize(STRG_TAB *pSrcStrg, STRG_TAB *pDesStrg, char *pBuf, UINT BufSize,UINT32 *pFileNum,UINT32 *pTolFileNum);

extern INT      FilesysCopyTo(STRG_TAB *pSrcStrg, STRG_TAB *pDesStrg, char *pBuf, UINT BufSize, UINT Mode, UW *pFileNum,Filesys_CB filesysCB,BOOL bDelete);
//#NT#2007/06/01#Ethan Wang -begin
//#add copy specific file
extern INT      FilesysCopySpecificTo(COPYTO_INFO *pCopyInfo, UW *pFileNum);
//#NT#2007/06/01#Ethan Wang -end
//#NT#2008/04/30#Ethan Wang -begin
//#add change disk and backup/restore dcf info api
extern INT      FilesysChangeDisk(STRG_TAB *pWhichObj, BOOL bUpdInfo);
//#NT#2008/04/30#Ethan Wang -end

extern INT      FilesysSetDCFPath(UB *pPath, BOOL IsDir);
//#NT#2009/09/30#Lincy Lin -begin
//#Add DCF function for get DCF path in filesystem
extern INT      FilesysGetDCFPath(UB *pPath, BOOL IsDir);
//#NT#2009/09/30#Lincy Lin -end
extern INT      FilesysSetDCFFilename(INT8 *pName, UINT16 Type);
extern INT      FilesysStatus(void);
extern INT      FilesysWaitCmdFinish(UINT uiTimeOut);
extern INT      FilesysWaitInitFinish(UINT uiTimeOut);
extern INT      FilesysWaitDiskInfoReady(UINT TimeOut);
extern INT      FilesysRearrange(UINT whichDir, UINT TimeOut);
extern INT      FilesysReadWriteByName2(UW Cmd, char *pFilename, UB *pBuf, UW *pBufSize, UW Pos,UW TimeOut);
//#NT#2009/08/18#Janice -begin
//add FilesysReadWriteByName2CB
extern INT      FilesysReadWriteByName2CB(UW Cmd, char *pFilename, UB *pBuf, UW *pBufSize, UW Pos, UW TimeOut,Filesys_CB CB);
//#NT#2009/08/18#Janice -begin
extern INT      FilesysGetFileTime2(UH dirID, UH fileID, UH fileFormat ,UINT mode, UINT *pParam1, UINT *pParam2, UINT *pParam3 );
extern INT      FilesysGetFileTime(HNVT_FILE *pFile,UINT mode, UINT *pParam1, UINT *pParam2, UINT *pParam3 );
extern INT      FilesysCloseActFile(void);
extern INT      FilesysDeleteFile(UH DirID, UH FileID, UH FileType);
//#NT#2008/10/02#Chris Chung -begin
//#Add new API to let application decide to wait or delete empty folder.
extern INT      FilesysDeleteFile2(UH DirID, UH FileID, UH FileType, BOOL bDelEmptyFolder, UW TimeOut);
//#NT#2008/10/02#Chris Chung -end
//#NT#2010/11/05#Janice Huang -begin
//#NT#add filter delete
extern INT      FilesysFilterDelete(Filesys_CB CB);
//#NT#2010/11/05#Janice Huang -end
extern INT      FilesysDeleteByName(char *pPath,BOOL bIsFolder);
//#NT#2008/09/02#Chris Chung -begin
//#Add to delete empty DCF folder.
extern INT      FilesysDeleteEmptyDCFFolder(void);
//#NT#2008/09/02#Chris Chung -end
//#NT#2007/12/26#HH Chuang -begin
//#NT#support undelete
extern INT      FilesysUnDeleteFile(UINT32 UndelMode);
extern INT      FilesysClearRecycler(void);
extern INT      FilesysGetUnDeleteID(UINT16 *pDirId, UINT16 *pFileId);
extern INT      FilesysEnableRecycler(BOOL bEnable);
//#NT#2007/12/26#HH Chuang -end
//#NT#2008/09/02#Chris Chung -begin
//#Force to remove file if RECYCLER folder can't be created.
extern BOOL     FilesysIsUndoDelete(void);
//#NT#2008/09/02#Chris Chung -end
//#NT#2007/03/08#Ethan Wang -begin
extern INT      FilesysGetAttribByName(char *pPath,UINT8 *pAttrib);
//#NT#2007/03/08#Ethan Wang -end
//#NT#2008/06/11#Ethan Wang -begin
//#add api for set attribute
extern INT      FilesysSetAttribByName(char *pPath,UINT8 Attrib, BOOL bSet);
//#NT#2008/06/11#Ethan Wang -end
//#NT#2008/05/26#Ethan Wang -begin
//#add api for getting long name
extern INT      FilesysGetLongName(char *pPath, UINT16 *wFileName);
//#NT#2008/05/26#Ethan Wang -end
//#NT#2007/09/11#HH Chuang -begin
extern INT      FilesysCopyToByName(COPYTO_BYNAME_INFO *pCopyInfo, UINT32 TimeOut);
//#NT#2007/09/11#HH Chuang -end
extern INT      FilesysRename(char *pNewname, char *pPath,BOOL bIsFolder, BOOL bIsOverwrite); //#NT#2007/03/02#Ethan Wang
//#NT#2011/06/23#Chris Chung -begin
//#NT#add API for file moving
extern INT      FilesysMoveFile(char *pSrcPath, char *pDstPath);
//#NT#2011/06/23#Chris Chung -end
extern INT      FilesysDirDirectory(char *pPath, Filesys_DIRCB DirCB, BOOL bGetlong);
extern INT      FilesysSetWriteNextFileId(UINT16 hwDirID,UINT16 hwFileID,BOOL bCreateFolder);
extern INT      FilesysQueryID(UINT Cmd, UH *pDirID, UH *pFileID, UH *pFormat,UINT ChgActFile, UINT Param);
extern INT      FilesysGetSeqByDCFID(UINT32 *puiSequ, UH DirID, UH FileID);
extern INT      FilesysGetDCFIDBySeq(UINT32 uiSequ, UH *DirID, UH *FileID);
extern INT      FilesysCheckIntStgFile(STRG_TAB *pStrg,UINT64 *pNandUsedSize);
//#NT#2007/03/14#Janice ::for segment write once
extern int      FilesysSegmetWriteMode(BOOL bSet);
extern void     FilesysPause(void);
extern void     FilesysWaitPaused(void);
extern void     FilesysResume(void);
//#NT#2007/06/14#Ethan Wang -begin
//#add abort function
extern void     FilesysAbortCmd(UW Cmd);
//#NT#2007/06/14#Ethan Wang -end

extern INT      FilesysGetDCFFileName(char *pFileName, UINT16 FolderID, UINT16 FileID, UINT16 Type);
extern UINT32   FilesysGetDCFIdFormat(UINT16 FolderID, UINT16 FileID);
extern INT      FilesysIsDCFFolderExist(UINT16 FolderID,UINT *IsExist);
extern INT      FilesysGetDCFFolderName(char *pFolderName, UINT16 FolderID);
extern UINT32   FilesysUpdateFile(UINT16 DirID, UINT16 FileID, UINT16 FileType, UINT8 *pBuf, UINT32 *pBufSize, UINT32 TimeOut);
extern UINT32   FilesysGetDCFNextID(UINT16 *uhFolderId,UINT16 *uhFileId);
extern UINT32   FilesysGetDCFReadID(UINT16 curDirId, UINT16 curFileId,BOOL IsForward,UINT16 *ackDir,UINT16 *ackFile,UINT32 *ackType);
//#NT#2007/05/08#Janice --begin
//add FilesysGetDCFFileTypeNum
extern INT      FilesysGetDCFFileTypeNum(UINT32 fileType,UINT32 *fileNum);
//#NT#2007/05/08#Janice --end
//#add DCF file attribute counting
extern INT      FilesysGetDCFFileNumByAttrib(UINT32 fileAttrib,UINT32 *fileNum);
//#NT#2007/05/18#Janice --begin
//add FilesysGetDCFFileTypeInitNum
extern INT FilesysGetDCFFileTypeInitNum(UINT32 fileType,UINT32 *fileNum);
//#NT#2007/05/08#Janice --end
//#NT#2009/10/16#Chris Chung -begin
//#Support getting max or min DCF file number in folder
extern UINT32 FilesysGetDCFMaxOrMinFileID(char * pPath,BOOL bFindMax,UINT DCF_ScopeType,UINT16 * pFileID,UINT16 * pFileType,UW TimeOut);
//#NT#2009/10/16#Chris Chung -end
extern INT      FilesysChangeWorkingDir(UINT16 FolderID);
#if CHK_VALID_FMT
extern INT      FilesysSetValidFileFormat(UINT uiValidFileFormat);
extern INT      FilesysGetValidFileFormat(UINT *uiValidFileFormat);
#endif
extern PFILE_TSK_DCF_PARAM  FilesysGetFSTParam(void);
extern PFILE_TSK_DISK_INFO  FilesysGetDiskInfo(void);
extern PFILE_TSK_PARTI_INFO FilesysGetPartiInfo(void);
extern PFST_DCF_INFO        FilesysGetDCFInfo(void);
extern INT      FilesysGetDCFDefFolderStatus(BOOL *bCreatedDefDir);

extern INT      FilesysBenchmark(STRG_TAB *pStrg, UB *pBuf, UW BufSize);
//#NT#2007/06/08#Ethan Wang -begin
//#add speed test
extern INT      FilesysSpeedTest(STRG_TAB *pStrg, UB *pBuf, UW BufSize, UW TestSize, BOOL bIncOpenClose, FS_SPD_TEST_INFO *pResult);
//#NT#2007/06/08#Ethan Wang -end

extern UINT64   FilesysGetDiskSize(void);
extern UINT64   FilesysGetFreeSpace(void);
//#NT#2007/04/09#Janice Huang -begin
//new:Filesys_fopen for open file handle
extern HNVT_FILE* Filesys_fopen(char *filename, char *mode);
//#NT#2008/06/17#Janice Huang -begin
extern INT FilesysScanDCFObj(void);
//#NT#2008/06/17#Janice Huang -end
//#NT#2010/12/27#Chris Chung -begin
//#NT#add to scan DCF obj in folder
extern INT FilesysScanDCFObjInFolder(UINT16 DirID);
//#NT#2010/12/27#Chris Chung -end
//#NT#2008/12/04#Janice Huang-begin
//#add segment read for abort specific file
extern int      FilesysSegmetReadMode(BOOL bSet);
//#NT#2008/12/04#Janice Huang-end

//#NT#2009/10/09#Brad Chen --begin
//for segment write size

extern int FilesysSetSegmetWriteSize(UINT32 Size);
extern int FilesysGetSegmetWriteSize(void);

//#NT#2009/10/09#Brad Chen --end

//#NT#2009/05/20#Brad Chen -begin
//#NT#Add New API For playback to File Lock
extern int FilesysSetFileLock(UINT32 File_ID,UINT32 DirectoryID,BOOL FileLock);
//#NT#2009/05/20#Brad Chen -end
//#NT#2009/05/20#Brad Chen -begin
//#add cancel write next file abort function
extern int FileSysInValidAbortCmd(UW Cmd);
//#NT#2009/05/20#Brad Chen -end

//#NT#2007/04/09#Janice Huang --end
//#NT#2009/10/07#Lincy -begin
//#Add function for enable , disable Filesystem storage clock
void FilesysStrgClkEnable(BOOL IsEnable);
//#NT#2009/10/07#Lincy -end
//#NT#2009/11/20#Janice Huang --begin
//set user assigned date when create next file
extern int FilesysSetFileDateTime(UINT mode, UINT Param1, UINT Param2, UINT Param3);
//#NT#2009/11/20#Janice Huang --end
//#NT#2008/09/08#Chris Chung -begin
//#NT# Support calling other filesystem API in playback and pb will not crash.
extern int      FilesysSaveReservedInfo(UH uiDirId, UH uiFileId, UH uiFileType);
extern int      FilesysRestoreReservedInfo(void);
//#NT#2008/09/08#Chris Chung -end

//#NT#2009/10/12#Chris Chung -begin
//#NT#add to get free root entry count
extern int      FilesysGetFreeRootEntCnt(void);
//#NT#2009/10/12#Chris Chung -end

//#NT#2010/05/20#Chris Chung -begin
//#NT#add file check-point function
extern int      FilesysCheckPointActFile(void);
//#NT#2010/05/20#Chris Chung -end

/////////////////////////////////////////////////////////////////
/*
    Below functions are reserved only for backward compatible.
    Don't use these functions anymore.
*/
#define FilesysReadWriteByName(a, b, c, d)  FilesysReadWriteByName2(a, b, c, d, 0, FST_TIME_INFINITE)
#define FilesysGetDCFNumCache(a,b)          FilesysGetDCFIdFormat(a,b)


//#NT#2012/04/17#Lincy Lin -begin
//#NT#add func for change DCF DCIM path
extern int     FilesysSetValidDCFPath(char* path);
//#NT#2012/04/17#Lincy Lin -end
#endif
