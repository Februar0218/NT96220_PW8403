
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "NVTSysCommand.h"
#include "WavStudioTsk.h"
#include "MediaPlayAPI.h"
#include "GPIOMapping.h"
#include "arc_autoredeye.h"

#define MEDIAPLAY_FORWARD           0
#define MEDIAPLAY_BACKWARD          1

static PLAY_SINGLE_OBJ      FlowPlaySingleObj;
static PLAY_BROWSER_OBJ     FlowPBBrowserObj;
static UINT32 g_uiStatus;        // return value of PB task
static UINT32 g_uiSliderTime;
static UINT32 *pZoomLevelTbl;
static MEDIAPLAY_OBJ g_MediaPlayObj;

PRECT   g_pFlowPBThumbRect;
RECT    g_FlowPBThumbRect2[FLOWPB_THUMB_H_NUM2*FLOWPB_THUMB_V_NUM2] =

// For display size 320x240, 3x3 thumbnail mode
#if (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_3)
{
    {  8, 12, 96, 64}, {112, 12, 96, 64}, {216, 12, 96, 64},
    {  8, 88, 96, 64}, {112, 88, 96, 64}, {216, 88, 96, 64},
    {  8,164, 96, 64}, {112,164, 96, 64}, {216,164, 96, 64},
};
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_2)
// For display size 320x240, 3x2 thumbnail mode
{
    {  8, 38, 96, 64}, {112, 38, 96, 64}, {216, 38, 96, 64},
    {  8,138, 96, 64}, {112,138, 96, 64}, {216,138, 96, 64},
};
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_5_5)
// For display size 320x240, 5x5 thumbnail mode
{
    {  8,  8, 56, 40}, {70,  8, 56, 40}, {132,  8, 56, 40}, {194,  8, 56, 40}, {256,  8, 56, 40},
    {  8, 54, 56, 40}, {70, 54, 56, 40}, {132, 54, 56, 40}, {194, 54, 56, 40}, {256, 54, 56, 40},
    {  8,100, 56, 40}, {70,100, 56, 40}, {132,100, 56, 40}, {194,100, 56, 40}, {256,100, 56, 40},
    {  8,146, 56, 40}, {70,146, 56, 40}, {132,146, 56, 40}, {194,146, 56, 40}, {256,146, 56, 40},
    {  8,192, 56, 40}, {70,192, 56, 40}, {132,192, 56, 40}, {194,192, 56, 40}, {256,192, 56, 40},
};
#endif

static ER FlowPB_VideoFileRecovery(void)
{
 char pString[50];
 char pcurPathName[6];
 UINT32 did, fid;
 HNVT_FILE *pFile;
 PLAY_SINGLE_OBJ playObj;
 ER Ret = E_OK;

    did = PB_ReadCurrFileSysInfo(PLAYFILEINFO_DIRID);
    fid = PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILEID);
    FilesysGetDCFFileName(pString, did, fid, FST_FMT_AVI);

    // Recovery/close currnet video file
    Ret = MediaPlay_FileRecovery(pString, PB_GetCurrDataBufAddr(), PB_GetCurrDataBufSize());
    return Ret;
}

//#NT#2009/6/29#Philex Lin - begin
// Add single playback function call
void FlowPB_SinglePlayFunc(UINT32 command)
{
    switch (command)
    {
    case FLGKEY_UP:
        FlowPlaySingleObj.PlayCommand = PB_SINGLE_PREV | PB_SINGLE_PRIMARY;
        FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;
        break;

    case FLGKEY_DOWN:
        FlowPlaySingleObj.PlayCommand = PB_SINGLE_NEXT | PB_SINGLE_PRIMARY;
        FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;
        break;

    case FLGKEY_LEFT:
        FlowPlaySingleObj.PlayCommand = PB_SINGLE_PREV | PB_SINGLE_PRIMARY;
        FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;
        break;

    case FLGKEY_LEFT | FLGKEY_HOLD:
        FlowPlaySingleObj.PlayCommand = PB_SINGLE_PREV | PB_SINGLE_PRIMARY;
        FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;
        break;

    case FLGKEY_RIGHT:
        FlowPlaySingleObj.PlayCommand = PB_SINGLE_NEXT | PB_SINGLE_PRIMARY;
        FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;
        break;

    case FLGKEY_RIGHT | FLGKEY_HOLD:
        FlowPlaySingleObj.PlayCommand = PB_SINGLE_NEXT | PB_SINGLE_PRIMARY;
        FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;
        break;

    default: // FLGKEY_ENTER
        FlowPlaySingleObj.PlayCommand = PB_SINGLE_CURR | PB_SINGLE_PRIMARY;
        FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;//SLIDE_EFFECT_IN2OUT;
        break;
    }

    PB_PlaySingleMode(&FlowPlaySingleObj);
    g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    if(UIPlay_GetData(PLAY_FILENUM) > 0 && g_uiStatus != PB_STA_DONE)
    {
        if (FlowPB_VideoFileRecovery() == E_OK)
        {
          // re-play single again
          FlowPlaySingleObj.PlayCommand = PB_SINGLE_CURR | PB_SINGLE_PRIMARY;
          FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;//SLIDE_EFFECT_IN2OUT;
          PB_PlaySingleMode(&FlowPlaySingleObj);
          g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
        } else {
          PlayStamp_DecodeErrorMsg();
        }
    } else {
        if (g_uiStatus == PB_STA_ERR_DECODE)
          PlayStamp_DecodeErrorMsg();
    }

}
//#NT#2009/6/29#Philex Lin - end

static void FlowPB_ShowThumbBG(void)
{
    RECT        OutRect;
    PPLAY_OBJ   pPlayObj;

    pPlayObj = PB_GetPBObject();
    OutRect.uiLeft = 0;
    OutRect.uiTop = 0;
    OutRect.uiWidth = pPlayObj->uiDeviceWidth;
    OutRect.uiHeight = pPlayObj->uiDeviceHeight;
#if (BG_THUMBNAIL)
    PB_PlayUserJPEG(BGDB_GetThumbnailBGAddr(), BGDB_GetThumbnailBGSize(), OutRect, PLAY_USER_JPEG_IN_TMP_FRAME_BUFFER);
#endif
}

