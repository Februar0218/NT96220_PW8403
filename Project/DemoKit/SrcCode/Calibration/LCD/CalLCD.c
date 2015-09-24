/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalLCD.c
    @ingroup    mIPRJAPTest

    @brief      LCD verification
                LCD verification

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <stdio.h>
#include <string.h>
#include "Kernel.h"
#include "CalLCD.h"
#include "ide.h"
#include "SysCfg.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "CalibrationTsk.h"

#if (_CALIBRATION_MODE_ == ENABLE)

#if 0
static RGB_COLOR    RGBBlack = { 0, 0, 0 };
static RGB_COLOR    RGBWhite = { 255, 255, 255 };
#endif

#if 0
/**
  Convert RGB to YCbCr

  Convert RGB color space to YCbCr
  [CalLCD internal API]

  @param PRGB_COLOR pRGB: 8 bits RGB color
  @param PYCBCR_COLOR pYCbCr [Out]: 8 bits YCbCr
  @return void
*/
static void Cal_LCDRGB2YCbCr(PRGB_COLOR pRGB, PYCBCR_COLOR pYCbCr)
{
    INT                 RMG, BMG;

    RMG = pRGB->R - pRGB->G;
    BMG = pRGB->B - pRGB->G;

    pYCbCr->Y = (UINT8)(pRGB->G + ((19 * (RMG + (49 * BMG >> 7))) >> 6));
    pYCbCr->CB = (UINT8)(((BMG - ((43 * RMG) >> 7)) >> 1) + 128);
    pYCbCr->CR = (UINT8)(((RMG - ((21 * BMG) >> 7)) >> 1) + 128);

}
#endif

/**
  Display LCD with full one color

  Display LCD with full one color
  [CalLCD internal API]

  @param PRGB_COLOR pRGB: 8 bits RGB color
  @param VIDEO_BUF_ADDR: 3 buffer address of video2
  @param UINT32 puiBufNum [In/Out]: Which buffer to display (0 or 1)
  @return void
*/
static void Cal_LCDDisplayFull(PRGB_COLOR pRGB, VIDEO_BUF_ADDR *pFB_Addr, UINT32 *puiBufNum)
{
//#NT#2007/05/04#Steven Wang -begin
//#200 only 1 buffer

#if 0
    YCBCR_COLOR         YCbCrColor;

    Cal_LCDRGB2YCbCr(pRGB, &YCbCrColor);

    if (*puiBufNum == 0)
    {
        memset((void *)pFB_Addr->B0_Y, YCbCrColor.Y, LCD_VERIFY_SIZE);
        memset((void *)pFB_Addr->B0_CB, YCbCrColor.CB, LCD_VERIFY_SIZE >> 2);
        memset((void *)pFB_Addr->B0_CR, YCbCrColor.CR, LCD_VERIFY_SIZE >> 2);

        ide_set_video2_changebuf(0);
    }
    else
    {
        memset((void *)pFB_Addr->B1_Y, YCbCrColor.Y, LCD_VERIFY_SIZE);
        memset((void *)pFB_Addr->B1_CB, YCbCrColor.CB, LCD_VERIFY_SIZE >> 2);
        memset((void *)pFB_Addr->B1_CR, YCbCrColor.CR, LCD_VERIFY_SIZE >> 2);

        ide_set_video2_changebuf(1);
    }

    (*puiBufNum) = !(*puiBufNum);

    ide_set_load();
#endif
//#NT#2007/05/04#Steven Wang -end

}

