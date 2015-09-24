/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.c
    @ingroup    mIPRJAPTest

    @brief      Calibration task API
                Calibration task API

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Kernel.h"
#include "SysCfg.h"
#include "CalibrationTsk.h"
#include "CalibrationInt.h"
#include "Calibration.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"
#include "FileSysTsk.h"
#include "UISystemObj.h"
#include "UIFlow.h"
#include "SystemInit.h"

#if (_CALIBRATION_MODE_ == ENABLE)
#include "UIFramework.h"
#include "UIGraphics.h"

static DC**     pDCList;

//1. 中間圓對焦OK時的 "對比值"(Param_01)
//2. 週圍圓對焦OK時的 "對比值"(Param_02)
//3. 中間圓 "對比值" 的 tolerance(Param_03)
//4. 週圍圓 "對比值" 的 tolerance(Param_04)
//5. 需要粗調時, "對比值" 的顯示顏色(Param_05)
//6. 需要微調時, "對比值" 的顯示顏色(Param_06)
//7. 調焦OK時, "對比值" 顯示的顏色(Param_07)
typedef enum
{
    FocusCenter,                //Param_01
    FocusEdge,                  //Param_02
    FocusCenterTolerance,    //Param_03
    FocusEdgeTolerance,      //Param_04
    FocusRoughColor,         //Param_05
    FocusFineColor,        //Param_06
    FocusOkColor,           //Param_07
    FocusTotal
}CAL_PARAM_SET;


UINT32 g_CalParam[FocusTotal];

/*
    0       1
        2
    3       4
*/
// 2 is center point,0,1,3,4 are edge point
void Cal_ShowFocusResult(UINT32 *pFocusPoint)
{
    char str[64];
    UINT32 leftX = 50 ;
    UINT32 rightX = 240;
    UINT32 upY = 40;
    UINT32 downY =180;

    if(!pFocusPoint)
        return;
    debug_err(("pFocusPoint = %d,%d,%d,%d,%d\r\n",pFocusPoint[0],pFocusPoint[1],pFocusPoint[2],pFocusPoint[3],pFocusPoint[4]));
    sprintf(str,"%ld",pFocusPoint[0]);
    if(pFocusPoint[0]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,leftX,upY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[0]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,leftX,upY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,leftX,upY,g_CalParam[FocusRoughColor]);

    sprintf(str,"%ld",pFocusPoint[1]);
    if(pFocusPoint[1]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,rightX,upY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[1]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,rightX,upY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,rightX,upY,g_CalParam[FocusRoughColor]);

    //center point
    sprintf(str,"%ld",pFocusPoint[2]);
    if(pFocusPoint[2]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,(leftX+rightX)/2 ,(upY+downY)/2,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[2]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,(leftX+rightX)/2 ,(upY+downY)/2,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,(leftX+rightX)/2 ,(upY+downY)/2,g_CalParam[FocusRoughColor]);


    sprintf(str,"%ld",pFocusPoint[3]);
    if(pFocusPoint[3]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,leftX,downY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[3]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,leftX,downY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,leftX,downY,g_CalParam[FocusRoughColor]);

    sprintf(str,"%ld",pFocusPoint[4]);
    if(pFocusPoint[4]>g_CalParam[FocusEdge])
        Cal_ShowStringWithColor(str,rightX,downY,g_CalParam[FocusOkColor]);
    else if(pFocusPoint[4]>g_CalParam[FocusEdge]-g_CalParam[FocusEdgeTolerance] )
        Cal_ShowStringWithColor(str,rightX,downY,g_CalParam[FocusFineColor]);
    else
        Cal_ShowStringWithColor(str,rightX,downY,g_CalParam[FocusRoughColor]);
}

