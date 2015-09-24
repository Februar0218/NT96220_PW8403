/**
    @file       dispctrl.h
    @ingroup    mISYSDisp

    @brief      Header file for Display Engine control
                This file is the header file that define the API and data type
                for Display control object
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/** \addtogroup  mISYSDisp*/
//@{

#ifndef _DISPCTRL_H
#define _DISPCTRL_H

#include "Oscommon.h"
#include "dispcomn.h"
#include "ide.h"



/**
    @name Display Control Definition Group

    These definitions are used for display control API (DISP_OBJ.dispCtrl) as the display control portal.
    The display general functionalities such as subpixel/HLPF/Contrast/Saturation/Brightness (...etc) are controlled by this API.
    The user must know that the API would not set load to activate the settings.
    The User would programs all the settings needed and then use DISP_OBJ.load() to activate all the settings.
*/
//@{

/**
    Display output format type definition

    This definition is used for the OP Code: "DISPCTRL_SET_DEVICE" in the dispCtrl API to specify the display engine output format type.
    Such as RGB/YUV-Serial,RGB/YUV-Parallel,CCIR-601/656, ...etc.
*/
typedef enum
{
    DISPDEV_TYPE_RGB_SERIAL,                ///< Display MODE is RGB Serial or UPS051
    DISPDEV_TYPE_RGB_PARALL,                ///< Display MODE is RGB Parallel
    DISPDEV_TYPE_RGBD,                      ///< Display MODE is RGB Dummy
    DISPDEV_TYPE_RGB_THROUGH,               ///< Display MODE is RGB through mode
    DISPDEV_TYPE_YUV,                       ///< Display MODE is YUV
    DISPDEV_TYPE_CCIR601_8BIT,              ///< Display MODE is CCIR601 8bits bus
    DISPDEV_TYPE_CCIR656_8BIT,              ///< Display MODE is CCIR656 8bits bus
    DISPDEV_TYPE_CCIR601_16BIT,             ///< Display MODE is CCIR601 16bits bus
    DISPDEV_TYPE_CCIR656_16BIT,             ///< Display MODE is CCIR656 16bits bus

    DISPDEV_TYPE_INF_HDMI_8BIT_DDR,         ///< Display MODE is outsourcing HDMI 8bits with double-data-rate(DDR) bus
    DISPDEV_TYPE_INF_HDMI_16BIT,            ///< Display MODE is outsourcing HDMI 16bits bus
    DISPDEV_TYPE_INF_HDMI_24BIT,            ///< Display MODE is outsourcing HDMI 24bits bus

    DISPDEV_TYPE_MI,                        ///< Display MODE is Memory interface
    DISPDEV_TYPE_EMBD_TV,                   ///< Display MODE is Embedded TV Encoder
    DISPDEV_TYPE_EMBD_HDMI,                 ///< Display MODE is Embedded HDMI transmitter

    ENUM_DUMMY4WORD(DISPDEV_TYPE)
} DISPDEV_TYPE;

/**
    Display Engine Source clock select definition

    This definition is used for the OP Code: "DISPCTRL_SET_SRCCLK" in the dispCtrl API to specify the display engine clock source.
*/
typedef enum
{
    DISPCTRL_SRCCLK_PLL1,                   ///< The display source clock from PLL1
    DISPCTRL_SRCCLK_PLL2,                   ///< The display source clock from PLL2
    DISPCTRL_SRCCLK_SSPLL,                  ///< The display source clock from SSPLL
    DISPCTRL_SRCCLK_MI_ENGINE,              ///< The display source clock from Fixed MI Engine Clock Source 240MHz

    DISPCTRL_SRCCLK_DEFAULT = DISPCTRL_SRCCLK_PLL2,///< Default display source clock selection for realchip
    ENUM_DUMMY4WORD(DISPCTRL_SRCCLK)
}DISPCTRL_SRCCLK;

