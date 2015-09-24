#ifndef _USER_PHOTO_FRAME_H
#define _USER_PHOTO_FRAME_H

#include "SysCfg.h"

#define PHOTO_FRAME_ID_MAX  16      //Max picture number

typedef struct
{
    UINT32      uiAddrOffset;       //The PStore address offset
    UINT32      uiFileSize;         //The picture filesize
    UINT32      uiRev1;             //Reserved1.
    UINT32      uiRev2;             //Reserved2.
} PHOTO_FRAME_INFO, *PPHOTO_FRAME_INFO;


extern BOOL     UserPF_ReadFrame(UINT32 uiFrameID, UINT32 uiBufAddr, PPHOTO_FRAME_INFO pFrameInfo);
extern BOOL     UserPF_WriteHeader(void);
extern BOOL     UserPF_WriteFrame(UINT32 uiFrameID, char *pFileName);
extern UINT32   UserPF_GetTotalNum(void);
extern void     UserPF_DeleteAll(void);
extern BOOL     UserPF_OpenPhotoFrame(UINT32 uiMode, UINT32 uiFrameID);
extern void     UserPF_ClosePhotoFrame(void);

#endif
