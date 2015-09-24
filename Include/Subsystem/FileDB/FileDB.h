/*
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       FilesDb.h
    @ingroup

    @brief

    @note       Nothing.

    @version    V0.00.001
    @author     Lincy Lin
    @date       2007/02/05
*/


#ifndef _FILESDB_H_
#define _FILESDB_H_

#include "type.h"
#include "Fs_file_op.h"
#include "FilesysTsk.h"

//#NT#2009/08/11#Lincy Lin -begin
//#Enhance File number and index from UINT16 to UINT32

//#NT#2008/06/03#Meg Lin -begin
//remove some comments
typedef INT16 FILEDB_HANDLE;
#define FILEDB_STATIC_MEM_MAX_FILE_NUM  10
//#NT#2008/08/12#Lincy Lin -begin
//#Change the max file number value to be defined by caller, it should be passed in FileDB_Create parameter
//#define FILEDB_MAX_FILE_NUM             5000
//Add some error handling for filter out the same DCF number folder ID
//#define FILEDB_MAX_DIFF_DCF_FOLDER      (FILEDB_MAX_FILE_NUM/5)
//#NT#2008/08/12#Lincy Lin -end
#define _SUPPORT_LONG_FILENAME_IN_FILEDB_      0
#define FILEDB_MAX_LONG_FILENAME_LEN          30


#define FILEDB_CREATE_ERROR   -1
#define FILEDB_NUM             2
#define FILE_FILTER_MAX_LEN   40
#define FILEDB_SEARCH_ERR     -1

#define FILEDB_SUPPORT_FILE_TYPE_NUM  7
#define FILEDB_FMT_JPG  0x00000001
#define FILEDB_FMT_MP3  0x00000002
#define FILEDB_FMT_AVI  0x00000004
#define FILEDB_FMT_ASF  0x00000008
#define FILEDB_FMT_WAV  0x00000010
#define FILEDB_FMT_MOV  0x00000020
#define FILEDB_FMT_BMP  0x00000040
//Add ANY file type for get all types of files
#define FILEDB_FMT_ANY  0xFFFFFFFF

//#NT#2008/03/14#Lincy -begin
//#Support to keep some flags in each file item
#define FILEDB_ITEM_MARKED  0x01
//#NT#2008/03/14#Lincy -end

#define FILEDB_GET_DAY_FROM_DATE(x)     (x & 0x1F)
#define FILEDB_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)
#define FILEDB_GET_YEAR_FROM_DATE(x)    ((x >> 9) & 0x7F)+1980
#define FILEDB_GET_SEC_FROM_TIME(x)     (x & 0x001F)<<1  // seconds(2 seconds / unit)
#define FILEDB_GET_MIN_FROM_TIME(x)     (x & 0x07E0)>>5  // Minutes
#define FILEDB_GET_HOUR_FROM_TIME(x)    (x & 0xF800)>>11  // hours
typedef enum {
    FILEDB_SORT_BY_NONE = 0x00,
    FILEDB_SORT_BY_CREDATE,
    FILEDB_SORT_BY_MODDATE,
    FILEDB_SORT_BY_NAME,
    FILEDB_SORT_BY_STROKENUM,
    FILEDB_SORT_BY_SIZE,
    FILEDB_SORT_BY_FILETYPE,
    //Add new sort type - sort by the sequence (1) File type, (2) File Cretate time, (3) file size
    FILEDB_SORT_BY_TYPE_CRETIME_SIZE
} FILEDB_SORT_TYPE;

typedef BOOL (*CHKLONGNAME_CB)(UINT16* longName, UINT16 nameLen);
typedef BOOL (*CHKABORT_CB)(void);

typedef struct {
    char                 filename[KFS_FILENAME_MAX_LENG+1];    ///< File Name
    char                 filePath[KFS_FILEPATH_MAX_LENG+1];    ///< File full path
    UINT8                attrib;                               ///< File attribute.
    //#NT#2008/08/11#lincy-begin
    //#reduce file attr size
    #if (_SUPPORT_LONG_FILENAME_IN_FILEDB_)
    BOOL                 bIsNeedRename;
    #endif
    //UINT8                playRotateDir;
    //#NT#2008/08/11#lincy-end
    //#NT#2008/03/14#Lincy -begin
    //#Support to keep some flags in each file item
    UINT8                Flags;             ///   bit 0 : This is item marked or not
    //#NT#2008/03/14#Lincy -end
    #if (_SUPPORT_LONG_FILENAME_IN_FILEDB_)
    UINT16               longName[FILEDB_MAX_LONG_FILENAME_LEN+1];
    #endif
    UINT16               creTime;                              ///< File created time. bit0~4: seconds/2, bit5~10: minutes, bit11~15: hours.
    UINT16               creDate;                              ///< File created date. bit0~4: day(1-31), bit5~8: month(1-12), bit9~15: year(0-127) add 1980 to convert
    UINT16               lastWriteTime;                        ///< The latest time of write file.
    UINT16               lastWriteDate;                        ///< The latest date of write file.
    UINT32               fileSize;                             ///< File size in bytes.
    UINT32               fileType;                             ///< File type, the value could be FILEDB_FMT_JPG, FILEDB_FMT_MP3 or FILEDB_FMT_AVI ...

} FILEDB_FILE_ATTR, *PFILEDB_FILE_ATTR;

