
#ifndef UI_SYSTEMOBJ_H
#define UI_SYSTEMOBJ_H

#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UIFlow.h"


//-----------------------------------------------------------------------------
// Global Definitions
//-----------------------------------------------------------------------------
// Change DSC mode type
#define DSCMODE_CHGTO_NEXT              0
#define DSCMODE_CHGTO_PREV              1
#define DSCMODE_CHGTO_CURR              2

#define ANIMATION_ON_CLOSE                        0
#define ANIMATION_ON_OPEN                         1
//-----------------------------------------------------------------------------
// Type Define
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern VControl UISystemObjCtrl;
extern EVENT_ENTRY UISystemObjCtrlCmdMap[];

extern UINT32   Get_BeepValue(void);
extern UINT32   Get_VolumeValue(void);
extern UINT32   Get_PowerOffValue(void);
extern UINT32   Get_FreqValue(void);
extern UINT32   Get_LanguageValue(UINT32 uiIndex);

extern int SysGetCaptureAddr(UINT32 *addr);
extern int SysGetRamDiskAddr(UINT32 *addr);
extern UINT32   SysGetDispRotateBuffSize(void);
extern int      SysGetDispRotateAddr(UINT32 *addr);
extern int      SysGetSICDAddr(UINT32 *addr);
extern int      SysGetPTPObjectAddr(UINT32 *addr);
extern int      SysGetFdSdAddr(UINT32 *addr);
extern int      SysGetDateStampAddr(UINT32 *addr);
extern int      SysGetDateStampThumbAddr(UINT32 *addr);
extern int      SysGetPhotoEndAddr(UINT32 *addr);
extern int      SysGetMovieDSAddr(UINT32 *addr);
extern int      SysGetMovieDSAddr_2nd(UINT32 *addr);

#endif