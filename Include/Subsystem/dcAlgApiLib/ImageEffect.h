#ifndef _IMAGEEFFECT_H
#define _IMAGEEFFECT_H

#include    "imageAlgInfor.h"

typedef enum
{
    IE_TYPE_SINGLE = 0,    
    IE_TYPE_MULTI = 1,
    IE_TYPE_CAPTURE = 2
    
} IE_TYPE;  //Image Effect Type

typedef enum
{
    IE_CH_1 = 0,    
    IE_CH_2 = 1,    
    IE_CH_3 = 2,
    IE_CH_4 = 3,
    IE_CH_MAX = 4
    
} IE_CH;  //Image Effect Channel

//#NT#2009/10/08#YC Peng -begin
//#NT#Add ARF sub-effect for EVARF/AWBARF mode
typedef enum
{
    IE_MODE_NULL    = 0,
    IE_MODE_EV      = 1,
    IE_MODE_AWB     = 2,
    IE_MODE_ARF     = 3,
    IE_MODE_EVARF   = 4,
    IE_MODE_AWBARF  = 5
    
} IE_MODE;  //Image Effect Mode
//#NT#2009/10/08#YC Peng -end

typedef enum
{
    IE_INDEX_NULL                   =   0,
        
    IE_INDEX_EV_N6                  = 100,
    IE_INDEX_EV_N5                  = 101,
    IE_INDEX_EV_N4                  = 102,
    IE_INDEX_EV_N3                  = 103,
    IE_INDEX_EV_N2                  = 104,
    IE_INDEX_EV_N1                  = 105,
    IE_INDEX_EV_00                  = 106,
    IE_INDEX_EV_P1                  = 107,
    IE_INDEX_EV_P2                  = 108,
    IE_INDEX_EV_P3                  = 109,
    IE_INDEX_EV_P4                  = 110,
    IE_INDEX_EV_P5                  = 111,
    IE_INDEX_EV_P6                  = 112,
    IE_INDEX_EV_MAX                 = 113,
    
    IE_INDEX_AWB_Auto               = 200,
    IE_INDEX_AWB_Sunny              = 201,
    IE_INDEX_AWB_Cloudy             = 202,
    IE_INDEX_AWB_Tungsten           = 203,
    IE_INDEX_AWB_Fluorescent1       = 204,
    IE_INDEX_AWB_Fluorescent2       = 205,
    IE_INDEX_AWB_Fluorescent3       = 206,
    IE_INDEX_AWB_MAX                = 207,
    
    IE_INDEX_ARF_PopArt             = 300,    
    IE_INDEX_ARF_ToyPhoto           = 301,
    IE_INDEX_ARF_FishEye            = 302,
    IE_INDEX_ARF_Sketch             = 303,
    IE_INDEX_ARF_FantasticFocus     = 304,
    IE_INDEX_ARF_RoughMonoChorme    = 305,
    IE_INDEX_ARF_MAX                = 306
    
} IE_INDEX;  //Image Effect Index

#define IE_INDEX_EV_MIN         100
#define IE_INDEX_AWB_MIN        200
#define IE_INDEX_ARF_MIN        300
#define IE_INDEX_EV_TOTAL       (IE_INDEX_EV_MAX - IE_INDEX_EV_MIN)
#define IE_INDEX_AWB_TOTAL      (IE_INDEX_AWB_MAX - IE_INDEX_AWB_MIN)
#define IE_INDEX_ARF_TOTAL      (IE_INDEX_ARF_MAX - IE_INDEX_ARF_MIN)

typedef enum
{
    FB_ID_0 = 0,    
    FB_ID_1 = 1,    
    FB_ID_2 = 2,
    FB_ID_MAX = 3
    
} FB_ID;  //Frame buffer ID

typedef struct _FBInfo
{
    UINT32  YAddr[FB_ID_MAX];
    UINT32  CbAddr[FB_ID_MAX];
    UINT32  CrAddr[FB_ID_MAX];
    UINT32  HSize;
    UINT32  VSize;
    UINT32  LineOffset;
    UINT32  Format;
    
}   FBInfo;

typedef struct _ChannelInfo
{              
    IE_INDEX    Index;          
    UINT32      Active;         
    FBInfo      FBInfo;
    
}   ChannelInfo;

//#NT#2009/10/08#YC Peng -begin
//#NT#Add ARF sub-effect for EVARF/AWBARF mode
typedef struct _ImgEffectInfo
{
    IE_TYPE     Type;                       // ImgEffect Type
    IE_MODE     Mode;                       // ImgEffect Mode
    FBInfo      SourceFBInfo;               // ImgEffect Source FB Info
    FBInfo      DisplayFBInfo;              // ImgEffect Display FB Info
    ChannelInfo CaptureCH;                  // ImgEffect ChannelInfo Capture
    ChannelInfo SingleCH;                   // ImgEffect ChannelInfo Single
    ChannelInfo MultiCH[IE_CH_MAX];         // ImgEffect ChannelInfo CH1/2/3/4
    IE_INDEX    MultiCHARFSubIndex;         // ImgEffect ChannelInfo CH1/2/3/4 for ARF Sub-effect
    FB_ID       ChannelFBActID;             // ImgEffect Channel FB Active ID   
    FB_ID       ChannelFBRdyID;             // ImgEffect Channel FB Ready ID
    UINT32      ChannelFBLock[FB_ID_MAX];   // ImgEffect Channel FB Lock
    UINT32      PrvStatus;                  // ImgEffect Status Preview
    UINT32      CapStatus;                  // ImgEffect Status Capture
    
}   ImgEffectInfo;
//#NT#2009/10/08#YC Peng -end