BOOL Cal_GetFocusParam(INT8 * pFileName)
{
    char *pCalTxtBuf;
    UINT32 filesize = 5*1024;
    UINT32 i=0;

    SysGetSICDAddr((UINT32 *)&pCalTxtBuf);

    memset((char *)pCalTxtBuf,0,256);
    FilesysWaitInitFinish(FST_TIME_INFINITE);
    if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME,
                            (char *)pFileName,
                            (UB *)pCalTxtBuf,
                            (UW *)&filesize,
                            0,
                            FST_TIME_INFINITE)==FST_STA_OK)
    {
        if(filesize >0)
        {
            for(i=0;i<FocusTotal;i++)
            {
                g_CalParam[i] =atoi(pCalTxtBuf);
                pCalTxtBuf=strchr(pCalTxtBuf,0x20);
                pCalTxtBuf++;
                debug_err(("%d %d\r\n",i,g_CalParam[i]));
            }
        }
        return TRUE;
    }
    else
    {
        debug_err(("no focus params\r\n"));
        return FALSE;
    }

}

UINT32 Cal_GetDefaultCal(INT8 * pFileName)
{
    UINT32 itemNum = 0;
    UINT32 pCalTxtBuf;
    UINT32 filesize = 5*1024;

    SysGetSICDAddr(&pCalTxtBuf);

    memset((char *)pCalTxtBuf,0,256);
    FilesysWaitInitFinish(FST_TIME_INFINITE);
    if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME,
                            (char *)pFileName,
                            (UB *)pCalTxtBuf,
                            (UW *)&filesize,
                            0,
                            FST_TIME_INFINITE)==FST_STA_OK)
    {
        if(filesize >0)
        {
            debug_err(("%s",pCalTxtBuf));

            for(itemNum =0 ;itemNum <= g_CalMaxItemNum;itemNum++)
            {
                if(strncmp((char *)pCalTxtBuf,
                              g_CalItem[itemNum].pItemName,strlen(g_CalItem[itemNum].pItemName))==0)
                {
                    debug_err(("  %s \r\n",g_CalItem[itemNum].pItemName));
                    return itemNum;
                }
            }
        }
    }
    return CAL_INVALID_DEFAULT;

}

//#NT#2007/05/04#Steven Wang -end

/**
  Find file that filename with extension in root folder

  Find file that filename with extension in root folder and return the main
  filename (convert to integer of UINT32)
  [Calibration internal API]

  @param INT8 *pExt: extension
  @return UINT32: Filename of integer
*/
UINT32 Cal_GetDefaultInRoot(INT8 * pExt)
{
    UINT32          uiResult, i;
    INT8            orgFilename[9];
    FIND_DATA       findData;
    PFILE_SEARCH    pHSearch;

    uiResult = CAL_INVALID_DEFAULT;

    if (Primary_GetStorage() != PRIMARY_STGTYPE_CARD)
    {
        return CAL_INVALID_DEFAULT;
    }

    if (FilesysWaitInitFinish(FST_TIME_NO_WAIT) != FST_STA_OK)
    {
        Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);
    }

    FilesysWaitInitFinish(FST_TIME_INFINITE);

    pHSearch = fs_FindFirstFile("A:\\", &findData);
    while (pHSearch)
    {
        if (!M_IsDirectory(findData.attrib))
        {
            if (!strncmp(findData.FATExtName , pExt, 3))
            {
                memcpy(orgFilename, findData.FATMainName, 8);
                orgFilename[8] = '\0';
                sscanf(orgFilename, "%08X", &uiResult);
                for (i=0; i<8; i++)
                {
                    if (isxdigit(*(orgFilename + i)) == 0)
                    {
                        uiResult = CAL_INVALID_DEFAULT;
                        break;
                    }
                }
                break;
            }
        }

        if(fs_FindNextFile(pHSearch, &findData) != FSS_OK)
        {
            break;
        }
    }

    if (pHSearch)
    {
        fs_FindClose(pHSearch);
    }

    return uiResult;
}

/**
  Default function

  Default function

  @param void
  @return void
*/
void Cal_NoFunction(void)
{
    Cal_ShowString("No Function", &g_CalStringRect, TRUE);
}

