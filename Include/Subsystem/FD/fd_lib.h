/** 2007/02/26 yuppie added */
/** last update 2007/02/26  */
/**                         */
//@{
#ifndef _FD_LIB_H
#define _FD_LIB_H
#include "Type.h"

void fd_SetFaceSize(UINT32 Size);
UINT32 fd_GetInitFlag(void);
void fd_SetInitFlag(UINT32 Flag);
void fd_Lock(UINT8 lock);
void fd_SetRate(UINT8 rate);
UINT8 fd_GetRate(void);
UINT8 fd_IsLock(void);
void fd_FaceDetectionProcess(void);
INT32 fd_FaceDetection(UINT32 SrcYAddr, UINT32 SrcCbAddr, UINT32 SrcCrAddr);
UINT32 fd_GetFaceInforAddr(void);
UINT32 fd_GetScreenWidth(void);
UINT32 fd_GetScreenHeight(void);
void fd_SetWorkingBufAddr(UINT32 fd_address);
UINT32 fd_GetWorkingBufSize(void);
void fd_GetResultBufAddr(UINT32 *YAddr, UINT32 *CbAddr, UINT32 *CrAddr, UINT32 *Size);
#endif
//@}