static BOOL FlowPB_ThumbConfig(THUMBNAIL_CONFIG *pThumbCfg)
{
    UINT32  i;
    UINT32  uiDispW, uiDispH;
    UINT16  wThumbWTotal, wThumbHTotal;
    UINT16  wThumbW, wThumbH;
    PPLAY_OBJ   pPlayObj;

    pPlayObj = PB_GetPBObject();
    uiDispW = pPlayObj->uiDisplayFBWidth;
    uiDispH = pPlayObj->uiDisplayFBHeight;
    //debug_err(("Display w = %d, h = %d\r\n", uiDispW, uiDispH));

    // support maximum 5 x 5 thumbnails
    if (pThumbCfg->wThumbPerLine > 5 || pThumbCfg->wThumbPerColumn > 5)
        return (FALSE);

    // at least we need 4 pixels for thumbnail frame
    if (pThumbCfg->wFirstHGap < 4)  pThumbCfg->wFirstHGap = 4;
    if (pThumbCfg->wHGap < 4)       pThumbCfg->wHGap = 4;
    if (pThumbCfg->wLastHGap < 4)   pThumbCfg->wLastHGap = 4;
    if (pThumbCfg->wFirstVGap < 4)  pThumbCfg->wFirstVGap = 4;
    if (pThumbCfg->wVGap < 4)       pThumbCfg->wVGap = 4;
    if (pThumbCfg->wLastVGap < 4)   pThumbCfg->wLastVGap = 4;

    wThumbWTotal = pThumbCfg->wFirstHGap + pThumbCfg->wLastHGap +
        pThumbCfg->wHGap * (pThumbCfg->wThumbPerLine - 1);

    if (wThumbWTotal >= uiDispW)
        return (FALSE);

    wThumbHTotal = pThumbCfg->wFirstVGap + pThumbCfg->wLastVGap +
        pThumbCfg->wVGap * (pThumbCfg->wThumbPerColumn - 1);

    if (wThumbHTotal >= uiDispH)
        return (FALSE);

    wThumbW = (uiDispW - wThumbWTotal) / pThumbCfg->wThumbPerLine;
    wThumbH = (uiDispH - wThumbHTotal) / pThumbCfg->wThumbPerColumn;

    for (i = 0; i < pThumbCfg->wThumbPerLine * pThumbCfg->wThumbPerColumn; i++)
    {
        g_FlowPBThumbRect2[i].uiLeft = pThumbCfg->wFirstHGap +
            (i % pThumbCfg->wThumbPerLine) * (wThumbW + pThumbCfg->wHGap);
        g_FlowPBThumbRect2[i].uiTop = pThumbCfg->wFirstVGap +
            (i / pThumbCfg->wThumbPerLine) * (wThumbH + pThumbCfg->wVGap);
        g_FlowPBThumbRect2[i].uiWidth = wThumbW;
        g_FlowPBThumbRect2[i].uiHeight = wThumbH;
    }

    FlowPBBrowserObj.HorNums = pThumbCfg->wThumbPerLine;
    FlowPBBrowserObj.VerNums = pThumbCfg->wThumbPerColumn;
    //uiThumbFrameColor = pThumbCfg->wFrColor;

    return (TRUE);
}

void FlowPB_ThumbDisplay(UINT32 mode)
{
    //UINT32  uiCurrFileIndex;
    THUMBNAIL_CONFIG ThumbCfg;

    if (mode == ON)
    {
        if (KeyScan_IsTVPlugIn())
        {
            //#For LCD panel with 16:9 ratio (but 4:3 pixel resolution)
            #if 1//(DISPLAY_169HD == DISPLAY_169HD_YES)
            if (SysGetFlag(FL_TV_MODE) == TV_MODE_NTSC)
            {
                ThumbCfg.wFirstHGap         = 22*(640)/(320);
                ThumbCfg.wHGap              = 22*(640)/(320);
                ThumbCfg.wLastHGap          = 24*(640)/(320);
                ThumbCfg.wFirstVGap         = 6*(448)/(240);
                ThumbCfg.wVGap              = 6*(448)/(240);
                ThumbCfg.wLastVGap          = 30*(448)/(240);
            }
            else
            {
                ThumbCfg.wFirstHGap         = 22*(640)/(320);
                ThumbCfg.wHGap              = 22*(640)/(320);
                ThumbCfg.wLastHGap          = 22*(640)/(320);
                ThumbCfg.wFirstVGap         = 7*(528)/(240);
                ThumbCfg.wVGap              = 6*(528)/(240);
                ThumbCfg.wLastVGap          = 30*(528)/(240);
            }
            #else
            if (SysGetFlag(FL_TV_MODE) == TV_MODE_NTSC)
            {
                ThumbCfg.wFirstHGap         = 16*(640)/(320);
                ThumbCfg.wHGap              = 16*(640)/(320);
                ThumbCfg.wLastHGap          = 32*(640)/(320);
                ThumbCfg.wFirstVGap         = 6*(448)/(240);
                ThumbCfg.wVGap              = 6*(448)/(240);
                ThumbCfg.wLastVGap          = 30*(448)/(240);
            }
            else
            {
                ThumbCfg.wFirstHGap         = 16*(640)/(320);
                ThumbCfg.wHGap              = 16*(640)/(320);
                ThumbCfg.wLastHGap          = 32*(640)/(320);
                ThumbCfg.wFirstVGap         = 6*(528)/(240);
                ThumbCfg.wVGap              = 6*(528)/(240);
                ThumbCfg.wLastVGap          = 30*(528)/(240);
            }
            #endif
        }
        else
        {
            if(KeyScan_IsHDMIPlugIn() == TRUE)
            {//HDMI
                ThumbCfg.wFirstHGap         = 20*(1280)/(320);
                ThumbCfg.wHGap              = 22*(1280)/(320);
                ThumbCfg.wLastHGap          = 20*(1280)/(320);
                ThumbCfg.wFirstVGap         = 6*(720)/(240);
                ThumbCfg.wVGap              = 6*(720)/(240);
                ThumbCfg.wLastVGap          = 30*(720)/(240);
            }
            else
            {//LCD
                #if (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_3)
                ThumbCfg.wFirstHGap         = 18;//16;
                ThumbCfg.wHGap              = 17;//16;
                ThumbCfg.wLastHGap          = 18;//32;
                ThumbCfg.wFirstVGap         = 6;
                ThumbCfg.wVGap              = 6;
                ThumbCfg.wLastVGap          = 30;
                #elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_2)
                ThumbCfg.wFirstHGap         = 18;
                ThumbCfg.wHGap              = 16;
                ThumbCfg.wLastHGap          = 18;
                ThumbCfg.wFirstVGap         = 50;
                ThumbCfg.wVGap              = 12;
                ThumbCfg.wLastVGap          = 50;
                #elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_5_5)
                //Todo: fine tune 5x5 location
                ThumbCfg.wFirstHGap         = 18;
                ThumbCfg.wHGap              = 16;
                ThumbCfg.wLastHGap          = 18;
                ThumbCfg.wFirstVGap         = 6;
                ThumbCfg.wVGap              = 6;
                ThumbCfg.wLastVGap          = 30;
                #endif
            }
        }

        ThumbCfg.wThumbPerLine          = FLOWPB_THUMB_H_NUM2;
        ThumbCfg.wThumbPerColumn        = FLOWPB_THUMB_V_NUM2;
        //ThumbCfg.wFrColor               = SHOWOSD_COLOR_WHITE;//SHOWOSD_COLOR_RED;
        FlowPB_ThumbConfig(&ThumbCfg);

        FlowPB_ShowThumbBG();

        FlowPBBrowserObj.BrowserCommand = PB_BROWSER_CURR;
        FlowPBBrowserObj.MoveImgMode    = SLIDE_EFFECT_NONE;
        FlowPBBrowserObj.JumpOffset     = 0;
        PB_PlayBrowserMode(&FlowPBBrowserObj);
        g_pFlowPBThumbRect = &g_FlowPBThumbRect2[0];
        PB_SetThumbRect(g_pFlowPBThumbRect);
    }
    else
    {
        //FlowPB_ClearAllOSD();
        //FlowPB_ThumbPlayFunc(FLGKEY_ENTER);
        //gPlbData.State = PLB_ST_FULL;
    }
}

