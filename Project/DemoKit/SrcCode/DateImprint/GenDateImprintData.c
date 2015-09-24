/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GenDateImprintData.c
    @ingroup    Unknown

    @brief      Sample Code for Date imprint data generation.
                Sample Code for Date imprint data generation.

    @note       Nothing.

    @version    V1.00.000
    @author     Shirley Cheng
    @date       2006/02/15

*/


/** \addtogroup Unknown*/
//@{

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"
#include "OSCommon.h"
#include "SysCfg.h"
#include "UIFlow.h"
#include "GenDateImprintData.h"
#include "DataStamp.h"
#include "UIPhotoObj.h"
#include "UISystemObj.h"
#include "ImageAlgInfor.h"
#include "filesystsk.h"
#include "DateStampFont10x16.h" 
#include "DateStampFont12x20.h"
#include "DateStampFont18x30.h"
#include "DateStampFont26x44.h"
#include "DateStampFont30x52.h"
#include "DateStampFont36x60.h"
#include "DateStampFont42x72.h"
#include "DateStampFont50x86.h"
#include "DateStampFont56x96.h"

#define COLOR_BG_Y      0x00
#define COLOR_BG_U      0x80
#define COLOR_BG_V      0x80

#define CAR_NUMBER_STAMP_LEN	16

//function declare
static void DateImprint_Open(void);
static void DateImprint_Close(void);
static void DateImprint_SetStrInfo(PDATEIMPRINT_INFO pDateImprintData, PDATEIMPRINT_INFO pDateImprintDataThumb, UINT32 flag);
static void DateImprint_Gen(PDATEIMPRINT_INFO DateImprintData);
static void DateImprint_GenThumb(PDATEIMPRINT_INFO DateImprintDataThumb);
static BOOL DateImprint_GenStr_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiMemAddr, UINT32 uiLineOffset);
static BOOL DateImprint_GenIcon_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset);
static void DateImprint_Gen_Scalar_FW(PDATEIMPRINT_INFO DateImprintData);
static BOOL DateImprint_GenStr_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiDstLineOffset, UINT32 uiDstHeight);
static BOOL DateImprint_GenIcon_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset, UINT32 uiIconWidth, UINT32 uiIconHeight);

//variable declare
static UINT32       uiMemoryAddr;
static UINT32       uiMemoryAddrThumb;
static STAMP_POS    g_StampPos = {0, 0};        // stamp position for primary image
static STAMP_POS    g_StampPosThumb = {0, 0};   // stamp position for thumbnail
static COLOR_YCBCR  g_StampColorBg = {COLOR_BG_Y, COLOR_BG_U, COLOR_BG_V}; // date stamp background color
static COLOR_YCBCR  g_StampColorFr = {COLOR_BG_Y, COLOR_BG_U, COLOR_BG_V}; // date stamp frame color
//static COLOR_YCBCR  g_StampColorFg = {RGB_GET_Y(250, 160, 10), RGB_GET_U(250, 160, 10), RGB_GET_V(250, 160, 10)}; // date stamp foreground color (orange)
static COLOR_YCBCR  g_StampColorFg = {RGB_GET_Y(250, 250, 250), RGB_GET_U(250, 250, 250), RGB_GET_V(250, 250, 250)}; // date stamp foreground color (white)

DATEIMPRINT_INFO    DateInfo, DateInfoThumb;
char                DateStampStr[32], DateStampThumbStr[32];

