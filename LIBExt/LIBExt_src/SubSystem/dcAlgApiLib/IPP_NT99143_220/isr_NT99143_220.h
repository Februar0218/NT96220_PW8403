#ifndef ISR_NT99143_220_H
#define ISR_NT99143_220_H

/**
ipp isr cb func.
*/
extern void NT99143_sie_isrCB(UINT32 IntStatus);
extern void NT99143_ime_isrCB(UINT32 IntStatus);
extern void NT99143_ipe_isrCB(UINT32 IntStatus);
extern void NT99143_ife_isrCB(UINT32 IntStatus);
#endif
