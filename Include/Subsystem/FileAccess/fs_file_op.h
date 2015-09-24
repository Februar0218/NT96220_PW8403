////////////////////////////////////////////////////////////////
/**
    Copyright (C) 2003 By Novatek Microelectronics Corp.

    @file   fs_file_op.h
    @ingroup mISYSFAT
    @brief  This file maintain the C language file operation interface.
        The file system provide the basic FAT file operation.

    @note   For DMA access, every file access and size must fall into word boundary.

*/



#ifndef _FS_FILE_OP_H
#define _FS_FILE_OP_H
#include "type.h"
#include "StrgDef.h"

//--------------------------------------
// Constant definition
//--------------------------------------
//#NT#2007/03/20#Ethan Wang -begin
//#change writing sequence
//If switch on, the writing sequence is : DATA -> FAT -> Directory
//If switch off, the writing sequence is : DATA -> Directory -> FAT
#define KFS_UPD_FAT_FIRST       0
//#NT#2007/03/20#Ethan Wang -end

//modiy for search directory deepth
//the maximum number of files that the implementation guarantees can be open at the same time;
#define KFS_FOPEN_MAX_NUM       4

//KFS_FSEARCH_MAX_NUM, KFS_MAX_DIRECTORY_DEEP are related,should the same
// the maximum number of search handle
#define KFS_FSEARCH_MAX_NUM     4

// the maximum directory deep in path
#define KFS_MAX_DIRECTORY_DEEP  KFS_FSEARCH_MAX_NUM

// the max length of file name,
// 3 is "A:\" , 9  dir name + "\"
#define KFS_PATH_MAX_LENG    3+ (9 * KFS_MAX_DIRECTORY_DEEP)

#define KFS_FILENAME_MAX_LENG   12  // 8.3 format + 1 null terminal

#define KFS_FILEPATH_MAX_LENG   KFS_PATH_MAX_LENG + KFS_FILENAME_MAX_LENG
//#define KFS_FILEPATH_MAX_LENG 64

#define KFS_LONGFILENAME_MAX_LENG  (256)
#define KFS_LONGNAME_PATH_MAX_LENG  (260)  //#NT#2007/02/12#Janice  ::support create long name

#define KFS_SEEK_SET 0 /* start of stream (see fseek) */
#define KFS_SEEK_CUR 1 /* current position in stream (see fseek) */
#define KFS_SEEK_END 2 /* end of stream (see fseek) */

// these constants are identical with ANSI C
#ifndef NULL
#define NULL    0
#endif

#ifndef EOF
#define EOF     (-1)
#endif

#define KFS_MIN_FAT_BUF_SIZE        (0x00020000)    //128k
#define KFS_MIN_CLUS_BUF_SIZE       (0x00008000)    // 32k, 64k will be better


#define     KFS_FORWARD_SEARCH      (0)
#define     KFS_BACKWARD_SEARCH     (1)

// file open mode
#define     KFS_OPENMODE_READ       ('R')
#define     KFS_OPENMODE_WRITE      ('W')
#define     KFS_OPENMODE_APPEND     ('A')
#define     KFS_OPENMODE_DIR        ('D')
 //#NT#2007/04/11#Janice Huang -begin
//new:fopen for update mode
#define     KFS_OPENMODE_UPDATE     ('U')
 //#NT#2007/04/11#Janice Huang -end
//#NT#2010/01/20#Chris Chung -begin
//#NT#sync NT98110 to support multi-language
//type of character set
#define KFS_CHARSET_ASCII     0
#define KFS_CHARSET_BIG5      1
#define KFS_CHARSET_GB2312    2
#define KFS_CHARSET_ISO8859_1    3
#define KFS_CHARSET_ISO8859_5    4
//file system character set
#define KFS_CHARSET                 KFS_CHARSET_BIG5
#define KFS_CHARSET_DYNAMIC_TABLE   0                   //0: static table, 1: dynamic table
//#NT#2010/01/20#Chris Chung -end
/*--------------------------------------*/
/*   File system interface return code defination   */
/*--------------------------------------*/
#define FSS_OFFSET          (-256)
#define FSS_OK              (0)                       // file system status ok
#define FSS_FAIL            ((-1)+ FSS_OFFSET)        // status fail
#define FSS_BUSY            ((-2)+ FSS_OFFSET)        // status busy
#define FSS_UNFORMAT        ((-3)+ FSS_OFFSET)        // storage is unformat. if realy want to use please format first
#define FSS_EXCESS_SIZE     ((-4)+ FSS_OFFSET)        // access area excess storage's size
#define FSS_CANT_ACCESS     ((-5)+ FSS_OFFSET)        // try to access reserved area
#define FSS_ERROR_PARAM     ((-6)+ FSS_OFFSET)        // parameter error
#define FSS_UNKNOW_FORMAT   ((-7)+ FSS_OFFSET)        // unknow format
#define FSS_DUPLEX_NAME     ((-8)+ FSS_OFFSET)       // find the same name in folder
#define FSS_NOMORE_SPACE    ((-9)+ FSS_OFFSET)        // nomore space
#define FSS_OPEN_TOO_MANY   ((-10)+ FSS_OFFSET)       // it open too many files
#define FSS_IS_FOLDER       ((-11)+ FSS_OFFSET)       // we want to open a folder
#define FSS_ILLEGAL_FILENAME    ((-12)+ FSS_OFFSET)   // filenaem is invalid, usually due to non 8.3 filename or file path deep
#define FSS_OPERATION_DENY  ((-13)+ FSS_OFFSET)       // don't allow this file operation.
#define FSS_FORMAT_FAT32    ((-14)+ FSS_OFFSET)       // FAT 32 format, not support so far

