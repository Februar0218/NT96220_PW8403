#ifndef ISR_MI9130_220_H
#define ISR_MI9130_220_H

/**
ipp isr cb func.
*/
extern void MI9130_sie_isrCB(UINT32 IntStatus);
extern void MI9130_ime_isrCB(UINT32 IntStatus);
extern void MI9130_ipe_isrCB(UINT32 IntStatus);
extern void MI9130_ife_isrCB(UINT32 IntStatus);
#endif
