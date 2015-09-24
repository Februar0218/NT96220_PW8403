
//    Rect
/*
    POINT_2D : Point
    
    SIZE_2D : Size
    
    RECT_2D : Rectangle
*/

#ifndef _GX_RECT_H_
#define _GX_RECT_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------------------
//  geometry
//--------------------------------------------------------------------------------------

//<type>

typedef INT32   LVALUE; //location value

typedef struct _POINT_2D
{
    LVALUE x;
    LVALUE y;
} 
POINT_2D;

typedef struct _SIZE_2D
{
    LVALUE w;
    LVALUE h;
} 
SIZE_2D;

typedef struct _RECT_2D
{
    LVALUE x;
    LVALUE y;
    LVALUE w;
    LVALUE h;
} 
RECT_2D;


//<function>

/*
//inline functions:

BOOL RANGE_IsInside(LVALUE small_value, LVALUE large_value, LVALUE x);
LVALUE RANGE_Clamp(LVALUE small_value, LVALUE large_value, LVALUE x);

void POINT_Set(POINT_2D* pPt, LVALUE x, LVALUE y);
void POINT_SetPoint(POINT_2D* pPt, const POINT_2D* pPt2);
void POINT_MoveX(POINT_2D* pPt, LVALUE dx);
void POINT_MoveY(POINT_2D* pPt, LVALUE dy);
void POINT_MoveXY(POINT_2D* pPt, LVALUE dx, LVALUE dy);
void POINT_MovePoint(POINT_2D* pPt, const POINT_2D* pPt2);
BOOL POINT_IsInside4Points(const POINT_2D* pPt, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);
BOOL POINT_IsInsideRect(const POINT_2D* pPt, RECT_2D* pRect);

void SIZE_Set(SIZE_2D* pSz, LVALUE w, LVALUE h);
void SIZE_SetSize(SIZE_2D* pSz, const SIZE_2D* pSz2);

void RECT_Set(RECT_2D* pRect, LVALUE x, LVALUE y, LVALUE w, LVALUE h);
void RECT_SetXY(RECT_2D* pRect, LVALUE x, LVALUE y);
void RECT_SetWH(RECT_2D* pRect, LVALUE w, LVALUE h);
void RECT_SetPoint(RECT_2D* pRect, const POINT_2D* pPt);
void RECT_SetSize(RECT_2D* pRect, const SIZE_2D* pSz);
void RECT_SetRect(RECT_2D* pRect, const RECT_2D* pRect2);
LVALUE RECT_GetX1(const RECT_2D* pRect);
LVALUE RECT_GetY1(const RECT_2D* pRect);
LVALUE RECT_GetX2(const RECT_2D* pRect);
LVALUE RECT_GetY2(const RECT_2D* pRect);
POINT_2D RECT_GetPoint(const RECT_2D* pRect);
POINT_2D RECT_GetPoint2(const RECT_2D* pRect);
POINT_2D RECT_GetCenterPoint(const RECT_2D* pRect);
SIZE_2D RECT_GetSize(const RECT_2D* pRect);
void RECT_MoveX(RECT_2D* pRect, LVALUE dx);
void RECT_MoveY(RECT_2D* pRect, LVALUE dy);
void RECT_MoveXY(RECT_2D* pRect, LVALUE dx, LVALUE dy);
void RECT_MovePoint(RECT_2D* pRect, const POINT_2D* pPt2);

void RECT_Normalize(RECT_2D* pRect);
void RECT_Inflate(RECT_2D* pRect, LVALUE left, LVALUE right, LVALUE top, LVALUE bottom);
void RECT_Shrink(RECT_2D* pRect, LVALUE left, LVALUE right, LVALUE top, LVALUE bottom);
*/


#define RANGE_IsInside(small_value, large_value, x) \
    ((((x)>=(small_value)) && ((x)<=(large_value)))?TRUE:FALSE)

#define RANGE_Clamp(small_value, large_value, x) \
    (((x)<(small_value))?(small_value):(((x)>(large_value))?(large_value):(x)))
    
__inline void POINT_Set(POINT_2D* pPt, LVALUE x, LVALUE y)
{   
    (pPt)->x = (x);
    (pPt)->y = (y);  
}

__inline void POINT_SetPoint(POINT_2D* pPt, const POINT_2D* pPt2)
{   
    *(pPt) = *(pPt2);   
}

__inline void POINT_MoveX(POINT_2D* pPt, LVALUE dx)
{   
    (pPt)->x += (dx);   
}

__inline void POINT_MoveY(POINT_2D* pPt, LVALUE dy)
{   
    (pPt)->y += (dy); 
}

__inline void POINT_MoveXY(POINT_2D* pPt, LVALUE dx, LVALUE dy)
{   
    (pPt)->x += (dx);   (pPt)->y += (dy);  
}

__inline void POINT_MovePoint(POINT_2D* pPt, const POINT_2D* pPt2)
{   
    (pPt)->x += (pPt2)->x;   (pPt)->y += (pPt2)->y;  
}

__inline BOOL POINT_IsInside4Points(const POINT_2D* pPt, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2)
{
    return 
        (
        (pPt->x >= x1) &&
        (pPt->x <= x2) &&
        (pPt->y >= y1) &&
        (pPt->y <= y2)
        );
}

