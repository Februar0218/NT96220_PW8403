/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_R2A30423.h
    @ingroup    mISYSLens

    @brief      Renesas R2A30423 driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/09/25
*/

#ifndef _LENSMOTOR_R2A30423_H
#define _LENSMOTOR_R2A30423_H
#include "Type.h"

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_PVT_OBJ
{
    //GPIO related var
    BOOL          bInitYet;
    UINT32        uiPinPower;
    UINT32        uiPinClock;
    UINT32        uiPinReset;
    UINT32        uiPinInput1;
    UINT32        uiPinInput2;
    UINT32        uiPinInput3;
    UINT32        uiPinInput4;
    UINT32        uiPinInput5;
    UINT32        uiPinInput6;

    UINT32        uiPinZoomPI;
    UINT32        uiPinZoomPR;
    UINT32        uiPinFocusPI;
    UINT32        uiZoomPIINT;

    UINT32        uiMotExt1;
    UINT32        uiMotExt2;
    UINT32        uiMotMOB1;
    UINT32        uiMotMOB2;

    //SIF,R2A30423 usage
    UINT32        uiSIFType;
    SIF_CH        uiSIFChannel;
    UINT32        uiGPIOSen;                  ///< GPIO pin number for SEN, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32        uiGPIOClk;                  ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32        uiGPIOData;                 ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO

    //Other info
    BOOL          bWaitZoomFinish;
    BOOL          bOpenZoomPR;
    UINT32        uiZoomPPS;
    UINT32        uiZoomDirection;
    UINT32        uiZoomStep;
    UINT32        uiFocusPPS;
    UINT32        uiFocusDirection;
    UINT32        uiFocusStep;
    MOTOR_ZOOM_ACT  zoomState;
} MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
// This zone is for Motor-driver Register Definition
//--------------------------------------------------------------------------------------------------------------------------
#define ADDRESS_SHIFT_OFFSET            14
#define REGISTER_SHIFT_OFFSET           10
#define VALUE_SHIFT_OFFSET              0

#define Address_12ch_Sel                0x00    //00
//register
    #define R12ch_Mode                  0x00    //0000
    #define R12ch_Pulse1                0x01    //0001
    #define R12ch_Pulse2                0x02    //0010
    #define R12ch_PulseRate0            0x03    //0011
    #define R12ch_PulseRateRange        0x04    //0100
    #define R12ch_NumPulse              0x05    //0101
    #define R12ch_PulseRate1AccDec      0x06    //0110
    #define R12ch_PulseRate2AccDec      0x07    //0111
    #define R12ch_PulseRate3AccDec      0x08    //1000
    #define R12ch_PulseRate4AccDec      0x09    //1001
    #define R12ch_PrePulseGen           0x0A    //1010
    #define R12ch_PostPulseGen          0x0B    //1011
    //Setting prohibited                0x0C    //1100
    #define R12ch_Volt                  0x0D    //1101
    #define R12ch_ReadPulsePos          0x0E    //1110
    #define R12ch_PerCHOpCtrl           0x0F    //1111

#define Address_34ch_Sel                0x01    //01
//register
    #define R34ch_Mode                  0x00
    #define R34ch_Pulse1                0x01
    #define R34ch_Pulse2                0x02
    #define R34ch_PulseRate0            0x03
    #define R34ch_PulseRateRange        0x04
    #define R34ch_NumPulse              0x05
    #define R34ch_PulseRate1AccDec      0x06
    #define R34ch_PulseRate2AccDec      0x07
    #define R34ch_PulseRate3AccDec      0x08
    #define R34ch_PulseRate4AccDec      0x09
    #define R34ch_PrePulseGen           0x0A
    #define R34ch_PostPulseGen          0x0B
    #define R34ch_OutVoltDC             0x0C
    #define R34ch_Volt                  0x0D
    #define R34ch_Prohibit2             0x0E
    #define R34ch_PerCHOpCtrl           0x0F

#define Address_56ch_Sel                0x02
//register
    #define R56ch_Mode                  0x00
    #define R56ch_Pulse1                0x01
    #define R56ch_Pulse2                0x02
    #define R56ch_PulseRate0            0x03
    #define R56ch_PulseRateRange        0x04
    #define R56ch_NumPulse              0x05
    #define R56ch_PulseRate1AccDec      0x06
    #define R56ch_PulseRate2AccDec      0x07
    #define R56ch_PulseRate3AccDec      0x08
    #define R56ch_PulseRate4AccDec      0x09
    #define R56ch_PrePulseGen           0x0A
    #define R56ch_PostPulseGen          0x0B
    #define R56ch_Mode2                 0x0C
    #define R56ch_FSPWMDutyCycle        0x0D
    #define R56ch_Ch6FSPWMDutyCycle     0x0E
    #define R56ch_PerCHOpCtrl           0x0F

