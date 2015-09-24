/*
    Header file for IDE driver backward compatibility

    Header file for IDE driver backward compatibility.

    @file       ide_compat.h
    @ingroup    mIDISPIDE
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IDE_COMPAT_H
#define _IDE_COMPAT_H

#include "Type.h"

/**
    @addtogroup mIDISPIDE
*/
//@{

//-------------------------------------------------
// IDE Compatible driver API
//-------------------------------------------------
#define ide_setreg_osd1_en(b_En)            ide_setO1En(b_En)
#define ide_setreg_osd2_en(b_En)            ide_setO2En(b_En)
#define ide_setreg_video1_en(b_En)          ide_setV1En(b_En)
#define ide_setreg_video2_en(b_En)          ide_setV2En(b_En)
#define ide_get_window_en()                 ide_getWindowEn()
#define ide_set_allwindow_dis()             ide_setAllWindowDis()
#define ide_set_allwindow_en(uiWins)        ide_setAllWindowEn(uiWins)
#define ide_set_icst(b_sel)                 do{ }while(0)
#define ide_set_dithering(b_en, b_freerun)  ide_setDithering(b_en, b_freerun)
#define ide_set_device(uidevice)            ide_setDevice(uidevice)
#define ide_get_device()                    ide_getDevice()
#define ide_set_pdir(b_pdir)                ide_setPdir(b_pdir)
#define ide_set_odd(uiODD)                  ide_setOdd(uiODD)
#define ide_set_even(uiEVEN)                ide_setEven(uiEVEN)
#define ide_set_hsinv(b_inv)                ide_setHsInv(b_inv)
#define ide_set_vsinv(b_inv)                ide_setVsInv(b_inv)
#define ide_set_hvldinv(b_inv)              ide_setHvldInv(b_inv)
#define ide_set_vvldinv(b_inv)              ide_setVvldInv(b_inv)
#define ide_set_clkinv(b_inv)               ide_setClkInv(b_inv)
#define ide_set_fieldinv(b_inv)             ide_setFldInv(b_inv)
#define ide_set_rgbdummy(b_RGBDsel)         ide_setRgbd(b_RGBDsel)
#define ide_get_rgbdummy()                  ide_getRgbd()
#define ide_set_reset()                     ide_setReset()
#define ide_set_through_mode(b_en)          ide_setThrough(b_en)
#define ide_get_through_mode()              ide_getThrough()
#define ide_set_shw_st()                    do{ }while(0)
#define ide_set_en(b_en)                    ide_setEn(b_en)
#define ide_get_en()                        ide_getEn()
#define ide_set_load()                      ide_setLoad()
#define ide_get_load()                      ide_getLoad()
#define ide_get_video1_en()                 ide_getV1En()
#define ide_get_video2_en()                 ide_getV2En()

