
#ifndef _OSD_SCREEN_DUMP_H
#define _OSD_SCREEN_DUMP_H

#include "Type.h"
#include "SysCfg.h"

#if (_OSD_SCREEN_DUMP_ == ENABLE)

#define     Filename_Len    32

#define     icst_prom0      103
#define     icst_prom1      88
#define     icst_prom2      183
#define     icst_prom3      198

#define YUV_GET_R(y,u,v)    (((INT32)y) + ((INT32)(v)-128) + ((icst_prom0*((INT32)(v)-128))>>8))
#define YUV_GET_G(y,u,v)    (((INT32)y) - ((icst_prom1*((INT32)(u)-128))>>8) - ((icst_prom2*((INT32)(v)-128))>>8))
#define YUV_GET_B(y,u,v)    (((INT32)y) + ((INT32)(u)-128) + ((icst_prom3*((INT32)(u)-128))>>8))


typedef __packed struct _BMPFileHeader_ {
  UINT16   u16Identifier;   //!< Bitmap identifer
  UINT32   uiFileSize;      //!< Bitmap file size
  UINT32   uiReserved;      //!< Reserved
  UINT32   uiFileOffset;    //!< Bitmap file offset
} BMPFileHeader,*pBMPFileHeader;


typedef __packed struct _BMPInfoHeader_ {
  UINT32   uiHeaderSize;        //!< Bitmap Header size
  UINT32   uiWidth;             //!< Bitmap file width
  UINT32   uiHeight;            //!< Bitmap file height
  UINT16   u16Planes;           //!< Bitmap planes
  UINT16   u16BitsPerPix;       //!< Bits per Pixel
  UINT32   uiCompression;       //!< Compression(0:none,1:RLE_8bit,2:RLE_4bit,3:Bitfields)
  UINT32   uiDataSize;          //!< bitmap Data Size
  UINT32   uiHResolution;       //!< HResolution in pixels per meter
  UINT32   uiVResolution;       //!< VResolution in pixels per meter
  UINT32   uiColors;            //!< colors used by this bitmap
  UINT32   uiImportantColors;   //!< Number of important colors
} BMPInfoHeader,*pBMPInfoHeader;

typedef __packed struct _BMPHeader_ {
  BMPFileHeader  bmp_FileHeader;
  BMPInfoHeader  bmp_InfoHeader;
} BMPHeader,*pBMPHeader;


typedef __packed struct _BMPStruct__ {
  UINT32 uiBMPBuffAdr;          //!< BMP file buffer address
  UINT32 uiPICTBuffAdr;         //!< Loaded PICT bitmap data buffer address
  UINT32 uiOSDBitmapBufAddr;    //!< OSD bitmap data buffer address
  UINT32 uiPaletteAddr;         //!< Palette table buffer address
  UINT32 uiOSDDataAddr;         //!< OSD Data buffer address
  BOOL   bzPICTValid;           //!< flags for checking PICT valid or not
  BMPHeader BMP_Header;         //!< BMP header info structure
  char   filename[Filename_Len];
} BMPStruct,*pBMPStruct;


//extern  void    BMP_INITHeaer(void);
//extern  void    BMP_GetPaletteData(void);
extern  void    BMP_MakeFile(char *pFilename);

#endif

#endif //_OSD_SCREEN_DUMP_H
