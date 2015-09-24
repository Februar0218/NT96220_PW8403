#include <string.h>
#include <stdio.h>
#include "debug.h"
#include "Type.h"
#include "ImageAlgInfor.h"
#include "dzoom.h"
#include "ipe_lib.h"
#include "sen_common.h"
//define
#define DisplayerUseIDEScale DISABLE// ENABLE//use ime if disable

typedef struct _IMEIDESIZE
{
    _IMGRatio_sel   ImgRatio;
    UINT32          DispBufH;
    UINT32          DispBufV;
}IMEIDESIZE;

//static var.
static UINT32 MAX_Dzoom_Step = 11;

//#NT#2012/03/29#Harry Tsai -begin
//#NT#Extend the ratio table
/*
//  4:3                 3:2                 16:9
const UINT32 usImgRatioTable[_Ratio_Num][_Ratio_Num] =
{
    {IMG_RATIO_UINT,    228,                192},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,     216},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,     IMG_RATIO_UINT}
};
*/
 
//  9:16                2:3                3:4                1:1                4:3                3:2                16:9
const UINT32 usImgRatioTable[_Ratio_Num][_Ratio_Num] =
{
    {IMG_RATIO_UINT,    216,               192,               144,               108,               96,                80},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,    228,               172,               128,               114,               96},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    192,               144,               128,               108},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    192,               172,               144},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    228,               192},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    216},
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT,    IMG_RATIO_UINT},
};
//#NT#2012/03/29#Harry Tsai -end


static DZOOM_INFOR VDOZOOM_INFOR = {0};

static UINT32 zoom_index = 10;
SIZE_INFOR    LCD_Size, JPEG_Pre_Size,JPEG_Cap_Size;
SIZE_INFOR    IPE_Out_Prv_Size, IPE_Out_Cap_Size;
SIZE_INFOR    PRE_Hsub_Prv_Size, PRE_Hsub_Cap_Size;
SIZE_INFOR    IME_Out_Prv_Size, IME_Out_Cap_Size;
SIZE_INFOR    PRE_Crop_Prv_Size, PRE_Crop_Cap_Size;

__inline static void GetIMEOutSize(IMEIDESIZE* info,UINT32* outH,UINT32* outV)
{
    *outH = info->DispBufH;
    *outV = info->DispBufV*usImgRatioTable[0][info->ImgRatio]/IMG_RATIO_UINT;
}

UINT32 Get_DzoomRatio(UINT32 DzoomIdx)
{
    UINT32 ratio;
    if(DzoomIdx < 10)
    {
        DzoomIdx = 10;
    }
    else if(DzoomIdx > Get_DzoomMaxIndex())
    {
        DzoomIdx = Get_DzoomMaxIndex();
    }
    ratio = (VDOZOOM_INFOR.DzTable[0][1] * 100) /VDOZOOM_INFOR.DzTable[DzoomIdx - 10][1];
    //#NT#2011/10/04#Ethan Lau -begin
    //#NT#enhance accurency
    //ratio /= 10;
    //#NT#2011/10/04#Ethan Lau -end
    return ratio;
}

void Set_DzoomIndex(INT idx)
{
    zoom_index = idx;
}

UINT32 Get_DzoomIndex(void)
{
    return zoom_index;
}

void Set_DzoomMaxIndex(UINT32 index)
{
    MAX_Dzoom_Step = index;
}

UINT32 Get_DzoomMaxIndex(void)
{
    return MAX_Dzoom_Step;
}

void Set_DZoomInfor(UINT32 *pdzoomtbl, UINT32 sen_ratio, UINT32 img_ratio,UINT32 disp_ratio, UINT32 disp_wind_fit)
{
    VDOZOOM_INFOR.SensorRatio = sen_ratio;
    VDOZOOM_INFOR.ImgRatio = img_ratio;
    VDOZOOM_INFOR.DispRatio = disp_ratio;
    VDOZOOM_INFOR.DispWindFit = disp_wind_fit;
    VDOZOOM_INFOR.DzTable = (UINT32(*)[6])pdzoomtbl;
    SetIPPSizeInfor(_SensorRatioIdx, VDOZOOM_INFOR.SensorRatio);
}

