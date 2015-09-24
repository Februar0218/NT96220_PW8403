#ifndef ISR_H22_220_H
#define ISR_H22_220_H

/**
ipp isr cb func.
*/
extern void H22_sie_isrCB(UINT32 IntStatus);
extern void H22_ime_isrCB(UINT32 IntStatus);
extern void H22_ipe_isrCB(UINT32 IntStatus);
extern void H22_ife_isrCB(UINT32 IntStatus);
#endif
