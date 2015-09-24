/**
    Header file for Playback application

    This file is the header file that define the API and data type for Playback application

    @file       PlaybackTsk.h
    @ingroup    mIAPPPlay
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

/** \addtogroup mIAPPPlay */
//@{

#ifndef _PLAYBACKAPI_H
#define _PLAYBACKAPI_H

#include "Type.h"
#include "JpgDec.h"
#include "OSCommon.h"
#include "AppCommon.h"
#include "SlideEffect.h"
//#NT#2009/07/29#Corey Ke -begin
//#NT#Support index view(view by date)
#include "FileDB.h"
//#NT#2009/07/29#Corey Ke -end
#include "AVILib.h"
#include "ime_lib.h"
#include "MediaReadLib.h"//2011/04/06 Meg Lin

//-----------------------------------------------------
typedef struct _PB_PAN_INFO
{
    UINT16  PanMaxX;            ///<  Pan mode, max X addr
    UINT16  PanMaxY;            ///<  Pan mode, max Y addr
    UINT16  PanSrcStartX;       ///<  Pan mode, current X addr
    UINT16  PanSrcStartY;       ///<  Pan mode, current Y addr
    UINT16  PanSrcWidth;        ///<  Pan mode, current width
    UINT16  PanSrcHeight;       ///<  Pan mode, current height
    UINT16  PanDstStartX;       ///<  Pan mode, moving to X addr
    UINT16  PanDstStartY;       ///<  Pan mode, moving to Y addr
} PB_PAN_INFO, *PPB_PAN_INFO;

//#NT#2010/01/19#Scottie -begin
//#NT#Support image showing callback function
// Callback function for showing image
typedef INT32 (*FPCALLBACK_SHOW)(UINT32, UINT32);
//#NT#2010/01/19#Scottie -end

typedef struct _PLAY_OBJ
{
    UINT32  uiMemoryAddr;
    UINT32  uiMemorySize;
    void    (*CallBackFunc)(void);
//#NT#2010/01/19#Scottie -begin
//#NT#Support image showing callback function
    FPCALLBACK_SHOW fpShowCB_Single;        ///< Callback function for showing image in Single/Event View
    FPCALLBACK_SHOW fpShowCB_Thumb;         ///< Callback function for showing image in Thumb/Highlight Thumb
//#NT#2010/01/19#Scottie -end
    UINT32  uiDeviceWidth;
    UINT32  uiDeviceHeight;
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
    UINT32  ui1stPlayMode;

    UINT16  usPlayFileFormat;
    UINT8   *uiMoveImgSpeedTable;           //speed table
    UINT8   Orientation;                    ///   PLAY_HORIZONTAL / PLAY_VERTICAL
    UINT32  Flags;                          ///   bit 0 : PB_ENABLE_TRANSFROM or not
                                            ///   bit 1 : PB_ENABLE_SHOW_BACKGROUND_IN_THUMB or not
                                            ///   bit 2 : PB_ENABLE_SEARCH_FILE_WITHOUT_DCF or not
    //#NT#2011/08/04#KS Hung -begin
    //#NT#Using flag controls to decode primary or not.
    //#NT#2010/08/11#Chris Chung -begin
    //#NT#add to control decode primary image
    //BOOL    bDecodePimaryImageOnly;         ///   Decode primary image only
    //#NT#2010/08/11#Chris Chung -end
    //#NT#2011/08/04#KS Hung -end
//#NT#2009/07/29#Corey Ke -begin
//#NT#Support index view(view by date)
    FILEDB_HANDLE       FileHandle;         ///   File handle to communicate with file DB
//#NT#2009/07/29#Corey Ke -end
    UINT8   uiThumbShowImageMode;           ///   PB_SHOW_THUMB_IMAGE_ONE_BY_ONE / PB_SHOW_THUMB_IMAGE_IN_THE_SAME_TIME
    UINT32  uiPanelWidth;                   ///   Real panel width after panel scaling
    UINT32  uiPanelHeight;                  ///   Real panel height after panel scaling
    BOOL    bEnableAutoRotate;              ///   TRUE: Auto-rotate image according to EXIF-Orientation
    RECT    ThumbOnlyRect;                  ///   Show thumb only jpeg in specific area
    BOOL    bDisplayLastImg;                ///   TRUE: Auto-display the last image after starting Playback task
    UINT32  uiMovieBGAddrY;                 ///< image Y buffer address for movie background
    UINT32  uiMovieBGAddrCb;                ///< image Cb buffer address for movie background
    UINT32  uiMovieBGAddrCr;                ///< image Cr buffer address for movie background
//#NT#2009/09/04#Scottie -begin
//#NT#Add for drawing the background image of thumbnail mode
    UINT32  uiThumbBGW;                     ///< image width for Thumb background
    UINT32  uiThumbBGH;                     ///< image height for Thumb background
    UINT32  uiThumbBGAddrY;                 ///< image Y buffer address for Thumb background
    UINT32  uiThumbBGAddrU;                 ///< image U buffer address for Thumb background
    UINT32  uiThumbBGAddrV;                 ///< image V buffer address for Thumb background
//#NT#2009/09/04#Scottie -end
//#NT#2009/08/18#Scottie -begin
//#NT#Add for drawing the background image of date tag in index view
    UINT32  uiDateTagBGW;                   ///< image width for DateTag background
    UINT32  uiDateTagBGH;                   ///< image height for DateTag background
    UINT32  uiDateTagBGAddrY;               ///< image Y buffer address for DateTag background
    UINT32  uiDateTagBGAddrU;               ///< image U buffer address for DateTag background
    UINT32  uiDateTagBGAddrV;               ///< image V buffer address for DateTag background
//#NT#2009/08/18#Scottie -end
    PPB_PAN_INFO    pstPanInfo;             ///< pointer for Playback Pan info object
    UINT32  uiBadFileBGAddrY;               ///< image Y buffer address for bad file background
    UINT32  uiBadFileBGAddrCb;              ///< image Cb buffer address for bad file background
    UINT32  uiBadFileBGAddrCr;              ///< image Cr buffer address for bad file background
    UINT32  uiWavFileBGAddrY;               ///< image Y buffer address for wav file background
    UINT32  uiWavFileBGAddrCb;              ///< image Cb buffer address for wav file background
    UINT32  uiWavFileBGAddrCr;              ///< image Cr buffer address for wav file background
    UINT32  uiMemFB2Addr;                   ///< memory pool FB2 buffer address
    void    (*Decode1stMediaFunc)(MEDIA_PLAY_1ST_INFO *pMediaInfo);//2011/04/06 Meg Lin
} PLAY_OBJ, *PPLAY_OBJ;