#define ide_set_video1_buf0_addr(uiYaddr, uiCBaddr, uiCRaddr)   ide_setV1Buf0Addr(uiYaddr, uiCBaddr, uiCRaddr)
#define ide_set_video1_buf1_addr(uiYaddr, uiCBaddr, uiCRaddr)   ide_setV1Buf1Addr(uiYaddr, uiCBaddr, uiCRaddr)
#define ide_set_video1_buf2_addr(uiYaddr, uiCBaddr, uiCRaddr)   ide_setV1Buf2Addr(uiYaddr, uiCBaddr, uiCRaddr)
#define ide_get_video1_buf_dim(puiBW, puiBH, puiLineoffset)     ide_getV1BufDim(puiBW, puiBH, puiLineoffset)
#define ide_set_video1_readorder(b_L2R, b_T2B)                  ide_setV1ReadOrder(b_L2R, b_T2B)
#define ide_set_video1_bufferop(uiBJMODE, uiOPTBUF, uiBUFNUM)   ide_setV1BufOp(uiBJMODE, uiOPTBUF, uiBUFNUM)
#define ide_get_video1_buf0_addr(puiYaddr, puiCBaddr, puiCRaddr)    ide_getV1Buf0Addr(puiYaddr, puiCBaddr, puiCRaddr)
#define ide_get_video1_buf1_addr(puiYaddr, puiCBaddr, puiCRaddr)    ide_getV1Buf1Addr(puiYaddr, puiCBaddr, puiCRaddr)
#define ide_get_video1_buf2_addr(puiYaddr, puiCBaddr, puiCRaddr)    ide_getV1Buf2Addr(puiYaddr, puiCBaddr, puiCRaddr)
#define ide_set_video2_buf0_addr(uiYaddr, uiCBaddr, uiCRaddr)       ide_setV2Buf0Addr(uiYaddr, uiCBaddr, uiCRaddr)
#define ide_set_video2_buf1_addr(uiYaddr, uiCBaddr, uiCRaddr)       ide_setV2Buf1Addr(uiYaddr, uiCBaddr, uiCRaddr)
#define ide_set_video2_buf2_addr(uiYaddr, uiCBaddr, uiCRaddr)       ide_setV2Buf2Addr(uiYaddr, uiCBaddr, uiCRaddr)
#define ide_get_video2_buf_dim(puiBW, puiBH, puiLineoffset)         ide_getV2BufDim(puiBW, puiBH, puiLineoffset)
#define ide_set_video2_readorder(b_L2R, b_T2B)                      ide_setV2ReadOrder(b_L2R, b_T2B)
#define ide_set_video2_bufferop(uiBJMODE, uiOPTBUF, uiBUFNUM)       ide_setV2BufOp(uiBJMODE, uiOPTBUF, uiBUFNUM)
#define ide_get_video2_buf0_addr(puiYaddr, puiCBaddr, puiCRaddr)    ide_getV2Buf0Addr(puiYaddr, puiCBaddr, puiCRaddr)
#define ide_get_video2_buf1_addr(puiYaddr, puiCBaddr, puiCRaddr)    ide_getV2Buf1Addr(puiYaddr, puiCBaddr, puiCRaddr)
#define ide_get_video2_buf2_addr(puiYaddr, puiCBaddr, puiCRaddr)    ide_getV2Buf2Addr(puiYaddr, puiCBaddr, puiCRaddr)

#define ide_set_osd1_buf_addr(uiaddr)                       ide_setO1BufAddr(uiaddr)
#define ide_get_osd1_buf_addr(uiaddr)                       ide_getO1BufAddr(uiaddr)
#define ide_set_osd1_buf_dim(uiBW, uiBH, uiLineoffset)      ide_setO1BufDim(uiBW, uiBH, uiLineoffset)
#define ide_get_osd1_buf_dim(puiBW, puiBH, puiLineoffset)   ide_getO1BufDim(puiBW, puiBH, puiLineoffset)
#define ide_set_osd1_readorder(b_L2R, b_T2B)                ide_setO1ReadOrder(b_L2R, b_T2B)
#define ide_set_osd2_buf_addr(uiaddr)                       ide_setO2BufAddr(uiaddr)
#define ide_get_osd2_buf_addr(uiaddr)                       ide_getO2BufAddr(uiaddr)
#define ide_set_osd2_buf_dim(uiBW, uiBH, uiLineoffset)      ide_setO2BufDim(uiBW, uiBH, uiLineoffset)
#define ide_get_osd2_buf_dim(puiBW, puiBH, puiLineoffset)   ide_getO2BufDim(puiBW, puiBH, puiLineoffset)
#define ide_set_osd2_readorder(b_L2R, b_T2B)                ide_setO2ReadOrder(b_L2R, b_T2B)

#define ide_set_video1_scalectrl(b_Hscaleup, b_Vscaleup)    ide_setV1ScaleCtrl(b_Hscaleup, b_Vscaleup)
#define ide_set_video2_scalectrl(b_Hscaleup, b_Vscaleup)    ide_setV2ScaleCtrl(b_Hscaleup, b_Vscaleup)
#define ide_set_osd1_scalectrl(b_Hscaleup, b_Vscaleup)      ide_setO1ScaleCtrl(b_Hscaleup, b_Vscaleup)
#define ide_set_osd2_scalectrl(b_Hscaleup, b_Vscaleup)      ide_setO2ScaleCtrl(b_Hscaleup, b_Vscaleup)
#define ide_set_dim_convert()                               do{ }while(0)
#define ide_set_convert_factor(uiHFT, uiVFT)                do{ }while(0)
#define ide_set_osd_colorkey(uiOSDColorKey_index)           ide_setOsdColorKey(uiOSDColorKey_index)
#define ide_set_osd_colorkey_en(b_en)                       ide_setOsdColorKeyEn(b_en)
#define ide_set_osd_blend_op(uiOSDBlendOp)                  do{ }while(0)
#define ide_set_v1_blend_v2(uiV1BldV2)                      do{ }while(0)