/**
    Display control operation ID definition

    This field is used as the first operand in the dispCtrl API of the display object to specify
    which of the display control operation would be performed.
*/
typedef enum
{
    /* SET control group */
    DISPCTRL_SET_ENABLE = DISPCMDBASE_DISPCTRL, ///< Set display engine's global enable/disable.
    DISPCTRL_SET_ALL_LYR_EN,                    ///< Set ALL selected display layers Enable/Disable.
    DISPCTRL_SET_BACKGROUND,                    ///< Set Display background color.
    DISPCTRL_SET_GAMMA_EN,                      ///< Set display Gamma function enable/disable. Also config the Gamma coefficients if function enabled.
    DISPCTRL_SET_ICST_EN,                       ///< Set display Inverse-Color-Space-Transform function enable/disable.
    DISPCTRL_SET_ICST_COEF,                     ///< Set display Inverse-Color-Space-Transform function coefficients.
    DISPCTRL_SET_HLPF_EN,                       ///< Set display horizontal low pass filter enable/disable.
    DISPCTRL_SET_CONST_OFS,                     ///< Set display window constant offset X/Y. This feature is used for handling the overscan/underscan hdmi tv.
    DISPCTRL_SET_SRCCLK,                        ///< Set display operating source clock

    /* GET control group */
    DISPCTRL_GET_ENABLE,                        ///< Get display engine's global enable/disable.
    DISPCTRL_GET_SRCCLK,                        ///< Get display operating source clock
    DISPCTRL_GET_ALL_LYR_EN,                    ///< Get ALL selected display layers Enable/Disable.

    ENUM_DUMMY4WORD(DISPCTRL_OP)
}DISPCTRL_OP;

