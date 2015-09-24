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

#ifndef _ARC_FACETRACKING_H_
#define _ARC_FACETRACKING_H_

#include "amcomdef.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef MVoid*                AMFPAF_PIXELARRAY;
typedef MVoid**                AMFPAF_ENGINE;
typedef MLong                AMFPAF_PIXELARRAYFORMAT;
typedef MLong                AMFPAF_ORIENTPRI;
typedef MLong                AMFPAF_ORIENTCODE;
typedef MLong                AMFPAF_FACEOUTPRI;

// Color format definition
enum PixelArrayFormat{
    AMFPAF_PAF_YUV422        = 0x4,        // YUV422, YUYV
    AMFPAF_PAF_YUV420        = 0x5,        // YUV420,planar
    AMFPAF_PAF_UYV422        = 0x8,        // YUV422,UYVY
    AMFPAF_PAF_P8YUV422        = 0x9,        // YUV422, package 8 pixels in one unit
    AMFPAF_PAF_LPYUV420        = 0xa,        // YUV420, Line packed
    AMFPAF_PAF_PYUV422        = 0xd        // YUV422, planar Y..U..V..
};

//orientation priority
enum OrientPriority{
    AMFPAF_OPF_EQUAL        = 0x1,        // equal priority: 0,90,270,0,90,270...
    AMFPAF_OPF_0_HIGHER        = 0x2,        // 0 higher priority: 0,90,0,270,0,90...
    AMFPAF_OPF_0_ONLY        = 0x3,        // 0 only:0,0...
    AMFPAF_OPF_90_ONLY        = 0x4,        // 90 only:90,90...
    AMFPAF_OPF_270_ONLY        = 0x5,        // 270 only:270,270...
    AMFPAF_OPF_USER_DEFINED    = 0x6        // user defined priority
};

//face orientation code
enum OrientCode{
    AMFPAF_FOC_0            = 0x1,        // 0 degree
    AMFPAF_FOC_90            = 0x2,        // 90 degree
    AMFPAF_FOC_270            = 0x3        // 270 degree
};

/*Face output priority */
enum FaceOutPriority{
    AMFPAF_FOP_SIMILARITY    = 0x0,        // similarity priority
    AMFPAF_FOP_CENTER        = 0x1,        // center priority
    AMFPAF_FOP_SIZE          = 0x2         // size priority
};

typedef struct{
    MRECT           *rcFace;                // The bounding box of face
    MLong           nFace;                  // number of faces detected
    MLong           lfaceOrient;            // the orient of the face
} AMFPAF_FACERES, *LPAMFPAF_FACERES;


typedef struct{
    MLong                lWidth;                // image width
    MLong                lHeight;               // image height
    MLong                lPixelArrayFormat;     // Format of pixel array
    MLong                lLineBytes;            // Line width of image in bytes
} AMFPAF_IMAGEINFO, *LPAMFPAF_IMAGEINFO;

typedef struct
{
    MLong lCodebase;            // Codebase version number
    MLong lMajor;                // major version number
    MLong lMinor;                // minor version number
    MLong lBuild;                // Build version number, increasable only
    const MTChar *Version;        // version in string form
    const MTChar *BuildDate;    // latest build Date
    const MTChar *CopyRight;    // copyright
} ArcSoft_Face_Tracking_Version;

/************************************************************************
 * This function is implemented by the caller and will be called
 * when faces are detected.
 ************************************************************************/
typedef MRESULT (*AMFPAF_FNPROGRESS) (
    MVoid       *pParam1,                    // detection result data.
    MVoid        *pParam2                    // Caller-defined data
);

/************************************************************************
 * The function used to detect and track face automatically.
 *
 * Comment:
 *    The incoming image is scanned for faces.The result pFaceRes will be
 *  passed to this interface in the next calling round.
 ************************************************************************/
MRESULT AMFPAF_FaceFeatureDetect(                    // return MOK if success, otherwise fail
    MHandle                hMemMgr,                    // [in]  User defined memory manager
    AMFPAF_ENGINE        pEngine,                    // [in]     Face Tracking engine
    AMFPAF_PIXELARRAY    pImgData,                    // [in]  The original image data
    LPAMFPAF_FACERES    pFaceRes,                    // [out] The tracking result
    AMFPAF_ORIENTCODE    iOrient,                    // [in]  The orientation of face to detect
    AMFPAF_FNPROGRESS    fnGetPartialFaceRes,        // [in]  The callback function to get detection result earlier.
    MVoid                *pGetPartialFaceResParam,    // [in]  Caller-specific data that will be passed into the callback function fnGetPartialFaceRes.
    AMFPAF_FNPROGRESS    fnTimeOut,                    // [in]  The callback function to terminate detection interface.
    MVoid                *pTimeOutParam                // [in]  Caller-specific data that will be passed into the callback fnTimeOut.
);

/************************************************************************
 * The function used to Initialize the face tracking engine.
 ************************************************************************/
MRESULT AMFPAF_InitialFaceEngine(
    MHandle                hMemMgr,        // [in]    User defined memory manager
    LPAMFPAF_IMAGEINFO    pImgData,        // [in] Pointing to the AMFPAF_IMAGEINFO structure containing input image information
    LPAMFPAF_FACERES    pFaceRes,        // [out] The tracking result
    AMFPAF_ENGINE        *pEngine,        // [out] pointing to the AMFPAF_ENGINE structure containing the tracking engine.
    AMFPAF_ORIENTPRI    iOrientsFlag,    // [in] Defining the priority of face orientation.
    MLong                nScale,            // [in]    An integer defining the minimal face to detect relative to the maximum of image width and height.
    MLong                nMaxFaceNum        // [in]    An integer defining the number of max faces to track
);

/************************************************************************
 * The function used to Uninitialize the detection module.
 ************************************************************************/
MRESULT AMFPAF_UninitialFaceEngine(
    MHandle    hMemMgr,                    // [in]    User defined memory manager
    AMFPAF_ENGINE *pEngine,                // [in] pointing to the AMFPAF_ENGINE structure containing the tracking engine.
    LPAMFPAF_FACERES pFaceRes              // [in] The tracking result
);

/************************************************************************
 * The function used to set face priority.
 ************************************************************************/
MRESULT AMFPAF_SetFaceOutPriority(
     AMFPAF_ENGINE    hEngine,            // [in]     Face Tracking engine
     MLong            lPrioity);            // [in]  Face priority

/************************************************************************
 * The function used to set the number of frames to be skipped between two successive whole frame detections .
 ************************************************************************/
MRESULT AMFPAF_SetTrackingFrameNumber(
     AMFPAF_ENGINE    hEngine,            // [in ] Face Tracking engine
     MLong nFrames);                    // [in]  number of frames to be tracked

/************************************************************************
 * The function used to get version information of face tracking library.
 ************************************************************************/
const ArcSoft_Face_Tracking_Version * ArcSoft_Face_Tracking_GetVersion(void);


#ifdef __cplusplus
}
#endif

#endif //_ARC_FACETRACKING_H_