#define ide_set_osd1_scalefactor(uiHSF, b_sub, uiVSF)   ide_setO1ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_get_osd1_scalefactor(uiHSF, b_sub, uiVSF)   ide_getO1ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_set_osd1_win_dim(uiWinW, uiWinH)            ide_setO1WinDim(uiWinW, uiWinH)
#define ide_get_osd1_win_dim(uiWinW, uiWinH)            ide_getO1WinDim(uiWinW, uiWinH)
#define ide_set_osd1_win_format(uiFormat)               ide_setO1Fmt(uiFormat)
#define ide_get_osd1_win_format(uiFormat)               ide_getO1Fmt(uiFormat)
#define ide_set_osd1_XY(uiX, uiY)                       ide_setO1WinPos(uiX, uiY)
#define ide_get_osd1_XY(uiX, uiY)                       ide_getO1WinPos(uiX, uiY)
#define ide_set_osd1_palette_highaddr(uiAddr)           ide_setO1PaletteHighAddr(uiAddr)
#define ide_set_osd1_psel(uiPsel)                       ide_setO1PaletteSel(uiPsel)
#define ide_set_osd2_scalefactor(uiHSF, b_sub, uiVSF)   ide_setO2ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_get_osd2_scalefactor(uiHSF, b_sub, uiVSF)   ide_getO2ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_set_osd2_win_dim(uiWinW, uiWinH)            ide_setO2WinDim(uiWinW, uiWinH)
#define ide_get_osd2_win_dim(uiWinW, uiWinH)            ide_getO2WinDim(uiWinW, uiWinH)
#define ide_set_osd2_win_format(uiFormat)               ide_setO2Fmt(uiFormat)
#define ide_get_osd2_win_format(uiFormat)               ide_getO2Fmt(uiFormat)
#define ide_set_osd2_XY(uiX, uiY)                       ide_setO2WinPos(uiX, uiY)
#define ide_get_osd2_XY(uiX, uiY)                       ide_getO2WinPos(uiX, uiY)
#define ide_set_osd2_palette_highaddr(uiAddr)           ide_setO2PaletteHighAddr(uiAddr)
#define ide_set_osd2_psel(uiPsel)                       ide_setO2PaletteSel(uiPsel)

#define ide_set_v1_scalefactor(uiHSF, b_sub, uiVSF)     ide_setV1ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_get_v1_scalefactor(uiHSF, b_sub, uiVSF)     ide_getV1ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_set_v1_win_dim(uiWinW, uiWinH)              ide_setV1WinDim(uiWinW, uiWinH)
#define ide_get_v1_win_dim(uiWinW, uiWinH)              ide_getV1WinDim(uiWinW, uiWinH)
#define ide_set_v1_win_format(uiFormat)                 ide_setV1Fmt(uiFormat)
#define ide_get_v1_win_format(uiFormat)                 ide_getV1Fmt(uiFormat)
#define ide_set_v1_XY(uiX, uiY)                         ide_setV1WinPos(uiX, uiY)
#define ide_get_v1_XY(uiX, uiY)                         ide_getV1WinPos(uiX, uiY)
#define ide_set_v1_palette_highaddr()                   do{ }while(0)
#define ide_set_v1_psel()                               do{ }while(0)
#define ide_get_v1_psel()                               0
#define ide_set_v2_scalefactor(uiHSF, b_sub, uiVSF)     ide_setV2ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_get_v2_scalefactor(uiHSF, b_sub, uiVSF)     ide_getV2ScaleFactor(uiHSF, b_sub, uiVSF)
#define ide_set_v2_win_dim(uiWinW, uiWinH)              ide_setV2WinDim(uiWinW, uiWinH)
#define ide_get_v2_win_dim(uiWinW, uiWinH)              ide_getV2WinDim(uiWinW, uiWinH)
#define ide_set_v2_win_format(uiFormat)                 ide_setV2Fmt(uiFormat)
#define ide_get_v2_win_format(uiFormat)                 ide_getV2Fmt(uiFormat)
#define ide_set_v2_XY(uiX, uiY)                         ide_setV2WinPos(uiX, uiY)
#define ide_get_v2_XY(uiX, uiY)                         ide_getV2WinPos(uiX, uiY)
#define ide_set_v2_palette_highaddr()                   do{ }while(0)
#define ide_set_v2_psel()                               do{ }while(0)
#define ide_get_v2_psel()                               0

