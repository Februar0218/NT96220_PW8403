/**
    Auto Exposure Algorithm.

    depend on current Luminance to calculate LV, ISO, Exposure Time, and Iris.

    @file       Ae_Alg.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_ALG_H_
#define _AE_ALG_H_


/**
     @name AE Alg. default value
     @note
*/
//@{
#define AEALG_TAB_END   0xffffffff      ///< AE table end flag
#define AEALG_HIST_NUM  64              ///< histogram max number
//@}

/**
     AEALG error code.

     @note
*/
typedef enum _AEALG_ER
{
    AEALG_OK = 0,         ///< AE OK
    AEALG_PAR,            ///< AE Parameter error
    AEALG_MAX,
    ENUM_DUMMY4WORD(AEALG_ER)
}AEALG_ER;

/**
     Iris index.

     big iris -> small iris
     AEALG_IRIS_0 -> AEALG_IRIS_1 -> ....... -> AEALG_IRIS_8
     @note
*/
typedef enum _AEALG_IRIS_IDX
{
    AEALG_IRIS_0 = 0, ///< iris index 0
    AEALG_IRIS_1,     ///< iris index 1
    AEALG_IRIS_2,     ///< iris index 2
    AEALG_IRIS_3,     ///< iris index 3
    AEALG_IRIS_4,     ///< iris index 4
    AEALG_IRIS_5,     ///< iris index 5
    AEALG_IRIS_6,     ///< iris index 6
    AEALG_IRIS_7,     ///< iris index 7
    AEALG_IRIS_8,     ///< iris index 8
    AEALG_IRIS_MAX,
    ENUM_DUMMY4WORD(AEALG_IRIS_IDX)
}AEALG_IRIS_IDX;


/**
     AE table information.

     AE table information.
     @note
*/
typedef enum _AEALG_TABLE_INFO
{
    AEALG_EV = 0,          ///< EV value
    AEALG_EXP,             ///< Exposure time
    AEALG_ISO,             ///< ISO value
    AEALG_IRIS,            ///< IRIS value
    AEALG_ADJ_TARGET,      ///< adjust flag
    AEALG_INFO_MAX,
    ENUM_DUMMY4WORD(AEALG_TABLE_INFO)
} AEALG_TABLE_INFO;

/**
     @name Ae adjust flag(AEALG_ADJ_TARGET)
     @note
*/
//@{
#define AEALG_ADJ_EXP           0x01    ///< Adjust exposure time
#define AEALG_ADJ_GAIN          0x02    ///< Adjust iso
#define AEALG_ADJ_IRIS          0x04    ///< Adjust iris idx
//@}

/**
     AE argument.

     AE argument.
     @note
*/
typedef struct _AEALG_ARG
{
    UINT32 ExpoTime;        ///< Exposure Time(us)
    UINT32 ISOGain;         ///< ISO value(50, 100, 200......)
    UINT32 Iris;            ///< Iris Value(idx)
}AEALG_ARG;


/**
     AE over exposure information.

     AE over exposure information.
     @note
*/
typedef struct _AEALG_REDUCE_OVER_EXPO_INFO
{
    UINT32 OutdoorEV;               ///< outdoor EV
    UINT32 IndoorEV;                ///< indoor EV
    UINT32 NightEV;                 ///< night EV
    UINT32 OutdoorMaxAdjY;          ///< outdoor max adj Y
    UINT32 IndoorMaxAdjY;           ///< indoor max adj Y
    UINT32 NightMaxAdjY;            ///< night max adj Y
    UINT32 OverExpoTh;              ///< over exopsure threshold
    UINT32 UnderExpoTh;             ///< under exopsure threshold
    UINT32 CenterOverExpoTh;        ///< over exopsure threshold for center area
    UINT32 AdjYStep;                ///< adjust Y step
} AEALG_REDUCE_OVER_EXPO_INFO;