/**
  Display LCD with black/white color

  Display LCD with black/white color, one color is half left side and
  the other is half right side.
  [CalLCD internal API]

  @param VIDEO_BUF_ADDR: 3 buffer address of video2
  @param UINT32 puiBufNum [In/Out]: Which buffer to display (0 or 1)
  @param BOOL bLeftBlack:
  @return void
*/
static void Cal_LCDDisplayHalf(VIDEO_BUF_ADDR *pFB_Addr, UINT32 *puiBufNum, BOOL bLeftBlack)
{
//#NT#2007/05/04#Steven Wang -begin
//#200 only 1 buffer wait to implement

#if 0
    YCBCR_COLOR         YCbCrColorLeft, YCbCrColorRight;
    UINT32              i;

    if (bLeftBlack == TRUE)
    {
        Cal_LCDRGB2YCbCr(&RGBBlack, &YCbCrColorLeft);
        Cal_LCDRGB2YCbCr(&RGBWhite, &YCbCrColorRight);
    }
    else
    {
        Cal_LCDRGB2YCbCr(&RGBBlack, &YCbCrColorRight);
        Cal_LCDRGB2YCbCr(&RGBWhite, &YCbCrColorLeft);
    }

    if (*puiBufNum == 0)
    {
        for (i=0; i<LCD_VERIFY_HEIGHT; i++)
        {
            memset((void *)(pFB_Addr->B0_Y+i*LCD_VERIFY_WIDTH), YCbCrColorLeft.Y, LCD_VERIFY_WIDTH >> 1);
            memset((void *)(pFB_Addr->B0_Y+i*LCD_VERIFY_WIDTH+(LCD_VERIFY_WIDTH>>1)), YCbCrColorRight.Y, LCD_VERIFY_WIDTH >> 1);
            if (i<(LCD_VERIFY_HEIGHT>>1))
            {
                memset((void *)(pFB_Addr->B0_CB+i*(LCD_VERIFY_WIDTH>>1)), YCbCrColorLeft.CB, LCD_VERIFY_WIDTH>>2);
                memset((void *)(pFB_Addr->B0_CB+i*(LCD_VERIFY_WIDTH>>1)+(LCD_VERIFY_WIDTH>>2)), YCbCrColorRight.CB, LCD_VERIFY_WIDTH>>2);
                memset((void *)(pFB_Addr->B0_CR+i*(LCD_VERIFY_WIDTH>>1)), YCbCrColorLeft.CR, LCD_VERIFY_WIDTH>>2);
                memset((void *)(pFB_Addr->B0_CR+i*(LCD_VERIFY_WIDTH>>1)+(LCD_VERIFY_WIDTH>>2)), YCbCrColorRight.CR, LCD_VERIFY_WIDTH>>2);
            }
        }

        ide_set_video2_changebuf(0);
    }
    else
    {
        for (i=0; i<LCD_VERIFY_HEIGHT; i++)
        {
            memset((void *)(pFB_Addr->B1_Y+i*LCD_VERIFY_WIDTH), YCbCrColorLeft.Y, LCD_VERIFY_WIDTH >> 1);
            memset((void *)(pFB_Addr->B1_Y+i*LCD_VERIFY_WIDTH+(LCD_VERIFY_WIDTH>>1)), YCbCrColorRight.Y, LCD_VERIFY_WIDTH >> 1);
            if (i<(LCD_VERIFY_HEIGHT>>1))
            {
                memset((void *)(pFB_Addr->B1_CB+i*(LCD_VERIFY_WIDTH>>1)), YCbCrColorLeft.CB, LCD_VERIFY_WIDTH>>2);
                memset((void *)(pFB_Addr->B1_CB+i*(LCD_VERIFY_WIDTH>>1)+(LCD_VERIFY_WIDTH>>2)), YCbCrColorRight.CB, LCD_VERIFY_WIDTH>>2);
                memset((void *)(pFB_Addr->B1_CR+i*(LCD_VERIFY_WIDTH>>1)), YCbCrColorLeft.CR, LCD_VERIFY_WIDTH>>2);
                memset((void *)(pFB_Addr->B1_CR+i*(LCD_VERIFY_WIDTH>>1)+(LCD_VERIFY_WIDTH>>2)), YCbCrColorRight.CR, LCD_VERIFY_WIDTH>>2);
            }
        }

        ide_set_video2_changebuf(1);
    }

    (*puiBufNum) = !(*puiBufNum);

    ide_set_load();
#endif
//#NT#2007/05/04#Steven Wang -end

}