#define ide_set_background(uiColorY, uiColorCB, uiColorCR)      ide_setBackground(uiColorY, uiColorCB, uiColorCR)
#define ide_set_osdpalette(uiEntry, uiColorY, uiColorCB, uiColorCR, uiBlinkP, uiBlendOP)    ide_setPalEntry(uiEntry, uiColorY, uiColorCB, uiColorCR, uiBlendOP)
#define ide_set_hor_timing(uiHsynct, uiHtotal, uiHvalidst, uiHvalided)      ide_setHorTiming(uiHsynct, uiHtotal, uiHvalidst, uiHvalided)
#define ide_set_ver_timing(uiVsynct, uiVtotal, uiODDVvalidst, uiODDVvalided, uiEVENVvalidst, uiEVNEVvalided)    ide_setVerTiming(uiVsynct, uiVtotal, uiODDVvalidst, uiODDVvalided, uiEVENVvalidst, uiEVNEVvalided)
#define ide_set_interlace(b_inter)                  ide_setInterlace(b_inter)
#define ide_get_interlace()                         ide_getInterlace()
#define ide_set_startfield(b_oddst)                 ide_setStartField(b_oddst)
#define ide_get_curfield()                          ide_getCurField()
#define ide_fillgamma(uipgamma)                     ide_fillGamma(uipgamma)
#define ide_set_ctrst(uiCTRST)                      ide_setCtrst(uiCTRST)
#define ide_set_brt(uiBRT)                          ide_setBrt(uiBRT)
#define ide_set_cmults(uiCMULTS)                    ide_setCmults(uiCMULTS)
#define ide_set_cex(b_cex)                          ide_setCex(b_cex)
#define ide_set_standby_signal(b_stbyb)             do{ }while(0)
#define ide_set_clamp(b_clm)                        do{ }while(0)
#define ide_set_tv_burstLevel(uiBrl)                ide_setTvBrl(uiBrl)
#define ide_set_tv_blandLevel(uiBll)                ide_setTvBll(uiBll)
#define ide_set_tv_YCbCr_Scale(uiYScale, uiCBScale, uiCRScale)      ide_setTvScale(uiYScale, uiCBScale, uiCRScale)
#define ide_set_tv_sela(uiSela)                     ide_setTvSela(uiSela)
#define ide_set_tv_powerdown(b_powerdown)           ide_setTvPowerDown(b_powerdown)
#define ide_set_interrupt_en(uiINTen)               ide_setInterruptEn(uiINTen)
#define ide_get_interrupt_status()                  ide_getInterruptStatus()
#define ide_clear_interrupt_status(uiINTstatus)     ide_clearInterruptStatus(uiINTstatus)
#define ide_set_sif_startend(uiSIFst, uiSIFed)      do{ }while(0)
#define ide_set_tv_amp(uiAMPunit, uiCRsf, uiCBsf, uiYsf)            do{ }while(0)
#define ide_setHlpfCoef(uiCoef0, uiCoef1, uiCoef2)  do{ }while(0)
#define ide_setFcstPostOffset(uiY,uiCb,uiCr)        do{ }while(0)

void ide_set_source(BOOL b_sel);

void ide_set_video1_changebuf(UINT32 uiOPTBUF);
void ide_get_video1_bufferop(UINT32 *puiBJMODE, UINT32 *puiOPTBUF, UINT32 *puiBUFNUM);
void ide_get_video1_readorder(BOOL *pb_L2R, BOOL *pb_T2B);
void ide_set_video2_changebuf(UINT32 uiOPTBUF);
void ide_get_video2_bufferop(UINT32 *puiBJMODE, UINT32 *puiOPTBUF, UINT32 *puiBUFNUM);
void ide_get_video2_readorder(BOOL *pb_L2R, BOOL *pb_T2B);

void ide_get_osd1_readorder(BOOL *pb_L2R, BOOL *pb_T2B);
void ide_get_osd2_readorder(BOOL *pb_L2R, BOOL *pb_T2B);

UINT8 ide_get_osd_colorkey(void);
IDE_OSD_BLEND_OP ide_get_osd_blend_op(void);
IDE_V1_V2_BLEND_OP ide_get_v1_blend_v2(void);

IDE_PALETTE_SEL ide_get_osd1_psel(void);
IDE_PALETTE_SEL ide_get_osd2_psel(void);

