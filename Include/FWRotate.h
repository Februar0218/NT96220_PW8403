
#ifndef _FWROTATE_H_
#define _FWROTATE_H_

#include "Type.h"
#include "disp_rotate.h"
#include "ImageAlgInfor.h"

enum _IME_PATH {
    IME_PATH1,
    IME_PATH2,
    IME_DCOUT
};

typedef struct  _FW_RT_IMG_INFO
{
    UINT32  uiImg_Width;        // Image width, Remark: this value is signed.
    UINT32  uiImg_Height;       // Image Height, Remark: this value is signed.
    UINT32  uiOld_Y_Addr;       // Original Y address. If NULL, the operation will not done in Y component.
    UINT32  uiOld_Cb_Addr;      // Original Cb address
    UINT32  uiOld_Cr_Addr;      // Original Cr address

    UINT32  uiNew_Y_Addr;       // New Y address, this address could not the same as old value!
    UINT32  uiNew_Cb_Addr;      // New Cb address.
    UINT32  uiNew_Cr_Addr;      // New Cr address.

    UINT32  uiRotateDir;
    UINT32  uiImgFormt;
} FW_RT_IMG_INFO;



#define FLGFWRT_IME_END                 0x00000001
#define FLGFWRT_IME_PATH1_END           0x00000002
#define FLGFWRT_IME_PATH2_END           0x00000004
#define FLGFWRT_IME_DCOut_END           0x00000008
#define FLGFWRT_IME_RECORD_END          0x00000010
#define FLGFWRT_SET_IDLE                0x00000020
#define FLGFWRT_IDLING                  0x00000040


extern  void    FWRotate_SetStatus(DISPROT_DIR Rot_Dir);
extern  BOOL    FWRotate_GetStatus(void);
extern  void    FWRotate_SetVideoAttrib(IDE_VIDEOID VideoID,VIDEO_BUF_ADDR* FB_Addr,VIDEO_BUF_ATTR* FB_Attr,VOSD_WINDOW_ATTR* FB_Win);
extern  void    FWRotate_SetBufIdx(UINT32 uiBufIdx);
extern  void    FWRotate_SetIMEPara(IME_FBADDR_PRAM* pIMEParam);
extern  void    FWRotate_Open(void);
extern  void    FWRotate_Close(void);
extern  void    FWRotate_Tsk(void);

//#NT#2012/7/24#Philex - begin
// add API to Get/Set Using GxGfx cup library status
void    FWRotate_SetGxCpuStatus(BOOL bUseGxCpu);
BOOL    FWRotate_GetGxCpuStatus(void);
//#NT#2012/7/24#Philex - end
#endif//_FWROTATE_H_