void FlowPB_ThumbPlayFunc(UINT32 key)
{
    UINT32  CurrFileIndex;
    UINT32  CurrFileSeqID, FileNumsInDir;
    UINT32  FileNumsInFinalPage, FileNumsInFinalLine;
    UINT16  *pThumbSeqID;
    UINT32  CurThumbNums, CurrPageID, FinalPageID;
    UINT32  NumsPerPage, CurrThumbRow, FinalThumbRow, NextFileIndex;

    FlowPBBrowserObj.bReDecodeImages = TRUE;

    CurrFileIndex = PB_GetCurFileIndex();
    pThumbSeqID = PB_GetAllThumbSeqIdArray();
    pThumbSeqID += (CurrFileIndex-1);
    CurrFileSeqID = *pThumbSeqID;
    NumsPerPage = FlowPBBrowserObj.HorNums * FlowPBBrowserObj.VerNums;

    //FileNumsInDir = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILENUMS_INDIR);
    FileNumsInDir = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILENUMS);

    if(FileNumsInDir == 0)
        return;

    if((FileNumsInDir % NumsPerPage) == 0)
        FileNumsInFinalPage = NumsPerPage;
    else
        FileNumsInFinalPage = FileNumsInDir % NumsPerPage;

    if((FileNumsInFinalPage % FlowPBBrowserObj.HorNums) == 0)
        FileNumsInFinalLine = FlowPBBrowserObj.HorNums;
    else
        FileNumsInFinalLine = FileNumsInFinalPage % FlowPBBrowserObj.HorNums;

    CurrPageID  = ((CurrFileSeqID-1) / NumsPerPage);
    FinalPageID = ((FileNumsInDir-1) / NumsPerPage);

    CurThumbNums = PB_GetCurThumbNums();

    CurrThumbRow  = (CurrFileIndex-1)/FlowPBBrowserObj.HorNums;
    FinalThumbRow = (CurThumbNums-1)/FlowPBBrowserObj.HorNums;

    // page-direction (LEFT)
    if(key == NVTEVT_KEY_LEFT)
    {
        FlowPBBrowserObj.BrowserCommand = PB_BROWSER_PREV;
        FlowPBBrowserObj.JumpOffset     = 1;
        if(FileNumsInDir <= NumsPerPage)
        {   // means only one page
            FlowPBBrowserObj.bReDecodeImages = FALSE;
        }
        else
        {
            //FlowPBBrowserObj.MoveImgMode  = SLIDE_EFFECT_PUSHDOWN2UP;
            FlowPBBrowserObj.MoveImgMode    = SLIDE_EFFECT_NONE;

            if(CurrFileIndex > 1)
                FlowPBBrowserObj.bReDecodeImages = FALSE;
        }
    }
    // page-direction (RIGHT)
    else if(key == NVTEVT_KEY_RIGHT)
    {
        FlowPBBrowserObj.BrowserCommand = PB_BROWSER_NEXT;
        FlowPBBrowserObj.JumpOffset     = 1;
        if(FileNumsInDir <= NumsPerPage)
        {   // means only one page
            FlowPBBrowserObj.bReDecodeImages = FALSE;
        }
        else
        {
            //FlowPBBrowserObj.MoveImgMode  = SLIDE_EFFECT_PUSHUP2DOWN;
            FlowPBBrowserObj.MoveImgMode    = SLIDE_EFFECT_NONE;

            if(CurrFileIndex < CurThumbNums)
                FlowPBBrowserObj.bReDecodeImages = FALSE;
        }
    }
    // page-direction (UP)
    else if(key == NVTEVT_KEY_UP)
    {
        FlowPBBrowserObj.BrowserCommand = PB_BROWSER_PREV;
        //FlowPBBrowserObj.MoveImgMode  = SLIDE_EFFECT_PUSHDOWN2UP;
        FlowPBBrowserObj.MoveImgMode    = SLIDE_EFFECT_NONE;
        //FlowPBBrowserObj.JumpOffset   = FlowPBBrowserObj.HorNums;
        if(CurrPageID > 0)
        {   // means now is not 1st-page, decode the same file-index-image
            FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums;
        }
        else // if(CurrPageID == 0)
        {   // means now is 1st-page
            if(CurrThumbRow > 0)
            {   // means CurrThumbRow=1,2, do not change page, decode the same file-index-image
                FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums;
                FlowPBBrowserObj.bReDecodeImages = FALSE;
            }
            else if(FileNumsInDir <= NumsPerPage)
            {   // means only one page & CurrThumbRow = 0,

                if(FinalThumbRow == 0)
                {   // means only one row, do nothing
                    FlowPBBrowserObj.JumpOffset = 0;
                }
                else if(FileNumsInFinalLine >= CurrFileIndex)
                {   // must decode the same file-index-image
                    FlowPBBrowserObj.JumpOffset = FileNumsInFinalLine;
                }
                else
                {   // must decode the same file-index-image
                    FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums + FileNumsInFinalLine;
                }
                FlowPBBrowserObj.bReDecodeImages = FALSE;
            }
            else
            {   // means CurrThumbRow=0, go to final page
                if(FileNumsInFinalPage < FlowPBBrowserObj.HorNums)
                {   // means final-page file nums < 3 (only one line),
                    if(FileNumsInFinalLine >= CurrFileIndex)
                    {   // must decode the same file-index-image
                        FlowPBBrowserObj.JumpOffset = FileNumsInFinalLine;
                    }
                    else
                    {   // must decode final file-index-image
                        FlowPBBrowserObj.JumpOffset = CurrFileIndex;
                    }
                }
                else
                {   // means final-page file nums >= 3
                    if(FileNumsInFinalLine >= CurrFileIndex)
                    {   // must decode the same file-index-image
                        FlowPBBrowserObj.JumpOffset = FileNumsInFinalLine;
                    }
                    else
                    {   // must decode the same file-index-image
                        FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums + FileNumsInFinalLine;
                    }
                }
            }
        }   // else if(CurrPageID == 0)
    }
    // page-direction (DOWN)
    else //if(key == NVTEVT_KEY_DOWN)
    {
        FlowPBBrowserObj.BrowserCommand = PB_BROWSER_NEXT;
        //FlowPBBrowserObj.MoveImgMode  = SLIDE_EFFECT_PUSHUP2DOWN;
        FlowPBBrowserObj.MoveImgMode    = SLIDE_EFFECT_NONE;
        //FlowPBBrowserObj.JumpOffset   = FlowPBBrowserObj.HorNums;
        if((CurrPageID+1) < FinalPageID)
        {   // means now is not final-page & (final-1)-page, decode the same file-index-image
            FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums;
        }
        else if((CurrPageID+1) == FinalPageID)
        {   // means now is (final-1)-page, go to final-page
            if(CurrThumbRow != FinalThumbRow)
            {   // means CurrThumbRow != MaxRow, do not change page, decode the same file-index-image
                FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums;
                FlowPBBrowserObj.bReDecodeImages = FALSE;
            }
            else if(FileNumsInFinalPage < FlowPBBrowserObj.HorNums)
            {   // means final-page file nums < 3 (only one line),
                if((CurrFileIndex % FlowPBBrowserObj.HorNums) == 0)
                {
                    NextFileIndex = FlowPBBrowserObj.HorNums;
                }
                else
                {
                    NextFileIndex = CurrFileIndex % FlowPBBrowserObj.HorNums;
                }

                if(FileNumsInFinalLine >= NextFileIndex)
                {   // must decode the same file-index-image
                    FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums;
                }
                else
                {   // must decode final file-index-image
                    FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums - (NextFileIndex - FileNumsInFinalLine);
                }
            }
            else
            {   // must decode the same file-index-image
                FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums;
            }
        }
        else // if(CurrPageID == FinalPageID)
        {   // means now is final-page
            if( (CurrFileIndex + FlowPBBrowserObj.HorNums) <= CurThumbNums)
            {   // means do not change page, decode the same file-index-image
                FlowPBBrowserObj.JumpOffset = FlowPBBrowserObj.HorNums;
                FlowPBBrowserObj.bReDecodeImages = FALSE;
            }
            else if(FileNumsInDir <= NumsPerPage)
            {   // means only one page & CurrThumbRow=end,
                if(FinalThumbRow == 0)
                {   // means only one row, do nothing
                    FlowPBBrowserObj.JumpOffset = 0;
                }
                else if(CurrThumbRow != FinalThumbRow)
                {
                    FlowPBBrowserObj.JumpOffset = (FlowPBBrowserObj.HorNums + FileNumsInFinalLine);
                }
                else
                {
                    FlowPBBrowserObj.JumpOffset = (FileNumsInFinalLine);
                }
                FlowPBBrowserObj.bReDecodeImages = FALSE;
            }
            else
            {   // means now is final-page, go to 1st-page
                // must decode the same file-index-image
                if(CurrThumbRow != FinalThumbRow)
                {
                    FlowPBBrowserObj.JumpOffset = (FlowPBBrowserObj.HorNums + FileNumsInFinalLine);
                }
                else
                {
                    FlowPBBrowserObj.JumpOffset = (FileNumsInFinalLine);
                }
            }
        }   // End of if(CurrPageID == FinalPageID)
    }

    if (FlowPBBrowserObj.bReDecodeImages)
        FlowPB_ShowThumbBG();

    // Set browser-command to playback
    if (PB_PlayBrowserMode(&FlowPBBrowserObj) != PB_STA_DONE)
    {
        Ux_Redraw();

        // change page, wait for playback done
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }

    #if 0
    pThumbSeqID = PB_GetAllThumbSeqIdArray();
    debug_err(("All Thumb Seq ID:\r\n"));
    debug_err(("%03d %03d %03d\r\n", *pThumbSeqID, *(pThumbSeqID + 1), *(pThumbSeqID + 2)));
    debug_err(("%03d %03d %03d\r\n", *(pThumbSeqID + 3), *(pThumbSeqID + 4), *(pThumbSeqID + 5)));
    debug_err(("%03d %03d %03d\r\n", *(pThumbSeqID + 6), *(pThumbSeqID + 7), *(pThumbSeqID + 8)));
    debug_err(("CurrFileSeqID = %d\r\n", *(pThumbSeqID + PB_GetCurFileIndex() - 1)));
    debug_err(("Total file number = %d\r\n\r\n", PB_GetCurrFileSysInfo(PLAYFILEINFO_FILENUMS)));
    #endif
}