// Structure for play single image command object
typedef struct
{
    UINT8   PlayCommand;            // PB_SINGLE_CURR / PB_SINGLE_NEXT / PB_SINGLE_PREV / PB_SINGLE_PRIMARY
    UINT8   MoveImgMode;            // Move window effect, refer to PLAY_MOVWIN_XXXX
    UINT8   JumpOffset;             // The JumpOffset of read next / previous file
    INT32   (*fpSpecEffect)(INT32); // Callback function for slide-effect
} PLAY_SINGLE_OBJ, *PPLAY_SINGLE_OBJ;

// Structure for browser (thumbnail) mode command object
typedef struct
{
    UINT8   BrowserCommand;         // PB_BROWSER_CURR / PB_BROWSER_NEXT / PB_BROWSER_PREV
    UINT8   HorNums;                // How many images in horizontal direction, HorNums * VerNums must <=25
    UINT8   VerNums;                // How many images in vertical direction, HorNums * VerNums must <=25
    UINT8   MoveImgMode;            // Move window effect, refer to SLIDE_EFFECT_XXXX (SlideEffect.h)
    UINT8   JumpOffset;             // The JumpOffset of read next / previous file
    BOOL    bReDecodeImages;        // Used to re-decode all thumb images or not, TRUE / FALSE
                                    // if JumpOffset is still in the same page, set it as FALSE,
                                    // otherwise it means playback has to decode all images in another page, set it as TRUE
    INT32   (*fpSpecEffect)(INT32); // Callback function for slide-effect
} PLAY_BROWSER_OBJ, *PPLAY_BROWSER_OBJ;

// Structure for show specific file in video1/video2 and FB buf/tmpFB and spec size
typedef struct
{
    UINT8   PlayFileBuf;            // PLAY_SPEC_FILE_IN_FRAME_BUFFER / PLAY_SPEC_FILE_IN_TMP_FRAME_BUFFER
    UINT8   PlayFileVideo;          // PLAY_SPEC_FILE_IN_VIDEO_1 / PLAY_SPEC_FILE_IN_VIDEO_2
    UINT8   PlayFileClearBuf;       // PLAY_SPEC_FILE_WITH_CLEAR_BUF / PLAY_SPEC_FILE_WITH_NOT_CLEAR_BUF
    RECT    PlayRect;               // Pointer of the rect-array of file
    BOOL    bDisplayWoReDec;        // TRUE / FALSE, for accelerating 2nd same spec file
} PLAY_SPECFILE_OBJ, *PPLAY_SPECFILE_OBJ;

//#NT#2012/02/15#KS Hung -begin
//#NT#Using JPEG crop preview replaces the UserZoomSetting.
typedef struct
{
    short  cropOSDXoff;    // real crop area x offset
    short  cropOSDYoff;    // real crop area y offset
    short  cropOSDXsize;   // real crop area width
    short  cropOSDYsize;   // real crop area height
    short  OrigOSDXstart;  // x start position in the cropped area
    short  OrigOSDYstart;  // y start position in the cropped area
    short  OrigOSDXmax;    // max position in x-coordination
    short  OrigOSDYmax;    // max position in y-coordination
    BOOL   bCropPreviewFlag;
}PLAY_CROPPREVIEW_OBJ, *PPLAY_CROPPREVIEW_OBJ;
//#NT#2012/02/15#KS Hung -end

typedef struct
{
    UINT32  DisplayStart_X;         // crop display start x
    UINT32  DisplayStart_Y;         // crop display start y
    UINT32  DisplayWidth;           // crop display width
    UINT32  DisplayHeight;          // crop display height
    BOOL    IfOverWrite;            // TRUE / FALSE, use to overwrite or not
    BOOL    IfDisplayImg;           // TRUE / FALSE, use to display or not
    BOOL    IfEncSNail;             // TRUE / FALSE, use to encode screennail or not
    FP      fpMakerNoteCB;          // Callback function to fill new EXIF data
//#NT#2010/04/19#Scottie -begin
//#NT#Add, support resize function for trimming image
    BOOL    bIfResize;              // TRUE / FALSE, use to resize or not
    UINT32  uiRSZWidth;             // Reszie width
    UINT32  uiRSZHeight;            // Reszie height
//#NT#2010/04/19#Scottie -end
} PLAY_CROPSAVE_OBJ, *PPLAY_CROPSAVE_OBJ;

typedef struct
{
    UINT16   FileId;                // Copy file Id, only apply when PLAY_COPY_SPECIFIC_FILE
    UINT16   DirId;                 // Copy dir Id, only apply when PLAY_COPY_SPECIFIC_FILE
    UINT16   FileFormat;            // Copy file format, only apply when PLAY_COPY_SPECIFIC_FILE, PLAYFMT_JPG / PLAYFMT_AVI / PLAYFMT_ASF / PLAYFMT_WAV
    UINT8    CopyToDist;            // PLAY_COPY_TO_CARD / PLAY_COPY_TO_NAMD
    UINT8    CopyMode;              // PB_COPY2CARD_NEW_DIR / PB_COPY2CARD_APPEND / PB_COPY2CARD_NO_RENAME / PB_COPY2CARD_NEW_DIR_ORG_FILE
    UINT8    CopyFiles;             // PLAY_COPY_ALL_FILES / PLAY_COPY_SPECIFIC_FILE
} PLAY_COPYTO_OBJ, *PPLAY_COPYTO_OBJ;

typedef void (*SliderCb)(void);

//-------------------------------------------------------------------------


