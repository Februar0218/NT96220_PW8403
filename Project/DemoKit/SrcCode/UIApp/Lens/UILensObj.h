#ifndef UI_LENSOBJ_H
#define UI_LENSOBJ_H

typedef enum {
    UILENS_AF_STA_OK,
    UILENS_AF_STA_FAIL
} _UILENS_AF_STATUS_;

extern UINT32   UILens_GetVariationCB(void);
extern void     UILens_AFPrepare(BOOL bBegin);
extern UINT32   UILens_AFProcess(void);
extern void     UILens_FocusToFar(UINT32 section);
extern void     UILens_WaitZoomFinish(void);
extern void     UILens_WaitAeConverge(void);
extern INT32    UILens_SearchFullRange(INT32 startIdx, INT32 endIdx, UINT32 jumpStep);
extern INT32    UILens_SearchFullRange_Bwd(INT32 startIdx, INT32 endIdx, UINT32 jumpStep);
extern UINT32   UILens_AFProcess_ST8003(void);
extern UINT32   UILens_AFProcess_ST8005(void);

#endif
