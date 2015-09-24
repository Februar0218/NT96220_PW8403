/**
    Auto Exposure api.


    @file       ae_api.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_API_H_
#define _AE_API_H_


/**
     AE process event.
     @note
*/
typedef struct _AE_EVENT_TAB
{
    //AE_Process event
    UINT32 (*AeProcessEnd)(UINT32 ISO, UINT32 ExposureTime);

}AE_EVENT_TAB;



/**

     load event table

     @note

     @param[in] event table(AE_EVENT_TAB)
     @return
         - @b void.
*/
extern void AE_LoadEventTAB(AE_EVENT_TAB *EventTab);

/**

     ae init function
*/
extern void AE_Init(void);

/**

     ae process function
*/
extern void AE_Process (void);

/**

     ae get preview ae arg.

     @note

     @param[out] iso
     @param[out] exposure time
     @param[out] iris
*/
extern void AE_GetPrvAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris);

/**

     ae get capture ae arg.

     @note

     @param[out] iso
     @param[out] exposure time
     @param[out] iris
*/
extern void AE_GetCapAEArg(UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris);


/**

     ae wait stable

     @note

     @param[in] stable count
*/
extern void AE_WaitStable(UINT32 count);

/**

     ae write debug information

     @note

     @param[in] buffer start address
     @return
         - @b buffer end address
*/
extern UINT32 AE_WriteDebugInfo(UINT32 Addr);

/**

     get histogram

     @note

     @param[in] buffer address
*/
extern void AE_GetHisto(UINT32 *Hist);

/**

     load preview ae setting
*/
extern void AE_Cap2Prv(void);

/**

     get preview lum

     @note

     @param[in] buffer address
*/
extern UINT32 AE_GetPrvCurLum(void);
#endif //_AE_API_H_