#define FSS_NO_MORE_FILES   ((-16)+ FSS_OFFSET)       // use in search file, it means no more file in folder/
#define FSS_CARD_ERROR      ((-17)+ FSS_OFFSET)       //card error
//#NT#2009/08/13#Brad Chen -begin
//#Check Nand total sectors match PBR Setting
#define FSS_STRSIZE_MISMATCH    ((-18)+ FSS_OFFSET)       //Storage Size mismatch
//#NT#2009/08/13#Brad Chen -end


// Directory structure attribute identify
#define FS_ATTRIB_FILE           0x00    // bit 0
#define FS_ATTRIB_READ           0x01    // bit 0
#define FS_ATTRIB_HIDDEN         0x02    // bit 1
#define FS_ATTRIB_SYSTEM         0x04    // bit 2
#define FS_ATTRIB_VOLUME         0x08    // bit 3
#define FS_ATTRIB_DIRECTORY      0x10    // bit 4
#define FS_ATTRIB_ACHIEVE        0x20    // bit 5

#define     M_IsReadOnly(x)         ((x) & FS_ATTRIB_READ)
#define     M_IsHidden(x)           ((x) & FS_ATTRIB_HIDDEN)
#define     M_IsSystem(x)           ((x) & FS_ATTRIB_SYSTEM)
#define     M_IsVolumeLabel(x)      ((x) & FS_ATTRIB_VOLUME)
#define     M_IsDirectory(x)        ((x) & FS_ATTRIB_DIRECTORY)
#define     M_IsArchive(x)          ((x) & FS_ATTRIB_ACHIEVE)
#define     M_IsFile(x)             (!((x) & FS_ATTRIB_DIRECTORY))


#define KFS_FAT12  (0x01)
#define KFS_FAT16  (0x02)
#define KFS_FAT32  (0x03)


//--------------------------------------
// structure define
//--------------------------------------


/////////////////////////////////////////////////////////////
/**
    This structure contain the necessary parameter for file system operation.

*//////////////////////////////////////////////////////////////
typedef struct{
    char *pFATBuf;              ///< the buffer for FAT cache
    unsigned int FATBufSize;    ///< the buffer size for FAT cache, must large then 128k bytes.
    char *pClusBuf;             ///< the cluster buffer
    unsigned int ClusBufSize;   ///< the cluster buffer size;, must large then 32k, 64k would be better
}FS_INIT_PARAM, *PFS_INIT_PARAM;


/////////////////////////////////////////////////////////////
/**
    @struct HNVT_FILE fs_file_op.h
    This sturcture cantain  file information. After open file success, you can get a HNVT_FILE point.
    This file point represent each file. We can get necessay file information form this sturcture, but
    do "NOT" modified any member value. It will cause unknow error. Usually, only the "attrib", "fileSize" will be used.
*//////////////////////////////////////////////////////////////
typedef struct {
    char            FATMainName[8]; ///< Major file name.
    char            FATExtName[3];  ///< Extern file name.
    unsigned char   attrib;         ///< File attribute.
    unsigned char   Reserved;       ///< Reserved.
    unsigned char   creSecond;      ///< File created time.
    unsigned short  creTime;        ///< File created time. bit0~4: seconds/2, bit5~10: minutes, bit11~15: hours.
    unsigned short  creDate;        ///< File created date. bit0~4: day(1-31), bit5~8: month(1-12), bit9~15: year(0-127) add 1980 to convert
    unsigned short  lastAccessDate; ///< The file latest access date.
    unsigned short  FstClusHI;      ///< Reserved.
    unsigned short  lastWriteTime;  ///< The latest time of write file.
    unsigned short  lastWriteDate;  ///< The latest date of write file.
    unsigned short  FstClusLO;      ///< Reserved.
    unsigned int    fileSize;       ///< File size in bytes.

    unsigned int    startCluster;   ///< file Start cluster
    unsigned char   pathDeep;       ///< FS internal used. Deep of the file name path. start from 1.
    unsigned char   pathLeng;       ///< FS internal used. Length of fileName;
    unsigned char   openMode;       ///< File open mode.
    unsigned char   chkSum;         ///< check sum
    //#NT#2007/09/04#Ethan Wang -begin
    // sync file created/latest acceess/latest write date and time
    unsigned char   bSyncDateTime;  ///< Sync file created/latest acceess/latest write date and time
    //#NT#2007/09/04#Ethan Wang -end
    //#NT#2009/11/20#Janice Huang --begin
    //set user assigned date when create next file
    unsigned char   bUserDateTime;
    //#NT#2009/11/20#Janice Huang --end
    //#NT#2010/01/14#Chris Chung -begin
    //#NT#support long name
    char            fullFileName[KFS_LONGNAME_PATH_MAX_LENG];   ///< Full file path.
    //#NT#2010/01/14#Chris Chung -end
    unsigned short  wFileName[KFS_LONGFILENAME_MAX_LENG];       ///< wchar_t the unicode file name
}HNVT_FILE, *PHNVT_FILE;



