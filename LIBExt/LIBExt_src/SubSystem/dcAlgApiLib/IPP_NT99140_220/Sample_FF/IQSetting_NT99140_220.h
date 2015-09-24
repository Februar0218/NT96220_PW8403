#ifndef _IQSETTING_NT99140_220_H
#define _IQSETTING_NT99140_220_H

#include "IQSetting.h"


typedef enum
{
    ISONUM0, //ISO50
    ISONUM1, //ISO100
    ISONUM2, //ISO200
    ISONUM3, //ISO400
    Total_ISONum
}IPE_ISONumEnum;

//#NT#2011/08/22#Ethan Lau -begin
//#NT# ife d2d param group number.
#define IFE_RAW_Param  2
#define IFE_Y_Param    1
#define IFE_YCC_Param  2
//#NT#2011/08/22#Ethan Lau -end

extern __align(4) IPE_ISO_PARAM  IPE_ISOParam[Total_ISONum];
extern __align(4) STR_IPE_EDGE_ENHMAP_PARA IPE_EdgeParam[Total_ISONum];
extern __align(4) STR_IPE_NOVANR_PARA IPE_NRParam[Total_ISONum];
extern __align(4) INT16 IPE_edgeKernelDiv[Total_ISONum][10];
extern __align(4) INT16 IPE_AFedgeKernelDiv[10];
extern __align(4) UINT8 IPE_emStab[Total_ISONum][16];
extern __align(4) UINT8 IPE_fmStab[Total_ISONum][16];
extern __align(4) UINT8 IPE_fmDtab[Total_ISONum][16];
extern __align(4) UINT8 IPE_hueTab[24];
//#NT#2011/08/08#Yuppie Lin -begin
//#NT# modify signed varible to INT32
extern __align(4) INT32 IPE_satTab[24];
extern __align(4) INT32 IPE_intTab[24];
//#NT#2011/08/08#Yuppie Lin -end
extern __align(4) UINT8 IPE_ddsTab[8];
extern __align(4) BOOL IFE_Func[Total_ISONum][4];
extern __align(4) IFE_FILTER IFE_param_embeded[Total_ISONum];
extern __align(4) IFE_FILTER IFE_param_raw[Total_ISONum][IFE_RAW_Param+1];
extern __align(4) IFE_FILTER IFE_param_ycc[Total_ISONum][IFE_YCC_Param+1];
extern __align(4) IFE_FILTER IFE_param_y[Total_ISONum][IFE_Y_Param+1];


extern __align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1];

extern __align(4) UINT32  SRA_SIE_GammaLUT[65];

extern __align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1];
extern __align(4) INT16 Saturation[Total_ISONum][Max_Saturation+1];

extern __align(4)INT16 Normal_CC[9];
extern __align(4)INT16 Indenity_CC[9];



extern __align(4) UINT CHR_Param[5][28];


//extern __align(4)UINT32 GammaLUT_Y_128Tab[130];
//extern __align(4)UINT32 GammaLUT_128Tab[130];
//extern __align(4)UW ChrCurveLUT[257];
//extern __align(4)UW YCurveLUT[257];

#endif