/**
    @name Playback file format
*/
//@{
#define PLAYFMT_UNKNOWN             0x0000  ///< File format: Unknown
#define PLAYFMT_JPG                 0x0001  ///< File format: JPEG
#define PLAYFMT_WAV                 0x0002  ///< File format: WAV
#define PLAYFMT_AVI                 0x0004  ///< File format: AVI
#define PLAYFMT_ASF                 0x0008  ///< File format: ASF
#define PLAYFMT_RAW                 0x0010  ///< File format: RAW
#define PLAYFMT_TIF                 0x0020  ///< File format: TIF
#define PLAYFMT_JPGMEMO             0x0010  ///< File format: JPEG with Memo
#define PLAYFMT_DPOF                0x0020  ///< File format: DPOF
#define PLAYFMT_READONLY            0x0040  ///< File format: Read only
#define PLAYFMT_MPG                 0x0080  ///< File format: MPEG
#define PLAYFMT_MOVMJPG             0x0100  ///< File format: MOV with motion JPEG
//#NT#2009/07/29#Corey Ke -begin
//#NT#Support index view(view by date)
#define PLAYFMT_EMPTY               0x0200  ///< File format: Empty (for play by date)
//#NT#2009/07/29#Corey Ke -end
//@}

/**
    @name Playback wait time
*/
//@{
#define PB_WAIT_NOWAIT              0x00    ///< Wait time: No wait
#define PB_WAIT_INFINITE            0xFF    ///< Wait time: Wait infinite
//@}

/**
    @name Playback image rotate
*/
//@{
#define PLAY_ROTATE_DIR_UNKNOWN     0xFF    ///< Rotate direction: Unknown
#define PLAY_ROTATE_DIR_0           PLAY_ROTATE_DIR_UNKNOWN
#define PLAY_ROTATE_DIR_90          0x00    ///< Rotate direction: 90 deg
#define PLAY_ROTATE_DIR_180         0x01    ///< Rotate direction: 180 deg
#define PLAY_ROTATE_DIR_270         0x02    ///< Rotate direction: 270 deg
#define PLAY_ROTATE_DIR_HOR         0x03    ///< Rotate direction: Horizontal
#define PLAY_ROTATE_DIR_VER         0x04    ///< Rotate direction: Vertical
//@}

/**
    @name Playback DPOF command
*/
//@{
#define PLAYDPOF_OPEN               0x00000001  ///< DPOF: Open
#define PLAYDPOF_SETONE             0x00000002  ///< DPOF: Set one
#define PLAYDPOF_SETALL             0x00000004  ///< DPOF: Set all
#define PLAYDPOF_RESETALL           0x00000008  ///< DPOF: Reset all
#define PLAYDPOF_CLOSE              0x00000010  ///< DPOF: Close
#define PLAYDPOF_GETONEQTY          0x00000020  ///< DPOF: Get one qty
#define PLAYDPOF_GETONEDATE         0x00000040  ///< DPOF: Get one date
#define PLAYDPOF_GETALL             0x00000080  ///< DPOF: Get all
#define PLAYDPOF_SETONE_DATEONLY    0x00000100  ///< DPOF: Set one date only
#define PLAYDPOF_SETONE_QTYONLY     0x00000200  ///< DPOF: Set one qty only
#define PLAYDPOF_SETREV             0x00000400  ///< DPOF: Set revision
#define PLAYDPOF_SETCRT             0x00000800  ///< DPOF: Set creator
#define PLAYDPOF_UPDATE             0x00001000  ///< DPOF: Update
#define PLAYDPOF_RELOAD             0x00002000  ///< DPOF: Reload
//@}

//#NT#2010/03/31#Hideo Lin -begin
//#NT#Added for user data (ex. social network tag, favorite tag)
/**
    @name Playback user data command
*/
//@{
#define PLAY_USERDATA_SOCIALNETWORK     0x01    ///< User data: social network
#define PLAY_USERDATA_FAVORITE          0x02    ///< User data: favorite

#define PLAY_SOCIALNETWORK_SETONE       0x01    ///< Social network: set current image/movie
#define PLAY_SOCIALNETWORK_RESETONE     0x02    ///< Social network: reset current image/movie
#define PLAY_SOCIALNETWORK_SETMULTI     0x04    ///< Social network: set selected images/movies
#define PLAY_SOCIALNETWORK_RESETMULTI   0x08    ///< Social network: reset selected images/movies
#define PLAY_SOCIALNETWORK_SETALL       0x10    ///< Social network: set all images/movies
#define PLAY_SOCIALNETWORK_RESETALL     0x20    ///< Social network: reset all images/movies

#define PLAY_FAVORITE_ONE               0x01    ///< Favorite: set current image/movie
#define PLAY_FAVORITE_MULTI             0x02    ///< Favorite: set all images/movies
#define PLAY_UNFAVORITE_ONE             0x04    ///< Favorite: reset current image/movie
#define PLAY_UNFAVORITE_MULTI           0x08    ///< Favorite: reset all images/movies
//@}
//#NT#2010/03/31#Hideo Lin -end

/**
    @name Playback protect file
*/
//@{
#define PLAY_PROTECT_ONE            0x01    ///< Protect one
#define PLAY_PROTECT_ALL            0x02    ///< Protect all
#define PLAY_UNPROTECT_ONE          0x04    ///< Unprotect one
#define PLAY_UNPROTECT_ALL          0x08    ///< Unprotect all
//@}

/**
    @name Playback format storage
*/
//@{
#define PLAY_FORMAT_CARD            0x01    ///< Format card
#define PLAY_FORMAT_NAND            0x02    ///< Format NAND
//@}

/**
    @name Playback photo display orientation
*/
//@{
#define PLAY_HORIZONTAL             0x01    ///< Horizontal
#define PLAY_VERTICAL               0x02    ///< Vertical
//@}

/**
    @name Playback display mode
*/
//@{
#define PLAY_SINGLE                 0x01    ///< Single view mode
#define PLAY_SLIDE_SHOW             0x02    ///< Slide show mode
//@}