#define Address_common_Sel              0x03    //11
//register
    //Setting prohibited                0x00    //0000
    #define Rcomm_ExtMobOut             0x01    //0001
    #define Rcomm_PIOut                 0x02    //0010
    #define Rcomm_PSOut                 0x03    //0011
    //Setting prohibited                0x04    //0100
    #define Rcomm_CtrlInput             0x05    //0101
    #define Rcomm_Ch5CtrlSetting        0x06    //0110
    #define Rcomm_Ch6CtrlSetting        0x07    //0111
    #define Rcomm_SWReset               0x08    //1000
    #define Rcomm_ExclusiveCtrl         0x09    //1001
    #define Rcomm_ReadStatus            0x0A    //1010
    #define Rcomm_Ch12StatusFLCtrl      0x0B    //1011 //?
    #define Rcomm_Ch34StatusFLCtrl      0x0C    //1100
    #define Rcomm_Ch57StatusFLCtrl      0x0D    //1101
    #define Rcomm_ReadPulsePos          0x0E    //1110
    #define Rcomm_OpCtrl                0x0F    //1111

#if 0
#define Address_12ch_DAC_Setting        3
    //Bit 4-1
    #define M12ch_CV_VALUE_MASK         0x1E
    #define M12ch_CV_VALUE_4_8V         0x00
    #define M12ch_CV_VALUE_4_6V         0x02
    #define M12ch_CV_VALUE_4_4V         0x04
    #define M12ch_CV_VALUE_4_2V         0x06
    #define M12ch_CV_VALUE_4_0V         0x08
    #define M12ch_CV_VALUE_3_8V         0x0A
    #define M12ch_CV_VALUE_3_6V         0x0C
    #define M12ch_CV_VALUE_3_4V         0x0E
    #define M12ch_CV_VALUE_3_2V         0x10
    #define M12ch_CV_VALUE_3_0V         0x12
    #define M12ch_CV_VALUE_2_8V         0x14
    #define M12ch_CV_VALUE_2_6V         0x16
    #define M12ch_CV_VALUE_2_4V         0x18
    #define M12ch_CV_VALUE_2_2V         0x1A
    #define M12ch_CV_VALUE_2_0V         0x1C
    #define M12ch_CV_VALUE_1_8V         0x1E
    //Bit 0
    #define M12ch_DRIVE_MODE_MASK       0x1
    #define M12ch_DRIVE_MODE_CV         0x0
    #define M12ch_DRIVE_MODE_FullSwing  0x1
#define Address_3ch_DAC_Setting         4
    //Bit 4-1
    #define M3ch_CV_VALUE_MASK          0x1E
    #define M3ch_CV_VALUE_4_8V          0x00
    #define M3ch_CV_VALUE_4_6V          0x02
    #define M3ch_CV_VALUE_4_4V          0x04
    #define M3ch_CV_VALUE_4_2V          0x06
    #define M3ch_CV_VALUE_4_0V          0x08
    #define M3ch_CV_VALUE_3_8V          0x0A
    #define M3ch_CV_VALUE_3_6V          0x0C
    #define M3ch_CV_VALUE_3_4V          0x0E
    #define M3ch_CV_VALUE_3_2V          0x10
    #define M3ch_CV_VALUE_3_0V          0x12
    #define M3ch_CV_VALUE_2_8V          0x14
    #define M3ch_CV_VALUE_2_6V          0x16
    #define M3ch_CV_VALUE_2_4V          0x18
    #define M3ch_CV_VALUE_2_2V          0x1A
    #define M3ch_CV_VALUE_2_0V          0x1C
    #define M3ch_CV_VALUE_1_8V          0x1E
    //Bit 0
    #define M3ch_DRIVE_MODE_MASK        0x1
    #define M3ch_DRIVE_MODE_CV          0x0
    #define M3ch_DRIVE_MODE_FullSwing   0x1
