#ifndef ISR_OV5653_220_H
#define ISR_OV5653_220_H

/**
ipp isr cb func.
*/
extern void OV5653_sie_isrCB(UINT32 IntStatus);
extern void OV5653_ime_isrCB(UINT32 IntStatus);
extern void OV5653_ipe_isrCB(UINT32 IntStatus);
extern void OV5653_ife_isrCB(UINT32 IntStatus);
#endif
