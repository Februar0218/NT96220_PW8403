#ifndef UI_MENUPRINTSETTING_H
#define UI_MENUPRINTSETTING_H
//-------------------------------------------------------------------------------------------------
#include "PTP_API.h"
#include "DPS_Header_Public.h"
//-------------------------------------------------------------------------------------------------
#define UIPRINT_IMAGE_AMOUNT     500
//-------------------------------------------------------------------------------------------------
typedef struct
{
    UINT32  CropX;
    UINT32  CropY;
    UINT32  CropW;
    UINT32  CropH;
}UIPRINT_CROP_STRUCT;
//-------------------------------------------------------------------------------------------------
extern UINT32 UIPrintDataImageCount;
extern UINT32 UIPrintDataPrintCount;
extern DPS_PRINT_DATA_INFO UIPrintData;
//-------------------------------------------------------------------------------------------------
extern void UIPrint_Image_Init(void);
extern void UIPrint_Image_SetPrintAllCopy(UINT8 CopyAmount);
extern UINT32 UIPrint_Image_GetPrintAllCopy(void);
extern UINT32 UIPrint_Image_GetPrintCopy(void);
extern UINT8 UIPrint_Image_GetCopy(void);
extern void UIPrint_Image_SetCopy(UINT8 CopyAmount);
extern BOOL UIPrint_Image_CheckArrayFull(void);

extern void UIPrint_Image_SetCrop(UIPRINT_CROP_STRUCT*    pCrop);
extern void UIPrint_Image_GetCrop(UIPRINT_CROP_STRUCT*    pCrop);

extern void UIPrint_GetJobStatus(
    UINT16  *pCurrent,
    UINT16  *pTotal);
extern void UIPrint_SetJobStatus(
    UINT16  Current,
    UINT16  Total);
//-------------------------------------------------------------------------------------------------
#endif
