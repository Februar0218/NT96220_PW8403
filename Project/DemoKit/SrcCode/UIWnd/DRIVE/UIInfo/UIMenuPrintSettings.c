#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "PlaybackTsk.h"
#include "DPOF.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UIFlow.h"
//-------------------------------------------------------------------------------------------------
static UINT32 UIPrintJobStatusCurrent;
static UINT32 UIPrintJobStatusTotal;
UINT32 UIPrintDataImageCount=1;
UINT32 UIPrintDataPrintCount;
UINT32 UIPrintDataPrintAllCount;
DPS_PRINT_DATA_INFO UIPrintData={0};
//-------------------------------------------------------------------------------------------------
void UIPrint_Image_Init(void)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_Init\r\n"));

    UIPrintDataImageCount = 0;
    UIPrintDataPrintCount = 0;
    UIPrintDataPrintAllCount = 0;
}
//-------------------------------------------------------------------------------------------------
void UIPrint_Image_SetPrintAllCopy(UINT8 CopyAmount)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_SetPrintAllCopy:%d\r\n",CopyAmount));
    UIPrintDataPrintAllCount = CopyAmount;
}
//-------------------------------------------------------------------------------------------------
UINT32 UIPrint_Image_GetPrintAllCopy(void)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_GetPrintAllCopy:%d\r\n",UIPrintDataPrintAllCount));
    return UIPrintDataPrintAllCount;
}
//-------------------------------------------------------------------------------------------------
UINT32 UIPrint_Image_GetPrintCopy(void)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_GetPrintCopy:%d\r\n",UIPrintDataPrintCount));
    return UIPrintDataPrintCount;
}
//-------------------------------------------------------------------------------------------------
UINT8 UIPrint_Image_GetCopy(void)
{
    //UINT16  FileId, DirId;
    //UINT32  i;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_GetCopy\r\n"));
    //#NT#2010/04/08#KS Hung -beign
    #if 0
    FileId = UIPrint_Playback_GetFileId();
    DirId = UIPrint_Playback_GetDirId();

    for (i=0; i<UIPrintDataImageCount; i++)
    {
        if( UIPrintData[i].dirID == DirId &&
            UIPrintData[i].fileID == FileId)
        {
            return (UIPrintData[i].quantity);
        }
    }
    #endif
    //#NT#2010/04/08#KS Hung -end

    return 0;
}
//-------------------------------------------------------------------------------------------------
void UIPrint_Image_SetCopy(UINT8 CopyAmount)
{
    //BOOL    IsFind = FALSE;
    UINT16  FileId, DirId;
    //UINT16  i, j;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_Copy:%d\r\n",CopyAmount));

    FileId = UIPrint_Playback_GetFileId();
    DirId = UIPrint_Playback_GetDirId();

    //#NT#2010/12/25#KS Hung -beign
    #if 0
    for (i=0; i<UIPrintDataImageCount; i++)
    {
        if( UIPrintData[i].dirID == DirId &&
            UIPrintData[i].fileID == FileId)
        {
            IsFind = TRUE;
            if(CopyAmount == 0)
            {
                UIPrintDataImageCount--;
                for (j=i; j<UIPrintDataImageCount; j++)
                {
                    memcpy(&UIPrintData[j], &UIPrintData[j+1], sizeof(DPS_PRINT_DATA_INFO));
                }
            }
            else
            {
                UIPrintData[i].quantity = CopyAmount;
            }
        }
    }

    if( !IsFind &&
        CopyAmount != 0)
    {
        if(UIPrintDataImageCount < UIPRINT_IMAGE_AMOUNT)
        {
            UIPrintData[UIPrintDataImageCount].dirID = DirId;
            UIPrintData[UIPrintDataImageCount].fileID = FileId;
            UIPrintData[UIPrintDataImageCount].quantity = CopyAmount;
            UIPrintData[UIPrintDataImageCount].cropX = 0;
            UIPrintData[UIPrintDataImageCount].cropY = 0;
            UIPrintData[UIPrintDataImageCount].cropW = UIPrint_Playback_GetCurrWidth();
            UIPrintData[UIPrintDataImageCount].cropH = UIPrint_Playback_GetCurrHeight();
            UIPrintDataImageCount++;
        }
        else
        {
            return;
        }
    }

    UIPrintDataPrintCount = 0;
    for (i=0; i<UIPrintDataImageCount; i++)
    {
        UIPrintDataPrintCount += UIPrintData[i].quantity;
    }
    #endif
    //#NT#2010/12/25#KS Hung -end

}
//-------------------------------------------------------------------------------------------------
BOOL UIPrint_Image_CheckArrayFull(void)
{

    if(UIPrintDataImageCount < UIPRINT_IMAGE_AMOUNT)
    {
        UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_CheckArrayFull:No\r\n"));
        return FALSE;
    }
    else
    {
        UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_CheckArrayFull:Yes\r\n"));
        return TRUE;
    }
}
//-------------------------------------------------------------------------------------------------
void UIPrint_Image_SetCrop(
    UIPRINT_CROP_STRUCT*    pCrop)
{
    UINT16  FileId, DirId;
    //UINT16  i;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_SetCrop:X=%d,Y=%d,W=%d,H=%d\r\n",pCrop->CropX,pCrop->CropY,pCrop->CropW,pCrop->CropH));

    FileId = UIPrint_Playback_GetFileId();
    DirId = UIPrint_Playback_GetDirId();
    //#NT#2010/04/08#KS Hung -beign
    #if 0
    for (i=0; i<UIPrintDataImageCount; i++)
    {
        if( UIPrintData[i].dirID == DirId &&
            UIPrintData[i].fileID == FileId &&
            UIPrintData[i].quantity > 0)
        {
            UIPrintData[i].cropX = pCrop->CropX;
            UIPrintData[i].cropY = pCrop->CropY;
            UIPrintData[i].cropW = pCrop->CropW;
            UIPrintData[i].cropH = pCrop->CropH;
        }
    }
    #endif
    //#NT#2010/04/08#KS Hung -end
}
//-------------------------------------------------------------------------------------------------
void UIPrint_Image_GetCrop(
    UIPRINT_CROP_STRUCT*    pCrop)
{
    UINT16  FileId, DirId;
    //UINT16  i;

    FileId = UIPrint_Playback_GetFileId();
    DirId = UIPrint_Playback_GetDirId();
    //#NT#2010/04/08#KS Hung -beign
    #if 0
    for (i=0; i<UIPrintDataImageCount; i++)
    {
        if( UIPrintData[i].dirID == DirId &&
            UIPrintData[i].fileID == FileId &&
            UIPrintData[i].quantity > 0)
        {
            UIPRINT_MSG(("UIPRINT_M:UIPrint_Image_GetCrop:X=%d,Y=%d,W=%d,H=%d\r\n",UIPrintData[i].cropX,UIPrintData[i].cropY,UIPrintData[i].cropW,UIPrintData[i].cropH));
            pCrop->CropX = UIPrintData[i].cropX;
            pCrop->CropY = UIPrintData[i].cropY;
            pCrop->CropW = UIPrintData[i].cropW;
            pCrop->CropH = UIPrintData[i].cropH;
        }
    }
    #endif
    //#NT#2010/04/08#KS Hung -end
}
//-------------------------------------------------------------------------------------------------
void UIPrint_GetJobStatus(
    UINT16  *pCurrent,
    UINT16  *pTotal)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetJobStatus:Current=%d, Total=%d\r\n",UIPrintJobStatusCurrent,UIPrintJobStatusTotal));
    *pCurrent = UIPrintJobStatusCurrent;
    *pTotal = UIPrintJobStatusTotal;
}
//-------------------------------------------------------------------------------------------------
void UIPrint_SetJobStatus(
    UINT16  Current,
    UINT16  Total)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetJobStatus:Current=%d, Total=%d\r\n",Current,Total));
    UIPrintJobStatusCurrent = Current;
    UIPrintJobStatusTotal = Total;
}
//-------------------------------------------------------------------------------------------------

