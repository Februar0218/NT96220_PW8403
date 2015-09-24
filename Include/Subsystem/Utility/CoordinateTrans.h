/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CoordinateTrans.h
    @ingroup    mIPRJAPUIFlow

    @brief      Coordinate Translate form LCD <-> TV

    @note       Nothing.

    @date       2009/07/06
*/


/** \addtogroup mUtility */
//@{
#ifndef COORDINATETRANS_H
#define COORDINATETRANS_H

#include "Type.h"

//*******************************************************************************************
//* Example:
//*     Step 1: Call CoordinateTrans_SetSizeOriginal(g_LCDSize.uiWidth,g_LCDSize.uiHeight);
//*                 and CoordinateTrans_SetSizeTarget(g_LCDSize.uiWidth,g_LCDSize.uiHeight);
//*                 after OpenPanel() in SysInit_InitLCD();
//*
//*     Step 2: Call CoordinateTrans_SetSizeTarget(g_LCDSize.uiWidth,g_LCDSize.uiHeight);
//*                 after OpenTVNTSC(); / OpenTVPAL();
//*
//*     Setp 3: Start to use TransferRects / InversePosY / InversePox Tools
//*******************************************************************************************

extern void CoordinateTrans_SetSizeOriginal(UINT32 Width, UINT32 Height); ///< Panel Size, Call it at SysInit_InitLCD(), ex Input: g_LCDSize.uiWidth,g_LCDSize.uiHeight;
extern void CoordinateTrans_SetSizeTarget(UINT32 Width, UINT32 Height);   ///< NTSC / PAL / HDMI Size, ex Input: g_LCDSize.uiWidth,g_LCDSize.uiHeight;
extern void CoordinateTrans_TransferRects(RECT* pOutRect,RECT* pInRect,INT nByteSize); ///< Rectangles Translate
extern INT32 CoordinateTrans_InversedPosY(INT32 Value); ///< Target Position X Inverse to Original Position X
extern INT32 CoordinateTrans_InversedPosX(INT32 Value); ///< Target Position Y Inverse to Original Position Y

/**
@}
*/
#endif