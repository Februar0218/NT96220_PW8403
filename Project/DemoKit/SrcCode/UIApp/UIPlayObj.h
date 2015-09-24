#ifndef UI_PLAYOBJ_H
#define UI_PLAYOBJ_H

#include "UIFramework.h"
#include "PlaybackTsk.h"
#include "NVTUserCommand.h"

typedef enum
{
    PLAY_PBSTATUS,
    PLAY_CURRMODE,
    //#NT#2011/06/08#KS Hung -begin
    // add get current file format command
    PLAY_FILEINDEX,
    PLAY_FILESEQID,
    PLAY_FILEFORMAT,
    PLAY_EXIF_ORI,
    //#NT#2011/06/08#KS Hung -end
    PLAY_LOCKSTATUS,
    PLAY_DPOFQTY,
    PLAY_DPOFDATESTATUS,
    PLAY_DIRID,
    PLAY_FILEID,
    PLAY_FILENUM,
    PLAY_SIZEINDEX,
    PLAY_QUALITY,
    PLAY_ISO,
    PLAY_ZOOMINDEX,
    PLAY_IMGWIDTH,
    PLAY_IMGHEIGHT,
    PLAY_IMGWIDTH_ORI,
    PLAY_IMGHEIGHT_ORI,
    PLAY_PANSRCX,
    PLAY_PANSRCY,
    PLAY_PANMAXX,
    PLAY_PANMAXY,
    PLAY_ZOOMTABLE,
    PLAY_SLIDERTIME,
    //#NT#2009/11/17#SA4(Hideo Lin) -begin
    //#NT#For social network data
    PLAY_USERDATA,
    PLAY_SOCIALNETWORK,
    //#NT#2009/11/17#SA4(Hideo Lin) -end
    PLAY_MAX
}PLAY_DATA_SET;

typedef enum
{
    PLAY_EV,
    PLAY_DATETIME,
    PLAY_STR_MAX
}PLAY_STR_SET;

typedef void (*MovPlayCb)(UINT32);

typedef struct _THUMBNAIL_CONFIG
{
    UINT16  wThumbPerLine;          // Number of thumbnails per line
    UINT16  wThumbPerColumn;        // Number of thumbnails per Column
    UINT16  wFirstHGap;             // The Horizontal gap size to the first thumbnail
    UINT16  wHGap;                  // The Horizontal gap size between thumbnails
    UINT16  wLastHGap;              // The Horizontal gap size after the last thumbnail
    UINT16  wFirstVGap;             // The Vertical gap size to the first thumbnail
    UINT16  wVGap;                  // The Vertical gap between thumbnails
    UINT16  wLastVGap;              // The Vertical gap size after the last thumbnail
    UINT16  wFrColor;               // Color of select frame
    UINT16  wBgColor;               // Color of background
} THUMBNAIL_CONFIG;

#define FLOWPB_THUMB_MODE_3_3           0
#define FLOWPB_THUMB_MODE_3_2           1
#define FLOWPB_THUMB_MODE_5_5           2
#define FLOWPB_THUMB_MODE               FLOWPB_THUMB_MODE_3_3

#if (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_3)
#define FLOWPB_THUMB_H_NUM2             3
#define FLOWPB_THUMB_V_NUM2             3
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_2)
#define FLOWPB_THUMB_H_NUM2             3
#define FLOWPB_THUMB_V_NUM2             2
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_5_5)
#define FLOWPB_THUMB_H_NUM2             5
#define FLOWPB_THUMB_V_NUM2             5
#endif

//-------------------------------------------------------------------------------------------------
#define UIPLAY_DEBUG_MSG    0
#define UIPLAY_DEBUG_ERR    1
//-------------------------------------------------------------------------------------------------
#if (UIPLAY_DEBUG_MSG & DEBUG_LEVEL)
#define UIPLAY_MSG(msg)     debug_msg msg
#else
#define UIPLAY_MSG(msg)
#endif

#if (UIPLAY_DEBUG_ERR & DEBUG_LEVEL)
#define UIPLAY_ERR(msg)     debug_msg msg
#else
#define UIPLAY_ERR(msg)
#endif
//-------------------------------------------------------------------------------------------------

extern VControl UIPlayObjCtrl;
extern UINT32 UIPlay_GetData(PLAY_DATA_SET attribute);
extern void UIPlay_SetData(PLAY_STR_SET attribute, UINT32 value);
CHAR * UIPlay_GetStrData(PLAY_STR_SET attribute);

#endif
