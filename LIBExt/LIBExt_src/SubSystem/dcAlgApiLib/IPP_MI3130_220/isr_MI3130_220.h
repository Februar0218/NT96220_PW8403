#ifndef ISR_MI3130_220_H
#define ISR_MI3130_220_H

/**
ipp isr cb func.
*/
extern void MI3130_sie_isrCB(UINT32 IntStatus);
extern void MI3130_ime_isrCB(UINT32 IntStatus);
extern void MI3130_ipe_isrCB(UINT32 IntStatus);
extern void MI3130_ife_isrCB(UINT32 IntStatus);
#endif
