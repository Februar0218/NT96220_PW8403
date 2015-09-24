/**
    @file       EAC.h
    @ingroup    mIAudEAC

    @brief      Header file for DAI module
                This file is the header file that define the API and data type
                for DAI module

    @note       Nothing.


    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

*/

#ifndef _EAC_H
#define _EAC_H

#include "Type.h"


/**
    @addtogroup mIAudEAC
*/
//@{

/**
    DAC output path selection

    DAC output path selection, this is used in eac_setDACOutput().
*/
typedef enum
{
    EAC_OUTPUT_SPK,         ///< DAC output to Speaker
    EAC_OUTPUT_LINE,        ///< DAC output to Line Out
    //#NT#2011/08/31#Klins Chen -begin
    EAC_OUTPUT_SPK_L,       ///< DAC output to speaker Left channel out only.
    EAC_OUTPUT_SPK_R,       ///< DAC output to speaker Right channel out only.
    EAC_OUTPUT_NONE,        ///< None of DAC output

    ENUM_DUMMY4WORD(EAC_OUTPUT)
    //#NT#2011/08/31#Klins Chen -end
} EAC_OUTPUT;

/**
    DC Cancellation Type definition

    This field is used in eac_configDccan() to specify the DCCAN initial behavior.
*/
typedef enum
{
    EAC_DCCAN_TYPE0,        ///< DCCAN Type0: DCCAN would reset the current value after seting eac_setADCPwr(FALSE).
    EAC_DCCAN_TYPE1,        ///< DCCAN Type1: DCCAN would not reset the current value.
    EAC_DCCAN_TYPE2,        ///< DCCAN Type2: DCCAN would not reset the current value, and assign 96220 optimal DC initial value to controller.
    EAC_DCCAN_TYPE3,        ///< DCCAN Type3: Assign 96220 optimal DC initial value to controller and DCCAN would reset the current value after seting eac_setADCPwr(FALSE)

    //#NT#2011/08/31#Klins Chen -begin
    ENUM_DUMMY4WORD(EAC_DCCAN_TYPE)
    //#NT#2011/08/31#Klins Chen -end
} EAC_DCCAN_TYPE;

// API
extern void eac_init(void);
extern BOOL eac_isDACEnable(void);
extern void eac_setLoutVolume(UINT32 uiVol);
extern void eac_setSpkVolume(UINT32 uiVol);
extern void eac_setDigitalVolume(UINT32 uiVol);
extern void eac_setDigitalGain(UINT32 uiGain);
extern void eac_setAnalogGain(UINT32 uiGain);
//#NT#2011/07/22#Klins Chen -begin
//#NT#Modify API naming
extern void eac_setALCMaxGain(UINT32 uiMaxgain);
extern void eac_setALCTarget(UINT32 uiTarget);
extern void eac_setALCNoiseThd(UINT32 uiNoiseThd);
extern void eac_setALCNoiseFactor(UINT32 uiFactor);
extern void eac_setNG(BOOL NoiseGate_en);
//#NT#2011/07/22#Klins Chen -end
extern void eac_setALC(BOOL bAlcEn);
extern void eac_setBoostA(BOOL bBoostA_en);
extern void eac_setBoostD(BOOL bBoostD_en);
extern void eac_setDACPwr(BOOL bEn);
//#NT#2011/06/10#Klins Chen -start
//#NT#Added for support the selected path clock always on
extern void eac_selectDACMode(void);
extern BOOL eac_isDACReset(void);
//#NT#2011/06/10#Klins Chen -end
extern void eac_setDACOutput(EAC_OUTPUT EacOutput, BOOL bEn);
extern void eac_setDACReset(BOOL bEn);
extern void eac_setADCPwr(BOOL bEn);
extern void eac_setADCReset(BOOL bEn);
extern BOOL eac_sendCommand(UINT32 uiReg, UINT32 uiData);
extern BOOL eac_readData(UINT32 uiReg, UINT32 *puiData);
extern BOOL eac_isALCEnable(void);
extern void eac_setDacSilence(BOOL bSliceMode);
extern UINT32 eac_getDccanOfs(void);
extern void eac_setDccanInitVal(UINT32 uiDCInit);
extern void eac_setDccan(BOOL bDccanEn);
extern void eac_setDigitalGain2(UINT32 uiGain);
extern void eac_setDccanRst(BOOL bDccanRst);
extern void eac_setADCPwrA(BOOL bEn);
extern void eac_setADCPwrD(BOOL bEn);
extern void eac_configDccan(EAC_DCCAN_TYPE Type);

//@}

#endif
