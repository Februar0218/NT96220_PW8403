/**
    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.

    @file       UserEXIF.c
    @ingroup    mIPRJAPCfg

    @brief      User defined EXIF tags
                User defined EXIF tags

    @note       Nothing.

    @date       2010/12/22
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#include "UserEXIF.h"
#include "Exif.h"
#include "ExifRW.h"
#include "Debug.h"
#include "UIFlow.h"


//static char   g_cTestString[20] = "I am a cat.";
//static UINT32 g_uiDistance[2] = {30, 1};
//static UINT32  g_uiconstant=1;


void UserEXIF_InsertTags(void)
{
#if 0
    /* Example for adding EXIF tags */
    EXIF_INSERTDATATAG  insertTagD;
    EXIF_INSERTPTRTAG   insertTagP;


    // Example 1: add an ASCII type tag
    insertTagP.tagPos       = EXIF_ExifIFD;
    insertTagP.tagId        = 0x8824; // Spectral Sensitivity
    insertTagP.tagType      = TypeASCII;
    insertTagP.ptagDataAddr = (UINT32 *)g_cTestString;
    insertTagP.tagLen       = strlen(g_cTestString);
    ExifW_InsertPtrTag(&insertTagP, 0);

    // Example 3: add a RATIONAL type tag
    insertTagP.tagPos       = EXIF_ExifIFD;
    insertTagP.tagId        = 0x9206; // Subject Distance
    insertTagP.tagType      = TypeRATIONAL;
    insertTagP.ptagDataAddr = (UINT32 *)g_uiDistance;
    insertTagP.tagLen       = 1;
    ExifW_InsertPtrTag(&insertTagP, 0);
    /*
    // Example 3: add a SHORT type tag
    insertTagD.tagPos       = EXIF_ExifIFD;
    insertTagD.tagId        = 0xA408; // Contrast
    insertTagD.tagType      = TypeSHORT;
    insertTagD.tagDataValue = 0; // normal
    insertTagD.tagLen       = 1;
    ExifW_InsertDataTag(&insertTagD, 0);
    */
    // Example 3: add a SHORT type tag
    insertTagP.tagPos       = EXIF_ExifIFD;
    insertTagP.tagId        = 0xA408; // Contrast
    insertTagP.tagType      = TypeSHORT;
    insertTagP.ptagDataAddr = (UINT32 *)&g_uiconstant; // normal
    insertTagP.tagLen       = 1;
    ExifW_InsertPtrTag(&insertTagP, 0);
#endif
}

void UserEXIF_ModifyTags(void)
{
    EXIF_MODIFYPTRTAG  modifyTag;
#if 0
    /* Example for modifying added EXIF tags */
    EXIF_MODIFYDATATAG  modifyDataTag;


    // Example 1: modify an ASCII type tag
    modifyTag.tagId         = 0x8824; // Spectral Sensitivity
    modifyTag.ptagDataAddr  = (UINT32 *)g_cTestString2;
    modifyTag.tagLen        = strlen(g_cTestString2);
    ExifW_ModifyInsertPtrTag(&modifyTag);



    // Example 2: modify a RATIONAL type tag
    modifyTag.tagId         = 0x9206; // Subject Distance
    g_uiDistance[0] = 20;
    modifyTag.ptagDataAddr  = (UINT32 *)g_uiDistance;
    modifyTag.tagLen        = 1;
    ExifW_ModifyInsertPtrTag(&modifyTag);



    // Example 3: modify a SHORT type tag
    modifyDataTag.tagId         = 0xA408; // Contrast
    modifyDataTag.tagDataValue  = (UINT32)2; // hard
    modifyDataTag.tagLen        = 1;
    ExifW_ModifyInsertDataTag(&modifyDataTag);

#endif

    // User Comment
    modifyTag.tagId = TagUserComment;
    modifyTag.ptagDataAddr = (UINT32 *)SysGetSocialNetworkData();
    modifyTag.tagLen = TagUserCommentLen;
    ExifW_ModifyPtrTag(&modifyTag);
}

//@}