static UINT32 g_uiPlayDir = MEDIAPLAY_FORWARD;

void FlowPB_SetPlayDir(UINT32 uiPlayDir)
{
    g_uiPlayDir = uiPlayDir;
}
UINT32 FlowPB_GetPlayDir(void)
{
    return g_uiPlayDir;
}

INT32 UIPlay_OnInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPLAY_MSG(("UIPlay_OnInit ++\r\n "));
    FlowPlaySingleObj.PlayCommand = PB_SINGLE_CURR | PB_SINGLE_PRIMARY;
    FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_IN2OUT;
    FlowPlaySingleObj.JumpOffset  = 1;
    g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    UIPLAY_MSG(("UIPlay_OnInit --\r\n "));

    if(UIPlay_GetData(PLAY_FILENUM) > 0 && g_uiStatus != PB_STA_DONE)
    {
        if (FlowPB_VideoFileRecovery() == E_OK)
        {
          // re-play single again
          FlowPlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;//SLIDE_EFFECT_IN2OUT;
          PB_PlaySingleMode(&FlowPlaySingleObj);
          g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
        } else {
          PlayStamp_DecodeErrorMsg();
        }
    } else {
        if (g_uiStatus == PB_STA_ERR_DECODE)
            PlayStamp_DecodeErrorMsg();
    }

    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnReInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PPLAY_OBJ    pPlayObj;

    /* re-open playback */
    PB_Close(PB_WAIT_INFINITE);

    pPlayObj = PB_GetPBObject();
    PB_Open(pPlayObj);

    return UIPlay_OnInit(pCtrl, paramNum, paramArray);
}
static  char   szFilename[32];

INT32 UIPlay_OnNext(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPLAY_MSG(("UIPlay_OnNext\r\n"));
    FlowPB_SinglePlayFunc(FLGKEY_RIGHT);
#if (_GPS_RECEIVER_LOG_ == ENABLE)
    if (FilesysGetDCFIdFormat(PB_ReadCurrFileSysInfo(PLAYFILEINFO_DIRID), PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILEID)) & FST_FMT_AVI)
    {
        FilesysGetDCFFileName(szFilename,PB_ReadCurrFileSysInfo(PLAYFILEINFO_DIRID),PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILEID),FST_FMT_AVI);
        debug_msg("%s,%s\r\n",__func__,szFilename);
    } else {
        debug_msg("No such file\r\n");
    }
#endif	
    return NVTEVT_CONSUME;
}
INT32 UIPlay_OnPrev(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPLAY_MSG(("UIPlay_OnPrev\r\n"));
    FlowPB_SinglePlayFunc(FLGKEY_LEFT);
#if (_GPS_RECEIVER_LOG_ == ENABLE)
    if (FilesysGetDCFIdFormat(PB_ReadCurrFileSysInfo(PLAYFILEINFO_DIRID), PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILEID)) & FST_FMT_AVI)
    {
        FilesysGetDCFFileName(szFilename,PB_ReadCurrFileSysInfo(PLAYFILEINFO_DIRID),PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILEID),FST_FMT_AVI);
        debug_msg("%s,%s\r\n",__func__,szFilename);
    } else {
        debug_msg("No such file\r\n");
    }
#endif	

    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnCurr(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPLAY_MSG(("UIPlay_OnCurr\r\n"));
    FlowPB_SinglePlayFunc(FLGKEY_ENTER);

    return NVTEVT_CONSUME;
}

static void UIPlay_DelGPSTxtFile(BOOL isDelAll)
{
  PFILE_TSK_DCF_PARAM pParam;
  char szFilename[32] = {0};
  UW   totalSeqID;
  UH did, fid;

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    pParam = FilesysGetFSTParam();

    if (isDelAll)
    { // delete all GPS txt file
        totalSeqID = pParam->TolIDSequ;
        while (totalSeqID)
        {
            FilesysGetDCFIDBySeq(totalSeqID--, &did, &fid);
            FilesysGetDCFFileName(szFilename,did,fid,FST_FMT_AVI);
            strcpy((szFilename+strlen(szFilename)-4),"\0");
            FilesysWaitCmdFinish(FST_TIME_INFINITE);
            FilesysDeleteByName(strcat(szFilename,".TXT"),FALSE);
        }
    } else { // delete one
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        if (FilesysGetDCFFileName(szFilename,pParam->DirectoryID,pParam->FilenameID,FST_FMT_AVI))
        {
            strcpy((szFilename+strlen(szFilename)-4),"\0");
            FilesysWaitCmdFinish(FST_TIME_INFINITE);
            FilesysDeleteByName(strcat(szFilename,".TXT"),FALSE);
        }
    }
}