void DateImprint_Setup(UINT32 flag, UINT32 dstFormat)
{
    if ((flag & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        DateImprint_Close();
    }
    else
    {
        DateImprint_Open();
        DateInfo.pi8Str = DateStampStr;
        DateInfoThumb.pi8Str = DateStampThumbStr;
        DateInfo.ui32DstFormat = dstFormat;
        DateImprint_SetStrInfo(&DateInfo, &DateInfoThumb, flag);
        DateInfo.ui32PutActiveAddr = uiMemoryAddr;
        DateImprint_Gen_Scalar_FW(&DateInfo);
        DateImprint_GenThumb(&DateInfoThumb);
    }
}

void DateImprint_SetPos(UINT32 x, UINT32 y)
{
    g_StampPos.uiX = x;
    g_StampPos.uiY = y;
}

void DateImprint_SetPosThumb(UINT32 x, UINT32 y)
{
    g_StampPosThumb.uiX = x;
    g_StampPosThumb.uiY = y;
}

void DateImprint_SetColor(PCOLOR_YCBCR pStampColorBg, PCOLOR_YCBCR pStampColorFr, PCOLOR_YCBCR pStampColorFg)
{
    // Stamp background color
    g_StampColorBg.uiY  = pStampColorBg->uiY;
    g_StampColorBg.uiCB = pStampColorBg->uiCB;
    g_StampColorBg.uiCR = pStampColorBg->uiCR;

    // Stamp frame color
    g_StampColorFr.uiY  = pStampColorFr->uiY;
    g_StampColorFr.uiCB = pStampColorFr->uiCB;
    g_StampColorFr.uiCR = pStampColorFr->uiCR;

    // Stamp foreground color (text body)
    g_StampColorFg.uiY  = pStampColorFg->uiY;
    g_StampColorFg.uiCB = pStampColorFg->uiCB;
    g_StampColorFg.uiCR = pStampColorFg->uiCR;
}

/**
    Set DateInfo values.

    Set DateInfo values.

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @return None.
*/
#define THUMB_WIDTH     320
#define THUMB_HEIGHT    240
#define VIRTUAL_WIDTH   1024
#define VIRTUAL_HEIGHT  1024

static void DateImprint_SetStrInfo(PDATEIMPRINT_INFO pDateImprintData, PDATEIMPRINT_INFO pDateImprintDataThumb, UINT32 flag)
{
    RTC_DATE        CurDate;
    RTC_TIME        CurTime;
    _IMGSize_sel    size;
    UINT32          uiImageWidth, uiImageHeight;
    UINT32          uiStampWidth;
    STAMP_POS       StampPos, StampPosThumb;
    COLOR_YCBCR     StampColorBg, StampColorFr, StampColorFg;

    // get RCT's Date/Time
    CurDate = rtc_getDate();
    CurTime = rtc_getTime();

    // Get stamp font database according to image size
    size = Get_SizeValue();
    uiImageWidth  = JPGIMG_SIZE[size][0];
    uiImageHeight = JPGIMG_SIZE[size][1];

#if (1)//(DRAM_END == 0x00800000) // For 8MB DRAM, use Font 36x60 only to reduce code size

    pDateImprintData->pDataBase = &gDateStampFont36x60;
    pDateImprintData->ui32FontWidth  = 36;
    pDateImprintData->ui32FontHeight = 60;

    switch (size)
    {
    case _IMGSize_15M:
    case _IMGSize_14M:
    case _IMGSize_12M:
        pDateImprintData->ui32DstHeight = 136;
        break;

    case _IMGSize_10M:
        pDateImprintData->ui32DstHeight = 124;
        break;

    case _IMGSize_9M:
        pDateImprintData->ui32DstHeight = 116;
        break;

    case _IMGSize_8M:
        pDateImprintData->ui32DstHeight = 110;
        break;

    case _IMGSize_7M:
    case _IMGSize_7M_169:
        pDateImprintData->ui32DstHeight = 102;
        break;

    case _IMGSize_6M:
        pDateImprintData->ui32DstHeight  = 96;
        break;

    case _IMGSize_5_4M:
        pDateImprintData->ui32DstHeight  = 90;
        break;

    case _IMGSize_5M:
    case _IMGSize_4_9M:
        pDateImprintData->ui32DstHeight  = 86;
        break;

    case _IMGSize_4_4M:
        pDateImprintData->ui32DstHeight  = 82;
        break;

    case _IMGSize_4M:
    case _IMGSize_3_8M:
        pDateImprintData->ui32DstHeight  = 76;
        break;

    case _IMGSize_3M:
        pDateImprintData->ui32DstHeight  = 72;
        break;

    case _IMGSize_2_6M:
        pDateImprintData->ui32DstHeight  = 66;
        break;

    case _IMGSize_2_1M:
    case _IMGSize_2M:
    case _IMGSize_2M_169:
        pDateImprintData->ui32DstHeight  = 60;
        break;

    case _IMGSize_1_5M:
        pDateImprintData->ui32DstHeight  = 48;
        break;

    case _IMGSize_1M:
        pDateImprintData->ui32DstHeight  = 44;
        break;

    case _IMGSize_0_8M:
        pDateImprintData->ui32DstHeight  = 38;
        break;

    case _IMGSize_0_6M:
        pDateImprintData->ui32DstHeight  = 32;
        break;

    case _IMGSize_0_4M:
    case _IMGSize_VGA:
    case _IMGSize_0_2M:
        pDateImprintData->ui32DstHeight  = 30;
        break;

    default:
        pDateImprintData->ui32DstHeight  = 96;
        break;
    }

#else

    switch (size)
    {
    case _IMGSize_15M:
    case _IMGSize_14M:
    case _IMGSize_12M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 136;
        break;

    case _IMGSize_10M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 124;
        break;

    case _IMGSize_9M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 116;
        break;

    case _IMGSize_8M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 110;
        break;

    case _IMGSize_7M:
    case _IMGSize_7M_169:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 102;
        break;

    case _IMGSize_6M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight  = 96;
        break;

    case _IMGSize_5_4M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 90;
        break;

    case _IMGSize_5M:
    case _IMGSize_4_9M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 86;
        break;

    case _IMGSize_4_4M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 82;
        break;

    case _IMGSize_4M:
    case _IMGSize_3_8M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 76;
        break;

    case _IMGSize_3M:
        pDateImprintData->pDataBase = &gDateStampFont42x72;
        pDateImprintData->ui32FontWidth  = 42;
        pDateImprintData->ui32FontHeight = 72;
        pDateImprintData->ui32DstHeight  = 72;
        break;

    case _IMGSize_2_6M:
        pDateImprintData->pDataBase = &gDateStampFont42x72;
        pDateImprintData->ui32FontWidth  = 42;
        pDateImprintData->ui32FontHeight = 72;
        pDateImprintData->ui32DstHeight  = 66;
        break;

    case _IMGSize_2_1M:
    case _IMGSize_2M:
    case _IMGSize_2M_169:
        pDateImprintData->pDataBase = &gDateStampFont36x60;
        pDateImprintData->ui32FontWidth  = 36;
        pDateImprintData->ui32FontHeight = 60;
        pDateImprintData->ui32DstHeight  = 60;
        break;

    case _IMGSize_1_5M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 48;
        break;

    case _IMGSize_1M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 44;
        break;

    case _IMGSize_0_8M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 38;
        break;

    case _IMGSize_0_6M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 32;
        break;

    case _IMGSize_0_4M:
    case _IMGSize_VGA:
    case _IMGSize_0_2M:
        pDateImprintData->pDataBase = &gDateStampFont18x30;
        pDateImprintData->ui32FontWidth  = 18;
        pDateImprintData->ui32FontHeight = 30;
        pDateImprintData->ui32DstHeight  = 30;
        break;

    default:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight  = 96;
        break;
    }

#endif

    // Stamp font database and size for screen-nail (320x240)
    pDateImprintDataThumb->pDataBase      = &gDateStampFont10x16;
    pDateImprintDataThumb->ui32FontWidth  = 10;
    pDateImprintDataThumb->ui32FontHeight = pDateImprintDataThumb->ui32DstHeight = 16;

    uiStampWidth = (pDateImprintData->ui32DstHeight * pDateImprintData->ui32FontWidth) / pDateImprintData->ui32FontHeight;

    // Prepare stamp date-time strings
    if ((flag & STAMP_DATE_TIME_MASK) == STAMP_DATE)
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.day, CurDate.s.month, CurDate.s.year);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d", CurDate.s.day, CurDate.s.month, CurDate.s.year);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.month, CurDate.s.day, CurDate.s.year);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d", CurDate.s.month, CurDate.s.day, CurDate.s.year);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day);
            sprintf(pDateImprintDataThumb->pi8Str, "%04d/%02d/%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day);
            break;
        }
    }
    else
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d %02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute);
            sprintf(pDateImprintDataThumb->pi8Str, "%04d/%02d/%02d %02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute);
            break;
        }
    }

    // Set date stamp position
    if ((flag & STAMP_OPERATION_MASK) == STAMP_MANUAL)
    {
        StampPosThumb.uiX = g_StampPosThumb.uiX;
        StampPosThumb.uiY = g_StampPosThumb.uiY;
        StampPos.uiX = g_StampPos.uiX;
        StampPos.uiY = g_StampPos.uiY;
    }
    else
    {
        // Primary image is related to virtual coordinate 1024 x 1024
        // Thumbnail (Screen-nail) is related to real size 320 x 240
        switch (flag & STAMP_POSITION_MASK)
        {
        case STAMP_BOTTOM_LEFT:
            StampPos.uiX = (uiStampWidth * 2 * VIRTUAL_WIDTH) / uiImageWidth;
            StampPosThumb.uiX = (pDateImprintDataThumb->ui32FontWidth * 2 * VIRTUAL_WIDTH) / THUMB_WIDTH;
            break;

        //case STAMP_BOTTOM_RIGHT:
        default:
            StampPos.uiX = (uiImageWidth - uiStampWidth * (strlen(pDateImprintData->pi8Str) + 2)) * VIRTUAL_WIDTH / uiImageWidth;
            StampPosThumb.uiX = THUMB_WIDTH - pDateImprintDataThumb->ui32FontWidth * (strlen(pDateImprintDataThumb->pi8Str) + 2);
            break;
        }

        StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight * 2) * VIRTUAL_HEIGHT / uiImageHeight;
        //StampPosThumb.uiY = THUMB_HEIGHT - pDateImprintDataThumb->ui32FontHeight * 2;
        StampPosThumb.uiY = 800;
    }

    datastamp_SetPosition(StampPos.uiX, StampPos.uiY);
    datastamp_SetPositionThumb(StampPosThumb.uiX, StampPosThumb.uiY);
    //datastamp_SetPositionThumb(448, 922); //KS Hung 2011/08/06@Temp for hard code

    // Set date stamp color
    if ((flag & STAMP_OPERATION_MASK) == STAMP_MANUAL)
    {
        // Stamp background color
        StampColorBg.uiY  = g_StampColorBg.uiY;
        StampColorBg.uiCB = g_StampColorBg.uiCB;
        StampColorBg.uiCR = g_StampColorBg.uiCR;
        // Stamp frame color
        StampColorFr.uiY  = g_StampColorFr.uiY;
        StampColorFr.uiCB = g_StampColorFr.uiCB;
        StampColorFr.uiCR = g_StampColorFr.uiCR;
        // Stamp foreground color (text body)
        StampColorFg.uiY  = g_StampColorFg.uiY;
        StampColorFg.uiCB = g_StampColorFg.uiCB;
        StampColorFg.uiCR = g_StampColorFg.uiCR;
    }
    else
    {
        // Stamp background color: color index 0, transparent
        StampColorBg.uiY  = COLOR_BG_Y;
        StampColorBg.uiCB = COLOR_BG_U;
        StampColorBg.uiCR = COLOR_BG_V;
        // Stamp frame color: color index 0, transparent
        StampColorFr.uiY  = COLOR_BG_Y;
        StampColorFr.uiCB = COLOR_BG_U;
        StampColorFr.uiCR = COLOR_BG_V;
        // Stamp foreground color (text body)
        StampColorFg.uiY  = g_StampColorFg.uiY;
        StampColorFg.uiCB = g_StampColorFg.uiCB;
        StampColorFg.uiCR = g_StampColorFg.uiCR;
    }

    pDateImprintData->colormap[0].uiY   = pDateImprintDataThumb->colormap[0].uiY   = StampColorBg.uiY;
    pDateImprintData->colormap[0].uiCB  = pDateImprintDataThumb->colormap[0].uiCB  = StampColorBg.uiCB;
    pDateImprintData->colormap[0].uiCR  = pDateImprintDataThumb->colormap[0].uiCR  = StampColorBg.uiCR;
    pDateImprintData->colormap[1].uiY   = pDateImprintDataThumb->colormap[1].uiY   = StampColorFr.uiY;
    pDateImprintData->colormap[1].uiCB  = pDateImprintDataThumb->colormap[1].uiCB  = StampColorFr.uiCB;
    pDateImprintData->colormap[1].uiCR  = pDateImprintDataThumb->colormap[1].uiCR  = StampColorFr.uiCR;
    pDateImprintData->colormap[2].uiY   = pDateImprintDataThumb->colormap[2].uiY   = StampColorFg.uiY;
    pDateImprintData->colormap[2].uiCB  = pDateImprintDataThumb->colormap[2].uiCB  = StampColorFg.uiCB;
    pDateImprintData->colormap[2].uiCR  = pDateImprintDataThumb->colormap[2].uiCR  = StampColorFg.uiCR;
}