/**
    @name Playback function enable flag
*/
//@{
#define PB_ENABLE_TRANSFROM                 0x00000001  ///< Transform
#define PB_ENABLE_SHOW_BACKGROUND_IN_THUMB  0x00000002  ///< Show background in thumbnail mode
#define PB_ENABLE_SEARCH_FILE_WITHOUT_DCF   0x00000004  ///< Search file without DCF
#define PB_ENABLE_SHOW_BG_IN_MOVIE          0x00000008  ///< Show background in Movie file
#define PB_ENABLE_SHOW_BG_IN_BADFILE        0x00000010  ///< Show background in Bad file
#define PB_ENABLE_PLAY_FILE_BY_DATE         0x00000020  ///< Play file by date
#define PB_ENABLE_THUMB_WITH_DIFF_FOLDER    0x00000040  ///< Thumbnail with diff folder
    //@note This flag should not co-exist with PB_ENABLE_PLAY_FILE_BY_DATE
#define PB_ENABLE_SHOW_BG_IN_WAVFILE        0x00000080  ///< Show background in WAV file
//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
#define PB_ENABLE_SLIDE_SHOW_IN_SMALL_BUF   0x00000100  ///< Slide show in small buffer
//#NT#2009/06/03#Corey Ke -end
#define PB_ENABLE_SPEEDUP_SCREENNAIL        0x00000200  ///< Speed up screennail display
//#NT#2010/04/28#Scottie -begin
//#NT#Support drawing MOV background in thumbnail mode enable opiton
#define PB_ENABLE_SHOW_MOVBG_IN_THUMB       0x00000400  ///< Show MOV background in thumbnail mode
//#NT#2010/04/28#Scottie -end
//#NT#2011/08/04#KS Hung -begin
//#NT#Add decode primary without hidden mechanism
#define PB_ENABLE_FORCE_TO_DECODE_PRIMARY       0x00000800
//#NT#2011/08/04#KS Hung -end
//@}

/**
    @name Playback thumbnail mode showing method
*/
//@{
#define PB_SHOW_THUMB_IMAGE_ONE_BY_ONE          0x00000001  ///< Show thumbnail image one bye one
#define PB_SHOW_THUMB_IMAGE_IN_THE_SAME_TIME    0x00000002  ///< Show thumbnail image in the same time
//@}

/**
    @name Playback copy to card mode
*/
//@{
#define PB_COPY2CARD_NEW_DIR            0x00  ///< Copy to latest empty dir whose dir name is equal to initialized name, else copy to new dir
#define PB_COPY2CARD_APPEND             0x01  ///< Append to existing dir and file index from maximum existing file id plus by 1
#define PB_COPY2CARD_NO_RENAME          0x02  ///< Copy to latest dir whose dir name is equal to initialized name and first source file id is greater than destination maximum file id, else copy to new dir
#define PB_COPY2CARD_NEW_DIR_ORG_FILE   0x03  ///< The same as FST_COPYTO_NEW_DIR, but keep original filename
//@}

/**
    @name Playback task status
*/
//@{
#define PB_STA_UNKNOWN              0x00000000  ///< Unknow
#define PB_STA_NOIMAGE              0x00000001  ///< No image
#define PB_STA_ERR_FILE             0x00000002  ///< Error file
#define PB_STA_ERR_DECODE           0x00000004  ///< Decode error
#define PB_STA_ERR_WRITE            0x00000008  ///< Write error
#define PB_STA_DONE                 0x00000010  ///< Done
#define PB_STA_BUSY                 0x00000020  ///< Busy
#define PB_STA_STANDBY              0x00000040  ///< Standby
#define PB_STA_INITFAIL             0x00000080  ///< Init fail
//@}

/**
    @name Playback single view command param.
*/
//@{
#define PB_SINGLE_CURR              0x01    ///< Play current image
#define PB_SINGLE_NEXT              0x02    ///< Play next image
#define PB_SINGLE_PREV              0x04    ///< Play previous image
#define PB_SINGLE_PRIMARY           0x08    ///< Play screennail image, or primary image
#define PB_SINGLE_SLIDE_START       0x10    ///< Start slide show
#define PB_SINGLE_SLIDE_STOP        0x20    ///< Stop slide show
#define PB_SINGLE_THUMB             0x40    ///< Play thumbnail image only
#define PB_SINGLE_NO_HIDDEN         0x80    ///< Play primary image only
    //@note To decode primary only, or return decode error
//@}

/**
    @name Playback thumbnail mode command param.
*/
//@{
#define PB_BROWSER_CURR             0x01    ///< Browser current image
#define PB_BROWSER_NEXT             0x02    ///< Browser next image
#define PB_BROWSER_PREV             0x04    ///< Browser previous image
// To decode thumbnail only, or return decode error
#define PB_BROWSER_THUMB            0x40    ///< Browser thumbnail image only
//#NT#2009/11/25#Scottie -begin
//#NT#Add for entering/exiting Event View wo slide effect
#define PB_BROWSER_WO_IOEFFECT      0x80    ///< Browser image wo entering/exiting slide effect
//#NT#2009/11/25#Scottie -end
//@}

/**
    @name Playback delete image command
*/
//@{
#define PB_DELETE_ONE               0x01    ///< Delete one image
#define PB_DELETE_ALL               0x02    ///< Delete all images
//@}

/**
    @name Playback zoom image command
*/
//@{
#define PLAYZOOM_IN                 0x01    ///< Zoom in
#define PLAYZOOM_OUT                0x02    ///< Zoom out
#define PLAYZOOM_UP                 0x04    ///< Zoom pan up
#define PLAYZOOM_DOWN               0x08    ///< Zoom pan down
#define PLAYZOOM_LEFT               0x10    ///< Zoom pan left
#define PLAYZOOM_RIGHT              0x20    ///< Zoom pan right
#define PLAYZOOM_USER               0x40    ///< User zoom
//@}

/**
    @name Playback picture bridge mode command
*/
//@{
#define PB_PICBRIDGE_ON             0x55    ///< Picture bridge on
#define PB_PICBRIDGE_OFF            0x00    ///< Picture bridge off
//@}