__inline BOOL POINT_IsInsideRect(const POINT_2D* pPt, RECT_2D* pRect)
{
    return 
        (
        (pPt->x >= pRect->x) &&
        (pPt->x < (pRect->x+pRect->w)) &&
        (pPt->y >= pRect->y) &&
        (pPt->y < (pRect->y+pRect->h))
        );
}


__inline void SIZE_Set(SIZE_2D* pSz, LVALUE w, LVALUE h)
{   
    (pSz)->w = (w);
    (pSz)->h = (h);  
}

__inline void SIZE_SetSize(SIZE_2D* pSz, const SIZE_2D* pSz2)
{   
    *(pSz) = *(pSz2);   
}
    


__inline void RECT_Set(RECT_2D* pRect, LVALUE x, LVALUE y, LVALUE w, LVALUE h)
{
    (pRect)->x = (x);
    (pRect)->y = (y);
    (pRect)->w = (w);
    (pRect)->h = (h);
}

__inline void RECT_SetXY(RECT_2D* pRect, LVALUE x, LVALUE y)
{
    (pRect)->x = (x);
    (pRect)->y = (y);
}

__inline void RECT_SetWH(RECT_2D* pRect, LVALUE w, LVALUE h)
{
    (pRect)->w = (w);
    (pRect)->h = (h);
}

__inline void RECT_SetPoint(RECT_2D* pRect, const POINT_2D* pPt)
{
    (pRect)->x = (pPt)->x;
    (pRect)->y = (pPt)->y;
}

__inline void RECT_SetSize(RECT_2D* pRect, const SIZE_2D* pSz)
{
    (pRect)->w = (pSz)->w;
    (pRect)->h = (pSz)->h;
}

__inline void RECT_SetRect(RECT_2D* pRect, const RECT_2D* pRect2)
{
    *(pRect) = *(pRect2);
}

__inline LVALUE RECT_GetX1(const RECT_2D* pRect)
{
    return (pRect)->x;
}

__inline LVALUE RECT_GetY1(const RECT_2D* pRect)
{
    return (pRect)->y;
}

__inline LVALUE RECT_GetX2(const RECT_2D* pRect)
{
    return (pRect)->x + (pRect)->w;
}

__inline LVALUE RECT_GetY2(const RECT_2D* pRect)
{
    return (pRect)->y + (pRect)->h;
}

__inline POINT_2D RECT_GetPoint(const RECT_2D* pRect)
{
    POINT_2D Pt;
    (Pt).x = (pRect)->x;
    (Pt).y = (pRect)->y;
    return Pt;
}
__inline POINT_2D RECT_GetPoint2(const RECT_2D* pRect)
{
    POINT_2D Pt;
    (Pt).x = (pRect)->x + (pRect)->w;
    (Pt).y = (pRect)->y + (pRect)->h;
    return Pt;
}
__inline POINT_2D RECT_GetCenterPoint(const RECT_2D* pRect)
{
    POINT_2D Pt;
    (Pt).x = (pRect)->x + ((pRect)->w>>1);
    (Pt).y = (pRect)->y + ((pRect)->h>>1);
    return Pt;
}
__inline SIZE_2D RECT_GetSize(const RECT_2D* pRect)
{
    SIZE_2D Sz;
    (Sz).w = (pRect)->w;
    (Sz).h = (pRect)->h;
    return Sz;
}

__inline void RECT_Normalize(RECT_2D* pRect)
{
    if((pRect)->w < 0)
    {
        (pRect)->x += (pRect)->w;
        (pRect)->w = -(pRect)->w;
    }   
    if((pRect)->h < 0)
    {   
        (pRect)->y += (pRect)->h;
        (pRect)->h = -(pRect)->h;
    }
}

__inline void RECT_SetX1Y1X2Y2(RECT_2D* pRect, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2)
{
    LVALUE temp;
    if(x2 < x1)
    {
        temp = x2;
        x2 = x1;
        x1 = temp;
    }   
    if(y2 < y1)
    {
        temp = y2;
        y2 = y1;
        y1 = temp;
    } 
    RECT_Set(pRect, x1, y1, x2-x1+1, y2-y1+1);
}

#define RECT_MoveX(pRect, dx)   \
    POINT_MoveX(((POINT_2D*)pRect), dx)
#define RECT_MoveY(pRect, dy)   \
    POINT_MoveY(((POINT_2D*)pRect), dy) 
#define RECT_MoveXY(pRect, dx, dy)   \
    POINT_MoveXY(((POINT_2D*)pRect), dx, dy) 
#define RECT_MovePoint(pRect, pPt)   \
    POINT_MovePoint(((POINT_2D*)pRect), pPt) 


__inline void RECT_Inflate(RECT_2D* pRect, LVALUE left, LVALUE right, LVALUE top, LVALUE bottom)
{   
    (pRect)->x += -(left);
    (pRect)->w += ((left)+(right));
    (pRect)->y += -(top);
    (pRect)->h += ((top)+(bottom));
}


#define RECT_Shrink(pRect, left, right, top, bottom) \
    RECT_Inflate(pRect, -(left), -(right), -(top), -(bottom))


#ifdef __cplusplus
} //extern "C"
#endif

#endif//_GX_RECT_H_