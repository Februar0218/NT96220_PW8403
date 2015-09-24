
//    Data
/*
    IMAGE : Image
        BITMAP : Image with Bitmap type object, contain memory for bitmap image (WORD align)
        ICON : Image with Icon type object, contain memory for icon image (bit-compacted)
        JPEG : Image with Jpeg stream type object, contain memory for jpeg stream image (only for Blt)

    STRING : String
        TCHAR : General string
        WCHAR : Wide char (2-byte) string
*/

#ifndef _GX_DATA_H_
#define _GX_DATA_H_


#include "GxCommon.h"
#include "GxDC.h"
#include "GxRect.h"

#ifdef __cplusplus
extern "C" {
#endif

//GxData feature:
#define GX_SUPPORT_WCHAR        1

//--------------------------------------------------------------------------------------
//  type - image
//--------------------------------------------------------------------------------------
typedef struct _IMAGE
{
    DC          dc;
    RECT_2D     rect;
//#NT#2007/5/23#JeahYen -begin
    UINT32      uiPixelSize;
    void*       pDynamicBuf;
//#NT#2007/5/23#JeahYen -end
}
IMAGE;

//--------------------------------------------------------------------------------------
//  function - image
//--------------------------------------------------------------------------------------
SIZE_2D IMAGE_GetSize           (const IMAGE* pImage);
RESULT IMAGE_GetActiveRect      (const IMAGE* pImage, RECT_2D* pRect);
RESULT IMAGE_SetActiveRect      (IMAGE* pImage, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
__inline DC* IMAGE_GetDC        (IMAGE* pImage)
{
    if(pImage == 0)
        return 0;
    return &(pImage->dc);
}

//--------------------------------------------------------------------------------------
//  type - string
//--------------------------------------------------------------------------------------
//char types:
typedef unsigned short          WCHAR;  //16-bits char
typedef CHAR                    TCHAR;  //8-bits char

//common char access type (maximal volume of all support char types)
typedef WCHAR                   TCHAR_VALUE;

#define _CHAR_T(str)            (str)
#define _WCHAR_T(str)           (L ## str)

#if (GX_SUPPORT_WCHAR)
#define CHAR_TYPE               WCHAR
#define _T(str)                 _WCHAR_T(str)
#else
#define CHAR_TYPE               TCHAR
#define _T(str)                 _CHAR_T(str)
#endif  

//<enum>
#define FMT_WCHAR               ((WCHAR)(0xFEFF))
#define FMT_WCHAR_1ST           ((CHAR)(0xFF))  //LOW BYTE
#define FMT_WCHAR_2ND           ((CHAR)(0xFE))  //HIGH BYTE
#define FMT_WCHAR_SIZE          1 //one WCHAR

//--------------------------------------------------------------------------------------
//  function - string
//--------------------------------------------------------------------------------------
INT16 STRING_GetLength          (const TCHAR* pszSrc);


//--------------------------------------------------------------------------------------
//  type - image table
//--------------------------------------------------------------------------------------
#ifndef IVALUE
typedef UINT16                  IVALUE; //table index value
#endif
typedef UINT32                  IMAGE_TABLE;

//--------------------------------------------------------------------------------------
//  function - image table
//--------------------------------------------------------------------------------------
RESULT IMAGE_LoadFromTable      (IMAGE* pImage, const IMAGE_TABLE* pTable, IVALUE id);
RESULT IMAGE_GetSizeFromTable   (const IMAGE_TABLE* pTable, IVALUE id, SIZE_2D* pImageSize);

//--------------------------------------------------------------------------------------
//  function - image JPG
//--------------------------------------------------------------------------------------
RESULT IMAGE_LoadFromJPG        (IMAGE* pImage, UINT32 uiJPGData, UINT32 uiJPGSize);
RESULT IMAGE_GetSizeFromJPG     (UINT32 uiJPGData, UINT32 uiJPGSize, SIZE_2D* pImageSize);

//--------------------------------------------------------------------------------------
//  type - string table
//--------------------------------------------------------------------------------------
#ifndef IVALUE
typedef UINT16                  IVALUE; //table index value
#endif
typedef UINT32                  STRING_TABLE;
//--------------------------------------------------------------------------------------
//  function - string table
//--------------------------------------------------------------------------------------
const TCHAR*  
STRING_LoadFromTable            (const STRING_TABLE* pTable, IVALUE id);
RESULT STRING_GetLengthFromTable(const STRING_TABLE* pTable, IVALUE id, INT16* pStringLen);

//--------------------------------------------------------------------------------------
//  type - font
//--------------------------------------------------------------------------------------
typedef UINT32  FONT;
//--------------------------------------------------------------------------------------
//  function - font
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
//  type - drawing table
//--------------------------------------------------------------------------------------
#ifndef IVALUE
typedef UINT16                  IVALUE; //table index value
#endif
typedef UINT32                  DRAWING_TABLE;

//--------------------------------------------------------------------------------------
//  function - drawing table
//--------------------------------------------------------------------------------------


#ifdef __cplusplus
} //extern "C"
#endif

#endif//_GX_DATA_H_