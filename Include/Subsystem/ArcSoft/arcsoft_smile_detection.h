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

#ifndef _ASSD_SMILEDETECTION_
#define _ASSD_SMILEDETECTION_

#include "amcomdef.h"
//#include "merror.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef MVoid*          ASSD_PIXELARRAY;
typedef MVoid**         ASSD_ENGINE;
typedef MLong           ASSD_SMILESENSELEVEL;
typedef MLong           ASSD_ORIENTCODE;

#define ASSD_CALLBACK_USER_ABORT        1
#define MERR_FACE_TOOSMALL              0x7000

/* Color format definition */
enum ASSD_PIXELARRAYFORMAT{
    ASSD_PAF_YUYV422    = 0x4,  /*YUV422,YUYV  */
    ASSD_PAF_YUV420     = 0x5,  /*YUV420,planar*/
    ASSD_PAF_P8YUV422   = 0x9,  /*YUV422, package 8 pixels in one unit*/
    ASSD_PAF_LPYUV420   = 0xa,  /*YUV420 line packaged,Y... VUVU*/
    ASSD_PAF_LPYIUV420= 0xb,    /*YUV420, Line packed,UV inverse, Y... UVUV...*/
    ASSD_PAF_GREY       = 0xc,  /*Y only */
    ASSD_PAF_PYUV422    = 0xd   /*YUV422, planar Y..U..V..*/
};

//face orientation code
enum ASSD_OrientCode{
    ASSD_FOC_0          = 0x1,      /* 0 degree */
    ASSD_FOC_90         = 0x2,      /* 90 degree */
    ASSD_FOC_270        = 0x3       /* 270 degree */
};

/* Smile sensitivity */
enum ASSD_SMILESENSITIVITY{
    ASSD_SENSELEVEL0 = 0x0, /*high sensitivity*/
    ASSD_SENSELEVEL1 = 0x1, /*higher sensitivity(recommend)*/
    ASSD_SENSELEVEL2 = 0x2, /*middle sensitivity*/
    ASSD_SENSELEVEL3 = 0x3, /*lower sensitivity*/
    ASSD_SENSELEVEL4 = 0x4    /*lowest sensitivity*/
};

typedef struct{
    MRECT               *pFaceRectArray;        /* The bounding box of face*/
    ASSD_ORIENTCODE     *pFaceOrientArray;      /* The orientation of face */
    MLong               lFaceNumber;            /* number of faces detected*/
}ASSD_FACEINPUT, *LPASSD_FACEINPUT;

typedef struct{
    MRECT           *pSmileRectArray;   /*The smile bounding box array with same length as pFaceRectArray in ASSD_FACEINPUT.Only rect information with flag equal to "1" is valid.*/
    MLong           *pFlagArrary;       /*The detection result flag array with same length as pFaceRectArray in ASSD_FACEINPUT."1" representing smile is detected, otherwise the flag is set to "0" */
    MLong           lFaceNumber;        /*The length of pSmileRectArray in RECT unit.*/
    MLong           lSmileNumber;       /*The number of smiles totally detected.*/
}ASSD_SMILEDETECTIONRESULT, *LPASSD_SMILEDETECTIONRESULT;

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
} ASSD_OFFSCREEN, *LPASSD_OFFSCREEN;


typedef struct
{
    MLong lCodebase;            /* Codebase version number */
    MLong lMajor;               /* major version number */
    MLong lMinor;               /* minor version number*/
    MLong lBuild;               /* Build version number, increasable only*/
    MTChar Version[20];         /* version in string form */
    MTChar BuildDate[20];       /* latest build Date */
    MTChar CopyRight[50];       /* copyright */
} ArcSoft_Smile_Detection_Version;

/************************************************************************
 * This function is implemented by the caller, registered with
 * any time-consuming processing functions, and will be called
 * periodically during processing so the caller application can
 * obtain the operation status (i.e., to draw a progress bar),
 * as well as determine whether the operation should be canceled or not
 ************************************************************************/
typedef MRESULT (*ASSD_FNPROGRESS) (
    MVoid        *pParam1,               /*[in] Not used*/
    MVoid        *pParam2                /*[in] Caller-defined data*/
);
/*Smile Shot*/
/************************************************************************
 * The function used to Initialize the smile detection module.
 * The handle will passed to detection interface.
 ************************************************************************/
MRESULT ASSD_InitSmileEngine(/* Return face engine handle. If MNull, meaning bad model*/
    MHandle             hMemMgr,    /* [in]  The handle of memory manager*/
    ASSD_ENGINE         *pEngine,   /* [out]  Pointer pointing to a smile detection engine*/
    ASSD_SMILESENSELEVEL    smilelevel      /* [in]  User defined smile sensitivity level,high level corresponding to high sensitivity on open mouth*/
);

/************************************************************************
 * The function used to detect smile on preview mode automatically.
 ************************************************************************/
MRESULT ASSD_SmileDetect(/* return MOK if success, otherwise fail*/
    MHandle                     hMemMgr,        /* [in]  User defined memory manager*/
    ASSD_ENGINE                 pEngine,        /* [in]  Detect engine*/
    LPASSD_OFFSCREEN            pImginfo,       /* [in]  The original image data*/
    LPASSD_FACEINPUT            pFaceRes,       /* [in]  The face position*/
    LPASSD_SMILEDETECTIONRESULT pSmileRes,      /* [out] The detection result*/
    ASSD_FNPROGRESS             fnCallback,     /* [in]  The callback function*/
    MVoid                       *pParam         /* [in]  Caller-specific data that will be passed into the callback function*/
);


/************************************************************************
 * The function used to release the smile detection module.
 ************************************************************************/
MRESULT ASSD_UninitSmileEngine(
    MHandle            hMemMgr,     /* [in]  The handle of memory manager*/
    ASSD_ENGINE        *pEngine     /* [in]  Pointer pointing to an ASSD_ENGINE structure containing the data of smile detection engine*/
);



/************************************************************************
 * The function used to get version information of smile detection library.
 ************************************************************************/
const ArcSoft_Smile_Detection_Version * ArcSoft_Smile_Detection_GetVersion(void);

#ifdef __cplusplus
}
#endif

#endif
