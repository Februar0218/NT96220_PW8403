/**
    uITRON interrupt related header file.

    @file       Int.h
    @ingroup    mIKernelIntr
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/


#ifndef _INT_H
#define _INT_H

union   sch
{
    struct
    {
        unsigned char   psf;
        unsigned char   isf;
        unsigned char   tmf;
        unsigned char   acf;
    } byt;
    struct
    {
        unsigned short  pif;
        unsigned short  taf;
    } srt;
    unsigned long       wrd;
};

#define gpsflag     gschflg.byt.psf     // Scheduler is processing
#define gtmflag     gschflg.byt.tmf     // Timer is being requested
#define girqflag    gschflg.byt.acf     // IRQ int
#define gfiqflag    gschflg.byt.isf     // FIQ int

/**
    @addtogroup mIKernelIntr
*/
//@{

extern ER       def_int(UINT dintno, FP hdl);
extern ER       dis_int(UINT intno);
extern ER       ena_int(UINT intno);

//#NT#2010/05/10#Elvis Chuang -begin
//#NT# enable or disable checking in ISR
extern void     ena_chk_wai(BOOL bIsEnable);
//#NT#2010/05/10#Elvis Chuang -end
//@}

#endif
