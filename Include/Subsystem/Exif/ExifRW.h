/*
    @file       ExifRW.h
    @ingroup    mILIBEXIF

    @brief      exif writer/reader header file
    @version    V1.01.007
    @note       add gExifW_maxLen
    @date       2011/08/30


    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _EXIFRW_H
#define _EXIFRW_H

#include "Type.h"
#include "jpgParseHeader.h"
#include "jpgenc.h"
typedef UINT32  Exif_Offset;

/**
    @name valid tag position

    Valid tag position.
*/
//@{
#define EXIF_0thIFD     1       ///< tag position: zeroth IFD
#define EXIF_1stIFD     2       ///< tag position: 1st IFD
#define EXIF_ExifIFD    3       ///< tag position: exif IFD
#define EXIF_0thIntIFD  4       ///< tag position: zeroth interoperability IFD
#define EXIF_GPSIFD     5       ///< tag position: GPS IFD
//@}

#define ERROR_NO_EXIFIFD 0x8769

/**
    @name valid parameter type

    Valid parameter type.
*/
//@{
#define EXIFW_SETPARAM_BASIC    1       ///< set basic type: enable/disable
#define EXIFW_SETPARAM_GPS      2       ///< set GPS info: on/off
#define EXIFW_SETPARAM_MAX_LEN  3       ///< set max exif header size//2011/08/30 Meg Lin
//@}



/**
    Status type for ExifW_WriteExif() and ExifW_CompleteHeader()
*/
typedef struct {
    UINT32 *ExifAddr; ///< [in] output Exif header address
    UINT32 *ExifSize; ///< [in] avaliable buffer size
} EXIFPARAM;


/**
    EXIF information structure for ExifR_GetTagData()
*/
typedef struct {
    UINT16 tagId;       ///< [in] wanted exif id
    UINT16 tagType;     ///< [out] tag data type
    UINT32 tagLen;      ///< [out] tag data length
    UINT32 tagDataAddr; ///< [out] if length <=4, value. if length > 4, tag data offset (to beginning of file)
} EXIF_GETTAG;
//#NT#2011/06/30#Meg Lin -begin
/**
    Structure for ExifW_ModifyDataTag() and ExifW_ModifyInsertDataTag()
*/
typedef struct {
    UINT32 tagLen;          ///< [in] new tag length (in bytes!!)
    UINT32 tagDataValue;    ///< [in] new tag data value
    UINT16 tagId;           ///< [in] tag id
    UINT16 rev;             ///< reserved
} EXIF_MODIFYDATATAG;
/**
    Structure for ExifW_ModifyPtrTag() and ExifW_ModifyInsertPtrTag()
*/
typedef struct {
    UINT32 tagLen;          ///< [in] new tag length (in bytes!!)
    UINT32 *ptagDataAddr;   ///< [in] new tag data address
    UINT16 tagId;           ///< [in] tag id
    UINT16 rev;             ///< reserved
} EXIF_MODIFYPTRTAG;

/**
    Structure for ExifW_InsertDataTag()
*/
typedef struct {
    UINT32 tagPos;          ///< [in] tag position, EXIF_0thIFD or others
    UINT16 tagId;           ///< [in] tag id, 0x920A or others
    UINT16 tagType;         ///< [in] tag type, TypeBYTE or others
    UINT32 tagLen;          ///< [in] tag length (in bytes!!)
    UINT32 tagDataValue;    ///< [in] tag data value
    Exif_Offset offset;     ///< [out] don't care
} EXIF_INSERTDATATAG;
/**
    Structure for ExifW_InsertPtrTag()
*/
typedef struct {
    UINT32 tagPos;          ///< [in] tag position, EXIF_0thIFD or others
    UINT16 tagId;           ///< [in] tag id, 0x920A or others
    UINT16 tagType;         ///< [in] tag type, TypeBYTE or others
    UINT32 tagLen;          ///< [in] tag length (in bytes!!)
    UINT32 *ptagDataAddr;   ///< [in] tag data addr
    Exif_Offset offset;     ///< [out] don't care
} EXIF_INSERTPTRTAG;
//#NT#2011/06/30#Meg Lin -end


extern void ExifW_ClearInsertTag(void);
extern UINT32   ExifW_WriteExif(EXIFPARAM *pEXIFIMG, UINT32 pThumbImg, UINT32 pHiddenImg);
extern void     ExifW_SetThumb(UINT32 addr, UINT32 size);
extern void     ExifW_SetHidden(UINT32 addr, UINT32 size);
extern void     ExifW_HideQuality(UINT8 value);
extern ER       ExifW_CompleteHeader(EXIFIMGPARAM *pEXIFIMG, PEXIFEncParam pEXIFDCFThumnail, PEXIFEncParam pEXIFHIDDENImg);
extern void     ExifW_SetParam(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);

extern UINT32   ExifR_ReadExif(UINT32 addr);
extern UINT32   ExifR_ParseExif(UCHAR **buf_p, PEXIFHeaderInfo pEXIFHeader);
extern void     ExifR_GetTagData(EXIF_GETTAG *pTag);


//#NT#2011/06/30#Meg Lin -begin
extern ER ExifW_InsertDataTag(EXIF_INSERTDATATAG *pExifTag, UINT32 insertType);
extern ER ExifW_InsertPtrTag(EXIF_INSERTPTRTAG *pExifTag, UINT32 insertType);
extern ER ExifW_ModifyDataTag(EXIF_MODIFYDATATAG *pExifTag);
extern ER ExifW_ModifyPtrTag(EXIF_MODIFYPTRTAG *pExifTag);
extern void ExifW_ModifyInsertDataTag(EXIF_MODIFYDATATAG *pModTag);
extern void ExifW_ModifyInsertPtrTag(EXIF_MODIFYPTRTAG *pModTag);
//#NT#2011/06/30#Meg Lin -end
extern void ExifW_SetSpecialValue(UINT32 addr, UINT32 totalLen);//2011/09/16 Meg

extern void ExifW_SetSpecialValue2(UINT32 addr, UINT32 minusLen);//2011/12/19 Meg

#endif //_EXIFRW_H

