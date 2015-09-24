/**
    Auto white balance.


    @file       awb_sample_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_MI5130_220_INT_H_
#define _AWB_MI5130_220_INT_H_

/**
     @name AE default value
     @note
*/
//@{

#define AWB_RATE            4          ///< awb rate
#define AWB_TAB_CNT         2          ///< awb table number
#define AWB_DEF_KRGAIN      100
#define AWB_DEF_KBGAIN      100

#define AWB_DEF_RGAIN       375
#define AWB_DEF_GGAIN       256
#define AWB_DEF_BGAIN       340

#define AWB_OUTL_R          17
#define AWB_OUTL_B          17
#define AWB_W_THRES         3
//@}


/**
     Awb mode

     @note
*/
typedef enum _AWB_MODE
{
    AWB_MDOE_AUTO = 0,
    AWB_MODE_DAYLIGHT,
    AWB_MODE_CLOUDY,
    AWB_MODE_TUNGSTEN,
    AWB_MODE_FLUORESCENT1,
    AWB_MODE_FLUORESCENT2,
    AWB_MODE_SUNSET,
    AWB_MODE_BIRTHDAYPARTY,
    AWB_MODE_CUSTOMER1,
    AWB_MODE_CUSTOMER2,
    AWB_MODE_CUSTOMER3,
    AWB_MODE_MAX,
    ENUM_DUMMY4WORD(AWB_MODE)
}AWB_MODE;


extern AWBALG_TAB AwbTAB[AWB_TAB_CNT];
extern const UINT32 MwbTAB[AWB_MODE_MAX][2];

/**

     set color gain

     @note

     @param[in] R gain
     @param[in] G gain
     @param[in] B gain
*/
extern void AWB_setGain(UINT32 RGain, UINT32 GGain, UINT32 BGain);

/**
     get ca value

     @note

     @param[out] R acc value
     @param[out] G acc value
     @param[out] B acc value
     @param[in] window size

*/
extern void AWB_getCA(UINT32 *R, UINT32 *G, UINT32 *B, UINT32 Size);


/**
     awb mode map function
     @note

     @param[in] awb mode index
     @return
         - @b awb mode.
*/
extern AWB_MODE AWB_getMode(UINT32 Idx);


/**
     awb auto process
     @note
*/
extern void AWB_autoProc(AWBALG_INFO *Info);

/**
     awb manual process
     @note

     @param[in] awb mode index
*/
extern void AWB_manualProc(AWB_MODE Idx);


/**
     awb get preview color gain
     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void AWB_GetPrvGain(UINT32 *Rgain, UINT32 *Ggain, UINT32 *Bgain);
#endif //_AWB_SAMPLE_INT_H_
