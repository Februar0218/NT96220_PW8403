#ifndef   _PTP_OBJECTMNG_FS_H
#define   _PTP_OBJECTMNG_FS_H
#include "fs_file_op.h"

#define     FAKE_LONGNAME_LEN           64 //2006/12/26 Meg
#define MAX_PTP_REG_NOTIFY_CB_NUMBER    6
//#define PTP_OBJ_READ_FIN        0
//#define PTP_OBJ_WRITE_FIN       1

#define PTP_ATTRIB_FILE      FS_ATTRIB_FILE           // bit 0
#define PTP_ATTRIB_READ      FS_ATTRIB_READ           // bit 0
#define PTP_ATTRIB_HIDDEN    FS_ATTRIB_HIDDEN         // bit 1
#define PTP_ATTRIB_SYSTEM    FS_ATTRIB_SYSTEM         // bit 2
#define PTP_ATTRIB_VOLUME    FS_ATTRIB_VOLUME         // bit 3
#define PTP_ATTRIB_DIRECTORY FS_ATTRIB_DIRECTORY      // bit 4
#define PTP_ATTRIB_ACHIEVE   FS_ATTRIB_ACHIEVE        // bit 5

#define PTP_IsReadOnly(x)      M_IsReadOnly(x)
#define PTP_IsHidden(x)        M_IsHidden(x)
#define PTP_IsSystem(x)        M_IsSystem(x)
#define PTP_IsVolumeLabel(x)   M_IsVolumeLabel(x)
#define PTP_IsDirectory(x)     M_IsDirectory(x)
#define PTP_IsArchive(x)       M_IsArchive(x)
#define PTP_IsFile(x)          M_IsFile(x)

typedef enum {
    PTP_OBJ_READ_FIN,
    PTP_OBJ_WRITE_FIN
} PTP_OM_RW_STATUS;

typedef __packed struct PTP_OBJECTINFO_T
{
    UINT32  StorageID;
    UINT16  ObjectFormat;
    UINT16  Protection;
    UINT32  ObjectSize;
    UINT16  ThumbFormat;
    UINT32  ThumbSize;
    UINT32  ThumbWidth;
    UINT32  ThumbHeight;
    UINT32  ImageWidth;
    UINT32  ImageHeight;
    UINT32  ImageBitDepth;
    UINT32  ParentObject;
    UINT16  AssociationType;
    UINT32  AssociationDesc;
    UINT32  SequenceNumber;
} PTP_OBJECTINFO, *PPTP_OBJECTINFO;//size = 52B
typedef void (*PTP_OBJ_INFO_FILL_CB)(UINT32 obj_handler,PTP_OBJECTINFO *objinfo);
typedef UINT32 (*PTP_OBJ_READ_DATA_CB)(UINT32 obj_handler,UINT32 pos,UINT8 *buf,UINT32 len);
typedef UINT32 (*PTP_OBJ_WRITE_DATA_CB)(UINT32 obj_handler,UINT32 pos,UINT8 *buf,UINT32 len);
typedef void (*PTP_OBJ_RWFIN_NOTIFY_CB)(UINT32 obj_handler,PTP_OM_RW_STATUS  mode);
typedef UINT32 (*PTP_OBJ_DEL_DATA_CB)(UINT32 obj_handler,UINT8 *filename);