/**
    Open Date Imprint.

    Open Date Imprint.

    @param None.
    @return None.
*/
static void DateImprint_Open(void)
{
    SysGetDateStampAddr(&uiMemoryAddr);
    SysGetDateStampThumbAddr(&uiMemoryAddrThumb);
    //#NT#2011/04/29#Ethan Lau -begin
    //#NT#220todo , plz reference datastamp.h
    datastamp_Enable(TRUE);
    datastampThumb_Enable(TRUE);
    //#NT#2011/04/29#Ethan Lau -end
}

/**
    Close Date Imprint.

    Close Date Imprint.

    @param None.
    @return None.
*/
static void DateImprint_Close(void)
{
    //#NT#2011/04/29#Ethan Lau -begin
    //#NT#220todo , plz reference datastamp.h
    datastamp_Enable(FALSE);
    datastampThumb_Enable(FALSE);
    //#NT#2011/04/29#Ethan Lau -end
}

/**
    DateImprint_Gen

    Date Imprint Date Generation.

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
*/
static void DateImprint_Gen(PDATEIMPRINT_INFO DateImprintData)
{
    UINT32  i, j, k;
    UINT32  ui32Y_Addr, ui32CB_Addr, ui32CR_Addr;
    UINT32  uiLineOffset;
    UINT32  ui32StrLen, uiDateHeight;
    UINT32  ui32CB_Addr2, ui32CR_Addr2;
    DS_STAMP_INFOR StampInfo;

    ui32StrLen = strlen(DateImprintData->pi8Str);
    uiDateHeight = DateImprintData->ui32FontHeight;
    uiLineOffset = DateImprintData->ui32FontWidth * ui32StrLen;

    /*
    if (uiLineOffset % 16 != 0)
    {
        uiLineOffset = ((uiLineOffset >> 4) + 1) << 4;
    }
    */

    if (uiLineOffset&0x7)
    {
        uiLineOffset = (uiLineOffset&0xfffffff8)+0x8;
    }

    DateImprint_GenStr_ToMem(DateImprintData, &ui32Y_Addr, &ui32CB_Addr, &ui32CR_Addr, uiMemoryAddr, uiLineOffset);

    {
        ui32CB_Addr2 = ui32CB_Addr;
        ui32CR_Addr2 = ui32CR_Addr;
        j = ui32CB_Addr;
        k = ui32CR_Addr;
        for (i = 0; i < uiLineOffset * uiDateHeight; i += 2)
        {
            memcpy((UINT8 *)j, (UINT8 *)ui32CB_Addr2, 1);
            memcpy((UINT8 *)k, (UINT8 *)ui32CR_Addr2, 1);
            j ++;
            k ++;
            ui32CB_Addr2 +=2;
            ui32CR_Addr2 +=2;
        }
    }

    StampInfo.AddrY      = ui32Y_Addr;
    StampInfo.AddrCb     = ui32CB_Addr;
    StampInfo.AddrCr     = ui32CR_Addr;
    StampInfo.LineOffset = uiLineOffset;
    StampInfo.Width      = uiLineOffset;
    StampInfo.Height     = DateImprintData->ui32FontHeight;
    StampInfo.ColorKeyY  = COLOR_BG_Y;
    StampInfo.ColorKeyCb = COLOR_BG_U;
    StampInfo.ColorKeyCr = COLOR_BG_V;
    StampInfo.Format     = DS_YCCFORMAT_422;
    datastamp_SetStampInfor(&StampInfo);
}

