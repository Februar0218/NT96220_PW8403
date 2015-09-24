/**
    @file       displyr.h
    @ingroup    mISYSDisp

    @brief      Header file for Display layer control
                This file is the header file that define the API and data type
                for Display control object
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/** \addtogroup  mISYSDisp*/
//@{

#ifndef _DISPLYR_H
#define _DISPLYR_H

#include "Oscommon.h"
#include "dispcomn.h"
#include "ide.h"


/**
    @name Display Layer Control Group

    These definitions are used for display layer control API (DISP_OBJ.dispLyrCtrl) as the layer control portal.
    The layer characteristics such as buffer format/ width / height/ Window-width / window-height / (...etc) are controlled by this API.
    The user must know that the API would not set load to activate the settings.
    The User would programs all the settings needed and then use DISP_OBJ.load() to activate all the settings.
*/
//@{

/**
    Display Layer Buffer Format

    This definition is used for the OP Code: "DISPLAYER_OP_SET_MODE" in the dispLyrCtrl API to specify the buffer format.
    This feature can be assigned for each display layer with different setting.
*/
typedef enum
{
    DISPBUFFORMAT_PAL4,                 ///< Buffer uses format 4bit palette.
    DISPBUFFORMAT_PAL2,                 ///< Buffer uses format 2bit palette.
    DISPBUFFORMAT_PAL1,                 ///< Buffer uses format 1bit palette.
    DISPBUFFORMAT_PAL8,                 ///< Buffer uses format 8bit palette.

    DISPBUFFORMAT_YUV444,               ///< Buffer uses format YUV444.
    DISPBUFFORMAT_YUV422,               ///< Buffer uses format YUV422.
    DISPBUFFORMAT_YUV420,               ///< Buffer uses format YUV420.

    DISPBUFFORMAT_ARGB4565,             ///< Buffer uses format ARGB 4565.
    DISPBUFFORMAT_ARGB8565,             ///< Buffer uses format ARGB 8565.

    ENUM_DUMMY4WORD(DISPBUFFORMAT)
}DISPBUFFORMAT;

/**
    Display Video Layer Mode selection

    This definition is used for the OP Code: "DISPLAYER_OP_SET_MODE" in the dispLyrCtrl API to specify the buffer display mode.
    In the NT96220, the OSD layer's display mode is fixed as DISPBUFMODE_BUFFER_REPEAT.
*/
typedef enum
{
    DISPBUFMODE_AUTO_HANDSHAKE,             ///< Automatically change buffer according to IPE/IME signal
    DISPBUFMODE_RW_INDEPENDENT,             ///< Automatically change buffer according to IDE's VSync signal
    DISPBUFMODE_BUFFER_REPEAT,              ///< Repeat the same buffer until user change buffer

    ENUM_DUMMY4WORD(DISPBUFMODE)
}DISPBUFMODE;

/**
    Display Layer Buffer number

    This definition is used for the OP Code: "DISPLAYER_OP_SET_MODE" in the dispLyrCtrl API to specify
    the layer buffer number in the buffer mode DISPBUFMODE_AUTO_HANDSHAKE/DISPBUFMODE_RW_INDEPENDENT/DISPBUFMODE_BUFFER_REPEAT.
*/
typedef enum
{
    DISPBUFNUM_3,                           ///< Total 3 buffers (Buffer 0, 1, 2)
    DISPBUFNUM_2,                           ///< Total 2 buffers (Buffer 0, 1)
    DISPBUFNUM_1,                           ///< Only 1 buffer (Buffer 0)

    ENUM_DUMMY4WORD(DISPBUFNUM)
}DISPBUFNUM;

/**
    Display Layer Buffer output direction

    This definition is used for the OP Code: "DISPLAYER_OP_SET_OUTDIR" in the dispLyrCtrl API to specify the buffer output direction.
    This feature can make the effects of Horizontal/Vertical Flip or the rotation 180 degrees on the display.
    This feature can be assigned for each display layer with different setting.
*/
typedef enum
{
    DISPOUTDIR_NORMAL       =0x0,           ///< Normal output direction
    DISPOUTDIR_HRZ_FLIP     =0x1,           ///< Horizontal flip
    DISPOUTDIR_VTC_FLIP     =0x2,           ///< Vertical flip
    DISPOUTDIR_ROT_180      =0x3,           ///< Rotate 180 degree

    ENUM_DUMMY4WORD(DISPOUTDIR)
}DISPOUTDIR;