#define Address_4ch_DAC_Setting         5
    //Bit 4-1
    #define M4ch_CC_VALUE_MASK          0x1E
    #define M4ch_CC_VALUE_300mV         0x00
    #define M4ch_CC_VALUE_290mV         0x02
    #define M4ch_CC_VALUE_280mV         0x04
    #define M4ch_CC_VALUE_270mV         0x06
    #define M4ch_CC_VALUE_260mV         0x08
    #define M4ch_CC_VALUE_250mV         0x0A
    #define M4ch_CC_VALUE_240mV         0x0C
    #define M4ch_CC_VALUE_230mV         0x0E
    #define M4ch_CC_VALUE_220mV         0x10
    #define M4ch_CC_VALUE_210mV         0x12
    #define M4ch_CC_VALUE_200mV         0x14
    #define M4ch_CC_VALUE_190mV         0x16
    #define M4ch_CC_VALUE_180mV         0x18
    #define M4ch_CC_VALUE_170mV         0x1A
    #define M4ch_CC_VALUE_160mV         0x1C
    #define M4ch_CC_VALUE_150mV         0x1E
    //Bit 4-1
    #define M4ch_CV_VALUE_MASK          0x1E
    #define M4ch_CV_VALUE_4_8V          0x00
    #define M4ch_CV_VALUE_4_6V          0x02
    #define M4ch_CV_VALUE_4_4V          0x04
    #define M4ch_CV_VALUE_4_2V          0x06
    #define M4ch_CV_VALUE_4_0V          0x08
    #define M4ch_CV_VALUE_3_8V          0x0A
    #define M4ch_CV_VALUE_3_6V          0x0C
    #define M4ch_CV_VALUE_3_4V          0x0E
    #define M4ch_CV_VALUE_3_2V          0x10
    #define M4ch_CV_VALUE_3_0V          0x12
    #define M4ch_CV_VALUE_2_8V          0x14
    #define M4ch_CV_VALUE_2_6V          0x16
    #define M4ch_CV_VALUE_2_4V          0x18
    #define M4ch_CV_VALUE_2_2V          0x1A
    #define M4ch_CV_VALUE_2_0V          0x1C
    #define M4ch_CV_VALUE_1_8V          0x1E
    //Bit 0
    #define M4ch_DRIVE_MODE_MASK        0x1
    #define M4ch_DRIVE_MODE_CV          0x0
    #define M4ch_DRIVE_MODE_FullSwing   0x1
#define Address_5ch_DAC_Setting         6
    //Bit 4-1
    #define M5ch_CC_VALUE_MASK          0x1E
    #define M5ch_CC_VALUE_300mV         0x00
    #define M5ch_CC_VALUE_290mV         0x02
    #define M5ch_CC_VALUE_280mV         0x04
    #define M5ch_CC_VALUE_270mV         0x06
    #define M5ch_CC_VALUE_260mV         0x08
    #define M5ch_CC_VALUE_250mV         0x0A
    #define M5ch_CC_VALUE_240mV         0x0C
    #define M5ch_CC_VALUE_230mV         0x0E
    #define M5ch_CC_VALUE_220mV         0x10
    #define M5ch_CC_VALUE_210mV         0x12
    #define M5ch_CC_VALUE_200mV         0x14
    #define M5ch_CC_VALUE_190mV         0x16
    #define M5ch_CC_VALUE_180mV         0x18
    #define M5ch_CC_VALUE_170mV         0x1A
    #define M5ch_CC_VALUE_160mV         0x1C
    #define M5ch_CC_VALUE_150mV         0x1E
    //Bit 0
    //DO NOT SET THIS BIT
#define Address_PSRESET_PI_Setting      7
    //Bit 4
    #define Power_MASK                  0x10
    #define Power_OFF                   0x00       //Standby
    #define Power_ON                    0x10
    //Bit 3
    #define MPI1_MASK                   0x8   //(Out: Hi-Z)
    #define MPI1_OFF                    0x0   //(Out: Hi-Z)
    #define MPI1_ON                     0x8   //(Out: L)
    //Bit 2
    #define MPI2_MASK                   0x4
    #define MPI2_OFF                    0x0   //(Out: Hi-Z)
    #define MPI2_ON                     0x4   //(Out: L)
    //Bit 1
    #define MPI3_MASK                   0x2
    #define MPI3_OFF                    0x0   //(Out: Hi-Z)
    #define MPI3_ON                     0x2   //(Out: L)
    //Bit 0
    #define Motor_Reset_MASK            0x1
    #define Motor_Reset_None            0x0   //Do Not thing
    #define Motor_Reset_Do              0x1   //Register area clear
#endif

#endif