void ide_get_osdpalette(UINT32 uiEntry, UINT8 *uiColorY, UINT8 *uiColorCB, UINT8 *uiColorCR, UINT8 *uiBlinkP, UINT8 *uiBlendOP);

void ide_set_tv_mode(UINT8 uiTVmode, UINT8 uiBurst, BOOL b_slave);



//-------------------------------------------------
// API for IDE.c
//-------------------------------------------------

#define ide_set_video_buf_addr(VideoID, pV_Buf_Addr)  ide_setVideoBufAddr(VideoID, pV_Buf_Addr)
#define ide_get_video_buf_addr(VideoID, pV_Buf_Addr)  ide_getVideoBufAddr(VideoID, pV_Buf_Addr)
#define ide_set_video_buf_attr(VideoID, pV_Buf_attr)  ide_setVideoBufAttr(VideoID, pV_Buf_attr)
#define ide_get_video_buf_attr(VideoID, pV_Buf_attr)  ide_getVideoBufAttr(VideoID, pV_Buf_attr)
#define ide_set_video_win_attr(VideoID, pV_Win_attr)  ide_setVideoWinAttr(VideoID, pV_Win_attr)
#define ide_set_video_vsfinit(VideoID, uiinit0, uiinit1)  ide_setVideoVsfInit(VideoID, uiinit0, uiinit1)
#define ide_set_osd_buf_addr(OSDID, uiOSD_Addr)  ide_setOsdBufAddr(OSDID, uiOSD_Addr)
#define ide_get_osd_buf_addr(OSDID, puiOSD_Addr)  ide_getOsdBufAddr(OSDID, puiOSD_Addr)
#define ide_set_osd_buf_attr(OSDID, pOSD_Buf_attr)  ide_setOsdBufAttr(OSDID, pOSD_Buf_attr)
#define ide_get_osd_buf_attr(OSDID, pOSD_Buf_attr)  ide_getOsdBufAttr(OSDID, pOSD_Buf_attr)
#define ide_set_osd_win_attr(OSDID, pOSD_Win_attr)  ide_setOsdWinAttr(OSDID, pOSD_Win_attr)
#define ide_set_osd_vsfinit(OSDID, uiinit0, uiinit1)  ide_setOsdVsfInit(OSDID, uiinit0, uiinit1)
#define ide_set_palette_entry(pPalette_entry)  ide_setPaletteEntry(pPalette_entry)
#define ide_get_palette_entry(pPalette_entry)  ide_getPaletteEntry(pPalette_entry)
#define ide_wait_frame_end()  ide_waitFrameEnd()
#define ide_wait_field_correct()    do{ }while(0)
#define ide_set_video_bgplane()     do{ }while(0)
#define ide_set_video_colorkey(uicolorkeyY, uicolorkeyCB, uicolorkeyCR)  ide_setVideoColorKey(uicolorkeyY, uicolorkeyCB, uicolorkeyCR)
#define ide_set_video_colorkey_op(uiVColorKeyOpt)  ide_setVideoColorKeyOp(uiVColorKeyOpt)
#define ide_set_video_blend(uiVOpt) do{ }while(0)
#define ide_set_video_colorkey_sel(CkSel)  ide_setVideoColorKeySel(CkSel)
#define ide_set_video_buffer_content(VIDEO_ID, BUFFER_ID, pYCbCr)  ide_setVideoBufferContent(VIDEO_ID, BUFFER_ID, pYCbCr)
#define ide_convert_RGB2YCbCr(pRGB, pYCbCr)  ide_convertRGB2YCbCr(pRGB, pYCbCr)
#define ide_disable_video(VideoID)  ide_disableVideo(VideoID)
#define ide_enable_video(VideoID)  ide_enableVideo(VideoID)
#define ide_get_enable_video(VideoID)  ide_getVideoEnable(VideoID)
#define ide_disable_osd(OSDID)  ide_disableOsd(OSDID)
#define ide_enable_osd(OSDID)  ide_enableOsd(OSDID)
#define ide_get_enable_osd(OSDID)  ide_getOsdEnable(OSDID)

void ide_set_video_bgcolorkey(IDE_VIDEOID VIDEO_ID, UINT8 uccolorkeyY, UINT8 uccolorkeyCB, UINT8 uccolorkeyCR);

//@}

#endif //_IDE_COMPAT_H