INT32 UIPlay_OnRotate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PJPGHEAD_DEC_CFG    pJPGInfo;
    UINT32 uiIndex = 0;

    pJPGInfo = PB_GetCurrImageInfo();
    if(paramNum == 1)
        uiIndex = paramArray[0];
    switch (pJPGInfo->pExifInfo->Orientation)
    {
    case PLAY_EXIF_ORI_ROTATE_90:
        uiIndex += 1;
        break;
    case PLAY_EXIF_ORI_ROTATE_180:
        uiIndex += 2;
        break;
    case PLAY_EXIF_ORI_ROTATE_270:
        uiIndex += 3;
        break;
    }

    debug_err(("UIPlay_OnRotate,uiIndex=%d\r\n",uiIndex));
    switch (uiIndex % 4)
    {
    case 0:
        PB_SetEXIF_Orientation(PLAY_EXIF_ORI_DEFAULT, FALSE);      // rotate & save
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        break;

    case 1:
        PB_SetEXIF_Orientation(PLAY_EXIF_ORI_ROTATE_90, FALSE);    // rotate & save
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        break;

    case 2:
        PB_SetEXIF_Orientation(PLAY_EXIF_ORI_ROTATE_180, FALSE);   // rotate & save
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        break;

    default:
        PB_SetEXIF_Orientation(PLAY_EXIF_ORI_ROTATE_270, FALSE);   // rotate & save
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        break;
    }
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnExeDel(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    NVTRET  uiRet = NVTRET_CLOSE_PARENT_WND;
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    if (GetCardStatus() == CARD_LOCKED)
        return uiRet;

    UIPLAY_MSG(("UIPlay_OnExeDel: selected = %d\r\n",uhSelect));

    switch(uhSelect)
    {
    case 0:
        #if (_GPS_RECEIVER_LOG_ == ENABLE)
        // delete one GPS txt file
        UIPlay_DelGPSTxtFile(FALSE);
        #endif		
        PB_PlayDeleteMode(PB_DELETE_ONE);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        uiRet = NVTRET_DELETE;
        break;
    case 1:
        #if (_GPS_RECEIVER_LOG_ == ENABLE)
        // delete all GPS txt files
        UIPlay_DelGPSTxtFile(TRUE);
        #endif		
        Ux_BackgroundPostEvent(NVTEVT_BKW_DELALL);
        //CreateWindow((VObject *)(&UIFlowInfoWaitMoment), 0);
        uiRet = NVTRET_WAIT_SCREEN;
        break;
    case 2:
        #if (_GPS_RECEIVER_LOG_ == ENABLE)
        // delete one GPS txt file
        UIPlay_DelGPSTxtFile(TRUE);
        #endif		
        PB_PlayDeleteMode(PB_DELETE_ALL);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        uiRet = NVTRET_DELETE;
        break;
    default:
        UIPLAY_ERR(("UIPlay_OnExeDel: uhSelect error!!\r\n"));
        break;
    }
    return uiRet;
}

INT32 UIPlay_OnExeCopy2Card(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (GetCardStatus() == CARD_LOCKED)
        return NVTRET_CLOSE_PARENT_WND;

    Ux_BackgroundPostEvent(NVTEVT_BKW_COPY2CARD);
    //CreateWindow((VObject *)(&UIFlowInfoWaitMoment), 0);
    return NVTRET_WAIT_SCREEN;
}

INT32 UIPlay_OnExeSlide(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    SysSetFlag(FL_SLIDE_SHOW, uiSelect);
    return NVTRET_SLIDE_SHOW;
}

INT32 UIPlay_OnExeDPOF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 PrtNum = 0;
    UINT32 DateOn = 0;
    UINT32 DPOFAll = 0;
    if (GetCardStatus() == CARD_LOCKED)
        return NVTRET_CLOSE_PARENT_WND;

    if(paramNum>0)
    {
        DPOFAll = paramArray[0];
        PrtNum= paramArray[1];
        DateOn= paramArray[2];
    }

    UIPLAY_MSG(("UIPlay_OnExeDPOF: DPOFAll=%d, PrtNum=%d, DateOn=%d\r\n",DPOFAll,PrtNum,DateOn));

    if(DPOFAll)
        PB_SetCurrDPOFQty(PLAYDPOF_SETALL, PrtNum, DateOn);
    else
        PB_SetCurrDPOFQty(PLAYDPOF_SETONE, PrtNum, DateOn);

    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTRET_CLOSE_PARENT_WND;
}

INT32 UIPlay_OnExeProtect(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    NVTRET uiRet = NVTRET_CLOSE_PARENT_WND;
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    if (GetCardStatus() == CARD_LOCKED)
        return NVTRET_CLOSE_PARENT_WND;

    UIPLAY_MSG(("UIPlay_OnExeProtect: uhSelect= %d\r\n",uhSelect));

    switch(uhSelect)
    {
    case PROTECT_ONE:
        PB_SetProtectFile(PLAY_PROTECT_ONE);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        uiRet = NVTRET_PROTECT;
        break;
    case UNPROTECT_ONE:
        PB_SetProtectFile(PLAY_UNPROTECT_ONE);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        uiRet = NVTRET_PROTECT;
        break;
    case PROTECT_ALL:
        PB_SetProtectFile(PLAY_PROTECT_ALL);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        Ux_BackgroundPostEvent(NVTEVT_BKW_PROTECTALL);
        uiRet = NVTRET_WAIT_SCREEN;
        break;
    case UNPROTECT_ALL:
        PB_SetProtectFile(PLAY_UNPROTECT_ALL);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
        Ux_BackgroundPostEvent(NVTEVT_BKW_UNPROTECTALL);
        uiRet = NVTRET_WAIT_SCREEN;
        break;
    default:
        UIPLAY_ERR(("UIPlay_OnExeProtect: uhSelect error!!\r\n"));
        break;
    }

    return uiRet;
}

INT32 UIPlay_OnExeThumbInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    FlowPB_ThumbDisplay(ON);
    //#NT#2009/10/21#Hideo Lin -begin
    //#NT#Remove the redundant code; it will execute playback browser twice (since we have called it in FlowPB_ThumbDisplay())
    //PB_PlayBrowserMode(&FlowPBBrowserObj);
    //#NT#2009/10/21#Hideo Lin -end
    g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return NVTRET_THUMBNAIL;
}

INT32 UIPlay_OnExeThumb(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum > 0)
    {
        FlowPB_ThumbPlayFunc(paramArray[0]);
    }
    return NVTRET_THUMBNAIL;
}