static void DateImprint_GenThumb(PDATEIMPRINT_INFO DateImprintDataThumb)
{
    UINT32  i, j, k;
    UINT32  ui32Y_Addr, ui32CB_Addr, ui32CR_Addr;
    UINT32  uiLineOffset;
    UINT32  ui32StrLen, uiDateHeight;
    UINT32  ui32CB_Addr2, ui32CR_Addr2;
    DS_STAMP_INFOR StampInfoThumb;

    ui32StrLen = strlen(DateImprintDataThumb->pi8Str);
    uiDateHeight = DateImprintDataThumb->ui32FontHeight;
    uiLineOffset = DateImprintDataThumb->ui32FontWidth * ui32StrLen;

    /*
    if (uiLineOffset % 16 != 0)
    {
        uiLineOffset = ((uiLineOffset >> 4) + 1) << 4;
    }
    */

    if (uiLineOffset&0x7)
    {
        uiLineOffset = (uiLineOffset&0xfffffff8)+0x8;
    }

    DateImprint_GenStr_ToMem(DateImprintDataThumb, &ui32Y_Addr, &ui32CB_Addr, &ui32CR_Addr, uiMemoryAddrThumb, uiLineOffset);

    // 444->422
    ui32CB_Addr2 = ui32CB_Addr;
    ui32CR_Addr2 = ui32CR_Addr;
    j = ui32CB_Addr;
    k = ui32CR_Addr;

    for (i = 0; i < uiLineOffset * uiDateHeight; i += 2)
    {
        memcpy((UINT8 *)j, (UINT8 *)ui32CB_Addr2, 1);
        memcpy((UINT8 *)k, (UINT8 *)ui32CR_Addr2, 1);
        j ++;
        k ++;
        ui32CB_Addr2 +=2;
        ui32CR_Addr2 +=2;
    }
    StampInfoThumb.AddrY      = ui32Y_Addr;
    StampInfoThumb.AddrCb     = ui32CB_Addr;
    StampInfoThumb.AddrCr     = ui32CR_Addr;
    StampInfoThumb.LineOffset = uiLineOffset;
    StampInfoThumb.Width      = uiLineOffset;
    StampInfoThumb.Height     = DateImprintDataThumb->ui32FontHeight;
    StampInfoThumb.ColorKeyY  = COLOR_BG_Y;
    StampInfoThumb.ColorKeyCb = COLOR_BG_U;
    StampInfoThumb.ColorKeyCr = COLOR_BG_V;
    StampInfoThumb.Format     = DS_YCCFORMAT_422;
    datastamp_SetStampInforThumb(&StampInfoThumb);
}