void Get_DZoomMaxInfor(UINT32 *Dzoomtbl)
{
    UINT32 i,  AdjSize;
    UINT32 HRatio, VRatio;

    for (i = 0; i < 6; i ++)
        Dzoomtbl[i] = VDOZOOM_INFOR.DzTable[0][i];

    if (GetIPPSizeInfor(_SensorRatioIdx) != VDOZOOM_INFOR.ImgRatio)
    {
        //get adjust ratio
        HRatio = Get_HRatio(VDOZOOM_INFOR.ImgRatio, GetIPPSizeInfor(_SensorRatioIdx));
        VRatio = Get_VRatio(VDOZOOM_INFOR.ImgRatio, GetIPPSizeInfor(_SensorRatioIdx));

        if ((HRatio != IMG_RATIO_UINT) && (VRatio == IMG_RATIO_UINT))
        {
            Dzoomtbl[0] = Adj_HSize(Dzoomtbl[0], HRatio, 16);
            AdjSize = Dzoomtbl[0];

            if (AdjSize > VDOZOOM_INFOR.DzTable[0][2])
            {
                Dzoomtbl[2] = VDOZOOM_INFOR.DzTable[0][2];
            }
            else
            {
                Dzoomtbl[2] = AdjSize / 16 * 16;
            }

            if (AdjSize > VDOZOOM_INFOR.DzTable[0][4])
            {
                Dzoomtbl[4] = VDOZOOM_INFOR.DzTable[0][4];
            }
            else
            {
                Dzoomtbl[4] = AdjSize / 16 * 16;
            }
            //AdjTable[0][4] = AdjTable[0][2] - IPE_HSIZE_IODIFF;
        }
        else if ((HRatio == IMG_RATIO_UINT) && (VRatio != IMG_RATIO_UINT))
        {
            Dzoomtbl[1] = Adj_VSize(Dzoomtbl[1], VRatio, 4);
            //AdjTable[0][7] = Adj_VSize(AdjTable[0][7], VRatio, 4);
            //#NT#2011/06/17#Jarkko Chang -begin
            //#NT# avoid size error in 16:9 mode
            Dzoomtbl[3] = Dzoomtbl[1];
            //#NT#2011/06/17#Jarkko Chang -end
            Dzoomtbl[5] = Dzoomtbl[1];
        }
        else
        {
            debug_err(("dzoom..not support\r\n"));
        }
    }
    if (VDOZOOM_INFOR.ImgRatio!= VDOZOOM_INFOR.DispRatio)
    {
        HRatio = Get_HRatio(VDOZOOM_INFOR.ImgRatio, VDOZOOM_INFOR.DispRatio);
        VRatio = Get_VRatio(VDOZOOM_INFOR.ImgRatio, VDOZOOM_INFOR.DispRatio);
    
        if(VDOZOOM_INFOR.DispWindFit == _WindowFitAuto)
        {
            Dzoomtbl[6] = Adj_HSize(DispSizeInfo.uiDisplayFBWidth, HRatio, 16);
            Dzoomtbl[7] = Adj_VSize(DispSizeInfo.uiDisplayFBHeight, VRatio, 4);
        }
        else if(VDOZOOM_INFOR.DispWindFit == _WindowFitHori)
        {
            Dzoomtbl[6] = Adj_HSize(DispSizeInfo.uiDisplayFBWidth, IMG_RATIO_UINT, 16);
            Dzoomtbl[7] = Adj_VSize(DispSizeInfo.uiDisplayFBHeight, VRatio*IMG_RATIO_UINT/HRatio, 4);
        }
        else if(VDOZOOM_INFOR.DispWindFit == _WindowFitVert)
        {
            Dzoomtbl[6] = Adj_HSize(DispSizeInfo.uiDisplayFBWidth, HRatio*IMG_RATIO_UINT/VRatio, 16);
            Dzoomtbl[7] = Adj_VSize(DispSizeInfo.uiDisplayFBHeight, IMG_RATIO_UINT, 4);
        }
    }
    else
    {
        Dzoomtbl[6] = DispSizeInfo.uiDisplayFBWidth;
        Dzoomtbl[7] = DispSizeInfo.uiDisplayFBHeight;
    }
    //debug_err(("Ratio: %d,%d,%d, Dzoom6:%d, 7:%d\r\n",GetIPPSizeInfor(_SensorRatioIdx),VDOZOOM_INFOR.ImgRatio,VDOZOOM_INFOR.DispRatio,Dzoomtbl[6],Dzoomtbl[7]));

}

UINT32 Get_HRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx)
{
    return usImgRatioTable[ImgRatioIdx][DefaultRatioIdx];
}

UINT32 Get_VRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx)
{
    return usImgRatioTable[DefaultRatioIdx][ImgRatioIdx];
}

UINT32 Adj_VSize(UINT32 InSize, UINT32 VRatio, UINT32 Align)
{
    UINT32 rtValue;

    rtValue = InSize * VRatio / IMG_RATIO_UINT;
    rtValue = (rtValue + (Align - 1)) / Align * Align;
    return rtValue;
}

