#ifndef _IMAGE_BORDER
#define _IMAGE_BORDER

#include "type.h"
#include "jpgEnc.h"
#include "jpgDec.h"

#define IMAGEBORDER_DEBUG_MSG                   1//0
#define IMAGEBORDER_DEBUG_ERR                   1

#define IMAGEBORDER_JPEG_SCREENNAIL_WIDTH       320
#define IMAGEBORDER_JPEG_SCREENNAIL_HEIGHT      240
#define IMAGEBORDER_JPEG_THUMBNNAIL_WIDTH       160
#define IMAGEBORDER_JPEG_THUMBNNAIL_HEIGHT      120

#define IMAGEBORDER_IDLE_STATE                    0
#define IMAGEBORDER_PREVIEW_STATE                10
#define IMAGEBORDER_CAPTURE_STATE                20
//#NT#2009/11/10#Chris Chung -begin
//#NT#add to crop image
#define IMAGEBORDER_CROP_STATE                   30
//#NT#2009/11/10#Chris Chung -end

#define IMAGEBORDER_BORDER                        0
#define IMAGEBORDER_PREVIEW                       1

#define IMAGEBORDER_NORMAL_SIZE_ACQUIRE_TYPE      0
#define IMAGEBORDER_MAX_MEMORY_SIZE_ACQUIRE_TYPE  0xffffffff
#define IMAGEBORDER_FILE_SIZE_ACQUIRE_TYPE        0xfffffffe

#define IMAGEBORDER_MAX_JPEG_BITSTREAM_SIZE       0x1C0000

typedef struct _IMAGE_BORDER_INFO{
    UINT32  uiBorderFileAddr;       // Border file address
    UINT32  uiBorderFileSize;       // Border file size
    UINT32  uiBorderWidth;          // Border Width size
    UINT32  uiBorderHeight;         // Border Height size
    UINT8   uiBorderColorKeyY;      // Color key for border Y
    UINT8   uiBorderColorKeyCb;     // Color key for border Cb
    UINT8   uiBorderColorKeyCr;     // Color key for border Cr
                                    // FALSE: do capture image border scaling while Image_SetImageBorder
    UINT32  uiPrvHsize;             // Border image scaling Horizontal size, only used in preview mode
    UINT32  uiPrvVsize;             // Border image scaling Vertical size, only used in preview mode
    UINT32  uiPrvOffset;            // Set the Offset value for specific position, only used in preview mode
    BOOL    bYOperationIsLess;      // TRUE for Y's operation is < , FALSE for Y's operation is >
} IMAGE_BORDER_INFO, *PIMAGE_BORDER_INFO;

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
    UINT32  maxJpgStrSize;
} IMAGE_BORDER_STOREIMAGE_INFO;
//#NT#2009/07/21Chris Chung -end

typedef UINT32 (*ImageBorder_MemAcquire_CB)(UINT32 type, UINT32 size);
typedef ER     (*ImageBorder_MemFree_CB)(UINT32 addr, UINT32 size);
typedef ER     (*ImageBorder_MemResize_CB)(UINT32 addr, UINT32 size);
typedef UINT32 (*ImageBorder_JpgScalar_CB)(PJPGHEAD_DEC_CFG decConfig, UINT32 *level);

extern void   ImageBorder_SetStatus(BOOL borderEnable, BOOL previewEnable);
extern BOOL   ImageBorder_GetStatus(UINT32 status);
extern ER     ImageBorder_OpenImageBorder(PIMAGE_BORDER_INFO imageBorder, BOOL boolYOpIsLess,
                                          ImageBorder_MemAcquire_CB acquireCB,
                                          ImageBorder_MemFree_CB freeCB,
                                          ImageBorder_MemResize_CB resizeCB,
                                          ImageBorder_JpgScalar_CB scalarHandler);
extern ER     ImageBorder_CloseImageBorder(void);
extern ER     ImageBorder_DrawPrvImageBorder(UINT32 srcAddrY, UINT32 srcAddrCb, UINT32 srcAddrCr);
extern ER     ImageBorder_StartCapSliceImageBorder(UINT32 capImgWidth, UINT32 capImgHeight, UINT32 hidenImgWidth, UINT32 hidenImgHeight);
extern ER     ImageBorder_StopCapSliceImageBorder(void);
extern ER     ImageBorder_CapSliceImageBorder(UINT32 srcAddrY, UINT32 srcAddrCb, UINT32 srcAddrCr);
extern ER     ImageBorder_CapQuickImageBorder(UINT32 srcAddrY, UINT32 srcAddrCb, UINT32 srcAddrCr);
extern ER     ImageBorder_StoreNewImagePhotoFrame(UINT32 directoryID, UINT32 filenameID, UINT32 maxJpgStrSize);
//#NT#2009/07/21Chris Chung -begin
//#NT#add to support digital zoom for photo frame
extern ER     ImageBorder_StoreNewImagePhotoFrame2(IMAGE_BORDER_STOREIMAGE_INFO *pInfo);
//#NT#2009/07/21Chris Chung -end
//#NT#2009/11/10#Chris Chung -begin
//#NT#add to crop image
extern ER     ImageBorder_OpenCrop(ImageBorder_MemAcquire_CB acquireCB,
                                   ImageBorder_MemFree_CB freeCB,
                                   ImageBorder_MemResize_CB resizeCB,
                                   ImageBorder_JpgScalar_CB scalarHandler);
extern ER     ImageBorder_CloseCrop(void);
extern ER     ImageBorder_StoreNewCropImage(IMAGE_BORDER_STOREIMAGE_INFO *pInfo);
//#NT#2009/11/10#Chris Chung -end

#endif // _PHOTO_FARME
