#ifndef ISR_MI1600_220_H
#define ISR_MI1600_220_H

/**
ipp isr cb func.
*/
extern void MI1600_sie_isrCB(UINT32 IntStatus);
extern void MI1600_ime_isrCB(UINT32 IntStatus);
extern void MI1600_ipe_isrCB(UINT32 IntStatus);
extern void MI1600_ife_isrCB(UINT32 IntStatus);
#endif
