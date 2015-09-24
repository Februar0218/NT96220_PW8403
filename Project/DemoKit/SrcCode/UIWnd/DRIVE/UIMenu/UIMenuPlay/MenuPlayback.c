#include <stdio.h>
#include "UIFlow.h"
#include "Utility.h"

#define CUSTOM_PROTECT_MENU     0   // set 1 if you want to use custom menu
#define CUSTOM_SLIDESHOW_MENU   0   // set 1 if you want to use custom menu
#define CUSTOM_ROTATE_MENU      0   // set 1 if you want to use custom menu

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
#if !CUSTOM_PROTECT_MENU
// Protect Options
TMDEF_BEGIN_OPTIONS(PROTECT)
    TMDEF_OPTION_TEXT(PROTECT_ONE)
    TMDEF_OPTION_TEXT(UNPROTECT_ONE)
    TMDEF_OPTION_TEXT(PROTECT_ALL)
    TMDEF_OPTION_TEXT(UNPROTECT_ALL)
TMDEF_END_OPTIONS()
#endif

#if !CUSTOM_SLIDESHOW_MENU
// Slide Show Options
TMDEF_BEGIN_OPTIONS(SLIDE_SHOW)
    TMDEF_OPTION_TEXT(SLIDE_SHOW_2SEC)
    TMDEF_OPTION_TEXT(SLIDE_SHOW_5SEC)
    TMDEF_OPTION_TEXT(SLIDE_SHOW_8SEC)
TMDEF_END_OPTIONS()
#endif

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
static int MenuCustom_Delete(UINT32 uiMessage, UINT32 uiParam)
{
    if (UIPlay_GetData(PLAY_FILENUM) == 0) {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
    } else {
        Ux_OpenWindow(&UIMenuWndPlayDelCtrl, 0);
    }

    return TMF_PROCESSED;
}

#if CUSTOM_PROTECT_MENU
static int MenuCustom_Rotate(UINT32 uiMessage, UINT32 uiParam)
{
    if (UIPlay_GetData(PLAY_FILENUM) == 0)
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
    else
        Ux_OpenWindow(&UIMenuWndPlayRotateCtrl, 0);
}
#endif

#if CUSTOM_PROTECT_MENU
static int MenuCustom_Protect(UINT32 uiMessage, UINT32 uiParam)
{
    if (UIPlay_GetData(PLAY_FILENUM) == 0)
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
    else
        Ux_OpenWindow(&UIMenuWndPlayProtectCtrl, 0);

    return TMF_PROCESSED;
}
#endif

#if CUSTOM_SLIDESHOW_MENU
static int MenuCustom_SlideShow(UINT32 uiMessage, UINT32 uiParam)
{
    if (UIPlay_GetData(PLAY_FILENUM) == 0)
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
    else
        Ux_OpenWindow(&UIMenuWndPlaySlideShowCtrl, 0);

    return TMF_PROCESSED;
}
#endif

static int MenuCustom_DPOF(UINT32 uiMessage, UINT32 uiParam)
{
    if (UIPlay_GetData(PLAY_FILENUM) == 0)
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
    else if (UIPlay_GetData(PLAY_CURRMODE) != PLAYMODE_PRIMARY)
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NOT_JPEG, FLOWWRNMSG_TIMER_1SEC);
    else
        Ux_OpenWindow(&UIMenuWndPlayDPOFCtrl, 0);

    return TMF_PROCESSED;
}

// Playback Menu Items
TMDEF_BEGIN_ITEMS(PLAYBACK)
    TMDEF_ITEM_CUSTOM(DELETE, MenuCustom_Delete)
    #if CUSTOM_PROTECT_MENU
    TMDEF_ITEM_CUSTOM(PROTECT, MenuCustom_Protect)
    #else
    TMDEF_ITEM_TEXTID(PROTECT)
    #endif
    /*#if CUSTOM_SLIDESHOW_MENU
    TMDEF_ITEM_CUSTOM(SLIDE_SHOW, MenuCustom_SlideShow)
    #else
    TMDEF_ITEM_TEXTID(SLIDE_SHOW)
    #endif*/
//    TMDEF_ITEM_CUSTOM(DPOF, MenuCustom_DPOF)
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Playback Menu Pages
TMDEF_BEGIN_PAGES(PLAYBACK)
    TMDEF_PAGE_TEXT_ICON(PLAYBACK)
    TMDEF_PAGE_TEXT_ICON(SETUP)
TMDEF_END_PAGES()

TMDEF_EMNU(gPlaybackMenu, PLAYBACK, Playback_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Playback_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
    UINT16  uwItemId;
    UINT16  uwOption;

    uwItemId = LO_WORD(uiParam);
    uwOption = HI_WORD(uiParam);

    if (uiMessage == TMM_CONFIRM_OPTION)
    {
        switch (uwItemId)
        {
        #if !CUSTOM_PROTECT_MENU
        case IDM_PROTECT:
            SysSetFlag(FL_PROTECT, PROTECT_ONE); // Reset protect menu item

            if (UI_Validate_Storage(STORAGE_CHECK_LOCKED, NULL) == FALSE)
            {
                Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_CARD_LOCKED, FLOWWRNMSG_TIMER_1SEC);
                return TMF_PROCESSED;
            }

            Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_EXE_PROTECT, 1, uwOption);

            if (uwOption == PROTECT_ALL || uwOption == UNPROTECT_ALL)
            {
                Ux_OpenWindow(&UIFlowWndWaitMomentCtrl, 1, FLOWWAITMOMENT_MSG_PLEASEWAIT);
            }
            break;
        #endif

        #if !CUSTOM_SLIDESHOW_MENU
        case IDM_SLIDE_SHOW:
            {
                UINT32  uiTime = 2000;  // default 2 sec

                if (uwOption == SLIDE_SHOW_5SEC)
                {
                    uiTime = 5000;
                }
                else if (uwOption == SLIDE_SHOW_8SEC)
                {
                    uiTime = 8000;
                }
                if (GetCardStatus() != CARD_REMOVED)
                    Ux_OpenWindow(&UIMenuWndPlaySlideShowCBCtrl, 1, uiTime);
            }
            break;
        #endif

        default:
            Setup_MenuCallback(uiMessage, uiParam);
            break;
        }
    }
    else if (uiMessage == TMM_ENTER_OPTION)
    {
        switch (uwItemId)
        {
        #if !CUSTOM_PROTECT_MENU
        case IDM_PROTECT:
            if (UIPlay_GetData(PLAY_FILENUM) == 0)
            {
                Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
                return TMF_PASS_MESSAGE;
            }
            break;
        #endif

        #if !CUSTOM_SLIDESHOW_MENU
        case IDM_SLIDE_SHOW:
            if (UIPlay_GetData(PLAY_FILENUM) == 0)
            {
                Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
            }
            break;
        #endif

        default:
            break;
        }
    }

    return TMF_PROCESSED;
}
