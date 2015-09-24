/**
    Auto white balance api.


    @file       awb_api.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_API_H_
#define _AWB_API_H_

/**

     awb init function
*/
extern void AWB_Init(void);

/**

     awb process function
*/
extern void AWB_Process(void);


/**

     awb get current gain.

     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void AWB_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);


/**

     awb write debug information

     @note

     @param[in] buffer start address
     @return
         - @b buffer end address
*/
extern UINT32 AWB_WriteDebugInfo(UINT32 Addr);

/**
    awb re-calculate awb color gain acoording new source.
    @note use AWB_GetColorGian(...) to get new color gain

    @param[in] source image information
    @param[in] DataBit : 8bit -> 8, 10bit -> 10 ...etc.
    @param[in] SubSample: unit:pixel
*/
extern void AWB_ReCalColorGain(UINT32 addr,UINT32 Lineoffset,UINT32 Hsize,UINT32 Vsize,UINT32 DataBit,UINT32 SubRate,UINT32 CFA);

#endif //_AWB_API_H_