/**
  Verify LCD by displaying different color

  Verify LCD by displaying different color

  @param void
  @return void
*/
void Cal_LCDVerify(void)
{
    FLGPTN              uiFlag;
    UINT32              uiWindowEn, uiBufNum, uiDispIndex;
    VIDEO_BUF_ADDR      FB_Addr;
    VIDEO_BUF_ATTR      FB_Attr;
    VOSD_WINDOW_ATTR    FB_Win;
    RGB_COLOR           RGBColor;

     // Get current enabled window
    uiWindowEn = ide_get_window_en();

    // Set Video2 parameter
    FB_Addr.B0_Y            = 0;//Cal_GetMemoryAddr();
    FB_Addr.B0_CB           = FB_Addr.B0_Y + LCD_VERIFY_SIZE;
    FB_Addr.B0_CR           = FB_Addr.B0_CB + (LCD_VERIFY_SIZE >> 2);
    FB_Addr.B1_Y            = FB_Addr.B0_CR + (LCD_VERIFY_SIZE >> 2);
    FB_Addr.B1_CB           = FB_Addr.B1_Y + LCD_VERIFY_SIZE;
    FB_Addr.B1_CR           = FB_Addr.B1_CB + (LCD_VERIFY_SIZE >> 2);

    FB_Attr.Buf_W           = LCD_VERIFY_WIDTH;
    FB_Attr.Buf_H           = LCD_VERIFY_HEIGHT;
    FB_Attr.Buf_Lineoffset  = FB_Attr.Buf_W >> 2;
    FB_Attr.Buf_L2R         = 0;        // Left to right
    FB_Attr.Buf_T2B         = 0;        // Top to buttom
    FB_Attr.Buf_BJmode      = 2;        // Buffer repeat
    FB_Attr.Buf_Opt         = 0;        // Buffer 0 in operation
    FB_Attr.Buf_Num         = 1;        // Two buffer

    FB_Win.Source_W         = LCD_VERIFY_WIDTH;
    FB_Win.Source_H         = LCD_VERIFY_HEIGHT;
    FB_Win.Des_W            = g_LCDSize.uiWinWidth;
    FB_Win.Des_H            = g_LCDSize.uiWinHeight;
    FB_Win.Win_Format       = 6;        // 420
    FB_Win.Win_X            = 0;
    FB_Win.Win_Y            = 0;
    FB_Win.High_Addr        = 0;

    ide_set_video_buf_addr(2, &FB_Addr);
    ide_set_video_buf_attr(2, &FB_Attr);
    ide_set_video_win_attr(2, &FB_Win);

    // Disable Video1, OSD1, OSD2. Enable Video2
    ide_disable_osd(IDE_OSDID_1);
    ide_disable_video(IDE_VIDEOID_1);
    ide_enable_video(IDE_VIDEOID_2);

    // Display white image
    RGBColor.R  = 255;
    RGBColor.G  = 255;
    RGBColor.B  = 255;
    uiBufNum    = 0;
    Cal_LCDDisplayFull(&RGBColor, &FB_Addr, &uiBufNum);
    uiDispIndex = 1;

    while (1)
    {
        wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_ENTER | FLGKEY_MENU, TWF_ORW | TWF_CLR);

        if (uiFlag & FLGKEY_ENTER)
        {
            switch (uiDispIndex)
            {
            case LCD_VERIFY_WHITE:
            default:
                RGBColor.R  = 255;
                RGBColor.G  = 255;
                RGBColor.B  = 255;
                Cal_LCDDisplayFull(&RGBColor, &FB_Addr, &uiBufNum);
                break;

            case LCD_VERIFY_RED:
                RGBColor.R  = 255;
                RGBColor.G  = 0;
                RGBColor.B  = 0;
                Cal_LCDDisplayFull(&RGBColor, &FB_Addr, &uiBufNum);
                break;

            case LCD_VERIFY_GREEN:
                RGBColor.R  = 0;
                RGBColor.G  = 255;
                RGBColor.B  = 0;
                Cal_LCDDisplayFull(&RGBColor, &FB_Addr, &uiBufNum);
                break;

            case LCD_VERIFY_BLUE:
                RGBColor.R  = 0;
                RGBColor.G  = 0;
                RGBColor.B  = 255;
                Cal_LCDDisplayFull(&RGBColor, &FB_Addr, &uiBufNum);
                break;

            case LCD_VERIFY_BLACK:
                RGBColor.R  = 0;
                RGBColor.G  = 0;
                RGBColor.B  = 0;
                Cal_LCDDisplayFull(&RGBColor, &FB_Addr, &uiBufNum);
                break;

            case LCD_VERIFY_BW:
                Cal_LCDDisplayHalf(&FB_Addr, &uiBufNum, TRUE);
                break;

            case LCD_VERIFY_WB:
                Cal_LCDDisplayHalf(&FB_Addr, &uiBufNum, FALSE);
                break;
            }

            uiDispIndex++;
            uiDispIndex %= LCD_VERIFY_MAX;
        }
        else if (uiFlag & FLGKEY_MENU)
        {
            if (uiWindowEn & DISPLAY_OSD1_EN)
            {
                ide_enable_osd(IDE_OSDID_1);
            }
            //#NT#2007/05/04#Steven Wang -begin
            //#200 only 1 buffer
            /*
            if (uiWindowEn & DISPLAY_OSD2_EN)
            {
                ide_set_osd2_en(TRUE);
            }
            */
            //#NT#2007/05/04#Steven Wang -end
            if (uiWindowEn & DISPLAY_VIDEO1_EN)
            {
                ide_enable_video(IDE_VIDEOID_1);
            }
            ide_disable_video(IDE_VIDEOID_2);

            break;
        }
    }
}

#endif

//@}