/**
     AE Information.

     AE Information.
     @note
*/
typedef struct _AE_INFO
{
    UINT32 ReduceOverExpoModeEn;                ///< reduce over exposure mode switch
    UINT32 EVAdjEn;                             ///< ev +0 or not
    UINT32 LumY;                                ///< current luminance
    UINT32 MinEV;                               ///< min ev
    UINT32 MaxEV;                               ///< max ev
    UINT32 MinISOValue;                         ///< min iso
    UINT32 MaxISOValue;                         ///< max iso
    UINT32 CovRangeH;                           ///< cover range tolerance (100 + H) %
    UINT32 CovRangeL;                           ///< cover range tolerance (100 + L) %
    UINT32 Hist[AEALG_HIST_NUM];                ///< histogram
    UINT32 Reset;                               ///< Reset Flag
    UINT32(* PrvCurve)[ AEALG_INFO_MAX ];       ///< preview Ae curve
    UINT32(* CapCurve)[ AEALG_INFO_MAX ];       ///< capture Ae curve
    UINT32 PrvBiningMode;                       ///< bining mode 1x, 2x, 3x, 4x......(x 100)
    UINT32 CapBiningMode;                       ///< bining mode 1x, 2x, 3x, 4x......(x 100)
    UINT32 ExpectY;                             ///< expected luminance
    UINT32 AdjRatioH;                           ///< adjust ratio boundary
    UINT32 AdjRatioL;                           ///< adjust ratio boundary
    UINT32 EVPrvWeight;                         ///< previous weight
    UINT32 EVNewWeight;                         ///< last weight
    AEALG_REDUCE_OVER_EXPO_INFO OverExpoInfo;   ///< reduce over exposure information
} AE_INFO;

/**
     AE algorithm event.
     @note
*/
typedef struct _AEALG_EVENT_TAB
{
    //AEAlg_getratio event
    UINT32 (*GetOverExpoRatio)(UINT32 HistMaxNum, UINT32 *Hist);
    UINT32 (*GetCenterOverExpoRatio)(UINT32 HistMaxNum, UINT32 *Hist);

    //AEAlg_Allot event(Fno x Fno x 10)
    UINT32 (*GetFnoRatio)(AEALG_IRIS_IDX Idx);

    //AEAlg_Allot event((FnoA^2 / FnoB^2) x 100)
    //FnoA = (Wide, big iris)
    //FnoB = (Current Section, big iris)
    UINT32 (*GetCurveOfsRatio)(void);
}AEALG_EVENT_TAB;


/**

     AEAlg_allot function

     depend on EV value to calculate iso, exposure time, and iris.

     @note

     @param[in] EVValue    Current EVValue.
     @param[in] AECurve    AE Curve.
     @param[out] Info    iso, exposure time, and iris.
     @return
         - @b AEALG_OK:   done with no error.
         - Others: Error occured.
*/
extern AEALG_ER AEAlg_Allot(UINT32 EVValue, UINT32 (*AeCurve)[AEALG_INFO_MAX], AEALG_ARG *Info);

/**

     load event table

     @note

     @param[in] event table(AEALG_EVENT_TAB)
     @return
         - @b void.
*/
extern void AEAlg_LoadEventTAB(AEALG_EVENT_TAB *EventTab);


/**

     converage luminance

     @note

     @param[in] AE_INFO(AEInfo)
     @param[out] preview iso, exposure time, iris
     @param[out] capture iso, exposure time, iris
     @return
         - @b void.
*/
extern void AEAlg_Converge(AE_INFO *AEInfo, AEALG_ARG *PrvInfo, AEALG_ARG *CapInfo);


/**

     set preview ev value

     @note

     @param[in] ev vaule
     @return
         - @b void.
*/
extern void AEAlg_SetPrvEVValue(UINT32 EVValue);

/**

     get stable counter

     @note

     @return
         - @b ae stable counter.
*/
extern UINT32 AEAlg_GetStbCnt(void);

/**

     get EV value

     @note

     @return
         - @b ev value.
*/
extern UINT32 AEAlg_GetPrvEVValue(void);

/**

     get preview LV value

     @note

     @return
         - @b lv value.
*/
extern UINT32 AEAlg_GetPrvLV(void);

/**

     get capture EV value

     @note

     @return
         - @b EV value.
*/
extern UINT32 AEAlg_GetCapEVValue(void);

/**

     get capture LV value

     @note

     @return
         - @b lv value.
*/
extern UINT32 AEAlg_GetCapLV(void);
/**

     get current expect y

     @note

     @return
         - @b expect y.
*/
extern UINT32 AEAlg_GetAdjExpectY(void);

/**

     get current adjust y

     @note

     @return
         - @b adjust y.
*/
extern UINT32 AEAlg_GetAdjY(void);

/**

     check current EV is boundary EV

     @note

     @return
         - @b TRUE or FALSE.
*/
extern UINT32 AEAlg_IsEVBound(void);
#endif //_AE_ALG_H_
