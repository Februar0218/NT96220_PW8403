/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       SlideEffect.h
    @ingroup    mISYSPlaySS

    @brief      All function for Slide effect
                This module can be a plug-in to Application Playback

    @note       Nothing.
*/

/** \addtogroup mISYSPlaySS */
//@{

#ifndef _SLIDEEFFECT_H
#define _SLIDEEFFECT_H

#include "Type.h"
#include "JpgDec.h"
#include "OSCommon.h"

//-------------------------------------------------------------------------
#define SLIDE_EFFECT_2010SPRING             DISABLE

//-------------------------------------------------------------------------
//#NT#2007/06/12#Hideo Lin -begin
//#NT#Modified to use different slide effect steps for TV/LCD output.
//#define SLIDE_EFFECT_IMAGE_STEP               8       // 8 step
extern UINT16 SLIDE_EFFECT_IMAGE_STEP;
extern void SlideEffect_SetStep(UINT16 step);
//#NT#2007/06/12#Hideo Lin -end

//#NT#2009/11/24#Scottie -begin
//#NT#Refine code & add SLIDE_EFFECT_IDXVIEW_IN2 for 2nd stage of index view
typedef enum
{
    SLIDE_EFFECT_NONE,                  ///<0x00, No effect
    SLIDE_EFFECT_IN2OUT,                ///<0x01, Move image from in to out
    SLIDE_EFFECT_SLIDEINLEFT2RIGHT,     ///<0x02, Move image from left to right with slide in effect
    SLIDE_EFFECT_SLIDEINRIGHT2LEFT,     ///<0x03, Move image from right to left with slide in effect
    SLIDE_EFFECT_PUSHUP2DOWN,           ///<0x04, Move image from up to down with push effect
    SLIDE_EFFECT_PUSHDOWN2UP,           ///<0x05, Move image from down to up with push effect
    SLIDE_EFFECT_SLIDEINPERFECTFIX,     ///<0x06, Move image with O's perfect fix effect
    SLIDE_EFFECT_SLIDEINPERFECTFIXV,    ///<0x07, Move image with O's perfect fix effect for rotated image
    SLIDE_EFFECT_FADEBLOCKCHECKER,      ///<0x08, Move image with O's erase image effect
    SLIDE_EFFECT_IDXVIEW_IN,            ///<0x09, Move image with O's index view effect (enter 1st stage)
    SLIDE_EFFECT_IDXVIEW_CHANGE,        ///<0x0A, Move image with O's index view effect (change)
    SLIDE_EFFECT_IDXVIEW_OUT,           ///<0x0B, Move image with O's index view effect (exit)
    SLIDE_EFFECT_SCALEUP2DOWN,          ///<0x0C, Move image with O's slide show
    SLIDE_EFFECT_IDXVIEW_IN2,           ///<0x0D, Move image with O's index view effect (enter 2nd stage)

    SLIDE_EFFECT_RANDOM,                ///<MAX number of Slide Effect, Move image with all above effect in sequence
    ENUM_DUMMY4WORD(SLIDE_EFFECT)
} SLIDE_EFFECT;
//#NT#2009/11/24#Scottie -end

#define SLIDE_EFFECT_FIRST                  0x01
#define SLIDE_EFFECT_LAST                   SLIDE_EFFECT_RANDOM - 1

#define SLIDE_EFFECT_MAX                    SLIDE_EFFECT_RANDOM

#define SLIDE_EFFECT_OK                     0
#define SLIDE_EFFECT_EMPTY                  1

//-----------------------------------------------------------------------------------
typedef struct _SLIDE_DISPLAY_OBJ
{
    UINT32      FSrcBufAddrY;              // the frame-buffer-addr-Y  of this display device
    UINT32      FSrcBufAddrCb;             // the frame-buffer-addr-Y  of this display device
    UINT32      FSrcBufAddrCr;             // the frame-buffer-addr-Y  of this display device
    UINT32      FDstBufAddrY;              // the frame-buffer-addr-Y  of this display device
    UINT32      FDstBufAddrCb;             // the frame-buffer-addr-Y  of this display device
    UINT32      FDstBufAddrCr;             // the frame-buffer-addr-Y  of this display device
    UINT32      FTmpBuf1AddrY;             // the frame-buffer-addr-Y  of this display device
    UINT32      FTmpBuf2AddrY;             // the frame-buffer-addr-Y  of this display device
    UINT32      FTmpBuf3AddrY;             // the frame-buffer-addr-Y  of this display device
} SLIDE_DISPLAY_OBJ, *PSLIDE_DISPLAY_OBJ;


typedef struct _SLIDE_EFFECT_OBJ{
    UINT32  SlideEffectIndex;
    INT32 (*SlideEffectFunc)(UINT32 ImgWidth_Y, UINT32 ImgHeight_Y, UINT32 FileFormat, SLIDE_DISPLAY_OBJ buf,
                             UINT8 *SpeedTable, INT32 (*fpSpecEffect)(INT32));
    INT8* SlideEffectName;
} SLIDE_EFFECT_OBJ,*PSLIDE_EFFECT_OBJ;

typedef struct _SLIDE_PERFECT_FIX_
{
    UINT32  ui1stDelay_ms;
    UINT32  ui2ndDelay_ms;
    UINT32  ui3rdDelay_ms;
} SLIDE_PFIX_Time, *PSLIDE_PFIX_Time;

//-----------------------------------------------------------------------------------
extern SLIDE_EFFECT_OBJ g_SlideEffectTable[];
extern void SE_SetSlideSpeed(UINT32 uiSpeed, PSLIDE_PFIX_Time pPFTime);

//-----------------------------------------------------------------------------------
//#NT#2011/07/06#Elvis Chuang -begin
//#NT#Add the Slide Effect Version & Build Date Information
extern CHAR *SE_GetVerInfo_Default(void);
extern CHAR *SE_GetBuildDate_Default(void);

extern CHAR *SE_GetVerInfo_FadeBlock(void);
extern CHAR *SE_GetBuildDate_FadeBlock(void);

extern CHAR *SE_GetVerInfo_In2Out(void);
extern CHAR *SE_GetBuildDate_In2Out(void);

extern CHAR *SE_GetVerInfo_IdxView(void);
extern CHAR *SE_GetBuildDate_IdxView(void);

extern CHAR *SE_GetVerInfo_PushDown2Up(void);
extern CHAR *SE_GetBuildDate_PushDown2Up(void);

extern CHAR *SE_GetVerInfo_PushUp2Down(void);
extern CHAR *SE_GetBuildDate_PushUp2Down(void);

extern CHAR *SE_GetVerInfo_ScaleUp2Down(void);
extern CHAR *SE_GetBuildDate_ScaleUp2Down(void);

extern CHAR *SE_GetVerInfo_SlideinLeft2Right(void);
extern CHAR *SE_GetBuildDate_SlideinLeft2Right(void);

extern CHAR *SE_GetVerInfo_SlideinPerfectFix(void);
extern CHAR *SE_GetBuildDate_SlideinPerfectFix(void);

extern CHAR *SE_GetVerInfo_SlideinPerfectFixV(void);
extern CHAR *SE_GetBuildDate_SlideinPerfectFixV(void);

extern CHAR *SE_GetVerInfo_SlideinRight2Left(void);
extern CHAR *SE_GetBuildDate_SlideinRight2Left(void);

//#NT#2011/07/06#Elvis Chuang -end

#endif

//@}
