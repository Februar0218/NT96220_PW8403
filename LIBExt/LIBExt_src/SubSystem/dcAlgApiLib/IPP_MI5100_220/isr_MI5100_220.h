#ifndef ISR_MI5100_220_H
#define ISR_MI5100_220_H

/**
ipp isr cb func.
*/
extern void MI5100_sie_isrCB(UINT32 IntStatus);
extern void MI5100_ime_isrCB(UINT32 IntStatus);
extern void MI5100_ipe_isrCB(UINT32 IntStatus);
extern void MI5100_ife_isrCB(UINT32 IntStatus);
#endif