/**
    DateImprint_GenStr_ToMem

    @param[in]  PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[out] UINT *puiY_Addr the Y starting address of this data generation.
    @param[out] UINT *puiCb_Addr the CB starting address of this data generation.
    @param[out] UINT *puiCr_Addr the CR starting address of this data generation.
    @param[in]  UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenStr_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiMemAddr, UINT32 uiLineOffset)
{
    UINT8   uiStrLen;
    UINT32  i, j;
    ICON_DB const *pDB;
    UINT16  uiIconID;
    UINT16  uiStroffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;

    uiStrLen = strlen(DateImprintData->pi8Str);
    //pointer to font data base
    pDB = DateImprintData->pDataBase;
    uiStroffset = pDB->uiDrawStrOffset;

    uiFBAddrY = uiMemAddr;
    uiFBAddrCb = uiFBAddrY + uiLineOffset * DateImprintData->ui32FontHeight;
    uiFBAddrCr = uiFBAddrCb + uiLineOffset * DateImprintData->ui32FontHeight;

    *puiY_Addr = uiFBAddrY;
    *puiCb_Addr = uiFBAddrCb;
    *puiCr_Addr = uiFBAddrCr;

    memset((UINT8 *)uiFBAddrY,  COLOR_BG_Y, uiLineOffset * DateImprintData->ui32FontHeight);
    memset((UINT8 *)uiFBAddrCb, COLOR_BG_U, uiLineOffset * DateImprintData->ui32FontHeight);
    memset((UINT8 *)uiFBAddrCr, COLOR_BG_V, uiLineOffset * DateImprintData->ui32FontHeight);

    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = DateImprintData->pi8Str[i] - uiStroffset ;

        j = pDB ->pIconHeader[uiIconID].uiWidth;

        if (j % 2 != 0)
        {
            j++;
        }

        DateImprint_GenIcon_ToMem(DateImprintData, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, uiLineOffset);
        uiFBAddrY  += (j);
        uiFBAddrCb += (j);
        uiFBAddrCr += (j);
    }

    return TRUE;
}

/**
    DateImprint_GenIcon_ToMem

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[in] UINT16 uiIconID Icon ID.
    @param[in] UINT uiFBY the CB starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenIcon_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    ICON_HEADER pIconHeader;
    UINT16  uiIconWidth;
    UINT16  uiIconHeight;
    UINT8   const *uiIconData;
    UINT16  uipixelcount;
    UINT32  i, j;
    UINT32  uiFBY_tmp, uiFBCb_tmp, uiFBCr_tmp;
    UINT8   uiIndex;
    ICON_DB const *pDB;

    pDB = DateImprintData->pDataBase;
    pIconHeader = pDB ->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB ->pIconData) + (pIconHeader.uiOffset);

    uipixelcount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        uiFBY_tmp = uiFBY;
        uiFBCb_tmp = uiFBCb;
        uiFBCr_tmp = uiFBCr;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;

            memset((UINT8 *)uiFBY_tmp, DateImprintData->colormap[uiIndex].uiY, 1);
            memset((UINT8 *)uiFBCb_tmp, DateImprintData->colormap[uiIndex].uiCB, 1);
            memset((UINT8 *)uiFBCr_tmp, DateImprintData->colormap[uiIndex].uiCR, 1);

            uiFBY_tmp += 1;
            uiFBCb_tmp += 1;
            uiFBCr_tmp += 1;

            uipixelcount++;
            if (uipixelcount % 4 == 0)
            {
                uiIconData++;
            }
        }
        uiFBY += uiLineOffset;
        uiFBCb += uiLineOffset;
        uiFBCr += uiLineOffset;
    }
    return TRUE;
}

/**
    DateImprint_Gen_Scalar_FW

    Date Imprint Date Generation with scaling function executed by FW

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[in] UINT32 uiDstHeight the wanted height of final icon image
*/
static void DateImprint_Gen_Scalar_FW(PDATEIMPRINT_INFO DateImprintData)
{
    UINT32 ui32Y_Addr, ui32CB_Addr, ui32CR_Addr;
    UINT32 ui32CB_Addr2, ui32CR_Addr2;
    UINT32 uiDstLineOffset;
    UINT32 ui32StrLen;
    UINT32 i, j, k, line;
    DS_STAMP_INFOR StampInfo;

    ui32StrLen = strlen(DateImprintData->pi8Str);

    uiDstLineOffset = ((DateImprintData->ui32FontWidth * ui32StrLen) * DateImprintData->ui32DstHeight) / DateImprintData->ui32FontHeight;

    if (uiDstLineOffset&0x7)
    {
        uiDstLineOffset = (uiDstLineOffset&0xfffffff8)+0x8;
    }

    DateImprint_GenStr_ToMem_Scalar_FW(DateImprintData, &ui32Y_Addr, &ui32CB_Addr, &ui32CR_Addr, uiDstLineOffset, DateImprintData->ui32DstHeight);

    if (DateImprintData->ui32DstFormat == DS_YCCFORMAT_422)
    {//444->422
        ui32CB_Addr2 = ui32CB_Addr;
        ui32CR_Addr2 = ui32CR_Addr;
        j = ui32CB_Addr;
        k = ui32CR_Addr;
        for (i = 0; i < uiDstLineOffset * DateImprintData->ui32DstHeight; i += 2)
        {
            memcpy((UINT8 *)j, (UINT8 *)ui32CB_Addr2, 1);
            memcpy((UINT8 *)k, (UINT8 *)ui32CR_Addr2, 1);
            j ++;
            k ++;
            ui32CB_Addr2 +=2;
            ui32CR_Addr2 +=2;
        }
    }
    else if (DateImprintData->ui32DstFormat == DS_YCCFORMAT_420)
    {//444->420
        ui32CB_Addr2 = ui32CB_Addr;
        ui32CR_Addr2 = ui32CR_Addr;
        j = ui32CB_Addr;
        k = ui32CR_Addr;
        line = 0;
        for (i = 0; i < uiDstLineOffset * DateImprintData->ui32DstHeight; i += 2)
        {
            line = (i/uiDstLineOffset)& 0x1;

            //ui32CB_Addr2 +=2;
            //ui32CR_Addr2 +=2;
            if (line == 0)
            {
                memcpy((UINT8 *)j, (UINT8 *)ui32CB_Addr2, 1);
                memcpy((UINT8 *)k, (UINT8 *)ui32CR_Addr2, 1);
                j ++;
                k ++;
            }
            ui32CB_Addr2 +=2;
            ui32CR_Addr2 +=2;
        }
    }

    StampInfo.AddrY      = ui32Y_Addr;
    StampInfo.AddrCb     = ui32CB_Addr;
    StampInfo.AddrCr     = ui32CR_Addr;
    StampInfo.LineOffset = uiDstLineOffset;
    StampInfo.Width      = uiDstLineOffset;
    StampInfo.Height     = DateImprintData->ui32DstHeight;
    StampInfo.ColorKeyY  = COLOR_BG_Y;
    StampInfo.ColorKeyCb = COLOR_BG_U;
    StampInfo.ColorKeyCr = COLOR_BG_V;
    //StampInfo.Format     = DS_YCCFORMAT_422;
    StampInfo.Format     = DateImprintData->ui32DstFormat;
    datastamp_SetStampInfor(&StampInfo);

}

