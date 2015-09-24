/**
    Sample module brief.

    Sample module detailed description.

    @file       filename.h
    @ingroup    Predefined_group_name
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _FDINT_H
#define _FDINT_H

volatile UINT32 uiFDFlags = FALSE;

#define FD_RATE     3
#define FD_BUF_HSIZE   320
#define FD_BUF_VSIZE   240

//#define FD_BUFFER_SIZE        (1.7 * 1024 * 1024)
#define FD_BUFFER_SIZE          0x1C0000 // 1.75MB

#define FD_FLAG_FD_ENABLE   (0x00000001)
#define FD_UNLOCK           (uiFDFlags |=   FD_FLAG_FD_ENABLE)
#define FD_LOCK             (uiFDFlags &= (~FD_FLAG_FD_ENABLE))

#define FD_MAX_SUPPORT_FACE_NUM    6
#define FD_MAX_SUPPORT_FACE_SCALE  9
#define FD_MAX_SUPPORT_FACE_ORIENT AMFPAF_OPF_0_HIGHER

INT32 fd_Init(UINT32 BuffAddr, UINT32 ImgWidth, UINT32 ImgHeight, UINT32 ImgLineOffset, UINT32 FaceSize, UINT32 ImgFmt);
void fd_LoadImg(UINT32 DestAddr, UINT32 SrcYAddr, UINT32 SrcCbAddr, UINT32 SrcCrAddr,
                UINT32 SrcWidth, UINT32 SrcHeight, UINT32 SrcFmt);
UINT32 fd_CheckInitFalg(void);

#endif // EOF