INT32 UIPlay_OnZoomInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_SetZoomLevelTbl(pZoomLevelTbl);
    PB_PlayZoomMode(PLAYZOOM_IN);
    g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnZoomIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_PlayZoomMode(PLAYZOOM_IN);
    g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnZoomOut(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PB_PlayZoomMode(PLAYZOOM_OUT);
    g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnZoomNavi(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum >= 2)
    {
        PB_PlayZoomPanMode(paramArray[0], paramArray[1]);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    else
    {
        UIPLAY_ERR(("E: UIPlay_OnZoomNavi paramNum error!!\r\n"));
    }
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnSlideInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPLAY_MSG(("UISlide_OnInit g_uiSliderTime=%d\r\n ",g_uiSliderTime));

    if(paramNum > 0)
    {
        PB_PlaySliderModeInit(PB_SINGLE_SLIDE_START|PB_SINGLE_PRIMARY, g_uiSliderTime, (SliderCb)(paramArray[0]));
    }
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnSlideKeep(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPLAY_MSG(("UISlide_OnKeep\r\n"));
    PB_PlaySliderMode(PB_SINGLE_SLIDE_START|PB_SINGLE_PRIMARY, SLIDE_EFFECT_NONE);
    g_uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OnSlideStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPLAY_MSG(("UISlide_OnStop\r\n"));
    PB_PlaySliderMode(PB_SINGLE_SLIDE_STOP, SLIDE_EFFECT_NONE);
    return NVTEVT_CONSUME;
}

INT32 UIPlay_OpenSpecific(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (paramNum != 2) {
        return NVTEVT_CONSUME;
    } else {
        PB_OpenSpecFile(paramArray[0],paramArray[1]);
        PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }

    return NVTEVT_CONSUME;
}

INT32 UIPlay_MovStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PLAY_OBJ    PlayObj;

    MediaPlay_Stop();
    MediaPlay_Close();
    //PlayObj.ui1stPlayMode = PLAYMODE_MOVMJPG; //??
    PlayObj.ui1stPlayMode = PLAYMODE_AVI;
    PB_Open(&PlayObj);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    ////FlowPB_PlaySingle();

    // restore key tone setting
    if (SysGetFlag(FL_BEEP)==BEEP_ON)
       KeyScan_EnableKeyTone(TRUE);

    return NVTEVT_CONSUME;
}

INT32 UIPlay_MovPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PJPGHEAD_DEC_CFG pJpgInfo;
    UIPLAY_MSG(("AVI play start..\r\n"));

    //if(paramNum > 0)
    {
        UINT32          y1, y2, cb1, cb2, cr1, cr2;
        PPLAY_OBJ       pPlayObj;
        VIDEO_DECODER   *pDecobj;
        ER              openMedia;
        UINT32          uiPoolAddr;

        debug_err(("FlowPB: Play AVI\r\n"));
        FlowPB_SetPlayDir(MEDIAPLAY_FORWARD);

        MediaPlay_Close();
        PB_Close(PB_WAIT_INFINITE);

        pPlayObj = PB_GetPBObject();
        pJpgInfo = PB_GetCurrImageInfoBeforeDec(FALSE);

        get_blk((VP*)&(uiPoolAddr), POOL_ID_FB);
        rel_blk(POOL_ID_FB, (VP)uiPoolAddr);

        g_MediaPlayObj.uiMemAddr = uiPoolAddr;
        //#NT#2011/10/13#KS Hung -begin
        if (GPIOMap_DetStrgCard() == TRUE)
        {
            g_MediaPlayObj.uiMemSize = END_MEM - uiPoolAddr - POOL_SIZE_WAV - SysGetDispRotateBuffSize();
        }
        else
        {
            g_MediaPlayObj.uiMemSize = END_MEM - uiPoolAddr - POOL_SIZE_RAM_DISK - POOL_SIZE_WAV - SysGetDispRotateBuffSize();
        }
        //#NT#2011/10/13#KS Hung -end
        g_MediaPlayObj.u16OutWidth = pPlayObj->uiDisplayFBWidth;
        g_MediaPlayObj.u16OutHeight= pPlayObj->uiDisplayFBHeight;
        g_MediaPlayObj.u32PanelW = pPlayObj->uiDeviceWidth;//2011/06/07 Meg Lin
        g_MediaPlayObj.u32PanelH = pPlayObj->uiDeviceHeight;//2011/06/07 Meg Lin
        g_MediaPlayObj.u32MaxWidth4Panel = MEDIAPLAY_DEF_MAXWIDTH4PANEL;//2011/08/08 Meg Lin

        if(KeyScan_IsTVPlugIn() == TRUE)
        {
            g_MediaPlayObj.u16OutWratio= 4;//g_LCDSize.uiWinWidth;//PB_PANEL_NO_SCALING;
            g_MediaPlayObj.u16OutHratio= 3;//g_LCDSize.uiWinHeight;//PB_PANEL_NO_SCALING;
            g_MediaPlayObj.u32decodeMode = MEMORY_NORMAL;
        }
        else if(KeyScan_IsHDMIPlugIn() == TRUE)//2011/06/07 Meg Lin
        {
            //g_MediaPlayObj.u16OutWratio= g_LCDSize.uiWinWidth;//PB_PANEL_NO_SCALING;
            //g_MediaPlayObj.u16OutHratio= g_LCDSize.uiWinHeight;//PB_PANEL_NO_SCALING;
            g_MediaPlayObj.u16OutWratio= g_LCDSize.uiWinWidth;//PB_PANEL_NO_SCALING;
            g_MediaPlayObj.u16OutHratio= g_LCDSize.uiWinHeight;//PB_PANEL_NO_SCALING;
            if (pJpgInfo->imagewidth==1440)
                g_MediaPlayObj.u32decodeMode = MEMORY_NORMAL; //2011/08/04 KS Hung for 1280x720
            else
                g_MediaPlayObj.u32decodeMode = MEMORY_TINY; //2011/08/04 KS Hung for 1280x720
            debug_msg("hdmi in width=%d, hei=%d !!\r\n", g_MediaPlayObj.u16OutWratio, g_MediaPlayObj.u16OutHratio);
        }
        else
        {//LCD panel
            //#NT#2011/10/13#KS Hung -begin
            #if (DISPLAY_169HD == DISPLAY_169HD_YES)
            g_MediaPlayObj.u16OutWratio= g_LCDSize.uiWidth;
            g_MediaPlayObj.u16OutHratio= g_LCDSize.uiHeight;
            #else
            g_MediaPlayObj.u16OutWratio= 4;
            g_MediaPlayObj.u16OutHratio= 3;
            #endif
            //#NT#2011/10/13#KS Hung -end
            g_MediaPlayObj.u32decodeMode = MEMORY_NORMAL;
        }

        g_MediaPlayObj.CallBackFunc = (MovPlayCb)(paramArray[0]);
        g_MediaPlayObj.bHWAudio = TRUE;//FALSE;

        PB_GetCurr3FBAddr(&y1, &cb1, &cr1);
        // Get temp display frame buffer
        PB_GetCurr3TempFBAddr(&y2, &cb2, &cr2);
        g_MediaPlayObj.panelInfo.y1Addr  = y1;
        g_MediaPlayObj.panelInfo.cb1Addr = cb1;
        g_MediaPlayObj.panelInfo.cr1Addr = cr1;
        g_MediaPlayObj.panelInfo.y2Addr  = y2;
        g_MediaPlayObj.panelInfo.cb2Addr = cb2;
        g_MediaPlayObj.panelInfo.cr2Addr = cr2;

        //if (play1stParam.videoCodec == MEDIAPLAY_VIDEO_MJPG)
        {
            pDecobj = MP_MjpgDec_getVideoDecodeObject();
            MediaPlay_SetVideoDecObj(pDecobj);
            debug_err(("videoCodec = MEDIAPLAY_VIDEO_MJPG!!\r\n "));
        }

        // Set output source
        if (KeyScan_IsTVPlugIn() == TRUE)
        {
            //g_MediaPlayObj.iAudioType = AUDIO_OUTPUT_HP;
		g_MediaPlayObj.iAudioType = AUDIO_OUTPUT_SPK;//henry 20141225
			
        }
        else
        {
            g_MediaPlayObj.iAudioType = AUDIO_OUTPUT_HP;
        }
        //#NT#2011/06/08#Meg Lin -begin
        if (g_MediaPlayObj.u32decodeMode == MEMORY_NORMAL)
        {
            g_MediaPlayObj.u32MaxHDMIWidth= 1280; //2011/06/07 Meg Lin
            g_MediaPlayObj.u32MaxHDMIHeight= 720; //2011/06/07 Meg Lin
        }
        else//TINY
        {
            g_MediaPlayObj.u32MaxHDMIWidth= 640; //2011/06/07 Meg Lin
            g_MediaPlayObj.u32MaxHDMIHeight= 720; //2011/06/07 Meg Lin
        }
        //#NT#2011/06/08#Meg Lin -end
        MediaPlay_Open(&g_MediaPlayObj);
        openMedia = MediaPlay_WaitReady();
        //??  aud_setOutput(g_MediaPlayObj.iAudioType);//2009/10/14 Meg added
        aud_setVolume(GetAudioVolume());
        if (openMedia != E_OK)
        {
            //FlowPB_StopAVI();
            //Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_MOVSTOP, 0);
            UIPlay_MovStop(&UIPlayObjCtrl, 0, 0);
            return NVTEVT_CONSUME;
        }
        MediaPlay_FWD();
    }
    return NVTEVT_CONSUME;
}