/**
    DateImprint_GenIcon_ToMem_Scalar_FW

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[in] UINT16 uiIconID Icon ID.
    @param[in] UINT uiFBY the CB starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenIcon_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset, UINT32 uiIconWidth, UINT32 uiIconHeight)
{
    ICON_HEADER pIconHeader;
    UINT16  uiIconOriWidth;
    UINT16  uiIconOriHeight;
    UINT8   const *uiIconData;
    UINT16  uipixelcount;
    UINT32  i, j, k;
    UINT32  uiFBY_tmp,uiFBCb_tmp,uiFBCr_tmp;
    UINT8   uiIndex;
    ICON_DB const *pDB;
    UINT32  uiLastX, uiLastY, uiCurrX, uiCurrY;
    UINT8   uiYtmp, uiCBtmp, uiCRtmp;
    UINT8   uiActiveX, uiActiveY;
    COLOR_YCBCR *pCMtmp;

    pCMtmp = DateImprintData->colormap;
    pDB = DateImprintData->pDataBase;
    pIconHeader = pDB ->pIconHeader[uiIconID];
    uiIconOriWidth = pIconHeader.uiWidth;
    uiIconOriHeight = pIconHeader.uiHeight;
    uiIconData = (pDB ->pIconData) + (pIconHeader.uiOffset);

    if(uiIconOriHeight>uiIconHeight)
    {//scaling down
        uipixelcount = 0;
        {//Y initial
            uiLastY = uiCurrY = 0;
        }
        for(i = 0; i < uiIconOriHeight; i++)
        {
            uiActiveY = (uiCurrY>=uiLastY)?1:0;

            {//X initial
                uiLastX = uiCurrX = 0;

                uiFBY_tmp = uiFBY;
                uiFBCb_tmp = uiFBCb;
                uiFBCr_tmp = uiFBCr;
            }

            for(j = 0; j < uiIconOriWidth; j ++)
            {
                uiActiveX = (uiCurrX>=uiLastX)?1:0;

                if(uiActiveX && uiActiveY)
                {//Copy this pixel
                    uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;
                    uiYtmp  = (*(pCMtmp+uiIndex)).uiY;
                    uiCBtmp = (*(pCMtmp+uiIndex)).uiCB;
                    uiCRtmp = (*(pCMtmp+uiIndex)).uiCR;
                    memset((UINT8 *)uiFBY_tmp, uiYtmp, 1);
                    memset((UINT8 *)uiFBCb_tmp, uiCBtmp, 1);
                    memset((UINT8 *)uiFBCr_tmp, uiCRtmp, 1);
                }

                {//moving input address
                    uipixelcount ++;
                    if (uipixelcount % 4 == 0)
                        uiIconData ++;
                }

                {//X accumulation
                    if(uiActiveX)
                    {
                        uiLastX += uiIconOriWidth;
                        uiFBY_tmp ++;
                        uiFBCb_tmp ++;
                        uiFBCr_tmp ++;
                    }
                    uiCurrX += uiIconWidth;
                }
            }
            {//Y accumulation
                if(uiActiveY)
                {
                    uiLastY += uiIconOriHeight;
                    uiFBY += uiLineOffset;
                    uiFBCb += uiLineOffset;
                    uiFBCr += uiLineOffset;
                }
                uiCurrY += uiIconHeight;
            }
        }
    }
    else
    {//scaling up
        uipixelcount = 0;
        {//Y initial
            uiCurrY = 0;
        }
        for(i = 0; i < uiIconOriHeight; i++)
        {
            {//X initial
                uiCurrX = 0;

                uiFBY_tmp  = uiFBY;
                uiFBCb_tmp = uiFBCb;
                uiFBCr_tmp = uiFBCr;
            }

            for(j = 0; j < uiIconOriWidth; j ++)
            {
                {//X computing
                    uiCurrX += uiIconWidth;
                    uiActiveX = uiCurrX/uiIconOriWidth;
                    uiCurrX -= uiActiveX*uiIconOriWidth;
                }

                {//Copy this pixel in X direction
                    uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;
                    uiYtmp  = (*(pCMtmp+uiIndex)).uiY;
                    uiCBtmp = (*(pCMtmp+uiIndex)).uiCB;
                    uiCRtmp = (*(pCMtmp+uiIndex)).uiCR;
                    memset((UINT8 *)uiFBY_tmp,  uiYtmp,  uiActiveX);
                    memset((UINT8 *)uiFBCb_tmp, uiCBtmp, uiActiveX);
                    memset((UINT8 *)uiFBCr_tmp, uiCRtmp, uiActiveX);
                }

                {//moving input address
                    uipixelcount ++;
                    if (uipixelcount % 4 == 0)
                        uiIconData ++;
                }

                {//X accumulation
                    uiFBY_tmp  +=uiActiveX;
                    uiFBCb_tmp +=uiActiveX;
                    uiFBCr_tmp +=uiActiveX;
                }
            }

            {//Y computing
                uiCurrY += uiIconHeight;
                uiActiveY = uiCurrY/uiIconOriHeight;
                uiCurrY -= uiActiveY*uiIconOriHeight;
            }

            {//copy in Y direction
                {
                    uiFBY_tmp  = uiFBY;
                    uiFBCb_tmp = uiFBCb;
                    uiFBCr_tmp = uiFBCr;
                }
                for(k=1; k<uiActiveY; k++)
                {//copy in Y direction
                    uiFBY_tmp  += uiLineOffset;
                    uiFBCb_tmp += uiLineOffset;
                    uiFBCr_tmp += uiLineOffset;
                    memcpy((void *)uiFBY_tmp,  (void *)uiFBY,  uiIconWidth);
                    memcpy((void *)uiFBCb_tmp, (void *)uiFBCb, uiIconWidth);
                    memcpy((void *)uiFBCr_tmp, (void *)uiFBCr, uiIconWidth);
                }
            }

            {//Y accumulation
                uiFBY  += uiActiveY*uiLineOffset;
                uiFBCb += uiActiveY*uiLineOffset;
                uiFBCr += uiActiveY*uiLineOffset;
            }
        }
    }

    return TRUE;
}

/**
    DateImprint_GenStr_ToMem_Scalar_FW

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[out] UINT *puiY_Addr the Y starting address of this data generation.
    @param[out] UINT *puiCb_Addr the CB starting address of this data generation.
    @param[out] UINT *puiCr_Addr the CR starting address of this data generation.
    @param[in] UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenStr_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiDstLineOffset, UINT32 uiDstHeight)
{
    UINT8   uiStrLen;
    UINT32  i;
    ICON_DB const *pDB;
    UINT16  uiIconID;
    UINT16  uiStroffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
    UINT16  uiIconWidth = 0;

    uiStrLen = strlen(DateImprintData->pi8Str);

    //pointer to font data base
    pDB = DateImprintData->pDataBase;
    uiStroffset = pDB->uiDrawStrOffset;

//    uiFBAddrY = uiMemoryAddr;
    uiFBAddrY = DateImprintData->ui32PutActiveAddr;
    uiFBAddrCb = uiFBAddrY + uiDstLineOffset * uiDstHeight;
    uiFBAddrCr = uiFBAddrCb + uiDstLineOffset * uiDstHeight;

    *puiY_Addr = uiFBAddrY;
    *puiCb_Addr = uiFBAddrCb;
    *puiCr_Addr = uiFBAddrCr;

    memset((UINT8 *)uiFBAddrY,  COLOR_BG_Y, uiDstLineOffset * uiDstHeight);
    memset((UINT8 *)uiFBAddrCb, COLOR_BG_U, uiDstLineOffset * uiDstHeight);
    memset((UINT8 *)uiFBAddrCr, COLOR_BG_V, uiDstLineOffset * uiDstHeight);

    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = DateImprintData->pi8Str[i] - uiStroffset;

        if(pDB ->pIconHeader[uiIconID].uiHeight != 0)
        {
            uiIconWidth = (pDB ->pIconHeader[uiIconID].uiWidth*uiDstHeight)
                          /pDB ->pIconHeader[uiIconID].uiHeight;
        }

        DateImprint_GenIcon_ToMem_Scalar_FW(DateImprintData, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, uiDstLineOffset, uiIconWidth, uiDstHeight);
        uiFBAddrY  += (uiIconWidth);
        uiFBAddrCb += (uiIconWidth);
        uiFBAddrCr += (uiIconWidth);
    }

    return TRUE;
}

UINT32 gGenMovieDID_flag = 0;
UINT32 gGenMovieAddr1 = 0, gGenMovieAddr2 =0;
DATEIMPRINT_INFO    gGenMovieDateInfo;
char                gGenMovieDSStr[98];
UINT32 gGneMovieLastSec = 0;
UINT8  gGenMovieOpen = FALSE;
char carnumber_stamp_str[CAR_NUMBER_STAMP_LEN] = {0};

void GenMovieDID_Open(void)
{
    SysGetMovieDSAddr(&gGenMovieAddr1);
    SysGetMovieDSAddr_2nd(&gGenMovieAddr2);
    datastamp_Enable(TRUE);
    Photo_VidstampEnable(TRUE);
    gGenMovieOpen = TRUE;
}

void GenMovieDID_Close(void)
{
    datastamp_Enable(FALSE);
    Photo_VidstampEnable(FALSE);
    gGenMovieOpen = FALSE;
}

void GenMovieDID_SetStrInfo(PDATEIMPRINT_INFO pDateImprintData, UINT32 flag)
{
    RTC_DATE        CurDate;
    RTC_TIME        CurTime;
    UINT32          uiImageWidth, uiImageHeight;
    UINT32          uiStampWidth;
    STAMP_POS       StampPos;
    COLOR_YCBCR     StampColorBg, StampColorFr, StampColorFg;

    // get RCT's Date/Time
    CurDate = rtc_getDate();
    CurTime = rtc_getTime();
    gGneMovieLastSec = CurTime.s.second;
    memset(carnumber_stamp_str, 0, CAR_NUMBER_STAMP_LEN);
	if (SysGetFlag(FL_PLATENUMBER) == PLATENUMBER_ON)
	{
		/*if (SysGetFlag(FL_LANGUAGE) == LANG_SC)
		{
			strcpy(carnumber_stamp_str, SysGetZHCarNoStamp());
		}
		else
		{
			strcpy(carnumber_stamp_str, SysGetNZHCarNoStamp());
		}*/
		strcpy(carnumber_stamp_str, SysGetNZHCarNoStamp());
	}
	else
	{
		sprintf(carnumber_stamp_str," ");
	}
    #if 0
    // always use 10x16 font size temporary
    pDateImprintData->pDataBase = &gDateStampFont10x16;
    pDateImprintData->ui32FontWidth  = 10;
    pDateImprintData->ui32FontHeight = 16;
    pDateImprintData->ui32DstHeight  = 16;
    #elif 1
    pDateImprintData->pDataBase = &gDateStampFont12x20;
    pDateImprintData->ui32FontWidth  = 12;
    pDateImprintData->ui32FontHeight = 20;
    pDateImprintData->ui32DstHeight  = 20;
    #else
    pDateImprintData->pDataBase = &gDateStampFont18x30;
    pDateImprintData->ui32FontWidth  = 18;
    pDateImprintData->ui32FontHeight = 30;
    pDateImprintData->ui32DstHeight  = 30;
    #endif

    uiImageWidth  = GetMovieSizeWidth(SysGetFlag(FL_MOVIE_SIZE));
    uiImageHeight = GetMovieSizeHeight(SysGetFlag(FL_MOVIE_SIZE));

    uiStampWidth = (pDateImprintData->ui32DstHeight * pDateImprintData->ui32FontWidth) / pDateImprintData->ui32FontHeight;

    // Prepare stamp date-time strings
    if ((flag & STAMP_DATE_TIME_MASK) == STAMP_DATE)
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.day, CurDate.s.month, CurDate.s.year);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.month, CurDate.s.day, CurDate.s.year);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day);
            break;
        }
    }
    else if ((flag & STAMP_DATE_TIME_MASK) == STAMP_TIME)
    {
        sprintf(pDateImprintData->pi8Str, "%02d:%02d:%02d", CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
    }
    else//STAMP_DATE_TIME
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            //sprintf(pDateImprintData->pi8Str, "%s  %02d/%02d/%04d %02d:%02d:%02d", carnumber_stamp_str,CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d %02d:%02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
            break;
        }
    }

    // Set date stamp position
    if ((flag & STAMP_OPERATION_MASK) == STAMP_MANUAL)
    {
        StampPos.uiX = g_StampPos.uiX;
        StampPos.uiY = g_StampPos.uiY;
    }
    else
    {
        // Primary image is related to virtual coordinate 1024 x 1024
        switch (flag & STAMP_POSITION_MASK)
        {
        case STAMP_BOTTOM_LEFT:
            StampPos.uiX = (uiStampWidth * 2 * VIRTUAL_WIDTH) / uiImageWidth;
            break;

        //case STAMP_BOTTOM_RIGHT:
        default:
            StampPos.uiX = (uiImageWidth - uiStampWidth * (strlen(pDateImprintData->pi8Str) + 2)) * VIRTUAL_WIDTH / uiImageWidth;
            break;
        }

        if (SysGetFlag(FL_MOVIE_SIZE)==MOVIE_SIZE_QVGA)
            StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight*3) * VIRTUAL_HEIGHT / uiImageHeight;
        else if(SysGetFlag(FL_MOVIE_SIZE)==MOVIE_SIZE_1080FHD)			
            StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight*3) * VIRTUAL_HEIGHT / uiImageHeight;
        else if(SysGetFlag(FL_MOVIE_SIZE)==MOVIE_SIZE_1080P)			
            StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight*3) * VIRTUAL_HEIGHT / uiImageHeight;		
        else if(SysGetFlag(FL_MOVIE_SIZE)==MOVIE_SIZE_720P)			
            StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight*2) * VIRTUAL_HEIGHT / uiImageHeight;		
        else if(SysGetFlag(FL_MOVIE_SIZE)==MOVIE_SIZE_WVGA)			
            StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight*3) * VIRTUAL_HEIGHT / uiImageHeight;	
	else if(SysGetFlag(FL_MOVIE_SIZE)==MOVIE_SIZE_VGA)			
            StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight*3) * VIRTUAL_HEIGHT / uiImageHeight;			
        else
            StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight*4) * VIRTUAL_HEIGHT / uiImageHeight;					
        //StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight) * VIRTUAL_HEIGHT / uiImageHeight;
        //StampPos.uiY = 0;
    }

    datastamp_SetPosition(StampPos.uiX, StampPos.uiY);

    // Set date stamp color
    if ((flag & STAMP_OPERATION_MASK) == STAMP_MANUAL)
    {
        // Stamp background color
        StampColorBg.uiY  = g_StampColorBg.uiY;
        StampColorBg.uiCB = g_StampColorBg.uiCB;
        StampColorBg.uiCR = g_StampColorBg.uiCR;
        // Stamp frame color
        StampColorFr.uiY  = g_StampColorFr.uiY;
        StampColorFr.uiCB = g_StampColorFr.uiCB;
        StampColorFr.uiCR = g_StampColorFr.uiCR;
        // Stamp foreground color (text body)
        StampColorFg.uiY  = g_StampColorFg.uiY;
        StampColorFg.uiCB = g_StampColorFg.uiCB;
        StampColorFg.uiCR = g_StampColorFg.uiCR;
    }
    else
    {
        // Stamp background color: black
        StampColorBg.uiY  = COLOR_BG_Y;
        StampColorBg.uiCB = COLOR_BG_U;
        StampColorBg.uiCR = COLOR_BG_V;
        // Stamp frame color: black
        StampColorFr.uiY  = COLOR_BG_Y;
        StampColorFr.uiCB = COLOR_BG_U;
        StampColorFr.uiCR = COLOR_BG_V;
        // Stamp foreground color (text body)
        StampColorFg.uiY  = g_StampColorFg.uiY;
        StampColorFg.uiCB = g_StampColorFg.uiCB;
        StampColorFg.uiCR = g_StampColorFg.uiCR;
    }

    pDateImprintData->colormap[0].uiY   = StampColorBg.uiY;
    pDateImprintData->colormap[0].uiCB  = StampColorBg.uiCB;
    pDateImprintData->colormap[0].uiCR  = StampColorBg.uiCR;
    pDateImprintData->colormap[1].uiY   = StampColorFr.uiY;
    pDateImprintData->colormap[1].uiCB  = StampColorFr.uiCB;
    pDateImprintData->colormap[1].uiCR  = StampColorFr.uiCR;
    pDateImprintData->colormap[2].uiY   = StampColorFg.uiY;
    pDateImprintData->colormap[2].uiCB  = StampColorFg.uiCB;
    pDateImprintData->colormap[2].uiCR  = StampColorFg.uiCR;
}

