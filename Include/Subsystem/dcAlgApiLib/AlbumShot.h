/**
    Public header file for album shot mode.

    This file is the header file that define the API and data type for album
    shot mode.

    @file       AlbumShot.h
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _ALBUM_SHOT_H
#define _ALBUM_SHOT_H

#include "ImageAlgInfor.h"
#include "Debug.h"


/** \addtogroup mISYSAlg */
//@{

#define ALBUMSHOT_MAX_NUM 3
typedef struct ALBUMSHOT_IMG_INFO_STRUCT
{
    UINT32 PrvOfsY;
    UINT32 PrvOfsCbCr;
    UINT32 PrvImgH;
    UINT32 PrvImgV;
    UINT32 ThumbImgAddr;
    UINT32 CapImgAddr;
    UINT32 CapOfsY;
    UINT32 CapOfsCbCr;
    UINT32 CapImgH;
    UINT32 CapImgV;
    UINT32 BackGroundColor[3];
    UINT32 FrameColor[3];
    UINT32 PrvFrameH;
    UINT32 PrvFrameV;
    UINT32 CapFrameH;
    UINT32 CapFrameV;
}ALBUMSHOT_IMG_INFO;

typedef struct ALBUMSHOT_INFO_STRUCT
{
    UINT32 DispBufAddrY[3];
    UINT32 DispBufAddrCb[3];
    UINT32 DispBufAddrCr[3];
    UINT32 BackGroundColor[3];
    UINT32 DispSizeV;
    UINT32 DispSizeH;
    UINT32 CapSizeV;
    UINT32 CapSizeH;
    UINT32 CapBufAddr;
    UINT32 CurIdx;
    ALBUMSHOT_IMG_INFO ImgInfo[ALBUMSHOT_MAX_NUM];

} ALBUMSHOT_INFO;

/**
    @name display type definition.
    @brief Contant symbols for display type.
*/
//@{
//#NT#2009/10/29#Bowen Li -begin
//#NT#expand enum size to word (4 bytes)
typedef enum {
    DISP_NORMAL = 0,        ///< multi-display mode.
    DISP_FULL,              ///< single-display mode.
    DISP_TOTAL,
    ENUM_DUMMY4WORD(ALBUM_DISP_TYPE)
} ALBUM_DISP_TYPE;
//#NT#2009/10/29#Bowen Li -end
//@}

extern ALBUMSHOT_INFO AlbumInfo;

void ASModeStart(void);
void ASModeReset(void);
void ASModeEnd(void);
void ASPrvBGInit(void);
void ASPrvParamInit(void);
void ASDrawDispWindow(UINT32 WinIdx);
void ASSetDispWindow(UINT32 WinIdx, UINT32 WaitCmdEnd);
void ASImgProcess(UINT32 Idx);
void ASCapParamInit(void);
void ASCapCounterReset(void);
void ASCapCounterAdd(void);
UINT32 ASGetCapCounter(void);
UINT32 ASGetMaxCounter(void);
void ASPasteThumbnail(void);
//#NT#2008/08/21#Bowen Li -begin
//#NT#add ASModeIsEnable function
UINT32 ASModeIsEnable(void);
//#NT#2008/08/21#Bowen Li -end

//#NT#2008/09/09#Bowen Li -begin
//#NT#modify albumshot buffer judgement
void ASGetBufRatio(UINT32 *Numerator, UINT32 *Denominator);
//#NT#2008/09/09#Bowen Li -end

//#NT#2008/07/11#Bowen Li -begin
//#NT#modify album shot interface
void ASFormJpgFile(UINT32 *JpgAddr, UINT32 *JpgSize);
//#NT#2008/07/11#Bowen Li -end

//#NT#2008/09/10#Bowen Li -begin
//#NT#add ASChgDisplayType function
void ASSetDispPos(UINT32 Idx);
void ASSetDispFull(void);
void ASChgDisplayType(ALBUM_DISP_TYPE DispType);
//#NT#2008/09/10#Bowen Li -end

//@}

#endif
