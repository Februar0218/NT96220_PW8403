/**
    uITRON fixed memory pool related header file.

    @file       Memory.h
    @ingroup    mIKernelMem
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _MEMORY_H
#define _MEMORY_H

struct  fpl
{
    struct tcb*             f_forw;     /*  0 Ptr to first waiting TCB         */
    struct tcb*             f_back;     /*  4 Ptr to last waiting PCB          */
    volatile UB             f_stat;     /*  8 Pool status                      */
    UB                      f_type;     /*  9 Structure type                   */
    UH                      f_rsvd;     /* 10 Reserved (for alignment)         */
    VP                      f_bfre;     /* 12 Free buffers queue               */
    INT                     f_nfre;     /* 16 Free buffers number              */
    VP                      f_addr;     /* 20 Pool starting memory address     */
    INT                     f_bnum;     /* 24 Total buffers number             */
    INT                     f_bsiz;     /* 28 Buffers size in bytes            */
    VP                      f_exinf;    /* 32 Extended information             */
};

/**
    @addtogroup mIKernelMem
*/
//@{

extern void     kini_mpf(void);
extern ER       rel_blk(ID mpfid, VP blf);
extern ER       get_blk(VP* p_blf, ID mpfid);
extern ER       icr_mpf(T_CMPF* pk_cmpf);
extern UINT     get_blksize(ID mpfid);

//@}

#endif
