#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTUserCommand.h"
#include "debug.h"
#include "SysCfg.h"
#include "UIFlowWndPrint.h"
#include "PlaybackTsk.h"
#include "GlobalVar.h"
#include "DPOF.h"
#include "UIPrintObj.h"
#include "UISystemObj.h"
#include "UIFlow.h"


//-------------------------------------------------------------------------------------------------
static PLAY_OBJ UIPrintPlayObj;
static BOOL UIPrintPlaybackIsOpen = FALSE;
static UINT32 UIPrintPaperType = 0;
static UINT32 UIPrintLayout = 0;
//-------------------------------------------------------------------------------------------------
BOOL UIPrint_GetDPOFExist(void)
{
    UINT32  TempData;
    BOOL IsExist = DPOF_GetHandleNum(&TempData);

    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetDPOFExist:%d\r\n",IsExist));
    return IsExist;
}

//-------------------------------------------------------------------------------------------------
UINT32 UIPrint_GetDCFFileAmount(void)
{
    FST_DCF_INFO*   pTempDCFInfo = FilesysGetDCFInfo();

    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetDCFFileAmount:%d\r\n",(pTempDCFInfo->TolFileNum)));
    return (pTempDCFInfo->TolFileNum);
}
//-------------------------------------------------------------------------------------------------
UINT32 UIPrint_GetPaperType(void)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetPaperType:0x%08X\r\n",UIPrintPaperType));
    return UIPrintPaperType;
}
//-------------------------------------------------------------------------------------------------
UINT32 UIPrint_Getlayout(void)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Getlayout:0x%08X\r\n",UIPrintLayout));
    return UIPrintLayout;
}
//-------------------------------------------------------------------------------------------------
UINT32 UIPrint_Playback_GetCurrWidth(void)
{
    JPGHEAD_DEC_CFG* pJpgDecInfo;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Playback_GetCurrWidth\r\n"));

    pJpgDecInfo = PB_GetCurrImageInfo();
    return  pJpgDecInfo->ori_imagewidth;
}
//-------------------------------------------------------------------------------------------------
UINT32 UIPrint_Playback_GetCurrHeight(void)
{
    JPGHEAD_DEC_CFG* pJpgDecInfo;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Playback_GetCurrHeight\r\n"));

    pJpgDecInfo = PB_GetCurrImageInfo();
    return  pJpgDecInfo->ori_imageheight;
}
//-------------------------------------------------------------------------------------------------
UINT16 UIPrint_Playback_GetDirId(void)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Playback_GetDirId\r\n"));
    return PB_GetCurrFileSysInfo(PLAYFILEINFO_DIRID);
}
//-------------------------------------------------------------------------------------------------
UINT16 UIPrint_Playback_GetFileId(void)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Playback_GetFileId\r\n"));
    return PB_GetCurrFileSysInfo(PLAYFILEINFO_FILEID);
}
//-------------------------------------------------------------------------------------------------
void UIPrint_Playback_Open(void)
{
    UINT32          uiPoolAddr, uiPoolAddr2;
    //#NT#2011/06/14#Chris Chung -begin
    //#NT#Mark to fix the bug that fileID is wrong if the last file is AVI or the last folder is empty.
    #if 0
    FILE_TSK_DCF_PARAM *pDCFParam;
    UH  maxDirId, maxFileId;
    #endif
    //#NT#2011/06/14#Chris Chung -end

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Playback_Open\r\n"));

    memset(&UIPrintPlayObj, 0, sizeof(PLAY_OBJ));

    get_blk((VP*)&(uiPoolAddr), POOL_ID_FB);
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);
    SysGetPTPObjectAddr(&uiPoolAddr2);

    UIPrintPlayObj.uiMemoryAddr        = uiPoolAddr;
    UIPrintPlayObj.uiMemorySize             = uiPoolAddr2 - uiPoolAddr;
    UIPrintPlayObj.CallBackFunc             = NULL;
    UIPrintPlayObj.uiDeviceWidth            = g_LCDSize.uiWinWidth;
    UIPrintPlayObj.uiDeviceHeight           = g_LCDSize.uiWinHeight;
    UIPrintPlayObj.uiDisplayFBWidth         = g_LCDSize.uiWidth;
    UIPrintPlayObj.uiDisplayFBHeight        = g_LCDSize.uiHeight;
    UIPrintPlayObj.Orientation              = PLAY_HORIZONTAL;
    UIPrintPlayObj.ui1stPlayMode            = PLAYMODE_PRIMARY;
    UIPrintPlayObj.usPlayFileFormat         = PLAYFMT_JPG;

    UIPrintPlayObj.Flags                    = PB_ENABLE_TRANSFROM;
    UIPrintPlayObj.uiThumbShowImageMode     = PB_SHOW_THUMB_IMAGE_ONE_BY_ONE;

    UIPrintPlayObj.ThumbOnlyRect.uiLeft     = PB_THUMB_ONLY_DEFAULT_SETTING;
    UIPrintPlayObj.ThumbOnlyRect.uiTop      = PB_THUMB_ONLY_DEFAULT_SETTING;
    UIPrintPlayObj.ThumbOnlyRect.uiWidth    = PB_THUMB_ONLY_DEFAULT_SETTING;
    UIPrintPlayObj.ThumbOnlyRect.uiHeight   = PB_THUMB_ONLY_DEFAULT_SETTING;

    //#NT#2011/06/14#Chris Chung -begin
    //#NT#modify for panel display ratio
    UIPrintPlayObj.uiPanelWidth             = 320;//PB_PANEL_NO_SCALING;
    UIPrintPlayObj.uiPanelHeight            = 240;//PB_PANEL_NO_SCALING;
    //#NT#2011/06/14#Chris Chung -end
    UIPrintPlayObj.bDisplayLastImg          = TRUE;
    if(PB_Open(&UIPrintPlayObj) != E_OK)
    {
        UIPRINT_ERR(("UIPRINT_E:UIPrint_Playback_Open:Open error\r\n"));
    }
    PB_WaitCommandFinish(PB_WAIT_INFINITE);

    //#NT#2011/06/14#Chris Chung -begin
    //#NT#Mark to fix the bug that fileID is wrong if the last file is AVI or the last folder is empty.
    #if 0
    pDCFParam = FilesysGetFSTParam();
    maxDirId = pDCFParam->MaxDirID;
    maxFileId = pDCFParam->MaxFileID;
    PB_OpenSpecFile(maxDirId, maxFileId);
    #endif
    //#NT#2011/06/14#Chris Chung -end
}
//-------------------------------------------------------------------------------------------------
void UIPrint_Playback_Close(void)
{
    if(PB_Close(PB_WAIT_INFINITE) != E_OK)
    {
        UIPRINT_ERR(("UIPRINT_E:UIPrint_Playback_Close:Close error\r\n"));
    }
}
//-------------------------------------------------------------------------------------------------
void UIPrint_EventHandler(DPS_EVENT_ID EventId)
{
    UINT16  TempJobStatusCurrent, TempJobStatusTotal;
    UIPRINT_MSG(("UIPRINT_M:UIPrint_EventHandler:EventId=%d\r\n",EventId));

    switch (EventId)
    {
        case DPS_EVENT_INIT_OK:
        {
            if(!UIPrintPlaybackIsOpen)
            {
                UIPrintPlaybackIsOpen = TRUE;
                //UIPrint_Playback_Open();
            }
            break;
        }

        case DPS_EVENT_END:
        {
            //UIPrint_Playback_Close();
            break;
        }

        case DPS_EVENT_PRINT_FINISH:
        {
            UIPrint_SetJobStatus(0, 0);
            break;
        }

        case DPS_EVENT_NOTIFY_JOB:
        {
            DPS_GetJobStatus(&TempJobStatusCurrent, &TempJobStatusTotal);
            UIPrint_SetJobStatus(TempJobStatusCurrent, TempJobStatusTotal);
            break;
        }

        case DPS_EVENT_UNKNOWN_ERROR_STATUS:
        case DPS_EVENT_XML_SYNTAX_ERROR:
        {
            UIPRINT_ERR(("UIPRINT_E:UIPrint_EventHandler:Unkonw error\r\n"));
            return;
        }

        default:
        {
            break;
        }
    }

    Ux_PostEvent(NVTEVT_CB_PRINT_DPS_EVENT, 1, EventId);
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_Init(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Init\r\n"));

    UIPrintPlaybackIsOpen = FALSE;
    DPS_RegEventHandler(UIPrint_EventHandler);
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_Close(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_Close\r\n"));
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_PrintImage(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_PrintImage\r\n"));
    DPS_PrintImage(&UIPrintData, UIPrintDataImageCount, FALSE, 0);
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_PrintAll(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  StartAddr,TotalImages;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_PrintALL\r\n"));

    USICD_GetPrintAllData(&StartAddr, &TotalImages);
    DPS_PrintImage((DPS_PRINT_DATA_INFO*)StartAddr, TotalImages, TRUE, UIPrint_Image_GetPrintAllCopy());

    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_PrintIndex(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  StartAddr,TotalImages;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_PrintIndex\r\n"));

    DPS_SetJobConfig(DPS_CAPITEM_LAYOUTS, DPS_CAP_LAYOUT_IDX);
    USICD_GetPrintAllData(&StartAddr, &TotalImages);
    DPS_PrintImage((DPS_PRINT_DATA_INFO*)StartAddr, TotalImages, TRUE, 1);

    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_PrintDPOF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_PrintDPOF\r\n"));
    DPS_PrintDPOF();
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_PrintDPOFRestart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_PrintDPOFRestart\r\n"));
    DPS_PrintDPOFRestart();
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_PrintAbort(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_PrintAbort\r\n"));
    DPS_PrintAbort();
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_PrintContinue(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_PrintContinue\r\n"));
    DPS_PrintContinue();
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_GetCapabilityAll(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetCapabilityAll\r\n"));
    if(paramNum == 1)
    {
        DPS_GetCapabilityAll((DPS_CAPABILITY_INFO*)(paramArray[0]));
    }
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_GetCapabilityPaperType(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetCapabilityPaperType\r\n"));
    if(paramNum == 1)
    {
        UIPrintPaperType = DPS_GetPaperType(paramArray[0]);
    }
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_GetCapabilityLayout(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_GetCapabilityLayout\r\n"));
    if(paramNum == 1)
    {
        UIPrintLayout = DPS_GetLayout(paramArray[0]);
    }
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_SetJobConfig(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIPRINT_MSG(("UIPRINT_M:UIPrint_SetJobConfig\r\n"));
    if(paramNum == 2)
    {
        DPS_SetJobConfig(paramArray[0], paramArray[1]);
    }
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_Playback_PlayCurr(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_Playback_PlayNext(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PLAY_SINGLE_OBJ PlaySingleObj;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Playback_PlayNext\r\n"));

    memset(&PlaySingleObj, 0, sizeof(PLAY_SINGLE_OBJ));
    PlaySingleObj.JumpOffset = 1;
    PlaySingleObj.PlayCommand = (PB_SINGLE_NEXT | PB_SINGLE_PRIMARY);
    PlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;//SLIDE_EFFECT_SLIDEINLEFT2RIGHT;
    PB_PlaySingleMode(&PlaySingleObj);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 UIPrint_Playback_PlayPrevious(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PLAY_SINGLE_OBJ PlaySingleObj;

    UIPRINT_MSG(("UIPRINT_M:UIPrint_Playback_PlayPrevious\r\n"));

    memset(&PlaySingleObj, 0, sizeof(PLAY_SINGLE_OBJ));
    PlaySingleObj.JumpOffset = 1;
    PlaySingleObj.PlayCommand = (PB_SINGLE_PREV | PB_SINGLE_PRIMARY);
    PlaySingleObj.MoveImgMode = SLIDE_EFFECT_NONE;//SLIDE_EFFECT_SLIDEINRIGHT2LEFT;
    PB_PlaySingleMode(&PlaySingleObj);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
EVENT_ENTRY UIPrintObjCtrlCmdMap[] =
{
    {NVTEVT_PRINT_INIT,                     UIPrint_Init},
    {NVTEVT_PRINT_CLOSE,                    UIPrint_Close},
    {NVTEVT_PRINT_START_PRINT,              UIPrint_PrintImage},
    {NVTEVT_PRINT_START_ALL,                UIPrint_PrintAll},
    {NVTEVT_PRINT_START_INDEX,              UIPrint_PrintIndex},
    {NVTEVT_PRINT_START_DPOF,               UIPrint_PrintDPOF},
    {NVTEVT_PRINT_STOP_PRINT,               UIPrint_PrintAbort},
    {NVTEVT_PRINT_CONTINUE_PRINT,           UIPrint_PrintContinue},
    {NVTEVT_PRINT_CONTINUE_DPOF,            UIPrint_PrintDPOFRestart},
    {NVTEVT_PRINT_GET_CAPABILITY_ALL,       UIPrint_GetCapabilityAll},
    {NVTEVT_PRINT_GET_CAPABILITY_PAPERTYPE, UIPrint_GetCapabilityPaperType},
    {NVTEVT_PRINT_GET_CAPABILITY_LAYOUT,    UIPrint_GetCapabilityLayout},
    {NVTEVT_PRINT_SET_JOB_CONFIG,           UIPrint_SetJobConfig},
    {NVTEVT_PLAYCURR,                       UIPrint_Playback_PlayCurr},
    {NVTEVT_PLAYNEXT,                       UIPrint_Playback_PlayNext},
    {NVTEVT_PLAYPREV,                       UIPrint_Playback_PlayPrevious},
    {NVTEVT_NULL,                           NULL}
};
//-------------------------------------------------------------------------------------------------
VControl UIPrintObjCtrl={
    "UIPrintObj",
    UIPrintObjCtrlCmdMap,
    0,
    CTRL_SYS,   // CTRL_SYS
    0,  //data
    0,
    0,
    {0,0,0,0},
    0,
    0
};
//-------------------------------------------------------------------------------------------------

