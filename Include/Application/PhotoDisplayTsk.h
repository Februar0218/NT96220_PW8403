
#ifndef _PHOTODISPLAYTSK_H
#define _PHOTODISPLAYTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"
#include "appcommon.h"
#include "Lens.h"
#include "ImageEffect.h"
#include "Ide.h"

//
//FLG_ID_PHOTODISPLAY
//
#define FLGPHOTODISPLAY_IDLE                FLGPTN_BIT(0)
#define FLGPHOTODISPLAY_VIDEO1ON            FLGPTN_BIT(1)
#define FLGPHOTODISPLAY_VIDEO1OFF           FLGPTN_BIT(2)
#define FLGPHOTODISPLAY_VIDEO2ON            FLGPTN_BIT(3)
#define FLGPHOTODISPLAY_VIDEO2OFF           FLGPTN_BIT(4)
#define FLGPHOTODISPLAY_CHGSIZE             FLGPTN_BIT(5)
#define FLGPHOTODISPLAY_PREVIEW             FLGPTN_BIT(6)
#define FLGPHOTODISPLAY_VIDEO               FLGPTN_BIT(7)
#define FLGPHOTODISPLAY_VIDEOREC            FLGPTN_BIT(8)
#define FLGPHOTODISPLAY_IE_MULTIVIEW        FLGPTN_BIT(9)
#define FLGPHOTODISPLAY_IE_SINGLEVIEW       FLGPTN_BIT(10)
#define FLGPHOTODISPLAY_MULTIVIEW_EVENT     FLGPTN_BIT(11)
//#NT#2011/10/31#Ethan Lau -begin
//#NT# IME/IDE fw handshake
#define FLGPHOTODISPLAY_FWHANDSHAKE         FLGPTN_BIT(12)
//#NT#2011/10/31#Ethan Lau -end
#define FLGPHOTODISPLAY_MULTIVIEW_INITDONE  FLGPTN_BIT(15)
//#NT#2009/12/30#YC Peng -begin
//#NT#Add single view init done flag
#define FLGPHOTODISPLAY_SINGLEVIEW_INITDONE FLGPTN_BIT(16)
//#NT#2009/12/30#YC Peng -end
//#NT#2010/09/08#Yuppie Lin -begin
//#NT#Add for Preview IFE
#define FLGPHOTODISPLAY_PREVIEWIFE          FLGPTN_BIT(18)
//#NT#2010/09/08#Yuppie Lin -end
//#NT#2011/02/09#ethanlau -begin
//#NT#Add for ctrl ide buf idx
#define FLGPHOTODISPLAY_IDEIDXCHG           FLGPTN_BIT(19)
#define FLGPHOTODISPLAY_IDECTRLEXIT         FLGPTN_BIT(20)
#define FLGPHOTODISPLAY_IDEFWCTRLIDLE       FLGPTN_BIT(21)
//#NT#2011/02/09#ethanlau -end
typedef struct _PHOTODISPLAY_APPOBJ{
    UINT32  uiMemoryAddr;
    UINT32  uiMemorySize;
    void    (*CallBackFunc)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
    UINT32  uiDisplayOutWidth;
    UINT32  uiDisplayOutHeight;
} PHOTODISPLAY_APPOBJ,*PPHOTODISPLAY_APPOBJ;

typedef enum
{
    DISPLAY_MODE_PREVIEW        = 0x00000000,
    DISPLAY_MODE_VIDEO          = 0x00000001,
    DISPLAY_MODE_VIDEOREC       = 0x00000002,
    DISPLAY_MODE_IE_MULTIVIEW   = 0x00000003,
    DISPLAY_MODE_IE_SINGLEVIEW  = 0x00000004,
    DISPLAY_MODE_PREVIEWIFE     = 0x00000005,
    DISPLAY_MODE_MAX            = 0x00000006,
    //#NT#2011/10/31#Ethan Lau -begin
    //#NT#add IDE fw handshkae mech., this mode can or above mode to run fw handshake mech.
    //and it must open fwctrl first
    DISPLAY_MODE_FWHANDSHAKE    = 0x10000000,
    //#NT#2011/10/31#Ethan Lau -end
    ENUM_DUMMY4WORD(DISPLAY_MODE)
}   DISPLAY_MODE;


typedef enum
{
    _DisplayInHsize,
    _DisplayInVsize,
    _DisplayInLineoffset,
    _DisplayInCCLineoffset,
    _DisplayOutHsize,
    _DisplayOutVsize,
    ENUM_DUMMY4WORD(_IdeSize_enum)
}   _IdeSize_enum;

typedef struct  _DisplaySizeInfor
{
    UINT    DisplayInHsize;
    UINT    DisplayInVsize;
    UINT    DisplayInLineoffset;
    UINT    DisplayInCCLineoffset;
    UINT    DisplayOutHsize;
    UINT    DisplayOutVsize;

}   DisplaySizeInfor;

typedef struct _DisplayIdeParam
{
    IDE_VIDEOID VideoID;
    VIDEO_BUF_ADDR FbAddr;
    VIDEO_BUF_ATTR FbAttr;
    VOSD_WINDOW_ATTR FbWin;

}   DisplayIdeParam;

//-----------------------------------------------------------------------
// function prototype description
//-----------------------------------------------------------------------
extern ER   PhotoDisplay_Open(PPHOTODISPLAY_APPOBJ pPhotoDisplayObj);
extern ER   PhotoDisplay_Close(void);
extern BOOL PhotoDisplay_WaitIdle(BOOL bWaitInfinite,FLGPTN waiptn);
extern void PhotoDisplay_SetMode(DISPLAY_MODE Mode, BOOL bWaitInfinite);
//extern void PhotoDisplay_ChgSize(DisplaySizeInfor* SizeInfor, BOOL bWaitInfinite);
extern void PhotoDisplay_EnableVideo(IDE_VIDEOID VideoID, BOOL bWaitInfinite);
extern void PhotoDisplay_DisableVideo(IDE_VIDEOID VideoID, BOOL bWaitInfinite);
extern UINT PhotoDisplay_SetDisplaySizeInfor(UINT item, UINT value);
extern UINT PhotoDisplay_GetDisplaySizeInfor(UINT item);
extern void PhotoDisplay_IdeConfig(DisplayIdeParam* IdeParam);
//#NT#2011/02/10#ethanlau -begin
//#NT#fw ctrl ide buf idx
extern void PhotoDisplay_fwctrlBufidx_open(void);
extern void PhotoDisplay_fwctrlBufidx_close(void);
#endif // _PHOTODISPLAYTSK_H