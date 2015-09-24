#ifndef ISR_OV9712_220_H
#define ISR_OV9712_220_H

/**
ipp isr cb func.
*/
extern void OV9712_sie_isrCB(UINT32 IntStatus);
extern void OV9712_ime_isrCB(UINT32 IntStatus);
extern void OV9712_ipe_isrCB(UINT32 IntStatus);
extern void OV9712_ife_isrCB(UINT32 IntStatus);
#endif