/**
  Set default settings and generate serial number

  Set default settings and generate serial number

  @param void
  @return void
*/
void Cal_SetDefault(void)
{
#if 0
    UINT32  uiSerialNumber;
    INT8    StringBuffer[32];

    // Set default settings
    SysReset_MenuInfo();
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE);

    // Generate serial number
    uiSerialNumber = Cal_GetSerialNum(1);
    sprintf(StringBuffer, "Ser #: 0x%.8lX", uiSerialNumber);
    Cal_ShowString(StringBuffer, &g_CalStringRect, TRUE);
#endif
}
void Cal_ShowStringWithColor(char *pStr, UINT16 X, UINT16 Y,UINT32 color)
{
    RECT rect;

    rect.uiLeft = X;
    rect.uiTop = Y;
    rect.uiWidth = 320;//OSD_W;
    rect.uiHeight = 30;//OSD_H;

    pDCList = (DC**)UI_BeginScreen();

    GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD],rect.uiLeft,rect.uiTop,rect.uiLeft+rect.uiWidth,rect.uiTop+rect.uiHeight);
    //debug_err(("c %d %s \r\n",color ,pStr));
    GxGfx_SetTextColor(color, color, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], rect.uiLeft, rect.uiTop, pStr);
    UI_EndScreen((UINT32)pDCList);
}

void Cal_ClearOSD(UINT32 uiColorIdx)
{
    DC** pDCList;

    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(0, uiColorIdx, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD], 0, 0, pDCList[GxGfx_OSD]->size.w, pDCList[GxGfx_OSD]->size.h);
    UI_EndScreen((UINT32)pDCList);
}

void Cal_ShowRectStringWithColor(INT8 *pStr, PRECT pRect, BOOL bClear,UINT32 color)
{
    pDCList = (DC**)UI_BeginScreen();
    if(bClear)
    {
        GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
        GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->uiLeft,pRect->uiTop,pRect->uiLeft+pRect->uiWidth,pRect->uiTop+pRect->uiHeight);
    }
    GxGfx_SetTextColor(color, color, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], pRect->uiLeft, pRect->uiTop, pStr);
    UI_EndScreen((UINT32)pDCList);
}

void Cal_ShowStringByColor(INT8 *pStr, PRECT pRect, UINT32 uiFrColor, UINT32 uiBgColor)
{
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(uiBgColor, uiBgColor, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->uiLeft,pRect->uiTop,pRect->uiLeft+pRect->uiWidth,pRect->uiTop+pRect->uiHeight);
    GxGfx_SetTextColor(uiFrColor, 0, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], pRect->uiLeft, pRect->uiTop, pStr);
    UI_EndScreen((UINT32)pDCList);
}


void Cal_FillRect(PRECT pRect, UINT32 uiBgColor)
{
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(uiBgColor, uiBgColor, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->uiLeft,pRect->uiTop,pRect->uiLeft+pRect->uiWidth,pRect->uiTop+pRect->uiHeight);
    UI_EndScreen((UINT32)pDCList);

}
/**
  Show calibration string on OSD

  Show calibration string on OSD

  @param INT8 *pStr: String pointer
  @param PRECT pRect: Rectangle to display string, if NULL will use default rectangle
  @param BOOL bClear: Clear the rectangle before display string or not.
    TRUE -> Clear
    FALSE -> Don't clear
  @return void
*/
void Cal_ShowString(INT8 *pStr, PRECT pRect, BOOL bClear)
{
    pDCList = (DC**)UI_BeginScreen();
    if(bClear)
    {
        GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
        GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->uiLeft,pRect->uiTop,pRect->uiLeft+pRect->uiWidth,pRect->uiTop+pRect->uiHeight);
    }
    GxGfx_SetTextColor(_OSD_INDEX_RED, _OSD_INDEX_GRAY, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], pRect->uiLeft, pRect->uiTop, pStr);
    UI_EndScreen((UINT32)pDCList);
}

