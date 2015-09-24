#ifndef ISR_NT99140_220_H
#define ISR_NT99140_220_H

/**
ipp isr cb func.
*/
extern void NT99140_sie_isrCB(UINT32 IntStatus);
extern void NT99140_ime_isrCB(UINT32 IntStatus);
extern void NT99140_ipe_isrCB(UINT32 IntStatus);
extern void NT99140_ife_isrCB(UINT32 IntStatus);
#endif