/**
    @name Playback mode
*/
//@{
#define PLAYMODE_THUMB              9       ///< Thumbnail mode
#define PLAYMODE_PRIMARY            10      ///< Single view mode: JPEG
#define PLAYMODE_AVI                11      ///< Single view mode: AVI
#define PLAYMODE_ROTATE             12      ///< Rotae display
#define PLAYMODE_WAV                13      ///< Single view mode: WAV
#define PLAYMODE_ASF                14      ///< Single view mode: ASF
#define PLAYMODE_SLIDESHOW          15      ///< Slide show mode
#define PLAYMODE_MPG                16      ///< Single view mode: MPEG
#define PLAYMODE_MOVMJPG            17      ///< Single view mode: Motion JPEG
#define PLAYMODE_ONLYTHUMB          33      ///< First decode always decode only thumbnail
#define PLAYMODE_PICBRIDGE          55      ///< Picture bridge mode
#define PLAYMODE_QVIEW              66      ///< Show first image from SDRAM Q-view buffer
#define PLAYMODE_USERJPGOUT         77      ///< User JPEG mode

#define PLAYMODE_UNKNOWN            99      ///< First decode the max-file-id file
//@}

/**
    @name Playback file info
*/
//@{
#define PLAYFILEINFO_FILEID             1   ///< File ID
#define PLAYFILEINFO_FILESIZE           2   ///< File size
#define PLAYFILEINFO_FILEFORMAT         3   ///< File format
#define PLAYFILEINFO_FILESEQ            4   ///< File sequence ID
#define PLAYFILEINFO_FILENUMS           5   ///< Total file number
#define PLAYFILEINFO_DIRNUMS            6   ///< Total folder number
#define PLAYFILEINFO_MAXDIRID           7   ///< Maximun directory ID
#define PLAYFILEINFO_DIRID              8   ///< Directory ID
#define PLAYFILEINFO_FILENUMS_INDIR     9   ///< Total file number in current directory
#define PLAYFILEINFO_FILESEQ_INDIR      10  ///< File sequence ID in current directory
#define PLAYFILEINFO_MAXFILEID_INDIR    11  ///< Maximun file ID in current directory
#define PLAYFILEINFO_ISFILEREADONLY     12  ///< Read only
#define PLAYFILEINFO_ISWITHMENO         13  ///< Vioce memo
//@}

/**
    @name Playback user JPEG display buffer
*/
//@{
#define PLAY_USER_JPEG_IN_FRAME_BUFFER      1   ///< Display in FB buffer
#define PLAY_USER_JPEG_IN_TMP_FRAME_BUFFER  2   ///< Display in TmpFB buffer
//@}

/**
    @name Playback show specific file in FB/TmpFB buffer
*/
//@{
#define PLAY_SPEC_FILE_IN_FRAME_BUFFER      0   ///< Display in FB buffer
#define PLAY_SPEC_FILE_IN_TMP_FRAME_BUFFER  1   ///< Display in TmpFB buffer
//@}

/**
    @name Playback show specific file in video1/video2
*/
//@{
#define PLAY_SPEC_FILE_IN_VIDEO_1   0       ///< Display in video1
#define PLAY_SPEC_FILE_IN_VIDEO_2   1       ///< Display in video2
//@}

/**
    @name Playback show specific file with/wo clear buffer
*/
//@{
#define PLAY_SPEC_FILE_WITH_CLEAR_BUF       0   ///< Clear buffer
#define PLAY_SPEC_FILE_WITH_NOT_CLEAR_BUF   1   ///< Not clear buffer
//@}

/**
    @name Playback copy to storage
*/
//@{
#define PLAY_COPY_TO_CARD           0       ///< Copy to card
#define PLAY_COPY_TO_NAND           1       ///< Copy to NAND
//@}

/**
    @name Playback copy file param.
*/
//@{
#define PLAY_COPY_ALL_FILES         0       ///< Copy all files
#define PLAY_COPY_SPECIFIC_FILE     1       ///< Copy specific file
//@}

/**
    @name Playback perfect fix function
*/
//@{
#define PB_PERFECT_FIX_ALL          0x01    ///< All (Nova light + Redeye removal)
#define PB_PERFECT_FIX_LIGHTING     0x02    ///< Nova light
#define PB_PERFECT_FIX_REDEYE       0x03    ///< Redeye removal
//@}

/**
    @name Playback scale for panel display ratio v.s. panel real view ratio
*/
//@{
#define PB_PANEL_NO_SCALING         0       ///< No scale (Panel real view ratio is equal to panel display ratio)
//@}

/**
    @name Playback thumbnail only rect
*/
//@{
#define PB_THUMB_ONLY_DEFAULT_SETTING   0   ///< Default setting, full screen
//@}

/**
    @name Playback image ratio related thresholds
*/
//@{
//#NT#2012/08/08#KS Hung -begin
//#define PB_NORMALIZE_IMGRATIO_THRESHOLD     5   ///< Normalization threshold
#define PB_NORMALIZE_IMGRATIO_THRESHOLD     6   ///< Normalization threshold
//#NT#2012/08/08#KS Hung -end
#define PB_TRANSFORM_THRESHOLD              100 ///< Transform base
//@}

/**
    JPEG Exif-Orientation Tag
*/
typedef enum {
    PLAY_EXIF_ORI_DEFAULT         = 1,  ///<1, Default orientation
    PLAY_EXIF_ORI_FLIP_HORIZONTAL = 2,  ///<2, flip horizontal
    PLAY_EXIF_ORI_ROTATE_180      = 3,  ///<3, rotate 180 deg
    PLAY_EXIF_ORI_FLIP_VERTICAL   = 4,  ///<4, flip vertical
    PLAY_EXIF_ORI_TRANSPOSE       = 5,  ///<5, transpose
    PLAY_EXIF_ORI_ROTATE_90       = 6,  ///<6, rotate 90 deg
    PLAY_EXIF_ORI_TRANSVERSE      = 7,  ///<7, transverse
    PLAY_EXIF_ORI_ROTATE_270      = 8   ///<8, rotate 270 deg
} PB_JPEG_EXIF_ORI;

//#NT#2009/07/29#Scottie -begin
/**
    Event View - Speed level
*/
typedef enum {
//#NT#2009/12/15#Scottie -begin
//#NT#Add to pause event view
    EV_SPEED_PAUSE  = 0,   ///<0, Pause
//#NT#2009/12/15#Scottie -end
    EV_SPEED_LOW    = 1,   ///<1, Low speed
    EV_SPEED_MEDIUM = 2,   ///<2, Medium speed
    EV_SPEED_HIGH   = 3,   ///<3, High speed
    EV_SPEED_XHIGH  = 4    ///<4, X-High speed
} PB_EV_SPEEDLVL;
//#NT#2009/07/29#Scottie -end

