
//    Drawing Canvas

/*
    DC : Drawing Canvas object, a bitmap use as a drawing target/source

Ex:
    LAYER : Display layer, display a DC to specified window on screen
    Get DC from LAYER:
        pDC = DISPLAY_BeginDraw(uiLayerID);
        DISPLAY_EndDraw(uiLayerID, pDC);

    BITMAP : Image with Bitmap type object, contain memory for bitmap image (WORD align)
    Get DC from BITMAP:
        pDC = BITMAP_GetDC(pBm);
        BITMAP_ReleaseDC(pBm, pDC);

    ICON : Image with Icon type object, contain memory for icon image (bit-compacted)
    Get DC from ICON:
        ICON_MakeDC(&dc, pIconDB, uiIconID);

    JPEG : Image with Jpeg stream type object, contain memory for jpeg stream image (only for Blt)
    Get DC from JPEG:
        JPEG_MakeDC(&dc, pJpegStream, uiJpegStreamSize);

*/

#ifndef _GX_DC_H_
#define _GX_DC_H_

#include "GxCommon.h"
#include "GxRect.h"

#ifdef __cplusplus
extern "C" {
#endif


//--------------------------------------------------------------------------------------
//  type - DC
//--------------------------------------------------------------------------------------
#define DC_RESERVE_DWORD            (28-8+16+16)

typedef struct _DC
{
    UINT32 uiFlag;
    UINT16 uiType;
    UINT16 uiPxlfmt;
    SIZE_2D size;
    RECT_2D winRect;
    POINT_2D origPt;
    POINT_2D lastPt;
    POINT_2D thisPt;

    UINT32 uiReserved[DC_RESERVE_DWORD];
}
DC;


//--------------------------------------------------------------------------------------
//  enum - DC
//--------------------------------------------------------------------------------------
#define PLANE_Y                 0x00    //Support format: PXLFMT_YUV444, PXLFMT_YUV422, PXLFMT_YUV420
#define PLANE_U                 0x01    //Support format: PXLFMT_YUV444, PXLFMT_YUV422, PXLFMT_YUV420
#define PLANE_V                 0x02    //Support format: PXLFMT_YUV444, PXLFMT_YUV422, PXLFMT_YUV420
#define PLANE_R                 0x00    //Support format: PXLFMT_RGB888
#define PLANE_G                 0x01    //Support format: PXLFMT_RGB888
#define PLANE_B                 0x02    //Support format: PXLFMT_RGB888
//#NT#2009/7/15#JeahYen -begin
#define PLANE_RGB               0x03    //Support format: PXLFMT_RGB888_PK
#define PLANE_RGB565            0x00    //Support format: PXLFMT_RGB565_PK, PXLFMT_RGBA5658_PK, PXLFMT_RGBA5654_PK
//#NT#2009/7/15#JeahYen -end
#define PLANE_IDX               0x03    //Support format: PXLFMT_INDEX1, PXLFMT_INDEX2, PXLFMT_INDEX4, PXLFMT_INDEX8

#define TYPE_NULL               0x00    //invalid
#define TYPE_FB                 0x01    //bitmap with pitch is 4 byes alignment
#define TYPE_BITMAP             0x02    //bitmap with pitch is 4 byes alignment
#define TYPE_ICON               0x03    //bitmap with compact bits
#define TYPE_JPEG               0x04    //JPEG compressed bit stream

//#NT#2009/7/15#JeahYen -begin
#define PXLFMT_PMODE            0xf0    //YUV, not RGB
#define PXLFMT_P1               0x00    //1 color plane
#define PXLFMT_P1_PK            0x10    //1 color plane (888 packed)
#define PXLFMT_P1_IDX           0x20    //1 color plane, index
#define PXLFMT_P3               0x30    //3 color plane x 3
#define PXLFMT_P1_PK2           0x40    //3 color plane (565 packed)

#define PXLFMT_YUV              0x08    //YUV, not RGB

#define PXLFMT_RGB888_PK        (PXLFMT_P1_PK | 0x00)   //1 color plane, pixel=RGB888, packed 3-byte with B8-G8-R8 order.
#define PXLFMT_RGBD8888_PK      (PXLFMT_P1_PK | 0x03)   //1 color plane, pixel=RGBD8888, packed 3-byte with B8-G8-R8 order.
#define PXLFMT_RGB565_PK        (PXLFMT_P1_PK2 | 0x00)  //1 color plane, pixel=RGB565, packed 2-byte with B5-G6-R5 order.
#define PXLFMT_RGBA5658_PK      (PXLFMT_P1_PK2 | 0x01)  //2 color plane, pixel=RGB565, and A8, packed 2-byte with B5-G6-R5 order.
#define PXLFMT_RGBA5654_PK      (PXLFMT_P1_PK2 | 0x02)  //2 color plane, pixel=RGB565, and A4, packed 2-byte with B5-G6-R5 order.
#define PXLFMT_INDEX1           (PXLFMT_P1_IDX | 0x00)  //1 color plane, pixel=INDEX2
#define PXLFMT_INDEX2           (PXLFMT_P1_IDX | 0x01)  //1 color plane, pixel=INDEX2
#define PXLFMT_INDEX4           (PXLFMT_P1_IDX | 0x02)  //1 color plane, pixel=INDEX4
#define PXLFMT_INDEX8           (PXLFMT_P1_IDX | 0x03)  //1 color plane, pixel=INDEX8
#define PXLFMT_RGB888           (PXLFMT_P3 | 0x00)      //3 color plane, pixel=R8, G8, and B8
#define PXLFMT_RGB565           (PXLFMT_P3 | 0x01)      //3 color plane, pixel=R8, G8, and B8
#define PXLFMT_RGB555           (PXLFMT_P3 | 0x02)      //3 color plane, pixel=R8, G8, and B8
#define PXLFMT_YUV444           (PXLFMT_P3 | 0x08)      //3 color plane, pixel=Y(w,h), U(w,h), and V(w,h)
#define PXLFMT_YUV422           (PXLFMT_P3 | 0x09)      //3 color plane, pixel=Y(w,h), U(w/2,h), and V(w/2,h)
#define PXLFMT_YUV420           (PXLFMT_P3 | 0x0A)      //3 color plane, pixel=Y(w,h), U(w/2,h/2), and V(w/2,h/2)
#define PXLFMT_YUV421           (PXLFMT_P3 | 0x0B)      //3 color plane, pixel=Y(w,h), U(w,h/2), and V(w,h/2)
#define PXLFMT_A8               PXLFMT_INDEX8
#define PXLFMT_A4               PXLFMT_INDEX4
//#NT#2009/7/15#JeahYen -end



//--------------------------------------------------------------------------------------
//  function - DC
//--------------------------------------------------------------------------------------
//dc memory
RESULT GxGfx_CheckDCTypeFmt     (UINT16 uiType, UINT16 uiPxlfmt);
UINT32 GxGfx_CalcDCPitchSize    (UINT32 uiWidth, UINT16 uiType, UINT16 uiPxlfmt);
UINT32 GxGfx_CalcDCMemSize      (UINT32 uiWidth, UINT32 uiHeight, UINT16 uiType, UINT16 uiPxlfmt);
//dc attach and detach
RESULT GxGfx_AttachDC           (DC* pDC,
                                UINT16 uiType, UINT16 uiPxlfmt,
                                UINT32 uiWidth, UINT32 uiHeight,
                                UINT32 uiPitch, UINT8* pBuf0, UINT8* pBuf1, UINT8* pBuf2);
void   GxGfx_DetachDC           (DC* pDC);
//dc buffer size
SIZE_2D GxGfx_GetSize           (const DC* pDC);


#ifdef __cplusplus
} //extern "C"
#endif

#endif//_GX_DC_H_