UINT32 Adj_HSize(UINT32 InSize, UINT32 HRatio, UINT32 Align)
{
    UINT32 rtValue;

    rtValue = InSize;
    rtValue = InSize * HRatio / IMG_RATIO_UINT;
    rtValue = (rtValue + (Align - 1)) / Align * Align;
    return rtValue;
}
//#NT#2011/04/29#Ethan Lau -begin
//#NT#220 have no pre
/*
UINT32 Adj_PreLineOffset(UINT32 LineOffset, SENSORFormatEnum VGAType, UINT8 PreBitDepth)
{
    UINT32 lofs;

    lofs = LineOffset * PreBitDepth / 8;
    if (VGAType == SENSOR_OUT_THREE_LINE)
    {
        lofs = (lofs + (PRE_BLOCK_BUF_SIZE/3 - 1)) / (PRE_BLOCK_BUF_SIZE/3) * (PRE_BLOCK_BUF_SIZE/3);
    }
    else if (VGAType == SENSOR_OUT_TWO_LINE)
    {
        lofs = (lofs + (PRE_BLOCK_BUF_SIZE/2 - 1)) / (PRE_BLOCK_BUF_SIZE/2) * (PRE_BLOCK_BUF_SIZE/2);
    }
    return lofs;
}
*/
//#NT#2011/04/29#Ethan Lau -end
void Set_digitalzoom(UINT32 DzoomIdx, UINT32 BitDepth)
{
#if DisplayerUseIDEScale
    UINT32 Disp_H=0,Disp_V=0;
    IMEIDESIZE  info={0};
#endif
    UINT32 lofs, i, idx, AdjSize;
    UINT32 (*DzTable)[6];
    UINT32 HRatio, VRatio, AdjTable[1][6];

    //#NT#2010/12/21#ethanlau -begin
    //avoid dzoom max index error causeed by dzoom talbe max size different between different tbl,index is start from 10 not 0
    if(DzoomIdx < 10)
    {
        DzoomIdx = 10;
    }
    else if(DzoomIdx > Get_DzoomMaxIndex())
    {
        DzoomIdx = Get_DzoomMaxIndex();
    }
    //#NT#2010/12/21#ethanlau -end
    DzTable = (UINT32(*)[6])VDOZOOM_INFOR.DzTable;
    SetIPPSizeInfor(_SensorRatioIdx, VDOZOOM_INFOR.SensorRatio);
    //for image pipeline
    if (GetIPPSizeInfor(_SensorRatioIdx) != VDOZOOM_INFOR.ImgRatio)
    {
        //get adjust ratio
        HRatio = Get_HRatio(VDOZOOM_INFOR.ImgRatio, GetIPPSizeInfor(_SensorRatioIdx));
        VRatio = Get_VRatio(VDOZOOM_INFOR.ImgRatio, GetIPPSizeInfor(_SensorRatioIdx));

        for (i = 0; i < 6; i ++) AdjTable[0][i] = DzTable[DzoomIdx - 10][i];

        if ((HRatio != IMG_RATIO_UINT) && (VRatio == IMG_RATIO_UINT))
        {
            //AdjTable[0][6] = Adj_HSize(AdjTable[0][6], HRatio, 16);

            AdjTable[0][0] = Adj_HSize(AdjTable[0][0], HRatio, 16);
            AdjSize = AdjTable[0][0];

            if (AdjSize > DzTable[DzoomIdx - 10][2])//SIE output size
            {
                AdjTable[0][2] = DzTable[DzoomIdx - 10][2];
            }
            else
            {
                AdjTable[0][2] = AdjSize;
            }

            if (AdjSize > DzTable[DzoomIdx - 10][4])// IPE input size
            {
                AdjTable[0][4] = DzTable[DzoomIdx - 10][4];
            }
            else
            {
                AdjTable[0][4] = AdjSize;
            }
            //AdjTable[0][4] = AdjTable[0][2] - IPE_HSIZE_IODIFF;
        }
        else if ((HRatio == IMG_RATIO_UINT) && (VRatio != IMG_RATIO_UINT))
        {
            AdjTable[0][1] = Adj_VSize(AdjTable[0][1], VRatio, 4);
            //AdjTable[0][7] = Adj_VSize(AdjTable[0][7], VRatio, 4);
            //#NT#2011/06/17#Jarkko Chang -begin
            //#NT# avoid size error in 16:9 mode
            AdjTable[0][3] = AdjTable[0][1];
            //#NT#2011/06/17#Jarkko Chang -end
            AdjTable[0][5] = AdjTable[0][1];
        }
        else
        {
            debug_err(("dzoom..not support\r\n"));
        }
        idx = 0;
        DzTable = (UINT32(*)[6])AdjTable;
    }
    else
    {
        idx = DzoomIdx - 10;
    }
#if DisplayerUseIDEScale
    //#NT#2011/11/17#Ethan Lau -begin
    //#NT#in order to maintain ime out image ratio for FD,use ide to scaling image to fit panel if needed
    info.ImgRatio = VDOZOOM_INFOR.ImgRatio;
    info.DispBufH = GetDisplayH();
    info.DispBufV = GetDisplayV();
    GetIMEOutSize(&info,&Disp_H,&Disp_V);

    SetIPPSizeInfor(_ImeOut1Hsize, Disp_H);
    SetIPPSizeInfor(_ImeOut1Vsize, Disp_V);
    SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    //#NT#2011/11/17#Ethan Lau -end
#else
    if (VDOZOOM_INFOR.ImgRatio!= VDOZOOM_INFOR.DispRatio)
    {
        //get adjust ratio
            HRatio = Get_HRatio(VDOZOOM_INFOR.ImgRatio, VDOZOOM_INFOR.DispRatio);
            VRatio = Get_VRatio(VDOZOOM_INFOR.ImgRatio, VDOZOOM_INFOR.DispRatio);

            //#NT#2011/06/17#Jarkko Chang -begin
            //#NT# avoid size error in 16:9 mode recording
            if(GetIPPFuncInfor(JPG_DCOUT_EN))
            {
                SetIPPSizeInfor(_ImeOut1Hsize, DispSizeInfo.uiDisplayFBWidth);
                SetIPPSizeInfor(_ImeOut1Vsize, DispSizeInfo.uiDisplayFBHeight);
                SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
            }
            else
            {
                if(VDOZOOM_INFOR.DispWindFit == _WindowFitAuto)
                {
                    SetIPPSizeInfor(_ImeOut1Hsize, Adj_HSize(DispSizeInfo.uiDisplayFBWidth, HRatio, 16));
                    SetIPPSizeInfor(_ImeOut1Vsize, Adj_VSize(DispSizeInfo.uiDisplayFBHeight, VRatio, 4));
                    SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
                }
                else if(VDOZOOM_INFOR.DispWindFit == _WindowFitHori)
                {
                    SetIPPSizeInfor(_ImeOut1Hsize, Adj_HSize(DispSizeInfo.uiDisplayFBWidth, IMG_RATIO_UINT, 16));
                    SetIPPSizeInfor(_ImeOut1Vsize, Adj_VSize(DispSizeInfo.uiDisplayFBHeight, VRatio*IMG_RATIO_UINT/HRatio, 4));
                    SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
                }
                else if(VDOZOOM_INFOR.DispWindFit == _WindowFitVert)
                {
                    SetIPPSizeInfor(_ImeOut1Hsize, Adj_HSize(DispSizeInfo.uiDisplayFBWidth, HRatio*IMG_RATIO_UINT/VRatio, 16));
                    SetIPPSizeInfor(_ImeOut1Vsize, Adj_VSize(DispSizeInfo.uiDisplayFBHeight, IMG_RATIO_UINT, 4));
                    SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
                }
            }
            //#NT#2011/06/17#Jarkko Chang -end
    }
    else
    {
        SetIPPSizeInfor(_ImeOut1Hsize, DispSizeInfo.uiDisplayFBWidth);
        SetIPPSizeInfor(_ImeOut1Vsize, DispSizeInfo.uiDisplayFBHeight);
        SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    }
#endif

    lofs = DzTable[idx][2] * BitDepth/8;
    SetIPPSizeInfor(_SieOutLineOffset, lofs);
    SetIPPSizeInfor(_IpeOutHsize,DzTable[idx][4] - IPE_HSIZE_IODIFF);
    SetIPPSizeInfor(_IpeOutVsize,DzTable[idx][5]);
    SetIPPSizeInfor(_SieOutHsize,DzTable[idx][2]);
    SetIPPSizeInfor(_SieOutVsize,DzTable[idx][3]);
    SetIPPSizeInfor(_SieCropHsize,DzTable[idx][0]);
    SetIPPSizeInfor(_SieCropVsize,DzTable[idx][1]);

    SEN_MSG(("Dzoom: %d %d(%d %d) %d %d(%d %d) \r\n",GetIPPSizeInfor(_SieCropHsize),GetIPPSizeInfor(_SieCropVsize)
        ,GetIPPSizeInfor(_SieOutHsize),GetIPPSizeInfor(_SieOutVsize)
        ,GetIPPSizeInfor(_IpeOutHsize),GetIPPSizeInfor(_IpeOutVsize)
        ,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize)));
}

void SetImageOutSize(UINT32 Hsize, UINT32 Vsize)
{
    SetIPPSizeInfor(_JpegCapHsize,Hsize);
    SetIPPSizeInfor(_JpegCapVsize,Vsize);
}