/**
    Display Control Parameter Definition

    This definition is used as the second operand in the dispCtrl API to specify the display control parameters
    for each of the display control opeation.
*/
typedef struct
{
    union
    {
        /* SET control group */

        /**
            Set Display Engine Enable/Disable

            This structure is used for display control operation code "DISPCTRL_SET_ENABLE"
            to set the display enable/disable. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_ENABLE.bEn     = TRUE;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_ENABLE, &DispCtrl);
        */
        struct// DISPCTRL_SET_ENABLE
        {
            BOOL            bEn;            ///< Display Enable/Disable
        } SET_ENABLE;

        /**
            Set Display Engine layers enable/disable

            This structure is used for display control operation code "DISPCTRL_SET_ALL_LYR_EN"
            to set the display layers enable/disable. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = TRUE;
        \n  DispCtrl.SEL.SET_ALL_LYR_EN.DispLyr = DISPLAYER_VDO1|DISPLAYER_OSD1;    // Enable the specified layers.
        \n  pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
        \n  Or Using like this:
        \n  DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = FALSE;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct//DISPCTRL_SET_ALL_LYR_EN
        {
            BOOL            bEn;            ///< For ALL display layers ON/OFF. TRUE: Enable. FALSE:Disable.
            DISPLAYER       DispLyr;        ///< If bEn = TRUE, the specified display layers would be enabled.
                                            ///< If bEn = FALSE, "ALL" the display layers would be disabled. This parameter would be useless.
        } SET_ALL_LYR_EN;

        /**
            Set the Display default background color

            This structure is used for display control operation code "DISPCTRL_SET_BACKGROUND"
            to set the display default background color.
            If none of the display layer is enabled, the default color on the display screen is the background color.
            This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_BACKGROUND.uiColorY     = 0x00;
        \n  DispCtrl.SEL.SET_BACKGROUND.uiColorCB    = 0x80;
        \n  DispCtrl.SEL.SET_BACKGROUND.uiColorCR    = 0x80;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_BACKGROUND, &DispCtrl);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPCTRL_SET_BACKGROUND
        {
            UINT32          uiColorY;       ///< Background color  Y value. Must be within 0~0xFF.
            UINT32          uiColorCB;      ///< Background color CB value. Must be within 0~0xFF.
            UINT32          uiColorCR;      ///< Background color CR value. Must be within 0~0xFF.
        } SET_BACKGROUND;

        /**
            Set Display Engine Gamma Function

            This structure is used for display control operation code "DISPCTRL_SET_GAMMA_EN"
            to set the display gamma function enable/disable and also the gamma table. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_GAMMA_EN.bEnY          = TRUE;
        \n  DispCtrl.SEL.SET_GAMMA_EN.pGammaTabY    = (Address of the Gamman Table);
        \n  pDispObj->dispCtrl(DISPCTRL_SET_GAMMA_EN, &DispCtrl);
        \n  Or Using like this:
        \n  DispCtrl.SEL.SET_GAMMA_EN.bEnY          = FALSE;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_GAMMA_EN, &DispCtrl);
        */
        struct// DISPCTRL_SET_GAMMA_EN
        {
            BOOL            bEnY;           ///< Enable/Disable the Gamma function for component Y
            UINT8           *pGammaTabY;    ///< The 17 UINT8 entries gamma table for component Y. This field is useless if bEnY=FALSE.
        } SET_GAMMA_EN;

        /**
            Set Display Engine Inverse Color Space Transform

            This structure is used for display control operation code "DISPCTRL_SET_ICST_EN"
            to set the display Inverse Color Space Transform configurations. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_ICST_EN.bEn        = TRUE;
        \n  DispCtrl.SEL.SET_ICST_EN.Select     = CST_YCBCR2RGB;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_ICST_EN, &DispCtrl);
        \n  Or Using like this:
        \n  DispCtrl.SEL.SET_ICST_EN.bEn        = FALSE;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_ICST_EN, &DispCtrl);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPCTRL_SET_ICST_EN
        {
            BOOL            bEn;            ///< The Inverse color space transform function enable/disable.
            CST_SEL         Select;         ///< The color space transform of RGB2YCBCR or YCBCR2RGB.
        } SET_ICST_EN;

        /**
            Set Display Engine ICST Coeficients

            This structure is used for display control operation code "DISPCTRL_SET_ICST_COEF"
            to set the display Inverse Color Space Transform Coeficients. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_ICST_COEF.piCoef     = (Address of the ICST Coeficients table);
        \n  DispCtrl.SEL.SET_ICST_COEF.iPreOfsY   = 0;
        \n  DispCtrl.SEL.SET_ICST_COEF.iPreOfsCb  = -128;
        \n  DispCtrl.SEL.SET_ICST_COEF.iPreOfsCr  = -128;
        \n  DispCtrl.SEL.SET_ICST_COEF.iPostOfsY  = 0;
        \n  DispCtrl.SEL.SET_ICST_COEF.iPostOfsCb = 0;
        \n  DispCtrl.SEL.SET_ICST_COEF.iPostOfsCr = 0;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_ICST_COEF, &DispCtrl);
        */
        struct// DISPCTRL_SET_ICST_COEF
        {
            INT32          *piCoef;         ///< The 3X3 ICST Matrix coefficients. The coefficient format is (Q3.8). Valid range from -2048 to 2047.
                                            ///< The coefficients in order are: Ry, Ru, Rv, Gy, Gu, Gv, By, Bu, Bv.

            INT16           iPreOfsY;       ///< Inverse color space transform Pre-Offset value for Y.  Valid range from -128 to 127.
            INT16           iPreOfsCb;      ///< Inverse color space transform Pre-Offset value for Cb. Valid range from -128 to 127.
            INT16           iPreOfsCr;      ///< Inverse color space transform Pre-Offset value for Cr. Valid range from -128 to 127.

            INT16           iPostOfsY;      ///< Inverse color space transform Post-Offset value for Y.  Valid range from -128 to 127.
            INT16           iPostOfsCb;     ///< Inverse color space transform Post-Offset value for Cb. Valid range from -128 to 127.
            INT16           iPostOfsCr;     ///< Inverse color space transform Post-Offset value for Cr. Valid range from -128 to 127.
        } SET_ICST_COEF;

        /**
            Set Display Engine Horizontal Low Pass Filter Enable/Disable

            This structure is used for display control operation code "DISPCTRL_SET_HLPF_EN"
            to set the display Horizontal Low Pass Filter Enable/Disable. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_HLPF_EN.bEn     = TRUE;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_HLPF_EN, &DispCtrl);
        */
        struct// DISPCTRL_SET_HLPF_EN
        {
            BOOL            bEn;            ///< TRUE: Enable horizontal LPF.  FALSE: Disable horizontal LPF.
        } SET_HLPF_EN;

        /**
            Set Display Engine Constant Window Offset

            This structure is used for display control operation code "DISPCTRL_SET_CONST_OFS"
            to set the display  Constant Window Offset. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsX    = 0x0;
        \n  DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsY    = 0x0;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_CONST_OFS, &DispCtrl);
            @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
        */
        struct// DISPCTRL_SET_CONST_OFS
        {
             UINT32          uiWinConstOfsX;    ///< The display Video/OSD layer output window constant offset X
             UINT32          uiWinConstOfsY;    ///< The display Video/OSD layer output window constant offset Y
        } SET_CONST_OFS;

        /**
            Set Display Engine Clock Source

            This structure is used for display control operation code "DISPCTRL_SET_SRCCLK"
            to set the display Clock Source. This structure shuld be used like this:
        \n  DispCtrl.SEL.SET_SRCCLK.SrcClk    = DISPCTRL_SRCCLK_PLL2;
        \n  pDispObj->dispCtrl(DISPCTRL_SET_SRCCLK, &DispCtrl);
        */
        struct// DISPCTRL_SET_SRCCLK
        {
             DISPCTRL_SRCCLK    SrcClk;         ///< Select display source clock
        } SET_SRCCLK;



        /* GET control group */

        /**
            Get Display Engine Enable/Disable

            This structure is used for display control operation code "DISPCTRL_GET_ENABLE"
            to get the display enable/disable. This structure shuld be used like this:
        \n  pDispObj->dispCtrl(DISPCTRL_GET_ENABLE, &DispCtrl);
        \n  "DispCtrl.SEL.GET_ENABLE.bEn"
            is the display enable/disable information.
        */
        struct// DISPCTRL_GET_ENABLE
        {
            BOOL            bEn;          ///< Get display Enable/Disable
        } GET_ENABLE;

        /**
            Get Display Engine Clock Source

            This structure is used for display control operation code "DISPCTRL_GET_SRCCLK"
            to get the display Clock Source. This structure shuld be used like this:
        \n  pDispObj->dispCtrl(DISPCTRL_GET_SRCCLK, &DispCtrl);
        \n  "DispCtrl.SEL.GET_SRCCLK.SrcClk"
            is the display Clock Source information.
        */
        struct// DISPCTRL_GET_SRCCLK
        {
             DISPCTRL_SRCCLK    SrcClk;   ///< Get display source clock
        } GET_SRCCLK;

        /**
            Get Display Engine layers enable information

            This structure is used for display control operation code "DISPCTRL_GET_ALL_LYR_EN"
            to get the display layers enable information. This structure shuld be used like this:
        \n  pDispObj->dispCtrl(DISPCTRL_GET_ALL_LYR_EN, &DispCtrl);
        \n  "DispCtrl.SEL.GET_ALL_LYR_EN.DispLyr"
            is the display layers enable information.
        */
        struct// DISPCTRL_GET_ALL_LYR_EN
        {
            DISPLAYER       DispLyr;      ///< Get the information of which the display layers are enabled.
        } GET_ALL_LYR_EN;

    }SEL;
}DISPCTRL_PARAM,*PDISPCTRL_PARAM;
//@}



typedef ER                (*DISPCTRL)(DISPCTRL_OP, PDISPCTRL_PARAM);


#endif
