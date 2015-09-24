#ifndef DZOOM_H
#define DZOOM_H

#define IMG_RATIO_UINT      256

typedef struct _SIZE_INFOR {
            unsigned long H_Size;
            unsigned long V_Size;
} SIZE_INFOR;

typedef struct
{
    UINT32 SensorRatio;
    UINT32 ImgRatio;
    UINT32 DispRatio;
    UINT32 DispWindFit;
    UINT32 (*DzTable)[6];
} DZOOM_INFOR;

extern void Get_DZoomMaxInfor(UINT32 *Dzoomtbl);//return 4 32-bit element
extern void Set_DZoomInfor(UINT32 *pdzoomtbl, UINT32 sen_ratio, UINT32 img_ratio,UINT32 disp_ratio, UINT32 disp_wind_fit);
extern void Set_digitalzoom(UINT32 DzoomIdx, UINT32 BitDepth);
extern void Set_DzoomMaxIndex(UINT32 index);
extern void SetImageOutSize(UINT32 Hsize, UINT32 Vsize);
extern UINT32 Get_DzoomIndex(void);

extern UINT32 Get_HRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
extern UINT32 Get_VRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
extern UINT32 Adj_VSize(UINT32 InSize, UINT32 VRatio, UINT32 Align);
extern UINT32 Adj_HSize(UINT32 InSize, UINT32 HRatio, UINT32 Align);
extern UINT32 Get_DzoomRatio(UINT32 DzoomIdx);

extern UINT32* SenMode2Tbl(UINT32 SenMode,UINT32 *DzMaxidx);
#endif