void GenMovieDID_Setup(UINT32 flag, UINT32 dstFormat)
{
    if ((flag & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        GenMovieDID_Close();
    }
    else
    {
        GenMovieDID_Open();
        gGenMovieDateInfo.pi8Str = gGenMovieDSStr;
        gGenMovieDateInfo.ui32DstFormat = dstFormat;
        //Perf_Open();
        //Perf_Mark();
        GenMovieDID_SetStrInfo(&gGenMovieDateInfo, flag);
        gGenMovieDateInfo.ui32PutStringAddr1 = gGenMovieAddr1;
        gGenMovieDateInfo.ui32PutStringAddr2 = gGenMovieAddr2;
        gGenMovieDateInfo.ui32PutActiveAddr = gGenMovieAddr1;
        DateImprint_Gen_Scalar_FW(&gGenMovieDateInfo);
        //debug_msg("Draw one line = %d ms\r\n", Perf_GetDuration()/1000);
        //Perf_Close();
        gGenMovieDID_flag = flag;
    }
}

void GenMovieDID_ResetShowingTime(void)
{
    gGneMovieLastSec = 61;//a value need to refresh

    GenMovieDID_ReFresh();
}

void GenMovieDID_ReFresh(void)
{
    RTC_DATE        CurDate;
    RTC_TIME        CurTime;
    DATEIMPRINT_INFO *pDateImprintData;
    UINT32 flag;
    UINT32 uiTimeCount1;

    if (gGenMovieOpen == FALSE)
        return;

    pDateImprintData = &gGenMovieDateInfo;
    flag = gGenMovieDID_flag;

    // get RCT's Date/Time
    CurDate = rtc_getDate();
    CurTime = rtc_getTime();

    if (gGneMovieLastSec == CurTime.s.second)
    {
        //debug_msg("0x%d !0x%d !\r\n", gGneMovieLastSec, CurTime.s.second);
        return;
    }
    gGneMovieLastSec = CurTime.s.second;

    // Prepare stamp date-time strings
    if ((flag & STAMP_DATE_TIME_MASK) == STAMP_DATE)
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.day, CurDate.s.month, CurDate.s.year);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.month, CurDate.s.day, CurDate.s.year);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day);
            break;
        }
    }
    else if ((flag & STAMP_DATE_TIME_MASK) == STAMP_TIME)
    {
        sprintf(pDateImprintData->pi8Str, "%02d:%02d:%02d", CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
    }
    else//STAMP_DATE_TIME
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d %02d:%02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
            break;
        }
    }
    //debug_msg("time=%s!\r\n", pDateImprintData->pi8Str);

    if (gGenMovieDateInfo.ui32PutActiveAddr == gGenMovieDateInfo.ui32PutStringAddr1)
    {
        gGenMovieDateInfo.ui32PutActiveAddr = gGenMovieDateInfo.ui32PutStringAddr2;
    }
    else
    {
        gGenMovieDateInfo.ui32PutActiveAddr = gGenMovieDateInfo.ui32PutStringAddr1;
    }
//    uiTimeCount1 = Perf_GetCurrent();
    DateImprint_Gen_Scalar_FW(&gGenMovieDateInfo);
    //debug_msg("^BDS time = %d us\r\n", Perf_GetCurrent() - uiTimeCount1);

}
//@}
