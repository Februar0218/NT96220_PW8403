#ifndef ISR_MI5131E_220_H
#define ISR_MI5131E_220_H

/**
ipp isr cb func.
*/
extern void MI5131E_sie_isrCB(UINT32 IntStatus);
extern void MI5131E_ime_isrCB(UINT32 IntStatus);
extern void MI5131E_ipe_isrCB(UINT32 IntStatus);
extern void MI5131E_ife_isrCB(UINT32 IntStatus);
#endif