//#NT#2009/09/17#Niven Cho -begin
//#Added., Shadow Show Support to Small Slide Effect
/**
    Single shadow command
*/
typedef enum {
    PB_SINGLE_SHADOW_CMD_OPEN,                      ///<0, Open
    PB_SINGLE_SHADOW_CMD_CLOSE,                     ///<1, Close
    PB_SINGLE_SHADOW_CMD_REFRESH,                   ///<2, Refresh
    PB_SINGLE_SHADOW_CMD_FADING_WITH_SCALE_DOWN,    ///<3, Fading with scale-down
    PB_SINGLE_SHADOW_CMD_FADING_BLOCK_CHECKER,      ///<4, Fading with block-checker
    PB_SINGLE_SHADOW_CMD_PREFECT_FIX,               ///<5, Perfect fix
    PB_SINGLE_SHADOW_CMD_BLEND_LARGE_SIZE,          ///<6, Blend large size
    //#NT#2009/12/02#Niven Cho -begin
    //#NT#Added., Fix Event View->Shadow View has opening animation
    PB_SINGLE_SHADOW_CMD_OPEN_WITHOUT_ANIMATION,    ///<7  Open without Open Animation
    //#NT#2009/12/02#Niven Cho -end
    //#NT#2009/12/04#Niven Cho -begin
    //#NT#Added., Fix Shadow view -> zoom in has closing animation
    PB_SINGLE_SHADOW_CMD_CLOSE_WITHOUT_ANIMATION,    ///<7  Open without Open Animation
    //#NT#2009/12/04#Niven Cho -end
    ENUM_DUMMY4WORD(PB_SINGLE_SHADOW_CMD_RESERVED)
}PB_SINGLE_SHADOW_CMD;
//#NT#2009/09/17#Niven Cho -end

//#NT#2010/01/15#Scottie -begin
//#NT#Only display the image that image ratio is 3:2/4:3/16:9 in Thumbnail mode
/**
    Image ratio - Width vs. Hieght
*/
typedef enum
{
    PB_IMG_RT_3_2,          ///<0,  3:2
    PB_IMG_RT_4_3,          ///<1,  4:3
    PB_IMG_RT_16_9,         ///<2, 16:9
//#NT#2010/04/26#Scottie -begin
//#NT#Add, support 2:1 & 1:1 image ratio
    PB_IMG_RT_2_1,          ///<3,  2:1
    PB_IMG_RT_1_1,          ///<4,  1:1
//#NT#2010/04/26#Scottie -end
    PB_IMG_RT_RESERVED      ///<5, Reserved
}PB_IMG_RATIO;
//#NT#2010/01/15#Scottie -end

//#NT#2010/01/19#Scottie -begin
/**
    Image checking result: Display or not, Displayed image type
*/
typedef enum
{
    PB_IMGSHOW_NG,          ///<0, Don't dispaly this image
    PB_IMGSHOW_OK,          ///<1, Dispaly this image
    PB_IMGSHOW_THUMB        ///<2, Dispaly Thumb image
}PB_IMG_SHOW;
//#NT#2010/01/19#Scottie -end

