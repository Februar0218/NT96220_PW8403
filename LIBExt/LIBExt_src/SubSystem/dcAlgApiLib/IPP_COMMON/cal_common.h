#ifndef _CAL_COMMON_H
#define _CAL_COMMON_H
#if 0
typedef unsigned char   UB;         /* Unsigned 8-bit integer       */
typedef unsigned short  UH;         /* Unsigned 16-bit integer      */
typedef unsigned long   UW;         /* Unsigned 32-bit integer      */

//----------------------------------------------------------------------------------------------------
//#define NandBlockSize    16000
#define AdjustDataNandAdd0 0xfb //After 4M
#define AdjustDataNandAdd1 0xfc
#define AdjustDataNandAdd2 0xfd
#define AdjustDataNandAdd3 0xfe
//----------------------------------------------------------------------------------------------------
#define Prv_Mode 0
#define Cap_Mode 1
#define Dzoom_Mode 2
#define Status_White 0
#define Status_Black 1
//----------------------------------------------------------------------------------------------------
/*
#define DP_Prv_MAX 500//200
#define DP_Cap_MAX 3000//500
#define DP_Dzoom_MAX 200
*/
//----------------------------------------------------------------------------------------------------
#define CAL_TRUE    1
#define CAL_FALSE   0

#define DP_SEARCH_TILL_END      0 // 1
#define DP_CHECKCONTINUEDP      0
#define DPC_SAVERAW_ENABLE      0
#define DPC_SAVEADJUST_ENABLE   0
#define AWB_SAVERAW_ENABLE      0
#define AWB_SAVEADJUST_ENABLE   0

//----------------------------------------------------------------------------------------------------
/*
typedef struct _Skip_Struct{
    UINT SkipXL;
    UINT SkipXR;
    UINT SkipYU;
    UINT SkipYD;
}Skip_Struct;

typedef struct _Block_Struct{
    UINT Block_X;
    UINT Block_Y;
}Block_Struct;

typedef struct _Sensor_Cal_Struct{
    UINT            DefaultAgcISOMin;
    UINT            DefaultMshutter;
    UINT            DefaultAWBRgain;
    UINT            DefaultAWBBgain;
    UINT            DefaultFlashTriggerPoint;
    UINT            DefaultMinOB;
    UINT            DefaultFlashYAvg;
    UINT            DarkLimitPrv;
    UINT            DarkLimitPrv2x;
    UINT            DarkLimitCap;
    UINT            BrightLimitPrv;
    UINT            BrightLimitPrv2x;
    UINT            BrightLimitCap;
    Block_Struct    BlockPrv;
    Skip_Struct     SkipPrv;
    Block_Struct    BlockPrv2x;
    Skip_Struct     SkipPrv2x;
    Block_Struct    BlockCap;
    Skip_Struct     SkipCap;
    UINT            MaxContDPPrv;
    UINT            MaxContDPCap;
    UINT            MaxContDPDzoom;
    UB              FlashYTarget;
    UB              FLashYMargin;
}Sensor_Cal_Struct;
*/
////////////////////////////////////////////////////////////////////////////////


//cal ipp/sensor mode switch api
extern ER Cal_SetMode(IPL_MODE mode, UINT32 uiSenMode);
#endif
#endif