typedef struct {
    char*                rootPath;                             ///<  Root path
    BOOL                 bIsRecursive;                         ///<  If recursive search this path
    BOOL                 bIsCyclic;                            ///<  If cyclic search
    BOOL                 bIsUseDynaMem;                        ///<  If Use Dynamic Memory
    BOOL                 bIsMoveToLastFile;                    ///<  If want to move file index to last file
    BOOL                 bIsDCFFileOnly;
    BOOL                 bIsFilterOutSameDCFNumFolder;
    //#NT#2009/09/30#lincy-begin
    //Fit our DCF rule
    char                 OurDCFDirName[5];
    char                 OurDCFFileName[4];
    UINT8                bIsFilterOutSameDCFNumFile;
    //#NT#2009/09/30#lincy-end
    //#NT#2008/08/06#lincy-begin
    //#For check if no files only
    BOOL                 bIsChkHasFile;                        ///<  If just want to check if there any matched file inside file system
    //#NT#2008/08/06#lincy-end
    //#NT#2008/08/12#Lincy Lin -begin
    //#Change the max file number value to be defined by caller, it should be passed in FileDB_Create parameter
    UINT32               u32MaxFileNum;
    //#NT#2008/08/12#Lincy Lin -end
    UINT32               fileFilter;
    UINT32               u32MemAddr;                           ///<  The Dynamic Memory address
    UINT32               u32MemSize;                           ///<  The Dynamic Memory Size
    //#NT#2009/08/21#Lincy Lin -begin
    //#Remove unused code
    //UINT32               *maxStoreNumRtn;                      ///<  The max file num can be stored in fileDB depends on the memory size
    //#NT#2009/08/21#Lincy Lin -end
    CHKLONGNAME_CB       fpChkLongname;
    CHKABORT_CB          fpChkAbort;
} FILEDB_INIT_OBJ, *PFILEDB_INIT_OBJ;

typedef struct {
    BOOL                 bIsUsed;                              ///< if this FileDB is used
    BOOL                 bIsRecursive;                         ///<  If recursive search this path
    BOOL                 bIsCyclic;                            ///<  If cyclic search
    BOOL                 bIsUseDynaMem;                        ///<  If Use Dynamic Memory
    BOOL                 bIsSortLargeFirst;
    char                 rootPath[KFS_FILEPATH_MAX_LENG+1];    ///<  Root path
    BOOL                 bIsDCFFileOnly;
    BOOL                 bIsFilterOutSameDCFNumFolder;
    //#NT#2009/09/30#lincy-begin
    //Fit our DCF rule
    char                 OurDCFDirName[5];
    char                 OurDCFFileName[4];
    UINT8                bIsGetFile;
    UINT8                bIsFilterOutSameDCFNumFile;
    //#NT#2009/09/30#lincy-end
    //#NT#2008/08/06#lincy-begin
    //#For check if no files only
    BOOL                 bIsChkHasFile;                        ///<  If just want to check if there any matched file inside file system
    BOOL                 bIsFindMatchFile;                     ///<  If already find the matched file, when this flag sets, FileDB will return
    //#NT#2008/08/06#lincy-end
    FILEDB_SORT_TYPE     sortType;                             ///<  The sort type of this database
    UINT32               u32TotalFilesNum;                     ///<  Total items count in this directory
    UINT32               u32CurrentFileIndex;                  ///<  Current file Index
    //#NT#2009/09/30#lincy-begin
    //Fit our DCF rule
    UINT32               u32CurDirStartFileIndex;            ///<  The start file index of current directory
    //#NT#2009/09/30#lincy-end
    //#NT#2008/08/12#Lincy Lin -begin
    //#Change the max file number value to be defined by caller, it should be passed in FileDB_Create parameter
    UINT32               u32MaxFileNum;
    //#NT#2008/08/12#Lincy Lin -end
    //#NT#2008/05/27#lincy-begin
    //#Add for support playback slide show some specific files
    UINT32               u32SpecificFileIndex;                  ///<  Specific file Index
    //#NT#2008/05/27#lincy-end
    //#NT#2008/08/12#Lincy Lin -begin
    //#Change the max file number value to be defined by caller, it should be passed in FileDB_Create parameter
    //UINT16               sortIndexArray[FILEDB_MAX_FILE_NUM+FILEDB_MAX_DIFF_DCF_FOLDER];  ///<  The sorting mapping index
    UINT32*              pSortIndex;
    //#NT#2008/08/12#Lincy Lin -end
    UINT32               filesNum[FILEDB_SUPPORT_FILE_TYPE_NUM];  ///<  The files count of one file type
    //#NT#2008/11/04#Lincy Lin -begin
    //#Add group file type index mapping
    UINT32*              GroupIndexMappTbl[FILEDB_SUPPORT_FILE_TYPE_NUM];          ///<  The Index mapping table of each grouped file type
    UINT32               u32GroupCurIdx[FILEDB_SUPPORT_FILE_TYPE_NUM];             ///<  The current file Index of each grouped file type
    UINT32               groupIndexMappTblSize[FILEDB_SUPPORT_FILE_TYPE_NUM];      ///<  The mapping table size
    UINT32               groupFileType[FILEDB_SUPPORT_FILE_TYPE_NUM];              ///<  The grouped file type
    //#NT#2008/11/04#Lincy Lin -end
    UINT32               fileFilter;
    UINT32               u32MemAddr;                           ///<  The Dynamic Memory address
    UINT32               u32MaxSotreItemsNum;                  ///<  The Max Number of Files Info can be sotred in the Dynamic Memory
    FILEDB_FILE_ATTR     file[FILEDB_STATIC_MEM_MAX_FILE_NUM];
    CHKLONGNAME_CB       fpChkLongname;
    CHKABORT_CB          fpChkAbort;
} FILEDB_INFO, *PFILEDB_INFO;

