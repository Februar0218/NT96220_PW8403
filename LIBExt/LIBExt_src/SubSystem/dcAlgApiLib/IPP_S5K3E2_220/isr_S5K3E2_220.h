#ifndef ISR_S5K3E2_220_H
#define ISR_S5K3E2_220_H

/**
ipp isr cb func.
*/
extern void S5K3E2_sie_isrCB(UINT32 IntStatus);
extern void S5K3E2_ime_isrCB(UINT32 IntStatus);
extern void S5K3E2_ipe_isrCB(UINT32 IntStatus);
extern void S5K3E2_ife_isrCB(UINT32 IntStatus);
#endif
