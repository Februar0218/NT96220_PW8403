#ifndef SEN_ISR_H
#define SEN_ISR_H

/**
     ipp isr cb func. for proj dep.

     @note (Optional)
*/
typedef struct _ISR_CB
{
    void (*SIEISR_CB)(UINT32); ///< ext. sie isr callback function
    void (*IPEISR_CB)(UINT32); ///< ext. ipe isr callback function
    void (*IMEISR_CB)(UINT32); ///< ext. ime isr callback function
    void (*IFEISR_CB)(UINT32); ///< ext. ife isr callback function
}ISR_CB,*PISR_CB;


/**
     Reg ipp isr cb
*/
extern void IsrCBReg(PISR_CB pIsr_cb);

/**
    cfg isr_cb information
*/
extern void SetIsrInfo(void* _Value);


/**
    dzoom flg
*/
#define DZOOM_IDLE          0x00000001
#define DZOOM_SET_SIEPRE    0x00000002

/**
    flg for run time chg ipp i/o size
    @param Flag     dzoom flg
*/
extern void Set_DZoomFlag(UINT32 Flag);
extern void Clr_DZoomFlag(UINT32 Flag);
extern UINT32 Get_DZoomFlag(void);

/**
    chg sensor mode flg
*/
#define CHGMODE_IDLE          0x00000001
#define CHGMODE_SIE_SET       0x00000002
#define CHGMODE_IME_SET       0x00000004
#define CHGMODE_WAIT_IMEEND   0x00000008

/**
    flg for run time chg sensor mode and ipp i/o size
*/
extern void Clr_ChgModeFlag(UINT32 Flag);
extern void Set_ChgModeFlag(UINT32 Flag);
extern UINT32 Get_ChgModeFlag(void);
#endif
