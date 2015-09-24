#ifndef _PRVIMAGEPROCESS_H
#define _PRVIMAGEPROCESS_H

extern void PrvIfeOpen(UINT32 path);
extern void PrvIfeClose(void);
extern void     Set_PrvIfeFlag(UINT32 flag);
extern UINT32   Get_PrvIfeFlag(void);
extern UINT32   Get_PrvIfeTotalSteps(void);
extern void PrvIfeProcess(UINT32 step);

#endif