/////////////////////////////////////////////////////////////
/**
    @struct HFILE_SEARCH fs_file_op.h
        After call fs_FindFirstFile() succeed, it will return the handle. We will use this
        handle to perform the following fs_FindNextFile() and fs_FindClose().
        The handle contain all necessay informations for file searching.
*//////////////////////////////////////////////////////////////
typedef struct{
    UW      FolderStartCluster; // start cluster of search folder
    UW      CurrentCluster;     // which cluster we are searching, note: if search folder is root, this alway be 0
    UW      SectorInCluster;    // which sector in cluster we are searching, if CurrentCluster is 0, this
                                // member will be the offset sector from root entry start sector.
    UW      OffsetInSector;     // (0~511) what offset in sector we are searching
    void    *pFuncTbl;      // STRG_TAB *  function table of this disk
    void    *pInfo;     // PARTI_BPB_INFO  * // information of partition
    char    SearchPath[KFS_FILEPATH_MAX_LENG];
}HFILE_SEARCH, *PFILE_SEARCH;


/////////////////////////////////////////////////////////////
/**
    @struct FIND_DATA fs_file_op.h
        This sturcture contain data for found file.
*//////////////////////////////////////////////////////////////
typedef struct{
    char            FATMainName[8]; ///< Major file name.
    char            FATExtName[3];  ///< Extern file name.
    unsigned char   attrib;         ///< File attribute.
    unsigned char   Reserved;       ///< Reserved.
    unsigned char   creSecond;      ///< File created time.
    unsigned short  creTime;        ///< File created time. bit0~4: seconds/2, bit5~10: minutes, bit11~15: hours.
    unsigned short  creDate;        ///< File created date. bit0~4: day(1-31), bit5~8: month(1-12), bit9~15: year(0-127) add 1980 to convert
    unsigned short  lastAccessDate; ///< The file latest access date.
    unsigned short  FstClusHI;  ///< Reserved.
    unsigned short  lastWriteTime;  ///< The latest time of write file.
    unsigned short  lastWriteDate;  ///< The latest date of write file.
    unsigned short  FstClusLO;//startCluster;   ///< File start cluster of fat.
    unsigned int    fileSize;       ///< File size in bytes.
    char            filename[16];   ///< The full filename.
}FIND_DATA, *PFIND_DATA;

/////////////////////////////////////////////////////////////
/*
    @struct FS_INFO fs_file_op.h
        This structure contain data of disk.
*//////////////////////////////////////////////////////////////
typedef struct{
    UW uiFreeSpace;
    UW uiTolSpace;
    UW uiClusterSize;
}FS_INFO, *PFS_INFO;



//--------------------------------------
// function declare
//--------------------------------------

// these functions are reference to ANSI C standard library
//
extern int          fs_remove(const char * szFileName);
extern int          fs_rename(const char * szOldName, const char * szNewName);
extern int          fs_fclose(HNVT_FILE *pFile);
extern HNVT_FILE*   fs_fopen(const char *filename, const char* mode);
extern unsigned int     fs_fread(void * pBuf, unsigned int size, unsigned int nmemb, HNVT_FILE * stream);
extern unsigned int     fs_fwrite(const void * pBuf, unsigned int size, unsigned int nmemb, HNVT_FILE * stream);
extern int          fs_fseek(HNVT_FILE * stream,  int offset, int whence);

