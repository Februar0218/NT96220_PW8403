/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMapphing.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of GPIO mapping
                This file is the header file of GPIO mapping

    @note       Nothing.
    @date       2005/12/05
*/

#ifndef _INTERFACE_H
#define _INTERFACE_H

extern char *pFwVersion;
extern char *pMakerString;
extern char *pModelString;

BOOL InterFace_CheckPrjFile(void);

void InterFace_KeyScan_PlaySound(UINT32 uiTempKeyStatus);

UINT32 InterFace_uiNoiseFilterLevel(void);

void InterFace_SysInit_InitAudio(void);

void InterFace_InitAvi_SysInit_InitAudio(void);
void InterFace_Aud_SetFeature_Off(void);

void InterFace_Get_WakeUp_BackLight_Key(UINT32* TempKeyStatus);

void InterFace_KeyScan_DetModedial(BOOL bForceChangeMode);

void InterFace_Dac_SetPgaGain(void);

void InterFace_VolDet_SetBatteryLevel_Record(void);

void InterFace_VolDet_SetBatteryLevel_RecordStop(void);

#endif