/**
    Display Layer Active Buffer

    This definition is used for the OP Code: "DISPLAYER_OP_SET_ACTBUF"/"DISPLAYER_OP_SET_VDOBUFADDR"/"DISPLAYER_OP_SET_OSDBUFADDR" in the dispLyrCtrl API
    to specify which of the active buffer is assigned to be displayed.
    This feature can be assigned for each display layer with different setting.
*/
typedef enum
{
    DISPACTBUF_0,                           ///< Select Active Buffer 0
    DISPACTBUF_1,                           ///< Select Active Buffer 1
    DISPACTBUF_2,                           ///< Select Active Buffer 2

    DISPACTBUF_NUM,
    ENUM_DUMMY4WORD(DISPACTBUF)
}DISPACTBUF;

/**


*/
typedef enum
{
    DISPBUFADR_ALL  =0x00,                  ///< Update Video/OSD layer Buffer 0/1/2 Address
    DISPBUFADR_0    =0x01,                  ///< Update Video/OSD layer Buffer 0 Address
    DISPBUFADR_1    =0x02,                  ///< Update Video/OSD layer Buffer 1 Address
    DISPBUFADR_2    =0x04,                  ///< Update Video/OSD layer Buffer 2 Address

    DISPBUFADR_MSK  =0x07,
    ENUM_DUMMY4WORD(DISPBUFADR)
}DISPBUFADR;

/**
    Display Layer operation selection

    This definition is used as the second operand in the dispLyrCtrl API to specify which of the display layer control operation
    would be performed.
*/
typedef enum
{
    /* SET control group */
    DISPLAYER_OP_SET_ENABLE = DISPCMDBASE_DISPLYR,  ///< Set specified display Layer Enable/Disable.
    DISPLAYER_OP_SET_MODE,                          ///< Configure selected layer buffer mode such as BUF Jump Mode,Buffer Number, Buffer format.
    DISPLAYER_OP_SET_BUFSIZE,                       ///< Set display Buffer Size for Video/OSD layer such as buffer Width/Height/LineOffset
    DISPLAYER_OP_SET_VDOBUFADDR,                    ///< Set display Video Buffer Address
    DISPLAYER_OP_SET_OSDBUFADDR,                    ///< Set display OSD Buffer Address
    DISPLAYER_OP_SET_ACTBUF,                        ///< Set specified active buffer number when buffer is at buffer-repeat-mode.
    DISPLAYER_OP_SET_OUTDIR,                        ///< Set Buffer Output Order such as Flip / Rotation
    DISPLAYER_OP_SET_WINSIZE,                       ///< Set Output Window Size and window offset. This is truely size seen on the display/panel.
    DISPLAYER_OP_SET_PALETTE,                       ///< Set the OSD palette table. Valid for OSD layer only.

    /* GET control group */
    DISPLAYER_OP_GET_ENABLE,                        ///< Get specified layer Enable/Disable.
    DISPLAYER_OP_GET_MODE,                          ///< Get selected layer buffer mode such as BUF Jump Mode,Buffer Number, Buffer format.
    DISPLAYER_OP_GET_BUFSIZE,                       ///< Get display Buffer Size for Video/OSD layer such as buffer Width/Height/LineOffset
    DISPLAYER_OP_GET_VDOBUFADDR,                    ///< Get display Video Buffer Address
    DISPLAYER_OP_GET_OSDBUFADDR,                    ///< Get display OSD Buffer Address
    DISPLAYER_OP_GET_ACTBUF,                        ///< Get specified layer active buffer number.
    DISPLAYER_OP_GET_OUTDIR,                        ///< Get Buffer Output Order such as Flip / Rotation
    DISPLAYER_OP_GET_WINSIZE,                       ///< Get Output Window Size and window offset. This is truely size seen on the display/panel.
    DISPLAYER_OP_GET_PALETTE,                       ///< Get the OSD palette table. Valid for OSD layer only.

    /* Macro API */
    DISPLAYER_OP_SET_VDOBUFCONTENT,                 ///< Set specified video layer buffer content to specified color. Valid for video layer only.

    ENUM_DUMMY4WORD(DISPLAYER_OP)
}DISPLAYER_OP;

