/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
and confidential information.

The information and code contained in this file is only for authorized ArcSoft
employees to design, create, modify, or review.

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy,
distribute, modify, or take any action in reliance on it.

If you have received this file in error, please immediately notify ArcSoft and
permanently delete the original and any copy of any file and any printout
thereof.
*******************************************************************************/

#ifndef _ASBD_BLINKDETECTION_
#define _ASBD_BLINKDETECTION_

#include "amcomdef.h"
//#include "merror.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef MVoid*          ASBD_PIXELARRAY;
typedef MVoid**         ASBD_ENGINE;
typedef MLong           ASBD_ORIENTCODE;

#define ASBD_CALLBACK_USER_ABORT        1
#define MERR_FACE_TOOSMALL              0x7000

/* Color format definition */
enum ASBD_PIXELARRAYFORMAT{
    ASBD_PAF_B8G8R8     = 0x1,  /* BGR24, B8G8R8*/
    ASBD_PAF_B5G6R5     = 0x2,  /* BGR16, B5G6R5*/
    ASBD_PAF_YVYU422    = 0x3,  /* YUV422,YVYU  */
    ASBD_PAF_YUYV422    = 0x4,  /* YUV422,YUYV  */
    ASBD_PAF_YUV420     = 0x5,  /* YUV420,planar*/
    ASBD_PAF_R5G6B5     = 0x6,  /* RGB16, R5G6B5*/
    ASBD_PAF_UYV422     = 0x8,  /* YUV422,UYVY  */
    ASBD_PAF_LPYUV420   = 0xa,  /* YUV420, Line packed */
    ASBD_PAF_GREY       = 0xc,  /* Y only   */
    ASBD_PAF_VYUY422    = 0xe,  /* YUV422,VYUY  */
    ASBD_PAF_YV12       = 0xf   /* YV12, Y... V... U...*/
};

//face orientation code
enum ASBD_OrientCode{
    ASBD_FOC_0          = 0x1,  /* 0 degree */
    ASBD_FOC_90         = 0x2,  /* 90 degree */
    ASBD_FOC_270        = 0x3   /* 270 degree */
};

typedef struct{
    MRECT    *pFaceRectArray;    /* The bounding box of face*/
    ASBD_ORIENTCODE   *pFaceOrientArray;    /* The orientation of face */
    MLong    lFaceNumber;    /* number of faces detected*/
}ASBD_FACEINPUT, *LPASBD_FACEINPUT;

typedef struct{
    MRECT           *pEyeRectArray;     /*The open eyes bounding box array with same length as pFaceRectArray in ASBD_FACEINPUT. Only rect information with flag equal to "1" is valid.*/
    MLong           *pFlagArrary;       /*The detection result flag array with same length as pFaceRectArray in ASBD_FACEINPUT."1" representing eye is open, otherwise the flag is set to "0" */
    MLong           lFaceNumber;        /*The length of pEyeRectArray in RECT unit.*/
    MLong           lEyeNumber;         /*The number of open eyes totally detected.*/
}ASBD_BLINKDETECTIONRESULT, *LPASBD_BLINKDETECTIONRESULT;


typedef struct{
    MLong lWidth;        // Off-screen width
    MLong lHeight;        // Off-screen height
    MLong lPixelArrayFormat;        // Format of pixel array
    union
    {
        struct
        {
            MLong lLineBytes;              // Off-screen linebytes
            MVoid *pPixel;                 // Off-screen pixel buffer
        }chunky;
        struct
        {
            MLong lLineBytesArray[4];      // Off-screen linebytes in each planar
            MVoid *pPixelArray[4];         // Off-screen pixel buffer in each planar
        }planar;
    } pixelArray;
} ASBD_OFFSCREEN, *LPASBD_OFFSCREEN;
/************************************************************************
 * This function is implemented by the caller, registered with
 * any time-consuming processing functions, and will be called
 * periodically during processing so the caller application can
 * obtain the operation status (i.e., to draw a progress bar),
 * as well as determine whether the operation should be canceled or not
 ************************************************************************/
typedef MRESULT (*ASBD_FNPROGRESS) (
    MLong lProgress,    /*[in] Percentage of the current operation.*/
    MVoid *pParam       /*[in] Caller-defined data*/
);


/*Blink Shot*/
/************************************************************************
 * The function used to Initialize the eye detection module.
 * The handle will passed to detection interface.
 ************************************************************************/
MRESULT ASBD_InitBlinkEngine( /* Return face engine handle. If MNull, meaning bad model*/
    MHandle             hMemMgr,    /* [in]  The handle of memory manager*/
    ASBD_ENGINE         *pEngine    /* [out]  Pointer pointing to an blink detection engine*/
);


/************************************************************************
 * The function used to detect eyes states automatically.
 ************************************************************************/
MRESULT ASBD_BlinkDetect(       /* return MOK if success, otherwise fail*/
    MHandle                     hMemMgr,        /* [in]  User defined memory manager*/
    ASBD_ENGINE                 pEngine,        /* [in]  Detect engine*/
    LPASBD_OFFSCREEN            pImginfo,       /* [in]  The original image data*/
    LPASBD_FACEINPUT            pFaceRes,       /* [in]  The face position*/
    LPASBD_BLINKDETECTIONRESULT pEyeRes,        /* [out] The detection result*/
    ASBD_FNPROGRESS             fnCallback,     /* [in]  The callback function*/
    MVoid                       *pParam         /* [in]  Caller-specific data that will be passed into the callback function*/
);


/************************************************************************
 * The function used to release the eye detection module.
 ************************************************************************/

MRESULT ASBD_UninitBlinkEngine(
    MHandle          hMemMgr,   /* [in]  The handle of memory manager*/
    ASBD_ENGINE      *pEngine   /* [in]  Pointer pointing to an ASBD_ENGINE structure containing the data of eye detection engine*/
);


#ifdef __cplusplus
}
#endif

#endif
