/**
    Auto White Balance Algorithm.

    depend on current image information to calculate white balance gain

    @file       Awb_Alg.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_ALG_H_
#define _AWB_ALG_H_


/**
     @name AWB Alg. default value
     @note
*/
//@{
#define AWBALG_TAB_END      0xffffffff    ///< AWB table end flag
#define AWBALG_WIN_X            16                                      ///< win number x
#define AWBALG_WIN_Y            16                                      ///< win number y
#define AWBALG_WIN_NUM          (AWBALG_WIN_X*AWBALG_WIN_Y)             ///< win number

//@}

/**
     check type.

     @note
*/
typedef enum _AWBALG_TYPE
{
    AWBALG_TYPE_Y = 0,
    AWBALG_TYPE_RG,
    AWBALG_TYPE_BG,
    AWBALG_TYPE_RBG,
    AWBALG_TYPE_SAT,
    AWBALG_TYPE_RBGSUM,
    AWBALG_TYPE_RXBG,
    AWBALG_TYPE_USER,
    AWBALG_TYPE_MAX,
    ENUM_DUMMY4WORD(AWBALG_TYPE)
}AWBALG_TYPE;


/**
     check type error.

     @note
*/
typedef enum _AWBALG_ER
{
    AWBALG_FAIL = 0,
    AWBALG_OK = 1,
    AWBALG_PAR = 2,
    AWBALG_Y = 100,
    AWBALG_RG = 110,
    AWBALG_BG = 120,
    AWBALG_RBG = 130,
    AWBALG_SAT = 140,
    AWBALG_RBGSUM = 150,
    AWBALG_RXBG = 160,
    AWBALG_USER = 170,
    AWBALG_OUTL = 180,//ethan_20110727
    AWBALG_MAX,
    ENUM_DUMMY4WORD(AWBALG_ER)
}AWBALG_ER;


/**
     AWB element.

     @note
*/
typedef struct _AWBALG_ELEMET
{
    AWBALG_TYPE Type;
    UINT32 ValH;
    UINT32 ValL;
}AWBALG_ELEMET;

/**
     AWB Table.

     @note
*/
typedef struct _AWBALG_TAB
{
    UINT32 EvH;
    UINT32 EvL;
    AWBALG_ELEMET *Element;
}AWBALG_TAB;

/**
     AWB input information.

     @note
*/
typedef struct _AWBALG_INFO
{
    UINT32 CurEV;
    UINT32 KRgain;
    UINT32 KBgain;
    UINT32 OutlR;
    UINT32 OutlB;
    UINT32 WThreshold;
    UINT32 RAcc[AWBALG_WIN_NUM];
    UINT32 GAcc[AWBALG_WIN_NUM];
    UINT32 BAcc[AWBALG_WIN_NUM];
    AWBALG_TAB *Tab;
    UINT32 TabCnt;
}AWBALG_INFO;



/**
     AWB algorithm event.
     @note
*/
typedef struct _AWBALG_EVENT_TAB
{
    //AWBALG_ChkWhite event
    AWBALG_ER (*UserChkWhite)(UINT32 R, UINT32 G, UINT32 B);

}AWBALG_EVENT_TAB;

/**

     calculate Rgain, Bgain

     @note

     @param[in] AWBALG_INFO
     @param[out] Rgain
     @param[out] Bgain
     @return
         - @b AWBALG_OK: SUCCESS
*/
extern AWBALG_ER AWBALG_ChkImg(AWBALG_INFO *Info, UINT32 *Rgain, UINT32 *Bgain);


/**

     load event table

     @note

     @param[in] event table(AWBALG_EVENT_TAB)
     @return
         - @b void.
*/
extern void AWBAlg_LoadEventTAB(AWBALG_EVENT_TAB *EventTab);

/**

     get weight table

     @note

     @param[out] total index
     @return
         - @b UINT32 table address.
*/
extern UINT32 AWBALG_GetWeightInfo(UINT32 *Total_Idx);
#endif //_AE_ALG_H_