/**
    Display Layer Control Parameter Definition

    This definition is used as the third operand in the dispLyrCtrl API to specify the display layer control parameters
    for each of the layer control opeation.
*/
typedef struct
{
    union
    {
        /* SET control group */

        /**
            Set Specified Display Layer Enable/Disable

            This structure is used for layer operation code "DISPLAYER_OP_SET_ENABLE" to set the specified "DISPLAYER" enable/disable.
            This structure shuld be used like this:
        \n  DispLyr.SEL.SET_ENABLE.bEn = TRUE;  //Or FALSE
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_ENABLE, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_ENABLE
        {
            BOOL            bEn;            ///< Set specified display layer ON/OFF. TRUE: Enable. FALSE:Disable.
        } SET_ENABLE;

        /**
            Set Specified Display Layer Buffer Mode/Format

            This structure is used for layer operation code "DISPLAYER_OP_SET_MODE"
            to set the specified "DISPLAYER" buffer Format/Mode/Number. This structure shuld be used like this:
        \n  DispLyr.SEL.SET_MODE.BufFormat = DISPBUFFORMAT_YUV422;              // Or DISPBUFFORMAT_PAL8 ...etc
        \n  DispLyr.SEL.SET_MODE.BufMode   = DISPBUFMODE_BUFFER_REPEAT;         // Or DISPBUFMODE_AUTO_HANDSHAKE
        \n  DispLyr.SEL.SET_MODE.BufNumber = DISPBUFNUM_3;                      // Or DISPBUFNUM_2 / DISPBUFNUM_1
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_MODE, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_MODE
        {
            DISPBUFFORMAT   BufFormat;      ///< Video/OSD Buffer Format, such as YUV444/YUV422/YUV420/ARGB8565/ARGB4565
            DISPBUFMODE     BufMode;        ///< Video/OSD Buffer operation mode. OSD layer support buffer repeat mode only
            DISPBUFNUM      BufNumber;      ///< Configure used buffer number for Video/OSD
        } SET_MODE;

        /**
            Set Specified Display Layer Buffer Size

            This structure is used for layer operation code "DISPLAYER_OP_SET_BUFSIZE"
            to set the specified "DISPLAYER" buffer Width/Height/LineOffset. This structure shuld be used like this:
        \n  DispLyr.SEL.SET_BUFSIZE.uiBufWidth  = 320;
        \n  DispLyr.SEL.SET_BUFSIZE.uiBufHeight = 240;
        \n  DispLyr.SEL.SET_BUFSIZE.uiBufLineOfs= 320;
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_BUFSIZE, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_BUFSIZE
        {
            UINT32          uiBufWidth;     ///< Set display buffer width in PIXEL COUNT for Video/OSD layer.
            UINT32          uiBufHeight;    ///< Set display buffer height in PIXEL COUNT for Video/OSD layer.

            UINT32          uiBufLineOfs;   ///< Set display buffer lineoffset in BYTES for Video/OSD layer.
        } SET_BUFSIZE;

        /**
            Set Specified Display VIDEO Layer Buffer Address

            This structure is used for layer operation code "DISPLAYER_OP_SET_VDOBUFADDR"
            to set the specified "DISPLAYER" buffer address.
            This structure shuld be used like this:
        \n  DispLyr.SEL.SET_VDOBUFADDR.BufSel     = DISPBUFADR_ALL;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrY0   = 0x400000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrCb0  = 0x480000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrCr0  = 0x500000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrY1   = 0x580000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrCb1  = 0x600000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrCr1  = 0x680000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrY2   = 0x700000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrCb2  = 0x780000;
        \n  DispLyr.SEL.SET_VDOBUFADDR.uiAddrCr2  = 0x800000;
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_VDOBUFADDR, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_VDOBUFADDR
        {
            DISPBUFADR      BufSel;         ///< Select which of the buffer addresses would be updated for Video layer.

            UINT32          uiAddrY0;       ///< Buffer 0 Starting Address for  Y/RGB565/RGB888 plane
            UINT32          uiAddrCb0;      ///< Buffer 0 Starting Address for Cb/Alpha  plane
            UINT32          uiAddrCr0;      ///< Buffer 0 Starting Address for Cr        plane

            UINT32          uiAddrY1;       ///< Buffer 1 Starting Address for  Y/RGB565/RGB888 plane
            UINT32          uiAddrCb1;      ///< Buffer 1 Starting Address for Cb/Alpha  plane
            UINT32          uiAddrCr1;      ///< Buffer 1 Starting Address for Cr        plane

            UINT32          uiAddrY2;       ///< Buffer 2 Starting Address for  Y/RGB565/RGB888 plane
            UINT32          uiAddrCb2;      ///< Buffer 2 Starting Address for Cb/Alpha  plane
            UINT32          uiAddrCr2;      ///< Buffer 2 Starting Address for Cr        plane
        } SET_VDOBUFADDR;

        /**
            Set Specified Display OSD Layer Buffer Address

            This structure is used for layer operation code "DISPLAYER_OP_SET_OSDBUFADDR"
            to set the specified "DISPLAYER" buffer ADDRESS. This structure shuld be used like this:
        \n  DispLyr.SEL.SET_OSDBUFADDR.BufSel     = DISPBUFADR_ALL;
        \n  DispLyr.SEL.SET_OSDBUFADDR.uiAddrBuf0 = 0x400000;
        \n  DispLyr.SEL.SET_OSDBUFADDR.uiAddrBuf1 = 0x480000;
        \n  DispLyr.SEL.SET_OSDBUFADDR.uiAddrBuf2 = 0x500000;
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_OSDBUFADDR, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_OSDBUFADDR
        {
            DISPBUFADR      BufSel;         ///< Select which of the buffer addresses would be updated for OSD layer.

            UINT32          uiAddrBuf0;     ///< Buffer 0 Starting Address for OSD layer
            UINT32          uiAddrBuf1;     ///< Buffer 1 Starting Address for OSD layer
            UINT32          uiAddrBuf2;     ///< Buffer 2 Starting Address for OSD layer

            #if 0// 96220 has no ARGB buffer format
            UINT32          uiAddrAlpha0;
            UINT32          uiAddrAlpha1;
            UINT32          uiAddrAlpha2;
            #endif
        } SET_OSDBUFADDR;

        /**
            Set Specified Display Layer Active Buffer

            This structure is used for layer operation code "DISPLAYER_OP_SET_ACTBUF"
            to set the specified "DISPLAYER" active buffer. This structure shuld be used like this:
        \n  DispLyr.SEL.SET_ACTBUF.ActiveBuf  = DISPACTBUF_0/1/2;
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_ACTBUF, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_ACTBUF
        {
            DISPACTBUF      ActiveBuf;      ///< Select which Video/OSD buffer is activated. For video layer, this field is valid only at buffer repeat mode.
        } SET_ACTBUF;

        /**
            Set Specified Display Layer Buffer output direction

            This structure is used for layer operation code "DISPLAYER_OP_SET_OUTDIR"
            to set the specified "DISPLAYER" buffer output direction. This structure shuld be used like this:
        \n  DispLyr.SEL.SET_OUTDIR.BufOutDir  = DISPOUTDIR_NORMAL;
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_OUTDIR, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_OUTDIR
        {
            DISPOUTDIR      BufOutDir;      ///< Select buffer output as Horizontal/Vertical-Flip, or Rotation 90/180/270 degrees.
        } SET_OUTDIR;

        /**
            Set Specified Display Layer output window size and window offset

            This structure is used for layer operation code "DISPLAYER_OP_SET_WINSIZE"
            to set the specified "DISPLAYER" window size and also the window offset. This structure shuld be used like this:
        \n  DispLyr.SEL.SET_WINSIZE.uiWinWidth  = 320;
        \n  DispLyr.SEL.SET_WINSIZE.uiWinHeight = 240;
        \n  DispLyr.SEL.SET_WINSIZE.iWinOfsX    = 0;
        \n  DispLyr.SEL.SET_WINSIZE.iWinOfsY    = 0;
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_WINSIZE, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_WINSIZE
        {
            UINT32          uiWinWidth;     ///< The display Video/OSD layer output window width
            UINT32          uiWinHeight;    ///< The display Video/OSD layer output window height

             INT32          iWinOfsX;       ///< The display Video/OSD layer output window offset X
             INT32          iWinOfsY;       ///< The display Video/OSD layer output window offset Y
        } SET_WINSIZE;

        /**
            Set Specified Display OSD Layer palette table

            This structure is used for layer operation code "DISPLAYER_OP_SET_PALETTE"
            to set the specified "DISPLAYER" palette table. This structure shuld be used like this:
        \n  DispLyr.SEL.SET_PALETTE.uiStart     = 0;
        \n  DispLyr.SEL.SET_PALETTE.uiNumber    = 256;
        \n  DispLyr.SEL.SET_PALETTE.pPaleEntry  = (Address of palette table);
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_PALETTE, &DispLyr);
        */
        struct// DISPLAYER_OP_SET_PALETTE
        {
            UINT32          uiStart;        ///< Start entry id. Valid range from 0~255.
            UINT32          uiNumber;       ///< Total number of palette entry to be set. Valid range from 1~256
            UINT32          *pPaleEntry;    ///< pPaleEntry[7~0]:      Cr value of the palette.
                                            ///< pPaleEntry[15~8]:     Cb value of the palette.
                                            ///< pPaleEntry[23~16]:    Y  value of the palette.
                                            ///< pPaleEntry[31~24]:    Alpha value of the palette.
        } SET_PALETTE;





        /* GET control group */

        /**
            Get Specified Display Layer enable/disable

            This structure is used for layer operation code "DISPLAYER_OP_GET_ENABLE"
            to get the specified "DISPLAYER" layer enable/disable. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_ENABLE, &DispLyr);
        \n  "DispLyr.SEL.GET_ENABLE.bEn"
            is the specified layer enable/disable.
        */
        struct// DISPLAYER_OP_GET_ENABLE
        {
            BOOL            bEn;          ///< Get specified display layer ON/OFF. TRUE: Enable. FALSE:Disable.
        } GET_ENABLE;

        /**
            Get Specified Display Layer buffer mode/format/number configurations

            This structure is used for layer operation code "DISPLAYER_OP_GET_ENABLE"
            to get the specified "DISPLAYER" layer buffer configurations. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_ENABLE, &DispLyr);
        \n  "DispLyr.SEL.GET_MODE.BufFormat"/"DispLyr.SEL.GET_MODE.BufMode"/"DispLyr.SEL.GET_MODE.BufNumber"
            are the specified layer buffer format/mode/number.
        */
        struct// DISPLAYER_OP_GET_MODE
        {
            DISPBUFFORMAT   BufFormat;    ///< Get Video/OSD Buffer Format, such as YUV444/YUV422/YUV420/ARGB8565/ARGB4565
            DISPBUFMODE     BufMode;      ///< Get Video/OSD Buffer operation mode. OSD layer support buffer repeat mode only
            DISPBUFNUM      BufNumber;    ///< Get active buffer number for Video/OSD
        } GET_MODE;

        /**
            Get Specified Display Layer buffer width/height/lineoffset

            This structure is used for layer operation code "DISPLAYER_OP_GET_BUFSIZE"
            to get the specified "DISPLAYER" layer buffer width/height/lineoffset. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_BUFSIZE, &DispLyr);
        \n  "DispLyr.SEL.GET_BUFSIZE.uiBufWidth"/"DispLyr.SEL.GET_BUFSIZE.uiBufHeight"/"DispLyr.SEL.GET_BUFSIZE.uiBufLineOfs"
            are the specified layer buffer width/height/lineoffset.
        */
        struct// DISPLAYER_OP_GET_BUFSIZE
        {
            UINT32          uiBufWidth;   ///< Get display buffer width for Video/OSD layer.
            UINT32          uiBufHeight;  ///< Get display buffer height for Video/OSD layer.
            UINT32          uiBufLineOfs; ///< Get display buffer lineoffset for Video/OSD layer.
        } GET_BUFSIZE;

        /**
            Get Specified Display VIDEO Layer buffer address

            This structure is used for layer operation code "DISPLAYER_OP_GET_VDOBUFADDR"
            to get the specified "DISPLAYER" layer buffer address. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_VDOBUFADDR, &DispLyr);
        \n  "DispLyr.SEL.GET_VDOBUFADDR.ActiveBuf"/"DispLyr.SEL.GET_VDOBUFADDR.uiAddrY0"/"DispLyr.SEL.GET_VDOBUFADDR.uiAddrCb0"/...
            are the specified VIDEO layer buffer address and active buffer number.
        */
        struct// DISPLAYER_OP_GET_VDOBUFADDR
        {
            DISPACTBUF      ActiveBuf;    ///< This field is valid only when video buffer at buffer repeat mode.
                                          ///< Get which of the buffer address is activated for Video layer.

            UINT32          uiAddrY0;     ///< Buffer 0 Starting Address for  Y/RGB565 plane
            UINT32          uiAddrCb0;    ///< Buffer 0 Starting Address for Cb/Alpha  plane
            UINT32          uiAddrCr0;    ///< Buffer 0 Starting Address for Cr        plane

            UINT32          uiAddrY1;     ///< Buffer 1 Starting Address for  Y/RGB565 plane
            UINT32          uiAddrCb1;    ///< Buffer 1 Starting Address for Cb/Alpha  plane
            UINT32          uiAddrCr1;    ///< Buffer 1 Starting Address for Cr        plane

            UINT32          uiAddrY2;     ///< Buffer 2 Starting Address for  Y/RGB565 plane
            UINT32          uiAddrCb2;    ///< Buffer 2 Starting Address for Cb/Alpha  plane
            UINT32          uiAddrCr2;    ///< Buffer 2 Starting Address for Cr        plane
        } GET_VDOBUFADDR;

        /**
            Get Specified Display OSD Layer buffer address

            This structure is used for layer operation code "DISPLAYER_OP_GET_OSDBUFADDR"
            to get the specified "DISPLAYER" layer buffer address. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_OSDBUFADDR, &DispLyr);
        \n  "DispLyr.SEL.GET_OSDBUFADDR.ActiveBuf"/"DispLyr.SEL.GET_OSDBUFADDR.uiAddrBuf0"/"DispLyr.SEL.GET_OSDBUFADDR.uiAddrBuf1"/...
            are the specified OSD layer buffer address and active buffer number.
        */
        struct// DISPLAYER_OP_GET_OSDBUFADDR
        {
            DISPACTBUF      ActiveBuf;      ///< Select which one of the buffer address is activated for OSD layer.

            UINT32          uiAddrBuf0;     ///< Buffer 0 Starting Address for OSD layer
            UINT32          uiAddrBuf1;     ///< Buffer 1 Starting Address for OSD layer
            UINT32          uiAddrBuf2;     ///< Buffer 2 Starting Address for OSD layer

            #if 0// 96220 has no ARGB buffer format
            UINT32          uiAddrAlpha0;
            UINT32          uiAddrAlpha1;
            UINT32          uiAddrAlpha2;
            #endif
        } GET_OSDBUFADDR;

        /**
            Get Specified Display Layer active buffer

            This structure is used for layer operation code "DISPLAYER_OP_GET_ACTBUF"
            to get the specified "DISPLAYER" layer active buffer if in the buffer repeat mode. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_ACTBUF, &DispLyr);
        \n  "DispLyr.SEL.GET_ACTBUF.ActiveBuf"
            is the specified layer active buffer ID.
        */
        struct// DISPLAYER_OP_GET_ACTBUF
        {
            DISPACTBUF      ActiveBuf;      ///< Get which Video/OSD buffer is activated. For video layer, this field is valid only at buffer repeat mode.
        } GET_ACTBUF;

        /**
            Get Specified Display Layer buffer output direction

            This structure is used for layer operation code "DISPLAYER_OP_GET_OUTDIR"
            to get the specified "DISPLAYER" layer buffer output direction. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_OUTDIR, &DispLyr);
        \n  "DispLyr.SEL.GET_OUTDIR.BufOutDir"
            is the specified layer buffer output direction.
        */
        struct// DISPLAYER_OP_GET_OUTDIR
        {
            DISPOUTDIR      BufOutDir;      ///< Get Horizontal/Vertical-Flip, or Rotation 90/180/270 degrees.
        } GET_OUTDIR;

        /**
            Get Specified Display Layer output window size/offset

            This structure is used for layer operation code "DISPLAYER_OP_GET_WINSIZE"
            to get the specified "DISPLAYER" layer output window size/offset. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_WINSIZE, &DispLyr);
        \n  "DispLyr.SEL.GET_WINSIZE.uiWinWidth"/"DispLyr.SEL.GET_WINSIZE.uiWinHeight"/"DispLyr.SEL.GET_WINSIZE.iWinOfsX"/"DispLyr.SEL.GET_WINSIZE.iWinOfsY"
            are the specified layer buffer output window size/offset.
        */
        struct// DISPLAYER_OP_GET_WINSIZE
        {
            UINT32          uiWinWidth;     ///< The display Video/OSD layer output window width
            UINT32          uiWinHeight;    ///< The display Video/OSD layer output window height

             INT32          iWinOfsX;       ///< The display Video/OSD layer output window offset X
             INT32          iWinOfsY;       ///< The display Video/OSD layer output window offset Y
        } GET_WINSIZE;

        /**
            Get Specified Display OSD Layer palette table

            This structure is used for layer operation code "DISPLAYER_OP_GET_PALETTE"
            to get the specified "DISPLAYER" layer palette table. This structure shuld be used like this:
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_GET_PALETTE, &DispLyr);
        \n  "DispLyr.SEL.GET_PALETTE.uiStart"/"DispLyr.SEL.GET_PALETTE.uiNumber"/"DispLyr.SEL.GET_PALETTE.pPaleEntry"
            are the specified OSD layer palette table.
        */
        struct// DISPLAYER_OP_GET_PALETTE
        {
            UINT32          uiStart;        ///< Start entry id. Valid range from 0~255.
            UINT32          uiNumber;       ///< Total number of palette entry to be set. Valid range from 1~256
            UINT32          *pPaleEntry;    ///< pPaleEntry[7~0]:      Cr value of the palette.
                                            ///< pPaleEntry[15~8]:     Cb value of the palette.
                                            ///< pPaleEntry[23~16]:    Y  value of the palette.
                                            ///< pPaleEntry[31~24]:    Alpha value of the palette.
        } GET_PALETTE;


        /* Macro API */

        /**
            Set Specified Video Display Layer Buffer Content

            This structure is used for layer operation code "DISPLAYER_OP_SET_VDOBUFCONTENT" to
            set the specified Video "DISPLAYER"'s buffer content.
            This structure shuld be used like this:
        \n  DispLyr.SEL.SET_VDOBUFCONTENT.bEn = TRUE;  //Or FALSE
        \n  pDispObj->dispLyrCtrl(DISPLAYER, DISPLAYER_OP_SET_ENABLE, &DispLyr);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPLAYER_OP_SET_VDOBUFCONTENT
        {
            IDE_BUFFERID            BufID;    ///< Select which of the Buffer ID to fill the content.
            PYUVCOLOR               pYCbCr;   ///< The color which will be filled up with the buffer
        } SET_VDOBUFCONTENT;

    }SEL;
}DISPLAYER_PARAM,*PDISPLAYER_PARAM;
//@}


typedef ER                (*DISPLYRCTRL)(DISPLAYER, DISPLAYER_OP, PDISPLAYER_PARAM);

#endif