//#NT#2009/09/30#lincy-begin
//#Modify FileDB_Create to call by address
extern FILEDB_HANDLE FileDB_Create(PFILEDB_INIT_OBJ pfileDbInitObj);
//#NT#2009/09/30#lincy-end
extern void FileDB_Release(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_SearhFile(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex);
extern PFILEDB_FILE_ATTR FileDB_SearhFile2(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex);
extern UINT32 FileDB_GetTotalFileNum(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_CurrFile(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_NextFile(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_PrevFile(FILEDB_HANDLE fileDbHandle);
//#NT#2008/05/27#lincy-begin
//#Add for support playback slide show some specific files
extern PFILEDB_FILE_ATTR FileDB_SpecificFile(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex);
extern void FileDB_ClrSpecificFileIndex(FILEDB_HANDLE fileDbHandle);
extern void FileDB_SetSpecificFileIndex(FILEDB_HANDLE fileDbHandle, UINT32 u32SpecFileIndex);
//#NT#2008/05/27#lincy-end
extern UINT32 FileDB_GetCurrFileIndex(FILEDB_HANDLE fileDbHandle);
extern void FileDB_SortBy(FILEDB_HANDLE fileDbHandle, FILEDB_SORT_TYPE sortType, BOOL bIsSortLargeFirst);
extern UINT32 FileDB_GetFilesNumByFileType(FILEDB_HANDLE fileDbHandle, UINT32 fileType);
extern INT32 FileDB_Refresh(FILEDB_HANDLE fileDbHandle);
BOOL FileDB_DeleteFile(FILEDB_HANDLE fileDbHandle,UINT32 u32FileIndex);
INT32 FileDB_GetIndexByPath(FILEDB_HANDLE fileDbHandle, char *filePath);
//#NT#2008/06/03#Meg Lin -end
//#NT#2008/11/04#lincy-begin
//#Add group file type index mapping
void FileDB_SetFileMappTblByFileType(FILEDB_HANDLE fileDbHandle, UINT32 fileType, UINT32* TblAddr, UINT32 TblSize);
PFILEDB_FILE_ATTR FileDB_NextFileByType(FILEDB_HANDLE fileDbHandle, UINT32 fileType);
PFILEDB_FILE_ATTR FileDB_PrevFileByType(FILEDB_HANDLE fileDbHandle, UINT32 fileType);
PFILEDB_FILE_ATTR FileDB_CurrFileByType(FILEDB_HANDLE fileDbHandle, UINT32 fileType);
PFILEDB_FILE_ATTR FileDB_SearhFileByType(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex, UINT32 fileType);
PFILEDB_FILE_ATTR FileDB_SearhFileByType2(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex, UINT32 fileType);
PFILEDB_FILE_ATTR FileDB_SpecificFileByType(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex, UINT32 fileType);
UINT32 FileDB_GetCurrFileIndexByType(FILEDB_HANDLE fileDbHandle, UINT32 fileType);
BOOL FileDB_DeleteFileByType(FILEDB_HANDLE fileDbHandle, UINT32 u32FileIndex, UINT32 fileType);
//#NT#2008/11/04#lincy-end
//#NT#2009/08/11#Lincy Lin -end

//#NT#2009/08/12#Lincy Lin -begin
UINT32 FileDB_GetBufSizeByFileNum(UINT32 fileNum);
//#NT#2009/08/12#Lincy Lin -end
#endif  //_FILESDB_H_
