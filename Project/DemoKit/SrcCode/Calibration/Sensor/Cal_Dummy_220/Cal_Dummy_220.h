#ifndef _CAL_MI5130_220_H_
#define _CAL_MI5130_220_H_

#define SENSOR_LSC_ENABLE DISABLE
#define SaveDPCoor DISABLE
/*-----------------------------------*/

#define Default_Agc_ISOMin          210     //MIN_AGC
#define Default_Mshutter            1000 //700
#define Default_AWB_Rgain           512
#define Default_AWB_Bgain           512
#define Default_FlashTriggerPoint   188562  //Lv8.5
#define Default_FlashAgc_Prv        100
#define Default_FlashAgc_Cap        100
#define Default_Min_OB              15
#define Default_Shading_Gain        1000
#define Default_Flash_Y_Avg         47
#define Default_FFidx               (156-40)

/*---------Defect Pixel define---------------------*/
//Exposure condition
#define EXPOSURE_COMPENSATE_RATIO   4   //prv 2 cap binning compensation
//DP_Adjust_White
#define DP_W_EXPOTIME_PRV   (16500 + 170) // 1/60 sec// (84*186+170) // 60 ms
#define DP_W_AGCGAIN_PRV    222//ISO100
#define DP_W_AGCGAIN_CAP    222
#define DP_NORMALY          60
#define DP_AE_ADJUST_STEP   (186 * 8) // 8 line per step
//DP_Adjust_Dark
#define DP_D_EXPOTIME_PRV   60000 // 60 ms
#define DP_D_AGCGAIN_PRV    360 // 18dB
#define DP_D_EXPOTIME_CAP   1000000 // 1 sec
#define DP_D_AGCGAIN_CAP    360 // 18dB

//Need not to change
#define Prv_Offset_X            1
#define Prv_Offset_Y            2

//Can modify if nessecary
#define Max_Continue_DP_Cap     4
#define Max_Continue_DP_Prv     2
#define Max_Continue_DP_Dzoom   2

// threshold
#define Bright_Limit_Prv    45
#define Bright_Limit_Cap    45
#define Dark_Limit_Prv      30
#define Dark_Limit_Cap      30

// detect area
#define Block_X_Prv         16
#define Block_Y_Prv         4
#define Block_X_Cap         32
#define Block_Y_Cap         32

//Have to modify if change sensor
#define SkipXL_Prv          0       // Skip the pixels in left
#define SkipXR_Prv          0     // Skip the pixels in right
#define SkipYU_Prv          0       // Skip the pixels in top
#define SkipYD_Prv          0      // Skip the pixels in button
#define SkipXL_Cap          0
#define SkipXR_Cap          0
#define SkipYU_Cap          0
#define SkipYD_Cap          0
#define SkipYU_Field_Cap    1
#define SkipYD_Field_Cap    1
//PS: 必須為2之倍數

#define DP_Prv_StartAdd     0x80ff0000
#define DP_Cap_StartAdd     0x80ff1000

/*--------AWB define----------*/
#define Rgain_Min 200 //256
#define Rgain_Max           1024
#define Bgain_Min 200 //256
#define Bgain_Max           1024
#define AWB_NG_ratio 110 //#NT#2009-01-06#Fincis

/*------------Flash define---------*/
#define FTP_Min         66667   //LV8
#define FTP_Max         266667  //LV10

#define FlashYAvgTarget 60      //實驗室灰牆 20041117
#define FLashYAvgMargin 20      //20041117

/*------------ISO define-----------*/
#define DGain_ISO100_Min    70
#define DGain_ISO100_Max    150
//#NT#2007/04/17#YC Peng -begin
//#modify ISO max/min
#define Agc_ISO100_Min       8//370 //(Default_Agc_ISO100-(AGC_Ev/2))
#define Agc_ISO100_Max      63//550 //(Default_Agc_ISO100+(AGC_Ev/2))
//#NT#2007/04/17#YC Peng -end
#define DGmin_Min           1
#define DGmin_Max           1000

/*---------Mshutter define-----------*/
#define Mshutter_Min        1000
#define Mshutter_Max        5000

/*------------OB define------------*/
#define OB_DFP_Threshold    30
#define OB_Threshold        32

//#NT#2007/04/17#YC Peng -begin
//#modify lens shading define
/*---------Lens Shading define------------*/
#define Shading_Gain_Max    1500
#define VigGain_Max         255
#define VigXcentRat_Min     90
#define VigXcentRat_Max     110//100
#define VigYcentRat_Min     90
#define VigYcentRat_Max     110//100
//#NT#2007/04/17#YC Peng -end
#define LSC_SIE_CAP_XCENTER    (4+0+(3264/2))
#define LSC_SIE_CAP_YCENTER    (5+0+(2448/2))

/**
    Structure of Sensor Adjust Data
*/
#define PS_CAL_HEAD             MAKE_PS_SECNAME_ID('C', 'A', 'L', 'H')
#define PS_CAL_DATA             MAKE_PS_SECNAME_ID('C', 'A', 'L', 'D')
#define PS_CAL_DATA2            MAKE_PS_SECNAME_ID('C', 'A', 'L', 'E')

#define DP_Prv_MAX 4000
#define DP_Cap_MAX 20000
#define DP_Dzoom_MAX 200

typedef enum
{
    PSTORE_SEC_SENSOR = 1,//ethan_20110801_chgstartidx
    PSTORE_SEC_DP,
    PSTORE_SEC_ECS,
    PSTORE_SEC_TOTAL,

    ENUM_DUMMY4WORD(PSTORE_SEC)
} PSTORE_SEC;

typedef enum
{
    DPC_PRV = 0,
    DPC_CAP,
    DPC_TOTAL
} DPC;

typedef enum
{
    ECS_FOV1 = 0,
    //ECS_FOV2,
    ECS_TOTAL
} ECS;
#define ECS_MAX (65*65*ECS_TOTAL)

typedef struct _DP_Struct{
    UINT Prv_White;
    UINT Prv_Black;
    UINT Cap_White;
    UINT Cap_Black;
    UINT Dzoom_White;
    UINT Dzoom_Black;
}DP_Struct;

typedef struct _AWB_Struct{
    UINT Rgain;
    UINT Bgain;
}AWB_Struct;

/**
  AAA_Adjust structure is flexible by project,
  so its prototype is in different folder instead
  in common file
*/
typedef struct _Adjust_Struct{
    // ISO
    UINT32 ISO_Status;
    UINT32 Agc_ISO_100;

    // AWB
    UINT32 AWB_Status;
    AWB_Struct AWB;
    AWB_Struct AWB_Prv;
    //AWBGS
    UINT32 AWBGS_Status;
    AWB_Struct AWBGS;
    // DP
    DP_Struct DP_Status;
    UW DPC_Addr[DPC_TOTAL];
    UW DPC_Num[DPC_TOTAL];
    //UW DP_Dzoom[DP_Dzoom_MAX];

    // Mshutter
    UINT32 Mshutter_Status;
    UINT32 Mshutter;

    UINT32 ECS_Status;
    UINT32 ECS_Addr[ECS_TOTAL];
} Adjust_Struct;
#endif