//-----------------------------------------------------------------------------------
extern INT32    PB_Open(PPLAY_OBJ pPlayObj);
extern INT32    PB_Close(UINT32 TimeOut);
extern UINT32   PB_GetStatus(void);
//----- play image op command -------------------------------------------------------------------
extern void     PB_PlaySingleMode(PPLAY_SINGLE_OBJ pPlaySingleObj);
extern void     PB_SetAutoLoadTerminate(void);
extern UINT32   PB_PlayBrowserMode(PPLAY_BROWSER_OBJ pPlayBrowserObj);
extern void     PB_PlayZoomMode(UINT32 PlayCommand);
extern UINT32   PB_PlayZoomPanMode(UINT32 PlayCommand, UINT32 MoveOffset);
extern UINT32   PB_PlayZoomUserSetting(UINT32 LeftUp_X, UINT32 LeftUp_Y, UINT32 RightDown_X, UINT32 RightDown_Y);
extern void     PB_PlayDeleteMode(UINT32 PlayCommand);
extern void     PB_PlaySliderModeInit(UINT32 PlayCommand, UINT32 SliderTime, SliderCb);
extern void     PB_PlaySliderMode(UINT32 PlayCommand, UINT32 MoveImgMode);
//#NT#2009/07/29#Corey Ke -begin
//#NT#Support index view(view by date)
//#NT#2009/10/26#Corey Ke -begin
//assign memory from project
extern UINT32   PB_PlayByDateInit(UINT32 poolAddr, UINT32 poolSize);
//#NT#2009/10/26#Corey Ke -end
extern void     PB_PlayByDateUnInit(void);
extern UINT16   PB_GetTotalPlayDay(void);
extern UINT16   PB_GetFirstPlayDay(void);
extern UINT16   PB_GetLastPlayDay(void);
extern UINT16   PB_GetNextPlayDay(void);
extern UINT16   PB_GetPrevPlayDay(void);
extern UINT16   PB_GetCurrPlayDay(void);
extern UINT16   PB_GetCurrPlayDaySeq(void);
extern UINT16   PB_GetCurrPlayYearSeq(void);
extern UINT16   PB_GetCurrPlayDaySeqInThisYear(void);
extern UINT16   PB_GetPlayDaysNuminYear(UINT16 SpecYear);
//#NT#2009/08/21#Corey Ke -begin
//#NT#Add API to get total files in spec date
extern UINT32   PB_GetPlayNuminDate(UINT16 SpecDate);
//#NT#2009/08/21#Corey Ke -end
extern UINT16   PB_GetTotalPlayMonth(void);
extern UINT16   PB_GetFirstPlayMonth(void);
extern UINT16   PB_GetNextPlayMonth(void);
extern UINT16   PB_GetTotalPlayYear(void);
extern UINT16   PB_GetFirstPlayYear(void);
extern UINT16   PB_GetNextPlayYear(void);
extern UINT16   PB_GetTotalFilesNumOfSpecDate(void);
extern void     PB_SetPlayDate(UINT32 DateType, UINT16 Date);
//#NT#2009/07/29#Corey Ke -end
extern void     PB_SetDPOFOpenClose(UINT32 DpofOPMode);
extern void     PB_SetCurrDPOFQty(UINT32 DpofOPMode, UINT32 DpofPrtNum, UINT32 DpofDateOn);
extern void     PB_SetProtectFile(UINT32 ProtectMode);
extern void     PB_SetReSizeFile(UINT32 NewWidth, UINT32 NewHeight, USHORT usJpegQual, UINT32 IfOverWrite);
//#NT#2009/04/06#Scottie -begin
//#Add for terminate resize function
extern void     PB_SetReSizeTerminate(void);
//#NT#2009/04/06#Scottie -end
extern void     PB_SetMyFavoriteFile(UINT32 uiNewQty);
extern void     PB_SetReQtyFile(UINT32 NewQty, UINT32 IfOverWrite);
extern void     PB_SetRotateCurrFile(UINT32 RotateDir, UINT32 IfOverWrite);
extern void     PB_SetEXIF_Orientation(PB_JPEG_EXIF_ORI EXIFOri, BOOL bDecodeIMG);
extern UINT32   PB_SetCopy2Card(UINT8 ucCopyMode);
extern UINT32   PB_SetCopyTo(PPLAY_COPYTO_OBJ pPlayCopyToObj);
extern void     PB_SetIMEIDESize(PIMEIDE_ATTR  pIMEIDEAttr);
extern INT32    PB_WaitCommandFinish(UINT32 TimeOut);
extern INT32    PB_SetFormatStorage(UINT32 StrgType);
extern INT32    PB_SetRotateDisplay(UINT32 RotateDir);
extern INT32    PB_SetReArrangeFileID(void);
extern void     PB_PlayUserJPEG(UINT32 SrcAddr, UINT32 SrcSize, RECT OutRect, UINT8 DstBuf);
extern void     PB_PlaySpecFile(PPLAY_SPECFILE_OBJ pPlayFileObj);
extern void     PB_1stVideoFrameInit(void);
extern void     PB_Set1stVideoFrame(UINT16 uiDstLU_X, UINT16 uiDstLU_Y, UINT16 uiDstSZ_W, UINT16 uiDstSZ_H);
extern void     PB_Set1stVdoFrame4Thumb(UINT16 uiDstLU_X, UINT16 uiDstLU_Y, UINT16 uiDstSZ_W, UINT16 uiDstSZ_H);
extern void     PB_SetCropSave(PPLAY_CROPSAVE_OBJ pPlayCropSaveObj);
extern void     PB_SetCropTerminate(void);
//#NT#2011/12/20#KS Hung -begin
//#NT#Using JPEG crop preview replaces the UserZoomSetting.
extern void     PB_SetCropPreview(PPLAY_CROPPREVIEW_OBJ pPlayCropPreviewObj);
//#NT#2011/12/20#KS Hung -end
//#NT#2009/03/19#Photon Lin -begin
//#Get frame buffer
extern void     PB_GetCurr3IMETempFBAddr(UINT32 *pFBAddrY, UINT32 *pFBAddrCb, UINT32 *pFBAddrCr);
//#NT#2009/03/19#Photon Lin -end
//#NT#2009/04/22#Scottie -begin
//#NT#Support Perfect Fix (All, Redeye removal, Novalight)
extern void     PB_SetPerfectFix(UINT8 ui8FixFunc, BOOL bIfOverWrite, FP fpPIM3CB);
extern void     PB_SetPerfectFixTerminate(void);
extern void     PB_SetPerfectFixDisplay(UINT8 ui8FixFunc, BOOL bIfDisplay, UINT8 ui8NLightLvl);
//#NT#2009/04/22#Scottie -end
//#NT#2010/04/28#Scottie -begin
//#NT#Support drawing MOV background in thumbnail mode enable opiton
extern void     PB_SetMOVBG4Thumb(BOOL bEnable);
//#NT#2010/04/28#Scottie -end
//----- get info. -------------------------------------------------------------------
extern UINT32   PB_GetFILELock(void);
extern UINT32   PB_GetFILEWithMemo(void);
extern UINT32   PB_GetCurrMode(void);
extern UINT32   PB_GetCurFileIndex(void);
extern UINT32   PB_GetCurrZoomIndex(void);
extern UINT32   PB_GetCurFileNameId(void);
extern UINT32   PB_GetCurFileSeqId(void);
extern UINT32   PB_GetCurFileFormat(void);
extern UINT32   PB_GetCurFileDirId(void);
extern UINT32   PB_GetCurThumbNums(void);
extern UINT32   PB_GetSlideInterval(void);
extern PJPGHEAD_DEC_CFG     PB_GetCurrImageInfo(void);
extern PJPGHEAD_DEC_CFG     PB_GetCurrImageInfoBeforeDec(BOOL bParseEXIF);
extern UINT16   *PB_GetAllThumbDirIdArray(void);
extern UINT16   *PB_GetAllThumbFileIdArray(void);
extern UINT16   *PB_GetAllThumbFileFormatArray(void);
//#NT#2009/07/29#Corey Ke -begin
//#NT#Support index view(view by date)
extern UINT16   *PB_GetAllThumbFileDateArray(void);
//#NT#2009/07/29#Corey Ke -end
extern BOOL     *PB_GetAllThumbDecErrArray(void);
extern UINT16   *PB_GetAllThumbSeqIdArray(void);
extern UINT32   PB_GetCurrDPOFQty(void);
extern UINT32   PB_GetCurrDPOFDateOn(void);
extern UINT32   PB_GetCurrDPOFAll(void);
extern UINT32   PB_GetCurrFileSysInfo(UINT32 WhichInfo);
extern PAVIHEAD_INFO    PB_GetCurrAVIInfo(void);
extern PRECT    PB_GetThumbRect(void);
extern UINT32   PB_GetRotateDir(void);
extern PPLAY_OBJ    PB_GetPBObject(void);
//#NT#2009/12/15#Scottie -begin
//#NT#Add to get the speed of event view
extern PB_EV_SPEEDLVL PB_GetEViewSpeed(void);
//#NT#2009/12/15#Scottie -end
//----- for AVI get info ---------------------------------------------
extern UINT32   PB_GetCurrDataBufAddr(void);
extern UINT32   PB_GetCurrDataBufSize(void);
extern void     PB_GetCurr3FBAddr(UINT32 *pFBAddrY, UINT32 *pFBAddrCb, UINT32 *pFBAddrCr);
extern void     PB_GetCurr3TempFBAddr(UINT32 *pFBAddrY, UINT32 *pFBAddrCb, UINT32 *pFBAddrCr);
//----- setting -------------------------------------
extern void     PB_SetZoomLevelTbl(UINT32 *pZoomLevelTbl);
extern void     PB_SetThumbRect(PRECT pRectDrawThumb);
//#NT#2009/09/04#Scottie -begin
//#NT#Add for drawing the background image of thumbnail mode
extern void     PB_SetThumbBG(BOOL bEnable);
//#NT#2009/09/04#Scottie -end
extern void     PB_SetSlideInterval(UINT32 SlideShowInt);
extern void     PB_SetSlideSpeedTable(UINT8 *SpeedTable);
extern void     PB_SetPictureBridgeON(UINT32 IfEnable);
extern INT32    PB_SetPBObject(PPLAY_OBJ pPlayObj);
//#NT#2009/06/11#Scottie -begin
//#NT#Add for the adjustment of display-able scope
extern void     PB_SetIDEWinDiff(UINT32 uiWDiff, UINT32 uiHDiff);
//#NT#2009/06/11#Scottie -end
extern void     PB_SetMaxPanelSize(UINT32 uiSize);
//#NT#2010/05/07#Scottie -begin
//#NT#Add for rotating image in thumbnail mode
extern void     PB_SetThumbUpdate(UINT32 uiCurIdx);
//#NT#2010/05/07#Scottie -end
//#NT#2010/06/03#Scottie -begin
//#NT#Use "Bit Rate Control" instead of old re-encoding mechanism
extern void     PB_SetExpectJPEGSize(UINT32 uiBSSize, UINT32 uiFineTuneVal, UINT32 uiTH1, UINT32 uiTH2, UINT32 uiLimitCnt);
//#NT#2010/06/03#Scottie -end
//#NT#2010/06/22#Scottie -begin
//#NT#Set current decoded-file-index in array
extern void     PB_SetCurFileIndex(UINT32 uiIdx);
//#NT#2010/06/22#Scottie -end
//----- others ----------------------------------------
extern void     PB_ReadImage(UINT32, INT8 *, UINT32, UINT32, UINT32);
extern void     PB_ReadImageforPlay(void);
extern void     PB_OpenSpecFile(UINT32 PlayDirId, UINT32 PlayFileId);
extern void     PB_OpenSpecFileBySeq(UINT32 uiSeqID, BOOL bOnlyQuery);
extern void     PB_SetCurrDPOFQtyOnly(UINT32 DpofPrtNum);
extern void     PB_SetCurrDPOFDateOnly(UINT32 DpofDateOn, char *pDscStr);
extern UINT8    PB_DPOFGetCreatorName(char * pRetVal, INT8 length);
extern UINT8    PB_DPOFGetRevision(char * pRetVal, INT8 length);
extern void     PB_DPOFSetDSCFormat(UINT8 dscFormat, UINT8 updateNow);
extern UINT8    PB_DPOFGetDSCFormat(void);
extern void     PB_DPOFSetDSCDateFormat(UINT8 dateFormat, UINT8 updateNow);
extern UINT8    PB_DPOFGetDSCDateFormat(void);
extern UINT8    PB_DPOFSetRevision(char * pRetVal, INT8 length, UINT8 updateNow);
extern UINT8    PB_DPOFSetCreatorName(char * pRetVal, INT8 length, UINT8 updateNow);
extern UINT8    PB_DPOFSave(void);
extern UINT8    PB_DPOFCancel(void);
extern UINT16   PB_SetCurrDPOFQtyDate(UINT32 DpofPrtNum, UINT32 DpofDateOn, char *pDscStr);
extern UINT8    PB_SetCurrDPOFQtyOnly2(UINT32 DpofPrtNum);
extern UINT8    PB_SetCurrDPOFDateOnly2(UINT32 DpofDateOn, char *pDscStr);
extern UINT16   PB_SetCurrDPOFQtyDate3(UINT32 DpofPrtNum, UINT32 DpofDateOn);//not in VSS; check only
//set dpof cropping
extern UINT8    PB_SetCurrDPOFCrop(UINT16 cropX, UINT16 cropY, UINT16 cropW, UINT16 cropH);

