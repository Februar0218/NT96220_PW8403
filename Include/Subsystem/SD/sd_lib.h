/** 2007/02/26 yuppie added */
/** last update 2007/02/26  */
/**                         */
//@{
#ifndef _SD_LIB_H
#define _SD_LIB_H
#include "Type.h"
#include "Arcsoft_Face_Tracking.h"
#include "arcsoft_smile_detection.h"

#define SD_RATE     1

#define SD_FLAG_SD_ENABLE   (0x00000001)
#define SD_UNLOCK           (uiSDFlags |=   SD_FLAG_SD_ENABLE)
#define SD_LOCK             (uiSDFlags &= (~SD_FLAG_SD_ENABLE))

UINT32 sd_CheckInitFalg(void);
void sd_setInitFlag(UINT32 Flag);
void sd_Lock(UINT8 lock);
void sd_SetRate(UINT8 rate);
UINT8 sd_GetRate(void);
UINT8 sd_IsLock(void);
UINT32 sd_GetResult(void);
void sd_SmileDetectionProcess(void);
INT32 sd_Init(UINT32 BufAddr, UINT32 ImgWidth, UINT32 ImgHeight, UINT32 ImgLineOffset, UINT32 ImgFmt);
INT32 sd_SmileDetection(UINT32 ImgYAddr, UINT32 ImgCbAddr, UINT32 ImgCrAddr, LPAMFPAF_FACERES pFcaceRes, ASSD_ORIENTCODE *pFaceOrient);

#endif
//@}
