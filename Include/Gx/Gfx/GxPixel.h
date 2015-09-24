#ifndef _GX_PIXEL_H_
#define _GX_PIXEL_H_

#include "GxCommon.h"
#include "GxRect.h"
#include "GxDC.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PIXEL_2D
{
    void (*Begin)(struct _PIXEL_2D* p, RECT_2D* pRect);
    void (*BeginLine)(struct _PIXEL_2D* p);
    void (*BeginPixel)(struct _PIXEL_2D* p);
    UINT32 (*Read)(struct _PIXEL_2D* p);
    void (*Write)(struct _PIXEL_2D* p, UINT32 value);
    void (*EndPixel)(struct _PIXEL_2D* p, BOOL bForceFlush, BOOL bNext);
    void (*EndLine)(struct _PIXEL_2D* p);
    void (*End)(struct _PIXEL_2D* p);
    void (*WriteOffset)(struct _PIXEL_2D* p, UINT32 value, INT32 dx, INT32 dy);
    void (*GotoXY)(struct _PIXEL_2D* p, int x, int y);
    UINT8* (*LineBuf)(struct _PIXEL_2D* p, int y);
    UINT8* (*PlaneBuf)(struct _PIXEL_2D* p, int i);

    DC* pDC;
    RECT_2D* pRect;

    UINT32 data[20];
}
PIXEL_2D;

BOOL PIXEL_2D_Init(PIXEL_2D* p, DC* _pDC);
/*
void PIXEL_2D_Begin(PIXEL_2D* p, RECT_2D* pRect);
void PIXEL_2D_BeginLine(PIXEL_2D* p);
void PIXEL_2D_BeginPixel(PIXEL_2D* p);
DATA_PACK PIXEL_2D_Read(PIXEL_2D* p);
void PIXEL_2D_Write(PIXEL_2D* p, DATA_PACK value);
void PIXEL_2D_EndPixel(PIXEL_2D* p, BOOL bForceFlush, BOOL bNext);
void PIXEL_2D_EndLine(PIXEL_2D* p);
void PIXEL_2D_End(PIXEL_2D* p);
void PIXEL_2D_WriteOffset(PIXEL_2D* p, UINT32 value, INT32 dx, INT32 dy);
*/
#define PIXEL_2D_Begin(p, pRect)                    (((p)->Begin))((p), pRect)
#define PIXEL_2D_BeginLine(p)                       (((p)->BeginLine))((p))
#define PIXEL_2D_BeginPixel(p)                      (((p)->BeginPixel))((p))
#define PIXEL_2D_Read(p)                            (((p)->Read))((p))
#define PIXEL_2D_Write(p, value)                    (((p)->Write))((p), value)
#define PIXEL_2D_EndPixel(p, bForceFlush, bNext)    (((p)->EndPixel))((p), bForceFlush, bNext)
#define PIXEL_2D_EndLine(p)                         (((p)->EndLine))((p))
#define PIXEL_2D_End(p)                             (((p)->End))((p))
#define PIXEL_2D_WriteOffset(p, value, dx, dy)      (((p)->WriteOffset))((p), value, dx, dy)
#define PIXEL_2D_GotoXY(p, x, y)                    (((p)->GotoXY))((p), x, y)
#define PIXEL_2D_LineBuf(p, y)                      (((p)->LineBuf))((p), y)
#define PIXEL_2D_PlaneBuf(p, i)                     (((p)->PlaneBuf))((p), i)

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GX_PIXEL_H_