INT32 UIPlay_MovPause(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MediaPlay_Pause();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_MovResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiPlayDir;

    uiPlayDir = FlowPB_GetPlayDir();

    if (uiPlayDir == MEDIAPLAY_FORWARD)
    {
        MediaPlay_SpeedUp(SPEED_NORMAL);
        MediaPlay_FWD();
        MediaPlay_WaitFFBW1XFinish();
    }
    else
    {
        MediaPlay_SpeedUp(SPEED_NORMAL);
        MediaPlay_BWD();
        MediaPlay_WaitFFBW1XFinish();
    }
    return NVTEVT_CONSUME;
}

UINT32 UIPlay_GetSpeedUp(UINT32 uiSetting)
{
  UINT32 uiRet;

    switch (uiSetting)
    {
      case PLB_FWD_MOV_2x:
      case PLB_BWD_MOV_2x:
        uiRet = SPEED_H264_1_5X; //2x
        break;
      case PLB_FWD_MOV_4x:
      case PLB_BWD_MOV_4x:
        uiRet = SPEED_H264_3_75X; //4x
        break;
      case PLB_FWD_MOV_8x:
      case PLB_BWD_MOV_8x:
        uiRet = SPEED_H264_7_5X; //8x
        break;
    }

   return uiRet;
}

INT32 UIPlay_MovFwd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSpeedLevel, uiPlayDir;
    UINT32  uiSelect;

    if (paramNum>=1)
        uiSelect = paramArray[0];

    if (uiSelect==PLB_FWD_MOV_1x)
    {
        MediaPlay_SpeedUp(SPEED_NORMAL);
        MediaPlay_FWD();
        MediaPlay_WaitFFBW1XFinish();
    } else {
        uiSpeedLevel = UIPlay_GetSpeedUp(uiSelect);
        MediaPlay_SpeedUp(uiSpeedLevel);
        MediaPlay_FWD();
        MediaPlay_WaitStepFinish();
    }

#if 0
    uiPlayDir = FlowPB_GetPlayDir();

    if (uiPlayDir == MEDIAPLAY_FORWARD)
    {
        MediaPlay_SpeedUp(uiSpeedLevel);
        MediaPlay_FWD();
        MediaPlay_WaitStepFinish();
    }
    else
    {
        FlowPB_SetPlayDir(MEDIAPLAY_FORWARD);
        MediaPlay_SpeedUp(SPEED_H264_1_5X);
        MediaPlay_FWD();
        MediaPlay_WaitStepFinish();
    }
#endif
    return NVTEVT_CONSUME;
}

INT32 UIPlay_MovBwd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSpeedLevel, uiPlayDir;
    UINT32  uiSelect;

    if (paramNum>=1)
        uiSelect = paramArray[0];

    if (uiSelect==PLB_FWD_MOV_1x)
    {
        MediaPlay_SpeedUp(SPEED_NORMAL);
        MediaPlay_FWD();
        MediaPlay_WaitFFBW1XFinish();
    } else {
        uiSpeedLevel = UIPlay_GetSpeedUp(uiSelect);
        MediaPlay_SpeedUp(uiSpeedLevel);
        MediaPlay_BWD();
        MediaPlay_WaitStepFinish();
    }

#if 0
    uiSpeedLevel = SPEED_H264_5X;
    uiPlayDir = FlowPB_GetPlayDir();

    if (uiPlayDir == MEDIAPLAY_BACKWARD)
    {
        MediaPlay_SpeedUp(uiSpeedLevel);
        MediaPlay_BWD();
        MediaPlay_WaitStepFinish();
    }
    else
    {
        FlowPB_SetPlayDir(MEDIAPLAY_BACKWARD);
        MediaPlay_SpeedUp(SPEED_H264_1_5X);
        MediaPlay_BWD();
        MediaPlay_WaitStepFinish();
    }
#endif
    return NVTEVT_CONSUME;
}

