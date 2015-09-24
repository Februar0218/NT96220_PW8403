#ifndef ISR_OV5633_220_H
#define ISR_OV5633_220_H

/**
ipp isr cb func.
*/
extern void OV5633_sie_isrCB(UINT32 IntStatus);
extern void OV5633_ime_isrCB(UINT32 IntStatus);
extern void OV5633_ipe_isrCB(UINT32 IntStatus);
extern void OV5633_ife_isrCB(UINT32 IntStatus);
#endif
