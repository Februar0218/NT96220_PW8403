  /*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and      
* confidential information. 
* 
* The information and code contained in this file is only for authorized ArcSoft employees 
* to design, create, modify, or review.
* 
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
* 
* If you are not an intended recipient of this file, you must not copy, distribute, modify, 
* or take any action in reliance on it. 
* 
* If you have received this file in error, please immediately notify ArcSoft and 
* permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/

/*************************************************************************************************
**
**  Copyright (c) 2007 by ArcSoft Inc.
**
**  Name            : Arc_autoredeye.h
**
**  Purpose         : A red-eye removal interface. 
**
**  Additional      : 
**
**------------------------------------------------------------------------------------------------
**
**  Maintenance History:
**
**************************************************************************************************/

#ifndef __ARCSOFT_RED_EYE_REMOVE_H__
#define __ARCSOFT_RED_EYE_REMOVE_H__

#include "amcomdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ARER_OK                     (0)
#define ARER_ERR_PARAMETER          (-2)
#define ARER_ERR_NOT_SUPPORT        (-3)
#define ARER_ERR_MEMORY             (-4)
#define ARER_ERR_INTERRUPT          (-7)
#define ARER_ERR_TIMEOUT            (-8)

#define ARER_PAF_RGB565             (8)
#define ARER_PAF_YYCbCr8888         (12)
#define ARER_PAF_YCbYCr8888         (13)
#define ARER_PAF_CbYCrY8888         (14)
#define REDEYE_FMT_YCbCr422_P       (52)


typedef struct _tag_ARER_Version
{
    MLong lMajor;           /* major version            */
    MLong lMinor;           /* minor version            */
    MLong lBuild;           /* platform dependent       */
    MLong lRevision;        /* increasable only         */
    MTChar *Version;        /* version in string format */
    MTChar *BuildDate;      /* latest build Date        */
    MTChar *CopyRight;      /* copyright                */
} ARER_VERSION;


typedef struct _tag_ARER_OFFSCREEN {
    MLong       lWidth;                 /* The width of the data. */
    MLong       lHeight;                /* The height of the data. */
    MLong       lPixelArrayFormat;      /* The color format of the data. */
    union
    {
        struct
        {
            MLong lLineBytes;           /* The pitch of the data. */
            MByte *pPixel;              /* The buffer pointer of the data. */
        } chunky;
        struct
        {
            MLong lLinebytesArray[4];   /* The pitch of the data. */
            MByte *pPixelArray[4];      /* The buffer pointer of the data. */
        } planar;
    } pixelArray;
} ARER_OFFSCREEN, * LPARER_OFFSCREEN;

/************************************************************************
* Description   : This function is implemented by the caller, registered with 
*                 any time-consuming processing functions, and will be called 
*                 periodically during processing so the caller application can 
*                 obtain the operation status (i.e., to draw a progress bar), 
*                 as well as determine whether the operation should be canceled or not.
* Parameters    :
*                 lProgress [in]:
*                       The percentage of the current operation. 10 means 10%, and so on.
*                 pUserParam    [in]:
*                       Caller-defined data. It should be registered when calling 
*                       the process functions and will be passed to the callback 
*                       without modification. The library itself does not use this data at all.
* Return        :
*                 ARER_OK
*                       Everything is OK. Continue further processing.
*                 ARER_ERR_TIMEOUT  
*                       Stop further process after callback returns and return the result of all previous work.
*                 ARER_ERR_INTERRUPT    
*                       Direct the process should be canceled after callback returns.
************************************************************************/
typedef MRESULT (*ARER_FNPROGRESS) (
                                    MLong lProgress,    /* The percentage of the current operation. 10 means 10%, and so on. */
                                    MVoid* pUserParam   /* Caller-defined data. */
);


/*******************************************Routine*Public*****************************************\
* ARER_GetVersion
* Description   : This function tells the SDK version by returning the reference of ARER_VERSION. 
* Parameters    :
* Return        : The address of ARER_VERSION reference.
* Remarks       : N/A
*
\**************************************************************************************************/
const ARER_VERSION *ARER_GetVersion (MVoid);

/*******************************************Routine*Public*****************************************\
* ARER_AutoRemove
* Description   : The function automatically detects and corrects red-eyes of an image in the input image.
* Parameters    :
*               pInputImg   [in/out]:
*                       The memory storing the input image. The library will do the red-eye correction directly on this image buffer.
*               pWorkMem    [in]:
*                       The working buffer for the library to do red-eye detection/correction processing. The caller is responsible for allocating and freeing the memory.
*               nWorkMemSize    [in]:
*                       The size of working memory.
*               fnCallback  [in]:
*                       The CALLBACK function, which is used for interruption when accessing time limitation reached. It could be NULL.
*               pUserParam  [in]:
*                       Caller-specific data that will be passed into the callback function. It could be NULL.
* Return        : 
*               Non-negative number The number of red-eyes corrected.
*               ARER_ERR_PARAMETER  
*                       pWorkMem or pInputImg is NULL or the specification of pInputImg is wrong.
*               ARER_ERR_NOT_SUPPORT    
*                       The pixel array format is not supported by the SDK.
*               ARER_ERR_MEMORY 
*                       Insufficiency of working memory.
*               ARER_ERR_INTERRUPT  
*                       Function was interrupted by callback function.
* Remarks       : 
*               [1] The return value of callback function could be one of the followings:
*                   ARER_OK:            Everything is OK, SDK will continue to do further processing.
*                   ARER_ERR_TIMEOUT:   Maximum processing time reached, SDK will not return this value but stop further processing. For example: if N red-eyes have been found when ARER_ERR_TIMEOUT received, the returned value of this API will be N and these N red-eyes will be corrected before escape processing.
*                   ARER_ERR_INTERRUPT: SDK will totally stop processing and return the same error code that callback returned.
*               [2] If callback function parameter passed to API is MNull, then callback function will be disabled.
\**************************************************************************************************/
MRESULT ARER_AutoRemove (
    LPARER_OFFSCREEN    pInputImg,
    MVoid           *pWorkMem,
    MLong           nWorkMemSize,
    ARER_FNPROGRESS fnCallback,
    MVoid           *pUserParam
);


#ifdef __cplusplus
}
#endif


#endif/*__ARCSOFT_RED_EYE_REMOVE_H__*/ 

