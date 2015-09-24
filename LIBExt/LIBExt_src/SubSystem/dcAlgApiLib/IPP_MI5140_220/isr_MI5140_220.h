#ifndef ISR_MI5140_220_H
#define ISR_MI5140_220_H

/**
ipp isr cb func.
*/
extern void MI5140_sie_isrCB(UINT32 IntStatus);
extern void MI5140_ime_isrCB(UINT32 IntStatus);
extern void MI5140_ipe_isrCB(UINT32 IntStatus);
extern void MI5140_ife_isrCB(UINT32 IntStatus);
#endif