INT32 UIPlay_MovFwdStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MediaPlay_SpeedUp(SPEED_NORMAL);
    MediaPlay_FWD();
    MediaPlay_WaitFFBW1XFinish();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_MovBwdStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MediaPlay_SpeedUp(SPEED_NORMAL);
    MediaPlay_BWD();
    MediaPlay_WaitFFBW1XFinish();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WAVSTUDIO_FILEINFO  WavFileInfo;

    WavFileInfo.uiDirID     = PB_GetCurFileDirId();
    WavFileInfo.uiFileID    = PB_GetCurFileNameId();

    // Set voice memo type
    WavFileInfo.uiType      = WAV_FILE_SINGLE;

    WavStudio_SetFile(&WavFileInfo);

    // Stop any sound played by PlaySound task
    ////PlaySound_Stop();

    // Buggy, must exit and enter again to make this information works correctly
    WavStudio_Play();

    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WavStudio_Stop();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavPause(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WavStudio_Pause();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WavStudio_Resume();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavRecord(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WAVSTUDIO_FILEINFO  WavFileInfo;

    WavFileInfo.uiDirID     = WAV_DIR_FILE_AUTO;
    WavFileInfo.uiFileID    = WAV_DIR_FILE_AUTO;

    // Set voice memo type
    WavFileInfo.uiType      = WAV_FILE_SINGLE;

    WavStudio_SetFile(&WavFileInfo);

    WavStudio_Record();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavRecordStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WavStudio_Stop();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavRecordPause(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WavStudio_Pause();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_WavRecordResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WavStudio_Resume();
    return NVTEVT_CONSUME;
}

INT32 UIPlay_Aud_Volume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
  AUDIO_DEVICE_OBJ    AudObj;
  UINT32 uhSelect = 0;

    if(paramNum>0)
        uhSelect= paramArray[0];

    SetAudioVolume(uhSelect);
    if (aud_getLockStatus() == TASK_LOCKED)
    {
        aud_setVolume(uhSelect);
    }
    else
    {
        aud_getDeviceObject(&AudObj);
        aud_open(&AudObj);
        aud_setVolume(uhSelect);
        aud_close();
    }

    return NVTEVT_CONSUME;
}

EVENT_ENTRY UIPlayObjCtrlCmdMap[] =
{
    {NVTEVT_PLAYINIT,          UIPlay_OnInit},
    {NVTEVT_PLAYREINIT,        UIPlay_OnReInit},
    {NVTEVT_PLAYNEXT,          UIPlay_OnNext},
    {NVTEVT_PLAYPREV,          UIPlay_OnPrev},
    {NVTEVT_PLAYCURR,          UIPlay_OnCurr},
    {NVTEVT_PLAYROTATE,        UIPlay_OnRotate},
    {NVTEVT_MOVPLAY,           UIPlay_MovPlay},
    {NVTEVT_MOVSTOP,           UIPlay_MovStop},
    {NVTEVT_MOVPAUSE,          UIPlay_MovPause},
    {NVTEVT_MOVRESUME,         UIPlay_MovResume},
    {NVTEVT_MOVFWD,            UIPlay_MovFwd},
    {NVTEVT_MOVBWD,            UIPlay_MovBwd},
    {NVTEVT_MOVFWDSTOP,        UIPlay_MovFwdStop},
    {NVTEVT_MOVBWDSTOP,        UIPlay_MovBwdStop},
    {NVTEVT_WAVPLAY,           UIPlay_WavPlay},
    {NVTEVT_WAVSTOP,           UIPlay_WavStop},
    {NVTEVT_WAVPAUSE,          UIPlay_WavPause},
    {NVTEVT_WAVRESUME,         UIPlay_WavResume},
    {NVTEVT_WAVRECORD,         UIPlay_WavRecord},
    {NVTEVT_WAVRECORDSTOP,     UIPlay_WavRecordStop},
    {NVTEVT_WAVRECORDPASUE,    UIPlay_WavRecordPause},
    {NVTEVT_WAVRECORDRESUME,   UIPlay_WavRecordResume},
    {NVTEVT_AUD_VOLUME,        UIPlay_Aud_Volume},
    {NVTEVT_EXE_DEL,           UIPlay_OnExeDel},
    {NVTEVT_EXE_COPY2CARD,     UIPlay_OnExeCopy2Card},
    {NVTEVT_EXE_SLIDE,         UIPlay_OnExeSlide},
    {NVTEVT_EXE_DPOF,          UIPlay_OnExeDPOF},
    {NVTEVT_EXE_PROTECT,       UIPlay_OnExeProtect},
    {NVTEVT_EXE_THUMBINIT,     UIPlay_OnExeThumbInit},
    {NVTEVT_EXE_THUMB,         UIPlay_OnExeThumb},
    {NVTEVT_ZOOMINIT,          UIPlay_OnZoomInit},
    {NVTEVT_ZOOMIN,            UIPlay_OnZoomIn},
    {NVTEVT_ZOOMOUT,           UIPlay_OnZoomOut},
    {NVTEVT_ZOOMNAVI,          UIPlay_OnZoomNavi},
    {NVTEVT_SLIDEINIT,         UIPlay_OnSlideInit},
    {NVTEVT_SLIDEKEEP,         UIPlay_OnSlideKeep},
    {NVTEVT_SLIDESTOP,         UIPlay_OnSlideStop},
    {NVTEVT_OPENSPECIFIC,      UIPlay_OpenSpecific},
    {NVTEVT_NULL,              0},  //End of Command Map
};

VControl UIPlayObjCtrl={
    "UIPlayObj",
    UIPlayObjCtrlCmdMap,
    0,
    CTRL_SYS,   // CTRL_SYS
    0,  //data
    0,
    0,
    {0,0,0,0},
    0,
    0
};

/////////////////////////////////////////////////////////////////////////

void UIPlay_SetData(PLAY_STR_SET attribute, UINT32 value)
{
    switch(attribute)
    {
        case PLAY_ZOOMTABLE:
            pZoomLevelTbl = (UINT32 *)value;
            break;
        case PLAY_SLIDERTIME:
            g_uiSliderTime = value;
            break;
        default:
            UIPLAY_ERR(("[UIPlay_SetData]no this attribute"));
            break;
    }
}

UINT32 UIPlay_GetData(PLAY_DATA_SET attribute)
{
    PJPGHEAD_DEC_CFG    pJPGInfo;
    PPLAY_OBJ   pPlayObj;

    pJPGInfo = PB_GetCurrImageInfo();
    pPlayObj = PB_GetPBObject();
    switch(attribute)
    {
        case PLAY_PBSTATUS:
            return g_uiStatus;
        case PLAY_CURRMODE:
            return PB_GetCurrMode();
        case PLAY_FILEFORMAT:
            return PB_GetCurFileFormat();
        case PLAY_FILESEQID:
            return PB_GetCurrFileSysInfo(PLAYFILEINFO_FILESEQ);
        case PLAY_EXIF_ORI:
            return (UINT32)pJPGInfo->pExifInfo->Orientation;
        case PLAY_LOCKSTATUS:
            return PB_GetFILELock();
        case PLAY_DPOFQTY:
            return PB_GetCurrDPOFQty();
        case PLAY_DPOFDATESTATUS:
            return PB_GetCurrDPOFDateOn();
        case PLAY_DIRID:
            return PB_GetCurFileDirId();
        case PLAY_FILEID:
            return PB_GetCurFileNameId();
        case PLAY_FILENUM:
            return PB_GetCurrFileSysInfo(PLAYFILEINFO_FILENUMS);
        case PLAY_SIZEINDEX:
            return 0;//to do
        case PLAY_QUALITY:
            return (UINT32)pJPGInfo->pExifInfo->JPGQuality;
        case PLAY_ISO:
            return (UINT32)pJPGInfo->pExifInfo->ISOValue;
        case PLAY_ZOOMINDEX:
            return PB_GetCurrZoomIndex();
        case PLAY_IMGWIDTH:
            return (UINT32)pJPGInfo->imagewidth;
        case PLAY_IMGHEIGHT:
            return (UINT32)pJPGInfo->imageheight;
        case PLAY_IMGWIDTH_ORI:
            return (UINT32)pJPGInfo->ori_imagewidth;
        case PLAY_IMGHEIGHT_ORI:
            return (UINT32)pJPGInfo->ori_imageheight;
        case PLAY_PANSRCX:
            return pPlayObj->pstPanInfo->PanSrcStartX;
        case PLAY_PANSRCY:
            return pPlayObj->pstPanInfo->PanSrcStartY;
        case PLAY_PANMAXX:
            return pPlayObj->pstPanInfo->PanMaxX;
        case PLAY_PANMAXY:
            return pPlayObj->pstPanInfo->PanMaxY;
        case PLAY_SOCIALNETWORK:
            {
                UINT32 uiSocialNetworkValue = PB_GetSocialNetworkValue();
                if (uiSocialNetworkValue >= SOCIALNETWORK_SETTING_MAX){
                    uiSocialNetworkValue = SOCIALNETWORK_NONE;
                }
                return uiSocialNetworkValue;
            }
        default:
            UIPLAY_ERR(("[UIPlay_GetData]no this attribute"));
            return 0;
    }
}

CHAR * UIPlay_GetStrData(PLAY_STR_SET attribute)
{
    PJPGHEAD_DEC_CFG    pJPGInfo;
    pJPGInfo = PB_GetCurrImageInfo();
    switch(attribute)
    {
        case PLAY_EV:
            return (CHAR *)pJPGInfo->pExifInfo->ExpBiasValue;
        case PLAY_DATETIME:
            return (CHAR *)&pJPGInfo->pExifInfo->CaptureTime;

        default:
            UIPLAY_ERR(("[UIPlay_GetStrData]no this attribute"));
            return 0;
    }
}