extern int          fs_fgetpos(HNVT_FILE * stream, unsigned int * pos);
//#NT#2007/12/26#HH Chuang -begin
//#NT#support undelete
extern int          fs_move(const char *pSrcPath, const char *pDstPath);
//#NT#2007/12/26#HH Chuang -end

/// not include in ANSI C
extern int          fs_init(STRG_TAB *pStrgDev, FS_INIT_PARAM *pInitParam);
extern int          fs_uninit(void);
//#NT#2012/04/17#Lincy Lin -begin
//#NT#add func for change DCF DCIM path
extern void         fs_releaseOpenList(void);
//#NT#2012/04/17#Lincy Lin -end
//extern int            fs_format(char Disk);
extern int          fs_format2(STRG_TAB *pStrgDev);
extern int          fs_SetDirAttrib(char * pPath, UINT uiAttrib, BOOL IsSet);
extern int          fs_setLabel(char *pLabelName);
//#NT#2011/06/24#Chris Chung -begin
//#NT#add to get disk label
extern int          fs_getLabel(char *pLabelName);
//#NT#2011/06/24#Chris Chung -end
extern int          fs_SetAttrib(const char * szFileName, UINT uiAttrib, BOOL IsSet);
//#NT#2007/08/28#HH Chuang -begin
//#NT#don't delete a folder if it is read-only
extern int          fs_GetAttrib(const char * szFileName, UINT *pAttrib);
//#NT#2007/08/28#HH Chuang -end
//#NT#2006/03/20#Janice Huang -begin
void fs_dir_setcwd(const char *pPath);
char *fs_dir_getcwd(void);
int fs_dir_getParent(const char *pPath,char *parentDir);
int fs_MakeDirectory(char *pPath);
int fs_ChangeDirectory(char *pPath);
int  fs_rmdir(char *pPath);
//// file manager api ////

extern HFILE_SEARCH*    fs_FindFileOpen(char *pPath, FIND_DATA *pFindData, int Direction, unsigned short *pLongFilename);
extern int              fs_FindFile(HFILE_SEARCH* pHSearch, FIND_DATA *pFindData, int Direction, unsigned short *pLongFilename);
extern int              fs_FindFileClose(HFILE_SEARCH* pHSearch);


//// get information api ////
extern UINT             fs_GetClusterSize(UCHAR ucDisk) ;
extern UINT64           fs_GetFreeSpace(UCHAR ucDisk, UINT *pHighInt, UINT *pLowInt);
extern UINT64           fs_GetDiskSize(UCHAR ucDisk, UINT *pHighInt, UINT *pLowInt);


//// reserved functions ///
extern HFILE_SEARCH*    fs_FindFirstFile(char *pPath, FIND_DATA *pFindData);
extern int              fs_FindNextFile(HFILE_SEARCH* pFileSearch, FIND_DATA *pFindData);
extern int              fs_FindClose(HFILE_SEARCH* pFileSearch);
//extern int              fs_SelfTest(unsigned int TestItem, char *pBuf, unsigned int BufSize);
extern int              fs_test_run_benchmark(char *pBuf, unsigned int BufSize);
//#NT#2007/06/08#Ethan Wang -begin
//#add speed test
extern int              fs_test_rw_spd(char *pBuf, unsigned int fsize, BOOL bIncOpenClose, unsigned int *rtime,unsigned int *wtime);
//#NT#2007/06/08#Ethan Wang -end
//#NT#2009/09/24#SA4 -begin
//#NT#2009/03/24#Marx Chiu -begin
extern void             fs_MakeStrUpper(char *Str);
//#NT#2009/03/24#Marx Chiu -end
//#NT#2009/09/24#SA4 -end

//#define     fs_FindFirstFile(x, y)  fs_FindFileOpen(x, y, KFS_FORWARD_SEARCH, NULL)
//#define     fs_FindNextFile(x, y)   fs_FindFile(x, y, KFS_FORWARD_SEARCH, NULL)
//#define     fs_FindClose(x)         fs_FindFileClose(x)

//#NT#2008/09/26#Janice Huang -begin
//#add set default entry date and time
extern int fs_setDefaultDate(UINT32 Year, UINT32 Month, UINT32 Day);
extern int fs_setDefaultTime(UINT32 Hour, UINT32 Minute, UINT32 Second);
extern void fs_usebDefaultTime(BOOL bDefault);
//#NT#2008/09/26#Janice Huang -end

//#NT#2010/05/20#Chris Chung -begin
//#NT#add file check-point/merge/truncate function
extern int          fs_fchkpnt(HNVT_FILE *stream);
extern int          fs_merge(HNVT_FILE *stream, const char * slaveName);
extern int          fs_truncate(HNVT_FILE *stream, UINT32 nClus);
//#NT#2010/05/20#Chris Chung -end

#endif //_FS_FILE_OP_H