extern INT32    PB_QuasiLoadFile(void);
//#NT#2010/06/09#Scottie -begin
//#NT#Support Quasi-load thumnail image of JPEG
extern INT32    PB_QuasiLoadThumb(void);
//#NT#2010/06/09#Scottie -end
//#NT#2010/07/13#Scottie -begin
//#NT#Support Quasi-load screennail image of JPEG
extern INT32    PB_QuasiLoadScreennail(void);
//#NT#2010/07/13#Scottie -end

//#NT#2010/03/31#Hideo Lin -begin
//#NT#Added for user data (ex. social network tag, favorite tag)
extern UINT32   PB_GetSocialNetworkValue(void);
extern UINT32   PB_GetJpgSocialNetworkValue(void);
extern UINT32   PB_GetAviSocialNetworkValue(void);
extern void     PB_SetSocialNetworkValue(UINT32 uiSetMode, UINT32 uiValue);
extern INT32    PB_UpdateEXIFSocialNetworkValue(UINT32 uiDirectoryID, UINT32 uiFileNameID, USHORT uwValue);
extern INT32    PB_SetAviSocialNetworkValue(UINT32 uiDirectoryID, UINT32 uiFileNameID, UINT32 uiValue);
extern void     PB_SetAviUserDataIdentify(UINT8 *pucIdentify);
extern UINT16   *PB_GetAllThumbSocialNetworkArray(void);
extern void     PB_SocialNetworkHandle(UINT32 uiMode);
//#NT#2010/03/31#Hideo Lin -end

//KH mark
#if 0
extern void     PB_SWScale2Display(STR_SCALE_PRAM_EXT scale_info_ext);
#endif

#endif

//@}