typedef struct PTP_OBJECTLINK_T
{
    struct PTP_OBJECTLINK_T        *pNext;
    struct PTP_OBJECTLINK_T        *pPrev;

    struct PTP_OBJECTLINK_T        *pParent;

    UINT32      ObjectHandle;
    UINT32      ThumbOffset;

    UINT16      Date;
    UINT16      Time;
//#NT#2007/03/21#Meg Lin -begin
// modify for WHQL vista-PTP test, extend the length to save filename
    char        FileName[38];
//#NT#2007/03/21#Meg Lin -end
    char        Attrib;
    char        ObjectInfoExist;
    UINT16      ObjectFormat;
    UINT16      rev;
    UINT32      ObjectSize;
    //UINT32        DirID;

    //SICD_OBJECTINFO               ObjectInfo;

    //#NT#2008/10/20#Randy -begin
    PTP_OBJ_INFO_FILL_CB      ptp_obj_fill_objinfo_cb;//register the object info read call back function
    PTP_OBJ_READ_DATA_CB      ptp_obj_read_data_cb;//register the data read call back function
    //USICD_OBJ_WRITE_DATA_CB sicd_obj_write_data_cb;//register the data write call back function
    PTP_OBJ_WRITE_DATA_CB     ptp_obj_write_data_cb;//register the data write call back function
    PTP_OBJ_RWFIN_NOTIFY_CB   ptp_obj_rwfin_notify_cb;
    PTP_OBJ_DEL_DATA_CB       ptp_obj_del_data_cb;//register the data delete call back function
    //#NT#2008/10/20#Randy -end

//#NT#2007/03/21#Meg Lin -begin
// modify for WHQL vista-PTP test, extend the length to save filename
} PTP_OBJECTLINK, *PPTP_OBJECTLINK;//size = 12+8+4+38+2+4 +4 = 72

typedef BOOL (*PTP_NOTIFY_CB)(INT8   *filename,PTP_OBJECTINFO  *objinfo,PPTP_OBJECTLINK pSICDObj);
typedef struct PTP_REG_NOTIFY_CB_T
{
    INT8                filename[12+1];
    PTP_NOTIFY_CB       notify_cb;
}PTP_REG_NOTIFY_CB, *PPTP_REG_NOTIFY_CB;



//extern variable
extern UINT8   g_SendObjectInfoExist;
extern UINT32  g_NumOfObjects;
extern UINT32  g_ObjectHandle;
extern PTP_OBJECTLINK  g_PTPListHead;
extern CHAR gTmpLongFileName[FAKE_LONGNAME_LEN*2];//reserved (filename*1 + dirname*1)
extern VP      pPTPFreeHead, pPTPFreeEnd, pPTPNextFreeHead;


//extern func
BOOL PTP_OM_FS_NOTIFY_CB(INT8 *filename,PTP_OBJECTINFO  *objinfo,PPTP_OBJECTLINK pSICDObj);
PPTP_OBJECTLINK  PTP_OM_getBlock(BOOL reserved);
void PTP_OM_freeBlock(PPTP_OBJECTLINK object,BOOL reserved);
ER  PTP_OM_FindObjectHandle(UINT32 objHandle, PPTP_OBJECTLINK *SearchObj);
ER  PTP_OM_FindObjectHandle_By_FullPath(UINT32 *objHandle, UINT8    *full_path);
void PTP_OM_ObjListInit(void);
void PTP_OM_ResetFreeHead(void);
// To get long filename.
ER PTP_GetFileName(PPTP_OBJECTLINK pDCFObj, char *FileName, UINT32  FileNameLength);
ER PTP_GetFileNameX(PPTP_OBJECTLINK pSICDGenericObj, unsigned short *pWLongFileName, UINT16 *pui16LongFileNameLength);

// To determine whether a Big5 file name or not
BOOL PTP_IsBig5FileName(char *szShortFileName);

// Added for "selected print" func.
int PTP_IsValidDCFFolder(char *pFolderName);
int PTP_IsValidDCFJPGFile(char *pFileName);
int PTP_IsValidObjectFile(char *extname, const char *extcmp, int len);

UINT32 PTP_GetObjectFormat(char *szExtName);
void PTP_OM_AddObject(PPTP_OBJECTLINK parentobj, PPTP_OBJECTLINK insertobj);
void PTP_OM_AddObject_with_assigned_objid(PPTP_OBJECTLINK parentobj, PPTP_OBJECTLINK insertobj);
extern void PTP_OM_FS_objinfo_fill(UINT32 obj_handler,PTP_OBJECTINFO *objinfo);
extern UINT32 PTP_OM_FS_data_read(UINT32 obj_handler,UINT32 pos,UINT8 *buf,UINT32 len);
extern UINT32 PTP_OM_FS_data_write(UINT32 obj_handler,UINT32 pos,UINT8 *buf,UINT32 len);
extern void PTP_OM_FS_rwfin_notify(UINT32 obj_handler,PTP_OM_RW_STATUS  mode);
extern UINT32 PTP_OM_FS_data_del(UINT32 obj_handler,UINT8 *filename);
#endif

