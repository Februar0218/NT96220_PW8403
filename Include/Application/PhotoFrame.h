#ifndef _PHOTO_FRAME_H
#define _PHOTO_FRAME_H

#include "type.h"
#include "ImageBorder.h"
#include "PrimaryTsk.h"
#include "ImageBorder.h"

//#NT#2009/07/21Chris Chung -begin
//#NT#add to support digital zoom for photo frame
typedef struct{
    UINT32  dirId;
    UINT32  fileId;
    BOOL    bZoomIn;
    UINT32  original_width;
    UINT32  original_height;
    UINT32  zoom_start_x;
    UINT32  zoom_start_y;
    UINT32  zoom_width;
    UINT32  zoom_height;
}PHOTOFRAME_STOREIMAGE_INFO;
//#NT#2009/07/21Chris Chung -end

extern void   PhotoFrame_EnableImageBorder(BOOL enable);
extern BOOL   PhotoFrame_GetImageBorderStatus(void);
extern ER     PhotoFrame_SetImageBorder(UINT32 currStatus, PIMAGE_BORDER_INFO puiImageBorder);
extern void   PhotoFrame_DrawPrvImageBorder(UINT32 tmpBuf_Y, UINT32 tmpBuf_Cb, UINT32 tmpBuf_Cr);
extern ER     PhotoFrame_StoreNewImagePhotoFrame(UINT32 directoryID, UINT32 filenameID);
extern UINT32 PhotoFrame_GetCurrFbBufferID(void);
//#NT#2009/11/10#Chris Chung -begin
//#NT#add to crop image
extern ER     PhotoFrame_EnableCrop(BOOL en);
extern ER     PhotoFrame_StoreNewCropImage(PHOTOFRAME_STOREIMAGE_INFO *pInfo);
//#NT#2009/11/10#Chris Chung -end

#endif
