#ifndef ISR_MI5130_220_H
#define ISR_MI5130_220_H

/**
ipp isr cb func.
*/
extern void MI5130_sie_isrCB(UINT32 IntStatus);
extern void MI5130_ime_isrCB(UINT32 IntStatus);
extern void MI5130_ipe_isrCB(UINT32 IntStatus);
extern void MI5130_ife_isrCB(UINT32 IntStatus);
#endif