void Cal_FormatPstoreSPI(void)
{
    FLGPTN  uiFlag;
    UINT32  uiKeyAct, uiKeyCode, err;
    RECT    Rect = {0, 0, 320, 240};
    PSTORE  PStore;

    Cal_FillRect(&Rect, _OSD_INDEX_BLACK);
    Rect.uiLeft   = 56;
    Rect.uiTop    = 80;
    Rect.uiWidth  = 212;
    Rect.uiHeight = 24;
    Cal_ShowStringByColor("Formatting SPI...", &Rect, _OSD_INDEX_WHITE, _OSD_INDEX_BLACK);

    // format SPI
    PStore.uiSignature  = 0x55AA55AA;
    PStore.uiPSVersion  = 0x0001;
    PStore.uiPStoreSize = SysInit_GetPStoreSize();

    err = PStore_Format(&PStore);

    debug_err(("Format PS Done.\r\n"));

    if (err == E_OK)
    {
        Cal_RstCalRst();

        Cal_ShowStringByColor("Format OK!", &Rect, _OSD_INDEX_GREEN, _OSD_INDEX_BLACK);
    }
    else
    {
        Cal_ShowStringByColor("Format failed!", &Rect, _OSD_INDEX_RED, _OSD_INDEX_BLACK);
    }

    Rect.uiTop = 112;
    Cal_ShowStringByColor("Press any key to return.", &Rect, _OSD_INDEX_WHITE, _OSD_INDEX_BLACK);

    while (1)
    {
        wai_flg(&uiFlag, FLG_ID_KEY, 0xffffffff, TWF_ORW | TWF_CLR);

        uiKeyAct = uiFlag & FLGKEY_ACT_MASK;
        uiKeyCode = uiFlag & FLGKEY_KEY_MASK;

        // check key
        if (uiKeyCode && uiKeyAct == FLGKEY_PRESSED)
        {
            break;
        }
    }

}

/**
  Get serial number

  This function will scan file that filename with extension "SEN" in root folder
  and use the filename as serial number. It will return the serial number
  and rename the filename to next value.

  @param UINT32 uiIncrement: filename increment of next value
  @return UINT32: Serial number
*/
UINT32 Cal_GetSerialNum(UINT32 uiIncrement)
{
    INT8    orgFilename[16], newFilename[16];
    UINT32  uiSerialNum;


    uiSerialNum = Cal_GetDefaultInRoot("SEN");

    if (uiSerialNum != CAL_INVALID_DEFAULT && uiIncrement != 0)
    {
        sprintf(orgFilename, "A:\\%.8lX.SEN", uiSerialNum);
        sprintf(newFilename, "%.8lX.SEN", (UINT32)(uiSerialNum + uiIncrement));
        fs_rename(orgFilename, newFilename);
    }
    return uiSerialNum;
}

/**
  Open Calibration task

  Open Calibration task

  @param PCAL_APPOBJ pCalObj: Calibration task object, specify memory available in calibration task.
  @return ER
    E_SYS: Task is already opened
    E_PAR: Parameter error (memory address or size is invalid)
    E_OK : Task opened correctly
*/
ER Cal_Open(void)
{
    UINT32 uiPoolAddr;
    CAL_APPOBJ CalAppObj;

    if (g_bCalbirationOpened == TRUE)
    {
        return E_SYS;
    }

    g_bCalbirationOpened = TRUE;

    //Get memory for calibartion task
    get_blk((VP *)&uiPoolAddr, POOL_ID_FB);
    rel_blk(POOL_ID_FB, (VP)uiPoolAddr);

    CalAppObj.uiMemoryAddr = uiPoolAddr;
    CalAppObj.uiMemorySize = END_MEM - uiPoolAddr - POOL_SIZE_RAM_DISK;
    CalAppObj.pCalApi = Cal_GetCalApi();

    Cal_Init(&CalAppObj);

    return E_OK;
}

#endif

//@}