//#NT#2009/10/08#YC Peng -begin
//#NT#Add ARF sub-effect for EVARF/AWBARF mode
typedef struct _ImgEffectSetting
{
    IE_TYPE         Type;
    IE_MODE         Mode;
    UINT32          SourceFBHsize;
    UINT32          SourceFBVsize;
    UINT32          SourceFBLineoffset;
    UINT32          DisplayFBHsize;
    UINT32          DisplayFBVsize;
    UINT32          DisplayFBLineoffset;
    IE_INDEX        CaptureChannelIndex;
    IE_INDEX        SingleChannelIndex;
    IE_INDEX        MultiChannelIndex[IE_CH_MAX];
    IE_INDEX        MultiChannelARFSubIndex;
    
}   ImgEffectSetting;
//#NT#2009/10/08#YC Peng -end

typedef struct _ImgEffectShadingJPG
{
    UINT8*      BitStreamAddr;                  
    UINT32      BitStreamSize;
    UINT32      Hsize;
    UINT32      Vsize;
    UINT32      LineOffset;
    
}   ImgEffectShadingJPG;

//#NT#2009/11/06#YC Peng -begin
//#NT#Add image ratio function for image effect
//#NT#2009/10/30#YC Peng -begin
//#NT# Add double buffer for Single view display 
//#NT#2009/10/08#YC Peng -begin
//#NT#Add ARF sub-effect for EVARF/AWBARF mode
extern ER       IE_EnableImgEffect(ImgEffectSetting* IE_Setting);
extern ER       IE_InitCaptureImgEffect(ImgEffectSetting* IE_Setting);    
extern ER       IE_InitSingleImgEffect(ImgEffectSetting* IE_Setting);
extern ER       IE_InitMultiImgEffect(ImgEffectSetting* IE_Setting);
extern void     IE_DisableImgEffect(void);
extern void     IE_SetPrvStatus(BOOL PrvStatus);
extern BOOL     IE_GetPrvStatus(void);
extern IE_MODE  IE_GetPrvMode(void);
extern void     IE_SetCapStatus(BOOL CapStatus);
extern BOOL     IE_GetCapStatus(void);
extern IE_INDEX IE_GetCapIndex(void);
extern FB_ID    IE_LockRdyFB(void);
extern void     IE_UnLockRdyFB(void);
extern void     IE_SetMultiChannelIndex(IE_CH Channel, IE_INDEX CH_Index);
extern void     IE_GetMultiChannelFBInfo(IE_CH Channel, FBInfo* FB_Info);
extern void     IE_GetSingleChannelFBInfo(FBInfo* FB_Info);
extern void     IE_GetDisplayFBInfo(FBInfo* FB_Info);
extern void     IE_PrvProcess(void);
extern void     IE_MultiPrvProcess(void);
extern void     IE_SinglePrvProcess(void);
extern UINT32   IE_InitMultiEV(UINT32 BufAddr);
extern UINT32   IE_InitMultiEVARF(UINT32 BufAddr);
extern UINT32   IE_InitMultiAWB(UINT32 BufAddr);
extern UINT32   IE_InitMultiAWBARF(UINT32 BufAddr);
extern UINT32   IE_InitMultiARF(UINT32 BufAddr);
extern UINT32   IE_InitSingleARF(UINT32 BufAddr);
extern UINT32   IE_InitCaptureARF(void);
extern void     IE_ProcessMultiEV(UINT32 SrcFBID, UINT32 ChannelActFBID);
extern void     IE_ProcessMultiEVARF(UINT32 SrcFBID, UINT32 ChannelActFBID);
extern void     IE_updateMultiAWB(void);
extern void     IE_ProcessMultiAWB(UINT32 SrcFBID, UINT32 ChannelActFBID);
extern void     IE_ProcessMultiAWBARF(UINT32 SrcFBID, UINT32 ChannelActFBID);
extern void     IE_ProcessMultiARF(UINT32 SrcFBID, UINT32 ChannelActFBID);
extern void     IE_ProcessSingleARF(UINT32 SrcFBID, UINT32 ChannelActFBID);
extern void     IE_ProcessCaptureARF(void);
extern void     IE_MultiGammaLUT(UINT32* CHAddr, UINT8** CHGammaLUT, UINT32 CHNum, UINT32 SrcAddr, UINT32 SrcHsize, UINT32 SrcVsize);
extern void     IE_SetShadingJPG(ImgEffectShadingJPG* IE_ShadingJPG);
extern UINT32   IE_GetShadingJPGBufSize(void);
//#NT#2009/10/08#YC Peng -end
//#NT#2009/10/30#YC Peng -end
//#NT#2009/11/06#YC Peng -end

#define IE_PRV_MAX_SIZE     (640 * 576 * 2 * 3) * 2
#define IE_DISPLAY_MAX_SIZE (640 * 576 * 2 * 3)

#endif
